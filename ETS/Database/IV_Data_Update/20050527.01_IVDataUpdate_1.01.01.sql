SET XACT_ABORT ON
GO
-- Creating script

IF not EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'ETS_ASP_IV')
  CREATE DATABASE [ETS_ASP_IV]  ON (NAME = N'ETS_ASP_IV', 
	FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL\Data\ETS_ASP_IV.mdf' , 
	SIZE = 10, MAXSIZE = 1000, FILEGROWTH = 5) 
  LOG ON (NAME = N'ETS_ASP_IV_log', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL\Data\ETS_ASP_IV.ldf' , SIZE = 5, MAXSIZE = 1000, FILEGROWTH = 2)
  GO


use ETS_ASP_IV

if exists (select * from dbo.sysobjects where id = object_id(N'[IVSymbolPrice]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [IVSymbolPrice]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[IVSymbolData]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [IVSymbolData]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[IVSymbol]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [IVSymbol]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[datainfo]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [datainfo]
GO

CREATE TABLE [DataInfo] (
	[dataInfoID] [int] NOT NULL ,
	[keyValue] [nvarchar] (3000) COLLATE Latin1_General_CI_AS NOT NULL 
) ON [PRIMARY]
GO

insert into DataInfo ( dataInfoID ,  keyValue )
values ( 1 , '1.1.1' )




CREATE TABLE [IVSymbol] (
	[iSymbolID] [int] IDENTITY (1, 1) NOT NULL ,
	[ilVSymbolID] [int] NOT NULL ,
	[vcSymbolName] [char] (10) COLLATE Latin1_General_CI_AS NOT NULL ,
	[tiSymbolType] [char] (1) COLLATE Latin1_General_CI_AS NOT NULL ,
	[iUnderlyingID] [int] NULL ,
	CONSTRAINT [PK_IVSymbol] PRIMARY KEY  CLUSTERED 
	(
		[iSymbolID]
	)  ON [PRIMARY] ,
	CONSTRAINT [FK_IVSymbol_IVSymbol] FOREIGN KEY 
	(
		[iUnderlyingID]
	) REFERENCES [IVSymbol] (
		[iSymbolID]
	)
) ON [PRIMARY]
GO


CREATE TABLE [IVSymbolData] (
	[iSymbolID] [int] NOT NULL ,
	[dModelIV] [float] NOT NULL ,
	[dIV] [float] NOT NULL ,
	[dDelta] [float] NOT NULL ,
	[dGamma] [float] NOT NULL ,
	[dVega] [float] NOT NULL ,
	[dThetta] [float] NOT NULL ,
	[dRho] [float] NOT NULL ,
	CONSTRAINT [FK_IVSymbolData_IVSymbol] FOREIGN KEY 
	(
		[iSymbolID]
	) REFERENCES [IVSymbol] (
		[iSymbolID]
	)
) ON [PRIMARY]
GO



CREATE TABLE [IVSymbolPrice] (
	[iSymbolID] [int] NOT NULL ,
	[mBidPrice] [money] NOT NULL ,
	[mAskPrice] [money] NOT NULL ,
	[mLastPrice] [money] NOT NULL ,
	[dSize] [int] NOT NULL ,
	[dLastSize] [int] NOT NULL ,
	[dtBidPriceTime] [datetime] NOT NULL ,
	[dtAskPriceTime] [datetime] NOT NULL ,
	[dtLastPriceTime] [datetime] NULL ,
	[cBidExchange] [char] (1) COLLATE Latin1_General_CI_AS NOT NULL ,
	[cAskExchange] [char] (1) COLLATE Latin1_General_CI_AS NOT NULL ,
	[cLastPriceExchange] [char] (1) COLLATE Latin1_General_CI_AS NOT NULL ,
	[dVolume] [int] NOT NULL ,
	CONSTRAINT [FK_IVSymbolPrice_IVSymbol] FOREIGN KEY 
	(
		[iSymbolID]
	) REFERENCES [IVSymbol] (
		[iSymbolID]
	)
) ON [PRIMARY]
GO





if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DatabaseCleanUp]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
  drop procedure [dbo].[usp_DatabaseCleanUp]
GO

Create Procedure dbo.usp_DatabaseCleanUp 
As
 SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
 Begin Tran
    delete IVSymbolData
    if @@ERROR <> 0
    Begin
      Rollback
      return -2
    End
    delete IVSymbolPrice
    if @@ERROR <> 0
    Begin
      Rollback
      return -3
    End
    delete IVSymbol
    if @@ERROR <> 0
    Begin
      Rollback
      return -1
    End

 commit
 return 0
Go

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_InputOptionData]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
  drop procedure [dbo].[usp_InputOptionData]
GO


CREATE  PROCEDURE dbo.usp_InputOptionData
-- Created by Artem Shylaev Egar T
-- 2005 - 04 - 06
  @vOptionID int,
  @vUnderlyingID int,
  @vModelIV float, 
  @vIV float,
  @vDelta float,
  @vGamma float,
  @vVega float,
  @vThetta float,
  @vRho float,
  @vBidPrice money,
  @vAskPrice money,
  @vBidSize int,
  @vAskSize int,
  @vBidTime  datetime,
  @vAskTime  datetime,
  @vBidExchange char,
  @vAskExchange char,
  @vVolume float
AS
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
Begin Tran
  declare @Symbol_Id int
  select top 1 @Symbol_Id = iSymbolID
    from IVSymbol 
  where @vOptionID  = ilVSymbolID --and @vUnderlyingID = iUnderlyingID
  if @Symbol_Id is null
  Begin
      rollback
      return -1
  End

  if ( exists ( select top 1 * from  IVSymbolData where @Symbol_Id = ISymbolID ) )
  begin
   update IVSymbolData set
	dModelIV = @vModelIV,
	dIV = @vIV,
	dDelta = @vDelta,
	dGamma = @vGamma,
	dVega = @vVega,
	dThetta = @vThetta,
	dRho = @vRho
   where @Symbol_Id = ISymbolID
  End
  else
  Begin
   insert into IVSymbolData
   (
     ISymbolID,
     dModelIV,
     dIV,
     dDelta,
     dGamma,
     dVega,
     dThetta,
     dRho
   )
   values
   (
     @Symbol_Id,
     @vModelIV ,
     @vIV,
     @vDelta,
     @vGamma,
     @vVega,
     @vThetta,
     @vRho
   )
  End
  if @@ERROR <> 0
  Begin
      Rollback
      return -3
  End
  insert into dbo.IVSymbolPrice
  (
	iSymbolID,
	mBidPrice,
	mAskPrice,
	mLastPrice,
	dSize,
	dLastSize,
	dtBidPriceTime,
	dtAskPriceTime,
	dtLastPriceTime,
	cBidExchange,
	cAskExchange,
	cLastPriceExchange,
	dVolume
  )
  values
  (
	@Symbol_Id,
	@vBidPrice,
	@vAskPrice,
	0,
	0,
	0,
	@vBidTime,
	@vAskTime,
	NULL,
	@vBidExchange,
	@vAskExchange,
	0,
	@vVolume
  )

  if @@ERROR <> 0
  Begin
      Rollback
      return -3
  End

 
commit
Go

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_InputStockData]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
  drop procedure [dbo].[usp_InputStockData]
GO


CREATE  PROCEDURE dbo.usp_InputStockData
-- Created by Artem Shylaev Egar T
-- 2005 - 04 - 06
  @vSymbol_Id int,
  @vBidPrice money,
  @vAskPrice money,
  @vBidSize int,
  @vAskSize int,
  @vBidTime  datetime,
  @vAskTime  datetime,
  @vBidExchange char,
  @vAskExchange char,
  @vVolume float
AS

 
  if @vSymbol_Id is null
  Begin
      rollback
      return -1
  End

  declare @vSymbolDBID int
  select @vSymbolDBID = iSymbolID from dbo.IVSymbol where ilVSymbolID = @vSymbol_Id
  if @vSymbolDBID is null
   return 

SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
Begin Tran  
  insert into dbo.IVSymbolPrice
  (
	iSymbolID,
	mBidPrice,
	mAskPrice,
	mLastPrice,
	dSize,
	dLastSize,
	dtBidPriceTime,
	dtAskPriceTime,
	dtLastPriceTime,
	cBidExchange,
	cAskExchange,
	cLastPriceExchange,
	dVolume
  )
  values
  (
	@vSymbolDBID ,--@vSymbol_Id,
	@vBidPrice,
	@vAskPrice,
	0,
	0,
	0,
	@vBidTime,
	@vAskTime,
	NULL,
	@vBidExchange,
	@vAskExchange,
	0,
	@vVolume
  )

  if @@ERROR <> 0
  Begin
      Rollback
      return -3
  End

 
commit
Go


if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExportDataFromIV]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
  drop procedure [dbo].[usp_ExportDataFromIV]
GO


Create Procedure dbo.usp_ExportDataFromIV
-- Created by Artem Shylaev Egar T
-- 2005 - 04 - 08
As

--Begin Tran
   exec usp_DatabaseCleanUp
   
   insert into IVSymbol
    (
	ilVSymbolID,
	vcSymbolName,
	tiSymbolType,
	iUnderlyingID
     )
    select 
         stock_id As  ilVSymbolID,
         symbol   As  vcSymbolName,
         1        As  tiSymbolType,
         NULL     As  iUnderlyingID
    from IVolatility.RTScanner.dbo.v_Stock

    if @@ERROR <> 0
    Begin
--        Rollback
        return -3
    End

    insert into IVSymbol
     (
	ilVSymbolID,
	vcSymbolName,
	tiSymbolType,
	iUnderlyingID
      )
     select 
        O.option_id As  ilVSymbolID,
        O.symbol    As  vcSymbolName,
        2           As  tiSymbolType,
        S.iSymbolID As  iUnderlyingID
     from IVolatility.RTScanner.dbo.v_option O
        join IVSymbol S on S.vcSymbolName = O.root COLLATE SQL_Latin1_General_CP1_CI_AS 
     if @@ERROR <> 0
     Begin
--        Rollback
        return -3
     End

--   Commit
Go


Go

Go

-- end of creating


 
GO
