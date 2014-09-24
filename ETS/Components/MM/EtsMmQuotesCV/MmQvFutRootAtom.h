// MmQvFutRootAtom.h : Declaration of the CMmQvFutRootAtom
#ifndef __MMQVFUTROOTATOM_H_
#define __MMQVFUTROOTATOM_H_
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"

_COM_SMARTPTR_TYPEDEF(IMmQvFutRootAtom, IID_IMmQvFutRootAtom);

struct __MmQvFutRootAtom
{
	LONG					m_nID;
	CComBSTR				m_bstrSymbol;
	CComBSTR				m_bstrName;
	LONG					m_nMatCalendarID;
	LONG					m_nFutLotSize;
	LONG					m_nOptLotSize;

	__MmQvFutRootAtom()
		: m_nID(0L), m_nMatCalendarID(0L),
		m_nFutLotSize(0L), m_nOptLotSize(0L)
	{
	}
};

// CMmQvFutRootAtom

class ATL_NO_VTABLE CMmQvFutRootAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvFutRootAtom, &CLSID_MmQvFutRootAtom>,
	public ISupportErrorInfoImpl<&IID_IMmQvFutRootAtom>,
	public IDispatchImpl<IMmQvFutRootAtom, &IID_IMmQvFutRootAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmQvFutRootAtom
{
public:
	CMmQvFutRootAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVFUTROOTATOM)


BEGIN_COM_MAP(CMmQvFutRootAtom)
	COM_INTERFACE_ENTRY(IMmQvFutRootAtom)
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
		m_pUnkMarshaler.Release();
	}
private:
	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_BSTR_PROPERTY(Name, m_bstrName)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, MatCalendarID, m_nMatCalendarID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, FutLotSize, m_nFutLotSize)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, OptLotSize, m_nOptLotSize)

	STDMETHOD(CleanUp)();
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvFutRootAtom), CMmQvFutRootAtom)

#endif //__MMQVFUTROOTATOM_H_