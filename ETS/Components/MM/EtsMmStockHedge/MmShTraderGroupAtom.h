// MmShTraderGroupAtom.h : Declaration of the CMmShTraderGroupAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"
#include "MmShStrategyGroupColl.h"

_COM_SMARTPTR_TYPEDEF(IMmShStrategyGroupAtom, IID_IMmShStrategyGroupAtom);

struct __MmShTraderGroupAtom
{
	IEtsTraderAtomPtr			m_spTrader;
	IMmShStrategyGroupCollPtr	m_spStrategy;
	MmShGroupStatusEnum			m_enStatus;

	__MmShTraderGroupAtom()
		: m_enStatus(enMmShGroupStatusAll) 
	{
	}
};

// CMmShTraderGroupAtom

class ATL_NO_VTABLE CMmShTraderGroupAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShTraderGroupAtom, &CLSID_MmShTraderGroupAtom>,
	public IDispatchImpl<IMmShTraderGroupAtom, &IID_IMmShTraderGroupAtom, &LIBID_EtsMmStockHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmShTraderGroupAtom
{
public:
	CMmShTraderGroupAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHTRADERGROUPATOM)


BEGIN_COM_MAP(CMmShTraderGroupAtom)
	COM_INTERFACE_ENTRY(IMmShTraderGroupAtom)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			_CHK(m_spStrategy.CreateInstance(CLSID_MmShStrategyGroupColl), _T("Fail to create strategy group."));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmShTraderGroupAtom, e.Error());
		}

		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spTrader = NULL;
		m_spStrategy = NULL;
	}

public:

	IMPLEMENT_OBJECT_PROPERTY(IEtsTraderAtom*, Trader, m_spTrader)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmShStrategyGroupColl*, Strategy, m_spStrategy)
	IMPLEMENT_SIMPLE_PROPERTY(MmShGroupStatusEnum, Status, m_enStatus)
};

OBJECT_ENTRY_AUTO(__uuidof(MmShTraderGroupAtom), CMmShTraderGroupAtom)
_COM_SMARTPTR_TYPEDEF(IMmShTraderGroupAtom, IID_IMmShTraderGroupAtom);
