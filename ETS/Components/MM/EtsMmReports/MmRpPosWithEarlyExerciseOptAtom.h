// MmRpPosWithEarlyExerciseOptAtom.h : Declaration of the CMmRpPosWithEarlyExerciseOptAtom
#ifndef __MM_RP_POS_WITH_EARLY_EXERCISE_OPT_ATOM__
#define __MM_RP_POS_WITH_EARLY_EXERCISE_OPT_ATOM__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpPosWithEarlyExerciseOptAtom, IID_IMmRpPosWithEarlyExerciseOptAtom);

struct __MmRpPosWithEarlyExerciseOptAtom
{	
	LONG					m_nStockID;
	CComBSTR				m_bstrSymbol;
	CComBSTR				m_bstrRootSymbol;
	DOUBLE					m_dCallPos;
	DOUBLE					m_dPutPos;
	LONG					m_nRootID;
	DATE					m_dtExpiry;
	DOUBLE					m_dStrike;
	VARIANT_BOOL			m_vbCallEarlyExercise;
	VARIANT_BOOL			m_vbPutEarlyExercise;
	DOUBLE					m_dDividents;
	DOUBLE					m_dInterest ;
	DOUBLE					m_dpOTM ;
	DOUBLE					m_dcOTM ;

	__MmRpPosWithEarlyExerciseOptAtom() :
		m_nStockID(0L), m_dCallPos(0.), m_dPutPos(0.), m_nRootID(0L), 
		m_dtExpiry(0.), m_dStrike(0.), m_vbCallEarlyExercise(VARIANT_FALSE),
		m_vbPutEarlyExercise(VARIANT_FALSE),
		m_dDividents(0.0), m_dInterest(0), m_dpOTM(0.0), m_dcOTM(0.0)
	{
	}
};

// CMmRpPosWithEarlyExerciseOptAtom

class ATL_NO_VTABLE CMmRpPosWithEarlyExerciseOptAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpPosWithEarlyExerciseOptAtom, &CLSID_MmRpPosWithEarlyExerciseOptAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRpPosWithEarlyExerciseOptAtom>,
	public IDispatchImpl<IMmRpPosWithEarlyExerciseOptAtom, &IID_IMmRpPosWithEarlyExerciseOptAtom, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRpPosWithEarlyExerciseOptAtom
{
public:
	CMmRpPosWithEarlyExerciseOptAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPPOSWITHEARLYEXERCISEOPTATOM)

DECLARE_NOT_AGGREGATABLE(CMmRpPosWithEarlyExerciseOptAtom)

BEGIN_COM_MAP(CMmRpPosWithEarlyExerciseOptAtom)
	COM_INTERFACE_ENTRY(IMmRpPosWithEarlyExerciseOptAtom)
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
	
	IMPLEMENT_SIMPLE_PROPERTY(LONG,			StockID,			m_nStockID)
	IMPLEMENT_BSTR_PROPERTY(Symbol,			m_bstrSymbol)
	IMPLEMENT_BSTR_PROPERTY(RootSymbol,		m_bstrRootSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		CallPos,			m_dCallPos)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		PutPos,				m_dPutPos)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,			RootID,				m_nRootID)
	IMPLEMENT_SIMPLE_PROPERTY(DATE,			Expiry,				m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		Strike,				m_dStrike)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL,	CallEarlyExercise,	m_vbCallEarlyExercise)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL,	PutEarlyExercise,	m_vbPutEarlyExercise)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		Dividents,			m_dDividents)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		Interest,			m_dInterest)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		pOTM,				m_dpOTM)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		cOTM,				m_dcOTM)
	STDMETHODIMP get_AreAnyEarlyExerc(VARIANT_BOOL* pVal)
	{
		if(!pVal)
		{
			return E_POINTER;
		}
		if ( m_vbCallEarlyExercise == VARIANT_TRUE || m_vbPutEarlyExercise == VARIANT_TRUE  )
			*pVal = VARIANT_TRUE ;
		else
			*pVal = VARIANT_FALSE ;
		return S_OK ;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpPosWithEarlyExerciseOptAtom), CMmRpPosWithEarlyExerciseOptAtom)

#endif //__MM_RP_POS_WITH_EARLY_EXERCISE_OPT_ATOM__