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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.145'  or (keyValue = '3.46.146') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.146 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.146' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

GO


ALTER                PROCEDURE dbo.usp_OptionRoot_Save 
	@iRootID int, 
	@vcSymbol varchar(20) = null,
	@iLotSize int = null,
	@tiIsFitEnabled tinyint = NULL,
	@tiIsSynthetic tinyint = NULL,
	@fCashValue 	float = null,
	@fSyntheticSkew float = null,
	@fSyntheticKurt float = null,
	@fSyntheticYield float = null,
	@tiSyntheticIsBasket tinyint = null
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is save information into OptionRoot table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if not exists(select * From OptionRootView where optionRootID = @iRootID) 
		begin
			Raiserror('OptionRoot with this RootID didn''t exists in DB!',16,1)
			Return(-1)
		end 

	if @vcSymbol is not null
		begin
			if exists(select * From OptionRootView where optionRootID <> @iRootID and symbol = @vcSymbol)
			begin
				Raiserror('OptionRoot with this symbol already exists in DB!',16,1)
				Return(-1)
			end 
		end

	Begin Transaction
		update OptionRoot set 
			symbol = isnull(@vcSymbol, symbol),
			lotSize = isnull(@iLotSize, lotSize),
			isFitEnabled =  isnull(@tiIsFitEnabled, isFitEnabled),
			isSynthetic = isnull(@tiIsSynthetic, isSynthetic)
		where optionRootID = @iRootID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

		if @vcSymbol is not null
		begin
			update Contract
				set symbol = @vcSymbol + right(C.symbol,2)
				from Contract C join OptionView O on C.contractID=O.optionID
					where O.optionRootID = @iRootID and C.actionID < 3
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end


		if @tiIsSynthetic = 1
			begin
				if not exists(select * From SyntheticOptionRootView where optionRootID = @iRootID)
					insert into SyntheticOptionRoot (optionRootID,cashValue,synthSkew,synthKurt,synthYield,synthIsBasket)
					values (
						@iRootID,
						@fCashValue,
						@fSyntheticSkew,
						@fSyntheticKurt,
						@fSyntheticYield,
						@tiSyntheticIsBasket)
	
		   			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				else
					update SyntheticOptionRoot set
						cashValue = isnull(@fCashValue, cashValue),
						synthSkew = isnull(@fSyntheticSkew, synthSkew),
						synthKurt = isnull(@fSyntheticKurt, synthKurt),
						synthYield = isnull(@fSyntheticYield, synthYield),
						synthIsBasket = isnull(@tiSyntheticIsBasket, synthIsBasket)
					where optionRootID = @iRootID		

		   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update SyntheticUnderlyingParam
					set actionID = 3 
					from SyntheticUnderlyingParam join SyntheticOptionRootView 
					on SyntheticOptionRootView.syntheticOptRootID = SyntheticUnderlyingParam.syntheticOptRootID
					where SyntheticOptionRootView.optionRootID = @iRootID

				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

				update SyntheticRootIndexBeta
					set actionID = 3 
					from SyntheticRootIndexBeta join SyntheticOptionRootView 
					on SyntheticOptionRootView.syntheticOptRootID = SyntheticRootIndexBeta.syntheticOptRootID
					where SyntheticOptionRootView.optionRootID = @iRootID

				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

				update SyntheticOptionRoot set actionID = 3 where optionRootID = @iRootID
		   		
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction
    set @error = 0
finish:
Return(@error)


GO

GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.146')
	begin
		update DataInfo set keyValue = '3.46.146' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.146' '3.46.146 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

