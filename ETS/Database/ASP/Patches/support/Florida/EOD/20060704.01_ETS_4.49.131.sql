/*
	Contains stored procedures for ETS ASP (Institutional) database
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.130' or (keyValue = '4.49.131')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.131 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.131' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
------------------------------------------------------------------------------------------------------------------------------------

alter   PROCEDURE usp_EodBasketIndexComp_Get 

 @iBasketIndexID int = null

AS

/*

 Created by: Michael Malyshkin

*/

 -------------------------------------------------

 set nocount on

 -------------------------------------------------

 declare @error int  set @error = -1

 

 if @iBasketIndexID is null

 begin

  select

   i.indexID as iBasketIndexID,

   idef.stockID as iUnderlyingID,

   idef.weight as fWeight

  from

   IndexDefinitionView idef

   inner join IndexView i on idef.indexID = i.indexID and isnull(i.isBasket, 0) <> 0

  where

   isnull(idef.weight, 0) <> 0

 end

 else

 begin

  select

   i.indexID as iBasketIndexID,

   idef.stockID as iUnderlyingID,

   idef.weight as fWeight

  from

   IndexDefinitionView idef

   inner join IndexView i on idef.indexID = i.indexID and isnull(i.isBasket, 0) <> 0

  where

   idef.indexID = @iBasketIndexID

   and isnull(idef.weight, 0) <> 0

 end

  

 set @error = @@error

Return(@error)

go

 

 

---------------------------------------------------------------------------------------------------------------------

alter    Procedure usp_EodExpirationDate_Get

as

 -------------------------------------------------

 set nocount on

 -------------------------------------------------

 declare @error int  set @error = -1

 

 select distinct expiryDate as dtExpiryDate

 from ExpiryInCalendarView

 where expiryDate > getdate()

 order by expiryDate

 

 set @error = @@error

Return(@error)

go

 

 

----------------------------------------------------------------------------------------------------------------------

 

alter PROC usp_EodFutureOption_Del

AS

 set nocount on

 

 declare @error int  set @error = 0

 

 if object_ID('tempdb..#FutureContractsForDelete') is null

 begin

  set @error = -1

  goto finish

 end

 

 begin tran

  

 -- delete future option

 delete FutureOption

 where futureOptionID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -1 if @@trancount = 1 rollback tran goto finish end

 

 -- delete common

 

 delete ContractPrice

 where contractID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -2 if @@trancount = 1 rollback tran goto finish end

 

 

 update Contract set actionID = 3

 where contractID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran goto finish end

 

 commit tran

 

finish:

 return @error

go

 

 

-----------------------------------------------------------------------------------------------------------------------

alter PROC usp_EodFuture_Del

AS

 set nocount on

 

 declare @error int  set @error = 0

 

 if object_ID('tempdb..#FutureContractsForDelete') is null

 begin

  set @error = -1

  goto finish

 end

 

 begin tran

  

 -- delete future option

 delete FutureOption

 where futureID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -2 if @@trancount = 1 rollback tran goto finish end

 

 -- delete future

 delete Future where

 futureID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran goto finish end

 

 

 -- delete common future and future options

 

 delete ContractPrice

 where contractID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -4 if @@trancount = 1 rollback tran goto finish end

 

 update ContractExchangeLink set actionID = 3

 where contractID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

 

 update Contract set actionID = 3

 where contractID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end

 

 commit tran

 

finish:

 return @error

go

 
--------------------------------------------------------------------------------------------------------------
 

alter PROCEDURE usp_EodFuturesOptionWithPositions_Get

  as

 ---------------

 set nocount on

 ---------------

 

 declare @error int  set @error = 0

 

--=================================================================

-- select future option with positions

--=================================================================

 select distinct 

  FO.futureID as iFutureID,

  FC.symbol as vcFutureSymbol,

  FOC.symbol as vcOptionSymbol,

  FO.futureOptionID as iID,

  FO.isCall as tiIsCall,

  FO.strike as fStrike,

  EC.expiryDate as dtExpiry

 from

  TradeView tv  

  inner join FutureOption FO on FO.futureOptionID = tv.contractID

  inner join ContractView FOC on FOC.contractID = tv.contractID

  inner join ContractView FC on FO.futureID = FC.contractID

  inner join ExpiryInCalendarView EC on FO.expiryID = EC.expiryID and EC.expCalendarID = FC.expCalendarID

 where

  ec.expiryDate >= getdate()

 

set @error = @@error

Return(@error)

go

 

 

-----------------------------------------------------------------------------------------------------------

 

alter PROCEDURE usp_EodFuturesWithPositions_Get

  as

 ---------------

 set nocount on

 ---------------

 

 declare @error int  set @error = 0

 

--=================================================================

-- select futures with positions

--=================================================================

 select 

  FR.underlyingID as iIndexID,

  F.futureID as iFutureID,

  FC.symbol as vcSymbol,

  F.isActive as tiIsActive,

  F.maturityID as iMaturityID,

  sum(T.quantity * T.price * (case when T.isBuy <> 0 then 1 else -1 end)) as fPosition,

  cp.priceClose as fPriceClose

 from TradeView T  

  inner join Future F on T.contractID=F.futureID

  inner join ContractView FC on FC.contractID=T.contractID

  inner join FutureRoot FR on FR.futureRootID=F.futureRootID

  inner join ContractPrice cp on cp.contractID = FC.contractID and cp.exchangeID is null

 group by FR.underlyingID, F.futureID, FC.symbol, F.isActive, F.maturityID, cp.priceClose

 order by FR.underlyingID 

 

set @error = @@error

Return(@error)

go

------------------------------

alter PROCEDURE usp_EodFuturesWithPositions_Get

  as

 ---------------

 set nocount on

 ---------------

 

 declare @error int  set @error = 0

 

--=================================================================

-- select futures with positions

--=================================================================

 select 

  FR.underlyingID as iIndexID,

  F.futureID as iFutureID,

  FC.symbol as vcSymbol,

  F.isActive as tiIsActive,

  F.maturityID as iMaturityID,

  sum(T.quantity * T.price * (case when T.isBuy <> 0 then 1 else -1 end)) as fPosition,

  cp.priceClose as fPriceClose

 from TradeView T  

  inner join Future F on T.contractID=F.futureID

  inner join ContractView FC on FC.contractID=T.contractID

  inner join FutureRoot FR on FR.futureRootID=F.futureRootID

  inner join ContractPrice cp on cp.contractID = FC.contractID and cp.exchangeID is null

 group by FR.underlyingID, F.futureID, FC.symbol, F.isActive, F.maturityID, cp.priceClose

 order by FR.underlyingID 

 

set @error = @@error

Return(@error)

go

 

 

alter PROCEDURE usp_EodMsUnderlyingCache_Check

 @iContractID int

as

/*

 Created by Michael Malyshkin

 Checked if no options of the underlying exist in MsOptionCache then delete underlying record

*/

 -------------------------------------------------

 set nocount on

 -------------------------------------------------

 declare @error int  set @error = 0

 

 if not exists(select * from MsOptionCache O where O.underlyingID = @iContractID)

  begin

   delete from MsUnderlyingCache where underlyingID=@iContractID

   

   if @@error <> 0 set @error = @@error

  end

  

   set @error = 0

finish:

Return(@error)

go

 

 

-------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodCommodity_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure [dbo].[usp_EodCommodity_Get]

GO

 

 

create PROC usp_EodCommodity_Get

 @iInterestID int = null

AS

 -- retrieve future

 

 set nocount on

 

 if @iInterestID is not null

  SELECT  c.GreeksCalculationType as GreeksCalcType,

   c.IsActive as tiIsActive, 

   CV.contractTypeID as iContractTypeID,

   CV.symbol as vcSymbol, 

                 CV.contractName as vcContractName, 

   c.CommodityID as iContractID 

  FROM         dbo.Commodity C INNER JOIN

                       dbo.ContractView CV ON C.CommodityID = CV.contractID

  WHERE  CV.ContractID =  @iInterestID And CV.ContractTypeID = 7

  Order by CV.Symbol

 

 else

 

  SELECT  c.GreeksCalculationType as GreeksCalcType,

   c.IsActive as tiIsActive, 

   CV.contractTypeID as iContractTypeID,

   CV.symbol as vcSymbol, 

                 CV.contractName as vcContractName, 

   c.CommodityID as iContractID 

  FROM         dbo.Commodity C INNER JOIN

                       dbo.ContractView CV ON C.CommodityID = CV.contractID

  WHERE  CV.ContractTypeID = 7

  Order by CV.Symbol

go

 

 

 

--------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmTrades_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure [dbo].[usp_MmTrades_Get]

GO

 

 

create    PROC usp_MmTrades_Get

as

 -------------------------------------------------

 set nocount on

 -------------------------------------------------

 select

   tr.tradeID as iTradeID,

   tr.tradeDate as dtTradeDate,

   tr.quantity as iQuantity,

   tr.price as fPrice,

   tr.isBuy as tiIsBuy,

   tr.traderID as iTraderID,

   te.acronym as traderAcronym,

   te.traderName as traderName,

   tr.strategyID as iStrategyID, 

   st.StrategyName as strategyName,

   tr.brokerID as iBrokerID,

   br.BrokerName as brokerName,

   tr.brokerCommission as fBrokerCommission,

   tr.clearingBrokerID as iClearingBrokerID,

   brc.BrokerName as clearingBrokerName,

   tr.clearingBrokerCommission as fClearingBrokerCommission,

   tr.tradedIV as fTradedIV,

   tr.spotReference as fSpotReference,

   tr.isPosition as tiIsPosition,

   tr.status as tiStatus,

   tr.execID as vcExecID,

   tr.mark as iMark,

 

   c.contractID as iContractID,

   c.contractTypeID as iContractTypeID,

   c.Symbol as vcSymbol,

   c.contractName as vcContractName,

   c.exportSymbol as vcImportID,

   cp.priceClose as fPriceClose,

   cp.priceTheo as fPriceTheoClose,

 

   Case when c.contractTypeID in (4,5) then fr.futureRootID else uc.contractID end as iUnderlyingContractID,

                 Case when c.contractTypeID in (4,5) then fr.futureRootSymbol else uc.Symbol end as underlyingContractSymbol,

                 ort.Symbol as optionRootSymbol,

 

   case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,

   case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,

   case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,

   

   ort.optionRootID as iOptionRootID,

 

   fr.futureRootID as iFutureRootID,

   f.futureID as iFutureID,

                 fr.futureRootSymbol as futureRoot,

                 fu.Symbol as Future,

   tr.actionID as ActionID,

   isnull(o.strike, fo.strike)            as OptionStrike,

   isnull(ec.expiryDate, fec.expiryDate)  as OptionExpityDate 

   

  from Trade tr 

   inner join ( 

    select tradeID, max(seqNum) as seqNum from Trade group by tradeID

    ) tr2 on tr.seqNum = tr2.seqNum

 

   -- contract and underlying contract

   inner join ContractView  c  on tr.contractID   = c.contractID

   inner join ContractView  uc on tr.underlyingID = uc.contractID

   left  join ContractPrice cp on tr.contractID   = cp.contractID and cp.exchangeID is null

   left  join ContractView  fu on fu.ContractID   = tr.FutureID

   inner join Trader        te on te.TraderID    = tr.traderID

   left  join Strategy    st  on st.StrategyID = tr.StrategyID

   left  join Broker       br  on br.brokerID = tr.brokerID 

   left  join Broker        brc on br.brokerID = tr.ClearingBrokerID

 

   -- regular option

   left join [Option] o on tr.contractID = o.optionID

   left join OptionRoot ort on tr.optionRootID = ort.optionRootID

   left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID

   -- futures part

   left join ContractView fc on tr.futureID = fc.contractID

   left join FutureRoot fr on tr.futureRootID = fr.futureRootID

   left join Future f on tr.futureID = f.futureID

   left join FutureOption fo on tr.contractID = fo.futureOptionID

   left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = c.expCalendarID

 

  order by TradeDate

go

 

--------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FutureIC_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure [dbo].[usp_FutureIC_Get]

GO

 

create  PROC usp_FutureIC_Get

  @iICID int = null

 AS

  -- retrieve future

  set nocount on

  if @iICID is not null

  SELECT     dbo.Future.futureID, dbo.Commodity.CommodityID

  FROM         dbo.FutureRoot INNER JOIN

                        dbo.Future ON dbo.FutureRoot.futureRootID = dbo.Future.futureRootID INNER JOIN

                        dbo.Contract ON dbo.FutureRoot.underlyingID = dbo.Contract.contractID INNER JOIN

                        dbo.Commodity ON dbo.Contract.contractID = dbo.Commodity.CommodityID

  WHERE     (dbo.Future.futureID = @iICID)

go

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FutureUnderlyingType_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure [dbo].[usp_FutureUnderlyingType_Del]

GO

 

 

create  PROCEDURE usp_FutureUnderlyingType_Del

 @iCptID int,

 @tiIsRaiseError tinyint = 1 

AS

/*

 Created by Pavel Ananyev

*/

 -------------------------------------------------

 set nocount on

 -------------------------------------------------

 declare @error int  set @error = 0

 

 if exists(select 1 from FutureUnderlyingType where iFutureUnderlyingTypeID = @iCptID)

  begin

   if @tiIsRaiseError = 1

    begin

     Raiserror('Can''t remove future underlying type, because it''s used in root defenitions!',16,1)

     Return(0)

    end

   else

    begin

     Return(-1)

    end

  end

 

 Begin Transaction TntCounterParty

 

 delete FutureUnderlyingType where iFutureUnderlyingTypeID = @iCptID

 if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end

 

 Commit Transaction TntCounterParty

 

finish:

 return @error

go

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FutureUnderlyingType_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure [dbo].[usp_FutureUnderlyingType_Get]

GO

 

 

create  PROCEDURE usp_FutureUnderlyingType_Get

 @iFutureUnderlyingTypeID int = null

AS

/*

 Created by Pavel Ananyev

*/

 -------------------------------------------------

 set nocount on

 -------------------------------------------------

 if @iFutureUnderlyingTypeID is null

  select

   cpt.iFutureUnderlyingTypeID as iFutureUnderlyingTypeID,

   cpt.vcName as vcName,

   cpt.vcDesc as vcDesc

  from

   FutureUnderlyingType cpt

 else

  select

   cpt.iFutureUnderlyingTypeID as iFutureUnderlyingTypeID,

   cpt.vcName as vcName,

   cpt.vcDesc as vcDesc

  from

   FutureUnderlyingType cpt

  where

   cpt.iFutureUnderlyingTypeID = @iFutureUnderlyingTypeID

 

return @@error

go

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FutureUnderlyingType_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure [dbo].[usp_FutureUnderlyingType_Save]

GO

 

 

create  PROCEDURE usp_FutureUnderlyingType_Save

 @iCptID int = null,

 @vcName varchar(255),

 @vcDesc varchar(255) = null,

 @tiRaiseError tinyint = null

AS

/*

 Created by Pavel Ananyev

*/

 -------------------------------------------------

 set nocount on

 -------------------------------------------------

 declare @error int  set @error = 0

 

 if @iCptID is null and isnull(@vcName, '') = ''

 begin

  raiserror ('Name for new future underlying type can not be empty!', 16, 1)

  set @error = -1

  goto finish

 end

 

 if @iCptID is null

 begin

  select

   @iCptID = cpt.iFutureUnderlyingTypeID

  from

   FutureUnderlyingType cpt

  where

   cpt.vcName = @vcName

 

  if @iCptID is not null and isnull(@tiRaiseError, 0) <> 0 

  begin

   raiserror ('Future underlying type with the same name is already exists!', 16, 1)

   set @error = -2

   goto finish

  end

 end

 

 Begin Transaction TntCounterParty

 

 if @iCptID is null

 begin

  insert into FutureUnderlyingType

   (vcName, vcDesc)

  values

   (@vcName, @vcDesc)

 

  if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end

  set @iCptID = @@identity

 end

 else

 begin

  update FutureUnderlyingType set 

   vcName = isnull(@vcName, vcName),

   vcDesc = isnull(@vcDesc, vcDesc)

  where

   iFutureUnderlyingTypeID = @iCptID

 

  if @@error <> 0 begin set @error = -4 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end

 end

 

 Commit Transaction TntCounterParty

 

finish:

 if @error = 0

  return @iCptID

 else

  return @error

go

 

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GetClosePriceContr]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure [dbo].[usp_GetClosePriceContr]

GO

 

 

create procedure usp_GetClosePriceContr

as

set nocount on

select fr.futureRootSymbol, e.ExpiryPeriod, dbo.fn_NameContr(e.ExpiryPeriod,0) as ContrName,  null as  isCall, null as strike, cp.PriceClose, 'fut' as typeContr 

from ContractPrice cp

inner join Future f on f.futureID = cp.ContractID

inner join FutureRoot fr on fr.futureRootID = f.futureRootID

inner join Expiry e on e.expiryID = f.MaturityID

where cast('1'+e.ExpiryPeriod  as datetime) >=  Cast('1/' + str(month(getdate())) + '/' + str(Year(getdate())) as datetime)

 

 

union all

 

 

select Case when isnull(fu.iFutureUnderlyingTypeID,0) > 0 then dbo.fn_OptS2FutS(c.Symbol) else  fr.futureRootSymbol end as futureRootSymbol,  

e.ExpiryPeriod, dbo.fn_NameContr(e.ExpiryPeriod,0) as ContrName, fo.isCall, fo.strike, cp.PriceClose, 'opt' as typeContr

from ContractPrice cp

inner join Contract C on C.ContractID = cp.ContractID  

inner join FutureOption fo on fo.futureOptionID = cp.ContractID

inner join Future f on f.futureID = fo.futureID

inner join FutureRoot fr on fr.futureRootID = f.futureRootID

inner join ContractView fc on f.FutureID = fc.contractID

inner join Expiry e on e.expiryID = fo.expiryID

Left join FutureUnderlying fu on fu.iFutureUnderlyingID = fr.UnderlyingID

where cast('1'+e.ExpiryPeriod  as datetime) >=  Cast('1/' + str(month(getdate())) + '/' + str(Year(getdate())) as datetime)

go

 

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GetClosePriceVolContr]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure usp_GetClosePriceVolContr

GO

 

 

create Procedure usp_GetClosePriceVolContr

 

as

 

delete TmpClosePriceVolETS

 

 

insert into TmpClosePriceVolETS

select fr.futureRootSymbol as Symbol , fr.futureRootSymbol, e.ExpiryPeriod, dbo.fn_NameContr(e.ExpiryPeriod,0) as ContrName,  null as  isCall, null as strike, cp.PriceClose, null as Volatility, 'fut' as typeContr 

from ContractPrice cp

inner join Future f on f.futureID = cp.ContractID

inner join FutureRoot fr on fr.futureRootID = f.futureRootID

inner join Expiry e on e.expiryID = f.MaturityID

where cast('1'+e.ExpiryPeriod  as datetime) >=  Cast('1/' + str(month(getdate())) + '/' + str(Year(getdate())) as datetime)

 

union all

 

select distinct co.Symbol, Case when isnull(fu.iFutureUnderlyingTypeID,0) > 0 then dbo.fn_OptS2FutS(co.Symbol) else  fr.futureRootSymbol end as futureRootSymbol

, e.ExpiryPeriod, dbo.fn_NameContr(e.ExpiryPeriod,0) as ContrName, fo.isCall, fo.strike, cp.PriceClose, VC.Volatility, 'opt' as typeContr 

from ContractPrice cp

inner join Contract co on co.ContractID = cp.ContractID

inner join FutureOption fo on fo.futureOptionID = cp.ContractID

inner join Future f on f.futureID = fo.futureID

inner join FutureRoot fr on fr.futureRootID = f.futureRootID

inner join Expiry e on e.expiryID = fo.expiryID

Left join FutureUnderlying fu on fu.iFutureUnderlyingID = fr.UnderlyingID

Left join Contract c on c.ContractID = fu.iFutureUnderlyingID

Left join VolaSurfaceData vsd on vsd.ContractID = c.ContractID

Left join CustomStrikeSkewPoint VC on VC.volaSurfaceDataID = vsd.VolaSurfaceDataID and fo.strike = VC.strike and VC.expiryID = e.expiryID

where cast('1'+e.ExpiryPeriod  as datetime) >=  Cast('1/' + str(month(getdate())) + '/' + str(Year(getdate())) as datetime)

go

 

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_GetVolaContr ]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure usp_GetVolaContr 

GO

 

create procedure usp_GetVolaContr

as

select distinct co.Symbol, Case when isnull(fu.iFutureUnderlyingTypeID,0) > 0 then dbo.fn_OptS2FutS(co.Symbol) else  fr.futureRootSymbol end as futureRootSymbol

, e.ExpiryPeriod, dbo.fn_NameContr(e.ExpiryPeriod,0) as ContrName, fo.isCall, fo.strike, cp.PriceClose, VC.Volatility  

from ContractPrice cp

inner join Contract co on co.ContractID = cp.ContractID

inner join FutureOption fo on fo.futureOptionID = cp.ContractID

inner join Future f on f.futureID = fo.futureID

inner join FutureRoot fr on fr.futureRootID = f.futureRootID

inner join Expiry e on e.expiryID = fo.expiryID

Left join FutureUnderlying fu on fu.iFutureUnderlyingID = fr.UnderlyingID

Left join Contract c on c.ContractID = fu.iFutureUnderlyingID

Left join VolaSurfaceData vsd on vsd.ContractID = c.ContractID

Left join CustomStrikeSkewPoint VC on VC.volaSurfaceDataID = vsd.VolaSurfaceDataID and fo.strike = VC.strike

where cast('1'+e.ExpiryPeriod  as datetime) >=  Cast('1/' + str(month(getdate())) + '/' + str(Year(getdate())) as datetime)

order by e.ExpiryPeriod, fr.futureRootSymbol, fo.strike, fo.isCall

go

 

 

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ICFutures_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure usp_ICFutures_Get

GO

 

create      PROC usp_ICFutures_Get

 @iICID int = null

AS

 -- retrieve future

 

 set nocount on

 

 if @iICID is not null

 SELECT 

  EIC.expiryDate     AS MaturityDate, 

  Cm.iFutureUnderlyingID  AS CommodityID, 

  Fr.futureRootName  AS FutureRootName, 

  Fr.futureLotSize  AS FutureLotSize, 

  Fr.futureRootSymbol  AS FutureRootSymbol,

  CF.symbol   AS vcSymbol, 

  Fr.optionLotSize  AS OptioLotSize, 

  Fr.exportSymbol  AS FRExportSymbol, 

  CF.contractName  AS FutureName, 

  F.futureRootID   AS FutureRootID,

  CP.priceClose  AS priceClose

  from Future F

 inner Join FutureRoot FR on FR.futureRootID = F.futureRootID And Fr.underlyingID =  @iICID

 inner join FutureUnderlying  CM on Cm.iFutureUnderlyingID  =  @iICID 

 inner join Contract   CF  on CF.ContractId    =  F.futureID

 inner join ExpiryInCalendarView EIC ON F.maturityID = EIC.expiryID and CF.expCalendarID = EIC.expCalendarID

 left join ContractPrice CP on CP.ContractID = CF.ContractId

 ORDER BY FutureName

 else

 SELECT 

  EIC.expiryDate     AS MaturityDate, 

  Cm.iFutureUnderlyingID  AS CommodityID, 

  Fr.futureRootName  AS FutureRootName, 

  Fr.futureLotSize  AS FutureLotSize, 

  Fr.futureRootSymbol  AS FutureRootSymbol,

  CF.symbol   AS vcSymbol, 

  Fr.optionLotSize  AS OptioLotSize, 

  Fr.exportSymbol  AS FRExportSymbol, 

  CF.contractName  AS FutureName, 

  F.futureRootID   AS FutureRootID,

  CP.priceClose  AS priceClose

  from Future F

 inner Join FutureRoot FR on FR.futureRootID = F.futureRootID 

 inner join FutureUnderlying  CM on  CM.iFutureUnderlyingID  is not null 

 inner join Contract   CF  on CF.ContractId    =  F.futureID

 inner join ExpiryInCalendarView EIC ON F.maturityID = EIC.expiryID and CF.expCalendarID = EIC.expCalendarID

 left join ContractPrice CP on CP.ContractID = CF.ContractId

 ORDER BY FutureName

go

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IC_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure usp_IC_Save

GO

 

 

create   PROCEDURE usp_IC_Save

      @iICID int,

      @vcSymbol varchar(20) = null,

      @vcName varchar(255) = null,

      @iICType int,   

      @tiGreeksCalcType tinyint = null,

      @tiIsActive tinyint = null,

      @iFutureUnderlyingTypeID int,

      @tiCheckExistance tinyint = null,

      @iExpCalendarID int = null,

      @iUndPriceProfileID int = null output,

      @iOptPriceProfileID int = null output

  

     as

    ---------------

    set nocount on

    ---------------

  --  if ((@iICType <> 6) And (@iICType <> 7 ))

  --    begin

  --      Raiserror ('Wrong Interest/FutureUnderlying type value', 16, 1)

  --      Return (-1)

  --    end

  

    if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID = @iICType and actionID < 3 and (@iICID <> contractID or @iICID is null))

      begin

        if isnull(@tiCheckExistance, 1) <> 0

          Raiserror ('Interest or FutureUnderlying with this ticker already exists!', 16, 1)

        Return (-1)

      end   

  

     if (@iICID is Null)  

     begin

       

       begin tran

        if @iUndPriceProfileID is null

        begin

          select

            @iUndPriceProfileID = priceProfileID

          from

            PriceProfileView

          where

            defaultID = 3

            and isOptionProfile = 0

        end

  

        if @iOptPriceProfileID is null

        begin

          select

            @iOptPriceProfileID = priceProfileID

          from

            PriceProfileView

          where

            defaultID = 1

            and isOptionProfile = 1

        end

 

      if @iICType = 6

      begin  

          --insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcSymbol, 'Default calendar')

          --set @iExpCalendarID = scope_identity()

 

               insert into Contract (contractTypeID, Symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)

                    values ( @iICType, @vcSymbol, @vcName, null, @iUndPriceProfileID, @iOptPriceProfileID)

      end

      else

      begin

          insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcSymbol, 'Default calendar')

          set @iExpCalendarID = scope_identity()  

 

               insert into Contract (contractTypeID, Symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)

                    values ( @iICType, @vcSymbol, @vcName, @iExpCalendarID, @iUndPriceProfileID, @iOptPriceProfileID) 

      end

  

         select @iICID = @@identity      

           if (@@error <> 0)

              begin

              RAISERROR ('Error. Can''t insert into Contract!', 16, 1)

              ROLLBACK TRAN

              RETURN (-1)

              end

         insert into [FutureUnderlying] (

                iFutureUnderlyingID, 

                tiActive,

                tiGreeksCalculationType,

                iFutureUnderlyingTypeID)

             values (

                @iICID, 

                @tiIsActive,

                @tiGreeksCalcType,

                @iFutureUnderlyingTypeID)

           if (@@error <> 0)

            begin

               RAISERROR ('Error. Can''t insert into [FutureUnderlying]!', 16, 1)

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

            contractName = isNull(@vcName,contractName),

            expCalendarID = isNull(@iExpCalendarID, expCalendarID),

            undPriceProfileID = isnull(@iUndPriceProfileID, undPriceProfileID),

            optPriceProfileID = isnull(@iOptPriceProfileID, optPriceProfileID)

          where contractID = @iICID

        if (@@error <> 0)

           begin

            RAISERROR ('Error. Can''t update Contract!', 16, 1)

            ROLLBACK TRAN

            RETURN (-1)

           end

        update [FutureUnderlying]

           set  tiGreeksCalculationType = isnull(@tiGreeksCalcType, tiGreeksCalculationType),

            tiActive = isnull(@tiIsActive, tiActive),

            iFutureUnderlyingTypeID = isnull(@iFutureUnderlyingTypeID, iFutureUnderlyingTypeID)

           where iFutureUnderlyingID = @iICID

         if (@@error <> 0)

           begin

            RAISERROR ('Error. Can''t update [FutureUnderlying]!', 16, 1)

            ROLLBACK TRAN

            RETURN (-1)

           end       

       commit tran

     end

  

    --========================================================================

    -- make record with null exchange for this index

    --========================================================================

  --  if not exists(select * from ContractPrice where exchangeID is null and contractID = @iICID)

  --    begin

  --      insert into ContractPrice(contractID,exchangeID)

  --        values(@iICID,null)

  --      if (@@error <> 0) begin rollback tran return(-1) end              

  --    end   

  

  if (@@error <> 0)

    return (-1)

  else  

    return @iICID

go

 

 

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureByIndex_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure usp_MmFutureByIndex_Get

GO

 

create PROC usp_MmFutureByIndex_Get

 @iIndexID int,

 @dtMinExpiry datetime = Null 

as

 -------------------------------------------------

 set nocount on

 -------------------------------------------------

 

 if @dtMinExpiry is null set @dtMinExpiry = 0

 

 select

  fc.contractID as iContractID,

  fc.contractTypeID as iContractTypeID,

  fc.symbol as vcSymbol,  

                fec.expiryDate as dtMaturity,

  iv.indexID as indexID,

                f.futureRootID as iFutureRootID

 from IndexView iv

                inner join FutureRoot fr on fr.underlyingID = iv.indexID

                inner join Future f on f.futureRootID = fr.futureRootID

  inner join ContractView fc on f.FutureID = fc.contractID    

                inner join ExpiryInCalendarView fec on f.MaturityID = fec.expiryID and fec.expCalendarID = fc.expCalendarID

 where iv.indexID = @iIndexID

  and fec.expiryDate >= @dtMinExpiry

    

 return @@error

go

 

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureByUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure usp_MmFutureByUnderlying_Get

GO

 

create   PROC usp_MmFutureByUnderlying_Get

 @iUnderlyingID int,

 @dtMinExpiry datetime = Null 

as

 -------------------------------------------------

 set nocount on

 -------------------------------------------------

 

 if @dtMinExpiry is null set @dtMinExpiry = 0 

 

 select

  f.futureID as iContractID,

  fc.contractTypeID as iContractTypeID,

  fc.symbol as vcSymbol,  

--                e.ExpiryPeriod as vcMaturity,

                fec.expiryDate as dtMaturity,

  fu.ContractId as iFutureUnderlyingID,

                f.futureRootID as iFutureRootID,

  isnull(ff.tiGreeksCalculationType, 1) as tiGreeksCalculationType

  

 from ContractView fu

                inner join FutureRoot fr on fr.underlyingID = fu.ContractId

                inner join Future f on f.futureRootID = fr.futureRootID

  inner join ContractView fc on f.futureID = fc.ContractId

--                inner join Expiry e on e.expiryID = f.MaturityID

                inner join ExpiryInCalendarView fec on f.MaturityID = fec.expiryID and fec.expCalendarID = fc.expCalendarID

  left join FutureUnderlying ff on ff.iFutureUnderlyingID = fu.ContractId

 where fu.ContractId = @iUnderlyingID

  and fec.expiryDate >= @dtMinExpiry  

 

 return @@error

go

 

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureExchangesByUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure usp_MmFutureExchangesByUnderlying_Get

GO

create PROC usp_MmFutureExchangesByUnderlying_Get

 @iUnderlyingID int

as

 set nocount on

 

 select e.exchangeID   as iExchangeID,

        e.isUnderlying as tiIsUnderlying,

        e.isOption     as tiIsOption,

        f.FutureID   as iFutureID 

 from future f

 inner join FutureRoot fr on fr.futureRootID = f.futureRootID and fr.underlyingID = @iUnderlyingID

 inner join Contract u on u.contractId = fr.underlyingID 

 inner join ContractExchangeLinkView e on f.FutureId = e.ContractID

 

 return @@error

go

 

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureOptionsByUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure usp_MmFutureOptionsByUnderlying_Get

GO

 

create PROC usp_MmFutureOptionsByUnderlying_Get

 @iUnderlyingID int,

 @dtMinExpiry datetime = Null

as

 -------------------------------------------------

 set nocount on

 -------------------------------------------------

 if @dtMinExpiry is null 

  begin

   select

    foc.contractID as iContractID,

    foc.contractTypeID as iContractTypeID,

    foc.symbol as vcSymbol,

    fo.strike as fStrike, 

    fec.expiryDate as dtExpiry,

    fo.isCall as tiIsCall,

    f.futureID as iFutureID

   from Future f

    inner join ContractView fc on f.futureID = fc.contractID

    inner join FutureRoot fr on fr.futureRootID = f.futureRootID and fr.underlyingID = @iUnderlyingID

    inner join FutureOption fo on f.futureID = fo.futureID

    inner join ContractView foc on fo.futureOptionID = foc.contractID

    inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = foc.expCalendarID

  end

 else

  begin

   select

    foc.contractID as iContractID,

    foc.contractTypeID as iContractTypeID,

    foc.symbol as vcSymbol,

    fo.strike as fStrike, 

    fec.expiryDate as dtExpiry,

    fo.isCall as tiIsCall,

    f.futureID as iFutureID

   from Future f

    inner join ContractView fc on f.futureID = fc.contractID

    inner join FutureRoot fr on fr.futureRootID = f.futureRootID and fr.underlyingID = @iUnderlyingID

    inner join FutureOption fo on f.futureID = fo.futureID

    inner join ContractView foc on fo.futureOptionID = foc.contractID

    inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = foc.expCalendarID

   where  fec.expiryDate >= @dtMinExpiry

  end 

 

 return @@error

go

 

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure usp_MmUnderlying_Get

GO

 

 

create   PROC usp_MmUnderlying_Get

 @iTraderID int = null

AS

 set nocount on

 

 if isnull(@iTraderID, 0) = 0

  select

      c.contractID as iContractID,

      c.contractTypeID as iContractTypeID,

      c.Symbol as vcSymbol,

   c.expCalendarID as iExpCalendarID,

   case when i.indexID is null then s.calcOptionType else i.calcOptionType end as tiCalcOptionType,

   case when i.indexID is null then s.isHTB else i.isHTB end as tiIsHTB,

  

      i.yield as fYield,

   i.isBasket as tiIsBasket,

      s.divFreq as iDivFreq,

      s.divDate as dtDivDate,

      s.divAmt as fDivAmt,

  

      s.isDivCustom as tiIsDivCustom,

      s.divFreqCustom as iDivFreqCustom,

      s.divDateCustom as dtDivDateCustom,

      s.divAmtCustom as fDivAmtCustom,

  

      c.priceClose as fPriceClose,

 

   null as iTraderID,

 

   case when i.indexID is null then s.skew else i.skew end as fSkew,

   case when i.indexID is null then s.kurt else i.kurt end as fKurt,

      c.contractName as vcContractName,

   

   c.undPriceProfileID as iUndPriceProfileID,

   c.optPriceProfileID as iOptPriceProfileID,

   case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

 

      c.volume10Day as iVolume10Day,

      c.volume5Expiration as iVolume5Expiration

   from ContractPriceView c 

      --inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null

      left join StockView s on s.stockID = c.contractID

      left join IndexView i on i.indexID = c.contractID

  where

   (c.contractTypeID < 3 or c.contractTypeID = 6) AND c.ActionId < 3

  order by c.Symbol

 else

  select

      c.contractID as iContractID,

      c.contractTypeID as iContractTypeID,

      c.Symbol as vcSymbol,

   c.expCalendarID as iExpCalendarID,

   case when i.indexID is null then s.calcOptionType else i.calcOptionType end as tiCalcOptionType,

   case when i.indexID is null then s.isHTB else i.isHTB end as tiIsHTB,

  

      i.yield as fYield,

   i.isBasket as tiIsBasket,

      s.divFreq as iDivFreq,

      s.divDate as dtDivDate,

      s.divAmt as fDivAmt,

  

      s.isDivCustom as tiIsDivCustom,

      s.divFreqCustom as iDivFreqCustom,

      s.divDateCustom as dtDivDateCustom,

      s.divAmtCustom as fDivAmtCustom,

  

      c.priceClose as fPriceClose,

 

   tc.traderID as iTraderID,

 

   case when i.indexID is null then s.skew else i.skew end as fSkew,

   case when i.indexID is null then s.kurt else i.kurt end as fKurt,

      c.contractName as vcContractName,

   

   c.undPriceProfileID as iUndPriceProfileID,

   c.optPriceProfileID as iOptPriceProfileID,

   case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

 

      c.volume10Day as iVolume10Day,

      c.volume5Expiration as iVolume5Expiration

  from ContractPriceView c 

     -- inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null

      left join StockView s on s.stockID = c.contractID

      left join IndexView i on i.indexID = c.contractID

      left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID

  where

   (c.contractTypeID < 3 or c.contractTypeID = 6) AND c.ActionId < 3

  order by c.Symbol

 return @@error

go

 

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure usp_MmUnderlying_Get

GO

 

create   PROC usp_MmUnderlying_Get

 @iTraderID int = null

AS

 set nocount on

 

 if isnull(@iTraderID, 0) = 0

  select

      c.contractID as iContractID,

      c.contractTypeID as iContractTypeID,

      c.Symbol as vcSymbol,

   c.expCalendarID as iExpCalendarID,

   case when i.indexID is null then s.calcOptionType else i.calcOptionType end as tiCalcOptionType,

   case when i.indexID is null then s.isHTB else i.isHTB end as tiIsHTB,

  

      i.yield as fYield,

   i.isBasket as tiIsBasket,

      s.divFreq as iDivFreq,

      s.divDate as dtDivDate,

      s.divAmt as fDivAmt,

  

      s.isDivCustom as tiIsDivCustom,

      s.divFreqCustom as iDivFreqCustom,

      s.divDateCustom as dtDivDateCustom,

      s.divAmtCustom as fDivAmtCustom,

  

      c.priceClose as fPriceClose,

 

   null as iTraderID,

 

   case when i.indexID is null then s.skew else i.skew end as fSkew,

   case when i.indexID is null then s.kurt else i.kurt end as fKurt,

      c.contractName as vcContractName,

   

   c.undPriceProfileID as iUndPriceProfileID,

   c.optPriceProfileID as iOptPriceProfileID,

   case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

 

      c.volume10Day as iVolume10Day,

      c.volume5Expiration as iVolume5Expiration

   from ContractPriceView c 

      --inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null

      left join StockView s on s.stockID = c.contractID

      left join IndexView i on i.indexID = c.contractID

  where

   (c.contractTypeID < 3 or c.contractTypeID = 6) AND c.ActionId < 3

  order by c.Symbol

 else

  select

      c.contractID as iContractID,

      c.contractTypeID as iContractTypeID,

      c.Symbol as vcSymbol,

   c.expCalendarID as iExpCalendarID,

   case when i.indexID is null then s.calcOptionType else i.calcOptionType end as tiCalcOptionType,

   case when i.indexID is null then s.isHTB else i.isHTB end as tiIsHTB,

  

      i.yield as fYield,

   i.isBasket as tiIsBasket,

      s.divFreq as iDivFreq,

      s.divDate as dtDivDate,

      s.divAmt as fDivAmt,

  

      s.isDivCustom as tiIsDivCustom,

      s.divFreqCustom as iDivFreqCustom,

      s.divDateCustom as dtDivDateCustom,

      s.divAmtCustom as fDivAmtCustom,

  

      c.priceClose as fPriceClose,

 

   tc.traderID as iTraderID,

 

   case when i.indexID is null then s.skew else i.skew end as fSkew,

   case when i.indexID is null then s.kurt else i.kurt end as fKurt,

      c.contractName as vcContractName,

   

   c.undPriceProfileID as iUndPriceProfileID,

   c.optPriceProfileID as iOptPriceProfileID,

   case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

 

      c.volume10Day as iVolume10Day,

      c.volume5Expiration as iVolume5Expiration

  from ContractPriceView c 

     -- inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null

      left join StockView s on s.stockID = c.contractID

      left join IndexView i on i.indexID = c.contractID

      left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID

  where

   (c.contractTypeID < 3 or c.contractTypeID = 6) AND c.ActionId < 3

  order by c.Symbol

 return @@error

go

 

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodCommodityFutureRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure usp_EodCommodityFutureRoot_Get

GO

 

create  PROC usp_EodCommodityFutureRoot_Get

AS

 -- retrieve all future roots

 

 set nocount on

 

 SELECT     fr.underlyingID AS iIndexID, 

     fr.futureRootID AS iFutureRootID, 

     fr.futureRootSymbol AS vcFutureRootSymbol, 

            fr.futureRootName AS vcFutureRootName

 FROM         dbo.FutureRoot fr INNER JOIN

                       dbo.Contract c ON fr.underlyingID = c.contractID

 WHERE     (c.contractTypeID = 6)

go

 

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodCommodity_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure usp_EodCommodity_Get

GO

 

create PROC usp_EodCommodity_Get

 @iInterestID int = null

AS

 -- retrieve future

 

 set nocount on

 

 if @iInterestID is not null

  SELECT  c.GreeksCalculationType as GreeksCalcType,

   c.IsActive as tiIsActive, 

   CV.contractTypeID as iContractTypeID,

   CV.symbol as vcSymbol, 

                 CV.contractName as vcContractName, 

   c.CommodityID as iContractID 

  FROM         dbo.Commodity C INNER JOIN

                       dbo.ContractView CV ON C.CommodityID = CV.contractID

  WHERE  CV.ContractID =  @iInterestID And CV.ContractTypeID = 7

  Order by CV.Symbol

 

 else

 

  SELECT  c.GreeksCalculationType as GreeksCalcType,

   c.IsActive as tiIsActive, 

   CV.contractTypeID as iContractTypeID,

   CV.symbol as vcSymbol, 

                 CV.contractName as vcContractName, 

   c.CommodityID as iContractID 

  FROM         dbo.Commodity C INNER JOIN

                       dbo.ContractView CV ON C.CommodityID = CV.contractID

  WHERE  CV.ContractTypeID = 7

  Order by CV.Symbol

go

 

 

-------------------------------------------------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodContractClosePrice_Get ]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure usp_EodContractClosePrice_Get 

GO

create Procedure usp_EodContractClosePrice_Get 

 @iContractID int = Null

AS

 -------------------------------------------------

 set nocount on

 -------------------------------------------------

 select PriceClose from ContractPrice where ContractId  = @iContractID

 

 

go

------------------------------------------------------------------------------------------------------------------------------------

alter   PROCEDURE usp_EodBasketIndexComp_Get 

 @iBasketIndexID int = null

AS

/*

 Created by: Michael Malyshkin

*/

 -------------------------------------------------

 set nocount on

 -------------------------------------------------

 declare @error int  set @error = -1

 

 if @iBasketIndexID is null

 begin

  select

   i.indexID as iBasketIndexID,

   idef.stockID as iUnderlyingID,

   idef.weight as fWeight

  from

   IndexDefinitionView idef

   inner join IndexView i on idef.indexID = i.indexID and isnull(i.isBasket, 0) <> 0

  where

   isnull(idef.weight, 0) <> 0

 end

 else

 begin

  select

   i.indexID as iBasketIndexID,

   idef.stockID as iUnderlyingID,

   idef.weight as fWeight

  from

   IndexDefinitionView idef

   inner join IndexView i on idef.indexID = i.indexID and isnull(i.isBasket, 0) <> 0

  where

   idef.indexID = @iBasketIndexID

   and isnull(idef.weight, 0) <> 0

 end

  

 set @error = @@error

Return(@error)

go

 

 

---------------------------------------------------------------------------------------------------------------------

alter    Procedure usp_EodExpirationDate_Get

as

 -------------------------------------------------

 set nocount on

 -------------------------------------------------

 declare @error int  set @error = -1

 

 select distinct expiryDate as dtExpiryDate

 from ExpiryInCalendarView

 where expiryDate > getdate()

 order by expiryDate

 

 set @error = @@error

Return(@error)

go

 

 

----------------------------------------------------------------------------------------------------------------------

 

alter PROC usp_EodFutureOption_Del

AS

 set nocount on

 

 declare @error int  set @error = 0

 

 if object_ID('tempdb..#FutureContractsForDelete') is null

 begin

  set @error = -1

  goto finish

 end

 

 begin tran

  

 -- delete future option

 delete FutureOption

 where futureOptionID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -1 if @@trancount = 1 rollback tran goto finish end

 

 -- delete common

 

 delete ContractPrice

 where contractID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -2 if @@trancount = 1 rollback tran goto finish end

 

 

 update Contract set actionID = 3

 where contractID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran goto finish end

 

 commit tran

 

finish:

 return @error

go

 

 

-----------------------------------------------------------------------------------------------------------------------

alter PROC usp_EodFuture_Del

AS

 set nocount on

 

 declare @error int  set @error = 0

 

 if object_ID('tempdb..#FutureContractsForDelete') is null

 begin

  set @error = -1

  goto finish

 end

 

 begin tran

  

 -- delete future option

 delete FutureOption

 where futureID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -2 if @@trancount = 1 rollback tran goto finish end

 

 -- delete future

 delete Future where

 futureID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran goto finish end

 

 

 -- delete common future and future options

 

 delete ContractPrice

 where contractID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -4 if @@trancount = 1 rollback tran goto finish end

 

 update ContractExchangeLink set actionID = 3

 where contractID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

 

 update Contract set actionID = 3

 where contractID in (select contractID from #FutureContractsForDelete)

 if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end

 

 commit tran

 

finish:

 return @error

go

 

 

--------------------------------------------------------------------------------------------------------------

 

 

alter PROCEDURE usp_EodFuturesOptionWithPositions_Get

  as

 ---------------

 set nocount on

 ---------------

 

 declare @error int  set @error = 0

 

--=================================================================

-- select future option with positions

--=================================================================

 select distinct 

  FO.futureID as iFutureID,

  FC.symbol as vcFutureSymbol,

  FOC.symbol as vcOptionSymbol,

  FO.futureOptionID as iID,

  FO.isCall as tiIsCall,

  FO.strike as fStrike,

  EC.expiryDate as dtExpiry

 from

  TradeView tv  

  inner join FutureOption FO on FO.futureOptionID = tv.contractID

  inner join ContractView FOC on FOC.contractID = tv.contractID

  inner join ContractView FC on FO.futureID = FC.contractID

  inner join ExpiryInCalendarView EC on FO.expiryID = EC.expiryID and EC.expCalendarID = FC.expCalendarID

 where

  ec.expiryDate >= getdate()

 

set @error = @@error

Return(@error)

go

 

 

-----------------------------------------------------------------------------------------------------------

 

alter PROCEDURE usp_EodFuturesWithPositions_Get

  as

 ---------------

 set nocount on

 ---------------

 

 declare @error int  set @error = 0

 

--=================================================================

-- select futures with positions

--=================================================================

 select 

  FR.underlyingID as iIndexID,

  F.futureID as iFutureID,

  FC.symbol as vcSymbol,

  F.isActive as tiIsActive,

  F.maturityID as iMaturityID,

  sum(T.quantity * T.price * (case when T.isBuy <> 0 then 1 else -1 end)) as fPosition,

  cp.priceClose as fPriceClose

 from TradeView T  

  inner join Future F on T.contractID=F.futureID

  inner join ContractView FC on FC.contractID=T.contractID

  inner join FutureRoot FR on FR.futureRootID=F.futureRootID

  inner join ContractPrice cp on cp.contractID = FC.contractID and cp.exchangeID is null

 group by FR.underlyingID, F.futureID, FC.symbol, F.isActive, F.maturityID, cp.priceClose

 order by FR.underlyingID 

 

set @error = @@error

Return(@error)

go

------------------------------

alter PROCEDURE usp_EodFuturesWithPositions_Get

  as

 ---------------

 set nocount on

 ---------------

 

 declare @error int  set @error = 0

 

--=================================================================

-- select futures with positions

--=================================================================

 select 

  FR.underlyingID as iIndexID,

  F.futureID as iFutureID,

  FC.symbol as vcSymbol,

  F.isActive as tiIsActive,

  F.maturityID as iMaturityID,

  sum(T.quantity * T.price * (case when T.isBuy <> 0 then 1 else -1 end)) as fPosition,

  cp.priceClose as fPriceClose

 from TradeView T  

  inner join Future F on T.contractID=F.futureID

  inner join ContractView FC on FC.contractID=T.contractID

  inner join FutureRoot FR on FR.futureRootID=F.futureRootID

  inner join ContractPrice cp on cp.contractID = FC.contractID and cp.exchangeID is null

 group by FR.underlyingID, F.futureID, FC.symbol, F.isActive, F.maturityID, cp.priceClose

 order by FR.underlyingID 

 

set @error = @@error

Return(@error)

go

 

 

alter PROCEDURE usp_EodMsUnderlyingCache_Check

 @iContractID int

as

/*

 Created by Michael Malyshkin

 Checked if no options of the underlying exist in MsOptionCache then delete underlying record

*/

 -------------------------------------------------

 set nocount on

 -------------------------------------------------

 declare @error int  set @error = 0

 

 if not exists(select * from MsOptionCache O where O.underlyingID = @iContractID)

  begin

   delete from MsUnderlyingCache where underlyingID=@iContractID

   

   if @@error <> 0 set @error = @@error

  end

  

   set @error = 0

finish:

Return(@error)

go


alter      procedure usp_CustomStrikeSkewDefaultPoint2_Save
 @iContractID int,
 @iExpiryID int,
 @iType int,
 @fVolatility float
as
 -------------------------------------------------
 set nocount on
 -------------------------------------------------
 declare @error int  set @error = -1
 declare @RootType int

 

 --check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
 declare @iVolaSurfaceDataID int set @iVolaSurfaceDataID = 0

 

 exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

 

 if @iVolaSurfaceDataID is not null
 begin
  exec @RootType =  usp_RootType_Get @iContractID
  -- option root
  if @RootType = 0 
  begin
   insert into CustomStrikeSkewPoint(VolaSurfaceDataID, strike, type, volatility, isBasePoint, expiryID)
     select distinct 
      @iVolaSurfaceDataID, round(ov.strike, 4) as fStrike, @iType, @fVolatility, -1, @iExpiryID
     from OptionView ov
      join OptionRootView ort on ov.optionRootID = ort.optionRootID
     where ov.expiryID = @iExpiryID 
      and ort.underlyingID = @iContractID
      and not exists(select 1 
          from CustomStrikeSkewPointView cssp
          where cssp.VolaSurfaceDataID = @iVolaSurfaceDataID and 
            round(cssp.strike, 4) = round(ov.strike, 4)
            and cssp.expiryID = ov.expiryID)
 
  end
  

 

  -- future option root
  if @RootType = 1 
  begin
   insert into CustomStrikeSkewPoint(VolaSurfaceDataID, strike, type, volatility, isBasePoint, expiryID)
    select distinct 
     @iVolaSurfaceDataID, round(fo.strike, 4) as fStrike, @iType, @fVolatility, -1, @iExpiryID
    from    dbo.Future f inner JOIN
            dbo.FutureOption fo  on f.futureID = fo.futureID inner JOIN
                          dbo.Expiry e ON fo.expiryID = e.expiryID
    where   f.MaturityID = @iExpiryID 
     and f.futureID = @iContractID
     and not exists(select 1 
         from CustomStrikeSkewPointView cssp
         where cssp.VolaSurfaceDataID = @iVolaSurfaceDataID and 
           round(cssp.strike, 4) = round(fo.strike, 4)
           and cssp.expiryID = fo.ExpiryID)

 

  end
 
 end
go
------------------------------------------------------------------------------------------------------------------------------------
alter   PROCEDURE usp_EodBasketIndexComp_Get 
 @iBasketIndexID int = null
AS
/*
 Created by: Michael Malyshkin
*/
 -------------------------------------------------
 set nocount on
 -------------------------------------------------
 declare @error int  set @error = -1

 

 if @iBasketIndexID is null
 begin
  select
   i.indexID as iBasketIndexID,
   idef.stockID as iUnderlyingID,
   idef.weight as fWeight
  from
   IndexDefinitionView idef
   inner join IndexView i on idef.indexID = i.indexID and isnull(i.isBasket, 0) <> 0
  where
   isnull(idef.weight, 0) <> 0
 end
 else
 begin
  select
   i.indexID as iBasketIndexID,
   idef.stockID as iUnderlyingID,
   idef.weight as fWeight
  from
   IndexDefinitionView idef
   inner join IndexView i on idef.indexID = i.indexID and isnull(i.isBasket, 0) <> 0
  where
   idef.indexID = @iBasketIndexID
   and isnull(idef.weight, 0) <> 0
 end
  
 set @error = @@error
Return(@error)
go

 


---------------------------------------------------------------------------------------------------------------------
alter    Procedure usp_EodExpirationDate_Get
as
 -------------------------------------------------
 set nocount on
 -------------------------------------------------
 declare @error int  set @error = -1

 

 select distinct expiryDate as dtExpiryDate
 from ExpiryInCalendarView
 where expiryDate > getdate()
 order by expiryDate

 

 set @error = @@error
Return(@error)
go

 


----------------------------------------------------------------------------------------------------------------------

 

alter PROC usp_EodFutureOption_Del
AS
 set nocount on
 
 declare @error int  set @error = 0

 

 if object_ID('tempdb..#FutureContractsForDelete') is null
 begin
  set @error = -1
  goto finish
 end

 

 begin tran
  
 -- delete future option
 delete FutureOption
 where futureOptionID in (select contractID from #FutureContractsForDelete)
 if @@error <> 0 begin set @error = -1 if @@trancount = 1 rollback tran goto finish end
 
 -- delete common
 
 delete ContractPrice
 where contractID in (select contractID from #FutureContractsForDelete)
 if @@error <> 0 begin set @error = -2 if @@trancount = 1 rollback tran goto finish end
 
 
 update Contract set actionID = 3
 where contractID in (select contractID from #FutureContractsForDelete)
 if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran goto finish end

 

 commit tran

 

finish:
 return @error
go

 


-----------------------------------------------------------------------------------------------------------------------
alter PROC usp_EodFuture_Del
AS
 set nocount on
 
 declare @error int  set @error = 0

 

 if object_ID('tempdb..#FutureContractsForDelete') is null
 begin
  set @error = -1
  goto finish
 end

 

 begin tran
  
 -- delete future option
 delete FutureOption
 where futureID in (select contractID from #FutureContractsForDelete)
 if @@error <> 0 begin set @error = -2 if @@trancount = 1 rollback tran goto finish end
 
 -- delete future
 delete Future where
 futureID in (select contractID from #FutureContractsForDelete)
 if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran goto finish end

 


 -- delete common future and future options
 
 delete ContractPrice
 where contractID in (select contractID from #FutureContractsForDelete)
 if @@error <> 0 begin set @error = -4 if @@trancount = 1 rollback tran goto finish end
 
 update ContractExchangeLink set actionID = 3
 where contractID in (select contractID from #FutureContractsForDelete)
 if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

 

 update Contract set actionID = 3
 where contractID in (select contractID from #FutureContractsForDelete)
 if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end

 

 commit tran

 

finish:
 return @error
go

 


--------------------------------------------------------------------------------------------------------------

 


alter PROCEDURE usp_EodFuturesOptionWithPositions_Get
  as
 ---------------
 set nocount on
 ---------------

 

 declare @error int  set @error = 0

 

--=================================================================
-- select future option with positions
--=================================================================
 select distinct 
  FO.futureID as iFutureID,
  FC.symbol as vcFutureSymbol,
  FOC.symbol as vcOptionSymbol,
  FO.futureOptionID as iID,
  FO.isCall as tiIsCall,
  FO.strike as fStrike,
  EC.expiryDate as dtExpiry
 from
  TradeView tv  
  inner join FutureOption FO on FO.futureOptionID = tv.contractID
  inner join ContractView FOC on FOC.contractID = tv.contractID
  inner join ContractView FC on FO.futureID = FC.contractID
  inner join ExpiryInCalendarView EC on FO.expiryID = EC.expiryID and EC.expCalendarID = FC.expCalendarID
 where
  ec.expiryDate >= getdate()
 
set @error = @@error
Return(@error)
go

 


-----------------------------------------------------------------------------------------------------------

 

alter PROCEDURE usp_EodFuturesWithPositions_Get
  as
 ---------------
 set nocount on
 ---------------

 

 declare @error int  set @error = 0

 

--=================================================================
-- select futures with positions
--=================================================================
 select 
  FR.underlyingID as iIndexID,
  F.futureID as iFutureID,
  FC.symbol as vcSymbol,
  F.isActive as tiIsActive,
  F.maturityID as iMaturityID,
  sum(T.quantity * T.price * (case when T.isBuy <> 0 then 1 else -1 end)) as fPosition,
  cp.priceClose as fPriceClose
 from TradeView T  
  inner join Future F on T.contractID=F.futureID
  inner join ContractView FC on FC.contractID=T.contractID
  inner join FutureRoot FR on FR.futureRootID=F.futureRootID
  inner join ContractPrice cp on cp.contractID = FC.contractID and cp.exchangeID is null
 group by FR.underlyingID, F.futureID, FC.symbol, F.isActive, F.maturityID, cp.priceClose
 order by FR.underlyingID 
 
set @error = @@error
Return(@error)
go
------------------------------
alter PROCEDURE usp_EodFuturesWithPositions_Get
  as
 ---------------
 set nocount on
 ---------------

 

 declare @error int  set @error = 0

 

--=================================================================
-- select futures with positions
--=================================================================
 select 
  FR.underlyingID as iIndexID,
  F.futureID as iFutureID,
  FC.symbol as vcSymbol,
  F.isActive as tiIsActive,
  F.maturityID as iMaturityID,
  sum(T.quantity * T.price * (case when T.isBuy <> 0 then 1 else -1 end)) as fPosition,
  cp.priceClose as fPriceClose
 from TradeView T  
  inner join Future F on T.contractID=F.futureID
  inner join ContractView FC on FC.contractID=T.contractID
  inner join FutureRoot FR on FR.futureRootID=F.futureRootID
  inner join ContractPrice cp on cp.contractID = FC.contractID and cp.exchangeID is null
 group by FR.underlyingID, F.futureID, FC.symbol, F.isActive, F.maturityID, cp.priceClose
 order by FR.underlyingID 
 
set @error = @@error
Return(@error)
go

 


alter PROCEDURE usp_EodMsUnderlyingCache_Check
 @iContractID int
as
/*
 Created by Michael Malyshkin
 Checked if no options of the underlying exist in MsOptionCache then delete underlying record
*/
 -------------------------------------------------
 set nocount on
 -------------------------------------------------
 declare @error int  set @error = 0

 

 if not exists(select * from MsOptionCache O where O.underlyingID = @iContractID)
  begin
   delete from MsUnderlyingCache where underlyingID=@iContractID
   
   if @@error <> 0 set @error = @@error
  end
  
   set @error = 0
finish:
Return(@error)
go

 


-------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodInterestFutureRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure usp_EodInterestFutureRoot_Get

GO

 

create  PROC usp_EodInterestFutureRoot_Get
AS
	-- retrieve all future roots

	set nocount on

	SELECT     fr.underlyingID AS iIndexID, 
		   fr.futureRootID AS iFutureRootID, 
		   fr.futureRootSymbol AS vcFutureRootSymbol, 
	           fr.futureRootName AS vcFutureRootName
	FROM         dbo.FutureRoot fr INNER JOIN
	                      dbo.Contract c ON fr.underlyingID = c.contractID
	WHERE     (c.contractTypeID = 6)
go


-------------------------------------------

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodInterest_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)

      drop procedure usp_EodInterest_Get

GO

 

create   PROC usp_EodInterest_Get
	@iInterestID int = null
AS
	-- retrieve future

	set nocount on

	if @iInterestID is not null
		SELECT 	c.GreeksCalculationType as GreeksCalcType,
			c.IsActive as tiIsActive, 
			CV.contractTypeID as iContractTypeID,
			CV.symbol as vcSymbol, 
	                CV.contractName as vcContractName, 
			c.CommodityID as iContractID 
		FROM         dbo.Commodity C INNER JOIN
	                      dbo.ContractView CV ON C.CommodityID = CV.contractID
		WHERE 	CV.ContractID =  @iInterestID And CV.ContractTypeID = 6
		Order by CV.Symbol

	else
	
		SELECT 	c.GreeksCalculationType as GreeksCalcType,
			c.IsActive as tiIsActive, 
			CV.contractTypeID as iContractTypeID,
			CV.symbol as vcSymbol, 
	                CV.contractName as vcContractName, 
			c.CommodityID as iContractID 
		FROM         dbo.Commodity C INNER JOIN
	                      dbo.ContractView CV ON C.CommodityID = CV.contractID
		WHERE 	CV.ContractTypeID = 6
		Order by CV.Symbol
go



GO
/* RESULTS ANALYZING */
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.131')
	begin
		update DataInfo set keyValue = '4.49.131' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.131' '4.49.131 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
-- unable to execute this script in transaction
