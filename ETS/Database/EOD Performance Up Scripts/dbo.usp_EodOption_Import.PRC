SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOption_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodOption_Import]
GO





CREATE     PROC dbo.usp_EodOption_Import
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
			FROM Contract WITH(NOLOCK)
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
			FROM Contract WITH(NOLOCK)
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

