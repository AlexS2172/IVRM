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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.245' or (keyValue = '4.46.246') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '4.46.246 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '4.46.246' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
/*  ExpiryOV inserting */
GO

if not exists (SELECT name FROM syscolumns WHERE id = object_id(N'[MsOptionCache]') and name = 'expiryOV')
begin
	ALTER TABLE [MsOptionCache] 
		ADD [expiryOV]     [datetime] NOT NULL DEFAULT ('2005-01-01 16:00')
end
GO

if not exists (SELECT name FROM syscolumns WHERE id = object_id(N'[MsOptionCache]') and name = 'TradingClose')
begin
	ALTER TABLE [MsOptionCache] 
		ADD [TradingClose] [datetime] NOT NULL DEFAULT ('2005-01-01 16:30')
end
GO

UPDATE [MsOptionCache] set expiryOV = expiryDate - 1
GO



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodMsOptionCache_Get]')
						and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop procedure [dbo].[usp_EodMsOptionCache_Get] 
GO

CREATE    PROCEDURE dbo.usp_EodMsOptionCache_Get
	@iUndID int
AS
/*	Created by Michael Malyshkin
	Description: this procedure is get information from MsOptionCache table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	select	O.optionID as iOptionID,
		O.symbol as vcSymbol,
		O.expiryDate as dtExpiryDate,
		O.expiryOV as dtExpiryOV,
		O.TradingClose as dtTradingClose,
		O.strike as fStrike,
		O.isCall as tiIsCall,
		O.lotSize as iLotSize
		from MsOptionCache O where O.underlyingID = @iUndID
	
Return(@@error)

GO

GO



if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodMsOptionCache_Save]')
						and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop procedure [dbo].[usp_EodMsOptionCache_Save] 
GO

CREATE PROCEDURE dbo.usp_EodMsOptionCache_Save
	@iUnderlyingID int,
	@vcSymbol varchar(20),
	@dtExpiry datetime,
	@dtExpiryOV datetime,
	@dtTradingClose datetime,
	@fStrike float,
	@tiIsCall tinyint,
	@iLotSize int 
AS
	----------------------------------------------------------------------
	SET NOCOUNT ON
	----------------------------------------------------------------------
	DECLARE @error INT	SET @error = 0
	DECLARE @vcUnderlyingSymbol VARCHAR(20)
	DECLARE @vcError AS VARCHAR(8000)

	-- check parameter
	IF @iUnderlyingID IS NULL OR @vcSymbol IS NULL OR @dtExpiry IS NULL OR @dtExpiryOV IS NULL 
				OR @dtTradingClose IS NULL OR ISNULL(@fStrike, 0) <= 0
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

	INSERT INTO MsOptionCache(underlyingID, symbol, expiryDate, expiryOV, TradingClose, strike, isCall, lotSize)
		VALUES(@iUnderlyingID, @vcSymbol, @dtExpiry, @dtExpiryOV, @dtTradingClose, @fStrike, @tiIsCall, @iLotSize)
	
	IF @@ERROR <> 0 BEGIN SET @error = -5 GOTO finish END

finish:
	RETURN @error
GO

GO


if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Import]')
						and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop procedure [dbo].[usp_Option_Import] 
GO

CREATE PROCEDURE dbo.usp_Option_Import
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
				expiryOV = isnull(@dtExpiry, expiryOV),
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

GO


if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Bulk_Import]')
						and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop procedure [dbo].[usp_Option_Bulk_Import] 
GO


CREATE PROCEDURE dbo.usp_Option_Bulk_Import
	@iUnderlyingContractID int,
	@iBulkXMLData ntext
as
	DECLARE @vcSymbol	varchar(20)
	DECLARE @iIsCall	tinyint
	DECLARE @fStrike	float
	DECLARE @dtExpiry	datetime
	DECLARE @dtExpiryOV	datetime
	DECLARE @dtTradingClose	datetime
	DECLARE @iLotSize	int
	DECLARE @iDoc		int
	DECLARE @fClosePrice	float
	DECLARE @vcUnderlyingSymbol varchar(8)
	DECLARE @isError	tinyint
	DECLARE @ERR_MSG	varchar(8000)
   	
	select @vcUnderlyingSymbol = symbol from ContractView where contractID = @iUnderlyingContractID
	set @isError = 0
	exec sp_xml_preparedocument @iDoc OUTPUT, @iBulkXMLData
	DECLARE BulkDataCursor CURSOR FOR
		select * FROM OPENXML(@iDoc, '/Import/Option')
 			WITH(	Symbol	  	varchar(20),
				IsCall		tinyint,
				Strike		float,
				Expiry		datetime,
				ExpiryOV	datetime,
				TradingClose	datetime,
				LotSize		int,
				ClosePrice	float)
	
	OPEN BulkDataCursor
	FETCH NEXT FROM BulkDataCursor 
	INTO  @vcSymbol, @iIsCall, @fStrike, @dtExpiry, @dtExpiryOV, @dtTradingClose, @iLotSize, @fClosePrice

	WHILE @@FETCH_STATUS = 0
	BEGIN
		declare @iRet int
		--select @iUnderlyingContractID, @vcSymbol, @iIsCall, @fStrike, @dtExpiry, @dtExpiryOV, @dtTradingClose, @iLotSize
		exec @iRet = usp_Option_Import @iUnderlyingContractID, @vcSymbol, @iIsCall, @fStrike, @dtExpiry, @dtExpiryOV, @dtTradingClose, @iLotSize, NULL, @vcUnderlyingSymbol, @fClosePrice
		if @iRet <> 0 
		begin 
		   set @isError = -1	
		   exec usp_EodMsOptionCache_Save @iUnderlyingContractID, @vcSymbol, @dtExpiry, @dtExpiryOV, @dtTradingClose, @fStrike, @iIsCall, @iLotSize
		end

		FETCH NEXT FROM BulkDataCursor 
		INTO @vcSymbol, @iIsCall, @fStrike, @dtExpiry, @dtExpiryOV, @dtTradingClose, @iLotSize, @fClosePrice
	END

	CLOSE      BulkDataCursor
	DEALLOCATE BulkDataCursor

	exec sp_xml_removedocument @iDoc
	return @isError
GO

GO

/* --------------- */
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFutureOption_Import]')
						and OBJECTPROPERTY(id, N'IsProcedure') = 1)
      drop procedure [dbo].[usp_EodFutureOption_Import] 
GO

CREATE   PROC [dbo].[usp_EodFutureOption_Import]
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

			UPDATE FutureOption SET actionDate = GETDATE() WHERE futureOptionID = @iFutureOptionID
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

GO

--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.46.246')
	begin
		update DataInfo set keyValue = '4.46.246' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0 OR 
   not exists (select * from dbo.sysobjects where OBJECTPROPERTY(id, N'IsProcedure') = 1 and
	(id = object_id(N'[dbo].[usp_Option_Bulk_Import]') or id = object_id(N'[dbo].[usp_Option_Import]') or
	 id = object_id(N'[dbo].[usp_EodMsOptionCache_Get]') or id = object_id(N'[dbo].[usp_EodMsOptionCache_Save]') or
	 id = object_id(N'[dbo].[usp_EodFutureOption_Import]')
	))

	ROLLBACK TRANSACTION
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
	select 'DB VERSION WAS CHANGED TO 4.46.246' '4.46.246 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

