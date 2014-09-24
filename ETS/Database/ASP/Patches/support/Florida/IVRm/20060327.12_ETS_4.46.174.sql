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

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[fn_GetMaturityID]') and xtype in (N'FN', N'IF', N'TF'))
drop function [dbo].[fn_GetMaturityID]
GO

create function dbo.fn_GetMaturityID (@dtDate datetime)  
	returns int
as  
begin 
	return (year(@dtDate) - 2000) * 12 + datepart(month, @dtDate)
end
GO
ALTER  PROCEDURE dbo.usp_EodOldFutures_Del
	@iFutureRootID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Create table #FutureContractsForDelete(contractID int) 

	declare @maturityID int
	set @maturityID = dbo.fn_GetMaturityID(GetDate())

	insert into #FutureContractsForDelete
		select F.futureID
			from 
				Future F
					join Contract C on F.futureID = C.contractID
					join FutureRoot FR on FR.futureRootID = F.futureRootID
					--join ExpiryInCalendar E on F.maturityID = E.expiryID and FR.matCalendarID = E.expCalendarID
			where 
				( 
					( 
						--E.expiryDate < GetDate() 
						F.maturityID < @maturityID
						or datediff(d, F.actionDate, GetDate()) > 0 
					)
					and F.futureRootID = @iFutureRootID
				)

	insert into #FutureContractsForDelete
		select FO.futureOptionID
			from 
				FutureOption FO
					join Future F on F.futureID = FO.futureID
					join Contract C on FO.futureOptionID = C.contractID
					join FutureRoot FR on FR.futureRootID = F.futureRootID
					--join ExpiryInCalendar E on F.maturityID = E.expiryID and FR.matCalendarID = E.expCalendarID
			where 
				( 
					( 
						--E.expiryDate < GetDate() 
						F.maturityID < @maturityID
						or datediff(d, F.actionDate, GetDate()) > 0 
					)
					and F.futureRootID = @iFutureRootID
				)

	exec @error = usp_EodFuture_Del

	select null as vcSymbol from #FutureContractsForDelete where contractID is null

finish:
	Return(@error)


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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.174')
	begin
		update DataInfo set keyValue = '4.49.174' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.174' '4.49.174 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

