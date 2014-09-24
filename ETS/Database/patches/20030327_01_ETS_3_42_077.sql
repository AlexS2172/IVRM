/*
	fix for MLine00001355, MLine00001492
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.42.076' or (keyValue = '3.42.077')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.42.077 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.42.077' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/


IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Trade_ClearDeleted' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Trade_ClearDeleted
GO

CREATE PROCEDURE dbo.usp_Trade_ClearDeleted 
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	Begin Transaction Trade

		delete from TradeLog 
		where seqNum in (select seqNum
						from Trade 
						where tradeID in (select tradeID from Trade where actionID = 0))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

		delete from Trade 
		where tradeID in (select tradeID from Trade where actionID = 0) 
			if @@error !=0 begin set @error = 2 if @@TranCount = 1 Rollback Tran goto finish end

		set @error = @@error

	Commit Transaction Trade

finish:
Return(@error)

GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Trade_Clear' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Trade_Clear
GO

CREATE PROCEDURE dbo.usp_Trade_Clear
	@iContractID int = null,
	@tiOnlyWithExecID tinyint = null,
	@lastUTCTradeDate datetime = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction

		delete from TradeLog 
		where seqNum in (select seqNum 
						from Trade 
						where tradeID in 
								(select tradeID from Trade where isnull(status, 0) in (1, 2) and actionID = 0
									 and (@iContractID is null or contractID = @iContractID)))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

		delete from Trade 
		where tradeID in (select tradeID from Trade where isnull(status, 0) in (1, 2) and actionID = 0) 
							and (@iContractID is null or contractID = @iContractID)
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
	
		if isnull(@tiOnlyWithExecID, 0) = 0
		begin
			delete from TradeLog 
				where seqNum in (select seqNum from Trade
									where (@iContractID is null or contractID = @iContractID) 
										and isnull(status, 0) in (0, 3)
										and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end	

			delete from Trade 
				where (@iContractID is null or contractID = @iContractID) 
					and isnull(status, 0) in (0, 3)
					and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0)
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
		end
		else
		begin
			delete from TradeLog 
				where seqNum in (select seqNum from Trade
									where (@iContractID is null or contractID = @iContractID) 
										and isnull(status, 0) in (0, 3)
										and isnull(execID, '') <> ''
										and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0))
			if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end

			delete from Trade 
			where (@iContractID is null or contractID = @iContractID) 
				and isnull(status, 0) in (0, 3)
				and isnull(execID, '') <> ''
				and (@lastUTCTradeDate is null or datediff(day,tradeDate,@lastUTCTradeDate) >= 0)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
		end
		set @error = @@error
	Commit Transaction

finish:
	return @error

GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_TradeSim_Clear' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_TradeSim_Clear
GO

CREATE PROCEDURE dbo.usp_TradeSim_Clear
	@tiSimulated tinyint = null,
	@tiManual tinyint = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Begin Transaction
		if isnull(@tiSimulated, 0) <> 0
		begin
			delete from TradeLog 
			where seqNum in (select seqNum 
							from Trade 
							where tradeID in 
								(select tradeID from Trade where isnull(status, 0) = 1))
				if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
	
			delete from Trade 
			where tradeID in (select tradeID from Trade where isnull(status, 0) = 1) 
				if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
		end
	
		if isnull(@tiManual, 0) <> 0
		begin
			delete from TradeLog 
			where seqNum in (select seqNum 
							from Trade 
							where tradeID in 
								(select tradeID from Trade where isnull(status, 0) = 2))
				if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
	
			delete from Trade 
			where tradeID in (select tradeID from Trade where isnull(status, 0) = 2) 
				if @@error !=0 begin set @error = 1 if @@TranCount = 1 Rollback Tran goto finish end
		end
	
		set @error = @@error
	Commit Transaction

finish:
	return @error

GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_OptionPair_Del' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_OptionPair_Del
GO

CREATE PROCEDURE dbo.usp_OptionPair_Del
	@iOptionIDCall int,
	@iOptionIDPut int
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Option table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	Create table #Options_for_delete(OptionID int)
	if  @iOptionIDCall is not null
		begin
			insert into #Options_for_delete values(@iOptionIDCall)
				if @@error > 0 begin set @error = 1 goto finish end
		end
	if  @iOptionIDPut is not null
		begin
			insert into #Options_for_delete values(@iOptionIDPut)
			if @@error > 0 begin set @error = 1 goto finish end
		end
	if exists (select tradeID from Trade where contractID in (select optionID from #Options_for_delete))
		begin
			Raiserror ('Unable to delete the Option. There is an open position on it.', 16, 1)
			Return (-1)
		end 
	if object_ID('tempdb..#Options_for_delete') is not null
		begin
			exec usp_Option_Del
			if @@error <> 0 set @error = 1 goto finish
		end
	set @error = @@error
finish:
Return(@error)

GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_DataHistoryDH_Save' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_DataHistoryDH_Save
GO

CREATE PROCEDURE dbo.usp_DataHistoryDH_Save
	@iContractID int,		-- indexID or stockID
	@iContractTypeID int,	-- 1 or 2
	@fCurrentIV float = null,
	@fCurrentHV float = null,
	@fCurrentVolOfVol float = null,
	@fCurrentVega float = null
as
/*
	Created by Zaur Nuraliev
	this procedure save historical data for index/stock
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iContractTypeID = 1 
		begin
			if not exists (select indexID from IndexDataHistory where indexID = @iContractID and tenorID=8 and [date]=(select max([date]) from IndexDataHistory where indexID = @iContractID and tenorID=8))
				begin
					insert into IndexDataHistory(
						indexID,
						tenorID,
						[date],
						IVMean,
						currentIV,
						currentHV,
						currentVolOfVol,
						currentVega)
					values(
						@iContractID,
						8,
						GetDate(),
						isnull(@fCurrentIV,0),
						@fCurrentIV,
						@fCurrentHV,
						@fCurrentVolOfVol,
						@fCurrentVega)
				end
			else
				begin
					update IndexDataHistory
					set 
						currentIV = isnull(@fCurrentIV,currentIV),
						currentHV = isnull(@fCurrentHV,currentHV),
						currentVolOfVol = isnull(@fCurrentVolOfVol,currentVolOfVol),
						currentVega = isnull(@fCurrentIV,currentVega),
						updateDate = GetDate()
					where indexID = @iContractID and tenorID=8 and [date]=(select max([date]) from IndexDataHistory where indexID = @iContractID and tenorID=8)
				end
		end
	else if @iContractTypeID = 2
		begin
			if not exists(select stockID from StockDataHistory where tenorID = 8 and [date]=(select max([date]) from StockDataHistory where stockID = @iContractID and tenorID=8)	and stockID = @iContractID)
				begin
					insert into StockDataHistory (
							stockID,
							tenorID,
							[date],
							IVMean,
							currentHV,
							currentVolOfVol) 
						values(
							@iContractID,
							8,
							GetDate(),
							isnull(@fCurrentIV,0),
							@fCurrentHV,
							@fCurrentVolOfVol)
				end
			else
				begin
					update StockDataHistory
					set 
						IVMean = isnull(@fCurrentIV,IVMean),
						currentHV = isnull(@fCurrentHV,currentHV),
						currentVolOfVol = isnull(@fCurrentVolOfVol,currentVolOfVol),
						updateDate = GetDate()
					where tenorID=8 and [date]=(select max([date]) from StockDataHistory where stockID = @iContractID and tenorID=8) and stockID = @iContractID
				end
	 	end

	set @error = @@error

finish:
Return(@error)

GO

IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Option_Check' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Option_Check
GO

CREATE   PROCEDURE dbo.usp_Option_Check 
	@iOptionID int = null, 
	@vcSymbol varchar(20),
	@iOptionRootID int,
	@dtExpiry smalldatetime,
	@iExpiryID int,
	@fStrike float,
	@iIsCall tinyint,
	@vcUnderlyingSymbol varchar(8),
	@ERR_MSG varchar(8000) = null output,
	@bRemoveOption bit = null   -- call usp_Option_Del for this option
AS
/*
	Created by: Zaur
	Description: this procedure created with template, desined by Zaur Nuraliev
		and check if option with same symbol exists
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iOptionID_bad int,
			@vcOptionRoot_bad varchar(20),
			@dtExpiry_bad smalldatetime,
			@fStrike_bad float,
			@iIsCall_bad tinyint,
			@vcSymbol_bad varchar(20)
	
	select @iOptionID_bad = optionID 
		from OptionView O join Contract C on O.optionID = C.contractID
		where (optionID != @iOptionID or @iOptionID is null) and C.symbol = @vcSymbol and
		(optionRootID != @iOptionRootID or expiryID != @iExpiryID or abs(strike - @fStrike) > 0.009 or isCall != @iIsCall)

--select @iOptionID_bad '@iOptionID_bad'


	if @iOptionID_bad is not null
	begin
		if @bRemoveOption is not null
			begin
				exec @error = usp_Option_Del @iOptionID = @iOptionID_bad,@bisRaiseError = 0
			end
		
		if (@error = -2) or (@bRemoveOption is null)
			begin
				select @vcSymbol_bad = symbol from ContractView where contractID = @iOptionID_bad
				select	@vcOptionRoot_bad = ORT.symbol
						from OptionView O join OptionRootView ORT on ORT.optionRootID = O.optionRootID
						where O.optionID = @iOptionID_bad
				select @dtExpiry_bad = expiryPeriod 
						from ExpiryView E join OptionView O on E.expiryID = O.expiryID 
						where O.optionID = @iOptionID_bad
				select 	@fStrike_bad = strike,
						@iIsCall_bad = isCall 
						from OptionView where optionID = @iOptionID_bad

			set @ERR_MSG = 'Can''t add option with Symbol=' + '''' +  @vcSymbol + ''''  +  
			CASE WHEN left(@vcSymbol,len(@vcSymbol)-2) != left(@vcSymbol_bad,len(@vcSymbol_bad)-2) THEN
				', Option Root=' + '''' + left(@vcSymbol,len(@vcSymbol)-2) + ''''
			ELSE '' END + 
			CASE WHEN @fStrike!=@fStrike_bad THEN
				', Strike=' + CAST(ROUND(@fStrike, 2) as varchar(10)) 
			ELSE '' END + 
			CASE WHEN cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) != cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) THEN
				', Expiry=' + '''' + cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) + '''' 
			ELSE '' END +  
			CASE WHEN @iIsCall!=@iIsCall_bad THEN
				'''' + ', Call\Put' + '''' + '=' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END  
			ELSE '' END +  
			CASE WHEN @error = -2 THEN
				' because exists option with trades and Symbol=' + '''' +  @vcSymbol_bad + ''''
				WHEN @error != -2 THEN
				' because exists option with Symbol=' + '''' +  @vcSymbol_bad + ''''
			ELSE '' END + 
			CASE WHEN left(@vcSymbol,len(@vcSymbol)-2) != left(@vcSymbol_bad,len(@vcSymbol_bad)-2) THEN
				', Option Root=' + '''' + left(@vcSymbol_bad,len(@vcSymbol_bad)-2) + ''''
			ELSE '' END + 
			CASE WHEN @fStrike!=@fStrike_bad THEN
				', Strike=' + CAST(ROUND(@fStrike_bad, 2) as varchar(10)) 
			ELSE '' END + 
			CASE WHEN cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) != cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) THEN
				', Expiry=' + '''' + cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) + ''''
			ELSE '' END + 
			CASE WHEN @iIsCall!=@iIsCall_bad THEN
				'''' + ', Call\Put' + '''' + '=' + CASE @iIsCall_bad WHEN 1 THEN 'Call' ELSE 'Put' END
			ELSE '' END +
			', Underlying Symbol=' + '''' + @vcUnderlyingSymbol + '''' 
--select @ERR_MSG '@ERR_MSG'
		end	
		Return(-1)
	end
	else
		begin
			set @ERR_MSG = null
			set @iOptionID_bad = null
		end

	--===================================================================
	-- update option symbol, if option with same attributes exists
	--===================================================================
	select @iOptionID_bad = optionID 
		from OptionView O join ContractView C on O.optionID = C.contractID
		where (optionID != @iOptionID) and C.symbol != @vcSymbol and
		(optionRootID = @iOptionRootID and expiryID = @iExpiryID and abs(strike - @fStrike) < 0.009 and isCall = @iIsCall)
	if @iOptionID_bad is not null
	begin
		update contract set symbol = @vcSymbol where contractID = @iOptionID_bad
		set @ERR_MSG = '*'
	end

/*	select 	@iOptionID_bad as '@iOptionID_bad',@iOptionID '@iOptionID', 
			@vcSymbol '@vcSymbol',@iOptionRootID '@iOptionRootID',@dtExpiry '@dtExpiry',
			@iExpiryID '@iExpiryID',@fStrike '@fStrike',@iIsCall '@iIsCall',
			@vcUnderlyingSymbol '@vcUnderlyingSymbol'

	select 	@vcOptionRoot_bad '@vcOptionRoot_bad',@dtExpiry_bad '@dtExpiry_bad',
			@fStrike_bad '@fStrike_bad',@iIsCall_bad '@iIsCall_bad', @vcSymbol_bad '@vcSymbol_bad' */


Return(@@error)
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.42.077')
	begin
		update DataInfo set keyValue = '3.42.077' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.42.077' '3.42.077 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
