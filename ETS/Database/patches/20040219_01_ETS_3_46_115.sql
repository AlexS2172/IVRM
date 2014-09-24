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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.45.114' or (keyValue = '3.46.115')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.115 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.115' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

----------------------------------------------------------------------------------------------------
-- remove old SPs
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Strategy_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Strategy_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderContract_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContract_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderContract_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContractStrategy_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderContractStrategy_Import]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContractStrategyWithSave_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderContractStrategyWithSave_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IsNewStrategyForUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_IsNewStrategyForUnderlying_Get]
GO

----------------------------------------------------------------------------------------------------
-- Trader
----------------------------------------------------------------------------------------------------
if (select OBJECT_ID('FK_Trader_DefStrategyID')) is not null
	ALTER TABLE dbo.Trader DROP CONSTRAINT FK_Trader_DefStrategyID
GO

if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Trader' and COLUMN_NAME='defStrategyID')
    ALTER TABLE [dbo].[Trader] DROP COLUMN [defStrategyID]
GO

----------------------------------------------------------------------------------------------------
-- TraderContract
----------------------------------------------------------------------------------------------------
if (select OBJECT_ID('FK_TraderContract_StrategyID')) is not null
	ALTER TABLE dbo.TraderContract DROP CONSTRAINT FK_TraderContract_StrategyID
GO

if exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='TraderContract' and COLUMN_NAME='strategyID')
    ALTER TABLE [dbo].[TraderContract] DROP COLUMN [strategyID]
GO

----------------------------------------------------------------------------------------------------
-- TraderView
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TraderView]') and OBJECTPROPERTY(id, N'IsView') = 1)
    drop view [dbo].[TraderView]
GO

CREATE VIEW dbo.TraderView
as
	Select 
		traderID,
		acronym,
		traderName,
		clearingTraderCode,
		traderGroupID,
		actionID,
		actionDate,
		tntCardLastID,
		tntCardLastDate
	from
		Trader
	where
		actionID < 3
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTrader_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTrader_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTrader_Save]
GO

CREATE PROC dbo.usp_BoTrader_Save
    @iTraderID int,
    @vcAcronym varchar(12) = null,
    @vcTraderName varchar(120) = null,
    @vcClearingTraderCode varchar(11) = null,
    @iTraderGroupID int = null, -- null group == Default group !!!
	@tiRaiseError tinyint = 1
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if exists (select 1 from TraderView where TraderName = @vcTraderName and (@iTraderID is null or @iTraderID <> traderID))
		begin
			if isnull(@tiRaiseError, 0) <> 0 raiserror ('Trader with this name already exists!', 16, 1)
			set @error = -1
			goto finish
		end
   
	if exists (select 1 from TraderView where Acronym = @vcAcronym and (@iTraderID is null or @iTraderID <> traderID))
		begin
			if isnull(@tiRaiseError, 0) <> 0 raiserror ('Trader with this acronym already exists!', 16, 1)
			set @error = -2
			goto finish
		end   

	begin transaction
	
	if @iTraderID is null
	begin
		-- insert new trader
		insert into Trader (Acronym, TraderName, clearingTraderCode, traderGroupID)
			values (@vcAcronym, @vcTraderName, @vcClearingTraderCode, @iTraderGroupID)

		if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

		set @iTraderID = @@identity
	end
	else
	begin
		-- update trader
		update Trader 
        set acronym = isnull(@vcAcronym, acronym),
	        traderName = isnull(@vcTraderName, traderName),
			clearingTraderCode = isnull(@vcClearingTraderCode, clearingTraderCode),
			traderGroupID = @iTraderGroupID
	    where traderID = @iTraderID

		if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end
	end

	commit transaction

finish:
	if @error = 0
		return @iTraderID
	else
		return @error
GO


----------------------------------------------------------------------------------------------------
-- usp_BoTrader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTrader_Get]
GO

CREATE PROC dbo.usp_BoTrader_Get
	@iTraderID int = Null
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	
    if @iTraderID is Null
    	select t.traderID as iTraderID, 
			t.acronym as vcAcronym, 
			t.traderName as vcTraderName, 
			t.clearingTraderCode as vcClearingTraderCode,
			t.traderGroupID as iTraderGroupID,
			tg.trgName as vcGroupName
		from TraderView t
			left join TraderGroup tg on t.traderGroupID = tg.trgID
		order by t.acronym
    else
    	select t.traderID as iTraderID, 
			t.acronym as vcAcronym, 
			t.traderName as vcTraderName, 
			t.clearingTraderCode as vcClearingTraderCode,
			t.traderGroupID as iTraderGroupID,
			tg.trgName as vcGroupName
		from TraderView t
			left join TraderGroup tg on t.traderGroupID = tg.trgID
		where
			t.traderID = @iTraderID

	return @@error
GO


----------------------------------------------------------------------------------------------------
-- usp_Trader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trader_Get]
GO

CREATE PROC dbo.usp_Trader_Get
	@iTraderID int = Null
as
	------------------------------------------------
	set nocount on
	------------------------------------------------

	if @iTraderID is Null
		select
			traderID as iTraderID, 
			acronym as vcAcronym, 
			traderName as vcTraderName, 
			clearingTraderCode as vcClearingTraderCode,
			actionDate as dtActionDate,
			tntCardLastID as iTntCardLastID,
			tntCardLastDate as dtTntCardLastDate,
			traderGroupID as iTraderGroupID
		from TraderView
		order by acronym
	else 
		select
			traderID as iTraderID, 
			acronym as vcAcronym, 
			traderName as vcTraderName, 
			clearingTraderCode as vcClearingTraderCode,
			actionDate as dtActionDate,
			tntCardLastID as iTntCardLastID,
			tntCardLastDate as dtTntCardLastDate,
			traderGroupID as iTraderGroupID
		from TraderView
		where traderID = @iTraderID
		order by acronym

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- TraderContractView
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TraderContractView]') and OBJECTPROPERTY(id, N'IsView') = 1)
    drop view [dbo].[TraderContractView]
GO

CREATE VIEW dbo.TraderContractView
AS
	select
		traderID, 
		contractID
	from
		dbo.TraderContract
	where
		actionID < 3
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTraderContract_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTraderContract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTraderContract_Get]
GO

CREATE PROC dbo.usp_BoTraderContract_Get
	@iTraderID int = null,
	@iContractID int = null
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	
	if @iContractID is null 
	begin
		if @iTraderID is null
			select 
				TV.traderID as iTraderID,
				TV.contractID as iContractID,
				C.symbol as vcContractSymbol
			from TraderContractView TV
				join ContractView C on  C.contractID = TV.contractID
			order by TV.traderID, C.symbol
		else
			select 
				TV.traderID as iTraderID,
				TV.contractID as iContractID,
				C.symbol as vcContractSymbol
			from TraderContractView TV
				join ContractView C on  C.contractID = TV.contractID
			where 
				TV.traderID = @iTraderID
			order by TV.traderID, C.symbol
	end
	else
	begin
		if @iTraderID is null
			select 
				TV.traderID as iTraderID,
				TV.contractID as iContractID,
				C.symbol as vcContractSymbol
			from TraderContractView TV
				join ContractView C on  C.contractID = TV.contractID
			where
				TV.contractID = @iContractID
			order by TV.traderID, C.symbol
		else
			select 
				TV.traderID as iTraderID,
				TV.contractID as iContractID,
				C.symbol as vcContractSymbol
			from TraderContractView TV
				join ContractView C on  C.contractID = TV.contractID
			where 
				TV.traderID = @iTraderID
				and TV.contractID = @iContractID
			order by TV.traderID, C.symbol
	end

	return @@error

GO

----------------------------------------------------------------------------------------------------
-- usp_BoTraderContract_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTraderContract_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTraderContract_Del]
GO

CREATE PROCEDURE dbo.usp_BoTraderContract_Del 
	@iTraderID int,
	@iContractID int = null
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	if @iContractID is null
	begin
		if exists(select 1 from Trade where traderID = @iTraderID)
			Return(-1)

		if exists(select 1 from [Order] where traderID = @iTraderID)
			Return(-2)
			
		Begin Transaction	
				
		update TraderContract set actionID=3
			where traderID = @iTraderID

		if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

		set @error = 0
						
		Commit Transaction

	end
	else 
	begin
		if exists(select 1 from Trade where traderID = @iTraderID and contractID = @iContractID)
			Return(-1)

		if exists(select 1 from [Order] where traderID = @iTraderID and contractID = @iContractID) -- !! need to add underlying id into orders
			Return(-2)
			
		Begin Transaction	
		
		update TraderContract set actionID=3
			where traderID = @iTraderID and contractID=@iContractID

		if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

		set @error = 0
		
		Commit Transaction

	end
	
finish:
	return @error

GO

----------------------------------------------------------------------------------------------------
-- usp_BoTraderContract_Save
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTraderContract_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTraderContract_Save]
GO

CREATE PROC dbo.usp_BoTraderContract_Save
	@iTraderID int,
	@iContractID int
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------

	declare @error int set @error = 0

	if not exists(select 1 from TraderContractView where traderID = @iTraderID and contractID = @iContractID)		
	begin
		begin transaction
		
		insert into TraderContract
			(traderID, contractID)
		values
			(@iTraderID, @iContractID)
		if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end			
	
		commit transaction
	end
			
finish:
	return @error

GO

----------------------------------------------------------------------------------------------------
-- usp_BoStrategy_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoStrategy_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoStrategy_Del]
GO

CREATE PROC dbo.usp_BoStrategy_Del
    @iStrategyID int
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	if exists(select * from Trade where strategyID = @iStrategyID)
		Return(-1)

	begin transaction
		
	update Strategy set actionID = 3 where strategyID = @iStrategyID
		if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

	commit transaction

finish:
	return @error

GO

----------------------------------------------------------------------------------------------------
-- usp_BoTraderUnderlyings_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTraderUnderlyings_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTraderUnderlyings_Get]
GO

CREATE PROCEDURE dbo.usp_BoTraderUnderlyings_Get
	@iTraderID int,
	@tiOnlySelected tinyint = 1
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	
	if @tiOnlySelected = 0
	begin
		select 
			C.contractID as iContractID,
			C.symbol as vcUndSymbol,
			max(case when(TC.traderID = @iTraderID) then 1 else 0 end) as tiIsSelected
		from ContractView C
			left join TraderContractView TC on TC.contractID = C.contractID 
		group by 
			C.contractID, C.symbol, C.contractTypeID
		having 
			C.contractTypeID < 3
		order by tiIsSelected desc, C.symbol
	end
	else
	begin
		select distinct
			TC.contractID as iContractID,
			C.symbol as vcUndSymbol,
			1 as tiIsSelected
		from TraderContractView TC
			left join ContractView C on TC.contractID = C.contractID 
		where C.contractTypeID < 3 and TC.traderID=@iTraderID
		order by C.symbol
	end	
	return @@error
GO

----------------------------------------------------------------------------------------------------
-- dbo.usp_BoStrategy_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoStrategy_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoStrategy_Del]
GO

CREATE PROC dbo.usp_BoStrategy_Del
    @iStrategyID int
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	if exists(select * from Trade where strategyID = @iStrategyID)
		Return(-1)

	begin transaction
		
	update Strategy set actionID = 3 where strategyID = @iStrategyID
		if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

	commit transaction

finish:
	return @error

GO


----------------------------------------------------------------------------------------------------
-- dbo.usp_TradeFeed_1_New
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_1_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeFeed_1_New]
GO

CREATE PROC dbo.usp_TradeFeed_1_New
	@vcTraderAcronym varchar(12),
	@vcExecID varchar(20),
	@tiIsBuy tinyint,  
	@iQuantity int,
	@fPrice float,
	@dtTradeDate datetime,
	@vcContractSymbol varchar(20),
	@iContractTypeID int,
	@vcUnderlyingSymbol varchar(20) = null,
	@tiIsCall tinyint = null,
	@fStrike float = null,
	@dtExpiry datetime = null,
	@iTradeLotSize int = null,
	@tiIsUpdateEnabled tinyint = 0,
	@iLastTradeTime int = null, 
	@vcOptRootSymbol varchar(20) = null,
	@iMark int = null,
	@vcStrategyName varchar(12) = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iMinSeqNum int 
		select @iMinSeqNum = max(seqNum)
			from Trade 
			where 	execID = @vcExecID and 
					actionID > 0 and 
					Convert(varchar(8),@dtTradeDate,112) = Convert(varchar(8),tradeDate,112)

	if @tiIsUpdateEnabled = 1
		begin
			if @iMinSeqNum is not null 
				begin
					exec @error = usp_TradeFeed_1_Del @vcExecID, @dtTradeDate, @tiIsUpdateEnabled
					---if removed succesfully
					if @error != -1
						begin	
							select @iMinSeqNum = max(seqNum)
								from Trade 
								where execID = @vcExecID and 
									actionID = 0 and 
									Convert(varchar(8),@dtTradeDate,112) = Convert(varchar(8),tradeDate,112)
						end
					else	
						begin
							set @iMinSeqNum = 0
						end
				end
		end

	--trade already exists
	else if @iMinSeqNum is not null and @tiIsUpdateEnabled = 0
		begin
			select -1 as iMaxSeqNum, @iMinSeqNum as iMinSeqNum, -1 as iStructureMissed
			Return(-1)
		end 

	Declare @iTradeContractID int, 
			@iUnderlyingContractID int,
			@iStructureMissed int

	set @iStructureMissed = 0

	--check underlying symbol
	if @vcUnderlyingSymbol is null and  @iContractTypeID <3 
		set @vcUnderlyingSymbol = @vcContractSymbol

	select @iUnderlyingContractID = contractID 
		from ContractView 
		where symbol = @vcUnderlyingSymbol and contractTypeID < 3

	Begin Transaction
	if @iUnderlyingContractID is null
	--add new stock\index
		begin
			insert into Contract(symbol,contractTypeID,expCalendarID) values(@vcUnderlyingSymbol, 2, 1)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			set @iUnderlyingContractID = @@identity

			insert into Stock (stockID,isActive) values(@iUnderlyingContractID,1)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

			insert into ContractPrice (contractID,exchangeID) values(@iUnderlyingContractID,null)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

			set @iStructureMissed = 1
		end
	else
		begin  --check this instruction carefully!
			update Stock set isActive = 1 where stockID = @iUnderlyingContractID
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end

	--check Option
	if @iContractTypeID = 3
		begin
			Declare @iOptionID int,
					@iExpiryID int,
					@iOptionRootID int

			Select @iOptionRootID = optionRootID from OptionRootView where symbol = @vcOptRootSymbol
	
			set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
			set @fStrike = round(@fStrike, 3)
	
			select @iOptionID = optionID
				from OptionView (nolock)
				where optionRootID = @iOptionRootID and 
					isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID < 3
	
			if @iOptionID is null
			--create new option
				begin
					if isnull(@vcContractSymbol, '') <> ''
					begin
						Declare @vcContractName varchar(255)
						  	set @vcContractName = @vcUnderlyingSymbol + ' ' + datename(month, @dtExpiry) + 
										' ' + datename(year, @dtExpiry) + ' ' + case @tiIsCall when 1 then 'Call' else 'Put' end +
											 ' ' + cast(round(@fStrike, 2) as varchar)
		
						insert into Contract (contractTypeID, symbol, contractName) values (3, @vcContractSymbol,@vcContractName)
							if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
							set @iOptionID = @@identity
						
						if @iOptionRootID is null
							begin
								insert into OptionRoot(underlyingID,symbol,lotSize) values(@iUnderlyingContractID,@vcOptRootSymbol, isnull(@iTradeLotSize, 100))
									set @iOptionRootID = @@identity
							end

						insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
							 values (@iOptionID, @iOptionRootID, @tiIsCall, @fStrike, @iExpiryID)
							if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
				
						-- make record with null exchange for this Option
						insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
							if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

					end
					else
					begin
						Select null as iSeqNum
						Return(-2)
					end

					set @iStructureMissed = @iStructureMissed | 2
				end

			set @iTradeContractID = @iOptionID
		end
		else
			set @iTradeContractID = @iUnderlyingContractID


	--work with Trader
	Declare @iTraderID int
		select @iTraderID = traderID from TraderView where acronym = @vcTraderAcronym

	if @iTraderID is null
		begin
			insert into Trader(acronym,traderName) values(@vcTraderAcronym, @vcTraderAcronym)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
				set @iTraderID = @@identity
		end

	if not exists(select 1 from TraderContractView where traderID = @iTraderID and contractID = @iUnderlyingContractID)
		begin
			insert into TraderContract (traderID, contractID) values (@iTraderID, @iUnderlyingContractID)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end
	
	declare @iStrategyID int
	set @vcStrategyName = ltrim(rtrim(isnull(@vcStrategyName, '')))

	if len(@vcStrategyName) > 0
	begin
		select @iStrategyID = strategyID from StrategyView where @vcStrategyName = strategyName
		if @iStrategyID is null
		begin
			insert into Strategy (strategyName) values (@vcStrategyName)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			set @iStrategyID = @@identity
		end
	end

	declare @iNewTradeID int
		select @iNewTradeID = max(tradeID) + 1 from Trade

	declare @iSeqNum int 
		select @iSeqNum = isnull(max(seqNum),0) + 1 from Trade

	if @iNewTradeID is null set @iNewTradeID = 1

	--create new trade	
	insert into Trade (execID, tradeID, actionID, contractID, tradeDate, 
		quantity, price, traderID, strategyID, isBuy, isPosition, status, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	values (@vcExecID, @iNewTradeID, 1, @iTradeContractID, @dtTradeDate, 
		@iQuantity, @fPrice, @iTraderID, @iStrategyID, @tiIsBuy, 0, 0, @iSeqNum, @iMark,
		@iUnderlyingContractID, @iOptionRootID, null, null)

	if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

	--write into TradeLog 
	if @iStructureMissed <> 0
		begin
			if exists(select 1 from TradeLog where underlyingID = @iUnderlyingContractID)
			begin
				update TradeLog set eventTypeID = isnull(eventTypeID, 0) | @iStructureMissed
				where underlyingID = @iUnderlyingContractID
					if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			end
			else
			begin
				insert into TradeLog (underlyingID, eventTypeID) values (@iUnderlyingContractID, @iStructureMissed)
					if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
			end
		end

	--save the last successful trade time
	if @iLastTradeTime is not null
		begin
			update DataInfo set keyValue = @iLastTradeTime where keyName='lastTradeTime'
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
		end

	--return output recordset
	select isnull(@iMinSeqNum,0) as iMinSeqNum,@iSeqNum as iMaxSeqNum, @iStructureMissed as iStructureMissed

	Commit Transaction

    set @error = 0
	return @error

finish:
	select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed
	return @@error

GO

----------------------------------------------------------------------------------------------------
-- usp_TraderContractByTrader_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TraderContractByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TraderContractByTrader_Get]
GO

CREATE PROC dbo.usp_TraderContractByTrader_Get
	@iTraderID int = null
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	
	if @iTraderID is null
		select 
			traderID as iTraderID,
			contractID as iContractID
		from TraderContractView
	else
		select 
			traderID as iTraderID,
			contractID as iContractID
		from TraderContractView
		where 
			traderID = @iTraderID

	return @@error

GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_PositionTransfer
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_PositionTransfer]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_PositionTransfer]
GO

CREATE PROC dbo.usp_Trade_PositionTransfer
	@iContractID int,
	@iQuantity int,
	@fPrice float,
	@tiStatus tinyint,

	@iTraderIDFrom int,
	@iStrategyIDFrom int,
	@iMarkFrom int,
    @tiIsBuyFrom tinyint,  

	@iTraderIDTo int,
	@iStrategyIDTo int,
	@iMarkTo int,
    @tiIsBuyTo tinyint,  

	@iSeqNumFrom int out,
	@iTradeIDFrom int out,
	@dtTradeDateFrom datetime out,

	@iSeqNumTo int out,
	@iTradeIDTo int out,
	@dtTradeDateTo datetime out,

	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null
as
	set nocount on

	begin tran

	declare @error int		set @error = 0

	if not exists(select top 1 1 from TraderContractView where traderID = @iTraderIDFrom and contractID = @iUnderlyingID)
	begin
		insert into TraderContract (traderID, contractID) values (@iTraderIDFrom, @iUnderlyingID)
			if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	end

	if not exists(select top 1 1 from TraderContractView where traderID = @iTraderIDTo and contractID = @iUnderlyingID)
	begin
		insert into TraderContract (traderID, contractID) values (@iTraderIDTo, @iUnderlyingID)
			if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	end

	declare @iDeltaGMT int
	exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

	-- Trade 'from' creation
	if @dtTradeDateFrom is null	
		set @dtTradeDateFrom = dateadd(minute, @iDeltaGMT, getdate())

	-- get new tradeID ane seqNum for trade from
	select 
		@iTradeIDFrom = isnull(max(tradeID), 0) + 1,
		@iSeqNumFrom = isnull(max(seqNum), 0) + 1
	from Trade

	-- create new trade from
	insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, strategyID,
		isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	values (@iTradeIDFrom, 1, @iContractID, @dtTradeDateFrom, @iQuantity, @fPrice, @iTraderIDFrom, @iStrategyIDFrom,
		@tiIsBuyFrom, null, null, null, null,
		null, null, 0, @tiStatus, @iSeqNumFrom, @iMarkFrom,
		@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID)

	if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

	-- Trade 'to' creation
	if @dtTradeDateTo is null	
		set @dtTradeDateTo = dateadd(minute, @iDeltaGMT, getdate())

	-- get new tradeID ane seqNum for trade to
	select 
		@iTradeIDTo = isnull(max(tradeID), 0) + 1,
		@iSeqNumTo = isnull(max(seqNum), 0) + 1
	from Trade

	-- create new trade to
	insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, strategyID,
		isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	values (@iTradeIDTo, 1, @iContractID, @dtTradeDateTo, @iQuantity, @fPrice, @iTraderIDTo, @iStrategyIDTo,
		@tiIsBuyTo, null, null, null, null,
		null, null, 0, @tiStatus, @iSeqNumTo, @iMarkTo,
		@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID)

	if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

	commit tran

finish:
	if @error <> 0
	begin
		set @iSeqNumFrom = null
		set @iTradeIDFrom = null
		set @dtTradeDateFrom = null
	
		set @iSeqNumTo = null
		set @iTradeIDTo = null
		set @dtTradeDateTo = null
	end

	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_New
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_New]
GO

CREATE PROC dbo.usp_Trade_New
    @iContractID int,
    @iQuantity int,
    @fPrice float,
    @tiIsBuy tinyint,  
    @iTraderID int,
    @iStrategyID int = null,
    @iBrokerID int = Null,
    @fBrokerCommission float =Null,
    @iClearingBrokerID int = Null,
    @fClearingBrokerCommission float =Null,
    @fTradedIV float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime = Null out,
	@tiStatus tinyint,
	@iMark int = null,
	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null,
    @vcImportID varchar(20) = null out
as
	set nocount on

	declare @error int		set @error = 0

	-- check for valid contract parameters
	exec @error = usp_ContractParameters_CheckValidity @iContractID, @iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID
	if @error <> 0 goto finish

	begin tran

	if not exists(select top 1 1 from TraderContractView where traderID = @iTraderID and contractID = @iUnderlyingID)
	begin
		insert into TraderContract (traderID, contractID) values (@iTraderID, @iUnderlyingID)
			if @@error <> 0 begin set @error = -1 if @@TranCount > 0 Rollback Transaction goto finish end
	end

	if @dtNewTradeDate is null	
	begin
		declare @iDeltaGMT int

		exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

		set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	end

	-- get new tradeID ane seqNum
	declare @iNewSeqNum int

	select 
		@iNewTradeID = isnull(max(tradeID), 0) + 1,
		@iNewSeqNum = isnull(max(seqNum), 0) + 1
	from Trade

	-- create new trade
	insert into Trade (tradeID, actionID, contractID, tradeDate,  quantity, price, traderID, strategyID,
		isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
		tradedIV, spotReference, isPosition, status, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, @iStrategyID, @tiIsBuy,
		@iBrokerID, @fBrokerCommission, @iClearingBrokerID, @fClearingBrokerCommission,
		@fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, @iNewSeqNum, @iMark,
		@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID)

	if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

	select @vcImportID = exportSymbol from ContractView where contractID = @iContractID

	if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

	commit tran

finish:
	if @error = 0
		return @iNewSeqNum
	else  
	begin
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		set @vcImportID = null
		return @error
	end
GO

----------------------------------------------------------------------------------------------------
-- usp_Trade_Update
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Trade_Update]
GO

CREATE PROC dbo.usp_Trade_Update
	@iOldTradeID int,
	@iContractID int,
	@iQuantity int,
	@fPrice float,
	@tiIsBuy tinyint,
	@iTraderID int,
    @iStrategyID int,
	@iBrokerID int = Null,
	@fBrokerCommission float =Null,
    @iClearingBrokerID int = Null,
    @fClearingBrokerCommission float =Null,
	@fTradedIV  float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime out,
	@tiStatus tinyint,
	@iMark int = null,
	@iUnderlyingID int,
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null,
    @vcImportID varchar(20) = Null out
as
	set nocount on

	declare @error int		set @error = 0

	declare @iNewSeqNum int,
			@vcExecID varchar(20)

	if @iOldTradeID is not null
	begin
		begin tran

		declare @nCount int
		
		set @nCount = 0
		select @nCount = count(tradeID) from Trade where tradeID = @iOldTradeID
		
		if @nCount = 1
		begin
			select @iNewSeqNum = isnull(max(seqNum), 0) + 1 from Trade
			select @vcExecID = execID from Trade where tradeID = @iOldTradeID

			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID)
			select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, @iNewSeqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID
			from Trade
			where tradeID = @iOldTradeID

			if @@error <> 0 or @error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
		end

		if not exists(select top 1 1 from TraderContractView where traderID = @iTraderID and contractID = @iUnderlyingID)
		begin
			insert into TraderContract (traderID, contractID) values (@iTraderID, @iUnderlyingID)
				if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction goto finish end
		end
	
		if @dtNewTradeDate is null	
		begin
			declare @iDeltaGMT int

			exec @iDeltaGMT = usp_TimeZoneActimeBias_Get
	
			set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
		end

		select 
			@iNewTradeID = isnull(max(tradeID), 0) + 1,
			@iNewSeqNum = isnull(max(seqNum), 0) + 1
		from Trade
	
		insert into Trade (tradeID, actionID, contractID, tradeDate, quantity, price, traderID, 
			strategyID, isBuy, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
			tradedIV, spotReference, isPosition, status, seqNum, execID, mark,
			underlyingID, optionRootID, futureRootID, futureID)
		values (@iNewTradeID, 1, @iContractID, @dtNewTradeDate, @iQuantity, @fPrice, @iTraderID, 
			@iStrategyID,  @tiIsBuy, @iBrokerID, @fBrokerCommission, @iClearingBrokerID, 
			@fClearingBrokerCommission, @fTradedIV, @fSpotReference, @tiIsPosition, @tiStatus, 
			@iNewSeqNum, @vcExecID, @iMark,
			@iUnderlyingID, @iOptionRootID, @iFutureRootID, @iFutureID)

		if @@error <> 0 or @error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end

		select @vcImportID = exportSymbol from ContractView where contractID = @iContractID
	
		if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

		commit tran
	end
	else
		set @error = -4

finish:
	if @error = 0
		return @iNewSeqNum
	else  
	begin
		set @iNewTradeID = null
		set @dtNewTradeDate = null
		set @vcImportID = null
		return @error
	end
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTraderContractStrategy_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTraderContractStrategy_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTraderContractStrategy_Get]
GO

CREATE PROC dbo.usp_BoTraderContractStrategy_Get
    @iTraderID int,
    @iUnderlyingID int = null
as
	-- returns a list of underlying-strategy pairs that has position for given trader

	set nocount on

	if @iUnderlyingID is not null
		select
			c.contractID as iUnderlyingID,
			c.symbol as vcUnderlyingSymbol,
			s.strategyID as iStrategyID,
			s.strategyName as vcStrategyName
		from
			TradeView tr
			inner join ContractView c on c.contractID = tr.underlyingID
			inner join StrategyView s on s.strategyID = tr.strategyID
		where
			tr.traderID = @iTraderID
			and tr.underlyingID = @iUnderlyingID
		group by
			c.contractID, c.symbol, s.strategyID, s.strategyName
	else
		select
			c.contractID as iUnderlyingID,
			c.symbol as vcUnderlyingSymbol,
			s.strategyID as iStrategyID,
			s.strategyName as vcStrategyName
		from
			TradeView tr
			inner join ContractView c on c.contractID = tr.underlyingID
			inner join StrategyView s on s.strategyID = tr.strategyID
		where
			tr.traderID = @iTraderID
		group by
			c.contractID, c.symbol, s.strategyID, s.strategyName

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_BoStrategyTraderContract_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoStrategyTraderContract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoStrategyTraderContract_Get]
GO

CREATE PROC dbo.usp_BoStrategyTraderContract_Get
    @iStrategID int
as
	-- returns a list of trader-underlying pairs that has position for given strategy

	set nocount on

	select
		t.traderID as iTraderID,
		t.acronym as vcAcronym,
		c.contractID as iUnderlyingID,
		c.symbol as vcUnderlyingSymbol
	from
		TradeView tr
		inner join ContractView c on c.contractID = tr.underlyingID
		inner join TraderView t on t.traderID = tr.traderID
	where
		tr.strategyID = @iStrategID
	group by
		t.traderID, t.acronym, c.contractID, c.symbol

	return @@error
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.115')
	begin
		update DataInfo set keyValue = '3.46.115' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.115' '3.46.115 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
