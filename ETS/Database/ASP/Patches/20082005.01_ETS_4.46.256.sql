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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.46.255' or (keyValue = '4.46.256') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '4.46.256 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '4.46.256' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION SECTION*/

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO







/* ---------------------------- */

ALTER    PROC dbo.usp_AspEodUpdateMarketStructure_ImportOption
	@iRootID int,
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
		
		update Contract set
			symbol = @vcSymbol,
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
		set 	isManualyEntered = 0,
			expiryDate = @dtExpiryDate,
			expiryOV = @dtExpiryOV,
			TradingClose = @dtTradeClose 
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
		values	(3, @vcSymbol, @vcContractName)

		if @@error != 0 or @error != 0
		begin
			exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Importing option: Fail to insert new option contract', @iErrorCode = @@error
			if @iWithTran is not null rollback tran @vcTranName
			set @error = -5
			goto finish
		end

		set @iOptionID = @@identity

		insert into [Option]
			(optionID, optionRootID, isCall, strike, expiryDate, ExpiryOV, TradingClose)
		values	(@iOptionID, @iRootID, @iIsCall, @fStrike, @dtExpiryDate, @dtExpiryOV, @dtTradeClose) 
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

-------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO






/* ---------------------------- */

ALTER   PROC dbo.usp_AspEodUpdateMarketStructure_ImportFutureOption
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

---------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO








ALTER           PROC usp_AspEodUpdateMarketStructure_Main_Execute 
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
  and eod_c.contractTypeID in (1,2,4,6)
  and c.contractTypeID in (1,2,4,6)
 
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
  --inner join EodUnderlyingMap eod_um on eod_um.contractID = fr.underlyingID
  inner join ETS_ASP_EOD.dbo.FutureRoot eod_fr
    on  eod_fr.futureRootSymbol = fr.futureRootSymbol
 
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
-- updated on 4.46.239 version
 Update [Stock] 
	set divAmt = case when eod_i.indexID is null then eod_s.divAmt else eod_i.divAmt end,
	divDate = case when eod_i.indexID is null then eod_s.divDate else eod_i.divDate end, 
	divFreq = case when eod_i.indexID is null then eod_s.divFreq else eod_i.divFreq end
from [Stock] stk
	left join EodUnderlyingMap eod_um on stk.stockID = eod_um.contractID
	left join ETS_ASP_EOD.dbo.[Index] eod_i on eod_i.indexID = eod_um.eodContractID
	left join ETS_ASP_EOD.dbo.[Stock] eod_s on eod_s.stockID = eod_um.eodContractID
where stk.actionID < 3

 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to update stocks info', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end

--addad on version 4.46.239
--index info
update [Index]
	set divAmt = case when eod_i.indexID is null then eod_s.divAmt else eod_i.divAmt end,
	divDate = case when eod_i.indexID is null then eod_s.divDate else eod_i.divDate end, 
	divFreq = case when eod_i.indexID is null then eod_s.divFreq else eod_i.divFreq end
from [Index] idx
	left join EodUnderlyingMap eod_um on idx.indexID = eod_um.contractID
	left join ETS_ASP_EOD.dbo.[Index] eod_i on eod_i.indexID = eod_um.eodContractID
	left join ETS_ASP_EOD.dbo.[Stock] eod_s on eod_s.stockID = eod_um.eodContractID
where idx.actionID < 3

 if @@error != 0
 begin
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating market structure: Fail to update Index info', @iErrorCode = @@error
  set @error = -1
  if @iWithTran is not null rollback tran @vcTranName
  goto finish
 end
 
 -----------------------------------------------------------------------------------------------
 
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
		inner join [StockView] sv on sv.stockID = eod_i.contractID and sv.isHedgeSymbol = 1
	where eod_ib.actionID <3 

 
	insert into IndexBeta (indexID, contractID, beta) 
	select 	eod_i.contractID,
		eod_u.contractID,
		eod_ib.beta
	from ETS_ASP_EOD.dbo.IndexBeta eod_ib
		inner join EodUnderlyingMap eod_i on eod_i.eodContractID = eod_ib.indexID
		inner join EodUnderlyingMap eod_u on eod_u.eodContractID = eod_ib.contractID
		inner join [IndexView] iv on iv.indexID = eod_i.contractID and iv.isHedgeSymbol = 1 	where eod_ib.actionID <3
	
	--Update Self Correlation for Stocks that marked like isHedge
	insert IndexBeta(indexID, ContractID, beta)
	select StockID, StockID, 1
	from Stock where isHedgeSymbol = 1 and actionID < 3

	--Update Self Correlation for Indeces that marked like isHedge
	insert IndexBeta(indexID, ContractID, beta)
	select IndexID, IndexID, 1
	from [Index] where isHedgeSymbol = 1 and actionID < 3
 

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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



ALTER   PROC dbo.usp_AspEodUpdateMarketStructure_ProcessMissedOptions 
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
		--and o.actionDate < uc.MSUpdateDate --uc.actionDate
		and (o.actionDate < uc.MSUpdateDate or abs(datediff(d, uc.MSUpdateDate, GetDate())) <> 0)
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

-----------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO






ALTER    PROC dbo.usp_AspEodUpdateMarketStructure_ProcessMissedFutureOptions 
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
		join Future f (nolock) on f.futureID = fo.futureID
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO






ALTER      PROC [dbo].[usp_AspEodUpdateMarketStructure_DefaultIVs_Execute]
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
  

 insert into CustomStrikeSkewPoint (volaSurfaceDataID, type, strike, volatility, isBasePoint, expiryDate, expiryOV)  
 select vsd.volaSurfaceDataID,  
	eod_cssp.type,
	eod_cssp.strike,
	eod_cssp.volatility,
	eod_cssp.isBasePoint,
	eod_cssp.expiryDate,
	eod_cssp.expiryOV
  
 from ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp  
  inner join ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.volaSurfaceDataID = eod_cssp.volaSurfaceDataID  
  inner join EodUnderlyingMap eod_um on eod_um.eodContractID = eod_vsd.contractID  
  inner join VolaSurfaceData vsd on vsd.contractID = eod_um.contractID  
  left join CustomStrikeSkewPoint cssp on cssp.volaSurfaceDataID = vsd.volaSurfaceDataID
	and abs(cssp.strike - eod_cssp.strike) < 0.009
	and cssp.type = eod_cssp.type
	and abs(datediff( d, cssp.expiryDate, eod_cssp.expiryDate )) < 2
	and cssp.actionID < 3
 where cssp.CustomStrikeSkewPointID is null
	and eod_cssp.actionID < 3
OPTION (MAXDOP 1)
  
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
     volatility = case when isnull(vsd.IsManualVol, 0) = 1 then cssp.Volatility else eod_cssp.volatility end,
     --volatility = eod_cssp.volatility,
     isBasePoint = eod_cssp.isBasePoint,
     expiryDate = eod_cssp.expiryDate,
     expiryOV = eod_cssp.expiryOV
 from ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp  
  inner join ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.volaSurfaceDataID = eod_cssp.volaSurfaceDataID  
  inner join EodUnderlyingMap eod_um on eod_um.eodContractID = eod_vsd.contractID  
  inner join VolaSurfaceData vsd on vsd.contractID = eod_um.contractID  
  inner join CustomStrikeSkewPoint cssp on cssp.volaSurfaceDataID = vsd.volaSurfaceDataID  
	and abs(cssp.strike - eod_cssp.strike) < 0.009  
	and cssp.type = eod_cssp.type  
	and abs(datediff( d, cssp.expiryDate, eod_cssp.expiryDate )) < 2   
	and cssp.actionID < 3  
 where eod_cssp.actionID < 3
	and eod_cssp.volatility > -100.0 --IV's mark 'not calculated'  
	--and ((vsd.IsManualVol is NULL OR vsd.IsManualVol != 1) or (cssp.ExpiryOV is NULL))

  
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
   (not exists(select top 1 1 from optionRoot orr where orr.underlyingID = c.ContractID and orr.ActionId < 3) and 
    not exists(select top 1 1 from futureRoot orr where orr.underlyingID = c.ContractID) )
   and c.ActionId < 3
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
   inner join VolaSurfaceData vsd on vsd.contractID = c.ContractID  
   inner join customstrikeskewpoint csp on vsd.volaSurfaceDataID = csp.volaSurfaceDataID   
  where   
   not exists(select top 1 1 from optionRoot orr where orr.ActionId < 3 and orr.underlyingID = c.ContractID)
   and c.ActionId < 3
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
   and isnull(vsd.isManualVol, 0) <> 1
   and c.ContractID  not in (    
    select  tt.contractID from (  
     select t.contractID, t.symbol, max(t.maxDate) as md from (  
      select  c.contractID, c.symbol, csp.actionDate as maxDate from contract c   
       inner join VolaSurfaceData vsd on vsd.contractID= c.ContractID  
       inner join customstrikeskewpoint csp on vsd.volaSurfaceDataID = csp.volaSurfaceDataID --and csp.expiryID> 66  
      where   
       (exists(select top 1 1 from optionRoot orr where orr.underlyingID = c.ContractID and orr.ActionId < 3) or
        exists(select top 1 1 from futureRoot orr where orr.underlyingID = c.ContractID)  
       )  
       and c.ActionId < 3
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



-------------------------------------------------------------------------------------------------------------------------------------------

/*DATA MODIFICATION COMPLETE*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.46.256')
	begin
		update DataInfo set keyValue = '4.46.256' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 ROLLBACK TRANSACTION
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
	select 'DB VERSION WAS CHANGED TO 4.46.256' '4.46.256 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


