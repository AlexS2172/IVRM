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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.176'  or (keyValue = '3.46.177') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.177 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.177' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

ALTER   PROC dbo.usp_CustomStrikeSkewPoint_Get 
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
			join ExpiryInCalendarView EICV on CSSP.expiryInCalendarID = EICV.expiryInCalendarID and CSSP.expiryID = EICV.expiryID
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
			join ExpiryInCalendarView EICV on CSSP.expiryInCalendarID = EICV.expiryInCalendarID and CSSP.expiryID = EICV.expiryID
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


ALTER            Procedure dbo.usp_CustomStrikeSkewPoint_Save 
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
	DECLARE @errStrk1 AS INT
	DECLARE @errStrk2 AS INT

	SET @iExpiryID = (YEAR(@dtExpDate) - 2000) * 12 + MONTH(@dtExpDate)

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

	IF EXISTS (
		SELECT * 
		FROM CustomStrikeSkewPoint 
		WHERE volaSurfaceDataID = @iVolaSurfaceDataID AND strike = @fStrike AND 
			type = @iType AND expiryID = @iExpiryID AND actionID < 3 AND @expInCalendarID = expiryInCalendarID AND 
			(@iCustomStrikeSkewPointID <> CustomStrikeSkewPointID OR @iCustomStrikeSkewPointID IS NULL)
	)
	BEGIN
		IF @iCustomStrikeSkewPointID IS NULL
			SELECT @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
			FROM CustomStrikeSkewPoint 
			WHERE volaSurfaceDataID = @iVolaSurfaceDataID AND strike = @fStrike AND 
				type = @iType AND expiryID = @iExpiryID AND actionID < 3
		ELSE IF @iCustomStrikeSkewPointID IS NOT NULL 
			BEGIN
				RAISERROR ('Row in CustomStrikeSkewPoint table  with same volaSurfaceDataID, strike, type and expDate already exists!', 16, 1)
				RETURN (-1)
			END
		END 
	
	BEGIN TRANSACTION

	IF @iCustomStrikeSkewPointID IS NULL 
	BEGIN
		INSERT INTO CustomStrikeSkewPoint ( volaSurfaceDataID, type, strike, volatility, expiryID, 
							isBasePoint, expiryInCalendarID ) 
			VALUES (@iVolaSurfaceDataID, @iType, @fStrike, @fVolatility,  @iExpiryID, 
							@iIsBasePoint, @expInCalendarID )
			IF @@ERROR <> 0 BEGIN SET @error = 1 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
			SET @iCustomStrikeSkewPointID=@@IDENTITY
	END
	ELSE
	BEGIN
		UPDATE CustomStrikeSkewPoint SET 
			volaSurfaceDataID = ISNULL(@iVolaSurfaceDataID,volaSurfaceDataID),
			type = ISNULL(@iType,type),
			strike = ISNULL(@fStrike,strike),
			volatility = ISNULL(@fVolatility,volatility),
			expiryID = ISNULL(@iExpiryID,@iExpiryID),
			isBasePoint = ISNULL(@iIsBasePoint,isBasePoint)
		WHERE customStrikeSkewPointID=@iCustomStrikeSkewPointID
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



--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.177')
	begin
		update DataInfo set keyValue = '3.46.177' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.177' '3.46.177 update message:' 
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