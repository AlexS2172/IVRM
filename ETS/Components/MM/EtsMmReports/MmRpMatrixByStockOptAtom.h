// MmRpMatrixByStockOptAtom.h : Declaration of the CMmRpMatrixByStockOptAtom
#ifndef __MM_RP_MATRIX_BY_STOCKOPT_ATOM__
#define __MM_RP_MATRIX_BY_STOCKOPT_ATOM__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"

_COM_SMARTPTR_TYPEDEF(IMmRpMatrixByStockOptAtom, IID_IMmRpMatrixByStockOptAtom);

struct __MmRpMatrixByStockOptAtom
{
	LONG					m_nID;
	LONG					m_nStockID;
	CComBSTR				m_bstrSymbol;
	CComBSTR				m_bstrRootSymbol;
	DOUBLE					m_dPos;
	LONG					m_nRootID;
	DATE					m_dtExpiry;
	DOUBLE					m_dStrike;
	EtsOptionTypeEnum		m_enOptType;

	__MmRpMatrixByStockOptAtom() :
		m_nID(0L), m_nStockID(0L), m_dPos(0.), m_nRootID(0L), 
		m_dtExpiry(0.), m_dStrike(0.), m_enOptType(enOtCall)
	{
	}
};

// CMmRpMatrixByStockOptAtom

class ATL_NO_VTABLE CMmRpMatrixByStockOptAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpMatrixByStockOptAtom, &CLSID_MmRpMatrixByStockOptAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRpMatrixByStockOptAtom>,
	public IDispatchImpl<IMmRpMatrixByStockOptAtom, &IID_IMmRpMatrixByStockOptAtom, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRpMatrixByStockOptAtom
{
public:
	CMmRpMatrixByStockOptAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPMATRIXBYSTOCKOPTATOM)

DECLARE_NOT_AGGREGATABLE(CMmRpMatrixByStockOptAtom)

BEGIN_COM_MAP(CMmRpMatrixByStockOptAtom)
	COM_INTERFACE_ENTRY(IMmRpMatrixByStockOptAtom)
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

	IMPLEMENT_SIMPLE_PROPERTY(LONG,					ID,			m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					StockID,	m_nStockID)
	IMPLEMENT_BSTR_PROPERTY(Symbol,			m_bstrSymbol)
	IMPLEMENT_BSTR_PROPERTY(RootSymbol,		m_bstrRootSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Pos,		m_dPos)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					RootID,		m_nRootID)
	IMPLEMENT_SIMPLE_PROPERTY(DATE,					Expiry,		m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Strike,		m_dStrike)
	IMPLEMENT_SIMPLE_PROPERTY(EtsOptionTypeEnum,	OptType,	m_enOptType)
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpMatrixByStockOptAtom), CMmRpMatrixByStockOptAtom)

#endif //__MM_RP_MATRIX_BY_STOCKOPT_ATOM__