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
/*BEGIN DATA MODIFICATION SECTION*/

/*FutUnd_4_Tables**********************************/
--Created by Nikita Kychkin
--14.08.2006
/*******************************************/

/*==========================================================================================*/
/*TABLES*/
/*==========================================================================================*/
/*Contract*/
if exists (select * from dbo.sysindexes where name = N'IDX_Contract_ContractType' and id = object_id(N'[dbo].[Contract]'))
drop index [dbo].[Contract].[IDX_Contract_ContractType]
go
create index IDX_Contract_ContractType on Contract(contractTypeID,symbol,actionID,contractID)
go
if exists (select * from dbo.sysindexes where name = N'Contract_2222' and id = object_id(N'[dbo].[Contract]'))
drop index [dbo].[Contract].[Contract_2222]
go
create index Contract_2222 on Contract(contractID,actionID,expCalendarID,contractTypeID,symbol)
go

/*ContractPrice*/
if exists (select * from dbo.sysindexes where name = N'IX_ContractPrice_27' and id = object_id(N'[dbo].[ContractPrice]'))
drop index [dbo].[ContractPrice].[IX_ContractPrice_27]
go
create index IX_ContractPrice_27 on ContractPrice(contractID,exchangeID,priceClose,priceTheo)
go

/*CustomStrikeSkewPoint*/--
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_CustomStrikeSkewPoint_VolaManagementData]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
alter table [dbo].[CustomStrikeSkewPoint] drop constraint [FK_CustomStrikeSkewPoint_VolaManagementData]
go
alter table CustomStrikeSkewPoint add
  constraint FK_CustomStrikeSkewPoint_VolaManagementData foreign key(volaSurfaceDataID) references VolaSurfaceData(volaSurfaceDataID)
go

/*ExpiryCalendar*/
if not  exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ExpiryCalendar' and COLUMN_NAME='bBelongsToFutureRoot')
alter table ExpiryCalendar add
  bBelongsToFutureRoot tinyint
go

--if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AK_ExpiryCalendarexpCalendarName]'))
--alter table ExpiryCalendar drop constraint [AK_ExpiryCalendarexpCalendarName]
--go
--alter table ExpiryCalendar
--  add constraint AK_ExpiryCalendarexpCalendarName
--  unique (expCalendarName)
--go

/*FutureOption*/
if exists (select * from dbo.sysindexes where name = N'IDX_FutureOption_ID' and id = object_id(N'[dbo].[FutureOption]'))
drop index [dbo].[FutureOption].[IDX_FutureOption_ID]
go
create index IDX_FutureOption_ID on FutureOption(futureOptionID,isCall,expiryID,strike)
go

/*FutureRoot*/
if not  exists 
	(select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and 
		COLUMN_NAME='tiQuoteFormat')
alter table FutureRoot add
  tiQuoteFormat tinyint
go

if not  exists 
	(select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and 
		COLUMN_NAME='iQuotationUnitAmmount')
alter table FutureRoot add
  iQuotationUnitAmmount int
go

if not  exists 
	(select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and 
		COLUMN_NAME='fQuoteDenomination')
alter table FutureRoot add
  fQuoteDenomination float
go

if not  exists 
	(select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and 
		COLUMN_NAME='fTickSize')
alter table FutureRoot add
  fTickSize float
go

if not  exists 
	(select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and 
		COLUMN_NAME='fTickValue')
alter table FutureRoot add
  fTickValue float
go

if not  exists 
	(select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and 
		COLUMN_NAME='iQuotationUnitID')
alter table FutureRoot add
  iQuotationUnitID int
go

if not  exists 
	(select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and 
		COLUMN_NAME='vcQuotationUnitCurrency')
alter table FutureRoot add
  vcQuotationUnitCurrency varchar(3)
go

/*FutureUnderlying*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FutureUnderlying]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[FutureUnderlying]
go
create table FutureUnderlying(
  iFutureUnderlyingID     int     not null constraint PK_FutureUnderlying primary key,
  iFutureUnderlyingTypeID int     not null,
  tiGreeksCalculationType tinyint not null,
  tiActive                tinyint not null
)
go
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_FutureUnderlying_Contract]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
alter table [dbo].[FutureUnderlying] drop constraint [FK_FutureUnderlying_Contract]
go
alter table FutureUnderlying add
  constraint FK_FutureUnderlying_Contract foreign key(iFutureUnderlyingID) references Contract(contractID)
go

/*FutureUnderlyingType*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FutureUnderlyingType]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[FutureUnderlyingType]
go
create table FutureUnderlyingType(
  iFutureUnderlyingTypeID int           not null constraint PK_FutureUnderlyingType primary key,
  vcName                  nvarchar(255) not null,
  vcDesc                  nvarchar(255)
)
go

/*IndexBeta*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_IndexBeta_Contract]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
alter table [dbo].[IndexBeta] drop constraint [FK_IndexBeta_Contract]
go
alter table IndexBeta add
  constraint FK_IndexBeta_Contract foreign key(indexID) references Contract(contractID)
go

/*NextDaySurface*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_NextDaySurface_VolaSurfaceData]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
alter table [dbo].[NextDaySurface] drop constraint [FK_NextDaySurface_VolaSurfaceData]
go
alter table NextDaySurface add
  constraint FK_NextDaySurface_VolaSurfaceData foreign key(volaSurfaceDataID) references VolaSurfaceData(volaSurfaceDataID) on delete cascade
go

/*[Option]*/
if exists (select * from dbo.sysindexes where name = N'IDX_Option_28' and id = object_id(N'[dbo].[Option]'))
drop index [dbo].[Option].[IDX_Option_28]
go
create index IDX_Option_28 on [Option](optionID,isCall,strike,expiryID)
go

/*QuotationUnit*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[QuotationUnit]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[QuotationUnit]
go
create table QuotationUnit(
  iQuotationUnitID int           not null constraint PK_QuotationUnit primary key,
  vcName           nvarchar(255) not null,
  vcDesc           nvarchar(255)
)
go

/*SyntheticUnderlyingParam*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_SyntheticUnderlyingParam]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[tU_SyntheticUnderlyingParam]
go
create   Trigger tU_SyntheticUnderlyingParam ON SyntheticUnderlyingParam
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	-- 1st step - update actionDate if record not already deleted
	update SyntheticUnderlyingParam
		set actionDate = GetDate()
	from SyntheticUnderlyingParam S 
			join inserted on S.syntheticUndParamID = inserted.syntheticUndParamID
			join deleted on S.syntheticUndParamID = deleted.syntheticUndParamID
	where deleted.actionID < 3

	-- 2nd step - if update record -> set actionID=2
	update SyntheticUnderlyingParam
		set actionID=2
	from SyntheticUnderlyingParam S
			join inserted on S.syntheticUndParamID = inserted.syntheticUndParamID
			join deleted on S.syntheticUndParamID = deleted.syntheticUndParamID
	where deleted.actionID<3 and inserted.actionID = 1

	-- 3d step -disable restore of deleted record
	update SyntheticUnderlyingParam 
		set actionID = 3
	from SyntheticUnderlyingParam S
			join inserted on S.syntheticUndParamID = inserted.syntheticUndParamID
			join deleted on S.syntheticUndParamID = deleted.syntheticUndParamID
	where deleted.actionID=3
go
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[DF_strikeSettlement]'))
alter table SyntheticUnderlyingParam drop constraint [DF_strikeSettlement]
go
alter table SyntheticUnderlyingParam add
  constraint DF_strikeSettlement default (0) for strikeSettlement
go

/*TmpClosePriceVolETS*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TmpClosePriceVolETS]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[TmpClosePriceVolETS]
go
create table TmpClosePriceVolETS(
  Symbol           nvarchar(50),
  futureRootSymbol nvarchar(50),
  ExpiryPeriod     nvarchar(10),
  ContrName        nvarchar(50),
  call             int,
  strike           float,
  ClosePrice       float,
  Volatility       float,
  typeContr        nvarchar(10)
)
go

/*Trade*/
--alter table Trade add
--  seqNum int not null constraint PK_Trade primary key constraint DFX_350B740A_025F default(0)
--go
--alter table Trade drop
--   constraint DFX_350B740A_025F
--go
if exists (select * from dbo.sysindexes where name = N'I_Trades_Contract' and id = object_id(N'[dbo].[Trade]'))
drop index [dbo].[Trade].[I_Trades_Contract]
go
create index I_Trades_Contract on Trade(contractID)
go
if exists (select * from dbo.sysindexes where name = N'IX_Trade_UnderlyingID' and id = object_id(N'[dbo].[Trade]'))
drop index [dbo].[Trade].[IX_Trade_UnderlyingID]
go
create index IX_Trade_UnderlyingID on Trade(underlyingID)
go
if exists (select * from dbo.sysindexes where name = N'I_Trades_Broker' and id = object_id(N'[dbo].[Trade]'))
drop index [dbo].[Trade].[I_Trades_Broker]
go
create index I_Trades_Broker on Trade(brokerID)
go
if exists (select * from dbo.sysindexes where name = N'I_Trades_ClearingBroker' and id = object_id(N'[dbo].[Trade]'))
drop index [dbo].[Trade].[I_Trades_ClearingBroker]
go
create index I_Trades_ClearingBroker on Trade(clearingBrokerID)
go
if exists (select * from dbo.sysindexes where name = N'I_Trades_Strategy' and id = object_id(N'[dbo].[Trade]'))
drop index [dbo].[Trade].[I_Trades_Strategy]
go
create index I_Trades_Strategy on Trade(strategyID)
go
if exists (select * from dbo.sysindexes where name = N'I_Trades_TradeDate' and id = object_id(N'[dbo].[Trade]'))
drop index [dbo].[Trade].[I_Trades_TradeDate]
go
create index I_Trades_TradeDate on Trade(tradeDate)
go
if exists (select * from dbo.sysindexes where name = N'traderID_ind' and id = object_id(N'[dbo].[Trade]'))
drop index [dbo].[Trade].[traderID_ind]
go
create index traderID_ind on Trade(traderID)
go
if exists (select * from dbo.sysindexes where name = N'tradeID_contractID_ind' and id = object_id(N'[dbo].[Trade]'))
drop index [dbo].[Trade].[tradeID_contractID_ind]
go
create index tradeID_contractID_ind on Trade(tradeID,contractID)
go
if exists (select * from dbo.sysindexes where name = N'actionDate_ind' and id = object_id(N'[dbo].[Trade]'))
drop index [dbo].[Trade].[actionDate_ind]
go
create index actionDate_ind on Trade(actionDate)
go
if exists (select * from dbo.sysindexes where name = N'IX_Trade_FutureID' and id = object_id(N'[dbo].[Trade]'))
drop index [dbo].[Trade].[IX_Trade_FutureID]
go
create index IX_Trade_FutureID on Trade(futureID)
go
if exists (select * from dbo.sysindexes where name = N'Trade_Strategy' and id = object_id(N'[dbo].[Trade]'))
drop index [dbo].[Trade].[Trade_Strategy]
go
create index Trade_Strategy on Trade(contractID,strategyID)
go

/*VolaSurfaceData*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_VolaSurfaceData_Contract]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
alter table [dbo].[VolaSurfaceData] drop constraint [FK_VolaSurfaceData_Contract]
go
alter table VolaSurfaceData add
  constraint FK_VolaSurfaceData_Contract foreign key(contractID) references Contract(contractID) on delete cascade
go

GO
/*END DATA MODIFICATION SECTION*/
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