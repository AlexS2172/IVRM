-- fix for MLine00000405 (Item cannot be found in the collection ...)
set nocount on 
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[vALLStockEOD]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[vALLStockEOD]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE          view dbo.vALLStockEOD
AS
SELECT 	C.contractID AS iContractID,   
		C.contractTypeID AS iContractTypeID,    
		C.Symbol AS vcSymbol, 
    	S.divFreq AS iDivFreq, 
		S.divAmt AS fDivAmt,  
		S.divDate AS dtDivDate,
		S.isHTB as bIsHTB, 
		S.divAmtCustom,
		S.divDateCustom,
		S.divFreqCustom,
		S.isDivCustom,
		S.CalcOptionType as tiCalcOptionType,
		S.CalcModelType as tiCalcModelType,
		C.expCalendarID as tiExpCalendarID,
		null as tiIsBasket
	--FROM ContractView C (nolock), StockView S (nolock)
	FROM Contract C (nolock), StockView S (nolock)
	WHERE S.stockID = C.contractID
UNION
SELECT 	C.contractID AS iContractID,  
		C.contractTypeID AS iContractTypeID,
	    C.Symbol AS vcSymbol,  
		NULL AS iDivFreq, 
		I.yield AS fDivAmt, 
		NULL AS dtDivDate,
		I.isHTB as bIsHTB, 
		null as divAmtCustom,
		null as divDateCustom,
		null as divFreqCustom,
		null as isDivCustom,
		I.CalcOptionType as tiCalcOptionType,
		I.CalcModelType as tiCalcModelType,
		C.expCalendarID as tiExpCalendarID,
		I.isBasket as tiIsBasket
	--FROM ContractView C (nolock), IndexView I (nolock)
	FROM Contract C (nolock), IndexView I (nolock)
	WHERE I.indexID = C.contractID



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_allStockEOD_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_allStockEOD_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE         proc dbo.usp_allStockEOD_Get
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
				tiExpCalendarID,
				tiIsBasket
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
				tiExpCalendarID,
				tiIsBasket  
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.21')
	begin
		update DataInfo set keyValue = '3.21' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.21'
	end

