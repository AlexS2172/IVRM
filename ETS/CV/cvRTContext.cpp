// cvRTContext.cpp : Implementation of CcvRTContext

#include "stdafx.h"
#include "cvCV.h"
#include "cvRTContext.h"
#include ".\cvrtcontext.h"
#include <algorithm>

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// CcvRTContext
using namespace cvRuntime;
using namespace std;
STDMETHODIMP CcvRTContext::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IcvRTContext
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CcvRTContext::InitContext(LONG contextID)
{
	HRESULT hr = S_OK;
	
	try{

		// Loading context info from database
		LoadContext( contextID );

		// creating meta pool for every level in context
		cvLevelsByNameMap::iterator b = m_mapLevels.begin();
		cvLevelsByNameMap::iterator e = m_mapLevels.end();

		// its important here to create meta pools in order of they weight, lighter first then heavier
		vector<cvLevelPtr>	vecLevels;
		while ( b != e ) {
			vecLevels.push_back( b->second );
			b++;
		}
		sort( vecLevels.begin(), vecLevels.end(), cvLevel::less );

		for ( size_t s = 0; s < vecLevels.size(); s++) {
			cvMetaPoolPtr ptr = cvMetaPoolPtr( new cvMetaPool( vecLevels[s]->m_ID, vecLevels[s]->m_mapSysVars, vecLevels[s]->m_mapSysFunc, vecLevels[s]->m_mapCV, m_mapMetaPool, m_StaticData));
			m_mapMetaPool[ vecLevels[s]->m_ID ] = ptr;
		}

		// setup index for static data in static storages 
		m_idxStatics = m_StaticData.size();
		
	}
	catch( _com_error& e ){
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IcvRTContext, e.Error());
	}
	catch( exception& ){
		hr = E_FAIL;
	}
	catch (...) {
		hr = E_FAIL;
	}
	return hr;
}
bool CcvRTContext::LoadContext( int contextID){
	if ( !m_Conn.IsOpened() ) {
		// open connection 
		m_Conn.Open( m_strConnection, 10, 10, 10, 10 );
	}	

	CStoredProc<CClientRecordset> sp( m_Conn, L"usp_cvContext_Get" );
	sp << contextID;
	sp << 6;	// requested info level

	sp.Open();
	if( sp.GetRecordCount() ) {

		// first recordset describes context itself
		for(sp.MoveFirst(); !sp.IsEOF(); ++sp) {
			m_ID = static_cast<int>( sp[L"ID"]);
			m_strName = (LPCSTR)( (_bstr_t ) sp[L"Name"] );
			break;
		}

		// second recordset for levels inside context 
		sp.NextRecordset();		
		if( sp.GetRecordCount() ){
			for(sp.MoveFirst(); !sp.IsEOF(); ++sp) {
				cvLevelPtr p = cvLevelPtr( new cvLevel );
				p->m_ID = static_cast<int>( sp[L"ID"] );
				p->m_context = static_cast<int>( sp[L"contextID"] );
				p->m_strName = (LPCSTR)( (_bstr_t ) sp[L"Name"] );
				p->m_Weight = static_cast<int>( sp[L"Weight"] );
				m_mapLevels[ p->m_strName ] = p;
			}
		}
		// third recordset for system variables within levels
		sp.NextRecordset();		
		if( sp.GetRecordCount() ){
			for(sp.MoveFirst(); !sp.IsEOF(); ++sp) {
				cvSysTypedObjPtr p = cvSysTypedObjPtr( new cvSysTypedObj );
				p->m_ID = static_cast<int>( sp[L"ID"] );
				p->m_context = static_cast<int>( sp[L"contextID"] );
				p->m_level = static_cast<int>( sp[L"levelID"] );
				p->m_strName = (LPCSTR)( (_bstr_t ) sp[L"Name"] );
				p->m_type = static_cast<cvtType>( static_cast<int>(sp[L"Type"]) );
				p->m_global = sp["Global"];
				cvLevelsByNameMap::const_iterator i = m_mapLevels.findByID( p->m_level );
				if( i != m_mapLevels.end() )
					(*(i->second)).m_mapSysVars[p->m_strName] = p;
			}
		}
		// fourth recordset for system functions within levels
		sp.NextRecordset();		
		if( sp.GetRecordCount() ){
			for(sp.MoveFirst(); !sp.IsEOF(); ++sp) {
				cvSysFuncPtr p = cvSysFuncPtr( new cvSysFunc );
				p->m_ID = static_cast<int>( sp[L"ID"] );
				p->m_context = static_cast<int>( sp[L"contextID"] );
				p->m_level = static_cast<int>( sp[L"levelID"] );
				p->m_strName = (LPCSTR)( (_bstr_t ) sp[L"Name"] );
				p->m_type = static_cast<cvtType>( static_cast<int>(sp[L"Type"]) );
				p->m_global = sp["Global"];
				p->InitParameters( (LPCSTR)((_bstr_t) sp[L"Params"]) );
				cvLevelsByNameMap::const_iterator i = m_mapLevels.findByID( p->m_level );
				if( i != m_mapLevels.end() )
					(*(i->second)).m_mapSysFunc[p->m_strName] = p;
			}
		}
		// fifth one for predefined custom variables
		sp.NextRecordset();		
		if( sp.GetRecordCount() ){
			for(sp.MoveFirst(); !sp.IsEOF(); ++sp) {
				cvMetaCVPtr spCV = cvMetaCVPtr(new cvMetaCV);
				//cvSysTypedObjPtr p = boost::shared_static_cast<cvSysTypedObj>( spCV );
				spCV->m_ID = static_cast<int>( sp[L"ID"] );
				spCV->m_context = static_cast<int>( sp[L"contextID"] );
				spCV->m_level = static_cast<int>( sp[L"levelID"] );
				spCV->m_strName = (LPCSTR)( (_bstr_t ) sp[L"Name"] );
				spCV->m_type = static_cast<cvtType>( static_cast<int>(sp[L"Type"]) );
				spCV->InitAggregations( (LPCSTR)( (_bstr_t) sp[L"aggLevels"]));
				spCV->m_caption = (LPCSTR)( (_bstr_t ) sp[L"Caption"] );
				spCV->m_format = (LPCSTR)( (_bstr_t ) sp[L"Format"] );
				spCV->m_source = (LPCSTR)( (_bstr_t ) sp[L"Source"] );
				spCV->m_weight = static_cast<int>(sp[L"Weight"] );

				cvLevelsByNameMap::const_iterator i = m_mapLevels.findByID( spCV->m_level );
				if( i != m_mapLevels.end() ) {
					_variant_t vtVal = sp[L"Code"].GetValue();
					size_t v = static_cast<size_t>(vtVal.parray->rgsabound[0].cElements);
					void *ptr = malloc(v);
					if (ptr)
					{
						sp[L"Code"].GetBinaryValue( ptr, v );

						spCV->InitFromBinary( ptr, *this, (*(i->second)) /*reference to level*/  );
						(*(i->second)).m_mapCV[spCV->m_strName] = shared_static_cast<cvSysTypedObj>( spCV );

						free(ptr);
					}
				}


				// now, add 'stub' meta variable to levels, where this variable have aggregations
				if ( !spCV->m_vecAggregations.empty() ) {
					for ( size_t s = 0; s < spCV->m_vecAggregations.size(); s++ ) {
						cvLevelsByNameMap::const_iterator iA = m_mapLevels.findByID( spCV->m_vecAggregations[s] );
						if( iA != m_mapLevels.end() ) {
							cvMetaCVPtr spCVA = cvMetaCVPtr( new cvMetaCV );
							spCVA->m_ID = static_cast<int>( sp[L"ID"] );
							spCVA->m_context = static_cast<int>( sp[L"contextID"] );
							spCVA->m_level = static_cast<int>( spCV->m_vecAggregations[s] );	//( sp[L"levelID"] );
							spCVA->m_strName = (LPCSTR)( (_bstr_t ) sp[L"Name"] );
							spCVA->m_type = static_cast<cvtType>( static_cast<int>(sp[L"Type"]) );
							spCVA->m_caption = (LPCSTR)( (_bstr_t ) sp[L"Caption"] );
							spCVA->m_format = (LPCSTR)( (_bstr_t ) sp[L"Format"] );
							spCVA->m_source = (LPCSTR)( (_bstr_t ) sp[L"Source"] );
							spCVA->m_weight = 0/*static_cast<int>( sp[L"Weight"] )*/;
							(*(iA->second)).m_mapCV[spCVA->m_strName] = shared_static_cast<cvSysTypedObj>(spCVA);
						}
					}
				}
			}

		}
	}
	return false;
}
STDMETHODIMP CcvRTContext::get_ConnectionString( BSTR* pVal)
{
	if( pVal ){	
		*pVal = m_strConnection;
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP CcvRTContext::put_ConnectionString( BSTR newVal)
{
	if( wcslen( newVal ) ) {
		m_strConnection = newVal;
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP CcvRTContext::get_Halted( VARIANT_BOOL* pVal)
{
	if( pVal ){	
		*pVal = m_bError ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP CcvRTContext::GetSystemEnvironment(LONG lLevelID, SAFEARRAY **arrayVariables, SAFEARRAY **arrayFunctions ){
	cvMetaPoolMap::iterator itr = m_mapMetaPool.find( static_cast<cvID>(lLevelID) );
	if ( itr == m_mapMetaPool.end() ) 
		return E_INVALIDARG;
	cvMetaPoolPtr spMetaPool = itr->second;


	/*if ( !*arrayVariables ) 
		return E_POINTER;*/

	if ( *arrayVariables ) 
		::SafeArrayDestroy( *arrayVariables );

	/*if ( !*arrayFunctions) 
		return E_POINTER;*/

	if ( *arrayFunctions ) 
		::SafeArrayDestroy( *arrayFunctions );

	// get system variables required by level
	EgLib::CComRecPtr<cvInfo> Info;
	Info.Init();
	cvInfo *pInfo = NULL;
	SAFEARRAYBOUND sab = { static_cast<ULONG>(spMetaPool->m_mapSysVars.size()), 0 };
	LPSAFEARRAY psa = ::SafeArrayCreateEx( VT_RECORD, 1, &sab, (PVOID) Info.GetRecordInfo() );
	::SafeArrayAccessData( psa, (void**)&pInfo);
	cvSysTypedObjByNameMap::iterator b = spMetaPool->m_mapSysVars.begin();
	cvSysTypedObjByNameMap::iterator e = spMetaPool->m_mapSysVars.end();
	for (LONG i = 0; b != e; b++, i++) {
		cvSysTypedObjPtr pSysVar = shared_dynamic_cast<cvSysTypedObj>( b->second );
		Info->Caption = _bstr_t(L"").copy();
		Info->Format = _bstr_t(L"").copy();
		Info->ID = pSysVar->m_ID;
		Info->Type = static_cast<cvType>(pSysVar->m_type);
		Info->Name = _bstr_t( pSysVar->m_strName.c_str() ).copy();
		Info.CopyTo( &pInfo[i]  );
	}
	::SafeArrayUnaccessData(psa);
	*arrayVariables = psa;

	// get system functions required by level
	SAFEARRAYBOUND sab1 = { static_cast<ULONG>(spMetaPool->m_mapSysFuncs.size()), 0 };
	LPSAFEARRAY psa1 = ::SafeArrayCreateEx( VT_RECORD, 1, &sab1, (PVOID) Info.GetRecordInfo() );
	cvInfo *pInfo1 = NULL;
	::SafeArrayAccessData( psa1, (void**)&pInfo1);
	b = spMetaPool->m_mapSysFuncs.begin();
	e = spMetaPool->m_mapSysFuncs.end();
	for (LONG i = 0; b != e; b++, i++) {
		cvSysFuncPtr pSysFunc = shared_dynamic_cast<cvSysFunc>( b->second );
		Info->Caption = _bstr_t("").copy();
		Info->Format = _bstr_t("").copy();
		Info->ID = pSysFunc->m_ID;
		Info->Type = static_cast<cvType>(pSysFunc->m_type);
		Info->Name = _bstr_t( pSysFunc->m_strName.c_str() ).copy();
		Info.CopyTo( &pInfo1[i]  );
	}
	::SafeArrayUnaccessData(psa1);
	*arrayFunctions= psa1;

	return S_OK;
}

STDMETHODIMP CcvRTContext::CreateCVPool(IcvDataProvider *pIcvDataProvider, LONG lLevelID, LONG* lPoolID)
{
	cvMetaPoolMap::iterator itr = m_mapMetaPool.find( static_cast<cvID>(lLevelID) );
	if ( itr == m_mapMetaPool.end() || !pIcvDataProvider ) 
		return E_INVALIDARG;
	cvMetaPoolPtr spMetaPool = itr->second;

	//IcvRTPoolPtr	spPool;
	CcvRTPoolPtr	spPool(new CcvRTPool);
	IcvDataProviderPtr spcvDataProvider(pIcvDataProvider);
	//CComObject<CcvRTPool> *pNewPool = NULL;
	

	/*if ( SUCCEEDED( CComObject<CcvRTPool>::CreateInstance(&pNewPool)))
		spPool.Attach( pNewPool, TRUE);
	else
		return E_FAIL;	*/
	spPool->SetContext( this );
	spPool->SetPoolsLevel(lLevelID);
	spPool->AttachDataProvider( spcvDataProvider );
	m_mapPools[ static_cast<unsigned int>( m_mapPools.size() ) ] = spPool;
	spPool->InitFromMetaPool( spMetaPool, m_StaticData );
	*lPoolID =	static_cast<unsigned int>( m_mapPools.size() - 1 );
	return S_OK;
}
STDMETHODIMP CcvRTContext::GetCVIDs(LONG lLevelID, SAFEARRAY** pLevelsArray){
	cvMetaPoolMap::iterator itr = m_mapMetaPool.find( static_cast<cvID>(lLevelID) );
	if ( itr == m_mapMetaPool.end() ) 
		return E_INVALIDARG;
	cvMetaPoolPtr spMetaPool = itr->second;
	vector<cvID>	vecRequiredDP;
	HRESULT hr = E_FAIL;
	SAFEARRAYBOUND sab = { static_cast<ULONG>(spMetaPool->m_vecMCV.size()), 0 };
	LPSAFEARRAY psa = ::SafeArrayCreateEx( VT_I4, 1, &sab, NULL);
	if(psa) {
		unsigned int* pvData = NULL;
		if( SUCCEEDED( hr = ::SafeArrayAccessData( psa, (void**)&pvData) ) ) {
			for ( size_t s = 0; s < spMetaPool->m_vecMCV.size(); s++) {
				*pvData = static_cast<unsigned int>(spMetaPool->m_vecMCV[s]->m_ID);
				pvData++;
			}		
			hr = S_OK;
			::SafeArrayUnaccessData(psa);
			*pLevelsArray = psa;
		}
		else {
			DWORD dwError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwError);
		}
	}
	else 	{
		DWORD dwError = GetLastError();
		hr = HRESULT_FROM_WIN32(dwError);
	}
	return hr;
}
STDMETHODIMP CcvRTContext::GetCVInfo( LONG lLevelID, LONG lCVID, cvInfo* pInfo ){
	cvMetaPoolMap::iterator itr = m_mapMetaPool.find( static_cast<cvID>(lLevelID) );
	if ( itr == m_mapMetaPool.end() ) 
		return E_INVALIDARG;
	cvMetaPoolPtr spMetaPool = itr->second;

	if ( pInfo == NULL )
		pInfo = new cvInfo;

	for ( size_t s = 0; s < spMetaPool->m_vecMCV.size(); s++ ) {
		if(spMetaPool->m_vecMCV[s]->m_ID == static_cast<cvID>(lCVID) ){
			pInfo->ID = spMetaPool->m_vecMCV[s]->m_ID;
			pInfo->Name = SysAllocString( CComBSTR(spMetaPool->m_vecMCV[s]->m_strName.c_str() ) );
			pInfo->Format = SysAllocString( CComBSTR(spMetaPool->m_vecMCV[s]->m_format.c_str() ) );
			pInfo->Type = static_cast<cvType>( spMetaPool->m_vecMCV[s]->m_type );
			pInfo->Caption = SysAllocString( CComBSTR(spMetaPool->m_vecMCV[s]->m_caption.c_str() ) );
			return S_OK;
		}
	}
	return E_INVALIDARG;
}
STDMETHODIMP CcvRTContext::RemoveCVPool( LONG lPoolID, VARIANT_BOOL bClearAggOnly){
	CcvRTPool *pPool = GetPool( lPoolID );

	if ( !pPool) 
		return E_INVALIDARG;

	for( size_t s = 0; s < pPool->m_vecCV.size(); ++s){
		cvRTCVPtr pCV = pPool->m_vecCV[ s ];
		if( m_StaticData[ pCV->m_DataIndex ]->m_Type == cvtNumeric ) {
			cvNDataAtomPtr pNumericDataPtr = shared_dynamic_cast<cvNDataAtom>(m_StaticData[pCV->m_DataIndex ]);
			double dValue = pNumericDataPtr->GetData();
			for( size_t sAgg = 0; sAgg < pCV->m_vecAggregations.size(); sAgg++ ){
				unsigned int AggIndex = pCV->m_vecAggregations[ sAgg ];
				cvNDataAtomPtr pAggValue = shared_dynamic_cast<cvNDataAtom>( m_StaticData[ AggIndex ] );
				double dNewAggValue = pAggValue->GetData() - dValue;
				pAggValue->SetData( dNewAggValue );
			}

			if(bClearAggOnly)
			{
				dValue = 0.;
				pNumericDataPtr->SetData(dValue);
			}
		}
	}

	if(!bClearAggOnly)
		m_mapPools.erase(lPoolID);
	return S_OK;
}
STDMETHODIMP CcvRTContext::Clear( ){
	m_mapPools.clear();

	// erase static data used by pools ( actually these are custom variables values and aggregations )
	while ( m_StaticData.size() > m_idxStatics) 
		m_StaticData.pop_back();

	return S_OK;
}

STDMETHODIMP CcvRTContext::RegisterPoolDataProvider( LONG lPoolID, LONG lDataProviderLevelID, IcvDataProvider* pIcvDataProvider ){
	CcvRTPool *pPool = GetPool( lPoolID );
	if ( !pPool) 
		return E_INVALIDARG;
	return pPool->RegisterDataProvider( lDataProviderLevelID, pIcvDataProvider) ? S_OK : E_FAIL;
}
STDMETHODIMP CcvRTContext::GetLevelIDByName( BSTR strLevelName, LONG *pID ){
	_bstr_t bstrLevelName( strLevelName );
	string sLevelName = bstrLevelName;
	cvLevelsByNameMap::iterator itrLevel = m_mapLevels.find( sLevelName );
	if ( itrLevel == m_mapLevels.end() ){ 
		pID = 0;
		return E_INVALIDARG;
	}
	else
		*pID = static_cast<LONG>( itrLevel->second->m_ID );
	return S_OK;
}

STDMETHODIMP CcvRTContext::GetCVValue( LONG lPoolID, LONG lCVID, VARIANT *pRetVal){

	if ( m_bError) {
		VariantInit( pRetVal );
		pRetVal->dblVal = BAD_DOUBLE_VALUE;
		pRetVal->vt = VT_R8;
		return S_OK;
	}

	CcvRTPool *pPool = GetPool( lPoolID );
	if ( !pPool){
		VariantInit( pRetVal );
		pRetVal->dblVal = BAD_DOUBLE_VALUE;
		pRetVal->vt = VT_R8;
		return S_OK;
	}

	cvRTCVPtr spCV = pPool->GetCV( lCVID );
	if ( !spCV)	{ 
		VariantInit( pRetVal );
		pRetVal->dblVal = BAD_DOUBLE_VALUE;
		pRetVal->vt = VT_R8;
		return S_OK;
	}

	cvDataAtomBasePtr spDataAtom = m_StaticData[ spCV->m_DataIndex ];	
	VariantInit( pRetVal );
	switch( spDataAtom->m_Type ) {
		case cvtNumeric:
			{
				pRetVal->vt = VT_R8;
				cvNDataAtomPtr spNData = shared_dynamic_cast<cvNDataAtom>(spDataAtom);
				pRetVal->dblVal = spNData->GetData();
			}
			break;
		case cvtString:
			{	
				pRetVal->vt = VT_BSTR;
				cvSDataAtomPtr spSData = shared_dynamic_cast<cvSDataAtom>(spDataAtom);
				pRetVal->bstrVal = CComBSTR( spSData->GetData().c_str() ).Copy();
			}
			break;
		case cvtBoolean:
			{
				pRetVal->vt = VT_BOOL;
				cvBDataAtomPtr spBData = shared_dynamic_cast<cvBDataAtom>(spDataAtom);
				pRetVal->boolVal = spBData->GetData();
			}
			break;
		default:
			RaiseError( E_POINTER, _T("Runtime error: No data provider supplied for pool") );
	}
	return S_OK;
}
STDMETHODIMP CcvRTContext::GetRequiredDataProviderLevels(LONG lLevelID, SAFEARRAY** pLevelsArray)
{
	cvMetaPoolMap::iterator itr = m_mapMetaPool.find( static_cast<cvID>(lLevelID) );
	if ( itr == m_mapMetaPool.end() ) 
      return E_INVALIDARG;
	cvMetaPoolPtr spMetaPool = itr->second;
	vector<cvID>	vecRequiredDP;
	spMetaPool->GetRequiredDataProviderLevels(vecRequiredDP);

	if( !pLevelsArray )
		return E_POINTER;

	if(*pLevelsArray) {
		::SafeArrayDestroy(*pLevelsArray);
		*pLevelsArray = NULL;
	}  
	HRESULT hr = E_FAIL;
	SAFEARRAYBOUND sab = { static_cast<ULONG>(vecRequiredDP.size()), 0 };
	LPSAFEARRAY psa = ::SafeArrayCreateEx( VT_I4, 1, &sab, NULL);
	if(psa) {
		unsigned int* pvData = NULL;
		if( SUCCEEDED( hr = ::SafeArrayAccessData( psa, (void**)&pvData) ) ) {
			for ( size_t s = 0; s < vecRequiredDP.size(); s++) {
				*pvData = static_cast<unsigned int>(vecRequiredDP[s]);
				pvData++;
			}		
			hr = S_OK;
			::SafeArrayUnaccessData(psa);
			*pLevelsArray = psa;
		}
		else {
			DWORD dwError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwError);
		}
	}
	else 	{
		DWORD dwError = GetLastError();
		hr = HRESULT_FROM_WIN32(dwError);
	}
	return hr;
}

STDMETHODIMP CcvRTContext::BindAggregations(LONG lPoolID, LONG lAggregationPoolID)
{
	CcvRTPool *pPool = GetPool( lPoolID );
	if ( pPool ) {
		pPool->BindAggregations( lAggregationPoolID );
		return S_OK;
	}

	return E_INVALIDARG;
}

STDMETHODIMP CcvRTContext::Recalculate(LONG lPoolID)
{
	if ( m_bError) 
		return S_OK;

	bool bCalc = false;
	size_t nTest;
	if( bCalc)
		nTest = m_StaticData.GetSize();

	CcvRTPool *pPool = GetPool( lPoolID );

	if ( !pPool) 
		return E_INVALIDARG;
	//if ( pPool->m_mapCV.empty() ) 
	if ( pPool->m_vecCV.empty() ) 
		return S_OK;

	vector<cvMetaCVPtr>::iterator b = pPool->m_spMetaPool->m_vecMCV.begin();
	vector<cvMetaCVPtr>::iterator e = pPool->m_spMetaPool->m_vecMCV.end();
	
	for (; b != e; b++)
	{
		if ( (*b)->m_code.size() )
		{
			bCalc = true;
			break;
		}
	}

	if (! bCalc)
		return S_OK;

	try{
	
		size_t CurrentVarIndex = 0;

		//////////////////////////////////////////////////////////////////////////
		cvRTCVPtr pCV= pPool->m_vecCV[ CurrentVarIndex  ];
		size_t nextReferencedCmd = 0;
		cvDataReference DataRef, DataRef1;
		size_t thruCmdCounter = 0;

		
		// walk among CVs in referred meta pool  
		for ( size_t currentVarIndex = 0; currentVarIndex < pPool->m_spMetaPool->m_vecMCV.size();currentVarIndex++){

			if (static_cast<long>(currentVarIndex) == m_RecalculateCvCount)
				break;

			cvVMCommand CurCmd;

			// get current local CV 
			pCV= pPool->m_vecCV[ currentVarIndex  ];

			// walk thru CV's code
			for (size_t currentCmdIndex = 0;currentCmdIndex < pPool->m_spMetaPool->m_vecMCV[currentVarIndex]->m_code.size();currentCmdIndex++, thruCmdCounter++){

				// get command opcode
				CurCmd.m_OpCode = pPool->m_spMetaPool->m_vecMCV[currentVarIndex]->m_code[currentCmdIndex].m_OpCode;


				while ( thruCmdCounter > (size_t)(pPool->m_Program[nextReferencedCmd].m_OpCode) ) {
					if ( nextReferencedCmd >= ( pPool->m_Program.size()-1 ) )
						break;
					nextReferencedCmd++;
				}

				// get command's operand
				if (thruCmdCounter == (size_t)(pPool->m_Program[nextReferencedCmd].m_OpCode) ) {
					CurCmd.m_Operand = pPool->m_Program[nextReferencedCmd].m_Operand;
					if ( nextReferencedCmd < ( pPool->m_Program.size()-1 ) )
						nextReferencedCmd++;
				}
				else
					CurCmd.m_Operand = 	pPool->m_spMetaPool->m_vecMCV[currentVarIndex]->m_code[currentCmdIndex].m_Operand;

				if (CurCmd.m_OpCode == STOP ) 
				{
					thruCmdCounter++;
					break;
				}

				switch( CurCmd.m_OpCode ) {
			/*case STOP:
				{

					CurrentVarIndex++;
					if ( CurrentVarIndex == pPool->m_vecCV.size() ) 
						pCV = cvRTCVPtr();
					else
						pCV= pPool->m_vecCV[ CurrentVarIndex  ];

				}
				break;*/
			case POP:
				{

					DataRef = m_ExecStack.top();
					if ( DataRef.m_StorageType == cvDynamic) 
						m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
					m_ExecStack.pop(); 
				}
				break;
			case pushS:
			case pushG:
				{
					unsigned int levelID = CurCmd.m_Operand >> 16;
					unsigned int ID = CurCmd.m_Operand & 0xFFFF;

					// check whether this is reference to custom variable or system one
					if ( levelID == 0xFFFF ) {
						/*cvRTCVMap::iterator itrRefCV = pPool->m_mapCV.find( ID );
						if (itrRefCV == pPool->m_mapCV.end() ) 
							RaiseError( E_POINTER, _T("Runtime error: Bad custom variable ID in code"));
						cvRTCVPtr pRefCV = itrRefCV->second;*/
						cvRTCVPtr pRefCV = pPool->find( ID );
						if(! pRefCV )
							RaiseError( E_POINTER, _T("Runtime error: Bad custom variable ID in code"));
						DataRef.m_DataIndex = pRefCV->m_DataIndex;
						DataRef.m_StorageType = cvStatic;
						DataRef.m_Type = m_StaticData[ pRefCV->m_DataIndex]->m_Type;
						m_ExecStack.push( DataRef );
					}
					else{
						cvDataProvidersMap::iterator itr = pPool->m_mapDataProviders.find(levelID);
						if ( itr == pPool->m_mapDataProviders.end() ) 
							RaiseError( E_POINTER, _T("Runtime error: No data provider supplied for pool"));
						IcvDataProviderPtr pDP(itr->second);
						_variant_t	v;
						VARIANT vV;
						VariantInit(&vV);
						if(FAILED( pDP->GetData(ID, &vV) ) )
							RaiseError(E_FAIL, _T("Runtime error: Unable to retrieve data from data provider") );
						v = vV;
						if (v.vt == VT_R8) {
							double dVal = v;
							DataRef.m_DataIndex = m_DynamicData.Add( cvtNumeric, &dVal );
							DataRef.m_StorageType = cvDynamic;
							DataRef.m_Type = cvtNumeric;
							m_ExecStack.push( DataRef );
						}
						else if ( v.vt == VT_BOOL) {
							bool bVal = v;
							DataRef.m_DataIndex = m_DynamicData.Add( cvtBoolean, &bVal );
							DataRef.m_StorageType = cvDynamic;
							DataRef.m_Type = cvtBoolean;
							m_ExecStack.push( DataRef );
						}
						else if ( v.vt == VT_BSTR) {
							_bstr_t	bstrVal = v;
							string strVal = (LPCSTR) bstrVal;
							DataRef.m_DataIndex = m_DynamicData.Add( cvtString, &strVal );
							DataRef.m_StorageType = cvDynamic;
							DataRef.m_Type = cvtString;
							m_ExecStack.push( DataRef );
						}
						else 
							RaiseError(E_FAIL, _T("Runtime error: Data of unsupported type returned from data provider") );
					}
				}
				break;
			case	pushV:
			case	pushC:
				{
					DataRef.m_DataIndex = CurCmd.m_Operand;
					DataRef.m_StorageType = cvStatic;
					DataRef.m_Type = m_StaticData[ CurCmd.m_Operand]->m_Type;
					m_ExecStack.push( DataRef );
				}
				break;
			case	popV:
				{
					DataRef = m_ExecStack.top();
					if ( DataRef.m_StorageType == cvStatic  && DataRef.m_Type != m_StaticData[ DataRef.m_DataIndex ]->m_Type ) 
						RaiseError(E_FAIL, _T("Runtime error: Attempt to save change custom variable type from code") );

					// depends on type

					switch( DataRef.m_Type) {
			case cvtNumeric:
				{
					// get pointer to new data
					cvNDataAtomPtr spNewData = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
						m_StaticData[DataRef.m_DataIndex] :
					m_DynamicData.GetNData(DataRef.m_DataIndex ) );

					// get pointer to old data 
					cvNDataAtomPtr spOldData = shared_dynamic_cast<cvNDataAtom>( m_StaticData[ CurCmd.m_Operand] );


					size_t szAggCnt = 0; 
					if (CurCmd.m_Operand == pCV->m_DataIndex /*&& DataRef.m_StorageType == cvStatic*/ ) {
						// subtract old data from aggregations
						szAggCnt = pCV->m_vecAggregations.size();
						for ( size_t sa = 0; sa < szAggCnt; sa++ ) {
							cvNDataAtomPtr spAggData= shared_dynamic_cast<cvNDataAtom>( m_StaticData[ pCV->m_vecAggregations[sa] ] );
							double NewAggValue = spAggData->GetData() - spOldData->GetData();
							spAggData->SetData( NewAggValue );
						}
					}

					// set new value
					spOldData->SetData(spNewData->GetData());

					if ( szAggCnt ) {
						// add new value to aggregations
						for ( size_t sa = 0; sa < szAggCnt; sa++ ) {
							cvNDataAtomPtr spAggData= shared_dynamic_cast<cvNDataAtom>( m_StaticData[ pCV->m_vecAggregations[sa] ] );
							double NewAggValue = spAggData->GetData() + spOldData->GetData();
							spAggData->SetData( NewAggValue );
						}
					}
				}
				break;
			case cvtString:
				{
					if (pCV->m_vecAggregations.size() ) 
						RaiseError(E_FAIL, _T("Runtime error: Attempt to aggreagate non-numeric data") );

					// get pointer to new data
					cvSDataAtomPtr spNewData = shared_dynamic_cast<cvSDataAtom>( DataRef.m_StorageType == cvStatic ?
						m_StaticData[DataRef.m_DataIndex] :
					m_DynamicData.GetSData(DataRef.m_DataIndex ) );

					// get pointer to old data 
					cvSDataAtomPtr spOldData = shared_dynamic_cast<cvSDataAtom>( m_StaticData[ CurCmd.m_Operand ] );
					// set new value
					spOldData->SetData(spNewData->GetData());
				}
				break;
			case cvtBoolean:
				{
					if (pCV->m_vecAggregations.size() ) 
						RaiseError(E_FAIL, _T("Runtime error: Attempt to aggreagate non-numeric data") );
					// get pointer to new data
					cvBDataAtomPtr spNewData = shared_dynamic_cast<cvBDataAtom>( DataRef.m_StorageType == cvStatic ?
						m_StaticData[DataRef.m_DataIndex] :
					m_DynamicData.GetBData(DataRef.m_DataIndex ) );

					// get pointer to old data 
					cvBDataAtomPtr spOldData = shared_dynamic_cast<cvBDataAtom>( m_StaticData[ CurCmd.m_Operand ] );

					// set new value
					spOldData->SetData(spNewData->GetData());
				}
				break;
			default:
				RaiseError(E_FAIL, _T("Runtime error: Unknown data type") );
					}

					if ( DataRef.m_StorageType == cvDynamic) 
						m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );

					m_ExecStack.pop();
				}
				break;
			case	CMP:
				{
					if ( m_ExecStack.size() < 2 ) 
						RaiseError(E_FAIL, _T("Runtime error: no enough data") );
					DataRef = m_ExecStack.top();
					m_ExecStack.pop();
					DataRef1 = m_ExecStack.top();
					m_ExecStack.pop();
					if ( DataRef1.m_Type == DataRef.m_Type ) {
						if ( DataRef1.m_Type == cvtNumeric ) {
							// subtract two numerics

							// get data for first operand
							cvNDataAtomPtr sp1Data = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
								m_StaticData[DataRef.m_DataIndex] :
							m_DynamicData.GetNData(DataRef.m_DataIndex ) );

							// get data for second operand
							cvNDataAtomPtr sp2Data = shared_dynamic_cast<cvNDataAtom>( DataRef1.m_StorageType == cvStatic ?
								m_StaticData[DataRef1.m_DataIndex] :
							m_DynamicData.GetNData(DataRef1.m_DataIndex ) );

							// subtract them
							double dSum = sp2Data->GetData() - sp1Data->GetData();
							if ( DataRef.m_StorageType == cvDynamic) 
								m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
							if ( DataRef1.m_StorageType == cvDynamic) 
								m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

							// set flags
							if ( dSum == 0 ) 
								m_bGreater = m_bLess = false;
							else 
								m_bGreater = !(m_bLess = dSum < 0 ? true : false );
						}
						else if ( DataRef1.m_Type == cvtString) {
							// compare two strings

							// get data for first operand
							cvSDataAtomPtr sp1Data = shared_dynamic_cast<cvSDataAtom>( DataRef.m_StorageType == cvStatic ?
								m_StaticData[DataRef.m_DataIndex] :
							m_DynamicData.GetSData(DataRef.m_DataIndex ) );

							// get data for second operand
							cvSDataAtomPtr sp2Data = shared_dynamic_cast<cvSDataAtom>( DataRef1.m_StorageType == cvStatic ?
								m_StaticData[DataRef1.m_DataIndex] :
							m_DynamicData.GetSData(DataRef1.m_DataIndex ) );

							// compare them
							int Cmp = strcmp( sp1Data->GetData().c_str(), sp2Data->GetData().c_str());
							if ( DataRef.m_StorageType == cvDynamic) 
								m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
							if ( DataRef1.m_StorageType == cvDynamic) 
								m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

							// set flags
							if ( Cmp == 0 ) 
								m_bGreater = m_bLess = false;
							else 
								m_bGreater = !(m_bLess = Cmp < 0 ? true : false );
						}
						else if ( DataRef1.m_Type == cvtBoolean ) {
							// compare two booleans

							// get data for first operand
							cvBDataAtomPtr sp1Data = shared_dynamic_cast<cvBDataAtom>( DataRef.m_StorageType == cvStatic ?
								m_StaticData[DataRef.m_DataIndex] :
							m_DynamicData.GetBData(DataRef.m_DataIndex ) );

							// get data for second operand
							cvBDataAtomPtr sp2Data = shared_dynamic_cast<cvBDataAtom>( DataRef1.m_StorageType == cvStatic ?
								m_StaticData[DataRef1.m_DataIndex] :
							m_DynamicData.GetBData(DataRef1.m_DataIndex ) );

							// subtract them
							bool Cmp = sp1Data->GetData() == sp2Data->GetData();
							if ( DataRef.m_StorageType == cvDynamic) 
								m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
							if ( DataRef1.m_StorageType == cvDynamic) 
								m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

							// set flags
							if ( Cmp ) 
								m_bGreater = m_bLess = false;
							else 
								m_bGreater = m_bLess = true;
						}
						else
							RaiseError(E_FAIL, _T("Runtime error: Comparing data of unknown types") );
					}
					else
						RaiseError(E_FAIL, _T("Runtime error: Comparing data of different types") );
				}
				break;
			case	cmpZ:
				{
					if ( m_ExecStack.size() < 1 ) 
						RaiseError(E_FAIL, _T("Runtime error: no enough data") );
					DataRef = m_ExecStack.top();
					m_ExecStack.pop();
					//if ( DataRef.m_Type != DataRef.m_Type ) {
					if ( DataRef.m_Type == cvtNumeric ) {
						// subtract two numerics

						// get data for first operand
						cvNDataAtomPtr sp1Data = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
							m_StaticData[DataRef.m_DataIndex] :
						m_DynamicData.GetNData(DataRef.m_DataIndex ) );

						// subtract them
						double dSum = 0 - sp1Data->GetData();
						if ( DataRef.m_StorageType == cvDynamic) 
							m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );

						// set flags
						if ( dSum == 0 ) 
							m_bGreater = m_bLess = false;
						else 
							m_bGreater = !(m_bLess = dSum < 0 ? false : true );
					}
					else if ( DataRef.m_Type == cvtString) {
						// compare two strings

						// get data for first operand
						cvSDataAtomPtr sp1Data = shared_dynamic_cast<cvSDataAtom>( DataRef.m_StorageType == cvStatic ?
							m_StaticData[DataRef.m_DataIndex] :
						m_DynamicData.GetSData(DataRef.m_DataIndex ) );


						// compare them
						size_t Cmp = sp1Data->GetData().length();
						if ( DataRef.m_StorageType == cvDynamic) 
							m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );

						// set flags
						if ( Cmp == 0 ) 
							m_bGreater = m_bLess = false;
						else 
							m_bGreater = !(m_bLess = false);
					}
					else if ( DataRef.m_Type == cvtBoolean ) {
						// compare two booleans

						// get data for first operand
						cvBDataAtomPtr sp1Data = shared_dynamic_cast<cvBDataAtom>( DataRef.m_StorageType == cvStatic ?
							m_StaticData[DataRef.m_DataIndex] :
						m_DynamicData.GetBData(DataRef.m_DataIndex ) );

						// subtract them
						bool Cmp = ( sp1Data->GetData() == false );
						if ( DataRef.m_StorageType == cvDynamic) 
							m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );

						// set flags
						if ( Cmp ) 
							m_bGreater = m_bLess = false;
						else 
							m_bGreater = m_bLess = true;
					}
					else
						RaiseError(E_FAIL, _T("Runtime error: Comparing data of unknown types") );
					//}
					//else
					//	RaiseError(E_FAIL, _T("Runtime error: Comparing data of different types") );
				}
				break;
			case	ADD:
				{
					if ( m_ExecStack.size() < 2 ) 
						RaiseError(E_FAIL, _T("Runtime error: no enough data") );
					DataRef = m_ExecStack.top();
					m_ExecStack.pop();
					DataRef1 = m_ExecStack.top();
					m_ExecStack.pop();
					if ( DataRef1.m_Type == DataRef.m_Type && ( DataRef.m_Type == cvtNumeric || DataRef.m_Type == cvtString) ) {
						if ( DataRef.m_Type == cvtNumeric ) {
							// add two numerics

							// get data for first operand
							cvNDataAtomPtr sp1Data = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
								m_StaticData[DataRef.m_DataIndex] :
							m_DynamicData.GetNData(DataRef.m_DataIndex ) );

							// get data for second operand
							cvNDataAtomPtr sp2Data = shared_dynamic_cast<cvNDataAtom>( DataRef1.m_StorageType == cvStatic ?
								m_StaticData[DataRef1.m_DataIndex] :
							m_DynamicData.GetNData(DataRef1.m_DataIndex ) );

							// add them
							double dSum = sp1Data->GetData() + sp2Data->GetData();
							if ( DataRef.m_StorageType == cvDynamic) 
								m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
							if ( DataRef1.m_StorageType == cvDynamic) 
								m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

							// create temporary data and store it in stack
							DataRef.m_DataIndex = m_DynamicData.Add( cvtNumeric, &dSum );
							DataRef.m_StorageType = cvDynamic;
							DataRef.m_Type = cvtNumeric;
							m_ExecStack.push( DataRef );

							// set flags
							if ( dSum == 0 ) 
								m_bGreater = m_bLess = false;
							else 
								m_bGreater = !(m_bLess = dSum < 0 ? true : false );
						}
						else{
							// add two strings

							// get data for first operand
							cvSDataAtomPtr sp1Data = shared_dynamic_cast<cvSDataAtom>( DataRef.m_StorageType == cvStatic ?
								m_StaticData[DataRef.m_DataIndex] :
							m_DynamicData.GetSData(DataRef.m_DataIndex ) );

							// get data for second operand
							cvSDataAtomPtr sp2Data = shared_dynamic_cast<cvSDataAtom>( DataRef1.m_StorageType == cvStatic ?
								m_StaticData[DataRef1.m_DataIndex] :
							m_DynamicData.GetSData(DataRef1.m_DataIndex ) );

							// add them
							string strSum = sp1Data->GetData() + sp2Data->GetData();
							if ( DataRef.m_StorageType == cvDynamic) 
								m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
							if ( DataRef1.m_StorageType == cvDynamic) 
								m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

							// create temporary data and store it in stack
							DataRef.m_DataIndex = m_DynamicData.Add( cvtNumeric, &strSum );
							DataRef.m_StorageType = cvDynamic;
							DataRef.m_Type = cvtNumeric;
							m_ExecStack.push( DataRef );

							// set flags
							if ( strSum.length() != 0 ) 
								m_bGreater = !(m_bLess = false);
							else 
								m_bGreater = m_bLess = false;
						}
					}
					else
						RaiseError(E_FAIL, _T("Runtime error: Bad types of data for addition") );
				}
				break;
			case	SUB:
				{
					if ( m_ExecStack.size() < 2 ) 
						RaiseError(E_FAIL, _T("Runtime error: no enough data") );
					DataRef = m_ExecStack.top();
					m_ExecStack.pop();
					DataRef1 = m_ExecStack.top();
					m_ExecStack.pop();
					if ( DataRef1.m_Type == DataRef.m_Type && DataRef.m_Type == cvtNumeric ) {
						// subtract two numerics

						// get data for first operand
						cvNDataAtomPtr sp1Data = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
							m_StaticData[DataRef.m_DataIndex] :
						m_DynamicData.GetNData(DataRef.m_DataIndex ) );

						// get data for second operand
						cvNDataAtomPtr sp2Data = shared_dynamic_cast<cvNDataAtom>( DataRef1.m_StorageType == cvStatic ?
							m_StaticData[DataRef1.m_DataIndex] :
						m_DynamicData.GetNData(DataRef1.m_DataIndex ) );

						// add them
						double dSum = sp2Data->GetData() - sp1Data->GetData();
						if ( DataRef.m_StorageType == cvDynamic) 
							m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
						if ( DataRef1.m_StorageType == cvDynamic) 
							m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

						// create temporary data and store it in stack
						DataRef.m_DataIndex = m_DynamicData.Add( cvtNumeric, &dSum );
						DataRef.m_StorageType = cvDynamic;
						DataRef.m_Type = cvtNumeric;
						m_ExecStack.push( DataRef );

						// set flags
						if ( dSum == 0 ) 
							m_bGreater = m_bLess = false;
						else 
							m_bGreater = !(m_bLess = dSum < 0 ? true : false );
					}
					else
						RaiseError(E_FAIL, _T("Runtime error: Bad types of data for subtraction") );
				}
				break;
			case	DIV:
				{
					if ( m_ExecStack.size() < 2 ) 
						RaiseError(E_FAIL, _T("Runtime error: no enough data") );
					DataRef = m_ExecStack.top();
					m_ExecStack.pop();
					DataRef1 = m_ExecStack.top();
					m_ExecStack.pop();
					if ( DataRef1.m_Type == DataRef.m_Type && DataRef.m_Type == cvtNumeric ) {
						// divide two numerics

						// get data for first operand
						cvNDataAtomPtr sp1Data = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
							m_StaticData[DataRef.m_DataIndex] :
						m_DynamicData.GetNData(DataRef.m_DataIndex ) );

						// get data for second operand
						cvNDataAtomPtr sp2Data = shared_dynamic_cast<cvNDataAtom>( DataRef1.m_StorageType == cvStatic ?
							m_StaticData[DataRef1.m_DataIndex] :
						m_DynamicData.GetNData(DataRef1.m_DataIndex ) );

						// add them
						double dSum = (sp1Data->GetData() != 0.0) ? ( sp2Data->GetData() /  sp1Data->GetData() ) : 0.0;
						if ( DataRef.m_StorageType == cvDynamic) 
							m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
						if ( DataRef1.m_StorageType == cvDynamic) 
							m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

						// create temporary data and store it in stack
						DataRef.m_DataIndex = m_DynamicData.Add( cvtNumeric, &dSum );
						DataRef.m_StorageType = cvDynamic;
						DataRef.m_Type = cvtNumeric;
						m_ExecStack.push( DataRef );

						// set flags
						if ( dSum == 0 ) 
							m_bGreater = m_bLess = false;
						else 
							m_bGreater = !(m_bLess = dSum < 0 ? true : false );
					}
					else
						RaiseError(E_FAIL, _T("Runtime error: Bad types of data for division") );
				}
				break;
			case	MUL:
				{
					if ( m_ExecStack.size() < 2 ) 
						RaiseError(E_FAIL, _T("Runtime error: no enough data") );
					DataRef = m_ExecStack.top();
					m_ExecStack.pop();
					DataRef1 = m_ExecStack.top();
					m_ExecStack.pop();
					if ( DataRef1.m_Type == DataRef.m_Type && DataRef.m_Type == cvtNumeric ) {
						// multiply two numerics

						// get data for first operand
						cvNDataAtomPtr sp1Data = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
							m_StaticData[DataRef.m_DataIndex] :
						m_DynamicData.GetNData(DataRef.m_DataIndex ) );

						// get data for second operand
						cvNDataAtomPtr sp2Data = shared_dynamic_cast<cvNDataAtom>( DataRef1.m_StorageType == cvStatic ?
							m_StaticData[DataRef1.m_DataIndex] :
						m_DynamicData.GetNData(DataRef1.m_DataIndex ) );

						// add them
						double dSum = sp1Data->GetData() * sp2Data->GetData();
						if ( DataRef.m_StorageType == cvDynamic) 
							m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
						if ( DataRef1.m_StorageType == cvDynamic) 
							m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

						// create temporary data and store it in stack
						DataRef.m_DataIndex = m_DynamicData.Add( cvtNumeric, &dSum );
						DataRef.m_StorageType = cvDynamic;
						DataRef.m_Type = cvtNumeric;
						m_ExecStack.push( DataRef );

						// set flags
						if ( dSum == 0 ) 
							m_bGreater = m_bLess = false;
						else 
							m_bGreater = !(m_bLess = dSum < 0 ? true : false );
					}
					else
						RaiseError(E_FAIL, _T("Runtime error: Bad types of data for multiplication") );
				}
				break;
			case	iDIV:
				break;
			case	MOD:
				break;
			case	JMP:
				{
					currentCmdIndex += CurCmd.m_Operand;
					currentCmdIndex--;

					thruCmdCounter += CurCmd.m_Operand;
					thruCmdCounter--;
				}
				break;
			case	jGE:
				if ( !m_bLess) {
					currentCmdIndex += CurCmd.m_Operand;				
					currentCmdIndex--;

					thruCmdCounter += CurCmd.m_Operand;
					thruCmdCounter--;
				}
				break;
			case	jLE:
				if ( !m_bGreater) {
					currentCmdIndex += CurCmd.m_Operand;				
					currentCmdIndex--;

					thruCmdCounter += CurCmd.m_Operand;
					thruCmdCounter--;
				}
				break;
			case	jG:
				if ( m_bGreater) {
					currentCmdIndex += CurCmd.m_Operand;				
					currentCmdIndex--;

					thruCmdCounter += CurCmd.m_Operand;
					thruCmdCounter--;
				}
				break;
			case	jNE:
				if ( m_bGreater || m_bLess	) {
					currentCmdIndex += CurCmd.m_Operand;				
					currentCmdIndex--;

					thruCmdCounter += CurCmd.m_Operand;
					thruCmdCounter--;
				}
				break;
			case	jL:
				if ( m_bLess) {
					currentCmdIndex += CurCmd.m_Operand;				
					currentCmdIndex--;

					thruCmdCounter += CurCmd.m_Operand;
					thruCmdCounter--;
				}
				break;
			case	jE:
				if ( !m_bLess && !m_bGreater ) {
					currentCmdIndex += CurCmd.m_Operand;				
					currentCmdIndex--;

					thruCmdCounter += CurCmd.m_Operand;
					thruCmdCounter--;
				}
				break;
			case	NEG:
				{
					if ( m_ExecStack.size() < 1 ) 
						RaiseError(E_FAIL, _T("Runtime error: no enough data") );
					DataRef = m_ExecStack.top();
					if ( DataRef.m_Type == cvtNumeric ) {

						// get data for operand
						cvNDataAtomPtr sp1Data = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
							m_StaticData[DataRef.m_DataIndex] :
						m_DynamicData.GetNData(DataRef.m_DataIndex ) );
						double Value = sp1Data->GetData() * -1;
						sp1Data->SetData(Value);
					}
					else
						RaiseError(E_FAIL, _T("Runtime error: Bad type for negation") );
				}
				break;
			case	stoV:
			default:
				{
					// is this a function call?
					if ( static_cast<unsigned int>( CurCmd.m_OpCode)  & static_cast<unsigned int>( callF )  ) {
						// yes

						cvExecutionStack tmpstack;
						size_t ParamsCount = static_cast<unsigned int>( CurCmd.m_OpCode ) & 0x0000FFFF;
						if ( m_ExecStack.size() < ParamsCount )
							RaiseError( E_FAIL, _T("Runtime error: No enough parameters for function call") );

						// put parameters into safearray
						SAFEARRAYBOUND sab = { static_cast<ULONG>(ParamsCount), 0 };
						LPSAFEARRAY psa = ::SafeArrayCreateEx( VT_VARIANT, 1, &sab, NULL);

						_variant_t	vtParam;
						//VARIANT svtParam;
						//VariantInit( &svtParam );
						LONG uParamIndex = 0;
						for ( size_t Param = 0; Param < ParamsCount; Param++) {
							DataRef = m_ExecStack.top();
							switch( DataRef.m_Type ) {
								case cvtNumeric:
									{
										cvNDataAtomPtr spNData = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
											m_StaticData[DataRef.m_DataIndex] :
										m_DynamicData.GetNData(DataRef.m_DataIndex ) );
										vtParam = spNData->GetData();
									}
									break;
								case cvtString:
									{
										cvSDataAtomPtr spSData = shared_dynamic_cast<cvSDataAtom>( DataRef.m_StorageType == cvStatic ?
											m_StaticData[DataRef.m_DataIndex] :
										m_DynamicData.GetSData(DataRef.m_DataIndex ) );
										vtParam = spSData->GetData().c_str();
									}
									break;
								case cvtBoolean:
									{
										cvBDataAtomPtr spBData = shared_dynamic_cast<cvBDataAtom>( DataRef.m_StorageType == cvStatic ?
											m_StaticData[DataRef.m_DataIndex] :
										m_DynamicData.GetBData(DataRef.m_DataIndex ) );
										vtParam = spBData->GetData();
									}
									break;
								default:	;
												}
							//svtParam = vtParam;
							m_ExecStack.pop();
							if ( DataRef.m_StorageType == cvDynamic ) 
								m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );

							SafeArrayPutElement( psa, &uParamIndex, &vtParam );
							uParamIndex++;
						}


						// call function from corresponding level
						unsigned int levelID = CurCmd.m_Operand >> 16;
						unsigned int ID = CurCmd.m_Operand & 0xFFFF;

						cvDataProvidersMap::iterator itr = pPool->m_mapDataProviders.find(levelID);
						if ( itr == pPool->m_mapDataProviders.end() ) 
							RaiseError( E_POINTER, _T("Runtime error: No data provider supplied for pool"));
						IcvDataProviderPtr pDP(itr->second);

						if(FAILED( pDP->CallFunction( ID, &psa, &vtParam) ) )
							RaiseError(E_FAIL, _T("Runtime error: Unable to retrieve data from data provider") );

						::SafeArrayDestroy(psa);

						//vtParam = svtParam;
						if (vtParam.vt == VT_R8) {
							double dVal = vtParam;
							DataRef.m_DataIndex = m_DynamicData.Add( cvtNumeric, &dVal );
							DataRef.m_StorageType = cvDynamic;
							DataRef.m_Type = cvtNumeric;
							m_ExecStack.push( DataRef );
						}
						else if ( vtParam.vt == VT_BOOL) {
							bool bVal = vtParam;
							DataRef.m_DataIndex = m_DynamicData.Add( cvtBoolean, &bVal );
							DataRef.m_StorageType = cvDynamic;
							DataRef.m_Type = cvtBoolean;
							m_ExecStack.push( DataRef );
						}
						else if ( vtParam.vt == VT_BSTR) {
							_bstr_t	bstrVal = vtParam;
							string strVal = (LPCSTR) bstrVal;
							DataRef.m_DataIndex = m_DynamicData.Add( cvtString, &strVal );
							DataRef.m_StorageType = cvDynamic;
							DataRef.m_Type = cvtString;
							m_ExecStack.push( DataRef );
						}
						else 
							RaiseError(E_FAIL, _T("Runtime error: Data of unsupported type returned from data provider") );
					}
					else
						RaiseError( E_FAIL, _T("Runtime error: Unknown opcode in program") );
				}
				}

			}
		}
		//////////////////////////////////////////////////////////////////////////
		//for ( size_t cmd = 0; cmd < pPool->m_Program.size(); cmd++) {
		//	cvDataReference DataRef, DataRef1;
		//	cvVMCommand CurCmd;
		//	CurCmd.m_OpCode = pPool->m_Program[cmd].m_OpCode;
		//	CurCmd.m_Operand= pPool->m_Program[cmd].m_Operand;
		//	switch( pPool->m_Program[cmd].m_OpCode ) {
		//	case STOP:
		//		{

		//			CurrentVarIndex++;
		//			if ( CurrentVarIndex == pPool->m_vecCV.size() ) 
		//				pCV = cvRTCVPtr();
		//			else
		//				pCV= pPool->m_vecCV[ CurrentVarIndex  ];
		//		
		//		}
		//		break;
		//	case POP:
		//		{

		//			DataRef = m_ExecStack.top();
		//			if ( DataRef.m_StorageType == cvDynamic) 
		//				m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
		//			m_ExecStack.pop(); 
		//		}
		//		break;
		//	case pushS:
		//	case pushG:
		//		{
		//			unsigned int levelID = CurCmd.m_Operand >> 16;
		//			unsigned int ID = CurCmd.m_Operand & 0xFFFF;

		//			// check whether this is reference to custom variable or system one
		//			if ( levelID == 0xFFFF ) {
		//				cvRTCVMap::iterator itrRefCV = pPool->m_mapCV.find( ID );
		//				if (itrRefCV == pPool->m_mapCV.end() ) 
		//					RaiseError( E_POINTER, _T("Runtime error: Bad custom variable ID in code"));
		//				cvRTCVPtr pRefCV = itrRefCV->second;
		//				DataRef.m_DataIndex = pRefCV->m_DataIndex;
		//				DataRef.m_StorageType = cvStatic;
		//				DataRef.m_Type = m_StaticData[ pRefCV->m_DataIndex]->m_Type;
		//				m_ExecStack.push( DataRef );
		//			}
		//			else{
		//				cvDataProvidersMap::iterator itr = pPool->m_mapDataProviders.find(levelID);
		//				if ( itr == pPool->m_mapDataProviders.end() ) 
		//					RaiseError( E_POINTER, _T("Runtime error: No data provider supplied for pool"));
		//				IcvDataProviderPtr pDP(itr->second);
		//				_variant_t	v;
		//				VARIANT vV;
		//				VariantInit(&vV);
		//				if(FAILED( pDP->GetData(ID, &vV) ) )
		//					RaiseError(E_FAIL, _T("Runtime error: Unable to retrieve data from data provider") );
		//				v = vV;
		//				if (v.vt == VT_R8) {
		//					double dVal = v;
		//					DataRef.m_DataIndex = m_DynamicData.Add( cvtNumeric, &dVal );
		//					DataRef.m_StorageType = cvDynamic;
		//					DataRef.m_Type = cvtNumeric;
		//					m_ExecStack.push( DataRef );
		//				}
		//				else if ( v.vt == VT_BOOL) {
		//					bool bVal = v;
		//					DataRef.m_DataIndex = m_DynamicData.Add( cvtBoolean, &bVal );
		//					DataRef.m_StorageType = cvDynamic;
		//					DataRef.m_Type = cvtBoolean;
		//					m_ExecStack.push( DataRef );
		//				}
		//				else if ( v.vt == VT_BSTR) {
		//					_bstr_t	bstrVal = v;
		//					string strVal = (LPCSTR) bstrVal;
		//					DataRef.m_DataIndex = m_DynamicData.Add( cvtString, &strVal );
		//					DataRef.m_StorageType = cvDynamic;
		//					DataRef.m_Type = cvtString;
		//					m_ExecStack.push( DataRef );
		//				}
		//				else 
		//					RaiseError(E_FAIL, _T("Runtime error: Data of unsupported type returned from data provider") );
		//			}
		//		}
		//		break;
		//	case	pushV:
		//	case	pushC:
		//		{
		//			DataRef.m_DataIndex = CurCmd.m_Operand;
		//			DataRef.m_StorageType = cvStatic;
		//			DataRef.m_Type = m_StaticData[ CurCmd.m_Operand]->m_Type;
		//			m_ExecStack.push( DataRef );
		//		}
		//		break;
		//	case	popV:
		//		{
		//			DataRef = m_ExecStack.top();
		//			if ( DataRef.m_StorageType == cvStatic  && DataRef.m_Type != m_StaticData[ DataRef.m_DataIndex ]->m_Type ) 
		//				RaiseError(E_FAIL, _T("Runtime error: Attempt to save change custom variable type from code") );
		//		
		//			// depends on type

		//			switch( DataRef.m_Type) {
		//				case cvtNumeric:
		//					{
		//						// get pointer to new data
		//						cvNDataAtomPtr spNewData = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
		//							m_StaticData[DataRef.m_DataIndex] :
		//							m_DynamicData.GetNData(DataRef.m_DataIndex ) );

		//						// get pointer to old data 
		//						cvNDataAtomPtr spOldData = shared_dynamic_cast<cvNDataAtom>( m_StaticData[ CurCmd.m_Operand] );

		//						
		//						size_t szAggCnt = 0; 
		//						if (CurCmd.m_Operand == pCV->m_DataIndex /*&& DataRef.m_StorageType == cvStatic*/ ) {
		//							// subtract old data from aggregations
		//							szAggCnt = pCV->m_vecAggregations.size();
		//							for ( size_t sa = 0; sa < szAggCnt; sa++ ) {
		//								cvNDataAtomPtr spAggData= shared_dynamic_cast<cvNDataAtom>( m_StaticData[ pCV->m_vecAggregations[sa] ] );
		//								double NewAggValue = spAggData->GetData() - spOldData->GetData();
		//								spAggData->SetData( NewAggValue );
		//							}
		//						}

		//						// set new value
		//						spOldData->SetData(spNewData->GetData());

		//						if ( szAggCnt ) {
		//							// add new value to aggregations
		//							for ( size_t sa = 0; sa < szAggCnt; sa++ ) {
		//								cvNDataAtomPtr spAggData= shared_dynamic_cast<cvNDataAtom>( m_StaticData[ pCV->m_vecAggregations[sa] ] );
		//								double NewAggValue = spAggData->GetData() + spOldData->GetData();
		//								spAggData->SetData( NewAggValue );
		//							}
		//						}
		//					}
		//					break;
		//				case cvtString:
		//					{
		//						if (pCV->m_vecAggregations.size() ) 
		//							RaiseError(E_FAIL, _T("Runtime error: Attempt to aggreagate non-numeric data") );

		//						// get pointer to new data
		//						cvSDataAtomPtr spNewData = shared_dynamic_cast<cvSDataAtom>( DataRef.m_StorageType == cvStatic ?
		//							m_StaticData[DataRef.m_DataIndex] :
		//						m_DynamicData.GetSData(DataRef.m_DataIndex ) );

		//						// get pointer to old data 
		//						cvSDataAtomPtr spOldData = shared_dynamic_cast<cvSDataAtom>( m_StaticData[ CurCmd.m_Operand ] );
		//						// set new value
		//						spOldData->SetData(spNewData->GetData());
		//					}
		//					break;
		//				case cvtBoolean:
		//					{
		//						if (pCV->m_vecAggregations.size() ) 
		//							RaiseError(E_FAIL, _T("Runtime error: Attempt to aggreagate non-numeric data") );
		//						// get pointer to new data
		//						cvBDataAtomPtr spNewData = shared_dynamic_cast<cvBDataAtom>( DataRef.m_StorageType == cvStatic ?
		//							m_StaticData[DataRef.m_DataIndex] :
		//						m_DynamicData.GetBData(DataRef.m_DataIndex ) );

		//						// get pointer to old data 
		//						cvBDataAtomPtr spOldData = shared_dynamic_cast<cvBDataAtom>( m_StaticData[ CurCmd.m_Operand ] );

		//						// set new value
		//						spOldData->SetData(spNewData->GetData());
		//					}
		//					break;
		//				default:
		//					RaiseError(E_FAIL, _T("Runtime error: Unknown data type") );
		//			}

		//			if ( DataRef.m_StorageType == cvDynamic) 
		//				m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );

		//			m_ExecStack.pop();
		//		}
		//		break;
		//	case	CMP:
		//		{
		//			if ( m_ExecStack.size() < 2 ) 
		//				RaiseError(E_FAIL, _T("Runtime error: no enough data") );
		//			DataRef = m_ExecStack.top();
		//			m_ExecStack.pop();
		//			DataRef1 = m_ExecStack.top();
		//			m_ExecStack.pop();
		//			if ( DataRef1.m_Type == DataRef.m_Type ) {
		//				if ( DataRef1.m_Type == cvtNumeric ) {
		//					// subtract two numerics

		//					// get data for first operand
		//					cvNDataAtomPtr sp1Data = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
		//						m_StaticData[DataRef.m_DataIndex] :
		//					m_DynamicData.GetNData(DataRef.m_DataIndex ) );

		//					// get data for second operand
		//					cvNDataAtomPtr sp2Data = shared_dynamic_cast<cvNDataAtom>( DataRef1.m_StorageType == cvStatic ?
		//						m_StaticData[DataRef1.m_DataIndex] :
		//					m_DynamicData.GetNData(DataRef1.m_DataIndex ) );

		//					// subtract them
		//					double dSum = sp2Data->GetData() - sp1Data->GetData();
		//					if ( DataRef.m_StorageType == cvDynamic) 
		//						m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
		//					if ( DataRef1.m_StorageType == cvDynamic) 
		//						m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

		//					// set flags
		//					if ( dSum == 0 ) 
		//						m_bGreater = m_bLess = false;
		//					else 
		//						m_bGreater = !(m_bLess = dSum < 0 ? true : false );
		//				}
		//				else if ( DataRef1.m_Type == cvtString) {
		//					// compare two strings

		//					// get data for first operand
		//					cvSDataAtomPtr sp1Data = shared_dynamic_cast<cvSDataAtom>( DataRef.m_StorageType == cvStatic ?
		//						m_StaticData[DataRef.m_DataIndex] :
		//					m_DynamicData.GetSData(DataRef.m_DataIndex ) );

		//					// get data for second operand
		//					cvSDataAtomPtr sp2Data = shared_dynamic_cast<cvSDataAtom>( DataRef1.m_StorageType == cvStatic ?
		//						m_StaticData[DataRef1.m_DataIndex] :
		//					m_DynamicData.GetSData(DataRef1.m_DataIndex ) );

		//					// compare them
		//					int Cmp = strcmp( sp1Data->GetData().c_str(), sp2Data->GetData().c_str());
		//					if ( DataRef.m_StorageType == cvDynamic) 
		//						m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
		//					if ( DataRef1.m_StorageType == cvDynamic) 
		//						m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

		//					// set flags
		//					if ( Cmp == 0 ) 
		//						m_bGreater = m_bLess = false;
		//					else 
		//						m_bGreater = !(m_bLess = Cmp < 0 ? true : false );
		//				}
		//				else if ( DataRef1.m_Type == cvtBoolean ) {
		//					// compare two booleans

		//					// get data for first operand
		//					cvBDataAtomPtr sp1Data = shared_dynamic_cast<cvBDataAtom>( DataRef.m_StorageType == cvStatic ?
		//						m_StaticData[DataRef.m_DataIndex] :
		//					m_DynamicData.GetBData(DataRef.m_DataIndex ) );

		//					// get data for second operand
		//					cvBDataAtomPtr sp2Data = shared_dynamic_cast<cvBDataAtom>( DataRef1.m_StorageType == cvStatic ?
		//						m_StaticData[DataRef1.m_DataIndex] :
		//					m_DynamicData.GetBData(DataRef1.m_DataIndex ) );

		//					// subtract them
		//					bool Cmp = sp1Data->GetData() == sp2Data->GetData();
		//					if ( DataRef.m_StorageType == cvDynamic) 
		//						m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
		//					if ( DataRef1.m_StorageType == cvDynamic) 
		//						m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

		//					// set flags
		//					if ( Cmp ) 
		//						m_bGreater = m_bLess = false;
		//					else 
		//						m_bGreater = m_bLess = true;
		//				}
		//				else
		//					RaiseError(E_FAIL, _T("Runtime error: Comparing data of unknown types") );
		//			}
		//			else
		//				RaiseError(E_FAIL, _T("Runtime error: Comparing data of different types") );
		//		}
		//		break;
		//	case	cmpZ:
		//		{
		//			if ( m_ExecStack.size() < 1 ) 
		//				RaiseError(E_FAIL, _T("Runtime error: no enough data") );
		//			DataRef = m_ExecStack.top();
		//			m_ExecStack.pop();
		//			//if ( DataRef.m_Type != DataRef.m_Type ) {
		//				if ( DataRef.m_Type == cvtNumeric ) {
		//					// subtract two numerics

		//					// get data for first operand
		//					cvNDataAtomPtr sp1Data = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
		//						m_StaticData[DataRef.m_DataIndex] :
		//					m_DynamicData.GetNData(DataRef.m_DataIndex ) );

		//					// subtract them
		//					double dSum = 0 - sp1Data->GetData();
		//					if ( DataRef.m_StorageType == cvDynamic) 
		//						m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );

		//					// set flags
		//					if ( dSum == 0 ) 
		//						m_bGreater = m_bLess = false;
		//					else 
		//						m_bGreater = !(m_bLess = dSum < 0 ? false : true );
		//				}
		//				else if ( DataRef.m_Type == cvtString) {
		//					// compare two strings

		//					// get data for first operand
		//					cvSDataAtomPtr sp1Data = shared_dynamic_cast<cvSDataAtom>( DataRef.m_StorageType == cvStatic ?
		//						m_StaticData[DataRef.m_DataIndex] :
		//					m_DynamicData.GetSData(DataRef.m_DataIndex ) );


		//					// compare them
		//					size_t Cmp = sp1Data->GetData().length();
		//					if ( DataRef.m_StorageType == cvDynamic) 
		//						m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );

		//					// set flags
		//					if ( Cmp == 0 ) 
		//						m_bGreater = m_bLess = false;
		//					else 
		//						m_bGreater = !(m_bLess = false);
		//				}
		//				else if ( DataRef.m_Type == cvtBoolean ) {
		//					// compare two booleans

		//					// get data for first operand
		//					cvBDataAtomPtr sp1Data = shared_dynamic_cast<cvBDataAtom>( DataRef.m_StorageType == cvStatic ?
		//						m_StaticData[DataRef.m_DataIndex] :
		//					m_DynamicData.GetBData(DataRef.m_DataIndex ) );

		//					// subtract them
		//					bool Cmp = ( sp1Data->GetData() == false );
		//					if ( DataRef.m_StorageType == cvDynamic) 
		//						m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );

		//					// set flags
		//					if ( Cmp ) 
		//						m_bGreater = m_bLess = false;
		//					else 
		//						m_bGreater = m_bLess = true;
		//				}
		//				else
		//					RaiseError(E_FAIL, _T("Runtime error: Comparing data of unknown types") );
		//			//}
		//			//else
		//			//	RaiseError(E_FAIL, _T("Runtime error: Comparing data of different types") );
		//		}
		//		break;
		//	case	ADD:
		//		{
		//			if ( m_ExecStack.size() < 2 ) 
		//				RaiseError(E_FAIL, _T("Runtime error: no enough data") );
		//			DataRef = m_ExecStack.top();
		//			m_ExecStack.pop();
		//			DataRef1 = m_ExecStack.top();
		//			m_ExecStack.pop();
		//			if ( DataRef1.m_Type == DataRef.m_Type && ( DataRef.m_Type == cvtNumeric || DataRef.m_Type == cvtString) ) {
		//				if ( DataRef.m_Type == cvtNumeric ) {
		//					// add two numerics

		//					// get data for first operand
		//					cvNDataAtomPtr sp1Data = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
		//						m_StaticData[DataRef.m_DataIndex] :
		//					m_DynamicData.GetNData(DataRef.m_DataIndex ) );

		//					// get data for second operand
		//					cvNDataAtomPtr sp2Data = shared_dynamic_cast<cvNDataAtom>( DataRef1.m_StorageType == cvStatic ?
		//						m_StaticData[DataRef1.m_DataIndex] :
		//					m_DynamicData.GetNData(DataRef1.m_DataIndex ) );

		//					// add them
		//					double dSum = sp1Data->GetData() + sp2Data->GetData();
		//					if ( DataRef.m_StorageType == cvDynamic) 
		//						m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
		//					if ( DataRef1.m_StorageType == cvDynamic) 
		//						m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

		//					// create temporary data and store it in stack
		//					DataRef.m_DataIndex = m_DynamicData.Add( cvtNumeric, &dSum );
		//					DataRef.m_StorageType = cvDynamic;
		//					DataRef.m_Type = cvtNumeric;
		//					m_ExecStack.push( DataRef );

		//					// set flags
		//					if ( dSum == 0 ) 
		//						m_bGreater = m_bLess = false;
		//					else 
		//						m_bGreater = !(m_bLess = dSum < 0 ? true : false );
		//				}
		//				else{
		//					// add two strings

		//					// get data for first operand
		//					cvSDataAtomPtr sp1Data = shared_dynamic_cast<cvSDataAtom>( DataRef.m_StorageType == cvStatic ?
		//						m_StaticData[DataRef.m_DataIndex] :
		//					m_DynamicData.GetSData(DataRef.m_DataIndex ) );

		//					// get data for second operand
		//					cvSDataAtomPtr sp2Data = shared_dynamic_cast<cvSDataAtom>( DataRef1.m_StorageType == cvStatic ?
		//						m_StaticData[DataRef1.m_DataIndex] :
		//					m_DynamicData.GetSData(DataRef1.m_DataIndex ) );

		//					// add them
		//					string strSum = sp1Data->GetData() + sp2Data->GetData();
		//					if ( DataRef.m_StorageType == cvDynamic) 
		//						m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
		//					if ( DataRef1.m_StorageType == cvDynamic) 
		//						m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

		//					// create temporary data and store it in stack
		//					DataRef.m_DataIndex = m_DynamicData.Add( cvtNumeric, &strSum );
		//					DataRef.m_StorageType = cvDynamic;
		//					DataRef.m_Type = cvtNumeric;
		//					m_ExecStack.push( DataRef );

		//					// set flags
		//					if ( strSum.length() != 0 ) 
		//						m_bGreater = !(m_bLess = false);
		//					else 
		//						m_bGreater = m_bLess = false;
		//				}
		//			}
		//			else
		//				RaiseError(E_FAIL, _T("Runtime error: Bad types of data for addition") );
		//		}
		//		break;
		//	case	SUB:
		//		{
		//			if ( m_ExecStack.size() < 2 ) 
		//				RaiseError(E_FAIL, _T("Runtime error: no enough data") );
		//			DataRef = m_ExecStack.top();
		//			m_ExecStack.pop();
		//			DataRef1 = m_ExecStack.top();
		//			m_ExecStack.pop();
		//			if ( DataRef1.m_Type == DataRef.m_Type && DataRef.m_Type == cvtNumeric ) {
		//				// subtract two numerics

		//				// get data for first operand
		//				cvNDataAtomPtr sp1Data = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
		//					m_StaticData[DataRef.m_DataIndex] :
		//				m_DynamicData.GetNData(DataRef.m_DataIndex ) );

		//				// get data for second operand
		//				cvNDataAtomPtr sp2Data = shared_dynamic_cast<cvNDataAtom>( DataRef1.m_StorageType == cvStatic ?
		//					m_StaticData[DataRef1.m_DataIndex] :
		//				m_DynamicData.GetNData(DataRef1.m_DataIndex ) );

		//				// add them
		//				double dSum = sp2Data->GetData() - sp1Data->GetData();
		//				if ( DataRef.m_StorageType == cvDynamic) 
		//					m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
		//				if ( DataRef1.m_StorageType == cvDynamic) 
		//					m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

		//				// create temporary data and store it in stack
		//				DataRef.m_DataIndex = m_DynamicData.Add( cvtNumeric, &dSum );
		//				DataRef.m_StorageType = cvDynamic;
		//				DataRef.m_Type = cvtNumeric;
		//				m_ExecStack.push( DataRef );

		//				// set flags
		//				if ( dSum == 0 ) 
		//					m_bGreater = m_bLess = false;
		//				else 
		//					m_bGreater = !(m_bLess = dSum < 0 ? true : false );
		//			}
		//			else
		//				RaiseError(E_FAIL, _T("Runtime error: Bad types of data for subtraction") );
		//		}
		//		break;
		//	case	DIV:
		//		{
		//			if ( m_ExecStack.size() < 2 ) 
		//				RaiseError(E_FAIL, _T("Runtime error: no enough data") );
		//			DataRef = m_ExecStack.top();
		//			m_ExecStack.pop();
		//			DataRef1 = m_ExecStack.top();
		//			m_ExecStack.pop();
		//			if ( DataRef1.m_Type == DataRef.m_Type && DataRef.m_Type == cvtNumeric ) {
		//				// divide two numerics

		//				// get data for first operand
		//				cvNDataAtomPtr sp1Data = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
		//					m_StaticData[DataRef.m_DataIndex] :
		//				m_DynamicData.GetNData(DataRef.m_DataIndex ) );

		//				// get data for second operand
		//				cvNDataAtomPtr sp2Data = shared_dynamic_cast<cvNDataAtom>( DataRef1.m_StorageType == cvStatic ?
		//					m_StaticData[DataRef1.m_DataIndex] :
		//				m_DynamicData.GetNData(DataRef1.m_DataIndex ) );

		//				// add them
		//				double dSum = (sp1Data->GetData() != 0.0) ? ( sp2Data->GetData() /  sp1Data->GetData() ) : 0.0;
		//				if ( DataRef.m_StorageType == cvDynamic) 
		//					m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
		//				if ( DataRef1.m_StorageType == cvDynamic) 
		//					m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

		//				// create temporary data and store it in stack
		//				DataRef.m_DataIndex = m_DynamicData.Add( cvtNumeric, &dSum );
		//				DataRef.m_StorageType = cvDynamic;
		//				DataRef.m_Type = cvtNumeric;
		//				m_ExecStack.push( DataRef );

		//				// set flags
		//				if ( dSum == 0 ) 
		//					m_bGreater = m_bLess = false;
		//				else 
		//					m_bGreater = !(m_bLess = dSum < 0 ? true : false );
		//			}
		//			else
		//				RaiseError(E_FAIL, _T("Runtime error: Bad types of data for division") );
		//		}
		//		break;
		//	case	MUL:
		//		{
		//			if ( m_ExecStack.size() < 2 ) 
		//				RaiseError(E_FAIL, _T("Runtime error: no enough data") );
		//			DataRef = m_ExecStack.top();
		//			m_ExecStack.pop();
		//			DataRef1 = m_ExecStack.top();
		//			m_ExecStack.pop();
		//			if ( DataRef1.m_Type == DataRef.m_Type && DataRef.m_Type == cvtNumeric ) {
		//				// multiply two numerics

		//				// get data for first operand
		//				cvNDataAtomPtr sp1Data = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
		//					m_StaticData[DataRef.m_DataIndex] :
		//				m_DynamicData.GetNData(DataRef.m_DataIndex ) );

		//				// get data for second operand
		//				cvNDataAtomPtr sp2Data = shared_dynamic_cast<cvNDataAtom>( DataRef1.m_StorageType == cvStatic ?
		//					m_StaticData[DataRef1.m_DataIndex] :
		//				m_DynamicData.GetNData(DataRef1.m_DataIndex ) );

		//				// add them
		//				double dSum = sp1Data->GetData() * sp2Data->GetData();
		//				if ( DataRef.m_StorageType == cvDynamic) 
		//					m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );
		//				if ( DataRef1.m_StorageType == cvDynamic) 
		//					m_DynamicData.Remove( DataRef1.m_Type, DataRef1.m_DataIndex );

		//				// create temporary data and store it in stack
		//				DataRef.m_DataIndex = m_DynamicData.Add( cvtNumeric, &dSum );
		//				DataRef.m_StorageType = cvDynamic;
		//				DataRef.m_Type = cvtNumeric;
		//				m_ExecStack.push( DataRef );

		//				// set flags
		//				if ( dSum == 0 ) 
		//					m_bGreater = m_bLess = false;
		//				else 
		//					m_bGreater = !(m_bLess = dSum < 0 ? true : false );
		//			}
		//			else
		//				RaiseError(E_FAIL, _T("Runtime error: Bad types of data for multiplication") );
		//		}
		//		break;
		//	case	iDIV:
		//		break;
		//	case	MOD:
		//		break;
		//	case	JMP:
		//		cmd += CurCmd.m_Operand;
		//		cmd--;
		//		break;
		//	case	jGE:
		//		if ( !m_bLess) {
		//			cmd += CurCmd.m_Operand;				
		//			cmd--;
		//		}
		//		break;
		//	case	jLE:
		//		if ( !m_bGreater) {
		//			cmd += CurCmd.m_Operand;				
		//			cmd--;
		//		}
		//		break;
		//	case	jG:
		//		if ( m_bGreater) {
		//			cmd += CurCmd.m_Operand;				
		//			cmd--;
		//		}
		//		break;
		//	case	jNE:
		//		if ( m_bGreater || m_bLess	) {
		//			cmd += CurCmd.m_Operand;				
		//			cmd--;
		//		}
		//		break;
		//	case	jL:
		//		if ( m_bLess) {
		//			cmd += CurCmd.m_Operand;				
		//			cmd--;
		//		}
		//		break;
		//	case	jE:
		//		if ( !m_bLess && !m_bGreater ) {
		//			cmd += CurCmd.m_Operand;				
		//			cmd--;
		//		}
		//		break;
		//	case	NEG:
		//		{
		//			if ( m_ExecStack.size() < 1 ) 
		//				RaiseError(E_FAIL, _T("Runtime error: no enough data") );
		//			DataRef = m_ExecStack.top();
		//			if ( DataRef.m_Type == cvtNumeric ) {

		//				// get data for operand
		//				cvNDataAtomPtr sp1Data = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
		//					m_StaticData[DataRef.m_DataIndex] :
		//				m_DynamicData.GetNData(DataRef.m_DataIndex ) );
		//				double Value = sp1Data->GetData() * -1;
		//				sp1Data->SetData(Value);
		//			}
		//			else
		//				RaiseError(E_FAIL, _T("Runtime error: Bad type for negation") );
		//		}
		//		break;
		//	case	stoV:
		//	default:
		//		{
		//			// is this a function call?
		//			if ( static_cast<unsigned int>( pPool->m_Program[cmd].m_OpCode )  & static_cast<unsigned int>( callF )  ) {
		//				// yes

		//				cvExecutionStack tmpstack;
		//				size_t ParamsCount = static_cast<unsigned int>( pPool->m_Program[cmd].m_OpCode ) & 0x0000FFFF;
		//				if ( m_ExecStack.size() < ParamsCount )
		//					RaiseError( E_FAIL, _T("Runtime error: No enough parameters for function call") );

		//				// put parameters into safearray
		//				SAFEARRAYBOUND sab = { static_cast<ULONG>(ParamsCount), 0 };
		//				LPSAFEARRAY psa = ::SafeArrayCreateEx( VT_VARIANT, 1, &sab, NULL);

		//				_variant_t	vtParam;
		//				//VARIANT svtParam;
		//				//VariantInit( &svtParam );
		//				LONG uParamIndex = 0;
		//				for ( size_t Param = 0; Param < ParamsCount; Param++) {
		//					DataRef = m_ExecStack.top();
		//					switch( DataRef.m_Type ) {
		//						case cvtNumeric:
		//							{
		//								cvNDataAtomPtr spNData = shared_dynamic_cast<cvNDataAtom>( DataRef.m_StorageType == cvStatic ?
		//									m_StaticData[DataRef.m_DataIndex] :
		//								m_DynamicData.GetNData(DataRef.m_DataIndex ) );
		//								vtParam = spNData->GetData();
		//							}
		//							break;
		//						case cvtString:
		//							{
		//								cvSDataAtomPtr spSData = shared_dynamic_cast<cvSDataAtom>( DataRef.m_StorageType == cvStatic ?
		//									m_StaticData[DataRef.m_DataIndex] :
		//								m_DynamicData.GetSData(DataRef.m_DataIndex ) );
		//								vtParam = spSData->GetData().c_str();
		//							}
		//							break;
		//						case cvtBoolean:
		//							{
		//								cvBDataAtomPtr spBData = shared_dynamic_cast<cvBDataAtom>( DataRef.m_StorageType == cvStatic ?
		//									m_StaticData[DataRef.m_DataIndex] :
		//								m_DynamicData.GetBData(DataRef.m_DataIndex ) );
		//								vtParam = spBData->GetData();
		//							}
		//							break;
		//						default:	;
		//					}
		//					//svtParam = vtParam;
		//					m_ExecStack.pop();
  //                   if ( DataRef.m_StorageType == cvDynamic ) 
		//						m_DynamicData.Remove( DataRef.m_Type, DataRef.m_DataIndex );

		//					SafeArrayPutElement( psa, &uParamIndex, &vtParam );
		//					uParamIndex++;
		//				}


		//				// call function from corresponding level
		//				unsigned int levelID = CurCmd.m_Operand >> 16;
		//				unsigned int ID = CurCmd.m_Operand & 0xFFFF;

		//				cvDataProvidersMap::iterator itr = pPool->m_mapDataProviders.find(levelID);
		//				if ( itr == pPool->m_mapDataProviders.end() ) 
		//					RaiseError( E_POINTER, _T("Runtime error: No data provider supplied for pool"));
		//				IcvDataProviderPtr pDP(itr->second);

		//				if(FAILED( pDP->CallFunction( ID, &psa, &vtParam) ) )
		//					RaiseError(E_FAIL, _T("Runtime error: Unable to retrieve data from data provider") );
		//				
		//				::SafeArrayDestroy(psa);

		//				//vtParam = svtParam;
		//				if (vtParam.vt == VT_R8) {
		//					double dVal = vtParam;
		//					DataRef.m_DataIndex = m_DynamicData.Add( cvtNumeric, &dVal );
		//					DataRef.m_StorageType = cvDynamic;
		//					DataRef.m_Type = cvtNumeric;
		//					m_ExecStack.push( DataRef );
		//				}
		//				else if ( vtParam.vt == VT_BOOL) {
		//					bool bVal = vtParam;
		//					DataRef.m_DataIndex = m_DynamicData.Add( cvtBoolean, &bVal );
		//					DataRef.m_StorageType = cvDynamic;
		//					DataRef.m_Type = cvtBoolean;
		//					m_ExecStack.push( DataRef );
		//				}
		//				else if ( vtParam.vt == VT_BSTR) {
		//					_bstr_t	bstrVal = vtParam;
		//					string strVal = (LPCSTR) bstrVal;
		//					DataRef.m_DataIndex = m_DynamicData.Add( cvtString, &strVal );
		//					DataRef.m_StorageType = cvDynamic;
		//					DataRef.m_Type = cvtString;
		//					m_ExecStack.push( DataRef );
		//				}
		//				else 
		//					RaiseError(E_FAIL, _T("Runtime error: Data of unsupported type returned from data provider") );
		//			}
		//			else
		//				RaiseError( E_FAIL, _T("Runtime error: Unknown opcode in program") );
		//		}
		//	}
		//}
	}
	catch ( _com_error& e) {
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IcvRTContext, e.Error());
	}
	return S_OK;
}
