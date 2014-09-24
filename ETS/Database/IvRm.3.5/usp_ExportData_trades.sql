SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ExportData_Trades]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ExportData_Trades]
GO

CREATE    proc [dbo].[usp_ExportData_Trades]
as

select
    case t.IsPosition
        when 0 then 'Trade'
        else 'Position'
    end                            as Type,
    t.TradeDate,
    tr.acronym                        as Trader,
    cu.symbol                        as SecuritySymbol,
    c.symbol                        as ContractSymbol,
    
    case c.contractTypeID
        when 1 then 'Spot'
        when 2 then 'Spot'
        when 3 then 'Option'
        when 4 then 'Future'
        else 'UNKOWN'
    end                            as ContractType,
                 
    case t.IsBuy
        when 0 then -1*t.quantity
        when 1 then t.quantity        
    end                            as Quantity,
    convert(decimal(10, 2), t.price)            as Price,
    convert(char(12), o.expiryDate, 110)            as ExpirationDate,
    o.strike                        as Strike,
    
    case o.IsCall
        when 0 then 'Put'            
        when 1 then 'Call'
    end                            as CallPut,
    
    r.BcPrice as BaseContractPrice,
    r.Price as ContractPrice,

    r.TheoPrice as TheoPrice,    
    r.IVola    as MarketVola,
    r.TheoVola as TheoVola,

    r.Delta as Delta,
    r.Vega as Vega,
    r.Theta as Theta,
    r.Rho as Rho,
    r.Gamma    as Gamma,
    r.DeltaVega as DeltaVega,
    r.DeltaTheta as DeltaTheta,
    r.GammaVega as GammaVega,
    r.GammaTheta as GammaTheta,

    r.PriceBid as PriceBid,
    r.PriceBidDate as PriceBidDate,
    r.PriceAsk as PriceAsk,
    r.PriceAskDate as PriceAskDate,
    r.PriceLast as PriceLast,
    r.PriceLastDate as PriceLastDate,
    r.BcPriceBid as BcPriceBid,
    r.BcPriceBidDate as BcPriceBidDate,
    r.BcPriceAsk as BcPriceAsk, 
    r.BcPriceAskDate as BcPriceAskDate,
    r.BcPriceLast as BcPriceLast,
    r.BcPriceLastDate as BcPriceLastDate,
    r.AnalyticsTimestamp as AnalyticsTimestamp
from
    TradeView t
    left join TraderView tr on (t.traderID = tr.traderId)
    left join ContractView c on (c.contractId = t.contractId)
    left join ContractView cu on (cu.contractId = t.underlyingId)
    left join OptionView o on (c.contractId = o.optionId)
    left join ContractHistoricalSnapshot r on (t.tradeID = r.tradeID) where
    t.isPosition = 0

order by    
    tr.acronym,
    t.TradeDate,
    c.symbol
	
SET QUOTED_IDENTIFIER OFF 

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO
