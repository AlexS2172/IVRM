----------------------------------------------------------------------------------------------------
-- usp_TradeFeed_PeQuote_Set
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_PeQuote_Set]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeFeed_PeQuote_Set]
GO

CREATE PROC dbo.usp_TradeFeed_PeQuote_Set
	@vcExecID varchar(20),
	@vcTraderAcronym varchar(12),
	@tiIsBuy tinyint,  
	@iQuantity int,
	@fPrice float,
	@dtTradeDate datetime,
	@vcContractSymbol varchar(20),
	@iContractTypeID int,
	@tiIsCall tinyint,
	@fStrike float,
	@dtExpiry datetime,
	@vcOptRootSymbol varchar(20)
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction

	insert into Current_PeQuote_SnapShot (execID, trader_acronym, isBuy, quantity,
		price, trade_date, contract_symbol, contract_type, isCall, 
		strike, expiry_date, opt_root_symbol, isNewTrade)
	values (@vcExecID, @vcTraderAcronym, @tiIsBuy, @iQuantity, @fPrice, 
		@dtTradeDate, @vcContractSymbol, @iContractTypeID, @tiIsCall, 
		@fStrike, @dtExpiry, @vcOptRootSymbol, 1)

	if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

	Commit Transaction

	set @error = 0
	return @error

finish:
	return @@error
GO