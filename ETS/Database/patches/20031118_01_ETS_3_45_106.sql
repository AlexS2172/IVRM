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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.45.105' or (keyValue = '3.45.106')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.45.106 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.45.106' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

----------------------------------------------------------------------------------------------------
-- Add FutureRoot
----------------------------------------------------------------------------------------------------
if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FutureRoot]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
begin
	CREATE TABLE [dbo].[FutureRoot]
	(
		[futureRootID] [int] IDENTITY (1, 1) NOT NULL ,
		[underlyingID] [int] NOT NULL ,
		[futureRootSymbol] [varchar] (20) NOT NULL ,
		[futureRootName] [varchar] (255) NULL ,
		[futureLotSize] [int] NOT NULL ,
		[optionLotSize] [int] NOT NULL ,
		[exportSymbol] [varchar] (20) NULL ,
		[matCalendarID] [int] NOT NULL ,
		[actionDate] [datetime] NOT NULL CONSTRAINT [DF_FutureRoot_actionDate] DEFAULT (getdate()),
		CONSTRAINT [PK_FutureRoot] PRIMARY KEY CLUSTERED 
			(
				[futureRootID]
			),
		CONSTRAINT [FK_FutureRoot_Contract] FOREIGN KEY 
			(
				[underlyingID]
			)
			REFERENCES [Contract] 
			(
				[contractID]
			),
		CONSTRAINT [FK_FutureRoot_MatCalendarID] FOREIGN KEY 
			(
				[matCalendarID]
			)
			REFERENCES [ExpiryCalendar] 
			(
				[expCalendarID]
			)
	)
end
GO

if not exists(select * from sysindexes where id=object_id('FutureRoot') and name='IX_FutureRoot_UnderlyingID')
	CREATE INDEX [IX_FutureRoot_UnderlyingID] ON [dbo].[FutureRoot]([underlyingID])
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_FutureRoot]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
	drop trigger [dbo].[tU_FutureRoot]
GO

CREATE TRIGGER dbo.tU_FutureRoot ON [dbo].[FutureRoot]
    FOR Update
AS
	set nocount on
	
	update FutureRoot
		set actionDate = getdate()
	from FutureRoot fr 
		join inserted on fr.futureRootID = inserted.futureRootID

GO
	
----------------------------------------------------------------------------------------------------
-- Add Future
----------------------------------------------------------------------------------------------------
if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Future]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
begin
	CREATE TABLE [dbo].[Future]
	(
		[futureID] [int] NOT NULL ,
		[futureRootID] [int] NOT NULL ,
		[maturityID] [int] NOT NULL ,
		[calcOptionType] [tinyint] NULL CONSTRAINT [DF_Future_calcOptionType] DEFAULT (1),
		[isActive] [tinyint] NULL CONSTRAINT [DF_Future_isActive] DEFAULT (1),
		[actionDate] [datetime] NOT NULL CONSTRAINT [DF_Future_actionDate] DEFAULT (getdate()),
		CONSTRAINT [PK_Future] PRIMARY KEY CLUSTERED 
			(
				[futureID]
			),
		CONSTRAINT [FK_Future_Contract] FOREIGN KEY 
			(
				[futureID]
			)
			REFERENCES [Contract] 
			(
				[contractID]
			),
		CONSTRAINT [FK_Future_FutureRoot] FOREIGN KEY 
			(
				[futureRootID]
			)
			REFERENCES [FutureRoot] 
			(
				[futureRootID]
			),
		CONSTRAINT [FK_Future_Maturity] FOREIGN KEY 
			(
				[maturityID]
			)
			REFERENCES [Expiry] 
			(
				[expiryID]
			)
	)
end
GO

if not exists(select * from sysindexes where id=object_id('Future') and name='IX_Future_FutureRootID')
	CREATE INDEX [IX_Future_FutureRootID] ON [dbo].[Future]([futureRootID])
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_Future]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
	drop trigger [dbo].[tU_Future]
GO

CREATE TRIGGER dbo.tU_Future ON [dbo].[Future]
    FOR Update
AS
	set nocount on
	
	update Future
		set actionDate = getdate()
	from Future f 
		join inserted on f.futureID = inserted.futureID

GO

----------------------------------------------------------------------------------------------------
-- Add FutureOption
----------------------------------------------------------------------------------------------------
if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FutureOption]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
begin
	CREATE TABLE [dbo].[FutureOption]
	(
		[futureOptionID] [int] NOT NULL ,
		[futureID] [int] NOT NULL ,
		[isCall] [tinyint] NOT NULL ,
		[expiryID] [int] NOT NULL ,
		[strike] [float] NOT NULL ,
		[actionDate] [datetime] NOT NULL CONSTRAINT [DF_FutureOption_actionDate] DEFAULT (getdate()),
		CONSTRAINT [PK_FutureOption] PRIMARY KEY CLUSTERED 
			(
				[futureOptionID]
			),
		CONSTRAINT [FK_FutureOption_Contract] FOREIGN KEY 
			(
				[futureOptionID]
			)
			REFERENCES [Contract] 
			(
				[contractID]
			),
		CONSTRAINT [FK_FutureOption_Future] FOREIGN KEY 
			(
				[futureID]
			)
			REFERENCES [Future] 
			(
				[futureID]
			),
		CONSTRAINT [FK_FutureOption_Expiry] FOREIGN KEY 
			(
				[expiryID]
			)
			REFERENCES [Expiry] 
			(
				[expiryID]
			)
	)
end
GO

if not exists(select * from sysindexes where id=object_id('FutureOption') and name='IX_FutureOption_FutureID')
	CREATE INDEX [IX_FutureOption_FutureID] ON [dbo].[FutureOption]([futureID])
GO

if not exists(select * from sysindexes where id=object_id('FutureOption') and name='IX_FutureOption_FutureOptionID_FutureID')
	CREATE UNIQUE INDEX [IX_FutureOption_FutureOptionID_FutureID] ON [dbo].[FutureOption]([futureOptionID], [futureID])
GO

if not exists(select * from sysindexes where id=object_id('FutureOption') and name='IX_FutureOption_IsCall')
	CREATE INDEX [IX_FutureOption_IsCall] ON [dbo].[FutureOption]([isCall])
GO

if not exists(select * from sysindexes where id=object_id('FutureOption') and name='IX_FutureOption_Option')
	CREATE INDEX [IX_FutureOption_Option] ON [dbo].[FutureOption]([futureID], [isCall], [expiryID], [strike])
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_FutureOption]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
	drop trigger [dbo].[tU_FutureOption]
GO

CREATE TRIGGER dbo.tU_FutureOption ON [dbo].[FutureOption]
    FOR Update
AS
	set nocount on
	
	update FutureOption
		set actionDate = getdate()
	from FutureOption fo 
		join inserted on fo.futureOptionID = inserted.futureOptionID

GO

----------------------------------------------------------------------------------------------------
-- add new helper columns to Trade table
----------------------------------------------------------------------------------------------------
-- underlying
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='underlyingID')
	ALTER TABLE [dbo].[Trade] ADD [underlyingID] int null
GO

if (select object_id('FK_Trade_UnderlyingID')) is null
	ALTER TABLE [dbo].[Trade] ADD CONSTRAINT [FK_Trade_UnderlyingID]
	FOREIGN KEY ([underlyingID]) REFERENCES [dbo].[Contract] ([contractID]) 
GO

-- option root
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='optionRootID')
	ALTER TABLE [dbo].[Trade] ADD [optionRootID] int null
GO

if (select object_id('FK_Trade_OptionRootID')) is null
	ALTER TABLE [dbo].[Trade] ADD CONSTRAINT [FK_Trade_OptionRootID]
	FOREIGN KEY ([optionRootID]) REFERENCES [dbo].[OptionRoot] ([optionRootID]) 
GO

-- future root
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='futureRootID')
	ALTER TABLE [dbo].[Trade] ADD [futureRootID] int null
GO

if (select object_id('FK_Trade_FutureRootID')) is null
	ALTER TABLE [dbo].[Trade] ADD CONSTRAINT [FK_Trade_FutureRootID]
	FOREIGN KEY ([futureRootID]) REFERENCES [dbo].[FutureRoot] ([futureRootID]) 
GO

-- future
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='futureID')
	ALTER TABLE [dbo].[Trade] ADD [futureID] int null
GO

if (select object_id('FK_Trade_FutureID')) is null
	ALTER TABLE [dbo].[Trade] ADD CONSTRAINT [FK_Trade_FutureID]
	FOREIGN KEY ([futureID]) REFERENCES [dbo].[Contract] ([contractID]) 
GO

-- update new columns in existing trades
if exists (select 1 from Trade)
begin
	-- update underlyings data
	update Trade
	set underlyingID = tr.contractID,
		optionRootID = null,
		futureRootID = null,
		futureID = null
	from Trade tr
		inner join ContractView c on tr.contractID = c.contractID
	where c.contractTypeID < 3

	-- update options data
	update Trade
	set underlyingID = uc.contractID,
		optionRootID = ort.optionRootID,
		futureRootID = null,
		futureID = null
	from Trade tr
		inner join ContractView c on tr.contractID = c.contractID
		inner join OptionView o on tr.contractID = o.optionID
		inner join OptionRoot ort on o.optionRootID = ort.optionRootID
		inner join Contract uc on ort.underlyingID = uc.contractID
	where c.contractTypeID = 3

	-- update futures data
	update Trade
	set underlyingID = uc.contractID,
		optionRootID = null,
		futureRootID = fr.futureRootID,
		futureID = tr.contractID
	from Trade tr
		inner join ContractView c on tr.contractID = c.contractID
		inner join Future f on tr.contractID = f.futureID
		inner join FutureRoot fr on f.futureRootID = fr.futureRootID
		inner join Contract uc on fr.underlyingID = uc.contractID
	where c.contractTypeID = 4

	-- update futures options data
	update Trade
	set underlyingID = uc.contractID,
		optionRootID = null,
		futureRootID = fr.futureRootID,
		futureID = f.futureID
	from Trade tr
		inner join ContractView c on tr.contractID = c.contractID
		inner join FutureOption fo on tr.contractID = fo.futureOptionID
		inner join Future f on fo.futureID = f.futureID
		inner join FutureRoot fr on f.futureRootID = fr.futureRootID
		inner join Contract uc on fr.underlyingID = uc.contractID
	where c.contractTypeID = 5
end
GO

-- underlying ID can't be null
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trade' and COLUMN_NAME='underlyingID')
	ALTER TABLE [dbo].[Trade] ALTER COLUMN [underlyingID] int not null
GO

-- add indices to Trade
if not exists(select * from sysindexes where id=object_id('Trade') and name='IX_Trade_UnderlyingID')
	CREATE INDEX [IX_Trade_UnderlyingID] ON [dbo].[Trade]([underlyingID])
GO

if not exists(select * from sysindexes where id=object_id('Trade') and name='IX_Trade_FutureID')
	CREATE INDEX [IX_Trade_FutureID] ON [dbo].[Trade]([futureID])
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
	 	tradeID, contractID, tradeDate, quantity, price, isBuy, 
		traderID, strategyID, brokerID, brokerCommission, 
		clearingBrokerID, clearingBrokerCommission, tradedIV, 
		spotReference, isPosition, status, execID, mark,
		underlyingID, optionRootID, futureRootID, futureID
	from 
		dbo.Trade
	group by
	 	tradeID, contractID, tradeDate, quantity, price, isBuy, 
		traderID, strategyID, brokerID, brokerCommission, 
		clearingBrokerID, clearingBrokerCommission, tradedIV, 
		spotReference, isPosition, status, execID, mark,
		underlyingID, optionRootID, futureRootID, futureID
	having
		(min(actionID) = 1)

GO

----------------------------------------------------------------------------------------------------
-- TradeLog modifications
----------------------------------------------------------------------------------------------------

TRUNCATE TABLE TradeLog
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TradeLog]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	DROP TABLE [dbo].[TradeLog]
GO

if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TradeLog]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
begin
	CREATE TABLE [dbo].[TradeLog]
	(
		[underlyingID] [int] NOT NULL ,
		[eventTypeID] [int] NULL ,
		CONSTRAINT [PK_TradeLog] PRIMARY KEY CLUSTERED 
			(
				[underlyingID]
			),
		CONSTRAINT [FK_TradeLog_Contract] FOREIGN KEY 
			(
				[underlyingID]
			)
			REFERENCES [Contract] 
			(
				[contractID]
			)
	)
end
GO

----------------------------------------------------------------------------------------------------
-- add additional columns to TradeHistory
----------------------------------------------------------------------------------------------------
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='TradeHistory' and COLUMN_NAME='futureRootID')
	ALTER TABLE [dbo].[TradeHistory] ADD [futureRootID] int null
GO

if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='TradeHistory' and COLUMN_NAME='futureOptionLotSize')
	ALTER TABLE [dbo].[TradeHistory] ADD [futureOptionLotSize] int null
GO

-- correct traderName 'not null' constraint
if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='TradeHistory' and COLUMN_NAME='traderName')
	ALTER TABLE [dbo].[TradeHistory] ALTER COLUMN [traderName] varchar(120) null
GO

----------------------------------------------------------------------------------------------------
-- usp_BoFutureRoot_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureRoot_Get]
GO

CREATE PROC dbo.usp_BoFutureRoot_Get
	@iUnderlyingID int = null
AS
	-- retrieve future root(s) by underlying

	set nocount on

	if @iUnderlyingID is not null
		select
			fr.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.symbol as vcUnderlyingSymbol,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.futureRootName as vcFutureRootName,
			fr.futureLotSize as iFutureLotSize,
			fr.optionLotSize as iOptionLotSize,
			fr.exportSymbol as vcExportSymbol,
			fr.matCalendarID as iMatCalendarID,
			fr.actionDate as dtActionDate
		from FutureRoot fr
			inner join ContractView uc on fr.underlyingID = uc.contractID
		where
			fr.underlyingID = @iUnderlyingID
		order by
			fr.futureRootSymbol
	else
		select
			fr.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.symbol as vcUnderlyingSymbol,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.futureRootName as vcFutureRootName,
			fr.futureLotSize as iFutureLotSize,
			fr.optionLotSize as iOptionLotSize,
			fr.exportSymbol as vcExportSymbol,
			fr.matCalendarID as iMatCalendarID,
			fr.actionDate as dtActionDate
		from FutureRoot fr
			inner join ContractView uc on fr.underlyingID = uc.contractID
		order by
			uc.symbol, fr.futureRootSymbol

GO

----------------------------------------------------------------------------------------------------
-- usp_BoFutureRoot_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureRoot_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureRoot_Del]
GO

CREATE PROC dbo.usp_BoFutureRoot_Del
	@iFutureRootID int,
	@tiIsRaiseError tinyint = 1 
AS
	-- delete future root (with all futures and futures options)

	set nocount on
	
	declare @error int		set @error = 0

	-- check future root ID
	if @iFutureRootID is null
	begin
		if @tiIsRaiseError = 1 raiserror('Future root to delete is not specified!', 16, 1)
		set @error = -1
		goto finish
	end

	-- create temporary table with contracts to delete
	create table #FutureRootsContractsForDelete(contractID int, contractTypeID int)
	
	insert into #FutureRootsContractsForDelete
		select futureID, 4 from Future
			where futureRootID = @iFutureRootID
		union select fo.futureOptionID, 5 from FutureOption fo
			inner join Future f on fo.futureID = f.futureID
			where f.futureRootID = @iFutureRootID

	-- check for trades existence
	if exists(select 1 from Trade where contractID in (select contractID from #FutureRootsContractsForDelete))
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future root, because its futures or its future options has trades or position!', 16, 1)
		set @error = -2
		goto finish
	end

	-- check for orders existence
	if exists(select 1 from [Order] where contractID in (select contractID from #FutureRootsContractsForDelete))
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future root, because its futures or its future options has orders!', 16, 1)
		set @error = -3
		goto finish
	end

	begin tran

	-- delete future options data
	delete FutureOption 
	where futureID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 5)
	if @@error <> 0 begin set @error = -4 if @@trancount = 1 rollback tran goto finish end

	-- delete future data
	delete Future
	where futureRootID = @iFutureRootID
	if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

	-- delete future root
	delete FutureRoot
	where futureRootID = @iFutureRootID
	if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end

	-- delete common future and future options contracts data
	delete ContractPrice 
	where contractID in (select contractID from #FutureRootsContractsForDelete)
	if @@error <> 0 begin set @error = -7 if @@trancount = 1 rollback tran goto finish end

	update ContractExchangeLink set actionID = 3
	where contractID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 4)
	if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end

	update Contract set actionID = 3
	where contractID in (select contractID from #FutureRootsContractsForDelete)
	if @@error <> 0 begin set @error = -9 if @@trancount = 1 rollback tran goto finish end

	commit tran

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoFutureRootByUnderlying_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureRootByUnderlying_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureRootByUnderlying_Del]
GO

CREATE PROC dbo.usp_BoFutureRootByUnderlying_Del
	@iUnderlyingID int,
	@tiIsRaiseError tinyint = 1 
AS
	-- delete future roots by underlying (with all futures and futures options)

	set nocount on
	
	declare @error int		set @error = 0

	-- check underlying ID
	if @iUnderlyingID is null
	begin
		if @tiIsRaiseError = 1 raiserror('Underlying for future roots to delete is not specified!', 16, 1)
		set @error = -1
		goto finish
	end

	-- create temporary table with contracts to delete
	create table #FutureRootsContractsForDelete(contractID int, contractTypeID int)
	
	insert into #FutureRootsContractsForDelete
		select 
			f.futureID, 4
		from FutureRoot fr
			inner join Future f on fr.futureRootID = f.futureRootID
		where fr.underlyingID = @iUnderlyingID
		union select 
			fo.futureOptionID, 5 
		from FutureRoot fr
			inner join Future f on fr.futureRootID = f.futureRootID
			inner join FutureOption fo on fo.futureID = f.futureID
		where fr.underlyingID = @iUnderlyingID

	-- check for trades existence
	if exists(select 1 from Trade where contractID in (select contractID from #FutureRootsContractsForDelete))
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future roots, because its futures or its future options has trades or position!', 16, 1)
		set @error = -2
		goto finish
	end

	-- check for orders existence
	if exists(select 1 from [Order] where contractID in (select contractID from #FutureRootsContractsForDelete))
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future roots, because its futures or its future options has orders!', 16, 1)
		set @error = -3
		goto finish
	end

	begin tran

	-- delete future options data
	delete FutureOption 
	where futureID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 5)
	if @@error <> 0 begin set @error = -4 if @@trancount = 1 rollback tran goto finish end

	-- delete future data
	delete Future
	where futureID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 4)
	if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

	-- delete future root
	delete FutureRoot
	where underlyingID = @iUnderlyingID
	if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end

	-- delete common future and future options contracts data
	delete ContractPrice 
	where contractID in (select contractID from #FutureRootsContractsForDelete)
	if @@error <> 0 begin set @error = -7 if @@trancount = 1 rollback tran goto finish end

	update ContractExchangeLink set actionID = 3
	where contractID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 4)
	if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end

	update Contract set actionID = 3
	where contractID in (select contractID from #FutureRootsContractsForDelete)
	if @@error <> 0 begin set @error = -9 if @@trancount = 1 rollback tran goto finish end

	commit tran

finish:
	return @error
GO
----------------------------------------------------------------------------------------------------
-- usp_BoFutureRoot_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureRoot_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureRoot_Save]
GO

CREATE PROC dbo.usp_BoFutureRoot_Save
	@iFutureRootID int out,
	@iUnderlyingID int, -- should be specified for new future root (ignored for update)
	@vcFutureRootSymbol varchar(20) = null,
	@vcFutureRootName varchar(255) = null,
	@iFutureLotSize int = null,
	@iOptionLotSize int = null,
	@vcExportSymbol varchar(20) = null,
	@iMatCalendarID int = null,
	@tiIsRaiseError tinyint = 1 
AS
	-- create/update future root

	set nocount on
	
	declare @error int		set @error = 0

	if @iFutureRootID is null -- new future root
	begin
		-- check underlying ID
		if @iUnderlyingID is null
		begin
			if @tiIsRaiseError = 1 raiserror('Underlying for new future root is not specified!', 16, 1)
			set @error = -1
			goto finish
		end

		-- check future root symbol
		set @vcFutureRootSymbol = isnull(@vcFutureRootSymbol, '')
		if len(ltrim(@vcFutureRootSymbol)) = 0
		begin
			if @tiIsRaiseError = 1 raiserror('Symbol for new future root is not specified!', 16, 1)
			set @error = -2
			goto finish
		end

		-- check future root
		if exists(select 1 from FutureRoot where futureRootSymbol = @vcFutureRootSymbol)
		begin
			if @tiIsRaiseError = 1 raiserror('Future root with symbol ''%s'' is already exists!', 16, 1, @vcFutureRootSymbol)
			set @error = -3
			goto finish
		end

		-- correct lot sizes
		if isnull(@iFutureLotSize, 0) <= 0
			set @iFutureLotSize = 100

		if isnull(@iOptionLotSize, 0) <= 0
			set @iOptionLotSize = 1

		-- correct maturity calendar ID
		if isnull(@iMatCalendarID, 0) = 0
			set @iMatCalendarID = 1

		-- check maturity calendar ID
		if not exists(select 1 from ExpiryCalendar where expCalendarID = @iMatCalendarID)
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future root maturity calendar specified!', 16, 1)
			set @error = -4
			goto finish
		end

		begin tran

		-- insert new future root
		insert into FutureRoot
			(underlyingID, futureRootSymbol, futureRootName,
			futureLotSize, optionLotSize, exportSymbol, matCalendarID)
		values
			(@iUnderlyingID, @vcFutureRootSymbol, @vcFutureRootName,
			@iFutureLotSize, @iOptionLotSize, @vcExportSymbol, @iMatCalendarID)
		if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

		set @iFutureRootID = @@identity

		commit tran
	end
	else
	begin

		-- check future root
		if exists(select 1 from FutureRoot where futureRootID <> @iFutureRootID and futureRootSymbol = @vcFutureRootSymbol)
		begin
			if @tiIsRaiseError = 1 raiserror('Future root with symbol ''%s'' is already exists!', 16, 1, @vcFutureRootSymbol)
			set @error = -6
			goto finish
		end

		-- correct future root name (prevent updates to empty string)
		if len(ltrim(@vcFutureRootSymbol)) = 0
			set @vcFutureRootSymbol = null

		-- correct future lot size (prevent updates to invalid lot size)
		if @iFutureLotSize <= 0
			set @iFutureLotSize = null

		-- correct option lot size (prevent updates to invalid lot size)
		if @iOptionLotSize <= 0
			set @iOptionLotSize = null

		-- correct maturity calendar ID (prevent updates to zero calendar)
		if @iMatCalendarID = 0
			set @iMatCalendarID = null

		-- check maturity calendar
		if @iMatCalendarID is not null
		begin
			if not exists(select 1 from ExpiryCalendar where expCalendarID = @iMatCalendarID)
			begin
				if @tiIsRaiseError = 1 raiserror('Invalid future root maturity calendar specified!', 16, 1)
				set @error = -7
				goto finish
			end
		end

		begin tran

		-- update future root
		update FutureRoot
		set futureRootSymbol = isnull(@vcFutureRootSymbol, futureRootSymbol),
			futureRootName = isnull(@vcFutureRootName, futureRootName),
			futureLotSize = isnull(@iFutureLotSize, futureLotSize),
			optionLotSize = isnull(@iOptionLotSize, optionLotSize),
			exportSymbol = isnull(@vcExportSymbol, exportSymbol),
			matCalendarID = isnull(@iMatCalendarID, matCalendarID)
		where
			futureRootID = @iFutureRootID
		if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end

		commit tran
	end

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoFuture_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFuture_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFuture_Get]
GO

CREATE PROC dbo.usp_BoFuture_Get
	@iFutureID int = null
AS
	-- retrieve future

	set nocount on

	if @iFutureID is not null
		select
			f.futureID as iFutureID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcFutureSymbol,
			fc.contractName as vcFutureName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			f.maturityID as iMaturityID,
			frmc.expiryDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			f.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
			inner join ExpiryInCalendarView frmc on f.maturityID = frmc.expiryID and fr.matCalendarID = frmc.expCalendarID
		where
			f.futureID = @iFutureID
	else
		select
			f.futureID as iFutureID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcFutureSymbol,
			fc.contractName as vcFutureName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			f.maturityID as iMaturityID,
			frmc.expiryDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			f.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
			inner join ExpiryInCalendarView frmc on f.maturityID = frmc.expiryID and fr.matCalendarID = frmc.expCalendarID
		order by
			fc.symbol

GO

----------------------------------------------------------------------------------------------------
-- usp_BoFuture_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFuture_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFuture_Del]
GO

CREATE PROC dbo.usp_BoFuture_Del
	@iFutureID int,
	@tiIsRaiseError tinyint = 1 
AS
	-- delete future (with all future options)

	set nocount on
	
	declare @error int		set @error = 0

	-- check future ID
	if @iFutureID is null
	begin
		if @tiIsRaiseError = 1 raiserror('Future to delete is not specified!', 16, 1)
		set @error = -1
		goto finish
	end

	-- create temporary table with contracts to delete
	create table #FutureContractsForDelete(contractID int)

	insert into #FutureContractsForDelete
		select @iFutureID
		union select futureOptionID
		from FutureOption where futureID = @iFutureID


	-- check for trades existence
	if exists(select 1 from Trade where contractID in (select contractID from #FutureContractsForDelete))
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future, because it (or its options) has trades!', 16, 1)
		set @error = -2
		goto finish
	end

	-- check for orders existence
	if exists(select 1 from [Order] where contractID in (select contractID from #FutureContractsForDelete))
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future, because it (or its options) has orders!', 16, 1)
		set @error = -3
		goto finish
	end

	begin tran
	
	-- delete future options
	delete FutureOption
	where futureID = @iFutureID
	if @@error <> 0 begin set @error = -4 if @@trancount = 1 rollback tran goto finish end

	-- delete future
	delete Future where
	futureID = @iFutureID
	if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

	-- delete common future and future options
	delete ContractPrice
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end

	update ContractExchangeLink set actionID = 3
	where contractID = @iFutureID
	if @@error <> 0 begin set @error = -7 if @@trancount = 1 rollback tran goto finish end

	update Contract set actionID = 3
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end

	commit tran

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoFuture_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFuture_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFuture_Save]
GO

CREATE PROC dbo.usp_BoFuture_Save
	@iFutureID int out,
	@iFutureRootID int, -- should be specified for new future (ignored for update)
	@vcFutureSymbol varchar(20) = null,
	@vcFutureName varchar(255) = null,
	@vcExportSymbol varchar(20) = null,
	@iMaturityID int = null, -- calculated by @dtMaturityDate if not specified
	@dtMaturityDate datetime = null, -- ignored if @iMaturityID is specified
	@iExpCalendarID int = null,
	@tiCalcOptionType tinyint = null,
	@tiIsActive tinyint = null,
	@iUndPriceProfileID int = null out,
	@iOptPriceProfileID int = null out,
	@tiIsRaiseError tinyint = 1 
AS
	-- create/update future

	set nocount on
	
	declare @error int		set @error = 0

	if @iFutureID is null -- new future
	begin
		-- check new future root ID
		if @iFutureRootID is null
		begin
			if @tiIsRaiseError = 1 raiserror('Future root for new future is not specified!', 16, 1)
			set @error = -1
			goto finish
		end

		-- check future symbol
		set @vcFutureSymbol = isnull(@vcFutureSymbol, '')
		if len(ltrim(@vcFutureSymbol)) = 0
		begin
			if @tiIsRaiseError = 1 raiserror('Symbol for new future is not specified!', 16, 1)
			set @error = -2
			goto finish
		end

		-- check future
		if exists(select 1 from ContractView where contractTypeID = 4 and symbol = @vcFutureSymbol)
		begin
			if @tiIsRaiseError = 1 raiserror('Future with symbol ''%s'' is already exists!', 16, 1, @vcFutureSymbol)
			set @error = -3
			goto finish
		end

		-- correct expiry calendar ID
		if isnull(@iExpCalendarID, 0) = 0
			set @iExpCalendarID = 1

		-- check expiry calendar ID
		if not exists(select 1 from ExpiryCalendarView where expCalendarID = @iExpCalendarID)
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future options expiry calendar specified!', 16, 1)
			set @error = -4
			goto finish
		end

		-- calculate maturity ID by maturity date if maturity ID is not specified
		if @iMaturityID is null and @dtMaturityDate is not null
			set @iMaturityID = (year(@dtMaturityDate) - 2000) * 12 + month(@dtMaturityDate)

		-- check maturity ID
		if not exists(select 1 from ExpiryView where expiryID = @iMaturityID)
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future maturity specified!', 16, 1)
			set @error = -5
			goto finish
		end

		-- correct default underlying price profile
		if isnull(@iUndPriceProfileID, 0) = 0
			select
				@iUndPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 0

		-- check underlying price profile
		if not exists(select 1 from PriceProfileView where priceProfileID = @iUndPriceProfileID)
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future underlying price profile specified!', 16, 1)
			set @error = -6
			goto finish
		end
	
		-- correct default option price profile
		if isnull(@iOptPriceProfileID, 0) = 0
			select
				@iOptPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 1

		-- check options price profile
		if not exists(select 1 from PriceProfileView where priceProfileID = @iOptPriceProfileID)
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future options price profile specified!', 16, 1)
			set @error = -7
			goto finish
		end

		-- correct option style
		if isnull(@tiCalcOptionType, 0) = 0
			set @tiCalcOptionType = 1	-- american

		-- correct 'is active'
		if isnull(@tiIsActive, 0) = 0
			set @tiIsActive = 1	-- active by default

		begin tran

		-- save new contract data
		insert into Contract
			(contractTypeID, symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
		values
			(4, @vcFutureSymbol, @vcFutureName, @iExpCalendarID, @iUndPriceProfileID, @iOptPriceProfileID)
		if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end

		set @iFutureID = @@identity

		-- save new future data
		insert into Future
			(futureID, futureRootID, maturityID, calcOptionType, isActive)
		values
			(@iFutureID, @iFutureRootID, @iMaturityID, @tiCalcOptionType, @tiIsActive)
		if @@error <> 0 begin set @error = -9 if @@trancount = 1 rollback tran goto finish end

		-- save price record for default exchange
		if not exists(select 1 from ContractPrice where exchangeID is null and contractID = @iFutureID)
		begin
			insert into ContractPrice
				(contractID, exchangeID)
			values
				(@iFutureID, null)
			if @@error <> 0 begin set @error = -10 if @@trancount = 1 rollback tran goto finish end
		end

		commit tran
	end
	else
	begin -- existing future

		-- correct future name (prevent updates to empty string)
		if len(ltrim(@vcFutureSymbol)) = 0
			set @vcFutureSymbol = null

		-- check future name
		if @vcFutureSymbol is not null
			if exists(select 1 from ContractView where contractTypeID = 4 and contractID <> @iFutureID and symbol = @vcFutureSymbol)
			begin
				if @tiIsRaiseError = 1 raiserror('Future with symbol ''%s'' is already exists!', 16, 1, @vcFutureSymbol)
				set @error = -11
				goto finish
			end

		-- correct expiry calendar ID (prevent updates to zero calendar)
		if @iExpCalendarID = 0
			set @iExpCalendarID = null

		-- check expiry calendar
		if @iExpCalendarID is not null
		begin
			if not exists(select 1 from ExpiryCalendar where expCalendarID = @iExpCalendarID)
			begin
				if @tiIsRaiseError = 1 raiserror('Invalid future options expiry calendar specified!', 16, 1, @vcFutureSymbol)
				set @error = -12
				goto finish
			end
		end

		-- correct maturity ID (prevent updates to zero)
		if @iMaturityID = 0
			set @iMaturityID = null

		-- calculate maturity ID by maturity date if maturity ID is not specified
		if @iMaturityID is null and @dtMaturityDate is not null
			set @iMaturityID = (year(@dtMaturityDate) - 2000) * 12 + month(@dtMaturityDate)

		-- check maturity ID
		if @iMaturityID is not null
			if not exists(select 1 from ExpiryView where expiryID = @iMaturityID)
			begin
				if @tiIsRaiseError = 1 raiserror('Invalid future maturity specified!', 16, 1)
				set @error = -13
				goto finish
			end

		-- correct underlying price profile (prevent updates to zero)
		if @iUndPriceProfileID = 0
			set @iUndPriceProfileID = null

		-- check underlying price profile
		if @iUndPriceProfileID is not null
			if not exists(select 1 from PriceProfileView where priceProfileID = @iUndPriceProfileID)
			begin
				if @tiIsRaiseError = 1 raiserror('Invalid future underlying price profile specified!', 16, 1)
				set @error = -14
				goto finish
			end

		-- correct option price profile (prevent updates to zero)
		if @iOptPriceProfileID = 0
			set @iOptPriceProfileID = null

		-- check option price profile
		if @iOptPriceProfileID is not null
			if not exists(select 1 from PriceProfileView where priceProfileID = @iOptPriceProfileID)
			begin
				if @tiIsRaiseError = 1 raiserror('Invalid future options price profile specified!', 16, 1)
				set @error = -15
				goto finish
			end

		begin tran

		-- update contract data
		update Contract
		set symbol = isnull(@vcFutureSymbol, symbol),
			contractName = isnull(@vcFutureName, contractName),
			expCalendarID = isnull(@iExpCalendarID, expCalendarID),
			undPriceProfileID = isnull(@iUndPriceProfileID, undPriceProfileID),
			optPriceProfileID = isnull(@iOptPriceProfileID, optPriceProfileID)
		where contractID = @iFutureID
		if @@error <> 0 begin set @error = -16 if @@trancount = 1 rollback tran goto finish end

		-- save future data
		update Future
			set maturityID = isnull(@iMaturityID, maturityID),
			calcOptionType = isnull(@tiCalcOptionType, calcOptionType),
			isActive = isnull(@tiIsActive, isActive)
		where futureID = @iFutureID
		if @@error <> 0 begin set @error = -17 if @@trancount = 1 rollback tran goto finish end

		commit tran
	end
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoFutureOption_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureOption_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureOption_Get]
GO

CREATE PROC dbo.usp_BoFutureOption_Get
	@iFutureID int = null
AS
	-- get future options

	set nocount on
	
	if @iFutureID is not null
		select
			fo.futureOptionID as iFutureOptionID,
			foc.contractTypeID as iFutureOptionContractTypeID,
			foc.symbol as vcFutureOptionSymbol,
			foc.contractName as vcFutureOptionName,
			f.futureID as iFutureID,
			fc.contractTypeID as iFutureContractTypeID,
			fc.symbol as vcFutureSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			fo.expiryID as iExpiryID, 
			fec.expiryDate as dtExpiry,
			fo.strike as fStrike, 
			fo.isCall as tiIsCall,
			fo.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureOption fo on f.futureID = fo.futureID
			inner join ContractView foc on fo.futureOptionID = foc.contractID
			inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
		where
			f.futureID = @iFutureID
	else
		select
			fo.futureOptionID as iFutureOptionID,
			foc.contractTypeID as iFutureOptionContractTypeID,
			foc.symbol as vcFutureOptionSymbol,
			foc.contractName as vcFutureOptionName,
			f.futureID as iFutureID,
			fc.contractTypeID as iFutureContractTypeID,
			fc.symbol as vcFutureSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			fo.expiryID as iExpiryID, 
			fec.expiryDate as dtExpiry,
			fo.strike as fStrike, 
			fo.isCall as tiIsCall,
			fo.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureOption fo on f.futureID = fo.futureID
			inner join ContractView foc on fo.futureOptionID = foc.contractID
			inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
GO

----------------------------------------------------------------------------------------------------
-- usp_BoFutureOption_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureOption_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureOption_Del]
GO

CREATE PROC dbo.usp_BoFutureOption_Del
	@iFutureOptionID int,
	@tiIsRaiseError tinyint = 1 
AS
	-- delete future option

	set nocount on
	
	declare @error int		set @error = 0

	-- check future ID
	if @iFutureOptionID is null
	begin
		if @tiIsRaiseError = 1 raiserror('Future option to delete is not specified!', 16, 1)
		set @error = -1
		goto finish
	end

	-- check for trades existence
	if exists(select 1 from Trade where contractID = @iFutureOptionID)
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future option, because it has trades!', 16, 1)
		set @error = -2
		goto finish
	end

	-- check for orders existence
	if exists(select 1 from [Order] where contractID = @iFutureOptionID)
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future option, because it has orders!', 16, 1)
		set @error = -3
		goto finish
	end

	begin tran
	
	-- delete future options
	delete FutureOption
	where futureID = @iFutureOptionID
	if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

	delete ContractPrice
	where contractID = @iFutureOptionID
	if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end

	update Contract set actionID = 3
	where contractID = @iFutureOptionID
	if @@error <> 0 begin set @error = -7 if @@trancount = 1 rollback tran goto finish end

	commit tran

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoFutureOption_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureOption_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureOption_Save]
GO

CREATE PROC dbo.usp_BoFutureOption_Save
	@iFutureOptionID int out,
	@iFutureID int, -- should be specified for new future option (ignored for update)
	@vcFutureOptionSymbol varchar(20) = null,
	@vcFutureOptionName varchar(255) = null, -- calculated if not specified (for new option)
	@vcExportSymbol varchar(20) = null,
	@tiIsCall tinyint = null,
	@fStrike float = null,
	@iExpiryID int = null, -- calculated by @dtExpiryDate if not specified
	@dtExpiryDate datetime = null, -- ignored if @iExpiryID is specified
	@tiIsRaiseError tinyint = 1 
AS
	-- create/update future

	set nocount on
	
	declare @error int		set @error = 0

	if @iFutureOptionID is null -- new option
	begin
		-- check new future option future ID
		if @iFutureID is null
		begin
			if @tiIsRaiseError = 1 raiserror('Future for new future option is not specified!', 16, 1)
			set @error = -1
			goto finish
		end

		declare @vcFutureSymbol varchar(20)
		declare @iFutureContractTypeID int
		declare @iFutureExpCalendarID int

		-- find future name and contract type
		select @iFutureContractTypeID = contractTypeID,
			@vcFutureSymbol = symbol,
			@iFutureExpCalendarID = expCalendarID
		from ContractView
		where contractID = @iFutureID		

		-- check future existence
		if @iFutureContractTypeID is null
		begin
			if @tiIsRaiseError = 1 raiserror('Underlying contract for new future option is invalid!', 16, 1)
			set @error = -2
			goto finish
		end

		-- check future contract type
		if @iFutureContractTypeID <> 4
		begin
			if @tiIsRaiseError = 1 raiserror('Underlying contract for new future option is not a future!', 16, 1)
			set @error = -3
			goto finish
		end

		-- check future option symbol
		set @vcFutureOptionSymbol = isnull(@vcFutureOptionSymbol, '')
		if len(ltrim(@vcFutureOptionSymbol)) = 0
		begin
			if @tiIsRaiseError = 1 raiserror('Symbol for new future option is not specified!', 16, 1)
			set @error = -4
			goto finish
		end

		-- check strike
		if isnull(@fStrike, 0) <= 0
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future option strike specified!', 16, 1)
			set @error = -5
			goto finish
		end

		-- round strike upto 3 decimals
		set @fStrike = round(@fStrike, 3)

		-- calculate expiry ID by expiry date if expiry ID is not specified
		if @iExpiryID is null and @dtExpiryDate is not null
			set @iExpiryID = (year(@dtExpiryDate) - 2000) * 12 + month(@dtExpiryDate)

		-- check expiry ID/Date
		select @dtExpiryDate = expiryDate
		from ExpiryInCalendarView
		where expiryID = @iExpiryID and expCalendarID = @iFutureExpCalendarID

		if @dtExpiryDate is null
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future option expiry specified!', 16, 1)
			set @error = -6
			goto finish
		end

		-- correct call/put
		set @tiIsCall = isnull(@tiIsCall, 0)
		if @tiIsCall <> 0
			set @tiIsCall = 1

		-- check option duplacates by symbol
		if exists(select 1 from ContractView where contractTypeID = 5 and symbol = @vcFutureOptionSymbol)
		begin
			if @tiIsRaiseError = 1 raiserror('Future option with symbol ''%s'' is already exists!', 16, 1, @vcFutureOptionSymbol)
			set @error = -7
			goto finish
		end

		-- check option duplacates by parameters
		if exists (select 1 from FutureOption
				where futureID = @iFutureID
					and expiryID = @iExpiryID and isCall = @tiIsCall
					and abs(strike - @fStrike) < 0.001) -- check equality upto 3 decimals
		begin
			if @tiIsRaiseError = 1 raiserror('Future option with the same parameters is already exists for specified future!', 16, 1)
			set @error = -8
			goto finish
		end

		-- make default future option contract name if not specified
		if @vcFutureOptionName is null
			set @vcFutureOptionName = @vcFutureSymbol +
									' ' + datename(month, @dtExpiryDate) + 
									' ' + datename(year, @dtExpiryDate) +
									' ' + case @tiIsCall when 1 then 'Call' else 'Put' end +
									' ' + cast(floor(@fStrike) as varchar)

		begin tran

		-- save new contract data
		insert into Contract
			(contractTypeID, symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
		values
			(5, @vcFutureOptionSymbol, @vcFutureOptionName, null, null, null)
		if @@error <> 0 begin set @error = -9 if @@trancount = 1 rollback tran goto finish end

		set @iFutureOptionID = @@identity

		-- save new future option data
		insert into FutureOption
			(futureOptionID, futureID, isCall, expiryID, strike)
		values
			(@iFutureOptionID, @iFutureID, @tiIsCall, @iExpiryID, @fStrike)
		if @@error <> 0 begin set @error = -10 if @@trancount = 1 rollback tran goto finish end

		-- save price record for default exchange
		if not exists(select 1 from ContractPrice where exchangeID is null and contractID = @iFutureOptionID)
		begin
			insert into ContractPrice
				(contractID, exchangeID)
			values
				(@iFutureOptionID, null)
			if @@error <> 0 begin set @error = -11 if @@trancount = 1 rollback tran goto finish end
		end

		commit tran
	end
	else
	begin
		-- correct future option name (prevent updates to empty string)
		if len(ltrim(@vcFutureOptionSymbol)) = 0
			set @vcFutureOptionSymbol = null

		-- check future option name
		if @vcFutureOptionSymbol is not null
			if exists(select 1 from ContractView where contractTypeID = 5
						and contractID <> @iFutureOptionID and symbol = @vcFutureOptionSymbol)
			begin
				if @tiIsRaiseError = 1 raiserror('Future option with symbol ''%s'' is already exists!', 16, 1, @vcFutureOptionSymbol)
				set @error = -12
				goto finish
			end

		-- correct strike (prevent updates to zero)
		if @fStrike = 0
			set @fStrike = null

		-- check strike
		if @fStrike is not null
		begin
			if @fStrike <= 0
			begin
				if @tiIsRaiseError = 1 raiserror('Invalid future option strike specified!', 16, 1)
				set @error = -13
				goto finish
			end

			-- round strike upto 3 decimals
			set @fStrike = round(@fStrike, 3)
		end

		-- correct call/put
		if @tiIsCall is not null
			if @tiIsCall <> 0
				set @tiIsCall = 1

		-- correct expiry ID (prevent updates to zero)
		if @iExpiryID = 0
			set @iExpiryID = null

		-- calculate expiry ID by expiry date if expiry ID is not specified
		if @iExpiryID is null and @dtExpiryDate is not null
			set @iExpiryID = (year(@dtExpiryDate) - 2000) * 12 + month(@dtExpiryDate)

		-- check expiry ID
		if @iExpiryID is not null
			if not exists(select 1 from ExpiryView where expiryID = @iExpiryID)
			begin
				if @tiIsRaiseError = 1 raiserror('Invalid future option expiry specified!', 16, 1)
				set @error = -14
				goto finish
			end

		if @tiIsCall is not null or @fStrike is not null or @iExpiryID is not null
		begin
			declare @tiIsCallToCheck tinyint
			declare @fStrikeToCheck float
			declare @iExpiryIDToCheck int
	
			-- prepare parameters to check option duplacates
			select @tiIsCallToCheck = isnull(@tiIsCall, isCall),
				@fStrikeToCheck = isnull(@fStrike, strike),
				@iExpiryIDToCheck = isnull(@iExpiryID, expiryID)
			from FutureOption
			where futureOptionID = @iFutureOptionID
	
			-- check option duplacates by parameters
			if exists (select 1 from FutureOption
					where futureOptionID <> @iFutureOptionID and futureID = @iFutureID
						and expiryID = @iExpiryIDToCheck and isCall = @tiIsCallToCheck
						and abs(strike - @fStrikeToCheck) < 0.001) -- check equality upto 3 decimals
			begin
				if @tiIsRaiseError = 1 raiserror('Future option with the same parameters is already exists for specified future!', 16, 1)
				set @error = -15
				goto finish
			end
		end

		begin tran

		-- update contract data
		update Contract
		set symbol = isnull(@vcFutureOptionSymbol, symbol),
			contractName = isnull(@vcFutureOptionName, contractName)
		where contractID = @iFutureOptionID
		if @@error <> 0 begin set @error = -16 if @@trancount = 1 rollback tran goto finish end

		-- save future option data
		update FutureOption
		set isCall = isnull(@tiIsCall, isCall),
			expiryID = isnull(@iExpiryID, expiryID),
			strike = isnull(@fStrike, strike)
		where futureOptionID = @iFutureOptionID
		if @@error <> 0 begin set @error = -17 if @@trancount = 1 rollback tran goto finish end

		commit tran
	end

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTrade_StoreToHistoryAndClear
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTrade_StoreToHistoryAndClear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTrade_StoreToHistoryAndClear]
GO

CREATE PROC dbo.usp_BoTrade_StoreToHistoryAndClear
	@tiStoreToHistory tinyint = null,
	@dtLastTradeDateUTC datetime = null,
	@iStoredTradesCount int = null out
AS
	declare @error int		set @error = 0
	
	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	begin tran

	-- clear simulated and manual deleted trades
	delete from Trade 
		where tradeID in (select tradeID from Trade where isnull(status, 0) in (1, 2) and actionID = 0)
	if @@error !=0 begin set @error = -2 if @@TranCount = 1 Rollback Tran goto finish end

	-- clear actual and actual matched deleted trades
	delete from Trade 
		where tradeID in (select tradeID from Trade where isnull(status, 0) in (0, 3) and actionID = 0)
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
	
			uc.contractID as iUnderlyingContractID, uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol, uc.exportSymbol as vcUnderlyingExportSymbol,
			s.divFreq as iUnderlyingDivFreq, s.divDate as dtUnderlyingDivDate,
			s.divAmt as fUnderlyingDivAmt, i.yield as fUnderlyingYield,
			ucp.priceClose as fUnderlyingPriceClose,
	
			fr.futureRootSymbol as vcFutureRootSymbol, f.futureID as iFutureID,
			fc.symbol as vcFutureSymbol, fc.exportSymbol as vcFutureExportSymbol,
			fmc.expiryDate as dtFutureMaturity, fr.futureLotSize as iFutureLotSize,
			fcp.priceClose as fFuturePriceClose,
	
			ort.optionRootID as iOptionRootID, ort.symbol as vcOptionRootSymbol, ort.lotSize as iOptionRootLotSize,
	
			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtOptionExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fOptionStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiOptionIsCall,
			case when c.contractTypeID = 3 then cp.priceTheo else fcp.priceTheo end as fPriceTheoClose,
	
			tr.traderID as iTraderID, t.acronym as vcTraderAcronym, t.traderName as vcTraderName,
	
			st.strategyID as iStrategyID, st.strategyName as vcStrategyName,
	
			tr.brokerID as iBrokerID, br.brokerName as vcBrokerName, tr.brokerCommission as fBrokerCommission,
	
			tr.clearingBrokerID as iClearingBrokerID, cbr.brokerName as vcClearingBrokerName,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
	
			tr.tradedIV as fTradedIV, tr.spotReference as fSpotReference,

			fr.futureRootID as iFutureRootID,
			fr.optionLotSize as iFutureOptionLotSize
	
		from TradeView tr
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			left join ContractPrice ucp on tr.underlyingID = ucp.contractID and ucp.exchangeID is null
			-- regular option
			left join OptionView o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- underlying
			left join StockView s on tr.underlyingID = s.stockID
			left join IndexView i on tr.underlyingID = i.indexID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join ExpiryInCalendarView fmc on f.maturityID = fmc.expiryID and fmc.expCalendarID = fr.matCalendarID
			left join ContractPrice fcp on tr.futureID = fcp.contractID and fcp.exchangeID is null
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- additional
			left join BrokerView br on tr.brokerID = br.brokerID
			left join BrokerView cbr on tr.clearingBrokerID = cbr.brokerID
			left join TraderView t on tr.traderID = t.traderID
			left join StrategyView st on tr.strategyID = st.strategyID
		where
			isnull(status, 0) in (0, 3)
			and isnull(tr.isPosition, 0) = 0
			and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
	
		set @iStoredTradesCount = @@rowcount

		if @@error !=0 begin set @error = -5 if @@TranCount = 1 Rollback Tran goto finish end

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
	drop procedure [dbo].[usp_BoTradeHistory_Del]
GO

CREATE PROC dbo.usp_BoTradeHistory_Del
	@dtLastTradeDateUTC datetime = null,
	@iDeletedTradesCount int = null out
AS
	declare @error int		set @error = 0
	
	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	begin tran

	delete from TradeHistory
	where datediff(day, tradeDate, @dtLastTradeDateUTC) >= 0

	set @iDeletedTradesCount = @@rowcount

	if @@error != 0 begin set @error = -1 if @@TranCount = 1 Rollback Tran goto finish end
	commit tran

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- remove usp_TradeByTrader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeByTrader_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_MmTradeByTrader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmTradeByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmTradeByTrader_Get]
GO

CREATE PROC dbo.usp_MmTradeByTrader_Get
	@iTraderID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iTraderID is null
		select
			tr.tradeID as iTradeID,
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
			tr.mark as iMark,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
		order by TradeDate
	else
		select
			tr.tradeID as iTradeID,
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
			tr.mark as iMark,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
		where
			tr.traderID = @iTraderID
		order by TradeDate

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- remove usp_TradeSeqByTrader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSeqByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeSeqByTrader_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_MmTradeSeqByTrader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmTradeSeqByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmTradeSeqByTrader_Get]
GO

CREATE PROC dbo.usp_MmTradeSeqByTrader_Get
	@iTraderID int = null,
	@iMinSeqNum int = null,
	@iMaxSeqNum int = null
AS
	set nocount on

	set @iMinSeqNum = isnull(@iMinSeqNum, 0)
	set @iMaxSeqNum = isnull(@iMaxSeqNum, 2147483647)

	if @iTraderID is null
		select
			tr.seqNum as iSeqNum,
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
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,
			uc.Symbol as vcUnderlyingSymbol,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,

			tl.eventTypeID as iStructureMissed
		from Trade tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where
			tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
		order by tr.seqNum
	else
		select
			tr.seqNum as iSeqNum,
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
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,
			uc.Symbol as vcUnderlyingSymbol,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,

			tl.eventTypeID as iStructureMissed
		from Trade tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where
			tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
			and tr.traderID = @iTraderID
		order by tr.seqNum
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
			tr.mark as iMark,
			tr.underlyingID as iUnderlyingID,
			tr.optionRootID as iOptionRootID,
			tr.futureRootID as iFutureRootID,
			tr.futureID as iFutureID
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
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			cp.priceClose as fPriceClose,

			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			ucp.priceClose as fUnderlyingPriceClose,

			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			case when c.contractTypeID = 3 then cp.priceTheo else fcp.priceTheo end as fPriceTheoClose,

			case when c.contractTypeID < 3 then 1
				when c.contractTypeID = 3 then ort.lotSize
				when c.contractTypeID = 4 then fr.futureLotSize
				when c.contractTypeID = 5 then fr.optionLotSize
			end as iLotSize,

			ort.optionRootID as iOptionRootID,
			ort.isSynthetic as tiIsSyntheticRoot,

			tl.eventTypeID as iStructureMissed
		from Trade tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			left join ContractPrice ucp on tr.underlyingID = ucp.contractID and ucp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- underlying
			left join Stock s on tr.underlyingID = s.stockID
			left join [Index] i on tr.underlyingID = i.indexID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join ExpiryInCalendarView fmc on f.maturityID = fmc.expiryID and fmc.expCalendarID = fr.matCalendarID
			left join ContractPrice fcp on tr.futureID = fcp.contractID and fcp.exchangeID is null
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where c.contractTypeID = 3
			and (@iMinSeqNum is null or @iMinSeqNum is not null and tr.seqNum >= @iMinSeqNum)
			and (@iMaxSeqNum is null or @iMaxSeqNum is not null and tr.seqNum <= @iMaxSeqNum)
			and (@dtTradeDate is null or @dtTradeDate is not null and tr.tradeDate >= @dtTradeDate and tr.tradeDate < @dtMaxDate)
			and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		order by tr.seqNum

GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_Clear
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_Clear]
GO

CREATE PROC dbo.usp_Trade_Clear
	@iContractID int = null,
	@tiOnlyWithExecID tinyint = null,
	@lastUTCTradeDate datetime = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction

	delete from Trade 
	where tradeID in (select tradeID from Trade where isnull(status, 0) in (1, 2) and actionID = 0) 
						and (@iContractID is null or contractID = @iContractID)
		if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

	if isnull(@tiOnlyWithExecID, 0) = 0
	begin
		delete from Trade 
			where (@iContractID is null or contractID = @iContractID) 
				and isnull(status, 0) in (0, 3)
				and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0)
		if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
	end
	else
	begin
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

----------------------------------------------------------------------------------------------------
-- usp_Trade_ClearDeleted
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_ClearDeleted]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_ClearDeleted]
GO

CREATE PROC dbo.usp_Trade_ClearDeleted 
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction Trade

	delete from Trade where tradeID in (select tradeID from Trade where actionID = 0) 
		if @@error !=0 begin set @error = 2 if @@TranCount = 1 Rollback Tran goto finish end

	set @error = @@error

	delete Trade from Trade T join [Option] O on O.optionID = T.contractID
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract UC on ORt.underlyingID = UC.contractID 
					join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
					where E.expiryDate < GetDate() 
	if @@error !=0 begin set @error = 2 if @@TranCount = 1 Rollback Tran goto finish end

	set @error = @@error

	Commit Transaction Trade

finish:
	return @error
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
				tradedIV, spotReference, isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID)
			select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID
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
		isPosition, status, execID, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	select @iNewOptTradeID, 1, contractID, @dtNewOptTradeDate, @iOptQuantity, 0, @tiOptIsBuy,
		traderID, strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, 0, @tiOptStatus, null, @iNewOptSeqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID
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
			isPosition, status, execID, seqNum, mark,
			underlyingID, optionRootID, futureRootID, futureID)
		select @iNewUndTradeID, 1, @iUndContractID, @dtNewUndTradeDate, @iUndQuantity, @fUndPrice, @tiUndIsBuy,
			traderID, strategyID, brokerID, @fUndBrokerCommission, clearingBrokerID, @fUndClearingBrokerCommission,
			0, 0, 0, @tiUndStatus, null, @iNewUndSeqNum, mark,
			@iUndContractID, null, null, null
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
				isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID)
			select tradeID, 0, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, @iOldManualSeqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID
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
				isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID)
			select tradeID, 0, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, @iOldActualSeqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID
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
				isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID)
			select @iNewActualTradeID, 1, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, 3, execID, @iNewActualSeqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID
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
    @iStrategyID int = null out,
	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null
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

	-- determine strategy for trade
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
		tradedIV, spotReference, isPosition, status, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, @iStrategyID, @tiIsBuy,
		@iBrokerID, @fBrokerCommission, @iClearingBrokerID, @fClearingBrokerCommission,
		@fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, @iNewSeqNum, @iMark,
		@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID)

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
	@dtTradeDateTo datetime out,

	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null
as
	set nocount on

	begin tran

	declare @error int		set @error = 0

	declare @iDeltaGMT int
	exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

	-- Trade 'from' creation
	if @dtTradeDateFrom is null	
		set @dtTradeDateFrom = dateadd(minute, @iDeltaGMT, getdate())

	-- determine strategy for trade from
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
		tradedIV, spotReference, isPosition, status, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	values (@iTradeIDFrom, 1, @iContractID, @dtTradeDateFrom, @iQuantity, @fPrice, @iTraderIDFrom, @iStrategyIDFrom,
		@tiIsBuyFrom, null, null, null, null,
		null, null, 0, @tiStatus, @iSeqNumFrom, @iMarkFrom,
		@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID)

	if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

	-- Trade 'to' creation
	if @dtTradeDateTo is null	
		set @dtTradeDateTo = dateadd(minute, @iDeltaGMT, getdate())

	-- determine strategy for trade to
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
		tradedIV, spotReference, isPosition, status, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	values (@iTradeIDTo, 1, @iContractID, @dtTradeDateTo, @iQuantity, @fPrice, @iTraderIDTo, @iStrategyIDTo,
		@tiIsBuyTo, null, null, null, null,
		null, null, 0, @tiStatus, @iSeqNumTo, @iMarkTo,
		@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID)

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
    @iNewTradeStrategyID int = Null out,
	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null
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
				tradedIV, spotReference, isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID)
			select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID
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
	
		-- determine strategy for trade
		exec @error = usp_TraderContractStrategyWithSave_Get @iTraderID, @iContractID, @iNewTradeStrategyID out
		if @@error <> 0 or @error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

		insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, 
			strategyID, isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
			tradedIV, spotReference, isPosition, status, seqNum, execID, mark,
			underlyingID, optionRootID, futureRootID, futureID)
		values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, 
			@iNewTradeStrategyID,  @tiIsBuy, @iBrokerID, @fBrokerCommission, @iClearingBrokerID, 
			@fClearingBrokerCommission, @fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, 
			@iNewSeqNum, @vcExecID, @iMark,
			@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID)

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
		tradedIV, spotReference, isPosition, status, execID, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	select 
		tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
		strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID
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
			mark as iMark,
			underlyingID as iUnderlyingID,
			optionRootID as iOptionRootID,
			futureRootID as iFutureRootID,
			futureID as iFutureID
		from TradeView
		where tradeID = @iTradeID
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
			mark as iMark,
			underlyingID as iUnderlyingID,
			optionRootID as iOptionRootID,
			futureRootID as iFutureRootID,
			futureID as iFutureID
		from TradeView
		where contractID = @iContractID
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
			status as tiStatus,
			mark as iMark,
			underlyingID as iUnderlyingID,
			optionRootID as iOptionRootID,
			futureRootID as iFutureRootID,
			futureID as iFutureID
		from TradeView
		where optionRootID = @iOptionRootID
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
			mark as iMark,
			underlyingID as iUnderlyingID,
			optionRootID as iOptionRootID,
			futureRootID as iFutureRootID,
			futureID as iFutureID,
			underlyingID as iUnderlyingID,
			optionRootID as iOptionRootID,
			futureRootID as iFutureRootID,
			futureID as iFutureID
		from TradeView
		where underlyingID = @iUnderlyingID
		order by TradeDate
	end

    return @@error
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
	@iMark int = null,
	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null
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
			clearingBrokerID, clearingBrokerCommission, tradeDate, mark,
			underlyingID, optionRootID, futureRootID, futureID)
		select @iNewTradeID, 1, contractID, @iQuantity, @fPrice,
			isBuy, traderID, strategyID, isPosition, status, @iNewSeqNum, brokerID, brokerCommission,
			clearingBrokerID, clearingBrokerCommission, tradeDate, mark,
			underlyingID, optionRootID, futureRootID, futureID
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

		-- determine strategy for trade
		declare @iStrategyID int
		exec @error = usp_TraderContractStrategyWithSave_Get @iTraderID, @iContractID, @iStrategyID out
		if @error != 0 begin Rollback Transaction TU Return(-1) end

		-- insert new trade
		insert into Trade(tradeID, actionID, contractID, quantity, price,
			isBuy, traderID, strategyID, isPosition, status, seqNum, brokerID, brokerCommission,
			clearingBrokerID, clearingBrokerCommission, tradeDate, mark,
			underlyingID, optionRootID, futureRootID, futureID)
		values 
			(@iNewTradeID, 1, @iContractID, @iQuantity, @fPrice, @tiIsBuy, @iTraderID,
			@iStrategyID, @tiIsPosition, @tiStatus, @iNewSeqNum, null, null, null, null, @dtNewTradeDate, @iMark,
			@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID)

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
		tradedIV, spotReference, isPosition, status, execID, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
		strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID
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

	return 0

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

	declare @iNewTradeID int
		select @iNewTradeID = max(tradeID) + 1 from Trade

	declare @iSeqNum int 
		select @iSeqNum = isnull(max(seqNum),0) + 1 from Trade

	if @iNewTradeID is null set @iNewTradeID = 1

	-- determine strategy for trade
	declare @iNewTradeStrategyID int
	exec @error = usp_TraderContractStrategyWithSave_Get @iTraderID, @iTradeContractID, @iNewTradeStrategyID out
	if @@error <> 0 or @error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

	--create new trade	
	insert into Trade (execID, tradeID, actionID, contractID, tradeDate, 
		quantity, price, traderID, strategyID, isBuy, isPosition, status, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	values (@vcExecID, @iNewTradeID, 1, @iTradeContractID, @dtTradeDate, 
		@iQuantity, @fPrice, @iTraderID, @iNewTradeStrategyID, @tiIsBuy, 0, 0, @iSeqNum, @iMark,
		@iUnderlyingContractID, @iOptionRootID, null, null)

	if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

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
	select isnull(@iMinSeqNum,0) as iMinSeqNum,@iSeqNum as iMaxSeqNum, @iStructureMissed as iStructureMissed

	Commit Transaction

    set @error = 0
	return @error

finish:
	select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed
	return @@error
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
		tr.isPosition as tiIsPosition,
		tr.status as tiStatus,
		tr.execID as vcExecID,
		tr.mark as iMark,

		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		cp.priceClose as fPriceClose,

		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		ucp.priceClose as fUnderlyingPriceClose,

		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,
		i.yield as fYield,

		case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
		case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
		case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
		case when c.contractTypeID = 3 then cp.priceTheo else fcp.priceTheo end as fPriceTheoClose,

		case when c.contractTypeID < 3 then 1
			when c.contractTypeID = 3 then ort.lotSize
			when c.contractTypeID = 4 then fr.futureLotSize
			when c.contractTypeID = 5 then fr.optionLotSize
		end as iLotSize,

		ort.optionRootID as iOptionRootID,
		ort.isSynthetic as tiIsSyntheticRoot,

		tl.eventTypeID as iStructureMissed
	from Trade tr 
		-- contract and underlying contract
		inner join Contract c on tr.contractID = c.contractID
		inner join Contract uc on tr.underlyingID = uc.contractID
		left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
		left join ContractPrice ucp on tr.underlyingID = ucp.contractID and ucp.exchangeID is null
		-- regular option
		left join [Option] o on tr.contractID = o.optionID
		left join OptionRoot ort on tr.optionRootID = ort.optionRootID
		left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
		-- underlying
		left join Stock s on tr.underlyingID = s.stockID
		left join [Index] i on tr.underlyingID = i.indexID
		-- futures part
		left join ContractView fc on tr.futureID = fc.contractID
		left join FutureRoot fr on tr.futureRootID = fr.futureRootID
		left join Future f on tr.futureID = f.futureID
		left join ExpiryInCalendarView fmc on f.maturityID = fmc.expiryID and fmc.expCalendarID = fr.matCalendarID
		left join ContractPrice fcp on tr.futureID = fcp.contractID and fcp.exchangeID is null
		left join FutureOption fo on tr.contractID = fo.futureOptionID
		left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
		-- trade log
		left join TradeLog tl on tr.underlyingID = tl.underlyingID
	where tr.actionID > 0 
		and tr.execID = @vcExecID 
		and datediff(day, @dtTradeDate, tr.tradeDate) = 0
GO

----------------------------------------------------------------------------------------------------
-- usp_TradeLog_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeLog_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeLog_Del]
GO

CREATE PROC dbo.usp_TradeLog_Del 
	@iUnderlyingContractID int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction

	if isnull(@iUnderlyingContractID, 0) = 0
		delete from TradeLog
	else
	begin
		delete from TradeLog 
		where underlyingID = @iUnderlyingContractID
	end

	if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

	Commit Transaction

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_TradeLog_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeLog_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeLog_Get]
GO

CREATE PROC dbo.usp_TradeLog_Get
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	select
		uc.contractID as iContractID,
		uc.contractTypeID as iContractTypeID,
		uc.symbol as vcSymbol,
		tl.eventTypeID as iEventTypeID
	from TradeLog tl
		inner join ContractView uc on tl.underlyingID = uc.contractID
	order by uc.symbol
	
	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_TradeLog_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeLog_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeLog_Save]
GO

CREATE PROC dbo.usp_TradeLog_Save
	@iUnderlyingID int, 
	@iEventTypeID int = 0
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction

	insert into TradeLog (underlyingID, eventTypeID) 
		values (@iUnderlyingID, @iEventTypeID)

	if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

	Commit Transaction

finish:
	return @error

GO

----------------------------------------------------------------------------------------------------
-- usp_TradeSim_Clear
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeSim_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeSim_Clear]
GO

CREATE PROC dbo.usp_TradeSim_Clear
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
			delete from Trade 
			where tradeID in (select tradeID from Trade where isnull(status, 0) = 1) 
				if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
		end
	
		if isnull(@tiManual, 0) <> 0
		begin
			delete from Trade 
			where tradeID in (select tradeID from Trade where isnull(status, 0) = 2) 
				if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
		end
	
		set @error = @@error
	Commit Transaction

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoPositionForReconcile_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoPositionForReconcile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoPositionForReconcile_Get]
GO

CREATE PROC dbo.usp_BoPositionForReconcile_Get
	@lastUTCTradeDate datetime = null
AS
	set nocount on

	if @lastUTCTradeDate is null set @lastUTCTradeDate = getdate()

	select 
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		cp.priceClose as fPriceClose,

		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,

		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,

		case when c.contractTypeID < 3 then 1
			when c.contractTypeID = 3 then ort.lotSize
			when c.contractTypeID = 4 then fr.futureLotSize
			when c.contractTypeID = 5 then fr.optionLotSize
		end as iLotSize,

		case when c.contractTypeID = 3 then o.expiryID else fo.expiryID end as iExpiryID,
		case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
		case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,

		trdr.TraderID as iTraderID,
		trdr.acronym as vcAcronym,
		sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

		tr.strategyID as iStrategyID,
		st.strategyName as vcStrategyName,

		fr.futureRootID as iFutureRootID,
		fr.futureRootSymbol as vcFutureRootSymbol,
		f.futureID as iFutureID,
		fc.symbol as vcFutureSymbol,
		f.maturityID as iFutureMaturityID, 
		fr.futureLotSize as iFutureLotSize,
		fr.optionLotSize as iFutureOptionLotSize
	from TradeView tr 
		-- contract and underlying contract
		inner join ContractView c on tr.contractID = c.contractID
		inner join ContractView uc on tr.underlyingID = uc.contractID
		left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		-- regular option
		left join OptionView o on c.contractID = o.optionID
		left join OptionRootView ort on tr.optionRootID = ort.optionRootID
		-- trader and strategy
		left join TraderView trdr on tr.traderID = trdr.traderID
		left join StrategyView st on st.strategyID = tr.strategyID
		-- futures
		left join ContractView fc on tr.futureID = fc.contractID
		left join FutureRoot fr on tr.futureRootID = fr.futureRootID
		left join Future f on tr.futureID = f.futureID
		left join FutureOption fo on tr.contractID = fo.futureOptionID
	where tr.status in (0, 3)
		and datediff(day, tr.tradeDate, @lastUTCTradeDate) >= 0
	group by c.contractID, c.contractTypeID, c.Symbol, ort.optionRootID, ort.symbol,
		o.expiryID, o.strike, o.isCall, fo.expiryID, fo.strike, fo.isCall, 
		uc.contractID, uc.contractTypeID, uc.Symbol,
		ort.lotSize, cp.priceClose, trdr.TraderID, trdr.acronym, tr.strategyID, st.strategyName,
		fr.futureRootID, f.futureID, f.maturityID, fr.futureLotSize, fr.optionLotSize,
		fr.futureRootSymbol, fc.symbol
	order by vcSymbol
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTradeHistory_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTradeHistory_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTradeHistory_Get]
GO

CREATE PROC dbo.usp_BoTradeHistory_Get
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

		futureRootID as iFutureRootID,
		futureID as iFutureID,
		futureRootSymbol as vcFutureRootSymbol,
		futureSymbol as vcFutureSymbol,
		futureExportSymbol as vcFutureExportSymbol,
		futureMaturity as vcFutureMaturity,
		futureLotSize as iFutureLotSize,
		futureOptionLotSize as iFutureOptionLotSize,
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
	
		case when c.contractTypeID = 3 then o.expiryID else fo.expiryID end as iExpiryID,
		case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
		case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
	
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		uc.exportSymbol as vcUnderlyingExportSymbol,
	
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,

		case when c.contractTypeID < 3 then 1
			when c.contractTypeID = 3 then ort.lotSize
			when c.contractTypeID = 4 then fr.futureLotSize
			when c.contractTypeID = 5 then fr.optionLotSize
		end as iLotSize,

		t.traderID as iTraderID,
		t.acronym as vcTraderName,

		br.brokerID as iBrokerID,
		br.brokerName as vcBrokerName,
		tr.brokerCommission as fBrokerCommission,

		tr.mark as iTradeMark,

		st.strategyID as iStrategyID,
		st.strategyName as vcStrategyName,

		trg.trgID as iTraderGroupID,
		trg.trgName as vcTraderGroupName,

		fr.futureRootID as iFutureRootID,
		fr.futureRootSymbol as vcFutureRootSymbol,
		f.futureID as iFutureID,
		fc.symbol as vcFutureSymbol,
		fc.exportSymbol as vcFutureExportSymbol,
		f.maturityID as iFutureMaturityID, 
		fr.futureLotSize as iFutureLotSize,
		fr.optionLotSize as iFutureOptionLotSize
	from
		TradeView tr
		-- contract and underlying contract
		inner join ContractView c on tr.contractID = c.contractID
		inner join ContractView uc on tr.underlyingID = uc.contractID
		-- regular option
		left join OptionView o on c.contractID = o.optionID
		left join OptionRootView ort on tr.optionRootID = ort.optionRootID
		-- trade data
		left join TraderView t on tr.traderID = t.traderID
		left join StrategyView st on st.strategyID = tr.strategyID
		left join TraderGroup trg on t.traderGroupID = trg.trgID
		left join BrokerView br on tr.brokerID = br.brokerID
		-- futures
		left join ContractView fc on tr.futureID = fc.contractID
		left join FutureRoot fr on tr.futureRootID = fr.futureRootID
		left join Future f on tr.futureID = f.futureID
		left join FutureOption fo on tr.contractID = fo.futureOptionID
	where
		tr.status in (0, 3) -- only actual (or actual matched) trades should be reported
		and isnull(tr.isPosition, 0) = 0 -- only trades (not positions) should be reported
		and len(isnull(tr.execID, '')) = 0 -- only non-RTTF trades should be reported (make sure that all RTTF trades has execID)
		and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
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
		c.symbol as vcSymbol,
		c.exportSymbol as vcExportSymbol,
	
		case when c.contractTypeID = 3 then o.expiryID else fo.expiryID end as iExpiryID,
		case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
		case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
	
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		uc.exportSymbol as vcUnderlyingExportSymbol,
	
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,

		fr.futureRootID as iFutureRootID,
		fr.futureRootSymbol as vcFutureRootSymbol,
		f.futureID as iFutureID,
		fc.symbol as vcFutureSymbol,
		fc.exportSymbol as vcFutureExportSymbol,
		f.maturityID as iFutureMaturityID
	from
		TradeView tr
		-- contract and underlying contract
		inner join ContractView c on tr.contractID = c.contractID
		inner join ContractView uc on tr.underlyingID = uc.contractID
		-- regular option
		left join OptionView o on c.contractID = o.optionID
		left join OptionRootView ort on tr.optionRootID = ort.optionRootID
		-- futures
		left join ContractView fc on tr.futureID = fc.contractID
		left join FutureRoot fr on tr.futureRootID = fr.futureRootID
		left join Future f on tr.futureID = f.futureID
		left join FutureOption fo on tr.contractID = fo.futureOptionID
	where
		c.contractTypeID = 3
		and tr.status in (0, 3) -- only actual (or actual matched) trades should be reported
		and isnull(tr.isPosition, 0) = 0 -- only trades (not positions) should be reported
		and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
		and c.contractID not in (select contractID from TradeView 
									where isnull(isPosition, 0) = 1)
	group by
		c.contractID, c.contractTypeID, c.symbol, c.exportSymbol, o.expiryID,
		o.strike, o.isCall, fo.expiryID, fo.strike, fo.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
		uc.exportSymbol, ort.optionRootID, ort.symbol,
		fr.futureRootID, fr.futureRootSymbol, f.futureID,
		fc.symbol, fc.exportSymbol, f.maturityID
GO

----------------------------------------------------------------------------------------------------
-- usp_BoUnderlyings_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoUnderlyings_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoUnderlyings_Get]
GO

CREATE PROC dbo.usp_BoUnderlyings_Get
as
	------------------------------------------------
	set nocount on
	------------------------------------------------	
	select
		contractID as iContractID, 
		contractTypeID as iContractTypeID, 
		symbol as vcSymbol
	from ContractView
	where contractTypeID < 3
	order by vcSymbol

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoIndex_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoIndex_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoIndex_Del]
GO

CREATE PROC dbo.usp_BoIndex_Del
	@iIndexID int
as
/*
	Description: this procedure is mark as delete information in Index table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0
	
	if exists (select 1 from Trade where contractID = @iIndexID)
	   return -1

	if exists (select 1 from [Order] where contractID = @iIndexID)
	   return -2

	Create table #Options_for_delete(OptionID int)

	insert into #Options_for_delete
		select optionID from [Option] o(nolock)
				join OptionRoot ort on o.optionRootID = ort.optionRootID
 		where underlyingID = @iIndexID

	if exists (select 1 from Trade where contractID in (select optionID from #Options_for_delete))
		return -3

	if exists (select 1 from [Order] where contractID in (select optionID from #Options_for_delete))
		return -4

	Begin Transaction IndexDel

		exec @error = usp_TradeLog_Del @iIndexID
			if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del @bisRemovePermanent = 0
			end

		exec @error = usp_BoFutureRootByUnderlying_Del @iIndexID, 0
			if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

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
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoStock_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoStock_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoStock_Del]
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

----------------------------------------------------------------------------------------------------
-- usp_BoUnderlyingForTradeHistory_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoUnderlyingForTradeHistory_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoUnderlyingForTradeHistory_Get]
GO

CREATE PROC dbo.usp_BoUnderlyingForTradeHistory_Get
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	select 
		c.contractID as iUnderlyingID,
		c.Symbol as vcUnderlyingSymbol
	from TradeView tr 
		join ContractView c on tr.underlyingID = c.contractID
	where 
		isnull(tr.isPosition, 0) = 0
	group by c.contractID, c.Symbol
	order by c.Symbol

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- remove usp_IndexWithPos_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexWithPos_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_IndexWithPos_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_MmIndex_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmIndex_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmIndex_Get]
GO

CREATE PROC dbo.usp_MmIndex_Get
as
	set nocount on

	select 
		c.contractID as iContractID,
		c.symbol as vcSymbol,
		i.isBasket as tiIsBasket
	from ContractView c 	
		inner join IndexView i on i.indexID = c.contractID
	where c.contractTypeID = 1

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_MmIndexDefinition_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmIndexDefinition_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	DROP PROC [dbo].[usp_MmIndexDefinition_Get]
GO

CREATE PROC dbo.usp_MmIndexDefinition_Get
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------

	select
		indexID as iIndexID,
		stockID as iStockID,
		weight as fWeight
	from IndexDefinitionView

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_MmIndexUnderlyingBeta_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmIndexUnderlyingBeta_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	DROP PROC [dbo].[usp_MmIndexUnderlyingBeta_Get]
GO

CREATE PROC dbo.usp_MmIndexUnderlyingBeta_Get
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	select 
		indexID as iIndexID, 
		contractID as iContractID, 
		beta as fBeta
	from IndexBetaView 

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_MmSyntheticIndexBeta_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmSyntheticIndexBeta_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	DROP PROC [dbo].[usp_MmSyntheticIndexBeta_Get]
GO

CREATE PROC dbo.usp_MmSyntheticIndexBeta_Get 
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	select 	
		sor.optionRootID as iOptRootID,
		sib.indexID as iIndexID, 
		sib.beta as fBeta
	from
		SyntheticRootIndexBetaView sib
		inner join SyntheticOptionRootView sor on sor.syntheticOptRootID = sib.syntheticOptRootID

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_MmOptionByUnderlying_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmOptionByUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	DROP PROC [dbo].[usp_MmOptionByUnderlying_Get]
GO

CREATE PROC dbo.usp_MmOptionByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null set @dtMinExpiry = 0

	select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		ec.expiryDate as dtExpiry,
		o.isCall as tiIsCall,
		ort.lotSize as iLotSize,
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol
	from ContractView uc
		inner join OptionRoot ort on uc.contractID = ort.underlyingID
		inner join OptionView o on o.optionRootID = ort.optionRootID
		inner join ContractView c on o.optionID = c.contractID
		inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
	where uc.contractID = @iUnderlyingID
		and ec.expiryDate >= @dtMinExpiry
	--order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_MmUnderlyingExchanges_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmUnderlyingExchanges_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	DROP PROC [dbo].[usp_MmUnderlyingExchanges_Get]
GO

CREATE PROC dbo.usp_MmUnderlyingExchanges_Get
	@iUnderlyingID int
as
	------------------------------------------------
	set nocount on
	------------------------------------------------

	select exchangeID as iExchangeID,
		isUnderlying as tiIsUnderlying,
		isOption as tiIsOption
	from ContractExchangeLinkView
	where contractID = @iUnderlyingID
	
	return @@error
GO

----------------------------------------------------------------------------------------------------
-- remove usp_ContractForTrade_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractForTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	DROP PROC [dbo].[usp_ContractForTrade_Get]
GO

----------------------------------------------------------------------------------------------------
-- remove usp_ContractForTradeByUnd_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractForTradeByUnd_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	DROP PROC [dbo].[usp_ContractForTradeByUnd_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_MmContractForTradeByUnd_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmContractForTradeByUnd_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	DROP PROC [dbo].[usp_MmContractForTradeByUnd_Get]
GO

CREATE PROC dbo.usp_MmContractForTradeByUnd_Get
	@iUnderlyingID int
as
	-- return 2 recordsets: 1st for option contracts, 2nd futures & futures options contracts
	set nocount on

	declare @dtToday datetime

	set @dtToday = cast(floor(cast(getdate() as float)) as datetime)

	-- options rowset
	select	
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		c.contractName as vcContractName, 
		cp.priceClose as fPriceClose,
		cp.priceTheo as fPriceTheoClose,
		
		ec.expiryDate as dtExpiry,
		o.strike as fStrike,
		o.isCall as tiIsCall,
		ort.optionRootID as iOptionRootID
	from OptionRootView ort
		inner join OptionView o on o.optionRootID = ort.optionRootID
		inner join ContractView c on c.contractID = o.optionID
		inner join Contract uc on uc.contractID = ort.underlyingID
		inner join ExpiryInCalendarView ec on ec.expiryID = o.expiryID and ec.expCalendarID = uc.expCalendarID
		left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
	where
		ort.underlyingID = @iUnderlyingID
		and ec.expiryDate >= @dtToday
	order by year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc

	-- futures & futures options rowset
	select * from
	(
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName, 
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			c.symbol as vcFutureSymbol
		from FutureRoot fr
			inner join Future f on f.futureRootID = fr.futureRootID
			inner join ContractView c on c.contractID = f.futureID
			left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
		where
			fr.underlyingID = @iUnderlyingID
		union
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName, 
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
			ec.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			fc.symbol as vcFutureSymbol
		from FutureRoot fr
			inner join Future f on f.futureRootID = fr.futureRootID
			inner join Contract fc on fc.contractID = f.futureID
			inner join FutureOption o on o.futureID = f.futureID
			inner join ContractView c on c.contractID = o.futureOptionID
			inner join ExpiryInCalendarView ec on ec.expiryID = o.expiryID and ec.expCalendarID = fc.expCalendarID
			left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where
			fr.underlyingID = @iUnderlyingID
			and ec.expiryDate >= @dtToday
	) as c order by c.vcFutureSymbol, year(c.dtExpiry), month(c.dtExpiry), c.fStrike, c.tiIsCall desc

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- remove usp_UnderlyingByTrader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	DROP PROC [dbo].[usp_UnderlyingByTrader_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_MmUnderlying_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	DROP PROC [dbo].[usp_MmUnderlying_Get]
GO

CREATE PROC dbo.usp_MmUnderlying_Get
	@iTraderID int = null
AS
	set nocount on

	if isnull(@iTraderID, 0) = 0
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
			case when i.indexID is null then s.calcOptionType else i.calcOptionType end as tiCalcOptionType,
			case when i.indexID is null then s.isHTB else i.isHTB end as tiIsHTB,
		
		    i.yield as fYield,
			i.isBasket as tiIsBasket,
		    s.divFreq as iDivFreq,
		    s.divDate as dtDivDate,
		    s.divAmt as fDivAmt,
		
		    s.isDivCustom as tiIsDivCustom,
		    s.divFreqCustom as iDivFreqCustom,
		    s.divDateCustom as dtDivDateCustom,
		    s.divAmtCustom as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,

			null as iTraderID,

			case when i.indexID is null then s.skew else i.skew end as fSkew,
			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration

		from ContractView c 
		    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		where
			c.contractTypeID < 3
	else
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
			case when i.indexID is null then s.calcOptionType else i.calcOptionType end as tiCalcOptionType,
			case when i.indexID is null then s.isHTB else i.isHTB end as tiIsHTB,
		
		    i.yield as fYield,
			i.isBasket as tiIsBasket,
		    s.divFreq as iDivFreq,
		    s.divDate as dtDivDate,
		    s.divAmt as fDivAmt,
		
		    s.isDivCustom as tiIsDivCustom,
		    s.divFreqCustom as iDivFreqCustom,
		    s.divDateCustom as dtDivDateCustom,
		    s.divAmtCustom as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,

			tc.traderID as iTraderID,

			case when i.indexID is null then s.skew else i.skew end as fSkew,
			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration

		from ContractView c 
		    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
		where
			c.contractTypeID < 3

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_MmFutureRoot_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmFutureRoot_Get]
GO

CREATE PROC dbo.usp_MmFutureRoot_Get
	@iUnderlyingID int = null
AS
	-- retrieve future root(s) by underlying
	set nocount on

	if @iUnderlyingID is null
		select
			fr.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.futureRootName as vcFutureRootName,
			fr.futureLotSize as iFutureLotSize,
			fr.optionLotSize as iOptionLotSize,
			fr.exportSymbol as vcExportSymbol,
			fr.matCalendarID as iMatCalendarID
		from FutureRoot fr
			inner join ContractView uc on fr.underlyingID = uc.contractID
	else
		select
			fr.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.futureRootName as vcFutureRootName,
			fr.futureLotSize as iFutureLotSize,
			fr.optionLotSize as iOptionLotSize,
			fr.exportSymbol as vcExportSymbol,
			fr.matCalendarID as iMatCalendarID
		from FutureRoot fr
			inner join ContractView uc on fr.underlyingID = uc.contractID
		where
			fr.underlyingID = @iUnderlyingID
GO

----------------------------------------------------------------------------------------------------
-- usp_MmFuture_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFuture_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmFuture_Get]
GO

CREATE PROC dbo.usp_MmFuture_Get
	@iFutureRootID int = null
AS
	-- retrieve futures by future root
	set nocount on

	if @iFutureRootID is null
		select
			fc.contractID as iFutureID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcFutureSymbol,
			fc.contractName as vcFutureName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			fc.undPriceProfileID as iUndPriceProfileID,
			fc.optPriceProfileID as iOptPriceProfileID,
			f.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
		    fcp.priceClose as fPriceClose,
			f.maturityID as iMaturityID,
			frmc.expiryDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			case when exists(select top 1 1 from FutureOption where futureID = fc.contractID) then 1 else 0 end as iHaveOptions
		from Future f
			inner join ContractView fc on fc.contractID = f.futureID
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			inner join ContractView uc on uc.contractID = fr.underlyingID
			inner join ExpiryInCalendarView frmc on frmc.expiryID = f.maturityID and frmc.expCalendarID = fr.matCalendarID
		    inner join ContractPrice fcp on fc.contractID = fcp.contractID and fcp.exchangeID is null
	else
		select
			fc.contractID as iFutureID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcFutureSymbol,
			fc.contractName as vcFutureName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			fc.undPriceProfileID as iUndPriceProfileID,
			fc.optPriceProfileID as iOptPriceProfileID,
			f.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
		    fcp.priceClose as fPriceClose,
			f.maturityID as iMaturityID,
			frmc.expiryDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			case when exists(select top 1 1 from FutureOption where futureID = fc.contractID) then 1 else 0 end as iHaveOptions
		from Future f
			inner join ContractView fc on fc.contractID = f.futureID
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			inner join ContractView uc on uc.contractID = fr.underlyingID
			inner join ExpiryInCalendarView frmc on frmc.expiryID = f.maturityID and frmc.expCalendarID = fr.matCalendarID
		    inner join ContractPrice fcp on fc.contractID = fcp.contractID and fcp.exchangeID is null
		where
			f.futureRootID = @iFutureRootID
GO

----------------------------------------------------------------------------------------------------
-- usp_MmFuture_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFuture_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmFuture_Save]
GO

CREATE PROC dbo.usp_MmFuture_Save
	@iFutureID int,
	@vcFutureName varchar(255) = null,
	@iExpCalendarID int = null,
	@tiCalcOptionType tinyint = null,
	@tiIsActive tinyint = null,
	@iUndPriceProfileID int = null,
	@iOptPriceProfileID int = null
AS
	-- update future

	set nocount on
	
	declare @error int		set @error = 0

	exec @error = usp_BoFuture_Save @iFutureID,
									null,
									null,
									@vcFutureName,
									null,
									null,
									null,
									@iExpCalendarID,
									@tiCalcOptionType,
									@tiIsActive,
									@iUndPriceProfileID,
									@iOptPriceProfileID,
									1

	if @@error <> 0 set @error = -20

	return @error
GO

----------------------------------------------------------------------------------------------------
-- remove usp_UnderlyingOptionRoot_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingOptionRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_UnderlyingOptionRoot_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_MmOptionRoot_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmOptionRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmOptionRoot_Get]
GO

CREATE PROC dbo.usp_MmOptionRoot_Get
	@iUnderlyingID int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iUnderlyingID is null
		select
			orv.underlyingID as iUnderlyingID,
			orv.optionRootID as iRootID,
			orv.symbol as vcSymbol,
			orv.IsSynthetic as tiIsSynthetic, 
			orv.lotSize as iLotSize, 
			sor.cashValue as fCash,
			sor.synthSkew as fSyntheticSkew, 
			sor.synthKurt as fSyntheticKurt,
			sor.synthYield as fSyntheticYield, 
			sor.synthIsBasket as tiSyntheticIsBasket
		from OptionRootView orv
			left join SyntheticOptionRootView sor on orv.optionRootID = sor.optionRootID
	else
		select
			orv.underlyingID as iUnderlyingID,
			orv.optionRootID as iRootID,
			orv.symbol as vcSymbol,
			orv.IsSynthetic as tiIsSynthetic, 
			orv.lotSize as iLotSize, 
			sor.cashValue as fCash,
			sor.synthSkew as fSyntheticSkew, 
			sor.synthKurt as fSyntheticKurt,
			sor.synthYield as fSyntheticYield, 
			sor.synthIsBasket as tiSyntheticIsBasket
		from OptionRootView orv
			left join SyntheticOptionRootView sor on orv.optionRootID = sor.optionRootID
		where
			orv.underlyingID = @iUnderlyingID
	
	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_MmSyntheticRootParams_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmSyntheticRootParams_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_MmSyntheticRootParams_Get]
GO

CREATE PROC dbo.usp_MmSyntheticRootParams_Get 
	@iOptionRootID int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iOptionRootID is null
		select 	
			ort.underlyingID as iUnderlyingID,
			sor.optionRootID as iOptionRootID,
			sup.underlyingID as iComponentUnderlyingID, 
			sup.SyntheticCoeff as fWeight
		from SyntheticUnderlyingParamView as sup
			inner join SyntheticOptionRootView as sor on sor.syntheticOptRootID = sup.syntheticOptRootID
			inner join OptionRootView as ort on ort.optionRootID = sor.optionRootID
	else
		select 	
			ort.underlyingID as iUnderlyingID,
			sor.optionRootID as iOptionRootID,
			sup.underlyingID as iComponentUnderlyingID, 
			sup.SyntheticCoeff as fWeight
		from SyntheticUnderlyingParamView as sup
			inner join SyntheticOptionRootView as sor on sor.syntheticOptRootID = sup.syntheticOptRootID
			inner join OptionRootView as ort on ort.optionRootID = sor.optionRootID
		where
			sor.optionRootID = @iOptionRootID

	return @@error
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

	-- find underlying is contract is futures
	if @iUnderlyingID is null
		select @iUnderlyingID = c.contractID,
				@vcUnderlyingSymbol = c.symbol
		from Future f
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			inner join ContractView c on c.contractID = fr.underlyingID
		where
			f.futureID = @iContractID

	-- find underlying is contract is futures option
	if @iUnderlyingID is null
		select @iUnderlyingID = c.contractID,
				@vcUnderlyingSymbol = c.symbol
		from FutureOption fo
			inner join Future f on f.futureID = fo.futureID
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			inner join ContractView c on c.contractID = fr.underlyingID
		where
			fo.futureOptionID = @iContractID

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
-- usp_EodFutureRoot_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFutureRoot_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFutureRoot_Get]
GO

CREATE PROC dbo.usp_EodFutureRoot_Get
AS
	-- retrieve all future roots

	set nocount on

	select
		fr.underlyingID as iIndexID,
		fr.futureRootID as iFutureRootID,
		fr.futureRootSymbol as vcFutureRootSymbol,
		fr.futureRootName as vcFutureRootName
	from FutureRoot fr

GO

----------------------------------------------------------------------------------------------------
-- usp_EodFuture_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFuture_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFuture_Import]
GO

CREATE	PROCEDURE dbo.usp_EodFuture_Import
	@iFutureRootID int,
	@iExpiryID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iLotSizeID int,
	@iOptionLotSizeID int,
	@tiIsActive tinyint output
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	declare	@iFutureID int,
			@iUndPriceProfileID int,
			@iOptPriceProfileID int

	--===================================================================			
	-- select the @iFutureID
	--===================================================================			
	select @iFutureID = futureID, @tiIsActive = isActive
			from Future (nolock)
			where futureRootID = @iFutureRootID and 
				maturityID = @iExpiryID

	if @iFutureID is not null
	begin
		Begin Transaction

			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@iFutureID and actionID<>3
			if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

			update Future set actionDate = getdate() where futureID=@iFutureID
			if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

			update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
			
		Commit Transaction
	end
	else
	begin
		Begin Transaction

			select
				@iUndPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 0

			-- check underlying price profile
			if not exists(select 1 from PriceProfileView where priceProfileID = @iUndPriceProfileID)
			begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end
		
			select
				@iOptPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 1

			-- check options price profile
			if not exists(select 1 from PriceProfileView where priceProfileID = @iOptPriceProfileID)
			begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into Contract
				(contractTypeID, symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
			values 
				(4, @vcSymbol, @vcContractName, 1, @iUndPriceProfileID, @iOptPriceProfileID)

			set @iFutureID = @@identity
			if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into Future (futureID, futureRootID, maturityID)
				 values (@iFutureID, @iFutureRootID, @iExpiryID)
			if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iFutureID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@iFutureID,null)
					if (@@error <> 0) begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end
				end 

			update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -8 if @@TranCount = 1 Rollback Transaction goto finish end
			
			set @tiIsActive = 0

		Commit Transaction
	end 
	 
   set @error = @iFutureID
finish:
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_Option_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Option_Del]
GO

CREATE PROCEDURE dbo.usp_Option_Del
	@iOptionID int = null, 	-- contract identifier
	@bisRemovePermanent bit = 0,
	@bisRemoveFromContract bit = 1,
	@bisRaiseError bit = 1
as
/*
	Create by Zaur Nuraliev
	Description: It deletes Option data from Option and Contract tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if object_ID('tempdb..#Options_for_delete') is null
		begin	
			Create table #Options_for_delete(OptionID int)
		end
	
	if @iOptionID is not null
		begin
			if exists (select tradeID from Trade where contractID = @iOptionID)
				begin
					if @bisRaiseError = 1 
						begin
							Raiserror ('Unable to delete the Option. There is an open position on it.', 16, 1)
						end
					Return (-2)
				end   
			if exists (select 1 from [Order] where contractID = @iOptionID)
				begin
					if @bisRaiseError = 1 
						begin
							Raiserror ('Unable to delete the Option. There is an order on it.', 16, 1)
						end
					Return (-3)
				end   
			insert into #Options_for_delete
				select @iOptionID
		end

	Begin Transaction DelOpt
		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		update ContractExchangeLink set actionID = 3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		--IndexDefinition by contractID
		update IndexBeta set actionID = 3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from DualQuadraticParams where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		update ContractInGroup set actionID=3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete VolaBands where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		update [Option] set actionID=3 where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		update Contract set actionID=3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		update SyntheticUnderlyingParam  set actionID=3 where syntheticOptRootID not in 
			(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		update SyntheticRootIndexBeta  set actionID=3 where syntheticOptRootID not in 
			(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		update SyntheticOptionRoot set actionID=3 where optionRootID not in (select optionRootID from OptionView)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		update OptionRoot set actionID = 3 where optionRootID not in (select optionRootID from OptionView)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end


		if @bisRemovePermanent = 1
			begin
				--IndexBeta by contractID
				delete from IndexBeta where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
				delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
				delete [Option] where OptionID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				if @bisRemoveFromContract = 1 
					begin
						delete Contract where contractID in (select OptionID from #Options_for_delete)
							if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
					end

				delete from SyntheticUnderlyingParam where syntheticOptRootID not in 
					(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from SyntheticRootIndexBeta where syntheticOptRootID not in 
					(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from SyntheticOptionRoot where optionRootID not in (select optionRootID from [Option])
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from OptionRoot where optionRootID not in (select optionRootID from [Option])
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
			end

	Commit Transaction DelOpt

finish:
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_EodFuture_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFuture_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFuture_Del]
GO

CREATE PROC dbo.usp_EodFuture_Del
AS
	set nocount on
	
	declare @error int		set @error = 0

	if object_ID('tempdb..#FutureContractsForDelete') is null
	begin
		set @error = -1
		goto finish
	end

	begin tran
		
	-- delete future option
	delete FutureOption
	where futureID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -2 if @@trancount = 1 rollback tran goto finish end
	
	-- delete future
	delete Future where
	futureID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran goto finish end


	-- delete common future and future options
	
	delete ContractPrice
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -4 if @@trancount = 1 rollback tran goto finish end
	
	update ContractExchangeLink set actionID = 3
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

	update Contract set actionID = 3
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end

	commit tran

finish:
	return @error
GO


----------------------------------------------------------------------------------------------------
-- usp_EodOldFutures_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOldFutures_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOldFutures_Del]
GO
CREATE PROCEDURE dbo.usp_EodOldFutures_Del
	@iFutureRootID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Create table #FutureContractsForDelete(contractID int) 

	Create table #FutureContracts_with_trades(contractID int, symbol varchar(20))

	--=============================================================
	-- select futures & futures options, which have a trades or orders
	--=============================================================
	insert into #FutureContracts_with_trades
		select C.contractID, C.symbol
			from 
				Future F
					join Contract C on F.futureID = C.contractID
					join FutureRoot FR on FR.futureRootID = F.futureRootID
					join ExpiryInCalendar E on F.maturityID = E.expiryID and FR.matCalendarID = E.expCalendarID
			where 
				( 
					( 
						E.expiryDate < GetDate() or datediff(d, F.actionDate, GetDate()) > 0 
					)
					and 
					(
						F.futureID in (select distinct ContractID from Trade) 
						or F.futureID in (select distinct ContractID from [Order])
					)
					and F.futureRootID = @iFutureRootID
				)

	insert into #FutureContracts_with_trades
		select C.contractID, C.symbol
			from 
				FutureOption FO
					join Future F on F.futureID = FO.futureID
					join Contract C on FO.futureOptionID = C.contractID
					join FutureRoot FR on FR.futureRootID = F.futureRootID
					join ExpiryInCalendar E on F.maturityID = E.expiryID and FR.matCalendarID = E.expCalendarID
			where 
				( 
					( 
						E.expiryDate < GetDate() or datediff(d, F.actionDate, GetDate()) > 0 
					)
					and 
					(
						FO.futureOptionID in (select distinct ContractID from Trade) 
						or FO.futureOptionID in (select distinct ContractID from [Order])
					)
					and F.futureRootID = @iFutureRootID
				)

	--=============================================================
	-- select futures & futures options for remove
	--=============================================================
	insert into #FutureContractsForDelete
		select F.futureID
			from 
				Future F
					join Contract C on F.futureID = C.contractID
					join FutureRoot FR on FR.futureRootID = F.futureRootID
					join ExpiryInCalendar E on F.maturityID = E.expiryID and FR.matCalendarID = E.expCalendarID
			where 
				( 
					( 
						E.expiryDate < GetDate() or datediff(d, F.actionDate, GetDate()) > 0 
					)
					and 
					(
						F.futureID not in (select distinct ContractID from Trade) 
						and F.futureID not in (select distinct ContractID from [Order])
					)
					and F.futureRootID = @iFutureRootID
				)

	insert into #FutureContractsForDelete
		select FO.futureOptionID
			from 
				FutureOption FO
					join Future F on F.futureID = FO.futureID
					join Contract C on FO.futureOptionID = C.contractID
					join FutureRoot FR on FR.futureRootID = F.futureRootID
					join ExpiryInCalendar E on F.maturityID = E.expiryID and FR.matCalendarID = E.expCalendarID
			where 
				( 
					( 
						E.expiryDate < GetDate() or datediff(d, F.actionDate, GetDate()) > 0 
					)
					and 
					(
						FO.futureOptionID not in (select distinct ContractID from Trade) 
						and FO.futureOptionID not in (select distinct ContractID from [Order])
					)
					and F.futureRootID = @iFutureRootID
				)

	exec @error = usp_EodFuture_Del

	select symbol as vcSymbol from #FutureContracts_with_trades

finish:
	Return(@error)

GO


----------------------------------------------------------------------------------------------------
-- usp_EodFutureOption_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFutureOption_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFutureOption_Del]
GO

CREATE PROC dbo.usp_EodFutureOption_Del
AS
	set nocount on
	
	declare @error int		set @error = 0

	if object_ID('tempdb..#FutureContractsForDelete') is null
	begin
		set @error = -1
		goto finish
	end

	begin tran
		
	-- delete future option
	delete FutureOption
	where futureOptionID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -1 if @@trancount = 1 rollback tran goto finish end
	
	-- delete common
	
	delete ContractPrice
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -2 if @@trancount = 1 rollback tran goto finish end
	
	
	update Contract set actionID = 3
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran goto finish end

	commit tran

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_EodFutureOption_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFutureOption_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFutureOption_Import]
GO

CREATE	PROCEDURE dbo.usp_EodFutureOption_Import
	@iFutureID int,
	@fStrike float,
	@iExpiryID int,
	@tiIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255)	
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	declare	@iFutureOptionID int

	--===================================================================			
	-- select the @iFutureOptionID
	--===================================================================			
	select @iFutureOptionID = futureOptionID
			from FutureOption (nolock)
			where futureID = @iFutureID and 
				isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID 

	if @iFutureOptionID is not null
	begin
		Begin Transaction

			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@iFutureOptionID and actionID<>3
			if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

			update FutureOption set actionDate = getdate() where futureOptionID=@iFutureOptionID
			if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end
		Commit Transaction
	end
	else
	begin
		Begin Transaction

			insert into Contract (contractTypeID, symbol, contractName)
				 values (5, @vcSymbol,@vcContractName)
			set @iFutureOptionID = @@identity
			if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into FutureOption (futureOptionID, futureID, isCall, expiryID, strike)
				 values (@iFutureOptionID, @iFutureID, @tiIsCall, @iExpiryID, @fStrike)
			if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iFutureOptionID)
			begin
				insert into ContractPrice(contractID,exchangeID)
					values(@iFutureOptionID,null)
				if (@@error <> 0) begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end
			end 

		Commit Transaction
	end 
	 
   set @error = @iFutureOptionID
finish:
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_EodOldFutureOptions_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOldFutureOptions_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOldFutureOptions_Del]
GO
CREATE PROCEDURE dbo.usp_EodOldFutureOptions_Del
	@iFutureID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Create table #FutureContractsForDelete(contractID int) 

	Create table #FutureContracts_with_trades(contractID int, symbol varchar(20))

	--=============================================================
	-- select futures options, which have a trades or orders
	--=============================================================
	insert into #FutureContracts_with_trades
		select C.contractID, C.symbol
			from 
				FutureOption FO
					join Future F on F.futureID = FO.futureID
					join Contract C on FO.futureOptionID = C.contractID
					join FutureRoot FR on FR.futureRootID = F.futureRootID
					join ExpiryInCalendar E on F.maturityID = E.expiryID and FR.matCalendarID = E.expCalendarID
			where 
				( 
					( 
						E.expiryDate < GetDate() or datediff(d, F.actionDate, GetDate()) > 0 
					)
					and 
					(
						FO.futureOptionID in (select distinct ContractID from Trade) 
						or FO.futureOptionID in (select distinct ContractID from [Order])
					)
					and FO.futureID = @iFutureID
				)

	--=============================================================
	-- select futures options for remove
	--=============================================================
	insert into #FutureContractsForDelete
		select FO.futureOptionID
			from 
				FutureOption FO
					join Future F on F.futureID = FO.futureID
					join Contract C on FO.futureOptionID = C.contractID
					join FutureRoot FR on FR.futureRootID = F.futureRootID
					join ExpiryInCalendar E on F.maturityID = E.expiryID and FR.matCalendarID = E.expCalendarID
			where 
				( 
					( 
						E.expiryDate < GetDate() or datediff(d, F.actionDate, GetDate()) > 0 
					)
					and 
					(
						FO.futureOptionID not in (select distinct ContractID from Trade) 
						and FO.futureOptionID not in (select distinct ContractID from [Order])
					)
					and FO.futureID = @iFutureID
				)

	exec @error = usp_EodFutureOption_Del

	select symbol as vcSymbol from #FutureContracts_with_trades

finish:
	Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_EodUnderlying_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodUnderlying_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodUnderlying_Import]
GO
CREATE PROCEDURE dbo.usp_EodUnderlying_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		update Contract
		    set ContractName = @vcContractName,
				MSupdateDate = getDate()
	 	where contractID = @iContractID

		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update Contract Name!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end
	
		delete from TradeLog
		where underlyingID = @iContractID
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t delete record from TradeLog!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

		--========================================================================
		-- make record with null exchange for this stock
		--========================================================================
		if not exists(select * from ContractPrice where exchangeID is null and contractID = @iContractID)
			begin
				insert into ContractPrice(contractID,exchangeID)
					values(@iContractID,null)
				if @@error <> 0 
					begin 
						Raiserror ('Error. Can''t insert default exchange!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish
					end
			end 
		--==================================================================================
		-- update DivAmnt & DivFreq values
		--==================================================================================
		if (@bIsStock <> 0 and exists(select * from StockView where stockID=@iContractID))
			begin 
				Declare @df smallint
				update Stock 
					set divAmt = @fDivAmt,
						divDate = @dtDivDate, 
						divFreq = @iDivFreq
				where stockID = @iContractID
				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update Dividends!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
			end
	Commit Transaction
    set @error = 0
finish:
	Return(@error)
GO
----------------------------------------------------------------------------------------------------
-- usp_EodFuturesWithPositions_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFuturesWithPositions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFuturesWithPositions_Get]
GO
CREATE PROCEDURE dbo.usp_EodFuturesWithPositions_Get
  as
	---------------
	set nocount on
	---------------

	declare @error int		set @error = 0

--=================================================================
-- select futures with positions
--=================================================================
	select 
		FR.underlyingID as iIndexID,
		F.futureID as iFutureID,
		FC.symbol as vcSymbol,
		F.isActive as tiIsActive,
		F.maturityID as iMaturityID,
		sum(T.quantity * T.price * (case when T.isBuy <> 0 then 1 else -1 end)) as fPosition,
		cp.priceClose as fPriceClose
	from TradeView T  
		inner join Future F on T.contractID=F.futureID
		inner join ContractView FC on FC.contractID=T.contractID
		inner join FutureRoot FR on FR.futureRootID=F.futureRootID
		inner join ContractPrice cp on cp.contractID = FC.contractID and cp.exchangeID is null
	group by FR.underlyingID, F.futureID, FC.symbol, F.isActive, F.maturityID, cp.priceClose
	order by FR.underlyingID 
	
set @error = @@error
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_EodFuturesWithOptionPositions_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFuturesWithOptionPositions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFuturesWithOptionPositions_Get]
GO
CREATE PROCEDURE dbo.usp_EodFuturesWithOptionPositions_Get
  as
	---------------
	set nocount on
	---------------

	declare @error int		set @error = 0

--=================================================================
-- select futures with positions
--=================================================================
	select 
		FR.underlyingID as iIndexID,
		F.futureID as iFutureID,
		FC.symbol as vcSymbol,
		F.isActive as tiIsActive,
		F.maturityID as iMaturityID,
		sum(T.quantity * T.price * (case when T.isBuy <> 0 then 1 else -1 end)) as fOptionPosition,
		cp.priceClose as fPriceClose
	from TradeView T  
		inner join FutureOption FO on T.contractID=FO.futureOptionID
		inner join Future F on F.futureID=FO.futureID
		inner join ContractView FC on FC.contractID=F.futureID
		inner join FutureRoot FR on FR.futureRootID=F.futureRootID
		inner join ContractPrice cp on cp.contractID = FC.contractID and cp.exchangeID is null
	group by FR.underlyingID, F.futureID, FC.symbol, F.isActive, F.maturityID, cp.priceClose
	order by FR.underlyingID 
	
set @error = @@error
Return(@error)

GO
----------------------------------------------------------------------------------------------------
-- usp_EodFuturesOptionWithPositions_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFuturesOptionWithPositions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFuturesOptionWithPositions_Get]
GO
CREATE PROCEDURE dbo.usp_EodFuturesOptionWithPositions_Get
  as
	---------------
	set nocount on
	---------------

	declare @error int		set @error = 0

--=================================================================
-- select future option with positions
--=================================================================
	select distinct 
		FO.futureID as iFutureID,
		FC.symbol as vcFutureSymbol,
		FOC.symbol as vcOptionSymbol,
		FO.futureOptionID as iID,
		FO.isCall as tiIsCall,
		FO.strike as fStrike,
		EC.expiryDate as dtExpiry
	from
		TradeView tv  
		inner join FutureOption FO on FO.futureOptionID = tv.contractID
		inner join ContractView FOC on FOC.contractID = tv.contractID
		inner join ContractView FC on FO.futureID = FC.contractID
		inner join ExpiryInCalendarView EC on FO.expiryID = EC.expiryID and EC.expCalendarID = FC.expCalendarID
	where
		ec.expiryDate >= getdate()
	
set @error = @@error
Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_MmFutureExchanges_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureExchanges_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	DROP PROC [dbo].[usp_MmFutureExchanges_Get]
GO

CREATE PROC dbo.usp_MmFutureExchanges_Get
	@iFutureID int
as
	------------------------------------------------
	set nocount on
	------------------------------------------------

	select exchangeID as iExchangeID,
		isUnderlying as tiIsUnderlying,
		isOption as tiIsOption
	from ContractExchangeLinkView
	where contractID = @iFutureID
	
	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_MmOptionByFuture_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmOptionByFuture_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	DROP PROC [dbo].[usp_MmOptionByFuture_Get]
GO

CREATE PROC dbo.usp_MmOptionByFuture_Get
	@iFutureID int,
	@dtMinExpiry datetime = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null set @dtMinExpiry = 0

	select
		foc.contractID as iContractID,
		foc.contractTypeID as iContractTypeID,
		foc.symbol as vcSymbol,
		fo.strike as fStrike, 
		fec.expiryDate as dtExpiry,
		fo.isCall as tiIsCall,
		f.futureID as iFutureID
	from Future f
		inner join ContractView fc on f.futureID = fc.contractID
		inner join FutureOption fo on f.futureID = fo.futureID
		inner join ContractView foc on fo.futureOptionID = foc.contractID
		inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
	where f.futureID = @iFutureID
		and fec.expiryDate >= @dtMinExpiry

	return @@error
GO

/* RESULTS ANALYTHING */
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.45.106')
	begin
		update DataInfo set keyValue = '3.45.106' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.45.106' '3.45.106 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
