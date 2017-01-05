I have authored a postgresql extension module for generating fast GUIDs.

It is patterned after publicly available information and blog posts:

http://rob.conery.io/2014/05/29/a-better-id-generator-for-postgresql/
and
http://instagram-engineering.tumblr.com/post/10853187575/sharding-ids-at-instagram

and basically converts the pl/pgsql function into a C one.
It was a fun distraction for me over an hour or two over a weekend.

Jon (jdnelson@dyn.com)

