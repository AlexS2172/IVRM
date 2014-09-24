/*
	this script for fix MLine00000383 " Vola manager: Fit to implied Submitted"
	also this script remove unusefull columns in Stock & Index tables
*/

declare @DFName varchar(125)
	select @DFName = [name] 
		from sysobjects 
		where parent_obj = object_id('Stock') and xtype = 'D' 
			and id = (select cdefault from syscolumns where id = object_id('Stock') and name = 'expCalendarTypeID')

if @DFName is not null
	begin
		exec('Alter table Stock Drop constraint ' + @DFName)
		if @@error = 0
				Alter Table Stock Drop column expCalendarTypeID
	end

select @DFName = [name] 
	from sysobjects 
	where parent_obj = object_id('Index') and xtype = 'D' 
		and id = (select cdefault from syscolumns where id = object_id('Index') and name = 'expCalendarTypeID')

if @DFName is not null
	begin
		exec('Alter table [Index] Drop constraint ' + @DFName)
		if @@error = 0
				Alter Table [Index] Drop column expCalendarTypeID
	end
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

ALTER      Procedure dbo.usp_TraderContract_Get
	@iContractID int = null,
	@iTraderID int = null,
	@dtActionDate datetime = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is get information from TraderContract table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	select traderID as iTraderID,contractID as iContractID
		from TraderContract
		where 
			(traderID = @iTraderID or @iTraderID is null) and 
			(contractID = @iContractID or @iContractID is null) and 
			(actionDate >= @dtActionDate or @dtActionDate is null and actionID < 3)

	set @error = @@error
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
ALTER       Procedure dbo.usp_ReportStock_Get
	@iStockID int = Null, 
	@iFilterID int = Null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from [Option] table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	--=====================================================
	-- work with filter
	--=====================================================
	create table #FilteredStocks(stockID int)
              insert into #FilteredStocks 
                      exec usp_FilterData_Get @iFilterID
	--=====================================================
	-- data selection
	--=====================================================
		select
			Contract.ContractID as iContractID,
			Contract.symbol as vcSymbol,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceLast as fPriceLast,
			Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
			case when isDivCustom = 1 then divFreqCustom else Stock.divfreq end iDivFreq,
			case when isDivCustom = 1 then divAmtCustom else Stock.divAmt end fDivAmt,
			case when isDivCustom = 1 then divDateCustom else Stock.DivDate end dtDivDate,
			S2.currentHV as fCurrentHV,
			CP.priceClose as fPriceClose,
	  		Stock.isHTB as bIsHTB,
			Stock.isDivCustom as tiIsDivCustom,
			Stock.CalcOptionType as tiCalcOptionType,
			Stock.CalcModelType as tiCalcModelType,
			Contract.expCalendarID as tiExpCalendarID
			from ContractView Contract
					join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
					join StockView Stock on Contract.contractID = Stock.StockID
					left join (select StockDataHistory.stockID,currentHV,[date] from StockDataHistory join
									(select stockID, max([date]) as maxDate from StockDataHistory group by stockID) S1
								on S1.stockID=StockDataHistory.stockID and S1.maxDate=StockDataHistory.[date]) S2 
						on S2.stockID=Stock.stockID
	
			where (
				(@iStockID is null and (Stock.stockID in (select stockID from #FilteredStocks)))
				or 
				(@iStockID is not null and Contract.ContractID=@iStockID 
					and (Stock.stockID in (select stockID from #FilteredStocks) or @iFilterID is null))
				)
				and Contract.contractTypeID = 2
	union
		select
			Contract.ContractID as iContractID,
			Contract.symbol as vcSymbol,
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceLast as fPriceLast,
			Contract.contractTypeID as iContractTypeID, -- 1-index 2-stock 3-option
			0 as iDivFreq,
			I.yield as fDivAmt,
	  		null as dtDivDate,
			S2.currentHV as fCurrentHV,
			CP.priceClose as fPriceClose,
	  		I.isHTB as bIsHTB,
			null as tiIsDivCustom,
			I.CalcOptionType as tiCalcOptionType,
			I.CalcModelType as tiCalcModelType,
			Contract.expCalendarID as tiExpCalendarID
			from ContractView Contract
					join ContractPrice CP on CP.contractID=Contract.contractID and CP.exchangeID is null
					join IndexView I on Contract.contractID = I.indexID
					left join (select IndexDataHistory.indexID,currentHV,[date] from IndexDataHistory join
									(select indexID, max([date]) as maxDate from IndexDataHistory group by indexID) S1
								on S1.indexID=IndexDataHistory.indexID and S1.maxDate=IndexDataHistory.[date]) S2 
						on S2.indexID=I.indexID
			where (
				(@iStockID is null and (I.indexID in (select stockID from #FilteredStocks))) or 
				(@iStockID is not null and @iFilterID is not null and (Contract.ContractID=@iStockID and (I.indexID in (select stockID from #FilteredStocks)))) or 
				(@iStockID is not null and I.indexID = @iStockID and @iFilterID is null)
				)
				and Contract.contractTypeID = 1
		order by Contract.symbol


	set @error = @@error
             drop table #FilteredStocks
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
ALTER Procedure dbo.usp_CustomStrikeSkewPoint_Get 
	@iCustomStrikeSkewPointID int = null, 
	@iVolaSurfaceDataID int = null,
	@iType int = null,
	@dtExpDate datetime = null,
	@dtActionDate datetime = null 
as
/*
	Created by Zaur Nuraliev
	Description:	this procedure get information from 
					CustomStrikeSkewPoint table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1

	declare	@iExpiryID int
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)

	select 
		CSSP.customStrikeSkewPointID as iCustomStrikeSkewPointID,
		CSSP.volaSurfaceDataID as iVolaSurfaceDataID,
		CSSP.strike as fStrike, 
		CSSP.type as iType,
		CSSP.volatility as fVolatility,
		convert(datetime,convert(varchar(6),E.expiryDate,112) + '01') as dtExpDate,
		CSSP.isBasePoint as iIsBasePoint,
		CSSP.actionDate as dtActionDate,
		CSSP.actionID as tiActionID
		from CustomStrikeSkewPoint CSSP
			join VolaSurfaceData on CSSP.volaSurfaceDataID = VolaSurfaceData.volaSurfaceDataID
			join Contract C on  VolaSurfaceData.contractID = C.contractID
			join ExpiryInCalendar E on E.expCalendarID = C.expCalendarID and CSSP.expiryID = E.expiryID
		where 
			(@iCustomStrikeSkewPointID is null or customStrikeSkewPointID=@iCustomStrikeSkewPointID)
		and (@iVolaSurfaceDataID is null or CSSP.volaSurfaceDataID=@iVolaSurfaceDataID) 
		and (@iType is null or type=@iType) 
		and (@dtExpDate is null or E.expiryID = @iExpiryID)
		and ((CSSP.actionDate>=@dtActionDate or @dtActionDate is null) and (@dtActionDate is not null or @dtActionDate is null and CSSP.actionID<3))

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

ALTER          Procedure dbo.usp_CustomStrikeSkewPoint_Save 
	@iCustomStrikeSkewPointID int = Null output,
	@iVolaSurfaceDataID int = Null,
	@iType int = Null,
	@fStrike float = Null,
	@fVolatility float = Null,
	@dtExpDate datetime = Null,
	@iIsBasePoint int = Null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure save inf. into CustomStrikeSkewPoint table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	declare	@iExpiryID int
		set @iExpiryID = (year(@dtExpDate) - 2000) * 12 + month(@dtExpDate)


	if exists (select * from CustomStrikeSkewPoint where 
						volaSurfaceDataID = @iVolaSurfaceDataID and 
						strike = @fStrike and 
						type = @iType and 
						expiryID = @iExpiryID
				and actionID < 3 and (@iCustomStrikeSkewPointID <> CustomStrikeSkewPointID or @iCustomStrikeSkewPointID is null))
		begin
			if @iCustomStrikeSkewPointID is null
				select @iCustomStrikeSkewPointID = CustomStrikeSkewPointID
					from CustomStrikeSkewPoint 
					where volaSurfaceDataID = @iVolaSurfaceDataID and strike = @fStrike and type = @iType and expiryID = @iExpiryID and actionID < 3
			else if @iCustomStrikeSkewPointID is not null 
				begin
					Raiserror ('Row in CustomStrikeSkewPoint table  with same volaSurfaceDataID, strike, type and expDate already exists!', 16, 1)
					Return (-1)
				end
		end 
  
	Begin Transaction
		if @iCustomStrikeSkewPointID is null 
			begin
				insert into CustomStrikeSkewPoint (
					volaSurfaceDataID,
					type,
					strike,
					volatility,
					expiryID,
					isBasePoint) 
				values (@iVolaSurfaceDataID,
						@iType,	
						@fStrike,
						@fVolatility,
						@iExpiryID,
						@iIsBasePoint)
	
				if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iCustomStrikeSkewPointID=@@IDENTITY
			end
		else
			begin
				update CustomStrikeSkewPoint set 
					volaSurfaceDataID = isnull(@iVolaSurfaceDataID,volaSurfaceDataID),
					type = isnull(@iType,type),
					strike = isnull(@fStrike,strike),
					volatility = isnull(@fVolatility,volatility),
					expiryID = isnull(@iExpiryID,@iExpiryID),
					isBasePoint = isnull(@iIsBasePoint,isBasePoint)
					where customStrikeSkewPointID=@iCustomStrikeSkewPointID
	
	   			if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			end
	Commit Transaction

    set @error = 0

finish:
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

set nocount on 
--============================================================================================================
--change DB version
--============================================================================================================
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.17')
	begin
		update DataInfo set keyValue = '3.17' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.17'
	end
