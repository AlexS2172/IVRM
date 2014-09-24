// MmRvReqAtom.h : Declaration of the CMmRvReqAtom
#ifndef __MMRVREQATOM_H__
#define __MMRVREQATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmRisks.h"
#include "MmRvUndAtom.h"
#include "MmRvPosAtom.h"
#include "MmRvFutAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmRvReqAtom, IID_IMmRvReqAtom);

/////////////////////////////////////////////////////////////////////////////
//
struct __MmRvReqAtom
{
	IMmRvUndAtomPtr		m_spUnd;
	IMmRvPosAtomPtr		m_spPos;
	IMmRvFutAtomPtr		m_spFut;

	VARIANT_BOOL		m_bIndexOnly;

	__MmRvReqAtom() :
		m_bIndexOnly(VARIANT_FALSE)
	{
	}
};

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmRvReqAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRvReqAtom, &CLSID_MmRvReqAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRvReqAtom>,
	public IDispatchImpl<IMmRvReqAtom, &IID_IMmRvReqAtom, &LIBID_EtsMmRisksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRvReqAtom
{
public:
	CMmRvReqAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRVREQATOM)

DECLARE_NOT_AGGREGATABLE(CMmRvReqAtom)

BEGIN_COM_MAP(CMmRvReqAtom)
	COM_INTERFACE_ENTRY(IMmRvReqAtom)
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
		m_spUnd = NULL;
		m_spPos = NULL;
		m_spFut = NULL;
	}

private:
	InstrumentTypeEnum _ContractTypeToPpInstrumentType(EtsContractTypeEnum enContractType);

public:
	
	IMPLEMENT_OBJECT_PROPERTY(IMmRvUndAtom*, Und, m_spUnd)
	IMPLEMENT_OBJECT_PROPERTY(IMmRvPosAtom*, Pos, m_spPos)
	IMPLEMENT_OBJECT_PROPERTY(IMmRvFutAtom*, Fut, m_spFut)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IndexOnly, m_bIndexOnly)

	STDMETHOD(GetQuoteUpdateParam)(QuoteUpdateParams* pRetVal);
	STDMETHOD(GetGroupQuoteUpdateParam)(QuoteUpdateParams* pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvReqAtom), CMmRvReqAtom)

#endif //__MMRVREQATOM_H__