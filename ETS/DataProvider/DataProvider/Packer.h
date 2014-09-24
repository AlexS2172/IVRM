#pragma once

#include <VolatilityServerCore\VSDefinitions.h>

class CPacker
{
public:
	CPacker(void);
	~CPacker(void);
	static void PackQuote(QuoteAtomPtr& spQuote, VS::FieldDataList& fieldDataList);

private:
	static void PackEquityQuote(QuoteAtomPtr& spQuote, VS::FieldDataList& fieldDataList);
	static void PackOptionQuote(QuoteAtomPtr& spQuote, VS::FieldDataList& fieldDataList);
	static void PackFutureQuote(QuoteAtomPtr& spQuote, VS::FieldDataList& fieldDataList);
	static void PackFutureOptionQuote(QuoteAtomPtr& spQuote, VS::FieldDataList& fieldDataList);
};
