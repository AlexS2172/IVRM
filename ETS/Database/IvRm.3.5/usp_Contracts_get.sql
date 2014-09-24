	-------------------------------------------
	-- return SpotOption contracts with trades
	-------------------------------------------
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
	inner join 
		TradeView tv on tv.contractID = cv.contractID
	where
		cv.ContractTypeID = 3
	order by
		orv.underlyingID

	-------------------------------------------
	-- return Future contracts with trades   --
	-------------------------------------------
	select
		-- asbstract contract property
		fc.contractID 		as iContractID,
		fc.contractTypeID 	as iContractType,
		fc.symbol 		as vcSymbol,
		fc.contractName 	as vcContractName,
	   	fcp.priceClose 		as fClosePrice,
		fcp.priceTheo 		as fTheoClosePrice,
		mp.manualprice 		as fManualPrice,
		
		fc.exportSymbol 	as vcExportSymbol,

		fc.undPriceProfileID 	as iUndPriceProfileID,
		fc.optPriceProfileID 	as iOptPriceProfileID,

		f.futureRootID 		as iRootID,
		uc.contractID 		as iAssetID,

		f.maturityDate 		as dtMaturityDate,
		f.calcOptionType 	as iOptionStyle, --is american
		f.isActive 		as iIsActive,
		case 
			when exists(select top 1 1 from FutureOption where futureID = fc.contractID) then 1 else 0 end as iHaveOptions,

		isnull(null, 0) 	as iShowInFutureContractEq,
		isnull(f.dBasis, 0) 			as fBasis,
		isnull(f.dActiveFutureRatio, 1)		as fActiveFutureRatio,
		isnull(f.bUseInCalc, 0) 		as iIsActiveFuture,
			
		fr.iQuotationUnitAmmount	as iQuotationUnitAmount,
		fr.tiQuoteFormat		as iQuoteFormat,
		fr.fTickSize			as fTickSize,
		fr.fTickValue			as fTickValue

	from Future f
		inner join 
			ContractView fc on fc.contractID = f.futureID
		inner join 
			FutureRoot fr on fr.futureRootID = f.futureRootID
		inner join 
			ContractView uc on uc.contractID = fr.underlyingID
		inner join 
			ContractPrice fcp on fc.contractID = fcp.contractID and fcp.exchangeID is null
		left join 
			ManualPrice mp on mp.contractID = f.futureID
		inner join
			TradeView tv on tv.contractID = f.futureID

	---------------------------------------------------
	---- return future option contracts with trades ---
	---------------------------------------------------
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
	inner join 
		TradeView tv on tv.contractID = cv.contractID
	where
		cv.ContractTypeID = 5
