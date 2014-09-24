/*
	MLine00000606 fixed.
	DB version 3.36.019	
*/

set nocount on
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PositionsByOptForExtApp_View]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[PositionsByOptForExtApp_View]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PositionsByStockForExtApp_View]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[PositionsByStockForExtApp_View]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[NetPositionsForExtApp_View]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[NetPositionsForExtApp_View]
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TradesForExtApp_View]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[TradesForExtApp_View]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE View dbo.PositionsByOptForExtApp_View
as

/*
	Created by Zaur Nuraliev
	Description: this view is getting information from Trade table special for A.D.

	Changed by Mike Malyshkin(08/01/2002)
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
		sum(TV.quantity * TV.lotSize * (case when TV.isBuy <> 0 then 1 else -1 end)) as 'Position',
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

CREATE  View dbo.PositionsByStockForExtApp_View
as

/*
	Created by Zaur Nuraliev
	Description: this view is getting information from Trade table special for A.D.
	Changed by Mike Malyshkin(08/01/2002)
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
		sum(TV.quantity * (case when TV.isBuy <> 0 then 1 else -1 end)) as 'Position',
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

CREATE    View dbo.NetPositionsForExtApp_View
as
/*
	Created by Zaur Nuraliev
	Description: this view is getting information from Trade table special for A.D.

	changed by Mike Malyshkin (08/01/2002)
*/
select top 100 percent *
from
(
		select
				--cast(convert(varchar(10),getdate(),112) as datetime) as [Trade Date],
				cast(convert(varchar(10),TV.TradeDate,112) as datetime) as [Trade Date],
				Trader.acronym as [Account Acronym],
				C.Symbol as [Stock Symbol],
				C.Symbol as [Full Option Symbol],
				null as [Option Root],
				null as [Expiration Month],
				null as [Strike],
				'S' as [Option Type],
				sum(TV.quantity * (case when TV.isBuy <> 0 then 1 else -1 end)) as [Position],
				null as [OptionMark],
				cast(CP.priceClose as decimal(15,2)) as [Underlier/Stock Mark],
				CP.lotSize as [Contract Multiplier],
				'STOCK' as [OPTION\STOCK],
				null as [Expiration Date]
			from ContractView C 
					left join TradeView TV on C.contractID = TV.contractID
					left join Trader on TV.traderID = Trader.traderID 
					left join ContractPrice CP on TV.contractID = CP.contractID and CP.exchangeID is null
			where isnull(TV.isSimulated,0) = 0 
					and C.contractTypeID = 2
					and (C.contractID in (select distinct contractID from Trade))
						--or (C.contractID in (select distinct underlyingContractID from OptionView O join Trade T on O.optionID = T.contractID)))
			group by	C.symbol,
						Trader.acronym,	
						CP.lotSize,
						CP.priceClose,
						cast(convert(varchar(10),TV.TradeDate,112) as datetime)
union
		select
				--cast(convert(varchar(10),getdate(),112) as datetime) as [Trade Date],
				cast(convert(varchar(10),TV.TradeDate,112) as datetime) as [Trade Date],
				Trader.acronym as [Account Acronym],
				C.Symbol as [Stock Symbol],
				C.Symbol as [Full Option Symbol],
				null as [Option Root],
				null as [Expiration Month],
				null as [Strike],
				'S' as [Option Type],
				sum(TV.quantity * (case when TV.isBuy <> 0 then 1 else -1 end)) as [Position],
				null as [OptionMark],
				cast(CP.priceClose as decimal(15,2)) as [Underlier/Stock Mark],
				CP.lotSize as [Contract Multiplier],
				'STOCK' as [OPTION\STOCK],
				null as [Expiration Date]
			from ContractView C 
					left join TradeView TV on C.contractID = TV.contractID
					left join Trader on TV.traderID = Trader.traderID 
					left join ContractPrice CP on TV.contractID = CP.contractID and CP.exchangeID is null
			where isnull(TV.isSimulated,0) = 0 
					and C.contractTypeID = 2
					and C.contractID in (select distinct underlyingContractID from OptionView O join Trade T on O.optionID = T.contractID)
			group by	C.symbol,
						Trader.acronym,	
						CP.lotSize,
						CP.priceClose,
						cast(convert(varchar(10),TV.TradeDate,112) as datetime)
union
		select
			--cast(convert(varchar(10),getdate(),112) as datetime) as [Trade Date],
			cast(convert(varchar(10),TV.TradeDate,112) as datetime) as [Trade Date],
			Trader.acronym as [Account Acronym],
			UC.Symbol as [Stock Symbol],
			left(C.symbol,len(C.symbol) - 2) + cast(Upper(Datename(month, EC.expiryDate)) as char(3)) + Cast(Cast(O.strike as int) as varchar)  + Case O.IsCall when 1 then 'C' else 'P' end  as [Full Option Symbol],
			left(C.symbol,len(C.symbol) - 2) as [Option Root],
			cast(Upper(Datename(month, EC.expiryDate)) as char(3)) as [Expiration Month],
			Cast(O.strike as decimal(15,2)) as [Strike],
			Case O.isCall when 1 then 'C' else 'P' end [Option Type],
			sum(TV.quantity * TV.lotSize * (case when TV.isBuy <> 0 then 1 else -1 end)) as [Position],
			cast (CP.priceClose as decimal(15,2)) as [OptionMark],
			cast(UCP.priceClose as decimal(15,2)) as [Underlier\Stock Mark],
			CP.lotSize as [Contract Multiplier],
			'OPTION' as [OPTION\STOCK],
			EC.expiryDate as [Expiration Date]
		from TradeView TV
			join Trader on TV.traderID = Trader.traderID 
			join [Option] O on TV.contractID = O.optionID
			join Contract C on C.contractID = O.optionID
			join Contract UC on O.underlyingContractID = UC.contractID
			join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			join ContractPrice CP on TV.contractID = CP.contractID and CP.exchangeID is null
			join ContractPrice UCP on O.UnderlyingContractID = UCP.contractID and CP.exchangeID is null
		where isnull(TV.isSimulated,0) = 0
		group by	C.symbol,
					Trader.acronym,
					UC.Symbol,
					EC.expiryDate,
					O.strike,
					O.IsCall,
					CP.lotSize,
					CP.priceClose,
					UCP.priceClose,
					cast(convert(varchar(10),TV.TradeDate,112) as datetime)
) v
order by	v.[Stock Symbol], v.[Option Root],v.[Account Acronym],
			v.[Expiration Date],v.[Strike], v.[Option Type]


GO

CREATE   view dbo.TradesForExtApp_View

as
select top 100 percent *
from
(
			select
				T.execID as [TradeID],
				cast(convert(varchar(10),getdate(),112) as datetime) as [Trade Date],
				null as [Settlement Date],
				Trader.acronym as [Account Acronym],
				C.Symbol as [Base Symbol],
				null as [Option Symbol],
				null as [Option Root],
				null as [Expiration Month],
				null as [Strike],
				'S' as [Option Type],
				T.quantity * (case when T.isBuy <> 0 then 1 else -1 end) as [Net Trade], 
				cast(T.price as decimal(15,2)) as [Trade Price],
				T.lotSize as [Multiplier],
				null as [Trade Code], 
				null as [Exchange Code],
				null as [GRoss Proceeds],
				null as [Commission], 
				null as [Exchange Fee], 
				null as [SEC Fees], 
				null as [OCC Fees], 
				null as [Net Proceeds], 
				convert(varchar,T.tradeDate,108) as [Execution Time], 
				null as [Expiration Date], 
				C.ContractName as [Security Name]
			from TradeView T
				join Trader on T.traderID = Trader.traderID 
				join Contract C on C.contractID = T.contractID
			where isnull(T.isSimulated,0) = 0 
					and C.contractTypeID = 2
					and isnull(isPosition,0) = 0
union
			select
				T.execID as [TradeID],
				cast( convert(varchar(10),getDate(),112) as datetime) as [Trade Date],
				null as [Settlement Date],
				Trader.acronym as [Account Acronym],
				UC.Symbol as [Base Symbol],
				left(C.symbol,len(C.symbol) - 2) + cast(Upper(Datename(month, EC.expiryDate)) as char(3)) + Cast(Cast(O.strike as int) as varchar)  + Case O.IsCall when 1 then 'C' else 'P' end  as [Option Symbol],
				left(C.symbol,len(C.symbol) - 2) as [Option Root],
				cast(Upper(Datename(month, EC.expiryDate)) as char(3)) as [Expiration Month],
				Cast(O.strike as decimal(15,2)) as [Strike],
				Case O.isCall when 1 then 'C' else 'P' end [Option Type],
				T.quantity * (case when T.isBuy <> 0 then 1 else -1 end) as [Net Trade], 
				cast(T.price as decimal(15,2)) as [Trade Price],
				T.lotSize as [Multiplier],
				null as [Trade Code], 
				null as [Exchange Code],
				null as [GRoss Proceeds],
				null as [Commission], 
				null as [Exchange Fee], 
				null as [SEC Fees], 
				null as [OCC Fees], 
				null as [Net Proceeds], 
				convert(varchar,T.tradeDate,108) as [Execution Time], 
				EC.expiryDate as [Expiration Date], 
				UC.ContractName as [Security Name]
			from TradeView T
				join Trader on T.traderID = Trader.traderID 
				join [Option] O on T.contractID = O.optionID
				join Contract C on C.contractID = O.optionID
				join Contract UC on O.underlyingContractID = UC.contractID
				join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			where isnull(T.isSimulated,0) = 0 
					and isnull(isPosition,0) = 0
) v
order by v.[Account Acronym], v.[Base Symbol], 
v.[Expiration Date],v.[Strike], v.[Option Type]


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
--============================================================================================================
--change DB version
--============================================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.36.019')
	begin
		update DataInfo set keyValue = '3.36.019' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.36.019'
	end


