/*
	MLine00000551
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.43.081' or (keyValue = '3.43.082')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.43.082 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.43.082' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


--========================================================================================================
-- usp_PriceProfile_CheckDefaults
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_UnderlyingByTrader_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_UnderlyingByTrader_Get
GO

CREATE PROCEDURE dbo.usp_UnderlyingByTrader_Get
	@iTraderID int = null,
	@tiTraderMode tinyint = 0 -- 0 - filter by TraderContract, 1 - don't filter by TraderContract
AS
	set nocount on

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

			null as iTraderID,

			s.skew as fSkew,
			s.kurt as fKurt,
		    c.contractName as vcContractName,
			
			case when exists(select 1 from OptionRootView where underlyingID = c.contractID and isSynthetic = 1) 
					then 1 else 0 end tiHasSyntheticRoots,

			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			s.isActive as tiIsActive,
			case when exists(select 1 from OptionRootView where underlyingID = c.contractID) 
					then 1 else 0 end tiHasOptions,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration

		from ContractView c 
		    inner join StockView s on c.contractID = s.stockID
		    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where
			c.contractTypeID = 2
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

			null as iTraderID,

			i.skew as fSkew,
			i.kurt as fKurt,
		    c.contractName as vcContractName,

			case when exists(select 1 from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
					then 1 else 0 end tiHasSyntheticRoots,

			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			i.isActive as tiIsActive,
			case when exists(select 1 from OptionRootView where underlyingID = c.contractID) 
					then 1 else 0 end tiHasOptions,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration

		from ContractView c 
		    inner join IndexView i on c.contractID = i.indexID
		    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where
			c.contractTypeID = 1
		order by c.symbol	 
	else
	begin
		if isnull(@tiTraderMode, 0) = 0
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

				tc.traderID as iTraderID,

				s.skew as fSkew,
				s.kurt as fKurt,
			    c.contractName as vcContractName,
	
				case when exists(select 1 from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
						then 1 else 0 end tiHasSyntheticRoots,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID,
				s.isActive as tiIsActive,
				case when exists(select 1 from OptionRootView where underlyingID = c.contractID) 
						then 1 else 0 end tiHasOptions,

			    cp.volume10Day as iVolume10Day,
			    cp.volume5Expiration as iVolume5Expiration

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
				and tc.traderID = @iTraderID
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

				tc.traderID as iTraderID,

				i.skew as fSkew,
				i.kurt as fKurt,
			    c.contractName as vcContractName,
		
				case when exists(select 1 from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
					then 1 else 0 end tiHasSyntheticRoots,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID,
				i.isActive as tiIsActive,
				case when exists(select 1 from OptionRootView where underlyingID = c.contractID) 
						then 1 else 0 end tiHasOptions,

			    cp.volume10Day as iVolume10Day,
			    cp.volume5Expiration as iVolume5Expiration

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
				and tc.traderID = @iTraderID
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

				tc.traderID as iTraderID,

				s.skew as fSkew,
				s.kurt as fKurt,
			    c.contractName as vcContractName,

				case when exists(select 1 from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
						then 1 else 0 end tiHasSyntheticRoots,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID,
				s.isActive as tiIsActive,
				case when exists(select 1 from OptionRootView where underlyingID = c.contractID) 
						then 1 else 0 end tiHasOptions,

			    cp.volume10Day as iVolume10Day,
			    cp.volume5Expiration as iVolume5Expiration

			from ContractView c 
			    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
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

				tc.traderID as iTraderID,

				i.skew as fSkew,
				i.kurt as fKurt,
			    c.contractName as vcContractName,
	
				case when exists(select 1 from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
						then 1 else 0 end tiHasSyntheticRoots,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID,
				i.isActive as tiIsActive,
				case when exists(select 1 from OptionRootView where underlyingID = c.contractID) 
						then 1 else 0 end tiHasOptions,

			    cp.volume10Day as iVolume10Day,
			    cp.volume5Expiration as iVolume5Expiration

			from ContractView c 
			    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
			order by c.symbol	 
	end

Return (@@error)

/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.43.082')
	begin
		update DataInfo set keyValue = '3.43.082' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.43.082' '3.43.082 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
