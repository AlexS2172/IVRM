// MmHsCorrelationColl.h : Declaration of the CMmHsCorrelationColl

#pragma once
#include "resource.h"       // main symbols
#include "EtsMmHedge.h"


_COM_SMARTPTR_TYPEDEF(IMmHsCorrelationColl, IID_IMmHsCorrelationColl);

//////////////////////////////////////////////////////////////////////////////
class CMmHsKey
{
public:
	CMmHsKey() : m_nId1(0L), m_nId2(0L) {}
	CMmHsKey::CMmHsKey(LONG nId1, LONG nId2) : m_nId1(nId1), m_nId2(nId2) {}
	
	bool operator<(const CMmHsKey& Key) const
	{
		return memcmp(this,&Key,sizeof(CMmHsKey))<0 ? true : false;
	}

	CMmHsKey& operator=(const CMmHsKey& Key)
	{
		Set(Key.m_nId1, Key.m_nId2);
		return *this;
	}

	void Set(LONG nId1, LONG nId2)
	{
		m_nId1 = nId1;
		m_nId2 = nId2;
	}

public:

	LONG m_nId1;
	LONG m_nId2;
};

template <class T, typename KeyType, class KeyClass = KeyType >
class ICollectionOnSTLMapOfDoubleImpl : 
	public ICollectionOnSTLMapImpl<T, KeyType, DOUBLE, _Copy<DOUBLE>, 
		CComEnumVariantOnSTLMap<KeyClass, DOUBLE>, KeyClass >
{
};

typedef IDispatchImpl<IMmHsCorrelationColl, &IID_IMmHsCorrelationColl, &LIBID_EtsMmHedgeLib>	IMmHsCorrelationCollDispImpl;
typedef ICollectionOnSTLMapOfDoubleImpl<IMmHsCorrelationCollDispImpl, CMmHsKey>					IMmHsCorrelationCollImpl;

// CMmHsCorrelationColl

class ATL_NO_VTABLE CMmHsCorrelationColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmHsCorrelationColl, &CLSID_MmHsCorrelationColl>,
	public ISupportErrorInfoImpl<&IID_IMmHsCorrelationColl>,
	public IMmHsCorrelationCollImpl
{
public:
	CMmHsCorrelationColl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMHSCORRELATIONCOLL)


BEGIN_COM_MAP(CMmHsCorrelationColl)
	COM_INTERFACE_ENTRY(IMmHsCorrelationColl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		IMmHsCorrelationCollImpl::Clear();
	}

public:

    STDMETHOD(Add)(LONG Id1, LONG Id2, DOUBLE Value );
	STDMETHOD(Remove)(LONG Id1, LONG Id2);
	STDMETHOD(get_Item)(LONG Id1, LONG Id2, DOUBLE* pVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmHsCorrelationColl), CMmHsCorrelationColl)
