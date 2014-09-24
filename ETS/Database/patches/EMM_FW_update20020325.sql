/*
	fix for Defect MLine00000226 (Error in VolMan start Submitted)

*/


if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Del]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockVM_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockVM_Save]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockWithPrice_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContract_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderContract_Save]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TradeView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TradeView]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE view dbo.TradeView
AS
	Select 	tradeID,
		contractID, 
		isClose, 
		tradeDate, 
		quantity, 
		price, 
		isBuy, 
		bookID, 
		strategyID, 
		traderID, 
		brokerID, 
		clearingBrokerID, 
		brokerCommissionID, 
       	clearingBrokerCommissionID, 
		tradedIV, 
		spotReference,
		isPosition
	from dbo.Trade
	GROUP BY tradeID, contractID, isClose, 
		 tradeDate, quantity, price, 
		 isBuy, bookID, strategyID, 
		 traderID, brokerID, clearingBrokerID, 
		 brokerCommissionID,clearingBrokerCommissionID,
		tradedIV, spotReference,isPosition
	HAVING  (MIN(actionID) = 1)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE         Procedure dbo.usp_Option_Del
	@iOptionID int = null, 	-- contract identifier
	@bisRemovePermanent bit = 0
as
/*
	Create by Zaur Nuraliev
	Description: It deletes Option data from Option and Contract tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if object_ID('tempdb..#Options_for_delete') is null
		begin	
			Create table #Options_for_delete(OptionID int)
		end
	
	if @iOptionID is not null
		begin
			if exists (select tradeID from Trade where contractID = @iOptionID)
				begin
					Raiserror ('Error. Can''t delete [Option] because of the existing Trade', 16, 1)
					Return (-1)
				end   
			insert into #Options_for_delete
				select @iOptionID
		end
	Begin Transaction DelOpt
		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete ContractPriceHistory where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ModelParameter where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update ContractInGroup set actionID=3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update [Option] set actionID=3 where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update Contract set actionID=3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete Trade where contractID = @iOptionID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		if @bisRemovePermanent = 1
			begin
				delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
				delete [Option] where OptionID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
				delete Contract where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
			end

	Commit Transaction DelOpt

finish:
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE      Procedure dbo.usp_StockVM_Save
	@iStockID int = null output,		-- stock's identifier
	@iContractTypeID int = null,		-- contract's type identifier (not used)
	@vcSymbolName varchar(12) = null,	-- stock's ticker
	@iExchangeID int = null,		-- exchange's identifier
	@vcDescription varchar(50) = null,	-- description
	@iLotSize int = null,			-- lot size (not used)
	@iDivFrequency int = null,		-- dividend's frequency
	@dtDate datetime = null,		-- date
	@fIVMean float = null,			-- iv mean
	@fCurrentHV float = null,		-- current hv
	@fCurrentIV float = null,		-- current iv
	@fAmount float = null, 			-- amount
	@dDivDate datetime = null,		-- dividend's date
	@iTenorID int = null,			-- tenor's identifier
	@iTenorValue int = null,		-- tenor's value
	@fIVCall float = null,			-- iv call
	@fIVPut float = null,			-- iv put
	@fPriceClose float = null,		-- stock's close price
	@iGroupID int = null,
	@iRuleID int = null
as
/*
	Created by Andrey Sinkin
	Can to save data to Contract, Stock, StockDataHistory, StockDatasnapshot tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	declare @iDividendID int, @iMarketDataID int
	set @iContractTypeID = 2	-- stock
	set @iLotSize = 0
	-- if wrong parameters -> exit
	if @vcSymbolName is null and @iStockID is null	goto finish
	if @iTenorID is null and @iTenorValue is not null
		select @iTenorID = tenorID from Tenor where value = @iTenorValue
	if @iStockID is null and @vcSymbolName is not null
		select @iStockID = contractID from ContractView where symbol = @vcSymbolName and contractTypeID = 2	-- stock
	Begin Transaction
		if @iStockID is null
			begin
				insert into Contract (contractTypeID, symbol,contractName,groupID,ruleID) 
					values (@iContractTypeID, @vcSymbolName,@vcDescription, @iGroupID,@iRuleID)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iStockID = @@identity
				insert into Stock (StockID, divFreq)
					values (@iStockID, @iDivFrequency)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				if @iContractTypeID is not null or @vcSymbolName is not null or @iExchangeID is not null or @vcDescription is not null or @iLotSize is not null
					begin
						update Contract set 
							contractTypeID = isnull(@iContractTypeID, contractTypeID),
							symbol = isnull(@vcSymbolName, symbol),
							contractName = isnull(@vcDescription, contractName),
							groupID = isnull(@iGroupID, groupID),
							ruleID = isnull(@iRuleID, ruleID)
						where contractID = @iStockID
						if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
					end
			end
		--============================================================================================================
		if exists(select * from ContractPrice where contractID = @iStockID and (exchangeID = @iExchangeID or @iExchangeID is null))
			update ContractPrice 
				set exchangeID = @iExchangeID,
					lotSize = isnull(@iLotSize, lotSize),
					contractID = isnull(@iStockID, contractID)
				where contractID = @iStockID and (exchangeID = @iExchangeID or @iExchangeID is null)
		else
			insert into ContractPrice
				(exchangeID,contractID,lotSize)
			values (@iExchangeID,@iLotSize,@iStockID)
		--============================================================================================================
		if @dtDate is not null and @fIVMean is not null and @iTenorID is not null
			begin
				exec @error = usp_StockDataHistory_Save @iStockID = @iStockID, @dtDate = @dtDate, @fIVMean = @fIVMean, @iTenorID = @iTenorID, @fIVCall = @fIVCall, @fIVPut = @fIVPut, @fCurrentHV = @fCurrentHV
				if @error <> 0 begin set @error = 5 if @@TranCount = 1 Rollback Transaction goto finish end
			end

		if @fAmount is not null or @dDivDate is not null or @iDivFrequency is not null
			begin
				update Stock set 
					divFreq = isnull(@iDivFrequency, divFreq),
					divAmt = isnull(@fAmount, divAmt),
					divDate = isnull(@dDivDate, divDate)
					where StockID = @iStockID

				if @error <> 0 begin set @error = 7 if @@TranCount = 1 Rollback Transaction goto finish end
			end

		if (@fCurrentHV is not null or @fCurrentIV is not null) and @iTenorID is not null
			begin
				if @@TranCount = 1 Rollback Transaction
				Raiserror ('Error. Can''t save CurrentIV for Stock (usp_StockDataSnapshot_Save stored procedure), please send this error message to DB developer !', 16, 1)
			end

		if @fPriceClose is not null
			begin
				if @@TranCount = 1 Rollback Transaction
				Raiserror ('Error. Can''t save close price for Stock (usp_StockVM_Save stored procedure), please send this error message to DB developer !', 16, 1)
			end

		set @error = 0
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE    proc usp_StockWithPrice_Get
	@iGroupID int = Null,
	@iStockID int = Null,
	@bIsBestPriceOnly bit = 0
  as
	if (@iGroupID is Null)
		if @bIsBestPriceOnly=0
			select	C.contractID as iContractID , C.contractTypeID as iContractTypeID , C.Symbol as vcSymbol, C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, S.DivFreq as iDivFreq,S.BaseIndexID as iBaseIndexID,
					S.DivDate as dtDivDate,S.DivAmt as fDivAmt, S.isHTB as bIsHTB,
				    CP.LotSize as iLotSize,CP.priceBid as fPriceBid, CP.priceAsk as fPriceAsk,
				    CP.priceLast as fPriceLast, CP.priceClose as fPriceClose, CP.priceOpen as fPriceOpen, CP.priceLow as fPriceLow, CP.priceHigh as fPriceHigh, CP.Volume as iVolume,
					E.exchangeID as iExchangeID,E.ExchangeCode as vcExchangeCode
				    from ContractView c  
						join StockView S on S.StockID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				    where c.contractTypeID = 2
						and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				   order by c.Symbol,E.ExchangeCode
		else
				select	C.contractID as iContractID , C.contractTypeID as iContractTypeID , C.Symbol as vcSymbol, C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, S.DivFreq as iDivFreq,S.BaseIndexID as iBaseIndexID,
					S.DivDate as dtDivDate,S.DivAmt as fDivAmt, S.isHTB as bIsHTB,
					CP.LotSize as iLotSize,CP.priceBid as fPriceBid, CP.priceAsk as fPriceAsk,
				    CP.priceLast as fPriceLast, CP.priceClose as fPriceClose, CP.priceOpen as fPriceOpen, CP.priceLow as fPriceLow, CP.priceHigh as fPriceHigh, CP.Volume as iVolume,
					E.exchangeID as iExchangeID,E.ExchangeCode as vcExchangeCode
				    from ContractView c  
						join StockView S on S.StockID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				    where c.contractTypeID = 2
						and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
						and CP.exchangeID is null
				   order by c.Symbol,E.ExchangeCode
	else
		select	C.contractID as iContractID , C.contractTypeID as iContractTypeID , C.Symbol as vcSymbol, C.ContractName as vcContractName, 
				S.defaultModelTypeID as iDefaultModelTypeID, S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, S.DivFreq as iDivFreq,S.BaseIndexID as iBaseIndexID,
				S.DivDate as dtDivDate,S.DivAmt as fDivAmt, S.isHTB as bIsHTB,
			    CP.LotSize as iLotSize,CP.priceBid as fPriceBid, CP.priceAsk as fPriceAsk,
			    CP.priceLast as fPriceLast, CP.priceClose as fPriceClose, CP.priceOpen as fPriceOpen, CP.priceLow as fPriceLow, CP.priceHigh as fPriceHigh, CP.Volume as iVolume,
				E.exchangeID as iExchangeID,E.ExchangeCode as vcExchangeCode
			    from ContractInGroupView cig
					join ContractView c  on C.contractID = CIG.contractID
					join StockView S on S.StockID = C.contractID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
		        where CIG.groupID = @iGroupID
		          and c.contractTypeID = 2
			   order by c.Symbol,E.ExchangeCode
if (@@error <> 0)
	Return (-1)
else  
   Return(0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE       Procedure dbo.usp_TraderContract_Save
	@iContractID int,
	@iTraderID int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is get information from TraderRole table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

/*	if exists (select * from TraderContract where 
				contractID=@iContractID and 
				traderID=@iTraderID and 
				actionID < 3)
		begin
			Raiserror ('Row in TraderContract table with same atributes already exists!', 16, 1)
			Return (-1)
		end   */

	--if @iContractID is null and @iTraderID is null 
	if not exists(select * from TraderContract where traderID=@iTraderID and contractID=@iContractID and actionID < 3)
		insert into TraderContract(traderID,contractID) 
			values(@iTraderID,@iContractID)
	set @error = @@error
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from sysobjects where id=object_id('[dbo].[usp_initContractPriceEOD_Save]') and OBJECTPROPERTY(id, 'IsProcedure')=1)
  drop procedure [dbo].[usp_initContractPriceEOD_Save]
GO
if exists (select * from sysobjects where id=object_id('[dbo].[usp_completeContractPriceEOD_Save]') and OBJECTPROPERTY(id, 'IsProcedure')=1)
  drop procedure [dbo].[usp_completeContractPriceEOD_Save]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Dividend_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Dividend_Del]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Dividend_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Dividend_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Dividend_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Dividend_Save]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Del]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Del]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Import]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[vALLStockEOD]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[vALLStockEOD]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[StockView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[StockView]
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
CREATE    View dbo.StockView
as
	Select 
		stockID,
		defaultModelTypeID,
		defaultSurfaceTypeID,
		divFreq,
		baseIndexID,
		isHTB,
		primaryExchangeID,
		divAmt,
		divDate,
		divAmtCustom,
		divDateCustom,
		divFreqCustom,
		isDivCustom,
		actionID,
		actionDate
	from Stock
	where actionID < 3




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
CREATE      view dbo.vALLStockEOD
AS
SELECT 	C.contractID AS iContractID,   
		C.contractTypeID AS iContractTypeID,    
		C.Symbol AS vcSymbol, 
    	S.divFreq AS iDivFreq, 
		S.divAmt AS fDivAmt,  
		S.divDate AS dtDivDate,
		S.isHTB as bIsHTB, 
		S.divAmtCustom,
		S.divDateCustom,
		S.divFreqCustom,
		S.isDivCustom
	FROM ContractView C (nolock), StockView S (nolock)
	WHERE S.stockID = C.contractID
UNION
SELECT 	C.contractID AS iContractID,  
		C.contractTypeID AS iContractTypeID,
	    C.Symbol AS vcSymbol,  
		NULL AS iDivFreq, 
		I.yield AS fDivAmt, 
		NULL AS dtDivDate,
		I.isHTB as bIsHTB, 
		null as divAmtCustom,
		null as divDateCustom,
		null as divFreqCustom,
		null as isDivCustom
	FROM ContractView C (nolock), IndexView I (nolock)
	WHERE I.indexID = C.contractID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE   Procedure dbo.usp_Contract_Del
	@iContractID int	-- contract identifier
as
/*
	Create by Zaur Nuraliev
	Description: It deletes Option data from Option and Contract tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
-- PLEASE CHECK THE LOGIC OF THIS PROCEDURE CAREFULLY !
	Begin Transaction
		delete StockDataHistory where stockID=@iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		--delete Dividend where stockID=@iContractID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		update Stock set actionID=3 where stockID=@iContractID
		--delete Stock where stockID=@iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		exec @error=usp_Option_Del @iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
		update Contract set actionID=0 where contractID = @iContractID
		--delete Contract where contractID = @iContractID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE     Procedure dbo.usp_Contract_Get
	@iContractID int = null,	-- contract's identifier
	@iContractTypeID int = null,	-- contract's type identifier
	@bAllWithoutOptions bit = 1,	-- flag for data output
	@dtActionDate datetime = null
as
/*
	Created by Andrey Sinkin
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	if @bAllWithoutOptions is null		set @bAllWithoutOptions = 0
	if @bAllWithoutOptions = 1
		select 	Contract.contractID iContractID, 
				Contract.symbol vcSymbolName, 
				Contract.contractName vcDescription, 
				Stock.primaryExchangeID iExchangeID, 
				Contract.contractTypeID iContractTypeID, 
				Stock.divFreq iDivFrequency, 
				Stock.divAmt fAmount, 
				Stock.divDate dtDivDate,
				IndexView.yield fYield,
				Contract.actionDate as dtActionDate,
				Contract.actionID as tiActionID
			from Contract
				left join StockView Stock on Contract.contractID = Stock.stockID
				left join IndexView on Contract.contractID = IndexView.indexID
			where contractTypeID < 3
				and (@iContractID is null or contractID = @iContractID)
				and (Contract.actionDate>=@dtActionDate or @dtActionDate is null and Contract.actionID<3)
			order by Contract.symbol
	else
		select 	Contract.contractID iContractID, 
				Contract.symbol vcSymbolName, 
				Contract.contractName vcDescription, 
				Stock.primaryExchangeID iExchangeID, 
				Contract.contractTypeID iContractTypeID, 
				Stock.divFreq iDivFrequency, 
				Stock.divAmt fAmount, 
				Stock.divDate dtDivDate,
				IndexView.yield fYield,
				Contract.actionDate as dtActionDate,
				Contract.actionID as tiActionID
			from Contract
				left join StockView Stock on Contract.contractID = Stock.stockID
				left join IndexView on Contract.contractID = IndexView.indexID
			where (@iContractTypeID is null or contractTypeID = @iContractTypeID)
				and (@iContractID is null or contractID = @iContractID)
				and (Contract.actionDate>=@dtActionDate or @dtActionDate is null and Contract.actionID<3)
			order by Contract.symbol
	
Return(0)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE       proc usp_Stock_Del
        @iStockID int,
        @bisRemovePermanent bit = 0
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Stock table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	if exists (select * from TradeView(nolock) where contractID = @iStockID)
	begin
		Raiserror ('Error. Can''t delete Stock because of the existing Trade', 16, 1)
		Return (-1)
	end 
	if exists(select * from [Option](nolock) where underlyingContractID=@iStockID)
		begin
			Create table #Options_for_delete(OptionID int)
		
			insert into #Options_for_delete
				select optionID from [Option](nolock) where underlyingContractID=@iStockID
		
			if exists (select tradeID from TradeView where contractID in (select optionID from #Options_for_delete))
				begin
					Raiserror ('Error. Can''t delete Stock because of the existing Trade by Stock Options', 16, 1)
					Return (-1)
				end 
		end
	Begin Transaction
		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del @bisRemovePermanent = @bisRemovePermanent
			end

		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID = @iStockID
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractExchangeLink where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	    delete ModelParameter where contractID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
        update ContractInGroup set actionID=3 where contractID = @iStockID 
        --delete ContractInGroup where contractID = @iStockID 
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockDataHistory where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockCorrelation where stockID1 = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockCorrelation where stockID2 = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

        update IndexDefinition set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		update TraderContract set actionID=3 where contractID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Stock set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

        delete ContractPriceHistory where contractID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete ContractPrice where contractID = @iStockID

			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Contract set actionID=3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		if (@bisRemovePermanent = 1) 
			begin
	        	delete IndexDefinition where stockID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				delete TraderContract where contractID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		       	delete Stock where stockID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	        	delete Contract where contractID = @iStockID
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end 
  
		set @error = @@error
	Commit Transaction
finish:
Return(@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE      proc usp_Stock_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@fPriceAsk float,
	@fPriceBid float,
	@fPriceOpen float,
	@fPriceClose float,
	@fPriceLow float,
	@fPriceHigh float,
	@iVolume int,
	@iExchangeID int = null,  --the ExchangeID special for save data into ContractPrice table
	@iLotSize int = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		update Contract
		    set ContractName = @vcContractName
	 	where contractID = @iContractID
	
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update Contract Name!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end
		update ContractPrice 
			set priceAsk = @fPriceAsk,
			priceBid = @fPriceBid,
			PriceOpen = @fPriceOpen,
			PriceClose = @fPriceClose,
			PriceLow = @fPriceLow,
			PriceHigh = @fPriceHigh,
			Volume = @iVolume,
			lotSize = isnull(@iLotSize,lotSize)
		where contractID = @iContractID and exchangeID=@iExchangeID
		if (@@error <> 0)
			begin
				Raiserror ('Error. Can''t update Contract Price!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end
		--========================================================================
		-- make record with null exchange for this stock
		--========================================================================
		if not exists(select * from ContractPrice where exchangeID is null and contractID = @iContractID)
			begin
				insert into ContractPrice(contractID,exchangeID,lotSize)
					values(@iContractID,null,@iLotSize)
				if (@@error <> 0) begin rollback tran return(-1) end           
			end 
		--==================================================================================
		-- update DivAmnt & DivFreq values
		--==================================================================================
		if (@bIsStock <> 0 and exists(select * from stock where stockID=@iContractID))
			begin 
				Declare @df smallint
				update Stock 
					set divAmt = @fDivAmt,
						divDate = @dtDivDate, 
						divFreq = @iDivFreq
				where stockID = @iContractID
				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update DivAmnt and DivDate!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
			end
	Commit Transaction
    set @error = 0
finish:
	if @error<>0 
		Return(@error)
	else
		Return @iContractID



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER     proc usp_allStockEOD_Get
	@iFormat int = 1	-- 1 - list only
				   		-- 2 - list with prices by exchange
  as
	if @iFormat = 1
		select	iContractID, 
				iContractTypeID, 
				vcSymbol
			from vALLStockEOD A
	else
		-- use NULL exchange only
		select iContractID, 
				iContractTypeID, 
				vcSymbol, 
				null as vcExchangeCode,  
				case when isDivCustom = 1 then A.divFreqCustom else A.iDivFreq end iDivFreq,
				case when isDivCustom = 1 then A.divAmtCustom else A.fDivAmt end fDivAmt,
				case when isDivCustom = 1 then A.divDateCustom else A.dtDivDate end dtDivDate,
				CP.priceClose as fPriceClose,
				A.bIsHTB as bIsHTB 
			from vALLStockEOD A, ContractPrice CP (nolock)
			where A.iContractID *= CP.contractID and CP.exchangeID is null

if (@@error <> 0)
	Return (-1)
else  
   Return(0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

ALTER    Procedure dbo.usp_ReportStock_Get
	@iStockID int = Null, 
	@iFilterID int = Null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from [Option] table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	--=====================================================
	-- work with filter
	--=====================================================
	create table #FilteredStocks(stockID int)
              insert into #FilteredStocks 
                      exec usp_FilterData_Get @iFilterID
	--=====================================================
	-- data selection
	--=====================================================
		select
			Contract.ContractID as iContractID,
			Contract.symbol as vcSymbol,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceLast as fPriceLast,
			Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
			case when isDivCustom = 1 then divFreqCustom else Stock.divfreq end iDivFreq,
			case when isDivCustom = 1 then divAmtCustom else Stock.divAmt end fDivAmt,
			case when isDivCustom = 1 then divDateCustom else Stock.DivDate end dtDivDate,
			S2.currentHV as fCurrentHV,
			CP.priceClose as fPriceClose,
	  		Stock.isHTB as bIsHTB,
			Stock.isDivCustom as tiIsDivCustom
	
			from ContractView Contract
					join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
					join StockView Stock on Contract.contractID = Stock.StockID
					left join (select StockDataHistory.stockID,currentHV,[date] from StockDataHistory join
									(select stockID, max([date]) as maxDate from StockDataHistory group by stockID) S1
								on S1.stockID=StockDataHistory.stockID and S1.maxDate=StockDataHistory.[date]) S2 
						on S2.stockID=Stock.stockID
	
			where (
				(@iStockID is null and (Stock.stockID in (select stockID from #FilteredStocks)))
				or 
				(@iStockID is not null and Contract.ContractID=@iStockID 
					and (Stock.stockID in (select stockID from #FilteredStocks) or @iFilterID is null))
				)
				and Contract.contractTypeID = 2
	union
		select
			Contract.ContractID as iContractID,
			Contract.symbol as vcSymbol,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceLast as fPriceLast,
			Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
			0 as iDivFreq,
			I.yield as fDivAmt,
	  		null as dtDivDate,
			S2.currentHV as fCurrentHV,
			CP.priceClose as fPriceClose,
	  		I.isHTB as bIsHTB,
			null as tiIsDivCustom
			from ContractView Contract
					join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
					join IndexView I on Contract.contractID = I.indexID
					left join (select IndexDataHistory.indexID,currentHV,[date] from IndexDataHistory join
									(select indexID, max([date]) as maxDate from IndexDataHistory group by indexID) S1
								on S1.indexID=IndexDataHistory.indexID and S1.maxDate=IndexDataHistory.[date]) S2 
						on S2.indexID=I.indexID
			where (
				(@iStockID is null and (I.indexID in (select stockID from #FilteredStocks))) or 
				(@iStockID is not null and @iFilterID is not null and (Contract.ContractID=@iStockID and (I.indexID in (select stockID from #FilteredStocks))))
				)
				and Contract.contractTypeID = 1
		order by Contract.symbol
	set @error = @@error
             drop table #FilteredStocks
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPriceEOD_Complete_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPriceEOD_Complete_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPriceEOD_Init_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPriceEOD_Init_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create procedure dbo.usp_ContractPriceEOD_Complete_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	Begin tran 
	   if object_id('tempdb..##ContractPriceEOD') is null
		   begin
		       raiserror ( 'Cannot complete EOD. Cannot find save data.', 16, 1)
		       return (-1)
		   end
	
		select	C.contractID, 
				E.exchangeID, 
				A.priceClose 
			into #temp
			from ##ContractPriceEOD A, Contract C (nolock), Exchange E (nolock)
			where A.symbol = C.symbol and A.exchangeCode *= E.exchangeCode and C.actionID < 3
	       
		update ContractPrice
			set contractID = S.contractID, 
				exchangeID = S.exchangeID, 
				priceClose = S.priceClose, 
				priceDate = getdate()
		from ContractPrice CP join #temp S on CP.contractID = S.contractID and isnull(CP.exchangeID, 0) = isnull(S.exchangeID, 0)
			--where Z.priceDate between convert(datetime, convert(varchar(15), GetDate(), 112), 112) and
			--               	convert(datetime, convert(varchar(15), GetDate()+1, 112), 112)
	
		insert into ContractPrice (contractID, exchangeID, priceClose, priceDate)
			select *, getdate()
				from #temp B
				where not exists(select * from ContractPrice C where C.contractID = B.contractID and isnull(C.exchangeID, 0) = isnull(B.exchangeID, 0))
	
	Commit tran

	if object_id('tempdb..##ContractPriceEOD') is not null
		drop table ##ContractPriceEOD

	set @error = @@error
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

Create procedure dbo.usp_ContractPriceEOD_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if object_id('tempdb..##ContractPriceEOD') is not null
	begin
		raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
		return (-1) 
		-- drop table ##ContractPriceEOD
	end
	create table ##ContractPriceEOD(
		symbol varchar (8) null ,
		contractTypeID int not null,
		priceClose float not null,
		exchangeCode char(10) null) 
	set @error = @@error
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

