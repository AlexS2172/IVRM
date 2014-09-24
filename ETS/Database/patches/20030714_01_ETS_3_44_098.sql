/*
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.44.097' or (keyValue = '3.44.098')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.44.098 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.44.098' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/


----------------------------------------------------------------------------------------------------
-- usp_TntOption_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_TntOption_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_TntOption_Get
GO

CREATE PROCEDURE dbo.usp_TntOption_Get
	@iOptRootID int,
	@tiIsCall tinyint,
	@fStrike float,
	@dtExpiry datetime
AS
/*
	Created by Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	declare @error int		set @error = 0

	--===================================================================			
	-- check option root id
	--===================================================================			

	if isnull(@iOptRootID, 0) = 0
	begin	
		set @error = -1
		goto finish
	end

	declare @iExpiryID int
	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	--===================================================================			
	-- check expiry
	--===================================================================			

	if not exists(select 1 from Expiry (nolock) where expiryID = @iExpiryID and actionID < 3)
	begin
		set @error = -2
		goto finish
	end

	set @fStrike = round(@fStrike, 3)

	declare @iOptionID int
	declare @vcOptionSymbol varchar(20)

	--===================================================================			
	-- find option
	--===================================================================			

	select
		@iOptionID = c.contractID,
		@vcOptionSymbol = c.symbol,
		@fStrike = o.strike
	from
		[Option] as o (nolock)
		left join Contract as c (nolock) on o.optionID = c.contractID and c.actionID < 3
	where
		o.optionRootID = @iOptRootID
		and o.isCall = @tiIsCall
		and abs(o.strike - @fStrike) < 0.0009
		and o.expiryID = @iExpiryID
		and o.actionID < 3
	
	--===================================================================			
	-- check existance
	--===================================================================			

	if @iOptionID is null
		set @error = -3

finish:
	if @error = 0
		select
			@error as iErrorCode,
			@iOptionID as iOptionID,
			@vcOptionSymbol as vcOptionSymbol,
			@iExpiryID as iExpiryID
	else
		select
			@error as iErrorCode,
			null as iOptionID,
			null as vcOptionSymbol,
			null as iExpiryID

	return @error

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.44.098')
	begin
		update DataInfo set keyValue = '3.44.098' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.44.098' '3.44.098 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
