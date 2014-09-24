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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.46.207'  or (keyValue = '3.46.208') ))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.46.208 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.46.208' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
GO

if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Future' and COLUMN_NAME='bUseInCalc')
ALTER TABLE dbo.Future ADD
	bUseInCalc [tinyint] NULL
GO
if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Future' and COLUMN_NAME='dBasis')
ALTER TABLE dbo.Future ADD
	dBasis [float]  NULL
GO
if NOT Exists (select * from INFORMATION_SCHEMA.COLUMNS where TABLE_NAME='Future' and COLUMN_NAME='dActiveFutureRatio')
ALTER TABLE Future
	ADD dActiveFutureRatio float

GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ActiveFutures_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ActiveFutures_Get]
GO

CREATE   PROC dbo.usp_ActiveFutures_Get
	@vSymbol AS NVARCHAR(20)
AS
	SELECT FUT.Symbol            AS Symbol,
	       FUT.ContractTypeID    AS ContractType,
               FC.Symbol             AS ActiveSymbol,
	       0		     AS Basis,
	       case isnull(FB.dActiveFutureRatio,0)
			WHEN 0 THEN 
			 CASE WHEN isnull(FBP.PriceClose,0) > 0 AND isnull(FP.PriceClose,0) >0 THEN FBP.PriceClose/FP.PriceClose
			 ELSE 1 END
			ELSE FB.dActiveFutureRatio
               END AS Ratio
	FROM [ContractView] FUT
	INNER join Future FB on FB.futureID = FUT.ContractId
	INNER join futureRoot FR on FR.FutureRootID = FB.FutureRootID 
	LEFT join Future F on F.bUseInCalc = 1 and FR.futureRootID = F.futureRootID
	LEFT join Contract FC on F.FutureId = FC.ContractID
	LEFT JOIN ContractPrice FBP on FBP.contractID = FB.futureID
	LEFT JOIN ContractPrice FP on FP.contractID = F.futureID
	WHERE FUT.symbol = @vSymbol and FUT.ContractTypeID = 4 and FC.Symbol is not null
UNION
	SELECT 
		IDX.Symbol		AS Symbol, 
		IDX.ContractTypeID	AS ContractType, 
		FC.Symbol 		AS ActiveSymbol,
		F.dBasis 		AS Basis,
		0			AS Ratio
	FROM [ContractView] IDX 
	LEFT join futureRoot FR on FR.underlyingID = IDX.ContractId 
	LEFT join Future F on F.bUseInCalc = 1 and FR.futureRootID = F.futureRootID
	LEFT join Contract FC on F.FutureId = FC.ContractID
	WHERE IDX.ContractTypeID = 1 and FC.Symbol is not null  and IDX.Symbol = @vSymbol

GO
ALTER     PROC usp_MmFuture_Get
	@iFutureRootID int = null
AS
	-- retrieve futures by future root
	set nocount on

	if @iFutureRootID is null
		select
			fc.contractID as iFutureID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcFutureSymbol,
			fc.contractName as vcFutureName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			fc.undPriceProfileID as iUndPriceProfileID,
			fc.optPriceProfileID as iOptPriceProfileID,
			f.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
		   	fcp.priceClose as fPriceClose,
			f.maturityDate as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			case when exists(select top 1 1 from FutureOption where futureID = fc.contractID) then 1 else 0 end as iHaveOptions,
			isnull(fu.tiGreeksCalculationType ,0) as iShowInFutureContractEq,
			isnull(f.dBasis, 0) as dBasis ,
			f.dActiveFutureRatio as dActiveRatio,
			isnull(f.bUseInCalc,0) as bUseInCalculation
		from Future f
			inner join ContractView fc on fc.contractID = f.futureID
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			inner join ContractView uc on uc.contractID = fr.underlyingID
		    inner join ContractPrice fcp on fc.contractID = fcp.contractID and fcp.exchangeID is null
			left outer join FutureUnderlying fu on fu.iFutureUnderlyingID = uc.contractID
	else
		select
			fc.contractID as iFutureID,
			fc.contractTypeID as iContractTypeID,
			fc.symbol as vcFutureSymbol,
			fc.contractName as vcFutureName,
			fc.expCalendarID as iExpCalendarID,
			fc.exportSymbol as vcExportSymbol,
			fc.undPriceProfileID as iUndPriceProfileID,
			fc.optPriceProfileID as iOptPriceProfileID,
			f.futureRootID as iFutureRootID,
			uc.contractID as iUnderlyingID,
			fcp.priceClose as fPriceClose,
			f.maturityDate  as dtMaturityDate,
			f.calcOptionType as tiCalcOptionType,
			f.isActive as tiIsActive,
			case when exists(select top 1 1 from FutureOption where futureID = fc.contractID) then 1 else 0 end as iHaveOptions,
			isnull(fu.tiGreeksCalculationType ,0) as iShowInFutureContractEq,
			isnull(f.dBasis, 0) as dBasis ,
			f.dActiveFutureRatio as dActiveRatio,
			isnull(f.bUseInCalc,0) as bUseInCalculation
		from Future f
			inner join ContractView fc on fc.contractID = f.futureID
			inner join FutureRoot fr on fr.futureRootID = f.futureRootID
			inner join ContractView uc on uc.contractID = fr.underlyingID
		    inner join ContractPrice fcp on fc.contractID = fcp.contractID and fcp.exchangeID is null
			left outer join FutureUnderlying fu on fu.iFutureUnderlyingID = uc.contractID
		where
			f.futureRootID = @iFutureRootID


GO
GO

ALTER    PROC dbo.usp_BoFuture_Save
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
	@tiIsRaiseError tinyint = 1 ,
	@dBasis float = null,
	@bUseInCalc tinyint = null,
	@dActiveFutureRatio float = null

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
			(futureID, futureRootID, maturityDate, calcOptionType, isActive, bUseInCalc, dBasis, dActiveFutureRatio)
		values
			(@iFutureID, @iFutureRootID, @dtMaturityDate, @tiCalcOptionType, @tiIsActive, @bUseInCalc, @dBasis, @dActiveFutureRatio)
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
			isActive = isnull(@tiIsActive, isActive),
			bUseInCalc = isnull(@bUseInCalc,bUseInCalc),
			dBasis = isnull(@dBasis,dBasis),
			dActiveFutureRatio = isnull(@dActiveFutureRatio,dActiveFutureRatio)
		where futureID = @iFutureID
		if @@error <> 0 begin set @error = -17 if @@trancount = 1 rollback tran goto finish end

		commit tran
	end
finish:
	return @error

GO

ALTER    PROC dbo.usp_MmFuture_Save
	@iFutureID int,
	@vcFutureName varchar(255) = null,
	@tiCalcOptionType tinyint = null,
	@tiIsActive tinyint = null,
	@iUndPriceProfileID int = null,
	@iOptPriceProfileID int = null,
	@bUseInCalc tinyint = null,
	@dBasis float = null,
	@dActiveFutureRatio float = null

AS
	-- update future

	set nocount on
	
	declare @error int		set @error = 0

	exec @error = usp_BoFuture_Save @iFutureID,
									null,
									null,
									@vcFutureName,
									null,
									--null,
									null,
									null,
									@tiCalcOptionType,
									@tiIsActive,
									@iUndPriceProfileID,
									@iOptPriceProfileID,
									1,
									@dBasis,
									@bUseInCalc,
									@dActiveFutureRatio


	if @@error <> 0 set @error = -20

	return @error


GO

ALTER       PROC usp_MmFutureByUnderlying_Get
	@iUnderlyingID int,
	@dtMinExpiry datetime = Null	
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------

	if @dtMinExpiry is null set @dtMinExpiry = 0	

	select
		f.futureID as iContractID,
		fc.contractTypeID as iContractTypeID,
		fc.symbol as vcSymbol,		
		--e.ExpiryPeriod as vcMaturity,
                f.maturityDate as dtMaturity,
		fu.ContractId as iFutureUnderlyingID,
                f.futureRootID as iFutureRootID,
		isnull(ff.tiGreeksCalculationType, 1) as tiGreeksCalculationType,
		cp.priceClose as fPriceClose,
		isnull(f.dBasis, 0) as dBasis ,
		f.dActiveFutureRatio as dActiveRatio,
		isnull(f.bUseInCalc,0) as bUseInCalculation
	from ContractView fu
                inner join FutureRoot fr on fr.underlyingID = fu.ContractId
                inner join Future f on f.futureRootID = fr.futureRootID
		inner join ContractView fc on f.futureID = fc.ContractId
		left join ContractPrice cp on f.futureID = cp.ContractID
		left join FutureUnderlying ff on ff.iFutureUnderlyingID = fu.ContractId
	where fu.ContractId = @iUnderlyingID
		and datediff(d, @dtMinExpiry, f.maturityDate) >=0

	return @@error




GO

GO
/*END DATA MODIFICATION SECTION*/
--=============================================================================================
--change version of DB
--=============================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.46.208')
	begin
		update DataInfo set keyValue = '3.46.208' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.46.208' '3.46.208 update message:' 
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