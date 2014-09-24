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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.222'  or (keyValue = '3.46.223') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.223 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.223' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END

--=============================================================================================

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



ALTER    PROC dbo.usp_ReportsUnderlyingOptions_Get
	@iUnderlyingID int,
	@dtMinExpDate datetime = Null
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @dtMinExpDate is  null
		set @dtMinExpDate = 0

	select
		c.contractID as iOptionID,
		c.symbol as vcSymbol,
		ort.optionRootID as iOptionRootID,
		o.expiryDate as dtExpiry,
		o.strike as fStrike, 
		o.isCall as tiIsCall,
		0	as IsFutureOption,
		null	as iFutureID
	from
		ContractView c  
		inner join OptionView o on o.optionID = c.contractID
		inner join OptionRootView ort on o.optionRootID = ort.optionRootID
	where
		c.contractTypeID = 3
		and ort.underlyingID = @iUnderlyingID
		and datediff(d,@dtMinExpDate, o.expiryDate) >= 0
union

	select
		c.contractID as iOptionID,
		c.symbol as vcSymbol,
		fr.futureRootID as iOptionRootID,
		fo.expiryDate as dtExpiry,
		fo.strike as fStrike, 
		fo.isCall as tiIsCall,
		1 as IsFutureOption,
		fo.futureID as iFutureID
	from
		ContractView c  
		inner join FutureRoot fr on fr.underlyingID = @iUnderlyingID
		inner join Future f on f.futurerootID = fr.futurerootID
		inner join FutureOption fo on fo.futureoptionID = c.contractID and fo.futureID = f.futureID
	where
		c.contractTypeID = 5 
		and datediff(d,@dtMinExpDate, fo.expiryDate) >= 0

	set @error = @@error

Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.223')
	begin
		update DataInfo set keyValue = '3.46.223' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.223' '3.46.223 update message:' 
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
