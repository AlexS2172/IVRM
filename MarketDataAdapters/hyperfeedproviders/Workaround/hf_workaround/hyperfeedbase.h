// HyperFeedBase.h: interface for the CHyperFeedBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HYPERFEEDBASE_H__17F312E7_8CEE_4B21_A5DD_B2822DE6E631__INCLUDED_)
#define AFX_HYPERFEEDBASE_H__17F312E7_8CEE_4B21_A5DD_B2822DE6E631__INCLUDED_
#include "Process.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Common.h"

//////////////////////////////////////////////////////////////////////////////
enum RequestType
{
	_enRequestNone,
    _enRequestStock,
	_enRequestOption,
	_enRequestFuture,
	_enRequestFutures,
	_enRequestFuturesOption,
	_enRequestLast,
	_enRequestRealtime,
	_enRequestGroup
};
//////////////////////////////////////////////////////////////////////////////
class CRequestBase
{
public:
	CRequestBase() : m_enType(_enRequestNone){}
	virtual ~CRequestBase(){}

	RequestType m_enType;
};

//////////////////////////////////////////////////////////////////////////////
class CQuoteRequest : public CRequestBase
{
public:
	CQuoteRequest(RequestType enType, QuoteUpdateParams* Params) { m_enType = enType; m_recParams.Init(Params);}
	_QuoteUpdateParams	m_recParams;
};

//////////////////////////////////////////////////////////////////////////////
class CGroupRequest : public CQuoteRequest
{
public:
	CGroupRequest(GroupRequestType enGroupType, QuoteUpdateParams* Params) : CQuoteRequest(_enRequestGroup, Params), m_enGroupRequestType(enGroupType){}
	GroupRequestType m_enGroupRequestType;
};
//////////////////////////////////////////////////////////////////////////////
class CStockRequest : public CRequestBase
{
public:
	CStockRequest(StockParams* Params) { m_enType = _enRequestStock; m_recParams.Init(Params);}
	_StockParams	m_recParams;
};

//////////////////////////////////////////////////////////////////////////////
class COptionRequest : public CRequestBase
{
public:
	COptionRequest(OptionParams* Params) { m_enType = _enRequestOption; m_recParams.Init(Params);}
	_OptionParams	m_recParams;
};

//////////////////////////////////////////////////////////////////////////////
class CFutureRequest : public CRequestBase
{
public:
	CFutureRequest(FutureParams* Params) { m_enType = _enRequestFuture; m_recParams.Init(Params);}
	_FutureParams	m_recParams;
};

//////////////////////////////////////////////////////////////////////////////
class CFuturesRequest : public CRequestBase
{
public:
	CFuturesRequest(FutureRootParams* Params) { m_enType = _enRequestFutures; m_recParams.Init(Params);}
	_FutureRootParams	m_recParams;
};

//////////////////////////////////////////////////////////////////////////////
class CFuturesOptionRequest : public CRequestBase
{
public:
	CFuturesOptionRequest(FuturesOptionParams* Params) { 
		m_enType = _enRequestFuturesOption; 
		m_recParams.Init(Params);
		if(!Params->Exchange)
			m_recParams->Exchange = ::SysAllocString(L"");
	}
	_FuturesOptionParams	m_recParams;
};

//////////////////////////////////////////////////////////////////////////////
class CDataForIvRequest : public CRequestBase
{
public:
	CDataForIvRequest() { m_enType = _enRequestOption;}
};
//////////////////////////////////////////////////////////////////////////////
enum ResponseType
{
	_enResponseNone,
	_enResponseError,
	_enResponseStock,
	_enResponseOption,
	_enResponseFuture,
	_enResponseFutures,
	_enResponseFuturesOption,
	_enResponseLast,
	_enResponseRealtime,
	_enResponseRealtimeNotification
};
//////////////////////////////////////////////////////////////////////////////
class CResponseBase
{
public:
	CResponseBase() : m_enType(_enResponseNone)
	{
		//QueryPerformanceCounter(&m_enTime);
	}
	virtual ~CResponseBase(){}

	ResponseType   m_enType;
	const LARGE_INTEGER& GetPerformanceCounter() const {return m_enTime;}
	void SetPerformanceCounter(LARGE_INTEGER& li){m_enTime.QuadPart = li.QuadPart;}
private:
	LARGE_INTEGER  m_enTime;
};

//////////////////////////////////////////////////////////////////////////////
class CErrorResponse : public CResponseBase
{
public:
	CErrorResponse() : m_Error(enUnknown), m_bstrDescription(L""), 
		m_enRequestType(enNoRequest), m_vtRequest(vtMissing) {m_enType = _enResponseError;}
	virtual ~CErrorResponse(){}

	ErrorNumberEnum			m_Error;
	_bstr_t					m_bstrDescription;	
	RequestsTypeEnum		m_enRequestType;
	CComVariant				m_vtRequest;
};

//////////////////////////////////////////////////////////////////////////////
class CQuoteInfoResponse : public CResponseBase
{
public:
	CQuoteInfoResponse(ResponseType enType, const _QuoteUpdateParams* pParams, const _QuoteUpdateInfo& Info ) 
	{ 
		m_enType = enType; 
		const_cast<_QuoteUpdateParams*>(pParams)->CopyTo(m_vtParams); 
		const_cast<_QuoteUpdateInfo&>(Info).CopyTo(m_vtInfo); 
	}
	CComVariant	m_vtParams;
	CComVariant	m_vtInfo;
};

//////////////////////////////////////////////////////////////////////////////
class CQuoteResultResponse : public CResponseBase
{
public:
	CQuoteResultResponse(ResponseType enType, const _QuoteUpdateParams* pParams, const _QuoteUpdateResult& Results) 
	{ 
		m_enType = enType; 
		const_cast<_QuoteUpdateParams*>(pParams)->CopyTo(m_vtParams); 
		const_cast<_QuoteUpdateResult&>(Results).CopyTo(m_vtResult); 
	}
	CComVariant	m_vtParams;
	CComVariant	m_vtResult;
};

//////////////////////////////////////////////////////////////////////////////
class CStockResultResponse : public CResponseBase
{
public:
	CStockResultResponse(const _StockParams& Params, const _StockResults& Result) 
	{ 
		m_enType = _enResponseStock; 
		const_cast<_StockParams&>(Params).CopyTo(m_vtParams); 
		const_cast<_StockResults&>(Result).CopyTo(m_vtResult);
	}
	CComVariant		m_vtParams;
	CComVariant		m_vtResult;
};

//////////////////////////////////////////////////////////////////////////////
class CStockResultExResponse : public CResponseBase
{
public:
	CStockResultExResponse(const _StockParams& Params, const _StockResultsEx& Result) 
	{ 
		m_enType = _enResponseStock; 
		const_cast<_StockParams&>(Params).CopyTo(m_vtParams); 
		const_cast<_StockResultsEx&>(Result).CopyTo(m_vtResult);
	}
	CComVariant		m_vtParams;
	CComVariant		m_vtResult;
};

//////////////////////////////////////////////////////////////////////////////
class CStockInfoResponse : public CResponseBase
{
public:
	CStockInfoResponse(const _StockParams& Params, const _StockInfo& Info) 
	{ 
		m_enType = _enResponseStock; 
		const_cast<_StockParams&>(Params).CopyTo(m_vtParams); 
		const_cast<_StockInfo&>(Info).CopyTo(m_vtInfo);
	}
	CComVariant		m_vtParams;
	CComVariant		m_vtInfo;
};

//////////////////////////////////////////////////////////////////////////////
class COptionResultResponse : public CResponseBase
{
public:
	COptionResultResponse(const _OptionParams& Params, const _OptionResults& Result, bool IsLast = false) : m_bIsLast(IsLast) 
	{ 
		m_enType = _enResponseOption; 
		const_cast<_OptionParams&>(Params).CopyTo(m_vtParams); 
		const_cast<_OptionResults&>(Result).CopyTo(m_vtResult);
	}
	CComVariant		m_vtParams;
	CComVariant		m_vtResult;
	bool			m_bIsLast;
};

//////////////////////////////////////////////////////////////////////////////
class COptionResultExResponse : public CResponseBase
{
public:
	COptionResultExResponse(const _OptionParams& Params, const _OptionResultsEx& Result, bool IsLast = false) : m_bIsLast(IsLast) 
	{ 
		m_enType = _enResponseOption; 
		const_cast<_OptionParams&>(Params).CopyTo(m_vtParams); 
		const_cast<_OptionResultsEx&>(Result).CopyTo(m_vtResult);
	}
	CComVariant		m_vtParams;
	CComVariant		m_vtResult;
	bool			m_bIsLast;
};

//////////////////////////////////////////////////////////////////////////////
class COptionInfoResponse : public CResponseBase
{
public:
	COptionInfoResponse(const _OptionParams& Params, const _OptionInfo& Info, bool IsLast = false) : m_bIsLast(IsLast) 
	{ 
		m_enType = _enResponseOption; 
		const_cast<_OptionParams&>(Params).CopyTo(m_vtParams); 
		const_cast<_OptionInfo&>(Info).CopyTo(m_vtInfo);
	}
	CComVariant		m_vtParams;
	CComVariant		m_vtInfo;
	bool			m_bIsLast;
};

//////////////////////////////////////////////////////////////////////////////
class CFutureResultResponse : public CResponseBase
{
public:
	CFutureResultResponse(const _FutureParams& Params, const _FutureResults& Result) 
	{ 
		m_enType = _enResponseFuture; 
		const_cast<_FutureParams&>(Params).CopyTo(m_vtParams); 
		const_cast<_FutureResults&>(Result).CopyTo(m_vtResult);
	}
	CComVariant		m_vtParams;
	CComVariant		m_vtResult;
};

//////////////////////////////////////////////////////////////////////////////
class CFutureResultExResponse : public CResponseBase
{
public:
	CFutureResultExResponse(const _FutureParams& Params, const _FutureResultsEx& Result) 
	{ 
		m_enType = _enResponseFuture; 
		const_cast<_FutureParams&>(Params).CopyTo(m_vtParams); 
		const_cast<_FutureResultsEx&>(Result).CopyTo(m_vtResult);
	}
	CComVariant		m_vtParams;
	CComVariant		m_vtResult;
};

//////////////////////////////////////////////////////////////////////////////
class CFuturesResultResponse : public CResponseBase
{
public:
	CFuturesResultResponse(const _FutureRootParams& Params, const _FutureResults& Result, bool IsLast = false) : m_bIsLast(IsLast)  
	{ 
		m_enType = _enResponseFutures; 
		const_cast<_FutureRootParams&>(Params).CopyTo(m_vtParams); 
		const_cast<_FutureResults&>(Result).CopyTo(m_vtResult);
	}
	CComVariant		m_vtParams;
	CComVariant		m_vtResult;
	bool			m_bIsLast;
};

//////////////////////////////////////////////////////////////////////////////
class CFuturesResultExResponse : public CResponseBase
{
public:
	CFuturesResultExResponse(const _FutureRootParams& Params, const _FutureResultsEx& Result, bool IsLast = false) : m_bIsLast(IsLast)  
	{ 
		m_enType = _enResponseFutures; 
		const_cast<_FutureRootParams&>(Params).CopyTo(m_vtParams); 
		const_cast<_FutureResultsEx&>(Result).CopyTo(m_vtResult);
	}
	CComVariant		m_vtParams;
	CComVariant		m_vtResult;
	bool			m_bIsLast;
};

//////////////////////////////////////////////////////////////////////////////
class CFutureOptionResultResponse : public CResponseBase
{
public:
	CFutureOptionResultResponse(const _FuturesOptionParams& Params, const _FuturesOptionResults& Result, bool IsLast = false) : m_bIsLast(IsLast)  
	{ 
		m_enType = _enResponseFuturesOption; 
		const_cast<_FuturesOptionParams&>(Params).CopyTo(m_vtParams); 
		const_cast<_FuturesOptionResults&>(Result).CopyTo(m_vtResult);
	}
	CComVariant		m_vtParams;
	CComVariant		m_vtResult;
	bool			m_bIsLast;
};

//////////////////////////////////////////////////////////////////////////////
class CFutureOptionResultExResponse : public CResponseBase
{
public:
	CFutureOptionResultExResponse(const _FuturesOptionParams& Params, const _FuturesOptionResultsEx& Result, bool IsLast = false) : m_bIsLast(IsLast)  
	{ 
		m_enType = _enResponseFuturesOption; 
		const_cast<_FuturesOptionParams&>(Params).CopyTo(m_vtParams); 
		const_cast<_FuturesOptionResultsEx&>(Result).CopyTo(m_vtResult);
	}
	CComVariant		m_vtParams;
	CComVariant		m_vtResult;
	bool			m_bIsLast;
};

//////////////////////////////////////////////////////////////////////////////
class CSubscribedResponse : public CResponseBase
{
public:
	CSubscribedResponse(const _QuoteUpdateParams& Params)
	{ 
		m_enType = _enResponseRealtimeNotification; 
		const_cast<_QuoteUpdateParams&>(Params).CopyTo(m_vtParams); 
	}
	CComVariant	m_vtParams;
};

//////////////////////////////////////////////////////////////////////////////
class COptionForIvResponse : public CResponseBase
{
public:
	COptionForIvResponse(const _OptionForIvResult& Result, bool IsLast = false) : m_bIsLast(IsLast)
	{ 
		m_enType = _enResponseOption; 
		const_cast<_OptionForIvResult&>(Result).CopyTo(m_vtResult);
	}
	CComVariant		m_vtResult;
	bool			m_bIsLast;
};

//////////////////////////////////////////////////////////////////////////////
class CStockForIvResponse : public CResponseBase
{
public:
	CStockForIvResponse(const _StockForIvResult& Result)
	{ 
		m_enType = _enResponseStock; 
		const_cast<_StockForIvResult&>(Result).CopyTo(m_vtResult);
	}
	CComVariant		m_vtResult;
};

//////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<CRequestBase>  CRequestBasePtr;
typedef boost::shared_ptr<CResponseBase> CResponseBasePtr;

typedef boost::shared_ptr<CSubscribedResponse>		CSubscribedResponsePtr;
typedef boost::shared_ptr<CErrorResponse>			CErrorResponsePtr;
typedef boost::shared_ptr<CFuturesResultResponse>	CFuturesResultResponsePtr;
typedef boost::shared_ptr<CFuturesResultExResponse>	CFuturesResultExResponsePtr;
typedef boost::shared_ptr<CFutureResultResponse>	CFutureResultResponsePtr;
typedef boost::shared_ptr<CFutureResultExResponse>	CFutureResultExResponsePtr;
typedef boost::shared_ptr<CQuoteRequest>            CQuoteRequestPtr;      
typedef boost::shared_ptr<CGroupRequest>            CGroupRequestPtr; 
typedef boost::shared_ptr<CQuoteInfoResponse>       CQuoteInfoResponsePtr;
typedef boost::shared_ptr<CFuturesOptionRequest>    CFuturesOptionRequestPtr;
//////////////////////////////////////////////////////////////////////////////

class CHyperFeedResponserBase
{
public:
	virtual void PublicResponse(CResponseBasePtr pResponse) = 0;
};



//////////////////////////////////////////////////////////////////////////////
template <class TBase>
class CHyperFeedBase : public EgStd::CEgTracingClass
{
private:
	CHandle                 m_hStop;
	CHandle                 m_hConnected;
	CHandle                 m_hCantConnect;
	CHandle                 m_hCancel;

	CStrategy<CRequestBasePtr,  CHyperFeedBase<TBase>, false>   m_RequestStrategy;
	CStrategy<CResponseBasePtr, CHyperFeedBase<TBase> >        m_ResponseStrategy;
	_com_error				m_errorConnect;				

public:
	CHyperFeedBase() 
	    :m_errorConnect(S_OK)
		,m_ResponseStrategy(this)
		,m_RequestStrategy(this)
		,m_lRespSize(0)
	{
		m_hStop       .Attach( ::CreateEvent(NULL, TRUE, FALSE, NULL) );
		m_hConnected  .Attach( ::CreateEvent(NULL, TRUE, FALSE, NULL) );
		m_hCantConnect.Attach( ::CreateEvent(NULL, TRUE, FALSE, NULL) );
		m_hCancel     .Attach( ::CreateEvent(NULL, FALSE,FALSE, NULL) );
	}

	virtual ~CHyperFeedBase()
	{
		DisconnectProvider();
	};

	void callOnRemoveAll()
	{
		Trace(enlogDebug, __FUNCTION__ , _T("Enter"));

		TBase* pThis = static_cast<TBase*>(this);
		if(pThis)
			pThis->OnRemoveAll();

		Trace(enlogDebug, __FUNCTION__ , _T("Exit"));

	}

	long GetResponseQueueLen(){return m_lRespSize;}

protected:
	void ConnectProvider()
	{
		Trace(enlogDebug, __FUNCTION__ , _T("Enter"));

		if (!IsConnected())
		{
            
			LPTSTR lpPath = (LPTSTR)malloc(1024);
			ZeroMemory(lpPath, sizeof(lpPath));
			//char chPath[] = "ProgramFiles";
			char chCommonPath[] = "CommonProgramFiles";

			if (GetEnvironmentVariable(chCommonPath, lpPath, 1024) > 0)
			 {
			   strncat(lpPath, "\\HyperFeed Shared", strlen("\\HyperFeed Shared"));
			   SetCurrentDirectory( lpPath );
			 }
			free(lpPath);
			
			HMODULE hModule = ::LoadLibrary(_T( "pcqw32.dll"));
			if ( hModule == NULL )
            {                

				EgLib::utils::ThrowError(HRESULT_FROM_WIN32(::GetLastError()), 
                     L"Unable to find PCQW32.DLL. Failed to start application.");
            }         
            
            ::FreeLibrary(hModule);

			ATLTRACE(_T("Connect object, thread=%d\n"), GetCurrentThreadId());

			ResetEvent(m_hStop);

			TBase* pThis = reinterpret_cast<TBase*>(this);
			pThis->InternalAddRef(); // protect destruction while threads starting

			m_ResponseStrategy.Start();
			m_RequestStrategy.Start();

/*
			m_hThreadResponse = (HANDLE)_beginthreadex(NULL,0,_ResponseThread,this,0,&m_nThreadResponseID);
			ATLTRACE(_T("Create event thread=%d\n"), m_nThreadResponseID);

			m_hThreadRequest  = (HANDLE)_beginthreadex(NULL,0,_RequestThread,this,0,&m_nThreadRequestID);
			ATLTRACE(_T("Create request thread=%d\n"), m_nThreadRequestID);
*/

			pThis->InternalRelease();

			HANDLE hEvents[] = {m_hConnected, m_hCantConnect};

			if( EgLib::WaitWithEvents(2, hEvents) == WAIT_OBJECT_0 + 1)
				throw m_errorConnect;
		}
		
		Trace(enlogDebug, __FUNCTION__ , _T("Exit"));


	};

	void DisconnectProvider()
	{
		Trace(enlogDebug, __FUNCTION__ , _T("Enter"));

		if (IsConnected())
		{
			SetEvent(m_hStop);
			RemoveAllRequest();
			ResetEvent(m_hCantConnect);
			ATLTRACE(_T("Disconnect object, thread=%d\n"), GetCurrentThreadId());
			m_ResponseStrategy.Stop();
			m_RequestStrategy.Stop();
		}

		Trace(enlogDebug, __FUNCTION__ , _T("Exit"));
	};

	bool IsStopped()
	{
		return WaitForSingleObject(m_hStop,0) != WAIT_TIMEOUT;
	}

	bool IsConnected()
	{
		return WaitForSingleObject(m_hConnected,0) != WAIT_TIMEOUT;		
	}

	bool IsCanceled()
	{
		return WaitForSingleObject(m_hCancel,0) != WAIT_TIMEOUT;
	}

	void Cancel()
	{
		Trace(enlogDebug, __FUNCTION__ , _T("Enter"));

		SetEvent(m_hCancel);

		Trace(enlogDebug, __FUNCTION__ , _T("Exit"));
	}

	void AddRequest(CRequestBasePtr pRequest)
	{
		Trace(enlogDebug, __FUNCTION__ , _T("Enter"));

		m_RequestStrategy.PushData(pRequest);

		Trace(enlogDebug, __FUNCTION__ , _T("Exit"));
	}

	void RemoveAllRequest()
	{
		Trace(enlogDebug, __FUNCTION__ , _T("Enter"));

		m_RequestStrategy.RemoveAll();

		Trace(enlogDebug, __FUNCTION__ , _T("Exit"));
	}

	void AddResponse(CResponseBasePtr pResponse)
	{
		Trace(enlogDebug, __FUNCTION__ , _T("Enter"));

		m_ResponseStrategy.PushData(pResponse);
		::InterlockedIncrement(&m_lRespSize);
		TBase* pThis = static_cast<TBase*>(this);
		pThis->OnAddResponse();
		
		Trace(enlogDebug, __FUNCTION__ , _T("Exit"));

	}

	//-----------------------------------------------------------------------------------------------//
	//void OnRemoveAll() {};
	//-----------------------------------------------------------------------------------------------//
	HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, RequestsTypeEnum enumRequest, const CComVariant &varRequest)
	{
		Trace(enlogDebug, __FUNCTION__ , _T("Enter"));
		HRESULT hr = S_OK;
		CComVariant varResult;
		TBase* pThis = static_cast<TBase*>(this);
		if(!pThis)
		{
			Trace(enlogFaults, __FUNCTION__ , _T("Exit with error hr = %d, file: %s line: %d"), E_FAIL, __FILE__, __LINE__);
			return E_FAIL;
		}
		try
		{

			int nConnectionIndex;
			CComVariant* pvars = new CComVariant[4];
			int nConnections = pThis->m_vec.GetSize();


			for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
			{
				pThis->Lock();
				CComPtr<IUnknown> sp = pThis->m_vec.GetAt(nConnectionIndex);
				pThis->Unlock();
				IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
				if (pDispatch != NULL)
				{
					VariantClear(&varResult);
					pvars[3] = enumError;
					pvars[2] = (BSTR)bstrDescription;
					pvars[1] = enumRequest;
					pvars[0] = varRequest;
					DISPPARAMS disp = { pvars, NULL, 4, 0 };
					hr = pDispatch->Invoke(0x3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
				}
			}
			delete[] pvars;

			if(FAILED(hr))
			{
				Trace(enlogFaults, __FUNCTION__ , _T("Exit with error hr = %d, file: %s line: %d"), E_FAIL, __FILE__, __LINE__);
				hr = E_FAIL;
			}

		}
		_CATCH_COM_EXCEPTION_RETURN_COM(L"On Error Failed")
		_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

		Trace(enlogDebug, __FUNCTION__ , _T("Exit"));
		if (hr == E_FAIL)
			return E_FAIL;
		else
			return varResult.scode;
	}

public:

	void OnAddResponse(){}
	virtual void OnData(CRequestBasePtr& Data)
	{
		Trace(enlogDebug, __FUNCTION__ , _T("Enter"));

		if(WAIT_TIMEOUT == ::WaitForSingleObject(m_hStop, 0))
		{
			TBase* pThis = static_cast<TBase*>(this);
			if(pThis && (bool)Data )
				pThis->OnRequest(Data);
		}
		
		Trace(enlogDebug, __FUNCTION__ , _T("Exit"));

	}

	virtual void OnData(CResponseBasePtr& Data)
	{
		Trace(enlogDebug, __FUNCTION__ , _T("Enter"));
		InterlockedDecrement(&m_lRespSize);
		try
		{

			if(WAIT_TIMEOUT == ::WaitForSingleObject(m_hStop, 0))
			{
				TBase* pThis = static_cast<TBase*>(this);
				if(pThis && (bool)Data )
				{
					if(_enResponseError == Data->m_enType)
					{
						CErrorResponsePtr pError = boost::shared_dynamic_cast<CErrorResponse>(Data);
						if(pError)
							pThis->OnError(pError->m_Error, pError->m_bstrDescription, pError->m_enRequestType, pError->m_vtRequest);
					}
					pThis->OnResponse(Data);
				}// end switch
			}
		}
		catch(...)
		 {
			//_HANDLE_EXCEPTION;
			Trace(enlogFaults, __FUNCTION__ , _T("On Data Failed with unhandled exception"));
		 }
		

		Trace(enlogDebug, __FUNCTION__ , _T("Exit"));

	}

	HRESULT OnStrategyInitialize()
	{
		Trace(enlogDebug, __FUNCTION__ , _T("Enter"));

		HRESULT dwResult = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);

		if(SUCCEEDED(dwResult))
		{
			TBase* pThis = static_cast<TBase*>(this);
			if(!pThis)
			{
				Trace(enlogFaults, __FUNCTION__ , _T("Exit with error hr = %d, file: %s line: %d"), E_FAIL, __FILE__, __LINE__);
				return E_FAIL;
			}
				

			EgLib::CAutoLock  lock(m_AttachLock);

			try
			{
				if(!IsConnected())
				{
					pThis->Attach();
					SetEvent(m_hConnected);
				}
			}
			catch (_com_error& err) 
			{
				Trace(enlogFaults, __FUNCTION__ , _T("Com exception handled desc: = %s, file: %s line: %d"), (const TCHAR*)err.Description(), __FILE__, __LINE__);

				m_errorConnect = err;
				SetEvent(m_hCantConnect);
				dwResult = err.Error();
			}
		}

		Trace(enlogDebug, __FUNCTION__ , _T("Exit"));

		return dwResult;
	}
	void  OnStrategyUninitialize()
	{ 
		Trace(enlogDebug, __FUNCTION__ , _T("Enter"));

		try
		{
			EgLib::CAutoLock  lock(m_AttachLock);
			TBase* pThis = static_cast<TBase*>(this);
			if(pThis && IsConnected())
			{
				pThis->Detach();
				::ResetEvent(m_hConnected);
			}
		}
		catch (_com_error&){}

		CoUninitialize();

		Trace(enlogDebug, __FUNCTION__ , _T("Exit"));
	}

	EgLib::CLock    m_AttachLock;
	volatile long   m_lRespSize;

};

#endif // !defined(AFX_HYPERFEEDBASE_H__17F312E7_8CEE_4B21_A5DD_B2822DE6E631__INCLUDED_)
