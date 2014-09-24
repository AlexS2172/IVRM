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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.177'  or (keyValue = '3.46.178') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.178 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.178' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
----------------------------------------------------------------
GO
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[QuotationUnit]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[QuotationUnit]
GO

if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[QuotationUnit]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
 BEGIN
CREATE TABLE [dbo].[QuotationUnit] (
	[iQuotationUnitID] [int] NOT NULL ,
	[vcName] nvarchar (255) NOT NULL ,
	[vcDesc] nvarchar (255) NULL ,
	CONSTRAINT [PK_QuotationUnit] PRIMARY KEY  CLUSTERED 
	(
		[iQuotationUnitID]
	)  ON [PRIMARY] 
) ON [PRIMARY]
END
GO
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (0,  N'None',       N'None')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (1,  N'Nominal',    N'Nominal')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (2,  N'Bags',       N'Bags')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (3,  N'Bushel',     N'Bushel')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (4,  N'Barrels',    N'Barrels')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (5,  N'Gallons',    N'Gallons')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (6,  N'Grams',      N'Grams')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (7,  N'Kilos',      N'Kilos')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (8,  N'ShortTons',  N'ShortTons')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (9,  N'LongTons',   N'LongTons')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (10, N'MetricTons', N'MetricTons')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (11, N'Ounces',     N'Ounces')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (12, N'Pounds',     N'Pounds')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (13, N'SquareFeet', N'SquareFeet')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (14, N'FineTroyOunces', N'FineTroyOunces')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (15, N'TroyOunces', N'TroyOunces')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (16, N'BoardFeet', N'BoardFeet')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (17, N'Coins', N'Coins')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (18, N'Bales', N'Bales')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (19, N'BoxCases', N'BoxCases')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (20, N'BottleFlaskes', N'BottleFlaskes')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (21, N'Percent', N'Percent')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (22, N'NonFatDryMilk', N'NonFatDryMilk')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (23, N'BtuMillions', N'BtuMillions')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (24, N'MegaWattHours', N'MegaWattHours')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (25, N'HundredWeight', N'HundredWeight')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (26, N'HundredWeight112LBS', N'HundredWeight112LBS')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (27, N'Ones', N'Ones')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (28, N'Tens', N'Tens')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (29, N'Dozens', N'Dozens')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (30, N'Hundreds', N'Hundreds')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (31, N'Thousands', N'Thousands')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (32, N'Millions', N'Millions')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (33, N'Pieces', N'Pieces')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (34, N'TenPieces', N'TenPieces')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (35, N'HundredPieces', N'HundredPieces')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (36, N'ThousandPieces', N'ThousandPieces')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (37, N'TenThousandPieces', N'TenThousandPieces')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (38, N'Indices', N'Indices')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (39, N'FuturesContracts', N'FuturesContracts')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (40, N'Currency', N'Currency')
insert into QuotationUnit (iQuotationUnitID, vcName,vcDesc) values (41, N'Shares', N'Shares')
GO
--ALTER TABLE [dbo].[FutureUnderlying] DROP CONSTRAINT [FK_FutureUnderlying_FutureUnderlyingType]

GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FutureUnderlyingType]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[FutureUnderlyingType]
GO

CREATE TABLE [dbo].[FutureUnderlyingType] (
	[iFutureUnderlyingTypeID] [int] NOT NULL ,
	[vcName] nvarchar (255) COLLATE Latin1_General_CI_AS NOT NULL, 
	[vcDesc] nvarchar (255) COLLATE Latin1_General_CI_AS NULL,
	CONSTRAINT [PK_FutureUnderlyingType] PRIMARY KEY  CLUSTERED 
	(
		[iFutureUnderlyingTypeID]
	)  ON [PRIMARY] 

) ON [PRIMARY]
GO
insert into FutureUnderlyingType (iFutureUnderlyingTypeID, vcName, vcDesc) values (0,  N'Index',  N'Index')
insert into FutureUnderlyingType (iFutureUnderlyingTypeID, vcName, vcDesc) values (1,  N'Commodity',  N'Commodity')
insert into FutureUnderlyingType (iFutureUnderlyingTypeID, vcName, vcDesc) values (2,  N'ForEx',  N'Foreign exchange')
insert into FutureUnderlyingType (iFutureUnderlyingTypeID, vcName, vcDesc) values (3,  N'InterestRate',  N'Interest rate')
insert into FutureUnderlyingType (iFutureUnderlyingTypeID, vcName, vcDesc) values (4,  N'Weather',  N'Weather')

GO

GO
ALTER  PROCEDURE dbo.usp_BoFuture_Import
	@iFutureID int,
	@iFutureRootID int,
	@iExpiryID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iLotSizeID int,
	@iOptionLotSizeID int,
        @fPriceClose float,
	@vcBadFutureName varchar(255) output,
        @fTickSize float = null,
        @iQuotationUnitID int = null,
        @vcQuotationUnitCurrency varchar(3) = null,
	@dtExpiryDate as datetime = null	
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	declare @tiIsActive tinyint
	
	if @iFutureRootID is null
		select @iFutureRootID = futureRootID
			from Future (nolock)
			where futureID = @iFutureID
			
	exec @error = usp_EodFuture_Import @iFutureRootID,@iExpiryID,@vcSymbol,@vcContractName,@iLotSizeID,@iOptionLotSizeID, @tiIsActive output, @vcBadFutureName output, @fPriceClose, @fTickSize, @iQuotationUnitID, @vcQuotationUnitCurrency, @dtExpiryDate
	 
finish:
Return(@error)

GO

ALTER  PROC dbo.usp_EodFuture_Import
	@iFutureRootID int,
	@iExpiryID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iLotSizeID int,
	@iOptionLotSizeID int,
	@tiIsActive tinyint output,
	@vcBadFutureName varchar(255) output,
	@fPriceClose float = null,
        @fTickSize float = null,
        @iQuotationUnitID int = null,
        @vcQuotationUnitCurrency varchar(3) = null,
	@dtExpiryDate as datetime = null 
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	declare @error int		set @error = 0

	if not exists(select 1 from Expiry where expiryID = isnull(@iExpiryID, 0))
	begin
		raiserror ('Wrong matiruty date.', 16, 1)
		set @error = -100
		goto finish
	end

	declare	@iFutureID int,
			@iUndPriceProfileID int,
			@iOptPriceProfileID int,
			@iFutureID_bad int
	declare @iExpCalendarID int

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
			select @iFutureID_bad = contractID
			from Contract
			where symbol=@vcSymbol
				--and contractID<>@iFutureID
				and contractTypeID=4

			if @iFutureID_bad is not null and @iFutureID_bad != @iFutureID
			begin
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

			if @dtExpiryDate is not null
			begin
				select @iExpCalendarID = ecv.ExpCalendarID from Future f
				inner join FutureRoot fr on fr.FutureRootID = f.FutureRootID
				inner join ExpiryCalendarView ecv on ecv.expCalendarName = fr.futureRootSymbol + '_Fut'
				where f.FutureID = @iFutureID

				if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtExpiryDate, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @iFutureID
					end
			end			

			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@iFutureID
			if @@error <> 0 begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end

			update Future set actionDate = getdate() where futureID=@iFutureID
			if @@error <> 0 begin set @error = -8 if @@TranCount = 1 Rollback Transaction goto finish end

                                        if @iLotSizeID <> 0 
           			     update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, fTickValue = @fTickSize * @iLotSizeID, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency where futureRootID=@iFutureRootID
                                        else
                                             update FutureRoot  set optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -9 if @@TranCount = 1 Rollback Transaction goto finish end
			
			update ContractPrice
			set priceClose = @fPriceClose
			where exchangeID is null and contractID = @iFutureID
			if @@error <> 0 begin set @error = -10 if @@TranCount = 1 Rollback Transaction goto finish end
		Commit Transaction
	end
	else
	begin
		Begin Transaction
			select @iFutureID_bad = contractID
			from Contract
				where symbol=@vcSymbol
				and contractTypeID=4

			if @iFutureID_bad is not null
			begin
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
			if not exists(select 1 from PriceProfile where priceProfileID = @iUndPriceProfileID)
			begin set @error = -10 if @@TranCount = 1 Rollback Transaction goto finish end
		
			select
				@iOptPriceProfileID = priceProfileID
			from
				PriceProfile
			where
				defaultID = 1
				and isOptionProfile = 1

			-- check options price profile
			if not exists(select 1 from PriceProfile where priceProfileID = @iOptPriceProfileID)
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

			if @dtExpiryDate is not null
			begin
				select @iExpCalendarID = ecv.ExpCalendarID from Future f
				inner join FutureRoot fr on fr.FutureRootID = f.FutureRootID
				inner join ExpiryCalendarView ecv on ecv.expCalendarName = fr.futureRootSymbol + '_Fut'
				where f.FutureID = @iFutureID

				if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtExpiryDate, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @iFutureID
					end
			end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iFutureID)
				begin
					insert into ContractPrice(contractID, exchangeID, priceClose)
						values(@iFutureID, null, @fPriceClose)
					if (@@error <> 0) begin set @error = -14 if @@TranCount = 1 Rollback Transaction goto finish end
				end 

                                        if @iLotSizeID <> 0
        			    update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, fTickValue = @fTickSize * @iLotSizeID, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency where futureRootID=@iFutureRootID
                                        else
                                            update FutureRoot  set optionLotSize = @iOptionLotSizeID, fTickSize = @fTickSize, iQuotationUnitID = @iQuotationUnitID, vcQuotationUnitCurrency = @vcQuotationUnitCurrency where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -15 if @@TranCount = 1 Rollback Transaction goto finish end
			
			set @tiIsActive = 1

		Commit Transaction
	end 
	 
   	set @error = @iFutureID
finish:
	Return(@error)

GO
GO

ALTER   proc dbo.usp_Stock_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate datetime,
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
						Raiserror ('Error. Can''t update DivAmnt and DivDate!', 16, 1)
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

ALTER  proc dbo.usp_BoOptionWithPrice_Get
	  @iUnderlyingID int		 
  as
	select	c.contractID as iContractID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName,
			o.optionRootID as iOptionRootID,
			ec.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheo,
			c.exportSymbol as vcExportSymbol
	from OptionRoot ort
			inner join OptionView o on o.optionRootID = ort.optionRootID
			inner join ContractView c on c.contractID = o.optionID
			inner join ContractView uc on ort.underlyingID = uc.contractID
			inner join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and uc.expCalendarID = ec.expCalendarID
			left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
	where	ort.underlyingID = @iUnderlyingID
			and ec.expiryDate >= getdate()
	order by ort.optionRootID, year(EC.expiryDate), month(EC.expiryDate), o.strike, o.isCall desc
 
  return @@error

GO
ALTER         PROC dbo.usp_AspEodUpdateMarketStructure_DefaultIVs_Execute 
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

	create table #CustStrikeskewpointInsert (volaSurfaceDataID int, type tinyint , strike float, volatility float, isBasePoint int, expiryID int)
	insert into #CustStrikeskewpointInsert (volaSurfaceDataID, type, strike, volatility, isBasePoint, expiryID) --values 
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

	insert into CustomStrikeSkewPoint (volaSurfaceDataID, type, strike, volatility, isBasePoint, expiryID)
		select volaSurfaceDataID, type,	strike,	volatility, isBasePoint, expiryID
		from #CustStrikeskewpointInsert

	if @@error != 0
	begin
		exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating default IV surface: Fail to add new IV points', @iErrorCode = @@error
		set @error = -1
		if @iWithTran is not null rollback tran @vcTranName
		goto finish
	end
	drop table #CustStrikeskewpointInsert

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
	and vsd.contractID not in (
		select t.ContractID from 
		(
			select count(u.contractId) as cnt, u.contractId  from
			(
				select
				     vsd.contractID as contractId, 
				     eod_cssp.volatility as vola
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
				group by vsd.contractID, eod_cssp.volatility 
			) u
			where u.Vola <0
			group by u.contractId
		) t where t.cnt=1
	)

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

GO

ALTER  PROC dbo.usp_CustomStrikeSkewPoint_Get 
	@iCustomStrikeSkewPointID int = null, 
	@iVolaSurfaceDataID int = null,
	@iType int = null,
	@dtExpDate datetime = null
as
/*
	Description:	this procedure get information from 
					CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare	@iExpiryID int
	declare @ContractTypeID int
	declare @UnderlyingContractID int

	-- Sharky: faster request for special parameters' set
	if @iExpiryID is null and @iCustomStrikeSkewPointID is null
		and @iVolaSurfaceDataID is not null and @iType is not null
	begin
		select 
		CSSP.customStrikeSkewPointID as iCustomStrikeSkewPointID,
		CSSP.volaSurfaceDataID as iVolaSurfaceDataID,
		CSSP.strike as fStrike, 
		CSSP.type as iType,
		CSSP.volatility as fVolatility,
		dateadd(mm, CSSP.expiryID, '19991201') as dtExpDate,
		CSSP.isBasePoint as iIsBasePoint --,
		--CSSP.actionDate as dtActionDate,
		--CSSP.actionID as tiActionID
		from CustomStrikeSkewPoint CSSP with(INDEX = IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID, nolock)
		--join VolaSurfaceData vsd on CSSP.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
		where 
		CSSP.volaSurfaceDataID = @iVolaSurfaceDataID
		and CSSP.type = @iType
		and CSSP.actionID < 3
	end
	else
	begin
		-- Sharky: old variant
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)

		select 
			CSSP.customStrikeSkewPointID as iCustomStrikeSkewPointID,
			CSSP.volaSurfaceDataID as iVolaSurfaceDataID,
			CSSP.strike as fStrike, 
			CSSP.type as iType,
			CSSP.volatility as fVolatility,
			dateadd(mm, CSSP.expiryID, '19991201') as dtExpDate,
			--convert(datetime,convert(varchar(6),E.expiryDate,112) + '01') as dtExpDate,
			CSSP.isBasePoint as iIsBasePoint --,
			--CSSP.actionDate as dtActionDate,
			--CSSP.actionID as tiActionID
		from CustomStrikeSkewPointView CSSP with(INDEX = IX_CustomStrikeSkewPoint_volaSurfaceDataID_type_actionID, nolock)
			--join VolaSurfaceData on CSSP.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
			--join Contract C on  VolaSurfaceData.contractID = C.contractID
			--join ExpiryInCalendarView E on E.expCalendarID = C.expCalendarID and CSSP.expiryID = E.expiryID
		where 
			customStrikeSkewPointID = isnull(@iCustomStrikeSkewPointID, customStrikeSkewPointID)
			and CSSP.volaSurfaceDataID = isnull(@iVolaSurfaceDataID, CSSP.volaSurfaceDataID) 
			and type = isnull(@iType, type) 
			and CSSP.expiryID = isnull(@iExpiryID, CSSP.expiryID)
			--and E.expiryID = isnull(@iExpiryID, E.expiryID)
	end

	set @error = @@error
	return(@error)

GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureExchangesByUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MmFutureExchangesByUnderlying_Get]
GO


CREATE PROC dbo.usp_MmFutureExchangesByUnderlying_Get
	@iUnderlyingID int
as
	set nocount on

	select e.exchangeID   as iExchangeID,
	       e.isUnderlying as tiIsUnderlying,
	       e.isOption     as tiIsOption,
	       f.FutureID   as iFutureID	
	from future f
	inner join FutureRoot fr on fr.futureRootID = f.futureRootID and fr.underlyingID = @iUnderlyingID
	inner join Contract u on u.contractId = fr.underlyingID 
	inner join ContractExchangeLinkView e on f.FutureId = e.ContractID
 
	return @@error
GO


GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmFutureOptionsByUnderlying_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MmFutureOptionsByUnderlying_Get]
GO


CREATE PROC dbo.usp_MmFutureOptionsByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if @dtMinExpiry is null 
		begin
			select
				foc.contractID as iContractID,
				foc.contractTypeID as iContractTypeID,
				foc.symbol as vcSymbol,
				fo.strike as fStrike, 
				fec.expiryDate as dtExpiry,
				fo.isCall as tiIsCall,
				f.futureID as iFutureID
			from Future f
				inner join ContractView fc on f.futureID = fc.contractID
				inner join FutureRoot fr on fr.futureRootID = f.futureRootID and fr.underlyingID = @iUnderlyingID
				inner join FutureOption fo on f.futureID = fo.futureID
				inner join ContractView foc on fo.futureOptionID = foc.contractID
				inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = foc.expCalendarID
		end
	else
		begin
			select
				foc.contractID as iContractID,
				foc.contractTypeID as iContractTypeID,
				foc.symbol as vcSymbol,
				fo.strike as fStrike, 
				fec.expiryDate as dtExpiry,
				fo.isCall as tiIsCall,
				f.futureID as iFutureID
			from Future f
				inner join ContractView fc on f.futureID = fc.contractID
				inner join FutureRoot fr on fr.futureRootID = f.futureRootID and fr.underlyingID = @iUnderlyingID
				inner join FutureOption fo on f.futureID = fo.futureID
				inner join ContractView foc on fo.futureOptionID = foc.contractID
				inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = foc.expCalendarID
			where  fec.expiryDate >= @dtMinExpiry
		end 

	return @@error

GO

ALTER  PROC dbo.usp_MmFutureByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null	
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @dtMinExpiry is null set @dtMinExpiry = 0	

	select
		f.futureID as iContractID,
		fc.contractTypeID as iContractTypeID,
		fc.symbol as vcSymbol,		
--                e.ExpiryPeriod as vcMaturity,
                fec.expiryDate as dtMaturity,
		fu.ContractId as iFutureUnderlyingID,
                f.futureRootID as iFutureRootID
	from ContractView fu
                inner join FutureRoot fr on fr.underlyingID = fu.ContractId
                inner join Future f on f.futureRootID = fr.futureRootID
		inner join ContractView fc on f.futureID = fc.ContractId
--                inner join Expiry e on e.expiryID = f.MaturityID
                inner join ExpiryInCalendarView fec on f.MaturityID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
	where fu.ContractId = @iUnderlyingID
		and fec.expiryDate >= @dtMinExpiry		

	return @@error

GO
GO


ALTER  PROC dbo.usp_MmContractForTradeByUnd_Get
	@iUnderlyingID int
as
	-- return 2 recordsets: 1st for option contracts, 2nd futures & futures options contracts
	set nocount on

	declare @dtToday datetime

	set @dtToday = cast(floor(cast(getdate() as float)) as datetime)

	-- options rowset
	select	
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		c.contractName as vcContractName, 
		cp.priceClose as fPriceClose,
		cp.priceTheo as fPriceTheoClose,
		
		ec.expiryDate as dtExpiry,
		o.strike as fStrike,
		o.isCall as tiIsCall,
		ort.optionRootID as iOptionRootID
	from OptionRootView ort
		inner join OptionView o on o.optionRootID = ort.optionRootID
		inner join ContractView c on c.contractID = o.optionID
		inner join Contract uc on uc.contractID = ort.underlyingID
		inner join ExpiryInCalendarView ec on ec.expiryID = o.expiryID and ec.expCalendarID = uc.expCalendarID
		left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
	where
		ort.underlyingID = @iUnderlyingID
		and ec.expiryDate >= @dtToday
	order by year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc

	-- futures & futures options rowset
	select * from
	(
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName, 
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			c.symbol as vcFutureSymbol
		from FutureRoot fr
			inner join Future f on f.futureRootID = fr.futureRootID
			inner join ContractView c on c.contractID = f.futureID
			left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
		where
			fr.underlyingID = @iUnderlyingID
		union
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName, 
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
			ec.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			fc.symbol as vcFutureSymbol
		from FutureRoot fr
			inner join Future f on f.futureRootID = fr.futureRootID
			inner join Contract fc on fc.contractID = f.futureID
			inner join FutureOption o on o.futureID = f.futureID
			inner join ContractView c on c.contractID = o.futureOptionID
			inner join ExpiryInCalendarView ec on ec.expiryID = o.expiryID and ec.expCalendarID = c.expCalendarID
			left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where
			fr.underlyingID = @iUnderlyingID
			and ec.expiryDate >= @dtToday
	) as c order by c.vcFutureSymbol, year(c.dtExpiry), month(c.dtExpiry), c.fStrike, c.tiIsCall desc

	return @@error

GO
ALTER  PROC dbo.usp_MmFutureRoot_Get
	@iUnderlyingID int = null
AS
	-- retrieve future root(s) by underlying
	set nocount on

	if @iUnderlyingID is null
		select
			fr.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.futureRootName as vcFutureRootName,
			fr.futureLotSize as iFutureLotSize,
			fr.optionLotSize as iOptionLotSize,
			fr.exportSymbol as vcExportSymbol,
			fr.matCalendarID as iMatCalendarID,
			fr.iQuotationUnitID as iQuotationUnitID,
			fr.vcQuotationUnitCurrency as vcQuotationUnitName,
			fr.fTickSize		 as fTickSize,
			fr.fTickValue		 as fTickValue,
			fr.tiQuoteFormat 	 as tiQuoteFormat,
			fr.iQuotationUnitAmmount as iQuotationUnitAmmount
		from FutureRoot fr
			inner join ContractView uc on fr.underlyingID = uc.contractID
	else
		select
			fr.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			fr.futureRootName as vcFutureRootName,
			fr.futureLotSize as iFutureLotSize,
			fr.optionLotSize as iOptionLotSize,
			fr.exportSymbol as vcExportSymbol,
			fr.matCalendarID as iMatCalendarID,
			fr.iQuotationUnitID as iQuotationUnitID,
			fr.vcQuotationUnitCurrency as vcQuotationUnitName,
			fr.fTickSize		 as fTickSize,
			fr.fTickValue		 as fTickValue,
			fr.tiQuoteFormat 	 as tiQuoteFormat,
			fr.iQuotationUnitAmmount as iQuotationUnitAmmount
		from FutureRoot fr
			inner join ContractView uc on fr.underlyingID = uc.contractID
		where
			fr.underlyingID = @iUnderlyingID

GO

ALTER   PROC dbo.usp_MmUnderlying_Get
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
		
		    c.priceClose as fPriceClose,

			null as iTraderID,

			case when i.indexID is null then s.skew else i.skew end as fSkew,
			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    c.volume10Day as iVolume10Day,
		    c.volume5Expiration as iVolume5Expiration
			from ContractPriceView c 
		    --inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		where
			(c.contractTypeID < 3 or c.contractTypeID = 6) AND c.ActionId < 3
		order by c.Symbol
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
		
		    c.priceClose as fPriceClose,

			tc.traderID as iTraderID,

			case when i.indexID is null then s.skew else i.skew end as fSkew,
			case when i.indexID is null then s.kurt else i.kurt end as fKurt,
		    c.contractName as vcContractName,
			
			c.undPriceProfileID as iUndPriceProfileID,
			c.optPriceProfileID as iOptPriceProfileID,
			case when i.indexID is null then s.isActive else i.isActive end as tiIsActive,

		    c.volume10Day as iVolume10Day,
		    c.volume5Expiration as iVolume5Expiration
		from ContractPriceView c 
		   -- inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		    left join StockView s on s.stockID = c.contractID
		    left join IndexView i on i.indexID = c.contractID
		    left join TraderContractView tc on c.contractID = tc.contractID and tc.traderID = @iTraderID
		where
			(c.contractTypeID < 3 or c.contractTypeID = 6) AND c.ActionId < 3
		order by c.Symbol
	return @@error

GO
GO


ALTER   PROC dbo.usp_MmFutureByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null	
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @dtMinExpiry is null set @dtMinExpiry = 0	

	select
		f.futureID as iContractID,
		fc.contractTypeID as iContractTypeID,
		fc.symbol as vcSymbol,		
--                e.ExpiryPeriod as vcMaturity,
                fec.expiryDate as dtMaturity,
		fu.ContractId as iFutureUnderlyingID,
                f.futureRootID as iFutureRootID,
		isnull(ff.tiGreeksCalculationType, 1) as tiGreeksCalculationType
		
	from ContractView fu
                inner join FutureRoot fr on fr.underlyingID = fu.ContractId
                inner join Future f on f.futureRootID = fr.futureRootID
		inner join ContractView fc on f.futureID = fc.ContractId
--                inner join Expiry e on e.expiryID = f.MaturityID
                inner join ExpiryInCalendarView fec on f.MaturityID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
		left join FutureUnderlying ff on ff.iFutureUnderlyingID = fu.ContractId
	where fu.ContractId = @iUnderlyingID
		and fec.expiryDate >= @dtMinExpiry		

	return @@error


GO
ALTER PROC dbo.usp_BoUnderlyings_Get
AS
	------------------------------------------------
	set nocount on
	------------------------------------------------	
	select * from (
	select
		contractID as iContractID, 
		contractTypeID as iContractTypeID, 
		symbol as vcSymbol
	from ContractView
	where contractTypeID < 3
	union
	select
		contractID as iContractID, 
		contractTypeID as iContractTypeID, 
		symbol as vcSymbol
	from ContractView
	where contractTypeID =6
	
	) c
	order by c.vcSymbol

	return @@error
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.178')
	begin
		update DataInfo set keyValue = '3.46.178' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.178' '3.46.178 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO