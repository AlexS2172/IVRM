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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.132'  or (keyValue = '3.46.133') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.133 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.133' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

GO
ALTER PROCEDURE dbo.usp_OptionRootMove_Save
	@iOldRootID int, 
	@iNewRootID int
AS

	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int,
			@iUnderlyingID int
	set @error = -1

	select @iUnderlyingID = underlyingID from OptionRootView where optionRootID = @iOldRootID

	if exists(select * From OptionRootView where optionRootID = @iNewRootID and underlyingID <> @iUnderlyingID)
		begin
			Raiserror('Can''t move roots options to different underlying. To perform this use Corporate action!',16,1)
			Return(-1)
		end 

	if exists(select O1.optionID From OptionView O1 
					join OptionView O2 
						on O1.isCall = O2.isCall 
						and abs(O1.strike - O2.strike) < 0.009
						and O1.expiryID = O2.expiryID
					where O1.optionRootID = @iNewRootID and O2.optionRootID = @iOldRootID)
		begin
			select C1.symbol as vcOptSymbol1, C2.symbol as vcOptSymbol2 From ContractView C1 
					join OptionView O1 on O1.optionID = C1.contractID
					join OptionView O2  
						on O1.isCall = O2.isCall 
						and abs(O1.strike - O2.strike) < 0.009
						and O1.expiryID = O2.expiryID
					join ContractView C2 on O2.optionID = C2.contractID 
					where O1.optionRootID = @iNewRootID and O2.optionRootID = @iOldRootID
			Return(1)
		end 

	Begin Transaction
		update OptionRoot set 
			actionID = 3
		where optionRootID = @iOldRootID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

		update Contract
			set symbol = ORT.symbol + right(C.symbol,2)
			from Contract C join OptionView O on C.contractID=O.optionID
				join OptionRootView ORT on ORT.optionRootID=@iNewRootID
				where O.optionRootID = @iOldRootID and C.actionID < 3

		update [Option]
			set optionRootID = @iNewRootID
				where optionRootID = @iOldRootID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

		update SyntheticUnderlyingParam
			set actionID = 3 
			from SyntheticUnderlyingParam join SyntheticOptionRootView 
			on SyntheticOptionRootView.syntheticOptRootID = SyntheticUnderlyingParam.syntheticOptRootID
			where SyntheticOptionRootView.optionRootID = @iOldRootID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

		update SyntheticRootIndexBeta
			set actionID = 3 
			from SyntheticRootIndexBeta join SyntheticOptionRootView 
			on SyntheticOptionRootView.syntheticOptRootID = SyntheticRootIndexBeta.syntheticOptRootID
			where SyntheticOptionRootView.optionRootID = @iOldRootID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

		update SyntheticOptionRoot set actionID = 3 where optionRootID = @iOldRootID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

		update Trade set optionRootID = @iNewRootID where optionRootID = @iOldRootID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction

	select null as vcOptSymbol1, null as vcOptSymbol2 where 0 = 1
    set @error = 0
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.133')
	begin
		update DataInfo set keyValue = '3.46.133' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.133' '3.46.133 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

