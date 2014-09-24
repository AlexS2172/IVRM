SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Dividends_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Dividends_Get]
GO

create    proc dbo.usp_Dividends_Get 
	@iAssetID int = null
AS
	set nocount on
	-------------------
	-- return stream --
	-------------------
	if isnull(@iAssetID, 0) = 0
		select * from CustomDividend
		order by StockID, DivYtes, DivAmnt
	else
		select * from CustomDividend 
		where StockID = @iAssetID
		order by DivYtes, DivAmnt

	-------------------
	-- return market --
	-------------------
	
	select
		case when iv.indexID is null then sv.divFreq else iv.divFreq end as iDivFreq,
		case when iv.indexID is null then sv.divDate else iv.divDate end as dtDivDate,
		case when iv.indexID is null then sv.divAmt else iv.divAmt end as fDivAmt,
		case when iv.indexID is null then sv.divFreqCustom else iv.divFreqCustom end as iDivFreqCustom,
		case when iv.indexID is null then sv.divDateCustom else iv.divDateCustom end as dtDivDateCustom,
		case when iv.indexID is null then sv.divAmtCustom else iv.divAmtCustom end as fDivAmtCustom,
		case when iv.indexID is null then sv.isDivCustom else iv.isDivCustom end as tiIsDivCustom
	from ContractView cv
		left join StockView sv on cv.contractID = sv.StockID
		left join IndexView iv on cv.ContractID = iv.IndexID
	where cv.contractID = @iAssetID and cv.contractTypeID in (1, 2)

	return @@error


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

