SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_DeleteOption]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AspEodUpdateMarketStructure_DeleteOption]
GO


CREATE PROC dbo.usp_AspEodUpdateMarketStructure_DeleteOption
	@iOptionID int,
	@iWithTran int = null
as
	-------------------------------------------------
	set nocount on
	declare @error int set @error = 0
	-------------------------------------------------
	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName
        declare @ContractsForDelete table (optID int, UseInTrade int)
         
     -- Need #options table!
      if @iOptionID is null begin
           
        insert into @ContractsForDelete
        select c.ContractID, 0 
        from #options o
          inner join Contract c on (c.symbol = o.symbol and c.contractID <> o.optID)
        where o.OptID is not null  and c.ContractTypeid = 3 and c.actionID < 3
         
        update @ContractsForDelete
          set UseInTrade = 1
        where exists(select 1 from EodContractWithTrades o (nolock) where o.contractID = optID) 
         
	delete OptionDataCache 
        from @ContractsForDelete c 
        where optionID = OptID and c.UseInTrade = 0 
        if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	delete ContractPrice 
        from @ContractsForDelete c 
        where contractID = OptID and c.UseInTrade = 0 
        if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	delete ContractMarketData 
        from @ContractsForDelete c 
        where contractID = OptID and c.UseInTrade = 0 
       	if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	delete DualQuadraticParams 
        from @ContractsForDelete c 
        where contractID = OptID and c.UseInTrade = 0 
        if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	delete [Option] 
        from @ContractsForDelete c 
        where optionID = OptID and c.UseInTrade = 0 
	if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	delete [Contract] 
        from @ContractsForDelete c 
        where contractID = OptID and c.UseInTrade = 0 
	if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
         
	update Contract
          set symbol = symbol + '*'
        from @ContractsForDelete c           
	where contractID = c.OptID and c.UseInTrade = 1
	if @@error != 0 begin set @error = -2 if @iWithTran is not null rollback tran @vcTranName goto finish end
        
      end
      else begin  
	if not exists (select 1 from EodContractWithTrades (nolock) where contractID = @iOptionID)
	begin
		delete OptionDataCache where optionID = @iOptionID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
		delete ContractPrice where contractID = @iOptionID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
		delete ContractMarketData where contractID = @iOptionID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
		delete DualQuadraticParams where contractID = @iOptionID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
		delete [Option] where optionID = @iOptionID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
		delete Contract where contractID = @iOptionID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
	end
	else
	begin
		update Contract
		set symbol = symbol + '*'
		where contractID = @iOptionID
			if @@error != 0 begin set @error = -2 if @iWithTran is not null rollback tran @vcTranName goto finish end
	end
      end

	if @iWithTran is not null commit tran @vcTranName

finish:
	return @error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

