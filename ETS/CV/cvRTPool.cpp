// cvRTPool.cpp : Implementation of CcvRTPool

#include "stdafx.h"
#include "cvRTContext.h"
#include "cvRTPool.h"
#include ".\cvrtpool.h"

#include "cvRT.h"

#include <vector>

// CcvRTPool
using namespace std;
using namespace boost;
using namespace CV;
using namespace cvRuntime;

bool CcvRTPool::RegisterDataProvider(LONG lLevelID, IcvDataProvider* pIcvDataProvider)
{
	IcvDataProviderPtr spDataProvider( pIcvDataProvider);
	if ( m_mapDataProviders.find( lLevelID) != m_mapDataProviders.end() ) 
		return false;
	m_mapDataProviders[ lLevelID ] = spDataProvider;
	return true;
}

void CcvRTPool::InitFromMetaPool(cvMetaPoolPtr spMetaPool, cvStaticStorage& StaticStorage ){

	// save meta pool for future use
	m_spMetaPool = spMetaPool;

	// for every meta CV in meta pool create corresponding runtime CV
	// note: meta CVs in meta pool are sorted regarding they calculation
	// order

	const vector<cvMetaCVPtr>& v = spMetaPool->GetMetaCVVector();
	size_t commandsToExecuteCounter = 0;
	for ( size_t i = 0; i < v.size(); i++) {
		cvMetaCVPtr pMCV = v[i];				

		// allocate variable in static storage
		unsigned int VarIndex = 0;
		switch(pMCV->m_type ) {
			case cvtString:
				{
					cvSDataAtomPtr ptr = cvSDataAtomPtr( new cvSDataAtom );
					ptr->SetData( string("") );
					VarIndex = StaticStorage.Add( shared_static_cast<cvDataAtomBase >( ptr ) );
				}
				break;
			case cvtBoolean:
				{
					cvBDataAtomPtr ptr = cvBDataAtomPtr( new cvBDataAtom );
					bool bFalse = false;
					ptr->SetData( bFalse );
					VarIndex = StaticStorage.Add( shared_static_cast<cvDataAtomBase >( ptr ) );
				}
				break;
			case cvtNumeric:  
				{
					cvNDataAtomPtr ptr = cvNDataAtomPtr( new cvNDataAtom );
					double dZero = 0.0;
					ptr->SetData( dZero );
					VarIndex = StaticStorage.Add( shared_static_cast<cvDataAtomBase >( ptr ) );
				}
				break;
			default:
				EgLib::CComErrorWrapper::ThrowError( E_FAIL, _T("Bad type of custom variable") );
		}

		// create RTCV

		cvRTCVPtr spCV = cvRTCVPtr( new CcvRTCV );
		spCV->m_DataIndex = VarIndex;
		spCV->m_spMetaCV = pMCV;
		//m_mapCV[ pMCV->m_ID ] = spCV;
		spCV->m_DBId = pMCV->m_ID;
		m_vecCV.push_back( spCV );
		if ( pMCV->m_code.empty() && pMCV->m_weight == 0) 
			continue;	// this is an aggregation, not a custom variable 

		
		// LC: vector<cvVMCommand> VariableCode;

		// get code of variable
		// LC: for ( size_t cmnd = 0; cmnd < pMCV->m_code.size(); cmnd++) 
		// LC: 	VariableCode.push_back(pMCV->m_code[ cmnd ]);
		
		// resolve references from code to this custom variable
		// LC: for ( size_t ref = 0; ref < pMCV->m_vecReferences.size(); ref++ )
		// LC: 	VariableCode[ pMCV->m_vecReferences[ref] ].m_Operand = VarIndex;
		

		//////////////////////////////////////////////////////////////////////////
		
		for ( size_t ref = 0; ref < pMCV->m_vecReferences.size(); ref++ )
			m_Program.push_back( 
				cvVMCommand(
								(cvVMOpCode)( pMCV->m_vecReferences[ref] + commandsToExecuteCounter ),
								VarIndex
							) 
			);
		commandsToExecuteCounter += pMCV->m_code.size();
		//////////////////////////////////////////////////////////////////////////

		// add code to pool's program code
		
		// LC: for ( size_t s = 0; s < VariableCode.size(); s++) 
		// LC: 	m_Program.push_back( VariableCode[s] );
	}
}


void CcvRTPool::BindAggregations( LONG lAggregationsPool )
{
	//cvRTCVMap::iterator b = m_mapCV.begin();
	//cvRTCVMap::iterator e = m_mapCV.end();
	vector<cvRTCVPtr>::iterator b = m_vecCV.begin();
	vector<cvRTCVPtr>::iterator e = m_vecCV.end();


	assert( m_pContext );
	CcvRTPool *pAggPool = m_pContext->GetPool( lAggregationsPool );
	if ( !pAggPool) 
		return;
		//assert(0);

	for (; b!=e; b++) {
		cvRTCVPtr pCV = *b;
		if (pCV->m_spMetaCV) {
			for ( size_t s = 0; s < pCV->m_spMetaCV->m_vecAggregations.size(); s++) {
				// check does this variable has aggregation on level for which lAggregationsPool created?
				if ( pCV->m_spMetaCV->m_vecAggregations[s] == pAggPool->m_levelID ) {
					// yes it has, so get aggregations pool
					
					if ( pAggPool ) {
						cvRTCVPtr pVar = pAggPool->GetCV( pCV->m_spMetaCV->m_ID );
						pCV->m_vecAggregations.push_back( pVar->m_DataIndex );
					}
					else
						assert(0);
					break;
				}
			}
		}
	}
}
