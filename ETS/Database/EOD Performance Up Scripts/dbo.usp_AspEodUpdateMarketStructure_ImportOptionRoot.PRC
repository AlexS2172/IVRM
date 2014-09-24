SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ImportOptionRoot]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ImportOptionRoot]
GO


CREATE PROC dbo.usp_AspEodUpdateMarketStructure_ImportOptionRoot
	@iUnderlyingID int,
	@vcSymbol varchar(20),
	@iLotSize int,
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	declare @error int set @error = 0
	-------------------------------------------------
	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	declare @iOptionRootID int, @iUnderlyingID_bad int

    -- This is import for all roots
    if @iUnderlyingID is null and @vcSymbol is null begin 
      
      declare @bad_symbols table (symbol varchar(20),underlyingID int)
      
      insert into @bad_symbols(symbol,underlyingID) 
      select o.symbol,max(r.underlyingID) underlyingID 
      from OptionRoot o,EodOptionRoot r (nolock) 
      where r.Symbol COLLATE database_default = o.Symbol  
            and o.actionID < 3 
      group by o.symbol
      having count(*) > 1
   
      -- update bad symbols        
      update OptionRoot
        set symbol = OptionRoot.symbol + '*' 
      from @bad_symbols n
      where n.symbol COLLATE database_default = OptionRoot.symbol and actionID < 3 and n.underlyingID <> OptionRoot.underlyingID

       if @@error <> 0 begin 
         exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option root: Fail to mark option root as invalid', @iErrorCode = @@error
	 if @iWithTran is not null rollback tran @vcTranName
	 set @error = -1
	 goto finish
       end

       -- insert new symbols 
      insert into OptionRoot(underlyingID, symbol, isSynthetic, lotSize)
      select eor.underlyingID, eor.symbol, 0, eor.lotSize 
      from EodOptionRoot eor (nolock) 
      where not exists(select 1 
                       from OptionRoot r with(nolock) 
                       where eor.symbol COLLATE database_default = r.symbol and actionId < 3 )
            and eor.symbol COLLATE database_default not in (select symbol from @bad_symbols)
      union all
      select eor.underlyingID, eor.symbol, 0, eor.lotSize 
      from EodOptionRoot eor (nolock) 
         inner join @bad_symbols n on (eor.symbol COLLATE database_default = n.symbol and n.underlyingID = eor.underlyingID)
      where not exists(select 1 
                       from OptionRoot r with(nolock) 
                       where eor.symbol COLLATE database_default = r.symbol and actionId < 3 )


      if @@error <> 0 begin 
        exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option root: Fail to create new option root', @iErrorCode = @@error
	if @iWithTran is not null rollback tran @vcTranName
	set @error = -2
	goto finish
      end
      
       set @iOptionRootID = @error 
    end 
    else
    -- import for only one root
      begin 
	select @iOptionRootID = optionRootID,
	       @iUnderlyingID_bad = underlyingID
	from OptionRoot with(nolock, index = IX_OptionRoot_underlyingID_symbol_actionID)
	where actionID < 3 and symbol = @vcSymbol
			--and underlyingID != @iUnderlyingID
	
	 if @iOptionRootID is not null and @iUnderlyingID_bad != @iUnderlyingID
	   begin
	     update OptionRoot
		set symbol = symbol + '*'
	      where optionRootID = @iOptionRootID
	
              if @@error != 0
		begin
		  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option root: Fail to mark option root as invalid', @iErrorCode = @@error
		  if @iWithTran is not null rollback tran @vcTranName
		    set @error = -1
		    goto finish
		  end
	       	  set @iOptionRootID = null
	      end  
		
		/*select @iOptionRootID = optionRootID
		from OptionRoot with(nolock, index = IX_OptionRoot_underlyingID_symbol_actionID)
		where actionID < 3
			and symbol = @vcSymbol
			and underlyingID = @iUnderlyingID*/
	
	      if @iOptionRootID is null
		begin
         	  insert into OptionRoot(underlyingID, symbol, isSynthetic, lotSize)
		  values(@iUnderlyingID, @vcSymbol, 0, @iLotSize)
	
		  if @@error != 0
		    begin
		      exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option root: Fail to create new option root', @iErrorCode = @@error
		      if @iWithTran is not null rollback tran @vcTranName
		      set @error = -2
		      goto finish
		    end
	
		   set @iOptionRootID = @@identity
		end
       end

  if @iWithTran is not null commit tran @vcTranName
  finish:
    if @error = 0
      return @iOptionRootID
    else
      return @error
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

