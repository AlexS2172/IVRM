#pragma once
#include "abstractcontract.h"

class CSpotContract: public CAbstractContract
{
public:
	CSpotContract(void);
	~CSpotContract(void);

public:

	VARIANT_BOOL m_bShouldMultOptDeltaEq;

	LONG m_nLotSize;

	DOUBLE m_dSkew;
	DOUBLE m_dKurt;

	DOUBLE m_dUndPosForRates;

	_bstr_t m_bstrPrimaryExchImportID;
	LONG m_nPrimaryExchangeID;

	LONG m_nTotalQtyInShares;
	DOUBLE m_dKOptDeltaEq;
	
public:
	//------------------------------------------------------------------------------------------//
	CRisksPtr
	Calculate(CMarketSimulator::ptr MarketSimulator);
	//------------------------------------------------------------------------------------------//
	/*virtual*/
	void 
	AssignGREEKS(GREEKS& resultGreeks, CRisksPtr Risks);
	//------------------------------------------------------------------------------------------//
	void 
	FitToMarketVolatility();
	//------------------------------------------------------------------------------------------//
	double	GetSkew() {return m_dSkew;}
	double	GetKurt() {return m_dKurt;}
	//------------------------------------------------------------------------------------------//
	void	Clear();
	//------------------------------------------------------------------------------------------//
};
