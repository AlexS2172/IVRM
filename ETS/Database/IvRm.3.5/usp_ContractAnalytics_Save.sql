


SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_TradeRisks_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_TradeRisks_Save]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[usp_ContractAnalytics_Save]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[usp_ContractAnalytics_Save]
GO

CREATE   procedure dbo.usp_ContractAnalytics_Save
   @vXMLData ntext
As

	DECLARE @idoc int  
	EXEC sp_xml_preparedocument @idoc OUTPUT, @vXMLData  
		
	begin Tran
	
	--update 
	update
		ContractHistoricalSnapshot
	set
		ContractID = rsRisks.xmlContractID,
		TheoPrice = rsRisks.xmlTheoPrice,
		Alpha = rsRisks.xmlAlpha,
		Delta = rsRisks.xmlDelta,
		Gamma = rsRisks.xmlGamma,
		Vega = rsRisks.xmlVega,
		Theta = rsRisks.xmlTheta,
		Rho = rsRisks.xmlRho,
		Volga = rsRisks.xmlVolga,
		DeltaVega = rsRisks.xmlDeltaVega,
		DeltaTheta = rsRisks.xmlDeltaTheta,
		GammaVega = rsRisks.xmlGammaVega,
		GammaTheta = rsRisks.xmlGammaTheta,
		IVola = rsRisks.xmlIVola,
		IVolaBid = rsRisks.xmlIVolaBid,
		IVolaAsk = rsRisks.xmlIVolaAsk,
		TheoVola = rsRisks.xmlTheoVola,
		Price = rsRisks.xmlPrice,
		BcPrice = rsRisks.xmlBcPrice,

		PriceBid = rsRisks.xmlPriceBid,
		PriceBidDate = rsRisks.xmlPriceBidDate,
		PriceAsk = rsRisks.xmlPriceAsk,
		PriceAskDate = rsRisks.xmlPriceAskDate,
		PriceLast = rsRisks.xmlPriceLast,
		PriceLastDate = rsRisks.xmlPriceLastDate,

		BcPriceBid = rsRisks.xmlBcPriceBid,
		BcPriceBidDate = rsRisks.xmlBcPriceBidDate,
		BcPriceAsk = rsRisks.xmlBcPriceAsk,
		BcPriceAskDate = rsRisks.xmlBcPriceAskDate,
		BcPriceLast = rsRisks.xmlBcPriceLast,
		BcPriceLastDate = rsRisks.xmlBcPriceLastDate,

		AnalyticsTimestamp = rsRisks.xmlAnalyticsTimestamp
	from
	(
		select 
			vXML.TradeID as xmlTradeID, 
			vXML.ContractID as xmlContractID,
			vXML.TheoPrice as xmlTheoPrice,
			vXML.Alpha as xmlAlpha,
			vXML.Delta as xmlDelta,
			vXML.Gamma as xmlGamma,
			vXML.Vega as xmlVega,
			vXML.Theta as xmlTheta,
			vXML.Rho as xmlRho,
			vXML.Volga as xmlVolga,
			vXML.DeltaVega as xmlDeltaVega,
			vXML.DeltaTheta as xmlDeltaTheta,
			vXML.GammaVega as xmlGammaVega,
			vXML.GammaTheta as xmlGammaTheta,
			vXML.IVola as xmlIVola,
			vXML.IVolaBid as xmlIVolaBid,
			vXML.IVolaAsk as xmlIVolaAsk,
			vXML.TheoVola as xmlTheoVola,
			vXML.Price as xmlPrice,
			vXML.BcPrice as xmlBcPrice,

			vXML.PriceBid as xmlPriceBid,
			cast(vXML.PriceBidDate as datetime) as xmlPriceBidDate,
			vXML.PriceAsk as xmlPriceAsk,
			cast(vXML.PriceAskDate as datetime) as xmlPriceAskDate,
			vXML.PriceLast as xmlPriceLast,
			cast(vXML.PriceLastDate as datetime) as xmlPriceLastDate,

			vXML.BcPriceBid as xmlBcPriceBid,
			cast(vXML.BcPriceBidDate as datetime) as xmlBcPriceBidDate,
			vXML.BcPriceAsk as xmlBcPriceAsk,
			cast(vXML.BcPriceAskDate as datetime) as xmlBcPriceAskDate,
			vXML.BcPriceLast as xmlBcPriceLast,
			cast(vXML.BcPriceLastDate as datetime) as xmlBcPriceLastDate,

			cast(vXML.AnalyticsTimestamp as datetime) as xmlAnalyticsTimestamp

		from openxml (@idoc, '/root/Risks', 2) with
	 	(
			TradeID 	int '@TradeID',
			ContractID 	int '@ContractID',
			TheoPrice 	float '@TheoPrice',
			Alpha 		float '@Alpha',
			Delta 		float '@Delta',
			Gamma 		float '@Gamma',	
			Vega 		float '@Vega',	
			Theta 		float '@Theta',		
			Rho 		float '@Rho',	
			Volga 		float '@Volga',
			DeltaVega 	float '@DeltaVega',
			DeltaTheta 	float '@DeltaTheta',	
			GammaVega 	float '@GammaVega',
			GammaTheta 	float '@GammaTheta',
			IVola 		float '@IVola',
			IVolaBid 	float '@IVolaBid',
			IVolaAsk 	float '@IVolaAsk',
			TheoVola 	float '@TheoVola',
			Price 		float '@ContractPrice',
			BcPrice 	float '@BaseContractPrice',
			PriceBid 	float '@ContractPriceBid',
			PriceBidDate 	datetime '@ContractPriceBidDate',
			PriceAsk 	float '@ContractPriceAsk',
			PriceAskDate 	datetime '@ContractPriceAskDate',
			PriceLast 	float '@ContractPriceLast',
			PriceLastDate 	datetime '@ContractPriceLastDate',
			BcPriceBid 	float '@BaseContractPriceBid',
			BcPriceBidDate 	datetime '@BaseContractPriceBidDate',
			BcPriceAsk 	float '@BaseContractPriceAsk',
			BcPriceAskDate 	datetime '@BaseContractPriceAskDate',
			BcPriceLast 	float '@BaseContractPriceLast',
			BcPriceLastDate datetime '@BaseContractPriceLastDate',
			AnalyticsTimestamp datetime '@AnalyticsTimestamp'
		) vXML
	) rsRisks
	where
		rsRisks.xmlTradeID = TradeID

	--insert
	insert into 
		ContractHistoricalSnapshot(	
				TradeID,
				ContractID,
				TheoPrice,
				Alpha,
				Delta,
				Gamma,
				Vega,
				Theta,
				Rho,
				Volga,
				DeltaVega,
				DeltaTheta,
				GammaVega,
				GammaTheta,
				IVola,
				IVolaBid,
				IVolaAsk,
				TheoVola,
				Price,
				BcPrice,
				PriceBid,
				PriceBidDate,
				PriceAsk,
				PriceAskDate,
				PriceLast,
				PriceLastDate,
				BcPriceBid,
				BcPriceBidDate,
				BcPriceAsk,
				BcPriceAskDate,
				BcPriceLast,
				BcPriceLastDate,
				AnalyticsTimestamp)
	select
		rsRisks.xmlTradeID as TradeID,			
		rsRisks.xmlContractID as ContractID,
		rsRisks.xmlTheoPrice as TheoPrice,
		rsRisks.xmlAlpha as Alpha,
		rsRisks.xmlDelta as Delta,
		rsRisks.xmlGamma as Gamma,
		rsRisks.xmlVega as Vega,
		rsRisks.xmlTheta as Theta,
		rsRisks.xmlRho as Rho,
		rsRisks.xmlVolga as Volga,
		rsRisks.xmlDeltaVega as DeltaVega,
		rsRisks.xmlDeltaTheta as DeltaTheta,
		rsRisks.xmlGammaVega as GammaVega,
		rsRisks.xmlGammaTheta as GammaTheta,
		rsRisks.xmlIVola as IVola,
		rsRisks.xmlIVolaBid as IVolaBid,
		rsRisks.xmlIVolaAsk as IVolaAsk,
		rsRisks.xmlTheoVola as TheoVola,
		rsRisks.xmlPrice as Price,
		rsRisks.xmlBcPrice as BcPrice,
		rsRisks.xmlPriceBid as PriceBid,
		cast(rsRisks.xmlPriceBidDate as datetime) as PriceBidDate,
		rsRisks.xmlPriceAsk as PriceAsk,
		cast(rsRisks.xmlPriceAskDate as datetime) as PriceAskDate,
		rsRisks.xmlPriceLast as PriceLast,
		cast(rsRisks.xmlPriceLastDate as datetime) as PriceLastDate,
		rsRisks.xmlBcPriceBid as BcPriceBid,
		cast(rsRisks.xmlBcPriceBidDate as datetime) as BcPriceBidDate,
		rsRisks.xmlBcPriceAsk as BcPriceAsk,
		cast(rsRisks.xmlBcPriceAskDate as datetime) as BcPriceAskDate,
		rsRisks.xmlBcPriceLast as BcPriceLast,
		cast(rsRisks.xmlBcPriceLastDate as datetime) as BcPriceLastDate,
		cast(rsRisks.xmlAnalyticsTimestamp as datetime) as AnalyticsTimestamp 
	from
	(
		select 
			vXML.TradeID as xmlTradeID, 
			vXML.ContractID as xmlContractID,
			vXML.TheoPrice as xmlTheoPrice,
			vXML.Alpha as xmlAlpha,
			vXML.Delta as xmlDelta,
			vXML.Gamma as xmlGamma,
			vXML.Vega as xmlVega,
			vXML.Theta as xmlTheta,
			vXML.Rho as xmlRho,
			vXML.Volga as xmlVolga,
			vXML.DeltaVega as xmlDeltaVega,
			vXML.DeltaTheta as xmlDeltaTheta,
			vXML.GammaVega as xmlGammaVega,
			vXML.GammaTheta as xmlGammaTheta,
			vXML.IVola as xmlIVola,
			vXML.IVolaBid as xmlIVolaBid,
			vXML.IVolaAsk as xmlIVolaAsk,
			vXML.TheoVola as xmlTheoVola,
			vXML.Price as xmlPrice,
			vXML.BcPrice as xmlBcPrice,

			vXML.PriceBid as xmlPriceBid,
			vXML.PriceBidDate as xmlPriceBidDate,
			vXML.PriceAsk as xmlPriceAsk,
			vXML.PriceAskDate as xmlPriceAskDate,
			vXML.PriceLast as xmlPriceLast,
			vXML.PriceLastDate as xmlPriceLastDate,

			vXML.BcPriceBid as xmlBcPriceBid,
			vXML.BcPriceBidDate as xmlBcPriceBidDate,
			vXML.BcPriceAsk as xmlBcPriceAsk,
			vXML.BcPriceAskDate as xmlBcPriceAskDate,
			vXML.BcPriceLast as xmlBcPriceLast,
			vXML.BcPriceLastDate as xmlBcPriceLastDate,

			vXML.AnalyticsTimestamp as xmlAnalyticsTimestamp

		from openxml (@idoc, '/root/Risks', 2) with
	 	(
			TradeID 	int '@TradeID',
			ContractID 	int '@ContractID',
			TheoPrice 	float '@TheoPrice',
			Alpha 		float '@Alpha',
			Delta 		float '@Delta',
			Gamma 		float '@Gamma',	
			Vega 		float '@Vega',	
			Theta 		float '@Theta',		
			Rho 		float '@Rho',	
			Volga 		float '@Volga',
			DeltaVega 	float '@DeltaVega',
			DeltaTheta 	float '@DeltaTheta',	
			GammaVega 	float '@GammaVega',
			GammaTheta 	float '@GammaTheta',
			IVola 		float '@IVola',
			IVolaBid 	float '@IVolaBid',
			IVolaAsk 	float '@IVolaAsk',
			TheoVola 	float '@TheoVola',
			Price 		float '@ContractPrice',
			BcPrice 	float '@BaseContractPrice',
			PriceBid 	float '@ContractPriceBid',
			PriceBidDate 	datetime '@ContractPriceBidDate',
			PriceAsk 	float '@ContractPriceAsk',
			PriceAskDate 	datetime '@ContractPriceAskDate',
			PriceLast 	float '@ContractPriceLast',
			PriceLastDate 	datetime '@ContractPriceLastDate',
			BcPriceBid 	float '@BaseContractPriceBid',
			BcPriceBidDate 	datetime '@BaseContractPriceBidDate',
			BcPriceAsk 	float '@BaseContractPriceAsk',
			BcPriceAskDate 	datetime '@BaseContractPriceAskDate',
			BcPriceLast 	float '@BaseContractPriceLast',
			BcPriceLastDate datetime '@BaseContractPriceLastDate',
			AnalyticsTimestamp datetime '@AnalyticsTimestamp'
		) vXML
	) rsRisks
	where 
		rsRisks.xmlTradeID not in (select distinct TradeID from ContractHistoricalSnapshot)
	
	if @@Error <> 0
	begin
		rollback
		Return -1
	end

	commit
	EXEC sp_xml_removedocument @idoc   
	Return 0

GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

