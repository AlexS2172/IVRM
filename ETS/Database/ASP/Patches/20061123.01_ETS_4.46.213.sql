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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.212'  or (keyValue = '3.46.213') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.213 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.213' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

ALTER   proc dbo.usp_ContractInExchangeEOD_Complete_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	Begin tran 
		if object_id('tempdb..##ContractExchangeLinkEOD') is null
		   begin
		       raiserror ( 'Cannot complete EOD. Cannot find save data of IV bands.', 16, 1)
		       return (-1)
		   end

		delete from ContractExchangeLink where ActionID =3
 		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

 		delete from ContractExchangeLink where isOption = 1 and (isUnderlying is null or isUnderlying = 0)
 		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update ContractExchangeLink set isOption = 0 where isOption = 1

		update ContractExchangeLinkView
			set exchangeID = EOD.exchangeID, 
				contractID = EOD.contractID, 
				isUnderlying = isnull(EOD.isUnderlying, CE.isUnderlying), 
				isOption = isnull(EOD.isOption, CE.isOption)
			from ContractExchangeLinkView CE 
				inner join ##ContractExchangeLinkEOD EOD
					on isnull(CE.exchangeID,0) = isnull(EOD.exchangeID,0) and
						CE.contractID = EOD.contractID

 		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
 	
		insert into ContractExchangeLinkView (exchangeID,contractID,isUnderlying,isOption )
			select exchangeID,contractID,isUnderlying,isOption
				from ##ContractExchangeLinkEOD EOD
					where not exists
						(select * from ContractExchangeLinkView CE where isnull(CE.exchangeID,0) = isnull(EOD.exchangeID,0) and CE.contractID = EOD.contractID)

 		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit tran
    set @error = 0
finish:
	if object_id('tempdb..##ContractExchangeLinkEOD') is not null
		drop table ##ContractExchangeLinkEOD
Return(@error)

GO


GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.213')
	begin
		update DataInfo set keyValue = '3.46.213' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.213' '3.46.213 update message:' 
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
