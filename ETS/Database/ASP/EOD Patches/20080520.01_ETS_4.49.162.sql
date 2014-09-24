/*
	Contains stored procedures for ETS ASP (Institutional) database
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
--BEGIN TRANSACTION
--GO
--SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
--GO

/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.161' or (keyValue = '4.49.162')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.162 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.162' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION SECTION*/
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




ALTER        PROCEDURE usp_Option_Import
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

	execute usp_Option_Check @iOptionID = @OptionID, @vcSymbol = @vcSymbol,	@iOptionRootID = @iOptionRootID, 			@dtExpiry = @dtExpiry,@fStrike = @fStrike,@iIsCall = @iIsCall,
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
			where optionID = @iMsCacheOptionID 		end

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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




ALTER    PROCEDURE dbo.usp_Option_Bulk_Import
	@iUnderlyingContractID int,
	@iBulkXMLData ntext
as

	DECLARE @vcSymbol varchar(20)
	DECLARE @iIsCall  tinyint
	DECLARE @fStrike  float
	DECLARE @dtExpiry datetime
	DECLARE @dtExpiryOV	datetime
	DECLARE @dtTradingClose	datetime
	DECLARE @iLotSize int
	DECLARE @iDoc 	  int
	DECLARE @fClosePrice float
	DECLARE	@vcUnderlyingSymbol varchar(8)
	DECLARE @isError  tinyint
	DECLARE @ERR_MSG  varchar(8000)
   	
	select @vcUnderlyingSymbol = symbol from ContractView where contractID = @iUnderlyingContractID
	set @isError = 0
	exec sp_xml_preparedocument @iDoc OUTPUT, @iBulkXMLData
	DECLARE BulkDataCursor CURSOR FOR
		select * 
			FROM OPENXML(@iDoc, '/Import/Option')
 				WITH (Symbol	  varchar(20),
				      IsCall      tinyint ,    	
				      Strike      float ,    	
               			      Expiry	  datetime ,
				      ExpiryOV	  datetime,
				      TradingClose datetime,
				      LotSize     int,
				      ClosePrice  float )

	
	OPEN BulkDataCursor
	FETCH NEXT FROM BulkDataCursor 
	INTO  @vcSymbol, @iIsCall, @fStrike, @dtExpiry, @dtExpiryOV, @dtTradingClose, @iLotSize,@fClosePrice

	WHILE @@FETCH_STATUS = 0
	BEGIN
		declare @iRet int
		--select @iUnderlyingContractID, @vcSymbol, @iIsCall, @fStrike, @dtExpiry,@iLotSize
		exec @iRet = usp_Option_Import @iUnderlyingContractID, @vcSymbol, @iIsCall, @fStrike, @dtExpiry, @dtExpiryOV, @dtTradingClose, @iLotSize, NULL, @vcUnderlyingSymbol, @fClosePrice
		if @iRet <> 0 
		begin 
		   set @isError = -1	
		   exec usp_EodMsOptionCache_Save @iUnderlyingContractID, @vcSymbol, @dtExpiry, @dtExpiryOV, @dtTradingClose, @fStrike, @iIsCall, @iLotSize
		end

		FETCH NEXT FROM BulkDataCursor 
		INTO @vcSymbol, @iIsCall, @fStrike, @dtExpiry, @dtExpiryOV, @dtTradingClose, @iLotSize,@fClosePrice
	END


	CLOSE      BulkDataCursor
	DEALLOCATE BulkDataCursor

	exec sp_xml_removedocument @iDoc
	return @isError
	



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

-----------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO






ALTER    PROC usp_EodFutureOption_Import
	@iFutureID INT,
	@fStrike FLOAT,
	@dtExpiryDate DATETIME,
	@dtExpiryOV DATETIME = NULL,
	@dtTradingClose DATETIME = NULL,
	@tiIsCall TINYINT,
	@vcSymbol VARCHAR(20),
	@vcContractName VARCHAR(255),
	@vcBadFutureOptionName VARCHAR(255) OUTPUT,
	@fPriceClose FLOAT = NULL
AS
	-------------------------------------------------
	SET NOCOUNT ON
	-------------------------------------------------
	DECLARE @error INT	SET @error = 0
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
				INSERT INTO #FutureContractsForDelete (contractID) VALUES (@iFutureOptionID_bad)
				IF @@ERROR <> 0 BEGIN SET @error = -4 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END
				
				EXEC @error = usp_EodFutureOption_Del

				IF @error <> 0
				BEGIN 
					SET @error = -5
					IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION 
					GOTO finish 
				END				
			END

			UPDATE Contract  SET symbol = @vcSymbol, contractName = @vcContractName WHERE contractID=@iFutureOptionID
			IF @@ERROR <> 0 BEGIN SET @error = -6 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END

			UPDATE FutureOption 
			SET	actionDate = GETDATE(),
				expiryOV = ISNULL(@dtExpiryOV, expiryOV),
				TradingClose = ISNULL(@dtTradingClose, TradingClose) 
			WHERE futureOptionID=@iFutureOptionID

			IF @@ERROR <> 0 BEGIN SET @error = -7 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END

			UPDATE ContractPrice
			SET priceClose = @fPriceClose
			WHERE exchangeID IS NULL AND contractID = @iFutureOptionID
			IF (@@ERROR <> 0) BEGIN SET @error = -10 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END
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
				IF @@ERROR <> 0 BEGIN SET @error = -4 iF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END
				
				EXEC @error = usp_EodFutureOption_Del

				IF @error <> 0
				BEGIN 
					SET @error = -5
					IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION 
					GOTO finish 
				END				
			END			
				
			INSERT INTO Contract (contractTypeID, symbol, contractName)
				 VALUES (5, @vcSymbol, @vcContractName)
			SET @iFutureOptionID = @@IDENTITY
			IF @@ERROR <> 0 BEGIN SET @error = -8 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END

			INSERT INTO FutureOption (futureOptionID, futureID, isCall, expiryDate, strike, expiryOV, TradingClose)
				 VALUES (@iFutureOptionID, @iFutureID, @tiIsCall, @dtExpiryDate, @fStrike, @dtExpiryOV, @dtTradingClose)
			IF @@ERROR <> 0 BEGIN SET @error = -9 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END

			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			IF NOT EXISTS(SELECT 1 FROM ContractPrice WHERE exchangeID IS NULL AND contractID = @iFutureOptionID)
			BEGIN
				INSERT INTO ContractPrice(contractID, exchangeID, priceClose)
					VALUES(@iFutureOptionID, null, @fPriceClose)
				IF (@@ERROR <> 0) BEGIN SET @error = -10 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END
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

-------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




ALTER    PROCEDURE dbo.usp_CustomStrikeSkewDefaultPointFromIV_Save 
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
				select distinct @dtOurExpiry = o.expiryDate, @dtOurExpiryOV = o.expiryOV  from [option] o
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

-----------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




ALTER      Procedure dbo.usp_CustomStrikeSkewPoint_Save 
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
		insert into CustomStrikeSkewPoint ( volaSurfaceDataID, type, strike, volatility, expiryOV, 
							isBasePoint) 
			values (@iVolaSurfaceDataID, @iType, @fStrike, @fVolatility,  @dtExpDate, 
							@iIsBasePoint)
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
			expiryOV = isnull(@dtExpDate,expiryDate),
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

------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO






ALTER   procedure dbo.usp_Option_Save
		@iOptionID int,
		@vcSymbol varchar(20) = Null,
		@iExchangeID int, 
		@iUnderlyingContractID int,
		@iIsCall tinyint = Null,
		@fStrike float = Null,
		@dtExpiry datetime = Null,
		@dtExpiryOV datetime = null,
		@dtTradeClose datetime = null,
		@tiIsManualyEntered tinyint /* now this flag if not 0, means that this option is flex */
as
-- changed by VK - flex option support added
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

	   if exists (select optionRootID from OptionRootView
		   where symbol = left(@vcSymbol,len(@vcSymbol)-2) and underlyingID != @iUnderlyingContractID)
		begin
		  RAISERROR ('Error. Can''t insert or update Option. Current Root assigned to different Underlying', 16, 1)
		  RETURN (-1)
		end  

	--===================================================================			
	-- check if option with same symbol exists
	--===================================================================		
	declare	@iOptionRootID int,
		@ContractName varchar(255) --,
		--@iExpiryID int,
		--@iExpCalID INT

	select @iOptionRootID = optionRootID from OptionRootView where symbol = left(@vcSymbol,len(@vcSymbol)-2)  and 
		underlyingID=@iUnderlyingContractID	

	declare @ERR_MSG varchar(8000),
		@vcUnderlyingSymbol varchar(10)

	select @vcUnderlyingSymbol = symbol 
		from ContractView 
		where contractID = @iUnderlyingContractID and contractTypeID <= 2

	--set @iExpiryID = cast (@dtExpiry As int) --(year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
	

	execute usp_Option_Check @iOptionID = @iOptionID, 
				@vcSymbol = @vcSymbol,	
				@iOptionRootID = @iOptionRootID,
				@dtExpiry = @dtExpiry,
				--@iExpiryID = @iExpiryID,
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
	IF @tiIsManualyEntered = 0 
   		set @ContractName = @vcUnderlyingSymbol + ' ' + datename(day, @dtExpiry) + ' ' + datename(month, @dtExpiry) + 
					' ' + datename(year, @dtExpiry) + ' ' + CASE @iIsCall when 1 then 'Call' else 'Put' end +
					' ' + cast(round(@fStrike, 2) as varchar)
	ELSE
		set @ContractName = @vcUnderlyingSymbol + ' ' + datename(day, @dtExpiry) + ' ' + datename(month, @dtExpiry) + 
					' ' + datename(year, @dtExpiry) + ' ' + CASE @iIsCall when 1 then 'Call' else 'Put' end +
					' ' + cast(round(@fStrike, 2) as varchar) + ' (OTC)'

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
			
			insert into [Option] (optionID, optionRootID, isCall, strike, expiryDate, ExpiryOV, TradingClose)
				values (@iOptionID, @iOptionRootID, @iIsCall, @fStrike, @dtExpiry, @dtExpiryOV, @dtTradeClose)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			/* Change ExpiryOV for all Options with the same Root */
			update [Option]
				set ExpiryOV = isnull(@dtExpiryOV, ExpiryOV) where optionRootID = @iOptionRootID and expiryDate = @dtExpiry
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end

			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		 end
	   else
		 begin
			update Contract set symbol = isNull(@vcSymbol,symbol), 
				contractName = isNull(@ContractName,contractName)
			where contractID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			declare @iOldOptionID as int
			select @iOldOptionID = optionRootID from [Option] 
				where optionID = @iOptionID

			update [Option]
				set optionRootID = @iOptionRootID,
					isCall = isNull(@iIsCall, isCall),
					strike = isNull(@fStrike, strike),
					expiryDate = isNull(@dtExpiry, expiryDate),
					ExpiryOV = isnull(@dtExpiryOV, ExpiryOV),
					TradingClose = isnull(@dtTradeClose, TradingClose)
					--expiryID = isNull(@iExpiryID, expiryID)
				where optionID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			/* Change ExpiryOV for all Options with the same Root */
			update [Option]
				set ExpiryOV = isnull(@dtExpiryOV, ExpiryOV) where optionRootID = @iOptionRootID and expiryDate = @dtExpiry
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end

			if @iOldOptionID is not null and @iOptionRootID is not null
			begin
			  update Trade set OptionRootId = @iOptionRootID where 	OptionRootId = @iOldOptionID
	  		  if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		 end		

		if @tiIsManualyEntered in (0,1) 
		begin
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

-----------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




ALTER         PROC dbo.usp_BoCustomStrikeSkewPoint_Default_Save 
	@iContractID int,
	@fStrike float,
	@dtExpiry datetime,
	@fVolatility float
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	declare @iCustomStrikeSkewPointID int 
		set @iCustomStrikeSkewPointID = Null

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int 
		set @iVolaSurfaceDataID = 0

	declare @iType int
		set @iType = 2 --OTM_IVTYPE

	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
		from CustomStrikeSkewPoint 
		where volaSurfaceDataID = @iVolaSurfaceDataID 
			and abs(strike - @fStrike) < 0.009
			and type = @iType 
			and abs(datediff(d, expiryDate, @dtExpiry))<2
			and actionID < 3

	declare @dtExpiryOV datetime
	set @dtExpiryOV = @dtExpiry
	
	if (isnull(@iCustomStrikeSkewPointID, -1) < 0)
	begin
		select  CE.expiryOV 
		from 	(
				select 
				distinct expiryOV, expiryDate 
				from OptionView 
				where OptionRootID in (
							select OptionRootID 
							from OptionRootView 
							where Underlyingid = @iContractID
						       )
			)CE
		where 
			abs(datediff(d, CE.expiryDate, @dtExpiry)) = 0
	end
  
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					isBasePoint,
					expiryDate,
					expiryOV)
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						-1,
						@dtExpiry,
						@dtExpiryOV)
	
				if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

			end
		else
			begin
				update CustomStrikeSkewPoint set 
					--volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					--type = isnull(@iType,type),
					--strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility)
					--expiryDate = isnull(@iExpiryID,expiryID )
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
	   			if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction

finish:
	Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

---------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




ALTER   proc usp_AspEod_DefaultIVSurface_Load
as
SET NOCOUNT ON

BEGIN TRANSACTION
	insert into VolaSurfaceData(contractID, isDefaultSurface, baseUnderlinePrice)
	select c.ContractID,1,0 from Contract c
	where   contractTypeId in (1, 2, 6)
		and (exists(select top 1 1 from optionRoot where c.ContractId = underlyingid) -- check if optionable
			or
			exists(select top 1 1 from futureRoot where c.ContractId = underlyingid) 
		)
		and not exists(select top 1 1 from VolaSurfaceData where c.ContractId = ContractId and isDefaultSurface = 1) -- check no default IV

	if @@error <> 0 
	begin	
		rollback tran
		RAISERROR ('Insert Default Volasurfaces failed', 16, 1)
		return (-1)
	end

-- Delete obsolete strike points for instruments which has IV surfaces in IV database
	delete from CustomStrikeSkewPoint where  customStrikeSkewPointID in (
		select cssp.customStrikeSkewPointID from ContractView cv
		inner join VolaSurfaceData vsd on vsd.contractID = cv.contractid
		inner join CustomStrikeSkewPoint cssp on cssp.volaSurfaceDataID=vsd.volaSurfaceDataID and datediff(d, cssp.actionDate, getDate())>0
		where cv.contractTypeId in (1, 2, 6) and cssp.actionID < 3 
			and exists( select top 1 1 from IVUnderlyingPointsRawData where iContractID = cv.ContractID)
	)
	
	if @@error <> 0 
	begin	
		rollback tran
		RAISERROR ('Delete Obsolete skew points failed', 16, 1)
		return (-2)
	end

-- Update existing
	update CustomStrikeSkewPoint set
		volatility = isnull(IV.fIV * 100, volatility),
		ExpiryDate = IV.dtExpDate,
		ExpiryOV = IV.dtExpDateOV
--	select 	vsd.VolaSurfaceDataID , IV.fStrike, IV.iExpiryID, IV.fIV
	from CustomStrikeSkewPoint cssp, ContractView c
	inner join IVUnderlyingPointsRawData IV on c.ContractID = IV.iContractID
	inner join VolaSurfaceData vsd on vsd.contractID = c.contractid
	where vsd.VolaSurfaceDataID = cssp.volaSurfaceDataID
		and abs(cssp.strike - IV.fStrike) < 0.009	
		and abs(datediff(d,cssp.ExpiryDate , IV.dtExpDate)) <= 3
			  
	if @@error <> 0 
	begin	
		rollback tran
		RAISERROR ('Update existing skew points failed', 16, 1)
		return (-3)
	end


-- Insert new IV points from IV database
	insert into CustomStrikeSkewPoint (
			volaSurfaceDataID,
			type,
			strike,
			volatility,
			isBasePoint,
			expiryDate,
			expiryOV)
	select vsd.VolaSurfaceDataID as volaSurfaceDataID,
	       2                     as Type,
               IV.fStrike            as Strike,
	       /*IV.fIV                as */
		volatility =
		case
			when IV.fIV is null then 20
			when IV.fIV = 0 then 20
			when IV.fIV < 0 then 20
			else IV.fIV * 100
		end,
               	-1                    as isBasePoint,
	      	IV.dtExpDate	     as expiryDate,
	      	IV.dtExpDateOV	     as expiryOV
	from IVUnderlyingPointsRawData IV
	inner join ContractView c on c.ContractID = IV.iContractID
	inner join VolaSurfaceData vsd on vsd.contractID = c.contractid
	where 
		not exists(select top 1 1 from CustomStrikeSkewPoint cssp 
	                   where cssp.volaSurfaceDataID = vsd.volaSurfaceDataID 
				   and cssp.actionID < 3 
	                           and abs(datediff(d,cssp.ExpiryDate, IV.dtExpDate)) <= 3
				   and abs(cssp.strike - IV.fStrike) < 0.009	
			  )
	
	--delete old points without expiryOV
	delete
	from 
		CustomStrikeSkewPoint 
	where 
		isnull(expiryOV, -1) < 0
	
	if @@error <> 0 
	begin	
		rollback tran
		RAISERROR ('Insert new skew points failed', 16, 1)
		return (-4)
	end
		
COMMIT TRANSACTION




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

---------------------------------------------------------------------------------------------------------------

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


ALTER   PROC usp_MmVegaWeight_Get
AS
set nocount on

	select dt.ExpiryOV as dtExpiryOV, VegaWeight as fVegaWeight 
	from (
			select distinct(CONVERT(varchar, expiryOV, 101)) as expiryOV from FutureOption
			where datediff(d, getDate(), expiryOV) >=0
		union
			select distinct(maturityDate) as expiryOV from Future
			where datediff(d, getDate(), maturityDate) >=0
		union
			select distinct(CONVERT(varchar, expiryOV, 101)) as expiryOV from OptionView
			where datediff(d, getDate(), expiryOV) >=0
		) dt
	left join VegaWeight vw  on datediff(d, vw.ExpiryDate,dt.expiryOV) = 0  
	order by dt.ExpiryOV

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
------------------------------------------------------------------------------------------------------------------
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.162')
	begin
		update DataInfo set keyValue = '4.49.162' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
--IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
--GO
--IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
--GO
--IF EXISTS (SELECT * FROM #tmpErrors)
--BEGIN
--	ROLLBACK TRANSACTION
	--SELECT * FROM #tmpErrors
--END
GO
--IF @@TRANCOUNT > 0 
--BEGIN
	select 'DB VERSION WAS CHANGED TO 4.49.162' '4.49.162 update message:' 
	select 'The database update succeeded'
--
IF @@TRANCOUNT > 0 
BEGIN
	COMMIT TRANSACTION
END
--	ELSE select 'The database update failed'
--GO
DROP TABLE #tmpErrors
GO
