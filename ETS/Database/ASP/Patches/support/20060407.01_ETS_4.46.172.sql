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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.171'  or (keyValue = '3.46.172') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.172 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.172' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER   Procedure dbo.usp_CustomStrikeSkewDefaultPointFromIV_Save 
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
		
	SELECT @expInCalID = eic.expiryInCalendarID
	FROM ExpiryInCalendar eic
	JOIN Contract c ON c.contractID = @iContractID
	WHERE eic.expiryID = @iExpiryID AND  eic.expCalendarID = c.expCalendarID		

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

ALTER    PROC dbo.usp_BoCustomStrikeSkewPoint_Default_Save 
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
	SELECT  @expInCalID = EIC.expiryInCalendarID
	FROM volaSurfaceData vsd
	JOIN Contract c ON c.contractID = vsd.contractID
	JOIN ExpiryInCalendar EIC on EIC.expiryID = @iExpiryID AND EIC.expCalendarID = c.expCalendarID
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.172')
	begin
		update DataInfo set keyValue = '3.46.172' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.172' '3.46.172 update message:' 
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