/*
	fix for MLine00001064
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.39.053' or (keyValue = '3.39.054')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.39.054 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.39.054' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER       PROCEDURE dbo.usp_SyntheticParamCA_Save 
	@iCorpActionID int, 
	@iOptionRootID int,
	@iUnderlyingID int,
	@fWeight float = null
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is save information into SyntheticUnderlyingParam table
	and in CorporateActionLog
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iSyntheticOptRootID int
		select @iSyntheticOptRootID = syntheticOptRootID 
			from SyntheticOptionRootView where optionRootID = @iOptionRootID


	declare @vcTableName sysname

	--===============================================
	-- variables for work with CorporateActionLog
	--================================================
	declare @iCAActionID int, --1 for insert, 2 - for update, 3 - for delete
			@vcKeyRow varchar(50),  -- name of field with identity atribute
			@iRowID int,			-- tipically is identity value
			@vcFieldName varchar(50),
			@vcFieldValue varchar(250),
			@vcFieldType varchar(50),
			@fOldSyntheticCoeff float

	if not exists(select * from ContractView where contractTypeID < 3 and contractID = @iUnderlyingID)
		begin
			Raiserror('Underlying with this identifier does not exists in DB!',16,1)
			Return(-1)
		end

	Begin Transaction CA2
		if not exists(select * From SyntheticUnderlyingParamView where syntheticOptRootID = @iSyntheticOptRootID and underlyingID = @iUnderlyingID) 
			begin
	
				insert into SyntheticUnderlyingParam (
							syntheticOptRootID, 
							underlyingID, 
							SyntheticCoeff)
					values (@iSyntheticOptRootID,
							@iUnderlyingID,
							@fWeight)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction CA2 goto finish end
				set @iRowID = @@identity

				set @iCAActionID = 1
				set @vcKeyRow = 'syntheticUndParamID'
				set @vcTableName = 'SyntheticUnderlyingParam'
				
				select @vcFieldName = 'syntheticUndParamID', @vcFieldValue = @vcKeyRow, @vcFieldType = 'int'
				exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow 
		   			if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA2 goto finish end
	
			end
		else
			begin
				select 	@iRowID = syntheticUndParamID,
					@fOldSyntheticCoeff = syntheticCoeff
				from SyntheticUnderlyingParamView 
				where syntheticOptRootID = @iSyntheticOptRootID and underlyingID = @iUnderlyingID


				update SyntheticUnderlyingParam set 
					SyntheticCoeff = isnull(@fWeight, SyntheticCoeff)
				where syntheticOptRootID = @iSyntheticOptRootID and underlyingID=@iUnderlyingID
		   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction CA2 goto finish end

				set @iCAActionID = 2
				set @vcKeyRow = 'syntheticUndParamID'
				set @vcTableName = 'SyntheticUnderlyingParam'

				select @vcFieldName = 'syntheticCoeff', @vcFieldValue = @fOldSyntheticCoeff, @vcFieldType = 'float'
				exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCAActionID,@iRowID,@vcKeyRow
			   		if @error <> 0 begin if @@TranCount = 1 Rollback Transaction CA2 goto finish end															
			end
	Commit Transaction CA2
    set @error = 0
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
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.39.054')
	begin
		update DataInfo set keyValue = '3.39.054' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.39.054' '3.39.054 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO






