/*
implementation of enhancement MLine00001366
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.41.071' or (keyValue = '3.41.072')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.41.072 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.41.072' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Check]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Check]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE PROCEDURE dbo.usp_Option_Check 
	@iOptionID int = null, 
	@vcSymbol varchar(20),
	@iOptionRootID int,
	@dtExpiry smalldatetime,
	@iExpiryID int,
	@fStrike float,
	@iIsCall tinyint,
	@vcUnderlyingSymbol varchar(8),
	@ERR_MSG varchar(8000) = null output
AS
/*
	Created by: Zaur
	Description: this procedure created with template, desined by Zaur Nuraliev
		and check if option with same symbol exists
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	declare @iOptionID_bad int,
			@vcOptionRoot_bad varchar(20),
			@dtExpiry_bad smalldatetime,
			@fStrike_bad float,
			@iIsCall_bad tinyint,
			@vcSymbol_bad varchar(20)
	
	select @iOptionID_bad = optionID 
		from OptionView O join Contract C on O.optionID = C.contractID
		where (optionID != @iOptionID or @iOptionID is null) and C.symbol = @vcSymbol and
		(optionRootID != @iOptionRootID or expiryID != @iExpiryID or abs(strike - @fStrike) > 0.009 or isCall != @iIsCall)

	if @iOptionID_bad is not null
	begin
		select @vcSymbol_bad = symbol from ContractView where contractID = @iOptionID_bad
		select	@vcOptionRoot_bad = ORT.symbol
				from OptionView O join OptionRootView ORT on ORT.optionRootID = O.optionRootID
				where O.optionID = @iOptionID_bad
		select @dtExpiry_bad = expiryPeriod 
				from ExpiryView E join OptionView O on E.expiryID = O.expiryID 
				where O.optionID = @iOptionID_bad
		select 	@fStrike_bad = strike,
				@iIsCall_bad = isCall 
				from OptionView where optionID = @iOptionID_bad

		set @ERR_MSG = 'Can''t add option with Symbol=' + '''' +  @vcSymbol + ''''  +  
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
			' because exists option with Symbol=' + '''' +  @vcSymbol_bad + ''''  +  
		CASE WHEN left(@vcSymbol,len(@vcSymbol)-2) != left(@vcSymbol_bad,len(@vcSymbol_bad)-2) THEN
			', Option Root=' + '''' + left(@vcSymbol_bad,len(@vcSymbol_bad)-2) + ''''
		ELSE '' END + 
		CASE WHEN @fStrike!=@fStrike_bad THEN
			', Strike=' + CAST(ROUND(@fStrike_bad, 2) as varchar(10)) 
		ELSE '' END + 
		CASE WHEN cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) != cast(upper(Left(DateName(m,@dtExpiry_bad),3)) as varchar) + cast(year(@dtExpiry_bad) as varchar) THEN
			', Expiry=' + '''' + cast(upper(Left(DateName(m,@dtExpiry),3)) as varchar) + cast(year(@dtExpiry) as varchar) + ''''
		ELSE '' END + 
		CASE WHEN @iIsCall!=@iIsCall_bad THEN
			'''' + ', Call\Put' + '''' + '=' + CASE @iIsCall_bad WHEN 1 THEN 'Call' ELSE 'Put' END
		ELSE '' END +
		', Underlying Symbol=' + '''' + @vcUnderlyingSymbol + ''''

		Return(-1)
	end
	else
		begin
			set @ERR_MSG = null
			set @iOptionID_bad = null
		end

	--===================================================================
	-- update option symbol, if option with same attributes exists
	--===================================================================
	select @iOptionID_bad = optionID 
		from OptionView O join ContractView C on O.optionID = C.contractID
		where (optionID != @iOptionID) and C.symbol != @vcSymbol and
		(optionRootID = @iOptionRootID and expiryID = @iExpiryID and abs(strike - @fStrike) < 0.009 and isCall = @iIsCall)
	if @iOptionID_bad is not null
	begin
		update contract set symbol = @vcSymbol where contractID = @iOptionID_bad
		set @ERR_MSG = '*'
	end

/*	select 	@iOptionID_bad as '@iOptionID_bad',@iOptionID '@iOptionID', 
			@vcSymbol '@vcSymbol',@iOptionRootID '@iOptionRootID',@dtExpiry '@dtExpiry',
			@iExpiryID '@iExpiryID',@fStrike '@fStrike',@iIsCall '@iIsCall',
			@vcUnderlyingSymbol '@vcUnderlyingSymbol'

	select 	@vcOptionRoot_bad '@vcOptionRoot_bad',@dtExpiry_bad '@dtExpiry_bad',
			@fStrike_bad '@fStrike_bad',@iIsCall_bad '@iIsCall_bad', @vcSymbol_bad '@vcSymbol_bad' */

Return(@@error)

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
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Import]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


CREATE	Procedure dbo.usp_Option_Import
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
	Description: this procedure save options in ETSBO.EOD procedure
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare	@OptionID int, 
			@cn varchar(255),
			@iExpiryID int,
			@iLastExpiryID int,
			@ERR_MSG varchar(8000)

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
	-- check the underlying contract
	--===================================================================			
	declare @iUnderlyingFirstContractID int
		set @iUnderlyingFirstContractID = @iUnderlyingContractID
	--select underlying ID
	select @iUnderlyingContractID = contractID  
		from ContractView 
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
			where contractID = @iUnderlyingContractID and Contract.contractTypeID < 3

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
	if (@OptionID is Null and @iOptionRootID is not null) 
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
			@vcUnderlyingSymbol = @vcUnderlyingSymbol,@ERR_MSG = @ERR_MSG output

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

	--===================================================================			
	-- check the type of underlying contract
	--===================================================================			
	if exists (select contractID from Contract(nolock) where contractID = @iUnderlyingContractID and contractTypeID > 2)
	 begin
	   RAISERROR ('Error. Can''t insert values to [Option]. The Underlying Contract is not stock or index!', 16, 1)
	   RETURN (-1)
	 end 

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
	
			insert into ContractPrice (contractID,exchangeID, priceBid, priceAsk, volume,openInterest)
				 values (@OptionID,@iExchangeID, @fPriceBid, @fPriceAsk, @iVolume,@iOpenInterest)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			if not exists(select * from ContractPrice where exchangeID is null and contractID = @OptionID)
				begin
					insert into ContractPrice(contractID,exchangeID)
						values(@OptionID,null)
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

		delete from TradeLog
		where contractID = @OptionID

		if @iOptionRootID is not null
		begin
		  update OptionRoot
				set
					lotSize = isnull(@iLotSize,lotSize)
				where optionRootID = @iOptionRootID
		end

		  update ContractPrice
				set	priceAsk = @fPriceAsk,
					priceBid = @fPriceBid,
					volume = @iVolume,
					openInterest=@iOpenInterest
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

CREATE            proc dbo.usp_Option_Save
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
   if len(@vcSymbol) < 3  and @iOptionID is null
	   begin
		 RAISERROR ('Error. Option symbol can''t be less three symbols!', 16, 1)
		 RETURN (-1)
	   end	 

   if exists (select optionRootID 
				   from OptionRootView
				   where symbol = left(@vcSymbol,len(@vcSymbol)-2)and 
					underlyingID<>@iUnderlyingContractID)
		begin
		  RAISERROR ('Error. Can''t insert or update Option. Current Root assigned to different Underlying', 16, 1)
		  RETURN (-1)
		end  

	--===================================================================			
	-- check if option with same symbol exists
	--===================================================================		
	declare	@OptionID int, 
			@iOptionRootID int,
			@ContractName varchar(255),
			@iExpiryID int

	select @iOptionRootID = optionRootID from OptionRootView where symbol = left(@vcSymbol,len(@vcSymbol)-2)  and 
		underlyingID=@iUnderlyingContractID	

	declare @ERR_MSG varchar(8000),
			@vcUnderlyingSymbol varchar(10)

	select @vcUnderlyingSymbol = symbol 
		from ContractView 
		where contractID = @iUnderlyingContractID and contractTypeID < 2

	set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

	execute usp_Option_Check @iOptionID = @OptionID, 
					@vcSymbol = @vcSymbol,	
					@iOptionRootID = @iOptionRootID,
					@dtExpiry = @dtExpiry,
					@iExpiryID = @iExpiryID,
					@fStrike = @fStrike,
					@iIsCall = @iIsCall,
					@vcUnderlyingSymbol = @vcUnderlyingSymbol,
					@ERR_MSG = @ERR_MSG output

	if isnull(@ERR_MSG,'')!= ''
		begin
			if @ERR_MSG != '*' Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end

/*  
	if exists (select * from ContractView where symbol = @vcSymbol and ContractTypeID = 3 and (@iOptionID <> contractID or @iOptionID is null) and not @vcSymbol is null)
	   begin
		 Raiserror ('Error. Option with this ticker already exists!', 16, 1)
		 Return (-1)
	   end

   if @iOptionRootID is not null and exists (select optionID 
				   from OptionView
				   where optionRootID = @iOptionRootID and 
					isCall = @iIsCall and 
					strike = @fStrike and 
					expiryID = @iExpiryID and
					(@iOptionID is Null or optionID <> @iOptionID))
		begin
		  RAISERROR ('Error. Can''t insert or update Option values. There are ambiguous Option in DB!', 16, 1)
		  RETURN (-1)
		end  */

   set @ContractName = @vcSymbol + ' ' + datename(day, @dtExpiry) + ' ' + datename(month, @dtExpiry) + 
					' ' + datename(year, @dtExpiry) + ' ' + CASE @iIsCall when 1 then 'Call' else 'Put' end +
						 ' ' + cast(round(@fStrike, 2) as varchar)


	Begin Transaction

		if  @iOptionRootID is null
			begin
				insert into OptionRoot (underlyingID,symbol,lotSize)
					select @iUnderlyingContractID,left(@vcSymbol,len(@vcSymbol)-2),100
				set @iOptionRootID = @@identity
			end 

	   if (@iOptionID is Null)	
		 begin
			insert into Contract (contractTypeID, symbol, contractName)
				values (3, @vcSymbol, @ContractName)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iOptionID = @@identity			  
			
			insert into [Option] (optionID, optionRootID, isCall, strike, expiryID)
				values (@iOptionID, @iOptionRootID, @iIsCall, @fStrike, @iExpiryID)
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
				set optionRootID = @iOptionRootID,
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
/* RESULTS ANALYTHING */
IF @@ERROR <> 0 AND @@TRANCOUNT > 0  ROLLBACK TRANSACTION
GO
IF @@TRANCOUNT = 0 BEGIN INSERT INTO #tmpErrors (Error) SELECT 1 BEGIN TRANSACTION END
GO
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.41.072')
	begin
		update DataInfo set keyValue = '3.41.072' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.41.072' '3.41.072 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
