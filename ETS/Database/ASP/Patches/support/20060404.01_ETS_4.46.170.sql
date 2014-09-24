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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.169'  or (keyValue = '3.46.170') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.170 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.170' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Index' and COLUMN_NAME='SOQ')
ALTER TABLE dbo.[Index] ADD
	SOQ float NOT NULL,
	CONSTRAINT DEF_Index_SOQ DEFAULT(0.0) for SOQ
GO

GO
ALTER   VIEW dbo.IndexView
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
			SOQ
		from [Index]
		where actionID < 3
GO


GO
ALTER   PROC dbo.usp_MmUnderlying_Get
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
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration,
		    i.SOQ As SOQ
		from ContractView c 
		    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		where
			c.contractTypeID < 3
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
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration,
		    i.SOQ As SOQ
		from ContractView c 
		    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
		where
			c.contractTypeID < 3

	return @@error
GO


GO
ALTER  PROCEDURE dbo.usp_Index_Save
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
		@iOptPriceProfileID int = null output
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
		if @iExpCalendarID is null or @iExpCalendarID = 0
			set @iExpCalendarID = 2
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
                  values (1, @vcSymbol, @vcContractName, @iExpCalendarID, @iUndPriceProfileID, @iOptPriceProfileID)

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
							SOQ)
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
							@SOQ)
				 if (@@error <> 0)
					begin
					   RAISERROR ('Error. Can''t insert into [Index]!', 16, 1)
					   ROLLBACK TRAN
					   RETURN (-1)
					end 			
		 commit tran
	 end
   else
	 begin
		begin tran
		   update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@vcContractName,contractName),
					expCalendarID = isNull(@iExpCalendarID, expCalendarID),
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
					SOQ = isnull(@SOQ,SOQ)
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


if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmExpiredPositions_Check]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
   drop procedure [dbo].[usp_MmExpiredPositions_Check]
GO


GO
create procedure dbo.usp_MmExpiredPositions_Check
-- Created by Artem Shylaev Egar T
-- 2005 - 08 - 30
  @vOptWithPsExp As int output
As
   select Cn.symbol, Op.optionid As OptionID , T.TotalSum  As TotalSum , ec.expirydate 
	from Contract Cn
	join ( select 
		contractid ,
		underlyingid ,
		sum( case isbuy when 0 then quantity else -quantity end ) As TotalSum
		from TradeView
			where  status <> 1
		group by contractid  , underlyingid ) T on ( Cn.contractid = T.contractid )
	inner join Contract uc on T.underlyingID = uc.contractID
	join [option]  Op on Cn.contractid = Op.optionid
	left join ExpiryInCalendarView ec on Op.expiryID = ec.expiryID and ec.expCalendarID = isnull ( Cn.expCalendarID, uc.expCalendarID)
	where datediff ( dd , ec.expirydate , getdate() ) >= 0 -- and Op.actionid < 3 
		and T.TotalSum  <> 0 and Cn.contracttypeid = 3 
   union select Cn.symbol, Op.futureoptionid As OptionID, T.TotalSum  As TotalSum , ec.expirydate 
	from Contract Cn
	join ( select 
		contractid ,
		underlyingid ,
		sum( case isbuy when 0 then quantity else -quantity end ) As TotalSum
		from TradeView
			where  status <> 1
		group by contractid  , underlyingid ) T on ( Cn.contractid = T.contractid )
	inner join Contract uc on T.underlyingID = uc.contractID
	join [FutureOption]  Op on Cn.contractid = Op.futureoptionid
	left join ExpiryInCalendarView ec on Op.expiryID = ec.expiryID and ec.expCalendarID = isnull ( Cn.expCalendarID, uc.expCalendarID)
	where datediff ( dd , ec.expirydate , getdate() ) >= 0 -- and Op.actionid < 3 
		and T.TotalSum  <> 0 and Cn.contracttypeid = 5 

   select @vOptWithPsExp = @@RowCount
GO


GO
ALTER     PROC dbo.usp_Trade_Exec
	@tiExercise tinyint,
	@iOptTradeID int,
	@iOptQuantity int,
	@tiOptIsBuy tinyint,
	@tiOptStatus tinyint,
	@fOptBrokerCommission float,
	@fOptClearingBrokerCommission float,
	@iUndQuantity int,
	@fUndPrice float,
	@tiUndIsBuy tinyint,
	@tiUndStatus tinyint,
	@fUndBrokerCommission float,
	@fUndClearingBrokerCommission float,
	@tiIsFutures tinyint,
	@priceForIndexAndExer float ,
	@tiIsIndex tinyint = 1 ,  
	@iNewOptSeqNum int = null out,
	@iNewOptTradeID int = null out,
	@dtNewOptTradeDate datetime = null out,
	@iNewUndSeqNum int = null out,
	@iNewUndTradeID int = null out,
	@dtNewUndTradeDate datetime = null out
as
	set nocount on
-- A Sh 02 - 09 - 2005	
	set transaction isolation level serializable

	begin tran

	declare @bSucceed bit
		
	set @bSucceed = 1

	declare @iDeltaGMT int

	exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

	select @iNewOptTradeID = max(tradeID) + 1 from Trade
	if @iNewOptTradeID is null set @iNewOptTradeID = 1

	select @iNewOptSeqNum = isnull(max(seqNum), 0) + 1 from Trade

	if @dtNewOptTradeDate is null	
		set @dtNewOptTradeDate = dateadd(minute, @iDeltaGMT, getdate())

	insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
		price, isBuy, traderID, strategyID, brokerID, brokerCommission,
		clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
		isPosition, status, execID, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	select @iNewOptTradeID, 1, contractID, @dtNewOptTradeDate, @iOptQuantity, @priceForIndexAndExer , @tiOptIsBuy,
		traderID, strategyID, brokerID, @fOptBrokerCommission, clearingBrokerID, @fOptClearingBrokerCommission,
		tradedIV, spotReference, 0, @tiOptStatus, null, @iNewOptSeqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID
	from Trade
	where tradeID = @iOptTradeID and actionID = 1

	if @@error <> 0 set @bSucceed = 0

	if @bSucceed = 1 and isnull(@tiExercise, 0) <> 0 and @tiIsIndex <> 1
	begin
		select @iNewUndTradeID = max(tradeID) + 1 from Trade
			if @iNewUndTradeID is null set @iNewUndTradeID = 1
	
		select @iNewUndSeqNum = isnull(max(seqNum), 0) + 1 from Trade
	
		if @dtNewUndTradeDate is null	
			set @dtNewUndTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	
		if isnull(@tiIsFutures, 0) = 0
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID)
			select @iNewUndTradeID, 1, underlyingID, @dtNewUndTradeDate, @iUndQuantity, @fUndPrice, @tiUndIsBuy,
				traderID, strategyID, brokerID, @fUndBrokerCommission, clearingBrokerID, @fUndClearingBrokerCommission,
				0, 0, 0, @tiUndStatus, null, @iNewUndSeqNum, mark,
				underlyingID, null, null, null
			from Trade
			where tradeID = @iOptTradeID and actionID = 1
		else
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
				price, isBuy, traderID, strategyID, brokerID, brokerCommission,
				clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
				isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID)
			select @iNewUndTradeID, 1, futureID, @dtNewUndTradeDate, @iUndQuantity, @fUndPrice, @tiUndIsBuy,
				traderID, strategyID, brokerID, @fUndBrokerCommission, clearingBrokerID, @fUndClearingBrokerCommission,
				0, 0, 0, @tiUndStatus, null, @iNewUndSeqNum, mark,
				underlyingID, null, futureRootID, futureID
			from Trade
			where tradeID = @iOptTradeID and actionID = 1
	
		if @@error <> 0 set @bSucceed = 0
	end

	if @bSucceed = 1
	begin
		commit tran
		if isnull(@tiExercise, 0) <> 0
			return @iNewUndSeqNum
		else
			return @iNewOptSeqNum
	end
	else  
	begin
		rollback tran
		set @iNewOptSeqNum = null
		set @iNewOptTradeID = null
		set @dtNewOptTradeDate = null
		set @iNewUndSeqNum = null
		set @iNewUndTradeID = null
		set @dtNewUndTradeDate = null
		return 0
	end
GO


GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.170')
	begin
		update DataInfo set keyValue = '3.46.170' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.170' '3.46.170 update message:' 
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