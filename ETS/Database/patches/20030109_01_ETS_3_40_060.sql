/*
patch for last release
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
if not exists(select * from DataInfo where dataInfoID = 1 and (keyValue = '3.40.059' or (keyValue = '3.40.060')))
	BEGIN
		select 'You can''t run this script, because you have not run script for previous version!' '3.40.060 error message:' 
		Select cast(keyValue as varchar(25)) as 'DB version', '3.40.060' as  'Version in script' from DataInfo where dataInfoID = 1
		INSERT INTO #tmpErrors (Error) SELECT -1 BEGIN TRANSACTION
	END
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO
/*DATA MODIFICATION SECTION*/
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_PositionForReconcile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_PositionForReconcile_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingByBook_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingByBook_Get]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_UnderlyingOptionsByList_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_UnderlyingOptionsByList_Get]
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE   PROCEDURE dbo.usp_PositionForReconcile_Get
	@tiByTrader tinyint = 0,
	@lastUTCTradeDate datetime = null
AS
	if isnull(@tiByTrader, 0) = 0
	begin
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as vcOptionRootID,
			null as vcOptionRootSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			1 as iLotSize,
			cp.priceClose as fPriceClose,

			null as iTraderID,
			null as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty
		
		from TradeView tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where c.contractTypeID < 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by c.contractID, c.contractTypeID, c.Symbol, cp.priceClose
		
		union
	
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			ORt.optionRootID as vcOptionRootID,
			ORt.symbol as vcOptionRootSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,

			ORt.lotSize as iLotSize,
			cp.priceClose as fPriceClose,

			null as iTraderID,
			null as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty

		from TradeView tr 
			inner join ContractView C on tr.contractID = C.contractID
			inner join OptionView o on c.contractID = o.optionID
			inner join OptionRootView ORt on o.optionRootID = ORt.optionRootID
			inner join ContractView UC on ORt.underlyingID = UC.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
	
		where c.contractTypeID = 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by C.contractID, C.contractTypeID, C.Symbol,ORt.optionRootID,ORt.symbol,
			EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
			ORt.lotSize, cp.priceClose
	
		order by vcSymbol
	end
	else
	begin
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as vcOptionRootID,
			null as vcOptionRootSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			1 as iLotSize,
			cp.priceClose as fPriceClose,
			trdr.TraderID as iTraderID,
			trdr.acronym as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty
		from TradeView tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join TraderView trdr on tr.traderID = trdr.traderID
		where c.contractTypeID < 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by c.contractID, c.contractTypeID, c.Symbol,cp.priceClose, trdr.TraderID, trdr.acronym
		
		union
	
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			ORt.optionRootID as vcOptionRootID,
			ORt.symbol as vcOptionRootSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			ORt.lotSize as iLotSize,
			cp.priceClose as fPriceClose,
			trdr.TraderID as iTraderID,
			trdr.acronym as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty

		from TradeView tr 
			inner join ContractView C on tr.contractID = C.contractID
			inner join OptionView o on c.contractID = o.optionID
			inner join OptionRootView ORt on o.optionRootID = ORt.optionRootID
			inner join ContractView UC on ORt.underlyingID = UC.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ExpiryInCalendarView EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			left join TraderView trdr on tr.traderID = trdr.traderID
	
		where c.contractTypeID = 3
			and tr.status in (0, 3)
			and (@lastUTCTradeDate is null or datediff(day,tr.tradeDate,@lastUTCTradeDate) >= 0)
		group by C.contractID, C.contractTypeID, C.Symbol,ORt.optionRootID,ORt.symbol,
			EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
			ORt.lotSize, cp.priceClose, trdr.TraderID, trdr.acronym
	
		order by vcSymbol
	end


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO



CREATE  PROCEDURE dbo.usp_UnderlyingByBook_Get 
	@iTraderID int = null, 
	@iBookID int
AS
/*
	Created by: Zaur N
	Description: this procedure created with template, desined by Zaur Nuraliev
*/
	-------------------------------------------------
	set nocount on
	set ansi_warnings off
	-------------------------------------------------
	declare @error int		set @error = -1

	select
	    c.contractID as iContractID,
	    c.contractTypeID as iContractTypeID,
	    c.Symbol as vcSymbol,
		c.expCalendarID as iExpCalendarID,
		s.calcOptionType as tiCalcOptionType,
		case when s.stockID is not null then s.isHTB else i.isHTB end tiIsHTB,
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
		case when s.stockID is not null then s.isActive else i.isActive  end tiIsActive,
		case when s.stockID is not null then s.skew else i.skew end fSkew,
		case when s.stockID is not null then s.kurt else i.kurt end fKurt,
		sum(tr.quantity * tr.price * (case when tr.isBuy <> 0 then 1 else -1 end)) as fPosition
	from ContractView c 
	    inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		left join TradeView tr on tr.contractID = c.contractID and isnull(tr.status,0)!= 1 and tr.bookID = @iBookID
	    left join StockView s on c.contractID = s.stockID
	    left join IndexView i on c.contractID = i.indexID
	where c.contractID in 
		(select c.contractID 
			from ContractView c join TradeView t on c.contractID = t.contractID 
			where c.contractTypeID < 3
				and t.bookID = @iBookID and (@iTraderID is null or t.traderID = @iTraderID)
		union
		select underlyingID 
			from OptionRootView ort
				join OptionView o on ort.optionRootID = o.optionRootID
				join TradeView t on t.contractID = o.optionID
			where t.bookID = @iBookID and (@iTraderID is null or t.traderID = @iTraderID))
	group by 
		c.contractID, c.contractTypeID, c.Symbol, c.expCalendarID, s.calcOptionType, s.isHTB, s.divFreq,
    	s.divDate, s.divAmt, s.isDivCustom, s.divFreqCustom, s.divDateCustom, s.divAmtCustom, cp.priceClose, 
		s.isActive, s.skew, s.kurt,s.stockID,i.isHTB,i.isActive,i.skew,i.kurt
	order by c.Symbol

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


CREATE PROCEDURE dbo.usp_UnderlyingOptionsByList_Get 
    @iTraderID int = null,
    @iBookID int,
    @vcUnderlyingID1 varchar(8000),
    @vcUnderlyingID2 varchar(8000) = null,
    @vcUnderlyingID3 varchar(8000) = null,
    @dtMinExpiry datetime = null
AS
/*
	Created by: Zaur N.
	Description: this procedure created with template, desined by Zaur Nuraliev
*/
	-------------------------------------------------
	set nocount on
	set ansi_warnings off
	-------------------------------------------------
	declare @error int		set @error = -1

	Declare @vcSQLstring1 as varchar(8000),
			@vcSQLstring2 as varchar(8000),
			@vcSQLstring3 as varchar(8000),
			@vcSQLstring4 as varchar(8000),
			@vcSQLstring5 as varchar(8000)

	set @vcSQLstring1 = 
		'select	c.contractID iContractID,
			c.symbol vcSymbol,
			ort.underlyingID iUnderlyingContractID,
			o.strike fStrike, 
			ec.expiryDate dtExpiry,
			o.isCall bIsCall,
			ort.lotSize iLotSize,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) iQty,
			sum(tr.quantity * ort.lotSize * (case when tr.isBuy <> 0 then 1 else -1 end)) iPosition
		from ContractView c  
			join [Option] o on o.optionID=c.contractID
			join OptionRoot ort on o.optionRootID=ort.optionRootID
			join Contract uc on ort.underlyingID=uc.contractID
			join ExpiryInCalendarView ec on o.expiryID=ec.expiryID and ec.expCalendarID=uc.expCalendarID
			join ContractPrice cp on cp.contractID=c.contractID and cp.exchangeID is null						
			left join TradeView tr on tr.contractID=c.contractID and isnull(tr.status, 0)!= 1 and tr.bookID=' + cast(@iBookID as varchar(2000)) +
		' where c.contractTypeID=3 '

	if @dtMinExpiry is not null
		set @vcSQLstring3 = ' and ec.expiryDate >=' + '''' + convert(varchar(12),getdate(),112) + '''' 
	else
		set @vcSQLstring3 = ''

	if @iTraderID is not null
		set @vcSQLstring4 = ' and tr.traderID=' + cast(@iTraderID as varchar(2000))
	else
		set @vcSQLstring4 = ''

	set @vcSQLstring5 = ' 
		group by c.contractID, c.symbol, ort.underlyingID,o.strike, ec.expiryDate, o.isCall, ort.lotSize 
			order by ort.underlyingID, year(ec.expiryDate), month(ec.expiryDate), o.strike, o.isCall desc'

	if isnull(@vcUnderlyingID1,'')!= '' and isnull(@vcUnderlyingID2,'') = '' and isnull(@vcUnderlyingID3,'') = ''
		begin
			exec(@vcSQLstring1 + 'and uc.contractID in (' + @vcUnderlyingID1 + ')' +
				 @vcSQLstring3 + @vcSQLstring4 + @vcSQLstring5)
		end
	else if isnull(@vcUnderlyingID1,'')!= '' and isnull(@vcUnderlyingID2,'')!= '' and isnull(@vcUnderlyingID3,'') = ''
		begin
			exec(@vcSQLstring1 + 'and uc.contractID in (' + @vcUnderlyingID1 + ',' + @vcUnderlyingID2 + ')' +
				 @vcSQLstring3 + @vcSQLstring4 + @vcSQLstring5)
		end
	else if isnull(@vcUnderlyingID1,'')!= '' and isnull(@vcUnderlyingID2,'')!= '' and isnull(@vcUnderlyingID3,'')!= ''
		begin
			exec(@vcSQLstring1 + 'and uc.contractID in (' + @vcUnderlyingID1 + ',' + @vcUnderlyingID2 + ',' + @vcUnderlyingID3 + ')' +
				 @vcSQLstring3 + @vcSQLstring4 + @vcSQLstring5)
		end

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
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.40.060')
	begin
		update DataInfo set keyValue = '3.40.060' where dataInfoID = 1
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
	select 'DB VERSION WAS CHANGED TO 3.40.060' '3.40.060 update message:' 
	select 'The database update succeeded'

	COMMIT TRANSACTION
END
	ELSE select 'The database update failed'
GO
DROP TABLE #tmpErrors
GO
