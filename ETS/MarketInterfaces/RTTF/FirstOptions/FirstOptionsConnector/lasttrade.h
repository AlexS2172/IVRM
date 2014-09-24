#ifndef _LAST_TRADE_H
#define _LAST_TRADE_H

#include "clearingbase.h"


//##ModelId=3C1DD43200F1
class CLastTrade
{
public:

	CLastTrade();
	//##ModelId=3C1DD4320119
	virtual ~CLastTrade();

	// Must call after creating object
	//##ModelId=3C1DD4320112
	bool Init();

	//##ModelId=3C1DD432010F
	bool GetDate(FOC_DATETIME& dttm) const;
	//##ModelId=3C1DD4320108
	bool SetDate(const FOC_DATETIME& dttm);
	//##ModelId=3C1DD4320105
	bool IsFirstStartOfToday(void) const { return m_bIsFirstStartOfToday; }

private:

	//##ModelId=3C1DD43200FC
	bool ResetTradeTime();

private:

	//##ModelId=3C1DD43200FB
	bool m_bIsFirstStartOfToday;
	//FOC_DATETIME m_Time;
};


#endif
