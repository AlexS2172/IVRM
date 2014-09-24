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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.214'  or (keyValue = '3.46.215') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.215 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.215' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

ALTER   procedure dbo.usp_MmExpiredPositions_Check
-- Created by Artem Shylaev Egar T
-- 2005 - 08 - 30
  @vOptWithPsExp As int output
As
   select Cn.symbol, Op.optionid As OptionID , T.TotalSum  As TotalSum , Op.expiryDate --ec.expirydate 
	from Contract Cn
	join ( select 
		contractid ,
		underlyingid ,
		sum( case isbuy when 0 then quantity else -quantity end ) As TotalSum
		from TradeView
			where  status <> 1
		group by contractid  , underlyingid ) T on ( Cn.contractid = T.contractid )
	inner join Contract uc on T.underlyingID = uc.contractID
	join [option]  Op on Cn.contractid = Op.optionid
	--left join ExpiryInCalendarView ec on Op.expiryID = ec.expiryID and ec.expCalendarID = isnull ( Cn.expCalendarID, uc.expCalendarID)
	where datediff ( dd , /*ec.expirydate*/ Op.expiryDate , getdate() ) >= 0 -- and Op.actionid < 3 
		and T.TotalSum  <> 0 and Cn.contracttypeid = 3 
   union select Cn.symbol, Op.futureoptionid As OptionID, T.TotalSum  As TotalSum , Op.expiryDate --ec.expirydate 
	from Contract Cn
	join ( select 
		contractid ,
		underlyingid ,
		sum( case isbuy when 0 then quantity else -quantity end ) As TotalSum
		from TradeView
			where  status <> 1
		group by contractid  , underlyingid ) T on ( Cn.contractid = T.contractid )
	inner join Contract uc on T.underlyingID = uc.contractID
	join [FutureOption]  Op on Cn.contractid = Op.futureoptionid
	--left join ExpiryInCalendarView ec on Op.expiryID = ec.expiryID and ec.expCalendarID = isnull ( Cn.expCalendarID, uc.expCalendarID)
	where datediff ( dd , /*ec.expirydate*/ Op.expiryDate , getdate() ) >= 0 -- and Op.actionid < 3 
		and T.TotalSum  <> 0 and Cn.contracttypeid = 5 

   select @vOptWithPsExp = @@RowCount



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.215')
	begin
		update DataInfo set keyValue = '3.46.215' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.215' '3.46.215 update message:' 
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
