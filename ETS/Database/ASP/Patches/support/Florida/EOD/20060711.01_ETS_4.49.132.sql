/*
	Contains stored procedures for ETS ASP (Institutional) database
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
--BEGIN TRANSACTION
GO
--SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.131' or (keyValue = '4.49.132')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.132 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.132' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Commodity]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[Commodity]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_FutureUnderlying_FutureUnderlyingType]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[FutureUnderlying] DROP CONSTRAINT FK_FutureUnderlying_FutureUnderlyingType
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_FutureRoot_QuotationUnit]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[FutureRoot] DROP CONSTRAINT FK_FutureRoot_QuotationUnit
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FutureUnderlying]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[FutureUnderlying]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FutureUnderlyingType]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[FutureUnderlyingType]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[QuotationUnit]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[QuotationUnit]
GO

CREATE TABLE [dbo].[FutureUnderlying] (
	[iFutureUnderlyingID] [int] NOT NULL ,
	[iFutureUnderlyingTypeID] [int] NOT NULL ,
	[tiGreeksCalculationType] [tinyint] NOT NULL ,
	[tiActive] [tinyint] NOT NULL ,
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[FutureUnderlyingType] (
	[iFutureUnderlyingTypeID] [int] IDENTITY (1, 1) NOT NULL ,
	[vcName] [varchar] (255) COLLATE Latin1_General_CI_AS NOT NULL, 
	[vcDesc] [varchar] (255) COLLATE Latin1_General_CI_AS NULL
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[QuotationUnit] (
	[iQuotationUnitID] [int] IDENTITY (1, 1) NOT NULL ,
	[vcName] [varchar] (255) COLLATE Latin1_General_CI_AS NOT NULL ,
	[vcDesc] [varchar] (255) COLLATE Latin1_General_CI_AS NULL 
) ON [PRIMARY]
GO

GO
if Not Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and COLUMN_NAME='tiQuoteFormat')
ALTER TABLE [dbo].[FutureRoot]  
	ADD [tiQuoteFormat] [tinyint] NULL
GO

if Not Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and COLUMN_NAME='iQuotationUnitAmmount')
ALTER TABLE [dbo].[FutureRoot]  
	ADD [iQuotationUnitAmmount] [int] NULL
GO

if Not Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and COLUMN_NAME='iQuotationUnitAmmount')
ALTER TABLE [dbo].[FutureRoot]  
	ADD [iQuotationUnitAmmount] [int] NULL
GO

if Not Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and COLUMN_NAME='fQuoteDenomination')
ALTER TABLE [dbo].[FutureRoot]  
	ADD [fQuoteDenomination] [float] NULL 
GO

if Not Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and COLUMN_NAME='fTickSize')
ALTER TABLE [dbo].[FutureRoot]  
	ADD [fTickSize] [float] NULL
GO

if Not Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and COLUMN_NAME='fTickValue')
ALTER TABLE [dbo].[FutureRoot]  
	ADD [fTickValue] [float] NULL
GO

if Not Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and COLUMN_NAME='iQuotationUnitID')
ALTER TABLE [dbo].[FutureRoot]  
	ADD [iQuotationUnitID] [int] NULL
GO

if Not Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='FutureRoot' and COLUMN_NAME='vcQuotationUnitCurrency')
ALTER TABLE [dbo].[FutureRoot]  
	ADD [vcQuotationUnitCurrency] nvarchar(20) NULL
GO



SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER   PROC dbo.usp_EodFuture_Import
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
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DefaultIVSurfaceStock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_DefaultIVSurfaceStock_Get]
GO

create proc dbo.usp_DefaultIVSurfaceStock_Get
	@iUnderlyingID int
as
if @iUnderlyingID is not null
begin
	select	cv.ContractID,
		CONVERT(datetime, IV.dtExpDate, 112) as dtExpDate,
		(DATEPART(yyyy, IV.dtExpDate)-2000)*12+DATEPART(m, IV.dtExpDate) as iExpiryID,
		IV.fStrike,
		IV.fIV,
		GetDate() as dtActionDate
	from IVData.IV.Dbo.FT_DefaultIV IV
		inner join ContractView cv on cv.Symbol = IV.vcSymbol and cv.ContractTypeID in (1,2)
			and cv.ContractID = @iUnderlyingID
end
return @@error

GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DefaultIVSurfaceFutureUnd_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_DefaultIVSurfaceFutureUnd_Get]
GO

create proc dbo.usp_DefaultIVSurfaceFutureUnd_Get
	@iFutUnderlyingID int
as
if @iFutUnderlyingID is not null
begin
	select	QQ.ContractID as ContractID,
		CONVERT(datetime, IV.dtExpDate, 112) as dtExpDate,
		(DATEPART(yyyy, IV.dtExpDate)-2000)*12+DATEPART(m, IV.dtExpDate) as iExpiryID,
		IV.fStrike as fStrike,
		cast(IV.fIV as float) as fIV,
		GetDate() as dtActionDate
	from	(select	CU.ContractID,
			IV.fStrike,
			IV.dtExpDate,
			FR.UnderlyingID,
			max(FR.futureLotSize) as maxLosSize,
			count(FR.futureRootID) as fRootsNumber
		from FutureRoot FR
			inner join IVData.IV.Dbo.FT_DefaultIV_Futures IV on IV.vcSymbol = FR.futureRootSymbol
			inner join ContractView CU on CU.ContractID = FR.underlyingID and CU.ContractTypeID = 6
				and CU.ContractID = @iFutUnderlyingID
		group by IV.fStrike, IV.dtExpDate, FR.UnderlyingId, CU.ContractID
		) QQ
		inner join FutureRoot GH on GH.underlyingID = QQ.UnderlyingID and GH.futureLotSize = QQ.maxLosSize
		inner join IVData.IV.Dbo.FT_DefaultIV_Futures IV on IV.vcSymbol = GH.futureRootSymbol and QQ.fStrike = IV.fStrike and QQ.dtExpDate = IV.dtExpDate
	order by QQ.UnderlyingID, QQ.dtExpDate, QQ.fStrike
end
return @@error

GO

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.132')
	begin
		update DataInfo set keyValue = '4.49.132' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
--IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
--GO
--IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
--GO
--IF EXISTS (SELECT * FROM #tmpErrors)
--BEGIN
--	ROLLBACK TRANSACTION
	--SELECT * FROM #tmpErrors
--END
--GO
--IF @@TRANCOUNT > 0 
--BEGIN
	select 'DB VERSION WAS CHANGED TO 4.49.132' '4.49.132 update message:' 
	select 'The database update succeeded'

--	COMMIT TRANSACTION
--END
--	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
-- unable to execute this script in transaction
