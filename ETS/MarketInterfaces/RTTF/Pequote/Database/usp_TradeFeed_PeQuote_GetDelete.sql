----------------------------------------------------------------------------------------------------
-- usp_TradeFeed_PeQuote_GetDelete
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_PeQuote_GetDelete]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeFeed_PeQuote_GetDelete]
GO

CREATE PROC usp_TradeFeed_PeQuote_GetDelete
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction

	select * from Last_PeQuote_SnapShot where (execID not in (
		select execID from Current_PeQuote_SnapShot)) and
		Convert(varchar(8),trade_date,112) = Convert(varchar(8),current_timestamp,112)
	if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

	Commit Transaction

	set @error = 0
	return @error

finish:
	return @@error
GO