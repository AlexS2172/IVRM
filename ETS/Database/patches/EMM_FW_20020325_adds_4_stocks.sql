/*
	this script adds 4 stocks (SLB,SII,TDW,BJS)
	and fix problem with stock in BO (items can't be found).
	this script work correctly only at 3.06 version (or higher) of DB 
*/

--===================================================================================================
--insert new stocks
--===================================================================================================
set nocount on

Declare @vcSymbol varchar(8)

set @vcSymbol = 'SLB'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='SII'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='TDW'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
set @vcSymbol='BJS'
if not exists(select * from Contract where contractTypeID=2 and symbol = @vcSymbol)
	begin
		insert into Contract (contractTypeID, symbol) values (2,@vcSymbol)
		insert into Stock(stockID) values(@@identity) 
	end
GO

--===================================================================================================
--fix problem with stock in BO (items can't be found).
--===================================================================================================

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
ALTER            proc usp_Stock_Get
	@iGroupID int = null,
	@iStockID int = null,
	@dtActionDate datetime = null,
	@tiWithPosition tinyint = 0
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
				select	C.contractID as iContractID, 
						C.contractTypeID as iContractTypeID, 
						C.Symbol as vcSymbol, 
						C.ContractName as vcContractName, 
						S.defaultModelTypeID as iDefaultModelTypeID, 
						S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
						S.DivFreq as iDivFreq,
						S.BaseIndexID as iBaseIndexID,
						S.primaryExchangeID as iPrimaryExchangeID, 
						S.DivDate as dtDivDate,
						S.DivAmt as fDivAmt, 
						S.isHTB as bIsHTB,
						S.actionDate as dtActionDate,
						S.actionID as tiActionID,
						S.isDivCustom as tiDivCustom,
						S.divAmtCustom as fDivAmtCustom,
						S.divDateCustom as dtDivDateCustom,
						S.divFreqCustom as iDivFreqCustom
				    from ContractView c  
						join Stock S on S.stockID = C.contractID
				    where c.contractTypeID = 2
						and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
						and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
				   order by c.Symbol
		else
				select	C.contractID as iContractID,
						C.contractTypeID as iContractTypeID,
						C.Symbol as vcSymbol,
						C.ContractName as vcContractName, 
						S.defaultModelTypeID as iDefaultModelTypeID, 
						S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
						S.DivFreq as iDivFreq,
						S.BaseIndexID as iBaseIndexID,
						S.primaryExchangeID as iPrimaryExchangeID, 
						S.DivDate as dtDivDate,
						S.DivAmt as fDivAmt, 
						S.isHTB as bIsHTB,
						S.actionDate as dtActionDate,
						S.actionID as tiActionID,
						S.isDivCustom as tiDivCustom,
						S.divAmtCustom as fDivAmtCustom,
						S.divDateCustom as dtDivDateCustom,
						S.divFreqCustom as iDivFreqCustom
				    from ContractInGroupView cig
						join ContractView c  on C.contractID = CIG.contractID
						join Stock S on S.stockID = C.contractID
			        where CIG.groupID = @iGroupID 
						and c.contractTypeID = 2
						and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
				   order by c.Symbol
	end
	else
	begin
		if (@iGroupID is Null)
					select	C.contractID as iContractID, 
						C.contractTypeID as iContractTypeID, 
						C.Symbol as vcSymbol, 
						C.ContractName as vcContractName, 
						S.defaultModelTypeID as iDefaultModelTypeID, 
						S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
						S.DivFreq as iDivFreq,
						S.BaseIndexID as iBaseIndexID,
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
						S.divFreqCustom as iDivFreqCustom
				    from ContractView c  
						join Stock S on S.stockID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
				    where c.contractTypeID = 2
						and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
						and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
					group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
						S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
						S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
						S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom
					order by c.Symbol
		else
					select	C.contractID as iContractID,
						C.contractTypeID as iContractTypeID,
						C.Symbol as vcSymbol,
						C.ContractName as vcContractName, 
						S.defaultModelTypeID as iDefaultModelTypeID, 
						S.defaultSurfaceTypeID as iDefaultSurfaceTypeID, 
						S.DivFreq as iDivFreq,
						S.BaseIndexID as iBaseIndexID,
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
						S.divFreqCustom as iDivFreqCustom
				    from ContractInGroupView cig
						join ContractView c  on C.contractID = CIG.contractID
						join Stock S on S.stockID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
			        where CIG.groupID = @iGroupID 
						and c.contractTypeID = 2
						and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
					group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
						S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
						S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
						S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom
					order by c.Symbol
	end
	
	
	set @error = @@error
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

