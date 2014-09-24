/*
	support of option Roots in ETS Market Maker
*/

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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.38.038' or (keyValue = '3.38.039')))
	BEGIN
		PRINT 'You can''t run this script, because you have not run script for previous version!'
		Select cast(keyValue as varchar(25)) as 'DB version', '3.38.039' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractForTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractForTrade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingByTrader_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE           proc dbo.usp_ContractForTrade_Get
	@iContractID int = null,
	@iUnderlyingID int = null,
	@tiNewOnly tinyint = 0,
	@tiExtendedOrder tinyint = 0
as
	declare @dtToday datetime

	set @dtToday = cast(floor(cast(getdate() as float)) as datetime)

	if isnull(@tiExtendedOrder, 0) = 0
		select
				-- common
				C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				null as fPriceTheoClose,
				1 as iLotSize,
				-- stock
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
			
				-- index
				i.yield as fYield,
			
				-- option
				null as dtExpiry,
				null as fStrike,
				null as bIsCall,
				c.contractID as iUnderlyingContractID,
				c.contractTypeID as iUnderlyingContractTypeID,
				c.symbol as vcUnderlyingSymbol,
				cp.priceBid as fUnderlyingPriceBid,
				cp.priceAsk as fUnderlyingPriceAsk,
				cp.priceClose as fUnderlyingPriceClose,
				1 as iUnderlyingLotSize,
				null as iOptionRootID
			from ContractView c  
				left join StockView S on S.stockID = C.contractID
				left join IndexView I on I.indexID = C.contractID
				left join ContractPrice CP on C.contractID = CP.contractID
			where	
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and c.contractID = @iUnderlyingID)
				and c.contractTypeID in (1, 2)
				and CP.exchangeID is null
		union
		select	
				-- common
				C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				cp.priceTheo as fPriceTheoClose,
				ort.lotSize as iLotSize,
				-- stock
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
			
				-- index
				i.yield as fYield,
			
				-- option
				EC.expiryDate as dtExpiry,
				o.strike as fStrike,
				o.isCall as bIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.symbol as vcUnderlyingSymbol,
				ucp.priceBid as fUnderlyingPriceBid,
				ucp.priceAsk as fUnderlyingPriceAsk,
				ucp.priceClose as fPriceClose,
				1 as iUnderlyingLotSize,
				ort.optionRootID as iOptionRootID
		from ContractView c  
				left join OptionView O on O.optionID = C.contractID
				left join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				left join Contract UC on ORt.underlyingID = UC.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				left join ContractPrice CP on C.contractID = CP.contractID
				left join ContractPrice ucp on uc.contractID = ucp.contractID
				left join StockView s on uc.contractID = s.stockID
				left join IndexView i on uc.contractID = i.indexID			
		where
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and ORt.underlyingID = @iUnderlyingID)
				and (@tiNewOnly = 0 or @tiNewOnly = 1 and EC.expiryDate >= @dtToday)
				and c.contractTypeID = 3
				and CP.exchangeID is null
				and ucp.exchangeID is null
		order by C.symbol
	else
		select * from 
		(select
				-- common
				C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				null as fPriceTheoClose,
				1 as iLotSize,
				-- stock
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
			
				-- index
				i.yield as fYield,
			
				-- option
				null as dtExpiry,
				null as fStrike,
				null as bIsCall,
				c.contractID as iUnderlyingContractID,
				c.contractTypeID as iUnderlyingContractTypeID,
				c.symbol as vcUnderlyingSymbol,
				cp.priceBid as fUnderlyingPriceBid,
				cp.priceAsk as fUnderlyingPriceAsk,
				cp.priceClose as fUnderlyingPriceClose,
				1 as iUnderlyingLotSize,
				null as iOptionRootID
			from ContractView c  
				left join StockView S on S.stockID = C.contractID
				left join IndexView I on I.indexID = C.contractID
				left join ContractPrice CP on C.contractID = CP.contractID
			where	
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and c.contractID = @iUnderlyingID)
				and c.contractTypeID < 3
				and CP.exchangeID is null
		union
		select	
				-- common
				C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				cp.priceTheo as fPriceTheoClose,
				ort.lotSize as iLotSize,
				-- stock
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
			
				-- index
				i.yield as fYield,
			
				-- option
				EC.expiryDate as dtExpiry,
				o.strike as fStrike,
				o.isCall as bIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.symbol as vcUnderlyingSymbol,
				ucp.priceBid as fUnderlyingPriceBid,
				ucp.priceAsk as fUnderlyingPriceAsk,
				ucp.priceClose as fPriceClose,
				1 as iUnderlyingLotSize,
				ort.optionRootID as iOptionRootID
		from ContractView c  
				left join OptionView O on O.optionID = C.contractID
				left join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				left join Contract uc on ORT.underlyingID = uc.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				left join ContractPrice CP on C.contractID = CP.contractID
				left join ContractPrice ucp on uc.contractID = ucp.contractID
				left join StockView s on uc.contractID = s.stockID
				left join IndexView i on uc.contractID = i.indexID			
		where
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and ORt.underlyingID = @iUnderlyingID)
				and (@tiNewOnly = 0 or @tiNewOnly = 1 and EC.expiryDate >= @dtToday)
				and c.contractTypeID = 3
				and CP.exchangeID is null
				and ucp.exchangeID is null
		) as c order by year(c.dtExpiry), month(c.dtExpiry), c.fStrike, c.bIsCall desc

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

CREATE   proc dbo.usp_UnderlyingByTrader_Get
	@iTraderID int = null,
	@tiTraderMode tinyint = 0 -- 0 - filter by TraderContract, 1 - don't filter by TraderContract
AS
	set nocount on

	if isnull(@iTraderID, 0) = 0
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
			s.calcOptionType as tiCalcOptionType,
			s.isHTB as tiIsHTB,
		
		    null as fYield,
			null as tiIsBasket,
		    s.divFreq as iDivFreq,
		    s.divDate as dtDivDate,
		    s.divAmt as fDivAmt,
		
		    s.isDivCustom as tiIsDivCustom,
		    s.divFreqCustom as iDivFreqCustom,
		    s.divDateCustom as dtDivDateCustom,
		    s.divAmtCustom as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,

			null as iTraderID,

			s.skew as fSkew,
			s.kurt as fKurt,
		    c.contractName as vcContractName

		from ContractView c 
		    inner join StockView s on c.contractID = s.stockID
		    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where
			c.contractTypeID = 2
		union
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
			i.calcOptionType as tiCalcOptionType,
			i.isHTB as tiIsHTB,
		
		    i.yield as fYield,
			i.isBasket as tiIsBasket,
		    null as iDivFreq,
		    null as dtDivDate,
		    null as fDivAmt,
		
		    null as tiIsDivCustom,
		    null as iDivFreqCustom,
		    null as dtDivDateCustom,
		    null as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,

			null as iTraderID,

			i.skew as fSkew,
			i.kurt as fKurt,
		    c.contractName as vcContractName

		from ContractView c 
		    inner join IndexView i on c.contractID = i.indexID
		    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where
			c.contractTypeID = 1
		order by c.symbol	 
	else
	begin
		if isnull(@tiTraderMode, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				tc.traderID as iTraderID,

				s.skew as fSkew,
				s.kurt as fKurt,
			    c.contractName as vcContractName

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
				and tc.traderID = @iTraderID
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				tc.traderID as iTraderID,

				i.skew as fSkew,
				i.kurt as fKurt,
			    c.contractName as vcContractName

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
				and tc.traderID = @iTraderID
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				tc.traderID as iTraderID,

				s.skew as fSkew,
				s.kurt as fKurt,
			    c.contractName as vcContractName

			from ContractView c 
			    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				tc.traderID as iTraderID,

				i.skew as fSkew,
				i.kurt as fKurt,
			    c.contractName as vcContractName

			from ContractView c 
			    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
			order by c.symbol	 
	end

Return (@@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TradesForExtApp_View]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TradesForExtApp_View]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE view dbo.TradesForExtApp_View
as
/*
	Created by Zaur Nuraliev
	Description: this view is getting information special for A.D.

	Changed by Zaur Nuraliev(08/08/2002)
*/
select top 100 percent *
from
(
			select
				T.execID as [TradeID],
				cast(convert(varchar(10),getdate(),112) as datetime) as [Trade Date],
				null as [Settlement Date],
				Trader.acronym as [Account Acronym],
				C.Symbol as [Base Symbol],
				null as [Option Symbol],
				null as [Option Root],
				null as [Expiration Month],
				null as [Strike],
				--'S' as [Option Type],
				case when C.contractTypeID = 1 then 'I' else 'S' end as [Option Type],
				T.quantity * (case when T.isBuy <> 0 then 1 else -1 end) as [Net Trade], 
				cast(T.price as decimal(15,2)) as [Trade Price],
				1 as [Multiplier],
				null as [Trade Code], 
				null as [Exchange Code],
				null as [GRoss Proceeds],
				null as [Commission], 
				null as [Exchange Fee], 
				null as [SEC Fees], 
				null as [OCC Fees], 
				null as [Net Proceeds], 
				convert(varchar,T.tradeDate,108) as [Execution Time], 
				null as [Expiration Date], 
				C.ContractName as [Security Name],
				isnull(T.isSimulated,0) as [isSimulated]
			from Trade T
				join Trader on T.traderID = Trader.traderID 
				join Contract C on C.contractID = T.contractID
			where --isnull(T.isSimulated,0) = 0 and 
					C.contractTypeID <3
					and isnull(isPosition,0) = 0 and T.actionID = 1
union
			select
				T.execID as [TradeID],
				cast( convert(varchar(10),getDate(),112) as datetime) as [Trade Date],
				null as [Settlement Date],
				Trader.acronym as [Account Acronym],
				UC.Symbol as [Base Symbol],
				left(C.symbol,len(C.symbol) - 2) + cast(Upper(Datename(month, EC.expiryDate)) as char(3)) + Cast(Cast(O.strike as int) as varchar)  + Case O.IsCall when 1 then 'C' else 'P' end  as [Option Symbol],
				left(C.symbol,len(C.symbol) - 2) as [Option Root],
				cast(Upper(Datename(month, EC.expiryDate)) as char(3)) as [Expiration Month],
				Cast(O.strike as decimal(15,2)) as [Strike],
				Case O.isCall when 1 then 'C' else 'P' end [Option Type],
				T.quantity * (case when T.isBuy <> 0 then 1 else -1 end) as [Net Trade], 
				cast(T.price as decimal(15,2)) as [Trade Price],
				ort.lotSize as [Multiplier],
				null as [Trade Code], 
				null as [Exchange Code],
				null as [GRoss Proceeds],
				null as [Commission], 
				null as [Exchange Fee], 
				null as [SEC Fees], 
				null as [OCC Fees], 
				null as [Net Proceeds], 
				convert(varchar,T.tradeDate,108) as [Execution Time], 
				EC.expiryDate as [Expiration Date], 
				UC.ContractName as [Security Name],
				isnull(T.isSimulated,0) as [isSimulated]
			from Trade T
				join Trader on T.traderID = Trader.traderID 
				join [Option] O on T.contractID = O.optionID
				join Contract C on C.contractID = O.optionID
				join OptionRoot Ort on O.optionRootID = Ort.optionRootID
				join Contract UC on Ort.underlyingID = UC.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			where 	--isnull(T.isSimulated,0) = 0 and 
					isnull(isPosition,0) = 0 and T.actionID = 1
				and EC.expiryDate > = GetDate()	
) v
order by v.[Account Acronym], v.[Base Symbol], 
v.[Expiration Date],v.[Strike], v.[Option Type]

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.38.039')
	begin
		update DataInfo set keyValue = '3.38.039' where dataInfoID = 1
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
	PRINT 'The database update succeeded'
	PRINT 'DB VERSION WAS CHANGED TO 3.38.039'
	COMMIT TRANSACTION
END
	ELSE PRINT 'The database update failed'
GO
DROP TABLE #tmpErrors
GO