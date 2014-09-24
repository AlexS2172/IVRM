/*
MLine00001732
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.43.086' or (keyValue = '3.43.087')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.43.087 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.43.087' as  'Version in script' from DataInfo where dataInfoID = 1
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
	   WHERE  name = N'usp_EodOptClosePrice_Init_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOptClosePrice_Init_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUndClosePrice_Init_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUndClosePrice_Init_Save
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE        procedure dbo.usp_EodOptClosePrice_Init_Save
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
			Raiserror('Can''t define EOD operation timeout! You DB contains incorrect settings in DataInfo!',16,1)
			Return(-1)
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



CREATE      procedure dbo.usp_EodUndClosePrice_Init_Save
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
			Raiserror('Can''t define EOD operation timeout! You DB contains incorrect settings in DataInfo!',16,1)
			Return(-1)
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.43.087')
	begin
		update DataInfo set keyValue = '3.43.087' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.43.087' '3.43.087 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
