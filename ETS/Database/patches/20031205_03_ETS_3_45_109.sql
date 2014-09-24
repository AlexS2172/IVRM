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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.45.108' or (keyValue = '3.45.109')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.45.109 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.45.109' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

----------------------------------------------------------------------------------------------------
-- usp_ContractParameters_CheckValidity
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractParameters_CheckValidity]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ContractParameters_CheckValidity]
GO

CREATE PROC dbo.usp_ContractParameters_CheckValidity
    @iContractID int, -- required parameter
	@iUnderlyingID int, -- required parameter
	@iOptionRootID int = null,
	@iFutureRootID int = null,
	@iFutureID int = null
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	declare @iContractTypeID int
	declare @iID int

	-- check for a valid parameters
	if @iContractID is null or @iUnderlyingID is null
	begin
		set @error = -101 -- invalid parameters
		goto finish
	end

	-- find a contract type
	select @iContractTypeID = contractTypeID
	from ContractView
	where contractID = @iContractID

	-- check for a contract existence
	if @iContractTypeID is null
	begin
		set @error = -102 -- invalid contract value
		goto finish
	end

	-- check data for underlying
	if @iContractTypeID = 1 or @iContractTypeID = 2
	begin
		if @iUnderlyingID != @iContractID
			or @iOptionRootID is not null
			or @iFutureRootID is not null
			or @iFutureID is not null
		begin
			set @error = -103 -- invalid contract data for underlying
			goto finish
		end

		goto finish
	end

	-- check data for option
	if @iContractTypeID = 3
	begin
		if @iOptionRootID is null
			or @iFutureRootID is not null
			or @iFutureID is not null
		begin
			set @error = -104 -- invalid contract data for option
			goto finish
		end

		-- check option data
		if not exists(select 1 from OptionView o where o.optionID = @iContractID)
		begin
			set @error = -105 -- invalid option data for option
			goto finish
		end

		set @iID = null
		-- check option root
		select @iID = ort.optionRootID
		from OptionRootView ort
			inner join OptionView o on o.optionRootID = ort.optionRootID
		where
			o.optionID = @iContractID

		if isnull(@iID, 0) != @iOptionRootID
		begin
			set @error = -106 -- invalid option root data for option
			goto finish
		end

		set @iID = null
		-- check underlying
		select @iID = uc.contractID
		from ContractView uc
			inner join OptionRootView ort on ort.underlyingID = uc.contractID
		where
			ort.optionRootID = @iOptionRootID

		if isnull(@iID, 0) != @iUnderlyingID
		begin
			set @error = -107 -- invalid underlying data for option
			goto finish
		end

		goto finish
	end

	-- check data for futures
	if @iContractTypeID = 4
	begin
		if @iFutureID != @iContractID
			or @iOptionRootID is not null
			or @iFutureRootID is null
			or @iFutureID is null
		begin
			set @error = -108 -- invalid contract data for futures
			goto finish
		end

		-- check futures data
		if not exists(select 1 from Future f where f.futureID = @iContractID)
		begin
			set @error = -109 -- invalid futures data for futures
			goto finish
		end

		set @iID = null
		-- check futures root
		select @iID = frt.futureRootID
		from FutureRoot frt
			inner join Future f on f.futureRootID = frt.futureRootID
		where
			f.futureID = @iContractID

		if isnull(@iID, 0) != @iFutureRootID
		begin
			set @error = -110 -- invalid futures root data for futures
			goto finish
		end

		set @iID = null
		-- check underlying
		select @iID = uc.contractID
		from ContractView uc
			inner join FutureRoot frt on frt.underlyingID = uc.contractID
		where
			frt.futureRootID = @iFutureRootID

		if isnull(@iID, 0) != @iUnderlyingID
		begin
			set @error = -111 -- invalid underlying data for futures
			goto finish
		end

		goto finish
	end

	-- check data for futures option
	if @iContractTypeID = 5
	begin
		if @iFutureID is null
			or @iOptionRootID is not null
			or @iFutureRootID is null
		begin
			set @error = -112 -- invalid contract data for futures option
			goto finish
		end

		-- check futures option data
		if not exists(select 1 from FutureOption f where f.futureOptionID = @iContractID)
		begin
			set @error = -113 -- invalid futures option data for futures option
			goto finish
		end

		set @iID = null
		-- check futures
		select @iID = f.futureID
		from Future f
			inner join FutureOption fo on fo.futureID = f.futureID
		where
			fo.futureOptionID = @iContractID

		if isnull(@iID, 0) != @iFutureID
		begin
			set @error = -114 -- invalid futures data for futures option
			goto finish
		end

		set @iID = null
		-- check futures root
		select @iID = frt.futureRootID
		from FutureRoot frt
			inner join Future f on f.futureRootID = frt.futureRootID
		where
			f.futureID = @iFutureID

		if isnull(@iID, 0) != @iFutureRootID
		begin
			set @error = -115 -- invalid futures root data for futures option
			goto finish
		end

		set @iID = null
		-- check underlying
		select @iID = uc.contractID
		from ContractView uc
			inner join FutureRoot frt on frt.underlyingID = uc.contractID
		where
			frt.futureRootID = @iFutureRootID

		if isnull(@iID, 0) != @iUnderlyingID
		begin
			set @error = -116 -- invalid underlying data for futures option
			goto finish
		end

		goto finish
	end

finish:
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
    @iBrokerID int = Null,
    @fBrokerCommission float =Null,
    @iClearingBrokerID int = Null,
    @fClearingBrokerCommission float =Null,
    @fTradedIV  float = Null,
	@fSpotReference  float = Null,
	@tiIsPosition tinyint = 0,
	@iNewTradeID int out,
	@dtNewTradeDate datetime = Null out,
	@tiStatus tinyint,
	@iMark int = null,
    @iStrategyID int = null out,
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

	if @dtNewTradeDate is null	
	begin
		declare @iDeltaGMT int

		exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

		set @dtNewTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	end

	-- determine strategy for trade
	exec @error = usp_TraderContractStrategyWithSave_Get @iTraderID, @iContractID, @iStrategyID out
	if @@error <> 0 or @error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

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
		set @iStrategyID = null
		set @vcImportID = null
		return @error
	end
GO

/* RESULTS ANALYTHING */
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.45.109')
	begin
		update DataInfo set keyValue = '3.45.109' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.45.109' '3.45.109 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
