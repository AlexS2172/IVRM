// MmShRequestAtom.h : Declaration of the CMmShRequestAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"
#include "MmTePriceColl.h"


_COM_SMARTPTR_TYPEDEF(IMmShUndAtom, __uuidof(IMmShUndAtom));
_COM_SMARTPTR_TYPEDEF(IMmShOptAtom, __uuidof(IMmShOptAtom));
_COM_SMARTPTR_TYPEDEF(IMmTePriceColl, __uuidof(IMmTePriceColl));

struct __CMmShRequestAtom
{
	IMmShUndAtomPtr   m_spUndAtom;
	IMmShOptAtomPtr   m_spOptAtom;
	IMmTePriceCollPtr  m_spPriceColl;

	__CMmShRequestAtom()
	{
	}
};

// CMmShRequestAtom

class ATL_NO_VTABLE CMmShRequestAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShRequestAtom, &CLSID_MmShRequestAtom>,
	public ISupportErrorInfoImpl<&IID_IMmShRequestAtom>,
	public IDispatchImpl<IMmShRequestAtom, &IID_IMmShRequestAtom, &LIBID_EtsMmStockHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __CMmShRequestAtom
{
public:
	CMmShRequestAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHREQUESTATOM)


BEGIN_COM_MAP(CMmShRequestAtom)
	COM_INTERFACE_ENTRY(IMmShRequestAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		HRESULT hr = CComObject<CMmTePriceColl>::CreateInstance(&m_pPriceColl);
		if(SUCCEEDED(hr))
			hr = m_pPriceColl->QueryInterface(IID_IMmTePriceColl, (void**)&m_spPriceColl);
		return hr;
	}
	
	void FinalRelease() 
	{
		m_spPriceColl = NULL;
	}

	InstrumentTypeEnum _ContractTypeToPpInstrumentType(EtsContractTypeEnum enContractType);

private:
	CComObject<CMmTePriceColl>* m_pPriceColl;
public:

	IMPLEMENT_OBJECT_PROPERTY(IMmShUndAtom*,	Und,		m_spUndAtom);
	IMPLEMENT_OBJECT_PROPERTY(IMmShOptAtom*,    Opt,		m_spOptAtom);
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmTePriceColl*,	PriceColl,	m_spPriceColl);
	STDMETHOD(GetQuoteUpdateParam)(QuoteUpdateParams* pRetVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmShRequestAtom), CMmShRequestAtom)
