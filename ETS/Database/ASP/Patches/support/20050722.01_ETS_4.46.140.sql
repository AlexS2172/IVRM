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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.139'  or (keyValue = '3.46.140') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.140 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.140' as  'Version in script' from DataInfo where dataInfoID = 1
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

ALTER      PROC dbo.usp_AspEodUpdateMarketStructure_DefaultIVs_Execute 
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
	where 
	     eod_cssp.actionID < 3

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
			not exists(select top 1 1 from optionRoot orr where orr.ActionId<3 and orr.underlyingID =c.ContractID)
			and c.ActionId <3
			and  c.contractTypeId <3
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
			and  c.contractTypeId <3
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
			and  c.contractTypeId <3
			and c.ContractID  not in (		
				select  tt.contractID from (
					select t.contractID, t.symbol, max(t.maxDate) as md from (
						select  c.contractID, c.symbol, csp.actionDate as maxDate from contract c 
							inner join VolaSurfaceData vsd on vsd.contractID= c.ContractID
							inner join customstrikeskewpoint csp on vsd.volaSurfaceDataID = csp.volaSurfaceDataID and csp.expiryID> 66
						where 
							exists(select top 1 1 from optionRoot orr where orr.ActionId<3 and orr.underlyingID =c.ContractID)
							and c.ActionId <3
							and  c.contractTypeId <3
						group by  c.ContractID, c.symbol, csp.actionDate) T
					group by t.contractID, t.symbol
				) TT where datediff(d,TT.md, getDate())<>0)
				group by csp.customStrikeSkewPointID
		)


	if @iWithTran is not null commit tran @vcTranName
	
finish:
	return @error


GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[SyntheticUnderlyingParam]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[SyntheticUnderlyingParam]
GO

if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[SyntheticUnderlyingParam]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
 BEGIN
CREATE TABLE [dbo].[SyntheticUnderlyingParam] (
	[syntheticUndParamID] [int] IDENTITY (1, 1) NOT NULL ,
	[syntheticOptRootID] [int] NOT NULL ,
	[underlyingID] [int] NOT NULL ,
	[syntheticCoeff] [float] NOT NULL ,
        [strikeSettlement] [float] not NULL,
	[actionID] [tinyint] NOT NULL CONSTRAINT [DF_SyntheticUnderlyingParam_actionID] DEFAULT (1),
	[actionDate] [datetime] NOT NULL CONSTRAINT [DF_SyntheticUnderlyingParam_actionDate] DEFAULT (getdate()),
	CONSTRAINT [PK_SynteticUnderlyingParam] PRIMARY KEY  CLUSTERED 
	(
		[syntheticUndParamID]
	)  ON [PRIMARY] ,
	CONSTRAINT [FK_SynteticUnderlyingParam_Contract] FOREIGN KEY 
	(
		[underlyingID]
	) REFERENCES [dbo].[Contract] (
		[contractID]
	),
	CONSTRAINT [FK_SyntheticUnderlyingParam_SyntheticOptionRoot] FOREIGN KEY 
	(
		[syntheticOptRootID]
	) REFERENCES [dbo].[SyntheticOptionRoot] (
		[syntheticOptRootID]
	)
) ON [PRIMARY]
END

GO

ALTER   PROCEDURE dbo.usp_SyntheticParam_Save 
	@iRootID int, 
	@iUnderlyingID int,
	@fWeight float = null,
	@fStrikeSettlement float = null
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is save information into SyntheticUnderlyingParam table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iSyntheticOptRootID int
		select @iSyntheticOptRootID = syntheticOptRootID 
			from SyntheticOptionRootView where optionRootID = @iRootID


--	Begin Transaction
		if not exists(select * From SyntheticUnderlyingParamView where syntheticOptRootID = @iSyntheticOptRootID and underlyingID = @iUnderlyingID) 
			begin
				insert into SyntheticUnderlyingParam (
							syntheticOptRootID, 
							underlyingID, 
							SyntheticCoeff,
							strikeSettlement)
					values (@iSyntheticOptRootID,
							@iUnderlyingID,
							@fWeight,
							@fStrikeSettlement)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update SyntheticUnderlyingParam set 
					SyntheticCoeff = isnull(@fWeight, SyntheticCoeff),
					strikeSettlement = isnull(@fStrikeSettlement, strikeSettlement)					
				where syntheticOptRootID = @iSyntheticOptRootID and underlyingID=@iUnderlyingID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
--	Commit Transaction
    set @error = 0
finish:
Return(@error)

GO
ALTER    VIEW dbo.SyntheticUnderlyingParamView
AS
	Select
		syntheticUndParamID, 
		syntheticOptRootID, 
		underlyingID, 
		syntheticCoeff, 
		strikeSettlement,
		actionID, 
		actionDate
		from SyntheticUnderlyingParam
		where actionID < 3


GO

ALTER  PROCEDURE dbo.usp_SyntheticParams_Get 
	@iRootID int,
	@bIsWithPrice bit = 0,
	@bIsWithHV bit = 0
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is getting information from SyntheticUnderlyingParam table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @bIsWithPrice = 0
		select 	
			sor.optionRootID as iRootID,
			sup.underlyingID as iUnderlyingID, 
			sup.SyntheticCoeff as fWeight,
			sup.strikeSettlement as fStrikeSettlement
		from SyntheticUnderlyingParamView as sup
			join SyntheticOptionRootView as sor on sor.syntheticOptRootID = sup.syntheticOptRootID
			join ContractView as c on c.contractID = sup.underlyingID
		where sor.optionRootID = @iRootID
	else
		begin
		
		if	@bIsWithHV = 0
			-- use NULL exchange only
			select 
				sor.optionRootID as iRootID,
				sup.underlyingID as iUnderlyingID, 
				sup.SyntheticCoeff as fWeight,
				sup.strikeSettlement as fStrikeSettlement,
				A.iContractID as iContractID,  
				A.iContractTypeID as iContractTypeID,
				A.vcSymbol as vcSymbol,
				case when isDivCustom = 1 then A.divFreqCustom else A.iDivFreq end iDivFreq,
				case when isDivCustom = 1 then A.divAmtCustom else A.fDivAmt end fDivAmt,
				case when isDivCustom = 1 then A.divDateCustom else A.dtDivDate end dtDivDate,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				A.tiCalcOptionType as tiCalcOptionType,
				A.tiCalcModelType as tiCalcModelType,
				A.tiExpCalendarID as tiExpCalendarID,
				A.tiIsBasket as tiIsBasket
			from SyntheticUnderlyingParamView sup 
				join vALLStockEOD A	on A.iContractID = sup.underlyingID
				join ContractView as c on c.contractID = sup.underlyingID
				join SyntheticOptionRootView as sor	on sor.syntheticOptRootID = sup.syntheticOptRootID
				join ContractPrice CP (nolock)
				on A.iContractID = CP.contractID where CP.exchangeID is null and sor.optionRootID = @iRootID
		else
			begin
			select 
				sor.optionRootID as iRootID,
				sup.underlyingID as iUnderlyingID, 
				sup.SyntheticCoeff as fWeight,
				sup.strikeSettlement as fStrikeSettlement,
				C.ContractID as iContractID,  
				C.ContractTypeID as iContractTypeID,
				C.Symbol as vcSymbol,
				case when isDivCustom = 1 then divFreqCustom else S.divfreq end iDivFreq,
				case when isDivCustom = 1 then divAmtCustom else S.divAmt end fDivAmt,
				case when isDivCustom = 1 then divDateCustom else S.DivDate end dtDivDate,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				S.CalcOptionType as tiCalcOptionType,
				S.CalcModelType as tiCalcModelType,
				C.ExpCalendarID as tiExpCalendarID,
				null as tiIsBasket,
				S2.currentHV as fCurrentHV
			from SyntheticUnderlyingParamView sup 
				join ContractView C on C.contractID = sup.underlyingID
				join StockView S on S.stockID = C.contractID
				join SyntheticOptionRootView as sor on sor.syntheticOptRootID = sup.syntheticOptRootID
				join ContractPrice CP (nolock) on S.stockID = CP.contractID 
				left join (select StockDataHistory.stockID,currentHV,[date] from StockDataHistory 
				join (select stockID, max([date]) as maxDate from StockDataHistory group by stockID) S1
					on S1.stockID=StockDataHistory.stockID and S1.maxDate=StockDataHistory.[date]) S2 
				on S2.stockID=S.stockID where CP.exchangeID is null and sor.optionRootID = @iRootID
			union
			select 
				sor.optionRootID as iRootID,
				sup.underlyingID as iUnderlyingID, 
				sup.SyntheticCoeff as fWeight,
				sup.strikeSettlement as fStrikeSettlement,
				C.ContractID as iContractID,  
				C.ContractTypeID as iContractTypeID,
				C.Symbol as vcSymbol,
				0 as iDivFreq,
				I.yield as fDivAmt,
				null as dtDivDate,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				I.CalcOptionType as tiCalcOptionType,
				I.CalcModelType as tiCalcModelType,
				C.ExpCalendarID as tiExpCalendarID,
				null as tiIsBasket,
				S2.currentHV as fCurrentHV
			from SyntheticUnderlyingParamView sup 
				join ContractView C on C.contractID = sup.underlyingID
				join IndexView I on I.indexID = C.contractID
				join SyntheticOptionRootView as sor on sor.syntheticOptRootID = sup.syntheticOptRootID
				join ContractPrice CP (nolock) on I.indexID = CP.contractID
				left join (select IndexDataHistory.indexID,currentHV,[date] from IndexDataHistory join
					(select indexID, max([date]) as maxDate from IndexDataHistory group by indexID) S1
					on S1.indexID=IndexDataHistory.indexID and S1.maxDate=IndexDataHistory.[date]) S2 
				on S2.indexID=I.indexID where CP.exchangeID is null and sor.optionRootID = @iRootID 
			end
		end
	set @error = @@error
Return(@error)


GO


ALTER  PROC dbo.usp_MmSyntheticRootParams_Get 
	@iOptionRootID int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iOptionRootID is null
		select 	
			ort.underlyingID as iUnderlyingID,
			sor.optionRootID as iOptionRootID,
			sup.underlyingID as iComponentUnderlyingID, 
			sup.SyntheticCoeff as fWeight,
			sup.strikeSettlement as fStrikeSettlement
		from SyntheticUnderlyingParamView as sup
			inner join SyntheticOptionRootView as sor on sor.syntheticOptRootID = sup.syntheticOptRootID
			inner join OptionRootView as ort on ort.optionRootID = sor.optionRootID
	else
		select 	
			ort.underlyingID as iUnderlyingID,
			sor.optionRootID as iOptionRootID,
			sup.underlyingID as iComponentUnderlyingID, 
			sup.SyntheticCoeff as fWeight,
			sup.strikeSettlement as fStrikeSettlement
		from SyntheticUnderlyingParamView as sup
			inner join SyntheticOptionRootView as sor on sor.syntheticOptRootID = sup.syntheticOptRootID
			inner join OptionRootView as ort on ort.optionRootID = sor.optionRootID
		where
			sor.optionRootID = @iOptionRootID

	return @@error

GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_SU_Exec]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_SU_Exec]
GO


CREATE  PROC dbo.usp_Trade_SU_Exec
	@iOptTradeID int,
        @iUnderlyingID int, 
	@iUndQuantity int,
	@fUndPrice float,
	@tiUndIsBuy tinyint,
	@tiUndStatus tinyint,
	@fUndBrokerCommission float,
	@fUndClearingBrokerCommission float,
	@iNewUndSeqNum int = null out,
	@iNewUndTradeID int = null out,
	@dtNewUndTradeDate datetime = null out
as
	set nocount on

	begin tran

	declare @bSucceed bit
		
	set @bSucceed = 1

	declare @iDeltaGMT int
	exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

	select @iNewUndTradeID = max(tradeID) + 1 from Trade
	select @iNewUndSeqNum = isnull(max(seqNum), 0) + 1 from Trade

	if @iNewUndTradeID is null set @iNewUndTradeID = 1
	
	if @dtNewUndTradeDate is null	
	   set @dtNewUndTradeDate = dateadd(minute, @iDeltaGMT, getdate())
	
	insert into Trade(tradeID, actionID, contractID, tradeDate, quantity,
		price, isBuy, traderID, strategyID, brokerID, brokerCommission,
		clearingBrokerID, clearingBrokerCommission, tradedIV, spotReference,
		isPosition, status, execID, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID)
	select @iNewUndTradeID, 1, @iUnderlyingID, @dtNewUndTradeDate, @iUndQuantity, @fUndPrice, @tiUndIsBuy,
		traderID, strategyID, brokerID, @fUndBrokerCommission, clearingBrokerID, @fUndClearingBrokerCommission,
		0, 0, 0, @tiUndStatus, null, @iNewUndSeqNum, mark,
		@iUnderlyingID, null, null, null
	from Trade
		where tradeID = @iOptTradeID and actionID = 1
	
		if @@error <> 0 set @bSucceed = 0

	if @bSucceed = 1
	begin
		commit tran
			return @iNewUndSeqNum
	end
	else  
	begin
		rollback tran
		set @iNewUndSeqNum = null
		set @iNewUndTradeID = null
		set @dtNewUndTradeDate = null
		return 0
	end


GO
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.140')
	begin
		update DataInfo set keyValue = '3.46.140' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.140' '3.46.140 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

