SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.153'  or (keyValue = '3.46.154') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.154 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.154' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
if NOT EXISTS (select * from dbo.sysindexes where name = 'Contract_34' and id = object_id('dbo.Contract'))
BEGIN
CREATE NONCLUSTERED INDEX [Contract_34] ON [dbo].[Contract] ([contractID] ASC, [contractTypeID] ASC, [actionID] ASC, [symbol] ASC, [expCalendarID] ASC, [undPriceProfileID] ASC, [optPriceProfileID] ASC )
CREATE NONCLUSTERED INDEX [TraderContract_52] ON [dbo].[TraderContract] ([contractID] ASC, [traderID] ASC, [actionID] ASC )
CREATE STATISTICS [hind_184387726_1A_7A_13A_3A_2A_6A_5A_9A_12A_11A_10A_15A_16A_17A] ON [dbo].[stock] ([stockid], [actionid], [calcoptiontype], [ishtb], [divfreq], [divdate], [divamt], [isdivcustom], [divfreqcustom], [divdatecustom], [divamtcustom], [isactive], [skew], [kurt])
CREATE STATISTICS [hind_2075870462_1A_2A_3A_7A_10A] ON [dbo].[contract] ([contractid], [contracttypeid], [symbol], [actionid], [expcalendarid])
CREATE STATISTICS [hind_2075870462_1A_10A_7A_2A_3A_11A_12A] ON [dbo].[contract] ([contractid], [expcalendarid], [actionid], [contracttypeid], [symbol], [undpriceprofileid], [optpriceprofileid])
CREATE STATISTICS [hind_2139870690_8A_2A] ON [dbo].[contractprice] ([priceclose], [contractid])
END

GO
GO

ALTER  procedure dbo.usp_UnderlyingByGroup_Get 
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int,
	@tiWithPosition tinyint = 0
AS
	set nocount on
    if not  @iContractID is null
     begin
	if isnull(@tiWithPosition, 0) = 0
	begin
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
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and c.contractID = @iContractID
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
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and c.contractID = @iContractID
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
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and c.contractID = @iContractID
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
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and c.contractID = @iContractID
			order by c.symbol	 
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
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

				sum(tr.quantity  * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and c.contractID = @iContractID
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
				s.isActive, s.skew, s.kurt
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

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and c.contractID = @iContractID
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt
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

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and c.contractID = @iContractID
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose,
				s.isActive, s.skew, s.kurt
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

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and c.contractID = @iContractID
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt
			order by c.symbol	 
	end
       end
       else
----------------------------------------------	
     begin
	if isnull(@tiWithPosition, 0) = 0
	begin
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
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
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
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
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
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
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
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and tc.traderID = @iTraderID
			order by c.symbol	 
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
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

				sum(tr.quantity  * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
				s.isActive, s.skew, s.kurt
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

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt
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

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and tc.traderID = @iTraderID
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose,
				s.isActive, s.skew, s.kurt
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

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and tc.traderID = @iTraderID
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt
			order by c.symbol	 
	end
       end

	Return (0)


GO

GO
ALTER  PROCEDURE dbo.usp_Underlying_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0
AS
	set nocount on

	if isnull(@iGroupID, 0) <> 0
	begin
		exec usp_UnderlyingByGroup_Get @iContractID,@iTraderID,@iGroupID,@tiWithPosition	
		Return(0)
	end 
	if not @iContractID is null
	begin
		if isnull(@tiWithPosition, 0) = 0
		begin
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
				    s.isActive as tiIsActive,

				    s.skew as fSkew,
				    s.kurt as fKurt,

				    c.undPriceProfileID as iUndPriceProfileID,
				    c.optPriceProfileID as iOptPriceProfileID
			   	    from ContractView c 
				    	inner join StockView s on c.contractID = s.stockID
				    	inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		  		    where
					c.contractTypeID = 2
					and c.contractID = @iContractID
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
					i.isActive as tiIsActive,
					i.skew as fSkew,
					i.kurt as fKurt,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
					and c.contractID = @iContractID
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
				    s.isActive as tiIsActive,

				    s.skew as fSkew,
				    s.kurt as fKurt,

				    c.undPriceProfileID as iUndPriceProfileID,
				    c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and  c.contractID = @iContractID
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
				    i.isActive as tiIsActive,
				    i.skew as fSkew,
				    i.kurt as fKurt,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
					and c.contractID = @iContractID
				order by c.symbol	 
		end
		else -- not isnull(@tiWithPosition, 0) = 0
		begin
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

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					s.isActive as tiIsActive,

					s.skew as fSkew,
					s.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 2
					and c.contractID = @iContractID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				    	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
					s.isActive, s.skew, s.kurt, c.undPriceProfileID, c.optPriceProfileID
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

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

					i.isActive as tiIsActive,

					i.skew as fSkew,
					i.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 1
					and c.contractID = @iContractID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt, c.undPriceProfileID, c.optPriceProfileID
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

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
	
					s.isActive as tiIsActive,

					s.skew as fSkew,
					s.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
				where 
					c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and c.contractID = @iContractID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				   	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
					s.isActive, s.skew, s.kurt, c.undPriceProfileID, c.optPriceProfileID
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

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

					i.isActive as tiIsActive,

					i.skew as fSkew,
					i.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
					and c.contractID = @iContractID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.symbol	 
		end
          end
	else
          begin
--------------------------------------------------
		if isnull(@tiWithPosition, 0) = 0
		begin
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
				    s.isActive as tiIsActive,

				    s.skew as fSkew,
				    s.kurt as fKurt,

				    c.undPriceProfileID as iUndPriceProfileID,
				    c.optPriceProfileID as iOptPriceProfileID
			   	    from ContractView c 
				    	inner join StockView s on c.contractID = s.stockID
				    	inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		  		    where
					c.contractTypeID = 2
					and @iContractID is null 
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
					i.isActive as tiIsActive,
					i.skew as fSkew,
					i.kurt as fKurt,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
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
				    s.isActive as tiIsActive,

				    s.skew as fSkew,
				    s.kurt as fKurt,

				    c.undPriceProfileID as iUndPriceProfileID,
				    c.optPriceProfileID as iOptPriceProfileID
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
				    i.isActive as tiIsActive,
				    i.skew as fSkew,
				    i.kurt as fKurt,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
				order by c.symbol	 
		end
		else -- not isnull(@tiWithPosition, 0) = 0
		begin
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

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					s.isActive as tiIsActive,

					s.skew as fSkew,
					s.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 2
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				    	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
					s.isActive, s.skew, s.kurt, c.undPriceProfileID, c.optPriceProfileID
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

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

					i.isActive as tiIsActive,

					i.skew as fSkew,
					i.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 1
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt, c.undPriceProfileID, c.optPriceProfileID
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

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
	
					s.isActive as tiIsActive,

					s.skew as fSkew,
					s.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
				where 
					c.contractTypeID = 2
					and tc.traderID = @iTraderID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				   	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
					s.isActive, s.skew, s.kurt, c.undPriceProfileID, c.optPriceProfileID
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

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

					i.isActive as tiIsActive,

					i.skew as fSkew,
					i.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.symbol	 
		end

        end

	Return (0)


GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.154')
	begin
		update DataInfo set keyValue = '3.46.154' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.154' '3.46.154 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

