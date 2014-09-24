// EtsMmFilterAtom.h : Declaration of the CEtsMmFilterAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"


_COM_SMARTPTR_TYPEDEF(IEtsMmFilterAtom, IID_IEtsMmFilterAtom);

// CEtsMmFilterAtom
class _CEtsMmFilterAtom
{
public:
	long m_lID;
	_CEtsMmFilterAtom()
	:m_lID(0L)
	{}
};

class ATL_NO_VTABLE CEtsMmFilterAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsMmFilterAtom, &CLSID_EtsMmFilterAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsMmFilterAtom>,
	public IDispatchImpl<IEtsMmFilterAtom, &IID_IEtsMmFilterAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public _CEtsMmFilterAtom
{
public:
	CEtsMmFilterAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSMMFILTERATOM)


BEGIN_COM_MAP(CEtsMmFilterAtom)
	COM_INTERFACE_ENTRY(IEtsMmFilterAtom)
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

	_bstr_t     m_bsName;
public:
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_lID);
	IMPLEMENT_BSTRT_PROPERTY(Name, m_bsName);

};

OBJECT_ENTRY_AUTO(__uuidof(EtsMmFilterAtom), CEtsMmFilterAtom)
