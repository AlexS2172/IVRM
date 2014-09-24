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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.162'  or (keyValue = '3.46.163') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.163 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.163' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
----------------------------------------------------------------

GO
ALTER   PROC dbo.usp_MmOptionByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null set @dtMinExpiry = 0

	select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		ec.expiryDate as dtExpiry,
		o.isCall as tiIsCall,
		ort.isFitEnabled as tiIsFitEnabled,
		ort.lotSize as iLotSize,
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol
	from ContractView uc
		inner join OptionRoot ort on uc.contractID = ort.underlyingID
		inner join OptionView o on o.optionRootID = ort.optionRootID
		inner join ContractView c on o.optionID = c.contractID
		inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
	where uc.contractID = @iUnderlyingID
		and ec.expiryDate >= @dtMinExpiry
	--order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc

	return @@error

GO
drop index Broker.I_BrokerName
go

alter table Broker alter column
  brokerName nvarchar(64) not null
go

create index I_BrokerName on Broker(brokerName)
go

--if exists (select * from dbo.sysindexes where name = N'Contract_CALENDAR' and id = object_id(N'[dbo].[Contract]'))
--drop index [dbo].[Contract].[Contract_CALENDAR]
--GO
--create index Contract_CALENDAR on Contract(contractID,contractTypeID,symbol,expCalendarID,actionID,undPriceProfileID,optPriceProfileID)
--on [Primary]
--go

--if exists (select * from dbo.sysindexes where name = N'Contract_34' and id = object_id(N'[dbo].[Contract]'))
--drop index [dbo].[Contract].[Contract_34]
--go
--create index Contract_34 on Contract(contractID,contractTypeID,actionID,symbol,expCalendarID,undPriceProfileID,optPriceProfileID)
--go

if exists (select * from dbo.sysindexes where name = N'ContractPrice_PriceClose' and id = object_id(N'[dbo].[ContractPrice]'))
drop index [dbo].[ContractPrice].[ContractPrice_PriceClose]
go
create index ContractPrice_PriceClose on ContractPrice(contractID,exchangeID,priceClose)
go
if exists (select * from dbo.sysindexes where name = N'CustomDividend_IDX' and id = object_id(N'[dbo].[CustomDividend]'))
drop index [dbo].[CustomDividend].[CustomDividend_IDX]
go
create clustered index CustomDividend_IDX on CustomDividend(StockID,DivYtes,DivAmnt)
go

if exists (select * from dbo.sysindexes where name = N'CustomStrikeSkewPoint_9' and id = object_id(N'[dbo].[CustomStrikeSkewPoint]'))
drop index [dbo].[CustomStrikeSkewPoint].[CustomStrikeSkewPoint_9]
go
create index CustomStrikeSkewPoint_9 on CustomStrikeSkewPoint(volaSurfaceDataID,strike,customStrikeSkewPointID,type,volatility,expiryID,isBasePoint)
go

if not  exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='SyntheticUnderlyingParam' and COLUMN_NAME='strikeSettlement')
alter table SyntheticUnderlyingParam 
	add strikeSettlement float not null,
 	CONSTRAINT [DF_strikeSettlement] DEFAULT (0) FOR [strikeSettlement]

go

if exists (select * from dbo.sysindexes where name = N'Trade_Strategy' and id = object_id(N'[dbo].[Trade]'))
drop index [dbo].[Trade].[Trade_Strategy]
go
create index Trade_Strategy on Trade(contractID,strategyID)
go

drop index TradeHistory.IX_TradeHistory_BrokerName
go

alter table TradeHistory alter column
  brokerName nvarchar(64)
go

if exists (select * from dbo.sysindexes where name = N'IX_TradeHistory_BrokerName' and id = object_id(N'[dbo].[TradeHistory]'))
drop index [dbo].[TradeHistory].[IX_TradeHistory_BrokerName]
go
create index IX_TradeHistory_BrokerName on TradeHistory(brokerName)
go

if exists (select * from dbo.sysindexes where name = N'TraderContract_52' and id = object_id(N'[dbo].[TraderContract]'))
drop index [dbo].[TraderContract].[TraderContract_52]
go
create index TraderContract_52 on TraderContract(contractID,traderID,actionID)
go

if exists (select * from dbo.sysindexes where name = N'TraderContract31' and id = object_id(N'[dbo].[TraderContract]'))
drop index [dbo].[TraderContract].[TraderContract31]
go
create index TraderContract31 on TraderContract(traderID,contractID,actionID)
go

if exists (select * from dbo.sysindexes where name = N'TraderContract_ContractID' and id = object_id(N'[dbo].[TraderContract]'))
drop index [dbo].[TraderContract].[TraderContract_ContractID]
go
create index TraderContract_ContractID on TraderContract(traderID,contractID,actionID)
go

alter table VolaSurfaceData drop
  constraint FK_VolaSurfaceData_Contract
go

alter table VolaSurfaceData add
  constraint FK_VolaSurfaceData_Contract foreign key(contractID) references Contract(contractID) on delete cascade
go

alter         View OptionRootView
as
	Select 
		optionRootID, 
		underlyingID, 
		symbol, 
		isSynthetic,
		isFitEnabled, 
		lotSize, 
		actionID,
		actionDate
	from optionRoot
	where actionID < 3
go

alter    VIEW SyntheticUnderlyingParamView
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
go
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRPoint_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRPoint_Update]
GO
create PROC usp_IRPoint_Update
	@iPeriodTypeID int = Null,
	@iNum int = Null,
	@fShortRate float = Null,
	@fLongRate float = Null,
	@fNeutralRate float = Null

as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from IRPoint where Num = @iNum and  PeriodTypeID=@iPeriodTypeID )
	begin
		update IRPoint set
			actionID = 1,
			shortRate = isNull(@fShortRate, shortRate),
			longRate = isNull(@fLongRate, longRate),
			neutralRate = isNull(@fNeutralRate, neutralRate)
		where @iNum = num AND @iPeriodTypeID = periodTypeID
	end	
 	else
		begin
			declare @curveID  int
	
			DECLARE curs1 CURSOR FOR
			select curveID
			from IRCurve
			
			OPEN curs1
			FETCH NEXT FROM curs1 
			INTO @curveID
			
			WHILE @@FETCH_STATUS = 0
			BEGIN
				exec dbo.usp_IRPoint_Save null,@curveID,@iPeriodTypeID,@iNum, @fShortRate,0,@fLongRate,@fNeutralRate
				FETCH NEXT FROM curs1 INTO @curveID
			END
		
			CLOSE curs1
			DEALLOCATE curs1

		end
	set @error = @@error
Return(@error)
go

GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRPoints_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRPoints_Import]
GO

create  PROC usp_IRPoints_Import
	@txXmlUpdateData as ntext
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		
	Declare @idoc int
	--Create an internal representation of the XML document.
	Exec sp_xml_preparedocument @idoc OUTPUT, @txXmlUpdateData
	
	-- SELECT statement using OPENXML rowset provider

	update IRPoint set actionID=3

	declare @Num  int,
		@NeutralRate float,
		@ShortRate float,
		@LongRate float

	DECLARE curs CURSOR FOR
	SELECT Num, NeutralRate, ShortRate, LongRate
	FROM       OPENXML (@idoc, '/IRCurve/IRPoint')
			WITH (Num  int,
				NeutralRate float,
				ShortRate float,
				LongRate float)
	
	OPEN curs
	FETCH NEXT FROM curs 
	INTO @Num,
		@NeutralRate,
		@ShortRate,
		@LongRate
	WHILE @@FETCH_STATUS = 0
	BEGIN
		exec dbo.usp_IRPoint_Update 1, @Num, @ShortRate, @LongRate, @NeutralRate 
		FETCH NEXT FROM curs 
		INTO @Num,
			@NeutralRate,
			@ShortRate,
			@LongRate
	END
	CLOSE curs
	DEALLOCATE curs

	--And finaly to release allocated XML datatable - following code must be executed
	Exec sp_xml_removedocument @idoc
	set @error = @@error
Return(@error)
go

alter            Procedure usp_ActiveSurface_Get 
	@vcSymbolName varchar(20) --the contract symbol
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure return default surface identifier
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iContractID int
		select @iContractID = contractID from ContractView where symbol = @vcSymbolName and  contractTypeID < 3

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int 
	set @iVolaSurfaceDataID = 0
		exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	select	@iVolaSurfaceDataID as iVolaSurfaceDataID


	set @error = @@error
Return(@error)
go
alter      PROC usp_AspEodUpdateMarketStructure_DefaultIVs_Execute 
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
go

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoCommodity_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoCommodity_Del]
GO

create  PROC usp_BoCommodity_Del
  @iCID int,
  @tiIsRaiseError tinyint = 1 
as
/*
  Description: Commodity Delete procedure
*/
  ------------------------------------------------
  set nocount on
  ------------------------------------------------
  declare @error int    set @error = 0
  
  if exists (select 1 from Trade where contractID = @iCID)
     return -1

  if exists (select 1 from [Order] where contractID = @iCID)
     return -2


  Begin Transaction CommodityDel

    exec @error = usp_TradeLog_Del @iCID
      if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end


    exec @error = usp_BoFutureRootByUnderlying_Del @iCID, 0
      if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end


    update IndexBeta set actionID=3 where indexID = @iCID
      if @@error != 0 begin set @error = -8 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end


    update ContractInGroup set actionID=3 where contractID = @iCID 
      if @@error != 0 begin set @error = -10 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end

    delete FutureUnderlying where
    iFutureUnderlyingID = @iCID
    if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran CommodityDel goto finish end



    update TraderContract set actionID=3 where contractID = @iCID  
      if @@error <> 0 begin set @error = -20 if @@TranCount != 0 Rollback Transaction CommodityDel goto finish end

    update Contract set actionID=3 where contractID = @iCID
      if @@error != 0 begin set @error = -21 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end
    
    set @error = @@error
  Commit Transaction CommodityDel
finish:
  return @error
go


if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoCommodity_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoCommodity_Get]
GO

create PROC usp_BoCommodity_Get
    @iInterestID int = null
  AS
    -- retrieve future
  
    set nocount on
  
    if @iInterestID is not null
      SELECT  c.tiGreeksCalculationType as GreeksCalcType,
        c.tiActive as tiIsActive, 
        CV.contractTypeID as iContractTypeID,
        CV.symbol as vcSymbol, 
                    CV.contractName as vcContractName, 
        c.iFutureUnderlyingID as iContractID,
        ecv.expCalendarName as expCalendarName,
        c.iFutureUnderlyingTypeID as iFutureUnderlyingTypeID

      FROM         dbo.FutureUnderlying C INNER JOIN
                          dbo.ContractView CV ON C.iFutureUnderlyingID = CV.contractID
        inner join ExpiryCalendarView ecv on cv.expCalendarID = ecv.expCalendarID
      WHERE   CV.ContractID =  @iInterestID And CV.ContractTypeID = 6
      Order by CV.Symbol
  
    else
    
      SELECT  c.tiGreeksCalculationType as GreeksCalcType,
        c.tiActive as tiIsActive, 
        CV.contractTypeID as iContractTypeID,
        CV.symbol as vcSymbol, 
                    CV.contractName as vcContractName, 
        c.iFutureUnderlyingID as iContractID,
        ecv.expCalendarName as expCalendarName,
        c.iFutureUnderlyingTypeID as iFutureUnderlyingTypeID

      FROM         dbo.FutureUnderlying C INNER JOIN
                          dbo.ContractView CV ON C.iFutureUnderlyingID = CV.contractID
        inner join ExpiryCalendarView ecv on cv.expCalendarID = ecv.expCalendarID
      WHERE   CV.ContractTypeID = 6
      Order by CV.Symbol
go

alter  PROC usp_Broker_Save
	@iBrokerID int,
	@vcBrokerName nvarchar(64) = null,
	@vcDescription varchar(120) = null,
	@tiIsClearing tinyint = null,
	@fUndComm float = null,
	@fOptRegComm float = null,
	@fOptSpreadComm float = null,
	@fLowPremComm float = null,
	@tiIsRaiseError tinyint = 1 
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0

	if exists (select 1 from BrokerView where BrokerName = @vcBrokerName and (@iBrokerID <> brokerID or @iBrokerID is null))
	begin
		if isnull(@tiIsRaiseError, 0) <> 0 raiserror ('Broker with this name already exists!', 16, 1)
		set @error = -1
		goto finish
	end   

	if @iBrokerID is not null and @tiIsClearing is not null
	begin
		declare @tiOldIsClearing tinyint

		select @tiOldIsClearing = isnull(isClearing, 0)
		from BrokerView
		where brokerID = @iBrokerID

		if @tiOldIsClearing <> @tiIsClearing
		begin
			if @tiIsClearing = 0
			begin
				if exists (select 1 from Trade where clearingbrokerID = @iBrokerID)
				begin
					if isnull(@tiIsRaiseError, 0) <> 0 raiserror ('Can''t set broker as not clearing broker because he is assigned as clearing broker for some trades or positions!', 16, 1)
					set @error = -2
					goto finish
				end
			end
			else
			begin
				if exists (select 1 from Trade where brokerID = @iBrokerID)
				begin
					if isnull(@tiIsRaiseError, 0) <> 0 raiserror ('Can''t set broker as clearing broker because he is assigned as broker for some trades or positions!', 16, 1)
					set @error = -3
					goto finish
				end
			end
		end
	end

	begin transaction

	if @iBrokerID is null 
	begin
		insert into Broker
			(BrokerName, [Description], isClearing, undComm, optRegComm, optSpreadComm, lowPremComm)
		values
			(@vcBrokerName, @vcDescription, @tiIsClearing, @fUndComm, @fOptRegComm, @fOptSpreadComm, @fLowPremComm)

		if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end
	   
		set @iBrokerID = @@identity
	end
	else
	begin 
		update Broker
		set BrokerName = isnull(@vcBrokerName, BrokerName),
			[Description] = isnull(@vcDescription, [Description]),
			isClearing = isnull(@tiIsClearing, isClearing),
			undComm = isnull(@fUndComm, undComm),
			optRegComm = isnull(@fOptRegComm, optRegComm),
			optSpreadComm = isnull(@fOptSpreadComm, optSpreadComm),
			lowPremComm = isnull(@fLowPremComm, lowPremComm)
		where brokerID = @iBrokerID

		if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end
	end 		 

	commit transaction

finish:
	if @error = 0
		return @iBrokerID
	else
		return @error
go

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_ClosePrice_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_ClosePrice_Save]
GO

create proc usp_Contract_ClosePrice_Save
	  @iContractID int,
	  @dContractClosePrice money	
  as
	if @iContractID is not null
	begin
	  update ContractPrice set priceClose = @dContractClosePrice 
		where ContractID = @iContractID
	  IF @@ROWCOUNT = 0 AND @@ERROR = 0
	  begin
		insert into ContractPrice (contractID,priceClose) values (@iContractID, @dContractClosePrice)
	  end	
	end

	return @@error
go


if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractInGroup_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractInGroup_Import]
GO

create    procedure usp_ContractInGroup_Import  
-- Created by Artem Shylaev Egar T  
-- 2005 - 08 - 24  
-- Modified by Anar Alimov
-- 2005 - 11 - 03
  @vXMLData ntext  
As  
  DECLARE @idoc int  
  EXEC sp_xml_preparedocument @idoc OUTPUT, @vXMLData  
  
  select V_XML.Symbol As SymbolsNotExist  
         from ( SELECT * FROM OPENXML(@idoc, '/Groups/Group',1)   
			 WITH (GroupId [varchar] (255) , Symbol[varchar](255) )      
               ) V_XML  
	 where V_XML.Symbol COLLATE SQL_Latin1_General_CP1_CI_AS not in (
		select symbol from contract where ContractTypeID in (1, 2) and actionID < 3
	 )  
    
  Begin Tran  
  
    update ContractInGroup set actionid = 1 where actionid = 2  
    update ContractGroup set actionid = 1 where actionid = 2  
    delete ContractInGroup where groupid in (
	select groupid from ContractGroup where actionid = 3)  
    delete ContractGroup where actionid = 3  
    delete ContractInGroup where actionid = 3  
    delete ContractInGroup where groupid in ( 
	select CG.groupid from ContractGroup  CG  
		join ( SELECT distinct GroupId FROM OPENXML (@idoc, '/Groups/Group',1)   
						    WITH (GroupId [varchar] (255)
		     )      
                ) V_XML on  V_XML.GroupID =  CG.GroupName COLLATE SQL_Latin1_General_CP1_CI_AS
	)   
  
  
insert into ContractGroup   ( groupName , [description] )  
    select V_XML.GroupId As groupName , '' As [description]  
         from
		( SELECT distinct GroupId FROM OPENXML (@idoc, '/Groups/Group',1)   
					  WITH (GroupId [varchar] (255) )      
		) V_XML  
		where V_XML.GroupId COLLATE SQL_Latin1_General_CP1_CI_AS not in ( 
			select groupName from ContractGroup 
			)   
    if @@Error <> 0  
	Begin
	   rollback  
	   return 1 
	End   
  
 insert into ContractInGroup   ( contractID , groupID )  
      select Cn.ContractID, CG.groupID   
           from 
		( SELECT * FROM OPENXML(@idoc, '/Groups/Group',1)   
			   WITH (GroupId [varchar] (255), Symbol [varchar](255) )      
                ) V_XML  
    	   join ContractGroup CG on CG.groupName = V_XML.GroupId COLLATE SQL_Latin1_General_CP1_CI_AS  
	   join Contract Cn 	 on Cn.symbol = V_XML.Symbol COLLATE SQL_Latin1_General_CP1_CI_AS  
	   where Cn.ContractID not in (
			 select distinct contractID from  ContractInGroup   
			 where groupID = CG.groupID
                         )  
		 and Cn.contractTypeID in (1,2,4)
		 and Cn.actionID<3  

  Commit  
    
  
  EXEC sp_xml_removedocument @idoc   
  return 0
go

alter    Procedure usp_EodDefaultIvPoint_Save 
	@iContractID int,
	@fStrike float,
	@iExpiryID int,
	@fVolatility float
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iCustomStrikeSkewPointID int 
		set @iCustomStrikeSkewPointID = Null

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int 
		set @iVolaSurfaceDataID = 0

	declare @iType int
		set @iType = 2 --OTM_IVTYPE

	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID
	
	-- mark olf strile skew points as deleted
	--update CustomStrikeSkewPoint set actionID = 3
	delete from CustomStrikeSkewPoint
	where 
		volaSurfaceDataID = @iVolaSurfaceDataID and
		actionID < 3 and
		datediff(d, actionDate, getDate())>0


	select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
		from CustomStrikeSkewPoint 
		where volaSurfaceDataID = @iVolaSurfaceDataID 
			and abs(strike - @fStrike) < 0.009
			and type = @iType 
			and expiryID = @iExpiryID 
			and actionID < 3
  
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					isBasePoint,
					expiryID)
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						-1,
						@iExpiryID)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			end
		else
			begin
				update CustomStrikeSkewPoint set 
					volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					type = isnull(@iType,type),
					strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility),
					expiryID = isnull(@iExpiryID,@iExpiryID)
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction

    set @error = 0

finish:
	Return(@error)
go

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFutureRootWOCommodity_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_EodFutureRootWOCommodity_Get]
GO

create  PROC usp_EodFutureRootWOCommodity_Get
	AS
		-- retrieve all future roots
	
		set nocount on
	
		SELECT     fr.underlyingID AS iIndexID, 
			   fr.futureRootID AS iFutureRootID, 
			   fr.futureRootSymbol AS vcFutureRootSymbol, 
		           fr.futureRootName AS vcFutureRootName
		FROM         dbo.FutureRoot fr INNER JOIN
		                      dbo.Contract c ON fr.underlyingID = c.contractID
		WHERE     (c.contractTypeID < 6)
go
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MarketStructureExport]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MarketStructureExport]
GO

create    procedure usp_MarketStructureExport  
-- Created by Victoria Prischepova Egar T  
-- 2005 - 11 - 30  
As  
    
declare @sql varchar(3000),
	@filename varchar(1000)

BEGIN TRAN

select @filename = keyValue from DataInfo where keyName = 'MarketStrucExpFile' 
	
set @sql='BCP  "select uct.symbol as UnderlyingSymbol,uct.symbol as OptionSymbol,ex.expiryDate as ExpiryDate, o.strike as Strike from ETS_ASP_INST.dbo.[option] o inner join  ETS_ASP_INST.dbo.OptionRoot rt on o.optionRootID = rt.optionRootID inner join ETS_ASP_INST.dbo.Contract uct on uct.ContractID = rt.underlyingID inner join ETS_ASP_INST.dbo.Contract oct on oct.ContractID = o.optionID inner join ETS_ASP_INST.dbo.ExpiryInCalendar ex on ex.ExpiryID = o.ExpiryID and ex.expCalendarID = ISNULL(oct.expCalendarID,uct.expCalendarID) where o.actionID<3 order by uct.symbol, oct.symbol,ex.expiryDate,o.strike"	queryout  "' + @filename + '" -e -c -t "\t" -C RAW -T'
exec master..xp_cmdshell @sql 

COMMIT
go


alter PROC usp_EodFuture_Import
	@iFutureRootID int,
	@iExpiryID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iLotSizeID int,
	@iOptionLotSizeID int,
	@tiIsActive tinyint output,
	@vcBadFutureName varchar(255) output
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	declare @error int		set @error = 0

	if not exists(select top 1 1 from ExpiryView where expiryID = isnull(@iExpiryID, 0))
	begin
		raiserror ('Wrong matiruty date.', 16, 1)
		set @error = -100
		goto finish
	end

	declare	@iFutureID int,
			@iUndPriceProfileID int,
			@iOptPriceProfileID int,
			@iFutureID_bad int

	if not exists (select * from tempdb..sysobjects where id=object_id('tempdb..#FutureContractsForDelete'))
		Create table #FutureContractsForDelete(contractID int)				
	if @@error <> 0 begin set @error = -1 goto finish end
	
	--===================================================================			
	-- select the @iFutureID
	--===================================================================			
	select @iFutureID = futureID, @tiIsActive = isActive
			from Future (nolock)
			where futureRootID = @iFutureRootID and 
				maturityID = @iExpiryID

	if @iFutureID is not null
	begin
		Begin Transaction
			select @iFutureID_bad = contractID from ContractView where symbol=@vcSymbol and contractID<>@iFutureID and contractTypeID=4
			if @iFutureID_bad is not null
			begin
				select @vcBadFutureName = c.contractName from ContractView c join
							Trade t on t.contractID=c.contractID where c.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					

				select @vcBadFutureName = fc.contractName from ContractView fc join 
							FutureOption o on fc.contractID = o.futureID join
							Trade t on t.contractID=o.futureoptionID where fc.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					
					
				select @vcBadFutureName = c.contractName from ContractView c join
							[Order] o on o.contractID=c.contractID where c.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
				select @vcBadFutureName = fc.contractName from ContractView fc join 
							FutureOption o on fc.contractID = o.futureID join
							[Order] ord on ord.contractID=o.futureoptionID where fc.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
					
				insert into #FutureContractsForDelete (contractID)  values (@iFutureID_bad)
				if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

				insert into #FutureContractsForDelete 
					select futureoptionID from FutureOption where futureID=@iFutureID_bad
				if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end
				
				exec @error = usp_EodFuture_Del

				if @error <> 0
				begin 
					set @error = -6
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end				
			
			end

			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@iFutureID and actionID<>3
			if @@error <> 0 begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end

			update Future set actionDate = getdate() where futureID=@iFutureID
			if @@error <> 0 begin set @error = -8 if @@TranCount = 1 Rollback Transaction goto finish end

			update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -9 if @@TranCount = 1 Rollback Transaction goto finish end
			
		Commit Transaction
	end
	else
	begin
		Begin Transaction
			select @iFutureID_bad = contractID from ContractView where symbol=@vcSymbol and contractTypeID=4
			if @iFutureID_bad is not null
			begin
				select @vcBadFutureName = c.contractName from ContractView c join
							Trade t on t.contractID=c.contractID where c.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					

				select @vcBadFutureName = fc.contractName from ContractView fc join 
							FutureOption o on fc.contractID = o.futureID join
							Trade t on t.contractID=o.futureoptionID where fc.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end					
					
				select @vcBadFutureName = c.contractName from ContractView c join
							[Order] o on o.contractID=c.contractID where c.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
				select @vcBadFutureName = fc.contractName from ContractView fc join 
							FutureOption o on fc.contractID = o.futureID join
							[Order] ord on ord.contractID=o.futureoptionID where fc.contractID=@iFutureID_bad

				if @vcBadFutureName is not null
					begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
					
					
				insert into #FutureContractsForDelete (contractID)  values (@iFutureID_bad)
				if @@error <> 0 begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end

				insert into #FutureContractsForDelete 
					select futureoptionID from FutureOption where futureID=@iFutureID_bad
				if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end
				
				exec @error = usp_EodFuture_Del

				if @error <> 0
				begin 
					set @error = -6
					if @@TranCount = 1 Rollback Transaction 
					goto finish 
				end				
			
			end
			
			select
				@iUndPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 0

			-- check underlying price profile
			if not exists(select 1 from PriceProfileView where priceProfileID = @iUndPriceProfileID)
			begin set @error = -10 if @@TranCount = 1 Rollback Transaction goto finish end
		
			select
				@iOptPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 1

			-- check options price profile
			if not exists(select 1 from PriceProfileView where priceProfileID = @iOptPriceProfileID)
			begin set @error = -11 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into Contract
				(contractTypeID, symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
			values 
				(4, @vcSymbol, @vcContractName, 1, @iUndPriceProfileID, @iOptPriceProfileID)

			set @iFutureID = @@identity
			if @@error <> 0 begin set @error = -12 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into Future (futureID, futureRootID, maturityID)
				 values (@iFutureID, @iFutureRootID, @iExpiryID)
			if @@error <> 0 begin set @error = -13 if @@TranCount = 1 Rollback Transaction goto finish end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iFutureID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@iFutureID,null)
					if (@@error <> 0) begin set @error = -14 if @@TranCount = 1 Rollback Transaction goto finish end
				end 

			update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -15 if @@TranCount = 1 Rollback Transaction goto finish end
			
			set @tiIsActive = 1

		Commit Transaction
	end 
	 
   	set @error = @iFutureID
finish:
	Return(@error)
go


alter   Procedure usp_IRPoint_Save
	@iIRpointID int = Null,
	@iCurveID int = Null,
	@iPeriodTypeID int = Null,
	@iNum int = Null,
	@fShortRate float = Null,
	@fHTBRate float = Null,
	@fLongRate float = Null,
	@fNeutralRate float = Null

as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from IRPoint where Num = @iNum and  PeriodTypeID=@iPeriodTypeID and actionID < 3 and (@iIRpointID <> IRpointID or @iIRpointID is null))
		begin
			Raiserror ('IRPoint with this period already exists!', 16, 1)
			Return (-1)
		end   
	if @iCurveID is not Null and @iIRpointID is Null
		begin
			insert into IRPoint
					(
					curveID,
					periodTypeID,
					num,
					shortRate,
					HTBRate,
					longRate,
					neutralRate
					) 
				values (
					@iCurveID,
					@iPeriodTypeID,
					@iNum,
					@fShortRate,
					@fHTBRate,
					@fLongRate,
					isNull(@fNeutralRate, (@fShortRate+@fLongRate)/2)
										)
			select @iIRpointID = @@identity
		end
	else if @iIRpointID is not Null
		begin
			update IRPoint set
				curveID = isNull(@iCurveID, curveID),
				periodTypeID = isNull(@iPeriodTypeID, periodTypeID),
				num = isNull(@iNum, num),
				shortRate = isNull(@fShortRate, shortRate),
				HTBRate = isNull(@fHTBRate, HTBRate),
				longRate = isNull(@fLongRate, longRate),
				neutralRate = isNull(@fNeutralRate, neutralRate)
			where IRpointID = @iIRpointID
		end
	set @error = @@error
	if (@error = 0) set @error = @iCurveID
Return(@error)
go


alter  PROC usp_MmOptionRoot_Get
	@iUnderlyingID int = null
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iUnderlyingID is null
		select
			orv.underlyingID as iUnderlyingID,
			orv.optionRootID as iRootID,
			orv.symbol as vcSymbol,
			orv.isFitEnabled as tiIsFitEnabled,
			orv.IsSynthetic as tiIsSynthetic, 
			orv.lotSize as iLotSize, 
			sor.cashValue as fCash,
			sor.synthSkew as fSyntheticSkew, 
			sor.synthKurt as fSyntheticKurt,
			sor.synthYield as fSyntheticYield, 
			sor.synthIsBasket as tiSyntheticIsBasket
		from OptionRootView orv
			left join SyntheticOptionRootView sor on orv.optionRootID = sor.optionRootID
	else
		select
			orv.underlyingID as iUnderlyingID,
			orv.optionRootID as iRootID,
			orv.symbol as vcSymbol,
			orv.isFitEnabled as tiIsFitEnabled,
			orv.IsSynthetic as tiIsSynthetic, 
			orv.lotSize as iLotSize, 
			sor.cashValue as fCash,
			sor.synthSkew as fSyntheticSkew, 
			sor.synthKurt as fSyntheticKurt,
			sor.synthYield as fSyntheticYield, 
			sor.synthIsBasket as tiSyntheticIsBasket
		from OptionRootView orv
			left join SyntheticOptionRootView sor on orv.optionRootID = sor.optionRootID
		where
			orv.underlyingID = @iUnderlyingID
	
	return @@error
go

alter  PROC usp_MmSyntheticRootParams_Get 
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
go

alter PROC usp_MmTradeByTrader_Get
	@iTraderID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @iTraderID is null
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID

		from TradeView tr 
			-- contract and underlying contract
			inner join ContractView c on tr.contractID = c.contractID
			inner join ContractView uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
		order by TradeDate
	else
		select
			tr.tradeID as iTradeID,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.traderID as iTraderID,
			tr.strategyID as iStrategyID, 
			tr.brokerID as iBrokerID,
			tr.brokerCommission as fBrokerCommission,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			tr.isPosition as tiIsPosition,
			tr.status as tiStatus,
			tr.execID as vcExecID,
			tr.mark as iMark,

			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			c.contractName as vcContractName,
			c.exportSymbol as vcImportID,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,

			uc.contractID as iUnderlyingContractID,

			case when c.contractTypeID = 3 then ec.expiryDate else fec.expiryDate end as dtExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
			
			ort.optionRootID as iOptionRootID,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID

		from TradeView tr 
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			-- regular option
			left join [Option] o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
		where
			tr.traderID = @iTraderID
		order by TradeDate

	return @@error
go

alter  PROC usp_MmUnderlying_Get
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
		    s.divFreq as iDivFreq,
		    s.divDate as dtDivDate,
		    s.divAmt as fDivAmt,
		
		    s.isDivCustom as tiIsDivCustom,
		    s.divFreqCustom as iDivFreqCustom,
		    s.divDateCustom as dtDivDateCustom,
		    s.divAmtCustom as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,

			null as iTraderID,

			case when i.indexID is null then s.skew else i.skew end as fSkew,
			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration

		from ContractView c 
		    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		where
			c.contractTypeID < 3
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
		    s.divFreq as iDivFreq,
		    s.divDate as dtDivDate,
		    s.divAmt as fDivAmt,
		
		    s.isDivCustom as tiIsDivCustom,
		    s.divFreqCustom as iDivFreqCustom,
		    s.divDateCustom as dtDivDateCustom,
		    s.divAmtCustom as fDivAmtCustom,
		
		    cp.priceClose as fPriceClose,

			tc.traderID as iTraderID,

			case when i.indexID is null then s.skew else i.skew end as fSkew,
			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    cp.volume10Day as iVolume10Day,
		    cp.volume5Expiration as iVolume5Expiration

		from ContractView c 
		    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
		where
			c.contractTypeID < 3

	return @@error
go


alter PROCEDURE usp_OptionRootMove_Save
	@iOldRootID int, 
	@iNewRootID int
AS

	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int,
			@iUnderlyingID int
	set @error = -1

	select @iUnderlyingID = underlyingID from OptionRootView where optionRootID = @iOldRootID

	if exists(select * From OptionRootView where optionRootID = @iNewRootID and underlyingID <> @iUnderlyingID)
		begin
			Raiserror('Can''t move roots options to different underlying. To perform this use Corporate action!',16,1)
			Return(-1)
		end 

	if exists(select O1.optionID From OptionView O1 
					join OptionView O2 
						on O1.isCall = O2.isCall 
						and abs(O1.strike - O2.strike) < 0.009
						and O1.expiryID = O2.expiryID
					where O1.optionRootID = @iNewRootID and O2.optionRootID = @iOldRootID)
		begin
			select C1.symbol as vcOptSymbol1, C2.symbol as vcOptSymbol2 From ContractView C1 
					join OptionView O1 on O1.optionID = C1.contractID
					join OptionView O2  
						on O1.isCall = O2.isCall 
						and abs(O1.strike - O2.strike) < 0.009
						and O1.expiryID = O2.expiryID
					join ContractView C2 on O2.optionID = C2.contractID 
					where O1.optionRootID = @iNewRootID and O2.optionRootID = @iOldRootID
			Return(1)
		end 

	Begin Transaction
		update OptionRoot set 
			actionID = 3
		where optionRootID = @iOldRootID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

		update Contract
			set symbol = ORT.symbol + right(C.symbol,2)
			from Contract C join OptionView O on C.contractID=O.optionID
				join OptionRootView ORT on ORT.optionRootID=@iNewRootID
				where O.optionRootID = @iOldRootID and C.actionID < 3

		update [Option]
			set optionRootID = @iNewRootID
				where optionRootID = @iOldRootID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

		update SyntheticUnderlyingParam
			set actionID = 3 
			from SyntheticUnderlyingParam join SyntheticOptionRootView 
			on SyntheticOptionRootView.syntheticOptRootID = SyntheticUnderlyingParam.syntheticOptRootID
			where SyntheticOptionRootView.optionRootID = @iOldRootID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

		update SyntheticRootIndexBeta
			set actionID = 3 
			from SyntheticRootIndexBeta join SyntheticOptionRootView 
			on SyntheticOptionRootView.syntheticOptRootID = SyntheticRootIndexBeta.syntheticOptRootID
			where SyntheticOptionRootView.optionRootID = @iOldRootID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

		update SyntheticOptionRoot set actionID = 3 where optionRootID = @iOldRootID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

		update Trade set optionRootID = @iNewRootID where optionRootID = @iOldRootID
		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	Commit Transaction

	select null as vcOptSymbol1, null as vcOptSymbol2 where 0 = 1
    set @error = 0
finish:
Return(@error)
go

alter        PROCEDURE usp_OptionRoot_Get 
	@iRootID int = null,
	@iUnderlyingID int = null
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is getting information from OptionRoot table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select orv.optionRootID as iRootID, orv.underlyingID as iUnderlyingID,
		orv.symbol as vcSymbol,
		orv.IsSynthetic as tiIsSynthetic, 
		orv.lotSize as iLotSize, 
		orv.isFitEnabled as tiIsFitEnabled,
		sor.cashValue as fCash,
		sor.synthSkew as fSyntheticSkew, 
		sor.synthKurt as fSyntheticKurt,
		sor.synthYield as fSyntheticYield, 
		sor.synthIsBasket as tiSyntheticIsBasket
		from OptionRootView as orv
		left join SyntheticOptionRootView as sor 
		on orv.optionRootID = sor.optionRootID
		where	(orv.underlyingID = @iUnderlyingID or @iUnderlyingID is null)
			and (orv.optionRootID=@iRootID or @iRootID is null)
	
	set @error = @@error
Return(@error)
go

alter                PROCEDURE usp_OptionRoot_Save 
	@iRootID int, 
	@vcSymbol varchar(20) = null,
	@iLotSize int = null,
	@tiIsFitEnabled tinyint = NULL,
	@tiIsSynthetic tinyint = NULL,
	@fCashValue 	float = null,
	@fSyntheticSkew float = null,
	@fSyntheticKurt float = null,
	@fSyntheticYield float = null,
	@tiSyntheticIsBasket tinyint = null
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is save information into OptionRoot table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if not exists(select * From OptionRootView where optionRootID = @iRootID) 
		begin
			Raiserror('OptionRoot with this RootID didn''t exists in DB!',16,1)
			Return(-1)
		end 

	if @vcSymbol is not null
		begin
			if exists(select * From OptionRootView where optionRootID <> @iRootID and symbol = @vcSymbol)
			begin
				Raiserror('OptionRoot with this symbol already exists in DB!',16,1)
				Return(-1)
			end 
		end

	Begin Transaction
		update OptionRoot set 
			symbol = isnull(@vcSymbol, symbol),
			lotSize = isnull(@iLotSize, lotSize),
			isFitEnabled =  isnull(@tiIsFitEnabled, isFitEnabled),
			isSynthetic = isnull(@tiIsSynthetic, isSynthetic)
		where optionRootID = @iRootID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

		if @vcSymbol is not null
		begin
			update Contract
				set symbol = @vcSymbol + right(C.symbol,2)
				from Contract C join OptionView O on C.contractID=O.optionID
					where O.optionRootID = @iRootID and C.actionID < 3
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end

	if @tiIsSynthetic is not null
        begin
		if @tiIsSynthetic = 1
			begin
				if not exists(select * From SyntheticOptionRootView where optionRootID = @iRootID)
					insert into SyntheticOptionRoot (optionRootID,cashValue,synthSkew,synthKurt,synthYield,synthIsBasket)
					values (
						@iRootID,
						@fCashValue,
						@fSyntheticSkew,
						@fSyntheticKurt,
						@fSyntheticYield,
						@tiSyntheticIsBasket)
	
		   			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				else
					update SyntheticOptionRoot set
						cashValue = isnull(@fCashValue, cashValue),
						synthSkew = isnull(@fSyntheticSkew, synthSkew),
						synthKurt = isnull(@fSyntheticKurt, synthKurt),
						synthYield = isnull(@fSyntheticYield, synthYield),
						synthIsBasket = isnull(@tiSyntheticIsBasket, synthIsBasket)
					where optionRootID = @iRootID		

		   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update SyntheticUnderlyingParam
					set actionID = 3 
					from SyntheticUnderlyingParam join SyntheticOptionRootView 
					on SyntheticOptionRootView.syntheticOptRootID = SyntheticUnderlyingParam.syntheticOptRootID
					where SyntheticOptionRootView.optionRootID = @iRootID

				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

				update SyntheticRootIndexBeta
					set actionID = 3 
					from SyntheticRootIndexBeta join SyntheticOptionRootView 
					on SyntheticOptionRootView.syntheticOptRootID = SyntheticRootIndexBeta.syntheticOptRootID
					where SyntheticOptionRootView.optionRootID = @iRootID

				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

				update SyntheticOptionRoot set actionID = 3 where optionRootID = @iRootID
		   		
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
        end
	Commit Transaction
    set @error = 0
finish:
Return(@error)
go


alter    Procedure usp_OptionVM_Get
	@iOptionID int = null,			-- option's identifier
	@iUnderlyingID int,				-- underlying's identifier
	@dtExpDateMin datetime = null,	-- expiration's date
	@bIsCall bit = null,
	@iSortOrder int = 1	--			-- 1, 2 or 3. see comments below
as
/*
	Created by Andrey Sinkin
	This procedure returns option's data
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iSortOrder = 1	-- sort by symbol
		select	O.OptionID as iOptionID, 
				O.optionRootID as iOptionRootID, 
				ORt.underlyingID as iUnderlyingID,
				isCall as bIsCall, 
				strike as fStrike, 
				EC.expiryDate as  dtExpDate,
				C.symbol as vcSymbolName, 
				null as iExchangeID,
				C.contractName as vcDescription,
				ORt.isFitEnabled as tiIsFitEnabled,
				ORt.lotSize as iLotSize,
				ORt.isSynthetic as tiIsSynthetic 
				from OptionView O
						join ContractView C on O.OptionID = C.contractID
						join ContractPrice CP on CP.contractID = C.contractID and exchangeID is null
						join OptionRoot ORt on O.optionRootID = ORt.optionRootID
						join Contract C1 on ORt.underlyingID = C1.contractID
						join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
				where ORt.underlyingID = @iUnderlyingID
						and (@iOptionID is null or O.OptionID = @iOptionID)
						and (@dtExpDateMin is null or EC.expiryDate >= @dtExpDateMin)
						and (@bIsCall is null or O.isCall = @bIsCall)
				order by C.symbol
	else
	if @iSortOrder = 2	-- sort by isCall+year(expiry)+month(expiry)+strike

		select	O.OptionID as iOptionID, 
				O.optionRootID as iOptionRootID, 
				ORt.underlyingID as iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				EC.expiryDate as dtExpDate,
				C.symbol as vcSymbolName, 
				null iExchangeID,
				C.contractName as vcDescription, 
				ORt.isFitEnabled as tiIsFitEnabled,
				ORt.lotSize as iLotSize,
				ORt.isSynthetic as tiIsSynthetic
			from OptionView O
					join ContractView C on O.OptionID = C.contractID
					join ContractPrice on ContractPrice.contractID = C.contractID and exchangeID is null
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract C1 on ORt.underlyingID = C1.contractID
					join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where ORt.underlyingID = @iUnderlyingID
					and (@iOptionID is null or O.OptionID = @iOptionID)
					and (@dtExpDateMin is null or EC.expiryDate >= @dtExpDateMin)
					and (@bIsCall is null or O.isCall = @bIsCall)
			order by isCall, year(EC.expiryDate),  month(EC.expiryDate), strike
	else
	if @iSortOrder = 3	-- sort by expiry+strike+symbol(without two last symbols)+isCall
		select 	O.OptionID iOptionID,
				O.optionRootID as iOptionRootID, 
				ORt.underlyingID iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				EC.expiryDate as dtExpDate,
				C.symbol vcSymbolName, 
				null iExchangeID,
				C.contractName vcDescription,	
				ORt.isFitEnabled as tiIsFitEnabled,
				ORt.lotSize iLotSize,
				ORt.isSynthetic as tiIsSynthetic 
			from OptionView O
					join ContractView C on O.OptionID = C.contractID
					join ContractPrice on ContractPrice.contractID = C.contractID and exchangeID is null
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract C1 on ORt.underlyingID = C1.contractID
					join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = C1.expCalendarID
			where ORt.underlyingID = @iUnderlyingID
					and (@iOptionID is null or EC.expiryDate = @iOptionID)
					and (@dtExpDateMin is null or EC.expiryDate >= @dtExpDateMin)
					and (@bIsCall is null or O.isCall = @bIsCall)
			order by EC.expiryDate, strike, left(C.symbol, len(C.symbol) - 2), isCall desc
	set @error = @@error
Return(@error)
go

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Bulk_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Bulk_Import]
GO

create PROCEDURE usp_Option_Bulk_Import
	@iUnderlyingContractID int,
	@iBulkXMLData ntext
as

	DECLARE @vcSymbol varchar(20)
	DECLARE @iIsCall  tinyint
	DECLARE @fStrike  float
	DECLARE @dtExpiry datetime
	DECLARE @iLotSize int
	DECLARE @iDoc 	  int
	DECLARE @fClosePrice float
	DECLARE	@vcUnderlyingSymbol varchar(8)
	DECLARE @isError  tinyint
	DECLARE @ERR_MSG  varchar(8000)
   	
	select @vcUnderlyingSymbol = symbol from ContractView where contractID = @iUnderlyingContractID
	set @isError = 0
	exec sp_xml_preparedocument @iDoc OUTPUT, @iBulkXMLData
	DECLARE BulkDataCursor CURSOR FOR
		select * 
			FROM OPENXML(@iDoc, '/Import/Option')
 				WITH (Symbol	  varchar(20),
				      IsCall      tinyint ,    	
				      Strike      float ,    	
               			      Expiry	  datetime ,
				      LotSize     int,
				      ClosePrice  float )

	
	OPEN BulkDataCursor
	FETCH NEXT FROM BulkDataCursor 
	INTO  @vcSymbol, @iIsCall, @fStrike, @dtExpiry,@iLotSize,@fClosePrice

	WHILE @@FETCH_STATUS = 0
	BEGIN
		declare @iRet int
		--select @iUnderlyingContractID, @vcSymbol, @iIsCall, @fStrike, @dtExpiry,@iLotSize
		exec @iRet = usp_Option_Import @iUnderlyingContractID, @vcSymbol, @iIsCall, @fStrike, @dtExpiry,@iLotSize, NULL, @vcUnderlyingSymbol, @fClosePrice
		if @iRet <> 0 
		begin 
		   set @isError = -1	
		   exec usp_EodMsOptionCache_Save @iUnderlyingContractID, @vcSymbol, @dtExpiry, @fStrike, @iIsCall, @iLotSize
		end

		FETCH NEXT FROM BulkDataCursor 
		INTO @vcSymbol, @iIsCall, @fStrike, @dtExpiry,@iLotSize,@fClosePrice
	END


	CLOSE      BulkDataCursor
	DEALLOCATE BulkDataCursor

	exec sp_xml_removedocument @iDoc
	return @isError
go




alter  PROCEDURE usp_Option_Import
	@iUnderlyingContractID int,
	@vcSymbol varchar(20),
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
	@iLotSize int,
	@iMsCacheOptionID int = NULL,
	@vcSymbolUnderlying varchar(20) = NULL,
	@fPriceClose float = NULL
as

/*

	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare	@OptionID int, 
			@cn varchar(255),
			@iExpiryID int,
			@iLastExpiryID int,
			@vcUnderlyingSymbol varchar(8),
			@ERR_MSG varchar(8000)

	if @vcSymbolUnderlying is null
      		select @vcUnderlyingSymbol = symbol from ContractView where contractID = @iUnderlyingContractID
	else
		set  @vcUnderlyingSymbol = @vcSymbolUnderlying

	--===================================================================			
	-- check the Expiry
	--===================================================================			
	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	if not exists(select * from Expiry where expiryID = @iExpiryID)
		begin
			set @ERR_MSG = 'Incorrect expiration! '  + '''' +  Convert(varchar(50),@dtExpiry) + '''' + ' for symbol=' + '''' + @vcSymbol + '''' +  ' UnderlyingSymbol=' + '''' + @vcUnderlyingSymbol + '''' + ' Strike=' + Convert(varchar(15),@fStrike) + ' expiryID=' + Convert(varchar(15),@iExpiryID)
			Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end

	--===================================================================			
	-- check symbol of option root
	--===================================================================			  
   DECLARE @vcOptionRootSymbol varchar(5)	
   set 	@vcOptionRootSymbol = left(@vcSymbol,len(@vcSymbol)-2)

   if exists (select optionRootID 
				   from OptionRootView
				   where symbol = @vcOptionRootSymbol and 
					underlyingID<>@iUnderlyingContractID)
		begin			
			Declare @vcDifferentUndSymbol varchar(8)
			select @vcDifferentUndSymbol = C.symbol from ContractView C join
					OptionRootView ort on ort.underlyingID = C.contractID
					where ort.symbol = @vcOptionRootSymbol and
					ort.underlyingID<>@iUnderlyingContractID
					
			set @ERR_MSG = 'Incorrect option symbol! '  + '''' +  @vcSymbol + ''''  +  ': Option Root=' + '''' + @vcOptionRootSymbol + '''' + ' assign to different underlying=' + '''' +  @vcDifferentUndSymbol + '''' + '. Please, rename Option Root and restart importing of market structure.'
			Raiserror(@ERR_MSG,16,1)
			RETURN (-1)
		end  


	select top 1 @iLastExpiryID = expiryID from ExpiryInCalendarView where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	--===================================================================			
	-- select the @iOptionRootID
	--===================================================================			
	declare @iOptionRootID int,
			@vcOptionRoot varchar(20)
	select @iOptionRootID = optionRootID from OptionRootView where symbol = @vcOptionRootSymbol
			and underlyingID = @iUnderlyingContractID 

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if @iOptionRootID is not null 
		select @OptionID = optionID
			from [Option] (nolock)
			where optionRootID = @iOptionRootID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	--===================================================================			
	-- check if option with same symbol exists
	--===================================================================		
	set @ERR_MSG = null

	execute usp_Option_Check @iOptionID = @OptionID, @vcSymbol = @vcSymbol,	@iOptionRootID = @iOptionRootID,
			@dtExpiry = @dtExpiry,@iExpiryID = @iExpiryID,@fStrike = @fStrike,@iIsCall = @iIsCall,
			@vcUnderlyingSymbol = @vcUnderlyingSymbol,@bRemoveOption = 1,@ERR_MSG = @ERR_MSG output

	if @ERR_MSG is not null and @ERR_MSG != ''
	begin
		if @ERR_MSG != '*' Raiserror(@ERR_MSG,16,1)
		Return(-1)
	end

	--===================================================================			
	-- make the contract name
	--===================================================================			
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(month, @dtExpiry) + 
					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)


  	if (@OptionID is Null) 
	begin
		Begin Transaction

			if  @iOptionRootID is null
			begin
				insert into OptionRoot (underlyingID,symbol,lotSize)
					select @iUnderlyingContractID, @vcOptionRootSymbol, isnull(@iLotSize,100)
				set @iOptionRootID = @@identity
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end 
	
			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@cn)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				 values (@OptionID, @iOptionRootID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			DECLARE @error_var int, @rowcount_var int
			UPDATE ContractPrice 
				SET priceClose = isnull(@fPriceClose,priceClose)
			WHERE exchangeID is null and contractID = @OptionID

			SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
			if @error_var = 0
			begin
			  if @rowcount_var = 0
			  begin
				insert into ContractPrice(contractID,exchangeID,priceClose)
					values(@OptionID, null, @fPriceClose)
				if (@@error <> 0) begin rollback tran return(-1) end           
			  end	
			end
                        else
			 if (@@error <> 0) begin rollback tran return(-1) end           



			if @iMsCacheOptionID is null
			begin
				delete from MsOptionCache
				where (
					symbol = @vcSymbol 
					and underlyingID = @iUnderlyingContractID 
					and isCall = @iIsCall 
					and abs(strike - @fStrike) < 0.009
					and expiryDate = @dtExpiry )
			end
			else
			begin
				delete from MsOptionCache
				where optionID = @iMsCacheOptionID
			end

		Commit Transaction
	end 
 	else
	begin
		update Contract
			set symbol = @vcSymbol,
					 contractName = @cn
			where contractID = @OptionID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract!', 16, 1)
				  RETURN (-1)
			   end 

		if @iMsCacheOptionID is null
		begin
			delete from MsOptionCache
			where (
				symbol = @vcSymbol 
				and underlyingID = @iUnderlyingContractID 
				and isCall = @iIsCall 
				and abs(strike - @fStrike) < 0.009
				and expiryDate = @dtExpiry )
		end
		else
		begin
			delete from MsOptionCache
			where optionID = @iMsCacheOptionID
		end

		if @iOptionRootID is not null
		begin
		  update OptionRoot
				set
					lotSize = isnull(@iLotSize,lotSize)
				where optionRootID = @iOptionRootID
		end

		UPDATE ContractPrice 
			SET priceClose = isnull(@fPriceClose,priceClose)
		WHERE exchangeID is null and contractID = @OptionID

		update [Option]
			set 
			IsCall = isnull(@iIsCall,IsCall),
			strike = isnull(@fStrike,Strike),
			expiryID = isnull(@iExpiryID,expiryID),
			isManualyEntered = 0
		where optionID = @OptionID

	   if (@@error <> 0)
		   begin
			  RAISERROR ('Error. Can''t update Option information (table Option)!', 16, 1)
			  RETURN (-1)
		   end 
	end 
	 
   set @error = 0
finish:
Return(@error)
go

alter  PROCEDURE usp_Option_Import
	@iUnderlyingContractID int,
	@vcSymbol varchar(20),
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
	@iLotSize int,
	@iMsCacheOptionID int = NULL,
	@vcSymbolUnderlying varchar(20) = NULL,
	@fPriceClose float = NULL
as

/*

	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare	@OptionID int, 
			@cn varchar(255),
			@iExpiryID int,
			@iLastExpiryID int,
			@vcUnderlyingSymbol varchar(8),
			@ERR_MSG varchar(8000)

	if @vcSymbolUnderlying is null
      		select @vcUnderlyingSymbol = symbol from ContractView where contractID = @iUnderlyingContractID
	else
		set  @vcUnderlyingSymbol = @vcSymbolUnderlying

	--===================================================================			
	-- check the Expiry
	--===================================================================			
	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	if not exists(select * from Expiry where expiryID = @iExpiryID)
		begin
			set @ERR_MSG = 'Incorrect expiration! '  + '''' +  Convert(varchar(50),@dtExpiry) + '''' + ' for symbol=' + '''' + @vcSymbol + '''' +  ' UnderlyingSymbol=' + '''' + @vcUnderlyingSymbol + '''' + ' Strike=' + Convert(varchar(15),@fStrike) + ' expiryID=' + Convert(varchar(15),@iExpiryID)
			Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end

	--===================================================================			
	-- check symbol of option root
	--===================================================================			  
   DECLARE @vcOptionRootSymbol varchar(5)	
   set 	@vcOptionRootSymbol = left(@vcSymbol,len(@vcSymbol)-2)

   if exists (select optionRootID 
				   from OptionRootView
				   where symbol = @vcOptionRootSymbol and 
					underlyingID<>@iUnderlyingContractID)
		begin			
			Declare @vcDifferentUndSymbol varchar(8)
			select @vcDifferentUndSymbol = C.symbol from ContractView C join
					OptionRootView ort on ort.underlyingID = C.contractID
					where ort.symbol = @vcOptionRootSymbol and
					ort.underlyingID<>@iUnderlyingContractID
					
			set @ERR_MSG = 'Incorrect option symbol! '  + '''' +  @vcSymbol + ''''  +  ': Option Root=' + '''' + @vcOptionRootSymbol + '''' + ' assign to different underlying=' + '''' +  @vcDifferentUndSymbol + '''' + '. Please, rename Option Root and restart importing of market structure.'
			Raiserror(@ERR_MSG,16,1)
			RETURN (-1)
		end  


	select top 1 @iLastExpiryID = expiryID from ExpiryInCalendarView where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	--===================================================================			
	-- select the @iOptionRootID
	--===================================================================			
	declare @iOptionRootID int,
			@vcOptionRoot varchar(20)
	select @iOptionRootID = optionRootID from OptionRootView where symbol = @vcOptionRootSymbol
			and underlyingID = @iUnderlyingContractID 

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if @iOptionRootID is not null 
		select @OptionID = optionID
			from [Option] (nolock)
			where optionRootID = @iOptionRootID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	--===================================================================			
	-- check if option with same symbol exists
	--===================================================================		
	set @ERR_MSG = null

	execute usp_Option_Check @iOptionID = @OptionID, @vcSymbol = @vcSymbol,	@iOptionRootID = @iOptionRootID,
			@dtExpiry = @dtExpiry,@iExpiryID = @iExpiryID,@fStrike = @fStrike,@iIsCall = @iIsCall,
			@vcUnderlyingSymbol = @vcUnderlyingSymbol,@bRemoveOption = 1,@ERR_MSG = @ERR_MSG output

	if @ERR_MSG is not null and @ERR_MSG != ''
	begin
		if @ERR_MSG != '*' Raiserror(@ERR_MSG,16,1)
		Return(-1)
	end

	--===================================================================			
	-- make the contract name
	--===================================================================			
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(month, @dtExpiry) + 
					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)


  	if (@OptionID is Null) 
	begin
		Begin Transaction

			if  @iOptionRootID is null
			begin
				insert into OptionRoot (underlyingID,symbol,lotSize)
					select @iUnderlyingContractID, @vcOptionRootSymbol, isnull(@iLotSize,100)
				set @iOptionRootID = @@identity
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end 
	
			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@cn)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				 values (@OptionID, @iOptionRootID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			DECLARE @error_var int, @rowcount_var int
			UPDATE ContractPrice 
				SET priceClose = isnull(@fPriceClose,priceClose)
			WHERE exchangeID is null and contractID = @OptionID

			SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
			if @error_var = 0
			begin
			  if @rowcount_var = 0
			  begin
				insert into ContractPrice(contractID,exchangeID,priceClose)
					values(@OptionID, null, @fPriceClose)
				if (@@error <> 0) begin rollback tran return(-1) end           
			  end	
			end
                        else
			 if (@@error <> 0) begin rollback tran return(-1) end           



			if @iMsCacheOptionID is null
			begin
				delete from MsOptionCache
				where (
					symbol = @vcSymbol 
					and underlyingID = @iUnderlyingContractID 
					and isCall = @iIsCall 
					and abs(strike - @fStrike) < 0.009
					and expiryDate = @dtExpiry )
			end
			else
			begin
				delete from MsOptionCache
				where optionID = @iMsCacheOptionID
			end

		Commit Transaction
	end 
 	else
	begin
		update Contract
			set symbol = @vcSymbol,
					 contractName = @cn
			where contractID = @OptionID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract!', 16, 1)
				  RETURN (-1)
			   end 

		if @iMsCacheOptionID is null
		begin
			delete from MsOptionCache
			where (
				symbol = @vcSymbol 
				and underlyingID = @iUnderlyingContractID 
				and isCall = @iIsCall 
				and abs(strike - @fStrike) < 0.009
				and expiryDate = @dtExpiry )
		end
		else
		begin
			delete from MsOptionCache
			where optionID = @iMsCacheOptionID
		end

		if @iOptionRootID is not null
		begin
		  update OptionRoot
				set
					lotSize = isnull(@iLotSize,lotSize)
				where optionRootID = @iOptionRootID
		end

		UPDATE ContractPrice 
			SET priceClose = isnull(@fPriceClose,priceClose)
		WHERE exchangeID is null and contractID = @OptionID

		update [Option]
			set 
			IsCall = isnull(@iIsCall,IsCall),
			strike = isnull(@fStrike,Strike),
			expiryID = isnull(@iExpiryID,expiryID),
			isManualyEntered = 0
		where optionID = @OptionID

	   if (@@error <> 0)
		   begin
			  RAISERROR ('Error. Can''t update Option information (table Option)!', 16, 1)
			  RETURN (-1)
		   end 
	end 
	 
   set @error = 0
finish:
Return(@error)
go


alter    procedure usp_Option_Save
		@iOptionID int,
		@vcSymbol varchar(20) = Null,
		@iExchangeID int, 
		@iUnderlyingContractID int,
		@iIsCall tinyint = Null,
		@fStrike float = Null,
		@dtExpiry datetime = Null,
		@tiIsManualyEntered tinyint
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	   if exists (select contractID from ContractView where contractID = @iUnderlyingContractID and contractTypeID > 2)
	   begin
		 RAISERROR ('Error. Can''t insert or update Option values. The Underlying Contract is not stock or index!', 16, 1)
		 RETURN (-1)
	   end	 
 	  
	   if len(@vcSymbol) < 3  and @iOptionID is null
	   begin
		 RAISERROR ('Error. Option symbol can''t be less three symbols!', 16, 1)
		 RETURN (-1)
	   end	 

	   if exists (select optionRootID 
		   from OptionRootView
		   where symbol = left(@vcSymbol,len(@vcSymbol)-2)and 
			underlyingID != @iUnderlyingContractID)
		begin
		  RAISERROR ('Error. Can''t insert or update Option. Current Root assigned to different Underlying', 16, 1)
		  RETURN (-1)
		end  

	--===================================================================			
	-- check if option with same symbol exists
	--===================================================================		
	declare	@iOptionRootID int,
			@ContractName varchar(255),
			@iExpiryID int

	select @iOptionRootID = optionRootID from OptionRootView where symbol = left(@vcSymbol,len(@vcSymbol)-2)  and 
		underlyingID=@iUnderlyingContractID	

	declare @ERR_MSG varchar(8000),
			@vcUnderlyingSymbol varchar(10)

	select @vcUnderlyingSymbol = symbol 
		from ContractView 
		where contractID = @iUnderlyingContractID and contractTypeID <= 2

	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	execute usp_Option_Check @iOptionID = @iOptionID, 
					@vcSymbol = @vcSymbol,	
					@iOptionRootID = @iOptionRootID,
					@dtExpiry = @dtExpiry,
					@iExpiryID = @iExpiryID,
					@fStrike = @fStrike,
					@iIsCall = @iIsCall,
					@vcUnderlyingSymbol = @vcUnderlyingSymbol,
					@ERR_MSG = @ERR_MSG output

		/*	select @vcSymbol '@vcSymbol',@iOptionRootID '@iOptionRootID',@dtExpiry '@dtExpiry',@iExpiryID '@iExpiryID',
				@fStrike '@fStrike',@iIsCall '@iIsCall',@vcUnderlyingSymbol '@vcUnderlyingSymbol',@ERR_MSG '@ERR_MSG'
			select @iOptionID = 0 */

	if isnull(@ERR_MSG,'')!= ''
		begin
			if @ERR_MSG != '*' Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end

   set @ContractName = @vcUnderlyingSymbol + ' ' + datename(day, @dtExpiry) + ' ' + datename(month, @dtExpiry) + 
					' ' + datename(year, @dtExpiry) + ' ' + CASE @iIsCall when 1 then 'Call' else 'Put' end +
						 ' ' + cast(round(@fStrike, 2) as varchar)

	Begin Transaction

	   if  @iOptionRootID is null
			begin
				insert into OptionRoot (underlyingID,symbol,lotSize)
					select @iUnderlyingContractID,left(@vcSymbol,len(@vcSymbol)-2),100
				set @iOptionRootID = @@identity
			end 

	   if (@iOptionID is Null)	
		 begin
			insert into Contract (contractTypeID, symbol, contractName)
				values (3, @vcSymbol, @ContractName)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iOptionID = @@identity			  
			
			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				values (@iOptionID, @iOptionRootID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		 end
	   else
		 begin
			update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@ContractName,contractName)
				where contractID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			declare @iOldOptionID as int
			select @iOldOptionID = optionRootID from [Option] 
				where optionID = @iOptionID

			update [Option]
				set optionRootID = @iOptionRootID,
					isCall = isNull(@iIsCall, isCall),
					strike = isNull(@fStrike, strike),
					expiryID = isNull(@iExpiryID, expiryID)
				where optionID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			if @iOldOptionID is not null and @iOptionRootID is not null
			begin
			  update Trade set OptionRootId = @iOptionRootID where 	OptionRootId = @iOldOptionID
	  		  if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		 end		

		if @tiIsManualyEntered in (0,1) 
			begin
--select @iOptionID as '@iOptionID*******************'
				update [Option] set isManualyEntered = @tiIsManualyEntered where optionID = @iOptionID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end 

	Commit Transaction

    set @error = 0
finish:
if (@error != 0) return (@error) else return @iOptionID
go

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OTCFutureOptions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_OTCFutureOptions_Get]
GO

create  PROC usp_OTCFutureOptions_Get
	@futureID int,
	@expiryDate datetime,
	@strike float
AS
	set nocount on

	select
	foc.symbol
	from Future f
		inner join ContractView fc on f.futureID = fc.contractID
		inner join FutureOption fo on f.futureID = fo.futureID
		inner join ContractView foc on fo.futureOptionID = foc.contractID
		inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
	where
		f.futureID = @futureID
		and fec.expiryDate = @expiryDate
		and fo.strike = @strike
go

alter  PROCEDURE usp_PriceProfile_Save 
	@iPriceProfileID int = null, 
	@vcPriceProfileName varchar(50) = null,
	@vcPriceProfileDesc varchar(255) = null,
	@tiIsOptionProfile tinyint = null,
	@tiAlwaysLast tinyint = null,
	@tiMidmarketPnL tinyint = null,
	@tiSinglePriceRule tinyint = null,
	@tiBothPriceRule tinyint = null,
	@tiZeroBidRuleForPnL tinyint = null
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if @iPriceProfileID is null and @vcPriceProfileName is null
	begin
		raiserror ('Name for new price profile can not be empty!', 16, 1)
		set @error = -1
		goto finish 
	end

	if exists (select * from PriceProfile
				where priceProfileName = @vcPriceProfileName
					and isOptionProfile = isnull(@tiIsOptionProfile, 0) and actionID < 3
					and (priceProfileID <> @iPriceProfileID or @iPriceProfileID is null))
		begin
			if isnull(@tiIsOptionProfile, 0) = 0
				raiserror ('Underlying price profile with the same name already exists!', 16, 1)
			else
				Raiserror ('Option price profile with the same name already exists!', 16, 1)
			set @error = -2
			goto finish 
		end

	Begin Transaction PriceProfile

	if @iPriceProfileID is null
		begin
			insert into PriceProfile 
				(priceProfileName, priceProfileDesc, defaultID, isOptionProfile,
				alwaysLast, midmarketPnL, singlePriceRule, bothPriceRule, ZeroBidRuleForPnL)
			values
				(@vcPriceProfileName, @vcPriceProfileDesc, 0, isnull(@tiIsOptionProfile, 0),
				isnull(@tiAlwaysLast, 0), isnull(@tiMidmarketPnL, 0), isnull(@tiSinglePriceRule, 0), isnull(@tiBothPriceRule, 0), isnull(@tiZeroBidRuleForPnL, 0))

			if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction PriceProfile goto finish end

			set @iPriceProfileID = @@identity
		end
	else
		begin
			update PriceProfile set 
				priceProfileName = isnull(@vcPriceProfileName, priceProfileName),
				priceProfileDesc = isnull(@vcPriceProfileDesc, priceProfileDesc),
				isOptionProfile = isnull(@tiIsOptionProfile, isOptionProfile),
				alwaysLast = isnull(@tiAlwaysLast, alwaysLast),
				midmarketPnL = isnull(@tiMidmarketPnL, midmarketPnL),
				singlePriceRule = isnull(@tiSinglePriceRule, singlePriceRule),
				bothPriceRule = isnull(@tiBothPriceRule, bothPriceRule),
				ZeroBidRuleForPnL = isnull(@tiZeroBidRuleForPnL, ZeroBidRuleForPnL)
			where
				priceProfileID = @iPriceProfileID

   			if @@error <> 0 begin set @error = -4 if @@TranCount > 0  Rollback Transaction PriceProfile goto finish end
		end

	Commit Transaction PriceProfile

finish:
	if @error = 0
		return @iPriceProfileID
	else
		return @error
go

alter  PROCEDURE usp_PriceProfile_Save 
	@iPriceProfileID int = null, 
	@vcPriceProfileName varchar(50) = null,
	@vcPriceProfileDesc varchar(255) = null,
	@tiIsOptionProfile tinyint = null,
	@tiAlwaysLast tinyint = null,
	@tiMidmarketPnL tinyint = null,
	@tiSinglePriceRule tinyint = null,
	@tiBothPriceRule tinyint = null,
	@tiZeroBidRuleForPnL tinyint = null
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	if @iPriceProfileID is null and @vcPriceProfileName is null
	begin
		raiserror ('Name for new price profile can not be empty!', 16, 1)
		set @error = -1
		goto finish 
	end

	if exists (select * from PriceProfile
				where priceProfileName = @vcPriceProfileName
					and isOptionProfile = isnull(@tiIsOptionProfile, 0) and actionID < 3
					and (priceProfileID <> @iPriceProfileID or @iPriceProfileID is null))
		begin
			if isnull(@tiIsOptionProfile, 0) = 0
				raiserror ('Underlying price profile with the same name already exists!', 16, 1)
			else
				Raiserror ('Option price profile with the same name already exists!', 16, 1)
			set @error = -2
			goto finish 
		end

	Begin Transaction PriceProfile

	if @iPriceProfileID is null
		begin
			insert into PriceProfile 
				(priceProfileName, priceProfileDesc, defaultID, isOptionProfile,
				alwaysLast, midmarketPnL, singlePriceRule, bothPriceRule, ZeroBidRuleForPnL)
			values
				(@vcPriceProfileName, @vcPriceProfileDesc, 0, isnull(@tiIsOptionProfile, 0),
				isnull(@tiAlwaysLast, 0), isnull(@tiMidmarketPnL, 0), isnull(@tiSinglePriceRule, 0), isnull(@tiBothPriceRule, 0), isnull(@tiZeroBidRuleForPnL, 0))

			if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction PriceProfile goto finish end

			set @iPriceProfileID = @@identity
		end
	else
		begin
			update PriceProfile set 
				priceProfileName = isnull(@vcPriceProfileName, priceProfileName),
				priceProfileDesc = isnull(@vcPriceProfileDesc, priceProfileDesc),
				isOptionProfile = isnull(@tiIsOptionProfile, isOptionProfile),
				alwaysLast = isnull(@tiAlwaysLast, alwaysLast),
				midmarketPnL = isnull(@tiMidmarketPnL, midmarketPnL),
				singlePriceRule = isnull(@tiSinglePriceRule, singlePriceRule),
				bothPriceRule = isnull(@tiBothPriceRule, bothPriceRule),
				ZeroBidRuleForPnL = isnull(@tiZeroBidRuleForPnL, ZeroBidRuleForPnL)
			where
				priceProfileID = @iPriceProfileID

   			if @@error <> 0 begin set @error = -4 if @@TranCount > 0  Rollback Transaction PriceProfile goto finish end
		end

	Commit Transaction PriceProfile

finish:
	if @error = 0
		return @iPriceProfileID
	else
		return @error
go

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_QuotationUnit_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_QuotationUnit_Del]
GO
create  PROCEDURE usp_QuotationUnit_Del
  @iCptID int,
  @tiIsRaiseError tinyint = 1 
AS
/*
  Created by Pavel Ananyev
*/
  -------------------------------------------------
  set nocount on
  -------------------------------------------------
  declare @error int    set @error = 0

  if exists(select 1 from QuotationUnit where iQuotationUnitID = @iCptID)
    begin
      if @tiIsRaiseError = 1
        begin
          Raiserror('Can''t remove Quotation unit, because it''s used in root defenitions!',16,1)
          Return(0)
        end
      else
        begin
          Return(-1)
        end
    end

  Begin Transaction TntCounterParty

  delete QuotationUnit where iQuotationUnitID = @iCptID
  if @@error <> 0 begin set @error = -2 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end

  Commit Transaction TntCounterParty

finish:
  return @error
go

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_QuotationUnit_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_QuotationUnit_Get]
GO
create  PROCEDURE usp_QuotationUnit_Get
  @iQuotationUnitID int = null
AS
/*
  Created by Pavel Ananyev
*/
  -------------------------------------------------
  set nocount on
  -------------------------------------------------
  if @iQuotationUnitID is null
    select
      cpt.iQuotationUnitID as iQuotationUnitID,
      cpt.vcName as vcName,
      cpt.vcDesc as vcDesc
    from
      QuotationUnit cpt
  else
    select
      cpt.iQuotationUnitID as iQuotationUnitID,
      cpt.vcName as vcName,
      cpt.vcDesc as vcDesc
    from
      QuotationUnit cpt
    where
      cpt.iQuotationUnitID = @iQuotationUnitID

return @@error
go

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_QuotationUnit_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_QuotationUnit_Save]
GO
create  PROCEDURE usp_QuotationUnit_Save
  @iCptID int = null,
  @vcName varchar(255),
  @vcDesc varchar(255) = null,
  @tiRaiseError tinyint = null
AS
/*
  Created by Pavel Ananyev
*/
  -------------------------------------------------
  set nocount on
  -------------------------------------------------
  declare @error int    set @error = 0

  if @iCptID is null and isnull(@vcName, '') = ''
  begin
    raiserror ('Name for new future underlying type can not be empty!', 16, 1)
    set @error = -1
    goto finish
  end

  if @iCptID is null
  begin
    select
      @iCptID = cpt.iQuotationUnitID
    from
      QuotationUnit cpt
    where
      cpt.vcName = @vcName

    if @iCptID is not null and isnull(@tiRaiseError, 0) <> 0 
    begin
      raiserror ('Quotation unit with the same name is already exists!', 16, 1)
      set @error = -2
      goto finish
    end
  end

  Begin Transaction TntCounterParty

  if @iCptID is null
  begin
    insert into QuotationUnit
      (vcName, vcDesc)
    values
      (@vcName, @vcDesc)

    if @@error <> 0 begin set @error = -3 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end
    set @iCptID = @@identity
  end
  else
  begin
    update QuotationUnit set 
      vcName = isnull(@vcName, vcName),
      vcDesc = isnull(@vcDesc, vcDesc)
    where
      iQuotationUnitID = @iCptID

    if @@error <> 0 begin set @error = -4 if @@TranCount > 0 Rollback Transaction TntCounterParty goto finish end
  end

  Commit Transaction TntCounterParty

finish:
  if @error = 0
    return @iCptID
  else
    return @error
go

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_RootType_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_RootType_Get]
GO
create   Procedure usp_RootType_Get 
		@ContractID int
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @RootType int		set @RootType = -1
	declare @RootID int		set @RootID = -1
	 
	SELECT  @RootID = fr.FutureRootId	
	FROM  dbo.Future f INNER JOIN dbo.FutureRoot fr ON f.futureRootID = fr.futureRootID
	WHERE     (f.futureID = @ContractID)

	if @RootID = -1
	  begin
		select  @RootID = OptionRootId  from OptionRoot where UnderlyingID = @ContractID
		if @RootID <> -1
		  begin
			Set @RootType = 0
		  end
		else
		  begin
			declare @ContractType int		set @ContractType = -1
				 
			select  @ContractType = c.contracttypeid
			from  dbo.Contract c 
			where     (c.contractid = @ContractID)
			if @ContractType = 4 
			  begin
				Set @RootType = 1
			  end

		  end
		
	  end
	else 	
	  begin
		
			Set @RootType = 1
	  end 

select @RootType as RootType
return (@RootType)
go

alter  proc usp_Stock_Import
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
				update ContractPrice 
					set priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					PriceOpen = @fPriceOpen,
					PriceClose = @fPriceClose,
					PriceLow = @fPriceLow,
					PriceHigh = @fPriceHigh,
					Volume = @iVolume
				where contractID = @iContractID and (exchangeID=@iExchangeID or @iExchangeID is null)
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
			set @exchangeID = (select TOP 1 e.exchangeID from Exchange e where @vcExchangeCode = exchangeCode)
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
						Raiserror ('Error. Can''t update DivAmnt, DivDate and primaryExchangeID!', 16, 1)
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
go


alter  PROCEDURE usp_SyntheticParams_Get 
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
go


alter   PROCEDURE usp_SyntheticParam_Save 
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
go


alter   PROC usp_TradeFeed_1_New
	@vcTraderAcronym nvarchar(12),
	@vcExecID nvarchar(128),
	@tiIsBuy tinyint,  
	@iQuantity int,
	@fPrice float,
	@dtTradeDate datetime,
	@vcContractSymbol nvarchar(20),
	@iContractTypeID int,
	@vcUnderlyingSymbol nvarchar(20) = null,
	@tiIsCall tinyint = null,
	@fStrike float = null,
	@dtExpiry datetime = null,
	@iTradeLotSize int = null,
	@tiIsUpdateEnabled tinyint = 0,
	@iLastTradeTime int = null, 
	@vcOptRootSymbol nvarchar(20) = null,
	@iMark int = null,
	@vcStrategyName nvarchar(12) = null,
	@fStockRefPrice float = null,
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
			select @iMinSeqNum = isnull(max(seqNum),0) + 1 from Trade
	
			insert into Trade(tradeID, actionID, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, seqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID)
			select tradeID, 0, contractID, tradeDate, quantity, price, isBuy, traderID,
				strategyID, brokerID, brokerCommission, clearingBrokerID, clearingBrokerCommission,
				tradedIV, spotReference, isPosition, status, execID, @iMinSeqNum, mark,
				underlyingID, optionRootID, futureRootID, futureID
			from Trade
			where seqNum = @iSeqNum
			if @@error <> 0 begin set @error = 2 if @@TranCount > 0  Rollback Transaction trade goto finish end
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
					@iExpiryID int,
					@iOptionRootID int

			Select @iOptionRootID = optionRootID from OptionRootView where symbol = @vcOptRootSymbol
	
			set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
			set @fStrike = round(@fStrike, 3)
	
			select @iOptionID = optionID
				from OptionView (nolock)
				where optionRootID = @iOptionRootID and 
					isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID < 3
	
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

						insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
							 values (@iOptionID, @iOptionRootID, @tiIsCall, @fStrike, @iExpiryID)
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

	Declare @iTraderID int
		select @iTraderID = traderID from TraderView where acronym = @vcTraderAcronym

	--work with Trader

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
	select @iNewTradeID = isnull(max(tradeID), 0) + 1 from Trade

	select @iSeqNum = isnull(max(seqNum),0) + 1 from Trade

	--create new trade	
	insert into Trade (execID, tradeID, actionID, contractID, tradeDate, 
		quantity, price, traderID, brokerId, strategyID, isBuy, isPosition, status, seqNum, mark,
		underlyingID, optionRootID, futureRootID, futureID, spotReference)
	values (@vcExecID, @iNewTradeID, 1, @iTradeContractID, @dtTradeDate, 
		@iQuantity, @fPrice, @iTraderID, @iBrokerId, @iStrategyID, @tiIsBuy, 0, 0, @iSeqNum, @iMark,
		@iUnderlyingContractID, @iOptionRootID, null, null, @fStockRefPrice )

	if @@error <> 0 begin set @error = 2 if @@TranCount > 0 Rollback Transaction goto finish end

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
go

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_SU_Exec]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_SU_Exec]
GO
create  PROC usp_Trade_SU_Exec
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
go

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Trade_UpdateIV]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Trade_UpdateIV]
GO
create  PROC usp_Trade_UpdateIV
	@iTradeID int,
	@fTradeIV float

as
	set nocount on

	declare @error int		set @error = 0
	
	update Trade set tradedIV = @fTradeIV
		where tradeID = @iTradeID and actionID = 1
go



alter  procedure usp_UnderlyingByGroup_Get 
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int,
	@tiWithPosition tinyint = 0
AS
	set nocount on
    if not  @iContractID is null
     begin
	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and c.contractID = @iContractID
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and c.contractID = @iContractID
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and c.contractID = @iContractID
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and c.contractID = @iContractID
			order by c.symbol	 
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity  * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and c.contractID = @iContractID
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
				s.isActive, s.skew, s.kurt
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and c.contractID = @iContractID
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,

			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and tc.traderID = @iTraderID
				and c.contractID = @iContractID
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose,
				s.isActive, s.skew, s.kurt
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and tc.traderID = @iTraderID
				and c.contractID = @iContractID
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt
			order by c.symbol	 
	end
       end
       else
----------------------------------------------	
     begin
	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and tc.traderID = @iTraderID
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and tc.traderID = @iTraderID
			order by c.symbol	 
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,
			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity  * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
				s.isActive, s.skew, s.kurt
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt
			order by c.symbol	 
		else
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				s.calcOptionType as tiCalcOptionType,
				s.isHTB as tiIsHTB,
			
			    null as fYield,
				null as tiIsBasket,
			    s.divFreq as iDivFreq,
			    s.divDate as dtDivDate,
			    s.divAmt as fDivAmt,
			
			    s.isDivCustom as tiIsDivCustom,
			    s.divFreqCustom as iDivFreqCustom,

			    s.divDateCustom as dtDivDateCustom,
			    s.divAmtCustom as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				s.isActive as tiIsActive,

				s.skew as fSkew,
				s.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join StockView s on c.contractID = s.stockID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 2
				and tc.traderID = @iTraderID
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
			    s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose,
				s.isActive, s.skew, s.kurt
			union
			select
			    c.contractID as iContractID,
			    c.contractTypeID as iContractTypeID,
			    c.Symbol as vcSymbol,
				c.expCalendarID as iExpCalendarID,
				i.calcOptionType as tiCalcOptionType,
				i.isHTB as tiIsHTB,
			
			    i.yield as fYield,
				i.isBasket as tiIsBasket,
			    null as iDivFreq,
			    null as dtDivDate,
			    null as fDivAmt,
			
			    null as tiIsDivCustom,
			    null as iDivFreqCustom,
			    null as dtDivDateCustom,
			    null as fDivAmtCustom,
			
			    cp.priceClose as fPriceClose,

				sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
				i.isActive as tiIsActive,

				i.skew as fSkew,
				i.kurt as fKurt,

				sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
			from ContractInGroupView cig
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
			    inner join IndexView i on c.contractID = i.indexID
			    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
			where cig.groupID = @iGroupID
				and c.contractTypeID = 1
				and tc.traderID = @iTraderID
			group by 
				c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
			    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt
			order by c.symbol	 
	end
       end

	Return (0)
go


alter  PROCEDURE usp_Underlying_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0
AS
	set nocount on

	if isnull(@iGroupID, 0) <> 0
	begin
		exec usp_UnderlyingByGroup_Get @iContractID,@iTraderID,@iGroupID,@tiWithPosition	
		Return(0)
	end 
	if not @iContractID is null
	begin
		if isnull(@tiWithPosition, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
				    c.expCalendarID as iExpCalendarID,
				    s.calcOptionType as tiCalcOptionType,
				    s.isHTB as tiIsHTB,			
				    null as fYield,
				    null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
				    s.isActive as tiIsActive,

				    s.skew as fSkew,
				    s.kurt as fKurt,

				    c.undPriceProfileID as iUndPriceProfileID,
				    c.optPriceProfileID as iOptPriceProfileID
			   	    from ContractView c 
				    	inner join StockView s on c.contractID = s.stockID
				    	inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		  		    where
					c.contractTypeID = 2
					and c.contractID = @iContractID
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
					i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
				
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
					i.isActive as tiIsActive,
					i.skew as fSkew,
					i.kurt as fKurt,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
					and c.contractID = @iContractID
				order by c.symbol	 
			else
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
					null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
				    s.isActive as tiIsActive,

				    s.skew as fSkew,
				    s.kurt as fKurt,

				    c.undPriceProfileID as iUndPriceProfileID,
				    c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and  c.contractID = @iContractID
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
			
				    i.yield as fYield,
 				    i.isBasket as tiIsBasket,
			 	    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
			
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
				    i.isActive as tiIsActive,
				    i.skew as fSkew,
				    i.kurt as fKurt,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
					and c.contractID = @iContractID
				order by c.symbol	 
		end
		else -- not isnull(@tiWithPosition, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
			
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					s.isActive as tiIsActive,

					s.skew as fSkew,
					s.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 2
					and c.contractID = @iContractID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				    	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
					s.isActive, s.skew, s.kurt, c.undPriceProfileID, c.optPriceProfileID
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
			
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
			
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

					i.isActive as tiIsActive,

					i.skew as fSkew,
					i.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 1
					and c.contractID = @iContractID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.symbol	 
			else
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
			
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
	
					s.isActive as tiIsActive,

					s.skew as fSkew,
					s.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
				where 
					c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and c.contractID = @iContractID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				   	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
					s.isActive, s.skew, s.kurt, c.undPriceProfileID, c.optPriceProfileID
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
			
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
			
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

					i.isActive as tiIsActive,

					i.skew as fSkew,
					i.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
					and c.contractID = @iContractID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.symbol	 
		end
          end
	else
          begin
--------------------------------------------------
		if isnull(@tiWithPosition, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
				    c.expCalendarID as iExpCalendarID,
				    s.calcOptionType as tiCalcOptionType,
				    s.isHTB as tiIsHTB,			
				    null as fYield,
				    null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
				    s.isActive as tiIsActive,

				    s.skew as fSkew,
				    s.kurt as fKurt,

				    c.undPriceProfileID as iUndPriceProfileID,
				    c.optPriceProfileID as iOptPriceProfileID
			   	    from ContractView c 
				    	inner join StockView s on c.contractID = s.stockID
				    	inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		  		    where
					c.contractTypeID = 2
					and @iContractID is null 
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
					i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
				
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
					i.isActive as tiIsActive,
					i.skew as fSkew,
					i.kurt as fKurt,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
				order by c.symbol	 
			else
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
					null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
				    s.isActive as tiIsActive,

				    s.skew as fSkew,
				    s.kurt as fKurt,

				    c.undPriceProfileID as iUndPriceProfileID,
				    c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 2
					and tc.traderID = @iTraderID
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
			
				    i.yield as fYield,
 				    i.isBasket as tiIsBasket,
			 	    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
			
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
				    i.isActive as tiIsActive,
				    i.skew as fSkew,
				    i.kurt as fKurt,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
				order by c.symbol	 
		end
		else -- not isnull(@tiWithPosition, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
			
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					s.isActive as tiIsActive,

					s.skew as fSkew,
					s.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 2
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				    	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
					s.isActive, s.skew, s.kurt, c.undPriceProfileID, c.optPriceProfileID
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
			
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
			
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

					i.isActive as tiIsActive,

					i.skew as fSkew,
					i.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 1
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.symbol	 
			else
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
			
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
	
					s.isActive as tiIsActive,

					s.skew as fSkew,
					s.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
				where 
					c.contractTypeID = 2
					and tc.traderID = @iTraderID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				   	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
					s.isActive, s.skew, s.kurt, c.undPriceProfileID, c.optPriceProfileID
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					c.expCalendarID as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
			
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
			
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

					i.isActive as tiIsActive,

					i.skew as fSkew,
					i.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.symbol	 
		end

        end

	Return (0)
go



--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.163')
	begin
		update DataInfo set keyValue = '3.46.163' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.163' '3.46.163 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
