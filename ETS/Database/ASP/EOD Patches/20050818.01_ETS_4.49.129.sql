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
BEGIN TRANSACTION
GO
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.128' or (keyValue = '4.49.129')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.129 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.128' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='OptionRoot' and COLUMN_NAME='isFitEnabled')
ALTER TABLE OptionRoot 
	ADD isFitEnabled INT NOT NULL,
	CONSTRAINT DEF_OptionRoot_isFitEnabled DEFAULT(1) for isFitEnabled


GO
UPDATE OptionRoot set isFitEnabled = 1 where isFitEnabled is NULL 
GO
ALTER                PROCEDURE dbo.usp_OptionRoot_Save 
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


GO
ALTER         View dbo.OptionRootView
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

GO
ALTER        PROCEDURE dbo.usp_OptionRoot_Get 
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
GO

ALTER  PROC dbo.usp_MmOptionRoot_Get
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

GO

ALTER  PROC dbo.usp_MmOptionByUnderlying_Get
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

ALTER    Procedure dbo.usp_OptionVM_Get
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

GO


ALTER   proc dbo.usp_Stock_Import
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.129')
	begin
		update DataInfo set keyValue = '4.49.129' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.129' '4.49.129 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
-- unable to execute this script in transaction
