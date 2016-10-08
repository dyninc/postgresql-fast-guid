-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION fast_guid" to load this file. \quit

CREATE FUNCTION fast_guid(SMALLINT, SMALLINT) RETURNS bigint
AS 'MODULE_PATHNAME', 'fast_guid'
LANGUAGE C STRICT;

