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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.167'  or (keyValue = '3.46.168') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.168 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.168' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

ALTER     procedure usp_MarketStructureExport  

As  
    
declare @sql varchar(3000),
	@filename varchar(1000)

BEGIN TRAN

select @filename = keyValue from DataInfo where keyName = 'MarketStrucExpFile' 
	
set @sql='BCP  "select uct.symbol as UnderlyingSymbol,oct.symbol as OptionSymbol,convert(varchar(50),ex.expiryDate,101) as ExpiryDate, o.strike as Strike from '+DB_NAME()+'.dbo.[option] o inner join  '+DB_NAME()+'.dbo.OptionRoot rt on o.optionRootID = rt.optionRootID inner join '+DB_NAME()+'.dbo.Contract uct on uct.ContractID = rt.underlyingID inner join '+DB_NAME()+'.dbo.Contract oct on oct.ContractID = o.optionID inner join '+DB_NAME()+'.dbo.ExpiryInCalendar ex on ex.ExpiryID = o.ExpiryID and ex.expCalendarID = ISNULL(oct.expCalendarID,uct.expCalendarID) where o.actionID<3 order by uct.symbol, oct.symbol,ex.expiryDate,o.strike"	queryout  "' + @filename + '" -e -c -t "\t" -C RAW -T'
exec master..xp_cmdshell @sql 

COMMIT

GO

--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.168')
	begin
		update DataInfo set keyValue = '3.46.168' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.168' '3.46.168 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
