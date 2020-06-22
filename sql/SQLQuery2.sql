create procedure sp_giveback
@cus_id varchar(30),
@cd_id varchar(30)
as
begin
  if(not exists(select * from Stock where stock_id=@cd_id))
  begin
    raiserror('库中不存在此CD！',16,1)
  end
  if(not exists(select * from Trade where stock_id=@cd_id))
  begin
    raiserror('此CD处于空闲状态！',16,1)
  end
  declare @othercus_id varchar(30)
  declare @tradetype bit
  set @othercus_id=(select cus_id from Trade where stock_id=@cd_id)
  set @tradetype=(select trade_type from Trade where stock_id=@cd_id)
  begin transaction
  delete from Trade where stock_id=@cd_id
  if(@othercus_id<>@cus_id)
  begin
    print '此CD已被其他顾客购买或正在被其他顾客租用！'
    rollback transaction
  end
  else if(@tradetype<>0)
  begin
    print '你已购买此CD，不能归还！'
    rollback transaction
  end
  else
  begin
    print '归还CD：'+@cd_id+'成功！'
    commit transaction
  end
end