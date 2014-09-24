SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
/*PREPARATIONS FOR ERROR HANDLING*/
SET NOCOUNT ON
GO
IF EXISTS (SELECT * FROM tempdb..sysobjects WHERE id=OBJECT_ID('tempdb..#tmpErrors')) DROP TABLE #tmpErrors
GO
CREATE TABLE #tmpErrors (Error int)
GO
SET XACT_ABORT ON
GO
/*SCRIPTS MAIN SECTION*/
BEGIN TRANSACTION
GO
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.178'  or (keyValue = '3.46.179') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.179 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.179' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

ALTER  PROCEDURE dbo.usp_Stock_Get
	@iGroupID int = null,
	@iStockID int = null,
	@tiWithPosition tinyint = 0,
	@iTraderID int = null
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Stock,Contract tables
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if isnull(@tiWithPosition, 0) = 0
	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView C  
					inner join StockView S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID = 2 and not c.Symbol is null
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				order by c.Symbol
			else
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView C  
					inner join StockView S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				order by c.Symbol
		end
		else
		begin
			if @iTraderID is null
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join StockView S on S.stockID = C.contractID

					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
				order by c.Symbol
			else
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join StockView S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
					and tc.traderID = @iTraderID
				order by c.Symbol
		end
	end
	else
	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
			    from ContractView c  
					inner join StockView S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) = 0
			    where c.contractTypeID = 2
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				group by C.contractID, 
					C.contractTypeID, 
					C.Symbol, 
					C.ContractName,
					S.DivFreq,
					S.DivDate, 
					S.DivAmt, 
					S.isHTB, 
					S.actionDate,	
					S.actionID,
					S.isDivCustom,
					S.divAmtCustom,
					S.divDateCustom,
					S.divFreqCustom,
					S.CalcOptionType,
					S.CalcModelType,
					C.expCalendarID, 
					S.isActive,
					S.primaryExchangeID,
					S.skew,
					S.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.Symbol
			else
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
			    from ContractView c  
					inner join StockView S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) = 0
			    where c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
					S.DivFreq, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive,
					S.skew,	S.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.Symbol
		end
		else
		begin
			if @iTraderID is null
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
			    from ContractInGroupView cig
					inner join ContractView c  on C.contractID = CIG.contractID
					inner join StockView S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) = 0
		        where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, 
					S.DivFreq,S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive,
					S.skew, S.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.Symbol
			else
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
			    from ContractInGroupView cig
					inner join ContractView c  on C.contractID = CIG.contractID
					inner join StockView S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) = 0
		        where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
					and tc.traderID = @iTraderID
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
					S.DivFreq, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive,
					S.skew, S.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.Symbol
		end
	end
	
	set @error = @@error

Return(@error)

GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.179')
	begin
		update DataInfo set keyValue = '3.46.179' where dataInfoID = 1
	end
GO

/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
IF EXISTS (SELECT * FROM #tmpErrors)
BEGIN
	ROLLBACK TRANSACTION
	--SELECT * FROM #tmpErrors
END
GO
IF @@TRANCOUNT > 0 
BEGIN
	select 'DB VERSION WAS CHANGED TO 3.46.179' '3.46.179 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO