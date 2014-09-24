SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AssetSpotOptions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AssetSpotOptions_Get]
GO

create    procedure dbo.usp_AssetSpotOptions_Get
   @iAssetID int = null,
   @iJoinTrades int = null
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
		ContractView av
	inner join
		OptionRootView orv on orv.underlyingID = av.contractID
	inner join
		OptionView ov on ov.OptionRootID = orv.OptionRootID
	inner join
		ContractPriceView cpv on cpv.contractid = ov.optionID
	inner join
		ContractView cv on cv.contractID = ov.optionID
	left join
		ManualPrice mprice on mprice.contractid = ov.optionID
	where
		av.ContractID = @iAssetID
	order by
		ov.OptionRootID, ov.ExpiryDate, ov.Strike

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

