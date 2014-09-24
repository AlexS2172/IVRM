// ContractColl.h : Declaration of the CContractColl

#pragma once
#include "resource.h"       // main symbols

//#include "EtsGeneral.h"
#include "CommonSPtr.h"

// CContractColl

typedef IDispatchImpl<IContractColl, &IID_IContractColl, &LIBID_EtsGeneralLib>	IContractCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IContractCollDispImpl, IContract, LONG, LONG, BSTR, _bstr_t  >	IContractCollImpl;

class ATL_NO_VTABLE CContractColl :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CContractColl, &CLSID_ContractColl>,
	public ISupportErrorInfo,
	public IContractCollImpl
{
public:
	CContractColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CONTRACTCOLL)


BEGIN_COM_MAP(CContractColl)
	COM_INTERFACE_ENTRY(IContractColl)
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
		IContractCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IContract* Value, IContract** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(ContractColl), CContractColl)
