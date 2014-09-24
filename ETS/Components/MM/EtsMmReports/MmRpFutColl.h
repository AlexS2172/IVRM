// MmRpFutColl.h : Declaration of the CMmRpFutColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmReports.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

_COM_SMARTPTR_TYPEDEF(IMmRpFutColl, IID_IMmRpFutColl);

// CMmRpFutColl
typedef IDispatchImpl<IMmRpFutColl, &IID_IMmRpFutColl, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>	IMmRpFutCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRpFutCollDispImpl, LONG, IMmRpFutAtom, LONG>						IMmRpFutCollImpl;

class ATL_NO_VTABLE CMmRpFutColl :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmRpFutColl, &CLSID_MmRpFutColl>,
	public ISupportErrorInfo,
	public IMmRpFutCollImpl
{
public:
	CMmRpFutColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPFUTCOLL)


BEGIN_COM_MAP(CMmRpFutColl)
	COM_INTERFACE_ENTRY(IMmRpFutColl)
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
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	STDMETHOD(Add)(LONG Key, IMmRpFutAtom* Value, IMmRpFutAtom** pRetVal);
	STDMETHOD(GetPriceProperty_)( BSTR Symbol , IMMRpPrice** PPrice) ;
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpFutColl), CMmRpFutColl)
