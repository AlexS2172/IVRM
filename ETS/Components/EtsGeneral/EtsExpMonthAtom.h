// EtsExpMonthAtom.h : Declaration of the CEtsExpMonthAtom
#ifndef __ETSEXPMONTHATOM_H__
#define __ETSEXPMONTHATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"

_COM_SMARTPTR_TYPEDEF(IEtsExpMonthAtom, IID_IEtsExpMonthAtom);

struct __EtsExpMonthAtom
{
	LONG				m_nID;
	DATE				m_dtExpiry;
	DOUBLE				m_dVegaWeight;
	VARIANT_BOOL		m_bIsDirty;

	__EtsExpMonthAtom()
		: m_nID(0L), m_dtExpiry(0.),
		m_dVegaWeight(0.), m_bIsDirty(VARIANT_FALSE)
	{
	}
};

// CEtsExpMonthAtom

class ATL_NO_VTABLE CEtsExpMonthAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsExpMonthAtom, &CLSID_EtsExpMonthAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsExpMonthAtom>,
	public IDispatchImpl<IEtsExpMonthAtom, &IID_IEtsExpMonthAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __EtsExpMonthAtom
{
public:
	CEtsExpMonthAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSEXPMONTHATOM)


BEGIN_COM_MAP(CEtsExpMonthAtom)
	COM_INTERFACE_ENTRY(IEtsExpMonthAtom)
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
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, Expiry, m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaWeight, m_dVegaWeight)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsDirty, m_bIsDirty)
};

OBJECT_ENTRY_AUTO(__uuidof(EtsExpMonthAtom), CEtsExpMonthAtom)

#endif //__ETSEXPMONTHATOM_H__
