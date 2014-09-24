#ifndef __EGATLCOM_H__
#define __EGATLCOM_H__

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLBASE_H__
	#error atlcom.h requires atlbase.h to be included first
#endif

#ifndef __ATLCOM_H__
	#error egatlcom.h requires atlcom.h to be included first
#endif

/*#ifndef _MAP_
	#error egatlcom.h requires <map> to be included first
#endif*/

namespace ATL
{

#define ID_FROM_CLASS_MEMBER offsetof

#define SINK_ENTRY_INFO2(m, iid, dispid, fn, info)\
    {ID_FROM_CLASS_MEMBER(_atl_event_classtype, m), &iid, 0, dispid,\
    (void (__stdcall _atl_event_classtype::*)())fn, info},

template <class T, const IID* pdiid>
class ATL_NO_VTABLE IDispEventSimpleImpl2 : public _IDispEvent
{
public:
    IDispEventSimpleImpl2(T* pInvoke) :
        nID((long)this - (long)pInvoke)
    {
    }

	STDMETHOD(_LocDEQueryInterface)(REFIID riid, void ** ppvObject)
	{
		if (InlineIsEqualGUID(riid, *pdiid) || 
			InlineIsEqualUnknown(riid) ||
			InlineIsEqualGUID(riid, IID_IDispatch) ||
			InlineIsEqualGUID(riid, m_iid))
		{
			if (ppvObject == NULL)
				return E_POINTER;
			*ppvObject = this;
			AddRef();
#ifdef _ATL_DEBUG_INTERFACES
			_Module.AddThunk((IUnknown**)ppvObject, _T("IDispEventSimpleImpl2"), riid);
#endif // _ATL_DEBUG_INTERFACES
			return S_OK;
		}
		else
			return E_NOINTERFACE;
	}

	// These are here only to support use in non-COM objects	
	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		return 1;
	}
	virtual ULONG STDMETHODCALLTYPE Release()
	{
		return 1;
	}

	STDMETHOD(GetTypeInfoCount)(UINT* pctinfo)
	{
        return E_NOTIMPL;
    }

	STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo** pptinfo)
	{
        return E_NOTIMPL;
    }

	STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames,
		LCID lcid, DISPID* rgdispid)
	{
        return E_NOTIMPL;
    }

	STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid,
		LCID lcid, WORD /*wFlags*/, DISPPARAMS* pdispparams, VARIANT* pvarResult,
		EXCEPINFO* /*pexcepinfo*/, UINT* /*puArgErr*/)
	{
		T* pT = reinterpret_cast<T*>((long)this - nID);
		const _ATL_EVENT_ENTRY<T>* pMap = T::_GetSinkMap();
		const _ATL_EVENT_ENTRY<T>* pFound = NULL;
		void (__stdcall T::*pEvent)() = NULL;
		while (pMap->piid != NULL)
		{
			if ((pMap->nControlID == nID) && (pMap->dispid == dispidMember) && 
				(pMap->piid == pdiid)) //comparing pointers here should be adequate
			{
				pFound = pMap;
				break;
			}
			pMap++;
		}
		if (pFound == NULL)
			return S_OK;
		

		_ATL_FUNC_INFO info;
		_ATL_FUNC_INFO* pInfo;
		if (pFound->pInfo != NULL)
			pInfo = pFound->pInfo;
		else
		{
			pInfo = &info;
			HRESULT hr = GetFuncInfoFromId(*pdiid, dispidMember, lcid, info);
			if (FAILED(hr))
				return S_OK;
		}
		InvokeFromFuncInfo(pFound->pfn, *pInfo, pdispparams, pvarResult);
		return S_OK;
	}

	//Helper for invoking the event
	HRESULT InvokeFromFuncInfo(void (__stdcall T::*pEvent)(), _ATL_FUNC_INFO& info, DISPPARAMS* pdispparams, VARIANT* pvarResult)
	{
		T* pT = reinterpret_cast<T*>((long)this - nID);
		VARIANTARG** pVarArgs = info.nParams ? (VARIANTARG**)alloca(sizeof(VARIANTARG*)*info.nParams) : 0;
		for (int i=0; i<info.nParams; i++)
			pVarArgs[i] = &pdispparams->rgvarg[info.nParams - i - 1];

		CComStdCallThunk<T> thunk;
		thunk.Init(pEvent, pT);
		CComVariant tmpResult;
		if (pvarResult == NULL)
			pvarResult = &tmpResult;

		HRESULT hr = DispCallFunc(
			&thunk,
			0,
			info.cc,
			info.vtReturn,
			info.nParams,
			info.pVarTypes,
			pVarArgs,
			pvarResult);
		ATLASSERT(SUCCEEDED(hr));
		return hr;
	}

	//Helper for finding the function index for a DISPID
	virtual HRESULT GetFuncInfoFromId(const IID& iid, DISPID dispidMember, LCID lcid, _ATL_FUNC_INFO& info)
	{
		return E_NOTIMPL;
	}
	//Helpers for sinking events on random IUnknown*
	HRESULT DispEventAdvise(IUnknown* pUnk, const IID* piid)
	{
		ATLASSERT(m_dwEventCookie == 0xFEFEFEFE);
		return AtlAdvise(pUnk, (IUnknown*)this, *piid, &m_dwEventCookie);
	}
	HRESULT DispEventUnadvise(IUnknown* pUnk, const IID* piid)
	{
		HRESULT hr = AtlUnadvise(pUnk, *piid, m_dwEventCookie);
		m_dwEventCookie = 0xFEFEFEFE;
		return hr;
	}
	HRESULT DispEventAdvise(IUnknown* pUnk)
	{
		return _IDispEvent::DispEventAdvise(pUnk, pdiid);
	}
	HRESULT DispEventUnadvise(IUnknown* pUnk)
	{
		return _IDispEvent::DispEventUnadvise(pUnk, pdiid);
	}

protected:
    UINT    nID;
};

#define BEGIN_PROPNOTIFYSINK_MAP(_class)\
	static const _ATL_EVENT_ENTRY<_class>* _GetPropNotifySinkMap()\
	{\
		typedef _class _atl_event_classtype;\
		static const _ATL_EVENT_ENTRY<_class> map[] = {

template <class T>
class ATL_NO_VTABLE IPropertyNotifySinkImpl2
{
public:
    IPropertyNotifySinkImpl2(T* pInvoke) :
        nID((long)this - (long)pInvoke),
		m_dwEventCookie(0xFEFEFEFE)
    {
    }

	// These are here only to support use in non-COM objects	
	STDMETHOD(QueryInterface)(REFIID riid, void ** ppvObject)
	{
		if (InlineIsEqualUnknown(riid) ||
			InlineIsEqualGUID(riid, IID_IPropertyNotifySink))
		{
			if (ppvObject == NULL)
				return E_POINTER;
			*ppvObject = this;
			AddRef();
#ifdef _ATL_DEBUG_INTERFACES
			_Module.AddThunk((IUnknown**)ppvObject, _T("IPropertyNotifySinkImpl2"), riid);
#endif // _ATL_DEBUG_INTERFACES
			return S_OK;
		}
		else
			return E_NOINTERFACE;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		return 1;
	}
	virtual ULONG STDMETHODCALLTYPE Release()
	{
		return 1;
	}

    STDMETHOD(OnChanged)(/* [in] */ DISPID dispID)
    {
		T* pT = reinterpret_cast<T*>((long)this - nID);
		const _ATL_EVENT_ENTRY<T>* pMap = T::_GetSinkMap();
		const _ATL_EVENT_ENTRY<T>* pFound = NULL;
		void (__stdcall T::*pEvent)() = NULL;
		while (pMap->piid != NULL)
		{
			if ((pMap->nControlID == nID) && (pMap->dispid == dispID) && 
				(*pMap->piid == IID_IPropertyNotifySink))
			{
				pFound = pMap;
				break;
			}
			pMap++;
		}
		if (pFound == NULL)
			return S_OK;
		
		CComStdCallThunk<T> thunk;
		thunk.Init(pFound->pfn, pT);

		CComVariant tmpResult;
		HRESULT hr = DispCallFunc(
			&thunk,
			0,
			CC_STDCALL,
			VT_EMPTY,
			0,
			0,
			0,
			&tmpResult);
		ATLASSERT(SUCCEEDED(hr));
		return hr;
    }
        
    STDMETHOD(OnRequestEdit)(/* [in] */ DISPID dispID)
    {
        return S_OK;
    }

	HRESULT Advise(IUnknown* pUnk)
	{
		ATLASSERT(m_dwEventCookie == 0xFEFEFEFE);
		return AtlAdvise(pUnk, (IUnknown*)this, IID_IPropertyNotifySink, &m_dwEventCookie);
	}
	HRESULT Unadvise(IUnknown* pUnk)
	{
		HRESULT hr = AtlUnadvise(pUnk, IID_IPropertyNotifySink, m_dwEventCookie);
		m_dwEventCookie = 0xFEFEFEFE;
		return hr;
	}

protected:
    UINT    nID;
    DWORD   m_dwEventCookie;
};

/////////////////////////////////////////////////////////////////////////////
class CComGITDynamicUnkArray : public CComDynamicUnkArray
{
private:
	IGlobalInterfaceTable*  GIT;

public:

	CComGITDynamicUnkArray() : CComDynamicUnkArray()
	{ 
		GIT = NULL;

		CoCreateInstance( CLSID_StdGlobalInterfaceTable, 
			NULL, 
			CLSCTX_INPROC_SERVER, 
			__uuidof(IGlobalInterfaceTable), 
			reinterpret_cast< void** >(&GIT) );
	}

	~CComGITDynamicUnkArray()
	{
		clear();
		if( GIT != NULL )
		{
			GIT->Release();
		}
	}
	DWORD Add(IUnknown* pUnk);
	BOOL Remove(DWORD dwCookie);

	CComPtr<IUnknown> GetAt(int nIndex)
	{
		DWORD dwCookie = (DWORD)CComDynamicUnkArray::GetAt( nIndex );

		if( dwCookie == 0 )
			return NULL;

		if( CookieMap.find( dwCookie ) == CookieMap.end() )
		{
			return (IUnknown*)dwCookie;
		}
		if( GIT != NULL )
		{
			CComPtr<IUnknown>   ppv;

			HRESULT hr = GIT->GetInterfaceFromGlobal(
				CookieMap[dwCookie],              //Cookie identifying the desired global 
				//interface and its object
				__uuidof(IUnknown),               //IID of the registered global interface
				reinterpret_cast< void** >(&ppv)  //Indirect pointer to the desired interface
				);
			if( hr == S_OK )
			{
				return ppv;
			}
		}
		return (IUnknown*)dwCookie;
	}

	void clear()
	{
		CComDynamicUnkArray::clear();

		if( GIT != NULL )
		{
			std::map< DWORD, DWORD >::iterator iter;
			for( iter = CookieMap.begin(); iter != CookieMap.end(); ++iter )
			{
				GIT->RevokeInterfaceFromGlobal(
					iter->second
					);
			}
		}
		CookieMap.clear();
	}

protected:
	std::map< DWORD, DWORD > CookieMap;
};

inline DWORD CComGITDynamicUnkArray::Add(IUnknown* pUnk)
{
	DWORD Result = CComDynamicUnkArray::Add( pUnk );

	HRESULT hr;
	DWORD   pdwCookie = 0;
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
	}
	if( hr == S_OK )
	{
		CookieMap[Result] = pdwCookie;
	}

	return Result;
}

inline BOOL CComGITDynamicUnkArray::Remove(DWORD dwCookie)
{
	BOOL Result = CComDynamicUnkArray::Remove( dwCookie );

	if( GIT != NULL )
	{
		if( CookieMap.find( dwCookie ) != CookieMap.end() )
		{
			GIT->RevokeInterfaceFromGlobal(
				CookieMap[dwCookie]   //Cookie that was returned from 
				//RegisterInterfaceInGlobal
				);
				CookieMap.erase(dwCookie);
		}
	}
	return Result;
}

/////////////////////////////////////////////////////////////////////////////
template <class T>
class CComGITPtr2
{
private:
	CComPtr<IGlobalInterfaceTable>  m_spGIT;
	DWORD m_dwCookie;
	DWORD m_dwThread;

public:
	static CComPtr<IGlobalInterfaceTable> GetGlobalInterfaceTable()
	{
		CComPtr<IGlobalInterfaceTable>  spGIT;
		HRESULT hr = CoCreateInstance( CLSID_StdGlobalInterfaceTable, 
			NULL, 
			CLSCTX_INPROC_SERVER, 
			__uuidof(IGlobalInterfaceTable), 
			reinterpret_cast< void** >(&spGIT) );

		return spGIT;
	}

	typedef T _PtrClass;
	CComGITPtr2() : m_dwCookie(0), m_dwThread(0)
	{
		m_spGIT = GetGlobalInterfaceTable();
		m_dwThread = GetCurrentThreadId();
	}

	CComGITPtr2(T* lp) : m_dwCookie(0), m_dwThread(0)
	{
		m_spGIT = GetGlobalInterfaceTable();
		m_dwThread = GetCurrentThreadId();

		CComPtr<IUnknown> pUnk = lp;
		if( SUCCEEDED(hr) )
			hr = Assign(pUnk);
	}

	CComGITPtr2(const CComPtr<T>& lp) : m_dwCookie(0), m_dwThread(0)
	{
		m_spGIT = GetGlobalInterfaceTable();
		m_dwThread = GetCurrentThreadId();

		CComPtr<IUnknown> spUnk = lp;
		if( SUCCEEDED(hr) )
			hr = Assign(spUnk);
	}

	~CComGITPtr2()
	{
		Release();
	}

	HRESULT Assign(IUnknown* pUnk)
	{
		HRESULT hr = S_OK;
		Release();
		if( m_spGIT != NULL && pUnk != NULL )
		{
			hr = m_spGIT->RegisterInterfaceInGlobal(
				pUnk,                 //Pointer to interface of type riid of object 
				//containing global interface
				__uuidof(IUnknown),   //IID of the interface to be registered
				&m_dwCookie            //Supplies a pointer to the cookie that provides 
				//a caller in another apartment access to the 
				//interface pointer
				);
		}

		return hr;
	}

	void Release()
	{
		if( m_spGIT != NULL && m_dwCookie != 0)
		{
			HRESULT hr = m_spGIT->RevokeInterfaceFromGlobal(
					m_dwCookie   //Cookie that was returned from RegisterInterfaceInGlobal
					);
			m_dwCookie = 0;
		}
	}

	operator T*() const
	{
		CComQIPtr<T> spRet;
//		CComPtr<IGlobalInterfaceTable> spGIT = 
//			(m_dwThread == GetCurrentThreadId()) ? m_spGIT : GetGlobalInterfaceTable();

		if( m_spGIT != NULL && m_dwCookie != 0)
		{
			CComPtr<IUnknown>   spUnk;
			HRESULT hr = m_spGIT->GetInterfaceFromGlobal(
				m_dwCookie,              //Cookie identifying the desired global 
				//interface and its object
				__uuidof(IUnknown),               //IID of the registered global interface
				reinterpret_cast< void** >(&spUnk)  //Indirect pointer to the desired interface
				);

			if ( SUCCEEDED(hr) )
				spRet = spUnk;
		}

		return spRet;
	}

/*	T& operator*() const
	{
		ATLASSERT(p!=NULL);
		return *p;
	}
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&()
	{
		ATLASSERT(p==NULL);
		return &p;
	}
	_NoAddRefReleaseOnCComPtr<T>* operator->() const
	{
		ATLASSERT(p!=NULL);
		return (_NoAddRefReleaseOnCComPtr<T>*)p;
	}*/

	T* operator=(T* lp)
	{
		CComPtr<IUnknown> spUnk = lp;
		Assign(spUnk);
		return (T*)*this;
	}

	T* operator=(const CComPtr<T>& lp)
	{
		CComPtr<IUnknown> spUnk = lp;
		Assign(spUnk);
		return (T*)*this;
	}

/*	bool operator!() const
	{
		return (p == NULL);
	}
	bool operator<(T* pT) const
	{
		return p < pT;
	}
	bool operator==(T* pT) const
	{
		return p == pT;
	}
	// Compare two objects for equivalence
	bool IsEqualObject(IUnknown* pOther)
	{
		if (p == NULL && pOther == NULL)
			return true; // They are both NULL objects

		if (p == NULL || pOther == NULL)
			return false; // One is NULL the other is not

		CComPtr<IUnknown> punk1;
		CComPtr<IUnknown> punk2;
		p->QueryInterface(IID_IUnknown, (void**)&punk1);
		pOther->QueryInterface(IID_IUnknown, (void**)&punk2);
		return punk1 == punk2;
	}
	void Attach(T* p2)
	{
		if (p)
			p->Release();
		p = p2;
	}
	T* Detach()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}
	HRESULT CopyTo(T** ppT)
	{
		ATLASSERT(ppT != NULL);
		if (ppT == NULL)
			return E_POINTER;
		*ppT = p;
		if (p)
			p->AddRef();
		return S_OK;
	}
	HRESULT SetSite(IUnknown* punkParent)
	{
		return AtlSetChildSite(p, punkParent);
	}
	HRESULT Advise(IUnknown* pUnk, const IID& iid, LPDWORD pdw)
	{
		return AtlAdvise(p, pUnk, iid, pdw);
	}
	HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		ATLASSERT(p == NULL);
		return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
	}
	HRESULT CoCreateInstance(LPCOLESTR szProgID, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		CLSID clsid;
		HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
		ATLASSERT(p == NULL);
		if (SUCCEEDED(hr))
			hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
		return hr;
	}
	template <class Q>
		HRESULT QueryInterface(Q** pp) const
	{
		ATLASSERT(pp != NULL && *pp == NULL);
		return p->QueryInterface(__uuidof(Q), (void**)pp);
	}*/
};

} // namespace ATL

#endif
