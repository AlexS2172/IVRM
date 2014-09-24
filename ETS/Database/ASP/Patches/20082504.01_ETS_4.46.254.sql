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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.46.253' or (keyValue = '4.46.254') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '4.46.254 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '4.46.254' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION SECTION*/


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



ALTER           Procedure dbo.usp_OptionVM_Get
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
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if @dtExpDateMin is null set @dtExpDateMin = 0

	if @iSortOrder = 1	-- sort by symbol
		select	O.OptionID as iOptionID, 
				O.optionRootID as iOptionRootID, 
				ORt.underlyingID as iUnderlyingID,
				isCall as bIsCall, 
				strike as fStrike, 
				O.expiryDate as dtExpDate,
				C.symbol as vcSymbolName, 
				null as iExchangeID,
				C.contractName as vcDescription,
				ORt.isFitEnabled as tiIsFitEnabled,
				ORt.lotSize as iLotSize,
				ORt.isSynthetic as tiIsSynthetic,
				O.expiryOV as dtExpiryOV,
				O.TradingClose as dtTradingClose
				from OptionView O
						join ContractView C on O.OptionID = C.contractID
						join ContractPrice CP on CP.contractID = C.contractID and exchangeID is null
						join OptionRoot ORt on O.optionRootID = ORt.optionRootID
				where ORt.underlyingID = @iUnderlyingID
						and (@iOptionID is null or O.OptionID = @iOptionID)
						and datediff(d,@dtExpDateMin, O.expiryDate) >= 0
						and (@bIsCall is null or O.isCall = @bIsCall)
						and  O.isManualyEntered  = 0
				order by C.symbol
	else
	if @iSortOrder = 2	-- sort by isCall+year(expiry)+month(expiry)+strike

		select	O.OptionID as iOptionID, 
				O.optionRootID as iOptionRootID, 
				ORt.underlyingID as iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				O.expiryDate as dtExpDate,
				C.symbol as vcSymbolName, 
				null iExchangeID,
				C.contractName as vcDescription, 
				ORt.isFitEnabled as tiIsFitEnabled,
				ORt.lotSize as iLotSize,
				ORt.isSynthetic as tiIsSynthetic,
				O.expiryOV as dtExpiryOV,
				O.TradingClose as dtTradingClose
			from OptionView O
					join ContractView C on O.OptionID = C.contractID
					join ContractPrice on ContractPrice.contractID = C.contractID and exchangeID is null
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
			where ORt.underlyingID = @iUnderlyingID
					and (@iOptionID is null or O.OptionID = @iOptionID)
					and datediff(d, @dtExpDateMin, O.expiryDate) >=0
					and (@bIsCall is null or O.isCall = @bIsCall)
					and  O.isManualyEntered  = 0
			order by isCall, O.expiryDate, /*year(EC.expiryDate),  month(EC.expiryDate),*/ strike
	else
	if @iSortOrder = 3	-- sort by expiry+strike+symbol(without two last symbols)+isCall
		select 	O.OptionID iOptionID,
				O.optionRootID as iOptionRootID, 
				ORt.underlyingID iUnderlyingID, 
				isCall as bIsCall, 
				strike as fStrike, 
				O.expiryDate as dtExpDate,
				C.symbol vcSymbolName, 
				null iExchangeID,
				C.contractName vcDescription,	
				ORt.isFitEnabled as tiIsFitEnabled,
				ORt.lotSize iLotSize,
				ORt.isSynthetic as tiIsSynthetic,
				O.expiryOV as dtExpiryOV,
				O.TradingClose as dtTradingClose
			from OptionView O
					join ContractView C on O.OptionID = C.contractID
					join ContractPrice on ContractPrice.contractID = C.contractID and exchangeID is null
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					--join Contract C1 on ORt.underlyingID = C1.contractID
					--join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = isnull( C.expCalendarID, C1.expCalendarID )
			where ORt.underlyingID = @iUnderlyingID
					and (@iOptionID is null or O.OptionID = @iOptionID)
					and datediff(d,@dtExpDateMin,O.expiryDate) >= 0
					and (@bIsCall is null or O.isCall = @bIsCall)
					and  O.isManualyEntered  = 0
			order by O.expiryDate, strike, left(C.symbol, len(C.symbol) - 2), isCall desc
	set @error = @@error
Return(@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

/*DATA MODIFICATION COMPLETE*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.46.254')
	begin
		update DataInfo set keyValue = '4.46.254' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 ROLLBACK TRANSACTION
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
	select 'DB VERSION WAS CHANGED TO 4.46.254' '4.46.254 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


