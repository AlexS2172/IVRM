/*
fix for defect  MLine00000844 Connectivity
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.37.035' or (keyValue = '3.37.036')))
	BEGIN
		PRINT 'You can''t run this script, because you have not run script for previous version!'
		Select cast(keyValue as varchar(25)) as 'DB version', '3.37.036' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
/*DATA MODIFICATION SECTION*/
if exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.37.035' or (keyValue = '3.37.036')))
	BEGIN
		if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_1_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
		drop procedure [dbo].[usp_TradeFeed_1_Del]
	END
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
CREATE      procedure dbo.usp_TradeFeed_1_Del
	@vcExecID varchar(20),
	@dtTradeDate datetime,
	@tiIsUpdateEnabled tinyint = 0,
	@iLastTradeTime int = null
/*
	Created by Zaur Nuraliev
	Description: this procedure is mark as delete information in Trade table
*/
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iSeqNum int,
			@iNewSeqNum int,
			@iRemovedSeqNum int

	--select SeqNum for remove
	select 	@iSeqNum = max(seqNum)
		from Trade 
		where execID = @vcExecID 
			and actionID > 0 
			and Convert(varchar(8),@dtTradeDate,112) = Convert(varchar(8),tradeDate,112)

	select @iRemovedSeqNum = max(seqNum) from Trade 
		where execID = @vcExecID and Convert(varchar(8),@dtTradeDate,112) = Convert(varchar(8),tradeDate,112) and actionID = 0

	--trade, which we plan to remove does not exist in DB
	if not exists(select * from Trade where execID = @vcExecID and Convert(varchar(8),@dtTradeDate,112) = Convert(varchar(8),tradeDate,112)) 
		begin
			if @tiIsUpdateEnabled = 0 
				select -1 as iMaxSeqNum, 0 as iMinSeqNum
			Return(-1)
		end

	--trade has been removed
	--pair added-removed exists & not exists active trade with same execID & TradeDate 
	if (select count(*)
				from Trade 
				where execID = @vcExecID and --there was mistake! I used constant tradeDate!
					convert(varchar(8),@dtTradeDate,112) = Convert(varchar(8),tradeDate,112)) % 2 = 0
		begin
			-- trade already has been deleted
			if @tiIsUpdateEnabled = 0 
				select @iRemovedSeqNum as iMaxSeqNum, -1 as iMinSeqNum
			Return(-1)
		end

	select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade

	Begin Transaction trade
		insert into Trade(tradeID,
			actionID,
			contractID,
			isClose,
			tradeDate,
			quantity,
			price,
			isBuy,
			bookID,
			strategyID,
			traderID,
			brokerID,
			clearingBrokerID,
			brokerCommissionID,
			clearingBrokerCommissionID,
			tradedIV,
			spotReference,
			isPosition,
			lotSize,
			isSimulated,
			execID,
			seqNum)
		select tradeID,
			0,
			contractID,
			isClose,
			tradeDate,
			quantity,
			price,
			isBuy,
			bookID,
			strategyID,
			traderID,
			brokerID,
			clearingBrokerID,
			brokerCommissionID,
			clearingBrokerCommissionID,
			tradedIV,
			spotReference,
			isPosition,
			lotSize,
			isSimulated,
			execID,
			@iNewSeqNum
		from Trade
		where seqNum = @iSeqNum
				
		set @error = @@error

		--trade has been successfully removed from DB
		if @tiIsUpdateEnabled = 0 
			select @iSeqNum as iMinSeqNum,@iNewSeqNum as iMaxSeqNum
		if @@error <> 0 begin set @error = 2 if @@TranCount > 0  Rollback Transaction trade goto finish end

		--save the last successful trade time
		if @iLastTradeTime is not null
			begin
				update DataInfo set keyValue = @iLastTradeTime where keyName='lastTradeTime'
					if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			end

	Commit Transaction trade

	set @error = 0
Return(0)

finish:
	if @tiIsUpdateEnabled = 0 
		select -1 as iMinSeqNum, -1 as iMaxSeqNum
	Return(@error)
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.37.036')
	begin
		update DataInfo set keyValue = '3.37.036' where dataInfoID = 1
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
	PRINT 'DB VERSION WAS CHANGED TO 3.37.036'
	COMMIT TRANSACTION
END
	ELSE PRINT 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
