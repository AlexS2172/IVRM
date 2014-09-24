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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.117' or (keyValue = '3.46.118')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.118 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.118' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

----------------------------------------------------------------------------------------------------
-- HedgeData
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id('dbo.HedgeData') and sysstat & 0xf = 3)
	DROP TABLE HedgeData
GO

CREATE TABLE HedgeData
(
	hedgeDataID int IDENTITY (1, 1) NOT NULL ,
	traderID int NOT NULL ,
	strategyID int NULL ,
	contractID int NULL ,
	state tinyint NOT NULL CONSTRAINT DF_HedgeData_state DEFAULT (0),
	actionDate datetime NOT NULL CONSTRAINT DF_HedgeData_actionDate DEFAULT (getdate()),
	CONSTRAINT PK_HedgeData PRIMARY KEY  CLUSTERED 
	(
		hedgeDataID
	),
	CONSTRAINT PK_HedgeData_Trader FOREIGN KEY 
	(
		traderID
	) REFERENCES Trader (
		traderID
	),
	CONSTRAINT PK_HedgeData_Strategy FOREIGN KEY 
	(
		strategyID
	) REFERENCES Strategy (
		strategyID
	),
	CONSTRAINT PK_HedgeData_Contract FOREIGN KEY 
	(
		contractID
	) REFERENCES Contract (
		contractID
	),
)
GO

CREATE UNIQUE INDEX [IX_HedgeData] ON [dbo].[HedgeData]([traderID], [strategyID], [contractID])
GO

if exists (select * from sysobjects where id = object_id('dbo.tU_HedgeData') and sysstat & 0xf = 8)
	drop trigger dbo.tU_HedgeData
GO

CREATE TRIGGER dbo.tU_HedgeData ON dbo.HedgeData
    FOR Update
AS
	set nocount on
	
	update HedgeData
		set actionDate = getdate()
	from HedgeData t 
		join inserted on t.hedgeDataID = inserted.hedgeDataID
GO

----------------------------------------------------------------------------------------------------
-- usp_MmHedgeData_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'dbo.usp_MmHedgeData_Get') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure dbo.usp_MmHedgeData_Get
GO

CREATE PROC dbo.usp_MmHedgeData_Get
AS
	set nocount on

	select
		iTraderID,
		iStrategyID,
		iContractID,
		tiState
	from
	(
		select
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID,
			tr.underlyingID as iContractID,
			isnull(hd.state, 0) as tiState
		from
			(
				select
					tr.traderID,
					tr.strategyID,
					tr.underlyingID
				from
					TradeView tr
					inner join Stock s on s.stockID = tr.underlyingID
				--where
				--	tr.futureRootID is null
				group by
					tr.traderID, tr.strategyID, tr.underlyingID
			) tr
			left join HedgeData hd on
					tr.traderID = hd.traderID
					and isnull(tr.strategyID, 0) = isnull(hd.strategyID, 0)
					and tr.underlyingID = hd.contractID
	union
		select
			hd.traderID as iTraderID,
			hd.strategyID as iStrategyID,
			hd.contractID as iContractID,
			hd.state as tiState
		from
			HedgeData hd
			left join
			(
				select
					tr.traderID
				from
					TradeView tr
					inner join Stock s on s.stockID = tr.underlyingID
				--where
				--	tr.futureRootID is null
				group by
					tr.traderID
			) tr on tr.traderID = hd.traderID
		where
			hd.strategyID is null
			and hd.contractID is null
	) hd
	order by
		iTraderID, iStrategyID, iContractID

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_MmHedgeData_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'dbo.usp_MmHedgeData_Save') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure dbo.usp_MmHedgeData_Save
GO

CREATE PROC dbo.usp_MmHedgeData_Save
	@iTraderID int = null,
	@iStrategyID int = null,
	@iContractID int = null,
	@tiState tinyint = 0
AS
	set nocount on
	declare @error int		set @error = 0

	if @iTraderID is null and @iStrategyID is null and @iContractID is null
	begin
		raiserror ('Invalid parameters specified.', 16, 1)
		set @error = -1
		goto finish
	end

	begin tran

	if exists(select top 1 1 from HedgeData where traderID = @iTraderID
												and isnull(strategyID, 0) = isnull(@iStrategyID, 0)
												and isnull(contractID, 0) = isnull(@iContractID, 0))
	begin
		update HedgeData
		set state = @tiState
		where
			traderID = @iTraderID
			and isnull(strategyID, 0) = isnull(@iStrategyID, 0)
			and isnull(contractID, 0) = isnull(@iContractID, 0)

		if @@error <> 0 begin set @error = -2 if @@trancount = 1 rollback tran goto finish end
	end
	else
	begin
		insert into HedgeData
			(traderID, strategyID, contractID, state)
		values
			(@iTraderID, @iStrategyID, @iContractID, @tiState)

		if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran goto finish end
	end

	commit tran

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_MmHedgeData_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'dbo.usp_MmHedgeData_Del') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure dbo.usp_MmHedgeData_Del
GO

CREATE PROC dbo.usp_MmHedgeData_Del
	@iTraderID int = null,
	@iStrategyID int = null,
	@iContractID int = null
AS
	set nocount on
	declare @error int		set @error = 0

	begin tran

	delete HedgeData
	where traderID = @iTraderID
		and strategyID = @iStrategyID
		and contractID = @iContractID
	if @@error <> 0 begin set @error = -1 if @@trancount = 1 rollback tran goto finish end

	commit tran

finish:
	return @error
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


----------------------------------------------------------------------------------------------------
-- usp_BoStrategy_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'dbo.usp_BoStrategy_Del') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure dbo.usp_BoStrategy_Del
GO

CREATE PROC dbo.usp_BoStrategy_Del
    @iStrategyID int
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	if exists(select * from Trade where strategyID = @iStrategyID)
		Return(-1)

	begin transaction
		
	delete HedgeData
	where strategyID = @iStrategyID

	update Strategy set actionID = 3 where strategyID = @iStrategyID
		if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

	commit transaction

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTrader_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'dbo.usp_BoTrader_Del') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure dbo.usp_BoTrader_Del
GO

CREATE PROC dbo.usp_BoTrader_Del
	@iTraderID int
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	if exists(select 1 from Trade where traderID = @iTraderID)
		begin
			Return(-1)
		end
	
	if exists(select 1 from [Order] where traderID = @iTraderID)
		begin
			Return(-2)
		end

	if exists(select 1 from TntCard where traderID = @iTraderID)
		begin
			Return(-3)
		end
	
	Begin Transaction
		delete HedgeData
		where traderID = @iTraderID
    
	    update TraderContract set actionID=3 where traderID = @iTraderID
			if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end
			
        update Trader set actionID=3, traderGroupID=null where traderID = @iTraderID
			if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction

finish:
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_BoStock_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'dbo.usp_BoStock_Del') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure dbo.usp_BoStock_Del
GO

CREATE PROC dbo.usp_BoStock_Del
    @iStockID int
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0
	
	if exists (select 1 from Trade (nolock) where contractID = @iStockID)
		return -1

	if exists (select 1 from [Order] (nolock) where contractID = @iStockID)
		return -2

	Create table #Options_for_delete(OptionID int)

	insert into #Options_for_delete
		select optionID from [Option] o (nolock)
				join OptionRoot ort on o.optionRootID = ort.optionRootID
 		where underlyingID = @iStockID

	if exists (select 1 from Trade where contractID in (select optionID from #Options_for_delete))
		return -3

	if exists (select 1 from [Order] where contractID in (select optionID from #Options_for_delete))
		return -4

	Begin Transaction StockDel
		delete HedgeData
		where contractID = @iStockID

		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del 1
			end

		exec usp_TradeLog_Del @iStockID

		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
			if @@error <> 0 begin set @error = -5 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
			if @@error <> 0 begin set @error = -6 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		delete from NextDaySurface where contractID = @iStockID
			if @@error <> 0 begin set @error = -7 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		delete VolaSurfaceData where contractID = @iStockID
			if @@error <> 0 begin set @error = -8 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		delete VolaBands where contractID = @iStockID
			if @@error <> 0 begin set @error = -9 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
		delete from ContractMarketData where contractID = @iStockID
			if @@error <> 0 begin set @error = -10 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update IndexBeta set actionID = 3 where contractID = @iStockID
			if @@error <> 0 begin set @error = -11 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update ContractExchangeLink set actionID = 3 where contractID = @iStockID
			if @@error <> 0 begin set @error = -12 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
	
        update ContractInGroup set actionID=3 where contractID = @iStockID 
			if @@error <> 0 begin set @error = -13 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        delete StockDataHistory where stockID = @iStockID  
			if @@error <> 0 begin set @error = -14 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        delete StockCorrelation where stockID1 = @iStockID  
			if @@error <> 0 begin set @error = -15 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        delete StockCorrelation where stockID2 = @iStockID  
			if @@error <> 0 begin set @error = -16 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        update IndexDefinition set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = -17 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update TraderContract set actionID=3 where contractID = @iStockID  
			if @@error <> 0 begin set @error = -19 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        update Stock set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = -20 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        delete ContractPrice where contractID = @iStockID
			if @@error <> 0 begin set @error = -21 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update SyntheticUnderlyingParam set actionID=3 where underlyingID = @iStockID or syntheticOptRootID in 
			(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iStockID)
			if @@error <> 0 begin set @error = -22 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where syntheticOptRootID in 
			(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iStockID)
			if @@error <> 0 begin set @error = -23 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update SyntheticOptionRoot set actionID=3 where optionRootID in (select optionRootID from OptionRoot where underlyingID = @iStockID)
			if @@error <> 0 begin set @error = -24 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

		update OptionRoot set actionID = 3 where underlyingID = @iStockID
			if @@error <> 0 begin set @error = -25 if @@TranCount != 0 Rollback Transaction StockDel goto finish end

        update Contract set actionID=3 where contractID = @iStockID
			if @@error <> 0 begin set @error = -26 if @@TranCount != 0 Rollback Transaction StockDel goto finish end
  
		set @error = @@error
	Commit Transaction
finish:
	return @error
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.118')
	begin
		update DataInfo set keyValue = '3.46.118' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.118' '3.46.118 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
