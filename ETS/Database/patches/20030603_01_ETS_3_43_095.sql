/*
fixing bug in Option Import
*/

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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.43.094' or (keyValue = '3.43.095')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.43.095 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.43.095' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOption_Import' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOption_Import
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE	PROCEDURE dbo.usp_EodOption_Import
	@iRootID int,
	@iExpiryID int,
	@fStrike float,
	@iIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iOptionID_bad int output
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	declare	@OptionID int

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	select @OptionID = optionID
			from [Option] (nolock)
			where optionRootID = @iRootID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	if @OptionID is not null
	begin
		Begin Transaction

			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@OptionID and actionID<>3
			if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

			update [Option]  set isManualyEntered = 0 where optionID=@OptionID and actionID<>3
			if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

		Commit Transaction
	end
	else
	begin
		Begin Transaction

			select @iOptionID_bad = o.optionID
			from OptionView o join ContractView c on c.contractID = o.optionID
			where c.symbol = @vcSymbol and
				(o.optionRootID <> @iRootID or o.isCall <> @iIsCall or abs(strike - @fStrike) >= 0.009 or expiryID <> @iExpiryID)

			if @iOptionID_bad is not null
			begin
				exec @error = usp_Option_Del @iOptionID = @iOptionID_bad,@bisRaiseError = 0				
	
				if @error <> -1
				begin 
					if @error = 3 set @error = -3
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end
				else set @error = 0
			end


			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@vcContractName)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				 values (@OptionID, @iRootID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@OptionID,null)
					if (@@error <> 0) begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
				end 

		Commit Transaction
	end 
	 
   set @error = @OptionID
finish:
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 

GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.43.095')
	begin
		update DataInfo set keyValue = '3.43.095' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.43.095' '3.43.095 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
