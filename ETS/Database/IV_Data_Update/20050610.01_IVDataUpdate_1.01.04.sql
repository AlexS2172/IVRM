use ETS_ASP_IV


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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '1.1.3'  or (keyValue = '1.1.4') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '1.1.4 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '1.1.4' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

Go
ALTER   PROCEDURE dbo.usp_InputOptionData
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
  where @vUnderlyingID = ilVSymbolID --and @vUnderlyingID = iUnderlyingID
  if @Symbol_Id is null
  Begin
      rollback
      return -1
  End

  if ( exists ( select top 1 * from  ETS_ASP_IV..IVSymbolData where @Symbol_Id = ISymbolID ) )
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
   
   update IVSymbolPrice set
	mBidPrice  = @vBidPrice ,
	mAskPrice  = @vAskPrice ,
	dtBidPriceTime = @vBidTime,
	dtAskPriceTime = @vAskTime,
	cBidExchange = @vBidExchange,
	cAskExchange = @vAskExchange,
	dVolume = @vVolume
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
  End

  if @@ERROR <> 0
  Begin
      Rollback
      return -3
  End
 
  if @@ERROR <> 0
  Begin
      Rollback
      return -3
  End

 
commit

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '1.1.4')
	begin
		update DataInfo set keyValue = '1.1.4' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 1.1.4' '1.1.4 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

