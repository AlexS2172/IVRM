// EtsPriceProfileColl.h : Declaration of the CEtsPriceProfileColl
#ifndef __ETSPRICEPROFILECOLL_H__
#define __ETSPRICEPROFILECOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "EtsPriceProfileAtom.h"
#include "CommonSPtr.h"

typedef IDispatchImpl<IEtsPriceProfileColl, &IID_IEtsPriceProfileColl, &LIBID_EtsGeneralLib>													IEtsPriceProfileCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IEtsPriceProfileCollDispImpl, IEtsPriceProfileAtom, LONG, LONG, BSTR, _bstr_t  >	IEtsPriceProfileCollImpl;

// CEtsPriceProfileColl


class ATL_NO_VTABLE CEtsPriceProfileColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsPriceProfileColl, &CLSID_EtsPriceProfileColl>,
	public ISupportErrorInfoImpl<&IID_IEtsPriceProfileColl>,
	public IEtsPriceProfileCollImpl
{
public:
	CEtsPriceProfileColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSPRICEPROFILECOLL)


BEGIN_COM_MAP(CEtsPriceProfileColl)
	COM_INTERFACE_ENTRY(IEtsPriceProfileColl)
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
		IEtsPriceProfileCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	IEtsPriceProfileAtomPtr AddNew(long lID, _bstr_t bsSymbol, CComObject<CEtsPriceProfileAtom>** pAtom = NULL);
	STDMETHOD(Add)(LONG Key, BSTR SortKey, IEtsPriceProfileAtom* Value, IEtsPriceProfileAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsPriceProfileColl), CEtsPriceProfileColl)

#endif //__ETSPRICEPROFILECOLL_H__
