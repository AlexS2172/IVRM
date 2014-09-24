/*
@PatchDecription
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.43.093' or (keyValue = '3.43.094')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.43.094 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.43.094' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Trade_ClearDeleted' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Trade_ClearDeleted
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE   PROCEDURE dbo.usp_Trade_ClearDeleted 
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	Begin Transaction Trade

		delete from TradeLog 
		where seqNum in (select seqNum
						from Trade 
						where tradeID in (select tradeID from Trade where actionID = 0))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

		delete from Trade where tradeID in (select tradeID from Trade where actionID = 0) 
			if @@error !=0 begin set @error = 2 if @@TranCount = 1 Rollback Tran goto finish end

		set @error = @@error

		delete from TradeLog 
		where seqNum in (select seqNum
						from Trade T join [Option] O on O.optionID = T.contractID
						join OptionRoot ORt on O.optionRootID = ORt.optionRootID
						join Contract UC on ORt.underlyingID = UC.contractID 
						join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
						where E.expiryDate < GetDate() )
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

		delete Trade from Trade T join [Option] O on O.optionID = T.contractID
						join OptionRoot ORt on O.optionRootID = ORt.optionRootID
						join Contract UC on ORt.underlyingID = UC.contractID 
						join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
						where E.expiryDate < GetDate() 
		if @@error !=0 begin set @error = 2 if @@TranCount = 1 Rollback Tran goto finish end

		set @error = @@error

	Commit Transaction Trade

finish:
Return(@error)


GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_ContractPriceEOD_Init_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_ContractPriceEOD_Init_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOptClosePrice_Init_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOptClosePrice_Init_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOptTheoPrice_Init_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOptTheoPrice_Init_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUndClosePrice_Init_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUndClosePrice_Init_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodVolaBands_Init_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodVolaBands_Init_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_VolaBandsEOD_Init_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_VolaBandsEOD_Init_Save
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    procedure dbo.usp_ContractPriceEOD_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000)

	set @vcTableName = '##ETSEOD' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	--number of minutes of EOD timeout
	declare @dtEODTimeOut varchar(80)

	select @dtEODTimeOut = keyValue from DataInfo where dataInfoID = 7
	if @dtEODTimeOut is null
		begin
			set @dtEODTimeOut = '10'
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			set @vcExecString1 = 'if exists(select * from ' + @vcTableName + ' where DATEDIFF(mi,actionDate,GetDate()) >' + @dtEODTimeOut + ') drop table ' + @vcTableName 
			--select @vcExecString1
			exec(@vcExecString1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
			Return (-1) 
		end

	exec('create table ' + @vcTableName + '(
		rowID int identity(1,1),
		symbol varchar(20) null,
		contractTypeID int not null,
		priceClose float not null,
		exchangeCode varchar(10) null,
		exchangeID int null,
		contractID int null,
		actionDate smalldatetime default GetDate())')

	--symbol, exchangeCode, contractTypeID
	exec('ALTER TABLE ' + @vcTableName + ' ADD CONSTRAINT PK_EOD PRIMARY KEY CLUSTERED (rowID)')

	exec('CREATE NONCLUSTERED INDEX IND1_EOD ON ' + @vcTableName + ' ([symbol],exchangeCode,contractTypeID)')

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



CREATE         procedure dbo.usp_EodOptClosePrice_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	update ContractPrice
		set priceClose = null
		from (select contractPriceID from ContractPrice CP 
		right join TradeView T on T.contractID = CP.contractID
		join OptionView O on O.optionID = T.contractID
		where CP.exchangeID is null) as CP2
		where ContractPrice.contractPriceID=CP2.contractPriceID

	set @error = @@error
	if @error<>0 goto finish

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcConstrName varchar(8000),
			@vcIndexName varchar(8000),
			@vcExecString1 varchar(8000)

	set @vcTableName = '##ETSEODOP' + + cast(db_name() as varchar(8000))
	set @vcConstrName = 'PK_ETSEODOP' + cast(db_name() as varchar(8000))
	set @vcIndexName = 'IND1_ETSEODOP' + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	--number of minutes of EOD timeout
	declare @dtEODTimeOut varchar(80)

	select @dtEODTimeOut = keyValue from DataInfo where dataInfoID = 7
	if @dtEODTimeOut is null
		begin
			set @dtEODTimeOut = '10'
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			set @vcExecString1 = 'if exists(select * from ' + @vcTableName + ' where DATEDIFF(mi,actionDate,GetDate()) >' + @dtEODTimeOut + ') drop table ' + @vcTableName 
			exec(@vcExecString1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
			Return (-1) 
		end

	exec('create table ' + @vcTableName + '(
		rowID int identity(1,1),
		contractID int not null,
		priceClose float not null,
		actionDate smalldatetime default GetDate())')

	exec('ALTER TABLE ' + @vcTableName + ' ADD CONSTRAINT ' + @vcConstrName + ' PRIMARY KEY CLUSTERED (rowID)')

	exec('CREATE NONCLUSTERED INDEX ' + @vcIndexName + ' ON ' + @vcTableName + ' (contractID)')

	set @error = @@error
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


CREATE         procedure dbo.usp_EodOptTheoPrice_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcConstrName varchar(8000),
			@vcIndexName varchar(8000),
			@vcExecString1 varchar(8000)

	set @vcTableName = '##ETSEODTP' + + cast(db_name() as varchar(8000))
	set @vcConstrName = 'PK_ETSEODTP' + cast(db_name() as varchar(8000))
	set @vcIndexName = 'IND1_ETSEODTP' + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	--number of minutes of EOD timeout
	declare @dtEODTimeOut varchar(80)

	select @dtEODTimeOut = keyValue from DataInfo where dataInfoID = 7
	if @dtEODTimeOut is null
		begin
			set @dtEODTimeOut = '10'
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			set @vcExecString1 = 'if exists(select * from ' + @vcTableName + ' where DATEDIFF(mi,actionDate,GetDate()) >' + @dtEODTimeOut + ') drop table ' + @vcTableName 
			exec(@vcExecString1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
			Return (-1) 
		end

	exec('create table ' + @vcTableName + '(
		rowID int identity(1,1),
		contractID int not null,
		priceTheo float not null,
		actionDate smalldatetime default GetDate())')

	exec('ALTER TABLE ' + @vcTableName + ' ADD CONSTRAINT ' + @vcConstrName + ' PRIMARY KEY CLUSTERED (rowID)')

	exec('CREATE NONCLUSTERED INDEX ' + @vcIndexName + ' ON ' + @vcTableName + ' (contractID)')

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


CREATE       procedure dbo.usp_EodUndClosePrice_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	update ContractPrice
		set priceClose = null
		from (select contractPriceID from ContractPrice CP
		right join TradeView T on T.contractID = CP.contractID
		left join ContractView C on C.contractID = T.contractID
		where CP.exchangeID is null 
			and C.contractTypeID < 3) as CP2
		where ContractPrice.contractPriceID=CP2.contractPriceID

	set @error = @@error
	if @error<>0 goto finish

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcConstrName varchar(8000),
			@vcIndexName varchar(8000),
			@vcExecString1 varchar(8000)

	set @vcTableName = '##ETSEODUP' + + cast(db_name() as varchar(8000))
	set @vcConstrName = 'PK_ETSEODUP' + cast(db_name() as varchar(8000))
	set @vcIndexName = 'IND1_ETSEODUP' + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	--number of minutes of EOD timeout
	declare @dtEODTimeOut varchar(80)

	select @dtEODTimeOut = keyValue from DataInfo where dataInfoID = 7
	if @dtEODTimeOut is null
		begin
			set @dtEODTimeOut = '10'			
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			set @vcExecString1 = 'if exists(select * from ' + @vcTableName + ' where DATEDIFF(mi,actionDate,GetDate()) >' + @dtEODTimeOut + ') drop table ' + @vcTableName 
			exec(@vcExecString1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
			Return (-1) 
		end

	exec('create table ' + @vcTableName + '(
		rowID int identity(1,1),
		contractID int not null,
		priceClose float not null,
		actionDate smalldatetime default GetDate())')

	exec('ALTER TABLE ' + @vcTableName + ' ADD CONSTRAINT ' + @vcConstrName + ' PRIMARY KEY CLUSTERED (rowID)')

	exec('CREATE NONCLUSTERED INDEX ' + @vcIndexName + ' ON ' + @vcTableName + ' (contractID)')

	set @error = @@error
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


CREATE   proc dbo.usp_EodVolaBands_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcConstrName varchar(8000),
			@vcIndexName varchar(8000)


	set @vcTableName = '##ETSEODSRVB' + cast(db_name() as varchar(8000))
	set @vcConstrName = 'PK_VolaBand' + cast(db_name() as varchar(8000))
	set @vcIndexName = 'VolaBand_contractID_ind' + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	--number of minutes of EOD timeout
	declare @dtEODTimeOut varchar(80)

	select @dtEODTimeOut = keyValue from DataInfo where dataInfoID = 7
	if @dtEODTimeOut is null
		begin
			set @dtEODTimeOut = '10'
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			set @vcExecString1 = 'if exists(select * from ' + @vcTableName + ' where DATEDIFF(mi,actionDate,GetDate()) >' + @dtEODTimeOut + ') drop table ' + @vcTableName 
			exec(@vcExecString1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of Vola Bands EOD.', 16, 1)
			Return (-1) 
		end

	exec('create table ' + @vcTableName + '(
		rowID int identity(1,1),
		contractID int not null,
		expPeriod int not null,
		strikeMoneyness float not null,
		IVBandLow float not null,
		IVBandHi float not null,
		actionDate smalldatetime default GetDate())')

	exec('ALTER TABLE ' + @vcTableName + ' ADD CONSTRAINT ' + @vcConstrName + ' PRIMARY KEY CLUSTERED (rowID)')

	exec('CREATE NONCLUSTERED INDEX ' + @vcIndexName + ' ON ' + @vcTableName + ' ([contractID])')

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


CREATE      proc dbo.usp_VolaBandsEOD_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000)

	set @vcTableName = '##ETSEODVB' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	--number of minutes of EOD timeout
	declare @dtEODTimeOut varchar(80)

	select @dtEODTimeOut = keyValue from DataInfo where dataInfoID = 7
	if @dtEODTimeOut is null
		begin
			set @dtEODTimeOut = '10'
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			set @vcExecString1 = 'if exists(select * from ' + @vcTableName + ' where DATEDIFF(mi,actionDate,GetDate()) >' + @dtEODTimeOut + ') drop table ' + @vcTableName 
			--select @vcExecString1
			exec(@vcExecString1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of Vola Bands EOD.', 16, 1)
			Return (-1) 
		end

	exec('create table ' + @vcTableName + '(
		rowID int identity(1,1),
		symbol varchar(20) null,
		expPeriod int not null,
		strikeMoneyness float not null,
		IVBandLow float not null,
		IVBandHi float not null,
		actionDate smalldatetime default GetDate())')

	--symbol, exchangeCode, contractTypeID
	exec('ALTER TABLE ' + @vcTableName + ' ADD CONSTRAINT PK_VolaBand PRIMARY KEY CLUSTERED (rowID)')

	exec('CREATE NONCLUSTERED INDEX symbol_ind ON ' + @vcTableName + ' ([symbol])')

	set @error = @@error
Return(@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.43.094')
	begin
		update DataInfo set keyValue = '3.43.094' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.43.094' '3.43.094 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
