// ContractRootColl.h : Declaration of the CContractRootColl

#pragma once
#include "resource.h"       // main symbols


#include "CommonSPtr.h"

// CContractRootColl

typedef IDispatchImpl<IContractRootColl, &IID_IContractRootColl, &LIBID_EtsGeneralLib>	IContractRootCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IContractRootCollDispImpl, IContractRoot, LONG, LONG, BSTR, _bstr_t  >	IContractRootCollImpl;


class ATL_NO_VTABLE CContractRootColl :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CContractRootColl, &CLSID_ContractRootColl>,
	public IContractRootCollImpl
{
public:
	CContractRootColl()
	{
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_CONTRACTROOTCOLL)


	BEGIN_COM_MAP(CContractRootColl)
		COM_INTERFACE_ENTRY(IContractRootColl)
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

private:
	CComPtr<IUnknown> m_pUnkMarshaler;

public:
   STDMETHOD(Add)(LONG Key, BSTR SortKey, IContractRoot* Value, IContractRoot** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(ContractRootColl), CContractRootColl)
