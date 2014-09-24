// SecurityProfile.cpp : Implementation of CSecurityProfile
#include "stdafx.h"
#include "SecurityProfile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSecurityProfile::~CSecurityProfile()
{
	m_MultipleRequests.clear();
}

long CSecurityProfile::Attach()
{
	return CDatabase<DBA_SECURITY_PROFILE>::Attach();
}

long CSecurityProfile::Detach()
{
	return CDatabase<DBA_SECURITY_PROFILE>::Detach();
}

void _ConvertOptionNameInKey(string &sspaced)
{
	string str(sspaced);
	string sPre  = str.substr(0, str.length() - 2);
	string sPost = str.substr(str.length() - 2, str.length());

	sspaced.erase(0, sspaced.length());

	sspaced += sPre;
	sspaced += ' ';
	sspaced += sPost;
}

void CSecurityProfile::_SetDbKey(const _QuoteUpdateParams& Params, DBA_KEY *dbaKey)
{
	_bstr_t bsSymbol(Params->Symbol);
	string sSymbol(bsSymbol);

	switch(Params->Type) 
	{
	case enOPT:
		_ConvertOptionNameInKey(sSymbol);
		bsSymbol = sSymbol.c_str();
		break;
	}

	StringToKey(bsSymbol, dbaKey);

	switch(Params->Type) 
	{
	case enSTK:
		dbaKey->type = IT_EQUITY;
		break;
	case enOPT:
		dbaKey->type = IT_EQUITY_OPTION;
		break;
	}

	//CreateSpacedUpper((char*)dbaKey->countryCode, "US", 2);
	//CreateSpacedUpper((char*)dbaKey.currencyCode, "USD", 3);
	//CreateSpaced((char *)dbaKey.symbol, Params->Symbol, MAX_TICKER);
}

long CSecurityProfile::GetMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey)
{
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("SecurityProfile GetMultiple Enter %s"), _T(pKey->dbaKey.symbol));
	long lRet = 0;
	try
	{
		INT nRecLen = sizeof(MULTIGE_FILTER);
		INT nKeyLen = sizeof(DBA_MAXKEY);
		lRet =  dba(&m_blk, DBA_MULTI_GET_EQUAL,	pKey, &nKeyLen,	pFilrer, &nRecLen, 0);
		if(lRet)
		{
			//return lRet;
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("SecurityProfile GetMultiple Return %d"), lRet);
		}
		else
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("SecurityProfile GetMultiple Infinite Wait %s"), _T(pKey->dbaKey.symbol));
			dba(&m_blk,	DBA_WAIT_FOR_STREAM_TERMINATE,	NULL, 0, NULL, 0, DBAX_INFINITE_WAIT);	
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("SecurityProfile GetMultiple Exit %s"), _T(pKey->dbaKey.symbol));
	return lRet;
}

long CSecurityProfile::RetrieveSecuritiesInfo(const CRequestBasePtr& pParams)
{
	long nResult = 0;
	const CQuoteRequestPtr Params = boost::shared_dynamic_cast<CQuoteRequest>(pParams);
	if(!Params)
		return nResult;

	switch(Params->m_recParams->Type)
	{
	case enMStart:
		{
			//m_bCancelLastQuote = false;
			EgLib::CAutoLock lock(m_csMultipleRequests);
			m_MultipleRequests.clear();
			//m_bMultipleRequests = true;

		} break;
	case enMStop:
		{
			MULTIGE_FILTER		multiGEFilter;
			//m_bCancelLastQuote = false;
			ZeroMemory(&multiGEFilter, sizeof(multiGEFilter));
			multiGEFilter.nRecLen				= sizeof(DBA_RECORD);
			multiGEFilter.count				= 0;
			multiGEFilter.flags				= 0;
			multiGEFilter.pUser             = this;
			//multiGEFilter.nUserInfo         = bFireError;  
			multiGEFilter.lpfnDbMultiGEProc	= _MultipleRequests;
			if(!m_MultipleRequests.empty())
			{
				//m_pLastQuoteKey = boost::shared_array<DBA_MAXKEY>(new DBA_MAXKEY[m_MultipleRequests.size()]);
				::ZeroMemory(m_pLastQuoteKey, sizeof(DBA_MAXKEY)*MAX_REQUESTS);
				multiGEFilter.count=0;
				for(CMuttipleRequests::iterator iter = m_MultipleRequests.begin(); iter != m_MultipleRequests.end(); ++iter)
				{
					char buffer[2048];
					ZeroMemory(buffer, sizeof(buffer));
					strncpy(buffer, iter->first.c_str(), sizeof(buffer)-1);
					//::StringToKey(buffer, &m_pLastQuoteKey[multiGEFilter.count].dbaKey);
					_SetDbKey(iter->second->m_recParams, (DBA_KEY*) &m_pLastQuoteKey[multiGEFilter.count]);

					multiGEFilter.count++;
					if(multiGEFilter.count > MAX_REQUESTS /*&& !m_bCancelLastQuote*/)
					{
						GetMultiple(&multiGEFilter, m_pLastQuoteKey);
						::ZeroMemory(m_pLastQuoteKey, sizeof(DBA_MAXKEY)*MAX_REQUESTS);
						multiGEFilter.count = 0;
					}

					if(m_MultipleRequests.empty())
						break;
				}
				if(multiGEFilter.count /*&& !m_bCancelLastQuote*/)
					GetMultiple(&multiGEFilter, m_pLastQuoteKey);
			}

			//m_bMultipleRequests = false;
			EgLib::CAutoLock lock(m_csMultipleRequests);
			m_MultipleRequests.clear();
		} break;
	default:
		{
			//m_bCancelLastQuote = false;
			DBA_MAXKEY maxKey;
			CHAR szBuffer[256];
			::ZeroMemory(&maxKey, sizeof(maxKey));
			::ZeroMemory(szBuffer, sizeof(szBuffer));

			_SetDbKey(Params->m_recParams, (DBA_KEY*) &maxKey);

			if(CUtilities::KeyToString(&maxKey.dbaKey, szBuffer, sizeof(szBuffer)))
			{
				EgLib::CAutoLock lock(m_csMultipleRequests);
				m_MultipleRequests[szBuffer] = Params;
			}
		}
		break;
	}

	return nResult;
}

long CSecurityProfile::_MultipleRequests(VOID* pblk,	// pointer to param block
							   ULONG			ulFunc,	// function
							   VOID*			pKey,	// pointer to key
							   INT				nKeyLen,// length of key struct
							   DBA_RECORD*		pRec,	// pointer to return record
							   INT				nRecLen,// length of return record
							   ULONG			ulKey,	// key number
							   DWORD			dwStatus,//Status of the DBA callback
							   VOID*			pVoid)//ptr to Stream Filter struct
{
	ULONG 			ulFunction  = ulFunc & DBAX_FUNCTION_MASK;
	MULTIGE_FILTER* pFilter = (MULTIGE_FILTER*)pVoid;
	//--------------------------------------
	// No more data to process, so all done 
	//--------------------------------------
	if (ulFunction == DBA_TERMINATE_STREAM_FUNCTION)
		return TRUE;

	CSecurityProfile* pSecurityProfile = reinterpret_cast<CSecurityProfile*>(pFilter->pUser);
	if(!pSecurityProfile /*|| pPrice->m_bCancelLastQuote*/)
		return TRUE;

	//bool bNotify = pFilter->nUserInfo!=0;

	char buffer[2048];
	ZeroMemory(buffer, sizeof(buffer));
	CUtilities::KeyToString((DBA_KEY *)pKey, buffer, 2047);

	EgLib::CAutoLock lock(pSecurityProfile->m_csMultipleRequests);
	CMuttipleRequests::iterator it = pSecurityProfile->m_MultipleRequests.find(buffer);

	if(!dwStatus && it != pSecurityProfile->m_MultipleRequests.end())
	{
		pSecurityProfile->OnSecurityInfo(it->second->m_recParams, pRec->securityProfile);
	}
	/*else if(bNotify)
	{
		//error
		CErrorResponsePtr pError = CErrorResponsePtr(new CErrorResponse());
		pError->m_enRequestType = enRequestLastQuote;
		if(it != pPrice->m_MultipleRequests.end())
			const_cast<_QuoteUpdateParams&>(it->second->m_recParams).CopyTo(pError->m_vtRequest);			
		else
			pError->m_vtRequest = vtMissing;

		pError->m_bstrDescription = L"Could not get the last quote data";
		pError->m_Error = enNoDataAvailableForSymbol;
		pPrice->PublicResponse(boost::shared_dynamic_cast<CResponseBase>(pError));		
	}*/
	return FALSE;
}



