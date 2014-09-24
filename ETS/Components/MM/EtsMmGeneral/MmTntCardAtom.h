// MmTntCardAtom.h : Declaration of the CMmTntCardAtom
#ifndef __MMTNTCARDATOM_H__
#define __MMTNTCARDATOM_H
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGeneral.h"
#include "MmTntCounterPartyAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmTntCardAtom, IID_IMmTntCardAtom);

/////////////////////////////////////////////////////////////////////////////
//
struct __MmTntCardAtom
{
	// core info
	LONG						m_nID;
	LONG						m_nSeqNum;
	DATE						m_dtTradeDate;
	CComBSTR					m_bstrTradeID;
	VARIANT_BOOL				m_bIsBuy;
	LONG						m_nQty;
	DOUBLE						m_dPrice;
	CComBSTR					m_bstrPoeID;
	EtsTntCardStatusEnum		m_enStatus;
	CComBSTR					m_bstrAccount;

	// TNT misc data
	CComBSTR					m_bstrExecutingFirm;
	CComBSTR					m_bstrCMTA;
	CComBSTR					m_bstrQuantifier;
	CComBSTR					m_bstrContraFirm;
	CComBSTR					m_bstrSpreadIndicator;
	CComBSTR					m_bstrOptionalData;
	VARIANT_BOOL				m_bMakeTrade;
	VARIANT_BOOL				m_bIsExpired;

	// additional info
	CComBSTR					m_bstrMsg;
	IUndAtomPtr					m_spUnd;
	IEtsOptAtomPtr				m_spOpt;
	IEtsOptRootAtomPtr			m_spOptRoot;
	IMmTntCounterPartyAtomPtr	m_spCntPty;
	CComBSTR					m_bstrDesc;

	__MmTntCardAtom() :
		m_nID(0L), m_nSeqNum(0L), m_dtTradeDate(0.),
		m_bIsBuy(VARIANT_FALSE), m_nQty(0L), m_dPrice(0.),
		m_enStatus(enTcsUnknown), m_bMakeTrade(VARIANT_FALSE),
		m_bIsExpired(VARIANT_FALSE)
	{
	}
};

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmTntCardAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmTntCardAtom, &CLSID_MmTntCardAtom>,
	public ISupportErrorInfoImpl<&IID_IMmTntCardAtom>,
	public IDispatchImpl<IMmTntCardAtom, &IID_IMmTntCardAtom, &LIBID_EtsMmGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmTntCardAtom
{
public:
	CMmTntCardAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMTNTCARDATOM)

DECLARE_NOT_AGGREGATABLE(CMmTntCardAtom)

BEGIN_COM_MAP(CMmTntCardAtom)
	COM_INTERFACE_ENTRY(IMmTntCardAtom)
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
		m_spCntPty = NULL;
		m_spOptRoot = NULL;
		m_spUnd = NULL;
		m_spOpt = NULL;
	}

public:
	
	// core info
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, SeqNum, m_nSeqNum)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, TradeDate, m_dtTradeDate)
	IMPLEMENT_BSTR_PROPERTY(TradeID, m_bstrTradeID)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsBuy, m_bIsBuy)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Qty, m_nQty)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Price, m_dPrice)
	IMPLEMENT_BSTR_PROPERTY(PoeID, m_bstrPoeID)
	IMPLEMENT_SIMPLE_PROPERTY(EtsTntCardStatusEnum, Status, m_enStatus)
	IMPLEMENT_BSTR_PROPERTY(Account, m_bstrAccount)

	// TNT misc data
	IMPLEMENT_BSTR_PROPERTY(ExecutingFirm, m_bstrExecutingFirm)
	IMPLEMENT_BSTR_PROPERTY(CMTA, m_bstrCMTA)
	IMPLEMENT_BSTR_PROPERTY(Quantifier, m_bstrQuantifier)
	IMPLEMENT_BSTR_PROPERTY(ContraFirm, m_bstrContraFirm)
	IMPLEMENT_BSTR_PROPERTY(SpreadIndicator, m_bstrSpreadIndicator)
	IMPLEMENT_BSTR_PROPERTY(OptionalData, m_bstrOptionalData)

	// additional info
	IMPLEMENT_BSTR_PROPERTY(Msg, m_bstrMsg)
	IMPLEMENT_OBJECT_PROPERTY(IUndAtom*, Und, m_spUnd)
	IMPLEMENT_OBJECT_PROPERTY(IEtsOptAtom*, Opt, m_spOpt)
	IMPLEMENT_OBJECT_PROPERTY(IEtsOptRootAtom*, OptRoot, m_spOptRoot)
	IMPLEMENT_OBJECT_PROPERTY(IMmTntCounterPartyAtom*, CntPty, m_spCntPty)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, MakeTrade, m_bMakeTrade)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsExpired, m_bIsExpired)
	IMPLEMENT_BSTR_PROPERTY(Desc, m_bstrDesc)

	// helpers
	STDMETHOD(get_CardStatusString)(BSTR* pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmTntCardAtom), CMmTntCardAtom)

#endif //__MMTNTCARDATOM_H