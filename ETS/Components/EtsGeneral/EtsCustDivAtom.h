// EtsCustDivAtom.h : Declaration of the CEtsCustDivAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"

_COM_SMARTPTR_TYPEDEF(IEtsCustDivAtom, IID_IEtsCustDivAtom);

struct __EtsCustDivAtom
{
	DATE					m_dtDate;
	DOUBLE					m_dAmount;

	__EtsCustDivAtom()
		: /*m_nID (0),*/ m_dtDate(0), m_dAmount(0.)
	{
	}
};

// CEtsCustDivAtom

class ATL_NO_VTABLE CEtsCustDivAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEtsCustDivAtom, &CLSID_EtsCustDivAtom>,
	public ISupportErrorInfo,
	public IDispatchImpl<IEtsCustDivAtom, &IID_IEtsCustDivAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0 > ,
	public __EtsCustDivAtom 
{
public:
	CEtsCustDivAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSCUSTDIVATOM)


BEGIN_COM_MAP(CEtsCustDivAtom)
	COM_INTERFACE_ENTRY(IEtsCustDivAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct ()
	{
		return S_OK;
	}
	
	void FinalRelease () 
	{
	}

public:
	//IMPLEMENT_SIMPLE_PROPERTY (LONG,	ID,			m_nID);
	IMPLEMENT_SIMPLE_PROPERTY (DOUBLE,	DivAmount,	m_dAmount);
	IMPLEMENT_SIMPLE_PROPERTY (DATE,	DivDate,	m_dtDate);
};

OBJECT_ENTRY_AUTO (__uuidof (EtsCustDivAtom), CEtsCustDivAtom)
