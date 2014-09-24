/*
MLine00001629 fix
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.43.084' or (keyValue = '3.43.085')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.43.085 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.43.085' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_OptionsForReconcile_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_OptionsForReconcile_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_PositionForReconcile_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_PositionForReconcile_Get
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE   proc dbo.usp_OptionsForReconcile_Get
	@iUnderlyingID int		 
  as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

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
		ORt.symbol as vcOptionRootSymbol,
		O.isManualyEntered as tiIsManualyEntered 
	from OptionView O  
		left join ContractView C on O.optionID = C.contractID
		left join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
		left join OptionRoot ORt on O.optionRootID = ORt.optionRootID
		left join Contract C1 on ORt.underlyingID = C1.contractID
		left join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
	where c.contractTypeID=3
		and ORt.underlyingID = @iUnderlyingID

	set @error = @@error
Return(@error)

GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE       PROCEDURE dbo.usp_PositionForReconcile_Get
	@tiByTrader tinyint = 0,
	@lastUTCTradeDate datetime = null
AS

	if @lastUTCTradeDate is null set @lastUTCTradeDate = getdate()

	if isnull(@tiByTrader, 0) = 0
	begin
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as iOptionRootID,
			null as vcOptionRootSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			1 as iLotSize,
			cp.priceClose as fPriceClose,

			null as iTraderID,
			null as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty
		
		from TradeView tr 
			left join ContractView c  on tr.contractID = c.contractID
			left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where c.contractTypeID < 3
			and tr.status in (0, 3)
			and datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0
		group by c.contractID, c.contractTypeID, c.Symbol, cp.priceClose
		
		union
	
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			ORt.optionRootID as iOptionRootID,
			ORt.symbol as vcOptionRootSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,

			ORt.lotSize as iLotSize,
			cp.priceClose as fPriceClose,

			null as iTraderID,
			null as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty

		from TradeView tr 
			left join ContractView C on tr.contractID = C.contractID
			left join OptionView o on c.contractID = o.optionID
			left join OptionRootView ORt on o.optionRootID = ORt.optionRootID
			left join ContractView UC on ORt.underlyingID = UC.contractID
			left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
	
		where c.contractTypeID = 3
			and tr.status in (0, 3)
			and datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0
		group by C.contractID, C.contractTypeID, C.Symbol,ORt.optionRootID,ORt.symbol,
			EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
			ORt.lotSize, cp.priceClose
	
		order by vcSymbol
	end
	else
	begin
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as iOptionRootID,
			null as vcOptionRootSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			1 as iLotSize,
			cp.priceClose as fPriceClose,
			trdr.TraderID as iTraderID,
			trdr.acronym as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty
		from TradeView tr 
			left join ContractView c  on tr.contractID = c.contractID
			left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join TraderView trdr on tr.traderID = trdr.traderID
		where c.contractTypeID < 3
			and tr.status in (0, 3)
			and datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0
		group by c.contractID, c.contractTypeID, c.Symbol,cp.priceClose, trdr.TraderID, trdr.acronym
		
		union
	
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			ORt.optionRootID as iOptionRootID,
			ORt.symbol as vcOptionRootSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			ORt.lotSize as iLotSize,
			cp.priceClose as fPriceClose,
			trdr.TraderID as iTraderID,
			trdr.acronym as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty

		from TradeView tr 
			left join ContractView C on tr.contractID = C.contractID
			left join OptionView o on c.contractID = o.optionID
			left join OptionRootView ORt on o.optionRootID = ORt.optionRootID
			left join ContractView UC on ORt.underlyingID = UC.contractID
			left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			left join TraderView trdr on tr.traderID = trdr.traderID
	
		where c.contractTypeID = 3
			and tr.status in (0, 3)
			and datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0
		group by C.contractID, C.contractTypeID, C.Symbol,ORt.optionRootID,ORt.symbol,
			EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
			ORt.lotSize, cp.priceClose, trdr.TraderID, trdr.acronym
	
		order by vcSymbol
	end

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
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.43.085')
	begin
		update DataInfo set keyValue = '3.43.085' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.43.085' '3.43.085 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
