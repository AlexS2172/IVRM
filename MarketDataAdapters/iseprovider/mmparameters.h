// MMParameters.h : Declaration of the CMMParameters

#ifndef __MMPARAMETERS_H_
#define __MMPARAMETERS_H_

#include "resource.h"       // main symbols
#include "ObjectID.h"

struct MMParams : public set_pmm_parameters_ui102_item_t
{
	_bstr_t		m_bsUnd;
};

/////////////////////////////////////////////////////////////////////////////
// CMMParameters
class ATL_NO_VTABLE CMMParameters : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMMParameters, &CLSID_MMParameters>,
	public ISupportErrorInfoImpl<&IID_IMMParameters>,
	public IDispatchImpl<IMMParameters, &IID_IMMParameters, &LIBID_ISEPROVIDERLib>,
	public MMParams
{
public:
	CMMParameters()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMPARAMETERS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMMParameters)
	COM_INTERFACE_ENTRY(IMMParameters)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	//STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IMMParameters
public:
	STDMETHOD(get_MatchAwayMarketMaxSize)(long Tick, /*[out, retval]*/ long *pVal);
	STDMETHOD(put_MatchAwayMarketMaxSize)(long Tick, /*[in]*/ long newVal);
	STDMETHOD(get_DerivedOrderMaxSize)(long Tick, /*[out, retval]*/ long *pVal);
	STDMETHOD(put_DerivedOrderMaxSize)(long Tick, /*[in]*/ long newVal);
	STDMETHOD(get_FarMMTradeLimitAbsolute)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_FarMMTradeLimitAbsolute)(/*[in]*/ long newVal);
	STDMETHOD(get_FirmTradeLimitFraction)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_FirmTradeLimitFraction)(/*[in]*/ long newVal);
	STDMETHOD(get_IseMmTradeLimitFraction)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_IseMmTradeLimitFraction)(/*[in]*/ long newVal);
	STDMETHOD(get_FarMmTradeLimitFraction)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_FarMmTradeLimitFraction)(/*[in]*/ long newVal);
	STDMETHOD(get_FirmTradeLimitAbsolute)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_FirmTradeLimitAbsolute)(/*[in]*/ long newVal);
	STDMETHOD(get_IseMMTradeLimitAbsolute)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_IseMMTradeLimitAbsolute)(/*[in]*/ long newVal);
	STDMETHOD(get_StepUpAfterRegenBuffer)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_StepUpAfterRegenBuffer)(/*[in]*/ long newVal);
	STDMETHOD(get_TickWorseVolume)(long Index, /*[out, retval]*/ long *pVal);
	STDMETHOD(put_TickWorseVolume)(long Index, /*[in]*/ long newVal);
	STDMETHOD(get_StrikePriceGroup)(/*[out, retval]*/ StrikePriceGroupEnum *pVal);
	STDMETHOD(put_StrikePriceGroup)(/*[in]*/ StrikePriceGroupEnum newVal);
	STDMETHOD(get_ExpirationGroup)(/*[out, retval]*/ ExpirationGroupEnum *pVal);
	STDMETHOD(put_ExpirationGroup)(/*[in]*/ ExpirationGroupEnum newVal);
	STDMETHOD(get_InstrumentGroup)(/*[out, retval]*/ InstrumentGroupEnum *pVal);
	STDMETHOD(put_InstrumentGroup)(/*[in]*/ InstrumentGroupEnum newVal);
	STDMETHOD(get_UnderlyingSymbol)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_UnderlyingSymbol)(/*[in]*/ BSTR newVal);
};

#endif //__MMPARAMETERS_H_
