// VolaControl.h : Declaration of the CVolaControl
#ifndef __VOLACONTROL_H__
#define __VOLACONTROL_H__

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"

// CVolaControl

class ATL_NO_VTABLE CVolaControl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVolaControl, &CLSID_VolaControl>,
	public ISupportErrorInfoImpl<&IID_IVolaControl>,
	public IDispatchImpl<IVolaControl, &IID_IVolaControl, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CVolaControl()
		: m_dUnderlyingPrice(0.),
		  m_bIsVolSimulated(VARIANT_FALSE),
		  m_ContractType(enCtNone)
	{
		m_sbsSymbol = L"--";
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VOLACONTROL)

BEGIN_COM_MAP(CVolaControl)
	COM_INTERFACE_ENTRY(IVolaControl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
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
		CleanUp();
		m_pUnkMarshaler.Release();
	}

public:
	CComPtr<IUnknown>				m_pUnkMarshaler;

	IVSSymbolVolatilityPtr			m_spSymbolVola;
	DOUBLE							m_dUnderlyingPrice;

	VARIANT_BOOL					m_bIsVolSimulated;
	CComBSTR						m_sbsSymbol;
	EtsContractTypeEnum				m_ContractType;

public:
	STDMETHOD(Init)(BSTR UnderlyingSymbol, EtsContractTypeEnum ContractType, IVolatilitySource* pVolaSrc, VARIANT_BOOL IsSimulated = VARIANT_FALSE);
	STDMETHOD(get_OptionVola)(DATE Expiry, DOUBLE Strike, LONG SurfaceID, DOUBLE* pVal);
	STDMETHOD(put_OptionVola)(DATE Expiry, DOUBLE Strike, LONG SurfaceID, DOUBLE newVal);
	STDMETHOD(get_OptionTargetVola)(DATE Expiry, DOUBLE Strike, DOUBLE* pVal);
	STDMETHOD(ShiftExpiryVola)(DATE Expiry, DOUBLE ShiftValue, LONG SurfaceID);
	STDMETHOD(SetExpiryVola)(DATE Expiry, DOUBLE Value, LONG SurfaceID);
	STDMETHOD(SetExpiryVolaByExpiry)(LPSAFEARRAY* saData, LONG SurfaceID);
	STDMETHOD(SetExpiryVolaAll)(DOUBLE Value, LONG SurfaceID);
	STDMETHOD(CleanUp)(void);
	STDMETHOD(SetExpiryAndStrikeVolaAll)(LPSAFEARRAY* saData, LONG SurfaceID);
	STDMETHOD(GetSurfaceByRoot)(/*[in]*/ LONG RootID, /*[out, retval]*/ LONG* pVal);

	STDMETHOD(SaveSimulatedVol)(void);

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, UnderlyingPrice, m_dUnderlyingPrice);

	IMPLEMENT_SIMPLEREADONLY_PROPERTY(VARIANT_BOOL,	IsInitialized, (m_spSymbolVola != NULL ? VARIANT_TRUE : VARIANT_FALSE))

	IMPLEMENT_SIMPLEREADONLY_PROPERTY(VARIANT_BOOL,	SimulatedVol, m_bIsVolSimulated)
};

OBJECT_ENTRY_AUTO(__uuidof(VolaControl), CVolaControl)

#endif //__VOLACONTROL_H__
