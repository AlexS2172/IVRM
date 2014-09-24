set nocount on 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionWithPrice_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingOptions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingOptions_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Underlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Underlying_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
CREATE  proc dbo.usp_Index_Get
	@iGroupID int = Null,
	@iIndexID int = Null,
	@dtActionDate datetime = Null,
	@tiWithPosition tinyint = 0,
	@iTraderID int = null
  as
	if isnull(@tiWithPosition, 0) = 0

	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractView C 	
					inner join [Index] I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractView C 	
					inner join [Index] I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where C.contractTypeID=1
					and tc.traderID = @iTraderID
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				order by C.symbol
		end
		else
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join [Index] I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join [Index] I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and tc.traderID = @iTraderID
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				order by C.symbol
		end
	end
	else
	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractView c 	
					inner join [Index] I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID, I.isBasket
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractView c 	
					inner join [Index] I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where C.contractTypeID=1
					and tc.traderID = @iTraderID
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID, I.isBasket
				order by C.symbol
		end
		else
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join [Index] I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID,I.isBasket
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join [Index] I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and tc.traderID = @iTraderID
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID,I.isBasket
				order by C.symbol
		end
	end
if (@@error <> 0)
	Return (-1)
else  
	Return (0)









GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE	proc dbo.usp_OptionWithPrice_Get
	  @iUnderlyingID int = Null,		 
	  @dtMinExpiry datetime = Null,
	  @iOptionID int = Null,
	  @bIsBestPriceOnly bit = 0
  as
   select	C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName, 
			O.underlyingContractID as iUnderlyingContractID,
			C2.contractTypeID as iUnderlyingContractTypeID,
			O.strike as fStrike, 
			EC.expiryDate as dtExpiry,
			O.isCall as bIsCall,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.lotSize as iLotSize, 
			CP.priceLast as fPriceLast,
			CP.priceClose as fPriceClose,
			CP.priceOpen as fPriceOpen,
			CP.priceLow as fPriceLow, 
			CP.priceHigh as fPriceHigh,
			CP.volume as iVolume,
			CP.openInterest as iOpenInterest, 
			E.exchangeID as iExchangeID,
			E.exchangeCode as vcExchangeCode,
			CP.priceTheo as fPriceTheo 
			  from ContractView c  
					join OptionView O on O.optionID = C.contractID
					join ContractView c2 on C2.contractID = O.underlyingContractID
					join ExpiryInCalendar EC on O.expiryID = EC.expiryID and c2.expCalendarID = EC.expCalendarID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
			where c.contractTypeID=3
					and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
					and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
					and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
					and (@bIsBestPriceOnly = 0 or @bIsBestPriceOnly = 1 and CP.exchangeID is null)
			order by o.underlyingContractID, year(EC.expiryDate), month(EC.expiryDate), o.strike,E.exchangeCode, o.isCall desc
 
  if (@@error <> 0)
	   return (-1)
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





CREATE        proc dbo.usp_Option_Get
	@iUnderlyingID int = Null,		 
	@dtMinExpiry datetime = Null,
	@iOptionID int = Null,
	@tiBaseInfoOnly tinyint = 0,
	@dtActionDate datetime = null,
	@tiWithPosition tinyint = 0,
	@iTraderID int = null
  as
/*
	Created by Zaur Nuraliev
	[Description]: this procedure return inf. from OptionView table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if isnull(@tiWithPosition, 0) = 0
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				CP.lotSize as iLotSize
			from ContractView C  
				inner join [Option] O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				inner join Contract C1 on O.underlyingContractID = C1.contractID
				inner join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
				and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
			order by o.underlyingContractID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				CP.lotSize as iLotSize
			from ContractView c  
				inner join [Option] O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				inner join Contract C1 on O.underlyingContractID = C1.contractID
				inner join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
				and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
	end
	else
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				CP.lotSize as iLotSize
			from ContractView C  
				inner join [Option] O on O.optionID = C.contractID
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0 and (tr.traderID = @iTraderID or @iTraderID is Null)
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				inner join Contract C1 on O.underlyingContractID = C1.contractID
				inner join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
--				and (tr.traderID = @iTraderID or @iTraderID is Null)
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
				and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName, O.underlyingContractID,
				O.strike, EC.expiryDate, O.isCall, O.actionDate, O.actionID,CP.lotSize
			order by o.underlyingContractID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity  * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				CP.lotSize as iLotSize
			from ContractView c  
				inner join [Option] O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null						
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0 and (tr.traderID = @iTraderID or @iTraderID is Null)
				inner join Contract C1 on O.underlyingContractID = C1.contractID
				inner join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
--				and (tr.traderID = @iTraderID or @iTraderID is Null)
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
				and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName, O.underlyingContractID,
				O.strike, EC.expiryDate, O.isCall, O.actionDate, O.actionID,CP.lotSize
	end


	set @error = @@error
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




CREATE       proc dbo.usp_Stock_Get
	@iGroupID int = null,
	@iStockID int = null,
	@dtActionDate datetime = null,
	@tiWithPosition tinyint = 0,
	@iTraderID int = null
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Stock,Contract tables
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if isnull(@tiWithPosition, 0) = 0
	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
				from ContractView C  
					inner join Stock S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID = 2
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
					and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
				order by c.Symbol
			else
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
				from ContractView C  
					inner join Stock S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
					and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
				order by c.Symbol
		end
		else
		begin
			if @iTraderID is null
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join Stock S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
					and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
				order by c.Symbol
			else
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join Stock S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
				order by c.Symbol
		end
	end
	else
	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
			    from ContractView c  
					inner join Stock S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
			    where c.contractTypeID = 2
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
					and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
					S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID
				order by c.Symbol
			else
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 

					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
			    from ContractView c  
					inner join Stock S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			    where c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
					and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
					S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID
				order by c.Symbol
		end
		else
		begin
			if @iTraderID is null
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
			    from ContractInGroupView cig
					inner join ContractView c  on C.contractID = CIG.contractID
					inner join Stock S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
		        where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
					and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
					S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID
				order by c.Symbol
			else
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.defaultModelTypeID as iDefaultModelTypeID, 
					S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
					S.DivFreq as iDivFreq,
					S.BaseIndexID as iBaseIndexID,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					CP.lotSize as iLotSize,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID
			    from ContractInGroupView cig
					inner join ContractView c  on C.contractID = CIG.contractID
					inner join Stock S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
		        where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
					S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID
				order by c.Symbol
		end
	end
	
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


CREATE proc dbo.usp_UnderlyingOptions_Get
	@iUnderlyingID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0,
	@dtMinExpiry datetime = Null
AS
	set nocount on
	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					inner join Contract uc on o.underlyingContractID = uc.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where c.contractTypeID = 3
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					inner join Contract uc on o.underlyingContractID = uc.contractID
				    inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize
				from ContractInGroupView cig
					inner join ContractView uc on cig.contractID = uc.contractID
					inner join [Option] o on uc.contractID = o.underlyingContractID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize
				from ContractInGroupView cig
					inner join ContractView uc on cig.contractID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on uc.contractID = o.underlyingContractID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize,
					sum(tr.quantity  * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					inner join Contract uc on o.underlyingContractID = uc.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where c.contractTypeID = 3
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, o.underlyingContractID,
					o.strike, ec.expiryDate, o.isCall, cp.lotSize
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize,
					sum(tr.quantity  * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					inner join Contract uc on o.underlyingContractID = uc.contractID
				    inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, o.underlyingContractID,
					o.strike, ec.expiryDate, o.isCall, cp.lotSize
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize,
					sum(tr.quantity  * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
					inner join ContractView uc on cig.contractID = uc.contractID
					inner join [Option] o on uc.contractID = o.underlyingContractID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, o.underlyingContractID,
					o.strike, ec.expiryDate, o.isCall, cp.lotSize
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					o.underlyingContractID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					cp.lotSize as iLotSize,
					sum(tr.quantity  * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
					inner join ContractView uc on cig.contractID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on uc.contractID = o.underlyingContractID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or o.underlyingContractID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, o.underlyingContractID,
					o.strike, ec.expiryDate, o.isCall, cp.lotSize
				order by o.underlyingContractID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
	end

	Return (0)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





CREATE      proc dbo.usp_Underlying_Get
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
				    cp.priceClose, cp.lotSize
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
				    cp.priceClose, cp.lotSize
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
				    cp.priceClose, cp.lotSize
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
				    cp.priceClose, cp.lotSize
				order by c.symbol	 
		end
	end

	Return (0)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
--============================================================================================================
--change DB version
--============================================================================================================
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.19')
	begin
		update DataInfo set keyValue = '3.19' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.19'
	end

