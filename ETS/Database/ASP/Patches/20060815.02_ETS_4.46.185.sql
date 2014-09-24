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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.184'  or (keyValue = '3.46.185') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.185 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.185' as  'Version in script' from DataInfo where dataInfoID = 1
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
ALTER TABLE Trade
	ALTER COLUMN execID nvarchar(128) NULL
GO

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
		@vcExecID nvarchar(128)

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
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.185')
	begin
		update DataInfo set keyValue = '3.46.185' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.185' '3.46.185 update message:' 
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