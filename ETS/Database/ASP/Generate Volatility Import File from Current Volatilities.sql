select c.symbol
	+ ',' + convert(varchar, eic.expiryDate, 101)
	+ ',' + cast(cssp.strike as varchar)
	+ ',' + cast(round(cssp.volatility, 4) as varchar)
from CustomStrikeSkewPoint cssp
	inner join VolaSurfaceData vsd on cssp.volaSurfaceDataID = vsd.volaSurfaceDataID
	inner join Contract c on c.contractID = vsd.contractID
	inner join ExpiryInCalendar eic on eic.expCalendarID = c.expCalendarID and eic.expiryID = cssp.expiryID
where
	c.actionID < 3
	and c.contractTypeID < 3 
	and vsd.isDefaultSurface = 1
	and datediff(mm, getdate(), eic.expiryDate) >= 0
	--and c.symbol in ('MSFT', 'IBM', 'AAPL', 'SPX', 'NDX', 'ORCL', 'A', 'AA', 'INTC', 'INTL', 'DJX', 'SOX', 'QQQ')
order by c.symbol, eic.expiryDate, cssp.strike