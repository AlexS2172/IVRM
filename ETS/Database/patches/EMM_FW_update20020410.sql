/*
	fix for Defect MLine00000249  ETS Back Office
*/


SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

ALTER        proc dbo.usp_Stock_Del
        @iStockID int,
        @bisRemovePermanent bit = 0
  as
/*
	Created by Zaur Nuraliev
	Description: this procedure is delete information from Stock table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	
	if exists (select * from TradeView(nolock) where contractID = @iStockID)
	begin
		Raiserror ('Error. Can''t delete Stock because of the existing Trade', 16, 1)
		Return (-1)
	end 
	if exists(select * from [Option](nolock) where underlyingContractID=@iStockID)
		begin
			Create table #Options_for_delete(OptionID int)
		
			insert into #Options_for_delete
				select optionID from [Option](nolock) where underlyingContractID=@iStockID
		
			if exists (select tradeID from TradeView where contractID in (select optionID from #Options_for_delete))
				begin
					Raiserror ('Error. Can''t delete Stock because of the existing Trade by Stock Options', 16, 1)
					Return (-1)
				end 
		end
	Begin Transaction
		if object_ID('tempdb..#Options_for_delete') is not null
			begin
				exec usp_Option_Del @bisRemovePermanent = @bisRemovePermanent
			end

		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID = @iStockID)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID = @iStockID
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractExchangeLink where contractID = @iStockID
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	    delete ModelParameter where contractID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	
        update ContractInGroup set actionID=3 where contractID = @iStockID 
        --delete ContractInGroup where contractID = @iStockID 
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockDataHistory where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockCorrelation where stockID1 = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete StockCorrelation where stockID2 = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

        update IndexDefinition set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		update TraderContract set actionID=3 where contractID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Stock set actionID=3 where stockID = @iStockID  
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

        --delete ContractPriceHistory where contractID = @iStockID
		--	if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        delete ContractPrice where contractID = @iStockID

			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
        update Contract set actionID=3 where contractID = @iStockID
			if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end

		if (@bisRemovePermanent = 1) 
			begin
	        	delete IndexDefinition where stockID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
				delete TraderContract where contractID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
		       	delete Stock where stockID = @iStockID  
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
	        	delete Contract where contractID = @iStockID
					if @@error <> 0 begin set @error = 1 if @@TranCount = 1 Rollback Transaction goto finish end
			end 
  
		set @error = @@error
	Commit Transaction
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

ALTER          Procedure dbo.usp_Option_Del
	@iOptionID int = null, 	-- contract identifier
	@bisRemovePermanent bit = 0
as
/*
	Create by Zaur Nuraliev
	Description: It deletes Option data from Option and Contract tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if object_ID('tempdb..#Options_for_delete') is null
		begin	
			Create table #Options_for_delete(OptionID int)
		end
	
	if @iOptionID is not null
		begin
			if exists (select tradeID from Trade where contractID = @iOptionID)
				begin
					Raiserror ('Error. Can''t delete [Option] because of the existing Trade', 16, 1)
					Return (-1)
				end   
			insert into #Options_for_delete
				select @iOptionID
		end
	Begin Transaction DelOpt
		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete ContractPriceHistory where ContractID in (select OptionID from #Options_for_delete)
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ModelParameter where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update ContractInGroup set actionID=3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update [Option] set actionID=3 where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		update Contract set actionID=3 where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete Trade where contractID = @iOptionID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		if @bisRemovePermanent = 1
			begin
				delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
				delete [Option] where OptionID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
				delete Contract where contractID in (select OptionID from #Options_for_delete)
					if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
			end

	Commit Transaction DelOpt

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

ALTER  Procedure dbo.usp_OptionPermanent_Del
	@iOptionID int = null 	-- contract identifier
as
/*
	Create by Zaur Nuraliev
	Description: It deletes Option data from Option and Contract tables
*/
	-------------------------------------------------
	set nocount on
	-------------------------------------------------
	declare @error int		set @error = -1
	if object_ID('tempdb..#Options_for_delete') is null
		begin	
			Create table #Options_for_delete(OptionID int)
		end
	
	if @iOptionID is not null
		begin
			if exists (select tradeID from Trade where contractID = @iOptionID)
				begin
					Raiserror ('Error. Can''t delete [Option] because of the existing Trade', 16, 1)
					Return (-1)
				end   
			insert into #Options_for_delete
				select @iOptionID
		end
	Begin Transaction DelOpt
		delete OptionDataCache where optionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractPrice where ContractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete ContractPriceHistory where ContractID in (select OptionID from #Options_for_delete)
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractExchangeLink where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractMarketData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ModelParameter where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from ContractInGroup where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from CustomStrikeSkewPoint where volaSurfaceDataID in 
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where volaSurfaceDataID in
			(select volaSurfaceDataID from VolaSurfaceData where contractID in (select OptionID from #Options_for_delete))
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete from NextDaySurface where contractID in (select OptionID from #Options_for_delete)
		if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete VolaSurfaceData where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete HistRangesCache where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete [Option] where OptionID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		delete Contract where contractID in (select OptionID from #Options_for_delete)
			if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
		--delete Trade where contractID = @iOptionID
		--	if @@error <> 0 begin set @error = 3 if @@TranCount = 1 Rollback Transaction DelOpt goto finish end
	Commit Transaction DelOpt

finish:
Return(@error)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS OFF 
GO

ALTER   PROCEDURE dbo.usp_ContractInGroup_Get
	@iGroupID int = null,
	@iContractID int = null,
	@dtActionDate datetime = null
as
/*
	Created by Zaur Nuraliev
	Description: this procedure is getting information from ContractInGroup table
*/
	------------------------------------------------
	set nocount on
	------------------------------------------------
	declare @error int		set @error = -1
	select 	groupID iGroupID, 
			contractID iContractID,
			actionDate as dtActionDate,
			actionID as tiActionID
		from ContractInGroupView
		where ((@iGroupID is null or @iGroupID is not null and groupID = @iGroupID)
			and (@iContractID is null or @iContractID is not null and contractID = @iContractID))
			and (actionDate>=@dtActionDate or @dtActionDate is null)
			and (@dtActionDate is not null or @dtActionDate is null and actionID<3)
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

ALTER     proc dbo.usp_ContractForTrade_Get
	@iContractID int = null,
	@iUnderlyingID int = null,
	@tiNewOnly tinyint = 0
as
	declare @dtToday datetime
	set @dtToday = cast(floor(cast(getdate() as float)) as datetime)
	select
			-- common
			C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName, 
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceClose as fPriceClose,
			CP.lotSize as iLotSize,
			-- stock
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
		
			-- index
			i.yield as fYield,
		
			-- option
			null as dtExpiry,
			null as fStrike,
			null as bIsCall,
			c.contractID as iUnderlyingContractID,
			c.contractTypeID as iUnderlyingContractTypeID,
			c.symbol as vcUnderlyingSymbol,
			cp.priceBid as fUnderlyingPriceBid,
			cp.priceAsk as fUnderlyingPriceAsk,
			cp.priceClose as fUnderlyingPriceClose,
			cp.lotSize as iUnderlyingLotSize
		from ContractView c  
			left join StockView S on S.stockID = C.contractID
			left join IndexView I on I.indexID = C.contractID
			left join ContractPrice CP on C.contractID = CP.contractID
		where	
			(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
			and (@iUnderlyingID is Null or @iUnderlyingID is not Null and c.contractID = @iUnderlyingID)
			and c.contractTypeID in (1, 2)
			and CP.exchangeID is null
	union
	select	
			-- common
			C.contractID as iContractID,
			C.contractTypeID as iContractTypeID,
			C.symbol as vcSymbol,
			C.contractName as vcContractName, 
			CP.priceBid as fPriceBid,
			CP.priceAsk as fPriceAsk,
			CP.priceClose as fPriceClose,
			CP.lotSize as iLotSize,
			-- stock
			s.divFreq as iDivFreq,
			s.divDate as dtDivDate,
			s.divAmt as fDivAmt,
		
			-- index
			i.yield as fYield,
		
			-- option
			o.expiry as dtExpiry,
			o.strike as fStrike,
			o.isCall as bIsCall,
			uc.contractID as iUnderlyingContractID,
			uc.contractTypeID as iUnderlyingContractTypeID,
			uc.symbol as vcUnderlyingSymbol,
			ucp.priceBid as fUnderlyingPriceBid,
			ucp.priceAsk as fUnderlyingPriceAsk,
			ucp.priceClose as fPriceClose,
			ucp.lotSize as iUnderlyingLotSize
	from ContractView c  
			left join OptionView O on O.optionID = C.contractID
			left join Contract uc on o.underlyingContractID = uc.contractID
			left join ContractPrice CP on C.contractID = CP.contractID
			left join ContractPrice ucp on uc.contractID = ucp.contractID
			left join StockView s on uc.contractID = s.stockID
			left join IndexView i on uc.contractID = i.indexID			
	where
			(@iContractID is Null or @iContractID is not Null and c.contractID = @iContractID)
			and (@iUnderlyingID is Null or @iUnderlyingID is not Null and O.underlyingContractID = @iUnderlyingID)
			and (@tiNewOnly = 0 or @tiNewOnly = 1 and O.expiry >= @dtToday)
			and c.contractTypeID = 3
			and CP.exchangeID is null
			and ucp.exchangeID is null
	order by C.symbol
	Return(0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

Alter  proc dbo.usp_ContractSymbol_Get
	  @iGroupID int,
	  @bIsAll bit,
	  @bStockOnly bit,   
	  @bIndexesOnly bit,
	  @bOptionsOnly bit 
	   
  as
	 if (@iGroupID is Null)
		if (@bIsAll is not Null)  and	(@bIsAll > 0)
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID, 
					C.symbol as vcSymbol, S.calcOptionType as tiCalcOptionType
				from ContractView c  
						join Stock S on S.stockID = C.contractID
				where ((@bStockOnly is not Null) and (@bStockOnly > 0))
				   and c.contractTypeID = 2
		  union
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID, 
					C.symbol as vcSymbol, I.calcOptionType as tiCalcOptionType
				 from ContractView c  
						join IndexView I on I.indexID = C.contractID
				 where ((@bIndexesOnly is not Null) and (@bIndexesOnly > 0)) 
						 and c.contractTypeID = 1
		  union 
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID, 
					C.symbol as vcSymbol, null as tiCalcOptionType
				 from ContractView c 
						join OptionView O on O.optionID = C.contractID
				  where ((@bOptionsOnly is not Null) and (@bOptionsOnly > 0)) 
						  and c.contractTypeID in (3, 4)
			 order by c.symbol	 
	   else
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol, S.calcOptionType as tiCalcOptionType
					from ContractView c  
							join Stock S on s.stockID = c.contractID
					where ((@bStockOnly is not Null) and (@bStockOnly > 0)) 
						   and c.contractTypeID = 2
						   and not exists (select * from ContractInGroup where contractID = c.contractID)
		  union
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol, I.calcOptionType as tiCalcOptionType
					from ContractView c  
							join IndexView I on I.indexID = C.contractID
					where ((@bIndexesOnly is not Null) and (@bIndexesOnly > 0)) 
						 and C.contractTypeID = 1
						 and not exists (select * from ContractInGroup where contractID = c.contractID)
					 order by C.symbol 
	  else
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol, S.calcOptionType as tiCalcOptionType
				from ContractInGroup CIG
						join ContractView c  on C.contractID = CIG.contractID
						join Stock S on S.stockID = C.contractID
				where CIG.groupID = @iGroupID
					   and ((@bStockOnly is not Null) and (@bStockOnly > 0))	
					   and c.contractTypeID = 2
		 union
		   select	C.contractID as iContractID, C.contractTypeID as iContractTypeID,
					C.symbol as vcSymbol, I.calcOptionType as tiCalcOptionType
				 from ContractInGroup CIG
						join ContractView c  on C.contractID = CIG.contractID
						join IndexView I on I.indexID = C.contractID
				 where CIG.groupID = @iGroupID
						 and ((@bIndexesOnly is not Null) and (@bIndexesOnly > 0)) 
						 and c.contractTypeID = 1
				 order by c.symbol			 
Return (0)

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

set nocount on 
GO
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.09')
	begin
		update DataInfo set keyValue = '3.09' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.09'
	end
GO