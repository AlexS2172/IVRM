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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.209'  or (keyValue = '3.46.210') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.210 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.210' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

GO

----------------------------------------------------------------------------------------------

ALTER TABLE [IndexBeta] DROP CONSTRAINT [FK_IndexBeta_Index]
GO

ALTER TABLE [IndexBeta] ADD CONSTRAINT [FK_IndexBeta_Index] FOREIGN KEY 
	(
		[indexID]
	) REFERENCES [Contract] (
		[contractID]
	)
GO

----------------------------------------------------------------------------------------------

if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Stock' and COLUMN_NAME='isHedgeSymbol')
ALTER TABLE Stock ADD isHedgeSymbol tinyint,
		CONSTRAINT DEF_Stock_isHedgeSymbol DEFAULT(0) for isHedgeSymbol
GO

if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Index' and COLUMN_NAME='isHedgeSymbol')
ALTER TABLE [Index] ADD isHedgeSymbol tinyint,
		CONSTRAINT DEF_Index_isHedgeSymbol DEFAULT(0) for isHedgeSymbol
GO


ALTER   VIEW dbo.StockView
AS
	select	stockID, 
			divFreq, 
			isHTB, 
			primaryExchangeID, 
			divAmt, 
			divDate, 
			divAmtCustom, 
            		divDateCustom, 
			divFreqCustom, 
			isDivCustom, 
			actionID, 
			actionDate, 
			calcOptionType, 
			calcModelType, 
			isActive,
			skew,
			kurt,
			isHedgeSymbol
		from dbo.Stock
		where actionID < 3



GO

ALTER    VIEW dbo.IndexView
AS
	select  indexID, 
			yield, 
			isHTB, 
			actionID,
			actionDate, 
			CalcOptionType, 
			CalcModelType, 
			isBasket, 
			isActive,
			skew,
			kurt,
			SOQ,
			isHedgeSymbol
		from [Index]
		where actionID < 3

GO
GO
ALTER     PROC usp_MmUnderlying_Get
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
		    i.SOQ As SOQ
		from ContractView c 
		    left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
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
		    i.SOQ As SOQ
		from ContractView c 
		    left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
		where
			c.contractTypeID in (1,2,6) and not c.Symbol is null

	return @@error

GO
if exists (select * from dbo.sysobjects where name = 'usp_HedgeSymbolBeta_Load' and id = object_id(N'[dbo].[usp_HedgeSymbolBeta_Load]'))
DROP PROCEDURE dbo.usp_HedgeSymbolBeta_Load
GO

CREATE	PROCEDURE dbo.usp_HedgeSymbolBeta_Load
	@iHedgeSymbolID int
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	delete IndexBeta where indexID = @iHedgeSymbolID

	insert into IndexBeta (indexID, contractID, beta) 
	select 	eod_i.contractID,
		eod_u.contractID,
		eod_ib.beta
	from ETS_ASP_EOD.dbo.IndexBeta eod_ib
		inner join EodUnderlyingMap eod_i on eod_i.eodContractID = eod_ib.indexID
		inner join EodUnderlyingMap eod_u on eod_u.eodContractID = eod_ib.contractID
	where eod_ib.actionID <3 and eod_i.contractID = @iHedgeSymbolID

	set @error = @@error
Return(@error)
GO

----------------------------------------------------------------------------------------------

update [Index] set isHedgeSymbol = 1 where indexId in (select contractId from Contract where symbol = 'SPX')
GO

update [Index] set isHedgeSymbol = 1 where indexId in (select contractId from Contract where symbol = 'DJX')
GO

update [Index] set isHedgeSymbol = 1 where indexId in (select contractId from Contract where symbol = 'OEX')
GO

update [Index] set isHedgeSymbol = 1 where indexId in (select contractId from Contract where symbol = 'NDX')
GO


----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------

if exists (select * from dbo.sysobjects where name = 'usp_HedgeSymbols_Get' and id = object_id(N'[dbo].[usp_HedgeSymbols_Get]'))
DROP PROCEDURE dbo.usp_HedgeSymbols_Get
GO

CREATE	PROC dbo.usp_HedgeSymbols_Get
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	select 
		c.contractID 	as 	iContractID,
		c.symbol 	as 	vcHedgeSymbol
	from [Contract] c
		left join [StockView] s on s.stockID = c.contractID
		left join [IndexView] i on i.indexID = c.contractID
	where i.isHedgeSymbol = 1 or s.isHedgeSymbol = 1
	order by c.symbol

	return @@error
GO

----------------------------------------------------------------------------------------------

ALTER  PROC dbo.usp_MmIndexUnderlyingBeta_Get
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	select 
		ib.indexID as iIndexID, 
		c.symbol as vcIndexSymbol, 
		ib.contractID as iContractID, 
		ib.beta as fBeta
	from IndexBetaView ib
	inner join Contract c on c.contractID = ib.indexID

	return @@error
GO

----------------------------------------------------------------------------------------------

ALTER  PROCEDURE dbo.usp_IndexUnderlyingBeta_Get 
	@iIndexID int = null, 
	@iContractID int = null
AS
/*
	Created by: Zaur
	Description: this procedure created with template, desined by Zaur Nuraliev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select 
		ib.contractBetaID as iContractBetaID, 
		ib.indexID as iIndexID, 
		c.symbol as vcIndexSymbol, 
		ib.contractID as iContractID, 
		ib.beta as fBeta,
		ib.actionID as tiActionID, 
		ib.actionDate as dtActionDate
	from IndexBetaView ib
	inner join Contract c on c.contractID = ib.indexID
	where (ib.indexID = @iIndexID or @iIndexID is null) and (ib.contractID = @iContractID or @iContractID is null)
		
	set @error = @@error
Return(@error)
GO

----------------------------------------------------------------------------------------------
GO


ALTER   PROCEDURE dbo.usp_Stock_Save
        @iStockID int,
        @iExchangeID int = Null,
        @vcSymbol varchar(20) = Null,
        @vcContractName varchar(255) = Null,
	@bIsHTB  bit = Null,
        @iDivFreq int = Null,
		@fDivAmt float = Null,
		@dtDivDate smalldatetime = Null,
		@tiIsDivCustom tinyint = 0,
		@fDivAmtCustom float = Null,
		@iDivFreqCustom int = Null,
		@dtDivDateCustom smalldatetime = Null,
		@tiCalcOptionType tinyint = null,
		@tiCalcModelType tinyint = null,
		@iExpCalendarID int = null,
		@tiIsActive tinyint = null,
		@fSkew float = null,
		@fKurt float = null,
		@tiCheckExistance tinyint = 1,
		@iUndPriceProfileID int = null output,
		@iOptPriceProfileID int = null output,
		@bIsHedgeSymbol  tinyint = Null
  as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @bIsHTB is null 
		set @bIsHTB=0

	--if @iExpCalendarID is null or @iExpCalendarID = 0
	--	set @iExpCalendarID = 1

	if len(ltrim(@vcSymbol)) = 0
		begin
			Raiserror ('Empty stock ticker!', 16, 1)
			Return (-1)
		end   
   
	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID in (1, 2) and actionID < 3 and (@iStockID <> contractID or @iStockID is null))
		begin
			if isnull(@tiCheckExistance, 1) <> 0
				Raiserror ('Stock or index with this ticker already exists!', 16, 1)
			Return (-1)
		end   
   if (@iStockID is Null)  
     begin
         begin tran
			if @iUndPriceProfileID is null
			begin
				select
					@iUndPriceProfileID = priceProfileID
				from
					PriceProfileView
				where
					defaultID = 1
					and isOptionProfile = 0
			end

			if @iOptPriceProfileID is null
			begin
				select
					@iOptPriceProfileID = priceProfileID
				from
					PriceProfileView
				where
					defaultID = 1
					and isOptionProfile = 1
			end

            insert into Contract (contractTypeID, Symbol, contractName/*, expCalendarID*/, undPriceProfileID, optPriceProfileID)
                  values (2, @vcSymbol, @vcContractName/*, null*//*@iExpCalendarID*/, @iUndPriceProfileID, @iOptPriceProfileID)
			if (@@error <> 0) begin rollback tran return(-1) end        

            set @iStockID = @@identity         
 
            insert into Stock (
				stockID, 
                divFreq, 
				isHTB,
				primaryExchangeID,
				divAmt,
				divDate,
				isDivCustom,
				divAmtCustom,
				divFreqCustom,
				divDateCustom,
				calcOptionType,
				calcModelType,
				isActive,
				skew,
				kurt,
				isHedgeSymbol)
            values (
				@iStockID, 
                @iDivFreq, 
				@bIsHTB,
				@iExchangeID,
				@fDivAmt,
				@dtDivDate,
				@tiIsDivCustom,
				@fDivAmtCustom,
				@iDivFreqCustom,
				@dtDivDateCustom,
				@tiCalcOptionType,
				@tiCalcModelType,
				@tiIsActive,
				@fSkew,
				@fKurt,
				@bIsHedgeSymbol)
			if (@@error <> 0) begin rollback tran return(-1) end           
			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iStockID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@iStockID,null)
					if (@@error <> 0) begin rollback tran return(-1) end           
				end 
         commit tran
     end
   else
     begin

        declare @df int

        select @df = DivFreq from Stock where stockID = @iStockID        
        begin tran
           update Contract
                set symbol = isNull(@vcSymbol,symbol),
                    contractName = isNull(@vcContractName,contractName),
					--expCalendarID = null --isNull(@iExpCalendarID,expCalendarID),
					undPriceProfileID = isnull(@iUndPriceProfileID, undPriceProfileID),
					optPriceProfileID = isnull(@iOptPriceProfileID, optPriceProfileID)
	            where contractID = @iStockID
			if (@@error <> 0) begin rollback tran return(-1) end         

			update Stock
	               set  
	                    divFreq = isNull(@iDivFreq,divFreq), 
						isHTB = isNull(@bIsHTB, isHTB),
						primaryExchangeID = isNull(@iExchangeID,primaryExchangeID),
						DivAmt = isNull(@fDivAmt, DivAmt),
						DivDate = isNull(@dtDivDate, DivDate),
						isDivCustom = isNull(@tiIsDivCustom, isDivCustom),
						divAmtCustom = isNull(@fDivAmtCustom, divAmtCustom),
						divFreqCustom = isNull(@iDivFreqCustom, divFreqCustom),
						divDateCustom = isNull(@dtDivDateCustom, divDateCustom),
						calcOptionType = isNull(@tiCalcOptionType, calcOptionType),
						calcModelType = isNull(@tiCalcModelType, calcModelType),
						isActive = isnull(@tiIsActive, isActive),
						skew = isNull(@fSkew,skew),
						kurt = isNull(@fKurt,kurt),
						isHedgeSymbol = isNull(@bIsHedgeSymbol, isHedgeSymbol)
		          where stockID = @iStockID
    	       if (@@error <> 0) begin rollback tran return(-1) end         

			if @iDivFreq = 0 
				begin
					update Stock set divAmt = null, divDate = null where stockID = @iStockID
				end

			if @iDivFreqCustom = 0 
				begin
					update Stock set divAmtCustom = null, divDateCustom = null where stockID = @iStockID
				end

         commit tran
     end        

 if (@@error <> 0)
    return (-1)
  else  
    return @iStockID




GO
GO

ALTER    PROCEDURE usp_Index_Save
		@iIndexID int,
		@vcSymbol varchar(20) = null,
		@vcContractName varchar(255) = null,
		@fYield float = Null,
		@bIsHTB bit = Null,
		@tiCalcOptionType tinyint = null,
		@tiCalcModelType tinyint = null,
		@iExpCalendarID int = null,
		@tiIsBasket tinyint = null,
		@tiIsActive tinyint = null,
		@fSkew float = null,
		@fKurt float = null,
		@tiCheckExistance tinyint = 1,
		@SOQ as integer = 0,
		@iUndPriceProfileID int = null output,
		@iOptPriceProfileID int = null output,
		@bIsHedgeSymbol  tinyint = Null
   as
	---------------
	set nocount on
	---------------

	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID in (1, 2) and actionID < 3 and (@iIndexID <> contractID or @iIndexID is null))
		begin
			if isnull(@tiCheckExistance, 1) <> 0
				Raiserror ('Index or stock with this ticker already exists!', 16, 1)
			Return (-1)
		end   

   if (@iIndexID is Null)  
	 begin
		--Michael Malyshkin
		--if @iExpCalendarID is null or @iExpCalendarID = 0
		--	set @iExpCalendarID = 2
		--Michael Malyshkin
		 
		 begin tran
			if @iUndPriceProfileID is null
			begin
				select
					@iUndPriceProfileID = priceProfileID
				from
					PriceProfileView
				where
					defaultID = 3
					and isOptionProfile = 0
			end

			if @iOptPriceProfileID is null
			begin
				select
					@iOptPriceProfileID = priceProfileID
				from
					PriceProfileView
				where
					defaultID = 1
					and isOptionProfile = 1
			end

            insert into Contract (contractTypeID, Symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
                  values (1, @vcSymbol, @vcContractName, null, @iUndPriceProfileID, @iOptPriceProfileID)

			 select @iIndexID = @@identity			 
				 if (@@error <> 0)
					  begin
						RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
						ROLLBACK TRAN
						RETURN (-1)
					  end
			 insert into [Index] (
							indexID, 
							yield,
							IsHTB,
							calcOptionType,
							calcModelType,
							isBasket, 
							isActive,
							skew,
							kurt,
							SOQ,
							isHedgeSymbol)
					 values (
							@iIndexID, 
							@fYield,
							isNull(@bIsHTB,0),
							@tiCalcOptionType,
							@tiCalcModelType,
							isnull(@tiIsBasket,0), 
							@tiIsActive,
							@fSkew,
							@fKurt,
							@SOQ,
							@bIsHedgeSymbol)
				 if (@@error <> 0)
					begin
					   RAISERROR ('Error. Can''t insert into [Index]!', 16, 1)
					   ROLLBACK TRAN
					   RETURN (-1)
					end 			

   		 --insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcSymbol, @vcSymbol)
         --        /* begin FT_CQ00008370*/
         --        declare @iExpCalendarID_Last int
         --       set @iExpCalendarID_Last = SCOPE_IDENTITY()

         --        update Contract set expCalendarID = @iExpCalendarID_Last
         --          where contractID = @iIndexID
         --        /* end   FT_CQ00008370*/
	
		 commit tran
	 end
   else
	 begin
		begin tran
		   update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@vcContractName,contractName),
					--expCalendarID = null,--isNull(@iExpCalendarID, expCalendarID),
					undPriceProfileID = isnull(@iUndPriceProfileID, undPriceProfileID),
					optPriceProfileID = isnull(@iOptPriceProfileID, optPriceProfileID)
				where contractID = @iIndexID
			if (@@error <> 0)
				 begin
					RAISERROR ('Error. Can''t update Contract!', 16, 1)
					ROLLBACK TRAN
					RETURN (-1)
				 end
		  update [Index]
			   set	yield = isnull(@fYield,yield),
					isHTB = isnull(@bIsHTB,isHTB),
					calcOptionType = isnull(@tiCalcOptionType, calcOptionType),
					calcModelType = isnull(@tiCalcModelType, calcModelType),
					isBasket = isnull(@tiIsBasket,isBasket),
					isActive = isnull(@tiIsActive, isActive),
					skew = isnull(@fSkew,Skew),
					kurt = isnull(@fKurt,Kurt),
					SOQ = isnull(@SOQ,SOQ),
					isHedgeSymbol = isnull(@bIsHedgeSymbol, isHedgeSymbol)
			   where indexID = @iIndexID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t update [Index]!', 16, 1)
				  ROLLBACK TRAN
				  RETURN (-1)
			   end		   
		 commit tran
	 end

	--========================================================================
	-- make record with null exchange for this index
	--========================================================================
	if not exists(select * from ContractPrice where exchangeID is null and contractID = @iIndexID)
		begin
			insert into ContractPrice(contractID,exchangeID)
				values(@iIndexID,null)
			if (@@error <> 0) begin rollback tran return(-1) end           		
		end		

if (@@error <> 0)
	return (-1)
else	
	return @iIndexID



GO

GO




ALTER         PROC usp_AspEodUpdateMarketStructure_Main_Execute 
 @iWithTran int = null
AS
 -------------------------------------------------
 set nocount on
 -------------------------------------------------
 declare @error int set @error = 0
 
 declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
 if @iWithTran is not null begin tran @vcTranName
 
 -------------------------------------------------
 -- underlying ID mapping
 truncate table EodUnderlyingMap
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to truncate underlyings map table', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 insert EodUnderlyingMap
  (eodContractID, contractID)
 select
  eod_c.contractID,
  c.contractID
 from Contract c (nolock)
  full join ETS_ASP_EOD.dbo.Contract eod_c
   on eod_c.symbol = c.symbol --and eod_c.contractTypeID = c.contractTypeID -- it is possible to have in client base indeced declared as stocks
 where
  c.actionID < 3
  and eod_c.actionID < 3
  and eod_c.contractTypeID in (1,2,4,6)
  and c.contractTypeID in (1,2,4,6)
 
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to get underlyings map', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 

 -------------------------------------------------
 -- future roots ID mapping
 truncate table EodFutureRootMap
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to truncate future roots table', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 insert EodFutureRootMap
  (eodFutureRootID, futureRootID, eodFutureLotSize, eodOptionLotSize)
select
  eod_fr.futureRootID,
  fr.futureRootID,
  eod_fr.futureLotSize,
  eod_fr.optionLotSize
 from FutureRoot fr (nolock)
  --inner join EodUnderlyingMap eod_um on eod_um.contractID = fr.underlyingID
  inner join ETS_ASP_EOD.dbo.FutureRoot eod_fr
    on  eod_fr.futureRootSymbol = fr.futureRootSymbol
 
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to get future roots map', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 -------------------------------------------------
 -- stocks & indices
 delete from TradeLog
 from TradeLog tl
  inner join Contract c (nolock) on tl.underlyingID = c.contractID
  inner join EodUnderlyingMap eod_um on eod_um.contractID = c.contractID
  inner join ETS_ASP_EOD.dbo.Contract eod_c
    on eod_c.contractID = eod_um.eodContractID and eod_c.contractTypeID < 6
 where c.actionID < 3
 
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to clear trades log', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 -- contract names
 update Contract
 set
  contractName = eod_c.contractName,
  MSupdateDate = eod_c.MSupdateDate
 from Contract c (nolock)
  inner join EodUnderlyingMap eod_um on eod_um.contractID = c.contractID
  inner join ETS_ASP_EOD.dbo.Contract eod_c
    on eod_c.contractID = eod_um.eodContractID and eod_c.contractTypeID in (1,2,4)
 where c.actionID < 3
 
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to update contracts', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 -- stocks info
 update Stock
 set divAmt = eod_s.divAmt,
  divDate = eod_s.divDate, 
  divFreq = eod_s.divFreq
 from Stock s (nolock)
  inner join Contract c (nolock) on c.contractID = s.stockID and c.actionID < 3
  inner join EodUnderlyingMap eod_um on eod_um.contractID = c.contractID
  inner join ETS_ASP_EOD.dbo.Contract eod_c
    on eod_c.contractID = eod_um.eodContractID and eod_c.contractTypeID = 2
  inner join ETS_ASP_EOD.dbo.Stock eod_s
    on eod_c.contractID = eod_s.stockID
 where s.actionID < 3
 
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to update stocks info', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 -----------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------
 
 update ContractPrice
 set priceClose = eod_cp.priceClose, volume10Day = eod_cp.volume10Day, volume5Expiration = eod_cp.volume5Expiration
 from  ETS_ASP_EOD.dbo.ContractView eod_cv 
  inner join ContractView cv on eod_cv.symbol = cv.symbol and (cv.contractTypeID = eod_cv.contractTypeID)
  inner join ContractPrice cp (nolock) on cv.contractID = cp.contractID
  inner join ETS_ASP_EOD.dbo.ContractPrice eod_cp on eod_cv.contractID = eod_cp.contractID
 
 -- case when the symbol type in EOD base and client base are differ. Only for stocks and indeces
 update ContractPrice
 set priceClose = eod_cp.priceClose, volume10Day = eod_cp.volume10Day, volume5Expiration = eod_cp.volume5Expiration
 from  ETS_ASP_EOD.dbo.ContractView eod_cv 
  inner join ContractView cv on eod_cv.symbol = cv.symbol and cv.contractTypeID <> eod_cv.contractTypeID and cv.contractTypeID in (1,2)  
  inner join ContractPrice cp (nolock) on cv.contractID = cp.contractID
  inner join ETS_ASP_EOD.dbo.ContractPrice eod_cp on eod_cv.contractID = eod_cp.contractID
 where eod_cv.contractTypeID in (1,2,6,7)
 
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to update ContractPrice info', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 -------------------------------------------------
 -- options & option roots
 
 truncate table EodContractWithTrades
 
 insert into EodContractWithTrades (contractID)
 select contractID
 from Contract
 where actionID < 3
  and (
    contractID in (select contractID from Trade group by contractID)
    or contractID in (select underlyingID from Trade group by underlyingID)
    or contractID in (select futureID from Trade group by futureID)
    or contractID in (select contractID from [Order] group by contractID)
    or contractID in (select optID from TntCard group by optID)
    or contractID in (select undID from TntCard group by undID)
   )
 
 exec @error = usp_AspEodUpdateMarketStructure_ProcessExpiredOptions
 if @@error != 0 or @error != 0
 begin
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 exec @error = usp_AspEodUpdateMarketStructure_ProcessNewOptions
 if @@error != 0 or @error != 0
 begin
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 exec @error = usp_AspEodUpdateMarketStructure_ProcessMissedOptions
 if @@error != 0 or @error != 0
 begin
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
	update OptionRoot set LotSize = EOD.LotSize from OptionRoot O
	inner join EODOptionRoot EOD on O.Symbol COLLATE Latin1_General_CI_AS = EOD.symbol COLLATE Latin1_General_CI_AS  and EOD.LotSize <> O.LotSize
	if @@error != 0 or @error != 0
	begin
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

 -------------------------------------------------
 -- futures & futures options
 exec @error = usp_AspEodUpdateMarketStructure_ProcessExpiredFutureOptions

 if @@error != 0 or @error != 0
 begin
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 exec @error = usp_AspEodUpdateMarketStructure_ProcessExpiredFutures
 if @@error != 0 or @error != 0
 begin
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 exec @error = usp_AspEodUpdateMarketStructure_ProcessNewFutureOptions
 if @@error != 0 or @error != 0
 begin
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 exec @error = usp_AspEodUpdateMarketStructure_ProcessMissedFutureOptions
 if @@error != 0 or @error != 0
 begin
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 exec @error = usp_AspEodUpdateMarketStructure_ProcessMissedFutures
 if @@error != 0 or @error != 0
 begin
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 -- delete option roots w/o options
 delete SyntheticUnderlyingParam  where syntheticOptRootID not in 
  (select S.syntheticOptRootID from [Option] O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to delete option roots w/o options', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 delete SyntheticRootIndexBeta  where syntheticOptRootID not in 
  (select S.syntheticOptRootID from [Option] O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to delete option roots w/o options', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 delete  SyntheticUnderlyingParam where syntheticOptRootID in
 ( select syntheticOptRootID  from SyntheticOptionRoot where optionRootID not in (select optionRootID from [Option] where actionID < 3))
 
 delete SyntheticOptionRoot where optionRootID not in (select optionRootID from [Option] where actionID < 3)
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to delete option roots w/o options', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 delete [Option] where actionID >= 3
 
 delete OptionRoot where optionRootID not in (select optionRootID from [Option] where actionID < 3)
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to delete option roots w/o options', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 -------------------------------------------------
 -- underlying index betas
 truncate table IndexBeta
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating index betas: Fail to clear old betas', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end

	insert into IndexBeta (indexID, contractID, beta) 
	select 	eod_i.contractID,
		eod_u.contractID,
		eod_ib.beta
	from ETS_ASP_EOD.dbo.IndexBeta eod_ib
		inner join EodUnderlyingMap eod_i on eod_i.eodContractID = eod_ib.indexID
		inner join EodUnderlyingMap eod_u on eod_u.eodContractID = eod_ib.contractID
		inner join [StockView] sv on sv.stockID = eod_i.contractID and sv.isHedgeSymbol = 1
	where eod_ib.actionID <3 

 
	insert into IndexBeta (indexID, contractID, beta) 
	select 	eod_i.contractID,
		eod_u.contractID,
		eod_ib.beta
	from ETS_ASP_EOD.dbo.IndexBeta eod_ib
		inner join EodUnderlyingMap eod_i on eod_i.eodContractID = eod_ib.indexID
		inner join EodUnderlyingMap eod_u on eod_u.eodContractID = eod_ib.contractID
		inner join [IndexView] iv on iv.indexID = eod_i.contractID and iv.isHedgeSymbol = 1
	where eod_ib.actionID <3 

 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating index betas: Fail to insert new betas', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 -------------------------------------------------
 -- underlying skew & kurtosis
 update [Index]
 set skew = eod_i.skew,
  kurt = eod_i.kurt
 from [Index] i
  inner join EodUnderlyingMap eod_um on eod_um.contractID = i.indexID
  inner join ETS_ASP_EOD.dbo.[Index] eod_i on eod_i.indexID = eod_um.eodContractID
 where i.actionID < 3
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating skew & kurtosis: Fail to update index skew & kurtosis', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
             -----------------------------------------------------------------------------------------------------------------------------
             
    ----------------------------------------------------------------------------------
 --Updating IV bands
 
 delete VolaBands where contractID in
   (select distinct cv.contractID 
   from  ETS_ASP_EOD.dbo.ContractView eod_cv 
    inner join ContractView cv on eod_cv.symbol = cv.symbol
    inner join VolaBands vb (nolock) on cv.contractID = vb.contractID
    inner join ETS_ASP_EOD.dbo.VolaBands eod_vb on eod_cv.contractID = eod_vb.contractID)
 
            if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating IV bands: Fail to delete IV bands', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 

 insert into VolaBands (contractID, expPeriod, strikeMoneyness, IVBandLow, IVBandHi) 
 select cv.contractID, eod_vb.ExpPeriod, eod_vb.StrikeMoneyness, eod_vb.IVBandLow, eod_vb.IVBandHi
  from  ETS_ASP_EOD.dbo.ContractView eod_cv 
   inner join ContractView cv on eod_cv.symbol = cv.symbol and cv.contractTypeID = eod_cv.contractTypeID
   inner join ETS_ASP_EOD.dbo.VolaBands eod_vb on eod_cv.contractID = eod_vb.contractID
 
        if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating IV bands: Fail to update IV bands', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 -- case when the symbol type in EOD base and client base are differ. Only for stocks and indeces
 insert into VolaBands (contractID, expPeriod, strikeMoneyness, IVBandLow, IVBandHi) 
 select cv.contractID, eod_vb.ExpPeriod, eod_vb.StrikeMoneyness, eod_vb.IVBandLow, eod_vb.IVBandHi
  from  ETS_ASP_EOD.dbo.ContractView eod_cv 
   inner join ContractView cv on eod_cv.symbol = cv.symbol and cv.contractTypeID <> eod_cv.contractTypeID and cv.contractTypeID in (1,2)
   inner join ETS_ASP_EOD.dbo.VolaBands eod_vb on eod_cv.contractID = eod_vb.contractID
  where eod_cv.contractTypeID in (1,2)
 
        if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating IV bands: Fail to update IV bands', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 ----------------------------------------------------------------------------------
 

 if @iWithTran is not null commit tran @vcTranName
 
finish:
 return @error


GO


GO
/*END DATA MODIFICATION SECTION*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.210')
	begin
		update DataInfo set keyValue = '3.46.210' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.210' '3.46.210 update message:' 
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