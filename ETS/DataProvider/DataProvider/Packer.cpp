#include "StdAfx.h"
#include "Packer.h"

#define PUSH_DATA(__list, __data)		\
__list.push_back(VS::CFieldData(__data))
//------------------------------------------------------------------------------------//
CPacker::CPacker(void)
{
}
//------------------------------------------------------------------------------------//
CPacker::~CPacker(void)
{
}
//------------------------------------------------------------------------------------//
void CPacker::PackEquityQuote(QuoteAtomPtr& spQuote, VS::FieldDataList& fieldDataList)
{
	PUSH_DATA(fieldDataList, spQuote->m_spRequest->Symbol.c_str());
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->BidPrice));				
	PUSH_DATA(fieldDataList, VS::CDateTimeType(spQuote->m_spResponse->BidDateTime));			
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->BidExchange.c_str());
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->BidSize);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->AskPrice));				
	PUSH_DATA(fieldDataList, VS::CDateTimeType(spQuote->m_spResponse->AskDateTime));			
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->AskExchange.c_str());
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->AskSize);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->LastPrice));				
	PUSH_DATA(fieldDataList, VS::CDateTimeType(spQuote->m_spResponse->LastDateTime));			
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->Exchange.c_str());
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->BidSize);
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->Volume);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->OpenPrice));
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->NetChange));
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->PriceTick);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->HighPrice));
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->LowPrice));
	PUSH_DATA(fieldDataList, static_cast<long>(spQuote->m_spResponse->OpenInterest));
};
//------------------------------------------------------------------------------------//
void CPacker::PackOptionQuote(QuoteAtomPtr& spQuote, VS::FieldDataList& fieldDataList)
{
	PUSH_DATA(fieldDataList, spQuote->m_spRequest->Symbol.c_str());
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->BaseContract.c_str());
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->BidPrice));				
	PUSH_DATA(fieldDataList, VS::CDateTimeType(spQuote->m_spResponse->BidDateTime));			
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->BidExchange.c_str());
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->BidSize);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->AskPrice));				
	PUSH_DATA(fieldDataList, VS::CDateTimeType(spQuote->m_spResponse->AskDateTime));			
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->AskExchange.c_str());
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->AskSize);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->LastPrice));				
	PUSH_DATA(fieldDataList, VS::CDateTimeType(spQuote->m_spResponse->LastDateTime));			
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->Exchange.c_str());
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->BidSize);
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->Volume);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->OpenPrice));
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->NetChange));
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->PriceTick);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->HighPrice));
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->LowPrice));
	PUSH_DATA(fieldDataList, static_cast<long>(spQuote->m_spResponse->OpenInterest));
};
//------------------------------------------------------------------------------------//
void CPacker::PackFutureQuote(QuoteAtomPtr& spQuote, VS::FieldDataList& fieldDataList)
{
	PUSH_DATA(fieldDataList, spQuote->m_spRequest->Symbol.c_str());
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->BaseContract.c_str());
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->BidPrice));				
	PUSH_DATA(fieldDataList, VS::CDateTimeType(spQuote->m_spResponse->BidDateTime));			
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->BidExchange.c_str());
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->BidSize);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->AskPrice));				
	PUSH_DATA(fieldDataList, VS::CDateTimeType(spQuote->m_spResponse->AskDateTime));			
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->AskExchange.c_str());
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->AskSize);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->LastPrice));				
	PUSH_DATA(fieldDataList, VS::CDateTimeType(spQuote->m_spResponse->LastDateTime));			
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->Exchange.c_str());
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->BidSize);
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->Volume);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->OpenPrice));
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->NetChange));
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->PriceTick);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->HighPrice));
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->LowPrice));
	PUSH_DATA(fieldDataList, static_cast<long>(spQuote->m_spResponse->OpenInterest));
};
//------------------------------------------------------------------------------------//
void CPacker::PackFutureOptionQuote(QuoteAtomPtr& spQuote, VS::FieldDataList& fieldDataList)
{
	PUSH_DATA(fieldDataList, spQuote->m_spRequest->Symbol.c_str());
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->BaseContract.c_str());
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->BidPrice));				
	PUSH_DATA(fieldDataList, VS::CDateTimeType(spQuote->m_spResponse->BidDateTime));			
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->BidExchange.c_str());
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->BidSize);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->AskPrice));				
	PUSH_DATA(fieldDataList, VS::CDateTimeType(spQuote->m_spResponse->AskDateTime));			
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->AskExchange.c_str());
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->AskSize);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->LastPrice));				
	PUSH_DATA(fieldDataList, VS::CDateTimeType(spQuote->m_spResponse->LastDateTime));			
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->Exchange.c_str());
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->BidSize);
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->Volume);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->OpenPrice));
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->NetChange));
	PUSH_DATA(fieldDataList, spQuote->m_spResponse->PriceTick);
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->HighPrice));
	PUSH_DATA(fieldDataList, static_cast<float>(spQuote->m_spResponse->LowPrice));
	PUSH_DATA(fieldDataList, static_cast<long>(spQuote->m_spResponse->OpenInterest));
};
//------------------------------------------------------------------------------------//
void CPacker::PackQuote(QuoteAtomPtr &spQuote, VS::FieldDataList &fieldDataList)
{
	CAutoLock lock(spQuote->m_lock);

	try
	{
		if (spQuote->m_spRequest->Type == enSTK || spQuote->m_spRequest->Type == enIDX)
		{
			PackEquityQuote(spQuote, fieldDataList);
		}
		else if (spQuote->m_spRequest->Type == enOPT)
		{
			PackOptionQuote(spQuote, fieldDataList);
		}
		else if (spQuote->m_spRequest->Type == enFUT)
		{
			PackFutureQuote(spQuote, fieldDataList);
		}
		else if (spQuote->m_spRequest->Type == enFOP)
		{
			PackFutureOptionQuote(spQuote, fieldDataList);
		}
	}
	catch (std::exception& e)
	{
		LOG4CPLUS_ERROR(ProviderLog, _T("std::exception occured while packing quote: ") << e.what() <<
									 _T(" Quote: ") << spQuote->m_spRequest->Symbol << 
									 _T(" Type: ") << spQuote->m_spRequest->Type );
		throw e;
	}
	catch (CSeException* e)
	{
		CString strErrDescription;
		e->GetErrorMessage(strErrDescription);
		LOG4CPLUS_FATAL(ProviderLog , _T("SEH exception occured while PackQuote() ") << strErrDescription <<
									_T(" Quote: ") << spQuote->m_spRequest->Symbol << 
									_T(" Type: ") << spQuote->m_spRequest->Type );
		throw e;
	}
	catch (...)
	{
		LOG4CPLUS_ERROR(ProviderLog , _T("Unknown C++ exception occured while PackQuote().") <<
									_T(" Quote: ") << spQuote->m_spRequest->Symbol << 
									_T(" Type: ") << spQuote->m_spRequest->Type );
		throw;
	}
};
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
