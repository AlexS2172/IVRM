#include "stdafx.h"

#include "FixMessage.h"
#include "Trace.h"
#include "SgConst.h"

_bstr_t	g_bsTargetCompID;
_bstr_t	g_bsSenderCompID;

char* CFixMessage::GenerateCheckSum(const char* buf, const long bufLen )
{
	static char tmpBuf[ 4 ];
	long idx;
	unsigned int cks;

	for( idx = 0L, cks = 0; idx < bufLen; cks += (unsigned int)buf[ idx++ ] );
	sprintf( tmpBuf, "%03d", (unsigned int)( cks % 256 ) );
	return( tmpBuf );	
}


void	CFixMessage::BeginConstructMessage(const FixMsgTypeEnum MsgType,
										   const int nSeqNo, CFixMessage & Body)
{
	// Current time in FIX format
	time_t tmNow = time(NULL);
	struct tm* tmTm = gmtime(&tmNow);
	CFixTimeValue	Time;
	memcpy(&Time, tmTm, sizeof(Time));

	m_Type = MsgType;
	m_nSeqNum = nSeqNo;

	// Begin header construction
	Body << CFixTag(enFtMsgType, (char)MsgType)
		 << CFixTag(enFtSenderCompID, (char*)g_bsSenderCompID)
		 << CFixTag(enFtTargetCompID, (char*)g_bsTargetCompID)
		 << CFixTag(enFtMsgSeqNum, nSeqNo)
		 << CFixTag(enFtSendingTime, Time);
}


void    CFixMessage::EndConstructMessage(CFixMessage & Body)
{
	// End header construction
	*this << CFixTag(enFtBeginString, FIX_BEGIN_STRING)
		  << CFixTag(enFtBodyLength, Body.GetLength());

	// Merging header & body
	*this << Body;

	// Prepare trailer
	const char* pCheckSum = GenerateCheckSum((char*)GetData(), GetLength());
	*this << CFixTag(enFtCheckSum, pCheckSum);
}

/*-------------------------------------------------------------------------------
	CFixMessage MakeXXX methods
--------------------------------------------------------------------------------*/

void	CFixMessage::MakeLogon(const int nSeqNo, const bool bResetSeqNo)
{
	CFixMessage	Body;

	BeginConstructMessage(enFmtLogon, nSeqNo, Body);
	
	Body << CFixTag(enFtEncryptMethod, (unsigned int)enFemNone)
		 << CFixTag(enFtHeartBtInt, HEART_BIT_INTERVAL);

	if(bResetSeqNo)
	{
		 Body << CFixTag(enFtResetSeqNumFlag, 'Y');
	}

	EndConstructMessage(Body);
};

void	CFixMessage::MakeTestRequest(const int nSeqNo, const char * const szTestReqID)
{
	CFixMessage	Body;

	BeginConstructMessage(enFmtTestRequest, nSeqNo, Body);

	Body << CFixTag(enFtTestReqID, szTestReqID);

	EndConstructMessage(Body);
};

void	CFixMessage::MakeHeartBeat(const int nSeqNo, const char * const szTestReqID)
{
	CFixMessage	Body;

	BeginConstructMessage(enFmtHeartBeat, nSeqNo, Body);

	if (szTestReqID)
		Body << CFixTag(enFtTestReqID, szTestReqID);

	EndConstructMessage(Body);
}

void	CFixMessage::MakeResendRequest(const int nSeqNo, const int nBeginSeqNo, 
									   const int nEndSeqNo)
{
	CFixMessage	Body;

	BeginConstructMessage(enFmtResendRequest, nSeqNo, Body);

	Body << CFixTag(enFtBeginSeqNo, nBeginSeqNo)
		 << CFixTag(enFtEndSeqNo, nEndSeqNo);

	EndConstructMessage(Body);
};

void	CFixMessage::MakeReject(const int nSeqNo, const int nRefSeqNum, 
								const char * const szText)
{
	CFixMessage	Body;

	BeginConstructMessage(enFmtReject, nSeqNo, Body);

	Body << CFixTag(enFtRefSeqNum, nRefSeqNum);

	if (szText)
		 Body << CFixTag(enFtText, szText);

	EndConstructMessage(Body);
};

void	CFixMessage::MakeSequenceReset(const int nSeqNo, const int nNewSeqNo, 
									   const char cGapFillFlag)
{
	CFixMessage	Body;

	BeginConstructMessage(enFmtSequenceReset, nSeqNo, Body);

	Body << CFixTag(enFtNewSeqNo, nNewSeqNo);

	if (cGapFillFlag != 'N')
		 Body << CFixTag(enFtGapFillFlag, cGapFillFlag);

	EndConstructMessage(Body);
}

void	CFixMessage::MakeLogout(const int nSeqNo, const char * const szText)
{
	CFixMessage	Body;

	BeginConstructMessage(enFmtLogout, nSeqNo, Body);

	if (szText)
		 Body << CFixTag(enFtText, szText);

	EndConstructMessage(Body);
}

DWORD CFixMessage::ParseTextTag(const char* const pMsgPtr, 
								const DWORD dwMsgLen,
								FixTagEnum& Key,
								char*& pTagValue,
								DWORD& dwTagValueLen,
								DWORD& dwTagLen)
{
	if (dwMsgLen == 0)
		return ERROR_INSUFFICIENT_BUFFER;

	const char * pSOH = pMsgPtr;
	const char * pEqual = NULL;
	dwTagLen = 0;

	while(dwTagLen++ != dwMsgLen)
	{
		if (*pSOH == SOH)
			break;
		
		if (*pSOH == '=')
			pEqual = pSOH;

		pSOH++;
	}

	// SOH found?
	if (*pSOH != SOH)
		return ERROR_INSUFFICIENT_BUFFER;

	// Sign '=' found?
	if (pEqual == NULL)
	{
		TraceDumpTag(_T("Expected '=' not found."), pMsgPtr, dwTagLen);
		return ERROR_INVALID_DATA;
	}

	// Parsing tag
	int iKey;
	if (sscanf(pMsgPtr, "%d", &iKey) != 1)
	{
		TraceDumpTag(_T("Invalid Tag Key."), pMsgPtr, dwTagLen);
		return ERROR_INVALID_DATA;
	}

	pTagValue = (char*)++pEqual;
	dwTagValueLen = pSOH - pTagValue;
	
	Key = (FixTagEnum)iKey;

	return ERROR_SUCCESS;
}


DWORD CFixMessage::Parse(CFixBuffer& Buffer)
{
	static const char * const pStandardBeginString = FIX_BEGIN_STRING;

	Clear();

	char* pPtr = (char*)Buffer.GetData();
	DWORD dwLen = Buffer.GetLength();
	DWORD dwMsgLen = 0;
	DWORD dwParseTagResult;
	DWORD dwTagLen;
	DWORD dwTagValueLen;
	char* pTagValue;
	FixTagEnum TagKey;
	DWORD dwTagNumber = 1;
	DWORD dwBodyLen = 0;
	DWORD dwParsedBodyLen = 0;
	bool  bComplete = false;

	while(!bComplete)
	{
		dwParseTagResult = ParseTextTag(pPtr, dwLen, 
										TagKey, pTagValue, dwTagValueLen, dwTagLen);
		switch(dwParseTagResult)
		{
			case ERROR_INSUFFICIENT_BUFFER:
			case ERROR_INVALID_DATA:
				return dwParseTagResult;
		}

		switch(dwTagNumber)
		{
			case 1:
			{
				// Message must begin with "8=FIX.4.1<SOH>"
				if (TagKey != enFtBeginString ||
					memcmp(pStandardBeginString, pTagValue, dwTagValueLen))
				{
					TraceDumpTag(_T("Invalid header (expected FIX.4.1)"), 
						pPtr, dwTagLen);

					return ERROR_INVALID_DATA;
				}
				break;
			}

			case 2:
			{
				// Next must be 9=XXX<SOH> where XXX = LENGTH of the Message
				bool bOK = false;

				if (TagKey == enFtBodyLength)
				{
					CFixTagValue Value;
					Value.Assign(pTagValue, dwTagValueLen);

					if (Value.GetUnsignedLong(dwBodyLen) == TRUE)
					{
						if (dwBodyLen > dwLen)
							return ERROR_INSUFFICIENT_BUFFER;

						bOK = true;
					}
				}

				if (!bOK)
				{
					TraceDumpTag(_T("Invalid header (Expected length field)"), 
						pPtr, dwTagLen);

					return ERROR_INVALID_DATA;
				}
				break;
			}

			default:
			{
				if (dwParsedBodyLen == dwBodyLen)
				{
					bool bOK = false;
					char * pCheckSum = GenerateCheckSum((char*)Buffer.GetData(), dwMsgLen);

					// checksum must be the last tag in the message
					if (TagKey == enFtCheckSum && dwTagValueLen == 3)
					{
						if (strncmp(pCheckSum, pTagValue, 3) == 0)
						{
							bOK = true;
							bComplete = true;
						}
					}

					if (!bOK)
					{
						TraceDumpTag(_T("Invalid trailer (Check sum is not valid)"), 
							pPtr, dwTagLen);
						CTracer::Trace(_T("Valid CheckSum = %s"), pCheckSum);

						return ERROR_INVALID_DATA;
					}
				}
				else
				{
					dwParsedBodyLen += dwTagLen;

					CFixTagValue Value;
					Value.Assign(pTagValue, dwTagValueLen);
					*this << CFixTag(TagKey, Value);
				}
				break;				
			}
		}


		dwTagNumber++;
		dwLen -= dwTagLen;
		pPtr += dwTagLen;
		dwMsgLen += dwTagLen;
	}

	// Succeeded
	CFixBuffer::Clear();
	Put(Buffer.GetData(), dwMsgLen);
	return ERROR_SUCCESS;
}

void CFixMessage::TraceDumpTag(	const TCHAR* const szText, 
								const char* const pTagPtr, 
								const DWORD dwTagLen)
{
	// Tag values may be binary.
	// In order not to crash in strlen we are to protect buffer

	char* pTagValue = (char*)malloc(dwTagLen + 1);
	memset(pTagValue, 0, dwTagLen + 1);

	if (pTagPtr)
		strncpy(pTagValue, pTagPtr, dwTagLen);
	CTracer::Trace(_T("%s (Tag = '%s')"), szText, pTagValue);
	
	free(pTagValue);
}