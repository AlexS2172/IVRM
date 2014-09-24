// RvMmQuoteAtom.h : Declaration of the CRvMmQuoteAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmRisks.h"
#include "MmRvPrice.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

_COM_SMARTPTR_TYPEDEF(IRvMmQuoteAtom, IID_IRvMmQuoteAtom);


/////////////////////////////////////////////////////////////////////////////
//
struct __MmRvQuoteAtom
{
	// price info
	IMmRvPricePtr				m_spPrice;
	CComObject<CMmRvPrice>*     m_pPrice;

	IMmRvPricePtr				m_spSuPrice;
	CComObject<CMmRvPrice>*     m_pSuPrice;

	// theo info
	DOUBLE						m_dPriceTheo;
	DOUBLE						m_dPriceTheoBid;
	DOUBLE						m_dPriceTheoAsk;

	DOUBLE						m_dIntrinsicValue;

	DOUBLE						m_dContractPrice;
	DOUBLE						m_dContractPriceBid;
	DOUBLE						m_dContractPriceAsk;

	DOUBLE						m_dBaseContractPrice;
	DOUBLE						m_dBaseContractPriceBid;
	DOUBLE						m_dBaseContractPriceAsk;

	DOUBLE						m_dDelta;
	DOUBLE						m_dGamma;
	DOUBLE						m_dVega;
	DOUBLE						m_dTheta;
	DOUBLE						m_dRho;
	DOUBLE						m_dThetaDelta;
	DOUBLE						m_dThetaGamma;
	DOUBLE						m_dVegaDelta;
	DOUBLE						m_dVegaGamma;


	DOUBLE						m_dVola;
	DOUBLE						m_dImpliedVola ;
	DOUBLE						m_dMIV;
	DOUBLE						m_dVolaCalcPrice ;
	VARIANT_BOOL				m_bIsVolaCalculatedWell ;


	// common position	info
	LONG						m_nLotSize;
	DOUBLE						m_dNetChange;
	EtsReplacePriceStatusEnum	m_enReplacePriceStatus;
	VARIANT_BOOL				m_bVolatilityEnginrCalculation ;
	_bstr_t						m_bsVolaCalculatedParametrs ;
	VARIANT_BOOL				m_bDirty;


	__MmRvQuoteAtom() :
		m_bIsVolaCalculatedWell(VARIANT_TRUE),
		m_nLotSize(0L),
		m_pSuPrice(NULL),
		m_pPrice(NULL),
		m_dVola(0.),
		m_dImpliedVola(0.),
		m_dMIV(0.),
		m_dVolaCalcPrice(0.),
		m_bDirty(VARIANT_FALSE),
		m_dNetChange(BAD_DOUBLE_VALUE),
		m_enReplacePriceStatus(enRpsNone)
	{
		ClearValues();
	}

	void ClearValues()
	{
		m_dPriceTheo = BAD_DOUBLE_VALUE;
		m_dDelta = BAD_DOUBLE_VALUE;
		m_dGamma = BAD_DOUBLE_VALUE;
		m_dVega = BAD_DOUBLE_VALUE;
		m_dTheta = BAD_DOUBLE_VALUE;
		m_dRho = BAD_DOUBLE_VALUE;
		m_dThetaDelta = BAD_DOUBLE_VALUE;
		m_dThetaGamma = BAD_DOUBLE_VALUE;
		m_dVegaDelta = BAD_DOUBLE_VALUE;
		m_dVegaGamma = BAD_DOUBLE_VALUE;

		m_dPriceTheoBid = BAD_DOUBLE_VALUE;
		m_dPriceTheoAsk = BAD_DOUBLE_VALUE;
		m_dContractPrice = BAD_DOUBLE_VALUE;
		m_dContractPriceBid = BAD_DOUBLE_VALUE;
		m_dContractPriceAsk = BAD_DOUBLE_VALUE;
		m_dBaseContractPrice = BAD_DOUBLE_VALUE;
		m_dBaseContractPriceBid = BAD_DOUBLE_VALUE;
		m_dBaseContractPriceAsk = BAD_DOUBLE_VALUE;
		m_dIntrinsicValue = BAD_DOUBLE_VALUE;

		m_bDirty = VARIANT_FALSE;

	}

};

// CRvMmQuoteAtom
class ATL_NO_VTABLE CRvMmQuoteAtom :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CRvMmQuoteAtom, &CLSID_RvMmQuoteAtom>,
	public ISupportErrorInfoImpl<&IID_IRvMmQuoteAtom>,
	public IDispatchImpl<IRvMmQuoteAtom, &IID_IRvMmQuoteAtom, &LIBID_EtsMmRisksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRvQuoteAtom
{
public:
	CRvMmQuoteAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_RVMMQUOTEATOM)


BEGIN_COM_MAP(CRvMmQuoteAtom)
	COM_INTERFACE_ENTRY(IRvMmQuoteAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		HRESULT hr = CComObject<CMmRvPrice>::CreateInstance(&m_pPrice);
		if(FAILED(hr))
			return hr;

		m_spPrice.Attach(m_pPrice, TRUE);

		hr = CComObject<CMmRvPrice>::CreateInstance(&m_pSuPrice);
		if(FAILED(hr))
			return hr;

		m_spSuPrice.Attach(m_pSuPrice, TRUE);

		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

	void SetDirty(){put_IsDirty(VARIANT_TRUE);}
	void ClearDirty(){put_IsDirty(VARIANT_FALSE);}
	bool IsDirty()
	{
		bool bDirty = false;

		if(m_bDirty!=VARIANT_FALSE)
			bDirty = true;
		else
		{
			if(m_pPrice && m_pPrice->m_bDirty != VARIANT_FALSE)
				bDirty = true;
			else
				if(m_pSuPrice && m_pSuPrice->m_bDirty != VARIANT_FALSE)
					bDirty = true;
		}
		return bDirty;
	}

public:
	IMPLEMENT_SIMPLE_WITHCHECK_PROPERTY(DOUBLE, Vola,			m_dVola)
	IMPLEMENT_SIMPLE_WITHCHECK_PROPERTY(DOUBLE, ImpliedVola, m_dImpliedVola)
	IMPLEMENT_SIMPLE_WITHCHECK_PROPERTY(DOUBLE, MIV, m_dMIV)

	// price info
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvPrice*,	SuPrice, m_spSuPrice);
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvPrice*,	Price,   m_spPrice);

	// position	info
	IMPLEMENT_SIMPLE_PROPERTY(LONG, LotSize, m_nLotSize)

	// theo info
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceTheo, m_dPriceTheo)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Delta, m_dDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Gamma, m_dGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Vega, m_dVega)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Theta, m_dTheta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Rho, m_dRho)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ThetaDelta, m_dThetaDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ThetaGamma, m_dThetaGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaDelta, m_dVegaDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaGamma, m_dVegaGamma)
	IMPLEMENT_SIMPLE_PROPERTY(EtsReplacePriceStatusEnum, ReplacePriceStatus, m_enReplacePriceStatus)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL , IsVolaCalculatedWell,m_bIsVolaCalculatedWell)
	IMPLEMENT_BSTRT_PROPERTY(VolaCalculatedParametrs, m_bsVolaCalculatedParametrs)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, CalcPrice, m_dVolaCalcPrice)

	STDMETHOD(put_IsDirty)(VARIANT_BOOL bDirty);
	STDMETHOD(get_IsDirty)(VARIANT_BOOL* pDirty);

};

OBJECT_ENTRY_AUTO(__uuidof(RvMmQuoteAtom), CRvMmQuoteAtom)
