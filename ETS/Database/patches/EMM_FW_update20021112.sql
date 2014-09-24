/*
	fix for MLine00000757
	DB version for update: 3.38.042
	DB version in patch: 3.38.043
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.38.042' or (keyValue = '3.38.043')))
	BEGIN
		PRINT 'You can''t run this script, because you have not run script for previous version!'
		Select cast(keyValue as varchar(25)) as 'DB version', '3.38.043' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
--============================================================
-- fix for MLine00000757 
--============================================================
	delete Trade 
	from Trade T
		join Contract C on T.contractID = C.contractID 
	where C.actionID = 3 and T.actionID=1 and isSimulated = 1
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
--============================================================
-- recreation of non dbo SP's
--============================================================
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BookRule_GetAll]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BookRule_GetAll]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ClearingHouseTradeByCacheID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ClearingHouseTradeByCacheID_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ClearingHouseTrade_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ClearingHouseTrade_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ClearingHouseTrade_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ClearingHouseTrade_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CreateDefaultVolaSurface_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CreateDefaultVolaSurface_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_LastCacheID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_LastCacheID_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Position_ByUnderlyingForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Position_ByUnderlyingForRisk_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeForRisk_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeForRisk_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE procedure dbo.usp_BookRule_GetAll
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	select B.BookName as PortfolioName,
			BRI.ItemName , 
			M.MaskName as Mask, 
			T.itemTypeName as TypeName
			from BookView b
				join BookRuleItem BRI on B.bookID = BRI.bookID
				join RuleItemType T on T.ruleItemTypeID = BRI.ruleItemTypeID
				join BookRuleItemMask M on BRI.bookRuleItemID = M.bookRuleItemID
			order by B.Priority

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

Create procedure dbo.usp_ClearingHouseTradeByCacheID_Get
 	@iCacheID int
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare @Now int
		exec usp_CurrentDate_Get @Now output

	select * 
		from ClearingHouseTrade 
		where iDateOfTrade = @Now and iCacheID = @iCacheID

	set @error = @@error

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

Create procedure dbo.usp_ClearingHouseTrade_Get
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @Now integer
		exec usp_CurrentDate_Get @Now output
	select * from ClearingHouseTrade where iDateOfTrade = @Now
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

Create procedure dbo.usp_ClearingHouseTrade_Save
				 	@iCacheID     int ,
					@ins_upd_del_flag tinyint,
					@put_call	 tinyint,
					@buy_sell tinyint,
					@origin   tinyint,
					@open_close tinyint,
					@trade_id   varchar(10),
					@user_id    varchar(10),
					@trading_sym varchar(7),
					@base_sym	varchar(7),
					@parent_account varchar(11),
					@account varchar(11),
					@q_account varchar(11),
					@security_type varchar(3),
					@security_subtype varchar(3),
					@clearing_firm varchar(5),
					@cusip varchar(10),
					@basket_id varchar(6),
					@giveup_firm varchar(5),	
					@contra_firm varchar(5),
					@exec_broker varchar(4),
					@contra_broker varchar(4),
					@mmcs_sub_acct varchar(3),
					@exercise_multiplier float(53),
					@extended_premium float(53),
					@premium float(53),
					@strike float(53),
					@price float(53),
					@quantity int,
					@tag int,
					@expiration_date int,
					@execution_date int,
					@last_upd_date int,
					@contract_date int,
					@source_system int ,
					@trade_id_num int ,
					@trade_group_id int ,
					@settlement_date int,
					@entry_firm varchar(5),
					@matched_flag tinyint,
					@spread_ind tinyint,
					@premium_multiplier float(53),
					@source_code varchar(3),
					@parent_acct_intrl_id varchar(14),
					@acct_intrl_id varchar(14),
					@q_acct_intrl_id varchar(14),
					@cap_interval float(53),
					@trade_exchange int ,
					@settled_unsettled_ind tinyint,
					@stock_origin tinyint,
					@mm_card_number varchar(8),
					@basket_tag int ,
					@basket_price float(53),
					@short_sale tinyint,
					@commission_code varchar(3),
					@commission float(53),
					@same_day_exer tinyint,
					@terminal_name varchar(11),
					@portfolio_name	varchar(255)
as
	Declare @Now integer
		exec usp_CurrentDate_Get @Now output
if exists (select * from ClearingHouseTrade where iDateOfTrade = @Now and iCacheID = @iCacheID)
	Return(0)

insert into ClearingHouseTrade values
(
	@Now,
	@iCacheID,
	@ins_upd_del_flag,
	@put_call	 ,
	@buy_sell ,
	@origin  , 
	@open_close ,
	@trade_id   ,
	@user_id    ,
	@trading_sym ,
	@base_sym	,
	@parent_account ,
	@account ,
	@q_account ,
	@security_type ,
	@security_subtype ,
	@clearing_firm ,
	@cusip ,
	@basket_id ,
	@giveup_firm ,	
	@contra_firm ,
	@exec_broker ,
	@contra_broker ,
	@mmcs_sub_acct ,
	@exercise_multiplier ,
	@extended_premium ,
	@premium ,
	@strike ,
	@price ,
	@quantity ,
	@tag,
	@expiration_date ,
	@execution_date ,
	@last_upd_date ,
	@contract_date ,
	@source_system ,
	@trade_id_num ,
	@trade_group_id ,
	@settlement_date ,
	@entry_firm ,
	@matched_flag ,
	@spread_ind ,
	@premium_multiplier ,
	@source_code ,
	@parent_acct_intrl_id ,
	@acct_intrl_id ,
	@q_acct_intrl_id ,
	@cap_interval ,
	@trade_exchange ,
	@settled_unsettled_ind ,
	@stock_origin ,
	@mm_card_number ,
	@basket_tag ,
	@basket_price ,
	@short_sale ,
	@commission_code ,
	@commission ,
	@same_day_exer ,
	@terminal_name,
	@portfolio_name
)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE procedure dbo.usp_CreateDefaultVolaSurface_Save
	@iContractID int
as
/*
	Created by Zaur Nuraliev
	Description: this procedure create default surface by ContractID
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iVolaSurfaceDataID int
	if not exists(select * from VolaSurfaceDataView 
					where contractID=@iContractID and isDefaultSurface = 1) and @iContractID is not null
		begin
			declare @fPrice float
		

/*			select Top 1 @fPrice = 
				case 
					when (priceBid is not null and priceAsk is null) then priceBid
					when (priceBid is null and priceAsk is not null) then priceAsk
					when (priceBid is not null and priceAsk is not null) then (priceAsk + priceBid)/2
					when (priceBid is null and priceAsk is null) then priceLast
					when (priceBid is null and priceAsk is null and priceLast is null) then priceClose
				end
					from ContractPriceHistory 
					where contractID = @iContractID
					order by actionDate desc */
			
			select @fPrice = 
				case 
					when (priceBid is not null and priceAsk is null) then priceBid
					when (priceBid is null and priceAsk is not null) then priceAsk
					when (priceBid is not null and priceAsk is not null) then (priceAsk + priceBid)/2
					when (priceBid is null and priceAsk is null) then priceLast
					when (priceBid is null and priceAsk is null and priceLast is null) then priceClose
				end
					from ContractPrice
					where contractID = @iContractID and exchangeID is null 

			insert into VolaSurfaceData(contractID,isDefaultSurface,baseUnderlinePrice)
				values (@iContractID,1,isnull(@fPrice,0))
			select @iVolaSurfaceDataID = @@identity
		end
	else 
		select @iVolaSurfaceDataID = VolaSurfaceDataID 
			from VolaSurfaceDataView 
			where contractID=@iContractID and isDefaultSurface = 1 and @iContractID is not null

Return(@iVolaSurfaceDataID)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

Create procedure dbo.usp_LastCacheID_Get
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @Now integer
		exec usp_CurrentDate_Get @Now output
	select count(*) from ClearingHouseTrade where iDateOfTrade = @Now
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

CREATE Procedure dbo.usp_Position_ByUnderlyingForRisk_Get
        @iContractID int,
        @iBookID int = Null,
        @iStrategyID int = Null,
        @iTraderID int = Null,
        @dtTradeDate datetime = Null,
        @tiIsClose tinyint = Null
        
as
begin
     set nocount on
     select t.contractID, 'S' 'IsStock', SUM(t.Price * t.quantity) 'Total Price', SUM(t.quantity) 'Total quantity'
       from TradeView t
      where t.contractID = @iContractID
        and (@iBookID is Null or t.bookID = @iBookID)
        and (@iStrategyID is Null or t.strategyID = @iStrategyID)
        and (@iTraderID is Null or t.traderID = @iTraderID)
        and (@dtTradeDate is Null or t.TradeDate = @dtTradeDate)
        and (@tiIsClose is Null or t.IsClose = @tiIsClose)
      group by t.contractID
     union all
    select t.contractID, 'O' 'IsStock', SUM(t.Price * t.quantity) 'Total Price', SUM(t.quantity) 'Total quantity'
       from OptionView O, Trade T, OptionRoot ORt
      where ORt.optionRootID=O.optionRootID and ORt.underlyingID = @iContractID
        and t.contractID = o.optionID
        and (@iBookID is Null or t.bookID = @iBookID)
        and (@iStrategyID is Null or t.strategyID = @iStrategyID)
        and (@iTraderID is Null or t.traderID = @iTraderID)
        and (@dtTradeDate is Null or t.TradeDate = @dtTradeDate)
        and (@tiIsClose is Null or t.IsClose = @tiIsClose)
      group by t.contractID
    set nocount off
end

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE PROCEDURE dbo.usp_TradeFeed_Get
	@vcExecID varchar(20),
	@dtTradeDate datetime
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is save information into table_name table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	select
		tr.seqNum as iSeqNum,
		tr.actionDate as dtActionDate,
		tr.tradeID as iTradeID,
		tr.actionID as tiActionID,
		tr.isClose as tiIsClose,
		tr.tradeDate as dtTradeDate,
		tr.quantity as iQuantity,
		tr.price as fPrice,
		tr.isBuy as tiIsBuy,
		tr.bookID as iBookID,     
		tr.strategyID as iStrategyID, 
		tr.traderID as iTraderID,
		tr.brokerID as iBrokerID,
		tr.clearingBrokerID as iClearingBrokerID,
		tr.BrokerCommissionID as iBrokerCommissionID,
		tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
		cv1.CommissionValue as fBrokerCommission,
		cv2.CommissionValue as fClearingBrokerCommission,
		tr.tradedIV as fTradedIV,
		tr.spotReference as fSpotReference,
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,
		i.yield as fYield,
		null as dtExpiry,
		null as fStrike,
		null as bIsCall,
		c.contractID as iUnderlyingContractID,
		c.contractTypeID as iUnderlyingContractTypeID,
		c.Symbol as vcUnderlyingSymbol,
		null as fPriceTheoClose,
		tr.isPosition as tiIsPosition,
		tr.isSimulated as tiIsSimulated,
		null as fPriceClose,
		null as fUnderlyingPriceClose
	from Trade tr 
		join Contract C  on tr.contractID = c.contractID
		join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		left join StockView s on c.contractID = s.stockID
		left join IndexView i on c.contractID = i.indexID
		left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
		left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
	where tr.actionID > 0 and tr.execID = @vcExecID and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tr.tradeDate,112)

union

	select
		tr.seqNum as iSeqNum,
		tr.actionDate as dtActionDate,
		tr.tradeID as iTradeID,
		tr.actionID as tiActionID,
		tr.isClose as tiIsClose,
		tr.tradeDate as dtTradeDate,
		tr.quantity as iQuantity,
		tr.price as fPrice,
		tr.isBuy as tiIsBuy,
		tr.bookID as iBookID,     
		tr.strategyID as iStrategyID, 
		tr.traderID as iTraderID,
		tr.brokerID as iBrokerID,
		tr.clearingBrokerID as iClearingBrokerID,
		tr.BrokerCommissionID as iBrokerCommissionID,
		tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
		cv1.CommissionValue as fBrokerCommission,
		cv2.CommissionValue as fClearingBrokerCommission,
		tr.tradedIV as fTradedIV,
		tr.spotReference as fSpotReference,
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		s.divFreq as iDivFreq,
		s.divDate as dtDivDate,
		s.divAmt as fDivAmt,
		i.yield as fYield,
		EC.expiryDate as dtExpiry,
		o.strike as fStrike,
		o.isCall as bIsCall,
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		cp.priceTheo as fPriceTheoClose,
		tr.isPosition as tiIsPosition,
		ort.lotSize as iLotSize,
		tr.isSimulated as tiIsSimulated,
		cp.PriceClose as fPriceClose,
		ucp.priceClose as fUnderlyingPriceClose
	from Trade tr 
		join Contract C  on tr.contractID = c.contractID
		inner join [Option] O on C.contractID = O.optionID
		join OpriotRoot ort on ort.optionRootID = o.optionRootID
		inner join Contract UC on ort.underlyingID = UC.contractID
		inner join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
		inner join ContractPrice UCP on ort.underlyingID = ucp.contractID and ucp.exchangeID is null
		join ExpiryInCalendarView EC on UC.expCalendarID = EC.expCalendarID and O.expiryID = EC.expiryID
		left join StockView s on uc.contractID = s.stockID
		left join [IndexView] i on uc.contractID = i.indexID
		left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
		left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
	where tr.actionID > 0 and tr.execID = @vcExecID and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tr.tradeDate,112)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE Procedure dbo.usp_TradeForRisk_Get
        @iContractID int,
        @iBookID int,
        @iStrategyID int,
        @iTraderID int,
        @dtTradeDate datetime = Null,
        @tiIsClose tinyint = Null
as
     select t.tradeID as iTradeID,
	t.contractID as iContractID,
	t.isClose as tiIsClose,
	t.tradeDate as dtTradeDate,
	t.quantity as iQuantity,
	t.price as fPrice,
	t.isBuy as bIsBuy,
	t.bookID as iBookID,
	t.strategyID as iStrategyID,
	t.traderID as iTraderID,
	t.brokerID as iBrokerID,
	t.clearingBrokerID as iClearingBrokerID,
	cv1.CommissionValue as fBrokerCommission,
	cv2.CommissionValue as fClearingBrokerCommission
       from ContractView c, TradeView t, CommissionView cv1, CommissionView cv2
      where t.contractID = c.contractID
        and t.clearingBrokerCommissionID = cv2.commissionID
        and t.BrokerCommissionID = cv1.commissionID
		and c.contractID = @iContractID
        and ((@iBookID is Null) or ((@iBookID is not Null) and (t.bookID = @iBookID)))
        and ((@iStrategyID is Null) or ((@iStrategyID is not Null) and (t.strategyID = @iStrategyID)))
        and ((@iTraderID is Null) or ((@iTraderID is not Null) and (t.traderID = @iTraderID)))
        and ((@dtTradeDate is Null) or ((@dtTradeDate is not Null) and (t.TradeDate = @dtTradeDate)))
        and ((@tiIsClose is Null) or ((@tiIsClose is not Null) and (t.IsClose = @tiIsClose)))

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.38.043')
	begin
		update DataInfo set keyValue = '3.38.043' where dataInfoID = 1
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
	PRINT 'DB VERSION WAS CHANGED TO 3.38.043'
	COMMIT TRANSACTION
END
	ELSE PRINT 'The database update failed'
GO
DROP TABLE #tmpErrors
GO