/*
SP for EOD service
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
BEGIN TRANSACTION
GO
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.43.082' or (keyValue = '3.43.083')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.43.083 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.43.083' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

/*DATA MODIFICATION SECTION*/

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[dbo].[MsOptionCache]') 
	   AND 	  OBJECTPROPERTY(id, N'IsUserTable') = 1)
    DROP TABLE [dbo].[MsOptionCache]
GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[dbo].[MsOptionRootCache]') 
	   AND 	  OBJECTPROPERTY(id, N'IsUserTable') = 1)
    DROP TABLE [dbo].[MsOptionRootCache]
GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[dbo].[MsUnderlyingCache]') 
	   AND 	  OBJECTPROPERTY(id, N'IsUserTable') = 1)
    DROP TABLE [dbo].[MsUnderlyingCache]
GO

CREATE TABLE [dbo].[MsOptionCache] (
	[optionID] [int] IDENTITY (1, 1) NOT NULL ,
	[underlyingID] [int] NOT NULL ,
	[symbol] [varchar] (20) NOT NULL ,
	[expiryDate] [smalldatetime] NOT NULL ,
	[strike] [float] NOT NULL ,
	[isCall] [tinyint] NOT NULL ,
	[lotSize] [int] NOT NULL ,
	CONSTRAINT [PK_MsOptionCache] PRIMARY KEY  CLUSTERED 
	(
		[optionID]
	)  ON [PRIMARY] 
) ON [PRIMARY]
GO

CREATE  INDEX [IX_MsOptionCache_underlyingID] ON [dbo].[MsOptionCache]([underlyingID]) ON [PRIMARY]
GO

CREATE TABLE [dbo].[MsUnderlyingCache] (
	[underlyingID] [int] NOT NULL ,
	[updateDate] [datetime] NULL ,
	CONSTRAINT [PK_MsUnderlyingCache] PRIMARY KEY  CLUSTERED 
	(
		[underlyingID]
	)  ON [PRIMARY] 
) ON [PRIMARY]
GO


IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodBasketIndexComp_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodBasketIndexComp_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodDefaultIvPoint_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodDefaultIvPoint_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodExpirationDate_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodExpirationDate_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodIndexUnderlyingBeta_Clear' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodIndexUnderlyingBeta_Clear
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodIrPoint_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodIrPoint_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodIrCurve_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodIrCurve_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodMsLoad_Init' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodMsLoad_Init
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodMsOption_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodMsOption_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodMsOptionCache_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodMsOptionCache_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodMsUnderlying_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodMsUnderlying_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodMsUnderlyingCache_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodMsUnderlyingCache_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOptClosePrice_Complete_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOptClosePrice_Complete_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOptClosePrice_Init_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOptClosePrice_Init_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOptClosePrice_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOptClosePrice_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOptionRootWithPosition_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOptionRootWithPosition_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOptTheoPrice_Complete_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOptTheoPrice_Complete_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOptTheoPrice_Init_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOptTheoPrice_Init_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOptTheoPrice_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOptTheoPrice_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodSynthRootCompWithPosition_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodSynthRootCompWithPosition_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUndClosePrice_Complete_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUndClosePrice_Complete_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUndClosePrice_Init_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUndClosePrice_Init_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUndClosePrice_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUndClosePrice_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUnderlying_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUnderlying_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUnderlyingAverageVolumes_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUnderlyingAverageVolumes_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUnderlyingIndexBeta_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUnderlyingIndexBeta_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUnderlyingSkewKurt_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUnderlyingSkewKurt_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUndExpirationsWithEmptyIvSurface_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUndExpirationsWithEmptyIvSurface_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUndOptionWithPos_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUndOptionWithPos_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUndWithOptionsPositions_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUndWithOptionsPositions_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUndWithPositions_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUndWithPositions_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodVolaBands_Complete_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodVolaBands_Complete_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodVolaBands_Init_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodVolaBands_Init_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodVolaBands_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodVolaBands_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodWeightedVega_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodWeightedVega_Save
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Option_Check' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Option_Check
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUnderlying_Import' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUnderlying_Import
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOption_Import' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOption_Import
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodMsUnderlyingCache_Check' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodMsUnderlyingCache_Check
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodMsLoad_Complete' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodMsLoad_Complete
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Trade_ClearDeleted' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Trade_ClearDeleted
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodMsUnderlyingCache_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodMsUnderlyingCache_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodMsOptionCache_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodMsOptionCache_Get
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   PROCEDURE dbo.usp_EodBasketIndexComp_Get 
	@iBasketIndexID int = null
AS
/*
	Created by: Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iBasketIndexID is null
	begin
		select
			i.indexID as iBasketIndexID,
			idef.stockID as iUnderlyingID,
			idef.weight as fWeight
		from
			IndexDefinitionView idef
			inner join IndexView i on idef.indexID = i.indexID and isnull(i.isBasket, 0) <> 0
		where
			isnull(idef.weight, 0) <> 0
	end
	else
	begin
		select
			i.indexID as iBasketIndexID,
			idef.stockID as iUnderlyingID,
			idef.weight as fWeight
		from
			IndexDefinitionView idef
			inner join IndexView i on idef.indexID = i.indexID and isnull(i.isBasket, 0) <> 0
		where
			idef.indexID = @iBasketIndexID
			and isnull(idef.weight, 0) <> 0
	end
		
	set @error = @@error
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



CREATE   Procedure dbo.usp_EodDefaultIvPoint_Save 
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

	select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
		from CustomStrikeSkewPoint 
		where volaSurfaceDataID = @iVolaSurfaceDataID 
			and abs(strike - @fStrike) < 0.009
			and type = @iType 
			and expiryID = @iExpiryID 
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
					expiryID)
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						-1,
						@iExpiryID)
	
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE    Procedure dbo.usp_EodExpirationDate_Get
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select distinct expiryDate as dtExpiryDate
	from ExpiryInCalendarView
	where expiryDate > getdate()
	order by expiryDate

	set @error = @@error
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


CREATE      Procedure dbo.usp_EodIndexUnderlyingBeta_Clear
	@iIndexID int = null, 	--the index identifier
	@iContractID int = null 	--the stock identifier
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iIndexID is null and @iContractID is null
		update IndexBeta set actionID=3
			where actionID < 3

	else if @iIndexID is null
		update IndexBeta set actionID=3
			where actionID < 3
				and contractID = @iContractID

	else if @iContractID is null
		update IndexBeta set actionID=3
			where 
				actionID < 3
				and indexID = @iIndexID

	if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

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

CREATE   PROCEDURE dbo.usp_EodIrCurve_Get
	@iCurveID int = null
AS
/*
	Created by: Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	
	if @iCurveID is null
	begin
		select
			curveID as iCurveID,
			ruleTypeID as iRuleTypeID, 
			positionThreshold as mPositionThreshold
		from
			IRCurveView
	end
	else
	begin
		select
			curveID as iCurveID,
			ruleTypeID as iRuleTypeID, 
			positionThreshold as mPositionThreshold
		from
			IRCurveView
		where
			curveID = @iCurveID
	end

	set @error = @@error
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

CREATE   PROCEDURE dbo.usp_EodIrPoint_Get
	@iCurveID int = null
AS
/*
	Created by: Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @dtToday datetime
	set @dtToday = getdate()
	
	if @iCurveID is null
	begin
		select
			curveID as iCurveID,
			case periodTypeID
				when 1 then datediff(day, @dtToday, dateadd(day, num, @dtToday))
				when 2 then datediff(day, @dtToday, dateadd(week, num, @dtToday))
				when 3 then datediff(day, @dtToday, dateadd(month, num, @dtToday))
				when 4 then datediff(day, @dtToday, dateadd(year, num, @dtToday))
			end as iDTE,
			shortRate as fRateShort, 
			longRate as fRateLong,
			HTBRate as fRateHTB
		from
			IRPointView
		order by
			iDTE
	end
	else
	begin
		select
			curveID as iCurveID,
			case periodTypeID
				when 1 then datediff(day, @dtToday, dateadd(day, num, @dtToday))
				when 2 then datediff(day, @dtToday, dateadd(week, num, @dtToday))
				when 3 then datediff(day, @dtToday, dateadd(month, num, @dtToday))
				when 4 then datediff(day, @dtToday, dateadd(year, num, @dtToday))
			end as iDTE,
			shortRate as fRateShort, 
			longRate as fRateLong,
			HTBRate as fRateHTB
		from
			IRPointView
		where
			curveID = @iCurveID
		order by
			iDTE
	end

	set @error = @@error
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

CREATE    PROCEDURE dbo.usp_EodMsLoad_Init
	@iUnderlyingID int = null	
as
/*
	Created by Sharky
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	declare @error int		set @error = 0

	if @iUnderlyingID is null
	begin
		Begin Transaction MsCacheClear	
	
		TRUNCATE TABLE MsOptionCache
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0  Rollback Transaction MsCacheClear goto finish end
	
		TRUNCATE TABLE MsUnderlyingCache
		if @@error <> 0 begin set @error = -2 if @@TranCount > 0  Rollback Transaction MsCacheClear goto finish end
	
		Commit Transaction MsCacheClear
	end
	else
	begin
		Begin Transaction MsCacheClear	
	
		Delete from MsOptionCache where underlyingID = @iUnderlyingID
		if @@error <> 0 begin set @error = -1 if @@TranCount > 0  Rollback Transaction MsCacheClear goto finish end
	
		Delete from MsUnderlyingCache where underlyingID = @iUnderlyingID
		if @@error <> 0 begin set @error = -2 if @@TranCount > 0  Rollback Transaction MsCacheClear goto finish end
	
		Commit Transaction MsCacheClear
	end

finish:
	return @error


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE   PROCEDURE dbo.usp_EodMsOptionCache_Save
	@iUnderlyingID int,
	@vcSymbol varchar(20),
	@dtExpiry datetime,
	@fStrike float,
	@tiIsCall tinyint,
	@iLotSize int
AS
/*
	Created by Michael Malyshkin
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	declare @error int		set @error = 0
	declare @vcUnderlyingSymbol varchar(20)
	declare @vcError as varchar(8000)

	-- check parameter
	if @iUnderlyingID is null or @vcSymbol is null or @dtExpiry is null or isnull(@fStrike, 0) <= 0
	begin
		raiserror ('Invalid parameter!', 16, 1)
		set @error = -1
		goto finish 
	end

	-- check underlying existance
	if not exists (select 1 from MsUnderlyingCache where underlyingID = @iUnderlyingID)
	begin
		raiserror ('Invalid underlying contract!', 16, 1)
		set @error = -2
		goto finish 
	end

	-- check the Expiry
	declare @iExpiryID int
	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	if not exists (select 1 from ExpiryView where expiryID = @iExpiryID)
	begin
		set @vcError = 'Invalid expiry ''' + Convert(varchar(50),@dtExpiry) + ''' for option ''' + @vcSymbol + ''' !'
		Raiserror(@vcError, 16, 1)
		set @error = -3
		goto finish 
	end

	-- check option root symbol
	if len(@vcSymbol) < 3
	begin
		set @vcError = 'Invalid option symbol ''' + @vcSymbol + ''': at least 3 characters expected!'
		Raiserror(@vcError, 16, 1)
		set @error = -4
		goto finish 
	end

	-- correct lotsize
	if isnull(@iLotSize, 0) <= 0
		set @iLotSize = 100

	insert into MsOptionCache
		(underlyingID, symbol, expiryDate, strike, isCall, lotSize)
	values
		(@iUnderlyingID, @vcSymbol, @dtExpiry, @fStrike, @tiIsCall, @iLotSize)
	
	if @@error <> 0 begin set @error = -5 goto finish end

finish:
	return @error


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  PROCEDURE dbo.usp_EodMsUnderlyingCache_Save
	@iUnderlyingID int,
	@dtUpdateDate datetime = null
AS
/*
	Created by Michael Malyshkin
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	declare @error int		set @error = 0

	-- check parameter
	if @iUnderlyingID is null
	begin
		raiserror ('Invalid parameter!', 16, 1)
		set @error = -1
		goto finish 
	end

	-- check underlying existance
	if not exists (select 1 from ContractView where contractID = @iUnderlyingID and contractTypeID < 3)
	begin
		raiserror ('Invalid underlying contract!', 16, 1)
		set @error = -2
		goto finish 
	end

	if @dtUpdateDate is null set @dtUpdateDate = getdate()

	-- update underlying cache
	Begin Transaction MsUnderlyingCache

	if not exists (select 1 from MsUnderlyingCache where underlyingID = @iUnderlyingID)
	begin
		insert into MsUnderlyingCache
			(underlyingID, updateDate)
		values
			(@iUnderlyingID, @dtUpdateDate)

		if @@error <> 0 begin set @error = -3 if @@TranCount > 0  Rollback Transaction MsUnderlyingCache goto finish end
	end
	else
	begin
		update
			MsUnderlyingCache
		set
			updateDate = @dtUpdateDate
		where
			underlyingID = @iUnderlyingID and updateDate < @dtUpdateDate

		if @@error <> 0 begin set @error = -4 if @@TranCount > 0  Rollback Transaction MsUnderlyingCache goto finish end
	end

	Commit Transaction MsUnderlyingCache

finish:
	return @error


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE      procedure dbo.usp_EodOptClosePrice_Complete_Save
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
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end					 

		set @vcExecString1 ='insert into ContractPrice (contractID, priceClose, actionDate) select contractID, priceClose,' + @vcDate
		set @vcExecString2 ='from ' + @vcTableName + ' EOD where not exists(select * from ContractPrice C where C.contractID = EOD.contractID and isnull(C.exchangeID, 0) = 0)'
			exec (@vcExecString1 + @vcExecString2)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end					 
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
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE       procedure dbo.usp_EodOptClosePrice_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcConstrName varchar(8000),
			@vcIndexName varchar(8000),
			@vcExecString1 varchar(8000)

	set @vcTableName = '##ETSEODOP' + + cast(db_name() as varchar(8000))
	set @vcConstrName = 'PK_ETSEODOP' + cast(db_name() as varchar(8000))
	set @vcIndexName = 'IND1_ETSEODOP' + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	--number of minutes of EOD timeout
	declare @dtEODTimeOut varchar(80)

	select @dtEODTimeOut = keyValue from DataInfo where dataInfoID = 7
	if @dtEODTimeOut is null
		begin
			Raiserror('Can''t define EOD operation timeout! You DB contains incorrect settings in DataInfo!',16,1)
			Return(-1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			set @vcExecString1 = 'if exists(select * from ' + @vcTableName + ' where DATEDIFF(mi,actionDate,GetDate()) >' + @dtEODTimeOut + ') drop table ' + @vcTableName 
			exec(@vcExecString1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
			Return (-1) 
		end

	exec('create table ' + @vcTableName + '(
		rowID int identity(1,1),
		contractID int not null,
		priceClose float not null,
		actionDate smalldatetime default GetDate())')

	exec('ALTER TABLE ' + @vcTableName + ' ADD CONSTRAINT ' + @vcConstrName + ' PRIMARY KEY CLUSTERED (rowID)')

	exec('CREATE NONCLUSTERED INDEX ' + @vcIndexName + ' ON ' + @vcTableName + ' (contractID)')

	set @error = @@error
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE          procedure dbo.usp_EodOptClosePrice_Save
	@iContractID int,
	@fPriceClose float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000),
			@vcDate varchar(8000)

	set @vcTableName = '##ETSEODOP' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	set @vcDate = '''' + convert(varchar(12),getdate(),112) + '''' 

	set @vcExecString1 = 'insert into ' + @vcTableName + '(contractID, priceClose) values(' + cast(@iContractID as varchar(10)) + ',' + cast(@fPriceClose as varchar(500)) + ')'

	if object_id(@vcFullTableName) is not null
		begin

			exec(@vcExecString1)
				if @@error<>0 begin set @error = @@error Return(@error) end 
		end

	set @vcExecString1 = 'if (select count(*) from ' + @vcTableName + '(nolock)) > 1999' + 
		' begin update ContractPrice set priceClose=EOD.priceClose,actionDate=' + @vcDate + 
		' from ContractPrice CP join ' + @vcTableName + ' EOD on CP.contractID = EOD.contractID and isnull(CP.exchangeID, 0) = 0'
	set @vcExecString2 =' insert into ContractPrice (contractID, priceClose, actionDate) select contractID, priceClose,' + @vcDate +
		' from ' + @vcTableName + ' EOD where not exists(select * from ContractPrice C where C.contractID = EOD.contractID and isnull(C.exchangeID, 0) = 0)' +
		' truncate table ' + @vcTableName + ' end'

	if object_id(@vcFullTableName) is not null
		begin
			exec (@vcExecString1 + @vcExecString2)
		end

finish:
    set @error = @@error
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

CREATE   PROCEDURE dbo.usp_EodOptionRootWithPosition_Get 
	@iUnderlyingID int = null
AS
/*
	Created by: Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iUnderlyingID is null
	begin
		select distinct
			ORT.underlyingID as iUnderlyingID,
			ORT.optionRootID as iRootID,
			ORT.symbol as vcSymbol,
			ORT.IsSynthetic as tiIsSynthetic, 
			ORT.lotSize as iLotSize, 
			SOR.cashValue as fCash,
			SOR.synthSkew as fSyntheticSkew, 
			SOR.synthKurt as fSyntheticKurt,
			SOR.synthYield as fSyntheticYield, 
			SOR.synthIsBasket as tiSyntheticIsBasket
		from
			OptionRootView as ORT
			left join SyntheticOptionRootView as SOR on ORT.optionRootID = SOR.optionRootID
			join OptionView O on O.optionRootID=ORT.optionRootID  
			join TradeView TV on TV.contractID=O.optionID
	end
	else
	begin
		select distinct
			ORT.underlyingID as iUnderlyingID,
			ORT.optionRootID as iRootID,
			ORT.symbol as vcSymbol,
			ORT.IsSynthetic as tiIsSynthetic, 
			ORT.lotSize as iLotSize, 
			SOR.cashValue as fCash,
			SOR.synthSkew as fSyntheticSkew, 
			SOR.synthKurt as fSyntheticKurt,
			SOR.synthYield as fSyntheticYield, 
			SOR.synthIsBasket as tiSyntheticIsBasket
		from
			OptionRootView as ORT
			left join SyntheticOptionRootView as SOR on ORT.optionRootID = SOR.optionRootID
			join OptionView O on O.optionRootID=ORT.optionRootID  
			join TradeView TV on TV.contractID=O.optionID
		where
			ORT.underlyingID = @iUnderlyingID
	end
		
	set @error = @@error
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


CREATE      procedure dbo.usp_EodOptTheoPrice_Complete_Save
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
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end					 

		set @vcExecString1 ='insert into ContractPrice (contractID, priceTheo, actionDate) select contractID, priceTheo,' + @vcDate
		set @vcExecString2 ='from ' + @vcTableName + ' EOD where not exists(select * from ContractPrice C where C.contractID = EOD.contractID and isnull(C.exchangeID, 0) = 0)'
			exec (@vcExecString1 + @vcExecString2)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end					 
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
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE        procedure dbo.usp_EodOptTheoPrice_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcConstrName varchar(8000),
			@vcIndexName varchar(8000),
			@vcExecString1 varchar(8000)

	set @vcTableName = '##ETSEODTP' + + cast(db_name() as varchar(8000))
	set @vcConstrName = 'PK_ETSEODTP' + cast(db_name() as varchar(8000))
	set @vcIndexName = 'IND1_ETSEODTP' + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	--number of minutes of EOD timeout
	declare @dtEODTimeOut varchar(80)

	select @dtEODTimeOut = keyValue from DataInfo where dataInfoID = 7
	if @dtEODTimeOut is null
		begin
			Raiserror('Can''t define EOD operation timeout! You DB contains incorrect settings in DataInfo!',16,1)
			Return(-1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			set @vcExecString1 = 'if exists(select * from ' + @vcTableName + ' where DATEDIFF(mi,actionDate,GetDate()) >' + @dtEODTimeOut + ') drop table ' + @vcTableName 
			exec(@vcExecString1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
			Return (-1) 
		end

	exec('create table ' + @vcTableName + '(
		rowID int identity(1,1),
		contractID int not null,
		priceTheo float not null,
		actionDate smalldatetime default GetDate())')

	exec('ALTER TABLE ' + @vcTableName + ' ADD CONSTRAINT ' + @vcConstrName + ' PRIMARY KEY CLUSTERED (rowID)')

	exec('CREATE NONCLUSTERED INDEX ' + @vcIndexName + ' ON ' + @vcTableName + ' (contractID)')

	set @error = @@error
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE  procedure dbo.usp_EodOptTheoPrice_Save
	@iContractID int,
	@fPriceTheo float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000),
			@vcDate varchar(8000)

	set @vcTableName = '##ETSEODTP' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	set @vcDate = '''' + convert(varchar(12),getdate(),112) + '''' 

	set @vcExecString1 = 'insert into ' + @vcTableName + '(contractID, priceTheo) values(' + cast(@iContractID as varchar(10)) + ',' + cast(@fPriceTheo as varchar(500)) + ')'

	if object_id(@vcFullTableName) is not null
		begin

			exec(@vcExecString1)
				if @@error<>0 begin set @error = @@error Return(@error) end 
		end

	set @vcExecString1 = 'if (select count(*) from ' + @vcTableName + '(nolock)) > 1999' + 
		' begin update ContractPrice set priceTheo=EOD.priceTheo,actionDate=' + @vcDate + 
		' from ContractPrice CP join ' + @vcTableName + ' EOD on CP.contractID = EOD.contractID and isnull(CP.exchangeID, 0) = 0'
	set @vcExecString2 =' insert into ContractPrice (contractID, priceTheo, actionDate) select contractID, priceTheo,' + @vcDate +
		' from ' + @vcTableName + ' EOD where not exists(select * from ContractPrice C where C.contractID = EOD.contractID and isnull(C.exchangeID, 0) = 0)' +
		' truncate table ' + @vcTableName + ' end'

	if object_id(@vcFullTableName) is not null
		begin
			exec (@vcExecString1 + @vcExecString2)
		end

finish:
    set @error = @@error
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

CREATE   PROCEDURE dbo.usp_EodSynthRootCompWithPosition_Get 
	@iUnderlyingID int = null
AS
/*
	Created by: Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iUnderlyingID is null
	begin
		select distinct	
			ort.underlyingID as iUnderlyingID,
			ort.optionRootID as iRootID,
			sup.underlyingID as iCompUndID, 
			sup.SyntheticCoeff as fWeight
		from
			SyntheticUnderlyingParamView as sup
			inner join SyntheticOptionRootView as sor
					on sor.syntheticOptRootID = sup.syntheticOptRootID
			inner join OptionRootView as ort
					on sor.optionRootID = ort.optionRootID
			join OptionView O on O.optionRootID=ort.optionRootID  
			join TradeView TV on TV.contractID=O.optionID
	end
	else
	begin
		select distinct
			ort.underlyingID as iUnderlyingID,
			ort.optionRootID as iRootID,
			sup.underlyingID as iCompUndID, 
			sup.SyntheticCoeff as fWeight
		from
			SyntheticUnderlyingParamView as sup
			inner join SyntheticOptionRootView as sor
					on sor.syntheticOptRootID = sup.syntheticOptRootID
			inner join OptionRootView as ort
					on sor.optionRootID = ort.optionRootID
			join OptionView O on O.optionRootID=ort.optionRootID  
			join TradeView TV on TV.contractID=O.optionID
		where
			ort.underlyingID = @iUnderlyingID
	end
		
	set @error = @@error
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


CREATE     procedure dbo.usp_EodUndClosePrice_Complete_Save
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
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end					 

		set @vcExecString1 ='insert into ContractPrice (contractID, priceClose, actionDate) select contractID, priceClose,' + @vcDate
		set @vcExecString2 ='from ' + @vcTableName + ' EOD where not exists(select * from ContractPrice C where C.contractID = EOD.contractID and isnull(C.exchangeID, 0) = 0)'
			exec (@vcExecString1 + @vcExecString2)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end					 
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
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE     procedure dbo.usp_EodUndClosePrice_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcConstrName varchar(8000),
			@vcIndexName varchar(8000),
			@vcExecString1 varchar(8000)

	set @vcTableName = '##ETSEODUP' + + cast(db_name() as varchar(8000))
	set @vcConstrName = 'PK_ETSEODUP' + cast(db_name() as varchar(8000))
	set @vcIndexName = 'IND1_ETSEODUP' + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	--number of minutes of EOD timeout
	declare @dtEODTimeOut varchar(80)

	select @dtEODTimeOut = keyValue from DataInfo where dataInfoID = 7
	if @dtEODTimeOut is null
		begin
			Raiserror('Can''t define EOD operation timeout! You DB contains incorrect settings in DataInfo!',16,1)
			Return(-1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			set @vcExecString1 = 'if exists(select * from ' + @vcTableName + ' where DATEDIFF(mi,actionDate,GetDate()) >' + @dtEODTimeOut + ') drop table ' + @vcTableName 
			exec(@vcExecString1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
			Return (-1) 
		end

	exec('create table ' + @vcTableName + '(
		rowID int identity(1,1),
		contractID int not null,
		priceClose float not null,
		actionDate smalldatetime default GetDate())')

	exec('ALTER TABLE ' + @vcTableName + ' ADD CONSTRAINT ' + @vcConstrName + ' PRIMARY KEY CLUSTERED (rowID)')

	exec('CREATE NONCLUSTERED INDEX ' + @vcIndexName + ' ON ' + @vcTableName + ' (contractID)')

	set @error = @@error
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE         procedure dbo.usp_EodUndClosePrice_Save
	@iContractID int,
	@fPriceClose float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000),
			@vcDate varchar(8000)

	set @vcTableName = '##ETSEODUP' + + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	set @vcDate = '''' + convert(varchar(12),getdate(),112) + '''' 

	set @vcExecString1 = 'insert into ' + @vcTableName + '(contractID, priceClose) values(' + cast(@iContractID as varchar(10)) + ',' + cast(@fPriceClose as varchar(500)) + ')'

	if object_id(@vcFullTableName) is not null
		begin

			exec(@vcExecString1)
				if @@error<>0 begin set @error = @@error Return(@error) end 
		end

	set @vcExecString1 = 'if (select count(*) from ' + @vcTableName + '(nolock)) > 1999' + 
		' begin update ContractPrice set priceClose=EOD.priceClose,actionDate=' + @vcDate + 
		' from ContractPrice CP join ' + @vcTableName + ' EOD on CP.contractID = EOD.contractID and isnull(CP.exchangeID, 0) = 0'
	set @vcExecString2 =' insert into ContractPrice (contractID, priceClose, actionDate) select contractID, priceClose,' + @vcDate +
		' from ' + @vcTableName + ' EOD where not exists(select * from ContractPrice C where C.contractID = EOD.contractID and isnull(C.exchangeID, 0) = 0)' +
		' truncate table ' + @vcTableName + ' end'

	if object_id(@vcFullTableName) is not null
		begin
			exec (@vcExecString1 + @vcExecString2)
		end

finish:
    set @error = @@error
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


CREATE      PROCEDURE dbo.usp_EodUnderlying_Get
	@iUnderlyingID int = null
AS
/*
	Created by: Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iUnderlyingID is null
	begin
		select
		    c.contractID as iUnderlyingID,
		    c.contractTypeID as iUnderlyingTypeID,
		    c.Symbol as vcSymbol,
			c.MSupdateDate as dtLastMsUpdate,
			cp.priceClose as fPriceClose,
			s.isActive as tiIsActive,
			s.calcOptionType as tiCalcOptionType,
			s.isHTB as tiIsHTB,
		
		    null as fYield,
			null as tiIsBasket,
		    s.divFreq as iDivFreq,
		    s.divDate as dtDivDate,
		    s.divAmt as fDivAmt,
		
		    s.isDivCustom as tiIsDivCustom,
		    s.divFreqCustom as iDivFreqCustom,
		    s.divDateCustom as dtDivDateCustom,
		    s.divAmtCustom as fDivAmtCustom,
			
			s.skew as fSkew,
			s.kurt as fKurt
			
		from ContractView c 
		    inner join StockView s on c.contractID = s.stockID
			inner join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
		where 
			c.contractTypeID = 2
		union
		select
		    c.contractID as iUnderlyingID,
		    c.contractTypeID as iUnderlyingTypeID,
		    c.Symbol as vcSymbol,
			c.MSupdateDate as dtLastMsUpdate,
			cp.priceClose as fPriceClose,
			i.isActive as tiIsActive,
			i.calcOptionType as tiCalcOptionType,
			i.isHTB as tiIsHTB,
		
		    i.yield as fYield,
			i.isBasket as tiIsBasket,
		    null as iDivFreq,
		    null as dtDivDate,
		    null as fDivAmt,
		
		    null as tiIsDivCustom,
		    null as iDivFreqCustom,
		    null as dtDivDateCustom,
		    null as fDivAmtCustom,
			
			i.skew as fSkew,
			i.kurt as fKurt
	
		from ContractView c 
		    inner join IndexView i on c.contractID = i.indexID
			inner join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
		where
			c.contractTypeID = 1
	end
	else
	begin
		select
		    c.contractID as iUnderlyingID,
		    c.contractTypeID as iUnderlyingTypeID,
		    c.Symbol as vcSymbol,
			c.MSupdateDate as dtLastMsUpdate,
			cp.priceClose as fPriceClose,
			s.isActive as tiIsActive,
			s.calcOptionType as tiCalcOptionType,
			s.isHTB as tiIsHTB,
		
		    null as fYield,
			null as tiIsBasket,
		    s.divFreq as iDivFreq,
		    s.divDate as dtDivDate,
		    s.divAmt as fDivAmt,
		
		    s.isDivCustom as tiIsDivCustom,
		    s.divFreqCustom as iDivFreqCustom,
		    s.divDateCustom as dtDivDateCustom,
		    s.divAmtCustom as fDivAmtCustom,
			
			s.skew as fSkew,
			s.kurt as fKurt
			
		from ContractView c 
		    inner join StockView s on c.contractID = s.stockID
			inner join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
		where
			c.contractTypeID = 2
			and c.contractID = @iUnderlyingID
		union
		select
		    c.contractID as iUnderlyingID,
		    c.contractTypeID as iUnderlyingTypeID,
		    c.Symbol as vcSymbol,
			c.MSupdateDate as dtLastMsUpdate,
			cp.priceClose as fPriceClose,
			i.isActive as tiIsActive,
			i.calcOptionType as tiCalcOptionType,
			i.isHTB as tiIsHTB,
		
		    i.yield as fYield,
			i.isBasket as tiIsBasket,
		    null as iDivFreq,
		    null as dtDivDate,
		    null as fDivAmt,
		
		    null as tiIsDivCustom,
		    null as iDivFreqCustom,
		    null as dtDivDateCustom,
		    null as fDivAmtCustom,
			
			i.skew as fSkew,
			i.kurt as fKurt
	
		from ContractView c 
		    inner join IndexView i on c.contractID = i.indexID
			inner join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
		where
			c.contractTypeID = 1
			and c.contractID = @iUnderlyingID
	end
		
	set @error = @@error
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

 
CREATE  PROCEDURE dbo.usp_EodUnderlyingAverageVolumes_Save 
	@iUndID int, 
	@iVolume10Day int = null,
	@iVolume5Expiration int = null
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is save information into ContractPrice table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction
		if not exists(select * from ContractPrice where contractID=@iUndID and isnull(exchangeID,0)=0)
			begin
				insert into ContractPrice (contractID,
						exchangeID, 
						volume10Day,
						volume5Expiration)
				  values (@iUndID,
						null,
						@iVolume10Day,
						@iVolume5Expiration)

				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin

				update ContractPrice set 
					volume10Day = isnull(@iVolume10Day, volume10Day),
					volume5Expiration = isnull(@iVolume5Expiration, volume5Expiration)
				where contractID = @iUndID and isnull(exchangeID,0)=0

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
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE     Procedure dbo.usp_EodUnderlyingIndexBeta_Save 
	@iUnderlyingID int,
	@iIndexID int,
	@fBeta float
as
/*
	Created by Mike Malyshkin
	Description: this procedure save data into IndexBeta table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	Begin Transaction
		if not exists(select * From IndexBetaView where indexID = @iIndexID and contractID = @iUnderlyingID) 
			begin
				insert into IndexBeta (indexID,contractID,beta) 
				values (@iIndexID,@iUnderlyingID,@fBeta)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update IndexBeta
					set beta = isnull(@fBeta, beta)
					where indexID = @iIndexID and contractID = @iUnderlyingID
	
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
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

 

CREATE  PROCEDURE dbo.usp_EodUnderlyingSkewKurt_Save 
	@iUndID int,
	@fSkew float = null,
	@fKurt float = null
AS
/*
	Created by Michael Malyshkin
	Description: this procedure is save information into Stock or Index table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iContractTypeID int

	select @iContractTypeID = ContractTypeID from ContractView 
		where contractTypeID < 3 and contractID=@iUndID

	Begin Transaction
		if @iContractTypeID = 1
			begin
				update [Index]
					set skew = isnull(@fSkew, skew),
						kurt = isnull(@fKurt, kurt)
				where indexID = @iUndID
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update Stock set 
					skew = isnull(@fSkew, skew),
					kurt = isnull(@fKurt, kurt)
				where stockID = @iUndID
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
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
 

CREATE  PROCEDURE dbo.usp_EodUndExpirationsWithEmptyIvSurface_Get
	@iUndID int = null
as
/*
	Created by Michael Malyshkin
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	if @iUndID is null
		select distinct 
				CV.contractID as iContractID,
				OV.expiryID as iExpiryID
			from OptionView OV
				join OptionRootView ORT on ORT.optionRootID = OV.optionRootID
				join ContractView CV on ORT.underlyingID = CV.contractID
				left join VolaSurfaceDataView VSD on ORT.underlyingID = VSD.contractID and VSD.isDefaultSurface = 1
				left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID and OV.expiryID = CSSP.expiryID
			where 
				CV.contractTypeID < 3
				and CSSP.volaSurfaceDataID is null
			order by CV.contractID,OV.expiryID
	else
		select distinct 
				CV.contractID as iContractID,
				OV.expiryID as iExpiryID
			from OptionView OV
				join OptionRootView ORT on ORT.optionRootID = OV.optionRootID
				join ContractView CV on ORT.underlyingID = CV.contractID
				left join VolaSurfaceDataView VSD on ORT.underlyingID = VSD.contractID and VSD.isDefaultSurface = 1
				left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
					and OV.expiryID = CSSP.expiryID
			where 
				CV.contractTypeID < 3
				and CV.contractID = @iUndID
				and CSSP.volaSurfaceDataID is null
			order by CV.contractID,OV.expiryID



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE   PROCEDURE dbo.usp_EodUndOptionWithPos_Get
	@iUnderlyingID int = null,
	@dtMinExpDate datetime = Null
AS
/*
	Created by: Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iUnderlyingID is null
	begin
		select distinct
			ort.underlyingID as iUnderlyingID,
			ort.optionRootID as iOptionRootID,
			c.contractID as iOptionID,
			c.symbol as vcSymbol,
			o.strike as fStrike, 
			ec.expiryDate as dtExpiry,
			o.isCall as tiIsCall,
		    cp.priceClose as fPriceClose
		from
			ContractView c  
			inner join OptionView o on o.optionID = c.contractID
			inner join OptionRootView ort on o.optionRootID = ort.optionRootID
			inner join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
			inner join TradeView tv on tv.contractID=o.optionID
			inner join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
		where
			c.contractTypeID = 3
			and ((ec.expiryDate >= @dtMinExpDate and @dtMinExpDate is not Null) or (@dtMinExpDate is Null))
	end
	else
	begin
		select distinct
			ort.underlyingID as iUnderlyingID,
			ort.optionRootID as iOptionRootID,
			c.contractID as iOptionID,
			c.symbol as vcSymbol,
			o.strike as fStrike, 
			ec.expiryDate as dtExpiry,
			o.isCall as tiIsCall,
		    cp.priceClose as fPriceClose
		from
			ContractView c  
			inner join OptionView o on o.optionID = c.contractID
			inner join OptionRootView ort on o.optionRootID = ort.optionRootID
			inner join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
			inner join TradeView tv on tv.contractID=o.optionID
			inner join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
		where
			c.contractTypeID = 3
			and ort.underlyingID = @iUnderlyingID
			and ((ec.expiryDate >= @dtMinExpDate and @dtMinExpDate is not Null) or (@dtMinExpDate is Null))
	end
		
	set @error = @@error
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


CREATE  PROCEDURE dbo.usp_EodUndWithOptionsPositions_Get
  as
	---------------
	set nocount on
	---------------

	declare @error int		set @error = -1

--=================================================================
-- select underlying with options positions
--=================================================================
	select distinct C.contractID as iUnderlyingID
		from ContractView C 
			join OptionRootView ORt on ORt.underlyingID = C.contractID
			join OptionView O on O.optionRootID=ORt.optionRootID  
			join TradeView TV on TV.contractID=O.optionID
	
set @error = @@error
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


CREATE    PROCEDURE dbo.usp_EodUndWithPositions_Get
  as
	---------------
	set nocount on
	---------------

	declare @error int		set @error = -1

--=================================================================
-- select underlying with positions
--=================================================================
	select 
		S.stockID as iUnderlyingID,
		sum(T.quantity * T.price * (case when T.isBuy <> 0 then 1 else -1 end)) as fPosition
		from StockView S
		inner join TradeView T on T.contractID=S.stockID
		group by S.stockID
	union 
	select 
		I.indexID as iUnderlyingID,
		sum(T.quantity * T.price * (case when T.isBuy <> 0 then 1 else -1 end)) as fPosition
		from IndexView I
		inner join TradeView T on T.contractID=I.indexID
		group by I.indexID
	
set @error = @@error
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


CREATE   proc dbo.usp_EodVolaBands_Complete_Save
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
 		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
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
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

  

CREATE  proc dbo.usp_EodVolaBands_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcConstrName varchar(8000),
			@vcIndexName varchar(8000)


	set @vcTableName = '##ETSEODSRVB' + cast(db_name() as varchar(8000))
	set @vcConstrName = 'PK_VolaBand' + cast(db_name() as varchar(8000))
	set @vcIndexName = 'VolaBand_contractID_ind' + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName

	--number of minutes of EOD timeout
	declare @dtEODTimeOut varchar(80)

	select @dtEODTimeOut = keyValue from DataInfo where dataInfoID = 7
	if @dtEODTimeOut is null
		begin
			Raiserror('Can''t define EOD operation timeout! You DB contains incorrect settings in DataInfo!',16,1)
			Return(-1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			set @vcExecString1 = 'if exists(select * from ' + @vcTableName + ' where DATEDIFF(mi,actionDate,GetDate()) >' + @dtEODTimeOut + ') drop table ' + @vcTableName 
			exec(@vcExecString1)
		end

	if object_id(@vcFullTableName) is not null 
		begin 
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of Vola Bands EOD.', 16, 1)
			Return (-1) 
		end

	exec('create table ' + @vcTableName + '(
		rowID int identity(1,1),
		contractID int not null,
		expPeriod int not null,
		strikeMoneyness float not null,
		IVBandLow float not null,
		IVBandHi float not null,
		actionDate smalldatetime default GetDate())')

	exec('ALTER TABLE ' + @vcTableName + ' ADD CONSTRAINT ' + @vcConstrName + ' PRIMARY KEY CLUSTERED (rowID)')

	exec('CREATE NONCLUSTERED INDEX ' + @vcIndexName + ' ON ' + @vcTableName + ' ([contractID])')

	set @error = @@error
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

CREATE   proc dbo.usp_EodVolaBands_Save
	@iUndID int,
	@iExpPeriod int,
	@fStrikeMoneyness float,
	@fIVBandLow float,
	@fIVBandHi float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1

	declare @vcFullTableName varchar(8000),
			@vcTableName varchar(8000),
			@vcExecString1 varchar(8000),
			@vcExecString2 varchar(8000)

	set @vcTableName = '##ETSEODSRVB' + cast(db_name() as varchar(8000))
	set @vcFullTableName ='tempdb..' + @vcTableName


	set @vcExecString1 = 'insert into ' + @vcTableName + ' (contractID,expPeriod,strikeMoneyness,IVBandLow,IVBandHi)'
							+ 'values (' + cast(@iUndID as varchar(50)) + ',' + cast(@iExpPeriod as varchar(50)) + ',' +  cast(@fStrikeMoneyness as varchar(50)) + ',' +  cast(@fIVBandLow as varchar(50)) + ',' + cast(@fIVBandHi as varchar(50)) + ')'

	exec(@vcExecString1)
		if @@error <> 0 begin set @error = 1 if @@TranCount > 0 Rollback Transaction goto finish end

	set @vcExecString1 = 'if (select count(*) from ' + @vcTableName + '(nolock)) > 1999' + 
			' begin delete VolaBands where contractID in (select distinct contractID from ' + @vcTableName + ')' 
	set @vcExecString2 = 'insert into VolaBands (contractID,expPeriod,strikeMoneyness,IVBandLow,IVBandHi)' + 
			' select contractID,ExpPeriod,StrikeMoneyness,IVBandLow,IVBandHi from ' + @vcTableName +
			' truncate table ' + @vcTableName + ' end'
	
	exec (@vcExecString1 + @vcExecString2)

finish:
    set @error = @@error
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



CREATE       Procedure dbo.usp_EodWeightedVega_Save
	@dtExpiryDate smalldatetime,
	@fVegaWeight float
as
/*
	Changed by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	update ExpiryInCalendar
	   	set vegaWeight = @fVegaWeight
	where datediff(d, expiryDate, @dtExpiryDate) = 0

    set @error = @@error
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


CREATE    PROCEDURE dbo.usp_Option_Check 
	@iOptionID int = null, 
	@vcSymbol varchar(20),
	@iOptionRootID int,
	@dtExpiry smalldatetime,
	@iExpiryID int,
	@fStrike float,
	@iIsCall tinyint,
	@vcUnderlyingSymbol varchar(8),
	@ERR_MSG varchar(8000) = null output,
	@bRemoveOption bit = null   -- call usp_Option_Del for this option
AS
/*
	Created by: Zaur
	Description: this procedure created with template, desined by Zaur Nuraliev
		and check if option with same symbol exists
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iOptionID_bad int,
			@vcUnderlying_bad varchar(8),
			@vcOptionRoot_bad varchar(20),
			@dtExpiry_bad smalldatetime,
			@fStrike_bad float,
			@iIsCall_bad tinyint,
			@vcSymbol_bad varchar(20)
	
	select @iOptionID_bad = optionID 
		from OptionView O join Contract C on O.optionID = C.contractID
		where (optionID != @iOptionID or @iOptionID is null) and C.symbol = @vcSymbol and
		(optionRootID != @iOptionRootID or expiryID != @iExpiryID or abs(strike - @fStrike) > 0.009 or isCall != @iIsCall)


	if @iOptionID_bad is not null
	begin
		if @bRemoveOption is not null
			begin
				exec @error = usp_Option_Del @iOptionID = @iOptionID_bad,@bisRaiseError = 0
			end
		
		if (@error = -2) or (@bRemoveOption is null)
			begin
				select @vcSymbol_bad = symbol from ContractView where contractID = @iOptionID_bad
				select	@vcOptionRoot_bad = ORT.symbol
						from OptionView O join OptionRootView ORT on ORT.optionRootID = O.optionRootID
						where O.optionID = @iOptionID_bad
				select	@vcUnderlying_bad = C.symbol
						from ContractView C 
						join OptionRootView ORT on ORT.underlyingID = C.contractID
						join OptionView O on ORT.optionRootID = O.optionRootID
						where O.optionID = @iOptionID_bad
				select @dtExpiry_bad = expiryPeriod 
						from ExpiryView E join OptionView O on E.expiryID = O.expiryID 
						where O.optionID = @iOptionID_bad
				select 	@fStrike_bad = strike,
						@iIsCall_bad = isCall 
						from OptionView where optionID = @iOptionID_bad
				

			set @ERR_MSG = 'Can''t add option with Symbol=' + '''' +  @vcSymbol + ''''  +  
			CASE WHEN @vcUnderlying_bad != @vcUnderlyingSymbol THEN
				', Underlying=' + '''' + @vcUnderlyingSymbol + ''''
			ELSE '' END + 
			CASE WHEN left(@vcSymbol,len(@vcSymbol)-2) != left(@vcSymbol_bad,len(@vcSymbol_bad)-2) THEN
				', Option Root=' + '''' + left(@vcSymbol,len(@vcSymbol)-2) + ''''
			ELSE '' END + 
			CASE WHEN @fStrike!=@fStrike_bad THEN
				', Strike=' + CAST(ROUND(@fStrike, 2) as varchar(10)) 
			ELSE '' END + 
			CASE WHEN cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) != cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) THEN
				', Expiry=' + '''' + cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) + '''' 
			ELSE '' END +  
			CASE WHEN @iIsCall!=@iIsCall_bad THEN
				'''' + ', Call\Put' + '''' + '=' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END  
			ELSE '' END +  
			CASE WHEN @error = -2 THEN
				' because exists option with trades and Symbol=' + '''' +  @vcSymbol_bad + ''''
				WHEN @error != -2 THEN
				' because exists option with Symbol=' + '''' +  @vcSymbol_bad + ''''
			ELSE '' END + 
			CASE WHEN @vcUnderlying_bad != @vcUnderlyingSymbol THEN
				', Underlying=' + '''' + @vcUnderlying_bad + ''''
			ELSE '' END + 
			CASE WHEN left(@vcSymbol,len(@vcSymbol)-2) != left(@vcSymbol_bad,len(@vcSymbol_bad)-2) THEN
				', Option Root=' + '''' + left(@vcSymbol_bad,len(@vcSymbol_bad)-2) + ''''
			ELSE '' END + 
			CASE WHEN @fStrike!=@fStrike_bad THEN
				', Strike=' + CAST(ROUND(@fStrike_bad, 2) as varchar(10)) 
			ELSE '' END + 
			CASE WHEN cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) != cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) THEN
				', Expiry=' + '''' + cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) + ''''
			ELSE '' END + 
			CASE WHEN @iIsCall!=@iIsCall_bad THEN
				'''' + ', Call\Put' + '''' + '=' + CASE @iIsCall_bad WHEN 1 THEN 'Call' ELSE 'Put' END
			ELSE '' END +
			CASE WHEN @vcUnderlying_bad = @vcUnderlyingSymbol THEN
			', Underlying=' + '''' + @vcUnderlyingSymbol + ''''
			ELSE '' END
		end	
		Return(-1)
	end
	else
		begin
			set @ERR_MSG = null
			set @iOptionID_bad = null
		end

	--===================================================================
	-- update option symbol, if option with same attributes exists
	--===================================================================
	select @iOptionID_bad = optionID 
		from OptionView O join ContractView C on O.optionID = C.contractID
		where (optionID != @iOptionID) and C.symbol != @vcSymbol and
		(optionRootID = @iOptionRootID and expiryID = @iExpiryID and abs(strike - @fStrike) < 0.009 and isCall = @iIsCall)
	if @iOptionID_bad is not null
	begin
		update contract set symbol = @vcSymbol where contractID = @iOptionID_bad
		set @ERR_MSG = '*'
	end


Return(@@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE PROCEDURE dbo.usp_EodUnderlying_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime
as
/*
	Created by Michael Malyshkin
*/
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
				if @@TranCount = 1 Rollback Transaction goto finish 
			end
	
		delete from TradeLog
		where contractID = @iContractID
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t delete record from TradeLog!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

		--========================================================================
		-- make record with null exchange for this stock
		--========================================================================
		if not exists(select * from ContractPrice where exchangeID is null and contractID = @iContractID)
			begin
				insert into ContractPrice(contractID,exchangeID)
					values(@iContractID,null)
				if @@error <> 0 
					begin 
						Raiserror ('Error. Can''t insert default exchange!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish
					end
			end 
		--==================================================================================
		-- update DivAmnt & DivFreq values
		--==================================================================================
		if (@bIsStock <> 0 and exists(select * from StockView where stockID=@iContractID))
			begin 
				Declare @df smallint
				update Stock 
					set divAmt = @fDivAmt,
						divDate = @dtDivDate, 
						divFreq = @iDivFreq
				where stockID = @iContractID
				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update Dividends!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
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
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE	PROCEDURE dbo.usp_EodOption_Import
	@iUnderlyingContractID int,
	@vcSymbol varchar(20),
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
	@iLotSize int,
	@iMsCacheOptionID int = null
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
			@iExpiryID int,
			@iLastExpiryID int,
			@vcUnderlyingSymbol varchar(8),
			@ERR_MSG varchar(8000)

	select @vcUnderlyingSymbol = symbol from ContractView where contractID = @iUnderlyingContractID
	--===================================================================			
	-- check the Expiry
	--===================================================================			
	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	if not exists(select * from Expiry where expiryID = @iExpiryID)
		begin
			set @ERR_MSG = 'Incorrect expiration! '  + '''' +  Convert(varchar(50),@dtExpiry) + '''' + ' for symbol=' + '''' + @vcSymbol + '''' +  ' UnderlyingSymbol=' + '''' + @vcUnderlyingSymbol + '''' + ' Strike=' + Convert(varchar(15),@fStrike) + ' expiryID=' + Convert(varchar(15),@iExpiryID)
			Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end

	--===================================================================			
	-- check symbol of option root
	--===================================================================			
   if exists (select optionRootID 
				   from OptionRootView
				   where symbol = left(@vcSymbol,len(@vcSymbol)-2)and 
					underlyingID<>@iUnderlyingContractID)
		begin			
			Declare @vcDifferentUndSymbol varchar(8)
			select @vcDifferentUndSymbol = C.symbol from ContractView C join
					OptionRootView ort on ort.underlyingID = C.contractID
					where ort.symbol = left(@vcSymbol,len(@vcSymbol)-2) and
					ort.underlyingID<>@iUnderlyingContractID
					
			set @ERR_MSG = 'Incorrect option symbol! '  + '''' +  @vcSymbol + ''''  +  ': Option Root=' + '''' + left(@vcSymbol,len(@vcSymbol)-2) + '''' + ' assign to different underlying=' + '''' +  @vcDifferentUndSymbol + '''' + '. Please, rename Option Root and restart importing of market structure.'
			Raiserror(@ERR_MSG,16,1)
			RETURN (-1)
		end  


	select top 1 @iLastExpiryID = expiryID from ExpiryInCalendarView where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	--===================================================================			
	-- select the @iOptionRootID
	--===================================================================			
	declare @iOptionRootID int,
			@vcOptionRoot varchar(20)
	select @iOptionRootID = optionRootID from OptionRootView where symbol = left(@vcSymbol,len(@vcSymbol)-2)
			and underlyingID = @iUnderlyingContractID 

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if @iOptionRootID is not null 
		select @OptionID = optionID
			from [Option] (nolock)
			where optionRootID = @iOptionRootID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	--===================================================================			
	-- check if option with same symbol exists
	--===================================================================		
	set @ERR_MSG = null

	execute usp_Option_Check @iOptionID = @OptionID, @vcSymbol = @vcSymbol,	@iOptionRootID = @iOptionRootID,
			@dtExpiry = @dtExpiry,@iExpiryID = @iExpiryID,@fStrike = @fStrike,@iIsCall = @iIsCall,
			@vcUnderlyingSymbol = @vcUnderlyingSymbol,@bRemoveOption = 1,@ERR_MSG = @ERR_MSG output

	if @ERR_MSG is not null and @ERR_MSG != ''
	begin
		if @ERR_MSG != '*' Raiserror(@ERR_MSG,16,1)
		Return(-1)
	end

	--===================================================================			
	-- make the contract name
	--===================================================================			
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(month, @dtExpiry) + 
					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)


  	if (@OptionID is Null) 
	begin
		Begin Transaction

			if  @iOptionRootID is null
			begin
				insert into OptionRoot (underlyingID,symbol,lotSize)
					select @iUnderlyingContractID,left(@vcSymbol,len(@vcSymbol)-2),isnull(@iLotSize,100)
				set @iOptionRootID = @@identity
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end 
	
			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@cn)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				 values (@OptionID, @iOptionRootID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@OptionID,null)
					if (@@error <> 0) begin rollback tran return(-1) end           
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

		delete from TradeLog
		where contractID = @OptionID

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
			where optionID = @iMsCacheOptionID
		end

		if @iOptionRootID is not null
		begin
		  update OptionRoot
				set
					lotSize = isnull(@iLotSize,lotSize)
				where optionRootID = @iOptionRootID
		end

		update [Option]
			set 
			IsCall = isnull(@iIsCall,IsCall),
			strike = isnull(@fStrike,Strike),
			expiryID = isnull(@iExpiryID,expiryID)
		where optionID = @OptionID

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


CREATE	PROCEDURE dbo.usp_EodMsUnderlyingCache_Check
	@iContractID int
as
/*
	Created by Michael Malyshkin
	Checked if no options of the underlying exist in MsOptionCache then delete underlying record
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if not exists(select * from MsOptionCache O where O.underlyingID = @iContractID)
		begin
			delete from MsUnderlyingCache where underlyingID=@iContractID
			
			if @@error <> 0 set @error = @@error
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


CREATE   PROCEDURE dbo.usp_EodMsLoad_Complete
as
/*
	Created by Michael Malyshkin
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------

	select distinct symbol as vcUnderlyingSymbol from ContractView C join MsUnderlyingCache MUC on C.contractID=MUC.underlyingID

	return @@error


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE  PROCEDURE dbo.usp_Trade_ClearDeleted 
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	Begin Transaction Trade

		delete from TradeLog 
		where seqNum in (select seqNum
						from Trade 
						where tradeID in (select tradeID from Trade where actionID = 0))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

		delete from Trade where actionID = 0 
			if @@error !=0 begin set @error = 2 if @@TranCount = 1 Rollback Tran goto finish end

		set @error = @@error

		delete from TradeLog 
		where seqNum in (select seqNum
						from Trade T join [Option] O on O.optionID = T.contractID
						join OptionRoot ORt on O.optionRootID = ORt.optionRootID
						join Contract UC on ORt.underlyingID = UC.contractID 
						join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
						where E.expiryDate < GetDate() )
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

		delete Trade from Trade T join [Option] O on O.optionID = T.contractID
						join OptionRoot ORt on O.optionRootID = ORt.optionRootID
						join Contract UC on ORt.underlyingID = UC.contractID 
						join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
						where E.expiryDate < GetDate() 
		if @@error !=0 begin set @error = 2 if @@TranCount = 1 Rollback Tran goto finish end

		set @error = @@error

	Commit Transaction Trade

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



CREATE    PROCEDURE dbo.usp_EodMsUnderlyingCache_Get
	@iUndID int = null
AS
/*
	Created by Michael Malyshkin
	Description: this procedure is get information from MsUnderlyingCache table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if isnull(@iUndID, 0) = 0
	begin
		select
			MS.underlyingID as iUnderlyingID,
			C.symbol as vcUndSymbol,
			MS.updateDate as dtUpdateDate
		from MsUnderlyingCache MS join ContractView C on C.contractID = MS.underlyingID
	end
	else
	begin
		select
			MS.underlyingID as iUnderlyingID,
			C.symbol as vcUndSymbol,
			MS.updateDate as dtUpdateDate
		from MsUnderlyingCache MS join ContractView C on C.contractID = MS.underlyingID
		where MS.underlyingID = @iUndID
	end
	
Return(@@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    PROCEDURE dbo.usp_EodMsOptionCache_Get
	@iUndID int
AS
/*
	Created by Michael Malyshkin
	Description: this procedure is get information from MsOptionCache table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	select
		O.optionID as iOptionID,
		O.symbol as vcSymbol,
		O.expiryDate as dtExpiryDate,
		O.strike as fStrike,
		O.isCall as tiIsCall,
		O.lotSize as iLotSize
		from MsOptionCache O where O.underlyingID = @iUndID
	
Return(@@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.43.083')
	begin
		update DataInfo set keyValue = '3.43.083' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.43.083' '3.43.083 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
