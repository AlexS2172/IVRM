/*
This script adds ability to work with Option roots, 
PLEASE CLOSE ALL USER CONNECTONS AND BACKUP DATABASE BEFORE RUN THIS SCRIPT.

If errors will be appear after execution of this script, 
please e-mail to ETS_support@egar.egartech.com and don't run next script.

After execution of this script you must run EMM_FW_update20021030_1.sql to commit 
DB changes.
DB version for update: 3.37.036
DB version in script:  3.38.037
*/
set nocount on
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
ALTER    procedure dbo.job_FixMarketStructure
	@iNumberOfHours int = null,
	@iContractID int = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure mark us deleted unusefull options
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iNumberOfHours is null 
		set @iNumberOfHours = 2

	Create table #Options_for_delete(OptionID int)

	insert into #Options_for_delete
		select O.optionID--,E.expiryDate 
			from 
				[Option] O
					join Contract UC on O.underlyingContractID = UC.contractID 
					join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
			where E.expiryDate < GetDate() 
					and O.isManualyEntered = 0
					and O.actionID < 3
					and O.optionID not in (select distinct ContractID from Trade)
	exec usp_Option_Del
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t mark us delete useless options!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

    set @error = 0
finish:
		Return(@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--first: Cleanup database - remove expired options
if object_id('OptionRoot') is null
	begin
		exec job_FixMarketStructure
		exec job_Cleanup_DB
	end
GO
--2. Add optionRootID field into Option table
if not  exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Option' and COLUMN_NAME='optionRootID')
	Alter Table [Option] 
		Add optionRootID int NULL
GO
--3. Create OptionRoot table
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_OptionRoot]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[tU_OptionRoot]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_Option_OptionRoot]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[Option] DROP CONSTRAINT FK_Option_OptionRoot
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_SyntheticOptionRoot_OptionRoot1]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[SyntheticOptionRoot] DROP CONSTRAINT FK_SyntheticOptionRoot_OptionRoot1
GO
--if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[OptionRoot]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
--drop table [dbo].[OptionRoot]
GO
if object_id('OptionRoot') is null
CREATE TABLE [dbo].[OptionRoot] (
	[optionRootID] [int] IDENTITY (1, 1) NOT NULL ,
	[underlyingID] [int] NOT NULL ,
	[symbol] [varchar] (20) NOT NULL ,
	[isSynthetic] [tinyint] NOT NULL ,
	[lotSize] [int] NOT NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO
--if object_id('OptionRoot') is not null and not exists(select * from OptionRoot)
if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PK_OptionRoot]') and OBJECTPROPERTY(id, N'IsPrimaryKey') = 1)
		ALTER TABLE [dbo].[OptionRoot] WITH NOCHECK ADD 
			CONSTRAINT [PK_OptionRoot] PRIMARY KEY  CLUSTERED 
			(
				[optionRootID]
			)  ON [PRIMARY] 
GO
if object_id('DF_OptionRoot_isSynthetic') is null
ALTER TABLE [dbo].[OptionRoot] WITH NOCHECK ADD 
	CONSTRAINT [DF_OptionRoot_isSynthetic] DEFAULT (0) FOR [isSynthetic],
	CONSTRAINT [DF_OptionRoot_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_OptionRoot_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO
if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_OptionRoot_Contract]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[OptionRoot] ADD 
	CONSTRAINT [FK_OptionRoot_Contract] FOREIGN KEY 
	(
		[underlyingID]
	) REFERENCES [dbo].[Contract] (
		[contractID]
	)
GO
--4. fill up OptionRoot table
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Option' and COLUMN_NAME='underlyingContractID')
	begin
		exec('insert into OptionRoot(symbol,underlyingID,lotSize)
				select distinct left(symbol,len(symbol)-2),min(o.underlyingContractID),100
					from [Option] O 
							join Contract C on O.optionID = C.ContractID
					group by left(symbol,len(symbol)-2)
					order by left(symbol,len(symbol)-2)')
	end
--update optionRootID field in Option table
if exists(select * from [Option] where optionRootID is null)
	update [Option]
		set optionRootID = ort.optionRootID
		from Contract c
			join [Option] O on c.contractID = o.optionID
			join OptionRoot ort on left(c.symbol,len(c.symbol)-2) = ort.symbol
		where c.contractTypeID = 3
GO
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Option' and COLUMN_NAME='optionRootID')
	Alter Table [Option] 
		Alter Column optionRootID int NOT NULL
GO
-- drop underlyingID column from [Option] table
if object_id('tempdb..#OptionIndices') is not null Drop table #OptionIndices
	create table #OptionIndices(index_name sysname,index_description varchar(210),index_keys nvarchar(2078))
	insert into #OptionIndices
		exec sp_helpindex [Option] 

Declare o_cursor cursor read_only for select * from #OptionIndices where index_keys like '%underlyingContractID%'
Declare @index_name sysname,
		@index_description varchar(210),
		@index_keys nvarchar(2078)
Open o_cursor
fetch next from o_cursor into @index_name, @index_description, @index_keys
while (@@fetch_status <> -1)
begin
	if (@@fetch_status <> -2)
	begin
		if exists(select * from sysindexes where [id]=object_id('Option') and indid between 0 and 255 and [name] = @index_name)
			begin
				exec ('DROP INDEX [OPTION].' + @index_name)
			end
	end
	fetch next from o_cursor into @index_name, @index_description, @index_keys
end
close o_cursor
deallocate o_cursor
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_Option_Contract1]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
	ALTER TABLE [Option] DROP CONSTRAINT [FK_Option_Contract1]
GO
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Option' and COLUMN_NAME='underlyingContractID')
	Alter Table [Option] 
		Drop column underlyingContractID
GO
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractPrice' and COLUMN_NAME='lotSize')
	Alter Table ContractPrice 
		Drop column lotSize
GO
--check the default constraints for lotSize column in Trade table
declare @vcConstraintName varchar(4000)
	select @vcConstraintName = object_name(constid)
		from sysconstraints s1
			join syscolumns s2 on s1.colid = s2.colid and s1.id = s2.id
		where s2.id = object_id('Trade') and s2.name = 'lotSize'

if @vcConstraintName is not null
	exec ('ALTER TABLE Trade DROP CONSTRAINT ' + @vcConstraintName)
--drop the lotSize from Trade
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='lotSize')
	Alter Table Trade 
		Drop column lotSize
GO
--rename priceDate to the actionDate in ContractPrice table
declare @vcConstraintName varchar(4000)
	select @vcConstraintName = object_name(constid)
		from sysconstraints s1
			join syscolumns s2 on s1.colid = s2.colid and s1.id = s2.id
		where s2.id = object_id('ContractPrice') and s2.name = 'priceDate'
if @vcConstraintName is not null
	exec ('ALTER TABLE ContractPrice DROP CONSTRAINT ' + @vcConstraintName)
GO
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractPrice' and COLUMN_NAME='priceDate')
	Alter Table ContractPrice
		Drop column priceDate
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractPrice' and COLUMN_NAME='actionDate')
	Alter Table ContractPrice 
		Add actionDate datetime null
GO
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractPrice' and COLUMN_NAME='actionDate')
	exec('UPDATE ContractPrice SET actionDate = GetDate()')
GO
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractPrice' and COLUMN_NAME='actionDate')
	Alter Table ContractPrice 
		Alter Column actionDate datetime  not null
GO
if object_id('DF_ContractPrice_actionDate') is null
begin
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ContractPrice' and COLUMN_NAME='actionDate')
	ALTER TABLE [dbo].[ContractPrice] WITH NOCHECK ADD 
		CONSTRAINT [DF_ContractPrice_actionDate] DEFAULT (getdate()) FOR [actionDate]
end
GO
--===================================================================================================================
-- Creation of SyntheticUnderlyingParam & SyntheticOptionRoot tables
--===================================================================================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_SyntheticUnderlyingParam_SyntheticOptionRoot]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[SyntheticUnderlyingParam] DROP CONSTRAINT FK_SyntheticUnderlyingParam_SyntheticOptionRoot
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[SyntheticOptionRoot]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[SyntheticOptionRoot]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[SyntheticUnderlyingParam]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[SyntheticUnderlyingParam]
GO
CREATE TABLE [dbo].[SyntheticOptionRoot] (
	[syntheticOptRootID] [int] IDENTITY (1, 1) NOT NULL ,
	[optionRootID] [int] NOT NULL ,
	[synthSkew] [float] NOT NULL ,
	[synthKurt] [float] NOT NULL ,
	[synthYield] [float] NOT NULL ,
	[synthIsBasket] [tinyint] NOT NULL ,
	[cashValue] [float] NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO
CREATE TABLE [dbo].[SyntheticUnderlyingParam] (
	[syntheticUndParamID] [int] IDENTITY (1, 1) NOT NULL ,
	[syntheticOptRootID] [int] NOT NULL ,
	[underlyingID] [int] NOT NULL ,
	[syntheticCoeff] [float] NOT NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[SyntheticOptionRoot] WITH NOCHECK ADD 
	CONSTRAINT [PK_SyntheticOptionRoot] PRIMARY KEY  CLUSTERED 
	(
		[syntheticOptRootID]
	)  ON [PRIMARY] 
GO
ALTER TABLE [dbo].[SyntheticUnderlyingParam] WITH NOCHECK ADD 
	CONSTRAINT [PK_SynteticUnderlyingParam] PRIMARY KEY  CLUSTERED 
	(
		[syntheticUndParamID]
	)  ON [PRIMARY] 
GO
ALTER TABLE [dbo].[SyntheticOptionRoot] WITH NOCHECK ADD 
	CONSTRAINT [DF_SyntheticOptionRoot_synthSkew] DEFAULT (0) FOR [synthSkew],
	CONSTRAINT [DF_SyntheticOptionRoot_synthKurt] DEFAULT (0) FOR [synthKurt],
	CONSTRAINT [DF_SyntheticOptionRoot_synthYield] DEFAULT (0) FOR [synthYield],
	CONSTRAINT [DF_SyntheticOptionRoot_synthIsBasket] DEFAULT (0) FOR [synthIsBasket],
	CONSTRAINT [DF_SyntheticOptionRoot_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_SyntheticOptionRoot_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO
ALTER TABLE [dbo].[SyntheticUnderlyingParam] WITH NOCHECK ADD 
	CONSTRAINT [DF_SyntheticUnderlyingParam_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_SyntheticUnderlyingParam_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO
ALTER TABLE [dbo].[SyntheticOptionRoot] ADD 
	CONSTRAINT [FK_SyntheticOptionRoot_OptionRoot1] FOREIGN KEY 
	(
		[optionRootID]
	) REFERENCES [dbo].[OptionRoot] (
		[optionRootID]
	)
GO
ALTER TABLE [dbo].[SyntheticUnderlyingParam] ADD 
	CONSTRAINT [FK_SynteticUnderlyingParam_Contract] FOREIGN KEY 
	(
		[underlyingID]
	) REFERENCES [dbo].[Contract] (
		[contractID]
	),
	CONSTRAINT [FK_SyntheticUnderlyingParam_SyntheticOptionRoot] FOREIGN KEY 
	(
		[syntheticOptRootID]
	) REFERENCES [dbo].[SyntheticOptionRoot] (
		[syntheticOptRootID]
	)
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_Option_OptionRoot]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
	ALTER TABLE [dbo].[Option] DROP CONSTRAINT FK_Option_OptionRoot
GO
ALTER TABLE [dbo].[Option] ADD 
	CONSTRAINT [FK_Option_OptionRoot] FOREIGN KEY 
	(
		[optionRootID]
	) REFERENCES [dbo].[OptionRoot] (
		[optionRootID]
	)
GO
--===================================================================================================================
-- Creation of CALog tables
--===================================================================================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_CorporateActionLog_CorporateAction]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[CorporateActionLog] DROP CONSTRAINT FK_CorporateActionLog_CorporateAction
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_CorporateAction_CorporateActionType]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[CorporateAction] DROP CONSTRAINT FK_CorporateAction_CorporateActionType
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[CorporateAction]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[CorporateAction]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[CorporateActionLog]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[CorporateActionLog]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[CorporateActionType]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[CorporateActionType]
GO
CREATE TABLE [dbo].[CorporateAction] (
	[corpActionID] [int] IDENTITY (1, 1) NOT NULL ,
	[typeID] [int] NOT NULL ,
	[effectiveDate] [smalldatetime] NOT NULL ,
	[underlyingID] [int] NOT NULL ,
	[statusID] [int] NOT NULL 
) ON [PRIMARY]
GO
CREATE TABLE [dbo].[CorporateActionLog] (
	[corpActionLogID] [int] IDENTITY (1, 1) NOT NULL ,
	[corpActionID] [int] NOT NULL ,
	[tableName] [varchar] (50) NOT NULL ,
	[keyRow] [varchar] (50) NOT NULL ,
	[rowID] [int] NOT NULL ,
	[fieldName] [varchar] (50) NOT NULL ,
	[fieldValue] [varchar] (250) NULL ,
	[fieldType] [varchar] (50) NOT NULL ,
	[actionID] [tinyint] NOT NULL ,
	[actionDate] [datetime] NOT NULL 
) ON [PRIMARY]
GO
CREATE TABLE [dbo].[CorporateActionType] (
	[typeID] [int] NOT NULL ,
	[typeName] [varchar] (250) NOT NULL 
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[CorporateAction] WITH NOCHECK ADD 
	CONSTRAINT [PK_CorporateAction] PRIMARY KEY  CLUSTERED 
	(
		[corpActionID]
	)  ON [PRIMARY] 
GO
ALTER TABLE [dbo].[CorporateActionLog] WITH NOCHECK ADD 
	CONSTRAINT [PK_CorporateActionLog] PRIMARY KEY  CLUSTERED 
	(
		[corpActionLogID]
	)  ON [PRIMARY] 
GO
ALTER TABLE [dbo].[CorporateActionType] WITH NOCHECK ADD 
	CONSTRAINT [PK_CorporateActionType] PRIMARY KEY  CLUSTERED 
	(
		[typeID]
	)  ON [PRIMARY] 
GO
ALTER TABLE [dbo].[CorporateAction] WITH NOCHECK ADD 
	CONSTRAINT [DF_CorporateAction_effectiveDate] DEFAULT (getdate()) FOR [effectiveDate],
	CONSTRAINT [DF_CorporateAction_statusID] DEFAULT (0) FOR [statusID],
	CONSTRAINT [IX_CorporateAction] UNIQUE  NONCLUSTERED 
	(
		[typeID],
		[underlyingID],
		[effectiveDate]
	)  ON [PRIMARY] 
GO
ALTER TABLE [dbo].[CorporateActionLog] WITH NOCHECK ADD 
	CONSTRAINT [DF_CorporateActionLog_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO
ALTER TABLE [dbo].[CorporateAction] ADD 
	CONSTRAINT [FK_CorporateAction_CorporateActionType] FOREIGN KEY 
	(
		[typeID]
	) REFERENCES [dbo].[CorporateActionType] (
		[typeID]
	)
GO
ALTER TABLE [dbo].[CorporateActionLog] ADD 
	CONSTRAINT [FK_CorporateActionLog_CorporateAction] FOREIGN KEY 
	(
		[corpActionID]
	) REFERENCES [dbo].[CorporateAction] (
		[corpActionID]
	)
GO
--===================================================================================
--change DB version
--===================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.38.037')
	begin
		update DataInfo set keyValue = '3.38.037' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.38.037'
	end



