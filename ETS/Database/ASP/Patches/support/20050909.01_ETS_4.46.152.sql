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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.151'  or (keyValue = '3.46.152') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.152 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.152' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
if NOT EXISTS (select * from dbo.sysindexes where name = 'ContractPrice_PriceClose' and id = object_id('dbo.ContractPrice'))
BEGIN
CREATE NONCLUSTERED INDEX [ContractPrice_PriceClose] ON [dbo].[ContractPrice] ([contractID] ASC, [exchangeID] ASC, [priceClose] ASC )
CREATE NONCLUSTERED INDEX [Contract_CALENDAR] ON [dbo].[Contract] ([contractID] ASC, [contractTypeID] ASC, [symbol] ASC, [expCalendarID] ASC, [actionID] ASC, [undPriceProfileID] ASC, [optPriceProfileID] ASC )
CREATE NONCLUSTERED INDEX [Trade_Strategy] ON [dbo].[Trade] ([contractID] ASC, [strategyID] ASC )

CREATE STATISTICS [hind_88387384_2A_3A_5A] ON [dbo].[tradercontract] ([contractid], [traderid], [actionid])
CREATE STATISTICS [hind_184387726_1A_2A_3A_5A_6A_7A_9A_10A_11A_12A_13A_15A_16A_17A] ON [dbo].[stock] ([stockid], [divfreq], [ishtb], [divamt], [divdate], [actionid], [isdivcustom], [divamtcustom], [divdatecustom], [divfreqcustom], [calcoptiontype], [isactive], [skew], [kurt])
CREATE STATISTICS [hind_184387726_1A_7A_2A_3A_5A_6A_9A_10A_11A_12A_13A_15A_16A_17A] ON [dbo].[stock] ([stockid], [actionid], [divfreq], [ishtb], [divamt], [divdate], [isdivcustom], [divamtcustom], [divdatecustom], [divfreqcustom], [calcoptiontype], [isactive], [skew], [kurt])
CREATE STATISTICS [hind_304056169_10A_6A] ON [dbo].[trade] ([strategyid], [tradedate])
CREATE STATISTICS [hind_304056169_22A_6A] ON [dbo].[trade] ([underlyingid], [tradedate])
CREATE STATISTICS [hind_304056169_22A_13A] ON [dbo].[trade] ([underlyingid], [clearingbrokerid])
CREATE STATISTICS [hind_304056169_11A_6A] ON [dbo].[trade] ([traderid], [tradedate])
CREATE STATISTICS [hind_304056169_25A_11A] ON [dbo].[trade] ([futureid], [traderid])
CREATE STATISTICS [hind_304056169_25A_22A] ON [dbo].[trade] ([futureid], [underlyingid])
CREATE STATISTICS [hind_304056169_10A_3A] ON [dbo].[trade] ([strategyid], [tradeid])
CREATE STATISTICS [hind_304056169_22A_5A] ON [dbo].[trade] ([underlyingid], [contractid])
CREATE STATISTICS [hind_304056169_22A_12A] ON [dbo].[trade] ([underlyingid], [brokerid])
CREATE STATISTICS [hind_304056169_11A_5A] ON [dbo].[trade] ([traderid], [contractid])
CREATE STATISTICS [hind_304056169_12A_6A] ON [dbo].[trade] ([brokerid], [tradedate])
CREATE STATISTICS [hind_304056169_10A_5A] ON [dbo].[trade] ([strategyid], [contractid])
CREATE STATISTICS [hind_304056169_25A_10A] ON [dbo].[trade] ([futureid], [strategyid])
CREATE STATISTICS [hind_304056169_22A_11A] ON [dbo].[trade] ([underlyingid], [traderid])
CREATE STATISTICS [hind_304056169_22A_25A] ON [dbo].[trade] ([underlyingid], [futureid])
CREATE STATISTICS [hind_304056169_12A_5A] ON [dbo].[trade] ([brokerid], [contractid])
CREATE STATISTICS [hind_304056169_13A_6A] ON [dbo].[trade] ([clearingbrokerid], [tradedate])
CREATE STATISTICS [hind_304056169_6A_10A] ON [dbo].[trade] ([tradedate], [strategyid])
CREATE STATISTICS [hind_304056169_5A_22A] ON [dbo].[trade] ([contractid], [underlyingid])
CREATE STATISTICS [hind_304056169_5A_11A] ON [dbo].[trade] ([contractid], [traderid])
CREATE STATISTICS [hind_304056169_11A_10A] ON [dbo].[trade] ([traderid], [strategyid])
CREATE STATISTICS [hind_304056169_6A_22A] ON [dbo].[trade] ([tradedate], [underlyingid])
CREATE STATISTICS [hind_304056169_6A_11A] ON [dbo].[trade] ([tradedate], [traderid])
CREATE STATISTICS [hind_304056169_6A_3A] ON [dbo].[trade] ([tradedate], [tradeid])
CREATE STATISTICS [hind_304056169_5A_12A] ON [dbo].[trade] ([contractid], [brokerid])
CREATE STATISTICS [hind_304056169_12A_10A] ON [dbo].[trade] ([brokerid], [strategyid])
CREATE STATISTICS [hind_304056169_11A_22A] ON [dbo].[trade] ([traderid], [underlyingid])
CREATE STATISTICS [hind_304056169_10A_22A] ON [dbo].[trade] ([strategyid], [underlyingid])
CREATE STATISTICS [hind_304056169_25A_12A] ON [dbo].[trade] ([futureid], [brokerid])
CREATE STATISTICS [hind_304056169_25A_13A] ON [dbo].[trade] ([futureid], [clearingbrokerid])
CREATE STATISTICS [hind_304056169_22A_3A] ON [dbo].[trade] ([underlyingid], [tradeid])
CREATE STATISTICS [hind_304056169_6A_12A] ON [dbo].[trade] ([tradedate], [brokerid])
CREATE STATISTICS [hind_304056169_5A_13A] ON [dbo].[trade] ([contractid], [clearingbrokerid])
CREATE STATISTICS [hind_304056169_5A_3A] ON [dbo].[trade] ([contractid], [tradeid])
CREATE STATISTICS [hind_304056169_13A_10A] ON [dbo].[trade] ([clearingbrokerid], [strategyid])
CREATE STATISTICS [hind_304056169_12A_22A] ON [dbo].[trade] ([brokerid], [underlyingid])
CREATE STATISTICS [hind_304056169_12A_11A] ON [dbo].[trade] ([brokerid], [traderid])
CREATE STATISTICS [hind_304056169_11A_12A] ON [dbo].[trade] ([traderid], [brokerid])
CREATE STATISTICS [hind_304056169_10A_12A] ON [dbo].[trade] ([strategyid], [brokerid])
CREATE STATISTICS [hind_304056169_22A_10A] ON [dbo].[trade] ([underlyingid], [strategyid])
CREATE STATISTICS [hind_304056169_11A_3A] ON [dbo].[trade] ([traderid], [tradeid])
CREATE STATISTICS [hind_304056169_13A_5A] ON [dbo].[trade] ([clearingbrokerid], [contractid])
CREATE STATISTICS [hind_304056169_25A_6A] ON [dbo].[trade] ([futureid], [tradedate])
CREATE STATISTICS [hind_304056169_6A_13A] ON [dbo].[trade] ([tradedate], [clearingbrokerid])
CREATE STATISTICS [hind_304056169_6A_5A] ON [dbo].[trade] ([tradedate], [contractid])
CREATE STATISTICS [hind_304056169_5A_25A] ON [dbo].[trade] ([contractid], [futureid])
CREATE STATISTICS [hind_304056169_13A_22A] ON [dbo].[trade] ([clearingbrokerid], [underlyingid])
CREATE STATISTICS [hind_304056169_13A_11A] ON [dbo].[trade] ([clearingbrokerid], [traderid])
CREATE STATISTICS [hind_304056169_11A_13A] ON [dbo].[trade] ([traderid], [clearingbrokerid])
CREATE STATISTICS [hind_304056169_10A_13A] ON [dbo].[trade] ([strategyid], [clearingbrokerid])
CREATE STATISTICS [hind_304056169_12A_3A] ON [dbo].[trade] ([brokerid], [tradeid])
CREATE STATISTICS [hind_304056169_13A_25A] ON [dbo].[trade] ([clearingbrokerid], [futureid])
CREATE STATISTICS [hind_304056169_25A_5A] ON [dbo].[trade] ([futureid], [contractid])
CREATE STATISTICS [hind_304056169_12A_25A] ON [dbo].[trade] ([brokerid], [futureid])
CREATE STATISTICS [hind_304056169_6A_25A] ON [dbo].[trade] ([tradedate], [futureid])
CREATE STATISTICS [hind_304056169_25A_3A] ON [dbo].[trade] ([futureid], [tradeid])
CREATE STATISTICS [hind_304056169_13A_12A] ON [dbo].[trade] ([clearingbrokerid], [brokerid])
CREATE STATISTICS [hind_304056169_12A_13A] ON [dbo].[trade] ([brokerid], [clearingbrokerid])
CREATE STATISTICS [hind_304056169_11A_25A] ON [dbo].[trade] ([traderid], [futureid])
CREATE STATISTICS [hind_304056169_10A_25A] ON [dbo].[trade] ([strategyid], [futureid])
CREATE STATISTICS [hind_304056169_13A_3A] ON [dbo].[trade] ([clearingbrokerid], [tradeid])
CREATE STATISTICS [hind_304056169_10A_11A] ON [dbo].[trade] ([strategyid], [traderid])
CREATE STATISTICS [hind_304056169_5A_6A] ON [dbo].[trade] ([contractid], [tradedate])
CREATE STATISTICS [hind_304056169_3A_5A_6A] ON [dbo].[trade] ([tradeid], [contractid], [tradedate])
CREATE STATISTICS [hind_304056169_3A_5A_12A] ON [dbo].[trade] ([tradeid], [contractid], [brokerid])
CREATE STATISTICS [hind_304056169_5A_10A_12A] ON [dbo].[trade] ([contractid], [strategyid], [brokerid])
CREATE STATISTICS [hind_304056169_5A_10A_6A] ON [dbo].[trade] ([contractid], [strategyid], [tradedate])
CREATE STATISTICS [hind_304056169_5A_10A_13A] ON [dbo].[trade] ([contractid], [strategyid], [clearingbrokerid])
CREATE STATISTICS [hind_304056169_3A_5A_13A] ON [dbo].[trade] ([tradeid], [contractid], [clearingbrokerid])
CREATE STATISTICS [hind_304056169_5A_10A_3A] ON [dbo].[trade] ([contractid], [strategyid], [tradeid])
CREATE STATISTICS [hind_304056169_3A_5A_11A] ON [dbo].[trade] ([tradeid], [contractid], [traderid])
CREATE STATISTICS [hind_304056169_3A_5A_22A] ON [dbo].[trade] ([tradeid], [contractid], [underlyingid])
CREATE STATISTICS [hind_304056169_5A_10A_22A] ON [dbo].[trade] ([contractid], [strategyid], [underlyingid])
CREATE STATISTICS [hind_304056169_5A_10A_25A] ON [dbo].[trade] ([contractid], [strategyid], [futureid])
CREATE STATISTICS [hind_304056169_3A_5A_25A] ON [dbo].[trade] ([tradeid], [contractid], [futureid])
CREATE STATISTICS [hind_304056169_3A_5A_10A] ON [dbo].[trade] ([tradeid], [contractid], [strategyid])
CREATE STATISTICS [hind_304056169_5A_10A_11A] ON [dbo].[trade] ([contractid], [strategyid], [traderid])
CREATE STATISTICS [hind_2075870462_3A_10A] ON [dbo].[contract] ([symbol], [expcalendarid])
CREATE STATISTICS [hind_2075870462_10A_3A] ON [dbo].[contract] ([expcalendarid], [symbol])
CREATE STATISTICS [hind_2075870462_2A_10A] ON [dbo].[contract] ([contracttypeid], [expcalendarid])
CREATE STATISTICS [hind_2075870462_2A_1A] ON [dbo].[contract] ([contracttypeid], [contractid])
CREATE STATISTICS [hind_2075870462_2A_3A] ON [dbo].[contract] ([contracttypeid], [symbol])
CREATE STATISTICS [hind_2075870462_10A_2A] ON [dbo].[contract] ([expcalendarid], [contracttypeid])
CREATE STATISTICS [hind_2075870462_10A_1A] ON [dbo].[contract] ([expcalendarid], [contractid])
CREATE STATISTICS [hind_2075870462_1A_2A_10A] ON [dbo].[contract] ([contractid], [contracttypeid], [expcalendarid])
CREATE STATISTICS [hind_2075870462_1A_3A_10A] ON [dbo].[contract] ([contractid], [symbol], [expcalendarid])
CREATE STATISTICS [hind_2075870462_1A_3A_2A] ON [dbo].[contract] ([contractid], [symbol], [contracttypeid])
CREATE STATISTICS [hind_2075870462_1A_2A_3A_7A_10A_11A_12A] ON [dbo].[contract] ([contractid], [contracttypeid], [symbol], [actionid], [expcalendarid], [undpriceprofileid], [optpriceprofileid])
CREATE STATISTICS [hind_2075870462_1A_2A_3A_10A_11A_12A_7A] ON [dbo].[contract] ([contractid], [contracttypeid], [symbol], [expcalendarid], [undpriceprofileid], [optpriceprofileid], [actionid])
CREATE STATISTICS [hind_2139870690_3A_2A] ON [dbo].[contractprice] ([exchangeid], [contractid])
CREATE STATISTICS [hind_2139870690_8A_3A] ON [dbo].[contractprice] ([priceclose], [exchangeid])
CREATE STATISTICS [hind_2139870690_3A_8A] ON [dbo].[contractprice] ([exchangeid], [priceclose])
CREATE STATISTICS [hind_2139870690_8A_2A_3A] ON [dbo].[contractprice] ([priceclose], [contractid], [exchangeid])
CREATE STATISTICS [hind_2139870690_2A_8A_3A] ON [dbo].[contractprice] ([contractid], [priceclose], [exchangeid])
END
if exists (select * from dbo.sysindexes where name = N'I_BrokerName' and id = object_id(N'[dbo].[Broker]'))
drop index [dbo].[Broker].[I_BrokerName]
GO

GO
 ALTER TABLE Broker
	ALTER COLUMN brokerName nvarchar(64) NOT NULL
GO
 CREATE  INDEX [I_BrokerName] ON [dbo].[Broker]([brokerName]) WITH  FILLFACTOR = 90 ON [PRIMARY]
GO
GO



ALTER  PROC dbo.usp_Broker_Save
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


GO

GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.152')
	begin
		update DataInfo set keyValue = '3.46.152' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.152' '3.46.152 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO

