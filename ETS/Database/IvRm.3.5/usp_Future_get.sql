SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Future_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Future_Get]
GO

create  procedure dbo.usp_Future_Get
   @iContractID int
As
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
		where 
			f.futureID = @iContractID
		
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

