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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.147'  or (keyValue = '3.46.148') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.148 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.148' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='PriceProfile' and COLUMN_NAME='ZeroBidRuleForPnL')
begin
	ALTER TABLE PriceProfile ADD ZeroBidRuleForPnL tinyint NULL
end
GO

ALTER  VIEW dbo.PriceProfileView
AS
	select
		priceProfileID,
		priceProfileName,
		priceProfileDesc,
		defaultID,
		isOptionProfile,
		alwaysLast,
		midmarketPnL,
		singlePriceRule,
		bothPriceRule,
		ZeroBidRuleForPnL
	from
		PriceProfile
	where
		actionID < 3


GO


ALTER  PROCEDURE dbo.usp_PriceProfile_Save 
	@iPriceProfileID int = null, 
	@vcPriceProfileName varchar(50) = null,
	@vcPriceProfileDesc varchar(255) = null,
	@tiIsOptionProfile tinyint = null,
	@tiAlwaysLast tinyint = null,
	@tiMidmarketPnL tinyint = null,
	@tiSinglePriceRule tinyint = null,
	@tiBothPriceRule tinyint = null,
	@tiZeroBidRuleForPnL tinyint = null
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if @iPriceProfileID is null and @vcPriceProfileName is null
	begin
		raiserror ('Name for new price profile can not be empty!', 16, 1)
		set @error = -1
		goto finish 
	end

	if exists (select * from PriceProfile
				where priceProfileName = @vcPriceProfileName
					and isOptionProfile = isnull(@tiIsOptionProfile, 0) and actionID < 3
					and (priceProfileID <> @iPriceProfileID or @iPriceProfileID is null))
		begin
			if isnull(@tiIsOptionProfile, 0) = 0
				raiserror ('Underlying price profile with the same name already exists!', 16, 1)
			else
				Raiserror ('Option price profile with the same name already exists!', 16, 1)
			set @error = -2
			goto finish 
		end

	Begin Transaction PriceProfile

	if @iPriceProfileID is null
		begin
			insert into PriceProfile 
				(priceProfileName, priceProfileDesc, defaultID, isOptionProfile,
				alwaysLast, midmarketPnL, singlePriceRule, bothPriceRule, ZeroBidRuleForPnL)
			values
				(@vcPriceProfileName, @vcPriceProfileDesc, 0, isnull(@tiIsOptionProfile, 0),
				isnull(@tiAlwaysLast, 0), isnull(@tiMidmarketPnL, 0), isnull(@tiSinglePriceRule, 0), isnull(@tiBothPriceRule, 0), isnull(@tiZeroBidRuleForPnL, 0))

			if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction PriceProfile goto finish end

			set @iPriceProfileID = @@identity
		end
	else
		begin
			update PriceProfile set 
				priceProfileName = isnull(@vcPriceProfileName, priceProfileName),
				priceProfileDesc = isnull(@vcPriceProfileDesc, priceProfileDesc),
				isOptionProfile = isnull(@tiIsOptionProfile, isOptionProfile),
				alwaysLast = isnull(@tiAlwaysLast, alwaysLast),
				midmarketPnL = isnull(@tiMidmarketPnL, midmarketPnL),
				singlePriceRule = isnull(@tiSinglePriceRule, singlePriceRule),
				bothPriceRule = isnull(@tiBothPriceRule, bothPriceRule),
				ZeroBidRuleForPnL = isnull(@tiZeroBidRuleForPnL, ZeroBidRuleForPnL)
			where
				priceProfileID = @iPriceProfileID

   			if @@error <> 0 begin set @error = -4 if @@TranCount > 0  Rollback Transaction PriceProfile goto finish end
		end

	Commit Transaction PriceProfile

finish:
	if @error = 0
		return @iPriceProfileID
	else
		return @error


GO


ALTER  PROCEDURE dbo.usp_PriceProfile_Get 
	@iPriceProfileID int = null
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if @iPriceProfileID is null	
		select
			priceProfileID as iPriceProfileID,
			priceProfileName as vcPriceProfileName,
			priceProfileDesc as vcPriceProfileDesc,
			defaultID as tiDefaultID,
			isOptionProfile as tiIsOptionProfile,
			alwaysLast as tiAlwaysLast,
			midmarketPnL as tiMidmarketPnL,
			singlePriceRule as tiSinglePriceRule,
			bothPriceRule as tiBothPriceRule,
			ZeroBidRuleForPnL as tiZeroBidRuleForPnL
		from
			PriceProfileView
	else
		select
			priceProfileID as iPriceProfileID,
			priceProfileName as vcPriceProfileName,
			priceProfileDesc as vcPriceProfileDesc,
			defaultID as tiDefaultID,
			isOptionProfile as tiIsOptionProfile,
			alwaysLast as tiAlwaysLast,
			midmarketPnL as tiMidmarketPnL,
			singlePriceRule as tiSinglePriceRule,
			bothPriceRule as tiBothPriceRule,
			ZeroBidRuleForPnL as tiZeroBidRuleForPnL
		from
			PriceProfileView
		where
			priceProfileID = @iPriceProfileID

	set @error = @@error

	return @error

GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.148')
	begin
		update DataInfo set keyValue = '3.46.148' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.148' '3.46.148 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

