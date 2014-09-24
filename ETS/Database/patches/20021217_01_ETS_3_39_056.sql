/*
fix for MLine00001083
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.39.055' or (keyValue = '3.39.056')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.39.056 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.39.056' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER     proc dbo.usp_TradeCA_Save
		@iCorpActionID int,
        @iTradeID int = null, --for new trade is null
		@iQuantity int, 
		@fPrice float, 
		@iContractID int = null, 
		@tiIsClose tinyint = 0, 
		@tiIsBuy tinyint = 0, 
		@iTraderID int = null, 
		@tiIsPosition tinyint = 0,
		@tiIsSimulated tinyint = 0
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction TU
		--delete of trade, which we plan to update
		if (select count(*) from Trade where tradeID = @iTradeID)!=2 and @iTradeID is not null
			begin
				exec @error = usp_TradeCA_Del @iCorpActionID,@iTradeID
					if @@error <> 0 begin Rollback Transaction TU Return(0) end
			end

		Declare @iNewTradeID int
			select @iNewTradeID = isnull(max(tradeID),0) + 1 from Trade
	
		Declare @iNewSeqNum int 
			select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade
	
		--===============================================
		-- variables for work with CorporateActionLog
		--================================================
		declare @vcTableName sysname,
				@iCActionID int, --1 for insert, 2 - for update, 3 - for delete
				@vcKeyRow varchar(50),  -- name of field with identity atribute
				@iRowID int,			-- tipically is identity value
				@vcFieldName varchar(50),
				@vcFieldValue varchar(250),
				@vcFieldType varchar(50)

		select @iCActionID = 1, @vcTableName = 'Trade', @vcKeyRow = 'seqNum'


		--update existing trade
		if @iTradeID is not null
		begin
			insert into Trade(tradeID,
				actionID,
				contractID,
				isClose,
				quantity,
				price,
				isBuy,
				traderID,
				isPosition,
				isSimulated,
				SeqNum,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradeDate)
			select @iNewTradeID,
				1,
				contractID,
				isClose,
				@iQuantity,
				@fPrice,
				isBuy,
				traderID,
				isPosition,
				isSimulated,
				@iNewSeqNum,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradeDate
			from Trade
			where tradeID = @iTradeID and actionID=1

			set @error = @@error
				if @error != 0 begin Rollback Transaction TU Return(-1) end
		end
	else
		begin
			--create the tradeDate
			declare @iDeltaGMT int,
					@dtNewTradeDate datetime
			exec master.dbo.xp_regread 'HKEY_LOCAL_MACHINE', 'SYSTEM\CurrentControlSet\Control\TimeZoneInformation', 'Bias', @iDeltaGMT OUT
			set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())

			-- insert new trade
			insert into Trade(
				tradeID,
				actionID,
				contractID,
				isClose,
				quantity,
				price,
				isBuy,
				traderID,
				isPosition,
				isSimulated,
				seqNum,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradeDate)
			select 
				@iNewTradeID,
				1,
				@iContractID,
				@tiIsClose,
				@iQuantity,
				@fPrice,
				@tiIsBuy,
				@iTraderID,
				@tiIsPosition,
				@tiIsSimulated,
				@iNewSeqNum,
				Null,
				Null,
				@dtNewTradeDate

			set @error = @@error
				if @error != 0 begin Rollback Transaction TU Return(-1) end
		end
	
		--write into CA log
		select 	@vcFieldName = 'seqNum', 
				@vcFieldValue = @iNewSeqNum,
				@vcFieldType = 'int',
				@iRowID = @iNewSeqNum
		exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCActionID,@iRowID,@vcKeyRow

		if @@error = 0 
			begin 
				Commit Transaction TU 
				Return @iNewSeqNum 
			end 
		else 
			begin 
				Rollback Transaction TU 
				Return(-1) 
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
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.39.056')
	begin
		update DataInfo set keyValue = '3.39.056' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.39.056' '3.39.056 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO