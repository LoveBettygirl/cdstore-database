create view CusCDlist as
select s1.stock_id as CD号,cd_name as CD名,
artist_name as 作者名,genre_name as 类别,
pur_price*1.2 as 售价,pur_price*1.2*0.25 as 租用价格,
pur_date as 入库时间,lent_count as 租用次数
from Stock s1,CDinfo,Genre
where s1.cd_id=CDinfo.cd_id and 
CDinfo.genre_id=Genre.genre_id and 
not exists(select * from Trade 
where Trade.stock_id=s1.stock_id)