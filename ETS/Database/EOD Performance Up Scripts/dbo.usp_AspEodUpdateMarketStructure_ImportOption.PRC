SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ImportOption]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ImportOption]
GO




/* ---------------------------- */

CREATE    PROC dbo.usp_AspEodUpdateMarketStructure_ImportOption
	@iRootID int,
	@dtExpiryDate datetime,
	@fStrike float,
	@iIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@fPriceClose float,
	@dtExpiryOV datetime = null,
	@dtTradeClose datetime = null,
	@iWithTran int = null
as
	-------------------------------------------------
	set nocount on
	declare @error int set @error = 0
	-------------------------------------------------
	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName
		
	declare	@iOptionID int,
		@iOptionID_bad int
      
        if object_id('tempdb..#options') is not null
          drop table #options
       
        create table #options
        ( optID int null,
          optRootid int null,
          symbol varchar(20) null,
	  ExpiryDate datetime null,
	  Strike float null,
	  IsCall tinyint null,
	  ContractName varchar(255) null,
	  PriceClose float null,
	  ExpiryOV datetime  null,
	  TradeClose datetime null,
        )
        create clustered index OptID_options on #options(OptID)
        create index Symbol_options on #options(Symbol)

        -- This is for all options
        if @iRootID is null and @dtExpiryDate is null and @fStrike is null and @iIsCall is null and @vcSymbol is null begin

          insert into #options(optID, optRootid, symbol, expiryDate, strike, isCall, contractName, priceClose, ExpiryOV, TradeClose)            
	  select o.OptionId, r2.OptionRootId , e.symbol, e.expiryDate, e.strike, e.isCall, e.contractName, e.priceClose, e.ExpiryOV, e.TradingClose
	  from EodOption e with (nolock)
            inner join EodOptionRoot r on (e.eodOptionRootID = r.eodOptionRootID)
            inner join OptionRoot r2 on (r.UnderlyingID = r2.UnderlyingID and r.symbol COLLATE database_default = r2.symbol and r2.actionID < 3)
            left join [Option] o  on (o.optionRootID = r2.optionRootid
                   			and o.isCall =e.IsCall 
			                and abs(o.strike - e.Strike) < 0.009
                   			and abs(datediff(d, o.expiryDate, e.ExpiryDate))<10 
			                and o.actionID < 3) 

          if @@error <> 0 begin 
	    if @iWithTran is not null rollback tran @vcTranName
	    set @error = -1
	    goto finish
          end 

          -- Delete bad options
	  exec @error = dbo.usp_AspEodUpdateMarketStructure_DeleteOption
					@iOptionID = null, @iWithTran = null
          if @error <> 0 begin 
	    if @iWithTran is not null rollback tran @vcTranName
	    set @error = -1
	    goto finish
          end 

	  update Contract
            set
		symbol = o.Symbol,
		contractName = o.ContractName
          from #options o
	  where contractID = o.OptID and actionID < 3 and contracttypeid = 3

	  update [Option]
	    set isManualyEntered = 0,
		expiryDate = o.ExpiryDate,
		expiryOV = o.ExpiryOV,
		TradingClose = o.TradeClose 
          from #options o
	  where optionID = o.OptID and actionID < 3

	 insert into Contract(contractTypeID, symbol, contractName)
         select distinct 3, Symbol, ContractName
         from #options
         where optID is null

         insert into [Option](optionID, optionRootID, isCall, strike, expiryDate, ExpiryOV, TradingClose)
 	 select	c.ContractID, max(o.optRootid), max(o.IsCall), max(o.Strike), max(o.ExpiryDate), max(o.ExpiryOV), max(o.TradeClose)
         from #options o
           inner join dbo.Contract c on (o.symbol = c.symbol and o.ContractName = c.ContractName and c.contracttypeid = 3)
         where optID is null and c.actionID < 3 and not exists(select 1 from [option] o where c.ContractID = o.OptionID )
         group by c.ContractId        

	insert into ContractPrice(contractID, exchangeID, priceClose)
        select distinct c.contractId, null, o.PriceClose 
         from #options o
           inner join dbo.Contract c on (o.symbol = c.symbol and o.ContractName = c.ContractName and c.contracttypeid = 3 and c.actionID < 3)
        where not exists(select 1 from ContractPrice c2 where c.ContractID = c2.contractID and c2.exchangeID is null ) and optID is null 

        update c
          set priceClose = o.priceClose
        from ContractPrice c
          inner join Contract c2 on (c.ContractID = c2.ContractId)
          inner join #options o on (c2.ContractID = o.optID)
        where c.exchangeID is null and c2.contracttypeid = 3
  
        end else
        -- For only one option
        begin 
		select @iOptionID = optionID
		from [Option] with(nolock, index = IX_Option_optionRootID_isCall_strike_expiryID_actionID)
		where optionRootID = @iRootID
			and isCall = @iIsCall
			and abs(strike - @fStrike) < 0.009
			and abs(datediff(d, expiryDate, @dtExpiryDate))<10 
			and actionID < 3
	
		if @iOptionID is not null
		begin
			select @iOptionID_bad = contractID
			from Contract with(nolock)
			where symbol = @vcSymbol
				--and contractID != @iOptionID
				and contractTypeID = 3
				and actionID < 3
	
			if @iOptionID_bad is not null and @iOptionID_bad != @iOptionID
			begin
				exec @error = dbo.usp_AspEodUpdateMarketStructure_DeleteOption
								@iOptionID = @iOptionID_bad, @iWithTran = @iWithTran
	
				if @@error != 0 or @error != 0
				begin
					exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to delete existing option', @iErrorCode = @@error
					if @iWithTran is not null rollback tran @vcTranName
					set @error = -1
					goto finish
				end
			end
			
			update Contract set
				symbol = @vcSymbol,
				contractName = @vcContractName
			where contractID = @iOptionID
				and actionID < 3
	
			if @@error != 0 or @error != 0
			begin
				exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to update option contract info', @iErrorCode = @@error
				if @iWithTran is not null rollback tran @vcTranName
				set @error = -2
				goto finish
			end
			
	
			update [Option]
			set 	isManualyEntered = 0,
				expiryDate = @dtExpiryDate,
				expiryOV = @dtExpiryOV,
				TradingClose = @dtTradeClose 
			where optionID = @iOptionID
				and actionID < 3
	
			if @@error != 0 or @error != 0
			begin
				exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to update option info', @iErrorCode = @@error
				if @iWithTran is not null rollback tran @vcTranName
				set @error = -3
				goto finish
			end
		end
		else
		begin
			select @iOptionID_bad = contractID
			from Contract with(nolock)
			where symbol = @vcSymbol
				and contractTypeID = 3
				and actionID < 3
	
			if @iOptionID_bad is not null
			begin
				exec @error = dbo.usp_AspEodUpdateMarketStructure_DeleteOption
								@iOptionID = @iOptionID_bad, @iWithTran = @iWithTran
	
				if @@error != 0 or @error != 0
				begin
					exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to delete existing option', @iErrorCode = @@error
					if @iWithTran is not null rollback tran @vcTranName
					set @error = -4
					goto finish
				end
			end
	
			insert into Contract
				(contractTypeID, symbol, contractName)
			values	(3, @vcSymbol, @vcContractName)
	
			if @@error != 0 or @error != 0
			begin
				exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to insert new option contract', @iErrorCode = @@error
				if @iWithTran is not null rollback tran @vcTranName
				set @error = -5
				goto finish
			end
	
			set @iOptionID = @@identity
	
			insert into [Option]
				(optionID, optionRootID, isCall, strike, expiryDate, ExpiryOV, TradingClose)
			values	(@iOptionID, @iRootID, @iIsCall, @fStrike, @dtExpiryDate, @dtExpiryOV, @dtTradeClose) 
			if @@error != 0 or @error != 0
			begin
				exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to insert new option', @iErrorCode = @@error
				if @iWithTran is not null rollback tran @vcTranName
				set @error = -6
				goto finish
			end
		end 
		 
		if not exists(select 1 from ContractPrice (nolock) where contractID = @iOptionID and exchangeID is null)
		begin
			insert into ContractPrice
				(contractID, exchangeID, priceClose)
			values (@iOptionID, null, @fPriceClose)
		end 
		else
		begin
			update ContractPrice
			set priceClose = @fPriceClose
			where contractID = @iOptionID
				and exchangeID is null
		end
        end

 
	if @@error != 0 or @error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to update option close price', @iErrorCode = @@error
		if @iWithTran is not null rollback tran @vcTranName
		set @error = -7
		goto finish
	end

  if object_id('tempdb..#options') is not null
    drop table #options

  if @iWithTran is not null commit tran @vcTranName

finish:
	if @error = 0
		return @iOptionID
	else
		return @error





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

