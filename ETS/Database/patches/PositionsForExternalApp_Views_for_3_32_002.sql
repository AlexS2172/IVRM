/*
	this script create 2 views for Positions
	which work correct only at 3.32.001 version of DB
*/

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PositionsByOptForExtApp_View]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[PositionsByOptForExtApp_View]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PositionsByStockForExtApp_View]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[PositionsByStockForExtApp_View]
GO
SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

Create View dbo.PositionsByOptForExtApp_View
as

/*
	Created by Zaur Nuraliev
	Description: this view is getting information from Trade table special for A.D.
*/

	select 
		convert(varchar,tradeDate,104) as 'Trade Date',
		Trader.acronym as 'Account Acronym',
		UC.Symbol as 'Stock Symbol',
		left(C.symbol,len(C.symbol) - 2) + Datename(month, EC.expiryDate) + Cast(Round(O.strike, 2) as varchar) + Case O.IsCall when 1 then 'C' else 'P' end  as 'Full Option Symbol',
		left(C.symbol,len(C.symbol) - 2) as 'Option Root',
		Upper(Datename(month, EC.expiryDate)) as 'Expiration Month',
		Cast(Round(O.strike,2) as int) as 'Strike',
		Case O.isCall when 1 then 'C' else 'P' end 'Option Type',
		TV.quantity as 'Position',
		Cast(Round(avg(TV.price),2) as varchar) as 'OptionMark',
		case when TV.spotReference is null then 0 else Cast(Round(TV.spotReference,2) as varchar) end 'Underlier/Stock Mark',
		TV.lotSize as 'Contract Multiplier',
		'OPTION' as [OPTION\STOCK],
		convert(varchar,EC.expiryDate,104) as 'Expiration Date'
	from TradeView TV
		join Trader on TV.traderID = Trader.traderID 
		join [Option] O on TV.contractID = O.optionID
		join Contract C on C.contractID = O.optionID
		join Contract UC on O.underlyingContractID = UC.contractID
		join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
	where (TV.isSimulated <> 1 or isSimulated is null)
	group by 	TV.contractID, TV.traderID,TV.tradeDate,Trader.acronym,
				UC.Symbol,C.symbol,EC.expiryDate,O.strike,O.IsCall,TV.price,
				TV.quantity,TV.spotReference,TV.lotSize,TV.contractID


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

Create View dbo.PositionsByStockForExtApp_View
as

/*
	Created by Zaur Nuraliev
	Description: this view is getting information from Trade table special for A.D.
*/

	select 
		convert(varchar,tradeDate,104) as 'Trade Date',
		Trader.acronym as 'Account Acronym',
		C.Symbol as 'Stock Symbol',
		C.Symbol as 'Full Option Symbol',
		null as 'Option Root',
		null as 'Expiration Month',
		null as 'Strike',
		'S' as 'Option Type',
		TV.quantity as 'Position',
		null as 'OptionMark',
		Cast(Round(avg(TV.price),2) as varchar) as 'Underlier/Stock Mark',
		TV.lotSize as 'Contract Multiplier',
		'STOCK' as [OPTION\STOCK],
		null as 'Expiration Date'
		from TradeView TV
			join Trader on TV.traderID = Trader.traderID 
			join Contract C on C.contractID = TV.contractID
		where (TV.isSimulated <> 1 or isSimulated is null) and C.contractTypeID = 2 
		group by	TV.contractID, TV.traderID, TV.tradeDate,
					Trader.acronym,	C.symbol,TV.price,
					TV.quantity,TV.spotReference,TV.lotSize,TV.contractID


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

