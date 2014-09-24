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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.217'  or (keyValue = '3.46.218') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.218 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.218' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



ALTER      PROC usp_MmUnderlying_Get
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
		    i.SOQ As SOQ,
		    	case when i.IndexID is null then isnull(s.primaryExchangeID,0) else 0 end as iPrimaryExchangeID
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
		    i.SOQ As SOQ,
		    	case when i.IndexID is null then isnull(s.primaryExchangeID,0) else 0 end as iPrimaryExchangeID
		from ContractView c 
		    left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
		where
			c.contractTypeID in (1,2,6) and not c.Symbol is null

	return @@error



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




ALTER    PROCEDURE dbo.usp_Stock_Save
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
						primaryExchangeID = @iExchangeID,
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='Index' AND COLUMN_NAME='primaryExchangeID')
ALTER TABLE dbo.[Index] ADD primaryExchangeID INT NULL
CONSTRAINT [FK_Index_Exchanges] FOREIGN KEY 
	(
		[primaryExchangeID]
	) REFERENCES [Exchange] (
		[exchangeID]
	)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER     VIEW dbo.IndexView
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
			isHedgeSymbol,
			primaryExchangeID
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





ALTER    PROCEDURE dbo.usp_Index_Get
	@iGroupID int = Null,
	@iIndexID int = Null,
	@tiWithPosition tinyint = 0,
	@iTraderID int = null
  as
	if isnull(@tiWithPosition, 0) = 0

	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID,
					I.primaryExchangeID as iPrimaryExchangeID
				from ContractView C 	
					inner join IndexView I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
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
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID,
					I.primaryExchangeID as iPrimaryExchangeID
				from ContractView C 	
					inner join IndexView I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where C.contractTypeID=1
					and tc.traderID = @iTraderID
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
				order by C.symbol
		end
		else
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID,
					I.primaryExchangeID as iPrimaryExchangeID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join IndexView I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
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
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID,
					I.primaryExchangeID as iPrimaryExchangeID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join IndexView I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and tc.traderID = @iTraderID
				order by C.symbol
		end
	end
	else
	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID,
					I.primaryExchangeID as iPrimaryExchangeID
				from ContractView c 	
					inner join IndexView I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
				group by C.contractID, C.contractTypeID, 
						C.symbol, C.contractName, 
						I.yield, I.isHTB, I.actionDate, 
						I.actionID,	I.CalcOptionType,
						I.CalcModelType, c.expCalendarID, I.isBasket, 
						I.isActive,	I.skew,	I.kurt, c.undPriceProfileID, c.optPriceProfileID,
						I.primaryExchangeID
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
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID,
					I.primaryExchangeID as iPrimaryExchangeID
				from ContractView c 	
					inner join IndexView I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where C.contractTypeID=1
					and tc.traderID = @iTraderID
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
				group by C.contractID,
						C.contractTypeID, C.symbol, C.contractName,
						I.yield, I.isHTB, I.actionDate, 
						I.actionID,I.CalcOptionType,
						I.CalcModelType,c.expCalendarID,
						I.isBasket, I.isActive,	I.skew,	I.kurt, c.undPriceProfileID, c.optPriceProfileID,
						I.primaryExchangeID
				order by C.symbol
		end
		else
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID,
					I.primaryExchangeID as iPrimaryExchangeID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join IndexView I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
					I.yield, I.isHTB, I.actionDate, I.actionID,I.CalcOptionType,
					I.CalcModelType,c.expCalendarID,I.isBasket, I.isActive,	I.skew, I.kurt, c.undPriceProfileID, c.optPriceProfileID,
					I.primaryExchangeID
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
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID,
					I.primaryExchangeID as iPrimaryExchangeID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join IndexView I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and tc.traderID = @iTraderID
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
					I.yield, I.isHTB, I.actionDate, I.actionID,I.CalcOptionType,
					I.CalcModelType,c.expCalendarID,I.isBasket, I.isActive,
					I.skew,I.kurt, c.undPriceProfileID, c.optPriceProfileID,
					I.primaryExchangeID
				order by C.symbol
		end
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



ALTER     PROCEDURE usp_Index_Save
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
		@bIsHedgeSymbol  tinyint = Null,
		@iExchangeID int = Null
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
							isHedgeSymbol,
							primaryExchangeID)
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
							@bIsHedgeSymbol,
							@iExchangeID)
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
					isHedgeSymbol = isnull(@bIsHedgeSymbol, isHedgeSymbol),
					primaryExchangeID = isnull(@iExchangeID, primaryExchangeID)
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




ALTER       PROC usp_MmUnderlying_Get
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
		    i.SOQ As SOQ,
		    	case when i.IndexID is null then isnull(s.primaryExchangeID,0) else isnull(i.primaryExchangeID,0) end as iPrimaryExchangeID
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
		    i.SOQ As SOQ,
		    	case when i.IndexID is null then isnull(s.primaryExchangeID,0) else isnull(i.primaryExchangeID,0) end as iPrimaryExchangeID
		from ContractView c 
		    left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
		where
			c.contractTypeID in (1,2,6) and not c.Symbol is null

	return @@error




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.218')
	begin
		update DataInfo set keyValue = '3.46.218' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.218' '3.46.218 update message:' 
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
