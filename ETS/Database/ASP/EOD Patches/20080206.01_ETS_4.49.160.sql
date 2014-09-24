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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.159' or (keyValue = '4.49.160')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.160 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.160' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

/*DATA MODIFICATION SECTION*/
/* EodManager Option Im[ort Procedures updated for working with expiryOV */
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER    PROC dbo.usp_EodOption_Import
	@iRootID INT,
	@dtExpiryDate DATETIME,
	@dtExpiryOV DATETIME = NULL,
	@dtTradingClose DATETIME = NULL,
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
		AND DATEDIFF(d, expiryDate, @dtExpiryDate) = 0
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
				IF NOT EXISTS (SELECT top 1 * FROM Trade WHERE ContractID = @iOptionID_bad) AND
					NOT EXISTS (SELECT top 1 *  FROM [Order] WHERE ContractID = @iOptionID_bad)
				BEGIN	
					EXEC @error = usp_Option_Del @iOptionID_bad			
	
					IF @@ERROR != 0 OR @error != 0
					BEGIN 
						if @@ERROR != 0 set @error = @@ERROR
						IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION 
						GOTO finish 
					END
				END
				ELSE
					UPDATE Contract
					SET symbol = symbol + '*'
					WHERE ContractID = @iOptionID_bad
			END
				
			UPDATE Contract 
			SET symbol = @vcSymbol, contractName = @vcContractName
			WHERE contractID = @OptionID AND actionID < 3
			IF @@ERROR <> 0 BEGIN SET @error = -5 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END

			UPDATE [Option]  
			SET isManualyEntered = 0,
			    expiryDate = @dtExpiryDate,
			    expiryOV   = ISNULL(@dtExpiryOV, ExpiryOV),
			    TradingClose = ISNULL(@dtTradingClose, TradingClose)
			WHERE optionID = @OptionID and actionID < 3
			IF @@ERROR <> 0 BEGIN SET @error = -6 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END

			IF @bSaveClosePrice IS NOT NULL AND  @bSaveClosePrice = 1
            BEGIN 	
				UPDATE ContractPrice
				SET priceClose = @fPriceClose
				WHERE contractID = @OptionID AND exchangeID IS NULL
				IF @@ERROR <> 0 BEGIN SET @error = -6 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END
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
				IF NOT EXISTS (SELECT top 1 * FROM Trade WHERE ContractID = @iOptionID_bad) AND
					NOT EXISTS (SELECT top 1 * FROM [Order] WHERE ContractID = @iOptionID_bad)
				BEGIN	
					EXEC @error = usp_Option_Del  @iOptionID_bad			
	
					IF @@ERROR != 0 OR @error != 0
					BEGIN 
						if @@ERROR != 0 set @error = @@ERROR
						IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION 
						GOTO finish 
					END
				END
				ELSE
					UPDATE Contract
					SET symbol = symbol + '*'
					WHERE ContractID = @iOptionID_bad
			END

			INSERT INTO Contract (contractTypeID, symbol, contractName)
				 VALUES (3, @vcSymbol, @vcContractName)

			SET @OptionID = @@IDENTITY
			IF @@ERROR <> 0 BEGIN SET @error = -5 IF @@TRANCOUNT >0 ROLLBACK TRANSACTION GOTO finish END

			INSERT INTO [Option] (optionID, optionRootID, isCall, strike, expiryDate, expiryOV, TradingClose)
				 VALUES (@OptionID, @iRootID, @iIsCall, @fStrike, @dtExpiryDate, @dtExpiryOV, @dtTradingClose)
			IF @@ERROR <> 0 BEGIN SET @error = -6 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END
	
			IF NOT EXISTS(SELECT 1 FROM ContractPrice WHERE exchangeID IS NULL AND contractID = @OptionID)
			BEGIN
	  		    IF @bSaveClosePrice IS NOT NULL AND  @bSaveClosePrice = 1
					INSERT INTO ContractPrice(contractID, exchangeID, priceClose)
						VALUES(@OptionID, NULL, @fPriceClose)
                ELSE
					INSERT INTO ContractPrice(contractID, exchangeID, priceClose)
						VALUES(@OptionID, NULL, NULL)

			    IF (@@ERROR <> 0) BEGIN SET @error = -7 iF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END
			END 
		
		COMMIT TRANSACTION
	END 
	 
   	SET @error = @OptionID
finish:
	IF @@TRANCOUNT > 0
	BEGIN	
		SET @error = -1000 * @@TRANCOUNT
		ROLLBACK TRANSACTION	
	END 

	RETURN(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



ALTER PROC usp_EodFutureOption_Import
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

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
if not exists (SELECT name FROM syscolumns WHERE id = object_id(N'[MsOptionCache]') and (name = 'expiryOV' or name = 'TradingClose'))
begin
	ALTER TABLE [MsOptionCache] 
		ADD [expiryOV] [datetime] DEFAULT NULL,
		    [TradingClose] [datetime] NOT NULL DEFAULT ('1900-01-01 00:00:00.000')
end
GO
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER     PROCEDURE dbo.usp_EodMsOptionCache_Save
	@iUnderlyingID INT,
	@vcSymbol VARCHAR(20),
	@dtExpiry DATETIME,
	@dtExpiryOV DATETIME = NULL,
	@dtTradingClose DATETIME = NULL,
	@fStrike FLOAT,
	@tiIsCall TINYINT,
	@iLotSize INT

AS
	----------------------------------------------------------------------
	SET NOCOUNT ON
	----------------------------------------------------------------------
	DECLARE @error INT		SET @error = 0
	DECLARE @vcUnderlyingSymbol VARCHAR(20)
	DECLARE @vcError AS VARCHAR(8000)

	-- check parameter
	IF @iUnderlyingID IS NULL OR @vcSymbol IS NULL OR @dtExpiry IS NULL OR ISNULL(@fStrike, 0) <= 0
	BEGIN
		RAISERROR ('Invalid parameter!', 16, 1)
		SET @error = -1
		GOTO finish 
	END

	-- check underlying existance
	IF NOT EXISTS (SELECT 1 FROM MsUnderlyingCache WHERE underlyingID = @iUnderlyingID)
	BEGIN
		RAISERROR ('Invalid underlying contract!', 16, 1)
		SET @error = -2
		GOTO finish 
	END

	-- check option root symbol
	IF LEN(@vcSymbol) < 3
	BEGIN
		SET @vcError = 'Invalid option symbol ''' + @vcSymbol + ''': at least 3 characters expected!'
		RAISERROR(@vcError, 16, 1)
		SET @error = -4
		GOTO finish 
	END

	-- correct lotsize
	IF ISNULL(@iLotSize, 0) <= 0
		SET @iLotSize = 100

	INSERT INTO MsOptionCache(underlyingID, symbol, expiryDate, strike, isCall, lotSize, expiryOV, tradingClose)
		VALUES(@iUnderlyingID, @vcSymbol, @dtExpiry, @fStrike, @tiIsCall, @iLotSize, @dtExpiryOV, @dtTradingClose)
	
	IF @@ERROR <> 0 BEGIN SET @error = -5 GOTO finish END

finish:
	RETURN @error


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


GO
--------------------------------------------------------------------------------------------------------------------------------------------------------
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.160')
	begin
		update DataInfo set keyValue = '4.49.160' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.160' '4.49.160 update message:' 
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

