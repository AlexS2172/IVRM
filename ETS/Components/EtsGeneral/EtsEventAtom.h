// EtsEventAtom.h : Declaration of the CEtsEventAtom
#ifndef __ETSEVENTATOM_H__
#define __ETSEVENTATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"

struct __EtsEventAtom
{
	LONG				m_nEvtID;
	DATE				m_dtEvtTime;
	EtsEventTypeEnum	m_enEvtType;
	_bstr_t				m_bstrEvtMessage;

	__EtsEventAtom()
		: m_nEvtID(0L), m_dtEvtTime(0.), m_enEvtType(enEvtUnknown)
	{
	}
};

// CEtsEventAtom

class ATL_NO_VTABLE CEtsEventAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsEventAtom, &CLSID_EtsEventAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsEventAtom>,
	public IDispatchImpl<IEtsEventAtom, &IID_IEtsEventAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __EtsEventAtom
{
public:
	CEtsEventAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSEVENTATOM)


BEGIN_COM_MAP(CEtsEventAtom)
	COM_INTERFACE_ENTRY(IEtsEventAtom)
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
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, EvtID, m_nEvtID);
	IMPLEMENT_SIMPLE_PROPERTY(DATE, EvtTime, m_dtEvtTime);
	IMPLEMENT_SIMPLE_PROPERTY(EtsEventTypeEnum, EvtType, m_enEvtType);
	IMPLEMENT_BSTRT_PROPERTY(EvtMessage, m_bstrEvtMessage);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsEventAtom), CEtsEventAtom)

#endif //__ETSEVENTATOM_H__
