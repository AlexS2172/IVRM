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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.170'  or (keyValue = '3.46.171') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.171 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.171' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

/***********************************************************************
		 changing ExpiryCalendar
************************************************************************/

-- check table existance
IF NOT EXISTS (
	SELECT * 
	FROM dbo.sysobjects 
	WHERE ID = object_id(N'[ExpiryCalendar]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1
)
BEGIN
	RAISERROR('Table ExpiryCalendar not found', 20, 1)
END

-- table exists, check column existance
IF NOT EXISTS (
	SELECT * 
	FROM dbo.sysobjects so
	JOIN dbo.syscolumns sc ON so.id = sc.id AND sc.name = 'flagHidden' 
	WHERE so.id = object_id( N'[ExpiryCalendar]' ) AND OBJECTPROPERTY( so.id, N'IsUserTable') = 1
)
BEGIN
	ALTER TABLE ExpiryCalendar ADD flagHidden TINYINT DEFAULT(0)
END
GO

-- check existance of calendars and add new if they aren't exist
IF NOT EXISTS (
	SELECT * 
	FROM ExpiryCalendar
	WHERE expCalendarName LIKE '31' AND expCalendarDesc LIKE 'Internal calendar for flex options' AND flagHidden = 1 
)
BEGIN
	DECLARE @Cntr AS TINYINT
	DECLARE @StrCntr AS NCHAR(2)
	SET @Cntr = 1
	WHILE @Cntr < 32
	BEGIN
		SELECT @StrCntr = 
		CASE LEN(RTRIM(LTRIM(CONVERT( NCHAR(2), @Cntr  ) ) ) )
			WHEN 1 THEN  '0' + RTRIM(LTRIM(CONVERT( NCHAR(2), @Cntr  ) ) )
			ELSE RTRIM(LTRIM(CONVERT( NCHAR(2), @Cntr  ) ) )
		END
		INSERT INTO ExpiryCalendar ( expCalendarName, expCalendarDesc, flagHidden )
		VALUES( @StrCntr, 'Internal calendar for flex options', 1 )
		SET @Cntr = @Cntr + 1
	END
END
GO

-- just to be shure
UPDATE ExpiryCalendar
SET flagHidden = 0
WHERE flagHidden IS NULL

GO


/***********************************************************************
		 changing CustomStrikeSkewPoint 
************************************************************************/
IF NOT EXISTS (
	SELECT * 
	FROM dbo.sysobjects so
	JOIN dbo.syscolumns sc ON so.id = sc.id AND sc.name = 'expiryInCalendarID' 
	WHERE so.id = object_id( N'[CustomStrikeSkewPoint]' ) AND OBJECTPROPERTY( so.id, N'IsUserTable') = 1
)
BEGIN
	ALTER TABLE CustomStrikeSkewPoint  ADD expiryInCalendarID INT NULL 
	CREATE TABLE #temp ( CustomStrikeSkewPointID int, ExpiryInCalendarID int)
 
	INSERT INTO #temp 
		SELECT csspv.customStrikeSkewPointID ,  eic.ExpiryInCalendarID
		FROM CustomStrikeSkewPointView csspv
		JOIN VolaSurfaceDataView vsdv ON vsdv.volaSurfaceDataID = csspv.volaSurfaceDataID
		JOIN ContractView ucv ON vsdv.contractID = ucv.contractID 
		JOIN ExpiryInCalendar eic ON eic.expCalendarID = ucv.expCalendarID AND eic.expiryID = csspv.expiryID
 
	UPDATE CustomStrikeSkewPoint 
	SET ExpiryInCalendarID = #temp.ExpiryInCalendarID
	FROM #temp
	WHERE CustomStrikeSkewPoint.customStrikeSkewPointID = #temp.customStrikeSkewPointID

	DROP table #temp


	ALTER TABLE CustomStrikeSkewPoint 
	ADD CONSTRAINT expCalInIdCheck 
	FOREIGN KEY (expiryInCalendarID) REFERENCES ExpiryInCalendar( expiryInCalendarID ) 
END
GO

--changing ExpiryCalendarView
ALTER  View dbo.ExpiryCalendarView
as
	Select 
		expCalendarID, 
		expCalendarName, 
		expCalendarDesc, 
		actionID, 
		actionDate,
		flagHidden
	from ExpiryCalendar
	where actionID < 3
GO

ALTER  View CustomStrikeSkewPointView
as
-- changed by VK
	Select 
		customStrikeSkewPointID,
		volaSurfaceDataID,
		strike,
		type,
		volatility,
		expiryID,
		isBasePoint,
		expiryInCalendarID,
		actionID,
		actionDate
	from CustomStrikeSkewPoint
	where actionID < 3
GO
ALTER  View dbo.ExpiryInCalendarView
as
	Select 
		expCalendarID,
		expiryID, 
		expiryDate, 
		vegaWeight, 
		actionID, 
		actionDate,
		expiryInCalendarID
	from ExpiryInCalendar
	where actionID < 3


GO

ALTER  PROC dbo.usp_ExpiryCalendar_Get 
	@iExpCalendarID int = null
AS
-- changed by VK
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iExpCalendarID is null
		select 
			expCalendarID as iExpCalendarID,
			expCalendarName as vcExpCalendarName,
			expCalendarDesc as vcExpCalendarDesc,
			flagHidden as vcFlagHidden
		from ExpiryCalendarView 
--		where flagHidden = 0
	else
		select 
			expCalendarID as iExpCalendarID,
			expCalendarName as vcExpCalendarName,
			expCalendarDesc as vcExpCalendarDesc,
			flagHidden as vcFlagHidden
		from ExpiryCalendarView 
		where expCalendarID = @iExpCalendarID /*and flagHidden = 0*/

	return @@error
GO

-- new proc usp_FlexOptionDate_Create - returns calendar id for date
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FlexOptionDate_Create]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FlexOptionDate_Create]
GO

CREATE  PROCEDURE usp_FlexOptionDate_Create
	@Date DATETIME
AS
BEGIN
-- created by VK
	DECLARE @ClndrName		AS NVARCHAR(5)
	DECLARE @ClndrId		AS INT
	DECLARE @Expiry			AS INT
	DECLARE @iWtdVegaMaturity	AS INT	
	DECLARE @vegaW			AS FLOAT( 53 )
	DECLARE	@DTE			AS INT

	-- is there record for exact this date exists?
	SELECT @ClndrId = expCalendarID
	FROM ExpiryInCalendar EIC
	WHERE DATEPART( d, EIC.expiryDate ) = DATEPART( d, @Date ) 
	AND DATEPART( m, EIC.expiryDate ) = DATEPART( m, @Date ) 
	AND DATEPART( yy, EIC.expiryDate ) = DATEPART( yy, @Date ) 

	-- if not - create new records
	IF @ClndrId IS NULL
	BEGIN
		-- get expiry id and calendar name
		SET @Expiry = ( DATEPART( yy, @Date ) - 2000 ) * 12 + DATEPART( m, @Date )
		--SET @ClndrName = RTRIM( LTRIM( STR( DATEPART( d, @Date ) ) ) ) -- calendar name is a day number
		SET @ClndrName = CASE WHEN DATEPART( d, @Date ) < 10 THEN  '0'+
							RTRIM( LTRIM( STR( DATEPART( d, @Date ) ) ) )
						ELSE
							RTRIM( LTRIM( STR( DATEPART( d, @Date ) ) ) )
						END

		-- ensure that expiry exists
		IF NOT EXISTS ( SELECT * FROM Expiry WHERE expiryID = @Expiry )
			INSERT INTO Expiry ( expiryID ) VALUES ( @Expiry )

		-- get calendar id
		SELECT @ClndrId = expCalendarID
		FROM ExpiryCalendarView ECV
		WHERE ECV.expCalendarName = @ClndrName
		IF @ClndrId IS NULL RETURN 0

		-- compute vega weight
		SELECT @iWtdVegaMaturity = CONVERT(INT,KeyValue) 
		FROM DataInfo	
		WHERE dataInfoID = 4
		SET @DTE = DATEDIFF( dd, GETDATE(), @Date )
		IF @DTE < 0 SET @vegaW = NULL 
		ELSE 
			IF @DTE = 0 SET @vegaW = NULL  
			ELSE 
				SET @vegaW = SQRT( @iWtdVegaMaturity / @DTE )

		-- create new record in ExpiryInCalendar
		INSERT INTO ExpiryInCalendar ( expCalendarID, expiryID, expiryDate, vegaWeight )
		VALUES ( @ClndrId, @Expiry, @Date, @vegaW )
	END
	RETURN @ClndrId	-- return calendar id for this date
END

GO
ALTER     procedure dbo.usp_Option_Save
		@iOptionID int,
		@vcSymbol varchar(20) = Null,
		@iExchangeID int, 
		@iUnderlyingContractID int,
		@iIsCall tinyint = Null,
		@fStrike float = Null,
		@dtExpiry datetime = Null,
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

	   if exists (select optionRootID 
		   from OptionRootView
		   where symbol = left(@vcSymbol,len(@vcSymbol)-2)and 
			underlyingID != @iUnderlyingContractID)
		begin
		  RAISERROR ('Error. Can''t insert or update Option. Current Root assigned to different Underlying', 16, 1)
		  RETURN (-1)
		end  

	--===================================================================			
	-- check if option with same symbol exists
	--===================================================================		
	declare	@iOptionRootID int,
			@ContractName varchar(255),
			@iExpiryID int,
			@iExpCalID INT

	select @iOptionRootID = optionRootID from OptionRootView where symbol = left(@vcSymbol,len(@vcSymbol)-2)  and 
		underlyingID=@iUnderlyingContractID	

	declare @ERR_MSG varchar(8000),
			@vcUnderlyingSymbol varchar(10)

	select @vcUnderlyingSymbol = symbol 
		from ContractView 
		where contractID = @iUnderlyingContractID and contractTypeID <= 2

	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
	IF @tiIsManualyEntered = 0
		select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	execute usp_Option_Check @iOptionID = @iOptionID, 
					@vcSymbol = @vcSymbol,	
					@iOptionRootID = @iOptionRootID,
					@dtExpiry = @dtExpiry,
					@iExpiryID = @iExpiryID,
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
			IF @tiIsManualyEntered = 0
				insert into Contract (contractTypeID, symbol, contractName)
				values (3, @vcSymbol, @ContractName)
			ELSE
			BEGIN
				-- saving flex option
				EXEC @iExpCalID = usp_FlexOptionDate_Create @dtExpiry 
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				insert into Contract (contractTypeID, symbol, contractName, expCalendarID )
				values (3, @vcSymbol, @ContractName, @iExpCalID )
			END
		
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iOptionID = @@identity			  
			
			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				values (@iOptionID, @iOptionRootID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		 end
	   else
		 begin
			IF @tiIsManualyEntered = 0
				SET @iExpCalID = NULL
			ELSE
				-- saving flex option
				EXEC @iExpCalID = usp_FlexOptionDate_Create @dtExpiry 
			update Contract set symbol = isNull(@vcSymbol,symbol), 
				contractName = isNull(@ContractName,contractName),
				expCalendarID = @iExpCalID
			where contractID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			declare @iOldOptionID as int
			select @iOldOptionID = optionRootID from [Option] 
				where optionID = @iOptionID

			update [Option]
				set optionRootID = @iOptionRootID,
					isCall = isNull(@iIsCall, isCall),
					strike = isNull(@fStrike, strike),
					expiryID = isNull(@iExpiryID, expiryID)
				where optionID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			if @iOldOptionID is not null and @iOptionRootID is not null
			begin
			  update Trade set OptionRootId = @iOptionRootID where 	OptionRootId = @iOldOptionID
	  		  if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		 end		

		if @tiIsManualyEntered in (0,1) 
			begin
--select @iOptionID as '@iOptionID*******************'
				update [Option] set isManualyEntered = @tiIsManualyEntered where optionID = @iOptionID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end 

	Commit Transaction

    set @error = 0
finish:
if (@error != 0) return (@error) else return @iOptionID

GO

ALTER  proc dbo.usp_BoOptionWithPrice_Get
	  @iUnderlyingID int		 
  as
-- changed by VK - flex option support added
	select	c.contractID as iContractID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName,
			o.optionRootID as iOptionRootID,
			ec.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheo,
			c.exportSymbol as vcExportSymbol,
			o.isManualyEntered as vcIsFlex
	from OptionRoot ort
			inner join OptionView o on o.optionRootID = ort.optionRootID
			inner join ContractView c on c.contractID = o.optionID
			inner join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
	where	ort.underlyingID = @iUnderlyingID
			and ec.expiryDate >= getdate()
	order by ort.optionRootID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
 
  return @@error
GO

ALTER  PROC dbo.usp_MmTradeByTrader_Get
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

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
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
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
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

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
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
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
		where
			tr.traderID = @iTraderID
		order by TradeDate

	return @@error

GO

ALTER    PROC dbo.usp_MmOptionByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null
as
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null set @dtMinExpiry = 0

	select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		ec.expiryDate as dtExpiry,
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
		inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
	where uc.contractID = @iUnderlyingID
		and ec.expiryDate >= @dtMinExpiry
	--order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc

	return @@error
GO

ALTER  PROC dbo.usp_MmContractForTradeByUnd_Get
	@iUnderlyingID int
as
-- changed by VK - flex option support added
	-- return 2 recordsets: 1st for option contracts, 2nd futures & futures options contracts
	set nocount on

	declare @dtToday datetime

	set @dtToday = cast(floor(cast(getdate() as float)) as datetime)

	-- options rowset
	select	
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		c.contractName as vcContractName, 
		cp.priceClose as fPriceClose,
		cp.priceTheo as fPriceTheoClose,
		
		ec.expiryDate as dtExpiry,
		o.strike as fStrike,
		o.isCall as tiIsCall,
		ort.optionRootID as iOptionRootID
	from OptionRootView ort
		inner join OptionView o on o.optionRootID = ort.optionRootID
		inner join ContractView c on c.contractID = o.optionID
		inner join Contract uc on uc.contractID = ort.underlyingID
		inner join ExpiryInCalendarView ec on ec.expiryID = o.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
		left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
	where
		ort.underlyingID = @iUnderlyingID
		and ec.expiryDate >= @dtToday
	order by year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc

	-- futures & futures options rowset
	select * from
	(
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName, 
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			c.symbol as vcFutureSymbol
		from FutureRoot fr
			inner join Future f on f.futureRootID = fr.futureRootID
			inner join ContractView c on c.contractID = f.futureID
			left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
		where
			fr.underlyingID = @iUnderlyingID
		union
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName, 
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
			ec.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			fc.symbol as vcFutureSymbol
		from FutureRoot fr
			inner join Future f on f.futureRootID = fr.futureRootID
			inner join Contract fc on fc.contractID = f.futureID
			inner join FutureOption o on o.futureID = f.futureID
			inner join ContractView c on c.contractID = o.futureOptionID
			inner join ExpiryInCalendarView ec on ec.expiryID = o.expiryID and ec.expCalendarID = fc.expCalendarID
			left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where
			fr.underlyingID = @iUnderlyingID
			and ec.expiryDate >= @dtToday
	) as c order by c.vcFutureSymbol, year(c.dtExpiry), month(c.dtExpiry), c.fStrike, c.tiIsCall desc

	return @@error

GO

ALTER  PROC dbo.usp_MmTradeSeqByTrader_Get
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

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.exportSymbol as vcImportID,
			c.contractName as vcContractName,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,
			uc.Symbol as vcUnderlyingSymbol,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
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
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
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

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,
			uc.Symbol as vcUnderlyingSymbol,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
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
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- trade log
			left join TradeLog tl on tr.underlyingID = tl.underlyingID
		where
			tr.seqNum >= @iMinSeqNum
			and tr.seqNum <= @iMaxSeqNum
			and tr.traderID = @iTraderID
		order by tr.seqNum

GO

ALTER  PROCEDURE dbo.usp_OrderByTrader_Get
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
			ec.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			ort.optionRootID as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
			left join OptionView o on c.contractID = o.optionID
			left join OptionRoot ort on o.optionRootID = ort.optionRootID
			left join ContractView uc on ort.underlyingID = uc.contractID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull(c.expCalendarID, uc.expCalendarID )
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
			ec.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,
			ort.optionRootID as iOptionRootID
			
		from [Order] ord
			left join ContractView c on ord.contractID = c.contractID
			left join OptionView o on c.contractID = o.optionID
			left join OptionRoot ort on o.optionRootID = ort.optionRootID
			left join ContractView uc on ort.underlyingID = uc.contractID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull(c.expCalendarID, uc.expCalendarID )
		where
			c.contractTypeID = 3
			and ord.traderID = @iTraderID
	end

return @@error
GO

ALTER  PROC dbo.usp_CustomStrikeSkewPoint_Get 
	@iCustomStrikeSkewPointID int = null, 
	@iVolaSurfaceDataID int = null,
	@iType int = null,
	@dtExpDate datetime = null
as
-- changed by VK - flex option support added
/*
	Description:	this procedure get information from 
					CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare	@iExpiryID int

	-- Sharky: faster request for special parameters' set
	if @iExpiryID is null and @iCustomStrikeSkewPointID is null
		and @iVolaSurfaceDataID is not null and @iType is not null
	begin
		select 
			CSSP.customStrikeSkewPointID as iCustomStrikeSkewPointID,
			CSSP.volaSurfaceDataID as iVolaSurfaceDataID,
			CSSP.strike as fStrike, 
			CSSP.type as iType,
			CSSP.volatility as fVolatility,
			--dateadd(mm, CSSP.expiryID, '19991201') as dtExpDate,
			EICV.expiryDate as dtExpDate,
			CSSP.isBasePoint as iIsBasePoint --,
			--CSSP.actionDate as dtActionDate,
			--CSSP.actionID as tiActionID
		from CustomStrikeSkewPoint CSSP with(INDEX = IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID, nolock)
			--join VolaSurfaceData vsd on CSSP.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
			join ExpiryInCalendarView EICV on CSSP.expiryInCalendarID = EICV.expiryInCalendarID
		where 
			CSSP.volaSurfaceDataID = @iVolaSurfaceDataID
			and CSSP.type = @iType
			and CSSP.actionID < 3
	end
	else
	begin
		-- Sharky: old variant
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)

		select 
			CSSP.customStrikeSkewPointID as iCustomStrikeSkewPointID,
			CSSP.volaSurfaceDataID as iVolaSurfaceDataID,
			CSSP.strike as fStrike, 
			CSSP.type as iType,
			CSSP.volatility as fVolatility,
			--dateadd(mm, CSSP.expiryID, '19991201') as dtExpDate,
			--convert(datetime,convert(varchar(6),E.expiryDate,112) + '01') as dtExpDate,
			EICV.expiryDate as dtExpDate,
			CSSP.isBasePoint as iIsBasePoint --,
			--CSSP.actionDate as dtActionDate,
			--CSSP.actionID as tiActionID
		from CustomStrikeSkewPointView CSSP with(INDEX = IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID, nolock)
			join ExpiryInCalendarView EICV on CSSP.expiryInCalendarID = EICV.expiryInCalendarID
			--join VolaSurfaceData on CSSP.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
			--join Contract C on  VolaSurfaceData.contractID = C.contractID
			--join ExpiryInCalendarView E on E.expCalendarID = C.expCalendarID and CSSP.expiryID = E.expiryID
		where 
			customStrikeSkewPointID = isnull(@iCustomStrikeSkewPointID, customStrikeSkewPointID)
			and CSSP.volaSurfaceDataID = isnull(@iVolaSurfaceDataID, CSSP.volaSurfaceDataID) 
			and type = isnull(@iType, type) 
			and CSSP.expiryID = isnull(@iExpiryID, CSSP.expiryID)
			--and E.expiryID = isnull(@iExpiryID, E.expiryID)
	end

	set @error = @@error
	return(@error)

GO

/*
ALTER    PROCEDURE dbo.usp_Position_Get
	@iUnderlyingID int = Null,
	@iTraderID int = Null,
	@iBookID int = Null,
	@iStrategyID int = Null,
	@dtMaxDate datetime = null,
	@dtToday datetime = null
AS
-- changed by VK - flex option support added
	set nocount on

	if @dtToday is null
	begin
		set @dtToday = cast(floor(cast(getdate() as float)) as datetime)
		declare @iDeltaGMT int
		 
		exec @iDeltaGMT = usp_TimeZoneActimeBias_Get
		 
		set @dtToday = dateadd(minute, @iDeltaGMT, @dtToday)
	end

	select 
		-- all
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
	
		-- stock
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,

		-- index
		i.yield as fYield,
	
		-- option
		null as dtExpiry,
		null as fStrike,
		null as bIsCall,
		c.contractID as iUnderlyingContractID,
		c.contractTypeID as iUnderlyingContractTypeID,
		c.Symbol as vcUnderlyingSymbol,
		
		-- trade
		sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when tr.tradeDate < @dtToday then 1 else 0 end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when tr.tradeDate >= @dtToday then tr.price else 0 end) * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		null as fPosDailyTheoPrevDay,
		null as fPriceTheoClose
	
	from TradeView tr 
		join ContractView c  on tr.contractID = c.contractID
		left join StockView s on c.contractID = s.stockID
		left join IndexView i on c.contractID = i.indexID
	where c.contractTypeID in (1, 2)
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and tr.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)
	group by c.contractID, c.contractTypeID, c.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield
	
	union

	select 
		-- all
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,

		-- stock
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,

		-- index
		i.yield as fYield,
	
		-- option
		EC.expiryDate as dtExpiry,
		o.strike as fStrike,
		o.isCall as bIsCall,
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		
		-- trade
		sum(tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantitySum,
		sum(tr.price * ORt.lotSize * tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosLTD,
		sum((case when tr.tradeDate < @dtToday then 1 else 0 end) * tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQuantityDailyPrevDate,
		sum((case when tr.tradeDate >= @dtToday then tr.price else 0 end) * tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyToday,
		sum((case when tr.tradeDate < @dtToday then isnull(cp.priceTheo, 0) else 0 end) * tr.quantity * ORt.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosDailyTheoPrevDay,
		cp.priceTheo as fPriceTheoClose

	from TradeView tr 
		join ContractView C on tr.contractID = C.contractID
		join ContractPrice CP on C.contractID = cp.contractID and exchangeID is null
		left join (OptionView o 
						join OptionRoot ORt on o.optionRootID = ORt.optionRootID 
						join Contract UC on ORt.underlyingID = UC.contractID) on c.contractID = o.optionID
--		left join (OptionView o join Contract UC on o.underlyingContractID = UC.contractID) on c.contractID = o.optionID
		join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = isnull( C.expCalendarID, UC.expCalendarID )
		left join StockView s on uc.contractID = s.stockID
		left join IndexView i on uc.contractID = i.indexID

	where c.contractTypeID = 3
		and (@dtMaxDate is null or @dtMaxDate is not null and tr.tradeDate < @dtMaxDate)
		and (@iUnderlyingID is null or @iUnderlyingID is not null and uc.contractID = @iUnderlyingID)
		and (@iTraderID is null or @iTraderID is not null and tr.traderID = @iTraderID)
		and (@iBookID is null or @iBookID is not null and tr.bookID = @iBookID)
		and (@iStrategyID is null or @iStrategyID is not null and tr.strategyID = @iStrategyID)

	group by C.contractID, C.contractTypeID, C.Symbol, s.divFreq, s.divDate, s.divAmt, i.yield,
		EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol, cp.priceTheo

	order by vcSymbol*/
GO

ALTER  PROC dbo.usp_UnderlyingOptions_Get
	@iUnderlyingID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0,
	@dtMinExpiry datetime = Null
AS
-- changed by VK - flex option support added
	set nocount on
	
	if @dtMinExpiry is null set @dtMinExpiry = 0

	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				if isnull(@iUnderlyingID, 0) <> 0
					select	c.contractID as iContractID,
						c.symbol as vcSymbol,
						ort.underlyingID as iUnderlyingContractID,
						o.strike as fStrike, 
						ec.expiryDate as dtExpiry,
						o.isCall as tiIsCall,
						ort.lotSize as iLotSize,
						ort.optionRootID as iOptionRootID
					from ContractView c  
						inner join [Option] o on o.optionID = c.contractID
						join OptionRoot ort on o.optionRootID = ort.optionRootID
						inner join Contract uc on ort.underlyingID = uc.contractID
						inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
						join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					where c.contractTypeID = 3
						and ort.underlyingID = @iUnderlyingID
						and ec.expiryDate >= @dtMinExpiry
					order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
				else
					select	c.contractID as iContractID,
						c.symbol as vcSymbol,
						ort.underlyingID as iUnderlyingContractID,
						o.strike as fStrike, 
						ec.expiryDate as dtExpiry,
						o.isCall as tiIsCall,
						ort.lotSize as iLotSize,
						ort.optionRootID as iOptionRootID
					from ContractView c  
						inner join [Option] o on o.optionID = c.contractID
						join OptionRoot ort on o.optionRootID = ort.optionRootID
						inner join Contract uc on ort.underlyingID = uc.contractID
						inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
						join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					where c.contractTypeID = 3
						and ec.expiryDate >= @dtMinExpiry
					order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
				    inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
							join OptionRoot ort on cig.contractID = ort.underlyingID
							join ContractView uc on ort.underlyingID = uc.contractID
							inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
							join OptionRoot ort on cig.contractID = ort.underlyingID					
							join ContractView uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, ec.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
				    inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, ec.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
					join OptionRootView ort on cig.contractID = ort.underlyingID					
					inner join ContractView uc on ort.underlyingID = uc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, ec.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
					join OptionRootView ort on cig.contractID = ort.underlyingID					
					inner join ContractView uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, ec.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
	end

	return @@error

GO

ALTER  PROC dbo.usp_UnderlyingOptionsForOrder_Get
	@iUnderlyingID int
AS
-- changed by VK - flex option support added
	set nocount on

	select
		ort.optionRootID as iOptionRootID,
		o.optionID as iOptionID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		o.isCall as tiIsCall,
		ec.expiryDate as dtExpiry
	from OptionRootView ort
		left join OptionView o on ort.optionRootID = o.optionRootID
		left join ContractView c on c.contractID = o.optionID
		left join Contract uc on ort.underlyingID = uc.contractID
		left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
	where
		ort.underlyingID = @iUnderlyingID
		and c.contractTypeID = 3
		and ec.expiryDate >= getdate()
	order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc

Return (0)

GO

ALTER     Procedure dbo.usp_OptionVM_Get
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

	if @iSortOrder = 1	-- sort by symbol
		select	O.OptionID as iOptionID, 
				O.optionRootID as iOptionRootID, 
				ORt.underlyingID as iUnderlyingID,
				isCall as bIsCall, 
				strike as fStrike, 
				EC.expiryDate as  dtExpDate,
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
						join Contract C1 on ORt.underlyingID = C1.contractID
						join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = isnull( C.expCalendarID, C1.expCalendarID )
				where ORt.underlyingID = @iUnderlyingID
						and (@iOptionID is null or O.OptionID = @iOptionID)
						and (@dtExpDateMin is null or EC.expiryDate >= @dtExpDateMin)
						and (@bIsCall is null or O.isCall = @bIsCall)
				order by C.symbol
	else
	if @iSortOrder = 2	-- sort by isCall+year(expiry)+month(expiry)+strike

		select	O.OptionID as iOptionID, 
				O.optionRootID as iOptionRootID, 
				ORt.underlyingID as iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				EC.expiryDate as dtExpDate,
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
					join Contract C1 on ORt.underlyingID = C1.contractID
					join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = isnull( C.expCalendarID, C1.expCalendarID )
			where ORt.underlyingID = @iUnderlyingID
					and (@iOptionID is null or O.OptionID = @iOptionID)
					and (@dtExpDateMin is null or EC.expiryDate >= @dtExpDateMin)
					and (@bIsCall is null or O.isCall = @bIsCall)
			order by isCall, year(EC.expiryDate),  month(EC.expiryDate), strike
	else
	if @iSortOrder = 3	-- sort by expiry+strike+symbol(without two last symbols)+isCall
		select 	O.OptionID iOptionID,
				O.optionRootID as iOptionRootID, 
				ORt.underlyingID iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				EC.expiryDate as dtExpDate,
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
					join Contract C1 on ORt.underlyingID = C1.contractID
					join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = isnull( C.expCalendarID, C1.expCalendarID )
			where ORt.underlyingID = @iUnderlyingID
					and (@iOptionID is null or EC.expiryDate = @iOptionID)
					and (@dtExpDateMin is null or EC.expiryDate >= @dtExpDateMin)
					and (@bIsCall is null or O.isCall = @bIsCall)
			order by EC.expiryDate, strike, left(C.symbol, len(C.symbol) - 2), isCall desc
	set @error = @@error
Return(@error)

GO

ALTER           Procedure dbo.usp_CustomStrikeSkewPoint_Save 
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
	DECLARE @expInCalendarID AS INT
	declare	@iExpiryID int

	set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)

	if exists (
		select * 
		from CustomStrikeSkewPoint 
		where volaSurfaceDataID = @iVolaSurfaceDataID and strike = @fStrike and 
			type = @iType and expiryID = @iExpiryID and actionID < 3 and 
			(@iCustomStrikeSkewPointID <> CustomStrikeSkewPointID or @iCustomStrikeSkewPointID is null)
	)
	begin
		if @iCustomStrikeSkewPointID is null
			select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
			from CustomStrikeSkewPoint 
			where volaSurfaceDataID = @iVolaSurfaceDataID and strike = @fStrike and 
				type = @iType and expiryID = @iExpiryID and actionID < 3
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

	SELECT @expInCalendarID = expiryInCalendarID
	FROM ExpiryInCalendarView 
	WHERE expiryDate = @dtExpDate

	IF @expInCalendarID IS NULL 
	BEGIN
		DECLARE @errDate AS NVARCHAR(40)
		SET @errDate = RTRIM(  LTRIM( CAST( @dtExpDate AS NVARCHAR(40) )  ) )
		SET @errStrk1 = CAST( @fStrike AS INT ) 
		SET @errStrk2 = CAST( ( @fStrike - FLOOR( @fStrike ) ) * 10 AS INT )
		RAISERROR ('Expiration date %s was not found for strike %d.%d, VolSurf %d', 16, 1, @errDate, @errStrk1, @errStrk2 , @iVolaSurfaceDataID ) WITH LOG
		RETURN -1
	END

	Begin Transaction

	if @iCustomStrikeSkewPointID is null 
	begin
		insert into CustomStrikeSkewPoint ( volaSurfaceDataID, type, strike, volatility, expiryID, 
							isBasePoint, expiryInCalendarID ) 
			values (@iVolaSurfaceDataID, @iType, @fStrike, @fVolatility,  @iExpiryID, 
							@iIsBasePoint, @expInCalendarID )
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
			expiryID = isnull(@iExpiryID,@iExpiryID),
			isBasePoint = isnull(@iIsBasePoint,isBasePoint)
		where customStrikeSkewPointID=@iCustomStrikeSkewPointID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	end
	Commit Transaction

	set @error = 0
finish:

Return(@error)
GO

ALTER       PROC dbo.usp_AspEodUpdateMarketStructure_DefaultIVs_Execute 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	-------------------------------------------------
	-- default IVs
	insert into VolaSurfaceData (contractID, isDefaultSurface, baseUnderlinePrice)
	select c.contractID,
		1,
		isnull(cp.priceClose, 0)
	from Contract c
		inner join ContractPrice cp on cp.contractID = c.contractID
		left join VolaSurfaceData vsd on vsd.contractID = c.contractID and vsd.isDefaultSurface = 1 and vsd.actionID < 3
	where c.actionID < 3
		and c.contractTypeID < 3
		and vsd.contractID is null

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating default IV surface: Fail to create missed IV surfaces', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into CustomStrikeSkewPoint (volaSurfaceDataID, type, strike, volatility, isBasePoint, expiryID, expiryInCalendarID)
	select vsd.volaSurfaceDataID,
		eod_cssp.type,
		eod_cssp.strike,
		eod_cssp.volatility,
		eod_cssp.isBasePoint,
		eod_cssp.expiryID
-- VK on
		, EIC.expiryInCalendarID
-- VK off
	from ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp
		inner join ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.volaSurfaceDataID = eod_cssp.volaSurfaceDataID
		inner join EodUnderlyingMap eod_um on eod_um.eodContractID = eod_vsd.contractID
		inner join VolaSurfaceData vsd on vsd.contractID = eod_um.contractID
--VK on
		inner join Contract c on c.contractID = vsd.contractID
		inner join ExpiryInCalendarView EIC on EIC.expiryID = eod_cssp.expiryID and c.expCalendarID = EIC.expCalendarID
--VK off
		left join CustomStrikeSkewPoint cssp on 
			cssp.volaSurfaceDataID = vsd.volaSurfaceDataID
			and abs(cssp.strike - eod_cssp.strike) < 0.009
			and cssp.type = eod_cssp.type
			and cssp.expiryID = eod_cssp.expiryID 
			and cssp.actionID < 3
	where
		cssp.CustomStrikeSkewPointID is null
		and eod_cssp.actionID < 3

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating default IV surface: Fail to add new IV points', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	update CustomStrikeSkewPoint
	set type = eod_cssp.type,
		strike = eod_cssp.strike,
		volatility = eod_cssp.volatility,
		isBasePoint = eod_cssp.isBasePoint,
		expiryID = eod_cssp.expiryID
-- VK on
		, expiryInCalendarID = EIC.expiryInCalendarID
-- VK off
	from ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp
		inner join ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.volaSurfaceDataID = eod_cssp.volaSurfaceDataID
		inner join EodUnderlyingMap eod_um on eod_um.eodContractID = eod_vsd.contractID
		inner join VolaSurfaceData vsd on vsd.contractID = eod_um.contractID
--VK on
		inner join Contract c on c.contractID = vsd.contractID
		inner join ExpiryInCalendarView EIC on EIC.expiryID = eod_cssp.expiryID and c.expCalendarID = EIC.expCalendarID
--VK off
		inner join CustomStrikeSkewPoint cssp on 
			cssp.volaSurfaceDataID = vsd.volaSurfaceDataID
			and abs(cssp.strike - eod_cssp.strike) < 0.009
			and cssp.type = eod_cssp.type
			and cssp.expiryID = eod_cssp.expiryID 
			and cssp.actionID < 3
	where 
	     eod_cssp.actionID < 3

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating default IV surface: Fail to update IV points', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	create table #CustStrikeskewpoint (customstrikeskewpoint int)
	insert into #CustStrikeskewpoint (customstrikeskewpoint) --values 
		select csp.customStrikeSkewPointID from contract c 
			inner join VolaSurfaceData vsd on vsd.contractID= c.ContractID
			inner join customstrikeskewpoint csp on vsd.volaSurfaceDataID = csp.volaSurfaceDataID 
		where 
			not exists(select top 1 1 from optionRoot orr where orr.ActionId<3 and orr.underlyingID =c.ContractID)
			and c.ActionId <3
			and  c.contractTypeId <3
		group by csp.customStrikeSkewPointID
	

	delete from customstrikeskewpoint where customStrikeSkewPointID in (
		select customstrikeskewpoint from #CustStrikeskewpoint
	)
/*
	----------------------------------------------------------------------------------
	delete from customstrikeskewpoint where customStrikeSkewPointID in (
	--update customstrikeskewpoint set ActionId = 3 where customStrikeSkewPointID in (	
		select csp.customStrikeSkewPointID from contract c 
			inner join VolaSurfaceData vsd on vsd.contractID= c.ContractID
			inner join customstrikeskewpoint csp on vsd.volaSurfaceDataID = csp.volaSurfaceDataID 
		where 
			not exists(select top 1 1 from optionRoot orr where orr.ActionId<3 and orr.underlyingID =c.ContractID)
			and c.ActionId <3
			and  c.contractTypeId <3
		group by csp.customStrikeSkewPointID
	)
*/
	delete from customstrikeskewpoint where customStrikeSkewPointID in (
	--update customstrikeskewpoint set ActionId = 3 where customStrikeSkewPointID in (	
		select csp.customStrikeSkewPointID from contract c 
			inner join VolaSurfaceData vsd on vsd.contractID= c.ContractID
			inner join customstrikeskewpoint csp on vsd.volaSurfaceDataID = csp.volaSurfaceDataID and  datediff(d,csp.actionDate, getDate()) >0
		where 
			exists(select top 1 1 from optionRoot orr where orr.ActionId<3 and orr.underlyingID =c.ContractID)
			and c.ActionId <3
			and  c.contractTypeId <3
			and c.ContractID  not in (		
				select  tt.contractID from (
					select t.contractID, t.symbol, max(t.maxDate) as md from (
						select  c.contractID, c.symbol, csp.actionDate as maxDate from contract c 
							inner join VolaSurfaceData vsd on vsd.contractID= c.ContractID
							inner join customstrikeskewpoint csp on vsd.volaSurfaceDataID = csp.volaSurfaceDataID and csp.expiryID> 66
						where 
							exists(select top 1 1 from optionRoot orr where orr.ActionId<3 and orr.underlyingID =c.ContractID)
							and c.ActionId <3
							and  c.contractTypeId <3
						group by  c.ContractID, c.symbol, csp.actionDate) T
					group by t.contractID, t.symbol
				) TT where datediff(d,TT.md, getDate())<>0)
				group by csp.customStrikeSkewPointID
		)


	if @iWithTran is not null commit tran @vcTranName
	
finish:
	return @error
GO

ALTER  PROC dbo.usp_BoCustomStrikeSkewPoint_Default_Save 
	@iContractID int,
	@fStrike float,
	@iExpiryID int,
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
			and expiryID = @iExpiryID 
			and actionID < 3
-- VK on
	DECLARE @expInCalID AS INT
	SELECT  @expInCalID = c.expCalendarID
	FROM volaSurfaceData vsd
	JOIN Contract c ON c.contractID = vsd.contractID
	WHERE vsd.volaSurfaceDataID = @iVolaSurfaceDataID
-- VK off
  
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					isBasePoint,
					expiryID, expiryInCalendarID)
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						-1,
						@iExpiryID, @expInCalID )
	
				if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

			end
		else
			begin
				update CustomStrikeSkewPoint set 
					volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					type = isnull(@iType,type),
					strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility),
					expiryID = isnull(@iExpiryID,expiryID ),
					expiryInCalendarID = isnull( @expInCalID, expiryInCalendarID )
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
	   			if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction

finish:
	Return(@error)
GO

ALTER     procedure dbo.usp_CustomStrikeSkewDefaultPoint2_Save
	@iContractID int,
	@iExpiryID int,
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

	if @iVolaSurfaceDataID is not null
		begin
			DECLARE @expInCalID AS INT 
			SELECT @expInCalID  = expCalendarID FROM Contract WHERE contractID = @iContractID
			insert into CustomStrikeSkewPoint(VolaSurfaceDataID, strike, type, volatility, isBasePoint, expiryID, expiryInCalendarID )
				select distinct 
					@iVolaSurfaceDataID, round(ov.strike, 4) as fStrike, @iType, @fVolatility, -1, @iExpiryID, @expInCalID 
				from OptionView ov
					join OptionRootView ort on ov.optionRootID = ort.optionRootID
				where ov.expiryID = @iExpiryID 
					and ort.underlyingID = @iContractID
					and not exists(
						select 1 
						from CustomStrikeSkewPointView cssp
						where cssp.VolaSurfaceDataID = @iVolaSurfaceDataID and 
							round(cssp.strike, 4) = round(ov.strike, 4)
							and cssp.expiryID = ov.expiryID 
					)
		end
GO

ALTER  Procedure dbo.usp_CustomStrikeSkewDefaultPointFromIV_Save 
	@iContractID int,
	@fStrike float = Null,
	@dtExpDate datetime = Null,
	@iType int,
	@fVolatility float
AS
/*
	Created by Michael Malyshkin
	Description: this procedure save inf. into CustomStrikeSkewPoint table
	Edited by Zaur Nuraliev 07/08/2002
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iCustomStrikeSkewPointID int 
		set @iCustomStrikeSkewPointID = Null

	declare	@iExpiryID int
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)

	DECLARE @expInCalID AS INT 
	SELECT @expInCalID  = expCalendarID FROM Contract WHERE contractID = @iContractID		

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int 
		set @iVolaSurfaceDataID = 0

	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
		from CustomStrikeSkewPoint 
		where volaSurfaceDataID = @iVolaSurfaceDataID 
			and abs(strike - @fStrike) < 0.009
			and type = @iType 
			and expiryID = @iExpiryID 
			and actionID < 3
			and expiryInCalendarID = @expInCalID
  
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					isBasePoint,
					expiryID, expiryInCalendarID )
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						-1,
						@iExpiryID, @expInCalID)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			end
		else
			begin
				update CustomStrikeSkewPoint set 
					volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					type = isnull(@iType,type),
					strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility),
					expiryID = isnull(@iExpiryID,@iExpiryID),
					expiryInCalendarID = isnull( @expInCalID, expiryInCalendarID )
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction

    set @error = 0

finish:
Return(@error)
GO

ALTER    Procedure dbo.usp_CustomStrikeSkewPoint_Copy 
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
				expiryID,
				isBasePoint,
				expiryInCalendarID
			into #SourcePoints
			from CustomStrikeSkewPoint
			where volaSurfaceDataID=@iVolaSurfaceDataIDSrc
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
		delete CustomStrikeSkewPoint where volaSurfaceDataID=@iVolaSurfaceDataIDDest
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
	
		insert into CustomStrikeSkewPoint (volaSurfaceDataID,strike,type,volatility,expiryID,isBasePoint, expiryInCalendarID)
		select 	volaSurfaceDataID,
				strike,
				type,
				volatility,
				expiryID,
				isBasePoint,
				expiryInCalendarID
				from #SourcePoints
		if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction
    set @error = 0
finish:
	drop table #SourcePoints
Return(@error)
GO

ALTER    Procedure dbo.usp_EodDefaultIvPoint_Save 
	@iContractID int,
	@fStrike float,
	@iExpiryID int,
	@fVolatility float
AS
/*
	Created by Michael Malyshkin
	Description: this procedure save Default Iv points
*/
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

	DECLARE @expInCalID AS INT 
	SELECT @expInCalID  = expCalendarID FROM Contract WHERE contractID = @iContractID		
		
	select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
		from CustomStrikeSkewPoint 
		where volaSurfaceDataID = @iVolaSurfaceDataID 
			and abs(strike - @fStrike) < 0.009
			and type = @iType 
			and expiryID = @iExpiryID 
			and actionID < 3
			and expiryInCalendarID = @expInCalID
  
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					isBasePoint,
					expiryID, expiryInCalendarID)
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						-1,
						@iExpiryID, @expInCalID )
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			end
		else
			begin
				update CustomStrikeSkewPoint set 
					volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					type = isnull(@iType,type),
					strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility),
					expiryID = isnull(@iExpiryID,@iExpiryID)
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction

    set @error = 0

finish:
Return(@error)

GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FlexOptionByUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FlexOptionByUnderlying_Get]
GO

CREATE    PROC dbo.usp_FlexOptionByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null
as
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null set @dtMinExpiry = 0

	select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		ec.expiryDate as dtExpiry,
		o.isCall as tiIsCall,
		ort.isFitEnabled as tiIsFitEnabled,
		ort.lotSize as iLotSize,
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol
	from ContractView uc
		inner join OptionRoot ort on uc.contractID = ort.underlyingID
		inner join OptionView o on o.optionRootID = ort.optionRootID
		inner join ContractView c on o.optionID = c.contractID
		inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
	where uc.contractID = @iUnderlyingID and ec.expiryDate >= @dtMinExpiry and o.isManualyEntered = 1
	--order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc

	return @@error
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingsWithFlexOptions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingsWithFlexOptions_Get]
GO

CREATE   PROC dbo.usp_UnderlyingsWithFlexOptions_Get
as
select distinct uc.symbol, uc.contractID, cp.priceClose 
from [option] o
join OptionRoot ort on o.optionRootID = ort.optionRootID
join Contract c on c.contractID = o.optionID
join Contract uc on uc.contractID = ort.underlyingID
join ExpiryInCalendar eic on eic.expiryID = o.expiryID and eic.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
join ContractPrice cp on uc.contractID = cp.contractID
where o.isManualyEntered = 1 and expiryDate > getdate()
GO

ALTER     proc dbo.usp_BoOptionsForReconcile_Get
	@iUnderlyingID int		 
  as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select	C.contractID as iContractID,
		C.contractTypeID as iContractTypeID,
		C.symbol as vcSymbol,
		ORt.underlyingID as iUnderlyingContractID,
		O.strike as fStrike, 
		O.expiryID as iExpiryID,
		EC.expiryDate as dtExpiry,
		O.isCall as bIsCall,
		O.actionDate as dtActionDate,
		O.actionID as tiActionID,
		ORt.lotSize as iLotSize,
		ORt.optionRootID as iOptionRootID,
		ORt.symbol as vcOptionRootSymbol,
		O.isManualyEntered as tiIsManualyEntered,
		C.exportSymbol as vcExportSymbol
	from OptionView O  
		left join ContractView C on O.optionID = C.contractID
		left join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
		left join OptionRoot ORt on O.optionRootID = ORt.optionRootID
		left join Contract C1 on ORt.underlyingID = C1.contractID
		left join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = isnull( C.expCalendarID, C1.expCalendarID )
	where c.contractTypeID=3
		and ORt.underlyingID = @iUnderlyingID

	set @error = @@error
Return(@error)
GO

--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.171')
	begin
		update DataInfo set keyValue = '3.46.171' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.171' '3.46.171 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

