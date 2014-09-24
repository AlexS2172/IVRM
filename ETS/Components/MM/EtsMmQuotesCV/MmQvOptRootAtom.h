// MmQvOptRootAtom.h : Declaration of the CMmQvOptRootAtom
#ifndef __MMQVOPTROOTATOM_H_
#define __MMQVOPTROOTATOM_H_
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"

_COM_SMARTPTR_TYPEDEF(IMmQvOptRootAtom, IID_IMmQvOptRootAtom);

struct __MmQvOptRootAtom
{
	LONG					m_nID;
	_bstr_t					m_bstrName;
	VARIANT_BOOL			m_bVisible;
	VARIANT_BOOL            m_bIsFit;
	LONG					m_nLotSize;
	VARIANT_BOOL			m_bSynthetic;
	ISynthRootAtomPtr		m_spSynthOptRoot;
	_bstr_t					m_bstrDPC;
	DOUBLE					m_dSU_Price;

	__MmQvOptRootAtom()				
		: m_nID(0L), m_bVisible(VARIANT_FALSE),m_nLotSize(0L),m_bSynthetic(VARIANT_FALSE)  ,m_dSU_Price(BAD_DOUBLE_VALUE), m_bIsFit(VARIANT_TRUE)
	{
	}
};

// CMmQvOptRootAtom
class ATL_NO_VTABLE CMmQvOptRootAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvOptRootAtom, &CLSID_MmQvOptRootAtom>,
	public ISupportErrorInfoImpl<&IID_IMmQvOptRootAtom>,
	public IDispatchImpl<IMmQvOptRootAtom, &IID_IMmQvOptRootAtom, &LIBID_EtsMmQuotesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmQvOptRootAtom
{
public:
	CMmQvOptRootAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVOPTROOTATOM)


BEGIN_COM_MAP(CMmQvOptRootAtom)
	COM_INTERFACE_ENTRY(IMmQvOptRootAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return 	CoCreateFreeThreadedMarshaler(	GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		m_spSynthOptRoot = NULL;
		m_pUnkMarshaler.Release();

	}

private:
	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTRT_PROPERTY(Name, m_bstrName)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, Visible, m_bVisible)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, LotSize, m_nLotSize)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, Synthetic, m_bSynthetic)
	IMPLEMENT_OBJECT_PROPERTY(ISynthRootAtom*, SynthOptRoot, m_spSynthOptRoot)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, SU_Price, m_dSU_Price)
	IMPLEMENT_BSTRT_PROPERTY(DPC, m_bstrDPC)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsFit, m_bIsFit)

};

OBJECT_ENTRY_AUTO(__uuidof(MmQvOptRootAtom), CMmQvOptRootAtom)

#endif //__MMQVOPTROOTATOM_H_