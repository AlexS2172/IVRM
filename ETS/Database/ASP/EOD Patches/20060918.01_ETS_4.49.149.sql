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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.148' or (keyValue = '4.49.149')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.149 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.148' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

ALTER      PROC usp_EodFuture_Import
	@iFutureRootID INT,
	@dtMaturityDate datetime,
	@vcSymbol VARCHAR(20),
	@vcContractName VARCHAR(255),
	@iLotSizeID INT,
	@iOptionLotSizeID INT,
	@tiIsActive TINYINT OUTPUT,
	@vcBadFutureName VARCHAR(255) OUTPUT,
	@fPriceClose FLOAT = NULL,
	@fTickSize FLOAT = NULL,
	@iQuotationUnitID INT = NULL,
	@vcQuotationUnitCurrency VARCHAR(3) = NULL
AS
	-------------------------------------------------
	SET NOCOUNT ON
	-------------------------------------------------

	DECLARE @error INT		SET @error = 0

	DECLARE	@iFutureID INT,
		@iUndPriceProfileID INT,
		@iOptPriceProfileID INT,
		@iFutureID_bad INT
	DECLARE @iExpCalENDarID INT

	IF NOT EXISTS (SELECT * FROM tempdb..sysobjects WHERE ID=OBJECT_ID('tempdb..#FutureContractsForDelete'))
		CREATE TABLE #FutureContractsForDelete(contractID INT)				
	IF @@ERROR <> 0 BEGIN SET @error = -1 GOTO finish END
	
	--===================================================================			
	-- SELECT the @iFutureID
	--===================================================================			
	SELECT @iFutureID = futureID, @tiIsActive = isActive
			FROM Future (NOLOCK)
			WHERE futureRootID = @iFutureRootID
                              AND abs(datediff(d, maturityDate, @dtMaturityDate)) <25

	IF @iFutureID IS NOT NULL
	BEGIN
		BEGIN TRANSACTION
			SELECT @iFutureID_bad = contractID
			FROM Contract
			WHERE symbol=@vcSymbol
				--AND contractID<>@iFutureID
				AND contractTypeID=4

			IF @iFutureID_bad IS NOT NULL AND @iFutureID_bad != @iFutureID
			BEGIN
				INSERT INTO #FutureContractsForDelete (contractID)  VALUES (@iFutureID_bad)
				IF @@ERROR <> 0 BEGIN SET @error = -4 IF @@TRANCOUNT >0 ROLLBACK TRANSACTION GOTO finish END

				INSERT INTO #FutureContractsForDelete 
					SELECT futureoptionID FROM FutureOption WHERE futureID=@iFutureID_bad
				IF @@ERROR <> 0 BEGIN SET @error = -5 IF @@TRANCOUNT >0 ROLLBACK TRANSACTION GOTO finish END
				
				EXEC @error = usp_EodFuture_Del

				IF @error <> 0
				BEGIN 
					SET @error = -6
					IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION 
					GOTO finish 
				END				
			END



			UPDATE Contract  SET symbol = @vcSymbol, contractName = @vcContractName WHERE contractID=@iFutureID
			IF @@ERROR <> 0 BEGIN SET @error = -7 IF @@TRANCOUNT >0 ROLLBACK TRANSACTION GOTO finish END

			UPDATE Future SET actionDate = GETDATE(), maturityDate = @dtMaturityDate WHERE futureID=@iFutureID
			IF @@ERROR <> 0 BEGIN SET @error = -8 IF @@TRANCOUNT >0 ROLLBACK TRANSACTION GOTO finish END

            IF @iLotSizeID <> 0 
           		UPDATE FutureRoot  SET futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, fTickValue = @fTickSize * @iLotSizeID, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency WHERE futureRootID=@iFutureRootID
            ELSE
            	UPDATE FutureRoot  SET optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency WHERE futureRootID=@iFutureRootID
			IF @@ERROR <> 0 BEGIN SET @error = -9 IF @@TRANCOUNT >0 ROLLBACK TRANSACTION GOTO finish END
			
			UPDATE ContractPrice
			SET priceClose = @fPriceClose
			WHERE exchangeID IS NULL AND contractID = @iFutureID
			IF @@ERROR <> 0 BEGIN SET @error = -10 IF @@TRANCOUNT >0 ROLLBACK TRANSACTION GOTO finish END
		COMMIT TRANSACTION
	END
	ELSE
	BEGIN
		BEGIN TRANSACTION
			SELECT @iFutureID_bad = contractID
			FROM Contract
				WHERE symbol=@vcSymbol
				AND contractTypeID=4

			IF @iFutureID_bad IS NOT NULL
			BEGIN
				INSERT INTO #FutureContractsForDelete (contractID)  values (@iFutureID_bad)
				IF @@ERROR <> 0 BEGIN SET @error = -4 IF @@TRANCOUNT >0 ROLLBACK TRANSACTION GOTO finish END

				INSERT INTO #FutureContractsForDelete 
					SELECT futureoptionID FROM FutureOption WHERE futureID=@iFutureID_bad
				IF @@ERROR <> 0 BEGIN SET @error = -5 IF @@TRANCOUNT >0 ROLLBACK TRANSACTION GOTO finish END
				
				exec @error = usp_EodFuture_Del

				IF @error <> 0
				BEGIN 
					SET @error = -6
					IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION 
					GOTO finish 
				END				
			
			END
			
			SELECT
				@iUndPriceProfileID = priceProfileID
			FROM
				PriceProfileView
			WHERE
				defaultID = 1
				AND isOptionProfile = 0

			-- check underlying price profile
			IF NOT EXISTS(SELECT 1 FROM PriceProfile WHERE priceProfileID = @iUndPriceProfileID)
			BEGIN SET @error = -10 IF @@TRANCOUNT >0 ROLLBACK TRANSACTION GOTO finish END
		
			SELECT
				@iOptPriceProfileID = priceProfileID
			FROM
				PriceProfile
			WHERE
				defaultID = 1
				AND isOptionProfile = 1

			-- check options price profile
			IF NOT EXISTS(SELECT 1 FROM PriceProfile WHERE priceProfileID = @iOptPriceProfileID)
			BEGIN SET @error = -11 IF @@TRANCOUNT >0 ROLLBACK TRANSACTION GOTO finish END
				

			INSERT INTO Contract
				(contractTypeID, symbol, contractName, expCalENDarID, undPriceProfileID, optPriceProfileID)
			values 
				(4, @vcSymbol, @vcContractName, 1, @iUndPriceProfileID, @iOptPriceProfileID)

			SET @iFutureID = @@identity
			IF @@ERROR <> 0 BEGIN SET @error = -12 IF @@TRANCOUNT >0 ROLLBACK TRANSACTION GOTO finish END

			INSERT INTO Future (futureID, futureRootID, maturityDate)
				 values (@iFutureID, @iFutureRootID, @dtMaturityDate)
			IF @@ERROR <> 0 BEGIN SET @error = -13 IF @@TRANCOUNT >0 ROLLBACK TRANSACTION GOTO finish END

			/*IF @dtExpiryDate IS NOT NULL
			BEGIN
				SELECT @iExpCalENDarID = ecv.ExpCalENDarID FROM Future f
				INNER JOIN FutureRoot fr on fr.FutureRootID = f.FutureRootID
				INNER JOIN ExpiryCalENDarView ecv on ecv.expCalENDarName = fr.futureRootSymbol + '_Fut'
				WHERE f.FutureID = @iFutureID

				IF @iExpCalENDarID IS NOT NULL
					BEGIN
						exec usp_Expiry_Save @iExpiryID, null, @dtExpiryDate, null, @iExpCalENDarID
						UPDATE Contract SET expCalENDarID = @iExpCalENDarID WHERE ContractID = @iFutureID
					END
			END
	*/
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			IF NOT EXISTS(SELECT * FROM ContractPrice WHERE exchangeID IS NULL AND contractID = @iFutureID)
				BEGIN
					INSERT INTO ContractPrice(contractID, exchangeID, priceClose)
						values(@iFutureID, NULL, @fPriceClose)
					IF (@@ERROR <> 0) BEGIN SET @error = -14 IF @@TRANCOUNT >0 ROLLBACK TRANSACTION GOTO finish END
				END 

                IF @iLotSizeID <> 0
        			UPDATE FutureRoot  SET futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, fTickValue = @fTickSize * @iLotSizeID, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency WHERE futureRootID=@iFutureRootID
                ELSE
                	UPDATE FutureRoot  SET optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency WHERE futureRootID=@iFutureRootID
			IF @@ERROR <> 0 BEGIN SET @error = -15 IF @@TRANCOUNT >0 ROLLBACK TRANSACTION GOTO finish END
			
			SET @tiIsActive = 1

		COMMIT TRANSACTION
	END 
	 
   	SET @error = @iFutureID
finish:
	RETURN(@error)
GO

ALTER   PROC dbo.usp_EodFuture_Del
AS
	set nocount on
	
	declare @error int		set @error = 0

	if object_ID('tempdb..#FutureContractsForDelete') is null
	begin
		set @error = -1
		goto finish
	end

	begin tran
		
	-- delete future option
	delete FutureOption
	where futureoptionID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -2 if @@trancount > 0 rollback tran goto finish end
	
	-- delete future
	delete Future where
	futureID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -3 if @@trancount > 0 rollback tran goto finish end


	-- delete common future and future options
	
	delete ContractPrice
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -4 if @@trancount >0 rollback tran goto finish end
	
	delete ContractExchangeLink
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -5 if @@trancount >0 rollback tran goto finish end

	delete  VolaSurfaceData
	where contractID in (select contractID from #FutureContractsForDelete)

	delete ContractInGroup 
	where contractID in (select contractID from #FutureContractsForDelete)
	
	delete Contract
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -6 if @@trancount >0 rollback tran goto finish end

	commit tran

finish:
	return @error

GO

GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.149')
	begin
		update DataInfo set keyValue = '4.49.149' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.149' '4.49.149 update message:' 
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
-- unable to execute this script in transaction