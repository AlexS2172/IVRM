/*
	fix for MLine00000954
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.39.047' or (keyValue = '3.39.048')))
	BEGIN
		PRINT 'You can''t run this script, because you have not run script for previous version!'
		Select cast(keyValue as varchar(25)) as 'DB version', '3.39.048' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_VolaBandsEOD_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_VolaBandsEOD_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
CREATE      proc dbo.usp_VolaBandsEOD_Save
	@vcSymbol varchar(20),
	@iExpPeriod int,
	@fStrikeMoneyness float,
	@fIVBandLow float,
	@fIVBandHi float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000)

	set @vcTableName = '##ETSEODVB' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	if @vcSymbol is Null 
		set @vcSymbol = 'Null'
	else
		begin
			Select REPLACE(@vcSymbol,'''','''''')
			set @vcSymbol = '''' + @vcSymbol + '''' 
		end


	set @vcExecString1 = 'insert into ' + @vcTableName + ' (symbol,expPeriod,strikeMoneyness,IVBandLow,IVBandHi)'
							+ 'values (' + @vcSymbol + ',' + cast(@iExpPeriod as varchar(50)) + ',' +  cast(@fStrikeMoneyness as varchar(50)) + ',' +  cast(@fIVBandLow as varchar(50)) + ',' + cast(@fIVBandHi as varchar(50)) + ')'

	exec(@vcExecString1)
		if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end

	set @vcExecString1 = 'if (select count(*) from ' + @vcTableName + '(nolock)) > 1999' + 
			' begin delete VolaBands where contractID in (select distinct contractID from ContractView CV ' + 
			'join ' + @vcTableName + ' EOD on CV.symbol = EOD.Symbol) '  
	set @vcExecString2 = 'insert into VolaBands (contractID,expPeriod,strikeMoneyness,IVBandLow,IVBandHi)' + 
			'select contractID,ExpPeriod,StrikeMoneyness,IVBandLow,IVBandHi from ' + @vcTableName +
			' EOD join ContractView CV on CV.symbol=EOD.Symbol ' +
			'truncate table ' + @vcTableName + ' end'
	
	exec (@vcExecString1 + @vcExecString2)

finish:
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
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.39.048')
	begin
		update DataInfo set keyValue = '3.39.048' where dataInfoID = 1
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
	PRINT 'The database update succeeded'
	PRINT 'DB VERSION WAS CHANGED TO 3.39.048'
	COMMIT TRANSACTION
END
	ELSE PRINT 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
