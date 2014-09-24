/*
Import market structure improvements
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.43.085' or (keyValue = '3.43.086')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.43.086 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.43.086' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Option_Check' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Option_Check
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodUndOptionWithPos_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodUndOptionWithPos_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOptionWithPos_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOptionWithPos_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOptionRootWithPosition_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOptionRootWithPosition_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOptionRoot_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOptionRoot_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOptionRoot_Import' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOptionRoot_Import
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOption_Import' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOption_Import
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_EodOptionName_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_EodOptionName_Get
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_Option_Import' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_Option_Import
GO
IF EXISTS (SELECT name 
	   FROM   sysobjects 
	   WHERE  name = N'usp_ReportsUnderlyingOptions_Get' 
	   AND 	  type = 'P')
    DROP PROCEDURE usp_ReportsUnderlyingOptions_Get
GO


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE      PROCEDURE dbo.usp_Option_Check 
	@iOptionID int = null, 
	@vcSymbol varchar(20),
	@iOptionRootID int,
	@dtExpiry smalldatetime,
	@iExpiryID int,
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
			@dtExpiry_bad smalldatetime,
			@fStrike_bad float,
			@iIsCall_bad tinyint,
			@vcSymbol_bad varchar(20)

	if @iOptionID is null
		select @iOptionID_bad = optionID 
			from OptionView O join Contract C on O.optionID = C.contractID
			where C.symbol = @vcSymbol and
			(optionRootID != @iOptionRootID or expiryID != @iExpiryID or abs(strike - @fStrike) > 0.009 or isCall != @iIsCall)
	else
		select @iOptionID_bad = optionID 
		from OptionView O join Contract C on O.optionID = C.contractID
		where optionID != @iOptionID and C.symbol = @vcSymbol and
		(optionRootID != @iOptionRootID or expiryID != @iExpiryID or abs(strike - @fStrike) > 0.009 or isCall != @iIsCall)


	if @iOptionID_bad is not null
	begin
		if @bRemoveOption is not null
			begin
				exec @error = usp_Option_Del @iOptionID = @iOptionID_bad,@bisRaiseError = 0
			end
		
		if (@error = -2) or (@bRemoveOption is null)
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
				select @dtExpiry_bad = expiryPeriod 
						from ExpiryView E join OptionView O on E.expiryID = O.expiryID 
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
				WHEN @error != -2 THEN
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
			where (optionRootID = @iOptionRootID and expiryID = @iExpiryID and abs(strike - @fStrike) < 0.009 and isCall = @iIsCall)
	else
		select @iOptionID_bad = optionID, @vcSymbol_bad = C.symbol
			from OptionView O join ContractView C on O.optionID = C.contractID
			where (optionID != @iOptionID) and
			(optionRootID = @iOptionRootID and expiryID = @iExpiryID and abs(strike - @fStrike) < 0.009 and isCall = @iIsCall)

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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE    PROCEDURE dbo.usp_EodOptionRoot_Get 
	@iUnderlyingID int = null
AS
/*
	Created by: Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iUnderlyingID is null
	begin
		select distinct
			ORT.underlyingID as iUnderlyingID,
			ORT.optionRootID as iRootID,
			ORT.symbol as vcSymbol,
			ORT.IsSynthetic as tiIsSynthetic, 
			ORT.lotSize as iLotSize, 
			SOR.cashValue as fCash,
			SOR.synthSkew as fSyntheticSkew, 
			SOR.synthKurt as fSyntheticKurt,
			SOR.synthYield as fSyntheticYield, 
			SOR.synthIsBasket as tiSyntheticIsBasket
		from
			OptionRootView as ORT
			left join SyntheticOptionRootView as SOR on ORT.optionRootID = SOR.optionRootID
			join OptionView O on O.optionRootID=ORT.optionRootID  
	end
	else
	begin
		select distinct
			ORT.underlyingID as iUnderlyingID,
			ORT.optionRootID as iRootID,
			ORT.symbol as vcSymbol,
			ORT.IsSynthetic as tiIsSynthetic, 
			ORT.lotSize as iLotSize, 
			SOR.cashValue as fCash,
			SOR.synthSkew as fSyntheticSkew, 
			SOR.synthKurt as fSyntheticKurt,
			SOR.synthYield as fSyntheticYield, 
			SOR.synthIsBasket as tiSyntheticIsBasket
		from
			OptionRootView as ORT
			left join SyntheticOptionRootView as SOR on ORT.optionRootID = SOR.optionRootID
			join OptionView O on O.optionRootID=ORT.optionRootID  
		where
			ORT.underlyingID = @iUnderlyingID
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


CREATE     PROCEDURE dbo.usp_EodOptionWithPos_Get
	@iUnderlyingID int = null,
	@dtMinExpDate datetime = Null
AS
/*
	Created by: Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @dtMinExpDate is null set @dtMinExpDate = getdate()

	if @iUnderlyingID is null
	begin
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
			TradeView tv  
			left join OptionView o on o.optionID = tv.contractID
			left join ContractView c on c.contractID = tv.contractID
			left join OptionRootView ort on o.optionRootID = ort.optionRootID
			left join SyntheticOptionRootView as sor on ort.optionRootID = sor.optionRootID
			left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
			left join ContractView uc on ort.underlyingID = uc.contractID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
		where
			c.contractTypeID = 3
			and ec.expiryDate >= @dtMinExpDate
		order by ort.underlyingID,ort.optionRootID
	end
	else
	begin
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
			TradeView tv
			left join OptionView o on o.optionID = tv.contractID
			left join ContractView c on c.contractID = tv.contractID
			left join OptionRootView ort on o.optionRootID = ort.optionRootID
			left join SyntheticOptionRootView as sor on ort.optionRootID = sor.optionRootID
			left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
			left join ContractView uc on ort.underlyingID = uc.contractID
			left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
		where
			c.contractTypeID = 3
			and ort.underlyingID = @iUnderlyingID
			and ec.expiryDate >= @dtMinExpDate
		order by ort.underlyingID,ort.optionRootID
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


CREATE   PROCEDURE dbo.usp_EodOptionRoot_Import
	@iUnderlyingID int,
	@vcSymbol varchar(20),
	@iLotSize int
AS
/*
	Created by: Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @iRootID int,
   			@ERR_MSG varchar(8000)

    if exists (select optionRootID from OptionRootView
				   where symbol = @vcSymbol and underlyingID<>@iUnderlyingID)
		begin			
			Declare @vcDifferentUndSymbol varchar(8)
			select @vcDifferentUndSymbol = C.symbol from ContractView C join
					OptionRootView ort on ort.underlyingID = C.contractID
					where ort.symbol = @vcSymbol and
					ort.underlyingID<>@iUnderlyingID
					
			set @ERR_MSG = 'Option Root ' + '''' + @vcSymbol + '''' + ' already assigned to different underlying ' + '''' +  @vcDifferentUndSymbol + '''' + '. Please, rename Option Root and restart importing of market structure.'			
			set @iRootID = -1
			goto finish
		end  
	
	select @iRootID = optionRootID from OptionRootView
	   where symbol = @vcSymbol and underlyingID=@iUnderlyingID

	if @iRootID is null
	begin
		insert into OptionRoot (underlyingID, symbol, isSynthetic, lotSize)
		values (@iUnderlyingID,@vcSymbol,0,@iLotSize)

		if @@error <> 0 set @iRootID = -2
		else set @iRootID = @@identity
	end

finish:
select @ERR_MSG	as vcErrorMsg
Return(@iRootID)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE	PROCEDURE dbo.usp_EodOption_Import
	@iRootID int,
	@iExpiryID int,
	@fStrike float,
	@iIsCall tinyint,
	@vcSymbol varchar(20),
	@vcContractName varchar(255),
	@iOptionID_bad int output
as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0
	declare	@OptionID int

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	select @OptionID = optionID
			from [Option] (nolock)
			where optionRootID = @iRootID and 
				isCall = @iIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3

	if @OptionID is not null
	begin
		update Contract  set symbol = @vcSymbol, contractName = @vcContractName where contractID=@OptionID 
		if @@error <> 0 begin set @error = -1 goto finish end
	end
	else
	begin
		Begin Transaction

			select @iOptionID_bad = o.optionID
			from OptionView o join ContractView c on c.contractID = o.optionID
			where c.symbol = @vcSymbol and
				(o.optionRootID <> @iRootID or o.isCall <> @iIsCall or abs(strike - @fStrike) >= 0.009 or expiryID <> @iExpiryID)

			if @iOptionID_bad is not null
			begin
				exec @error = usp_Option_Del @iOptionID = @iOptionID_bad,@bisRaiseError = 0				
			end

			if @error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into Contract (contractTypeID, symbol, contractName)
				 values (3, @vcSymbol,@vcContractName)
			set @OptionID = @@identity
			if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				 values (@OptionID, @iRootID, @iIsCall, @fStrike, @iExpiryID)
			if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@OptionID,null)
					if (@@error <> 0) begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
				end 

		Commit Transaction
	end 
	 
   set @error = @OptionID
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


CREATE             proc dbo.usp_EodOptionName_Get
	@iOptionID int
  as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select symbol as vcSymbol, contractName as vcContractName from ContractView where contractID=@iOptionID


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


CREATE	PROCEDURE dbo.usp_Option_Import
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

		delete from TradeLog
		where contractID = @OptionID

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
			expiryID = isnull(@iExpiryID,expiryID)
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
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE PROC dbo.usp_ReportsUnderlyingOptions_Get
	@iUnderlyingID int,
	@dtMinExpDate datetime = Null
AS
/*
	Created by: Sharky
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @iMinExpID int

	if @dtMinExpDate is not null
		set @iMinExpID = datediff(mm, '19991201', @dtMinExpDate)
	else
		set @iMinExpID = 0

	select
		c.contractID as iOptionID,
		c.symbol as vcSymbol,
		ort.optionRootID as iOptionRootID,
		o.expiryID as iExpiryID,
		o.strike as fStrike, 
		o.isCall as tiIsCall
	from
		ContractView c  
		inner join OptionView o on o.optionID = c.contractID
		inner join OptionRootView ort on o.optionRootID = ort.optionRootID
	where
		c.contractTypeID = 3
		and ort.underlyingID = @iUnderlyingID
		and o.expiryID >= @iMinExpID
		
	set @error = @@error
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.43.086')
	begin
		update DataInfo set keyValue = '3.43.086' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.43.086' '3.43.086 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
