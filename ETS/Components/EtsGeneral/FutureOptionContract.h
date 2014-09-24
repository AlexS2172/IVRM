#pragma once
#include "optioncontract.h"

class CFutureOptionContract: public COptionContract
{
public:
	CFutureOptionContract(void);
	~CFutureOptionContract(void);

public:
	//-----------------------------------------------------------------------------------------------------//
	void	Clear();
	//-----------------------------------------------------------------------------------------------------//
	CRisksPtr	
	Calculate(CMarketSimulator::ptr MarketSimulator);
	//-----------------------------------------------------------------------------------------------------//
	/*virtual*/
	IEtsPriceProfileAtomPtr
	getPriceProfile();
};
