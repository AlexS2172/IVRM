/*
	New features for ETS floor version (see update of spec)
*/

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

ALTER         proc dbo.usp_ContractForTrade_Get
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
				CP.lotSize as iLotSize,
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
				cp.lotSize as iUnderlyingLotSize
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
				CP.lotSize as iLotSize,
				-- stock
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
			
				-- index
				i.yield as fYield,
			
				-- option
				o.expiry as dtExpiry,
				o.strike as fStrike,
				o.isCall as bIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.symbol as vcUnderlyingSymbol,
				ucp.priceBid as fUnderlyingPriceBid,
				ucp.priceAsk as fUnderlyingPriceAsk,
				ucp.priceClose as fPriceClose,
				ucp.lotSize as iUnderlyingLotSize
		from ContractView c  
				left join OptionView O on O.optionID = C.contractID
				left join Contract uc on o.underlyingContractID = uc.contractID
				left join ContractPrice CP on C.contractID = CP.contractID
				left join ContractPrice ucp on uc.contractID = ucp.contractID
				left join StockView s on uc.contractID = s.stockID
				left join IndexView i on uc.contractID = i.indexID			
		where
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and O.underlyingContractID = @iUnderlyingID)
				and (@tiNewOnly = 0 or @tiNewOnly = 1 and O.expiry >= @dtToday)
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
				CP.lotSize as iLotSize,
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
				cp.lotSize as iUnderlyingLotSize
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
				CP.lotSize as iLotSize,
				-- stock
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
			
				-- index
				i.yield as fYield,
			
				-- option
				o.expiry as dtExpiry,
				o.strike as fStrike,
				o.isCall as bIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.symbol as vcUnderlyingSymbol,
				ucp.priceBid as fUnderlyingPriceBid,
				ucp.priceAsk as fUnderlyingPriceAsk,
				ucp.priceClose as fPriceClose,
				ucp.lotSize as iUnderlyingLotSize
		from ContractView c  
				left join OptionView O on O.optionID = C.contractID
				left join Contract uc on o.underlyingContractID = uc.contractID
				left join ContractPrice CP on C.contractID = CP.contractID
				left join ContractPrice ucp on uc.contractID = ucp.contractID
				left join StockView s on uc.contractID = s.stockID
				left join IndexView i on uc.contractID = i.indexID			
		where
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and O.underlyingContractID = @iUnderlyingID)
				and (@tiNewOnly = 0 or @tiNewOnly = 1 and O.expiry >= @dtToday)
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
set nocount on 
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.12')
	begin
		update DataInfo set keyValue = '3.12' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.12'
	end
GO