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
BEGIN TRANSACTION
GO
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.122' or (keyValue = '3.46.123')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.123 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.123' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

----------------------------------------------------------------------------------------------------
-- usp_BoCustomStrikeSkewPoint_ByUnderlying_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoCustomStrikeSkewPoint_ByUnderlying_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoCustomStrikeSkewPoint_ByUnderlying_Del]
GO

CREATE PROC dbo.usp_BoCustomStrikeSkewPoint_ByUnderlying_Del
	@iUnderlyingID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	begin tran

	delete CustomStrikeSkewPoint
	from CustomStrikeSkewPoint cssp
		inner join VolaSurfaceData vsd on cssp.volaSurfaceDataID = vsd.volaSurfaceDataID
			and vsd.contractID = @iUnderlyingID
	if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

	update VolaSurfaceData
	set baseUnderlinePrice = cp.priceClose
	from VolaSurfaceData vsd
		inner join ContractPrice cp on cp.contractID = vsd.contractID and exchangeID is null 
	where
		vsd.contractID = @iUnderlyingID
	if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

	commit tran
finish:
	return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_BoCustomStrikeSkewPoint_Default_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoCustomStrikeSkewPoint_Default_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoCustomStrikeSkewPoint_Default_Save]
GO

CREATE PROC dbo.usp_BoCustomStrikeSkewPoint_Default_Save 
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
	
				if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

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
	
	   			if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction

finish:
	Return(@error)
GO

/* RESULTS ANALYZING */
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.123')
	begin
		update DataInfo set keyValue = '3.46.123' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.123' '3.46.123 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
