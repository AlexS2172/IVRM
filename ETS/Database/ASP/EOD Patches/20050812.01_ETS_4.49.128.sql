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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.127' or (keyValue = '4.49.128')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.128 error message:' 
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


ALTER   PROCEDURE dbo.usp_EodUnderlying_Import
	@iContractID int,
	@bIsStock bit,
	@vcContractName varchar(255),
	@iDivFreq smallint,
	@fDivAmt float,
	@dtDivDate smalldatetime,
	@fPriceClose float = null,
        @bSaveClosePrice bit = null,
	@vcExchangeCode varchar(20) = null

as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction

	update Contract
	    set ContractName = @vcContractName,
			MSupdateDate = getDate()
 	where contractID = @iContractID

	if @@error <> 0 
		begin 
			Raiserror ('Error. Can''t update Contract Name!', 16, 1)
			set @error = -1 
			if @@TranCount = 1 Rollback Transaction goto finish 
		end

	delete from TradeLog
	where underlyingID = @iContractID
	if @@error <> 0 
		begin 
			Raiserror ('Error. Can''t delete record from TradeLog!', 16, 1)
			set @error = -1 
			if @@TranCount = 1 Rollback Transaction goto finish 
		end

	--========================================================================
	-- make record with null exchange for this stock
	--========================================================================
	if exists(select 1 from ContractPrice where exchangeID is null and contractID = @iContractID)
	begin
	      if @bSaveClosePrice is not null and  @bSaveClosePrice = 1
              begin		
		update ContractPrice
		set priceClose = @fPriceClose
		where exchangeID is null and contractID = @iContractID
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t update close price for default exchange!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish
			end
              end 
	end
	else
	begin
	      if @bSaveClosePrice is not null and  @bSaveClosePrice = 1
		insert into ContractPrice(contractID, exchangeID, priceClose)
			values(@iContractID, null, @fPriceClose)
	      else
		insert into ContractPrice(contractID, exchangeID, priceClose)
			values(@iContractID, null, null)
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t insert default exchange!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish
			end
	end
	--==================================================================================
	-- update DivAmnt & DivFreq values
	--==================================================================================
	if (@bIsStock <> 0 and exists(select 1 from Stock where stockID=@iContractID))
		begin 
			declare @exchangeID as int
			set @exchangeID = null
			if (@vcExchangeCode is not null)
			begin
				set @exchangeID = (select TOP 1 e.exchangeID from Exchange e where @vcExchangeCode = exchangeCode)
			end

			Declare @df smallint
			update Stock 
				set divAmt = @fDivAmt,
					divDate = @dtDivDate, 
					divFreq = @iDivFreq,
					primaryExchangeID = isNull(@exchangeID,primaryExchangeID)
			where stockID = @iContractID
			if (@@error <> 0)
				begin
					Raiserror ('Error. Can''t update Dividends and Primary Exchange!', 16, 1)
					set @error = -1 
					if @@TranCount = 1 Rollback Transaction goto finish 
				end
		end

	Commit Transaction
    set @error = 0
finish:
	Return(@error)


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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.128')
	begin
		update DataInfo set keyValue = '4.49.128' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.128' '4.49.128 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
-- unable to execute this script in transaction
