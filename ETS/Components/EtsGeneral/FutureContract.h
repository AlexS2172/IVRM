#pragma once
#include "abstractcontract.h"
#include "EtsPriceProfileAtom.h"

class CFutureContract: public CAbstractContract
{
public:

	LONG					m_nFutRootID;
	IEtsPriceProfileAtomPtr	m_spUndPriceProfile;
	IEtsPriceProfileAtomPtr	m_spOptPriceProfile;
	DATE					m_dtMaturityDate;
	VARIANT_BOOL			m_bIsAmerican;
	VARIANT_BOOL			m_bIsActive;
	VARIANT_BOOL			m_bHaveOptions;

	VARIANT_BOOL			m_bShowInFutureContractEq;
	DOUBLE					m_dTickSize;
	DOUBLE					m_dTickValue;
	DOUBLE					m_dPriceQuotationUnit;
	DOUBLE					m_dQuoteFormat;
	double					m_dK;
	double					m_dKEq;
	VARIANT_BOOL            m_bMultOptDltEq;
	DOUBLE					m_dBasis;
	VARIANT_BOOL			m_bUseInCalc;
	DOUBLE					m_dActiveFutureRatio;

	CFutureContract();
	~CFutureContract();

public:
	//------------------------------------------------------------------------------------------//
	void	Clear();
	//------------------------------------------------------------------------------------------//
	CRisksPtr
	Calculate(CMarketSimulator::ptr MarketSimulator);
	//------------------------------------------------------------------------------------------//
	/*virtual*/
	double
	getMaturityWeight(double Period, double CurrentDate);
	//------------------------------------------------------------------------------------------//
	/*virtual*/
	IEtsPriceProfileAtomPtr
	getPriceProfile();
	//------------------------------------------------------------------------------------------//
	/*virtual*/
	void
	AssignGREEKS(GREEKS& resultGreeks, CRisksPtr Risks);
	//------------------------------------------------------------------------------------------//
	/*virtual*/
	double
	getContractSizeInAsset();
	//------------------------------------------------------------------------------------------//
};
