/*
fix for MLine00000930
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.38.045' or (keyValue = '3.38.046')))
	BEGIN
		PRINT 'You can''t run this script, because you have not run script for previous version!'
		Select cast(keyValue as varchar(25)) as 'DB version', '3.38.046' as  'Version in script' from DataInfo where dataInfoID = 1
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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Clear]
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE  PROCEDURE dbo.usp_Trade_Clear
	@iContractID int = null,
	@tiOnlyWithExecID tinyint = null,
	@lastUTCTradeDate datetime = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction

		delete from TradeLog 
		where seqNum in (select seqNum from Trade where isSimulated = 1 and actionID = 0
									 and (@iContractID is null or contractID = @iContractID))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

		delete from Trade 
		where tradeID in (select tradeID from Trade where isSimulated = 1 and actionID = 0) 
							and (@iContractID is null or contractID = @iContractID)
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
	
		if isnull(@tiOnlyWithExecID, 0) = 0
		begin
			delete from TradeLog 
				where seqNum in (select seqNum from Trade
									where (@iContractID is null or contractID = @iContractID) 
										and isnull(isSimulated, 0) = 0
										and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end	

			delete from Trade 
				where (@iContractID is null or contractID = @iContractID) 
					and isnull(isSimulated, 0) = 0
					and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0)
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
		end
		else
		begin
			delete from TradeLog 
				where seqNum in (select seqNum from Trade
									where (@iContractID is null or contractID = @iContractID) 
										and isnull(isSimulated, 0) = 0 
										and isnull(execID, '') <> ''
										and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

			delete from Trade 
			where (@iContractID is null or contractID = @iContractID) 
				and isnull(isSimulated, 0) = 0 
				and isnull(execID, '') <> ''
				and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
		end
		set @error = @@error
	Commit Transaction

finish:
	return @error


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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.38.046')
	begin
		update DataInfo set keyValue = '3.38.046' where dataInfoID = 1
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
	PRINT 'DB VERSION WAS CHANGED TO 3.38.046'
	COMMIT TRANSACTION
END
	ELSE PRINT 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
