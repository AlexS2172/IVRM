#pragma once
#include "stdafx.h"

class CParseHelper
{
public:
	static void InitQUI(QuoteUpdateInfo* pQuoteIndo);
	static void InitRISKS(IRisks* pRisksInfo);
	static void ParseEquityBaseRecord	(const VS::CResponseRecord& record, EquiteBaseInfoPtr eqInfo);
	static void ParseOptionBaseRecord	(const VS::CResponseRecord& record, OptionBaseInfoPtr opInfo);
	static void ParseEquityQuoteRecord	(const VS::CResponseRecord& record, EquiteQuoteInfo* eqQuote);
	static void ParseOptionQuoteRecord	(const VS::CResponseRecord& record, OptionQuoteInfo* opQuote);
	static void ParseRisksRecord		(const VS::CResponseRecord& record, OptionQuoteInfo* opQuote);
	static void ParseEquityBaseRecord	(const VS::CResponseRecord& record, QuoteUpdateMap* req_map, QuoteUpdateInfoMap* update_map);
	static void ParseOptionBaseRecord	(const VS::CResponseRecord& record, QuoteUpdateMap* req_map, QuoteUpdateInfoMap* update_map);
	static void ParseEquityQuoteRecord	(const VS::CResponseRecord& record, QuoteUpdateMap* req_map, QuoteUpdateInfoMap* update_map);
	static void ParseOptionQuoteRecord	(const VS::CResponseRecord& record, QuoteUpdateMap* req_map, QuoteUpdateInfoMap* update_map);
	static void ParseRisksRecord		(const VS::CResponseRecord& record, RisksUpdateInfoMap* req_map, RisksUpdateMap* update_map);
	static void ParseEquityBaseRecord	(const VS::CResponseRecord& record, RequestMap* req_map, StockInfoMap* update_map);
	static void ParseOptionBaseRecord	(const VS::CResponseRecord& record, RequestMap* req_map, OptionInfoMap* update_map);
	static void ParseEquityQuoteRecord	(const VS::CResponseRecord& record, RequestMap* req_map, StockInfoMap* update_map);
	static void ParseOptionQuoteRecord	(const VS::CResponseRecord& record, RequestMap* req_map, OptionInfoMap* update_map);
	static void	ParseRisksRecord		(const VS::CResponseRecord& record, RequestMap* req_map, StockInfoMap* update_map);
};

class CFieldCollections
{
public:
	static CORBA::Any							OptionBaseFields;
	static CORBA::Any							EquityBaseFields;
	static CORBA::Any							OptionQuoteFields;
	static CORBA::Any							EquityQuoteFields;
	static CORBA::Any							DividendsFields;
	static CORBA::Any							RatesFields;
	static void Initialize();
};

//-------------------------------------------------------------------------------------//
//class CResponseParser
//{
//public:
//	static CRequestKeyPtr	GetRequestKey(const VS::CResponseRecord& record, long messageType);
//	static void	ParseOptionQuote(const VS::CResponseRecord& record, CResponsePtr& spResponse);
//	static void	ParseEquityQuote(const VS::CResponseRecord& record, CResponsePtr& spResponse);
//	static void ParseRisks(const VS::CResponseRecord& record, CResponsePtr& spResponse);
//	static void ParseResponse(const VS::CResponseRecord& record, long messageType, CResponsePtr& spResponse);
//};
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//