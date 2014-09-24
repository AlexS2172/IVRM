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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.45.102' or (keyValue = '3.45.103')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.45.103 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.45.103' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/


----------------------------------------------------------------------------------------------------
-- usp_UnderlyingExpiry_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingExpiry_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_UnderlyingExpiry_Get]
GO

CREATE PROC dbo.usp_UnderlyingExpiry_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@dtMinExpiry datetime = null
AS
	set nocount on

	if @dtMinExpiry is null	set @dtMinExpiry = '19000101' -- set to min available date to avoid 'is null' checking

	if isnull(@iGroupID, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
		begin
			if isnull(@iContractID, 0) = 0
				select
					c.contractID as iUnderlyingID,
					ec.expiryID as iExpiryID,
					ec.expiryDate as dtExpiryDate
				from ContractView c
					inner join OptionRootView r on c.contractID = r.underlyingID
					inner join OptionView o on r.optionRootID = o.optionRootID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = c.expCalendarID
				where 
					ec.expiryDate >= @dtMinExpiry
				group by
					c.contractID, ec.expiryID, ec.expiryDate
				order by expiryDate
			else
				select
					c.contractID as iUnderlyingID,
					ec.expiryID as iExpiryID,
					ec.expiryDate as dtExpiryDate
				from ContractView c
					inner join OptionRootView r on c.contractID = r.underlyingID
					inner join OptionView o on r.optionRootID = o.optionRootID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = c.expCalendarID
				where 
					c.contractID = @iContractID
					and ec.expiryDate >= @dtMinExpiry
				group by
					c.contractID, ec.expiryID, ec.expiryDate
				order by expiryDate
		end
		else
		begin
			if isnull(@iContractID, 0) = 0
				select
					c.contractID as iUnderlyingID,
					ec.expiryID as iExpiryID,
					ec.expiryDate as dtExpiryDate
				from TraderContractView tc 
				    inner join ContractView c on tc.contractID = c.contractID
					inner join OptionRootView r on c.contractID = r.underlyingID
					inner join OptionView o on r.optionRootID = o.optionRootID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = c.expCalendarID
				where 
					tc.traderID = @iTraderID
					and ec.expiryDate >= @dtMinExpiry
				group by
					c.contractID, ec.expiryID, ec.expiryDate
				order by expiryDate
			else
				select
					c.contractID as iUnderlyingID,
					ec.expiryID as iExpiryID,
					ec.expiryDate as dtExpiryDate
				from TraderContractView tc 
				    inner join ContractView c on tc.contractID = c.contractID
					inner join OptionRootView r on c.contractID = r.underlyingID
					inner join OptionView o on r.optionRootID = o.optionRootID
					inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = c.expCalendarID
				where 
					tc.traderID = @iTraderID
					and c.contractID = @iContractID
					and ec.expiryDate >= @dtMinExpiry
				group by
					c.contractID, ec.expiryID, ec.expiryDate
				order by expiryDate
		end
	end
	else
	begin
		if isnull(@iTraderID, 0) = 0
			select
				c.contractID as iUnderlyingID,
				ec.expiryID as iExpiryID,
				ec.expiryDate as dtExpiryDate
			from ContractInGroupView cig 
				inner join ContractView c on c.contractID = cig.contractID
				inner join OptionRootView r on c.contractID = r.underlyingID
				inner join OptionView o on r.optionRootID = o.optionRootID
				inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = c.expCalendarID
			where 
				cig.groupID = @iGroupID
				and ec.expiryDate >= @dtMinExpiry
			group by
				c.contractID, ec.expiryID, ec.expiryDate
			order by expiryDate
		else
			select
				c.contractID as iUnderlyingID,
				ec.expiryID as iExpiryID,
				ec.expiryDate as dtExpiryDate
			from ContractInGroupView cig 
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
				inner join OptionRootView r on c.contractID = r.underlyingID
				inner join OptionView o on r.optionRootID = o.optionRootID
				inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = c.expCalendarID
			where 
				cig.groupID = @iGroupID
				and tc.traderID = @iTraderID
				and ec.expiryDate >= @dtMinExpiry
			group by
				c.contractID, ec.expiryID, ec.expiryDate
			order by expiryDate
	end

GO

----------------------------------------------------------------------------------------------------
-- usp_UnderlyingPos_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingPos_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_UnderlyingPos_Get]
GO

CREATE PROC dbo.usp_UnderlyingPos_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithSimulated tinyint = 0
AS
	set nocount on

	if isnull(@iContractID, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
	
				from ContractView c 
					left join TradeView tr on tr.contractID = c.contractID 
								and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.status,0)!=1)
				where
					c.contractTypeID in (1, 2)
				group by 
					c.contractID
			else
				select
				    c.contractID as iContractID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
	
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
					left join TradeView tr on tr.contractID = c.contractID 
								and tr.traderID = @iTraderID 
								and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.status,0)!=1)
				where 
					c.contractTypeID in (1, 2)
					and tc.traderID = @iTraderID
				group by 
					c.contractID
		end
		else
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
	
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
					left join TradeView tr on tr.contractID = c.contractID 
								and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.status,0)!=1)
				where 
					cig.groupID = @iGroupID
					and c.contractTypeID in (1, 2)
				group by 
					c.contractID
			else
				select
				    c.contractID as iContractID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
	
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					left join TradeView tr on tr.contractID = c.contractID 
								and tr.traderID = @iTraderID 
								and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.status, 0)!=1)
				where cig.groupID = @iGroupID
					and c.contractTypeID in (1, 2)
					and tc.traderID = @iTraderID
				group by 
					c.contractID
		end
	end
	else
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
	
				from ContractView c 
					left join TradeView tr on tr.contractID = c.contractID 
								and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.status,0)!=1)
				where
					c.contractTypeID in (1, 2)
					and c.contractID = @iContractID
				group by 
					c.contractID
			else
				select
				    c.contractID as iContractID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
	
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
					left join TradeView tr on tr.contractID = c.contractID 
								and tr.traderID = @iTraderID 
								and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.status,0)!=1)
				where 
					c.contractTypeID in (1, 2)
					and tc.traderID = @iTraderID
					and c.contractID = @iContractID
				group by 
					c.contractID
		end
		else
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
	
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
					left join TradeView tr on tr.contractID = c.contractID 
								and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.status,0)!=1)
				where 
					cig.groupID = @iGroupID
					and c.contractTypeID in (1, 2)
					and c.contractID = @iContractID
				group by 
					c.contractID
			else
				select
				    c.contractID as iContractID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
	
				from ContractInGroupView cig
					inner join ContractView c on c.contractID = cig.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					left join TradeView tr on tr.contractID = c.contractID 
								and tr.traderID = @iTraderID 
								and (isnull(@tiWithSimulated, 0) <> 0 or isnull(tr.status, 0)!=1)
				where cig.groupID = @iGroupID
					and c.contractTypeID in (1, 2)
					and tc.traderID = @iTraderID
					and c.contractID = @iContractID
				group by 
					c.contractID
		end
	end
GO

----------------------------------------------------------------------------------------------------
-- usp_TraderContractStrategy_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContractStrategy_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderContractStrategy_Import]
GO

CREATE PROC dbo.usp_TraderContractStrategy_Import
	@vcTraderAcronym varchar(12),
	@vcUnderlyingSymbol varchar(20),
	@vcStrategyName varchar(12),
	@iContractTypeID int
AS
	set nocount on
	declare @error int		set @error = -1

	set @vcTraderAcronym = ltrim(rtrim(isnull(@vcTraderAcronym, '')))
	if len(@vcTraderAcronym) <= 0
	begin
		raiserror ('Trader acronym can not be empty!', 16, 1)
		set @error = -1
		goto finish
	end

	set @vcUnderlyingSymbol = ltrim(rtrim(isnull(@vcUnderlyingSymbol, '')))
	if len(@vcUnderlyingSymbol) <= 0
	begin
		raiserror ('Underlying symbol can not be empty!', 16, 1)
		set @error = -2
		goto finish
	end

	set @vcStrategyName = ltrim(rtrim(isnull(@vcStrategyName, '')))
	if len(@vcStrategyName) <= 0
	begin
		raiserror ('Strategy name can not be empty!', 16, 1)
		set @error = -3
		goto finish
	end

	declare @iUnderlyingContractID int
	declare @iTraderID int
	declare @iStrategyID int
	declare @iTraderContractID int

	-- set possible invalid contract type to stock
	set @iContractTypeID = isnull(@iContractTypeID, 2)
	if @iContractTypeID <> 1 set @iContractTypeID = 2

	-- check underlying
	select @iUnderlyingContractID = contractID 
		from ContractView 
		where symbol = @vcUnderlyingSymbol and contractTypeID < 3

	begin tran

	-- insert new underlying
	if @iUnderlyingContractID is null
	begin
		insert into Contract (symbol, contractTypeID, expCalendarID) values (@vcUnderlyingSymbol, @iContractTypeID, 1)
			if @@error <> 0 begin set @error = -4 if @@TranCount > 0 Rollback Transaction goto finish end
		set @iUnderlyingContractID = @@identity

		if @iContractTypeID = 2
		begin
			insert into Stock (stockID, isActive) values (@iUnderlyingContractID, 1)
				if @@error <> 0 begin set @error = -5 if @@TranCount > 0 Rollback Transaction goto finish end
		end
		else
		begin
			insert into [Index] (indexID, isActive) values (@iUnderlyingContractID, 1)
				if @@error <> 0 begin set @error = -6 if @@TranCount > 0 Rollback Transaction goto finish end
		end

		insert into ContractPrice (contractID, exchangeID) values (@iUnderlyingContractID, null)
			if @@error <> 0 begin set @error = -7 if @@TranCount > 0 Rollback Transaction goto finish end
	end

	-- check trader
	select @iTraderID = traderID 
		from TraderView 
		where acronym = @vcTraderAcronym

	-- insert new trader
	if @iTraderID is null
	begin
		insert into Trader (acronym, traderName) values (@vcTraderAcronym, @vcTraderAcronym)
			if @@error <> 0 begin set @error = -8 if @@TranCount > 0 Rollback Transaction goto finish end
		set @iTraderID = @@identity
	end

	-- check strategy
	select @iStrategyID = strategyID 
		from StrategyView 
		where strategyName = @vcStrategyName

	-- insert new strategy
	if @iStrategyID is null
	begin
		insert into Strategy (strategyName, [description]) values (@vcStrategyName, @vcStrategyName)
			if @@error <> 0 begin set @error = -9 if @@TranCount > 0 Rollback Transaction goto finish end
		set @iStrategyID = @@identity
	end

	-- check trader/contract/strategy association
	select @iTraderContractID = traderContractID
		from TraderContractView 
		where traderID = @iTraderID and contractID = @iUnderlyingContractID

	if @iTraderContractID is null
	begin
		insert into TraderContract (traderID, contractID, strategyID) values (@iTraderID, @iUnderlyingContractID, @iStrategyID)
			if @@error <> 0 begin set @error = -10 if @@TranCount > 0 Rollback Transaction goto finish end
	end
	else
	begin
		update TraderContract set strategyID = @iStrategyID
		where traderContractID = @iTraderContractID
			if @@error <> 0 begin set @error = -11 if @@TranCount > 0 Rollback Transaction goto finish end
	end

	commit tran

finish:
	return @error

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.45.103')
	begin
		update DataInfo set keyValue = '3.45.103' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.45.103' '3.45.103 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
