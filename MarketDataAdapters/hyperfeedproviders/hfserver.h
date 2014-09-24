// HFServer.h: interface for the CHFServer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __HFSERVER_H__
#define __HFSERVER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "common.h"
#include <Boost/noncopyable.hpp>
#include "Utilities.h"


class CPrice;
class CHFServer;
typedef CEgTaskBase<CHFServer>  CEngineTaskBase;
typedef boost::shared_ptr<DBA_RECORD_3> CDBARecordPtr;
class CHFServer;

struct CHahedKeyString : public boost::noncopyable
{
public:
	enum KeyType {enKeyComposite, enKeyRegional} ;
	CHahedKeyString(DBA_KEY* pKey)
	{
		if(pKey)
		{
			char szKey[MAX_TICKER*2+1];
			szKey[MAX_TICKER*2] = 0;
			CUtilities::KeyToString(pKey, szKey, MAX_TICKER*2);

			m_sKeyString = szKey;
			m_lHashValue= CalcHashCode(m_sKeyString); 
			memcpy(&m_rawKey, pKey, sizeof(m_rawKey));
		}
	}

	CHahedKeyString(std::string& sKey): m_sKeyString(sKey)
	{ m_lHashValue= CalcHashCode(m_sKeyString); }

	void  				GetRawKey(DBA_KEY *key) const 
	{
		if(key)
			memcpy(key,&m_rawKey, sizeof(DBA_KEY));
	}

	const std::string&	GetKeyString() const {return m_sKeyString;}
	const size_t		GetHashCode() const {return m_lHashValue;}
private:
	size_t			m_lHashValue;
	std::string		m_sKeyString;
	KeyType         m_enKeyType;
	DBA_KEY         m_rawKey;

	size_t CalcHashCode(std::string& _Keyval)
	{
		UINT x1=0x14f2affc;
		UINT x2=0xfac5bd8f;
		string::const_iterator itr = _Keyval.begin();
		string::const_iterator itrEnd = _Keyval.end();
		LPCSTR pBuffer = _Keyval.c_str();
		for(int i=0; itr!=itrEnd; ++itr)
		{
			x1+=8*x2 + 0xe5fc82a9;
			x2+=8*x1 + pBuffer[i++];//*itr;
		}
		return x1+x2;
	}

private:
	CHahedKeyString():m_lHashValue(0){}
};

typedef boost::shared_ptr<CHahedKeyString> CHahedKeyStringPtr;

//----------------------------------------------------------------------------------------------------------------------------
class CEngineQuoteMessage: public CStrategyPoolObjectBase<CHFServer>
{
public:
	virtual void DoTask();  

	CEngineQuoteMessage(CHFServer* pHF, CHahedKeyStringPtr pKey, DBA_RECORD_3* pRecord):
	CStrategyPoolObjectBase<CHFServer>(pHF), m_Key(pKey)
	{

		m_record = CDBARecordPtr(new DBA_RECORD_3);
		::memcpy(m_record.get(), pRecord, sizeof(DBA_RECORD_3));
	};

private:
	CHahedKeyStringPtr  m_Key;
	CDBARecordPtr       m_record;

};


class CEngineNotificationMessage: public CStrategyPoolObjectBase<CHFServer>
{
public:
	virtual void DoTask();  

	CEngineNotificationMessage(CHFServer* pHF, ErrorNumberEnum enErrorType, std::string& sText):
		CStrategyPoolObjectBase<CHFServer>(pHF), m_enErrorType(enErrorType),m_strText(sText){};

private:
	ErrorNumberEnum  m_enErrorType;
	std::string      m_strText;

};


class CEngineError: public CStrategyPoolObjectBase<CHFServer>
{
public:
	virtual void DoTask();  

	CEngineError(CHFServer* pHF, long lStatus): CStrategyPoolObjectBase<CHFServer>(pHF), m_lStatus(lStatus){};

private:
	long m_lStatus;
};

//***********************************************************************************************
class CHFServerService   :
	protected EgLib::CThread
{
public:
	CHFServerService(CHFServer* pServer): m_pServer(pServer){}

	DWORD Start() {return EgLib::CThread::Start();}
	void  Stop(){ EgLib::CThread::Stop();}
protected:
	virtual UINT Run();


private:
	CHFServer* m_pServer;
};
class CHFServer;


#ifdef __PERFTEST

class CDataDlowEmulator:
	public CThread
{
public:
	CDataDlowEmulator(CHFServer* pServer):CThread(),m_pServer(pServer){}
	virtual UINT Run();
	CHFServer* m_pServer;
};

class CEnginePerfTestMessage: public CStrategyPoolObjectBase<CHFServer>
{
public:
	CEnginePerfTestMessage(CHFServer* pServer):CStrategyPoolObjectBase<CHFServer>(pServer){}
	virtual void DoTask();  

};

#endif // __PERFTEST

//***********************************************************************************************
typedef CStrategyPoolT<CComPoolThreadT<CHFServer> >  CHFServerThreadPool;

class CHFServer:  
	protected EgLib::CThread
{
	typedef std::set<CPrice*> subset;

public:
	CHFServer();
	~CHFServer();

	EgLib::CLock m_csQueue;
#ifdef __PERFMON
	static LARGE_INTEGER m_lPerformanceFrequeny;
#endif // __PERFMON

	static ULONG  m_lClients;
	static ULONG  m_lRTClients;
	static volatile LONG  m_lRTResponse;

	CHandle  m_hHasSubscriptions;

protected:
	long m_nStatus;
	bool m_bTerminate;
	CHandle m_hThread;
	//CHandle m_hEventStop;
	long m_nCount;
	bool m_bConnected;


//	SubscriptionStrategy m_Strategy;

	char m_etiPblk[256];
	ETI_TABLE* m_etiPtr;

	subset m_setSubs;
private:
	CHFServerService  m_Service;
protected:
	virtual UINT Run();

	void Terminate();
#ifdef __PERFMON
	static PerfMon::IPPBatchInfoPtr m_spBatchPerfMon;
#endif // _DEBUG

	bool Kill(DWORD dwID);

#ifdef __PERFMON
public:
	static PerfMon::IPPBatchInfoPtr GetGlobalPerfMonitor();
#endif // _DEBUG

public:
	long Initialize();
	long Uninitialize();
	long GetStatus();
	bool IsTerminate() const {return m_bTerminate;}
	bool IsConnected() const {return m_bConnected;}

	void Subscribe(CPrice* pSub);
	void Unsubscribe(CPrice* pSub);
	long GetThreadPoolCapacity( ) const {return static_cast<long>(m_Pool.GetSize());}
	void SetThreadPoolCapacity(long lNewSize)
	{ 
		long lOldSize = GetThreadPoolCapacity(); 
		m_Pool.SetSize(lNewSize);
		if(lNewSize > 0)
			EgLib::CEgLibTraceManager::Trace(LogWarning, _T("") , _T("Thread Pool Capacity\t Changed from %d to %d"),lOldSize ,lNewSize);	
		else
			EgLib::CEgLibTraceManager::Trace(LogWarning, _T("") , _T("Thread Pool Capacity\t Changed from %d to (%d)%d"),lOldSize ,GetThreadPoolCapacity(), lNewSize);	
	}

	//void OnData(CSubsData& data);
	HRESULT OnStrategyInitialize()
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if(SUCCEEDED(hr))
			m_spSettingsManager.CoCreateInstance(__uuidof(SettingsManager));
		return hr;							  
	}

	void    OnStrategyUninitialize()
	{
		m_spSettingsManager = NULL;
		 CoUninitialize();
	}
private:
	CHandle GetProcessId(std::string& sProcessName, DWORD& dwProcessID);
	void CloseHFProcess(LPCSTR szProcessName);

public:
	// Events
	void OnError(long lStatus);
	void OnNotificationMessage(ErrorNumberEnum enType, std::string& sText);
	void OnQuoteMessage(CHahedKeyStringPtr pKey, CDBARecordPtr pRecord);
	void OnIdle(bool bForce);

#ifdef __PERFTEST
	void OnPerfTest();
#endif

public:
	const std::string& GetSymbolFilter() const {return m_sFilter;}
	void SetSymbolFilter(std::string sFilter ){m_sFilter = sFilter; }


private:
	CComPtr<ISettingsManager> m_spSettingsManager;
	std::string m_sFilter;

	CFairSingleWriterMultipleReaderGuard  m_WRcsSubscscription;

#ifdef __PERFTEST
	 CDataDlowEmulator m_DataSimulator;
#endif

public:
	long volatile m_lProcessedQuotes;
	long volatile m_lCompositeUpdates;
	long volatile m_lRegionalUpdates;
	long volatile m_lHeartbeat;
	long volatile m_lPeriodic;

	CLock								m_csInitialize;
	CHFServerThreadPool				    m_Pool;  

	//std::map<UINT, DWORD>  m_threadPoolStatistics;
	//std::vector<DWORD>     m_vecThreadPoolStatistics;

};

inline long CHFServer::GetStatus()
{
	return m_nStatus;
}

#endif // __HFSERVER_H__
