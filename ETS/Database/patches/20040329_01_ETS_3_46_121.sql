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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.120' or (keyValue = '3.46.121')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.121 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.121' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/


----------------------------------------------------------------------------------------------------
-- job_Cleanup_DB
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'dbo.job_Cleanup_DB') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure dbo.job_Cleanup_DB
GO

CREATE PROC dbo.job_Cleanup_DB
AS
	/*
		Description: this procedure is remove information from base tables with actionID = 3
	*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

--	Begin Transaction DB_Cleanup

		--SyntheticUnderlyingParam by actionID = 3
		exec @error = usp_FixOldOrder_Del
  			if @error <> 0 begin if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--SyntheticUnderlyingParam by actionID = 3
		delete SyntheticUnderlyingParam where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--SyntheticUnderlyingParam by Contract.actionID = 3
		delete SyntheticUnderlyingParam from SyntheticUnderlyingParam s join Contract c on s.underlyingID = c.contractID
			where c.actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--IRPoint
		delete IRPoint where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--IRCurve
		delete IRPoint where curveID in (select curveID from IRCurve where actionID = 3)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		delete IRCurve where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--ContractInGroup
		delete ContractInGroup where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--ContractGroup
		delete ContractInGroup where groupID in (select groupID from ContractGroup where actionID = 3)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		delete ContractGroup where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--IndexDefinition
		delete IndexDefinition where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--ExchangeHoliday
		delete ExchangeHoliday where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Holiday
		delete ExchangeHoliday where holidayID in (select holidayID from Holiday where actionID = 3)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		delete Holiday where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Strategy
		delete Strategy 
			where actionID = 3 and strategyID not in 
				(select distinct strategyID from Trade where strategyID is not null)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--TraderContract
		delete TraderContract where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--RMScenario
		delete RMScenario where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--CustomStrikeSkewPoint
		delete from CustomStrikeSkewPoint where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--VolaSurfaceData
		delete VolaSurfaceData where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--VolaSurfaceData
		delete CustomStrikeSkewPoint 
			where volaSurfaceDataID in (select volaSurfaceDataID from VolaSurfaceData where actionID = 3)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--TraderContract 
		delete from TraderContract 
			where traderID in 
				(select traderID from Trader where actionID = 3 and traderID not in (select distinct traderID from Trade where traderID is not null)
					and traderID not in (select distinct traderID from [Order] where traderID is not null))
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Trader
		delete Trader where traderID not in (select distinct traderID from Trade) and actionID =3
			and traderID not in (select distinct traderID from [Order] where traderID is not null)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Broker
		delete Broker 
			where actionID = 3 and 
				brokerID not in (select distinct clearingBrokerID from Trade where clearingBrokerID is not null) and 
				brokerID not in (select distinct brokerID from Trade where brokerID is not null)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
	
		--=========================================================================================================
		--INDEX
		if object_id('tempdb..#IDs') is null
			Create table #IDs(contractID int not null)
		else
			Delete from #IDs
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--select indices for deletion (which not releated with Trades)
		insert into #IDs
			select indexID from [Index] 
				where actionID = 3 
						and indexID not in (select distinct contractID from Trade T join [Index] I on T.contractID = I.indexID)
						and indexID not in (select indexID 
												from [Index] I 
														join OptionRoot ort on ort.underlyingID = i.indexID
														join [Option] o on ort.optionRootID = o.optionRootID
														join Trade T on T.contractID = O.optionID)
						and indexID not in (select distinct contractID from [Order] Ord join [Index] I on Ord.contractID = I.indexID)
						and indexID not in (select indexID 
												from [Index] I 
														join OptionRoot ort on ort.underlyingID = i.indexID
														join [Option] o on ort.optionRootID = o.optionRootID
														join [Order] Ord on Ord.contractID = O.optionID)
 			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--IndexCorrelation
		delete from IndexCorrelation 
			where indexID1 in (select contractID from #IDs) or indexID2 in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--IndexDataHistory
		delete from IndexDataHistory where indexID in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		--IndexDefinition
		delete from IndexDefinition where indexID in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--delete all related rows in Contract table by indexID
		if object_ID('tempdb..#Contracts_for_delete') is null
			Create table #Contracts_for_delete(contractID int)
		else
			Delete from #Contracts_for_delete
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		insert into #Contracts_for_delete
			select contractID from #IDs 
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		exec usp_Contract_Del Null,1
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Index
		delete from [Index] where indexID in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Contract
		delete from Contract where contractID in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		if object_id('tempdb..#IDs') is not null
			Drop table #IDs
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--=====================================================================================================================
		--STOCK
		delete StockDataHistory 
			where stockID in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade)
				and stockID not in (select distinct ContractID from [Order]))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		delete StockCorrelation 
			where stockID1 in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade)
				and stockID not in (select distinct ContractID from [Order]))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		delete StockCorrelation 
			where stockID2 in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade)
				and stockID not in (select distinct ContractID from [Order]))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		delete IndexDefinition 
			where stockID in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade)
				and stockID not in (select distinct ContractID from [Order]))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		if object_ID('tempdb..#Contracts_for_delete') is not null
			delete from #Contracts_for_delete
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		insert into #Contracts_for_delete
			select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade)
				and stockID not in (select distinct ContractID from [Order])
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end


		--VolaSurfaceData
		delete VolaSurfaceData where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		delete CustomStrikeSkewPoint 
			where volaSurfaceDataID in (select volaSurfaceDataID from VolaSurfaceData where contractID in (select contractID from #Contracts_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--VolaBands
		delete VolaBands 
			where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--VolaSurfaceData
		delete VolaSurfaceData 
			where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		exec usp_Contract_Del Null,1
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		delete from Stock where stockID in (select contractID from #Contracts_for_delete)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end


		delete from Contract where contractID in (select contractID from #Contracts_for_delete)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		--=====================================================================================================================
		--OPTION

		if object_ID('tempdb..#Options_for_delete') is null
			create table #Options_for_delete(OptionID int)
		else
			delete from #Options_for_delete
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		insert into #Options_for_delete 
			select optionID 
				from [Option] O
				where optionID not in (select distinct ContractID from Trade) and actionID = 3
					and optionID not in (select distinct ContractID from [Order])
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		exec usp_Option_Del null,1
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		--=====================================================================================================================

		--fix DB cleanup date	
		--update DataInfo set keyValue = GetDate() where dataInfoID = 6

		update DataInfo set keyValue = convert(varchar, GetDate(), 120) where dataInfoID = 6
		if @@ROWCOUNT = 0 
			begin 
				insert into DataInfo 
				    select 6,'lastCleanupDate',convert(varchar, GetDate(), 120)
	  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
			end 
--	Commit Transaction DB_Cleanup

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.121')
	begin
		update DataInfo set keyValue = '3.46.121' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.121' '3.46.121 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
