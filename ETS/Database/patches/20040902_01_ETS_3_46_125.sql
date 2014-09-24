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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.124' or (keyValue = '3.46.125')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.125 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.125' as  'Version in script' from DataInfo where dataInfoID = 1
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
	   WHERE  name = N'usp_AspEodUpdateMarketStructure_Execute' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_AspEodUpdateMarketStructure_Execute

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   PROC dbo.usp_AspEodUpdateMarketStructure_Execute 
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
		inner join ETS_ASP_EOD.dbo.Contract eod_c
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
	from ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp
		inner join ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.volaSurfaceDataID = eod_cssp.volaSurfaceDataID
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
	from ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp
		inner join ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.volaSurfaceDataID = eod_cssp.volaSurfaceDataID
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


/* RESULTS ANALYZING */
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.125')
	begin
		update DataInfo set keyValue = '3.46.125' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.125' '3.46.125 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
