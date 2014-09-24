// MmQvStrikeColl.h : Declaration of the CMmQvStrikeColl
#ifndef __MMQVSTRIKECOLL_H__
#define __MMQVSTRIKECOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"
#include "MmQvStrikeAtom.h"
#include "MmQvOptRootColl.h"

_COM_SMARTPTR_TYPEDEF(IMmQvUndAtom, IID_IMmQvUndAtom);
_COM_SMARTPTR_TYPEDEF(IMmQvStrikeColl, IID_IMmQvStrikeColl);

typedef IDispatchImpl<IMmQvStrikeColl, &IID_IMmQvStrikeColl, &LIBID_EtsMmQuotesLib>							IMmQvStrikeCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmQvStrikeCollDispImpl, DOUBLE, IMmQvStrikeAtom, DOUBLE >	IMmQvStrikeCollImpl;

// CMmQvStrikeColl

class ATL_NO_VTABLE CMmQvStrikeColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvStrikeColl, &CLSID_MmQvStrikeColl>,
	public ISupportErrorInfoImpl<&IID_IMmQvStrikeColl>,
	public IMmQvStrikeCollImpl
{
public:
	CMmQvStrikeColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVSTRIKECOLL)


BEGIN_COM_MAP(CMmQvStrikeColl)
	COM_INTERFACE_ENTRY(IMmQvStrikeColl)
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
		IMmQvStrikeCollImpl::Clear();
		m_pUnkMarshaler.Release();

	}

private:
	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	IMmQvStrikeAtomPtr FindATMStrike(double dSpotPrice);
	IMmQvStrikeAtomPtr GetStrike(DOUBLE dStrike);

	IMmQvStrikeAtomPtr AddNew(DOUBLE dStrike, CComObject<CMmQvStrikeAtom>** ppStrike = NULL);


public:

	STDMETHOD(Add)(DOUBLE Key, IMmQvStrikeAtom* Value, IMmQvStrikeAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvStrikeColl), CMmQvStrikeColl)

#endif //__MMQVSTRIKECOLL_H__