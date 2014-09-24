#include <stdafx.h>
#include <map>
#include <boost/shared_ptr.hpp>
#include "cvRT.h"
#include <assert.h>
#include <algorithm>

using namespace std;
using namespace boost;
using namespace CV;
using namespace cvRuntime;

cvMetaPool::cvMetaPool( cvID levelID, cvSysTypedObjByNameMap& mapSysVars, cvSysTypedObjByNameMap& mapSysFuncs, 
							  cvSysTypedObjByNameMap& mapCV, cvMetaPoolMap& metaPoolMap, cvStaticStorage& sd  ){
	m_levelID = levelID;
	cvSysTypedObjByNameMap::iterator b = mapSysVars.begin();
	cvSysTypedObjByNameMap::iterator e = mapSysVars.end();
	for (; b != e; b++) 
		m_mapSysVars[ b->first ] = b->second;
	
	b = mapSysFuncs.begin();
	e = mapSysFuncs.end();
	for (; b != e; b++) 
		m_mapSysFuncs[ b->first ] = b->second;

	// its important to add meta CV to m_vecMCV and hold'em there ordered by it weight
	vector<cvMetaCVPtr> vecTmpMCV;
	b = mapCV.begin();
	e = mapCV.end();
	for (; b != e; b++) 
		vecTmpMCV.push_back( shared_dynamic_cast<cvMetaCV>(b->second) );
	sort( vecTmpMCV.begin(), vecTmpMCV.end(), cvMetaCV::greater );	

	for ( size_t MCV = 0; MCV < vecTmpMCV.size(); MCV++ ) 
		AddMetaCV( vecTmpMCV[MCV], metaPoolMap, sd );
	
	sort( m_vecMCV.begin(), m_vecMCV.end(), cvMetaCV::greater );	
}

void cvMetaPool::AddMetaCV(cvMetaCVPtr MetaCV, cvMetaPoolMap& metaPoolMap, cvStaticStorage& sd ){
	m_vecMCV.push_back( MetaCV );

	if ( MetaCV->m_code.size() == 0) 
		return;	// this is aggregation from lower level, not a variable
	
	// resolve references to constants, system functions, system and local variables in code
	
	// constants
	cvTRefPtrByNameMap::iterator b = MetaCV->m_mapConsts.begin();
	cvTRefPtrByNameMap::iterator e = MetaCV->m_mapConsts.end();
	while ( b != e ) {
		pair<unsigned int, cvRefPtr> pr = b->second;
		cvTReferable< void, cvtNone> *pConst = static_cast<cvTReferable< void, cvtNone> *>(pr.second.get());
		// reserve storage for constant
		unsigned int ConstIndex = 0;
		switch(pConst->getType() ) {
			case cvtString:
				{
					cvSDataAtomPtr ptr = cvSDataAtomPtr( new cvSDataAtom );
					ptr->SetData( pConst->m_name );
					ConstIndex = sd.Add( shared_static_cast<cvDataAtomBase >( ptr ) );
				}
				break;
			case cvtBoolean:
				{
					cvBDataAtomPtr ptr = cvBDataAtomPtr( new cvBDataAtom );
					bool bValue = pConst->m_name == "true" ? true : false;
					ptr->SetData( bValue  );
					ConstIndex = sd.Add( shared_static_cast<cvDataAtomBase >( ptr ) );
				}
				break;
			case cvtNumeric:  
				{
					cvNDataAtomPtr ptr = cvNDataAtomPtr( new cvNDataAtom );
					double dValue = atof( pConst->m_name.c_str() );
					ptr->SetData( dValue );
					ConstIndex = sd.Add( shared_static_cast<cvDataAtomBase >( ptr ) );
				}
				break;
			default:
				assert(0);
		}

		// resolve reference
		
		for ( unsigned int s = 0; s < pConst->GetRefsCount(); s++ ) {
			MetaCV->m_code[ pConst->GetRef(s) ].m_Operand = ConstIndex;
		}  
		b++;
	}
	
	// local, system and custom variables
	b = MetaCV->m_mapVars.begin();
	e = MetaCV->m_mapVars.end();
	while ( b != e ) {
		pair<unsigned int, cvRefPtr> pr = b->second;
		cvTReferable< void, cvtNone> *pVar = static_cast<cvTReferable< void, cvtNone> *>(pr.second.get());
		if ( !pVar->IsSystem() ) {
			
			// this is local referenced variable
			// reserve storage for it
			unsigned int VarIndex = 0;
			switch(pVar->getType() ) {
				case cvtString:
					{
						cvSDataAtomPtr ptr = cvSDataAtomPtr( new cvSDataAtom );
						VarIndex = sd.Add( shared_static_cast<cvDataAtomBase >( ptr ) );
					}
				break;
				case cvtBoolean:
					{
						cvBDataAtomPtr ptr = cvBDataAtomPtr( new cvBDataAtom );
						VarIndex = sd.Add( shared_static_cast<cvDataAtomBase >( ptr ) );
					}
					break;
				case cvtNumeric:  
					{
						cvNDataAtomPtr ptr = cvNDataAtomPtr( new cvNDataAtom );
						VarIndex = sd.Add( shared_static_cast<cvDataAtomBase >( ptr ) );
					}
					break;
				default:
					assert(0);
			}

			// resolve reference
			for ( unsigned int s = 0; s < pVar->GetRefsCount(); s++ ) {
				MetaCV->m_code[ pVar->GetRef(s) ].m_Operand = VarIndex;
			}
		}
		else{
			// this is system or custom variable, get it level info
			unsigned int SysVarLevel = pVar->GetLevel();

			
			cvMetaPool *pMetaPool = NULL;
			
			// if it on current level, try to find it here
			if ( SysVarLevel == m_levelID) {
				pMetaPool = this;
			}
			else{
				cvMetaPoolMap::iterator l = metaPoolMap.find( SysVarLevel );
				if ( l == metaPoolMap.end() ) {
					
					bool bError = true;
					vector<cvMetaCVPtr>::iterator b = m_vecMCV.begin();
					vector<cvMetaCVPtr>::iterator e = m_vecMCV.end();
					for(vector<cvMetaCVPtr>::const_iterator it = b; it != e; ++it)
					{
						if((*it)->m_strName == pVar->m_name && (*it)->m_code.size() == 0)
						{
							bError = false;
							pMetaPool = this;
							break;
						}
					}

					if(bError)
						EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Bad Variable definition") );
					//assert(0);		// error - no referenced level loaded yet
				}
				else
					pMetaPool = l->second.get();
			}

			// try to find variable info in meta pool corresponding to level of variable
			cvSysTypedObjByNameMap::iterator itr = pMetaPool->m_mapSysVars.find( pVar->m_name ) ;
			if ( itr != pMetaPool->m_mapSysVars.end() ) {
				// this is system var
				cvSysTypedObjPtr pSV = itr->second;	// get it
				for ( unsigned int s1 = 0; s1 < pVar->GetRefsCount(); s1++)	 // resolve references
					MetaCV->m_code[ pVar->GetRef(s1)].m_Operand = ( (pMetaPool->m_levelID) << 16 ) | ( pSV->m_ID ); // level | id
			}
			else{

				// search referenced MetaPool for id of custom var
				bool bError = true;
				for ( size_t s = 0; s < pMetaPool->m_vecMCV.size(); s++ ) {
					if ( pMetaPool->m_vecMCV[s]->m_strName == pVar->m_name) {
						// found it - resolve references
						// operand of command that referencing to this variable will become combination
						// of id of level where this variable was defined and id of this variable
						// level id 0xFFFF00000 marks custom variables
						for ( unsigned int s1 = 0; s1 < pVar->GetRefsCount(); s1++) 
							MetaCV->m_code[ pVar->GetRef(s1)].m_Operand = /* ( (pMetaPool->m_levelID) << 16 )*/0xFFFF0000  | ( pMetaPool->m_vecMCV[s]->m_ID ) ; // level | id
						bError = false;
						break;
					}
				}
				if ( bError) {
					assert(0); // referenced custom variable not found in level
				}
			}
		}
		b++;
	}

	// Do same for functions
	b = MetaCV->m_mapFuncs.begin();
	e = MetaCV->m_mapFuncs.end();
	while ( b != e ) {
		// get description of referenced function
		pair<unsigned int, cvRefPtr> pr = b->second;
		cvTReferable< void, cvtNone> *pFunc = static_cast<cvTReferable< void, cvtNone> *>(pr.second.get());

		// get level info of referenced function
		unsigned int SysFuncLevel = pFunc->GetLevel();

		
		cvSysTypedObjByNameMap::iterator itr;
		cvMetaPool *pMetaPool = NULL;

		// if this function is from current level, try to find it in this meta pool
		if ( SysFuncLevel == m_levelID) {
			itr = m_mapSysFuncs.find( pFunc->m_name ) ;
			pMetaPool = this;
		}
		else{

			// search for function object in meta pool that corresponds to function's level
			cvMetaPoolMap::iterator l = metaPoolMap.find( SysFuncLevel );
			if ( l == metaPoolMap.end() ) 
				assert(0);		// error - downward reference prohibited. referenced level has not been loaded yet

			// get system function object
			pMetaPool = l->second.get();
			itr = pMetaPool->m_mapSysFuncs.find( pFunc->m_name ) ;
		}
		

		
		if ( itr != pMetaPool->m_mapSysFuncs.end() ) {
			cvSysTypedObjPtr pSF = itr->second;
			// resolve references
			for ( unsigned int s = 0; s < pFunc->GetRefsCount(); s++ ) {
				MetaCV->m_code[ pFunc->GetRef(s)].m_Operand = ( (pMetaPool->m_levelID << 16 ) | pSF->m_ID );
			}
		}
		else
			assert(0);	// error - referenced function not found on specified level


		b++;
	}  
}
void cvMetaPool::GetRequiredDataProviderLevels( vector<cvID>& levels){
	levels.clear();
	map<unsigned int, bool> mapLevels;
	vector<unsigned int> vecCVLevels;

	for ( size_t s = 0; s < m_vecMCV.size(); s++) {		// looking through all variables in pool
		m_vecMCV[s]->GetRequiredDataProviderLevels( vecCVLevels );	
		for ( size_t sCV = 0; sCV < vecCVLevels.size(); sCV++) // get levels of each variable
			mapLevels[ vecCVLevels[ sCV] ] = true;
		vecCVLevels.clear();
	}

	map<unsigned int, bool>::iterator b = mapLevels.begin();
	map<unsigned int, bool>::iterator e = mapLevels.end();
	while ( b != e) {
		levels.push_back( b->first);
		b++;
	}
}
void cvMetaPool::GetRequiredAggregationLevels( vector<cvID>& levels ){
}
/*
STDMETHODIMP CcvDataProviderBase::Check( SAFEARRAY *arrSysVars, SAFEARRAY*arrSysFuncs){
	long lUBound = 0;
	long lLBound = 0;
	SAFEARRAY *pSA = arrSysVars;

	SafeArrayGetUBound( arrSysVars, 1, &lUBound );
	SafeArrayGetLBound( arrSysVars, 1, &lLBound );

	cvInfo *pInfo;
	SafeArrayAccessData( arrSysVars, (void**)pInfo);
	for ( long i = lLBound; i < lLBound;i++) {
		// add every system variable to vector
		string strName = (LPCSTR)_bstr_t(pInfo->Name);
		size_t s =FindDataByName(strName);
		if ( s != CV_DATANOTSUPPORTED ) {
			m_mapID[pInfo->ID] = m_vecLocalID[s].m_DataId;
			m_vecLocalID.erase(s);
		}
		pInfo++;
	}
}*/