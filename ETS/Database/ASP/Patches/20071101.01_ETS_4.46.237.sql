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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.236'  or (keyValue = '3.46.237') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.237 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.237' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
---------------------------------------------------------------------------------------------------------------------------
--now this proc return underlyingid and contracttype
ALTER    PROC usp_MmManualPrice_Get
@ContractID int = NULL
AS

SELECT 
	MP.contractID,
	MP.manualPrice,
	C.Symbol,
	isnull(FR.underlyingID, isnull([OR].underlyingID, isnull(FR2.underlyingID, MP.contractID)))  as UnderlyingID,
	C.ContractTypeID
FROM ManualPrice MP
	JOIN Contract C ON C.contractID = MP.contractID
	/* future */
	LEFT JOIN Future F ON F.futureID = MP.contractID
	LEFT JOIN futureRoot FR ON FR.futureRootID = F.futureRootID
	/* option */
	LEFT JOIN [option] O ON O.optionID = MP.contractID
	LEFT JOIN optionRoot [OR] ON [OR].optionRootID = O.optionRootID
	 
	/* future option */
	LEFT JOIN futureOption FO ON FO.futureOptionID = MP.contractID
	LEFT JOIN Future F2 ON F2.futureID = FO.futureID
	LEFT JOIN futureRoot FR2 ON FR2.futureRootID = F2.futureRootID
WHERE 
	@ContractID = C.ContractID OR @ContractID IS NULL


GO
---------------------------------------------------------------------------------------------------------------------------
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.237')
	begin
		update DataInfo set keyValue = '3.46.237' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.237' '3.46.237 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




