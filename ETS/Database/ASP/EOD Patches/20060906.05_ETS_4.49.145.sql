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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '4.49.144' or (keyValue = '4.49.145')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '4.49.145 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '4.49.144' as  'Version in script' from DataInfo where dataInfoID = 1
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
ALTER     PROCEDURE usp_EodOldFutures_Del
	@iFutureRootID INT
AS
	-------------------------------------------------
	SET NOCOUNT ON
	-------------------------------------------------
	DECLARE @error INT		SET @error = 0

	CREATE TABLE #FutureContractsForDelete(contractID INT) 

	INSERT INTO #FutureContractsForDelete
		SELECT F.futureID
			FROM 
				Future F
					JOIN FutureRoot FR ON FR.futureRootID = F.futureRootID
			WHERE 
				( 
					( 
						datediff(d, GetDate(), F.maturityDate)  < 0
						OR DATEDIFF(d, F.actionDate, GETDATE()) > 0 
					)
					AND F.futureRootID = @iFutureRootID
				)

	INSERT INTO #FutureContractsForDelete
		SELECT FO.futureOptionID
			FROM 
				FutureOption FO
					JOIN Future F ON F.futureID = FO.futureID
					JOIN FutureRoot FR ON FR.futureRootID = F.futureRootID
			WHERE 
				( 
					( 
						datediff(d, GetDate(), FO.expiryDate) < 0 
						OR DATEDIFF(d, F.actionDate, GETDATE()) > 0 
					)
					AND F.futureRootID = @iFutureRootID
				)

	EXEC @error = usp_EodFuture_Del
	-- leave this shit or change CEodStepLoadMktStructure::_FinishLoadRootFutures() in EodLoadMktStructure.cpp
	SELECT NULL AS vcSymbol FROM #FutureContractsForDelete WHERE contractID IS NULL

finish:
	RETURN(@error)

GO
GO
ALTER     PROCEDURE usp_EodOldFutureOptions_Del
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
						 datediff(d, GetDate(), FO.expiryDate) < 0 OR DATEDIFF(d, FO.actionDate, GETDATE()) > 0 
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
					   datediff(d, GetDate(), FO.expiryDate) < 0 or DATEDIFF(d, FO.actionDate, GETDATE()) > 0 
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



ALTER   PROC usp_FutureIC_Get
		@iICID int = null
	AS
		-- retrieve future
		set nocount on
		if @iICID is not null
		SELECT    dbo.Future.futureID, dbo.Contract.ContractID
		FROM      dbo.FutureRoot 
			INNER JOIN dbo.Future ON dbo.FutureRoot.futureRootID = dbo.Future.futureRootID 
			INNER JOIN dbo.Contract ON dbo.FutureRoot.underlyingID = dbo.Contract.contractID and dbo.Contract.ContractTypeID = 6
		WHERE     (dbo.Future.futureID = @iICID)

GO
GO

ALTER    PROC dbo.usp_MmContractForTradeByUnd_Get
	@iUnderlyingID int
as
-- changed by VK - flex option support added
	-- return 2 recordsets: 1st for option contracts, 2nd futures & futures options contracts
	set nocount on


	-- options rowset
	select	
		c.contractID as iContractID,
		c.contractTypeID as iContractTypeID,
		c.symbol as vcSymbol,
		c.contractName as vcContractName, 
		cp.priceClose as fPriceClose,
		cp.priceTheo as fPriceTheoClose,
		
		-- ec.expiryDate as dtExpiry,
		 o.expiryDate as dtExpiry,
		o.strike as fStrike,
		o.isCall as tiIsCall,
		ort.optionRootID as iOptionRootID
	from OptionRootView ort
		inner join OptionView o on o.optionRootID = ort.optionRootID
		inner join ContractView c on c.contractID = o.optionID
		inner join Contract uc on uc.contractID = ort.underlyingID
		-- inner join ExpiryInCalendarView ec on ec.expiryID = o.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
		left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
	where
		ort.underlyingID = @iUnderlyingID
		and datediff(d,getDate(), o.expiryDate) >= 0
	order by o.expiryDate, o.strike, o.isCall desc

	-- futures & futures options rowset
	select * from
	(
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName, 
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,
			
			null as dtExpiry,
			null as fStrike,
			null as tiIsCall,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			c.symbol as vcFutureSymbol
		from FutureRoot fr
			inner join Future f on f.futureRootID = fr.futureRootID and fr.underlyingID = @iUnderlyingID
			inner join ContractView c on c.contractID = f.futureID
			left join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
			
		union
		select
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.symbol as vcSymbol,
			c.contractName as vcContractName, 
			cp.priceClose as fPriceClose,
			cp.priceTheo as fPriceTheoClose,
			o.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as tiIsCall,

			fr.futureRootID as iFutureRootID,
			f.futureID as iFutureID,
			fc.symbol as vcFutureSymbol
		from FutureRoot fr
			inner join Future f on f.futureRootID = fr.futureRootID and fr.underlyingID = @iUnderlyingID
			inner join Contract fc on fc.contractID = f.futureID
			inner join FutureOption o on o.futureID = f.futureID
			inner join ContractView c on c.contractID = o.futureOptionID
			left join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where
			datediff(d,GetDate(),o.expiryDate) >= 0
	) as c order by c.vcFutureSymbol, c.dtExpiry, c.fStrike, c.tiIsCall desc

	return @@error
GO


ALTER    PROC dbo.usp_UnderlyingOptionsForOrder_Get
	@iUnderlyingID int
AS
-- changed by VK - flex option support added
	set nocount on

	select
		ort.optionRootID as iOptionRootID,
		o.optionID as iOptionID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		o.isCall as tiIsCall,
		--ec.expiryDate as dtExpiry,
		o.expiryDate as dtExpiry
	from OptionRootView ort
		left join OptionView o on ort.optionRootID = o.optionRootID
		left join ContractView c on c.contractID = o.optionID
		--left join Contract uc on ort.underlyingID = uc.contractID
		--left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = isnull( c.expCalendarID, uc.expCalendarID )
	where
		ort.underlyingID = @iUnderlyingID
		and c.contractTypeID = 3
		and datediff(d, GetDate(), o.expiryDate) >= 0
	order by ort.underlyingID, o.expiryDate, o.strike, o.isCall desc

Return (0)
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingsWithFlexOptions_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingsWithFlexOptions_Get]
GO
CREATE    PROC dbo.usp_UnderlyingsWithFlexOptions_Get
as
	select distinct uc.symbol, uc.contractID, cp.priceClose 
	from [option] o
		join OptionRoot ort on o.optionRootID = ort.optionRootID
		join Contract c on c.contractID = o.optionID
		join Contract uc on uc.contractID = ort.underlyingID
		join ContractPrice cp on uc.contractID = cp.contractID
	where o.isManualyEntered = 1 and datediff(d, GetDate(), expiryDate) >=0 

GO

ALTER     PROC dbo.usp_UnderlyingOptions_Get
	@iUnderlyingID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@tiWithPosition tinyint = 0,
	@dtMinExpiry datetime = Null
AS
-- changed by VK - flex option support added
	set nocount on
	
	if @dtMinExpiry is null set @dtMinExpiry = 0

	if isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				if isnull(@iUnderlyingID, 0) <> 0
					select	c.contractID as iContractID,
						c.symbol as vcSymbol,
						ort.underlyingID as iUnderlyingContractID,
						o.strike as fStrike, 
						o.expiryDate as dtExpiry,
						o.isCall as tiIsCall,
						ort.lotSize as iLotSize,
						ort.optionRootID as iOptionRootID
					from ContractView c  
						inner join [Option] o on o.optionID = c.contractID
						join OptionRoot ort on o.optionRootID = ort.optionRootID
						join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null
					where c.contractTypeID = 3
						and ort.underlyingID = @iUnderlyingID
						and datediff(d, @dtMinExpiry, o.expiryDate) >= 0
					order by ort.underlyingID, o.expiryDate, o.strike, o.isCall desc
				else
					select	c.contractID as iContractID,
						c.symbol as vcSymbol,
						ort.underlyingID as iUnderlyingContractID,
						o.strike as fStrike, 
						o.expiryDate as dtExpiry,
						o.isCall as tiIsCall,
						ort.lotSize as iLotSize,
						ort.optionRootID as iOptionRootID
					from ContractView c  
						inner join [Option] o on o.optionID = c.contractID
						join OptionRoot ort on o.optionRootID = ort.optionRootID
						join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					where c.contractTypeID = 3
						and datediff(d, @dtMinExpiry, o.expiryDate) >= 0
					order by ort.underlyingID, o.expiryDate, o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					o.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and datediff(d,@dtMinExpiry,o.expiryDate) >= 0
				order by ort.underlyingID, o.expiryDate, o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					o.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
							join OptionRoot ort on cig.contractID = ort.underlyingID
							inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and datediff(d,@dtMinExpiry,o.expiryDate) >= 0
				order by ort.underlyingID, o.expiryDate, o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					o.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
							join OptionRoot ort on cig.contractID = ort.underlyingID					
							join ContractView uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and datediff(d,@dtMinExpiry,o.expiryDate) >= 0
				order by ort.underlyingID, o.expiryDate,o.strike, o.isCall desc
		end
	end
	else -- not isnull(@tiWithPosition, 0) = 0
	begin
		if isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					o.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					inner join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and datediff(d,@dtMinExpiry,o.expiryDate) >= 0
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, o.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, o.expiryDate, o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					o.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractView c  
					join [Option] o on o.optionID = c.contractID
					join OptionRoot ort on o.optionRootID = ort.optionRootID
					inner join Contract uc on ort.underlyingID = uc.contractID
				    inner join TraderContractView tc on uc.contractID = tc.contractID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and datediff(d,@dtMinExpiry,o.expiryDate) >= 0
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, o.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, o.expiryDate, o.strike, o.isCall desc
		end
		else -- not isnull(@iGroupID, 0) = 0
		begin
			if isnull(@iTraderID, 0) = 0
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					o.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
					join OptionRootView ort on cig.contractID = ort.underlyingID					
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status, 0) != 1
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and datediff(d,@dtMinExpiry,o.expiryDate) >= 0
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, o.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, o.expiryDate, /* year(ec.expiryDate), month(ec.expiryDate),*/ o.strike, o.isCall desc
			else
				select	c.contractID as iContractID,
					c.symbol as vcSymbol,
					ort.underlyingID as iUnderlyingContractID,
					o.strike as fStrike, 
					o.expiryDate as dtExpiry,
					o.isCall as tiIsCall,
					ort.lotSize as iLotSize,
					sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty,
					sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) as iPosition,
					ort.optionRootID as iOptionRootID
				from ContractInGroupView cig
					join OptionRootView ort on cig.contractID = ort.underlyingID					
					inner join ContractView uc on ort.underlyingID = uc.contractID
					inner join TraderContractView tc on uc.contractID = tc.contractID
					inner join [Option] o on ort.optionRootID = o.optionRootID
					inner join Contract c on o.optionID = c.contractID
					join ContractPrice cp on cp.contractID = c.contractID and cp.exchangeID is null						
					left join TradeView tr on tr.contractID = c.contractID and tr.traderID = @iTraderID and isnull(tr.status, 0) != 1
				where cig.groupID = @iGroupID 
					and c.contractTypeID = 3
					and tc.traderID = @iTraderID
					and (@iUnderlyingID is Null or ort.underlyingID = @iUnderlyingID)
					and datediff(d,@dtMinExpiry,o.expiryDate) >= 0
				group by c.contractID, c.symbol, ort.underlyingID,
					o.strike, o.expiryDate, o.isCall, ort.lotSize, ort.optionRootID
				order by ort.underlyingID, o.expiryDate, o.strike, o.isCall desc
		end
	end

	return @@error


GO

ALTER  PROC dbo.usp_UnderlyingExpiry_Get
	@iContractID int = null,
	@iTraderID int = null,
	@iGroupID int = null,
	@dtMinExpiry datetime = null
AS
	set nocount on

	if @dtMinExpiry is null	set @dtMinExpiry = '19000101' -- set to min available date to avoid 'is null' checking

	if isnull(@iGroupID, 0) = 0
	begin
		if isnull(@iTraderID, 0) = 0
		begin
			if isnull(@iContractID, 0) = 0
				select
					c.contractID as iUnderlyingID,
					o.expiryDate as dtExpiryDate
				from ContractView c
					inner join OptionRootView r on c.contractID = r.underlyingID
					inner join OptionView o on r.optionRootID = o.optionRootID
				where 
					datediff(d, @dtMinExpiry, o.expiryDate) >= 0
				group by
					c.contractID, o.expiryDate
				order by o.expiryDate
			else
				select
					c.contractID as iUnderlyingID,
					o.expiryDate as dtExpiryDate
				from ContractView c
					inner join OptionRootView r on c.contractID = r.underlyingID
					inner join OptionView o on r.optionRootID = o.optionRootID
				where 
					c.contractID = @iContractID
					and datediff(d, @dtMinExpiry, o.expiryDate) >= 0
				group by
					c.contractID, o.expiryDate
				order by expiryDate
		end
		else
		begin
			if isnull(@iContractID, 0) = 0
				select
					c.contractID as iUnderlyingID,
					o.expiryDate as dtExpiryDate
				from TraderContractView tc 
				    inner join ContractView c on tc.contractID = c.contractID
					inner join OptionRootView r on c.contractID = r.underlyingID
					inner join OptionView o on r.optionRootID = o.optionRootID
				where 
					tc.traderID = @iTraderID
					and datediff(d, @dtMinExpiry, o.expiryDate) >= 0

				group by
					c.contractID, o.expiryDate
				order by o.expiryDate
			else
				select
					c.contractID as iUnderlyingID,
					o.expiryDate as dtExpiryDate
				from TraderContractView tc 
				    inner join ContractView c on tc.contractID = c.contractID
					inner join OptionRootView r on c.contractID = r.underlyingID
					inner join OptionView o on r.optionRootID = o.optionRootID
				where 
					tc.traderID = @iTraderID
					and c.contractID = @iContractID
					and datediff(d, @dtMinExpiry, o.expiryDate) >= 0
				group by
					c.contractID, o.expiryDate
				order by expiryDate
		end
	end
	else
	begin
		if isnull(@iTraderID, 0) = 0
			select
				c.contractID as iUnderlyingID,
				o.expiryDate as dtExpiryDate
			from ContractInGroupView cig 
				inner join ContractView c on c.contractID = cig.contractID
				inner join OptionRootView r on c.contractID = r.underlyingID
				inner join OptionView o on r.optionRootID = o.optionRootID
			where 
				cig.groupID = @iGroupID
				and datediff(d, @dtMinExpiry, o.expiryDate) >= 0
			group by
				c.contractID, o.expiryDate
			order by o.expiryDate
		else
			select
				c.contractID as iUnderlyingID,
				o.expiryDate as dtExpiryDate
			from ContractInGroupView cig 
				inner join ContractView c on c.contractID = cig.contractID
			    inner join TraderContractView tc on c.contractID = tc.contractID
				inner join OptionRootView r on c.contractID = r.underlyingID
				inner join OptionView o on r.optionRootID = o.optionRootID
			where 
				cig.groupID = @iGroupID
				and tc.traderID = @iTraderID
				and datediff(d, @dtMinExpiry, o.expiryDate) >= 0
			group by
				c.contractID, o.expiryDate
			order by expiryDate
	end


GO

ALTER     proc dbo.usp_UnderlyingOptionByExpiry_Get
	@iUnderlyingID int,		 
	@iExpiryDate datetime
AS

	set nocount on

	select
		o.optionID as iContractID,
		c.symbol as vcSymbol,
		o.strike as fStrike, 
		o.isCall as tiIsCall,
		ort.optionRootID as iOptionRootID,
		ort.symbol as vcOptionRootSymbol,
		ort.lotSize as iLotSize,
		cp.priceClose as fPriceClose,
		cp.priceTheo as fPriceTheoClose
	from OptionRootView ort
		inner join OptionView o on ort.optionRootID = o.optionRootID
		inner join ContractView c on o.optionID = c.contractID
	    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
	where
		ort.underlyingID = @iUnderlyingID
		and datediff(d, @iExpiryDate, o.expiryDate )=0

GO


ALTER  PROC dbo.usp_ExpiryVegaWeightSave_Save
	@dtExpiry datetime,
	@fVegaWeight float
as
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	begin tran
	DECLARE @error_var int, @rowcount_var int

	update VegaWeight
	set vegaWeight = @fVegaWeight
	where 
  	 datediff(d, @dtExpiry, expiryDate) = 0
	SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
	if @error_var <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

	if @rowcount_var = 0
        begin
         insert into VegaWeight (ExpiryDate, VegaWeight) values (@dtExpiry, @fVegaWeight)
	 if @@ERROR <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end
        end

	commit tran
finish:
	return @error

GO

ALTER      PROC dbo.usp_BoCustomStrikeSkewPoint_Default_Save 
	@iContractID int,
	@fStrike float,
	@dtExpiry datetime,
	@fVolatility float
AS
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = 0

	declare @iCustomStrikeSkewPointID int 
		set @iCustomStrikeSkewPointID = Null

	--check and create record in VolaSurfaceData with 'IsDefaultSurface=1'
	declare @iVolaSurfaceDataID int 
		set @iVolaSurfaceDataID = 0

	declare @iType int
		set @iType = 2 --OTM_IVTYPE

	exec @iVolaSurfaceDataID = usp_CreateDefaultVolaSurface_Save @iContractID

	select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
		from CustomStrikeSkewPoint 
		where volaSurfaceDataID = @iVolaSurfaceDataID 
			and abs(strike - @fStrike) < 0.009
			and type = @iType 
			and abs(datediff(d, expiryDate, @dtExpiry))<2
			and actionID < 3
-- VK on
	--DECLARE @expInCalID AS INT
	--SELECT  @expInCalID = EIC.expiryInCalendarID
	--FROM volaSurfaceData vsd
	--JOIN Contract c ON c.contractID = vsd.contractID
	--JOIN ExpiryInCalendar EIC on EIC.expiryID = @iExpiryID AND EIC.expCalendarID = c.expCalendarID
	--WHERE vsd.volaSurfaceDataID = @iVolaSurfaceDataID
-- VK off
  
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					isBasePoint,
					expiryDate)
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						-1,
						@dtExpiry)
	
				if @@error <> 0 begin set @error = -1 if @@TranCount = 1 Rollback Transaction goto finish end

			end
		else
			begin
				update CustomStrikeSkewPoint set 
					--volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					--type = isnull(@iType,type),
					--strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility)
					--expiryDate = isnull(@iExpiryID,expiryID )
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
	   			if @@error <> 0 begin set @error = -2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction

finish:
	Return(@error)
GO

ALTER    PROCEDURE usp_EodOldFutures_Del
	@iFutureRootID INT
AS
	-------------------------------------------------
	SET NOCOUNT ON
	-------------------------------------------------
	DECLARE @error INT		SET @error = 0

	CREATE TABLE #FutureContractsForDelete(contractID INT) 

	INSERT INTO #FutureContractsForDelete
		SELECT F.futureID
			FROM 
				Future F
					JOIN FutureRoot FR ON FR.futureRootID = F.futureRootID
			WHERE 
				( 
					( 
						 F.maturityDate  <  GETDATE()
						OR DATEDIFF(d, F.actionDate, GETDATE()) > 0 
					)
					AND F.futureRootID = @iFutureRootID
				)

	INSERT INTO #FutureContractsForDelete
		SELECT FO.futureOptionID
			FROM 
				FutureOption FO
					JOIN Future F ON F.futureID = FO.futureID
					JOIN FutureRoot FR ON FR.futureRootID = F.futureRootID
			WHERE 
				( 
					( 
						FO.expiryDate < GETDATE() 
						OR DATEDIFF(d, F.actionDate, GETDATE()) > 0 
					)
					AND F.futureRootID = @iFutureRootID
				)

	EXEC @error = usp_EodFuture_Del
	-- leave this shit or change CEodStepLoadMktStructure::_FinishLoadRootFutures() in EodLoadMktStructure.cpp
	SELECT NULL AS vcSymbol FROM #FutureContractsForDelete WHERE contractID IS NULL

finish:
	RETURN(@error)


GO
GO


ALTER    PROC dbo.usp_EodOption_Import
	@iRootID INT,
	@dtExpiryDate DATETIME,
	@fStrike FLOAT,
	@iIsCall TINYINT,
	@vcSymbol VARCHAR(20),
	@vcContractName VARCHAR(255),
	@iOptionID_bad INT OUTPUT,
	@fPriceClose FLOAT = NULL,
    @bSaveClosePrice BIT = NULL
AS
	-------------------------------------------------
	SET NOCOUNT ON
	-------------------------------------------------
	DECLARE @error INT		SET @error = 0
	DECLARE	@OptionID INT

	--===================================================================			
	-- select the @OptionID
	--===================================================================			
	SELECT @OptionID = optionID
	FROM [Option] WITH(NOLOCK, INDEX = IX_Option_optionRootID_isCall_strike_expiryID_actionID)
	WHERE optionRootID = @iRootID
		AND isCall = @iIsCall
		AND ABS(strike - @fStrike) < 0.009
		AND DATEDIFF(d, expiryDate,@dtExpiryDate) < 1
		AND actionID < 3

	IF @OptionID IS NOT NULL
	BEGIN
		BEGIN TRANSACTION

			SELECT @iOptionID_bad = contractID
			FROM Contract WITH(NOLOCK, INDEX = IX_Contract_symbol_contractTypeID_actionID)
			WHERE symbol = @vcSymbol
				--and contractID <> @OptionID
				AND contractTypeID = 3
				AND actionID < 3

			IF @iOptionID_bad IS NOT NULL AND @iOptionID_bad != @OptionID
			BEGIN
				IF NOT EXISTS (SELECT * FROM Trade WHERE ContractID = @iOptionID_bad) AND
					NOT EXISTS (SELECT * FROM [Order] WHERE ContractID = @iOptionID_bad)
				BEGIN	
					EXEC @error = usp_Option_Del @iOptionID = @iOptionID_bad			
	
					IF @@ERROR != 0 OR @error != 0
					BEGIN 
						IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION 
						GOTO finish 
					END
				END
				ELSE
					UPDATE
						Contract
					SET 
						symbol = symbol + '*'
					WHERE
						ContractID = @iOptionID_bad
			END
				
			UPDATE Contract 
			SET symbol = @vcSymbol, contractName = @vcContractName
			WHERE contractID=@OptionID AND actionID < 3
			IF @@ERROR <> 0 BEGIN SET @error = -5 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			UPDATE [Option]  
			SET isManualyEntered = 0 
			WHERE optionID=@OptionID and actionID < 3
			IF @@ERROR <> 0 BEGIN SET @error = -6 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			IF @bSaveClosePrice IS NOT NULL AND  @bSaveClosePrice = 1
            BEGIN 	
				UPDATE ContractPrice
					SET priceClose = @fPriceClose
					WHERE contractID = @OptionID AND exchangeID IS NULL
				IF @@ERROR <> 0 BEGIN SET @error = -6 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
            END
		COMMIT TRANSACTION
	END
	ELSE
	BEGIN
		BEGIN TRANSACTION

			SELECT @iOptionID_bad = contractID
			FROM Contract WITH(NOLOCK, INDEX = IX_Contract_symbol_contractTypeID_actionID)
			WHERE symbol = @vcSymbol
				AND contractTypeID = 3
				AND actionID < 3

			IF @iOptionID_bad IS NOT NULL
			BEGIN
				IF NOT EXISTS (SELECT * FROM Trade WHERE ContractID = @iOptionID_bad) AND
					NOT EXISTS (SELECT * FROM [Order] WHERE ContractID = @iOptionID_bad)
				BEGIN	
					EXEC @error = usp_Option_Del @iOptionID = @iOptionID_bad			
	
					IF @@ERROR != 0 OR @error != 0
					BEGIN 
						IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION 
						GOTO finish 
					END
				END
				ELSE
					UPDATE
						Contract
					SET 
						symbol = symbol + '*'
					WHERE
						ContractID = @iOptionID_bad
			END

			INSERT INTO Contract (contractTypeID, symbol, contractName)
				 VALUES (3, @vcSymbol,@vcContractName)

			SET @OptionID = @@IDENTITY
			IF @@ERROR <> 0 BEGIN SET @error = -5 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			INSERT INTO [Option] (optionID, optionRootID, isCall, strike, expiryDate)
				 VALUES (@OptionID, @iRootID, @iIsCall, @fStrike, @dtExpiryDate)
			IF @@ERROR <> 0 BEGIN SET @error = -6 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
	
			IF NOT EXISTS(SELECT 1 FROM ContractPrice WHERE exchangeID IS NULL AND contractID = @OptionID)
			BEGIN
	  		    IF @bSaveClosePrice IS NOT NULL AND  @bSaveClosePrice = 1
					INSERT INTO ContractPrice(contractID, exchangeID, priceClose)
						VALUES(@OptionID, NULL, @fPriceClose)
                ELSE
					INSERT INTO ContractPrice(contractID, exchangeID, priceClose)
						VALUES(@OptionID, NULL, NULL)

			    IF (@@ERROR <> 0) BEGIN SET @error = -7 iF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
			END 
		
		COMMIT TRANSACTION
	END 
	 
   	SET @error = @OptionID
finish:
	RETURN(@error)



GO


ALTER    PROCEDURE dbo.usp_EodMsOptionCache_Save
	@iUnderlyingID INT,
	@vcSymbol VARCHAR(20),
	@dtExpiry DATETIME,
	@fStrike FLOAT,
	@tiIsCall TINYINT,
	@iLotSize INT
AS
	----------------------------------------------------------------------
	SET NOCOUNT ON
	----------------------------------------------------------------------
	DECLARE @error INT		SET @error = 0
	DECLARE @vcUnderlyingSymbol VARCHAR(20)
	DECLARE @vcError AS VARCHAR(8000)

	-- check parameter
	IF @iUnderlyingID IS NULL OR @vcSymbol IS NULL OR @dtExpiry IS NULL OR ISNULL(@fStrike, 0) <= 0
	BEGIN
		RAISERROR ('Invalid parameter!', 16, 1)
		SET @error = -1
		GOTO finish 
	END

	-- check underlying existance
	IF NOT EXISTS (SELECT 1 FROM MsUnderlyingCache WHERE underlyingID = @iUnderlyingID)
	BEGIN
		RAISERROR ('Invalid underlying contract!', 16, 1)
		SET @error = -2
		GOTO finish 
	END

	-- check option root symbol
	IF LEN(@vcSymbol) < 3
	BEGIN
		SET @vcError = 'Invalid option symbol ''' + @vcSymbol + ''': at least 3 characters expected!'
		RAISERROR(@vcError, 16, 1)
		SET @error = -4
		GOTO finish 
	END

	-- correct lotsize
	IF ISNULL(@iLotSize, 0) <= 0
		SET @iLotSize = 100

	INSERT INTO MsOptionCache(underlyingID, symbol, expiryDate, strike, isCall, lotSize)
		VALUES(@iUnderlyingID, @vcSymbol, @dtExpiry, @fStrike, @tiIsCall, @iLotSize)
	
	IF @@ERROR <> 0 BEGIN SET @error = -5 GOTO finish END

finish:
	RETURN @error

GO


ALTER PROCEDURE dbo.usp_EodWeightedVega_Save
	@dtExpiryDate SMALLDATETIME,
	@fVegaWeight FLOAT
AS
	-------------------------------------------------
	SET NOCOUNT ON
	-------------------------------------------------
	DECLARE @error INT		SET @error = -1
	

	DELETE FROM VegaWeight WHERE ExpiryDate < GETDATE()

	IF EXISTS (SELECT * FROM VegaWeight WHERE DATEDIFF( d, ExpiryDate, @dtExpiryDate ) < 1) 
		UPDATE VegaWeight
		SET VegaWeight = @fVegaWeight
		WHERE DATEDIFF( d, ExpiryDate, @dtExpiryDate ) < 1
	ELSE
		INSERT INTO VegaWeight(ExpiryDate, VegaWeight)
		VALUES(@dtExpiryDate, @fVegaWeight)
	
    SET @error = @@ERROR
	RETURN(@error)

GO





ALTER    PROC usp_EodFutureOption_Import
	@iFutureID INT,
	@fStrike FLOAT,
	@dtExpiryDate DATETIME,
	@tiIsCall TINYINT,
	@vcSymbol VARCHAR(20),
	@vcContractName VARCHAR(255),
	@vcBadFutureOptionName VARCHAR(255) OUTPUT,
	@fPriceClose FLOAT = NULL
AS
	-------------------------------------------------
	SET NOCOUNT ON
	-------------------------------------------------
	DECLARE @error INT		SET @error = 0
	DECLARE	@iFutureOptionID INT
	DECLARE @iFutureOptionID_bad INT
	DECLARE @iExpCalendarID INT 
	
	--===================================================================			
	-- select the @iFutureOptionID
	--===================================================================			
	SELECT @iFutureOptionID = futureOptionID
	FROM FutureOption (NOLOCK)
	WHERE futureID = @iFutureID
		AND isCall = @tiIsCall
		AND ABS(strike - @fStrike) < 0.009
		AND Datediff(d, expiryDate, @dtExpiryDate) = 0
				
	IF NOT EXISTS (SELECT 1 FROM tempdb..sysobjects WHERE ID=OBJECT_ID('tempdb..#FutureContractsForDelete'))
		CREATE TABLE #FutureContractsForDelete(contractID INT)				
	IF @@ERROR <> 0 BEGIN SET @error = -1 GOTO finish END
				
	IF @iFutureOptionID IS NOT NULL
	BEGIN
		BEGIN TRANSACTION
			SELECT @iFutureOptionID_bad = contractID
			FROM Contract
			WHERE symbol=@vcSymbol
				--AND contractID<>@iFutureOptionID
				AND contractTypeID=5

			IF @iFutureOptionID_bad IS NOT NULL AND @iFutureOptionID_bad != @iFutureOptionID
			BEGIN
				INSERT INTO #FutureContractsForDelete (contractID)  VALUES (@iFutureOptionID_bad)
				IF @@ERROR <> 0 BEGIN SET @error = -4 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
				
				EXEC @error = usp_EodFutureOption_Del

				IF @error <> 0
				BEGIN 
					SET @error = -5
					IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION 
					GOTO finish 
				END				
			END

			/*IF @dtExpiryDate IS NOT NULL
			begin
				select @iExpCalendarID = ecv.ExpCalendarID from Future f
				inner join FutureRoot fr on fr.FutureRootID = f.FutureRootID
				inner join ExpiryCalendarView ecv on ecv.expCalendarName = fr.futureRootSymbol + '_Opt'
				where f.FutureID = @iFutureID

				if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtExpiryDate, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @iFutureOptionID--@iFutureID
					end
			end			
*/
			UPDATE Contract  SET symbol = @vcSymbol, contractName = @vcContractName WHERE contractID=@iFutureOptionID
			IF @@ERROR <> 0 BEGIN SET @error = -6 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			UPDATE FutureOption SET actionDate = GETDATE() WHERE futureOptionID=@iFutureOptionID
			IF @@ERROR <> 0 BEGIN SET @error = -7 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			UPDATE ContractPrice
			SET priceClose = @fPriceClose
			WHERE exchangeID IS NULL AND contractID = @iFutureOptionID
			IF (@@ERROR <> 0) BEGIN SET @error = -10 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
		COMMIT TRANSACTION
	END
	ELSE
	BEGIN
		BEGIN TRANSACTION
			SELECT @iFutureOptionID_bad = contractID
			FROM Contract
			WHERE symbol=@vcSymbol
				AND contractTypeID=5

			IF @iFutureOptionID_bad IS NOT NULL
			BEGIN	
				INSERT INTO #FutureContractsForDelete (contractID)  VALUES (@iFutureOptionID_bad)
				IF @@ERROR <> 0 BEGIN SET @error = -4 iF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
				
				EXEC @error = usp_EodFutureOption_Del

				IF @error <> 0
				BEGIN 
					SET @error = -5
					IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION 
					GOTO finish 
				END				
			END			
				
			INSERT INTO Contract (contractTypeID, symbol, contractName)
				 VALUES (5, @vcSymbol,@vcContractName)
			SET @iFutureOptionID = @@IDENTITY
			IF @@ERROR <> 0 BEGIN SET @error = -8 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			INSERT INTO FutureOption (futureOptionID, futureID, isCall, expiryDate, strike)
				 VALUES (@iFutureOptionID, @iFutureID, @tiIsCall, @dtExpiryDate, @fStrike)
			IF @@ERROR <> 0 BEGIN SET @error = -9 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END

			/*if @dtExpiryDate is not null
			begin
				select @iExpCalendarID = ecv.ExpCalendarID from Future f
				inner join FutureRoot fr on fr.FutureRootID = f.FutureRootID
				inner join ExpiryCalendarView ecv on ecv.expCalendarName = fr.futureRootSymbol + '_Opt'
				where f.FutureID = @iFutureID

				if @iExpCalendarID is not null
					begin
						exec usp_Expiry_Save @iExpiryID, null, @dtExpiryDate, null, @iExpCalendarID
						update Contract set expCalendarID = @iExpCalendarID where ContractID = @iFutureOptionID--@iFutureID
					end
			end*/
	
			--========================================================================
			-- make record with null exchange for this option
			--========================================================================
			IF NOT EXISTS(SELECT 1 FROM ContractPrice WHERE exchangeID IS NULL AND contractID = @iFutureOptionID)
			BEGIN
				INSERT INTO ContractPrice(contractID, exchangeID, priceClose)
					VALUES(@iFutureOptionID, null, @fPriceClose)
				IF (@@ERROR <> 0) BEGIN SET @error = -10 IF @@TRANCOUNT = 1 ROLLBACK TRANSACTION GOTO finish END
			END 

		COMMIT TRANSACTION
	END 
	 
   	SET @error = @iFutureOptionID
finish:
	RETURN(@error)




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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '4.49.145')
	begin
		update DataInfo set keyValue = '4.49.145' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 4.49.145' '4.49.145 update message:' 
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