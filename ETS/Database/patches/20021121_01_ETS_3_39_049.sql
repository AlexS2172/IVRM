/*
	DB update for nearest release
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.39.048' or (keyValue = '3.39.049')))
	BEGIN
		PRINT 'You can''t run this script, because you have not run script for previous version!'
		Select cast(keyValue as varchar(25)) as 'DB version', '3.39.049' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE           proc dbo.usp_Option_Get
	@iUnderlyingID int = Null,		 
	@dtMinExpiry datetime = Null,
	@iOptionID int = Null,
	@tiBaseInfoOnly tinyint = 0,
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
				ORt.underlyingID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				ORt.lotSize as iLotSize,
				ORt.optionRootID as iOptionRootID,
				ORt.symbol as vcOptionRootSymbol
			from ContractView C  
				inner join OptionView O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID
				inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((ORt.underlyingID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
			order by ORt.underlyingID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				ORt.underlyingID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				ORt.lotSize as iLotSize,
				ORt.optionRootID as iOptionRootID,
				ORt.symbol as vcOptionRootSymbol
			from ContractView c  
				inner join OptionView O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID
				inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((ORt.underlyingID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
	end
	else
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				ORt.underlyingID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				ORt.lotSize as iLotSize,
				ORt.optionRootID as iOptionRootID,
				ORt.symbol as vcOptionRootSymbol
			from ContractView C  
				inner join OptionView O on O.optionID = C.contractID
				left join TradeView tr on tr.contractID = c.contractID and (tr.traderID = @iTraderID or @iTraderID is Null)
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID
				inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID

			where c.contractTypeID=3
--				and (tr.traderID = @iTraderID or @iTraderID is Null)
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((ORt.underlyingID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName, ORt.underlyingID,
				O.strike, EC.expiryDate, O.isCall, O.actionDate, O.actionID,ORt.lotSize, ORt.optionRootID, ORt.symbol
			order by ORt.underlyingID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				ORt.underlyingID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				ORt.lotSize as iLotSize,
				ORt.optionRootID as iOptionRootID,
				ORt.symbol as vcOptionRootSymbol
			from ContractView c  
				inner join OptionView O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null						
				left join TradeView tr on tr.contractID = c.contractID and (tr.traderID = @iTraderID or @iTraderID is Null)
				join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID
				inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where c.contractTypeID=3
--				and (tr.traderID = @iTraderID or @iTraderID is Null)
				and ((EC.expiryDate >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
				and ((ORt.underlyingID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName, ORt.underlyingID,
				O.strike, EC.expiryDate, O.isCall, O.actionDate, O.actionID,ORt.lotSize, ORt.optionRootID, ORt.symbol
	end


	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Book_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Book_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Broker_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Broker_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Commission_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Commission_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractForTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractForTrade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewDefaultPoint2_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomStrikeSkewDefaultPoint2_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomVolatilityPointIsExist2_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomVolatilityPointIsExist2_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionRootCA_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionRootCA_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionStrikeCA_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionStrikeCA_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Strategy_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Strategy_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeByTrader_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSeq_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeSeq_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContract_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderContract_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trader_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trader_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingExpiryByList_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingExpiryByList_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingExpiry_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingExpiry_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingOptionByExpiry_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingOptionByExpiry_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingPosByList_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingPosByList_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE    proc dbo.usp_Book_Del
		@iBookID int,
		@tiIsRaiseError tinyint = 1 
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if exists (select * from TradeView where bookID = @iBookID)
		begin
			if @tiIsRaiseError = 1
				begin
					Raiserror('Can''t remove book, because this book has trades!',16,1)
					Return(0)
				end
			else
				begin
					Return(-1)
				end
		end

	Begin Transaction

		declare @iPriority int
			select @iPriority = priority from Book where bookID=@iBookID

		update Book set priority = priority-1 where priority > @iPriority
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	
		update Book set actionID=3 where bookID = @iBookID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
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


CREATE  procedure dbo.usp_Broker_Del
	@iBrokerID int,
	@tiIsRaiseError tinyint = 1 
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Broker table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	if exists (select * from TradeView where brokerID = @iBrokerID or clearingBrokerID = @iBrokerID)
		begin
			if @tiIsRaiseError = 1
				begin
					Raiserror('Can''t remove broker, because this broker have a trades!',16,1)
					Return(0)
				end
			else
				begin
					Return(-1)
				end
		end
	
	Begin Transaction

		update Commission set actionID=3 where brokerID = @iBrokerID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		update Trade set actionID=0 where brokerID = @iBrokerID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		update Broker set actionID=3 where brokerID = @iBrokerID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		set @error = @@error
	Commit Transaction
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



CREATE    proc dbo.usp_Commission_Del
		@iCommissionID int,
		@iBrokerID int = Null,
		@tiIsRaiseError tinyint = 1 
  as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	if (@iCommissionID is Null) and (@iBrokerID is Null)
		Return (0)	--Return (-2)

	if exists(select * from Trade 
		where (brokerCommissionID = @iCommissionID or clearingBrokerCommissionID = @iCommissionID) and 
				(brokerID = @iBrokerID or @iBrokerID is null))
		begin
			if @tiIsRaiseError = 1
				begin
					Raiserror('Can''t remove commission, because this commission have a trades!',16,1)
					Return(0)
				end
			else
				begin
					Return(-1)
				end
		end
	
	   if (@iCommissionID is Null) 
			update Commission set actionID=3 where brokerID = @iBrokerID
		else
			update Commission set actionID=3 where commissionID = @iCommissionID

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



CREATE            proc dbo.usp_ContractForTrade_Get
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
				null as iOptionRootID,
				null as tiIsSyntheticRoot
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
				ort.optionRootID as iOptionRootID,
				ort.isSynthetic as tiIsSyntheticRoot
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
				null as iOptionRootID,
				null as tiIsSyntheticRoot
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
				ort.optionRootID as iOptionRootID,
				ort.isSynthetic as tiIsSyntheticRoot
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


CREATE    procedure dbo.usp_CustomStrikeSkewDefaultPoint2_Save
	@iContractID int,
	@iExpiryID int,
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

	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	if @iVolaSurfaceDataID is not null
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


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO







CREATE       Procedure dbo.usp_CustomVolatilityPointIsExist2_Get
	@vcSymbolName varchar(20)
as
/*
	Created by Zaur Nuraliev
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	select 
			CV.contractID as iContractID,
			OV.expiryID as iExpiryID
		from OptionView OV
			join OptionRoot ort on ort.optionRootID = OV.optionRootID
			join ContractView CV on Ort.underlyingID = CV.contractID
			left join VolaSurfaceDataView VSD on Ort.underlyingID = VSD.contractID and VSD.isDefaultSurface = 1
			left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID and OV.expiryID = CSSP.expiryID
		where 
			CV.symbol = @vcSymbolName
			and CSSP.volaSurfaceDataID is null
		group by CV.contractID, ov.expiryID



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE                   PROCEDURE dbo.usp_OptionRootCA_Save 
	@iCorpActionID int, 
	@iOptionRootID int,
	@vcSymbol varchar(20) = null,
	@iUnderlyingID int = null,
	@iLotSize int = null,
	@tiIsSynthetic tinyint = 0,
	@fCashValue float = null
AS
/*
	Created by: Z
	Description: this procedure created with template, desined by Zaur Nuraliev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @vcTableName sysname

	--===============================================
	-- variables for work with CorporateActionLog
	--================================================
	declare @iCAActionID int, --1 for insert, 2 - for update, 3 - for delete
			@vcKeyRow varchar(50),  -- name of field with identity atribute
			@iRowID int,			-- tipically is identity value
			@vcFieldName varchar(50),
			@vcFieldValue varchar(250),
			@vcFieldType varchar(50),
			@iOldUnderlyingID int,
			@vcOldSymbol varchar(20),
			@tiOldisSynthetic tinyint,
			@iOldLotSize int,
			@vcLogMessage varchar(250)

	if object_ID('tempdb..#CAStatusLog') is null
		begin	
			create table #CAStatusLog(
						logMessage varchar(250) null)
			if @@error <> 0 begin set @error = 1 goto finish end
		end

	Begin Transaction CA1
		--======================================================
		-- write into CorporateActionLog update of OptionRoot
		--======================================================
		select 	@iOldUnderlyingID = underlyingID,
				@vcOldSymbol = symbol,
				@tiOldisSynthetic = isSynthetic,
				@iOldLotSize = lotSize
			from OptionRootView 
			where optionRootID = @iOptionRootID

		update OptionRoot 
			set symbol = isnull(@vcSymbol, symbol),
				underlyingID = isnull(@iUnderlyingID, underlyingID),
				isSynthetic = isnull(@tiIsSynthetic,isSynthetic),
				lotSize = isnull(@iLotSize, lotSize)
		where optionRootID = @iOptionRootID and actionID < 3
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction CA1 goto finish end

		set @vcLogMessage = 'Update ''' + @vcOldSymbol + ''' Option Root : SET (Symbol, Is Synthetic,Lot Size) = (''' + isnull(@vcSymbol, @vcOldSymbol) +  ''',' + 
			convert(varchar(5),@tiIsSynthetic) + ',' + convert(varchar(25),isnull(@iLotSize,@iOldLotSize)) + ')' 

		insert into #CAStatusLog(logMessage) values(@vcLogMessage)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction CA1 goto finish end

		set @iCAActionID = 2
		set @vcKeyRow = 'optionRootID'
		set @iRowID = @iOptionRootID
		set @vcTableName = 'OptionRoot'

		
		select @vcFieldName = 'symbol', @vcFieldValue = @vcOldSymbol, @vcFieldType = 'varchar(20)'
		exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow 
   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

		select @vcFieldName = 'underlyingID', @vcFieldValue = @iOldUnderlyingID, @vcFieldType = 'int'
		exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

		select @vcFieldName = 'isSynthetic', @vcFieldValue = @tiOldisSynthetic, @vcFieldType = 'tinyint'
		exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

		select @vcFieldName = 'lotSize', @vcFieldValue = @iOldLotSize, @vcFieldType = 'int'
		exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

		if @tiIsSynthetic = 1
			begin
				if not exists(select * From SyntheticOptionRootView where optionRootID = @iOptionRootID)
					begin
						insert into SyntheticOptionRoot(
							optionRootID,
							cashValue)
						values (

							@iOptionRootID,
							@fCashValue)
			   			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction CA1 goto finish end
						set @iRowID = @@identity

						--================================================================
						-- write into CorporateActionLog insert into SyntheticOptionRoot
						--================================================================
						set @iCAActionID = 1
						set @vcTableName = 'SyntheticOptionRoot'
						set @vcKeyRow = 'syntheticOptRootID'

						select @vcFieldName = 'optionRootID', @vcFieldValue = @iOptionRootID, @vcFieldType = 'int'
						exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															
					end
				else
					begin
						declare @fOldCashValue float
						
						select 	@iRowID = syntheticOptRootID,
								@fOldCashValue = cashValue
							from SyntheticOptionRootView
							where optionRootID = @iOptionRootID
	

						update SyntheticOptionRoot set
							cashValue = isnull(@fCashValue, cashValue)
						where optionRootID = @iOptionRootID and actionID < 3		
				   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction CA1 goto finish end
						--=================================================================
						-- write into CorporateActionLog update of SyntheticOptionRoot
						--=================================================================
						set @iCAActionID = 2
						set @vcTableName = 'SyntheticOptionRoot'
						set @vcKeyRow = 'syntheticOptRootID'					

						select @vcFieldName = 'cashValue', @vcFieldValue = @fOldCashValue, @vcFieldType = 'float'
						exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

				end

			end
		--we delete unnecessary record
		else
			begin
				--=========================================================================
				-- write into CorporateActionLog update of SyntheticUnderlyingParam table
				--=========================================================================
				set @iCAActionID = 3
				--set @iRowID = @@identity
				set @vcTableName = 'SyntheticUnderlyingParam'
				set	@vcKeyRow = 'syntheticUndParamID'
				
				declare	@iSyntheticUndParamID int,
					@iSyntheticOptRootID int,
					@fSyntheticCoeff float

				DECLARE synthundparam_cursor CURSOR
				READ_ONLY
				FOR select
						S1.syntheticUndParamID,
						S1.syntheticOptRootID,
						S1.underlyingID,
						S1.syntheticCoeff
					from SyntheticUnderlyingParamView S1
							join SyntheticOptionRootView S2	on S1.syntheticOptRootID = S2.syntheticOptRootID
					where S2.optionRootID = @iOptionRootID

				Open synthundparam_cursor
					fetch next from synthundparam_cursor into 
						@iSyntheticUndParamID,
						@iSyntheticOptRootID,
						@iOldUnderlyingID,
						@fSyntheticCoeff
					
				while (@@fetch_status <> -1)
				begin
					if (@@fetch_status <> -2)
					BEGIN

					--============================================
					-- execute update of SyntheticUnderlyingParam
					--============================================
					update SyntheticUnderlyingParam
						set actionID = 3 
						where syntheticUndParamID = @iSyntheticUndParamID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction CA1 goto finish end

					set @iRowID = @iSyntheticUndParamID

					-- write into CorporateActionLog
					select @vcFieldName = 'syntheticUndParamID', @vcFieldValue = @iSyntheticUndParamID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					select @vcFieldName = 'syntheticOptRootID', @vcFieldValue = @iSyntheticOptRootID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					select @vcFieldName = 'underlyingID', @vcFieldValue = @iOldUnderlyingID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					select @vcFieldName = 'syntheticCoeff', @vcFieldValue = @fSyntheticCoeff, @vcFieldType = 'float'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					END
					fetch next from synthundparam_cursor into 
						@iSyntheticUndParamID,
						@iSyntheticOptRootID,
						@iOldUnderlyingID,
						@fSyntheticCoeff
				end
				Close synthundparam_cursor
				Deallocate synthundparam_cursor

				--=========================================================================
				-- write into CorporateActionLog update of SyntheticRootIndexBeta table
				--=========================================================================
				set @iCAActionID = 3
				set @vcTableName = 'SyntheticRootIndexBeta'
				set	@vcKeyRow = 'syntheticUndParamID'
				
				declare	@iSyntheticIndexBetaID int,
					@fBeta float

				DECLARE synthindexbeta_cursor CURSOR
				READ_ONLY
				FOR select
						S1.syntheticIndexBetaID,
						S1.syntheticOptRootID,
						S1.indexID,
						S1.beta
					from SyntheticRootIndexBetaView S1
							join SyntheticOptionRootView S2	on S1.syntheticOptRootID = S2.syntheticOptRootID
					where S2.optionRootID = @iOptionRootID

				Open synthindexbeta_cursor
					fetch next from synthindexbeta_cursor into 
						@iSyntheticIndexBetaID,
						@iSyntheticOptRootID,
						@iOldUnderlyingID,
						@fBeta
					
				while (@@fetch_status <> -1)
				begin
					if (@@fetch_status <> -2)
					BEGIN

					--============================================
					-- execute update of SyntheticRootIndexBeta
					--============================================
					update SyntheticRootIndexBeta
						set actionID = 3 
						where syntheticIndexBetaID = @iSyntheticIndexBetaID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction CA1 goto finish end

					set @iRowID = @iSyntheticIndexBetaID

					-- write into CorporateActionLog
					select @vcFieldName = 'syntheticIndexBetaID', @vcFieldValue = @iSyntheticIndexBetaID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					select @vcFieldName = 'syntheticOptRootID', @vcFieldValue = @iSyntheticOptRootID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					select @vcFieldName = 'indexID', @vcFieldValue = @iOldUnderlyingID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					select @vcFieldName = 'beta', @vcFieldValue = @fBeta, @vcFieldType = 'float'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					END
					fetch next from synthindexbeta_cursor into 
						@iSyntheticUndParamID,
						@iSyntheticOptRootID,
						@iOldUnderlyingID,
						@fSyntheticCoeff
				end
				Close synthindexbeta_cursor
				Deallocate synthindexbeta_cursor

				--=========================================================================
				-- write into CorporateActionLog update of SyntheticOptionRoot table
				--=========================================================================
				declare	@fSynthSkew float,
					@fSynthKurt float,
					@fSynthYield float,
					@tiSynthIsBasket tinyint

				set @iCAActionID = 3
				set @vcTableName = 'SyntheticOptionRoot'
				set @vcKeyRow = 'syntheticOptRootID'

				if exists ( select * from SyntheticOptionRootView where optionRootID = @iOptionRootID)
				begin
					select 	@iSyntheticOptRootID = syntheticOptRootID, 
						@fSynthSkew = synthSkew,
						@fSynthYield = synthYield,
						@tiSynthIsBasket = synthIsBasket,
						@fOldCashValue = cashValue 
					from SyntheticOptionRootView where optionRootID = @iOptionRootID

					set @iRowID = @iSyntheticOptRootID
					--============================================
					-- execute update of SyntheticOptionRoot
					--============================================
					update SyntheticOptionRoot set actionID = 3 where actionID < 3 and syntheticOptRootID = @iSyntheticOptRootID
						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction CA1 goto finish end
					
	
					select @vcFieldName = 'syntheticOptRootID', @vcFieldValue = @iSyntheticOptRootID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   		if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															
	
					select @vcFieldName = 'optionRootID', @vcFieldValue = @iOptionRootID, @vcFieldType = 'int'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   		if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															
	
					select @vcFieldName = 'synthSkew', @vcFieldValue = @fSynthSkew, @vcFieldType = 'float'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   		if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															
	
					select @vcFieldName = 'synthYield', @vcFieldValue = @fSynthYield, @vcFieldType = 'float'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   		if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															
	
					select @vcFieldName = 'synthIsBasket', @vcFieldValue = @tiSynthIsBasket, @vcFieldType = 'tinyint'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   		if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															
	
					select @vcFieldName = 'cashValue', @vcFieldValue = @fOldCashValue, @vcFieldType = 'float'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
				   		if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															
		
				end
			end

			if @vcSymbol is not null
			begin
				--=========================================================================
				-- Rename all options of current OptionRoot
				--=========================================================================
				set @iCAActionID = 2
				set @vcTableName = 'Contract'
				set @vcKeyRow = 'contractID'
						
				DECLARE contract_cursor CURSOR
				READ_ONLY
				FOR select
					C.contractID,
					C.symbol
					from ContractView C
						join OptionView O on C.contractID = O.optionID
						where O.optionRootID = @iOptionRootID
	
					Open contract_cursor
					fetch next from contract_cursor into 
						@iRowID,
						@vcOldSymbol
						
					while (@@fetch_status <> -1)
				begin
					if (@@fetch_status <> -2)
					BEGIN

					--============================================
					-- execute update of Contract
					--============================================
					update Contract
						set symbol = @vcSymbol + right(symbol,2) 
						where contractID = @iRowID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction CA1 goto finish end
			
					set @vcLogMessage = 'Update ''' + @vcOldSymbol + ''' Option : SET symbol = ''' + @vcSymbol + right(@vcOldSymbol,2) + ''''

					insert into #CAStatusLog(logMessage) values(@vcLogMessage)
						if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction CA1 goto finish end

					-- write into CorporateActionLog
					select @vcFieldName = 'symbol', @vcFieldValue = @vcOldSymbol, @vcFieldType = 'varchar(20)'
					exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA1 goto finish end															

					END
					fetch next from contract_cursor into 
						@iRowID,
						@vcOldSymbol
				end
				Close contract_cursor
				Deallocate contract_cursor
			end

Commit Transaction CA1
set @error = 0

finish:

if @error=0
begin
	select logMessage as vcLogMessage from #CAStatusLog 
end

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















CREATE                  proc dbo.usp_OptionStrikeCA_Save
		@iCorpActionID int,
		@iOptionRootID int,
		@fOldStrike float,
		@vcStrikeTicker varchar(1),
		@fStrike float
as
/*
	Created by: Michael Malyshkin
	Description: this procedure change options strikes and strike tickers
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @vcTableName sysname

	--===============================================
	-- variables for work with CorporateActionLog
	--================================================
	declare @iCAActionID int, --1 for insert, 2 - for update, 3 - for delete
			@vcKeyRow varchar(50),  -- name of field with identity atribute
			@iRowID int,			-- tipically is identity value
			@vcFieldName varchar(50),
			@vcFieldValue varchar(250),
			@vcFieldType varchar(50),
			@vcOldSymbol varchar(20),
			@vcNewSymbol varchar(20),
			@vcLogMessage varchar(250)

	if object_ID('tempdb..#CAStatusLog') is null
		begin	
			create table #CAStatusLog(
						logMessage varchar(250) null)
			if @@error <> 0 begin set @error = 1 goto finish end
		end

	Begin Transaction CA3		

		DECLARE log_cursor CURSOR
		READ_ONLY
		FOR select
			c.contractID,
			c.symbol,
			o.strike
			from ContractView c
			join OptionView o on c.contractID=o.optionID
			where o.optionRootID = @iOptionRootID and o.strike=@fOldStrike

		Open log_cursor
		fetch next from log_cursor into 
			@iRowID,
			@vcOldSymbol,
			@fOldStrike
					
		while (@@fetch_status <> -1)
		begin
			if (@@fetch_status <> -2)
			BEGIN
						
				--======================================================
				-- update Contract table
				--======================================================

				set @vcNewSymbol = left(@vcOldSymbol,len(@vcOldSymbol)-1) + @vcStrikeTicker

				update Contract 
					set symbol = isnull(@vcNewSymbol, symbol)
				where contractID = @iRowID and actionID < 3
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction CA3 goto finish end

				set @vcLogMessage = 'Update ''' + @vcOldSymbol + ''' Option : SET Symbol = ''' + @vcNewSymbol +  ''''

				insert into #CAStatusLog(logMessage) values(@vcLogMessage)
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction CA3 goto finish end

				--======================================================
				-- write into CorporateActionLog update of Contract
				--======================================================
			
				set @iCAActionID = 2
				set @vcKeyRow = 'contractID'
				set @vcTableName = 'Contract'
			
					
				select @vcFieldName = 'symbol', @vcFieldValue = @vcOldSymbol, @vcFieldType = 'varchar(20)'
				exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow 
		   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA3 goto finish end															
			
						
				--======================================================
				-- update Option table
				--======================================================

				update [Option]
					set strike = @fStrike
				where optionID = @iRowID and actionID < 3
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction CA3 goto finish end


				set @vcLogMessage = 'Update ''' + @vcOldSymbol + ''' Option : SET strike = ''' + convert(varchar(25),@fStrike) +  ''''

				insert into #CAStatusLog(logMessage) values(@vcLogMessage)
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction CA3 goto finish end

				--======================================================
				-- write into CorporateActionLog update of Option
				--======================================================
			
				set @iCAActionID = 2
				set @vcKeyRow = 'optionID'
				set @vcTableName = '[Option]'
			
					
				select @vcFieldName = 'strike', @vcFieldValue = @fOldStrike, @vcFieldType = 'float'
				exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow 
					if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA3 goto finish end															
												
			END
			fetch next from log_cursor into 
				@iRowID,
				@vcOldSymbol,
				@fOldStrike
		end
		Close log_cursor
		Deallocate log_cursor

Commit Transaction CA3
set @error = 0

finish:

if @error=0
begin
	select logMessage as vcLogMessage from #CAStatusLog 
end

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

CREATE   proc dbo.usp_Strategy_Del
        @iStrategyID int,
		@tiIsRaiseError tinyint = 1 
  as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	if exists(select * from Trade where strategyID = @iStrategyID)
		begin
			if @tiIsRaiseError = 1
				begin
					Raiserror('Can''t remove strategy, because this strategy has a trades!',16,1)
					Return(0)
				end
			else
				begin
					Return(-1)
				end
		end

	update Strategy set actionID=3 where strategyID = @iStrategyID
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

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



CREATE   proc dbo.usp_TradeByTrader_Get
	@iTraderID int = null
as

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
			1 as iLotSize,					
			tr.isSimulated as tiIsSimulated,
			null as iOptionRootID,
			null as tiIsSyntheticRoot
		from Trade tr
			inner join Contract c on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join Stock s on c.contractID = s.stockID
			left join [Index] i on c.contractID = i.indexID
		where c.contractTypeID < 3
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.isSimulated,cp.priceClose
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
			ort.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated,
			ort.optionRootID as iOptionRootID,
			ort.isSynthetic as tiIsSyntheticRoot
		from Trade tr inner join Contract c on tr.contractID = c.contractID
			inner join [Option] o on c.contractID = o.optionID
			join OptionRoot ort on o.optionRootID = ort.optionRootID
			inner join Contract UC on ort.underlyingID = UC.contractID
			join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join Stock s on uc.contractID = s.stockID
			left join [Index] i on uc.contractID = i.indexID
		where c.contractTypeID = 3
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.isSimulated,
			ort.lotSize,cp.priceClose,ucp.priceClose, ort.optionRootID,ort.isSynthetic
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
SET ANSI_NULLS OFF 
GO



CREATE       PROC dbo.usp_TradeSeq_Get
	@iMinSeqNum int = null,
	@iMaxSeqNum int = null,
	@dtTradeDate datetime = null,
	@tiExtendedInfo tinyint = null,
	@iTraderID int = null
AS
	set nocount on

	declare @dtMaxDate datetime
	if @dtTradeDate is not null set @dtMaxDate = dateadd(d, 1, @dtTradeDate)

	if @iMinSeqNum = 0 set @iMinSeqNum = Null
	if @iMaxSeqNum = 0 set @iMaxSeqNum = Null
	if @tiExtendedInfo is null or @tiExtendedInfo = 0
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.contractID as iContractID,
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
			tr.isPosition as tiIsPosition,
			--tr.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated
		from Trade tr
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where 
			(@iMinSeqNum is null or @iMinSeqNum is not null and seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and cast(floor(cast(tradeDate as float)) as datetime) = @dtTradeDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
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
			tr.isPosition as tiIsPosition,
			1 as iLotSize,
			tr.isSimulated as tiIsSimulated,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			--case when exists(select * from TradeLog where contractID = tr.contractID) then 1 else 0 end iStructureMissed,
			case when exists(select * from TradeLog where contractID = tr.contractID and seqNum = tr.seqNum) then 1 else 0 end iStructureMissed,
			null as iOptionRootID,
			null as tiIsSyntheticRoot
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID=cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where c.contractTypeID < 3
			and (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and tr.tradeDate >= @dtTradeDate and tr.tradeDate < @dtMaxDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
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
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceTheo as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			ORt.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose,
			case 
			    when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum)
					and exists(select * from TradeLog where contractID = O.optionID and seqNum = tr.seqNum) then 3
				when exists(select * from TradeLog where contractID = o.optionID and seqNum = tr.seqNum) then 2
				when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum) then 1
            else 0 	
			end iStructureMissed,
			ORT.optionRootID as iOptionRootID,
			ORT.isSynthetic as tiIsSyntheticRoot
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join [OptionView] o on c.contractID = o.optionID
			inner join OptionRootView ORt on ORt.optionRootID = o.optionRootID
			inner join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView EC on o.expiryID = EC.expiryID and EC.expCalendarID = uc.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where c.contractTypeID = 3
			and (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and tr.tradeDate >= @dtTradeDate and tr.tradeDate < @dtMaxDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		order by tr.seqNum



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    Procedure dbo.usp_TraderContract_Del 
	@iContractID int = null,
	@iTraderID int = null,
	@tiIsRaiseError tinyint = 1 
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from TraderContract table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	if exists(select * from Trade 
		where (traderID = @iTraderID or @iTraderID is null) and (contractID = @iContractID or @iContractID is null))
		begin
			if @tiIsRaiseError = 1
				begin
					Raiserror('Can''t remove trader contract, because he related with active trades!',16,1)
					Return(0)
				end
			else
				begin
					Return(-1)
				end
		end

	if 	@iTraderID is not null and @iContractID is not null
		update TraderContract set actionID=3
			where traderID = @iTraderID  and ContractID=@iContractID
	else if @iTraderID is not null and @iContractID is null
		update TraderContract set actionID=3
			where traderID = @iTraderID
	else if @iTraderID is null and @iContractID is not null
		update TraderContract set actionID=3
			where ContractID=@iContractID
	else if @iTraderID is null and @iContractID is null
		update TraderContract set actionID=3


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


CREATE      proc dbo.usp_Trader_Del
        @iTraderID int,
		@tiIsRaiseError tinyint = 1 
  as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	if exists(select * from Trade where traderID = @iTraderID)
		begin
			if @tiIsRaiseError = 1
				begin
					Raiserror('Can''t remove trader, because this trader have a trades!',16,1)
					Return(0)
				end
			else
				begin
					Return(-1)
				end
		end
	
	Begin Transaction
        update Trader set actionID=3 where traderRefID = @iTraderID		
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

        update Trader set actionID=3 where traderID = @iTraderID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		set @error = @@error
	Commit Transaction

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



CREATE    procedure dbo.usp_UnderlyingExpiryByList_Get
	@dtMinExpiry datetime = null,
	@vcUnderlyingID1 varchar(8000) = null,
	@vcUnderlyingID2 varchar(8000) = null
AS
	set nocount on

	declare @vcString1 as varchar(8000),
			@vcString2 as varchar(8000),
			@vcString3 as varchar(8000),
			@vcString4 as varchar(8000)

	select @vcString1 = '
		select
			c.contractID as iUnderlyingID,
			ec.expiryID as iExpiryID,
			ec.expiryDate as dtExpiryDate
		from ContractView c
			join OptionRootView r on c.contractID = r.underlyingID
			join OptionView o on r.optionRootID = o.optionRootID
			join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = c.expCalendarID '

	if @dtMinExpiry is not null
		set @vcString3 = 'where ec.expiryDate >=' + '''' + convert(varchar(12),@dtMinExpiry,112) + '''' 
	else 	
		set @vcString3 = 'where'

	if @vcString2 is null set @vcString2 = ''

	set @vcString3 = @vcString3 + ' and c.contractID in ('
	set @vcString4 = 'group by c.contractID, ec.expiryID, ec.expiryDate order by expiryDate'

	if isnull(@vcUnderlyingID1,'') != '' and isnull(@vcUnderlyingID2,'') = ''
		begin
			exec(@vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ') ' + @vcString4)
			--select @vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ') ' + @vcString4
		end
	else if isnull(@vcUnderlyingID1,'') != '' and isnull(@vcUnderlyingID2,'') != ''
		begin
			exec(@vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ',' + @vcUnderlyingID2 + ') ' + @vcString4)
			--select @vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ',' + @vcUnderlyingID2 + ') ' + @vcString4
		end			

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



CREATE   procedure dbo.usp_UnderlyingExpiry_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@dtMinExpiry datetime = null
AS
	set nocount on


	if isnull(@iGroupID, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
				c.contractID as iUnderlyingID,
				ec.expiryID as iExpiryID,
				ec.expiryDate as dtExpiryDate
			from ContractView c
				inner join OptionRootView r on c.contractID = r.underlyingID
				inner join OptionView o on r.optionRootID = o.optionRootID
				inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = c.expCalendarID
			where 
				@iContractID is null or c.contractID = @iContractID
				and (@dtMinExpiry is null or ec.expiryDate >= @dtMinExpiry)
			group by
				c.contractID, ec.expiryID, ec.expiryDate
			order by expiryDate
		else
			select
				c.contractID as iUnderlyingID,
				ec.expiryID as iExpiryID,
				ec.expiryDate as dtExpiryDate
			from TraderContractView tc 
			    inner join ContractView c on tc.contractID = c.contractID
				inner join OptionRootView r on c.contractID = r.underlyingID
				inner join OptionView o on r.optionRootID = o.optionRootID
				inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = c.expCalendarID
			where 
				tc.traderID = @iTraderID
				and @iContractID is null or c.contractID = @iContractID
				and (@dtMinExpiry is null or ec.expiryDate >= @dtMinExpiry)
			group by
				c.contractID, ec.expiryID, ec.expiryDate
			order by expiryDate
	end
	else
	begin
		if isnull(@iTraderID, 0) = 0
			select
				c.contractID as iUnderlyingID,
				ec.expiryID as iExpiryID,
				ec.expiryDate as dtExpiryDate
			from ContractInGroupView cig 
				inner join ContractView c on c.contractID = cig.contractID
				inner join OptionRootView r on c.contractID = r.underlyingID
				inner join OptionView o on r.optionRootID = o.optionRootID
				inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = c.expCalendarID
			where 
				cig.groupID = @iGroupID
				and (@dtMinExpiry is null or ec.expiryDate >= @dtMinExpiry)
			group by
				c.contractID, ec.expiryID, ec.expiryDate
			order by expiryDate
		else
			select
				c.contractID as iUnderlyingID,
				ec.expiryID as iExpiryID,
				ec.expiryDate as dtExpiryDate
			from ContractInGroupView cig 
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
				inner join OptionRootView r on c.contractID = r.underlyingID
				inner join OptionView o on r.optionRootID = o.optionRootID
				inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = c.expCalendarID
			where 
				cig.groupID = @iGroupID
				and tc.traderID = @iTraderID
				and (@dtMinExpiry is null or ec.expiryDate >= @dtMinExpiry)
			group by
				c.contractID, ec.expiryID, ec.expiryDate
			order by expiryDate
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



CREATE    proc dbo.usp_UnderlyingOptionByExpiry_Get
	@iUnderlyingID int,		 
	@iExpiryID int
AS

	set nocount on

	select
		o.optionID as iContractID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		o.isCall as tiIsCall,
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,
		ort.lotSize as iLotSize,
		cp.priceClose as fPriceClose,
		cp.priceTheo as fPriceTheoClose
	from OptionRootView ort
		inner join OptionView o on ort.optionRootID = o.optionRootID
		inner join ContractView c on o.optionID = c.contractID
	    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
	where
		ort.underlyingID = @iUnderlyingID
		and o.expiryID = @iExpiryID



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

Create procedure dbo.usp_UnderlyingPosByList_Get
	@iTraderID int = null,
	@tiWithSimulated tinyint = 0,
	@vcUnderlyingID1 varchar(8000),
	@vcUnderlyingID2 varchar(8000) = null
AS
	set nocount on

	declare @vcString1 as varchar(8000),
			@vcString2 as varchar(8000),
			@vcString3 as varchar(8000),
			@vcString4 as varchar(8000)

	if isnull(@iTraderID, 0) = 0
		begin
			set @vcString1 = 'select 
						c.contractID iContractID,
						sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) iQty,
						sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) fPosition
						from TradeView tr 
							join ContractView c on tr.contractID=c.contractID '
	
			if isnull(@tiWithSimulated,0)= 0 
				begin
					select @vcString1 = @vcString1 + ' and isnull(tr.isSimulated, 0)=0 '
				end
	
	        set @vcString2 = 'where c.contractTypeID < 3 '
			set @vcString3 = 'and c.contractID in (' 
			set @vcString4 = 'group by c.contractID'
		end
	else
		begin
			set @vcString1 = 'select
				c.contractID iContractID,
				sum(tr.quantity * (case when tr.isBuy!=0 then 1 else -1 end)) iQty,
				sum(tr.quantity * tr.price * (case when tr.isBuy!=0 then 1 else -1 end)) fPosition
					from ContractView c
			    		join TraderContractView tc on c.contractID=tc.contractID
						join TradeView tr on tr.contractID=c.contractID ' 

			set @vcString2 = 'and tr.traderID=' + convert(varchar(25),@iTraderID) + ' ' 

			if isnull(@tiWithSimulated,0)= 0 
				begin
					select @vcString2 = isnull(@vcString2,'') + ' and isnull(tr.isSimulated, 0)=0 '
				end

			set @vcString3 = 'where c.contractTypeID < 3 and tc.traderID=' + convert(varchar(25),@iTraderID) + ' and c.contractID in ('
			set @vcString4 = 'group by c.contractID'
		end

		if isnull(@vcUnderlyingID1,'') != '' and isnull(@vcUnderlyingID2,'') = ''
			begin
				exec(@vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ') ' + @vcString4)
				--select @vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ') ' + @vcString4
			end
		else if isnull(@vcUnderlyingID1,'') != '' and isnull(@vcUnderlyingID2,'') != ''
			begin
				exec(@vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ',' + @vcUnderlyingID2 + ') ' + @vcString4)
				--select @vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ',' + @vcUnderlyingID2 + ') ' + @vcString4
			end			

	Return (@@error)
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.39.049')
	begin
		update DataInfo set keyValue = '3.39.049' where dataInfoID = 1
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
	PRINT 'DB VERSION WAS CHANGED TO 3.39.049'
	COMMIT TRANSACTION
END
	ELSE PRINT 'The database update failed'
GO
DROP TABLE #tmpErrors
GO