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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.145' or (keyValue = '4.49.146')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.146 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.145' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

GO

ALTER  PROCEDURE dbo.usp_EodOptionWithPos_Get
	@iUnderlyingID INT = NULL,
	@dtMinExpDate DATETIME = NULL
AS
	-------------------------------------------------
	SET NOCOUNT ON
	-------------------------------------------------
	DECLARE @error INT		SET @error = -1

	IF @dtMinExpDate IS NULL SET @dtMinExpDate = GETDATE()
	if @iUnderlyingID is null
		SELECT DISTINCT
			ort.underlyingID AS iUnderlyingID,
			ort.optionRootID AS iOptionRootID,
			ort.symbol AS vcOptionRootName,
			ort.IsSynthetic AS tiIsSynthetic, 
			ort.lotSize AS iLotSize, 
			sor.cAShValue AS fCASh,
			sor.synthSkew AS fSyntheticSkew, 
			sor.synthKurt AS fSyntheticKurt,
			sor.synthYield AS fSyntheticYield, 
			sor.synthIsBASket AS tiSyntheticIsBASket,
			c.contractID AS iOptionID,
			c.symbol AS vcSymbol,
			o.strike AS fStrike, 
			o.expiryDate AS dtExpiry,
			o.isCall AS tiIsCall,
		    cp.priceClose AS fPriceClose
		FROM
			[Option] o
			INNER JOIN ContractView c ON c.contractID = o.optionID 
			INNER JOIN OptionRoot ort ON o.optionRootID = ort.optionRootID
			LEFT JOIN SyntheticOptionRoot AS sor ON ort.optionRootID = sor.optionRootID
			LEFT JOIN ContractPrice cp ON cp.contractID = c.contractID AND cp.exchangeID IS NULL						
		WHERE
			datediff(d, @dtMinExpDate, o.expiryDate) >= 0
		ORDER BY ort.underlyingID,ort.optionRootID
	ELSE
		SELECT DISTINCT
			ort.underlyingID AS iUnderlyingID,
			ort.optionRootID AS iOptionRootID,
			ort.symbol AS vcOptionRootName,
			ort.IsSynthetic AS tiIsSynthetic, 
			ort.lotSize AS iLotSize, 
			sor.cAShValue AS fCASh,
			sor.synthSkew AS fSyntheticSkew, 
			sor.synthKurt AS fSyntheticKurt,
			sor.synthYield AS fSyntheticYield, 
			sor.synthIsBASket AS tiSyntheticIsBASket,
			c.contractID AS iOptionID,
			c.symbol AS vcSymbol,
			o.strike AS fStrike, 
			o.expiryDate AS dtExpiry,
			o.isCall AS tiIsCall,
		    cp.priceClose AS fPriceClose
		FROM
			[OptionView] o
			INNER JOIN Contract c ON c.contractID = o.optionID 
			INNER JOIN OptionRoot ort ON o.optionRootID = ort.optionRootID
			LEFT JOIN SyntheticOptionRoot AS sor ON ort.optionRootID = sor.optionRootID
			LEFT JOIN ContractPrice cp ON cp.contractID = c.contractID AND cp.exchangeID IS NULL						
		WHERE
			ort.underlyingID = ISNULL(@iUnderlyingID, ort.underlyingID)
			AND datediff(d, @dtMinExpDate, o.expiryDate) >= 0
		ORDER BY ort.underlyingID,ort.optionRootID


	SET @error = @@ERROR
	RETURN(@error)
GO

ALTER PROCEDURE usp_EodFuturesOptionWithPositions_Get
AS
	 ---------------
	 SET NOCOUNT ON
	 ---------------
	 DECLARE @error INT  SET @error = 0
	--=================================================================
	-- select future option with positions
	--=================================================================
	 SELECT DISTINCT 
		 FO.futureID as iFutureID,
		 FC.symbol as vcFutureSymbol,
		 FOC.symbol as vcOptionSymbol,
		 FO.futureOptionID as iID,
		 FO.isCall as tiIsCall,
		 FO.strike as fStrike,
		 FO.expiryDate AS dtExpiry
	 FROM
	 	TradeView tv  
		 INNER JOIN FutureOption FO ON FO.futureOptionID = tv.contractID
		 INNER JOIN ContractView FOC ON FOC.contractID = tv.contractID
		 INNER JOIN ContractView FC ON FO.futureID = FC.contractID
	 WHERE
		CAST( FO.expiryDate AS INT ) >= CAST( GETDATE() AS  INT )
	 
	SET @error = @@ERROR
	RETURN(@error)
GO


ALTER   PROCEDURE usp_EodOldFutureOptions_Del
	@iFutureID INT
AS
	-------------------------------------------------
	SET NOCOUNT ON
	-------------------------------------------------
	DECLARE @error INT		SET @error = 0

	CREATE TABLE #FutureContractsForDelete(contractID INT) 

	CREATE TABLE #FutureContracts_with_trades(contractID INT/*, symbol VARCHAR(20)*/)

	--=============================================================
	-- select futures options, which have a trades or orders
	--=============================================================
	INSERT INTO #FutureContracts_with_trades
		SELECT FO.futureOptionID --, C.symbol
			FROM 
				FutureOption FO
					JOIN Future F ON F.futureID = FO.futureID
			WHERE 
				( 
					( 
						FO.expiryDate < GETDATE() OR DATEDIFF(d, FO.actionDate, GETDATE()) > 0 
					)
					AND 
					(
						FO.futureOptionID IN (SELECT DISTINCT ContractID FROM Trade) 
						OR FO.futureOptionID IN (SELECT DISTINCT ContractID FROM [Order])
					)
					AND FO.futureID = @iFutureID
				)

	--=============================================================
	-- select futures options for remove
	--=============================================================
	INSERT INTO #FutureContractsForDelete
		SELECT FO.futureOptionID
			FROM 
				FutureOption FO
					JOIN Future F ON F.futureID = FO.futureID
			WHERE 
				( 
					( 
						FO.expiryDate < GETDATE() or DATEDIFF(d, FO.actionDate, GETDATE()) > 0 
					)
					AND 
					(
						FO.futureOptionID NOT IN (SELECT DISTINCT ContractID FROM Trade) 
						AND FO.futureOptionID NOT IN (SELECT DISTINCT ContractID FROM [Order])
					)
					AND FO.futureID = @iFutureID
				)

	EXEC @error = usp_EodFutureOption_Del
	-- leave this shit or change CEodStepLoadMktStructure::_FinishLoadRootFutures() in EodLoadMktStructure.cpp
	SELECT NULL AS vcSymbol FROM #FutureContracts_with_trades

finish:
	RETURN(@error)


GO



ALTER  PROC dbo.usp_EodFuture_Del
AS
	set nocount on
	
	declare @error int		set @error = 0

	if object_ID('tempdb..#FutureContractsForDelete') is null
	begin
		set @error = -1
		goto finish
	end

	begin tran
		
	-- delete future option
	delete FutureOption
	where futureoptionID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -2 if @@trancount = 1 rollback tran goto finish end
	
	-- delete future
	delete Future where
	futureID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran goto finish end


	-- delete common future and future options
	
	delete ContractPrice
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -4 if @@trancount = 1 rollback tran goto finish end
	
	delete ContractExchangeLink
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

	delete  VolaSurfaceData
	where contractID in (select contractID from #FutureContractsForDelete)

	delete ContractInGroup 
	where contractID in (select contractID from #FutureContractsForDelete)
	
	delete Contract
	where contractID in (select contractID from #FutureContractsForDelete)
	if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end

	commit tran

finish:
	return @error


GO


ALTER  PROCEDURE dbo.usp_EodFuturesWithPositions_Get
  as
	---------------
	set nocount on
	---------------

	declare @error int		set @error = 0

	select 
		FR.underlyingID as iIndexID,
		F.futureID as iFutureID,
		FC.symbol as vcSymbol,
		F.isActive as tiIsActive,
		F.maturityDate as dtMaturityDate,
		0 as fPosition,
		cp.priceClose as fPriceClose
	from Future F
		inner join Contract FC on FC.contractID=F.futureID
		inner join FutureRoot FR on FR.futureRootID=F.futureRootID
		inner join ContractPrice cp on cp.contractID = FC.contractID and cp.exchangeID is null
	group by FR.underlyingID, F.futureID, FC.symbol, F.isActive, F.maturityDate, cp.priceClose
	order by FR.underlyingID 

/*
	select 
		FR.underlyingID as iIndexID,
		F.futureID as iFutureID,
		FC.symbol as vcSymbol,
		F.isActive as tiIsActive,
		F.maturityID as iMaturityID,
		sum(T.quantity * T.price * (case when T.isBuy <> 0 then 1 else -1 end)) as fPosition,
		cp.priceClose as fPriceClose
	from TradeView T  
		inner join Future F on T.contractID=F.futureID
		inner join Contract FC on FC.contractID=T.contractID
		inner join FutureRoot FR on FR.futureRootID=F.futureRootID
		inner join ContractPrice cp on cp.contractID = FC.contractID and cp.exchangeID is null
	group by FR.underlyingID, F.futureID, FC.symbol, F.isActive, F.maturityID, cp.priceClose
	order by FR.underlyingID 
*/	
	set @error = @@error
	Return(@error)

GO

ALTER PROCEDURE dbo.job_FixMarketStructure
	@iNumberOfHours int = null,
	@iContractID int = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure mark us deleted unusefull options
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @iNumberOfHours is null 
		set @iNumberOfHours = 2

	Create table #Options_for_delete(OptionID int) 

	insert into #Options_for_delete
		select O.optionID--,E.expiryDate 
			from 
				[OptionView] O
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract UC on ORt.underlyingID = UC.contractID 
			where datediff(d,GetDate(),O.expiryDate )< 0 
					and O.isManualyEntered = 0
					and O.optionID not in (select distinct ContractID from Trade)
					and O.optionID not in (select distinct ContractID from [Order])

	exec usp_Option_Del
		if @@error <> 0 
			begin 
				Raiserror ('Error. Can''t mark us delete useless options!', 16, 1)
				set @error = -1 
				if @@TranCount = 1 Rollback Transaction goto finish 
			end

    set @error = 0
finish:
		Return(@error)


GO

ALTER PROC dbo.usp_BoFutureOption_Get
	@iFutureID int = null
AS
	-- get future options

	set nocount on
	
	if @iFutureID is not null
		select
			fo.futureOptionID as iFutureOptionID,
			foc.contractTypeID as iFutureOptionContractTypeID,
			foc.symbol as vcFutureOptionSymbol,
			foc.contractName as vcFutureOptionName,
			f.futureID as iFutureID,
			fc.contractTypeID as iFutureContractTypeID,
			fc.symbol as vcFutureSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			fo.expiryDate as dtExpiry,
			fo.strike as fStrike, 
			fo.isCall as tiIsCall,
			fo.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureOption fo on f.futureID = fo.futureID
			inner join ContractView foc on fo.futureOptionID = foc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
		where
			f.futureID = @iFutureID
	else
		select
			fo.futureOptionID as iFutureOptionID,
			foc.contractTypeID as iFutureOptionContractTypeID,
			foc.symbol as vcFutureOptionSymbol,
			foc.contractName as vcFutureOptionName,
			f.futureID as iFutureID,
			fc.contractTypeID as iFutureContractTypeID,
			fc.symbol as vcFutureSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			fo.expiryDate as dtExpiry,
			fo.strike as fStrike, 
			fo.isCall as tiIsCall,
			fo.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureOption fo on f.futureID = fo.futureID
			inner join ContractView foc on fo.futureOptionID = foc.contractID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID


GO

ALTER   procedure usp_CustomStrikeSkewDefaultPoint2_Save
	@iContractID int,
	@iExpiryID int,
	@iType int,
	@fVolatility float
as
/*
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @RootType int

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int set @iVolaSurfaceDataID = 0

	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	if @iVolaSurfaceDataID is not null
	begin
		exec @RootType =  usp_RootType_Get @iContractID
		-- option root
		if @RootType = 0 
		begin
			insert into CustomStrikeSkewPoint(VolaSurfaceDataID, strike, type, volatility, isBasePoint, expiryID)
					select distinct 
						@iVolaSurfaceDataID, round(ov.strike, 4) as fStrike, @iType, @fVolatility, -1, @iExpiryID
					from OptionView ov
						join OptionRootView ort on ov.optionRootID = ort.optionRootID
					where ov.expiryID = @iExpiryID 
						and ort.underlyingID = @iContractID
						and not exists(select 1 
										from CustomStrikeSkewPointView cssp
										where cssp.VolaSurfaceDataID = @iVolaSurfaceDataID and 
												round(cssp.strike, 4) = round(ov.strike, 4)
												and cssp.expiryID = ov.expiryID)
	
		end
		

		-- future option root
		if @RootType = 1 
		begin
			insert into CustomStrikeSkewPoint(VolaSurfaceDataID, strike, type, volatility, isBasePoint, expiryID)
				select distinct 
					@iVolaSurfaceDataID, round(fo.strike, 4) as fStrike, @iType, @fVolatility, -1, @iExpiryID
				from    dbo.Future f inner JOIN
				        dbo.FutureOption fo  on f.futureID = fo.futureID inner JOIN
				                      dbo.Expiry e ON fo.expiryID = e.expiryID
				where   f.MaturityID = @iExpiryID 
					and f.futureID = @iContractID
					and not exists(select 1 
									from CustomStrikeSkewPointView cssp
									where cssp.VolaSurfaceDataID = @iVolaSurfaceDataID and 
											round(cssp.strike, 4) = round(fo.strike, 4)
											and cssp.expiryID = fo.ExpiryID)

		end
	
	end
*/
GO
if exists (select * from dbo.sysindexes where name = N'IX_Option_optionRootID_isCall_strike_expiryID_actionID' and id = object_id(N'[dbo].[Option]'))
drop index [dbo].[Option].[IX_Option_optionRootID_isCall_strike_expiryID_actionID]

GO
CREATE 
  INDEX [IX_Option_optionRootID_isCall_strike_expiryID_actionID] ON [dbo].[Option] ([optionRootID], [isCall], [strike], [expiryDate], [actionID])
ON [PRIMARY]
GO

GO

if exists (select * from dbo.sysobjects where id = object_id(N'dbo.usp_EodExpiredOptions_Del') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure dbo.usp_EodExpiredOptions_Del 
GO

CREATE PROCEDURE dbo.usp_EodExpiredOptions_Del 
	@iContractID INT -- stock or index identifier
AS
	-------------------------------------------------
	SET NOCOUNT ON
	-------------------------------------------------
	DECLARE @error INT		SET @error = 0

	CREATE TABLE #Options_for_delete(OptionID INT) 

	INSERT INTO #Options_for_delete
		SELECT O.optionID
			FROM [Option] O
			inner join OptionRoot optR on OptR.OptionRootID = O.OptionRootID
			inner join ContractView uc on uc.ContractID = optR.UnderlyingID
		WHERE (datediff(d, GETDATE(), O.expiryDate) < 0 OR DATEDIFF(d, O.actionDate, GETDATE()) > 0)
			AND O.actionID < 3
			AND UC.contractID = @iContractID

	IF EXISTS (SELECT 1 FROM #Options_for_delete)
	BEGIN
		BEGIN TRAN
	
		DELETE OptionDataCache
		FROM OptionDataCache t
			INNER JOIN #Options_for_delete d on t.optionID = d.optionID
		IF @@ERROR <> 0 BEGIN SET @error = -1 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END

		DELETE FROM ContractPrice
		where ContractPrice.contractID in (select optionID from #Options_for_delete)
		IF @@ERROR <> 0 BEGIN SET @error = -1 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END

		DELETE [Option]
		WHERE OptionID in (select optionID from #Options_for_delete) 
		IF @@ERROR <> 0 BEGIN SET @error = -1 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END
	
		DELETE Contract 
		FROM Contract T
			INNER JOIN #Options_for_delete D ON t.contractID = d.optionID
		IF @@ERROR <> 0 BEGIN SET @error = -1 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END

		CREATE TABLE #Optionroots_for_delete (optionrootID INT) 
		IF @@ERROR <> 0 BEGIN SET @error = -1 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END

		INSERT #OptionRoots_for_delete (optionRootID)
		SELECT ort.optionRootID
		FROM OptionRoot ort
			LEFT JOIN [Option] o ON o.optionRootID = ort.optionRootID and o.actionID < 3
		WHERE ort.actionID < 3
			AND ort.underlyingID = @iContractID
			AND o.optionRootID IS NULL
		GROUP BY ort.optionRootID
		IF @@ERROR <> 0 BEGIN SET @error = -1 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END

		DELETE SyntheticUnderlyingParam
		FROM SyntheticUnderlyingParam t
			INNER JOIN SyntheticOptionRoot t2 ON t2.syntheticOptRootID = t.syntheticOptRootID
			INNER JOIN #OptionRoots_for_delete d ON d.optionRootID = t2.optionRootID
		IF @@ERROR <> 0 BEGIN SET @error = -1 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END
	
		DELETE SyntheticRootIndexBeta
		FROM SyntheticRootIndexBeta t
			INNER JOIN SyntheticOptionRoot t2 ON t2.syntheticOptRootID = t.syntheticOptRootID
			INNER JOIN #OptionRoots_for_delete d ON d.optionRootID = t2.optionRootID
		IF @@ERROR <> 0 BEGIN SET @error = -1 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END
	
		DELETE SyntheticOptionRoot
		FROM SyntheticOptionRoot t
			INNER JOIN #OptionRoots_for_delete d ON d.optionRootID = t.optionRootID
		IF @@ERROR <> 0 BEGIN SET @error = -1 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END
	
		DELETE OptionRoot
		FROM OptionRoot t
			INNER JOIN #OptionRoots_for_delete d ON d.optionRootID = t.optionRootID
		IF @@ERROR <> 0 BEGIN SET @error = -1 IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION GOTO finish END

		COMMIT TRAN
	END

	SELECT NULL AS vcSymbol FROM #Options_for_delete WHERE OptionID IS NULL


finish:
	IF @error != 0
		RAISERROR ('Error. Can''t delete useless options!', 16, 1)
	RETURN @error


GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER      procedure dbo.usp_Option_Save
		@iOptionID int,
		@vcSymbol varchar(20) = Null,
		@iExchangeID int, 
		@iUnderlyingContractID int,
		@iIsCall tinyint = Null,
		@fStrike float = Null,
		@dtExpiry datetime = Null,
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
 	  
	   if len(@vcSymbol) < 3  and @iOptionID is null
	   begin
		 RAISERROR ('Error. Option symbol can''t be less three symbols!', 16, 1)
		 RETURN (-1)
	   end	 

	   if exists (select optionRootID 
		   from OptionRootView
		   where symbol = left(@vcSymbol,len(@vcSymbol)-2)and 
			underlyingID != @iUnderlyingContractID)
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

	select @iOptionRootID = optionRootID from OptionRootView where symbol = left(@vcSymbol,len(@vcSymbol)-2)  and 
		underlyingID=@iUnderlyingContractID	

	declare @ERR_MSG varchar(8000),
			@vcUnderlyingSymbol varchar(10)

	select @vcUnderlyingSymbol = symbol 
		from ContractView 
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
			
			insert into [Option] (optionID, optionRootID, isCall, strike, expiryDate)
				values (@iOptionID, @iOptionRootID, @iIsCall, @fStrike, @dtExpiry)
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

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
					expiryDate = isNull(@dtExpiry, expiryDate)
					--expiryID = isNull(@iExpiryID, expiryID)
				where optionID = @iOptionID
			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

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

-----------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER  PROC dbo.usp_BoFutureOption_Save
	@iFutureOptionID int out,
	@iFutureID int, -- should be specified for new future option (ignored for update)
	@vcFutureOptionSymbol varchar(20) = null,
	@vcFutureOptionName varchar(255) = null, -- calculated if not specified (for new option)
	@vcExportSymbol varchar(20) = null,
	@tiIsCall tinyint = null,
	@fStrike float = null,
	--@iExpiryID int = null, -- calculated by @dtExpiryDate if not specified
	@dtExpiryDate datetime = null, -- ignored if @iExpiryID is specified
	@tiIsRaiseError tinyint = 1 
AS
	-- create/update future

	set nocount on
	
	declare @error int		set @error = 0

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
		select @iFutureContractTypeID = contractTypeID,
			@vcFutureSymbol = symbol --,
			--@iFutureExpCalendarID = expCalendarID
		from ContractView
		where contractID = @iFutureID		

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
		values
			(5, @vcFutureOptionSymbol, @vcFutureOptionName/*, null*/, null, null)
		if @@error <> 0 begin set @error = -9 if @@trancount = 1 rollback tran goto finish end

		set @iFutureOptionID = @@identity

		-- save new future option data
		insert into FutureOption
			(futureOptionID, futureID, isCall, expiryDate, strike)
		values
			(@iFutureOptionID, @iFutureID, @tiIsCall, @dtExpiryDate, @fStrike)
		if @@error <> 0 begin set @error = -10 if @@trancount = 1 rollback tran goto finish end

		-- save price record for default exchange
		if not exists(select 1 from ContractPrice where exchangeID is null and contractID = @iFutureOptionID)
		begin
			insert into ContractPrice
				(contractID, exchangeID)
			values
				(@iFutureOptionID, null)
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
			strike = isnull(@fStrike, strike)
		where futureOptionID = @iFutureOptionID
		if @@error <> 0 begin set @error = -17 if @@trancount = 1 rollback tran goto finish end

		commit tran
	end

finish:
if (@error <> 0) return (@error) else return @iFutureOptionID

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

------------------------------------------------------------------------------------------------------------------------------------------------

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



ALTER   PROC dbo.usp_BoFuture_Save
	@iFutureID int out,
	@iFutureRootID int, -- should be specified for new future (ignored for update)
	@vcFutureSymbol varchar(20) = null,
	@vcFutureName varchar(255) = null,
	@vcExportSymbol varchar(20) = null,
	--@iMaturityID int = null, -- calculated by @dtMaturityDate if not specified
	@dtMaturityDate datetime = null, -- ignored if @iMaturityID is specified
	@iExpCalendarID int = null,
	@tiCalcOptionType tinyint = null,
	@tiIsActive tinyint = null,
	@iUndPriceProfileID int = null out,
	@iOptPriceProfileID int = null out,
	@tiIsRaiseError tinyint = 1 
AS
	-- create/update future

	set nocount on
	
	declare @error int		set @error = 0

	if @iFutureID is null -- new future
	begin
		-- check new future root ID
		if @iFutureRootID is null
		begin
			if @tiIsRaiseError = 1 raiserror('Future root for new future is not specified!', 16, 1)
			set @error = -1
			goto finish
		end

		-- check future symbol
		set @vcFutureSymbol = isnull(@vcFutureSymbol, '')
		if len(ltrim(@vcFutureSymbol)) = 0
		begin
			if @tiIsRaiseError = 1 raiserror('Symbol for new future is not specified!', 16, 1)
			set @error = -2
			goto finish
		end

		-- check future
		if exists(select 1 from ContractView where contractTypeID = 4 and symbol = @vcFutureSymbol)
		begin
			if @tiIsRaiseError = 1 raiserror('Future with symbol ''%s'' is already exists!', 16, 1, @vcFutureSymbol)
			set @error = -3
			goto finish
		end

		-- correct expiry calendar ID
		--if isnull(@iExpCalendarID, 0) = 0
		--	set @iExpCalendarID = 1

		-- check expiry calendar ID
		--if not exists(select 1 from ExpiryCalendarView where expCalendarID = @iExpCalendarID)
		--begin
		--	if @tiIsRaiseError = 1 raiserror('Invalid future options expiry calendar specified!', 16, 1)
		--	set @error = -4
		--	goto finish
		--end

		-- calculate maturity ID by maturity date if maturity ID is not specified
		--if @iMaturityID is null and @dtMaturityDate is not null
		--	set @iMaturityID = cast (@dtMaturityDate as int)--(year(@dtMaturityDate) - 2000) * 12 + month(@dtMaturityDate)

		-- check maturity ID
		--if not exists(select 1 from ExpiryView where expiryID = @iMaturityID)
		--begin
		--	if @tiIsRaiseError = 1 raiserror('Invalid future maturity specified!', 16, 1)
		--	set @error = -5
		--	goto finish
		--end

		-- correct default underlying price profile
		if isnull(@iUndPriceProfileID, 0) = 0
			select
				@iUndPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 0

		-- check underlying price profile
		if not exists(select 1 from PriceProfileView where priceProfileID = @iUndPriceProfileID)
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future underlying price profile specified!', 16, 1)
			set @error = -6
			goto finish
		end
	
		-- correct default option price profile
		if isnull(@iOptPriceProfileID, 0) = 0
			select
				@iOptPriceProfileID = priceProfileID
			from
				PriceProfileView
			where
				defaultID = 1
				and isOptionProfile = 1

		-- check options price profile
		if not exists(select 1 from PriceProfileView where priceProfileID = @iOptPriceProfileID)
		begin
			if @tiIsRaiseError = 1 raiserror('Invalid future options price profile specified!', 16, 1)
			set @error = -7
			goto finish
		end

		-- correct option style
		if isnull(@tiCalcOptionType, 0) = 0
			set @tiCalcOptionType = 1	-- american

		-- correct 'is active'
		if isnull(@tiIsActive, 0) = 0
			set @tiIsActive = 1	-- active by default

		begin tran

		-- save new contract data
		insert into Contract
			(contractTypeID, symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
		values
			(4, @vcFutureSymbol, @vcFutureName, null, @iUndPriceProfileID, @iOptPriceProfileID)
		if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end

		set @iFutureID = @@identity

		-- save new future data
		insert into Future
			(futureID, futureRootID, maturityDate, calcOptionType, isActive)
		values
			(@iFutureID, @iFutureRootID, @dtMaturityDate, @tiCalcOptionType, @tiIsActive)
		if @@error <> 0 begin set @error = -9 if @@trancount = 1 rollback tran goto finish end

		-- save price record for default exchange
		if not exists(select 1 from ContractPrice where exchangeID is null and contractID = @iFutureID)
		begin
			insert into ContractPrice
				(contractID, exchangeID)
			values
				(@iFutureID, null)
			if @@error <> 0 begin set @error = -10 if @@trancount = 1 rollback tran goto finish end
		end

		commit tran
	end
	else
	begin -- existing future

		-- correct future name (prevent updates to empty string)
		if len(ltrim(@vcFutureSymbol)) = 0
			set @vcFutureSymbol = null

		-- check future name
		if @vcFutureSymbol is not null
			if exists(select 1 from ContractView where contractTypeID = 4 and contractID <> @iFutureID and symbol = @vcFutureSymbol)
			begin
				if @tiIsRaiseError = 1 raiserror('Future with symbol ''%s'' is already exists!', 16, 1, @vcFutureSymbol)
				set @error = -11
				goto finish
			end

		-- correct expiry calendar ID (prevent updates to zero calendar)
		--if @iExpCalendarID = 0
		--	set @iExpCalendarID = null

		-- check expiry calendar
		--if @iExpCalendarID is not null
		--begin
		--	if not exists(select 1 from ExpiryCalendar where expCalendarID = @iExpCalendarID)
		--	begin
		--		if @tiIsRaiseError = 1 raiserror('Invalid future options expiry calendar specified!', 16, 1, @vcFutureSymbol)
		--		set @error = -12
		--		goto finish
		--	end
		--end

		-- correct maturity ID (prevent updates to zero)
		--if @iMaturityID = 0
		--	set @iMaturityID = null

		-- calculate maturity ID by maturity date if maturity ID is not specified
		--if @iMaturityID is null and @dtMaturityDate is not null
		--	set @iMaturityID = cast(@dtMaturityDate as int) --(year(@dtMaturityDate) - 2000) * 12 + month(@dtMaturityDate)

		-- check maturity ID
		--if @iMaturityID is not null
		--	if not exists(select 1 from ExpiryView where expiryID = @iMaturityID)
		--	begin
		--		if @tiIsRaiseError = 1 raiserror('Invalid future maturity specified!', 16, 1)
		--		set @error = -13
		--		goto finish
		--	end

		-- correct underlying price profile (prevent updates to zero)
		if @iUndPriceProfileID = 0
			set @iUndPriceProfileID = null

		-- check underlying price profile
		if @iUndPriceProfileID is not null
			if not exists(select 1 from PriceProfileView where priceProfileID = @iUndPriceProfileID)
			begin
				if @tiIsRaiseError = 1 raiserror('Invalid future underlying price profile specified!', 16, 1)
				set @error = -14
				goto finish
			end

		-- correct option price profile (prevent updates to zero)
		if @iOptPriceProfileID = 0
			set @iOptPriceProfileID = null

		-- check option price profile
		if @iOptPriceProfileID is not null
			if not exists(select 1 from PriceProfileView where priceProfileID = @iOptPriceProfileID)
			begin
				if @tiIsRaiseError = 1 raiserror('Invalid future options price profile specified!', 16, 1)
				set @error = -15
				goto finish
			end

		begin tran

		-- update contract data
		update Contract
		set symbol = isnull(@vcFutureSymbol, symbol),
			contractName = isnull(@vcFutureName, contractName),
			expCalendarID = null,
			undPriceProfileID = isnull(@iUndPriceProfileID, undPriceProfileID),
			optPriceProfileID = isnull(@iOptPriceProfileID, optPriceProfileID)
		where contractID = @iFutureID
		if @@error <> 0 begin set @error = -16 if @@trancount = 1 rollback tran goto finish end

		-- save future data
		update Future
			set maturityDate = isnull(@dtMaturityDate, maturityDate),
			calcOptionType = isnull(@tiCalcOptionType, calcOptionType),
			isActive = isnull(@tiIsActive, isActive)
		where futureID = @iFutureID
		if @@error <> 0 begin set @error = -17 if @@trancount = 1 rollback tran goto finish end

		commit tran
	end
finish:
	return @error


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO



---------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER    PROCEDURE usp_IC_Save
      @iICID int,
      @vcSymbol varchar(20) = null,
      @vcName varchar(255) = null,
      @iICType int,   
      @tiGreeksCalcType tinyint = null,
      @tiIsActive tinyint = null,
      @iFutureUnderlyingTypeID int,
      @tiCheckExistance tinyint = null,
      @iExpCalendarID int = null,
      @iUndPriceProfileID int = null output,
      @iOptPriceProfileID int = null output
  
     as
    ---------------
    set nocount on
    ---------------
  --  if ((@iICType <> 6) And (@iICType <> 7 ))
  --    begin
  --      Raiserror ('Wrong Interest/FutureUnderlying type value', 16, 1)
  --      Return (-1)
  --    end
  
    if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID = @iICType and actionID < 3 and (@iICID <> contractID or @iICID is null))
      begin
        if isnull(@tiCheckExistance, 1) <> 0
          Raiserror ('Interest or FutureUnderlying with this ticker already exists!', 16, 1)
        Return (-1)
      end   
  
     if (@iICID is Null)  
     begin
       
       begin tran
        if @iUndPriceProfileID is null
        begin
          select
            @iUndPriceProfileID = priceProfileID
          from
            PriceProfileView
          where
            defaultID = 3
            and isOptionProfile = 0
        end
  
        if @iOptPriceProfileID is null
        begin
          select
            @iOptPriceProfileID = priceProfileID
          from
            PriceProfileView
          where
            defaultID = 1
            and isOptionProfile = 1
        end

      if @iICType = 6
      begin		
          --insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcSymbol, 'Default calendar')
          --set @iExpCalendarID = scope_identity()

               insert into Contract (contractTypeID, Symbol, contractName/*, expCalendarID*/, undPriceProfileID, optPriceProfileID)
                    values ( @iICType, @vcSymbol, @vcName/*, null*/, @iUndPriceProfileID, @iOptPriceProfileID)
      end
      else
      begin
          --insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcSymbol, 'Default calendar')
          --set @iExpCalendarID = scope_identity()  

               insert into Contract (contractTypeID, Symbol, contractName/*, expCalendarID*/, undPriceProfileID, optPriceProfileID)
                    values ( @iICType, @vcSymbol, @vcName/*, null*/, @iUndPriceProfileID, @iOptPriceProfileID)	
      end
  
         select @iICID = @@identity      
           if (@@error <> 0)
              begin
              RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
              ROLLBACK TRAN
              RETURN (-1)
              end
         insert into [FutureUnderlying] (
                iFutureUnderlyingID, 
                tiActive,
                tiGreeksCalculationType,
                iFutureUnderlyingTypeID)
             values (
                @iICID, 
                @tiIsActive,
                @tiGreeksCalcType,
                @iFutureUnderlyingTypeID)
           if (@@error <> 0)
            begin
               RAISERROR ('Error. Can''t insert into [FutureUnderlying]!', 16, 1)
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
            contractName = isNull(@vcName,contractName),
            --expCalendarID = null,--isNull(@iExpCalendarID, expCalendarID),
            undPriceProfileID = isnull(@iUndPriceProfileID, undPriceProfileID),
            optPriceProfileID = isnull(@iOptPriceProfileID, optPriceProfileID)
          where contractID = @iICID
        if (@@error <> 0)
           begin
            RAISERROR ('Error. Can''t update Contract!', 16, 1)
            ROLLBACK TRAN
            RETURN (-1)
           end
        update [FutureUnderlying]
           set  tiGreeksCalculationType = isnull(@tiGreeksCalcType, tiGreeksCalculationType),
            tiActive = isnull(@tiIsActive, tiActive),
            iFutureUnderlyingTypeID = isnull(@iFutureUnderlyingTypeID, iFutureUnderlyingTypeID)
           where iFutureUnderlyingID = @iICID
         if (@@error <> 0)
           begin
            RAISERROR ('Error. Can''t update [FutureUnderlying]!', 16, 1)
            ROLLBACK TRAN
            RETURN (-1)
           end       
       commit tran
     end
  
    --========================================================================
    -- make record with null exchange for this index
    --========================================================================
  --  if not exists(select * from ContractPrice where exchangeID is null and contractID = @iICID)
  --    begin
  --      insert into ContractPrice(contractID,exchangeID)
  --        values(@iICID,null)
  --      if (@@error <> 0) begin rollback tran return(-1) end              
  --    end   
  
  if (@@error <> 0)
    return (-1)
  else  
    return @iICID

GO



ALTER   PROCEDURE usp_Index_Save
		@iIndexID int,
		@vcSymbol varchar(20) = null,
		@vcContractName varchar(255) = null,
		@fYield float = Null,
		@bIsHTB bit = Null,
		@tiCalcOptionType tinyint = null,
		@tiCalcModelType tinyint = null,
		@iExpCalendarID int = null,
		@tiIsBasket tinyint = null,
		@tiIsActive tinyint = null,
		@fSkew float = null,
		@fKurt float = null,
		@tiCheckExistance tinyint = 1,
		@SOQ as integer = 0,
		@iUndPriceProfileID int = null output,
		@iOptPriceProfileID int = null output
   as
	---------------
	set nocount on
	---------------

	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID in (1, 2) and actionID < 3 and (@iIndexID <> contractID or @iIndexID is null))
		begin
			if isnull(@tiCheckExistance, 1) <> 0
				Raiserror ('Index or stock with this ticker already exists!', 16, 1)
			Return (-1)
		end   

   if (@iIndexID is Null)  
	 begin
		--Michael Malyshkin
		--if @iExpCalendarID is null or @iExpCalendarID = 0
		--	set @iExpCalendarID = 2
		--Michael Malyshkin
		 
		 begin tran
			if @iUndPriceProfileID is null
			begin
				select
					@iUndPriceProfileID = priceProfileID
				from
					PriceProfileView
				where
					defaultID = 3
					and isOptionProfile = 0
			end

			if @iOptPriceProfileID is null
			begin
				select
					@iOptPriceProfileID = priceProfileID
				from
					PriceProfileView
				where
					defaultID = 1
					and isOptionProfile = 1
			end

            insert into Contract (contractTypeID, Symbol, contractName, expCalendarID, undPriceProfileID, optPriceProfileID)
                  values (1, @vcSymbol, @vcContractName, null, @iUndPriceProfileID, @iOptPriceProfileID)

			 select @iIndexID = @@identity			 
				 if (@@error <> 0)
					  begin
						RAISERROR ('Error. Can''t insert into Contract!', 16, 1)
						ROLLBACK TRAN
						RETURN (-1)
					  end
			 insert into [Index] (
							indexID, 
							yield,
							IsHTB,
							calcOptionType,
							calcModelType,
							isBasket, 
							isActive,
							skew,
							kurt)
					 values (
							@iIndexID, 
							@fYield,
							isNull(@bIsHTB,0),
							@tiCalcOptionType,
							@tiCalcModelType,
							isnull(@tiIsBasket,0), 
							@tiIsActive,
							@fSkew,
							@fKurt)
				 if (@@error <> 0)
					begin
					   RAISERROR ('Error. Can''t insert into [Index]!', 16, 1)
					   ROLLBACK TRAN
					   RETURN (-1)
					end 			

   		 --insert into ExpiryCalendar (expCalendarName, expCalendarDesc) values(@vcSymbol, @vcSymbol)
         --        /* begin FT_CQ00008370*/
         --        declare @iExpCalendarID_Last int
         --       set @iExpCalendarID_Last = SCOPE_IDENTITY()

         --        update Contract set expCalendarID = @iExpCalendarID_Last
         --          where contractID = @iIndexID
         --        /* end   FT_CQ00008370*/
	
		 commit tran
	 end
   else
	 begin
		begin tran
		   update Contract
				set symbol = isNull(@vcSymbol,symbol),
					contractName = isNull(@vcContractName,contractName),
					--expCalendarID = null,--isNull(@iExpCalendarID, expCalendarID),
					undPriceProfileID = isnull(@iUndPriceProfileID, undPriceProfileID),
					optPriceProfileID = isnull(@iOptPriceProfileID, optPriceProfileID)
				where contractID = @iIndexID
			if (@@error <> 0)
				 begin
					RAISERROR ('Error. Can''t update Contract!', 16, 1)
					ROLLBACK TRAN
					RETURN (-1)
				 end
		  update [Index]
			   set	yield = isnull(@fYield,yield),
					isHTB = isnull(@bIsHTB,isHTB),
					calcOptionType = isnull(@tiCalcOptionType, calcOptionType),
					calcModelType = isnull(@tiCalcModelType, calcModelType),
					isBasket = isnull(@tiIsBasket,isBasket),
					isActive = isnull(@tiIsActive, isActive),
					skew = isnull(@fSkew,Skew),
					kurt = isnull(@fKurt,Kurt)					
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

ALTER  PROCEDURE dbo.usp_Stock_Save
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
		@tiIsActive tinyint = null,
		@fSkew float = null,
		@fKurt float = null,
		@tiCheckExistance tinyint = 1,
		@iUndPriceProfileID int = null output,
		@iOptPriceProfileID int = null output
  as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @bIsHTB is null 
		set @bIsHTB=0

	--if @iExpCalendarID is null or @iExpCalendarID = 0
	--	set @iExpCalendarID = 1

	if len(ltrim(@vcSymbol)) = 0
		begin
			Raiserror ('Empty stock ticker!', 16, 1)
			Return (-1)
		end   
   
	if exists (select * from Contract where symbol = @vcSymbol and ContractTypeID in (1, 2) and actionID < 3 and (@iStockID <> contractID or @iStockID is null))
		begin
			if isnull(@tiCheckExistance, 1) <> 0
				Raiserror ('Stock or index with this ticker already exists!', 16, 1)
			Return (-1)
		end   
   if (@iStockID is Null)  
     begin
         begin tran
			if @iUndPriceProfileID is null
			begin
				select
					@iUndPriceProfileID = priceProfileID
				from
					PriceProfileView
				where
					defaultID = 1
					and isOptionProfile = 0
			end

			if @iOptPriceProfileID is null
			begin
				select
					@iOptPriceProfileID = priceProfileID
				from
					PriceProfileView
				where
					defaultID = 1
					and isOptionProfile = 1
			end

            insert into Contract (contractTypeID, Symbol, contractName/*, expCalendarID*/, undPriceProfileID, optPriceProfileID)
                  values (2, @vcSymbol, @vcContractName/*, null*//*@iExpCalendarID*/, @iUndPriceProfileID, @iOptPriceProfileID)
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
				isActive,
				skew,
				kurt)
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
				@tiIsActive,
				@fSkew,
				@fKurt)
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
					--expCalendarID = null --isNull(@iExpCalendarID,expCalendarID),
					undPriceProfileID = isnull(@iUndPriceProfileID, undPriceProfileID),
					optPriceProfileID = isnull(@iOptPriceProfileID, optPriceProfileID)
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
						isActive = isnull(@tiIsActive, isActive),
						skew = isNull(@fSkew,skew),
						kurt = isNull(@fKurt,kurt) 
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


ALTER  PROC dbo.usp_BoFutureOption_Get
	@iFutureID int = null
AS
	-- get future options

	set nocount on
	
	if @iFutureID is not null
		select
			fo.futureOptionID as iFutureOptionID,
			foc.contractTypeID as iFutureOptionContractTypeID,
			foc.symbol as vcFutureOptionSymbol,
			foc.contractName as vcFutureOptionName,
			f.futureID as iFutureID,
			fc.contractTypeID as iFutureContractTypeID,
			fc.symbol as vcFutureSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			--fo.expiryID as iExpiryID, 
			fo.expiryDate as dtExpiry,
			fo.strike as fStrike, 
			fo.isCall as tiIsCall,
			fo.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureOption fo on f.futureID = fo.futureID
			inner join ContractView foc on fo.futureOptionID = foc.contractID
			--inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID
		where
			f.futureID = @iFutureID
	else
		select
			fo.futureOptionID as iFutureOptionID,
			foc.contractTypeID as iFutureOptionContractTypeID,
			foc.symbol as vcFutureOptionSymbol,
			foc.contractName as vcFutureOptionName,
			f.futureID as iFutureID,
			fc.contractTypeID as iFutureContractTypeID,
			fc.symbol as vcFutureSymbol,
			f.futureRootID as iFutureRootID,
			fr.futureRootSymbol as vcFutureRootSymbol,
			uc.contractID as iUnderlyingID,
			uc.symbol as vcUnderlyingSymbol,
			--fo.expiryID as iExpiryID, 
			fo.expiryDate as dtExpiry,
			fo.strike as fStrike, 
			fo.isCall as tiIsCall,
			fo.actionDate as dtActionDate
		from Future f
			inner join ContractView fc on f.futureID = fc.contractID
			inner join FutureOption fo on f.futureID = fo.futureID
			inner join ContractView foc on fo.futureOptionID = foc.contractID
			--inner join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			inner join FutureRoot fr on f.futureRootID = fr.futureRootID
			inner join ContractView uc on fr.underlyingID = uc.contractID

GO


ALTER   PROC usp_BoFutureRoot_Save
	@iFutureRootID int out,
	@iUnderlyingID int, -- should be specified for new future root (ignored for update)
	@vcFutureRootSymbol varchar(20) = null,
	@vcFutureRootName varchar(255) = null,
	@iFutureLotSize int = null,
	@iOptionLotSize int = null,
	@vcExportSymbol varchar(20) = null,
	@iMatCalendarID int = null,
	@tiIsRaiseError tinyint = 1,
	@tiQuoteFormat  tinyint = null,
	@iQuotationUnitAmmount int = null,
	@iQuotationUnitID int = null,
	@fQuoteDenomination float = null,
	@fTickSize float = null,
	@fTickValue float = null

AS
	-- create/update future root

	set nocount on

	--declare @iExpCalendarID int
	declare @FutureRootSymbol as varchar(20)	
	declare @error int		set @error = 0

	if @iFutureRootID is null -- new future root
	begin
		-- check underlying ID
		if @iUnderlyingID is null
		begin
			if @tiIsRaiseError = 1 raiserror('Underlying for new future root is not specified!', 16, 1)
			set @error = -1
			goto finish
		end

		-- check future root symbol
		set @vcFutureRootSymbol = ltrim(isnull(@vcFutureRootSymbol, ''))
		if len(@vcFutureRootSymbol) = 0
		begin
			if @tiIsRaiseError = 1 raiserror('Symbol for new future root is not specified!', 16, 1)
			set @error = -2
			goto finish
		end

		-- check future root
		if exists(select 1 from FutureRoot where futureRootSymbol = @vcFutureRootSymbol)
		begin
			if @tiIsRaiseError = 1 raiserror('Future root with symbol ''%s'' is already exists!', 16, 1, @vcFutureRootSymbol)
			set @error = -3
			goto finish
		end

		set @vcExportSymbol = ltrim(isnull(@vcExportSymbol, ''))
		if len(@vcExportSymbol) > 0
		begin
			-- check export symbol
			if exists(select 1 from FutureRoot
					where exportSymbol = @vcExportSymbol)
			begin
				if @tiIsRaiseError = 1 raiserror('Future root with import ID ''%s'' is already exists!', 16, 1, @vcExportSymbol)
				set @error = -104
				goto finish
			end
		end

		-- correct lot sizes
		if isnull(@iFutureLotSize, 0) <= 0
			set @iFutureLotSize = 100

		if isnull(@iOptionLotSize, 0) <= 0
			set @iOptionLotSize = 1

		-- correct maturity calendar ID
		--if isnull(@iMatCalendarID, 0) = 0
		--	set @iMatCalendarID = 1

		-- check maturity calendar ID
		--if not exists(select 1 from ExpiryCalendar where expCalendarID = @iMatCalendarID)
		--begin
		--	if @tiIsRaiseError = 1 raiserror('Invalid future root maturity calendar specified!', 16, 1)
		--	set @error = -4
		--	goto finish
		--end

		begin tran

		-- insert new future root
		insert into FutureRoot
			(underlyingID, futureRootSymbol, futureRootName,
			futureLotSize, optionLotSize, exportSymbol, matCalendarID, tiQuoteFormat,
			iQuotationUnitAmmount,
			iQuotationUnitID,
			fQuoteDenomination,
			fTickSize,
			fTickValue
)
		values
			(@iUnderlyingID, @vcFutureRootSymbol, @vcFutureRootName,
			@iFutureLotSize, @iOptionLotSize, @vcExportSymbol, 1/*@iMatCalendarID*/, @tiQuoteFormat,
			@iQuotationUnitAmmount,
			@iQuotationUnitID,
			@fQuoteDenomination,
			@fTickSize,
			@fTickValue
)
		if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

		set @iFutureRootID = SCOPE_IDENTITY()

                /* begin fixing FT8413 */
		--insert into ExpiryCalendar (expCalendarName, expCalendarDesc, bBelongsToFutureRoot) 
        --          values(@vcFutureRootSymbol+'_Fut', @vcFutureRootName+' Fut', 1)		
		--insert into ExpiryCalendar (expCalendarName, expCalendarDesc, bBelongsToFutureRoot) 
        --          values(@vcFutureRootSymbol+'_Opt', @vcFutureRootName+' Opt', 1)
                /* end fixing FT8413 */

		--set @iExpCalendarID = SCOPE_IDENTITY()
		--update FutureRoot set MatCalendarID = @iExpCalendarID where FutureRootID = @iFutureRootID

		commit tran
		return @iFutureRootID
	end
	else
	begin

		-- correct future root name (prevent updates to empty string)
		if len(ltrim(@vcFutureRootSymbol)) = 0
			set @vcFutureRootSymbol = null

		-- check future root
		if @vcFutureRootSymbol is not null
		begin
			if exists(select 1 from FutureRoot where futureRootID <> @iFutureRootID and futureRootSymbol = @vcFutureRootSymbol)
			begin
				if @tiIsRaiseError = 1 raiserror('Future root with symbol ''%s'' is already exists!', 16, 1, @vcFutureRootSymbol)
				set @error = -6
				goto finish
			end
		end

		-- check future root export symbol
		if @vcExportSymbol is not null
		begin
			set @vcExportSymbol = ltrim(@vcExportSymbol)
			
			if len(@vcExportSymbol) > 0 -- export symbol update
			begin
				-- check export symbol
				if exists(select 1 from FutureRoot
						where futureRootID <> @iFutureRootID and exportSymbol = @vcExportSymbol)
				begin
					if @tiIsRaiseError = 1 raiserror('Future root with import ID ''%s'' is already exists!', 16, 1, @vcExportSymbol)
					set @error = -104
					goto finish
				end
			end
		end
		
		-- correct future lot size (prevent updates to invalid lot size)
		if @iFutureLotSize <= 0
			set @iFutureLotSize = null

		-- correct option lot size (prevent updates to invalid lot size)
		if @iOptionLotSize <= 0
			set @iOptionLotSize = null

		-- correct maturity calendar ID (prevent updates to zero calendar)
		--if @iMatCalendarID = 0
		--	set @iMatCalendarID = null

		-- check maturity calendar
		--if @iMatCalendarID is not null
		--begin
		--	if not exists(select 1 from ExpiryCalendar where expCalendarID = @iMatCalendarID)
		--	begin
		--		if @tiIsRaiseError = 1 raiserror('Invalid future root maturity calendar specified!', 16, 1)
		--		set @error = -7
		--		goto finish
		--	end
		--end

		begin tran

		--select @FutureRootSymbol = FutureRootSymbol from FutureRoot where futureRootID = @iFutureRootID
		
		--update ExpiryCalendar
		--set expCalendarName = @vcFutureRootSymbol+'_Fut', 
		--      expCalendarDesc = @vcFutureRootName+' Fut'
		--where expCalendarID in (select expCalendarID from ExpiryCalendar where expCalendarName = @FutureRootSymbol+'_Fut')
		
		--update ExpiryCalendar
		--set expCalendarName = @vcFutureRootSymbol+'_Opt', 
		--      expCalendarDesc = @vcFutureRootName+' Opt'
		--where expCalendarID in (select expCalendarID from ExpiryCalendar where expCalendarName = @FutureRootSymbol+'_Opt')

		--update Contract set expCalendarID = isnull(@iMatCalendarID, expCalendarID) where contractID in 
		--	(select fo.futureOptionID from FutureOption fo 
		--		inner join Future f on f.futureID = fo.futureID
		--		where f.futureRootID = @iFutureRootID)

                /*begin FT_CQ00007114*/
       --         update Contract set expCalendarID = isnull(@iMatCalendarID, expCalendarID) where contractID in
       --                 (select futureID from Future f
       --                         inner join FutureRoot fr on f.futureRootID = fr.futureRootID
       --                         where f.futureRootID = @iFutureRootID)
                /*end FT_CQ00007114*/

		-- update future root
		update FutureRoot
		set futureRootSymbol = isnull(@vcFutureRootSymbol, futureRootSymbol),
			futureRootName = isnull(@vcFutureRootName, futureRootName),
			futureLotSize = isnull(@iFutureLotSize, futureLotSize),
			optionLotSize = isnull(@iOptionLotSize, optionLotSize),
			exportSymbol = isnull(@vcExportSymbol, exportSymbol),
			matCalendarID = 1,--isnull(@iMatCalendarID, matCalendarID),
			tiQuoteFormat = isnull(@tiQuoteFormat, tiQuoteFormat),
			iQuotationUnitAmmount = isnull(@iQuotationUnitAmmount, iQuotationUnitAmmount),
			iQuotationUnitID = isnull(@iQuotationUnitID, iQuotationUnitID),
			fQuoteDenomination = isnull(@fQuoteDenomination, fQuoteDenomination),
			fTickSize = isnull(@fTickSize, fTickSize),
			fTickValue = isnull(@fTickValue, fTickValue)

		where
			futureRootID = @iFutureRootID
		if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end		

		commit tran
	end

finish:
	return @error

GO

GO

ALTER  PROC usp_BoFutureRoot_Del
	@iFutureRootID int,
	@tiIsRaiseError tinyint = 1 
AS
	-- delete future root (with all futures and futures options)

	set nocount on
	
	--declare @expCalendarID_Opt int
	--declare @expCalendarID_Fut int
	declare @error int		set @error = 0

	-- check future root ID
	if @iFutureRootID is null
	begin
		if @tiIsRaiseError = 1 raiserror('Future root to delete is not specified!', 16, 1)
		set @error = -1
		goto finish
	end

        /*begin FT_CQ00008412*/
        declare @futureRootSymbol varchar(20)

        set @futureRootSymbol = (select futureRootSymbol from FutureRoot where futureRootID = @iFutureRootID)

        --if exists 
        --  (select 1 from FutureRoot where 
        --    futureRootID <> @iFutureRootID
        --    and
        --    ( (matCalendarID in 
        --        (select expCalendarID from ExpiryCalendar where expCalendarName = @futureRootSymbol + '_Opt'))
        --      or
        --      (matCalendarID in 
        --        (select expCalendarID from ExpiryCalendar where expCalendarName = @futureRootSymbol + '_Fut')) ))
        --begin
        --  raiserror('Can''t delete future root because some other future root points to it''s default calendar', 16, 1)
		--set @error = -4
		--goto finish
        --end        
        /*end   FT_CQ00008412*/

	-- create temporary table with contracts to delete
	create table #FutureRootsContractsForDelete(contractID int, contractTypeID int)
	
	insert into #FutureRootsContractsForDelete
		select futureID, 4 from Future
			where futureRootID = @iFutureRootID
		union select fo.futureOptionID, 5 from FutureOption fo
			inner join Future f on fo.futureID = f.futureID
			where f.futureRootID = @iFutureRootID

	-- check for trades existence
	if exists(select 1 from Trade where contractID in (select contractID from #FutureRootsContractsForDelete)
										or futureID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 4))
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future root, because its futures or its future options has trades or position!', 16, 1)
		set @error = -2
		goto finish
	end

	-- check for orders existence
	if exists(select 1 from [Order] where contractID in (select contractID from #FutureRootsContractsForDelete))
	begin
		if @tiIsRaiseError = 1 raiserror('Can''t remove future root, because its futures or its future options has orders!', 16, 1)
		set @error = -3
		goto finish
	end

	begin tran

	--select @expCalendarID_Opt = e.expCalendarID from FutureRoot fr inner join ExpiryCalendar e on e.expCalendarName = (fr.FutureRootSymbol + '_Fut') where fr.futureRootID = @iFutureRootID
	--select @expCalendarID_Fut = e.expCalendarID from FutureRoot fr inner join ExpiryCalendar e on e.expCalendarName = (fr.FutureRootSymbol + '_Opt') where fr.futureRootID = @iFutureRootID

	-- delete future options data
	delete FutureOption 
	where futureOptionID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 5)
	if @@error <> 0 begin set @error = -4 if @@trancount = 1 rollback tran goto finish end

	-- delete future data
	delete Future
	where futureRootID = @iFutureRootID
	if @@error <> 0 begin set @error = -5 if @@trancount = 1 rollback tran goto finish end

	-- delete future root
	delete FutureRoot
	where futureRootID = @iFutureRootID
	if @@error <> 0 begin set @error = -6 if @@trancount = 1 rollback tran goto finish end

	--delete from ExpiryInCalendar where expCalendarID = @expCalendarID_Opt
	--delete from ExpiryCalendar where expCalendarID = @expCalendarID_Opt

	--delete from ExpiryInCalendar where expCalendarID = @expCalendarID_Fut
	--delete from ExpiryCalendar where expCalendarID = @expCalendarID_Fut

	-- delete common future and future options contracts data
	delete ContractPrice 
	where contractID in (select contractID from #FutureRootsContractsForDelete)
	if @@error <> 0 begin set @error = -7 if @@trancount = 1 rollback tran goto finish end	

	update ContractExchangeLink set actionID = 3
	where contractID in (select contractID from #FutureRootsContractsForDelete where contractTypeID = 4)
	if @@error <> 0 begin set @error = -8 if @@trancount = 1 rollback tran goto finish end

	update Contract set actionID = 3
	where contractID in (select contractID from #FutureRootsContractsForDelete)
	if @@error <> 0 begin set @error = -9 if @@trancount = 1 rollback tran goto finish end	

	commit tran

finish:
	return @error

GO



ALTER  PROC usp_BoIndex_Del
	@iIndexID int
as
/*
	Description: this procedure is mark as delete information in Index table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	--declare @expCalendarID int
	declare @error int		set @error = 0
	
	if exists (select 1 from Trade where contractID = @iIndexID)
	   return -1

	if exists (select 1 from [Order] where contractID = @iIndexID)
	   return -2

	Create table #Options_for_delete(OptionID int)

	--select @expCalendarID = e.expCalendarID from ContractView cv inner join ExpiryCalendar e on e.expCalendarName = cv.symbol where cv.ContractID = @iIndexID and cv.contractTypeID = 1

	insert into #Options_for_delete
		select optionID from [Option] o(nolock)
				join OptionRoot ort on o.optionRootID = ort.optionRootID
 		where underlyingID = @iIndexID

	if exists (select 1 from Trade where contractID in (select optionID from #Options_for_delete))
		return -3

	if exists (select 1 from [Order] where contractID in (select optionID from #Options_for_delete))
		return -4

	Begin Transaction IndexDel

		exec @error = usp_TradeLog_Del @iIndexID
			if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del @bisRemovePermanent = 0
			end

		exec @error = usp_BoFutureRootByUnderlying_Del @iIndexID, 0
			if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		delete from IndexCorrelation where indexID1 = @iIndexID
			if @@error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		delete from IndexCorrelation where indexID2 = @iIndexID
			if @@error != 0 begin set @error = -6 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		delete from IndexDataHistory where indexID = @iIndexID
			if @@error != 0 begin set @error = -7 if @@TranCount != 0 Rollback Tran IndexDel goto finish end							
		
		--delete from ExpiryInCalendar where expCalendarID = @expCalendarID  					
		--delete from ExpiryCalendar where expCalendarID = @expCalendarID

		update IndexBeta set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -8 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update IndexDefinition set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -9 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update ContractInGroup set actionID=3 where contractID = @iIndexID 
			if @@error != 0 begin set @error = -10 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update [Index] set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -11 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update Contract set actionID=3 
			where contractID in 
				(select OptionID 
					from [Option] O 
							join OptionRoot ORt on O.optionRootID=ORt.optionRootID and ORt.underlyingID=@iIndexID)
			if @@error != 0 begin set @error = -12 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticUnderlyingParam set actionID=3 where underlyingID = @iIndexID or syntheticOptRootID in 
				(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -13 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where syntheticOptRootID in 
				(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -14 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where indexID = @iIndexID
			if @@error != 0 begin set @error = -15 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticRootIndexBeta set actionID=3 where syntheticOptRootID in 
			(select S.syntheticOptRootID from OptionRoot O join SyntheticOptionRoot S on O.optionRootID = S.optionRootID where O.underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -16 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update SyntheticOptionRoot set actionID=3 where optionRootID in (select optionRootID from OptionRoot where underlyingID = @iIndexID)
			if @@error != 0 begin set @error = -17 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update OptionRoot set actionID = 3 where underlyingID = @iIndexID
			if @@error != 0 begin set @error = -18 if @@TranCount != 0 Rollback Tran IndexDel goto finish end

		update TraderContract set actionID=3 where contractID = @iIndexID  
			if @@error <> 0 begin set @error = -20 if @@TranCount != 0 Rollback Transaction IndexDel goto finish end

		update Contract set actionID=3 where contractID = @iIndexID
			if @@error != 0 begin set @error = -21 if @@TranCount != 0 Rollback Tran IndexDel goto finish end		
		
		set @error = @@error
	Commit Transaction IndexDel
finish:
	return @error

GO



ALTER  PROCEDURE dbo.usp_Index_Get
	@iGroupID int = Null,
	@iIndexID int = Null,
	@tiWithPosition tinyint = 0,
	@iTraderID int = null
  as
	if isnull(@tiWithPosition, 0) = 0

	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView C 	
					inner join IndexView I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView C 	
					inner join IndexView I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where C.contractTypeID=1
					and tc.traderID = @iTraderID
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
				order by C.symbol
		end
		else
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join IndexView I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join IndexView I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and tc.traderID = @iTraderID
				order by C.symbol
		end
	end
	else
	begin
		if (@iGroupID is Null)
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 	
					inner join IndexView I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where C.contractTypeID=1
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
				group by C.contractID, C.contractTypeID, 
						C.symbol, C.contractName, 
						I.yield, I.isHTB, I.actionDate, 
						I.actionID,	I.CalcOptionType,
						I.CalcModelType, c.expCalendarID, I.isBasket, 
						I.isActive,	I.skew,	I.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 	
					inner join IndexView I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where C.contractTypeID=1
					and tc.traderID = @iTraderID
					and ((C.contractID = @iIndexID and @iIndexID is not Null) or (@iIndexID is Null))
				group by C.contractID,
						C.contractTypeID, C.symbol, C.contractName,
						I.yield, I.isHTB, I.actionDate, 
						I.actionID,I.CalcOptionType,
						I.CalcModelType,c.expCalendarID,
						I.isBasket, I.isActive,	I.skew,	I.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by C.symbol
		end
		else
		begin
			if @iTraderID is null
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,
					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join IndexView I on I.indexID = C.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
					I.yield, I.isHTB, I.actionDate, I.actionID,I.CalcOptionType,
					I.CalcModelType,c.expCalendarID,I.isBasket, I.isActive,	I.skew, I.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by C.symbol
			else
				select C.contractID as iContractID,
					C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol,
					C.contractName as vcContractName,
					I.yield as fYield,

					I.isHTB as bIsHTB,
					I.actionDate as dtActionDate,
					I.actionID as tiActionID,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					I.CalcOptionType as tiCalcOptionType,
					I.CalcModelType as tiCalcModelType,
					--C.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					I.isBasket as tiIsBasket,
					I.isActive as tiIsActive,
					I.skew as fSkew,
					I.kurt as fKurt,
					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractInGroupView cig
					inner join ContractView C  on C.contractID = CIG.contractID
					inner join IndexView I on I.indexID = C.contractID
				    inner join TraderContractView tc on c.contractID = tc.contractID
					join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where CIG.groupID = @iGroupID
					and C.contractTypeID=1
					and tc.traderID = @iTraderID
				group by C.contractID, C.contractTypeID, C.symbol, C.contractName,
					I.yield, I.isHTB, I.actionDate, I.actionID,I.CalcOptionType,
					I.CalcModelType,c.expCalendarID,I.isBasket, I.isActive,
					I.skew,I.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by C.symbol
		end
	end
if (@@error <> 0)
	Return (-1)
else  
	Return (0)



GO

ALTER   PROC usp_BoCommodity_Del 
	@iCID int,
	@tiIsRaiseError tinyint = 1 
as
/*
	Description: Commodity Delete procedure
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = 0
	
	if exists (select 1 from Trade where contractID = @iCID)
	   return -1

	if exists (select 1 from [Order] where contractID = @iCID)
	   return -2


	
        /*begin fixing FT8413*/
        create table #Deleted_FutureRootSymbols
                  (futureRootSymbol varchar(20)/*, 
                  expCalendarName_Fut varchar(50),
                  expCalendarName_Opt varchar(50)*/)

       --create table #ExpiryCalendar_ForDelete(expCalendarID int)

       --insert into #Deleted_FutureRootSymbols (futureRootSymbol)
                  (select futureRootSymbol from FutureRoot where underlyingID = @iCID)

       --update #Deleted_FutureRootSymbols set expCalendarName_Fut = futureRootSymbol + '_Fut'
       --update #Deleted_FutureRootSymbols set expCalendarName_Opt = futureRootSymbol + '_Opt'                

       --insert into #ExpiryCalendar_ForDelete
       --           select expCalendarID from ExpiryCalendar
       --             where expCalendarName in 
       --               (select expCalendarName_Fut from #Deleted_FutureRootSymbols)
       --insert into #ExpiryCalendar_ForDelete
       --           select expCalendarID from ExpiryCalendar
       --             where expCalendarName in 
       --               (select expCalendarName_Opt from #Deleted_FutureRootSymbols)

       /*begin FTCQ00008412*/
       /*
       if exists (select 1 from FutureRoot where 
                  underlyingID <> @iCID
                  and
                  matCalendarID in (select expCalendarID from #ExpiryCalendar_ForDelete))
                begin 
                   raiserror('Can''t delete future root because some other future root points to it''s default calendar', 16, 1)
                   return -3
                end*/
                /*end   FTCQ00008412*/
                /* ?? ?????? */
                /*end fixing FT8413*/
	Begin Transaction CommodityDel
		exec @error = usp_TradeLog_Del @iCID
			if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end


		exec @error = usp_BoFutureRootByUnderlying_Del @iCID, 0
			if @@error != 0 or @error != 0 begin set @error = -5 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end


		update IndexBeta set actionID=3 where indexID = @iCID
			if @@error != 0 begin set @error = -8 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end


		update ContractInGroup set actionID=3 where contractID = @iCID 
			if @@error != 0 begin set @error = -10 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end

		delete FutureUnderlying where
		iFutureUnderlyingID = @iCID
		if @@error <> 0 begin set @error = -3 if @@trancount = 1 rollback tran CommodityDel goto finish end

		update TraderContract set actionID=3 where contractID = @iCID  
			if @@error <> 0 begin set @error = -20 if @@TranCount != 0 Rollback Transaction CommodityDel goto finish end

		update Contract set actionID=3 where contractID = @iCID
			if @@error != 0 begin set @error = -21 if @@TranCount != 0 Rollback Tran CommodityDel goto finish end

                /*begin update fixing FT8413*/
                /*
                update ExpiryCalendar set actionID=3
                  where expCalendarID in 
                    (select expCalendarID from #ExpiryCalendar_ForDelete)
                    if @@error <> 0 begin set @error = -22 if @@trancount = 1 rollback tran CommodityDel goto finish end

                update ExpiryInCalendar set actionID=3
                  where expCalendarID in 
                  (select expCalendarID from #ExpiryCalendar_ForDelete)
                  if @@error <> 0 begin set @error = -23 if @@trancount = 1 rollback tran CommodityDel goto finish end
                  */
                /*end update fixing FT8413*/

		set @error = @@error
	Commit Transaction CommodityDel

finish:
	return @error

GO


ALTER  PROCEDURE dbo.usp_TradeByExpiredOptions_Del 
	@dtExpiry datetime 

AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Tub table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1


	declare @iTradeID int

	declare TradeByExpiredOptions_cursor cursor
	read_only
		for 
			Select distinct tradeID 
				from TradeView
				where contractID in (
						Select optionID 
						from OptionView O
							join OptionRoot ORt on O.optionRootID = ORt.optionRootID
							join Contract C on ORt.underlyingID = C.contractID
							--join ExpiryInCalendar EIC on O.expiryID = EIC.expiryID and EIC.expCalendarID = C.expCalendarID
						where datediff(d, O.expiryDate, @dtExpiry) > 0 )
						--where EIC.expiryDate < @dtExpiry
									

--			Select distinct tradeID 
--				from TradeView
--				where contractID in (
--						Select optionID 
--						from OptionView O
--							join Contract C on O.underlyingContractID = C.contractID
--							join ExpiryInCalendar EIC on O.expiryID = EIC.expiryID and EIC.expCalendarID = C.expCalendarID
--						where EIC.expiryDate < @dtExpiry)
	Begin Transaction

		open TradeByExpiredOptions_cursor
			fetch next from TradeByExpiredOptions_cursor into @iTradeID
				while (@@fetch_status <> -1)
					begin
						if (@@fetch_status <> -2)
					begin
						exec usp_Trade_Del @iTradeID
						--select @iTradeID
						if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Tran goto finish end
					end
					fetch next from TradeByExpiredOptions_cursor into @iTradeID
				end

		Close TradeByExpiredOptions_cursor
		Deallocate TradeByExpiredOptions_cursor

		set @error = @@error
	Commit Transaction

finish:
Return(@error)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



ALTER   PROCEDURE dbo.usp_Underlying_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0
AS
	set nocount on

	if isnull(@iGroupID, 0) <> 0
	begin
		exec usp_UnderlyingByGroup_Get @iContractID,@iTraderID,@iGroupID,@tiWithPosition	
		Return(0)
	end 
	if not @iContractID is null
	begin
		if isnull(@tiWithPosition, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
				    --c.expCalendarID as iExpCalendarID,
				    1 as iExpCalendarID,
				    s.calcOptionType as tiCalcOptionType,
				    s.isHTB as tiIsHTB,			
				    null as fYield,
				    null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
				    s.isActive as tiIsActive,

				    s.skew as fSkew,
				    s.kurt as fKurt,

				    c.undPriceProfileID as iUndPriceProfileID,
				    c.optPriceProfileID as iOptPriceProfileID
			   	    from ContractView c 
				    	inner join StockView s on c.contractID = s.stockID
				    	inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		  		    where
					c.contractTypeID = 2
					and c.contractID = @iContractID
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					--c.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
					i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
				
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
					i.isActive as tiIsActive,
					i.skew as fSkew,
					i.kurt as fKurt,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
					and c.contractID = @iContractID
				order by c.symbol	 
			else
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					--c.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
					null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
				    s.isActive as tiIsActive,

				    s.skew as fSkew,
				    s.kurt as fKurt,

				    c.undPriceProfileID as iUndPriceProfileID,
				    c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and  c.contractID = @iContractID
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					--c.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
			
				    i.yield as fYield,
 				    i.isBasket as tiIsBasket,
			 	    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
			
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
				    i.isActive as tiIsActive,
				    i.skew as fSkew,
				    i.kurt as fKurt,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
					and c.contractID = @iContractID
				order by c.symbol	 
		end
		else -- not isnull(@tiWithPosition, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					--c.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
			
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					s.isActive as tiIsActive,

					s.skew as fSkew,
					s.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 2
					and c.contractID = @iContractID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				    	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
					s.isActive, s.skew, s.kurt, c.undPriceProfileID, c.optPriceProfileID
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					--c.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
			
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
			
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

					i.isActive as tiIsActive,

					i.skew as fSkew,
					i.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 1
					and c.contractID = @iContractID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.symbol	 
			else
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					--c.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
			
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
	
					s.isActive as tiIsActive,

					s.skew as fSkew,
					s.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
				where 
					c.contractTypeID = 2
					and tc.traderID = @iTraderID
					and c.contractID = @iContractID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				   	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
					s.isActive, s.skew, s.kurt, c.undPriceProfileID, c.optPriceProfileID
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					--c.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
			
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
			
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

					i.isActive as tiIsActive,

					i.skew as fSkew,
					i.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
					and c.contractID = @iContractID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.symbol	 
		end
          end
	else
          begin
--------------------------------------------------
		if isnull(@tiWithPosition, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
				    --c.expCalendarID as iExpCalendarID,
				    1 as iExpCalendarID,
				    s.calcOptionType as tiCalcOptionType,
				    s.isHTB as tiIsHTB,			
				    null as fYield,
				    null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
				    s.isActive as tiIsActive,

				    s.skew as fSkew,
				    s.kurt as fKurt,

				    c.undPriceProfileID as iUndPriceProfileID,
				    c.optPriceProfileID as iOptPriceProfileID
			   	    from ContractView c 
				    	inner join StockView s on c.contractID = s.stockID
				    	inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		  		    where
					c.contractTypeID = 2
					and @iContractID is null 
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					--c.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
					i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
				
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
					i.isActive as tiIsActive,
					i.skew as fSkew,
					i.kurt as fKurt,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
				order by c.symbol	 
			else
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					--c.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
					null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
				    s.isActive as tiIsActive,

				    s.skew as fSkew,
				    s.kurt as fKurt,

				    c.undPriceProfileID as iUndPriceProfileID,
				    c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 2
					and tc.traderID = @iTraderID
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					--c.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
			
				    i.yield as fYield,
 				    i.isBasket as tiIsBasket,
			 	    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
			
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,
				    i.isActive as tiIsActive,
				    i.skew as fSkew,
				    i.kurt as fKurt,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID
				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
				order by c.symbol	 
		end
		else -- not isnull(@tiWithPosition, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					--c.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
			
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					s.isActive as tiIsActive,

					s.skew as fSkew,
					s.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 2
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				    	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
					s.isActive, s.skew, s.kurt, c.undPriceProfileID, c.optPriceProfileID
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					--c.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
			
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
			
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

					i.isActive as tiIsActive,

					i.skew as fSkew,
					i.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 1
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.symbol	 
			else
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					--c.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					s.calcOptionType as tiCalcOptionType,
					s.isHTB as tiIsHTB,
			
				    null as fYield,
					null as tiIsBasket,
				    s.divFreq as iDivFreq,
				    s.divDate as dtDivDate,
				    s.divAmt as fDivAmt,
			
				    s.isDivCustom as tiIsDivCustom,
				    s.divFreqCustom as iDivFreqCustom,
				    s.divDateCustom as dtDivDateCustom,
				    s.divAmtCustom as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
	
					s.isActive as tiIsActive,

					s.skew as fSkew,
					s.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join StockView s on c.contractID = s.stockID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
				where 
					c.contractTypeID = 2
					and tc.traderID = @iTraderID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
				   	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
					s.isActive, s.skew, s.kurt, c.undPriceProfileID, c.optPriceProfileID
				union
				select
				    c.contractID as iContractID,
				    c.contractTypeID as iContractTypeID,
				    c.Symbol as vcSymbol,
					--c.expCalendarID as iExpCalendarID,
					1 as iExpCalendarID,
					i.calcOptionType as tiCalcOptionType,
					i.isHTB as tiIsHTB,
			
				    i.yield as fYield,
					i.isBasket as tiIsBasket,
				    null as iDivFreq,
				    null as dtDivDate,
				    null as fDivAmt,
			
				    null as tiIsDivCustom,
				    null as iDivFreqCustom,
				    null as dtDivDateCustom,
				    null as fDivAmtCustom,
			
				    cp.priceClose as fPriceClose,

					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

					i.isActive as tiIsActive,

					i.skew as fSkew,
					i.kurt as fKurt,
					sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition,

					c.undPriceProfileID as iUndPriceProfileID,
					c.optPriceProfileID as iOptPriceProfileID

				from ContractView c 
				    inner join TraderContractView tc on c.contractID = tc.contractID
				    inner join IndexView i on c.contractID = i.indexID
				    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status,0)!= 1
				where
					c.contractTypeID = 1
					and tc.traderID = @iTraderID
				group by 
					c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, i.calcOptionType, i.isHTB, i.yield,
				    i.isBasket, cp.priceClose, i.isActive, i.skew, i.kurt, c.undPriceProfileID, c.optPriceProfileID
				order by c.symbol	 
		end

        end

	Return (0)

GO



ALTER  PROCEDURE dbo.usp_CustomVolatilityPointIsExist_Get
	@vcSymbolName varchar(20) = null
as
/*
	Created by Zaur Nuraliev
*/
	----------------------------------------------------------------------
	set nocount on
	----------------------------------------------------------------------
	if @vcSymbolName is null
		select distinct 
				CV.contractID as iContractID,
				CV.symbol as vcSymbolName,
				--EC.expiryDate as dtExpiry,
				OV.expiryDate as dtExpiry,
				case when CSSP.volaSurfaceDataID is null then 0  
				else 1 end 'CustomVolatilityPointsCount'
			from OptionView OV
				join optionRoot ort on ort.optionRootID = OV.optionRootID
				join ContractView CV on Ort.underlyingID = CV.contractID
				--join ExpiryInCalendarView EC on OV.expiryID = EC.expiryID and EC.expCalendarID = CV.expCalendarID
				left join VolaSurfaceDataView VSD on Ort.underlyingID = VSD.contractID and VSD.isDefaultSurface = 1
				left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
					and datediff(d, OV.expiryDate, CSSP.expiryDate) = 0
			where 
				CV.contractTypeID < 3
				and CSSP.volaSurfaceDataID is null
			order by CV.contractID, dtExpiry
	else
		select distinct 
				CV.contractID as iContractID,
				CV.symbol as vcSymbolName,
				--EC.expiryDate as dtExpiry,
				OV.expiryDate as dtExpiry,
				case when CSSP.volaSurfaceDataID is null then 0  
				else 1 end 'CustomVolatilityPointsCount'
			from OptionView OV
				join optionRoot ort on ort.optionRootID = OV.optionRootID
				join ContractView CV on Ort.underlyingID = CV.contractID
				--join ExpiryInCalendarView EC on OV.expiryID = EC.expiryID and EC.expCalendarID = CV.expCalendarID
				left join VolaSurfaceDataView VSD on Ort.underlyingID = VSD.contractID and VSD.isDefaultSurface = 1
				left join CustomStrikeSkewPointView CSSP on CSSP.volaSurfaceDataID = VSD.volaSurfaceDataID 
					and datediff(d, OV.expiryDate, CSSP.expiryDate) = 0
			where 
				CV.contractTypeID < 3
				and CV.symbol = @vcSymbolName
				and CSSP.volaSurfaceDataID is null
			order by CV.contractID, dtExpiry



GO


ALTER   PROCEDURE dbo.usp_SyntheticParams_Get 
	@iRootID int,
	@bIsWithPrice bit = 0,
	@bIsWithHV bit = 0
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is getting information from SyntheticUnderlyingParam table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if @bIsWithPrice = 0
		select 	
			sor.optionRootID as iRootID,
			sup.underlyingID as iUnderlyingID, 
			sup.SyntheticCoeff as fWeight,
			sup.strikeSettlement as fStrikeSettlement
		from SyntheticUnderlyingParamView as sup
			join SyntheticOptionRootView as sor on sor.syntheticOptRootID = sup.syntheticOptRootID
			join ContractView as c on c.contractID = sup.underlyingID
		where sor.optionRootID = @iRootID
	else
		begin
		
		if	@bIsWithHV = 0
			-- use NULL exchange only
			select 
				sor.optionRootID as iRootID,
				sup.underlyingID as iUnderlyingID, 
				sup.SyntheticCoeff as fWeight,
				sup.strikeSettlement as fStrikeSettlement,
				A.iContractID as iContractID,  
				A.iContractTypeID as iContractTypeID,
				A.vcSymbol as vcSymbol,
				case when isDivCustom = 1 then A.divFreqCustom else A.iDivFreq end iDivFreq,
				case when isDivCustom = 1 then A.divAmtCustom else A.fDivAmt end fDivAmt,
				case when isDivCustom = 1 then A.divDateCustom else A.dtDivDate end dtDivDate,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				A.tiCalcOptionType as tiCalcOptionType,
				A.tiCalcModelType as tiCalcModelType,
				--A.tiExpCalendarID as tiExpCalendarID,
				1 as tiExpCalendarID,
				A.tiIsBasket as tiIsBasket
			from SyntheticUnderlyingParamView sup 
				join vALLStockEOD A	on A.iContractID = sup.underlyingID
				join ContractView as c on c.contractID = sup.underlyingID
				join SyntheticOptionRootView as sor	on sor.syntheticOptRootID = sup.syntheticOptRootID
				join ContractPrice CP (nolock)
				on A.iContractID = CP.contractID where CP.exchangeID is null and sor.optionRootID = @iRootID
		else
			begin
			select 
				sor.optionRootID as iRootID,
				sup.underlyingID as iUnderlyingID, 
				sup.SyntheticCoeff as fWeight,
				sup.strikeSettlement as fStrikeSettlement,
				C.ContractID as iContractID,  
				C.ContractTypeID as iContractTypeID,
				C.Symbol as vcSymbol,
				case when isDivCustom = 1 then divFreqCustom else S.divfreq end iDivFreq,
				case when isDivCustom = 1 then divAmtCustom else S.divAmt end fDivAmt,
				case when isDivCustom = 1 then divDateCustom else S.DivDate end dtDivDate,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				S.CalcOptionType as tiCalcOptionType,
				S.CalcModelType as tiCalcModelType,
				--C.ExpCalendarID as tiExpCalendarID,
				1 as tiExpCalendarID,
				null as tiIsBasket,
				S2.currentHV as fCurrentHV
			from SyntheticUnderlyingParamView sup 
				join ContractView C on C.contractID = sup.underlyingID
				join StockView S on S.stockID = C.contractID
				join SyntheticOptionRootView as sor on sor.syntheticOptRootID = sup.syntheticOptRootID
				join ContractPrice CP (nolock) on S.stockID = CP.contractID 
				left join (select StockDataHistory.stockID,currentHV,[date] from StockDataHistory 
				join (select stockID, max([date]) as maxDate from StockDataHistory group by stockID) S1
					on S1.stockID=StockDataHistory.stockID and S1.maxDate=StockDataHistory.[date]) S2 
				on S2.stockID=S.stockID where CP.exchangeID is null and sor.optionRootID = @iRootID
			union
			select 
				sor.optionRootID as iRootID,
				sup.underlyingID as iUnderlyingID, 
				sup.SyntheticCoeff as fWeight,
				sup.strikeSettlement as fStrikeSettlement,
				C.ContractID as iContractID,  
				C.ContractTypeID as iContractTypeID,
				C.Symbol as vcSymbol,
				0 as iDivFreq,
				I.yield as fDivAmt,
				null as dtDivDate,
				CP.priceBid as fPriceBid,
				CP.priceAsk as fPriceAsk,
				CP.priceClose as fPriceClose,
				I.CalcOptionType as tiCalcOptionType,
				I.CalcModelType as tiCalcModelType,
				--C.ExpCalendarID as tiExpCalendarID,
				1 as tiExpCalendarID,
				null as tiIsBasket,
				S2.currentHV as fCurrentHV
			from SyntheticUnderlyingParamView sup 
				join ContractView C on C.contractID = sup.underlyingID
				join IndexView I on I.indexID = C.contractID
				join SyntheticOptionRootView as sor on sor.syntheticOptRootID = sup.syntheticOptRootID
				join ContractPrice CP (nolock) on I.indexID = CP.contractID
				left join (select IndexDataHistory.indexID,currentHV,[date] from IndexDataHistory join
					(select indexID, max([date]) as maxDate from IndexDataHistory group by indexID) S1
					on S1.indexID=IndexDataHistory.indexID and S1.maxDate=IndexDataHistory.[date]) S2 
				on S2.indexID=I.indexID where CP.exchangeID is null and sor.optionRootID = @iRootID 
			end
		end
	set @error = @@error
Return(@error)


GO


ALTER        PROCEDURE dbo.usp_OptionRootWithStrikes_Get 
	@iRootID int = null,
	@iUnderlyingID int = null
AS
/*
	Created by Mike Malyshkin
	Description: this procedure is getting information from OptionRoot and Option tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	declare @dtCurrentDate datetime set @dtCurrentDate =getdate()

	select distinct 
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcSymbol,
		ort.IsSynthetic as tiIsSynthetic,
		ort.lotSize as iLotSize,
		o.strike as fStrike,
		right(c.symbol,1) as vcStrikeTicker
		from OptionRootView As ort 
			join OptionView As o on o.optionRootID = ort.optionRootID 
			join ContractView As c on c.contractID = o.optionID	
			--join ContractView as c1 on ort.underlyingID = c1.contractID 
			--join ExpiryInCalendarView as ec on o.expiryID = ec.expiryID and ec.expCalendarID = c1.expCalendarID
		where	(ort.underlyingID = @iUnderlyingID or @iUnderlyingID is null)
			and (ort.optionRootID=@iRootID or @iRootID is null)
			and datediff(d, @dtCurrentDate, o.ExpiryDate) >= 0
	
	set @error = @@error
Return(@error)


GO

-------------------------------------------------------------------------------------------------------------------------------------------------
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO


ALTER  PROC dbo.usp_Trade_ClearDeleted 
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	Begin Transaction Trade

	delete from Trade where tradeID in (select tradeID from Trade where actionID = 0) 
		if @@error !=0 begin set @error = 2 if @@TranCount = 1 Rollback Tran goto finish end

	set @error = @@error

	delete Trade from Trade T join [Option] O on O.optionID = T.contractID
					join OptionRoot ORt on O.optionRootID = ORt.optionRootID
					join Contract UC on ORt.underlyingID = UC.contractID 
					--join ExpiryInCalendar E on O.expiryID = E.expiryID and UC.expCalendarID = E.expCalendarID
					where datediff(d, O.expiryDate, GetDate()) > 0
	if @@error !=0 begin set @error = 2 if @@TranCount = 1 Rollback Tran goto finish end

	set @error = @@error

	Commit Transaction Trade

finish:
	return @error


GO
ALTER      proc dbo.usp_BoOptionsForReconcile_Get
	@iUnderlyingID int		 
  as
/*
	Created by Michael Malyshkin
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	select	C.contractID as iContractID,
		C.contractTypeID as iContractTypeID,
		C.symbol as vcSymbol,
		ORt.underlyingID as iUnderlyingContractID,
		O.strike as fStrike, 
		--cast(O.expiryDate as int) as iExpiryID,
		O.expiryDate as dtExpiry,
		O.isCall as bIsCall,
		O.actionDate as dtActionDate,
		O.actionID as tiActionID,
		ORt.lotSize as iLotSize,
		ORt.optionRootID as iOptionRootID,
		ORt.symbol as vcOptionRootSymbol,
		O.isManualyEntered as tiIsManualyEntered,
		C.exportSymbol as vcExportSymbol
	from OptionView O  
		left join ContractView C on O.optionID = C.contractID
		left join ContractPrice CP on CP.contractID=C.contractID and CP.exchangeID is null
		left join OptionRoot ORt on O.optionRootID = ORt.optionRootID
		--left join Contract C1 on ORt.underlyingID = C1.contractID
		--left join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = isnull( C.expCalendarID, C1.expCalendarID )
	where c.contractTypeID=3
		and ORt.underlyingID = @iUnderlyingID

	set @error = @@error
Return(@error)

 
GO


ALTER  PROC dbo.usp_BoPositionForReconcile_Get
	@lastUTCTradeDate datetime = null
AS
	set nocount on

	if @lastUTCTradeDate is null set @lastUTCTradeDate = getdate()

	select 
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.Symbol as vcSymbol,
		cp.priceClose as fPriceClose,

		uc.contractID as iUnderlyingContractID,
		uc.contractTypeID as iUnderlyingContractTypeID,
		uc.Symbol as vcUnderlyingSymbol,

		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,

		case when c.contractTypeID < 3 then 1
			when c.contractTypeID = 3 then ort.lotSize
			when c.contractTypeID = 4 then fr.futureLotSize
			when c.contractTypeID = 5 then fr.optionLotSize
		end as iLotSize,

		case when c.contractTypeID = 3 then o.expiryDate else fo.expiryDate end as dtExpiryDate,
		case when c.contractTypeID = 3 then o.strike else fo.strike end as fStrike,
		case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiIsCall,

		trdr.TraderID as iTraderID,
		trdr.acronym as vcAcronym,
		sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,

		tr.strategyID as iStrategyID,
		st.strategyName as vcStrategyName,

		fr.futureRootID as iFutureRootID,
		fr.futureRootSymbol as vcFutureRootSymbol,
		fr.exportSymbol as vcFutureRootExportSymbol,
		f.futureID as iFutureID,
		fc.symbol as vcFutureSymbol,
		f.maturityDate as dtFutureMaturityDate, 
		fr.futureLotSize as iFutureLotSize,
		fr.optionLotSize as iFutureOptionLotSize
	from TradeView tr 
		-- contract and underlying contract
		inner join ContractView c on tr.contractID = c.contractID
		inner join ContractView uc on tr.underlyingID = uc.contractID
		left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		-- regular option
		left join OptionView o on c.contractID = o.optionID
		left join OptionRootView ort on tr.optionRootID = ort.optionRootID
		-- trader and strategy
		left join TraderView trdr on tr.traderID = trdr.traderID
		left join StrategyView st on st.strategyID = tr.strategyID
		-- futures
		left join ContractView fc on tr.futureID = fc.contractID
		left join FutureRoot fr on tr.futureRootID = fr.futureRootID
		left join Future f on tr.futureID = f.futureID
		left join FutureOption fo on tr.contractID = fo.futureOptionID
	where tr.status in (0, 3)
		and datediff(day, tr.tradeDate, @lastUTCTradeDate) >= 0
	group by c.contractID, c.contractTypeID, c.Symbol, ort.optionRootID, ort.symbol,
		o.expiryDate, o.strike, o.isCall, fo.expiryDate, fo.strike, fo.isCall, 
		uc.contractID, uc.contractTypeID, uc.Symbol,
		ort.lotSize, cp.priceClose, trdr.TraderID, trdr.acronym, tr.strategyID, st.strategyName,
		fr.futureRootID, f.futureID, f.maturityDate, fr.futureLotSize, fr.optionLotSize,
		fr.futureRootSymbol, fr.exportSymbol, fc.symbol
	order by vcSymbol

GO


ALTER  PROC usp_BoTrade_StoreToHistoryAndClear
	@tiStoreToHistory tinyint = null,
	@dtLastTradeDateUTC datetime = null,
	@iStoredTradesCount int = null out
AS
	declare @error int		set @error = 0
	
	if @dtLastTradeDateUTC is null	set @dtLastTradeDateUTC = '99991231' -- set to max available date to avoid 'is null' checking

	begin tran

	-- clear simulated and manual deleted trades
	delete from Trade 
		where tradeID in (select tradeID from Trade where isnull(status, 0) in (1, 2) and actionID = 0)
	if @@error !=0 begin set @error = -2 if @@TranCount = 1 Rollback Tran goto finish end

	-- clear actual and actual matched deleted trades
	delete from Trade 
		where tradeID in (select tradeID from Trade where isnull(status, 0) in (0, 3) and actionID = 0)
	if @@error !=0 begin set @error = -4 if @@TranCount = 1 Rollback Tran goto finish end

	if isnull(@tiStoreToHistory, 0) <> 0
	begin
		-- move actual and actual matched trades to history
		insert into TradeHistory
		select
			getdate() as dtActionDate,
	
			tr.tradeID as iTradeID, tr.tradeDate as dtTradeDate, tr.quantity as iQuantity,
			tr.price as fPrice, tr.isBuy as tiIsBuy, tr.status as tiStatus, tr.execID as vcExecID,
			tr.mark as iTradeMark,
			
			c.contractID as iContractID, c.contractTypeID as iContractTypeID, c.Symbol as vcSymbol,
			c.exportSymbol as vcExportSymbol, cp.priceClose as fPriceClose,
	
			uc.contractID as iUnderlyingContractID, uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol, uc.exportSymbol as vcUnderlyingExportSymbol,
			s.divFreq as iUnderlyingDivFreq, s.divDate as dtUnderlyingDivDate,
			s.divAmt as fUnderlyingDivAmt, i.yield as fUnderlyingYield,
			ucp.priceClose as fUnderlyingPriceClose,
	
			f.futureID as iFutureID, fr.futureRootSymbol as vcFutureRootSymbol,
			fc.symbol as vcFutureSymbol, fc.exportSymbol as vcFutureExportSymbol,
			f.maturityDate as dtFutureMaturity, fr.futureLotSize as iFutureLotSize,
			fcp.priceClose as fFuturePriceClose,
	
			ort.optionRootID as iOptionRootID, ort.symbol as vcOptionRootSymbol, ort.lotSize as iOptionRootLotSize,
	
			case when c.contractTypeID = 3 then o.expiryDate else f.maturityDate end as dtOptionExpiry,
			case when c.contractTypeID = 3 then o.strike else fo.strike end as fOptionStrike,
			case when c.contractTypeID = 3 then o.isCall else fo.isCall end as tiOptionIsCall,
			case when c.contractTypeID = 3 then cp.priceTheo else fcp.priceTheo end as fPriceTheoClose,
	
			tr.traderID as iTraderID, t.acronym as vcTraderAcronym, t.traderName as vcTraderName,
	
			st.strategyID as iStrategyID, st.strategyName as vcStrategyName,
	
			tr.brokerID as iBrokerID, br.brokerName as vcBrokerName, tr.brokerCommission as fBrokerCommission,
	
			tr.clearingBrokerID as iClearingBrokerID, cbr.brokerName as vcClearingBrokerName,
			tr.clearingBrokerCommission as fClearingBrokerCommission,
	
			tr.tradedIV as fTradedIV, tr.spotReference as fSpotReference,

			fr.futureRootID as iFutureRootID,
			fr.optionLotSize as iFutureOptionLotSize
	
		from TradeView tr
			-- contract and underlying contract
			inner join Contract c on tr.contractID = c.contractID
			inner join Contract uc on tr.underlyingID = uc.contractID
			left join ContractPrice cp on tr.contractID = cp.contractID and cp.exchangeID is null
			left join ContractPrice ucp on tr.underlyingID = ucp.contractID and ucp.exchangeID is null
			-- regular option
			left join OptionView o on tr.contractID = o.optionID
			left join OptionRoot ort on tr.optionRootID = ort.optionRootID
			--left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
			-- underlying
			left join StockView s on tr.underlyingID = s.stockID
			left join IndexView i on tr.underlyingID = i.indexID
			-- futures part
			left join ContractView fc on tr.futureID = fc.contractID
			left join FutureRoot fr on tr.futureRootID = fr.futureRootID
			left join Future f on tr.futureID = f.futureID
			--left join ExpiryInCalendarView fmc on f.maturityID = fmc.expiryID and fmc.expCalendarID = fr.matCalendarID
			left join ContractPrice fcp on tr.futureID = fcp.contractID and fcp.exchangeID is null
			left join FutureOption fo on tr.contractID = fo.futureOptionID
			--left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
			-- additional
			left join BrokerView br on tr.brokerID = br.brokerID
			left join BrokerView cbr on tr.clearingBrokerID = cbr.brokerID
			left join TraderView t on tr.traderID = t.traderID
			left join StrategyView st on tr.strategyID = st.strategyID
		where
			isnull(status, 0) in (0, 3)
			and isnull(tr.isPosition, 0) = 0
			and datediff(day, tr.tradeDate, @dtLastTradeDateUTC) >= 0
	
		set @iStoredTradesCount = @@rowcount

		if @@error !=0 begin set @error = -5 if @@TranCount = 1 Rollback Tran goto finish end
	end

	-- clear actual and actual matched trades
	delete from Trade 
		where isnull(status, 0) in (0, 3) and datediff(day, tradeDate, @dtLastTradeDateUTC) >= 0
	if @@error !=0 begin set @error = -7 if @@TranCount = 1 Rollback Tran goto finish end

	commit tran

finish:
	return @error

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.146')
	begin
		update DataInfo set keyValue = '4.49.146' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.146' '4.49.146 update message:' 
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