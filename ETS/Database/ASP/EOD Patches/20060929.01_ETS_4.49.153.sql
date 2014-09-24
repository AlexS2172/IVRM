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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.152' or (keyValue = '4.49.153')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.153 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.152' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
-------------------------------------------------------------------------------------------------------------------------------------------------------
/*	UPDATE	[@@trancount = 1] to [@@trancount > 0]	*/
-------------------------------------------------------------------------------------------------------------------------------------------------------
/*
	SEARCH_IN_FILE
	PROC usp_EodFuture_Import
*/
-------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER       PROC usp_EodFuture_Import
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
                              AND datediff(m, maturityDate, @dtMaturityDate) = 0

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
					IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION 
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
					IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION 
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
-------------------------------------------------------------------------------------------------------------------------------------------------------
/*
	SEARCH_IN_FILE
	PROC usp_EodFutureOption_Import
*/
-------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER     PROC usp_EodFutureOption_Import
	@iFutureID INT,
	@fStrike FLOAT,
	@dtExpiryDate DATETIME,
	@tiIsCall TINYINT,
	@vcSymbol VARCHAR(20),
	@vcContractName VARCHAR(255),
	@vcBadFutureOptionName VARCHAR(255) OUTPUT,
	@fPriceClose FLOAT = NULL
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
			WHERE symbol=@vcSymbol
				--AND contractID<>@iFutureOptionID
				AND contractTypeID=5

			IF @iFutureOptionID_bad IS NOT NULL AND @iFutureOptionID_bad != @iFutureOptionID
			BEGIN
				INSERT INTO #FutureContractsForDelete (contractID)  VALUES (@iFutureOptionID_bad)
				IF @@ERROR <> 0 BEGIN SET @error = -4 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END
				
				EXEC @error = usp_EodFutureOption_Del

				IF @error <> 0
				BEGIN 
					SET @error = -5
					IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION 
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
			end			
*/
			UPDATE Contract  SET symbol = @vcSymbol, contractName = @vcContractName WHERE contractID=@iFutureOptionID
			IF @@ERROR <> 0 BEGIN SET @error = -6 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END

			UPDATE FutureOption SET actionDate = GETDATE() WHERE futureOptionID=@iFutureOptionID
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
				 VALUES (5, @vcSymbol,@vcContractName)
			SET @iFutureOptionID = @@IDENTITY
			IF @@ERROR <> 0 BEGIN SET @error = -8 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END

			INSERT INTO FutureOption (futureOptionID, futureID, isCall, expiryDate, strike)
				 VALUES (@iFutureOptionID, @iFutureID, @tiIsCall, @dtExpiryDate, @fStrike)
			IF @@ERROR <> 0 BEGIN SET @error = -9 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END

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
--------------------------------------------------------------------------------------------------------------------------------------------------
/*
	SEARCH_IN_FILE
	PROC usp_EodOptClosePrice_Complete_Save
*/
--------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER       procedure dbo.usp_EodOptClosePrice_Complete_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000),
			@vcDate varchar(8000)

	set @vcTableName = '##ETSEODOP' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	set @vcDate = '''' + convert(varchar(12),getdate(),112) + '''' 

	if object_id(@vcFullTableName) is null
		begin
	    	Raiserror ( 'Cannot complete EOD. Cannot find save data.', 16, 1)
	       	Return (-1)
	   	end

	Begin Transaction
		set @vcExecString1 ='update ContractPrice set priceClose=EOD.priceClose,actionDate=' + @vcDate
		set @vcExecString2 =' from ContractPrice CP join ' + @vcTableName + ' EOD on CP.contractID = EOD.contractID and isnull(CP.exchangeID, 0) = 0'
		exec (@vcExecString1 + @vcExecString2)
			if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end					 

		set @vcExecString1 ='insert into ContractPrice (contractID, priceClose, actionDate) select contractID, priceClose,' + @vcDate
		set @vcExecString2 ='from ' + @vcTableName + ' EOD where not exists(select 1 from ContractPrice C where C.contractID = EOD.contractID and isnull(C.exchangeID, 0) = 0)'
			exec (@vcExecString1 + @vcExecString2)
				if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end					 
	Commit Transaction

    set @error = 0
finish:
	if object_id(@vcFullTableName) is not null
		exec ('drop table ' +  @vcTableName) 

	Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--------------------------------------------------------------------------------------------------------------------------------------------------
/*
	SEARCH_IN_FILE
	PROC usp_EODOption_Bulk_Import
*/
----------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EODOption_Bulk_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EODOption_Bulk_Import]
GO



CREATE PROCEDURE dbo.usp_EODOption_Bulk_Import
	@iUnderlyingContractID int,
	@iBulkRootsData ntext,
	@iBulkOptionData ntext
as

	DECLARE @vcSymbol varchar(20)
	DECLARE @vcContractName varchar(50)
	DECLARE @iIsCall  tinyint
	DECLARE @fStrike  float
	DECLARE @dtExpiry datetime
	DECLARE @iLotSize int
	DECLARE @iDocOption  int
	DECLARE @iDocRoots   int
	DECLARE @fPriceClose float
	DECLARE	@vcUnderlyingSymbol varchar(8)
	DECLARE @isError  int
	DECLARE @ERR_MSG  varchar(8000)
   	
	CREATE TABLE #Roots(RootName varchar(5) PRIMARY KEY, RootID INT)

	select @vcUnderlyingSymbol = symbol from ContractView where contractID = @iUnderlyingContractID
	set @isError = 0
	exec sp_xml_preparedocument @iDocRoots OUTPUT, @iBulkRootsData
	DECLARE BulkDataRoots CURSOR FOR
		SELECT *
			FROM OPENXML(@iDocRoots, '/Roots/OptRoot')
			WITH (Symbol	  varchar(20),
			      LotSize     int)    	
	OPEN BulkDataRoots
	FETCH NEXT FROM BulkDataRoots 
	INTO  @vcSymbol, @iLotSize
	WHILE @@FETCH_STATUS = 0
	BEGIN
		DECLARE @RootID as int
		EXEC @RootID = usp_EodOptionRoot_Import @iUnderlyingContractID, @vcSymbol, @iLotSize
		IF @RootID >0
		BEGIN
			INSERT INTO #Roots (RootName, RootID) values (@vcSymbol, @RootID)
		END

		FETCH NEXT FROM BulkDataRoots 
		INTO  @vcSymbol, @iLotSize
	END

	CLOSE      BulkDataRoots
	DEALLOCATE BulkDataRoots

	exec sp_xml_preparedocument @iDocOption OUTPUT, @iBulkOptionData
	DECLARE BulkDataCursor CURSOR FOR
		SELECT * 
			FROM OPENXML(@iDocOption, '/Import/Option')
 				WITH (Symbol	   varchar(20),
				      IsCall       tinyint ,    	
				      Strike       float ,    	
               			      Expiry	   datetime ,
				      ContractName varchar(50),
				      ClosePrice   float )

	
	OPEN BulkDataCursor
	FETCH NEXT FROM BulkDataCursor 
	INTO  @vcSymbol, @iIsCall, @fStrike, @dtExpiry, @vcContractName,@fPriceClose

	WHILE @@FETCH_STATUS = 0
	BEGIN
		  DECLARE @vcOptionRootSymbol varchar(5)	
		  DECLARE @iRootID            int
		  --DECLARE @iExpiryID          int
  		  declare @iRet 	      int
		  declare @iOptionID_bad      int
		  DECLARE @bSaveClosePrice    bit	

		  SET     @vcOptionRootSymbol = left(@vcSymbol,len(@vcSymbol)-2)
		  SELECT @iRootID = RootID from #Roots WHERE RootName = @vcOptionRootSymbol
		  IF @iRootID is not null
		  begin
			if @fPriceClose is NULL
				set @bSaveClosePrice = 0
			else 
				set @bSaveClosePrice = 1

			--set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

declare @error int		set @error = 0

	/*if not exists(select 1 from Expiry where expiryDate = isnull(@dtExpiry, 0))
	begin
		raiserror ('Wrong expiry date.', 16, 1)
		set @error = -100
		goto finish
	end*/

	declare	@OptionID int

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	select @OptionID = optionID
	from [Option] with(nolock, index = IX_Option_optionRootID_isCall_strike_expiryID_actionID)
	where optionRootID = @iRootID
		and isCall = @iIsCall
		and abs(strike - @fStrike) < 0.009
		and DATEDIFF(d, expiryDate, @dtExpiry) = 0
		and actionID < 3

	if @OptionID is not null
	begin
		Begin Transaction

			select @iOptionID_bad = contractID
			from Contract with(nolock, index = IX_Contract_symbol_contractTypeID_actionID)
			where symbol = @vcSymbol
				--and contractID <> @OptionID
				and contractTypeID = 3
				and actionID < 3

			if @iOptionID_bad is not null and @iOptionID_bad != @OptionID
			begin
				exec @error = usp_EodOption_Del @iOptionID = @iOptionID_bad			
	
				if @@error != 0 or @error != 0
				begin 
					if @@TranCount > 0 Rollback Transaction 
					goto finish 
				end
			end
				
			update Contract 
			set symbol = @vcSymbol, contractName = @vcContractName
			where contractID=@OptionID and actionID < 3
			if @@error <> 0 begin set @error = -5 if @@TranCount > 0 Rollback Transaction goto finish end

			update [Option]  
			set isManualyEntered = 0 
			where optionID=@OptionID and actionID < 3
			if @@error <> 0 begin set @error = -6 if @@TranCount > 0 Rollback Transaction goto finish end

			if @bSaveClosePrice is not null and  @bSaveClosePrice = 1
                        begin 	
				update ContractPrice
					set priceClose = @fPriceClose
					where contractID = @OptionID and exchangeID is null
				if @@error <> 0 begin set @error = -6 if @@TranCount > 0 Rollback Transaction goto finish end
                        end
		Commit Transaction
	end
	else
	begin
		Begin Transaction

			select @iOptionID_bad = contractID
			from Contract with(nolock, index = IX_Contract_symbol_contractTypeID_actionID)
			where symbol = @vcSymbol
				and contractTypeID = 3
				and actionID < 3

			if @iOptionID_bad is not null
			begin
				exec @error = usp_EodOption_Del @iOptionID = @iOptionID_bad			
	
				if @@error != 0 or @error != 0
				begin 
					if @@TranCount > 0 Rollback Transaction 
					goto finish 
				end
			end

			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@vcContractName)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = -5 if @@TranCount > 0 Rollback Transaction goto finish end

			insert into [Option] (optionID, optionRootID, isCall, strike, expiryDate)
				 values (@OptionID, @iRootID, @iIsCall, @fStrike, @dtExpiry)
			if @@error <> 0 begin set @error = -6 if @@TranCount > 0 Rollback Transaction goto finish end
	
			if not exists(select 1 from ContractPrice where exchangeID is null and contractID = @OptionID)
			begin
	  		    if @bSaveClosePrice is not null and  @bSaveClosePrice = 1
				insert into ContractPrice(contractID, exchangeID, priceClose)
					values(@OptionID, null, @fPriceClose)
                            else
				insert into ContractPrice(contractID, exchangeID, priceClose)
					values(@OptionID, null, null)

			    if (@@error <> 0) begin set @error = -7 if @@TranCount > 0 Rollback Transaction goto finish end
			end 
		
		Commit Transaction
	end 
	 

		  end	


		FETCH NEXT FROM BulkDataCursor 
		INTO @vcSymbol, @iIsCall, @fStrike, @dtExpiry, @vcContractName, @fPriceClose
	END

finish:
	CLOSE      BulkDataCursor
	DEALLOCATE BulkDataCursor

	exec sp_xml_removedocument @iDocOption
	exec sp_xml_removedocument @iDocRoots
	return @error
	


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--------------------------------------------------------------------------------------------------------------------------------------------------
/*
	SEARCH_IN_FILE
	PROC usp_EodOptTheoPrice_Complete_Save
*/
--------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER       procedure dbo.usp_EodOptTheoPrice_Complete_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000),
			@vcDate varchar(8000)

	set @vcTableName = '##ETSEODTP' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	set @vcDate = '''' + convert(varchar(12),getdate(),112) + '''' 

	if object_id(@vcFullTableName) is null
		begin
	    	Raiserror ( 'Cannot complete EOD. Cannot find save data.', 16, 1)
	       	Return (-1)
	   	end

	Begin Transaction
		set @vcExecString1 ='update ContractPrice set priceTheo=EOD.priceTheo,actionDate=' + @vcDate
		set @vcExecString2 =' from ContractPrice CP join ' + @vcTableName + ' EOD on CP.contractID = EOD.contractID and isnull(CP.exchangeID, 0) = 0'
		exec (@vcExecString1 + @vcExecString2)
			if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end					 

		set @vcExecString1 ='insert into ContractPrice (contractID, priceTheo, actionDate) select contractID, priceTheo,' + @vcDate
		set @vcExecString2 ='from ' + @vcTableName + ' EOD where not exists(select 1 from ContractPrice C where C.contractID = EOD.contractID and isnull(C.exchangeID, 0) = 0)'
			exec (@vcExecString1 + @vcExecString2)
				if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end					 
	Commit Transaction

    set @error = 0
finish:
	if object_id(@vcFullTableName) is not null
		exec ('drop table ' +  @vcTableName) 

	Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--------------------------------------------------------------------------------------------------------------------------------------------------
/*
	SEARCH_IN_FILE
	PROC usp_EodUndClosePrice_Complete_Save
*/
--------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER      procedure dbo.usp_EodUndClosePrice_Complete_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000),
			@vcDate varchar(8000)

	set @vcTableName = '##ETSEODUP' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	set @vcDate = '''' + convert(varchar(12),getdate(),112) + '''' 

	if object_id(@vcFullTableName) is null
		begin
	    	Raiserror ( 'Cannot complete EOD. Cannot find save data.', 16, 1)
	       	Return (-1)
	   	end

	Begin Transaction
		set @vcExecString1 ='update ContractPrice set priceClose=EOD.priceClose,actionDate=' + @vcDate
		set @vcExecString2 =' from ContractPrice CP join ' + @vcTableName + ' EOD on CP.contractID = EOD.contractID and isnull(CP.exchangeID, 0) = 0'
		exec (@vcExecString1 + @vcExecString2)
			if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end					 

		set @vcExecString1 ='insert into ContractPrice (contractID, priceClose, actionDate) select contractID, priceClose,' + @vcDate
		set @vcExecString2 ='from ' + @vcTableName + ' EOD where not exists(select 1 from ContractPrice C where C.contractID = EOD.contractID and isnull(C.exchangeID, 0) = 0)'
			exec (@vcExecString1 + @vcExecString2)
				if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end					 
	Commit Transaction

    set @error = 0
finish:
	if object_id(@vcFullTableName) is not null
		exec ('drop table ' +  @vcTableName) 

	Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--------------------------------------------------------------------------------------------------------------------------------------------------
/*
	SEARCH_IN_FILE
	PROC usp_EodUnderlying_Import
*/
--------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




ALTER    PROCEDURE dbo.usp_EodUnderlying_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@fPriceClose float = null,
        @bSaveClosePrice bit = null,
	@vcExchangeCode varchar(20) = null

as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction

	update Contract
	    set ContractName = @vcContractName,
			MSupdateDate = getDate()
 	where contractID = @iContractID

	if @@error <> 0 
		begin 
			Raiserror ('Error. Can''t update Contract Name!', 16, 1)
			set @error = -1 
			if @@TranCount > 0 Rollback Transaction goto finish 
		end

	delete from TradeLog
	where underlyingID = @iContractID
	if @@error <> 0 
		begin 
			Raiserror ('Error. Can''t delete record from TradeLog!', 16, 1)
			set @error = -1 
			if @@TranCount > 0 Rollback Transaction goto finish 
		end

	--========================================================================
	-- make record with null exchange for this stock
	--========================================================================
	if exists(select 1 from ContractPrice where exchangeID is null and contractID = @iContractID)
	begin
	      if @bSaveClosePrice is not null and  @bSaveClosePrice = 1
              begin		
		update ContractPrice
		set priceClose = @fPriceClose
		where exchangeID is null and contractID = @iContractID
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update close price for default exchange!', 16, 1)
				set @error = -1 
				if @@TranCount > 0 Rollback Transaction goto finish
			end
              end 
	end
	else
	begin
	      if @bSaveClosePrice is not null and  @bSaveClosePrice = 1
		insert into ContractPrice(contractID, exchangeID, priceClose)
			values(@iContractID, null, @fPriceClose)
	      else
		insert into ContractPrice(contractID, exchangeID, priceClose)
			values(@iContractID, null, null)
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t insert default exchange!', 16, 1)
				set @error = -1 
				if @@TranCount > 0 Rollback Transaction goto finish
			end
	end
	--==================================================================================
	-- update DivAmnt & DivFreq values
	--==================================================================================
	if (@bIsStock <> 0 and exists(select 1 from Stock where stockID=@iContractID))
		begin 
			declare @exchangeID as int
			set @exchangeID = null
			if (@vcExchangeCode is not null)
			begin
				set @exchangeID = (select TOP 1 e.exchangeID from Exchange e where @vcExchangeCode = exchangeCode)
			end

			Declare @df smallint
			update Stock 
				set divAmt = @fDivAmt,
					divDate = @dtDivDate, 
					divFreq = @iDivFreq,
					primaryExchangeID = isNull(@exchangeID,primaryExchangeID)
			where stockID = @iContractID
			if (@@error <> 0)
				begin
					Raiserror ('Error. Can''t update Dividends and Primary Exchange!', 16, 1)
					set @error = -1 
					if @@TranCount > 0 Rollback Transaction goto finish 
				end
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
--------------------------------------------------------------------------------------------------------------------------------------------------
/*
	SEARCH_IN_FILE
	PROC usp_EodUnderlyingAverageVolumes_Save
*/
-------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER   PROCEDURE dbo.usp_EodUnderlyingAverageVolumes_Save 
	@iUndID int, 
	@iVolume10Day int = null,
	@iVolume5Expiration int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction
	if not exists(select 1 from ContractPrice where contractID=@iUndID and isnull(exchangeID,0)=0)
		begin
			insert into ContractPrice (contractID,
					exchangeID, 
					volume10Day,
					volume5Expiration)
			  values (@iUndID,
					null,
					@iVolume10Day,
					@iVolume5Expiration)

			if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end
		end
	else
		begin

			update ContractPrice set 
				volume10Day = isnull(@iVolume10Day, volume10Day),
				volume5Expiration = isnull(@iVolume5Expiration, volume5Expiration)
			where contractID = @iUndID and isnull(exchangeID,0)=0

   			if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
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
--------------------------------------------------------------------------------------------------------------------------------------------------
/*
	SEARCH_IN_FILE
	PROC usp_EodUnderlyingIndexBeta_Save
*/
-------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER      Procedure dbo.usp_EodUnderlyingIndexBeta_Save 
	@iUnderlyingID int,
	@iIndexID int,
	@fBeta float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	Begin Transaction
		if not exists(select 1 From IndexBeta where indexID = @iIndexID and contractID = @iUnderlyingID) 
			begin
				insert into IndexBeta (indexID,contractID,beta) 
				values (@iIndexID,@iUnderlyingID,@fBeta)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end
			end
		else
			begin
				update IndexBeta
					set beta = isnull(@fBeta, beta)
					where indexID = @iIndexID and contractID = @iUnderlyingID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
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
--------------------------------------------------------------------------------------------------------------------------------------------------
/*
	SEARCH_IN_FILE
	PROC usp_EodUnderlyingSkewKurt_Save
*/
--------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER   PROCEDURE dbo.usp_EodUnderlyingSkewKurt_Save 
	@iUndID int,
	@fSkew float = null,
	@fKurt float = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iContractTypeID int

	select @iContractTypeID = ContractTypeID from Contract
		where contractTypeID < 3 and contractID=@iUndID

	Begin Transaction
	if @iContractTypeID = 1
		begin
			update [Index]
				set skew = isnull(@fSkew, skew),
					kurt = isnull(@fKurt, kurt)
			where indexID = @iUndID
			if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end
		end
	else
		begin
			update Stock set 
				skew = isnull(@fSkew, skew),
				kurt = isnull(@fKurt, kurt)
			where stockID = @iUndID
   			if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
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
--------------------------------------------------------------------------------------------------------------------------------------------------
/*
	SEARCH_IN_FILE
	PROC usp_EodVolaBands_Complete_Save
*/
--------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER    proc dbo.usp_EodVolaBands_Complete_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000)

	set @vcTableName = '##ETSEODSRVB' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	if object_id(@vcFullTableName) is null 
		begin
		   raiserror ('Cannot complete EOD. Cannot find save data of IV bands.', 16, 1)
		   return (-1)
		end

	Begin tran 
		set @vcExecString1 = 'delete VolaBands where contractID in (select distinct contractID from ' + @vcTableName + ')'
		set @vcExecString2 = ' insert into VolaBands (contractID,expPeriod,strikeMoneyness,IVBandLow,IVBandHi)' + 
				'select contractID,ExpPeriod,StrikeMoneyness,IVBandLow,IVBandHi from ' + @vcTableName +
				' truncate table ' + @vcTableName
		
		exec (@vcExecString1 + @vcExecString2)
 		if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
	Commit tran

    set @error = 0

finish:
	if object_id(@vcFullTableName) is not null
		exec ('drop table ' +  @vcTableName) 

	Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--------------------------------------------------------------------------------------------------------------------------------------------------
/*
	SEARCH_IN_FILE
	PROC usp_EodDefaultIvPoint_Save
*/
--------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER     Procedure dbo.usp_EodDefaultIvPoint_Save 
	@iContractID int,
	@fStrike float,
	@dtExpiryDate int,
	@fVolatility float
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iCustomStrikeSkewPointID int 
		set @iCustomStrikeSkewPointID = Null

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int 
		set @iVolaSurfaceDataID = 0

	declare @iType int
		set @iType = 2 --OTM_IVTYPE

	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID
	
	-- mark olf strile skew points as deleted
	--update CustomStrikeSkewPoint	set actionID = 3
	delete from CustomStrikeSkewPoint
	where 
		volaSurfaceDataID = @iVolaSurfaceDataID and
		actionID < 3 and
		datediff(d, actionDate, getDate())>0


	select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
		from CustomStrikeSkewPoint 
		where volaSurfaceDataID = @iVolaSurfaceDataID 
			and abs(strike - @fStrike) < 0.009
			and type = @iType 
			and DATEDIFF(d, expiryDate, @dtExpiryDate) = 0 
			and actionID < 3
  
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					isBasePoint,
					expiryDate)
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						-1,
						@dtExpiryDate)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end

			end
		else
			begin
				update CustomStrikeSkewPoint set 
					volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					type = isnull(@iType,type),
					strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility),
					expiryDate = isnull(@dtExpiryDate,@dtExpiryDate)
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
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
--------------------------------------------------------------------------------------------------------------------------------------------------
/*
	SEARCH_IN_FILE
	PROC usp_EodFutureOption_Del
*/
--------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER  PROC dbo.usp_EodFutureOption_Del
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
	where futureOptionID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -1 if @@trancount > 0 rollback tran goto finish end
	
	-- delete common
	delete ContractPrice
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -2 if @@trancount > 0 rollback tran goto finish end
	
	delete Contract
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -3 if @@trancount > 0 rollback tran goto finish end

	commit tran

finish:
	return @error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--------------------------------------------------------------------------------------------------------------------------------------------------
/*
	SEARCH_IN_FILE
	PROC usp_EodFutureOption_Del
*/
--------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER  PROC dbo.usp_EodFutureOption_Del
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
	where futureOptionID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -1 if @@trancount > 0 rollback tran goto finish end
	
	-- delete common
	delete ContractPrice
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -2 if @@trancount > 0 rollback tran goto finish end
	
	delete Contract
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -3 if @@trancount > 0 rollback tran goto finish end

	commit tran

finish:
	return @error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--------------------------------------------------------------------------------------------------------------------------------------------------------
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.153')
	begin
		update DataInfo set keyValue = '4.49.153' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.153' '4.49.153 update message:' 
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

