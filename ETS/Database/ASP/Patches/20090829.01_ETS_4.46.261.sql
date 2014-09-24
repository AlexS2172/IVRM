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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.46.260' or (keyValue = '4.46.261') ))
BEGIN
	select 'You can''t run this script, because you have not run script for previous version!' '4.46.261 error message:' 
	Select cast(keyValue as varchar(25)) as 'DB version', '4.46.261' as  'Version in script' from DataInfo where dataInfoID = 1
	INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
END
GO

---------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION SECTION*/
---------------------------------------------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[VolaSurfaceGroup]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [VolaSurfaceGroup]
GO
CREATE TABLE [VolaSurfaceGroup] (
	[ID] [int] IDENTITY (1, 1) NOT NULL,
	[BI_ID] [int] NOT NULL,
	[OptionRootID] [int] ,
	[VSD_ID] [int] NOT NULL,
	[ActionDate] [datetime] NOT NULL DEFAULT (getdate()),
	CONSTRAINT [PK_VolaSurfaceGroup] PRIMARY KEY  CLUSTERED 
	(
		[ID]
	) WITH  FILLFACTOR = 60  ON [PRIMARY] ,
) ON [PRIMARY]
GO
----------------------------------------------------------------------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_SubSurface_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_SubSurface_Save]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

create     proc dbo.usp_SubSurface_Save
    @iVolaSurfaceGroupID int = null output,
    @iBaseInstrumentID int = null,
    @iRootID int = null,
    @iVolaSurfaceID int = null, 
    @iContractTypeID int  = null
as
   
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0
   	

	declare @iVSGID int
	set @iVSGID = null


	if (isnull(@iVolaSurfaceGroupID, -1) > 0)
	begin

		update 	VolaSurfaceGroup
		set	vsd_id = @iVolaSurfaceID
		where 	[ID] = @iVolaSurfaceGroupID
		
	end
	else
	begin

		if (isnull(@iBaseInstrumentID, -1) > 0 and isnull(@iRootID, -1) > 0)
		begin
			select	@iVSGID = [ID]
			from	VolaSurfaceGroup
			where 	bi_id = @iBaseInstrumentID and optionRootId = @iRootID

			if (isnull(@iVSGID, -1) > 0)
			begin
				update	VolaSurfaceGroup
				set	vsd_id = @iVolaSurfaceID
				where [ID] = @iVSGID
			end
			else
			begin

				Begin Transaction		

		
				if @error <> 0 begin
					if @@TranCount > 1 Rollback Transaction goto finish
				end
			
				insert into VolaSurfaceGroup (bi_id, OptionRootID, vsd_id)
				values (@iBaseInstrumentID, @iRootID, @iVolaSurfaceID)

				if @@error <> 0 begin 
					set @error = -2 
					if @@TranCount > 1 Rollback Transaction goto finish 
				end

				Commit Transaction
			end
			set @error = 0
		end
				
	end
                
finish:
	return @error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
-------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_SubSurface_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_SubSurface_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

create      proc dbo.usp_SubSurface_Get 
    @iVolaSurfaceGroupID int = null,
    @iBaseInstrumentID int = null,
    @iRootID int = null, 
    @iContractTypeID int  = null,
    @vcSymbolName varchar(20) = null
as
   
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0
   	
	if ( isnull(@iVolaSurfaceGroupID, -1) > 0 )
	begin
		select 
			vsg.[id] as iGroupID,
			vsg.bi_id as iBaseAsset,
			vsg.OptionRootID as iRootId,
			vsd.SurfaceName as vcSurfaceName,
			vsg.vsd_id as iSurfaceID
		from	VolaSurfaceGroup vsg
		inner join VolaSurfaceData vsd on vsd.VolaSurfaceDataID = vsg.vsd_id
		where 	vsg.[id] = @iVolaSurfaceGroupID
	end
	else
	begin
		if ( isnull(@iBaseInstrumentID, -1) > 0 and isnull(@iRootID, -1) > 0 )
		begin
			select 
				vsg.[id] as iGroupID,
				vsg.bi_id as iBaseAsset,
				vsg.OptionRootID as iRootId,
				vsd.SurfaceName as vcSurfaceName,
				vsg.vsd_id as iSurfaceID
			from	VolaSurfaceGroup vsg
			inner join VolaSurfaceData vsd on vsd.VolaSurfaceDataID = vsg.vsd_id
			where 	vsg.[bi_id] = @iBaseInstrumentID and vsg.[OptionRootID] = @iRootID
		end
		else
		begin
			if ( isnull(@iBaseInstrumentID, -1) > 0 )
			begin
				select 
					vsg.[id] as iGroupID,
					vsg.bi_id as iBaseAsset,
					vsg.OptionRootID as iRootId,
					vsd.SurfaceName as vcSurfaceName,
					vsg.vsd_id as iSurfaceID
				from	VolaSurfaceGroup vsg
				inner join VolaSurfaceData vsd on vsd.VolaSurfaceDataID = vsg.vsd_id
				where 	vsg.[bi_id] = @iBaseInstrumentID
			end
			else
			begin
				if (isnull(@vcSymbolName, '*NO*') = '*NO*')
				begin
					select 
						vsg.[id] as iGroupID,
						vsg.bi_id as iBaseAsset,
						vsg.OptionRootID as iRootId,
						vsd.SurfaceName as vcSurfaceName,
						vsg.vsd_id as iSurfaceID
					from	VolaSurfaceGroup vsg
					inner join VolaSurfaceData vsd on vsd.VolaSurfaceDataID = vsg.vsd_id
				end
				else
				begin
					declare @iContractID int
					set @iContractID = NULL

					select @iContractID = ContractID
					from ContractView
					where Symbol = rtrim(ltrim(@vcSymbolName)) and ContractTypeID in (1, 2)
					
					if ( isnull(@iContractID, -1) > 0)
					begin
						select 
							vsg.[id] as iGroupID,
							vsg.bi_id as iBaseAsset,
							vsg.OptionRootID as iRootId,
							vsd.SurfaceName as vcSurfaceName,
							vsg.vsd_id as iSurfaceID,
							orv.Symbol as vcRootSymbol	
						from	VolaSurfaceGroup vsg
						inner join VolaSurfaceData vsd on vsd.VolaSurfaceDataID = vsg.vsd_id
						inner join OptionRootView orv on orv.OptionRootID = vsg.OptionRootID and orv.UnderlyingID = vsg.bi_id
						where 	vsg.[bi_id] = @iContractID	
					end
				end
			end
		end
	end
                
finish:
	return @error


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


-------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_SubSurface_Del]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_SubSurface_Del]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

create     proc dbo.usp_SubSurface_Del
    @iVolaSurfaceGroupID int = null,
    @iBaseInstrumentID int = null,
    @iRootID int = null, 
    @iContractTypeID int  = null
as
   
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0
   	
	if ( isnull(@iVolaSurfaceGroupID, -1) > 0 )
	begin
		delete from VolaSurfaceGroup
		where [ID] = @iVolaSurfaceGroupID
	end
	else
	begin
		if ( isnull(@iBaseInstrumentID, -1) > 0 and isnull(@iRootID, -1) > 0 )
		begin

			delete	from	VolaSurfaceGroup
			where 	[bi_id] = @iBaseInstrumentID and [OptionRootID] = @iRootID
		end
		else
		begin
			if ( isnull(@iBaseInstrumentID, -1) > 0 )
			begin
				delete	from	VolaSurfaceGroup
				where 	[bi_id] = @iBaseInstrumentID
			end
		end
	end
                
finish:
	return @error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_SubSurfaceList_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_SubSurfaceList_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

create     proc dbo.usp_SubSurfaceList_Get
    @iBaseInstrumentID int = null
as
   
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0
   	
	
	select 
		vsd.ContractID as iBaseAsset,
		vsd.SurfaceName as vcSurfaceName,
		vsd.[VolaSurfaceDataID] as iSurfaceID
	from	VolaSurfaceData vsd
	where 	vsd.IsDefaultSurface = 0 and @iBaseInstrumentID = vsd.ContractID
	
	                
finish:
	return @error

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

ALTER   procedure dbo.usp_Option_Save
		@iOptionID int,
		@vcSymbol varchar(20) = Null,
		@iExchangeID int, 
		@iUnderlyingContractID int,
		@iIsCall tinyint = Null,
		@fStrike float = Null,
		@dtExpiry datetime = Null,
		@dtExpiryOV datetime = null,
		@dtTradeClose datetime = null,
		@tiIsManualyEntered tinyint /* now this flag if not 0, means that this option is flex */
as
-- changed by VK - flex option support added
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	   if exists (select contractID from ContractView where contractID = @iUnderlyingContractID and contractTypeID > 2)
	   begin
		 RAISERROR ('Error. Can''t insert or update Option values. The Underlying Contract is not stock or index!', 16, 1)
		 RETURN (-1)
	   end	 
 	  
	   if len(@vcSymbol) < 3 and @iOptionID is null
	   begin
		 RAISERROR ('Error. Option symbol can''t be less three symbols!', 16, 1)
		 RETURN (-1)
	   end	 

	   if exists (select optionRootID from OptionRootView
		   where symbol = left(@vcSymbol,len(@vcSymbol)-2) and underlyingID != @iUnderlyingContractID)
		begin
		  RAISERROR ('Error. Can''t insert or update Option. Current Root assigned to different Underlying', 16, 1)
		  RETURN (-1)
		end  

	--===================================================================			
	-- check if option with same symbol exists
	--===================================================================		
	declare	@iOptionRootID int,
		@ContractName varchar(255) --,
		--@iExpiryID int,
		--@iExpCalID INT

	select @iOptionRootID = optionRootID from OptionRootView 
		where symbol = left(@vcSymbol,len(@vcSymbol)-2) and 
			underlyingID = @iUnderlyingContractID

	declare @ERR_MSG varchar(8000),
		@vcUnderlyingSymbol varchar(10)

	select @vcUnderlyingSymbol = symbol from ContractView 
		where contractID = @iUnderlyingContractID and contractTypeID <= 2

	--set @iExpiryID = cast (@dtExpiry As int) --(year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
	

	execute usp_Option_Check @iOptionID = @iOptionID, 
					@vcSymbol = @vcSymbol,	
					@iOptionRootID = @iOptionRootID,
					@dtExpiry = @dtExpiry,
					--@iExpiryID = @iExpiryID,
					@fStrike = @fStrike,
					@iIsCall = @iIsCall,
					@vcUnderlyingSymbol = @vcUnderlyingSymbol,
					@ERR_MSG = @ERR_MSG output

		/*	select @vcSymbol '@vcSymbol',@iOptionRootID '@iOptionRootID',@dtExpiry '@dtExpiry',@iExpiryID '@iExpiryID',
				@fStrike '@fStrike',@iIsCall '@iIsCall',@vcUnderlyingSymbol '@vcUnderlyingSymbol',@ERR_MSG '@ERR_MSG'
			select @iOptionID = 0 */

	if isnull(@ERR_MSG,'')!= ''
		begin
			if @ERR_MSG != '*' Raiserror(@ERR_MSG,16,1)
			Return(-1)
		end
	IF @tiIsManualyEntered = 0 
   		set @ContractName = @vcUnderlyingSymbol + ' ' + datename(day, @dtExpiry) + ' ' + datename(month, @dtExpiry) + 
					' ' + datename(year, @dtExpiry) + ' ' + CASE @iIsCall when 1 then 'Call' else 'Put' end +
					' ' + cast(round(@fStrike, 2) as varchar)
	ELSE
		set @ContractName = @vcUnderlyingSymbol + ' ' + datename(day, @dtExpiry) + ' ' + datename(month, @dtExpiry) + 
					' ' + datename(year, @dtExpiry) + ' ' + CASE @iIsCall when 1 then 'Call' else 'Put' end +
					' ' + cast(round(@fStrike, 2) as varchar) + ' (OTC)'

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
			
			insert into [Option] (optionID, optionRootID, isCall, strike, expiryDate, ExpiryOV, TradingClose)
				values (@iOptionID, @iOptionRootID, @iIsCall, @fStrike, @dtExpiry, @dtExpiryOV, @dtTradeClose)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

			/* Change ExpiryOV for all Options with the same Root */
			update [Option]
				set 	ExpiryOV = isnull(@dtExpiryOV, ExpiryOV),
					TradingClose = isnull(@dtTradeClose, TradingClose)
				where optionRootID = @iOptionRootID and expiryDate = @dtExpiry
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end

			--========================================================================
			-- make record with null exchange for this stock
			--========================================================================
			insert into ContractPrice(contractID,exchangeID) values(@iOptionID,null)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		 end
	   else
		 begin
			update Contract set symbol = isNull(@vcSymbol,symbol), 
				contractName = isNull(@ContractName,contractName)
			where contractID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			declare @iOldOptionID as int
			select @iOldOptionID = optionRootID from [Option] 
				where optionID = @iOptionID

			update [Option]
				set optionRootID = @iOptionRootID,
					isCall = isNull(@iIsCall, isCall),
					strike = isNull(@fStrike, strike),
					expiryDate = isNull(@dtExpiry, expiryDate),
					ExpiryOV = isnull(@dtExpiryOV, ExpiryOV),
					TradingClose = isnull(@dtTradeClose, TradingClose)
					--expiryID = isNull(@iExpiryID, expiryID)
				where optionID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			/* Change ExpiryOV for all Options with the same Root */
			update [Option]
				set ExpiryOV = isnull(@dtExpiryOV, ExpiryOV), 
				TradingClose = isnull(@dtTradeClose, TradingClose)
			where optionRootID = @iOptionRootID and expiryDate = @dtExpiry
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction goto finish end

			if @iOldOptionID is not null and @iOptionRootID is not null
			begin
			  update Trade set OptionRootId = @iOptionRootID where 	OptionRootId = @iOldOptionID
	  		  if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
		 end		

		if @tiIsManualyEntered in (0,1) 
		begin
			update [Option] set isManualyEntered = @tiIsManualyEntered where optionID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end 



	Commit Transaction

    set @error = 0
finish:
if (@error != 0) return (@error) else return @iOptionID



GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

ALTER     PROC dbo.usp_BoFutureOption_Save
		@iFutureOptionID int out,
		@iFutureID int, -- should be specified for new future option (ignored for update)
		@vcFutureOptionSymbol varchar(20) = null,
		@vcFutureOptionName varchar(255) = null, -- calculated if not specified (for new option)
		@vcExportSymbol varchar(20) = null,
		@tiIsCall tinyint = null,
		@fStrike float = null,
		--@iExpiryID int = null, -- calculated by @dtExpiryDate if not specified
		@dtExpiryDate datetime = null, -- ignored if @iExpiryID is specified
		@dtExpiryOV datetime = null,
		@dtTradeClose datetime = null,
		@tiIsRaiseError tinyint = 1 
AS
	-- create/update future

	set nocount on
	
	declare @error int set @error = 0

	if @iFutureOptionID is null -- new option
	begin
		-- check new future option future ID
		if @iFutureID is null
		begin
			if @tiIsRaiseError = 1 raiserror('Future for new future option is not specified!', 16, 1)
			set @error = -1
			goto finish
		end

		declare @vcFutureSymbol varchar(20)
		declare @iFutureContractTypeID int
		--M declare @iFutureExpCalendarID int

		-- find future name and contract type
		select @iFutureContractTypeID = contractTypeID,	@vcFutureSymbol = symbol --, @iFutureExpCalendarID = expCalendarID
		from ContractView where contractID = @iFutureID		

		-- check future existence
		if @iFutureContractTypeID is null
		begin
			if @tiIsRaiseError = 1 raiserror('Underlying contract for new future option is invalid!', 16, 1)
			set @error = -2
			goto finish
		end

		-- check future contract type
		if @iFutureContractTypeID <> 4
		begin
			if @tiIsRaiseError = 1 raiserror('Underlying contract for new future option is not a future!', 16, 1)
			set @error = -3
			goto finish
		end

		-- check future option symbol
		set @vcFutureOptionSymbol = isnull(@vcFutureOptionSymbol, '')
		if len(ltrim(@vcFutureOptionSymbol)) = 0
		begin
			if @tiIsRaiseError = 1 raiserror('Symbol for new future option is not specified!', 16, 1)
			set @error = -4
			goto finish
		end

		-- check strike
		if isnull(@fStrike, 0) <= 0
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future option strike specified!', 16, 1)
			set @error = -5
			goto finish
		end

		-- round strike upto 3 decimals
		set @fStrike = round(@fStrike, 3)

		-- calculate expiry ID by expiry date if expiry ID is not specified
		--if @iExpiryID is null and @dtExpiryDate is not null
		--	set @iExpiryID = cast (@dtExpiryDate as int ) --(year(@dtExpiryDate) - 2000) * 12 + month(@dtExpiryDate)

		-- check expiry ID/Date
		-- select @dtExpiryDate = expiryDate
		-- from ExpiryInCalendarView
		-- where expiryID = @iExpiryID and expCalendarID = @iFutureExpCalendarID
		--if @iExpiryID is not null  
		--	set @dtExpiryDate = cast (@iExpiryID as DateTime)
		
		if @dtExpiryDate is null
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future option expiry specified!', 16, 1)
			set @error = -6
			goto finish
		end

		-- correct call/put
		set @tiIsCall = isnull(@tiIsCall, 0)
		if @tiIsCall <> 0
			set @tiIsCall = 1

		-- check option duplacates by symbol
		if exists(select 1 from ContractView where contractTypeID = 5 and symbol = @vcFutureOptionSymbol)
		begin
			if @tiIsRaiseError = 1 raiserror('Future option with symbol ''%s'' is already exists!', 16, 1, @vcFutureOptionSymbol)
			set @error = -7
			goto finish
		end

		-- check option duplacates by parameters
		if exists (select 1 from FutureOption
				where futureID = @iFutureID
					and datediff(d, expiryDate, @dtExpiryDate) = 0 and isCall = @tiIsCall
					and abs(strike - @fStrike) < 0.001) -- check equality upto 3 decimals
		begin
			if @tiIsRaiseError = 1 raiserror('Future option with the same parameters is already exists for specified future!', 16, 1)
			set @error = -8
			goto finish
		end

		-- make default future option contract name if not specified
		if @vcFutureOptionName is null
			set @vcFutureOptionName = @vcFutureSymbol +
						' ' + datename(month, @dtExpiryDate) + 
						' ' + datename(year, @dtExpiryDate) +
						' ' + case @tiIsCall when 1 then 'Call' else 'Put' end +
						' ' + cast(floor(@fStrike) as varchar)

		begin tran

		-- save new contract data
		insert into Contract
			(contractTypeID, symbol, contractName/*, expCalendarID*/, undPriceProfileID, optPriceProfileID)
		values	(5, @vcFutureOptionSymbol, @vcFutureOptionName/*, null*/, null, null)
		if @@error <> 0 begin set @error = -9 if @@trancount = 1 rollback tran goto finish end

		set @iFutureOptionID = @@identity

		-- save new future option data
		insert into FutureOption
			(futureOptionID, futureID, isCall, expiryDate, ExpiryOV, TradingClose, strike)
		values	(@iFutureOptionID, @iFutureID, @tiIsCall, @dtExpiryDate, @dtExpiryOV, @dtTradeClose, @fStrike)
		if @@error <> 0 begin set @error = -10 if @@trancount = 1 rollback tran goto finish end

		/* Change ExpiryOV for all Options with the same Root */
		update [FutureOption]
			set ExpiryOV = isnull(@dtExpiryOV, ExpiryOV),
			TradingClose = isnull(@dtTradeClose, TradingClose)
		where futureOptionID = @iFutureOptionID and expiryDate = @dtExpiryDate
		if @@error <> 0 begin set @error = -18 if @@trancount = 1 rollback tran goto finish end

		-- save price record for default exchange
		if not exists(select 1 from ContractPrice where exchangeID is null and contractID = @iFutureOptionID)
		begin
			insert into ContractPrice
				(contractID, exchangeID)
			values	(@iFutureOptionID, null)
			if @@error <> 0 begin set @error = -11 if @@trancount = 1 rollback tran goto finish end
		end

		commit tran
	end
	else
	begin
		-- correct future option name (prevent updates to empty string)
		if len(ltrim(@vcFutureOptionSymbol)) = 0
			set @vcFutureOptionSymbol = null

		-- check future option name
		if @vcFutureOptionSymbol is not null
			if exists(select 1 from ContractView where contractTypeID = 5
						and contractID <> @iFutureOptionID and symbol = @vcFutureOptionSymbol)
			begin
				if @tiIsRaiseError = 1 raiserror('Future option with symbol ''%s'' is already exists!', 16, 1, @vcFutureOptionSymbol)
				set @error = -12
				goto finish
			end

		-- correct strike (prevent updates to zero)
		if @fStrike = 0
			set @fStrike = null

		-- check strike
		if @fStrike is not null
		begin
			if @fStrike <= 0
			begin
				if @tiIsRaiseError = 1 raiserror('Invalid future option strike specified!', 16, 1)
				set @error = -13
				goto finish
			end

			-- round strike upto 3 decimals
			set @fStrike = round(@fStrike, 3)
		end

		-- correct call/put
		if @tiIsCall is not null
			if @tiIsCall <> 0
				set @tiIsCall = 1

		-- correct expiry ID (prevent updates to zero)
		--if @iExpiryID = 0
		--	set @iExpiryID = null

		-- calculate expiry ID by expiry date if expiry ID is not specified
		--if @iExpiryID is null and @dtExpiryDate is not null
		--	set @iExpiryID = cast (@dtExpiryDate as int) --(year(@dtExpiryDate) - 2000) * 12 + month(@dtExpiryDate)


		if @tiIsCall is not null or @fStrike is not null or @dtExpiryDate is not null
		begin
			declare @tiIsCallToCheck tinyint
			declare @fStrikeToCheck float
			declare @dtExpiryDateToCheck DateTime
	
			-- prepare parameters to check option duplacates
			select @tiIsCallToCheck = isnull(@tiIsCall, isCall),
				@fStrikeToCheck = isnull(@fStrike, strike),
				@dtExpiryDateToCheck = isnull(@dtExpiryDate, expiryDate)
			from FutureOption
			where futureOptionID = @iFutureOptionID
	
			-- check option duplacates by parameters
			if exists (select 1 from FutureOption
					where futureOptionID <> @iFutureOptionID and futureID = @iFutureID
						and datediff(d, expiryDate, @dtExpiryDateToCheck) = 0 and isCall = @tiIsCallToCheck
						and abs(strike - @fStrikeToCheck) < 0.001) -- check equality upto 3 decimals
			begin
				if @tiIsRaiseError = 1 raiserror('Future option with the same parameters is already exists for specified future!', 16, 1)
				set @error = -15
				goto finish
			end
		end

		begin tran

		-- update contract data
		update Contract
		set symbol = isnull(@vcFutureOptionSymbol, symbol),
			contractName = isnull(@vcFutureOptionName, contractName)
		where contractID = @iFutureOptionID
		if @@error <> 0 begin set @error = -16 if @@trancount = 1 rollback tran goto finish end

		-- save future option data
		update FutureOption
		set isCall = isnull(@tiIsCall, isCall),
			expiryDate = isnull(@dtExpiryDate, expiryDate),
			ExpiryOV = isnull(@dtExpiryOV, ExpiryOV),
			TradingClose = isnull(@dtTradeClose, TradingClose),
			strike = isnull(@fStrike, strike)
		where futureOptionID = @iFutureOptionID
		if @@error <> 0 begin set @error = -17 if @@trancount = 1 rollback tran goto finish end

		/* Change ExpiryOV for all Options with the same Root */
		update [FutureOption]
			set ExpiryOV = isnull(@dtExpiryOV, ExpiryOV),
			TradingClose = isnull(@dtTradeClose, TradingClose)
		where futureOptionID = @iFutureOptionID and expiryDate = @dtExpiryDate
		if @@error <> 0 begin set @error = -18 if @@trancount = 1 rollback tran goto finish end
		commit tran
	end

finish:
if (@error <> 0) return (@error) else return @iFutureOptionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_AspEodUpdateMarketStructure_SynthSubSurfaceIV_Execute]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_AspEodUpdateMarketStructure_SynthSubSurfaceIV_Execute]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

create       PROC [dbo].[usp_AspEodUpdateMarketStructure_SynthSubSurfaceIV_Execute]
		@iWithTran int = null
AS  
 -------------------------------------------------  
 set nocount on  
 -------------------------------------------------  

 create  table #tbl(RowID int identity(1, 1), eodSurfaceID int, SurfaceID int, BaseAsset int, RootID int, surfaceName varchar (250))
 truncate table #tbl

 /*Insert exsisting*/

 insert into #tbl(BaseAsset, RootID, SurfaceName, eodSurfaceID)
 select
	eod_um.ContractID as BaseAsset,
	(orv.OptionRootID) as OptionRootID,
	eod_vsd.SurfaceName as SurfaceName,
	eod_vsg.vsd_id as SurfaceID
 from ETS_ASP_EOD.dbo.VolaSurfaceGroup eod_vsg
 inner join ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.VolaSurfaceDataID = eod_vsg.vsd_id
 inner join EodUnderlyingMap eod_um on eod_um.eodContractID = eod_vsg.bi_id
 inner join EodOptionRoot eod_ort on eod_ort.UnderlyingID = eod_um.contractID and eod_ort.eodOptionRootID = eod_vsg.OptionRootID
 inner join OptionRootView orv on orv.underlyingID = eod_um.ContractID and orv.Symbol = eod_ort.Symbol COLLATE SQL_Latin1_General_CP1_CI_AS
 left join VolaSurfaceGroup vsg on vsg.bi_id = eod_um.ContractID and vsg.OptionRootID = orv.OptionRootID
 where vsg.bi_id is null

 declare
	@vsd_id int,
	@eod_vsd_id int,
	@count int, 
	@iRow int,
	@BaseAsset int,
	@vcName varchar (250),
	@cSurfaceID int,
	@iExistSurface int

 select @count = count(*) from #tbl

 SET @iRow = 1

 WHILE @iRow <= @count
 BEGIN
	SELECT	@vsd_id = SurfaceID,
		@eod_vsd_id = eodSurfaceID, 
		@BaseAsset = BaseAsset, 
		@vcName = SurfaceName
	FROM 	#tbl
	WHERE 	RowID = @iRow
	
	set 	@iExistSurface = null

	select 	@iExistSurface = SurfaceID 
	from 	#tbl
	where 	eodSurfaceID = @eod_vsd_id and isnull(SurfaceID, -1) > 0
	
	if (isnull(@iExistSurface, -1) < 0)
	begin
		select @iExistSurface = VolaSurfaceDataID
		from VolaSUrfaceData
		where SurfaceName = @vcName and ContractID = @BaseAsset and IsDefaultSurface = 0
	end
	
	if (isnull(@iExistSurface, -1) < 0)
	begin
		
		insert into VolaSurfaceData(ContractID, SurfaceName, IsDefaultSurface)
		values (@BaseAsset, @vcName, 0)

		Set @cSurfaceID = @@identity
		update #tbl set SurfaceID = @cSurfaceID where RowID = @iRow
	end
	else
	begin
		update #tbl set SurfaceID = @iExistSurface where RowID = @iRow
	end

	SET @iRow = @iRow + 1
 END 

 insert into VolaSurfaceGroup (bi_id, OptionRootID, vsd_id)
 select	BaseAsset, RootID, SurfaceID
 from #tbl

 drop table #tbl
 -------------------------------------------------------------------------------------------------------
 declare @error int set @error = 0  
  
 declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')  
 if @iWithTran is not null begin tran @vcTranName  
 -------------------------------------------------------------------------------------------------------------------
 /*delete old surfaces*/
 delete from VolaSurfaceGroup 
 where [ID] in 	(
			select vsg.[ID]
			from VolaSurfaceGroup vsg
			inner join EodUnderlyingMap um on vsg.bi_id = um.contractid
			inner join OptionRootView orv on orv.OptionRootID = vsg.OptionRootID and orv.underlyingid = um.contractID
			inner join EodOptionRoot eor on eor.underlyingID = um.contractid and eor.symbol = orv.Symbol COLLATE SQL_Latin1_General_CP1_CI_AS
			left join ETS_ASP_EOD.dbo.VolaSurfaceGroup eod_vsg on eod_vsg.bi_id = um.eodContractID and eod_vsg.OptionRootID = eor.eodOptionRootID
			where isnull(eod_vsg.bi_id, -1) < 0
		)
 --------------------------------------------------------------------------------------------------------------------
 /*Insert new Surfaces*/

 insert into VolaSurfaceData(ContractID, SurfaceName, IsDefaultSurface)
 select 
	vsg.bi_id,
	eod_vsd.SurfaceName as EodSurfaceName,
	0 as IsDefault
 from VolaSurfaceGroup vsg
 inner join EodUnderlyingMap um on vsg.bi_id = um.contractid
 inner join OptionRootView orv on orv.OptionRootID = vsg.OptionRootID and orv.underlyingid = um.contractID
 inner join EodOptionRoot eor on eor.underlyingID = um.contractid and eor.symbol = orv.Symbol COLLATE SQL_Latin1_General_CP1_CI_AS
 inner join ETS_ASP_EOD.dbo.VolaSurfaceGroup eod_vsg on eod_vsg.bi_id = um.eodContractID and eod_vsg.OptionRootID = eor.eodOptionRootID
 inner join ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.VolaSurfaceDataID = eod_vsg.vsd_id
 inner join VolaSurfaceData vsd on vsd.VolaSurfaceDataID = vsg.vsd_id
 where eod_vsd.SurfaceName <> vsd.SurfaceName
	and not exists (select top 1 1 from VolaSurfaceData where SurfaceName = eod_vsd.SurfaceName and ContractID = vsg.bi_id)
 group by vsg.bi_id, eod_vsd.SurfaceName

 /*Update Surfaces Groups*/
 update VolaSurfaceGroup
 set vsd_id = vsd.VolaSurfaceDataID
 from 
	(select 
		eod_vsg.vsd_id as eod_vsdid,
		vsg.vsd_id as cli_vsdid,
		vsg.bi_id as assetid,
		vsg.OptionRootID as optionRootID,
		vsg.[ID] as vsgID,
		eod_vsd.SurfaceName as EodSurfaceName,
		vsd.SurfaceName as ClientSurfaceName
	from VolaSurfaceGroup vsg
	inner join EodUnderlyingMap um on vsg.bi_id = um.contractid
	inner join OptionRootView orv on orv.OptionRootID = vsg.OptionRootID and orv.underlyingid = um.contractID
	inner join EodOptionRoot eor on eor.underlyingID = um.contractid and eor.symbol = orv.Symbol COLLATE SQL_Latin1_General_CP1_CI_AS
	inner join ETS_ASP_EOD.dbo.VolaSurfaceGroup eod_vsg on eod_vsg.bi_id = um.eodContractID and eod_vsg.OptionRootID = eor.eodOptionRootID
	inner join ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.VolaSurfaceDataID = eod_vsg.vsd_id
	inner join VolaSurfaceData vsd on vsd.VolaSurfaceDataID = vsg.vsd_id
	where eod_vsd.SurfaceName <> vsd.SurfaceName) NU
 inner join VolaSurfaceData vsd on vsd.SurfaceName = NU.EodSurfaceName and vsd.ContractID = NU.assetid and vsd.IsDefaultSurface = 0
 where NU.vsgID = [ID] 

 -------------------------------------------------------------------------------------------------------
 -- Clear Synthetic Surfaces ---------------------------------------------------------------------------
 /*
	Delete points and surfaces without Root, BaseAsset, RecordIn VolaSurfaceGroup
 */
--------------------------------------------------------------------------------------------------------	
 -- delete wrong Surafces without BaseAsset --
 delete from VolaSurfaceGroup
 where BI_ID not in (select ContractID from ContractView where ContractTypeID in (1, 2))
 -------------------------------------------------------------------------------------------------------
 -- delete wrong Surfaces without RootID
 delete from VolaSurfaceGroup
 where OptionRootID not in (	select 	OptionRootID 
				from 	OptionRootView 
				where 	UnderlyingID = BI_ID)

 -------------------------------------------------------------------------------------------------------
 -- delete points without surface in volasurfacegroup --
 delete from CustomStrikeSkewPoint 
 where customStrikeSkewPointID in 
 (
	select 	customStrikeSkewPointID 
	from 	CustomStrikeSkewPoint  cssp
	inner 	join VolaSurfaceData vsd on 
						cssp.VolaSurfaceDataID = vsd.VolaSurfaceDataID and 
						vsd.IsDefaultSurface = 0 and 
						vsd.VolaSurfaceDataID not in (select distinct vsd_id from VolaSurfaceGroup)
 )
 --------------------------------------------------------------------------------------------------------
 -- delete Custom Surface without Group --
 delete from VolaSurfaceData
 where VolaSurfaceDataID not in ( select vsd_id from VolaSurfaceGroup )
				and IsDefaultSurface = 0

 -------------------------------------------------------------------------------------
  
 if @@error != 0  
 begin  
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating Synthetic IV surface: Fail to clear wrong IV surfaces', @iErrorCode = @@error  
  set @error = -1  
  if @iWithTran is not null rollback tran @vcTranName  
  goto finish  
 end  
 
 -------------------------------------------------------------------------------------
 ------------- Insert New Points -----------------------------------------------------
 insert into CustomStrikeSkewPoint (volaSurfaceDataID, type, strike, volatility, isBasePoint, expiryDate, expiryOV)
 select 	vsg.vsd_id,  
		eod_cssp.type,
		eod_cssp.strike,
		eod_cssp.volatility,
		eod_cssp.isBasePoint,
		eod_cssp.expiryDate,
		eod_cssp.expiryOV
 from ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp  
 inner join 	(
			select max(OptionRootID) as RootID, bi_id, vsd_id
			from ETS_ASP_EOD.dbo.VolaSurfaceGroup
			group by bi_id, vsd_id
		)SRM on SRM.vsd_id = eod_cssp.VolaSurfaceDataID
 inner join EodUnderlyingMap eod_um on eod_um.eodContractID = SRM.bi_id
 inner join EodOptionRoot eor on eor.underlyingID = eod_um.contractid and eor.eodOptionRootID = SRM.RootID
 inner join OptionRootView orv on orv.underlyingID = eod_um.ContractID and orv.Symbol = eor.Symbol COLLATE SQL_Latin1_General_CP1_CI_AS
 inner join VolaSurfaceGroup vsg on vsg.bi_id = eod_um.ContractID and vsg.OptionRootID = orv.OptionRootID
 left join CustomStrikeSkewPoint cssp on 
	cssp.volaSurfaceDataID = vsg.vsd_id
	and abs(cssp.strike - eod_cssp.strike) < 0.0005
	and cssp.type = eod_cssp.type
	and abs(datediff( d, cssp.expiryDate, eod_cssp.expiryDate )) < 2
	and cssp.actionID < 3
 where cssp.CustomStrikeSkewPointID is null
	and eod_cssp.actionID < 3
 OPTION (MAXDOP 1)
  
  
 if @@error != 0  
 begin  
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating Synthetic IV surface: Fail to add new IV points', @iErrorCode = @@error  
  set @error = -1  
  if @iWithTran is not null rollback tran @vcTranName  
  goto finish  
 end  
  
 --------------------------------------------------------------------------------------
 ------ update exists points ----------------------------------------------------------
 update CustomStrikeSkewPoint
 set  type = eod_cssp.type,
      strike = eod_cssp.strike,
      volatility = case when isnull(vsd.IsManualVol, 0) = 1 then cssp.Volatility else eod_cssp.volatility end,
      isBasePoint = eod_cssp.isBasePoint,
      expiryDate = eod_cssp.expiryDate,
      expiryOV = eod_cssp.expiryOV,
      actionDate = GetDate()
 from ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp  
 inner join 	(
			select max(OptionRootID) as RootID, bi_id, vsd_id
			from ETS_ASP_EOD.dbo.VolaSurfaceGroup
			group by bi_id, vsd_id
		)SRM on SRM.vsd_id = eod_cssp.VolaSurfaceDataID
 inner join EodUnderlyingMap eod_um on eod_um.eodContractID = SRM.bi_id
 inner join EodOptionRoot eor on eor.underlyingID = eod_um.contractid and eor.eodOptionRootID = SRM.RootID
 inner join OptionRootView orv on orv.underlyingID = eod_um.ContractID and orv.Symbol = eor.Symbol COLLATE SQL_Latin1_General_CP1_CI_AS
 inner join VolaSurfaceGroup vsg on vsg.bi_id = eod_um.ContractID and vsg.OptionRootID = orv.OptionRootID
 inner join VolaSurfaceData vsd on vsd.VolaSurfaceDataID = vsg.vsd_id
 inner join CustomStrikeSkewPoint cssp on 
	cssp.volaSurfaceDataID = vsg.vsd_id
	and abs(cssp.strike - eod_cssp.strike) < 0.0005
	and cssp.type = eod_cssp.type
	and abs(datediff( d, cssp.expiryDate, eod_cssp.expiryDate )) < 2
	and cssp.actionID < 3
 where eod_cssp.actionID < 3
	and eod_cssp.volatility > 0.0

 
 if @@error != 0  
 begin  
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating default IV surface: Fail to update IV points', @iErrorCode = @@error  
  set @error = -1  
  if @iWithTran is not null rollback tran @vcTranName  
  goto finish  
 end  
 
 -------------------------------------------------------------------------------------------
 -- delete old points( Note: the point is old if ActionDate not is GetDate()) --------------
 delete from CustomStrikeSkewPoint
 where CustomStrikeSkewPointID in (
 select cssp.CustomStrikeSkewPointID
 from CustomStrikeSkewPoint cssp
 inner join VolaSurfaceGroup vsg on vsg.vsd_id = cssp.VolaSurfaceDataID
 inner join EodUnderlyingMap eod_um on eod_um.ContractID = vsg.bi_id
 inner join OptionRootView orv on orv.underlyingID = eod_um.ContractID and orv.OptionRootID = vsg.OptionRootID
 inner join EodOptionRoot eor on eor.underlyingID = eod_um.contractid and orv.Symbol = eor.Symbol COLLATE SQL_Latin1_General_CP1_CI_AS
 inner join ETS_ASP_EOD.dbo.VolaSurfaceGroup eod_vsg on eod_vsg.bi_id = eod_um.eodContractID and eod_vsg.OptionRootID = eor.eodOptionRootID
 left join ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp on eod_cssp.volaSurfaceDataID = eod_vsg.vsd_id and
								datediff(d, eod_cssp.ExpiryDate, cssp.expiryDate) = 0 and
								abs(eod_cssp.Strike - cssp.strike) < 0.0005
 where isnull(eod_cssp.CustomStrikeSkewPointID, -1) < 0)

 if @@error != 0  
 begin  
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Delete old points IV surface: Fail to delete IV points', @iErrorCode = @@error  
  set @error = -1  
  if @iWithTran is not null rollback tran @vcTranName  
  goto finish  
 end 
 ------------------------------------------------------------------------------------------- 
 
 if @iWithTran is not null commit tran @vcTranName  
   
finish:  
 return @error  
  

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER       PROC [dbo].[usp_AspEodUpdateMarketStructure_DefaultIVs_Execute]
		@iWithTran int = null
AS  
 -------------------------------------------------  
 set nocount on  
 -------------------------------------------------  
 declare @error int set @error = 0  
  
 declare @vcTranName varchar(32) if @iWithTran is not null set @vcTranName = replace(cast(newid() as varchar(36)), '-', '')  
 if @iWithTran is not null begin tran @vcTranName  
  
 -------------------------------------------------  
 -- default IVs  
 insert into VolaSurfaceData (contractID, isDefaultSurface, baseUnderlinePrice)  
 select c.contractID,
	1,
	isnull(cp.priceClose, 0)  
 from Contract c  
 inner join ContractPrice cp on cp.contractID = c.contractID  
 left join VolaSurfaceData vsd on vsd.contractID = c.contractID and vsd.isDefaultSurface = 1 and vsd.actionID < 3  
 where c.actionID < 3
	and c.contractTypeID in (1,2,6)
	and vsd.contractID is null
  
 if @@error != 0  
 begin  
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating default IV surface: Fail to create missed IV surfaces', @iErrorCode = @@error  
  set @error = -1  
  if @iWithTran is not null rollback tran @vcTranName  
  goto finish  
 end  
  

 insert into CustomStrikeSkewPoint (volaSurfaceDataID, type, strike, volatility, isBasePoint, expiryDate, expiryOV)  
 select vsd.volaSurfaceDataID,  
	eod_cssp.type,
	eod_cssp.strike,
	eod_cssp.volatility,
	eod_cssp.isBasePoint,
	eod_cssp.expiryDate,
	eod_cssp.expiryOV
  
 from ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp  
  inner join ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.volaSurfaceDataID = eod_cssp.volaSurfaceDataID  
  inner join EodUnderlyingMap eod_um on eod_um.eodContractID = eod_vsd.contractID  
  inner join VolaSurfaceData vsd on vsd.contractID = eod_um.contractID  
  left join CustomStrikeSkewPoint cssp on cssp.volaSurfaceDataID = vsd.volaSurfaceDataID
	and abs(cssp.strike - eod_cssp.strike) < 0.009
	and cssp.type = eod_cssp.type
	and abs(datediff( d, cssp.expiryDate, eod_cssp.expiryDate )) < 2
	and cssp.actionID < 3
 where cssp.CustomStrikeSkewPointID is null
	and eod_cssp.actionID < 3
	and vsd.isDefaultSurface = 1
	and eod_vsd.isDefaultSurface = 1
OPTION (MAXDOP 1)
  
 if @@error != 0  
 begin  
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating default IV surface: Fail to add new IV points', @iErrorCode = @@error  
  set @error = -1  
  if @iWithTran is not null rollback tran @vcTranName  
  goto finish  
 end  
  
 update CustomStrikeSkewPoint
 set type = eod_cssp.type,
     strike = eod_cssp.strike,
     volatility = case when isnull(vsd.IsManualVol, 0) = 1 then cssp.Volatility else eod_cssp.volatility end,
     --volatility = eod_cssp.volatility,
     isBasePoint = eod_cssp.isBasePoint,
     expiryDate = eod_cssp.expiryDate,
     expiryOV = eod_cssp.expiryOV
 from ETS_ASP_EOD.dbo.CustomStrikeSkewPoint eod_cssp  
  inner join ETS_ASP_EOD.dbo.VolaSurfaceData eod_vsd on eod_vsd.volaSurfaceDataID = eod_cssp.volaSurfaceDataID  
  inner join EodUnderlyingMap eod_um on eod_um.eodContractID = eod_vsd.contractID  
  inner join VolaSurfaceData vsd on vsd.contractID = eod_um.contractID  
  inner join CustomStrikeSkewPoint cssp on cssp.volaSurfaceDataID = vsd.volaSurfaceDataID  
	and abs(cssp.strike - eod_cssp.strike) < 0.009  
	and cssp.type = eod_cssp.type  
	and abs(datediff( d, cssp.expiryDate, eod_cssp.expiryDate )) < 2   
	and cssp.actionID < 3  
 where eod_cssp.actionID < 3
	and eod_cssp.volatility > -100.0 
	and vsd.isDefaultSurface = 1
	and eod_vsd.isDefaultSurface = 1 --IV's mark 'not calculated'  
	--and ((vsd.IsManualVol is NULL OR vsd.IsManualVol != 1) or (cssp.ExpiryOV is NULL))

  
 if @@error != 0  
 begin  
  exec usp_AspEodMsUpdateLog_Add @vcEvent = 'Updating default IV surface: Fail to update IV points', @iErrorCode = @@error  
  set @error = -1  
  if @iWithTran is not null rollback tran @vcTranName  
  goto finish  
 end  
  
 create table #CustStrikeskewpoint (customstrikeskewpoint int)  
 insert into #CustStrikeskewpoint (customstrikeskewpoint) --values   
 select csp.customStrikeSkewPointID from contract c   
  inner join VolaSurfaceData vsd on vsd.contractID= c.ContractID  and vsd.isDefaultSurface = 1
  inner join customstrikeskewpoint csp on vsd.volaSurfaceDataID = csp.volaSurfaceDataID   
 where   
   (not exists(select top 1 1 from optionRoot orr where orr.underlyingID = c.ContractID and orr.ActionId < 3) and 
    not exists(select top 1 1 from futureRoot orr where orr.underlyingID = c.ContractID) )
   and c.ActionId < 3
   and  c.contractTypeId in (1,2,6)  
  group by csp.customStrikeSkewPointID  


 delete from customstrikeskewpoint where customStrikeSkewPointID in (  
	select customstrikeskewpoint from #CustStrikeskewpoint  
 )  
/*  
 ----------------------------------------------------------------------------------  
 delete from customstrikeskewpoint where customStrikeSkewPointID in (  
 --update customstrikeskewpoint set ActionId = 3 where customStrikeSkewPointID in (   
  select csp.customStrikeSkewPointID from contract c   
   inner join VolaSurfaceData vsd on vsd.contractID = c.ContractID  
   inner join customstrikeskewpoint csp on vsd.volaSurfaceDataID = csp.volaSurfaceDataID   
  where   
   not exists(select top 1 1 from optionRoot orr where orr.ActionId < 3 and orr.underlyingID = c.ContractID)
   and c.ActionId < 3
   and  c.contractTypeId in (1,2,6)
  group by csp.customStrikeSkewPointID
 )  
*/  
 delete from customstrikeskewpoint where customStrikeSkewPointID in (  
 --update customstrikeskewpoint set ActionId = 3 where customStrikeSkewPointID in (   
  select csp.customStrikeSkewPointID from contract c   
   inner join VolaSurfaceData vsd on vsd.contractID= c.ContractID  
   inner join customstrikeskewpoint csp on vsd.volaSurfaceDataID = csp.volaSurfaceDataID and  datediff(d,csp.actionDate, getDate()) >0  
  where   
   exists(select top 1 1 from optionRoot orr where orr.ActionId<3 and orr.underlyingID =c.ContractID)  
   and c.ActionId <3  
   and  c.contractTypeId in (1,2,6)
   and isnull(vsd.isManualVol, 0) <> 1
   and vsd.isDefaultSurface = 1
   and c.ContractID  not in (    
    select  tt.contractID from (  
     select t.contractID, t.symbol, max(t.maxDate) as md from (  
      select  c.contractID, c.symbol, csp.actionDate as maxDate from contract c   
       inner join VolaSurfaceData vsd on vsd.contractID= c.ContractID and vsd.isDefaultSurface = 1
       inner join customstrikeskewpoint csp on vsd.volaSurfaceDataID = csp.volaSurfaceDataID --and csp.expiryID> 66  
      where   
       (exists(select top 1 1 from optionRoot orr where orr.underlyingID = c.ContractID and orr.ActionId < 3) or
        exists(select top 1 1 from futureRoot orr where orr.underlyingID = c.ContractID)  
       )  
       and c.ActionId < 3
       and  c.contractTypeId in (1,2,6)
      group by  c.ContractID, c.symbol, csp.actionDate) T
     group by t.contractID, t.symbol  
    ) TT where datediff(d,TT.md, getDate())<>0)  
    group by csp.customStrikeSkewPointID  
  )  
  
  
 if @iWithTran is not null commit tran @vcTranName  
   
finish:  
 return @error  
  
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


---------------------------------------------------------------------------------------------------------------------------------------------
/*DATA MODIFICATION COMPLETE*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.46.261')
	begin
		update DataInfo set keyValue = '4.46.261' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.46.261' '4.46.261 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO


