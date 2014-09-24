#ifndef _RT_SIM_H
#define _RT_SIM_H

#include "rt_Structures.h"
int GetRandomValue(long startPoint, long endPoint);

class CClientInfo
{
public:

	CClientInfo()
	{
		m_ID = 0;
		m_pStartUp = NULL;
		m_pShutDown = NULL;
		m_pResponce = NULL;
		m_pGroup = NULL;
		m_lCount = 0;
	}

	void Put_StartUp(RT_STARTUP_CLBK pData){m_pStartUp = pData;}
	void Put_ShutDown(RT_SHUTDOWN_CLBK pData){m_pShutDown = pData;}
	void Put_GetFields(RT_GET_FIELDS_CLBK pData){if(pData) m_pResponce = pData;}
	void Put_GetGroup(RT_GET_GROUP_CLBK pData){if(pData) m_pGroup = pData;}

	void Call_GetGroup(CRequest& req, int iRecCount, int iSent, int iNum, RT_HANDLE* pHandles, RT_ERR err_code)
	{
		EgLib::CAutoLock sd(m_cs);
		RT_GET_GROUP_CLBK pFunct = NULL;
		if(CheckPointer(req.m_pGroupNotifier))
			pFunct = req.m_pGroupNotifier;
		else 
			if(CheckPointer(m_pGroup))
				pFunct = m_pGroup;
       
		if(CheckPointer(pFunct))
		{
			for(RTUSERDATA::iterator iter = req.m_UserData.begin(); iter!=req.m_UserData.end();iter++)
			{
				(*pFunct)(m_ID, (RT_REQ_ID)iter->first, iter->second, req.m_groupType, req.m_Handle, iRecCount, iSent, iNum, pHandles,err_code);			
			}

		}
	}
	
	void Call_StartUp(RT_ERR err_code)
	{
		EgLib::CAutoLock sd(m_cs);
		if(InterlockedIncrement(&m_lCount)==5 || err_code != RT_ERR_OK)
		{
			if(CheckPointer(m_pStartUp))
				(*m_pStartUp)(m_ID, err_code);

			InterlockedExchange(&m_lCount, 0L);
		}
	}

	void Call_ShutDown(RT_ERR err_code)
	{
		InterlockedExchange(&m_lCount, 0L);
		EgLib::CAutoLock sd(m_cs);
		if(CheckPointer(m_pShutDown))
			(*m_pShutDown)(m_ID, err_code);
	}

	void Call_GetFields(CSymbolResponce& resp)
	{
		EgLib::CAutoLock sd(m_cs);
		RT_GET_FIELDS_CLBK pFunct  = NULL;
		if(CheckPointer(resp.m_Request.m_pNotifier))
			pFunct = resp.m_Request.m_pNotifier;
		else
		{
			if(CheckPointer(m_pResponce))
				pFunct = m_pResponce;
		}

		if(pFunct)
		{
			for(RTUSERDATA::iterator iter = resp.m_Request.m_UserData.begin(); iter!=resp.m_Request.m_UserData.end();iter++)
			{
				(*pFunct)(m_ID, 
						  (RT_REQ_ID)iter->first, 
						  iter->second, 
						  resp.m_Request.m_Type,
						  resp.m_Request.GetFieldCount(), 
						  resp.m_Request.GetFields(),
						  resp.m_Responce.getBufferSize(), 
						  resp.m_Responce.getBuffer(), 
						  resp.m_enErrorType);
			}

		}
	}
	RT_CONNECT_ID m_ID;
private:
	RT_STARTUP_CLBK    m_pStartUp;
	RT_SHUTDOWN_CLBK   m_pShutDown;
	RT_GET_FIELDS_CLBK m_pResponce;
	RT_GET_GROUP_CLBK  m_pGroup;
    long			   m_lCount;

	EgLib::CLock 	   m_cs;
	bool CheckPointer(void* pData) { return (pData && !IsBadCodePtr(FARPROC(pData)));}

};

class CRt_Sim : public CClientInfo{

	typedef queue<CRequest> GETREQ;
	typedef vector<CRequest> MINDREQ;	
	typedef queue<CSymbolResponce> RESPQU;
	typedef map<LONG, CSymbol> SYMBOLS;
	typedef map<_bstr_t, LONG> SYMBOLMAP;

public:
	RT_ERR Disconnect (RT_CONNECT_ID id);
	CRt_Sim(void):
	m_lFreq(0),
	m_nRequestCount(0),
	m_bConnected(false),
	m_lSymbolCount(0)
	{
		m_hEventStop = CreateEvent(NULL,TRUE,FALSE,NULL);
		m_hEventGetSymbol = CreateEvent(NULL,FALSE,FALSE,NULL);
		m_hEventDataReady = CreateEvent(NULL,FALSE,FALSE,NULL);
		m_hEventMindStock = CreateEvent(NULL,FALSE,FALSE,NULL);
		m_hEventMindOption = CreateEvent(NULL,FALSE,FALSE,NULL);
		m_hEventGroup = CreateEvent(NULL,FALSE,FALSE,NULL);

		m_hEventMindPBStock = CreateEvent(NULL,FALSE,FALSE,NULL);
		m_hEventMindPBOption = CreateEvent(NULL,FALSE,FALSE,NULL);

		m_hEventMindUnBlock = CreateEvent(NULL, TRUE, FALSE, NULL );
		m_hEventPlayBackUnBlock = CreateEvent(NULL, TRUE, FALSE, NULL );

	};

	~CRt_Sim()
	{
		SetEvent(m_hEventStop);

		CloseHandle(m_hEventGetSymbol);
		CloseHandle(m_hEventStop);
		CloseHandle(m_hEventDataReady);
		CloseHandle(m_hEventMindStock);
		CloseHandle(m_hEventMindOption);
		CloseHandle(m_hEventMindPBStock);
		CloseHandle(m_hEventMindPBOption);
		CloseHandle(m_hEventGroup);
	}
	RT_ERR Connect(RT_CONNECT_ID*, RT_STARTUP_CLBK);

	RT_ERR SetShutdownNotifier(RT_CONNECT_ID rt_id, 
		 					   RT_SHUTDOWN_CLBK shutdown_clbk);

	RT_ERR RequestSymbol(RT_CONNECT_ID	rt_id,
						RT_REQ_ID*	    req_id_ptr_to_fill,
						void*			user_data,
						RT_REQ_TYPE		req_type,
						RT_SYMBOL*		symbol,
						int             field_count,
						RT_FIELD*       fields,
				  RT_GET_FIELDS_CLBK    callback);


	RT_ERR RequestHandle(RT_CONNECT_ID	rt_id,
						RT_REQ_ID*	    req_id_ptr_to_fill,
						void*			user_data,
						RT_REQ_TYPE		req_type,
						RT_HANDLE		handle,
						int             field_count,
						RT_FIELD*       fields,
				  RT_GET_FIELDS_CLBK    callback);

	RT_ERR RequestGroup(
		  RT_CONNECT_ID                   rt_id , // RT connect id
		  RT_REQ_ID *        req_id_ptr_to_fill , // Returned address of a req_entry to        
		  void  *                     user_data , // Pointer to any user data. This pointer, 
		  RT_GRP_REQ_TY              group_type , // Type of GROUP for which information is 
		  RT_HANDLE                      handle , // Root symbol (i.e. ticker, future family)    
		  RT_GET_GROUP_CLBK            callback); // Address of callback routine which gets  

	RT_ERR RemoveFields(RT_CONNECT_ID rt_id, RT_REQ_ID req_id, int field_count, RT_FIELD* pFields);
	RT_ERR AddFields(RT_CONNECT_ID rt_id, RT_REQ_ID req_id, int field_count, RT_FIELD* pFields);
	RT_ERR RemoveSymbols(RT_CONNECT_ID rt_id);

private:
	static UINT WINAPI SymbolResponce(LPVOID pData);
	static UINT WINAPI GroupResponce(LPVOID pData);

	static UINT WINAPI SymbolGet(LPVOID pData);
	
	static UINT WINAPI SymbolMind(LPVOID pData);
	static UINT WINAPI OptionMind(LPVOID pData);
	static UINT WINAPI PlaybackMind(LPVOID pData);

	bool  FillSymbol(CSymbol& symbol, CClientRecordset& recSet);
	bool  FillFields(long lID, CRequest& request, CSymbol& symbol, CSymbolResponce& responce);
	void  Simulation(CSymbol& symbol);
	
	double RandomPercent(double persent)
	{
		double dRet = 1;
		double dPersent = GetRandomValue(long(-persent*10000), long(persent*10000));							
		dPersent /= 1000000;
		dRet = 1. + dPersent;
		if(dRet <= 0) 
			return 1;
		else
			return dRet;
	}

private:
	long		 m_nRequestCount;
	long		 m_lFreq;
	bool	     m_bConnected; 	

	HANDLE       m_hEventMindUnBlock;
	HANDLE       m_hEventPlayBackUnBlock;

	HANDLE       m_hEventStop;
	HANDLE       m_hEventGetSymbol;
	HANDLE       m_hEventDataReady;
	HANDLE		 m_hEventMindStock;
	HANDLE		 m_hEventMindOption;

	HANDLE		 m_hEventMindPBStock;
	HANDLE		 m_hEventMindPBOption;

	HANDLE		 m_hEventGroup;

	GETREQ       m_GetRequest;
	
	MINDREQ      m_MindStockRequest;
	MINDREQ      m_MindOptionRequest;

	GETREQ		 m_GroupRequest;
	EgLib::CLock m_csResponce;
	EgLib::CLock m_csMind;
	EgLib::CLock m_csRequest;
	RESPQU       m_Responce;

	SYMBOLS      m_Symbols;
	SYMBOLMAP    m_SymbolID;

	long		 m_lSymbolCount;

	SP::ISimParametersPtr  m_spOptions;

	bool InitializeParameters()
	{
		if(m_spOptions == NULL)
		{
			HRESULT hr = m_spOptions.CreateInstance(__uuidof(SP::SimParameters));
			if(FAILED(hr)) return false;
		}
		return true;
	}

};



#endif //_RT_SIM_H 