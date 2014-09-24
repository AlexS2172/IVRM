----------------------------------------------------------------------------------------------------
-- usp_TradeFeed_PeQuote_GetNew
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_PeQuote_GetNew]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeFeed_PeQuote_GetNew]
GO

CREATE PROC usp_TradeFeed_PeQuote_GetNew
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction

	select * from Current_PeQuote_SnapShot where (execID not in (
		select execID from Last_PeQuote_SnapShot))

	if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

	Commit Transaction

	set @error = 0
	return @error

finish:
	return @@error
GO