SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_ImportFutureOption]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AspEodUpdateMarketStructure_ImportFutureOption]
GO







/* ---------------------------- */

CREATE   PROC dbo.usp_AspEodUpdateMarketStructure_ImportFutureOption
	@iFutureID int,
	@dtExpiryDate datetime,
	@fStrike float,
	@iIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@fPriceClose float,
	@dtExpiryOV datetime = null,
	@dtTradeClose datetime = null,
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
		from Contract with(nolock)
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
		
		update Contract set
			symbol = @vcSymbol,
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

		update FutureOption set
			actionDate = getdate(),
			expiryDate = @dtExpiryDate,
			ExpiryOV = @dtExpiryOV,
			TradingClose = @dtTradeClose -- should be expiration date filter also I guess */
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
		from Contract with(nolock)
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
		values	(5, @vcSymbol, @vcContractName)

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing future option: Fail to insert new option contract', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -5
			goto finish
		end

		set @iFutureOptionID = @@identity

		insert into [FutureOption]
			(futureOptionID, futureID, isCall, strike, expiryDate, ExpiryOV, TradingClose)
		values	(@iFutureOptionID, @iFutureID, @iIsCall, @fStrike, @dtExpiryDate, @dtExpiryOV, @dtTradeClose)

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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

