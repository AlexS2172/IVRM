#pragma once

#include <VolatilityServerCore\VSDefinitions.h>
#include <MsgAssembler/StreamWriterFactory.h>

class CTableInitializer
{
public:
	CTableInitializer(void);
	~CTableInitializer(void);

	static void InitializeWriter();
	static void GetEquityQuoteTable(VS::FieldIDList& fieldIDList, VS::IDList& idList);
	static void GetOptionQuoteTable(VS::FieldIDList& fieldIDList, VS::IDList& idList);
	static void GetFutureQuoteTable(VS::FieldIDList &fieldIDList, VS::IDList &idList);
	static void GetFutureOptionQuoteTable(VS::FieldIDList &fieldIDList, VS::IDList &idList);
};
