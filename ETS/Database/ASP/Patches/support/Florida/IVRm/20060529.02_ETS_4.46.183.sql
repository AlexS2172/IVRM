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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.182'  or (keyValue = '3.46.183') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.183 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.183' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
----------------------------------------------------------------



ALTER PROC dbo.usp_BoUnderlyings_Get
as
	------------------------------------------------
	set nocount on
	------------------------------------------------	
	select
		contractID as iContractID, 
		contractTypeID as iContractTypeID, 
		symbol as vcSymbol
	from ContractView
	where contractTypeID in (1, 2, 6)
	order by vcSymbol

	return @@error

GO


ALTER  PROC dbo.usp_BoFutureOption_Get
	@iFutureID int = null
AS
	-- get future options

	set nocount on
	
	if @iFutureID is not null
		select
			fo.futureOptionID as iFutureOptionID,
			foc.contractTypeID as iFutureOptionContractTypeID,
			foc.symbol as vcFutureOptionSymbol,
			foc.contractName as vcFutureOptionName,
			f.futureID as iFutureID,
			fc.contractTypeID as iFutureContractTypeID,
			fc.symbol as vcFutureSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			fo.expiryID as iExpiryID, 
			fec.expiryDate as dtExpiry,
			fo.strike as fStrike, 
			fo.isCall as tiIsCall,
			fo.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureOption fo on f.futureID = fo.futureID
			inner join ContractView foc on fo.futureOptionID = foc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fr.matCalendarID
			inner join ContractView uc on fr.underlyingID = uc.contractID
		where
			f.futureID = @iFutureID
	else
		select
			fo.futureOptionID as iFutureOptionID,
			foc.contractTypeID as iFutureOptionContractTypeID,
			foc.symbol as vcFutureOptionSymbol,
			foc.contractName as vcFutureOptionName,
			f.futureID as iFutureID,
			fc.contractTypeID as iFutureContractTypeID,
			fc.symbol as vcFutureSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			fo.expiryID as iExpiryID, 
			fec.expiryDate as dtExpiry,
			fo.strike as fStrike, 
			fo.isCall as tiIsCall,
			fo.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureOption fo on f.futureID = fo.futureID
			inner join ContractView foc on fo.futureOptionID = foc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fr.matCalendarID
			inner join ContractView uc on fr.underlyingID = uc.contractID


GO


GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.183')
	begin
		update DataInfo set keyValue = '3.46.183' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.183' '3.46.183 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO