// MmRpMatrixByStockAtom.h : Declaration of the CMmRpMatrixByStockAtom
#ifndef __MM_RP_MATRIX_BY_STOCK_ATOM__
#define __MM_RP_MATRIX_BY_STOCK_ATOM__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"
#include "MmRpMatrixByStockOptColl.h"

_COM_SMARTPTR_TYPEDEF(IMmRpMatrixByStockAtom, IID_IMmRpMatrixByStockAtom);

struct __MmRpMatrixByStockAtom
{
	LONG									m_nID;
	DOUBLE									m_dPrice;
	CComBSTR								m_bstrSymbol;
	EtsContractTypeEnum						m_enUndType;
	IMmRpMatrixByStockOptCollPtr			m_spOpt;

	__MmRpMatrixByStockAtom() :
		m_nID(0L), m_dPrice(0.), m_enUndType(enCtIndex)
	{
	}
};

// CMmRpMatrixByStockAtom

class ATL_NO_VTABLE CMmRpMatrixByStockAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpMatrixByStockAtom, &CLSID_MmRpMatrixByStockAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRpMatrixByStockAtom>,
	public IDispatchImpl<IMmRpMatrixByStockAtom, &IID_IMmRpMatrixByStockAtom, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRpMatrixByStockAtom
{
public:
	CMmRpMatrixByStockAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPMATRIXBYSTOCKATOM)

DECLARE_NOT_AGGREGATABLE(CMmRpMatrixByStockAtom)

BEGIN_COM_MAP(CMmRpMatrixByStockAtom)
	COM_INTERFACE_ENTRY(IMmRpMatrixByStockAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			__CHECK_HRESULT(m_spOpt.CreateInstance(CLSID_MmRpMatrixByStockOptColl), 
				_T("Failed to create options collection"));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpMatrixByStockAtom, e.Error());
		}

		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spOpt = NULL;
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG,					ID,			m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Price,		m_dPrice)
	IMPLEMENT_BSTR_PROPERTY(Symbol,		m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum,	UndType,	m_enUndType)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRpMatrixByStockOptColl*,	Opt,	m_spOpt)
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpMatrixByStockAtom), CMmRpMatrixByStockAtom)

#endif //__MM_RP_MATRIX_BY_STOCK_ATOM__