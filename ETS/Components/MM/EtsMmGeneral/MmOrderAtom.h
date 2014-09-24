// MmOrderAtom.h : Declaration of the CMmOrderAtom
#ifndef __MMORDERATOM_H__
#define __MMORDERATOM_H
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGeneral.h"
#include "MmOrderExecColl.h"
#include "MmOrderExecDestAtom.h"
//#include "MmTradeInfoColl.h"

_COM_SMARTPTR_TYPEDEF(IMmOrderAtom, IID_IMmOrderAtom);

/////////////////////////////////////////////////////////////////////////////
//
struct __MmOrderAtom
{
	// core info
	LONG						m_nID;
	LONG						m_nSeqNum;
	DATE						m_dtUpdateDate;
	IMmOrderExecDestAtomPtr		m_spDest;
	EtsOrderStatusEnum			m_enStatus;
	EtsOrderTypeEnum			m_enOrderType;
	VARIANT_BOOL				m_bIsBuy;
	LONG						m_nOrderQty;
	DOUBLE						m_dOrderPrice;
	DATE						m_dtOrderDate;
	DOUBLE						m_dAvgPrice;
	LONG						m_nCumQty;
	_bstr_t						m_bstrOrderID;

	// additional info
	IEtsTraderAtomPtr			m_spTrader;
	IUndAtomPtr					m_spUnd;
	IEtsOptAtomPtr				m_spOpt;
	IEtsOptRootAtomPtr			m_spOptRoot;
	IMmOrderExecCollPtr			m_spExecs;
	IMmTradeInfoCollPtr			m_spTrades;
	_bstr_t						m_bstrRequestUID;

	__MmOrderAtom() :
		m_nID(0L), m_nSeqNum(0L), m_dtUpdateDate(0.),
		m_enStatus(enOrsUnknown), m_enOrderType(enOrtUnknown),
		m_bIsBuy(VARIANT_FALSE), m_nOrderQty(0L), m_dOrderPrice(0.), m_dtOrderDate(0.),
		m_dAvgPrice(0.), m_nCumQty(0L)
	{
	}
};

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmOrderAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmOrderAtom, &CLSID_MmOrderAtom>,
	public ISupportErrorInfoImpl<&IID_IMmOrderAtom>,
	public IDispatchImpl<IMmOrderAtom, &IID_IMmOrderAtom, &LIBID_EtsMmGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmOrderAtom
{
public:
	CMmOrderAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMORDERATOM)

DECLARE_NOT_AGGREGATABLE(CMmOrderAtom)

BEGIN_COM_MAP(CMmOrderAtom)
	COM_INTERFACE_ENTRY(IMmOrderAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			CComObject<CMmOrderExecColl>* pObject = NULL;
			_CHK(CComObject<CMmOrderExecColl>::CreateInstance(&pObject), _T("Fail to create orders executions collection"));
			m_spExecs.Attach(pObject, TRUE);
			
			//CComObject<CMmTradeInfoColl>* pTradeColl = NULL;
			//_CHK(CComObject<CMmTradeInfoColl>::CreateInstance(&pTradeColl), _T("Fail to create trades collection for order."));
			//m_spTrades.Attach(pTradeColl, TRUE);

			m_spTrades = IMmTradeInfoCollPtr(__uuidof(MmTradeInfoColl));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmOrderAtom, e.Error());
		}

		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spExecs = NULL;
		m_spDest = NULL;
		m_spOptRoot = NULL;
		m_spUnd = NULL;
		m_spTrader = NULL;
		m_spOpt = NULL;
		m_spTrades = NULL;
	}

public:
	
	// core info
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, SeqNum, m_nSeqNum)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, UpdateDate, m_dtUpdateDate)
	IMPLEMENT_OBJECT_PROPERTY(IMmOrderExecDestAtom*, Dest, m_spDest)
	IMPLEMENT_SIMPLE_PROPERTY(EtsOrderStatusEnum, Status, m_enStatus)
	IMPLEMENT_SIMPLE_PROPERTY(EtsOrderTypeEnum, OrderType, m_enOrderType)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsBuy, m_bIsBuy)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, OrderQty, m_nOrderQty)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, OrderPrice, m_dOrderPrice)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, OrderDate, m_dtOrderDate)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, AvgPrice, m_dAvgPrice)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, CumQty, m_nCumQty)
	IMPLEMENT_BSTRT_PROPERTY(OrderID, m_bstrOrderID)

	// additional info
	IMPLEMENT_OBJECT_PROPERTY(IEtsTraderAtom*, Trader, m_spTrader)
	IMPLEMENT_OBJECT_PROPERTY(IUndAtom*, Und, m_spUnd)
	IMPLEMENT_OBJECT_PROPERTY(IEtsOptAtom*, Opt, m_spOpt)
	IMPLEMENT_OBJECT_PROPERTY(IEtsOptRootAtom*, OptRoot, m_spOptRoot)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmOrderExecColl*, Execs, m_spExecs)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmTradeInfoColl*, Trades, m_spTrades)
	IMPLEMENT_BSTRT_PROPERTY(RequestUID, m_bstrRequestUID)

	// helpers for additional info
	STDMETHOD(get_ContractType)(enum EtsContractTypeEnum* pRetVal);
	STDMETHOD(get_Symbol)(BSTR* pRetVal);
	STDMETHOD(get_UnderlyingID)(LONG* pRetVal);
	STDMETHOD(get_OptRootID)(LONG* pRetVal);
	STDMETHOD(get_ContractID)(LONG* pRetVal);

	STDMETHOD(get_OrderTypeString)(BSTR* pRetVal);
	STDMETHOD(get_OrderStatusString)(BSTR* pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmOrderAtom), CMmOrderAtom)

#endif //__MMORDERATOM_H