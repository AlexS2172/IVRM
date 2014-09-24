/*
	script for next release of ETS 
*/
set nocount on
GO
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Index' and COLUMN_NAME='defaultModelTypeID')
	begin
		Alter Table [Index]
			Drop column defaultModelTypeID
	end
GO
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Stock' and COLUMN_NAME='defaultModelTypeID')
	begin
		Alter Table Stock
			Drop column defaultModelTypeID
	end
GO
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Stock' and COLUMN_NAME='defaultSurfaceTypeID')
	begin
		Alter Table Stock
			Drop column defaultSurfaceTypeID
	end
GO
--foreign key !
if object_id('FK_Stock_Indexes') is not null
	Alter table Stock Drop constraint FK_Stock_Indexes
GO
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Stock' and COLUMN_NAME='baseIndexID')
	begin
		Alter Table Stock
			Drop column baseIndexID
	end
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IndexView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[IndexView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[StockView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[StockView]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE VIEW dbo.IndexView
AS
	select  indexID, 
			yield, 
			isHTB, 
			actionID,
			actionDate, 
			CalcOptionType, 
			CalcModelType, 
			isBasket, 
			isActive
		from [Index]
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


CREATE VIEW dbo.StockView
AS
	select	stockID, 
			divFreq, 
			isHTB, 
			primaryExchangeID, 
			divAmt, 
			divDate, 
			divAmtCustom, 
            divDateCustom, 
			divFreqCustom, 
			isDivCustom, 
			actionID, 
			actionDate, 
			calcOptionType, 
			calcModelType, 
			isActive
		from dbo.Stock
		where actionID < 3


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VarStockIndex_Set]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VarStockIndex_Set]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractStockAndIndex_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractStockAndIndex_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexWithPos_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexWithPos_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexWithPrice_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ModelParameter_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ModelParameter_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockWithPos_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockWithPos_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockWithPrice_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingByTrader_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO






CREATE proc usp_ContractStockAndIndex_Get
	 @iGroupID int,
	 @bIsAll bit,
	 @iContractTypeID int	 
	   
  as
		 
   if (@iGroupID is Null)
	  if (@bIsAll is not Null)	and   (@bIsAll > 0)
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				S.divFreq as iDivFreq, 
				Null as fYield
					from ContractView c 
							join Stock S on S.stockID = C.contractID
							left join ContractPrice CP on C.contractID=CP.contractID
					where ((@iContractTypeID = 2) or (@iContractTypeID is Null))
							and C.contractTypeID = 2
			union
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				Null as iDivFreq, 
				i.yield as fYield
					from ContractView c 
							join IndexView I on I.indexID = C.contractID
							left join ContractPrice CP on C.contractID=CP.contractID
					where ((@iContractTypeID = 1) or (@iContractTypeID is Null))
							and c.contractTypeID = 1
					order by c.symbol	
	   else
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				S.beta as fBeta,
				S.divFreq as iDivFreq, 
				Null as fYield
					from ContractView c  
							join Stock S on S.stockID = C.contractID
							left join ContractPrice CP on C.contractID=CP.contractID
					where ((@iContractTypeID = 2) or (@iContractTypeID is Null))
						and c.contractTypeID = 2
						and not exists (select * from ContractsInGroup where contractID = c.contractID)
		  union
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				Null as fBeta,
				Null as iDivFreq, 
				i.yield as fYield
					from ContractView c  
							join IndexView I on I.indexID = C.contractID
							left join ContractPrice CP on C.contractID=CP.contractID
					where ((@iContractTypeID = 1) or (@iContractTypeID is Null))
							and c.contractTypeID = 1
							and not exists (select * from ContractsInGroup where contractID = c.contractID) 
					order by c.symbol	
	else
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				S.beta as fBeta,
				S.divFreq as iDivFreq, 
				Null as fYield
			 from ContractsInGroup CIG
					join ContractView c  on C.contractID = CIG.contractID
					join Stock S on S.stockID = C.contractID
					left join ContractPrice CP on C.contractID=CP.contractID
			 where CIG.groupID = @iGroupID
					   and c.contractTypeID = 2
	   union
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				CP.exchangeID as iExchangeID,
				Null as fBeta,
				Null as iDivFreq, 
				i.yield as fYield
				from ContractsInGroup CIG
						join ContractView c  on C.contractID = CIG.contractID
						join IndexView I on I.indexID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
				where CIG.groupID = @iGroupID
						and c.contractTypeID = 1
				order by c.symbol  
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



CREATE  proc dbo.usp_IndexWithPos_Get
	@iIndexID int = null,
	@iTraderID int = null,
	@tiPositionMode tinyint = 0 -- 0 - filter by TraderContract, 1 - don't filter by TraderContract
as
	------------------------------------------------
	set nocount on
	------------------------------------------------

	if @iTraderID is null
		select C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName,
			I.yield as fYield,
			I.isHTB as bIsHTB,
			I.actionDate as dtActionDate,
			I.actionID as tiActionID,
			sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
			CP.lotSize as iLotSize,
			I.CalcOptionType as tiCalcOptionType,
			I.CalcModelType as tiCalcModelType,
			C.expCalendarID as iExpCalendarID,
			I.isBasket as tiIsBasket,
			I.isActive as tiIsActive
		from ContractView c 	
			inner join IndexView I on I.indexID = C.contractID
			join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
			left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
		where C.contractTypeID = 1
			and (C.contractID = @iIndexID or @iIndexID is Null)
		group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
			I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID, I.isBasket, I.isActive
		order by C.symbol
	else
	begin
		if isnull(@tiPositionMode, 0) = 0
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName,
				I.yield as fYield,
				I.isHTB as bIsHTB,
				I.actionDate as dtActionDate,
				I.actionID as tiActionID,
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				CP.lotSize as iLotSize,
				I.CalcOptionType as tiCalcOptionType,
				I.CalcModelType as tiCalcModelType,
				C.expCalendarID as iExpCalendarID,
				I.isBasket as tiIsBasket,
				I.isActive as tiIsActive
			from ContractView c 	
				inner join IndexView I on I.indexID = C.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where C.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (C.contractID = @iIndexID or @iIndexID is Null)
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
				I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID, I.isBasket, I.isActive
			order by C.symbol
		else
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName,
				I.yield as fYield,
				I.isHTB as bIsHTB,
				I.actionDate as dtActionDate,
				I.actionID as tiActionID,
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				CP.lotSize as iLotSize,
				I.CalcOptionType as tiCalcOptionType,
				I.CalcModelType as tiCalcModelType,
				C.expCalendarID as iExpCalendarID,
				I.isBasket as tiIsBasket,
				I.isActive as tiIsActive
			from ContractView c 	
				inner join IndexView I on I.indexID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where C.contractTypeID = 1
				and (C.contractID = @iIndexID or @iIndexID is Null)
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
				I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID, I.isBasket, I.isActive
			order by C.symbol
	end

Return (@@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO






CREATE	  proc usp_IndexWithPrice_Get
	@iGroupID int = Null,
	@iIndexID int = Null,
	@bIsBestPriceOnly bit = 0
  as
	if (@iGroupID is Null)
		if @bIsBestPriceOnly=0
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				I.yield as fYield,
				CP.priceLast as fPriceLast,
				CP.priceClose as fPriceClose,
				CP.priceOpen as fPriceOpen,
				CP.priceLow as fPriceLow,
				CP.priceHigh as fPriceHigh,
				CP.volume as iVolume,
				E.exchangeID as iExchangeID,
				E.exchangeCode as vcExchangeCode
				from ContractView c 	
						join IndexView I on I.indexID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				where C.contractTypeID=1
					and (C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null)
				order by C.symbol,E.exchangeCode
		else 
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.lotSize as iLotSize,
				I.yield as fYield,
				CP.priceLast as fPriceLast,
				CP.priceClose as fPriceClose,
				CP.priceOpen as fPriceOpen,
				CP.priceLow as fPriceLow,
				CP.priceHigh as fPriceHigh,
				CP.volume as iVolume,
				E.exchangeID as iExchangeID,
				E.exchangeName as vcExchangeCode
				from ContractView c 	
						join IndexView I on I.indexID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				where C.contractTypeID=1
					and (C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null)
					and CP.exchangeID is null
				order by C.symbol,E.exchangeCode
	else
		select C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.lotSize as iLotSize,
			I.yield as fYield,
			CP.priceLast as fPriceLast,
			CP.priceClose as fPriceClose,
			CP.priceOpen as fPriceOpen,
			CP.priceLow as fPriceLow,
			CP.priceHigh as fPriceHigh,
			CP.volume as iVolume,
			E.exchangeID as iExchangeID,
			E.exchangeCode as vcExchangeCode
			from ContractViewInGroup CIG
					join ContractView c  on C.contractID = CIG.contractID
					join IndexView I on I.indexID = C.contractID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
			where CIG.groupID = @iGroupID
					 and C.contractTypeID=1
			order by C.symbol,E.exchangeCode
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





CREATE   proc dbo.usp_Index_Get
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
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive
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
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive
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
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive
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
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive
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
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive
				from ContractView c 	
					inner join [Index] I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, 
						C.contractTypeID, 
						C.symbol, 
						C.contractName, 
						I.yield, 
						I.isHTB, 
						I.actionDate, 
						I.actionID,
						CP.lotSize,
						I.CalcOptionType,
						I.CalcModelType,
						C.expCalendarID, 
						I.isBasket, 
						I.isActive
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive
				from ContractView c 	
					inner join [Index] I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
				where C.contractTypeID=1
					and tc.traderID = @iTraderID
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID,
						C.contractTypeID, 
						C.symbol, 
						C.contractName,
						I.yield, 
						I.isHTB, 
						I.actionDate, 
						I.actionID,
						CP.lotSize,
						I.CalcOptionType,
						I.CalcModelType,
						C.expCalendarID,
						I.isBasket, 
						I.isActive
				order by C.symbol
		end
		else
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join [Index] I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
					I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID,I.isBasket, I.isActive
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					CP.lotSize as iLotSize,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive
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
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
					I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID,I.isBasket, I.isActive
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



CREATE    proc dbo.usp_Index_Save
		@iIndexID int,
		@vcSymbol varchar(8) = Null,
		@vcContractName varchar(255) = Null,
		@fYield float = Null,
		@bIsHTB bit = Null,
		@tiCalcOptionType tinyint = null,
		@tiCalcModelType tinyint = null,
		@iExpCalendarID int = null,
		@tiIsBasket tinyint = null,
		@tiIsActive tinyint = null
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
				  values (1, @vcSymbol, @vcContractName,@iExpCalendarID)
			 select @iIndexID = @@identity			 
				 if (@@error <> 0)
					  begin
						RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
						ROLLBACK TRAN
						RETURN (-1)
					  end
			 insert into [Index] (indexID, yield,IsHTB,calcOptionType,calcModelType,isBasket, isActive)
					 values (@iIndexID, @fYield,isNull(@bIsHTB,0),@tiCalcOptionType,@tiCalcModelType,isnull(@tiIsBasket,0), @tiIsActive)
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
					expCalendarID = isNull(@iExpCalendarID, expCalendarID)
				where contractID = @iIndexID
			if (@@error <> 0)
				 begin
					RAISERROR ('Error. Can''t update Contract!', 16, 1)
					ROLLBACK TRAN
					RETURN (-1)
				 end
		  update [Index]
			   set	yield = isNull(@fYield,yield),
					isHTB = isNull(@bIsHTB,isHTB),
					calcOptionType = isNull(@tiCalcOptionType, calcOptionType),
					calcModelType = isNull(@tiCalcModelType, calcModelType),
					isBasket = isNull(@tiIsBasket,isBasket),
					isActive = isnull(@tiIsActive, isActive)
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO






CREATE Procedure dbo.usp_ModelParameter_Get
		@iContractID int,
		@bDefaultOnly bit,
		@iParameterID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		if (@iParameterID is Null) 
		   if (@iContractID is Null)			
			  select parameterID as iParameterID, contractID as iContractID, modelTypeID as iModelTypeID, param1 as fParam1, param2 as fParam2, param3 as fParam3
				 from ModelParameter
				order by contractID, modelTypeID
			 else
			  if ((@bDefaultOnly is NOT Null) and (@bDefaultOnly = 1))		  
				  select mp.parameterID as iParameterID, mp.contractID as iContractID, mp.modelTypeID as iModelTypeID, mp.param1 as fParam1, mp.param2 as fParam2, mp.param3 as fParam3
					from StockView s, ModelParameter mp
				   where s.stockID = @iContractID
					 and mp.contractID = s.stockID
					 --and mp.modelTypeID = s.defaultModelTypeID 
				  union 
				  select mp.parameterID as iParameterID, mp.contractID as iContractID, mp.modelTypeID as iModelTypeID, mp.param1 as fParam1, mp.param2 as fParam2, mp.param3 as fParam3
					from IndexView i, ModelParameter mp
				   where i.indexID = @iContractID
					 and mp.contractID = i.indexID
					--and mp.modelTypeID = i.defaultModelTypeID
				else
			  select parameterID as iParameterID, contractID as iContractID, modelTypeID as iModelTypeID, param1 as fParam1, param2 as fParam2, param3 as fParam3
					from ModelParameter
				   where contractID = @iContractID
				  order by modelTypeID
		 else
			  select parameterID as iParameterID, contractID as iContractID, modelTypeID as iModelTypeID, param1 as fParam1, param2 as fParam2, param3 as fParam3
		from ModelParameter
		where parameterID = @iParameterID
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




CREATE  proc dbo.usp_StockWithPos_Get
	@iStockID int = null,
	@iTraderID int = null,
	@tiPositionMode tinyint = 0 -- 0 - filter by TraderContract, 1 - don't filter by TraderContract
as
	------------------------------------------------
	set nocount on
	------------------------------------------------

	if @iTraderID is null
		select	C.contractID as iContractID, 
			C.contractTypeID as iContractTypeID, 
			C.Symbol as vcSymbol, 
			C.ContractName as vcContractName, 
			S.DivFreq as iDivFreq,
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
			C.expCalendarID as iExpCalendarID,
			S.isActive as tiIsActive
	    from ContractView c  
			inner join StockView S on S.stockID = C.contractID
			join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
			left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
	    where c.contractTypeID = 2
			and (c.contractID = @iStockID or @iStockID is Null)
		group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
			S.DivFreq,S.primaryExchangeID, 
			S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
			S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
			S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive
		order by c.Symbol
	else
	begin 
		if isnull(@tiPositionMode, 0) = 0
			select	C.contractID as iContractID, 
				C.contractTypeID as iContractTypeID, 
				C.Symbol as vcSymbol, 
				C.ContractName as vcContractName, 

				S.DivFreq as iDivFreq,
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
				C.expCalendarID as iExpCalendarID,
				S.isActive as tiIsActive
		    from ContractView c  
				inner join StockView S on S.stockID = C.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
		    where c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (c.contractID = @iStockID or @iStockID is Null)
			group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
				S.DivFreq, S.primaryExchangeID, 
				S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
				S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
				S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive
			order by c.Symbol
		else
			select	C.contractID as iContractID, 
				C.contractTypeID as iContractTypeID, 
				C.Symbol as vcSymbol, 
				C.ContractName as vcContractName, 

				S.DivFreq as iDivFreq,
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
				C.expCalendarID as iExpCalendarID,
				S.isActive as tiIsActive
		    from ContractView c  
				inner join StockView S on S.stockID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
		    where c.contractTypeID = 2
				and (c.contractID = @iStockID or @iStockID is Null)
			group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
				S.DivFreq, S.primaryExchangeID, 
				S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
				S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
				S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive
			order by c.Symbol
	end
	
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







CREATE    proc usp_StockWithPrice_Get
	@iGroupID int = Null,
	@iStockID int = Null,
	@bIsBestPriceOnly bit = 0
  as
	if (@iGroupID is Null)
		if @bIsBestPriceOnly=0
			select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
				    CP.LotSize as iLotSize,
					CP.priceBid as fPriceBid, 
					CP.priceAsk as fPriceAsk,
				    CP.priceLast as fPriceLast, 
					CP.priceClose as fPriceClose, 
					CP.priceOpen as fPriceOpen, 
					CP.priceLow as fPriceLow, 
					CP.priceHigh as fPriceHigh, 
					CP.Volume as iVolume,
					E.exchangeID as iExchangeID,
					E.ExchangeCode as vcExchangeCode
				    from ContractView c  
						join StockView S on S.StockID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				    where c.contractTypeID = 2
						and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				   order by c.Symbol,E.ExchangeCode
		else
				select	
					C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					CP.LotSize as iLotSize,
					CP.priceBid as fPriceBid, 
					CP.priceAsk as fPriceAsk,
				    CP.priceLast as fPriceLast, 
					CP.priceClose as fPriceClose, 
					CP.priceOpen as fPriceOpen, 
					CP.priceLow as fPriceLow, 
					CP.priceHigh as fPriceHigh, 
					CP.Volume as iVolume,
					E.exchangeID as iExchangeID,
					E.ExchangeCode as vcExchangeCode
				    from ContractView c  
						join StockView S on S.StockID = C.contractID
						left join ContractPrice CP on C.contractID=CP.contractID
						left join ExchangeView E on CP.exchangeID=E.exchangeID
				    where c.contractTypeID = 2
						and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
						and CP.exchangeID is null
				   order by c.Symbol,E.ExchangeCode
	else
		select	C.contractID as iContractID , 
		C.contractTypeID as iContractTypeID ,
		 C.Symbol as vcSymbol,
		  C.ContractName as vcContractName, 
				S.DivFreq as iDivFreq,
				S.DivDate as dtDivDate,
				S.DivAmt as fDivAmt, 
				S.isHTB as bIsHTB,
			    CP.LotSize as iLotSize,
				CP.priceBid as fPriceBid, 
				CP.priceAsk as fPriceAsk,
			    CP.priceLast as fPriceLast, 
				CP.priceClose as fPriceClose, 
				CP.priceOpen as fPriceOpen, 
				CP.priceLow as fPriceLow, 
				CP.priceHigh as fPriceHigh, 
				CP.Volume as iVolume,
				E.exchangeID as iExchangeID,
				E.ExchangeCode as vcExchangeCode
			    from ContractInGroupView cig
					join ContractView c  on C.contractID = CIG.contractID
					join StockView S on S.StockID = C.contractID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
		        where CIG.groupID = @iGroupID
		          and c.contractTypeID = 2
			   order by c.Symbol,E.ExchangeCode
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




CREATE         proc dbo.usp_Stock_Get
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
					S.DivFreq as iDivFreq,
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
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive
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
					S.DivFreq as iDivFreq,
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
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive
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
					S.DivFreq as iDivFreq,
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
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive
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
					S.DivFreq as iDivFreq,
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
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive
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
					S.DivFreq as iDivFreq,
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
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive
			    from ContractView c  
					inner join Stock S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
			    where c.contractTypeID = 2
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
					and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
				group by C.contractID, 
					C.contractTypeID, 
					C.Symbol, 
					C.ContractName,
					S.DivFreq,
					S.DivDate, 
					S.DivAmt, 
					S.isHTB, 
					S.actionDate,	
					S.actionID,
					S.isDivCustom,
					S.divAmtCustom,
					S.divDateCustom,
					S.divFreqCustom,
					CP.lotSize,
					S.CalcOptionType,
					S.CalcModelType,
					C.expCalendarID, 
					S.isActive,
					S.primaryExchangeID
				order by c.Symbol
			else
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
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
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive
			    from ContractView c  
					inner join Stock S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			    where c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
					and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
					S.DivFreq, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive
				order by c.Symbol
		end
		else
		begin
			if @iTraderID is null
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
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
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive
			    from ContractInGroupView cig
					inner join ContractView c  on C.contractID = CIG.contractID
					inner join Stock S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
		        where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
					and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, 
					S.DivFreq,S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive
				order by c.Symbol
			else
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
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
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive
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
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
					S.DivFreq, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive
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

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE     proc dbo.usp_Stock_Save
        @iStockID int,
        @iExchangeID int = Null,
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
		@iExpCalendarID int = null,
		@tiIsActive tinyint = null
  as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @bIsHTB is null 
		set @bIsHTB=0

	if @iExpCalendarID is null or @iExpCalendarID = 0
		set @iExpCalendarID = 1
   
	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID=2 and actionID < 3 and (@iStockID <> contractID or @iStockID is null))
		begin
			Raiserror ('Stock with this ticker already exists!', 16, 1)
			Return (-1)
		end   
   if (@iStockID is Null)  
     begin
         begin tran
            insert into Contract (contractTypeID, Symbol, contractName, expCalendarID)
                  values (2, @vcSymbol, @vcContractName, @iExpCalendarID)
			if (@@error <> 0) begin rollback tran return(-1) end        

            set @iStockID = @@identity         
 
            insert into Stock (
				stockID, 
                divFreq, 
				isHTB,
				primaryExchangeID,
				divAmt,
				divDate,
				isDivCustom,
				divAmtCustom,
				divFreqCustom,
				divDateCustom,
				calcOptionType,
				calcModelType,
				isActive)
            values (
				@iStockID, 
                @iDivFreq, 
				@bIsHTB,
				@iExchangeID,
				@fDivAmt,
				@dtDivDate,
				@tiIsDivCustom,
				@fDivAmtCustom,
				@iDivFreqCustom,
				@dtDivDateCustom,
				@tiCalcOptionType,
				@tiCalcModelType,
				@tiIsActive)
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
					expCalendarID = isNull(@iExpCalendarID,expCalendarID)
	            where contractID = @iStockID
			if (@@error <> 0) begin rollback tran return(-1) end         

			update Stock
	               set  
	                    divFreq = isNull(@iDivFreq,divFreq), 
						isHTB = isNull(@bIsHTB, isHTB),
						primaryExchangeID = isNull(@iExchangeID,primaryExchangeID),
						DivAmt = isNull(@fDivAmt, DivAmt),
						DivDate = isNull(@dtDivDate, DivDate),
						isDivCustom = isNull(@tiIsDivCustom, isDivCustom),
						divAmtCustom = isNull(@fDivAmtCustom, divAmtCustom),
						divFreqCustom = isNull(@iDivFreqCustom, divFreqCustom),
						divDateCustom = isNull(@dtDivDateCustom, divDateCustom),
						calcOptionType = isNull(@tiCalcOptionType, calcOptionType),
						calcModelType = isNull(@tiCalcModelType, calcModelType),
						isActive = isnull(@tiIsActive, isActive)
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




CREATE  proc dbo.usp_UnderlyingByTrader_Get
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
		    cp.lotSize as iLotSize,

			null as iTraderID
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
		    cp.lotSize as iLotSize,

			null as iTraderID
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
			    cp.lotSize as iLotSize,

				tc.traderID as iTraderID
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
			    cp.lotSize as iLotSize,

				tc.traderID as iTraderID
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
			    cp.lotSize as iLotSize,

				tc.traderID as iTraderID
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
			    cp.lotSize as iLotSize,

				tc.traderID as iTraderID
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
--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.28')
	begin
		update DataInfo set keyValue = '3.28' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.28'
	end



