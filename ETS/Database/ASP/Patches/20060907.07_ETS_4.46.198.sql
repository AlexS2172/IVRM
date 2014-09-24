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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.197'  or (keyValue = '3.46.198') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.198 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.198' as  'Version in script' from DataInfo where dataInfoID = 1
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

ALTER     PROC dbo.usp_EodOption_Import
	@iRootID INT,
	@dtExpiryDate DATETIME,
	@fStrike FLOAT,
	@iIsCall TINYINT,
	@vcSymbol VARCHAR(20),
	@vcContractName VARCHAR(255),
	@iOptionID_bad INT OUTPUT,
	@fPriceClose FLOAT = NULL,
    @bSaveClosePrice BIT = NULL
AS

-------------------------------------------------
	SET NOCOUNT ON
	-------------------------------------------------
	DECLARE @error INT		SET @error = 0
	DECLARE	@OptionID INT

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	SELECT @OptionID = optionID
	FROM [Option] WITH(NOLOCK, INDEX = IX_Option_optionRootID_isCall_strike_expiryID_actionID)
	WHERE optionRootID = @iRootID
		AND isCall = @iIsCall
		AND ABS(strike - @fStrike) < 0.009
		AND DATEDIFF(m, expiryDate, @dtExpiryDate) = 0
		AND actionID < 3

	IF @OptionID IS NOT NULL
	BEGIN
		BEGIN TRANSACTION

			SELECT @iOptionID_bad = contractID
			FROM Contract WITH(NOLOCK, INDEX = IX_Contract_symbol_contractTypeID_actionID)
			WHERE symbol = @vcSymbol
				--and contractID <> @OptionID
				AND contractTypeID = 3
				AND actionID < 3

			IF @iOptionID_bad IS NOT NULL AND @iOptionID_bad != @OptionID
			BEGIN
				IF NOT EXISTS (SELECT * FROM Trade WHERE ContractID = @iOptionID_bad) AND
					NOT EXISTS (SELECT * FROM [Order] WHERE ContractID = @iOptionID_bad)
				BEGIN	
					EXEC @error = usp_Option_Del @iOptionID_bad			
	
					IF @@ERROR != 0 OR @error != 0
					BEGIN 
						IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION 
						GOTO finish 
					END
				END
				ELSE
					UPDATE
						Contract
					SET 
						symbol = symbol + '*'
					WHERE
						ContractID = @iOptionID_bad
			END
				
			UPDATE Contract 
			SET symbol = @vcSymbol, contractName = @vcContractName
			WHERE contractID=@OptionID AND actionID < 3
			IF @@ERROR <> 0 BEGIN SET @error = -5 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			UPDATE [Option]  
			SET isManualyEntered = 0,
			    expiryDate = @dtExpiryDate	 
			WHERE optionID=@OptionID and actionID < 3
			IF @@ERROR <> 0 BEGIN SET @error = -6 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			IF @bSaveClosePrice IS NOT NULL AND  @bSaveClosePrice = 1
            BEGIN 	
				UPDATE ContractPrice
					SET priceClose = @fPriceClose
					WHERE contractID = @OptionID AND exchangeID IS NULL
				IF @@ERROR <> 0 BEGIN SET @error = -6 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
            END
		COMMIT TRANSACTION
	END
	ELSE
	BEGIN
		BEGIN TRANSACTION

			SELECT @iOptionID_bad = contractID
			FROM Contract WITH(NOLOCK, INDEX = IX_Contract_symbol_contractTypeID_actionID)
			WHERE symbol = @vcSymbol
				AND contractTypeID = 3
				AND actionID < 3

			IF @iOptionID_bad IS NOT NULL
			BEGIN
				IF NOT EXISTS (SELECT * FROM Trade WHERE ContractID = @iOptionID_bad) AND
					NOT EXISTS (SELECT * FROM [Order] WHERE ContractID = @iOptionID_bad)
				BEGIN	
					EXEC @error = usp_Option_Del  @iOptionID_bad			
	
					IF @@ERROR != 0 OR @error != 0
					BEGIN 
						IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION 
						GOTO finish 
					END
				END
				ELSE
					UPDATE
						Contract
					SET 
						symbol = symbol + '*'
					WHERE
						ContractID = @iOptionID_bad
			END

			INSERT INTO Contract (contractTypeID, symbol, contractName)
				 VALUES (3, @vcSymbol,@vcContractName)

			SET @OptionID = @@IDENTITY
			IF @@ERROR <> 0 BEGIN SET @error = -5 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			INSERT INTO [Option] (optionID, optionRootID, isCall, strike, expiryDate)
				 VALUES (@OptionID, @iRootID, @iIsCall, @fStrike, @dtExpiryDate)
			IF @@ERROR <> 0 BEGIN SET @error = -6 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
	
			IF NOT EXISTS(SELECT 1 FROM ContractPrice WHERE exchangeID IS NULL AND contractID = @OptionID)
			BEGIN
	  		    IF @bSaveClosePrice IS NOT NULL AND  @bSaveClosePrice = 1
					INSERT INTO ContractPrice(contractID, exchangeID, priceClose)
						VALUES(@OptionID, NULL, @fPriceClose)
                ELSE
					INSERT INTO ContractPrice(contractID, exchangeID, priceClose)
						VALUES(@OptionID, NULL, NULL)

			    IF (@@ERROR <> 0) BEGIN SET @error = -7 iF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
			END 
		
		COMMIT TRANSACTION
	END 
	 
   	SET @error = @OptionID
finish:
	RETURN(@error)




GO
ALTER   PROCEDURE dbo.usp_Option_Del
	@iOptionID int = null, 	-- contract identifier
	@bisRemovePermanent bit = 0,
	@bisRemoveFromContract bit = 1,
	@bisRaiseError bit = 1
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if object_ID('tempdb..#Options_for_delete') is null
		begin	
			Create table #Options_for_delete(OptionID int)
		end
	
	if @iOptionID is not null
		begin
			if exists (select tradeID from Trade where contractID = @iOptionID)
				begin
					if @bisRaiseError = 1 
						begin
							Raiserror ('Unable to delete the Option. There is an open position on it.', 16, 1)
						end
					Return (-2)
				end   
			if exists (select 1 from [Order] where contractID = @iOptionID)
				begin
					if @bisRaiseError = 1 
						begin
							Raiserror ('Unable to delete the Option. There is an order on it.', 16, 1)
						end
					Return (-3)
				end   
			insert into #Options_for_delete
				select @iOptionID
		end

	Begin Transaction DelOpt
		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		--IndexDefinition by contractID
		delete IndexBeta where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from DualQuadraticParams where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete ContractInGroup where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete VolaBands where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
		delete [Option] where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		delete SyntheticUnderlyingParam  where syntheticOptRootID not in 
			(select S.syntheticOptRootID from [Option] O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		delete SyntheticRootIndexBeta  where syntheticOptRootID not in 
			(select S.syntheticOptRootID from [Option] O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		delete SyntheticOptionRoot where optionRootID not in (select optionRootID from OptionView)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		delete OptionRoot where optionRootID not in (select optionRootID from OptionView)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		delete Contract where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

		if @bisRemovePermanent = 1
			begin
				--IndexBeta by contractID
				delete from IndexBeta where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
				delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
				delete [Option] where OptionID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				if @bisRemoveFromContract = 1 
					begin
						delete Contract where contractID in (select OptionID from #Options_for_delete)
							if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
					end

				delete from SyntheticUnderlyingParam where syntheticOptRootID not in 
					(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from SyntheticRootIndexBeta where syntheticOptRootID not in 
					(select S.syntheticOptRootID from OptionView O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from SyntheticOptionRoot where optionRootID not in (select optionRootID from [Option])
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end

				delete from OptionRoot where optionRootID not in (select optionRootID from [Option])
					if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DelOpt goto finish end
			end

	Commit Transaction DelOpt

finish:
	Return(@error)


GO
GO
ALTER    PROCEDURE dbo.usp_Option_Check 
	@iOptionID int = null, 
	@vcSymbol varchar(20),
	@iOptionRootID int,
	@dtExpiry datetime,
	@fStrike float,
	@iIsCall tinyint,
	@vcUnderlyingSymbol varchar(8),
	@ERR_MSG varchar(8000) = null output,
	@bRemoveOption bit = null   -- call usp_Option_Del for this option
AS
/*
	Created by: Zaur
	Description: this procedure created with template, desined by Zaur Nuraliev
		and check if option with same symbol exists
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iOptionID_bad int,
		@vcUnderlying_bad varchar(8),
		@vcOptionRoot_bad varchar(20),
		@dtExpiry_bad datetime,
		@fStrike_bad float,
		@iIsCall_bad tinyint,
		@vcSymbol_bad varchar(20)

	if @iOptionID is null
		select @iOptionID_bad = optionID 
			from OptionView O join Contract C on O.optionID = C.contractID
			where C.symbol = @vcSymbol and
			(optionRootID != @iOptionRootID or 
                         datediff(m, expiryDate, @dtExpiry)<>0 or 
                         abs(strike - @fStrike) > 0.009 or isCall != @iIsCall)
	else
		select @iOptionID_bad = optionID 
			from OptionView O join Contract C on O.optionID = C.contractID
			where optionID != @iOptionID and C.symbol = @vcSymbol and
			(optionRootID != @iOptionRootID or 
                         datediff(m, expiryDate, @dtExpiry)<>0 or 
                         abs(strike - @fStrike) > 0.009 or isCall != @iIsCall)

	if @iOptionID_bad is not null
	begin
		if @bRemoveOption is not null
			begin
				exec @error = usp_Option_Del @iOptionID = @iOptionID_bad, @bisRaiseError = 0
			end
		
		if (@error = -2) or (@error = -3) or (@bRemoveOption is null)
			begin
				select @vcSymbol_bad = symbol from ContractView where contractID = @iOptionID_bad
				select	@vcOptionRoot_bad = ORT.symbol
						from OptionView O join OptionRootView ORT on ORT.optionRootID = O.optionRootID
						where O.optionID = @iOptionID_bad
				select	@vcUnderlying_bad = C.symbol
						from ContractView C 
						join OptionRootView ORT on ORT.underlyingID = C.contractID
						join OptionView O on ORT.optionRootID = O.optionRootID
						where O.optionID = @iOptionID_bad
				select @dtExpiry_bad =  expiryDate
						from OptionView O 
						where O.optionID = @iOptionID_bad
				select 	@fStrike_bad = strike,
						@iIsCall_bad = isCall 
						from OptionView where optionID = @iOptionID_bad
				

			set @ERR_MSG = 'Can''t add option with Symbol=' + '''' +  @vcSymbol + ''''  +  
			CASE WHEN @vcUnderlying_bad != @vcUnderlyingSymbol THEN
				', Underlying=' + '''' + @vcUnderlyingSymbol + ''''
			ELSE '' END + 
			CASE WHEN left(@vcSymbol,len(@vcSymbol)-2) != left(@vcSymbol_bad,len(@vcSymbol_bad)-2) THEN
				', Option Root=' + '''' + left(@vcSymbol,len(@vcSymbol)-2) + ''''
			ELSE '' END + 
			CASE WHEN @fStrike!=@fStrike_bad THEN
				', Strike=' + CAST(ROUND(@fStrike, 2) as varchar(10)) 
			ELSE '' END + 
			CASE WHEN cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) != cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) THEN
				', Expiry=' + '''' + cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) + '''' 
			ELSE '' END +  
			CASE WHEN @iIsCall!=@iIsCall_bad THEN
				'''' + ', Call\Put' + '''' + '=' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END  
			ELSE '' END +  
			CASE WHEN @error = -2 THEN
				' because exists option with trades and Symbol=' + '''' +  @vcSymbol_bad + ''''
				WHEN @error = -3 THEN
				' because exists option with orders and Symbol=' + '''' +  @vcSymbol_bad + ''''
				WHEN @error != -2 and @error != -3 THEN
				' because exists option with Symbol=' + '''' +  @vcSymbol_bad + ''''
			ELSE '' END + 
			CASE WHEN @vcUnderlying_bad != @vcUnderlyingSymbol THEN
				', Underlying=' + '''' + @vcUnderlying_bad + ''''
			ELSE '' END + 
			CASE WHEN left(@vcSymbol,len(@vcSymbol)-2) != left(@vcSymbol_bad,len(@vcSymbol_bad)-2) THEN
				', Option Root=' + '''' + left(@vcSymbol_bad,len(@vcSymbol_bad)-2) + ''''
			ELSE '' END + 
			CASE WHEN @fStrike!=@fStrike_bad THEN
				', Strike=' + CAST(ROUND(@fStrike_bad, 2) as varchar(10)) 
			ELSE '' END + 
			CASE WHEN cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) != cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) THEN
				', Expiry=' + '''' + cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) + ''''
			ELSE '' END + 
			CASE WHEN @iIsCall!=@iIsCall_bad THEN
				'''' + ', Call\Put' + '''' + '=' + CASE @iIsCall_bad WHEN 1 THEN 'Call' ELSE 'Put' END
			ELSE '' END +
			CASE WHEN @vcUnderlying_bad = @vcUnderlyingSymbol THEN
			', Underlying=' + '''' + @vcUnderlyingSymbol + ''''
			ELSE '' END
		end	
		Return(-1)
	end
	else
		begin
			set @ERR_MSG = null
			set @iOptionID_bad = null
		end

	if @iOptionID is null
		select @iOptionID_bad = optionID, @vcSymbol_bad = C.symbol
			from OptionView O join ContractView C on O.optionID = C.contractID
			where (optionRootID = @iOptionRootID and datediff(d, expiryDate, @dtExpiry) = 0 and abs(strike - @fStrike) < 0.009 and isCall = @iIsCall)
	else
		select @iOptionID_bad = optionID, @vcSymbol_bad = C.symbol
			from OptionView O join ContractView C on O.optionID = C.contractID
			where (optionID != @iOptionID) and
			(optionRootID = @iOptionRootID and datediff(d, expiryDate, @dtExpiry) = 0 and abs(strike - @fStrike) < 0.009 and isCall = @iIsCall)

	if @iOptionID_bad is not null
	begin
		if @bRemoveOption = 1
			begin
				--===================================================================
				-- update option symbol, if option with same attributes exists
				--===================================================================		
				update contract set symbol = @vcSymbol where contractID = @iOptionID_bad
				set @ERR_MSG = '*'
			end
		else
			begin
				set @ERR_MSG = 'Can''t add option ' + '''' +  @vcSymbol + ''''  +  
				' because already exists option ' + '''' +  @vcSymbol_bad + '''' +
				' with the same parameters'
			end
	end

Return(@@error)


GO
ALTER    PROCEDURE usp_Option_Import
	@iUnderlyingContractID int,
	@vcSymbol varchar(20),
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry datetime,
	@iLotSize int,
	@iMsCacheOptionID int = NULL,
	@vcSymbolUnderlying varchar(20) = NULL,
	@fPriceClose float = NULL
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
			@iLastExpiryID int,
			@vcUnderlyingSymbol varchar(8),
			@ERR_MSG varchar(8000)

	if @vcSymbolUnderlying is null
      		select @vcUnderlyingSymbol = symbol from ContractView where contractID = @iUnderlyingContractID
	else
		set  @vcUnderlyingSymbol = @vcSymbolUnderlying

	--===================================================================			
	-- check the Expiry
	--===================================================================			

	/*if not exists(select * from Expiry where expiryID = @iExpiryID)
		begin
			set @ERR_MSG = 'Incorrect expiration! '  + '''' +  Convert(varchar(50),@dtExpiry) + '''' + ' for symbol=' + '''' + @vcSymbol + '''' +  ' UnderlyingSymbol=' + '''' + @vcUnderlyingSymbol + '''' + ' Strike=' + Convert(varchar(15),@fStrike) + ' expiryID=' + Convert(varchar(15),@iExpiryID)
			Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end
	*/
	--===================================================================			
	-- check symbol of option root
	--===================================================================			  
   DECLARE @vcOptionRootSymbol varchar(5)	
   set 	@vcOptionRootSymbol = left(@vcSymbol,len(@vcSymbol)-2)

   if exists (select optionRootID 
				   from OptionRootView
				   where symbol = @vcOptionRootSymbol and 
					underlyingID<>@iUnderlyingContractID)
		begin			
			Declare @vcDifferentUndSymbol varchar(8)
			select @vcDifferentUndSymbol = C.symbol from ContractView C join
					OptionRootView ort on ort.underlyingID = C.contractID
					where ort.symbol = @vcOptionRootSymbol and
					ort.underlyingID<>@iUnderlyingContractID
					
			set @ERR_MSG = 'Incorrect option symbol! '  + '''' +  @vcSymbol + ''''  +  ': Option Root=' + '''' + @vcOptionRootSymbol + '''' + ' assign to different underlying=' + '''' +  @vcDifferentUndSymbol + '''' + '. Please, rename Option Root and restart importing of market structure.'
			Raiserror(@ERR_MSG,16,1)
			RETURN (-1)
		end  


	--select top 1 @iLastExpiryID = expiryID from ExpiryInCalendarView where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc
	--select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	--===================================================================			
	-- select the @iOptionRootID
	--===================================================================			
	declare @iOptionRootID int,
			@vcOptionRoot varchar(20)
	select @iOptionRootID = optionRootID from OptionRootView where symbol = @vcOptionRootSymbol
			and underlyingID = @iUnderlyingContractID 

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if @iOptionRootID is not null 
		select @OptionID = optionID
			from [OptionView] (nolock)
			where optionRootID = @iOptionRootID and 
				isCall = @iIsCall 
                                and abs(strike - @fStrike) < 0.009 
                                and datediff(m, expiryDate ,@dtExpiry) = 0 

	--===================================================================			
	-- check if option with same symbol exists
	--===================================================================		
	set @ERR_MSG = null

	execute usp_Option_Check @iOptionID = @OptionID, @vcSymbol = @vcSymbol,	@iOptionRootID = @iOptionRootID,
			@dtExpiry = @dtExpiry,@fStrike = @fStrike,@iIsCall = @iIsCall,
			@vcUnderlyingSymbol = @vcUnderlyingSymbol,@bRemoveOption = 1,@ERR_MSG = @ERR_MSG output

	if @ERR_MSG is not null and @ERR_MSG != ''
	begin
		if @ERR_MSG != '*' Raiserror(@ERR_MSG,16,1)
		Return(-1)
	end

	--===================================================================			
	-- make the contract name
	--===================================================================			
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(month, @dtExpiry) +',' + DATENAME(day, @dtExpiry) + ' '  
					 + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)


  	if (@OptionID is Null) 
	begin
		Begin Transaction

			if  @iOptionRootID is null
			begin
				insert into OptionRoot (underlyingID,symbol,lotSize)
					select @iUnderlyingContractID, @vcOptionRootSymbol, isnull(@iLotSize,100)
				set @iOptionRootID = @@identity
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end 
	
			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@cn)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into [Option] (optionID, optionRootID, isCall, strike, expiryDate)
				 values (@OptionID, @iOptionRootID, @iIsCall, @fStrike, @dtExpiry)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			/*if @dtExpiry is not null
			begin
				select @iExpCalendarID = e.expCalendarID from ContractView cv 
					inner join ExpiryCalendarView e on e.expCalendarName = cv.symbol 
					where cv.ContractID = @iUnderlyingContractID and cv.contractTypeID = 1
				
					if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtIndexCalendarExpity, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @OptionID
					end
			end*/
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			DECLARE @error_var int, @rowcount_var int
			UPDATE ContractPrice 
				SET priceClose = isnull(@fPriceClose,priceClose)
			WHERE exchangeID is null and contractID = @OptionID

			SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
			if @error_var = 0
			begin
			  if @rowcount_var = 0
			  begin
				insert into ContractPrice(contractID,exchangeID,priceClose)
					values(@OptionID, null, @fPriceClose)
				if (@@error <> 0) begin rollback tran return(-1) end           
			  end	
			end
                        else
			 if (@@error <> 0) begin rollback tran return(-1) end           



			if @iMsCacheOptionID is null
			begin
				delete from MsOptionCache
				where (
					symbol = @vcSymbol 
					and underlyingID = @iUnderlyingContractID 
					and isCall = @iIsCall 
					and abs(strike - @fStrike) < 0.009
					and datediff(d,expiryDate, @dtExpiry)=0 )
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

		UPDATE ContractPrice 
			SET priceClose = isnull(@fPriceClose,priceClose)
		WHERE exchangeID is null and contractID = @OptionID

		update [Option]
			set 
			IsCall = isnull(@iIsCall, IsCall),
			strike = isnull(@fStrike, Strike),
			expiryDate = isnull(@dtExpiry, expiryDate),
			isManualyEntered = 0
		where optionID = @OptionID

		/*if @dtExpiry is not null
		begin
				select @iExpCalendarID = e.expCalendarID from ContractView cv 
					inner join ExpiryCalendarView e on e.expCalendarName = cv.symbol 
					where cv.ContractID = @iUnderlyingContractID and cv.contractTypeID = 1
				
				if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtIndexCalendarExpity, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @OptionID
					end
		end
*/
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

GO
/*END DATA MODIFICATION SECTION*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.198')
	begin
		update DataInfo set keyValue = '3.46.198' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.198' '3.46.198 update message:' 
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