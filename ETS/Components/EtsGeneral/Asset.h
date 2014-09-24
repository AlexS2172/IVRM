#pragma once
#include "CommonSPtr.h"
#include "AbstractContract.h"

class ForwardCurve;

class CAsset
{
public:
	CAsset(void);
	~CAsset(void);

public:

	typedef CAsset* ptr;

	LONG m_nID; //Asset

	EtsContractTypeEnum m_enUndType; //Asset

	_bstr_t m_bstrSymbol; //
	_bstr_t m_bstrContractName; //Asset

	VARIANT_BOOL m_bIsAmerican; //asset
	VARIANT_BOOL m_bIsHTB; //Asset
	VARIANT_BOOL m_bIsTraderContract; // Asset 
	
	VARIANT_BOOL m_bHaveSyntheticRoots; //Asset
	ISynthRootCollPtr m_spSyntheticRoots; //Asset

	VARIANT_BOOL m_bHaveOptions; //Asset
	VARIANT_BOOL m_bIsActive; // Asset 

	IEtsPriceProfileAtomPtr m_spUndPriceProfile; //Asset
	IEtsPriceProfileAtomPtr m_spOptPriceProfile; //Asset

	VARIANT_BOOL m_bIsBasketIndex; //Asset
	LONG m_nVolume10Day; //Asset
	LONG m_nVolume5Expiration; //Asset
	IEtsOptRootCollPtr m_spRoots; //Asset

	IEtsFutRootCollPtr m_spFutRoots; //Asset
	VARIANT_BOOL m_bHaveFutures; //Asset

	EtsDivTypeEnum m_enDivType; //Asset
	IEtsDivCollPtr m_spCustomDivs; //Asset
	IEtsIndexDivAtomPtr m_spDividend; //Asset
	DOUBLE m_dYield; //Asset

	IIndexAtomPtr m_spBasketIndex ; //Asset

	IEtsFutAtomPtr m_spActiveFuture; //Asset

	DOUBLE m_dSOQ; //Custom Field CIBC 
	VARIANT_BOOL m_bIsAllowLending; //Asset

	IEtsRateCollPtr m_spHTBRates; //Asset

	VARIANT_BOOL m_bIsHedgeSymbol; //Asset
	VARIANT_BOOL m_fIsManualVol; //Asset

	IUndAtomPtr m_spHeadComponent; //Asset
	DOUBLE m_dCoeff; //Asset
	VARIANT_BOOL m_bPriceByHead; //Asset
	VARIANT_BOOL m_bIsHead; //Asset

	ForwardCurve* forward_curve;
	bool price_option_using_fwd_curve; //????

public:
	ISpotContractAtomPtr	m_spSpot; //Spot contract of this asset
	IContractCollPtr		m_spContracts; //List of all contract in this asset
	IEtsRateCollPtr			m_spBorrowingRatesCurve; //interest rate curve
	IAssetCollPtr			m_spAssets;

	//return Yield if DivType is the same type
	virtual	double			GetDividends(double dtDate, double dtExpiryOV, double dtTradingClose,
										 CSafeArrayWrapper<double>& saDates,
										 CSafeArrayWrapper<double>& saAmounts,
										 long&	lDividendsCount);

	//return interest rate curve
	double					GetBorrowingRate(double dtDate, double dtExpiryOV);

	void					Clear();

public:
	DWORD					dwVolaControlCooke;
	IVolaControlPtr			m_spVolatilitySource;
	IVolaControl*			GetVolatilitySource();
	bool					m_bVolatilityInitialized;
};

typedef boost::shared_ptr<CAsset>	CAssetPtr;
