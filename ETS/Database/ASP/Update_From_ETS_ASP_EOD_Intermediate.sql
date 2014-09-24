/*
	Updating market structure from ETS ASP EOD intermediate market structure database
	(replace SHARKY.ETS_ASP_EOD with real ETS ASP EOD intermediate market structure database address)
*/


----------------------------------------------------------------------------------------------------
-- EodUnderlyingMap
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id('dbo.EodUnderlyingMap') and sysstat & 0xf = 3)
	drop table dbo.EodUnderlyingMap

create table dbo.EodUnderlyingMap
(
	eodContractID int null,
	contractID int null
)

create index IX_EodUnderlyingMap_eodContractID on dbo.EodUnderlyingMap(eodContractID)
create index IX_EodUnderlyingMap_contractID on dbo.EodUnderlyingMap(contractID)

----------------------------------------------------------------------------------------------------
-- EodFutureRootMap
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id('dbo.EodFutureRootMap') and sysstat & 0xf = 3)
	drop table dbo.EodFutureRootMap

create table dbo.EodFutureRootMap
(
	eodFutureRootID int null,
	futureRootID int null,
	eodFutureLotSize int null,
	eodOptionLotSize int null
)

create index IX_EodFutureRootMap_eodContractID on dbo.EodFutureRootMap(eodFutureRootID)
create index IX_EodFutureRootMap_contractID on dbo.EodFutureRootMap(futureRootID)

----------------------------------------------------------------------------------------------------
-- EodContractForDelete
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id('dbo.EodContractForDelete') and sysstat & 0xf = 3)
	drop table dbo.EodContractForDelete

create table dbo.EodContractForDelete (contractID int) 
create index IX_EodContractForDelete_contractID on dbo.EodContractForDelete (contractID)

----------------------------------------------------------------------------------------------------
-- EodContractForMark
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id('dbo.EodContractForMark') and sysstat & 0xf = 3)
	drop table dbo.EodContractForMark

create table dbo.EodContractForMark (contractID int) 
create index IX_EodContractForMark_contractID on dbo.EodContractForMark (contractID)

----------------------------------------------------------------------------------------------------
-- EodOptionRoot
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id('dbo.EodOptionRoot') and sysstat & 0xf = 3)
	drop table dbo.EodOptionRoot

create table dbo.EodOptionRoot
(
	eodOptionRootID int,
	underlyingID int,
	symbol varchar(20),
	lotSize int
)

----------------------------------------------------------------------------------------------------
-- EodOption
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id('dbo.EodOption') and sysstat & 0xf = 3)
	drop table dbo.EodOption

create table dbo.EodOption
(
	eodOptionRootID int,
	symbol varchar(20),
	contractName varchar(255) null,
	expiryID int null,
	strike float null,
	isCall tinyint null,
	priceClose float null
)

create index IX_EodOption_eodOptionRootID on dbo.EodOption (eodOptionRootID)

----------------------------------------------------------------------------------------------------
-- EodFuture
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id('dbo.EodFuture') and sysstat & 0xf = 3)
	drop table dbo.EodFuture

create table dbo.EodFuture
(
	eodFutureID int,
	futureRootID int null,
	symbol varchar(20),
	contractName varchar(255) null,
	maturityID int null,
	priceClose float null
)

create index IX_EodFuture_eodFutureID on dbo.EodFuture (eodFutureID)

----------------------------------------------------------------------------------------------------
-- EodFutureOption
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id('dbo.EodFutureOption') and sysstat & 0xf = 3)
	drop table dbo.EodFutureOption

create table dbo.EodFutureOption
(
	eodFutureID int,
	symbol varchar(20),
	contractName varchar(255) null,
	expiryID int null,
	strike float null,
	isCall tinyint null,
	priceClose float null
)

create index IX_EodFutureOption_eodFutureID on dbo.EodFutureOption (eodFutureID)

----------------------------------------------------------------------------------------------------
-- EodContractWithTrades
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id('dbo.EodContractWithTrades') and sysstat & 0xf = 3)
	drop table dbo.EodContractWithTrades

create table dbo.EodContractWithTrades (contractID int) 

----------------------------------------------------------------------------------------------------
-- AspEodMsUpdateLog
----------------------------------------------------------------------------------------------------
if not exists (select * from dbo.sysobjects where id = object_id('dbo.AspEodMsUpdateLog') and sysstat & 0xf = 3)
BEGIN
	CREATE TABLE dbo.AspEodMsUpdateLog
	(
		eventDate datetime NOT NULL CONSTRAINT DF_AspEodMsUpdateLog_eventDate DEFAULT (getdate()),
		event varchar (1000) NULL
	)
END
GO

----------------------------------------------------------------------------------------------------
-- IX_OptionRoot_underlyingID_symbol_actionID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_OptionRoot_underlyingID_symbol_actionID' and id = object_id('dbo.OptionRoot'))
	drop index dbo.OptionRoot.IX_OptionRoot_underlyingID_symbol_actionID
GO
CREATE NONCLUSTERED INDEX IX_OptionRoot_underlyingID_symbol_actionID ON dbo.OptionRoot (underlyingID, symbol, actionID)
GO

----------------------------------------------------------------------------------------------------
-- IX_OptionRoot_optionRootID_underlyingID_actionID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_OptionRoot_optionRootID_underlyingID_actionID' and id = object_id('dbo.OptionRoot'))
	drop index dbo.OptionRoot.IX_OptionRoot_optionRootID_underlyingID_actionID
GO
CREATE NONCLUSTERED INDEX IX_OptionRoot_optionRootID_underlyingID_actionID ON dbo.OptionRoot (optionRootID, underlyingID, actionID)
GO

----------------------------------------------------------------------------------------------------
-- IX_Contract_symbol_contractID_contractTypeID_actionID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_Contract_symbol_contractID_contractTypeID_actionID' and id = object_id('dbo.Contract'))
	drop index dbo.Contract.IX_Contract_symbol_contractID_contractTypeID_actionID
GO
CREATE NONCLUSTERED INDEX IX_Contract_symbol_contractID_contractTypeID_actionID ON dbo.Contract (symbol, contractID, contractTypeID, actionID)
GO

----------------------------------------------------------------------------------------------------
-- IX_Option_optionRootID_isCall_strike_expiryID_actionID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_Option_optionRootID_isCall_strike_expiryID_actionID' and id = object_id('dbo.[Option]'))
	drop index dbo.[Option].IX_Option_optionRootID_isCall_strike_expiryID_actionID
GO
CREATE NONCLUSTERED INDEX IX_Option_optionRootID_isCall_strike_expiryID_actionID ON dbo.[Option] (optionRootID, isCall, strike, expiryID, actionID)
GO

----------------------------------------------------------------------------------------------------
-- IX_Contract_symbol_contractTypeID_actionID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_Contract_symbol_contractTypeID_actionID' and id = object_id('dbo.Contract'))
	drop index dbo.Contract.IX_Contract_symbol_contractTypeID_actionID
GO
CREATE NONCLUSTERED INDEX IX_Contract_symbol_contractTypeID_actionID ON dbo.Contract (symbol, contractTypeID, actionID)
GO

----------------------------------------------------------------------------------------------------
-- Contract24 (drop)
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'Contract24' and id = object_id('dbo.Contract'))
	drop index dbo.Contract.Contract24
GO

----------------------------------------------------------------------------------------------------
-- IX_Contract_contractID_expCalendarID_actionID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_Contract_contractID_expCalendarID_actionID' and id = object_id('dbo.Contract'))
	drop index dbo.Contract.IX_Contract_contractID_expCalendarID_actionID
GO

CREATE NONCLUSTERED INDEX IX_Contract_contractID_expCalendarID_actionID ON dbo.Contract (contractID, expCalendarID, actionID)
GO


----------------------------------------------------------------------------------------------------
-- IX_Future_futureRootID_maturityID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_Future_futureRootID_maturityID' and id = object_id('dbo.Future'))
	drop index dbo.Future.IX_Future_futureRootID_maturityID
GO
CREATE NONCLUSTERED INDEX IX_Future_futureRootID_maturityID ON dbo.Future (futureRootID, maturityID)
GO

----------------------------------------------------------------------------------------------------
-- IX_FutureOption_futureID_isCall_strike_expiryID
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysindexes where name = 'IX_FutureOption_futureID_isCall_strike_expiryID' and id = object_id('dbo.FutureOption'))
	drop index dbo.FutureOption.IX_FutureOption_futureID_isCall_strike_expiryID
GO
CREATE NONCLUSTERED INDEX IX_FutureOption_futureID_isCall_strike_expiryID ON dbo.FutureOption (futureID, isCall, strike, expiryID)
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodMsUpdateLog_Add
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodMsUpdateLog_Add]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodMsUpdateLog_Add]
GO

CREATE PROC dbo.usp_AspEodMsUpdateLog_Add 
	@vcEvent varchar(1000),
	@iErrorCode int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	
	insert AspEodMsUpdateLog (event)
	values (@vcEvent + ' (' + cast(@iErrorCode as varchar(20)) + ')')

	return @@error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_DeleteOptions
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_DeleteOptions]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_DeleteOptions]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_DeleteOptions
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	delete OptionDataCache 
	from OptionDataCache d
		inner join EodContractForDelete t on t.contractID = d.optionID
	if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	delete ContractPrice
	from ContractPrice d
		inner join EodContractForDelete t on t.contractID = d.contractID
	if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	delete from ContractMarketData
	from ContractMarketData d
		inner join EodContractForDelete t on t.contractID = d.contractID
	if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	delete DualQuadraticParams
	from DualQuadraticParams d
		inner join EodContractForDelete t on t.contractID = d.contractID
	if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	delete [Option]
	from [Option] d
		inner join EodContractForDelete t on t.contractID = d.optionID
	if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	delete Contract
	from Contract d
		inner join EodContractForDelete t on t.contractID = d.contractID
	if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_MarkContracts
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_MarkContracts]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_MarkContracts]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_MarkContracts
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	update Contract
	set symbol = symbol + '*'
	where contractID in (select contractID from EodContractForMark)
		and symbol not like '%*'
	if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_ImportOptionRoot
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ImportOptionRoot]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ImportOptionRoot]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_ImportOptionRoot
	@iUnderlyingID int,
	@vcSymbol varchar(20),
	@iLotSize int,
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	declare @error int set @error = 0
	-------------------------------------------------
	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	declare @iOptionRootID int, @iUnderlyingID_bad int

	select @iOptionRootID = optionRootID,
		@iUnderlyingID_bad = underlyingID
	from OptionRoot with(nolock, index = IX_OptionRoot_underlyingID_symbol_actionID)
	where actionID < 3
		and symbol = @vcSymbol
		--and underlyingID != @iUnderlyingID

    if @iOptionRootID is not null and @iUnderlyingID_bad != @iUnderlyingID
	begin
		update OptionRoot
		set symbol = symbol + '*'
		where optionRootID = @iOptionRootID

		if @@error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option root: Fail to mark option root as invalid', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -1
			goto finish
		end

		set @iOptionRootID = null
	end  
	
	/*select @iOptionRootID = optionRootID
	from OptionRoot with(nolock, index = IX_OptionRoot_underlyingID_symbol_actionID)
	where actionID < 3
		and symbol = @vcSymbol
		and underlyingID = @iUnderlyingID*/

	if @iOptionRootID is null
	begin
		insert into OptionRoot
			(underlyingID, symbol, isSynthetic, lotSize)
		values
			(@iUnderlyingID, @vcSymbol, 0, @iLotSize)

		if @@error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option root: Fail to create new option root', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -2
			goto finish
		end

		set @iOptionRootID = @@identity
	end

	if @iWithTran is not null commit tran @vcTranName
finish:
	if @error = 0
		return @iOptionRootID
	else
		return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_DeleteOption
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_DeleteOption]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_DeleteOption]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_DeleteOption
	@iOptionID int,
	@iWithTran int = null
as
	-------------------------------------------------
	set nocount on
	declare @error int set @error = 0
	-------------------------------------------------
	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	if not exists (select 1 from EodContractWithTrades (nolock) where contractID = @iOptionID)
	begin
		delete OptionDataCache where optionID = @iOptionID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
		delete ContractPrice where contractID = @iOptionID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
		delete ContractMarketData where contractID = @iOptionID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
		delete DualQuadraticParams where contractID = @iOptionID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
		delete [Option] where optionID = @iOptionID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
		delete Contract where contractID = @iOptionID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
	end
	else
	begin
		update Contract
		set symbol = symbol + '*'
		where contractID = @iOptionID
			if @@error != 0 begin set @error = -2 if @iWithTran is not null rollback tran @vcTranName goto finish end
	end

	if @iWithTran is not null commit tran @vcTranName

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_ImportOption
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ImportOption]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ImportOption]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_ImportOption
	@iRootID int,
	@iExpiryID int,
	@fStrike float,
	@iIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@fPriceClose float,
	@iWithTran int = null
as
	-------------------------------------------------
	set nocount on
	declare @error int set @error = 0
	-------------------------------------------------
	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	declare	@iOptionID int,
		@iOptionID_bad int

	select @iOptionID = optionID
	from [Option] with(nolock, index = IX_Option_optionRootID_isCall_strike_expiryID_actionID)
	where optionRootID = @iRootID
		and isCall = @iIsCall
		and abs(strike - @fStrike) < 0.009
		and expiryID = @iExpiryID
		and actionID < 3

	if @iOptionID is not null
	begin
		select @iOptionID_bad = contractID
		from Contract with(nolock, index = IX_Contract_symbol_contractTypeID_actionID) --IX_Contract_symbol_contractID_contractTypeID_actionID)
		where symbol = @vcSymbol
			--and contractID != @iOptionID
			and contractTypeID = 3
			and actionID < 3

		if @iOptionID_bad is not null and @iOptionID_bad != @iOptionID
		begin
			exec @error = dbo.usp_AspEodUpdateMarketStructure_DeleteOption
							@iOptionID = @iOptionID_bad, @iWithTran = @iWithTran

			if @@error != 0 or @error != 0
			begin
				exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to delete existing option', @iErrorCode = @@error
				if @iWithTran is not null rollback tran @vcTranName
				set @error = -1
				goto finish
			end
		end
		
		update Contract
		set symbol = @vcSymbol,
			contractName = @vcContractName
		where contractID = @iOptionID
			and actionID < 3

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to update option contract info', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -2
			goto finish
		end

		update [Option]
		set isManualyEntered = 0
		where optionID = @iOptionID
			and actionID < 3

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to update option info', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -3
			goto finish
		end
	end
	else
	begin
		select @iOptionID_bad = contractID
		from Contract with(nolock, index = IX_Contract_symbol_contractTypeID_actionID)
		where symbol = @vcSymbol
			and contractTypeID = 3
			and actionID < 3

		if @iOptionID_bad is not null
		begin
			exec @error = dbo.usp_AspEodUpdateMarketStructure_DeleteOption
							@iOptionID = @iOptionID_bad, @iWithTran = @iWithTran

			if @@error != 0 or @error != 0
			begin
				exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to delete existing option', @iErrorCode = @@error
				if @iWithTran is not null rollback tran @vcTranName
				set @error = -4
				goto finish
			end
		end

		insert into Contract
			(contractTypeID, symbol, contractName)
		values
			(3, @vcSymbol, @vcContractName)

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to insert new option contract', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -5
			goto finish
		end

		set @iOptionID = @@identity

		insert into [Option]
			(optionID, optionRootID, isCall, strike, expiryID)
		values
			(@iOptionID, @iRootID, @iIsCall, @fStrike, @iExpiryID)

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to insert new option', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -6
			goto finish
		end
	end 
	 
	if not exists(select 1 from ContractPrice (nolock) where contractID = @iOptionID and exchangeID is null)
	begin
		insert into ContractPrice
			(contractID, exchangeID, priceClose)
		values (@iOptionID, null, @fPriceClose)
	end 
	else
	begin
		update ContractPrice
		set priceClose = @fPriceClose
		where contractID = @iOptionID
			and exchangeID is null
	end

	if @@error != 0 or @error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to update option close price', @iErrorCode = @@error
		if @iWithTran is not null rollback tran @vcTranName
		set @error = -7
		goto finish
	end

	if @iWithTran is not null commit tran @vcTranName

finish:
	if @error = 0
		return @iOptionID
	else
		return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_ProcessExpiredOptions
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ProcessExpiredOptions]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ProcessExpiredOptions]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_ProcessExpiredOptions 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	-- delete expired options
	truncate table EodContractForDelete
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired options: Fail to truncate temporary table', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into EodContractForDelete
	select o.optionID
	from [Option] o (nolock)
		join OptionRoot ort (nolock, index = IX_OptionRoot_optionRootID_underlyingID_actionID) on o.optionRootID = ort.optionRootID and ort.actionID < 3
		join Contract uc (nolock) on ort.underlyingID = uc.contractID and uc.actionID < 3
		join ExpiryInCalendar eic (nolock) on o.expiryID = eic.expiryID and uc.expCalendarID = eic.expCalendarID and eic.actionID < 3
	where o.actionID < 3
		and eic.expiryDate < getdate() 
		and o.isManualyEntered = 0
		and o.optionID not in (select contractID from EodContractWithTrades (nolock))
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired options: Fail to get useless expired options list', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_DeleteOptions
	if @@error != 0 or @error < 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired options: Fail to delete expired options', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-- mark used expired options as invalid
	truncate table EodContractForMark
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired options: Fail to truncate temporary table', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into EodContractForMark
	select o.optionID
	from [Option] o (nolock)
		join OptionRoot ort (nolock, index = IX_OptionRoot_optionRootID_underlyingID_actionID) on o.optionRootID = ort.optionRootID and ort.actionID < 3
		join Contract uc (nolock) on ort.underlyingID = uc.contractID and uc.actionID < 3
		join ExpiryInCalendar eic (nolock) on o.expiryID = eic.expiryID and uc.expCalendarID = eic.expCalendarID and eic.actionID < 3
	where o.actionID < 3
		and eic.expiryDate < getdate() 
		and o.isManualyEntered = 0
		and o.optionID in (select contractID from EodContractWithTrades (nolock))
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired options: Fail to get used expired options list', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_MarkContracts
	if @@error != 0 or @error < 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired options: Fail to mark expired options as invalid', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_ProcessNewOptions
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ProcessNewOptions]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ProcessNewOptions]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_ProcessNewOptions 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	truncate table EodOptionRoot

	insert EodOptionRoot (eodOptionRootID, underlyingID, symbol, lotSize)
	select
		eod_ort.optionRootID,
		eod_um.contractID,
		eod_ort.symbol,
		eod_ort.lotSize
	from SHARKY.ETS_ASP_EOD.dbo.OptionRoot eod_ort
		inner join EodUnderlyingMap eod_um
				on eod_ort.underlyingID = eod_um.eodContractID


	truncate table EodOption
	
	insert EodOption (eodOptionRootID, symbol, contractName, expiryID, strike, isCall, priceClose)
	select
		eod_o.optionRootID,
		eod_c.symbol,
		eod_c.contractName,
		eod_o.expiryID,
		eod_o.strike,
		eod_o.isCall,
		eod_cp.priceClose
	from
		SHARKY.ETS_ASP_EOD.dbo.[Option] eod_o
		inner join SHARKY.ETS_ASP_EOD.dbo.Contract eod_c
			on eod_c.contractID = eod_o.optionID
		left join SHARKY.ETS_ASP_EOD.dbo.ContractPrice eod_cp
			on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null

	declare @eodOptionRootID int,
		@underlyingID int,
		@symbol varchar(20),
		@lotSize int,
		@expiryID int,
		@strike float,
		@isCall tinyint,
		@contractName varchar(255),
		@priceClose float,
		@optionID int,
		@optionRootID int

	declare cur_OptionRoot cursor local fast_forward read_only
	for select eodOptionRootID, underlyingID, symbol, lotSize
		from EodOptionRoot (nolock)

	open cur_OptionRoot
	
	fetch next from cur_OptionRoot
	into @eodOptionRootID, @underlyingID, @symbol, @lotSize

	while @@fetch_status != -1
	begin
		if @@fetch_status != -2
		begin
	
			exec @optionRootID = dbo.usp_AspEodUpdateMarketStructure_ImportOptionRoot
							@iUnderlyingID = @underlyingID, @vcSymbol = @symbol, @iLotSize = @lotSize
	
			if @@error = 0 and isnull(@optionRootID, 0) > 0
			begin
				declare cur_Option cursor local fast_forward read_only
				for select symbol, contractName, expiryID, strike, isCall, priceClose
					from EodOption with (nolock, index = IX_EodOption_eodOptionRootID)
					where eodOptionRootID = @eodOptionRootID

				open cur_Option
	
				fetch next from cur_Option
				into @symbol, @contractName, @expiryID, @strike, @isCall, @priceClose
	
				while @@fetch_status != -1
				begin
					if @@fetch_status != -2
					begin
		
						exec @optionID = dbo.usp_AspEodUpdateMarketStructure_ImportOption
											@iRootID = @optionRootID,
											@iExpiryID = @expiryID,
											@fStrike = @strike,
											@iIsCall = @isCall,
											@vcSymbol = @symbol,
											@vcContractName = @contractName,
											@fPriceClose = @priceClose
		
						fetch next from cur_Option
						into @symbol, @contractName, @expiryID, @strike, @isCall, @priceClose
					end
				end
	
				close cur_Option
				deallocate cur_Option
			end
		end

		fetch next from cur_OptionRoot
		into @eodOptionRootID, @underlyingID, @symbol, @lotSize
	end

	close cur_OptionRoot
	deallocate cur_OptionRoot

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_ProcessMissedOptions
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ProcessMissedOptions]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ProcessMissedOptions]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_ProcessMissedOptions 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	-- delete not updated options
	truncate table EodContractForDelete
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting missed options: Fail to truncate temporary table', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into EodContractForDelete
	select o.optionID
	from [Option] o (nolock)
		join OptionRoot ort (nolock) on o.optionRootID = ort.optionRootID and ort.actionID < 3
		join Contract uc (nolock) on ort.underlyingID = uc.contractID and uc.actionID < 3
		join EodUnderlyingMap eod_um on eod_um.contractID = uc.contractID
	where o.actionID < 3
		and o.actionDate < uc.MSUpdateDate --uc.actionDate
		and o.isManualyEntered = 0
		and o.optionID not in (select contractID from EodContractWithTrades (nolock))
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting missed options: Fail to get useless missed options list', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_DeleteOptions
	if @@error != 0 or @error < 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting missed options: Fail to delete missed options', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-- mark used expired options as invalid
	truncate table EodContractForMark
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting missed options: Fail to truncate temporary table', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into EodContractForMark
	select o.optionID
	from [Option] o (nolock)
		join OptionRoot ort (nolock) on o.optionRootID = ort.optionRootID and ort.actionID < 3
		join Contract uc (nolock) on ort.underlyingID = uc.contractID and uc.actionID < 3
		join EodUnderlyingMap eod_um on eod_um.contractID = uc.contractID
	where o.actionID < 3
		and o.actionDate < uc.MSUpdateDate --uc.actionDate
		and o.isManualyEntered = 0
		and o.optionID in (select contractID from EodContractWithTrades (nolock))
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting missed options: Fail to get used missed options list', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_MarkContracts
	if @@error != 0 or @error < 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting missed options: Fail to mark missed options as invalid', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_DeleteFutureOptions
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_DeleteFutureOptions]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_DeleteFutureOptions]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_DeleteFutureOptions 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	delete FutureOption
	where futureOptionID in (select contractID from EodContractForDelete)
		if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
	
	delete ContractPrice
	where contractID in (select contractID from EodContractForDelete)
		if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
	
	delete Contract
	where contractID in (select contractID from EodContractForDelete)
		if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_DeleteFutures
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_DeleteFutures]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_DeleteFutures]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_DeleteFutures 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	delete FutureOption
	from FutureOption d
		inner join EodContractForDelete t on t.contractID = d.futureID
	if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	delete Future
	from Future d
		inner join EodContractForDelete t on t.contractID = d.futureID
	if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	delete ContractPrice
	from ContractPrice d
		inner join EodContractForDelete t on t.contractID = d.contractID
	if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
	
	delete ContractExchangeLink
	from ContractExchangeLink d
		inner join EodContractForDelete t on t.contractID = d.contractID
	if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	delete Contract
	from Contract d
		inner join EodContractForDelete t on t.contractID = d.contractID
	if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_ProcessExpiredFutureOptions
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ProcessExpiredFutureOptions]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ProcessExpiredFutureOptions]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_ProcessExpiredFutureOptions 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	-- delete expired future options
	truncate table EodContractForDelete
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired future options: Fail to truncate temporary table', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into EodContractForDelete
	select fo.futureOptionID
	from FutureOption fo (nolock)
		join Future f (nolock) on f.futureID = fo.futureOptionID
		join FutureRoot fr on fr.futureRootID = f.futureRootID
		join ExpiryInCalendar eic (nolock) on f.maturityID = eic.expiryID and fr.matCalendarID = eic.expCalendarID and eic.actionID < 3
	where eic.expiryDate < getdate() 
		and fo.futureOptionID not in (select contractID from EodContractWithTrades (nolock))
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired future options: Fail to get useless expired future options list', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_DeleteFutureOptions
	if @@error != 0 or @error < 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired future options: Fail to delete expired future options', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-- mark used expired options as invalid
	truncate table EodContractForMark
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired future options: Fail to truncate temporary table', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into EodContractForMark
	select fo.futureOptionID
	from FutureOption fo (nolock)
		join Future f (nolock) on f.futureID = fo.futureOptionID
		join FutureRoot fr on fr.futureRootID = f.futureRootID
		join ExpiryInCalendar eic (nolock) on f.maturityID = eic.expiryID and fr.matCalendarID = eic.expCalendarID and eic.actionID < 3
	where eic.expiryDate < getdate() 
		and fo.futureOptionID in (select contractID from EodContractWithTrades (nolock))
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired future options: Fail to get used expired future options list', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_MarkContracts
	if @@error != 0 or @error < 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired future options: Fail to mark expired future options as invalid', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_ProcessExpiredFutures
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ProcessExpiredFutures]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ProcessExpiredFutures]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_ProcessExpiredFutures 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	-- delete expired futures
	truncate table EodContractForDelete
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to truncate temporary table', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into EodContractForDelete
	select f.futureID
	from Future f (nolock)
		join FutureRoot fr on fr.futureRootID = f.futureRootID
		join ExpiryInCalendar eic (nolock) on f.maturityID = eic.expiryID and fr.matCalendarID = eic.expCalendarID and eic.actionID < 3
	where eic.expiryDate < getdate() 
		and f.futureID not in (select contractID from EodContractWithTrades (nolock))
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to get useless expired futures list', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_DeleteFutures
	if @@error != 0 or @error < 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to delete expired futures', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-- mark used expired options as invalid
	truncate table EodContractForMark
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to truncate temporary table', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into EodContractForMark
	select f.futureID
	from Future f (nolock)
		join FutureRoot fr on fr.futureRootID = f.futureRootID
		join ExpiryInCalendar eic (nolock) on f.maturityID = eic.expiryID and fr.matCalendarID = eic.expCalendarID and eic.actionID < 3
	where eic.expiryDate < getdate() 
		and f.futureID in (select contractID from EodContractWithTrades (nolock))
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to get used expired futures list', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_MarkContracts
	if @@error != 0 or @error < 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to mark expired futures as invalid', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_DeleteFuture
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_DeleteFuture]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_DeleteFuture]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_DeleteFuture 
	@iFutureID int,
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	if not exists (select 1 from EodContractWithTrades (nolock) where contractID = @iFutureID)
		and not exists (select 1
					from EodContractWithTrades (nolock)
					where contractID in (select futureOptionID from FutureOption (nolock) where futureID = @iFutureID))
	begin
		delete Future
		where futureID = @iFutureID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
	
		delete ContractPrice
		where contractID = @iFutureID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
		
		delete ContractExchangeLink
		where contractID = @iFutureID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
	
		delete Contract
		where contractID = @iFutureID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
	end
	else
	begin
		update Contract
		set symbol = symbol + '*'
		where contractID = @iFutureID
			if @@error != 0 begin set @error = -2 if @iWithTran is not null rollback tran @vcTranName goto finish end
	end

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_ImportFuture
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ImportFuture]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ImportFuture]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_ImportFuture
	@iFutureRootID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iMaturityID int,
	@fPriceClose float,
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	declare @error int set @error = 0
	-------------------------------------------------
	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	declare @iFutureID int,
			@iUndPriceProfileID int,
			@iOptPriceProfileID int,
			@iFutureID_bad int

	select
		@iFutureID = futureID
	from Future (nolock, index = IX_Future_futureRootID_maturityID)
	where futureRootID = @iFutureRootID
		and maturityID = @iMaturityID

	if @iFutureID is not null
	begin
		select @iFutureID_bad = contractID
		from Contract with(nolock, index = IX_Contract_symbol_contractTypeID_actionID) --IX_Contract_symbol_contractID_contractTypeID_actionID)
		where symbol = @vcSymbol
			--and contractID != @iFutureID
			and contractTypeID = 4
			and actionID < 3

		if @iFutureID_bad is not null and @iFutureID_bad != @iFutureID
		begin
			exec @error = dbo.usp_AspEodUpdateMarketStructure_DeleteFuture
							@iFutureID = @iFutureID_bad, @iWithTran = @iWithTran

			if @@error != 0 or @error != 0
			begin
				exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future: Fail to delete existing future', @iErrorCode = @@error
				if @iWithTran is not null rollback tran @vcTranName
				set @error = -1
				goto finish
			end
		end

		update Contract
		set symbol = @vcSymbol,
			contractName = @vcContractName
		where contractID = @iFutureID
			and actionID < 3

		if @@error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future: Fail to update contract info', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -1
			goto finish
		end

		update Future
		set actionDate = getdate()
		where futureID = @iFutureID

		if @@error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future: Fail to update future info', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -1
			goto finish
		end
	end
	else
	begin
		select @iFutureID_bad = contractID
		from Contract with(nolock, index = IX_Contract_symbol_contractTypeID_actionID)
		where symbol = @vcSymbol
			and contractTypeID = 4
			and actionID < 3

		if @iFutureID_bad is not null
		begin
			exec @error = dbo.usp_AspEodUpdateMarketStructure_DeleteFuture
							@iFutureID = @iFutureID_bad, @iWithTran = @iWithTran

			if @@error != 0 or @error != 0
			begin
				exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future: Fail to delete existing future', @iErrorCode = @@error
				if @iWithTran is not null rollback tran @vcTranName
				set @error = -1
				goto finish
			end
		end
	
		select
			@iUndPriceProfileID = priceProfileID
		from
			PriceProfile (nolock)
		where actionID < 3
			and defaultID = 1
			and isOptionProfile = 0

		if @@error != 0 or @iUndPriceProfileID is null
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future: Fail to get default underlying price profile', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -1
			goto finish
		end
	
		select
			@iOptPriceProfileID = priceProfileID
		from
			PriceProfile (nolock)
		where actionID < 3
			and defaultID = 1
			and isOptionProfile = 1

		if @@error != 0 or @iOptPriceProfileID is null
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future: Fail to get default option price profile', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -1
			goto finish
		end
	
		insert into Contract
			(contractTypeID, symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
		values 
			(4, @vcSymbol, @vcContractName, 1, @iUndPriceProfileID, @iOptPriceProfileID)

		if @@error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future: Fail to insert futures contract', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -1
			goto finish
		end
	
		set @iFutureID = @@identity

		insert into Future
			(futureID, futureRootID, maturityID)
		values
			(@iFutureID, @iFutureRootID, @iMaturityID)

		if @@error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future: Fail to insert futures', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -1
			goto finish
		end
	end
	
	if not exists(select 1 from ContractPrice (nolock) where contractID = @iFutureID and exchangeID is null)
	begin
		insert into ContractPrice
			(contractID, exchangeID, priceClose)
		values (@iFutureID, null, @fPriceClose)
	end 
	else
	begin
		update ContractPrice
		set priceClose = @fPriceClose
		where contractID = @iFutureID
			and exchangeID is null
	end

	if @@error != 0 or @error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future: Fail to update futures close price', @iErrorCode = @@error
		if @iWithTran is not null rollback tran @vcTranName
		set @error = -7
		goto finish
	end

	if @iWithTran is not null commit tran @vcTranName

finish:
	if @error = 0
		return @iFutureID
	else
		return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_DeleteFutureOption
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_DeleteFutureOption]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_DeleteFutureOption]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_DeleteFutureOption
	@iFutureOptionID int,
	@iWithTran int = null
as
	-------------------------------------------------
	set nocount on
	declare @error int set @error = 0
	-------------------------------------------------
	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	if not exists (select 1 from EodContractWithTrades (nolock) where contractID = @iFutureOptionID)
	begin
		delete FutureOption where futureOptionID = @iFutureOptionID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

		delete ContractPrice where contractID = @iFutureOptionID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end

		delete Contract where contractID = @iFutureOptionID
			if @@error != 0 begin set @error = -1 if @iWithTran is not null rollback tran @vcTranName goto finish end
	end
	else
	begin
		update Contract
		set symbol = symbol + '*'
		where contractID = @iFutureOptionID
			if @@error != 0 begin set @error = -2 if @iWithTran is not null rollback tran @vcTranName goto finish end
	end

	if @iWithTran is not null commit tran @vcTranName

finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_ImportFutureOption
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ImportFutureOption]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ImportFutureOption]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_ImportFutureOption
	@iFutureID int,
	@iExpiryID int,
	@fStrike float,
	@iIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@fPriceClose float,
	@iWithTran int = null
as
	-------------------------------------------------
	set nocount on
	declare @error int set @error = 0
	-------------------------------------------------
	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	declare	@iFutureOptionID int,
		@iFutureOptionID_bad int

	select @iFutureOptionID = futureOptionID
	from FutureOption with(nolock, index = IX_FutureOption_futureID_isCall_strike_expiryID)
	where futureID = @iFutureID
		and isCall = @iIsCall
		and abs(strike - @fStrike) < 0.009
		and expiryID = @iExpiryID

	if @iFutureOptionID is not null
	begin
		select @iFutureOptionID_bad = contractID
		from Contract with(nolock, index = IX_Contract_symbol_contractTypeID_actionID) --IX_Contract_symbol_contractID_contractTypeID_actionID)
		where symbol = @vcSymbol
			--and contractID != @iFutureOptionID
			and contractTypeID = 5
			and actionID < 3

		if @iFutureOptionID_bad is not null and @iFutureOptionID_bad != @iFutureOptionID
		begin
			exec @error = dbo.usp_AspEodUpdateMarketStructure_DeleteFutureOption
							@iFutureOptionID = @iFutureOptionID_bad, @iWithTran = @iWithTran

			if @@error != 0 or @error != 0
			begin
				exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future option: Fail to delete existing future option', @iErrorCode = @@error
				if @iWithTran is not null rollback tran @vcTranName
				set @error = -1
				goto finish
			end
		end
		
		update Contract
		set symbol = @vcSymbol,
			contractName = @vcContractName
		where contractID = @iFutureOptionID
			and actionID < 3

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future option: Fail to update option contract info', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -2
			goto finish
		end

		update FutureOption
		set actionDate = getdate()
		where futureOptionID = @iFutureOptionID

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future option: Fail to update option info', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -3
			goto finish
		end
	end
	else
	begin
		select @iFutureOptionID_bad = contractID
		from Contract with(nolock, index = IX_Contract_symbol_contractTypeID_actionID)
		where symbol = @vcSymbol
			and contractTypeID = 5
			and actionID < 3

		if @iFutureOptionID_bad is not null
		begin
			exec @error = dbo.usp_AspEodUpdateMarketStructure_DeleteFutureOption
							@iFutureOptionID = @iFutureOptionID_bad, @iWithTran = @iWithTran

			if @@error != 0 or @error != 0
			begin
				exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future option: Fail to delete existing option', @iErrorCode = @@error
				if @iWithTran is not null rollback tran @vcTranName
				set @error = -4
				goto finish
			end
		end

		insert into Contract
			(contractTypeID, symbol, contractName)
		values
			(5, @vcSymbol, @vcContractName)

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future option: Fail to insert new option contract', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -5
			goto finish
		end

		set @iFutureOptionID = @@identity

		insert into [FutureOption]
			(futureOptionID, futureID, isCall, strike, expiryID)
		values
			(@iFutureOptionID, @iFutureID, @iIsCall, @fStrike, @iExpiryID)

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future option: Fail to insert new option', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -6
			goto finish
		end
	end 
	 
	if not exists(select 1 from ContractPrice (nolock) where contractID = @iFutureOptionID and exchangeID is null)
	begin
		insert into ContractPrice
			(contractID, exchangeID, priceClose)
		values (@iFutureOptionID, null, @fPriceClose)
	end 
	else
	begin
		update ContractPrice
		set priceClose = @fPriceClose
		where contractID = @iFutureOptionID
			and exchangeID is null
	end

	if @@error != 0 or @error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future option: Fail to update option close price', @iErrorCode = @@error
		if @iWithTran is not null rollback tran @vcTranName
		set @error = -7
		goto finish
	end

	if @iWithTran is not null commit tran @vcTranName

finish:
	if @error = 0
		return @iFutureOptionID
	else
		return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_ProcessNewFutureOptions
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ProcessNewFutureOptions]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ProcessNewFutureOptions]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_ProcessNewFutureOptions 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	update FutureRoot
	set futureLotSize = eod_frm.eodFutureLotSize,
		optionLotSize = eod_frm.eodOptionLotSize
	from FutureRoot f
		inner join EodFutureRootMap eod_frm
			on eod_frm.futureRootID = f.futureRootID

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future options: Fail to update future root info', @iErrorCode = @@error
		if @iWithTran is not null rollback tran @vcTranName
		set @error = -1
		goto finish
	end

	truncate table EodFuture

	insert EodFuture (eodFutureID, futureRootID, symbol, contractName, maturityID, priceClose)
	select
		eod_f.futureID,
		eod_frm.futureRootID,
		eod_c.symbol,
		eod_c.contractName,
		eod_f.maturityID,
		eod_cp.priceClose
	from
		SHARKY.ETS_ASP_EOD.dbo.Future eod_f
		inner join SHARKY.ETS_ASP_EOD.dbo.Contract eod_c
			on eod_c.contractID = eod_f.futureID
		inner join EodFutureRootMap eod_frm
				on eod_frm.eodFutureRootID = eod_f.futureRootID
		left join SHARKY.ETS_ASP_EOD.dbo.ContractPrice eod_cp
			on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null

	truncate table EodFutureOption
	
	insert EodFutureOption (eodFutureID, symbol, contractName, expiryID, strike, isCall, priceClose)
	select
		eod_fo.futureID,
		eod_c.symbol,
		eod_c.contractName,
		eod_fo.expiryID,
		eod_fo.strike,
		eod_fo.isCall,
		eod_cp.priceClose
	from
		SHARKY.ETS_ASP_EOD.dbo.FutureOption eod_fo
		inner join SHARKY.ETS_ASP_EOD.dbo.Contract eod_c
			on eod_c.contractID = eod_fo.futureOptionID
		left join SHARKY.ETS_ASP_EOD.dbo.ContractPrice eod_cp
			on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null

	declare @eodFutureID int,
		@futureRootID int,
		@symbol varchar(20),
		@maturityID int,
		@expiryID int,
		@strike float,
		@isCall tinyint,
		@contractName varchar(255),
		@priceClose float,
		@futureOptionID int,
		@futureID int

	declare cur_Future cursor local fast_forward read_only
	for select eodFutureID, futureRootID, symbol, contractName, maturityID, priceClose
		from EodFuture (nolock)

	open cur_Future
	
	fetch next from cur_Future
	into @eodFutureID, @futureRootID, @symbol, @contractName, @maturityID, @priceClose

	while @@fetch_status != -1
	begin
		if @@fetch_status != -2
		begin
	
			exec @futureID = dbo.usp_AspEodUpdateMarketStructure_ImportFuture
							@iFutureRootID = @futureRootID,
							@vcSymbol = @symbol,
							@vcContractName = @contractName,
							@iMaturityID = @maturityID,
							@fPriceClose = @priceClose
	
			if @@error = 0 and isnull(@futureID, 0) > 0
			begin
				declare cur_FutureOption cursor local fast_forward read_only
				for select symbol, contractName, expiryID, strike, isCall, priceClose
					from EodFutureOption with (nolock, index = IX_EodFutureOption_eodFutureID)
					where eodFutureID = @eodFutureID

				open cur_FutureOption
	
				fetch next from cur_FutureOption
				into @symbol, @contractName, @expiryID, @strike, @isCall, @priceClose
	
				while @@fetch_status != -1
				begin
					if @@fetch_status != -2
					begin
		
						exec @futureOptionID = dbo.usp_AspEodUpdateMarketStructure_ImportFutureOption
											@iFutureID = @futureID,
											@iExpiryID = @expiryID,
											@fStrike = @strike,
											@iIsCall = @isCall,
											@vcSymbol = @symbol,
											@vcContractName = @contractName,
											@fPriceClose = @priceClose
		
						fetch next from cur_FutureOption
						into @symbol, @contractName, @expiryID, @strike, @isCall, @priceClose
					end
				end
	
				close cur_FutureOption
				deallocate cur_FutureOption
			end
		end

		fetch next from cur_Future
		into @eodFutureID, @futureRootID, @symbol, @contractName, @maturityID, @priceClose
	end

	close cur_Future
	deallocate cur_Future

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_ProcessMissedFutureOptions
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ProcessMissedFutureOptions]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ProcessMissedFutureOptions]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_ProcessMissedFutureOptions 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	-- delete expired future options
	truncate table EodContractForDelete
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired future options: Fail to truncate temporary table', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into EodContractForDelete
	select fo.futureOptionID
	from FutureOption fo (nolock)
		join Future f (nolock) on f.futureID = fo.futureOptionID
		join FutureRoot fr on fr.futureRootID = f.futureRootID
		join Contract uc (nolock) on fr.underlyingID = uc.contractID and uc.actionID < 3
		join EodUnderlyingMap eod_um on eod_um.contractID = uc.contractID
	where fo.actionDate < uc.MSUpdateDate --uc.actionDate
		and fo.futureOptionID not in (select contractID from EodContractWithTrades (nolock))
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired future options: Fail to get useless expired future options list', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_DeleteFutureOptions
	if @@error != 0 or @error < 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired future options: Fail to delete expired future options', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-- mark used expired options as invalid
	truncate table EodContractForMark
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired future options: Fail to truncate temporary table', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into EodContractForMark
	select fo.futureOptionID
	from FutureOption fo (nolock)
		join Future f (nolock) on f.futureID = fo.futureOptionID
		join FutureRoot fr on fr.futureRootID = f.futureRootID
		join Contract uc (nolock) on fr.underlyingID = uc.contractID and uc.actionID < 3
		join EodUnderlyingMap eod_um on eod_um.contractID = uc.contractID
	where fo.actionDate < uc.MSUpdateDate --uc.actionDate
		and fo.futureOptionID in (select contractID from EodContractWithTrades (nolock))
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired future options: Fail to get used expired future options list', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_MarkContracts
	if @@error != 0 or @error < 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired future options: Fail to mark expired future options as invalid', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_ProcessMissedFutures
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ProcessMissedFutures]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ProcessMissedFutures]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_ProcessMissedFutures 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	-- delete expired futures
	truncate table EodContractForDelete
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to truncate temporary table', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into EodContractForDelete
	select f.futureID
	from Future f (nolock)
		join FutureRoot fr on fr.futureRootID = f.futureRootID
		join Contract uc (nolock) on fr.underlyingID = uc.contractID and uc.actionID < 3
		join EodUnderlyingMap eod_um on eod_um.contractID = uc.contractID
	where f.actionDate < uc.MSUpdateDate --uc.actionDate
		and f.futureID not in (select contractID from EodContractWithTrades (nolock))
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to get useless expired futures list', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_DeleteFutures
	if @@error != 0 or @error < 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to delete expired futures', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-- mark used expired options as invalid
	truncate table EodContractForMark
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to truncate temporary table', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into EodContractForMark
	select f.futureID
	from Future f (nolock)
		join FutureRoot fr on fr.futureRootID = f.futureRootID
		join Contract uc (nolock) on fr.underlyingID = uc.contractID and uc.actionID < 3
		join EodUnderlyingMap eod_um on eod_um.contractID = uc.contractID
	where f.actionDate < uc.MSUpdateDate -- uc.actionDate
		and f.futureID in (select contractID from EodContractWithTrades (nolock))
	if @@error != 0
	begin
		set @error = @@error
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to get used expired futures list', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_MarkContracts
	if @@error != 0 or @error < 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Deleting expired futures: Fail to mark expired futures as invalid', @iErrorCode = @error
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error
GO

----------------------------------------------------------------------------------------------------
-- usp_AspEodUpdateMarketStructure_Execute
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_Execute]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_AspEodUpdateMarketStructure_Execute]
GO

CREATE PROC dbo.usp_AspEodUpdateMarketStructure_Execute 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

	-------------------------------------------------
	-- underlying ID mapping
	truncate table EodUnderlyingMap
	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to truncate underlyings map table', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert EodUnderlyingMap
		(eodContractID, contractID)
	select
		eod_c.contractID,
		c.contractID
	from Contract c (nolock)
		full join SHARKY.ETS_ASP_EOD.dbo.Contract eod_c
			on eod_c.symbol = c.symbol and eod_c.contractTypeID = c.contractTypeID
	where
		c.actionID < 3
		and eod_c.contractTypeID in (1, 2)
		and c.contractTypeID in (1, 2)

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to get underlyings map', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-------------------------------------------------
	-- future roots ID mapping
	truncate table EodFutureRootMap
	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to truncate future roots table', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert EodFutureRootMap
		(eodFutureRootID, futureRootID, eodFutureLotSize, eodOptionLotSize)
	select
		eod_fr.futureRootID,
		fr.futureRootID,
		eod_fr.futureLotSize,
		eod_fr.optionLotSize
	from FutureRoot fr (nolock)
		inner join EodUnderlyingMap eod_um on eod_um.contractID = fr.underlyingID
		inner join SHARKY.ETS_ASP_EOD.dbo.FutureRoot eod_fr
				on eod_fr.underlyingID = eod_um.eodContractID
					and eod_fr.futureRootSymbol = fr.futureRootSymbol

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to get future roots map', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-------------------------------------------------
	-- stocks & indices
	delete from TradeLog
	from TradeLog tl
		inner join Contract c (nolock) on tl.underlyingID = c.contractID
		inner join EodUnderlyingMap eod_um on eod_um.contractID = c.contractID
		inner join SHARKY.ETS_ASP_EOD.dbo.Contract eod_c
				on eod_c.contractID = eod_um.eodContractID and eod_c.contractTypeID in (1, 2)
	where c.actionID < 3

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to clear trades log', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-- contract names
	update Contract
	set
		contractName = eod_c.contractName,
		MSupdateDate = eod_c.MSupdateDate
	from Contract c (nolock)
		inner join EodUnderlyingMap eod_um on eod_um.contractID = c.contractID
		inner join SHARKY.ETS_ASP_EOD.dbo.Contract eod_c
				on eod_c.contractID = eod_um.eodContractID and eod_c.contractTypeID in (1, 2)
	where c.actionID < 3

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to update contracts', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-- stocks info
	update Stock
	set divAmt = eod_s.divAmt,
		divDate = eod_s.divDate, 
		divFreq = eod_s.divFreq
	from Stock s (nolock)
		inner join Contract c (nolock) on c.contractID = s.stockID and c.actionID < 3
		inner join EodUnderlyingMap eod_um on eod_um.contractID = c.contractID
		inner join SHARKY.ETS_ASP_EOD.dbo.Contract eod_c
				on eod_c.contractID = eod_um.eodContractID and eod_c.contractTypeID = 2
		inner join SHARKY.ETS_ASP_EOD.dbo.Stock eod_s
				on eod_c.contractID = eod_s.stockID
	where s.actionID < 3

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to update stocks info', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-- close prices
	update ContractPrice
	set priceClose = eod_cp.priceClose
	from ContractPrice cp (nolock)
		inner join EodUnderlyingMap eod_um on eod_um.contractID = cp.contractID
		inner join SHARKY.ETS_ASP_EOD.dbo.Contract eod_c
				on eod_c.contractID = eod_um.eodContractID and eod_c.contractTypeID in (1, 2)
		inner join SHARKY.ETS_ASP_EOD.dbo.ContractPrice eod_cp on eod_um.eodContractID = eod_cp.contractID and eod_cp.exchangeID is null
	where cp.exchangeID is null

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to update underlying close prices', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-------------------------------------------------
	-- options & option roots

	truncate table EodContractWithTrades

	insert into EodContractWithTrades (contractID)
	select contractID
	from Contract
	where actionID < 3
		and (
				contractID in (select contractID from Trade group by contractID)
				or contractID in (select underlyingID from Trade group by underlyingID)
				or contractID in (select futureID from Trade group by futureID)
				or contractID in (select contractID from [Order] group by contractID)
				or contractID in (select optID from TntCard group by optID)
				or contractID in (select undID from TntCard group by undID)
			)

	exec @error = usp_AspEodUpdateMarketStructure_ProcessExpiredOptions
	if @@error != 0 or @error != 0
	begin
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_ProcessNewOptions
	if @@error != 0 or @error != 0
	begin
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_ProcessMissedOptions
	if @@error != 0 or @error != 0
	begin
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-------------------------------------------------
	-- futures & futures options
	exec @error = usp_AspEodUpdateMarketStructure_ProcessExpiredFutureOptions
	if @@error != 0 or @error != 0
	begin
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_ProcessExpiredFutures
	if @@error != 0 or @error != 0
	begin
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_ProcessNewFutureOptions
	if @@error != 0 or @error != 0
	begin
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_ProcessMissedFutureOptions
	if @@error != 0 or @error != 0
	begin
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	exec @error = usp_AspEodUpdateMarketStructure_ProcessMissedFutures
	if @@error != 0 or @error != 0
	begin
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-- clear deleted options
	delete OptionDataCache where optionID in (select optionID from [Option] where actionID >= 3)
	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to clear deleted options', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	delete [Option] where actionID >= 3
	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to clear deleted options', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-- delete option roots w/o options
	delete SyntheticUnderlyingParam  where syntheticOptRootID not in 
		(select S.syntheticOptRootID from [Option] O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to delete option roots w/o options', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	delete SyntheticRootIndexBeta  where syntheticOptRootID not in 
		(select S.syntheticOptRootID from [Option] O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID)
	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to delete option roots w/o options', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	delete SyntheticOptionRoot where optionRootID not in (select optionRootID from [Option] where actionID < 3)
	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to delete option roots w/o options', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	delete OptionRoot where optionRootID not in (select optionRootID from [Option] where actionID < 3)
	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to delete option roots w/o options', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-------------------------------------------------
	-- underlying index betas
	truncate table IndexBeta
	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating index betas: Fail to clear old betas', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into IndexBeta (indexID, contractID, beta) 
	select eod_i.contractID,
		eod_u.contractID,
		eod_ib.beta
	from SHARKY.ETS_ASP_EOD.dbo.IndexBeta eod_ib
		inner join EodUnderlyingMap eod_i on eod_i.eodContractID = eod_ib.indexID
		inner join EodUnderlyingMap eod_u on eod_u.eodContractID = eod_ib.contractID
	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating index betas: Fail to insert new betas', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-------------------------------------------------
	-- underlying skew & kurtosis
	update [Index]
	set skew = eod_i.skew,
		kurt = eod_i.kurt
	from [Index] i
		inner join EodUnderlyingMap eod_um on eod_um.contractID = i.indexID
		inner join SHARKY.ETS_ASP_EOD.dbo.[Index] eod_i on eod_i.indexID = eod_um.eodContractID
	where i.actionID < 3
	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating skew & kurtosis: Fail to update index skew & kurtosis', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	-------------------------------------------------
	-- default IVs
	insert into VolaSurfaceData (contractID, isDefaultSurface, baseUnderlinePrice)
	select c.contractID,
		1,
		isnull(cp.priceClose, 0)
	from Contract c
		inner join ContractPrice cp on cp.contractID = c.contractID
		left join VolaSurfaceData vsd on vsd.contractID = c.contractID and vsd.isDefaultSurface = 1 and vsd.actionID < 3
	where c.actionID < 3
		and c.contractTypeID < 3
		and vsd.contractID is null

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating default IV surface: Fail to create missed IV surfaces', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into CustomStrikeSkewPoint (volaSurfaceDataID, type, strike, volatility, isBasePoint, expiryID)
	select vsd.volaSurfaceDataID,
		eod_cssp.type,
		eod_cssp.strike,
		eod_cssp.volatility,
		eod_cssp.isBasePoint,
		eod_cssp.expiryID
	from SHARKY.ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp
		inner join SHARKY.ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.volaSurfaceDataID = eod_cssp.volaSurfaceDataID
		inner join EodUnderlyingMap eod_um on eod_um.eodContractID = eod_vsd.contractID
		inner join VolaSurfaceData vsd on vsd.contractID = eod_um.contractID
		left join CustomStrikeSkewPoint cssp on 
			cssp.volaSurfaceDataID = vsd.volaSurfaceDataID
			and abs(cssp.strike - eod_cssp.strike) < 0.009
			and cssp.type = eod_cssp.type
			and cssp.expiryID = eod_cssp.expiryID 
			and cssp.actionID < 3
	where
		cssp.CustomStrikeSkewPointID is null

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating default IV surface: Fail to add new IV points', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	update CustomStrikeSkewPoint
	set type = eod_cssp.type,
		strike = eod_cssp.strike,
		volatility = eod_cssp.volatility,
		isBasePoint = eod_cssp.isBasePoint,
		expiryID = eod_cssp.expiryID
	from SHARKY.ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp
		inner join SHARKY.ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.volaSurfaceDataID = eod_cssp.volaSurfaceDataID
		inner join EodUnderlyingMap eod_um on eod_um.eodContractID = eod_vsd.contractID
		inner join VolaSurfaceData vsd on vsd.contractID = eod_um.contractID
		inner join CustomStrikeSkewPoint cssp on 
			cssp.volaSurfaceDataID = vsd.volaSurfaceDataID
			and abs(cssp.strike - eod_cssp.strike) < 0.009
			and cssp.type = eod_cssp.type
			and cssp.expiryID = eod_cssp.expiryID 
			and cssp.actionID < 3

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating default IV surface: Fail to update IV points', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	if @iWithTran is not null commit tran @vcTranName
	
finish:
	return @error
GO

