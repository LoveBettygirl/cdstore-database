create view CusCDlist as
select s1.stock_id as CD��,cd_name as CD��,
artist_name as ������,genre_name as ���,
pur_price*1.2 as �ۼ�,pur_price*1.2*0.25 as ���ü۸�,
pur_date as ���ʱ��,lent_count as ���ô���
from Stock s1,CDinfo,Genre
where s1.cd_id=CDinfo.cd_id and 
CDinfo.genre_id=Genre.genre_id and 
not exists(select * from Trade 
where Trade.stock_id=s1.stock_id)