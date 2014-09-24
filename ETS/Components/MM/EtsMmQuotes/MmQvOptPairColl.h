// MmQvOptPairColl.h : Declaration of the CMmQvOptPairColl
#ifndef __MMQVOPTPAIRCOLL_H__
#define __MMQVOPTPAIRCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"
#include "MmQvOptPairAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmQvOptPairColl, IID_IMmQvOptPairColl);

typedef IDispatchImpl<IMmQvOptPairColl, &IID_IMmQvOptPairColl, &LIBID_EtsMmQuotesLib>													IMmQvOptPairCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmQvOptPairCollDispImpl, IMmQvOptPairAtom, LONG, LONG, BSTR, _bstr_t  >	IMmQvOptPairCollImpl;

// CMmQvOptPairColl
class ATL_NO_VTABLE CMmQvOptPairColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvOptPairColl, &CLSID_MmQvOptPairColl>,
	public ISupportErrorInfoImpl<&IID_IMmQvOptPairColl>,
	public IMmQvOptPairCollImpl
{
public:
	CMmQvOptPairColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVOPTPAIRCOLL)


BEGIN_COM_MAP(CMmQvOptPairColl)
	COM_INTERFACE_ENTRY(IMmQvOptPairColl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(	GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		IMmQvOptPairCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}
private:
	CComPtr<IUnknown> m_pUnkMarshaler;


public:
	IMmQvOptPairAtomPtr GetPair(long lRootID)
	{
		IMmQvOptPairAtomPtr spPair;
		CollType::iterator itrPair = m_collRef.find(lRootID);
		if(itrPair!=m_collRef.end())
			spPair = itrPair->second->second;
		return spPair;
	}
	IMmQvOptPairAtomPtr AddNew(long lRootID, _bstr_t bsRootName, CComObject<CMmQvOptPairAtom>** ppPairAtom = NULL);
public:

	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmQvOptPairAtom* Value, IMmQvOptPairAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvOptPairColl), CMmQvOptPairColl)

#endif //__MMQVOPTPAIRCOLL_H__