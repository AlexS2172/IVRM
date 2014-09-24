/*
	New features for ETS floor version (see update of spec)
*/

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Underlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Underlying_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE         proc dbo.usp_Index_Get
	@iGroupID int = Null,
	@iIndexID int = Null,
	@dtActionDate datetime = Null,
	@tiWithPosition tinyint = 0
  as
	if isnull(@tiWithPosition, 0) = 0
	begin
		if (@iGroupID is Null)
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
					I.ExpCalendarTypeID as tiExpCalendarTypeID
				from ContractView C 	
						join [Index] I on I.indexID = C.contractID
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
					I.ExpCalendarTypeID as tiExpCalendarTypeID
				from ContractInGroupView cig
						join ContractView C  on C.contractID = CIG.contractID
						join [Index] I on I.indexID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				order by C.symbol
	end
	else
	begin
		if (@iGroupID is Null)
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
					I.ExpCalendarTypeID as tiExpCalendarTypeID
				from ContractView c 	
						join [Index] I on I.indexID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
						left join TradeView tr on tr.contractID = c.contractID and tr.isSimulated = 0
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,I.ExpCalendarTypeID
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
					I.ExpCalendarTypeID as tiExpCalendarTypeID
				from ContractInGroupView cig
						join ContractView C  on C.contractID = CIG.contractID
						join [Index] I on I.indexID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
						left join TradeView tr on tr.contractID = c.contractID and tr.isSimulated = 0
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,I.ExpCalendarTypeID
				order by C.symbol
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


CREATE      proc dbo.usp_Option_Get
	@iUnderlyingID int = Null,		 
	@dtMinExpiry datetime = Null,
	@iOptionID int = Null,
	@tiBaseInfoOnly tinyint = 0,
	@dtActionDate datetime = null,
	@tiWithPosition tinyint = 0
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
				O.expiry as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				CP.lotSize as iLotSize
				  from ContractView C  
						inner join [Option] O on O.optionID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID=3
						and ((O.expiry >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
						and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
						and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
						and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
				order by o.underlyingContractID, year(o.expiry), month(o.expiry), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				O.expiry as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				CP.lotSize as iLotSize
				  from ContractView c  
						inner join [Option] O on O.optionID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID=3
						and ((O.expiry >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
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
				O.expiry as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				CP.lotSize as iLotSize
				  from ContractView C  
						inner join [Option] O on O.optionID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID and tr.isSimulated = 0
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID=3
						and ((O.expiry >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
						and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
						and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
						and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, O.underlyingContractID,
					O.strike, O.expiry, O.isCall, O.actionDate, O.actionID,CP.lotSize
				order by o.underlyingContractID, year(o.expiry), month(o.expiry), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				O.expiry as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity  * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				CP.lotSize as iLotSize
				  from ContractView c  
						inner join [Option] O on O.optionID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID and tr.isSimulated = 0
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID=3
						and ((O.expiry >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
						and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
						and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
						and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, O.underlyingContractID,
					O.strike, O.expiry, O.isCall, O.actionDate, O.actionID,CP.lotSize
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

CREATE     proc dbo.usp_Stock_Get
	@iGroupID int = null,
	@iStockID int = null,
	@dtActionDate datetime = null,
	@tiWithPosition tinyint = 0
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
						S.ExpCalendarTypeID as tiExpCalendarTypeID
				    from ContractView C  
						join Stock S on S.stockID = C.contractID
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
						S.ExpCalendarTypeID as tiExpCalendarTypeID
				    from ContractInGroupView cig
						join ContractView C  on C.contractID = CIG.contractID
						join Stock S on S.stockID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
			        where CIG.groupID = @iGroupID 
						and c.contractTypeID = 2
						and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
				   order by c.Symbol
	end
	else
	begin
		if (@iGroupID is Null)
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
						S.ExpCalendarTypeID as tiExpCalendarTypeID
				    from ContractView c  
						join Stock S on S.stockID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
						left join TradeView tr on tr.contractID = c.contractID and tr.isSimulated = 0
				    where c.contractTypeID = 2
						and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
						and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
					group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
						S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
						S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
						S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
						S.CalcOptionType,S.CalcModelType,S.ExpCalendarTypeID
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
						S.ExpCalendarTypeID as tiExpCalendarTypeID
				    from ContractInGroupView cig
						join ContractView c  on C.contractID = CIG.contractID
						join Stock S on S.stockID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
						left join TradeView tr on tr.contractID = c.contractID and tr.isSimulated = 0
			        where CIG.groupID = @iGroupID 
						and c.contractTypeID = 2
						and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
					group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
						S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
						S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
						S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
						S.CalcOptionType,S.CalcModelType,S.ExpCalendarTypeID
					order by c.Symbol
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



CREATE   proc dbo.usp_Underlying_Get
	@iContractID int = null
AS
	select
	    c.contractID as iContractID,
	    c.contractTypeID as iContractTypeID,
	    c.Symbol as vcSymbol,
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


Return (0)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
set nocount on 
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.13')
	begin
		update DataInfo set keyValue = '3.13' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.13'
	end

