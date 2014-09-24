/*
	fix for MLine00000993
*/

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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.39.050' or (keyValue = '3.39.051')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.39.051 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.39.051' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_SyntheticParams_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_SyntheticParams_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeCA_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeCA_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE          PROCEDURE dbo.usp_SyntheticParams_Get 
	@iRootID int,
	@bIsWithPrice bit = 0,
	@bIsWithHV bit = 0
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is getting information from SyntheticUnderlyingParam table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @bIsWithPrice = 0
		select 	
			sor.optionRootID as iRootID,
			sup.underlyingID as iUnderlyingID, 
			sup.SyntheticCoeff as fWeight
		from SyntheticUnderlyingParamView as sup
		join SyntheticOptionRootView as sor
		on sor.syntheticOptRootID = sup.syntheticOptRootID
		where sor.optionRootID = @iRootID
	else
		begin
		
		if	@bIsWithHV = 0
			-- use NULL exchange only
			select 
				sor.optionRootID as iRootID,
				sup.underlyingID as iUnderlyingID, 
				sup.SyntheticCoeff as fWeight,
				A.iContractID as iContractID,  
				A.iContractTypeID as iContractTypeID,
				A.vcSymbol as vcSymbol,
				case when isDivCustom = 1 then A.divFreqCustom else A.iDivFreq end iDivFreq,
				case when isDivCustom = 1 then A.divAmtCustom else A.fDivAmt end fDivAmt,
				case when isDivCustom = 1 then A.divDateCustom else A.dtDivDate end dtDivDate,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				A.tiCalcOptionType as tiCalcOptionType,
				A.tiCalcModelType as tiCalcModelType,
				A.tiExpCalendarID as tiExpCalendarID,
				A.tiIsBasket as tiIsBasket
			from SyntheticUnderlyingParamView sup join vALLStockEOD A			
				on A.iContractID = sup.underlyingID
				join SyntheticOptionRootView as sor
				on sor.syntheticOptRootID = sup.syntheticOptRootID
				join ContractPrice CP (nolock)
				on A.iContractID = CP.contractID where CP.exchangeID is null and sor.optionRootID = @iRootID
		else
			begin
			select 
				sor.optionRootID as iRootID,
				sup.underlyingID as iUnderlyingID, 
				sup.SyntheticCoeff as fWeight,
				C.ContractID as iContractID,  
				C.ContractTypeID as iContractTypeID,
				C.Symbol as vcSymbol,
				case when isDivCustom = 1 then divFreqCustom else S.divfreq end iDivFreq,
				case when isDivCustom = 1 then divAmtCustom else S.divAmt end fDivAmt,
				case when isDivCustom = 1 then divDateCustom else S.DivDate end dtDivDate,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				S.CalcOptionType as tiCalcOptionType,
				S.CalcModelType as tiCalcModelType,
				C.ExpCalendarID as tiExpCalendarID,
				null as tiIsBasket,
				S2.currentHV as fCurrentHV
			from SyntheticUnderlyingParamView sup 
				join ContractView C 
				on C.contractID = sup.underlyingID
				join StockView S			
				on S.stockID = C.contractID
				join SyntheticOptionRootView as sor
				on sor.syntheticOptRootID = sup.syntheticOptRootID
				join ContractPrice CP (nolock)
				on S.stockID = CP.contractID 
				left join (select StockDataHistory.stockID,currentHV,[date] from StockDataHistory 
				join (select stockID, max([date]) as maxDate from StockDataHistory group by stockID) S1
					on S1.stockID=StockDataHistory.stockID and S1.maxDate=StockDataHistory.[date]) S2 
				on S2.stockID=S.stockID where CP.exchangeID is null and sor.optionRootID = @iRootID
			union
			select 
				sor.optionRootID as iRootID,
				sup.underlyingID as iUnderlyingID, 
				sup.SyntheticCoeff as fWeight,
				C.ContractID as iContractID,  
				C.ContractTypeID as iContractTypeID,
				C.Symbol as vcSymbol,
				0 as iDivFreq,
				I.yield as fDivAmt,
				null as dtDivDate,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				I.CalcOptionType as tiCalcOptionType,
				I.CalcModelType as tiCalcModelType,
				C.ExpCalendarID as tiExpCalendarID,
				null as tiIsBasket,
				S2.currentHV as fCurrentHV
			from SyntheticUnderlyingParamView sup 
				join ContractView C 
				on C.contractID = sup.underlyingID
				join IndexView I			
				on I.indexID = C.contractID
				join SyntheticOptionRootView as sor
				on sor.syntheticOptRootID = sup.syntheticOptRootID
				join ContractPrice CP (nolock)
				on I.indexID = CP.contractID
				left join (select IndexDataHistory.indexID,currentHV,[date] from IndexDataHistory join
					(select indexID, max([date]) as maxDate from IndexDataHistory group by indexID) S1
					on S1.indexID=IndexDataHistory.indexID and S1.maxDate=IndexDataHistory.[date]) S2 
				on S2.indexID=I.indexID where CP.exchangeID is null and sor.optionRootID = @iRootID 
			end
		end
	set @error = @@error
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE    proc dbo.usp_TradeCA_Save
		@iCorpActionID int,
        @iTradeID int = null, --for new trade is null
		@iQuantity int, 
		@fPrice float, 
		@iContractID int = null, 
		@tiIsClose tinyint = 0, 
		@tiIsBuy tinyint = 0, 
		@iTraderID int = null, 
		@tiIsPosition tinyint = 0,
		@tiIsSimulated tinyint = 0
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction TU
		--delete of trade, which we plan to update
		if (select count(*) from Trade where tradeID = @iTradeID)!=2 and @iTradeID is not null
			begin
				exec @error = usp_TradeCA_Del @iCorpActionID,@iTradeID
					if @@error <> 0 begin Rollback Transaction TU Return(0) end
			end

		Declare @iNewTradeID int
			select @iNewTradeID = isnull(max(tradeID),0) + 1 from Trade
	
		Declare @iNewSeqNum int 
			select @iNewSeqNum = isnull(max(seqNum),0) + 1 from Trade
	
		--===============================================
		-- variables for work with CorporateActionLog
		--================================================
		declare @vcTableName sysname,
				@iCActionID int, --1 for insert, 2 - for update, 3 - for delete
				@vcKeyRow varchar(50),  -- name of field with identity atribute
				@iRowID int,			-- tipically is identity value
				@vcFieldName varchar(50),
				@vcFieldValue varchar(250),
				@vcFieldType varchar(50)

		select @iCActionID = 1, @vcTableName = 'Trade', @vcKeyRow = 'seqNum'

		--update existing trade
		if @iTradeID is not null
		begin
			insert into Trade(tradeID,
				actionID,
				contractID,
				isClose,
				quantity,
				price,
				isBuy,
				traderID,
				isPosition,
				isSimulated,
				SeqNum,
				brokerCommissionID,
				clearingBrokerCommissionID)
			select @iNewTradeID,
				1,
				contractID,
				isClose,
				@iQuantity,
				@fPrice,
				isBuy,
				traderID,
				isPosition,
				isSimulated,
				@iNewSeqNum,
				brokerCommissionID,
				clearingBrokerCommissionID
			from Trade
			where tradeID = @iTradeID and actionID=1

			set @error = @@error
				if @error != 0 begin Rollback Transaction TU Return(-1) end
		end
	else
		begin
			-- insert new trade
			insert into Trade(
				tradeID,
				actionID,
				contractID,
				isClose,
				quantity,
				price,
				isBuy,
				traderID,
				isPosition,
				isSimulated,
				seqNum,
				brokerCommissionID,
				clearingBrokerCommissionID)
			select 
				@iNewTradeID,
				1,
				@iContractID,
				@tiIsClose,
				@iQuantity,
				@fPrice,
				@tiIsBuy,
				@iTraderID,
				@tiIsPosition,
				@tiIsSimulated,
				@iNewSeqNum,
				Null,
				Null

			set @error = @@error
				if @error != 0 begin Rollback Transaction TU Return(-1) end
		end
	
		--write into CA log
		select 	@vcFieldName = 'seqNum', 
				@vcFieldValue = @iNewSeqNum,
				@vcFieldType = 'int',
				@iRowID = @iNewSeqNum
		exec @error = usp_LogCA_Save @iCorpActionID, @vcTableName, @vcFieldName, @vcFieldValue, @vcFieldType,@iCActionID,@iRowID,@vcKeyRow

		if @@error = 0 
			begin 
				Commit Transaction TU 
				Return @iNewSeqNum 
			end 
		else 
			begin 
				Rollback Transaction TU 
				Return(-1) 
			end

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.39.051')
	begin
		update DataInfo set keyValue = '3.39.051' where dataInfoID = 1
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
	PRINT 'The database update succeeded'
	PRINT 'DB VERSION WAS CHANGED TO 3.39.051'
	COMMIT TRANSACTION
END
	ELSE PRINT 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
