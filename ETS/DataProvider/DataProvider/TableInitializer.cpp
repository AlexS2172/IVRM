#include "StdAfx.h"
#include "TableInitializer.h"
#include <MsgParser/StreamFactory.h>

namespace IVRMFields{
	static const CORBA::Short	FIELD_OPEN_PRICE	=	296;
	static const CORBA::Short	FIELD_NET_CHANGE_1	=	297;
	static const CORBA::Short	FIELD_PRICE_TICK	=	298;
};

#define ADD_FIELD(__Id, __Type)		\
VS::CStreamWriterFactory::Instance().AddField(__Id, VS::CStreamFactory::__Type)

#define PUSH_FILED(__list, __Id)	\
__list.push_back(__Id)
//---------------------------------------------------------------//
CTableInitializer::CTableInitializer(void)
{
};
//---------------------------------------------------------------//
CTableInitializer::~CTableInitializer(void)
{
};
//---------------------------------------------------------------//
void CTableInitializer::InitializeWriter()
{
	ADD_FIELD(DDS::FIELD_STOCK_SYMBOL,			STREAM_STRING_ID);
	ADD_FIELD(DDS::FIELD_BID,					STREAM_FLOAT_ID);
	ADD_FIELD(DDS::FIELD_BID_DATE,				STREAM_DATETIME_ID);
	ADD_FIELD(DDS::FIELD_BID_EXCHANGE,			STREAM_EXCHANGE_ID);
	ADD_FIELD(DDS::FIELD_BID_SIZE,				STREAM_INT_ID);
	ADD_FIELD(DDS::FIELD_ASK,					STREAM_FLOAT_ID);
	ADD_FIELD(DDS::FIELD_ASK_DATE,				STREAM_DATETIME_ID);
	ADD_FIELD(DDS::FIELD_ASK_EXCHANGE,			STREAM_EXCHANGE_ID);
	ADD_FIELD(DDS::FIELD_ASK_SIZE,				STREAM_INT_ID);
	ADD_FIELD(DDS::FIELD_LAST,					STREAM_FLOAT_ID);
	ADD_FIELD(DDS::FIELD_LAST_DATE,				STREAM_DATETIME_ID);
	ADD_FIELD(DDS::FIELD_LAST_EXCHANGE,			STREAM_EXCHANGE_ID);
	ADD_FIELD(DDS::FIELD_LAST_SIZE,				STREAM_INT_ID);
	ADD_FIELD(DDS::FIELD_VOLUME,				STREAM_INT_ID);
	ADD_FIELD(DDS::FIELD_OPTION_SYMBOL,			STREAM_64BYTE_STRING_ID);
	ADD_FIELD(DDS::FIELD_FUTURE_SYMBOL,			STREAM_STRING_ID);
	ADD_FIELD(DDS::FIELD_FUTURE_ROOT_SYMBOL,	STREAM_STRING_ID);
	ADD_FIELD(DDS::FIELD_FUTURE_OPTION_SYMBOL,	STREAM_64BYTE_STRING_ID);
	ADD_FIELD(DDS::FIELD_OPEN_INTEREST,			STREAM_INT_ID);
	ADD_FIELD(DDS::FIELD_PRICE_HIGH,			STREAM_FLOAT_ID);
	ADD_FIELD(DDS::FIELD_PRICE_LOW,				STREAM_FLOAT_ID);
	ADD_FIELD(IVRMFields::FIELD_OPEN_PRICE,		STREAM_FLOAT_ID);
	ADD_FIELD(IVRMFields::FIELD_NET_CHANGE_1,	STREAM_FLOAT_ID);
	ADD_FIELD(IVRMFields::FIELD_PRICE_TICK,		STREAM_INT_ID);
};
//---------------------------------------------------------------//
void CTableInitializer::GetEquityQuoteTable(VS::FieldIDList& fieldIDList, VS::IDList& idList)
{
	PUSH_FILED(fieldIDList, DDS::FIELD_STOCK_SYMBOL);
	PUSH_FILED(fieldIDList, DDS::FIELD_BID);				
	PUSH_FILED(fieldIDList, DDS::FIELD_BID_DATE);			
	PUSH_FILED(fieldIDList, DDS::FIELD_BID_EXCHANGE);
	PUSH_FILED(fieldIDList, DDS::FIELD_BID_SIZE);
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK);				
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK_DATE);			
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK_EXCHANGE);
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK_SIZE);
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST);				
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST_DATE);			
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST_EXCHANGE);
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST_SIZE);
	PUSH_FILED(fieldIDList, DDS::FIELD_VOLUME);

	PUSH_FILED(fieldIDList, IVRMFields::FIELD_OPEN_PRICE);
	PUSH_FILED(fieldIDList, IVRMFields::FIELD_NET_CHANGE_1);
	PUSH_FILED(fieldIDList, IVRMFields::FIELD_PRICE_TICK);
	PUSH_FILED(fieldIDList,	DDS::FIELD_PRICE_HIGH);
	PUSH_FILED(fieldIDList,	DDS::FIELD_PRICE_LOW);
	PUSH_FILED(fieldIDList,	DDS::FIELD_OPEN_INTEREST);

	for (size_t i = 0; i < fieldIDList.size(); ++i)
	idList.push_back(static_cast<long>(i));
};
//---------------------------------------------------------------//
void CTableInitializer::GetOptionQuoteTable(VS::FieldIDList &fieldIDList, VS::IDList &idList)
{
	PUSH_FILED(fieldIDList, DDS::FIELD_OPTION_SYMBOL);
	PUSH_FILED(fieldIDList, DDS::FIELD_STOCK_SYMBOL);
	PUSH_FILED(fieldIDList, DDS::FIELD_BID);				
	PUSH_FILED(fieldIDList, DDS::FIELD_BID_DATE);			
	PUSH_FILED(fieldIDList, DDS::FIELD_BID_EXCHANGE);
	PUSH_FILED(fieldIDList, DDS::FIELD_BID_SIZE);
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK);				
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK_DATE);			
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK_EXCHANGE);
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK_SIZE);
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST);				
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST_DATE);			
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST_EXCHANGE);
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST_SIZE);
	PUSH_FILED(fieldIDList, DDS::FIELD_VOLUME);

	PUSH_FILED(fieldIDList, IVRMFields::FIELD_OPEN_PRICE);
	PUSH_FILED(fieldIDList, IVRMFields::FIELD_NET_CHANGE_1);
	PUSH_FILED(fieldIDList, IVRMFields::FIELD_PRICE_TICK);
	PUSH_FILED(fieldIDList,	DDS::FIELD_PRICE_HIGH);
	PUSH_FILED(fieldIDList,	DDS::FIELD_PRICE_LOW);
	PUSH_FILED(fieldIDList,	DDS::FIELD_OPEN_INTEREST);

	for (size_t i = 0; i < fieldIDList.size(); ++i)
	idList.push_back(static_cast<long>(i));
};
//---------------------------------------------------------------//
void CTableInitializer::GetFutureQuoteTable(VS::FieldIDList &fieldIDList, VS::IDList &idList)
{
	PUSH_FILED(fieldIDList, DDS::FIELD_FUTURE_SYMBOL);
	PUSH_FILED(fieldIDList, DDS::FIELD_FUTURE_ROOT_SYMBOL);
	PUSH_FILED(fieldIDList, DDS::FIELD_BID);				
	PUSH_FILED(fieldIDList, DDS::FIELD_BID_DATE);			
	PUSH_FILED(fieldIDList, DDS::FIELD_BID_EXCHANGE);
	PUSH_FILED(fieldIDList, DDS::FIELD_BID_SIZE);
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK);				
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK_DATE);			
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK_EXCHANGE);
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK_SIZE);
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST);				
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST_DATE);			
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST_EXCHANGE);
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST_SIZE);
	PUSH_FILED(fieldIDList, DDS::FIELD_VOLUME);

	PUSH_FILED(fieldIDList, IVRMFields::FIELD_OPEN_PRICE);
	PUSH_FILED(fieldIDList, IVRMFields::FIELD_NET_CHANGE_1);
	PUSH_FILED(fieldIDList, IVRMFields::FIELD_PRICE_TICK);
	PUSH_FILED(fieldIDList,	DDS::FIELD_PRICE_HIGH);
	PUSH_FILED(fieldIDList,	DDS::FIELD_PRICE_LOW);
	PUSH_FILED(fieldIDList,	DDS::FIELD_OPEN_INTEREST);

	for (size_t i = 0; i < fieldIDList.size(); ++i)
	idList.push_back(static_cast<long>(i));
};
//---------------------------------------------------------------//
void CTableInitializer::GetFutureOptionQuoteTable(VS::FieldIDList &fieldIDList, VS::IDList &idList)
{
	PUSH_FILED(fieldIDList, DDS::FIELD_FUTURE_OPTION_SYMBOL);
	PUSH_FILED(fieldIDList, DDS::FIELD_FUTURE_SYMBOL);
	PUSH_FILED(fieldIDList, DDS::FIELD_BID);				
	PUSH_FILED(fieldIDList, DDS::FIELD_BID_DATE);			
	PUSH_FILED(fieldIDList, DDS::FIELD_BID_EXCHANGE);
	PUSH_FILED(fieldIDList, DDS::FIELD_BID_SIZE);
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK);				
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK_DATE);			
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK_EXCHANGE);
	PUSH_FILED(fieldIDList, DDS::FIELD_ASK_SIZE);
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST);				
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST_DATE);			
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST_EXCHANGE);
	PUSH_FILED(fieldIDList, DDS::FIELD_LAST_SIZE);
	PUSH_FILED(fieldIDList, DDS::FIELD_VOLUME);

	PUSH_FILED(fieldIDList, IVRMFields::FIELD_OPEN_PRICE);
	PUSH_FILED(fieldIDList, IVRMFields::FIELD_NET_CHANGE_1);
	PUSH_FILED(fieldIDList, IVRMFields::FIELD_PRICE_TICK);
	PUSH_FILED(fieldIDList,	DDS::FIELD_PRICE_HIGH);
	PUSH_FILED(fieldIDList,	DDS::FIELD_PRICE_LOW);
	PUSH_FILED(fieldIDList,	DDS::FIELD_OPEN_INTEREST);

	for (size_t i = 0; i < fieldIDList.size(); ++i)
	idList.push_back(static_cast<long>(i));
}
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//