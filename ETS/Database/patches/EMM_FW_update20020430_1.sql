set nocount on
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
ALTER        proc dbo.usp_Underlying_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0
AS
	set nocount on

	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
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
				    cp.lotSize as iLotSize
				from ContractView c 
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 2
					and (@iContractID is null or c.contractID = @iContractID)
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
				    cp.lotSize as iLotSize
				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
					and (@iContractID is null or c.contractID = @iContractID)
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
				    cp.lotSize as iLotSize
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
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
				    cp.lotSize as iLotSize
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				order by c.symbol	 
		end
		else -- not isnull(@iGroupID, 0) = 0
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
				    cp.lotSize as iLotSize
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where cig.groupID = @iGroupID
					and c.contractTypeID = 2
					and (@iContractID is null or c.contractID = @iContractID)
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
				    cp.lotSize as iLotSize
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where cig.groupID = @iGroupID
					and c.contractTypeID = 1
					and (@iContractID is null or c.contractID = @iContractID)
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
				    cp.lotSize as iLotSize
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where cig.groupID = @iGroupID
					and c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
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
				    cp.lotSize as iLotSize
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where cig.groupID = @iGroupID
					and c.contractTypeID = 1
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				order by c.symbol	 
		end
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
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
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c 
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where
					c.contractTypeID = 2
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, cp.lotSize
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
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where
					c.contractTypeID = 1
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, cp.lotSize
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
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where 
					c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, cp.lotSize
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
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, cp.lotSize
				order by c.symbol	 
		end
		else -- not isnull(@iGroupID, 0) = 0
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
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where cig.groupID = @iGroupID
					and c.contractTypeID = 2
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, cp.lotSize
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
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where cig.groupID = @iGroupID
					and c.contractTypeID = 1
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, cp.lotSize
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
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where cig.groupID = @iGroupID
					and c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, cp.lotSize
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
				    cp.lotSize as iLotSize,

					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where cig.groupID = @iGroupID
					and c.contractTypeID = 1
					and tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, cp.lotSize
				order by c.symbol	 
		end
	end

	Return (0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
GO
--============================================================================================================
--change DB version
--============================================================================================================
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.23')
	begin
		update DataInfo set keyValue = '3.23' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.23'
	end


