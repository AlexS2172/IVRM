/*
There are fixes for:
	'MLine00000527 Trades from SAGE: if MS n/a then show this in ETS'
	'MLine00000534 Adding Skew &Kurt Opt Model'
	'MLine00000532 Reconciliation: close expired options'
	and oher DB changes 

DB version:	3.35.009

*/


--=======================================================================================
-- Tables modification
--=======================================================================================
set nocount on
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TradeLog]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[TradeLog]
GO
CREATE TABLE [dbo].[TradeLog] (
	[contractID] [int] NOT NULL ,
	[eventTypeID] [tinyint] NULL 
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[TradeLog] WITH NOCHECK ADD 
	CONSTRAINT [IX_TradeLog_ContractID] UNIQUE  NONCLUSTERED 
	(
		[contractID]
	)  ON [PRIMARY] 
GO
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Index' and COLUMN_NAME='skew')
	begin
		Alter Table [Index]
			Add skew float null
	end
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Index' and COLUMN_NAME='kurt')
	begin
		Alter Table [Index]
			Add kurt float null
	end
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Stock' and COLUMN_NAME='skew')
	begin
		Alter Table Stock
			Add skew float null
	end
GO
if not exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Stock' and COLUMN_NAME='kurt')
	begin
		Alter Table Stock
			Add kurt float null
	end
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ModelParameter]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[ModelParameter]
GO
--=======================================================================================
-- section of SP's and View modification
--=======================================================================================
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[IndexView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[IndexView]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[StockView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[StockView]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE  VIEW dbo.IndexView
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
			kurt
		from [Index]
		where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  VIEW dbo.StockView
AS
	select	stockID, 
			divFreq, 
			isHTB, 
			primaryExchangeID, 
			divAmt, 
			divDate, 
			divAmtCustom, 
            divDateCustom, 
			divFreqCustom, 
			isDivCustom, 
			actionID, 
			actionDate, 
			calcOptionType, 
			calcModelType, 
			isActive,
			skew,
			kurt
		from dbo.Stock
		where actionID < 3

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ModelParameter_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ModelParameter_Del]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ModelParameter_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ModelParameter_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ModelParameter_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ModelParameter_Save]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_SkewKurt_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_SkewKurt_Save]
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
CREATE PROCEDURE dbo.usp_SkewKurt_Save 
	@vcSymbol varchar(20), 
	@fSkew float = null,
	@fKurt float = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is save information into Stock table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iContractID int,
			@iContractTypeID int

	select @iContractID = contractID, @iContractTypeID = ContractTypeID 
		from ContractView 
		where contractTypeID < 3 and upper(symbol) = upper(@vcSymbol)

	Begin Transaction
		if @iContractTypeID = 1
			begin
				update [Index]
					set skew = isnull(@fSkew, skew),
						kurt = isnull(@fKurt, kurt)
				where indexID = @iContractID
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update Stock set 
					skew = isnull(@fSkew, skew),
					kurt = isnull(@fKurt, kurt)
				where stockID = @iContractID
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
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
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[vALLStockEOD]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[vALLStockEOD]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE view dbo.vALLStockEOD
AS
SELECT 	C.contractID AS iContractID,   
		C.contractTypeID AS iContractTypeID,    
		C.Symbol AS vcSymbol, 
    	S.divFreq AS iDivFreq, 
		S.divAmt AS fDivAmt,  
		S.divDate AS dtDivDate,
		S.isHTB as bIsHTB, 
		S.divAmtCustom,
		S.divDateCustom,
		S.divFreqCustom,
		S.isDivCustom,
		S.CalcOptionType as tiCalcOptionType,
		S.CalcModelType as tiCalcModelType,
		C.expCalendarID as tiExpCalendarID,
		null as tiIsBasket,
		S.Skew as fSkew,
		S.Kurt as fKurt
	--FROM ContractView C (nolock), StockView S (nolock)
	FROM Contract C (nolock), StockView S (nolock)
	WHERE S.stockID = C.contractID
UNION
SELECT 	C.contractID AS iContractID,  
		C.contractTypeID AS iContractTypeID,
	    C.Symbol AS vcSymbol,  
		NULL AS iDivFreq, 
		I.yield AS fDivAmt, 
		NULL AS dtDivDate,
		I.isHTB as bIsHTB, 
		null as divAmtCustom,
		null as divDateCustom,
		null as divFreqCustom,
		null as isDivCustom,
		I.CalcOptionType as tiCalcOptionType,
		I.CalcModelType as tiCalcModelType,
		C.expCalendarID as tiExpCalendarID,
		I.isBasket as tiIsBasket,
		I.Skew as fSkew,
		I.Kurt as fKurt
	--FROM ContractView C (nolock), IndexView I (nolock)
	FROM Contract C (nolock), IndexView I (nolock)
	WHERE I.indexID = C.contractID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexWithPos_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexWithPos_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionPermanent_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OptionPermanent_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Import]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ReportStock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ReportStock_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockHH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockHH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockWithPos_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockWithPos_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Import]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeByContractAndTrader_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeByContractAndTrader_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeByExpiredOptions_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeByExpiredOptions_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_New]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeLog_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeLog_Del]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeLog_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeLog_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeLog_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeLog_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingByTrader_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingByTrader_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Underlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Underlying_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_allStockEOD_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_allStockEOD_Get]
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  PROCEDURE dbo.usp_TradeLog_Del 
	@iContractID int = null,
	@iUnderlyingContractID int = null
AS
/*
	Created by Sharky
	Description: this procedure is delete information into TradeLog table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction

	if isnull(@iContractID, 0) = 0 and isnull(@iUnderlyingContractID, 0) = 0
		delete from TradeLog
	else
	begin
		if isnull(@iUnderlyingContractID, 0) = 0
			delete from TradeLog 
			where contractID = @iContractID
		else
			delete from TradeLog 
			where contractID = @iUnderlyingContractID
				and contractID in
					(
						select 
							optionID
						from OptionView
						where underlyingContractID = @iUnderlyingContractID
					)
	end

	if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

	Commit Transaction
    set @error = 0
finish:
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

CREATE             Procedure dbo.usp_Option_Del
	@iOptionID int = null, 	-- contract identifier
	@bisRemovePermanent bit = 0,
	@bisRemoveFromContract bit = 1
as
/*
	Create by Zaur Nuraliev
	Description: It deletes Option data from Option and Contract tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if object_ID('tempdb..#Options_for_delete') is null
		begin	
			Create table #Options_for_delete(OptionID int)
		end
	
	if @iOptionID is not null
		begin
			if exists (select tradeID from Trade where contractID = @iOptionID)
				begin
					Raiserror ('Error. Can''t delete [Option] because of the existing Trade', 16, 1)
					Return (-1)
				end   
			insert into #Options_for_delete
				select @iOptionID
		end
	Begin Transaction DelOpt
		delete from TradeLog where contractID in (select OptionID from #Options_for_delete)

		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update ContractExchangeLink set actionID = 3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete from ModelParameter where contractID in (select OptionID from #Options_for_delete)
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update ContractInGroup set actionID=3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update [Option] set actionID=3 where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update Contract set actionID=3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete Trade where contractID = @iOptionID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		if @bisRemovePermanent = 1
			begin
				delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
				delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
				delete [Option] where OptionID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
				if @bisRemoveFromContract = 1 
					begin
						delete Contract where contractID in (select OptionID from #Options_for_delete)
							if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
					end
			end

	Commit Transaction DelOpt

finish:
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

CREATE     Procedure dbo.usp_Contract_Del
	@iContractID int,			-- contract identifier
	@bisRemovePermanent bit = 0
as
/*
	Create by Zaur Nuraliev
	Description: It deletes Option data from Option and Contract tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if object_ID('tempdb..#Contracts_for_delete') is null
		Create table #Contracts_for_delete(contractID int)

	if @bisRemovePermanent = 0
		begin
			Raiserror('Couldn''t change actionID of Contracts!',16,1)
			Return(-1)
		end

	if @iContractID is not null
		begin
			if exists (select tradeID from Trade where contractID = @iContractID)
				begin
					Raiserror ('Error. Can''t delete Contract because of the existing Trade', 16, 1)
					Return (-1)
				end   
			insert into #Contracts_for_delete
				select @iContractID
		end

	Begin Transaction DelContract

		--ContractExchangeLink
		delete from ContractExchangeLink
		--update ContractExchangeLink set actionID = 3
			where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--ContractPrice
		delete from ContractPrice 
			where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--ModelParameter
		--delete from ModelParameter
		--	where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--ContractInGroup
		delete from ContractInGroup
			where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--ContractMarketData
		delete from ContractMarketData where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--TraderContract 
		delete from TraderContract where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--HistRangesCache
		delete from HistRangesCache where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--NextDaySurface
		delete from NextDaySurface where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
		--VolaSurfaceData
		delete from VolaSurfaceData where contractID in (select contractID from #Contracts_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end

		--remove options of above selected contracts
		if object_ID('tempdb..#Options_for_delete') is null
			create table #Options_for_delete(OptionID int)
		else
			delete from #Options_for_delete

		insert into #Options_for_delete 
			select optionID 
				from [Option] O
					join #Contracts_for_delete C on C.ContractID = O.underlyingContractID
				where O.optionID not in (select distinct contractID from Trade)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end

		exec usp_Option_Del null,1
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end

		delete from #Options_for_delete
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end

		insert into #Options_for_delete 
			select contractID from #Contracts_for_delete
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end

		exec usp_Option_Del null,1,0
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelContract goto finish end
	Commit Transaction DelContract


finish:
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






CREATE   PROC dbo.usp_IndexHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select i.indexID as iIndexID,
		i.yield as fYield,
		i.isHTB as tiIsHTB,
		i.CalcOptionType as tiCalcOptionType,
		i.CalcModelType as tiCalcModelType,
		i.isBasket as tiIsBasket,
		i.isActive as tiIsActive,
		i.actionDate as dtActionDate,
		i.actionID as tiActionID,
		i.skew as fSkew,
		i.kurt as fKurt
	from [Index] i
	where (i.actionDate > @dtActionDate or @dtActionDate is null)
	order by i.actionDate

return(@@error)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO









CREATE   proc dbo.usp_IndexWithPos_Get
	@iIndexID int = null,
	@iTraderID int = null,
	@tiPositionMode tinyint = 0 -- 0 - filter by TraderContract, 1 - don't filter by TraderContract
as
	------------------------------------------------
	set nocount on
	------------------------------------------------

	if @iTraderID is null
		select C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName,
			I.yield as fYield,
			I.isHTB as bIsHTB,
			I.actionDate as dtActionDate,
			I.actionID as tiActionID,
			sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
			CP.lotSize as iLotSize,
			I.CalcOptionType as tiCalcOptionType,
			I.CalcModelType as tiCalcModelType,
			C.expCalendarID as iExpCalendarID,
			I.isBasket as tiIsBasket,
			I.isActive as tiIsActive,
			i.skew as fSkew,
			i.kurt as fKurt
		from ContractView c 	
			inner join IndexView I on I.indexID = C.contractID
			join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
			left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
		where C.contractTypeID = 1
			and (C.contractID = @iIndexID or @iIndexID is Null)
		group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
			I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID, 
			I.isBasket, I.isActive, i.skew, i.kurt
		order by C.symbol
	else
	begin
		if isnull(@tiPositionMode, 0) = 0
			select C.contractID as iContractID,
				C.contractTypeID as iContractTypeID,
				C.symbol as vcSymbol,
				C.contractName as vcContractName,
				I.yield as fYield,
				I.isHTB as bIsHTB,
				I.actionDate as dtActionDate,
				I.actionID as tiActionID,
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				CP.lotSize as iLotSize,
				I.CalcOptionType as tiCalcOptionType,
				I.CalcModelType as tiCalcModelType,
				C.expCalendarID as iExpCalendarID,
				I.isBasket as tiIsBasket,
				I.isActive as tiIsActive,
				i.skew as fSkew,
				i.kurt as fKurt
			from ContractView c 	
				inner join IndexView I on I.indexID = C.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where C.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (C.contractID = @iIndexID or @iIndexID is Null)
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
				I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID, 
				I.isBasket, I.isActive, i.skew, i.kurt
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
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				CP.lotSize as iLotSize,
				I.CalcOptionType as tiCalcOptionType,
				I.CalcModelType as tiCalcModelType,
				C.expCalendarID as iExpCalendarID,
				I.isBasket as tiIsBasket,
				I.isActive as tiIsActive,
				i.skew as fSkew,
				i.kurt as fKurt
			from ContractView c 	
				inner join IndexView I on I.indexID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where C.contractTypeID = 1
				and (C.contractID = @iIndexID or @iIndexID is Null)
			group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
				I.yield, I.isHTB, I.actionDate, I.actionID,CP.lotSize,I.CalcOptionType,I.CalcModelType,C.expCalendarID, 
				I.isBasket, I.isActive, i.skew, i.kurt
			order by C.symbol
	end

Return (@@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE Procedure dbo.usp_Index_Del
		@iIndexID int
  as
/*
	Created by Banderlog 
	Description: this procedure is mark as delete information in Index table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	 if exists (select tradeID from Trade
					 where Trade.contractID = @iIndexID)
	 begin
	   RAISERROR ('Error. Can''t delete [Index] because of the existing Trade', 16, 1)
	   RETURN (-1)
	 end

	Begin Transaction

		--delete ModelParameter where contractID = @iIndexID
		--	if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update ContractInGroup set actionID=3 where contractID = @iIndexID 
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update [Index] set actionID=3 where indexID = @iIndexID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update [Option] set actionID=3 where underlyingContractID = @iIndexID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update Contract set actionID=3 
		where contractID in (select OptionID from [Option] where underlyingContractID = @iIndexID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update Contract set actionID=3 
		where contractID in (select underlyingContractID from [Option] where optionID= @iIndexID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		update Contract set actionID=3 where contractID = @iIndexID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		
		set @error = @@error
	Commit Transaction
finish:
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

CREATE   Procedure dbo.usp_OptionPermanent_Del
	@iOptionID int = null 	-- contract identifier
as
/*
	Create by Zaur Nuraliev
	Description: It deletes Option data from Option and Contract tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if object_ID('tempdb..#Options_for_delete') is null
		begin	
			Create table #Options_for_delete(OptionID int)
		end
	
	if @iOptionID is not null
		begin
			if exists (select tradeID from Trade where contractID = @iOptionID)
				begin
					Raiserror ('Error. Can''t delete [Option] because of the existing Trade', 16, 1)
					Return (-1)
				end   
			insert into #Options_for_delete
				select @iOptionID
		end
	Begin Transaction DelOpt
		delete from TradeLog where contractID in (select OptionID from #Options_for_delete)

		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete from ModelParameter where contractID in (select OptionID from #Options_for_delete)
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID in (select OptionID from #Options_for_delete)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete [Option] where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete Contract where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete Trade where contractID = @iOptionID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	Commit Transaction DelOpt

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

CREATE           Procedure dbo.usp_Option_Import
	@iUnderlyingContractID int = null,  --underlying contractID
	@vcSymbol varchar(20),  -- option symbol
	@fPriceAsk float, 
	@fPriceBid float, 
	@iVolume int,
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
	@iOpenInterest int,
	@iExchangeID int = Null, -- the exchange identifier (special for ContractPrice update)
	@vcUnderlyingSymbol varchar(8) = null,  -- stock symbol
	@iLotSize int = null
as
/*
	Created by ... Redesigned by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare	@OptionID int, 
			@cn varchar(255),
			@iExpiryID int,
			@iLastExpiryID int

	--===================================================================			
	-- check the Expiry
	--===================================================================			
	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	if not exists(select * from Expiry where expiryID = @iExpiryID)
		begin
			Declare @ERR_MSG varchar(8000)
			set @ERR_MSG = 'Incorrect expiration! '  + '"' +  Convert(varchar(50),@dtExpiry) + '"' + ' for symbol=' + '"' + @vcSymbol + '"' +  ' UnderlyingSymbol=' + '"' + @vcUnderlyingSymbol + '"' + ' Strike=' + Convert(varchar(15),@fStrike) + ' expiryID=' + Convert(varchar(15),@iExpiryID)
			Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end

	select top 1 @iLastExpiryID = expiryID from ExpiryInCalendarView where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	--===================================================================			
	-- check the consistensy of input parameters (for stock)
	--===================================================================			
	declare @iUnderlyingFirstContractID int
		set @iUnderlyingFirstContractID = @iUnderlyingContractID
	--select underlying ID
	select @iUnderlyingContractID = contractID  
		from Contract 
		where symbol = @vcUnderlyingSymbol and contractTypeID < 3
	if @iUnderlyingFirstContractID <> @iUnderlyingContractID
		begin
			Raiserror ('Wrong underlying contract ID in usp_Option_Import!', 16, 1)
			Return (-1)
		end  
	--select underlying option symbol
	if @vcUnderlyingSymbol is null
		select @vcUnderlyingSymbol=Contract.Symbol 
			from Contract (nolock) 	
			where Contract.contractID = @iUnderlyingContractID and Contract.contractTypeID < 3

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if (@OptionID is Null) 
		select @OptionID = optionID
			from [Option] (nolock)
			where underlyingContractID = @iUnderlyingContractID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	--===================================================================			
	-- try to find Option with same ticker in DB
	--===================================================================			
	declare @iIncorrectOptionID int

	select @iIncorrectOptionID = contractID 
		from Contract (nolock)
				join [Option] (nolock) on Contract.contractID = [Option].optionID
		where symbol = @vcSymbol and contractTypeID = 3 and 
			[Option].actionID < 3 and (@OptionID <> contractID) -- or @OptionID is null

	if @iIncorrectOptionID is not null
		begin
			-- mark as delete
			update [Option] set [Option].actionID = 3 
				where optionID = @iIncorrectOptionID and expiryID < @iLastExpiryID

			if @@rowcount = 0 
				begin
					--Raiserror ('Option with this symbol already exists!', 16, 1)
					--Return (-1)
					update [Option] set [Option].actionID = 3 
						where optionID = @iIncorrectOptionID
					update Contract set actionID = 3 
						where ContractID = @iIncorrectOptionID
				end   
			else
				update Contract set actionID = 3 
					where ContractID = @iIncorrectOptionID

			delete from TradeLog
			where contractID = @iIncorrectOptionID
		end

	--===================================================================			
	-- make the contract name
	--===================================================================			
--  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(day, @dtExpiry) + ' ' + DATENAME(month, @dtExpiry) + 
--					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
--						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(month, @dtExpiry) + 
					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)

  if exists (select contractID from Contract(nolock) where contractID = @iUnderlyingContractID and contractTypeID > 2)
	 begin
	   RAISERROR ('Error. Can''t insert values to [Option]. The Underlying Contract is not stock or index!', 16, 1)
	   RETURN (-1)
	 end 

  if (@OptionID is Null) 
	begin
		Begin Transaction
	
			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@cn)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
				 values (@OptionID, @iUnderlyingContractID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			insert into ContractPrice (contractID,exchangeID, priceBid, priceAsk, volume,openInterest,lotSize)
				 values (@OptionID,@iExchangeID, @fPriceBid, @fPriceAsk, @iVolume,@iOpenInterest,@iLotSize)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID,lotSize)
						values(@OptionID,null,@iLotSize)
					if (@@error <> 0) begin rollback tran return(-1) end           
				end 
		Commit Transaction
	end 
   else
	begin
		  update Contract
			   set symbol = @vcSymbol,
					 contractName = @cn
			where contractID = @OptionID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract!', 16, 1)
				  RETURN (-1)
			   end 

		delete from TradeLog
		where contractID = @OptionID

		  update ContractPrice
				set	priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					volume = @iVolume,
					openInterest=@iOpenInterest,
					lotSize = isnull(@iLotSize,lotSize)
				where contractID = @OptionID and isnull(exchangeID,0)=isnull(@iExchangeID,0)
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract price!', 16, 1)
				  RETURN (-1)
			   end 

			update [Option]
				set 
				IsCall = isnull(@iIsCall,IsCall),
				strike = isnull(@fStrike,Strike),
				expiryID = isnull(@iExpiryID,expiryID)
			where optionID = @OptionID

		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t update Option information (table Option)!', 16, 1)
				  RETURN (-1)
			   end 

	end 
	 
-- return @OptionID 
    set @error = 0
finish:
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









CREATE         Procedure dbo.usp_ReportStock_Get
	@iStockID int = Null, 
	@iFilterID int = Null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from [Option] table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	--=====================================================
	-- work with filter
	--=====================================================
	create table #FilteredStocks(stockID int)
              insert into #FilteredStocks 
                      exec usp_FilterData_Get @iFilterID
	--=====================================================
	-- data selection
	--=====================================================
		select
			Contract.ContractID as iContractID,
			Contract.symbol as vcSymbol,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceLast as fPriceLast,
			Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
			case when isDivCustom = 1 then divFreqCustom else Stock.divfreq end iDivFreq,
			case when isDivCustom = 1 then divAmtCustom else Stock.divAmt end fDivAmt,
			case when isDivCustom = 1 then divDateCustom else Stock.DivDate end dtDivDate,
			S2.currentHV as fCurrentHV,
			CP.priceClose as fPriceClose,
	  		Stock.isHTB as bIsHTB,
			Stock.isDivCustom as tiIsDivCustom,
			Stock.CalcOptionType as tiCalcOptionType,
			Stock.CalcModelType as tiCalcModelType,
			Contract.expCalendarID as tiExpCalendarID,
			null as tiIsBasket,
			Stock.Skew as fSkew,
			Stock.Kurt as fKurt
			from ContractView Contract
					join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
					join StockView Stock on Contract.contractID = Stock.StockID
					left join (select StockDataHistory.stockID,currentHV,[date] from StockDataHistory join
									(select stockID, max([date]) as maxDate from StockDataHistory group by stockID) S1
								on S1.stockID=StockDataHistory.stockID and S1.maxDate=StockDataHistory.[date]) S2 
						on S2.stockID=Stock.stockID
	
			where (
				(@iStockID is null and (Stock.stockID in (select stockID from #FilteredStocks)))
				or 
				(@iStockID is not null and Contract.ContractID=@iStockID 
					and (Stock.stockID in (select stockID from #FilteredStocks) or @iFilterID is null))
				)
				and Contract.contractTypeID = 2
	union
		select
			Contract.ContractID as iContractID,
			Contract.symbol as vcSymbol,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceLast as fPriceLast,
			Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
			0 as iDivFreq,
			I.yield as fDivAmt,
	  		null as dtDivDate,
			S2.currentHV as fCurrentHV,
			CP.priceClose as fPriceClose,
	  		I.isHTB as bIsHTB,
			null as tiIsDivCustom,
			I.CalcOptionType as tiCalcOptionType,
			I.CalcModelType as tiCalcModelType,
			Contract.expCalendarID as tiExpCalendarID,
			I.IsBasket as tiIsBasket,
			I.Skew as fSkew,
			I.Kurt as fKurt
			from ContractView Contract
					join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
					join IndexView I on Contract.contractID = I.indexID
					left join (select IndexDataHistory.indexID,currentHV,[date] from IndexDataHistory join
									(select indexID, max([date]) as maxDate from IndexDataHistory group by indexID) S1
								on S1.indexID=IndexDataHistory.indexID and S1.maxDate=IndexDataHistory.[date]) S2 
						on S2.indexID=I.indexID
			where (
				(@iStockID is null and (I.indexID in (select stockID from #FilteredStocks))) or 
				(@iStockID is not null and @iFilterID is not null and (Contract.ContractID=@iStockID and (I.indexID in (select stockID from #FilteredStocks)))) or 
				(@iStockID is not null and I.indexID = @iStockID and @iFilterID is null)
				)
				and Contract.contractTypeID = 1
		order by Contract.symbol


	set @error = @@error
             drop table #FilteredStocks
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






CREATE   PROC dbo.usp_StockHH_Get
	@dtActionDate datetime = null
AS
/*
	Edited by Sharky
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	select s.stockID as iStockID, 
		s.primaryExchangeID as iPrimaryExchangeID, 
		s.isHTB as tiIsHTB,
		s.DivAmt as fDivAmt, 
		s.DivFreq as iDivFreq,
		s.DivDate as dtDivDate,
		s.isDivCustom as tiIsDivCustom,
		s.divAmtCustom as fDivAmtCustom,
		s.divFreqCustom as iDivFreqCustom,
		s.divDateCustom as dtDivDateCustom,
		s.CalcOptionType as tiCalcOptionType,
		s.CalcModelType as tiCalcModelType,
		s.isActive as tiIsActive,
		s.actionDate as dtActionDate,
		s.actionID as tiActionID,
		s.skew as fSkew,
		s.kurt as fKurt
	from Stock s
	where (s.actionDate > @dtActionDate or @dtActionDate is null)
	order by s.actionDate

return(@@error)





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO










CREATE   proc dbo.usp_StockWithPos_Get
	@iStockID int = null,
	@iTraderID int = null,
	@tiPositionMode tinyint = 0 -- 0 - filter by TraderContract, 1 - don't filter by TraderContract
as
	------------------------------------------------
	set nocount on
	------------------------------------------------

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
			sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
			S.isDivCustom as tiDivCustom,
			S.divAmtCustom as fDivAmtCustom,
			S.divDateCustom as dtDivDateCustom,
			S.divFreqCustom as iDivFreqCustom,
			CP.lotSize as iLotSize,
			S.CalcOptionType as tiCalcOptionType,
			S.CalcModelType as tiCalcModelType,
			C.expCalendarID as iExpCalendarID,
			S.isActive as tiIsActive,
			s.skew as fSkew,
			s.kurt as fKurt
	    from ContractView c  
			inner join StockView S on S.stockID = C.contractID
			join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
			left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
	    where c.contractTypeID = 2
			and (c.contractID = @iStockID or @iStockID is Null)
		group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
			S.DivFreq,S.primaryExchangeID, 
			S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
			S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
			S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive, s.skew, s.kurt
		order by c.Symbol
	else
	begin 
		if isnull(@tiPositionMode, 0) = 0
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
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				S.isDivCustom as tiDivCustom,
				S.divAmtCustom as fDivAmtCustom,
				S.divDateCustom as dtDivDateCustom,
				S.divFreqCustom as iDivFreqCustom,
				CP.lotSize as iLotSize,
				S.CalcOptionType as tiCalcOptionType,
				S.CalcModelType as tiCalcModelType,
				C.expCalendarID as iExpCalendarID,
				S.isActive as tiIsActive,
				s.skew as fSkew,
				s.kurt as fKurt
		    from ContractView c  
				inner join StockView S on S.stockID = C.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
		    where c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (c.contractID = @iStockID or @iStockID is Null)
			group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
				S.DivFreq, S.primaryExchangeID, 
				S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
				S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
				S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive, s.skew, s.kurt
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
				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				S.isDivCustom as tiDivCustom,
				S.divAmtCustom as fDivAmtCustom,
				S.divDateCustom as dtDivDateCustom,
				S.divFreqCustom as iDivFreqCustom,
				CP.lotSize as iLotSize,
				S.CalcOptionType as tiCalcOptionType,
				S.CalcModelType as tiCalcModelType,
				C.expCalendarID as iExpCalendarID,
				S.isActive as tiIsActive,
				s.skew as fSkew,
				s.kurt as fKurt
		    from ContractView c  
				inner join StockView S on S.stockID = C.contractID
				join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
		    where c.contractTypeID = 2
				and (c.contractID = @iStockID or @iStockID is Null)
			group by C.contractID, C.contractTypeID, C.Symbol, C.ContractName,
				S.DivFreq, S.primaryExchangeID, 
				S.DivDate, S.DivAmt, S.isHTB, S.actionDate,	S.actionID,
				S.isDivCustom,S.divAmtCustom,S.divDateCustom,S.divFreqCustom,CP.lotSize,
				S.CalcOptionType,S.CalcModelType,C.expCalendarID, S.isActive, s.skew, s.kurt
			order by c.Symbol
	end
	
Return(@@error)










GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
CREATE         proc dbo.usp_Stock_Del
        @iStockID int,
        @bisRemovePermanent bit = 0
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Stock table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	if exists (select * from TradeView(nolock) where contractID = @iStockID)
	begin
		Raiserror ('Error. Can''t delete Stock because of the existing Trade', 16, 1)
		Return (-1)
	end 
	if exists(select * from [Option](nolock) where underlyingContractID=@iStockID)
		begin
			Create table #Options_for_delete(OptionID int)
		
			insert into #Options_for_delete
				select optionID from [Option](nolock) where underlyingContractID=@iStockID
		
			if exists (select tradeID from TradeView where contractID in (select optionID from #Options_for_delete))
				begin
					Raiserror ('Error. Can''t delete Stock because of the existing Trade by Stock Options', 16, 1)
					Return (-1)
				end 
		end
	Begin Transaction
		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del @bisRemovePermanent = @bisRemovePermanent
			end

		exec usp_TradeLog_Del null, @iStockID

		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID = @iStockID
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update ContractExchangeLink set actionID = 3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	    --delete ModelParameter where contractID = @iStockID
		--	if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
        update ContractInGroup set actionID=3 where contractID = @iStockID 
        --delete ContractInGroup where contractID = @iStockID 
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockDataHistory where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockCorrelation where stockID1 = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockCorrelation where stockID2 = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

        update IndexDefinition set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		update TraderContract set actionID=3 where contractID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Stock set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

        --delete ContractPriceHistory where contractID = @iStockID
		--	if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete ContractPrice where contractID = @iStockID

			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Contract set actionID=3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		if (@bisRemovePermanent = 1) 
			begin
				delete from ContractExchangeLink where contractID = @iStockID
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	        	delete IndexDefinition where stockID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				delete TraderContract where contractID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		       	delete Stock where stockID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	        	delete Contract where contractID = @iStockID
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end 
  
		set @error = @@error
	Commit Transaction
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








CREATE  proc dbo.usp_Stock_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@fPriceAsk float,
	@fPriceBid float,
	@fPriceOpen float,
	@fPriceClose float,
	@fPriceLow float,
	@fPriceHigh float,
	@iVolume int,
	@iExchangeID int = null,  --the ExchangeID special for save data into ContractPrice table
	@iLotSize int = null
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
		where contractID = @iContractID

		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update Contract Name!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end
		update ContractPrice 
			set priceAsk = @fPriceAsk,
			priceBid = @fPriceBid,
			PriceOpen = @fPriceOpen,
			PriceClose = @fPriceClose,
			PriceLow = @fPriceLow,
			PriceHigh = @fPriceHigh,
			Volume = @iVolume,
			lotSize = isnull(@iLotSize,lotSize)
		where contractID = @iContractID and (exchangeID=@iExchangeID or @iExchangeID is null)
		if (@@error <> 0)
			begin
				Raiserror ('Error. Can''t update Contract Price!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end
		--========================================================================
		-- make record with null exchange for this stock
		--========================================================================
		if not exists(select * from ContractPrice where exchangeID is null and contractID = @iContractID)
			begin
				insert into ContractPrice(contractID,exchangeID,lotSize)
					values(@iContractID,null,@iLotSize)
				if (@@error <> 0) begin rollback tran return(-1) end           
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
						divFreq = @iDivFreq
				where stockID = @iContractID
				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update DivAmnt and DivDate!', 16, 1)
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

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE PROCEDURE dbo.usp_TradeByContractAndTrader_Del 
	@vcAcronym varchar(12) = null,
    @vcUnderlyingSymbol varchar(20),
    @dtExpiry datetime,
    @fStrike float,
    @tiIsCall tinyint
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Tub table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	declare @iTraderID int,
			@iOptionID int,
			@iExpiryID int

	if @vcAcronym is not null
		select @iTraderID = traderID from Trader where acronym = @vcAcronym

	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	set @fStrike = round(@fStrike, 3)

	select @iOptionID = optionID
		from OptionView (nolock)
				join Contract C (nolock) on C.contractID = OptionView.underlyingContractID
		where C.symbol = @vcUnderlyingSymbol and 
			isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID

	declare @iTradeID int

	declare Trades_cursor cursor
	read_only
		for Select distinct tradeID 
				from TradeView
				where contractID = @iOptionID and (traderID = @iTraderID or @iTraderID is null)

	Begin Transaction

		open Trades_cursor
			fetch next from Trades_cursor into @iTradeID
				while (@@fetch_status <> -1)
					begin
						if (@@fetch_status <> -2)
					begin
						exec usp_Trade_Del @iTradeID
						--select @iTradeID
						if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
					end
					fetch next from Trades_cursor into @iTradeID
				end

		Close Trades_cursor
		Deallocate Trades_cursor

		set @error = @@error
	Commit Transaction

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



CREATE PROCEDURE dbo.usp_TradeByExpiredOptions_Del 
	@dtExpiry datetime 

AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Tub table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	declare @iTradeID int

	declare TradeByExpiredOptions_cursor cursor
	read_only
		for 
			Select distinct tradeID 
				from TradeView
				where contractID in (
						Select optionID 
						from OptionView O
							join Contract C on O.underlyingContractID = C.contractID
							join ExpiryInCalendar EIC on O.expiryID = EIC.expiryID and EIC.expCalendarID = C.expCalendarID
						where EIC.expiryDate < @dtExpiry)

	Begin Transaction

		open TradeByExpiredOptions_cursor
			fetch next from TradeByExpiredOptions_cursor into @iTradeID
				while (@@fetch_status <> -1)
					begin
						if (@@fetch_status <> -2)
					begin
						exec usp_Trade_Del @iTradeID
						--select @iTradeID
						if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
					end
					fetch next from TradeByExpiredOptions_cursor into @iTradeID
				end

		Close TradeByExpiredOptions_cursor
		Deallocate TradeByExpiredOptions_cursor

		set @error = @@error
	Commit Transaction

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






CREATE    PROCEDURE dbo.usp_TradeFeed_New
	@vcTraderAcronym varchar(12),
	@vcExecID varchar(20),
	@tiIsBuy tinyint,  
	@iQuantity int,
	@fPrice float,
	@dtTradeDate datetime,
	@vcContractSymbol varchar(8),
	@iContractTypeID int,
	@vcUnderlyingSymbol varchar(8) = null,
	@tiIsCall tinyint = null,
	@fStrike float = null,
	@dtExpiry datetime = null,
	@iTradeLotSize int = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is save information into table_name table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if exists(select * from Trade where execID = @vcExecID and actionID > 0 and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tradeDate,112))
		begin
			--Raiserror('Record with same execID alreadty exists in DB!',16,1)
			Select null as iSeqNum
			Return(-1)

		end

	Begin Transaction

	Declare @iTradeContractID int, @iStructureMissed int
	set @iStructureMissed = 0

	--check underlying symbol
	Declare @iUnderlyingContractID int

		if @vcUnderlyingSymbol is null and  @iContractTypeID <3 
			set @vcUnderlyingSymbol = @vcContractSymbol

	select @iUnderlyingContractID = contractID from ContractView where symbol = @vcUnderlyingSymbol and contractTypeID in(1,2)


	if @iUnderlyingContractID is null
	--add new stock\index
		begin
			insert into Contract(symbol,contractTypeID,expCalendarID) values(@vcUnderlyingSymbol, 2, 1)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iUnderlyingContractID = @@identity

			insert into Stock (stockID,isActive) values(@iUnderlyingContractID,1)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			if @iTradeLotSize is null set @iTradeLotSize = 1
			insert into ContractPrice (contractID,exchangeID,lotSize) values(@iUnderlyingContractID,null,@iTradeLotSize)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			set @iStructureMissed = 1

			insert into TradeLog (contractID, eventTypeID) values (@iUnderlyingContractID, 0)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end


/*			insert into Contract(symbol,contractTypeID,expCalendarID) values(@vcUnderlyingSymbol,@iContractTypeID,1)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iUnderlyingContractID = @@identity
			if @iContractTypeID = 1
				begin
					insert into [Index](indexID) values(@iUnderlyingContractID)
						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else if @iContractTypeID = 2
				begin
					insert into Stock (stockID) values(@iUnderlyingContractID)
						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			if @iContractTypeID < 3 
				begin	
					if @iTradeLotSize is null set @iTradeLotSize = 1
					insert into ContractPrice (contractID,exchangeID,lotSize) values(@iUnderlyingContractID,null,@iTradeLotSize)
						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else
				insert into ContractPrice (contractID,exchangeID) values(@iUnderlyingContractID,null)
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end*/
		end
	else
		begin
			update Stock set isActive = 1 where stockID = @iUnderlyingContractID
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end

	--check Option
	if @iContractTypeID = 3
		begin
			Declare @iOptionID int,
					@iExpiryID int
	
			set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
			set @fStrike = round(@fStrike, 3)
	
			select @iOptionID = optionID
				from OptionView (nolock)
				where underlyingContractID = @iUnderlyingContractID and 
					isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3
	
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
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
							set @iOptionID = @@identity
				
						insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
							 values (@iOptionID, @iUnderlyingContractID, @tiIsCall, @fStrike, @iExpiryID)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				
						-- make record with null exchange for this Option
						insert into ContractPrice(contractID,exchangeID,lotSize) values(@iOptionID,null,@iTradeLotSize)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

						insert into TradeLog (contractID, eventTypeID) values (@iOptionID, 0)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
					end
					else
					begin
						Select null as iSeqNum
						Return(-2)
					end

					set @iStructureMissed = @iStructureMissed + 2
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
			insert into Trader(acronym,traderName) values(@vcTraderAcronym,@vcTraderAcronym)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iTraderID = @@identity
		end

	if not exists(select 1 from TraderContractView where traderID = @iTraderID and contractID = @iUnderlyingContractID)
		begin
			insert into TraderContract (traderID, contractID) values (@iTraderID, @iUnderlyingContractID)
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end

	--create new trade
	if @iTradeLotSize is null
		select @iTradeLotSize = lotSize from ContractPrice 
			where exchangeID is null and 
					(contractID = @iOptionID and @iContractTypeID = 3) or
					(contractID = @iUnderlyingContractID and @iContractTypeID < 3)

	if @iTradeLotSize is null
		if @iContractTypeID = 3 
			set @iTradeLotSize = 100
		else if @iContractTypeID < 3 
			set @iTradeLotSize = 1

	Declare @iNewTradeID int
		select @iNewTradeID = max(tradeID) + 1 from Trade

	if @iNewTradeID is null 
			set @iNewTradeID = 1
	
	insert into Trade (
		execID,
		tradeID,
		actionID, 
		contractID,
		isClose, 
		tradeDate, 
		quantity, 
		price, 
		traderID, 
		isBuy, 
		isPosition,
		lotSize,
		isSimulated,
		brokerCommissionID,
		clearingBrokerCommissionID)
	values (
		@vcExecID,
		@iNewTradeID,
		1,
		@iTradeContractID, 
		0, 
		@dtTradeDate, 
		@iQuantity, 
		@fPrice, 
		@iTraderID, 
		@tiIsBuy, 
		0,
		@iTradeLotSize,
		0,
		null,
		null)

	Declare @iSeqNum int 
		set @iSeqNum = @@identity

		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

	Commit Transaction

 --if all OK
	if @iContractTypeID < 3 
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
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iUnderlyingLotSize,
			cp.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated,
			null as fPriceClose,
			null as fUnderlyingPriceClose,
			@iStructureMissed as iStructureMissed
		from Trade tr 
			join Contract C  on tr.contractID = c.contractID
			join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where tr.seqNum = @iSeqNum

	else if @iContractTypeID =3
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
			tr.lotSize as iTradeLotSize,
			ucp.lotSize as iUnderlyingLotSize,
			cp.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated,
			CP.PriceClose as fPriceClose,
			UCP.priceClose as fUnderlyingPriceClose,
			@iStructureMissed as iStructureMissed
		from Trade tr 
			join Contract C  on tr.contractID = c.contractID
			inner join [Option] O on C.contractID = O.optionID
			inner join Contract UC on O.underlyingContractID = UC.contractID
			inner join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
			inner join ContractPrice UCP on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
			join ExpiryInCalendarView EC on UC.expCalendarID = EC.expCalendarID and O.expiryID = EC.expiryID
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where tr.seqNum = @iSeqNum

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


CREATE  PROCEDURE dbo.usp_TradeLog_Get
	@iTraderID int = null,
	@tiWithOptions tinyint = 0
AS
/*
	Created by Sharky
	Description: this procedure is get information into TradeLog table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if isnull(@iTraderID, 0) = 0
	begin
		if isnull(@tiWithOptions, 0) = 0
		begin
			select
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.symbol as vcSymbol
			from TradeLog tl
				inner join ContractView c on tl.contractID = c.contractID
			where
				c.contractTypeID in (1, 2)
			union
			select
				uc.contractID as iContractID,
				uc.contractTypeID as iContractTypeID,
				uc.symbol as vcSymbol
			from TradeLog tl
				inner join ContractView c on tl.contractID = c.contractID
				inner join OptionView o on c.contractID = o.optionID
				inner join ContractView uc on o.underlyingContractID = uc.contractID
			where
				c.contractTypeID = 3
		end
		else
		begin
			select
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.symbol as vcSymbol,

				c.contractID as iUnderlyingContractID,
				c.contractTypeID as iUnderlyingContractTypeID,
				c.symbol as vcUnderlyingSymbol
			from TradeLog tl
				inner join ContractView c on tl.contractID = c.contractID
			where
				c.contractTypeID in (1, 2)
			union
			select
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.symbol as vcSymbol,
		
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.symbol as vcUnderlyingSymbol
		
			from TradeLog tl
				inner join ContractView c on tl.contractID = c.contractID
				inner join OptionView o on c.contractID = o.optionID
				inner join ContractView uc on o.underlyingContractID = uc.contractID
			where
				c.contractTypeID = 3
		end
	end
	else
	begin
		if isnull(@tiWithOptions, 0) = 0
		begin
			select
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.symbol as vcSymbol
			from TradeLog tl
				inner join ContractView c on tl.contractID = c.contractID
				inner join TraderContractView tc on c.contractID = tc.contractID
			where
				c.contractTypeID in (1, 2)
				and tc.traderID = @iTraderID
			union
			select
				uc.contractID as iContractID,
				uc.contractTypeID as iContractTypeID,
				uc.symbol as vcSymbol
			from TradeLog tl
				inner join ContractView c on tl.contractID = c.contractID
				inner join OptionView o on c.contractID = o.optionID
				inner join ContractView uc on o.underlyingContractID = uc.contractID
				inner join TraderContractView tc on uc.contractID = tc.contractID
			where
				c.contractTypeID = 3
				and tc.traderID = @iTraderID
		end
		else
		begin
			select
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.symbol as vcSymbol,

				c.contractID as iUnderlyingContractID,
				c.contractTypeID as iUnderlyingContractTypeID,
				c.symbol as vcUnderlyingSymbol
			from TradeLog tl
				inner join ContractView c on tl.contractID = c.contractID
				inner join TraderContractView tc on c.contractID = tc.contractID
			where
				c.contractTypeID in (1, 2)
				and tc.traderID = @iTraderID
			union
			select
				c.contractID as iContractID,
				c.contractTypeID as iContractTypeID,
				c.symbol as vcSymbol,
		
				uc.contractID as iUnderlyingContractID,
				uc.contractTypeID as iUnderlyingContractTypeID,
				uc.symbol as vcUnderlyingSymbol
		
			from TradeLog tl
				inner join ContractView c on tl.contractID = c.contractID
				inner join OptionView o on c.contractID = o.optionID
				inner join ContractView uc on o.underlyingContractID = uc.contractID
				inner join TraderContractView tc on uc.contractID = tc.contractID
			where
				c.contractTypeID = 3
				and tc.traderID = @iTraderID
		end
	end
	
return(@@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE PROCEDURE dbo.usp_TradeLog_Save 
	@iContractID int, 
	@tiEventTypeID int = 0
AS
/*
	Created by Sharky
	Description: this procedure is save information into TradeLog table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction

	insert into TradeLog (contractID, eventTypeID) 
		values (@iContractID, @tiEventTypeID)

	if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

	Commit Transaction
    set @error = 0
finish:
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










CREATE   proc dbo.usp_UnderlyingByTrader_Get
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
		    cp.lotSize as iLotSize,

			null as iTraderID,

			s.skew as fSkew,
			s.kurt as fKurt

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
		    cp.lotSize as iLotSize,

			null as iTraderID,

			i.skew as fSkew,
			i.kurt as fKurt

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
			    cp.lotSize as iLotSize,

				tc.traderID as iTraderID,

				s.skew as fSkew,
				s.kurt as fKurt

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
			    cp.lotSize as iLotSize,

				tc.traderID as iTraderID,

				i.skew as fSkew,
				i.kurt as fKurt

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
			    cp.lotSize as iLotSize,

				tc.traderID as iTraderID,

				s.skew as fSkew,
				s.kurt as fKurt

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
			    cp.lotSize as iLotSize,

				tc.traderID as iTraderID,

				i.skew as fSkew,
				i.kurt as fKurt

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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE  procedure dbo.usp_Underlying_Get
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
			    cp.lotSize as iLotSize,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
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
			    cp.lotSize as iLotSize,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
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
			    cp.lotSize as iLotSize,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
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

			    cp.lotSize as iLotSize,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
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
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractView c 
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
			where
				c.contractTypeID = 2
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
				cp.lotSize, s.isActive, s.skew, s.kurt
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
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,

				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractView c 
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.isSimulated, 0) = 0
			where
				c.contractTypeID = 1
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, cp.lotSize, i.isActive, i.skew, i.kurt
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
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,

				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where 
				c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
				cp.lotSize, s.isActive, s.skew, s.kurt
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
			    cp.lotSize as iLotSize,

				sum(tr.quantity * tr.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,

				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractView c 
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.isSimulated, 0) = 0
			where
				c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and (@iContractID is null or c.contractID = @iContractID)
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, cp.lotSize, i.isActive, i.skew, i.kurt
			order by c.symbol	 
	end

	Return (0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE          proc dbo.usp_allStockEOD_Get
	@iFormat int = 1	-- 1 - list only
						-- 2 - list with prices by exchange
  as
	---------------
	set nocount on
	---------------
--=================================================================
-- select stocks for which trades was
--=================================================================
	if object_ID('tempdb..#SelContracts') is null
		Create table #SelContracts(contractID int not null)

	insert into #SelContracts
		select contractID 
			from Contract
		where 
		contractID in (select distinct O.underlyingContractID from OptionView O join TradeView TV on TV.contractID=O.optionID)
	union 
		select distinct contractID 
			from contract 
		where contractID in (select contractID from TradeView)
	
--=================================================================
--base section 
--=================================================================
	if @iFormat = 1
		select	iContractID, 
				iContractTypeID, 
				vcSymbol,
				tiCalcOptionType,
				tiCalcModelType,
				tiExpCalendarID,
				tiIsBasket,
				fSkew,
				fKurt  
			from vALLStockEOD A
		where iContractID in (select contractID from #SelContracts)
	else
		-- use NULL exchange only
		select iContractID, 
				iContractTypeID, 
				vcSymbol, 
				null as vcExchangeCode,  
				case when isDivCustom = 1 then A.divFreqCustom else A.iDivFreq end iDivFreq,
				case when isDivCustom = 1 then A.divAmtCustom else A.fDivAmt end fDivAmt,
				case when isDivCustom = 1 then A.divDateCustom else A.dtDivDate end dtDivDate,
				CP.priceClose as fPriceClose,
				A.bIsHTB as bIsHTB,
				tiCalcOptionType,
				tiCalcModelType,
				tiExpCalendarID,
				tiIsBasket,
				fSkew,
				fKurt  
			from vALLStockEOD A, ContractPrice CP (nolock)
			where A.iContractID *= CP.contractID and CP.exchangeID is null
			and A.iContractID in (select contractID from #SelContracts)

if object_ID('tempdb..#SelContracts') is not null
	Drop table #SelContracts

if (@@error <> 0)
	Return (-1)
else  
   Return(0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Save]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE     proc dbo.usp_Index_Save
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
		@fKurt float = null
   as
	---------------
	set nocount on
	---------------

	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID=1 and actionID < 3 and (@iIndexID <> contractID or @iIndexID is null))
		begin
			Raiserror ('Index with this ticker already exists!', 16, 1)
			Return (-1)
		end   

   if (@iIndexID is Null)  
	 begin
		 
		 begin tran
			 insert into Contract (
							contractTypeID, 
							symbol, 
							contractName,
							expCalendarID)
				  values (
							1, 
							@vcSymbol, 
							@vcContractName,
							@iExpCalendarID)
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
					expCalendarID = isNull(@iExpCalendarID, expCalendarID)
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE      proc dbo.usp_Stock_Save
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
		@fKurt float = null
  as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @bIsHTB is null 
		set @bIsHTB=0

	if @iExpCalendarID is null or @iExpCalendarID = 0
		set @iExpCalendarID = 1
   
	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID=2 and actionID < 3 and (@iStockID <> contractID or @iStockID is null))
		begin
			Raiserror ('Stock with this ticker already exists!', 16, 1)
			Return (-1)
		end   
   if (@iStockID is Null)  
     begin
         begin tran
            insert into Contract (contractTypeID, Symbol, contractName, expCalendarID)
                  values (2, @vcSymbol, @vcContractName, @iExpCalendarID)
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
					expCalendarID = isNull(@iExpCalendarID,expCalendarID)
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.35.009')
	begin
		update DataInfo set keyValue = '3.35.009' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.35.009'
	end





