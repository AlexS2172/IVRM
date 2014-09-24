SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ImportFuture]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ImportFuture]
GO



CREATE     PROC dbo.usp_AspEodUpdateMarketStructure_ImportFuture
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
		from Contract with(nolock)
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
		from Contract with(nolock)
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

