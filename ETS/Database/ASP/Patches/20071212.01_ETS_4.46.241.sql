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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.240' or (keyValue = '3.46.241') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '3.46.241 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '3.46.241' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
/* New procedure DataInfo_Put plus additional error message in case if EOD update is failed */
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DataInfo_Put]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop procedure [dbo].[usp_DataInfo_Put]
GO

CREATE    Procedure dbo.usp_DataInfo_Put
	  @vDataInfoID int,  
	  @vcKeyName varchar(50) = null,   -- key's name    
	  @vcKeyValue varchar(150) = null  -- value  
AS
	Begin Tran  
	  if exists ( select * from DataInfo where dataInfoID = @vDataInfoID  )
	     Begin  
		update DataInfo set keyValue = @vcKeyValue where dataInfoID = @vDataInfoID
	     End
	  Else
	     Begin
		insert into DataInfo (dataInfoID, keyName, keyValue) values (@vDataInfoID, @vcKeyName, @vcKeyValue)
	     End
	Commit
GO

GO


if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_Execute]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop procedure [dbo].[usp_AspEodUpdateMarketStructure_Execute]
GO

CREATE    PROC dbo.usp_AspEodUpdateMarketStructure_Execute 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

--	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
--	if @iWithTran is not null begin tran @vcTranName

	/* To alert users about EOD procedure being in process */
	declare @EODDate varchar(150)
	declare @RC int

	select @EODDate = convert(varchar, GetDate() - 1, 20)
	exec usp_DataInfo_Put 21, "EODLastDate", @EODDate

	exec @RC = usp_AspEodUpdateMarketStructure_Main_Execute @iWithTran
	if @@error != 0 or @RC != 0 begin set @error = -1 goto finish end

	exec @RC = usp_AspEodUpdateMarketStructure_DefaultIVs_Execute @iWithTran
	if @@error != 0 or @RC != 0  begin set @error = -2 goto finish end

	exec @RC = usp_AspEodUpdateMarketStructure_Exchanges_Execute @iWithTran
	if @@error != 0 or @RC != 0  begin set @error = -3 goto finish end

	select @EODDate = convert(varchar, GetDate(), 20)
	exec usp_DataInfo_Put 21, "EODLastDate", @EODDate

--	if @iWithTran is not null commit tran @vcTranName

finish:
	if @@error != 0 or @error != 0
	begin	
		select @EODDate = convert(varchar, GetDate() - 2, 20)
		exec usp_DataInfo_Put 21, "EODLastDate", @EODDate

		DECLARE @PrnLine nvarchar(4000)
		PRINT 'Stored Procedure: usp_AspEodUpdateMarketStructure_Execute'
		SELECT @PrnLine = '	Return Code = ' + CONVERT(nvarchar, @RC)  + '. Exit code = ' + CONVERT(nvarchar, @error )
		PRINT @PrnLine
	end

	return @error
GO

GO

--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.241')
	begin
		update DataInfo set keyValue = '3.46.241' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0 OR 
   not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_Execute]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	ROLLBACK TRANSACTION
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
	select 'DB VERSION WAS CHANGED TO 3.46.241' '3.46.241 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

