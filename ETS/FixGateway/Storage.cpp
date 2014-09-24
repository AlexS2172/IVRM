// Storage.cpp: implementation of the CFgExecutionStorage class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Storage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace FXG
{

CFgOrderStorage::CFgOrderStorage() 
{
}

//////////////////////////////////////////////////////////////////////
CFgOrderStorage::~CFgOrderStorage()
{
	m_DB.Close();
}

//////////////////////////////////////////////////////////////////////
void CFgOrderStorage::Init(FXGSM::CSmSettingsMgrPtr pSettingsMgr)
{
	if(!pSettingsMgr)
		throw exception(_T("Invalid settings manager object."));
	
	m_DB.Close();

	m_DB.Open(pSettingsMgr->DbConnection().GetString(), pSettingsMgr->DbConnectionTimeout(), pSettingsMgr->DbCommandTimeout());
	m_collOrders = CFgOrderCollPtr(new CFgOrderColl);
}

//////////////////////////////////////////////////////////////////////
void CFgOrderStorage::Clear()
{
	try
	{
		CStoredProc<> spOrderDel(m_DB, L"usp_FixOldOrder_Del");
		spOrderDel << CDBNull();
		spOrderDel.Execute();
	}
	catch (exception& e)
	{
		CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(e.what()));
	}
	CAutoLock lock(m_Lock);
	m_collOrders->clear();
}
//////////////////////////////////////////////////////////////////////
CFgOrderPtr CFgOrderStorage::Find(_bstr_t& ClOrdID)
{   
	CAutoLock lock(m_Lock);
	CFgOrderColl::const_iterator  iter = m_collOrders->find(ClOrdID);
    
	CFgOrderPtr spOrder;
    if (iter != m_collOrders->end())
		spOrder = iter->second;

	return spOrder;
}

//////////////////////////////////////////////////////////////////////
void CFgOrderStorage::Insert(IOrder* pOrder, bool bNewOrder)
{
	IOrderPtr spOrder(pOrder);

	if(bNewOrder)
	{
		// insert in DB
		try
		{
			CStoredProc<CClientRecordset> spOrderNew(m_DB, _T("usp_FixOrder_New"));

			spOrderNew << spOrder->GetClOrdID();
			if(enStOption == spOrder->GetSymbolType())
				spOrderNew << spOrder->GetOptionID();
			else
				spOrderNew << spOrder->GetUndID();
			
			spOrderNew << spOrder->GetTraderID();

			long nVal = spOrder->GetDestID();
			if(nVal > 0L)
				spOrderNew << nVal;
			else
				spOrderNew << CDBNull();

			spOrderNew << static_cast<long>(spOrder->GetOrderType());
			spOrderNew << static_cast<byte>(spOrder->GetIsBuy());
			spOrderNew << spOrder->GetQty();

			double dVal = spOrder->GetPrice();
			if(dVal > 0.)
				spOrderNew << dVal;
			else
				spOrderNew << CDBNull();

			spOrderNew.Open();

			long nRes = spOrderNew.GetResult();

			if(nRes >= 0L)
			{
				if(!spOrderNew.IsEOF())
				{						
					spOrder->PutId(spOrderNew[_T("iOrderID")]);
					spOrder->PutSeqNum(spOrderNew[_T("iSeqNum")]);
					spOrder->PutSendingTime(spOrderNew[_T("dtOrderDate")]);
				}
			}
		}
		catch (exception& e)
		{
			CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(e.what()));
		}
	}

	CFgOrderPtr spOrd(new CFgOrder);
	spOrd->m_strClOrdID = spOrder->GetClOrdID();
	spOrd->m_nId = spOrder->GetId();

	if(bNewOrder)
		spOrd->m_enConfirmedStatus = enMsOsRejected;
	else
		spOrd->m_enConfirmedStatus = spOrder->GetOrdStatus();

	CAutoLock lock(m_Lock);
	m_collOrders->insert(std::make_pair(spOrd->m_strClOrdID, spOrd));
}

void CFgOrderStorage::UpdateStatus(IOrder* pOrder, bool bFromFix)
{
	IOrderPtr spOrder(pOrder);

	// insert in DB
	long nOrderDbId = 0L;
	try
	{
		CStoredProc<CClientRecordset> spOrderUpdate(m_DB, _T("usp_FixOrderStatus_Update"));

		spOrderUpdate << spOrder->GetId();
		spOrderUpdate << static_cast<long>(spOrder->GetOrdStatus());

		spOrderUpdate.Open();

		long nRes = spOrderUpdate.GetResult();

		if(nRes >= 0L)
		{
			if(!spOrderUpdate.IsEOF())
			{						
				spOrder->PutSeqNum(spOrderUpdate[_T("iSeqNum")]);

				spOrder->PutUndID(spOrderUpdate[_T("iUndID")]);				
				spOrder->PutSymbol(spOrderUpdate[_T("vcSymbol")]);

				long nContractType = spOrderUpdate[_T("iContractType")];
				if(nContractType == 1L)
					spOrder->PutSymbolType(enStIndex);
				else if(nContractType == 2L)
					spOrder->PutSymbolType(enStStock);
				else if(nContractType == 3L)
				{
					spOrder->PutSymbolType(enStOption);
					spOrder->PutIsCall(static_cast<byte>(spOrderUpdate[_T("tiIsCall")]) != 0);
					spOrder->PutExpiry(spOrderUpdate[_T("dtExpiry")]);
					spOrder->PutStrike(spOrderUpdate[_T("fStrike")]);
					spOrder->PutOptionID(spOrderUpdate[_T("iOptionID")]);
					spOrder->PutOptionRootID(spOrderUpdate[_T("iOptionRootID")]);
					spOrder->PutOptionRootSymbol(spOrderUpdate[_T("vcOptionRootSymbol")]);
				}
				spOrder->PutTraderID(spOrderUpdate[_T("iTraderID")]);
				spOrder->PutAccount(spOrderUpdate[_T("vcAccount")]);
				spOrder->PutIsBuy(static_cast<byte>(spOrderUpdate[_T("tiIsBuy")]) != 0);
				spOrder->PutOrderType(static_cast<MsOrderTypeEnum>((long)spOrderUpdate[_T("iOrdType")]));
				spOrder->PutQty(spOrderUpdate[_T("iQuantity")]);
				spOrder->PutPrice(spOrderUpdate[_T("fPrice")]);
				spOrder->PutSendingTime(spOrderUpdate[_T("dtSendingTime")]);
				spOrder->PutTransactTime(spOrderUpdate[_T("dtTransactTime")]);
				spOrder->PutDestID(spOrderUpdate[_T("iDestID")]);				
				spOrder->PutExDestination(spOrderUpdate[_T("vcDestination")]);
				spOrder->PutAvgPrice(spOrderUpdate[_T("fAvgPrice")]);
				spOrder->PutCumQty(spOrderUpdate[_T("iCumQty")]);
			}
		}
	}
	catch (exception& e)
	{
		CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(e.what()));
	}

	if(bFromFix)
	{
		CAutoLock lock(m_Lock);
		CFgOrderColl::const_iterator  iter = m_collOrders->find(spOrder->GetClOrdID());
	
		if (iter != m_collOrders->end())
		{
			CFgOrderPtr spOrd(iter->second);
			spOrd->m_enConfirmedStatus = spOrder->GetOrdStatus();
		}	
	}
}

void CFgOrderStorage::Update(_bstr_t& bsOldKey, IOrder* pOrder)
{
	IOrderPtr spOrder(pOrder);

	// insert in DB
	try
	{
		CStoredProc<CClientRecordset> spOrderUpdate(m_DB, _T("usp_FixOrder_Update"));

		spOrderUpdate << spOrder->GetId();

		long nVal = static_cast<long>(spOrder->GetOrdStatus());
		if(nVal > 0L)
			spOrderUpdate << nVal;
		else
			spOrderUpdate << EgLib::CDBNull();

		_bstr_t bsVal = spOrder->GetClOrdID();
		if(bsVal.length())
			spOrderUpdate << bsVal;
		else
			spOrderUpdate << CDBNull();

		nVal = static_cast<long>(spOrder->GetOrderType());
		if(nVal > 0L)
			spOrderUpdate << nVal;
		else
			spOrderUpdate << CDBNull();

		nVal = spOrder->GetQty();
		if(nVal > 0L)
			spOrderUpdate << nVal;
		else
			spOrderUpdate << CDBNull();

		double dVal = spOrder->GetPrice();
		if(dVal > 0.)
			spOrderUpdate << dVal;
		else
			spOrderUpdate << CDBNull();

		spOrderUpdate << vt_date(spOrder->GetTransactTime());

		spOrderUpdate.Open();

		long nRes = spOrderUpdate.GetResult();

		if(nRes >= 0L)
		{
			if(!spOrderUpdate.IsEOF())
			{						
				spOrder->PutSeqNum(spOrderUpdate[_T("iSeqNum")]);

				spOrder->PutUndID(spOrderUpdate[_T("iUndID")]);				
				spOrder->PutSymbol(spOrderUpdate[_T("vcSymbol")]);

				long nContractType = spOrderUpdate[_T("iContractType")];
				if(nContractType == 1L)
					spOrder->PutSymbolType(enStIndex);
				else if(nContractType == 2L)
					spOrder->PutSymbolType(enStStock);
				else if(nContractType == 3L)
				{
					spOrder->PutSymbolType(enStOption);
					spOrder->PutIsCall(static_cast<byte>(spOrderUpdate[_T("tiIsCall")]) != 0);
					spOrder->PutExpiry(spOrderUpdate[_T("dtExpiry")]);
					spOrder->PutStrike(spOrderUpdate[_T("fStrike")]);
					spOrder->PutOptionID(spOrderUpdate[_T("iOptionID")]);
					spOrder->PutOptionRootID(spOrderUpdate[_T("iOptionRootID")]);
					spOrder->PutOptionRootSymbol(spOrderUpdate[_T("vcOptionRootSymbol")]);
				}
				spOrder->PutTraderID(spOrderUpdate[_T("iTraderID")]);
				spOrder->PutAccount(spOrderUpdate[_T("vcAccount")]);
				spOrder->PutIsBuy(static_cast<byte>(spOrderUpdate[_T("tiIsBuy")]) != 0);
				spOrder->PutOrderType(static_cast<MsOrderTypeEnum>((long)spOrderUpdate[_T("iOrdType")]));
				spOrder->PutQty(spOrderUpdate[_T("iQuantity")]);
				spOrder->PutPrice(spOrderUpdate[_T("fPrice")]);
				spOrder->PutSendingTime(spOrderUpdate[_T("dtSendingTime")]);
				spOrder->PutTransactTime(spOrderUpdate[_T("dtTransactTime")]);
				spOrder->PutDestID(spOrderUpdate[_T("iDestID")]);				
				spOrder->PutExDestination(spOrderUpdate[_T("vcDestination")]);
				spOrder->PutAvgPrice(spOrderUpdate[_T("fAvgPrice")]);
				spOrder->PutCumQty(spOrderUpdate[_T("iCumQty")]);
			}
		}
	}
	catch (exception& e)
	{
		CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(e.what()));
	}

	if( !(!bsOldKey) && spOrder->GetClOrdID() != bsOldKey)
	{
		CFgOrderPtr spOrd(new CFgOrder);
		spOrd->m_strClOrdID = spOrder->GetClOrdID();
		spOrd->m_nId = spOrder->GetId();
		spOrd->m_enConfirmedStatus = spOrder->GetOrdStatus();

		CAutoLock lock(m_Lock);

		m_collOrders->erase(bsOldKey);
		m_collOrders->insert(std::make_pair(spOrd->m_strClOrdID, spOrd));
	}
}
//////////////////////////////////////////////////////////////////////
void CFgOrderStorage::Load()
{   
	try
	{
		CStoredProc<CClientRecordset> rsOrder(m_DB, _T("usp_FixOrder_Get"));

		rsOrder.Open();
		long nRes = rsOrder.GetResult();

		if(!nRes)
		{
			while(!rsOrder.IsEOF())
			{						
				CFgOrderPtr spOrd(new CFgOrder);
				spOrd->m_strClOrdID = rsOrder[_T("vcClOrderID")];
				spOrd->m_nId = rsOrder[_T("iOrderID")];
				spOrd->m_enConfirmedStatus = static_cast<MsOrderStatusEnum>((long)rsOrder[_T("iStatus")]);

				CAutoLock lock(m_Lock);
				m_collOrders->insert(std::make_pair(_bstr_t(rsOrder[_T("vcClOrderID")]), spOrd));
				++rsOrder;
			}
		}
	}
	catch (exception& e)
	{
		CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(e.what()));
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFgExecutionStorage::CFgExecutionStorage() 
{
}

//////////////////////////////////////////////////////////////////////
CFgExecutionStorage::~CFgExecutionStorage()
{
	m_DB.Close();
}

//////////////////////////////////////////////////////////////////////
void CFgExecutionStorage::Init(FXGSM::CSmSettingsMgrPtr pSettingsMgr)
{
	if(!pSettingsMgr)
		throw exception(_T("Invalid settings manager object."));

	m_DB.Close();

	m_DB.Open(pSettingsMgr->DbConnection().GetString(), pSettingsMgr->DbConnectionTimeout(), pSettingsMgr->DbCommandTimeout());

	m_collExecutions = CFgExecutionsCollPtr(new CFgExecutionsColl);
}

//////////////////////////////////////////////////////////////////////
void CFgExecutionStorage::Clear()
{
	CAutoLock lock(m_Lock);
	m_collExecutions->clear();
}
//////////////////////////////////////////////////////////////////////
void CFgExecutionStorage::Insert(IExecutionReport* pReport)
{
	IExecutionReportPtr spReport(pReport);

	// insert in DB
	long nExecDbId = 0L;
	try
	{
		CStoredProc<CClientRecordset> spExecNew(m_DB, _T("usp_FixOrderExec_New"));

		spExecNew << spReport->GetExecID();
		//spExecNew << spReport->GetOrderId();
		spExecNew << spReport->GetExecType();
		spExecNew << spReport->GetLastShares();
		spExecNew << spReport->GetLastPrice();
		spExecNew << COleDateTime(spReport->GetTransactTime());
		spExecNew << spReport->GetOrdStatus();
		
		double dVal = spReport->GetAvgPrice();
		if(dVal > 0.)
			spExecNew << dVal;
		else
			spExecNew << CDBNull();

		long nVal = spReport->GetCumQty();
		if(nVal > 0L)
			spExecNew << nVal;
		else
			spExecNew << CDBNull();
	
		spExecNew.Open();
		long nRes = spExecNew.GetResult();

		if(nRes >= 0L)
		{
			if(!spExecNew.IsEOF())
			{						
				nExecDbId = spExecNew[_T("iExecID")];
				spReport->PutId(nExecDbId);
				spReport->PutSeqNum(spExecNew[_T("iSeqNum")]);
				spReport->PutAvgPrice(spExecNew[_T("fAvgPrice")]);
			}
		}
	}
	catch (exception& e)
	{
		CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(e.what()));
	}

	CAutoLock lock(m_Lock);
	m_collExecutions->insert(std::make_pair(spReport->GetExecID(), spReport->GetId()));
}


//////////////////////////////////////////////////////////////////////
void CFgExecutionStorage::Update(IExecutionReport* pReport)
{
	IExecutionReportPtr spReport(pReport);

	// insert in DB
	try
	{
		CStoredProc<CClientRecordset> spExecUpd(m_DB, _T("usp_FixOrderExec_Update"));

		spExecUpd << spReport->GetId();
		spExecUpd << spReport->GetExecID();
		spExecUpd << spReport->GetExecType();

		long nVal = spReport->GetLastShares();
		if(nVal > 0L)
			spExecUpd << nVal;
		else
			spExecUpd << CDBNull();

		double dVal = spReport->GetLastPrice();
		if(dVal > 0.)
			spExecUpd << dVal;
		else
			spExecUpd << CDBNull();

		spExecUpd << COleDateTime(spReport->GetTransactTime());
		spExecUpd << spReport->GetOrdStatus();

		dVal = spReport->GetAvgPrice();
		if(dVal > 0.)
			spExecUpd << dVal;
		else
			spExecUpd << CDBNull();

		nVal = spReport->GetCumQty();
		if(nVal > 0L)
			spExecUpd << nVal;
		else
			spExecUpd << CDBNull();
	
		spExecUpd.Open();
		long nRes = spExecUpd.GetResult();

		if(nRes >= 0L)
		{
			if(!spExecUpd.IsEOF())
			{						
				spReport->PutSeqNum(spExecUpd[_T("iSeqNum")]);				
				spReport->PutAvgPrice(spExecUpd[_T("fAvgPrice")]);
			}
		}
	}
	catch (exception& e)
	{
		CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(e.what()));
	}

	CAutoLock lock(m_Lock);

	m_collExecutions->erase(spReport->GetExecRefID());
	m_collExecutions->insert(std::make_pair(spReport->GetExecID(), spReport->GetId()));
}

//////////////////////////////////////////////////////////////////////
long CFgExecutionStorage::Find(_bstr_t& ClOrdID)
{   
	CAutoLock lock(m_Lock);
	CFgExecutionsColl::const_iterator  iter = m_collExecutions->find(ClOrdID);
    
    if (iter != m_collExecutions->end())
		return iter->second;
    
    return 0L;
}

//////////////////////////////////////////////////////////////////////
void CFgExecutionStorage::Load()
{   
	try
	{
		CStoredProc<CClientRecordset> rsExec(m_DB, _T("usp_FixOrderExec_Get"));

		rsExec.Open();
		long nRes = rsExec.GetResult();

		if(!nRes)
		{
			while(!rsExec.IsEOF())
			{						
				CAutoLock lock(m_Lock);
				m_collExecutions->insert(std::make_pair<_bstr_t, long>(rsExec[_T("vcExecID")], rsExec[_T("iID")]));
				++rsExec;
			}
		}
	}
	catch (exception& e)
	{
		CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(e.what()));
	}
}

//////////////////////////////////////////////////////////////////////
DWORD CFgExecutionStorage::InsertTrade(FX::FoExecutionReport* Report, ITradeUpdate* pTrade)
{
	//DWORD	dwTries = MAX_DBRECONNECTION_TRIES;
	//bool	bOk = false;
	DWORD	dwRes = ERROR_SUCCESS;
	long    lMaxSeqNum;
	long    lMinSeqNum;
	long    lStructureMissed;
	CString strFailMsg;
	//TDBFailsStore::iterator it;

	//do
	//{
	//	if(m_pConnection==NULL)
	//	{
	//		Reconnect();
	//		if(m_pConnection==NULL)
	//		{
	//			if(!--dwTries)
	//			{
	//				FlushTrade(Report);
	//				BroadcastError(ERROR_REMOTE_STORAGE_NOT_ACTIVE, "Unable to open the database connection");
	//				dwRes = ERROR_REMOTE_STORAGE_NOT_ACTIVE;
	//				bOk = true;
	//				break;
	//			}
	//		}
	//	}
	//	else
	//	{
			try
			{
				switch(Report->ExecTransType)
				{
				case FX::enFoTrnNew:
					{                    
						CClientRecordset rs;
						CStoredProc<> sp(m_DB, L"usp_TradeFeed_1_New");

						sp << _bstr_t(Report->Account);
						sp << _bstr_t(Report->ExecID);
						sp << ((Report->Side == FX::enFoOsdBuy) ? (int)1 : int(0));
						sp << (int)Report->LastShares;
						sp << (float)Report->LastPrice;
						sp << vt_date(Report->TransactTime);
						sp << _bstr_t(Report->Symbol);

						if(Report->SecurityType != FX::enFoStOPT)
						{
							sp << (int)2;
							sp << CDBNull();
							sp << CDBNull();
							sp << CDBNull();
							sp << CDBNull();
							sp << CDBNull();
							sp << CDBNull();
							sp << CDBNull();
							sp << CDBNull();
						}
						else if (Report->SecurityType == FX::enFoStOPT)
						{
							sp << (int)3;
							if(SysStringLen(Report->UndSymbol))
								sp << _bstr_t(Report->UndSymbol);
							else
								sp << CDBNull(); // No underlying supplied

							sp << (Report->IsCall == VARIANT_TRUE);
							sp << (float)Report->StrikePrice;
							sp << vt_date(Report->ExpirationDate);
							sp << CDBNull();
							sp << CDBNull();
							sp << CDBNull();
							sp << _bstr_t(Report->Root);
						}
						else
						{
							CString strError;
							strError.Format(_T("Future Orders Executions is not supported. ExecID (%s)"),(LPCSTR)_bstr_t(Report->ExecID));

							CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(strError));
							dwRes = ERROR_INVALID_PARAMETER;
							return dwRes;

							//	sp << (int)4;
							//	if(Report->sUnderlying.length())
							//		sp << _bstr_t(Report->sUnderlying.c_str());
							//	else
							//		sp << CDBNull(); 
							//	sp << CDBNull();
							//	sp << CDBNull();

							//	DATE dtExpiryDate;
							//	OleDateFromTm(Report->fmyMaturityMonthYear.tm_year + 1900, 
							//		Report->fmyMaturityMonthYear.tm_mon + 1, 
							//		Report->fmdMaturityDay.tm_mday, 
							//		0, 0, 0, 
							//		dtExpiryDate);

							//	sp << vt_date(dtExpiryDate);
							//	sp << CDBNull();
							//	sp << CDBNull();
							//	sp << CDBNull();
							//	sp << _bstr_t(Report->sRoot.c_str());


						}

						sp << CDBNull();										//@iMark
						sp << CDBNull();										//@vcStrategyName
						if ( Report->StockRefPrice > 0.0  )						//@fStockRefPrice
							sp << Report->StockRefPrice ;
						else
							sp << CDBNull();									

						if(SysStringLen(Report->ClOrdID))						//@vcOrderID
							sp << _bstr_t(Report->ClOrdID);

						sp << ((MsOrderStatusEnum)Report->ExecType);			//@iExecStatus
						if(SysStringLen(Report->ExecBroker))					//@vcBrokerName
							sp << _bstr_t(Report->ExecBroker);
						else
							sp << CDBNull();


						rs.Open (sp);
						if(rs.IsEOF())
						{
							rs.Close();

							_com_issue_error( E_FAIL );
						}                        

						lMaxSeqNum = rs[L"iMaxSeqNum"];
						lMinSeqNum = rs[L"iMinSeqNum"];
						lStructureMissed = rs[L"iStructureMissed"];
						rs.Close();

						if ( lMaxSeqNum == -1 
							&& lMinSeqNum == -1 
							&& lStructureMissed == -1) // unknown error
							_com_issue_error( E_FAIL );
						else
							if ( lMaxSeqNum == -1 && lStructureMissed == -1 ) //trade already exists
							{
								dwRes = ERROR_ALREADY_EXISTS;                            
								if ( ReProcessTrade(Report, lMinSeqNum, pTrade) == ERROR_SUCCESS )
									dwRes = ERROR_SUCCESS;
								break;
							}
							
							//strFailMsg.Format(_T("Trade '%s' stored in database."), Report->ExecID);
							//CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(e.what()));

							//CTracer::Trace(_T("Trade '%s' stored in database."), Report->sExecID.c_str());
							//InterlockedIncrement((LPLONG)&m_dwDBStored);

							CStoredProc<> sp_get(m_DB, L"usp_TradeSeq_Get");
							sp_get << (int)lMaxSeqNum;
							sp_get << (int)lMaxSeqNum;
							sp_get << CDBNull();
							sp_get << (unsigned char)1;

							rs.Open (sp_get);

							if(rs.IsEOF())
							{
								strFailMsg.Format(_T("Failed to retrieve trade from database - trade with ExecID '%s' does not exist."),
									Report->ExecID);
								
								CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogInfo, _bstr_t(strFailMsg));
								dwRes = ERROR_NO_DATA_DETECTED;
							}
							else
							{
								HRESULT hRes;
								if(FAILED(hRes = FillTradeUpdate(rs, Report, pTrade)))
								{
									CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, L"Failed to fill TradeUpdate message.");
									dwRes = ERROR_INVALID_PARAMETER;
								}
								else if(hRes == S_FALSE)
								{
									strFailMsg.Format(_T("Failed to retrieve trade from database - trade with ExecID '%s' does not exist."),
										Report->ExecID);
									
									CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(strFailMsg));
									dwRes = ERROR_NO_DATA_DETECTED;
								}
							}

							break;
					}

				case FX::enFoTrnCancel:
					{
						CClientRecordset rs;
						CStoredProc<> sp(m_DB, L"usp_TradeFeed_1_Del");
						sp << _bstr_t(Report->ExecID);
						sp << vt_date(Report->TransactTime);

						rs.Open (sp);
						if(rs.IsEOF())
						{
							rs.Close();
							_com_issue_error( E_FAIL );
						}

						lMaxSeqNum = rs[L"iMaxSeqNum"];
						lMinSeqNum = rs[L"iMinSeqNum"];
						rs.Close();

						if (lMaxSeqNum == -1 
							&& lMinSeqNum == -1) // unknown error
							_com_issue_error(E_FAIL);
						else                    
							if (lMinSeqNum == -1) // already exists
							{                            
								dwRes = ERROR_ALREADY_EXISTS;
								if ( ReProcessTrade(Report, lMaxSeqNum, pTrade) == ERROR_SUCCESS )
									dwRes = ERROR_SUCCESS;
								break;
							}
							else
								if (lMaxSeqNum == -1)
								{
									strFailMsg.Format(_T("Failed to store trade in database - trade with the same ExecID '%s' either does not exist."),
										Report->ExecID);

									CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(strFailMsg));
									dwRes = ERROR_FILE_NOT_FOUND;
									break;
								}

								//CTracer::Trace(_T("Trade '%s' deleted from database."), Report->sExecID.c_str());
								//InterlockedIncrement((LPLONG)&m_dwDBDeleted);

								CStoredProc<> sp_get(m_DB, L"usp_TradeSeq_Get");
								sp_get << (int)lMaxSeqNum;
								sp_get << (int)lMaxSeqNum;
								sp_get << CDBNull();
								sp_get << (unsigned char)1;

								rs.Open (sp_get);

								if(rs.IsEOF())
								{
									strFailMsg.Format(_T("Failed to retrieve trade from database - trade with ExecID '%s' does not exist."),
										Report->ExecID);

									CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogInfo, _bstr_t(strFailMsg));
									dwRes = ERROR_NO_DATA_DETECTED;
								}
								else
								{
									HRESULT hRes;
									if(FAILED(hRes = FillTradeUpdate(rs, Report, pTrade)))
									{
										CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, L"Failed to fill TradeUpdate message.");
										dwRes = ERROR_INVALID_PARAMETER;
									}
									else if(hRes == S_FALSE)
									{
										strFailMsg.Format(_T("Failed to retrieve trade from database - trade with ExecID '%s' does not exist."),
											Report->ExecID);

										CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(strFailMsg));
										dwRes = ERROR_NO_DATA_DETECTED;
									}                        
								}

								break;
					}

				default:
					dwRes = ERROR_INVALID_PARAMETER;
					break;
				}

				//bOk = true;
			}
			catch(_com_error& e)
			{
				CString strError;
				strError.Format(_T("Failed to store trade in database. Error: %X  %s"), e.Error(), (LPCTSTR)e.Description());

				CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(strError));
				dwRes = ERROR_INVALID_PARAMETER;

				//CTracer::Trace(strError.GetString());

				//pair<TDBFailsStore::iterator, bool> pairResult = 
				//	m_DBFails.insert(Report->sExecID);

				//if(--dwTries)
				//{				
				//	Reconnect();
				//}
				//else
				//{
				//	CTracer::Trace(_T("CRITICAL ERROR : FAILED TO STORE TRADE IN DATABASE."));

				//	if(/*Uncomment this Oleg StoreInFile(Trade) != 0*/false)
				//	{
				//		CTracer::Trace(_T("CRITICAL ERROR : FAILED TO STORE TRADE IN FILE."));
				//		CTracer::Trace(_T("MANUAL INTERVENTION IS REQUIRED. THE APPLICATION IS STOPPED."));
				//		ExitProcess(-1);
				//	}
				//	else
				//	{
				//		FlushTrade(Report);
				//		BroadcastError(e.Error(), strError.GetString());
				//		dwRes = ERROR_REMOTE_STORAGE_NOT_ACTIVE;
				//		bOk = true;
				//	}
				//}
			}
			catch (exception& ex)
			{
				CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(ex.what()));
			}
	//	}
	//} while(!bOk);
	return dwRes;
}

DWORD CFgExecutionStorage::ReProcessTrade(FX::FoExecutionReport* Report, long lSeqNum, ITradeUpdate* pTrade)
{
	DWORD dwRes = ERROR_SUCCESS;
	_bstr_t sbsFailMsg = L"Failed to reprocess trade with ExecID = '";
	sbsFailMsg += Report->ExecID;
	sbsFailMsg += L"' : ";

	CClientRecordset rs;
	CStoredProc<> sp_get(m_DB, L"usp_TradeSeq_Get");
	try
	{        
		sp_get << (int)lSeqNum;
		sp_get << (int)lSeqNum;
		sp_get << CDBNull();
		sp_get << (unsigned char)1;        

		rs.Open (sp_get);

		if(rs.IsEOF())
		{
			rs.Close();
			_com_issue_error( E_FAIL );
		}             
	}
	catch (_com_error& e)
	{ 
		CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, sbsFailMsg + e.Description());
		//CTracer::Trace(_T("Failed to reprocess trade with ExecID = '%s' : %s"), Report->sExecID.c_str(), (char*)e.Description() );
		dwRes = ERROR_DATABASE_FAILURE;
	}

	HRESULT hRes = FillTradeUpdate(rs, Report, pTrade);

	if( FAILED(hRes) )
	{	 
		CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, sbsFailMsg + L"unable to fill TradeUpdate message.");
		//CTracer::Trace(_T("Failed to reprocess trade with ExecID = '%s' : unable to fill TradeUpdate message"), Report->sExecID.c_str() );
		dwRes = ERROR_INVALID_PARAMETER;
	}
	else if(hRes == S_FALSE)
	{
		CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, sbsFailMsg + L"unable to retrieve trade from database.");
		//CTracer::Trace(_T("Failed to reprocess trade with ExecID = '%s' : unable to retrieve trade from database."), Report->sExecID.c_str());
		dwRes = ERROR_NO_DATA_DETECTED;
	}                   

	return dwRes;
}

HRESULT CFgExecutionStorage::FillTradeUpdate(CClientRecordset& rs, FX::FoExecutionReport* Report, ITradeUpdate* pTrade)
{
	CString strFailMsg;

	try
	{
		pTrade->SeqNum = rs[L"iSeqNum"];
		if(pTrade->SeqNum == 0)
		{
			CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, L"Duplicated trade.");
			//CTracer::Trace("Duplicated trade");
			_com_issue_error(S_FALSE);
		}

		pTrade->TradeID = rs[L"iTradeID"];
		pTrade->Action = (TRADE_ACTION)(int)rs[L"tiActionID"];
		pTrade->TradeStatus = (TRADE_STATUS)(long)rs[L"tiStatus"];
		pTrade->Mark = rs[L"iMark"];
		pTrade->TradeDate = rs[L"dtTradeDate"];
		pTrade->TraderID = rs[L"iTraderID"];
		pTrade->SymbolID = rs[L"iContractID"];
		pTrade->Quantity = rs[L"iQuantity"];
		pTrade->Price = rs[L"fPrice"];
		pTrade->IsBuy = rs[L"tiIsBuy"];
		pTrade->StrategyID = rs[L"iStrategyID"];
		pTrade->BrokerID = rs[L"iBrokerID"];
		pTrade->BrokerCommission = rs[L"fBrokerCommission"];
		pTrade->ClearingBrokerID = rs[L"iClearingBrokerID"];
		pTrade->ClearingBrokerCommission = rs[L"fClearingBrokerCommission"];
		pTrade->SpotReference = rs[L"fSpotReference"];
		pTrade->TradedIV = rs[L"fTradedIV"];

		int iContractTypeID = rs[L"iContractTypeID"];
		switch(iContractTypeID)
		{
		case 1: 
			pTrade->SymbolType = enStIndex; 
			break;

		case 2: 
			pTrade->SymbolType = enStStock; 
			break;

		case 3: 
			pTrade->SymbolType = enStOption; 
			break;
		
		// /* Futures are not supported yet */
		//case 4: 
		//	pTrade->SymbolType = enStFuture; 
		//	break;

		//case 5: 
		//	pTrade->SymbolType = enStFutureOption; 
		//	break;

		default:
			{
				strFailMsg.Format(_T("Error. Unknown SymbolType for ContractTypeID = %d."), iContractTypeID);
				CFixGServer::s_pFixGServer->Fire_OnLogMessage(COleDateTime::GetCurrentTime(), enFgLogError, _bstr_t(strFailMsg));
				//CTracer::Trace(_T("Error. Unknown SymbolType for ContractTypeID = %d."), iContractTypeID);
				_com_issue_error(E_FAIL);
				break;
			}
		}

		pTrade->Symbol = rs[L"vcSymbol"];
		pTrade->UndSymbolID = rs[L"iUnderlyingContractID"];
		pTrade->UndSymbol = rs[L"vcUnderlyingSymbol"];
		pTrade->Expiry = rs[L"dtExpiry"];
		pTrade->IsCall = rs[L"tiIsCall"];
		pTrade->Strike = rs[L"fStrike"];
		pTrade->PriceClose = rs[L"fPriceClose"];
		pTrade->PriceTheoClose = rs[L"fPriceTheoClose"];
		pTrade->LotSize = rs[L"iLotSize"];
		pTrade->OptionRootID = rs[L"iOptionRootID"];
		pTrade->FutID =  rs[L"iFutureID"];
		pTrade->FutRootID =  rs[L"iFutureRootID"];
		pTrade->OrderID = rs[L"iOrderID"];
		pTrade->ExecStatus = rs[L"iExecStatus"];

		if(Report->ExecTransType == FX::enFoTrnNew)
		{
			long lStructureMissed = rs[L"iStructureMissed"];
			pTrade->MsState = (TRADE_MS_STATE)lStructureMissed;

			if(lStructureMissed)
			{
				_TCHAR szBuf[1024];

				switch(lStructureMissed)
				{
				case enTmssNewStock:
					_sntprintf_s(szBuf,1024, _T("New stock '%s' added.\n"), Report->UndSymbol);
					break;

				case enTmssNewOption:
					_sntprintf_s(szBuf,1024, _T("New option(future) '%s' for stock '%s' added.\n"),
						Report->Symbol, Report->UndSymbol);
					break;

				case enTmssNewStock | enTmssNewOption:
					_sntprintf_s(szBuf,1024, _T("New stock '%s' and new option '%s' for this stock are added.\n"),
						Report->UndSymbol, Report->Symbol);
					break;

				default:
					_tcscpy_s(szBuf, _T("Unexpected structure error."));
				}

				SYSTEMTIME st;
				::GetLocalTime(&st);
				_TCHAR szTime[32];
				_sntprintf_s(szTime, 32, _T("%d/%d/%d-%d:%d:%d "), st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

				//m_fStructFile << T2A(szTime) << T2A(szBuf) << flush;
			}
		}
	}
	catch(_com_error& e)
	{
		//CTracer::TraceError(e.Error(), (LPCSTR)e.Description());
		return e.Error();
	}

	return S_OK;
}

};