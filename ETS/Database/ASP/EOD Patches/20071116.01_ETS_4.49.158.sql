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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.157'  or (keyValue = '4.49.158') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.158 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.158' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
--=============================================================================================

--=============================================================================================--
					--ALTER TABLE INDEX--
--=============================================================================================--
--add 7 columns for index table based on SUD-265
--index view updated
--=============================================================================================--
if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Index' and COLUMN_NAME='isDivCustom')
ALTER TABLE [Index]
	ADD [isDivCustom] [tinyint] NOT NULL DEFAULT (0)
GO

if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Index' and COLUMN_NAME='divFreq')
ALTER TABLE [Index]
	ADD [divFreq] [int] NULL DEFAULT (0)
GO

if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Index' and COLUMN_NAME='divAmt')
ALTER TABLE [Index]
	ADD [divAmt] [float] NULL
GO

if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Index' and COLUMN_NAME='divDate')
ALTER TABLE [Index]
	ADD [divDate] [smalldatetime] NULL 
GO

if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Index' and COLUMN_NAME='divFreqCustom')
ALTER TABLE [Index]
	ADD [divFreqCustom] [int] NULL DEFAULT (0)
GO

if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Index' and COLUMN_NAME='divAmtCustom')
ALTER TABLE [Index]
	ADD [divAmtCustom] [float] NULL
GO

if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Index' and COLUMN_NAME='divDateCustom')
ALTER TABLE [Index]
	ADD [divDateCustom] [smalldatetime] NULL 
GO
--=======================================================================================================================
GO
-- alter index view
ALTER       VIEW dbo.IndexView
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
			primaryExchangeID,
			isDivCustom,
			divFreq, 
			divAmt, 
			divDate,
			divAmtCustom, 
            			divDateCustom, 
			divFreqCustom
		from [Index]
		where actionID < 3

GO

--=====================================================================================--
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
					I.primaryExchangeID as iPrimaryExchangeID,
					I.isDivCustom	as tiDivCustom,
					I.DivFreq as iDivFreq,
					I.DivDate as dtDivDate,
					I.DivAmt as fDivAmt,
					I.DivAmtCustom as fDivAmtCustom,
					I.DivDateCustom as dtDivDateCustom,
					I.DivFreqCustom as iDivFreqCustom
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
					I.primaryExchangeID as iPrimaryExchangeID,
					I.isDivCustom	as tiDivCustom,
					I.DivFreq as iDivFreq,
					I.DivDate as dtDivDate,
					I.DivAmt as fDivAmt,
					I.DivAmtCustom as fDivAmtCustom,
					I.DivDateCustom as dtDivDateCustom,
					I.DivFreqCustom as iDivFreqCustom
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
					I.primaryExchangeID as iPrimaryExchangeID,
					I.isDivCustom	as tiDivCustom,
					I.DivFreq as iDivFreq,
					I.DivDate as dtDivDate,
					I.DivAmt as fDivAmt,
					I.DivAmtCustom as fDivAmtCustom,
					I.DivDateCustom as dtDivDateCustom,
					I.DivFreqCustom as iDivFreqCustom
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
					I.primaryExchangeID as iPrimaryExchangeID,
					I.isDivCustom	as tiDivCustom,
					I.DivFreq as iDivFreq,
					I.DivDate as dtDivDate,
					I.DivAmt as fDivAmt,
					I.DivAmtCustom as fDivAmtCustom,
					I.DivDateCustom as dtDivDateCustom,
					I.DivFreqCustom as iDivFreqCustom
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
					I.primaryExchangeID as iPrimaryExchangeID,
					I.isDivCustom	as tiDivCustom,
					I.DivFreq as iDivFreq,
					I.DivDate as dtDivDate,
					I.DivAmt as fDivAmt,
					I.DivAmtCustom as fDivAmtCustom,
					I.DivDateCustom as dtDivDateCustom,
					I.DivFreqCustom as iDivFreqCustom
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
						I.primaryExchangeID,
						I.DivFreq,
						I.DivDate, 
						I.DivAmt, 
						I.isDivCustom,
						I.divAmtCustom,
						I.divDateCustom,
						I.divFreqCustom
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
					I.primaryExchangeID as iPrimaryExchangeID,
					I.isDivCustom	as tiDivCustom,
					I.DivFreq as iDivFreq,
					I.DivDate as dtDivDate,
					I.DivAmt as fDivAmt,
					I.DivAmtCustom as fDivAmtCustom,
					I.DivDateCustom as dtDivDateCustom,
					I.DivFreqCustom as iDivFreqCustom
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
						I.primaryExchangeID,
						I.DivFreq,
						I.DivDate, 
						I.DivAmt, 
						I.isDivCustom,
						I.divAmtCustom,
						I.divDateCustom,
						I.divFreqCustom
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
					I.primaryExchangeID as iPrimaryExchangeID,
					I.isDivCustom	as tiDivCustom,
					I.DivFreq as iDivFreq,
					I.DivDate as dtDivDate,
					I.DivAmt as fDivAmt,
					I.DivAmtCustom as fDivAmtCustom,
					I.DivDateCustom as dtDivDateCustom,
					I.DivFreqCustom as iDivFreqCustom
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
					I.primaryExchangeID,
					I.DivFreq,
					I.DivDate, 
					I.DivAmt, 
					I.isDivCustom,
					I.divAmtCustom,
					I.divDateCustom,
					I.divFreqCustom
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
					I.primaryExchangeID as iPrimaryExchangeID,
					I.isDivCustom	as tiDivCustom,
					I.DivFreq as iDivFreq,
					I.DivDate as dtDivDate,
					I.DivAmt as fDivAmt,
					I.DivAmtCustom as fDivAmtCustom,
					I.DivDateCustom as dtDivDateCustom,
					I.DivFreqCustom as iDivFreqCustom
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
					I.primaryExchangeID,
					I.DivFreq,
					I.DivDate, 
					I.DivAmt, 
					I.isDivCustom,
					I.divAmtCustom,
					I.divDateCustom,
					I.divFreqCustom
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

--=================================================================================================================--

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
		@iExchangeID int = Null,
		@tiIsDivCustom tinyint = Null,
		@iDivFreq int = Null,
		@fDivAmt float = Null,
		@dtDivDate smalldatetime = Null,
		@fDivAmtCustom float = Null,
		@iDivFreqCustom int = Null,
		@dtDivDateCustom smalldatetime = Null
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
                                                        primaryExchangeID,
							isDivCustom,
							DivFreq,
							DivAmt,
							DivDate,
							DivFreqCustom,
							DivAmtCustom,
							DivDateCustom)
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
                                                        @iExchangeID,
							@tiIsDivCustom,
							@iDivFreq,
							@fDivAmt,
							@dtDivDate,
							@iDivFreqCustom,
							@fDivAmtCustom,
							@dtDivDateCustom)
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
					primaryExchangeID = @iExchangeID,
					isDivCustom = isnull(@tiIsDivCustom, isDivCustom),
					DivFreq = isnull(@iDivFreq, DivFreq),
					DivAmt = isnull(@fDivAmt, DivAmt),
					DivDate = isnull(@dtDivDate, DivDate),
					DivFreqCustom = isnull(@iDivFreqCustom, DivFreqCustom),
					DivAmtCustom = isnull(@fDivAmtCustom, DivAmtCustom),
					DivDateCustom = isnull(@dtDivDateCustom, DivDateCustom)
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

--=====================================================================================================================--
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER      PROCEDURE dbo.usp_EodUnderlying_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@fPriceClose float = null,
        @bSaveClosePrice bit = null,
	@vcExchangeCode varchar(20) = null

as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction

	update Contract
	    set ContractName = @vcContractName,
			MSupdateDate = getDate()
 	where contractID = @iContractID

	if @@error <> 0 
		begin 
			Raiserror ('Error. Can''t update Contract Name!', 16, 1)
			set @error = -1 
			if @@TranCount > 0 Rollback Transaction goto finish 
		end

	delete from TradeLog
	where underlyingID = @iContractID
	if @@error <> 0 
		begin 
			Raiserror ('Error. Can''t delete record from TradeLog!', 16, 1)
			set @error = -1 
			if @@TranCount > 0 Rollback Transaction goto finish 
		end

	--========================================================================
	-- make record with null exchange for this stock
	--========================================================================
	if exists(select 1 from ContractPrice where exchangeID is null and contractID = @iContractID)
	begin
	      if @bSaveClosePrice is not null and  @bSaveClosePrice = 1
              begin		
		update ContractPrice
		set priceClose = @fPriceClose
		where exchangeID is null and contractID = @iContractID
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update close price for default exchange!', 16, 1)
				set @error = -1 
				if @@TranCount > 0 Rollback Transaction goto finish
			end
              end 
	end
	else
	begin
	      if @bSaveClosePrice is not null and  @bSaveClosePrice = 1
		insert into ContractPrice(contractID, exchangeID, priceClose)
			values(@iContractID, null, @fPriceClose)
	      else
		insert into ContractPrice(contractID, exchangeID, priceClose)
			values(@iContractID, null, null)
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t insert default exchange!', 16, 1)
				set @error = -1 
				if @@TranCount > 0 Rollback Transaction goto finish
			end
	end
	--==================================================================================
	-- update DivAmnt & DivFreq values
	--==================================================================================
	--==================================================================================
	-- update DivAmnt & DivFreq values
	--==================================================================================
	declare @exchangeID as int
	if (@bIsStock <> 0 and exists(select 1 from Stock where stockID=@iContractID))
		begin 
			set @exchangeID = null
			if (@vcExchangeCode is not null)
			begin
				set @exchangeID = (select TOP 1 e.exchangeID from Exchange e where @vcExchangeCode = exchangeCode)
			end

			Declare @df smallint
			update Stock 
				set divAmt = @fDivAmt,
					divDate = @dtDivDate, 
					divFreq = @iDivFreq,
					primaryExchangeID = isNull(@exchangeID,primaryExchangeID)
			where stockID = @iContractID
			if (@@error <> 0)
				begin
					Raiserror ('Error. Can''t update Dividends and Primary Exchange!', 16, 1)
					set @error = -1 
					if @@TranCount > 0 Rollback Transaction goto finish 
				end
		end
	
	--index dividend structure update
	if (exists(select 1 from [Index] where indexID = @iContractID))
	begin
		set @exchangeID = null
		if (@vcExchangeCode is not null)
		begin
			set @exchangeID = (select TOP 1 e.exchangeID from Exchange e where @vcExchangeCode = exchangeCode)
		end

		update [Index] 
		set 	divAmt = @fDivAmt,
			divDate = @dtDivDate, 
			divFreq = @iDivFreq,
			primaryExchangeID = isNull(@exchangeID,primaryExchangeID)
		where indexID = @iContractID
		if (@@error <> 0)
		begin
			Raiserror ('Error. Can''t update Dividends for Index!', 16, 1)
			set @error = -1 
			if @@TranCount > 0 Rollback Transaction goto finish 
		end	
	end

	Commit Transaction
    set @error = 0
finish:
	Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--==============================================================================================================--
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



ALTER      proc usp_Stock_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@vcExchangeCode varchar(20) = null,
	@fPriceAsk float = -1, -- fPriceAsk < 0 ????????, ??? ????????? ????????? ????????????
	@fPriceBid float = 0,
	@fPriceOpen float = 0,
	@fPriceClose float = 0,
	@fPriceLow float = 0,
	@fPriceHigh float = 0,
	@iVolume int = 0,
	@iExchangeID int = null  --the ExchangeID special for save data into ContractPrice table
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		update Contract
		    set ContractName = @vcContractName,
				MSupdateDate = getDate()
	 	where contractID = @iContractID
	
		delete from TradeLog
		where underlyingID = @iContractID

		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update Contract Name!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

		if @fPriceAsk >= 0
			begin
			 if @iExchangeID is null
                            begin	
				update ContractPrice 
					set priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					PriceOpen = @fPriceOpen,
					PriceClose = @fPriceClose,
					PriceLow = @fPriceLow,
					PriceHigh = @fPriceHigh,
					Volume = @iVolume
				where contractID = @iContractID 
			    end
                            else
                            begin	
				update ContractPrice 
					set priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					PriceOpen = @fPriceOpen,
					PriceClose = @fPriceClose,
					PriceLow = @fPriceLow,
					PriceHigh = @fPriceHigh,
					Volume = @iVolume
				where contractID = @iContractID and exchangeID=@iExchangeID
			    end

				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update Contract Price!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
			end

		--========================================================================
		-- make record with null exchange for this stock
		--========================================================================
		if not exists(select * from ContractPrice where exchangeID is null and contractID = @iContractID)
			begin
				insert into ContractPrice(contractID,exchangeID)
					values(@iContractID,null)
				if (@@error <> 0) begin rollback tran return(-1) end           
			end 

		declare @exchangeID as int
		set @exchangeID = null
		if (@vcExchangeCode is not null)
		begin
			set @exchangeID = (select TOP 1 e.exchangeID from Exchange e where @vcExchangeCode = exchangeCode and actionID < 3)
		end		
		--==================================================================================
		-- update DivAmnt & DivFreq values
		--==================================================================================
		if (@bIsStock <> 0 and exists(select * from stock where stockID=@iContractID))
			begin 
				Declare @df smallint
				update Stock 
					set divAmt = @fDivAmt,
						divDate = @dtDivDate, 
						divFreq = @iDivFreq,
						primaryExchangeID = isNull(@exchangeID,primaryExchangeID)
				where stockID = @iContractID
				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update DivAmnt and DivDate and primaryExchangeID!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
			end
		--==================================================================================
		if (exists(select * from [Index] where indexID=@iContractID))
			begin 
				update [Index]
					set divAmt = @fDivAmt,
						divDate = @dtDivDate, 
						divFreq = @iDivFreq,
						primaryExchangeID = isNull(@exchangeID,primaryExchangeID)
				where indexID = @iContractID
				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update DivAmnt and DivDate and primaryExchangeID for Index!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
			end
	Commit Transaction
    set @error = 0
finish:
	if @error<>0 
		Return(@error)
	else
		Return @iContractID



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--===============================================================================================================

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO




ALTER      PROCEDURE dbo.usp_ContractType_Change 
	@iContractID int
AS
/*
	Created by: Zaur
	Description: this procedure created with template, desined by Zaur Nuraliev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iContractTypeID int 
		select @iContractTypeID = contractTypeID from Contract where contractID = @iContractID

	if @iContractTypeID > 2 
		begin
			Raiserror('Can''t perform changes with this contract!',16,1)
		end

	Begin Transaction t1
		if @iContractTypeID = 1 
		--change the contract type from index to stock
			begin
				delete from IndexCorrelation where indexID1 = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from IndexCorrelation where indexID2 = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from IndexDataHistory where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from IndexDefinition where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from IndexBeta where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from SyntheticRootIndexBeta where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				insert into Stock(stockID) values(@iContractID)
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				update 
					Stock
				set
					divAmt = idx.divAmt,
					divFreq = idx.divFreq,
					divDate = idx.divDate,
					divDateCustom = idx.divDateCustom,
					divAmtCustom = idx.divAmtCustom,
					divFreqCustom = idx.divFreqCustom,
					primaryExchangeID = idx.primaryExchangeID
				from
					[Index] idx, Stock s
				where
					idx.IndexID = @iContractID And s.StockID = @iContractID

				delete from [Index] where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			end
		else if @iContractTypeID = 2 
		--change the contract type from stock to index
			begin
				delete from StockDataHistory where stockID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				delete from StockCorrelation where stockID1 = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				delete from StockCorrelation where stockID2  = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				delete from IndexDefinition where stockID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				insert into [Index](indexID) values(@iContractID)
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
				
				update 
					[Index]
				set
					divAmt = s.divAmt,
					divFreq = s.divFreq,
					divDate = s.DivDate,
					divDateCustom = s.divDateCustom,
					divAmtCustom = s.divAmtCustom,
					divFreqCustom = s.divFreqCustom,
					primaryExchangeID = s.primaryExchangeID
				from
					[Index] idx, Stock s
				where
					idx.IndexID = @iContractID And s.StockID = @iContractID

				delete from Stock where stockID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			end
			--change the contractType

			if @iContractTypeID = 1 
				set @iContractTypeID = 2 
			else 
				set @iContractTypeID = 1

			update Contract 
				set contractTypeID = @iContractTypeID 
				where contractID = @iContractID
			if @@error != 0 begin if @@TranCount != 0 Rollback Tran t1 goto finish end

	Commit Transaction t1

    set @error = 0
finish:
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--===========================================================================================================
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
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
   case when i.indexID is null then s.divFreq else i.divFreq end as iDivFreq,
   case when i.indexID is null then s.divDate else i.divDate end as dtDivDate,
   case when i.indexID is null then s.divAmt else i.divAmt end as fDivAmt,
   case when i.indexID is null then s.isDivCustom else i.isDivCustom end as tiIsDivCustom,
   case when i.indexID is null then s.divFreqCustom else i.divFreqCustom end as iDivFreqCustom,
   case when i.indexID is null then s.divDateCustom else i.divDateCustom end as dtDivDateCustom,
   case when i.indexID is null then s.divAmtCustom else i.divAmtCustom end as fDivAmtCustom,
      c.priceClose as fPriceClose,
      null as iTraderID,
   case when i.indexID is null then s.skew else i.skew end as fSkew,
   case when i.indexID is null then s.kurt else i.kurt end as fKurt,
      c.contractName as vcContractName,
      c.undPriceProfileID as iUndPriceProfileID,
      c.optPriceProfileID as iOptPriceProfileID,
   case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,
      c.volume10Day as iVolume10Day,
      c.volume5Expiration as iVolume5Expiration,
         case when i.IndexID is null then e.exchangeCode else ei.exchangeCode end as exchangeCode,
   case when i.IndexID is null then isnull(s.primaryExchangeID,0) else isnull(i.primaryExchangeID,0) end as iPrimaryExchangeID
   from ContractPriceView c 
      --inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
      left join StockView s on s.stockID = c.contractID
      left join IndexView i on i.indexID = c.contractID
      left join Exchange e on e.exchangeID = s.primaryExchangeID
      left join Exchange ei on ei.exchangeID = i.primaryExchangeID
  where
   (c.contractTypeID < 3 or c.contractTypeID = 6) AND c.ActionId < 3
  order by c.Symbol
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
   case when i.indexID is null then s.divFreq else i.divFreq end as iDivFreq,
   case when i.indexID is null then s.divDate else i.divDate end as dtDivDate,
   case when i.indexID is null then s.divAmt else i.divAmt end as fDivAmt,
   case when i.indexID is null then s.isDivCustom else i.isDivCustom end as tiIsDivCustom,
   case when i.indexID is null then s.divFreqCustom else i.divFreqCustom end as iDivFreqCustom,
   case when i.indexID is null then s.divDateCustom else i.divDateCustom end as dtDivDateCustom,
   case when i.indexID is null then s.divAmtCustom else i.divAmtCustom end as fDivAmtCustom,
      c.priceClose as fPriceClose,
      tc.traderID as iTraderID,
   case when i.indexID is null then s.skew else i.skew end as fSkew,
   case when i.indexID is null then s.kurt else i.kurt end as fKurt,
      c.contractName as vcContractName,
      c.undPriceProfileID as iUndPriceProfileID,
      c.optPriceProfileID as iOptPriceProfileID,
   case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,
      c.volume10Day as iVolume10Day,
      c.volume5Expiration as iVolume5Expiration,
   case when i.IndexID is null then e.exchangeCode else ei.exchangeCode end as exchangeCode,
   case when i.IndexID is null then isnull(s.primaryExchangeID,0) else isnull(i.primaryExchangeID,0) end as iPrimaryExchangeID
  from ContractPriceView c 
     -- inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
	left join StockView s on s.stockID = c.contractID
	left join IndexView i on i.indexID = c.contractID
	left join Exchange e on e.exchangeID = s.primaryExchangeID
	left join Exchange ei on ei.exchangeID = i.primaryExchangeID
	left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
  where
   (c.contractTypeID < 3 or c.contractTypeID = 6) AND c.ActionId < 3
  order by c.Symbol
 return @@error


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--===============================================================================================================


--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.158')
	begin
		update DataInfo set keyValue = '4.49.158' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.158' '4.49.158 update message:' 
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
