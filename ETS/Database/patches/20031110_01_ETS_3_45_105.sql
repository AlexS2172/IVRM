/*PREPARATIONS FOR ERROR HANDLING*/
SET NOCOUNT ON
GO
IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#tmpErrors')) DROP TABLE #tmpErrors
GO
CREATE TABLE #tmpErrors (Error int)
GO
SET XACT_ABORT ON
GO
/*SCRIPTS MAIN SECTION*/
BEGIN TRANSACTION
GO
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.45.104' or (keyValue = '3.45.105')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.45.105 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.45.105' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

----------------------------------------------------------------------------------------------------
-- remove usp_OptionWithPrice_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_OptionWithPrice_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_BoOptionWithPrice_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoOptionWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoOptionWithPrice_Get]
GO
CREATE proc dbo.usp_BoOptionWithPrice_Get
	  @iUnderlyingID int		 
  as
	select	c.contractID as iContractID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName,
			o.optionRootID as iOptionRootID,
			ec.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheo,
			c.exportSymbol as vcExportSymbol
	from OptionRoot ort
			inner join OptionView o on o.optionRootID = ort.optionRootID
			inner join ContractView c on c.contractID = o.optionID
			inner join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and uc.expCalendarID = ec.expCalendarID
			left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
	where	ort.underlyingID = @iUnderlyingID
			and ec.expiryDate >= getdate()
	order by ort.optionRootID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
 
  return @@error

GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_New
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_New]
GO
CREATE  PROC dbo.usp_Trade_New
    @iContractID int,
    @iQuantity int,
    @fPrice float,
    @tiIsBuy tinyint,  
    @iTraderID int,
    @iBrokerID int = Null,
    @fBrokerCommission float =Null,
    @iClearingBrokerID int = Null,
    @fClearingBrokerCommission float =Null,
    @fTradedIV  float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime = Null out,
	@tiStatus tinyint,
	@iMark int = null,
    @iStrategyID int = null out,
    @vcImportID varchar(20) = null out
as
	set nocount on

	begin tran

	declare @error int		set @error = 0

	if @dtNewTradeDate is null	
	begin
		declare @iDeltaGMT int

		exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

		set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	end

	-- detemine strategy for trade
	exec @error = usp_TraderContractStrategyWithSave_Get @iTraderID, @iContractID, @iStrategyID out
	if @@error <> 0 or @error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

	-- get new tradeID ane seqNum
	declare @iNewSeqNum int

	select 
		@iNewTradeID = isnull(max(tradeID), 0) + 1,
		@iNewSeqNum = isnull(max(seqNum), 0) + 1
	from Trade

	-- create new trade
	insert into Trade (tradeID, actionID, contractID, tradeDate,  quantity, price, traderID, strategyID,
		isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, seqNum, mark)
	values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, @iStrategyID, @tiIsBuy,
		@iBrokerID, @fBrokerCommission, @iClearingBrokerID, @fClearingBrokerCommission,
		@fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, @iNewSeqNum, @iMark)

	if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

	select @vcImportID = exportSymbol from ContractView where contractID = @iContractID

	if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

	commit tran

finish:
	if @error = 0
		return @iNewSeqNum
	else  
	begin
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		set @iStrategyID = null
		set @vcImportID = null
		return @error
	end

GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_Update
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_Update]
GO
CREATE  PROC dbo.usp_Trade_Update
	@iOldTradeID int,
	@iContractID int,
	@iQuantity int,
	@fPrice float,
	@tiIsBuy tinyint,
	@iTraderID int,
	@iBrokerID int = Null,
	@fBrokerCommission float =Null,
    @iClearingBrokerID int = Null,
    @fClearingBrokerCommission float =Null,
	@fTradedIV  float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime out,
	@tiStatus tinyint,
	@iMark int = null,
    @iNewTradeStrategyID int = Null out,
    @vcImportID varchar(20) = Null out
 as
	set nocount on

	declare @error int		set @error = 0

	declare @iNewSeqNum int,
			@vcExecID varchar(20)

	if @iOldTradeID is not null
	begin
		begin tran

		declare @nCount int
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iOldTradeID
		
		if @nCount = 1
		begin
			select @iNewSeqNum = isnull(max(seqNum), 0) + 1 from Trade
			select @vcExecID = execID from Trade where tradeID = @iOldTradeID

			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, seqNum, mark)
			select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark
			from Trade
			where tradeID = @iOldTradeID

			if @@error <> 0 or @error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
		end

		if @dtNewTradeDate is null	
		begin
			declare @iDeltaGMT int

			exec @iDeltaGMT = usp_TimeZoneActimeBias_Get
	
			set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
		end
	
		select 
			@iNewTradeID = isnull(max(tradeID), 0) + 1,
			@iNewSeqNum = isnull(max(seqNum), 0) + 1
		from Trade
	
		-- detemine strategy for trade
		exec @error = usp_TraderContractStrategyWithSave_Get @iTraderID, @iContractID, @iNewTradeStrategyID out
		if @@error <> 0 or @error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

		insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, 
			strategyID, isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
			tradedIV, spotReference, isPosition, status, seqNum, execID, mark)
		values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, 
			@iNewTradeStrategyID,  @tiIsBuy, @iBrokerID, @fBrokerCommission, @iClearingBrokerID, 
			@fClearingBrokerCommission, @fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, 
			@iNewSeqNum, @vcExecID, @iMark)

		if @@error <> 0 or @error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

		select @vcImportID = exportSymbol from ContractView where contractID = @iContractID
	
		if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

		commit tran
	end
	else
		set @error = -4

finish:
	if @error = 0
		return @iNewSeqNum
	else  
	begin
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		set @iNewTradeStrategyID = null
		set @vcImportID = null
		return @error
	end

GO


----------------------------------------------------------------------------------------------------
-- usp_TradeByTrader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeByTrader_Get]
GO
CREATE  PROC dbo.usp_TradeByTrader_Get
	@iTraderID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iTraderID is null
	begin
			select
				tr.tradeID as iTradeID,
				
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.Symbol as vcSymbol,
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
				i.yield as fYield,
				null as dtExpiry,
				null as fStrike,
				null as tiIsCall,
				c.contractID as iUnderlyingContractID,
				c.contractTypeID as iUnderlyingContractTypeID,
				c.Symbol as vcUnderlyingSymbol,
				cp.priceClose as fPriceClose,
				cp.priceClose as fUnderlyingPriceClose,
				null as fPriceTheoClose,
				
				tr.tradeDate as dtTradeDate,
				tr.quantity as iQuantity,
				tr.price as fPrice,
				tr.isBuy as tiIsBuy,
				tr.traderID as iTraderID,
				tr.strategyID as iStrategyID, 
				tr.brokerID as iBrokerID,
				tr.brokerCommission as fBrokerCommission,
				tr.clearingBrokerID as iClearingBrokerID,
				tr.clearingBrokerCommission as fClearingBrokerCommission,
				tr.tradedIV as fTradedIV,
				tr.spotReference as fSpotReference,
				tr.isPosition as tiIsPosition,	
				1 as iLotSize,					
				tr.status as tiStatus,
				null as iOptionRootID,
				null as tiIsSyntheticRoot,
				tr.execID as vcExecID,
				tr.mark as iMark,
				c.exportSymbol as vcImportID
			from Trade tr
				inner join Contract c on tr.contractID = c.contractID
				inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join Stock s on c.contractID = s.stockID
				left join [Index] i on c.contractID = i.indexID
			where
				c.contractTypeID < 3
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				tr.contractID,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,
				tr.traderID,tr.strategyID,tr.brokerID,tr.brokerCommission,tr.clearingBrokerID,tr.clearingBrokerCommission,
				tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,cp.priceClose,tr.execID,tr.mark,c.exportSymbol
			having min(tr.actionID) = 1
	
	   union
	
			select
				tr.tradeID as iTradeID,
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.Symbol as vcSymbol,
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
				i.yield as fYield,
				EC.expiryDate as dtExpiry,
				o.strike as fStrike,
				o.isCall as tiIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.Symbol as vcUnderlyingSymbol,
				cp.priceClose as fPriceClose,
				ucp.priceClose as fUnderlyingPriceClose,
				cp.priceTheo as fPriceTheoClose,
				
				tr.tradeDate as dtTradeDate,
				tr.quantity as iQuantity,
				tr.price as fPrice,
				tr.isBuy as tiIsBuy,
				tr.traderID as iTraderID,
				tr.strategyID as iStrategyID, 
				tr.brokerID as iBrokerID,
				tr.brokerCommission as fBrokerCommission,
				tr.clearingBrokerID as iClearingBrokerID,
				tr.clearingBrokerCommission as fClearingBrokerCommission,
				tr.tradedIV as fTradedIV,
				tr.spotReference as fSpotReference,
				tr.isPosition as tiIsPosition,
				ort.lotSize as iLotSize,
				tr.status as tiStatus,
				ort.optionRootID as iOptionRootID,
				ort.isSynthetic as tiIsSyntheticRoot,
				tr.execID as vcExecID,
				tr.mark as iMark,
				c.exportSymbol as vcImportID
			from Trade tr inner join Contract c on tr.contractID = c.contractID
				inner join [Option] o on c.contractID = o.optionID
				join OptionRoot ort on o.optionRootID = ort.optionRootID
				inner join Contract UC on ort.underlyingID = UC.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
				left join Stock s on uc.contractID = s.stockID
				left join [Index] i on uc.contractID = i.indexID
			where 
				c.contractTypeID = 3
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
				tr.contractID,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,
				tr.traderID,tr.strategyID,tr.brokerID,tr.brokerCommission,tr.clearingBrokerID,tr.clearingBrokerCommission,
				tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,
				ort.lotSize,cp.priceClose,ucp.priceClose, ort.optionRootID,ort.isSynthetic,tr.execID,tr.mark,c.exportSymbol
			having min(tr.actionID) = 1
			order by TradeDate
	end
	else
	begin
			select
				tr.tradeID as iTradeID,
				
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.Symbol as vcSymbol,
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
				i.yield as fYield,
				null as dtExpiry,
				null as fStrike,
				null as tiIsCall,
				c.contractID as iUnderlyingContractID,
				c.contractTypeID as iUnderlyingContractTypeID,
				c.Symbol as vcUnderlyingSymbol,
				cp.priceClose as fPriceClose,
				cp.priceClose as fUnderlyingPriceClose,
				null as fPriceTheoClose,
				
				tr.tradeDate as dtTradeDate,
				tr.quantity as iQuantity,
				tr.price as fPrice,
				tr.isBuy as tiIsBuy,
				tr.traderID as iTraderID,
				tr.strategyID as iStrategyID, 
				tr.brokerID as iBrokerID,
				tr.brokerCommission as fBrokerCommission,
				tr.clearingBrokerID as iClearingBrokerID,
				tr.clearingBrokerCommission as fClearingBrokerCommission,
				tr.tradedIV as fTradedIV,
				tr.spotReference as fSpotReference,
				tr.isPosition as tiIsPosition,	
				1 as iLotSize,					
				tr.status as tiStatus,
				null as iOptionRootID,
				null as tiIsSyntheticRoot,
				tr.execID as vcExecID,
				tr.mark as iMark,
				c.exportSymbol as vcImportID
			from Trade tr
				inner join Contract c on tr.contractID = c.contractID
				inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join Stock s on c.contractID = s.stockID
				left join [Index] i on c.contractID = i.indexID
			where
				c.contractTypeID < 3
				and tr.traderID = @iTraderID
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				tr.contractID,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,
				tr.traderID,tr.strategyID,tr.brokerID,tr.brokerCommission,tr.clearingBrokerID,tr.clearingBrokerCommission,
				tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,cp.priceClose,tr.execID,tr.mark,c.exportSymbol
			having min(tr.actionID) = 1
	
	   union
	
			select
				tr.tradeID as iTradeID,
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.Symbol as vcSymbol,
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
				i.yield as fYield,
				EC.expiryDate as dtExpiry,
				o.strike as fStrike,
				o.isCall as tiIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.Symbol as vcUnderlyingSymbol,
				cp.priceClose as fPriceClose,
				ucp.priceClose as fUnderlyingPriceClose,
				cp.priceTheo as fPriceTheoClose,
				
				tr.tradeDate as dtTradeDate,
				tr.quantity as iQuantity,
				tr.price as fPrice,
				tr.isBuy as tiIsBuy,
				tr.traderID as iTraderID,
				tr.strategyID as iStrategyID, 
				tr.brokerID as iBrokerID,
				tr.brokerCommission as fBrokerCommission,
				tr.clearingBrokerID as iClearingBrokerID,
				tr.clearingBrokerCommission as fClearingBrokerCommission,
				tr.tradedIV as fTradedIV,
				tr.spotReference as fSpotReference,
				tr.isPosition as tiIsPosition,
				ort.lotSize as iLotSize,
				tr.status as tiStatus,
				ort.optionRootID as iOptionRootID,
				ort.isSynthetic as tiIsSyntheticRoot,
				tr.execID as vcExecID,
				tr.mark as iMark,
				c.exportSymbol as vcImportID
			from Trade tr inner join Contract c on tr.contractID = c.contractID
				inner join [Option] o on c.contractID = o.optionID
				join OptionRoot ort on o.optionRootID = ort.optionRootID
				inner join Contract UC on ort.underlyingID = UC.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
				left join Stock s on uc.contractID = s.stockID
				left join [Index] i on uc.contractID = i.indexID
			where c.contractTypeID = 3
				and tr.traderID = @iTraderID
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
				tr.contractID,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,
				tr.traderID,tr.strategyID,tr.brokerID,tr.brokerCommission,tr.clearingBrokerID,tr.clearingBrokerCommission,
				tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,
				ort.lotSize,cp.priceClose,ucp.priceClose, ort.optionRootID,ort.isSynthetic,tr.execID,tr.mark,c.exportSymbol
			having min(tr.actionID) = 1
			order by TradeDate
	end

     Return(@@error)


GO

----------------------------------------------------------------------------------------------------
-- usp_TradeSeqByTrader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSeqByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeSeqByTrader_Get]
GO
CREATE  PROC dbo.usp_TradeSeqByTrader_Get
	@iTraderID int = null,
	@iMinSeqNum int,
	@iMaxSeqNum int
AS
	set nocount on

	set @iMinSeqNum = isnull(@iMinSeqNum, 0)
	set @iMaxSeqNum = isnull(@iMaxSeqNum, 2147483647)

	if @iTraderID is null
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID,
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			null as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			1 as iLotSize,
			tr.status as tiStatus,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			case when exists(select * from TradeLog where contractID = tr.contractID and seqNum = tr.seqNum) then 1 else 0 end iStructureMissed,
			null as iOptionRootID,
			null as tiIsSyntheticRoot,
			tr.execID as vcExecID,
			tr.mark as iMark,
			c.exportSymbol as vcImportID
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID=cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
		where c.contractTypeID < 3
			and tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
		union
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID,
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceTheo as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			ORt.lotSize as iLotSize,
			status as tiStatus,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose,
			case 
			    when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum)
					and exists(select * from TradeLog where contractID = O.optionID and seqNum = tr.seqNum) then 3

				when exists(select * from TradeLog where contractID = o.optionID and seqNum = tr.seqNum) then 2
				when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum) then 1
            else 0 	
			end iStructureMissed,
			ORT.optionRootID as iOptionRootID,
			ORT.isSynthetic as tiIsSyntheticRoot,
			tr.execID as vcExecID,
			tr.mark as iMark,
			c.exportSymbol as vcImportID
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join [OptionView] o on c.contractID = o.optionID
			inner join OptionRootView ORt on ORt.optionRootID = o.optionRootID
			inner join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView EC on o.expiryID = EC.expiryID and EC.expCalendarID = uc.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
		where c.contractTypeID = 3
			and tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
		order by tr.seqNum
	else
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID,
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			null as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			1 as iLotSize,
			tr.status as tiStatus,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			case when exists(select * from TradeLog where contractID = tr.contractID and seqNum = tr.seqNum) then 1 else 0 end iStructureMissed,
			null as iOptionRootID,
			null as tiIsSyntheticRoot,
			tr.execID as vcExecID,
			tr.mark as iMark,
			c.exportSymbol as vcImportID
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID=cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
		where c.contractTypeID < 3
			and tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
			and tr.traderID = @iTraderID
		union
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID,
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceTheo as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			ORt.lotSize as iLotSize,
			status as tiStatus,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose,
			case 
			    when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum)
					and exists(select * from TradeLog where contractID = O.optionID and seqNum = tr.seqNum) then 3
				when exists(select * from TradeLog where contractID = o.optionID and seqNum = tr.seqNum) then 2
				when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum) then 1
            else 0 	
			end iStructureMissed,
			ORT.optionRootID as iOptionRootID,
			ORT.isSynthetic as tiIsSyntheticRoot,
			tr.execID as vcExecID,
			tr.mark as iMark,
			c.exportSymbol as vcImportID
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join [OptionView] o on c.contractID = o.optionID
			inner join OptionRootView ORt on ORt.optionRootID = o.optionRootID
			inner join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView EC on o.expiryID = EC.expiryID and EC.expCalendarID = uc.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
		where c.contractTypeID = 3
			and tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
			and tr.traderID = @iTraderID
		order by tr.seqNum


GO
/* RESULTS ANALYTHING */
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.45.105')
	begin
		update DataInfo set keyValue = '3.45.105' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
IF EXISTS (SELECT * FROM #tmpErrors)
BEGIN
	ROLLBACK TRANSACTION
	--SELECT * FROM #tmpErrors
END
GO
IF @@TRANCOUNT > 0 
BEGIN
	select 'DB VERSION WAS CHANGED TO 3.45.105' '3.45.105 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
