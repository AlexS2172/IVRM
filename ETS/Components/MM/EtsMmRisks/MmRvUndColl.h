// MmRvUndColl.h : Declaration of the CMmRvUndColl
#ifndef __MMRVUNDCOLL_H__
#define __MMRVUNDCOLL_H__

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmRisks.h"
#include "MmRvUndAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmRvUndColl, IID_IMmRvUndColl);

typedef IDispatchImpl<IMmRvUndColl, &IID_IMmRvUndColl, &LIBID_EtsMmRisksLib>												IMmRvUndCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmRvUndCollDispImpl, IMmRvUndAtom, LONG, LONG, BSTR, _bstr_t  >	IMmRvUndCollImpl;

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmRvUndColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmRvUndColl, &CLSID_MmRvUndColl>,
	public ISupportErrorInfoImpl<&IID_IMmRvUndColl>,
	public IMmRvUndCollImpl
{
public:
	CMmRvUndColl()
		:m_dNetExposureAUM(BAD_DOUBLE_VALUE)
	{
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MMRVUNDCOLL)


	BEGIN_COM_MAP(CMmRvUndColl)
		COM_INTERFACE_ENTRY(IMmRvUndColl)
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
		IMmRvUndCollImpl::Clear();

		m_pUnkMarshaler.Release();
	}

private:

	CComPtr<IUnknown>		m_pUnkMarshaler;
	DOUBLE                  m_dNetExposureAUM;

public:
	IMmRvUndAtomPtr GetUnderlying(long lUndID)
	{
		IMmRvUndAtomPtr spUnd;
		IterType itrFind = m_collRef.find(lUndID);
		if(itrFind != m_collRef.end())
			spUnd = itrFind->second->second;
		return spUnd;
	}

	IMmRvUndAtomPtr AddNew(long lUndId, _bstr_t bsSymbol, CComObject<CMmRvUndAtom>** pObject = NULL);

public:
	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmRvUndAtom* Value, IMmRvUndAtom** pRetVal);

	void    SetDirty();
	void    SetNetExposureAUM(DOUBLE dNewVal);
	double  GetNetExposureAUM() const {return m_dNetExposureAUM;}
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvUndColl), CMmRvUndColl)

#endif //__MMRVUNDCOLL_H__
