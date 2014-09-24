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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.237'  or (keyValue = '3.46.238') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.238 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.238' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
---------------------------------------------------------------------------------------------------------------------------
-- proc for dividend save
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_MmDividend_Save]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_MmDividend_Save
GO
---------------------------------------------------------------------------------------------------------------------------
CREATE   PROC dbo.usp_MmDividend_Save 

	@iContractID int = null,
	@iDivType int = null,
	@fDivAmt float = null,
	@dtDivDate smalldatetime = null,
	@fDivFreq int = null,
	@fDivAmtCust float = null,
	@dtDivDateCust smalldatetime = null,
	@fDivFreqCust int = null,
	@fYield float = null
AS
	set nocount on
	
	if ( exists( select StockID from StockView where StockID = @iContractID) )
	begin
		begin tran

			update [Stock]
			set 
				isDivCustom = isnull(@iDivType, isDivCustom),
				divFreq = isnull(@fDivFreq, divFreq),
				divAmt = isnull(@fDivAmt, divAmt),
				divDate = isnull(@dtDivDate, divDate),	 
				divFreqCustom = isnull(@fDivFreqCust, divFreqCustom),
				divAmtCustom = isnull(@fDivAmtCust, divAmtCustom),
				divDateCustom = isnull(@dtDivDateCust, divDateCustom)	 
			where	StockID = @iContractID	

			if (@@error <> 0)
			begin
				RAISERROR ('Error. Can''t update [Stock]!', 16, 1)
				ROLLBACK TRAN
				RETURN (-1)
			end
 	
		commit tran
	end

	if ( exists( select IndexID from IndexView where IndexID = @iContractID) )
	begin
		begin tran

			update [Index]
			set 
				isDivCustom = isnull(@iDivType, isDivCustom),
				Yield = isnull(@fYield, Yield),
				divFreq = isnull(@fDivFreq, divFreq),
				divAmt = isnull(@fDivAmt, divAmt),
				divDate = isnull(@dtDivDate, divDate),	 
				divFreqCustom = isnull(@fDivFreqCust, divFreqCustom),
				divAmtCustom = isnull(@fDivAmtCust, divAmtCustom),
				divDateCustom = isnull(@dtDivDateCust, divDateCustom)	 
			where	IndexID = @iContractID
			
			if (@@error <> 0)
			begin
				RAISERROR ('Error. Can''t update [Index]!', 16, 1)
				ROLLBACK TRAN
				RETURN (-1)
			end
 	
		commit tran
	end

GO
---------------------------------------------------------------------------------------------------------------------------
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.238')
	begin
		update DataInfo set keyValue = '3.46.238' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.238' '3.46.238 update message:' 
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




