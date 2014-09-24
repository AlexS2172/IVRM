// IndexColl.h : Declaration of the CIndexColl
#ifndef __INDEXCOLL_H__
#define __INDEXCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "IndexAtom.h"

typedef IDispatchImpl<IIndexColl, &IID_IIndexColl, &LIBID_EtsGeneralLib>													IIndexCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IIndexCollDispImpl, IIndexAtom, LONG, LONG, BSTR, _bstr_t  >	IIndexCollImpl;
_COM_SMARTPTR_TYPEDEF(IIndexColl, IID_IIndexColl);

// CIndexColl
_COM_SMARTPTR_TYPEDEF(IIndexColl, IID_IIndexColl);

class ATL_NO_VTABLE CIndexColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CIndexColl, &CLSID_IndexColl>,
	public ISupportErrorInfoImpl<&IID_IIndexColl>,
	public IIndexCollImpl
{
public:
	CIndexColl()
	{
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_INDEXCOLL)


	BEGIN_COM_MAP(CIndexColl)
		COM_INTERFACE_ENTRY(IIndexColl)
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
		IIndexCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	IIndexAtomPtr GetIndex(LONG lID)
	{
		IIndexAtomPtr spIndex;
		CollType::iterator itrIndex = m_collRef.find(lID);
		if(itrIndex != m_collRef.end())
			spIndex = itrIndex->second->second;

		return spIndex;
	}


	STDMETHOD(Add)(LONG Key, BSTR SortKey, IIndexAtom* Value, IIndexAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(IndexColl), CIndexColl)

#endif //__INDEXCOLL_H__
