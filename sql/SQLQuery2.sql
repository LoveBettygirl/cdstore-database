create procedure sp_giveback
@cus_id varchar(30),
@cd_id varchar(30)
as
begin
  if(not exists(select * from Stock where stock_id=@cd_id))
  begin
    raiserror('���в����ڴ�CD��',16,1)
  end
  if(not exists(select * from Trade where stock_id=@cd_id))
  begin
    raiserror('��CD���ڿ���״̬��',16,1)
  end
  declare @othercus_id varchar(30)
  declare @tradetype bit
  set @othercus_id=(select cus_id from Trade where stock_id=@cd_id)
  set @tradetype=(select trade_type from Trade where stock_id=@cd_id)
  begin transaction
  delete from Trade where stock_id=@cd_id
  if(@othercus_id<>@cus_id)
  begin
    print '��CD�ѱ������˿͹�������ڱ������˿����ã�'
    rollback transaction
  end
  else if(@tradetype<>0)
  begin
    print '���ѹ����CD�����ܹ黹��'
    rollback transaction
  end
  else
  begin
    print '�黹CD��'+@cd_id+'�ɹ���'
    commit transaction
  end
end