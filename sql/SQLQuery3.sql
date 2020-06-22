/*declare @trade_price float
declare @trade_balance float
declare @return integer
exec @return=sp_trade '20190516_1','20190516_4',0,@trade_price output,@trade_balance output
select @trade_price,@trade_balance,@return
go*/

--insert into Trade values('20190516_8','20190516_1','20190516_4',0,getdate())
--update Customer set balance=balance+50 where cus_id='20160715_1'
--select * from Customer
select * from Trade
--select * from Genre
--select * from Administrator
--select * from CusCDlist
--select * from CusCDlist
--update Stock set lent_count=2 where stock_id='20190514_2'