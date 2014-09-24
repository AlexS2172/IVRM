----------------------------------------------------------------------------------------------------
-- usp_TradeFeed_PeQuote_GetUpdate
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_PeQuote_GetUpdate]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeFeed_PeQuote_GetUpdate]
GO

CREATE PROC usp_TradeFeed_PeQuote_GetUpdate
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction

	select css.* from Current_PeQuote_SnapShot as css inner join Last_PeQuote_SnapShot as lss on
		lss.ExecID = css.ExecID where lss.trader_acronym <> css.trader_acronym or 
		lss.isBuy <> css.isBuy or lss.quantity <> css.quantity or lss.price <> css.price or 
		Convert(varchar(8),lss.trade_date,112) <> Convert(varchar(8),css.trade_date,112) or 
		lss.contract_symbol <> css.contract_symbol or lss.contract_type <> css.contract_type or
		lss.isCall <> css.isCall or lss.strike <> css.strike or lss.expiry_date <> css.expiry_date or 
		lss.opt_root_symbol <> css.opt_root_symbol

	if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

	Commit Transaction

	set @error = 0
	return @error

finish:
	return @@error
GO