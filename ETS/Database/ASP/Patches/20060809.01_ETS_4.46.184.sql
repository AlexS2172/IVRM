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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.183'  or (keyValue = '3.46.184') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.184 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.184' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO


---------------------------------------------------------------------------------------------------

IF EXISTS(SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = 'TradeIDs')
   DROP TABLE TradeIDs
GO

CREATE TABLE dbo.[TradeIDs] (
	[tradeID] [int] IDENTITY(1, 1) NOT NULL
) ON [PRIMARY]
GO

SET IDENTITY_INSERT [TradeIDs] ON
GO
INSERT [TradeIDs] (tradeID) SELECT max(tradeID) from [Trade]
GO
SET IDENTITY_INSERT [TradeIDs] OFF
GO

---------------------------------------------------------------------------------------------------

---------------------------------------------------------------------------------------------------
ALTER   PROC dbo.usp_Trade_Del
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

		--select @iNewSeqNum = isnull(max(seqNum), 0) + 1 from Trade
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iTradeID

		if @nCount = 1
		begin
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
			from Trade
			where tradeID = @iTradeID
			if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
			
			set @iNewSeqNum = @@identity
			
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


ALTER       PROC dbo.usp_Trade_Exec
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

	--select @iNewOptTradeID = max(tradeID) + 1 from Trade
	--if @iNewOptTradeID is null set @iNewOptTradeID = 1
	insert TradeIDs default values
	set @iNewOptTradeID = @@identity
	
	--select @iNewOptSeqNum = isnull(max(seqNum), 0) + 1 from Trade
	
	if @dtNewOptTradeDate is null	
		set @dtNewOptTradeDate = dateadd(minute, @iDeltaGMT, getdate())

	insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
		price, isBuy, traderID, strategyID, brokerID, brokerCommission,
		clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
		isPosition, status, execID, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
	select @iNewOptTradeID, 1, contractID, @dtNewOptTradeDate, @iOptQuantity, @priceForIndexAndExer , @tiOptIsBuy,
		traderID, strategyID, brokerID, @fOptBrokerCommission, clearingBrokerID, @fOptClearingBrokerCommission,
		tradedIV, spotReference, 0, @tiOptStatus, null, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
	from Trade
	where tradeID = @iOptTradeID and actionID = 1

	if @@error <> 0 set @bSucceed = 0
	
	set @iNewOptSeqNum = @@identity

	if @bSucceed = 1 and isnull(@tiExercise, 0) <> 0 and @tiIsIndex <> 1
	begin
		--select @iNewUndTradeID = max(tradeID) + 1 from Trade
			--if @iNewUndTradeID is null set @iNewUndTradeID = 1
		insert TradeIDs default values
		set @iNewUndTradeID = @@identity
	
		--select @iNewUndSeqNum = isnull(max(seqNum), 0) + 1 from Trade
	
		if @dtNewUndTradeDate is null	
			set @dtNewUndTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	
		if isnull(@tiIsFutures, 0) = 0
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select @iNewUndTradeID, 1, underlyingID, @dtNewUndTradeDate, @iUndQuantity, @fUndPrice, @tiUndIsBuy,
				traderID, strategyID, brokerID, @fUndBrokerCommission, clearingBrokerID, @fUndClearingBrokerCommission,
				0, 0, 0, @tiUndStatus, null, mark,
				underlyingID, null, null, null, orderID, execStatus
			from Trade
			where tradeID = @iOptTradeID and actionID = 1
		else
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select @iNewUndTradeID, 1, futureID, @dtNewUndTradeDate, @iUndQuantity, @fUndPrice, @tiUndIsBuy,
				traderID, strategyID, brokerID, @fUndBrokerCommission, clearingBrokerID, @fUndClearingBrokerCommission,
				0, 0, 0, @tiUndStatus, null, mark,
				underlyingID, null, futureRootID, futureID, orderID, execStatus
			from Trade
			where tradeID = @iOptTradeID and actionID = 1
		
		set @iNewUndSeqNum = @@identity
		
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

ALTER   PROC dbo.usp_Trade_Match
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
			--select @iOldManualSeqNum = isnull(max(seqNum),0) + 1 from Trade

			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select tradeID, 0, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
			from Trade
			where tradeID = @iOldManualTradeID
		
			if @@error <> 0 set @bSucceed = 0
			
			set @iOldManualSeqNum = @@identity
		end
	end

	if @bSucceed = 1 and @iOldActualTradeID is not null -- delete old actual trade and make new with matched status
	begin
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iOldActualTradeID
		
		if @nCount = 1
		begin
			--select @iOldActualSeqNum = isnull(max(seqNum),0) + 1 from Trade

			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select tradeID, 0, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
			from Trade
			where tradeID = @iOldActualTradeID
		
			if @@error <> 0 set @bSucceed = 0
			
			set @iOldActualSeqNum = @@identity
		end

		if @bSucceed = 1 -- create new actual matched trade
		begin
			--select @iNewActualTradeID = max(tradeID) + 1 from Trade
			--if @iNewActualTradeID is null set @iNewActualTradeID = 1
			insert TradeIDs default values
			set @iNewActualTradeID = @@identity
	
			if @dtNewActualTradeDate is null	
			begin
				declare @iDeltaGMT int

				exec @iDeltaGMT = usp_TimeZoneActimeBias_Get
		
				set @dtNewActualTradeDate = dateadd(minute, @iDeltaGMT, getdate())
			end
		
			--select @iNewActualSeqNum = isnull(max(seqNum),0) + 1 from Trade
		
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select @iNewActualTradeID, 1, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, 3, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
			from Trade
			where tradeID = @iOldActualTradeID and actionID = 0

			if @@error <> 0 set @bSucceed = 0
			
			set @iNewActualSeqNum = @@identity
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

ALTER   PROC dbo.usp_Trade_New
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

	--select 
		--@iNewTradeID = isnull(max(tradeID), 0) + 1,
		--@iNewSeqNum = isnull(max(seqNum), 0) + 1
	--from Trade
	insert TradeIDs default values
	set @iNewTradeID = @@identity


	-- create new trade
	insert into Trade (tradeID, actionID, contractID, tradeDate,  quantity, price, traderID, strategyID,
		isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
	values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, @iStrategyID, @tiIsBuy,
		@iBrokerID, @fBrokerCommission, @iClearingBrokerID, @fClearingBrokerCommission,
		@fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, @iMark,
		@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID, @iOrderID, @iExecStatus)

	if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

	set @iNewSeqNum = @@identity

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

ALTER  PROC dbo.usp_Trade_PositionTransfer
	@iContractID int,
	@iQuantity int,
	@fPrice float,
	@tiStatus tinyint,

	@iTraderIDFrom int,
	@iStrategyIDFrom int,
	@iMarkFrom int,
    @tiIsBuyFrom tinyint,  

	@iTraderIDTo int,
	@iStrategyIDTo int,
	@iMarkTo int,
    @tiIsBuyTo tinyint,  

	@iSeqNumFrom int out,
	@iTradeIDFrom int out,
	@dtTradeDateFrom datetime out,

	@iSeqNumTo int out,
	@iTradeIDTo int out,
	@dtTradeDateTo datetime out,

	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null
as
	set nocount on

	begin tran

	declare @error int		set @error = 0

	if not exists(select top 1 1 from TraderContractView where traderID = @iTraderIDFrom and contractID = @iUnderlyingID)
	begin
		insert into TraderContract (traderID, contractID) values (@iTraderIDFrom, @iUnderlyingID)
			if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	end

	if not exists(select top 1 1 from TraderContractView where traderID = @iTraderIDTo and contractID = @iUnderlyingID)
	begin
		insert into TraderContract (traderID, contractID) values (@iTraderIDTo, @iUnderlyingID)
			if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	end

	declare @iDeltaGMT int
	exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

	-- Trade 'from' creation
	if @dtTradeDateFrom is null	
		set @dtTradeDateFrom = dateadd(minute, @iDeltaGMT, getdate())

	-- get new tradeID ane seqNum for trade from
	--select 
		--@iTradeIDFrom = isnull(max(tradeID), 0) + 1,
		--@iSeqNumFrom = isnull(max(seqNum), 0) + 1
	--from Trade
	insert TradeIDs default values
	set @iTradeIDFrom = @@identity

	-- create new trade from
	insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, strategyID,
		isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	values (@iTradeIDFrom, 1, @iContractID, @dtTradeDateFrom, @iQuantity, @fPrice, @iTraderIDFrom, @iStrategyIDFrom,
		@tiIsBuyFrom, null, null, null, null,
		null, null, 0, @tiStatus, @iMarkFrom,
		@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID) 

	if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

	set @iSeqNumFrom = @@identity

	-- Trade 'to' creation
	if @dtTradeDateTo is null	
		set @dtTradeDateTo = dateadd(minute, @iDeltaGMT, getdate())

	-- get new tradeID ane seqNum for trade to
	--select 
		--@iTradeIDTo = isnull(max(tradeID), 0) + 1,
		--@iSeqNumTo = isnull(max(seqNum), 0) + 1
	--from Trade
	insert TradeIDs default values
	set @iTradeIDTo = @@identity

	-- create new trade to
	insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, strategyID,
		isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	values (@iTradeIDTo, 1, @iContractID, @dtTradeDateTo, @iQuantity, @fPrice, @iTraderIDTo, @iStrategyIDTo,
		@tiIsBuyTo, null, null, null, null,
		null, null, 0, @tiStatus, @iMarkTo,
		@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID) 

	if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

	set @iSeqNumTo = @@identity
	
	commit tran

finish:
	if @error <> 0
	begin
		set @iSeqNumFrom = null
		set @iTradeIDFrom = null
		set @dtTradeDateFrom = null
	
		set @iSeqNumTo = null
		set @iTradeIDTo = null
		set @dtTradeDateTo = null
	end

	return @error

GO

ALTER    PROC dbo.usp_Trade_SU_Exec
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

	--select @iNewUndTradeID = max(tradeID) + 1 from Trade
	--select @iNewUndSeqNum = isnull(max(seqNum), 0) + 1 from Trade
	insert TradeIDs default values
	set @iNewUndTradeID = @@identity
	
	if @iNewUndTradeID is null set @iNewUndTradeID = 1
	
	if @dtNewUndTradeDate is null	
	   set @dtNewUndTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	
	insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
		price, isBuy, traderID, strategyID, brokerID, brokerCommission,
		clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
		isPosition, status, execID, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
	select @iNewUndTradeID, 1, @iUnderlyingID, @dtNewUndTradeDate, @iUndQuantity, @fUndPrice, @tiUndIsBuy,
		traderID, strategyID, brokerID, @fUndBrokerCommission, clearingBrokerID, @fUndClearingBrokerCommission,
		0, 0, 0, @tiUndStatus, null, mark,
		@iUnderlyingID, null, null, null, orderID, execStatus
	from Trade
		where tradeID = @iOptTradeID and actionID = 1
	
		if @@error <> 0 set @bSucceed = 0
		
	set @iNewUndSeqNum = @@identity

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

ALTER    PROC dbo.usp_Trade_Update
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
				--select @iNewSeqNum = isnull(max(seqNum), 0) + 1 from Trade
				select @vcExecID = execID from Trade where tradeID = @iOldTradeID
	
				insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
					strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
					tradedIV, spotReference, isPosition, status, execID, mark,
					underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
				select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
					strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
					tradedIV, spotReference, isPosition, status, execID, mark,
					underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
				from Trade
				where tradeID = @iOldTradeID

				if @@error <> 0 or @error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
				
				set @iNewSeqNum = @@identity
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

		--select 
			--@iNewTradeID = isnull(max(tradeID), 0) + 1,
			--@iNewSeqNum = isnull(max(seqNum), 0) + 1
		--from Trade
		insert TradeIDs default values
		set @iNewTradeID = @@identity
	
	
		insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, 
			strategyID, isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
			tradedIV, spotReference, isPosition, status, execID, mark,
			underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
		values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, 
			@iStrategyID,  @tiIsBuy, @iBrokerID, @fBrokerCommission, @iClearingBrokerID, 
			@fClearingBrokerCommission, @fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, 
			@vcExecID, @iMark,
			@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID, @iOrderID, @iExecStatus)

		if @@error <> 0 or @error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
		
		set @iNewSeqNum = @@identity

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

ALTER   PROC dbo.usp_TradeCA_Del
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

	--select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade
	
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
		tradedIV, spotReference, isPosition, status, execID, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
	select 
		tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
		strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, execID, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
	from Trade
	where tradeID = @iTradeID

	set @error = @@error
		if @error <> 0 begin Rollback Transaction TD Return(0) end
		
	set @iNewSeqNum = @@identity

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

ALTER   PROC dbo.usp_TradeCA_Save
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
		--select @iNewTradeID = isnull(max(tradeID),0) + 1 from Trade
		insert TradeIDs default values
		set @iNewTradeID = @@identity

	Declare @iNewSeqNum int 
		--select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade

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
			isBuy, traderID, strategyID, isPosition, status, execID, brokerID, brokerCommission,
			clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference, tradeDate, mark,
			underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)	
		select @iNewTradeID, 1, contractID, @iQuantity, @fPrice,
			isBuy, traderID, strategyID, isPosition, status, execID, brokerID, brokerCommission,
			clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference, tradeDate, mark,
			underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
		from Trade
		where tradeID = @iTradeID and actionID=1

		set @error = @@error
			if @error != 0 begin Rollback Transaction TU Return(-1) end
			
		set @iNewSeqNum = @@identity
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
			isBuy, traderID, strategyID, isPosition, status, brokerID, brokerCommission,
			clearingBrokerID, clearingBrokerCommission, tradeDate, mark,
			underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
		values 
			(@iNewTradeID, 1, @iContractID, @iQuantity, @fPrice, @tiIsBuy, @iTraderID,
			@iStrategyID, @tiIsPosition, @tiStatus, null, null, null, null, @dtNewTradeDate, @iMark,
			@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID, @iOrderID, @iExecStatus)

		set @error = @@error
			if @error != 0 begin Rollback Transaction TU Return(-1) end
		
		set @iNewSeqNum = @@identity
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

ALTER   PROC dbo.usp_TradeFeed_1_Del
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

	--select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade

	Begin Transaction trade

	insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
		strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, execID, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
	select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
		strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, execID, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
	from Trade
	where seqNum = @iSeqNum
	if @@error <> 0 begin set @error = 2 if @@TranCount > 0  Rollback Transaction trade goto finish end
	
	set @iNewSeqNum = @@identity

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

ALTER     PROC dbo.usp_TradeFeed_1_New
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
			--select @iMinSeqNum = isnull(max(seqNum),0) + 1 from Trade
	
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
			from Trade
			where seqNum = @iSeqNum
			if @@error <> 0 begin set @error = 2 if @@TranCount > 0  Rollback Transaction trade goto finish end
			
			set @iMinSeqNum = @@identity
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
	--select @iNewTradeID = isnull(max(tradeID), 0) + 1 from Trade
	insert TradeIDs default values
	set @iNewTradeID = @@identity
	
	--select @iSeqNum = isnull(max(seqNum),0) + 1 from Trade
	

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
		quantity, price, traderID, strategyID, isBuy, isPosition, status, mark,
		underlyingID, optionRootID, futureRootID, futureID, spotReference, orderID, execStatus)
	values (@vcExecID, @iNewTradeID, 1, @iTradeContractID, @dtTradeDate, 
		@iQuantity, @fPrice, @iTraderID, @iStrategyID, @tiIsBuy, 0, 0, @iMark,
		@iUnderlyingContractID, @iOptionRootID, null, null, @fStockRefPrice, @iOrderID, @iExecStatus)

	if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

	set @iSeqNum = @@identity

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

--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.184')
	begin
		update DataInfo set keyValue = '3.46.184' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.184' '3.46.184 update message:' 
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