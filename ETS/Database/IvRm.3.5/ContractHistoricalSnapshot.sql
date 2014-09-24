
if exists (select * from dbo.sysobjects where id = object_id(N'[TradeRisks]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [TradeRisks]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[ContractHistoricalSnapshot]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[ContractHistoricalSnapshot]
GO

create table [dbo].[ContractHistoricalSnapshot] (
	[ID] [int] IDENTITY (1, 1) not null,
	[TradeID] [int] not null,
	[ContractID] [int] null,
	[TheoPrice] [float] null,
	[Alpha] [float] null,
	[Delta] [float] null,
	[Gamma] [float] null,
	[Vega] [float] null,
	[Theta] [float] null,
	[Rho] [float] null,
	[Volga] [float] null,
	[DeltaVega] [float] null,
	[DeltaTheta] [float] null,
	[GammaVega] [float] null,
	[GammaTheta] [float] null,
	[IVola] [float] null,
	[IVolaBid] [float] null,
	[IVolaAsk] [float] null,
	[TheoVola] [float] null,
	[BcPrice] [float] null,
	[Price] [float] null,
	[PriceBid] [float] null,
	[PriceBidDate] [datetime] null,
	[PriceAsk] [float] null,
	[PriceAskDate] [datetime] null,
	[PriceLast] [float] null,
	[PriceLastDate] [datetime] null,
	[BcPriceBid] [float] null,
	[BcPriceBidDate] [datetime] null,
	[BcPriceAsk] [float] null,
	[BcPriceAskDate] [datetime] null,
	[BcPriceLast] [float] null,
	[BcPriceLastDate] [datetime] null,
	[AnalyticsTimestamp] [datetime] null,
	[ActionDate] [datetime] not null constraint [DF_ContractHistoricalSnapshot_actionDate] default (getdate()),

	constraint [PK_ContractHistoricalSnapshot_ID] primary key  clustered 
	(
		[ID]
	)  on [PRIMARY]
) on [PRIMARY]
GO


