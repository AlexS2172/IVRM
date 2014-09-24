// MmTradeInfoAtom.h : Declaration of the CMmTradeInfoAtom
#ifndef __MM_TRADE_INFO_ATOM_H__
#define __MM_TRADE_INFO_ATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"

#include "CommonSPtr.h"
#include "EtsOptAtom.h"
#include "EtsOptRootColl.h"
#include "EtsOptRootAtom.h"
#include "EtsFutRootAtom.h"
#include "EtsFutRootColl.h"
#include "EtsTraderAtom.h"
#include "EtsBrokerAtom.h"
#include "EtsStrategyAtom.h"
#include "EtsFilterData.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;

_COM_SMARTPTR_TYPEDEF(IMmTradeInfoAtom, IID_IMmTradeInfoAtom);

struct __MmTradeInfoAtom
{
	LONG						m_nTradeID;
	LONG						m_nSeqNum;
	DOUBLE						m_dPrice;
	LONG						m_nQuantity;
	VARIANT_BOOL				m_bIsBuy;
	DATE						m_dtTradeDate;
	DOUBLE						m_dSpotReference;
	DOUBLE						m_dTradedIV;
	VARIANT_BOOL				m_bIsPosition;
	EtsTradeStatusEnum			m_enStatus;
	DOUBLE						m_dBrokerCommission;
	DOUBLE						m_dClearingBrokerCommission;
	LONG						m_nMark;
	LONG						m_nOrderID;
	LONG						m_nExecStatus;
	DOUBLE						m_dManualActivePrice;

	IUndAtomPtr					m_spUnd;
	IEtsOptAtomPtr				m_spOpt;
	IEtsOptRootAtomPtr			m_spOptRoot;
	IEtsTraderAtomPtr			m_spTrader;
	IEtsBrokerAtomPtr			m_spBroker;
	IEtsBrokerAtomPtr			m_spClearingBroker;
	IEtsStrategyAtomPtr			m_spStrategy;
	IEtsFutRootAtomPtr			m_spFutRoot;
	IEtsFutAtomPtr				m_spFut;
	IEtsFutOptAtomPtr			m_spFutOpt;
	IEtsMainPtr					m_spMain;
	IContractPtr				m_spContract;

	__MmTradeInfoAtom() :
		m_nTradeID(0L), m_nSeqNum(0L), m_dPrice(0.), m_nQuantity(0L),
		m_bIsBuy(VARIANT_FALSE), m_dtTradeDate(0.), m_dSpotReference(0.), m_dTradedIV(0.),
		m_bIsPosition(VARIANT_FALSE), m_enStatus(enTrsRealtime),
		m_dBrokerCommission(0.), m_dClearingBrokerCommission(0.), m_nMark(0L), m_nOrderID(0L), m_nExecStatus(0L),
		m_dManualActivePrice(0.)
	{
		m_spStrategy = m_spTrader = NULL;
	}
};

// CMmTradeInfoAtom

class ATL_NO_VTABLE CMmTradeInfoAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmTradeInfoAtom, &CLSID_MmTradeInfoAtom>,
	public ISupportErrorInfoImpl<&IID_IMmTradeInfoAtom>,
	public IDispatchImpl<IMmTradeInfoAtom, &IID_IMmTradeInfoAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmTradeInfoAtom
{
public:
	CMmTradeInfoAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMTRADEINFOATOM)

BEGIN_COM_MAP(CMmTradeInfoAtom)
	COM_INTERFACE_ENTRY(IMmTradeInfoAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		m_spUnd = NULL;
		m_spOpt = NULL;
		m_spOptRoot = NULL;
		m_spTrader = NULL;
		m_spBroker = NULL;
		m_spClearingBroker = NULL;
		m_spStrategy = NULL;
		m_spFutRoot = NULL;
		m_spFut = NULL;
		m_spFutOpt = NULL;
		m_spMain = NULL;
		m_spContract = NULL;
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

	long  GetUndID()
	{
		long lUndID = 0;
		get_UndID(&lUndID);
		return lUndID;
	}
public:
	HRESULT Load(EtsContractTypeEnum enType, CClientRecordset& rc);

	bool GetField(TradesListColumnEnum field, _variant_t& vtField, bool bForCompare = false);

	_bstr_t    TradeTypeString()
	{
		_bstr_t bsRet;
		switch(m_enStatus)
		{
		case enTrsRealtime:
		case enTrsMatchedRealtime:
			bsRet = bsTradeTypeActual;
			break;
		case enTrsSimulated:
			bsRet = bsTradeTypeSimulated;
			break;
		case enTrsManual:
			bsRet = bsTradeTypeManual;
			break;
		default:
			bsRet = bsTradeTypeUnknown;
		}
		return bsRet;
	}

private:
	static _variant_t bsTradeTypeActual;
	static _variant_t bsTradeTypeSimulated;
	static _variant_t bsTradeTypeManual;
	static _variant_t bsTradeTypeUnknown;
	static _variant_t bsEmpty;
	static _variant_t bsNA;

private:
	_bstr_t m_bsUndSymbol;
	_bstr_t m_bsTlcSymbol;

	bool IsToday( bool bAdnSimulated );
	bool IsSimulated();
	bool IsOpenPos( bool bAdnSimulated );

public:
	IMPLEMENT_SIMPLE_PROPERTY(LONG, SeqNum, m_nSeqNum)

	IMPLEMENT_SIMPLE_PROPERTY(LONG, TradeID, m_nTradeID)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Price, m_dPrice)	
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Quantity, m_nQuantity)	
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsBuy, m_bIsBuy)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, TradeDate, m_dtTradeDate)
	
	IMPLEMENT_OBJECT_PROPERTY(IEtsTraderAtom*, Trader, m_spTrader)
	IMPLEMENT_OBJECT_PROPERTY(IEtsBrokerAtom*, Broker, m_spBroker)
	IMPLEMENT_OBJECT_PROPERTY(IEtsBrokerAtom*, ClearingBroker, m_spClearingBroker)
	IMPLEMENT_OBJECT_PROPERTY(IEtsStrategyAtom*, Strategy, m_spStrategy)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, BrokerCommission, m_dBrokerCommission)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ClearingBrokerCommission, m_dClearingBrokerCommission)

	IMPLEMENT_SIMPLE_PROPERTY(LONG, OrderID, m_nOrderID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ExecStatus, m_nExecStatus)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, SpotReference, m_dSpotReference)	
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TradedIV, m_dTradedIV)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsPosition, m_bIsPosition)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Mark, m_nMark)
	IMPLEMENT_SIMPLE_PROPERTY(EtsTradeStatusEnum, Status, m_enStatus)	


	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ManualActivePrice, m_dManualActivePrice)
	IMPLEMENT_OBJECT_PROPERTY(IUndAtom*, Und, m_spUnd)
	IMPLEMENT_OBJECT_PROPERTY(IEtsOptAtom*, Opt, m_spOpt)
	IMPLEMENT_OBJECT_PROPERTY(IEtsFutAtom*, Fut, m_spFut)
	IMPLEMENT_OBJECT_PROPERTY(IEtsFutOptAtom*, FutOpt, m_spFutOpt)
	IMPLEMENT_OBJECT_PROPERTY(IEtsOptRootAtom*, OptRoot, m_spOptRoot)
	IMPLEMENT_OBJECT_PROPERTY(IEtsFutRootAtom*, FutRoot, m_spFutRoot)
	IMPLEMENT_OBJECT_PROPERTY(IContract*, Contract, m_spContract)

	STDMETHOD(get_ContractID)(LONG* pRetVal);
	STDMETHOD(get_ContractType)(enum EtsContractTypeEnum* pRetVal);
	STDMETHOD(get_Symbol)(BSTR* pRetVal);
	STDMETHOD(get_UndID)(LONG* pRetVal);
	STDMETHOD(get_OptRootID)(LONG* pRetVal);
	STDMETHOD(get_FutRootID)(LONG* pRetVal);
	STDMETHOD(get_FutID)(LONG* pRetVal);
	STDMETHOD(get_LotSize)(LONG* pRetVal);
	STDMETHOD(get_TotalCommission)(DOUBLE* pRetVal);
	STDMETHOD(get_TotalPrice)(DOUBLE* pRetVal);
	STDMETHOD(get_StrategyID)(LONG* pRetVal);
	STDMETHOD(get_ContractName)(BSTR* pRetVal);
	STDMETHOD(get_BrokerID)(LONG* pRetVal);
	STDMETHOD(get_ClearingBrokerID)(LONG* pRetVal);
	STDMETHOD(get_ImportID)(BSTR* pRetVal);

	STDMETHOD(CopyTo)(IMmTradeInfoAtom* pDest);
    STDMETHOD(GetTradeTypeString)(BSTR* pDest);
    STDMETHOD(GetTradeMatchedString)(BSTR* pDest);
	STDMETHOD(get_ExecStatusString)(BSTR* pRetVal);
	STDMETHOD(CheckByFilter)(IEtsFilterData* pFilter, VARIANT_BOOL* pRetVal);
	STDMETHOD(GetField)(TradesListColumnEnum enField, VARIANT_BOOL Compare, VARIANT *pValue, VARIANT_BOOL* HasValue);
};

OBJECT_ENTRY_AUTO(__uuidof(MmTradeInfoAtom), CMmTradeInfoAtom)
typedef  std::vector<IMmTradeInfoAtomPtr> CTradeInfoViewData;

#endif //__MM_TRADE_INFO_ATOM_H__
