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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.190'  or (keyValue = '3.46.191') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.191 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.191' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO


ALTER    PROC dbo.usp_AspEodUpdateMarketStructure_Main_Execute 
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
		and eod_c.contractTypeID in (1, 2, 4, 6, 7)
		and c.contractTypeID in (1, 2, 4, 6, 7)

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
				on eod_c.contractID = eod_um.eodContractID and eod_c.contractTypeID in (1, 2, 6, 7)
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
				on eod_c.contractID = eod_um.eodContractID and eod_c.contractTypeID in (1, 2, 6, 7)
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
	from 	ETS_ASP_EOD.dbo.ContractView eod_cv 
		inner join ContractView cv on eod_cv.symbol = cv.symbol and (cv.contractTypeID = eod_cv.contractTypeID)
		inner join ContractPrice cp (nolock) on cv.contractID = cp.contractID
		inner join ETS_ASP_EOD.dbo.ContractPrice eod_cp on eod_cv.contractID = eod_cp.contractID

	-- case when the symbol type in EOD base and client base are differ. Only for stocks and indeces
	update ContractPrice
	set priceClose = eod_cp.priceClose, volume10Day = eod_cp.volume10Day, volume5Expiration = eod_cp.volume5Expiration
	from 	ETS_ASP_EOD.dbo.ContractView eod_cv 
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
	select eod_i.contractID,
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
			from 	ETS_ASP_EOD.dbo.ContractView eod_cv 
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
		from 	ETS_ASP_EOD.dbo.ContractView eod_cv 
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
		from 	ETS_ASP_EOD.dbo.ContractView eod_cv 
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
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.191')
	begin
		update DataInfo set keyValue = '3.46.191' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.191' '3.46.191 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

