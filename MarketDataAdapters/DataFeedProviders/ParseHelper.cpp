#include "stdafx.h"
#include "ParseHelper.h"

void CParseHelper::InitQUI(QuoteUpdateInfo* pQuoteInfo)
{
	pQuoteInfo->BidPrice     = BAD_DOUBLE_VALUE;
	pQuoteInfo->AskPrice     = BAD_DOUBLE_VALUE;
	pQuoteInfo->LastPrice    = BAD_DOUBLE_VALUE;
	pQuoteInfo->OpenInterest = BAD_DOUBLE_VALUE;
	pQuoteInfo->OpenPrice    = BAD_DOUBLE_VALUE;
	pQuoteInfo->ClosePrice   = BAD_DOUBLE_VALUE;
	pQuoteInfo->HighPrice	 = BAD_DOUBLE_VALUE;
	pQuoteInfo->LowPrice	 = BAD_DOUBLE_VALUE;
	pQuoteInfo->NetChange    = BAD_DOUBLE_VALUE;
	pQuoteInfo->SeqNum		 = BAD_LONG_VALUE;
	pQuoteInfo->BidSize		 = BAD_LONG_VALUE;
	pQuoteInfo->AskSize      = BAD_LONG_VALUE; 
	pQuoteInfo->Volume		 = BAD_LONG_VALUE;
	pQuoteInfo->Skipped		 = BAD_LONG_VALUE;
	pQuoteInfo->TotalRequests= BAD_LONG_VALUE;
	pQuoteInfo->WaitTime     = BAD_LONG_VALUE; 
	pQuoteInfo->PriceTick    = BAD_LONG_VALUE;
	pQuoteInfo->LotSize		 = BAD_LONG_VALUE;
	pQuoteInfo->UpdateDateTime = 0;
	pQuoteInfo->Currency	 = _bstr_t("").copy();
	pQuoteInfo->Exchange	 = _bstr_t("").copy();
	pQuoteInfo->BidExchange	 = _bstr_t("").copy();
	pQuoteInfo->AskExchange	 = _bstr_t("").copy();
}

void CParseHelper::InitRISKS(IRisks* pRisksInfo)
{
	pRisksInfo->TheoPrice			= BAD_DOUBLE_VALUE;
	pRisksInfo->TheoPriceBid		= BAD_DOUBLE_VALUE;
	pRisksInfo->TheoPriceAsk		= BAD_DOUBLE_VALUE;
	pRisksInfo->Alpha				= BAD_DOUBLE_VALUE;
	pRisksInfo->Delta				= BAD_DOUBLE_VALUE;
	pRisksInfo->Gamma				= BAD_DOUBLE_VALUE;
	pRisksInfo->Vega				= BAD_DOUBLE_VALUE;
	pRisksInfo->Theta				= BAD_DOUBLE_VALUE;
	pRisksInfo->Rho					= BAD_DOUBLE_VALUE;
	pRisksInfo->Volga				= BAD_DOUBLE_VALUE;
	pRisksInfo->DeltaVega			= BAD_DOUBLE_VALUE;
	pRisksInfo->DeltaTheta			= BAD_DOUBLE_VALUE;
	pRisksInfo->GammaVega			= BAD_DOUBLE_VALUE;
	pRisksInfo->GammaTheta			= BAD_DOUBLE_VALUE; 
	pRisksInfo->IVola				= BAD_DOUBLE_VALUE;
	pRisksInfo->TheoVola			= BAD_DOUBLE_VALUE;
	pRisksInfo->BaseContractPrice	= BAD_DOUBLE_VALUE;
	pRisksInfo->BaseContractPriceBid= BAD_DOUBLE_VALUE;
	pRisksInfo->BaseContractPriceAsk= BAD_DOUBLE_VALUE;
	pRisksInfo->ContractPrice		= BAD_DOUBLE_VALUE; 
	pRisksInfo->ContractPriceBid	= BAD_DOUBLE_VALUE; 
	pRisksInfo->ContractPriceAsk	= BAD_DOUBLE_VALUE; 
	pRisksInfo->IVolaBid			= BAD_DOUBLE_VALUE;
	pRisksInfo->IVolaAsk			= BAD_DOUBLE_VALUE;
	pRisksInfo->IntrinsicValue		= BAD_DOUBLE_VALUE;
}

void CParseHelper::ParseEquityBaseRecord(const VS::CResponseRecord& record, EquiteBaseInfoPtr eqInfo)
{
	TRACEINFO(__FUNCTION__,"call");
	eqInfo->StockSymbol					= record.GetField(DDS::FIELD_STOCK_SYMBOL).AsString();
	eqInfo->DIV_Date					= record.GetField(DDS::FIELD_STOCK_DIV_DATE).AsDateTime();
	eqInfo->DIV_Freq					= record.GetField(DDS::FIELD_STOCK_DIV_FREQ).AsShort();
	eqInfo->DIV_AMT_YIELD				= record.GetField(DDS::FIELD_STOCK_DIV_AMT_YIELD).AsFloat();
	eqInfo->currency					= record.GetField(DDS::FIELD_CURRENCY).AsString();
	eqInfo->StockType					= record.GetField(DDS::FIELD_STOCK_TYPE).AsString()[0];
	eqInfo->PriceClose					= record.GetField(DDS::FIELD_PRICE_CLOSE).AsFloat();
	eqInfo->PriceHigh52WK				= record.GetField(DDS::FIELD_PRICE_HIGH52WK).AsFloat();
	eqInfo->PriceLow52WK				= record.GetField(DDS::FIELD_PRICE_LOW52WK).AsFloat();
	eqInfo->SOD_IVX30					= record.GetField(DDS::FIELD_SOD_IVX30).AsFloat();
	eqInfo->SOD_IVX30_YEST				= record.GetField(DDS::FIELD_SOD_IVX30_YEST).AsFloat();
	eqInfo->SOD_IVX30_High52WK			= record.GetField(DDS::FIELD_SOD_IVX30_HIGH52WK).AsFloat();
	eqInfo->SOD_IVX30_Low52WK			= record.GetField(DDS::FIELD_SOD_IVX30_LOW52WK).AsFloat();
	eqInfo->SOD_HV20					= record.GetField(DDS::FIELD_SOD_HV20).AsFloat();
	eqInfo->SOD_HV20_YEST				= record.GetField(DDS::FIELD_SOD_HV20_YEST).AsFloat();
	eqInfo->SOD_HV20_High52WK			= record.GetField(DDS::FIELD_SOD_HV20_HIGH52WK).AsFloat();
	eqInfo->SOD_HV20_LOW52WK			= record.GetField(DDS::FIELD_SOD_HV20_LOW52WK).AsFloat();
	eqInfo->SOD_OPT_VOLUME_CALL			= record.GetField(DDS::FIELD_SOD_OPT_VOLUME_CALL).AsInt();
	eqInfo->SOD_OPT_VOLUME_PUT			= record.GetField(DDS::FIELD_SOD_OPT_VOLUME_PUT).AsInt();
	eqInfo->SOD_OPT_VOLUME_AVG			= record.GetField(DDS::FIELD_SOD_OPT_VOLUME_AVG).AsFloat();
	eqInfo->OutstandingShares			= record.GetField(DDS::FIELD_OUTSTANDING_SHARES).AsFloat();
	TRACEINFO(__FUNCTION__,"finished");
}

void CParseHelper::ParseOptionBaseRecord(const VS::CResponseRecord& record, OptionBaseInfoPtr opInfo)
{
	TRACEINFO(__FUNCTION__,"call");
	opInfo->OptionSymbol	= record.GetField(DDS::FIELD_OPTION_SYMBOL).AsString();
	opInfo->StockSymbol		= record.GetField(DDS::FIELD_STOCK_SYMBOL).AsString();
	opInfo->Expiration		= record.GetField(DDS::FIELD_EXPIRATION).AsDateTime();
	opInfo->Strike			= record.GetField(DDS::FIELD_STRIKE).AsDouble();
	opInfo->OptionType		= record.GetField(DDS::FIELD_OPTION_TYPE).AsString()[0];
	opInfo->OptionStyle		= record.GetField(DDS::FIELD_OPTION_STYLE).AsString()[0];
	opInfo->OpenInterest	= record.GetField(DDS::FIELD_OPEN_INTEREST).AsInt();
	TRACEINFO(__FUNCTION__,"finished");
}

void CParseHelper::ParseEquityQuoteRecord(const VS::CResponseRecord& record, EquiteQuoteInfo* eqQuote)
{
	TRACEINFO(__FUNCTION__,"call");
	eqQuote->StockSymbol	= record.GetField(DDS::FIELD_STOCK_SYMBOL).AsString();
	eqQuote->Bid			= record.GetField(DDS::FIELD_BID).AsDouble();
	eqQuote->BidExchange	= record.GetField(DDS::FIELD_BID_EXCHANGE).AsString();
	eqQuote->BidDate		= record.GetField(DDS::FIELD_BID_DATE).AsDouble();
	eqQuote->BibSize		= record.GetField(DDS::FIELD_BID_SIZE).AsInt();
	eqQuote->Ask			= record.GetField(DDS::FIELD_ASK).AsDouble();
	eqQuote->AskExchange	= record.GetField(DDS::FIELD_ASK_EXCHANGE).AsString();
	eqQuote->AskDate		= record.GetField(DDS::FIELD_ASK_DATE).AsDouble();
	eqQuote->AskSize		= record.GetField(DDS::FIELD_ASK_SIZE).AsInt();
	eqQuote->Last			= record.GetField(DDS::FIELD_LAST).AsDouble();
	eqQuote->LastExchange	= record.GetField(DDS::FIELD_LAST_EXCHANGE).AsString();
	eqQuote->LastDate		= record.GetField(DDS::FIELD_LAST_DATE).AsDouble();
	eqQuote->LastSize		= record.GetField(DDS::FIELD_LAST_SIZE).AsInt();
	eqQuote->Volume			= record.GetField(DDS::FIELD_VOLUME).AsLongLong();
	TRACEINFO(__FUNCTION__,"finished");
}

void CParseHelper::ParseOptionQuoteRecord(const VS::CResponseRecord& record, OptionQuoteInfo* opQuote)
{
	TRACEINFO(__FUNCTION__,"call");
	opQuote->OptionSymbol	= record.GetField(DDS::FIELD_OPTION_SYMBOL).AsString();
	opQuote->StockSymbol	= record.GetField(DDS::FIELD_STOCK_SYMBOL).AsString();
	opQuote->Bid			= record.GetField(DDS::FIELD_BID).AsDouble();
	opQuote->BidDate		= record.GetField(DDS::FIELD_BID_DATE).AsDateTime();
	opQuote->BidExchange	= record.GetField(DDS::FIELD_BID_EXCHANGE).AsString();
	opQuote->BidSize		= record.GetField(DDS::FIELD_BID_SIZE).AsInt();
	opQuote->Ask			= record.GetField(DDS::FIELD_ASK).AsDouble();
	opQuote->AskDate		= record.GetField(DDS::FIELD_ASK_DATE).AsDateTime();
	opQuote->AskExchange	= record.GetField(DDS::FIELD_ASK_EXCHANGE).AsString();
	opQuote->AskSize		= record.GetField(DDS::FIELD_ASK_SIZE).AsInt();
	opQuote->Last			= record.GetField(DDS::FIELD_LAST).AsDouble();
	opQuote->LastDate		= record.GetField(DDS::FIELD_LAST_DATE).AsDateTime();
	opQuote->LastExchange	= record.GetField(DDS::FIELD_LAST_EXCHANGE).AsString();
	opQuote->LastSize		= record.GetField(DDS::FIELD_LAST_SIZE).AsInt();
	opQuote->Volume			= record.GetField(DDS::FIELD_VOLUME).AsLongLong();
	opQuote->IV				= record.GetField(DDS::FIELD_IV).AsDouble();
	opQuote->PreIV			= record.GetField(DDS::FIELD_PREIV).AsFloat();
	opQuote->Delta			= record.GetField(DDS::FIELD_DELTA).AsDouble();
	opQuote->Gamma			= record.GetField(DDS::FIELD_GAMMA).AsDouble();
	opQuote->Vega			= record.GetField(DDS::FIELD_VEGA).AsDouble();
	opQuote->Theta			= record.GetField(DDS::FIELD_THETA).AsDouble();
	opQuote->Rho			= record.GetField(DDS::FIELD_RHO).AsFloat();
	TRACEINFO(__FUNCTION__,"finished");
}



void CParseHelper::ParseEquityBaseRecord(const VS::CResponseRecord& record, QuoteUpdateMap* req_map, QuoteUpdateInfoMap* update_map)
{
	TRACEINFO(__FUNCTION__,"call");
	std::string StockSymbol = record.GetField(DDS::FIELD_STOCK_SYMBOL).AsString();
	CTicker tick;
	tick.m_sSymbol = _bstr_t(StockSymbol.c_str()).Detach();
	tick.m_enType = (InstrumentTypeEnum)enSTK;
	tick.m_sExchange = _bstr_t("").Detach();
	QuoteUpdateMap::const_iterator request = req_map->find(tick);

	//if stock not found, try to find index
	if (request == req_map->end())
	{
		tick.m_enType = (InstrumentTypeEnum)enIDX;
		request = req_map->find(tick);
	};

	if (request != req_map->end())
	{
		QuoteUpdateInfoMap::const_iterator info = update_map->find(StockSymbol);
		if (info == update_map->end())
		{
			QuoteUpdateFullInfoPtr ptr(new QuoteUpdateFullInfo());
			InitQUI(&(ptr->Info));
			request->second->CopyTo(ptr->Params);
			info = update_map->insert(QuoteUpdateInfoMap::value_type(StockSymbol,ptr)).first;
		}
		QuoteUpdateFullInfoPtr ptr = info->second;
		ptr->Info.Currency = (_bstr_t(record.GetField(DDS::FIELD_CURRENCY).AsString().c_str())).copy();
		ptr->Info.ClosePrice = record.GetField(DDS::FIELD_PRICE_CLOSE).AsFloat();
		ptr->Info.HighPrice	= record.GetField(DDS::FIELD_PRICE_HIGH52WK).AsFloat();
		ptr->Info.LowPrice = record.GetField(DDS::FIELD_PRICE_LOW52WK).AsFloat();
		ptr->Info.Status = (QuoteStatusType)enQuoteStatus_OK;
	}
	TRACEINFO(__FUNCTION__,"finished");
}

void CParseHelper::ParseRisksRecord(const VS::CResponseRecord& record, RisksUpdateInfoMap* req_map, RisksUpdateMap* update_map)
{
	TRACEINFO(__FUNCTION__,"call");
	std::string Symbol = record.GetField(DFFields::FIELD_SYMBOL).AsString();
	CTicker tick;
	tick.m_sSymbol = _bstr_t(Symbol.c_str()).Detach();
	tick.m_enType = (InstrumentTypeEnum)enOPT;
	tick.m_sExchange = _bstr_t("").Detach();
	RisksUpdateInfoMap::const_iterator request = req_map->find(tick);

	if (request != req_map->end())
	{
		RisksUpdateMap::const_iterator info = update_map->find(request->first);
		if (info == update_map->end())
		{
			RisksFullInfoPtr ptr(new IRisksFullInfo());
			InitRISKS(&(ptr->Risks));
			request->second->CopyTo(ptr->Ticker);// = *(request->second);
			info = update_map->insert(RisksUpdateMap::value_type(request->first,ptr)).first;
		}

		RisksFullInfoPtr ptr = info->second;
		ptr->Risks.TheoPrice =			record.GetField(DFFields::FIELD_THEO_PRICE).AsFloat();
		ptr->Risks.Alpha =				0;//record.GetField(DFFields::FIELD_PRICE_CLOSE).AsFloat();
		ptr->Risks.Delta =				record.GetField(DFFields::FIELD_DELTA).AsFloat();
		ptr->Risks.Gamma =				record.GetField(DFFields::FIELD_GAMMA).AsFloat();
		ptr->Risks.Vega =				record.GetField(DFFields::FIELD_VEGA).AsFloat();
		ptr->Risks.Theta =				record.GetField(DFFields::FIELD_THETA).AsFloat();
		ptr->Risks.Rho =				record.GetField(DFFields::FIELD_RHO).AsFloat();
		ptr->Risks.Volga =				record.GetField(DFFields::FIELD_VOLGA).AsFloat();

		ptr->Risks.DeltaVega =			record.GetField(DFFields::FIELD_DELTA_VEGA).AsFloat();
		ptr->Risks.DeltaTheta =			record.GetField(DFFields::FIELD_DELTA_THETA).AsFloat();
		ptr->Risks.GammaVega =			record.GetField(DFFields::FIELD_GAMMA_VEGA).AsFloat();
		ptr->Risks.GammaTheta =			record.GetField(DFFields::FIELD_GAMMA_THETA).AsFloat();

		ptr->Risks.IVola =				record.GetField(DFFields::FIELD_IV).AsFloat();
		ptr->Risks.IVolaBid =			0;//record.GetField(DDS::FIELD_PRICE_CLOSE).AsFloat();
		ptr->Risks.IVolaAsk =			0;//record.GetField(DDS::FIELD_PRICE_CLOSE).AsFloat();
		ptr->Risks.TheoVola =			record.GetField(DFFields::FIELD_THEO_VOLA).AsFloat();

		ptr->Risks.BaseContractPrice =	record.GetField(DFFields::FIELD_BASECONTRACT_PRICE).AsFloat();
		ptr->Risks.ContractPrice =		record.GetField(DFFields::FIELD_CONTRACT_PRICE).AsFloat();
	}
	TRACEINFO(__FUNCTION__,"finished");
}

void CParseHelper::ParseOptionBaseRecord(const VS::CResponseRecord& record, QuoteUpdateMap* req_map, QuoteUpdateInfoMap* update_map)
{
	TRACEINFO(__FUNCTION__,"call");
	std::string StockSymbol = record.GetField(DDS::FIELD_STOCK_SYMBOL).AsString();
	std::string OptionSymbol = record.GetField(DDS::FIELD_OPTION_SYMBOL).AsString();
	CTicker tick;
	tick.m_sSymbol = _bstr_t(OptionSymbol.c_str()).Detach();
	tick.m_enType = (InstrumentTypeEnum)enOPT;
	tick.m_sExchange = _bstr_t("").Detach();
	QuoteUpdateMap::const_iterator request = req_map->find(tick);
	if (request != req_map->end())
	{
		QuoteUpdateInfoMap::const_iterator info = update_map->find(OptionSymbol);
		if (info == update_map->end())
		{
			QuoteUpdateFullInfoPtr ptr(new QuoteUpdateFullInfo());
			InitQUI(&(ptr->Info));
			request->second->CopyTo(ptr->Params);
			info = update_map->insert(QuoteUpdateInfoMap::value_type(OptionSymbol,ptr)).first;
		}
		QuoteUpdateFullInfoPtr ptr = info->second;
		ptr->Info.Currency = (_bstr_t(record.GetField(DDS::FIELD_CURRENCY).AsString().c_str())).copy();
		ptr->Info.ClosePrice = record.GetField(DDS::FIELD_PRICE_CLOSE).AsFloat();
		ptr->Info.HighPrice	= record.GetField(DDS::FIELD_PRICE_HIGH52WK).AsFloat();
		ptr->Info.LowPrice = record.GetField(DDS::FIELD_PRICE_LOW52WK).AsFloat();
		ptr->Info.Status = (QuoteStatusType)enQuoteStatus_OK;
	}
	TRACEINFO(__FUNCTION__,"finished");
}

void CParseHelper::ParseEquityQuoteRecord(const VS::CResponseRecord& record, QuoteUpdateMap* req_map, QuoteUpdateInfoMap* update_map)
{
	TRACEINFO(__FUNCTION__,"call");
	std::string StockSymbol = record.GetField(DDS::FIELD_STOCK_SYMBOL).AsString();
	CTicker tick;
	tick.m_sSymbol = _bstr_t(StockSymbol.c_str()).Detach();
	tick.m_enType = (InstrumentTypeEnum)enSTK;
	tick.m_sExchange = _bstr_t("").Detach();
	QuoteUpdateMap::const_iterator request = req_map->find(tick);

	//if stock not found, try to find index
	if (request == req_map->end())
	{
		tick.m_enType = (InstrumentTypeEnum)enIDX;
		request = req_map->find(tick);
	};
	
	if (request != req_map->end())
	{
		QuoteUpdateInfoMap::const_iterator info = update_map->find(StockSymbol);
		if (info == update_map->end())
		{
			QuoteUpdateFullInfoPtr ptr(new QuoteUpdateFullInfo());
			InitQUI(&(ptr->Info));
			request->second->CopyTo(ptr->Params);
			info = update_map->insert(QuoteUpdateInfoMap::value_type(StockSymbol,ptr)).first;
		}
		QuoteUpdateFullInfoPtr ptr = info->second;
		ptr->Info.BidPrice = record.GetField(DDS::FIELD_BID).AsDouble();
		ptr->Info.AskPrice = record.GetField(DDS::FIELD_ASK).AsDouble();
		ptr->Info.BidSize = record.GetField(DDS::FIELD_BID_SIZE).AsInt();
		ptr->Info.AskSize = record.GetField(DDS::FIELD_ASK_SIZE).AsInt();
		ptr->Info.LastPrice = record.GetField(DDS::FIELD_LAST).AsDouble();
		ptr->Info.Volume = static_cast<long>(record.GetField(DDS::FIELD_VOLUME).AsLongLong());
		ptr->Info.AskExchange = (_bstr_t(record.GetField(DDS::FIELD_ASK_EXCHANGE).AsString().c_str())).copy();
		ptr->Info.BidExchange = (_bstr_t(record.GetField(DDS::FIELD_BID_EXCHANGE).AsString().c_str())).copy();
		ptr->Info.Status = (QuoteStatusType)enQuoteStatus_OK;
	}
	TRACEINFO(__FUNCTION__,"finished");
}

void CParseHelper::ParseOptionQuoteRecord(const VS::CResponseRecord& record, QuoteUpdateMap* req_map, QuoteUpdateInfoMap* update_map)
{
	TRACEINFO(__FUNCTION__,"call");
	std::string StockSymbol = record.GetField(DDS::FIELD_STOCK_SYMBOL).AsString();
	std::string OptionSymbol = record.GetField(DDS::FIELD_OPTION_SYMBOL).AsString();
	CTicker tick;
	tick.m_sSymbol = OptionSymbol.c_str();
	tick.m_enType = (InstrumentTypeEnum)enOPT;
	tick.m_sExchange = _T("");
	QuoteUpdateMap::const_iterator request = req_map->find(tick);
	if (request != req_map->end())
	{
		QuoteUpdateInfoMap::const_iterator info = update_map->find(OptionSymbol);
		if (info == update_map->end())
		{
			QuoteUpdateFullInfoPtr ptr(new QuoteUpdateFullInfo());
			InitQUI(&(ptr->Info));
			request->second->CopyTo(ptr->Params);
			info = update_map->insert(QuoteUpdateInfoMap::value_type(OptionSymbol,ptr)).first;
		}
		QuoteUpdateFullInfoPtr ptr = info->second;
		ptr->Info.BidPrice		= record.GetField(DDS::FIELD_BID).AsDouble();
		ptr->Info.AskPrice		= record.GetField(DDS::FIELD_ASK).AsDouble();
		ptr->Info.BidExchange	= (_bstr_t(record.GetField(DDS::FIELD_BID_EXCHANGE).AsString().c_str())).copy();
		ptr->Info.AskExchange	= (_bstr_t(record.GetField(DDS::FIELD_ASK_EXCHANGE).AsString().c_str())).copy();
		ptr->Info.LastPrice		= record.GetField(DDS::FIELD_LAST).AsDouble();
		ptr->Info.Volume		= static_cast<long>(record.GetField(DDS::FIELD_VOLUME).AsLongLong());
		ptr->Info.BidSize		= record.GetField(DDS::FIELD_BID_SIZE).AsInt();
		ptr->Info.AskSize		= record.GetField(DDS::FIELD_ASK_SIZE).AsInt();
	}
	TRACEINFO(__FUNCTION__,"finished");
}

void CParseHelper::ParseEquityBaseRecord(const VS::CResponseRecord& record, RequestMap* req_map, StockInfoMap* update_map)
{
	TRACEINFO(__FUNCTION__,"call");
	std::string StockSymbol = record.GetField(DDS::FIELD_STOCK_SYMBOL).AsString();
	RequestMap::const_iterator request = req_map->find(StockSymbol);
	if (request != req_map->end())
	{
		StockInfoMap::const_iterator info = update_map->find(StockSymbol);
		if (info == update_map->end())
		{
			StockResultsExFullInfoPtr ptr(new StockResultsExFullInfo());
			ptr->Params = *(request->second);
			info = update_map->insert(StockInfoMap::value_type(StockSymbol,ptr)).first;
		}
		StockResultsExFullInfoPtr ptr = info->second;
		ptr->Info.DivFreq		= record.GetField(DDS::FIELD_STOCK_DIV_FREQ).AsShort();
		ptr->Info.DivAmount		= record.GetField(DDS::FIELD_STOCK_DIV_AMT_YIELD).AsFloat();
		ptr->Info.DivLastDate	= record.GetField(DDS::FIELD_STOCK_DIV_DATE).AsDateTime();
		ptr->Info.Currency		= (_bstr_t(record.GetField(DDS::FIELD_CURRENCY).AsString().c_str())).copy();
		ptr->Info.StockType		= (record.GetField(DDS::FIELD_STOCK_TYPE).AsString()[0] == 'S')? (StockTypeEnum)enStock : (StockTypeEnum)enIndex;
	}
	TRACEINFO(__FUNCTION__,"finished");
}

void CParseHelper::ParseRisksRecord(const VS::CResponseRecord& record, RequestMap* req_map, StockInfoMap* update_map)
{
	TRACEINFO(__FUNCTION__,"call");
	std::string OptionSymbol = record.GetField(DDS::FIELD_OPTION_SYMBOL).AsString();
	RequestMap::const_iterator request = req_map->find(OptionSymbol);
	if (request != req_map->end())
	{
		StockInfoMap::const_iterator info = update_map->find(OptionSymbol);
		if (info == update_map->end())
		{
			StockResultsExFullInfoPtr ptr(new StockResultsExFullInfo());
			ptr->Params = *(request->second);
			info = update_map->insert(StockInfoMap::value_type(OptionSymbol,ptr)).first;
		}
		StockResultsExFullInfoPtr ptr = info->second;
		/*ptr->Info.DivFreq		= record.GetField(DDS::FIELD_STOCK_DIV_FREQ).AsShort();
		ptr->Info.DivAmount		= record.GetField(DDS::FIELD_STOCK_DIV_AMT_YIELD).AsFloat();
		ptr->Info.DivLastDate	= record.GetField(DDS::FIELD_STOCK_DIV_DATE).AsDateTime();
		ptr->Info.Currency		= (_bstr_t(record.GetField(DDS::FIELD_CURRENCY).AsString().c_str())).copy();
		ptr->Info.StockType		= (record.GetField(DDS::FIELD_STOCK_TYPE).AsString()[0] == 'S')? StockTypeEnum::enStock : StockTypeEnum::enIndex;*/
	}
	TRACEINFO(__FUNCTION__,"finished");
}

void CParseHelper::ParseOptionBaseRecord(const VS::CResponseRecord& record, RequestMap* req_map, OptionInfoMap* update_map)
{
	TRACEINFO(__FUNCTION__,"call");
	std::string StockSymbol = record.GetField(DDS::FIELD_STOCK_SYMBOL).AsString();
	std::string OptionSymbol = record.GetField(DDS::FIELD_OPTION_SYMBOL).AsString();
	RequestMap::const_iterator request = req_map->find(OptionSymbol);
	if (request != req_map->end())
	{
		OptionInfoMap::const_iterator info = update_map->find(OptionSymbol);
		if (info == update_map->end())
		{
			OptionResultsExFullInfoPtr ptr(new OptionResultsExFullInfo());
			ptr->Params = *(request->second);
			info = update_map->insert(OptionInfoMap::value_type(OptionSymbol,ptr)).first;
		}
		OptionResultsExFullInfoPtr ptr = info->second;

		ptr->Info.ExpirationDate = record.GetField(DDS::FIELD_EXPIRATION).AsDateTime();
		ptr->Info.OptionType =( (OptionTypeEnum)record.GetField(DDS::FIELD_OPTION_TYPE).AsString()[0] == '0') ? (OptionTypeEnum)enCall : (OptionTypeEnum)enPut;
		ptr->Info.OpenInterest = record.GetField(DDS::FIELD_OPEN_INTEREST).AsInt();
		ptr->Info.StrikePrice = record.GetField(DDS::FIELD_STRIKE).AsDouble();
	}
	TRACEINFO(__FUNCTION__,"finished");
}

void CParseHelper::ParseEquityQuoteRecord(const VS::CResponseRecord& record, RequestMap* req_map, StockInfoMap* update_map)
{
	TRACEINFO(__FUNCTION__,"call");
	std::string StockSymbol = record.GetField(DDS::FIELD_STOCK_SYMBOL).AsString();
	RequestMap::const_iterator request = req_map->find(StockSymbol);
	if (request != req_map->end())
	{
		StockInfoMap::const_iterator info = update_map->find(StockSymbol);
		if (info == update_map->end())
		{
			StockResultsExFullInfoPtr ptr(new StockResultsExFullInfo());
			ptr->Params = *(request->second);
			info = update_map->insert(StockInfoMap::value_type(StockSymbol,ptr)).first;
		}
		StockResultsExFullInfoPtr ptr = info->second;
		ptr->Info.Ask = record.GetField(DDS::FIELD_ASK).AsDouble();
		ptr->Info.Bid = record.GetField(DDS::FIELD_BID).AsDouble();
		ptr->Info.Volume = static_cast<double>(record.GetField(DDS::FIELD_VOLUME).AsLongLong());
	}
	TRACEINFO(__FUNCTION__,"finished");
}

void CParseHelper::ParseOptionQuoteRecord(const VS::CResponseRecord& record, RequestMap* req_map, OptionInfoMap* update_map)
{
	TRACEINFO(__FUNCTION__,"call");
	std::string StockSymbol = record.GetField(DDS::FIELD_STOCK_SYMBOL).AsString();
	std::string OptionSymbol = record.GetField(DDS::FIELD_OPTION_SYMBOL).AsString();
	RequestMap::const_iterator request = req_map->find(OptionSymbol);
	if (request != req_map->end())
	{
		OptionInfoMap::const_iterator info = update_map->find(OptionSymbol);
		if (info == update_map->end())
		{
			OptionResultsExFullInfoPtr ptr(new OptionResultsExFullInfo());
			ptr->Params = *(request->second);
			info = update_map->insert(OptionInfoMap::value_type(OptionSymbol,ptr)).first;
		}
		OptionResultsExFullInfoPtr ptr = info->second;
		ptr->Info.Ask = record.GetField(DDS::FIELD_ASK).AsDouble();
		ptr->Info.Bid = record.GetField(DDS::FIELD_BID).AsDouble();
		ptr->Info.Last = record.GetField(DDS::FIELD_LAST).AsDouble();
	}
	TRACEINFO(__FUNCTION__,"finished");
}


void CFieldCollections::Initialize()
{
	TRACEINFO(__FUNCTION__,"call");
	DDS::IVFieldList seq_fields;

	//Stock base
	seq_fields.length(23);
	int indx(0);
	seq_fields[indx++] =		DDS::FIELD_STOCK_SYMBOL;
	seq_fields[indx++] =		DDS::FIELD_STOCK_DIV_DATE;
	seq_fields[indx++] =		DDS::FIELD_STOCK_DIV_FREQ;
	seq_fields[indx++] =		DDS::FIELD_STOCK_DIV_AMT_YIELD;
	seq_fields[indx++] =		DDS::FIELD_CURRENCY;
	seq_fields[indx++] =		DDS::FIELD_STOCK_TYPE;
	seq_fields[indx++] =		DDS::FIELD_PRICE_CLOSE;
	seq_fields[indx++] =		DDS::FIELD_PRICE_HIGH52WK;
	seq_fields[indx++] =		DDS::FIELD_PRICE_LOW52WK;
	seq_fields[indx++] =		DDS::FIELD_SOD_IVX30;
	seq_fields[indx++] =		DDS::FIELD_SOD_IVX30_YEST;
	seq_fields[indx++] =		DDS::FIELD_SOD_IVX30_HIGH52WK;
	seq_fields[indx++] =		DDS::FIELD_SOD_IVX30_LOW52WK;
	seq_fields[indx++] =		DDS::FIELD_SOD_HV20;
	seq_fields[indx++] =		DDS::FIELD_SOD_HV20_YEST;
	seq_fields[indx++] =		DDS::FIELD_SOD_HV20_HIGH52WK;
	seq_fields[indx++] =		DDS::FIELD_SOD_HV20_LOW52WK;
	seq_fields[indx++] =		DDS::FIELD_SOD_OPT_VOLUME_CALL;
	seq_fields[indx++] =		DDS::FIELD_SOD_OPT_VOLUME_PUT;
	seq_fields[indx++] =		DDS::FIELD_SOD_OPT_VOLUME_AVG;
	seq_fields[indx++] =		DDS::FIELD_OUTSTANDING_SHARES;
	EquityBaseFields <<= seq_fields;

	//Stock Quote
	seq_fields.length(14);
	indx = 0;
	seq_fields[indx++] =		DDS::FIELD_STOCK_SYMBOL;
	seq_fields[indx++] =		DDS::FIELD_BID;
	seq_fields[indx++] =		DDS::FIELD_BID_EXCHANGE;
	seq_fields[indx++] =		DDS::FIELD_BID_DATE;
	seq_fields[indx++] =		DDS::FIELD_BID_SIZE;
	seq_fields[indx++] =		DDS::FIELD_ASK;
	seq_fields[indx++] =		DDS::FIELD_ASK_EXCHANGE;
	seq_fields[indx++] =		DDS::FIELD_ASK_DATE;
	seq_fields[indx++] =		DDS::FIELD_ASK_SIZE;
	seq_fields[indx++] =		DDS::FIELD_LAST;
	seq_fields[indx++] =		DDS::FIELD_LAST_EXCHANGE;
	seq_fields[indx++] =		DDS::FIELD_LAST_DATE;
	seq_fields[indx++] =		DDS::FIELD_LAST_SIZE;
	seq_fields[indx++] =		DDS::FIELD_VOLUME;
	EquityQuoteFields <<= seq_fields;

	seq_fields.length(4);
	indx = 0;
	seq_fields[indx++] =		DDS::FIELD_STOCK_SYMBOL;
	seq_fields[indx++] =		DDS::FIELD_STOCK_DIV_DATE;
	seq_fields[indx++] =		DDS::FIELD_STOCK_DIV_FREQ;
	seq_fields[indx++] =		DDS::FIELD_STOCK_DIV_AMT_YIELD;
	DividendsFields <<= seq_fields;

	seq_fields.length(3);
	indx = 0;
	seq_fields[indx++] =		DDS::FIELD_CURRENCY;
	seq_fields[indx++] =		DDS::FIELD_IR_PERIOD;
	seq_fields[indx++] =		DDS::FIELD_INTEREST_RATE;
	RatesFields <<= seq_fields;


	// Option base
	seq_fields.length(7);
	indx = 0;
	seq_fields[indx++] = DDS::FIELD_OPTION_SYMBOL;
	seq_fields[indx++] = DDS::FIELD_STOCK_SYMBOL;
	seq_fields[indx++] = DDS::FIELD_EXPIRATION;
	seq_fields[indx++] = DDS::FIELD_STRIKE;
	seq_fields[indx++] = DDS::FIELD_OPTION_TYPE;
	seq_fields[indx++] = DDS::FIELD_OPTION_STYLE;
	seq_fields[indx++] = DDS::FIELD_OPEN_INTEREST;
	OptionBaseFields <<= seq_fields;

	//Option quote
	seq_fields.length(22);
	indx = 0;
	seq_fields[indx++] = DDS::FIELD_OPTION_SYMBOL;
	seq_fields[indx++] = DDS::FIELD_STOCK_SYMBOL;
	seq_fields[indx++] = DDS::FIELD_IV;
	seq_fields[indx++] = DDS::FIELD_PREIV;
	seq_fields[indx++] = DDS::FIELD_DELTA;
	seq_fields[indx++] = DDS::FIELD_GAMMA;
	seq_fields[indx++] = DDS::FIELD_VEGA;
	seq_fields[indx++] = DDS::FIELD_THETA;
	seq_fields[indx++] = DDS::FIELD_RHO;
	seq_fields[indx++] = DDS::FIELD_BID;
	seq_fields[indx++] = DDS::FIELD_BID_DATE;
	seq_fields[indx++] = DDS::FIELD_BID_EXCHANGE;
	seq_fields[indx++] = DDS::FIELD_BID_SIZE;
	seq_fields[indx++] = DDS::FIELD_ASK;
	seq_fields[indx++] = DDS::FIELD_ASK_DATE;
	seq_fields[indx++] = DDS::FIELD_ASK_EXCHANGE;
	seq_fields[indx++] = DDS::FIELD_ASK_SIZE;
	seq_fields[indx++] = DDS::FIELD_LAST;
	seq_fields[indx++] = DDS::FIELD_LAST_DATE;
	seq_fields[indx++] = DDS::FIELD_LAST_EXCHANGE;
	seq_fields[indx++] = DDS::FIELD_LAST_SIZE;
	seq_fields[indx++] = DDS::FIELD_VOLUME;
	OptionQuoteFields <<= seq_fields;
	TRACEINFO(__FUNCTION__,"finished");
}

CORBA::Any CFieldCollections::OptionBaseFields;
CORBA::Any CFieldCollections::EquityBaseFields;
CORBA::Any CFieldCollections::OptionQuoteFields;
CORBA::Any CFieldCollections::EquityQuoteFields;
CORBA::Any CFieldCollections::DividendsFields;
CORBA::Any CFieldCollections::RatesFields;
