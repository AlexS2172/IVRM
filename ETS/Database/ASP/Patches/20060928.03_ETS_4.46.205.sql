SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.204'  or (keyValue = '3.46.205') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.205 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.205' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
GO

ALTER     PROC dbo.usp_AspEodUpdateMarketStructure_ImportFuture
	@iFutureRootID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@dtMaturityDate datetime,
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
		and abs(datediff( d, maturityDate, @dtMaturityDate ))< 10

	if @iFutureID is not null
	begin
		select @iFutureID_bad = contractID
		from Contract with(nolock, index = IX_Contract_symbol_contractTypeID_actionID) --IX_Contract_symbol_contractID_contractTypeID_actionID)
		where symbol = @vcSymbol
			--and contractID != @iFutureID
			and contractTypeID = 4
			and actionID < 3
			and @iFutureID_bad not in ( select contractID from EodContractWithTrades )

		if @iFutureID_bad is not null and @iFutureID_bad != @iFutureID
		begin
			exec @error = dbo.usp_AspEodUpdateMarketStructure_DeleteFuture	 @iFutureID_bad,  @iWithTran

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
		,maturityDate= @dtMaturityDate
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
			and @iFutureID_bad not in ( select contractID from EodContractWithTrades )

		if @iFutureID_bad is not null
		begin
			exec @error = dbo.usp_AspEodUpdateMarketStructure_DeleteFuture	@iFutureID_bad,  @iWithTran

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
			(futureID, futureRootID, maturityDate)
		values
			(@iFutureID, @iFutureRootID, @dtMaturityDate)

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

ALTER   PROC dbo.usp_AspEodUpdateMarketStructure_ImportFutureOption
	@iFutureID int,
	@dtExpiryDate datetime,
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
		and abs(datediff( d, expiryDate, @dtExpiryDate ))<10

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
		,expiryDate= @dtExpiryDate
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
			(futureOptionID, futureID, isCall, strike, expiryDate)
		values
			(@iFutureOptionID, @iFutureID, @iIsCall, @fStrike, @dtExpiryDate)

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



ALTER   PROC dbo.usp_AspEodUpdateMarketStructure_ImportOption
	@iRootID int,
	@dtExpiryDate datetime,
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
		and abs(datediff(d, expiryDate, @dtExpiryDate))<10 
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
		set isManualyEntered = 0, expiryDate = @dtExpiryDate
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
			(optionID, optionRootID, isCall, strike, expiryDate)
		values
			(@iOptionID, @iRootID, @iIsCall, @fStrike, @dtExpiryDate)

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

ALTER    PROC dbo.usp_AspEodUpdateMarketStructure_ProcessMissedFutures 
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
	where datediff(d,uc.MSUpdateDate,  f.actionDate) < 0  --uc.actionDate
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
		where datediff(d, uc.MSUpdateDate, f.actionDate) < 0 or uc.MSUpdateDate is null
			and uc.ContractTypeID < 3
			and f.futureID in (select contractID from EodContractWithTrades (nolock))

	insert into EodContractForMark
	select f.futureID
		from Future f (nolock)
			join FutureRoot fr on fr.futureRootID = f.futureRootID
			join Contract uc (nolock) on fr.underlyingID = uc.contractID and uc.actionID < 3
			join EodUnderlyingMap eod_um on eod_um.contractID = uc.contractID
		where datediff(d, GetDate(), f.actionDate) <0 
			and uc.ContractTypeID = 6
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
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.205')
	begin
		update DataInfo set keyValue = '3.46.205' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.205' '3.46.205 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO