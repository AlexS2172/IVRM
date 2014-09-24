/*
	fix for MLine00000484
	(Hedger shows bad values (>1) for correlation, please see attachment.
	Either IV.com gives us garbage numbers, or we incorrectly interpret them...)
	+ new version implementation (3 segmens number)
	and other non key features
*/
set nocount on
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_DataInfo_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_DataInfo_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_PositionForReconcile_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_PositionForReconcile_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_StockReportDH_Get]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_StockReportDH_Get]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeFeed_New]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeFeed_New]
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE    Procedure dbo.usp_DataInfo_Get
	@iDataInfoID int = null,		-- DataInfo identifier
	@vcKeyName varchar(50) = null,		-- key's name
	@dtDateMin datetime = null,		-- the minimum date for seek
	@dtDateMax datetime = null,		-- the maximum date for seek
	@iTenorValue int = null,		-- tenor's value
	@vcSID varchar(32) = '',		-- session's identifier
	@iStatus int = null out,		-- (1 - unexpected error, 0 - success, 1 - user is not logged on. Access denied)
	@bOnlyMaxDate bit = 0,			-- flag for max date returning
	@vcKeyValue varchar(10) = null	-- the incoming version number
as
/*
	Created by Andrey Sinkin
	Description:	this procedure returns information about databases parameters
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1


	-- Declaring variables
	declare @iTenorID int

	if @iDataInfoID is null and @vcKeyName is not null
		select @iDataInfoID = @iDataInfoID from DataInfo where keyName = @vcKeyName

	if @iDataInfoID in (2, 3) and @iTenorValue is not null
		select @iTenorID = tenorID from Tenor where value = @iTenorValue

	else if @iDataInfoID = 3
		begin
			if @bOnlyMaxDate <> 1
				begin
					if @dtDateMin is null	set @dtDateMin = getdate()-getdate()
					if @dtDateMax is null	set @dtDateMax = getdate()

					select 	@dtDateMin = convert(varchar(8), @dtDateMin, 112), 
							@dtDateMax = convert(varchar(8), @dtDateMax, 112)

					select [date] dtDate 
						from StockDataHistory 
						where tenorID = @iTenorID and [date] between @dtDateMin and @dtDateMax
						group by [date] 
						order by [date]
				end
			else
				select max(date) dtDate from StockDataHistory where tenorID = @iTenorID
		end
	else if @iDataInfoID = 1 and @vcKeyName is null and @vcKeyValue is not null
		begin
			declare @vcDBVersion varchar(150),
					@vcMajor char(2),
					@vcMinor char(2),
					@vcRevision char(3)

			select @vcDBVersion = keyValue from DataInfo where dataInfoID = 1

			if Substring(@vcDBVersion,2,1) = '.' 
				set @vcDBVersion = '0' + @vcDBVersion 

			if Substring(@vcKeyValue,2,1) = '.' 
				set @vcKeyValue = '0' + @vcKeyValue 

			select 	@vcMajor = Left(@vcDBVersion,2),
					@vcMinor = Substring(@vcDBVersion,4,2), 
					@vcRevision = Right(@vcDBVersion,3)

			set @iStatus = 1

			if @vcMajor <> Left(@vcKeyValue,2) set @iStatus = 2
			if @vcMinor <> Substring(@vcKeyValue,4,2) set @iStatus = 2

			if @iStatus = 2 begin select @iStatus as iStatus Return(@iStatus) end

			if @vcRevision <> Right(@vcKeyValue,3) set @iStatus = 3

			select isnull(@iStatus,0) as iStatus
			Return(@iStatus) 
		end
	else if (@iDataInfoID is not null and @vcKeyName is null and @vcKeyValue is null) or @vcKeyName is not null
		select 	dataInfoID as iDataInfoID, 
				keyName as vcKeyName, 
				keyValue as vcKeyValue
			from DataInfo
			where (@iDataInfoID is null or dataInfoID = @iDataInfoID) and (@vcKeyName is null or KeyName = @vcKeyName)

	else if @iDataInfoID is null and @vcKeyName is null and @vcKeyValue is null
		select 	dataInfoID as iDataInfoID, 
				keyName as vcKeyName, 
				keyValue as vcKeyValue
			from DataInfo


	set @error = @@error
finish:

	if @iStatus is null 
		set @iStatus=0

Return(@iStatus)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS OFF 
GO

CREATE PROCEDURE dbo.usp_PositionForReconcile_Get
	@tiByTrader tinyint = 0
AS
	if isnull(@tiByTrader, 0) = 0
	begin
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,

			cp.lotSize as iLotSize,
			cp.priceClose as fPriceClose,

			null as iTraderID,
			null as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty
		
		from TradeView tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
		where c.contractTypeID in (1, 2)
			and isnull(tr.isSimulated, 0) = 0
		group by c.contractID, c.contractTypeID, c.Symbol, cp.lotSize, cp.priceClose
		
		union
	
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,

			cp.lotSize as iLotSize,
			cp.priceClose as fPriceClose,

			null as iTraderID,
			null as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty

		from TradeView tr 
			inner join ContractView C on tr.contractID = C.contractID
			inner join [Option] o on c.contractID = o.optionID
			inner join Contract UC on o.underlyingContractID = UC.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
	
		where c.contractTypeID = 3
			and isnull(tr.isSimulated, 0) = 0
		group by C.contractID, C.contractTypeID, C.Symbol, 
			EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
			cp.lotSize, cp.priceClose
	
		order by vcSymbol
	end
	else
	begin
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,

			cp.lotSize as iLotSize,
			cp.priceClose as fPriceClose,

			trdr.TraderID as iTraderID,
			trdr.acronym as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty
		
		from TradeView tr 
			inner join ContractView c  on tr.contractID = c.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join TraderView trdr on tr.traderID = trdr.traderID
		where c.contractTypeID in (1, 2)
			and isnull(tr.isSimulated, 0) = 0
		group by c.contractID, c.contractTypeID, c.Symbol, cp.lotSize, cp.priceClose, trdr.TraderID, trdr.acronym
		
		union
	
		select 
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,

			cp.lotSize as iLotSize,
			cp.priceClose as fPriceClose,

			trdr.TraderID as iTraderID,
			trdr.acronym as vcAcronym,
			sum(tr.quantity * (case when tr.isBuy <> 0 then 1 else -1 end)) as iQty

		from TradeView tr 
			inner join ContractView C on tr.contractID = C.contractID
			inner join [Option] o on c.contractID = o.optionID
			inner join Contract UC on o.underlyingContractID = UC.contractID
			inner join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			inner join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			left join TraderView trdr on tr.traderID = trdr.traderID
	
		where c.contractTypeID = 3
			and isnull(tr.isSimulated, 0) = 0
		group by C.contractID, C.contractTypeID, C.Symbol, 
			EC.expiryDate, o.strike, o.isCall, uc.contractID, uc.contractTypeID, uc.Symbol,
			cp.lotSize, cp.priceClose, trdr.TraderID, trdr.acronym
	
		order by vcSymbol
	end
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO


CREATE      Procedure dbo.usp_StockReportDH_Get
	@vcSymbolList varchar(8000) = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from <table_name, sysname,table_name> table
*/
	------------------------------------------------
	set nocount on
	set ansi_warnings off
	------------------------------------------------
	declare @error int		set @error = -1
	Declare @iSPX_ID int,
			@iNDX_ID int,
			@iOEX_ID int,
			@iDJX_ID int

	select @iSPX_ID=ContractID from Contract where symbol='SPX' and contractTypeID=1
	select @iNDX_ID=ContractID from Contract where symbol='NDX' and contractTypeID=1
	select @iOEX_ID=ContractID from Contract where symbol='OEX' and contractTypeID=1
	select @iDJX_ID=ContractID from Contract where symbol='DJX' and contractTypeID=1

	declare @SDH_Dt datetime
		select @SDH_Dt=max([date]) from StockDataHistory where tenorID=8

	-- check if data not exists in StockDataHistory
	select  @SDH_Dt= isnull(@SDH_Dt, 0)
	select @iSPX_ID = isnull(@iSPX_ID, 0)
	select @iNDX_ID = isnull(@iNDX_ID, 0)
	select @iOEX_ID = isnull(@iOEX_ID, 0)
	select @iDJX_ID = isnull(@iDJX_ID, 0)

	if @vcSymbolList is null 
		begin
			select
				Contract.Symbol,
				CP.priceClose,
				SDH.currentHV as HV,
				SDH.IVMean as IV,
				sum(case when IndexD.indexID=@iSPX_ID then correlationPrice else null end) as CorrSPX,
				sum(case when IndexD.indexID=@iSPX_ID then correlationVola else null end) as VCorrSPX,
				sum(case when IndexD.indexID=@iNDX_ID then correlationPrice else null end) as CorrNDX,
				sum(case when IndexD.indexID=@iNDX_ID then correlationVola else null end) as VCorrNDX,
				sum(case when IndexD.indexID=@iOEX_ID then correlationPrice else null end) as CorrOEX,
				sum(case when IndexD.indexID=@iOEX_ID then correlationVola else null end) as VCorrOEX,
				sum(case when IndexD.indexID=@iDJX_ID then correlationPrice else null end) as CorrDJX,
				sum(case when IndexD.indexID=@iDJX_ID then correlationVola else null end) as VCorrDJX
				from ContractView Contract
					left join StockDataHistory SDH on Contract.contractID=SDH.stockID and SDH.[date]=@SDH_Dt and SDH.tenorID=8
					left join IndexDefinition IndexD on Contract.contractID=IndexD.stockID
					left join ContractPrice CP on contract.contractID=CP.contractID and CP.exchangeID is null 
				where contract.contractTypeID=2
				group by Contract.Symbol,CP.priceClose,SDH.currentHV,SDH.IVMean
		end 
	else 
		begin
			declare @ExecString varchar(8000)
			select @ExecString='select
						Contract.Symbol,
						CP.priceClose,
						SDH.currentHV as HV,
						SDH.IVMean as IV,
						sum(case when IndexD.indexID=' + convert(varchar(15),@iSPX_ID) + ' then correlationPrice else null end) as CorrSPX,
						sum(case when IndexD.indexID=' + convert(varchar(15),@iSPX_ID) + ' then correlationVola else null end) as VCorrSPX,
						sum(case when IndexD.indexID=' + convert(varchar(15),@iNDX_ID) + ' then correlationPrice else null end) as CorrNDX,
						sum(case when IndexD.indexID=' + convert(varchar(15),@iNDX_ID) + ' then correlationVola else null end) as VCorrNDX,
						sum(case when IndexD.indexID=' + convert(varchar(15),@iOEX_ID) + ' then correlationPrice else null end) as CorrOEX,
						sum(case when IndexD.indexID=' + convert(varchar(15),@iOEX_ID) + ' then correlationVola else null end) as VCorrOEX,
						sum(case when IndexD.indexID=' + convert(varchar(15),@iDJX_ID) + ' then correlationPrice else null end) as CorrDJX,
						sum(case when IndexD.indexID=' + convert(varchar(15),@iDJX_ID) + ' then correlationVola else null end) as VCorrDJX '
						+'from ContractView Contract
									left join StockDataHistory SDH on Contract.contractID=SDH.stockID and convert(varchar(15),SDH.[date],112)=' + convert(varchar(15),@SDH_Dt,112) + ' and SDH.tenorID=8 
									left join IndexDefinitionView IndexD on Contract.contractID=IndexD.stockID 
									left join ContractPrice CP on contract.contractID=CP.contractID and CP.exchangeID is null  
									where Symbol in ('	+ @vcSymbolList + ') and contract.contractTypeID=2' 
		
						+ ' group by Contract.Symbol,CP.priceClose,SDH.currentHV,SDH.IVMean'
			exec (@ExecString)
		--select @ExecString
		end


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


CREATE  PROCEDURE dbo.usp_TradeFeed_New
	@vcTraderAcronym varchar(12),
	@vcExecID varchar(20),
	@tiIsBuy tinyint,  
	@iQuantity int,
	@fPrice float,
	@dtTradeDate datetime,
	@vcContractSymbol varchar(8),
	@iContractTypeID int,
	@vcUnderlyingSymbol varchar(8) = null,
	@tiIsCall tinyint = null,
	@fStrike float = null,
	@dtExpiry datetime = null,
	@iTradeLotSize int = null
AS
/*
	Created by Zaur Nuraliev
	Description: this procedure is save information into table_name table
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1

	if exists(select * from Trade where execID = @vcExecID and actionID > 0 and Convert(varchar(12),@dtTradeDate,112) = Convert(varchar(12),tradeDate,112))
		begin
			--Raiserror('Record with same execID alreadty exists in DB!',16,1)
			Select null as iSeqNum
			Return(-1)

		end

	Begin Transaction

	Declare @iTradeContractID int, @iStructureMissed int
	set @iStructureMissed = 0

	--check underlying symbol
	Declare @iUnderlyingContractID int

		if @vcUnderlyingSymbol is null and  @iContractTypeID <3 
			set @vcUnderlyingSymbol = @vcContractSymbol

	select @iUnderlyingContractID = contractID from ContractView where symbol = @vcUnderlyingSymbol and contractTypeID in(1,2)


	if @iUnderlyingContractID is null
	--add new stock\index
		begin
			insert into Contract(symbol,contractTypeID,expCalendarID) values(@vcUnderlyingSymbol, 2, 1)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iUnderlyingContractID = @@identity

			insert into Stock (stockID,isActive) values(@iUnderlyingContractID,1)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			if @iTradeLotSize is null set @iTradeLotSize = 1
			insert into ContractPrice (contractID,exchangeID,lotSize) values(@iUnderlyingContractID,null,@iTradeLotSize)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

			set @iStructureMissed = 1

/*			insert into Contract(symbol,contractTypeID,expCalendarID) values(@vcUnderlyingSymbol,@iContractTypeID,1)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
			set @iUnderlyingContractID = @@identity
			if @iContractTypeID = 1
				begin
					insert into [Index](indexID) values(@iUnderlyingContractID)
						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else if @iContractTypeID = 2
				begin
					insert into Stock (stockID) values(@iUnderlyingContractID)
						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			if @iContractTypeID < 3 
				begin	
					if @iTradeLotSize is null set @iTradeLotSize = 1
					insert into ContractPrice (contractID,exchangeID,lotSize) values(@iUnderlyingContractID,null,@iTradeLotSize)
						if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				end
			else
				insert into ContractPrice (contractID,exchangeID) values(@iUnderlyingContractID,null)
					if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end*/
		end
	else
		begin
			update Stock set isActive = 1 where stockID = @iUnderlyingContractID
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
		end

	--check Option
	if @iContractTypeID = 3
		begin
			Declare @iOptionID int,
					@iExpiryID int
	
			set @iExpiryID = (year(@dtExpiry) - 2000) * 12 + month(@dtExpiry)
			set @fStrike = round(@fStrike, 3)
	
			select @iOptionID = optionID
				from OptionView (nolock)
				where underlyingContractID = @iUnderlyingContractID and 
					isCall = @tiIsCall and abs(strike - @fStrike) < 0.009 and expiryID = @iExpiryID and actionID<>3
	
			if @iOptionID is null
			--create new option
				begin
					if isnull(@vcContractSymbol, '') <> ''
					begin
						Declare @vcContractName varchar(255)
						  	set @vcContractName = @vcUnderlyingSymbol + ' ' + datename(month, @dtExpiry) + 
										' ' + datename(year, @dtExpiry) + ' ' + case @tiIsCall when 1 then 'Call' else 'Put' end +
											 ' ' + cast(round(@fStrike, 2) as varchar)
		
						insert into Contract (contractTypeID, symbol, contractName) values (3, @vcContractSymbol,@vcContractName)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
							set @iOptionID = @@identity
				
						insert into [Option] (optionID, underlyingContractID, isCall, strike, expiryID)
							 values (@iOptionID, @iUnderlyingContractID, @tiIsCall, @fStrike, @iExpiryID)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				
						-- make record with null exchange for this Option
						insert into ContractPrice(contractID,exchangeID,lotSize) values(@iOptionID,null,@iTradeLotSize)
							if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
					end
					else
					begin
						Select null as iSeqNum
						Return(-2)
					end
				end

			set @iStructureMissed = @iStructureMissed + 2
			set @iTradeContractID = @iOptionID
		end
		else
			set @iTradeContractID = @iUnderlyingContractID


	--work with Trader
	Declare @iTraderID int
		select @iTraderID = traderID from TraderView where acronym = @vcTraderAcronym

	if @iTraderID is null
		begin
			insert into Trader(acronym,traderName) values(@vcTraderAcronym,@vcTraderAcronym)
				if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end
				set @iTraderID = @@identity
		end

	--create new trade
	if @iTradeLotSize is null
		select @iTradeLotSize = lotSize from ContractPrice 
			where exchangeID is null and 
					(contractID = @iOptionID and @iContractTypeID = 3) or
					(contractID = @iUnderlyingContractID and @iContractTypeID < 3)

	if @iTradeLotSize is null
		if @iContractTypeID = 3 
			set @iTradeLotSize = 100
		else if @iContractTypeID < 3 
			set @iTradeLotSize = 1

	Declare @iNewTradeID int
		select @iNewTradeID = max(tradeID) + 1 from Trade

	if @iNewTradeID is null 
			set @iNewTradeID = 1
	
	insert into Trade (
		execID,
		tradeID,
		actionID, 
		contractID,
		isClose, 
		tradeDate, 
		quantity, 
		price, 
		traderID, 
		isBuy, 
		isPosition,
		lotSize,
		isSimulated,
		brokerCommissionID,
		clearingBrokerCommissionID)
	values (
		@vcExecID,
		@iNewTradeID,
		1,
		@iTradeContractID, 
		0, 
		@dtTradeDate, 
		@iQuantity, 
		@fPrice, 
		@iTraderID, 
		@tiIsBuy, 
		0,
		@iTradeLotSize,
		0,
		null,
		null)

	Declare @iSeqNum int 
		set @iSeqNum = @@identity

		if @@error <> 0 begin set @error = 2 if @@TranCount = 1 Rollback Transaction goto finish end

	Commit Transaction

 --if all OK
	if @iContractTypeID < 3 
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.Symbol as vcUnderlyingSymbol,
			null as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			tr.lotSize as iTradeLotSize,
			cp.lotSize as iUnderlyingLotSize,
			cp.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated,
			null as fPriceClose,
			null as fUnderlyingPriceClose,
			@iStructureMissed as iStructureMissed
		from Trade tr 
			join Contract C  on tr.contractID = c.contractID
			join ContractPrice cp on c.contractID = cp.contractID and cp.exchangeID is null
			left join StockView s on c.contractID = s.stockID
			left join IndexView i on c.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where tr.seqNum = @iSeqNum

	else if @iContractTypeID =3
		select
			tr.seqNum as iSeqNum,
			tr.actionDate as dtActionDate,
			tr.tradeID as iTradeID,
			tr.actionID as tiActionID,
			tr.isClose as tiIsClose,
			tr.tradeDate as dtTradeDate,
			tr.quantity as iQuantity,
			tr.price as fPrice,
			tr.isBuy as tiIsBuy,
			tr.bookID as iBookID,     
			tr.strategyID as iStrategyID, 
			tr.traderID as iTraderID,
			tr.brokerID as iBrokerID,
			tr.clearingBrokerID as iClearingBrokerID,
			tr.BrokerCommissionID as iBrokerCommissionID,
			tr.clearingBrokerCommissionID as iClearingBrokerCommissionID,
			cv1.CommissionValue as fBrokerCommission,
			cv2.CommissionValue as fClearingBrokerCommission,
			tr.tradedIV as fTradedIV,
			tr.spotReference as fSpotReference,
			c.contractID as iContractID,
			c.contractTypeID as iContractTypeID,
			c.Symbol as vcSymbol,
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
			i.yield as fYield,
			EC.expiryDate as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.Symbol as vcUnderlyingSymbol,
			cp.priceTheo as fPriceTheoClose,
			tr.isPosition as tiIsPosition,
			tr.lotSize as iTradeLotSize,
			ucp.lotSize as iUnderlyingLotSize,
			cp.lotSize as iLotSize,
			tr.isSimulated as tiIsSimulated,
			CP.PriceClose as fPriceClose,
			UCP.priceClose as fUnderlyingPriceClose,
			@iStructureMissed as iStructureMissed
		from Trade tr 
			join Contract C  on tr.contractID = c.contractID
			inner join [Option] O on C.contractID = O.optionID
			inner join Contract UC on O.underlyingContractID = UC.contractID
			inner join ContractPrice CP on C.contractID = CP.contractID and CP.exchangeID is null
			inner join ContractPrice UCP on o.underlyingContractID = ucp.contractID and ucp.exchangeID is null
			join ExpiryInCalendar EC on UC.expCalendarID = EC.expCalendarID and O.expiryID = EC.expiryID
			left join StockView s on uc.contractID = s.stockID
			left join [IndexView] i on uc.contractID = i.indexID
			left join CommissionView cv1 on tr.brokerCommissionID=cv1.CommissionID
			left join CommissionView cv2 on tr.clearingBrokerCommissionID=cv2.CommissionID
		where tr.seqNum = @iSeqNum

    set @error = 0

finish:
Return(@error)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--============================================================================================================
--change version of DB
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.32.000')
	begin
		update DataInfo set keyValue = '3.32.000' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.32.000'
	end
