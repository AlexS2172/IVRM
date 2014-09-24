// MmShPosAtom.h : Declaration of the CMmShPosAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"
#include "MmShUndAtom.h"
#include "MmShOptAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmShPosAtom, IID_IMmShPosAtom);

struct __MmShPosAtom
{
	LONG						m_nID;
	LONG						m_nQty;
	DOUBLE						m_dDelta;

	IMmShUndAtomPtr				m_spUnd;
	IMmShOptAtomPtr				m_spOpt;

	__MmShPosAtom()
		: m_nID(0L), m_nQty(BAD_LONG_VALUE), m_dDelta(BAD_DOUBLE_VALUE)
	{
	}
};

// CMmShPosAtom

class ATL_NO_VTABLE CMmShPosAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShPosAtom, &CLSID_MmShPosAtom>,
	public IDispatchImpl<IMmShPosAtom, &IID_IMmShPosAtom, &LIBID_EtsMmStockHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmShPosAtom
{
public:
	CMmShPosAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHPOSATOM)


BEGIN_COM_MAP(CMmShPosAtom)
	COM_INTERFACE_ENTRY(IMmShPosAtom)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spUnd = NULL;
		m_spOpt = NULL;
	}

public:
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Qty, m_nQty)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Delta, m_dDelta)
	IMPLEMENT_OBJECT_PROPERTY(IMmShUndAtom*, Und, m_spUnd)
	IMPLEMENT_OBJECT_PROPERTY(IMmShOptAtom*, Opt, m_spOpt)
};

OBJECT_ENTRY_AUTO(__uuidof(MmShPosAtom), CMmShPosAtom)
