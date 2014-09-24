#pragma once
#include "OptionContract.h"

class CSpotOptionContract: public COptionContract
{
public:
	CSpotOptionContract(void);
	~CSpotOptionContract(void);

public:
	//----------------------------------------------------------------------------------------------------//
	CRisksPtr	
	Calculate(CMarketSimulator::ptr MarketSimulator);
	//----------------------------------------------------------------------------------------------------//
	/*virtual*/
	IEtsPriceProfileAtomPtr
	getPriceProfile();
	//----------------------------------------------------------------------------------------------------//
public:
	void		Clear();
};
