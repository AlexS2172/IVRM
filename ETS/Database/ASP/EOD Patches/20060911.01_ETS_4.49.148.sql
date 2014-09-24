/*
	Contains stored procedures for ETS ASP (Institutional) database
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
--BEGIN TRANSACTION
--GO
--SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
--GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.147' or (keyValue = '4.49.148')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.148 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.147' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureUnderlying' and COLUMN_NAME='actionID')
begin
	ALTER TABLE FutureUnderlying Add 
	actionID tinyint Default(0) NOT NULL 
end

GO

GO


ALTER    PROC usp_BoCommodity_Del 
	@iCID int,
	@tiIsRaiseError tinyint = 1 
as
/*
	Description: Commodity Delete procedure
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0
	
	if exists (select 1 from Trade where contractID = @iCID)
	   return -1

	if exists (select 1 from [Order] where contractID = @iCID)
	   return -2


	Begin Transaction CommodityDel
		exec @error = usp_TradeLog_Del @iCID
			if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end

		exec @error = usp_BoFutureRootByUnderlying_Del @iCID, 0
			if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end


		update IndexBeta set actionID=3 where indexID = @iCID
			if @@error != 0 begin set @error = -8 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end


		update ContractInGroup set actionID=3 where contractID = @iCID 
			if @@error != 0 begin set @error = -10 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end

		update FutureUnderlying set actionID=3 where
		iFutureUnderlyingID = @iCID

		if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran CommodityDel goto finish end

		update TraderContract set actionID=3 where contractID = @iCID  
			if @@error <> 0 begin set @error = -20 if @@TranCount != 0 Rollback Transaction CommodityDel goto finish end

		update Contract set actionID=3 where contractID = @iCID
			if @@error != 0 begin set @error = -21 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end


		set @error = @@error
	Commit Transaction CommodityDel

finish:
	return @error


GO

ALTER      PROCEDURE usp_IC_Save
      @iICID int,
      @vcSymbol varchar(20) = null,
      @vcName varchar(255) = null,
      @iICType int,   
      @tiGreeksCalcType tinyint = null,
      @tiIsActive tinyint = null,
      @iFutureUnderlyingTypeID int,
      @tiCheckExistance tinyint = null,
      @iUndPriceProfileID int = null output,
      @iOptPriceProfileID int = null output
  
     as
    ---------------
    set nocount on
    ---------------
  --  if ((@iICType <> 6) And (@iICType <> 7 ))
  --    begin
  --      Raiserror ('Wrong Interest/FutureUnderlying type value', 16, 1)
  --      Return (-1)
  --    end
  
    if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID = @iICType and actionID < 3 and (@iICID <> contractID or @iICID is null))
      begin
        if isnull(@tiCheckExistance, 1) <> 0
          Raiserror ('Interest or FutureUnderlying with this ticker already exists!', 16, 1)
        Return (-1)
      end   
  
     if (@iICID is Null)  
     begin
       
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

      if @iICType = 6
      begin		
          --insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcSymbol, 'Default calendar')
          --set @iExpCalendarID = scope_identity()

               insert into Contract (contractTypeID, Symbol, contractName/*, expCalendarID*/, undPriceProfileID, optPriceProfileID)
                    values ( @iICType, @vcSymbol, @vcName/*, null*/, @iUndPriceProfileID, @iOptPriceProfileID)
      end
      else
      begin
          --insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcSymbol, 'Default calendar')
          --set @iExpCalendarID = scope_identity()  

               insert into Contract (contractTypeID, Symbol, contractName/*, expCalendarID*/, undPriceProfileID, optPriceProfileID)
                    values ( @iICType, @vcSymbol, @vcName/*, null*/, @iUndPriceProfileID, @iOptPriceProfileID)	
      end
  
         select @iICID = @@identity      
           if (@@error <> 0)
              begin
              RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
              ROLLBACK TRAN
              RETURN (-1)
              end
         insert into [FutureUnderlying] (
                iFutureUnderlyingID, 
                tiActive,
                tiGreeksCalculationType,
                iFutureUnderlyingTypeID)
             values (
                @iICID, 
                @tiIsActive,
                @tiGreeksCalcType,
                @iFutureUnderlyingTypeID)
           if (@@error <> 0)
            begin
               RAISERROR ('Error. Can''t insert into [FutureUnderlying]!', 16, 1)
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
            contractName = isNull(@vcName,contractName),
            --expCalendarID = null,--isNull(@iExpCalendarID, expCalendarID),
            undPriceProfileID = isnull(@iUndPriceProfileID, undPriceProfileID),
            optPriceProfileID = isnull(@iOptPriceProfileID, optPriceProfileID)
          where contractID = @iICID
        if (@@error <> 0)
           begin
            RAISERROR ('Error. Can''t update Contract!', 16, 1)
            ROLLBACK TRAN
            RETURN (-1)
           end
        update [FutureUnderlying]
           set  tiGreeksCalculationType = isnull(@tiGreeksCalcType, tiGreeksCalculationType),
            tiActive = isnull(@tiIsActive, tiActive),
            iFutureUnderlyingTypeID = isnull(@iFutureUnderlyingTypeID, iFutureUnderlyingTypeID)
           where iFutureUnderlyingID = @iICID
         if (@@error <> 0)
           begin
            RAISERROR ('Error. Can''t update [FutureUnderlying]!', 16, 1)
            ROLLBACK TRAN
            RETURN (-1)
           end       
       commit tran
     end
  
    --========================================================================
    -- make record with null exchange for this index
    --========================================================================
  --  if not exists(select * from ContractPrice where exchangeID is null and contractID = @iICID)
  --    begin
  --      insert into ContractPrice(contractID,exchangeID)
  --        values(@iICID,null)
  --      if (@@error <> 0) begin rollback tran return(-1) end              
  --    end   
  
  if (@@error <> 0)
    return (-1)
  else  
    return @iICID



GO

ALTER  PROC dbo.usp_BoFutureRootByUnderlying_Del
	@iUnderlyingID int,
	@tiIsRaiseError tinyint = 1 
AS
	-- delete future roots by underlying (with all futures and futures options)

	set nocount on
	
	declare @error int		set @error = 0

	-- check underlying ID
	if @iUnderlyingID is null
	begin
		if @tiIsRaiseError = 1 raiserror('Underlying for future roots to delete is not specified!', 16, 1)
		set @error = -1
		goto finish
	end

	-- create temporary table with contracts to delete
	create table #FutureRootsContractsForDelete(contractID int, contractTypeID int)
	
	insert into #FutureRootsContractsForDelete
		select 
			f.futureID, 4
		from FutureRoot fr
			inner join Future f on fr.futureRootID = f.futureRootID
		where fr.underlyingID = @iUnderlyingID
		union select 
			fo.futureOptionID, 5 
		from FutureRoot fr
			inner join Future f on fr.futureRootID = f.futureRootID
			inner join FutureOption fo on fo.futureID = f.futureID
		where fr.underlyingID = @iUnderlyingID

	-- check for trades existence
	if exists(select 1 from Trade where contractID in (select contractID from #FutureRootsContractsForDelete))
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future roots, because its futures or its future options has trades or position!', 16, 1)
		set @error = -2
		goto finish
	end

	-- check for orders existence
	if exists(select 1 from [Order] where contractID in (select contractID from #FutureRootsContractsForDelete))
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future roots, because its futures or its future options has orders!', 16, 1)
		set @error = -3
		goto finish
	end

	begin tran

	-- delete future options data
	delete FutureOption 
	where futureID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 4)
	if @@error <> 0 begin set @error = -4 if @@trancount = 1 rollback tran goto finish end

	-- delete future data
	delete Future
	where futureID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 4)
	if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

	-- delete future root
	delete FutureRoot
	where underlyingID = @iUnderlyingID
	if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end

	-- delete common future and future options contracts data
	delete ContractPrice 
	where contractID in (select contractID from #FutureRootsContractsForDelete)
	if @@error <> 0 begin set @error = -7 if @@trancount = 1 rollback tran goto finish end

	delete ContractExchangeLink 
	where contractID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 4)
	if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end

	delete Contract 
	where contractID in (select contractID from #FutureRootsContractsForDelete)
	if @@error <> 0 begin set @error = -9 if @@trancount = 1 rollback tran goto finish end

	commit tran

finish:
	return @error


GO
ALTER      PROC usp_MmTrades_Get
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
                        tr.actionDate as dtActionDate, --m
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			te.acronym as traderAcronym,
			te.traderName as traderName,
			tr.strategyID as iStrategyID, 
			st.StrategyName as strategyName,
			tr.brokerID as iBrokerID,
			br.BrokerName as brokerName,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			brc.BrokerName as clearingBrokerName,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			Case when c.contractTypeID in (4,5) then fr.futureRootID else uc.contractID end as iUnderlyingContractID,
	                Case when c.contractTypeID in (4,5) then fr.futureRootSymbol else uc.Symbol end as underlyingContractSymbol,
	                ort.Symbol as optionRootSymbol,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
	                fr.futureRootSymbol as futureRoot,
        	        fu.Symbol as Future,
			tr.actionID as ActionID,
			isnull(o.strike, fo.strike)            as OptionStrike,
			isnull(ec.expiryDate, fec.expiryDate)  as OptionExpityDate	
			
		from Trade tr 
			inner join ( 
				select tradeID, max(seqNum) as seqNum from Trade group by tradeID
			 ) tr2 on tr.seqNum = tr2.seqNum

			-- contract and underlying contract
			inner join ContractView  c  on tr.contractID   = c.contractID
			inner join ContractView  uc on tr.underlyingID = uc.contractID
			left  join ContractPrice cp on tr.contractID   = cp.contractID and cp.exchangeID is null
			left  join ContractView  fu on fu.ContractID   = tr.FutureID
			inner join Trader        te on te.TraderID    = tr.traderID
			left  join Strategy  	 st  on st.StrategyID = tr.StrategyID
			left  join Broker     	 br  on br.brokerID = tr.brokerID 
			left  join Broker        brc on br.brokerID = tr.ClearingBrokerID

			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on datediff(d,o.expiryDate, ec.expiryDate)=0 and ec.expCalendarID = uc.expCalendarID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on datediff(d, fo.expiryDate, fec.expiryDate)=0 and fec.expCalendarID = c.expCalendarID

		order by TradeDate


GO

delete from contractPrice where ContractID in (
select contractID
from contract where  contractTypeid = 5
and contractID not in (select FutureOptionID from futureOption)
)
GO
delete from Trade where ContractID in (
select contractID
from contract where  contractTypeid = 5
and contractID not in (select FutureOptionID from futureOption)
)
GO
DELETE from contract where  contractTypeid = 5
and contractID not in (select FutureOptionID from futureOption)
GO

GO
/* RESULTS ANALYZING */
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.148')
	begin
		update DataInfo set keyValue = '4.49.148' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
--IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
--GO
--IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
--GO
--IF EXISTS (SELECT * FROM #tmpErrors)
--BEGIN
--	ROLLBACK TRANSACTION
	--SELECT * FROM #tmpErrors
--END
GO
--IF @@TRANCOUNT > 0 
--BEGIN
	select 'DB VERSION WAS CHANGED TO 4.49.148' '4.49.148 update message:' 
	select 'The database update succeeded'
--
IF @@TRANCOUNT > 0 
BEGIN
	COMMIT TRANSACTION
END
--	ELSE select 'The database update failed'
--GO
DROP TABLE #tmpErrors
GO
-- unable to execute this script in transaction