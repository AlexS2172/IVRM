----------------------------------------------------------------------------------------------------
-- usp_TradeFeed_PeQuote_Clear
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_PeQuote_Clear]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeFeed_PeQuote_Clear]
GO

CREATE PROC usp_TradeFeed_PeQuote_Clear
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction

	delete from Last_PeQuote_SnapShot
	if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

	insert into Last_PeQuote_SnapShot select * from Current_PeQuote_SnapShot
	if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction goto finish end

	delete from Current_PeQuote_SnapShot
	if @@error <> 0 begin set @error = 4 if @@TranCount > 0 Rollback Transaction goto finish end

	Commit Transaction

	set @error = 0
	return @error

finish:
	return @@error
GO