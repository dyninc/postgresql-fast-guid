#include "postgres.h"
#include "fmgr.h"
#include "access/xlog.h"
#include "miscadmin.h"
#include "replication/walreceiver.h"
#include "storage/ipc.h"
#include "storage/lwlock.h"
#include "storage/procarray.h"
#include "storage/shmem.h"
#include "storage/spin.h"
#include "utils/builtins.h"
#include "utils/timestamp.h"
#include <sys/time.h>

PG_MODULE_MAGIC;

void		_PG_init(void);
void		_PG_fini(void);

PG_FUNCTION_INFO_V1(fast_guid);

Datum		fast_guid(PG_FUNCTION_ARGS);

void
_PG_init(void)
{
}

void
_PG_fini(void)
{
}

/*
SELECT
    (
        (
            (
                FLOOR(
                    EXTRACT(
                        EPOCH FROM clock_timestamp()
                    ) * 1000
                )::bigint  - 1314220021721
            ) << 23
        ) | (1 << 1024)
    ) | ( nextval('shard_1.global_id_sequence') % 1024 );
*/
Datum
fast_guid(PG_FUNCTION_ARGS)
/* patterned after
 * http://instagram-engineering.tumblr.com/post/10853187575/sharding-ids-at-instagram
 *
 * 41 bits for time-in-milliseconds
 * 13 bits for logical shard ID
 * 10 bits for autoincrementing sequences modulus 1024 (1024 IDs per
 *   shard per MS)
 *
 */
// (Jan 1st, 2011 in epoch seconds)
#define INSTAGRAM_EPOCH_START 1293840000
{
    uint16_t shard_id = PG_GETARG_INT16(0);
    uint16_t seq_id = PG_GETARG_INT16(1);
    struct timeval tp;
    uint64_t res;

    gettimeofday(&tp, NULL);

    res = tp.tv_sec - INSTAGRAM_EPOCH_START;
    res *= 1000; /* turn seconds into MS */
    res += tp.tv_usec / 1000; /* add in MS */

    res = (res << (64-41)); /* fill in the 41 left-most bits */
    res |= ((shard_id % 8192) << 13); /* fill in the next 13 bits with the shard ID */
    res |= (seq_id % 1024); /* fill in the last 10 bits w/the seq ID */

    /*
    res =
    (
        (
            (
                (tp.tv_sec - INSTAGRAM_EPOCH_START) * 1000
            ) + (tp.tv_usec / 1000)
        ) << (64-41)
    ) | ((shard_id & (1 << 13)) << (64-41-13)) | (seq_id % 1024);
    */

	PG_RETURN_INT64(res);
}


