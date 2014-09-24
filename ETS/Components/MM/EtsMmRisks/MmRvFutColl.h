// MmRvFutColl.h : Declaration of the CMmRvFutColl
#ifndef __MMRVFUTCOLL_H__
#define __MMRVFUTCOLL_H__

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmRisks.h"
#include "MmRvFutAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmRvFutColl, IID_IMmRvFutColl);

typedef IDispatchImpl<IMmRvFutColl, &IID_IMmRvFutColl, &LIBID_EtsMmRisksLib>												IMmRvFutCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmRvFutCollDispImpl, IMmRvFutAtom, LONG, LONG, BSTR, _bstr_t  >	IMmRvFutCollImpl;

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmRvFutColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmRvFutColl, &CLSID_MmRvFutColl>,
	public ISupportErrorInfoImpl<&IID_IMmRvFutColl>,
	public IMmRvFutCollImpl
{
public:
	CMmRvFutColl()
	{
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MMRVFUTCOLL)


	BEGIN_COM_MAP(CMmRvFutColl)
		COM_INTERFACE_ENTRY(IMmRvFutColl)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease() 
	{
		IMmRvFutCollImpl::Clear();

		m_pUnkMarshaler.Release();
	}

private:

	CComPtr<IUnknown>	m_pUnkMarshaler;

public:
	IMmRvFutAtomPtr GetFutures(long lKey)
	{
		IMmRvFutAtomPtr spResult;
		IterType itrFind = m_collRef.find(lKey);
		if(itrFind != m_collRef.end())
			spResult = itrFind->second->second;
		return spResult;
	}
	IMmRvFutAtomPtr AddNew(long lFutId, _bstr_t bsFutSymbol, CComObject<CMmRvFutAtom>** ppObject = NULL);

public:
	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmRvFutAtom* Value, IMmRvFutAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvFutColl), CMmRvFutColl)

#endif //__MMRVFUTCOLL_H__
