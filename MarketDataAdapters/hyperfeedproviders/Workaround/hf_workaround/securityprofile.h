// SecurityProfile.h : Declaration of the CSecurityProfile

#ifndef __SECURITYPROFILE_H__
#define __SECURITYPROFILE_H__

#include "Database.h"
#include "HyperFeedBase.h"

#define   MAX_REQUESTS     1000

/////////////////////////////////////////////////////////////////////////////
// CSecurityProfile

class CSecurityProfile : 
	public CDatabase<DBA_SECURITY_PROFILE>
{
	typedef std::map<std::string, CQuoteRequestPtr>  CMuttipleRequests;
public:
	typedef struct 
	{
		_bstr_t bsSymb;
		DATE    dtExpiration;
	} 
	sec_info_t;

	virtual ~CSecurityProfile();

	long Attach();
	long Detach();

protected:
	virtual void OnSecurityInfo(const _QuoteUpdateParams& Params, const DBA_RECORD_52& rec) {};
	long RetrieveSecuritiesInfo(const CRequestBasePtr& pParams);

private:
	long GetMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey);
	void _SetDbKey(const _QuoteUpdateParams& Params, DBA_KEY *dbaKey);

	static long CALLBACK _MultipleRequests(VOID* pblk,	// pointer to param block
		ULONG			ulFunc,	// function
		VOID*			pKey,	// pointer to key
		INT				nKeyLen,// length of key struct
		DBA_RECORD*		pRec,	// pointer to return record
		INT				nRecLen,// length of return record
		ULONG			ulKey,	// key number
		DWORD			dwStatus,//Status of the DBA callback
		VOID*			pVoid);//ptr to Stream Filter struct

private:
	EgLib::CLock				m_csMultipleRequests;
	CMuttipleRequests			m_MultipleRequests;
	DBA_MAXKEY					m_pLastQuoteKey[MAX_REQUESTS];
};


#endif //__SECURITYPROFILE_H__
