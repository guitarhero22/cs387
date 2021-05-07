
drop table if exists testmultiple;
drop extension lsm3 cascade;
create extension lsm3;
create table testmultiple(
	keycol varchar(64),
	valcol varchar(64)
);