// ExchAtom.h : Declaration of the CExchAtom
#ifndef __EXCHATOM_H__
#define __EXCHATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"

struct __ExchAtom
{
	LONG			m_nID;
	_bstr_t			m_bstrCode;
	_bstr_t			m_bstrName;
	VARIANT_BOOL	m_bIsUnderlying;
	VARIANT_BOOL	m_bIsOption;
	VARIANT_BOOL	m_bVisible;

	_bstr_t			m_bstrRegion;
	DOUBLE			m_dRate;

	__ExchAtom()
		: m_nID(0L), m_bIsUnderlying(VARIANT_FALSE), m_bIsOption(VARIANT_FALSE), m_bVisible(VARIANT_FALSE),
		  m_dRate(1)
	{
	}
};

// CExchAtom

class ATL_NO_VTABLE CExchAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CExchAtom, &CLSID_ExchAtom>,
	public ISupportErrorInfoImpl<&IID_IExchAtom>,
	public IDispatchImpl<IExchAtom, &IID_IExchAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __ExchAtom
{
public:
	CExchAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_EXCHATOM)


BEGIN_COM_MAP(CExchAtom)
	COM_INTERFACE_ENTRY(IExchAtom)
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
	IMPLEMENT_BSTRT_PROPERTY(Code, m_bstrCode);
	IMPLEMENT_BSTRT_PROPERTY(Name, m_bstrName);
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsUnderlying, m_bIsUnderlying);
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsOption, m_bIsOption);
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, Visible, m_bVisible);

	IMPLEMENT_BSTRT_PROPERTY(Region, m_bstrRegion);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Rate, m_dRate);
};

OBJECT_ENTRY_AUTO(__uuidof(ExchAtom), CExchAtom)

#endif //__EXCHATOM_H__
