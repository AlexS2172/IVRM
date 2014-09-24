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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.225'  or (keyValue = '3.46.226') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.226 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.226' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END

--=============================================================================================
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO


ALTER    PROC dbo.usp_MmEtsCustomDividend_Get 

	@iStockID int = null
AS
	set nocount on

	if (select contractTypeID from contractview where contractid = 873836) = 1
	begin
		-- empty recordset for index
		select 
		       [ID], 
                       StockID, 
                       DivYtes as [Dividend Date] ,
		       datediff(d, GetDate(),DivYtes) as [DTD],
		       DivAmnt as [Dividend Amount]
		from CustomDividend 
		where StockID = -1
		
		return @@error
	end
	

	if isnull(@iStockID, 0) = 0
		select [ID], 
                       StockID, 
                       DivYtes as [Dividend Date] ,
		       datediff(d, GetDate(),DivYtes) as [DTD],
		       DivAmnt as [Dividend Amount] 
                from CustomDividend
		order by StockID, DivYtes, DivAmnt
	else
		select 
		       [ID], 
                       StockID, 
                       DivYtes as [Dividend Date] ,
		       datediff(d, GetDate(),DivYtes) as [DTD],
		       DivAmnt as [Dividend Amount]
		 from CustomDividend 
		where StockID = @iStockID
		order by DivYtes, DivAmnt
	return @@error
GO


--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.226')
	begin
		update DataInfo set keyValue = '3.46.226' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.226' '3.46.226 update message:' 
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



