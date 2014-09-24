SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FutureOption_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FutureOption_Get]
GO

create  procedure dbo.usp_FutureOption_Get
   @iContractID int
As
	select
		cv.ContractID as iContractID,
		cv.Symbol as vcSymbol,
		cv.ContractName as vcContractName,
		cv.ContractName as vcSeries,
		cv.ExportSymbol as vcImportID,
		cv.ContractTypeID as iContractType,
		fop.IsCall as iOptionType,
		fop.Strike as fStrike,
		fop.ExpiryDate as dtExpiry,
		fop.ExpiryOV as dtExpiryOV,
		fop.TradingClose as dtTradingClose,
		null as iIsManualyEntered,
		mprice.ManualPrice as fManualPrice,
		cpv.PriceClose as fClosePrice,
		cpv.PriceTheo as fTheoClosePrice,
		0 as iIsFlex,
		fr.FutureRootID as iRootID,
		fop.futureID as iBaseContractID,
		fr.underlyingID as iAssetID
	from 
		ContractView cv
	inner join
		FutureOption fop on fop.futureOptionID = cv.ContractID
	left join
		ManualPrice mprice on mprice.contractID = cv.contractID
	inner join
		ContractPriceView cpv on cpv.contractID = cv.contractID
	inner join
		Future fut on fut.futureID = fop.futureID
	inner join
		FutureRoot fr on fr.futureRootID = fut.futureRootID
	where
		cv.ContractID = @iContractID
	
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

