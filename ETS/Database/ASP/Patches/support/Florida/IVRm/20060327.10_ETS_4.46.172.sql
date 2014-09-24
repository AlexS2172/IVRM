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

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodFuturesWithPositions2_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodFuturesWithPositions2_Get

GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE  PROCEDURE dbo.usp_EodFuturesWithPositions2_Get
  as
	---------------
	set nocount on
	---------------

	declare @error int		set @error = 0

	select 
		FR.underlyingID as iIndexID,
		F.futureID as iFutureID,
		FC.symbol as vcSymbol,
		F.isActive as tiIsActive,
		F.maturityID as iMaturityID,
		F.calcOptionType As IsAmerican,
		0 as fPosition,
		cp.priceClose as fPriceClose
	from Future F
		inner join Contract FC on FC.contractID=F.futureID
		inner join FutureRoot FR on FR.futureRootID=F.futureRootID
		inner join ContractPrice cp on cp.contractID = FC.contractID and cp.exchangeID is null
	group by FR.underlyingID, F.futureID, FC.symbol, F.isActive, F.maturityID, cp.priceClose, f.CalcOptionType
	order by FR.underlyingID 

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.172')
	begin
		update DataInfo set keyValue = '4.49.172' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.172' '4.49.172 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

