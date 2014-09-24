// SyntheticEquityAtom.h : Declaration of the CSyntheticEquityAtom

#pragma once
#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "SynthRootColl.h"
#include "EtsOptRootColl.h"
#include "EtsFutRootColl.h"
#include "EtsPriceProfileAtom.h"
#include "EtsDivAtom.h"
#include "EtsDivColl.h"
#include "EtsIndexDivAtom.h"
#include "EtsIndexDivColl.h"
#include "EtsRateColl.h"
#include "IndexAtom.h"
#include "ContractColl.h"

#include "SpotContractAtom.h"
#include "SyntheticEquity.h"

// CSyntheticEquityAtom
class ATL_NO_VTABLE CSyntheticEquityAtom :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSyntheticEquityAtom, &CLSID_SyntheticEquityAtom>,
	public ISupportErrorInfo,
	public IDispatchImpl<ISyntheticEquityAtom, &IID_ISyntheticEquityAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CSyntheticEquity
{
public:
	CSyntheticEquityAtom()
	{
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_SyntheticEquityAtom)

	BEGIN_COM_MAP(CSyntheticEquityAtom)
		COM_INTERFACE_ENTRY(IAsset)
		COM_INTERFACE_ENTRY(ISyntheticEquityAtom)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{

		try
		{
			CComObject<CEtsIndexDivAtom>* pEtsIndexDivAtom; 
			_CHK(CComObject<CEtsIndexDivAtom>::CreateInstance(&pEtsIndexDivAtom),  _T("Fail to create dividend object."));
			m_spDividend.Attach(pEtsIndexDivAtom, TRUE);

			CComObject<CIndexAtom>* pBasketIndex; 
			_CHK(CComObject<CIndexAtom>::CreateInstance(&pBasketIndex),  _T("Fail to create basket index object."));
			m_spBasketIndex.Attach(pBasketIndex, TRUE);

			CComObject<CSpotContractAtom>* pSpotContractAtom; 
			_CHK(CComObject<CSpotContractAtom>::CreateInstance(&pSpotContractAtom),  _T("Fail to create Spot Contract object."));
			m_spSpot.Attach(pSpotContractAtom, TRUE);

			CComObject<CContractColl>* pContractColl; 
			_CHK(CComObject<CContractColl>::CreateInstance(&pContractColl),  _T("Fail to create ContractColl object."));
			m_spContracts.Attach(pContractColl, TRUE);

			_CHK(m_spVolatilitySource.CreateInstance(CLSID_VolaControl), _T("Fail to create volatility control."));

			CComGITPtr<IVolaControl> pVolaControlGIT(m_spVolatilitySource.GetInterfacePtr());
			dwVolaControlCooke = pVolaControlGIT.Detach();

		}
		catch (_com_error& err)
		{
			TRACE_COM_ERROR(err);
		};

		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	//IUndAtom & IAsset
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsAmerican, m_bIsAmerican)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsHTB, m_bIsHTB)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Yield, m_dYield)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsTraderContract, m_bIsTraderContract)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, HaveSyntheticRoots, m_bHaveSyntheticRoots)	
	IMPLEMENT_OBJECT_PROPERTY(ISynthRootColl*, SyntheticRoots, m_spSyntheticRoots)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, HaveOptions, m_bHaveOptions)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsActive, m_bIsActive)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, UndPriceProfile, m_spUndPriceProfile)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, OptPriceProfile, m_spOptPriceProfile)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsBasketIndex, m_bIsBasketIndex)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Volume10Day, m_nVolume10Day)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Volume5Expiration, m_nVolume5Expiration)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IEtsOptRootColl*, Roots, m_spRoots)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IEtsFutRootColl*, FutRoots, m_spFutRoots)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, HaveFutures, m_bHaveFutures)
	IMPLEMENT_OBJECT_PROPERTY(IEtsIndexDivAtom*, Dividend, m_spDividend)
	IMPLEMENT_OBJECT_PROPERTY(IIndexAtom*, BasketIndex, m_spBasketIndex)
	IMPLEMENT_OBJECT_PROPERTY(IEtsFutAtom*, ActiveFuture, m_spActiveFuture)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, SOQ, m_dSOQ)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsAllowLending, m_bIsAllowLending)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsHedgeSymbol, m_bIsHedgeSymbol)
	IMPLEMENT_OBJECT_PROPERTY(IEtsRateColl*, HTBRates, m_spHTBRates)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsManualVol, m_fIsManualVol)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, PriceByHead,	m_bPriceByHead)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		Coeff,			m_dCoeff)
	IMPLEMENT_OBJECT_PROPERTY(IUndAtom*,	HeadComponent,	m_spHeadComponent)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsHead,			m_bIsHead)

	IMPLEMENT_OBJECT_PROPERTY(ISpotContract*,	Spot,		m_spSpot)
	IMPLEMENT_OBJECT_PROPERTY(IContractColl*,	Contracts,	m_spContracts)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IVolaControl*, VolatilitySource, m_spVolatilitySource)
	IMPLEMENT_OBJECT_PROPERTY(IAssetColl*,		Assets,		m_spAssets)

	//Synthetic Asset property
	IMPLEMENT_OBJECT_PROPERTY(IBasketAssetComponentsColl*,	Components,	m_spComponents)

	//property of spot contract
	STDMETHOD(get_TotalQtyInShares)(LONG* pRetVal);
	STDMETHOD(put_TotalQtyInShares)(LONG  pNewVal);
	STDMETHOD(get_bShouldMultOptDeltaEq)(VARIANT_BOOL* pRetVal);
	STDMETHOD(put_bShouldMultOptDeltaEq)(VARIANT_BOOL NewVal);
	STDMETHOD(get_dKOptDeltaEq)(DOUBLE* pRetVal);
	STDMETHOD(put_dKOptDeltaEq)(DOUBLE NewVal);
	STDMETHOD(get_PriceClose)(DOUBLE* pRetVal);
	STDMETHOD(put_PriceClose)(DOUBLE NewVal);
	STDMETHOD(get_PriceTheoClose)(DOUBLE* pRetVal);
	STDMETHOD(put_PriceTheoClose)(DOUBLE NewVal);
	STDMETHOD(get_LotSize)(LONG* pRetVal);
	STDMETHOD(put_LotSize)(LONG NewVal);
	STDMETHOD(get_Skew)(DOUBLE* pRetVal);
	STDMETHOD(put_Skew)(DOUBLE NewVal);
	STDMETHOD(get_Kurt)(DOUBLE* pRetVal);
	STDMETHOD(put_Kurt)(DOUBLE NewVal);
	STDMETHOD(get_PrimaryExchangeID)(LONG* pRetVal);
	STDMETHOD(put_PrimaryExchangeID)(LONG NewVal);
	STDMETHOD(get_ManualActivePrice)(DOUBLE* pRetVal);
	STDMETHOD(put_ManualActivePrice)(DOUBLE NewVal);
	STDMETHOD(get_PrimaryExchImportID)(BSTR* pRetVal);
	STDMETHOD(put_PrimaryExchImportID)(BSTR pNewVal);
	STDMETHOD(get_UndPosForRates)(DOUBLE* pRetVal);
	STDMETHOD(put_UndPosForRates)(DOUBLE NewVal);

	STDMETHOD(put_UndType)(EtsContractTypeEnum newVal);
	STDMETHOD(get_UndType)(EtsContractTypeEnum *pRetVal);
	STDMETHOD(put_Symbol)(BSTR newVal);
	STDMETHOD(get_Symbol)(BSTR *pRetVal);
	STDMETHOD(put_ContractName)(BSTR newVal);
	STDMETHOD(get_ContractName)(BSTR *pRetVal);
	STDMETHOD(put_ID)(LONG newVal);
	STDMETHOD(get_ID)(LONG *pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(SyntheticEquityAtom), CSyntheticEquityAtom)
