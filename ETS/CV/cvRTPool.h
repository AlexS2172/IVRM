// cvRTPool.h : Declaration of the CcvRTPool

#pragma once
#ifndef _CV_cvRTPool_H
#define _CV_cvRTPool_H

#include "resource.h"       // main symbols
#include "CV.h"
#include "cvCV.h"
#include "cvRT.h"
#include <map>
#include <vector>

// CcvRTPool
using namespace std;
using namespace boost;
using namespace CV;
using namespace cvRuntime;

class CcvRTPool;
class CcvRTContext;

typedef vector< unsigned int> cvAggregationsVector;

class CcvRTCV{
	long						m_DBId;
	unsigned int				m_DataIndex;
	cvAggregationsVector		m_vecAggregations;	
	cvMetaCVPtr					m_spMetaCV;
	friend class CcvRTPool;
	friend class CcvRTContext;
};


typedef shared_ptr<CcvRTCV>	cvRTCVPtr;
typedef map<cvID, cvRTCVPtr>	cvRTCVMap;

_COM_SMARTPTR_TYPEDEF( IcvDataProvider, __uuidof(IcvDataProvider) );
//_COM_SMARTPTR_TYPEDEF( IcvRTPool, __uuidof( IcvRTPool) );

typedef map<cvID, IcvDataProviderPtr>	cvDataProvidersMap;



class /*ATL_NO_VTABLE*/ CcvRTPool
	//public CComObjectRootEx<CComSingleThreadModel>,
	//public CComCoClass<CcvRTPool, &CLSID_cvRTPool>,
	//public ISupportErrorInfoImpl<&IID_IcvRTPool>,
	//public IDispatchImpl<IcvRTPool, &IID_IcvRTPool, &LIBID_CVLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
	cvID									m_levelID;
	cvDataProvidersMap				m_mapDataProviders;
	//cvRTCVMap							m_mapCV;					// to access variables by ID
	vector<cvRTCVPtr>					m_vecCV;					// 
	vector<cvVMCommand>				m_Program;
	cvMetaPoolPtr						m_spMetaPool;
	CcvRTContext						*m_pContext;
	cvRTCVPtr GetCV( cvID VariableID ){
		//vector<cvRTCVPtr>::iterator itr = find(m_vecCV.begin(), m_vecCV.end(), VariableID);
		//cvRTCVMap::iterator itr = m_mapCV.find( VariableID );
		//return itr != m_mapCV.end() ? itr->second : cvRTCVPtr();
		return find(VariableID);
	}
	
	cvRTCVPtr find( cvID VariableID ){
		for (vector<cvRTCVPtr>::iterator itr = m_vecCV.begin(); itr != m_vecCV.end(); ++itr)
		{
			if ((*itr)->m_DBId == VariableID)
				return *itr;
		}

		return cvRTCVPtr();
	}

public:
	CcvRTPool() :m_levelID(0), m_pContext( NULL )
	{
		
	}

/*DECLARE_REGISTRY_RESOURCEID(IDR_CVRTPOOL)

DECLARE_NOT_AGGREGATABLE(CcvRTPool)

BEGIN_COM_MAP(CcvRTPool)
	COM_INTERFACE_ENTRY(IcvRTPool)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}*/

public:
	void	AttachDataProvider( IcvDataProviderPtr pIcvDataProviderPtr ){ 
		m_mapDataProviders[ m_levelID ] = pIcvDataProviderPtr;
	}

	void	SetContext( CcvRTContext *pContext) {
		m_pContext = pContext;
	}
	void	SetPoolsLevel( cvID levelID){
		m_levelID = levelID;
	}

	void BindAggregations(LONG lAggregationPoolID);
	void InitFromMetaPool( cvMetaPoolPtr, cvStaticStorage& StaticStorage );
	bool RegisterDataProvider(LONG lLevelID, IcvDataProvider* pIcvDataProvider);
	friend class CcvRTContext;
};


//OBJECT_ENTRY_AUTO(__uuidof(cvRTPool), CcvRTPool)
#endif