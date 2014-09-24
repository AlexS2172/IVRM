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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.45.106' or (keyValue = '3.45.107')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.45.107 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.45.107' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/

----------------------------------------------------------------------------------------------------
-- usp_BoFuture_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFuture_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFuture_Get]
GO

CREATE PROC dbo.usp_BoFuture_Get
	@iFutureID int = null
AS
	-- retrieve future

	set nocount on

	if @iFutureID is not null
		select
			f.futureID as iContractID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcSymbol,
			fc.contractName as vcContractName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			f.maturityID as iMaturityID,
			frmc.expiryDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			fr.futureLotSize as iFutureLotSize,
			f.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
			inner join ExpiryInCalendarView frmc on f.maturityID = frmc.expiryID and fr.matCalendarID = frmc.expCalendarID
		where
			f.futureID = @iFutureID
	else
		select
			f.futureID as iContractID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcSymbol,
			fc.contractName as vcContractName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			f.maturityID as iMaturityID,
			frmc.expiryDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			fr.futureLotSize as iFutureLotSize,
			f.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
			inner join ExpiryInCalendarView frmc on f.maturityID = frmc.expiryID and fr.matCalendarID = frmc.expCalendarID
		order by
			fc.symbol

GO

----------------------------------------------------------------------------------------------------
-- usp_EodOldFutureOptions_Del
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodOldFutureOptions_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodOldFutureOptions_Del]
GO

CREATE PROCEDURE dbo.usp_EodOldFutureOptions_Del
	@iFutureID int
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	Create table #FutureContractsForDelete(contractID int) 

	Create table #FutureContracts_with_trades(contractID int, symbol varchar(20))

	--=============================================================
	-- select futures options, which have a trades or orders
	--=============================================================
	insert into #FutureContracts_with_trades
		select C.contractID, C.symbol
			from 
				FutureOption FO
					join Future F on F.futureID = FO.futureID
					join Contract C on FO.futureOptionID = C.contractID
					join FutureRoot FR on FR.futureRootID = F.futureRootID
					join ExpiryInCalendar E on F.maturityID = E.expiryID and FR.matCalendarID = E.expCalendarID
			where 
				( 
					( 
						E.expiryDate < GetDate() or datediff(d, FO.actionDate, GetDate()) > 0 
					)
					and 
					(
						FO.futureOptionID in (select distinct ContractID from Trade) 
						or FO.futureOptionID in (select distinct ContractID from [Order])
					)
					and FO.futureID = @iFutureID
				)

	--=============================================================
	-- select futures options for remove
	--=============================================================
	insert into #FutureContractsForDelete
		select FO.futureOptionID
			from 
				FutureOption FO
					join Future F on F.futureID = FO.futureID
					join Contract C on FO.futureOptionID = C.contractID
					join FutureRoot FR on FR.futureRootID = F.futureRootID
					join ExpiryInCalendar E on F.maturityID = E.expiryID and FR.matCalendarID = E.expCalendarID
			where 
				( 
					( 
						E.expiryDate < GetDate() or datediff(d, FO.actionDate, GetDate()) > 0 
					)
					and 
					(
						FO.futureOptionID not in (select distinct ContractID from Trade) 
						and FO.futureOptionID not in (select distinct ContractID from [Order])
					)
					and FO.futureID = @iFutureID
				)

	exec @error = usp_EodFutureOption_Del

	select symbol as vcSymbol from #FutureContracts_with_trades

finish:
	Return(@error)

GO

----------------------------------------------------------------------------------------------------
-- usp_BoContractsSymbol_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoContractsSymbol_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	DROP PROC [dbo].[usp_BoContractsSymbol_Get]
GO

CREATE proc dbo.usp_BoContractsSymbol_Get
	@iContractTypeID int = null
as
	if isnull(@iContractTypeID, 0) <> 0
		select	
			C.contractID as iContractID, 
			C.contractTypeID as iContractTypeID, 
			C.symbol as vcSymbol 
		from ContractView c  
		where c.contractTypeID = @iContractTypeID
		order by C.symbol
	else
		select	
			C.contractID as iContractID, 
			C.contractTypeID as iContractTypeID, 
			C.symbol as vcSymbol 
		from ContractView c  
		where c.contractTypeID in (1, 2, 4)
		order by C.symbol

Return (@@Error)
GO

----------------------------------------------------------------------------------------------------
-- remove usp_OptionWithPrice_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_OptionWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_OptionWithPrice_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_BoOptionWithPrice_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoOptionWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoOptionWithPrice_Get]
GO

CREATE proc dbo.usp_BoOptionWithPrice_Get
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
			cp.priceTheo as fPriceTheo
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

----------------------------------------------------------------------------------------------------
-- usp_BoFutureOptionWithPrice_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFutureOptionWithPrice_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFutureOptionWithPrice_Get]
GO

CREATE proc dbo.usp_BoFutureOptionWithPrice_Get
	  @iFutureID int		 
  as
	select	c.contractID as iContractID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName,
			null as iOptionRootID,
			ec.expiryDate as dtExpiry,
			fo.strike as fStrike,
			fo.isCall as bIsCall,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheo
	from FutureOption fo
			inner join ContractView c on c.contractID = fo.futureOptionID
			inner join ContractView fc on fc.contractID = fo.futureID
			inner join ExpiryInCalendarView ec on fo.expiryID = ec.expiryID and fc.expCalendarID = ec.expCalendarID
			left join ContractPrice cp on fo.futureOptionID = cp.contractID and cp.exchangeID is null
	where	fo.futureID = @iFutureID
			and ec.expiryDate >= getdate()
	order by year(EC.expiryDate), month(EC.expiryDate), fo.strike, fo.isCall desc

  return @@error

GO

----------------------------------------------------------------------------------------------------
-- remove usp_ContractBySymbol_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractBySymbol_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_ContractBySymbol_Get]
GO

----------------------------------------------------------------------------------------------------
-- usp_BoContractBySymbol_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoContractBySymbol_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoContractBySymbol_Get]
GO

CREATE proc dbo.usp_BoContractBySymbol_Get
		@vcSymbol varchar(20)
as
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					1 as iLotSize, 
					CP.priceClose as fPriceClose,
					CP.priceTheo as fPriceTheo,
					CP.actionDate as fPriceDate,
					CP.volume10Day as iVolume10Day,
					CP.volume5Expiration as iVolume5Expiration
			from ContractView c 
					left join ContractPrice CP on C.contractID=CP.contractID and CP.exchangeID is null
			where C.symbol = @vcSymbol	
					and c.contractTypeID < 3
	union
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					ort.lotSize as iLotSize, 
					CP.priceClose as fPriceClose,
					CP.priceTheo as fPriceTheo,
					CP.actionDate as fPriceDate,
					CP.volume10Day as iVolume10Day,
					CP.volume5Expiration as iVolume5Expiration
			from ContractView c 
					join OptionView O on O.optionID = C.contractID
					join OptionRoot ort on ort.optionRootID = o.optionRootID
					left join ContractPrice CP on C.contractID=CP.contractID and CP.exchangeID is null
		   where c.symbol = @vcSymbol  
					and c.contractTypeID = 3
	union
			select	c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					frt.futureLotSize as iLotSize, 
					CP.priceClose as fPriceClose,
					CP.priceTheo as fPriceTheo,
					CP.actionDate as fPriceDate,
					CP.volume10Day as iVolume10Day,
					CP.volume5Expiration as iVolume5Expiration
			from ContractView c 
					join Future f on f.futureID = C.contractID
					join FutureRoot frt on frt.futureRootID = f.futureRootID
					left join ContractPrice CP on C.contractID=CP.contractID and CP.exchangeID is null
		   where c.symbol = @vcSymbol  
					and c.contractTypeID = 4
	union
			select	c.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					frt.optionLotSize as iLotSize, 
					CP.priceClose as fPriceClose,
					CP.priceTheo as fPriceTheo,
					CP.actionDate as fPriceDate,
					CP.volume10Day as iVolume10Day,
					CP.volume5Expiration as iVolume5Expiration
			from ContractView c 
					join FutureOption fo on fo.futureOptionID = C.contractID
					join Future f on f.futureID = fo.futureID
					join FutureRoot frt on frt.futureRootID = f.futureRootID
					left join ContractPrice CP on C.contractID=CP.contractID and CP.exchangeID is null
		   where c.symbol = @vcSymbol  
					and c.contractTypeID = 5
GO
 

----------------------------------------------------------------------------------------------------
-- usp_Option_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Option_Import]
GO
CREATE PROCEDURE dbo.usp_Option_Import
	@iUnderlyingContractID int,
	@vcSymbol varchar(20),
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
	@iLotSize int,
	@iMsCacheOptionID int = null
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

	select @vcUnderlyingSymbol = symbol from ContractView where contractID = @iUnderlyingContractID
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
   if exists (select optionRootID 
				   from OptionRootView
				   where symbol = left(@vcSymbol,len(@vcSymbol)-2)and 
					underlyingID<>@iUnderlyingContractID)
		begin			
			Declare @vcDifferentUndSymbol varchar(8)
			select @vcDifferentUndSymbol = C.symbol from ContractView C join
					OptionRootView ort on ort.underlyingID = C.contractID
					where ort.symbol = left(@vcSymbol,len(@vcSymbol)-2) and
					ort.underlyingID<>@iUnderlyingContractID
					
			set @ERR_MSG = 'Incorrect option symbol! '  + '''' +  @vcSymbol + ''''  +  ': Option Root=' + '''' + left(@vcSymbol,len(@vcSymbol)-2) + '''' + ' assign to different underlying=' + '''' +  @vcDifferentUndSymbol + '''' + '. Please, rename Option Root and restart importing of market structure.'
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
	select @iOptionRootID = optionRootID from OptionRootView where symbol = left(@vcSymbol,len(@vcSymbol)-2)
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
					select @iUnderlyingContractID,left(@vcSymbol,len(@vcSymbol)-2),isnull(@iLotSize,100)
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
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@OptionID,null)
					if (@@error <> 0) begin rollback tran return(-1) end           
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

GO

----------------------------------------------------------------------------------------------------
-- usp_Stock_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_Stock_Import]
GO
CREATE proc dbo.usp_Stock_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@fPriceAsk float,
	@fPriceBid float,
	@fPriceOpen float,
	@fPriceClose float,
	@fPriceLow float,
	@fPriceHigh float,
	@iVolume int,
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
		--========================================================================
		-- make record with null exchange for this stock
		--========================================================================
		if not exists(select * from ContractPrice where exchangeID is null and contractID = @iContractID)
			begin
				insert into ContractPrice(contractID,exchangeID)
					values(@iContractID,null)
				if (@@error <> 0) begin rollback tran return(-1) end           
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
						divFreq = @iDivFreq
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
 
----------------------------------------------------------------------------------------------------
-- usp_EodFuture_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_EodFuture_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_EodFuture_Import]
GO
CREATE PROCEDURE dbo.usp_EodFuture_Import
	@iFutureRootID int,
	@iExpiryID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iLotSizeID int,
	@iOptionLotSizeID int,
	@tiIsActive tinyint output
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	declare	@iFutureID int,
			@iUndPriceProfileID int,
			@iOptPriceProfileID int

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

			update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@iFutureID and actionID<>3
			if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

			update Future set actionDate = getdate() where futureID=@iFutureID
			if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

			update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -3 if @@TranCount = 1 Rollback Transaction goto finish end
			
		Commit Transaction
	end
	else
	begin
		Begin Transaction

			select
				@iUndPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 0

			-- check underlying price profile
			if not exists(select 1 from PriceProfileView where priceProfileID = @iUndPriceProfileID)
			begin set @error = -4 if @@TranCount = 1 Rollback Transaction goto finish end
		
			select
				@iOptPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 1

			-- check options price profile
			if not exists(select 1 from PriceProfileView where priceProfileID = @iOptPriceProfileID)
			begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into Contract
				(contractTypeID, symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
			values 
				(4, @vcSymbol, @vcContractName, 1, @iUndPriceProfileID, @iOptPriceProfileID)

			set @iFutureID = @@identity
			if @@error <> 0 begin set @error = -5 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into Future (futureID, futureRootID, maturityID)
				 values (@iFutureID, @iFutureRootID, @iExpiryID)
			if @@error <> 0 begin set @error = -6 if @@TranCount = 1 Rollback Transaction goto finish end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iFutureID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@iFutureID,null)
					if (@@error <> 0) begin set @error = -7 if @@TranCount = 1 Rollback Transaction goto finish end
				end 

			update FutureRoot  set futureLotSize = @iLotSizeID, optionLotSize = @iOptionLotSizeID where futureRootID=@iFutureRootID
			if @@error <> 0 begin set @error = -8 if @@TranCount = 1 Rollback Transaction goto finish end
			
			set @tiIsActive = 1

		Commit Transaction
	end 
	 
   set @error = @iFutureID
finish:
Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_BoFuture_Import
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoFuture_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoFuture_Import]
GO
CREATE PROCEDURE dbo.usp_BoFuture_Import
	@iFutureID int,
	@iFutureRootID int,
	@iExpiryID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iLotSizeID int,
	@iOptionLotSizeID int
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
			
	exec @error = usp_EodFuture_Import @iFutureRootID,@iExpiryID,@vcSymbol,@vcContractName,@iLotSizeID,@iOptionLotSizeID, @tiIsActive
	 
finish:
Return(@error)
GO

----------------------------------------------------------------------------------------------------
-- usp_BoTradeNewContract_Get
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoTradeNewContract_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_BoTradeNewContract_Get]
GO
CREATE PROCEDURE dbo.usp_BoTradeNewContract_Get
	@dtLastTradeDateUTC datetime = null -- return all contracts for trades were made earlier or at @dtLastTradeDateUTC
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	select
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		c.exportSymbol as vcExportSymbol,
	
		case when c.contractTypeID = 3 then o.expiryID else fo.expiryID end as iExpiryID,
		case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
		case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,
	
		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,
		uc.exportSymbol as vcUnderlyingExportSymbol,
	
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,

		fr.futureRootID as iFutureRootID,
		fr.futureRootSymbol as vcFutureRootSymbol,
		f.futureID as iFutureID,
		fc.symbol as vcFutureSymbol,
		fc.exportSymbol as vcFutureExportSymbol,
		f.maturityID as iFutureMaturityID
	from
		TradeView tr
		-- contract and underlying contract
		inner join ContractView c on tr.contractID = c.contractID
		inner join ContractView uc on tr.underlyingID = uc.contractID
		-- regular option
		left join OptionView o on c.contractID = o.optionID
		left join OptionRootView ort on tr.optionRootID = ort.optionRootID
		-- futures
		left join ContractView fc on tr.futureID = fc.contractID
		left join FutureRoot fr on tr.futureRootID = fr.futureRootID
		left join Future f on tr.futureID = f.futureID
		left join FutureOption fo on tr.contractID = fo.futureOptionID
	where
		tr.status in (0, 3) -- only actual (or actual matched) trades should be reported
		and isnull(tr.isPosition, 0) = 0 -- only trades (not positions) should be reported
		and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
		and c.contractID not in (select contractID from TradeView 
									where isnull(isPosition, 0) = 1)
	group by
		c.contractID, c.contractTypeID, c.symbol, c.exportSymbol, o.expiryID,
		o.strike, o.isCall, fo.expiryID, fo.strike, fo.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
		uc.exportSymbol, ort.optionRootID, ort.symbol,
		fr.futureRootID, fr.futureRootSymbol, f.futureID,
		fc.symbol, fc.exportSymbol, f.maturityID
GO
----------------------------------------------------------------------------------------------------
-- remove usp_TradeFeedWithBookAndStrategy_New
----------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeedWithBookAndStrategy_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
	drop procedure [dbo].[usp_TradeFeedWithBookAndStrategy_New]
GO
/* RESULTS ANALYTHING */
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.45.107')
	begin
		update DataInfo set keyValue = '3.45.107' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.45.107' '3.45.107 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
