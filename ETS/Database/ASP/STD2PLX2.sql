SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE function dbo.fn_get_expiry (@dtExpiryOV as datetime )
returns  datetime
begin
	declare @expiry as datetime
	set @expiry = @dtExpiryOV
		
	if DatePart(hh, @dtExpiryOV) > 12 
	begin
		select @expiry = cast(floor(cast(@expiry as float)) + 1 as DateTime)	
	end
	else
	begin
		select @expiry = cast(floor(cast(@expiry as float)) as DateTime)		
	end

	return @expiry
end


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
-------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER       PROC [dbo].[usp_AspEodUpdateMarketStructure_DefaultIVs_Execute]   
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
  
 insert into CustomStrikeSkewPoint (volaSurfaceDataID, type, strike, volatility, isBasePoint, expiryID, expiryInCalendarID)  
 select 
	vsd.volaSurfaceDataID,   
	eod_cssp.type,
	eod_cssp.strike,
	eod_cssp.volatility,
	eod_cssp.isBasePoint,
	ExpCal.expiryID,
	ExpCal.expiryInCalendarID  
 from ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp  
  inner join ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.volaSurfaceDataID = eod_cssp.volaSurfaceDataID  
  inner join EodUnderlyingMap eod_um on eod_um.eodContractID = eod_vsd.contractID  
  inner join VolaSurfaceData vsd on vsd.contractID = eod_um.contractID  
  
  inner join Contract c on c.contractID = vsd.contractID  
  inner join ExpiryInCalendar ExpCal on ExpCal.ExpiryDate = dbo.fn_get_expiry(eod_cssp.ExpiryOV)  
  
  left join CustomStrikeSkewPoint cssp on   
   cssp.volaSurfaceDataID = vsd.volaSurfaceDataID  
   and abs(cssp.strike - eod_cssp.strike) < 0.009  
   and cssp.type = eod_cssp.type  
   and cssp.expiryID =  (year(eod_cssp.ExpiryOV) - 2000) * 12 + month(eod_cssp.ExpiryOV)  
   and cssp.expiryInCalendarID = ExpCal.expiryInCalendarID  
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
 set 	type = eod_cssp.type,
	strike = eod_cssp.strike,
      volatility = eod_cssp.volatility,
	isBasePoint = eod_cssp.isBasePoint,
	expiryID = ExpCal.expiryID,
	expiryInCalendarID = ExpCal.expiryInCalendarID  
 from ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp  
  inner join ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.volaSurfaceDataID = eod_cssp.volaSurfaceDataID  
  inner join EodUnderlyingMap eod_um on eod_um.eodContractID = eod_vsd.contractID  
  inner join VolaSurfaceData vsd on vsd.contractID = eod_um.contractID  
  
  inner join Contract c on c.contractID = vsd.contractID  
  inner join ExpiryInCalendar ExpCal on ExpCal.ExpiryDate = dbo.fn_get_expiry(eod_cssp.ExpiryOV)  
  
  inner join CustomStrikeSkewPoint cssp on   
   cssp.volaSurfaceDataID = vsd.volaSurfaceDataID  
   and abs(cssp.strike - eod_cssp.strike) < 0.009  
   and cssp.type = eod_cssp.type  
   and cssp.expiryID = (year(eod_cssp.ExpiryOV) - 2000) * 12 + month(eod_cssp.ExpiryOV)  
   and cssp.expiryInCalendarID = ExpCal.expiryInCalendarID  
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--------------------------------------------------------------------------------------------------------------------------------------------------


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER PROC [dbo].[usp_AspEodUpdateMarketStructure_ProcessNewFutureOptions] 
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

	insert EodFuture (eodFutureID, futureRootID, symbol, contractName, maturityID, priceClose)
	select
		eod_f.futureID,
		eod_frm.futureRootID,
		eod_c.symbol,
		eod_c.contractName,
--		eod_f.maturityID,
		(year(eod_f.maturityDate) - 2000) * 12 + month(eod_f.maturityDate),
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
	

	declare @eodFutureID_eod int,
		@symbol_eod varchar(20),
		@contractName_eod varchar(255),
		@dtExpiryDate_eod datetime,
		@expiryID_eod int,
		@strike_eod float,
		@isCall_eod tinyint,
		@priceClose_eod float,
		@iExpCalID_eod int

	declare cur_EodFutOption cursor local fast_forward read_only
	for select eod_fo.futureID, eod_c.symbol,	eod_c.contractName, dbo.fn_get_expiry(eod_fo.ExpiryOV),
		(year(eod_fo.ExpiryOV) - 2000) * 12 + month(eod_fo.ExpiryOV),
		eod_fo.strike, eod_fo.isCall, eod_cp.priceClose
	from ETS_ASP_EOD.dbo.FutureOption eod_fo
		inner join ETS_ASP_EOD.dbo.Contract eod_c on eod_c.contractID = eod_fo.futureOptionID

	left join ETS_ASP_EOD.dbo.ContractPrice eod_cp
		on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null

	open cur_EodFutOption

	fetch next from cur_EodFutOption
	into @eodFutureID_eod, @symbol_eod, @contractName_eod, @dtExpiryDate_eod, @expiryID_eod, @strike_eod, @isCall_eod, @priceClose_eod
	
	while @@fetch_status != -1
	begin
		if @@fetch_status != -2
		begin
			-- Insert new date rows for the FutureOption
			exec @iExpCalID_eod = usp_FlexOptionDate_Create @dtExpiryDate_eod 
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			insert EodFutureOption (eodFutureID, symbol, contractName, expiryID, strike, isCall, priceClose)
			values(@eodFutureID_eod, @symbol_eod, @contractName_eod, @expiryID_eod, @strike_eod, @isCall_eod, @priceClose_eod)

			fetch next from cur_EodFutOption
			into @eodFutureID_eod, @symbol_eod, @contractName_eod, @dtExpiryDate_eod, @expiryID_eod, @strike_eod, @isCall_eod, @priceClose_eod
		end
	end
	
	close cur_EodFutOption
	deallocate cur_EodFutOption	

	declare @eodFutureID int,
		@futureRootID int,
		@symbol varchar(20),
		@maturityID int,
		@expiryID int,
		@expCalendarID int,
		@strike float,
		@isCall tinyint,
		@contractName varchar(255),
		@priceClose float,
		@futureOptionID int,
		@futureID int

	declare cur_Future cursor local fast_forward read_only
	for select eodFutureID, futureRootID, symbol, contractName, maturityID, priceClose
		from EodFuture (nolock)

	open cur_Future
	
	fetch next from cur_Future
	into @eodFutureID, @futureRootID, @symbol, @contractName, @maturityID, @priceClose

	while @@fetch_status != -1
	begin
		if @@fetch_status != -2
		begin
	 			exec @futureID = dbo.usp_AspEodUpdateMarketStructure_ImportFuture
							@iFutureRootID = @futureRootID,
							@vcSymbol = @symbol,
							@vcContractName = @contractName,
							@iMaturityID = @maturityID,
							@fPriceClose = @priceClose
	
			if @@error = 0 and isnull(@futureID, 0) > 0
			begin
				declare cur_FutureOption cursor local fast_forward read_only
				for select EIC.expCalendarID, eod_fo.symbol, eod_fo.contractName, eod_fo.expiryID, eod_fo.strike, eod_fo.isCall, eod_fo.priceClose
					from EodFutureOption eod_fo with (nolock, index = IX_EodFutureOption_eodFutureID)
	  				     inner join ETS_ASP_EOD.dbo.[FutureOption] FutOp on FutOp.futureID = eod_fo.eodFutureID
					     inner join ETS_ASP_EOD.dbo.Contract eod_c on eod_c.contractID = FutOp.futureOptionID
					           and eod_c.symbol = eod_fo.symbol COLLATE SQL_Latin1_General_CP1_CI_AS
					     inner join ExpiryInCalendarView EIC on 
			                                           (eod_c.expCalendarID = EIC.expCalendarID and abs(datediff( d, EIC.expiryDate, dbo.fn_get_expiry(FutOp.expiryOV))) < 2)
						or (eod_c.expCalendarID is NULL and EIC.expiryDate = dbo.fn_get_expiry(FutOp.expiryOV))
					where eodFutureID = @eodFutureID

				open cur_FutureOption
	
				fetch next from cur_FutureOption
				into @expCalendarID, @symbol, @contractName, @expiryID, @strike, @isCall, @priceClose
	
				while @@fetch_status != -1
				begin
					if @@fetch_status != -2
					begin
		
						exec @futureOptionID = dbo.usp_AspEodUpdateMarketStructure_ImportFutureOption
											@iFutureID = @futureID,
											@iExpiryID = @expiryID,
											@iexpCalendarID = @expCalendarID,
											@fStrike = @strike,
											@iIsCall = @isCall,
											@vcSymbol = @symbol,
											@vcContractName = @contractName,
											@fPriceClose = @priceClose
		
						fetch next from cur_FutureOption
						into @expCalendarID, @symbol, @contractName, @expiryID, @strike, @isCall, @priceClose
					end
				end
	
				close cur_FutureOption
				deallocate cur_FutureOption
			end
		end

		fetch next from cur_Future
		into @eodFutureID, @futureRootID, @symbol, @contractName, @maturityID, @priceClose
	end

	close cur_Future
	deallocate cur_Future

	if @iWithTran is not null commit tran @vcTranName
finish:
	return @error


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER PROC [dbo].[usp_AspEodUpdateMarketStructure_ProcessNewOptions] 
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
	
-- see cursor using below
--	insert EodOption (eodOptionRootID, symbol, contractName, expiryID, strike, isCall, priceClose)
--	select	eod_o.optionRootID, eod_c.symbol, eod_c.contractName,
--		eod_o.expiryID, (year(eod_o.ExpiryDate) - 2000) * 12 + month(eod_o.ExpiryDate),
--		eod_o.strike, eod_o.isCall, eod_cp.priceClose
--	from ETS_ASP_EOD.dbo.[Option] eod_o
--		inner join ETS_ASP_EOD.dbo.Contract eod_c on eod_c.contractID = eod_o.optionID
--		left join ETS_ASP_EOD.dbo.ContractPrice eod_cp
--			on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null

	declare @OptionRootID_eod int,
		@symbol_eod varchar(20),
		@contractName_eod varchar(255),
		@dtExpiryDate_eod datetime,
		@expiryID_eod int,
		@strike_eod float,
		@isCall_eod tinyint,
		@priceClose_eod float,
		@iExpCalID_eod int

	declare cur_EodOption cursor local fast_forward read_only
	for select eod_o.optionRootID, eod_c.symbol, eod_c.contractName, dbo.fn_get_expiry(eod_o.ExpiryOV),
		(year(eod_o.ExpiryOV) - 2000) * 12 + month(eod_o.ExpiryOV),
		eod_o.strike, eod_o.isCall, eod_cp.priceClose
	from ETS_ASP_EOD.dbo.[Option] eod_o
		inner join ETS_ASP_EOD.dbo.Contract eod_c on eod_c.contractID = eod_o.optionID
		left join ETS_ASP_EOD.dbo.ContractPrice eod_cp
			on eod_cp.contractID = eod_c.contractID and eod_cp.exchangeID is null	

	open cur_EodOption

	fetch next from cur_EodOption
	into @OptionRootID_eod, @symbol_eod, @contractName_eod, @dtExpiryDate_eod, @expiryID_eod, @strike_eod, @isCall_eod, @priceClose_eod
	
	while @@fetch_status != -1
	begin
		if @@fetch_status != -2
		begin
			-- Insert new date rows for the Option
			exec @iExpCalID_eod = usp_FlexOptionDate_Create @dtExpiryDate_eod 
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			insert EodOption (eodOptionRootID, symbol, contractName, expiryID, strike, isCall, priceClose)
			values(@OptionRootID_eod, @symbol_eod, @contractName_eod, @expiryID_eod, @strike_eod, @isCall_eod, @priceClose_eod)

			fetch next from cur_EodOption
			into @OptionRootID_eod, @symbol_eod, @contractName_eod, @dtExpiryDate_eod, @expiryID_eod, @strike_eod, @isCall_eod, @priceClose_eod
		end
	end
	
	close cur_EodOption
	deallocate cur_EodOption

	declare @eodOptionRootID int,
		@underlyingID int,
		@symbol varchar(20),
		@lotSize int,
		@expiryID int,
  		@expCalendarID int,
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
				for select EIC.expCalendarID, eod_o.symbol, eod_o.contractName, eod_o.expiryID, eod_o.strike, eod_o.isCall, eod_o.priceClose
		  	                  from EodOption eod_o with (nolock, index = IX_EodOption_eodOptionRootID)
  				            inner join ETS_ASP_EOD.dbo.[Option] Opti on Opti.OptionRootID = eod_o.eodOptionRootID
				            inner join ETS_ASP_EOD.dbo.Contract eod_c on eod_c.contractID = Opti.optionID
				                    and eod_c.symbol = eod_o.symbol COLLATE SQL_Latin1_General_CP1_CI_AS
				            inner join ExpiryInCalendarView EIC on 
						(eod_c.expCalendarID = EIC.expCalendarID and abs(datediff( d, EIC.expiryDate, dbo.fn_get_expiry(Opti.expiryOV) )) < 2)
					         or (eod_c.expCalendarID is NULL and EIC.expiryDate = dbo.fn_get_expiry(Opti.expiryOV))
				     where eodOptionRootID = @eodOptionRootID

				open cur_Option
	
				fetch next from cur_Option
				into @expCalendarID, @symbol, @contractName, @expiryID, @strike, @isCall, @priceClose
	
				while @@fetch_status != -1
				begin
					if @@fetch_status != -2
					begin
		
						exec @optionID = dbo.usp_AspEodUpdateMarketStructure_ImportOption
											@iRootID = @optionRootID,
											@iExpiryID = @expiryID,
											@iexpCalendarID = @expCalendarID,
											@fStrike = @strike,
											@iIsCall = @isCall,
											@vcSymbol = @symbol,
											@vcContractName = @contractName,
											@fPriceClose = @priceClose
		
						fetch next from cur_Option
						into @expCalendarID, @symbol, @contractName, @expiryID, @strike, @isCall, @priceClose
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

