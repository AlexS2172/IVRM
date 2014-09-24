/*
fix errors in reports of BO & cleanup operation.
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.40.060' or (keyValue = '3.40.061')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.40.061 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.40.061' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[job_Cleanup_DB]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[job_Cleanup_DB]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_PositionForReconcile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_PositionForReconcile_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE      Procedure dbo.job_Cleanup_DB
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is remove information from base tables with actionID = 3
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction DB_Cleanup

		--SyntheticUnderlyingParam by actionID = 3
		delete SyntheticUnderlyingParam where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--SyntheticUnderlyingParam by Contract.actionID = 3
		delete SyntheticUnderlyingParam from SyntheticUnderlyingParam s join Contract c on s.underlyingID = c.contractID
			where c.actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--IRPoint
		delete IRPoint where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--IRCurve
		delete IRPoint where curveID in (select curveID from IRCurve where actionID = 3)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		delete IRCurve where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--ContractInGroup
		delete ContractInGroup where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--ContractGroup
		delete ContractInGroup where groupID in (select groupID from ContractGroup where actionID = 3)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		delete ContractGroup where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--IndexDefinition
		delete IndexDefinition where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--ExchangeHoliday
		delete ExchangeHoliday where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Holiday
		delete ExchangeHoliday where holidayID in (select holidayID from Holiday where actionID = 3)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		delete Holiday where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Strategy
		delete Strategy 
			where actionID = 3 and strategyID not in 
				(select distinct strategyID from Trade where strategyID is not null)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--TraderContract
		delete TraderContract where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--RMScenario
		delete RMScenario where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--CustomStrikeSkewPoint
		delete from CustomStrikeSkewPoint where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--VolaSurfaceData
		delete VolaSurfaceData where actionID = 3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--VolaSurfaceData
		delete CustomStrikeSkewPoint 
			where volaSurfaceDataID in (select volaSurfaceDataID from VolaSurfaceData where actionID = 3)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--don't forget about TraderRole

		--TraderContract 
		delete from TraderContract 
			where traderID in 
				(select traderID from Trader where actionID = 3 and traderID not in (select distinct traderID from Trade where traderID is not null))
			or traderID in 
				(select traderRefID from Trader where actionID = 3 and traderRefID not in (select distinct traderID from Trade where traderID is not null))
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Trader
		delete Trader 
			where traderRefID in 
				(select traderID from Trader where actionID = 3 and traderID not in (select distinct traderID from Trade where traderID is not null))
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		delete Trader where traderID not in (select distinct traderID from Trade) and actionID =3
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Commission
		delete Commission
			where actionID = 3 and 
				commissionID not in (select distinct brokerCommissionID from Trade where brokerCommissionID is not null) and 
				commissionID not in (select distinct clearingBrokerCommissionID from Trade where clearingBrokerCommissionID is not null)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		delete Commission
			where brokerID in 
					(select brokerID from Broker
						where actionID = 3 and 
							brokerID not in (select distinct clearingBrokerID from Trade where clearingBrokerID is not null) and 
							brokerID not in (select distinct brokerID from Trade where brokerID is not null))
 			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Broker
		delete Broker 
			where actionID = 3 and 
				brokerID not in (select distinct clearingBrokerID from Trade where clearingBrokerID is not null) and 
				brokerID not in (select distinct brokerID from Trade where brokerID is not null)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
	
		--BookRuleItemMask
		delete BookRuleItemMask 
			where bookRuleItemID in (select bookRuleItemID from BookRuleItem where bookID in 
					(select bookID 
						from Book 
							where actionID = 3 and bookID not in (select distinct bookID from Trade where bookID is not null)))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--BookRuleItem
		delete BookRuleItem
			where bookID in 
					(select bookID 
						from Book 
							where actionID = 3 and bookID not in (select distinct bookID from Trade where bookID is not null))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Book
		delete Book
			where actionID = 3 and bookID not in (select distinct bookID from Trade where bookID is not null)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--=========================================================================================================
		--INDEX
		if object_id('tempdb..#IDs') is null
			Create table #IDs(contractID int not null)
		else
			Delete from #IDs
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--select indices for deletion (which not releated with Trades)
		insert into #IDs
			select indexID from [Index] 
				where actionID = 3 
						and indexID not in (select distinct contractID from Trade T join [Index] I on T.contractID = I.indexID)
						and indexID not in (select indexID 
												from [Index] I 
														join OptionRoot ort on ort.underlyingID = i.indexID
														join [Option] o on ort.optionRootID = o.optionRootID
														join Trade T on T.contractID = O.optionID)
 			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--IndexCorrelation
		delete from IndexCorrelation 
			where indexID1 in (select contractID from #IDs) or indexID2 in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--IndexDataHistory
		delete from IndexDataHistory where indexID in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		--IndexDefinition
		delete from IndexDefinition where indexID in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--delete all related rows in Contract table by indexID
		if object_ID('tempdb..#Contracts_for_delete') is null
			Create table #Contracts_for_delete(contractID int)
		else
			Delete from #Contracts_for_delete
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		insert into #Contracts_for_delete
			select contractID from #IDs 
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		exec usp_Contract_Del Null,1
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Index
		delete from [Index] where indexID in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--Contract
		delete from Contract where contractID in (select contractID from #IDs)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		if object_id('tempdb..#IDs') is not null
			Drop table #IDs
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		--=====================================================================================================================
		--STOCK
		delete StockDataHistory 
			where stockID in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		delete StockCorrelation 
			where stockID1 in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		delete StockCorrelation 
			where stockID2 in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		delete IndexDefinition 
			where stockID in (select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade))
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		if object_ID('tempdb..#Contracts_for_delete') is not null
			delete from #Contracts_for_delete
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		insert into #Contracts_for_delete
			select stockID from Stock where actionID = 3 and stockID not in (select distinct ContractID from Trade)
			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		exec usp_Contract_Del Null,1
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		delete from Stock where stockID in (select contractID from #Contracts_for_delete)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end


		delete from Contract where contractID in (select contractID from #Contracts_for_delete)
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		--=====================================================================================================================
		--OPTION

		if object_ID('tempdb..#Options_for_delete') is null
			create table #Options_for_delete(OptionID int)
		else
			delete from #Options_for_delete
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		insert into #Options_for_delete 
			select optionID 
				from [Option] O
				where optionID not in (select distinct ContractID from Trade) and actionID = 3
		if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end

		exec usp_Option_Del null,1
  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
		--=====================================================================================================================

		--fix DB cleanup date	
		--update DataInfo set keyValue = GetDate() where dataInfoID = 6

		update DataInfo set keyValue = convert(varchar, GetDate(), 120) where dataInfoID = 6
		if @@ROWCOUNT = 0 
			begin 
				insert into DataInfo 
				    select 6,'lastCleanupDate',convert(varchar, GetDate(), 120)
	  			if @@error <> 0 begin set @error = 3 if @@TranCount > 0 Rollback Transaction DB_Cleanup goto finish end
			end 
	Commit Transaction DB_Cleanup
    set @error = 0
finish:
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

CREATE    PROCEDURE dbo.usp_PositionForReconcile_Get
	@tiByTrader tinyint = 0,
	@lastUTCTradeDate datetime = null
AS
	if isnull(@tiByTrader, 0) = 0
	begin
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as iOptionRootID,
			null as vcOptionRootSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			1 as iLotSize,
			cp.priceClose as fPriceClose,

			null as iTraderID,
			null as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty
		
		from TradeView tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where c.contractTypeID < 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by c.contractID, c.contractTypeID, c.Symbol, cp.priceClose
		
		union
	
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			ORt.optionRootID as iOptionRootID,
			ORt.symbol as vcOptionRootSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,

			ORt.lotSize as iLotSize,
			cp.priceClose as fPriceClose,

			null as iTraderID,
			null as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty

		from TradeView tr 
			inner join ContractView C on tr.contractID = C.contractID
			inner join OptionView o on c.contractID = o.optionID
			inner join OptionRootView ORt on o.optionRootID = ORt.optionRootID
			inner join ContractView UC on ORt.underlyingID = UC.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
	
		where c.contractTypeID = 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by C.contractID, C.contractTypeID, C.Symbol,ORt.optionRootID,ORt.symbol,
			EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
			ORt.lotSize, cp.priceClose
	
		order by vcSymbol
	end
	else
	begin
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as iOptionRootID,
			null as vcOptionRootSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			1 as iLotSize,
			cp.priceClose as fPriceClose,
			trdr.TraderID as iTraderID,
			trdr.acronym as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty
		from TradeView tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join TraderView trdr on tr.traderID = trdr.traderID
		where c.contractTypeID < 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by c.contractID, c.contractTypeID, c.Symbol,cp.priceClose, trdr.TraderID, trdr.acronym
		
		union
	
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			ORt.optionRootID as iOptionRootID,
			ORt.symbol as vcOptionRootSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			ORt.lotSize as iLotSize,
			cp.priceClose as fPriceClose,
			trdr.TraderID as iTraderID,
			trdr.acronym as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty

		from TradeView tr 
			inner join ContractView C on tr.contractID = C.contractID
			inner join OptionView o on c.contractID = o.optionID
			inner join OptionRootView ORt on o.optionRootID = ORt.optionRootID
			inner join ContractView UC on ORt.underlyingID = UC.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			left join TraderView trdr on tr.traderID = trdr.traderID
	
		where c.contractTypeID = 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by C.contractID, C.contractTypeID, C.Symbol,ORt.optionRootID,ORt.symbol,
			EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
			ORt.lotSize, cp.priceClose, trdr.TraderID, trdr.acronym
	
		order by vcSymbol
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.40.061')
	begin
		update DataInfo set keyValue = '3.40.061' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.40.061' '3.40.061 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
