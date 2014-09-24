// MmShStrategyGroupAtom.h : Declaration of the CMmShStrategyGroupAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"
#include "MmShUndGroupColl.h"

_COM_SMARTPTR_TYPEDEF(IMmShStrategyGroupAtom, IID_IMmShStrategyGroupAtom);

struct __MmShStrategyGroupAtom
{
	IEtsStrategyAtomPtr			m_spStrategy;
	IMmShUndGroupCollPtr		m_spUnd;
	MmShGroupStatusEnum			m_enStatus;

	__MmShStrategyGroupAtom()
		: m_enStatus(enMmShGroupStatusAll) 
	{
	}
};

// CMmShStrategyGroupAtom
class ATL_NO_VTABLE CMmShStrategyGroupAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShStrategyGroupAtom, &CLSID_MmShStrategyGroupAtom>,
	public IDispatchImpl<IMmShStrategyGroupAtom, &IID_IMmShStrategyGroupAtom, &LIBID_EtsMmStockHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmShStrategyGroupAtom
{
public:
	CMmShStrategyGroupAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHSTRATEGYGROUPATOM)


BEGIN_COM_MAP(CMmShStrategyGroupAtom)
	COM_INTERFACE_ENTRY(IMmShStrategyGroupAtom)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			_CHK(m_spUnd.CreateInstance(CLSID_MmShUndGroupColl), _T("Fail to create underlying group."));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmShStrategyGroupAtom, e.Error());
		}

		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spUnd = NULL;
		m_spStrategy = NULL;
	}

public:

	IMPLEMENT_OBJECT_PROPERTY(IEtsStrategyAtom*, Strategy, m_spStrategy)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmShUndGroupColl*, Und, m_spUnd)
	IMPLEMENT_SIMPLE_PROPERTY(MmShGroupStatusEnum, Status, m_enStatus)
};

OBJECT_ENTRY_AUTO(__uuidof(MmShStrategyGroupAtom), CMmShStrategyGroupAtom)
