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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.198'  or (keyValue = '3.46.199') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.199 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.199' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
ALTER   PROCEDURE usp_Index_Save
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
		@SOQ as integer = 0,
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
		--if @iExpCalendarID is null or @iExpCalendarID = 0
		--	set @iExpCalendarID = 2
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
                  values (1, @vcSymbol, @vcContractName, null, @iUndPriceProfileID, @iOptPriceProfileID)

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
							kurt,
							SOQ)
					 values (
							@iIndexID, 
							@fYield,
							isNull(@bIsHTB,0),
							@tiCalcOptionType,
							@tiCalcModelType,
							isnull(@tiIsBasket,0), 
							@tiIsActive,
							@fSkew,
							@fKurt,
							@SOQ)
				 if (@@error <> 0)
					begin
					   RAISERROR ('Error. Can''t insert into [Index]!', 16, 1)
					   ROLLBACK TRAN
					   RETURN (-1)
					end 			

   		 --insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcSymbol, @vcSymbol)
         --        /* begin FT_CQ00008370*/
         --        declare @iExpCalendarID_Last int
         --       set @iExpCalendarID_Last = SCOPE_IDENTITY()

         --        update Contract set expCalendarID = @iExpCalendarID_Last
         --          where contractID = @iIndexID
         --        /* end   FT_CQ00008370*/
	
		 commit tran
	 end
   else
	 begin
		begin tran
		   update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@vcContractName,contractName),
					--expCalendarID = null,--isNull(@iExpCalendarID, expCalendarID),
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
					kurt = isnull(@fKurt,Kurt),
					SOQ = isnull(@SOQ,SOQ)
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
ALTER      PROC usp_TradeFeed_1_New
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
			select -1 as iMaxSeqNum, @iSeqNum as iMinSeqNum, -1 as iStructureMissed
			Rollback Transaction
			Return(-1)
		end 
	end

	Declare @iTradeContractID int, 
			@iUnderlyingContractID int,
			@iStructureMissed int

	set @iStructureMissed = 0

	--check underlying symbol
	if @vcUnderlyingSymbol is null and  @iContractTypeID <3 
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
				select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed
				Rollback Transaction
				Return(-1)			
			end	
		end
        end
       
	select @iUnderlyingContractID = contractID 
		from ContractView 
		where symbol = @vcUnderlyingSymbol and contractTypeID < 3

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
			
			if @iOptionID = 0 begin set @iOptionID = NULL end
	
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
			select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed
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
		@iUnderlyingContractID, @iOptionRootID, null, null, @fStockRefPrice, @iOrderID, @iExecStatus)

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
	select isnull(@iMinSeqNum,@iSeqNum) as iMinSeqNum,@iSeqNum as iMaxSeqNum, @iStructureMissed as iStructureMissed

	Commit Transaction

    set @error = 0
	return @error

finish:
	select -1 as iMinSeqNum, -1 as iMaxSeqNum, -1 as iStructureMissed
	return @@error


GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_FindOption]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_FindOption]
GO

GO

CREATE   PROC dbo.usp_FindOption
                        @iOptionRootID INT,
                        @fStrike FLOAT,
                        @tiIsCall TINYINT,
                        @dtExpiry DATETIME
AS

            DECLARE /*@iExpiry INT,*/ @iOptionID INT  
            
            set @iOptionID = 0       

            IF NOT @dtExpiry IS NULL

            BEGIN
                        --SET @iExpiry = CAST( @dtExpiry AS INT )
                        -- first, try to find option with same strike, same root and same expiration exactly
                        SELECT
                                   @iOptionID = optionID
                        FROM
                                   OptionView (NOLOCK)
                        WHERE 
                                   optionRootID = @iOptionRootID AND ABS( strike - @fStrike ) < 0.009 AND
                                   DATEDIFF(d, expiryDate, @dtExpiry) = 0 AND isCall = @tiIsCall


                        --IF @iOptionID IS NOT NULL GOTO Finish
                        IF @@ROWCOUNT = 1 GOTO Finish

                        -- couldn't find option with exact same parameters. try to find one in same month and year

                        SELECT
                                   @iOptionID = optionID
                        FROM
                                   OptionView (NOLOCK)
                        WHERE 
                                   optionRootID = @iOptionRootID AND ABS( strike - @fStrike ) < 0.009 AND
                                   DATEPART(yyyy, expiryDate ) = DATEPART( yyyy, @dtExpiry ) AND
                                   DATEPART(mm, expiryDate ) = DATEPART( mm, @dtExpiry ) AND
                                   isCall = @tiIsCall

 

                        IF @@ROWCOUNT > 1 

                        BEGIN
                                   -- more than one option found                 
                                   SET @iOptionID = 0 GOTO Finish 
                        END

 

                        IF @iOptionID IS NOT NULL GOTO Finish
                        -- still nothing, let's try using only month
                        SELECT
                                   @iOptionID = optionID
                        FROM
                                   OptionView (NOLOCK)
                        WHERE 
                                   optionRootID = @iOptionRootID AND ABS( strike - @fStrike ) < 0.009 AND
                                   DATEPART(yyyy, expiryDate) = DATEPART( yyyy, @dtExpiry ) AND
                                   isCall = @tiIsCall

 

                        IF @@ROWCOUNT > 1 

                        BEGIN
                                   -- more than one option found                 
                                   SET @iOptionID = 0 
                        END

            END     

Finish:

            RETURN @iOptionID


GO
/*
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEod_RawData_StocksDefaultIVSurface_Load]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AspEod_RawData_StocksDefaultIVSurface_Load]
*/
GO

GO
/*
CREATE  PROC usp_AspEod_RawData_StocksDefaultIVSurface_Load
AS
	INSERT INTO IVUnderlyingPointsRawData (iContractID, dtExpDate, iExpiryID, fStrike, fIV, dtActionDate)
		SELECT	cv.ContractID, IV.dtExpDate, 
				CAST( CAST(IV.dtExpDate AS DATETIME) AS INT ) AS iExpiryID,
				IV.fStrike,IV.fIV,GETDATE() AS dtActionDate
		FROM 
			IVData.IV.Dbo.FT_DefaultIV IV
	 			INNER JOIN ContractView cv ON cv.Symbol = IV.vcSymbol AND cv.ContractTypeID IN (1,2)
*/			
GO
/*
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEod_RawData_FuturesDefaultIVSurface_Load]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AspEod_RawData_FuturesDefaultIVSurface_Load]
*/
GO

/*
CREATE   PROC usp_AspEod_RawData_FuturesDefaultIVSurface_Load
AS
 SET NOCOUNT ON

 INSERT INTO IVUnderlyingPointsRawData (iContractID, dtExpDate, iExpiryID, fStrike, fIV, dtActionDate)
 	SELECT 
		QQ.ContractID   AS ContractID, IV.dtExpDate	AS dtExpDate,
		CAST( CAST(IV.dtExpDate AS DATETIME) AS INT ) AS ExpiryID,IV.fStrike	AS fStrike,
		CAST( IV.fIV AS FLOAT) AS fIV, GETDATE() AS dtActionDate
	FROM 
		(
			SELECT	cu.ContractID, IV.fStrike, IV.dtExpDate, fr.UnderlyingID, 
					MAX(fr.futureLotSize) AS maxLosSize, COUNT( fr.futureRootID) AS fRootsNumber
			FROM 
				FutureRoot fr 
				INNER JOIN IVData.IV.Dbo.FT_DefaultIV_Futures IV ON IV.vcSymbol = fr.futureRootSymbol
				INNER JOIN ContractView cu ON cu.ContractID = fr.underlyingID AND cu.ContractTypeID = 6
			GROUP BY IV.fStrike, IV.dtExpDate,fr.UnderlyingId, cu.ContractID
		) QQ
		INNER JOIN FutureRoot gh ON gh.underlyingID=qq.UnderlyingID AND gh.futureLotSize = qq.maxLosSize
		INNER JOIN IVData.IV.Dbo.FT_DefaultIV_Futures IV ON IV.vcSymbol = gh.futureRootSymbol AND qq.fStrike = IV.fStrike and qq.dtExpDate = iv.dtExpDate
		ORDER BY qq.UnderlyingID, qq.dtExpDate, qq.fStrike
*/
GO

ALTER   PROC dbo.usp_AspEodUpdateMarketStructure_ProcessExpiredOptions 
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
	where o.actionID < 3
		and datediff(d, GetDate(), o.expiryDate) <  0
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
	where o.actionID < 3
		and datediff(d, GetDate(), o.expiryDate) < 0
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
		and (datediff(m, expiryDate, @dtExpiryDate)) = 0
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
							 @iOptionID_bad,  @iWithTran

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
		set isManualyEntered = 0,
		    expiryDate = @dtExpiryDate
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
							 @iOptionID_bad,  @iWithTran

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

ALTER   PROC dbo.usp_AspEodUpdateMarketStructure_ProcessExpiredFutureOptions 
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
	where datediff(d, GetDate(), fo.expiryDate ) < 0
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
	where DATEDIFF(D, getDate(),fo.expiryDate )< 0
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

ALTER   PROC usp_AspEodUpdateMarketStructure_ProcessExpiredFutures 
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
	where datediff(d,GetDate(),f.maturityDate )< 0
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
	where datediff(d, Getdate(),f.maturityDate) < 0
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


ALTER   PROC dbo.usp_AspEodUpdateMarketStructure_ProcessNewOptions 
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
	from ETS_ASP_EOD.dbo.OptionRoot eod_ort
		inner join EodUnderlyingMap eod_um
				on eod_ort.underlyingID = eod_um.eodContractID


	truncate table EodOption
	
	insert EodOption (eodOptionRootID, symbol, contractName, expiryDate, strike, isCall, priceClose)
	select
		eod_o.optionRootID,
		eod_c.symbol,
		eod_c.contractName,
		eod_o.expiryDate,
		eod_o.strike,
		eod_o.isCall,
		eod_cp.priceClose
	from
		ETS_ASP_EOD.dbo.[Option] eod_o
		inner join ETS_ASP_EOD.dbo.Contract eod_c
			on eod_c.contractID = eod_o.optionID
		left join ETS_ASP_EOD.dbo.ContractPrice eod_cp
			on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null

	declare @eodOptionRootID int,
		@underlyingID int,
		@symbol varchar(20),
		@lotSize int,
		@expiryDate datetime,
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
				for select symbol, contractName, expiryDate, strike, isCall, priceClose
					from EodOption with (nolock, index = IX_EodOption_eodOptionRootID)
					where eodOptionRootID = @eodOptionRootID

				open cur_Option
	
				fetch next from cur_Option
				into @symbol, @contractName, @expiryDate, @strike, @isCall, @priceClose
	
				while @@fetch_status != -1
				begin
					if @@fetch_status != -2
					begin
		
						exec @optionID = dbo.usp_AspEodUpdateMarketStructure_ImportOption
											@iRootID = @optionRootID,
											@dtExpiryDate = @expiryDate,
											@fStrike = @strike,
											@iIsCall = @isCall,
											@vcSymbol = @symbol,
											@vcContractName = @contractName,
											@fPriceClose = @priceClose
		
						fetch next from cur_Option
						into @symbol, @contractName, @expiryDate, @strike, @isCall, @priceClose
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
GO


ALTER  PROC dbo.usp_AspEodUpdateMarketStructure_ProcessMissedFutures 
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
	where datediff(d,uc.MSUpdateDate,  f.actionDate) < 0 or uc.MSUpdateDate is null --uc.actionDate
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

GO


ALTER  PROC dbo.usp_AspEodUpdateMarketStructure_ProcessMissedFutureOptions 
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
	where datediff(d,uc.MSUpdateDate, fo.actionDate) < 0  --uc.actionDate
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
	where datediff(d,uc.MSUpdateDate, fo.actionDate) < 0  --uc.actionDate
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



ALTER    PROC dbo.usp_AspEodUpdateMarketStructure_ImportFuture
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
		and datediff( m, maturityDate, @dtMaturityDate ) = 0

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
ALTER   PROC dbo.usp_AspEodUpdateMarketStructure_ProcessNewFutureOptions 
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

	insert EodFuture (eodFutureID, futureRootID, symbol, contractName, maturityDate, priceClose)
	select
		eod_f.futureID,
		eod_frm.futureRootID,
		eod_c.symbol,
		eod_c.contractName,
		eod_f.maturityDate,
		eod_cp.priceClose
	from
		ETS_ASP_EOD.dbo.Future eod_f
		inner join ETS_ASP_EOD.dbo.Contract eod_c
			on eod_c.contractID = eod_f.futureID
		inner join EodFutureRootMap eod_frm
				on eod_frm.eodFutureRootID = eod_f.futureRootID
		left join ETS_ASP_EOD.dbo.ContractPrice eod_cp
			on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null

	truncate table EodFutureOption
	
	insert EodFutureOption (eodFutureID, symbol, contractName, expiryDate, strike, isCall, priceClose)
	select
		eod_fo.futureID,
		eod_c.symbol,
		eod_c.contractName,
		eod_fo.expiryDate,
		eod_fo.strike,
		eod_fo.isCall,
		eod_cp.priceClose
	from
		ETS_ASP_EOD.dbo.FutureOption eod_fo
		inner join ETS_ASP_EOD.dbo.Contract eod_c
			on eod_c.contractID = eod_fo.futureOptionID
		left join ETS_ASP_EOD.dbo.ContractPrice eod_cp
			on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null

	declare @eodFutureID int,
		@futureRootID int,
		@symbol varchar(20),
		@maturityDate datetime,
		@expiryDate datetime,
		@strike float,
		@isCall tinyint,
		@contractName varchar(255),
		@priceClose float,
		@futureOptionID int,
		@futureID int

	declare cur_Future cursor local fast_forward read_only
	for select eodFutureID, futureRootID, symbol, contractName, maturityDate, priceClose
		from EodFuture (nolock)

	open cur_Future
	
	fetch next from cur_Future
	into @eodFutureID, @futureRootID, @symbol, @contractName, @maturityDate, @priceClose

	while @@fetch_status != -1
	begin
		if @@fetch_status != -2
		begin
	
			exec @futureID = dbo.usp_AspEodUpdateMarketStructure_ImportFuture
							@iFutureRootID = @futureRootID,
							@vcSymbol = @symbol,
							@vcContractName = @contractName,
							@dtMaturityDate = @maturityDate,
							@fPriceClose = @priceClose
	
			if @@error = 0 and isnull(@futureID, 0) > 0
			begin
				declare cur_FutureOption cursor local fast_forward read_only
				for select symbol, contractName, expiryDate, strike, isCall, priceClose
					from EodFutureOption with (nolock, index = IX_EodFutureOption_eodFutureID)
					where eodFutureID = @eodFutureID

				open cur_FutureOption
	
				fetch next from cur_FutureOption
				into @symbol, @contractName, @expiryDate, @strike, @isCall, @priceClose
	
				while @@fetch_status != -1
				begin
					if @@fetch_status != -2
					begin
		
						exec @futureOptionID = dbo.usp_AspEodUpdateMarketStructure_ImportFutureOption
											@iFutureID = @futureID,
											@dtExpiryDate = @expiryDate,
											@fStrike = @strike,
											@iIsCall = @isCall,
											@vcSymbol = @symbol,
											@vcContractName = @contractName,
											@fPriceClose = @priceClose
		
						fetch next from cur_FutureOption
						into @symbol, @contractName, @expiryDate, @strike, @isCall, @priceClose
					end
				end
	
				close cur_FutureOption
				deallocate cur_FutureOption
			end
		end

		fetch next from cur_Future
		into @eodFutureID, @futureRootID, @symbol, @contractName, @maturityDate, @priceClose
	end

	close cur_Future
	deallocate cur_Future

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error



GO
GO

ALTER       PROC usp_AspEodUpdateMarketStructure_Main_Execute 
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
  full join ETS_ASP_EOD.dbo.Contract eod_c
   on eod_c.symbol = c.symbol --and eod_c.contractTypeID = c.contractTypeID -- it is possible to have in client base indeced declared as stocks
 where
  c.actionID < 3
  and eod_c.actionID < 3
  and eod_c.contractTypeID in (1,2,4)
  and c.contractTypeID in (1,2,4)
 
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
  inner join ETS_ASP_EOD.dbo.FutureRoot eod_fr
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
  inner join ETS_ASP_EOD.dbo.Contract eod_c
    on eod_c.contractID = eod_um.eodContractID and eod_c.contractTypeID < 6
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
  inner join ETS_ASP_EOD.dbo.Contract eod_c
    on eod_c.contractID = eod_um.eodContractID and eod_c.contractTypeID in (1,2,4)
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
  inner join ETS_ASP_EOD.dbo.Contract eod_c
    on eod_c.contractID = eod_um.eodContractID and eod_c.contractTypeID = 2
  inner join ETS_ASP_EOD.dbo.Stock eod_s
    on eod_c.contractID = eod_s.stockID
 where s.actionID < 3
 
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to update stocks info', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 -----------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------
 
 update ContractPrice
 set priceClose = eod_cp.priceClose, volume10Day = eod_cp.volume10Day, volume5Expiration = eod_cp.volume5Expiration
 from  ETS_ASP_EOD.dbo.ContractView eod_cv 
  inner join ContractView cv on eod_cv.symbol = cv.symbol and (cv.contractTypeID = eod_cv.contractTypeID)
  inner join ContractPrice cp (nolock) on cv.contractID = cp.contractID
  inner join ETS_ASP_EOD.dbo.ContractPrice eod_cp on eod_cv.contractID = eod_cp.contractID
 
 -- case when the symbol type in EOD base and client base are differ. Only for stocks and indeces
 update ContractPrice
 set priceClose = eod_cp.priceClose, volume10Day = eod_cp.volume10Day, volume5Expiration = eod_cp.volume5Expiration
 from  ETS_ASP_EOD.dbo.ContractView eod_cv 
  inner join ContractView cv on eod_cv.symbol = cv.symbol and cv.contractTypeID <> eod_cv.contractTypeID and cv.contractTypeID in (1,2)  
  inner join ContractPrice cp (nolock) on cv.contractID = cp.contractID
  inner join ETS_ASP_EOD.dbo.ContractPrice eod_cp on eod_cv.contractID = eod_cp.contractID
 where eod_cv.contractTypeID in (1,2,6,7)
 
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to update ContractPrice info', @iErrorCode = @@error
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
 
	update OptionRoot set LotSize = EOD.LotSize from OptionRoot O
	inner join EODOptionRoot EOD on O.Symbol COLLATE Latin1_General_CI_AS = EOD.symbol COLLATE Latin1_General_CI_AS  and EOD.LotSize <> O.LotSize
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
 
 delete  SyntheticUnderlyingParam where syntheticOptRootID in
 ( select syntheticOptRootID  from SyntheticOptionRoot where optionRootID not in (select optionRootID from [Option] where actionID < 3))
 
 delete SyntheticOptionRoot where optionRootID not in (select optionRootID from [Option] where actionID < 3)
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to delete option roots w/o options', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 delete [Option] where actionID >= 3
 
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
	select 	eod_i.contractID,
		eod_u.contractID,
		eod_ib.beta
	from ETS_ASP_EOD.dbo.IndexBeta eod_ib
		inner join EodUnderlyingMap eod_i on eod_i.eodContractID = eod_ib.indexID
		inner join EodUnderlyingMap eod_u on eod_u.eodContractID = eod_ib.contractID
		inner join [Index] i on i.indexId = eod_i.contractID and i.indexId in 
						(
							select contractID 
							from ContractView 
							where symbol in ('SPX', 'DJX', 'OEX', 'NDX')
						)
	where eod_ib.actionID <3 

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
  inner join ETS_ASP_EOD.dbo.[Index] eod_i on eod_i.indexID = eod_um.eodContractID
 where i.actionID < 3
 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating skew & kurtosis: Fail to update index skew & kurtosis', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
             -----------------------------------------------------------------------------------------------------------------------------
             
    ----------------------------------------------------------------------------------
 --Updating IV bands
 
 delete VolaBands where contractID in
   (select distinct cv.contractID 
   from  ETS_ASP_EOD.dbo.ContractView eod_cv 
    inner join ContractView cv on eod_cv.symbol = cv.symbol
    inner join VolaBands vb (nolock) on cv.contractID = vb.contractID
    inner join ETS_ASP_EOD.dbo.VolaBands eod_vb on eod_cv.contractID = eod_vb.contractID)
 
            if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating IV bands: Fail to delete IV bands', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 

 insert into VolaBands (contractID, expPeriod, strikeMoneyness, IVBandLow, IVBandHi) 
 select cv.contractID, eod_vb.ExpPeriod, eod_vb.StrikeMoneyness, eod_vb.IVBandLow, eod_vb.IVBandHi
  from  ETS_ASP_EOD.dbo.ContractView eod_cv 
   inner join ContractView cv on eod_cv.symbol = cv.symbol and cv.contractTypeID = eod_cv.contractTypeID
   inner join ETS_ASP_EOD.dbo.VolaBands eod_vb on eod_cv.contractID = eod_vb.contractID
 
        if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating IV bands: Fail to update IV bands', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 -- case when the symbol type in EOD base and client base are differ. Only for stocks and indeces
 insert into VolaBands (contractID, expPeriod, strikeMoneyness, IVBandLow, IVBandHi) 
 select cv.contractID, eod_vb.ExpPeriod, eod_vb.StrikeMoneyness, eod_vb.IVBandLow, eod_vb.IVBandHi
  from  ETS_ASP_EOD.dbo.ContractView eod_cv 
   inner join ContractView cv on eod_cv.symbol = cv.symbol and cv.contractTypeID <> eod_cv.contractTypeID and cv.contractTypeID in (1,2)
   inner join ETS_ASP_EOD.dbo.VolaBands eod_vb on eod_cv.contractID = eod_vb.contractID
  where eod_cv.contractTypeID in (1,2)
 
        if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating IV bands: Fail to update IV bands', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 ----------------------------------------------------------------------------------
 

 if @iWithTran is not null commit tran @vcTranName
 
finish:
 return @error

GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_DefaultIVs_Execute]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AspEodUpdateMarketStructure_DefaultIVs_Execute]
GO

create  PROC dbo.usp_AspEodUpdateMarketStructure_DefaultIVs_Execute 
	@iWithTran int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int set @error = 0

	declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')
	if @iWithTran is not null begin tran @vcTranName

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
		and c.contractTypeID in (1,2,6)
		and vsd.contractID is null

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating default IV surface: Fail to create missed IV surfaces', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	insert into CustomStrikeSkewPoint (volaSurfaceDataID, type, strike, volatility, isBasePoint, expiryDate)
	select vsd.volaSurfaceDataID,
		eod_cssp.type,
		eod_cssp.strike,
		eod_cssp.volatility,
		eod_cssp.isBasePoint,
		eod_cssp.expiryDate

	from ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp
		inner join ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.volaSurfaceDataID = eod_cssp.volaSurfaceDataID
		inner join EodUnderlyingMap eod_um on eod_um.eodContractID = eod_vsd.contractID
		inner join VolaSurfaceData vsd on vsd.contractID = eod_um.contractID
		left join CustomStrikeSkewPoint cssp on 
			cssp.volaSurfaceDataID = vsd.volaSurfaceDataID
			and abs(cssp.strike - eod_cssp.strike) < 0.009
			and cssp.type = eod_cssp.type
			and abs(datediff( d, cssp.expiryDate, eod_cssp.expiryDate )) < 2 
			and cssp.actionID < 3
	where
		cssp.CustomStrikeSkewPointID is null
		and eod_cssp.actionID < 3

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
	    expiryDate = eod_cssp.expiryDate
	from ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp
		inner join ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.volaSurfaceDataID = eod_cssp.volaSurfaceDataID
		inner join EodUnderlyingMap eod_um on eod_um.eodContractID = eod_vsd.contractID
		inner join VolaSurfaceData vsd on vsd.contractID = eod_um.contractID
		inner join CustomStrikeSkewPoint cssp on 
			cssp.volaSurfaceDataID = vsd.volaSurfaceDataID
			and abs(cssp.strike - eod_cssp.strike) < 0.009
			and cssp.type = eod_cssp.type
			and abs(datediff( d, cssp.expiryDate, eod_cssp.expiryDate )) < 2 
			and cssp.actionID < 3
	where 
	     eod_cssp.actionID < 3 and
	     eod_cssp.volatility > -100.0 --IV's mark 'not calculated'


	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating default IV surface: Fail to update IV points', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end

	create table #CustStrikeskewpoint (customstrikeskewpoint int)
	insert into #CustStrikeskewpoint (customstrikeskewpoint) --values 
		select csp.customStrikeSkewPointID from contract c 
			inner join VolaSurfaceData vsd on vsd.contractID= c.ContractID
			inner join customstrikeskewpoint csp on vsd.volaSurfaceDataID = csp.volaSurfaceDataID 
		where 
			(not exists(select top 1 1 from optionRoot orr where orr.ActionId<3 and orr.underlyingID =c.ContractID) and not exists(select top 1 1 from futureRoot orr where orr.underlyingID =c.ContractID) )
			and c.ActionId <3
			and  c.contractTypeId in (1,2,6)
		group by csp.customStrikeSkewPointID
	

	delete from customstrikeskewpoint where customStrikeSkewPointID in (
		select customstrikeskewpoint from #CustStrikeskewpoint
	)
/*
	----------------------------------------------------------------------------------
	delete from customstrikeskewpoint where customStrikeSkewPointID in (
	--update customstrikeskewpoint set ActionId = 3 where customStrikeSkewPointID in (	
		select csp.customStrikeSkewPointID from contract c 
			inner join VolaSurfaceData vsd on vsd.contractID= c.ContractID
			inner join customstrikeskewpoint csp on vsd.volaSurfaceDataID = csp.volaSurfaceDataID 
		where 
			not exists(select top 1 1 from optionRoot orr where orr.ActionId<3 and orr.underlyingID =c.ContractID)
			and c.ActionId <3
			and  c.contractTypeId in (1,2,6)
		group by csp.customStrikeSkewPointID
	)
*/
	delete from customstrikeskewpoint where customStrikeSkewPointID in (
	--update customstrikeskewpoint set ActionId = 3 where customStrikeSkewPointID in (	
		select csp.customStrikeSkewPointID from contract c 
			inner join VolaSurfaceData vsd on vsd.contractID= c.ContractID
			inner join customstrikeskewpoint csp on vsd.volaSurfaceDataID = csp.volaSurfaceDataID and  datediff(d,csp.actionDate, getDate()) >0
		where 
			exists(select top 1 1 from optionRoot orr where orr.ActionId<3 and orr.underlyingID =c.ContractID)
			and c.ActionId <3
			and  c.contractTypeId in (1,2,6)
			and c.ContractID  not in (		
				select  tt.contractID from (
					select t.contractID, t.symbol, max(t.maxDate) as md from (
						select  c.contractID, c.symbol, csp.actionDate as maxDate from contract c 
							inner join VolaSurfaceData vsd on vsd.contractID= c.ContractID
							inner join customstrikeskewpoint csp on vsd.volaSurfaceDataID = csp.volaSurfaceDataID --and csp.expiryID> 66
						where 
							(
								exists(select top 1 1 from optionRoot orr where orr.ActionId<3 and orr.underlyingID =c.ContractID)
								or
								exists(select top 1 1 from futureRoot orr where orr.underlyingID =c.ContractID)
							)
							and c.ActionId <3
							and  c.contractTypeId in (1,2,6)
						group by  c.ContractID, c.symbol, csp.actionDate) T
					group by t.contractID, t.symbol
				) TT where datediff(d,TT.md, getDate())<>0)
				group by csp.customStrikeSkewPointID
		)


	if @iWithTran is not null commit tran @vcTranName
	
finish:
	return @error

GO

GO
ALTER  PROC usp_AspEodUpdateMarketStructure_ProcessExpiredFutures 
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
	where f.maturityDate < getdate()
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
	where f.maturityDate < getdate()
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
 

ALTER  PROC dbo.usp_AspEodUpdateMarketStructure_ImportOption
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
		and datediff(m, expiryDate, @dtExpiryDate) = 0 
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


ALTER  PROC dbo.usp_AspEodUpdateMarketStructure_ProcessNewOptions 
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
	from ETS_ASP_EOD.dbo.OptionRoot eod_ort
		inner join EodUnderlyingMap eod_um
				on eod_ort.underlyingID = eod_um.eodContractID


	truncate table EodOption
	
	insert EodOption (eodOptionRootID, symbol, contractName, expiryDate, strike, isCall, priceClose)
	select
		eod_o.optionRootID,
		eod_c.symbol,
		eod_c.contractName,
		eod_o.expiryDate,
		eod_o.strike,
		eod_o.isCall,
		eod_cp.priceClose
	from
		ETS_ASP_EOD.dbo.[Option] eod_o
		inner join ETS_ASP_EOD.dbo.Contract eod_c
			on eod_c.contractID = eod_o.optionID
		left join ETS_ASP_EOD.dbo.ContractPrice eod_cp
			on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null

	declare @eodOptionRootID int,
		@underlyingID int,
		@symbol varchar(20),
		@lotSize int,
		@expiryDate datetime,
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
				for select symbol, contractName, expiryDate, strike, isCall, priceClose
					from EodOption with (nolock, index = IX_EodOption_eodOptionRootID)
					where eodOptionRootID = @eodOptionRootID

				open cur_Option
	
				fetch next from cur_Option
				into @symbol, @contractName, @expiryDate, @strike, @isCall, @priceClose
	
				while @@fetch_status != -1
				begin
					if @@fetch_status != -2
					begin
		
						exec @optionID = dbo.usp_AspEodUpdateMarketStructure_ImportOption
											@iRootID = @optionRootID,
											@dtExpiryDate = @expiryDate,
											@fStrike = @strike,
											@iIsCall = @isCall,
											@vcSymbol = @symbol,
											@vcContractName = @contractName,
											@fPriceClose = @priceClose
		
						fetch next from cur_Option
						into @symbol, @contractName, @expiryDate, @strike, @isCall, @priceClose
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


ALTER  PROC dbo.usp_AspEodUpdateMarketStructure_ImportFutureOption
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
		and datediff( m, expiryDate, @dtExpiryDate ) = 0

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


ALTER  PROC dbo.usp_AspEodUpdateMarketStructure_ProcessNewFutureOptions 
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

	insert EodFuture (eodFutureID, futureRootID, symbol, contractName, maturityDate, priceClose)
	select
		eod_f.futureID,
		eod_frm.futureRootID,
		eod_c.symbol,
		eod_c.contractName,
		eod_f.maturityDate,
		eod_cp.priceClose
	from
		ETS_ASP_EOD.dbo.Future eod_f
		inner join ETS_ASP_EOD.dbo.Contract eod_c
			on eod_c.contractID = eod_f.futureID
		inner join EodFutureRootMap eod_frm
				on eod_frm.eodFutureRootID = eod_f.futureRootID
		left join ETS_ASP_EOD.dbo.ContractPrice eod_cp
			on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null

	truncate table EodFutureOption
	
	insert EodFutureOption (eodFutureID, symbol, contractName, expiryDate, strike, isCall, priceClose)
	select
		eod_fo.futureID,
		eod_c.symbol,
		eod_c.contractName,
		eod_fo.expiryDate,
		eod_fo.strike,
		eod_fo.isCall,
		eod_cp.priceClose
	from
		ETS_ASP_EOD.dbo.FutureOption eod_fo
		inner join ETS_ASP_EOD.dbo.Contract eod_c
			on eod_c.contractID = eod_fo.futureOptionID
		left join ETS_ASP_EOD.dbo.ContractPrice eod_cp
			on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null

	declare @eodFutureID int,
		@futureRootID int,
		@symbol varchar(20),
		@maturityDate datetime,
		@expiryDate datetime,
		@strike float,
		@isCall tinyint,
		@contractName varchar(255),
		@priceClose float,
		@futureOptionID int,
		@futureID int

	declare cur_Future cursor local fast_forward read_only
	for select eodFutureID, futureRootID, symbol, contractName, maturityDate, priceClose
		from EodFuture (nolock)

	open cur_Future
	
	fetch next from cur_Future
	into @eodFutureID, @futureRootID, @symbol, @contractName, @maturityDate, @priceClose

	while @@fetch_status != -1
	begin
		if @@fetch_status != -2
		begin
	
			exec @futureID = dbo.usp_AspEodUpdateMarketStructure_ImportFuture
							@iFutureRootID = @futureRootID,
							@vcSymbol = @symbol,
							@vcContractName = @contractName,
							@dtMaturityDate = @maturityDate,
							@fPriceClose = @priceClose
	
			if @@error = 0 and isnull(@futureID, 0) > 0
			begin
				declare cur_FutureOption cursor local fast_forward read_only
				for select symbol, contractName, expiryDate, strike, isCall, priceClose
					from EodFutureOption with (nolock, index = IX_EodFutureOption_eodFutureID)
					where eodFutureID = @eodFutureID

				open cur_FutureOption
	
				fetch next from cur_FutureOption
				into @symbol, @contractName, @expiryDate, @strike, @isCall, @priceClose
	
				while @@fetch_status != -1
				begin
					if @@fetch_status != -2
					begin
		
						exec @futureOptionID = dbo.usp_AspEodUpdateMarketStructure_ImportFutureOption
											@iFutureID = @futureID,
											@dtExpiryDate = @expiryDate,
											@fStrike = @strike,
											@iIsCall = @isCall,
											@vcSymbol = @symbol,
											@vcContractName = @contractName,
											@fPriceClose = @priceClose
		
						fetch next from cur_FutureOption
						into @symbol, @contractName, @expiryDate, @strike, @isCall, @priceClose
					end
				end
	
				close cur_FutureOption
				deallocate cur_FutureOption
			end
		end

		fetch next from cur_Future
		into @eodFutureID, @futureRootID, @symbol, @contractName, @maturityDate, @priceClose
	end

	close cur_Future
	deallocate cur_Future

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error


GO

/*END DATA MODIFICATION SECTION*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.199')
	begin
		update DataInfo set keyValue = '3.46.199' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.199' '3.46.199 update message:' 
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