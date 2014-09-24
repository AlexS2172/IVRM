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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.189'  or (keyValue = '3.46.190') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.190 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.190' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

 DELETE FROM dbo.CustomStrikeSkewPoint WHERE expiryInCalendarID = 1
GO


ALTER   PROC dbo.usp_BoFuture_Get
	@iFutureID int = null
AS
	-- retrieve future

	set nocount on

	if @iFutureID is not null
		select
			f.futureID as iContractID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcSymbol,
			fc.contractName as vcContractName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.exportSymbol as vcFutureRootExportSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			f.maturityID as iMaturityID,
			frmc.expiryDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			fr.futureLotSize as iFutureLotSize,
			f.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
			inner join ExpiryInCalendarView frmc on f.maturityID = frmc.expiryID and fc.expCalendarID= frmc.expCalendarID
		where
			f.futureID = @iFutureID
	else
		select
			f.futureID as iContractID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcSymbol,
			fc.contractName as vcContractName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.exportSymbol as vcFutureRootExportSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			f.maturityID as iMaturityID,
			frmc.expiryDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			fr.futureLotSize as iFutureLotSize,
			f.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
			inner join ExpiryInCalendarView frmc on f.maturityID = frmc.expiryID and fc.expCalendarID= frmc.expCalendarID
		order by
			fc.symbol


GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.190')
	begin
		update DataInfo set keyValue = '3.46.190' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.190' '3.46.190 update message:' 
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