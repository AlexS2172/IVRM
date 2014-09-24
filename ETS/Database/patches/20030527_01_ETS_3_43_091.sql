/*
MLine00001806,
MLine00001807
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.43.090' or (keyValue = '3.43.091')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.43.091 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.43.091' as  'Version in script' from DataInfo where dataInfoID = 1
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
	   WHERE  name = N'usp_OptionRoot_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_OptionRoot_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_OptionRootMove_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_OptionRootMove_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Option_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Option_Save
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE              PROCEDURE dbo.usp_OptionRoot_Save 
	@iRootID int, 
	@vcSymbol varchar(20) = null,
	@iLotSize int = null,
	@tiIsSynthetic tinyint = 0,
	@fCashValue float = null,
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




CREATE  PROCEDURE dbo.usp_OptionRootMove_Save
	@iOldRootID int, 
	@iNewRootID int
AS
/*
	Created by Mike Malyshkin
*/
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
	Commit Transaction

	select null as vcOptSymbol1, null as vcOptSymbol2 where 0 = 1
    set @error = 0
finish:
Return(@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE   procedure dbo.usp_Option_Save
		@iOptionID int,
		@vcSymbol varchar(20) = Null,
		@iExchangeID int, 
		@iUnderlyingContractID int,
		@iIsCall tinyint = Null,
		@fStrike float = Null,
		@dtExpiry datetime = Null,
		@tiIsManualyEntered tinyint
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

   if exists (select contractID from ContractView where contractID = @iUnderlyingContractID and contractTypeID > 2)
	   begin
		 RAISERROR ('Error. Can''t insert or update Option values. The Underlying Contract is not stock or index!', 16, 1)
		 RETURN (-1)
	   end	 
   if len(@vcSymbol) < 3  and @iOptionID is null
	   begin
		 RAISERROR ('Error. Option symbol can''t be less three symbols!', 16, 1)
		 RETURN (-1)
	   end	 

   if exists (select optionRootID 
				   from OptionRootView
				   where symbol = left(@vcSymbol,len(@vcSymbol)-2)and 
					underlyingID != @iUnderlyingContractID)
		begin
		  RAISERROR ('Error. Can''t insert or update Option. Current Root assigned to different Underlying', 16, 1)
		  RETURN (-1)
		end  

	--===================================================================			
	-- check if option with same symbol exists
	--===================================================================		
	declare	@iOptionRootID int,
			@ContractName varchar(255),
			@iExpiryID int

	select @iOptionRootID = optionRootID from OptionRootView where symbol = left(@vcSymbol,len(@vcSymbol)-2)  and 
		underlyingID=@iUnderlyingContractID	

	declare @ERR_MSG varchar(8000),
			@vcUnderlyingSymbol varchar(10)

	select @vcUnderlyingSymbol = symbol 
		from ContractView 
		where contractID = @iUnderlyingContractID and contractTypeID <= 2

	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	execute usp_Option_Check @iOptionID = @iOptionID, 
					@vcSymbol = @vcSymbol,	
					@iOptionRootID = @iOptionRootID,
					@dtExpiry = @dtExpiry,
					@iExpiryID = @iExpiryID,
					@fStrike = @fStrike,
					@iIsCall = @iIsCall,
					@vcUnderlyingSymbol = @vcUnderlyingSymbol,
					@ERR_MSG = @ERR_MSG output

		/*	select @vcSymbol '@vcSymbol',@iOptionRootID '@iOptionRootID',@dtExpiry '@dtExpiry',@iExpiryID '@iExpiryID',
				@fStrike '@fStrike',@iIsCall '@iIsCall',@vcUnderlyingSymbol '@vcUnderlyingSymbol',@ERR_MSG '@ERR_MSG'
			select @iOptionID = 0 */

	if isnull(@ERR_MSG,'')!= ''
		begin
			if @ERR_MSG != '*' Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end

   set @ContractName = @vcUnderlyingSymbol + ' ' + datename(day, @dtExpiry) + ' ' + datename(month, @dtExpiry) + 
					' ' + datename(year, @dtExpiry) + ' ' + CASE @iIsCall when 1 then 'Call' else 'Put' end +
						 ' ' + cast(round(@fStrike, 2) as varchar)

	Begin Transaction

		if  @iOptionRootID is null
			begin
				insert into OptionRoot (underlyingID,symbol,lotSize)
					select @iUnderlyingContractID,left(@vcSymbol,len(@vcSymbol)-2),100
				set @iOptionRootID = @@identity
			end 

	   if (@iOptionID is Null)	
		 begin
			insert into Contract (contractTypeID, symbol, contractName)
				values (3, @vcSymbol, @ContractName)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iOptionID = @@identity			  
			
			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				values (@iOptionID, @iOptionRootID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		 end
	   else
		 begin
			update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@ContractName,contractName)
				where contractID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			update [Option]
				set optionRootID = @iOptionRootID,
					isCall = isNull(@iIsCall, isCall),
					strike = isNull(@fStrike, strike),
					expiryID = isNull(@iExpiryID, expiryID)
				where optionID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		 end		

		if @tiIsManualyEntered in (0,1) 
			begin
--select @iOptionID as '@iOptionID*******************'
				update [Option] set isManualyEntered = @tiIsManualyEntered where optionID = @iOptionID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end 

	Commit Transaction


    set @error = 0
finish:
if (@error != 0) return (@error) else return @iOptionID



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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.43.091')
	begin
		update DataInfo set keyValue = '3.43.091' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.43.091' '3.43.091 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
