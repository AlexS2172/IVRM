// MmRpExercisedStockAtom.h : Declaration of the CMmRpExercisedStockAtom
#ifndef __MM_RP_EXERCISED_STOCK_ATOM__
#define __MM_RP_EXERCISED_STOCK_ATOM__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpExercisedStockAtom, IID_IMmRpExercisedStockAtom);

struct __MmRpExercisedStockAtom
{
	LONG					m_nID;
	CComBSTR				m_bstrSymbol;
	EtsContractTypeEnum		m_enUndType;
	DATE					m_dtExpiry;
	CComBSTR				m_bstrSeries;
	DOUBLE					m_dPos;
	LONG					m_nPrev5ExpAvgVol;
	LONG					m_nPrev10TrdAvgVol;

	__MmRpExercisedStockAtom() :
		m_nID(0L),m_enUndType(enCtIndex), m_dtExpiry(0.), m_dPos(0.),
		m_nPrev5ExpAvgVol(0L), m_nPrev10TrdAvgVol(0L)
	{
	}
};

// CMmRpExercisedStockAtom

class ATL_NO_VTABLE CMmRpExercisedStockAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpExercisedStockAtom, &CLSID_MmRpExercisedStockAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRpExercisedStockAtom>,
	public IDispatchImpl<IMmRpExercisedStockAtom, &IID_IMmRpExercisedStockAtom, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRpExercisedStockAtom
{
public:
	CMmRpExercisedStockAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPEXERCISEDSTOCKATOM)

DECLARE_NOT_AGGREGATABLE(CMmRpExercisedStockAtom)

BEGIN_COM_MAP(CMmRpExercisedStockAtom)
	COM_INTERFACE_ENTRY(IMmRpExercisedStockAtom)
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
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG,						ID,					m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol,		m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum,		UndType,			m_enUndType)
	IMPLEMENT_SIMPLE_PROPERTY(DATE,						Expiry,				m_dtExpiry)
	IMPLEMENT_BSTR_PROPERTY(Series,		m_bstrSeries)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,					Pos,				m_dPos)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,						Prev5ExpAvgVol,		m_nPrev5ExpAvgVol)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,						Prev10TrdAvgVol,	m_nPrev10TrdAvgVol)
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpExercisedStockAtom), CMmRpExercisedStockAtom)

#endif //__MM_RP_EXERCISED_STOCK_ATOM__