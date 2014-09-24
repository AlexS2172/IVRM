-- fix for MLine00000404 (Indexes do not handled in EOD procedure (db issue))
set nocount on 
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
ALTER        proc dbo.usp_allStockEOD_Get
	@iFormat int = 1	-- 1 - list only
						-- 2 - list with prices by exchange
  as
	---------------
	set nocount on
	---------------
--=================================================================
-- select stocks for which trades was
--=================================================================
	if object_ID('tempdb..#SelContracts') is null
		Create table #SelContracts(contractID int not null)

	insert into #SelContracts
		select contractID 
			from Contract
		where 
		contractID in (select distinct O.underlyingContractID from OptionView O join TradeView TV on TV.contractID=O.optionID)
	union 
		select distinct contractID 
			from contract 
		where contractID in (select contractID from TradeView)
	
--=================================================================
--base section 
--=================================================================
	if @iFormat = 1
		select	iContractID, 
				iContractTypeID, 
				vcSymbol,
				tiCalcOptionType,
				tiCalcModelType,
				tiExpCalendarID
			from vALLStockEOD A
		where iContractID in (select contractID from #SelContracts)
	else
		-- use NULL exchange only
		select iContractID, 
				iContractTypeID, 
				vcSymbol, 
				null as vcExchangeCode,  
				case when isDivCustom = 1 then A.divFreqCustom else A.iDivFreq end iDivFreq,
				case when isDivCustom = 1 then A.divAmtCustom else A.fDivAmt end fDivAmt,
				case when isDivCustom = 1 then A.divDateCustom else A.dtDivDate end dtDivDate,
				CP.priceClose as fPriceClose,
				A.bIsHTB as bIsHTB,
				tiCalcOptionType,
				tiCalcModelType,
				tiExpCalendarID  
			from vALLStockEOD A, ContractPrice CP (nolock)
			where A.iContractID *= CP.contractID and CP.exchangeID is null
			and A.iContractID in (select contractID from #SelContracts)

if object_ID('tempdb..#SelContracts') is not null
	Drop table #SelContracts

if (@@error <> 0)
	Return (-1)
else  
   Return(0)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--============================================================================================================
--change DB version
--============================================================================================================
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.20')
	begin
		update DataInfo set keyValue = '3.20' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.20'
	end

