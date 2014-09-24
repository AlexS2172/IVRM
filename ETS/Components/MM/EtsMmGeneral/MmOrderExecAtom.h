// MmOrderExecAtom.h : Declaration of the CMmOrderExecAtom
#ifndef __MMORDEREXECATOM_H__
#define __MMORDEREXECATOM_H
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGeneral.h"

_COM_SMARTPTR_TYPEDEF(IMmOrderExecAtom, IID_IMmOrderExecAtom);

/////////////////////////////////////////////////////////////////////////////
//
struct __MmOrderExecAtom
{
	// core info
	LONG						m_nID;
	LONG						m_nSeqNum;
	EtsOrderExecutionStatusEnum	m_enExecStatus;
	LONG						m_nExecQty;
	DOUBLE						m_dExecPrice;
	DATE						m_dtExecDate;
	CComBSTR					m_bstrExecID;

	__MmOrderExecAtom() :
		m_nID(0L), m_nSeqNum(0L), m_enExecStatus(enOesUnknown),
		m_nExecQty(0L), m_dExecPrice(0.), m_dtExecDate(0.)
	{
	}
};

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmOrderExecAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmOrderExecAtom, &CLSID_MmOrderExecAtom>,
	public ISupportErrorInfoImpl<&IID_IMmOrderExecAtom>,
	public IDispatchImpl<IMmOrderExecAtom, &IID_IMmOrderExecAtom, &LIBID_EtsMmGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmOrderExecAtom
{
public:
	CMmOrderExecAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMORDEREXECATOM)

DECLARE_NOT_AGGREGATABLE(CMmOrderExecAtom)

BEGIN_COM_MAP(CMmOrderExecAtom)
	COM_INTERFACE_ENTRY(IMmOrderExecAtom)
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
	
	// core info
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, SeqNum, m_nSeqNum)
	IMPLEMENT_SIMPLE_PROPERTY(EtsOrderExecutionStatusEnum, ExecStatus, m_enExecStatus)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ExecQty, m_nExecQty)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ExecPrice, m_dExecPrice)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, ExecDate, m_dtExecDate)
	IMPLEMENT_BSTR_PROPERTY(ExecID, m_bstrExecID)

	//STDMETHOD(CopyTo)(IMmOrderExecAtom* pDest);
	STDMETHOD(get_OrderExecStatusString)(BSTR* pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmOrderExecAtom), CMmOrderExecAtom)

#endif //__MMORDEREXECATOM_H