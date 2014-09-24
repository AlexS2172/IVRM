/*
This is a script for next applications release.
DB version:	3.35.010
*/
set nocount on
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexWithPos_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexWithPos_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_InterestRateCurve_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_InterestRateCurve_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockWithPos_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockWithPos_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingByGroup_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingByGroup_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingPos_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingPos_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Underlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Underlying_Get]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO









CREATE        Procedure dbo.usp_Contract_Get
	@iContractID int = null,	-- contract's identifier
	@iContractTypeID int = null,	-- contract's type identifier
	@bAllWithoutOptions bit = 1,	-- flag for data output
	@dtActionDate datetime = null
as
/*
	Created by Andrey Sinkin
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	if @bAllWithoutOptions is null		set @bAllWithoutOptions = 0

	if @bAllWithoutOptions = 1
		select 	Contract.contractID iContractID, 
				Contract.symbol vcSymbolName, 
				Contract.contractName vcDescription, 
				Stock.primaryExchangeID iExchangeID, 
				Contract.contractTypeID iContractTypeID, 
				Stock.divFreq iDivFrequency, 
				Stock.divAmt fAmount, 
				Stock.divDate dtDivDate,
				IndexView.yield fYield,
				Contract.actionDate as dtActionDate,
				Contract.actionID as tiActionID,
				case 
					when Stock.stockID is not null then Stock.calcOptionType 
					else IndexView.calcOptionType end tiCalcOptionType,
				case 
					when Stock.stockID is not null then Stock.IsHTB 
					else IndexView.IsHTB end bIsHTB,
				case 
					when Stock.stockID is not null then Stock.skew 
					else IndexView.skew end fSkew,
				case 
					when Stock.stockID is not null then Stock.kurt 
					else IndexView.kurt end fKurt
			from Contract
				left join StockView Stock on Contract.contractID = Stock.stockID
				left join IndexView on Contract.contractID = IndexView.indexID
			where contractTypeID < 3
				and (@iContractID is null or contractID = @iContractID)
				and (Contract.actionDate>=@dtActionDate or @dtActionDate is null and Contract.actionID<3)
			order by Contract.symbol
	else
		select 	Contract.contractID iContractID, 
				Contract.symbol vcSymbolName, 
				Contract.contractName vcDescription, 
				Stock.primaryExchangeID iExchangeID, 
				Contract.contractTypeID iContractTypeID, 
				Stock.divFreq iDivFrequency, 
				Stock.divAmt fAmount, 
				Stock.divDate dtDivDate,
				IndexView.yield fYield,
				Contract.actionDate as dtActionDate,
				Contract.actionID as tiActionID,
				case 
					when Stock.stockID is not null then Stock.calcOptionType 
					else IndexView.calcOptionType end tiCalcOptionType,
				case 
					when Stock.stockID is not null then Stock.IsHTB 
					else IndexView.IsHTB end bIsHTB,
				case 
					when Stock.stockID is not null then Stock.skew 
					else IndexView.skew end fSkew,
				case 
					when Stock.stockID is not null then Stock.kurt 
					else IndexView.kurt end fKurt
			from Contract
				left join StockView Stock on Contract.contractID = Stock.stockID
				left join IndexView on Contract.contractID = IndexView.indexID
			where (@iContractTypeID is null or contractTypeID = @iContractTypeID)
				and (@iContractID is null or contractID = @iContractID)
				and (Contract.actionDate>=@dtActionDate or @dtActionDate is null and Contract.actionID<3)
			order by Contract.symbol
	
Return(@@Error)









GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO











CREATE    proc dbo.usp_IndexWithPos_Get
	@iIndexID int = null,
	@iTraderID int = null,
	@tiPositionMode tinyint = 0 -- 0 - filter by TraderContract, 1 - don't filter by TraderContract
as
	------------------------------------------------
	set nocount on
	------------------------------------------------

	if @iTraderID is null
		select C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName,
			I.yield as fYield,
			I.isHTB as bIsHTB,
			I.actionDate as dtActionDate,
			I.actionID as tiActionID,
			sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
			CP.lotSize as iLotSize,
			I.CalcOptionType as tiCalcOptionType,
			I.CalcModelType as tiCalcModelType,
			C.expCalendarID as iExpCalendarID,
			I.isBasket as tiIsBasket,
			I.isActive as tiIsActive,
			i.skew as fSkew,
			i.kurt as fKurt,
			sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
		from ContractView c 	
			inner join IndexView I on I.indexID = C.contractID
			join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
			left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
		where C.contractTypeID = 1
			and (C.contractID = @iIndexID or @iIndexID is Null)
		group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
			I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID, 
			I.isBasket, I.isActive, i.skew, i.kurt
		order by C.symbol
	else
	begin
		if isnull(@tiPositionMode, 0) = 0
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName,
				I.yield as fYield,
				I.isHTB as bIsHTB,
				I.actionDate as dtActionDate,
				I.actionID as tiActionID,
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				CP.lotSize as iLotSize,
				I.CalcOptionType as tiCalcOptionType,
				I.CalcModelType as tiCalcModelType,
				C.expCalendarID as iExpCalendarID,
				I.isBasket as tiIsBasket,
				I.isActive as tiIsActive,
				i.skew as fSkew,
				i.kurt as fKurt,
				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractView c 	
				inner join IndexView I on I.indexID = C.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where C.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (C.contractID = @iIndexID or @iIndexID is Null)
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
				I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID, 
				I.isBasket, I.isActive, i.skew, i.kurt
			order by C.symbol
		else
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName,
				I.yield as fYield,
				I.isHTB as bIsHTB,
				I.actionDate as dtActionDate,
				I.actionID as tiActionID,
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				CP.lotSize as iLotSize,
				I.CalcOptionType as tiCalcOptionType,
				I.CalcModelType as tiCalcModelType,
				C.expCalendarID as iExpCalendarID,
				I.isBasket as tiIsBasket,
				I.isActive as tiIsActive,
				i.skew as fSkew,
				i.kurt as fKurt,
				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractView c 	
				inner join IndexView I on I.indexID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where C.contractTypeID = 1
				and (C.contractID = @iIndexID or @iIndexID is Null)
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
				I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID, 
				I.isBasket, I.isActive, i.skew, i.kurt
			order by C.symbol
	end

Return (@@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE    Procedure dbo.usp_InterestRateCurve_Get
	@iCurvePointID int = null,	-- InterestRateCurve identifier
	@bAdvancedMode bit = 0		-- flag for advanced mode
as
/*
	Created by Andrey Sinkin
	this procedure returns data from InterestRateCurve table (now from IRPoint table)
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @date datetime		set @date = getdate()
	if @bAdvancedMode = 0
		select 	IRPointID as iCurvePointID, 
				periodTypeID as iPeriodTypeID, 
				num as iNumber, 
				shortRate as fRateAsk, 
				longRate as fRateBid,
				HTBRate as fHTBRate
			from IRPointView
			where @iCurvePointID is null or IRPointID = @iCurvePointID
			order by iPeriodTypeID, iNumber
	else
		select	IRPointID as iCurvePointID, 
				periodTypeID as iPeriodTypeID, 
				num as iNumber, 
				shortRate as fRateAsk, 
				longRate as fRateBid,
			case periodTypeID
				when 1 then datediff(day, getdate(), dateadd(day, num, @date))
				when 2 then datediff(day, getdate(), dateadd(week, num, @date))
				when 3 then datediff(day, getdate(), dateadd(month, num, @date))
				when 4 then datediff(day, getdate(), dateadd(year, num, @date))
			end iDaysFromToday,
			HTBRate as fHTBRate
			from IRPointView
			where @iCurvePointID is null or IRPointID = @iCurvePointID
			order by iDaysFromToday

Return(@@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO












CREATE    proc dbo.usp_StockWithPos_Get
	@iStockID int = null,
	@iTraderID int = null,
	@tiPositionMode tinyint = 0 -- 0 - filter by TraderContract, 1 - don't filter by TraderContract
as
	------------------------------------------------
	set nocount on
	------------------------------------------------

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
			sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
			S.isDivCustom as tiDivCustom,
			S.divAmtCustom as fDivAmtCustom,
			S.divDateCustom as dtDivDateCustom,
			S.divFreqCustom as iDivFreqCustom,
			CP.lotSize as iLotSize,
			S.CalcOptionType as tiCalcOptionType,
			S.CalcModelType as tiCalcModelType,
			C.expCalendarID as iExpCalendarID,
			S.isActive as tiIsActive,
			s.skew as fSkew,
			s.kurt as fKurt,
			sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
	    from ContractView c  
			inner join StockView S on S.stockID = C.contractID
			join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
			left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
	    where c.contractTypeID = 2
			and (c.contractID = @iStockID or @iStockID is Null)
		group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
			S.DivFreq,S.primaryExchangeID, 
			S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
			S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
			S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive, s.skew, s.kurt
		order by c.Symbol
	else
	begin 
		if isnull(@tiPositionMode, 0) = 0
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
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				S.isDivCustom as tiDivCustom,
				S.divAmtCustom as fDivAmtCustom,
				S.divDateCustom as dtDivDateCustom,
				S.divFreqCustom as iDivFreqCustom,
				CP.lotSize as iLotSize,
				S.CalcOptionType as tiCalcOptionType,
				S.CalcModelType as tiCalcModelType,
				C.expCalendarID as iExpCalendarID,
				S.isActive as tiIsActive,
				s.skew as fSkew,
				s.kurt as fKurt,
				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
		    from ContractView c  
				inner join StockView S on S.stockID = C.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
		    where c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (c.contractID = @iStockID or @iStockID is Null)
			group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
				S.DivFreq, S.primaryExchangeID, 
				S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
				S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
				S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive, s.skew, s.kurt
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
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				S.isDivCustom as tiDivCustom,
				S.divAmtCustom as fDivAmtCustom,
				S.divDateCustom as dtDivDateCustom,
				S.divFreqCustom as iDivFreqCustom,
				CP.lotSize as iLotSize,
				S.CalcOptionType as tiCalcOptionType,
				S.CalcModelType as tiCalcModelType,
				C.expCalendarID as iExpCalendarID,
				S.isActive as tiIsActive,
				s.skew as fSkew,
				s.kurt as fKurt,
				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
		    from ContractView c  
				inner join StockView S on S.stockID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
		    where c.contractTypeID = 2
				and (c.contractID = @iStockID or @iStockID is Null)
			group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
				S.DivFreq, S.primaryExchangeID, 
				S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
				S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
				S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive, s.skew, s.kurt
			order by c.Symbol
	end
	
Return(@@error)












GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE         proc dbo.usp_Stock_Del
        @iStockID int,
        @bisRemovePermanent bit = 0
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Stock table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	if exists (select * from TradeView(nolock) where contractID = @iStockID)
	begin
		Raiserror ('Error. Can''t delete Stock because of the existing Trade', 16, 1)
		Return (-1)
	end 
	if exists(select * from [Option](nolock) where underlyingContractID=@iStockID)
		begin
			Create table #Options_for_delete(OptionID int)
		
			insert into #Options_for_delete
				select optionID from [Option](nolock) where underlyingContractID=@iStockID
		
			if exists (select tradeID from TradeView where contractID in (select optionID from #Options_for_delete))
				begin
					Raiserror ('Error. Can''t delete Stock because of the existing Trade by Stock Options', 16, 1)
					Return (-1)
				end 
		end
	Begin Transaction
		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del @bisRemovePermanent = @bisRemovePermanent
			end

		exec usp_TradeLog_Del null, @iStockID

		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID = @iStockID
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update ContractExchangeLink set actionID = 3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	    --delete ModelParameter where contractID = @iStockID
		--	if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
        update ContractInGroup set actionID=3 where contractID = @iStockID 
        --delete ContractInGroup where contractID = @iStockID 
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockDataHistory where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockCorrelation where stockID1 = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockCorrelation where stockID2 = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

        update IndexDefinition set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		update TraderContract set actionID=3 where contractID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Stock set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

        --delete ContractPriceHistory where contractID = @iStockID
		--	if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete ContractPrice where contractID = @iStockID

			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Contract set actionID=3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		if (@bisRemovePermanent = 1) 
			begin
				delete from ContractExchangeLink where contractID = @iStockID
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	        	delete IndexDefinition where stockID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				delete TraderContract where contractID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		       	delete Stock where stockID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	        	delete Contract where contractID = @iStockID
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end 
  
		set @error = @@error
	Commit Transaction
finish:
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








CREATE            proc dbo.usp_UnderlyingByGroup_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int,
	@tiWithPosition tinyint = 0
AS
	set nocount on

	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
			    cp.lotSize as iLotSize,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and (@iContractID is null or c.contractID = @iContractID)
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
			    cp.lotSize as iLotSize,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and (@iContractID is null or c.contractID = @iContractID)
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
			    cp.lotSize as iLotSize,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
			    cp.lotSize as iLotSize,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			order by c.symbol	 
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,

				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
				cp.lotSize, s.isActive, s.skew, s.kurt
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,

				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, cp.lotSize, i.isActive, i.skew, i.kurt
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,

			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,

				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose,
				cp.lotSize, s.isActive, s.skew, s.kurt
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,

				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, cp.lotSize, i.isActive, i.skew, i.kurt
			order by c.symbol	 
	end

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






CREATE  procedure dbo.usp_UnderlyingPos_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithSimulated tinyint = 0
AS
	set nocount on

	if isnull(@iGroupID, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractView c 
				left join TradeView tr on tr.contractID = c.contractID 
							and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.isSimulated, 0) = 0)
			where
				c.contractTypeID in (1, 2)
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID
		else
			select
			    c.contractID as iContractID,
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
				left join TradeView tr on tr.contractID = c.contractID 
							and tr.traderID = @iTraderID 
							and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.isSimulated, 0) = 0)
			where 
				c.contractTypeID in (1, 2)
				and tc.traderID = @iTraderID
			group by 
				c.contractID
	end
	else
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
				left join TradeView tr on tr.contractID = c.contractID 
							and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.isSimulated, 0) = 0)
			where 
				cig.groupID = @iGroupID
				and c.contractTypeID in (1, 2)
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID
		else
			select
			    c.contractID as iContractID,
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
				left join TradeView tr on tr.contractID = c.contractID 
							and tr.traderID = @iTraderID 
							and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.isSimulated, 0) = 0)
			where cig.groupID = @iGroupID
				and c.contractTypeID in (1, 2)
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID
	end

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




CREATE   procedure dbo.usp_Underlying_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0
AS
	set nocount on

	if isnull(@iGroupID, 0) <> 0
	begin
		exec usp_UnderlyingByGroup_Get @iContractID,@iTraderID,@iGroupID,@tiWithPosition	
		Return(0)
	end 

	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
			    cp.lotSize as iLotSize,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractView c 
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
				and (@iContractID is null or c.contractID = @iContractID)
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
			    cp.lotSize as iLotSize,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractView c 
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
				and (@iContractID is null or c.contractID = @iContractID)
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
			    cp.lotSize as iLotSize,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

			    cp.lotSize as iLotSize,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			order by c.symbol	 
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,
				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractView c 
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
			where
				c.contractTypeID = 2
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
				cp.lotSize, s.isActive, s.skew, s.kurt
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,
				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractView c 
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
			where
				c.contractTypeID = 1
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, cp.lotSize, i.isActive, i.skew, i.kurt
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,
				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where 
				c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
				cp.lotSize, s.isActive, s.skew, s.kurt
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,
				sum(tr.quantity * tr.price * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where
				c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, cp.lotSize, i.isActive, i.skew, i.kurt
			order by c.symbol	 
	end

	Return (0)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.35.010')
	begin
		update DataInfo set keyValue = '3.35.010' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.35.010'
	end





