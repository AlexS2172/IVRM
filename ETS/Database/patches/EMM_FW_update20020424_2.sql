if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ContractView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ContractView]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  View dbo.ContractView
as
	Select 
		contractID,
		contractTypeID,
		symbol,
		contractName,
		ruleID,
		groupID,
		actionID,
		actionDate,
		expCalendarID 
	from Contract
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[CustomStrikeSkewPointView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[CustomStrikeSkewPointView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[OptionView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[OptionView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[vALLStockEOD]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[vALLStockEOD]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ExpiryView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[ExpiryView]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  View dbo.ExpiryView
as
	Select 
		expiryID,
		expiryPeriod,
		actionID,
		actionDate
	from Expiry
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE         view dbo.vALLStockEOD
AS
SELECT 	C.contractID AS iContractID,   
		C.contractTypeID AS iContractTypeID,    
		C.Symbol AS vcSymbol, 
    	S.divFreq AS iDivFreq, 
		S.divAmt AS fDivAmt,  
		S.divDate AS dtDivDate,
		S.isHTB as bIsHTB, 
		S.divAmtCustom,
		S.divDateCustom,
		S.divFreqCustom,
		S.isDivCustom,
		S.CalcOptionType as tiCalcOptionType,
		S.CalcModelType as tiCalcModelType,
		C.expCalendarID as tiExpCalendarID
	--FROM ContractView C (nolock), StockView S (nolock)
	FROM Contract C (nolock), StockView S (nolock)
	WHERE S.stockID = C.contractID
UNION
SELECT 	C.contractID AS iContractID,  
		C.contractTypeID AS iContractTypeID,
	    C.Symbol AS vcSymbol,  
		NULL AS iDivFreq, 
		I.yield AS fDivAmt, 
		NULL AS dtDivDate,
		I.isHTB as bIsHTB, 
		null as divAmtCustom,
		null as divDateCustom,
		null as divFreqCustom,
		null as isDivCustom,
		I.CalcOptionType as tiCalcOptionType,
		I.CalcModelType as tiCalcModelType,
		C.expCalendarID as tiExpCalendarID
	--FROM ContractView C (nolock), IndexView I (nolock)
	FROM Contract C (nolock), IndexView I (nolock)
	WHERE I.indexID = C.contractID


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  View dbo.OptionView
as
	Select 
		optionID,
		underlyingContractID,
		isCall,
		strike,
		expiryID,
		actionID,
		actionDate
	from [Option]
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE View dbo.CustomStrikeSkewPointView
as
	Select 
		customStrikeSkewPointID,
		volaSurfaceDataID,
		strike,
		type,
		volatility,
		expiryID,
		isBasePoint,
		actionID,
		actionDate
	from CustomStrikeSkewPoint
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER   Trigger tU_IndexDefinition ON dbo.IndexDefinition
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================

	update IndexDefinition set actionDate=GetDate(),actionID=2
		from IndexDefinition
				join inserted on IndexDefinition.indexDefinitionID=inserted.indexDefinitionID
		where inserted.actionID < 3 --for eliminate set of 'delete' flag operation

	update IndexDefinition set actionID=3, actionDate=deleted.actionDate
		from IndexDefinition
				join deleted on IndexDefinition.indexDefinitionID=deleted.indexDefinitionID
		where deleted.actionID = 3 --for eliminate set 3-->1

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ActiveSurface_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ActiveSurface_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractBySymbol_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractBySymbol_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractForTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractForTrade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewData_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewData_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewDefaultPoint_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewDefaultPoint_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPoint_Copy]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewPoint_Copy]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPoint_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewPoint_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPoint_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewPoint_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomVolatilityPointIsExist_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomVolatilityPointIsExist_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Expiry_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Expiry_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Expiry_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Expiry_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Expiry_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Expiry_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FixMarketStructure]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FixMarketStructure]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinition_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinition_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_InterestRateCurve_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_InterestRateCurve_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionVM_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionVM_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionWithPrice_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Import]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Position_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Position_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RMScenario_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RMScenario_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportMarketStructure_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ReportMarketStructure_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportStock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ReportStock_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ReportTrade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Import]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Underlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Underlying_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_allStockEOD_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_allStockEOD_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_allStockOptionEOD_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_allStockOptionEOD_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE            Procedure dbo.usp_ActiveSurface_Get 
	@vcSymbolName varchar(12) --the contract symbol
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure return default surface identifier
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iContractID int
		select @iContractID = contractID from ContractView where symbol = @vcSymbolName and  contractTypeID < 3

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int 
	set @iVolaSurfaceDataID = 0
		exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	select	@iVolaSurfaceDataID as iVolaSurfaceDataID
/*
	select	volaSurfaceDataID as iVolaSurfaceDataID
		from VolaSurfaceDataView V
				join ContractView C on V.contractID=C.contractID
		where C.symbol = @vcSymbolName 
				and V.isDefaultSurface=1
*/
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


CREATE	proc dbo.usp_ContractBySymbol_Get
		@vcSymbol varchar(8)
as
begin
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.priceTheo as fPriceTheo,
					CP.priceDate as fPriceDate,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID,
					E.exchangeName as vcExchangeName
			from ContractView c 
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
			where C.symbol = @vcSymbol	
					and c.contractTypeID in (1, 2)
	union
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.priceTheo as fPriceTheo,
					CP.priceDate as fPriceDate,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID,
					E.exchangeName as vcExchangeName
			from ContractView c 
					join OptionView O on O.optionID = C.contractID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID				
		   where c.symbol = @vcSymbol  
					and c.contractTypeID not in (1, 2)
end





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE          proc dbo.usp_ContractForTrade_Get
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
				EC.expiryDate as dtExpiry,
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
				left join Contract UC on O.underlyingContractID = UC.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				left join ContractPrice CP on C.contractID = CP.contractID
				left join ContractPrice ucp on uc.contractID = ucp.contractID
				left join StockView s on uc.contractID = s.stockID
				left join IndexView i on uc.contractID = i.indexID			
		where
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and O.underlyingContractID = @iUnderlyingID)
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
				EC.expiryDate as dtExpiry,
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
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				left join ContractPrice CP on C.contractID = CP.contractID
				left join ContractPrice ucp on uc.contractID = ucp.contractID
				left join StockView s on uc.contractID = s.stockID
				left join IndexView i on uc.contractID = i.indexID			
		where
				(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
				and (@iUnderlyingID is Null or @iUnderlyingID is not Null and O.underlyingContractID = @iUnderlyingID)
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
SET ANSI_NULLS OFF 
GO


CREATE       Procedure dbo.usp_Contract_Get
	@iContractID int = null,	-- contract's identifier
	@iContractTypeID int = null,	-- contract's type identifier
	@bAllWithoutOptions bit = 1,	-- flag for data output
	@dtActionDate datetime = null
as
/*
	Created by Andrey Sinkin
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	if @bAllWithoutOptions is null		set @bAllWithoutOptions = 0

	if @bAllWithoutOptions = 1
		select 	Contract.contractID iContractID, 
				Contract.symbol vcSymbolName, 
				Contract.contractName vcDescription, 
				Stock.primaryExchangeID iExchangeID, 
				Contract.contractTypeID iContractTypeID, 
				Stock.divFreq iDivFrequency, 
				Stock.divAmt fAmount, 
				Stock.divDate dtDivDate,
				IndexView.yield fYield,
				Contract.actionDate as dtActionDate,
				Contract.actionID as tiActionID,
				case 
					when Stock.calcOptionType is not null then Stock.calcOptionType 
					else IndexView.calcOptionType end tiCalcOptionType,
				case 
					when Stock.IsHTB is not null then Stock.IsHTB 
					else IndexView.IsHTB end bIsHTB
			from Contract
				left join StockView Stock on Contract.contractID = Stock.stockID
				left join IndexView on Contract.contractID = IndexView.indexID
			where contractTypeID < 3
				and (@iContractID is null or contractID = @iContractID)
				and (Contract.actionDate>=@dtActionDate or @dtActionDate is null and Contract.actionID<3)
			order by Contract.symbol
	else
		select 	Contract.contractID iContractID, 
				Contract.symbol vcSymbolName, 
				Contract.contractName vcDescription, 
				Stock.primaryExchangeID iExchangeID, 
				Contract.contractTypeID iContractTypeID, 
				Stock.divFreq iDivFrequency, 
				Stock.divAmt fAmount, 
				Stock.divDate dtDivDate,
				IndexView.yield fYield,
				Contract.actionDate as dtActionDate,
				Contract.actionID as tiActionID,
				case 
					when Stock.calcOptionType is not null then Stock.calcOptionType 
					else IndexView.calcOptionType end tiCalcOptionType,
				case 
					when Stock.IsHTB is not null then Stock.IsHTB 
					else IndexView.IsHTB end bIsHTB
			from Contract
				left join StockView Stock on Contract.contractID = Stock.stockID
				left join IndexView on Contract.contractID = IndexView.indexID
			where (@iContractTypeID is null or contractTypeID = @iContractTypeID)
				and (@iContractID is null or contractID = @iContractID)
				and (Contract.actionDate>=@dtActionDate or @dtActionDate is null and Contract.actionID<3)
			order by Contract.symbol
	
Return(@@Error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE     proc usp_Contract_Save
	@iContractID int,
	@vcSymbol varchar(8),
	@vcContractName varchar(255),
	@iLotSize int,
	@fPriceBid float,
	@fPriceAsk float,
	@iExchangeID int,
	@fPriceLast float,
	@fPriceOpen float,
	@fPriceClose float,
	@fPriceLow float,
	@fPriceHigh float,
	@iVolume int,
	@iOpenInterest int

  as

-------------------------------------------------
set nocount on
-------------------------------------------------
  begin tran
	update Contract
		 set symbol = @vcSymbol,
			   contractName = CASE WHEN contractTypeID in (1, 2) THEN @vcContractName ELSE contractName END
	  where contractID = @iContractID
   if (@@error <> 0) begin rollback tran return @@error end  

	if exists(select * from ContractPrice where isnull(exchangeID,0) = isnull(@iExchangeID,0) and contractID = @iContractID)	
		begin
			update ContractPrice 
				set lotSize = @iLotSize,
					priceBid = @fPriceBid,
					priceAsk = @fPriceAsk,
					priceLast = @fPriceLast,
					priceOpen = @fPriceOpen,
					priceClose = @fPriceClose,
					priceLow = @fPriceLow,
					priceHigh = @fPriceHigh,
					volume = @iVolume
				where isnull(exchangeID,0) = isnull(@iExchangeID,0) and contractID = @iContractID
			   if (@@error <> 0) begin rollback tran return @@error end  
		end
	else 
		begin
			insert into ContractPrice (contractID,
					exchangeID,
					lotSize,
					priceBid,
					priceAsk,
					priceLast,
					priceOpen,
					priceClose,
					priceLow,
					priceHigh,
					volume)
			values (@iContractID,
					@iExchangeID,
					@iLotSize,
					@fPriceBid,
					@fPriceAsk,
					@fPriceLast,
					@fPriceOpen,
					@fPriceClose,
					@fPriceLow,
					@fPriceHigh,
					@iVolume)
		   if (@@error <> 0) begin rollback tran return @@error end  
		end

   if (@iOpenInterest is not Null)
	  begin
		declare @ctID int
		select @ctID = contractTypeID from Contract where contractID = @iContractID
		if (@ctID = 3)
		  begin
			update ContractPrice
				set openInterest = @iOpenInterest
				where contractID = @iContractID and isnull(exchangeID,0) = isnull(@iExchangeID,0)
			   if (@@error <> 0) begin rollback tran return @@error end  
		  end	
	  end
  commit tran

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


CREATE     Procedure dbo.usp_CustomStrikeSkewData_Get
	@iVolaSurfaceDataID int,
	@dtExpDate datetime = null,
	@iSetupMode int,
	@iCustomStrikeSkewID int = null,
	@iCustomStrikeSkewPointID int = null,
	@bType bit = 1
as
/*
	this procedure gets data from CustomStrikeSkewData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	-- truncate time information
	--set @dtExpDate = convert(varchar(8), @dtExpDate, 112)
	declare	@iExpiryID int
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)

	if @bType is null	set @bType = 1

	if @iExpiryID is not null
--	if @dtExpDate is not null
		begin
			if not exists(select * 
							from CustomStrikeSkewPointView C
									join VolaSurfaceData  on C.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
							where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and C.expiryID = @iExpiryID and type = @bType)
--			set @dtExpDate = null
			set @iExpiryID = null		

			select distinct CustomStrikeSkewPoint.customStrikeSkewPointID iCustomStrikeSkewPointID, 
				CustomStrikeSkewPoint.volaSurfaceDataID iVolaSurfaceDataID, 
				setupMode iSetupMode, 
				strike fStrike, 
				case when expiryID = @iExpiryID then isnull(volatility, 0) else 0 end fVolatility, 
				case when expiryID = @iExpiryID then CustomStrikeSkew.customStrikeSkewID else null end iCustomStrikeSkewID, 
				case when expiryID = @iExpiryID then E.expiryDate else null end dtExpDate,
				type bType
		
				from CustomStrikeSkewPointView
						join VolaSurfaceData on CustomStrikeSkewPoint.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
						join Contract C on  VolaSurfaceData.contractID = C.contractID
						join ExpiryInCalendar E on E.expCalendarID = C.expCalendarID
						left join CustomStrikeSkewData on CustomStrikeSkewPoint.customStrikeSkewPointID = CustomStrikeSkewData.customStrikeSkewPointID
						left join CustomStrikeSkew on CustomStrikeSkewData.customStrikeSkewID = CustomStrikeSkew.customStrikeSkewID
				where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and setupMode = @iSetupMode and type = @bType
					and (@dtExpDate is null or expiry = @dtExpDate)
					and (@iCustomStrikeSkewID is null or CustomStrikeSkew.customStrikeSkewID = @iCustomStrikeSkewID)
					and (@iCustomStrikeSkewPointID is null or CustomStrikeSkewPoint.customStrikeSkewPointID = @iCustomStrikeSkewPointID)
/*
			select distinct CustomStrikeSkewPoint.customStrikeSkewPointID iCustomStrikeSkewPointID, 
				CustomStrikeSkewPoint.volaSurfaceDataID iVolaSurfaceDataID, 
				setupMode iSetupMode, 
				strike fStrike, 
				case when expiry=@dtExpDate then isnull(volatility, 0) else 0 end fVolatility, 
				case when expiry=@dtExpDate then CustomStrikeSkew.customStrikeSkewID else null end iCustomStrikeSkewID, 
				case when expiry=@dtExpDate then expiry else null end dtExpDate,
				type bType
		
				from CustomStrikeSkewPointView
						inner join VolaSurfaceData on CustomStrikeSkewPoint.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
						left join CustomStrikeSkewData on CustomStrikeSkewPoint.customStrikeSkewPointID = CustomStrikeSkewData.customStrikeSkewPointID
						left join CustomStrikeSkew on CustomStrikeSkewData.customStrikeSkewID = CustomStrikeSkew.customStrikeSkewID
				where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and setupMode = @iSetupMode and type = @bType
					and (@dtExpDate is null or expiry = @dtExpDate)
					and (@iCustomStrikeSkewID is null or CustomStrikeSkew.customStrikeSkewID = @iCustomStrikeSkewID)
					and (@iCustomStrikeSkewPointID is null or CustomStrikeSkewPoint.customStrikeSkewPointID = @iCustomStrikeSkewPointID)
*/
		end
	else
		select  CustomStrikeSkewPointView.customStrikeSkewPointID iCustomStrikeSkewPointID, 
				CustomStrikeSkewPointView.volaSurfaceDataID as ivolaSurfaceDataID, 
				0 iSetupMode, 
				strike fStrike, 
				volatility as fVolatility, 
				0 as iCustomStrikeSkewID, 
				E.expiryDate as dtExpDate,
				type as bType
			from CustomStrikeSkewPointView
				join VolaSurfaceData on CustomStrikeSkewPointView.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
				join Contract C on  VolaSurfaceData.contractID = C.contractID
				join ExpiryInCalendar E on E.expCalendarID = C.expCalendarID
			where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and type = @bType
					and (@iCustomStrikeSkewPointID is null or CustomStrikeSkewPointView.customStrikeSkewPointID = @iCustomStrikeSkewPointID)
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


CREATE   procedure dbo.usp_CustomStrikeSkewDefaultPoint_Save
	@iContractID int,
	@dtExpiry smalldatetime,
	@iType int,
	@fVolatility float
as
/*
	Created by Zaur Nuraliev
	Description: this procedure ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int set @iVolaSurfaceDataID = 0

	declare	@iExpiryID int
		set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

--select @dtExpiry as '@dtExpiry', @iVolaSurfaceDataID as '@iVolaSurfaceDataID'
	if @iVolaSurfaceDataID is not null
		begin
			insert into CustomStrikeSkewPoint(VolaSurfaceDataID,strike,type,volatility,isBasePoint,expiryID)
				select distinct @iVolaSurfaceDataID, strike as fStrike, @iType,@fVolatility,-1,@iExpiryID --convert(varchar(6),expiry,112) + '01' as expiry
					from OptionView 
					where OptionView.expiryID = @iExpiryID and underlyingContractID = @iContractID
						and not exists(select * 
											from CustomStrikeSkewPointView 
											where VolaSurfaceDataID = @iVolaSurfaceDataID and 
													strike = OptionView.strike 
													and CustomStrikeSkewPointView.expiryID = OptionView.expiryID)
					--and convert(varchar(8),expDate,112) = convert(varchar(6),expiry,112) + '01')
		end


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE   Procedure dbo.usp_CustomStrikeSkewPoint_Copy 
	@iVolaSurfaceDataIDSrc int,
	@iVolaSurfaceDataIDDest int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is move information inside CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		select 	volaSurfaceDataID,
				strike,
				type,
				volatility,
				expiryID,
				isBasePoint
			into #SourcePoints
			from CustomStrikeSkewPoint
			where volaSurfaceDataID=@iVolaSurfaceDataIDSrc
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
		delete CustomStrikeSkewPoint where volaSurfaceDataID=@iVolaSurfaceDataIDDest
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
	
		insert into CustomStrikeSkewPoint (volaSurfaceDataID,strike,type,volatility,expiryID,isBasePoint)
		select 	volaSurfaceDataID,
				strike,
				type,
				volatility,
				expiryID,
				isBasePoint
				from #SourcePoints
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
    set @error = 0
finish:
	drop table #SourcePoints
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


CREATE         Procedure dbo.usp_CustomStrikeSkewPoint_Get 
	@iCustomStrikeSkewPointID int = null, 
	@iVolaSurfaceDataID int = null,
	@iType int = null,
	@dtExpDate datetime = null,
	@dtActionDate datetime = null 
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure get information from 
					CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare	@iExpiryID int
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)


	select 
		CSSP.customStrikeSkewPointID as iCustomStrikeSkewPointID,
		CSSP.volaSurfaceDataID as iVolaSurfaceDataID,
		CSSP.strike as fStrike, 
		CSSP.type as iType,
		CSSP.volatility as fVolatility,
--		E.expiryDate as dtExpDate,
		convert(datetime,convert(varchar(6),E.expiryDate,112) + '01') as dtExpDate,
		CSSP.isBasePoint as iIsBasePoint,
		CSSP.actionDate as dtActionDate,
		CSSP.actionID as tiActionID
		from CustomStrikeSkewPoint CSSP
			join VolaSurfaceData on CSSP..volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
			join Contract C on  VolaSurfaceData.contractID = C.contractID
			join ExpiryInCalendar E on E.expCalendarID = C.expCalendarID
		where 
			(@iCustomStrikeSkewPointID is null or customStrikeSkewPointID=@iCustomStrikeSkewPointID)
		and (@iVolaSurfaceDataID is null or CSSP.volaSurfaceDataID=@iVolaSurfaceDataID) 
		and (@iType is null or type=@iType) 
		and (@dtExpDate is null or E.expiryID = @iExpiryID)
		and ((CSSP.actionDate>=@dtActionDate or @dtActionDate is null) and (@dtActionDate is not null or @dtActionDate is null and CSSP.actionID<3))

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


CREATE       Procedure dbo.usp_CustomStrikeSkewPoint_Save 
	@iCustomStrikeSkewPointID int = Null output,
	@iVolaSurfaceDataID int = Null,
	@iType int = Null,
	@fStrike float = Null,
	@fVolatility float = Null,
	@dtExpDate datetime = Null,
	@iIsBasePoint int = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save inf. into CustomStrikeSkewPoint table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare	@iExpiryID int
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)

	if exists (select * from CustomStrikeSkewPoint where 
						volaSurfaceDataID = @iVolaSurfaceDataID and 
						strike = @fStrike and 
						type = @iType and 
						expiryID = @iExpiryID
				and actionID < 3 and (@iCustomStrikeSkewPointID <> CustomStrikeSkewPointID or @iCustomStrikeSkewPointID is null))
		begin
			Raiserror ('Row in CustomStrikeSkewPoint table  with same volaSurfaceDataID, strike, type and expDate already exists!', 16, 1)
			Return (-1)
		end 
  
	Begin Transaction
--		if not exists(select * From CustomStrikeSkewPoint where customStrikeSkewPointID=@iCustomStrikeSkewPointID) 
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					expiryID,
					isBasePoint) 
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						@iExpiryID,
						@iIsBasePoint)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iCustomStrikeSkewPointID=@@IDENTITY
			end
		else
			begin
				update CustomStrikeSkewPoint set 
					volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					type = isnull(@iType,type),
					strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility),
					expiryID = isnull(@iExpiryID,@iExpiryID),
					isBasePoint = isnull(@iIsBasePoint,isBasePoint)
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction

    set @error = 0

finish:
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


CREATE     Procedure dbo.usp_CustomVolatilityPointIsExist_Get
	@vcSymbolName varchar(8) = null
as
/*
	Created by Zaur Nuraliev
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	select distinct 
			CV.contractID as iContractID,
			CV.symbol as vcSymbolName,
			EC.expiryDate as dtExpiry,
			case when CSSP.volaSurfaceDataID is null then 0  
			else 1 end 'CustomVolatilityPointsCount'
		from OptionView OV
			join ContractView CV on OV.underlyingContractID = CV.contractID
			join ExpiryInCalendar EC on OV.expiryID = EC.expiryID and EC.expCalendarID = CV.expCalendarID
			left join VolaSurfaceDataView VSD on OV.underlyingContractID = VSD.contractID and VSD.isDefaultSurface = 1
			left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
and OV.expiryID = CSSP.expiryID

--and Convert(varchar(6),OV.expiry,112) = Convert(varchar(6),CSSP.expDate,112)


		where 
			CSSP.volaSurfaceDataID is null and 
			(CV.symbol = @vcSymbolName or @vcSymbolName is null) 
		order by CV.contractID,EC.expiryDate


Return(0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE    Procedure dbo.usp_Expiry_Del
	@iExpiryID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int
	set @error = -1

	Raiserror ('Can''t delete expiration information ',16,1)
/*
		begin tran
			update expiry set actionID=3 where expiryID = @iExpiryID
			--delete expiry where expiryID = @iExpiryID
	
		  set @error = @@error
		  if (@error <> 0)
			begin
			  rollback tran
			  goto finish
			end 
		commit tran 
*/
  finish:	 
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


CREATE       Procedure dbo.usp_Expiry_Get
	@iExpiryID int = null,
	@vcExpiryPeriod varchar(30) = null,
	@dtActionDate datetime = null,
	@iExpCalendarID int = 1
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select Expiry.expiryID as iExpiryID, 
		expiryPeriod as vcExpiryPeriod, 
		EC.expiryDate as dtExpiryDate,
		EC.vegaWeight as fVegaWeight,
		actionDate as dtActionDate,
		actionID as tiActionID
		from Expiry
				join ExpiryInCalendar EC on Expiry.expiryID = EC.expiryID and EC.expCalendarID = @iExpCalendarID

		where ((expiryPeriod = @vcExpiryPeriod or @vcExpiryPeriod is null) and 
				(Expiry.expiryID = @iExpiryID or @iExpiryID is null))
			and (actionDate>=@dtActionDate or @dtActionDate is null and actionID<3)

	order by expiryDate

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

CREATE   Procedure dbo.usp_Expiry_Save
	@iExpiryID int,
	@vcExpiryPeriod varchar(30),
	@dtExpiryDate smalldatetime = Null,
	@fVegaWeight float = Null,
	@iExpCalendarID int = 1 
as
/*
	Changed by Zaur Nuraliev 20011204
	[Description]:	this procedure save expiry parametrs for GU interface
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction

	update Expiry
	   	set expiryPeriod = isNull(@vcExpiryPeriod,expiryPeriod)
		where expiryID = @iExpiryID
	if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

	if @dtExpiryDate is not null or @fVegaWeight is not null
		begin
			if exists(select * from ExpiryInCalendar where expiryID = @iExpiryID  and expCalendarID = @iExpCalendarID)
				begin
					update ExpiryInCalendar
					   	set vegaWeight = isNull(@fVegaWeight,vegaWeight),
							expiryDate = isNull(@dtExpiryDate,expiryDate)
						where expiryID = @iExpiryID and expCalendarID = @iExpCalendarID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else	
				begin
					insert into ExpiryInCalendar (expiryID,expCalendarID,vegaWeight,expiryDate)
						values(@iExpiryID,@iExpCalendarID,@fVegaWeight,@dtExpiryDate)

					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				end
		end

	Commit Transaction
    set @error = 0
finish:
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


CREATE proc dbo.usp_FixMarketStructure
@iNumberOfHours int = null,
@iContractID int = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure mark us deleted unusefull options
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iNumberOfHours is null 
		set @iNumberOfHours = 2

	Create table #Options_for_delete(OptionID int)

	insert into #Options_for_delete
		select O.OptionID--,A.MSUpdateDate,O.actionDate
			from [Option] O
				join (select contractID,MSUpdateDate from Contract where MSUpdateDate is not null and actionID<3) A 
					on O.underlyingContractID = A.contractID 
					and O.actionDate < A.MSUpdateDate 
			where O.actionID < 3 and O.isManualyEntered=0 and 
					datediff(hour,O.actionDate, A.MSUpdateDate) > @iNumberOfHours and 
					(A.contractID = @iContractID or @iContractID is null)

	exec usp_Option_Del
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t mark us delete useless options!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

    set @error = 0
finish:
	if @error<>0 
		Return(@error)
	else
		Return @iContractID


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE        Procedure dbo.usp_IndexDefinition_Save 
	@iIndexID int = null, --the index identifier
	@iStockID int = null, --the stock identifier
	@fWeight float = null,  --the weight
	@fBeta float = null,
	@vcIndexSymbol varchar(8) = null,
	@vcStockSymbol varchar(8) = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save data into IndexDefinition table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iIndexID is null and @iStockID is null and @vcIndexSymbol is null and @vcStockSymbol is null 
		begin
			Raiserror ('Impossible define identifiers of the Stock and Index!', 16, 1)
			Return (-1)
		end   

	if @vcIndexSymbol is not null and @iIndexID is null
		select	@iIndexID = contractID from Contract where symbol = @vcIndexSymbol and contractTypeID = 1 and actionID < 3
		if @iIndexID is null 
			begin
				Raiserror ('Impossible define identifier of the Index!', 16, 1)
				Return (-1)
			end   

	if @vcStockSymbol is not null and @iStockID is null 
		select	@iStockID = contractID from Contract where symbol = @vcStockSymbol and contractTypeID = 2 and actionID < 3
		if @iStockID is null 
			begin
				Raiserror ('Impossible define identifier of the Stock!', 16, 1)
				Return (-1)
			end   

	Begin Transaction
		if not exists(select * From IndexDefinitionView where indexID = @iIndexID and stockID = @iStockID) 
			begin
				insert into IndexDefinition (indexID,stockID,weight,beta) 
				values (@iIndexID,@iStockID,isnull(@fWeight,0),@fBeta)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update IndexDefinition
					set weight = isnull(@fWeight, weight),
						beta=isnull(@fBeta,beta)
					where indexID = @iIndexID and stockID = @iStockID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction
    set @error = 0
finish:
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



CREATE           proc dbo.usp_Index_Get
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
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
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
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
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
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket
				from ContractView c 	
						join [Index] I on I.indexID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
						left join TradeView tr on tr.contractID = c.contractID and tr.isSimulated = 0
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
				from ContractInGroupView cig
						join ContractView C  on C.contractID = CIG.contractID
						join [Index] I on I.indexID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
						left join TradeView tr on tr.contractID = c.contractID and tr.isSimulated = 0
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID,I.isBasket
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


CREATE   proc dbo.usp_Index_Save
		@iIndexID int,
		@vcSymbol varchar(8) = Null,
		@vcContractName varchar(255) = Null,
		@iDefaultModelTypeID int = Null,
		@fYield float = Null,
		@bIsHTB bit = Null,
		@tiCalcOptionType tinyint = null,
		@tiCalcModelType tinyint = null,
		@tiExpCalendarID tinyint = null,
		@tiIsBasket tinyint = null
   as
	---------------
	set nocount on
	---------------

	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID=1 and actionID < 3 and (@iIndexID <> contractID or @iIndexID is null))
		begin
			Raiserror ('Index with this ticker already exists!', 16, 1)
			Return (-1)
		end   

   if (@iIndexID is Null)  
	 begin
		 
		 begin tran
			 insert into Contract (contractTypeID, symbol, contractName,expCalendarID)
				  values (1, @vcSymbol, @vcContractName,@tiExpCalendarID)
			 select @iIndexID = @@identity			 
				 if (@@error <> 0)
					  begin
						RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
						ROLLBACK TRAN
						RETURN (-1)
					  end
			 insert into [Index] (indexID, defaultModelTypeID, yield,IsHTB,calcOptionType,calcModelType,isBasket)
					 values (@iIndexID, @iDefaultModelTypeID, @fYield,isNull(@bIsHTB,0),@tiCalcOptionType,@tiCalcModelType,isnull(@tiIsBasket,0))
				 if (@@error <> 0)
					begin
					   RAISERROR ('Error. Can''t insert into [Index]!', 16, 1)
					   ROLLBACK TRAN
					   RETURN (-1)
					end 			
		 commit tran
	 end
   else
	 begin
		begin tran
		   update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@vcContractName,contractName),
					expCalendarID = isNull(@tiExpCalendarID, expCalendarID)
				where contractID = @iIndexID
			if (@@error <> 0)
				 begin
					RAISERROR ('Error. Can''t update Contract!', 16, 1)
					ROLLBACK TRAN
					RETURN (-1)
				 end
		  update [Index]
			   set	defaultModelTypeID = isNull(@iDefaultModelTypeID,defaultModelTypeID),
					yield = isNull(@fYield,yield),
					isHTB = isNull(@bIsHTB,isHTB),
					calcOptionType = isNull(@tiCalcOptionType, calcOptionType),
					calcModelType = isNull(@tiCalcModelType, calcModelType),
					isBasket = isNull(@tiIsBasket,isBasket)
			   where indexID = @iIndexID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t update [Index]!', 16, 1)
				  ROLLBACK TRAN
				  RETURN (-1)
			   end		   
		 commit tran
	 end

	--========================================================================
	-- make record with null exchange for this index
	--========================================================================
	if not exists(select * from ContractPrice where exchangeID is null and contractID = @iIndexID)
		begin
			insert into ContractPrice(contractID,exchangeID)
				values(@iIndexID,null)
			if (@@error <> 0) begin rollback tran return(-1) end           		
		end		


if (@@error <> 0)
	return (-1)
else	
	return @iIndexID


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE  Procedure dbo.usp_InterestRateCurve_Get
	@iCurvePointID int = null,	-- InterestRateCurve identifier
	@bAdvancedMode bit = 0		-- flag for advanced mode
as
/*
	Created by Andrey Sinkin
	this procedure returns data from InterestRateCurve table (now from IRPoint table)
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @date datetime		set @date = getdate()
	if @bAdvancedMode = 0
		select 	IRPointID as iCurvePointID, 
				periodTypeID as iPeriodTypeID, 
				num as iNumber, 
				longRate as fRateAsk, 
				shortRate as fRateBid,
				HTBRate as fHTBRate
			from IRPointView
			where @iCurvePointID is null or IRPointID = @iCurvePointID
			order by iPeriodTypeID, iNumber
	else
		select	IRPointID as iCurvePointID, 
				periodTypeID as iPeriodTypeID, 
				num as iNumber, 
				longRate as fRateAsk, 
				shortRate as fRateBid,
			case periodTypeID
				when 1 then datediff(day, getdate(), dateadd(day, num, @date))
				when 2 then datediff(day, getdate(), dateadd(week, num, @date))
				when 3 then datediff(day, getdate(), dateadd(month, num, @date))
				when 4 then datediff(day, getdate(), dateadd(year, num, @date))
			end iDaysFromToday,
			HTBRate as fHTBRate
			from IRPointView
			where @iCurvePointID is null or IRPointID = @iCurvePointID
			order by iDaysFromToday

Return(@@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  Procedure dbo.usp_OptionVM_Get
	@iOptionID int = null,			-- option's identifier
	@iUnderlyingID int,				-- underlying's identifier
	@dtExpDateMin datetime = null,	-- expiration's date
	@bIsCall bit = null,
	@iSortOrder int = 1	--			-- 1, 2 or 3. see comments below
as
/*
	Created by Andrey Sinkin
	This procedure returns option's data
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iSortOrder = 1	-- sort by symbol
		select	O.OptionID as iOptionID, 
				O.underlyingContractID as iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				EC.expiryDate as  dtExpDate,
				C.symbol as vcSymbolName, 
				null as iExchangeID,
				C.contractName as vcDescription,
				CP.lotSize as iLotSize
				from OptionView O
						join ContractView C on O.OptionID = C.contractID
						join ContractPrice CP on CP.contractID = C.contractID and exchangeID is null
						join Contract C1 on O.underlyingContractID = C1.contractID
						join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				where O.underlyingContractID = @iUnderlyingID
						and (@iOptionID is null or O.OptionID = @iOptionID)
						and (@dtExpDateMin is null or EC.expiryDate >= @dtExpDateMin)
						and (@bIsCall is null or O.isCall = @bIsCall)
				order by C.symbol
	else
	if @iSortOrder = 2	-- sort by isCall+year(expiry)+month(expiry)+strike
		select	O.OptionID as iOptionID, 
				O.underlyingContractID as iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				EC.expiryDate as dtExpDate,
				C.symbol as vcSymbolName, 
				null iExchangeID,
				C.contractName as vcDescription, 
				ContractPrice.lotSize as iLotSize
			from OptionView O
					join ContractView C on O.OptionID = C.contractID
					join ContractPrice on ContractPrice.contractID = C.contractID and exchangeID is null
					join Contract C1 on O.underlyingContractID = C1.contractID
					join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where O.underlyingContractID = @iUnderlyingID
					and (@iOptionID is null or O.OptionID = @iOptionID)
					and (@dtExpDateMin is null or EC.expiryDate >= @dtExpDateMin)
					and (@bIsCall is null or O.isCall = @bIsCall)
			order by isCall, year(EC.expiryDate),  month(EC.expiryDate), strike
	else
	if @iSortOrder = 3	-- sort by expiry+strike+symbol(without two last symbols)+isCall
		select 	O.OptionID iOptionID,
				O.underlyingContractID iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				EC.expiryDate as dtExpDate,
				C.symbol vcSymbolName, 
				null iExchangeID,
				C.contractName vcDescription,	
				ContractPrice.lotSize iLotSize
			from OptionView O
					join ContractView C on O.OptionID = C.contractID
					join ContractPrice on ContractPrice.contractID = C.contractID and exchangeID is null
					join Contract C1 on O.underlyingContractID = C1.contractID
					join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where O.underlyingContractID = @iUnderlyingID
					and (@iOptionID is null or EC.expiryDate = @iOptionID)
					and (@dtExpDateMin is null or EC.expiryDate >= @dtExpDateMin)
					and (@bIsCall is null or O.isCall = @bIsCall)
			order by EC.expiryDate, strike, left(C.symbol, len(C.symbol) - 2), isCall desc
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
			E.exchangeCode as vcExchangeCode
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
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				CP.lotSize as iLotSize
				  from ContractView C  
						join [Option] O on O.optionID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
						join Contract C1 on O.underlyingContractID = C1.contractID
						join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
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
						join [Option] O on O.optionID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
						join Contract C1 on O.underlyingContractID = C1.contractID
						join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
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
						left join TradeView tr on tr.contractID = c.contractID and tr.isSimulated = 0
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
						join Contract C1 on O.underlyingContractID = C1.contractID
						join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				where c.contractTypeID=3
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
						join [Option] O on O.optionID = C.contractID
						join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null						
						left join TradeView tr on tr.contractID = c.contractID and tr.isSimulated = 0
						join Contract C1 on O.underlyingContractID = C1.contractID
						join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				where c.contractTypeID=3
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE        Procedure dbo.usp_Option_Import
	@iUnderlyingContractID int = null,  --underlying contractID
	@vcSymbol varchar(8),  -- option symbol
	@fPriceAsk float, 
	@fPriceBid float, 
	@iVolume int,
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
	@iOpenInterest int,
	@iExchangeID int = Null, -- the exchange identifier (special for ContractPrice update)
	@vcUnderlyingSymbol varchar(8) = null,  -- stock symbol
	@iLotSize int = null
as
/*
	Created by ... Redesigned by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare	@OptionID int, 
			@cn varchar(255),
			@iExpiryID int,
			@iLastExpiryID int

	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
	select top 1 @iLastExpiryID = expiryID from ExpiryInCalendar where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc
	select @dtExpiry = expiryDate from ExpiryInCalendar where expiryID = @iExpiryID and expCalendarID = 1


	--===================================================================			
	-- check the consistensy of input parameters (for stock)
	--===================================================================			
	declare @iUnderlyingFirstContractID int
		set @iUnderlyingFirstContractID = @iUnderlyingContractID
	--select underlying ID
	select @iUnderlyingContractID = contractID  
		from Contract 
		where symbol = @vcUnderlyingSymbol and contractTypeID=2
	if @iUnderlyingFirstContractID <> @iUnderlyingContractID
		begin
			Raiserror ('Wrong underlying contract ID in usp_Option_Import!', 16, 1)
			Return (-1)
		end  
	--select underlying option symbol
	if @vcUnderlyingSymbol is null
		select @vcUnderlyingSymbol=Contract.Symbol 
			from Contract (nolock) 	
			where Contract.contractID = @iUnderlyingContractID and Contract.contractTypeID < 3

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if (@OptionID is Null) 
		select @OptionID = optionID
			from [Option] (nolock)
			where underlyingContractID = @iUnderlyingContractID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	--===================================================================			
	-- try to find Option with same ticker in DB
	--===================================================================			
	declare @iIncorrectOptionID int

	select @iIncorrectOptionID = contractID 
		from Contract (nolock)
				join [Option] (nolock) on Contract.contractID = [Option].optionID
		where symbol = @vcSymbol and contractTypeID = 3 and 
			[Option].actionID < 3 and (@OptionID <> contractID) -- or @OptionID is null

	if @iIncorrectOptionID is not null
		begin
			-- mark as delete
			update [Option] set [Option].actionID = 3 
				where optionID = @iIncorrectOptionID and expiryID < @iLastExpiryID

			if @@rowcount = 0 
				begin
					--Raiserror ('Option with this symbol already exists!', 16, 1)
					--Return (-1)
					update [Option] set [Option].actionID = 3 
						where optionID = @iIncorrectOptionID
					update Contract set actionID = 3 
						where ContractID = @iIncorrectOptionID
				end   
			else
				update Contract set actionID = 3 
					where ContractID = @iIncorrectOptionID
		end

	--===================================================================			
	-- make the contract name
	--===================================================================			
--  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(day, @dtExpiry) + ' ' + DATENAME(month, @dtExpiry) + 
--					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
--						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(month, @dtExpiry) + 
					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)

  if exists (select contractID from Contract(nolock) where contractID = @iUnderlyingContractID and contractTypeID > 2)
	 begin
	   RAISERROR ('Error. Can''t insert values to [Option]. The Underlying Contract is not stock or index!', 16, 1)
	   RETURN (-1)
	 end 

  if (@OptionID is Null) 
	begin
		Begin Transaction
	
			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@cn)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
				 values (@OptionID, @iUnderlyingContractID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			insert into ContractPrice (contractID,exchangeID, priceBid, priceAsk, volume,openInterest,lotSize)
				 values (@OptionID,@iExchangeID, @fPriceBid, @fPriceAsk, @iVolume,@iOpenInterest,@iLotSize)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID,lotSize)
						values(@OptionID,null,@iLotSize)
					if (@@error <> 0) begin rollback tran return(-1) end           
				end 
		Commit Transaction
	end 
   else
	begin
		  update Contract
			   set symbol = @vcSymbol,
					 contractName = @cn
			where contractID = @OptionID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract!', 16, 1)
				  RETURN (-1)
			   end 
		  update ContractPrice
				set	priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					volume = @iVolume,
					openInterest=@iOpenInterest,
					lotSize = isnull(@iLotSize,lotSize)
				where contractID = @OptionID and isnull(exchangeID,0)=isnull(@iExchangeID,0)
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract price!', 16, 1)
				  RETURN (-1)
			   end 

			update [Option]
				set 
				IsCall = isnull(@iIsCall,IsCall),
				strike = isnull(@fStrike,Strike),
				expiryID = isnull(@iExpiryID,expiryID)
			where optionID = @OptionID

		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t update Option information (table Option)!', 16, 1)
				  RETURN (-1)
			   end 

	end 
	 
-- return @OptionID 
    set @error = 0
finish:
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



CREATE      proc dbo.usp_Option_Save
		@iOptionID int,
		@vcSymbol varchar(8) = Null,
		@iExchangeID int, 
		@iUnderlyingContractID int,
		@iIsCall tinyint = Null,
		@fStrike float = Null,
		@dtExpiry datetime = Null,
		@tiIsManualyEntered tinyint
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

   if exists (select contractID from ContractView where contractID = @iUnderlyingContractID and contractTypeID > 2)
	   begin
		 RAISERROR ('Error. Can''t insert or update Option values. The Underlying Contract is not stock or index!', 16, 1)
		 RETURN (-1)
	   end	 

	declare	@OptionID int, 
			@ContractName varchar(255),
			@iExpiryID int

	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
	select @dtExpiry = expiryDate from ExpiryInCalendar where expiryID = @iExpiryID and expCalendarID = 1


   if exists (select optionID from OptionView where underlyingContractID = @iUnderlyingContractID and 
					isCall = @iIsCall and 
					strike = @fStrike and 
					expiryID = @iExpiryID and
					(@iOptionID is Null or optionID <> @iOptionID))
		begin
		  RAISERROR ('Error. Can''t insert or update Option values. There are ambiguous Option in DB!', 16, 1)
		  RETURN (-1)
		end  

   set @ContractName = @vcSymbol + ' ' + datename(day, @dtExpiry) + ' ' + datename(month, @dtExpiry) + 
					' ' + datename(year, @dtExpiry) + ' ' + CASE @iIsCall when 1 then 'Call' else 'Put' end +
						 ' ' + cast(round(@fStrike, 2) as varchar)
 
	Begin Transaction
	   if (@iOptionID is Null)	
		 begin
			insert into Contract (contractTypeID, symbol, contractName)
				values (3, @vcSymbol, @ContractName)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iOptionID = @@identity			  

			insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
				values (@iOptionID, @iUnderlyingContractID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		 end
	   else
		 begin
			update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@ContractName,contractName)
				where contractID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			update [Option]
				set underlyingContractID = @iUnderlyingContractID,
					isCall = isNull(@iIsCall, isCall),
					strike = isNull(@fStrike, strike),
					expiryID = isNull(@iExpiryID, expiryID)
				where optionID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		 end		

		if @tiIsManualyEntered in (0,1) 
			begin
--select @iOptionID as '@iOptionID*******************'
				update [Option] set isManualyEntered = @tiIsManualyEntered where optionID = @iOptionID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end 

	Commit Transaction

    set @error = 0
finish:
if (@error <> 0) return (@error) else return @iOptionID





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE    PROCEDURE dbo.usp_Position_Get
	@iUnderlyingID int = Null,
	@iTraderID int = Null,
	@iBookID int = Null,
	@iStrategyID int = Null,
	@dtMaxDate datetime = null
AS

	declare @dtToday datetime
		set @dtToday = cast(floor(cast(getdate() as float)) as datetime)

	select 
		-- all
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
	
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
		c.Symbol as vcUnderlyingSymbol,
		
		-- trade
		sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * tr.lotSize * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when tr.tradeDate < @dtToday then 1 else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when tr.tradeDate >= @dtToday then tr.price else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		null as fPosDailyTheoPrevDay,
		null as fPriceTheoClose
	
	from TradeView tr 
		join ContractView c  on tr.contractID = c.contractID
		left join StockView s on c.contractID = s.stockID
		left join IndexView i on c.contractID = i.indexID
	where c.contractTypeID in (1, 2)
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
	group by c.contractID, c.contractTypeID, c.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield
	
	union

	select 
		-- all
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,

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
		uc.Symbol as vcUnderlyingSymbol,
		
		-- trade
		sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * tr.lotSize * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when tr.tradeDate < @dtToday then 1 else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when tr.tradeDate >= @dtToday then tr.price else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		sum((case when tr.tradeDate < @dtToday then isnull(cp.priceTheo, 0) else 0 end) * tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyTheoPrevDay,
		cp.priceTheo as fPriceTheoClose

	from TradeView tr 
		join ContractView C on tr.contractID = C.contractID
		join ContractPrice CP on C.contractID = cp.contractID and exchangeID is null
		left join (OptionView o join Contract UC on o.underlyingContractID = UC.contractID) on c.contractID = o.optionID
		join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
		left join StockView s on uc.contractID = s.stockID
		left join IndexView i on uc.contractID = i.indexID

	where c.contractTypeID = 3
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)

	group by C.contractID, C.contractTypeID, C.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield,
		EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol, cp.priceTheo

	order by vcSymbol



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE      Procedure dbo.usp_RMScenario_Save
	@iRMScenarioID int = Null output,
	@vcScenarioName varchar(30) = Null,
	@vcDescription varchar(120) = Null,
    @iPoints1 int = Null,
	@fStep1 int = Null,
	@tiIsAbs1 tinyint = Null,
	@tiType1 tinyint = Null,
	@iPoints2 int = Null,
	@fStep2 float= Null,
	@tiIsAbs2 tinyint = Null,
	@tiType2 tinyint = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
/*
	if exists (select * from RMScenario where scenarioName = @vcScenarioName and actionID < 3 and (@iRMScenarioID <> RMScenarioID or @iRMScenarioID is null))
		begin
			Raiserror ('Row in RMScenario table with same scenario name already exists!', 16, 1)
			Return (-1)
		end
*/   
	if @iRMScenarioID is null
	--if not exists(select * from RMScenario where RMScenarioID = @iRMScenarioID)
		begin
			insert into RMScenario(
					ScenarioName,
					[Description],
					points1,
					step1,
					isAbs1,
					type1,
					points2,
					step2,
					isAbs2,
					type2) 
				values (
					@vcScenarioName,
					@vcDescription,
				    @iPoints1,
					@fStep1,
					@tiIsAbs1,
					@tiType1,
					@iPoints2,
					@fStep2,
					@tiIsAbs2,
					@tiType2)
			select @iRMScenarioID = @@identity
		end
	else
		begin
			update RMScenario set
				ScenarioName = isNull(@vcScenarioName, ScenarioName),
				[Description] = isNull(@vcDescription, [Description]),
				points1 = isNull(@iPoints1, points1),
				step1 = isNull(@fStep1, step1),
				isAbs1 = isNull(@tiIsAbs1, isAbs1),
				type1 = isNull(@tiType1, type1),
				points2 = isNull(@iPoints2, points2),
				step2 = isNull(@fStep2, step2),
				isAbs2 = isNull(@tiIsAbs2, isAbs2),
				type2 = isNull(@tiType2, type2)
				where RMScenarioID = @iRMScenarioID
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



CREATE   Procedure dbo.usp_ReportMarketStructure_Get
	@iStockID int = Null, 
	@iFilterID int = Null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from [Option] table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare	@iLastExpiryID int
		select top 1 @iLastExpiryID = expiryID from ExpiryInCalendar where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc

	create table #FilteredStocks(stockID int)

	if @iStockID is null 
		begin
			insert into #FilteredStocks exec usp_FilterData_Get @iFilterID
		end

if @iStockID is not null and @iFilterID is null
	select
		O.underlyingContractID as iUnderlyingContractID,
		Contract.symbol as vcSymbol,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		EC.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where 	O.expiryID > @iLastExpiryID and 
				underlyingContractID=@iStockID

else if @iStockID is not null and @iFilterID is not null
	select
		O.underlyingContractID as iUnderlyingContractID,
		Contract.symbol as vcSymbol,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		EC.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where 	O.expiryID > @iLastExpiryID and
				underlyingContractID = @iStockID and 
				O.underlyingContractID in (select stockID from #FilteredStocks)

else if @iStockID is null and @iFilterID is not null
	select
		O.underlyingContractID as iUnderlyingContractID,
		Contract.symbol as vcSymbol,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		EC.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where O.expiryID > @iLastExpiryID and
			underlyingContractID in (Select stockID from #FilteredStocks)

else if @iStockID is null and @iFilterID is null
	select
		O.underlyingContractID as iUnderlyingContractID,
		Contract.symbol as vcSymbol,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		EC.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where O.expiryID > @iLastExpiryID
/*
	select
		O.underlyingContractID as iUnderlyingContractID,
		Contract.symbol as vcSymbol,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where O.expiryID > @iLastExpiryID and
	(
		(@iStockID is not null and @iFilterID is null and underlyingContractID=@iStockID) or 
		(@iStockID is null and @iFilterID is not null and underlyingContractID in (Select stockID from #FilteredStocks)) or 
		(@iStockID is not null  and @iFilterID is not null and (underlyingContractID=@iStockID and O.underlyingContractID in (Select stockID from #FilteredStocks))) or 
		(@iStockID is null and @iFilterID is null)
	)

*/
	if object_id('tempdb..#FilteredStocks') is not null
		drop table #FilteredStocks

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



CREATE     Procedure dbo.usp_ReportStock_Get
	@iStockID int = Null, 
	@iFilterID int = Null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from [Option] table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	--=====================================================
	-- work with filter
	--=====================================================
	create table #FilteredStocks(stockID int)
              insert into #FilteredStocks 
                      exec usp_FilterData_Get @iFilterID
	--=====================================================
	-- data selection
	--=====================================================
		select
			Contract.ContractID as iContractID,
			Contract.symbol as vcSymbol,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceLast as fPriceLast,
			Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
			case when isDivCustom = 1 then divFreqCustom else Stock.divfreq end iDivFreq,
			case when isDivCustom = 1 then divAmtCustom else Stock.divAmt end fDivAmt,
			case when isDivCustom = 1 then divDateCustom else Stock.DivDate end dtDivDate,
			S2.currentHV as fCurrentHV,
			CP.priceClose as fPriceClose,
	  		Stock.isHTB as bIsHTB,
			Stock.isDivCustom as tiIsDivCustom,
			Stock.CalcOptionType as tiCalcOptionType,
			Stock.CalcModelType as tiCalcModelType,
			Contract.expCalendarID as tiExpCalendarID
			from ContractView Contract
					join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
					join StockView Stock on Contract.contractID = Stock.StockID
					left join (select StockDataHistory.stockID,currentHV,[date] from StockDataHistory join
									(select stockID, max([date]) as maxDate from StockDataHistory group by stockID) S1
								on S1.stockID=StockDataHistory.stockID and S1.maxDate=StockDataHistory.[date]) S2 
						on S2.stockID=Stock.stockID
	
			where (
				(@iStockID is null and (Stock.stockID in (select stockID from #FilteredStocks)))
				or 
				(@iStockID is not null and Contract.ContractID=@iStockID 
					and (Stock.stockID in (select stockID from #FilteredStocks) or @iFilterID is null))
				)
				and Contract.contractTypeID = 2
	union
		select
			Contract.ContractID as iContractID,
			Contract.symbol as vcSymbol,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceLast as fPriceLast,
			Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
			0 as iDivFreq,
			I.yield as fDivAmt,
	  		null as dtDivDate,
			S2.currentHV as fCurrentHV,
			CP.priceClose as fPriceClose,
	  		I.isHTB as bIsHTB,
			null as tiIsDivCustom,
			I.CalcOptionType as tiCalcOptionType,
			I.CalcModelType as tiCalcModelType,
			Contract.expCalendarID as tiExpCalendarID
			from ContractView Contract
					join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
					join IndexView I on Contract.contractID = I.indexID
					left join (select IndexDataHistory.indexID,currentHV,[date] from IndexDataHistory join
									(select indexID, max([date]) as maxDate from IndexDataHistory group by indexID) S1
								on S1.indexID=IndexDataHistory.indexID and S1.maxDate=IndexDataHistory.[date]) S2 
						on S2.indexID=I.indexID
			where (
				(@iStockID is null and (I.indexID in (select stockID from #FilteredStocks))) or 
				(@iStockID is not null and @iFilterID is not null and (Contract.ContractID=@iStockID and (I.indexID in (select stockID from #FilteredStocks))))
				)
				and Contract.contractTypeID = 1
		order by Contract.symbol
	set @error = @@error
             drop table #FilteredStocks
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



CREATE         Procedure dbo.usp_ReportTrade_Get
	@iStockID int = Null, 
	@iFilterID int = Null,
	@dtBeginDate datetime = Null,
	@dtEndDate datetime = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from OptionView table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	create table #FilteredStocks(stockID int)
	
	-- for filtering by trades
	create table #T(tradeID int)
	if @iFilterID is not null
		begin
			declare @iTraderID int,
					@iBookID int,
					@iStrategyID int,
					@bTradeRowCount bit
			set @bTradeRowCount=0
			insert into #FilteredStocks
				exec usp_FilterData_Get @iFilterID, @iTraderID output, @iBookID output, @iStrategyID output
			if (@iTraderID<>0 or @iBookID<>0 or @iStrategyID<>0)
				begin
					set @bTradeRowCount = 1
					insert into #T
						exec usp_FilterTradeData_Get @iTraderID,@iBookID,@iStrategyID
				end
		end
--	select * from #T	
	select	@dtBeginDate = convert(datetime,convert(varchar(10),@dtBeginDate,112)),
			@dtEndDate = convert(datetime,convert(varchar(10),@dtEndDate,112))
	select 
		O.underlyingContractID as iUnderlyingContractID,
		EC.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		case T.isBuy when 0 then T.quantity * -1 else T.quantity end as iQuantity,
		T.tradeDate as dtTradeDate,
		T.price as fPriceTrade,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceLast as fPriceLast,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceTheo as fPriceTheo,
		T.traderID as iTraderID,
		T.bookID as iBookID,
		T.strategyID as iStrategyID,
		CP.priceClose as fPriceClose,
		Contract.symbol as vcSymbol,
		T.lotSize as iLotSize,
		EC.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join TradeView T on O.optionID = T.contractID 
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
				join Contract C1 on O.underlyingContractID = C1.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
		where EC.expiryDate > GetDate() and
			((
				(@iStockID is not null and @iFilterID is null and underlyingContractID=@iStockID) or 
				(@iStockID is null and @iFilterID is null) or
				(@iStockID is null and @iFilterID is not null and (underlyingContractID in (select stockID from #FilteredStocks)) and ((@bTradeRowCount=1 and T.tradeID in (select tradeID from #T)) or @bTradeRowCount=0)  or 
				(@iStockID is not null and @iFilterID is not null 
					and underlyingContractID=@iStockID 
					and (underlyingContractID in (select stockID from #FilteredStocks)) 
					and ((@bTradeRowCount=1 and T.tradeID in (select tradeID from #T)) or @bTradeRowCount=0))
			))
				and
				(
				(@dtBeginDate is  not null and @dtEndDate is not null and (convert(datetime,convert(varchar(10),T.tradeDate,112)) between @dtBeginDate and @dtEndDate)) or 
				(@dtBeginDate is not null and @dtEndDate is null and convert(datetime,convert(varchar(10),T.tradeDate,112)) >= @dtBeginDate) or
				(@dtBeginDate is null and @dtEndDate is not null and convert(datetime,convert(varchar(10),T.tradeDate,112)) <= @dtEndDate) or
				(@dtBeginDate is null and @dtEndDate is null)
				)
			)
	set @error = @@error
	if @iFilterID is not null
		begin
			drop table #FilteredStocks
		end
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



CREATE      proc dbo.usp_Stock_Get
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
						C.expCalendarID as iExpCalendarID
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
						C.expCalendarID as iExpCalendarID
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
						C.expCalendarID as iExpCalendarID
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
						S.CalcOptionType,S.CalcModelType,C.expCalendarID
					order by c.Symbol
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

CREATE proc dbo.usp_Stock_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@fPriceAsk float,
	@fPriceBid float,
	@fPriceOpen float,
	@fPriceClose float,
	@fPriceLow float,
	@fPriceHigh float,
	@iVolume int,
	@iExchangeID int = null,  --the ExchangeID special for save data into ContractPrice table
	@iLotSize int = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		update Contract
		    set ContractName = @vcContractName,
				MSupdateDate = getDate()
	 	where contractID = @iContractID
	
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update Contract Name!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end
		update ContractPrice 
			set priceAsk = @fPriceAsk,
			priceBid = @fPriceBid,
			PriceOpen = @fPriceOpen,
			PriceClose = @fPriceClose,
			PriceLow = @fPriceLow,
			PriceHigh = @fPriceHigh,
			Volume = @iVolume,
			lotSize = isnull(@iLotSize,lotSize)
		where contractID = @iContractID and (exchangeID=@iExchangeID or @iExchangeID is null)
		if (@@error <> 0)
			begin
				Raiserror ('Error. Can''t update Contract Price!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end
		--========================================================================
		-- make record with null exchange for this stock
		--========================================================================
		if not exists(select * from ContractPrice where exchangeID is null and contractID = @iContractID)
			begin
				insert into ContractPrice(contractID,exchangeID,lotSize)
					values(@iContractID,null,@iLotSize)
				if (@@error <> 0) begin rollback tran return(-1) end           
			end 
		--==================================================================================
		-- update DivAmnt & DivFreq values
		--==================================================================================
		if (@bIsStock <> 0 and exists(select * from stock where stockID=@iContractID))
			begin 
				Declare @df smallint
				update Stock 
					set divAmt = @fDivAmt,
						divDate = @dtDivDate, 
						divFreq = @iDivFreq
				where stockID = @iContractID
				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update DivAmnt and DivDate!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
			end
	Commit Transaction
    set @error = 0
finish:
	if @error<>0 
		Return(@error)
	else
		Return @iContractID








GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE proc dbo.usp_Stock_Save
        @iStockID int,
        @iExchangeID int = Null,
        @iDefaultModelTypeID int = Null,
        @iDefaultSurfaceTypeID int = Null,
        @iBaseIndexID int = Null,
        @vcSymbol varchar(8) = Null,
        @vcContractName varchar(255) = Null,
		@bIsHTB  bit = Null,
        @iDivFreq int = Null,
		@fDivAmt float = Null,
		@dtDivDate smalldatetime = Null,
		@tiIsDivCustom tinyint = 0,
		@fDivAmtCustom float = Null,
		@iDivFreqCustom int = Null,
		@dtDivDateCustom smalldatetime = Null,
		@tiCalcOptionType tinyint = null,
		@tiCalcModelType tinyint = null,
		@tiExpCalendarID tinyint = null
  as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @bIsHTB is null 
		set @bIsHTB=0
   
	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID=2 and actionID < 3 and (@iStockID <> contractID or @iStockID is null))
		begin
			Raiserror ('Stock with this ticker already exists!', 16, 1)
			Return (-1)
		end   
   if (@iStockID is Null)  
     begin
         begin tran
            insert into Contract (contractTypeID, Symbol, contractName, expCalendarID)
                  values (2, @vcSymbol, @vcContractName, @tiExpCalendarID)
			if (@@error <> 0) begin rollback tran return(-1) end        

            set @iStockID = @@identity         
 
            insert into Stock (
				stockID, 
				defaultModelTypeID, 
				defaultSurfaceTypeID,
                divFreq, 
				baseIndexID, 
				isHTB,
				primaryExchangeID,
				divAmt,
				divDate,
				isDivCustom,
				divAmtCustom,
				divFreqCustom,
				divDateCustom,
				calcOptionType,
				calcModelType)
            values (
				@iStockID, 
				@idefaultModelTypeID, 
				@idefaultSurfaceTypeID,
                @iDivFreq, 
				@ibaseIndexID,
				@bIsHTB,
				@iExchangeID,
				@fDivAmt,
				@dtDivDate,
				@tiIsDivCustom,
				@fDivAmtCustom,
				@iDivFreqCustom,
				@dtDivDateCustom,
				@tiCalcOptionType,
				@tiCalcModelType)
			if (@@error <> 0) begin rollback tran return(-1) end           
			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iStockID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@iStockID,null)
					if (@@error <> 0) begin rollback tran return(-1) end           
				end 
         commit tran
     end
   else
     begin

        declare @df int

        select @df = DivFreq from Stock where stockID = @iStockID        
        begin tran
           update Contract
                set symbol = isNull(@vcSymbol,symbol),
                    contractName = isNull(@vcContractName,contractName),
					expCalendarID = isNull(@tiExpCalendarID,expCalendarID)
	            where contractID = @iStockID
			if (@@error <> 0) begin rollback tran return(-1) end         

			update Stock
	               set  defaultModelTypeID = isNull(@idefaultModelTypeID,defaultModelTypeID), 
	                    defaultSurfaceTypeID = isNull(@idefaultSurfaceTypeID,defaultSurfaceTypeID),
	                    divFreq = isNull(@iDivFreq,divFreq), 
	                    baseIndexID = isNull(@ibaseIndexID,baseIndexID),
						isHTB = isNull(@bIsHTB, isHTB),
						primaryExchangeID = isNull(@iExchangeID,primaryExchangeID),
						DivAmt = isNull(@fDivAmt, DivAmt),
						DivDate = isNull(@dtDivDate, DivDate),
						isDivCustom = isNull(@tiIsDivCustom, isDivCustom),
						divAmtCustom = isNull(@fDivAmtCustom, divAmtCustom),
						divFreqCustom = isNull(@iDivFreqCustom, divFreqCustom),
						divDateCustom = isNull(@dtDivDateCustom, divDateCustom),
						calcOptionType = isNull(@tiCalcOptionType, calcOptionType),
						calcModelType = isNull(@tiCalcModelType, calcModelType)
		          where stockID = @iStockID
    	       if (@@error <> 0) begin rollback tran return(-1) end         

			if @iDivFreq = 0 
				begin
					update Stock set divAmt = null, divDate = null where stockID = @iStockID
				end

			if @iDivFreqCustom = 0 
				begin
					update Stock set divAmtCustom = null, divDateCustom = null where stockID = @iStockID
				end

         commit tran
     end        

 if (@@error <> 0)
    return (-1)
  else  
    return @iStockID






GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE        proc dbo.usp_Trade_Get
	@iTradeID int = null,
	@iContractID int = null,
	@tiExtendedInfo tinyint = 0,
	@iUnderlyingID int = null,
	@iTraderID int = null,
	@iBookID int = null,
	@iStrategyID int = null,
	@iContractGroupID int = null,
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
		isPosition as tiIsPosition,
		lotSize as iLotSize,
		isSimulated as tiIsSimulated
		from Trade 
			where 
			(@iTradeID is null or @iTradeID is not null and tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and contractID = @iContractID)
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tradeDate >= @dtMinTradeDate)
		group by tradeID,contractID,isClose,tradeDate,quantity,price,isBuy,bookID,strategyID,traderID,brokerID,clearingBrokerID,brokerCommissionID,clearingBrokerCommissionID,tradedIV,spotReference,isPosition,lotSize,isSimulated
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
			join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
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
			EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,
			cp.lotSize,cp.priceClose,ucp.lotSize,ucp.priceClose
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
		from Trade tr 
			inner join Contract c on tr.contractID = c.contractID
			inner join [Option] o on c.contractID = o.optionID
			inner join Contract uc on O.underlyingContractID = UC.contractID
			join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			inner join ContractInGroup cig on o.underlyingContractID = cig.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
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
			EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.lotSize,tr.isSimulated,
			cp.lotSize,cp.priceClose,ucp.lotSize, ucp.priceClose
		having min(tr.actionID) = 1
		order by TradeDate

     Return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    proc dbo.usp_Underlying_Get
	@iContractID int = null,
	@iTraderID int = null
AS
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE       proc dbo.usp_allStockEOD_Get
	@iFormat int = 1	-- 1 - list only
				   		-- 2 - list with prices by exchange
  as
	---------------
	set nocount on
	---------------

--=================================================================
-- select stocks for which trades was
--=================================================================
	if object_ID('tempdb..#SelStocks') is null
		Create table #SelStocks(stockID int not null)
	
	insert into #SelStocks
		select stockID 
			from Stock
		where 
		stockID in (select distinct O.underlyingContractID from OptionView O join TradeView TV on TV.contractID=O.optionID)
	union 
		select distinct stockID 
			from Stock 
		where stockID in (select contractID from TradeView)

--=================================================================
--base section 
--=================================================================
	if @iFormat = 1
		select	iContractID, 
				iContractTypeID, 
				vcSymbol,
				tiCalcOptionType,
				tiCalcModelType,
				tiExpCalendarID
			from vALLStockEOD A
		where iContractID in (select stockID from #SelStocks)
	else
		-- use NULL exchange only
		select iContractID, 
				iContractTypeID, 
				vcSymbol, 
				null as vcExchangeCode,  
				case when isDivCustom = 1 then A.divFreqCustom else A.iDivFreq end iDivFreq,
				case when isDivCustom = 1 then A.divAmtCustom else A.fDivAmt end fDivAmt,
				case when isDivCustom = 1 then A.divDateCustom else A.dtDivDate end dtDivDate,
				CP.priceClose as fPriceClose,
				A.bIsHTB as bIsHTB,
				tiCalcOptionType,
				tiCalcModelType,
				tiExpCalendarID  
			from vALLStockEOD A, ContractPrice CP (nolock)
			where A.iContractID *= CP.contractID and CP.exchangeID is null
			and A.iContractID in (select stockID from #SelStocks)

if object_ID('tempdb..#SelStocks') is not null
	Drop table #SelStocks

if (@@error <> 0)
	Return (-1)
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


CREATE       proc dbo.usp_allStockOptionEOD_Get
	@iStockID int = null
as
	if @iStockID is null 
		-- all options
	 	 select	C.contractID as iContractID,
				C.Symbol as vcSymbol,  
				null as vcExchangeCode, 
				null as iExchangeID, 
				O.isCall as bIsCall, 
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry
         	from ContractView C (nolock)
					join OptionView O (nolock) on O.optionID = C.contractID
					join Contract C1 on O.underlyingContractID = C1.contractID
					join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
	   		where EC.expiryID > (select top 1 expiryID from ExpiryInCalendar where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc)
	else
	-- get list of NULL exchanges

	 	select C.contractID as iContractID,
				C.Symbol as vcSymbol,  
				null as vcExchangeCode, 
				null as iExchangeID, 
				O.isCall as bIsCall, 
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry
            from Contract C (nolock)
					join OptionView O (nolock) on O.optionID = C.contractID
					join Contract C1 on O.underlyingContractID = C1.contractID
					join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
	   		where O.underlyingContractID = @iStockID
				and EC.expiryID > (select top 1 expiryID from ExpiryInCalendar where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc)
		
Return(@@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryCalendar_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExpiryCalendar_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryCalendar_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExpiryCalendar_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryCalendar_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExpiryCalendar_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE PROCEDURE dbo.usp_ExpiryCalendar_Del 
	@iExpCalendarID int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from ExpiryCalendar table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iExpCalendarID <= 3 
	begin
		Raiserror('You can''t delete system defined calendar!',16,1)
		Return(-1)
	end

	begin tran
		delete from ExpiryInCalendar where expCalendarID  = @iExpCalendarID 
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
	
		delete from ExpiryCalendar where expCalendarID  = @iExpCalendarID 
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
	commit tran


	set @error = @@error

finish:
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


CREATE PROCEDURE dbo.usp_ExpiryCalendar_Get 
	@iExpCalendarID int = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from ExpiryCalendar table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select 
		expCalendarID as iExpCalendarID,
		expCalendarName  as vcExpCalendarName,
		expCalendarDesc  as vcExpCalendarDesc
		from ExpiryCalendar 
		where (expCalendarID = @iExpCalendarID or @iExpCalendarID is null)
		
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


CREATE  PROCEDURE dbo.usp_ExpiryCalendar_Save 
	@iExpCalendarID int = null output, 
	@vcExpCalendarName varchar(50) = null,
	@vcExpCalendarDesc varchar(250) = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is save information into ExpiryCalendar table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction
		if not exists(select * From ExpiryCalendar where expCalendarID = @iExpCalendarID) 
			begin
				insert into ExpiryCalendar 
					(expCalendarName, expCalendarDesc) 
					values (@vcExpCalendarName,@vcExpCalendarDesc)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iExpCalendarID = @@identity
			end
		else
			begin
				update ExpiryCalendar set 
					expCalendarName = isnull(@vcExpCalendarName, expCalendarName),
					expCalendarDesc = isnull(@vcExpCalendarDesc, expCalendarDesc)
				where expCalendarID = @iExpCalendarID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction
    set @error = 0
finish:
	if @error=0 
	    set @error = @iExpCalendarID

Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
set nocount on 
--============================================================================================================
--change DB version
--============================================================================================================
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.15')
	begin
		update DataInfo set keyValue = '3.15' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.15'
	end
