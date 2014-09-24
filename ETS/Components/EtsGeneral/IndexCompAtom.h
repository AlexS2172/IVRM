// IndexCompAtom.h : Declaration of the CIndexCompAtom
#ifndef __INDEXCOMPATOM_H__
#define __INDEXCOMPATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"

_COM_SMARTPTR_TYPEDEF(IIndexCompAtom, IID_IIndexCompAtom);

struct __IndexCompAtom
{
	LONG			m_nID;
	DOUBLE			m_dBeta;
	DOUBLE			m_dWeight;
	VARIANT_BOOL    m_bBasketComponent;

	__IndexCompAtom()
		: m_nID(0L), m_dBeta(0.), m_dWeight(0.), m_bBasketComponent(VARIANT_FALSE)
	{
	}
};

// CIndexCompAtom

class ATL_NO_VTABLE CIndexCompAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CIndexCompAtom, &CLSID_IndexCompAtom>,
	public ISupportErrorInfoImpl<&IID_IIndexCompAtom>,
	public IDispatchImpl<IIndexCompAtom, &IID_IIndexCompAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __IndexCompAtom
{
public:
	CIndexCompAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INDEXCOMPATOM)


BEGIN_COM_MAP(CIndexCompAtom)
	COM_INTERFACE_ENTRY(IIndexCompAtom)
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

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Beta, m_dBeta);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Weight, m_dWeight);
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsBasketComponent, m_bBasketComponent);
};

OBJECT_ENTRY_AUTO(__uuidof(IndexCompAtom), CIndexCompAtom)

#endif //__INDEXCOMPATOM_H__