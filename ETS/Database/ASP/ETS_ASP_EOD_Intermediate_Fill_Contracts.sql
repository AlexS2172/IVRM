/*
	Initial fill of ETS ASP EOD intermediate market structure database
*/

begin tran

-- indices
	if exists (select 1
				from SHARKY.ETS_ASP_INST.dbo.ContractView asp_c
					inner join ContractView c on c.symbol = asp_c.symbol and asp_c.contractTypeID = 2
				where c.contractTypeID = 1)
	begin
		select asp_c.symbol
		from SHARKY.ETS_ASP_INST.dbo.ContractView asp_c
			inner join ContractView c on c.symbol = asp_c.symbol and asp_c.contractTypeID = 2
		where c.contractTypeID = 1

		raiserror ('EOD DB already has a indices that are like stocks in ASP DB.', 16, 1)
		rollback tran
		goto finish
	end

	insert into Contract (contractTypeID, symbol, expCalendarID, undPriceProfileID, optPriceProfileID)
	select asp_c.contractTypeID, asp_c.symbol, asp_c.expCalendarID, asp_c.undPriceProfileID, asp_c.optPriceProfileID
	from SHARKY.ETS_ASP_INST.dbo.ContractView asp_c
		left join ContractView c on c.symbol = asp_c.symbol and c.contractTypeID = 1
	where asp_c.contractTypeID = 1
		and c.contractID is null
	if @@error != 0 begin rollback tran goto finish end

	insert into [Index] (indexID)
	select c.contractID
	from Contract c
		left join IndexView i on i.indexID = c.contractID
	where contractTypeID = 1
		and i.indexID is null
	if @@error != 0 begin rollback tran goto finish end

-- futures roots
	insert into FutureRoot (underlyingID, futureRootSymbol, futureLotSize, optionLotSize, matCalendarID)
	select dc.contractID, fr_asp.futureRootSymbol, fr_asp.futureLotSize, fr_asp.optionLotSize, fr_asp.matCalendarID
	from SHARKY.ETS_ASP_INST.dbo.FutureRoot fr_asp
		inner join SHARKY.ETS_ASP_INST.dbo.ContractView sc on sc.contractID = fr_asp.underlyingID
		inner join ContractView dc on dc.symbol = sc.symbol and dc.contractTypeID = sc.contractTypeID
		left join FutureRoot fr on fr.futureRootSymbol = fr_asp.futureRootSymbol
	where
		fr.futureRootID is null
	if @@error != 0 begin rollback tran goto finish end

-- stocks
	if exists (select 1
				from SHARKY.ETS_ASP_INST.dbo.ContractView asp_c
					inner join ContractView c on c.symbol = asp_c.symbol and asp_c.contractTypeID = 1
				where c.contractTypeID = 2)
	begin
		select asp_c.symbol
		from SHARKY.ETS_ASP_INST.dbo.ContractView asp_c
			inner join ContractView c on c.symbol = asp_c.symbol and asp_c.contractTypeID = 1
		where c.contractTypeID = 2

		raiserror ('EOD DB already has a stocks that are like indices in ASP DB.', 16, 1)
		rollback tran
		goto finish
	end

	insert into Contract (contractTypeID, symbol, expCalendarID, undPriceProfileID, optPriceProfileID)
	select asp_c.contractTypeID, asp_c.symbol, asp_c.expCalendarID, asp_c.undPriceProfileID, asp_c.optPriceProfileID
	from SHARKY.ETS_ASP_INST.dbo.ContractView asp_c
		left join ContractView c on c.symbol = asp_c.symbol and c.contractTypeID = 2
	where asp_c.contractTypeID = 2
		and c.contractID is null
	if @@error != 0 begin rollback tran goto finish end

	insert into Stock (stockID)
	select c.contractID
	from ContractView c
		left join StockView s on s.stockID = c.contractID
	where contractTypeID = 2
		and s.stockID is null
	if @@error != 0 begin rollback tran goto finish end

-- price records
	insert into ContractPrice (contractID, exchangeID)
	select c.contractID, null
	from ContractView c
		left join ContractPrice cp on cp.contractID = c.contractID
	where cp.contractID is null
	if @@error != 0 begin rollback tran goto finish end

commit tran
finish:

/*
delete ContractPrice
from ContractPrice cp
	inner join Contract c on c.contractID = cp.contractID and c.contractTypeID = 2
*/
