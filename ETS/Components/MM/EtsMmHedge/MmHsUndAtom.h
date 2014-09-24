// MmHsUndAtom.h : Declaration of the CMmHsUndAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmHedge.h"
#include "MmHsOptColl.h"


_COM_SMARTPTR_TYPEDEF(IMmHsUndAtom, IID_IMmHsUndAtom);

struct __MmHsUndAtom
{
	DOUBLE						m_dDelta;
	//DOUBLE						m_dDivAmt;
	//DATE						m_dtDivDate;
	//LONG						m_nDivFreq;
	VARIANT_BOOL				m_vbEnabled;
	DOUBLE						m_dHV;
	LONG						m_nID;
	VARIANT_BOOL				m_vbIsAmerican;
	DOUBLE						m_dIV;
	LONG						m_nLotSize;
	IMmHsOptCollPtr				m_spOpt;
	DOUBLE						m_dPos;
	DOUBLE						m_dPriceAsk;
	DOUBLE						m_dPriceBid;
	DOUBLE						m_dPriceLast;
	LONG						m_nQty;
	CComBSTR					m_bstrSymbol;
	EtsContractTypeEnum			m_enUndType;
	DOUBLE						m_dVega;
	IVolaControlPtr				m_spVolaSrv;
	DOUBLE						m_dVolOfVol;
	DOUBLE						m_dYield;
	VARIANT_BOOL				m_vbUseMidRates;
	IEtsPriceProfileAtomPtr		m_spUndPriceProfile;
	IEtsPriceProfileAtomPtr		m_spOptPriceProfile;
	IEtsIndexDivAtomPtr			m_spDividend;


	void ClearValues()
	{
        m_dDelta = BAD_DOUBLE_VALUE;
		//m_dDivAmt = 0.;
		//m_dtDivDate = 0.;
		//m_nDivFreq = 0L;
		m_vbEnabled = VARIANT_TRUE;		
		m_dHV = BAD_DOUBLE_VALUE;
		m_nID = 0L;
		m_vbIsAmerican = VARIANT_TRUE;
		m_dIV = BAD_DOUBLE_VALUE;
		m_nLotSize = BAD_LONG_VALUE;
		m_dPos = BAD_DOUBLE_VALUE;
		m_dPriceAsk = BAD_DOUBLE_VALUE;
		m_dPriceBid = BAD_DOUBLE_VALUE;
		m_dPriceLast = BAD_DOUBLE_VALUE;
		m_nQty = BAD_LONG_VALUE;
		m_enUndType = enCtNone;
		m_dVega = BAD_DOUBLE_VALUE;
		m_dVolOfVol = BAD_DOUBLE_VALUE;
		m_dYield = 0.;
		m_vbUseMidRates = VARIANT_TRUE;
	}

	__MmHsUndAtom()
	{
		ClearValues();
	}
};

// CMmHsUndAtom

class ATL_NO_VTABLE CMmHsUndAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmHsUndAtom, &CLSID_MmHsUndAtom>,
	public ISupportErrorInfoImpl<&IID_IMmHsUndAtom>,
	public IDispatchImpl<IMmHsUndAtom, &IID_IMmHsUndAtom, &LIBID_EtsMmHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmHsUndAtom
{
public:
	CMmHsUndAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMHSUNDATOM)


BEGIN_COM_MAP(CMmHsUndAtom)
	COM_INTERFACE_ENTRY(IMmHsUndAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			__CHECK_HRESULT(m_spOpt.CreateInstance(CLSID_MmHsOptColl), _T("Fail to create collection object"));
			__CHECK_HRESULT(m_spVolaSrv.CreateInstance(CLSID_VolaControl), _T("Fail to create VolaControl object"));
			__CHECK_HRESULT(m_spDividend.CreateInstance(CLSID_EtsIndexDivAtom), _T("Fail to create div object."));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmHsUndAtom, e.Error());
		}
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spOpt = NULL;
		m_spVolaSrv = NULL;
		m_spUndPriceProfile = NULL;
		m_spOptPriceProfile = NULL;
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Delta,			m_dDelta)
	//IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				DivAmt,			m_dDivAmt)
	//IMPLEMENT_SIMPLE_PROPERTY(DATE,				DivDate,		m_dtDivDate)
	//IMPLEMENT_SIMPLE_PROPERTY(LONG,				DivFreq,		m_nDivFreq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL,			Enabled,		m_vbEnabled)	
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				HV,				m_dHV)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					ID,				m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL,			IsAmerican,		m_vbIsAmerican)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				IV,				m_dIV)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					LotSize,		m_nLotSize)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmHsOptColl*,	Opt,	m_spOpt)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Pos,			m_dPos)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				PriceAsk,		m_dPriceAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				PriceBid,		m_dPriceBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				PriceLast,		m_dPriceLast)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					Qty,			m_nQty)
	IMPLEMENT_BSTR_PROPERTY(Symbol,		m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum,	UndType,		m_enUndType)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Vega,			m_dVega)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IVolaControl*,	VolaSrv,	m_spVolaSrv)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				VolOfVol,		m_dVolOfVol)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Yield,			m_dYield)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL,			UseMidRates,	m_vbUseMidRates)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*,	UndPriceProfile,	m_spUndPriceProfile)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*,	OptPriceProfile,	m_spOptPriceProfile)
	IMPLEMENT_OBJECT_PROPERTY(	IEtsIndexDivAtom*, Dividend, m_spDividend);

	STDMETHOD(ClearValues)();
	STDMETHOD(CalcGreeks)( LONG nGreekMask, EtsCalcModelTypeEnum enCalcModel, 
						VARIANT_BOOL vbVegaWeighted, VARIANT_BOOL bUseTheoVolatility, 
						VARIANT_BOOL bUseTheoVolaNoBid, VARIANT_BOOL bUseTheoVolaBadMarket,
						DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule,
						ICalculationParametrs* pParams);
	
	private:

};

OBJECT_ENTRY_AUTO(__uuidof(MmHsUndAtom), CMmHsUndAtom)
