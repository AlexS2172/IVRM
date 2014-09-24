/*
	 MLine00001642, MLine00001653	
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.43.083' or (keyValue = '3.43.084')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.43.084 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.43.084' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

/*DATA MODIFICATION SECTION*/

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_PriceProfile_CheckDefaults' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_PriceProfile_CheckDefaults
GO

CREATE PROCEDURE dbo.usp_PriceProfile_CheckDefaults
AS
/*
	Created by Sharky
	Description: this procedure is fill default price profiles
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction PriceProfileCheckDefaults

	-- Default Stock Price Profile N1
	if not exists (select 1 from PriceProfile where defaultID = 1 and isOptionProfile = 0)
	begin
		insert into PriceProfile
			(priceProfileName, priceProfileDesc, defaultID, isOptionProfile, alwaysLast, midmarketPnL, singlePriceRule, bothPriceRule)
		values 
			('StkProfileN1', 'Default Stock Price Profile N1', 1, 0, 0, 1, 1, 0)

		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction PriceProfileCheckDefaults goto finish end

		update Contract
			set undPriceProfileID = @@identity
		where
			contractTypeID = 2
			and isnull(undPriceProfileID, 0) = 0

		if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction PriceProfileCheckDefaults goto finish end
	end

	-- Default Stock Price Profile N2
	if not exists (select 1 from PriceProfile where defaultID = 2 and isOptionProfile = 0)
	begin
		insert into PriceProfile
			(priceProfileName, priceProfileDesc, defaultID, isOptionProfile, alwaysLast, midmarketPnL, singlePriceRule, bothPriceRule)
		values 
			('StkProfileN2', 'Default Stock Price Profile N2', 2, 0, 0, 0, 1, 0)

		if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction PriceProfileCheckDefaults goto finish end
	end

	-- Default Stock Price Profile N3
	if not exists (select 1 from PriceProfile where defaultID = 3 and isOptionProfile = 0)
	begin
		insert into PriceProfile
			(priceProfileName, priceProfileDesc, defaultID, isOptionProfile, alwaysLast, midmarketPnL, singlePriceRule, bothPriceRule)
		values 
			('StkProfileN3', 'Default Stock Price Profile N3', 3, 0, 1, 0, 0, 0)

		if @@error <> 0 begin set @error = -4 if @@TranCount > 0 Rollback Transaction PriceProfileCheckDefaults goto finish end

		update Contract
			set undPriceProfileID = @@identity
		where
			contractTypeID = 1
			and isnull(undPriceProfileID, 0) = 0

		if @@error <> 0 begin set @error = -5 if @@TranCount > 0 Rollback Transaction PriceProfileCheckDefaults goto finish end
	end

	-- Default Option Price Profile N1
	if not exists (select 1 from PriceProfile where defaultID = 1 and isOptionProfile = 1)
	begin
		insert into PriceProfile
			(priceProfileName, priceProfileDesc, defaultID, isOptionProfile, alwaysLast, midmarketPnL, singlePriceRule, bothPriceRule)
		values 
			('OptProfileN1', 'Default Option Price Profile N1', 1, 1, 0, 1, 0, 0)

		if @@error <> 0 begin set @error = -6 if @@TranCount > 0 Rollback Transaction PriceProfileCheckDefaults goto finish end

		update Contract
			set optPriceProfileID = @@identity
		where
			contractTypeID < 3
			and isnull(optPriceProfileID, 0) = 0

		if @@error <> 0 begin set @error = -7 if @@TranCount > 0 Rollback Transaction PriceProfileCheckDefaults goto finish end
	end

	-- Default Option Price Profile N2
	if not exists (select 1 from PriceProfile where defaultID = 2 and isOptionProfile = 1)
	begin
		insert into PriceProfile
			(priceProfileName, priceProfileDesc, defaultID, isOptionProfile, alwaysLast, midmarketPnL, singlePriceRule, bothPriceRule)
		values 
			('OptProfileN2', 'Default Option Price Profile N2', 2, 1, 0, 0, 0, 0)

		if @@error <> 0 begin set @error = -8 if @@TranCount > 0 Rollback Transaction PriceProfileCheckDefaults goto finish end
	end

	set @error = @@error

	Commit Transaction PriceProfileCheckDefaults

finish:
	return @error

GO

-- change underlying profile names
begin
	update PriceProfile set 
		priceProfileName = 'StkProfileN1',
		priceProfileDesc = 'Default Stock Price Profile N1'
	where
		isOptionProfile = 0
		and defaultID = 1

	update PriceProfile set 
		priceProfileName = 'StkProfileN2',
		priceProfileDesc = 'Default Stock Price Profile N2'
	where
		isOptionProfile = 0
		and defaultID = 2

	update PriceProfile set 
		priceProfileName = 'StkProfileN3',
		priceProfileDesc = 'Default Stock Price Profile N3'
	where
		isOptionProfile = 0
		and defaultID = 3
end
GO

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
	if isnull(@tiWithPosition, 0) = 0
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
					ort.optionRootID as iOptionRootID
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
					inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where c.contractTypeID = 3
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
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) = 0
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.43.084')
	begin
		update DataInfo set keyValue = '3.43.084' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.43.084' '3.43.084 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
