SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_SpotOption_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_SpotOption_Get]
GO

create  procedure dbo.usp_SpotOption_Get
   @iContractID int
As
	select
		cv.ContractID as iContractID,
		cv.Symbol as vcSymbol,
		cv.ContractName as vcContractName,
		cv.ContractName as vcSeries,
		cv.ContractTypeID as iContractType,
		cv.ExportSymbol	as vcImportID,
		ov.IsCall as iOptionType,
		ov.Strike as fStrike,
		ov.ExpiryDate as dtExpiry,
		ov.ExpiryOV as dtExpiryOV,
		ov.TradingClose as dtTradingClose,
		ov.isManualyEntered as iIsManualyEntered,
		mprice.ManualPrice as fManualPrice,
		cpv.PriceClose as fClosePrice,
		cpv.PriceTheo as fTheoClosePrice,
		ov.IsManualyEntered as iIsFlex,
		orv.OptionRootID as iRootID,
		orv.underlyingID as iBaseContractID,
		orv.underlyingID as iAssetID,
		orv.isSynthetic as iIsSynthetic
	from 
		ContractView cv
	inner join
		OptionView ov on ov.OptionID = cv.ContractID
	left join
		ManualPrice mprice on mprice.contractid = cv.contractid
	inner join
		ContractPriceView cpv on cpv.contractid = cv.contractid
	inner join
		OptionRootView orv on orv.optionRootID = ov.OptionRootID
	where
		cv.ContractID = @iContractID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



