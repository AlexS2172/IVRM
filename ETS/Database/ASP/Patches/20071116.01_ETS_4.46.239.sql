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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.238'  or (keyValue = '3.46.239') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.239 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.239' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
---------------------------------------------------------------------------------------------------------------------------
ALTER     PROCEDURE dbo.usp_EodUnderlying_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@fPriceClose float = null,
        @bSaveClosePrice bit = null,
	@vcExchangeCode varchar(20) = null

as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction

	update Contract
	    set ContractName = @vcContractName,
			MSupdateDate = getDate()
 	where contractID = @iContractID

	if @@error <> 0 
		begin 
			Raiserror ('Error. Can''t update Contract Name!', 16, 1)
			set @error = -1 
			if @@TranCount > 0 Rollback Transaction goto finish 
		end

	delete from TradeLog
	where underlyingID = @iContractID
	if @@error <> 0 
		begin 
			Raiserror ('Error. Can''t delete record from TradeLog!', 16, 1)
			set @error = -1 
			if @@TranCount > 0 Rollback Transaction goto finish 
		end

	--========================================================================
	-- make record with null exchange for this stock
	--========================================================================
	if exists(select 1 from ContractPrice where exchangeID is null and contractID = @iContractID)
	begin
	      if @bSaveClosePrice is not null and  @bSaveClosePrice = 1
              begin		
		update ContractPrice
		set priceClose = @fPriceClose
		where exchangeID is null and contractID = @iContractID
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update close price for default exchange!', 16, 1)
				set @error = -1 
				if @@TranCount > 0 Rollback Transaction goto finish
			end
              end 
	end
	else
	begin
	      if @bSaveClosePrice is not null and  @bSaveClosePrice = 1
		insert into ContractPrice(contractID, exchangeID, priceClose)
			values(@iContractID, null, @fPriceClose)
	      else
		insert into ContractPrice(contractID, exchangeID, priceClose)
			values(@iContractID, null, null)
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t insert default exchange!', 16, 1)
				set @error = -1 
				if @@TranCount > 0 Rollback Transaction goto finish
			end
	end
	--==================================================================================
	-- update DivAmnt & DivFreq values
	--==================================================================================
	declare @exchangeID as int
	if (@bIsStock <> 0 and exists(select 1 from Stock where stockID=@iContractID))
		begin 
			set @exchangeID = null
			if (@vcExchangeCode is not null)
			begin
				set @exchangeID = (select TOP 1 e.exchangeID from Exchange e where @vcExchangeCode = exchangeCode)
			end

			Declare @df smallint
			update Stock 
				set divAmt = @fDivAmt,
					divDate = @dtDivDate, 
					divFreq = @iDivFreq,
					primaryExchangeID = isNull(@exchangeID,primaryExchangeID)
			where stockID = @iContractID
			if (@@error <> 0)
				begin
					Raiserror ('Error. Can''t update Dividends and Primary Exchange!', 16, 1)
					set @error = -1 
					if @@TranCount > 0 Rollback Transaction goto finish 
				end
		end

	--Addad on version 4.46.239
	--market dividends for indices
	--index dividend structure update
	if (exists(select 1 from [Index] where indexID = @iContractID))
	begin
		set @exchangeID = null
		if (@vcExchangeCode is not null)
		begin
			set @exchangeID = (select TOP 1 e.exchangeID from Exchange e where @vcExchangeCode = exchangeCode)
		end

		update [Index] 
		set 	divAmt = @fDivAmt,
			divDate = @dtDivDate, 
			divFreq = @iDivFreq,
			primaryExchangeID = isNull(@exchangeID,primaryExchangeID)
		where indexID = @iContractID
		if (@@error <> 0)
		begin
			Raiserror ('Error. Can''t update Dividends for Index!', 16, 1)
			set @error = -1 
			if @@TranCount > 0 Rollback Transaction goto finish 
		end	
	end

	Commit Transaction
    set @error = 0
finish:
	Return(@error)

GO

--==================================================================
-- load from eod db market dividend for indices
--==================================================================
ALTER          PROC usp_AspEodUpdateMarketStructure_Main_Execute 
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
		inner join [IndexView] iv on iv.indexID = eod_i.contractID and iv.isHedgeSymbol = 1
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--=================================================================================================
--=======================================================================================================================
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



ALTER      proc usp_Stock_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@vcExchangeCode varchar(20) = null,
	@fPriceAsk float = -1, -- fPriceAsk < 0 ????????, ??? ????????? ????????? ????????????
	@fPriceBid float = 0,
	@fPriceOpen float = 0,
	@fPriceClose float = 0,
	@fPriceLow float = 0,
	@fPriceHigh float = 0,
	@iVolume int = 0,
	@iExchangeID int = null  --the ExchangeID special for save data into ContractPrice table
as
/*
	Created by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
		update Contract
		    set ContractName = @vcContractName,
				MSupdateDate = getDate()
	 	where contractID = @iContractID
	
		delete from TradeLog
		where underlyingID = @iContractID

		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update Contract Name!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

		if @fPriceAsk >= 0
			begin
			 if @iExchangeID is null
                            begin	
				update ContractPrice 
					set priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					PriceOpen = @fPriceOpen,
					PriceClose = @fPriceClose,
					PriceLow = @fPriceLow,
					PriceHigh = @fPriceHigh,
					Volume = @iVolume
				where contractID = @iContractID 
			    end
                            else
                            begin	
				update ContractPrice 
					set priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					PriceOpen = @fPriceOpen,
					PriceClose = @fPriceClose,
					PriceLow = @fPriceLow,
					PriceHigh = @fPriceHigh,
					Volume = @iVolume
				where contractID = @iContractID and exchangeID=@iExchangeID
			    end

				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update Contract Price!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
			end

		--========================================================================
		-- make record with null exchange for this stock
		--========================================================================
		if not exists(select * from ContractPrice where exchangeID is null and contractID = @iContractID)
			begin
				insert into ContractPrice(contractID,exchangeID)
					values(@iContractID,null)
				if (@@error <> 0) begin rollback tran return(-1) end           
			end 

		declare @exchangeID as int
		set @exchangeID = null
		if (@vcExchangeCode is not null)
		begin
			set @exchangeID = (select TOP 1 e.exchangeID from Exchange e where @vcExchangeCode = exchangeCode and actionID < 3)
		end		
		--==================================================================================
		-- update DivAmnt & DivFreq values
		--==================================================================================
		if (@bIsStock <> 0 and exists(select * from stock where stockID=@iContractID))
			begin 
				Declare @df smallint
				update Stock 
					set divAmt = @fDivAmt,
						divDate = @dtDivDate, 
						divFreq = @iDivFreq,
						primaryExchangeID = isNull(@exchangeID,primaryExchangeID)
				where stockID = @iContractID
				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update DivAmnt and DivDate and primaryExchangeID!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
			end

		if (exists(select * from [index] where indexID=@iContractID))
			begin 
				update [Index] 
					set divAmt = @fDivAmt,
						divDate = @dtDivDate, 
						divFreq = @iDivFreq,
						primaryExchangeID = isNull(@exchangeID,primaryExchangeID)
				where indexID = @iContractID
				if (@@error <> 0)
					begin
						Raiserror ('Error. Can''t update DivAmnt and DivDate and primaryExchangeID for Index!', 16, 1)
						set @error = -1 
						if @@TranCount = 1 Rollback Transaction goto finish 
					end
			end
	Commit Transaction
    set @error = 0
finish:
	if @error<>0 
		Return(@error)
	else
		Return @iContractID



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--=========================================================================================================================
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO





ALTER       PROCEDURE dbo.usp_ContractType_Change 
	@iContractID int
AS
/*
	Created by: Zaur
	Description: this procedure created with template, desined by Zaur Nuraliev
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iContractTypeID int 
		select @iContractTypeID = contractTypeID from Contract where contractID = @iContractID

	if @iContractTypeID > 2 
		begin
			Raiserror('Can''t perform changes with this contract!',16,1)
		end

	Begin Transaction t1
		if @iContractTypeID = 1 
		--change the contract type from index to stock
			begin
				delete from IndexCorrelation where indexID1 = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from IndexCorrelation where indexID2 = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from IndexDataHistory where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from IndexDefinition where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from IndexBeta where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				delete from SyntheticRootIndexBeta where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
				
				insert into Stock(stockID) values(@iContractID)
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				update 
					Stock
				set
					divAmt = idx.divAmt,
					divFreq = idx.divFreq,
					divDate = idx.divDate,
					divDateCustom = idx.divDateCustom,
					divAmtCustom = idx.divAmtCustom,
					divFreqCustom = idx.divFreqCustom,
					primaryExchangeID = idx.PrimaryExchangeID
				from
					[Index] idx, Stock s
				where
					idx.IndexID = @iContractID And s.StockID = @iContractID

				delete from [Index] where indexID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

			end
		else if @iContractTypeID = 2 
		--change the contract type from stock to index
			begin
				delete from StockDataHistory where stockID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				delete from StockCorrelation where stockID1 = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				delete from StockCorrelation where stockID2  = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			
				delete from IndexDefinition where stockID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end

				insert into [Index](indexID) values(@iContractID)
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
				
				update 
					[Index]
				set
					divAmt = s.divAmt,
					divFreq = s.divFreq,
					divDate = s.DivDate,
					divDateCustom = s.divDateCustom,
					divAmtCustom = s.divAmtCustom,
					divFreqCustom = s.divFreqCustom,
					primaryExchangeID = s.PrimaryExchangeID
				from
					[Index] idx, Stock s
				where
					idx.IndexID = @iContractID And s.StockID = @iContractID

				delete from Stock where stockID = @iContractID
					if @@error != 0 begin set @error = -1 if @@TranCount != 0 Rollback Tran t1 goto finish end
			end
			--change the contractType

			if @iContractTypeID = 1 
				set @iContractTypeID = 2 
			else 
				set @iContractTypeID = 1

			update Contract 
				set contractTypeID = @iContractTypeID 
				where contractID = @iContractID
			if @@error != 0 begin if @@TranCount != 0 Rollback Tran t1 goto finish end

	Commit Transaction t1

    set @error = 0
finish:
Return(@error)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--===============================================================================================================
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO





ALTER           PROC usp_MmUnderlying_Get
	@iTraderID int = null
AS
	set nocount on

	if isnull(@iTraderID, 0) = 0
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
			case when i.indexID is null then s.calcOptionType else i.calcOptionType end as tiCalcOptionType,
			case when i.indexID is null then s.isHTB else i.isHTB end as tiIsHTB,
		
		    i.yield as fYield,
	            i.isBasket as tiIsBasket,
		    case when i.indexID is null then s.divFreq else i.divFreq end as iDivFreq,
		    case when i.indexID is null then s.divDate else i.divDate end as dtDivDate,
		    case when i.indexID is null then s.divAmt else i.divAmt end as fDivAmt,
		
--		    s.isDivCustom as tiIsDivCustom,
		    case when i.indexID is null then s.isDivCustom else i.isDivCustom end as tiIsDivCustom,
		    case when i.indexID is null then s.divFreqCustom else i.divFreqCustom end as iDivFreqCustom,
		    case when i.indexID is null then s.divDateCustom else i.divDateCustom end as dtDivDateCustom,
		    case when i.indexID is null then s.divAmtCustom else i.divAmtCustom end as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,
		    cp.priceTheo as fPriceTheoClose,

			null as iTraderID,

			case when i.indexID is null then s.skew else i.skew end as fSkew,
			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
			case when i.indexID is null then s.isHedgeSymbol else i.isHedgeSymbol end as tiIsHedgeSymbol,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration,
		    i.SOQ As SOQ,
		    	case when i.IndexID is null then isnull(s.primaryExchangeID,0) else isnull(i.primaryExchangeID,0) end as iPrimaryExchangeID,
		    mp.manualPrice as dManualActivePrice,
		    vsd.IsManualVol as fManualVol
		from ContractView c 
		    left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		    left join ManualPrice mp on mp.contractID = c.contractID
		    left join VolaSurfaceData vsd on vsd.contractID = c.contractID 
		where
			c.contractTypeID in (1,2,6) and not c.Symbol is null
	else
		select
		    c.contractID as iContractID,
		    c.contractTypeID as iContractTypeID,
		    c.Symbol as vcSymbol,
			c.expCalendarID as iExpCalendarID,
			case when i.indexID is null then s.calcOptionType else i.calcOptionType end as tiCalcOptionType,
			case when i.indexID is null then s.isHTB else i.isHTB end as tiIsHTB,
		
		    i.yield as fYield,
			i.isBasket as tiIsBasket,
		    case when i.indexID is null then s.divFreq else i.divFreq end as iDivFreq,
		    case when i.indexID is null then s.divDate else i.divDate end as dtDivDate,
		    case when i.indexID is null then s.divAmt else i.divAmt end as fDivAmt,
		
--		    s.isDivCustom as tiIsDivCustom,
			case when i.indexID is null then s.isDivCustom else i.isDivCustom end as tiIsDivCustom,
		    case when i.indexID is null then s.divFreqCustom else i.divFreqCustom end as iDivFreqCustom,
		    case when i.indexID is null then s.divDateCustom else i.divDateCustom end as dtDivDateCustom,
		    case when i.indexID is null then s.divAmtCustom else i.divAmtCustom end as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			tc.traderID as iTraderID,

			case when i.indexID is null then s.skew else i.skew end as fSkew,
			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
			case when i.indexID is null then s.isHedgeSymbol else i.isHedgeSymbol end as tiIsHedgeSymbol,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration,
		    i.SOQ As SOQ,
		    	case when i.IndexID is null then isnull(s.primaryExchangeID,0) else isnull(i.primaryExchangeID,0) end as iPrimaryExchangeID,
		    mp.manualPrice as dManualActivePrice,
		    vsd.IsManualVol as fManualVol
		from ContractView c 
		    left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
		    left join ManualPrice mp on mp.contractID = c.contractID
		    left join VolaSurfaceData vsd on vsd.contractID = c.contractID 
		where
			c.contractTypeID in (1,2,6) and not c.Symbol is null

	return @@error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


--===============================================================================================================
---------------------------------------------------------------------------------------------------------------------------
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.239')
	begin
		update DataInfo set keyValue = '3.46.239' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.239' '3.46.239 update message:' 
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



