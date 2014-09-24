// MmShRowData.h : Declaration of the CMmShRowData

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"

_COM_SMARTPTR_TYPEDEF(IMmShTraderGroupAtom, IID_IMmShTraderGroupAtom);
_COM_SMARTPTR_TYPEDEF(IMmShStrategyGroupAtom, IID_IMmShStrategyGroupAtom);
_COM_SMARTPTR_TYPEDEF(IMmShUndGroupAtom, IID_IMmShUndGroupAtom);

struct __CMmShRowData
{
	IMmShTraderGroupAtomPtr		m_spTrdGrp;
	IMmShStrategyGroupAtomPtr   m_spStgGrp;
	IMmShUndGroupAtomPtr        m_spUndGrp;


};

// CMmShRowData

class ATL_NO_VTABLE CMmShRowData : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShRowData, &CLSID_MmShRowData>,
	public IDispatchImpl<IMmShRowData, &IID_IMmShRowData, &LIBID_EtsMmStockHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __CMmShRowData
{
public:
	CMmShRowData()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHROWDATA)


BEGIN_COM_MAP(CMmShRowData)
	COM_INTERFACE_ENTRY(IMmShRowData)
	COM_INTERFACE_ENTRY(IDispatch)
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

	IMPLEMENT_OBJECT_PROPERTY(IMmShTraderGroupAtom*,	TrdGrp, m_spTrdGrp);
	IMPLEMENT_OBJECT_PROPERTY(IMmShStrategyGroupAtom*,  StgGrp, m_spStgGrp);
	IMPLEMENT_OBJECT_PROPERTY(IMmShUndGroupAtom*,		UndGrp, m_spUndGrp);

};

OBJECT_ENTRY_AUTO(__uuidof(MmShRowData), CMmShRowData)
