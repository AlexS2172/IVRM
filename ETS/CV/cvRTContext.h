// cvRTContext.h : Declaration of the CcvRTContext

#pragma once
#ifndef _CV_cvRTContext_H
#define _CV_cvRTContext_H

#include "resource.h"       // main symbols

#include "CV.h"
#include "cvRT.h"
#include "cvRTPool.h"
#include <map>
#include <vector>
#include <stack>

// CcvRTContext
using namespace std;
using namespace EgLib;
using namespace CV;
using namespace cvRuntime;


_COM_SMARTPTR_TYPEDEF( IcvRTContext, __uuidof(IcvRTContext) );

typedef	shared_ptr<CcvRTPool> CcvRTPoolPtr;

//typedef map<unsigned int, IcvRTPoolPtr>	cvPoolMap;
typedef map<unsigned int, CcvRTPoolPtr>	cvPoolMap;

class ATL_NO_VTABLE CcvRTContext : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CcvRTContext, &CLSID_cvRTContext>,
	public ISupportErrorInfo,
	public IDispatchImpl<IcvRTContext, &IID_IcvRTContext, &LIBID_CVLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public cvContext
{
	_bstr_t						m_strConnection;
	CDBConnection				m_Conn;
	cvMetaPoolMap				m_mapMetaPool;
	cvStaticStorage				m_StaticData;
	cvDynamicStorage			m_DynamicData;
	cvExecutionStack			m_ExecStack;
	cvPoolMap					m_mapPools;
	bool						m_bLess;
	bool						m_bGreater;
	long						m_RecalculateCvCount;

	bool							m_bError;
	bool LoadContext(int contextID);

	cvStaticStorage::size_type	m_idxStatics;
	void	RaiseError( HRESULT hr, LPCTSTR text){
		m_bError = true;
		EgLib::CComErrorWrapper::ThrowError(hr, text);
	}

public:
	CcvRTContext() : m_bLess( false ), m_bGreater( false ), m_bError(false),
							m_idxStatics(0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CVRTCONTEXT)

DECLARE_NOT_AGGREGATABLE(CcvRTContext)

BEGIN_COM_MAP(CcvRTContext)
	COM_INTERFACE_ENTRY(IcvRTContext)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_RecalculateCvCount = -1L;
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:
	CcvRTPool *GetPool( unsigned int uPoolID ){
		CcvRTPoolPtr ptr;
		cvPoolMap::iterator itr = m_mapPools.find( uPoolID );
		if ( itr != m_mapPools.end() ) {
			//IcvRTPoolPtr pI = itr->second;
			//ptr = dynamic_cast<CcvRTPool*>( (IcvRTPool*)pI);
			ptr = itr->second;
		}
		return ptr.get();
	}
	STDMETHOD(InitContext)(LONG contextID);
	STDMETHOD(get_ConnectionString)( BSTR* pVal);
	STDMETHOD(get_Halted)( VARIANT_BOOL* pVal);
	STDMETHOD(put_ConnectionString)( BSTR newVal);
	STDMETHOD(CreateCVPool)(IcvDataProvider *pIcvDataProvider, LONG lLevelID, LONG* lPoolID);
	STDMETHOD(GetRequiredDataProviderLevels)(LONG lLevelID, SAFEARRAY** LevelsArray);
	STDMETHOD(BindAggregations)(LONG lPoolID, LONG lAggregationPoolID);
	STDMETHOD(Recalculate)(LONG lPoolID);
	STDMETHOD(GetCVIDs)(LONG lLevelID, SAFEARRAY** LevelsArray);
	STDMETHOD(GetCVInfo)( LONG lLevelID, LONG lCVID, cvInfo* pInfo );
	STDMETHOD(GetCVValue)( LONG lPoolID, LONG lCVID, VARIANT *pRetVal);
	STDMETHOD(RegisterPoolDataProvider)( LONG lPoolID, LONG lDataProviderLevelID, IcvDataProvider* pIcvDataProvider );
	STDMETHOD(GetSystemEnvironment)( LONG lLevelID, SAFEARRAY **arrayVariables, SAFEARRAY **arrayFunctions );
	STDMETHOD(GetLevelIDByName)( BSTR strLevelName, LONG *pID );
	STDMETHOD(RemoveCVPool)( LONG lPoolID, VARIANT_BOOL bClearAggOnly);
	STDMETHOD(Halt)(void){ m_bError = true; return S_OK; }
	STDMETHOD(Clear)(void);
	STDMETHOD(SetRecalculateCvCount)(LONG Count) {m_RecalculateCvCount = Count; return S_OK;};
};

typedef shared_ptr<CcvRTContext>	cvRTContextPtr;

OBJECT_ENTRY_AUTO(__uuidof(cvRTContext), CcvRTContext)

#endif