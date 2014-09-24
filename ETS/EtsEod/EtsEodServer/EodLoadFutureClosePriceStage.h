#ifndef __EODLOADFUTURECLOSEPRICESTAGE_H__
#define __EODLOADFUTURECLOSEPRICESTAGE_H__

#include "EodGeneral.h"

namespace EOD
{

	/////////////////////////////////////////////////////////////////////////////
	//
	enum EodLoadFutureClosePriceStage
	{	
		enEodInvalid = 0,
		enEodLoadFuturePricesFromPp,
		enEodLoadFutureOptionPricesFromPp
	};

};

#endif //__EODLOADCLOSEPRICESTAGE_H__