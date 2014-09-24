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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.155'  or (keyValue = '3.46.156') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.156 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.156' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
ALTER  PROC dbo.usp_AspEodUpdateMarketStructure_DeleteFuture 
	@iFutureID int,
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	if not exists (select 1 from EodContractWithTrades (nolock) where contractID = @iFutureID)
		and not exists (select 1
					from EodContractWithTrades (nolock)
					where contractID in (select futureOptionID from FutureOption (nolock) where futureID = @iFutureID))
	begin
		delete Future
		where futureID = @iFutureID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
	
		delete ContractPrice
		where contractID = @iFutureID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
		
		delete ContractExchangeLink
		where contractID = @iFutureID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
		delete  ContractInGroup
			where contractID = @iFutureID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
		delete Contract
		where contractID = @iFutureID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
	end
	else
	begin
		update Contract
		set symbol = symbol + '*'
		where contractID = @iFutureID
			if @@error != 0 begin set @error = -2 if @iWithTran is not null rollback tran @vcTranName goto finish end
	end

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error

GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.156')
	begin
		update DataInfo set keyValue = '3.46.156' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.156' '3.46.156 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

