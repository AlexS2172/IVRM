/*
	-- BACK UP YOUR DATABASE before running this script
	DB version: 3.36.013
*/
set nocount on
GO
IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#tmpErrors')) DROP TABLE #tmpErrors
GO
CREATE TABLE #tmpErrors (Error int)
GO
SET XACT_ABORT ON
GO
BEGIN TRANSACTION
GO
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
-- Remove foreign key constraints
ALTER TABLE [dbo].[Trade] DROP CONSTRAINT [FK_Trade_Books]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
ALTER TABLE [dbo].[Trade] DROP CONSTRAINT [FK_Trade_Brokers]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
ALTER TABLE [dbo].[Trade] DROP CONSTRAINT [FK_Trade_ClearingBrokers]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
ALTER TABLE [dbo].[Trade] DROP CONSTRAINT [FK_Trade_Commission]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
ALTER TABLE [dbo].[Trade] DROP CONSTRAINT [FK_Trade_Commission1]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
ALTER TABLE [dbo].[Trade] DROP CONSTRAINT [FK_Trade_Contracts]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
ALTER TABLE [dbo].[Trade] DROP CONSTRAINT [FK_Trade_Strategies]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
ALTER TABLE [dbo].[Trade] DROP CONSTRAINT [FK_Trade_Traders]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
-- Script for dbo.Trade
-- Foreign keys etc. will appear at the end
--PRINT 'Updating dbo.Trade'
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO

CREATE TABLE [dbo].[tmp_sc_Trade]
    (
  [seqNum] [int] NOT NULL ,
  [actionDate] [datetime] NOT NULL ,
  [tradeID] [int] NOT NULL ,
  [actionID] [tinyint] NOT NULL ,
  [contractID] [int] NOT NULL ,
  [isClose] [tinyint] NOT NULL ,
  [tradeDate] [datetime] NULL ,
  [quantity] [int] NOT NULL ,
  [price] [float] NOT NULL ,
  [isBuy] [tinyint] NOT NULL ,
  [bookID] [int] NULL ,
  [strategyID] [int] NULL ,
  [traderID] [int] NULL ,
  [brokerID] [int] NULL ,
  [clearingBrokerID] [int] NULL ,
  [brokerCommissionID] [int] NULL ,
  [clearingBrokerCommissionID] [int] NULL ,
  [tradedIV] [float] NULL ,
  [spotReference] [float] NULL ,
  [isPosition] [tinyint] NOT NULL ,
  [lotSize] [int] NULL ,
  [isSimulated] [tinyint] NULL ,
  [execID] [varchar] (20) NULL 
)

GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
if exists (select * from syscolumns where id=OBJECT_ID('[dbo].[tmp_sc_Trade]') and name='actionDate' and cdefault>0) EXEC ('sp_unbindefault ''[dbo].[tmp_sc_Trade].[actionDate]''')
if exists (select * from syscolumns where id=OBJECT_ID('[dbo].[tmp_sc_Trade]') and name='brokerCommissionID' and cdefault>0) EXEC ('sp_unbindefault ''[dbo].[tmp_sc_Trade].[brokerCommissionID]''')
if exists (select * from syscolumns where id=OBJECT_ID('[dbo].[tmp_sc_Trade]') and name='clearingBrokerCommissionID' and cdefault>0) EXEC ('sp_unbindefault ''[dbo].[tmp_sc_Trade].[clearingBrokerCommissionID]''')
if exists (select * from syscolumns where id=OBJECT_ID('[dbo].[tmp_sc_Trade]') and name='isPosition' and cdefault>0) EXEC ('sp_unbindefault ''[dbo].[tmp_sc_Trade].[isPosition]''')
if exists (select * from syscolumns where id=OBJECT_ID('[dbo].[tmp_sc_Trade]') and name='lotSize' and cdefault>0) EXEC ('sp_unbindefault ''[dbo].[tmp_sc_Trade].[lotSize]''')
if exists (select * from syscolumns where id=OBJECT_ID('[dbo].[tmp_sc_Trade]') and name='isSimulated' and cdefault>0) EXEC ('sp_unbindefault ''[dbo].[tmp_sc_Trade].[isSimulated]''')
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
declare @DFName varchar(125)
select @DFName = [name] 
	from sysobjects 
	where parent_obj = object_id('Trade') and xtype = 'D' 
		and id = (select cdefault from syscolumns where id = object_id('Trade') and name = 'actionDate')
if @DFName is not null
	begin
		exec('Alter table Trade Drop constraint ' + @DFName)
	end
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
declare @DFName varchar(125)
select @DFName = [name] 
	from sysobjects 
	where parent_obj = object_id('Trade') and xtype = 'D' 
		and id = (select cdefault from syscolumns where id = object_id('Trade') and name = 'brokerCommissionID')

if @DFName is not null
	begin
		exec('Alter table Trade Drop constraint ' + @DFName)
	end
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
declare @DFName varchar(125)
select @DFName = [name] 
	from sysobjects 
	where parent_obj = object_id('Trade') and xtype = 'D' 
		and id = (select cdefault from syscolumns where id = object_id('Trade') and name = 'clearingBrokerCommissionID')

if @DFName is not null
	begin
		exec('Alter table Trade Drop constraint ' + @DFName)
	end
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
declare @DFName varchar(125)
select @DFName = [name] 
	from sysobjects 
	where parent_obj = object_id('Trade') and xtype = 'D' 
		and id = (select cdefault from syscolumns where id = object_id('Trade') and name = 'isPosition')

if @DFName is not null
	begin
		exec('Alter table Trade Drop constraint ' + @DFName)
	end
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
declare @DFName varchar(125)
select @DFName = [name] 
	from sysobjects 
	where parent_obj = object_id('Trade') and xtype = 'D' 
		and id = (select cdefault from syscolumns where id = object_id('Trade') and name = 'lotSize')

if @DFName is not null
	begin
		exec('Alter table Trade Drop constraint ' + @DFName)
	end
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
declare @DFName varchar(125)
select @DFName = [name] 
	from sysobjects 
	where parent_obj = object_id('Trade') and xtype = 'D' 
		and id = (select cdefault from syscolumns where id = object_id('Trade') and name = 'isSimulated')

if @DFName is not null
	begin
		exec('Alter table Trade Drop constraint ' + @DFName)
	end
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
ALTER TABLE [dbo].[tmp_sc_Trade] WITH NOCHECK
ADD
 CONSTRAINT [DF_Trade_actionDate] DEFAULT (getdate()) FOR [actionDate],
 CONSTRAINT [DF__Trades__BrokerCo__3BFFE745] DEFAULT (0) FOR [brokerCommissionID],
 CONSTRAINT [DF__Trades__Clearing__3CF40B7E] DEFAULT (0) FOR [clearingBrokerCommissionID],
 CONSTRAINT [DF_Trade_isPosition] DEFAULT (0) FOR [isPosition],
 CONSTRAINT [DF_Trade_lotSize] DEFAULT (1) FOR [lotSize],
 CONSTRAINT [DF__Trade__isSimulat__006AEB82] DEFAULT (0) FOR [isSimulated]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
INSERT INTO [dbo].[tmp_sc_Trade] ([seqNum], [actionDate], [tradeID], [actionID], [contractID], [isClose], [tradeDate], [quantity], [price], [isBuy], [bookID], [strategyID], [traderID], [brokerID], [clearingBrokerID], [brokerCommissionID], [clearingBrokerCommissionID], [tradedIV], [spotReference], [isPosition], [lotSize], [isSimulated], [execID]) 
	SELECT [seqNum], [actionDate], [tradeID], [actionID], [contractID], [isClose], [tradeDate], [quantity], [price], [isBuy], [bookID], [strategyID], [traderID], [brokerID], [clearingBrokerID], [brokerCommissionID], [clearingBrokerCommissionID], [tradedIV], [spotReference], [isPosition], [lotSize], [isSimulated], [execID] FROM [dbo].[Trade]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO

if exists (select * from sysobjects where id=object_id('[dbo].[Trade]') and OBJECTPROPERTY(id, 'IsUserTable')=1)
  drop table [dbo].[Trade]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
sp_rename 'dbo.tmp_sc_Trade', 'Trade'
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF NOT EXISTS (SELECT * FROM SYSINDEXES WHERE ID =  object_id('Trade') AND NAME = 'I_Trades_Contract')
	CREATE  INDEX [I_Trades_Contract] ON [dbo].[Trade]([contractID]) ON [PRIMARY]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF NOT EXISTS (SELECT * FROM SYSINDEXES WHERE ID =  object_id('Trade') AND NAME = 'I_Trades_Book')
	CREATE  INDEX [I_Trades_Book] ON [dbo].[Trade]([bookID]) ON [PRIMARY]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF NOT EXISTS (SELECT * FROM SYSINDEXES WHERE ID =  object_id('Trade') AND NAME = 'I_Trades_Broker')
 CREATE  INDEX [I_Trades_Broker] ON [dbo].[Trade]([brokerID]) ON [PRIMARY]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF NOT EXISTS (SELECT * FROM SYSINDEXES WHERE ID =  object_id('Trade') AND NAME = 'I_Trades_ClearingBroker')
	CREATE  INDEX [I_Trades_ClearingBroker] ON [dbo].[Trade]([clearingBrokerID]) ON [PRIMARY]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF NOT EXISTS (SELECT * FROM SYSINDEXES WHERE ID =  object_id('Trade') AND NAME = 'I_Trades_Strategy')
 CREATE  INDEX [I_Trades_Strategy] ON [dbo].[Trade]([strategyID]) ON [PRIMARY]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF NOT EXISTS (SELECT * FROM SYSINDEXES WHERE ID =  object_id('Trade') AND NAME = 'I_Trades_TradeDate')
 CREATE  INDEX [I_Trades_TradeDate] ON [dbo].[Trade]([tradeDate]) ON [PRIMARY]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF NOT EXISTS (SELECT * FROM SYSINDEXES WHERE ID =  object_id('Trade') AND NAME = 'traderID_ind')
	CREATE  INDEX [traderID_ind] ON [dbo].[Trade]([traderID]) ON [PRIMARY]
GO
if @@error<>0 and @@trancount>0 rollback transaction
GO
if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF NOT EXISTS (SELECT * FROM SYSINDEXES WHERE ID =  object_id('Trade') AND NAME = 'tradeID_contractID_ind')
	CREATE  INDEX [tradeID_contractID_ind] ON [dbo].[Trade]([tradeID], [contractID]) ON [PRIMARY]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF NOT EXISTS (SELECT * FROM SYSINDEXES WHERE ID =  object_id('Trade') AND NAME = 'actionDate_ind')
 CREATE  INDEX [actionDate_ind] ON [dbo].[Trade]([actionDate]) ON [PRIMARY]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF OBJECT_ID('CHK_IsBuy') IS NULL
	ALTER TABLE [dbo].[Trade] ADD CONSTRAINT [CHK_IsBuy] CHECK ([isBuy] = 1 or [isBuy] = 0)
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF OBJECT_ID('CHK_IsClose') IS NULL
	ALTER TABLE [dbo].[Trade] ADD CONSTRAINT [CHK_IsClose] CHECK ([isClose] = 1 or [isClose] = 0)
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF OBJECT_ID('PK_Trade') IS NULL
	ALTER TABLE [dbo].[Trade] WITH NOCHECK ADD  CONSTRAINT [PK_Trade] PRIMARY KEY CLUSTERED ([seqNum]) ,
		CONSTRAINT [IX_tradeID_actionID] UNIQUE NONCLUSTERED ([tradeID], [actionID]) 
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
-- Add foreign key constraints
IF OBJECT_ID('FK_Trade_Books') IS NULL
	ALTER TABLE [dbo].[Trade] WITH NOCHECK ADD CONSTRAINT [FK_Trade_Books] FOREIGN KEY ([bookID])  REFERENCES [dbo].[Book] ([bookID]) 
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
--*******************************************
	ALTER TABLE [dbo].[Trade] CHECK CONSTRAINT [FK_Trade_Books]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF OBJECT_ID('FK_Trade_Brokers') IS NULL
	ALTER TABLE [dbo].[Trade] WITH NOCHECK ADD CONSTRAINT [FK_Trade_Brokers] FOREIGN KEY ([brokerID])  REFERENCES [dbo].[Broker] ([brokerID]) 
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
ALTER TABLE [dbo].[Trade] CHECK CONSTRAINT [FK_Trade_Brokers]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF OBJECT_ID('FK_Trade_ClearingBrokers') IS NULL
	ALTER TABLE [dbo].[Trade] WITH NOCHECK ADD CONSTRAINT [FK_Trade_ClearingBrokers] FOREIGN KEY ([clearingBrokerID])  REFERENCES [dbo].[Broker] ([brokerID]) 
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
ALTER TABLE [dbo].[Trade] CHECK CONSTRAINT [FK_Trade_ClearingBrokers]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF OBJECT_ID('FK_Trade_Commission') IS NULL
	ALTER TABLE [dbo].[Trade] WITH NOCHECK ADD CONSTRAINT [FK_Trade_Commission] FOREIGN KEY ([brokerCommissionID])  REFERENCES [dbo].[Commission] ([commissionID]) 
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
ALTER TABLE [dbo].[Trade] CHECK CONSTRAINT [FK_Trade_Commission]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF OBJECT_ID('FK_Trade_Commission1') IS NULL
	ALTER TABLE [dbo].[Trade] WITH NOCHECK ADD CONSTRAINT [FK_Trade_Commission1] FOREIGN KEY ([clearingBrokerCommissionID])  REFERENCES [dbo].[Commission] ([commissionID]) 
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
ALTER TABLE [dbo].[Trade] CHECK CONSTRAINT [FK_Trade_Commission1]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF OBJECT_ID('FK_Trade_Contracts') IS NULL
	ALTER TABLE [dbo].[Trade] WITH NOCHECK ADD CONSTRAINT [FK_Trade_Contracts] FOREIGN KEY ([contractID])  REFERENCES [dbo].[Contract] ([contractID]) 
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
ALTER TABLE [dbo].[Trade] CHECK CONSTRAINT [FK_Trade_Contracts]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF OBJECT_ID('FK_Trade_Strategies') IS NULL
	ALTER TABLE [dbo].[Trade] WITH NOCHECK ADD CONSTRAINT [FK_Trade_Strategies] FOREIGN KEY ([strategyID])  REFERENCES [dbo].[Strategy] ([strategyID]) 
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
ALTER TABLE [dbo].[Trade] CHECK CONSTRAINT [FK_Trade_Strategies]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
IF OBJECT_ID('FK_Trade_Traders') IS NULL
	ALTER TABLE [dbo].[Trade] WITH NOCHECK ADD CONSTRAINT [FK_Trade_Traders] FOREIGN KEY ([traderID])  REFERENCES [dbo].[Trader] ([traderID]) 
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO
ALTER TABLE [dbo].[Trade] CHECK CONSTRAINT [FK_Trade_Traders]
GO
	if @@error<>0 and @@trancount>0 rollback transaction
GO
	if @@trancount=0 begin insert into #tmperrors (error) select 1 begin transaction end
GO

IF EXISTS (SELECT * FROM #tmpErrors) ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT>0 BEGIN
--PRINT 'The database update succeeded'
COMMIT TRANSACTION
END
ELSE PRINT 'The database update failed!'
GO
DROP TABLE #tmpErrors
GO
--=============================================================
--changes of SP's:
--=============================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_New]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Clear]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_New]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_Update]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE        proc dbo.usp_TradeFeed_Del
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
				lotSize,
				isSimulated,
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
				lotSize,
				isSimulated,
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
							tr.lotSize as iTradeLotSize,
							cp.lotSize as iUnderlyingLotSize,
							cp.lotSize as iLotSize,
							tr.isSimulated as tiIsSimulated,
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
							tr.lotSize as iTradeLotSize,
							ucp.lotSize as iUnderlyingLotSize,
							cp.lotSize as iLotSize,
							tr.isSimulated as tiIsSimulated,
							cp.PriceClose as fPriceClose,
							ucp.priceClose as fUnderlyingPriceClose
						from Trade tr 
							join Contract C  on tr.contractID = c.contractID
							inner join [Option] O on C.contractID = O.optionID
							inner join Contract UC on O.underlyingContractID = UC.contractID
							inner join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
							inner join ContractPrice UCP on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
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

CREATE     PROCEDURE dbo.usp_TradeFeed_New
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
	@iTradeLotSize int = null
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

			if @iTradeLotSize is null set @iTradeLotSize = 1
			insert into ContractPrice (contractID,exchangeID,lotSize) values(@iUnderlyingContractID,null,@iTradeLotSize)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			set @iStructureMissed = 1

			insert into TradeLog (contractID, eventTypeID) values (@iUnderlyingContractID, 0)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end


/*			insert into Contract(symbol,contractTypeID,expCalendarID) values(@vcUnderlyingSymbol,@iContractTypeID,1)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iUnderlyingContractID = @@identity
			if @iContractTypeID = 1
				begin
					insert into [Index](indexID) values(@iUnderlyingContractID)
						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else if @iContractTypeID = 2
				begin
					insert into Stock (stockID) values(@iUnderlyingContractID)
						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			if @iContractTypeID < 3 
				begin	
					if @iTradeLotSize is null set @iTradeLotSize = 1
					insert into ContractPrice (contractID,exchangeID,lotSize) values(@iUnderlyingContractID,null,@iTradeLotSize)
						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else
				insert into ContractPrice (contractID,exchangeID) values(@iUnderlyingContractID,null)
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end*/
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
	
			select @iOptionID = optionID
				from OptionView (nolock)
				where underlyingContractID = @iUnderlyingContractID and 
					isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3
	
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
				
						insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
							 values (@iOptionID, @iUnderlyingContractID, @tiIsCall, @fStrike, @iExpiryID)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				
						-- make record with null exchange for this Option
						insert into ContractPrice(contractID,exchangeID,lotSize) values(@iOptionID,null,@iTradeLotSize)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

						insert into TradeLog (contractID, eventTypeID) values (@iOptionID, 0)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
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
	if @iTradeLotSize is null
		select @iTradeLotSize = lotSize from ContractPrice 
			where exchangeID is null and 
					(contractID = @iOptionID and @iContractTypeID = 3) or
					(contractID = @iUnderlyingContractID and @iContractTypeID < 3)

	if @iTradeLotSize is null
		if @iContractTypeID = 3 
			set @iTradeLotSize = 100
		else if @iContractTypeID < 3 
			set @iTradeLotSize = 1

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
		lotSize,
		isSimulated,
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
		@iTradeLotSize,
		0,
		null,
		null,
		@iSeqNum)

		--set @iSeqNum = @@identity

		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

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
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iUnderlyingLotSize,
			cp.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated,
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
			tr.lotSize as iTradeLotSize,
			ucp.lotSize as iUnderlyingLotSize,
			cp.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated,
			CP.PriceClose as fPriceClose,
			UCP.priceClose as fUnderlyingPriceClose,
			@iStructureMissed as iStructureMissed
		from Trade tr 
			join Contract C  on tr.contractID = c.contractID
			inner join [Option] O on C.contractID = O.optionID
			inner join Contract UC on O.underlyingContractID = UC.contractID
			inner join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
			inner join ContractPrice UCP on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
			join ExpiryInCalendarView EC on UC.expCalendarID = EC.expCalendarID and O.expiryID = EC.expiryID
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
SET ANSI_NULLS OFF 
GO


CREATE      PROCEDURE dbo.usp_Trade_Clear
	@iContractID int = null,
	@tiOnlyWithExecID tinyint = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction
		delete from Trade 
		where tradeID in (select tradeID from Trade where isSimulated = 1 and actionID = 0) 
			and (@iContractID is null or contractID = @iContractID)
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
	
		if isnull(@tiOnlyWithExecID, 0) = 0
		begin
			delete from Trade 
			where (@iContractID is null or contractID = @iContractID) 
				and isnull(isSimulated, 0) = 0
	
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Tran goto finish end
		end
		else
		begin
			delete from Trade 
			where (@iContractID is null or contractID = @iContractID) 
				and isnull(isSimulated, 0) = 0 
				and isnull(execID, '') <> ''
	
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
		end
		set @error = @@error
	Commit Transaction

finish:
	return @error


/*
			where tradeID in (select tradeID from Trade where isSimulated = 1 and actionID = 0) 
				and (@iContractID is null or contractID = @iContractID)

	Begin Transaction
		delete from Trade 

		if isnull(@tiOnlyWithExecID, 0) = 0
			begin
				if isnull(@iContractID, 0) = 0
					--delete from Trade where isnull(isSimulated, 0) = 0
					delete from Trade where tradeID in (select tradeID from Trade where isSimulated = 1 and actionID = 0)
						if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
				else
					--delete from Trade where contractID = @iContractID and isnull(isSimulated, 0) = 0
					delete from Trade 
						where tradeID in (select tradeID from Trade where isSimulated = 1 and actionID = 0) and contractID = @iContractID 
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
			end
		else
			begin
				if isnull(@iContractID, 0) = 0
					--delete from Trade where isnull(isSimulated, 0) = 0 and isnull(execID, '') <> ''
					delete from Trade 
						where tradeID in (select tradeID from Trade where isSimulated = 1 and actionID = 0) and isnull(execID, '') <> ''
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
				else
					--delete from Trade where contractID = @iContractID and isnull(isSimulated, 0) = 0 and isnull(execID, '') <> ''
					delete from Trade 
						where tradeID in (select tradeID from Trade where isSimulated = 1 and actionID = 0)
							and  contractID = @iContractID and isnull(execID, '') <> ''
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
			end

		set @error = @@error
	Commit Transaction

finish:
	Return(@error)
*/



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE     proc dbo.usp_Trade_Del
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
				lotSize,
				isSimulated,
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
				lotSize,
				isSimulated,
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE  procedure dbo.usp_Trade_New
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
	@iLotSize int = Null,
	@tiIsSimulated tinyint = Null
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
		lotSize,
		isSimulated,
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
		@iLotSize,
		@tiIsSimulated,
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


CREATE     proc usp_Trade_Update
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
	@iLotSize int = Null,
	@tiIsSimulated tinyint = null
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
				lotSize,
				isSimulated,
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
				lotSize,
				isSimulated,
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
			lotSize,
			isSimulated,
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
			@iLotSize,
			@tiIsSimulated,
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
--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.36.013')
	begin
		update DataInfo set keyValue = '3.36.013' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.36.013'
	end
