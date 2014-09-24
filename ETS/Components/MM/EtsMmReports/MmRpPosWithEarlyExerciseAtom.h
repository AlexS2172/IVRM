// MmRpPosWithEarlyExerciseAtom.h : Declaration of the CMmRpPosWithEarlyExerciseAtom
#ifndef __MM_RP_POS_WITH_EARLY_EXERCISE_ATOM__
#define __MM_RP_POS_WITH_EARLY_EXERCISE_ATOM__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"
#include "MmRpPosWithEarlyExerciseOptColl.h"

_COM_SMARTPTR_TYPEDEF(IMmRpPosWithEarlyExerciseAtom, IID_IMmRpPosWithEarlyExerciseAtom);

struct __MmRpPosWithEarlyExerciseAtom
{
	LONG									m_nID;
	DOUBLE									m_dPosInShares;
	DOUBLE									m_dPrice;
	CComBSTR								m_bstrSymbol;
	EtsContractTypeEnum						m_enUndType;
	CComBSTR								m_bstrNetDelta;
	IMmRpPosWithEarlyExerciseOptCollPtr		m_spOpt;

	__MmRpPosWithEarlyExerciseAtom() :
		m_nID(0L), m_dPosInShares(0.), m_dPrice(0.), m_enUndType(enCtIndex)
	{
	}
};

// CMmRpPosWithEarlyExerciseAtom

class ATL_NO_VTABLE CMmRpPosWithEarlyExerciseAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpPosWithEarlyExerciseAtom, &CLSID_MmRpPosWithEarlyExerciseAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRpPosWithEarlyExerciseAtom>,
	public IDispatchImpl<IMmRpPosWithEarlyExerciseAtom, &IID_IMmRpPosWithEarlyExerciseAtom, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRpPosWithEarlyExerciseAtom
{
public:
	CMmRpPosWithEarlyExerciseAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPPOSWITHEARLYEXERCISEATOM)

DECLARE_NOT_AGGREGATABLE(CMmRpPosWithEarlyExerciseAtom)

BEGIN_COM_MAP(CMmRpPosWithEarlyExerciseAtom)
	COM_INTERFACE_ENTRY(IMmRpPosWithEarlyExerciseAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{	
			__CHECK_HRESULT(m_spOpt.CreateInstance(CLSID_MmRpPosWithEarlyExerciseOptColl), 
				_T("Failed to create MmRpPosWithEarlyExerciseOptColl object."));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpPosWithEarlyExerciseAtom, e.Error());
		}

		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spOpt = NULL;
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG,					ID,				m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				PosInShares,	m_dPosInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Price,			m_dPrice)
	IMPLEMENT_BSTR_PROPERTY(Symbol,		m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum,	UndType,		m_enUndType)
	IMPLEMENT_BSTR_PROPERTY(NetDelta,	m_bstrNetDelta)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRpPosWithEarlyExerciseOptColl*,	Opt,	m_spOpt)
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpPosWithEarlyExerciseAtom), CMmRpPosWithEarlyExerciseAtom)

#endif //__MM_RP_POS_WITH_EARLY_EXERCISE_ATOM__