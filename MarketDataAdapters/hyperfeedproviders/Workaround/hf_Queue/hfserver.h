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
#include <mq.h>

class CPrice;
class CHFServer;
typedef CEgTaskBase<CHFServer>  CEngineTaskBase;

struct DerivativeData
{
	char  symbol[12];
	FLOAT bidprice;
	FLOAT askprice;
	FLOAT lastprice; 
	DATE   bidDate;
	DATE   askDate;
	DATE   lastDate;
	LONG bidsize;
	LONG asksize;
	LONG lastsize;
	char bidexch;
	char askexch;
	char lastexch;
	LONG volume;
};

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
	{
		if(*sKey.c_str() == '.' && sKey.length()<6)
		{
            char buffer[10];
			const char* pBuf = sKey.c_str();
			buffer[6] = 0;
			buffer[5] = pBuf[sKey.length() - 1];
			buffer[4] = pBuf[sKey.length() - 2];
			if(sKey.length()==5)
			{
				buffer[3] = ' ';
				buffer[2] = pBuf[2];
				buffer[1] = pBuf[1];
				buffer[0] = pBuf[0];
				m_sKeyString = buffer;
			}else
			if(sKey.length()==4)
			{
				buffer[3] = ' ';
				buffer[2] = ' ';
				buffer[1] = pBuf[1];
				buffer[0] = pBuf[0];
				m_sKeyString = buffer;
			}else   
				m_sKeyString = sKey;  
		}                                          
		m_lHashValue= CalcHashCode(m_sKeyString); 
	}

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
		for(; itr!=itrEnd; ++itr)
		{
			x1+=8*x2 + 0xe5fc82a9;
			x2+=8*x1 + *itr;
		}
		return x1+x2;
	}

private:
	CHahedKeyString():m_lHashValue(0){}
};

typedef boost::shared_ptr<CHahedKeyString> CHahedKeyStringPtr;


class CEngineQuoteMessage: public CEngineTaskBase
{
public:
	virtual void DoTask(LPVOID, LPOVERLAPPED);  

	CEngineQuoteMessage(CHFServer* pHF, CHahedKeyStringPtr pKey, DerivativeData* pRecord): CEngineTaskBase(pHF), m_Key(pKey)
	{
		::memcpy(&m_record, pRecord, sizeof(DerivativeData));
	};

private:
	CHahedKeyStringPtr  m_Key;
	DerivativeData        m_record;

};


class CEngineMessage: public CEngineTaskBase
{
public:
	virtual void DoTask(LPVOID, LPOVERLAPPED);  
    CEngineMessage(CHFServer* pHF, boost::shared_array<BYTE> pMessage, DWORD dwSize):CEngineTaskBase(pHF), m_pMessage(pMessage),m_dwSize(dwSize)
	{
		m_dwStart = GetTickCount();
	};

private:
	boost::shared_array<BYTE> m_pMessage;
	DWORD m_dwSize;
	DWORD m_dwStart;
};

class CEngineNotificationMessage: public CEngineTaskBase
{
public:
	virtual void DoTask(LPVOID, LPOVERLAPPED);  

	CEngineNotificationMessage(CHFServer* pHF, ErrorNumberEnum enErrorType, std::string& sText):CEngineTaskBase(pHF), m_enErrorType(enErrorType),m_strText(sText){};

private:
	ErrorNumberEnum  m_enErrorType;
	std::string      m_strText;

};


class CEngineSubscribe: public CEngineTaskBase
{
public:
	virtual void DoTask(LPVOID, LPOVERLAPPED);  

	CEngineSubscribe(CHFServer* pHF, CPrice* pPrice):CEngineTaskBase(pHF), m_pPrice(pPrice){};

private:
	CPrice* m_pPrice;
};
class CEngineUnsubscribe: public CEngineTaskBase
{
public:
	virtual void DoTask(LPVOID, LPOVERLAPPED);  

	CEngineUnsubscribe(CHFServer* pHF, CPrice* pPrice): CEngineTaskBase(pHF), m_pPrice(pPrice){};

private:
	CPrice* m_pPrice;
};

class CEngineError: public CEngineTaskBase
{
public:
	virtual void DoTask(LPVOID, LPOVERLAPPED);  

	CEngineError(CHFServer* pHF, long lStatus): CEngineTaskBase(pHF), m_lStatus(lStatus){};

private:
	long m_lStatus;
};

//***********************************************************************************************

class CHFServer:  
	protected EgLib::CThread
{
	typedef std::set<CPrice*> subset;
	typedef CEgThreadPool<CHFServer>  CHFServerThreadPool;

public:
	CHFServer();
	~CHFServer();

	EgLib::CLock m_csQueue;
	static LARGE_INTEGER m_lPerformanceFrequeny;

	static ULONG  m_lClients;
	static ULONG  m_lRTClients;
	static volatile LONG  m_lRTResponse;

protected:
	long m_nStatus;
	bool m_bTerminate;
	HANDLE m_hThread;
	
	CHandle m_hEventStop;
	long m_nCount;

	//	SubscriptionStrategy m_Strategy;

	char m_etiPblk[256];
	ETI_TABLE* m_etiPtr;

	subset m_setSubs;

protected:
	virtual UINT Run();

	void Terminate();
	static PerfMon::IPPBatchInfoPtr m_spBatchPerfMon;
public:
	static PerfMon::IPPBatchInfoPtr GetGlobalPerfMonitor();
public:
	long Initialize();
	long Uninitialize();
	long GetStatus();

	void Subscribe(CPrice* pSub);
	void Unsubscribe(CPrice* pSub);


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
public:
	// Events
	void OnSubscribe(CPrice* pPrice);
	void OnUnsubscribe(CPrice* pPrice);
	void OnError(long lStatus);
	void OnNotificationMessage(ErrorNumberEnum enType, std::string& sText);
	void OnQuoteMessage(CHahedKeyStringPtr pKey, DerivativeData* pRecord);
	void OnQuoteMessage(boost::shared_array<BYTE> pMessage, DWORD dwSize, DWORD dwTicks);

public:
	const std::string& GetSymbolFilter() const {return m_sFilter;}
	void SetSymbolFilter(std::string sFilter ){m_sFilter = sFilter; }

public:
	HRESULT OpenQueue(DWORD dwAccess, DWORD dwShareMode);
	HRESULT CloseQueue();
	HRESULT GetQueueMessage(DWORD dwTimeOut, string& label, BYTE* pBody, DWORD* dwSize);
	HRESULT GetMessageSize(DWORD dwTimeOut, DWORD* dwSize);
        
private:
	CComPtr<ISettingsManager> m_spSettingsManager;
	std::string m_sFilter;
	CThreadPool<CHFServerThreadPool>    m_Pool;  
	CThreadPool<CHFServerThreadPool>    m_PoolQueueProcessing;  

	MQMSGPROPS     msgprops;     
	MQPROPVARIANT  aPropVar[20];     
	QUEUEPROPID    aqPropId[20];     
	MSGPROPID      amPropId[20]; 

	QUEUEHANDLE         m_hQueue; 
	long volatile		m_lProcessedQuotes;
};

inline long CHFServer::GetStatus()
{
	return m_nStatus;
}

#endif // __HFSERVER_H__
