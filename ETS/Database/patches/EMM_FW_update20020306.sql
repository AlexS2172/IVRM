/*
	this script make the ability to store positions in Trade table
*/


if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.02')
	begin
		update DataInfo set keyValue = '3.02' where DataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.02'
	end
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='isPosition')
	begin
		Alter Table Trade
			Add isPosition tinyint null
	end



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSeq_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeSeq_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Clear]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_New]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Update]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TraderView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TraderView]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE  View TraderView
as
	Select 
		traderID,
		acronym,
		traderName,
		clearingTraderCode,
		traderRoleID,
		traderRefID,
		actionID,
		actionDate
	from Trader
	where actionID < 3



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO



CREATE PROC usp_TradeSeq_Get
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
			seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tradeID as iTradeID,
			tr.actionID as tiActionID,
			contractID as iContractID,
			isClose as tiIsClose,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			bookID as iBookID,     
			strategyID as iStrategyID, 
			traderID as iTraderID,
			tr.brokerID as iBrokerID,
			clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
			tradedIV as fTradedIV,
			spotReference as fSpotReference,
			isPosition as tiIsPosition
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
			tr.isPosition as tiIsPosition

		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
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
			o.expiry as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cph.priceTheo as fPriceTheoClose,
			tr.isPosition as tiIsPosition

		from Trade tr inner join ContractView c  on tr.contractID = c.contractID
			inner join [OptionView] o on c.contractID = o.optionID
			inner join ContractView uc on o.underlyingContractID = uc.contractID
			left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceTheo
						from ContractPriceHistory
						inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
							on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
						cph on c.contractID = cph.contractID
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
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE usp_Trade_Clear
AS
	begin tran
	
	delete from Trade
	
	if @@error = 0
		commit tran
	else
		rollback tran
	
	return @@error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE  proc usp_Trade_Get
		@iTradeID int = Null,
		@iContractID int = Null,
		@tiExtendedInfo tinyint = 0,

		@iUnderlyingID int = Null,
	        @iTraderID int = Null,
	        @iBookID int = Null,
	        @iStrategyID int = Null,
	        @iContractGroupID int = Null,
		
		@dtMinTradeDate datetime = null
as
	if @tiExtendedInfo is null or @tiExtendedInfo = 0
		select
			tradeID as iTradeID,
			contractID as iContractID,
			isClose as tiIsClose,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			bookID as iBookID,     
			strategyID as iStrategyID, 
			traderID as iTraderID,
			brokerID as iBrokerID,
			clearingBrokerID as iClearingBrokerID,
			brokerCommissionID as iBrokerCommissionID,
			clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tradedIV as fTradedIV,
			spotReference as fSpotReference,
			isPosition as tiIsPosition
		from Trade 
		where 
			(@iTradeID is null or @iTradeID is not null and tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and contractID = @iContractID)
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tradeDate >= @dtMinTradeDate)
		group by tradeID,contractID,isClose,tradeDate,quantity,price,isBuy,bookID,strategyID,traderID,brokerID,clearingBrokerID,brokerCommissionID,clearingBrokerCommissionID,tradedIV,spotReference,isPosition
		having min(actionID) = 1
		order by TradeDate
	else if @iContractGroupID is null
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
				tr.isPosition as tiIsPosition
			from Trade tr
				inner join Contract c on tr.contractID = c.contractID
				left join Stock s on c.contractID = s.stockID
				left join [Index] i on c.contractID = i.indexID
			where c.contractTypeID in (1, 2)
				and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
				and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
				and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
				and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
				and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
				and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
				and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
				tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
				tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition
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
				o.expiry as dtExpiry,
				o.strike as fStrike,
				o.isCall as bIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.Symbol as vcUnderlyingSymbol,
				cph.priceTheo as fPriceTheoClose,
	
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
				tr.isPosition as tiIsPosition
			from Trade tr inner join Contract c on tr.contractID = c.contractID
				inner join [Option] o on c.contractID = o.optionID
				inner join Contract uc on o.underlyingContractID = uc.contractID
				left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceTheo
							from ContractPriceHistory
							inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
								on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
							cph on c.contractID = cph.contractID
				left join Stock s on uc.contractID = s.stockID
				left join [Index] i on uc.contractID = i.indexID
			where c.contractTypeID = 3
				and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
				and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
				and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
				and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
				and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
				and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
				and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				o.expiry,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cph.priceTheo,
				tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
				tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
				tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition
			having min(tr.actionID) = 1
			order by TradeDate
		else
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
				tr.isPosition as tiIsPosition
			from Trade tr
				inner join Contract c on tr.contractID = c.contractID
				inner join ContractInGroup cig on c.contractID = cig.contractID
				left join Stock s on c.contractID = s.stockID
				left join [Index] i on c.contractID = i.indexID
			where c.contractTypeID in (1, 2)
				and cig.groupID = @iContractGroupID
				and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
				and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
				and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
				and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
				and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
				and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
				and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
				tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
				tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition
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
				o.expiry as dtExpiry,
				o.strike as fStrike,
				o.isCall as bIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.Symbol as vcUnderlyingSymbol,
				cph.priceTheo as fPriceTheoClose,
	
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
				tr.isPosition as tiIsPosition
			from Trade tr inner join Contract c on tr.contractID = c.contractID
				inner join [Option] o on c.contractID = o.optionID
				inner join Contract uc on o.underlyingContractID = uc.contractID
				inner join ContractInGroup cig on o.underlyingContractID = cig.contractID
				left join (select ContractPriceHistory.contractID, ContractPriceHistory.priceTheo
							from ContractPriceHistory
							inner join (select contractID, max(priceDate) as priceDate from ContractPriceHistory where exchangeID is null group by contractID) LastPrice
								on ContractPriceHistory.contractID = LastPrice.contractID and ContractPriceHistory.priceDate = LastPrice.priceDate)
							cph on c.contractID = cph.contractID
				left join Stock s on uc.contractID = s.stockID
				left join [Index] i on uc.contractID = i.indexID
			where c.contractTypeID = 3
				and cig.groupID = @iContractGroupID
				and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
				and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
				and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
				and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
				and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
				and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
				and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				o.expiry,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cph.priceTheo,
				tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
				tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
				tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition
			having min(tr.actionID) = 1
			order by TradeDate

/*     if (@iTradeID is Null)
        if (@iContractID is Null)
	     select

			tradeID as iTradeID,
	     	contractID as iContractID,
			isClose as tiIsClose,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			bookID as iBookID,     
			strategyID as iStrategyID, 
			traderID as iTraderID,
			brokerID as iBrokerID,
			clearingBrokerID as iClearingBrokerID,
			brokerCommission as fBrokerCommission,
			clearingBrokerCommission as fClearingBrokerCommission,
			brokerCommission + clearingBrokerCommission as fTotalCommission,
			tradedIV as fTradedIV,
			spotReference as fSpotReference
            from Trade 
			order by TradeDate
      else 
	     select
			tradeID as iTradeID,
	     	contractID as iContractID,
			isClose as tiIsClose,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			bookID as iBookID,     
			strategyID as iStrategyID, 
			traderID as iTraderID,
			brokerID as iBrokerID,
			clearingBrokerID as iClearingBrokerID,
			brokerCommission as fBrokerCommission,
			clearingBrokerCommission as fClearingBrokerCommission,
			brokerCommission + clearingBrokerCommission as fTotalCommission,
			tradedIV as fTradedIV,
			spotReference as fSpotReference
            from Trade 
			where contractID = @iContractID order by TradeDate
      else
		select 
			tradeID as iTradeID,
		  	contractID as iContractID,
			isClose as tiIsClose,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			bookID as iBookID,     
			strategyID as iStrategyID, 
			traderID as iTraderID,
			brokerID as iBrokerID,
			clearingBrokerID as iClearingBrokerID,
			brokerCommission as fBrokerCommission,
			clearingBrokerCommission as fClearingBrokerCommission,
			brokerCommission + clearingBrokerCommission as fTotalCommission,
			tradedIV as fTradedIV,
			spotReference as fSpotReference
		    from Trade 
			where tradeID = @iTradeID
*/  
     Return(@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE  proc usp_Trade_New
        @iContractID int = Null,
        @tiIsClose tinyint = Null,
        @iQuantity int = Null,
        @fPrice float = Null,
        @tiIsBuy tinyint = Null,  
        @iBookID int = Null,
        @iStrategyID int = Null,
        @iTraderID int = Null,
        @iBrokerID int = Null,
        @iClearingBrokerID int = Null,
        @iBrokerCommissionID int =Null,
        @iClearingBrokerCommissionID int = Null,
        @fTradedIV  float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,

	@iNewTradeID int out,
        @dtNewTradeDate datetime = Null out
  as
	begin tran
	declare @iNewSeqNum int

	select @iNewTradeID = max(tradeID) + 1 from Trade
	if @iNewTradeID is null set @iNewTradeID = 1

	if @dtNewTradeDate is null set @dtNewTradeDate = getdate()

	insert into Trade (
		tradeID,
		actionID, 
		contractID,
		isClose, 
		tradeDate, 
		quantity, 
		price, 
		bookID, 
		strategyID, 
		traderID, 
		brokerID, 
		clearingBrokerID, 
		isBuy, 
		brokerCommissionID, 
		clearingBrokerCommissionID,
		tradedIV,
		spotReference,
		isPosition)
	values (
		@iNewTradeID,
		1,
		@iContractID, 
		@tiIsClose, 
		@dtNewTradeDate, 
		@iQuantity, 
		@fPrice, 
		@iBookID, 
		@iStrategyID, 
		@iTraderID, 
		@iBrokerID,
		@iClearingBrokerID,
		@tiIsBuy, 
		@iBrokerCommissionID,
		@iClearingBrokerCommissionID,
		@fTradedIV,
		@fSpotReference,
		@tiIsPosition)
	if @@error = 0
	begin
		set @iNewSeqNum = @@identity 
		commit tran
		return @iNewSeqNum
	end
	else  
	begin
		rollback tran
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		return 0
	end
/*
   if not exists(select * from Trade where tradeID = @iTradeID)  
     begin
         insert into Trade (
				contractID,
				seqNum,
				publisherID, 
				isClose, 
				tradeDate, 
				quantity, 
				price, 
				bookID, 
				strategyID, 
				traderID, 
				brokerID, 
				clearingBrokerID, 
				isBuy, 
				brokerCommission, 
				clearingBrokerCommission,
				tradedIV,
				spotReference)
                values (
				@iContractID, 
				@iSeqNum,
				@vcPublisherID,
				@iIsClose, 
				@dtTradeDate, 
				@iquantity, 
				@fPrice, 
				@iBookID, 
				@iStrategyID, 
				@iTraderID, 
				@iBrokerID,
				@iClearingBrokerID,
				@iIsBuy, 
				@fBrokerCommission,
				@fClearingBrokerCommission,
				@fTradedIV,
				@fSpotReference)
         if (@@error <> 0)
            return (-1)
          else  
            return @@identity
     end
   else 
     begin
        update Trade 
             set contractID = isnull(@iContractID,contractID),
				seqNum = isnull(@iSeqNum,seqNum),
				publisherID = isnull(@vcPublisherID,publisherID),
				isClose = isnull(@iIsClose,isClose),
				tradeDate = isnull(@dtTradeDate,tradeDate),
				quantity = isnull(@iquantity,quantity),
				price = isnull(@fPrice,price),
				bookID = isnull(@iBookID,bookID),
				strategyID = isnull(@iStrategyID,strategyID),
				traderID = isnull(@iTraderID,traderID),
				brokerID = isnull(@iBrokerID,brokerID),
				clearingBrokerID = isnull(@iClearingBrokerID,clearingBrokerID),
				isBuy = isnull(@iIsBuy,isBuy),
				brokerCommission = isnull(@fBrokerCommission,brokerCommission),
				ClearingBrokerCommission = isnull(@fClearingBrokerCommission,ClearingBrokerCommission),
				tradedIV = isnull(@fTradedIV,tradedIV),
				spotReference = isnull(@fSpotReference,spotReference)
          where tradeID = @iTradeID

        if (@@error <> 0)
           return (-1)
         else  
           return 0
     end
*/
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO



CREATE  proc usp_Trade_Update
	@iOldTradeID int,
        @iContractID int = Null,
        @tiIsClose tinyint = Null,
        @iQuantity int = Null,
        @fPrice float = Null,
        @tiIsBuy tinyint = Null,  
        @iBookID int = Null,
        @iStrategyID int = Null,
        @iTraderID int = Null,
        @iBrokerID int = Null,
        @iClearingBrokerID int = Null,
        @iBrokerCommissionID float =Null,
        @iClearingBrokerCommissionID float = Null,
        @fTradedIV  float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,

	@iNewTradeID int out,
        @dtNewTradeDate datetime out
  as
	begin tran
	declare @bSucceed bit,
		@iNewSeqNum int
	set @bSucceed = 0

	if @iOldTradeID is not null
	begin
		declare @nCount int
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iOldTradeID
		
		if @nCount = 1
		begin
			insert into Trade(tradeID,
				actionID,
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
				isPosition)
			select tradeID,
				0,
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
			from Trade
			where tradeID = @iOldTradeID
		
			if @@error = 0 set @bSucceed = 1
		end
	end


	if @bSucceed = 1
	begin
		select @iNewTradeID = max(tradeID) + 1 from Trade
		if @iNewTradeID is null set @iNewTradeID = 1

		set @dtNewTradeDate = getdate()		
	
		insert into Trade (
			tradeID,
			actionID, 
			contractID,
			isClose, 
			tradeDate, 
			quantity, 
			price, 
			bookID, 
			strategyID, 
			traderID, 
			brokerID, 
			clearingBrokerID, 
			isBuy, 
			brokerCommissionID, 
			clearingBrokerCommissionID,
			tradedIV,
			spotReference,
			isPosition)
		values (
			@iNewTradeID,
			1,
			@iContractID, 
			@tiIsClose, 
			@dtNewTradeDate, 
			@iQuantity, 
			@fPrice, 
			@iBookID, 
			@iStrategyID, 
			@iTraderID, 
			@iBrokerID,
			@iClearingBrokerID,
			@tiIsBuy, 
			@iBrokerCommissionID,
			@iClearingBrokerCommissionID,
			@fTradedIV,
			@fSpotReference,
			@tiIsPosition)
		if @@error = 0
			set @bSucceed = 1
		else
			set @bSucceed = 0
	end

	if @bSucceed = 1
	begin
		set @iNewSeqNum = @@identity
		commit tran
		return @iNewSeqNum
	end
	else  
	begin
		rollback tran
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		return 0
	end
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

