#ifndef __EODLOADCLOSEPRICESTAGE_H__
#define __EODLOADCLOSEPRICESTAGE_H__

#include "EodGeneral.h"

namespace EOD
{

	/////////////////////////////////////////////////////////////////////////////
	//
	enum EodLoadClosePriceStage
	{	
		enEodCpInvalid = 0,
		enEodCpLoadFromIv,
		enEodCpLoadFromPp,
		enEodCpLoadFutureFromPp
	};

};

#endif //__EODLOADCLOSEPRICESTAGE_H__