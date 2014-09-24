// CalculationSettings.h : Declaration of the CCalculationSettings

#pragma once
#include "resource.h"       // main symbols
#include "EtsGeneral.h"

// CCalculationSettings
_COM_SMARTPTR_TYPEDEF(ICalculationSettings, IID_ICalculationSettings);

class ATL_NO_VTABLE CCalculationSettingsAtom :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CCalculationSettingsAtom, &CLSID_CalculationSettings>,
	public IDispatchImpl<ICalculationSettings, &IID_ICalculationSettings, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CCalculationSettingsAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CALCULATIONSETTINGS)


BEGIN_COM_MAP(CCalculationSettingsAtom)
	COM_INTERFACE_ENTRY(ICalculationSettings)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	STDMETHOD(get_GreeksCalculationMode)(LONG* pRetVal);
	STDMETHOD(put_GreeksCalculationMode)(LONG  Val);
	STDMETHOD(get_GreeksCalculationModel)(LONG* pRetVal);
	STDMETHOD(put_GreeksCalculationModel)(LONG  Val);
	STDMETHOD(get_UseTimePrecision)(LONG* pRetVal);
	STDMETHOD(put_UseTimePrecision)(LONG  Val);

	STDMETHOD(get_UseTheoVolaIfNoBid)(LONG* pRetVal);
	STDMETHOD(put_UseTheoVolaIfNoBid)(LONG  Val);
	STDMETHOD(get_UseTheoVolaIfBadMarketVola)(LONG* pRetVal);
	STDMETHOD(put_UseTheoVolaIfBadMarketVola)(LONG  Val);
	STDMETHOD(get_PriceTolerance)(DOUBLE* pRetVal);
	STDMETHOD(put_PriceTolerance)(DOUBLE  Val);
	STDMETHOD(get_PriceRoundingRule)(LONG* pRetVal);
	STDMETHOD(put_PriceRoundingRule)(LONG  Val);

	STDMETHOD(put_Normal)(DOUBLE Value);
	STDMETHOD(get_Normal)(DOUBLE* RetVal);

	STDMETHOD(get_CurveInterpolationType)(LONG* pRetVal);
	STDMETHOD(put_CurveInterpolationType)(LONG  Val);

	STDMETHOD(get_CurveExtrapolationType)(LONG* pRetVal);
	STDMETHOD(put_CurveExtrapolationType)(LONG  Val);
};

OBJECT_ENTRY_AUTO(__uuidof(CalculationSettings), CCalculationSettingsAtom)
