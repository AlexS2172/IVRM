// MmVaStrikeAtom.h : Declaration of the CMmVaStrikeAtom
#ifndef __MMVASTRIKEATOM_H__
#define __MMVASTRIKEATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmVolaAnalysis.h"
#include "MmVaOptAtom.h"
#include "MmVaOptPairColl.h"

_COM_SMARTPTR_TYPEDEF(IMmVaStrikeAtom, IID_IMmVaStrikeAtom);

struct __MmVaStrikeAtom
{
	DOUBLE				m_dStrike;
	IMmVaOptPairCollPtr	m_spOptPair;
	CComBSTR			m_bstrRootNames;
	IMmVaOptPairAtomPtr	m_spCurOptPair;

	__MmVaStrikeAtom()
		: m_dStrike(0.)
	{
	}
};

// CMmVaStrikeAtom

class ATL_NO_VTABLE CMmVaStrikeAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmVaStrikeAtom, &CLSID_MmVaStrikeAtom>,
	public ISupportErrorInfoImpl<&IID_IMmVaStrikeAtom>,
	public IDispatchImpl<IMmVaStrikeAtom, &IID_IMmVaStrikeAtom, &LIBID_EtsMmVolaAnalysisLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmVaStrikeAtom
{
public:
	CMmVaStrikeAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMVASTRIKEATOM)


BEGIN_COM_MAP(CMmVaStrikeAtom)
	COM_INTERFACE_ENTRY(IMmVaStrikeAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			__CHECK_HRESULT(m_spOptPair.CreateInstance(CLSID_MmVaOptPairColl), _T("Fail to create option pairs collection."));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmVaStrikeAtom, e.Error());
		}
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spCurOptPair = NULL;
		m_spOptPair = NULL;
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Strike, m_dStrike)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmVaOptPairColl*, OptPair, m_spOptPair)
	IMPLEMENT_BSTR_PROPERTY(RootNames, m_bstrRootNames)
	IMPLEMENT_OBJECT_PROPERTY(IMmVaOptPairAtom*, CurOptPair, m_spCurOptPair)
};

OBJECT_ENTRY_AUTO(__uuidof(MmVaStrikeAtom), CMmVaStrikeAtom)

#endif //__MMVASTRIKEATOM_H__