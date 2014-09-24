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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.223'  or (keyValue = '3.46.224') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.224 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.224' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END

--=============================================================================================

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



ALTER            PROC usp_TradeFeed_1_New
	@vcTraderAcronym varchar(12),
	@vcExecID nvarchar(128),
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
	@vcStrategyName varchar(12) = null,
	@fStockRefPrice float = null,
	@vcOrderID varchar(50) = null,
	@iExecStatus int = null,
	@vcBrokerName nvarchar(64) = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iMinSeqNum int 
	declare @iSeqNum int

	-- future support
	declare @iFutureRootID int
	set @iFutureRootID = null
	declare @iFutureID int
	set @iFutureID = null
	-- find existing trade

	select @iSeqNum = max(seqNum)
	from Trade 
	where execID = @vcExecID and 
		Convert(varchar(8),@dtTradeDate,112) = Convert(varchar(8),tradeDate,112)
	group by
	 	execID, Convert(varchar(8),tradeDate,112), tradeID
	having
		min(actionID) = 1

	Begin Transaction

	if @iSeqNum is not null
	begin
		if @tiIsUpdateEnabled = 1
		begin
			-- delete existing trade
			--select @iMinSeqNum = isnull(max(seqNum),0) + 1 from Trade
	
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus)
			select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, mark,
				underlyingID, optionRootID, futureRootID, futureID, orderID, execStatus
			from Trade
			where seqNum = @iSeqNum
			if @@error <> 0 begin set @error = 2 if @@TranCount > 0  Rollback Transaction trade goto finish end
			
			set @iMinSeqNum = @@identity
		end
		else
		begin
			--trade already exists
			select -1 as iMaxSeqNum, @iSeqNum as iMinSeqNum, -1 as iStructureMissed, -5 as iInfoId
			Rollback Transaction
			Return(-1)
		end 
	end

	Declare @iTradeContractID int, 
			@iUnderlyingContractID int,
			@iStructureMissed int

	set @iStructureMissed = 0

	--check underlying symbol
	if @vcUnderlyingSymbol is null and  @iContractTypeID < 3 
		set @vcUnderlyingSymbol = @vcContractSymbol

	if @iContractTypeID = 3 and @vcUnderlyingSymbol is null
        begin
		select @vcUnderlyingSymbol = C.Symbol from Contract C
			inner join OptionRoot O on O.symbol = @vcOptRootSymbol and O.underlyingID = C.ContractID 
		where C.contractTypeID < 3
		if @vcUnderlyingSymbol is NULL
		begin
			select @vcUnderlyingSymbol = C.Symbol from ETS_ASP_EOD.dbo.Contract C
				inner join ETS_ASP_EOD.dbo.OptionRoot O on O.symbol = @vcOptRootSymbol and O.underlyingID = C.ContractID 
			where C.contractTypeID < 3
			if @vcUnderlyingSymbol is NULL
			begin
				-- unknown underlying
				select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed, -4 as iInfoId
				Rollback Transaction
				Return(-1)			
			end	
		end
        end
       
	select @iUnderlyingContractID = contractID 
		from ContractView 
		where symbol = @vcUnderlyingSymbol and contractTypeID < 3
	
	if (@iContractTypeID < 3) --for Stocks and Indices
		begin
	
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
		end
	else --future and future optins processing
		-- if this is a future (iContractTypeID = 4) then
		-- dtExpiry is future maturityDate
		-- vcUnderlyingSymbol is "SPX" for example
		-- optionRootSymbol is futureRoot symbol, "ES" for example
		-- then we will try to find future with the same params
		-- maturity, root, undsymbol
		begin		
			if (@iContractTypeID = 4) --trade on future
				begin
					if (@iUnderlyingContractID is null)
						begin
							--infoID = -6 mean that we need update market structure
							--underlying symbol for this future not exists
							select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed, -6 as iInfoId 
							Rollback Transaction
							Return(-1)	
						end
					else	--underlying symbol for this future exists
						begin
							select	@iFutureRootID = futureRootID 
							from	FutureRoot
							where	futureRootSymbol = @vcOptRootSymbol and underlyingID = @iUnderlyingContractID

							if (@iFutureRootID is null)
								begin
									--unable to find FutureRoot for this Future
									select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed, -7 as iInfoId 
									Rollback Transaction
									Return(-1)	
								end
							else
								begin
									select	@iFutureID = futureID 
									from	Future
									where	datediff(mm, @dtExpiry, maturityDate) = 0 and futureRootID = @iFutureRootID
									
									if (@iFutureID is null)
										begin
											select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed, -8 as iInfoId 
											Rollback Transaction
											Return(-1)	
										end
									else
										begin
											set @iTradeContractID = @iFutureID
										end
									
								end
						end
				end
		end

	--check Option
	if @iContractTypeID = 3
		begin
			Declare @iOptionID int,
					--@iExpiryID int,
					@iOptionRootID int

			Select @iOptionRootID = optionRootID from OptionRootView where symbol = @vcOptRootSymbol
	
			--set @iExpiryID = cast(@dtExpiry as int) --(year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
			set @fStrike = round(@fStrike, 3)
	
			--select @iOptionID = optionID
			--	from OptionView (nolock)
			--	where optionRootID = @iOptionRootID and 
			--		isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID < 3
			exec @iOptionID = usp_FindOption @iOptionRootID, @fStrike, @tiIsCall, @dtExpiry
			
			--in case of no Stike/Expiry/IsCall and we have only contractname like 'LMTIQ'
			--will try to finde optionID by name
			if @iOptionID = 0 AND isnull(@vcContractSymbol,'')<>''
			begin 
				select 
					@iOptionID = contractId 
				from 
					contract 
				where 
					contractTypeId=3 and actionId < 3 and symbol = @vcContractSymbol 
				
				if isNull(@iOptionID,0) = 0
				begin
					select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed, -3 as iInfoId 
					Rollback Transaction
					Return(-1)
				end
			end
	
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

						insert into [Option] (optionID, optionRootID, isCall, strike, expiryDate)
							 values (@iOptionID, @iOptionRootID, @tiIsCall, @fStrike, @dtExpiry)
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
		begin
			if @iContractTypeID in (1,2)
			begin
				set @iTradeContractID = @iUnderlyingContractID
			end
		end


	--work with Trader
	Declare @iTraderID int
		select @iTraderID = traderID from TraderView where acronym = @vcTraderAcronym

	if @iTraderID is null
		begin
			insert into Trader(acronym,traderName) values(@vcTraderAcronym, @vcTraderAcronym)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
				set @iTraderID = @@identity
		end
	--work with broker
	Declare @iBrokerID int
		select @iTraderID = traderID from TraderView where acronym = @vcTraderAcronym
		if @vcBrokerName is not null
		begin
			select @iBrokerId = traderId from TradetView where acronym = @vcBrokerName
			if @iBrokerId is NULL
			begin
			insert into Trader(acronym,traderName) values(@vcBrokerName, @vcBrokerName)
				if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end
				set @iBrokerId = @@identity
			end
		end
		else
			set @iBrokerId = null
	
		
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
	--select @iNewTradeID = isnull(max(tradeID), 0) + 1 from Trade
	insert TradeIDs default values
	set @iNewTradeID = @@identity
	
	--select @iSeqNum = isnull(max(seqNum),0) + 1 from Trade
	

	--check if was an execution
	declare @iOrderID int
	if not (@vcOrderID is null)
	begin
		select @iOrderID = orderID from [Order] where @vcOrderID = clOrderID
		if @iOrderID is NULL
		begin
			-- unknown Client OrderID
			select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed, -2 as iInfoId
			Rollback Transaction
			Return(-1)			
		end
	end
	else
		set @iOrderID = null
		
	--create new trade	
	insert into Trade (execID, tradeID, actionID, contractID, tradeDate, 
		quantity, price, traderID, strategyID, isBuy, isPosition, status, mark,
		underlyingID, optionRootID, futureRootID, futureID, spotReference, orderID, execStatus)
	values (@vcExecID, @iNewTradeID, 1, @iTradeContractID, @dtTradeDate, 
		@iQuantity, @fPrice, @iTraderID, @iStrategyID, @tiIsBuy, 0, 0, @iMark,
		@iUnderlyingContractID, @iOptionRootID, @iFutureRootID, @iFutureID, @fStockRefPrice, @iOrderID, @iExecStatus)

	if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

	set @iSeqNum = @@identity

	--calculate order AvgPrice
	if not (@iOrderID is null)
	begin
		declare @fAvgPrice float
		declare @iCumQty int		set @iCumQty = null
		
		exec usp_FixOrderAvgPrice_Calc @iOrderID, @fAvgPrice output, @iCumQty output
	
		update [Order]
			set 
				status = @iExecStatus,
				avgPrice = isnull(@fAvgPrice, avgPrice), 
				cumQty = isnull(@iCumQty, cumQty) 
			where orderID = @iOrderID
	end

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
	select isnull(@iMinSeqNum,@iSeqNum) as iMinSeqNum,@iSeqNum as iMaxSeqNum, @iStructureMissed as iStructureMissed, 0 as iInfoId

	Commit Transaction

    set @error = 0
	return @error

finish:
	select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed, -1 as iInfoId
	return @@error



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO




--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.224')
	begin
		update DataInfo set keyValue = '3.46.224' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.224' '3.46.224 update message:' 
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
