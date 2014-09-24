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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.46.250' or (keyValue = '4.46.251') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '4.46.251 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '4.46.251' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION SECTION*/

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



ALTER   PROCEDURE dbo.usp_Option_Import
	@iUnderlyingContractID int,
	@vcSymbol varchar(20),
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry datetime,
	@dtExpiryOV datetime,
	@dtTradingClose datetime,
	@iLotSize int,
	@iMsCacheOptionID int = NULL,
	@vcSymbolUnderlying varchar(20) = NULL,
	@fPriceClose float = NULL

as
/*	Created by Michael Malyshkin	*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1
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
		set @ERR_MSG = 'Incorrect expiration! ' + '''' +  Convert(varchar(50),@dtExpiry) + 
			'''' + ' for symbol = ' + '''' + @vcSymbol + '''' + ' UnderlyingSymbol=' + 
			'''' + @vcUnderlyingSymbol + '''' + ' Strike = ' + Convert(varchar(15),@fStrike) + 
			' expiryID = ' + Convert(varchar(15),@iExpiryID)
		Raiserror(@ERR_MSG,16,1)
		Return(-1)
	end */
	--===================================================================			
	-- check symbol of option root
	--===================================================================			  
	DECLARE @vcOptionRootSymbol varchar(5)	
	set 	@vcOptionRootSymbol = left(@vcSymbol,len(@vcSymbol)-2)

	if exists (select optionRootID from OptionRootView
		where symbol = @vcOptionRootSymbol and 
			underlyingID<>@iUnderlyingContractID)
		begin			
			Declare @vcDifferentUndSymbol varchar(8)
			select @vcDifferentUndSymbol = C.symbol from ContractView C join
					OptionRootView ort on ort.underlyingID = C.contractID
					where ort.symbol = @vcOptionRootSymbol and
					ort.underlyingID <> @iUnderlyingContractID

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
	select @iOptionRootID = optionRootID from OptionRootView 
		where symbol = @vcOptionRootSymbol and underlyingID = @iUnderlyingContractID 

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if @iOptionRootID is not null 
		select @OptionID = optionID from [OptionView] (nolock)
			where optionRootID = @iOptionRootID and isCall = @iIsCall 
                                and abs(strike - @fStrike) < 0.009 
                                and datediff(m, expiryDate, @dtExpiry) = 0 

	--===================================================================			
	-- check if option with same symbol exists
	--===================================================================		
	set @ERR_MSG = null

	execute usp_Option_Check @iOptionID = @OptionID, @vcSymbol = @vcSymbol,	@iOptionRootID = @iOptionRootID,
			@dtExpiry = @dtExpiry, @fStrike = @fStrike, @iIsCall = @iIsCall,
			@vcUnderlyingSymbol = @vcUnderlyingSymbol, @bRemoveOption = 1, @ERR_MSG = @ERR_MSG output

	if @ERR_MSG is not null and @ERR_MSG != ''
	begin
		if @ERR_MSG != '*' Raiserror(@ERR_MSG,16,1)
		Return(-1)
	end

	--===================================================================			
	-- make the contract name
	--===================================================================			
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(month, @dtExpiry) +',' + DATENAME(day, @dtExpiry) + ' '  
		+ DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END + ' '
		+ CAST(ROUND(@fStrike, 2) as varchar)
  	if (@OptionID is Null) 
	begin
	Begin Transaction
		if  @iOptionRootID is null
		begin
			insert into OptionRoot (underlyingID, symbol, lotSize)
				select @iUnderlyingContractID, @vcOptionRootSymbol, isnull(@iLotSize,100)
			set @iOptionRootID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		end 
	
		insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol, @cn)
		set @OptionID = @@identity
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		insert into [Option] (optionID, optionRootID, isCall, strike, expiryDate, expiryOV, TradingClose)
			 values (@OptionID, @iOptionRootID, @iIsCall, @fStrike, @dtExpiry, @dtExpiryOV, @dtTradingClose)
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
		declare @error_var int, @rowcount_var int
		update ContractPrice 
			set priceClose = isnull(@fPriceClose, priceClose)
		where exchangeID is null and contractID = @OptionID

		select @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
		if @error_var = 0
		begin
		  if @rowcount_var = 0
		  begin
			insert into ContractPrice(contractID, exchangeID, priceClose)
				values(@OptionID, null, @fPriceClose)
			if (@@error <> 0) begin rollback tran return(-1) end           
		  end	
		end
                else
		if (@@error <> 0) begin rollback tran return(-1) end           

		if @iMsCacheOptionID is null
		begin
			delete from MsOptionCache
			where (	symbol = @vcSymbol 
				and underlyingID = @iUnderlyingContractID 
				and isCall = @iIsCall 
				and abs(strike - @fStrike) < 0.009
				and datediff(d,expiryDate, @dtExpiry)=0 )
		end
		else
		begin
			delete from MsOptionCache where optionID = @iMsCacheOptionID
		end
	Commit Transaction
	end 
 	else
	begin
		update Contract
			set symbol = @vcSymbol, contractName = @cn
			where contractID = @OptionID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract!', 16, 1)
				  RETURN (-1)
			   end 

		if @iMsCacheOptionID is null
		begin
			delete from MsOptionCache
			where (	symbol = @vcSymbol 
				and underlyingID = @iUnderlyingContractID 
				and isCall = @iIsCall 
				and abs(strike - @fStrike) < 0.009
				and expiryDate = @dtExpiry )
		end
		else
		begin
			delete from MsOptionCache where optionID = @iMsCacheOptionID
		end

		if @iOptionRootID is not null
		begin
		  update OptionRoot
			set	lotSize = isnull(@iLotSize, lotSize)
			where optionRootID = @iOptionRootID
		end

		update ContractPrice 
			set priceClose = isnull(@fPriceClose, priceClose)
		where exchangeID is null and contractID = @OptionID

		update [Option]
			set 	IsCall = isnull(@iIsCall, IsCall),
				strike = isnull(@fStrike, Strike),
				expiryDate = isnull(@dtExpiry, expiryDate),
				expiryOV = isnull(@dtExpiryOV, expiryOV),
				TradingClose = isnull(@dtTradingClose, TradingClose),
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
		end*/
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


ALTER    PROC [dbo].[usp_EodFutureOption_Import]
	@iFutureID	int,
	@fStrike	float,
	@dtExpiryDate	datetime,
	@dtExpiryOV	datetime,
	@dtTradingClose	datetime,
	@tiIsCall	tinyint,
	@vcSymbol	varchar(20),
	@vcContractName varchar(255),
	@vcBadFutureOptionName varchar(255) OUTPUT,
	@fPriceClose	float = NULL

AS
	-------------------------------------------------
	SET NOCOUNT ON
	-------------------------------------------------
	DECLARE @error INT		SET @error = 0
	DECLARE	@iFutureOptionID INT
	DECLARE @iFutureOptionID_bad INT
	DECLARE @iExpCalendarID INT 
	
	--===================================================================			
	-- select the @iFutureOptionID
	--===================================================================			
	SELECT @iFutureOptionID = futureOptionID
	FROM FutureOption (NOLOCK)
	WHERE futureID = @iFutureID
		AND isCall = @tiIsCall
		AND ABS(strike - @fStrike) < 0.009
		AND Datediff(d, expiryDate, @dtExpiryDate) = 0
				
	IF NOT EXISTS (SELECT 1 FROM tempdb..sysobjects WHERE ID=OBJECT_ID('tempdb..#FutureContractsForDelete'))
		CREATE TABLE #FutureContractsForDelete(contractID INT)				
	IF @@ERROR <> 0 BEGIN SET @error = -1 GOTO finish END
				
	IF @iFutureOptionID IS NOT NULL
	BEGIN
		BEGIN TRANSACTION
			SELECT @iFutureOptionID_bad = contractID
			FROM Contract
			WHERE symbol = @vcSymbol
				--AND contractID <> @iFutureOptionID
				AND contractTypeID = 5

			IF @iFutureOptionID_bad IS NOT NULL AND @iFutureOptionID_bad != @iFutureOptionID
			BEGIN
				INSERT INTO #FutureContractsForDelete (contractID)  VALUES (@iFutureOptionID_bad)
				IF @@ERROR <> 0 BEGIN SET @error = -4 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
				
				EXEC @error = usp_EodFutureOption_Del

				IF @error <> 0
				BEGIN 
					SET @error = -5
					IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION 
					GOTO finish 
				END				
			END

			/*IF @dtExpiryDate IS NOT NULL
			begin
				select @iExpCalendarID = ecv.ExpCalendarID from Future f
				inner join FutureRoot fr on fr.FutureRootID = f.FutureRootID
				inner join ExpiryCalendarView ecv on ecv.expCalendarName = fr.futureRootSymbol + '_Opt'
				where f.FutureID = @iFutureID

				if @iExpCalendarID is not null
				begin
					exec usp_Expiry_Save @iExpiryID, null, @dtExpiryDate, null, @iExpCalendarID
					update Contract set expCalendarID = @iExpCalendarID where ContractID = @iFutureOptionID--@iFutureID
				end
			end	*/
				
			
			UPDATE Contract  SET symbol = @vcSymbol, contractName = @vcContractName WHERE contractID = @iFutureOptionID
			IF @@ERROR <> 0 BEGIN SET @error = -6 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			UPDATE 
				FutureOption 
			SET 
				actionDate = GETDATE(),
				expiryOV = @dtExpiryOV,
				tradingClose = @dtTradingClose
			WHERE 
				futureOptionID = @iFutureOptionID
			IF @@ERROR <> 0 BEGIN SET @error = -7 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			UPDATE ContractPrice
			SET priceClose = @fPriceClose
			WHERE exchangeID IS NULL AND contractID = @iFutureOptionID
			IF (@@ERROR <> 0) BEGIN SET @error = -10 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
		COMMIT TRANSACTION
	END
	ELSE
	BEGIN
		BEGIN TRANSACTION
			SELECT @iFutureOptionID_bad = contractID
			FROM Contract
			WHERE symbol=@vcSymbol
				AND contractTypeID=5

			IF @iFutureOptionID_bad IS NOT NULL
			BEGIN	
				INSERT INTO #FutureContractsForDelete (contractID)  VALUES (@iFutureOptionID_bad)
				IF @@ERROR <> 0 BEGIN SET @error = -4 iF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
				
				EXEC @error = usp_EodFutureOption_Del

				IF @error <> 0
				BEGIN 
					SET @error = -5
					IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION 
					GOTO finish 
				END				
			END			
				
			INSERT INTO Contract (contractTypeID, symbol, contractName)
				 VALUES (5, @vcSymbol,@vcContractName)
			SET @iFutureOptionID = @@IDENTITY
			IF @@ERROR <> 0 BEGIN SET @error = -8 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			INSERT INTO FutureOption (futureOptionID, futureID, isCall, expiryDate, strike, ExpiryOV, TradingClose)
				 VALUES (@iFutureOptionID, @iFutureID, @tiIsCall, @dtExpiryDate, @fStrike, @dtExpiryOV, @dtTradingClose)
			IF @@ERROR <> 0 BEGIN SET @error = -9 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			/*if @dtExpiryDate is not null
			begin
				select @iExpCalendarID = ecv.ExpCalendarID from Future f
				inner join FutureRoot fr on fr.FutureRootID = f.FutureRootID
				inner join ExpiryCalendarView ecv on ecv.expCalendarName = fr.futureRootSymbol + '_Opt'
				where f.FutureID = @iFutureID

				if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtExpiryDate, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @iFutureOptionID--@iFutureID
					end
			end*/
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			IF NOT EXISTS(SELECT 1 FROM ContractPrice WHERE exchangeID IS NULL AND contractID = @iFutureOptionID)
			BEGIN
				INSERT INTO ContractPrice(contractID, exchangeID, priceClose)
					VALUES(@iFutureOptionID, null, @fPriceClose)
				IF (@@ERROR <> 0) BEGIN SET @error = -10 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
			END 

		COMMIT TRANSACTION
	END 
	 
   	SET @error = @iFutureOptionID
finish:
	RETURN(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER  PROCEDURE dbo.usp_CustomStrikeSkewDefaultPointFromIV_Save 
	@iContractID INT,
	@fStrike FLOAT = NULL,
	@dtExpDate DATETIME = NULL,
	@iType INT,
	@fVolatility FLOAT,
	@dtExpiryOV DATETIME = NULL
AS
/*
	Created BY Michael Malyshkin
	Description: this procedure save inf. into CustomStrikeSkewPoINT table
*/
	-------------------------------------------------
	SET NOCOUNT ON
	-------------------------------------------------
	DECLARE @error int		SET @error = -1

	DECLARE @iCustomStrikeSkewPointID INT 
	SET @iCustomStrikeSkewPointID = NULL

	--check AND ALTER  record in VolaSurfaceData with 'IsDefaultSurface=1'
	DECLARE @iVolaSurfaceDataID INT 
	SET @iVolaSurfaceDataID = 0

	EXEC @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	-- mark olf strile skew points AS deleted
	--UPDATE CustomStrikeSkewPoint	SET actionID = 3
	DELETE
	FROM
		CustomStrikeSkewPoint
	WHERE 
		volaSurfaceDataID = @iVolaSurfaceDataID AND
		actionID < 3 AND
		DATEDIFF(d, actionDate, GetDate())>0

	SELECT
		@iCustomStrikeSkewPointID = CustomStrikeSkewPointID
	FROM
		CustomStrikeSkewPoint 
	WHERE
		volaSurfaceDataID = @iVolaSurfaceDataID AND
		ABS(strike - @fStrike) < 0.009 AND
		type = @iType AND
		ABS(DateDiff(d, expiryDate, @dtExpDate)) < 2 AND
		actionID < 3
  
	BEGIN TRANSACTION
		IF @iCustomStrikeSkewPointID IS NULL 
		BEGIN
			DECLARE @iContractType as integer 	
			DECLARE @dtOurExpiry as datetime
			DECLARE @dtOurExpiryOV as datetime

			select @iContractType = ContractTypeID from Contract where ContractID = @iContractID
			--Stock / Index
			IF @iContractType = 1 or @iContractType =2   
			BEGIN
				select distinct @dtOurExpiry = o.expiryDate, @dtOurExpiryOV = o.expiryOV from [option] o
					inner join OptionRoot orr on orr.OptionRootId = o.optionRootID
					inner join ContractView uc on orr.UnderlyingID = uc.ContractID  and uc.ContractID = @iContractID
				where abs(datediff(d, o.expiryDate, @dtExpDate))<2
				--group by o.expiryDate
			END

			--Future Underlying
			IF @iContractType = 6 
			BEGIN
				select distinct @dtOurExpiry = o.expiryDate, @dtOurExpiryOV = o.expiryOV from [futureOption] o
					inner join Future f on o.futureID = f.futureID
					inner join FutureRoot fr on f.futureRootID = fr.futureRootID
					inner join ContractView uc on uc.ContractID = fr.UnderlyingID and uc.ContractID = @iContractID
				where abs(datediff(d, o.expiryDate, @dtExpDate))<2
				--group by o.expiryDate			
			END
			if @dtOurExpiry is null
				set @dtOurExpiry = @dtExpDate

			INSERT INTO 
				CustomStrikeSkewPoINT
					(
						volaSurfaceDataID,
						type,
						strike,
						volatility,
						isBasePoint,
						expiryDate,
						expiryOV
					)
			VALUES	
				(
					@iVolaSurfaceDataID,
					@iType,	
					@fStrike,
					@fVolatility,
					-1,
					@dtOurExpiry,
					@dtOurExpiryOV
				)
	
				IF @@ERROR <> 0 BEGIN SET @error = 1 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

		END
		ELSE
		BEGIN
			UPDATE 
				CustomStrikeSkewPoint
			SET 
				volatility = ISNULL(@fVolatility,volatility)	
				--volaSurfaceDataID = ISNULL(@iVolaSurfaceDataID,volaSurfaceDataID),
				--type = ISNULL(@iType,type),
				--strike = ISNULL(@fStrike,strike),
				--expiryDate = ISNULL(@dtExpDate, expiryDate)
			WHERE
				customStrikeSkewPointID = @iCustomStrikeSkewPointID
	
	   		IF @@ERROR <> 0 BEGIN SET @error = 2 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
		END
	COMMIT TRANSACTION

    SET @error = 0

finish:
	RETURN(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



ALTER     Procedure dbo.usp_CustomStrikeSkewPoint_Save 
	@iCustomStrikeSkewPointID int = Null output,
	@iVolaSurfaceDataID int = Null,
	@iType int = Null,
	@fStrike float = Null,
	@fVolatility float = Null,
	@dtExpDate datetime = Null,
	@iIsBasePoint int = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save inf. into CustomStrikeSkewPoint table
*/
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if exists (
		select * 
		from CustomStrikeSkewPoint 
		where volaSurfaceDataID = @iVolaSurfaceDataID and strike = @fStrike and 
			type = @iType and expiryOV = @dtExpDate and actionID < 3 and 
			(@iCustomStrikeSkewPointID <> CustomStrikeSkewPointID or @iCustomStrikeSkewPointID is null)
	)
	begin
		if @iCustomStrikeSkewPointID is null
			select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
			from CustomStrikeSkewPoint 
			where volaSurfaceDataID = @iVolaSurfaceDataID and strike = @fStrike and 
				type = @iType and expiryOV = @dtExpDate and actionID < 3
			else if @iCustomStrikeSkewPointID is not null 
				begin
					Raiserror ('Row in CustomStrikeSkewPoint table  with same volaSurfaceDataID, strike, type and expDate already exists!', 16, 1)
					Return (-1)
				end
		end 
	
	DECLARE @errStrk1 AS INT
	DECLARE @errStrk2 AS INT

	IF @dtExpDate IS NULL
	BEGIN
		SET @errStrk1 = CAST( @fStrike AS INT ) 
		SET @errStrk2 = CAST( ( @fStrike - FLOOR( @fStrike ) ) * 10 AS INT )
		RAISERROR ('Expiration date was not supplied for strike %d.%d, VolSurf %d', 16, 1, @errStrk1, @errStrk2 , @iVolaSurfaceDataID ) WITH LOG
		RETURN -1
	END			

-- 	SELECT @expInCalendarID = expiryInCalendarID
-- 	FROM ExpiryInCalendarView 
-- 	WHERE expiryDate = @dtExpDate

	Begin Transaction

	if @iCustomStrikeSkewPointID is null 
	begin
		insert into CustomStrikeSkewPoint ( volaSurfaceDataID, type, strike, volatility, expiryOV, isBasePoint) 
			values (@iVolaSurfaceDataID, @iType, @fStrike, @fVolatility,  @dtExpDate, @iIsBasePoint)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iCustomStrikeSkewPointID=@@IDENTITY
	end
	else
	begin
		update CustomStrikeSkewPoint set 
			volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
			type = isnull(@iType,type),
			strike = isnull(@fStrike,strike),
			volatility = isnull(@fVolatility,volatility),
			expiryOV = isnull(@dtExpDate,expiryOV),
			isBasePoint = isnull(@iIsBasePoint,isBasePoint)
		where customStrikeSkewPointID=@iCustomStrikeSkewPointID
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



/*DATA MODIFICATION COMPLETE*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.46.251')
	begin
		update DataInfo set keyValue = '4.46.251' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 ROLLBACK TRANSACTION
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
	select 'DB VERSION WAS CHANGED TO 4.46.251' '4.46.251 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
