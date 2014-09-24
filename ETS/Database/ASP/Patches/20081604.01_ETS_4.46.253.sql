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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.46.252' or (keyValue = '4.46.253') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '4.46.253 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '4.46.253' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION SECTION*/


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER       PROC dbo.usp_BoCustomStrikeSkewPoint_Default_Save 
	@iContractID int,
	@fStrike float,
	@dtExpiry datetime,
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
			and abs(datediff(d, expiryDate, @dtExpiry)) = 0
			and actionID < 3


	declare @dtExpiryOV datetime
	set @dtExpiryOV = @dtExpiry
	
	if (isnull(@iCustomStrikeSkewPointID, -1) < 0)
	begin
		select  CE.expiryOV 
		from 	(
				select 
				distinct expiryOV, expiryDate 
				from OptionView 
				where OptionRootID in (
							select OptionRootID 
							from OptionRootView 
							where Underlyingid = @iContractID
						       )
			)CE
		where 
			abs(datediff(d, CE.expiryDate, @dtExpiry)) = 0
	end
  
	
	
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					isBasePoint,
					expiryDate,
					expiryOV)
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						-1,
						@dtExpiry,
						@dtExpiryOV)
	
				if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

			end
		else
			begin
				update CustomStrikeSkewPoint set 
					--volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					--type = isnull(@iType,type),
					--strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility)
					--expiryDate = isnull(@iExpiryID,expiryID )
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


/*DATA MODIFICATION COMPLETE*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.46.253')
	begin
		update DataInfo set keyValue = '4.46.253' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 ROLLBACK TRANSACTION
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
	select 'DB VERSION WAS CHANGED TO 4.46.253' '4.46.253 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


