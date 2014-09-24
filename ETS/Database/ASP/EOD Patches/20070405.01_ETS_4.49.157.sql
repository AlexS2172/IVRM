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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.156'  or (keyValue = '4.49.157') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.157 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.157' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
--=============================================================================================
--=============================================================================================
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER     proc usp_Stock_Import
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
			 if @iExchangeID is null
                            begin	
				update ContractPrice 
					set priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					PriceOpen = @fPriceOpen,
					PriceClose = @fPriceClose,
					PriceLow = @fPriceLow,
					PriceHigh = @fPriceHigh,
					Volume = @iVolume
				where contractID = @iContractID 
			    end
                            else
                            begin	
				update ContractPrice 
					set priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					PriceOpen = @fPriceOpen,
					PriceClose = @fPriceClose,
					PriceLow = @fPriceLow,
					PriceHigh = @fPriceHigh,
					Volume = @iVolume
				where contractID = @iContractID and exchangeID=@iExchangeID
			    end

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
			set @exchangeID = (select TOP 1 e.exchangeID from Exchange e where @vcExchangeCode = exchangeCode and actionID < 3)
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
						Raiserror ('Error. Can''t update DivAmnt and DivDate and primaryExchangeID!', 16, 1)
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--=============================================================================================
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_BoAllStocksGet]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_BoAllStocksGet]


GO
CREATE  PROC usp_BoAllStocksGet
	@iFormat int = null
	--null - All Stocks (Stocks and Indeces)
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	if isnull(@iFormat,0) = 0
		select 
			case when i.IndexID is null then s.stockID else i.indexID end as iContractID,
			C.Symbol as sSymbol, 
			case when i.IndexID is null then ews.ExchangeCode else ewi.ExchangeCode end as sExchange,
			C.ContractTypeID as iContractType
		from Contract C 
			left join [index] i on i.IndexID = C.ContractID
			left join [stock] s on s.stockid = C.ContractID
			left join ExchangeView ews on ews.exchangeID = isnull(s.primaryExchangeID,0) and C.ContractTypeID = 2
			left join ExchangeView ewi on ewi.exchangeID = isnull(i.primaryExchangeID,0) and C.ContractTypeID = 1
		where C.ContractTypeID in (1,2) and C.actionID < 3
		order by C.symbol
-- 	else
-- 		select 
-- 			C.Symbol as sSymbol, 
-- 			case when i.IndexID is null then isnull(ews.ExchangeCode,'') else isnull(ewi.ExchangeCode,'') end as sExchange
-- 			case when i.IndexID is null then s.stockID else i.indexID end as iContractID
-- 		from Contract C 
-- 			left join [index] i on i.IndexID = C.ContractID
-- 			left join [stock] s on s.stockid = C.ContractID
-- 			left join ExchangeView ews on ews.exchangeID = isnull(s.primaryExchangeID,0) and C.ContractTypeID = 1
-- 			left join ExchangeView ewi on ewi.exchangeID = isnull(i.primaryExchangeID,0) and C.ContractTypeID = 2
-- 		where C.ContractTypeID in (1,2) and C.actionID < 3

	return @@error


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.157')
	begin
		update DataInfo set keyValue = '4.49.157' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.157' '4.49.157 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
