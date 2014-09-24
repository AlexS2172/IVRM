// MmShUndGroupAtom.h : Declaration of the CMmShUndGroupAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"
#include "MmShPosColl.h"

_COM_SMARTPTR_TYPEDEF(IMmShUndGroupAtom, IID_IMmShUndGroupAtom);

struct __MmShUndGroupAtom
{
	IUndAtomPtr					m_spUnd;
	MmShGroupStatusEnum			m_enStatus;

	DOUBLE						m_dNetDelta;
	LONG						m_nDeltaQty;
	LONG						m_nHedgeQty;

	IMmShPosCollPtr				m_spPos;

	__MmShUndGroupAtom()
		: m_enStatus(enMmShGroupStatusAll), m_dNetDelta(BAD_DOUBLE_VALUE), m_nDeltaQty(BAD_LONG_VALUE),
		m_nHedgeQty(BAD_LONG_VALUE) 
	{
	}
};

// CMmShUndGroupAtom

class ATL_NO_VTABLE CMmShUndGroupAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShUndGroupAtom, &CLSID_MmShUndGroupAtom>,
	public IDispatchImpl<IMmShUndGroupAtom, &IID_IMmShUndGroupAtom, &LIBID_EtsMmStockHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmShUndGroupAtom
{
public:
	CMmShUndGroupAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHUNDGROUPATOM)


BEGIN_COM_MAP(CMmShUndGroupAtom)
	COM_INTERFACE_ENTRY(IMmShUndGroupAtom)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			_CHK(m_spPos.CreateInstance(CLSID_MmShPosColl), _T("Fail to create underlyings."));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmShUndGroupAtom, e.Error());
		}

		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spPos = NULL;
		m_spUnd = NULL;
	}

public:
	IMPLEMENT_OBJECT_PROPERTY(IUndAtom*, Und, m_spUnd)
	IMPLEMENT_SIMPLE_PROPERTY(MmShGroupStatusEnum, Status, m_enStatus)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetDelta, m_dNetDelta)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, DeltaQty, m_nDeltaQty)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, HedgeQty, m_nHedgeQty)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmShPosColl*, Pos, m_spPos)
};

OBJECT_ENTRY_AUTO(__uuidof(MmShUndGroupAtom), CMmShUndGroupAtom)
