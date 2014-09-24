/*
Changes for market maker & feed
	add new SP's: usp_TradeByTrader_Get
				  usp_TradeFeed_Get
	DB version: 3.37.030
*/
set nocount on
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeByTrader_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
CREATE   proc dbo.usp_TradeByTrader_Get
	@iTraderID int = null
as

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
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			null as fPriceTheoClose,
			
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
			tr.brokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iLotSize,
			cp.lotSize as iUnderlyingLotSize,
			
			tr.isSimulated as tiIsSimulated
		from Trade tr
			inner join Contract c on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join Stock s on c.contractID = s.stockID
			left join [Index] i on c.contractID = i.indexID
		where c.contractTypeID in (1, 2)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,cp.priceClose,cp.lotSize
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
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
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
			tr.brokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iLotSize,
			ucp.lotSize as iUnderlyingLotSize,
	
			tr.isSimulated as tiIsSimulated
		from Trade tr inner join Contract c on tr.contractID = c.contractID
			inner join [Option] o on c.contractID = o.optionID
			inner join Contract UC on o.underlyingContractID = UC.contractID
			join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
			left join Stock s on uc.contractID = s.stockID
			left join [Index] i on uc.contractID = i.indexID
		where c.contractTypeID = 3
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,
			cp.lotSize,cp.priceClose,ucp.lotSize,ucp.priceClose
		having min(tr.actionID) = 1
		order by TradeDate

     Return(@@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE PROCEDURE usp_TradeFeed_Get
	@vcExecID varchar(20),
	@dtTradeDate datetime
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is check trade by execID 
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

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
		tr.isSimulated as tiIsSimulated,
		null as fPriceClose,
		null as fUnderlyingPriceClose
	from Trade tr 
		join Contract C  on tr.contractID = c.contractID
		join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		left join StockView s on c.contractID = s.stockID
		left join IndexView i on c.contractID = i.indexID
		left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
		left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
	where tr.actionID > 0 and tr.execID = @vcExecID and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tr.tradeDate,112)
	
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
		tr.isSimulated as tiIsSimulated,
		CP.PriceClose as fPriceClose,
		UCP.priceClose as fUnderlyingPriceClose
	from Trade tr 
		join Contract C  on tr.contractID = c.contractID
		inner join [Option] O on C.contractID = O.optionID
		inner join Contract UC on O.underlyingContractID = UC.contractID
		inner join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
		inner join ContractPrice UCP on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
		join ExpiryInCalendarView EC on UC.expCalendarID = EC.expCalendarID and O.expiryID = EC.expiryID
		left join StockView s on uc.contractID = s.stockID
		left join [IndexView] i on uc.contractID = i.indexID
		left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
		left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
	where tr.actionID > 0 and tr.execID = @vcExecID and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tr.tradeDate,112)

	Return(@@Error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--==================================================================================
--change DB version
--===================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.37.030')
	begin
		update DataInfo set keyValue = '3.37.030' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.37.030'
	end
