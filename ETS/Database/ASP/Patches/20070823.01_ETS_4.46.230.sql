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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.229'  or (keyValue = '3.46.230') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.230 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.230' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END






IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  id = object_id(N'[usp_MmTheoPrices_Save]') 
	   AND 	  OBJECTPROPERTY(id, N'IsProcedure') = 1)
    DROP PROC usp_MmTheoPrices_Save
GO


/*DATA MODIFICATION SECTION*/

-- DATA SAMPLE
-- <root>
--   <contract id="123556" price="234.6000000001" symbol="NDXGG" type="1" />
-- </root>


CREATE     procedure dbo.usp_MmTheoPrices_Save
  @vXMLData ntext  
As  

	DECLARE @idoc int  
	EXEC sp_xml_preparedocument @idoc OUTPUT, @vXMLData  
	
	begin Tran
	
	-- write new theo prices
	update
		ContractPrice
	set
		priceTheo = rsTheoPrices.xmlTheoPrice
	from
	(
		select 
			vXML.contractid as xmlContractID, 
			vXML.price as xmlTheoPrice
		from openxml (@idoc, '/root/contract', 2) with
	 	(
			contractid int '@id',
			price float '@price',
			type int    '@type'
		) vXML
	) rsTheoPrices
	where
		rsTheoPrices.xmlContractID = ContractID

	if @@Error <> 0
	begin
		rollback
		Return -1
	end

	commit
	EXEC sp_xml_removedocument @idoc   
	Return 0


GO


ALTER         PROC usp_MmUnderlying_Get
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

ALTER       PROC usp_MmFuture_Get
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

ALTER      proc dbo.usp_ContractPrice_Save
	@iContractPriceID int = Null output,
	@iContractID int = Null,
	@iExchangeID int = Null, 
	@fPriceBid float = Null,
	@fPriceAsk float = Null,
	@iLotSize int = Null,	-- currently not used !!!
	@fPriceLast float = Null,
	@fPriceOpen float = Null,
	@fPriceClose float = Null,
	@fPriceLow float = Null,
	@fPriceHigh float = Null,
	@iVolume int = Null,
	@iOpenInterest int = Null,
	@vcSymbol varchar(20) = Null,
	@iContractTypeID int = Null, --2-stock 3-option
	@vcExchangeCode varchar(10) = Null,
	@fPriceTheo float = Null
as
/*
	Created by Zaur Nuraliev
	[Description]:	this procedure save information into 
					ContractPrice table
*/
	-------------------------------------------------
	set nocount on
	set ANSI_NULLS OFF --(exchangeID may be null)
	-------------------------------------------------
	declare @error int		set @error = -1
	if @vcSymbol is not null and @iContractTypeID is not null 
		begin
			Select @iContractID = contractID from Contract where symbol=@vcSymbol and contractTypeID=@iContractTypeID
			if @iContractID is null goto finish 
		end
	if @vcExchangeCode is not null
		begin
			Select @iExchangeID=exchangeID from Exchange where exchangeCode=@vcExchangeCode
		end

	Begin Transaction
		if (not exists (select * from ContractPrice(nolock) where contractPriceID = @iContractPriceID) and @iContractPriceID is not Null)
		or (@iContractID is not Null and not exists(select * from ContractPrice where contractID=@iContractID and isnull(exchangeID,0)=isnull(@iExchangeID,0)))
		 begin
			insert into ContractPrice (contractID,
						exchangeID, 
						priceBid,
						priceAsk,
						priceLast,
						priceOpen,
						priceClose,
						priceLow,
						priceHigh,
						volume,
						openInterest,
						priceTheo)
				  values (@iContractID,
						@iExchangeID, 
						@fPriceBid,
						@fPriceAsk,
						@fPriceLast,
						@fPriceOpen,
						@fPriceClose,
						@fPriceLow,
						@fPriceHigh,
						@iVolume,
						@iOpenInterest,
						@fPriceTheo)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			select @iContractPriceID = @@identity

		 end
	   else
		 begin
			update ContractPrice
				set contractID = isNull(@iContractID,contractID),
					exchangeID = isNull(@iExchangeID,exchangeID),
					priceBid = isNull(@fPriceBid,priceBid),
					priceAsk = isNull(@fPriceAsk,priceAsk),
					priceLast = isNull(@fPriceLast,priceLast),
					priceOpen = isNull(@fPriceOpen,priceOpen),
					priceClose = isNull(@fPriceClose,priceClose),
					priceLow = isNull(@fPriceLow,priceLow),
					priceHigh = isNull(@fPriceHigh,priceHigh),
					volume = isNull(@iVolume,volume),
					openInterest = isNull(@iOpenInterest,openInterest),
					actionDate = GetDate(),
					priceTheo = isNull(@fPriceTheo, priceTheo)
					where (@iContractPriceID is not Null and contractPriceID = @iContractPriceID) or 
						(@iContractID is not Null and contractID=@iContractID and isnull(exchangeID,0)=isnull(@iExchangeID,0))
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		 end
		--===========================================================================================
		-- create record with Null ExchangeID
		--===========================================================================================
		if not exists(select * from ContractPrice where ContractID=@iContractID and exchangeID is null)
			begin
				insert into ContractPrice (contractID,
							exchangeID, 
							priceBid,
							priceAsk,
							priceLast,
							priceOpen,
							priceClose,
							priceLow,
							priceHigh,
							volume,
							openInterest,
							priceTheo)
					  values (@iContractID,
							Null, 
							@fPriceBid,
							@fPriceAsk,
							@fPriceLast,
							@fPriceOpen,
							@fPriceClose,
							@fPriceLow,
							@fPriceHigh,
							@iVolume,
							@iOpenInterest,
							@fPriceTheo)
			end
	Commit Transaction
	set @error = 0
finish:



Return(@error)










GO





if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.230')
	begin
		update DataInfo set keyValue = '3.46.230' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.230' '3.46.230 update message:' 
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




