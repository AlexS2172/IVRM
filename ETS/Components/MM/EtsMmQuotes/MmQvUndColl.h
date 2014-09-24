// MmQvUndColl.h : Declaration of the CMmQvUndColl
#ifndef  __MMQVUNDCOLL_H__
#define  __MMQVUNDCOLL_H__
#pragma once

#include "resource.h"       // main symbols
//#include "EtsMmQuotes.h"
#include "MmQvUndAtom.h"
class CMmQvUndAtom;

_COM_SMARTPTR_TYPEDEF(IMmQvUndColl, IID_IMmQvUndColl);
_COM_SMARTPTR_TYPEDEF(IMmQvUndAtom, IID_IMmQvUndAtom);

typedef IDispatchImpl<IMmQvUndColl, &IID_IMmQvUndColl, &LIBID_EtsMmQuotesLib>													IMmQvUndCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmQvUndCollDispImpl, IMmQvUndAtom, LONG, LONG, BSTR, _bstr_t  >	IMmQvUndCollImpl;

// CMmQvUndColl

class ATL_NO_VTABLE CMmQvUndColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvUndColl, &CLSID_MmQvUndColl>,
	public ISupportErrorInfoImpl<&IID_IMmQvUndColl>,
	public IMmQvUndCollImpl
{
public:
	CMmQvUndColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVUNDCOLL)


BEGIN_COM_MAP(CMmQvUndColl)
	COM_INTERFACE_ENTRY(IMmQvUndColl)
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
		IMmQvUndCollImpl::Clear();
		m_pUnkMarshaler.Release();

	}
private:
	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	IMmQvUndAtomPtr GetUnd(long lKey)
	{
		IMmQvUndAtomPtr spReturn;
		CollType::iterator itr = m_collRef.find(lKey);
		if(itr != m_collRef.end())
			spReturn = itr->second->second;
		return spReturn;
	}

	IMmQvUndAtomPtr AddNew(long lUndID, _bstr_t bsUnd, CComObject<CMmQvUndAtom>** ppUndAtom = NULL);


public:
	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmQvUndAtom* Value, IMmQvUndAtom** pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmQvUndColl), CMmQvUndColl)

#endif // __MMQVUNDCOLL_H__
