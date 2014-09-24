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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '1.01.001' or (keyValue = '1.01.002')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '1.01.002 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '1.01.002' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

----------------------------------------------------------------------------------------------------
-- Trade table
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Trade]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[Trade]
GO

CREATE TABLE [dbo].[Trade] (
	[iSeqID] [int] IDENTITY (1, 1) NOT NULL ,
	[vcAcronym] [varchar] (8) NOT NULL , 
	[nTradeID] [numeric](9, 0) NOT NULL CONSTRAINT [DF_Trade_nTradeID] DEFAULT (0),
	[siStatus] [smallint] NULL CONSTRAINT [DF_Trade_siStatus] DEFAULT (0),
	[vcSymbol] [varchar] (15) NOT NULL ,
	[vcAction] [varchar] (15) NOT NULL ,
	[fUnits] [float] NOT NULL CONSTRAINT [DF_Trade_fUnits] DEFAULT (0),
	[fPrice] [float] NOT NULL CONSTRAINT [DF_Trade_fPrice] DEFAULT (0),
	[dtTradeDate] [datetime] NOT NULL ,
	[vcHedgeGroup] [varchar] (15) NULL ,
	[dtUpdateDate] [datetime] NOT NULL CONSTRAINT [DF_Trade_dtUpdateDate] DEFAULT (getdate()),
	[siProcessedStatus] [smallint] NOT NULL CONSTRAINT [DF_Trade_bUsed] DEFAULT (0)
) ON [PRIMARY]
GO


----------------------------------------------------------------------------------------------------
-- TradeSnapShot table
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TradeSnapShot]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
	drop table [dbo].[TradeSnapShot]
GO

CREATE TABLE [dbo].[TradeSnapShot] (
	[vcSymbol] [varchar] (15) NOT NULL ,
	[vcAction] [varchar] (15) NOT NULL ,
	[vcActionType] [varchar] (1) NULL ,
	[fUnits] [float] NOT NULL CONSTRAINT [DF_Trade_SnapShot_fUnits] DEFAULT (0),
	[fPrice] [float] NOT NULL CONSTRAINT [DF_Trade_SnapShot_fPrice] DEFAULT (0),
	[dtTradeDate] [datetime] NOT NULL ,
	[dtSettleDate] [datetime] NOT NULL ,
	[vcBrokerID] [varchar] (15) NOT NULL ,
	[vcTradedSymbol] [varchar] (10) NOT NULL ,
	[vcComment] [varchar] (100) NULL ,
	[vcHedgeGroup] [varchar] (15) NULL ,
	[nBasketID] [int] NULL CONSTRAINT [DF_Trade_SnapShot_nBasketID] DEFAULT (0),
	[fSecFee] [float] NULL CONSTRAINT [DF_Trade_SnapShot_fSecFee] DEFAULT (0),
	[fComm] [float] NULL ,
	[nTradeID] [numeric](18, 0) NOT NULL CONSTRAINT [DF_Trade_SnapShot_nTradeID] DEFAULT (0),
	[nNetTradeID] [int] NULL CONSTRAINT [DF_Trade_SnapShot_nNetTradeID] DEFAULT (0),
	[vcBranchCode] [varchar] (3) NULL ,
	[nSeqNumber] [int] NULL ,
	[dtOrderEntryTime] [datetime] NULL ,
	[dtOrderFillTime] [datetime] NULL ,
	[vcOrderActionType] [varchar] (15) NULL ,
	[vcOrderLimitType] [varchar] (15) NULL ,
	[vcOrderLifeType] [varchar] (15) NULL ,
	[vcOrderacctType] [varchar] (15) NULL ,
	[vcOrderEntryMethod] [varchar] (15) NULL ,
	[vcOrderClass] [varchar] (15) NULL ,
	[vcOrderExchCode] [varchar] (1) NULL ,
	[vcTradeRecType] [varchar] (3) NOT NULL ,
	[vcDataSource] [varchar] (15) NULL ,
	[siStatusFlag] [smallint] NULL CONSTRAINT [DF_Trade_SnapShot_siStatusFlag] DEFAULT (0),
	[vcAcronym] [varchar] (8) NOT NULL 
) ON [PRIMARY]
GO


----------------------------------------------------------------------------------------------------
-- sp_Trade_Insert
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[sp_Trade_Insert]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[sp_Trade_Insert]
GO

CREATE PROC dbo.sp_Trade_Insert
	@vcAcronym varchar(8),
	@nTradeID numeric(9, 0),
	@siStatus smallint,
	@vcSymbol varchar(15),
	@vcAction varchar(15),
	@fUnits float,
	@fPrice float,
	@dtTradeDate datetime,
	@vcHedgeGroup varchar(15)
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	begin transaction
		
	insert into Trade
		(vcAcronym, nTradeID, siStatus, vcSymbol, vcAction, fUnits, fPrice, dtTradeDate, vcHedgeGroup)
	values
		(@vcAcronym, @nTradeID, @siStatus, @vcSymbol, @vcAction, @fUnits, @fPrice, @dtTradeDate, @vcHedgeGroup)

	if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

	commit transaction

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- sp_Trade_ActiveProcessed_Select
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[sp_Trade_ActiveProcessed_Select]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[sp_Trade_ActiveProcessed_Select]
GO

CREATE PROC dbo.sp_Trade_ActiveProcessed_Select
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	-- select latest processed updates of trades without deleted trades
	select 
		t.iSeqID as iSeqID,
		t.vcAcronym as vcAcronym,
		t.nTradeID as nTradeID,
		2 as siStatus,
		t.vcSymbol as vcSymbol,
		t.vcAction as vcAction,
	             CASE t.vcAction WHEN 'B' THEN 1 ELSE 0 END as 'tiIsBuy',
		t.fUnits as fUnits,
		t.fPrice as fPrice,
		t.dtTradeDate as dtTradeDate,
		t.vcHedgeGroup as vcHedgeGroup
	from 
		Trade t
             where 	
		t.siStatus != 2 and 
		t.iSeqID in (
				select  max (tt.iSeqID)
				from Trade tt 
				where
					tt.siProcessedStatus = 1 /* successfully processed */  
				group by tt.nTradeID
			     )
	
finish:
   	set nocount off
	return @@error
GO

----------------------------------------------------------------------------------------------------
-- sp_Trade_Select
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[sp_Trade_Select]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[sp_Trade_Select]
GO

CREATE PROC dbo.sp_Trade_Select
@top int = 0
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	set rowcount @top

	select
		vcAcronym,
		nTradeID,
		siStatus,
		vcSymbol,
		vcAction,
		fUnits,
		fPrice,
		dtTradeDate,
		vcHedgeGroup,
		dtUpdateDate
	from Trade

finish:
	return @@error
GO

----------------------------------------------------------------------------------------------------
-- sp_Trade_Select_UnProcessed
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[sp_Trade_Select_UnProcessed]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[sp_Trade_Select_UnProcessed]
GO

CREATE   PROC dbo.sp_Trade_Select_UnProcessed
@top int = null
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	set rowcount @top

	select  iSeqID,
		vcAcronym,
		nTradeID,
		siStatus,
		vcSymbol,
		vcAction,
	        CASE vcAction
                WHEN 'B' THEN 1
                ELSE 0
        	END AS tiIsBuy,
		fUnits,
		fPrice,
		dtTradeDate,
		vcHedgeGroup
	from Trade
	where siProcessedStatus = 0
	order by iSeqID ASC
finish:
	set rowcount 0
	return @@error
GO


----------------------------------------------------------------------------------------------------
-- sp_TradeSnapShot_Select
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[sp_TradeSnapShot_Select]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[sp_TradeSnapShot_Select]
GO

CREATE PROC dbo.sp_TradeSnapShot_Select
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select  
		-1 as 'iSeqID',
		vcAcronym,
		nTradeID,
		0 as 'siStatus',
		vcSymbol,
		vcAction,
	        CASE vcAction WHEN 'B' THEN 1 ELSE 0 END as 'tiIsBuy',
		fUnits,
		fPrice,
		dtTradeDate,
		vcHedgeGroup
	from TradeSnapShot

finish:
	return @@error
GO

----------------------------------------------------------------------------------------------------
-- sp_TradeSnapShot_Select
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[sp_TradeSnapShot_Insert]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[sp_TradeSnapShot_Insert]
GO

CREATE PROC dbo.sp_TradeSnapShot_Insert
	@vcSymbol varchar(15) ,
	@vcAction varchar(15),
	@vcActionType varchar(1) = NULL ,
	@fUnits float = 0,
	@fPrice float = 0,
	@dtTradeDate datetime ,
	@dtSettleDate datetime ,
	@vcBrokerID varchar(15)  ,
	@vcTradedSymbol varchar(10)  ,
	@vcComment varchar(100) = NULL ,
	@vcHedgeGroup varchar(15) = NULL ,
	@vcAcronym varchar(8),
	@nBasketID int = 0,
	@fSecFee float = 0,
	@fComm float = NULL ,
	@nTradeID numeric(18, 0) = 0,
	@nNetTradeID int = 0,
	@vcBranchCode varchar(3) = NULL ,
	@nSeqNumber int = NULL ,
	@dtOrderEntryTime datetime = NULL ,
	@dtOrderFillTime datetime = NULL ,
	@vcOrderActionType varchar(15) = NULL ,
	@vcOrderLimitType varchar(15) = NULL ,
	@vcOrderLifeType varchar(15) = NULL ,
	@vcOrderacctType varchar(15) = NULL ,
	@vcOrderEntryMethod varchar(15) = NULL ,
	@vcOrderClass varchar(15) = NULL ,
	@vcOrderExchCode varchar(1) = NULL ,
	@vcTradeRecType varchar(3),
	@vcDataSource varchar(15) = NULL ,
	@siStatusFlag smallint = 0
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	begin transaction
			
	insert into TradeSnapShot 
		([vcSymbol], [vcAction], [vcActionType], [fUnits], [fPrice], [dtTradeDate], [dtSettleDate], [vcBrokerID], [vcTradedSymbol], [vcComment], [vcHedgeGroup], [vcAcronym], [nBasketID], [fSecFee], [fComm], [nTradeID], [nNetTradeID], [vcBranchCode], [nSeqNumber], [dtOrderEntryTime], [dtOrderFillTime], [vcOrderActionType], [vcOrderLimitType], [vcOrderLifeType], [vcOrderacctType], [vcOrderEntryMethod], [vcOrderClass], [vcOrderExchCode], [vcTradeRecType], [vcDataSource], [siStatusFlag])
	values
		(@vcSymbol, @vcAction, @vcActionType, @fUnits, @fPrice, @dtTradeDate, @dtSettleDate, @vcBrokerID, @vcTradedSymbol, @vcComment, @vcHedgeGroup, @vcAcronym, @nBasketID, @fSecFee, @fComm, @nTradeID, @nNetTradeID, @vcBranchCode, @nSeqNumber, @dtOrderEntryTime, @dtOrderFillTime, @vcOrderActionType, @vcOrderLimitType, @vcOrderLifeType, @vcOrderacctType, @vcOrderEntryMethod, @vcOrderClass, @vcOrderExchCode, @vcTradeRecType, @vcDataSource, @siStatusFlag)

	if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

	commit transaction

finish:
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
-- change version of DB
--=============================================================================================
if not exists (select * from DataInfo where DataInfoID = 1 and keyValue = '1.01.002')
	begin
		update DataInfo set keyValue = '1.01.002' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 1.01.002' '1.01.002 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
