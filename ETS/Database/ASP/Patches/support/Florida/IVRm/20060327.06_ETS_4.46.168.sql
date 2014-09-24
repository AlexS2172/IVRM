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
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

ALTER   PROC dbo.usp_ICFutures_Get
	@iICID int = null
AS
	-- retrieve future

	set nocount on

	if @iICID is not null
	SELECT 
		EIC.expiryDate    	AS MaturityDate, 
		Cm.CommodityID    	AS CommodityID, 
		Fr.futureRootName 	AS FutureRootName, 
		Fr.futureLotSize 	AS FutureLotSize, 
		Fr.futureRootSymbol 	AS FutureRootSymbol,
		CF.symbol 		AS vcSymbol, 
		Fr.optionLotSize 	AS OptioLotSize, 
		Fr.exportSymbol 	AS FRExportSymbol, 
		CF.contractName 	AS FutureName, 
		F.futureRootID 		AS FutureRootID
	 from Future F
	inner Join FutureRoot FR on FR.futureRootID = F.futureRootID And Fr.underlyingID =  @iICID
	inner join Commodity  CM on Cm.CommodityID  =  @iICID	
	inner join Contract   CF  on CF.ContractId    =  F.futureID
	inner join ExpiryInCalendarView EIC ON F.maturityID = EIC.expiryID and CF.expCalendarID = EIC.expCalendarID
	ORDER BY FutureName
	else
	SELECT 
		EIC.expiryDate    	AS MaturityDate, 
		Cm.CommodityID    	AS CommodityID, 
		Fr.futureRootName 	AS FutureRootName, 
		Fr.futureLotSize 	AS FutureLotSize, 
		Fr.futureRootSymbol 	AS FutureRootSymbol,
		CF.symbol 		AS vcSymbol, 
		Fr.optionLotSize 	AS OptioLotSize, 
		Fr.exportSymbol 	AS FRExportSymbol, 
		CF.contractName 	AS FutureName, 
		F.futureRootID 		AS FutureRootID
	 from Future F
	inner Join FutureRoot FR on FR.futureRootID = F.futureRootID 
	inner join Commodity  CM on  CM.CommodityID  is not null	
	inner join Contract   CF  on CF.ContractId    =  F.futureID
	inner join ExpiryInCalendarView EIC ON F.maturityID = EIC.expiryID and CF.expCalendarID = EIC.expCalendarID
	ORDER BY FutureName


GO

/* RESULTS ANALYZING */
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.168')
	begin
		update DataInfo set keyValue = '4.49.168' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.168' '4.49.168 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
