create table Supplier
(
    sup_id integer check(sup_id>0),
	sup_name varchar(30) not null,
	addr varchar(50),
	phone varchar(20),
	primary key(sup_id)
);

create table Customer
(
    cus_id varchar(30) not null,
	cus_password varchar(30) not null,
	balance decimal(20,2) check(balance>=0) not null,
	register_date datetime default(getdate()),
	primary key(cus_id)
);

create table Administrator
(
    admin_id integer check(admin_id>0),
	admin_password varchar(30) not null,
	admin_name varchar(30) not null,
	register_date datetime default(getdate()),
	primary key(admin_id)
);

create table Genre
(
    genre_id integer check(genre_id>=0),
	genre_name varchar(30),
	primary key(genre_id)
);

create table CDinfo
(
    cd_id integer check(cd_id>0),
	cd_name varchar(50),
	artist_name varchar(30),
	genre_id integer default(0) check(genre_id>=0) not null,
	primary key(cd_id),
	unique(cd_name,artist_name,genre_id),
    foreign key(genre_id) references Genre(genre_id)
);

create table Stock
(
    stock_id varchar(30) not null,
	admin_id integer not null,
	sup_id integer not null,
	cd_id integer not null,
	pur_price decimal(20,2) check(pur_price>0) not null,
	lent_count integer default(0) check(lent_count>=0) not null,
	pur_date datetime default(getdate()),
	primary key(stock_id),
	foreign key(admin_id) references Administrator(admin_id),
	foreign key(sup_id) references Supplier(sup_id),
	foreign key(cd_id) references CDinfo(cd_id)
);

create table Trade
(
    trade_id varchar(30) not null,
	cus_id varchar(30),
	stock_id varchar(30) not null,
	trade_type bit default(0) not null,
	trade_date datetime default(getdate()),
	primary key(trade_id),
	foreign key(cus_id) references Customer(cus_id) on delete set null,
	foreign key(stock_id) references Stock(stock_id)
);