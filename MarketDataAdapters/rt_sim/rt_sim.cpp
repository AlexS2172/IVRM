// rt_Sim.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <Process.h>
#include <time.h>
#include "rt_Sim.h"
#include "rt_Structures.h"

CRt_Sim g_RT;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

// The Simpson's Deviation
int GetRandomValue2(int startPoint, int endPoint)
{
	int z = startPoint + rand()%(endPoint-startPoint+1);
	if(z>endPoint) z = endPoint;
	if(z<startPoint) z = startPoint;
	return z;
}

int GetRandomValue(long startPoint, long endPoint)
{
	static bool bInited=false;
	if(!bInited)
		srand(time(NULL));
	bInited = true;

	int ret = startPoint;
	if(endPoint>startPoint)
	{
		int sPoint = startPoint /2;
		int ePoint = endPoint /2;

		int z = sPoint + rand()%(ePoint-sPoint+1);
		int y = sPoint + rand()%(ePoint-sPoint+1);
		
		ret = z+y;
		if(ret >= endPoint  || ret < startPoint)
			ret = (endPoint - startPoint)/2;
	}	
	return ret;
}
BOOL APIENTRY DllMain( HINSTANCE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			_Module.Init(ObjectMap, hModule, &GUID_NULL);
			DisableThreadLibraryCalls(hModule);
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
	        _Module.Term();
			break;
    }
    return TRUE;
}



RT_ERR CRt_Sim::Connect(RT_CONNECT_ID*  pID, RT_STARTUP_CLBK pFunction)
{
	if(!pFunction || !pID) return RT_ERR_CONNECTION_NOT_OPENED;
                                          //        RT_ERR_OK                    
                                          //        RT_ERR_CONNECTION_NOT_OPENED 
                                          //        RT_ERR_NOT_ENTITLED          

	ResetEvent(m_hEventStop);
	unsigned ID = 0;
	_beginthreadex(NULL,0,SymbolResponce,this,0,&ID);
	_beginthreadex(NULL,0,GroupResponce,this,0,&ID);

	_beginthreadex(NULL,0,SymbolGet,this,0,&ID);
	_beginthreadex(NULL,0,SymbolMind,this,0,&ID);
	_beginthreadex(NULL,0,OptionMind,this,0,&ID);
	_beginthreadex(NULL,0,PlaybackMind,this,0,&ID);

	Put_StartUp(pFunction);
	m_bConnected = true; 
	*pID = m_ID;
	return RT_ERR_OK;
}

RT_ERR CRt_Sim::SetShutdownNotifier(RT_CONNECT_ID rt_id, RT_SHUTDOWN_CLBK shutdown_clbk)
{
	RT_ERR ret = RT_ERR_OK;

                                          //        RT_ERR_OK                    
                                          //        RT_ERR_CONNECTION_INVALID    

	Put_ShutDown(shutdown_clbk);
	m_bConnected = false; 
	return ret;
}

RT_ERR CRt_Sim::Disconnect(RT_CONNECT_ID id)
{
	RT_ERR ret = RT_ERR_OK;
	if(!m_bConnected) return RT_ERR_CONNECTION_NOT_OPEN;

                                 //        RT_ERR_OK                          
                                 //        RT_ERR_CONNECTION_INVALID          
                                 //        RT_ERR_CONNECTION_NOT_OPEN         
	SetEvent(m_hEventStop);
	return ret;
}

RT_ERR CRt_Sim::RequestSymbol(RT_CONNECT_ID	rt_id,
							RT_REQ_ID*	    req_id_ptr_to_fill,
							void*			user_data,
							RT_REQ_TYPE		req_type,
							RT_SYMBOL*		pSymbol,
							int             field_count,
							RT_FIELD*       fields,
					RT_GET_FIELDS_CLBK    callback)
{
	RT_ERR ret = RT_ERR_CONNECTION_INVALID;
	if(!m_bConnected) return RT_ERR_CONNECTION_NOT_OPEN;

	
	*req_id_ptr_to_fill = (RT_REQ_ID)InterlockedIncrement(&m_nRequestCount);
	CRequest req;
	if(req.Initialize((long)*req_id_ptr_to_fill, req_type, user_data, pSymbol, field_count, fields, callback))
	{
		Put_GetFields(callback);
		if(RT_REQ_TYPE_GET & req_type)
		{
			req.m_Type = RT_REQ_TYPE_GET;
		    m_GetRequest.push(req);
			SetEvent(m_hEventGetSymbol);
			ret = RT_ERR_OK;
		}
		if(RT_REQ_TYPE_MIND & req_type)
		{
			req.m_Type = RT_REQ_TYPE_MIND;

			if(!InitializeParameters())
				return RT_ERR_CONNECTION_INVALID;

			try
			{
				CDBConnection db(m_spOptions->ConnectionString);
				db.Open();

				req.m_bsSymbolName = pSymbol->name;
				req.m_bsSymbolName +=_T("_");
				req.m_bsSymbolName +=pSymbol->exchange;
				
				if(!pSymbol->u.dummy.dummy)
				{
// Stock
					long lID = 0;
					SYMBOLMAP::iterator itMap = m_SymbolID.find(req.m_bsSymbolName);
					if(itMap==m_SymbolID.end())
					{
						// New Symbol
						CSQLQuery<> cmd(db,L"SELECT * FROM Contract WHERE (contractTypeID = 2 OR contractTypeID = 1) AND symbolName = ?");
						cmd << _bstr_t(pSymbol->name);
						CClientRecordset recSet;
						recSet.Open(cmd);
						CSymbol _symbol;
						_symbol.m_szExchange = pSymbol->exchange;
						if(FillSymbol(_symbol, recSet))
						{
							lID = ++m_lSymbolCount;
							m_SymbolID[req.m_bsSymbolName] = lID;
							m_Symbols[lID] = _symbol;
						}
					}
					else
					{
						// Existing symbol
						lID = itMap->second;
					}
					if(!lID)
						return RT_ERR_SYMBOL_NOT_FOUND;

					req.m_Handle = lID;
					SYMBOLS::iterator iterSymb = m_Symbols.find(lID);
					if(iterSymb != m_Symbols.end()) 
					{
						if(!iterSymb->second.m_bSubscribed)
						{
							EgLib::CAutoLock sd(m_csMind);
							iterSymb->second.m_bSubscribed = true;
							m_MindStockRequest.insert(m_MindStockRequest.end(),req);
							SetEvent(m_hEventMindStock);
							SetEvent(m_hEventMindPBStock);
						}
						else
						{

							for(MINDREQ::iterator it = m_MindStockRequest.begin(); it != m_MindStockRequest.end(); it++)
							{
								CRequest& r = *it;
								CSymbol& symb = iterSymb->second;
								_bstr_t bsSymbol2(symb.m_szName);
								bsSymbol2 +=_T("_");
								bsSymbol2 +=symb.m_szExchange;

								if(r.m_bsSymbolName == bsSymbol2 )
								{
									it->AddUserData(*req_id_ptr_to_fill, user_data);
									SetEvent(m_hEventMindStock);
									SetEvent(m_hEventMindPBStock);
									break;
								}
							}
						}
						ret = RT_ERR_OK;
					}
					else
						ret =  RT_ERR_INVALID_HANDLE;
						
				}
				else
				{
// Option
					long lID = 0;
					SYMBOLMAP::iterator itMap = m_SymbolID.find(req.m_bsSymbolName);
					if(itMap==m_SymbolID.end())
					{
						// New Option
						_bstr_t bstrCmd =
							L"SELECT [Option].[OptionID] AS iOptionID, [Option].[UnderlyingID] AS iUnderlyingID, [isCall] AS bIsCall, [strike] AS fStrike, [expDate] AS dtExpDate, [Contract].[symbolName] AS vcSymbolName, [exchangeID] AS iExchangeID, [Contract].[description] AS vcDescription, [Contract].[lotSize] AS iLotSize, [priceMid] AS fPriceMid ,[IV] AS fIV ,[dateUpdate] AS dtDate FROM [Option] INNER JOIN Contract ON [Option].[OptionID]=[Contract].[contractID]"
							L"	WHERE [Contract].[SymbolName]= ? And [Contract].contractTypeID = 3 And [Option].[expDate]>= now()";
							CSQLQuery<> cmd(db, bstrCmd);

							cmd << _bstr_t(pSymbol->name);
							CClientRecordset recSet;
							recSet.Open(cmd);

							long iID = recSet[L"iOptionID"];

							CSymbol symbol;

							symbol.m_lID = recSet[L"iOptionID"];
							symbol.m_lParentID =recSet[L"iUnderlyingID"];
							symbol.m_enType = ((long)recSet[L"bIsCall"])==1?CSymbol::enOptCall : CSymbol::enOptPut;
//							symbol.m_type = (long)recSet[L"bIsCall"];
							symbol.m_Strike = recSet[L"fStrike"];
							symbol.m_dtExpDate = (vt_date)recSet[L"dtExpDate"];
							symbol.m_szName = recSet[L"vcSymbolName"];
							symbol.m_lMarketID =  recSet[L"iExchangeID"];
							symbol.m_szDescription = recSet[L"vcDescription"];
							symbol.m_dPrice = recSet[L"fPriceMid"];
							symbol.m_dOpenPrice =  symbol.m_dPrice;


							symbol.m_dClose = symbol.m_dOpenPrice * RandomPercent( m_spOptions->PricePercent);

							symbol.m_dtLastOperation = (vt_date)recSet[L"dtDate"];

							symbol.m_szExchange = pSymbol->exchange;

							lID = ++m_lSymbolCount;
							m_SymbolID[req.m_bsSymbolName] = lID;
							m_Symbols[lID] = symbol;
					}
					else
					{
						_bstr_t bstrCmd =
							L"SELECT [Option].[OptionID] AS iOptionID, [Option].[UnderlyingID] AS iUnderlyingID, [isCall] AS bIsCall, [strike] AS fStrike, [expDate] AS dtExpDate, [Contract].[symbolName] AS vcSymbolName, [exchangeID] AS iExchangeID, [Contract].[description] AS vcDescription, [Contract].[lotSize] AS iLotSize, [priceMid] AS fPriceMid ,[IV] AS fIV ,[dateUpdate] AS dtDate FROM [Option] INNER JOIN Contract ON [Option].[OptionID]=[Contract].[contractID]"
							L"	WHERE [Contract].[SymbolName]= ? And [Contract].contractTypeID = 3 And [Option].[expDate]>= now()";
						CSQLQuery<> cmd(db, bstrCmd);

						cmd << _bstr_t(pSymbol->name);
						CClientRecordset recSet;
						recSet.Open(cmd);

						lID = itMap->second;
						SYMBOLS::iterator iter = m_Symbols.find(lID);
						iter->second.m_lParentID = req.m_Handle;
						iter->second.m_Strike = recSet[L"fStrike"];
						iter->second.m_dtExpDate = (vt_date)recSet[L"dtExpDate"];
					}

					req.m_Handle = lID;
					if(!lID)
						return RT_ERR_INVALID_HANDLE;

					SYMBOLS::iterator iterSymb = m_Symbols.find(req.m_Handle);
					if(!iterSymb->second.m_bSubscribed)
					{
						EgLib::CAutoLock sd(m_csMind);
						iterSymb->second.m_bSubscribed = true;
						m_MindOptionRequest.insert(m_MindOptionRequest.end(),req);
						SetEvent(m_hEventMindOption);
						SetEvent(m_hEventMindPBOption);
					}
					else
					{

						for(MINDREQ::iterator it = m_MindOptionRequest.begin(); it != m_MindOptionRequest.end(); it++)
						{
							CRequest& r = *it;
							CSymbol& symb = iterSymb->second;
							_bstr_t bsSymbol2(symb.m_szName);
							bsSymbol2 +=_T("_");
							bsSymbol2 +=symb.m_szExchange;

							if(r.m_bsSymbolName == bsSymbol2 )
							{
								it->AddUserData(*req_id_ptr_to_fill, user_data);
								SetEvent(m_hEventMindOption);
								SetEvent(m_hEventMindPBOption);
								break;
							}
						}
					}

					ret = RT_ERR_OK;
				}
			}
			catch(...)
			{
				ret = RT_ERR_CONNECTION_INVALID;
			}
			if(double(m_spOptions->PlayBackDate)<0.05)
			{
				// Simulation
				ResetEvent(m_hEventPlayBackUnBlock);
				SetEvent(m_hEventMindUnBlock);
			}
			else
			{
				ResetEvent(m_hEventMindUnBlock);
				SetEvent(m_hEventPlayBackUnBlock);
			}

		}
	}
	return ret;
}

RT_ERR CRt_Sim::RequestHandle(RT_CONNECT_ID	rt_id,
					RT_REQ_ID*	    req_id_ptr_to_fill,
					void*			user_data,
					RT_REQ_TYPE		req_type,
					RT_HANDLE		handle,
					int             field_count,
					RT_FIELD*       fields,
			  RT_GET_FIELDS_CLBK    callback)
{
	RT_ERR ret = RT_ERR_CONNECTION_INVALID;
	if(!m_bConnected) return RT_ERR_CONNECTION_NOT_OPEN;
	
	*req_id_ptr_to_fill = (RT_REQ_ID)InterlockedIncrement(&m_nRequestCount);
	CRequest req;
	if(req.Initialize((long)*req_id_ptr_to_fill, req_type, user_data,handle,field_count,fields,callback))
	{
		Put_GetFields(callback);
		if(RT_REQ_TYPE_GET & req_type)
		{
			SYMBOLS::iterator iter = m_Symbols.find(req.m_Handle);
			if(iter == m_Symbols.end()) return RT_ERR_INVALID_HANDLE;

			req.m_Type = RT_REQ_TYPE_GET;
			for(SYMBOLMAP::iterator itSymbol = m_SymbolID.begin(); itSymbol != m_SymbolID.end(); itSymbol++)
			{
				if(itSymbol->second == handle)
				{
					req.m_bsSymbolName = itSymbol->first;
					break;
				}
			}
		    m_GetRequest.push(req);
			SetEvent(m_hEventGetSymbol);
			ret = RT_ERR_OK;
		}
		if(RT_REQ_TYPE_MIND & req_type)
		{
			req.m_Type = RT_REQ_TYPE_MIND;
			SYMBOLS::iterator iter = m_Symbols.find(req.m_Handle);
			if(iter == m_Symbols.end()) return RT_ERR_INVALID_HANDLE;
			for(SYMBOLMAP::iterator itSymbol = m_SymbolID.begin(); itSymbol != m_SymbolID.end(); itSymbol++)
			{
				if(itSymbol->second == handle)
				{
					req.m_bsSymbolName = itSymbol->first;
					break;
				}
			}

			if(!iter->second.m_bSubscribed)
			{
				iter->second.m_bSubscribed = true;
				EgLib::CAutoLock sd(m_csMind);
				if(iter->second.m_enType == CSymbol::enIndex || iter->second.m_enType == CSymbol::enStock)
				{
					m_MindStockRequest.insert(m_MindStockRequest.end(),req);
					SetEvent(m_hEventMindStock);
					SetEvent(m_hEventMindPBStock);
				}
				else
					if(iter->second.m_enType == CSymbol::enOptCall || iter->second.m_enType == CSymbol::enOptPut)
					{

//						req.m_bsSymbolName = m_SymbolID[];
						m_MindOptionRequest.insert(m_MindOptionRequest.end(),req);
						SetEvent(m_hEventMindOption);
						SetEvent(m_hEventMindPBOption);

					}
			}
			else
			{
				EgLib::CAutoLock sd(m_csMind);
				if(iter->second.m_enType == CSymbol::enIndex || iter->second.m_enType == CSymbol::enStock)
				{
					for(MINDREQ::iterator it = m_MindStockRequest.begin(); it != m_MindStockRequest.end(); it++)
					{
						CRequest& r = *it;
						CSymbol& symb = iter->second;

						_bstr_t bsSymbol2(symb.m_szName);
						bsSymbol2 +=_T("_");
						bsSymbol2 +=symb.m_szExchange;

						if(r.m_bsSymbolName == bsSymbol2 )
						{
							if(!it->AddUserData(*req_id_ptr_to_fill, user_data))
							{
								ret = RT_ERR_INVALID_REQ_ID;
								return ret;
							}
							else
							{
								SetEvent(m_hEventMindStock);
								SetEvent(m_hEventMindPBStock);
							}
							break;
						}
					}
				}
				else
					if(iter->second.m_enType == CSymbol::enOptCall || iter->second.m_enType == CSymbol::enOptPut)
					{
						for(MINDREQ::iterator it = m_MindOptionRequest.begin(); it != m_MindOptionRequest.end(); it++)
						{
							CRequest& r = *it;
							CSymbol& symb = iter->second;

							_bstr_t bsSymbol2(symb.m_szName);
							bsSymbol2 +=_T("_");
							bsSymbol2 +=symb.m_szExchange;

							if(r.m_bsSymbolName == bsSymbol2 )
							{
								if(!it->AddUserData(*req_id_ptr_to_fill, user_data))
								{
									ret = RT_ERR_INVALID_REQ_ID;
									return ret;
								}
								else
								{
									SetEvent(m_hEventMindOption);
									SetEvent(m_hEventMindPBOption);
								}
								break;
							}
						}
					}
				
			}
			if(double(m_spOptions->PlayBackDate)<0.05)
			{
				// Simulation
				ResetEvent(m_hEventPlayBackUnBlock);
				SetEvent(m_hEventMindUnBlock);
			}
			else
			{
				ResetEvent(m_hEventMindUnBlock);
				SetEvent(m_hEventPlayBackUnBlock);
			}

			ret = RT_ERR_OK;
		}
	}
	return ret;
}


UINT CRt_Sim::SymbolGet(LPVOID pData)
{
	CRt_Sim* pThis = reinterpret_cast<CRt_Sim*>(pData);
	if(!pThis) return 0;
	
	CoInitializeEx(NULL,COINIT_SPEED_OVER_MEMORY);
	HANDLE hEvents[] = 
	{
		pThis->m_hEventStop,
        pThis->m_hEventGetSymbol
	};
	if(!pThis->InitializeParameters())
	{
		pThis->Call_StartUp(RT_ERR_CONNECTION_NOT_OPENED);
		SetEvent(pThis->m_hEventStop);
		return 0;
	}

	CDBConnection db(pThis->m_spOptions->ConnectionString);
	try
	{
		db.Open();
		pThis->Call_StartUp(RT_ERR_OK);
	}
	catch(...)
	{
		pThis->Call_StartUp(RT_ERR_CONNECTION_NOT_OPENED);
		SetEvent(pThis->m_hEventStop);
		return 0;
	}

	_bstr_t bsStock  = L"SELECT * FROM Contract WHERE (contractTypeID = 2 OR contractTypeID = 1) AND symbolName = ?";
	_bstr_t bsOption = L"SELECT * FROM Contract WHERE contractTypeID = 3 AND symbolName = ?";

	while(1)
	{
		DWORD dwStatus = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
		if(WAIT_OBJECT_0 == dwStatus) break;
		while(pThis->m_GetRequest.size())
		{
			if(WAIT_OBJECT_0 == WaitForSingleObject(pThis->m_hEventStop,0)) 
				break;
			CRequest req = pThis->m_GetRequest.front();
			pThis->m_GetRequest.pop();

			_bstr_t bsSymbol = req.m_bsSymbolName;
			if(!bsSymbol.length())
			{
				bsSymbol=	(req.m_Symbol.name);
				bsSymbol +=_T("_");
				bsSymbol +=req.m_Symbol.exchange;
			}

			long lHandle = 0;
			RT_ERR  errCode = RT_ERR_OK;
			try
			{
				SYMBOLMAP::iterator symbIter = pThis->m_SymbolID.find(bsSymbol);
				if(symbIter != pThis->m_SymbolID.end())
					req.m_Handle = symbIter->second;
				if(!req.m_Handle )
				{
					// Read from DB
					CSQLQuery<> cmd(db, (req.m_Symbol.u.dummy.dummy!=0)?bsOption:bsStock);
					cmd << _bstr_t(req.m_Symbol.name);
					CClientRecordset recSet;
					recSet.Open(cmd);
					CSymbol symbol;
					
					symbol.m_szExchange = req.m_Symbol.exchange;
					if(pThis->FillSymbol(symbol, recSet))
					{
						long lID = ++pThis->m_lSymbolCount;
						pThis->m_SymbolID[bsSymbol] = lID;
						pThis->m_Symbols[lID] = symbol;
						lHandle = lID;
					}
					else
						errCode = RT_ERR_SYMBOL_NOT_FOUND;
				}
				else
				{
					SYMBOLS::iterator it = pThis->m_Symbols.find(req.m_Handle);
					if(it==pThis->m_Symbols.end())
					{
						//Read from DB
						CSQLQuery<> cmd(db, "SELECT * FROM Contract WHERE contractID = ?");
						cmd <<(long)it->second.m_lID;
						cmd.Execute();
						CClientRecordset recSet;
						recSet.Open(cmd);
						CSymbol symbol;

						if(pThis->FillSymbol(symbol, recSet))
						{
							pThis->m_Symbols[req.m_Handle] = symbol;
							lHandle = req.m_Handle;
						}							
					}
					else
					{
						lHandle = req.m_Handle; 
					}
				}
			}
			catch(...)
			{
				errCode = RT_ERR_SYMBOL_NOT_FOUND;
			}
			SYMBOLS::iterator it = pThis->m_Symbols.find(lHandle);
			CSymbolResponce resp;

			resp.m_enErrorType = errCode;
			if(errCode == RT_ERR_OK && it != pThis->m_Symbols.end())
			{
				pThis->Simulation(it->second);
				pThis->FillFields(it->first, req, it->second, resp);
			}
			resp.m_Request = req;

			if(WAIT_OBJECT_0 == WaitForSingleObject(pThis->m_hEventStop, GetRandomValue2(pThis->m_spOptions->NonRealtimeIntervalMin  ,pThis->m_spOptions->NonRealtimeIntervalMax)))
				break;

			pThis->m_csResponce.Lock();
			pThis->m_Responce.push(resp);
			pThis->m_csResponce.Unlock();
			::SetEvent(pThis->m_hEventDataReady);

		}
	}

	CoUninitialize();
	return 0;
}

bool CRt_Sim::FillSymbol(CSymbol& symbol, CClientRecordset& recSet)
{
	bool bRet = false;
	if(recSet.GetRecordCount())
	{
		long lType     = recSet[L"contractTypeID"];

		symbol.m_lMarketID = recSet[L"exchangeID"];
		symbol.m_lID   = recSet[L"contractID"];
		symbol.m_szName= recSet[L"symbolName"];
		symbol.m_szDescription = recSet[L"description"];
		symbol.m_dPrice = recSet[L"priceMid"];
		symbol.m_dOpenPrice = symbol.m_dPrice;

		symbol.m_dClose = symbol.m_dOpenPrice * RandomPercent(m_spOptions->PricePercent);
	
		symbol.m_dtLastOperation = (vt_date)recSet[L"dateUpdate"];

		symbol.m_dLowPrice = symbol.m_dPrice;
		symbol.m_dHighPrice = symbol.m_dPrice;

		switch(lType)
		{
		case 1:
			symbol.m_enType = CSymbol::enIndex;
			break;
		case 2:
			symbol.m_enType = CSymbol::enStock;
			break;
		case 3:
			{
				UINT len = symbol.m_szName.length();
				TCHAR cType = *(((LPCTSTR)symbol.m_szName)+(len-1)*sizeof(TCHAR));
				if(cType<_T('M'))
					symbol.m_enType = CSymbol::enOptCall;
				else
					symbol.m_enType = CSymbol::enOptPut;
			}break;
		}

		bRet = true;
	}
	return bRet;
}

bool CRt_Sim::FillFields(long lID, CRequest& request, CSymbol& symbol, CSymbolResponce& responce)
{
	bool bRet = true;
	long lCount = request.GetFieldCount();
	if(!lCount) return false;

	if(!responce.m_Responce.Initialize(lCount)) 
		return false;

	for(int i = 0; i<lCount; i++)
	{
		CRespBuffer re;
		switch(request.GetField(i))
		{
		case RT_PREV_DIFF_PRICE:
			re.Initialize(RT_PREV_DIFF_PRICE, RT_REAL, _variant_t(_bstr_t(symbol.m_dNetchange)));
			break;
		case RT_SYMBOL_NAME://                    , // instrument's symbolic name                
			re.Initialize(RT_SYMBOL_NAME, RT_CHAR, _variant_t(_bstr_t(symbol.m_szName)));
			break;
		case RT_DESCRIPTION://                       , // Up to 128-char. description of symbol     
			re.Initialize(RT_DESCRIPTION, RT_CHAR, _variant_t(_bstr_t(symbol.m_szDescription)));
			break;                                          //  e.g. International Business Machines     
		case RT_PRIMARY_MKT_FLAG://                  , // set if this is the primary market         
			re.Initialize(RT_PRIMARY_MKT_FLAG, RT_INT32, _variant_t(1L));
			break;
		case RT_INSTRUMENT_TYPE://                   , // type of instrument                        
			re.Initialize(RT_INSTRUMENT_TYPE, RT_INT32, _variant_t((LONG)symbol.m_enType));
			break;
		case RT_MARKET_CODE://                       , // exchange code                             
			re.Initialize(RT_MARKET_CODE, RT_CHAR , _variant_t(symbol.m_szExchange));
			break;
		case RT_SYMBOL_HANDLE://                     , // Internal "handle" for symbol. It can be   
			re.Initialize(RT_SYMBOL_HANDLE, RT_UINT32, _variant_t((LONG)lID));
			break;
		case RT_PARENT_HANDLE://                     , // Internal "handle" for "parent" of symbol. 
			re.Initialize(RT_PARENT_HANDLE, RT_UINT32, _variant_t(0L));
			break;
		case RT_LAST_VOLUME://                       , // volume of last trade                      
			re.Initialize(RT_LAST_VOLUME, RT_UINT32, _variant_t(symbol.m_lVolume));
			break;
		case RT_LAST_PRICE://                        , // price at last trade                       
			re.Initialize(RT_LAST_PRICE, RT_REAL, _variant_t(symbol.m_dPrice));
			break;
		case RT_LAST_TIME://                         , // Time of last trade midnight sec           
			{
				vt_date dt(symbol.m_dtLastOperation);
				re.Initialize(RT_LAST_TIME, RT_UINT32, _variant_t((long)(dt.get_hour()*3600+dt.get_minute()*60+dt.get_second())));				
			}
			break;
		case RT_ANN_HIGH://                          , // annual high price                         
		case RT_HIGH://                              , // high for day                              
			re.Initialize(request.GetField(i), RT_REAL, _variant_t(symbol.m_dHighPrice));
			break;
		case RT_ANN_LOW://                           , // annual low                                
		case RT_LOW://                               , // low for day                               
			re.Initialize(request.GetField(i), RT_REAL, _variant_t(symbol.m_dLowPrice));
			break;
		case RT_OPEN://                              , // day's open                                
			re.Initialize(RT_OPEN, RT_REAL, _variant_t(symbol.m_dOpenPrice));
			break;
		case RT_BID://                               , // real-time bid                             
			re.Initialize(RT_BID, RT_REAL, _variant_t(symbol.m_dBid));
			break;
		case RT_ASK://                               , // real-time ask                             
			re.Initialize(RT_ASK, RT_REAL, _variant_t(symbol.m_dAsk));
			break;
		case RT_PRICE_RENDER_FMT://                  , // display format for price fields           
			re.Initialize(RT_PRICE_RENDER_FMT, RT_CHAR, _variant_t(_bstr_t(_T(""))));
			break;
		case RT_ASK_SIZE://                          , // real-time ask in 100's                    
			re.Initialize(RT_ASK_SIZE, RT_INT32, _variant_t(symbol.m_lAskSize));
			break;
		case RT_BID_SIZE://                          , // real-time bid in 100's                    
			re.Initialize(RT_BID_SIZE, RT_INT32, _variant_t(symbol.m_lBidSize));
			break;
		case RT_TOT_VOLUME://                        , // real-time volume                          
			re.Initialize(RT_TOT_VOLUME, RT_UINT32, _variant_t(symbol.m_lSumVolume));
			break;
		case RT_STRIKE_PRICE://                      , // strike price                              
			if(symbol.m_enType == CSymbol::enOptCall || symbol.m_enType == CSymbol::enOptPut)
			{				
				long dte = long(symbol.m_dtExpDate) - long(vt_date::GetCurrentDate());
				re.Initialize(RT_STRIKE_PRICE, RT_REAL,  _variant_t(symbol.m_Strike));
				break;
			}
		case RT_OPTION_DAY://                        , // # of days to expiration                   
			if(symbol.m_enType == CSymbol::enOptCall || symbol.m_enType == CSymbol::enOptPut)
			{				
				long dte = long(symbol.m_dtExpDate) - long(vt_date::GetCurrentDate());
				re.Initialize(RT_OPTION_DAY, RT_UINT32, _variant_t(dte));
				break;
			}
		case RT_Y_CLOSE://                           , // yesterday's closing price                 
			re.Initialize(RT_Y_CLOSE, RT_REAL, _variant_t(symbol.m_dClose));
			break;

		case RT_Y_BID://                             , // yesterday's closing bid                   
		case RT_Y_ASK://                             , // yesterday's closing ask                   
		case RT_Y_VOLUME://                          , // yesterday's closing  volume               
		case RT_Y_COMPOSITE_CLOSE://                 , // yesterday's composite close               
		case RT_VARIANCE://                          , // price variance                            
		case RT_TRADING_UNITS://                     , // option trading units                      
		case RT_OPTION_DIV://                        , // # of dividend payments                    
		case RT_OPEN_INTEREST://                     , // open interest                             
		case RT_OPTION_SERIES://                     , // series #                                  
		case RT_TRADE_FLAG://                        , // trade : 0=last 1=bid 2=ask 3=sett         
		case RT_FUTURE_OPEN_RANGE_1://               , // opening range 1                           
		case RT_FUTURE_OPEN_RANGE_2://               , // opening range 2                           
		case RT_FUTURE_TYPE://                       , // future type                               
		case RT_FIRST_DELIVERY_DATE://               , // first delivery date, yymmdd               
                                          // to extract use sprintf(buf,"%02d\/%02d\/%02",          
                                          //    d/10000 , (d%10000)/100 , d%100 );       
		case RT_LAST_DELIVERY_DATE://                , // last  delivery date, yymmdd               
		case RT_LAST_TRADE_DATE://                   , // last trade date, yymmdd                   
		case RT_VENDOR1_NAME://                      , // vendor 1 sec name                         
		case RT_VENDOR2_NAME://                      , // vendor 2 sec name                         
		case RT_STRIKE_DEV_CODE://                  , // strike derivation code                    
		case RT_FUT_MON_LTR_CODE://                  , // future month letter code                  
		case RT_VENDOR_STRIKE_PRICE://               , // vendor_strike_price;                      
		case RT_BOND_RATE://                         , // interest rate in 1000's                   
		case RT_BOND_INT_DATE://                     , // next int. payment date                    
		case RT_BOND_FREQ://                         , // int. pay frequency:                       
		case RT_ISSUE_DATE://                        , // issue date, mmddyy                        
		case RT_COUPON1_DATE://                      , // 1st coupon date, mmddyy                   
		case RT_CALL_DATE://                         , // call date, mmddyy                         
		case RT_AUCTION_SETT_DATE://                 , // auction settlement date, mmddyy           
		case RT_MATURITY_DATE://                     , // maturity_date, mmddyy                     
		case RT_COUPON_RATE://                       , // coupon_rate in percentage                 
		case RT_CUSIP://                             , // cusip number                              
		case RT_SYS_ALERT://                         , // E.g. CBOE feed down                       
		case RT_TKR_TYPE_BITS://                     , // see ticker line trade types in rt_share.h 
		case RT_DAYS_TO_EXPIRATION://                , //                                           
		case RT_TRADE_SEQUENCE_NUM://               , //                                           
		case RT_SERIES_YEAR://                       , // opt/bnd series year                       
		case RT_CALL_PUT_FLAG://                     , // C/P                                       
		case RT_MODEL://                             , // Model to use                              
		case RT_QUOTE_CONDITION://                   , // quote condition                           
		case RT_MARKET_CODE_NUMBER://                , // exchange code number                      
		case RT_BIT_FLAGS://                         , // bit flags in pricefile record             
		case RT_PRIMARY_OPTION_MKT_CODE://           , // primary option market code                
		case RT_BLOCK_SIZE://                        , // trading block size ofr instrument
		case RT_RESTRICTED_LIST://                   , // restricted list level bits 
                                          // !!PRIVATE TO BROKERAGE HOUSE API'S
                                          // and only aviliable where entitled
		case RT_NEXT_DB://                           , // next valid db number in pricefile
		case RT_TICK_INDICATOR://                    , // 1 if tick is up -1 if tick is down ?? can tick be unchanged??
		case RT_SIGNAL_TYPE://                       , // defined in pstypes.h from track's internal use
		case RT_H2://                                , // defined in pstypes.h from track's internal use
		case RT_TRADE_BIT_FLAGS://                   , // defined in pstypes.h from track's internal use

		case RT_TRADE_CONDITION_CHAR://              , // defined in pstypes.h from track's internal use

		case RT_TICK_TIME://                         , // time of tick (resynch, quote, trade, correction etc...
		case RT_PREV_TRADE://                        , // previous trade price (may be same)        
		case RT_OPTION_CODE://                       , // option series designation                 
		case RT_RESTRICTED://                        , // restricted list indicator                 
		case RT_LAST_NEWS_TIME://                    , // hours/minutes                             
		case RT_1ST_DIVID_DATE://                    , // 1st dividend date, yymmdd                 
		case RT_1ST_DIVID://                         , // 1st dividend amount                       
		case RT_2ND_DIVID_DATE://                    , // 2st dividend date, yymmdd                 
		case RT_2ND_DIVID://                         , // 2st dividend amount                       
		case RT_PROJECTED_DIV://                     , // 0=actual, 1=projected                     
		case RT_DIV_PER_YEAR://                      , // # divs per year                           
		case RT_EPS://                               , // earnings per share                        
		default:
			re.Initialize(request.GetField(i), RT_UINT32, _variant_t(0L));
			break;
		}
		responce.m_Responce.putData(i,re);

	}

	return bRet;
}

void CRt_Sim::Simulation(CSymbol& symbol)
{
	if(symbol.m_dPrice <0.0001) return;
	// Trade or Quote 
	if(symbol.m_dPrice < symbol.m_dOpenPrice*0.70)			
		symbol.m_dPrice = symbol.m_dOpenPrice*0.8;

	if(symbol.m_dPrice > symbol.m_dOpenPrice*1.30)
		symbol.m_dPrice = symbol.m_dOpenPrice*1.2;


	{
		EgLib::CAutoLock s(symbol.m_cs);
		// Quote
		symbol.m_lVolume = 0; 
	
	
		if(m_spOptions->AskPercent > 0.00001)
			symbol.m_dAsk = symbol.m_dPrice  * RandomPercent(m_spOptions->AskPercent);
		else
			symbol.m_dAsk = symbol.m_dPrice+0.01;

		symbol.m_lAskSize = GetRandomValue(m_spOptions->AskSizeMin ,m_spOptions->AskSizeMax); ;

		if(m_spOptions->BidPercent > 0.00001)
			symbol.m_dBid = symbol.m_dPrice*RandomPercent(m_spOptions->BidPercent);
		else
			symbol.m_dBid = symbol.m_dPrice-0.01;

		if(m_spOptions->AskPercent > 0.00001 || m_spOptions->BidPercent > 0.00001)
		{

		}

		symbol.m_lBidSize = GetRandomValue(m_spOptions->BidSizeMin ,m_spOptions->BidSizeMax); 
		if(symbol.m_dBid<0.00001)
			symbol.m_dBid = 0;
		if(symbol.m_dAsk<0.00001)
			symbol.m_dAsk = 0;

		if(m_spOptions->AskPercent > 0.00001 || m_spOptions->BidPercent > 0.00001)
		{
			symbol.m_dBid = long(symbol.m_dBid*1000)/1000.;
			symbol.m_dAsk = long(symbol.m_dAsk*1000)/1000.;

			if(symbol.m_dBid>symbol.m_dAsk)
			{
				if(GetRandomValue2(0,100)<80)
					symbol.m_dAsk = symbol.m_dBid +0.02;
				else
					symbol.m_dBid = symbol.m_dAsk -0.01;
			}

			if(symbol.m_dBid == symbol.m_dAsk)
			{
				if(GetRandomValue2(0,100)<75)
					symbol.m_dAsk +=0.02;
				else
					symbol.m_dBid -=0.01;
			}

			symbol.m_dPrice = (symbol.m_dAsk+symbol.m_dBid)/2;

			if(symbol.m_dAsk<0.0005)
			{
				symbol.m_dPrice = symbol.m_dBid;
				symbol.m_lAskSize = 0;
			}

			if(symbol.m_dBid<0.0005)
			{
				symbol.m_dPrice = symbol.m_dAsk;
				symbol.m_lBidSize = 0;
			}
			if(symbol.m_dPrice<0)
			{
				symbol.m_dPrice = symbol.m_dOpenPrice;
				symbol.m_dBid = symbol.m_dPrice -0.05;
				symbol.m_dAsk = symbol.m_dPrice +0.05;
			}

		}



	}
	symbol.m_dtLastOperation = vt_date::GetCurrentDate();	

	if(symbol.m_dLowPrice < 0.005) symbol.m_dLowPrice = symbol.m_dPrice;
	if(symbol.m_dHighPrice < 0.005) symbol.m_dHighPrice = symbol.m_dPrice;

	if(symbol.m_dLowPrice>symbol.m_dPrice && symbol.m_dPrice>0.005)
		symbol.m_dLowPrice = symbol.m_dPrice;
	if(symbol.m_dHighPrice<symbol.m_dPrice && symbol.m_dPrice>0.005)
		symbol.m_dHighPrice = symbol.m_dPrice;

	symbol.m_dNetchange = symbol.m_dPrice - symbol.m_dClose;
}

 
UINT CRt_Sim::SymbolResponce(LPVOID pData)
{
	if(!pData) return 0;
	CoInitializeEx(NULL,COINIT_SPEED_OVER_MEMORY);
	CRt_Sim* pThis = reinterpret_cast<CRt_Sim*>(pData);

	HANDLE hEvents[] = 
	{
		pThis->m_hEventStop,
        pThis->m_hEventDataReady
	};

	pThis->Call_StartUp(RT_ERR_OK);
	while(1)
	{
		if(WAIT_OBJECT_0 == WaitForMultipleObjects(2, hEvents, FALSE,INFINITE))
			break;
		while(pThis->m_Responce.size())
		{
			CSymbolResponce resp;
			{
				EgLib::CAutoLock s(pThis->m_csResponce);
				if(pThis->m_Responce.empty()) break;

				resp = pThis->m_Responce.front();
				pThis->m_Responce.pop();
			}
			
			pThis->Call_GetFields(resp);


		}		
	}
	CoUninitialize();
	return 0;
}


UINT CRt_Sim::PlaybackMind(LPVOID pData)
{

	CRt_Sim* pThis = reinterpret_cast<CRt_Sim*>(pData);
	if(!pThis) return 0;
	CRequest r;
	bool bInitialized = false;
	DATE dtSaveDate = 0;
	
	CoInitializeEx(NULL,COINIT_SPEED_OVER_MEMORY);
	HANDLE hEvents[] = 
	{
		pThis->m_hEventStop,
        pThis->m_hEventMindPBStock,
        pThis->m_hEventMindPBOption

	};

	HANDLE hEvtBlock[] =
	{
		pThis->m_hEventStop,
        pThis->m_hEventPlayBackUnBlock
	};

	if(!pThis->InitializeParameters())
	{
		pThis->Call_StartUp(RT_ERR_CONNECTION_NOT_OPENED);
		SetEvent(pThis->m_hEventStop);
		return 0;
	}

	CDBConnection db(pThis->m_spOptions->ConnectionString);
	try
	{
		db.Open();
		pThis->Call_StartUp(RT_ERR_OK);
	}
	catch(...)
	{
		pThis->Call_StartUp(RT_ERR_CONNECTION_NOT_OPENED);
		SetEvent(pThis->m_hEventStop);
		CoUninitialize();
		return 0;
	}
	CSQLQuery<CDefaultRecordset> spQuery(db, L"Select * from QuoteCache where tradeDate = ?");

	while(1)
	{
		DWORD dwStatus = WaitForMultipleObjects(3, hEvents, FALSE, INFINITE);
		if(WAIT_OBJECT_0 == dwStatus) break;
		long lMind = 1;
		while(lMind)
		{
			if(WAIT_OBJECT_0 == WaitForMultipleObjects(2,hEvtBlock,FALSE,INFINITE))
				break;

			if(double(pThis->m_spOptions->PlayBackDate)<0.05)
			{
				// Playback
				SetEvent(pThis->m_hEventMindUnBlock);
				ResetEvent(pThis->m_hEventPlayBackUnBlock);
				bInitialized = false;
				continue;
			}
			if(!bInitialized || dtSaveDate != pThis->m_spOptions->PlayBackDate)
			{
				spQuery.ClearParams();
				spQuery.Close();

				spQuery << vt_date(long(pThis->m_spOptions->PlayBackDate));
				spQuery.Open();
				dtSaveDate = pThis->m_spOptions->PlayBackDate;
				bInitialized = true;

			}			
			if(spQuery.IsEOF() || !spQuery.IsOpened())
			{
				ResetEvent(pThis->m_hEventPlayBackUnBlock);
				bInitialized = false;
				continue;
			}

			long lPeriod = spQuery[L"Period"];
			long lType = spQuery[L"TickerType"];
			_bstr_t bsName = spQuery[L"Symbol"];
			_bstr_t bsFullname = bsName + _bstr_t(L"_");

			if(pThis->m_spOptions->PlayBackSpeed)
			{
				if(pThis->m_spOptions->PlayBackSpeed<0)
				{
					//Slow
					long  per = -pThis->m_spOptions->PlayBackSpeed;
					per++;
					lPeriod *=  per;

				}
				else
				{
					// Quick
					long  per = pThis->m_spOptions->PlayBackSpeed;
					per++;
					double dou = lPeriod;
					lPeriod = long(dou / per);
				}
			}
			if(WAIT_OBJECT_0 == WaitForSingleObject(pThis->m_hEventStop, lPeriod)) 
				break;

			if(!(pThis->m_MindOptionRequest.size()+pThis->m_MindStockRequest.size()))
			{
				ResetEvent(pThis->m_hEventPlayBackUnBlock);
				bInitialized = false;
				continue;
			}

				SYMBOLMAP::iterator itt = pThis->m_SymbolID.find(bsFullname);

				if(itt == pThis->m_SymbolID.end() ||
				   !pThis->m_Symbols[itt->second].m_bSubscribed)
				{
					spQuery.MoveNext();
					continue;
				}


			CRequest& req = r;
			bool bOption = true;
			int iPos = 0;
			bool bFound = false;

			if(lType == 3)
			{
				EgLib::CAutoLock sd(pThis->m_csMind);
				// Options
				MINDREQ::iterator itMind;
				for(itMind = pThis->m_MindOptionRequest.begin(); itMind != pThis->m_MindOptionRequest.end(); itMind++)
				{
					req = *itMind;
					if(req.m_bsSymbolName == bsFullname)
					{
						bFound = true;
						break;
					}				
				}
			}
			else
			{
				EgLib::CAutoLock sd(pThis->m_csMind);
				// Stocks
				MINDREQ::iterator itMind;
				for(itMind = pThis->m_MindStockRequest.begin(); itMind != pThis->m_MindStockRequest.end(); itMind++)
				{
					req = *itMind;
					if(req.m_bsSymbolName == bsFullname)
					{
						bFound = true;
						break;
					}				
				}

			}
			if(!bFound)
			{
				spQuery.MoveNext();
				continue;
			}

			CSymbolResponce resp;
			CSymbol& symb = pThis->m_Symbols[req.m_Handle];
			
			symb.m_dAsk = spQuery[L"AskPrice"];
			symb.m_dBid = spQuery[L"BidPrice"];

			symb.m_dBid =  (double(long(symb.m_dBid*1000)))/1000;
			symb.m_dAsk =  (double(long(symb.m_dAsk*1000)))/1000;
			symb.m_lBidSize = spQuery[L"BidSize"];
			symb.m_lAskSize = spQuery[L"AskSize"];

			symb.m_dtLastOperation = spQuery[L"DateUpdate"];

			symb.m_dPrice = spQuery[L"LastPrice"];
			symb.m_lVolume = spQuery[L"Volume"];
			symb.m_lSumVolume += symb.m_lVolume;

			if(symb.m_dPrice < 0.005)
			{
				symb.m_dPrice = (symb.m_dAsk+symb.m_dBid)/2;
				if(symb.m_dAsk < 0.005)
					symb.m_dPrice = symb.m_dBid;

				if(symb.m_dBid < 0.005)
					symb.m_dPrice = symb.m_dAsk;
			}

			if(symb.m_dLowPrice < 0.005) symb.m_dLowPrice = symb.m_dPrice;
			if(symb.m_dHighPrice < 0.005) symb.m_dHighPrice = symb.m_dPrice;

			if(symb.m_dLowPrice>symb.m_dPrice && symb.m_dPrice>0.005)
				symb.m_dLowPrice = symb.m_dPrice;
			if(symb.m_dHighPrice<symb.m_dPrice && symb.m_dPrice>0.005)
				symb.m_dHighPrice = symb.m_dPrice;


			pThis->FillFields(req.m_Handle, req, symb, resp);

			resp.m_enErrorType = RT_ERR_OK;
			resp.m_Request = req;

			pThis->m_csResponce.Lock();
			pThis->m_Responce.push(resp);
			pThis->m_csResponce.Unlock();
			::SetEvent(pThis->m_hEventDataReady);
			spQuery.MoveNext();

		}

	}

	CoUninitialize();

	return 0;
}

UINT CRt_Sim::SymbolMind(LPVOID pData)
{
	CRt_Sim* pThis = reinterpret_cast<CRt_Sim*>(pData);
	if(!pThis) return 0;
	CRequest r;
	
	CoInitializeEx(NULL,COINIT_SPEED_OVER_MEMORY);
	HANDLE hEvents[] = 
	{
		pThis->m_hEventStop,
        pThis->m_hEventMindStock
	};

	HANDLE hEvtBlock[] =
	{
		pThis->m_hEventStop,
        pThis->m_hEventMindUnBlock
	};

	pThis->Call_StartUp(RT_ERR_OK);
	while(1)
	{
		DWORD dwStatus = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
		if(WAIT_OBJECT_0 == dwStatus) break;
		long lMind = 1;
		while(lMind)
		{
			if(WAIT_OBJECT_0 == WaitForMultipleObjects(2,hEvtBlock,FALSE,INFINITE))
				break;

			if(WAIT_OBJECT_0 == WaitForSingleObject(pThis->m_hEventStop, GetRandomValue(pThis->m_spOptions->StockIntervalMin, pThis->m_spOptions->StockIntervalMax))) 
				break;

			if(double(pThis->m_spOptions->PlayBackDate)>1)
			{
				// Playback
				ResetEvent(pThis->m_hEventMindUnBlock);
				SetEvent(pThis->m_hEventPlayBackUnBlock);
				continue;
			}

			CRequest& req = r;
			bool bOption = true;
			size_t iPos = 0;

			{
				EgLib::CAutoLock sd(pThis->m_csMind);
				lMind = pThis->m_MindStockRequest.size();
				if(!lMind)
					break;

				if(lMind<20)
					iPos = GetRandomValue2(0,lMind-1);
				else
					iPos = GetRandomValue(0,lMind-1);

				req = pThis->m_MindStockRequest[iPos];
				bOption = false;
			}
			if(VARIANT_FALSE == pThis->m_spOptions->EnableStocks) 
				continue;			


			CSymbolResponce resp;
			CSymbol& symb = pThis->m_Symbols[req.m_Handle];
			try
			{
				if(symb.m_dOpenPrice<=0.05)
				{
					EgLib::CAutoLock s(pThis->m_csMind);
					if(WAIT_TIMEOUT != WaitForSingleObject(pThis->m_hEventStop,0)) 
						break;

					MINDREQ::iterator iter= pThis->m_MindStockRequest.begin();
					for(size_t k = 0; k<iPos;k++)
						iter++;
					if(pThis->m_MindStockRequest.size()>iPos)
					{	
						if(iter->m_bsSymbolName == pThis->m_MindStockRequest[k].m_bsSymbolName)
						{
							pThis->m_MindStockRequest.erase(iter);
						} 
					} 
					lMind = pThis->m_MindStockRequest.size();
					continue;
				}
			}
			catch(...)
			{ continue;}

			pThis->Simulation(symb);
			pThis->FillFields(req.m_Handle, req, symb, resp);

			resp.m_enErrorType = RT_ERR_OK;
			resp.m_Request = req;

			pThis->m_csResponce.Lock();
			pThis->m_Responce.push(resp);
			pThis->m_csResponce.Unlock();
			::SetEvent(pThis->m_hEventDataReady);
		}
	}

	CoUninitialize();
	return 0;
}

UINT CRt_Sim::OptionMind(LPVOID pData)
{
	CRt_Sim* pThis = reinterpret_cast<CRt_Sim*>(pData);
	if(!pThis) return 0;
	CRequest r;
	
	CoInitializeEx(NULL,COINIT_SPEED_OVER_MEMORY);
	HANDLE hEvents[] = 
	{
		pThis->m_hEventStop,
        pThis->m_hEventMindOption
	};
	HANDLE hEvtBlock[] =
	{
		pThis->m_hEventStop,
        pThis->m_hEventMindUnBlock
	};

	pThis->Call_StartUp(RT_ERR_OK);
	while(1)
	{
		DWORD dwStatus = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
		if(WAIT_OBJECT_0 == dwStatus) break;
		long lMind = 1;
		while(lMind)
		{
			if(WAIT_OBJECT_0 == WaitForMultipleObjects(2,hEvtBlock,FALSE,INFINITE))
				break;

			if(WAIT_OBJECT_0 == WaitForSingleObject(pThis->m_hEventStop, GetRandomValue(pThis->m_spOptions->IntervalMin, pThis->m_spOptions->IntervalMax))) 
				break;

			if(double(pThis->m_spOptions->PlayBackDate)>1)
			{
				// Playback
				ResetEvent(pThis->m_hEventMindUnBlock);
				SetEvent(pThis->m_hEventPlayBackUnBlock);
				continue;
			}

			CRequest& req = r;
			size_t iPos = 0;
			
			{
				EgLib::CAutoLock sd(pThis->m_csMind);

				lMind = pThis->m_MindOptionRequest.size();
				if(!lMind) 
					break;
				if(lMind > 50)
					iPos = GetRandomValue(0,lMind-1);
				else
					iPos = GetRandomValue2(0,lMind-1);
				
				req = pThis->m_MindOptionRequest[iPos];
			}
			if(VARIANT_FALSE == pThis->m_spOptions->EnableOptions) 
				continue;			

			CSymbolResponce resp;
			CSymbol& symb = pThis->m_Symbols[req.m_Handle];
			try
			{
				if(symb.m_dOpenPrice<=0.05)
				{
					EgLib::CAutoLock s(pThis->m_csMind);
					if(WAIT_TIMEOUT != WaitForSingleObject(pThis->m_hEventStop,0)) 
						break;

					MINDREQ::iterator iter= pThis->m_MindOptionRequest.begin();
					for(size_t k = 0; k<iPos;k++)
						iter++;

					if(pThis->m_MindOptionRequest.size()>iPos)
					{	
						if(iter->m_bsSymbolName == pThis->m_MindOptionRequest[k].m_bsSymbolName)
							pThis->m_MindOptionRequest.erase(iter);
					}
					lMind = pThis->m_MindOptionRequest.size();
					continue;
				}
			}
			catch(...)
			{ continue; }
	
			pThis->Simulation(symb);
			pThis->FillFields(req.m_Handle, req, symb, resp);

			resp.m_enErrorType = RT_ERR_OK;
			resp.m_Request = req;

			
			pThis->m_csResponce.Lock();
			pThis->m_Responce.push(resp);
			pThis->m_csResponce.Unlock();
			::SetEvent(pThis->m_hEventDataReady);
		}
	}

	CoUninitialize();
	return 0;
}

RT_ERR CRt_Sim::RequestGroup(
	  RT_CONNECT_ID                   rt_id , // RT connect id
	  RT_REQ_ID *        req_id_ptr_to_fill , // Returned address of a req_entry to        
	  void  *                     user_data , // Pointer to any user data. This pointer, 
	  RT_GRP_REQ_TY              group_type , // Type of GROUP for which information is 
	  RT_HANDLE                      handle , // Root symbol (i.e. ticker, future family)    
	  RT_GET_GROUP_CLBK            callback   // Address of callback routine which gets  
)
{
	RT_ERR ret = RT_ERR_CONNECTION_INVALID;
	if(!m_bConnected ) return RT_ERR_CONNECTION_NOT_OPEN;
	if(!req_id_ptr_to_fill) return RT_ERR_CONNECTION_INVALID;
	
	*req_id_ptr_to_fill = (RT_REQ_ID)InterlockedIncrement(&m_nRequestCount);
	CRequest req;
	if(req.Initialize((long)(*req_id_ptr_to_fill), group_type, user_data, handle, callback))
	{
		Put_GetGroup(callback);
		if(RT_GRP_REQ_TY_STKOPTS == group_type)
		{
			SYMBOLS::iterator iter = m_Symbols.find(req.m_Handle);
			if(iter == m_Symbols.end()) return RT_ERR_INVALID_HANDLE;
			m_GroupRequest.push(req);
			SetEvent(m_hEventGroup);
			ret = RT_ERR_OK;
		}
	}
	return ret;
}

UINT CRt_Sim::GroupResponce(LPVOID pData)
{
	CRt_Sim* pThis = reinterpret_cast<CRt_Sim*>(pData);
	if(!pThis) return 0;
	
	CoInitializeEx(NULL,COINIT_SPEED_OVER_MEMORY);
	HANDLE hEvents[] = 
	{
		pThis->m_hEventStop,
        pThis->m_hEventGroup
	};
	if(!pThis->InitializeParameters())
	{
		pThis->Call_StartUp(RT_ERR_CONNECTION_NOT_OPENED);
		SetEvent(pThis->m_hEventStop);
		CoUninitialize();
		return 0;
	}
	CDBConnection db(pThis->m_spOptions->ConnectionString);
	try
	{
		db.Open();
		pThis->Call_StartUp(RT_ERR_OK);
	}
	catch(...)
	{
		pThis->Call_StartUp(RT_ERR_CONNECTION_NOT_OPENED);
		SetEvent(pThis->m_hEventStop);
		CoUninitialize();
		return 0;
	}

	_bstr_t bstrCmd =
		L"SELECT [Option].[OptionID] AS iOptionID, [Option].[UnderlyingID] AS iUnderlyingID, [isCall] AS bIsCall, [strike] AS fStrike, [expDate] AS dtExpDate, [Contract].[symbolName] AS vcSymbolName, [exchangeID] AS iExchangeID, [Contract].[description] AS vcDescription, [Contract].[lotSize] AS iLotSize, [priceMid] AS fPriceMid ,[IV] AS fIV ,[dateUpdate] AS dtDate FROM [Option] INNER JOIN Contract ON [Option].[OptionID]=[Contract].[contractID]"
		L"	WHERE [Option].[UnderlyingID]= ? And [Option].[expDate]>= now()";

	while(1)
	{
		if(WAIT_OBJECT_0 == WaitForMultipleObjects(2, hEvents, FALSE, INFINITE))
			break;
		long lSize = 1;

		while(pThis->m_GroupRequest.size())
		{
			if(WAIT_OBJECT_0 == WaitForSingleObject(pThis->m_hEventStop, 0))
				break;
			CRequest req = pThis->m_GroupRequest.front();
			SYMBOLS::iterator iterMap =  pThis->m_Symbols.find(req.m_Handle);
			if(iterMap == pThis->m_Symbols.end())
			{
				//Error
				continue;
			}

			pThis->m_GroupRequest.pop();
			if(req.m_Handle)
			{
				CSQLQuery<> cmd(db, bstrCmd);
				cmd << long(iterMap->second.m_lID);

				CClientRecordset recSet;
				recSet.Open(cmd);
				int iRecords = recSet.GetRecordCount();
				if(iRecords)
				{
					RT_HANDLE* pHandles = new RT_HANDLE[iRecords];
					int i = 0;
					for(recSet.MoveFirst(); !recSet.IsEOF(); recSet.MoveNext())
					{
						long iID = recSet[L"iOptionID"];
						long lID = 0;
						_bstr_t bsOption = recSet[L"vcSymbolName"];

						_bstr_t bsSymbol(bsOption);
						bsSymbol +=_T("_");
						bsSymbol +=iterMap->second.m_szExchange;
						SYMBOLMAP::iterator itr = pThis->m_SymbolID.find(bsSymbol);
						if(itr == pThis->m_SymbolID.end())
						{
							// new
							lID = ++pThis->m_lSymbolCount;
							pThis->m_SymbolID[bsSymbol] = lID;

							CSymbol symbol;

							symbol.m_lID = recSet[L"iOptionID"];
							symbol.m_lParentID =recSet[L"iUnderlyingID"];
							symbol.m_enType = ((long)recSet[L"bIsCall"])==1?CSymbol::enOptCall : CSymbol::enOptPut;
							symbol.m_type = (long)recSet[L"bIsCall"];

							symbol.m_Strike = recSet[L"fStrike"];
							symbol.m_dtExpDate = (vt_date)recSet[L"dtExpDate"];
							symbol.m_szName = bsOption;
							symbol.m_lMarketID =  recSet[L"iExchangeID"];
							symbol.m_szDescription = recSet[L"vcDescription"];
							symbol.m_dPrice = recSet[L"fPriceMid"];
							symbol.m_dOpenPrice =  symbol.m_dPrice;

							symbol.m_dClose = symbol.m_dOpenPrice * pThis->RandomPercent(pThis->m_spOptions->PricePercent);

							symbol.m_dtLastOperation = (vt_date)recSet[L"dtDate"];
							pThis->m_Symbols[lID] = symbol;

						}
						else
						{
							//existing
							lID = itr->second;
							SYMBOLS::iterator iter = pThis->m_Symbols.find(lID);
							if(iter!=pThis->m_Symbols.end())
							{

								CSymbol symbol;

								symbol.m_lID = recSet[L"iOptionID"];
								symbol.m_lParentID =recSet[L"iUnderlyingID"];
								symbol.m_enType = ((long)recSet[L"bIsCall"])==1?CSymbol::enOptCall : CSymbol::enOptPut;
								symbol.m_type = (long)recSet[L"bIsCall"];

								symbol.m_Strike = recSet[L"fStrike"];
								symbol.m_dtExpDate = (vt_date)recSet[L"dtExpDate"];
								symbol.m_szName = bsOption;
								symbol.m_lMarketID =  recSet[L"iExchangeID"];
								symbol.m_szDescription = recSet[L"vcDescription"];
								symbol.m_dPrice = recSet[L"fPriceMid"];
								symbol.m_dOpenPrice =  symbol.m_dPrice;

								symbol.m_dClose = symbol.m_dOpenPrice * pThis->RandomPercent(pThis->m_spOptions->PricePercent);

								symbol.m_dtLastOperation = (vt_date)recSet[L"dtDate"];
								iter->second = symbol;

//								iter->second.m_lParentID = req.m_Handle;
//								iter->second.m_Strike = recSet[L"fStrike"];
//								iter->second.m_dtExpDate = (vt_date)recSet[L"dtExpDate"];
							}
							else
							{
								CSymbol symbol;

								symbol.m_lID = recSet[L"iOptionID"];
								symbol.m_lParentID =recSet[L"iUnderlyingID"];
								symbol.m_enType = ((long)recSet[L"bIsCall"])==1?CSymbol::enOptCall : CSymbol::enOptPut;
								symbol.m_type = (long)recSet[L"bIsCall"];

								symbol.m_Strike = recSet[L"fStrike"];
								symbol.m_dtExpDate = (vt_date)recSet[L"dtExpDate"];
								symbol.m_szName = bsOption;
								symbol.m_lMarketID =  recSet[L"iExchangeID"];
								symbol.m_szDescription = recSet[L"vcDescription"];
								symbol.m_dPrice = recSet[L"fPriceMid"];
								symbol.m_dOpenPrice =  symbol.m_dPrice;

								symbol.m_dClose = symbol.m_dOpenPrice * pThis->RandomPercent(pThis->m_spOptions->PricePercent);

								symbol.m_dtLastOperation = (vt_date)recSet[L"dtDate"];


								pThis->m_Symbols[lID] = symbol;
							}
						}


						pHandles[i] = lID;
						i++;
					}

					int iSent = 0;
					int iBuf = 5;
					while(iSent < iRecords)
					{
						if((iRecords - iSent)< iBuf)
						{
							iBuf = iRecords - iSent;
						}
						RT_HANDLE* pHandleBuf = new RT_HANDLE[iBuf];
						if(pHandleBuf)
						{
							for(int k = 0; k<iBuf; k++)
							{
								pHandleBuf[k] = pHandles[iSent+k]; 
							}

							if(WAIT_OBJECT_0 == WaitForSingleObject(pThis->m_hEventStop, GetRandomValue2(pThis->m_spOptions->NonRealtimeIntervalMin  ,pThis->m_spOptions->NonRealtimeIntervalMax)))
								break;

							pThis->Call_GetGroup(req, iRecords, iSent, iBuf, pHandleBuf, RT_ERR_OK);
							delete[] pHandleBuf;
						}
						iSent += iBuf;
					}
					delete[] pHandles;
				}
				else
	 				pThis->Call_GetGroup(req, 0, 0, 0, NULL, RT_ERR_SYMBOL_NOT_FOUND);
			}		
		}
	}	
	CoUninitialize();
	return 0;
}

RT_ERR CRt_Sim::RemoveFields(RT_CONNECT_ID rt_id, RT_REQ_ID req_id, int field_count, RT_FIELD* pFields)
{
	if(!m_bConnected) return RT_ERR_CONNECTION_NOT_OPEN;
	EgLib::CAutoLock f(m_csMind);
	EgLib::CAutoLock fa(m_csRequest);
	for(MINDREQ::iterator iterStock = m_MindStockRequest.begin(); iterStock!=m_MindStockRequest.end(); iterStock++)
	{
		CRequest& req = *iterStock;
		for(RTUSERDATA::iterator iterUD = req.m_UserData.begin(); iterUD != req.m_UserData.end(); iterUD++)
		{

			if(iterUD->first == req_id)
			{
				req.m_UserData.erase(iterUD);
				if(req.m_UserData.empty())
				{
					int iFields = req.GetFieldCount();
					if(field_count == 0 || !pFields)
					{
						m_Symbols[req.m_Handle].m_bSubscribed = false;
						m_MindStockRequest.erase(iterStock);
						return RT_ERR_OK;
					}
					else
					{
						RT_FIELD* pFieldsNew = new RT_FIELD[iFields];
						int iCopied = 0;				
						for(int i = 0; i<iFields; i++)
						{
							RT_FIELD fld = req.GetField(i);
							bool bFound = false;
							for(int j=0; j>field_count; j++)
							{
								if(fld == pFields[j])
								{
									bFound = true;
									break;
								}
							}
							if(!bFound)
							{
								pFieldsNew[iCopied] = fld;
								iCopied++;
							}
						}
						req.CopyFields(iCopied,pFieldsNew);
						return RT_ERR_OK;
					}
				}
				break;
			}

		}
	}

	for(MINDREQ::iterator iterOption = m_MindOptionRequest.begin(); iterOption!=m_MindOptionRequest.end(); iterOption++)
	{
		CRequest& req = *iterOption;
		for(RTUSERDATA::iterator iterUD = req.m_UserData.begin();iterUD != req.m_UserData.end(); iterUD++)
		{
			if(iterUD->first == req_id)
			{
				req.m_UserData.erase(iterUD);
				if(req.m_UserData.empty())
				{
					int iFields = req.GetFieldCount();
					if(field_count == 0 || !pFields)
					{
						m_Symbols[req.m_Handle].m_bSubscribed = false;
						m_MindOptionRequest.erase(iterOption);
						return RT_ERR_OK;
					}
					else
					{
						RT_FIELD* pFieldsNew = new RT_FIELD[iFields];
						int iCopied = 0;				
						for(int i = 0; i<iFields; i++)
						{
							RT_FIELD fld = req.GetField(i);
							bool bFound = false;
							for(int j=0; j>field_count; j++)
							{
								if(fld == pFields[j])
								{
									bFound = true;
									break;
								}
							}
							if(!bFound)
							{
								pFieldsNew[iCopied] = fld;
								iCopied++;
							}
						}
						req.CopyFields(iCopied,pFieldsNew);
						return RT_ERR_OK;
					}

				}
				break;
			}
		}
	}
	return RT_ERR_INVALID_REQ_ID;
}

RT_ERR CRt_Sim::AddFields(RT_CONNECT_ID rt_id, RT_REQ_ID req_id, int field_count, RT_FIELD* pFields)
{
/*
	if(!m_bConnected) return RT_ERR_CONNECTION_NOT_OPEN;
	EgLib::CAutoLock f(m_csMind);
	if(field_count == 0 || !pFields) return RT_ERR_INVALID_FIELD;
	for(MINDREQ::iterator iter = m_MindRequest.begin(); iter!=m_MindRequest.end(); iter++)
	{
		if(iter->m_ReqID == req_id)
		{
			CRequest& req = *iter;
			int iFields = req.GetFieldCount();
			if(field_count == 0 || !pFields)
			{
				m_MindRequest.erase(iter);
				return RT_ERR_OK;
			}
			else
			{
				RT_FIELD* pFieldsNew = new RT_FIELD[iFields+field_count];
				int iCopied = 0;				
				for(int i = 0; i<iFields; i++)
				{
					RT_FIELD fld = req.GetField(i);
					bool bFound = false;
					for(int j=0; j>field_count; j++)
					{
						if(fld == pFields[j])
						{
							bFound = true;
							break;
						}
					}
					if(!bFound)
					{
						pFieldsNew[iCopied] = fld;
						iCopied++;
					}
				}
				req.CopyFields(iCopied,pFieldsNew);
				return RT_ERR_OK;
			}
		}
	}
	return RT_ERR_INVALID_REQ_ID;
*/
	return RT_ERR_OK;
}
RT_ERR CRt_Sim::RemoveSymbols(RT_CONNECT_ID rt_id)
{
	EgLib::CAutoLock f(m_csMind);
	m_MindStockRequest.erase(m_MindStockRequest.begin(), m_MindStockRequest.end());
	return RT_ERR_OK;
}
