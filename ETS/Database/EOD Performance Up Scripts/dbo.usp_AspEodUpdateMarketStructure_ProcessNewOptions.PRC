SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ProcessNewOptions]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ProcessNewOptions]
GO





/* ---------------------------- */

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_ProcessNewOptions 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	truncate table EodOptionRoot

	insert EodOptionRoot (eodOptionRootID, underlyingID, symbol, lotSize)
	select
		eod_ort.optionRootID,
		eod_um.contractID,
		eod_ort.symbol,
		eod_ort.lotSize
	from ETS_ASP_EOD.dbo.OptionRoot eod_ort
		inner join EodUnderlyingMap eod_um
				on eod_ort.underlyingID = eod_um.eodContractID

	truncate table EodOption

	insert EodOption (eodOptionRootID, symbol, contractName, expiryDate, strike, isCall, priceClose, ExpiryOV, TradingClose)
	select	eod_o.optionRootID,
		eod_c.symbol,
		eod_c.contractName,
		eod_o.expiryDate,
		eod_o.strike,
		eod_o.isCall,
		eod_cp.priceClose,
		eod_o.ExpiryOV,
		eod_o.TradingClose
	from	ETS_ASP_EOD.dbo.[Option] eod_o
		inner join ETS_ASP_EOD.dbo.Contract eod_c
			on eod_c.contractID = eod_o.optionID
		left join ETS_ASP_EOD.dbo.ContractPrice eod_cp
			on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null

	declare @eodOptionRootID int,
		@underlyingID int,
		@symbol varchar(20),
		@lotSize int,
		@expiryDate datetime,
		@strike float,
		@isCall tinyint,
		@contractName varchar(255),
		@priceClose float,
		@optionID int,
		@dtOVExpiry datetime,
		@dtCloseTrade datetime,
		@optionRootID int

         
         -- import for all option roots    
         exec @error = dbo.usp_AspEodUpdateMarketStructure_ImportOptionRoot
							@iUnderlyingID = null, @vcSymbol = null, @iLotSize = null
         if @error <> 0 begin 
	   if @iWithTran is not null rollback tran @vcTranName
	   goto finish
         end 

         -- import for all options
	 exec @error = dbo.usp_AspEodUpdateMarketStructure_ImportOption
							@iRootID = null,
							@dtExpiryDate = null,
							@fStrike = null,
							@iIsCall = null,
							@vcSymbol = null,
							@vcContractName = null,
							@fPriceClose = null,
							@dtExpiryOV = null,
							@dtTradeClose = null
         if @error <> 0 begin 
	   if @iWithTran is not null rollback tran @vcTranName
	   goto finish
         end 

/* -- comment by Eugene Pinchuk
	declare cur_OptionRoot cursor local fast_forward read_only
	for select eodOptionRootID, underlyingID, symbol, lotSize
		from EodOptionRoot (nolock)

	open cur_OptionRoot
	
	fetch next from cur_OptionRoot
	into @eodOptionRootID, @underlyingID, @symbol, @lotSize

	while @@fetch_status != -1
	begin
		if @@fetch_status != -2
		begin
	
			exec @optionRootID = dbo.usp_AspEodUpdateMarketStructure_ImportOptionRoot
							@iUnderlyingID = @underlyingID, @vcSymbol = @symbol, @iLotSize = @lotSize
	
			if @@error = 0 and isnull(@optionRootID, 0) > 0
			begin
				declare cur_Option cursor local fast_forward read_only
				for select symbol, contractName, expiryDate, strike, isCall, priceClose, ExpiryOV, TradingClose
					from EodOption with (nolock, index = IX_EodOption_eodOptionRootID)
					where eodOptionRootID = @eodOptionRootID

				open cur_Option
	
				fetch next from cur_Option
				into @symbol, @contractName, @expiryDate, @strike, @isCall, @priceClose, @dtOVExpiry, @dtCloseTrade
	
				while @@fetch_status != -1
				begin
					if @@fetch_status != -2
					begin
						exec @optionID = dbo.usp_AspEodUpdateMarketStructure_ImportOption
											@iRootID = @optionRootID,
											@dtExpiryDate = @expiryDate,
											@fStrike = @strike,
											@iIsCall = @isCall,
											@vcSymbol = @symbol,
											@vcContractName = @contractName,
											@fPriceClose = @priceClose,
											@dtExpiryOV = @dtOVExpiry,
											@dtTradeClose = @dtCloseTrade
						fetch next from cur_Option
						into @symbol, @contractName, @expiryDate, @strike, @isCall, @priceClose, @dtOVExpiry, @dtCloseTrade
					end
				end
	
				close cur_Option
				deallocate cur_Option
			end
		end

		fetch next from cur_OptionRoot
		into @eodOptionRootID, @underlyingID, @symbol, @lotSize
	end

	close cur_OptionRoot
	deallocate cur_OptionRoot
*/

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

