/*
	fix for MLine00000526
*/

set nocount on
GO
Alter table Contract
	Alter column symbol varchar(20)
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ActiveSurface_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ActiveSurface_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ActiveSurface_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ActiveSurface_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractBySymbol_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractBySymbol_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractBySymbol_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractBySymbol_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPriceEOD_Init_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPriceEOD_Init_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPriceEOD_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPriceEOD_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractPrice_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractPrice_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractVM_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractVM_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Contract_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Contract_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_CustomVolatilityPointIsExist_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_CustomVolatilityPointIsExist_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexDataHistoryDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexDataHistoryDH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IndexID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IndexID_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Index_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Index_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IsSymbolExists_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IsSymbolExists_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Import]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_PriceCloseDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_PriceCloseDH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_PriceCloseDH_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_PriceCloseDH_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockDataHistoryDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockDataHistoryDH_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockID_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockID_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockVM_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockVM_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Stock_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Stock_Save]
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE      Procedure dbo.usp_StockVM_Save
	@iStockID int = null output,		-- stock's identifier
	@iContractTypeID int = null,		-- contract's type identifier (not used)
	@vcSymbolName varchar(20) = null,	-- stock's ticker
	@iExchangeID int = null,		-- exchange's identifier
	@vcDescription varchar(50) = null,	-- description
	@iLotSize int = null,			-- lot size (not used)
	@iDivFrequency int = null,		-- dividend's frequency
	@dtDate datetime = null,		-- date
	@fIVMean float = null,			-- iv mean
	@fCurrentHV float = null,		-- current hv
	@fCurrentIV float = null,		-- current iv
	@fAmount float = null, 			-- amount
	@dDivDate datetime = null,		-- dividend's date
	@iTenorID int = null,			-- tenor's identifier
	@iTenorValue int = null,		-- tenor's value
	@fIVCall float = null,			-- iv call
	@fIVPut float = null,			-- iv put
	@fPriceClose float = null,		-- stock's close price
	@iGroupID int = null,
	@iRuleID int = null
as
/*
	Created by Andrey Sinkin
	Can to save data to Contract, Stock, StockDataHistory, StockDatasnapshot tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	-- Declaring variables
	declare @error int		set @error = -1
	declare @iDividendID int, @iMarketDataID int
	set @iContractTypeID = 2	-- stock
	set @iLotSize = 0
	-- if wrong parameters -> exit
	if @vcSymbolName is null and @iStockID is null	goto finish
	if @iTenorID is null and @iTenorValue is not null
		select @iTenorID = tenorID from Tenor where value = @iTenorValue
	if @iStockID is null and @vcSymbolName is not null
		select @iStockID = contractID from ContractView where symbol = @vcSymbolName and contractTypeID = 2	-- stock
	Begin Transaction
		if @iStockID is null
			begin
				insert into Contract (contractTypeID, symbol,contractName,groupID,ruleID) 
					values (@iContractTypeID, @vcSymbolName,@vcDescription, @iGroupID,@iRuleID)
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iStockID = @@identity
				insert into Stock (StockID, divFreq)
					values (@iStockID, @iDivFrequency)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				if @iContractTypeID is not null or @vcSymbolName is not null or @iExchangeID is not null or @vcDescription is not null or @iLotSize is not null
					begin
						update Contract set 
							contractTypeID = isnull(@iContractTypeID, contractTypeID),
							symbol = isnull(@vcSymbolName, symbol),
							contractName = isnull(@vcDescription, contractName),
							groupID = isnull(@iGroupID, groupID),
							ruleID = isnull(@iRuleID, ruleID)
						where contractID = @iStockID
						if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end
					end
			end
		--============================================================================================================
		if exists(select * from ContractPrice where contractID = @iStockID and (exchangeID = @iExchangeID or @iExchangeID is null))
			update ContractPrice 
				set exchangeID = @iExchangeID,
					lotSize = isnull(@iLotSize, lotSize),
					contractID = isnull(@iStockID, contractID)
				where contractID = @iStockID and (exchangeID = @iExchangeID or @iExchangeID is null)
		else
			insert into ContractPrice
				(exchangeID,contractID,lotSize)
			values (@iExchangeID,@iLotSize,@iStockID)
		--============================================================================================================
		if @dtDate is not null and @fIVMean is not null and @iTenorID is not null
			begin
				exec @error = usp_StockDataHistory_Save @iStockID = @iStockID, @dtDate = @dtDate, @fIVMean = @fIVMean, @iTenorID = @iTenorID, @fIVCall = @fIVCall, @fIVPut = @fIVPut, @fCurrentHV = @fCurrentHV
				if @error <> 0 begin set @error = 5 if @@TranCount = 1 Rollback Transaction goto finish end

			end

		if @fAmount is not null or @dDivDate is not null or @iDivFrequency is not null
			begin
				update Stock set 
					divFreq = isnull(@iDivFrequency, divFreq),
					divAmt = isnull(@fAmount, divAmt),
					divDate = isnull(@dDivDate, divDate)
					where StockID = @iStockID

				if @error <> 0 begin set @error = 7 if @@TranCount = 1 Rollback Transaction goto finish end
			end

		if (@fCurrentHV is not null or @fCurrentIV is not null) and @iTenorID is not null
			begin
				if @@TranCount = 1 Rollback Transaction
				Raiserror ('Error. Can''t save CurrentIV for Stock (usp_StockDataSnapshot_Save stored procedure), please send this error message to DB developer !', 16, 1)
			end

		if @fPriceClose is not null
			begin
				if @@TranCount = 1 Rollback Transaction
				Raiserror ('Error. Can''t save close price for Stock (usp_StockVM_Save stored procedure), please send this error message to DB developer !', 16, 1)
			end

		set @error = 0
	Commit Transaction
finish:
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



CREATE            Procedure dbo.usp_ActiveSurface_Get 
	@vcSymbolName varchar(20) --the contract symbol
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure return default surface identifier
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iContractID int
		select @iContractID = contractID from ContractView where symbol = @vcSymbolName and  contractTypeID < 3

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int 
	set @iVolaSurfaceDataID = 0
		exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	select	@iVolaSurfaceDataID as iVolaSurfaceDataID


	set @error = @@error
Return(@error)




GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE    Procedure dbo.usp_ActiveSurface_Save
	@vcSymbolName varchar(20), --the contract symbol
	@iVolaSurfaceDataID int -- the VolaSurfaceData row ID
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure return default surface identifier
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	Begin Transaction
--clear isDefaultSurface Status
	update VolaSurfaceData 
		set isDefaultSurface=0
		from VolaSurfaceData 
				join Contract on VolaSurfaceData.contractID=Contract.contractID
		where Contract.symbol = @vcSymbolName
	if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
--set isDefaultSurface Status
	update VolaSurfaceData 
		set isDefaultSurface=1
		from VolaSurfaceData 
				join Contract on VolaSurfaceData.contractID=Contract.contractID
		where Contract.symbol = @vcSymbolName and volaSurfaceDataID=@iVolaSurfaceDataID
	
	if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
	set @error = @@error
	Commit Transaction
    set @error = 0
finish:
Return(@error)









GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE	proc dbo.usp_ContractBySymbol_Get
		@vcSymbol varchar(20)
as
begin
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.priceTheo as fPriceTheo,
					CP.priceDate as fPriceDate,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID,
					E.exchangeName as vcExchangeName
			from ContractView c 
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID
			where C.symbol = @vcSymbol	
					and c.contractTypeID in (1, 2)
	union
			select	C.contractID as iContractID,
					c.contractTypeID as iContractTypeID,
					c.symbol as vcSymbol,
					c.contractName as vcContractName, 
					CP.priceBid as fPriceBid,
					CP.priceAsk as fPriceAsk,
					CP.lotSize as iLotSize, 
					CP.priceLast as fPriceLast,
					CP.priceClose as fPriceClose,
					CP.priceOpen as fPriceOpen,
					CP.priceLow as fPriceLow,
					CP.priceHigh as fPriceHigh,
					CP.priceTheo as fPriceTheo,
					CP.priceDate as fPriceDate,
					CP.volume as iVolume,
					CP.openInterest as iOpenInterest,
					E.exchangeID as iExchangeID,
					E.exchangeName as vcExchangeName
			from ContractView c 
					join OptionView O on O.optionID = C.contractID
					left join ContractPrice CP on C.contractID=CP.contractID
					left join ExchangeView E on CP.exchangeID=E.exchangeID				
		   where c.symbol = @vcSymbol  
					and c.contractTypeID not in (1, 2)
end

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE proc dbo.usp_ContractBySymbol_Save
	  @iContractID int,
	  @vcSymbol varchar(20)
	  
  as
	update Contract 
		set symbol = @vcSymbol
	  where contractID = @iContractID
	return @@error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE  procedure dbo.usp_ContractPriceEOD_Init_Save
as
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	if object_id('tempdb..##CPriceEOD') is not null
		begin
			Raiserror ('EOD already is in progress. Cannot run 2nd copy of EOD.', 16, 1)
			return (-1) 
			-- drop table ##CPriceEOD
		end

	create table ##CPriceEOD(
		symbol varchar (20) null ,
		contractTypeID int not null,
		priceClose float not null,
		exchangeCode char(10) null,
		exchangeID int null,
		contractID int null) 

	set @error = @@error
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE    procedure dbo.usp_ContractPriceEOD_Save
	@vcSymbol varchar(20),
	@fPriceClose float,
	@iContractTypeID int, --2-stock 3-option
	@vcExchangeCode varchar(10) = Null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int	set @error = -1
	if (select object_id('tempdb..##CPriceEOD')) is not null
		begin
			insert into ##CPriceEOD (symbol, contractTypeID, priceClose, exchangeCode)
				values (@vcSymbol, @iContractTypeID, @fPriceClose, @vcExchangeCode)
		end
    set @error = @@error
Return(@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
CREATE    proc dbo.usp_ContractPrice_Save
	@iContractPriceID int = Null output,
	@iContractID int = Null,
	@iExchangeID int = Null, 
	@fPriceBid float = Null,
	@fPriceAsk float = Null,
	@iLotSize int = Null,
	@fPriceLast float = Null,
	@fPriceOpen float = Null,
	@fPriceClose float = Null,
	@fPriceLow float = Null,
	@fPriceHigh float = Null,
	@iVolume int = Null,
	@iOpenInterest int = Null,
	@vcSymbol varchar(20) = Null,
	@iContractTypeID int = Null, --2-stock 3-option
	@vcExchangeCode varchar(10) = Null
as
/*
	Created by Zaur Nuraliev
	[Description]:	this procedure save information into 
					ContractPrice table
*/
	-------------------------------------------------
	set nocount on
	set ANSI_NULLS OFF --(exchangeID may be null)
	-------------------------------------------------
	declare @error int		set @error = -1
	if @vcSymbol is not null and @iContractTypeID is not null 
		begin
			Select @iContractID = contractID from Contract where symbol=@vcSymbol and contractTypeID=@iContractTypeID
			if @iContractID is null goto finish 
		end
	if @vcExchangeCode is not null
		begin
			Select @iExchangeID=exchangeID from Exchange where exchangeCode=@vcExchangeCode
		end
	Begin Transaction
		if (not exists (select * from ContractPrice where contractPriceID = @iContractPriceID) and @iContractPriceID is not Null)
		or (@iContractID is not Null and not exists(select * from ContractPrice where contractID=@iContractID and isnull(exchangeID,0)=isnull(@iExchangeID,0)))
		 begin
			insert into ContractPrice (contractID,
						exchangeID, 
						priceBid,
						priceAsk,
						lotSize,
						priceLast,
						priceOpen,
						priceClose,
						priceLow,
						priceHigh,
						volume,
						openInterest)
				  values (@iContractID,
						@iExchangeID, 
						@fPriceBid,
						@fPriceAsk,
						@iLotSize,
						@fPriceLast,
						@fPriceOpen,
						@fPriceClose,
						@fPriceLow,
						@fPriceHigh,
						@iVolume,
						@iOpenInterest)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			select @iContractPriceID = @@identity
		 end
	   else
		 begin
			update ContractPrice
				set contractID = isNull(@iContractID,contractID),
					exchangeID = isNull(@iExchangeID,exchangeID),
					priceBid = isNull(@fPriceBid,priceBid),
					priceAsk = isNull(@fPriceAsk,priceAsk),
					lotSize = isNull(@iLotSize,lotSize),
					priceLast = isNull(@fPriceLast,priceLast),
					priceOpen = isNull(@fPriceOpen,priceOpen),
					priceClose = isNull(@fPriceClose,priceClose),
					priceLow = isNull(@fPriceLow,priceLow),
					priceHigh = isNull(@fPriceHigh,priceHigh),
					volume = isNull(@iVolume,volume),
					openInterest = isNull(@iOpenInterest,openInterest),
					priceDate = GetDate()
					where (@iContractPriceID is not Null and contractPriceID = @iContractPriceID) or 
						(@iContractID is not Null and contractID=@iContractID and exchangeID=@iExchangeID)
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		 end
		--===========================================================================================
		-- create record with Null ExchangeID
		--===========================================================================================
		if not exists(select * from ContractPrice where ContractID=@iContractID and exchangeID is null)
			begin
				insert into ContractPrice (contractID,
							exchangeID, 
							priceBid,
							priceAsk,
							lotSize,
							priceLast,
							priceOpen,
							priceClose,
							priceLow,
							priceHigh,
							volume,
							openInterest)
					  values (@iContractID,
							Null, 
							@fPriceBid,
							@fPriceAsk,
							@iLotSize,
							@fPriceLast,
							@fPriceOpen,
							@fPriceClose,
							@fPriceLow,
							@fPriceHigh,
							@iVolume,
							@iOpenInterest)
			end
	Commit Transaction
	set @error = 0
finish:

Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

CREATE Procedure dbo.usp_ContractVM_Save 
	@iContractID int = null output,		-- contract's identifier
	@iContractTypeID int = null,		-- contract's type identifier
	@vcSymbolName varchar(20) = null,	-- index's ticker
	@iExchangeID int = null,		-- exchange's identifier
	@vcDescription varchar(50) = null,	-- description
	@iLotSize int = null,			-- lot size (not used)
					-- Stock's data:
	@iDivFrequency int = null,		-- dividend's frequency
	@fAmount float = null, 			-- amount
	@dDivDate datetime = null,		-- dividend's date
					-- Index's data:
	@fYield float = null,			-- yield
	@bIsAutoUpdateWeight bit = null,	-- flag for enable/disable index's weights updating
					-- ETFs data:
	@iIndexBaseID int = null,		-- index's base identifier
	@rDenominator real = null,		-- denominator
	@bIsActive bit = null,			-- flag for enable/disable
					-- Option's data:
	@iUnderlyingID int = null,		-- underlying identifier
	@iIsCall bit = null,			-- flag for call/put
	@fStrike float = null,			-- strike
	@dtExpDate datetime = null,		-- expiration's date
	@iGroupID int = null,
	@iRuleID int = null
as
/*
*/
	-----------------------------------------------------------
	set nocount on
	-----------------------------------------------------------
	declare @error int		set @error = -1
	declare @iIndexID int, @iStockID int, @iOptionID int
	-- checking for wrong parameters
	if @iContractTypeID is null or @iContractTypeID not in (1,2,3)		goto finish
	if @iContractTypeID = 1	-- index
		exec @error = usp_IndexVM_Save
				@iIndexID = @iContractID output,		-- index's identifier
				@iContractTypeID = @iContractTypeID,		-- contract's type identifier
				@iSymbolName = @vcSymbolName,			-- index's ticker
				@iExchangeID = @iExchangeID,			-- exchange's identifier
				@vcDescription = @vcDescription,		-- description
				@iLotSize = 0,					-- lot size (not used)
			
				@fYield = @fYield,				-- yield
				@bIsAutoUpdateWeight = @bIsAutoUpdateWeight,	-- flag for enable/disable index's weights updating
				@iGroupID = @iGroupID,
				@iRuleID = @iRuleID
--				@iIndexBaseID = null,				-- index's base identifier
--				@rDenominator = null,				-- denominator
--				@bIsActive = null				-- flag for enable/disable
	else
	if @iContractTypeID = 2	-- stock
		exec @error = usp_StockVM_Save
				@iStockID = @iContractID output,		-- stock's identifier
			
				@iContractTypeID = @iContractTypeID,		-- contract's type identifier (not used)
				@vcSymbolName = @vcSymbolName,			-- stock's ticker
				@iExchangeID = @iExchangeID,			-- exchange's identifier
				@vcDescription = @vcDescription,		-- description
				@iLotSize = 0,					-- lot size (not used)
			
				@iDivFrequency = @iDivFrequency,		-- dividend's frequency
				@fAmount = @fAmount, 				-- amount
				@dDivDate = @dDivDate,				-- dividend's date
				@iGroupID = @iGroupID,
				@iRuleID = @iRuleID
/*
	else
	if @iContractTypeID = 3	-- option
		exec @error = usp_OptionVM_Save
				@iOptionID = @iContractID output,	-- option's identifier
			
				@iContractTypeID = @iContractTypeID,-- contract's type identifier
				@iSymbolName = @vcSymbolName,		-- option's ticker
				@iExchangeID = @iExchangeID,		-- exchange's identifier
				@vcDescription = @vcDescription,	-- description
				@iLotSize = @iLotSize,				-- lot's size
			
				@iUnderlyingID = @iUnderlyingID,	-- underlying identifier
				@iIsCall = @iIsCall,				-- flag for call/put
				@fStrike = @fStrike,				-- strike
				@dtExpDate = @dtExpDate,			-- expiration's date
				@iGroupID = @iGroupID,
				@iRuleID = @iRuleID
*/
--====================================================================
-- update ruleID, groupID fields
--====================================================================
	if @iContractID is not null
	--if exists(select * from Contract where contractID=@iContractID)
		begin
			if (@iGroupID is not null) or (@iRuleID is not null)
				update Contract 
					set groupID = isnull(@iGroupID,groupID), ruleID = isnull(@iRuleID,ruleID)
				where contractID = @iContractID
		end 
finish:
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE     proc dbo.usp_Contract_Save
	@iContractID int,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iLotSize int,
	@fPriceBid float,
	@fPriceAsk float,
	@iExchangeID int,
	@fPriceLast float,
	@fPriceOpen float,
	@fPriceClose float,
	@fPriceLow float,
	@fPriceHigh float,
	@iVolume int,
	@iOpenInterest int

  as

-------------------------------------------------
set nocount on
-------------------------------------------------
  begin tran
	update Contract
		 set symbol = @vcSymbol,
			   contractName = CASE WHEN contractTypeID in (1, 2) THEN @vcContractName ELSE contractName END
	  where contractID = @iContractID
   if (@@error <> 0) begin rollback tran return @@error end  

	if exists(select * from ContractPrice where isnull(exchangeID,0) = isnull(@iExchangeID,0) and contractID = @iContractID)	
		begin
			update ContractPrice 
				set lotSize = @iLotSize,
					priceBid = @fPriceBid,
					priceAsk = @fPriceAsk,
					priceLast = @fPriceLast,
					priceOpen = @fPriceOpen,
					priceClose = @fPriceClose,
					priceLow = @fPriceLow,
					priceHigh = @fPriceHigh,
					volume = @iVolume
				where isnull(exchangeID,0) = isnull(@iExchangeID,0) and contractID = @iContractID
			   if (@@error <> 0) begin rollback tran return @@error end  
		end
	else 
		begin
			insert into ContractPrice (contractID,
					exchangeID,
					lotSize,
					priceBid,
					priceAsk,
					priceLast,
					priceOpen,
					priceClose,
					priceLow,
					priceHigh,
					volume)
			values (@iContractID,
					@iExchangeID,
					@iLotSize,
					@fPriceBid,
					@fPriceAsk,
					@fPriceLast,
					@fPriceOpen,
					@fPriceClose,
					@fPriceLow,
					@fPriceHigh,
					@iVolume)
		   if (@@error <> 0) begin rollback tran return @@error end  
		end

   if (@iOpenInterest is not Null)
	  begin
		declare @ctID int
		select @ctID = contractTypeID from Contract where contractID = @iContractID
		if (@ctID = 3)
		  begin
			update ContractPrice
				set openInterest = @iOpenInterest
				where contractID = @iContractID and isnull(exchangeID,0) = isnull(@iExchangeID,0)
			   if (@@error <> 0) begin rollback tran return @@error end  
		  end	
	  end
  commit tran

return @@error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE     Procedure dbo.usp_CustomVolatilityPointIsExist_Get
	@vcSymbolName varchar(20) = null
as
/*
	Created by Zaur Nuraliev
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	select distinct 
			CV.contractID as iContractID,
			CV.symbol as vcSymbolName,
			EC.expiryDate as dtExpiry,
			case when CSSP.volaSurfaceDataID is null then 0  
			else 1 end 'CustomVolatilityPointsCount'
		from OptionView OV
			join ContractView CV on OV.underlyingContractID = CV.contractID
			join ExpiryInCalendarView EC on OV.expiryID = EC.expiryID and EC.expCalendarID = CV.expCalendarID
			left join VolaSurfaceDataView VSD on OV.underlyingContractID = VSD.contractID and VSD.isDefaultSurface = 1
			left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
				and OV.expiryID = CSSP.expiryID
		where 
			CSSP.volaSurfaceDataID is null and 
			(CV.symbol = @vcSymbolName or @vcSymbolName is null) 
		order by CV.contractID,EC.expiryDate


Return(0)



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE    Procedure dbo.usp_IndexDataHistoryDH_Get
	@vcSymbol varchar(20),	-- index ticker
	@fCurrentIV float = null Output,
	@fCurrentHV float = null Output,
	@fCurrentVolOfVol float = null Output,
	@fCurrentVega float = null Output
as
/*
	Created by Zaur Nuraliev
	this procedure returns historical data for index
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iIndexID int
	select @iIndexID=contractID from ContractView where symbol=@vcSymbol and contractTypeID = 1
	select 	@fCurrentIV=currentIV, 
			@fCurrentHV=currentHV,
			@fCurrentVolOfVol=currentVolOfVol,
			@fCurrentVega=currentVega
		from IndexDataHistory
		where tenorID=8 
			and [date]=(select max([date]) from IndexDataHistory where indexID=@iIndexID and tenorID=8)
			and indexID=@iIndexID
	set @error = @@error
finish:
Return(@error)









GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE  procedure dbo.usp_IndexID_Get 
	   @vcSymbol varchar(20) 
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting index identifiers
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select C.contractID as iIndexID
		from ContractView c 	
				join IndexView I on I.indexID = C.contractID
		where C.contractTypeID = 1 and C.symbol = @vcSymbol
	set @error = @@error
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE    proc dbo.usp_Index_Save
		@iIndexID int,
		@vcSymbol varchar(20) = null,
		@vcContractName varchar(255) = null,
		@fYield float = Null,
		@bIsHTB bit = Null,
		@tiCalcOptionType tinyint = null,
		@tiCalcModelType tinyint = null,
		@iExpCalendarID int = null,
		@tiIsBasket tinyint = null,
		@tiIsActive tinyint = null
   as
	---------------
	set nocount on
	---------------

	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID=1 and actionID < 3 and (@iIndexID <> contractID or @iIndexID is null))
		begin
			Raiserror ('Index with this ticker already exists!', 16, 1)
			Return (-1)
		end   

   if (@iIndexID is Null)  
	 begin
		 
		 begin tran
			 insert into Contract (contractTypeID, symbol, contractName,expCalendarID)
				  values (1, @vcSymbol, @vcContractName,@iExpCalendarID)
			 select @iIndexID = @@identity			 
				 if (@@error <> 0)
					  begin
						RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
						ROLLBACK TRAN
						RETURN (-1)
					  end
			 insert into [Index] (indexID, yield,IsHTB,calcOptionType,calcModelType,isBasket, isActive)
					 values (@iIndexID, @fYield,isNull(@bIsHTB,0),@tiCalcOptionType,@tiCalcModelType,isnull(@tiIsBasket,0), @tiIsActive)
				 if (@@error <> 0)
					begin
					   RAISERROR ('Error. Can''t insert into [Index]!', 16, 1)
					   ROLLBACK TRAN
					   RETURN (-1)
					end 			
		 commit tran
	 end
   else
	 begin
		begin tran
		   update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@vcContractName,contractName),
					expCalendarID = isNull(@iExpCalendarID, expCalendarID)
				where contractID = @iIndexID
			if (@@error <> 0)
				 begin
					RAISERROR ('Error. Can''t update Contract!', 16, 1)
					ROLLBACK TRAN
					RETURN (-1)
				 end
		  update [Index]
			   set	yield = isNull(@fYield,yield),
					isHTB = isNull(@bIsHTB,isHTB),
					calcOptionType = isNull(@tiCalcOptionType, calcOptionType),
					calcModelType = isNull(@tiCalcModelType, calcModelType),
					isBasket = isNull(@tiIsBasket,isBasket),
					isActive = isnull(@tiIsActive, isActive)
			   where indexID = @iIndexID
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t update [Index]!', 16, 1)
				  ROLLBACK TRAN
				  RETURN (-1)
			   end		   
		 commit tran
	 end

	--========================================================================
	-- make record with null exchange for this index
	--========================================================================
	if not exists(select * from ContractPrice where exchangeID is null and contractID = @iIndexID)
		begin
			insert into ContractPrice(contractID,exchangeID)
				values(@iIndexID,null)
			if (@@error <> 0) begin rollback tran return(-1) end           		
		end		


if (@@error <> 0)
	return (-1)
else	
	return @iIndexID








GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO









CREATE  Procedure dbo.usp_IsSymbolExists_Get 
	@vcSymbol varchar(20)
AS
/*
	Created by ...
	Description: this procedure is getting information from Contract table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select contractID as iContractID
		from ContractView	
		where contractTypeID < 3 and symbol=@vcSymbol
	set @error = @@error
Return(@error)









GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO







CREATE          Procedure dbo.usp_Option_Import
	@iUnderlyingContractID int = null,  --underlying contractID
	@vcSymbol varchar(20),  -- option symbol
	@fPriceAsk float, 
	@fPriceBid float, 
	@iVolume int,
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
	@iOpenInterest int,
	@iExchangeID int = Null, -- the exchange identifier (special for ContractPrice update)
	@vcUnderlyingSymbol varchar(8) = null,  -- stock symbol
	@iLotSize int = null
as
/*
	Created by ... Redesigned by Zaur Nuraliev
	Description: this procedure save ...
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare	@OptionID int, 
			@cn varchar(255),
			@iExpiryID int,
			@iLastExpiryID int

	--===================================================================			
	-- check the Expiry
	--===================================================================			
	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)

	if not exists(select * from Expiry where expiryID = @iExpiryID)
		begin
			Declare @ERR_MSG varchar(8000)
			set @ERR_MSG = 'Incorrect expiration! '  + '"' +  Convert(varchar(50),@dtExpiry) + '"' + ' for symbol=' + '"' + @vcSymbol + '"' +  ' UnderlyingSymbol=' + '"' + @vcUnderlyingSymbol + '"' + ' Strike=' + Convert(varchar(15),@fStrike) + ' expiryID=' + Convert(varchar(15),@iExpiryID)
			Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end

	select top 1 @iLastExpiryID = expiryID from ExpiryInCalendarView where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	--===================================================================			
	-- check the consistensy of input parameters (for stock)
	--===================================================================			
	declare @iUnderlyingFirstContractID int
		set @iUnderlyingFirstContractID = @iUnderlyingContractID
	--select underlying ID
	select @iUnderlyingContractID = contractID  
		from Contract 
		where symbol = @vcUnderlyingSymbol and contractTypeID < 3
	if @iUnderlyingFirstContractID <> @iUnderlyingContractID
		begin
			Raiserror ('Wrong underlying contract ID in usp_Option_Import!', 16, 1)
			Return (-1)
		end  
	--select underlying option symbol
	if @vcUnderlyingSymbol is null
		select @vcUnderlyingSymbol=Contract.Symbol 
			from Contract (nolock) 	
			where Contract.contractID = @iUnderlyingContractID and Contract.contractTypeID < 3

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if (@OptionID is Null) 
		select @OptionID = optionID
			from [Option] (nolock)
			where underlyingContractID = @iUnderlyingContractID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	--===================================================================			
	-- try to find Option with same ticker in DB
	--===================================================================			
	declare @iIncorrectOptionID int

	select @iIncorrectOptionID = contractID 
		from Contract (nolock)
				join [Option] (nolock) on Contract.contractID = [Option].optionID
		where symbol = @vcSymbol and contractTypeID = 3 and 
			[Option].actionID < 3 and (@OptionID <> contractID) -- or @OptionID is null

	if @iIncorrectOptionID is not null
		begin
			-- mark as delete
			update [Option] set [Option].actionID = 3 
				where optionID = @iIncorrectOptionID and expiryID < @iLastExpiryID

			if @@rowcount = 0 
				begin
					--Raiserror ('Option with this symbol already exists!', 16, 1)
					--Return (-1)
					update [Option] set [Option].actionID = 3 
						where optionID = @iIncorrectOptionID
					update Contract set actionID = 3 
						where ContractID = @iIncorrectOptionID
				end   
			else
				update Contract set actionID = 3 
					where ContractID = @iIncorrectOptionID
		end

	--===================================================================			
	-- make the contract name
	--===================================================================			
--  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(day, @dtExpiry) + ' ' + DATENAME(month, @dtExpiry) + 
--					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
--						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(month, @dtExpiry) + 
					' ' + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)

  if exists (select contractID from Contract(nolock) where contractID = @iUnderlyingContractID and contractTypeID > 2)
	 begin
	   RAISERROR ('Error. Can''t insert values to [Option]. The Underlying Contract is not stock or index!', 16, 1)
	   RETURN (-1)
	 end 

  if (@OptionID is Null) 
	begin
		Begin Transaction
	
			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@cn)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
				 values (@OptionID, @iUnderlyingContractID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			insert into ContractPrice (contractID,exchangeID, priceBid, priceAsk, volume,openInterest,lotSize)
				 values (@OptionID,@iExchangeID, @fPriceBid, @fPriceAsk, @iVolume,@iOpenInterest,@iLotSize)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID,lotSize)
						values(@OptionID,null,@iLotSize)
					if (@@error <> 0) begin rollback tran return(-1) end           
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
		  update ContractPrice
				set	priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					volume = @iVolume,
					openInterest=@iOpenInterest,
					lotSize = isnull(@iLotSize,lotSize)
				where contractID = @OptionID and isnull(exchangeID,0)=isnull(@iExchangeID,0)
		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t updates Contract price!', 16, 1)
				  RETURN (-1)
			   end 

			update [Option]
				set 
				IsCall = isnull(@iIsCall,IsCall),
				strike = isnull(@fStrike,Strike),
				expiryID = isnull(@iExpiryID,expiryID)
			where optionID = @OptionID

		   if (@@error <> 0)
			   begin
				  RAISERROR ('Error. Can''t update Option information (table Option)!', 16, 1)
				  RETURN (-1)
			   end 

	end 
	 
-- return @OptionID 
    set @error = 0
finish:
Return(@error)







GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO






CREATE      proc dbo.usp_Option_Save
		@iOptionID int,
		@vcSymbol varchar(20) = Null,
		@iExchangeID int, 
		@iUnderlyingContractID int,
		@iIsCall tinyint = Null,
		@fStrike float = Null,
		@dtExpiry datetime = Null,
		@tiIsManualyEntered tinyint
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

   if exists (select contractID from ContractView where contractID = @iUnderlyingContractID and contractTypeID > 2)
	   begin
		 RAISERROR ('Error. Can''t insert or update Option values. The Underlying Contract is not stock or index!', 16, 1)
		 RETURN (-1)
	   end	 

	declare	@OptionID int, 
			@ContractName varchar(255),
			@iExpiryID int

	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1


   if exists (select optionID from OptionView where underlyingContractID = @iUnderlyingContractID and 
					isCall = @iIsCall and 
					strike = @fStrike and 
					expiryID = @iExpiryID and
					(@iOptionID is Null or optionID <> @iOptionID))
		begin
		  RAISERROR ('Error. Can''t insert or update Option values. There are ambiguous Option in DB!', 16, 1)
		  RETURN (-1)
		end  

   set @ContractName = @vcSymbol + ' ' + datename(day, @dtExpiry) + ' ' + datename(month, @dtExpiry) + 
					' ' + datename(year, @dtExpiry) + ' ' + CASE @iIsCall when 1 then 'Call' else 'Put' end +
						 ' ' + cast(round(@fStrike, 2) as varchar)
 
	Begin Transaction
	   if (@iOptionID is Null)	
		 begin
			insert into Contract (contractTypeID, symbol, contractName)
				values (3, @vcSymbol, @ContractName)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iOptionID = @@identity			  

			insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
				values (@iOptionID, @iUnderlyingContractID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		 end
	   else
		 begin
			update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@ContractName,contractName)
				where contractID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			update [Option]
				set underlyingContractID = @iUnderlyingContractID,
					isCall = isNull(@iIsCall, isCall),
					strike = isNull(@fStrike, strike),
					expiryID = isNull(@iExpiryID, expiryID)
				where optionID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		 end		

		if @tiIsManualyEntered in (0,1) 
			begin
--select @iOptionID as '@iOptionID*******************'
				update [Option] set isManualyEntered = @tiIsManualyEntered where optionID = @iOptionID
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end 

	Commit Transaction

    set @error = 0
finish:
if (@error <> 0) return (@error) else return @iOptionID











GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO









CREATE  Procedure dbo.usp_PriceCloseDH_Get
	@vcSymbol varchar(20),	--ticker of stock  
	@fPriceClose float = Null Output
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from ContractPrice table for DeltaHedger
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select @fPriceClose = priceClose
		from ContractPrice	
			join ContractView Contract on Contract.contractID=ContractPrice.contractID
		where contractTypeID in (1,2) 
			and exchangeID is null
			and Contract.symbol=@vcSymbol
	set @error = @@error
Return(@error)









GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO









CREATE   Procedure dbo.usp_PriceCloseDH_Save 
	@vcSymbol varchar(20),	--ticker of stock(index)  
	@fPriceClose float
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from ContractPrice table for DeltaHedger
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	declare @iSymbolID int
	select @iSymbolID=contractID from Contract where symbol=@vcSymbol and ContractTypeID < 3
	Begin Transaction
		if not exists(select * From ContractPrice where contractID=@iSymbolID and exchangeID is null) 
			begin
				insert into ContractPrice (contractID,exchangeID,priceClose) 
				values (@iSymbolID,null,@fPriceClose)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		else
			begin
				update ContractPrice set 
					priceClose = @fPriceClose
				where contractID=@iSymbolID and exchangeID is null
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction
    set @error = 0
finish:
Return(@error)









GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO









CREATE   Procedure dbo.usp_StockDataHistoryDH_Get
	@vcSymbol varchar(20),	--ticker of stock 
	@fCurrentIV float = null Output, 
	@fCurrentHV float = null Output, 
	@fCurrentVolOfVol float = null Output 
as
/*
	Created by Zaur Nuraliev
	this procedure returns historical data for stock by ticker
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iStockID int 
	select @iStockID=contractID from ContractView where symbol=@vcSymbol and contractTypeID = 2
	select 	@fCurrentIV = IVMean, 
			@fCurrentHV = currentHV,
			@fCurrentVolOfVol = currentVolOfVol
		from StockDataHistory
		where tenorID=8 
			and [date]=(select max([date]) from StockDataHistory where stockID=@iStockID and tenorID=8)
			and stockID=@iStockID
	set @error = @@error
finish:
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE procedure dbo.usp_StockID_Get
	   @vcSymbol varchar(20) 
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting stock identifiers
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select C.contractID as iStockID
		from ContractView c 	
				join StockView S on S.stockID = C.contractID
		where C.contractTypeID = 2 and C.symbol = @vcSymbol
	set @error = @@error
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE     proc dbo.usp_Stock_Save
        @iStockID int,
        @iExchangeID int = Null,
        @vcSymbol varchar(20) = Null,
        @vcContractName varchar(255) = Null,
		@bIsHTB  bit = Null,
        @iDivFreq int = Null,
		@fDivAmt float = Null,
		@dtDivDate smalldatetime = Null,
		@tiIsDivCustom tinyint = 0,
		@fDivAmtCustom float = Null,
		@iDivFreqCustom int = Null,
		@dtDivDateCustom smalldatetime = Null,
		@tiCalcOptionType tinyint = null,
		@tiCalcModelType tinyint = null,
		@iExpCalendarID int = null,
		@tiIsActive tinyint = null
  as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @bIsHTB is null 
		set @bIsHTB=0

	if @iExpCalendarID is null or @iExpCalendarID = 0
		set @iExpCalendarID = 1
   
	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID=2 and actionID < 3 and (@iStockID <> contractID or @iStockID is null))
		begin
			Raiserror ('Stock with this ticker already exists!', 16, 1)
			Return (-1)
		end   
   if (@iStockID is Null)  
     begin
         begin tran
            insert into Contract (contractTypeID, Symbol, contractName, expCalendarID)
                  values (2, @vcSymbol, @vcContractName, @iExpCalendarID)
			if (@@error <> 0) begin rollback tran return(-1) end        

            set @iStockID = @@identity         
 
            insert into Stock (
				stockID, 
                divFreq, 
				isHTB,
				primaryExchangeID,
				divAmt,
				divDate,
				isDivCustom,
				divAmtCustom,
				divFreqCustom,
				divDateCustom,
				calcOptionType,
				calcModelType,
				isActive)
            values (
				@iStockID, 
                @iDivFreq, 
				@bIsHTB,
				@iExchangeID,
				@fDivAmt,
				@dtDivDate,
				@tiIsDivCustom,
				@fDivAmtCustom,
				@iDivFreqCustom,
				@dtDivDateCustom,
				@tiCalcOptionType,
				@tiCalcModelType,
				@tiIsActive)
			if (@@error <> 0) begin rollback tran return(-1) end           
			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @iStockID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@iStockID,null)
					if (@@error <> 0) begin rollback tran return(-1) end           
				end 
         commit tran
     end
   else
     begin

        declare @df int

        select @df = DivFreq from Stock where stockID = @iStockID        
        begin tran
           update Contract
                set symbol = isNull(@vcSymbol,symbol),
                    contractName = isNull(@vcContractName,contractName),
					expCalendarID = isNull(@iExpCalendarID,expCalendarID)
	            where contractID = @iStockID
			if (@@error <> 0) begin rollback tran return(-1) end         

			update Stock
	               set  
	                    divFreq = isNull(@iDivFreq,divFreq), 
						isHTB = isNull(@bIsHTB, isHTB),
						primaryExchangeID = isNull(@iExchangeID,primaryExchangeID),
						DivAmt = isNull(@fDivAmt, DivAmt),
						DivDate = isNull(@dtDivDate, DivDate),
						isDivCustom = isNull(@tiIsDivCustom, isDivCustom),
						divAmtCustom = isNull(@fDivAmtCustom, divAmtCustom),
						divFreqCustom = isNull(@iDivFreqCustom, divFreqCustom),
						divDateCustom = isNull(@dtDivDateCustom, divDateCustom),
						calcOptionType = isNull(@tiCalcOptionType, calcOptionType),
						calcModelType = isNull(@tiCalcModelType, calcModelType),
						isActive = isnull(@tiIsActive, isActive)
		          where stockID = @iStockID
    	       if (@@error <> 0) begin rollback tran return(-1) end         

			if @iDivFreq = 0 
				begin
					update Stock set divAmt = null, divDate = null where stockID = @iStockID
				end

			if @iDivFreqCustom = 0 
				begin
					update Stock set divAmtCustom = null, divDateCustom = null where stockID = @iStockID
				end

         commit tran
     end        

 if (@@error <> 0)
    return (-1)
  else  
    return @iStockID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.34.004')
	begin
		update DataInfo set keyValue = '3.34.004' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.34.004'
	end