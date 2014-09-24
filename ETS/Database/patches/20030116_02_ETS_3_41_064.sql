/*
fix for last release of ETS
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.41.063' or (keyValue = '3.41.064')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.41.064 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.41.064' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingAverageVolumes_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingAverageVolumes_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE  PROCEDURE dbo.usp_UnderlyingAverageVolumes_Save 
	@vcSymbol varchar(20), 
	@iVolume10Day int = null,
	@iVolume5Expiration int = null
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is save information into ContractPrice table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iContractID int

	select @iContractID = contractID
		from ContractView 
		where contractTypeID < 3 and upper(symbol) = upper(@vcSymbol)

	Begin Transaction
		if not exists(select * from ContractPrice where contractID=@iContractID and isnull(exchangeID,0)=0)
			begin
				insert into ContractPrice (contractID,
						exchangeID, 
						volume10Day,
						volume5Expiration)
				  values (@iContractID,
						0, 
						@iVolume10Day,
						@iVolume5Expiration)

				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin

				update ContractPrice set 
					volume10Day = isnull(@iVolume10Day, volume10Day),
					volume5Expiration = isnull(@iVolume5Expiration, volume5Expiration)
				where contractID = @iContractID and isnull(exchangeID,0)=0

	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction


    set @error = 0
finish:
Return(@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Exec]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Exec]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Match]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Match]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE  procedure dbo.usp_Trade_Exec
	@tiExercise tinyint,
	@iOptTradeID int,
	@iOptQuantity int,
	@tiOptIsBuy tinyint,
	@tiOptStatus tinyint,
	@iUndContractID int,
	@iUndQuantity int,
	@fUndPrice float,
	@tiUndIsBuy tinyint,
	@tiUndStatus tinyint,
	@iUndBrokerCommissionID int,
	@iUndClearingBrokerCommissionID int,
	@iNewOptSeqNum int = null out,
	@iNewOptTradeID int = null out,
	@dtNewOptTradeDate datetime = null out,
	@iNewUndSeqNum int = null out,
	@iNewUndTradeID int = null out,
	@dtNewUndTradeDate datetime = null out
as
	set nocount on

	begin tran

	declare @bSucceed bit
		
	set @bSucceed = 1

	declare @iDeltaGMT int
	exec master.dbo.xp_regread 'HKEY_LOCAL_MACHINE', 'SYSTEM\CurrentControlSet\Control\TimeZoneInformation', 'Bias', @iDeltaGMT OUT

	select @iNewOptTradeID = max(tradeID) + 1 from Trade
		if @iNewOptTradeID is null set @iNewOptTradeID = 1

	select @iNewOptSeqNum = isnull(max(seqNum),0) + 1 from Trade

	if @dtNewOptTradeDate is null	
	begin
		set @dtNewOptTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	end

	insert into Trade(tradeID, actionID, contractID, isClose, tradeDate, quantity,
		price, isBuy, bookID, strategyID, traderID, brokerID, clearingBrokerID,
		brokerCommissionID, clearingBrokerCommissionID, tradedIV, spotReference,
		isPosition, status, execID, SeqNum)
	select @iNewOptTradeID, 1, contractID, isClose, @dtNewOptTradeDate, @iOptQuantity, 0, @tiOptIsBuy,
		bookID, strategyID, traderID, brokerID, clearingBrokerID, brokerCommissionID,
		clearingBrokerCommissionID, tradedIV, spotReference, 0, @tiOptStatus,
		null, @iNewOptSeqNum
	from Trade
	where tradeID = @iOptTradeID and actionID = 1

	if @@error <> 0 set @bSucceed = 0

	if @bSucceed = 1 and isnull(@tiExercise, 0) <> 0
	begin
		select @iNewUndTradeID = max(tradeID) + 1 from Trade
			if @iNewUndTradeID is null set @iNewUndTradeID = 1
	
		select @iNewUndSeqNum = isnull(max(seqNum),0) + 1 from Trade
	
		if @dtNewUndTradeDate is null	
		begin
			set @dtNewUndTradeDate = dateadd(minute, @iDeltaGMT, getdate())
		end
	
		insert into Trade(tradeID, actionID, contractID, isClose, tradeDate, quantity,
			price, isBuy, bookID, strategyID, traderID, brokerID, clearingBrokerID,
			brokerCommissionID, clearingBrokerCommissionID, tradedIV, spotReference,
			isPosition, status, execID, SeqNum)
		select @iNewUndTradeID, 1, @iUndContractID, isClose, @dtNewUndTradeDate, @iUndQuantity, @fUndPrice, @tiUndIsBuy,
			bookID, strategyID, traderID, brokerID, clearingBrokerID, @iUndBrokerCommissionID,
			@iUndClearingBrokerCommissionID, 0, 0, 0, @tiUndStatus,
			null, @iNewUndSeqNum
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE   procedure dbo.usp_Trade_Match
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

			insert into Trade(tradeID, actionID, contractID, isClose, tradeDate, quantity,
				price, isBuy, bookID, strategyID, traderID, brokerID, clearingBrokerID,
				brokerCommissionID, clearingBrokerCommissionID, tradedIV, spotReference,
				isPosition, status, execID, SeqNum)
			select tradeID, 0, contractID, isClose, tradeDate, quantity, price, isBuy,
				bookID, strategyID, traderID, brokerID, clearingBrokerID, brokerCommissionID,
				clearingBrokerCommissionID, tradedIV, spotReference, isPosition, status,
				execID, @iOldManualSeqNum
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

			insert into Trade(tradeID, actionID, contractID, isClose, tradeDate, quantity,
				price, isBuy, bookID, strategyID, traderID, brokerID, clearingBrokerID,
				brokerCommissionID, clearingBrokerCommissionID, tradedIV, spotReference,
				isPosition, status, execID, SeqNum)
			select tradeID, 0, contractID, isClose, tradeDate, quantity, price, isBuy,
				bookID, strategyID, traderID, brokerID, clearingBrokerID, brokerCommissionID,
				clearingBrokerCommissionID, tradedIV, spotReference, isPosition, status,
				execID, @iOldActualSeqNum
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
				exec master.dbo.xp_regread 'HKEY_LOCAL_MACHINE', 'SYSTEM\CurrentControlSet\Control\TimeZoneInformation', 'Bias', @iDeltaGMT OUT
		
				set @dtNewActualTradeDate = dateadd(minute, @iDeltaGMT, getdate())
			end
		
			select @iNewActualSeqNum = isnull(max(seqNum),0) + 1 from Trade
		
			insert into Trade(tradeID, actionID, contractID, isClose, tradeDate, quantity,
				price, isBuy, bookID, strategyID, traderID, brokerID, clearingBrokerID,
				brokerCommissionID, clearingBrokerCommissionID, tradedIV, spotReference,
				isPosition, status, execID, SeqNum)
			select @iNewActualTradeID, 1, contractID, isClose, tradeDate, quantity, price, isBuy,
				bookID, strategyID, traderID, brokerID, clearingBrokerID, brokerCommissionID,
				clearingBrokerCommissionID, tradedIV, spotReference, isPosition, 3,
				execID, @iNewActualSeqNum
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.41.064')
	begin
		update DataInfo set keyValue = '3.41.064' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.41.064' '3.41.064 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
