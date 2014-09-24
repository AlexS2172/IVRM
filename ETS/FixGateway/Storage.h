// Storage.h: interface for the COrderStorage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __STORAGE_H__
#define __STORAGE_H__
#include "FIXGGeneral.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace FXG
{

//////////////////////////////////////////////////////////////////////
class CFgOrder 
{
public:
	CFgOrder()
		: m_nId(0L) 
		, m_enConfirmedStatus(enMsOsRejected){}

	_bstr_t					m_strClOrdID;
	long					m_nId;
	MsOrderStatusEnum		m_enConfirmedStatus;
};

// object pointer
typedef boost::shared_ptr<CFgOrder>		CFgOrderPtr;
typedef std::map<_bstr_t, CFgOrderPtr>	CFgOrderColl;
typedef boost::shared_ptr<CFgOrderColl>	CFgOrderCollPtr;


class CFgOrderStorage
{
public:
	CFgOrderStorage();
	virtual ~CFgOrderStorage();
	
	void Init(FXGSM::CSmSettingsMgrPtr pSettingsMgr);

	void Clear();

	void Load();

    void Insert(IOrder* pOrder, bool bNewOrder = false);
    
    CFgOrderPtr Find(_bstr_t& ClOrdID);
    
	void UpdateStatus(IOrder* pOrder, bool bFromFix = false);
	
	void Update(_bstr_t& bsOldKey, IOrder* pOrder);
    
private:
	CLock					m_Lock;	
    CFgOrderCollPtr			m_collOrders;
	EgLib::CDBConnection	m_DB;
};

typedef std::map<_bstr_t, long>					CFgExecutionsColl;
typedef boost::shared_ptr<CFgExecutionsColl>	CFgExecutionsCollPtr;
//////////////////////////////////////////////////////////////////////
class CFgExecutionStorage
{
public:

	CFgExecutionStorage();
	virtual ~CFgExecutionStorage();

	void Init(FXGSM::CSmSettingsMgrPtr pSettingsMgr);

	void Load();

	void Clear();

	long Find(_bstr_t& ExecID);

	void Insert(IExecutionReport* pReport);
	DWORD InsertTrade(FX::FoExecutionReport* Report, ITradeUpdate* pTrade);
	DWORD ReProcessTrade(FX::FoExecutionReport* Report, long lSeqNum, ITradeUpdate* pTrade);
	HRESULT FillTradeUpdate(CClientRecordset& rs, FX::FoExecutionReport* Report, ITradeUpdate* pTrade);

	void Update(IExecutionReport* pReport);

private:
	CLock					m_Lock;	
	CFgExecutionsCollPtr	m_collExecutions;
	EgLib::CDBConnection	m_DB;
};

};
#endif // __STORAGE_H__
