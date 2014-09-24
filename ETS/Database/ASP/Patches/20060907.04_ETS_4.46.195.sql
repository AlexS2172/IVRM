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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.194'  or (keyValue = '3.46.195') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.195 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.195' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
ALTER     PROC usp_EodFuture_Import
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
                              AND datediff(d, maturityDate, @dtMaturityDate) = 0

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
				IF @@ERROR <> 0 BEGIN SET @error = -4 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

				INSERT INTO #FutureContractsForDelete 
					SELECT futureoptionID FROM FutureOption WHERE futureID=@iFutureID_bad
				IF @@ERROR <> 0 BEGIN SET @error = -5 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
				
				EXEC @error = usp_EodFuture_Del

				IF @error <> 0
				BEGIN 
					SET @error = -6
					IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION 
					GOTO finish 
				END				
			END



			UPDATE Contract  SET symbol = @vcSymbol, contractName = @vcContractName WHERE contractID=@iFutureID
			IF @@ERROR <> 0 BEGIN SET @error = -7 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			UPDATE Future SET actionDate = GETDATE() WHERE futureID=@iFutureID
			IF @@ERROR <> 0 BEGIN SET @error = -8 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

            IF @iLotSizeID <> 0 
           		UPDATE FutureRoot  SET futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, fTickValue = @fTickSize * @iLotSizeID, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency WHERE futureRootID=@iFutureRootID
            ELSE
            	UPDATE FutureRoot  SET optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency WHERE futureRootID=@iFutureRootID
			IF @@ERROR <> 0 BEGIN SET @error = -9 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
			
			UPDATE ContractPrice
			SET priceClose = @fPriceClose
			WHERE exchangeID IS NULL AND contractID = @iFutureID
			IF @@ERROR <> 0 BEGIN SET @error = -10 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
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
				IF @@ERROR <> 0 BEGIN SET @error = -4 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

				INSERT INTO #FutureContractsForDelete 
					SELECT futureoptionID FROM FutureOption WHERE futureID=@iFutureID_bad
				IF @@ERROR <> 0 BEGIN SET @error = -5 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
				
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
			BEGIN SET @error = -10 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
		
			SELECT
				@iOptPriceProfileID = priceProfileID
			FROM
				PriceProfile
			WHERE
				defaultID = 1
				AND isOptionProfile = 1

			-- check options price profile
			IF NOT EXISTS(SELECT 1 FROM PriceProfile WHERE priceProfileID = @iOptPriceProfileID)
			BEGIN SET @error = -11 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
				

			INSERT INTO Contract
				(contractTypeID, symbol, contractName, expCalENDarID, undPriceProfileID, optPriceProfileID)
			values 
				(4, @vcSymbol, @vcContractName, 1, @iUndPriceProfileID, @iOptPriceProfileID)

			SET @iFutureID = @@identity
			IF @@ERROR <> 0 BEGIN SET @error = -12 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			INSERT INTO Future (futureID, futureRootID, maturityDate)
				 values (@iFutureID, @iFutureRootID, @dtMaturityDate)
			IF @@ERROR <> 0 BEGIN SET @error = -13 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

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
					IF (@@ERROR <> 0) BEGIN SET @error = -14 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
				END 

                IF @iLotSizeID <> 0
        			UPDATE FutureRoot  SET futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, fTickValue = @fTickSize * @iLotSizeID, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency WHERE futureRootID=@iFutureRootID
                ELSE
                	UPDATE FutureRoot  SET optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency WHERE futureRootID=@iFutureRootID
			IF @@ERROR <> 0 BEGIN SET @error = -15 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
			
			SET @tiIsActive = 1

		COMMIT TRANSACTION
	END 
	 
   	SET @error = @iFutureID
finish:
	RETURN(@error)


GO

GO

ALTER   PROCEDURE usp_BoFuture_Import
	@iFutureID int,
	@iFutureRootID int,
	@dtMaturityDate datetime,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iLotSizeID int,
	@iOptionLotSizeID int,
        @fPriceClose float,
	@vcBadFutureName varchar(255) output,
        @fTickSize float = null,
        @iQuotationUnitID int = null,
        @vcQuotationUnitCurrency varchar(3) = null,
	@dtExpiryDate as datetime = null	
as

	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	declare @tiIsActive tinyint
	
	if @iFutureRootID is null
		select @iFutureRootID = futureRootID
			from Future (nolock)
			where futureID = @iFutureID
			
	exec @error = usp_EodFuture_Import @iFutureRootID, @dtMaturityDate, @vcSymbol, @vcContractName,@iLotSizeID,@iOptionLotSizeID, @tiIsActive output, @vcBadFutureName output, @fPriceClose, @fTickSize, @iQuotationUnitID, @vcQuotationUnitCurrency 
	 
finish:
Return(@error)

GO

ALTER   PROC usp_EodFutureOption_Import
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
			end			
*/
			UPDATE Contract  SET symbol = @vcSymbol, contractName = @vcContractName WHERE contractID=@iFutureOptionID
			IF @@ERROR <> 0 BEGIN SET @error = -6 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			UPDATE FutureOption SET actionDate = GETDATE() WHERE futureOptionID=@iFutureOptionID
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

			INSERT INTO FutureOption (futureOptionID, futureID, isCall, expiryDate, strike)
				 VALUES (@iFutureOptionID, @iFutureID, @tiIsCall, @dtExpiryDate, @fStrike)
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

ALTER    PROC dbo.usp_BoTradeNew_Get
	@dtLastTradeDateUTC datetime = null -- return all trades were made earlier or at @dtLastTradeDateUTC
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	select
		tr.tradeID as iTradeID,
		tr.tradeDate as dtTradeDate,
		tr.quantity as iQuantity,
		tr.price as fPrice,
		tr.isBuy as tiIsBuy,

		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		c.exportSymbol as vcExportSymbol,
		--c.ImportID as ImportID,
	
		case when c.contractTypeID = 3 then o.expiryDate else fo.expiryDate end as dtExpiryDate,
		case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
		case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
	
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		--uc.ImportID as vcUnderlyingImportID,
		uc.exportSymbol as vcUnderlyingExportSymbol,
	
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,

		case when c.contractTypeID < 3 then 1
			when c.contractTypeID = 3 then ort.lotSize
			when c.contractTypeID = 4 then fr.futureLotSize
			when c.contractTypeID = 5 then fr.optionLotSize
		end as iLotSize,

		t.traderID as iTraderID,
		t.acronym as vcTraderName,

		br.brokerID as iBrokerID,
		br.brokerName as vcBrokerName,
		tr.brokerCommission as fBrokerCommission,

		tr.mark as iTradeMark,

		st.strategyID as iStrategyID,
		st.strategyName as vcStrategyName,

		trg.trgID as iTraderGroupID,
		trg.trgName as vcTraderGroupName,

		fr.futureRootID as iFutureRootID,
		fr.futureRootSymbol as vcFutureRootSymbol,
		fr.exportSymbol as vcFutureRootExportSymbol,
		f.futureID as iFutureID,
		fc.symbol as vcFutureSymbol,
		fc.exportSymbol as vcFutureExportSymbol,
		f.maturityDate as FutureMaturityDate, 
		fr.futureLotSize as iFutureLotSize,
		fr.optionLotSize as iFutureOptionLotSize
	from
		TradeView tr
		-- contract and underlying contract
		inner join ContractView c on tr.contractID = c.contractID
		inner join ContractView uc on tr.underlyingID = uc.contractID
		-- regular option
		left join OptionView o on c.contractID = o.optionID
		left join OptionRootView ort on tr.optionRootID = ort.optionRootID
		-- trade data
		left join TraderView t on tr.traderID = t.traderID
		left join StrategyView st on st.strategyID = tr.strategyID
		left join TraderGroup trg on t.traderGroupID = trg.trgID
		left join BrokerView br on tr.brokerID = br.brokerID
		-- futures
		left join ContractView fc on tr.futureID = fc.contractID
		left join FutureRoot fr on tr.futureRootID = fr.futureRootID
		left join Future f on tr.futureID = f.futureID
		left join FutureOption fo on tr.contractID = fo.futureOptionID
	where
		tr.status in (0, 3) -- only actual (or actual matched) trades should be reported
		and isnull(tr.isPosition, 0) = 0 -- only trades (not positions) should be reported
		and len(isnull(tr.execID, '')) = 0 -- only non-RTTF trades should be reported (make sure that all RTTF trades has execID)
		and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0



GO

ALTER   PROC dbo.usp_BoOptionByParams_Get
	@iUnderlyingID int, 
	@dtExpiry datetime,
	@fStrike float,
	@tiIsCall tinyint
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	select	C.contractID as iContractID,
			C.symbol as vcSymbol,
			C.exportSymbol as vcExportSymbol, 
			ORt.optionRootID as iOptionRootID,
			ORt.symbol as vcOptionRootSymbol
			from ContractView C  
				inner join OptionView O on O.optionID = C.contractID
				join OptionRoot ORt on O.optionRootID = ORt.optionRootID
			where c.contractTypeID=3
				and datediff(d,O.expiryDate, @dtExpiry) = 0
				and ORt.underlyingID = @iUnderlyingID
				and abs(@fStrike - O.strike) < 0.001
				and O.isCall = @tiIsCall
			order by ORt.symbol

	return @@error

GO

ALTER  PROC dbo.usp_BoTradeNewContract_Get
	@dtLastTradeDateUTC datetime = null -- return all contracts for trades were made earlier or at @dtLastTradeDateUTC
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		c.exportSymbol as vcExportSymbol,
	
		case when c.contractTypeID = 3 then o.expiryDate else fo.expiryDate end as dtExpiry,
		case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
		case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
	
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		uc.exportSymbol as vcUnderlyingExportSymbol,
	
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,

		fr.futureRootID as iFutureRootID,
		fr.futureRootSymbol as vcFutureRootSymbol,
		fr.exportSymbol as vcFutureRootExportSymbol,
		f.futureID as iFutureID,
		fc.symbol as vcFutureSymbol,
		fc.exportSymbol as vcFutureExportSymbol,
		f.maturityDate as dtFutureMaturity
	from
		TradeView tr
		-- contract and underlying contract
		inner join ContractView c on tr.contractID = c.contractID
		inner join ContractView uc on tr.underlyingID = uc.contractID
		-- regular option
		left join OptionView o on c.contractID = o.optionID
		left join OptionRootView ort on tr.optionRootID = ort.optionRootID
		-- futures
		left join ContractView fc on tr.futureID = fc.contractID
		left join FutureRoot fr on tr.futureRootID = fr.futureRootID
		left join Future f on tr.futureID = f.futureID
		left join FutureOption fo on tr.contractID = fo.futureOptionID
	where
		tr.status in (0, 3) -- only actual (or actual matched) trades should be reported
		and isnull(tr.isPosition, 0) = 0 -- only trades (not positions) should be reported
		and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
		and c.contractID not in (select contractID from TradeView 
									where isnull(isPosition, 0) = 1)
	group by
		c.contractID, c.contractTypeID, c.symbol, c.exportSymbol, o.expiryDate,
		o.strike, o.isCall, fo.expiryDate, fo.strike, fo.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
		uc.exportSymbol, ort.optionRootID, ort.symbol,
		fr.futureRootID, fr.futureRootSymbol, fr.exportSymbol, f.futureID,
		fc.symbol, fc.exportSymbol, f.maturityDate

GO

ALTER    VIEW CustomStrikeSkewPointView

AS

            SELECT 
                        customStrikeSkewPointID,
                        volaSurfaceDataID,
                        strike,
                        type,
                        volatility,
                        expiryDate,
                        isBasePoint,
                        actionID,
                        actionDate
            FROM
                        CustomStrikeSkewPoint
            WHERE
                        actionID < 3 


GO

GO

ALTER      Procedure dbo.usp_CustomStrikeSkewData_Get
	@iVolaSurfaceDataID int,
	@dtExpDate datetime = null,
	@iSetupMode int,
	@iCustomStrikeSkewID int = null,
	@iCustomStrikeSkewPointID int = null,
	@bType bit = 1
as
/*
	this procedure gets data from CustomStrikeSkewData table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	

	if @bType is null	set @bType = 1

	if @dtExpDate is not null
		begin
			if not exists(select * 
				from CustomStrikeSkewPointView C
					join VolaSurfaceData  on C.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
					where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and 
					datediff(d, C.expiryDate, @dtExpDate)=0 and type = @bType)

			select distinct CustomStrikeSkewPoint.customStrikeSkewPointID iCustomStrikeSkewPointID, 
				CustomStrikeSkewPoint.volaSurfaceDataID iVolaSurfaceDataID, 
				setupMode iSetupMode, 
				strike fStrike, 
				case when datediff(d,expiryDate,@dtExpDate)=0 then isnull(volatility, 0) else 0 end fVolatility, 
				case when datediff(d,expiryDate,@dtExpDate)=0 then CustomStrikeSkew.customStrikeSkewID else null end iCustomStrikeSkewID, 
				case when datediff(d,expiryDate,@dtExpDate)=0 then E.expiryDate else null end dtExpDate,
				type bType
		
				from CustomStrikeSkewPointView
						join VolaSurfaceData on CustomStrikeSkewPoint.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
						join Contract C on  VolaSurfaceData.contractID = C.contractID
						join ExpiryInCalendarView E on E.expCalendarID = C.expCalendarID
						left join CustomStrikeSkewData on CustomStrikeSkewPoint.customStrikeSkewPointID = CustomStrikeSkewData.customStrikeSkewPointID
						left join CustomStrikeSkew on CustomStrikeSkewData.customStrikeSkewID = CustomStrikeSkew.customStrikeSkewID
				where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and setupMode = @iSetupMode and type = @bType
					and (@dtExpDate is null or expiry = @dtExpDate)
					and (@iCustomStrikeSkewID is null or CustomStrikeSkew.customStrikeSkewID = @iCustomStrikeSkewID)
					and (@iCustomStrikeSkewPointID is null or CustomStrikeSkewPoint.customStrikeSkewPointID = @iCustomStrikeSkewPointID)

		end
	else
		select  CustomStrikeSkewPointView.customStrikeSkewPointID iCustomStrikeSkewPointID, 
				CustomStrikeSkewPointView.volaSurfaceDataID as ivolaSurfaceDataID, 
				0 iSetupMode, 
				strike fStrike, 
				volatility as fVolatility, 
				0 as iCustomStrikeSkewID, 
				E.expiryDate as dtExpDate,
				type as bType
			from CustomStrikeSkewPointView
				join VolaSurfaceData on CustomStrikeSkewPointView.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
				join Contract C on  VolaSurfaceData.contractID = C.contractID
				join ExpiryInCalendarView E on E.expCalendarID = C.expCalendarID
			where VolaSurfaceData.volaSurfaceDataID = @iVolaSurfaceDataID and type = @bType
					and (@iCustomStrikeSkewPointID is null or CustomStrikeSkewPointView.customStrikeSkewPointID = @iCustomStrikeSkewPointID)
	set @error = @@error
Return(@error)
GO
ALTER    procedure dbo.usp_CustomStrikeSkewDefaultPoint_Save
	@iContractID int,
	@dtExpiry datetime,
	@iType int,
	@fVolatility float
as
/*
	Created by Zaur Nuraliev
	Description: this procedure ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int set @iVolaSurfaceDataID = 0


	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

--select @dtExpiry as '@dtExpiry', @iVolaSurfaceDataID as '@iVolaSurfaceDataID'
	if @iVolaSurfaceDataID is not null
		begin
			insert into CustomStrikeSkewPoint(VolaSurfaceDataID,strike,type,volatility,isBasePoint,expiryDate)
				select distinct @iVolaSurfaceDataID, strike as fStrike, @iType,@fVolatility,-1,@dtExpiry --convert(varchar(6),expiry,112) + '01' as expiry
					from OptionView 
						join OptionRoot ort on OptionView.optionRootID = ort.optionRootID
					where OptionView.expiryDate = @dtExpiry and ort.underlyingID = @iContractID
						and not exists(select * 
											from CustomStrikeSkewPointView 
											where VolaSurfaceDataID = @iVolaSurfaceDataID and 
													strike = OptionView.strike 
													and CustomStrikeSkewPointView.expiryDate = OptionView.expiryDate)
					--and convert(varchar(8),expDate,112) = convert(varchar(6),expiry,112) + '01')
		end
GO
ALTER      Procedure usp_CustomStrikeSkewPoint_Copy 
	@iVolaSurfaceDataIDSrc int,
	@iVolaSurfaceDataIDDest int
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is move information inside CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		select 	volaSurfaceDataID,
				strike,
				type,
				volatility,
				expiryDate,
				isBasePoint
			into #SourcePoints
			from CustomStrikeSkewPoint
			where volaSurfaceDataID=@iVolaSurfaceDataIDSrc
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
		delete CustomStrikeSkewPoint where volaSurfaceDataID=@iVolaSurfaceDataIDDest
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
	
		insert into CustomStrikeSkewPoint (volaSurfaceDataID,strike,type,volatility,expiryID,isBasePoint)
		select 	volaSurfaceDataID,
				strike,
				type,
				volatility,
				expiryDate,
				isBasePoint
				from #SourcePoints
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
    set @error = 0
finish:
	drop table #SourcePoints
Return(@error)


GO


GO

ALTER    PROC dbo.usp_CustomStrikeSkewPoint_Get 
            @iCustomStrikeSkewPointID INT = NULL, 
            @iVolaSurfaceDataID INT = NULL,
            @iType INT = NULL,
            @dtExpDate DATETIME = NULL
AS
            ------------------------------------------------
            SET NOCOUNT ON
            ------------------------------------------------
            DECLARE @error INT               SET @error = -1
 
            -- Sharky: faster request for special parameters' set
            IF @dtExpDate IS NULL AND @iCustomStrikeSkewPointID IS NULL
                        AND @iVolaSurfaceDataID IS NOT NULL AND @iType IS NOT NULL
            BEGIN
                        SELECT 
                                   CSSP.customStrikeSkewPointID AS iCustomStrikeSkewPointID,
                                   CSSP.volaSurfaceDataID AS iVolaSurfaceDataID,
                                   CSSP.strike AS fStrike, 
                                   CSSP.type AS iType,
                                   CSSP.volatility AS fVolatility,
                                   CSSP.expiryDate AS dtExpDate,
                                   CSSP.isBasePoint AS iIsBasePoint
                        FROM 
                                   CustomStrikeSkewPoint CSSP WITH(INDEX = IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID, NOLOCK)
                        WHERE 
                                   CSSP.volaSurfaceDataID = @iVolaSurfaceDataID
                                   AND CSSP.type = @iType
                                   AND CSSP.actionID < 3
            END
            ELSE
            BEGIN

                        SELECT 
                                   CSSP.customStrikeSkewPointID AS iCustomStrikeSkewPointID,
                                   CSSP.volaSurfaceDataID AS iVolaSurfaceDataID,
                                   CSSP.strike AS fStrike, 
                                   CSSP.type AS iType,
                                    CSSP.volatility AS fVolatility,
                                    CSSP.expiryDate AS dtExpDate,
                                   CSSP.isBasePoint AS iIsBasePoint 
                        FROM CustomStrikeSkewPointView CSSP WITH(INDEX = IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID, NOLOCK)
                        WHERE 
                                   customStrikeSkewPointID = ISNULL(@iCustomStrikeSkewPointID, customStrikeSkewPointID) AND
                                   CSSP.volaSurfaceDataID = ISNULL(@iVolaSurfaceDataID, CSSP.volaSurfaceDataID) AND
                                   type = ISNULL(@iType, type) AND
                                   CSSP.expiryDate = ISNULL(@dtExpDate, CSSP.expiryDate)
            END

            SET @error = @@ERROR
            RETURN(@error)


GO

ALTER   Procedure dbo.usp_CustomStrikeSkewPoint_Save 
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
			type = @iType and expiryDate = @dtExpDate and actionID < 3 and 
			(@iCustomStrikeSkewPointID <> CustomStrikeSkewPointID or @iCustomStrikeSkewPointID is null)
	)
	begin
		if @iCustomStrikeSkewPointID is null
			select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
			from CustomStrikeSkewPoint 
			where volaSurfaceDataID = @iVolaSurfaceDataID and strike = @fStrike and 
				type = @iType and expiryDate = @dtExpDate and actionID < 3
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
		insert into CustomStrikeSkewPoint ( volaSurfaceDataID, type, strike, volatility, expiryDate, 
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
			expiryDate = isnull(@dtExpDate,expiryDate),
			isBasePoint = isnull(@iIsBasePoint,isBasePoint)
		where customStrikeSkewPointID=@iCustomStrikeSkewPointID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	end
	Commit Transaction

	set @error = 0
finish:

Return(@error)


GO

ALTER   PROCEDURE dbo.usp_CustomVolatilityPointIsExist_Get
	@vcSymbolName varchar(20) = null
as
/*
	Created by Zaur Nuraliev
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	if @vcSymbolName is null
		select distinct 
				CV.contractID as iContractID,
				CV.symbol as vcSymbolName,
				--EC.expiryDate as dtExpiry,
				OV.expiryDate as dtExpiry,
				case when CSSP.volaSurfaceDataID is null then 0  
				else 1 end 'CustomVolatilityPointsCount'
			from OptionView OV
				join optionRoot ort on ort.optionRootID = OV.optionRootID
				join ContractView CV on Ort.underlyingID = CV.contractID
				--join ExpiryInCalendarView EC on OV.expiryID = EC.expiryID and EC.expCalendarID = CV.expCalendarID
				left join VolaSurfaceDataView VSD on Ort.underlyingID = VSD.contractID and VSD.isDefaultSurface = 1
				left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
					and OV.expiryDate = CSSP.expiryDate
			where 
				CV.contractTypeID < 3
				and CSSP.volaSurfaceDataID is null
			order by CV.contractID, dtExpiry
	else
		select distinct 
				CV.contractID as iContractID,
				CV.symbol as vcSymbolName,
				--EC.expiryDate as dtExpiry,
				OV.expiryDate as dtExpiry,
				case when CSSP.volaSurfaceDataID is null then 0  
				else 1 end 'CustomVolatilityPointsCount'
			from OptionView OV
				join optionRoot ort on ort.optionRootID = OV.optionRootID
				join ContractView CV on Ort.underlyingID = CV.contractID
				--join ExpiryInCalendarView EC on OV.expiryID = EC.expiryID and EC.expCalendarID = CV.expCalendarID
				left join VolaSurfaceDataView VSD on Ort.underlyingID = VSD.contractID and VSD.isDefaultSurface = 1
				left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
					and OV.expiryDate = CSSP.expiryDate
			where 
				CV.contractTypeID < 3
				and CV.symbol = @vcSymbolName
				and CSSP.volaSurfaceDataID is null
			order by CV.contractID, dtExpiry



GO
if exists (select * from dbo.sysindexes where name = N'Contract24' and id = object_id(N'[dbo].[Contract]'))
drop index [dbo].[Contract].[Contract24]
GO

CREATE 
  INDEX [Contract24] ON dbo.Contract ([contractID], [actionID], [contractTypeID], [symbol])
ON [PRIMARY]
GO

ALTER       PROC dbo.usp_MmOptionByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null
as
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null
		set @dtMinExpiry = 0
	select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		o.expiryDate as dtExpiry,
		o.isCall as tiIsCall,
		ort.isFitEnabled as tiIsFitEnabled,
		ort.lotSize as iLotSize,
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,
		o.isManualyEntered as iIsFlex
	from ContractView uc
		inner join OptionRoot ort on uc.contractID = ort.underlyingID
		inner join OptionView o on o.optionRootID = ort.optionRootID
		inner join ContractView c on o.optionID = c.contractID
	where uc.contractID = @iUnderlyingID
		and datediff(d,@dtMinExpiry, o.expiryDate) >= 0

	return @@error

GO
ALTER    PROC usp_MmFuture_Get
	@iFutureRootID int = null
AS
	-- retrieve futures by future root
	set nocount on

	if @iFutureRootID is null
		select
			fc.contractID as iFutureID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcFutureSymbol,
			fc.contractName as vcFutureName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			fc.undPriceProfileID as iUndPriceProfileID,
			fc.optPriceProfileID as iOptPriceProfileID,
			f.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
		   	fcp.priceClose as fPriceClose,
			f.maturityDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			case when exists(select top 1 1 from FutureOption where futureID = fc.contractID) then 1 else 0 end as iHaveOptions
,			isnull(fu.tiGreeksCalculationType ,0) as iShowInFutureContractEq
		from Future f
			inner join ContractView fc on fc.contractID = f.futureID
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			inner join ContractView uc on uc.contractID = fr.underlyingID
		    inner join ContractPrice fcp on fc.contractID = fcp.contractID and fcp.exchangeID is null
			left outer join FutureUnderlying fu on fu.iFutureUnderlyingID = uc.contractID
	else
		select
			fc.contractID as iFutureID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcFutureSymbol,
			fc.contractName as vcFutureName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			fc.undPriceProfileID as iUndPriceProfileID,
			fc.optPriceProfileID as iOptPriceProfileID,
			f.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
			fcp.priceClose as fPriceClose,
			f.maturityDate  as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			case when exists(select top 1 1 from FutureOption where futureID = fc.contractID) then 1 else 0 end as iHaveOptions
,			isnull(fu.tiGreeksCalculationType ,0) as iShowInFutureContractEq
		from Future f
			inner join ContractView fc on fc.contractID = f.futureID
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			inner join ContractView uc on uc.contractID = fr.underlyingID
		    inner join ContractPrice fcp on fc.contractID = fcp.contractID and fcp.exchangeID is null
			left outer join FutureUnderlying fu on fu.iFutureUnderlyingID = uc.contractID
		where
			f.futureRootID = @iFutureRootID

GO
ALTER  PROC usp_MmFutureByIndex_Get
 @iIndexID int,
 @dtMinExpiry datetime = Null 
as

 -------------------------------------------------
 set nocount on
 -------------------------------------------------

 if @dtMinExpiry is null set @dtMinExpiry = 0
 select
  fc.contractID as iContractID,
  fc.contractTypeID as iContractTypeID,
  fc.symbol as vcSymbol,  
  f.maturityDate as dtMaturity,
  iv.indexID as indexID,
  f.futureRootID as iFutureRootID
 from IndexView iv
      inner join FutureRoot fr on fr.underlyingID = iv.indexID
      inner join Future f on f.futureRootID = fr.futureRootID
      inner join ContractView fc on f.FutureID = fc.contractID    
 where iv.indexID = @iIndexID
   and datediff(d,@dtMinExpiry,f.maturityDate) >=0

 return @@error

GO

ALTER      PROC usp_MmFutureByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null	
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @dtMinExpiry is null set @dtMinExpiry = 0	

	select
		f.futureID as iContractID,
		fc.contractTypeID as iContractTypeID,
		fc.symbol as vcSymbol,		
		--e.ExpiryPeriod as vcMaturity,
                f.maturityDate as dtMaturity,
		fu.ContractId as iFutureUnderlyingID,
                f.futureRootID as iFutureRootID,
		isnull(ff.tiGreeksCalculationType, 1) as tiGreeksCalculationType,
		cp.priceClose as fPriceClose
	from ContractView fu
                inner join FutureRoot fr on fr.underlyingID = fu.ContractId
                inner join Future f on f.futureRootID = fr.futureRootID
		inner join ContractView fc on f.futureID = fc.ContractId
		left join ContractPrice cp on f.futureID = cp.ContractID
		left join FutureUnderlying ff on ff.iFutureUnderlyingID = fu.ContractId
	where fu.ContractId = @iUnderlyingID
		and datediff(d, @dtMinExpiry, f.maturityDate) >=0

	return @@error



GO

ALTER    PROC usp_MmFutureOptionsByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null 
		set @dtMinExpiry = 0
	select
		foc.contractID as iContractID,
		foc.contractTypeID as iContractTypeID,
		foc.symbol as vcSymbol,
		fo.strike as fStrike, 
		fo.expiryDate as dtExpiry,
		fo.isCall as tiIsCall,
		fo.futureID as iFutureID
	from FutureOption fo
		inner join Future f on f.futureID = fo.futureID
		inner join FutureRoot fr on fr.futureRootID = f.futureRootID and fr.underlyingID = @iUnderlyingID
		inner join ContractView foc on fo.futureOptionID = foc.contractID
	where  datediff(d,@dtMinExpiry,fo.expiryDate) >= 0

	return @@error

GO


ALTER  PROC dbo.usp_MmOptionByFuture_Get
	@iFutureID int,
	@dtMinExpiry datetime = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null set @dtMinExpiry = 0

	select
		foc.contractID as iContractID,
		foc.contractTypeID as iContractTypeID,
		foc.symbol as vcSymbol,
		fo.strike as fStrike, 
		fo.expiryDate as dtExpiry,
		fo.isCall as tiIsCall,
		f.futureID as iFutureID
	from Future f
		inner join ContractView fc on f.futureID = fc.contractID
		inner join FutureOption fo on f.futureID = fo.futureID
		inner join ContractView foc on fo.futureOptionID = foc.contractID
	where f.futureID = @iFutureID
		and datediff(d,@dtMinExpiry, fo.expiryDate) >= 0

	return @@error

GO
GO
IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='Trade' AND COLUMN_NAME='orderID')
ALTER TABLE Trade
	ADD orderID INT NULL
GO
IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='Trade' AND COLUMN_NAME='execStatus')
ALTER TABLE Trade
	ADD execStatus INT NULL
GO
ALTER TABLE Trade
	ALTER COLUMN execID nvarchar(128) NULL
GO

ALTER  VIEW dbo.TradeView
AS
	select
	 	tradeID, contractID, tradeDate, quantity, price, isBuy, 
		traderID, strategyID, brokerID, brokerCommission, 
		clearingBrokerID, clearingBrokerCommission, tradedIV, 
		spotReference, isPosition, status, execID, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
	from 
		dbo.Trade
	group by
	 	tradeID, contractID, tradeDate, quantity, price, isBuy, 
		traderID, strategyID, brokerID, brokerCommission, 
		clearingBrokerID, clearingBrokerCommission, tradedIV, 
		spotReference, isPosition, status, execID, mark,
		underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
	having
		(min(actionID) = 1)

GO

GO
ALTER  PROC usp_MmTradeByTrader_Get
	@iTraderID int = null
as
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iTraderID is null
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then o.expiryDate else  fo.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
		order by TradeDate
	else
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then o.expiryDate else  fo.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
		where
			tr.traderID = @iTraderID
		order by TradeDate

	return @@error


GO

ALTER     PROC usp_MmTrades_Get
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			te.acronym as traderAcronym,
			te.traderName as traderName,
			tr.strategyID as iStrategyID, 
			st.StrategyName as strategyName,
			tr.brokerID as iBrokerID,
			br.BrokerName as brokerName,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			brc.BrokerName as clearingBrokerName,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			Case when c.contractTypeID in (4,5) then fr.futureRootID else uc.contractID end as iUnderlyingContractID,
	                Case when c.contractTypeID in (4,5) then fr.futureRootSymbol else uc.Symbol end as underlyingContractSymbol,
	                ort.Symbol as optionRootSymbol,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
	                fr.futureRootSymbol as futureRoot,
        	        fu.Symbol as Future,
			tr.actionID as ActionID,
			isnull(o.strike, fo.strike)            as OptionStrike,
			isnull(ec.expiryDate, fec.expiryDate)  as OptionExpityDate	
			
		from Trade tr 
			inner join ( 
				select tradeID, max(seqNum) as seqNum from Trade group by tradeID
			 ) tr2 on tr.seqNum = tr2.seqNum

			-- contract and underlying contract
			inner join ContractView  c  on tr.contractID   = c.contractID
			inner join ContractView  uc on tr.underlyingID = uc.contractID
			left  join ContractPrice cp on tr.contractID   = cp.contractID and cp.exchangeID is null
			left  join ContractView  fu on fu.ContractID   = tr.FutureID
			inner join Trader        te on te.TraderID    = tr.traderID
			left  join Strategy  	 st  on st.StrategyID = tr.StrategyID
			left  join Broker     	 br  on br.brokerID = tr.brokerID 
			left  join Broker        brc on br.brokerID = tr.ClearingBrokerID

			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on datediff(d,o.expiryDate, ec.expiryDate)=0 and ec.expCalendarID = uc.expCalendarID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on datediff(d, fo.expiryDate, fec.expiryDate)=0 and fec.expCalendarID = c.expCalendarID

		order by TradeDate

GO

ALTER     PROC dbo.usp_MmTradeSeqByTrader_Get
	@iTraderID int = null,
	@iMinSeqNum int = null,
	@iMaxSeqNum int = null
AS
-- changed by VK - flex option support added
	set nocount on

	set @iMinSeqNum = isnull(@iMinSeqNum, 0)
	set @iMaxSeqNum = isnull(@iMaxSeqNum, 2147483647)

	if @iTraderID is null
		select
			tr.seqNum as iSeqNum,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.exportSymbol as vcImportID,
			c.contractName as vcContractName,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,
			uc.Symbol as vcUnderlyingSymbol,

			-- case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then  o.expiryDate else  fo.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,

			tl.eventTypeID as iStructureMissed
		from Trade tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			-- left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where
			tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
		order by tr.seqNum
	else
		select
			tr.seqNum as iSeqNum,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,
			uc.Symbol as vcUnderlyingSymbol,

			-- case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then  o.expiryDate  else  fo.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,

			tl.eventTypeID as iStructureMissed
		from Trade tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			-- left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where
			tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
			and tr.traderID = @iTraderID
		order by tr.seqNum

GO

ALTER  PROC dbo.usp_Option_Get
	@iUnderlyingID int = Null,		 
	@dtMinExpiry datetime = Null,
	@iOptionID int = Null,
	@tiBaseInfoOnly tinyint = 0,
	@tiWithPosition tinyint = 0,
	@iTraderID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null set @dtMinExpiry = 0

	if isnull(@tiWithPosition, 0) = 0
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			if @iUnderlyingID is not null
				and @iOptionID is null
				and @tiBaseInfoOnly = 0
				and @tiWithPosition = 0
				
				-- Sharky: speed optimization for quote view

				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName, 
					ORt.underlyingID as iUnderlyingContractID,
					O.strike as fStrike, 
					O.expiryDate as dtExpiry,
					O.isCall as tiIsCall,
					O.actionDate as dtActionDate,
					O.actionID as tiActionID,
					ORt.lotSize as iLotSize,
					ORt.optionRootID as iOptionRootID,
					ORt.symbol as vcOptionRootSymbol,
					O.isManualyEntered as tiIsManualyEntered
				from ContractView C  
					inner join OptionView O on O.optionID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract C1 on ORt.underlyingID = C1.contractID
				where c.contractTypeID=3
					and datediff(d, @dtMinExpiry, O.expiryDate) >=0 
					and ORt.underlyingID = @iUnderlyingID
				order by ORt.underlyingID, O.expiryDate, o.strike, o.isCall desc
			else
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName, 
					ORt.underlyingID as iUnderlyingContractID,
					O.strike as fStrike, 
					O.expiryDate as dtExpiry,
					O.isCall as tiIsCall,
					O.actionDate as dtActionDate,
					O.actionID as tiActionID,
					ORt.lotSize as iLotSize,
					ORt.optionRootID as iOptionRootID,
					ORt.symbol as vcOptionRootSymbol,
					O.isManualyEntered as tiIsManualyEntered
				from ContractView C  
					inner join OptionView O on O.optionID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract C1 on ORt.underlyingID = C1.contractID
				where c.contractTypeID=3
					and datediff(d, @dtMinExpiry, O.expiryDate) >= 0 
					and ((ORt.underlyingID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
					and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
				order by ORt.underlyingID, O.expiryDate, o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				ORt.underlyingID as iUnderlyingContractID,
				O.strike as fStrike, 
				O.expiryDate as dtExpiry,
				O.isCall as tiIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				ORt.lotSize as iLotSize,
				ORt.optionRootID as iOptionRootID,
				ORt.symbol as vcOptionRootSymbol,
				O.isManualyEntered as tiIsManualyEntered 
			from ContractView c  
				inner join OptionView O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID
			where c.contractTypeID=3
				and datediff(d,@dtMinExpiry,O.expiryDate) >= 0 
				and ((ORt.underlyingID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
	end
	else
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				ORt.underlyingID as iUnderlyingContractID,
				O.strike as fStrike, 
				O.expiryDate as dtExpiry,
				O.isCall as tiIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				ORt.lotSize as iLotSize,
				ORt.optionRootID as iOptionRootID,
				ORt.symbol as vcOptionRootSymbol,
				O.isManualyEntered as tiIsManualyEntered 
			from ContractView C  
				inner join OptionView O on O.optionID = C.contractID
				left join TradeView tr on tr.contractID = c.contractID and (tr.traderID = @iTraderID or @iTraderID is Null)
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID

			where c.contractTypeID=3
				and datediff(d,@dtMinExpiry, O.expiryDate) >= 0 
				and ((ORt.underlyingID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName, ORt.underlyingID,
				O.strike, O.expiryDate, O.isCall, O.actionDate, O.actionID,ORt.lotSize, ORt.optionRootID, ORt.symbol,O.isManualyEntered
			order by ORt.underlyingID, O.expiryDate, o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				ORt.underlyingID as iUnderlyingContractID,
				O.strike as fStrike, 
				O.expiryDate as dtExpiry,
				O.isCall as tiIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				ORt.lotSize as iLotSize,
				ORt.optionRootID as iOptionRootID,
				ORt.symbol as vcOptionRootSymbol,
				O.isManualyEntered as tiIsManualyEntered 
			from ContractView c  
				inner join OptionView O on O.optionID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null						
				left join TradeView tr on tr.contractID = c.contractID and (tr.traderID = @iTraderID or @iTraderID is Null)
				join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID
			where c.contractTypeID=3
				and datediff(d,@dtMinExpiry,O.expiryDate) >= 0 
				and ((ORt.underlyingID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
				and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName, ORt.underlyingID,
				O.strike, O.expiryDate, O.isCall, O.actionDate, O.actionID,ORt.lotSize, ORt.optionRootID, ORt.symbol,O.isManualyEntered
	end

	return @@error

GO


ALTER  PROCEDURE dbo.usp_OptionRootMove_Save
	@iOldRootID int, 
	@iNewRootID int
AS

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
						and DateDiff(d, O1.expiryDate, O2.expiryDate)=0
					where O1.optionRootID = @iNewRootID and O2.optionRootID = @iOldRootID)
		begin
			select C1.symbol as vcOptSymbol1, C2.symbol as vcOptSymbol2 From ContractView C1 
					join OptionView O1 on O1.optionID = C1.contractID
					join OptionView O2  
						on O1.isCall = O2.isCall 
						and abs(O1.strike - O2.strike) < 0.009
						and DateDiff(d, O1.expiryDate, O2.expiryDate) = 0
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

		update Trade set optionRootID = @iNewRootID where optionRootID = @iOldRootID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction

	select null as vcOptSymbol1, null as vcOptSymbol2 where 0 = 1
    set @error = 0
finish:
Return(@error)

GO

ALTER         Procedure dbo.usp_OptionVM_Get
	@iOptionID int = null,			-- option's identifier
	@iUnderlyingID int,				-- underlying's identifier
	@dtExpDateMin datetime = null,	-- expiration's date
	@bIsCall bit = null,
	@iSortOrder int = 1	--			-- 1, 2 or 3. see comments below
as
/*
	Created by Andrey Sinkin
	This procedure returns option's data
*/
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @dtExpDateMin is null set @dtExpDateMin = 0

	if @iSortOrder = 1	-- sort by symbol
		select	O.OptionID as iOptionID, 
				O.optionRootID as iOptionRootID, 
				ORt.underlyingID as iUnderlyingID,
				isCall as bIsCall, 
				strike as fStrike, 
				--EC.expiryDate as  dtExpDate,
				O.expiryDate as dtExpDate,
				C.symbol as vcSymbolName, 
				null as iExchangeID,
				C.contractName as vcDescription,
				ORt.isFitEnabled as tiIsFitEnabled,
				ORt.lotSize as iLotSize,
				ORt.isSynthetic as tiIsSynthetic
				from OptionView O
						join ContractView C on O.OptionID = C.contractID
						join ContractPrice CP on CP.contractID = C.contractID and exchangeID is null
						join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				where ORt.underlyingID = @iUnderlyingID
						and (@iOptionID is null or O.OptionID = @iOptionID)
						and datediff(d,@dtExpDateMin, O.expiryDate) >= 0
						and (@bIsCall is null or O.isCall = @bIsCall)
						and  O.isManualyEntered  = 0
				order by C.symbol
	else
	if @iSortOrder = 2	-- sort by isCall+year(expiry)+month(expiry)+strike

		select	O.OptionID as iOptionID, 
				O.optionRootID as iOptionRootID, 
				ORt.underlyingID as iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				O.expiryDate as dtExpDate,
				C.symbol as vcSymbolName, 
				null iExchangeID,
				C.contractName as vcDescription, 
				ORt.isFitEnabled as tiIsFitEnabled,
				ORt.lotSize as iLotSize,
				ORt.isSynthetic as tiIsSynthetic
			from OptionView O
					join ContractView C on O.OptionID = C.contractID
					join ContractPrice on ContractPrice.contractID = C.contractID and exchangeID is null
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
			where ORt.underlyingID = @iUnderlyingID
					and (@iOptionID is null or O.OptionID = @iOptionID)
					and datediff(d, @dtExpDateMin, O.expiryDate) >=0
					and (@bIsCall is null or O.isCall = @bIsCall)
					and  O.isManualyEntered  = 0
			order by isCall, O.expiryDate, /*year(EC.expiryDate),  month(EC.expiryDate),*/ strike
	else
	if @iSortOrder = 3	-- sort by expiry+strike+symbol(without two last symbols)+isCall
		select 	O.OptionID iOptionID,
				O.optionRootID as iOptionRootID, 
				ORt.underlyingID iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				O.expiryDate as dtExpDate,
				C.symbol vcSymbolName, 
				null iExchangeID,
				C.contractName vcDescription,	
				ORt.isFitEnabled as tiIsFitEnabled,
				ORt.lotSize iLotSize,
				ORt.isSynthetic as tiIsSynthetic
			from OptionView O
					join ContractView C on O.OptionID = C.contractID
					join ContractPrice on ContractPrice.contractID = C.contractID and exchangeID is null
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					--join Contract C1 on ORt.underlyingID = C1.contractID
					--join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = isnull( C.expCalendarID, C1.expCalendarID )
			where ORt.underlyingID = @iUnderlyingID
					and (@iOptionID is null or O.OptionID = @iOptionID)
					and datediff(d,@dtExpDateMin,O.expiryDate) >= 0
					and (@bIsCall is null or O.isCall = @bIsCall)
					and  O.isManualyEntered  = 0
			order by O.expiryDate, strike, left(C.symbol, len(C.symbol) - 2), isCall desc
	set @error = @@error
Return(@error)


GO

ALTER    PROCEDURE dbo.usp_OrderByTrader_Get
	@iTraderID int = null
AS
/*
	Created by Sharky
*/
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @iTraderID is null
	begin
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			c.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			null as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
		where
			c.contractTypeID < 3
		union
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			uc.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			o.expiryDate  as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			ort.optionRootID as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
			left join OptionView o on c.contractID = o.optionID
			left join OptionRoot ort on o.optionRootID = ort.optionRootID
			left join ContractView uc on ort.underlyingID = uc.contractID
		where
			c.contractTypeID = 3
	end
	else
	begin
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			c.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			null as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
		where
			c.contractTypeID < 3
			and ord.traderID = @iTraderID
		union
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			uc.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			-- ec.expiryDate as dtExpiry,
			o.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			ort.optionRootID as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
			left join OptionView o on c.contractID = o.optionID
			left join OptionRoot ort on o.optionRootID = ort.optionRootID
			left join ContractView uc on ort.underlyingID = uc.contractID
			--left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull(c.expCalendarID, uc.expCalendarID )
		where
			c.contractTypeID = 3
			and ord.traderID = @iTraderID
	end

return @@error
GO

ALTER  PROCEDURE dbo.usp_OrderSeqByTrader_Get
	@iTraderID int = null,
	@iMinSeqNum int,
	@iMaxSeqNum int
AS
/*
	Created by Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	set @iMinSeqNum = isnull(@iMinSeqNum, 0)
	set @iMaxSeqNum = isnull(@iMaxSeqNum, 2147483647)

	if @iTraderID is null
	begin
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			c.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			null as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
		where
			ord.seqNum between @iMinSeqNum and @iMaxSeqNum
			and c.contractTypeID < 3
		union
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			uc.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			o.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			ort.optionRootID as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
			left join OptionView o on c.contractID = o.optionID
			left join OptionRoot ort on o.optionRootID = ort.optionRootID
			left join ContractView uc on ort.underlyingID = uc.contractID
		where
			ord.seqNum between @iMinSeqNum and @iMaxSeqNum
			and c.contractTypeID = 3
	end
	else
	begin
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			c.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,
			null as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
		where
			ord.seqNum between @iMinSeqNum and @iMaxSeqNum
			and c.contractTypeID < 3
			and ord.traderID = @iTraderID
		union
		select
			ord.orderID as iOrderID,
			ord.seqNum as iSeqNum,
			ord.actionDate as dtActionDate,
			ord.contractID as iContractID,
			ord.traderID as iTraderID,
			ord.destID as iDestID,
			ord.status as iStatus,
			ord.orderType as iOrderType,
			ord.isBuy as tiIsBuy,
			ord.orderQty as iOrderQty,
			ord.orderPrice as fOrderPrice,
			ord.orderDate as dtOrderDate,
			ord.avgPrice as fAvgPrice,
			ord.cumQty as iCumQty,
			ord.clOrderID as vcClOrderID,

--			case when exists(select 1 from OrderExec where orderID = ord.orderID) 
--					then 1 else 0 end tiHasExecs,

			uc.contractID as iUnderlyingID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			o.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			ort.optionRootID as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
			left join OptionView o on c.contractID = o.optionID
			left join OptionRoot ort on o.optionRootID = ort.optionRootID
			left join ContractView uc on ort.underlyingID = uc.contractID
		where
			ord.seqNum between @iMinSeqNum and @iMaxSeqNum
			and c.contractTypeID = 3
			and ord.traderID = @iTraderID
	end

return @@error


GO

ALTER   PROC dbo.usp_OTCFutureOptions_Get
	@futureID int,
	@expiryDate datetime,
	@strike float
AS
	set nocount on

	select
	foc.symbol
	from Future f
		inner join ContractView fc on f.futureID = fc.contractID
		inner join FutureOption fo on f.futureID = fo.futureID
		inner join ContractView foc on fo.futureOptionID = foc.contractID
	where
		f.futureID = @futureID
		and datediff(d,fo.expiryDate , @expiryDate)=0
		and fo.strike = @strike

GO

GO

ALTER    PROCEDURE dbo.usp_PositionsForExternalApp_Get  
	@iContractTypeID int = 2 -- 2 for Stocks,3 - for Options
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Trade table special for A.D.
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iContractTypeID = 2
		begin
			select 
				convert(varchar,tradeDate,103) as 'Trade Date',
				Trader.acronym as 'Account Acronym',
				C.Symbol as 'Stock Symbol',
				C.Symbol as 'Full Option Symbol',
				null as 'Option Root',
				null as 'Expiration Month',
				null as 'Strike',
				'S' as 'Option Type',
				sum(TV.quantity * (case when TV.isBuy <> 0 then 1 else -1 end)) as 'Position',
				null as 'OptionMark',
				cast(CP.priceClose as decimal(15,2)) as [Underlier/Stock Mark],
				1 as 'Contract Multiplier',
				'STOCK' as [OPTION\STOCK],
				null as 'Expiration Date'
			from TradeView TV
				join Trader on TV.traderID = Trader.traderID 
				join Contract C on C.contractID = TV.contractID
				join ContractPrice CP on TV.contractID = CP.contractID and CP.exchangeID is null
			where (tv.status != 1 or tv.status is null) and C.contractTypeID = 2
			group by	convert(varchar,tradeDate,103),
						C.symbol,
						Trader.acronym,	
						CP.priceClose
			order by Trader.acronym,C.symbol
			
		end 
	else if @iContractTypeID = 3
		begin
			select 
				convert(varchar,tradeDate,103) as 'Trade Date',
				Trader.acronym as 'Account Acronym',
				UC.Symbol as 'Stock Symbol',
				left(C.symbol,len(C.symbol) - 2) + cast(Upper(Datename(month, O.expiryDate)) as char(3)) + Cast(Cast(O.strike as int) as varchar)  + Case O.IsCall when 1 then 'C' else 'P' end  as 'Full Option Symbol',
				left(C.symbol,len(C.symbol) - 2) as 'Option Root',
				cast(Upper(Datename(month, O.expiryDate)) as char(3)) as 'Expiration Month',
				Cast(O.strike as decimal(15,2)) as 'Strike',
				Case O.isCall when 1 then 'C' else 'P' end 'Option Type',
				sum(TV.quantity * (case when TV.isBuy <> 0 then 1 else -1 end)) as 'Position',
				cast (CP.priceClose as decimal(15,2)) as 'OptionMark',
				cast(UCP.priceClose as decimal(15,2)) as [Underlier/Stock Mark],
    				ORt.lotSize as 'Contract Multiplier',
				'OPTION' as [OPTION\STOCK],
				convert(varchar,O.expiryDate,103) as 'Expiration Date'
			from TradeView TV
				join Trader on TV.traderID = Trader.traderID 
				join [Option] O on TV.contractID = O.optionID
				join Contract C on C.contractID = O.optionID
				join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				join Contract UC on ORt.underlyingID = UC.contractID
				join ContractPrice CP on TV.contractID = CP.contractID and CP.exchangeID is null
				join ContractPrice UCP on ORt.underlyingID = UCP.contractID and CP.exchangeID is null
			where (tv.status != 1 or tv.status is null)
			group by 	convert(varchar,TV.tradeDate,103),
						C.symbol, --c.contractID
						Trader.acronym,
						UC.Symbol,
						O.expiryDate,
						O.strike,
						O.IsCall,
						ORt.lotSize,
						CP.priceClose,
						UCP.priceClose
			order by convert(varchar,tradeDate,103),
					UC.Symbol,
					Trader.acronym,
					O.expiryDate,
					cast(O.strike as int),O.isCall desc
		end 
	else 
		Raiserror ('Please use 2 or 3 for input parameter (@iContractTypeID = 2 or 3 for Stocks\Options )',16,2)

		
	set @error = @@error
Return(@error)


GO
ALTER  Procedure dbo.usp_ReportMarketStructure_Get
	@iStockID int = Null, 
	@iFilterID int = Null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from [Option] table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------

	declare @dtToday datetime
	set @dtToday = cast(floor(cast(getdate() as float)) as datetime)

	declare @error int		set @error = -1

	declare	@dtLastExpiryDate datetime
		select top 1 @dtLastExpiryDate = expiryDate from [Option] where datediff(d,@dtToday,expiryDate) <=0 order by expiryDate desc

	create table #FilteredStocks(stockID int)

	if @iStockID is null 
		begin
			insert into #FilteredStocks exec usp_FilterData_Get @iFilterID
		end

if @iStockID is not null and @iFilterID is null
	select
		ORt.underlyingID as iUnderlyingContractID,
		ORt.optionRootID as iRootID,
		ORt.isSynthetic as tiIsSynthetic,
		SoRt.synthSkew as fSyntheticSkew,
		SoRt.synthKurt as fSyntheticKurt,
		ORt.symbol as vcRootSymbol,
		SoRt.cashValue as fCashAmount,
		SoRt.synthIsBasket as tiSyntheticIsBasket,
		SoRt.synthYield as fSyntheticYield,
		Contract.symbol as vcSymbol,
		O.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		vw.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join OptionRoot ORt on ORt.optionRootID = O.optionRootID
				left join SyntheticOptionRootView SoRt on SoRt.optionRootID = O.optionRootID
				left join VegaWeight vw on datediff(d, vw.ExpiryDate, O.ExpiryDate) = 0
				join Contract C1 on ORt.underlyingID = C1.contractID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where 	datediff(d,@dtLastExpiryDate,O.expiryDate) > 0 and 
				ORt.underlyingID=@iStockID

else if @iStockID is not null and @iFilterID is not null
	select
		ORt.underlyingID as iUnderlyingContractID,
		ORt.optionRootID as iRootID,
		ORt.isSynthetic as tiIsSynthetic,
		SoRt.synthSkew as fSyntheticSkew,
		SoRt.synthKurt as fSyntheticKurt,
		ORt.symbol as vcRootSymbol,
		SoRt.cashValue as fCashAmount,
		SoRt.synthIsBasket as tiSyntheticIsBasket,
		SoRt.synthYield as fSyntheticYield,
		Contract.symbol as vcSymbol,
		O.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		vw.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join OptionRoot ORt on ORt.optionRootID = O.optionRootID
				left join SyntheticOptionRootView SoRt on SoRt.optionRootID = O.optionRootID
				left join VegaWeight vw on datediff(d, vw.ExpiryDate, O.ExpiryDate) = 0
				join Contract C1 on ORt.underlyingID = C1.contractID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where 	datediff(d,@dtLastExpiryDate,O.expiryDate) > 0 and 
				underlyingID = @iStockID and 
				ORt.underlyingID in (select stockID from #FilteredStocks)

else if @iStockID is null and @iFilterID is not null
	select
		ORt.underlyingID as iUnderlyingContractID,
		ORt.optionRootID as iRootID,
		ORt.isSynthetic as tiIsSynthetic,
		SoRt.synthSkew as fSyntheticSkew,
		SoRt.synthKurt as fSyntheticKurt,
		ORt.symbol as vcRootSymbol,
		SoRt.cashValue as fCashAmount,
		SoRt.synthIsBasket as tiSyntheticIsBasket,
		SoRt.synthYield as fSyntheticYield,
		Contract.symbol as vcSymbol,
		O.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		vw.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join OptionRoot ORt on ORt.optionRootID = O.optionRootID
				left join SyntheticOptionRootView SoRt on SoRt.optionRootID = O.optionRootID
				left join VegaWeight vw on datediff(d, vw.ExpiryDate, O.ExpiryDate) = 0
				join Contract C1 on ORt.underlyingID = C1.contractID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where datediff(d,@dtLastExpiryDate,O.expiryDate) > 0 and 
			ORt.underlyingID in (Select stockID from #FilteredStocks)

else if @iStockID is null and @iFilterID is null
	select
		ORt.underlyingID as iUnderlyingContractID,
		ORt.optionRootID as iRootID,
		ORt.isSynthetic as tiIsSynthetic,
		SoRt.synthSkew as fSyntheticSkew,
		SoRt.synthKurt as fSyntheticKurt,
		ORt.symbol as vcRootSymbol,
		SoRt.cashValue as fCashAmount,
		SoRt.synthIsBasket as tiSyntheticIsBasket,
		SoRt.synthYield as fSyntheticYield,
		Contract.symbol as vcSymbol,
		O.expiryDate as dtExpiry,
		O.strike as fStrike,
		O.isCall as iIsCall,
		CP.priceBid as fPriceBid,
		CP.priceAsk as fPriceAsk,
		CP.priceLast as fPriceLast,
		Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
		CP.priceClose as fPriceClose,
		vw.vegaWeight as fVegaWeight
		from OptionView O
				join ContractView Contract on O.optionID = Contract.contractID
				join OptionRoot ORt on ORt.optionRootID = O.optionRootID
				left join VegaWeight vw on datediff(d, vw.ExpiryDate, O.ExpiryDate) = 0
				left join SyntheticOptionRootView SoRt on SoRt.optionRootID = O.optionRootID
				join Contract C1 on ORt.underlyingID = C1.contractID
				join ContractPrice CP on CP.contractID=O.optionID and CP.contractID=Contract.contractID and CP.exchangeID is null
		where datediff(d,@dtLastExpiryDate,O.expiryDate) > 0 

	if object_id('tempdb..#FilteredStocks') is not null
		drop table #FilteredStocks

	set @error = @@error
Return(@error)

GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmVegaWeight_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MmVegaWeight_Get]
GO

GO
CREATE PROC usp_MmVegaWeight_Get
AS
set nocount on

	select dt.ExpiryDate as dtExpiryDate, VegaWeight as fVegaWeight 
	from (
			select distinct(expiryDate) from FutureOption
			where datediff(d, getDate(), expiryDate) >=0
		union
			select distinct(maturityDate) from Future
			where datediff(d, getDate(), maturityDate) >=0
		union
			select distinct(expiryDate) from OptionView
			where datediff(d, getDate(), expiryDate) >=0
		) dt
	left join VegaWeight vw  on datediff(d, vw.ExpiryDate,dt.expiryDate) = 0  
	order by dt.ExpiryDate

GO

GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmCancelledOrderExec_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MmCancelledOrderExec_Get]
GO

CREATE PROC dbo.usp_MmCancelledOrderExec_Get
	@iTraderID int = null
as
-- derived from usp_MmTradeByTrader_Get
-- by Anar Alimov: loads cancelled/deleted executions/trades with not null orderID
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iTraderID is null
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then o.expiryDate else fo.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from Trade tr 
			-- contract and underlying contract
			inner join [Order] ord on tr.orderID = ord.orderID
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
		-- only deleted trades
		where tr.actionID = 0
		order by TradeDate
	else
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then o.expiryDate else fo.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from Trade tr 
			-- contract and underlying contract
			inner join [Order] ord on tr.orderID = ord.orderID
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
		where
			-- only deleted trades
			tr.traderID = @iTraderID and tr.actionID = 0
		order by TradeDate

	return @@error

GO

ALTER  PROC dbo.usp_ReportsUnderlyingOptions_Get
	@iUnderlyingID int,
	@dtMinExpDate datetime = Null
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @dtMinExpDate is  null
		set @dtMinExpDate = 0

	select
		c.contractID as iOptionID,
		c.symbol as vcSymbol,
		ort.optionRootID as iOptionRootID,
		o.expiryDate as dtExpiry,
		o.strike as fStrike, 
		o.isCall as tiIsCall
	from
		ContractView c  
		inner join OptionView o on o.optionID = c.contractID
		inner join OptionRootView ort on o.optionRootID = ort.optionRootID
	where
		c.contractTypeID = 3
		and ort.underlyingID = @iUnderlyingID
		and datediff(d,@dtMinExpDate, o.expiryDate) >= 0
		
	set @error = @@error
Return(@error)


GO

GO
/*END DATA MODIFICATION SECTION*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.195')
	begin
		update DataInfo set keyValue = '3.46.195' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.195' '3.46.195 update message:' 
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