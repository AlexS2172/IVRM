/*
	patch contain view for net positions
	DB version: 3.37.028
*/
set nocount on
GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[NetPositionsForExtApp_View]') and OBJECTPROPERTY(id, N'IsView') = 1)
drop view [dbo].[NetPositionsForExtApp_View]
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
CREATE View dbo.NetPositionsForExtApp_View
as
/*
	Created by Zaur Nuraliev
	Description: this view is getting information from Trade table special for A.D.

	Changed by Mike Malyshkin (08/01/2002)
	Changed by Zaur Nuraliev (08/08/2002)
	Changed by Zaur Nuraliev (27/08/2002)
*/
select top 100 percent *
from
(
		select
				--cast(convert(varchar(10),getdate(),112) as datetime) as [Trade Date],
				cast(convert(varchar(10),TV.TradeDate,112) as datetime) as [Trade Date],
--				Trader.acronym as [Account Acronym],
				isnull(Trader.acronym,'DUMMY') as [Account Acronym],
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
				null as [Expiration Date],
				isnull(TV.isSimulated,0) as [isSimulated]
			from ContractView C 
					left join TradeView TV on C.contractID = TV.contractID
					left join Trader on TV.traderID = Trader.traderID 
					left join ContractPrice CP on TV.contractID = CP.contractID and CP.exchangeID is null
			where --isnull(TV.isSimulated,0) = 0 
					C.contractTypeID = 2
					and (C.contractID in (select distinct contractID from Trade))
						--or (C.contractID in (select distinct underlyingContractID from OptionView O join Trade T on O.optionID = T.contractID)))
			group by	C.symbol,
						Trader.acronym,	
						CP.lotSize,
						CP.priceClose,
						cast(convert(varchar(10),TV.TradeDate,112) as datetime),
						TV.isSimulated
union
		select
				--cast(convert(varchar(10),getdate(),112) as datetime) as [Trade Date],
				cast(convert(varchar(10),TV.TradeDate,112) as datetime) as [Trade Date],
--				Trader.acronym as [Account Acronym],
				isnull(Trader.acronym,'DUMMY') as [Account Acronym],
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
				null as [Expiration Date],
				isnull(TV.isSimulated,0) as [isSimulated]
			from ContractView C 
					left join TradeView TV on C.contractID = TV.contractID
					left join Trader on TV.traderID = Trader.traderID 
					left join ContractPrice CP on TV.contractID = CP.contractID and CP.exchangeID is null
			where --isnull(TV.isSimulated,0) = 0 
					C.contractTypeID = 2
					and C.contractID in (select distinct underlyingContractID from OptionView O join Trade T on O.optionID = T.contractID)
			group by	C.symbol,
						Trader.acronym,	
						CP.lotSize,
						CP.priceClose,
						cast(convert(varchar(10),TV.TradeDate,112) as datetime),
						TV.isSimulated
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
			--sum(TV.quantity * TV.lotSize * (case when TV.isBuy <> 0 then 1 else -1 end)) as [Position],
			sum(TV.quantity * (case when TV.isBuy <> 0 then 1 else -1 end)) as [Position],
			cast (CP.priceClose as decimal(15,2)) as [OptionMark],
			cast(UCP.priceClose as decimal(15,2)) as [Underlier\Stock Mark],
			CP.lotSize as [Contract Multiplier],
			'OPTION' as [OPTION\STOCK],
			EC.expiryDate as [Expiration Date],
			isnull(TV.isSimulated,0) as [isSimulated]
		from TradeView TV
			join Trader on TV.traderID = Trader.traderID 
			join [Option] O on TV.contractID = O.optionID
			join Contract C on C.contractID = O.optionID
			join Contract UC on O.underlyingContractID = UC.contractID
			join ExpiryInCalendar EC on O.expiryID = EC.expiryID and EC.expCalendarID = UC.expCalendarID
			join ContractPrice CP on TV.contractID = CP.contractID and CP.exchangeID is null
			join ContractPrice UCP on O.UnderlyingContractID = UCP.contractID and CP.exchangeID is null
		where isnull(TV.isSimulated,0) = 0 and EC.expiryDate >= GetDate()
		group by	C.symbol,
					Trader.acronym,
					UC.Symbol,
					EC.expiryDate,
					O.strike,
					O.IsCall,
					CP.lotSize,
					CP.priceClose,
					UCP.priceClose,
					cast(convert(varchar(10),TV.TradeDate,112) as datetime),
					TV.isSimulated) v
order by	v.[Stock Symbol], v.[Option Root],v.[Account Acronym],
			v.[Expiration Date],v.[Strike], v.[Option Type]





GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

--===================================================================================
--change DB version
--===================================================================================
if not exists(select * from DataInfo where DataInfoID = 1 and keyValue = '3.37.028')
	begin
		update DataInfo set keyValue = '3.37.028' where dataInfoID = 1
		print 'DB VERSION WAS CHANGED TO 3.37.028'
	end
