// MmShOptRootAtom.h : Declaration of the CMmShOptRootAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"

// CMmShOptRootAtom
_COM_SMARTPTR_TYPEDEF(IMmShOptRootAtom, IID_IMmShOptRootAtom);

struct __MmShOptRootAtom
{
	ISynthRootAtomPtr	m_spSynthOptRoot;
	LONG				m_nID;
	LONG				m_nLotSize;
	VARIANT_BOOL		m_bIsSynth;

	__MmShOptRootAtom()
		: m_nID(BAD_LONG_VALUE), m_nLotSize(BAD_LONG_VALUE), m_bIsSynth(VARIANT_FALSE)
	{
	}
};

class ATL_NO_VTABLE CMmShOptRootAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShOptRootAtom, &CLSID_MmShOptRootAtom>,
	public IDispatchImpl<IMmShOptRootAtom, &IID_IMmShOptRootAtom, &LIBID_EtsMmStockHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmShOptRootAtom
{
public:
	CMmShOptRootAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHOPTROOTATOM)


BEGIN_COM_MAP(CMmShOptRootAtom)
	COM_INTERFACE_ENTRY(IMmShOptRootAtom)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spSynthOptRoot = NULL;
	}

public:
	IMPLEMENT_OBJECT_PROPERTY(ISynthRootAtom*, SynthOptRoot, m_spSynthOptRoot)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, LotSize, m_nLotSize)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsSynth, m_bIsSynth)
};

OBJECT_ENTRY_AUTO(__uuidof(MmShOptRootAtom), CMmShOptRootAtom)
