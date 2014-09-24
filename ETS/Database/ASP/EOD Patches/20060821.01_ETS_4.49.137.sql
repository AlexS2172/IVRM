/*
	Contains stored procedures for ETS ASP (Institutional) database
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
--BEGIN TRANSACTION
--GO
--SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
--GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.136' or (keyValue = '4.49.137')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.137 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.136' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
/*******************************************/

/*==========================================================================================*/
/*TABLES*/
/*==========================================================================================*/

/*AspEodMsUpdateLog*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AspEodMsUpdateLog]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AspEodMsUpdateLog]
go
create table AspEodMsUpdateLog(
  eventDate datetime      not null constraint DF_AspEodMsUpdateLog_eventDate default (getdate()),
  event     varchar(1000) collate SQL_Latin1_General_CP1_CI_AS
)
go

/*Broker*/
if exists (select * from dbo.sysindexes where name = N'I_BrokerName' and id = object_id(N'[dbo].[Broker]'))
drop index [dbo].[Broker].[I_BrokerName]
go
alter table Broker
	alter column brokerName nvarchar(64) not null
go
create index I_BrokerName on Broker(brokerName)
go

/*Contract*/
if exists (select * from dbo.sysindexes where name = N'Contract_CALENDAR' and id = object_id(N'[dbo].[Contract]'))
drop index [dbo].[Contract].[Contract_CALENDAR]
go
create index Contract_CALENDAR on Contract(contractID,contractTypeID,symbol,expCalendarID,actionID,undPriceProfileID,optPriceProfileID)
go

if exists (select * from dbo.sysindexes where name = N'Contract_34' and id = object_id(N'[dbo].[Contract]'))
drop index [dbo].[Contract].[Contract_34]
go
create index Contract_34 on Contract(contractID,contractTypeID,actionID,symbol,expCalendarID,undPriceProfileID,optPriceProfileID)
go

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

if exists (select * from dbo.sysindexes where name = N'ContractPrice_PriceClose' and id = object_id(N'[dbo].[ContractPrice]'))
drop index [dbo].[ContractPrice].[ContractPrice_PriceClose]
go
create index ContractPrice_PriceClose on ContractPrice(contractID,exchangeID,priceClose)
go

/*CustomDividend*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[CustomDividend]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[CustomDividend]
go
create table CustomDividend(
  ID      numeric(18,0) not null identity not for replication,
  StockID int,
  DivAmnt float,
  DivYtes datetime
)
go

if exists (select * from dbo.sysindexes where name = N'CustomDividend_IDX' and id = object_id(N'[dbo].[CustomDividend]'))
drop index [dbo].[CustomDividend].[CustomDividend_IDX]
go
create clustered index CustomDividend_IDX on CustomDividend(StockID,DivYtes,DivAmnt)
go

/*CustomStrikeSkewPoint*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_CustomStrikeSkewPoint_VolaManagementData]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
alter table [dbo].[CustomStrikeSkewPoint] drop constraint [FK_CustomStrikeSkewPoint_VolaManagementData]
go
alter table CustomStrikeSkewPoint add
  constraint FK_CustomStrikeSkewPoint_VolaManagementData foreign key(volaSurfaceDataID) references VolaSurfaceData(volaSurfaceDataID) on delete cascade
go

if exists (select * from dbo.sysindexes where name = N'CustomStrikeSkewPoint_9' and id = object_id(N'[dbo].[CustomStrikeSkewPoint]'))
drop index [dbo].[CustomStrikeSkewPoint].[CustomStrikeSkewPoint_9]
go
create index CustomStrikeSkewPoint_9 on CustomStrikeSkewPoint(volaSurfaceDataID,strike,customStrikeSkewPointID,type,volatility,expiryID,isBasePoint)
go

/*EodContractForDelete*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[EodContractForDelete]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[EodContractForDelete]
go
create table EodContractForDelete(
  contractID int
)
go

if exists (select * from dbo.sysindexes where name = N'IX_EodContractForDelete_contractID' and id = object_id(N'[dbo].[EodContractForDelete]'))
drop index [dbo].[EodContractForDelete].[IX_EodContractForDelete_contractID]
go
create index IX_EodContractForDelete_contractID on EodContractForDelete(contractID)
go

/*EodContractForMark*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[EodContractForMark]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[EodContractForMark]
go
create table EodContractForMark(
  contractID int
)
go

if exists (select * from dbo.sysindexes where name = N'IX_EodContractForMark_contractID' and id = object_id(N'[dbo].[EodContractForMark]'))
drop index [dbo].[EodContractForMark].[IX_EodContractForMark_contractID]
go
create index IX_EodContractForMark_contractID on EodContractForMark(contractID)
go

/*EodContractWithTrades*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[EodContractWithTrades]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[EodContractWithTrades]
go
create table EodContractWithTrades(
  contractID int
)
go

/*EodFuture*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[EodFuture]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[EodFuture]
go
create table EodFuture(
  eodFutureID  int,
  futureRootID int,
  symbol       varchar(20),
  contractName varchar(255),
  maturityID   int,
  priceClose   float
)
go

if exists (select * from dbo.sysindexes where name = N'IX_EodFuture_eodFutureID' and id = object_id(N'[dbo].[EodFuture]'))
drop index [dbo].[EodFuture].[IX_EodFuture_eodFutureID]
go
create index IX_EodFuture_eodFutureID on EodFuture(eodFutureID)
go

/*EodFutureOption*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[EodFutureOption]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[EodFutureOption]
go
create table EodFutureOption(
  eodFutureID  int,
  symbol       varchar(20),
  contractName varchar(255),
  expiryID     int,
  strike       float,
  isCall       tinyint,
  priceClose   float
)
go

if exists (select * from dbo.sysindexes where name = N'IX_EodFutureOption_eodFutureID' and id = object_id(N'[dbo].[EodFutureOption]'))
drop index [dbo].[EodFutureOption].[IX_EodFutureOption_eodFutureID]
go
create index IX_EodFutureOption_eodFutureID on EodFutureOption(eodFutureID)
go

/*EodFutureRootMap*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[EodFutureRootMap]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[EodFutureRootMap]
go
create table EodFutureRootMap(
  eodFutureRootID  int,
  futureRootID     int,
  eodFutureLotSize int,
  eodOptionLotSize int
)
go

if exists (select * from dbo.sysindexes where name = N'IX_EodFutureRootMap_contractID' and id = object_id(N'[dbo].[EodFutureRootMap]'))
drop index [dbo].[EodFutureRootMap].[IX_EodFutureRootMap_contractID]
go
create index IX_EodFutureRootMap_contractID on EodFutureRootMap(futureRootID)
go

if exists (select * from dbo.sysindexes where name = N'IX_EodFutureRootMap_eodContractID' and id = object_id(N'[dbo].[EodFutureRootMap]'))
drop index [dbo].[EodFutureRootMap].[IX_EodFutureRootMap_eodContractID]
go
create index IX_EodFutureRootMap_eodContractID on EodFutureRootMap(eodFutureRootID)
go

/*EodOption*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[EodOption]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[EodOption]
go
create table EodOption(
  eodOptionRootID int,
  symbol          varchar(20),
  contractName    varchar(255),
  expiryID        int,
  strike          float,
  isCall          tinyint,
  priceClose      float
)
go

if exists (select * from dbo.sysindexes where name = N'IX_EodOption_eodOptionRootID' and id = object_id(N'[dbo].[EodOption]'))
drop index [dbo].[EodOption].[IX_EodOption_eodOptionRootID]
go
create index IX_EodOption_eodOptionRootID on EodOption(eodOptionRootID)
go

/*EodOptionRoot*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[EodOptionRoot]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[EodOptionRoot]
go
create table EodOptionRoot(
  eodOptionRootID int,
  underlyingID    int,
  symbol          varchar(20),
  lotSize         int
)
go

/*EodOptionsForDelete*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[EodOptionsForDelete]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[EodOptionsForDelete]
go
create table EodOptionsForDelete(
  optionID int
)
go

if exists (select * from dbo.sysindexes where name = N'IX_EodOptionsForDelete_optionID' and id = object_id(N'[dbo].[EodOptionsForDelete]'))
drop index [dbo].[EodOptionsForDelete].[IX_EodOptionsForDelete_optionID]
go
create index IX_EodOptionsForDelete_optionID on EodOptionsForDelete(optionID)
go

/*EodOptionsForMark*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[EodOptionsForMark]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[EodOptionsForMark]
go
create table EodOptionsForMark(
  optionID int
)
go

if exists (select * from dbo.sysindexes where name = N'IX_EodOptionsForMark_optionID' and id = object_id(N'[dbo].[EodOptionsForMark]'))
drop index [dbo].[EodOptionsForMark].[IX_EodOptionsForMark_optionID]
go
create index IX_EodOptionsForMark_optionID on EodOptionsForMark(optionID)
go

/*EodUnderlyingMap*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[EodUnderlyingMap]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[EodUnderlyingMap]
go
create table EodUnderlyingMap(
  eodContractID int,
  contractID    int
)
go

if exists (select * from dbo.sysindexes where name = N'IX_EodUnderlyingMap_contractID' and id = object_id(N'[dbo].[EodUnderlyingMap]'))
drop index [dbo].[EodUnderlyingMap].[IX_EodUnderlyingMap_contractID]
go
create index IX_EodUnderlyingMap_contractID on EodUnderlyingMap(contractID)
go

if exists (select * from dbo.sysindexes where name = N'IX_EodUnderlyingMap_eodContractID' and id = object_id(N'[dbo].[EodUnderlyingMap]'))
drop index [dbo].[EodUnderlyingMap].[IX_EodUnderlyingMap_eodContractID]
go
create index IX_EodUnderlyingMap_eodContractID on EodUnderlyingMap(eodContractID)
go

/*ExpiryCalendar*/
if not  exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='ExpiryCalendar' and COLUMN_NAME='bBelongsToFutureRoot')
alter table ExpiryCalendar add
  bBelongsToFutureRoot tinyint
go

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
DROP TABLE [dbo].[FutureUnderlying]
go
CREATE TABLE [FutureUnderlying] (
	[iFutureUnderlyingID] [int] NOT NULL ,
	[iFutureUnderlyingTypeID] [int] NOT NULL ,
	[tiGreeksCalculationType] [tinyint] NOT NULL ,
	[tiActive] [tinyint] NOT NULL ,
	[actionID] [tinyint] NOT NULL CONSTRAINT [DF__FutureUnd__actio__6F8121F9] DEFAULT (0),
	CONSTRAINT [PK_FutureUnderlying] PRIMARY KEY  CLUSTERED 
	(
		[iFutureUnderlyingID]
	)  ON [PRIMARY] ,
	CONSTRAINT [FK_FutureUnderlying_Contract] FOREIGN KEY 
	(
		[iFutureUnderlyingID]
	) REFERENCES [Contract] (
		[contractID]
	)
) ON [PRIMARY]
go

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_FutureUnderlying_Contract]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
alter table [dbo].[FutureUnderlying] drop constraint [FK_FutureUnderlying_Contract]
go
alter table FutureUnderlying add
  constraint FK_FutureUnderlying_Contract foreign key(iFutureUnderlyingID) references Contract(contractID)
go

/*HedgeSymbolsBeta*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[HedgeSymbolsBeta]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[HedgeSymbolsBeta]
go
create table HedgeSymbolsBeta(
  HedgeSymbol nvarchar(20) not null,
  Symbol      nvarchar(20) not null,
  Beta        float        not null
)
go

/*IVUnderlyingPointsRawData*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IVUnderlyingPointsRawData]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[IVUnderlyingPointsRawData]
go
create table IVUnderlyingPointsRawData(
  iContractID  int,
  dtExpDate    datetime,
  iExpiryID    int,
  fStrike      float,
  fIV          float,
  dtActionDate datetime
)
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

/*SyntheticUnderlyingParam*/
if not  exists 
	(select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='SyntheticUnderlyingParam' and 
		COLUMN_NAME='strikeSettlement')
alter table SyntheticUnderlyingParam add
	strikeSettlement    float    not null constraint DF_strikeSettlement default (0)
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
if exists (select * from dbo.sysindexes where name = N'Trade_Strategy' and id = object_id(N'[dbo].[Trade]'))
drop index [dbo].[Trade].[Trade_Strategy]
go
create index Trade_Strategy on Trade(contractID,strategyID)
go

/*TradeHistory*/
if exists (select * from dbo.sysindexes where name = N'IX_TradeHistory_BrokerName' and id = object_id(N'[dbo].[TradeHistory]'))
drop index [dbo].[TradeHistory].[IX_TradeHistory_BrokerName]
go
alter table TradeHistory
	alter column brokerName nvarchar(64)
go
create index IX_TradeHistory_BrokerName on TradeHistory(brokerName)
go

/*TraderContract*/
if exists (select * from dbo.sysindexes where name = N'TraderContract_52' and id = object_id(N'[dbo].[TraderContract]'))
drop index [dbo].[TraderContract].[TraderContract_52]
go
create index TraderContract_52 on TraderContract(contractID,traderID,actionID)
go

if exists (select * from dbo.sysindexes where name = N'TraderContract31' and id = object_id(N'[dbo].[TraderContract]'))
drop index [dbo].[TraderContract].[TraderContract31]
go
create index TraderContract31 on TraderContract(traderID,contractID,actionID)
go

if exists (select * from dbo.sysindexes where name = N'TraderContract_ContractID' and id = object_id(N'[dbo].[TraderContract]'))
drop index [dbo].[TraderContract].[TraderContract_ContractID]
go
create index TraderContract_ContractID on TraderContract(traderID,contractID,actionID)
go

/*VolaSurfaceData*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_VolaSurfaceData_Contract]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
alter table [dbo].[VolaSurfaceData] drop constraint [FK_VolaSurfaceData_Contract]
go
alter table VolaSurfaceData add
  constraint FK_VolaSurfaceData_Contract foreign key(contractID) references Contract(contractID) on delete cascade

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.137')
	begin
		update DataInfo set keyValue = '4.49.137' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
--IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
--GO
--IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
--GO
--IF EXISTS (SELECT * FROM #tmpErrors)
--BEGIN
--	ROLLBACK TRANSACTION
	--SELECT * FROM #tmpErrors
--END
GO
--IF @@TRANCOUNT > 0 
--BEGIN
	select 'DB VERSION WAS CHANGED TO 4.49.137' '4.49.137 update message:' 
	select 'The database update succeeded'
--
IF @@TRANCOUNT > 0 
BEGIN
	COMMIT TRANSACTION
END
--	ELSE select 'The database update failed'
--GO
DROP TABLE #tmpErrors
GO
-- unable to execute this script in transaction