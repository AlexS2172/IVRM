/*
patch for last release of ETS
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
--if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.39.058' or (keyValue = '3.40.059')))
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue in('3.39.058','3.40.059') or (left(keyValue,4)='3.39' and right(keyValue,3) >= '058')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.40.059 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.40.059' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
--===============================================================================================================
-- changes in tables
--===============================================================================================================
-- add new column
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='status')
	begin
		Alter table Trade
			Add status tinyint null
	end
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--move information from IsSimulated to status field
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='isSimulated')
	begin
		exec('update Trade set status = IsSimulated')	
	end
-- drop constraint at the isSimulated column
declare @vcConstraintName varchar(4000)
	select @vcConstraintName = object_name(constid)
		from sysconstraints s1
			join syscolumns s2 on s1.colid = s2.colid and s1.id = s2.id
		where s2.id = object_id('Trade') and s2.name = 'isSimulated'
if @vcConstraintName is not null
	exec ('ALTER TABLE Trade DROP CONSTRAINT ' + @vcConstraintName)
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--drop isSimulated column
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='isSimulated') and 
   exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='status')
	begin
		Alter table Trade Drop column IsSimulated
	end
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--===============================================================================================================
-- creation of IndexBeta table
--===============================================================================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IndexBeta]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[IndexBeta]
GO
CREATE TABLE [dbo].[IndexBeta] (
	[contractBetaID] [int] IDENTITY (1, 1) NOT NULL ,
	[indexID] [int] NOT NULL ,
	[contractID] [int] NOT NULL ,
	[beta] [float] NOT NULL ,
	[actionID] [tinyint] NULL ,
	[actionDate] [datetime] NULL 
) ON [PRIMARY]
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
ALTER TABLE [dbo].[IndexBeta] WITH NOCHECK ADD 
	CONSTRAINT [PK_ContractBeta] PRIMARY KEY  CLUSTERED 
	(
		[contractBetaID]
	)  ON [PRIMARY] 
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
ALTER TABLE [dbo].[IndexBeta] WITH NOCHECK ADD 
	CONSTRAINT [DF_ContractBeta_actionID] DEFAULT (1) FOR [actionID],
	CONSTRAINT [DF_ContractBeta_actionDate] DEFAULT (getdate()) FOR [actionDate]
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
ALTER TABLE [dbo].[IndexBeta] ADD 
	CONSTRAINT [FK_ContractBeta_Contract1] FOREIGN KEY 
	(
		[contractID]
	) REFERENCES [dbo].[Contract] (
		[contractID]
	),
	CONSTRAINT [FK_IndexBeta_Index] FOREIGN KEY 
	(
		[indexID]
	) REFERENCES [dbo].[Index] (
		[indexID]
	)
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--===============================================================================================================
-- changes in triggers
--===============================================================================================================
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_IndexBeta]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[tU_IndexBeta]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_IndexDefinition]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[tU_IndexDefinition]
GO
CREATE  trigger tU_IndexBeta ON dbo.IndexBeta
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	-- 1st step - update actionDate if record not already deleted
	update IndexBeta
		set actionDate = GetDate()
	from IndexBeta I 
			join inserted on I.contractBetaID = inserted.contractBetaID
			join deleted on I.contractBetaID = deleted.contractBetaID
	where deleted.actionID < 3

	-- 2nd step - if update record -> set actionID=2
	update IndexBeta
		set actionID=2
	from IndexBeta I
			join inserted on I.contractBetaID = inserted.contractBetaID
			join deleted on I.contractBetaID = deleted.contractBetaID
	where deleted.actionID<3 and inserted.actionID = 1

	-- 3d step -disable restore of deleted record
	update IndexBeta
		set actionID = 3
	from IndexBeta I
			join inserted on I.contractBetaID = inserted.contractBetaID
			join deleted on I.contractBetaID = deleted.contractBetaID
	where deleted.actionID=3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  trigger tU_IndexDefinition ON dbo.[IndexDefinition]
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	-- 1st step - update actionDate if record not already deleted
	update [IndexDefinition]
		set actionDate = GetDate()
	from [IndexDefinition] I 
			join inserted on I.indexDefinitionID = inserted.indexDefinitionID
			join deleted on I.indexDefinitionID = deleted.indexDefinitionID
	where deleted.actionID < 3

	-- 2nd step - if update record -> set actionID=2
	update [IndexDefinition]
		set actionID=2
	from [IndexDefinition] I
			join inserted on I.indexDefinitionID = inserted.indexDefinitionID
			join deleted on I.indexDefinitionID = deleted.indexDefinitionID
	where deleted.actionID<3 and inserted.actionID = 1

	-- 3d step -disable restore of deleted record
	update [IndexDefinition] 
		set actionID = 3
	from [IndexDefinition] I
			join inserted on I.indexDefinitionID = inserted.indexDefinitionID
			join deleted on I.indexDefinitionID = deleted.indexDefinitionID
	where deleted.actionID=3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--===============================================================================================================
-- changes in View's
--===============================================================================================================
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[NetPositionsForExtApp_View]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[NetPositionsForExtApp_View]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IndexDefinitionView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[IndexDefinitionView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PositionsByOptForExtApp_View]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[PositionsByOptForExtApp_View]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TodayPositionsForExtApp_View]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TodayPositionsForExtApp_View]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TradesForExtApp_View]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TradesForExtApp_View]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TradeView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TradeView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IndexBetaView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[IndexBetaView]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


Create View dbo.IndexBetaView
as
	Select 
		contractBetaID, 
		indexID, 
		contractID, 
		beta, 
		actionID, 
		actionDate
	from IndexBeta
	where actionID < 3


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE view dbo.TradeView
AS
	Select 	tradeID,
		contractID, 
		isClose, 
		tradeDate, 
		quantity, 
		price, 
		isBuy, 
		bookID, 
		strategyID, 
		traderID, 
		brokerID, 
		clearingBrokerID, 
		brokerCommissionID, 
	       	clearingBrokerCommissionID, 
		tradedIV, 
		spotReference,
		isPosition,
		status,
		execID
	from dbo.Trade
	GROUP BY tradeID, contractID, isClose, 
		 tradeDate, quantity, price, 
		 isBuy, bookID, strategyID, 
		 traderID, brokerID, clearingBrokerID, 
		 brokerCommissionID,clearingBrokerCommissionID,
		tradedIV, spotReference,isPosition,status,execID
	HAVING  (MIN(actionID) = 1)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE   View dbo.IndexDefinitionView
as
	Select 
		indexID,
		stockID,
		weight,
		correlationPrice,
		correlationVola,
		actionID,
		actionDate
	from IndexDefinition
	where actionID < 3


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE   View dbo.PositionsByOptForExtApp_View
as

/*
	Created by Zaur Nuraliev
	Description: this view is getting information from Trade table special for A.D.

	Changed by Mike Malyshkin(08/01/2002)
	Changed by Zaur Nuraliev(08/08/2002)
*/

	select 
		convert(varchar,tradeDate,104) as [Trade Date],
		Trader.acronym as [Account Acronym],
		UC.Symbol as [Stock Symbol],
		left(C.symbol,len(C.symbol) - 2) + Datename(month, EC.expiryDate) + Cast(Round(O.strike, 2) as varchar) + Case O.IsCall when 1 then 'C' else 'P' end  as [Full Option Symbol],
		left(C.symbol,len(C.symbol) - 2) as [Option Root],
		Upper(Datename(month, EC.expiryDate)) as [Expiration Month],
		Cast(Round(O.strike,2) as int) as [Strike],
		Case O.isCall when 1 then 'C' else 'P' end [Option Type],
--		sum(TV.quantity * TV.lotSize * (case when TV.isBuy <> 0 then 1 else -1 end)) as [Position],
		sum(TV.quantity * ort.lotSize * (case when TV.isBuy <> 0 then 1 else -1 end)) as [Position],
		Cast(Round(avg(TV.price),2) as varchar) as [OptionMark],
		case when TV.spotReference is null then 0 else Cast(Round(TV.spotReference,2) as varchar) end [Underlier/Stock Mark],
		ort.lotSize as [Contract Multiplier],
		'OPTION' as [OPTION\STOCK],
		convert(varchar,EC.expiryDate,104) as [Expiration Date]
	from TradeView TV
		join Trader on TV.traderID = Trader.traderID 
		join [Option] O on TV.contractID = O.optionID
		join Contract C on C.contractID = O.optionID
		join OptionRoot ort on O.optionRootID = ort.optionRootID
		join Contract UC on ort.underlyingID = UC.contractID
		join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
--	where (TV.isSimulated <> 1 or isSimulated is null)
	where (isnull(TV.status,0) != 1)
		and EC.expiryDate > = GetDate()
	group by 	TV.contractID, TV.traderID,TV.tradeDate,Trader.acronym,
				UC.Symbol,C.symbol,EC.expiryDate,O.strike,O.IsCall,TV.price,
				TV.quantity,TV.spotReference,ort.lotSize,TV.contractID




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE  View dbo.TodayPositionsForExtApp_View
as
/*
	Created by Zaur Nuraliev
	Description: this view is getting information from Trade table special for A.D.
*/
select top 100 percent *
from
(
			select
				cast(convert(varchar(10),getdate(),112) as datetime) as [Trade Date],
				Trader.acronym as [Account Acronym],
				C.Symbol as [Stock Symbol],
				C.Symbol as [Full Option Symbol],
				null as [Option Root],
				null as [Expiration Month],
				null as [Strike],
				'S' as [Option Type],
				sum(TV.quantity * (case when TV.isBuy <> 0 then 1 else -1 end)) as [Position],
				null as [OptionMark],
				cast(CP.priceClose as decimal(15,2)) as [Underlier/Stock Mark],
				1 as [Contract Multiplier],
				'STOCK' as [OPTION\STOCK],
				null as [Expiration Date]
			from TradeView TV
				join Trader on TV.traderID = Trader.traderID 
				join Contract C on C.contractID = TV.contractID
				join ContractPrice CP on TV.contractID = CP.contractID and CP.exchangeID is null
			where isnull(TV.status,0) != 1 
					and C.contractTypeID = 2
					and isPosition = 0
			group by	C.symbol,
						Trader.acronym,	
						CP.priceClose
union

			select
				cast(convert(varchar(10),getdate(),112) as datetime) as [Trade Date],
				Trader.acronym as [Account Acronym],
				UC.Symbol as [Stock Symbol],
				left(C.symbol,len(C.symbol) - 2) + cast(Upper(Datename(month, EC.expiryDate)) as char(3)) + Cast(Cast(O.strike as int) as varchar)  + Case O.IsCall when 1 then 'C' else 'P' end  as [Full Option Symbol],
				left(C.symbol,len(C.symbol) - 2) as [Option Root],
				cast(Upper(Datename(month, EC.expiryDate)) as char(3)) as [Expiration Month],
				Cast(O.strike as decimal(15,2)) as [Strike],
				Case O.isCall when 1 then 'C' else 'P' end [Option Type],
				sum(TV.quantity * (case when TV.isBuy <> 0 then 1 else -1 end)) as [Position],
				cast (CP.priceClose as decimal(15,2)) as [OptionMark],
				cast(UCP.priceClose as decimal(15,2)) as [Underlier\Stock Mark],
    			1 as [Contract Multiplier],
				'OPTION' as [OPTION\STOCK],
				EC.expiryDate as [Expiration Date]
			from TradeView TV
				join Trader on TV.traderID = Trader.traderID 
				join [Option] O on TV.contractID = O.optionID
				join Contract C on C.contractID = O.optionID
				join OptionRoot ort on ort.optionRootID = o.optionRootID
				join Contract UC on ort.underlyingID = UC.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				join ContractPrice CP on TV.contractID = CP.contractID and CP.exchangeID is null
				join ContractPrice UCP on Ort.UnderlyingID = UCP.contractID and CP.exchangeID is null
			where isnull(TV.status,0) != 1
					and isPosition = 0
			group by
						C.symbol,
						Trader.acronym,
						UC.Symbol,
						EC.expiryDate,
						O.strike,
						O.IsCall,
						ort.lotSize,
						CP.priceClose,
						UCP.priceClose
) v
order by v.[Account Acronym], v.[Stock Symbol], 
v.[Expiration Date],v.[Strike], v.[Option Type]









GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE  view dbo.TradesForExtApp_View
as
/*
	Created by Zaur Nuraliev
	Description: this view is getting information special for A.D.

	Changed by Zaur Nuraliev(08/08/2002)
*/
select top 100 percent *
from
(
			select
				T.execID as [TradeID],
				cast(convert(varchar(10),getdate(),112) as datetime) as [Trade Date],
				null as [Settlement Date],
				Trader.acronym as [Account Acronym],
				C.Symbol as [Base Symbol],
				null as [Option Symbol],
				null as [Option Root],
				null as [Expiration Month],
				null as [Strike],
				--'S' as [Option Type],
				case when C.contractTypeID = 1 then 'I' else 'S' end as [Option Type],
				T.quantity * (case when T.isBuy <> 0 then 1 else -1 end) as [Net Trade], 
				cast(T.price as decimal(15,2)) as [Trade Price],
				1 as [Multiplier],
				null as [Trade Code], 
				null as [Exchange Code],
				null as [GRoss Proceeds],
				null as [Commission], 
				null as [Exchange Fee], 
				null as [SEC Fees], 
				null as [OCC Fees], 
				null as [Net Proceeds], 
				convert(varchar,T.tradeDate,108) as [Execution Time], 
				null as [Expiration Date], 
				C.ContractName as [Security Name],
				--isnull(T.isSimulated,0) as [isSimulated]
				case t.status when 1 then 1 else 0 end [isSimulated]
			from Trade T
				join Trader on T.traderID = Trader.traderID 
				join Contract C on C.contractID = T.contractID
			where --isnull(T.isSimulated,0) = 0 and 
					C.contractTypeID <3
					and isnull(isPosition,0) = 0 and T.actionID = 1
union
			select
				T.execID as [TradeID],
				cast( convert(varchar(10),getDate(),112) as datetime) as [Trade Date],
				null as [Settlement Date],
				Trader.acronym as [Account Acronym],
				UC.Symbol as [Base Symbol],
				left(C.symbol,len(C.symbol) - 2) + cast(Upper(Datename(month, EC.expiryDate)) as char(3)) + Cast(Cast(O.strike as int) as varchar)  + Case O.IsCall when 1 then 'C' else 'P' end  as [Option Symbol],
				left(C.symbol,len(C.symbol) - 2) as [Option Root],
				cast(Upper(Datename(month, EC.expiryDate)) as char(3)) as [Expiration Month],
				Cast(O.strike as decimal(15,2)) as [Strike],
				Case O.isCall when 1 then 'C' else 'P' end [Option Type],
				T.quantity * (case when T.isBuy <> 0 then 1 else -1 end) as [Net Trade], 
				cast(T.price as decimal(15,2)) as [Trade Price],
				ort.lotSize as [Multiplier],
				null as [Trade Code], 
				null as [Exchange Code],
				null as [GRoss Proceeds],
				null as [Commission], 
				null as [Exchange Fee], 
				null as [SEC Fees], 
				null as [OCC Fees], 
				null as [Net Proceeds], 
				convert(varchar,T.tradeDate,108) as [Execution Time], 
				EC.expiryDate as [Expiration Date], 
				UC.ContractName as [Security Name],
				--isnull(T.isSimulated,0) as [isSimulated]
				case t.status when 1 then 1 else 0 end [isSimulated]
			from Trade T
				join Trader on T.traderID = Trader.traderID 
				join [Option] O on T.contractID = O.optionID
				join Contract C on C.contractID = O.optionID
				join OptionRoot Ort on O.optionRootID = Ort.optionRootID
				join Contract UC on Ort.underlyingID = UC.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			where 	--isnull(T.isSimulated,0) = 0 and 
					isnull(isPosition,0) = 0 and T.actionID = 1
				and EC.expiryDate > = GetDate()	
) v
order by v.[Account Acronym], v.[Base Symbol], 
v.[Expiration Date],v.[Strike], v.[Option Type]




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE         View dbo.NetPositionsForExtApp_View
as
/*
	Created by Zaur Nuraliev
	Description: this view is getting information from Trade table special for Ary Dobner from Main Line
	Changed by Zaur Nuraliev(29/10/2002)
	changed by Mike Malyshkin (08/01/2002)
	Changed by Zaur Nuraliev(08/08/2002)
	Changed by Zaur Nuraliev(27/08/2002)
*/
select top 100 percent *
from
(
		select
				cast(convert(varchar(10),TV.TradeDate,112) as datetime) as [Trade Date],
				isnull(Trader.acronym,'DUMMY') as [Account Acronym],
				C.Symbol as [Stock Symbol],
				C.Symbol as [Full Option Symbol],
				null as [Option Root],
				null as [Expiration Month],
				null as [Strike],
				'S' as [Option Type],
				sum(TV.quantity * (case when TV.isBuy <> 0 then 1 else -1 end)) as [Position],
				null as [OptionMark],
				cast(CP.priceClose as decimal(15,2)) as [Underlier/Stock Mark],
				1 as [Contract Multiplier],
				'STOCK' as [OPTION\STOCK],
				null as [Expiration Date],
				case TV.status when 1 then 1 else 0 end [isSimulated]
			from ContractView C 
					left join TradeView TV on C.contractID = TV.contractID
					left join Trader on TV.traderID = Trader.traderID 
					left join ContractPrice CP on TV.contractID = CP.contractID and CP.exchangeID is null
			where	C.contractTypeID = 2 and 
					(C.contractID in (select distinct contractID from Trade))
			group by	C.symbol,
						Trader.acronym,	
						CP.priceClose,
						cast(convert(varchar(10),TV.TradeDate,112) as datetime),
						TV.status
union
		select
				cast(convert(varchar(10),TV.TradeDate,112) as datetime) as [Trade Date],
				isnull(Trader.acronym,'DUMMY') as [Account Acronym],
				C.Symbol as [Stock Symbol],
				C.Symbol as [Full Option Symbol],
				null as [Option Root],
				null as [Expiration Month],
				null as [Strike],
				'S' as [Option Type],
				sum(TV.quantity * (case when TV.isBuy <> 0 then 1 else -1 end)) as [Position],
				null as [OptionMark],
				cast(CP.priceClose as decimal(15,2)) as [Underlier/Stock Mark],
				1 as [Contract Multiplier],
				'STOCK' as [OPTION\STOCK],
				null as [Expiration Date],
				case TV.status when 1 then 1 else 0 end [isSimulated]
			from ContractView C 
					left join TradeView TV on C.contractID = TV.contractID
					left join Trader on TV.traderID = Trader.traderID 
					left join ContractPrice CP on TV.contractID = CP.contractID and CP.exchangeID is null
			where 	C.contractTypeID = 2 and 
					C.contractID in (select distinct underlyingID from OptionRoot O1 join OptionView O2 on O1.optionRootID = O2.optionRootID join Trade T on O2.optionID = T.contractID)
			group by	C.symbol,
						Trader.acronym,	
						CP.priceClose,
						cast(convert(varchar(10),TV.TradeDate,112) as datetime),
						TV.status
union
		select
			cast(convert(varchar(10),TV.TradeDate,112) as datetime) as [Trade Date],
			Trader.acronym as [Account Acronym],
			UC.Symbol as [Stock Symbol],
			left(C.symbol,len(C.symbol) - 2) + cast(Upper(Datename(month, EC.expiryDate)) as char(3)) + Cast(Cast(O.strike as int) as varchar)  + Case O.IsCall when 1 then 'C' else 'P' end  as [Full Option Symbol],
			left(C.symbol,len(C.symbol) - 2) as [Option Root],
			cast(Upper(Datename(month, EC.expiryDate)) as char(3)) as [Expiration Month],
			cast(O.strike as decimal(15,2)) as [Strike],
			case O.isCall when 1 then 'C' else 'P' end [Option Type],
			sum(TV.quantity * (case when TV.isBuy <> 0 then 1 else -1 end)) as [Position],
			cast (CP.priceClose as decimal(15,2)) as [OptionMark],
			cast(UCP.priceClose as decimal(15,2)) as [Underlier\Stock Mark],
			ORT.lotSize as [Contract Multiplier],
			'OPTION' as [OPTION\STOCK],
			EC.expiryDate as [Expiration Date],
			case TV.status when 1 then 1 else 0 end [isSimulated]
		from TradeView TV
			join Trader on TV.traderID = Trader.traderID 
			join [Option] O on TV.contractID = O.optionID
			join Contract C on C.contractID = O.optionID
			join OptionRoot ORT on O.optionRootID = ORT.optionRootID
			join Contract UC on ORT.underlyingID = UC.contractID
			join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			join ContractPrice CP on TV.contractID = CP.contractID and CP.exchangeID is null
			join ContractPrice UCP on ORT.underlyingID = UCP.contractID and CP.exchangeID is null
		where isnull(TV.status,0)!= 1 and EC.expiryDate >= GetDate()
		group by	C.symbol,
					Trader.acronym,
					UC.Symbol,
					EC.expiryDate,
					O.strike,
					O.IsCall,
					ORT.lotSize,
					CP.priceClose,
					UCP.priceClose,
					cast(convert(varchar(10),TV.TradeDate,112) as datetime),
					TV.status
) v
order by	v.[Stock Symbol], v.[Option Root],v.[Account Acronym],
			v.[Expiration Date],v.[Strike], v.[Option Type]




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--===============================================================================================================
-- changes in SP's
--===============================================================================================================
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractStockAndIndex_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractStockAndIndex_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinitionBeta_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinitionBeta_Clear]
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinitionHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinitionHH_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinition_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinition_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinition_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinition_Save]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Import]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Save]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContractForHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TraderContractForHH_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE  PROC dbo.usp_IndexDefinitionHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select indexID as iIndexID,
		stockID as iStockID,
		weight as fWeight,
	--	beta as fBeta,
		actionDate as dtActionDate,
		actionID as tiActionID
	from IndexDefinition
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE     Procedure dbo.usp_IndexDefinition_Get 
	@iIndexID int = Null, 
	@iStockID int = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from IndexDefinition table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select Idv.indexID as iIndexID,
			Idv.stockID as iStockID,
			Idv.weight as fWeight,
			null as fBeta,
			Idv.actionDate as dtActionDate,
			Idv.actionID as tiActionID
		from IndexDefinitionView idv

		where (@iIndexID is null or idv.indexID = @iIndexID) and (@iStockID is null or idv.stockID = @iStockID)

	set @error = @@error
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE        Procedure dbo.usp_IndexDefinition_Save 
	@iIndexID int = null, --the index identifier
	@iStockID int = null, --the stock identifier
	@fWeight float = null,  --the weight
	@fBeta float = null,
	@vcIndexSymbol varchar(8) = null,
	@vcStockSymbol varchar(8) = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save data into IndexDefinition table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iIndexID is null and @iStockID is null and @vcIndexSymbol is null and @vcStockSymbol is null 
		begin
			Raiserror ('Impossible define identifiers of the Stock and Index!', 16, 1)
			Return (-1)
		end   

	if @vcIndexSymbol is not null and @iIndexID is null
		select	@iIndexID = contractID from Contract where symbol = @vcIndexSymbol and contractTypeID = 1 and actionID < 3
		if @iIndexID is null 
			begin
				Raiserror ('Impossible define identifier of the Index!', 16, 1)
				Return (-1)
			end   

	if @vcStockSymbol is not null and @iStockID is null 
		select	@iStockID = contractID from Contract where symbol = @vcStockSymbol and contractTypeID = 2 and actionID < 3
		if @iStockID is null 
			begin
				Raiserror ('Impossible define identifier of the Stock!', 16, 1)
				Return (-1)
			end   

	Begin Transaction
		if not exists(select * From IndexDefinitionView where indexID = @iIndexID and stockID = @iStockID) 
			begin
				insert into IndexDefinition (indexID,stockID,weight) 
				values (@iIndexID,@iStockID,@fWeight)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update IndexDefinition
					set weight = isnull(@fWeight, weight)
					where indexID = @iIndexID and stockID = @iStockID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end


		if @fBeta is not null
			begin
				if exists(select * from IndexBetaView where indexID = @iIndexID and contractID = @iStockID)
					begin
						update IndexBeta set beta=isnull(@fBeta,beta) where indexID = @iIndexID and contractID = @iStockID
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
					end
				else
					begin
						insert into IndexBeta (indexID, contractID, beta) 
							values (@iIndexID, @iStockID, @fBeta)

						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
					end
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE               Procedure dbo.usp_Option_Import
	@iUnderlyingContractID int = null,  --underlying contractID
	@vcSymbol varchar(20),  -- option symbol
	@fPriceAsk float, 
	@fPriceBid float, 
	@iVolume int,
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
	@iOpenInterest int,
	@iExchangeID int = Null, -- the exchange identifier (special for ContractPrice update)
	@vcUnderlyingSymbol varchar(8) = null,  -- stock symbol
	@iLotSize int = null
as
/*
	Created by ... Redesigned by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare	@OptionID int, 
			@cn varchar(255),
			@iExpiryID int,
			@iLastExpiryID int

	--===================================================================			
	-- check the Expiry
	--===================================================================			
	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	if not exists(select * from Expiry where expiryID = @iExpiryID)
		begin
			Declare @ERR_MSG varchar(8000)
			set @ERR_MSG = 'Incorrect expiration! '  + '"' +  Convert(varchar(50),@dtExpiry) + '"' + ' for symbol=' + '"' + @vcSymbol + '"' +  ' UnderlyingSymbol=' + '"' + @vcUnderlyingSymbol + '"' + ' Strike=' + Convert(varchar(15),@fStrike) + ' expiryID=' + Convert(varchar(15),@iExpiryID)
			Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end

	select top 1 @iLastExpiryID = expiryID from ExpiryInCalendarView where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	--===================================================================			
	-- check the consistensy of input parameters (for stock)
	--===================================================================			
	declare @iUnderlyingFirstContractID int
		set @iUnderlyingFirstContractID = @iUnderlyingContractID
	--select underlying ID
	select @iUnderlyingContractID = contractID  
		from ContractView 
		where symbol = @vcUnderlyingSymbol and contractTypeID < 3

	if @iUnderlyingFirstContractID <> @iUnderlyingContractID
		begin
			Raiserror ('Wrong underlying contract ID in usp_Option_Import!', 16, 1)
			Return (-1)
		end  
	--select underlying option symbol
	if @vcUnderlyingSymbol is null
		select @vcUnderlyingSymbol=Contract.Symbol 
			from Contract (nolock) 	
			where contractID = @iUnderlyingContractID and Contract.contractTypeID < 3

	--===================================================================			
	-- select the @iOptionRootID
	--===================================================================			
	declare @iOptionRootID int,
			@vcOptionRoot varchar(20)
	select @iOptionRootID = optionRootID from OptionRootView where symbol = left(@vcSymbol,len(@vcSymbol)-2) 


	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if (@OptionID is Null and @iOptionRootID is not null) 
		select @OptionID = optionID
			from [Option] (nolock)
			where optionRootID = @iOptionRootID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	--===================================================================			
	-- delete Option with same ticker in DB if find
	--===================================================================			
	Create table #Options_for_delete(OptionID int) 
	
	insert into #Options_for_delete
		select contractID 
			from Contract (nolock)
				join [Option] (nolock) on Contract.contractID = [Option].optionID
			where symbol = @vcSymbol and contractTypeID = 3 and 
				[Option].actionID < 3 and ((@OptionID <> contractID) or @OptionID is null)
	begin
		exec usp_Option_Del

	end

	--===================================================================			
	-- make the contract name
	--===================================================================			
--  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(day, @dtExpiry) + ' ' + DATENAME(month, @dtExpiry) + 
--					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
--						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(month, @dtExpiry) + 
					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)

  if exists (select contractID from Contract(nolock) where contractID = @iUnderlyingContractID and contractTypeID > 2)
	 begin
	   RAISERROR ('Error. Can''t insert values to [Option]. The Underlying Contract is not stock or index!', 16, 1)
	   RETURN (-1)
	 end 



  if (@OptionID is Null) 
	begin
		Begin Transaction

			if  @iOptionRootID is null
			begin
				insert into OptionRoot (underlyingID,symbol,lotSize)
					select @iUnderlyingContractID,left(@vcSymbol,len(@vcSymbol)-2),isnull(@iLotSize,100)
				set @iOptionRootID = @@identity
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end 
	
			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@cn)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				 values (@OptionID, @iOptionRootID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
--			insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
--				 values (@OptionID, @iUnderlyingContractID, @iIsCall, @fStrike, @iExpiryID)

	
			insert into ContractPrice (contractID,exchangeID, priceBid, priceAsk, volume,openInterest)
				 values (@OptionID,@iExchangeID, @fPriceBid, @fPriceAsk, @iVolume,@iOpenInterest)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@OptionID,null)
					if (@@error <> 0) begin rollback tran return(-1) end           
				end 
		Commit Transaction
	end 
   else
	begin
		  update Contract
			   set symbol = @vcSymbol,
					 contractName = @cn
			where contractID = @OptionID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract!', 16, 1)
				  RETURN (-1)
			   end 

		delete from TradeLog
		where contractID = @OptionID

		if @iOptionRootID is not null
		begin
		  update OptionRoot
				set
					lotSize = isnull(@iLotSize,lotSize)
				where optionRootID = @iOptionRootID
		end

		  update ContractPrice
				set	priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					volume = @iVolume,
					openInterest=@iOpenInterest
				where contractID = @OptionID and isnull(exchangeID,0)=isnull(@iExchangeID,0)
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract price!', 16, 1)
				  RETURN (-1)
			   end 

			update [Option]
				set 
				IsCall = isnull(@iIsCall,IsCall),
				strike = isnull(@fStrike,Strike),
				expiryID = isnull(@iExpiryID,expiryID)
			where optionID = @OptionID

		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t update Option information (table Option)!', 16, 1)
				  RETURN (-1)
			   end 

	end 
	 
-- return @OptionID 
    set @error = 0
finish:
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE           proc dbo.usp_Option_Save
		@iOptionID int,
		@vcSymbol varchar(20) = Null,
		@iExchangeID int, 
		@iUnderlyingContractID int,
		@iIsCall tinyint = Null,
		@fStrike float = Null,
		@dtExpiry datetime = Null,
		@tiIsManualyEntered tinyint
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

   if exists (select contractID from ContractView where contractID = @iUnderlyingContractID and contractTypeID > 2)
	   begin
		 RAISERROR ('Error. Can''t insert or update Option values. The Underlying Contract is not stock or index!', 16, 1)
		 RETURN (-1)
	   end	 
   if len(@vcSymbol) < 3  and @iOptionID is null
	   begin
		 RAISERROR ('Error. Option symbol can''t be less three symbols!', 16, 1)
		 RETURN (-1)
	   end	 
   if exists (select * from ContractView where symbol = @vcSymbol and ContractTypeID = 3 and (@iOptionID <> contractID or @iOptionID is null) and not @vcSymbol is null)
	   begin
		 Raiserror ('Error. Option with this ticker already exists!', 16, 1)
		 Return (-1)
	   end

   if exists (select optionRootID 
				   from OptionRootView
				   where symbol = left(@vcSymbol,len(@vcSymbol)-2)and 
					underlyingID<>@iUnderlyingContractID)
		begin
		  RAISERROR ('Error. Can''t insert or update Option. Current Root assigned to different Underlying', 16, 1)
		  RETURN (-1)
		end  

	declare	@OptionID int, 
			@ContractName varchar(255),
			@iExpiryID int

	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	declare @iOptionRootID int,
			@vcOptionRoot varchar(20)
	select @iOptionRootID = optionRootID from OptionRootView where symbol = left(@vcSymbol,len(@vcSymbol)-2)  and 
		underlyingID=@iUnderlyingContractID	

   if @iOptionRootID is not null and exists (select optionID 
				   from OptionView
				   where optionRootID = @iOptionRootID and 
					isCall = @iIsCall and 
					strike = @fStrike and 
					expiryID = @iExpiryID and
					(@iOptionID is Null or optionID <> @iOptionID))
		begin
		  RAISERROR ('Error. Can''t insert or update Option values. There are ambiguous Option in DB!', 16, 1)
		  RETURN (-1)
		end  

   set @ContractName = @vcSymbol + ' ' + datename(day, @dtExpiry) + ' ' + datename(month, @dtExpiry) + 
					' ' + datename(year, @dtExpiry) + ' ' + CASE @iIsCall when 1 then 'Call' else 'Put' end +
						 ' ' + cast(round(@fStrike, 2) as varchar)


	Begin Transaction

		if  @iOptionRootID is null
			begin
				insert into OptionRoot (underlyingID,symbol,lotSize)
					select @iUnderlyingContractID,left(@vcSymbol,len(@vcSymbol)-2),100
				set @iOptionRootID = @@identity
			end 

	   if (@iOptionID is Null)	
		 begin
			insert into Contract (contractTypeID, symbol, contractName)
				values (3, @vcSymbol, @ContractName)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iOptionID = @@identity			  
			
			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				values (@iOptionID, @iOptionRootID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		 end
	   else
		 begin
			update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@ContractName,contractName)
				where contractID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			update [Option]
				set optionRootID = @iOptionRootID,
					isCall = isNull(@iIsCall, isCall),
					strike = isNull(@fStrike, strike),
					expiryID = isNull(@iExpiryID, expiryID)
				where optionID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		 end		

		if @tiIsManualyEntered in (0,1) 
			begin
--select @iOptionID as '@iOptionID*******************'
				update [Option] set isManualyEntered = @tiIsManualyEntered where optionID = @iOptionID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end 

	Commit Transaction

    set @error = 0
finish:
if (@error <> 0) return (@error) else return @iOptionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE Procedure dbo.usp_TraderContractForHH_Get 
	@iTraderID int = null, -- trader ID
	@vcAcronym varchar(12) = null, -- trader acronym
	@tiInfoType tinyint = 0, -- information required: 
								-- 0 - available underlyiings list,
								-- 1 - market structure updates,
								-- 2 - contracts for price feed,
								-- 3 - index definition,
								-- 4 - contract prices
	@iContractTypeID int = Null, -- contract type ID for @tiInfoType = 1, if Null or 0 then retrieve general contract updates
	@dtMinUpdateDate datetime = Null, -- minimal update date for @tiInfoType = 1, if Null then retrieve full actual info (w/o "deleted" records)
	@iContractID int = Null -- contract ID for @tiInfoType = 1
as
/*
	Created by Sharky
	Description: this procedure is for getting information for HendHeld initialize
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	if @iContractID = 0 set @iContractID = Null
	-- check tarder parameters
	if isnull(@iTraderID, 0) = 0
	begin
		if not @vcAcronym is null
			select top 1 @iTraderID = traderID
			from TraderView
			where acronym = @vcAcronym
		if isnull(@iTraderID, 0) = 0
		begin
			raiserror('Error. Invalid trader ID or acronym were specified.', 16, 1)
			return -1
		end
	end
	else if not exists(select 1 from Trader where traderID = @iTraderID)
	begin
		raiserror('Error. Invalid trader ID or acronym were specified.', 16, 1)
		return -1
	end
	set @tiInfoType = isnull(@tiInfoType, 0)
	if @tiInfoType = 0 -- available underlyings list
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			ce.exchangeID as iExchangeID,
			isUnderlying as iType1,
			isOption as iType2
		from TraderContractView tc
			join ContractView c on tc.contractID = c.contractID
			join ContractExchangeLinkView ce on c.contractID = ce.contractID
		where
			tc.traderID = @iTraderID
		union
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			null as iExchangeID,
			1 as iType1,
			1 as iType2
		from TraderContractView tc
			join ContractView c on tc.contractID = c.contractID
		where
			tc.traderID = @iTraderID
	else if @tiInfoType = 1 -- market structure updates
	begin
		if @dtMinUpdateDate is not null -- updates info
		begin
			if isnull(@iContractTypeID, 0) = 0 -- general contract info
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					null as fPriceTheoClose,
	
					c.actionID as tiActionID,
					c.actionDate as dtActionDate
	
				from TraderContractView tc
					join Contract c on tc.contractID = c.contractID
					join ContractPrice cph on c.contractID = cph.contractID and exchangeID is null
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or c.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or c.contractID = @iContractID)
				union
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					cph.priceTheo as fPriceTheoClose,
	
					c.actionID as tiActionID,
					c.actionDate as dtActionDate

				from TraderContractView tc
						join OptionRoot ORt on ORt.underlyingID = tc.contractID
						join [Option] o on ORt.optionRootID = o.optionRootID
						join Contract c on o.optionID = c.contractID
					--join [Option] o on tc.contractID = o.underlyingContractID
					--join Contract c on o.optionID = c.contractID
						join ContractPrice cph on c.contractID = cph.contractID and exchangeID is null
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or c.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or ort.underlyingID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 3 -- options
				select
					o.optionID as iOptionID,
					ort.underlyingID as iUnderlyingContractID,
					o.isCall as bIsCall,
					o.strike as fStrike,
					E.expiryDate as dtExpiry,
					o.actionID as tiActionID,
					o.actionDate as dtActionDate
					
				from TraderContractView tc
					join OptionRoot ORt on ORt.underlyingID = tc.contractID
					join [Option] o on ORt.optionRootID = o.optionRootID
					join Contract c on o.optionID = c.contractID
					join ExpiryInCalendarView E on E.expCalendarID = C.expCalendarID
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or o.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or ort.underlyingID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 2 -- stocks
				select
					s.stockID as iStockID,
	
					s.divFreq as iDivFreq,
					s.divDate as dtDivDate,
					s.divAmt as fDivAmt,
					
					null as iBaseIndexID,
					s.primaryExchangeID as iPrimaryExchangeID,
					s.isHTB as bIsHTB,
	
					s.actionID as tiActionID,
					s.actionDate as dtActionDate,
					s.calcOptionType as tiCalcOptionType,
					s.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join Stock s on tc.contractID = s.stockID
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or s.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or s.stockID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 1 -- indices
				select
					i.indexID as iIndexID,
	
					i.yield as fYield,
					i.isHTB as bIsHTB,
	
					i.actionID as tiActionID,
					i.actionDate as dtActionDate,

					i.calcOptionType as tiCalcOptionType,
					i.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join [Index] i on tc.contractID = i.indexID
				where
					tc.traderID = @iTraderID
					and (@dtMinUpdateDate is null or i.actionDate > @dtMinUpdateDate)
					and (@iContractID is null or i.indexID = @iContractID)
				order by dtActionDate
			else
			begin
				raiserror('Error. Invalid contract type ID was specified.', 16, 1)
				return -1
			end
		end
		else
		begin
			if isnull(@iContractTypeID, 0) = 0 -- general contract info
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					null as fPriceTheoClose,
	
					c.actionID as tiActionID,
					c.actionDate as dtActionDate
	
				from TraderContractView tc
					join ContractView c on tc.contractID = c.contractID
					join ContractPrice cph on c.contractID = cph.contractID and exchangeID is null
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or c.contractID = @iContractID)
				union
				select
					c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName,
					cph.priceClose as fPriceClose,
					cph.priceTheo as fPriceTheoClose,
					c.actionID as tiActionID,
					c.actionDate as dtActionDate

				from TraderContractView tc
					join OptionRootView ORt on ORt.underlyingID = tc.contractID
					join OptionView o on ORt.optionRootID = o.optionRootID
					join ContractView c on o.optionID = c.contractID
					left join ContractPrice cph on c.contractID = cph.contractID
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or ort.underlyingID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 3 -- options
				select
					o.optionID as iOptionID,
					ort.underlyingID as iUnderlyingContractID,
					o.isCall as bIsCall,
					o.strike as fStrike,
					E.expiryDate as dtExpiry,

					o.actionID as tiActionID,
					o.actionDate as dtActionDate
				from TraderContractView tc
					join OptionRootView ORt on ORt.underlyingID = tc.contractID
					join OptionView o on ORt.optionRootID = o.optionRootID
					join ContractView c on o.optionID = c.contractID
					join ExpiryInCalendarView E on E.expCalendarID = C.expCalendarID
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or ort.underlyingID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 2 -- stocks
				select
					s.stockID as iStockID,
	
					s.divFreq as iDivFreq,
					s.divDate as dtDivDate,
					s.divAmt as fDivAmt,
					
					null as iBaseIndexID,
					s.primaryExchangeID as iPrimaryExchangeID,
					s.isHTB as bIsHTB,
	
					s.actionID as tiActionID,
					s.actionDate as dtActionDate,

					s.calcOptionType as tiCalcOptionType,
					s.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join StockView s on tc.contractID = s.stockID
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or s.stockID = @iContractID)
				order by dtActionDate
	
			else if @iContractTypeID = 1 -- indices
				select
					i.indexID as iIndexID,
	
					i.yield as fYield,
					i.isHTB as bIsHTB,
	
					i.actionID as tiActionID,
					i.actionDate as dtActionDate,

					i.calcOptionType as tiCalcOptionType,
					i.calcModelType as tiCalcModelType
	
				from TraderContractView tc
					join IndexView i on tc.contractID = i.indexID
				where
					tc.traderID = @iTraderID
					and (@iContractID is null or i.indexID = @iContractID)
				order by dtActionDate
			else
			begin
				raiserror('Error. Invalid contract type ID was specified.', 16, 1)
				return -1
			end
		end
	end
	else if @tiInfoType = 2 -- contracts for price feed
	begin
		declare @dtToday datetime
		set @dtToday = cast(floor(cast(getdate() as float)) as datetime)

		select distinct 
			C.ContractID as iContractID,
			C.ContractTypeID as iContractTypeID,
			C.Symbol as vcSymbol, 
			E.ExchangeCode as vcExchangeCode
		from TraderContractView tc	
			join ContractView c on tc.contractID = c.contractID
			join ContractExchangeLinkView CE on C.contractID = CE.contractID
			join ExchangeView E on CE.exchangeID = E.exchangeID
		where
			tc.traderID = @iTraderID and ce.isUnderlying = 1
		union
		select distinct 
			c.ContractID as iContractID,
			c.ContractTypeID as iContractTypeID,
			c.Symbol as vcSymbol, 
			null as vcExchangeCode
		from TraderContractView tc 
			join ContractView c on tc.contractID = c.contractID
		where 
			tc.traderID = @iTraderID
		union
		select distinct 
			c.ContractID as iContractID,
			c.ContractTypeID as iContractTypeID,
			c.Symbol as vcSymbol, 
			e.ExchangeCode as vcExchangeCode
		from TraderContractView tc	
			join OptionRootView ORt on ORt.underlyingID = tc.contractID
			join OptionView o on ORt.optionRootID = o.optionRootID
			join ContractView c on o.optionID = c.contractID
			join Contract C1 on ORt.underlyingID = C1.contractID
			join ContractExchangeLinkView CE on ORt.underlyingID = CE.contractID
			join ExchangeView e on ce.exchangeID = e.exchangeID
			join ExpiryInCalendarView E1 on E1.expCalendarID = C1.expCalendarID
		where
			tc.traderID = @iTraderID
			and ce.IsOption = 1
			and E1.expiryDate > @dtToday
		union
		select distinct 
			c.ContractID as iContractID,
			c.ContractTypeID as iContractTypeID,
			c.Symbol as vcSymbol, 

			null as vcExchangeCode
		from TraderContractView tc	
			join OptionRootView ORt on ORt.underlyingID = tc.contractID
			join OptionView o on ORt.optionRootID = o.optionRootID
			join ContractView c on o.optionID = c.contractID
			join Contract C1 on ORt.underlyingID = C1.contractID
			join ExpiryInCalendarView E1 on E1.expCalendarID = C1.expCalendarID
		where
			tc.traderID = @iTraderID
			and E1.expiryDate > @dtToday
	end
	else if @tiInfoType = 3 -- index definition
	begin
		if @dtMinUpdateDate is not null -- updates info
			select
				i.indexID as iIndexID,
				i.stockID as iStockID,
				i.weight as fWeight,
				ibv.beta as fBeta,
	
				i.actionID as tiActionID,
				i.actionDate as dtActionDate

			from
				IndexDefinition i
					left join IndexBetaView ibv on i.indexID = ibv.indexID and i.stockID = ibv.contractID
			where
				i.indexID in (select contractID from TraderContractView where traderID = @iTraderID)
				and i.stockID in (select contractID from TraderContractView where traderID = @iTraderID)
				and (@dtMinUpdateDate is null or i.actionDate > @dtMinUpdateDate)
				order by dtActionDate
		else
			select
				i.indexID as iIndexID,
				i.stockID as iStockID,
				i.weight as fWeight,
				ibv.beta as fBeta,
	
				i.actionID as tiActionID,
				i.actionDate as dtActionDate
			from
				IndexDefinitionView i
					left join IndexBetaView ibv on i.indexID = ibv.indexID and i.stockID = ibv.contractID
			where
				i.indexID in (select contractID from TraderContractView where traderID = @iTraderID)
				and i.stockID in (select contractID from TraderContractView where traderID = @iTraderID)
				order by dtActionDate
	end
	else if @tiInfoType = 4 -- contract prices
	begin
		select
			c.contractID as iContractID,
			cph.priceClose as fPriceClose,
			null as fPriceTheoClose
		from TraderContractView tc
			join Contract c on tc.contractID = c.contractID
			left join ContractPrice cph on c.contractID = cph.contractID 
		where
			tc.traderID = @iTraderID
		union
		select
			c.contractID as iContractID,
			cph.priceClose as fPriceClose,
			cph.priceTheo as fPriceTheoClose
		from TraderContractView tc
			join OptionRootView ORt on ORt.underlyingID = tc.contractID
			join OptionView o on ORt.optionRootID = o.optionRootID
			join ContractView c on o.optionID = c.contractID
			left join ContractPrice cph on c.contractID = cph.contractID
		where
			tc.traderID = @iTraderID
	end
	else
	begin
		raiserror('Error. Invalid information type was specified.', 16, 1)
		return -1
	end
	return @@error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexUnderlyingBeta_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexUnderlyingBeta_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_PositionForReconcile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_PositionForReconcile_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSim_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeSim_Clear]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Clear]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Match]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Match]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE         Procedure dbo.usp_IndexUnderlyingBeta_Save 
	@iIndexID int = null, --the index identifier
	@iUnderlyingID int = null, --the underlying identifier
	@fBeta float,
	@vcIndexSymbol varchar(8) = null,
	@vcUnderlyingSymbol varchar(8) = null
as
/*
	Created by Mike Malyshkin
	Description: this procedure save data into IndexBeta table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iIndexID is null and @iUnderlyingID is null and @vcIndexSymbol is null and @vcUnderlyingSymbol is null 
		begin
			Raiserror ('Impossible define identifiers of the Underlying and Index!', 16, 1)
			Return (-1)
		end   

	if @vcIndexSymbol is not null and @iIndexID is null
		select	@iIndexID = contractID from Contract where symbol = @vcIndexSymbol and contractTypeID = 1 and actionID < 3
		if @iIndexID is null 
			begin
				Raiserror ('Impossible define identifier of the Index!', 16, 1)
				Return (-1)
			end   

	if @vcUnderlyingSymbol is not null and @iUnderlyingID is null 
		select	@iUnderlyingID = contractID from Contract where symbol = @vcUnderlyingSymbol and contractTypeID in (1,2) and actionID < 3
		if @iUnderlyingID is null 
			begin
				Raiserror ('Impossible define identifier of the Underlying!', 16, 1)
				Return (-1)
			end   

	Begin Transaction
		if not exists(select * From IndexBetaView where indexID = @iIndexID and contractID = @iUnderlyingID) 
			begin
				insert into IndexBeta (indexID,contractID,beta) 
				values (@iIndexID,@iUnderlyingID,@fBeta)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update IndexBeta
					set beta = isnull(@fBeta, beta)
					where indexID = @iIndexID and contractID = @iUnderlyingID
	
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE   PROCEDURE dbo.usp_PositionForReconcile_Get
	@tiByTrader tinyint = 0,
	@lastUTCTradeDate datetime = null
AS
	if isnull(@tiByTrader, 0) = 0
	begin
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as vcOptionRootID,
			null as vcOptionRootSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			1 as iLotSize,
			cp.priceClose as fPriceClose,

			null as iTraderID,
			null as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty
		
		from TradeView tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where c.contractTypeID < 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by c.contractID, c.contractTypeID, c.Symbol, cp.priceClose
		
		union
	
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			ORt.optionRootID as vcOptionRootID,
			ORt.symbol as vcOptionRootSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,

			ORt.lotSize as iLotSize,
			cp.priceClose as fPriceClose,

			null as iTraderID,
			null as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty

		from TradeView tr 
			inner join ContractView C on tr.contractID = C.contractID
			inner join OptionView o on c.contractID = o.optionID
			inner join OptionRootView ORt on o.optionRootID = ORt.optionRootID
			inner join ContractView UC on ORt.underlyingID = UC.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
	
		where c.contractTypeID = 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by C.contractID, C.contractTypeID, C.Symbol,ORt.optionRootID,ORt.symbol,
			EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
			ORt.lotSize, cp.priceClose
	
		order by vcSymbol
	end
	else
	begin
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as vcOptionRootID,
			null as vcOptionRootSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			1 as iLotSize,
			cp.priceClose as fPriceClose,
			trdr.TraderID as iTraderID,
			trdr.acronym as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty
		from TradeView tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join TraderView trdr on tr.traderID = trdr.traderID
		where c.contractTypeID < 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by c.contractID, c.contractTypeID, c.Symbol,cp.priceClose, trdr.TraderID, trdr.acronym
		
		union
	
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			ORt.optionRootID as vcOptionRootID,
			ORt.symbol as vcOptionRootSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			ORt.lotSize as iLotSize,
			cp.priceClose as fPriceClose,
			trdr.TraderID as iTraderID,
			trdr.acronym as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty

		from TradeView tr 
			inner join ContractView C on tr.contractID = C.contractID
			inner join OptionView o on c.contractID = o.optionID
			inner join OptionRootView ORt on o.optionRootID = ORt.optionRootID
			inner join ContractView UC on ORt.underlyingID = UC.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			left join TraderView trdr on tr.traderID = trdr.traderID
	
		where c.contractTypeID = 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by C.contractID, C.contractTypeID, C.Symbol,ORt.optionRootID,ORt.symbol,
			EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
			ORt.lotSize, cp.priceClose, trdr.TraderID, trdr.acronym
	
		order by vcSymbol
	end

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE PROCEDURE dbo.usp_TradeSim_Clear
	@tiSimulated tinyint = null,
	@tiManual tinyint = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction
		if isnull(@tiSimulated, 0) <> 0
		begin
			delete from TradeLog 
			where seqNum in (select seqNum from Trade where isnull(status, 0) = 1)
				if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
	
			delete from Trade 
			where tradeID in (select tradeID from Trade where isnull(status, 0) = 1) 
				if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
		end
	
		if isnull(@tiManual, 0) <> 0
		begin
			delete from TradeLog 
			where seqNum in (select seqNum from Trade where isnull(status, 0) = 2)
				if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
	
			delete from Trade 
			where tradeID in (select tradeID from Trade where isnull(status, 0) = 2) 
				if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
		end
	
		set @error = @@error
	Commit Transaction

finish:
	return @error




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE    PROCEDURE dbo.usp_Trade_Clear
	@iContractID int = null,
	@tiOnlyWithExecID tinyint = null,
	@lastUTCTradeDate datetime = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction

		delete from TradeLog 
		where seqNum in (select seqNum from Trade where isnull(status, 0) in (1, 2) and actionID = 0
									 and (@iContractID is null or contractID = @iContractID))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

		delete from Trade 
		where tradeID in (select tradeID from Trade where isnull(status, 0) in (1, 2) and actionID = 0) 
							and (@iContractID is null or contractID = @iContractID)
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
	
		if isnull(@tiOnlyWithExecID, 0) = 0
		begin
			delete from TradeLog 
				where seqNum in (select seqNum from Trade
									where (@iContractID is null or contractID = @iContractID) 
										and isnull(status, 0) in (0, 3)
										and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end	

			delete from Trade 
				where (@iContractID is null or contractID = @iContractID) 
					and isnull(status, 0) in (0, 3)
					and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0)
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
		end
		else
		begin
			delete from TradeLog 
				where seqNum in (select seqNum from Trade
									where (@iContractID is null or contractID = @iContractID) 
										and isnull(status, 0) in (0, 3)
										and isnull(execID, '') <> ''
										and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

			delete from Trade 
			where (@iContractID is null or contractID = @iContractID) 
				and isnull(status, 0) in (0, 3)
				and isnull(execID, '') <> ''
				and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
		end
		set @error = @@error
	Commit Transaction

finish:
	return @error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE  procedure dbo.usp_Trade_Match
	@iOldManualTradeID int,
	@iOldActualTradeID int,
	@iOldManualSeqNum int out,
	@iOldActualSeqNum int out,
	@iNewActualSeqNum int out,
	@iNewActualTradeID int out,
	@dtNewActualTradeDate datetime out
  as
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
		
			if @@error = 0 set @bSucceed = 1
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

			if @@error = 0
				set @bSucceed = 1
			else
				set @bSucceed = 0
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
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractGroup_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractGroup_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexWithPos_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexWithPos_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_PositionForReconcile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_PositionForReconcile_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_PositionsForExternalApp_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_PositionsForExternalApp_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeByTrader_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE  Procedure dbo.usp_ContractGroup_Save
	@iGroupID int out,
	@vcGroupName varchar(30),
	@vcDescription varchar(120),
	@tiRaiseError tinyint = 1
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from ContractGroup where groupName = @vcGroupName and actionID < 3 and (@iGroupID <> groupID or @iGroupID is null))
		begin
			if isnull(@tiRaiseError, 0) <> 0
				Raiserror ('Contract group with this name already exists!', 16, 1)
			Return (-1)
		end   
	if @iGroupID is Null 
		begin
			insert into ContractGroup (
					groupName,
					[Description]) 
				values (
					@vcGroupName,
					@vcDescription)
			select @iGroupID = @@identity
		end
	else
		begin
			update ContractGroup set
				groupName = isNull(@vcGroupName, groupName),
				[Description] = isNull(@vcDescription, [Description])
			where @iGroupID = groupID
		end
	set @error = @@error
	if (@error = 0) set @error = @iGroupID
Return(@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE     proc dbo.usp_IndexWithPos_Get
	@iIndexID int = null,
	@iTraderID int = null,
	@tiPositionMode tinyint = 0 -- 0 - filter by TraderContract, 1 - don't filter by TraderContract
as
	------------------------------------------------
	set nocount on
	------------------------------------------------

	if @iTraderID is null
		select C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName,
			I.yield as fYield,
			I.isHTB as bIsHTB,
			I.actionDate as dtActionDate,
			I.actionID as tiActionID,
			--sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
			--CP.lotSize as iLotSize,
			1 as iLotSize,
			I.CalcOptionType as tiCalcOptionType,
			I.CalcModelType as tiCalcModelType,
			C.expCalendarID as iExpCalendarID,
			I.isBasket as tiIsBasket,
			I.isActive as tiIsActive,
			i.skew as fSkew,
			i.kurt as fKurt,
			--sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
		from ContractView c 	
			inner join IndexView I on I.indexID = C.contractID
			join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
			left join TradeView tr on tr.contractID = c.contractID and tr.status = 0
		where C.contractTypeID = 1
			and (C.contractID = @iIndexID or @iIndexID is Null)
		group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
			I.yield, I.isHTB, I.actionDate, I.actionID,I.CalcOptionType,I.CalcModelType,C.expCalendarID, 
			I.isBasket, I.isActive, i.skew, i.kurt
		order by C.symbol
	else
	begin
		if isnull(@tiPositionMode, 0) = 0
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName,
				I.yield as fYield,
				I.isHTB as bIsHTB,
				I.actionDate as dtActionDate,
				I.actionID as tiActionID,
				--sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				--CP.lotSize as iLotSize,
				1 as iLotSize,
				I.CalcOptionType as tiCalcOptionType,
				I.CalcModelType as tiCalcModelType,
				C.expCalendarID as iExpCalendarID,
				I.isBasket as tiIsBasket,
				I.isActive as tiIsActive,
				i.skew as fSkew,
				i.kurt as fKurt,
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractView c 	
				inner join IndexView I on I.indexID = C.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and tr.status = 0
			where C.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (C.contractID = @iIndexID or @iIndexID is Null)
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
				I.yield, I.isHTB, I.actionDate, I.actionID, I.CalcOptionType,I.CalcModelType,C.expCalendarID, 
				I.isBasket, I.isActive, i.skew, i.kurt
			order by C.symbol
		else
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName,
				I.yield as fYield,
				I.isHTB as bIsHTB,
				I.actionDate as dtActionDate,
				I.actionID as tiActionID,
				--sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				1 as iLotSize,
				I.CalcOptionType as tiCalcOptionType,
				I.CalcModelType as tiCalcModelType,
				C.expCalendarID as iExpCalendarID,
				I.isBasket as tiIsBasket,
				I.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,
				--sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractView c 	
				inner join IndexView I on I.indexID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and tr.status = 0
			where C.contractTypeID = 1
				and (C.contractID = @iIndexID or @iIndexID is Null)
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
				I.yield, I.isHTB, I.actionDate, I.actionID,I.CalcOptionType,I.CalcModelType,C.expCalendarID, 
				I.isBasket, I.isActive, i.skew, i.kurt
			order by C.symbol
	end

Return (@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE     proc dbo.usp_Index_Get
	@iGroupID int = Null,
	@iIndexID int = Null,
	@tiWithPosition tinyint = 0,
	@iTraderID int = null
  as
	if isnull(@tiWithPosition, 0) = 0

	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt
				from ContractView C 	
					inner join IndexView I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt
				from ContractView C 	
					inner join IndexView I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where C.contractTypeID=1
					and tc.traderID = @iTraderID
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
				order by C.symbol
		end
		else
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join IndexView I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join IndexView I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and tc.traderID = @iTraderID
				order by C.symbol
		end
	end
	else
	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt
				from ContractView c 	
					inner join IndexView I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
				group by C.contractID, C.contractTypeID, 
						C.symbol, C.contractName, 
						I.yield, I.isHTB, I.actionDate, 
						I.actionID,	I.CalcOptionType,
						I.CalcModelType, C.expCalendarID, I.isBasket, 
						I.isActive,	I.skew,	I.kurt
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt
				from ContractView c 	
					inner join IndexView I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where C.contractTypeID=1
					and tc.traderID = @iTraderID
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
				group by C.contractID,
						C.contractTypeID, C.symbol, C.contractName,
						I.yield, I.isHTB, I.actionDate, 
						I.actionID,I.CalcOptionType,
						I.CalcModelType,C.expCalendarID,
						I.isBasket, I.isActive,	I.skew,	I.kurt
				order by C.symbol
		end
		else
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join IndexView I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
					I.yield, I.isHTB, I.actionDate, I.actionID,I.CalcOptionType,
					I.CalcModelType,C.expCalendarID,I.isBasket, I.isActive,	I.skew, I.kurt
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,

					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join IndexView I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and tc.traderID = @iTraderID
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
					I.yield, I.isHTB, I.actionDate, I.actionID,I.CalcOptionType,
					I.CalcModelType,C.expCalendarID,I.isBasket, I.isActive,
					I.skew,I.kurt
				order by C.symbol
		end
	end
if (@@error <> 0)
	Return (-1)
else  
	Return (0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   PROCEDURE dbo.usp_PositionForReconcile_Get
	@tiByTrader tinyint = 0,
	@lastUTCTradeDate datetime = null
AS
	if isnull(@tiByTrader, 0) = 0
	begin
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as vcOptionRootID,
			null as vcOptionRootSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			1 as iLotSize,
			cp.priceClose as fPriceClose,

			null as iTraderID,
			null as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty
		
		from TradeView tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where c.contractTypeID < 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by c.contractID, c.contractTypeID, c.Symbol, cp.priceClose
		
		union
	
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			ORt.optionRootID as vcOptionRootID,
			ORt.symbol as vcOptionRootSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,

			ORt.lotSize as iLotSize,
			cp.priceClose as fPriceClose,

			null as iTraderID,
			null as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty

		from TradeView tr 
			inner join ContractView C on tr.contractID = C.contractID
			inner join OptionView o on c.contractID = o.optionID
			inner join OptionRootView ORt on o.optionRootID = ORt.optionRootID
			inner join ContractView UC on ORt.underlyingID = UC.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
	
		where c.contractTypeID = 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by C.contractID, C.contractTypeID, C.Symbol,ORt.optionRootID,ORt.symbol,
			EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
			ORt.lotSize, cp.priceClose
	
		order by vcSymbol
	end
	else
	begin
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as vcOptionRootID,
			null as vcOptionRootSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			1 as iLotSize,
			cp.priceClose as fPriceClose,
			trdr.TraderID as iTraderID,
			trdr.acronym as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty
		from TradeView tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join TraderView trdr on tr.traderID = trdr.traderID
		where c.contractTypeID < 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by c.contractID, c.contractTypeID, c.Symbol,cp.priceClose, trdr.TraderID, trdr.acronym
		
		union
	
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			ORt.optionRootID as vcOptionRootID,
			ORt.symbol as vcOptionRootSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			ORt.lotSize as iLotSize,
			cp.priceClose as fPriceClose,
			trdr.TraderID as iTraderID,
			trdr.acronym as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty

		from TradeView tr 
			inner join ContractView C on tr.contractID = C.contractID
			inner join OptionView o on c.contractID = o.optionID
			inner join OptionRootView ORt on o.optionRootID = ORt.optionRootID
			inner join ContractView UC on ORt.underlyingID = UC.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			left join TraderView trdr on tr.traderID = trdr.traderID
	
		where c.contractTypeID = 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by C.contractID, C.contractTypeID, C.Symbol,ORt.optionRootID,ORt.symbol,
			EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
			ORt.lotSize, cp.priceClose, trdr.TraderID, trdr.acronym
	
		order by vcSymbol
	end

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   PROCEDURE dbo.usp_PositionsForExternalApp_Get  
	@iContractTypeID int = 2 -- 2 for Stocks,3 - for Options
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Trade table special for A.D.
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iContractTypeID = 2
		begin
			select 
				convert(varchar,tradeDate,103) as 'Trade Date',
				Trader.acronym as 'Account Acronym',
				C.Symbol as 'Stock Symbol',
				C.Symbol as 'Full Option Symbol',
				null as 'Option Root',
				null as 'Expiration Month',
				null as 'Strike',
				'S' as 'Option Type',
				sum(TV.quantity * (case when TV.isBuy <> 0 then 1 else -1 end)) as 'Position',
				null as 'OptionMark',
				cast(CP.priceClose as decimal(15,2)) as [Underlier/Stock Mark],
				1 as 'Contract Multiplier',
				'STOCK' as [OPTION\STOCK],
				null as 'Expiration Date'
			from TradeView TV
				join Trader on TV.traderID = Trader.traderID 
				join Contract C on C.contractID = TV.contractID
				join ContractPrice CP on TV.contractID = CP.contractID and CP.exchangeID is null
			where (tv.status != 1 or tv.status is null) and C.contractTypeID = 2
			group by	convert(varchar,tradeDate,103),
						C.symbol,
						Trader.acronym,	
						CP.priceClose
			order by Trader.acronym,C.symbol
			
		end 
	else if @iContractTypeID = 3
		begin
			select 
				convert(varchar,tradeDate,103) as 'Trade Date',
				Trader.acronym as 'Account Acronym',
				UC.Symbol as 'Stock Symbol',
				left(C.symbol,len(C.symbol) - 2) + cast(Upper(Datename(month, EC.expiryDate)) as char(3)) + Cast(Cast(O.strike as int) as varchar)  + Case O.IsCall when 1 then 'C' else 'P' end  as 'Full Option Symbol',
				left(C.symbol,len(C.symbol) - 2) as 'Option Root',
				cast(Upper(Datename(month, EC.expiryDate)) as char(3)) as 'Expiration Month',
				Cast(O.strike as decimal(15,2)) as 'Strike',
				Case O.isCall when 1 then 'C' else 'P' end 'Option Type',
				sum(TV.quantity * (case when TV.isBuy <> 0 then 1 else -1 end)) as 'Position',
				cast (CP.priceClose as decimal(15,2)) as 'OptionMark',
				cast(UCP.priceClose as decimal(15,2)) as [Underlier/Stock Mark],
    				ORt.lotSize as 'Contract Multiplier',
				'OPTION' as [OPTION\STOCK],
				convert(varchar,EC.expiryDate,103) as 'Expiration Date'
			from TradeView TV
				join Trader on TV.traderID = Trader.traderID 
				join [Option] O on TV.contractID = O.optionID
				join Contract C on C.contractID = O.optionID
				join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				join Contract UC on ORt.underlyingID = UC.contractID
				join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
				join ContractPrice CP on TV.contractID = CP.contractID and CP.exchangeID is null
				join ContractPrice UCP on ORt.underlyingID = UCP.contractID and CP.exchangeID is null
			where (tv.status != 1 or tv.status is null)
			group by 	convert(varchar,TV.tradeDate,103),
						C.symbol, --c.contractID
						Trader.acronym,
						UC.Symbol,
						EC.expiryDate,
						O.strike,
						O.IsCall,
						ORt.lotSize,
						CP.priceClose,
						UCP.priceClose
			order by convert(varchar,tradeDate,103),
					UC.Symbol,
					Trader.acronym,
					EC.expiryDate,
					cast(O.strike as int),O.isCall desc
		end 
	else 
		Raiserror ('Please use 2 or 3 for input parameter (@iContractTypeID = 2 or 3 for Stocks\Options )',16,2)

		
	set @error = @@error
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE     proc dbo.usp_TradeByTrader_Get
	@iTraderID int = null
as

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
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			null as fPriceTheoClose,
			
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.brokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,	
			1 as iLotSize,					
			tr.status as tiStatus,
			null as iOptionRootID,
			null as tiIsSyntheticRoot
		from Trade tr
			inner join Contract c on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join Stock s on c.contractID = s.stockID
			left join [Index] i on c.contractID = i.indexID
		where c.contractTypeID < 3
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,cp.priceClose
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
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.brokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			ort.lotSize as iLotSize,
			tr.status as tiStatus,
			ort.optionRootID as iOptionRootID,
			ort.isSynthetic as tiIsSyntheticRoot
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
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,
			ort.lotSize,cp.priceClose,ucp.priceClose, ort.optionRootID,ort.isSynthetic
		having min(tr.actionID) = 1
		order by TradeDate

     Return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexBetaHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexBetaHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexUnderlyingBeta_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexUnderlyingBeta_Clear]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexUnderlyingBeta_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexUnderlyingBeta_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexUnderlyingBeta_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexUnderlyingBeta_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockWithPos_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockWithPos_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeCA_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeCA_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeCA_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeCA_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeCA_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeCA_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_1_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_1_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_1_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_1_New]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_New]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeForReconcile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeForReconcile_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSeqHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeSeqHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSeq_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeSeq_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Clear]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_New]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Update]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingByGroup_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingByGroup_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingOptions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingOptions_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingPosByList_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingPosByList_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingPos_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingPos_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Underlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Underlying_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


Create  PROC dbo.usp_IndexBetaHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Zaur
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select indexID as iIndexID,
		contractID as iContractID,
		beta as fBeta,
		actionDate as dtActionDate,
		actionID as tiActionID
	from IndexBeta
	where (actionDate > @dtActionDate or @dtActionDate is null)
	order by actionDate

Return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   Procedure dbo.usp_IndexUnderlyingBeta_Clear
	@iIndexID int = null, 	--the index identifier
	@iContractID int = null 	--the stock identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save data into IndexDefinition table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	update IndexBeta set actionID=3
		where 
			(@iIndexID is null or indexID = @iIndexID) and 
			(@iContractID is null or contractID = @iContractID)

	if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

    set @error = 0
finish:
Return(@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE PROCEDURE dbo.usp_IndexUnderlyingBeta_Get 
	@iIndexID int = null, 
	@iContractID int = null
AS
/*
	Created by: Zaur
	Description: this procedure created with template, desined by Zaur Nuraliev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select 
		contractBetaID as iContractBetaID, 
		indexID as iIndexID, 
		contractID as iContractID, 
		beta as fBeta,
		actionID as tiActionID, 
		actionDate as dtActionDate
	from IndexBetaView 
	where (indexID = @iIndexID or @iIndexID is null) and (contractID = @iContractID or @iContractID is null)
		
	set @error = @@error
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE         Procedure dbo.usp_IndexUnderlyingBeta_Save 
	@iIndexID int = null, --the index identifier
	@iUnderlyingID int = null, --the underlying identifier
	@fBeta float,
	@vcIndexSymbol varchar(8) = null,
	@vcUnderlyingSymbol varchar(8) = null
as
/*
	Created by Mike Malyshkin
	Description: this procedure save data into IndexBeta table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iIndexID is null and @iUnderlyingID is null and @vcIndexSymbol is null and @vcUnderlyingSymbol is null 
		begin
			Raiserror ('Impossible define identifiers of the Underlying and Index!', 16, 1)
			Return (-1)
		end   

	if @vcIndexSymbol is not null and @iIndexID is null
		select	@iIndexID = contractID from Contract where symbol = @vcIndexSymbol and contractTypeID = 1 and actionID < 3
		if @iIndexID is null 
			begin
				Raiserror ('Impossible define identifier of the Index!', 16, 1)
				Return (-1)
			end   

	if @vcUnderlyingSymbol is not null and @iUnderlyingID is null 
		select	@iUnderlyingID = contractID from Contract where symbol = @vcUnderlyingSymbol and contractTypeID in (1,2) and actionID < 3
		if @iUnderlyingID is null 
			begin
				Raiserror ('Impossible define identifier of the Underlying!', 16, 1)
				Return (-1)
			end   

	Begin Transaction
		if not exists(select * From IndexBetaView where indexID = @iIndexID and contractID = @iUnderlyingID) 
			begin
				insert into IndexBeta (indexID,contractID,beta) 
				values (@iIndexID,@iUnderlyingID,@fBeta)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update IndexBeta
					set beta = isnull(@fBeta, beta)
					where indexID = @iIndexID and contractID = @iUnderlyingID
	
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

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





CREATE     proc dbo.usp_StockWithPos_Get
	@iStockID int = null,
	@iTraderID int = null,
	@tiPositionMode tinyint = 0 -- 0 - filter by TraderContract, 1 - don't filter by TraderContract
as
	------------------------------------------------
	set nocount on
	------------------------------------------------

	if @iTraderID is null
		select	C.contractID as iContractID, 
			C.contractTypeID as iContractTypeID, 
			C.Symbol as vcSymbol, 
			C.ContractName as vcContractName, 
			S.DivFreq as iDivFreq,
			S.primaryExchangeID as iPrimaryExchangeID, 
			S.DivDate as dtDivDate,
			S.DivAmt as fDivAmt, 
			S.isHTB as bIsHTB,
			S.actionDate as dtActionDate,
			S.actionID as tiActionID,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
			S.isDivCustom as tiDivCustom,
			S.divAmtCustom as fDivAmtCustom,
			S.divDateCustom as dtDivDateCustom,
			S.divFreqCustom as iDivFreqCustom,
			S.CalcOptionType as tiCalcOptionType,
			S.CalcModelType as tiCalcModelType,
			C.expCalendarID as iExpCalendarID,
			S.isActive as tiIsActive,
			s.skew as fSkew,
			s.kurt as fKurt,
			sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
	    from ContractView c  
			inner join StockView S on S.stockID = C.contractID
			join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
			left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) = 0
	    where c.contractTypeID = 2
			and (c.contractID = @iStockID or @iStockID is Null)
		group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
			S.DivFreq,S.primaryExchangeID, 
			S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
			S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,
			S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive, s.skew, s.kurt
		order by c.Symbol
	else
	begin 
		if isnull(@tiPositionMode, 0) = 0
			select	C.contractID as iContractID, 
				C.contractTypeID as iContractTypeID, 
				C.Symbol as vcSymbol, 
				C.ContractName as vcContractName, 

				S.DivFreq as iDivFreq,
				S.primaryExchangeID as iPrimaryExchangeID, 
				S.DivDate as dtDivDate,
				S.DivAmt as fDivAmt, 
				S.isHTB as bIsHTB,
				S.actionDate as dtActionDate,
				S.actionID as tiActionID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				S.isDivCustom as tiDivCustom,
				S.divAmtCustom as fDivAmtCustom,
				S.divDateCustom as dtDivDateCustom,
				S.divFreqCustom as iDivFreqCustom,
				S.CalcOptionType as tiCalcOptionType,
				S.CalcModelType as tiCalcModelType,
				C.expCalendarID as iExpCalendarID,
				S.isActive as tiIsActive,
				s.skew as fSkew,
				s.kurt as fKurt,
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
		    from ContractView c  
				inner join StockView S on S.stockID = C.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) = 0
		    where c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (c.contractID = @iStockID or @iStockID is Null)
			group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
				S.DivFreq, S.primaryExchangeID, 
				S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
				S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,
				S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive, s.skew, s.kurt
			order by c.Symbol
		else
			select	C.contractID as iContractID, 
				C.contractTypeID as iContractTypeID, 
				C.Symbol as vcSymbol, 
				C.ContractName as vcContractName, 

				S.DivFreq as iDivFreq,
				S.primaryExchangeID as iPrimaryExchangeID, 
				S.DivDate as dtDivDate,
				S.DivAmt as fDivAmt, 
				S.isHTB as bIsHTB,
				S.actionDate as dtActionDate,
				S.actionID as tiActionID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				S.isDivCustom as tiDivCustom,
				S.divAmtCustom as fDivAmtCustom,
				S.divDateCustom as dtDivDateCustom,
				S.divFreqCustom as iDivFreqCustom,
				S.CalcOptionType as tiCalcOptionType,
				S.CalcModelType as tiCalcModelType,
				C.expCalendarID as iExpCalendarID,
				S.isActive as tiIsActive,
				s.skew as fSkew,
				s.kurt as fKurt,
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
		    from ContractView c  
				inner join StockView S on S.stockID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) = 0
		    where c.contractTypeID = 2
				and (c.contractID = @iStockID or @iStockID is Null)
			group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
				S.DivFreq, S.primaryExchangeID, 
				S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
				S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,
				S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive, s.skew, s.kurt
			order by c.Symbol
	end
	
Return(@@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





CREATE           proc dbo.usp_Stock_Get
	@iGroupID int = null,
	@iStockID int = null,
	@tiWithPosition tinyint = 0,
	@iTraderID int = null
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Stock,Contract tables
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if isnull(@tiWithPosition, 0) = 0
	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt
				from ContractView C  
					inner join StockView S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID = 2
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				order by c.Symbol
			else
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt
				from ContractView C  
					inner join StockView S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				order by c.Symbol
		end
		else
		begin
			if @iTraderID is null
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join StockView S on S.stockID = C.contractID

					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
				order by c.Symbol
			else
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join StockView S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
					and tc.traderID = @iTraderID
				order by c.Symbol
		end
	end
	else
	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt
			    from ContractView c  
					inner join StockView S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) = 0
			    where c.contractTypeID = 2
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				group by C.contractID, 
					C.contractTypeID, 
					C.Symbol, 
					C.ContractName,
					S.DivFreq,
					S.DivDate, 
					S.DivAmt, 
					S.isHTB, 
					S.actionDate,	
					S.actionID,
					S.isDivCustom,
					S.divAmtCustom,
					S.divDateCustom,
					S.divFreqCustom,
					S.CalcOptionType,
					S.CalcModelType,
					C.expCalendarID, 
					S.isActive,
					S.primaryExchangeID,
					S.skew,
					S.kurt
				order by c.Symbol
			else
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt
			    from ContractView c  
					inner join StockView S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) = 0
			    where c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
					S.DivFreq, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive,
					S.skew,	S.kurt
				order by c.Symbol
		end
		else
		begin
			if @iTraderID is null
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt
			    from ContractInGroupView cig
					inner join ContractView c  on C.contractID = CIG.contractID
					inner join StockView S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) = 0
		        where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, 
					S.DivFreq,S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive,
					S.skew, S.kurt
				order by c.Symbol
			else
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt
			    from ContractInGroupView cig
					inner join ContractView c  on C.contractID = CIG.contractID
					inner join StockView S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) = 0
		        where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
					and tc.traderID = @iTraderID
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
					S.DivFreq, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive,
					S.skew, S.kurt
				order by c.Symbol
		end
	end
	
	set @error = @@error

Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO





CREATE     proc dbo.usp_TradeCA_Del
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

	insert into Trade(
		tradeID,
		actionID,
		contractID,
		isClose,
		tradeDate,
		quantity,
		price,
		isBuy,
		bookID,
		strategyID,
		traderID,
		brokerID,
		clearingBrokerID,
		brokerCommissionID,
		clearingBrokerCommissionID,
		tradedIV,
		spotReference,
		isPosition,
		status,
		execID,
		seqNum)
	select 
		tradeID,
		0,
		contractID,
		isClose,
		tradeDate,
		quantity,
		price,
		isBuy,
		bookID,
		strategyID,
		traderID,
		brokerID,
		clearingBrokerID,
		brokerCommissionID,
		clearingBrokerCommissionID,
		tradedIV,
		spotReference,
		isPosition,
		status,
		execID,
		@iNewSeqNum
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE         proc dbo.usp_TradeCA_Get
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
			isClose as tiIsClose,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			bookID as iBookID,     
			strategyID as iStrategyID, 
			traderID as iTraderID,
			brokerID as iBrokerID,
			clearingBrokerID as iClearingBrokerID,
			brokerCommissionID as iBrokerCommissionID,
			clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tradedIV as fTradedIV,
			spotReference as fSpotReference,
			isPosition as tiIsPosition,
			status as tiStatus
			from Trade
				where tradeID = @iTradeID
			group by tradeID,contractID,isClose,tradeDate,quantity,price,isBuy,bookID,strategyID,traderID,brokerID,clearingBrokerID,brokerCommissionID,clearingBrokerCommissionID,tradedIV,spotReference,isPosition,status
			having min(actionID) = 1
			order by TradeDate
	end
	else if @iContractID is not null
	begin
		select
			tradeID as iTradeID,
			contractID as iContractID,
			isClose as tiIsClose,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			bookID as iBookID,     
			strategyID as iStrategyID, 
			traderID as iTraderID,
			brokerID as iBrokerID,
			clearingBrokerID as iClearingBrokerID,
			brokerCommissionID as iBrokerCommissionID,
			clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tradedIV as fTradedIV,
			spotReference as fSpotReference,
			isPosition as tiIsPosition,
			status as tiStatus
			from Trade
				where contractID = @iContractID
			group by tradeID,contractID,isClose,tradeDate,quantity,price,isBuy,bookID,strategyID,traderID,brokerID,clearingBrokerID,brokerCommissionID,clearingBrokerCommissionID,tradedIV,spotReference,isPosition,status
			having min(actionID) = 1
			order by TradeDate
	end
	else if @iOptionRootID is not null
	begin
		select
			tradeID as iTradeID,
			contractID as iContractID,
			isClose as tiIsClose,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			bookID as iBookID,     
			strategyID as iStrategyID, 
			traderID as iTraderID,
			brokerID as iBrokerID,
			clearingBrokerID as iClearingBrokerID,
			brokerCommissionID as iBrokerCommissionID,
			clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tradedIV as fTradedIV,
			spotReference as fSpotReference,
			isPosition as tiIsPosition,
			T.status as tiStatus
			from Trade T	
				join OptionView O on O.optionID = T.contractID
				where o.optionRootID = @iOptionRootID
			group by tradeID,contractID,isClose,tradeDate,quantity,price,isBuy,bookID,strategyID,traderID,brokerID,clearingBrokerID,brokerCommissionID,clearingBrokerCommissionID,tradedIV,spotReference,isPosition,status
			having min(T.actionID) = 1
			order by TradeDate
	end
	else if @iUnderlyingID is not null
	begin
		select
			tradeID as iTradeID,
			contractID as iContractID,
			isClose as tiIsClose,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			bookID as iBookID,     
			strategyID as iStrategyID, 
			traderID as iTraderID,
			brokerID as iBrokerID,
			clearingBrokerID as iClearingBrokerID,
			brokerCommissionID as iBrokerCommissionID,
			clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tradedIV as fTradedIV,
			spotReference as fSpotReference,
			isPosition as tiIsPosition,
			status as tiStatus
			from Trade T	
				join OptionView O on O.optionID = T.contractID
				join OptionRootView ORT on ORT.optionRootID=O.optionRootID
				where ORT.underlyingID = @iUnderlyingID
			group by tradeID,contractID,isClose,tradeDate,quantity,price,isBuy,bookID,strategyID,traderID,brokerID,clearingBrokerID,brokerCommissionID,clearingBrokerCommissionID,tradedIV,spotReference,isPosition,status
			having min(T.actionID) = 1
			order by TradeDate
	end

     Return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO





CREATE      proc dbo.usp_TradeCA_Save
		@iCorpActionID int,
        @iTradeID int = null, --for new trade is null
		@iQuantity int, 
		@fPrice float, 
		@iContractID int = null, 
		@tiIsClose tinyint = 0, 
		@tiIsBuy tinyint = 0, 
		@iTraderID int = null, 
		@tiIsPosition tinyint = 0,
		@tiStatus tinyint = 0
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
			insert into Trade(tradeID,
				actionID,
				contractID,
				isClose,
				quantity,
				price,
				isBuy,
				traderID,
				isPosition,
				status,
				SeqNum,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradeDate)
			select @iNewTradeID,
				1,
				contractID,
				isClose,
				@iQuantity,
				@fPrice,
				isBuy,
				traderID,
				isPosition,
				status,
				@iNewSeqNum,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradeDate
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
			exec master.dbo.xp_regread 'HKEY_LOCAL_MACHINE', 'SYSTEM\CurrentControlSet\Control\TimeZoneInformation', 'Bias', @iDeltaGMT OUT
			set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())

			-- insert new trade
			insert into Trade(
				tradeID,
				actionID,
				contractID,
				isClose,
				quantity,
				price,
				isBuy,
				traderID,
				isPosition,
				status,
				seqNum,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradeDate)
			select 
				@iNewTradeID,
				1,
				@iContractID,
				@tiIsClose,
				@iQuantity,
				@fPrice,
				@tiIsBuy,
				@iTraderID,
				@tiIsPosition,
				@tiStatus,
				@iNewSeqNum,
				Null,
				Null,
				@dtNewTradeDate

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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO






CREATE  procedure dbo.usp_TradeFeed_1_Del
	@vcExecID varchar(20),
	@dtTradeDate datetime,
	@tiIsUpdateEnabled tinyint = 0,
	@iLastTradeTime int = null
/*
	Created by Zaur Nuraliev
	Description: this procedure is mark as delete information in Trade table
*/
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
		insert into Trade(tradeID,
			actionID,
			contractID,
			isClose,
			tradeDate,
			quantity,
			price,
			isBuy,
			bookID,
			strategyID,
			traderID,
			brokerID,
			clearingBrokerID,
			brokerCommissionID,
			clearingBrokerCommissionID,
			tradedIV,
			spotReference,
			isPosition,
			status,
			execID,
			seqNum)
		select tradeID,
			0,
			contractID,
			isClose,
			tradeDate,
			quantity,
			price,
			isBuy,
			bookID,
			strategyID,
			traderID,
			brokerID,
			clearingBrokerID,
			brokerCommissionID,
			clearingBrokerCommissionID,
			tradedIV,
			spotReference,
			isPosition,
			status,
			execID,
			@iNewSeqNum
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE      PROCEDURE dbo.usp_TradeFeed_1_New
	@vcTraderAcronym varchar(12),
	@vcExecID varchar(20),
	@tiIsBuy tinyint,  
	@iQuantity int,
	@fPrice float,
	@dtTradeDate datetime,
	@vcContractSymbol varchar(8),
	@iContractTypeID int,
	@vcUnderlyingSymbol varchar(8) = null,
	@tiIsCall tinyint = null,
	@fStrike float = null,
	@dtExpiry datetime = null,
	@iTradeLotSize int = null,
	@tiIsUpdateEnabled tinyint = 0,
	@iLastTradeTime int = null, 
	@vcOptRootSymbol varchar(20) = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is save information into Trade table
*/
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
					exec @error = usp_TradeFeed_1_Del @vcExecID,@dtTradeDate, @tiIsUpdateEnabled
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


	if @iTradeLotSize is null
		begin	
			if @iContractTypeID = 3 
				set @iTradeLotSize = 100
			else if @iContractTypeID < 3 
				set @iTradeLotSize = 1
		end

	Begin Transaction
	if @iUnderlyingContractID is null
	--add new stock\index
		begin
			insert into Contract(symbol,contractTypeID,expCalendarID) values(@vcUnderlyingSymbol, 2, 1)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			set @iUnderlyingContractID = @@identity

			insert into Stock (stockID,isActive) values(@iUnderlyingContractID,1)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

			if @iTradeLotSize is null set @iTradeLotSize = 1

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
								insert into OptionRoot(underlyingID,symbol,lotSize) values(@iUnderlyingContractID,@vcOptRootSymbol,@iTradeLotSize)
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

	--create new trade	
	insert into Trade (
		execID,
		tradeID,
		actionID, 
		contractID,
		isClose, 
		tradeDate, 
		quantity, 
		price, 
		traderID, 
		isBuy, 
		isPosition,
		status,
		brokerCommissionID,
		clearingBrokerCommissionID,
		seqNum)
	values (
		@vcExecID,
		@iNewTradeID,
		1,
		@iTradeContractID, 
		0, 
		@dtTradeDate, 
		@iQuantity, 
		@fPrice, 
		@iTraderID, 
		@tiIsBuy, 
		0,
		0,
		null,
		null,
		@iSeqNum)

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
	select -1 as iMinSeqNum,-1 as iMaxSeqNum, -1 as iStructureMissed
	Return(@@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO





CREATE         proc dbo.usp_TradeFeed_Del
	@vcExecID varchar(20),
	@dtTradeDate datetime
/*
	Created by Zaur Nuraliev
	Description: this procedure is mark as delete information in Trade table
*/
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	if not exists(select * from Trade where execID = @vcExecID and actionID > 0 and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tradeDate,112))
		begin
			--Raiserror('Record with this execID does not exists in DB!',16,1)
			--Return(-1)
			select null as iSeqNum
		end

	Declare @iSeqNum int,
		@iContractTypeID int,
		@iNewSeqNum int

	select @iSeqNum = seqNum, @iContractTypeID = contractTypeID
		from Contract C join Trade T on C.contractID = T.contractID
		where execID = @vcExecID and T.actionID > 0 

	select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade

--    if @iSeqNum is null or @iContractTypeID is null 
--		Return(-1)


	Begin tran

			insert into Trade(tradeID,
				actionID,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				status,
				execID,
				seqNum)
			select tradeID,
				0,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				status,
				execID,
				@iNewSeqNum
			from Trade
			where seqNum = @iSeqNum

			if @@error = 0
				begin
					--set @iNewSeqNum = @@identity
					Commit Tran
					--Return @iNewSeqNum

					if @iContractTypeID < 3 
						select
							tr.seqNum as iSeqNum,
							tr.actionDate as dtActionDate,
							tr.tradeID as iTradeID,
							tr.actionID as tiActionID,
							tr.isClose as tiIsClose,
							tr.tradeDate as dtTradeDate,
							tr.quantity as iQuantity,
							tr.price as fPrice,
							tr.isBuy as tiIsBuy,
							tr.bookID as iBookID,     
							tr.strategyID as iStrategyID, 
							tr.traderID as iTraderID,
							tr.brokerID as iBrokerID,
							tr.clearingBrokerID as iClearingBrokerID,
							tr.BrokerCommissionID as iBrokerCommissionID,
							tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
							cv1.CommissionValue as fBrokerCommission,
							cv2.CommissionValue as fClearingBrokerCommission,
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
							null as bIsCall,
							c.contractID as iUnderlyingContractID,
							c.contractTypeID as iUnderlyingContractTypeID,
							c.Symbol as vcUnderlyingSymbol,
							null as fPriceTheoClose,
							tr.isPosition as tiIsPosition,
							tr.status as tiStatus,
							null as fPriceClose,
							null as fUnderlyingPriceClose
						from Trade tr 
							join Contract C  on tr.contractID = c.contractID
							join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
							left join StockView s on c.contractID = s.stockID
							left join IndexView i on c.contractID = i.indexID
							left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
							left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
						where tr.seqNum = @iNewSeqNum
				
					else --if @iContractTypeID =3

						select
							tr.seqNum as iSeqNum,
							tr.actionDate as dtActionDate,
							tr.tradeID as iTradeID,
							tr.actionID as tiActionID,
							tr.isClose as tiIsClose,
							tr.tradeDate as dtTradeDate,
							tr.quantity as iQuantity,
							tr.price as fPrice,
							tr.isBuy as tiIsBuy,
							tr.bookID as iBookID,     
							tr.strategyID as iStrategyID, 
							tr.traderID as iTraderID,
							tr.brokerID as iBrokerID,
							tr.clearingBrokerID as iClearingBrokerID,
							tr.BrokerCommissionID as iBrokerCommissionID,
							tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
							cv1.CommissionValue as fBrokerCommission,
							cv2.CommissionValue as fClearingBrokerCommission,
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
							o.isCall as bIsCall,
							uc.contractID as iUnderlyingContractID,
							uc.contractTypeID as iUnderlyingContractTypeID,
							uc.Symbol as vcUnderlyingSymbol,
							cp.priceTheo as fPriceTheoClose,
							tr.isPosition as tiIsPosition,
							ort.lotSize as iLotSize,
							tr.status as tiStatus,
							cp.PriceClose as fPriceClose,
							ucp.priceClose as fUnderlyingPriceClose
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
							left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
							left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
						where tr.seqNum = @iNewSeqNum
				end
			else
				begin
					Rollback Tran
					Return (0)
				end

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  PROCEDURE dbo.usp_TradeFeed_Get
	@vcExecID varchar(20),
	@dtTradeDate datetime
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is save information into table_name table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	select
		tr.seqNum as iSeqNum,
		tr.actionDate as dtActionDate,
		tr.tradeID as iTradeID,
		tr.actionID as tiActionID,
		tr.isClose as tiIsClose,
		tr.tradeDate as dtTradeDate,
		tr.quantity as iQuantity,
		tr.price as fPrice,
		tr.isBuy as tiIsBuy,
		tr.bookID as iBookID,     
		tr.strategyID as iStrategyID, 
		tr.traderID as iTraderID,
		tr.brokerID as iBrokerID,
		tr.clearingBrokerID as iClearingBrokerID,
		tr.BrokerCommissionID as iBrokerCommissionID,
		tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
		cv1.CommissionValue as fBrokerCommission,
		cv2.CommissionValue as fClearingBrokerCommission,
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
		null as bIsCall,
		c.contractID as iUnderlyingContractID,
		c.contractTypeID as iUnderlyingContractTypeID,
		c.Symbol as vcUnderlyingSymbol,
		null as fPriceTheoClose,
		tr.isPosition as tiIsPosition,
		tr.status as tiStatus,
		null as fPriceClose,
		null as fUnderlyingPriceClose
	from Trade tr 
		join Contract C  on tr.contractID = c.contractID
		join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		left join StockView s on c.contractID = s.stockID
		left join IndexView i on c.contractID = i.indexID
		left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
		left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
	where tr.actionID > 0 and tr.execID = @vcExecID and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tr.tradeDate,112)

union

	select
		tr.seqNum as iSeqNum,
		tr.actionDate as dtActionDate,
		tr.tradeID as iTradeID,
		tr.actionID as tiActionID,
		tr.isClose as tiIsClose,
		tr.tradeDate as dtTradeDate,
		tr.quantity as iQuantity,
		tr.price as fPrice,
		tr.isBuy as tiIsBuy,
		tr.bookID as iBookID,     
		tr.strategyID as iStrategyID, 
		tr.traderID as iTraderID,
		tr.brokerID as iBrokerID,
		tr.clearingBrokerID as iClearingBrokerID,
		tr.BrokerCommissionID as iBrokerCommissionID,
		tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
		cv1.CommissionValue as fBrokerCommission,
		cv2.CommissionValue as fClearingBrokerCommission,
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
		o.isCall as bIsCall,
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		cp.priceTheo as fPriceTheoClose,
		tr.isPosition as tiIsPosition,
		ort.lotSize as iLotSize,
		tr.status as tiStatus,
		cp.PriceClose as fPriceClose,
		ucp.priceClose as fUnderlyingPriceClose
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
		left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
		left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
	where tr.actionID > 0 and tr.execID = @vcExecID and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tr.tradeDate,112)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
Create PROCEDURE dbo.usp_TradeFeed_New
	@vcTraderAcronym varchar(12),
	@vcExecID varchar(20),
	@tiIsBuy tinyint,  
	@iQuantity int,
	@fPrice float,
	@dtTradeDate datetime,
	@vcContractSymbol varchar(8),
	@iContractTypeID int,
	@vcUnderlyingSymbol varchar(8) = null,
	@tiIsCall tinyint = null,
	@fStrike float = null,
	@dtExpiry datetime = null,
	@iTradeLotSize int = null,
	@vcOptionRootSymbol varchar(20) = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is save information into table_name table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if exists(select * from Trade where execID = @vcExecID and actionID > 0 and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tradeDate,112))
		begin
			--Raiserror('Record with same execID alreadty exists in DB!',16,1)
			Select null as iSeqNum
			Return(-1)

		end

	
	--check the option root symbol
	declare @iOptionRootID int

	Begin Transaction

	Declare @iTradeContractID int, @iStructureMissed int
	set @iStructureMissed = 0

	--check underlying symbol
	Declare @iUnderlyingContractID int

		if @vcUnderlyingSymbol is null and  @iContractTypeID <3 
			set @vcUnderlyingSymbol = @vcContractSymbol

	select @iUnderlyingContractID = contractID from ContractView where symbol = @vcUnderlyingSymbol and contractTypeID in(1,2)


	if @iUnderlyingContractID is null
	--add new stock\index
		begin
			insert into Contract(symbol,contractTypeID,expCalendarID) values(@vcUnderlyingSymbol, 2, 1)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iUnderlyingContractID = @@identity

			insert into Stock (stockID,isActive) values(@iUnderlyingContractID,1)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into ContractPrice (contractID,exchangeID) values(@iUnderlyingContractID,null)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			set @iStructureMissed = 1

			--insert into TradeLog (contractID, eventTypeID) values (@iUnderlyingContractID, 0)
			--	if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end
	else
		begin
			update Stock set isActive = 1 where stockID = @iUnderlyingContractID
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end

	--check Option
	if @iContractTypeID = 3
		begin
			Declare @iOptionID int,
					@iExpiryID int
	
			set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
			set @fStrike = round(@fStrike, 3)
	
			if @vcOptionRootSymbol is not null
			begin
				select @iOptionRootID = optionRootID 
					from OptionRootView where symbol = @vcOptionRootSymbol

				if @iOptionRootID is null
				begin
					insert into OptionRoot (underlyingID,symbol,lotSize)
						values (@iUnderlyingContractID,@vcOptionRootSymbol,isnull(@iTradeLotSize,100))

					set @iOptionRootID = @@identity
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			end

			select @iOptionID = O.optionID
				from OptionView O (nolock)
				where optionRootID = @iOptionRootID and 
					isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID --and O.actionID<>3
	
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
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
							set @iOptionID = @@identity
				
						insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
							 values (@iOptionID, @iOptionRootID, @tiIsCall, @fStrike, @iExpiryID)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				
						-- make record with null exchange for this Option
						insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

						--insert into TradeLog (contractID, eventTypeID) values (@iOptionID, 0)
						--	if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
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
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iTraderID = @@identity
		end

	if not exists(select 1 from TraderContractView where traderID = @iTraderID and contractID = @iUnderlyingContractID)
		begin
			insert into TraderContract (traderID, contractID) values (@iTraderID, @iUnderlyingContractID)
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end

	--create new trade

	Declare @iNewTradeID int
		select @iNewTradeID = max(tradeID) + 1 from Trade

	Declare @iSeqNum int 
		select @iSeqNum = isnull(max(seqNum),0) + 1 from Trade

	if @iNewTradeID is null 
			set @iNewTradeID = 1
	
	insert into Trade (
		execID,
		tradeID,
		actionID, 
		contractID,
		isClose, 
		tradeDate, 
		quantity, 
		price, 
		traderID, 
		isBuy, 
		isPosition,
		status,
		brokerCommissionID,
		clearingBrokerCommissionID,
		seqNum)
	values (
		@vcExecID,
		@iNewTradeID,
		1,
		@iTradeContractID, 
		0, 
		@dtTradeDate, 
		@iQuantity, 
		@fPrice, 
		@iTraderID, 
		@tiIsBuy, 
		0,
		0,
		null,
		null,
		@iSeqNum)

		--set @iSeqNum = @@identity

		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

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

	Commit Transaction

 --if all OK
	if @iContractTypeID < 3 
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
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
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			null as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			null as fPriceClose,
			null as fUnderlyingPriceClose,
			@iStructureMissed as iStructureMissed
		from Trade tr 
			join Contract C  on tr.contractID = c.contractID
			join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where tr.seqNum = @iSeqNum

	else if @iContractTypeID =3
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
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
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceTheo as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			ort.lotSize as iLotSize,
			tr.status as tiStatus,
			CP.PriceClose as fPriceClose,
			UCP.priceClose as fUnderlyingPriceClose,
			@iStructureMissed as iStructureMissed
		from Trade tr 
			join Contract C  on tr.contractID = c.contractID
			inner join [Option] O on C.contractID = O.optionID
			join OptionRoot ORt on ORt.optionRootID = O.optionRootID
			join Contract UC on ORt.underlyingID = UC.contractID
			join ExpiryInCalendarView EC on UC.expCalendarID = EC.expCalendarID and O.expiryID = EC.expiryID

			inner join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
			inner join ContractPrice UCP on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where tr.seqNum = @iSeqNum

    set @error = 0

finish:
Return(@error)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





CREATE  proc dbo.usp_TradeForReconcile_Get
as
		select
			tr.tradeID as iTradeID,
			tr.execID as vcExecID,
			
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			1 as iTradeLotSize
		from TradeView tr
			inner join Contract c on tr.contractID = c.contractID
			left join Stock s on c.contractID = s.stockID
			left join [Index] i on c.contractID = i.indexID
		where c.contractTypeID in (1, 2)
			and isnull(tr.status, 0) != 0
			and isnull(tr.isPosition, 0) = 0
			and isnull(tr.execID, '') != ''

	union

		select
			tr.tradeID as iTradeID,
			tr.execID as vcExecID,
			
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			ort.lotSize as iTradeLotSize

		from TradeView tr 
			inner join Contract c on tr.contractID = c.contractID
			inner join [Option] o on c.contractID = o.optionID
			join OptionRoot ort on o.optionRootID = ort.optionRootID
			join Contract UC on ort.underlyingID = UC.contractID
			join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			left join Stock s on uc.contractID = s.stockID
			left join [Index] i on uc.contractID = i.indexID
		where c.contractTypeID = 3
			and isnull(tr.status, 0) != 0
			and isnull(tr.isPosition, 0) = 0
			and isnull(tr.execID, '') != ''
		order by TradeDate

Return(@@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO






CREATE  PROC dbo.usp_TradeSeqHH_Get
	@iMinSeqNum int = null,
	@iMaxSeqNum int = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
		select
			seqNum as iSeqNum,
			actionDate as dtActionDate,
			tradeID as iTradeID,
			actionID as tiActionID,
			contractID as iContractID,
			isClose as tiIsClose,
			tradeDate as dtTradeDate,
			quantity as iQuantity,
			price as fPrice,
			isBuy as tiIsBuy,
			bookID as iBookID,     
			strategyID as iStrategyID, 
			traderID as iTraderID,
			brokerID as iBrokerID,
			clearingBrokerID as iClearingBrokerID,
			BrokerCommissionID as iBrokerCommissionID,
			clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tradedIV as fTradedIV,
			spotReference as fSpotReference,
			isPosition as tiIsPosition,
			status as tiStatus,
			execID as vcExecID
		from Trade
		where 
			(seqNum >= @iMinSeqNum or @iMinSeqNum is null)
			and (seqNum <= @iMaxSeqNum or @iMaxSeqNum is null)
		order by seqNum

return(@@error)














GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO




CREATE        PROC dbo.usp_TradeSeq_Get
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
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus
		from Trade tr
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
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
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
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
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			null as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			1 as iLotSize,
			tr.status as tiStatus,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			--case when exists(select * from TradeLog where contractID = tr.contractID) then 1 else 0 end iStructureMissed,
			case when exists(select * from TradeLog where contractID = tr.contractID and seqNum = tr.seqNum) then 1 else 0 end iStructureMissed,
			null as iOptionRootID,
			null as tiIsSyntheticRoot
		from Trade tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID=cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
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
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
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
			o.isCall as bIsCall,
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
			ORT.isSynthetic as tiIsSyntheticRoot
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
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where c.contractTypeID = 3
			and (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and tr.tradeDate >= @dtTradeDate and tr.tradeDate < @dtMaxDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		order by tr.seqNum








GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO








CREATE    PROCEDURE dbo.usp_Trade_Clear
	@iContractID int = null,
	@tiOnlyWithExecID tinyint = null,
	@lastUTCTradeDate datetime = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction

		delete from TradeLog 
		where seqNum in (select seqNum from Trade where isnull(status, 0) in (1, 2) and actionID = 0
									 and (@iContractID is null or contractID = @iContractID))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

		delete from Trade 
		where tradeID in (select tradeID from Trade where isnull(status, 0) in (1, 2) and actionID = 0) 
							and (@iContractID is null or contractID = @iContractID)
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
	
		if isnull(@tiOnlyWithExecID, 0) = 0
		begin
			delete from TradeLog 
				where seqNum in (select seqNum from Trade
									where (@iContractID is null or contractID = @iContractID) 
										and isnull(status, 0) in (0, 3)
										and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end	

			delete from Trade 
				where (@iContractID is null or contractID = @iContractID) 
					and isnull(status, 0) in (0, 3)
					and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0)
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
		end
		else
		begin
			delete from TradeLog 
				where seqNum in (select seqNum from Trade
									where (@iContractID is null or contractID = @iContractID) 
										and isnull(status, 0) in (0, 3)
										and isnull(execID, '') <> ''
										and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

			delete from Trade 
			where (@iContractID is null or contractID = @iContractID) 
				and isnull(status, 0) in (0, 3)
				and isnull(execID, '') <> ''
				and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
		end
		set @error = @@error
	Commit Transaction

finish:
	return @error








GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO





CREATE      proc dbo.usp_Trade_Del
        @iTradeID int
as
	if @iTradeID is not null
	begin
		begin tran
		declare @nCount int,
				@iNewSeqNum int

		select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iTradeID

		
		if @nCount = 1
		begin
			insert into Trade(tradeID,
				actionID,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				status,
				execID,
				seqNum)
			select tradeID,
				0,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				status,
				execID,
				@iNewSeqNum
			from Trade
			where tradeID = @iTradeID
			if @@error = 0
			begin
				--set @iNewSeqNum = @@identity
				commit tran
				return @iNewSeqNum
			end
			else
			begin
				rollback tran
				return 0
			end
		end
		else
		begin
			rollback tran
			return 0
		end
	end
	else
		return 0
/*
        delete Trade where tradeID = @iTradeID
        if (@@error <> 0)
           return (-1)
         else  
           return 0
*/





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





CREATE         proc dbo.usp_Trade_Get
	@iTradeID int = null,
	@iContractID int = null,
	@tiExtendedInfo tinyint = 0,
	@iUnderlyingID int = null,
	@iTraderID int = null,
	@iBookID int = null,
	@iStrategyID int = null,
	@iContractGroupID int = null,
	@dtMinTradeDate datetime = null
as

 if @tiExtendedInfo is null or @tiExtendedInfo = 0

	select
		tradeID as iTradeID,
		contractID as iContractID,
		isClose as tiIsClose,
		tradeDate as dtTradeDate,
		quantity as iQuantity,
		price as fPrice,
		isBuy as tiIsBuy,
		bookID as iBookID,     
		strategyID as iStrategyID, 
		traderID as iTraderID,
		brokerID as iBrokerID,
		clearingBrokerID as iClearingBrokerID,
		brokerCommissionID as iBrokerCommissionID,
		clearingBrokerCommissionID as iClearingBrokerCommissionID,
		tradedIV as fTradedIV,
		spotReference as fSpotReference,
		isPosition as tiIsPosition,
		status as tiStatus
		from Trade 
			where 
			(@iTradeID is null or @iTradeID is not null and tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and contractID = @iContractID)
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tradeDate >= @dtMinTradeDate)
		group by tradeID,contractID,isClose,tradeDate,quantity,price,isBuy,bookID,strategyID,traderID,brokerID,clearingBrokerID,brokerCommissionID,clearingBrokerCommissionID,tradedIV,spotReference,isPosition,status
		having min(actionID) = 1
		order by TradeDate

 else if @iContractGroupID is null
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
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			null as fPriceTheoClose,
			
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.brokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,	
			1 as iLotSize,				
			tr.status as tiStatus
		from Trade tr
			inner join Contract c on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join Stock s on c.contractID = s.stockID
			left join [Index] i on c.contractID = i.indexID
		where c.contractTypeID < 3
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
			and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
			and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
			and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
			and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,cp.priceClose
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
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.brokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			ORt.lotSize as iLotSize,
			tr.status as tiStatus
		from Trade tr inner join Contract c on tr.contractID = c.contractID
			inner join [Option] o on c.contractID = o.optionID
			join OptionRoot ORt on ORt.optionRootID = o.optionRootID
			inner join Contract UC on ort.underlyingID = UC.contractID
			join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join Stock s on uc.contractID = s.stockID
			left join [Index] i on uc.contractID = i.indexID
		where c.contractTypeID = 3
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
			and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
			and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
			and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
			and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,
			ORt.lotSize,cp.priceClose,ucp.priceClose
		having min(tr.actionID) = 1
		order by TradeDate
  else
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
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			cp.priceClose as fPriceClose,
			cp.priceClose as fUnderlyingPriceClose,
			null as fPriceTheoClose,
			
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     

			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.brokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			1 as iLotSize,
			tr.status as tiStatus
		from Trade tr
			inner join Contract c on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractInGroup cig on c.contractID = cig.contractID
			left join Stock s on c.contractID = s.stockID
			left join [Index] i on c.contractID = i.indexID
		where c.contractTypeID in (1, 2)
			and cig.groupID = @iContractGroupID
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
			and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
			and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
			and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
			and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,cp.priceClose
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
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceClose as fPriceClose,
			ucp.priceClose as fUnderlyingPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.brokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			ORt.lotSize as iLotSize,
			tr.status as tiStatus
		from Trade tr 
			inner join Contract c on tr.contractID = c.contractID
			inner join [Option] o on c.contractID = o.optionID
			join OptionRootView ORt on o.optionRootID = ort.optionRootID
			join Contract uc on ORt.underlyingID = UC.contractID
			join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			inner join ContractInGroup cig on ort.underlyingID = cig.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ContractPrice ucp on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
			left join Stock s on uc.contractID = s.stockID
			left join [Index] i on uc.contractID = i.indexID
		where c.contractTypeID = 3
			and cig.groupID = @iContractGroupID
			and (@dtMinTradeDate is null or @dtMinTradeDate is not null and tr.tradeDate >= @dtMinTradeDate)
			and (@iTradeID is null or @iTradeID is not null and tr.tradeID = @iTradeID)
			and (@iContractID is null or @iContractID is not null and tr.contractID = @iContractID)
			and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
			and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
			and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
		group by tr.tradeID,c.contractID,c.contractTypeID,c.Symbol,s.divFreq,s.divDate,s.divAmt,i.yield,
			EC.expiryDate,o.strike,o.isCall,uc.contractID,uc.contractTypeID,uc.Symbol,cp.priceTheo,
			tr.contractID,tr.isClose,tr.tradeDate,tr.quantity,tr.price,tr.isBuy,tr.bookID,
			tr.strategyID,tr.traderID,tr.brokerID,tr.clearingBrokerID,tr.brokerCommissionID,
			tr.clearingBrokerCommissionID,tr.tradedIV,tr.spotReference,tr.isPosition,tr.status,
			ORt.lotSize,cp.priceClose, ucp.priceClose
		having min(tr.actionID) = 1
		order by TradeDate

     Return(@@error)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE   procedure dbo.usp_Trade_New
        @iContractID int = Null,
        @tiIsClose tinyint = Null,
        @iQuantity int = Null,
        @fPrice float = Null,
        @tiIsBuy tinyint = Null,  
        @iBookID int = Null,
        @iStrategyID int = Null,
        @iTraderID int = Null,
        @iBrokerID int = Null,
        @iClearingBrokerID int = Null,
        @iBrokerCommissionID int =Null,
        @iClearingBrokerCommissionID int = Null,
        @fTradedIV  float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime = Null out,
	@tiStatus tinyint = Null
  as
	set nocount on

	begin tran
	
	Declare @iNewSeqNum int
	
	select @iNewTradeID = max(tradeID) + 1 from Trade
		if @iNewTradeID is null set @iNewTradeID = 1

	select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade


	if @dtNewTradeDate is null	
	begin
		declare @iDeltaGMT int
		exec master.dbo.xp_regread 'HKEY_LOCAL_MACHINE', 'SYSTEM\CurrentControlSet\Control\TimeZoneInformation', 'Bias', @iDeltaGMT OUT

		set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	end

	insert into Trade (
		tradeID,
		actionID, 
		contractID,
		isClose, 
		tradeDate, 
		quantity, 
		price, 
		bookID, 
		strategyID, 
		traderID, 
		brokerID, 
		clearingBrokerID, 
		isBuy, 
		brokerCommissionID, 
		clearingBrokerCommissionID,
		tradedIV,
		spotReference,
		isPosition,
		status,
		seqNum)
	values (
		@iNewTradeID,
		1,
		@iContractID, 
		@tiIsClose, 
		@dtNewTradeDate, 
		@iQuantity, 
		@fPrice, 
		@iBookID, 
		@iStrategyID, 
		@iTraderID, 
		@iBrokerID,
		@iClearingBrokerID,
		@tiIsBuy, 
		@iBrokerCommissionID,
		@iClearingBrokerCommissionID,
		@fTradedIV,
		@fSpotReference,
		@tiIsPosition,
		@tiStatus,
		@iNewSeqNum)
	if @@error = 0
	begin
		--set @iNewSeqNum = @@identity 
		commit tran
		return @iNewSeqNum
	end
	else  
	begin
		rollback tran
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		return 0
	end
/*
   if not exists(select * from Trade where tradeID = @iTradeID)  
     begin
         insert into Trade (
				contractID,
				seqNum,
				publisherID, 
				isClose, 
				tradeDate, 
				quantity, 
				price, 
				bookID, 
				strategyID, 
				traderID, 
				brokerID, 
				clearingBrokerID, 
				isBuy, 
				brokerCommission, 
				clearingBrokerCommission,
				tradedIV,
				spotReference)
                values (
				@iContractID, 
				@iSeqNum,
				@vcPublisherID,
				@iIsClose, 
				@dtTradeDate, 
				@iquantity, 
				@fPrice, 
				@iBookID, 
				@iStrategyID, 
				@iTraderID, 
				@iBrokerID,
				@iClearingBrokerID,
				@iIsBuy, 
				@fBrokerCommission,
				@fClearingBrokerCommission,
				@fTradedIV,
				@fSpotReference)
         if (@@error <> 0)
            return (-1)
          else  
            return @@identity
     end
   else 
     begin
        update Trade 
             set contractID = isnull(@iContractID,contractID),
				seqNum = isnull(@iSeqNum,seqNum),
				publisherID = isnull(@vcPublisherID,publisherID),
				isClose = isnull(@iIsClose,isClose),
				tradeDate = isnull(@dtTradeDate,tradeDate),
				quantity = isnull(@iquantity,quantity),
				price = isnull(@fPrice,price),
				bookID = isnull(@iBookID,bookID),
				strategyID = isnull(@iStrategyID,strategyID),
				traderID = isnull(@iTraderID,traderID),
				brokerID = isnull(@iBrokerID,brokerID),
				clearingBrokerID = isnull(@iClearingBrokerID,clearingBrokerID),
				isBuy = isnull(@iIsBuy,isBuy),
				brokerCommission = isnull(@fBrokerCommission,brokerCommission),
				ClearingBrokerCommission = isnull(@fClearingBrokerCommission,ClearingBrokerCommission),
				tradedIV = isnull(@fTradedIV,tradedIV),
				spotReference = isnull(@fSpotReference,spotReference)
          where tradeID = @iTradeID
        if (@@error <> 0)
           return (-1)

         else  
           return 0
     end
*/

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO





CREATE procedure dbo.usp_Trade_Update
	@iOldTradeID int,
	@iContractID int = Null,
	@tiIsClose tinyint = Null,
	@iQuantity int = Null,
	@fPrice float = Null,
	@tiIsBuy tinyint = Null,  
	@iBookID int = Null,
	@iStrategyID int = Null,
	@iTraderID int = Null,
	@iBrokerID int = Null,
	@iClearingBrokerID int = Null,
	@iBrokerCommissionID float =Null,
	@iClearingBrokerCommissionID float = Null,
	@fTradedIV  float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime out,
--	@iLotSize int = Null,
	@tiStatus tinyint = null
  as
	begin tran

	declare @bSucceed bit,
			@iNewSeqNum int

	set @bSucceed = 0

	select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade

	if @iOldTradeID is not null

	begin
		declare @nCount int
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iOldTradeID
		
		if @nCount = 1
		begin
			insert into Trade(tradeID,
				actionID,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				status,
				execID,
				SeqNum)
			select tradeID,
				0,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition,
				status,
				execID,
				@iNewSeqNum
			from Trade
			where tradeID = @iOldTradeID
		
			if @@error = 0 set @bSucceed = 1
		end
	end
	if @bSucceed = 1
	begin
		select @iNewTradeID = max(tradeID) + 1 from Trade
		if @iNewTradeID is null set @iNewTradeID = 1

		if @dtNewTradeDate is null	
		begin
			declare @iDeltaGMT int
			exec master.dbo.xp_regread 'HKEY_LOCAL_MACHINE', 'SYSTEM\CurrentControlSet\Control\TimeZoneInformation', 'Bias', @iDeltaGMT OUT
	
			set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
		end
	
		select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade
	
		insert into Trade (
			tradeID,
			actionID, 
			contractID,
			isClose, 
			tradeDate, 
			quantity, 
			price, 
			bookID, 
			strategyID, 
			traderID, 
			brokerID, 
			clearingBrokerID, 
			isBuy, 
			brokerCommissionID, 
			clearingBrokerCommissionID,
			tradedIV,
			spotReference,
			isPosition,
			status,
			seqNum)
		values (
			@iNewTradeID,
			1,
			@iContractID, 
			@tiIsClose, 
			@dtNewTradeDate, 
			@iQuantity, 
			@fPrice, 
			@iBookID, 
			@iStrategyID, 
			@iTraderID, 
			@iBrokerID,
			@iClearingBrokerID,
			@tiIsBuy, 
			@iBrokerCommissionID,
			@iClearingBrokerCommissionID,
			@fTradedIV,
			@fSpotReference,
			@tiIsPosition,
			@tiStatus,
			@iNewSeqNum)
		if @@error = 0
			set @bSucceed = 1
		else
			set @bSucceed = 0
	end
	if @bSucceed = 1
	begin
--		set @iNewSeqNum = @@identity
		commit tran
		return @iNewSeqNum
	end
	else  
	begin
		rollback tran
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		return 0
	end

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE procedure dbo.usp_UnderlyingByGroup_Get 
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int,
	@tiWithPosition tinyint = 0
AS
	set nocount on

	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and (@iContractID is null or c.contractID = @iContractID)
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and (@iContractID is null or c.contractID = @iContractID)
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			order by c.symbol	 
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity  * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
				s.isActive, s.skew, s.kurt
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,

			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose,
				s.isActive, s.skew, s.kurt
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt
			order by c.symbol	 
	end

	Return (0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE   proc dbo.usp_UnderlyingOptions_Get
	@iUnderlyingID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0,
	@dtMinExpiry datetime = Null
AS
	set nocount on
	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					ort.lotSize as iLotSize
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					ort.lotSize as iLotSize
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
					o.isCall as bIsCall,
					ort.lotSize as iLotSize
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
					o.isCall as bIsCall,
					ort.lotSize as iLotSize
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
					o.isCall as bIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
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
					o.strike, ec.expiryDate, o.isCall, ort.lotSize
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
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
					o.strike, ec.expiryDate, o.isCall, ort.lotSize
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
					o.isCall as bIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
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
					o.strike, ec.expiryDate, o.isCall, ort.lotSize
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
					join OptionRootView ort on cig.contractID = ort.underlyingID					
					inner join ContractView uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) = 0
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, ec.expiryDate, o.isCall, ort.lotSize
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
	end

Return (0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   procedure dbo.usp_UnderlyingPosByList_Get
	@iTraderID int = null,
	@tiWithSimulated tinyint = 0,
	@vcUnderlyingID1 varchar(8000),
	@vcUnderlyingID2 varchar(8000) = null
AS
	set nocount on

	declare @vcString1 as varchar(8000),
			@vcString2 as varchar(8000),
			@vcString3 as varchar(8000),
			@vcString4 as varchar(8000)

	if isnull(@iTraderID, 0) = 0
		begin
			set @vcString1 = 'select 
						c.contractID iContractID,
						sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) iQty,
						sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) fPosition
						from TradeView tr 
							join ContractView c on tr.contractID=c.contractID '
	
			if isnull(@tiWithSimulated,0)= 0 
				begin
					select @vcString1 = @vcString1 + ' and isnull(tr.status,0)!=1 '
				end
	
	        set @vcString2 = 'where c.contractTypeID < 3 '
			set @vcString3 = 'and c.contractID in (' 
			set @vcString4 = 'group by c.contractID'
		end
	else
		begin
			set @vcString1 = 'select
				c.contractID iContractID,
				sum(tr.quantity * (case when tr.isBuy!=0 then 1 else -1 end)) iQty,
				sum(tr.quantity * tr.price * (case when tr.isBuy!=0 then 1 else -1 end)) fPosition
					from ContractView c
			    		join TraderContractView tc on c.contractID=tc.contractID
						join TradeView tr on tr.contractID=c.contractID ' 

			set @vcString2 = 'and tr.traderID=' + convert(varchar(25),@iTraderID) + ' ' 

			if isnull(@tiWithSimulated,0)= 0 
				begin
					select @vcString2 = isnull(@vcString2,'') + ' and isnull(tr.status,0)!=1 '
				end

			set @vcString3 = 'where c.contractTypeID < 3 and tc.traderID=' + convert(varchar(25),@iTraderID) + ' and c.contractID in ('
			set @vcString4 = 'group by c.contractID'
		end

		if isnull(@vcUnderlyingID1,'') != '' and isnull(@vcUnderlyingID2,'') = ''
			begin
				exec(@vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ') ' + @vcString4)
				--select @vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ') ' + @vcString4
			end
		else if isnull(@vcUnderlyingID1,'') != '' and isnull(@vcUnderlyingID2,'') != ''
			begin
				exec(@vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ',' + @vcUnderlyingID2 + ') ' + @vcString4)
				--select @vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ',' + @vcUnderlyingID2 + ') ' + @vcString4
			end			

	Return (@@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE   procedure dbo.usp_UnderlyingPos_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithSimulated tinyint = 0
AS
	set nocount on

	if isnull(@iGroupID, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractView c 
				left join TradeView tr on tr.contractID = c.contractID 
							and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.status,0)!=1)
			where
				c.contractTypeID in (1, 2)
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID
		else
			select
			    c.contractID as iContractID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
				left join TradeView tr on tr.contractID = c.contractID 
							and tr.traderID = @iTraderID 
							and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.status,0)!=1)
			where 
				c.contractTypeID in (1, 2)
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID
	end
	else
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
				left join TradeView tr on tr.contractID = c.contractID 
							and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.status,0)!=1)
			where 
				cig.groupID = @iGroupID
				and c.contractTypeID in (1, 2)
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID
		else
			select
			    c.contractID as iContractID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
				left join TradeView tr on tr.contractID = c.contractID 
							and tr.traderID = @iTraderID 
							and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.status, 0)!=1)
			where cig.groupID = @iGroupID
				and c.contractTypeID in (1, 2)
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID
	end

	Return (0)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE    procedure dbo.usp_Underlying_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0
AS
	set nocount on

	if isnull(@iGroupID, 0) <> 0
	begin
		exec usp_UnderlyingByGroup_Get @iContractID,@iTraderID,@iGroupID,@tiWithPosition	
		Return(0)
	end 

	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractView c 
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
				and (@iContractID is null or c.contractID = @iContractID)
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractView c 
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
				and (@iContractID is null or c.contractID = @iContractID)
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractView c 


			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			order by c.symbol	 
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractView c 
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
			where
				c.contractTypeID = 2
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
				s.isActive, s.skew, s.kurt
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractView c 
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
			where
				c.contractTypeID = 1
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,



				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
			where 
				c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			   	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
				s.isActive, s.skew, s.kurt
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
			where
				c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt
			order by c.symbol	 
	end

	Return (0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDefinitionBeta_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDefinitionBeta_Clear]
GO

/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.40.059')
	begin
		update DataInfo set keyValue = '3.40.059' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.40.059' '3.40.059 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO