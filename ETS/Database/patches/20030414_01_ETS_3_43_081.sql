/*
	MLine00000862, MLine00000771
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.42.080' or (keyValue = '3.43.081')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.43.081 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.43.081' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

/*DATA MODIFICATION SECTION*/

--========================================================================================================
-- PriceProfile table
--========================================================================================================

if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PriceProfile]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
begin
	CREATE TABLE [dbo].[PriceProfile]
	(
		[priceProfileID] [int] IDENTITY (1, 1) NOT NULL ,
		[priceProfileName] [varchar] (50) NOT NULL ,
		[priceProfileDesc] [varchar] (255) NULL ,
		[defaultID] [tinyint] NOT NULL CONSTRAINT [DF_PriceProfile_defaultID] DEFAULT (0),
		[isOptionProfile] [tinyint] NOT NULL CONSTRAINT [DF_PriceProfile_isOptionProfile] DEFAULT (0),
		[alwaysLast] [tinyint] NOT NULL CONSTRAINT [DF_PriceProfile_alwaysLast] DEFAULT (0),
		[midmarketPnL] [tinyint] NOT NULL CONSTRAINT [DF_PriceProfile_midmarketPnL] DEFAULT (0),
		[singlePriceRule] [tinyint] NOT NULL CONSTRAINT [DF_PriceProfile_singlePriceRule] DEFAULT (0),
		[bothPriceRule] [tinyint] NOT NULL CONSTRAINT [DF_PriceProfile_bothPriceRule] DEFAULT (0),
		[actionID] [tinyint] NOT NULL CONSTRAINT [DF_PriceProfile_actionID] DEFAULT (1),
		[actionDate] [datetime] NOT NULL CONSTRAINT [DF_PriceProfile_actionDate] DEFAULT (getdate()),
		CONSTRAINT [PK_PriceProfile] PRIMARY KEY  CLUSTERED 
		(
			[priceProfileID]
		)
	)
end
GO

--========================================================================================================
-- tU_PriceProfile trigger
--========================================================================================================

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[tU_PriceProfile]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
	drop trigger [dbo].[tU_PriceProfile]
GO

CREATE TRIGGER dbo.tU_PriceProfile ON dbo.PriceProfile
    FOR Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	-- 1st step - update actionDate if record not already deleted
	update PriceProfile
		set actionDate = GetDate()
	from PriceProfile PR 
			join inserted on PR.priceProfileID = inserted.priceProfileID
			join deleted on PR.priceProfileID = deleted.priceProfileID
	where deleted.actionID < 3

	-- 2nd step - if update record -> set actionID=2
	update PriceProfile
		set actionID=2
	from PriceProfile PR
			join inserted on PR.priceProfileID = inserted.priceProfileID
			join deleted on PR.priceProfileID = deleted.priceProfileID
	where deleted.actionID<3 and inserted.actionID = 1

	-- 3d step -disable restore of deleted record
	update PriceProfile
		set actionID = 3
	from PriceProfile PR
			join inserted on PR.priceProfileID = inserted.priceProfileID
			join deleted on PR.priceProfileID = deleted.priceProfileID
	where deleted.actionID=3
GO

--========================================================================================================
-- Contract table columns and foreign keys
--========================================================================================================

if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Contract' and COLUMN_NAME='undPriceProfileID')
begin
	ALTER TABLE Contract ADD undPriceProfileID int null
end
GO

if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Contract' and COLUMN_NAME='optPriceProfileID')
begin
	ALTER TABLE Contract ADD optPriceProfileID int null
end
GO

if not exists (select * from INFORMATION_SCHEMA.REFERENTIAL_CONSTRAINTS where CONSTRAINT_NAME='FK_Contract_PriceProfile_UndPriceProfile')
begin
	ALTER TABLE [dbo].[Contract] ADD
		CONSTRAINT [FK_Contract_PriceProfile_UndPriceProfile] FOREIGN KEY 
			(
				[undPriceProfileID]
			)
			REFERENCES [PriceProfile]
			(
				[priceProfileID]
			)
end
GO

if not exists (select * from INFORMATION_SCHEMA.REFERENTIAL_CONSTRAINTS where CONSTRAINT_NAME='FK_Contract_PriceProfile_OptPriceProfile')
begin
	ALTER TABLE [dbo].[Contract] ADD
		CONSTRAINT [FK_Contract_PriceProfile_OptPriceProfile] FOREIGN KEY 
			(
				[optPriceProfileID]
			)
			REFERENCES [PriceProfile]
			(
				[priceProfileID]
			)
end
GO

--========================================================================================================
-- PriceProfileView
--========================================================================================================

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PriceProfileView]') and OBJECTPROPERTY(id, N'IsView') = 1)
	drop view [dbo].[PriceProfileView]
GO

CREATE VIEW dbo.PriceProfileView
AS
	select
		priceProfileID,
		priceProfileName,
		priceProfileDesc,
		defaultID,
		isOptionProfile,
		alwaysLast,
		midmarketPnL,
		singlePriceRule,
		bothPriceRule
	from
		PriceProfile
	where
		actionID < 3

GO

--========================================================================================================
-- ContractView
--========================================================================================================

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ContractView]') and OBJECTPROPERTY(id, N'IsView') = 1)
	drop view [dbo].[ContractView]
GO

CREATE VIEW dbo.ContractView
AS
	select 
		contractID,
		contractTypeID,
		symbol,
		contractName,
		ruleID,
		groupID,
		actionID,
		actionDate,
		expCalendarID,
		MSupdateDate,
		undPriceProfileID,
		optPriceProfileID
	from
		Contract
	where
		actionID < 3

GO

--========================================================================================================
-- usp_PriceProfile_CheckDefaults
--========================================================================================================

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

	-- Default underlying Price Profile N1
	if not exists (select 1 from PriceProfile where defaultID = 1 and isOptionProfile = 0)
	begin
		insert into PriceProfile
			(priceProfileName, priceProfileDesc, defaultID, isOptionProfile, alwaysLast, midmarketPnL, singlePriceRule, bothPriceRule)
		values 
			('UndProfileN1', 'Default Underlying Price Profile N1', 1, 0, 0, 1, 1, 0)

		if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction PriceProfileCheckDefaults goto finish end

		update Contract
			set undPriceProfileID = @@identity
		where
			contractTypeID = 2
			and isnull(undPriceProfileID, 0) = 0

		if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction PriceProfileCheckDefaults goto finish end
	end

	-- Default underlying Price Profile N2
	if not exists (select 1 from PriceProfile where defaultID = 2 and isOptionProfile = 0)
	begin
		insert into PriceProfile
			(priceProfileName, priceProfileDesc, defaultID, isOptionProfile, alwaysLast, midmarketPnL, singlePriceRule, bothPriceRule)
		values 
			('UndProfileN2', 'Default Underlying Price Profile N2', 2, 0, 0, 0, 1, 0)

		if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction PriceProfileCheckDefaults goto finish end
	end

	-- Default underlying Price Profile N3
	if not exists (select 1 from PriceProfile where defaultID = 3 and isOptionProfile = 0)
	begin
		insert into PriceProfile
			(priceProfileName, priceProfileDesc, defaultID, isOptionProfile, alwaysLast, midmarketPnL, singlePriceRule, bothPriceRule)
		values 
			('UndProfileN3', 'Default Underlying Price Profile N3', 3, 0, 1, 0, 0, 0)

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

--========================================================================================================
-- Fill up PriceProfile table with default rules
--========================================================================================================

exec usp_PriceProfile_CheckDefaults
GO

--========================================================================================================
-- Fill up underlying price tolerance and price rounding settings
--========================================================================================================

begin
	if not exists(select * from DataInfo where dataInfoID = 10)
		begin
			insert into DataInfo (dataInfoID, keyName, keyValue) values (10, 'UndPriceToleranceValue', 0.05)
		end

	if not exists(select * from DataInfo where dataInfoID = 11)
		begin
			insert into DataInfo (dataInfoID, keyName, keyValue) values (11, 'PriceRoundingType', 0)
		end
end
GO

--========================================================================================================
-- usp_UndPriceToleranceValue_Get
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_UndPriceToleranceValue_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_UndPriceToleranceValue_Get
GO

CREATE PROCEDURE dbo.usp_UndPriceToleranceValue_Get
AS
/*
	Created by Sharky
	Description: this procedure is getting UndPriceToleranceValue value
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	select convert(float, KeyValue) as fUndPriceToleranceValue
		from DataInfo	
		where dataInfoID = 10
	set @error = @@error

Return(@error)
GO

--========================================================================================================
-- usp_UndPriceToleranceValue_Save
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_UndPriceToleranceValue_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_UndPriceToleranceValue_Save
GO

CREATE PROCEDURE dbo.usp_UndPriceToleranceValue_Save
	@fUndPriceToleranceValue float
AS
/*
	Created by Sharky
	Description: this procedure update UndPriceToleranceValue value
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

		update DataInfo set keyValue = @fUndPriceToleranceValue
			where dataInfoID = 10
	
    set @error = 0
finish:
Return(@error)
GO

--========================================================================================================
-- usp_PriceRoundingType_Get
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_PriceRoundingType_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_PriceRoundingType_Get
GO

CREATE PROCEDURE dbo.usp_PriceRoundingType_Get
AS
/*
	Created by Sharky
	Description: this procedure is getting PriceRoundingType value
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	select convert(tinyint, KeyValue) as tiPriceRoundingType
		from DataInfo	
		where dataInfoID = 11
	set @error = @@error

Return(@error)
GO

--========================================================================================================
-- usp_PriceRoundingType_Save
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_PriceRoundingType_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_PriceRoundingType_Save
GO

CREATE PROCEDURE dbo.usp_PriceRoundingType_Save
	@tiPriceRoundingType tinyint
AS
/*
	Created by Sharky
	Description: this procedure update PriceRoundingType value
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

		update DataInfo set keyValue = @tiPriceRoundingType
			where dataInfoID = 11
	
    set @error = 0
finish:
Return(@error)
GO

--========================================================================================================
-- usp_PriceProfile_Save
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_PriceProfile_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_PriceProfile_Save
GO

CREATE PROCEDURE dbo.usp_PriceProfile_Save 
	@iPriceProfileID int = null, 
	@vcPriceProfileName varchar(50) = null,
	@vcPriceProfileDesc varchar(255) = null,
	@tiIsOptionProfile tinyint = null,
	@tiAlwaysLast tinyint = null,
	@tiMidmarketPnL tinyint = null,
	@tiSinglePriceRule tinyint = null,
	@tiBothPriceRule tinyint = null
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if @iPriceProfileID is null and @vcPriceProfileName is null
	begin
		raiserror ('Name for new price profile can not be empty!', 16, 1)
		set @error = -1
		goto finish 
	end

	if exists (select * from PriceProfile
				where priceProfileName = @vcPriceProfileName
					and isOptionProfile = isnull(@tiIsOptionProfile, 0) and actionID < 3
					and (priceProfileID <> @iPriceProfileID or @iPriceProfileID is null))
		begin
			if isnull(@tiIsOptionProfile, 0) = 0
				raiserror ('Underlying price profile with the same name already exists!', 16, 1)
			else
				Raiserror ('Option price profile with the same name already exists!', 16, 1)
			set @error = -2
			goto finish 
		end

	Begin Transaction PriceProfile

	if @iPriceProfileID is null
		begin
			insert into PriceProfile 
				(priceProfileName, priceProfileDesc, defaultID, isOptionProfile,
				alwaysLast, midmarketPnL, singlePriceRule, bothPriceRule)
			values
				(@vcPriceProfileName, @vcPriceProfileDesc, 0, isnull(@tiIsOptionProfile, 0),
				isnull(@tiAlwaysLast, 0), isnull(@tiMidmarketPnL, 0), isnull(@tiSinglePriceRule, 0), isnull(@tiBothPriceRule, 0))

			if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction PriceProfile goto finish end

			set @iPriceProfileID = @@identity
		end
	else
		begin
			update PriceProfile set 
				priceProfileName = isnull(@vcPriceProfileName, priceProfileName),
				priceProfileDesc = isnull(@vcPriceProfileDesc, priceProfileDesc),
				isOptionProfile = isnull(@tiIsOptionProfile, isOptionProfile),
				alwaysLast = isnull(@tiAlwaysLast, alwaysLast),
				midmarketPnL = isnull(@tiMidmarketPnL, midmarketPnL),
				singlePriceRule = isnull(@tiSinglePriceRule, singlePriceRule),
				bothPriceRule = isnull(@tiBothPriceRule, bothPriceRule)
			where
				priceProfileID = @iPriceProfileID

   			if @@error <> 0 begin set @error = -4 if @@TranCount > 0  Rollback Transaction PriceProfile goto finish end
		end

	Commit Transaction PriceProfile

finish:
	if @error = 0
		return @iPriceProfileID
	else
		return @error

GO

--========================================================================================================
-- usp_PriceProfile_Get
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_PriceProfile_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_PriceProfile_Get
GO

CREATE PROCEDURE dbo.usp_PriceProfile_Get 
	@iPriceProfileID int = null
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if @iPriceProfileID is null	
		select
			priceProfileID as iPriceProfileID,
			priceProfileName as vcPriceProfileName,
			priceProfileDesc as vcPriceProfileDesc,
			defaultID as tiDefaultID,
			isOptionProfile as tiIsOptionProfile,
			alwaysLast as tiAlwaysLast,
			midmarketPnL as tiMidmarketPnL,
			singlePriceRule as tiSinglePriceRule,
			bothPriceRule as tiBothPriceRule
		from
			PriceProfileView
	else
		select
			priceProfileID as iPriceProfileID,
			priceProfileName as vcPriceProfileName,
			priceProfileDesc as vcPriceProfileDesc,
			defaultID as tiDefaultID,
			isOptionProfile as tiIsOptionProfile,
			alwaysLast as tiAlwaysLast,
			midmarketPnL as tiMidmarketPnL,
			singlePriceRule as tiSinglePriceRule,
			bothPriceRule as tiBothPriceRule
		from
			PriceProfileView
		where
			priceProfileID = @iPriceProfileID

	set @error = @@error

	return @error
GO

--========================================================================================================
-- usp_PriceProfile_Del
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_PriceProfile_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_PriceProfile_Del
GO

CREATE PROCEDURE dbo.usp_PriceProfile_Del
	@iPriceProfileID int
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	-- check parameter
	if @iPriceProfileID is null
	begin
		raiserror ('Invalid parameter!', 16, 1)
		set @error = -1
		goto finish 
	end

	-- check for default profile
	declare @tiDefaultID tinyint

	select
		@tiDefaultID = defaultID
	from
		PriceProfileView
	where
		priceProfileID = @iPriceProfileID

	if isnull(@tiDefaultID, 0) <> 0
	begin
		raiserror ('Can not delete default price profile!', 16, 1)
		set @error = -2
		goto finish 
	end

	-- check for profile assignment
	if exists (select contractID
				from ContractView
				where
					contractTypeID < 3
					and (isnull(undPriceProfileID, 0) = @iPriceProfileID
						or isnull(optPriceProfileID, 0) = @iPriceProfileID))
	begin
		raiserror ('Can not delete price profile because it assigned to one or more underlying!', 16, 1)
		set @error = -3
		goto finish 
	end

	-- delete profile
	Begin Transaction PriceProfile

	update 
		PriceProfile
	set
		actionID = 3
	where
		priceProfileID = @iPriceProfileID
		
	if @@error <> 0 begin set @error = -4 if @@TranCount > 0  Rollback Transaction PriceProfile goto finish end

finish:
	return @error

GO

--========================================================================================================
-- usp_UnderlyingByTrader_Get
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_UnderlyingByTrader_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_UnderlyingByTrader_Get
GO

CREATE PROCEDURE dbo.usp_UnderlyingByTrader_Get
	@iTraderID int = null,
	@tiTraderMode tinyint = 0 -- 0 - filter by TraderContract, 1 - don't filter by TraderContract
AS
	set nocount on

	if isnull(@iTraderID, 0) = 0
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
			s.calcOptionType as tiCalcOptionType,
			s.isHTB as tiIsHTB,
		
		    null as fYield,
			null as tiIsBasket,
		    s.divFreq as iDivFreq,
		    s.divDate as dtDivDate,
		    s.divAmt as fDivAmt,
		
		    s.isDivCustom as tiIsDivCustom,
		    s.divFreqCustom as iDivFreqCustom,
		    s.divDateCustom as dtDivDateCustom,
		    s.divAmtCustom as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,

			null as iTraderID,

			s.skew as fSkew,
			s.kurt as fKurt,
		    c.contractName as vcContractName,
			
			case when exists(select 1 from OptionRootView where underlyingID = c.contractID and isSynthetic = 1) 
					then 1 else 0 end tiHasSyntheticRoots,

			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			s.isActive as tiIsActive,
			case when exists(select 1 from OptionRootView where underlyingID = c.contractID) 
					then 1 else 0 end tiHasOptions

		from ContractView c 
		    inner join StockView s on c.contractID = s.stockID
		    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where
			c.contractTypeID = 2
		union
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
			i.calcOptionType as tiCalcOptionType,
			i.isHTB as tiIsHTB,
		
		    i.yield as fYield,
			i.isBasket as tiIsBasket,
		    null as iDivFreq,
		    null as dtDivDate,
		    null as fDivAmt,
		
		    null as tiIsDivCustom,
		    null as iDivFreqCustom,
		    null as dtDivDateCustom,
		    null as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,

			null as iTraderID,

			i.skew as fSkew,
			i.kurt as fKurt,
		    c.contractName as vcContractName,

			case when exists(select 1 from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
					then 1 else 0 end tiHasSyntheticRoots,

			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			i.isActive as tiIsActive,
			case when exists(select 1 from OptionRootView where underlyingID = c.contractID) 
					then 1 else 0 end tiHasOptions

		from ContractView c 
		    inner join IndexView i on c.contractID = i.indexID
		    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where
			c.contractTypeID = 1
		order by c.symbol	 
	else
	begin
		if isnull(@tiTraderMode, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				tc.traderID as iTraderID,

				s.skew as fSkew,
				s.kurt as fKurt,
			    c.contractName as vcContractName,
	
				case when exists(select 1 from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
						then 1 else 0 end tiHasSyntheticRoots,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID,
				s.isActive as tiIsActive,
				case when exists(select 1 from OptionRootView where underlyingID = c.contractID) 
						then 1 else 0 end tiHasOptions

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
				and tc.traderID = @iTraderID
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				tc.traderID as iTraderID,

				i.skew as fSkew,
				i.kurt as fKurt,
			    c.contractName as vcContractName,
		
				case when exists(select 1 from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
					then 1 else 0 end tiHasSyntheticRoots,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID,
				i.isActive as tiIsActive,
				case when exists(select 1 from OptionRootView where underlyingID = c.contractID) 
						then 1 else 0 end tiHasOptions

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
				and tc.traderID = @iTraderID
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				tc.traderID as iTraderID,

				s.skew as fSkew,
				s.kurt as fKurt,
			    c.contractName as vcContractName,

				case when exists(select 1 from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
						then 1 else 0 end tiHasSyntheticRoots,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID,
				s.isActive as tiIsActive,
				case when exists(select 1 from OptionRootView where underlyingID = c.contractID) 
						then 1 else 0 end tiHasOptions

			from ContractView c 
			    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				tc.traderID as iTraderID,

				i.skew as fSkew,
				i.kurt as fKurt,
			    c.contractName as vcContractName,
	
				case when exists(select 1 from OptionRootView where underlyingID=c.contractID and isSynthetic = 1) 
						then 1 else 0 end tiHasSyntheticRoots,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID,
				i.isActive as tiIsActive,
				case when exists(select 1 from OptionRootView where underlyingID = c.contractID) 
						then 1 else 0 end tiHasOptions

			from ContractView c 
			    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
			order by c.symbol	 
	end

Return (@@error)

GO

--========================================================================================================
-- usp_Stock_Save
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Stock_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Stock_Save
GO

CREATE PROCEDURE dbo.usp_Stock_Save
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
		@iOptPriceProfileID int = null output
  as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @bIsHTB is null 
		set @bIsHTB=0

	if @iExpCalendarID is null or @iExpCalendarID = 0
		set @iExpCalendarID = 1
   
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

            insert into Contract (contractTypeID, Symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
                  values (2, @vcSymbol, @vcContractName, @iExpCalendarID, @iUndPriceProfileID, @iOptPriceProfileID)
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
				kurt)
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
				@fKurt)
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
					expCalendarID = isNull(@iExpCalendarID,expCalendarID),
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
						kurt = isNull(@fKurt,kurt) 
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

--========================================================================================================
-- usp_Stock_Get
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Stock_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Stock_Get
GO

CREATE PROCEDURE dbo.usp_Stock_Get
	@iGroupID int = null,
	@iStockID int = null,
	@tiWithPosition tinyint = 0,
	@iTraderID int = null
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from Stock,Contract tables
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if isnull(@tiWithPosition, 0) = 0
	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView C  
					inner join StockView S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID = 2
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				order by c.Symbol
			else
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView C  
					inner join StockView S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				order by c.Symbol
		end
		else
		begin
			if @iTraderID is null
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join StockView S on S.stockID = C.contractID

					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
				order by c.Symbol
			else
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join StockView S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
					and tc.traderID = @iTraderID
				order by c.Symbol
		end
	end
	else
	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
			    from ContractView c  
					inner join StockView S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) = 0
			    where c.contractTypeID = 2
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				group by C.contractID, 
					C.contractTypeID, 
					C.Symbol, 
					C.ContractName,
					S.DivFreq,
					S.DivDate, 
					S.DivAmt, 
					S.isHTB, 
					S.actionDate,	
					S.actionID,
					S.isDivCustom,
					S.divAmtCustom,
					S.divDateCustom,
					S.divFreqCustom,
					S.CalcOptionType,
					S.CalcModelType,
					C.expCalendarID, 
					S.isActive,
					S.primaryExchangeID,
					S.skew,
					S.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.Symbol
			else
				select	C.contractID as iContractID, 
					C.contractTypeID as iContractTypeID, 
					C.Symbol as vcSymbol, 
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
			    from ContractView c  
					inner join StockView S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) = 0
			    where c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and ((c.contractID = @iStockID and @iStockID is not Null) or (@iStockID is Null))
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
					S.DivFreq, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive,
					S.skew,	S.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.Symbol
		end
		else
		begin
			if @iTraderID is null
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
			    from ContractInGroupView cig
					inner join ContractView c  on C.contractID = CIG.contractID
					inner join StockView S on S.stockID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) = 0
		        where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName, 
					S.DivFreq,S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive,
					S.skew, S.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.Symbol
			else
				select	C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.Symbol as vcSymbol,
					C.ContractName as vcContractName, 
					S.DivFreq as iDivFreq,
					S.primaryExchangeID as iPrimaryExchangeID, 
					S.DivDate as dtDivDate,
					S.DivAmt as fDivAmt, 
					S.isHTB as bIsHTB,
					S.actionDate as dtActionDate,
					S.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					S.isDivCustom as tiDivCustom,
					S.divAmtCustom as fDivAmtCustom,
					S.divDateCustom as dtDivDateCustom,
					S.divFreqCustom as iDivFreqCustom,
					S.CalcOptionType as tiCalcOptionType,
					S.CalcModelType as tiCalcModelType,
					C.expCalendarID as iExpCalendarID,
					S.isActive as tiIsActive,
					S.skew as fSkew,
					S.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
			    from ContractInGroupView cig
					inner join ContractView c  on C.contractID = CIG.contractID
					inner join StockView S on S.stockID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) = 0
		        where CIG.groupID = @iGroupID 
					and c.contractTypeID = 2
					and tc.traderID = @iTraderID
				group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
					S.DivFreq, S.primaryExchangeID, 
					S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
					S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,
					S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive,
					S.skew, S.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.Symbol
		end
	end
	
	set @error = @@error

Return(@error)

GO

--========================================================================================================
-- usp_Index_Save
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Index_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Index_Save
GO

CREATE PROCEDURE dbo.usp_Index_Save
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
							kurt)
					 values (
							@iIndexID, 
							@fYield,
							isNull(@bIsHTB,0),
							@tiCalcOptionType,
							@tiCalcModelType,
							isnull(@tiIsBasket,0), 
							@tiIsActive,
							@fSkew,
							@fKurt)
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
					kurt = isnull(@fKurt,Kurt)
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

--========================================================================================================
-- usp_Index_Get
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Index_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Index_Get
GO

CREATE PROCEDURE dbo.usp_Index_Get
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
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
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
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
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
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
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
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
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
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
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
						I.CalcModelType, C.expCalendarID, I.isBasket, 
						I.isActive,	I.skew,	I.kurt, c.undPriceProfileID, c.optPriceProfileID
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
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
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
						I.CalcModelType,C.expCalendarID,
						I.isBasket, I.isActive,	I.skew,	I.kurt, c.undPriceProfileID, c.optPriceProfileID
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
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join IndexView I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
					I.yield, I.isHTB, I.actionDate, I.actionID,I.CalcOptionType,
					I.CalcModelType,C.expCalendarID,I.isBasket, I.isActive,	I.skew, I.kurt, c.undPriceProfileID, c.optPriceProfileID
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
					C.expCalendarID as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
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
					I.CalcModelType,C.expCalendarID,I.isBasket, I.isActive,
					I.skew,I.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by C.symbol
		end
	end
if (@@error <> 0)
	Return (-1)
else  
	Return (0)

GO

--========================================================================================================
-- usp_Underlying_Get
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Underlying_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Underlying_Get
GO

CREATE PROCEDURE dbo.usp_Underlying_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0
AS
	set nocount on

	if isnull(@iGroupID, 0) <> 0
	begin
		exec usp_UnderlyingByGroup_Get @iContractID,@iTraderID,@iGroupID,@tiWithPosition	
		Return(0)
	end 

	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID
			from ContractView c 
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
				and (@iContractID is null or c.contractID = @iContractID)
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID
			from ContractView c 
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
				and (@iContractID is null or c.contractID = @iContractID)
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID
			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID
			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where
				c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			order by c.symbol	 
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID

			from ContractView c 
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
			where
				c.contractTypeID = 2
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
				s.isActive, s.skew, s.kurt, c.undPriceProfileID, c.optPriceProfileID
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID

			from ContractView c 
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
			where
				c.contractTypeID = 1
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt, c.undPriceProfileID, c.optPriceProfileID
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
			where 
				c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			   	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
				s.isActive, s.skew, s.kurt, c.undPriceProfileID, c.optPriceProfileID
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,
				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

				c.undPriceProfileID as iUndPriceProfileID,
				c.optPriceProfileID as iOptPriceProfileID

			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
			where
				c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt, c.undPriceProfileID, c.optPriceProfileID
			order by c.symbol	 
	end

	Return (0)

GO

--========================================================================================================
-- usp_Contract_Get
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Contract_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Contract_Get
GO

CREATE PROCEDURE dbo.usp_Contract_Get
	@iContractID int = null,		-- contract's identifier
	@iContractTypeID int = null,	-- contract's type identifier
	@bAllWithoutOptions bit = 1		-- flag for data output
as
/*
	Created by Andrey Sinkin
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	if @bAllWithoutOptions is null		set @bAllWithoutOptions = 0

	if @bAllWithoutOptions = 1
		select 	Contract.contractID iContractID, 
				Contract.symbol vcSymbolName, 
				Contract.contractName vcDescription, 
				Stock.primaryExchangeID iExchangeID, 
				Contract.contractTypeID iContractTypeID, 
				Stock.divFreq iDivFrequency, 
				Stock.divAmt fAmount, 
				Stock.divDate dtDivDate,
				Stock.isDivCustom isDivCustom, 
				Stock.divFreqCustom iDivFrequencyCustom, 
				Stock.divAmtCustom fAmountCustom, 
				Stock.divDateCustom dtDivDateCustom,
				IndexView.yield fYield,
				Contract.actionDate as dtActionDate,
				Contract.actionID as tiActionID,
				case 
					when Stock.stockID is not null then Stock.calcOptionType 
					else IndexView.calcOptionType end tiCalcOptionType,
				case 
					when Stock.stockID is not null then Stock.IsHTB 
					else IndexView.IsHTB end bIsHTB,
				case 
					when Stock.stockID is not null then Stock.skew 
					else IndexView.skew end fSkew,
				case 
					when Stock.stockID is not null then Stock.kurt 
					else IndexView.kurt end fKurt,

				Contract.undPriceProfileID as iUndPriceProfileID,
				Contract.optPriceProfileID as iOptPriceProfileID
			from ContractView Contract
				left join StockView Stock on Contract.contractID = Stock.stockID
				left join IndexView on Contract.contractID = IndexView.indexID
			where contractTypeID < 3
				and (@iContractID is null or contractID = @iContractID)
			order by Contract.symbol
	else
		select 	Contract.contractID iContractID, 
				Contract.symbol vcSymbolName, 
				Contract.contractName vcDescription, 
				Stock.primaryExchangeID iExchangeID, 
				Contract.contractTypeID iContractTypeID, 
				Stock.divFreq iDivFrequency, 
				Stock.divAmt fAmount, 
				Stock.divDate dtDivDate,
				Stock.isDivCustom isDivCustom, 
				Stock.divFreqCustom iDivFrequencyCustom, 
				Stock.divAmtCustom fAmountCustom, 
				Stock.divDateCustom dtDivDateCustom,
				IndexView.yield fYield,
				Contract.actionDate as dtActionDate,
				Contract.actionID as tiActionID,
				case 
					when Stock.stockID is not null then Stock.calcOptionType 
					else IndexView.calcOptionType end tiCalcOptionType,
				case 
					when Stock.stockID is not null then Stock.IsHTB 
					else IndexView.IsHTB end bIsHTB,
				case 
					when Stock.stockID is not null then Stock.skew 
					else IndexView.skew end fSkew,
				case 
					when Stock.stockID is not null then Stock.kurt 
					else IndexView.kurt end fKurt,

				Contract.undPriceProfileID as iUndPriceProfileID,
				Contract.optPriceProfileID as iOptPriceProfileID
			from ContractView Contract
				left join StockView Stock on Contract.contractID = Stock.stockID
				left join IndexView on Contract.contractID = IndexView.indexID
			where (@iContractTypeID is null or contractTypeID = @iContractTypeID)
				and (@iContractID is null or contractID = @iContractID)
			order by Contract.symbol
	
Return(@@Error)

GO

--========================================================================================================
-- MsUnderlyingCache table
--========================================================================================================

if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[MsUnderlyingCache]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
begin
	CREATE TABLE [dbo].[MsUnderlyingCache]
	(
		[underlyingID] [int] NOT NULL ,
		[contractName] [varchar] (255) NULL ,
		[divDate] [smalldatetime] NULL ,
		[divFreq] [int] NULL ,
		[divAmt] [float] NULL,
		[updateDate] [datetime] NULL
		CONSTRAINT [PK_MsUnderlyingCache] PRIMARY KEY  CLUSTERED 
		(
			[underlyingID]
		)
	)
end
GO

--========================================================================================================
-- MsOptionRootCache table
--========================================================================================================

if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[MsOptionRootCache]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
begin
	CREATE TABLE [dbo].[MsOptionRootCache]
	(
		[optionRootID] [int] IDENTITY (1, 1) NOT NULL ,
		[underlyingID] [int] NOT NULL ,
		[symbol] [varchar] (20) NOT NULL ,
		[lotSize] [int] NOT NULL
		CONSTRAINT [PK_MsOptionRootCache] PRIMARY KEY  CLUSTERED 
		(
			[optionRootID]
		)
	)

end
GO

if not exists(select * from sysindexes where id=object_id('MsOptionRootCache') and name='IX_MsOptionRootCache_underlyingID')
	CREATE INDEX [IX_MsOptionRootCache_underlyingID] ON [dbo].[MsOptionRootCache]([underlyingID])
GO

if not exists(select * from sysindexes where id=object_id('MsOptionRootCache') and name='IX_MsOptionRootCache_underlyingID_optionRootID')
	CREATE INDEX [IX_MsOptionRootCache_underlyingID_optionRootID] ON [dbo].[MsOptionRootCache]([underlyingID],[optionRootID])
GO

if not exists(select * from sysindexes where id=object_id('MsOptionRootCache') and name='IX_MsOptionRootCache_symbol')
	CREATE INDEX [IX_MsOptionRootCache_symbol] ON [dbo].[MsOptionRootCache]([symbol])
GO


--========================================================================================================
-- MsOptionCache table
--========================================================================================================

if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[MsOptionCache]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
begin
	CREATE TABLE [dbo].[MsOptionCache]
	(
		[optionID] [int] IDENTITY (1, 1) NOT NULL ,
		[optionRootID] [int] NOT NULL ,
		[symbol] [varchar] (20) NOT NULL ,
		[contractName] [varchar] (255) NULL ,
		[expiryID] [int] NOT NULL ,
		[strike] [float] NOT NULL ,
		[isCall] [tinyint] NOT NULL
		CONSTRAINT [PK_MsOptionCache] PRIMARY KEY  CLUSTERED 
		(
			[optionID]
		)
	)
end
GO

if not exists(select * from sysindexes where id=object_id('MsOptionCache') and name='IX_MsOptionCache_optionRootID')
	CREATE INDEX [IX_MsOptionCache_optionRootID] ON [dbo].[MsOptionCache]([optionRootID])
GO

if not exists(select * from sysindexes where id=object_id('MsOptionCache') and name='IX_MsOptionCache_optionRootID_optionID')
	CREATE INDEX [IX_MsOptionCache_optionRootID_optionID] ON [dbo].[MsOptionCache]([optionRootID],[optionID])
GO

if not exists(select * from sysindexes where id=object_id('MsOptionCache') and name='IX_MsOptionCache_symbol')
	CREATE INDEX [IX_MsOptionCache_symbol] ON [dbo].[MsOptionCache]([symbol])
GO

--========================================================================================================
-- usp_EodMsLoad_Init
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodMsLoad_Init' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodMsLoad_Init
GO

CREATE PROCEDURE dbo.usp_EodMsLoad_Init
as
/*
	Created by Sharky
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction MsCacheClear

	TRUNCATE TABLE MsOptionCache
	if @@error <> 0 begin set @error = -1 if @@TranCount > 0  Rollback Transaction MsCacheClear goto finish end

	TRUNCATE TABLE MsOptionRootCache
	if @@error <> 0 begin set @error = -2 if @@TranCount > 0  Rollback Transaction MsCacheClear goto finish end

	TRUNCATE TABLE MsUnderlyingCache
	if @@error <> 0 begin set @error = -2 if @@TranCount > 0  Rollback Transaction MsCacheClear goto finish end

	Commit Transaction MsCacheClear

finish:
	return @error
GO

--========================================================================================================
-- usp_EodMsUnderlying_Save
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodMsUnderlying_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodMsUnderlying_Save
GO

CREATE PROCEDURE dbo.usp_EodMsUnderlying_Save
	@iUnderlyingID int,
	@vcContractName varchar(255) = null,
	@dtDivDate smalldatetime = null,
	@iDivFreq int = null,
	@fDivAmt float = null
AS
/*
	Created by Sharky
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	declare @error int		set @error = 0

	-- check parameter
	if @iUnderlyingID is null
	begin
		raiserror ('Invalid parameter!', 16, 1)
		set @error = -1
		goto finish 
	end

	-- check underlying existance
	if not exists (select 1 from ContractView where contractID = @iUnderlyingID and contractTypeID < 3)
	begin
		raiserror ('Invalid underlying contract!', 16, 1)
		set @error = -2
		goto finish 
	end

	-- update underlying cache
	Begin Transaction MsUnderlyingCache

	if not exists (select 1 from MsUnderlyingCache where underlyingID = @iUnderlyingID)
	begin
		insert into MsUnderlyingCache
			(underlyingID, contractName, divDate, divFreq, divAmt, updateDate)
		values
			(@iUnderlyingID, @vcContractName, @dtDivDate, @iDivFreq, @fDivAmt, getdate())

		if @@error <> 0 begin set @error = -3 if @@TranCount > 0  Rollback Transaction MsUnderlyingCache goto finish end
	end
	else
	begin
		update
			MsUnderlyingCache
		set
			contractName = @vcContractName,
			divDate = @dtDivDate,
			divFreq = @iDivFreq,
			divAmt = @fDivAmt,
			updateDate = getdate()
		where
			underlyingID = @iUnderlyingID

		if @@error <> 0 begin set @error = -4 if @@TranCount > 0  Rollback Transaction MsUnderlyingCache goto finish end
	end

	Commit Transaction MsUnderlyingCache

finish:
	return @error
GO

--========================================================================================================
-- usp_EodMsOption_Save
--========================================================================================================

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodMsOption_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodMsOption_Save
GO

CREATE PROCEDURE dbo.usp_EodMsOption_Save
	@iUnderlyingID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255) = null,
	@dtExpiry datetime,
	@fStrike float,
	@tiIsCall tinyint,
	@iLotSize int
AS
/*
	Created by Sharky
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	declare @error int		set @error = 0
	declare @vcUnderlyingSymbol varchar(20)
	declare @vcError as varchar(8000)

	-- check parameter
	if @iUnderlyingID is null or @vcSymbol is null or @dtExpiry is null or isnull(@fStrike, 0) <= 0
	begin
		raiserror ('Invalid parameter!', 16, 1)
		set @error = -1
		goto finish 
	end

	-- check underlying existance
	if not exists (select 1 from MsUnderlyingCache where underlyingID = @iUnderlyingID)
	begin
		raiserror ('Invalid underlying contract!', 16, 1)
		set @error = -2
		goto finish 
	end

	-- check the Expiry
	declare @iExpiryID int
	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	if not exists (select 1 from ExpiryView where expiryID = @iExpiryID)
	begin
		set @vcError = 'Invalid expiry ''' + Convert(varchar(50),@dtExpiry) + ''' for option ''' + @vcSymbol + ''' !'
		Raiserror(@vcError, 16, 1)
		set @error = -3
		goto finish 
	end

	-- check option root symbol
	if len(@vcSymbol) < 3
	begin
		set @vcError = 'Invalid option symbol ''' + @vcSymbol + ''': at least 3 characters expected!'
		Raiserror(@vcError, 16, 1)
		set @error = -4
		goto finish 
	end

	declare @vsOptionRootSymbol varchar(20)
	set @vsOptionRootSymbol = left(@vcSymbol, len(@vcSymbol) - 2)

	declare @iUnderlyingID_Bad int

	select top 1 @iUnderlyingID_Bad = underlyingID
	from MsOptionRootCache
	where
		symbol = @vsOptionRootSymbol
		and underlyingID <> @iUnderlyingID

	if @iUnderlyingID_Bad is not null
	begin
		select @vcUnderlyingSymbol from ContractView (nolock) where contractID = @iUnderlyingID_Bad and contractTypeID < 3

		set @vcError = 'Invalid option root ''' + @vsOptionRootSymbol + ''': already exists for underlying ''' + @vcUnderlyingSymbol + '''!'
		Raiserror(@vcError, 16, 1)
		set @error = -5
		goto finish 
	end

	declare @iOptionRootID int
	select @iOptionRootID = optionRootID from MsOptionRootCache where symbol = @vsOptionRootSymbol and underlyingID = @iUnderlyingID

	-- correct lotsize
	if isnull(@iLotSize, 0) <= 0
		set @iLotSize = 100

	Begin Transaction MsOptionCache

	-- insert new option root
	if @iOptionRootID is null
	begin
		insert into MsOptionRootCache
			(underlyingID, symbol, lotSize)
		values
			(@iUnderlyingID, @vsOptionRootSymbol, @iLotSize)

		set @iOptionRootID = @@identity

		if @@error <> 0 begin set @error = -6 if @@TranCount > 0  Rollback Transaction MsOptionCache goto finish end
	end

	-- check option symbol
	declare @iOptionID int

	select @iOptionID = optionID
	from MsOptionCache
	where
		optionRootID = @iOptionRootID
		and ((symbol = @vcSymbol
			and (expiryID <> @iExpiryID
				or abs(strike - @fStrike) >= 0.0009
				or isCall <> @tiIsCall))
		or
			(symbol <> @vcSymbol
			and expiryID = @iExpiryID
			and abs(strike - @fStrike) < 0.0009
			and isCall = @tiIsCall))

	if @iOptionID is not null
	begin
		select @vcUnderlyingSymbol from ContractView (nolock) where contractID = @iUnderlyingID and contractTypeID < 3

		set @vcError = 'Invalid option ''' + @vcSymbol + ''': already exists for underlying ''' + @vcUnderlyingSymbol + ''' with different parameters or name!'
		Raiserror(@vcError, 16, 1)
		set @error = -7
		goto finish 
	end

	select @iOptionID = optionID from MsOptionCache where optionRootID = @iOptionRootID and symbol = @vcSymbol
	
	-- insert new option
	if @iOptionID is null
	begin
		if @vcContractName is null
		  	set @vcContractName = @vcUnderlyingSymbol + ' ' + datename(month, @dtExpiry) + 
							' ' + datename(year, @dtExpiry) + ' ' + case @tiIsCall when 1 then 'Call' else 'Put' end +
							 ' ' + cast(round(@fStrike, 2) as varchar)
	
		insert into MsOptionCache
			(optionRootID, symbol, contractName, expiryID, strike, isCall)
		values
			(@iOptionRootID, @vcSymbol, @vcContractName, @iExpiryID, @fStrike, @tiIsCall)
	
		set @iOptionID = @@identity
		if @@error <> 0 begin set @error = -8 if @@TranCount > 0  Rollback Transaction MsOptionCache goto finish end
	end

	Commit Transaction MsOptionCache

finish:
	if @error = 0
		return @iOptionID
	else
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.43.081')
	begin
		update DataInfo set keyValue = '3.43.081' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.43.081' '3.43.081 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
