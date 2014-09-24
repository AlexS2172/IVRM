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
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO


Alter PROCEDURE dbo.usp_EodOptionWithPos_Get  
 @iUnderlyingID int = null,  
 @dtMinExpDate datetime = Null  
AS  
 -------------------------------------------------  
 set nocount on  
 -------------------------------------------------  
  
 if @dtMinExpDate is null set @dtMinExpDate = getdate()  
  
 select distinct  
  ort.underlyingID as iUnderlyingID,  
  ort.optionRootID as iOptionRootID,  
  ort.symbol as vcOptionRootName,  
  ort.IsSynthetic as tiIsSynthetic,   
  ort.lotSize as iLotSize,   
  sor.cashValue as fCash,  
  sor.synthSkew as fSyntheticSkew,   
  sor.synthKurt as fSyntheticKurt,  
  sor.synthYield as fSyntheticYield,   
  sor.synthIsBasket as tiSyntheticIsBasket,  
  c.contractID as iOptionID,  
  c.symbol as vcSymbol,  
  o.strike as fStrike,   
  ec.expiryDate as dtExpiry,  
  o.isCall as tiIsCall,  
     cp.priceClose as fPriceClose  
 from  
  [Option] o  
  inner join Contract c on c.contractID = o.optionID  
  inner join OptionRoot ort on o.optionRootID = ort.optionRootID  
  inner join Contract uc on ort.underlyingID = uc.contractID  
  inner join ExpiryInCalendar ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID  
  left join SyntheticOptionRoot as sor on ort.optionRootID = sor.optionRootID  
  left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null        
 where  
  o.actionID < 3  
  and c.contractTypeID = 3  
  and ort.underlyingID = isnull(@iUnderlyingID, ort.underlyingID)  
  and ec.expiryDate >= @dtMinExpDate  
 order by ort.underlyingID,ort.optionRootID  
  
Go

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.175')
	begin
		update DataInfo set keyValue = '4.49.175' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.175' '4.49.175 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

