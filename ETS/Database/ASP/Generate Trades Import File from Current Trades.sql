declare @iDeltaGMT int
exec @iDeltaGMT = usp_TimeZoneActimeBias_Get

select
	ltrim(rtrim(t.acronym))
	+ ',' + isnull(ltrim(rtrim(st.strategyName)), '')
	+ ',' + convert(varchar, dateadd(minute, -@iDeltaGMT, tr.tradeDate), 101) + ' ' + convert(varchar, dateadd(minute, -@iDeltaGMT, tr.tradeDate), 108)
	+ ',' + uc.symbol
	+ ',' + case when c.contractTypeID < 3 then 'STK'
				when c.contractTypeID = 3 then 'OPT'
				when c.contractTypeID = 4 then 'FUT'
				when c.contractTypeID = 5 then 'OFT' end
	+ ',' + case when c.contractTypeID = 3 then ort.symbol
				when c.contractTypeID > 3 then fr.futureRootSymbol
				else '' end
	+ ',' + case when c.contractTypeID > 3 then convert(varchar, fmc.expiryDate, 101)
				else '' end
	+ ',' + case when c.contractTypeID = 3 then convert(varchar, ec.expiryDate, 101)
				 when c.contractTypeID = 5 then convert(varchar, fec.expiryDate, 101)
				else '' end
	+ ',' + case when c.contractTypeID = 3 then cast(o.strike as varchar)
				 when c.contractTypeID = 5 then cast(fo.strike as varchar)
				else '' end
	+ ',' + case when c.contractTypeID = 3 then case when o.isCall != 0 then 'C' else 'P' end
				 when c.contractTypeID = 5 then case when fo.isCall != 0 then 'C' else 'P' end
				else '' end
	+ ',' + cast(case when tr.isBuy != 0 then tr.quantity else -tr.quantity end as varchar)
	+ ',' + cast(tr.price as varchar)
from
	TradeView tr
	inner join ContractView c on c.contractID = tr.contractID
	inner join ContractView uc on uc.contractID = tr.underlyingID
	inner join TraderView t on t.traderID = tr.traderID
	left join StrategyView st on st.strategyID = tr.strategyID
	-- regular option
	left join [Option] o on tr.contractID = o.optionID
	left join OptionRoot ort on tr.optionRootID = ort.optionRootID
	left join ExpiryInCalendarView ec on o.expiryID = ec.expiryID and ec.expCalendarID = uc.expCalendarID
	-- futures part
	left join ContractView fc on tr.futureID = fc.contractID
	left join FutureRoot fr on tr.futureRootID = fr.futureRootID
	left join Future f on tr.futureID = f.futureID
	left join ExpiryInCalendarView fmc on f.maturityID = fmc.expiryID and fmc.expCalendarID = fr.matCalendarID
	left join FutureOption fo on tr.contractID = fo.futureOptionID
	left join ExpiryInCalendarView fec on fo.expiryID = fec.expiryID and fec.expCalendarID = fc.expCalendarID
