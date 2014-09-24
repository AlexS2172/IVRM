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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.243' or (keyValue = '3.46.244') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '3.46.244 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '3.46.244' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

--=============================================================================================

ALTER     View dbo.OptionView
as
	Select 
		optionID,
		optionRootID,
		isCall,
		strike,
		expiryDate,
		actionID,
		actionDate,
		isManualyEntered,
		expiryOV,
		TradingClose
	from [Option]
	where actionID < 3
GO



ALTER      PROC usp_MmFutureOptionsByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null 
		set @dtMinExpiry = 0
	select
		foc.contractID as iContractID,
		foc.contractTypeID as iContractTypeID,
		foc.symbol as vcSymbol,
		fo.strike as fStrike, 
		fo.expiryDate as dtExpiry,
		fo.isCall as tiIsCall,
		fo.futureID as iFutureID,

		fo.expiryOV 		as dtExpiryOV,
		fo.TradingClose		as dtTradingClose

	from FutureOption fo
		inner join Future f on f.futureID = fo.futureID
		inner join FutureRoot fr on fr.futureRootID = f.futureRootID and fr.underlyingID = @iUnderlyingID
		inner join ContractView foc on fo.futureOptionID = foc.contractID
	where  datediff(d,@dtMinExpiry,fo.expiryDate) >= 0

	return @@error
GO


ALTER         PROC dbo.usp_MmOptionByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null
as
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null
		set @dtMinExpiry = 0
	select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		o.expiryDate as dtExpiry,
		o.isCall as tiIsCall,
		ort.isFitEnabled as tiIsFitEnabled,
		ort.lotSize as iLotSize,
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,
		o.isManualyEntered as iIsFlex,

		o.expiryOV 		as dtExpiryOV,
		o.TradingClose	as dtTradingClose

	from ContractView uc
		inner join OptionRoot ort on uc.contractID = ort.underlyingID
		inner join OptionView o on o.optionRootID = ort.optionRootID
		inner join ContractView c on o.optionID = c.contractID
	where uc.contractID = @iUnderlyingID
		and datediff(d,@dtMinExpiry, o.expiryDate) >= 0

	return @@error
GO




ALTER   PROC dbo.usp_UnderlyingExpiry_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@dtMinExpiry datetime = null
AS
	set nocount on

	if @dtMinExpiry is null	set @dtMinExpiry = '19000101' -- set to min available date to avoid 'is null' checking

	if isnull(@iGroupID, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
		begin
			if isnull(@iContractID, 0) = 0
				select
					c.contractID 	as iUnderlyingID,
					o.expiryDate 	as dtExpiryDate,
					o.ExpiryOV   	as dtExpiryOV,
					o.TradingClose	as dtTradingClose
				from ContractView c
					inner join OptionRootView r on c.contractID = r.underlyingID
					inner join OptionView o on r.optionRootID = o.optionRootID
				where 
					datediff(d, @dtMinExpiry, o.expiryDate) >= 0
				group by
					c.contractID, o.expiryDate, o.ExpiryOV, o.TradingClose
				order by o.expiryDate
			else
				select
					c.contractID 	as iUnderlyingID,
					o.expiryDate 	as dtExpiryDate,
					o.ExpiryOV   	as dtExpiryOV,
					o.TradingClose	as dtTradingClose
				from ContractView c
					inner join OptionRootView r on c.contractID = r.underlyingID
					inner join OptionView o on r.optionRootID = o.optionRootID
				where 
					c.contractID = @iContractID
					and datediff(d, @dtMinExpiry, o.expiryDate) >= 0
				group by
					c.contractID, o.expiryDate, o.ExpiryOV, o.TradingClose
				order by expiryDate
		end
		else
		begin
			if isnull(@iContractID, 0) = 0
				select
					c.contractID 	as iUnderlyingID,
					o.expiryDate 	as dtExpiryDate,
					o.ExpiryOV   	as dtExpiryOV,
					o.TradingClose	as dtTradingClose
				from TraderContractView tc 
				    inner join ContractView c on tc.contractID = c.contractID
					inner join OptionRootView r on c.contractID = r.underlyingID
					inner join OptionView o on r.optionRootID = o.optionRootID
				where 
					tc.traderID = @iTraderID
					and datediff(d, @dtMinExpiry, o.expiryDate) >= 0

				group by
					c.contractID, o.expiryDate, o.ExpiryOV, o.TradingClose
				order by o.expiryDate
			else
				select
					c.contractID 	as iUnderlyingID,
					o.expiryDate 	as dtExpiryDate,
					o.ExpiryOV   	as dtExpiryOV,
					o.TradingClose	as dtTradingClose
				from TraderContractView tc 
				    inner join ContractView c on tc.contractID = c.contractID
					inner join OptionRootView r on c.contractID = r.underlyingID
					inner join OptionView o on r.optionRootID = o.optionRootID
				where 
					tc.traderID = @iTraderID
					and c.contractID = @iContractID
					and datediff(d, @dtMinExpiry, o.expiryDate) >= 0
				group by
					c.contractID, o.expiryDate, o.ExpiryOV, o.TradingClose
				order by expiryDate
		end
	end
	else
	begin
		if isnull(@iTraderID, 0) = 0
			select
				c.contractID 	as iUnderlyingID,
				o.expiryDate 	as dtExpiryDate,
				o.ExpiryOV   	as dtExpiryOV,
				o.TradingClose	as dtTradingClose
			from ContractInGroupView cig 
				inner join ContractView c on c.contractID = cig.contractID
				inner join OptionRootView r on c.contractID = r.underlyingID
				inner join OptionView o on r.optionRootID = o.optionRootID
			where 
				cig.groupID = @iGroupID
				and datediff(d, @dtMinExpiry, o.expiryDate) >= 0
			group by
				c.contractID, o.expiryDate, o.ExpiryOV, o.TradingClose
			order by o.expiryDate
		else
			select
				c.contractID 	as iUnderlyingID,
				o.expiryDate 	as dtExpiryDate,
				o.ExpiryOV   	as dtExpiryOV,
				o.TradingClose	as dtTradingClose
			from ContractInGroupView cig 
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
				inner join OptionRootView r on c.contractID = r.underlyingID
				inner join OptionView o on r.optionRootID = o.optionRootID
			where 
				cig.groupID = @iGroupID
				and tc.traderID = @iTraderID
				and datediff(d, @dtMinExpiry, o.expiryDate) >= 0
			group by
				c.contractID, o.expiryDate, o.ExpiryOV, o.TradingClose
			order by expiryDate
	end
GO



ALTER     procedure dbo.usp_UnderlyingExpiryByList_Get
	@dtMinExpiry datetime = null,
	@vcUnderlyingID1 varchar(8000) = null,
	@vcUnderlyingID2 varchar(8000) = null
AS
	set nocount on

	declare @vcString1 as varchar(8000),
			@vcString2 as varchar(8000),
			@vcString3 as varchar(8000),
			@vcString4 as varchar(8000)

	select @vcString1 = '
		select
			c.contractID 	as iUnderlyingID,
			o.expiryDate 	as dtExpiryDate,
			o.ExpiryOV   	as dtExpiryOV,
			o.TradingClose	as dtTradingClose
		from ContractView c
			join OptionRootView r on c.contractID = r.underlyingID
			join OptionView o on r.optionRootID = o.optionRootID '

	if @dtMinExpiry is not null
		set @vcString3 = 'where o.expiryDate >=' + '''' + convert(varchar(12),@dtMinExpiry,112) + '''' 
	else 	
		set @vcString3 = 'where'

	if @vcString2 is null set @vcString2 = ''

	set @vcString3 = @vcString3 + ' and c.contractID in ('
	set @vcString4 = 'group by c.contractID, o.expiryDate, o.ExpiryOV, o.TradingClose order by expiryDate'

	if isnull(@vcUnderlyingID1,'') != '' and isnull(@vcUnderlyingID2,'') = ''
		begin
			exec(@vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ') ' + @vcString4)
			--select @vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ') ' + @vcString4
		end
	else if isnull(@vcUnderlyingID1,'') != '' and isnull(@vcUnderlyingID2,'') != ''
		begin
			exec(@vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ',' + @vcUnderlyingID2 + ') ' + @vcString4)
			--select @vcString1 + @vcString2 + @vcString3 + @vcUnderlyingID1 + ',' + @vcUnderlyingID2 + ') ' + @vcString4
		end			

Return(@@error)
GO





---------------------------------------------------------------------------------------------------------

if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='RMScenario' and COLUMN_NAME='hours')
ALTER TABLE [RMScenario]
	ADD [hours] int not null default(0),
	    [minutes] int not null default(0)
GO

ALTER    Procedure dbo.usp_RMScenario_Save
	@iRMScenarioID int = Null,
	@vcScenarioName varchar(30) = Null,
	@vcDescription varchar(120) = Null,
    	@iPoints1 int = Null,
	@fStep1 float = Null,
	@tiIsAbs1 tinyint = Null,
	@tiType1 tinyint = Null,
	@iPoints2 int = Null,
	@fStep2 float= Null,
	@tiIsAbs2 tinyint = Null,
	@tiType2 tinyint = Null,
	@tiVolaShift tinyint = Null,
	@iHours int = Null,
	@iMinutes int = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
/*
	if exists (select * from RMScenario where scenarioName = @vcScenarioName and actionID < 3 and (@iRMScenarioID <> RMScenarioID or @iRMScenarioID is null))
		begin
			Raiserror ('Row in RMScenario table with same scenario name already exists!', 16, 1)
			Return (-1)
		end
*/   
	DECLARE @ErrorSave as int
	DECLARE @iRmNewScenatioID as int 

	set @ErrorSave = 0

	if @iRMScenarioID is null
	
	--if not exists(select * from RMScenario where RMScenarioID = @iRMScenarioID)
		begin
			insert into RMScenario(
					ScenarioName,
					[Description],
					points1,
					step1,
					isAbs1,
					type1,
					points2,
					step2,
					isAbs2,
					type2,
					volaShift,
					hours,
					minutes
					) 
				values (
					@vcScenarioName,
					@vcDescription,
				   	@iPoints1,
					@fStep1,
					@tiIsAbs1,
					@tiType1,
					@iPoints2,
					@fStep2,
					@tiIsAbs2,
					@tiType2,
					@tiVolaShift,
					@iHours,
					@iMinutes)
				IF (@@ERROR <> 0)
                                	begin 
    						SET @ErrorSave = @@ERROR
						set @iRmNewScenatioID = 0
	                                end
				else
					begin                               
						set @iRmNewScenatioID = @@IDENTITY
					end

		end
	else
		begin
			update RMScenario set
				ScenarioName = isNull(@vcScenarioName, ScenarioName),
				[Description] = isNull(@vcDescription, [Description]),
				points1 = isNull(@iPoints1, points1),
				step1 = isNull(@fStep1, step1),
				isAbs1 = isNull(@tiIsAbs1, isAbs1),
				type1 = isNull(@tiType1, type1),
				points2 = isNull(@iPoints2, points2),
				step2 = isNull(@fStep2, step2),
				isAbs2 = isNull(@tiIsAbs2, isAbs2),
				type2 = isNull(@tiType2, type2),
				volaShift = isnull(@tiVolaShift, volaShift),
				hours = isnull(@iHours, hours),
				minutes = isnull(@iMinutes, minutes)
				where RMScenarioID = @iRMScenarioID
				IF (@@ERROR <> 0)
                                	begin 
    						SET @ErrorSave = @@ERROR
						set @iRmNewScenatioID = 0
	                                end
				else
					begin                               
						set @iRmNewScenatioID = @iRMScenarioID
					end

		end
if @ErrorSave <> 0
	return -1
else
	return 	@iRmNewScenatioID
GO



ALTER       Procedure dbo.usp_RMScenario_Get
	@iRMScenarioID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
    select rs.RMScenarioID as iRMScenarioID, 
			rs.ScenarioName as vcScenarioName, 
			rs.[Description] as vcDescription,
			points1 as iPoints1,
			step1 as fStep1,
			isAbs1 as tiIsAbs1,
			type1 as tiType1,
			points2 as iPoints2,
			step2 as fStep2,
			isAbs2 as tiIsAbs2,
			type2 as tiType2,
			volaShift as tiVolaShift,
			actionDate as dtActionDate,
			actionID as tiActionID,
			hours as iHours,
			minutes as iMinutes
		from RMScenario rs
		where ((@iRMScenarioID is Null) 
			or (@iRMScenarioID is Not Null and rs.RMScenarioID = @iRMScenarioID))

set @error = @@error
    Return(@error)
GO

-----------------------------------------------------------------------------------------------------------

ALTER   PROC usp_MmTradeByTrader_Get
	@iTraderID int = null
as
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iTraderID is null
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then o.expiryDate else  fo.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,

			case when c.contractTypeID = 3 then o.expiryOV else  fo.expiryOV end as dtExpiryOV,
			case when c.contractTypeID = 3 then o.TradingClose else  fo.TradingClose end as dtTradingClose,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
		order by TradeDate
	else
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,
			tr.orderID as iOrderID,
			tr.execStatus as iExecStatus,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then o.expiryDate else  fo.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,

			case when c.contractTypeID = 3 then o.expiryOV else  fo.expiryOV end as dtExpiryOV,
			case when c.contractTypeID = 3 then o.TradingClose else  fo.TradingClose end as dtTradingClose,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			o.isManualyEntered as iIsFlex,
			c.expCalendarID as iFlexExpCalID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
		where
			tr.traderID = @iTraderID
		order by TradeDate

	return @@error
GO


ALTER      PROC dbo.usp_UnderlyingOptions_Get
	@iUnderlyingID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0,
	@dtMinExpiry datetime = Null
AS
-- changed by VK - flex option support added
	set nocount on
	
	if @dtMinExpiry is null set @dtMinExpiry = 0

	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				if isnull(@iUnderlyingID, 0) <> 0
					select	c.contractID as iContractID,
						c.symbol as vcSymbol,
						ort.underlyingID as iUnderlyingContractID,
						o.strike as fStrike, 
						o.expiryDate as dtExpiry,
						o.expiryOV as dtExpiryOV,
						o.TradingClose as dtTradingClose,
						o.isCall as tiIsCall,
						ort.lotSize as iLotSize,
						ort.optionRootID as iOptionRootID
					from ContractView c  
						inner join [Option] o on o.optionID = c.contractID
						join OptionRoot ort on o.optionRootID = ort.optionRootID
						join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
					where c.contractTypeID = 3
						and ort.underlyingID = @iUnderlyingID
						and datediff(d, @dtMinExpiry, o.expiryDate) >= 0
					order by ort.underlyingID, o.expiryDate, o.strike, o.isCall desc
				else
					select	c.contractID as iContractID,
						c.symbol as vcSymbol,
						ort.underlyingID as iUnderlyingContractID,
						o.strike as fStrike, 
						o.expiryDate as dtExpiry,
						o.expiryOV as dtExpiryOV,
						o.TradingClose as dtTradingClose,
						o.isCall as tiIsCall,
						ort.lotSize as iLotSize,
						ort.optionRootID as iOptionRootID
					from ContractView c  
						inner join [Option] o on o.optionID = c.contractID
						join OptionRoot ort on o.optionRootID = ort.optionRootID
						join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					where c.contractTypeID = 3
						and datediff(d, @dtMinExpiry, o.expiryDate) >= 0
					order by ort.underlyingID, o.expiryDate, o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					o.expiryDate as dtExpiry,
					o.expiryOV as dtExpiryOV,
					o.TradingClose as dtTradingClose,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and datediff(d,@dtMinExpiry,o.expiryDate) >= 0
				order by ort.underlyingID, o.expiryDate, o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					o.expiryDate as dtExpiry,
					o.expiryOV as dtExpiryOV,
					o.TradingClose as dtTradingClose,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
							join OptionRoot ort on cig.contractID = ort.underlyingID
							inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and datediff(d,@dtMinExpiry,o.expiryDate) >= 0
				order by ort.underlyingID, o.expiryDate, o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					o.expiryDate as dtExpiry,
					o.expiryOV as dtExpiryOV,
					o.TradingClose as dtTradingClose,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
							join OptionRoot ort on cig.contractID = ort.underlyingID					
							join ContractView uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and datediff(d,@dtMinExpiry,o.expiryDate) >= 0
				order by ort.underlyingID, o.expiryDate,o.strike, o.isCall desc
		end
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					o.expiryDate as dtExpiry,
					o.expiryOV as dtExpiryOV,
					o.TradingClose as dtTradingClose,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and datediff(d,@dtMinExpiry,o.expiryDate) >= 0
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, o.expiryDate, o.isCall, ort.lotSize, ort.optionRootID,
					o.expiryOV, o.TradingClose
				order by ort.underlyingID, o.expiryDate, o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					o.expiryDate as dtExpiry,
					o.expiryOV as dtExpiryOV,
					o.TradingClose as dtTradingClose,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
				    inner join TraderContractView tc on uc.contractID = tc.contractID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and datediff(d,@dtMinExpiry,o.expiryDate) >= 0
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, o.expiryDate, o.isCall, ort.lotSize, ort.optionRootID,
					o.expiryOV, o.TradingClose
				order by ort.underlyingID, o.expiryDate, o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					o.expiryDate as dtExpiry,
					o.expiryOV as dtExpiryOV,
					o.TradingClose as dtTradingClose,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
					join OptionRootView ort on cig.contractID = ort.underlyingID					
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and datediff(d,@dtMinExpiry,o.expiryDate) >= 0
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, o.expiryDate, o.isCall, ort.lotSize, ort.optionRootID,
					o.expiryOV, o.TradingClose
				order by ort.underlyingID, o.expiryDate, /* year(ec.expiryDate), month(ec.expiryDate),*/ o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					o.expiryDate as dtExpiry,
					o.expiryOV as dtExpiryOV,
					o.TradingClose as dtTradingClose,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
					join OptionRootView ort on cig.contractID = ort.underlyingID					
					inner join ContractView uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and datediff(d,@dtMinExpiry,o.expiryDate) >= 0
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, o.expiryDate, o.isCall, ort.lotSize, ort.optionRootID,
					o.expiryOV, o.TradingClose
				order by ort.underlyingID, o.expiryDate, o.strike, o.isCall desc
		end
	end

	return @@error
GO





ALTER     PROC dbo.usp_ReportsUnderlyingOptions_Get
	@iUnderlyingID int,
	@dtMinExpDate datetime = Null
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @dtMinExpDate is  null
		set @dtMinExpDate = 0

	select
		c.contractID as iOptionID,
		c.symbol as vcSymbol,
		ort.optionRootID as iOptionRootID,
		o.expiryDate as dtExpiry,
		o.expiryOV as dtExpiryOV,
		o.TradingClose as dtTradingClose,
		o.strike as fStrike, 
		o.isCall as tiIsCall,
		0	as IsFutureOption,
		null	as iFutureID
	from
		ContractView c  
		inner join OptionView o on o.optionID = c.contractID
		inner join OptionRootView ort on o.optionRootID = ort.optionRootID
	where
		c.contractTypeID = 3
		and ort.underlyingID = @iUnderlyingID
		and datediff(d,@dtMinExpDate, o.expiryDate) >= 0
union

	select
		c.contractID as iOptionID,
		c.symbol as vcSymbol,
		fr.futureRootID as iOptionRootID,
		fo.expiryDate as dtExpiry,
		fo.expiryOV as dtExpiryOV,
		fo.TradingClose as dtTradingClose,
		fo.strike as fStrike, 
		fo.isCall as tiIsCall,
		1 as IsFutureOption,
		fo.futureID as iFutureID
	from
		ContractView c  
		inner join FutureRoot fr on fr.underlyingID = @iUnderlyingID
		inner join Future f on f.futurerootID = fr.futurerootID
		inner join FutureOption fo on fo.futureoptionID = c.contractID and fo.futureID = f.futureID
	where
		c.contractTypeID = 5 
		and datediff(d,@dtMinExpDate, fo.expiryDate) >= 0

	set @error = @@error

Return(@error)
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER     PROC dbo.usp_MmContractForTradeByUnd_Get
	@iUnderlyingID int
as
-- changed by VK - flex option support added
	-- return 2 recordsets: 1st for option contracts, 2nd futures & futures options contracts
	set nocount on


	-- options rowset
	select	
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		c.contractName as vcContractName, 
		cp.priceClose as fPriceClose,
		cp.priceTheo as fPriceTheoClose,
		
		-- ec.expiryDate as dtExpiry,
		o.expiryDate as dtExpiry,
		o.strike as fStrike,
		o.isCall as tiIsCall,
		ort.optionRootID as iOptionRootID,
		o.expiryOV as dtExpiryOV,
		o.TradingClose as dtTradingClose
	from OptionRootView ort
		inner join OptionView o on o.optionRootID = ort.optionRootID
		inner join ContractView c on c.contractID = o.optionID
		inner join Contract uc on uc.contractID = ort.underlyingID
		-- inner join ExpiryInCalendarView ec on ec.expiryID = o.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
		left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
	where
		ort.underlyingID = @iUnderlyingID
		and datediff(d,getDate(), o.expiryDate) >= 0
	order by o.expiryDate, o.strike, o.isCall desc

	-- futures & futures options rowset
	select * from
	(
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName, 
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			c.symbol as vcFutureSymbol,
			null as dtExpiryOV,
			null as dtTradingClose
		from FutureRoot fr
			inner join Future f on f.futureRootID = fr.futureRootID and fr.underlyingID = @iUnderlyingID
			inner join ContractView c on c.contractID = f.futureID
			left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
			
		union
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName, 
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,
			o.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			fc.symbol as vcFutureSymbol,
			o.expiryOV as dtExpiryOV,
			o.TradingClose as dtTradingClose
		from FutureRoot fr
			inner join Future f on f.futureRootID = fr.futureRootID and fr.underlyingID = @iUnderlyingID
			inner join Contract fc on fc.contractID = f.futureID
			inner join FutureOption o on o.futureID = f.futureID
			inner join ContractView c on c.contractID = o.futureOptionID
			left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where
			datediff(d,GetDate(),o.expiryDate) >= 0
	) as c order by c.vcFutureSymbol, c.dtExpiry, c.fStrike, c.tiIsCall desc

	return @@error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


update stock
set divFreq = 4
where isnull(divFreq, 0) = 0

update stock
set divAmt = 0.1
where isnull(divAmt, 0) = 0

update stock
set divDate = getdate()
where isnull(divDate, 0) = 0

update stock
set divFreqCustom = 4
where isnull(divFreqCustom, 0) = 0

update stock
set divAmtCustom = 0.1
where isnull(divAmtCustom, 0) = 0

update stock
set divDateCustom = getdate()
where isnull(divDateCustom, 0) = 0

------------------
update [index]
set divFreq = 4
where isnull(divFreq, 0) = 0

update [index]
set divAmt = 0.1
where isnull(divAmt, 0) = 0

update [index]
set divDate = getdate()
where isnull(divDate, 0) = 0

update [index]
set divFreqCustom = 4
where isnull(divFreqCustom, 0) = 0

update [index]
set divAmtCustom = 0.1
where isnull(divAmtCustom, 0) = 0

update [index]
set divDateCustom = getdate()
where isnull(divDateCustom, 0) = 0

GO
UPDATE [Option] set TradingClose = cast (0.75 as DateTime)

UPDATE [Option] set expiryOV = expiryDate - 1 + cast (0.625 as DateTime)

GO
UPDATE [FutureOption] set TradingClose = cast (0.75 as DateTime)

UPDATE [FutureOption] set expiryOV = expiryDate - 1 + cast (0.625 as DateTime)
GO

ALTER  PROC usp_MmVegaWeight_Get
AS
set nocount on

	select dt.ExpiryOV as dtExpiryOV, VegaWeight as fVegaWeight 
	from (
			select distinct(CONVERT(varchar, expiryOV, 101)) as expiryOV from FutureOption
			where datediff(d, getDate(), expiryOV) >=0
		union
			select distinct(maturityDate) as expiryOV from Future
			where datediff(d, getDate(), maturityDate) >=0
		union
			select distinct(CONVERT(varchar, expiryOV, 101)) as expiryOV from OptionView
			where datediff(d, getDate(), expiryOV) >=0
		) dt
	left join VegaWeight vw  on datediff(d, vw.ExpiryDate,dt.expiryOV) = 0  
	order by dt.ExpiryOV
GO
--=============================================================================================
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.244')
	begin
		update DataInfo set keyValue = '3.46.244' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0 OR 
   not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_Execute]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	ROLLBACK TRANSACTION
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
	select 'DB VERSION WAS CHANGED TO 3.46.244' '3.46.244 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

