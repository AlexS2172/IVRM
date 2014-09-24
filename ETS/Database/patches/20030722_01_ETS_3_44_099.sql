/*
	MLine00002024
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.44.098' or (keyValue = '3.44.099')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.44.099 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.44.099' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/


----------------------------------------------------------------------------------------------------
-- usp_UnderlyingOptions_Get
----------------------------------------------------------------------------------------------------

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_UnderlyingOptions_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_UnderlyingOptions_Get
GO

CREATE PROC dbo.usp_UnderlyingOptions_Get
	@iUnderlyingID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0,
	@dtMinExpiry datetime = Null
AS
	set nocount on
	
	if @dtMinExpiry is null set @dtMinExpiry = 0

	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				if isnull(@iUnderlyingID, 0) <> 0
					select	c.contractID as iContractID,
						c.symbol as vcSymbol,
						ort.underlyingID as iUnderlyingContractID,
						o.strike as fStrike, 
						ec.expiryDate as dtExpiry,
						o.isCall as bIsCall,
						ort.lotSize as iLotSize,
						ort.optionRootID as iOptionRootID
					from ContractView c  
						inner join [Option] o on o.optionID = c.contractID
						join OptionRoot ort on o.optionRootID = ort.optionRootID
						inner join Contract uc on ort.underlyingID = uc.contractID
						inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
						join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					where c.contractTypeID = 3
						and ort.underlyingID = @iUnderlyingID
						and ec.expiryDate >= @dtMinExpiry
					order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
				else
					select	c.contractID as iContractID,
						c.symbol as vcSymbol,
						ort.underlyingID as iUnderlyingContractID,
						o.strike as fStrike, 
						ec.expiryDate as dtExpiry,
						o.isCall as bIsCall,
						ort.lotSize as iLotSize,
						ort.optionRootID as iOptionRootID
					from ContractView c  
						inner join [Option] o on o.optionID = c.contractID
						join OptionRoot ort on o.optionRootID = ort.optionRootID
						inner join Contract uc on ort.underlyingID = uc.contractID
						inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
						join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					where c.contractTypeID = 3
						and ec.expiryDate >= @dtMinExpiry
					order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
				    inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
							join OptionRoot ort on cig.contractID = ort.underlyingID
							join ContractView uc on ort.underlyingID = uc.contractID
							inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
							join OptionRoot ort on cig.contractID = ort.underlyingID					
							join ContractView uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, ec.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
				    inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, ec.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
					join OptionRootView ort on cig.contractID = ort.underlyingID					
					inner join ContractView uc on ort.underlyingID = uc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, ec.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					ec.expiryDate as dtExpiry,
					o.isCall as bIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
					join OptionRootView ort on cig.contractID = ort.underlyingID					
					inner join ContractView uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and (@dtMinExpiry is Null or ec.expiryDate >= @dtMinExpiry)
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, ec.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc
		end
	end

Return (0)

GO


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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.44.099')
	begin
		update DataInfo set keyValue = '3.44.099' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.44.099' '3.44.099 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
