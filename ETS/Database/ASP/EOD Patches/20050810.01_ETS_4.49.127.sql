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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.126' or (keyValue = '4.49.127')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.127 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.127' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO
if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='IRPoint' and COLUMN_NAME='neutralRate')
begin
	ALTER TABLE IRPoint ADD neutralRate float NULL
end
GO
if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='IRPoint' and COLUMN_NAME='neutralRate')
begin
	update IRPoint set neutralRate = (shortRate + longRate)/2
	ALTER TABLE IRPoint ALTER COLUMN neutralRate float NOT NULL
	
end
GO
ALTER View IRPointView
as
	Select 
		IRpointID,
		curveID,
		periodTypeID,
		num,
		shortRate,
		HTBRate,
		longRate,
		actionID,
		actionDate,
		neutralRate
	from IRPoint
	where actionID < 3
GO

ALTER  PROC dbo.usp_IRPoint_Get
	@iCurveID int = null
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	
	if @iCurveID is not null
		select
			IRpointID as iIRpointID, 
			curveID as iCurveID, 
			periodTypeID as iPeriodTypeID, 
			num as iNum, 
			shortRate as fShortRate, 
			HTBRate as fHTBRate, 
			longRate as fLongRate,
			actionDate as dtActionDate,
			actionID as tiActionID,
			neutralRate as neutralRate
		from
			IRPointView
		where
			curveID = @iCurveID
		order by periodTypeID, num 
	else
		select
			IRpointID as iIRpointID, 
			curveID as iCurveID, 
			periodTypeID as iPeriodTypeID, 
			num as iNum, 
			shortRate as fShortRate, 
			HTBRate as fHTBRate, 
			longRate as fLongRate,
			actionDate as dtActionDate,
			actionID as tiActionID,
			neutralRate as neutralRate
		from
			IRPointView
		order by periodTypeID, num 

	set @error = @@error
Return(@error)
GO
ALTER   Procedure dbo.usp_IRPoint_Save
	@iIRpointID int = Null,
	@iCurveID int = Null,
	@iPeriodTypeID int = Null,
	@iNum int = Null,
	@fShortRate float = Null,
	@fHTBRate float = Null,
	@fLongRate float = Null,
	@fNeutralRate float = Null

as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from IRPoint where Num = @iNum and  PeriodTypeID=@iPeriodTypeID and actionID < 3 and (@iIRpointID <> IRpointID or @iIRpointID is null))
		begin
			Raiserror ('IRPoint with this period already exists!', 16, 1)
			Return (-1)
		end   
	if @iCurveID is not Null and @iIRpointID is Null
		begin
			insert into IRPoint
					(
					curveID,
					periodTypeID,
					num,
					shortRate,
					HTBRate,
					longRate,
					neutralRate
					) 
				values (
					@iCurveID,
					@iPeriodTypeID,
					@iNum,
					@fShortRate,
					@fHTBRate,
					@fLongRate,
					isNull(@fNeutralRate, (@fShortRate+@fLongRate)/2)
										)
			select @iIRpointID = @@identity
		end
	else if @iIRpointID is not Null
		begin
			update IRPoint set
				curveID = isNull(@iCurveID, curveID),
				periodTypeID = isNull(@iPeriodTypeID, periodTypeID),
				num = isNull(@iNum, num),
				shortRate = isNull(@fShortRate, shortRate),
				HTBRate = isNull(@fHTBRate, HTBRate),
				longRate = isNull(@fLongRate, longRate),
				neutralRate = isNull(@fNeutralRate, neutralRate)
			where IRpointID = @iIRpointID
		end
	set @error = @@error
	if (@error = 0) set @error = @iCurveID
Return(@error)
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRPoint_Update]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRPoint_Update]
GO

CREATE PROC dbo.usp_IRPoint_Update
	@iPeriodTypeID int = Null,
	@iNum int = Null,
	@fShortRate float = Null,
	@fLongRate float = Null,
	@fNeutralRate float = Null

as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if exists (select * from IRPoint where Num = @iNum and  PeriodTypeID=@iPeriodTypeID )
	begin
		update IRPoint set
			actionID = 1,
			shortRate = isNull(@fShortRate, shortRate),
			longRate = isNull(@fLongRate, longRate),
			neutralRate = isNull(@fNeutralRate, neutralRate)
		where @iNum = num AND @iPeriodTypeID = periodTypeID
	end	
 	else
		begin
			declare @curveID  int
	
			DECLARE curs1 CURSOR FOR
			select curveID
			from IRCurve
			
			OPEN curs1
			FETCH NEXT FROM curs1 
			INTO @curveID
			
			WHILE @@FETCH_STATUS = 0
			BEGIN
				exec dbo.usp_IRPoint_Save null,@curveID,@iPeriodTypeID,@iNum, @fShortRate,0,@fLongRate,@fNeutralRate
				FETCH NEXT FROM curs1 INTO @curveID
			END
		
			CLOSE curs1
			DEALLOCATE curs1

		end
	set @error = @@error
Return(@error)

GO

GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_IRPoints_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_IRPoints_Import]
GO

CREATE  PROC dbo.usp_IRPoints_Import
	@txXmlUpdateData as ntext
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
		
	Declare @idoc int
	--Create an internal representation of the XML document.
	Exec sp_xml_preparedocument @idoc OUTPUT, @txXmlUpdateData
	
	-- SELECT statement using OPENXML rowset provider

	update IRPoint set actionID=3

	declare @Num  int,
		@NeutralRate float,
		@ShortRate float,
		@LongRate float

	DECLARE curs CURSOR FOR
	SELECT Num, NeutralRate, ShortRate, LongRate
	FROM       OPENXML (@idoc, '/IRCurve/IRPoint')
			WITH (Num  int,
				NeutralRate float,
				ShortRate float,
				LongRate float)
	
	OPEN curs
	FETCH NEXT FROM curs 
	INTO @Num,
		@NeutralRate,
		@ShortRate,
		@LongRate
	WHILE @@FETCH_STATUS = 0
	BEGIN
		exec dbo.usp_IRPoint_Update 1, @Num, @ShortRate, @LongRate, @NeutralRate 
		FETCH NEXT FROM curs 
		INTO @Num,
			@NeutralRate,
			@ShortRate,
			@LongRate
	END
	CLOSE curs
	DEALLOCATE curs

	--And finaly to release allocated XML datatable - following code must be executed
	Exec sp_xml_removedocument @idoc
	set @error = @@error
Return(@error)
GO
ALTER   Trigger tU_IRPoint ON dbo.IRPoint
    for Update
AS
	--======================================================================
	-- fix row modifications
	--======================================================================
	set nocount on
	
	-- 1st step - update actionDate if record not already deleted
	update IRPoint
		set actionDate = GetDate()
	from IRPoint IRP 
			join inserted on IRP.IRPointID = inserted.IRPointID
			join deleted on IRP.IRPointID = deleted.IRPointID
	where deleted.actionID < 3

	-- 2nd step - if update record -> set actionID=2
	update IRPoint
		set actionID=2
	from IRPoint IRP
			join inserted on IRP.IRPointID = inserted.IRPointID
			join deleted on IRP.IRPointID = deleted.IRPointID
	where deleted.actionID<3 and inserted.actionID = 1
/*
	-- 3d step -disable restore of deleted record
	update IRPoint 
		set actionID = 3
	from IRPoint IRP
			join inserted on IRP.IRPointID = inserted.IRPointID
			join deleted on IRP.IRPointID = deleted.IRPointID
	where deleted.actionID=3
*/
GO
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_Option_Bulk_Import]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_Option_Bulk_Import]
GO

CREATE PROCEDURE dbo.usp_Option_Bulk_Import
	@iUnderlyingContractID int,
	@iBulkXMLData ntext
as

	DECLARE @vcSymbol varchar(20)
	DECLARE @iIsCall  tinyint
	DECLARE @fStrike  float
	DECLARE @dtExpiry datetime
	DECLARE @iLotSize int
	DECLARE @iDoc 	  int
	DECLARE @fClosePrice float
	DECLARE	@vcUnderlyingSymbol varchar(8)
	DECLARE @isError  tinyint
	DECLARE @ERR_MSG  varchar(8000)
   	
	select @vcUnderlyingSymbol = symbol from ContractView where contractID = @iUnderlyingContractID
	set @isError = 0
	exec sp_xml_preparedocument @iDoc OUTPUT, @iBulkXMLData
	DECLARE BulkDataCursor CURSOR FOR
		select * 
			FROM OPENXML(@iDoc, '/Import/Option')
 				WITH (Symbol	  varchar(20),
				      IsCall      tinyint ,    	
				      Strike      float ,    	
               			      Expiry	  datetime ,
				      LotSize     int,
				      ClosePrice  float )

	
	OPEN BulkDataCursor
	FETCH NEXT FROM BulkDataCursor 
	INTO  @vcSymbol, @iIsCall, @fStrike, @dtExpiry,@iLotSize,@fClosePrice

	WHILE @@FETCH_STATUS = 0
	BEGIN
		declare @iRet int
		--select @iUnderlyingContractID, @vcSymbol, @iIsCall, @fStrike, @dtExpiry,@iLotSize
		exec @iRet = usp_Option_Import @iUnderlyingContractID, @vcSymbol, @iIsCall, @fStrike, @dtExpiry,@iLotSize, NULL, @vcUnderlyingSymbol, @fClosePrice
		if @iRet <> 0 
		begin 
		   set @isError = -1	
		   exec usp_EodMsOptionCache_Save @iUnderlyingContractID, @vcSymbol, @dtExpiry, @fStrike, @iIsCall, @iLotSize
		end

		FETCH NEXT FROM BulkDataCursor 
		INTO @vcSymbol, @iIsCall, @fStrike, @dtExpiry,@iLotSize,@fClosePrice
	END


	CLOSE      BulkDataCursor
	DEALLOCATE BulkDataCursor

	exec sp_xml_removedocument @iDoc
	return @isError
	
GO

ALTER  PROCEDURE dbo.usp_Option_Import
	@iUnderlyingContractID int,
	@vcSymbol varchar(20),
	@iIsCall tinyint,
	@fStrike float,
	@dtExpiry smalldatetime,
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
			@iExpiryID int,
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


	select top 1 @iLastExpiryID = expiryID from ExpiryInCalendarView where expiryDate <= GetDate() and expCalendarID = 1 order by expiryID desc
	select @dtExpiry = expiryDate from ExpiryInCalendarView where expiryID = @iExpiryID and expCalendarID = 1

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
					select @iUnderlyingContractID, @vcOptionRootSymbol, isnull(@iLotSize,100)
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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.127')
	begin
		update DataInfo set keyValue = '4.49.127' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.127' '4.49.127 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
-- unable to execute this script in transaction
