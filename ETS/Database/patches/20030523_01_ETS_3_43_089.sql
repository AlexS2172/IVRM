/*
MLine00001757
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.43.088' or (keyValue = '3.43.089')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.43.089 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.43.089' as  'Version in script' from DataInfo where dataInfoID = 1
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
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Option_Import' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Option_Import    
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodSynthRootCompWithPosition_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodSynthRootCompWithPosition_Get    
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUndWithOptionsPositions_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUndWithOptionsPositions_Get    
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
			end

			if @error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

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
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE	PROCEDURE dbo.usp_Option_Import
	@iUnderlyingContractID int,
	@vcSymbol varchar(20),
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
	@iLotSize int,
	@iMsCacheOptionID int = null
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare	@OptionID int, 
			@cn varchar(255),
			@iExpiryID int,
			@iLastExpiryID int,
			@vcUnderlyingSymbol varchar(8),
			@ERR_MSG varchar(8000)

	select @vcUnderlyingSymbol = symbol from ContractView where contractID = @iUnderlyingContractID
	--===================================================================			
	-- check the Expiry
	--===================================================================			
	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	if not exists(select * from Expiry where expiryID = @iExpiryID)
		begin
			set @ERR_MSG = 'Incorrect expiration! '  + '''' +  Convert(varchar(50),@dtExpiry) + '''' + ' for symbol=' + '''' + @vcSymbol + '''' +  ' UnderlyingSymbol=' + '''' + @vcUnderlyingSymbol + '''' + ' Strike=' + Convert(varchar(15),@fStrike) + ' expiryID=' + Convert(varchar(15),@iExpiryID)
			Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end

	--===================================================================			
	-- check symbol of option root
	--===================================================================			
   if exists (select optionRootID 
				   from OptionRootView
				   where symbol = left(@vcSymbol,len(@vcSymbol)-2)and 
					underlyingID<>@iUnderlyingContractID)
		begin			
			Declare @vcDifferentUndSymbol varchar(8)
			select @vcDifferentUndSymbol = C.symbol from ContractView C join
					OptionRootView ort on ort.underlyingID = C.contractID
					where ort.symbol = left(@vcSymbol,len(@vcSymbol)-2) and
					ort.underlyingID<>@iUnderlyingContractID
					
			set @ERR_MSG = 'Incorrect option symbol! '  + '''' +  @vcSymbol + ''''  +  ': Option Root=' + '''' + left(@vcSymbol,len(@vcSymbol)-2) + '''' + ' assign to different underlying=' + '''' +  @vcDifferentUndSymbol + '''' + '. Please, rename Option Root and restart importing of market structure.'
			Raiserror(@ERR_MSG,16,1)
			RETURN (-1)
		end  


	select top 1 @iLastExpiryID = expiryID from ExpiryInCalendarView where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	--===================================================================			
	-- select the @iOptionRootID
	--===================================================================			
	declare @iOptionRootID int,
			@vcOptionRoot varchar(20)
	select @iOptionRootID = optionRootID from OptionRootView where symbol = left(@vcSymbol,len(@vcSymbol)-2)
			and underlyingID = @iUnderlyingContractID 

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if @iOptionRootID is not null 
		select @OptionID = optionID
			from [Option] (nolock)
			where optionRootID = @iOptionRootID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	--===================================================================			
	-- check if option with same symbol exists
	--===================================================================		
	set @ERR_MSG = null

	execute usp_Option_Check @iOptionID = @OptionID, @vcSymbol = @vcSymbol,	@iOptionRootID = @iOptionRootID,
			@dtExpiry = @dtExpiry,@iExpiryID = @iExpiryID,@fStrike = @fStrike,@iIsCall = @iIsCall,
			@vcUnderlyingSymbol = @vcUnderlyingSymbol,@bRemoveOption = 1,@ERR_MSG = @ERR_MSG output

	if @ERR_MSG is not null and @ERR_MSG != ''
	begin
		if @ERR_MSG != '*' Raiserror(@ERR_MSG,16,1)
		Return(-1)
	end

	--===================================================================			
	-- make the contract name
	--===================================================================			
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(month, @dtExpiry) + 
					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)


  	if (@OptionID is Null) 
	begin
		Begin Transaction

			if  @iOptionRootID is null
			begin
				insert into OptionRoot (underlyingID,symbol,lotSize)
					select @iUnderlyingContractID,left(@vcSymbol,len(@vcSymbol)-2),isnull(@iLotSize,100)
				set @iOptionRootID = @@identity
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end 
	
			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@cn)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				 values (@OptionID, @iOptionRootID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@OptionID,null)
					if (@@error <> 0) begin rollback tran return(-1) end           
				end 

			if @iMsCacheOptionID is null
			begin
				delete from MsOptionCache
				where (
					symbol = @vcSymbol 
					and underlyingID = @iUnderlyingContractID 
					and isCall = @iIsCall 
					and abs(strike - @fStrike) < 0.009
					and expiryDate = @dtExpiry )
			end
			else
			begin
				delete from MsOptionCache
				where optionID = @iMsCacheOptionID
			end

		Commit Transaction
	end 
 	else
	begin
		update Contract
			set symbol = @vcSymbol,
					 contractName = @cn
			where contractID = @OptionID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract!', 16, 1)
				  RETURN (-1)
			   end 

		delete from TradeLog
		where contractID = @OptionID

		if @iMsCacheOptionID is null
		begin
			delete from MsOptionCache
			where (
				symbol = @vcSymbol 
				and underlyingID = @iUnderlyingContractID 
				and isCall = @iIsCall 
				and abs(strike - @fStrike) < 0.009
				and expiryDate = @dtExpiry )
		end
		else
		begin
			delete from MsOptionCache
			where optionID = @iMsCacheOptionID
		end

		if @iOptionRootID is not null
		begin
		  update OptionRoot
				set
					lotSize = isnull(@iLotSize,lotSize)
				where optionRootID = @iOptionRootID
		end

		update [Option]
			set 
			IsCall = isnull(@iIsCall,IsCall),
			strike = isnull(@fStrike,Strike),
			expiryID = isnull(@iExpiryID,expiryID),
			isManualyEntered = 0
		where optionID = @OptionID

	   if (@@error <> 0)
		   begin
			  RAISERROR ('Error. Can''t update Option information (table Option)!', 16, 1)
			  RETURN (-1)
		   end 
	end 
	 
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


CREATE    PROCEDURE dbo.usp_EodSynthRootCompWithPosition_Get 
	@iUnderlyingID int = null
AS
/*
	Created by: Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iUnderlyingID is null
	begin
		select distinct	
			ort.underlyingID as iUnderlyingID,
			ort.optionRootID as iRootID,
			sup.underlyingID as iCompUndID, 
			sup.SyntheticCoeff as fWeight
		from
			SyntheticUnderlyingParamView as sup
			inner join SyntheticOptionRootView as sor
					on sor.syntheticOptRootID = sup.syntheticOptRootID
			inner join OptionRootView as ort
					on sor.optionRootID = ort.optionRootID
			inner join ContractView as C 
					on C.contractID = sup.underlyingID
			join OptionView O on O.optionRootID=ort.optionRootID  
			join TradeView TV on TV.contractID=O.optionID
			
	end
	else
	begin
		select distinct
			ort.underlyingID as iUnderlyingID,
			ort.optionRootID as iRootID,
			sup.underlyingID as iCompUndID, 
			sup.SyntheticCoeff as fWeight
		from
			SyntheticUnderlyingParamView as sup
			inner join SyntheticOptionRootView as sor
					on sor.syntheticOptRootID = sup.syntheticOptRootID
			inner join OptionRootView as ort
					on sor.optionRootID = ort.optionRootID
			inner join ContractView as C
					on C.contractID = sup.underlyingID
			join OptionView O on O.optionRootID=ort.optionRootID  
			join TradeView TV on TV.contractID=O.optionID
		where
			ort.underlyingID = @iUnderlyingID
	end
		
	set @error = @@error
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.43.089')
	begin
		update DataInfo set keyValue = '3.43.089' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.43.089' '3.43.089 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
