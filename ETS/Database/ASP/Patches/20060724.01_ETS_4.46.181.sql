SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.180'  or (keyValue = '3.46.181') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.181 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.181' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO



GO
IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='Trade' AND COLUMN_NAME='orderID')
ALTER TABLE Trade
	ADD orderID INT NULL
GO
IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='Trade' AND COLUMN_NAME='execStatus')
ALTER TABLE Trade
	ADD execStatus INT NULL
GO
ALTER TABLE Trade
	ALTER COLUMN execID nvarchar(128) NULL
GO

ALTER  VIEW dbo.TradeView
AS
	select
	 	tradeID, contractID, tradeDate, quantity, price, isBuy, 
		traderID, strategyID, brokerID, brokerCommission, 
		clearingBrokerID, clearingBrokerCommission, tradedIV, 
		spotReference, isPosition, status, execID, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
	from 
		dbo.Trade
	group by
	 	tradeID, contractID, tradeDate, quantity, price, isBuy, 
		traderID, strategyID, brokerID, brokerCommission, 
		clearingBrokerID, clearingBrokerCommission, tradedIV, 
		spotReference, isPosition, status, execID, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
	having
		(min(actionID) = 1)

GO

ALTER  PROC dbo.usp_Trade_New
	@iContractID int,
	@iQuantity int,
	@fPrice float,
	@tiIsBuy tinyint,  
	@iTraderID int,
	@iStrategyID int = null,
	@iBrokerID int = Null,
	@fBrokerCommission float =Null,
	@iClearingBrokerID int = Null,
	@fClearingBrokerCommission float =Null,
	@fTradedIV float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime = Null out,
	@tiStatus tinyint,
	@iMark int = null,
	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null,
	@vcImportID varchar(20) = null out,
	@iOrderID int = null,
	@iExecStatus int = null
as
	set nocount on

	declare @error int		set @error = 0

	-- check for valid contract parameters
	exec @error = usp_ContractParameters_CheckValidity @iContractID, @iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID
	if @error <> 0 goto finish

	begin tran

	if not exists(select top 1 1 from TraderContractView where traderID = @iTraderID and contractID = @iUnderlyingID)
	begin
		insert into TraderContract (traderID, contractID) values (@iTraderID, @iUnderlyingID)
			if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	end

	if @dtNewTradeDate is null	
	begin
		declare @iDeltaGMT int

		exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

		set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	end

	-- get new tradeID ane seqNum
	declare @iNewSeqNum int

	select 
		@iNewTradeID = isnull(max(tradeID), 0) + 1,
		@iNewSeqNum = isnull(max(seqNum), 0) + 1
	from Trade

	-- create new trade
	insert into Trade (tradeID, actionID, contractID, tradeDate,  quantity, price, traderID, strategyID,
		isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
	values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, @iStrategyID, @tiIsBuy,
		@iBrokerID, @fBrokerCommission, @iClearingBrokerID, @fClearingBrokerCommission,
		@fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, @iNewSeqNum, @iMark,
		@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID, @iOrderID, @iExecStatus)

	if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

	select @vcImportID = exportSymbol from ContractView where contractID = @iContractID

	if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

	commit tran

finish:
	if @error = 0
		return @iNewSeqNum
	else  
	begin
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		set @vcImportID = null
		return @error
	end

GO

ALTER   PROC dbo.usp_Trade_Update
	@iOldTradeID int,
	@iContractID int,
	@iQuantity int,
	@fPrice float,
	@tiIsBuy tinyint,
	@iTraderID int,
	@iStrategyID int,
	@iBrokerID int = Null,
	@fBrokerCommission float =Null,
	@iClearingBrokerID int = Null,
	@fClearingBrokerCommission float =Null,
	@fTradedIV  float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime out,
	@tiStatus tinyint,
	@iMark int = null,
	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null,
	@vcImportID varchar(20) = Null out,
	@iOrderID int = null,
	@iExecStatus int = null
as
	set nocount on

	declare @error int		set @error = 0

	declare @iNewSeqNum int,
			@vcExecID varchar(20)

	if @iOldTradeID is not null
	begin
		begin tran

		declare @nCount int
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iOldTradeID
		
		if @nCount = 1
			begin
				select @iNewSeqNum = isnull(max(seqNum), 0) + 1 from Trade
				select @vcExecID = execID from Trade where tradeID = @iOldTradeID
	
				insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
					strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
					tradedIV, spotReference, isPosition, status, execID, seqNum, mark,
					underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
				select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
					strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
					tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark,
					underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
				from Trade
				where tradeID = @iOldTradeID

				if @@error <> 0 or @error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				-- Does not find what we should update
				set @error = -5
				if @@TranCount = 1 Rollback Transaction
				goto finish
			end

		if not exists(select top 1 1 from TraderContractView where traderID = @iTraderID and contractID = @iUnderlyingID)
		begin
			insert into TraderContract (traderID, contractID) values (@iTraderID, @iUnderlyingID)
				if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction goto finish end
		end
	
		if @dtNewTradeDate is null	
		begin
			declare @iDeltaGMT int

			exec @iDeltaGMT = usp_TimeZoneActimeBias_Get
	
			set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
		end

		select 
			@iNewTradeID = isnull(max(tradeID), 0) + 1,
			@iNewSeqNum = isnull(max(seqNum), 0) + 1
		from Trade
	
		insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, 
			strategyID, isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
			tradedIV, spotReference, isPosition, status, seqNum, execID, mark,
			underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
		values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, 
			@iStrategyID,  @tiIsBuy, @iBrokerID, @fBrokerCommission, @iClearingBrokerID, 
			@fClearingBrokerCommission, @fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, 
			@iNewSeqNum, @vcExecID, @iMark,
			@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID, @iOrderID, @iExecStatus)

		if @@error <> 0 or @error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

		select @vcImportID = exportSymbol from ContractView where contractID = @iContractID
	
		if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

		commit tran
	end
	else
		set @error = -4

finish:
	if @error = 0
		return @iNewSeqNum
	else  
	begin
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		set @vcImportID = null
		return @error
	end
GO

ALTER  PROC dbo.usp_Trade_Del
    @iTradeID int
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	if @iTradeID is not null
	begin
		begin tran

		declare @nCount int,
				@iNewSeqNum int

		select @iNewSeqNum = isnull(max(seqNum), 0) + 1 from Trade
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iTradeID

		if @nCount = 1
		begin
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
			from Trade
			where tradeID = @iTradeID
			if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

		end
		else
			set @error = -2

		commit tran
	end
	else
		set @error = -3

finish:
	if @error = 0
		return @iNewSeqNum
	else
		return @error

GO

ALTER  PROC dbo.usp_TradeCA_Save
	@iCorpActionID int,
	@iTradeID int = null, --for new trade is null
	@iQuantity int, 
	@fPrice float, 
	@iContractID int = null, 
	@tiIsBuy tinyint = 0, 
	@iTraderID int = null, 
	@tiIsPosition tinyint = 0,
	@tiStatus tinyint = 0,
	@iMark int = null,
	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null,
	@iOrderID int = null,
	@iExecStatus int = null
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
		insert into Trade(tradeID, actionID, contractID, quantity, price,
			isBuy, traderID, strategyID, isPosition, status, seqNum, brokerID, brokerCommission,
			clearingBrokerID, clearingBrokerCommission, tradeDate, mark,
			underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
		select @iNewTradeID, 1, contractID, @iQuantity, @fPrice,
			isBuy, traderID, strategyID, isPosition, status, @iNewSeqNum, brokerID, brokerCommission,
			clearingBrokerID, clearingBrokerCommission, tradeDate, mark,
			underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
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

		exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

		set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())

		-- determine strategy for trade
		declare @iStrategyID int
		exec @error = usp_TraderContractStrategyWithSave_Get @iTraderID, @iContractID, @iStrategyID out
		if @error != 0 begin Rollback Transaction TU Return(-1) end

		-- insert new trade
		insert into Trade(tradeID, actionID, contractID, quantity, price,
			isBuy, traderID, strategyID, isPosition, status, seqNum, brokerID, brokerCommission,
			clearingBrokerID, clearingBrokerCommission, tradeDate, mark,
			underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
		values 
			(@iNewTradeID, 1, @iContractID, @iQuantity, @fPrice, @tiIsBuy, @iTraderID,
			@iStrategyID, @tiIsPosition, @tiStatus, @iNewSeqNum, null, null, null, null, @dtNewTradeDate, @iMark,
			@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID, @iOrderID, @iExecStatus)

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

ALTER  PROC dbo.usp_TradeCA_Del
	@iCorpActionID int,
	@iTradeID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iTradeID is null Return(-1)	

	Begin Transaction TD

	declare @nCount int,
			@iNewSeqNum int

	select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade
	
	set @nCount = 0
	select @nCount = count(tradeID) from Trade where tradeID = @iTradeID

	if @nCount != 1	begin Rollback Tran TD Return(-1) end

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

	set @iCActionID = 1
	set @vcTableName = 'Trade'
	set @vcKeyRow = 'seqNum'

	insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
		strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, execID, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
	select 
		tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
		strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
	from Trade
	where tradeID = @iTradeID

	set @error = @@error
		if @error <> 0 begin Rollback Transaction TD Return(0) end

	--write into CA log
	select 	@vcFieldName = 'seqNum', 
			@vcFieldValue = @iNewSeqNum,
			@vcFieldType = 'int',
			@iRowID = @iNewSeqNum
	exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCActionID,@iRowID,@vcKeyRow

	if @@error = 0 
		begin 
			Commit Transaction TD 
			Return @iNewSeqNum 
		end 
	else 
		begin 
			Rollback Transaction TD 
			Return(0) 
		end

GO

ALTER      PROC dbo.usp_Trade_Exec
	@tiExercise tinyint,
	@iOptTradeID int,
	@iOptQuantity int,
	@tiOptIsBuy tinyint,
	@tiOptStatus tinyint,
	@fOptBrokerCommission float,
	@fOptClearingBrokerCommission float,
	@iUndQuantity int,
	@fUndPrice float,
	@tiUndIsBuy tinyint,
	@tiUndStatus tinyint,
	@fUndBrokerCommission float,
	@fUndClearingBrokerCommission float,
	@tiIsFutures tinyint,
	@priceForIndexAndExer float ,
	@tiIsIndex tinyint = 1 ,  
	@iNewOptSeqNum int = null out,
	@iNewOptTradeID int = null out,
	@dtNewOptTradeDate datetime = null out,
	@iNewUndSeqNum int = null out,
	@iNewUndTradeID int = null out,
	@dtNewUndTradeDate datetime = null out
as
	set nocount on
-- A Sh 02 - 09 - 2005	
	set transaction isolation level serializable

	begin tran

	declare @bSucceed bit
		
	set @bSucceed = 1

	declare @iDeltaGMT int

	exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

	select @iNewOptTradeID = max(tradeID) + 1 from Trade
	if @iNewOptTradeID is null set @iNewOptTradeID = 1

	select @iNewOptSeqNum = isnull(max(seqNum), 0) + 1 from Trade

	if @dtNewOptTradeDate is null	
		set @dtNewOptTradeDate = dateadd(minute, @iDeltaGMT, getdate())

	insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
		price, isBuy, traderID, strategyID, brokerID, brokerCommission,
		clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
		isPosition, status, execID, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
	select @iNewOptTradeID, 1, contractID, @dtNewOptTradeDate, @iOptQuantity, @priceForIndexAndExer , @tiOptIsBuy,
		traderID, strategyID, brokerID, @fOptBrokerCommission, clearingBrokerID, @fOptClearingBrokerCommission,
		tradedIV, spotReference, 0, @tiOptStatus, null, @iNewOptSeqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
	from Trade
	where tradeID = @iOptTradeID and actionID = 1

	if @@error <> 0 set @bSucceed = 0

	if @bSucceed = 1 and isnull(@tiExercise, 0) <> 0 and @tiIsIndex <> 1
	begin
		select @iNewUndTradeID = max(tradeID) + 1 from Trade
			if @iNewUndTradeID is null set @iNewUndTradeID = 1
	
		select @iNewUndSeqNum = isnull(max(seqNum), 0) + 1 from Trade
	
		if @dtNewUndTradeDate is null	
			set @dtNewUndTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	
		if isnull(@tiIsFutures, 0) = 0
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select @iNewUndTradeID, 1, underlyingID, @dtNewUndTradeDate, @iUndQuantity, @fUndPrice, @tiUndIsBuy,
				traderID, strategyID, brokerID, @fUndBrokerCommission, clearingBrokerID, @fUndClearingBrokerCommission,
				0, 0, 0, @tiUndStatus, null, @iNewUndSeqNum, mark,
				underlyingID, null, null, null, orderID, execStatus
			from Trade
			where tradeID = @iOptTradeID and actionID = 1
		else
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select @iNewUndTradeID, 1, futureID, @dtNewUndTradeDate, @iUndQuantity, @fUndPrice, @tiUndIsBuy,
				traderID, strategyID, brokerID, @fUndBrokerCommission, clearingBrokerID, @fUndClearingBrokerCommission,
				0, 0, 0, @tiUndStatus, null, @iNewUndSeqNum, mark,
				underlyingID, null, futureRootID, futureID, orderID, execStatus
			from Trade
			where tradeID = @iOptTradeID and actionID = 1
	
		if @@error <> 0 set @bSucceed = 0
	end

	if @bSucceed = 1
	begin
		commit tran
		if isnull(@tiExercise, 0) <> 0
			return @iNewUndSeqNum
		else
			return @iNewOptSeqNum
	end
	else  
	begin
		rollback tran
		set @iNewOptSeqNum = null
		set @iNewOptTradeID = null
		set @dtNewOptTradeDate = null
		set @iNewUndSeqNum = null
		set @iNewUndTradeID = null
		set @dtNewUndTradeDate = null
		return 0
	end

GO

ALTER  PROC dbo.usp_Trade_Match
	@iOldManualTradeID int,
	@iOldActualTradeID int,
	@iOldManualSeqNum int out,
	@iOldActualSeqNum int out,
	@iNewActualSeqNum int out,
	@iNewActualTradeID int out,
	@dtNewActualTradeDate datetime out
as
	set nocount on

	begin tran

	declare @bSucceed bit,
			@nCount int
		
	set @bSucceed = 1

	if @iOldManualTradeID is not null -- delete old manual trade
	begin
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iOldManualTradeID
		
		if @nCount = 1
		begin
			select @iOldManualSeqNum = isnull(max(seqNum),0) + 1 from Trade

			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select tradeID, 0, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, @iOldManualSeqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
			from Trade
			where tradeID = @iOldManualTradeID
		
			if @@error <> 0 set @bSucceed = 0
		end
	end

	if @bSucceed = 1 and @iOldActualTradeID is not null -- delete old actual trade and make new with matched status
	begin
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iOldActualTradeID
		
		if @nCount = 1
		begin
			select @iOldActualSeqNum = isnull(max(seqNum),0) + 1 from Trade

			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select tradeID, 0, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, @iOldActualSeqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
			from Trade
			where tradeID = @iOldActualTradeID
		
			if @@error <> 0 set @bSucceed = 0
		end

		if @bSucceed = 1 -- create new actual matched trade
		begin
			select @iNewActualTradeID = max(tradeID) + 1 from Trade
			if @iNewActualTradeID is null set @iNewActualTradeID = 1
	
			if @dtNewActualTradeDate is null	
			begin
				declare @iDeltaGMT int

				exec @iDeltaGMT = usp_TimeZoneActimeBias_Get
		
				set @dtNewActualTradeDate = dateadd(minute, @iDeltaGMT, getdate())
			end
		
			select @iNewActualSeqNum = isnull(max(seqNum),0) + 1 from Trade
		
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select @iNewActualTradeID, 1, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, 3, execID, @iNewActualSeqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
			from Trade
			where tradeID = @iOldActualTradeID and actionID = 0

			if @@error <> 0 set @bSucceed = 0
		end
	end

	if @bSucceed = 1
	begin
		commit tran
		return @iNewActualSeqNum
	end
	else  
	begin
		rollback tran
		set @iOldManualSeqNum = null
		set @iOldActualSeqNum = null
		set @iNewActualSeqNum = null
		set @iNewActualTradeID = null
		set @dtNewActualTradeDate = null
		return 0
	end

GO

ALTER   PROC dbo.usp_Trade_SU_Exec
	@iOptTradeID int,
        @iUnderlyingID int, 
	@iUndQuantity int,
	@fUndPrice float,
	@tiUndIsBuy tinyint,
	@tiUndStatus tinyint,
	@fUndBrokerCommission float,
	@fUndClearingBrokerCommission float,
	@iNewUndSeqNum int = null out,
	@iNewUndTradeID int = null out,
	@dtNewUndTradeDate datetime = null out
as
	set nocount on

	begin tran

	declare @bSucceed bit
		
	set @bSucceed = 1

	declare @iDeltaGMT int
	exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

	select @iNewUndTradeID = max(tradeID) + 1 from Trade
	select @iNewUndSeqNum = isnull(max(seqNum), 0) + 1 from Trade

	if @iNewUndTradeID is null set @iNewUndTradeID = 1
	
	if @dtNewUndTradeDate is null	
	   set @dtNewUndTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	
	insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
		price, isBuy, traderID, strategyID, brokerID, brokerCommission,
		clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
		isPosition, status, execID, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
	select @iNewUndTradeID, 1, @iUnderlyingID, @dtNewUndTradeDate, @iUndQuantity, @fUndPrice, @tiUndIsBuy,
		traderID, strategyID, brokerID, @fUndBrokerCommission, clearingBrokerID, @fUndClearingBrokerCommission,
		0, 0, 0, @tiUndStatus, null, @iNewUndSeqNum, mark,
		@iUnderlyingID, null, null, null, orderID, execStatus
	from Trade
		where tradeID = @iOptTradeID and actionID = 1
	
		if @@error <> 0 set @bSucceed = 0

	if @bSucceed = 1
	begin
		commit tran
			return @iNewUndSeqNum
	end
	else  
	begin
		rollback tran
		set @iNewUndSeqNum = null
		set @iNewUndTradeID = null
		set @dtNewUndTradeDate = null
		return 0
	end

GO

ALTER    PROC dbo.usp_TradeFeed_1_New
	@vcTraderAcronym varchar(12),
	@vcExecID nvarchar(128),
	@tiIsBuy tinyint,  
	@iQuantity int,
	@fPrice float,
	@dtTradeDate datetime,
	@vcContractSymbol varchar(20),
	@iContractTypeID int,
	@vcUnderlyingSymbol varchar(20) = null,
	@tiIsCall tinyint = null,
	@fStrike float = null,
	@dtExpiry datetime = null,
	@iTradeLotSize int = null,
	@tiIsUpdateEnabled tinyint = 0,
	@iLastTradeTime int = null, 
	@vcOptRootSymbol varchar(20) = null,
	@iMark int = null,
	@vcStrategyName varchar(12) = null,
	@fStockRefPrice float = null,
	@vcOrderID varchar(50) = null,
	@iExecStatus int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iMinSeqNum int 
	declare @iSeqNum int 

	-- find existing trade
	select @iSeqNum = max(seqNum)
	from Trade 
	where execID = @vcExecID and 
		Convert(varchar(8),@dtTradeDate,112) = Convert(varchar(8),tradeDate,112)
	group by
	 	execID, Convert(varchar(8),tradeDate,112), tradeID
	having
		min(actionID) = 1

	Begin Transaction

	if @iSeqNum is not null
	begin
		if @tiIsUpdateEnabled = 1
		begin
			-- delete existing trade
			select @iMinSeqNum = isnull(max(seqNum),0) + 1 from Trade
	
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, @iMinSeqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
			from Trade
			where seqNum = @iSeqNum
			if @@error <> 0 begin set @error = 2 if @@TranCount > 0  Rollback Transaction trade goto finish end
		end
		else
		begin
			--trade already exists
			select -1 as iMaxSeqNum, @iSeqNum as iMinSeqNum, -1 as iStructureMissed
			Rollback Transaction
			Return(-1)
		end 
	end

	Declare @iTradeContractID int, 
			@iUnderlyingContractID int,
			@iStructureMissed int

	set @iStructureMissed = 0

	--check underlying symbol
	if @vcUnderlyingSymbol is null and  @iContractTypeID <3 
		set @vcUnderlyingSymbol = @vcContractSymbol

	if @iContractTypeID = 3 and @vcUnderlyingSymbol is null
        begin
		select @vcUnderlyingSymbol = C.Symbol from Contract C
			inner join OptionRoot O on O.symbol = @vcOptRootSymbol and O.underlyingID = C.ContractID 
		where C.contractTypeID < 3
		if @vcUnderlyingSymbol is NULL
		begin
			select @vcUnderlyingSymbol = C.Symbol from ETS_ASP_EOD.dbo.Contract C
				inner join ETS_ASP_EOD.dbo.OptionRoot O on O.symbol = @vcOptRootSymbol and O.underlyingID = C.ContractID 
			where C.contractTypeID < 3
			if @vcUnderlyingSymbol is NULL
			begin
				-- unknown underlying
				select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed
				Rollback Transaction
				Return(-1)			
			end	
		end
        end
       
	select @iUnderlyingContractID = contractID 
		from ContractView 
		where symbol = @vcUnderlyingSymbol and contractTypeID < 3

	if @iUnderlyingContractID is null
	--add new stock\index
		begin
			insert into Contract(symbol,contractTypeID,expCalendarID) values(@vcUnderlyingSymbol, 2, 1)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			set @iUnderlyingContractID = @@identity

			insert into Stock (stockID,isActive) values(@iUnderlyingContractID,1)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

			insert into ContractPrice (contractID,exchangeID) values(@iUnderlyingContractID,null)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

			set @iStructureMissed = 1
		end
	else
		begin  --check this instruction carefully!
			update Stock set isActive = 1 where stockID = @iUnderlyingContractID
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end

	--check Option
	if @iContractTypeID = 3
		begin
			Declare @iOptionID int,
					@iExpiryID int,
					@iOptionRootID int

			Select @iOptionRootID = optionRootID from OptionRootView where symbol = @vcOptRootSymbol
	
			set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
			set @fStrike = round(@fStrike, 3)
	
			select @iOptionID = optionID
				from OptionView (nolock)
				where optionRootID = @iOptionRootID and 
					isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID < 3
	
			if @iOptionID is null
			--create new option
				begin
					if isnull(@vcContractSymbol, '') <> ''
					begin
						Declare @vcContractName varchar(255)
						  	set @vcContractName = @vcUnderlyingSymbol + ' ' + datename(month, @dtExpiry) + 
										' ' + datename(year, @dtExpiry) + ' ' + case @tiIsCall when 1 then 'Call' else 'Put' end +
											 ' ' + cast(round(@fStrike, 2) as varchar)
		
						insert into Contract (contractTypeID, symbol, contractName) values (3, @vcContractSymbol,@vcContractName)
							if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
							set @iOptionID = @@identity
						
						if @iOptionRootID is null
							begin
								insert into OptionRoot(underlyingID,symbol,lotSize) values(@iUnderlyingContractID,@vcOptRootSymbol, isnull(@iTradeLotSize, 100))
									set @iOptionRootID = @@identity
							end

						insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
							 values (@iOptionID, @iOptionRootID, @tiIsCall, @fStrike, @iExpiryID)
							if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
				
						-- make record with null exchange for this Option
						insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
							if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

					end
					else
					begin
						Select null as iSeqNum
						Return(-2)
					end

					set @iStructureMissed = @iStructureMissed | 2
				end

			set @iTradeContractID = @iOptionID
		end
		else
			set @iTradeContractID = @iUnderlyingContractID


	--work with Trader
	Declare @iTraderID int
		select @iTraderID = traderID from TraderView where acronym = @vcTraderAcronym

	if @iTraderID is null
		begin
			insert into Trader(acronym,traderName) values(@vcTraderAcronym, @vcTraderAcronym)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
				set @iTraderID = @@identity
		end

	if not exists(select 1 from TraderContractView where traderID = @iTraderID and contractID = @iUnderlyingContractID)
		begin
			insert into TraderContract (traderID, contractID) values (@iTraderID, @iUnderlyingContractID)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end
	
	declare @iStrategyID int
	set @vcStrategyName = ltrim(rtrim(isnull(@vcStrategyName, '')))

	if len(@vcStrategyName) > 0
	begin
		select @iStrategyID = strategyID from StrategyView where @vcStrategyName = strategyName
		if @iStrategyID is null
		begin
			insert into Strategy (strategyName) values (@vcStrategyName)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			set @iStrategyID = @@identity
		end
	end

	declare @iNewTradeID int
	select @iNewTradeID = isnull(max(tradeID), 0) + 1 from Trade

	select @iSeqNum = isnull(max(seqNum),0) + 1 from Trade


	--check if was an execution
	declare @iOrderID int
	if not (@vcOrderID is null)
	begin
		select @iOrderID = orderID from [Order] where @vcOrderID = clOrderID
		if @iOrderID is NULL
		begin
			-- unknown Client OrderID
			select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed
			Rollback Transaction
			Return(-1)			
		end
	end
	else
		set @iOrderID = null

	--create new trade	
	insert into Trade (execID, tradeID, actionID, contractID, tradeDate, 
		quantity, price, traderID, strategyID, isBuy, isPosition, status, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID, spotReference, orderID, execStatus)
	values (@vcExecID, @iNewTradeID, 1, @iTradeContractID, @dtTradeDate, 
		@iQuantity, @fPrice, @iTraderID, @iStrategyID, @tiIsBuy, 0, 0, @iSeqNum, @iMark,
		@iUnderlyingContractID, @iOptionRootID, null, null, @fStockRefPrice, @iOrderID, @iExecStatus)

	if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

	--calculate order AvgPrice
	if not (@iOrderID is null)
	begin
		declare @fAvgPrice float
		declare @iCumQty int		set @iCumQty = null
		
		exec usp_FixOrderAvgPrice_Calc @iOrderID, @fAvgPrice output, @iCumQty output
	
		update [Order]
			set 
				status = @iExecStatus,
				avgPrice = isnull(@fAvgPrice, avgPrice), 
				cumQty = isnull(@iCumQty, cumQty) 
			where orderID = @iOrderID
	end

	--write into TradeLog 
	if @iStructureMissed <> 0
		begin
			if exists(select 1 from TradeLog where underlyingID = @iUnderlyingContractID)
			begin
				update TradeLog set eventTypeID = isnull(eventTypeID, 0) | @iStructureMissed
				where underlyingID = @iUnderlyingContractID
					if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			end
			else
			begin
				insert into TradeLog (underlyingID, eventTypeID) values (@iUnderlyingContractID, @iStructureMissed)
					if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			end
		end

	--save the last successful trade time
	if @iLastTradeTime is not null
		begin
			update DataInfo set keyValue = @iLastTradeTime where keyName='lastTradeTime'
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end

	--return output recordset
	select isnull(@iMinSeqNum,@iSeqNum) as iMinSeqNum,@iSeqNum as iMaxSeqNum, @iStructureMissed as iStructureMissed

	Commit Transaction

    set @error = 0
	return @error

finish:
	select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed
	return @@error

GO

ALTER  PROC dbo.usp_TradeFeed_1_Del
	@vcExecID nvarchar(128),
	@dtTradeDate datetime,
	@tiIsUpdateEnabled tinyint = 0,
	@iLastTradeTime int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iSeqNum int,
			@iNewSeqNum int

	-- find existing trade
	select @iSeqNum = max(seqNum)
	from Trade 
	where execID = @vcExecID and 
		Convert(varchar(8),@dtTradeDate,112) = Convert(varchar(8),tradeDate,112)
	group by
	 	execID, Convert(varchar(8),tradeDate,112), tradeID
	having
		min(actionID) = 1

	if @iSeqNum is null
	begin
		-- trade no found
		select -1 as iMaxSeqNum, 0 as iMinSeqNum
		Return(-1)
	end

	select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade

	Begin Transaction trade

	insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
		strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, execID, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
	select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
		strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
	from Trade
	where seqNum = @iSeqNum
	if @@error <> 0 begin set @error = 2 if @@TranCount > 0  Rollback Transaction trade goto finish end

	--trade has been successfully removed from DB
	select @iSeqNum as iMinSeqNum, @iNewSeqNum as iMaxSeqNum

	--save the last successful trade time
	if @iLastTradeTime is not null
	begin
		update DataInfo set keyValue = @iLastTradeTime where keyName='lastTradeTime'
			if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
	end

	Commit Transaction trade

	set @error = 0

	return 0

finish:
	select -1 as iMinSeqNum, -1 as iMaxSeqNum
	Return(@error)

GO

ALTER   PROC dbo.usp_MmTradeByTrader_Get
	@iTraderID int = null
as
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iTraderID is null
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
		order by TradeDate
	else
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
		where
			tr.traderID = @iTraderID
		order by TradeDate

	return @@error
GO

ALTER   PROC dbo.usp_MmTradeSeqByTrader_Get
	@iTraderID int = null,
	@iMinSeqNum int = null,
	@iMaxSeqNum int = null
AS
-- changed by VK - flex option support added
	set nocount on

	set @iMinSeqNum = isnull(@iMinSeqNum, 0)
	set @iMaxSeqNum = isnull(@iMaxSeqNum, 2147483647)

	if @iTraderID is null
		select
			tr.seqNum as iSeqNum,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.exportSymbol as vcImportID,
			c.contractName as vcContractName,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,
			uc.Symbol as vcUnderlyingSymbol,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,

			tl.eventTypeID as iStructureMissed
		from Trade tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where
			tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
		order by tr.seqNum
	else
		select
			tr.seqNum as iSeqNum,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,
			uc.Symbol as vcUnderlyingSymbol,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,

			tl.eventTypeID as iStructureMissed
		from Trade tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where
			tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
			and tr.traderID = @iTraderID
		order by tr.seqNum


GO

ALTER      PROC dbo.usp_TradeSeq_Get
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
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID,
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.underlyingID as iUnderlyingID,
			tr.optionRootID as iOptionRootID,
			tr.futureRootID as iFutureRootID,
			tr.futureID as iFutureID,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus
		from Trade tr
		where 
			(@iMinSeqNum is null or @iMinSeqNum is not null and seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and cast(floor(cast(tradeDate as float)) as datetime) = @dtTradeDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		order by seqNum
	else
		if @iMinSeqNum is not null and @iMaxSeqNum is not null and @dtTradeDate is null and @tiExtendedInfo is not null and @iTraderID is null and @iMinSeqNum = @iMaxSeqNum
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID,
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.underlyingID as iUnderlyingID,
			tr.optionRootID as iOptionRootID,
			tr.futureRootID as iFutureRootID,
			tr.futureID as iFutureID,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.priceClose as fPriceClose,

			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			uc.priceClose as fUnderlyingPriceClose,

			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			case when c.contractTypeID = 3 then c.priceTheo else fcp.priceTheo end as fPriceTheoClose,

			case when c.contractTypeID < 3 then 1
				when c.contractTypeID = 3 then ort.lotSize
				when c.contractTypeID = 4 then fr.futureLotSize
				when c.contractTypeID = 5 then fr.optionLotSize
			end as iLotSize,

			ort.isSynthetic as tiIsSyntheticRoot,

			tl.eventTypeID as iStructureMissed
		from Trade tr 
			-- contract and underlying contract
			inner join ContractPriceView c on tr.contractID = c.contractID
			inner join ContractPriceView uc on tr.underlyingID = uc.contractID
			--left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			--left join ContractPrice ucp on tr.underlyingID = ucp.contractID and ucp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- underlying
			left join Stock s on tr.underlyingID = s.stockID
			left join [Index] i on tr.underlyingID = i.indexID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join ExpiryInCalendarView fmc on f.maturityID = fmc.expiryID and fmc.expCalendarID = fr.matCalendarID
			left join ContractPrice fcp on tr.futureID = fcp.contractID and fcp.exchangeID is null
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where  tr.seqNum = @iMinSeqNum 
		order by tr.seqNum

		else
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID,
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.underlyingID as iUnderlyingID,
			tr.optionRootID as iOptionRootID,
			tr.futureRootID as iFutureRootID,
			tr.futureID as iFutureID,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			cp.priceClose as fPriceClose,

			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			ucp.priceClose as fUnderlyingPriceClose,

			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			case when c.contractTypeID = 3 then cp.priceTheo else fcp.priceTheo end as fPriceTheoClose,

			case when c.contractTypeID < 3 then 1
				when c.contractTypeID = 3 then ort.lotSize
				when c.contractTypeID = 4 then fr.futureLotSize
				when c.contractTypeID = 5 then fr.optionLotSize
			end as iLotSize,

			ort.isSynthetic as tiIsSyntheticRoot,

			tl.eventTypeID as iStructureMissed
		from Trade tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			left join ContractPrice ucp on tr.underlyingID = ucp.contractID and ucp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- underlying
			left join Stock s on tr.underlyingID = s.stockID
			left join [Index] i on tr.underlyingID = i.indexID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join ExpiryInCalendarView fmc on f.maturityID = fmc.expiryID and fmc.expCalendarID = fr.matCalendarID
			left join ContractPrice fcp on tr.futureID = fcp.contractID and fcp.exchangeID is null
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and tr.tradeDate >= @dtTradeDate and tr.tradeDate < @dtMaxDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		order by tr.seqNum

GO

GO
ALTER  PROCEDURE dbo.usp_FixOldOrder_Del
	@dtOrderDate datetime = null
AS
/*
	Created by Michael Malyshkin
	Description: this procedure is delete old Order & OrderExec
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if @dtOrderDate is null set @dtOrderDate = getdate()

	--convert to GMT
	declare @iDeltaGMT int

	exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

	set @dtOrderDate = dateadd(minute, @iDeltaGMT, @dtOrderDate)

	if not exists ( select 1 from [Order] where datediff(dd, orderDate, @dtOrderDate) > 0 ) 
		goto finish

	Begin Transaction

	update Trade
	set orderID = null, execStatus = null
	where seqNum in (
		select tr.seqNum
		from [Trade] tr join [Order] O on O.orderID= tr.orderID
			where datediff(dd, O.orderDate, @dtOrderDate) > 0)
	if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end

	delete [Order]
		where datediff(dd, orderDate, @dtOrderDate) > 0
	if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction goto finish end


	Commit Transaction

    set @error = 0
Return(@error)

finish:
	Return(@error)

GO

GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmCancelledOrderExec_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MmCancelledOrderExec_Get]
GO
CREATE	PROC dbo.usp_MmCancelledOrderExec_Get
	@iTraderID int = null
as
-- derived from usp_MmTradeByTrader_Get
-- by Anar Alimov: loads cancelled/deleted executions/trades with not null orderID
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iTraderID is null
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from Trade tr 
			-- contract and underlying contract
			inner join [Order] ord on tr.orderID = ord.orderID
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
		-- only deleted trades
		where tr.actionID = 0
		order by TradeDate
	else
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from Trade tr 
			-- contract and underlying contract
			inner join [Order] ord on tr.orderID = ord.orderID
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
		where
			-- only deleted trades
			tr.traderID = @iTraderID and tr.actionID = 0
		order by TradeDate

	return @@error
GO

GO

ALTER  PROCEDURE dbo.usp_FixOrderAvgPrice_Calc
	@iOrderID int,
	@fAvgPrc float output,
	@iCumQty int = null out
AS
/*
	Created by Michael Malyshkin
	Description: this procedure is calculate Average Price of order
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	Select @fAvgPrc = sum(tr.price * tr.quantity) , @iCumQty =  sum(tr.quantity)
		from Trade tr 	
	where tr.orderID=@iOrderID and tr.execStatus in (49, 50) and tr.actionID > 0 -- partially filled, filled

	if @@error <> 0 begin set @fAvgPrc = null goto finish end

	if isnull(@iCumQty,0) > 0 set @fAvgPrc = @fAvgPrc / @iCumQty
	else set @fAvgPrc = null

finish:
	Return(0)

GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.181')
	begin
		update DataInfo set keyValue = '3.46.181' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.181' '3.46.181 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO