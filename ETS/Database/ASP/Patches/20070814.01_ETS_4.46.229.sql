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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.228'  or (keyValue = '3.46.229') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.229 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.229' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[ManualPrice]') 
	   AND 	  OBJECTPROPERTY(id, N'IsUserTable') = 1)
    DROP TABLE [ManualPrice]
GO


CREATE TABLE ManualPrice
(
    ContractID int NOT NULL,
    ManualPrice float NOT NULL
	CONSTRAINT [FK_ManualPrice_Contract] FOREIGN KEY 
	(
		[ContractID]
	) REFERENCES [Contract] (
		[ContractID]
	)
)


if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='VolaSurfaceData' and COLUMN_NAME='IsManualVol')
	ALTER TABLE VolaSurfaceData ADD IsManualVol TINYINT NULL
GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_IsManualVol_Save]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_IsManualVol_Save
GO

CREATE PROC usp_IsManualVol_Save
@ContractID int,
@IsManualVal tinyint
AS

UPDATE	VolaSurfaceData SET IsManualVol = @IsManualVal WHERE contractID = @ContractID

GO


IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_MmManualPrice_Save]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_MmManualPrice_Save
GO

CREATE PROC usp_MmManualPrice_Save
@ContractID int,
@ManualPrice float
AS

if exists (select 1 from ManualPrice where @ContractID = ContractID)
	UPDATE ManualPrice SET ManualPrice = @ManualPrice WHERE @ContractID = ContractID
else
	INSERT INTO ManualPrice (ContractID, ManualPrice) VALUES (@ContractID, @ManualPrice)

GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_MmManualPrice_Del]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_MmManualPrice_Del
GO

CREATE PROC usp_MmManualPrice_Del
@ContractID int
AS

DELETE FROM ManualPrice WHERE @ContractID = ContractID

GO


IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_MmManualPrice_Get]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_MmManualPrice_Get
GO

CREATE PROC usp_MmManualPrice_Get
@ContractID int = NULL
AS

SELECT MP.contractID, MP.manualPrice, C.Symbol FROM ManualPrice MP
JOIN Contract C ON C.contractID = MP.contractID
WHERE @ContractID = C.ContractID OR @ContractID IS NULL

GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_MmManualPriceForTrade_Get]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_MmManualPriceForTrade_Get
GO

CREATE PROC usp_MmManualPriceForTrade_Get
AS

SELECT T.TradeID, MP.ContractID, MP.ManualPrice FROM ManualPrice MP
LEFT JOIN Trade T ON T.contractID = MP.contractID
--SELECT MP.ContractID, MP.ManualPrice FROM ManualPrice MP

GO



ALTER        PROC usp_MmUnderlying_Get
	@iTraderID int = null
AS
	set nocount on

	if isnull(@iTraderID, 0) = 0
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
			case when i.indexID is null then s.calcOptionType else i.calcOptionType end as tiCalcOptionType,
			case when i.indexID is null then s.isHTB else i.isHTB end as tiIsHTB,
		
		    i.yield as fYield,
			i.isBasket as tiIsBasket,
		    s.divFreq as iDivFreq,
		    s.divDate as dtDivDate,
		    s.divAmt as fDivAmt,
		
		    s.isDivCustom as tiIsDivCustom,
		    s.divFreqCustom as iDivFreqCustom,
		    s.divDateCustom as dtDivDateCustom,
		    s.divAmtCustom as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,
    		    cp.priceTheo as fPriceTheoClose,

			null as iTraderID,

			case when i.indexID is null then s.skew else i.skew end as fSkew,
			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
			case when i.indexID is null then s.isHedgeSymbol else i.isHedgeSymbol end as tiIsHedgeSymbol,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration,
		    i.SOQ As SOQ,
		    	case when i.IndexID is null then isnull(s.primaryExchangeID,0) else isnull(i.primaryExchangeID,0) end as iPrimaryExchangeID,
		    mp.manualPrice as dManualActivePrice,
		    vsd.IsManualVol as fManualVol
		from ContractView c 
		    left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		    left join ManualPrice mp on mp.contractID = c.contractID
		    left join VolaSurfaceData vsd on vsd.contractID = c.contractID 
		where
			c.contractTypeID in (1,2,6) and not c.Symbol is null
	else
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
			case when i.indexID is null then s.calcOptionType else i.calcOptionType end as tiCalcOptionType,
			case when i.indexID is null then s.isHTB else i.isHTB end as tiIsHTB,
		
		    i.yield as fYield,
			i.isBasket as tiIsBasket,
		    s.divFreq as iDivFreq,
		    s.divDate as dtDivDate,
		    s.divAmt as fDivAmt,
		
		    s.isDivCustom as tiIsDivCustom,
		    s.divFreqCustom as iDivFreqCustom,
		    s.divDateCustom as dtDivDateCustom,
		    s.divAmtCustom as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,
			    		    cp.priceTheo as fPriceTheoClose,

			tc.traderID as iTraderID,

			case when i.indexID is null then s.skew else i.skew end as fSkew,
			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
			case when i.indexID is null then s.isHedgeSymbol else i.isHedgeSymbol end as tiIsHedgeSymbol,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration,
		    i.SOQ As SOQ,
		    	case when i.IndexID is null then isnull(s.primaryExchangeID,0) else isnull(i.primaryExchangeID,0) end as iPrimaryExchangeID,
		    mp.manualPrice as dManualActivePrice,
		    vsd.IsManualVol as fManualVol
		from ContractView c 
		    left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
		    left join ManualPrice mp on mp.contractID = c.contractID
		    left join VolaSurfaceData vsd on vsd.contractID = c.contractID 
		where
			c.contractTypeID in (1,2,6) and not c.Symbol is null

	return @@error


GO

ALTER      PROC usp_MmFuture_Get
	@iFutureRootID int = null
AS
	-- retrieve futures by future root
	set nocount on

	if @iFutureRootID is null
		select
			fc.contractID as iFutureID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcFutureSymbol,
			fc.contractName as vcFutureName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			fc.undPriceProfileID as iUndPriceProfileID,
			fc.optPriceProfileID as iOptPriceProfileID,
			f.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
		   	fcp.priceClose as fPriceClose,
    		    fcp.priceTheo as fPriceTheoClose,
			f.maturityDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			case when exists(select top 1 1 from FutureOption where futureID = fc.contractID) then 1 else 0 end as iHaveOptions,
			isnull(fu.tiGreeksCalculationType ,0) as iShowInFutureContractEq,
			isnull(f.dBasis, 0) as dBasis ,
			f.dActiveFutureRatio as dActiveRatio,
			isnull(f.bUseInCalc,0) as bUseInCalculation,
			mp.manualprice as fManualActivePrice
		from Future f
			inner join ContractView fc on fc.contractID = f.futureID
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			inner join ContractView uc on uc.contractID = fr.underlyingID
		    inner join ContractPrice fcp on fc.contractID = fcp.contractID and fcp.exchangeID is null
			left outer join FutureUnderlying fu on fu.iFutureUnderlyingID = uc.contractID
			left join ManualPrice mp on mp.contractID = f.futureID
	else
		select
			fc.contractID as iFutureID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcFutureSymbol,
			fc.contractName as vcFutureName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			fc.undPriceProfileID as iUndPriceProfileID,
			fc.optPriceProfileID as iOptPriceProfileID,
			f.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
			fcp.priceClose as fPriceClose,
    		    fcp.priceTheo as fPriceTheoClose,
			f.maturityDate  as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			case when exists(select top 1 1 from FutureOption where futureID = fc.contractID) then 1 else 0 end as iHaveOptions,
			isnull(fu.tiGreeksCalculationType ,0) as iShowInFutureContractEq,
			isnull(f.dBasis, 0) as dBasis ,
			f.dActiveFutureRatio as dActiveRatio,
			isnull(f.bUseInCalc,0) as bUseInCalculation,
			mp.manualprice as fManualActivePrice
		from Future f
			inner join ContractView fc on fc.contractID = f.futureID
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			inner join ContractView uc on uc.contractID = fr.underlyingID
		    inner join ContractPrice fcp on fc.contractID = fcp.contractID and fcp.exchangeID is null
			left outer join FutureUnderlying fu on fu.iFutureUnderlyingID = uc.contractID
			left join ManualPrice mp on mp.contractID = f.futureID
		where
			f.futureRootID = @iFutureRootID



GO





ALTER        PROC usp_MmFutureByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null	
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @dtMinExpiry is null set @dtMinExpiry = 0	

	select
		f.futureID as iContractID,
		fc.contractTypeID as iContractTypeID,
		fc.symbol as vcSymbol,		
		--e.ExpiryPeriod as vcMaturity,
                f.maturityDate as dtMaturity,
		fu.ContractId as iFutureUnderlyingID,
                f.futureRootID as iFutureRootID,
		isnull(ff.tiGreeksCalculationType, 1) as tiGreeksCalculationType,
		cp.priceClose as fPriceClose,
    		    cp.priceTheo as fPriceTheoClose,
		isnull(f.dBasis, 0) as dBasis ,
		f.dActiveFutureRatio as dActiveRatio,
		isnull(f.bUseInCalc,0) as bUseInCalculation,
		mp.manualprice as fManualPrice
	from ContractView fu
                inner join FutureRoot fr on fr.underlyingID = fu.ContractId
                inner join Future f on f.futureRootID = fr.futureRootID
		inner join ContractView fc on f.futureID = fc.ContractId
		left join ContractPrice cp on f.futureID = cp.ContractID
		left join FutureUnderlying ff on ff.iFutureUnderlyingID = fu.ContractId
		left join ManualPrice mp on mp.contractID = f.futureID
	where fu.ContractId = @iUnderlyingID
		and datediff(d, @dtMinExpiry, f.maturityDate) >=0

	return @@error
GO



if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.229')
	begin
		update DataInfo set keyValue = '3.46.229' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.229' '3.46.229 update message:' 
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




