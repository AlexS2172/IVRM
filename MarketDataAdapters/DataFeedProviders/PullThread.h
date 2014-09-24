#include "stdafx.h"
//#include "DataFeedBaseClass.h"

//class CStopEvent
//{
//public:
//	static bool CreateEvent()
//	{
//		if (!hEvent)
//		{
//			hEvent = ::CreateEvent(NULL,TRUE,FALSE,NULL);
//			if (hEvent)
//				return true;
//		}
//		return false;
//	}
//
//	static void CloseEvent()
//	{
//		if (hEvent)
//		{
//			CloseHandle(hEvent);
//			hEvent = NULL;
//		}
//	}
//	static BOOL SetEvent()
//	{
//		return ::SetEvent(hEvent);
//	}
//	static bool HasSignaledState()
//	{
//		return (::WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0);
//	}
//	static HANDLE GetHandle() { return hEvent; }
//
//
//	static HANDLE hEvent;
//};

//class CWorkerInterface
//{
//protected:
//	CORBA::Any							m_Data;
//	CORBA::Short						m_subsType;
//	QuoteUpdateMap						m_requestData;
//	RequestMap							m_requestTypedData;
//	RequestType							m_reqType;	
//public:
//	virtual void DoTask() = 0;
//	virtual ~CWorkerInterface(){};
//	static CORBA::Short GetType(InstrumentTypeEnum Type);
//	void QuotesToAnyData(vector<QuoteUpdateParams> &requestData);
//};
//
//
//class CPullThreadPool
//{
//public:
//	typedef DWORD_PTR RequestType;
//
//	CPullThreadPool(void){};
//	virtual ~CPullThreadPool(void){};
//
//	virtual BOOL Initialize(void* /*pvParam*/)
//	{
//		return SUCCEEDED(::CoInitializeEx(NULL, COINIT_MULTITHREADED))?TRUE:FALSE;
//	}
//
//	virtual void Terminate(void* /*pvParam*/)
//	{
//		::CoUninitialize();
//	}
//
//	void Execute(RequestType dw, void* /*pvParam*/, OVERLAPPED* /*pOverlapped*/) throw()
//	{
//		CWorkerInterface* pTask = reinterpret_cast<CWorkerInterface*>(dw);
//		try
//		{
//			pTask->DoTask();
//		}
//		catch (...)
//		{
//		}
//		delete pTask;
//	};
//};
//
//class CStockMsgWorker : public CWorkerInterface
//{
//public:
//	CStockMsgWorker(DDS::IVDataManager_var dataManager, CDataFeedBaseClass* pClient,  vector<QuoteUpdateParams> &requestData, RequestType reqType);
//	virtual void DoTask();
//	virtual ~CStockMsgWorker();
//
//private:
//	CORBA::Any							m_anyFields;
//	CDataFeedBaseClass*					m_pClient;
//	DDS::IVDataManager_var				m_dataManager;
//};
//
//class COptionMsgWorker : public CWorkerInterface
//{
//public:
//	COptionMsgWorker(DDS::IVDataManager_var dataManager, CDataFeedBaseClass* pClient, vector<QuoteUpdateParams> &requestData, RequestType reqType);
//	virtual void DoTask();
//	virtual ~COptionMsgWorker();
//private:
//	CORBA::Any							m_anyFields;
//	CDataFeedBaseClass*					m_pClient;
//	DDS::IVDataManager_var				m_dataManager;
//};
//
//class CSubscriptionMsgWorker : public CWorkerInterface
//{
//public:
//	CSubscriptionMsgWorker(DDS::IVDataManager_var dataManager, CDataFeedBaseClass* pClient, CRequestColl& request, bool subscribe, int subsType = -1);
//	virtual void DoTask();
//	virtual ~CSubscriptionMsgWorker();
//private:
//	CORBA::Any							m_anyFields;
//	CDataFeedBaseClass*					m_pClient;
//	DDS::IVDataManager_var				m_dataManager;
//	bool								m_Subscribe;
//	CORBA::Short						m_symbType;
//	CRequestColl&						m_request;
//};


