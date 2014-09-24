/*
	this script fix MLine00000209 bug
		and make the ability to show current position in quote screen 
*/
set nocount on 
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.05')
	begin
		update DataInfo set keyValue = '3.05' where DataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.05'
	end
GO
--changes in [Index] and Stock tables (isHTB fields)
Begin transaction
	if (select OBJECT_ID('DF_Indexes_isHTB')) is not null
		Alter table dbo.[Index]
			Drop constraint DF_Indexes_isHTB

	Alter table dbo.[Index]
		Alter Column isHTB tinyint

	Alter table dbo.[Index] Add constraint
		DF_Indexes_IsHTB Default 0 For isHTB

if @@trancount > 0
	Commit tran
GO

Begin transaction
	if (select OBJECT_ID('DF_Stocks_IsHTB')) is not null
		Alter table dbo.Stock
			Drop constraint DF_Stocks_IsHTB

	Alter table dbo.Stock 
		Alter Column isHTB tinyint

	Alter table dbo.Stock Add constraint
		DF_Stocks_IsHTB Default 0 For isHTB

if @@trancount > 0
	Commit tran

GO
-- refresh [Index] and Stock view's 
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
ALTER   View dbo.IndexView
as
	Select 
		indexID,
		defaultModelTypeID,
		yield,
		isHTB,
		actionID,
		actionDate
	from [Index]
	where actionID < 3
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
ALTER   View dbo.StockView
as
	Select 
		stockID,
		defaultModelTypeID,
		defaultSurfaceTypeID,
		divFreq,
		baseIndexID,
		isHTB,
		primaryExchangeID,
		divAmt,
		divDate,
		actionID,
		actionDate
	from Stock
	where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
-- eliminate existing null values in Trades
Update Trade set isPosition = 0 where isPosition is null
GO
Begin transaction
-- make a default value for Trades (for isPosition field)

	if (select OBJECT_ID('DF_Trade_isPosition')) is not null
		Alter table dbo.Trade
			Drop constraint DF_Trade_isPosition

	Alter table dbo.Trade Add constraint
		DF_Trade_isPosition Default 0 For isPosition

if @@trancount > 0
	Commit tran
GO
-- last version of usp_Trade_Del
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

ALTER  proc usp_Trade_Del
        @iTradeID int
as
	if @iTradeID is not null
	begin
		begin tran

		declare @nCount int,
			@iNewSeqNum int
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iTradeID
		
		if @nCount = 1
		begin
			insert into Trade(tradeID,
				actionID,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition)
			select tradeID,
				0,
				contractID,
				isClose,
				tradeDate,
				quantity,
				price,
				isBuy,
				bookID,
				strategyID,
				traderID,
				brokerID,
				clearingBrokerID,
				brokerCommissionID,
				clearingBrokerCommissionID,
				tradedIV,
				spotReference,
				isPosition
			from Trade
			where tradeID = @iTradeID

			if @@error = 0
			begin
				set @iNewSeqNum = @@identity
				commit tran
				return @iNewSeqNum
			end
			else
			begin
				rollback tran
				return 0
			end
		end
		else
		begin
			rollback tran
			return 0
		end
	end
	else
		return 0

/*
        delete Trade where tradeID = @iTradeID
        if (@@error <> 0)
           return (-1)
         else  
           return 0
*/
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--last version of usp_CustomVolatilityPointIsExist_Get for Vola Manager and indices for Vola Manager core tables
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

ALTER   Procedure dbo.usp_CustomVolatilityPointIsExist_Get
	@vcSymbolName varchar(8) = null
as
/*
	Created by Zaur Nuraliev
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------

	select distinct 
			CV.contractID as iContractID,
			CV.symbol as vcSymbolName,
			OV.expiry as dtExpiry,
			case when CSSP.volaSurfaceDataID is null then 0  
			else 1 end 'CustomVolatilityPointsCount'
		from OptionView OV
			join ContractView CV on OV.underlyingContractID = CV.contractID
			left join VolaSurfaceDataView VSD on OV.underlyingContractID = VSD.contractID and VSD.isDefaultSurface = 1
			left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
			and Convert(varchar(6),OV.expiry,112) = Convert(varchar(6),CSSP.expDate,112)
		where 
			CSSP.volaSurfaceDataID is null and 
			(CV.symbol = @vcSymbolName or @vcSymbolName is null) 
		order by CV.contractID,OV.expiry

Return(0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--CustomStrikeSkewPoint indices
if not exists(select * from sysindexes where id=object_id('CustomStrikeSkewPoint') and name='volaSurfaceDataID_expDate_ind')
	create index volaSurfaceDataID_expDate_ind on CustomStrikeSkewPoint(volaSurfaceDataID,expDate)
GO
if not exists(select * from sysindexes where id=object_id('CustomStrikeSkewPoint') and name='actionID_volaSurfaceDataID_ind')
	create index actionID_volaSurfaceDataID_ind on CustomStrikeSkewPoint(actionID,volaSurfaceDataID)
GO
--VolaSurfaceData indices
if not exists(select * from sysindexes where id=object_id('VolaSurfaceData') and name='volaSurfaceDataID_contractID_ind')
	create index volaSurfaceDataID_contractID_ind on VolaSurfaceData(volaSurfaceDataID,contractID)
GO
if not exists(select * from sysindexes where id=object_id('VolaSurfaceData') and name='actionDate_ind')
	create index actionDate_ind on VolaSurfaceData(actionDate)
GO
if not exists(select * from sysindexes where id=object_id('VolaSurfaceData') and name='contractID_ind')
	create index contractID_ind on VolaSurfaceData(contractID)
GO
if not exists(select * from sysindexes where id=object_id('Trade') and name='actionDate_ind')
	create index actionDate_ind on Trade(actionDate)

GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE    proc usp_Index_Get
	@iGroupID int = Null,
	@iIndexID int = Null,
	@dtActionDate datetime = Null,
	@tiWithPosition tinyint = 0
  as
	if isnull(@tiWithPosition, 0) = 0
	begin
		if (@iGroupID is Null)
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID
				from ContractView c 	
						join [Index] I on I.indexID = C.contractID
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				order by C.symbol
		else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID
				from ContractInGroupView cig
						join ContractView c  on C.contractID = CIG.contractID
						join [Index] I on I.indexID = C.contractID
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				order by C.symbol
	end
	else
	begin
		if (@iGroupID is Null)
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractView c 	
						join [Index] I on I.indexID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID
				order by C.symbol
		else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.defaultModelTypeID as iDefaultModelTypeID,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				from ContractInGroupView cig
						join ContractView c  on C.contractID = CIG.contractID
						join [Index] I on I.indexID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and (I.actionDate>=@dtActionDate or @dtActionDate is null and I.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, I.defaultModelTypeID,
					I.yield, I.isHTB, I.actionDate, I.actionID
				order by C.symbol
	end

if (@@error <> 0)
	Return (-1)
else  
	Return (0)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




CREATE   proc usp_Option_Get
	@iUnderlyingID int = Null,		 
	@dtMinExpiry datetime = Null,
	@iOptionID int = Null,
	@tiBaseInfoOnly tinyint = 0,
	@dtActionDate datetime = null,
	@tiWithPosition tinyint = 0
  as
/*
	Created by Zaur Nuraliev
	[Description]: this procedure return inf. from OptionView table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if isnull(@tiWithPosition, 0) = 0
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				O.expiry as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID
				  from ContractView c  
						inner join [Option] O on O.optionID = C.contractID
				where c.contractTypeID=3
						and ((O.expiry >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
						and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
						and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
						and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
				order by o.underlyingContractID, year(o.expiry), month(o.expiry), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				O.expiry as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID
				  from ContractView c  
						inner join [Option] O on O.optionID = C.contractID
				where c.contractTypeID=3
						and ((O.expiry >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
						and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
						and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
						and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
	end
	else
	begin
		if @tiBaseInfoOnly = 0 or @tiBaseInfoOnly is null
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName, 
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				O.expiry as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				  from ContractView c  
						inner join [Option] O on O.optionID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
				where c.contractTypeID=3
						and ((O.expiry >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
						and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
						and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
						and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, O.underlyingContractID,
					O.strike, O.expiry, O.isCall, O.actionDate, O.actionID
				order by o.underlyingContractID, year(o.expiry), month(o.expiry), o.strike, o.isCall desc
		else
			select	C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				O.underlyingContractID as iUnderlyingContractID,
				O.strike as fStrike, 
				O.expiry as dtExpiry,
				O.isCall as bIsCall,
				O.actionDate as dtActionDate,
				O.actionID as tiActionID,
				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition
				  from ContractView c  
						inner join [Option] O on O.optionID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
				where c.contractTypeID=3
						and ((O.expiry >= @dtMinExpiry and @dtMinExpiry is not Null) or (@dtMinExpiry is Null))
						and ((O.underlyingContractID = @iUnderlyingID and @iUnderlyingID is not Null) or (@iUnderlyingID is Null))
						and ((O.optionID=@iOptionID and @iOptionID is not Null) or (@iOptionID is Null))
						and (O.actionDate>=@dtActionDate or @dtActionDate is null and O.actionID<3)
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName, O.underlyingContractID,
					O.strike, O.expiry, O.isCall, O.actionDate, O.actionID
	end

	set @error = @@error
Return(@error)






GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE         proc usp_Stock_Get
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
						S.actionID as tiActionID
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
						S.actionID as tiActionID
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
						sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition

				    from ContractView c  
						join Stock S on S.stockID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
				    where c.contractTypeID = 2
						and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
						and (S.actionDate >= @dtActionDate or @dtActionDate is null  and S.actionID < 3)
					group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
						S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
						S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID
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
						sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition

				    from ContractInGroupView cig
						join ContractView c  on C.contractID = CIG.contractID
						join Stock S on S.stockID = C.contractID
						left join TradeView tr on tr.contractID = c.contractID
			        where CIG.groupID = @iGroupID 
						and c.contractTypeID = 2
						and (S.actionDate >= @dtActionDate or @dtActionDate is null and S.actionID < 3)
					group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, S.defaultModelTypeID, 
						S.defaultSurfaceTypeID, S.DivFreq, S.BaseIndexID, S.primaryExchangeID, 
						S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID
					order by c.Symbol
	end
	
	
	set @error = @@error
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



