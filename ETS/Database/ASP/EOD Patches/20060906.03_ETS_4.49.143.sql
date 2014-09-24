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
--BEGIN TRANSACTION
--GO
--SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
--GO
/*CHECKING of DB VERSION*/
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.142' or (keyValue = '4.49.143')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.143 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.142' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
if exists (select * from dbo.sysindexes where name = N'IX_CustomStrikeSkewPoint_9' and id = object_id(N'[dbo].[CustomStrikeSkewPoint]'))
drop index [dbo].[CustomStrikeSkewPoint].[IX_CustomStrikeSkewPoint_9]
GO
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[OptionContractsView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[OptionContractsView]
GO
--!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
CREATE  VIEW [dbo].[OptionContractsView]
        WITH SCHEMABINDING AS 
		SELECT  [dbo].[option].[strike] as Strike,  
			[dbo].[option].[iscall] as isCall,  
			[dbo].[contract].[contractid] as ContractID,  
			[dbo].[contract].[symbol] as Symbol,  
			[dbo].[option].[expiryDate] as expiryDate,
			[dbo].[option].[optionrootid] as OptionRootId 
		FROM  [dbo].[option],  [dbo].[contract]   
		WHERE ( [dbo].[option].[optionid] = [dbo].[contract].[contractid] ) AND ( [dbo].[option].[actionid] < 3 ) AND ( [dbo].[contract].[contracttypeid] = 3 ) 


GO
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[OptionView]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[OptionView]
GO

GO
CREATE View dbo.OptionView
as
	Select 
		optionID,
		optionRootID,
		isCall,
		strike,
		expiryDate,
		actionID,
		actionDate,
		isManualyEntered
	from [Option]
	where actionID < 3


GO


ALTER    PROC usp_BoFutureRoot_Get
		@iUnderlyingID INT = NULL
AS
  -- retrieve future root(s) BY underlying
	SET NOCOUNT ON
	IF @iUnderlyingID IS NOT NULL
		SELECT
			fr.futureRootID AS iFutureRootID, 
			uc.contractID AS iUnderlyingID, 
			uc.contractTypeID AS iUnderlyingContractTypeID, 
			uc.symbol AS vcUnderlyingSymbol, 
			fr.futureRootSymbol AS vcFutureRootSymbol, 
			fr.futureRootName AS vcFutureRootName,
			fr.futureLotSize AS iFutureLotSize,
			fr.optionLotSize AS iOptionLotSize, 
			fr.exportSymbol AS vcExportSymbol,
			fr.actionDate AS dtActionDate, 
			fr.tiQuoteFormat, fr.iQuotationUnitAmmount,
			fr.iQuotationUnitID, 
			fr.fQuoteDenomination, 
			fr.fTickSize, 
			fr.fTickValue, 
			fr.vcQuotationUnitCurrency AS vcQuotationUnitName
		FROM 
			FutureRoot fr
				INNER JOIN ContractView uc ON fr.underlyingID = uc.contractID
				LEFT JOIN QuotationUnit qu ON fr.iQuotationUnitID = qu.iQuotationUnitID
		WHERE
			fr.underlyingID = @iUnderlyingID
		ORDER BY
			fr.futureRootSymbol
	ELSE
		SELECT
			fr.futureRootID AS iFutureRootID, 
			uc.contractID AS iUnderlyingID, 
			uc.contractTypeID AS iUnderlyingContractTypeID,
			uc.symbol AS vcUnderlyingSymbol, 
			fr.futureRootSymbol AS vcFutureRootSymbol, 
			fr.futureRootName AS vcFutureRootName,
			fr.futureLotSize AS iFutureLotSize, 
			fr.optionLotSize AS iOptionLotSize, 
			fr.exportSymbol AS vcExportSymbol,
			fr.actionDate AS dtActionDate, 
			fr.tiQuoteFormat, fr.iQuotationUnitAmmount,
			fr.iQuotationUnitID, fr.fQuoteDenomination, 
			fr.fTickSize, 
			fr.fTickValue, 
			fr.vcQuotationUnitCurrency AS vcQuotationUnitName
		FROM 
			FutureRoot fr
				INNER JOIN ContractView uc ON fr.underlyingID = uc.contractID
				LEFT JOIN QuotationUnit qu ON fr.iQuotationUnitID = qu.iQuotationUnitID
		ORDER BY
			uc.symbol, fr.futureRootSymbol

GO
ALTER   proc dbo.usp_BoFutureOptionWithPrice_Get
	  @iFutureID int		 
  as
	select	c.contractID as iContractID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName,
			null as iOptionRootID,
			fo.expiryDate as dtExpiry,
			fo.strike as fStrike,
			fo.isCall as bIsCall,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheo,
			c.exportSymbol as vcExportSymbol
	from FutureOption fo
			inner join ContractView c on c.contractID = fo.futureOptionID
			inner join ContractView fc on fc.contractID = fo.futureID
			--inner join ExpiryInCalendarView ec on fo.expiryID = ec.expiryID and fc.expCalendarID = ec.expCalendarID
			left join ContractPrice cp on fo.futureOptionID = cp.contractID and cp.exchangeID is null
	where	fo.futureID = @iFutureID
			and datediff(d, GetDate(), fo.expiryDate) >=0
	order by fo.expiryDate, fo.strike, fo.isCall desc

  return @@error
GO
ALTER     PROC dbo.usp_BoFuture_Get
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
			fr.exportSymbol as vcFutureRootExportSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			f.maturityDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			fr.futureLotSize as iFutureLotSize,
			f.actionDate as dtActionDate
			--fc.importID as ImportID
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
			--inner join ExpiryInCalendarView frmc on f.maturityID = frmc.expiryID and fr.matCalendarID = frmc.expCalendarID
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
			fr.exportSymbol as vcFutureRootExportSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			f.maturityDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			fr.futureLotSize as iFutureLotSize,
			f.actionDate as dtActionDate
			--fc.importID as ImportID
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
			--inner join ExpiryInCalendarView frmc on f.maturityID = frmc.expiryID and fr.matCalendarID = frmc.expCalendarID
		order by
			fc.symbol




GO
ALTER   proc dbo.usp_BoOptionWithPrice_Get
	  @iUnderlyingID int		 
  as
	select	c.contractID as iContractID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName,
			o.optionRootID as iOptionRootID,
			o.expiryDate   as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheo,
			c.exportSymbol as vcExportSymbol,
			o.IsManualyEntered as vcIsFlex
	from OptionRoot ort
			inner join [Option] o on o.optionRootID = ort.optionRootID
			inner join ContractView c on c.contractID = o.optionID
			left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
	where	ort.underlyingID = @iUnderlyingID
			and datediff(d, GetDate(), o.expiryDate) >=0
	order by ort.optionRootID, o.expiryDate, o.strike, o.isCall desc
 
  return @@error

GO
ALTER   PROCEDURE dbo.usp_Option_Check 
	@iOptionID int = null, 
	@vcSymbol varchar(20),
	@iOptionRootID int,
	@dtExpiry datetime,
	@fStrike float,
	@iIsCall tinyint,
	@vcUnderlyingSymbol varchar(8),
	@ERR_MSG varchar(8000) = null output,
	@bRemoveOption bit = null   -- call usp_Option_Del for this option
AS
/*
	Created by: Zaur
	Description: this procedure created with template, desined by Zaur Nuraliev
		and check if option with same symbol exists
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare @iOptionID_bad int,
		@vcUnderlying_bad varchar(8),
		@vcOptionRoot_bad varchar(20),
		@dtExpiry_bad datetime,
		@fStrike_bad float,
		@iIsCall_bad tinyint,
		@vcSymbol_bad varchar(20)

	if @iOptionID is null
		select @iOptionID_bad = optionID 
			from OptionView O join Contract C on O.optionID = C.contractID
			where C.symbol = @vcSymbol and
			(optionRootID != @iOptionRootID or 
                         datediff(d, expiryDate, @dtExpiry)<>0 or 
                         abs(strike - @fStrike) > 0.009 or isCall != @iIsCall)
	else
		select @iOptionID_bad = optionID 
			from OptionView O join Contract C on O.optionID = C.contractID
			where optionID != @iOptionID and C.symbol = @vcSymbol and
			(optionRootID != @iOptionRootID or 
                         datediff(d, expiryDate, @dtExpiry)<>0 or 
                         abs(strike - @fStrike) > 0.009 or isCall != @iIsCall)

	if @iOptionID_bad is not null
	begin
		if @bRemoveOption is not null
			begin
				exec @error = usp_Option_Del @iOptionID = @iOptionID_bad, @bisRaiseError = 0
			end
		
		if (@error = -2) or (@error = -3) or (@bRemoveOption is null)
			begin
				select @vcSymbol_bad = symbol from ContractView where contractID = @iOptionID_bad
				select	@vcOptionRoot_bad = ORT.symbol
						from OptionView O join OptionRootView ORT on ORT.optionRootID = O.optionRootID
						where O.optionID = @iOptionID_bad
				select	@vcUnderlying_bad = C.symbol
						from ContractView C 
						join OptionRootView ORT on ORT.underlyingID = C.contractID
						join OptionView O on ORT.optionRootID = O.optionRootID
						where O.optionID = @iOptionID_bad
				select @dtExpiry_bad =  expiryDate
						from OptionView O 
						where O.optionID = @iOptionID_bad
				select 	@fStrike_bad = strike,
						@iIsCall_bad = isCall 
						from OptionView where optionID = @iOptionID_bad
				

			set @ERR_MSG = 'Can''t add option with Symbol=' + '''' +  @vcSymbol + ''''  +  
			CASE WHEN @vcUnderlying_bad != @vcUnderlyingSymbol THEN
				', Underlying=' + '''' + @vcUnderlyingSymbol + ''''
			ELSE '' END + 
			CASE WHEN left(@vcSymbol,len(@vcSymbol)-2) != left(@vcSymbol_bad,len(@vcSymbol_bad)-2) THEN
				', Option Root=' + '''' + left(@vcSymbol,len(@vcSymbol)-2) + ''''
			ELSE '' END + 
			CASE WHEN @fStrike!=@fStrike_bad THEN
				', Strike=' + CAST(ROUND(@fStrike, 2) as varchar(10)) 
			ELSE '' END + 
			CASE WHEN cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) != cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) THEN
				', Expiry=' + '''' + cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) + '''' 
			ELSE '' END +  
			CASE WHEN @iIsCall!=@iIsCall_bad THEN
				'''' + ', Call\Put' + '''' + '=' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END  
			ELSE '' END +  
			CASE WHEN @error = -2 THEN
				' because exists option with trades and Symbol=' + '''' +  @vcSymbol_bad + ''''
				WHEN @error = -3 THEN
				' because exists option with orders and Symbol=' + '''' +  @vcSymbol_bad + ''''
				WHEN @error != -2 and @error != -3 THEN
				' because exists option with Symbol=' + '''' +  @vcSymbol_bad + ''''
			ELSE '' END + 
			CASE WHEN @vcUnderlying_bad != @vcUnderlyingSymbol THEN
				', Underlying=' + '''' + @vcUnderlying_bad + ''''
			ELSE '' END + 
			CASE WHEN left(@vcSymbol,len(@vcSymbol)-2) != left(@vcSymbol_bad,len(@vcSymbol_bad)-2) THEN
				', Option Root=' + '''' + left(@vcSymbol_bad,len(@vcSymbol_bad)-2) + ''''
			ELSE '' END + 
			CASE WHEN @fStrike!=@fStrike_bad THEN
				', Strike=' + CAST(ROUND(@fStrike_bad, 2) as varchar(10)) 
			ELSE '' END + 
			CASE WHEN cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) != cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) THEN
				', Expiry=' + '''' + cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) + ''''
			ELSE '' END + 
			CASE WHEN @iIsCall!=@iIsCall_bad THEN
				'''' + ', Call\Put' + '''' + '=' + CASE @iIsCall_bad WHEN 1 THEN 'Call' ELSE 'Put' END
			ELSE '' END +
			CASE WHEN @vcUnderlying_bad = @vcUnderlyingSymbol THEN
			', Underlying=' + '''' + @vcUnderlyingSymbol + ''''
			ELSE '' END
		end	
		Return(-1)
	end
	else
		begin
			set @ERR_MSG = null
			set @iOptionID_bad = null
		end

	if @iOptionID is null
		select @iOptionID_bad = optionID, @vcSymbol_bad = C.symbol
			from OptionView O join ContractView C on O.optionID = C.contractID
			where (optionRootID = @iOptionRootID and datediff(d, expiryDate, @dtExpiry) = 0 and abs(strike - @fStrike) < 0.009 and isCall = @iIsCall)
	else
		select @iOptionID_bad = optionID, @vcSymbol_bad = C.symbol
			from OptionView O join ContractView C on O.optionID = C.contractID
			where (optionID != @iOptionID) and
			(optionRootID = @iOptionRootID and datediff(d, expiryDate, @dtExpiry) = 0 and abs(strike - @fStrike) < 0.009 and isCall = @iIsCall)

	if @iOptionID_bad is not null
	begin
		if @bRemoveOption = 1
			begin
				--===================================================================
				-- update option symbol, if option with same attributes exists
				--===================================================================		
				update contract set symbol = @vcSymbol where contractID = @iOptionID_bad
				set @ERR_MSG = '*'
			end
		else
			begin
				set @ERR_MSG = 'Can''t add option ' + '''' +  @vcSymbol + ''''  +  
				' because already exists option ' + '''' +  @vcSymbol_bad + '''' +
				' with the same parameters'
			end
	end

Return(@@error)
GO
ALTER   PROCEDURE usp_Option_Import
	@iUnderlyingContractID int,
	@vcSymbol varchar(20),
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry datetime,
	@iLotSize int,
	@iMsCacheOptionID int = NULL,
	@vcSymbolUnderlying varchar(20) = NULL,
	@fPriceClose float = NULL
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
			@iLastExpiryID int,
			@vcUnderlyingSymbol varchar(8),
			@ERR_MSG varchar(8000)

	if @vcSymbolUnderlying is null
      		select @vcUnderlyingSymbol = symbol from ContractView where contractID = @iUnderlyingContractID
	else
		set  @vcUnderlyingSymbol = @vcSymbolUnderlying

	--===================================================================			
	-- check the Expiry
	--===================================================================			

	/*if not exists(select * from Expiry where expiryID = @iExpiryID)
		begin
			set @ERR_MSG = 'Incorrect expiration! '  + '''' +  Convert(varchar(50),@dtExpiry) + '''' + ' for symbol=' + '''' + @vcSymbol + '''' +  ' UnderlyingSymbol=' + '''' + @vcUnderlyingSymbol + '''' + ' Strike=' + Convert(varchar(15),@fStrike) + ' expiryID=' + Convert(varchar(15),@iExpiryID)
			Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end
	*/
	--===================================================================			
	-- check symbol of option root
	--===================================================================			  
   DECLARE @vcOptionRootSymbol varchar(5)	
   set 	@vcOptionRootSymbol = left(@vcSymbol,len(@vcSymbol)-2)

   if exists (select optionRootID 
				   from OptionRootView
				   where symbol = @vcOptionRootSymbol and 
					underlyingID<>@iUnderlyingContractID)
		begin			
			Declare @vcDifferentUndSymbol varchar(8)
			select @vcDifferentUndSymbol = C.symbol from ContractView C join
					OptionRootView ort on ort.underlyingID = C.contractID
					where ort.symbol = @vcOptionRootSymbol and
					ort.underlyingID<>@iUnderlyingContractID
					
			set @ERR_MSG = 'Incorrect option symbol! '  + '''' +  @vcSymbol + ''''  +  ': Option Root=' + '''' + @vcOptionRootSymbol + '''' + ' assign to different underlying=' + '''' +  @vcDifferentUndSymbol + '''' + '. Please, rename Option Root and restart importing of market structure.'
			Raiserror(@ERR_MSG,16,1)
			RETURN (-1)
		end  


	--select top 1 @iLastExpiryID = expiryID from ExpiryInCalendarView where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc
	--select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	--===================================================================			
	-- select the @iOptionRootID
	--===================================================================			
	declare @iOptionRootID int,
			@vcOptionRoot varchar(20)
	select @iOptionRootID = optionRootID from OptionRootView where symbol = @vcOptionRootSymbol
			and underlyingID = @iUnderlyingContractID 

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	if @iOptionRootID is not null 
		select @OptionID = optionID
			from [Option] (nolock)
			where optionRootID = @iOptionRootID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and datediff(d, expiryDate ,@dtExpiry) = 0 and actionID<>3

	--===================================================================			
	-- check if option with same symbol exists
	--===================================================================		
	set @ERR_MSG = null

	execute usp_Option_Check @iOptionID = @OptionID, @vcSymbol = @vcSymbol,	@iOptionRootID = @iOptionRootID,
			@dtExpiry = @dtExpiry,@fStrike = @fStrike,@iIsCall = @iIsCall,
			@vcUnderlyingSymbol = @vcUnderlyingSymbol,@bRemoveOption = 1,@ERR_MSG = @ERR_MSG output

	if @ERR_MSG is not null and @ERR_MSG != ''
	begin
		if @ERR_MSG != '*' Raiserror(@ERR_MSG,16,1)
		Return(-1)
	end

	--===================================================================			
	-- make the contract name
	--===================================================================			
  	set @cn = @vcUnderlyingSymbol + ' ' + DATENAME(month, @dtExpiry) +',' + DATENAME(day, @dtExpiry) + ' '  
					 + DATENAME(year, @dtExpiry) + ' ' + CASE @iIsCall WHEN 1 THEN 'Call' ELSE 'Put' END +
						 ' ' + CAST(ROUND(@fStrike, 2) as varchar)


  	if (@OptionID is Null) 
	begin
		Begin Transaction

			if  @iOptionRootID is null
			begin
				insert into OptionRoot (underlyingID,symbol,lotSize)
					select @iUnderlyingContractID, @vcOptionRootSymbol, isnull(@iLotSize,100)
				set @iOptionRootID = @@identity
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end 
	
			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@cn)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into [Option] (optionID, optionRootID, isCall, strike, expiryDate)
				 values (@OptionID, @iOptionRootID, @iIsCall, @fStrike, @dtExpiry)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			/*if @dtExpiry is not null
			begin
				select @iExpCalendarID = e.expCalendarID from ContractView cv 
					inner join ExpiryCalendarView e on e.expCalendarName = cv.symbol 
					where cv.ContractID = @iUnderlyingContractID and cv.contractTypeID = 1
				
					if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtIndexCalendarExpity, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @OptionID
					end
			end*/
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			DECLARE @error_var int, @rowcount_var int
			UPDATE ContractPrice 
				SET priceClose = isnull(@fPriceClose,priceClose)
			WHERE exchangeID is null and contractID = @OptionID

			SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
			if @error_var = 0
			begin
			  if @rowcount_var = 0
			  begin
				insert into ContractPrice(contractID,exchangeID,priceClose)
					values(@OptionID, null, @fPriceClose)
				if (@@error <> 0) begin rollback tran return(-1) end           
			  end	
			end
                        else
			 if (@@error <> 0) begin rollback tran return(-1) end           



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

		UPDATE ContractPrice 
			SET priceClose = isnull(@fPriceClose,priceClose)
		WHERE exchangeID is null and contractID = @OptionID

		update [Option]
			set 
			IsCall = isnull(@iIsCall,IsCall),
			strike = isnull(@fStrike,Strike),
			expiryDate = isnull(@dtExpiry,expiryDate),
			isManualyEntered = 0
		where optionID = @OptionID

		/*if @dtExpiry is not null
		begin
				select @iExpCalendarID = e.expCalendarID from ContractView cv 
					inner join ExpiryCalendarView e on e.expCalendarName = cv.symbol 
					where cv.ContractID = @iUnderlyingContractID and cv.contractTypeID = 1
				
				if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtIndexCalendarExpity, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @OptionID
					end
		end
*/
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
ALTER  PROCEDURE usp_ExpiredOptions_Del
	@iContractID int -- stock or index identifier
as
/*
	Created by Zaur Nuraliev
	Description: this procedure mark us deleted unusefull options
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Create table #Options_for_delete(OptionID int) 

	Create table #O_with_trades(symbol varchar(20)) 


	--=============================================================
	-- select options for remove
	--=============================================================
	insert into #Options_for_delete
		select O.optionID
			from 
				[Option] O
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract UC on ORt.underlyingID = UC.contractID 
			where ( datediff(d, GetDate(), O.expiryDate ) <0
					or ( datediff(d, O.actionDate, GetDate()) > 0
					and O.isManualyEntered = 0 ))
					and O.actionID < 3
					and O.optionID not in (select distinct ContractID from Trade)
					and O.optionID not in (select distinct ContractID from [Order])
					and UC.contractID = @iContractID

	--=============================================================
	-- select options, which have a trades or orders
	--=============================================================
	insert into #O_with_trades
		select C.symbol
			from 
				[Option] O
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract C on O.optionID = C.contractID
					join Contract UC on ORt.underlyingID = UC.contractID 
			where ( datediff(d,GetDate(),O.expiryDate ) <0 
					or ( datediff(d, O.actionDate, GetDate()) > 0
					and O.isManualyEntered = 0 ))
					and O.actionID < 3
					and (O.optionID in (select distinct ContractID from Trade) 
					or O.optionID in (select distinct ContractID from [Order]))
					and UC.contractID = @iContractID

	exec usp_Option_Del
		if @@error != 0 
			begin 
				Raiserror ('Error. Can''t mark us delete useless options!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

	select symbol as vcSymbol from #O_with_trades

set @error = 0

finish:
	Return(@error)

GO
ALTER	PROC usp_DefaultIVSurfaceStock_Get
	@iUnderlyingID INT
AS
	IF @iUnderlyingID IS NOT NULL
	BEGIN
		SELECT
			cv.ContractID,
			CONVERT(datetime, IV.dtExpDate, 112) AS dtExpDate,
			IV.fStrike,
			IV.fIV,
			GETDATE() AS dtActionDate
		FROM
			IVData.IV.Dbo.FT_DefaultIV IV
				INNER JOIN ContractView cv ON cv.Symbol = IV.vcSymbol AND
												cv.ContractTypeID IN (1,2) AND
												cv.ContractID = @iUnderlyingID
	END
	RETURN @@ERROR
GO
GO
ALTER PROCEDURE dbo.usp_CustomStrikeSkewDefaultPointFromIV_Save 
	@iContractID INT,
	@fStrike FLOAT = NULL,
	@dtExpDate DATETIME = NULL,
	@iType INT,
	@fVolatility FLOAT
AS
/*
	Created BY Michael Malyshkin
	Description: this procedure save inf. into CustomStrikeSkewPoINT table
*/
	-------------------------------------------------
	SET NOCOUNT ON
	-------------------------------------------------
	DECLARE @error int		SET @error = -1

	DECLARE @iCustomStrikeSkewPointID INT 
	SET @iCustomStrikeSkewPointID = NULL

	--check AND ALTER  record in VolaSurfaceData with 'IsDefaultSurface=1'
	DECLARE @iVolaSurfaceDataID INT 
	SET @iVolaSurfaceDataID = 0

	EXEC @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	-- mark olf strile skew points AS deleted
	--UPDATE CustomStrikeSkewPoint	SET actionID = 3
	DELETE
	FROM
		CustomStrikeSkewPoint
	WHERE 
		volaSurfaceDataID = @iVolaSurfaceDataID AND
		actionID < 3 AND
		DATEDIFF(d, actionDate, GetDate())>0

	SELECT
		@iCustomStrikeSkewPointID = CustomStrikeSkewPointID
	FROM
		CustomStrikeSkewPoint 
	WHERE
		volaSurfaceDataID = @iVolaSurfaceDataID AND
		ABS(strike - @fStrike) < 0.009 AND
		type = @iType AND
		ABS(DateDiff(d, expiryDate, @dtExpDate)) < 2 AND
		actionID < 3
  
	BEGIN TRANSACTION
		IF @iCustomStrikeSkewPointID IS NULL 
		BEGIN
			DECLARE @iContractType as integer 	
			DECLARE @dtOurExpiry as datetime

			select @iContractType = ContractTypeID from Contract where ContractID = @iContractID
			--Stock / Index
			IF @iContractType = 1 or @iContractType =2   
			BEGIN
				select @dtOurExpiry = o.expiryDate from [option] o
					inner join OptionRoot orr on orr.OptionRootId = o.optionRootID
					inner join ContractView uc on orr.UnderlyingID = uc.ContractID  and uc.ContractID = @iContractID
				where abs(datediff(d, o.expiryDate, @dtExpDate))<2
				group by o.expiryDate
			END

			--Future Underlying
			IF @iContractType = 6 
			BEGIN
				select @dtOurExpiry = o.expiryDate from [futureOption] o
					inner join Future f on o.futureID = f.futureID
					inner join FutureRoot fr on f.futureRootID = fr.futureRootID
					inner join ContractView uc on uc.ContractID = fr.UnderlyingID and uc.ContractID = @iContractID
				where abs(datediff(d, o.expiryDate, @dtExpDate))<2
				group by o.expiryDate			
			END
			if @dtOurExpiry is null
				set @dtOurExpiry = @dtExpDate

			INSERT INTO 
				CustomStrikeSkewPoINT
					(
						volaSurfaceDataID,
						type,
						strike,
						volatility,
						isBasePoint,
						expiryDate
					)
			VALUES	
				(
					@iVolaSurfaceDataID,
					@iType,	
					@fStrike,
					@fVolatility,
					-1,
					@dtOurExpiry
				)
	
				IF @@ERROR <> 0 BEGIN SET @error = 1 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

		END
		ELSE
		BEGIN
			UPDATE 
				CustomStrikeSkewPoint
			SET 
				volatility = ISNULL(@fVolatility,volatility)	
				--volaSurfaceDataID = ISNULL(@iVolaSurfaceDataID,volaSurfaceDataID),
				--type = ISNULL(@iType,type),
				--strike = ISNULL(@fStrike,strike),
				--expiryDate = ISNULL(@dtExpDate, expiryDate)
			WHERE
				customStrikeSkewPointID = @iCustomStrikeSkewPointID
	
	   		IF @@ERROR <> 0 BEGIN SET @error = 2 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
		END
	COMMIT TRANSACTION

    SET @error = 0

finish:
	RETURN(@error)


GO
GO

ALTER	PROC usp_DefaultIVSurfaceFutureUnd_Get
		@iFutUnderlyingID INT
AS
	IF @iFutUnderlyingID IS NOT NULL
	BEGIN
		SELECT
			QQ.ContractID AS ContractID,
			CONVERT(DATETIME, IV.dtExpDate, 112) AS dtExpDate,
			CAST( CAST(IV.dtExpDate AS DATETIME) AS INT ) AS iExpiryID,
			IV.fStrike AS fStrike,
			CAST(IV.fIV AS FLOAT) AS fIV,
			GETDATE() AS dtActionDate
		FROM
			(
				SELECT
					CU.ContractID,
					IV.fStrike,
					IV.dtExpDate,
					FR.UnderlyingID,
					MAX(FR.futureLotSize) AS maxLosSize,
					COUNT(FR.futureRootID) AS fRootsNumber
				FROM
					FutureRoot FR
						INNER JOIN IVData.IV.Dbo.FT_DefaultIV_Futures IV ON IV.vcSymbol = FR.futureRootSymbol
						INNER JOIN ContractView CU ON CU.ContractID = FR.underlyingID AND
									CU.ContractTypeID = 6 AND CU.ContractID = @iFutUnderlyingID
				GROUP BY IV.fStrike, IV.dtExpDate, FR.UnderlyingId, CU.ContractID
			) QQ
				INNER JOIN FutureRoot GH ON GH.underlyingID = QQ.UnderlyingID AND GH.futureLotSize = QQ.maxLosSize
				INNER JOIN IVData.IV.Dbo.FT_DefaultIV_Futures IV ON IV.vcSymbol = GH.futureRootSymbol AND
											QQ.fStrike = IV.fStrike AND QQ.dtExpDate = IV.dtExpDate
		ORDER BY QQ.UnderlyingID, QQ.dtExpDate, QQ.fStrike
	END
	RETURN @@ERROR
GO

-------------------------------------------------------------------------------------------------------------

GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_MmVegaWeight_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_MmVegaWeight_Get]
GO

CREATE 
  INDEX [IX_CustomStrikeSkewPoint_9] ON [dbo].[CustomStrikeSkewPoint] ([volaSurfaceDataID], [strike], [customStrikeSkewPointID], [type], [volatility], [expiryDate], [isBasePoint])
ON [PRIMARY]
GO

if exists (select * from dbo.sysindexes where name = N'IX_CustomStrikeSkewPoint' and id = object_id(N'[dbo].[CustomStrikeSkewPoint]'))
drop index [dbo].[CustomStrikeSkewPoint].[IX_CustomStrikeSkewPoint]
GO
CREATE 
  INDEX [IX_CustomStrikeSkewPoint] ON [dbo].[CustomStrikeSkewPoint] ([volaSurfaceDataID], [strike], [type], [expiryDate])
ON [PRIMARY]
GO

if exists (select * from dbo.sysindexes where name = N'volaSurfaceDataID_expDate_ind' and id = object_id(N'[dbo].[CustomStrikeSkewPoint]'))
drop index [dbo].[CustomStrikeSkewPoint].[volaSurfaceDataID_expDate_ind]
GO
CREATE 
  INDEX [volaSurfaceDataID_expDate_ind] ON [dbo].[CustomStrikeSkewPoint] ([volaSurfaceDataID], [expiryDate])
ON [PRIMARY]


IF EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='CustomStrikeSkewPoint' AND COLUMN_NAME='expiryID')
BEGIN
--DROP STATISTICS [CustomStrikeSkewPoint].hind_778485852_2A_1A_3A_4A_5A_6A_7A
ALTER TABLE [CustomStrikeSkewPoint] DROP COLUMN  expiryID 
END
GO
GO
GO
IF EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='Option' AND COLUMN_NAME='expiryID')
BEGIN
DROP STATISTICS [Option].hind_618485282_1A_5A_2A_3A_4A_8A
DROP STATISTICS [Option].hind_618485282_1A_4A_5A_8A_2A_3A
--DROP STATISTICS [Option].hind_618485282_2A_3A_4A_5A_8A
--DROP STATISTICS [Option].hind_618485282_2A_3A_5A_8A_4A
ALTER TABLE [Option] DROP COLUMN expiryID 
END
GO

GO
IF EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='Future' AND COLUMN_NAME='maturityID')
BEGIN
ALTER TABLE [Future] DROP COLUMN maturityID 
END
GO


GO
IF EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='FutureOption' AND COLUMN_NAME='expiryID')
BEGIN
ALTER TABLE [FutureOption] DROP COLUMN expiryID 
END
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.143')
	begin
		update DataInfo set keyValue = '4.49.143' where dataInfoID = 1
	end
GO
/* RESULTS ANALYTHING */
--IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
--GO
--IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
--GO
--IF EXISTS (SELECT * FROM #tmpErrors)
--BEGIN
--	ROLLBACK TRANSACTION
	--SELECT * FROM #tmpErrors
--END
GO
--IF @@TRANCOUNT > 0 
--BEGIN
	select 'DB VERSION WAS CHANGED TO 4.49.143' '4.49.143 update message:' 
	select 'The database update succeeded'
--
IF @@TRANCOUNT > 0 
BEGIN
	COMMIT TRANSACTION
END
--	ELSE select 'The database update failed'
--GO
DROP TABLE #tmpErrors
GO
-- unable to execute this script in transaction