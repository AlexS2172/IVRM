// MmIhRequestAtom.h : Declaration of the CMmIhRequestAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmIndexHedge.h"
_COM_SMARTPTR_TYPEDEF(IMmIhIndexAtom, IID_IMmIhIndexAtom);
_COM_SMARTPTR_TYPEDEF(IMmIhOptAtom, IID_IMmIhOptAtom);
_COM_SMARTPTR_TYPEDEF(IMmIhUndAtom, IID_IMmIhUndAtom);


// CMmIhRequestAtom
struct __CMmIhRequestAtom
{
	IMmIhIndexAtomPtr m_spIndexAtom;
	IMmIhOptAtomPtr   m_spOptAtom;
	IMmIhUndAtomPtr	  m_spUndAtom;
	IMmIhUndAtomPtr	  m_spUnd2[2];
};

class ATL_NO_VTABLE CMmIhRequestAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmIhRequestAtom, &CLSID_MmIhRequestAtom>,
	public ISupportErrorInfoImpl<&IID_IMmIhRequestAtom>,
	public IDispatchImpl<IMmIhRequestAtom, &IID_IMmIhRequestAtom, &LIBID_EtsMmIndexHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0> ,
	public __CMmIhRequestAtom
{
public:
	CMmIhRequestAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMIHREQUESTATOM)


BEGIN_COM_MAP(CMmIhRequestAtom)
	COM_INTERFACE_ENTRY(IMmIhRequestAtom)
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
	}

public:

	IMPLEMENT_OBJECT_PROPERTY(IMmIhIndexAtom*,	Idx, m_spIndexAtom);
	IMPLEMENT_OBJECT_PROPERTY(IMmIhOptAtom*,	Opt, m_spOptAtom);
	IMPLEMENT_OBJECT_PROPERTY(IMmIhUndAtom*,	Und, m_spUndAtom);


	STDMETHOD(get_Und2)(LONG UndIndex, IMmIhUndAtom** pVal);
	STDMETHOD(putref_Und2)(LONG UndIndex, IMmIhUndAtom* newVal);

	STDMETHOD(GetQuoteUpdateParam)(QuoteUpdateParams* pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmIhRequestAtom), CMmIhRequestAtom)
