#pragma once

#pragma warning(disable:4530)	//Template memory warning
#pragma warning(disable:4786)	//Template memory warning

namespace EgLib
{
	using namespace std;

	class CComDynamicUnkGITArray : public CComDynamicUnkArray
	{
	private:
		IGlobalInterfaceTable*  GIT;

	public:

		CComDynamicUnkGITArray() : CComDynamicUnkArray()
		{ 
			//DWORD threadID = GetCurrentThreadId();
			//ATLTRACE(/*_T(*/"CComDynamicUnkGITArray :CComDynamicUnkArray Enter. thread %x\n", threadID /*)*/);
			GIT = NULL;

			CoCreateInstance( CLSID_StdGlobalInterfaceTable, 
				NULL, 
				CLSCTX_INPROC_SERVER, 
				__uuidof(IGlobalInterfaceTable), 
				reinterpret_cast< void** >(&GIT) );
			//ATLTRACE(/*_T(*/"CComDynamicUnkGITArray :CComDynamicUnkArray exit. thread %x\n", threadID /*)*/);
		}

		~CComDynamicUnkGITArray()
		{
			//DWORD threadID = GetCurrentThreadId();
			//ATLTRACE(/*_T(*/"CComDynamicUnkGITArray :~CComDynamicUnkGITArray Enter. thread %x\n", threadID /*)*/);

			clear();
			if( GIT != NULL )
			{
				GIT->Release();
			}
			//ATLTRACE(/*_T(*/"CComDynamicUnkGITArray :~CComDynamicUnkGITArray Exit. thread %x\n", threadID /*)*/);
		}
		DWORD CComDynamicUnkGITArray::Add(IUnknown* pUnk)
		{
			//DWORD threadID = GetCurrentThreadId();
			//ATLTRACE(/*_T(*/"CComDynamicUnkGITArray :Add Enter. thread %x\n", threadID /*)*/);
			DWORD pdwCookie = 0;
			DWORD dwCookie = CComDynamicUnkArray::Add( pUnk );

			HRESULT hr = S_OK;
			if( GIT != NULL )
			{
				hr = GIT->RegisterInterfaceInGlobal(
					pUnk,                 //Pointer to interface of type riid of object 
					//containing global interface
					__uuidof(IUnknown),   //IID of the interface to be registered
					&pdwCookie            //Supplies a pointer to the cookie that provides 
					//a caller in another apartment access to the 
					//interface pointer
					);
				if( S_OK == hr )
				{
					CookieMap[dwCookie] = pdwCookie;
					ThreadIdMap[dwCookie] = GetCurrentThreadId();
				}
			}
			//ATLTRACE(/*_T(*/"CComDynamicUnkGITArray :Add Exit. thread %x\n", threadID /*)*/);
			return dwCookie;
		}

		BOOL CComDynamicUnkGITArray::Remove(DWORD dwCookie)
		{
			//DWORD threadID = GetCurrentThreadId();
			//ATLTRACE(/*_T(*/"CComDynamicUnkGITArray :Remove Enter. Cookie: %d, thread %x\n",dwCookie, threadID /*)*/);
			BOOL Result = CComDynamicUnkArray::Remove( dwCookie );

			if( Result && GIT != NULL )
			{
				CookieMapType::iterator itrGIT =  CookieMap.find( dwCookie );
				if(  CookieMap.end()!= itrGIT )
				{
					GIT->RevokeInterfaceFromGlobal(//Cookie that was returned from RegisterInterfaceInGlobal
						itrGIT->second);
					CookieMap.erase(itrGIT);
					ThreadIdMap.erase(dwCookie);
				}
			}
			//ATLTRACE(/*_T(*/"CComDynamicUnkGITArray :Remove Exit. Cookie: %d, thread %x\n",dwCookie, threadID /*)*/);
			return Result;
		}

		CComPtr<IUnknown> GetAt(int nIndex)
		{
			DWORD dwCookie = nIndex+1;
			CookieMapType::const_iterator   itrGIT      = CookieMap.find( dwCookie );
			ThreadIdMapType::const_iterator itrThreadID = ThreadIdMap.find(dwCookie);
			DWORD threadID = GetCurrentThreadId(); // vk
			if(CookieMap.end() == itrGIT || itrThreadID->second == threadID )
			{
				//ATLTRACE(_T("CComDynamicUnkGITArray :Call CComDynamicUnkArray::GetAt( nIndex )\n"));
				return CComDynamicUnkArray::GetAt( nIndex );
			}

			if( GIT != NULL )
			{
				CComPtr<IUnknown>   ppv;
				//ATLTRACE(/*_T(*/"CComDynamicUnkGITArray :Call GetInterfaceFromGlobal Enter. Thread %x\n",threadID /*)*/);
				HRESULT hr = GIT->GetInterfaceFromGlobal(
					itrGIT->second,					  //Cookie identifying the desired global 
													  //interface and its object
					__uuidof(IUnknown),               //IID of the registered global interface
					reinterpret_cast< void** >(&ppv)  //Indirect pointer to the desired interface
					);
				//ATLTRACE(_T("CComDynamicUnkGITArray :Call GetInterfaceFromGlobal Exit\n"));
				if( hr == S_OK )
					return ppv;
			}
			return NULL;
		}

		void clear()
		{
			//DWORD threadID = GetCurrentThreadId();
			//ATLTRACE(/*_T(*/"CComDynamicUnkGITArray :clear Enter. thread %x\n", threadID /*)*/);
			CComDynamicUnkArray::clear();

			if( GIT != NULL )
			{
				CookieMapType::iterator iter;
				for( iter = CookieMap.begin(); iter != CookieMap.end(); ++iter )
				{
					GIT->RevokeInterfaceFromGlobal(
						iter->second
						);
				}
			}
			CookieMap.clear();
			ThreadIdMap.clear();
			//ATLTRACE(/*_T(*/"CComDynamicUnkGITArray :clear Exit. thread %x\n", threadID /*)*/);
		}
	protected:
		typedef map<DWORD, DWORD > CookieMapType;
		typedef map<DWORD, DWORD > ThreadIdMapType;

		CookieMapType CookieMap;
		ThreadIdMapType ThreadIdMap;
	};
//-------------------------------------------------------------------------------------------------------------------------
class CComDynamicMarshalledUnkArray : public CComDynamicUnkArray
{

public:

	CComDynamicMarshalledUnkArray() : CComDynamicUnkArray(){ }

	~CComDynamicMarshalledUnkArray(){	clear(); }

	DWORD Add(IUnknown* pUnk)
	{
		DWORD pdwCookie = 0;
		DWORD dwCookie = CComDynamicUnkArray::Add( pUnk );

		HRESULT hr = S_OK;
		if( dwCookie != 0 )
		{
			LPSTREAM lpStream = NULL;
			if(S_OK == CoMarshalInterThreadInterfaceInStream(__uuidof(IUnknown), pUnk, &lpStream))
			{
				sConnection data;
				data.m_lpStream   = lpStream;
				data.m_uiThreadId = GetCurrentThreadId();

				CookieMap[dwCookie] = data;
			}
		}
		return dwCookie;
	}

	BOOL Remove(DWORD dwCookie)
	{
		BOOL Result = CComDynamicUnkArray::Remove( dwCookie );

		if( Result  )
		{
			CookieMapType::iterator itr =  CookieMap.find( dwCookie );
			if(  CookieMap.end()!= itr )
			{
				CComPtr<IUnknown> spI = NULL;
				if(itr->second.m_lpStream!=NULL)
					CoGetInterfaceAndReleaseStream(itr->second.m_lpStream, __uuidof(IUnknown),(LPVOID*) &spI);
				CookieMap.erase(itr);
			}
		}
		return Result;
	}

	CComPtr<IUnknown> GetAt(int nIndex)
	{
		DWORD dwCookie = nIndex+1;
		CookieMapType::iterator itr =  CookieMap.find( dwCookie );
		if(CookieMap.end() == itr || ::GetCurrentThreadId() == itr->second.m_uiThreadId)
			return CComDynamicUnkArray::GetAt( nIndex );

		CComPtr<IUnknown> ppv;
		if(itr->second.m_lpStream != NULL)
		{
			DWORD dwResult =  CoGetInterfaceAndReleaseStream(itr->second.m_lpStream, __uuidof(IUnknown), (LPVOID*)&ppv);
			itr->second.m_lpStream = NULL;
			if(S_OK == dwResult)
			{
				itr->second.m_spInterface = ppv;
				return ppv;
			}
		}
		if(itr->second.m_spInterface!=NULL)
			return itr->second.m_spInterface;

		return NULL;
	}

	void clear()
	{
		CComDynamicUnkArray::clear();

		CookieMapType::iterator iter;
		for( iter = CookieMap.begin(); iter != CookieMap.end(); ++iter )
		{
			CComPtr<IUnknown> spI = NULL;
			if(iter->second.m_lpStream!=NULL)
				CoGetInterfaceAndReleaseStream(iter->second.m_lpStream, __uuidof(IUnknown),(LPVOID*) &spI);
		}
		CookieMap.clear();
	}
protected:
	struct sConnection{
	public:
		UINT				m_uiThreadId; 
		LPSTREAM			m_lpStream;
		CComPtr<IUnknown>	m_spInterface;

		sConnection()
			:m_uiThreadId(0)
			,m_lpStream(NULL){};

		sConnection(const sConnection& sCn){ Copy(sCn); }
		sConnection& operator=(const sConnection& sCn){ Copy(sCn); return *this; }

	private:
		void Copy(const sConnection& sCn)
		{
			m_uiThreadId = sCn.m_uiThreadId;
			m_lpStream   = sCn.m_lpStream;
			m_spInterface= sCn.m_spInterface; 

		}

	} ;
	typedef map<DWORD, sConnection > CookieMapType;
	

	CookieMapType			CookieMap;
};

}
