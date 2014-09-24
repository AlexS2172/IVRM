/*
	script for next release of ETS 
*/
set nocount on
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Exchange' and COLUMN_NAME='occCode')
	Alter Table Exchange Add occCode varchar(1) null
GO
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Exchange' and COLUMN_NAME='occCode')
	begin
		update Exchange set occCode = Left(exchangeName,1) where exchangeName in ('AMEX','CBOE','PSE','ISE')
		update Exchange set occCode = 'X' where exchangeName = 'PHLX'
	end
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Exchange_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Exchange_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Exchange_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Exchange_Save]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ExchangeView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ExchangeView]
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
CREATE VIEW dbo.ExchangeView
AS
	select  exchangeID, 
			exchangeName, 
			[description], 
			exchangeCode, 
			actionID, 
			actionDate, 
			isUnderlying, 
			isOption, 
			occCode
		from Exchange
		where (actionID < 3)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE procedure dbo.usp_Exchange_Get
	@iExchangeID int = null,
	@dtActionDate datetime = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Exchange table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
  
	select
		exchangeID as iExchangeID,
		exchangeName as vcExchangeName,
		[Description] as vcDescription,
		ExchangeCode as vcExchangeCode,
		actionDate as dtActionDate,
		actionID as tiActionID,
		isUnderlying as tiIsUnderlying,
		isOption as tiIsOption,
		occCode as vcOccCode
	from Exchange
	where (exchangeID=@iExchangeID or @iExchangeID is null) and
			(actionDate >= @dtActionDate or @dtActionDate is null and actionID<3)

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

CREATE    proc dbo.usp_Exchange_Save
		@iExchangeID int = null output,
		@vcExchangeName varchar(30) = null,
		@vcDescription varchar(120) = null,
		@vcExchangeCode varchar(10) = null,
		@tiIsUnderlying tinyint = null,
		@tiIsOption tinyint = null,
		@vcOccCode varchar(10) = null
  as
	if exists (select * from Exchange where ExchangeName = @vcExchangeName and actionID < 3 and (@iExchangeID <> ExchangeID or @iExchangeID is null))
		begin
			Raiserror ('Exchange with this name already exists!', 16, 1)
			Return (-1)
		end   
	
	if exists (select * from Exchange where ExchangeCode = @vcExchangeCode and actionID < 3 and (@iExchangeID <> ExchangeID or @iExchangeID is null))
		begin
			Raiserror ('Exchange with this code already exists!', 16, 1)
			Return (-1)
		end   
   
	if isnull(@vcOccCode, '') <> ''
	begin
		if exists (select * from Exchange where occCode = @vcOccCode and actionID < 3 and (@iExchangeID <> ExchangeID or @iExchangeID is null))
			begin
				Raiserror ('Exchange with this OCC code already exists!', 16, 1)
				Return (-1)
			end   
	end
   
	if @iExchangeID is null
	begin
		insert into Exchange (
					exchangeName, 
					[Description],
					exchangeCode, 
					isUnderlying, 
					isOption,
					occCode)
				values (
					@vcExchangeName, 
					@vcDescription,
					@vcExchangeCode, 
					isnull(@tiIsUnderlying, 1), 
					isnull(@tiIsOption, 1),
					@vcOccCode)

			if (@@error <> 0) return (-1)

		set @iExchangeID = @@identity
	 end
   else 
	 begin
		update Exchange
			 set  
				exchangeName = isnull(@vcExchangeName,exchangeName),
				[Description] = isnull(@vcDescription,[Description]),
				exchangeCode = isnull(@vcExchangeCode,exchangeCode),
				isUnderlying = isnull(@tiIsUnderlying, isUnderlying),
				isOption = isnull(@tiIsOption, isOption),
				occCode = isnull(@vcOccCode, occCode)
			where exchangeID = @iExchangeID

		if (@@error <> 0)
		   return (-1)
		 else  
		   return (0)
	 end
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSeq_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeSeq_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContractForHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderContractForHH_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROC dbo.usp_TradeSeq_Get
	@iMinSeqNum int = null,
	@iMaxSeqNum int = null,
	@dtTradeDate datetime = null,
	@tiExtendedInfo tinyint = null
AS
	set @dtTradeDate = cast(floor(cast(@dtTradeDate as float)) as datetime)
	if @iMinSeqNum = 0 set @iMinSeqNum = Null
	if @iMaxSeqNum = 0 set @iMaxSeqNum = Null
	if @tiExtendedInfo is null or @tiExtendedInfo = 0
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.contractID as iContractID,
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated
		from Trade tr
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where 
			(@iMinSeqNum is null or @iMinSeqNum is not null and seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and cast(floor(cast(tradeDate as float)) as datetime) = @dtTradeDate)
		order by seqNum
	else
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
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
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			null as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iUnderlyingLotSize,
			cp.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID=cp.contractId and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where c.contractTypeID in (1, 2)
			and (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and cast(floor(cast(tr.tradeDate as float)) as datetime) = @dtTradeDate)
		union
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
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
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceTheo as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			tr.lotSize as iTradeLotSize,
			ucp.lotSize as iUnderlyingLotSize,
			cp.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join [OptionView] o on c.contractID = o.optionID
			inner join ContractView uc on o.underlyingContractID = uc.contractID
			inner join ExpiryInCalendar EC on o.expiryID = EC.expiryID and EC.expCalendarID = uc.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where c.contractTypeID = 3
			and (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and cast(floor(cast(tr.tradeDate as float)) as datetime) = @dtTradeDate)
		order by tr.seqNum




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE Procedure dbo.usp_TraderContractForHH_Get 
	@iTraderID int = null, -- trader ID
	@vcAcronym varchar(12) = null, -- trader acronym
	@tiInfoType tinyint = 0, -- information required: 
								-- 0 - available underlyiings list,
								-- 1 - market structure updates,
								-- 2 - contracts for price feed,
								-- 3 - index definition,
								-- 4 - contract prices
	@iContractTypeID int = Null, -- contract type ID for @tiInfoType = 1, if Null or 0 then retrieve general contract updates
	@dtMinUpdateDate datetime = Null, -- minimal update date for @tiInfoType = 1, if Null then retrieve full actual info (w/o "deleted" records)
	@iContractID int = Null -- contract ID for @tiInfoType = 1
as
/*
	Created by Sharky
	Description: this procedure is for getting information for HendHeld initialize
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	if @iContractID = 0 set @iContractID = Null
	-- check tarder parameters
	if isnull(@iTraderID, 0) = 0
	begin
		if not @vcAcronym is null
			select top 1 @iTraderID = traderID
			from TraderView
			where acronym = @vcAcronym
		if isnull(@iTraderID, 0) = 0
		begin
			raiserror('Error. Invalid trader ID or acronym were specified.', 16, 1)
			return -1
		end
	end
	else if not exists(select 1 from Trader where traderID = @iTraderID)
	begin
		raiserror('Error. Invalid trader ID or acronym were specified.', 16, 1)
		return -1
	end
	set @tiInfoType = isnull(@tiInfoType, 0)
	if @tiInfoType = 0 -- available underlyings list
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			ce.exchangeID as iExchangeID,
			isUnderlying as iType1,
			isOption as iType2
		from TraderContractView tc
			join ContractView c on tc.contractID = c.contractID
			join ContractExchangeLink ce on c.contractID = ce.contractID
		where
			tc.traderID = @iTraderID
		union
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			null as iExchangeID,
			1 as iType1,
			1 as iType2
		from TraderContractView tc
			join ContractView c on tc.contractID = c.contractID
		where
			tc.traderID = @iTraderID
	else if @tiInfoType = 1 -- market structure updates
	begin
		if @dtMinUpdateDate is not null -- updates info
		begin
			if isnull(@iContractTypeID, 0) = 0 -- general contract info
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					null as fPriceTheoClose,
	
					c.actionID as tiActionID,
					c.actionDate as dtActionDate
	
				from TraderContractView tc
					join Contract c on tc.contractID = c.contractID
					join ContractPrice cph on c.contractID = cph.contractID and exchangeID is null
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or c.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or c.contractID = @iContractID)
				union
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					cph.priceTheo as fPriceTheoClose,
	
					c.actionID as tiActionID,
					c.actionDate as dtActionDate

				from TraderContractView tc
					join [Option] o on tc.contractID = o.underlyingContractID
					join Contract c on o.optionID = c.contractID
					join ContractPrice cph on c.contractID = cph.contractID and exchangeID is null
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or c.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or o.underlyingContractID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 3 -- options
				select
					o.optionID as iOptionID,
					o.underlyingContractID as iUnderlyingContractID,
					o.isCall as bIsCall,
					o.strike as fStrike,
					E.expiryDate as dtExpiry,
					o.actionID as tiActionID,
					o.actionDate as dtActionDate
					
				from TraderContractView tc
					join [Option] O on tc.contractID = o.underlyingContractID
					join Contract C on O.underlyingContractID = C.contractID
					join ExpiryInCalendar E on E.expCalendarID = C.expCalendarID
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or o.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or o.underlyingContractID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 2 -- stocks
				select
					s.stockID as iStockID,
	
					s.divFreq as iDivFreq,
					s.divDate as dtDivDate,
					s.divAmt as fDivAmt,
					
					null as iBaseIndexID,
					s.primaryExchangeID as iPrimaryExchangeID,
					s.isHTB as bIsHTB,
	
					s.actionID as tiActionID,
					s.actionDate as dtActionDate,
					s.calcOptionType as tiCalcOptionType,
					s.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join Stock s on tc.contractID = s.stockID
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or s.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or s.stockID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 1 -- indices
				select
					i.indexID as iIndexID,
	
					i.yield as fYield,
					i.isHTB as bIsHTB,
	
					i.actionID as tiActionID,
					i.actionDate as dtActionDate,

					i.calcOptionType as tiCalcOptionType,
					i.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join [Index] i on tc.contractID = i.indexID
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or i.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or i.indexID = @iContractID)
				order by dtActionDate
			else
			begin
				raiserror('Error. Invalid contract type ID was specified.', 16, 1)
				return -1
			end
		end
		else
		begin
			if isnull(@iContractTypeID, 0) = 0 -- general contract info
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					null as fPriceTheoClose,
	
					c.actionID as tiActionID,
					c.actionDate as dtActionDate
	
				from TraderContractView tc
					join ContractView c on tc.contractID = c.contractID
					join ContractPrice cph on c.contractID = cph.contractID and exchangeID is null
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				union
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					cph.priceTheo as fPriceTheoClose,
					c.actionID as tiActionID,
					c.actionDate as dtActionDate

				from TraderContractView tc
					join OptionView o on tc.contractID = o.underlyingContractID
					join ContractView c on o.optionID = c.contractID
					left join ContractPrice cph on c.contractID = cph.contractID
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or o.underlyingContractID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 3 -- options
				select
					o.optionID as iOptionID,
					o.underlyingContractID as iUnderlyingContractID,
					o.isCall as bIsCall,
					o.strike as fStrike,
					E.expiryDate as dtExpiry,

					o.actionID as tiActionID,
					o.actionDate as dtActionDate
				from TraderContractView tc
					join OptionView o on tc.contractID = o.underlyingContractID
					join Contract C on O.underlyingContractID = C.contractID
					join ExpiryInCalendar E on E.expCalendarID = C.expCalendarID
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or o.underlyingContractID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 2 -- stocks
				select
					s.stockID as iStockID,
	
					s.divFreq as iDivFreq,
					s.divDate as dtDivDate,
					s.divAmt as fDivAmt,
					
					null as iBaseIndexID,
					s.primaryExchangeID as iPrimaryExchangeID,
					s.isHTB as bIsHTB,
	
					s.actionID as tiActionID,
					s.actionDate as dtActionDate,

					s.calcOptionType as tiCalcOptionType,
					s.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join StockView s on tc.contractID = s.stockID
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or s.stockID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 1 -- indices
				select
					i.indexID as iIndexID,
	
					i.yield as fYield,
					i.isHTB as bIsHTB,
	
					i.actionID as tiActionID,
					i.actionDate as dtActionDate,

					i.calcOptionType as tiCalcOptionType,
					i.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join IndexView i on tc.contractID = i.indexID
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or i.indexID = @iContractID)
				order by dtActionDate
			else
			begin
				raiserror('Error. Invalid contract type ID was specified.', 16, 1)
				return -1
			end
		end
	end
	else if @tiInfoType = 2 -- contracts for price feed
	begin
		declare @dtToday datetime
		set @dtToday = cast(floor(cast(getdate() as float)) as datetime)

		select distinct 
			c.ContractID as iContractID,
			c.ContractTypeID as iContractTypeID,
			c.Symbol as vcSymbol, 
			e.ExchangeCode as vcExchangeCode
		from TraderContractView tc	
			join ContractView c on tc.contractID = c.contractID
			join ContractExchangeLink ce on c.contractID = ce.contractID
			join ExchangeView e on ce.exchangeID = e.exchangeID
		where
			tc.traderID = @iTraderID
			--and isnull(convert(bit, left(ce.exchangeLinkType,1)), 0) <> 0
			and ce.isUnderlying = 1
		union
		select distinct 
			c.ContractID as iContractID,
			c.ContractTypeID as iContractTypeID,
			c.Symbol as vcSymbol, 
			null as vcExchangeCode
		from TraderContractView tc 
			join ContractView c on tc.contractID = c.contractID
		where 
			tc.traderID = @iTraderID
		union
		select distinct 
			c.ContractID as iContractID,
			c.ContractTypeID as iContractTypeID,
			c.Symbol as vcSymbol, 
			e.ExchangeCode as vcExchangeCode
		from TraderContractView tc	
			join OptionView o on tc.contractID = o.underlyingContractID
			join ContractView c on o.optionID = c.contractID
			join ContractExchangeLink ce on o.underlyingContractID = ce.contractID
			join ExchangeView e on ce.exchangeID = e.exchangeID
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendar E1 on E1.expCalendarID = C1.expCalendarID
		where
			tc.traderID = @iTraderID
--			and isnull(convert(bit, right(ce.exchangeLinkType,1)), 0) <> 0
			and ce.IsOption = 1
			and E1.expiryDate > @dtToday
		union
		select distinct 
			c.ContractID as iContractID,
			c.ContractTypeID as iContractTypeID,
			c.Symbol as vcSymbol, 
			null as vcExchangeCode
		from TraderContractView tc	
			join OptionView o on tc.contractID = o.underlyingContractID
			join ContractView c on o.optionID = c.contractID
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendar E1 on E1.expCalendarID = C1.expCalendarID
		where
			tc.traderID = @iTraderID
			and E1.expiryDate > @dtToday
	end
	else if @tiInfoType = 3 -- index definition
	begin
		if @dtMinUpdateDate is not null -- updates info
			select
				i.indexID as iIndexID,
				i.stockID as iStockID,
				i.weight as fWeight,
				i.beta as fBeta,
	
				i.actionID as tiActionID,
				i.actionDate as dtActionDate

			from
				IndexDefinition i
			where
				i.indexID in (select contractID from TraderContractView where traderID = @iTraderID)
				and i.stockID in (select contractID from TraderContractView where traderID = @iTraderID)
				and (@dtMinUpdateDate is null or i.actionDate > @dtMinUpdateDate)
				order by dtActionDate
		else
			select
				i.indexID as iIndexID,
				i.stockID as iStockID,
				i.weight as fWeight,
				i.beta as fBeta,
	
				i.actionID as tiActionID,
				i.actionDate as dtActionDate
			from
				IndexDefinitionView i
			where
				i.indexID in (select contractID from TraderContractView where traderID = @iTraderID)
				and i.stockID in (select contractID from TraderContractView where traderID = @iTraderID)
				order by dtActionDate
	end
	else if @tiInfoType = 4 -- contract prices
	begin
		select
			c.contractID as iContractID,
			cph.priceClose as fPriceClose,
			null as fPriceTheoClose
		from TraderContractView tc
			join Contract c on tc.contractID = c.contractID
			left join ContractPrice cph on c.contractID = cph.contractID 
		where
			tc.traderID = @iTraderID
		union
		select
			c.contractID as iContractID,
			cph.priceClose as fPriceClose,
			cph.priceTheo as fPriceTheoClose
		from TraderContractView tc
			join [Option] o on tc.contractID = o.underlyingContractID
			join Contract c on o.optionID = c.contractID
			left join ContractPrice cph on c.contractID = cph.contractID
		where
			tc.traderID = @iTraderID
	end
	else
	begin
		raiserror('Error. Invalid information type was specified.', 16, 1)
		return -1
	end
	return @@error


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.30')
	begin
		update DataInfo set keyValue = '3.30' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.30'
	end



