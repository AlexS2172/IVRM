// EtsExpCalColl.h : Declaration of the CEtsExpCalColl
#ifndef __ETSEXPCALCOLL_H__
#define __ETSEXPCALCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include <OptionCalc\OptionCalc.h>

typedef IDispatchImpl<IEtsExpCalColl, &IID_IEtsExpCalColl, &LIBID_EtsGeneralLib>						IEtsExpCalCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IEtsExpCalCollDispImpl, LONG, IEtsExpMonthAtom, LONG >	IEtsExpCalCollImpl;

struct __EtsExpCalColl
{
	LONG	m_nMaturity;
	LONG  m_nHiddenCount;
	__EtsExpCalColl()
		: m_nMaturity(0L), m_nHiddenCount(0)
	{
	}
};
_COM_SMARTPTR_TYPEDEF(IEtsExpCalColl, IID_IEtsExpCalColl);

// CEtsExpCalColl
class ATL_NO_VTABLE CEtsExpCalColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsExpCalColl, &CLSID_EtsExpCalColl>,
	public ISupportErrorInfoImpl<&IID_IEtsExpCalColl>,
	public IEtsExpCalCollImpl,
	public __EtsExpCalColl
{
public:
	CEtsExpCalColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSEXPCALCOLL)


BEGIN_COM_MAP(CEtsExpCalColl)
	COM_INTERFACE_ENTRY(IEtsExpCalColl)
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
		IEtsExpCalCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(Add)(LONG Key,  IEtsExpMonthAtom* Value, IEtsExpMonthAtom** pRetVal);
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Maturity, m_nMaturity)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, HiddenCount, m_nHiddenCount)
	STDMETHOD(GetVegaWeight)(DATE dtExpiry, DOUBLE* pdRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsExpCalColl), CEtsExpCalColl)

#endif //__ETSEXPCALCOLL_H__
