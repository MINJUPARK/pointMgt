CREATE DATABASE point_db default CHARACTER SET UTF8; 

use point_db;

create table point (
	hakbun int(50) primary key not null,
	name char(50) not null,
	p1 int(50) not null default 0,
	p2 int(50) not null default 0,
	p3 int(50) not null default 0,
	p4 int(50) not null default 0,
	p5 int(50) not null default 0,
	p6 int(50) not null default 0,
	p7 int(50) not null default 0,
	p8 int(50) not null default 0,
	sum int(50) not null default 0);