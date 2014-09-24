#include "stdafx.h"

#include "ISEStatus.h"
#include "ISESession.h"
#include "OmniApiUtils.h"
#include "Trace.h"

bool NormalizeErrorMessage(const int32 iRes, char* szBuf, uint32 uiLen)
{
	switch (iRes)
	{
		case OMNIAPI_NOT_FOUND:
			strcpy (szBuf, "No text translation found for code");
			break;

		case OMNIAPI_TRUNCATED:
		case OMNIAPI_SUCCESS:
			szBuf[uiLen - 1] = 0;
			break;

		default:
			strcpy(szBuf, "No text error available.");
			return false;
			break;
	}

	return true;
}

void GetErrorClass(int32 iStatus, IseErrorClass& Class)
{
	switch(iStatus)
	{
		case OMNIAPI_NOT_LOGGED_IN:		/* OAPI not logged in */
		case OMNIAPI_NET_INIT_FAIL:		/* OAPI network initialization failed */
		case OMNIAPI_NOT_INITIALIZED:	/* OAPI not initialized */
		case OMNIAPI_NO_INFO_RCVD:		/* No network information received */
		case OMNIAPI_VER_INT_ERROR:		/* Message verific. internal error */
		case OMNIAPI_TX_TIMEOUT:		/* Transaction timeout */
		case OMNIAPI_TX_DECLFAIL:		/* Transaction server/requester */
		case OMNIAPI_APINORSP:		    /* Api not responding, GBLSEC reinitialized */
		case OMNIAPI_APIDISCON:		    /* Api disconnected, GBLSEC reinitialized   */

		case OMNIAPI_NONETWORK:			/* no network is present for IPC */
		case OMNIAPI_NOTCONNECTED:		/* invalid operation before LOGIN */
		case OMNIAPI_NOGWYSRV:			/* no gateway service defined */
		case OMNIAPI_BADHOSTNAME:		/* host name could not be translated */
		case OMNIAPI_ERRSOCKET:			/* socket could not be allocated */
		case OMNIAPI_ERRCONNECT:		/* system error occured on 'connect' */
		case OMNIAPI_ERRBIND:			/* system error occured on 'bind' */
		case OMNIAPI_NOSESSION:			/* session is aborted */
		case OMNIAPI_ERRSEND:			/* error on send() */
		case OMNIAPI_APIOLD:			/* OMNIAPI too old for the gateway */
		case OMNIAPI_RECONREJ:			/* reconnection rejected */
		case OMNIAPI_SESINUSE:			/* gateway API session already in use */
			Class = enDisconnect;
			break;

		default:
			Class = enCommon;
			break;
	}
};

char* GetTextError(CISESession* pSession, const int32 iStatus)
{
	__declspec(thread) static char szBuf[1024] = {0};

	uint32 uiLen = sizeof (szBuf) - 1;
	int32 iRes;

	szBuf[0] = 0;

	{
		CAutoLock OmniApiLock(&pSession->m_OmniApiLock);
		iRes = omniapi_get_message_ex ( pSession->GetHandle(), iStatus, 
					szBuf, &uiLen, FALSE );
	}

	NormalizeErrorMessage(iRes, szBuf, uiLen);

	return szBuf;
}

void CISEException::GetErrorString(char* szToString) const
{
    int8   szBuf[1024] = {0};
	int8   szSecBuf[1024] = {0};
    
	uint32 uiLen = sizeof (szBuf) - 1;
	uint32 uiSecLen = sizeof (szSecBuf) - 1;

	int32 iRes;

	bool bConnected = (0 != m_iCStatus);

	if(bConnected)
	{
		{
			CAutoLock OmniApiLock(&m_pSession->m_OmniApiLock);

			iRes = omniapi_get_message_ex ( m_pSession->GetHandle(), m_iCStatus, 
						szBuf, &uiLen, FALSE );
		}

		bConnected = NormalizeErrorMessage(iRes, szBuf, uiLen);
	}

	if(bConnected)
	{
		if (iRes == OMNIAPI_VER_FIELD_ERROR)
		{
			sprintf ( szSecBuf, "Field error offset %u bytes, from start of msg", m_iTransactionStatus);
		}
		else
		{
			if ( m_iTransactionStatus != 0)
			{
				{
					CAutoLock OmniApiLock(&m_pSession->m_OmniApiLock);
				
					iRes = omniapi_get_message_ex ( m_pSession->GetHandle(), m_iTransactionStatus,
							szSecBuf, &uiSecLen, FALSE );
				}

				NormalizeErrorMessage(iRes, szSecBuf, uiSecLen);
			}
		}
	}
	else
    {
		switch (m_iCStatus)
		{
			case OMNIAPI_FAILURE :
				strcpy ( szBuf, "Failure completion" );
				break;
			case OMNIAPI_NOT_READY :
				strcpy ( szBuf, "OAPI not ready" );
				break;
			case OMNIAPI_FACID_NOT_VALID :
				strcpy ( szBuf, "Facility Id not in range" );
				break;
			case OMNIAPI_INVALID_TABLE :
				strcpy ( szBuf, "Invalid table type" );
				break;
			case OMNIAPI_NOT_LOGGED_IN :
				strcpy ( szBuf, "OAPI not logged in" );
				break;
			case OMNIAPI_LOCKED :
				strcpy ( szBuf, "OAPI locked" );
				break;
			case OMNIAPI_ALREADY_LOCKED :
				strcpy ( szBuf, "OAPI already locked" );
				break;
			case OMNIAPI_NET_INIT_FAIL :
				strcpy ( szBuf, "OAPI network initialization failed" );
				break;
			case OMNIAPI_NOT_INITIALIZED :
				strcpy ( szBuf, "OAPI not initialized" );
				break;
			case OMNIAPI_NO_INFO_RCVD :
				strcpy ( szBuf, "No network information received" );
				break;
			case OMNIAPI_NORTR :
				strcpy ( szBuf, "VAX RTR connection failed" );
				break;
			case OMNIAPI_TX_ABORTED :
				strcpy ( szBuf, "Transaction aborted" );
				break;
			case OMNIAPI_TRUNCATED :
				strcpy ( szBuf, "Data truncated" );
				break;
			case OMNIAPI_CNV_NO_RANGE :
				strcpy ( szBuf, "Table conversion range error" );
				break;
			case OMNIAPI_CNV_NOT_SORTED :
				strcpy ( szBuf, "Table conversion sort error" );
				break;
			case OMNIAPI_CNV_OFFS_ERROR :
				strcpy ( szBuf, "Table conversion offset error" );
				break;
			case OMNIAPI_NO_SUCH_ID :
				strcpy ( szBuf, "Invalid transaction type" );
				break;
			case OMNIAPI_VER_FIELD_ERROR :
				strcpy ( szBuf, "Transaction verification error" );
				break;
			case OMNIAPI_VER_INT_ERROR :
				strcpy ( szBuf, "Message verific. internal error" );
				break;
			case OMNIAPI_VER_TABLE_ERROR :
				strcpy ( szBuf, "Table verification error" );
				break;
			case OMNIAPI_TX_TIMEOUT :
				strcpy ( szBuf, "Transaction timeout" );
				break;
			case OMNIAPI_TX_DECLFAIL :
				strcpy ( szBuf, "Transaction server/requester declaration failure" );
				break;
			case OMNIAPI_TX_FAILURE :
				strcpy ( szBuf, "Transaction failure" );
				break;
			case OMNIAPI_DYNMEM :
				strcpy ( szBuf, "Error obtaining dynamic memory" );
				break;
			case OMNIAPI_INVARG :
				strcpy ( szBuf, "Invalid argument" );
				break;
			case OMNIAPI_NOT_FOUND :
				strcpy ( szBuf, "Requested data not found" );
				break;
			case OMNIAPI_ITV_ERROR :
				strcpy ( szBuf, "Information table verification error" );
				break;
			case OMNIAPI_NO_USR_OR_PASSW :
				strcpy ( szBuf, "Username and/or Password missing" );
				break;
			case OMNIAPI_NO_NET_PATH :
				strcpy ( szBuf, "Net Path data missing" );
				break;
			case OMNIAPI_INVEVT :
				strcpy ( szBuf, "Invalid event type" );
				break;
			case OMNIAPI_NOT_LOCKED :
				strcpy ( szBuf, "OAPI not locked" );
				break;
			case OMNIAPI_INVTXTYPE :
				strcpy ( szBuf, "Invalid transaction type" );
				break;
			case OMNIAPI_NOFACILITY :
				strcpy ( szBuf, "OMnet facility not found" );
				break;
			case OMNIAPI_HIGHRTRCHN :
				strcpy ( szBuf, "Expected RTR channel # exceeded" );
				break;
			case OMNIAPI_FATAL :
				strcpy ( szBuf, "Fatal OAPI error" );
				break;
			case OMNIAPI_NOORDERID :
				strcpy ( szBuf, "No order id" );
				break;
			case OMNIAPI_APINORSP :
				strcpy ( szBuf, "Api not responding, GBLSEC reinitialized" );
				break;
			case OMNIAPI_APIDISCON :
				strcpy ( szBuf, "Api disconnected, GBLSEC reinitialized" );
				break;
			case OMNIAPI_NOTAUTH :
				strcpy ( szBuf, "Subscription not authorized, or failed" );
				break;
			case OMNIAPI_RECONNECT :
				strcpy ( szBuf, "successful reconnection" );
				break;
			case OMNIAPI_SUCCESS :
				strcpy ( szToString, m_sDescription.c_str());
				return;
			case OMNIAPI_PROBLEM :
				strcpy ( szBuf, "error in OMNIAPI internal call" );
				break;
			case OMNIAPI_INTFAILURE :
				strcpy ( szBuf, "internal OMNIAPI error" );
				break;
			case OMNIAPI_BADNARGS :
				strcpy ( szBuf, "bad number of arguments" );
				break;
			case OMNIAPI_BADARGVAL :
				strcpy ( szBuf, "bad argument value" );
				break;
			case OMNIAPI_NONETWORK :
				strcpy ( szBuf, "no network is present for IPC" );
				break;
			case OMNIAPI_OSBADCONFIG :
				strcpy ( szBuf, "O/S incorrectly configured" );
				break;
			case OMNIAPI_OAPI :
				strcpy ( szBuf, "problem in the internal OM API" );
				break;
			case OMNIAPI_NOTCONNECTED :
				strcpy ( szBuf, "invalid operation before LOGIN" );
				break;
			case OMNIAPI_NOGWYSRV :
				strcpy ( szBuf, "no gateway service defined" );
				break;
			case OMNIAPI_BADHOSTNAME :
				strcpy ( szBuf, "host name could not be translated" );
				break;
			case OMNIAPI_ERRSOCKET :
				strcpy ( szBuf, "socket could not be allocated" );
				break;
			case OMNIAPI_ERRCONNECT :
				strcpy ( szBuf, "system error occured on 'connect'" );
				break;
			case OMNIAPI_ERRBIND :
				strcpy ( szBuf, "system error occured on 'bind'" );
				break;
			case OMNIAPI_NOSESSION :
				strcpy ( szBuf, "session is aborted" );
				break;
			case OMNIAPI_ERRSEND :
				strcpy ( szBuf, "error on send()" );
				break;
			case OMNIAPI_ERRMEM :
				strcpy ( szBuf, "error on malloc()" );
				break;
			case OMNIAPI_APIOLD :
				strcpy ( szBuf, "OMNIAPI too old for the gateway" );
				break;
			case OMNIAPI_RECONREJ :
				strcpy ( szBuf, "reconnection rejected" );
				break;
			case OMNIAPI_SESINUSE :
				strcpy ( szBuf, "gateway API session already in use" );
				break;
			default:
				sprintf ( szBuf, "Unknown error 0x%08x, untranslatable while offline", 
					m_iCStatus);
				break;
		};
    }

    if (m_iTransactionStatus != 0)
    {
		sprintf(szToString, "%s : Error = 0x%08x ('%s'), Secondary error = 0x%08x ('%s')", 
				m_sDescription.c_str(), 
				m_iCStatus, szBuf,
				m_iTransactionStatus, szSecBuf);
    }
	else
	{
		sprintf(szToString, "%s : Error = 0x%08x ('%s')", m_sDescription.c_str(), 
			m_iCStatus, szBuf);
	}
}

HRESULT	CISEException::GetHRESULT()
{
	if(m_sDescription.length() == 0)
		return S_OK;

	USES_CONVERSION;

	CComPtr<ICreateErrorInfo> spCEI;
	if(SUCCEEDED(::CreateErrorInfo(&spCEI)))
	{
		spCEI->SetDescription(A2W(m_sDescription.c_str()));
		
		CComQIPtr<IErrorInfo> spEI = spCEI;
		ATLASSERT(spEI);
		
		::SetErrorInfo(0, spEI);
	}
	return E_FAIL;
}