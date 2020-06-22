create procedure sp_trade
@cus_id varchar(30),
@stock_id varchar(30),
@trade_type bit,
@trade_price float output,
@trade_balance float output
as
begin
  if(not exists(select * from Stock where stock_id=@stock_id))
  begin --店里没有相应的CD
    select @trade_price=0
	select @trade_balance=0
    return -1
  end
  else if(exists(select * from Trade where stock_id=@stock_id and cus_id<>@cus_id))
  begin  --其他顾客已购买或正在租用CD
    select @trade_price=0
	select @trade_balance=0
    return 0
  end
  else if(exists(select * from Trade where cus_id=@cus_id and stock_id=@stock_id and trade_type=1))
  begin --自己已经购买CD
    select @trade_price=0
	select @trade_balance=0
	return 1
  end
  else
  begin
    declare @balance decimal(20,2)
	declare @sell_price decimal(20,2)
	declare @lent_price decimal(20,2)
	set @balance=(select balance from Customer where cus_id=@cus_id)
	set @sell_price=(select pur_price from Stock where stock_id=@stock_id)*1.2
	set @lent_price=@sell_price*0.25
	if(exists(select * from Trade where cus_id=@cus_id and stock_id=@stock_id and trade_type=0))
	begin
	  if(@trade_type=1)--购买自己正在租用的CD
	  begin
	    if(@balance>=3*@lent_price)
	    begin
		  update Customer set balance=balance-3*@lent_price where cus_id=@cus_id
	      update Trade set trade_type=1 where stock_id=@stock_id and cus_id=@cus_id
	      set @balance=(select balance from Customer where cus_id=@cus_id)
	      select @trade_price=cast(3*@lent_price as float)
	      select @trade_balance=cast(@balance as float)
	      return 2
		end
		else
		begin
		  select @trade_price=cast(3*@lent_price as float)
	      select @trade_balance=cast(@balance as float)
	      return 3
		end
	  end
	  else  --自己已经正在租用此CD，不能重复租用
	  begin
	    select @trade_price=0
	    select @trade_balance=0
	    return 4
	  end
	end
	else if(@trade_type=0 and @balance>=@lent_price)
	begin
	  update Customer set balance=balance-@lent_price where cus_id=@cus_id
	  update Stock set lent_count=lent_count+1 where stock_id=@stock_id
	  set @balance=(select balance from Customer where cus_id=@cus_id)
	  select @trade_price=cast(@lent_price as float)
	  select @trade_balance=cast(@balance as float)
	  return 5
	end
	else if(@trade_type=1 and @balance>=@sell_price)
	begin
	  update Customer set balance=balance-@sell_price where cus_id=@cus_id
	  set @balance=(select balance from Customer where cus_id=@cus_id)
	  select @trade_price=cast(@sell_price as float)
	  select @trade_balance=cast(@balance as float)
	  return 6
	end
	else if(@trade_type=0 and @balance<@lent_price)
	begin
	  select @trade_price=cast(@lent_price as float)
	  select @trade_balance=cast(@balance as float)
	  return 7
	end
	else if(@trade_type=1 and @balance<@sell_price)
	begin
	  select @trade_price=cast(@sell_price as float)
	  select @trade_balance=cast(@balance as float)
	  return 8
	end
  end
end