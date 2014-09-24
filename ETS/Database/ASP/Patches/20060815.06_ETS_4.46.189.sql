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
--BEGIN TRANSACTION
--GO
--SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.188'  or (keyValue = '3.46.189') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.189 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.189' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 --BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*BEGIN DATA MODIFICATION SECTION*/

/*usp_AspEodOptionsClosePricesFromIV_Update*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodOptionsClosePricesFromIV_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AspEodOptionsClosePricesFromIV_Update]
go
create PROC usp_AspEodOptionsClosePricesFromIV_Update 
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	UPDATE ContractPrice 
		SET PriceClose = (IV.Bid+IV.Ask)/2
	FROM  ContractPrice CP
		INNER join Contract C ON C.ContractID = CP.ContractID and C.contractTypeId = 3
		INNER join IVData.IV.Dbo.FT_Options_EOD IV on IV.option_ticker = C.symbol
	
	return @@error
go

/*usp_AspEodStocksClosePricesFromIV_Update*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodStocksClosePricesFromIV_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AspEodStocksClosePricesFromIV_Update]
go
create PROC usp_AspEodStocksClosePricesFromIV_Update 
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	UPDATE ContractPrice  SET
		[PriceClose] = IV.Price 
  	FROM ContractPrice CP
		INNER join Contract C ON C.ContractID = CP.ContractID and C.contractTypeId in (1,2)
		INNER join IVData.IV.Dbo.FT_Stocks_EOD IV ON IV.symbol = C.symbol

	return @@error
go

/*usp_DefaultIVSurfaceFutureUnd_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DefaultIVSurfaceFutureUnd_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_DefaultIVSurfaceFutureUnd_Get]
go
create proc usp_DefaultIVSurfaceFutureUnd_Get
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
go

/*usp_DefaultIVSurfaceStock_Get*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DefaultIVSurfaceStock_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_DefaultIVSurfaceStock_Get]
go
create proc usp_DefaultIVSurfaceStock_Get
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
go

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

/*END DATA MODIFICATION SECTION*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.189')
	begin
		update DataInfo set keyValue = '3.46.189' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.189' '3.46.189 update message:' 
	select 'The database update succeeded'

--	COMMIT TRANSACTION
--END
--	ELSE select 'The database update failed'
--GO
DROP TABLE #tmpErrors
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
