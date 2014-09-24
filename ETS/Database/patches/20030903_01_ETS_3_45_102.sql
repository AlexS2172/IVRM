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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.44.101' or (keyValue = '3.45.102')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.45.102 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.45.102' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/


----------------------------------------------------------------------------------------------------
-- remove all HH SPs
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BrokerHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BrokerHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BookHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CommissionHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_CommissionHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractExchangeLinkHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ContractExchangeLinkHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractForPriceFeedHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ContractForPriceFeedHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractGroupHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ContractGroupHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ContractHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInGroupHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ContractInGroupHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomStrikeSkewPointHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_CustomStrikeSkewPointHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DataInfoHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_DataInfoHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExchangeHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ExchangeHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExchangeHolidayHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ExchangeHolidayHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryCalendarHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ExpiryCalendarHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ExpiryHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryInCalendarHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ExpiryInCalendarHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_HolidayHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_HolidayHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexBetaHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_IndexBetaHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinitionHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_IndexDefinitionHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_IndexHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRCurveHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_IRCurveHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRPointHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_IRPointHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_OptionHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionRootHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_OptionRootHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RMScenarioHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_RMScenarioHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_StockHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StrategyHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_StrategyHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContractForHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderContractForHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContractHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderContractHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSeqHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeSeqHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VolaSurfaceDataHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_VolaSurfaceDataHH_Get]
GO

----------------------------------------------------------------------------------------------------
-- add export symbol to Contract table
----------------------------------------------------------------------------------------------------
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Contract' and COLUMN_NAME='exportSymbol')
begin
	ALTER TABLE [dbo].[Contract] ADD [exportSymbol] varchar(20) null
end
GO

----------------------------------------------------------------------------------------------------
-- ContractView
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ContractView]') and OBJECTPROPERTY(id, N'IsView') = 1)
	drop view [dbo].[ContractView]
GO

CREATE VIEW dbo.ContractView
as
	select 
		contractID,
		contractTypeID,
		symbol,
		contractName,
		ruleID,
		groupID,
		actionID,
		actionDate,
		expCalendarID,
		MSupdateDate,
		undPriceProfileID,
		optPriceProfileID,
		exportSymbol
	from
		Contract
	where
		actionID < 3

GO

----------------------------------------------------------------------------------------------------
-- Add TraderGroup
----------------------------------------------------------------------------------------------------
if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TraderGroup]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
begin
	CREATE TABLE [dbo].[TraderGroup]
	(
		[trgID] [int] IDENTITY (1, 1) NOT NULL ,
		[trgName] [varchar] (20) NOT NULL ,
		[trgDesc] [varchar] (255) NULL,
		[actionDate] [datetime] NOT NULL CONSTRAINT [DF_TraderGroup_actionDate] DEFAULT (getdate()),
		CONSTRAINT [PK_TraderGroup] PRIMARY KEY  CLUSTERED 
			(
				[trgID]
			)
	)
end
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_TraderGroup]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
	drop trigger [dbo].[tU_TraderGroup]
GO

CREATE    Trigger dbo.tU_TraderGroup ON [dbo].[TraderGroup]
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	-- 1st step - update actionDate if record not already deleted
	update TraderGroup
		set actionDate = GetDate()
	from TraderGroup T 
		join inserted on T.trgID = inserted.trgID

GO
	
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trader' and COLUMN_NAME='traderGroupID')
	ALTER TABLE [dbo].[Trader] ADD [traderGroupID] int null
GO

if (select OBJECT_ID('FK_Trader_TraderGroup')) is null
	ALTER TABLE [dbo].[Trader] ADD CONSTRAINT [FK_Trader_TraderGroup]
	FOREIGN KEY ([traderGroupID])  REFERENCES [dbo].[TraderGroup] ([trgID]) 
GO

if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trader' and COLUMN_NAME='defStrategyID')
	ALTER TABLE [dbo].[Trader] ADD [defStrategyID] int null
GO

if (select OBJECT_ID('FK_Trader_DefStrategyID')) is null
	ALTER TABLE [dbo].[Trader] ADD CONSTRAINT [FK_Trader_DefStrategyID]
	FOREIGN KEY ([defStrategyID])  REFERENCES [dbo].[Strategy] ([strategyID]) 
GO

----------------------------------------------------------------------------------------------------
-- usp_TraderGroup_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderGroup_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderGroup_Get]
GO

CREATE PROC dbo.usp_TraderGroup_Get
	@iTraderGroupID int = null
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------

	if @iTraderGroupID is null
		select
			trgID as iTraderGroupID,
			trgName as vcGroupName,
			trgDesc as vcGroupDesc,
			actionDate as dtActionDate
		from TraderGroup
	else
		select
			trgID as iTraderGroupID,
			trgName as vcGroupName,
			trgDesc as vcGroupDesc,
			actionDate as dtActionDate
		from TraderGroup
		where trgID = @iTraderGroupID

	return @@error

GO

----------------------------------------------------------------------------------------------------
-- usp_BoTraderGroup_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTraderGroup_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTraderGroup_Get]
GO

CREATE    Procedure dbo.usp_BoTraderGroup_Get 
	@iTraderGroupID int = null
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------

	if @iTraderGroupID is null
		select 	trgID as iTraderGroupID,
			trgName as vcGroupName,
			trgDesc as vcGroupDesc,
			actionDate as dtActionDate
		from TraderGroup
	else
		select 	trgID as iTraderGroupID,
			trgName as vcGroupName,
			trgDesc as vcGroupDesc,
			actionDate as dtActionDate
		from TraderGroup
		where trgID=@iTraderGroupID

	return @@error

GO

----------------------------------------------------------------------------------------------------
-- usp_BoTraderGroup_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTraderGroup_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTraderGroup_Save]
GO

CREATE PROCEDURE dbo.usp_BoTraderGroup_Save
	@iTraderGroupID int,
	@vcGroupName varchar(20),
	@vcGroupDesc varchar(255),
	@tiRaiseError tinyint = 1
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	
	if exists (select * from TraderGroup where trgName = @vcGroupName and (@iTraderGroupID is null or trgID <> @iTraderGroupID))
		begin
			if isnull(@tiRaiseError, 0) <> 0 raiserror ('Trader group with the same name already exists!', 16, 1)
			set @error = -1
			goto finish
		end
		
	Begin Transaction

	if @iTraderGroupID is null
	begin
		insert into TraderGroup 
			(trgName, trgDesc) 
		values 
			(@vcGroupName, @vcGroupDesc)

		if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

		set @iTraderGroupID = @@identity
	end
	else
	begin
		update TraderGroup 
		set trgName = isnull(@vcGroupName, trgName),
			trgDesc = isnull(@vcGroupDesc, trgDesc)
		where trgID = @iTraderGroupID

		if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
	end

	Commit Transaction

finish:
	if @error = 0
		return @iTraderGroupID
	else
		return @error

GO

----------------------------------------------------------------------------------------------------
-- usp_BoTraderGroup_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTraderGroup_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTraderGroup_Del]
GO

CREATE   Procedure dbo.usp_BoTraderGroup_Del
	@iTraderGroupID int
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	
	delete TraderGroup 
		where trgID = @iTraderGroupID
		
	set @error = @@error

Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- remove TraderRole
----------------------------------------------------------------------------------------------------
IF EXISTS (SELECT name
	   FROM   sysobjects 
	   WHERE  name = N'TraderView' 
	   AND 	  type = 'V')
    DROP VIEW TraderView
GO

if (select OBJECT_ID('FK_Trader_TraderRole')) is not null
	ALTER TABLE [dbo].[Trader] DROP CONSTRAINT [FK_Trader_TraderRole]
GO

if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trader' and COLUMN_NAME='traderRoleID')
	ALTER TABLE [dbo].[Trader] DROP COLUMN [traderRoleID]
GO

if (select OBJECT_ID('FK_Trader_Trader')) is not null
	ALTER TABLE [dbo].[Trader] DROP CONSTRAINT [FK_Trader_Trader]
GO

if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trader' and COLUMN_NAME='traderRefID')
	ALTER TABLE [dbo].[Trader] DROP COLUMN [traderRefID]
GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[dbo].[usp_TraderRole_Get]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROCEDURE [dbo].[usp_TraderRole_Get]
GO 

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[dbo].[usp_TraderRole_Save]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROCEDURE [dbo].[usp_TraderRole_Save]
GO 

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderRM_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderRM_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderRM_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderRM_Save]
GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[dbo].[TraderRoleView]') 
	   AND 	  OBJECTPROPERTY(id, N'IsView') = 1)
    DROP VIEW [dbo].[TraderRoleView]
GO 

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[dbo].[TraderRole]') 
	   AND 	  OBJECTPROPERTY(id, N'IsUserTable') = 1)
    DROP TABLE [dbo].[TraderRole]
GO 

CREATE VIEW dbo.TraderView
as
	Select 
		traderID,
		acronym,
		traderName,
		clearingTraderCode,
		traderGroupID,
		actionID,
		actionDate,
		tntCardLastID,
		tntCardLastDate,
		defStrategyID
	from Trader
	where actionID < 3

GO

----------------------------------------------------------------------------------------------------
-- remove BookRuleItemMask
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItemMask_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BookRuleItemMask_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItemMask_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BookRuleItemMask_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItemMask_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BookRuleItemMask_Save]
GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[dbo].[BookRuleItemMask]') 
	   AND 	  OBJECTPROPERTY(id, N'IsUserTable') = 1)
    DROP TABLE [dbo].[BookRuleItemMask]
GO 

----------------------------------------------------------------------------------------------------
-- remove BookRuleItem
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRule_GetAll]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BookRule_GetAll]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItem_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BookRuleItem_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItem_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BookRuleItem_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRuleItem_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BookRuleItem_Save]
GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[dbo].[BookRuleItem]') 
	   AND 	  OBJECTPROPERTY(id, N'IsUserTable') = 1)
    DROP TABLE [dbo].[BookRuleItem]
GO 

----------------------------------------------------------------------------------------------------
-- remove RuleItemType
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RuleItemType_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_RuleItemType_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RuleItemType_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_RuleItemType_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RuleItemType_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_RuleItemType_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[RuleItemType]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [RuleItemType]
GO

----------------------------------------------------------------------------------------------------
-- remove Book
----------------------------------------------------------------------------------------------------
if (select OBJECT_ID('FK_Trade_Books')) is not null
	ALTER TABLE [dbo].[Trade] DROP CONSTRAINT [FK_Trade_Books]
GO
if exists (select * from dbo.sysindexes where name = N'I_Trades_Book' and id = object_id(N'[dbo].[Trade]'))
	drop index [dbo].[Trade].[I_Trades_Book]
GO
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='bookID')
	ALTER TABLE [dbo].[Trade] DROP COLUMN [bookID]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Book_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Book_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Book_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Book_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Book_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Book_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookSwap_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BookSwap_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[BookView]') and OBJECTPROPERTY(id, N'IsView') = 1)
	drop view [dbo].[BookView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[Book]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [Book]
GO

----------------------------------------------------------------------------------------------------
-- remove clearing broker commission ID, broker commission ID and isClose
-- fields from Trade table and add broker commission and clearing broker commission field
----------------------------------------------------------------------------------------------------

if object_id('FK_Trade_Commission') is not null
	ALTER TABLE [dbo].[Trade] DROP CONSTRAINT [FK_Trade_Commission]
GO

if object_id('DF__Trades__BrokerCo__3BFFE745') is not null
	ALTER TABLE [dbo].[Trade] DROP CONSTRAINT [DF__Trades__BrokerCo__3BFFE745]
GO

if object_id('DF__Trades__Clearing__3CF40B7E') is not null
	ALTER TABLE [dbo].[Trade] DROP CONSTRAINT [DF__Trades__Clearing__3CF40B7E]
GO

if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='brokerCommissionID')
	ALTER TABLE [dbo].[Trade] DROP COLUMN [brokerCommissionID]
GO

if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='brokerCommission')
	ALTER TABLE [dbo].[Trade] ADD [brokerCommission] float null
GO

if object_id('FK_Trade_Commission1') is not null
	ALTER TABLE [dbo].[Trade] DROP CONSTRAINT [FK_Trade_Commission1]
GO

if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='clearingBrokerCommissionID')
	ALTER TABLE [dbo].[Trade] DROP COLUMN [clearingBrokerCommissionID]
GO

if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='clearingBrokerCommission')
	ALTER TABLE [dbo].[Trade] ADD [clearingBrokerCommission] float null
GO

if object_id('CHK_IsClose') is not null
	ALTER TABLE [dbo].[Trade] DROP CONSTRAINT [CHK_IsClose]
GO

if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='isClose')
	ALTER TABLE [dbo].[Trade] DROP COLUMN [isClose]
GO

if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='mark')
	ALTER TABLE [dbo].[Trade] ADD [mark] int null
GO

----------------------------------------------------------------------------------------------------
-- TradeView
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TradeView]') and OBJECTPROPERTY(id, N'IsView') = 1)
	drop view [dbo].[TradeView]
GO

CREATE VIEW dbo.TradeView
AS
	select
	 	tradeID,
		contractID, 
		tradeDate, 
		quantity, 
		price, 
		isBuy, 
		traderID, 
		strategyID, 
		brokerID, 
		brokerCommission, 
		clearingBrokerID, 
		clearingBrokerCommission, 
		tradedIV, 
		spotReference,
		isPosition,
		status,
		execID,
		mark
	from 
		dbo.Trade
	group by
		tradeID, contractID, tradeDate, quantity, price, 
		isBuy, traderID, strategyID, brokerID, brokerCommission,
		clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference,isPosition,status,execID, mark
	having
		(min(actionID) = 1)

GO

----------------------------------------------------------------------------------------------------
-- remove Commission
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = N'I_CommissionBrokerDate' and id = object_id(N'[dbo].[Commission]'))
	drop index [dbo].[Commission].[I_CommissionBrokerDate]
GO

if object_id('DF_Commission_MDate') is not null
	ALTER TABLE [dbo].[Commission] DROP CONSTRAINT [DF_Commission_MDate]
GO

if object_id('DF_Commission_MType') is not null
	ALTER TABLE [dbo].[Commission] DROP CONSTRAINT [DF_Commission_MType]
GO

if object_id('CK_Commission_action') is not null
	ALTER TABLE [dbo].[Commission] DROP CONSTRAINT [CK_Commission_action]
GO

if object_id('FK_Commission_Brokers') is not null
	ALTER TABLE [dbo].[Commission] DROP CONSTRAINT [FK_Commission_Brokers]
GO

if object_id('PK_Commissions') is not null
	ALTER TABLE [dbo].[Commission] DROP CONSTRAINT [PK_Commissions]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_Commission]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
	drop trigger [dbo].[tU_Commission]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Commission_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Commission_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Commission_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Commission_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Commission_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Commission_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[CommissionView]') and OBJECTPROPERTY(id, N'IsView') = 1)
	drop view [dbo].[CommissionView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[Commission]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[Commission]
GO

----------------------------------------------------------------------------------------------------
-- tU_Broker
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_Broker]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
	drop trigger [dbo].[tU_Broker]
GO

CREATE TRIGGER dbo.tU_Broker ON dbo.Broker
    FOR Update
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	--======================================================================
	-- fix row modifications
	--======================================================================
	-- 1st step - update actionDate if record not already deleted
	update Broker 
		set actionDate = GetDate()
	from Broker B 
			join inserted on B.BrokerID = inserted.BrokerID
			join deleted on B.BrokerID = deleted.BrokerID
	where deleted.actionID < 3

	-- 2nd step - if update record -> set actionID=2
	update Broker
		set actionID=2
	from Broker B
			join inserted on B.BrokerID = inserted.BrokerID
			join deleted on B.BrokerID = deleted.BrokerID
	where deleted.actionID<3 and inserted.actionID = 1

	-- 3d step -disable restore of deleted record
	update Broker 
		set actionID = 3
	from Broker B
			join inserted on B.BrokerID = inserted.BrokerID
			join deleted on B.BrokerID = deleted.BrokerID
	where deleted.actionID=3
GO

----------------------------------------------------------------------------------------------------
-- Broker
----------------------------------------------------------------------------------------------------
if object_id('CHK_IsClearing') is not null
	ALTER TABLE [dbo].[Broker] DROP CONSTRAINT [CHK_IsClearing]
GO

if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Broker' and COLUMN_NAME='undComm')
	ALTER TABLE [dbo].[Broker] ADD [undComm] float null
GO

if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Broker' and COLUMN_NAME='optRegComm')
	ALTER TABLE [dbo].[Broker] ADD [optRegComm] float null
GO

if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Broker' and COLUMN_NAME='optSpreadComm')
	ALTER TABLE [dbo].[Broker] ADD [optSpreadComm] float null
GO

if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Broker' and COLUMN_NAME='lowPremComm')
	ALTER TABLE [dbo].[Broker] ADD [lowPremComm] float null
GO

----------------------------------------------------------------------------------------------------
-- BrokerView
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[BrokerView]') and OBJECTPROPERTY(id, N'IsView') = 1)
	drop view [dbo].[BrokerView]
GO

CREATE View dbo.BrokerView
as
	Select 
		brokerID,
		brokerName,
		[description],
		isClearing,
		actionID,
		actionDate,
		undComm,
		optRegComm,
		optSpreadComm,
		lowPremComm
	from Broker
	where actionID < 3
GO

----------------------------------------------------------------------------------------------------
-- remove usp_BrokerRate_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BrokerRate_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BrokerRate_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_Broker_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Broker_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Broker_Del]
GO

CREATE  procedure dbo.usp_Broker_Del
	@iBrokerID int,
	@tiIsRaiseError tinyint = 1 
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	if exists (select 1 from Trade where brokerID = @iBrokerID or clearingBrokerID = @iBrokerID)
		begin
			if isnull(@tiIsRaiseError, 0) <> 0 raiserror('Can''t remove broker, because this broker have a trades or position!',16,1)
			set @error = -1
			goto finish
		end
	
	Begin Transaction

		update Broker set actionID = 3 where brokerID = @iBrokerID
			if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

	Commit Transaction

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_Broker_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Broker_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Broker_Get]
GO

CREATE PROC dbo.usp_Broker_Get
	@iBrokerID int = Null
as
	------------------------------------------------
	set nocount on
	------------------------------------------------

	if @iBrokerID is Null
		select
			brokerID as iBrokerID,
			brokerName as vcBrokerName,
			[Description] as vcDescription,
			isClearing as tiIsClearing,
			actionDate as dtActionDate,
			actionID as tiActionID,
			undComm as fUndComm,
			optRegComm as fOptRegComm,
			optSpreadComm as fOptSpreadComm,
			lowPremComm as fLowPremComm
		from
			BrokerView Broker
		order by BrokerName
	else	
		select
			brokerID as iBrokerID,
			brokerName as vcBrokerName,
			[Description] as vcDescription,
			isClearing as tiIsClearing,
			actionDate as dtActionDate,
			actionID as tiActionID,
			undComm as fUndComm,
			optRegComm as fOptRegComm,
			optSpreadComm as fOptSpreadComm,
			lowPremComm as fLowPremComm
		from
			BrokerView Broker
		where 
			brokerID = @iBrokerID

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_Broker_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Broker_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Broker_Save]
GO

CREATE PROC dbo.usp_Broker_Save
	@iBrokerID int,
	@vcBrokerName varchar(12) = null,
	@vcDescription varchar(120) = null,
	@tiIsClearing tinyint = null,
	@fUndComm float = null,
	@fOptRegComm float = null,
	@fOptSpreadComm float = null,
	@fLowPremComm float = null,
	@tiIsRaiseError tinyint = 1 
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	if exists (select 1 from BrokerView where BrokerName = @vcBrokerName and (@iBrokerID <> brokerID or @iBrokerID is null))
	begin
		if isnull(@tiIsRaiseError, 0) <> 0 raiserror ('Broker with this name already exists!', 16, 1)
		set @error = -1
		goto finish
	end   

	if @iBrokerID is not null and @tiIsClearing is not null
	begin
		declare @tiOldIsClearing tinyint

		select @tiOldIsClearing = isnull(isClearing, 0)
		from BrokerView
		where brokerID = @iBrokerID

		if @tiOldIsClearing <> @tiIsClearing
		begin
			if @tiIsClearing = 0
			begin
				if exists (select 1 from Trade where clearingbrokerID = @iBrokerID)
				begin
					if isnull(@tiIsRaiseError, 0) <> 0 raiserror ('Can''t set broker as not clearing broker because he is assigned as clearing broker for some trades or positions!', 16, 1)
					set @error = -2
					goto finish
				end
			end
			else
			begin
				if exists (select 1 from Trade where brokerID = @iBrokerID)
				begin
					if isnull(@tiIsRaiseError, 0) <> 0 raiserror ('Can''t set broker as clearing broker because he is assigned as broker for some trades or positions!', 16, 1)
					set @error = -3
					goto finish
				end
			end
		end
	end

	begin transaction

	if @iBrokerID is null 
	begin
		insert into Broker
			(BrokerName, [Description], isClearing, undComm, optRegComm, optSpreadComm, lowPremComm)
		values
			(@vcBrokerName, @vcDescription, @tiIsClearing, @fUndComm, @fOptRegComm, @fOptSpreadComm, @fLowPremComm)

		if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end
	   
		set @iBrokerID = @@identity
	end
	else
	begin 
		update Broker
		set BrokerName = isnull(@vcBrokerName, BrokerName),
			[Description] = isnull(@vcDescription, [Description]),
			isClearing = isnull(@tiIsClearing, isClearing),
			undComm = isnull(@fUndComm, undComm),
			optRegComm = isnull(@fOptRegComm, optRegComm),
			optSpreadComm = isnull(@fOptSpreadComm, optSpreadComm),
			lowPremComm = isnull(@fLowPremComm, lowPremComm)
		where brokerID = @iBrokerID

		if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end
	end 		 

	commit transaction

finish:
	if @error = 0
		return @iBrokerID
	else
		return @error

GO

----------------------------------------------------------------------------------------------------
-- usp_CommissionLowPremiumLimit_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CommissionLowPremiumLimit_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_CommissionLowPremiumLimit_Get]
GO

CREATE PROCEDURE dbo.usp_CommissionLowPremiumLimit_Get
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------

	select convert(float, KeyValue) as fCommissionLowPremiumLimit
		from DataInfo	
		where dataInfoID = 12

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_CommissionLowPremiumLimit_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CommissionLowPremiumLimit_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_CommissionLowPremiumLimit_Save]
GO

CREATE PROCEDURE dbo.usp_CommissionLowPremiumLimit_Save
	@fCommissionLowPremiumLimit float
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	begin tran

	if exists(select * from DataInfo where dataInfoID = 12)
		update DataInfo set keyValue = @fCommissionLowPremiumLimit
			where dataInfoID = 12
	else
		insert into DataInfo (dataInfoID, keyName, keyValue) 
		values (12, 'CommissionLowPremiumLimit', @fCommissionLowPremiumLimit)

	if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

	commit tran
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- add default commission low premium limit value
----------------------------------------------------------------------------------------------------
begin	
	if not exists(select * from DataInfo where dataInfoID = 12)
	begin
		insert into DataInfo (dataInfoID, keyName, keyValue) values (12, 'CommissionLowPremiumLimit', 1)
	end
end
GO

----------------------------------------------------------------------------------------------------
-- usp_ContractGroup_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractGroup_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ContractGroup_Get]
GO

CREATE PROC dbo.usp_ContractGroup_Get
	@iGroupID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iGroupID is Null
		select groupID as iGroupID, 
			groupName as vcGroupName, 
			[Description] as vcDescription,
			actionDate as dtActionDate,
			actionID as tiActionID
		from ContractGroupView
		order by groupName
	else 		 
		select groupID as iGroupID, 
			groupName as vcGroupName, 
			[Description] as vcDescription,
			actionDate as dtActionDate,
			actionID as tiActionID
		from ContractGroupView
		where groupID = @iGroupID

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_ContractInGroup_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInGroup_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ContractInGroup_Get]
GO

CREATE PROC dbo.usp_ContractInGroup_Get
	@iGroupID int = null,
	@iContractID int = null
as
	------------------------------------------------
	set nocount on
	------------------------------------------------

	if @iGroupID is null
	begin
		if @iContractID is null
			select
				groupID as iGroupID, 
				contractID as iContractID,
				actionDate as dtActionDate,
				actionID as tiActionID
			from ContractInGroupView
		else		
			select
				groupID as iGroupID, 
				contractID as iContractID,
				actionDate as dtActionDate,
				actionID as tiActionID
			from ContractInGroupView
			where contractID = @iContractID
	end
	else
	begin
		if @iContractID is null
			select
				groupID as iGroupID, 
				contractID as iContractID,
				actionDate as dtActionDate,
				actionID as tiActionID
			from ContractInGroupView
			where groupID = @iGroupID
		else		
			select
				groupID as iGroupID, 
				contractID as iContractID,
				actionDate as dtActionDate,
				actionID as tiActionID
			from ContractInGroupView
			where groupID = @iGroupID
				and contractID = @iContractID
	end

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_ExpiryCalendar_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryCalendar_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ExpiryCalendar_Get]
GO

CREATE PROC dbo.usp_ExpiryCalendar_Get 
	@iExpCalendarID int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iExpCalendarID is null
		select 
			expCalendarID as iExpCalendarID,
			expCalendarName as vcExpCalendarName,
			expCalendarDesc as vcExpCalendarDesc
		from ExpiryCalendarView 
	else
		select 
			expCalendarID as iExpCalendarID,
			expCalendarName as vcExpCalendarName,
			expCalendarDesc as vcExpCalendarDesc
		from ExpiryCalendarView 
		where expCalendarID = @iExpCalendarID

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_ExpiryInCalendar_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryInCalendar_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ExpiryInCalendar_Get]
GO

CREATE PROC dbo.usp_ExpiryInCalendar_Get
	@iExpiryID int = null,
	@iExpCalendarID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iExpCalendarID is null
	begin
		if @iExpiryID is null
			select
				expCalendarID as iExpCalendarID,
				expiryID as iExpiryID, 
				expiryDate as dtExpiryDate, 
				vegaWeight as fVegaWeight, 
				actionDate as dtActionDate,
				actionID as tiActionID
			from ExpiryInCalendarView
		else
			select
				expCalendarID as iExpCalendarID,
				expiryID as iExpiryID, 
				expiryDate as dtExpiryDate, 
				vegaWeight as fVegaWeight, 
				actionDate as dtActionDate,
				actionID as tiActionID
			from ExpiryInCalendarView EC 
			where expiryID = @iExpiryID
	end
	else
	begin
		if @iExpiryID is null
			select
				expCalendarID as iExpCalendarID,
				expiryID as iExpiryID, 
				expiryDate as dtExpiryDate, 
				vegaWeight as fVegaWeight, 
				actionDate as dtActionDate,
				actionID as tiActionID
			from ExpiryInCalendarView
			where expCalendarID = @iExpCalendarID
		else
			select
				expCalendarID as iExpCalendarID,
				expiryID as iExpiryID, 
				expiryDate as dtExpiryDate, 
				vegaWeight as fVegaWeight, 
				actionDate as dtActionDate,
				actionID as tiActionID
			from ExpiryInCalendarView EC 
			where expCalendarID = @iExpCalendarID
				and expiryID = @iExpiryID
	end

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_ExpiryVegaWeightSave_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExpiryVegaWeightSave_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ExpiryVegaWeightSave_Save]
GO

CREATE PROC dbo.usp_ExpiryVegaWeightSave_Save
	@iExpiryID int,
	@iExpCalendarID int,
	@fVegaWeight float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	begin tran

	update ExpiryInCalendar
	set vegaWeight = @fVegaWeight
	where expCalendarID = @iExpCalendarID
		and expiryID = @iExpiryID

	if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

	commit tran
finish:
	return @error

GO

----------------------------------------------------------------------------------------------------
-- add strategy ID to TraderContract table
----------------------------------------------------------------------------------------------------
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='TraderContract' and COLUMN_NAME='strategyID')
begin
	ALTER TABLE [dbo].[TraderContract] ADD [strategyID] int null
end
GO

if (select OBJECT_ID('FK_TraderContract_StrategyID')) is null
	ALTER TABLE [dbo].[TraderContract] ADD CONSTRAINT [FK_TraderContract_StrategyID] FOREIGN KEY
	(
		[strategyID]
	)  
	REFERENCES [dbo].[Strategy]
	(
		[strategyID]
	) 
GO

----------------------------------------------------------------------------------------------------
-- TraderContractView
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TraderContractView]') and OBJECTPROPERTY(id, N'IsView') = 1)
	drop view [dbo].[TraderContractView]
GO

CREATE VIEW dbo.TraderContractView
AS
SELECT     
	traderID, 
	traderContractID, 
	contractID, 
	actionID, 
	actionDate,
	strategyID 
FROM         
	dbo.TraderContract
WHERE     
	(actionID < 3)
GO

----------------------------------------------------------------------------------------------------
-- remove usp_TraderContract_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContract_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderContract_Del]
GO

----------------------------------------------------------------------------------------------------
-- usp_TraderContract_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderContract_Get]
GO

CREATE PROC dbo.usp_TraderContract_Get
	@iContractID int = null,
	@iTraderID int = null
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	
	if @iContractID is null 
	begin
		if @iTraderID is null
			select 
				traderID as iTraderID,
				contractID as iContractID,
				strategyID as iStrategyID
			from TraderContractView
		else
			select 
				traderID as iTraderID,
				contractID as iContractID,
				strategyID as iStrategyID
			from TraderContractView
			where 
				traderID = @iTraderID
	end
	else
	begin
		if @iTraderID is null
			select 
				traderID as iTraderID,
				contractID as iContractID,
				strategyID as iStrategyID
			from TraderContractView
			where
				contractID = @iContractID
		else
			select 
				traderID as iTraderID,
				contractID as iContractID,
				strategyID as iStrategyID
			from TraderContractView
			where 
				traderID = @iTraderID
				and contractID = @iContractID
	end

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_TraderContractByTrader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContractByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderContractByTrader_Get]
GO

CREATE PROC dbo.usp_TraderContractByTrader_Get
	@iTraderID int = null
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	
	if @iTraderID is null
		select 
			traderID as iTraderID,
			contractID as iContractID,
			strategyID as iStrategyID
		from TraderContractView
	else
		select 
			traderID as iTraderID,
			contractID as iContractID,
			strategyID as iStrategyID
		from TraderContractView
		where 
			traderID = @iTraderID

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_TraderContract_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContract_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderContract_Save]
GO

CREATE PROC dbo.usp_TraderContract_Save
	@iContractID int,
	@iTraderID int,
	@iStrategyID int -- set to 'null' to remove strategy association
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------

	if exists(select 1 from TraderContractView where traderID = @iTraderID and contractID = @iContractID)
		update TraderContract
			set strategyID = @iStrategyID
		where
			traderID = @iTraderID 
			and contractID = @iContractID
			and actionID < 3
	else
		insert into TraderContract
			(traderID, contractID, strategyID)
		values
			(@iTraderID, @iContractID, @iStrategyID)

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTrader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTrader_Get]
GO

CREATE PROC dbo.usp_BoTrader_Get
	@iTraderID int = Null
  as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	
    if @iTraderID is Null
    	select t.traderID as iTraderID, 
			t.acronym as vcAcronym, 
			t.traderName as vcTraderName, 
			t.clearingTraderCode as vcClearingTraderCode,
			t.traderGroupID as iTraderGroupID,
			tg.trgName as vcGroupName,
			t.defStrategyID as iDefStrategyID,
			st.strategyName as vcStrategyName
		from TraderView t
			left join TraderGroup tg on t.traderGroupID = tg.trgID
			left join StrategyView st on t.defStrategyID = st.strategyID
		order by t.acronym
    else
    	select t.traderID as iTraderID, 
			t.acronym as vcAcronym, 
			t.traderName as vcTraderName, 
			t.clearingTraderCode as vcClearingTraderCode,
			t.traderGroupID as iTraderGroupID,
			tg.trgName as vcGroupName,
			t.defStrategyID as iDefStrategyID,
			st.strategyName as vcStrategyName
		from TraderView t
			left join TraderGroup tg on t.traderGroupID = tg.trgID
			left join StrategyView st on t.defStrategyID = st.strategyID
		where
			t.traderID = @iTraderID

	return @@error

GO

----------------------------------------------------------------------------------------------------
-- remove usp_Trader_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trader_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trader_Del]
GO

----------------------------------------------------------------------------------------------------
-- remove usp_Trader_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trader_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trader_Save]
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTrader_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTrader_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTrader_Del]
GO

CREATE  PROCEDURE dbo.usp_BoTrader_Del
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
        update TraderContract set actionID=3 where traderID = @iTraderID
			if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end
			
        update Trader set actionID=3, traderGroupID=null where traderID = @iTraderID
			if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction

finish:
	Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_BoTrader_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTrader_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTrader_Save]
GO

CREATE PROC dbo.usp_BoTrader_Save
    @iTraderID int,
    @vcAcronym varchar(12) = null,
    @vcTraderName varchar(120) = null,
    @vcClearingTraderCode varchar(11) = null,
    @iTraderGroupID int = null, -- null group == Default group !!!
    @iDefStrategyID int = null, -- null strategy == no default strategy
	@tiRaiseError tinyint = 1
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if exists (select 1 from TraderView where TraderName = @vcTraderName and (@iTraderID is null or @iTraderID <> traderID))
		begin
			if isnull(@tiRaiseError, 0) <> 0 raiserror ('Trader with this name already exists!', 16, 1)
			set @error = -1
			goto finish
		end
   
	if exists (select 1 from TraderView where Acronym = @vcAcronym and (@iTraderID is null or @iTraderID <> traderID))
		begin
			if isnull(@tiRaiseError, 0) <> 0 raiserror ('Trader with this acronym already exists!', 16, 1)
			set @error = -2
			goto finish
		end   

	begin transaction
	
	if @iTraderID is null
	begin
		-- insert new trader
		insert into Trader (Acronym, TraderName, clearingTraderCode, traderGroupID, defStrategyID)
			values (@vcAcronym, @vcTraderName, @vcClearingTraderCode, @iTraderGroupID, @iDefStrategyID)

		if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

		set @iTraderID = @@identity
	end
	else
	begin
		-- check if default strategy changed
		declare @iOldDefStrategyID int
		
		select @iOldDefStrategyID = isnull(defStrategyID, 0)
			from Trader
			where traderID = @iTraderID

		if @iOldDefStrategyID <> isnull(@iDefStrategyID, 0)
		begin
			-- update underlyings trades' strategy
			update Trade
				set strategyID = @iDefStrategyID
			from Trade T 
				join TraderContractView TC on TC.contractID=T.contractID and TC.traderID=T.traderID
			where T.traderID = @iTraderID
				and TC.strategyID is null

			if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end
			
			-- update options trades' strategy
			update Trade
				set strategyID = @iDefStrategyID
			from Trade T 
				join OptionView O on O.optionID=T.contractID
				join OptionRootView ORT on ORT.optionRootID=O.optionRootID
				join TraderContractView TC on TC.contractID=ORT.underlyingID and TC.traderID=T.traderID
			where T.traderID = @iTraderID
				and TC.strategyID is null
				
			if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end
		end

		-- update trader
		update Trader 
        set acronym = isnull(@vcAcronym, acronym),
	        traderName = isnull(@vcTraderName, traderName),
			clearingTraderCode = isnull(@vcClearingTraderCode, clearingTraderCode),
			traderGroupID = @iTraderGroupID,
			defStrategyID = @iDefStrategyID
	    where traderID = @iTraderID

		if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end
	end

	commit transaction

finish:
	if @error = 0
		return @iTraderID
	else
		return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_Trader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trader_Get]
GO

CREATE PROCEDURE dbo.usp_Trader_Get
	@iTraderID int = Null
as
	------------------------------------------------
	set nocount on
	------------------------------------------------

	if @iTraderID is Null
		select
			traderID as iTraderID, 
			acronym as vcAcronym, 
			traderName as vcTraderName, 
			clearingTraderCode as vcClearingTraderCode,
			actionDate as dtActionDate,
			tntCardLastID as iTntCardLastID,
			tntCardLastDate as dtTntCardLastDate,
			traderGroupID as iTraderGroupID,
			defStrategyID as iDefStrategyID
		from TraderView
		order by acronym
	else 
		select
			traderID as iTraderID, 
			acronym as vcAcronym, 
			traderName as vcTraderName, 
			clearingTraderCode as vcClearingTraderCode,
			actionDate as dtActionDate,
			tntCardLastID as iTntCardLastID,
			tntCardLastDate as dtTntCardLastDate,
			traderGroupID as iTraderGroupID,
			defStrategyID as iDefStrategyID
		from TraderView
		where traderID = @iTraderID
		order by acronym

	return @@error

GO

----------------------------------------------------------------------------------------------------
-- usp_BoTraderInGroup_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTraderInGroup_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTraderInGroup_Get]
GO

CREATE PROCEDURE dbo.usp_BoTraderInGroup_Get
	@iTraderGroupID int
  as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	
   	select Trader.traderID as iTraderID, 
		Trader.acronym as vcAcronym, 
		Trader.traderName as vcTraderName 
	from TraderView Trader
		left join TraderGroup on Trader.traderGroupID=TraderGroup.trgID
	where TraderGroup.trgID = @iTraderGroupID
	order by Trader.acronym

	return @@error

GO

----------------------------------------------------------------------------------------------------
-- usp_BoTraderUnderlyings_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTraderUnderlyings_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTraderUnderlyings_Get]
GO

CREATE PROCEDURE dbo.usp_BoTraderUnderlyings_Get
	@iTraderID int,
	@tiOnlySelected tinyint = 1
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	
	if @tiOnlySelected = 0
	begin
		select 
			C.contractID as iContractID,
			C.symbol as vcUndSymbol,
			max(case when(TC.traderID = @iTraderID) then 1 else 0 end) as tiIsSelected,
			max(case when(TC.traderID = @iTraderID) then isnull(tc.strategyID,0) else 0 end) as iStrategyID,
			max(case when(TC.traderID = @iTraderID) then isnull(S.strategyName,'') else '' end) as vcStrategyName
		from ContractView C
			left join TraderContractView TC on TC.contractID = C.contractID 
			left join StrategyView S on S.strategyID = TC.strategyID 
		group by 
			C.contractID, C.symbol, C.contractTypeID
		having 
			C.contractTypeID < 3
		order by tiIsSelected desc, C.symbol
	end
	else
	begin
		select distinct
			TC.contractID as iContractID,
			C.symbol as vcUndSymbol,
			1 as tiIsSelected,
			tc.strategyID as iStrategyID,
			S.strategyName as vcStrategyName
		from TraderContractView TC
			left join ContractView C on TC.contractID = C.contractID 
			left join StrategyView S on S.strategyID = TC.strategyID 
		where C.contractTypeID < 3 and TC.traderID=@iTraderID
		order by C.symbol
	end	
	return @@error

GO

----------------------------------------------------------------------------------------------------
-- remove usp_ContractPositionForRisk_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPositionForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ContractPositionForRisk_Get]
GO

----------------------------------------------------------------------------------------------------
-- remove usp_ContractSymbolForRisk_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractSymbolForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ContractSymbolForRisk_Get]
GO

----------------------------------------------------------------------------------------------------
-- remove usp_Position_ByUnderlyingForRisk_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Position_ByUnderlyingForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Position_ByUnderlyingForRisk_Get]
GO


----------------------------------------------------------------------------------------------------
-- usp_Strategy_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Strategy_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Strategy_Del]
GO

CREATE PROC dbo.usp_Strategy_Del
    @iStrategyID int,
	@tiIsRaiseError tinyint = 1 
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	if exists(select * from Trade where strategyID = @iStrategyID)
		begin
			if @tiIsRaiseError = 1 Raiserror('Can''t remove strategy, because this strategy has a trades!',16,1)
			Return(-1)
		end

	if exists(select * from TraderView where defStrategyID = @iStrategyID)
		begin
			if @tiIsRaiseError = 1 Raiserror('Can''t remove strategy, because this strategy is assigned as a default strategy to trader(s)!',16,1)
			Return(-2)
		end

	if exists(select * from TraderContractView where strategyID = @iStrategyID)
		begin
			if @tiIsRaiseError = 1 Raiserror('Can''t remove strategy, because this strategy is assigned to trader contract(s)!',16,1)
			Return(-3)
		end

	update Strategy set actionID=3 where strategyID = @iStrategyID
		if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

	set @error = @@error
finish:
	Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_BoStrategy_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoStrategy_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoStrategy_Del]
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

	if exists(select * from TraderView where defStrategyID = @iStrategyID)
		Return(-2)

	if exists(select * from TraderContractView where strategyID = @iStrategyID)
		Return(-3)

	begin transaction
		
	update Strategy set actionID = 3 where strategyID = @iStrategyID
		if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

	commit transaction

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_Strategy_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Strategy_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Strategy_Get]
GO

CREATE PROC dbo.usp_Strategy_Get
	@iStrategyID int = Null
as
	------------------------------------------------
	set nocount on
	------------------------------------------------

	if @iStrategyID is Null
	   select
			strategyID as iStrategyID, 
			strategyName as vcStrategyName, 
			[description] as vcDescription,
			actionDate as dtActionDate,
			actionID as tiActionID
		from StrategyView
		order by StrategyName
	else
		select
			strategyID as iStrategyID, 
			strategyName as vcStrategyName, 
			[description] as vcDescription,
			actionDate as dtActionDate,
			actionID as tiActionID
		from Strategy
		where strategyID = @iStrategyID


	return @@error
GO

----------------------------------------------------------------------------------------------------
-- remove usp_Trade_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_TradeByTrader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeByTrader_Get]
GO

CREATE PROC dbo.usp_TradeByTrader_Get
	@iTraderID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iTraderID is null
	begin
			select
				tr.tradeID as iTradeID,
				
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.Symbol as vcSymbol,
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
				i.yield as fYield,
				null as dtExpiry,
				null as fStrike,
				null as tiIsCall,
				c.contractID as iUnderlyingContractID,
				c.contractTypeID as iUnderlyingContractTypeID,
				c.Symbol as vcUnderlyingSymbol,
				cp.priceClose as fPriceClose,
				cp.priceClose as fUnderlyingPriceClose,
				null as fPriceTheoClose,
				
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
				1 as iLotSize,					
				tr.status as tiStatus,
				null as iOptionRootID,
				null as tiIsSyntheticRoot,
				tr.execID as vcExecID,
				tr.mark as iMark
			from Trade tr
				inner join Contract c on tr.contractID = c.contractID
				inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join Stock s on c.contractID = s.stockID
				left join [Index] i on c.contractID = i.indexID
			where
				c.contractTypeID < 3
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				tr.contractID,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,
				tr.traderID,tr.strategyID,tr.brokerID,tr.brokerCommission,tr.clearingBrokerID,tr.clearingBrokerCommission,
				tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,cp.priceClose,tr.execID,tr.mark
			having min(tr.actionID) = 1
	
	   union
	
			select
				tr.tradeID as iTradeID,
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.Symbol as vcSymbol,
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
				i.yield as fYield,
				EC.expiryDate as dtExpiry,
				o.strike as fStrike,
				o.isCall as tiIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.Symbol as vcUnderlyingSymbol,
				cp.priceClose as fPriceClose,
				ucp.priceClose as fUnderlyingPriceClose,
				cp.priceTheo as fPriceTheoClose,
				
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
				ort.lotSize as iLotSize,
				tr.status as tiStatus,
				ort.optionRootID as iOptionRootID,
				ort.isSynthetic as tiIsSyntheticRoot,
				tr.execID as vcExecID,
				tr.mark as iMark
			from Trade tr inner join Contract c on tr.contractID = c.contractID
				inner join [Option] o on c.contractID = o.optionID
				join OptionRoot ort on o.optionRootID = ort.optionRootID
				inner join Contract UC on ort.underlyingID = UC.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
				left join Stock s on uc.contractID = s.stockID
				left join [Index] i on uc.contractID = i.indexID
			where 
				c.contractTypeID = 3
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
				tr.contractID,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,
				tr.traderID,tr.strategyID,tr.brokerID,tr.brokerCommission,tr.clearingBrokerID,tr.clearingBrokerCommission,
				tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,
				ort.lotSize,cp.priceClose,ucp.priceClose, ort.optionRootID,ort.isSynthetic,tr.execID,tr.mark
			having min(tr.actionID) = 1
			order by TradeDate
	end
	else
	begin
			select
				tr.tradeID as iTradeID,
				
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.Symbol as vcSymbol,
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
				i.yield as fYield,
				null as dtExpiry,
				null as fStrike,
				null as tiIsCall,
				c.contractID as iUnderlyingContractID,
				c.contractTypeID as iUnderlyingContractTypeID,
				c.Symbol as vcUnderlyingSymbol,
				cp.priceClose as fPriceClose,
				cp.priceClose as fUnderlyingPriceClose,
				null as fPriceTheoClose,
				
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
				1 as iLotSize,					
				tr.status as tiStatus,
				null as iOptionRootID,
				null as tiIsSyntheticRoot,
				tr.execID as vcExecID,
				tr.mark as iMark
			from Trade tr
				inner join Contract c on tr.contractID = c.contractID
				inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join Stock s on c.contractID = s.stockID
				left join [Index] i on c.contractID = i.indexID
			where
				c.contractTypeID < 3
				and tr.traderID = @iTraderID
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				tr.contractID,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,
				tr.traderID,tr.strategyID,tr.brokerID,tr.brokerCommission,tr.clearingBrokerID,tr.clearingBrokerCommission,
				tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,cp.priceClose,tr.execID,tr.mark
			having min(tr.actionID) = 1
	
	   union
	
			select
				tr.tradeID as iTradeID,
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.Symbol as vcSymbol,
				s.divFreq as iDivFreq,
				s.divDate as dtDivDate,
				s.divAmt as fDivAmt,
				i.yield as fYield,
				EC.expiryDate as dtExpiry,
				o.strike as fStrike,
				o.isCall as tiIsCall,
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.Symbol as vcUnderlyingSymbol,
				cp.priceClose as fPriceClose,
				ucp.priceClose as fUnderlyingPriceClose,
				cp.priceTheo as fPriceTheoClose,
				
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
				ort.lotSize as iLotSize,
				tr.status as tiStatus,
				ort.optionRootID as iOptionRootID,
				ort.isSynthetic as tiIsSyntheticRoot,
				tr.execID as vcExecID,
				tr.mark as iMark
			from Trade tr inner join Contract c on tr.contractID = c.contractID
				inner join [Option] o on c.contractID = o.optionID
				join OptionRoot ort on o.optionRootID = ort.optionRootID
				inner join Contract UC on ort.underlyingID = UC.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
				left join Stock s on uc.contractID = s.stockID
				left join [Index] i on uc.contractID = i.indexID
			where c.contractTypeID = 3
				and tr.traderID = @iTraderID
			group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
				EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
				tr.contractID,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,
				tr.traderID,tr.strategyID,tr.brokerID,tr.brokerCommission,tr.clearingBrokerID,tr.clearingBrokerCommission,
				tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,
				ort.lotSize,cp.priceClose,ucp.priceClose, ort.optionRootID,ort.isSynthetic,tr.execID,tr.mark
			having min(tr.actionID) = 1
			order by TradeDate
	end

     Return(@@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_IsNewStrategyForUnderlying_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IsNewStrategyForUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_IsNewStrategyForUnderlying_Get]
GO

CREATE PROCEDURE dbo.usp_IsNewStrategyForUnderlying_Get
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @iRetVal int
	
	select @iRetVal = convert(float, KeyValue)
		from DataInfo	
		where dataInfoID = 13

	if @@error = 0
		return @iRetVal
	else
		return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_IsNewStrategyForUnderlying_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IsNewStrategyForUnderlying_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_IsNewStrategyForUnderlying_Save]
GO

CREATE PROCEDURE dbo.usp_IsNewStrategyForUnderlying_Save
	@iNewStrategyForUnderlying int
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	begin tran

	if exists(select * from DataInfo where dataInfoID = 13)
		update DataInfo set keyValue = @iNewStrategyForUnderlying
			where dataInfoID = 13
	else
		insert into DataInfo (dataInfoID, keyName, keyValue) 
		values (13, 'NewStrategyForUnderlying', @iNewStrategyForUnderlying)

	if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

	commit tran
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_TraderContractStrategyWithSave_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContractStrategyWithSave_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderContractStrategyWithSave_Get]
GO

CREATE PROC dbo.usp_TraderContractStrategyWithSave_Get
    @iTraderID int,
    @iContractID int, -- underlying or option contract ID
	@iStrategyID int = null out
as
	-- this SP find strategy for trader and underlying
	-- 		if underlying is not assaigned to trader then new association will be added
	-- 		if no strategy was assigned to trader/contract then new strategy will be created
	-- returns internal error code ( 0 if succeeded )
	set nocount on
	declare @error int		set @error = 0
	
	declare @iUnderlyingID int
	declare @vcUnderlyingSymbol varchar(20)

	-- find underlying
	select @iUnderlyingID = contractID,
		@vcUnderlyingSymbol = symbol
	from ContractView
	where contractID = @iContractID
		and contractTypeID < 3

	-- find underlying is contract is option
	if @iUnderlyingID is null
		select @iUnderlyingID = c.contractID,
				@vcUnderlyingSymbol = c.symbol
		from OptionView o
			inner join OptionRootView ort on o.optionRootID = ort.optionRootID
			inner join ContractView c on ort.underlyingID = c.contractID
		where
			o.optionID = @iContractID

	if @iUnderlyingID is not null
	begin

		begin tran

		-- check trader/contract association
		if not exists(select 1 from TraderContractView where traderID = @iTraderID and contractID = @iUnderlyingID)
		begin
			-- make new trader/contract association
			insert into TraderContract
				(traderID, contractID, strategyID)
			values
				(@iTraderID, @iUnderlyingID, null)
			if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
		end
		else
		begin
			-- find exact strategy
			select @iStrategyID = strategyID
			from TraderContractView
			where traderID = @iTraderID
				and contractID = @iUnderlyingID

			if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end
		end

		if @iStrategyID is null
		begin
			-- find default trader strategy if no exact strategy exists
			select @iStrategyID = defStrategyID
			from TraderView
			where traderID = @iTraderID

			if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

			if @iStrategyID is null
			begin
				-- check whether create new strategy for underlying
				declare @iIsNewStrategyForUnderlying int
				exec @iIsNewStrategyForUnderlying = usp_IsNewStrategyForUnderlying_Get
				if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end
			
				if isnull(@iIsNewStrategyForUnderlying, 0) <> 0
				begin
					-- find strategy with underlying symbol
					select @iStrategyID = strategyID
					from StrategyView 
					where strategyName = @vcUnderlyingSymbol

					if @iStrategyID is null
					begin
						-- create new strategy with underlying symbol name
				        insert into Strategy
							(strategyName, [Description])
				        values
							(@vcUnderlyingSymbol, null)
						if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end
			
						select @iStrategyID = @@identity
					end

					-- update trader/contract association with new strategy
					update TraderContract
						set strategyID = @iStrategyID
					where
						traderID = @iTraderID 
						and contractID = @iUnderlyingID
						and actionID < 3

					if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			end
		end

		commit tran
	end

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_New
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_New]
GO

CREATE PROC dbo.usp_Trade_New
    @iContractID int,
    @iQuantity int,
    @fPrice float,
    @tiIsBuy tinyint,  
    @iTraderID int,
    @iBrokerID int = Null,
    @fBrokerCommission float =Null,
    @iClearingBrokerID int = Null,
    @fClearingBrokerCommission float =Null,
    @fTradedIV  float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime = Null out,
	@tiStatus tinyint,
	@iMark int = null,
    @iStrategyID int = null out
as
	set nocount on

	begin tran

	declare @error int		set @error = 0

	if @dtNewTradeDate is null	
	begin
		declare @iDeltaGMT int

		exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

		set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	end

	-- detemine strategy for trade
	exec @error = usp_TraderContractStrategyWithSave_Get @iTraderID, @iContractID, @iStrategyID out
	if @@error <> 0 or @error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

	-- get new tradeID ane seqNum
	declare @iNewSeqNum int

	select 
		@iNewTradeID = isnull(max(tradeID), 0) + 1,
		@iNewSeqNum = isnull(max(seqNum), 0) + 1
	from Trade

	-- create new trade
	insert into Trade (tradeID, actionID, contractID, tradeDate,  quantity, price, traderID, strategyID,
		isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, seqNum, mark)
	values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, @iStrategyID, @tiIsBuy,
		@iBrokerID, @fBrokerCommission, @iClearingBrokerID, @fClearingBrokerCommission,
		@fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, @iNewSeqNum, @iMark)

	if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

	commit tran

finish:
	if @error = 0
		return @iNewSeqNum
	else  
	begin
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		set @iStrategyID = null
		return @error
	end
GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_Update
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_Update]
GO

CREATE PROC dbo.usp_Trade_Update
	@iOldTradeID int,
	@iContractID int,
	@iQuantity int,
	@fPrice float,
	@tiIsBuy tinyint,
	@iTraderID int,
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
    @iNewTradeStrategyID int = Null out
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
				tradedIV, spotReference, isPosition, status, execID, seqNum, mark)
			select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark
			from Trade
			where tradeID = @iOldTradeID

			if @@error <> 0 or @error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
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
	
		-- detemine strategy for trade
		exec @error = usp_TraderContractStrategyWithSave_Get @iTraderID, @iContractID, @iNewTradeStrategyID out
		if @@error <> 0 or @error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

		insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, 
			strategyID, isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
			tradedIV, spotReference, isPosition, status, seqNum, execID, mark)
		values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, 
			@iNewTradeStrategyID,  @tiIsBuy, @iBrokerID, @fBrokerCommission, @iClearingBrokerID, 
			@fClearingBrokerCommission, @fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, 
			@iNewSeqNum, @vcExecID, @iMark)

		if @@error <> 0 or @error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

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
		set @iNewTradeStrategyID = null
		return @error
	end
GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_Del]
GO

CREATE PROC dbo.usp_Trade_Del
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
				tradedIV, spotReference, isPosition, status, execID, seqNum, mark)
			select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark
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

----------------------------------------------------------------------------------------------------
-- usp_Trade_Exec
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Exec]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_Exec]
GO

CREATE PROC dbo.usp_Trade_Exec
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
	@fUndBrokerCommission float,
	@fUndClearingBrokerCommission float,
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

	exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

	select @iNewOptTradeID = max(tradeID) + 1 from Trade
		if @iNewOptTradeID is null set @iNewOptTradeID = 1

	select @iNewOptSeqNum = isnull(max(seqNum),0) + 1 from Trade

	if @dtNewOptTradeDate is null	
	begin
		set @dtNewOptTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	end

	insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
		price, isBuy, traderID, strategyID, brokerID, brokerCommission,
		clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
		isPosition, status, execID, SeqNum, mark)
	select @iNewOptTradeID, 1, contractID, @dtNewOptTradeDate, @iOptQuantity, 0, @tiOptIsBuy,
		traderID, strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, 0, @tiOptStatus, null, @iNewOptSeqNum, mark
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
	
		insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
			price, isBuy, traderID, strategyID, brokerID, brokerCommission,
			clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
			isPosition, status, execID, SeqNum, mark)
		select @iNewUndTradeID, 1, @iUndContractID, @dtNewUndTradeDate, @iUndQuantity, @fUndPrice, @tiUndIsBuy,
			traderID, strategyID, brokerID, @fUndBrokerCommission, clearingBrokerID, @fUndClearingBrokerCommission,
			0, 0, 0, @tiUndStatus, null, @iNewUndSeqNum, mark
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

----------------------------------------------------------------------------------------------------
-- usp_Trade_Match
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Match]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_Match]
GO

CREATE PROC dbo.usp_Trade_Match
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
				isPosition, status, execID, SeqNum, mark)
			select tradeID, 0, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, @iOldManualSeqNum, mark
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
				isPosition, status, execID, SeqNum, mark)
			select tradeID, 0, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, @iOldActualSeqNum, mark
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
				isPosition, status, execID, SeqNum, mark)
			select @iNewActualTradeID, 1, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, 3, execID, @iNewActualSeqNum, mark
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

----------------------------------------------------------------------------------------------------
-- usp_Trade_Split
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Split]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_Split]
GO

CREATE PROC dbo.usp_Trade_Split
    @iContractID int,
    @fK float,
    @fF float,
    @iC int,
    @iW int,
	@iExchangeID int = Null, -- the exchangeID special for update ContractPrice table
	@tiIsRaiseError tinyint = 1
as
	set nocount on
	declare @error int		set @error = 0

	if (@fK = 0) or (@fF = 0)
	begin
		if isnull(@tiIsRaiseError, 0) <> 0 raiserror ('Error. Invalid parameters!', 16, 1)
		set @error = -2
		goto finish
	end

	begin tran
	update ContractPrice 
		set priceBid = priceBid / @fK,
	       priceAsk = priceAsk / @fK,
	       priceLast = priceLast / @fK,
	       priceOpen = priceOpen / @fK,
	       priceClose = priceClose / @fK,
	       priceLow = priceLow / @fK,
	       priceHigh = priceHigh / @fK
		where contractID = @iContractID and exchangeID = @iExchangeID

	if (@@error <> 0)
	begin
		if isnull(@tiIsRaiseError, 0) <> 0 raiserror ('Error. Can''t update ContractPrice for Underlying!', 16, 1)
		if @@TranCount = 1 Rollback Transaction
		set @error = -1
		goto finish
	end

	update Trade
	  set Price = Price / @fk,
	      quantity = round(quantity * @fK, 0)
	where contractID = @iContractID

	if (@@error <> 0)
	begin
		if isnull(@tiIsRaiseError, 0) <> 0 raiserror ('Error. Can''t update Trade for Underlying!', 16, 1)
		if @@TranCount = 1 Rollback Transaction
		set @error = -3
		goto finish
	end

	update Options
	  set Strike = Strike / @fF
	where UnderlyingcontractID = @iContractID

	if (@@error <> 0)
	begin
		if isnull(@tiIsRaiseError, 0) <> 0 raiserror ('Error. Can''t update Options for Underlying!', 16, 1)
		if @@TranCount = 1 Rollback Transaction
		set @error = -4
		goto finish
	end

	update ContractPrice
	  set priceBid = priceBid / @fF,
	      priceAsk = priceAsk / @fF,
	      priceLast = priceLast / @fF,
	      priceOpen = priceOpen / @fF,
	      priceClose = priceClose / @fF,
	      priceLow = priceLow / @fF,
	      priceHigh = priceHigh / @fF,
	      LotSize = @iW
	 from Options
	where Options.UnderlyingcontractID = @iContractID
	  and ContractPrice.contractID = Options.Option_ID

	if (@@error <> 0)
	begin
		if isnull(@tiIsRaiseError, 0) <> 0 raiserror ('Error. Can''t update ContractPrice for Options!', 16, 1)
		if @@TranCount = 1 Rollback Transaction
		set @error = -5
		goto finish
	end

	update Trade
	  set Price = Price / @fF,
	      quantity = quantity * @iC
	 from Options
	where Options.UnderlyingcontractID = @iContractID
	  and Trade.contractID = Options.Option_ID

	if (@@error <> 0)
	begin
		if isnull(@tiIsRaiseError, 0) <> 0 raiserror ('Error. Can''t update Trade for Options!', 16, 1)
		if @@TranCount = 1 Rollback Transaction
		set @error = -5
		goto finish
	end

	commit tran

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- remove usp_TradeByContractAndTrader_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeByContractAndTrader_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeByContractAndTrader_Del]
GO

----------------------------------------------------------------------------------------------------
-- usp_TradeCA_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeCA_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeCA_Del]
GO

CREATE PROC dbo.usp_TradeCA_Del
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
		tradedIV, spotReference, isPosition, status, execID, seqNum, mark)
	select 
		tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
		strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark
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

----------------------------------------------------------------------------------------------------
-- usp_TradeCA_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeCA_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeCA_Get]
GO

CREATE PROC dbo.usp_TradeCA_Get
	@iTradeID int = null,
	@iContractID int = null,
	@iUnderlyingID int = null,
	@iOptionRootID int = null
as
	if @iTradeID is not null
	begin
		select
			tradeID as iTradeID,
			contractID as iContractID,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			traderID as iTraderID,
			strategyID as iStrategyID, 
			brokerID as iBrokerID,
			brokerCommission as fBrokerCommission,
			clearingBrokerID as iClearingBrokerID,
			clearingBrokerCommission as fClearingBrokerCommission,
			tradedIV as fTradedIV,
			spotReference as fSpotReference,
			isPosition as tiIsPosition,
			status as tiStatus,
			mark as iMark
		from Trade
			where tradeID = @iTradeID
		group by tradeID,contractID,tradeDate,quantity,price,isBuy,traderID,
			strategyID, brokerID,brokerCommission,clearingBrokerID,clearingBrokerCommission,
			tradedIV,spotReference,isPosition,status,mark
		having min(actionID) = 1
		order by TradeDate
	end
	else if @iContractID is not null
	begin
		select
			tradeID as iTradeID,
			contractID as iContractID,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			traderID as iTraderID,
			strategyID as iStrategyID, 
			brokerID as iBrokerID,
			brokerCommission as fBrokerCommission,
			clearingBrokerID as iClearingBrokerID,
			clearingBrokerCommission as fClearingBrokerCommission,
			tradedIV as fTradedIV,
			spotReference as fSpotReference,
			isPosition as tiIsPosition,
			status as tiStatus,
			mark as iMark
		from Trade
			where contractID = @iContractID
		group by tradeID,contractID,tradeDate,quantity,price,isBuy,traderID,
			strategyID, brokerID,brokerCommission,clearingBrokerID,clearingBrokerCommission,
			tradedIV,spotReference,isPosition,status,mark
		having min(actionID) = 1
		order by TradeDate
	end
	else if @iOptionRootID is not null
	begin
		select
			tradeID as iTradeID,
			contractID as iContractID,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			traderID as iTraderID,
			strategyID as iStrategyID, 
			brokerID as iBrokerID,
			brokerCommission as fBrokerCommission,
			clearingBrokerID as iClearingBrokerID,
			clearingBrokerCommission as fClearingBrokerCommission,
			tradedIV as fTradedIV,
			spotReference as fSpotReference,
			isPosition as tiIsPosition,
			T.status as tiStatus,
			mark as iMark
		from Trade T	
			join OptionView O on O.optionID = T.contractID
			where o.optionRootID = @iOptionRootID
		group by tradeID,contractID,tradeDate,quantity,price,isBuy,traderID,
			strategyID, brokerID,brokerCommission,clearingBrokerID,clearingBrokerCommission,
			tradedIV,spotReference,isPosition,status,mark
		having min(T.actionID) = 1
		order by TradeDate
	end
	else if @iUnderlyingID is not null
	begin
		select
			tradeID as iTradeID,
			contractID as iContractID,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			traderID as iTraderID,
			strategyID as iStrategyID, 
			brokerID as iBrokerID,
			brokerCommission as fBrokerCommission,
			clearingBrokerID as iClearingBrokerID,
			clearingBrokerCommission as fClearingBrokerCommission,
			tradedIV as fTradedIV,
			spotReference as fSpotReference,
			isPosition as tiIsPosition,
			status as tiStatus,
			mark as iMark
		from Trade T	
			join OptionView O on O.optionID = T.contractID
			join OptionRootView ORT on ORT.optionRootID=O.optionRootID
			where ORT.underlyingID = @iUnderlyingID
		group by tradeID,contractID,tradeDate,quantity,price,isBuy,traderID,
			strategyID, brokerID,brokerCommission,clearingBrokerID,clearingBrokerCommission,
			tradedIV,spotReference,isPosition,status,mark
		having min(T.actionID) = 1
		order by TradeDate
	end

    Return(@@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_TradeCA_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeCA_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeCA_Save]
GO

CREATE PROC dbo.usp_TradeCA_Save
	@iCorpActionID int,
    @iTradeID int = null, --for new trade is null
	@iQuantity int, 
	@fPrice float, 
	@iContractID int = null, 
	@tiIsBuy tinyint = 0, 
	@iTraderID int = null, 
	@tiIsPosition tinyint = 0,
	@tiStatus tinyint = 0,
	@iMark int = null
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
				clearingBrokerID, clearingBrokerCommission, tradeDate, mark)
			select @iNewTradeID, 1, contractID, @iQuantity, @fPrice,
				isBuy, traderID, strategyID, isPosition, status, @iNewSeqNum, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradeDate, mark
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

			-- detemine strategy for trade
			declare @iStrategyID int
			exec @error = usp_TraderContractStrategyWithSave_Get @iTraderID, @iContractID, @iStrategyID out
			if @error != 0 begin Rollback Transaction TU Return(-1) end

			-- insert new trade
			insert into Trade(tradeID, actionID, contractID, quantity, price,
				isBuy, traderID, strategyID, isPosition, status, seqNum, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradeDate, mark)
			values 
				(@iNewTradeID, 1, @iContractID, @iQuantity, @fPrice, @tiIsBuy, @iTraderID,
				@iStrategyID, @tiIsPosition, @tiStatus, @iNewSeqNum, null, null, null, null, @dtNewTradeDate, @iMark)

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

----------------------------------------------------------------------------------------------------
-- usp_TradeFeed_1_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_1_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeFeed_1_Del]
GO

CREATE PROC dbo.usp_TradeFeed_1_Del
	@vcExecID varchar(20),
	@dtTradeDate datetime,
	@tiIsUpdateEnabled tinyint = 0,
	@iLastTradeTime int = null
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
		insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
			strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
			tradedIV, spotReference, isPosition, status, execID, seqNum, mark)
		select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
			strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
			tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark
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

----------------------------------------------------------------------------------------------------
-- usp_TradeFeed_1_New
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_1_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeFeed_1_New]
GO

CREATE PROC dbo.usp_TradeFeed_1_New
	@vcTraderAcronym varchar(12),
	@vcExecID varchar(20),
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
	@iMark int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iMinSeqNum int 
		select @iMinSeqNum = max(seqNum)
			from Trade 
			where 	execID = @vcExecID and 
					actionID > 0 and 
					Convert(varchar(8),@dtTradeDate,112) = Convert(varchar(8),tradeDate,112)

	if @tiIsUpdateEnabled = 1
		begin
			if @iMinSeqNum is not null 
				begin
					exec @error = usp_TradeFeed_1_Del @vcExecID, @dtTradeDate, @tiIsUpdateEnabled
					---if removed succesfully
					if @error != -1
						begin	
							select @iMinSeqNum = max(seqNum)
								from Trade 
								where execID = @vcExecID and 
									actionID = 0 and 
									Convert(varchar(8),@dtTradeDate,112) = Convert(varchar(8),tradeDate,112)
						end
					else	
						begin
							set @iMinSeqNum = 0
						end
				end
		end

	--trade already exists
	else if @iMinSeqNum is not null and @tiIsUpdateEnabled = 0
		begin
			select -1 as iMaxSeqNum, @iMinSeqNum as iMinSeqNum, -1 as iStructureMissed
			Return(-1)
		end 

	Declare @iTradeContractID int, 
			@iUnderlyingContractID int,
			@iStructureMissed int

	set @iStructureMissed = 0

	--check underlying symbol
		if @vcUnderlyingSymbol is null and  @iContractTypeID <3 
			set @vcUnderlyingSymbol = @vcContractSymbol

	select @iUnderlyingContractID = contractID 
		from ContractView 
		where symbol = @vcUnderlyingSymbol and contractTypeID < 3

	Begin Transaction
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

					set @iStructureMissed = @iStructureMissed + 2
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
			insert into Trader(acronym,traderName) values(@vcTraderAcronym,@vcTraderAcronym)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
				set @iTraderID = @@identity
		end

	if not exists(select 1 from TraderContractView where traderID = @iTraderID and contractID = @iUnderlyingContractID)
		begin
			insert into TraderContract (traderID, contractID) values (@iTraderID, @iUnderlyingContractID)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end

	declare @iNewTradeID int
		select @iNewTradeID = max(tradeID) + 1 from Trade

	declare @iSeqNum int 
		select @iSeqNum = isnull(max(seqNum),0) + 1 from Trade

	if @iNewTradeID is null set @iNewTradeID = 1

	-- detemine strategy for trade
	declare @iNewTradeStrategyID int
	exec @error = usp_TraderContractStrategyWithSave_Get @iTraderID, @iTradeContractID, @iNewTradeStrategyID out
	if @@error <> 0 or @error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

	--create new trade	
	insert into Trade (execID, tradeID, actionID, contractID, tradeDate, 
		quantity, price, traderID, strategyID, isBuy, isPosition, status, seqNum, mark)
	values (@vcExecID, @iNewTradeID, 1, @iTradeContractID, @dtTradeDate, 
		@iQuantity, @fPrice, @iTraderID, @iNewTradeStrategyID, @tiIsBuy, 0, 0, @iSeqNum, @iMark)

	if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

	--write into TradeLog 
	if @iStructureMissed = 1
		begin
			insert into TradeLog (contractID, eventTypeID, seqNum) values (@iUnderlyingContractID, 2, @iSeqNum)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end
	else if @iStructureMissed = 2
		begin
			insert into TradeLog (contractID, eventTypeID, seqNum) values (@iOptionID, 3, @iSeqNum)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end
	else if @iStructureMissed = 3
		begin
			insert into TradeLog (contractID, eventTypeID, seqNum) values (@iOptionID, 3, @iSeqNum)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

			insert into TradeLog (contractID, eventTypeID, seqNum) values (@iUnderlyingContractID, 2, @iSeqNum)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end

	--save the last successful trade time
	if @iLastTradeTime is not null
		begin
			update DataInfo set keyValue = @iLastTradeTime where keyName='lastTradeTime'
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end

	--return output recordset
	select isnull(@iMinSeqNum,0) as iMinSeqNum,@iSeqNum as iMaxSeqNum, @iStructureMissed as iStructureMissed

	Commit Transaction

    set @error = 0
Return(@error)

finish:
	select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed
	Return(@@error)
GO

----------------------------------------------------------------------------------------------------
-- remove usp_TradeFeed_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeFeed_Del]
GO

----------------------------------------------------------------------------------------------------
-- remove usp_TradeFeed_New
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeFeed_New]
GO

----------------------------------------------------------------------------------------------------
-- usp_TradeFeed_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeFeed_Get]
GO

CREATE PROC dbo.usp_TradeFeed_Get
	@vcExecID varchar(20),
	@dtTradeDate datetime
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

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
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,
		i.yield as fYield,
		null as dtExpiry,
		null as fStrike,
		null as tiIsCall,
		c.contractID as iUnderlyingContractID,
		c.contractTypeID as iUnderlyingContractTypeID,
		c.Symbol as vcUnderlyingSymbol,
		null as fPriceTheoClose,
		tr.isPosition as tiIsPosition,
		tr.status as tiStatus,
		null as fPriceClose,
		null as fUnderlyingPriceClose,
		tr.mark as iMark
	from Trade tr 
		join Contract C  on tr.contractID = c.contractID
		join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		left join StockView s on c.contractID = s.stockID
		left join IndexView i on c.contractID = i.indexID
	where tr.actionID > 0 
		and tr.execID = @vcExecID 
		and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tr.tradeDate,112)

union

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
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,
		i.yield as fYield,
		EC.expiryDate as dtExpiry,
		o.strike as fStrike,
		o.isCall as tiIsCall,
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		cp.priceTheo as fPriceTheoClose,
		tr.isPosition as tiIsPosition,
		ort.lotSize as iLotSize,
		tr.status as tiStatus,
		cp.PriceClose as fPriceClose,
		ucp.priceClose as fUnderlyingPriceClose,
		tr.mark as iMark
	from Trade tr 
		join Contract C  on tr.contractID = c.contractID
		inner join [Option] O on C.contractID = O.optionID
		join OpriotRoot ort on ort.optionRootID = o.optionRootID
		inner join Contract UC on ort.underlyingID = UC.contractID
		inner join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
		inner join ContractPrice UCP on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
		join ExpiryInCalendarView EC on UC.expCalendarID = EC.expCalendarID and O.expiryID = EC.expiryID
		left join StockView s on uc.contractID = s.stockID
		left join [IndexView] i on uc.contractID = i.indexID
	where tr.actionID > 0 
		and tr.execID = @vcExecID 
		and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tr.tradeDate,112)
GO

----------------------------------------------------------------------------------------------------
-- usp_TradeForReconcile_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeForReconcile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeForReconcile_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_TradeForRisk_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeForRisk_Get]
GO


----------------------------------------------------------------------------------------------------
-- usp_TradeSeq_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSeq_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeSeq_Get]
GO

CREATE PROC dbo.usp_TradeSeq_Get
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
			tr.mark as iMark
		from Trade tr
		where 
			(@iMinSeqNum is null or @iMinSeqNum is not null and seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and cast(floor(cast(tradeDate as float)) as datetime) = @dtTradeDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		order by seqNum
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
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			null as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			1 as iLotSize,
			tr.status as tiStatus,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			case when exists(select * from TradeLog where contractID = tr.contractID and seqNum = tr.seqNum) then 1 else 0 end iStructureMissed,
			null as iOptionRootID,
			null as tiIsSyntheticRoot,
			tr.execID as vcExecID,
			tr.mark as iMark
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID=cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
		where c.contractTypeID < 3
			and (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and tr.tradeDate >= @dtTradeDate and tr.tradeDate < @dtMaxDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		union
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
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceTheo as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			ORt.lotSize as iLotSize,
			status as tiStatus,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose,
			case 
			    when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum)
					and exists(select * from TradeLog where contractID = O.optionID and seqNum = tr.seqNum) then 3
				when exists(select * from TradeLog where contractID = o.optionID and seqNum = tr.seqNum) then 2
				when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum) then 1
            else 0 	
			end iStructureMissed,
			ORT.optionRootID as iOptionRootID,
			ORT.isSynthetic as tiIsSyntheticRoot,
			tr.execID as vcExecID,
			tr.mark as iMark
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join [OptionView] o on c.contractID = o.optionID
			inner join OptionRootView ORt on ORt.optionRootID = o.optionRootID
			inner join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView EC on o.expiryID = EC.expiryID and EC.expCalendarID = uc.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
		where c.contractTypeID = 3
			and (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and tr.tradeDate >= @dtTradeDate and tr.tradeDate < @dtMaxDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		order by tr.seqNum
GO

----------------------------------------------------------------------------------------------------
-- usp_TradeSeqByTrader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSeqByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeSeqByTrader_Get]
GO

CREATE PROC dbo.usp_TradeSeqByTrader_Get
	@iTraderID int = null,
	@iMinSeqNum int,
	@iMaxSeqNum int
AS
	set nocount on

	set @iMinSeqNum = isnull(@iMinSeqNum, 0)
	set @iMaxSeqNum = isnull(@iMaxSeqNum, 2147483647)

	if @iTraderID is null
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
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			null as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			1 as iLotSize,
			tr.status as tiStatus,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			case when exists(select * from TradeLog where contractID = tr.contractID and seqNum = tr.seqNum) then 1 else 0 end iStructureMissed,
			null as iOptionRootID,
			null as tiIsSyntheticRoot,
			tr.execID as vcExecID,
			tr.mark as iMark
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID=cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
		where c.contractTypeID < 3
			and tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
		union
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
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceTheo as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			ORt.lotSize as iLotSize,
			status as tiStatus,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose,
			case 
			    when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum)
					and exists(select * from TradeLog where contractID = O.optionID and seqNum = tr.seqNum) then 3

				when exists(select * from TradeLog where contractID = o.optionID and seqNum = tr.seqNum) then 2
				when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum) then 1
            else 0 	
			end iStructureMissed,
			ORT.optionRootID as iOptionRootID,
			ORT.isSynthetic as tiIsSyntheticRoot,
			tr.execID as vcExecID,
			tr.mark as iMark
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join [OptionView] o on c.contractID = o.optionID
			inner join OptionRootView ORt on ORt.optionRootID = o.optionRootID
			inner join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView EC on o.expiryID = EC.expiryID and EC.expCalendarID = uc.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
		where c.contractTypeID = 3
			and tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
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
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			null as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			1 as iLotSize,
			tr.status as tiStatus,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			case when exists(select * from TradeLog where contractID = tr.contractID and seqNum = tr.seqNum) then 1 else 0 end iStructureMissed,
			null as iOptionRootID,
			null as tiIsSyntheticRoot,
			tr.execID as vcExecID,
			tr.mark as iMark
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID=cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
		where c.contractTypeID < 3
			and tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
			and tr.traderID = @iTraderID
		union
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
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceTheo as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			ORt.lotSize as iLotSize,
			status as tiStatus,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose,
			case 
			    when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum)
					and exists(select * from TradeLog where contractID = O.optionID and seqNum = tr.seqNum) then 3
				when exists(select * from TradeLog where contractID = o.optionID and seqNum = tr.seqNum) then 2
				when exists(select * from TradeLog where contractID = uc.contractID and seqNum = tr.seqNum) then 1
            else 0 	
			end iStructureMissed,
			ORT.optionRootID as iOptionRootID,
			ORT.isSynthetic as tiIsSyntheticRoot,
			tr.execID as vcExecID,
			tr.mark as iMark
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join [OptionView] o on c.contractID = o.optionID
			inner join OptionRootView ORt on ORt.optionRootID = o.optionRootID
			inner join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView EC on o.expiryID = EC.expiryID and EC.expCalendarID = uc.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
		where c.contractTypeID = 3
			and tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
			and tr.traderID = @iTraderID
		order by tr.seqNum

GO

----------------------------------------------------------------------------------------------------
-- remove usp_TradeTotalForCID_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeTotalForCID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeTotalForCID_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_ContractForTrade_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractForTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ContractForTrade_Get]
GO

CREATE PROC dbo.usp_ContractForTrade_Get
	@iContractID int
as
	declare @dtToday datetime

	set @dtToday = cast(floor(cast(getdate() as float)) as datetime)

	select
			-- common
			C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName, 
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceClose as fPriceClose,
			null as fPriceTheoClose,
			1 as iLotSize,
			-- stock
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
		
			-- index
			i.yield as fYield,
		
			-- option
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.symbol as vcUnderlyingSymbol,
			cp.priceBid as fUnderlyingPriceBid,
			cp.priceAsk as fUnderlyingPriceAsk,
			cp.priceClose as fUnderlyingPriceClose,
			1 as iUnderlyingLotSize,
			null as iOptionRootID,
			null as tiIsSyntheticRoot
		from ContractView c  
			left join StockView S on S.stockID = C.contractID
			left join IndexView I on I.indexID = C.contractID
			left join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
		where	
			c.contractID = @iContractID
			and c.contractTypeID < 3
	union
	select	
			-- common
			C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName, 
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,
			ort.lotSize as iLotSize,
			-- stock
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
		
			-- index
			i.yield as fYield,
		
			-- option
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.symbol as vcUnderlyingSymbol,
			ucp.priceBid as fUnderlyingPriceBid,
			ucp.priceAsk as fUnderlyingPriceAsk,
			ucp.priceClose as fPriceClose,
			1 as iUnderlyingLotSize,
			ort.optionRootID as iOptionRootID,
			ort.isSynthetic as tiIsSyntheticRoot
	from ContractView c  
			left join OptionView O on O.optionID = C.contractID
			left join OptionRoot ORt on O.optionRootID = ORt.optionRootID
			left join Contract UC on ORt.underlyingID = UC.contractID
			join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			left join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
			left join ContractPrice ucp on uc.contractID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join IndexView i on uc.contractID = i.indexID			
	where
			c.contractID = @iContractID
			and EC.expiryDate >= @dtToday
			and c.contractTypeID = 3

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_ContractForTradeByUnd_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractForTradeByUnd_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ContractForTradeByUnd_Get]
GO

CREATE PROC dbo.usp_ContractForTradeByUnd_Get
	@iUnderlyingID int
as
	declare @dtToday datetime

	set @dtToday = cast(floor(cast(getdate() as float)) as datetime)

	select * from 
	(select
			-- common
			C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName, 
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceClose as fPriceClose,
			null as fPriceTheoClose,
			1 as iLotSize,
			-- stock
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
		
			-- index
			i.yield as fYield,
		
			-- option
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.symbol as vcUnderlyingSymbol,
			cp.priceBid as fUnderlyingPriceBid,
			cp.priceAsk as fUnderlyingPriceAsk,
			cp.priceClose as fUnderlyingPriceClose,
			1 as iUnderlyingLotSize,
			null as iOptionRootID,
			null as tiIsSyntheticRoot
		from ContractView c  
			left join StockView S on S.stockID = C.contractID
			left join IndexView I on I.indexID = C.contractID
			left join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
		where	
			c.contractID = @iUnderlyingID
			and c.contractTypeID < 3
	union
	select	
			-- common
			C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName, 
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,
			ort.lotSize as iLotSize,
			-- stock
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
		
			-- index
			i.yield as fYield,
		
			-- option
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.symbol as vcUnderlyingSymbol,
			ucp.priceBid as fUnderlyingPriceBid,
			ucp.priceAsk as fUnderlyingPriceAsk,
			ucp.priceClose as fPriceClose,
			1 as iUnderlyingLotSize,
			ort.optionRootID as iOptionRootID,
			ort.isSynthetic as tiIsSyntheticRoot
	from ContractView c  
			left join OptionView O on O.optionID = C.contractID
			left join OptionRoot ORt on O.optionRootID = ORt.optionRootID
			left join Contract UC on ORt.underlyingID = UC.contractID
			join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			left join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
			left join ContractPrice ucp on uc.contractID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join IndexView i on uc.contractID = i.indexID			
	where
			ORt.underlyingID = @iUnderlyingID
			and EC.expiryDate >= @dtToday
			and c.contractTypeID = 3
	) as c order by year(c.dtExpiry), month(c.dtExpiry), c.fStrike, c.tiIsCall desc

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- remove usp_UnderlyingByBook_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingByBook_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_UnderlyingByBook_Get]
GO

----------------------------------------------------------------------------------------------------
-- remove usp_UnderlyingOptionsByList_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingOptionsByList_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_UnderlyingOptionsByList_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_UnderlyingOptions_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingOptions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_UnderlyingOptions_Get]
GO

CREATE PROC dbo.usp_UnderlyingOptions_Get
	@iUnderlyingID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0,
	@dtMinExpiry datetime = Null
AS
	set nocount on
	
	if @dtMinExpiry is null set @dtMinExpiry = 0

	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				if isnull(@iUnderlyingID, 0) <> 0
					select	c.contractID as iContractID,
						c.symbol as vcSymbol,
						ort.underlyingID as iUnderlyingContractID,
						o.strike as fStrike, 
						ec.expiryDate as dtExpiry,
						o.isCall as tiIsCall,
						ort.lotSize as iLotSize,
						ort.optionRootID as iOptionRootID
					from ContractView c  
						inner join [Option] o on o.optionID = c.contractID
						join OptionRoot ort on o.optionRootID = ort.optionRootID
						inner join Contract uc on ort.underlyingID = uc.contractID
						inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
						join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					where c.contractTypeID = 3
						and ort.underlyingID = @iUnderlyingID
						and ec.expiryDate >= @dtMinExpiry
					order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
				else
					select	c.contractID as iContractID,
						c.symbol as vcSymbol,
						ort.underlyingID as iUnderlyingContractID,
						o.strike as fStrike, 
						ec.expiryDate as dtExpiry,
						o.isCall as tiIsCall,
						ort.lotSize as iLotSize,
						ort.optionRootID as iOptionRootID
					from ContractView c  
						inner join [Option] o on o.optionID = c.contractID
						join OptionRoot ort on o.optionRootID = ort.optionRootID
						inner join Contract uc on ort.underlyingID = uc.contractID
						inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
						join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					where c.contractTypeID = 3
						and ec.expiryDate >= @dtMinExpiry
					order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
				    inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
							join OptionRoot ort on cig.contractID = ort.underlyingID
							join ContractView uc on ort.underlyingID = uc.contractID
							inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
							join OptionRoot ort on cig.contractID = ort.underlyingID					
							join ContractView uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, ec.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
				    inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, ec.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
					join OptionRootView ort on cig.contractID = ort.underlyingID					
					inner join ContractView uc on ort.underlyingID = uc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, ec.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
					join OptionRootView ort on cig.contractID = ort.underlyingID					
					inner join ContractView uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, ec.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
	end

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_Option_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Option_Get]
GO

CREATE PROC dbo.usp_Option_Get
	@iUnderlyingID int = Null,		 
	@dtMinExpiry datetime = Null,
	@iOptionID int = Null,
	@tiBaseInfoOnly tinyint = 0,
	@tiWithPosition tinyint = 0,
	@iTraderID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null set @dtMinExpiry = 0

	if isnull(@tiWithPosition, 0) = 0
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			if @iUnderlyingID is not null
				and @iOptionID is null
				and @tiBaseInfoOnly = 0
				and @tiWithPosition = 0
				
				-- Sharky: speed optimization for quote view

				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName, 
					ORt.underlyingID as iUnderlyingContractID,
					O.strike as fStrike, 
					EC.expiryDate as dtExpiry,
					O.isCall as tiIsCall,
					O.actionDate as dtActionDate,
					O.actionID as tiActionID,
					ORt.lotSize as iLotSize,
					ORt.optionRootID as iOptionRootID,
					ORt.symbol as vcOptionRootSymbol,
					O.isManualyEntered as tiIsManualyEntered
				from ContractView C  
					inner join OptionView O on O.optionID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract C1 on ORt.underlyingID = C1.contractID
					inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				where c.contractTypeID=3
					and EC.expiryDate >= @dtMinExpiry
					and ORt.underlyingID = @iUnderlyingID
				order by ORt.underlyingID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
			else
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName, 
					ORt.underlyingID as iUnderlyingContractID,
					O.strike as fStrike, 
					EC.expiryDate as dtExpiry,
					O.isCall as tiIsCall,
					O.actionDate as dtActionDate,
					O.actionID as tiActionID,
					ORt.lotSize as iLotSize,
					ORt.optionRootID as iOptionRootID,
					ORt.symbol as vcOptionRootSymbol,
					O.isManualyEntered as tiIsManualyEntered
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
				O.isCall as tiIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				ORt.lotSize as iLotSize,
				ORt.optionRootID as iOptionRootID,
				ORt.symbol as vcOptionRootSymbol,
				O.isManualyEntered as tiIsManualyEntered 
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
				O.isCall as tiIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				ORt.lotSize as iLotSize,
				ORt.optionRootID as iOptionRootID,
				ORt.symbol as vcOptionRootSymbol,
				O.isManualyEntered as tiIsManualyEntered 
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
				O.strike, EC.expiryDate, O.isCall, O.actionDate, O.actionID,ORt.lotSize, ORt.optionRootID, ORt.symbol,O.isManualyEntered
			order by ORt.underlyingID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				ORt.underlyingID as iUnderlyingContractID,
				O.strike as fStrike, 
				EC.expiryDate as dtExpiry,
				O.isCall as tiIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				ORt.lotSize as iLotSize,
				ORt.optionRootID as iOptionRootID,
				ORt.symbol as vcOptionRootSymbol,
				O.isManualyEntered as tiIsManualyEntered 
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
				O.strike, EC.expiryDate, O.isCall, O.actionDate, O.actionID,ORt.lotSize, ORt.optionRootID, ORt.symbol,O.isManualyEntered
	end

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- remove usp_Strategy_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Strategy_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Strategy_Save]
GO

----------------------------------------------------------------------------------------------------
-- usp_BoStrategy_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoStrategy_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoStrategy_Save]
GO

CREATE    proc dbo.usp_BoStrategy_Save
    @iStrategyID int,
    @vcStrategyName varchar(12),
    @vcDescription varchar(120)
as
   
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0
   
   if (@iStrategyID is Null)
   begin
		select @iStrategyID = strategyID 
		from StrategyView 
		where strategyName = @vcStrategyName

		if @iStrategyID is not null
			return @iStrategyID
		
		Begin Transaction
        
        insert into Strategy (StrategyName, [Description])
            values (@vcStrategyName, @vcDescription)
		if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

		set @error = @@identity
		
		Commit Transaction
   end 	
   else 
   begin
		if exists (select * from StrategyView where @vcStrategyName = strategyName and strategyID<>@iStrategyID)
		begin
			Return (-2)
		end
		
		Begin Transaction

        update Strategy 
             set  StrategyName = @vcStrategyName,
                  [Description] = @vcDescription
        where strategyID = @iStrategyID
        
		if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

		set @error = 0
		
		Commit Transaction
   end                
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTraderContract_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTraderContract_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTraderContract_Del]
GO

CREATE PROCEDURE dbo.usp_BoTraderContract_Del 
	@iTraderID int,
	@iContractID int = null
AS
/*
	Created by Michael Malyshkin
	Description: this procedure is delete information from TraderContract table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	if @iContractID is null
	begin
		if exists(select 1 from Trade where traderID = @iTraderID)
			Return(-1)

		if exists(select 1 from [Order] where traderID = @iTraderID)
			Return(-2)
			
		Begin Transaction	
				
			update TraderContract set actionID=3
				where traderID = @iTraderID

			if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

			set @error = 0
						
		Commit Transaction

	end
	else 
	begin
		if exists(select 1 from Trade where traderID = @iTraderID and contractID = @iContractID)
			Return(-1)

		if exists(select 1 from [Order] where traderID = @iTraderID and contractID = @iContractID)
			Return(-2)
			
		Begin Transaction	
		
			update TraderContract set actionID=3
				where traderID = @iTraderID and contractID=@iContractID

			if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

			set @error = 0
		
		Commit Transaction

	end
	
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- remove usp_Stock_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Del]
GO

----------------------------------------------------------------------------------------------------
-- usp_BoStock_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoStock_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoStock_Del]
GO

CREATE PROCEDURE dbo.usp_BoStock_Del
        @iStockID int
  as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0
	
	if exists (select * from Trade(nolock) where contractID = @iStockID)
	begin
		Return (-1)
	end 

	if exists (select 1 from [Order](nolock) where contractID = @iStockID)
	begin
		Return (-2)
	end 

	Create table #Options_for_delete(OptionID int)

	insert into #Options_for_delete
		select optionID from [Option] o(nolock)
				join OptionRoot ort on o.optionRootID = ort.optionRootID
 		where underlyingID = @iStockID

	if exists (select tradeID from Trade where contractID in (select optionID from #Options_for_delete))
		begin
			Return (-3)
		end 

	if exists (select 1 from [Order] where contractID in (select optionID from #Options_for_delete))
		begin
			Return (-4)
		end 

	Begin Transaction StockDel
		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del 1
			end

		exec usp_TradeLog_Del null, @iStockID

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
Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- remove usp_Index_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Del]
GO
----------------------------------------------------------------------------------------------------
-- usp_BoIndex_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoIndex_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoIndex_Del]
GO

CREATE PROCEDURE dbo.usp_BoIndex_Del
		@iIndexID int
  as
/*
	Description: this procedure is mark as delete information in Index table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0
	
	if exists (select tradeID from Trade
				where Trade.contractID = @iIndexID)
	begin
	   RETURN (-1)
	end

	if exists (select 1 from [Order]
				where [Order].contractID = @iIndexID)
	begin
	   RETURN (-2)
	 end

	Create table #Options_for_delete(OptionID int)

	insert into #Options_for_delete
		select optionID from [Option] o(nolock)
				join OptionRoot ort on o.optionRootID = ort.optionRootID
 		where underlyingID = @iIndexID

	if exists (select tradeID from Trade where contractID in (select optionID from #Options_for_delete))
		begin
			Return (-3)
		end 

	if exists (select 1 from [Order] where contractID in (select optionID from #Options_for_delete))
		begin
			Return (-4)
		end 

	Begin Transaction IndexDel

		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del @bisRemovePermanent = 0
			end

		delete from IndexCorrelation where indexID1 = @iIndexID
			if @@error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		delete from IndexCorrelation where indexID2 = @iIndexID
			if @@error != 0 begin set @error = -6 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		delete from IndexDataHistory where indexID = @iIndexID
			if @@error != 0 begin set @error = -7 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update IndexBeta set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -8 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update IndexDefinition set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -9 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update ContractInGroup set actionID=3 where contractID = @iIndexID 
			if @@error != 0 begin set @error = -10 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update [Index] set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -11 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update Contract set actionID=3 
			where contractID in 
				(select OptionID 
					from [Option] O 
							join OptionRoot ORt on O.optionRootID=ORt.optionRootID and ORt.underlyingID=@iIndexID)
			if @@error != 0 begin set @error = -12 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticUnderlyingParam set actionID=3 where underlyingID = @iIndexID or syntheticOptRootID in 
				(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -13 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where syntheticOptRootID in 
				(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -14 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -15 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where syntheticOptRootID in 
			(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -16 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticOptionRoot set actionID=3 where optionRootID in (select optionRootID from OptionRoot where underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -17 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update OptionRoot set actionID = 3 where underlyingID = @iIndexID
			if @@error != 0 begin set @error = -18 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update TraderContract set actionID=3 where contractID = @iIndexID  
			if @@error <> 0 begin set @error = -20 if @@TranCount != 0 Rollback Transaction IndexDel goto finish end

		update Contract set actionID=3 where contractID = @iIndexID
			if @@error != 0 begin set @error = -21 if @@TranCount != 0 Rollback Tran IndexDel goto finish end
		
		set @error = @@error
	Commit Transaction IndexDel
finish:
Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- TradeHistory table
----------------------------------------------------------------------------------------------------
if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TradeHistory]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
begin
	CREATE TABLE [dbo].[TradeHistory]
	(
		[actionDate] [datetime] NOT NULL,
		
		[tradeID] [int] NOT NULL ,
		[tradeDate] [datetime] NULL ,
		[tradeQty] [int] NOT NULL ,
		[tradePrice] [float] NOT NULL ,
		[tradeIsBuy] [tinyint] NOT NULL ,
		[tradeStatus] [tinyint] NULL,
		[tradeExecID] [varchar] (20) NULL ,
		[tradeMark] [int] NULL ,

		[contractID] [int] NOT NULL ,
		[contractTypeID] [int] NOT NULL ,
		[contractSymbol] [varchar] (20) NOT NULL ,
		[contractExportSymbol] [varchar] (20) NULL ,
		[contractPriceClose] [float] NULL ,

		[underlyingID] [int] NOT NULL ,
		[underlyingTypeID] [int] NOT NULL ,
		[underlyingSymbol] [varchar] (20) NOT NULL ,
		[underlyingExportSymbol] [varchar] (20) NULL ,
		[underlyingDivFreq] [int] NULL ,
		[underlyingDivDate] [datetime] NULL ,
		[underlyingDivAmt] [float] NULL ,
		[underlyingYield] [float] NULL ,
		[underlyingPriceClose] [float] NULL ,

		[futureID] [int] NULL ,
		[futureRootSymbol] [varchar] (20) NULL ,
		[futureSymbol] [varchar] (20) NULL ,
		[futureExportSymbol] [varchar] (20) NULL ,
		[futureMaturity] [datetime] NULL ,
		[futureLotSize] [int] NULL ,
		[futurePriceClose] [float] NULL ,

		[optionRootID] [int] NULL ,
		[optionRootSymbol] [varchar] (20) NULL ,
		[optionRootLotSize] [int] NULL ,

		[optionExpiry] [datetime] NULL ,
		[optionStrike] [float] NULL ,
		[optionIsCall] [tinyint] NULL ,
		[optionPriceTheoClose] [float] NULL ,

		[traderID] [int] NULL ,
		[traderAcronym] [varchar] (12) NOT NULL ,
		[traderName] [varchar] (120) NOT NULL ,

		[strategyID] [int] NULL ,
		[strategyName] [varchar] (12) NULL ,

		[brokerID] [int] NULL ,
		[brokerName] [varchar] (12) NULL ,
		[brokerCommission] [float] NULL ,

		[clearingBrokerID] [int] NULL ,
		[clearingBrokerName] [varchar] (12) NULL ,
		[clearingBrokerCommission] [float] NULL ,

		[tradedIV] [float] NULL ,
		[spotReference] [float] NULL ,
	)
end
GO

if not exists(select * from sysindexes where id=object_id('TradeHistory') and name='IX_TradeHistory_TraderAcronym')
	CREATE INDEX [IX_TradeHistory_TraderAcronym] ON [dbo].[TradeHistory]([traderAcronym])
GO

if not exists(select * from sysindexes where id=object_id('TradeHistory') and name='IX_TradeHistory_BrokerName')
	CREATE INDEX [IX_TradeHistory_BrokerName] ON [dbo].[TradeHistory]([brokerName])
GO

if not exists(select * from sysindexes where id=object_id('TradeHistory') and name='IX_TradeHistory_TradeDate')
	CREATE INDEX [IX_TradeHistory_TradeDate] ON [dbo].[TradeHistory]([tradeDate])
GO

if not exists(select * from sysindexes where id=object_id('TradeHistory') and name='IX_TradeHistory_ContractSymbol')
	CREATE INDEX [IX_TradeHistory_ContractSymbol] ON [dbo].[TradeHistory]([contractSymbol])
GO

if not exists(select * from sysindexes where id=object_id('TradeHistory') and name='IX_TradeHistory_UnderlyingSymbol')
	CREATE INDEX [IX_TradeHistory_UnderlyingSymbol] ON [dbo].[TradeHistory]([underlyingSymbol])
GO

----------------------------------------------------------------------------------------------------
-- usp_BoContractExportSymbol_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoContractExportSymbol_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
    drop proc usp_BoContractExportSymbol_Save
GO

CREATE PROCEDURE dbo.usp_BoContractExportSymbol_Save
	@iContractID int,
	@vcExportSymbol varchar(20)
AS
/*
	Created by Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	declare @error int		set @error = 0

	Begin Transaction
		if isnull(@vcExportSymbol, '') <> ''
	        update Contract
			set exportSymbol = @vcExportSymbol
			where contractID = @iContractID
		else
	        update Contract
			set exportSymbol = null
			where contractID = @iContractID
		if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

	Commit Transaction

finish:
	return @error

GO

----------------------------------------------------------------------------------------------------
-- usp_BoUnderlyingForTradeHistory_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoUnderlyingForTradeHistory_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
    drop proc usp_BoUnderlyingForTradeHistory_Get
GO

CREATE PROCEDURE dbo.usp_BoUnderlyingForTradeHistory_Get
AS
/*
	Created by Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

		select 
			c.contractID as iUnderlyingID,
			c.Symbol as vcUnderlyingSymbol
		from TradeView tr 
			join ContractView c on tr.contractID = c.contractID
		where c.contractTypeID in (1, 2)
			and isnull(tr.isPosition, 0) = 0
		group by c.contractID, c.Symbol
	union
		select 
			uc.contractID as iUnderlyingID,
			uc.Symbol as vcUnderlyingSymbol
		from TradeView tr 
			inner join ContractView c on tr.contractID = c.contractID
			left join OptionView o on tr.contractID = o.optionID
			left join OptionRoot ort on o.optionRootID = ort.optionRootID
			inner join Contract uc on ort.underlyingID = uc.contractID
		where c.contractTypeID = 3
			and isnull(tr.isPosition, 0) = 0
		group by uc.contractID, uc.Symbol
	
		order by vcUnderlyingSymbol


return @@error

GO

----------------------------------------------------------------------------------------------------
-- usp_BoTrade_StoreToHistoryAndClear
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTrade_StoreToHistoryAndClear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
    drop proc usp_BoTrade_StoreToHistoryAndClear
GO

CREATE PROCEDURE dbo.usp_BoTrade_StoreToHistoryAndClear
	@tiStoreToHistory tinyint = null,
	@dtLastTradeDateUTC datetime = null,
	@iStoredTradesCount int = null out
AS
/*
	Created by Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	
	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	begin tran

	-- clear simulated and manual deleted trades log
	delete from TradeLog 
		where seqNum in (select seqNum from Trade 
					where tradeID in (select tradeID from Trade where isnull(status, 0) in (1, 2) and actionID = 0))
	if @@error != 0 begin set @error = -1 if @@TranCount = 1 Rollback Tran goto finish end

	-- clear simulated and manual deleted trades
	delete from Trade 
		where tradeID in (select tradeID from Trade where isnull(status, 0) in (1, 2) and actionID = 0)
	if @@error !=0 begin set @error = -2 if @@TranCount = 1 Rollback Tran goto finish end

	-- clear actual and actual matched deleted trades log
	delete from TradeLog 
		where seqNum in (select seqNum from Trade
							where isnull(status, 0) in (0, 3) and datediff(day, tradeDate, @dtLastTradeDateUTC) >= 0)
	if @@error !=0 begin set @error = -3 if @@TranCount = 1 Rollback Tran goto finish end	

	-- clear actual and actual matched deleted trades
	delete from Trade 
		where isnull(status, 0) in (0, 3) and datediff(day, tradeDate, @dtLastTradeDateUTC) >= 0
	if @@error !=0 begin set @error = -4 if @@TranCount = 1 Rollback Tran goto finish end

	if isnull(@tiStoreToHistory, 0) <> 0
	begin
		-- move actual and actual matched trades to history
		insert into TradeHistory
		select
			getdate() as dtActionDate,
	
			tr.tradeID as iTradeID, tr.tradeDate as dtTradeDate, tr.quantity as iQuantity,
			tr.price as fPrice, tr.isBuy as tiIsBuy, tr.status as tiStatus, tr.execID as vcExecID,
			tr.mark as iTradeMark,
			
			c.contractID as iContractID, c.contractTypeID as iContractTypeID, c.Symbol as vcSymbol,
			c.exportSymbol as vcExportSymbol, cp.priceClose as fPriceClose,
	
			c.contractID as iUnderlyingContractID, c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol, c.exportSymbol as vcUnderlyingExportSymbol,
			s.divFreq as iUnderlyingDivFreq, s.divDate as dtUnderlyingDivDate,
			s.divAmt as fUnderlyingDivAmt, i.yield as fUnderlyingYield,
			cp.priceClose as fUnderlyingPriceClose,
	
			null as iFutureID, null as vcFutureRootSymbol, null as vcFutureSymbol,
			null as vcFutureExportSymbol, null as dtFutureMaturity, null as iFutureLotSize,
			null as fFuturePriceClose,
	
			null as iOptionRootID, null as vcOptionRootSymbol, null as iOptionRootLotSize,
	
			null as dtOptionExpiry, null as fOptionStrike, null as tiOptionIsCall, null as fPriceTheoClose,
	
			tr.traderID as iTraderID, t.acronym as vcTraderAcronym, t.traderName as vcTraderName,
	
			st.strategyID as iStrategyID, st.strategyName as vcStrategyName,
	
			tr.brokerID as iBrokerID, br.brokerName as vcBrokerName, tr.brokerCommission as fBrokerCommission,
	
			tr.clearingBrokerID as iClearingBrokerID, cbr.brokerName as vcClearingBrokerName,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
	
			tr.tradedIV as fTradedIV, tr.spotReference as fSpotReference
	
		from TradeView tr
			inner join ContractView c on tr.contractID = c.contractID
			left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
			left join BrokerView br on tr.brokerID = br.brokerID
			left join BrokerView cbr on tr.clearingBrokerID = cbr.brokerID
			left join TraderView t on tr.traderID = t.traderID
			left join StrategyView st on tr.strategyID = st.strategyID
		where
			isnull(status, 0) in (0, 3)
			and isnull(tr.isPosition, 0) = 0
			and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
			and c.contractTypeID < 3
		union
		select
			getdate() as dtActionDate,
	
			tr.tradeID as iTradeID, tr.tradeDate as dtTradeDate, tr.quantity as iQuantity,
			tr.price as fPrice, tr.isBuy as tiIsBuy, tr.status as tiStatus, tr.execID as vcExecID,
			tr.mark as iTradeMark,
			
			c.contractID as iContractID, c.contractTypeID as iContractTypeID, c.Symbol as vcSymbol,
			c.exportSymbol as vcExportSymbol, cp.priceClose as fPriceClose,
	
			uc.contractID as iUnderlyingContractID, uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol, uc.exportSymbol as vcUnderlyingExportSymbol,
			s.divFreq as iUnderlyingDivFreq, s.divDate as dtUnderlyingDivDate,
			s.divAmt as fUnderlyingDivAmt, i.yield as fUnderlyingYield,
			ucp.priceClose as fUnderlyingPriceClose,
	
			null as iFutureID, null as vcFutureRootSymbol, null as vcFutureSymbol,
			null as vcFutureExportSymbol, null as dtFutureMaturity, null as iFutureLotSize,
			null as fFuturePriceClose,
	
			ort.optionRootID as iOptionRootID, ort.symbol as vcOptionRootSymbol, ort.lotSize as iOptionRootLotSize,
	
			ec.expiryDate as dtOptionExpiry, o.strike as fOptionStrike, o.isCall as tiOptionIsCall,
			cp.priceTheo as fPriceTheoClose,
	
			tr.traderID as iTraderID, t.acronym as vcTraderAcronym, t.traderName as vcTraderName,
	
			st.strategyID as iStrategyID, st.strategyName as vcStrategyName,
	
			tr.brokerID as iBrokerID, br.brokerName as vcBrokerName, tr.brokerCommission as fBrokerCommission,
	
			tr.clearingBrokerID as iClearingBrokerID, cbr.brokerName as vcClearingBrokerName,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
	
			tr.tradedIV as fTradedIV, tr.spotReference as fSpotReference
	
		from TradeView tr
			inner join ContractView c on tr.contractID = c.contractID
			left join OptionView o on tr.contractID = o.optionID
			left join OptionRoot ort on o.optionRootID = ort.optionRootID
			inner join Contract uc on ort.underlyingID = uc.contractID
			join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join IndexView i on uc.contractID = i.indexID
			left join BrokerView br on tr.brokerID = br.brokerID
			left join BrokerView cbr on tr.clearingBrokerID = cbr.brokerID
			left join TraderView t on tr.traderID = t.traderID
			left join StrategyView st on tr.strategyID = st.strategyID
		where
			isnull(status, 0) in (0, 3)
			and isnull(tr.isPosition, 0) = 0
			and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
			and c.contractTypeID = 3
	
		select @iStoredTradesCount = @@rowcount

		if @@error !=0 begin set @error = -5 if @@TranCount = 1 Rollback Tran goto finish end

		-- clear actual and actual matched trades log
		delete from TradeLog 
		where seqNum in (select seqNum from Trade
						where isnull(status, 0) in (0, 3) and datediff(day, tradeDate, @dtLastTradeDateUTC) >= 0)
		if @@error !=0 begin set @error = -6 if @@TranCount = 1 Rollback Tran goto finish end	

		-- clear actual and actual matched trades
		delete from Trade 
			where isnull(status, 0) in (0, 3) and datediff(day, tradeDate, @dtLastTradeDateUTC) >= 0
		if @@error !=0 begin set @error = -7 if @@TranCount = 1 Rollback Tran goto finish end
	end

	commit tran

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTradeHistory_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTradeHistory_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
    drop proc usp_BoTradeHistory_Del
GO

CREATE PROCEDURE dbo.usp_BoTradeHistory_Del
	@dtLastTradeDateUTC datetime = null,
	@iDeletedTradesCount int = null out
AS
/*
	Created by Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	
	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	begin tran

	delete from TradeHistory
	where datediff(day, tradeDate, @dtLastTradeDateUTC) >= 0

	select @iDeletedTradesCount = @@rowcount

	if @@error != 0 begin set @error = -1 if @@TranCount = 1 Rollback Tran goto finish end
	commit tran

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTradeHistory_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTradeHistory_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
    drop proc usp_BoTradeHistory_Get
GO

CREATE PROCEDURE dbo.usp_BoTradeHistory_Get
	@dtMinTradeDateUTC datetime = null,
	@dtMaxTradeDateUTC datetime = null
AS
/*
	Created by Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	
	if @dtMinTradeDateUTC is null	set @dtMinTradeDateUTC = 0 -- set to min available date to avoid 'is null' checking
	if @dtMaxTradeDateUTC is null	set @dtMaxTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	select
		actionDate as dtActionDate,
		
		tradeID as iTradeID,
		tradeDate as dtTradeDate,
		tradeQty as iTradeQty,
		tradePrice as fTradePrice,
		tradeIsBuy as tiTradeIsBuy,
		tradeStatus as tiTradeStatus,
		tradeExecID as vcTraeExecID,
		tradeMark as iTradeMark,

		contractID as iContractID,
		contractTypeID as iContractTypeID,
		contractSymbol as vcContractSymbol,
		contractExportSymbol as vcContractExportSymbol,
		contractPriceClose as fContractPriceClose,

		underlyingID as iUnderlyingID,
		underlyingTypeID as iUnderlyingTypeID,
		underlyingSymbol as vcUnderlyingSymbol,
		underlyingExportSymbol as vcUnderlyingExportSymbol,
		underlyingDivFreq as iUnderlyingDivFreq,
		underlyingDivDate as dtUnderlyingDivDate,
		underlyingDivAmt as fUnderlyingDivAmt,
		underlyingYield as fUnderlyingYield,
		underlyingPriceClose as fUnderlyingPriceClose,

		futureID as iFutureID,
		futureRootSymbol as vcFutureRootSymbol,
		futureSymbol as vcFutureSymbol,
		futureExportSymbol as vcFutureExportSymbol,
		futureMaturity as vcFutureMaturity,
		futureLotSize as iFutureLotSize,
		futurePriceClose as fFuturePriceClose,

		optionRootID as iOptionRootID,
		optionRootSymbol as vcOptionRootSymbol,
		optionRootLotSize as iOptionRootLotSize,

		optionExpiry as dtOptionExpiry,
		optionStrike as fOptionStrike,
		optionIsCall as tiOptionIsCall,
		optionPriceTheoClose as fOptionPriceTheoClose,

		traderID as iTraderID,
		traderAcronym as vcTraderAcronym,
		traderName as vcTraderName,

		strategyID as iStrategyID,
		strategyName as vcStrategyName,

		brokerID as iBrokerID,
		brokerName as vcBrokerName,
		brokerCommission as fBrokerCommission,

		clearingBrokerID as iClearingBrokerID,
		clearingBrokerName as vcClearingBrokerName,
		clearingBrokerCommission as fClearingBrokerCommission,

		tradedIV as fTradedIV,
		spotReference as fSpotReference
	from TradeHistory
	where datediff(day, tradeDate, @dtMinTradeDateUTC) >= 0
		and datediff(day, tradeDate, @dtMaxTradeDateUTC) <= 0

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoUnderlyings_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoUnderlyings_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoUnderlyings_Get]
GO

CREATE PROCEDURE dbo.usp_BoUnderlyings_Get
  as
	------------------------------------------------
	set nocount on
	------------------------------------------------	
	select	C.contractID as iContractID, 
			C.contractTypeID as iContractTypeID, 
			C.symbol as vcSymbol
			from StockView S
				join ContractView C on S.stockID = C.contractID
	union
	select	C.contractID as iContractID, 
			C.contractTypeID as iContractTypeID, 
			C.symbol as vcSymbol
			from IndexView I
				join ContractView C on I.indexID = C.contractID
	order by vcSymbol
Return(@@error)
GO

----------------------------------------------------------------------------------------------------
-- remove usp_OptionsForReconcile_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionsForReconcile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionsForReconcile_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_BoOptionsForReconcile_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoOptionsForReconcile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoOptionsForReconcile_Get]
GO

CREATE    proc dbo.usp_BoOptionsForReconcile_Get
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
		O.expiryID as iExpiryID,
		EC.expiryDate as dtExpiry,
		O.isCall as bIsCall,
		O.actionDate as dtActionDate,
		O.actionID as tiActionID,
		ORt.lotSize as iLotSize,
		ORt.optionRootID as iOptionRootID,
		ORt.symbol as vcOptionRootSymbol,
		O.isManualyEntered as tiIsManualyEntered,
		C.exportSymbol as vcExportSymbol
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
----------------------------------------------------------------------------------------------------
-- remove usp_PositionForReconcile_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_PositionForReconcile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_PositionForReconcile_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_BoPositionForReconcile_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoPositionForReconcile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoPositionForReconcile_Get]
GO

CREATE PROCEDURE dbo.usp_BoPositionForReconcile_Get
	@lastUTCTradeDate datetime = null
AS

	if @lastUTCTradeDate is null set @lastUTCTradeDate = getdate()

	select 
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		null as iOptionRootID,
		null as vcOptionRootSymbol,
		null as iExpiryID,
		null as fStrike,
		null as bIsCall,
		c.contractID as iUnderlyingContractID,
		c.contractTypeID as iUnderlyingContractTypeID,
		c.Symbol as vcUnderlyingSymbol,
		1 as iLotSize,
		cp.priceClose as fPriceClose,
		trdr.TraderID as iTraderID,
		trdr.acronym as vcAcronym,
		sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
		tr.strategyId as iStrategyID,
		str.strategyName as vcStrategyName
	from TradeView tr 
		left join ContractView c  on tr.contractID = c.contractID
		left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		left join TraderView trdr on tr.traderID = trdr.traderID
		left join StrategyView str on str.strategyID = tr.strategyID
	where c.contractTypeID < 3
		and tr.status in (0, 3)
		and datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0
	group by c.contractID, c.contractTypeID, c.Symbol,cp.priceClose, trdr.TraderID, trdr.acronym, 
		tr.strategyId, str.strategyName
	
	union

	select 
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		ORt.optionRootID as iOptionRootID,
		ORt.symbol as vcOptionRootSymbol,
		o.expiryID as iExpiryID,
		o.strike as fStrike,
		o.isCall as bIsCall,
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		ORt.lotSize as iLotSize,
		cp.priceClose as fPriceClose,
		trdr.TraderID as iTraderID,
		trdr.acronym as vcAcronym,
		sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
		tr.strategyId as iStrategyID,
		str.strategyName as vcStrategyName
	from TradeView tr 
		left join ContractView C on tr.contractID = C.contractID
		left join OptionView o on c.contractID = o.optionID
		left join OptionRootView ORt on o.optionRootID = ORt.optionRootID
		left join ContractView UC on ORt.underlyingID = UC.contractID
		left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		left join TraderView trdr on tr.traderID = trdr.traderID
		left join StrategyView str on str.strategyID = tr.strategyID

	where c.contractTypeID = 3
		and tr.status in (0, 3)
		and datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0
	group by C.contractID, C.contractTypeID, C.Symbol,ORt.optionRootID,ORt.symbol,
		o.expiryID, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
		ORt.lotSize, cp.priceClose, trdr.TraderID, trdr.acronym, tr.strategyId, str.strategyName

	order by vcSymbol
GO

----------------------------------------------------------------------------------------------------
-- usp_Stock_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Save]
GO

CREATE PROCEDURE dbo.usp_Stock_Save
        @iStockID int,
        @iExchangeID int = Null,
        @vcSymbol varchar(20) = Null,
        @vcContractName varchar(255) = Null,
		@bIsHTB  bit = Null,
        @iDivFreq int = Null,
		@fDivAmt float = Null,
		@dtDivDate smalldatetime = Null,
		@tiIsDivCustom tinyint = 0,
		@fDivAmtCustom float = Null,
		@iDivFreqCustom int = Null,
		@dtDivDateCustom smalldatetime = Null,
		@tiCalcOptionType tinyint = null,
		@tiCalcModelType tinyint = null,
		@iExpCalendarID int = null,
		@tiIsActive tinyint = null,
		@fSkew float = null,
		@fKurt float = null,
		@tiCheckExistance tinyint = 1,
		@iUndPriceProfileID int = null output,
		@iOptPriceProfileID int = null output
  as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @bIsHTB is null 
		set @bIsHTB=0

	if @iExpCalendarID is null or @iExpCalendarID = 0
		set @iExpCalendarID = 1

	if len(ltrim(@vcSymbol)) = 0
		begin
			Raiserror ('Empty stock ticker!', 16, 1)
			Return (-1)
		end   
   
	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID in (1, 2) and actionID < 3 and (@iStockID <> contractID or @iStockID is null))
		begin
			if isnull(@tiCheckExistance, 1) <> 0
				Raiserror ('Stock or index with this ticker already exists!', 16, 1)
			Return (-1)
		end   
   if (@iStockID is Null)  
     begin
         begin tran
			if @iUndPriceProfileID is null
			begin
				select
					@iUndPriceProfileID = priceProfileID
				from
					PriceProfileView
				where
					defaultID = 1
					and isOptionProfile = 0
			end

			if @iOptPriceProfileID is null
			begin
				select
					@iOptPriceProfileID = priceProfileID
				from
					PriceProfileView
				where
					defaultID = 1
					and isOptionProfile = 1
			end

            insert into Contract (contractTypeID, Symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
                  values (2, @vcSymbol, @vcContractName, @iExpCalendarID, @iUndPriceProfileID, @iOptPriceProfileID)
			if (@@error <> 0) begin rollback tran return(-1) end        

            set @iStockID = @@identity         
 
            insert into Stock (
				stockID, 
                divFreq, 
				isHTB,
				primaryExchangeID,
				divAmt,
				divDate,
				isDivCustom,
				divAmtCustom,
				divFreqCustom,
				divDateCustom,
				calcOptionType,
				calcModelType,
				isActive,
				skew,
				kurt)
            values (
				@iStockID, 
                @iDivFreq, 
				@bIsHTB,
				@iExchangeID,
				@fDivAmt,
				@dtDivDate,
				@tiIsDivCustom,
				@fDivAmtCustom,
				@iDivFreqCustom,
				@dtDivDateCustom,
				@tiCalcOptionType,
				@tiCalcModelType,
				@tiIsActive,
				@fSkew,
				@fKurt)
			if (@@error <> 0) begin rollback tran return(-1) end           
			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iStockID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@iStockID,null)
					if (@@error <> 0) begin rollback tran return(-1) end           
				end 
         commit tran
     end
   else
     begin

        declare @df int

        select @df = DivFreq from Stock where stockID = @iStockID        
        begin tran
           update Contract
                set symbol = isNull(@vcSymbol,symbol),
                    contractName = isNull(@vcContractName,contractName),
					expCalendarID = isNull(@iExpCalendarID,expCalendarID),
					undPriceProfileID = isnull(@iUndPriceProfileID, undPriceProfileID),
					optPriceProfileID = isnull(@iOptPriceProfileID, optPriceProfileID)
	            where contractID = @iStockID
			if (@@error <> 0) begin rollback tran return(-1) end         

			update Stock
	               set  
	                    divFreq = isNull(@iDivFreq,divFreq), 
						isHTB = isNull(@bIsHTB, isHTB),
						primaryExchangeID = isNull(@iExchangeID,primaryExchangeID),
						DivAmt = isNull(@fDivAmt, DivAmt),
						DivDate = isNull(@dtDivDate, DivDate),
						isDivCustom = isNull(@tiIsDivCustom, isDivCustom),
						divAmtCustom = isNull(@fDivAmtCustom, divAmtCustom),
						divFreqCustom = isNull(@iDivFreqCustom, divFreqCustom),
						divDateCustom = isNull(@dtDivDateCustom, divDateCustom),
						calcOptionType = isNull(@tiCalcOptionType, calcOptionType),
						calcModelType = isNull(@tiCalcModelType, calcModelType),
						isActive = isnull(@tiIsActive, isActive),
						skew = isNull(@fSkew,skew),
						kurt = isNull(@fKurt,kurt) 
		          where stockID = @iStockID
    	       if (@@error <> 0) begin rollback tran return(-1) end         

			if @iDivFreq = 0 
				begin
					update Stock set divAmt = null, divDate = null where stockID = @iStockID
				end

			if @iDivFreqCustom = 0 
				begin
					update Stock set divAmtCustom = null, divDateCustom = null where stockID = @iStockID
				end

         commit tran
     end        

 if (@@error <> 0)
    return (-1)
  else  
    return @iStockID
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTraderContract_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTraderContract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoTraderContract_Get]
GO
CREATE  PROC dbo.usp_BoTraderContract_Get
	@iTraderID int = null,
	@iContractID int = null
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	
	if @iContractID is null 
	begin
		if @iTraderID is null
			select 
				TV.traderID as iTraderID,
				TV.contractID as iContractID,
				TV.strategyID as iStrategyID,
				S.strategyName as vcStrategyName,
				C.symbol as vcContractSymbol
			from TraderContractView TV
				left join StrategyView S on  S.strategyID = TV.strategyID
				join ContractView C on  C.contractID = TV.contractID
			order by TV.traderID, C.symbol
		else
			select 
				TV.traderID as iTraderID,
				TV.contractID as iContractID,
				TV.strategyID as iStrategyID,
				S.strategyName as vcStrategyName,
				C.symbol as vcContractSymbol
			from TraderContractView TV
				left join StrategyView S on  S.strategyID = TV.strategyID
				join ContractView C on  C.contractID = TV.contractID
			where 
				TV.traderID = @iTraderID
			order by TV.traderID, C.symbol
	end
	else
	begin
		if @iTraderID is null
			select 
				TV.traderID as iTraderID,
				TV.contractID as iContractID,
				TV.strategyID as iStrategyID,
				S.strategyName as vcStrategyName,
				C.symbol as vcContractSymbol
			from TraderContractView TV
				left join StrategyView S on  S.strategyID = TV.strategyID
				join ContractView C on  C.contractID = TV.contractID
			where
				TV.contractID = @iContractID
			order by TV.traderID, C.symbol
		else
			select 
				TV.traderID as iTraderID,
				TV.contractID as iContractID,
				TV.strategyID as iStrategyID,
				S.strategyName as vcStrategyName,
				C.symbol as vcContractSymbol
			from TraderContractView TV
				left join StrategyView S on  S.strategyID = TV.strategyID
				join ContractView C on  C.contractID = TV.contractID
			where 
				TV.traderID = @iTraderID
				and TV.contractID = @iContractID
			order by TV.traderID, C.symbol
	end

	return @@error

GO
----------------------------------------------------------------------------------------------------
-- usp_BoTradeNewContract_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTradeNewContract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTradeNewContract_Get]
GO

CREATE PROC dbo.usp_BoTradeNewContract_Get
	@dtLastTradeDateUTC datetime = null -- return all contracts for trades were made earlier or at @dtLastTradeDateUTC
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		c.exportSymbol as vcExportSymbol,
	
		null as iExpiryID,
		null as fStrike,
		null as tiIsCall,
	
		c.contractID as iUnderlyingContractID,
		c.contractTypeID as iUnderlyingContractTypeID,
		c.symbol as vcUnderlyingSymbol,
		c.exportSymbol as vcUnderlyingExportSymbol,
	
		null as iOptionRootID,
		null as vcOptionRootSymbol
	from
		TradeView tr
		inner join ContractView c on tr.contractID = c.contractID
	where
		c.contractTypeID < 3
		and tr.status in (0, 3) -- only actual (or actual matched) trades should be reported
		and isnull(tr.isPosition, 0) = 0 -- only trades (not positions) should be reported
		and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
		and c.contractID not in (select contractID from TradeView 
									where isnull(isPosition, 0) = 1)
	group by
		c.contractID, c.contractTypeID, c.symbol, c.exportSymbol
	union
	select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		c.exportSymbol as vcExportSymbol,
	
		o.expiryID as iExpiryID,
		o.strike as fStrike,
		o.isCall as tiIsCall,
	
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		uc.exportSymbol as vcUnderlyingExportSymbol,
	
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol
	from
		TradeView tr
		inner join ContractView c on tr.contractID = c.contractID
		inner join OptionView o on c.contractID = o.optionID
		inner join OptionRoot ort on o.optionRootID = ort.optionRootID
		inner join Contract uc on ort.underlyingID = uc.contractID
	where
		c.contractTypeID = 3
		and tr.status in (0, 3) -- only actual (or actual matched) trades should be reported
		and isnull(tr.isPosition, 0) = 0 -- only trades (not positions) should be reported
		and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
		and c.contractID not in (select contractID from TradeView 
									where isnull(isPosition, 0) = 1)
	group by
		c.contractID, c.contractTypeID, c.symbol, c.exportSymbol, o.expiryID,
		o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
		uc.exportSymbol, ort.optionRootID, ort.symbol

GO

----------------------------------------------------------------------------------------------------
-- usp_BoTradeNew_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTradeNew_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTradeNew_Get]
GO

CREATE PROC dbo.usp_BoTradeNew_Get
	@dtLastTradeDateUTC datetime = null -- return all trades were made earlier or at @dtLastTradeDateUTC
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	select
		tr.tradeID as iTradeID,
		tr.tradeDate as dtTradeDate,
		tr.quantity as iQuantity,
		tr.price as fPrice,
		tr.isBuy as tiIsBuy,

		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		c.exportSymbol as vcExportSymbol,
	
		null as iExpiryID,
		null as fStrike,
		null as tiIsCall,
	
		c.contractID as iUnderlyingContractID,
		c.contractTypeID as iUnderlyingContractTypeID,
		c.symbol as vcUnderlyingSymbol,
		c.exportSymbol as vcUnderlyingExportSymbol,
	
		null as iOptionRootID,
		null as vcOptionRootSymbol,
		1 as iLotSize,

		t.traderID as iTraderID,
		t.acronym as vcTraderName,

		br.brokerID as iBrokerID,
		br.brokerName as vcBrokerName,
		tr.brokerCommission as fBrokerCommission,

		tr.mark as iTradeMark,

		st.strategyID as iStrategyID,
		st.strategyName as vcStrategyName,

		trg.trgID as iTraderGroupID,
		trg.trgName as vcTraderGroupName
	from
		TradeView tr
		inner join ContractView c on tr.contractID = c.contractID
		inner join TraderView t on tr.traderID = t.traderID
		left join TraderGroup trg on t.traderGroupID = trg.trgID
		left join BrokerView br on tr.brokerID = br.brokerID
		left join StrategyView st on tr.strategyID = st.strategyID
	where
		c.contractTypeID < 3
		and tr.status in (0, 3) -- only actual (or actual matched) trades should be reported
		and isnull(tr.isPosition, 0) = 0 -- only trades (not positions) should be reported
		and len(isnull(tr.execID, '')) = 0 -- only non-RTTF trades should be reported (make sure that all RTTF trades has execID)
		and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
	union
	select
		tr.tradeID as iTradeID,
		tr.tradeDate as dtTradeDate,
		tr.quantity as iQuantity,
		tr.price as fPrice,
		tr.isBuy as tiIsBuy,

		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		c.exportSymbol as vcExportSymbol,
	
		o.expiryID as iExpiryID,
		o.strike as fStrike,
		o.isCall as tiIsCall,
	
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		uc.exportSymbol as vcUnderlyingExportSymbol,
	
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,
		ort.lotSize as iLotSize,

		t.traderID as iTraderID,
		t.acronym as vcTraderName,

		br.brokerID as iBrokerID,
		br.brokerName as vcBrokerName,
		tr.brokerCommission as fBrokerCommission,

		tr.mark as iTradeMark,

		st.strategyID as iStrategyID,
		st.strategyName as vcStrategyName,

		trg.trgID as iTraderGroupID,
		trg.trgName as vcTraderGroupName
	from
		TradeView tr
		inner join ContractView c on tr.contractID = c.contractID
		inner join OptionView o on c.contractID = o.optionID
		inner join OptionRoot ort on o.optionRootID = ort.optionRootID
		inner join Contract uc on ort.underlyingID = uc.contractID
		inner join TraderView t on tr.traderID = t.traderID
		left join TraderGroup trg on t.traderGroupID = trg.trgID
		left join BrokerView br on tr.brokerID = br.brokerID
		left join StrategyView st on tr.strategyID = st.strategyID
	where
		c.contractTypeID = 3
		and tr.status in (0, 3) -- only actual (or actual matched) trades should be reported
		and isnull(tr.isPosition, 0) = 0 -- only trades (not positions) should be reported
		and len(isnull(tr.execID, '')) = 0 -- only non-RTTF trades should be reported (make sure that all RTTF trades has execID)
		and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTraderContract_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTraderContract_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTraderContract_Save]
GO

CREATE PROC dbo.usp_BoTraderContract_Save
	@iTraderID int,
	@iContractID int,
	@iStrategyID int -- set to 'null' to remove strategy association
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------

	declare @error int set @error = 0

	begin transaction
	
	if exists(select 1 from TraderContractView where traderID = @iTraderID and contractID = @iContractID)		
		update TraderContract
			set strategyID = @iStrategyID
		where
			traderID = @iTraderID 
			and contractID = @iContractID
			and actionID < 3
	else
		insert into TraderContract
			(traderID, contractID, strategyID)
		values
			(@iTraderID, @iContractID, @iStrategyID)
			
	if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end			

	if @iStrategyID is null
	begin
		declare @iDefStrategyID int
		
		select @iDefStrategyID = defStrategyID 
			from TraderView
		where traderID = @iTraderID			
	
		-- update underlyings trades' strategy
		update Trade
			set strategyID = @iDefStrategyID
		where traderID = @iTraderID
			and contractId = @iContractID

		if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end
		
		-- update options trades' strategy
		update Trade
			set strategyID = @iDefStrategyID
		from Trade T 
			join OptionView O on O.optionID=T.contractID
			join OptionRootView ORT on ORT.optionRootID=O.optionRootID
		where T.traderID = @iTraderID
			and ORT.underlyingID = @iContractID	
	end
	else
	begin 
		-- update underlyings trades' strategy
		update Trade
			set strategyID = @iStrategyID
		where traderID = @iTraderID
			and contractId = @iContractID

		if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end
		
		-- update options trades' strategy
		update Trade
			set strategyID = @iStrategyID
		from Trade T 
			join OptionView O on O.optionID=T.contractID
			join OptionRootView ORT on ORT.optionRootID=O.optionRootID
		where T.traderID = @iTraderID
			and ORT.underlyingID = @iContractID	
	end
		
	if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
	
	commit transaction
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoOptionByParams_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoOptionByParams_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoOptionByParams_Get]
GO

CREATE  PROC dbo.usp_BoOptionByParams_Get
	@iUnderlyingID int, 
	@iExpiryID int,
	@fStrike float,
	@tiIsCall tinyint
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	select	C.contractID as iContractID,
			C.symbol as vcSymbol,
			C.exportSymbol as vcExportSymbol, 
			ORt.optionRootID as iOptionRootID,
			ORt.symbol as vcOptionRootSymbol
			from ContractView C  
				inner join OptionView O on O.optionID = C.contractID
				join OptionRoot ORt on O.optionRootID = ORt.optionRootID
			where c.contractTypeID=3
				and O.expiryID = @iExpiryID
				and ORt.underlyingID = @iUnderlyingID
				and abs(@fStrike - O.strike) < 0.001
				and O.isCall = @tiIsCall
			order by ORt.symbol

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_PositionTransfer
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_PositionTransfer]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_PositionTransfer]
GO

CREATE PROC dbo.usp_Trade_PositionTransfer
	@iContractID int,
	@iQuantity int,
	@fPrice float,
	@tiStatus tinyint,

	@iTraderIDFrom int,
	@iMarkFrom int,
    @tiIsBuyFrom tinyint,  

	@iTraderIDTo int,
	@iMarkTo int,
    @tiIsBuyTo tinyint,  

	@iSeqNumFrom int out,
	@iTradeIDFrom int out,
	@iStrategyIDFrom int out,
	@dtTradeDateFrom datetime out,

	@iSeqNumTo int out,
	@iTradeIDTo int out,
	@iStrategyIDTo int out,
	@dtTradeDateTo datetime out
as
	set nocount on

	begin tran

	declare @error int		set @error = 0

	declare @iDeltaGMT int
	exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

	-- Trade 'from' creation
	if @dtTradeDateFrom is null	
		set @dtTradeDateFrom = dateadd(minute, @iDeltaGMT, getdate())

	-- detemine strategy for trade from
	exec @error = usp_TraderContractStrategyWithSave_Get @iTraderIDFrom, @iContractID, @iStrategyIDFrom out
	if @@error <> 0 or @error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

	-- get new tradeID ane seqNum for trade from
	select 
		@iTradeIDFrom = isnull(max(tradeID), 0) + 1,
		@iSeqNumFrom = isnull(max(seqNum), 0) + 1
	from Trade

	-- create new trade from
	insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, strategyID,
		isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, seqNum, mark)
	values (@iTradeIDFrom, 1, @iContractID, @dtTradeDateFrom, @iQuantity, @fPrice, @iTraderIDFrom, @iStrategyIDFrom,
		@tiIsBuyFrom, null, null, null, null,
		null, null, 0, @tiStatus, @iSeqNumFrom, @iMarkFrom)

	if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

	-- Trade 'to' creation
	if @dtTradeDateTo is null	
		set @dtTradeDateTo = dateadd(minute, @iDeltaGMT, getdate())

	-- detemine strategy for trade to
	exec @error = usp_TraderContractStrategyWithSave_Get @iTraderIDTo, @iContractID, @iStrategyIDTo out
	if @@error <> 0 or @error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

	-- get new tradeID ane seqNum for trade to
	select 
		@iTradeIDTo = isnull(max(tradeID), 0) + 1,
		@iSeqNumTo = isnull(max(seqNum), 0) + 1
	from Trade

	-- create new trade to
	insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, strategyID,
		isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, seqNum, mark)
	values (@iTradeIDTo, 1, @iContractID, @dtTradeDateTo, @iQuantity, @fPrice, @iTraderIDTo, @iStrategyIDTo,
		@tiIsBuyTo, null, null, null, null,
		null, null, 0, @tiStatus, @iSeqNumTo, @iMarkTo)

	if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

	commit tran

finish:
	if @error <> 0
	begin
		set @iSeqNumFrom = null
		set @iTradeIDFrom = null
		set @iStrategyIDFrom = null
		set @dtTradeDateFrom = null
	
		set @iSeqNumTo = null
		set @iTradeIDTo = null
		set @iStrategyIDTo = null
		set @dtTradeDateTo = null
	end

	return @error
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.45.102')
	begin
		update DataInfo set keyValue = '3.45.102' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.45.102' '3.45.102 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
