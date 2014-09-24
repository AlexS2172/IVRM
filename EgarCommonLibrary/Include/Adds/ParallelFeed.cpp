/*
* This software is copyright protected (c) 1993-2003 by EGAR Technology 
*
* Project: ETX
* Status:  CORE
*
* Author: Eugene M.Koshelev
* E-mail: Koshelev@egartech.com 
*
* Owner:  Eugene M.Koshelev
* E-mail: Koshelev@egartech.com
*
* $Header:$
*
*/

// ParallelFeed.cpp : Implementation of CParallelFeed
#include "stdafx.h"

#include <stdio.h>
#include <time.h>

#include "DREParallelFeed.h"
#include "ParallelFeed.h"
#include <list>

#include "logger.h"

//////////////////////////////////////////////////////////////////////
//
#define ERRCHK(error, pszDesc)	\
	__ErrCheck(error, L"CParallelFeed", pszDesc, __FILE__, __LINE__)

#define ERRSET(error, pszDesc)	\
	__ErrSet(error, L"CParallelFeed", pszDesc, __FILE__, __LINE__)

#define ERRSETEX(error, pszDesc)	\
	__ErrSetEx(error, L"CParallelFeed", pszDesc, __FILE__, __LINE__)

#define ERRTRW(error, pszDesc)	\
	__ErrThrow(error, L"CParallelFeed", pszDesc, __FILE__, __LINE__)

#define ERRCHKNULL(expression, pszDesc)	\
	{	\
		if(NULL == expression)	\
			__ErrThrow(E_FAIL, L"CParallelFeed", pszDesc, __FILE__, __LINE__);	\
	}

#define ERRCHKEX(expression, pszDesc)	\
	{	\
		try	\
		{	\
			expression;	\
		}	\
		catch(const _com_error& e)	\
		{	\
			__ErrThrow(e, L"CParallelFeed", pszDesc, __FILE__, __LINE__);	\
		}	\
		catch(CATCH_ALL_TYPE)	\
		{	\
			__ErrThrow(E_UNEXPECTED, L"CParallelFeed", pszDesc, __FILE__, __LINE__);	\
		}	\
	}

#define ERRCHKNULLEX(expression, expressionNull, pszDesc)	\
	{	\
		try	\
		{	\
			expression;	\
		}	\
		catch(const _com_error& e)	\
		{	\
			__ErrThrow(e, L"CParallelFeed", pszDesc, __FILE__, __LINE__);	\
		}	\
		catch(CATCH_ALL_TYPE)	\
		{	\
			__ErrThrow(E_UNEXPECTED, L"CParallelFeed", pszDesc, __FILE__, __LINE__);	\
		}	\
		if(NULL == expressionNull)	\
			__ErrThrow(E_FAIL, L"CParallelFeed", pszDesc, __FILE__, __LINE__);	\
	}

#define ERRIGNORE(expression)	\
	{	\
		try	\
		{	\
			expression;	\
		}	\
		catch(CATCH_ALL_TYPE)	\
		{	\
		}	\
	}

// Hash Function
typedef  unsigned long  int  ub4;   /* unsigned 4-byte quantities */
typedef  unsigned       char ub1;   /* unsigned 1-byte quantities */

#define hashsize(n) ((ub4)1<<(n))
#define hashmask(n) (hashsize(n)-1)

/*
--------------------------------------------------------------------
mix -- mix 3 32-bit values reversibly.
For every delta with one or two bits set, and the deltas of all three
high bits or all three low bits, whether the original value of a,b,c
is almost all zero or is uniformly distributed,
* If mix() is run forward or backward, at least 32 bits in a,b,c
have at least 1/4 probability of changing.
* If mix() is run forward, every bit of c will change between 1/3 and
2/3 of the time.  (Well, 22/100 and 78/100 for some 2-bit deltas.)
mix() was built out of 36 single-cycle latency instructions in a 
structure that could supported 2x parallelism, like so:
a -= b; 
a -= c; x = (c>>13);
b -= c; a ^= x;
b -= a; x = (a<<8);
c -= a; b ^= x;
c -= b; x = (b>>13);
CATCH_ALL_TYPE
Unfortunately, superscalar Pentiums and Sparcs can't take advantage 
of that parallelism.  They've also turned some of those single-cycle
latency instructions into multi-cycle latency instructions.  Still,
this is the fastest good hash I could find.  There were about 2^^68
to choose from.  I only looked at a billion or so.
--------------------------------------------------------------------
*/
#define mix(a,b,c) \
{ \
	a -= b; a -= c; a ^= (c>>13); \
	b -= c; b -= a; b ^= (a<<8); \
	c -= a; c -= b; c ^= (b>>13); \
	a -= b; a -= c; a ^= (c>>12);  \
	b -= c; b -= a; b ^= (a<<16); \
	c -= a; c -= b; c ^= (b>>5); \
	a -= b; a -= c; a ^= (c>>3);  \
	b -= c; b -= a; b ^= (a<<10); \
	c -= a; c -= b; c ^= (b>>15); \
}

/*
--------------------------------------------------------------------
hash() -- hash a variable-length key into a 32-bit value
k       : the key (the unaligned variable-length array of bytes)
len     : the length of the key, counting by bytes
initval : can be any 4-byte value
Returns a 32-bit value.  Every bit of the key affects every bit of
the return value.  Every 1-bit and 2-bit delta achieves avalanche.
About 6*len+35 instructions.

The best hash table sizes are powers of 2.  There is no need to do
mod a prime (mod is sooo slow!).  If you need less than 32 bits,
use a bitmask.  For example, if you need only 10 bits, do
h = (h & hashmask(10));
In which case, the hash table should have hashsize(10) elements.
  
If you are hashing n strings (ub1 **)k, do it like this:
for (i=0, h=0; i<n; ++i) h = hash( k[i], len[i], h);
	
By Bob Jenkins, 1996.  bob_jenkins@burtleburtle.net.  You may use this
code any way you wish, private, educational, or commercial.  It's free.
	  
See http://burtleburtle.net/bob/hash/evahash.html
Use for hash table lookup, or anything where one collision in 2^^32 is
acceptable.  Do NOT use for cryptographic purposes.
--------------------------------------------------------------------
*/

ub4 Hash(ub1 *k, ub4 length, ub4 initval = 0)
{
	register ub4 a,b,c,len;
	
	/* Set up the internal state */
	len = length;
	a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
	c = initval;         /* the previous hash value */
	
	/*---------------------------------------- handle most of the key */
	while (len >= 12)
	{
		a += (k[0] +((ub4)k[1]<<8) +((ub4)k[2]<<16) +((ub4)k[3]<<24));
		b += (k[4] +((ub4)k[5]<<8) +((ub4)k[6]<<16) +((ub4)k[7]<<24));
		c += (k[8] +((ub4)k[9]<<8) +((ub4)k[10]<<16)+((ub4)k[11]<<24));
		mix(a,b,c);
		k += 12; len -= 12;
	}
	
	/*------------------------------------- handle the last 11 bytes */
	c += length;
	switch(len)              /* all the case statements fall through */
	{
	case 11: c+=((ub4)k[10]<<24);
	case 10: c+=((ub4)k[9]<<16);
	case 9 : c+=((ub4)k[8]<<8);
	/* the first byte of c is reserved for the length */
	case 8 : b+=((ub4)k[7]<<24);
	case 7 : b+=((ub4)k[6]<<16);
	case 6 : b+=((ub4)k[5]<<8);
	case 5 : b+=k[4];
	case 4 : a+=((ub4)k[3]<<24);
	case 3 : a+=((ub4)k[2]<<16);
	case 2 : a+=((ub4)k[1]<<8);
	case 1 : a+=k[0];
		/* case 0: nothing left to add */
	}
	mix(a,b,c);
	/*-------------------------------------------- report the result */
	return c;
}

/////////////////////////////////////////////////////////////////////////////
// CParallelFeed

const long CParallelFeed::MAX_THREADS_TO_START	= 128;
const char CParallelFeed::THREAD_POOL_NAME[]	= "ParallelFeedThrPool";
const long CParallelFeed::QUOTIENT				= 0x04c11db7;
const long CParallelFeed::ADO_CACHE_SIZE		= 100;
const long CParallelFeed::ADO_CMD_TIMEOUT		= 7200;
const ULONG CParallelFeed::MAX_CACHE_SIZE		= 100000;
const ULONG CParallelFeed::MAX_QUERY_SIZE		= 512;

CParallelFeed::CParallelFeed()
	: m_ThrPool(THREAD_POOL_NAME, ThrProc, MAX_THREADS_TO_START),
	m_bIsCacheInitialized(false)
{
	CRC32_Init();
	m_pUnkMarshaler = NULL;
}

STDMETHODIMP CParallelFeed::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IParallelFeed
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CParallelFeed::Execute(VARIANT command, VARIANT additionalData, long lPortionSize, VARIANT *pRecordSet)
{
	CLogger logger(NULL);

	try
	{
		if(!m_bIsCacheInitialized)
			ERRTRW(E_FAIL, L"Cache is not initialized");

		if(!pRecordSet)
			ERRTRW(E_INVALIDARG, L"Invalid output dataset argument passed");

		itf_ptr<ParallelFeedData> pData(new ParallelFeedData);

		ERRCHKNULLEX(pData->m_spCommand = _variant_t(command), pData->m_spCommand,
					L"Invalid command object passed");

		ERRCHKNULLEX(pData->m_spAdditionalData = _variant_t(additionalData), pData->m_spAdditionalData,
					L"Invalid additional data object passed");

		pData->m_lPortionSize = lPortionSize;

		pData->m_hReadyEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if(!pData->m_hReadyEvent.IsValid())
			ERRTRW(HRESULT_FROM_WIN32(::GetLastError()), L"Fail to create 'ready' event");

		pData->m_hErrorEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if(!pData->m_hErrorEvent.IsValid())
			ERRTRW(HRESULT_FROM_WIN32(::GetLastError()), L"Fail to create 'error' event");

		pData->m_pData = this;

		// Create data record set
		ERRCHK(pData->m_spRecordSet.CreateInstance(CLSID_DataRecordSet), L"Fail to create results dataset object");

		// Pass structure to the thread pool
		ERRCHKEX(m_ThrPool.PassData(pData), L"Fail to start execution thread");

		// Wait for the event
		HANDLE rghEvents[] = { pData->m_hReadyEvent, pData->m_hErrorEvent };
		DWORD dwWaitResult = WAIT_TIMEOUT;

		while ( WAIT_TIMEOUT == (dwWaitResult = ::WaitForMultipleObjects(sizeof(rghEvents) / sizeof(rghEvents[0]), rghEvents, FALSE, 30000) ) )
		{
			logger.DumpToFile("CParallelFeed::execute still waits for data processing...");
		}

		if(WAIT_OBJECT_0 == dwWaitResult)
		{
			IDataRecordSet2Ptr spClone;
			ERRCHKEX(spClone = pData->m_spRecordSet->Clone(SUBRECORDSET_BEGIN, SUBRECORDSET_COUNT_ALL),
																L"Fail to clone results recordset");
			VariantClearSafe(pRecordSet);
			*pRecordSet = _variant_t(spClone.GetInterfacePtr()).Detach();
		}
		else if(WAIT_OBJECT_0 + 1 == dwWaitResult) // Error
			_com_raise_error(pData->m_hr, pData->m_spErrorInfo);

		else 
			ERRTRW(HRESULT_FROM_WIN32(::GetLastError()), L"Fail to execute database operation");
	}
	catch(const _com_error& e)
	{
		return ERRSET(e, L"Fail to execute command");
	}
	catch(CATCH_ALL_TYPE)
	{
		return ERRSET(E_UNEXPECTED, L"Fail to execute command: Unexpected error");
	}
	return S_OK;
}

STDMETHODIMP CParallelFeed::ProcessUpdate(VARIANT updatedObject, VARIANT additionalData)
{
	try
	{
		if(!m_bIsCacheInitialized)
			ERRTRW(E_FAIL, L"Cache is not initialized");

		_bstr_t strKeyField;
		ERRCHKEX(strKeyField = _variant_t(additionalData),
					L"Invalid key field value passed");

		if(strKeyField.length() == 0)
			ERRTRW(E_INVALIDARG, L"Invalid key field value passed");

		IDataRecordPtr spRecordSrc;
		ERRCHKNULLEX(spRecordSrc = _variant_t(updatedObject), spRecordSrc,
					L"Invalid data record object passed");

		// Make buffer for hash & CRC calculation
		DWORD dwHash = 0L, dwCRC = 0L;
		_variant_t vaKeyValue;
		ERRCHKEX(vaKeyValue = spRecordSrc->FieldData[strKeyField],
					L"Fail to get key field value");

		CalcHashAndCRC(vaKeyValue, dwHash, dwCRC);

		IDataRecordPtr spRecord;
		{
			CLockCS lock(m_Cs);
			HRESULT hRes = m_ObjCache.Get(dwHash, dwCRC, &spRecord);
			ERRCHK(hRes, L"Fail to get record from cache");

			if(hRes == S_OK)
				m_ObjCache.Delete(dwHash, dwCRC);

			ERRCHKEX(m_ObjCache.Put(dwHash, dwCRC, spRecordSrc),
				L"Fail to put record into cache");
		}
	}
	catch(const _com_error& e)
	{
		return ERRSET(e, L"Fail to process update");
	}
	catch(CATCH_ALL_TYPE)
	{
		return ERRSET(E_UNEXPECTED, L"Fail to process update: Unexpected error");
	}
	return S_OK;
}

#ifdef _DEBUG
inline _bstr_t GetTime()
{
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	wchar_t tmStr[128] = {0};
	_snwprintf(tmStr, 127, L"%04d-%02d-%02d %02d:%02d:%02d.%03d", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
	return tmStr;
}

void LogDbg(LPCSTR str)
{
	::OutputDebugStringA(str);
	::OutputDebugStringA("\n");

	char msg[1024] = {0};
	_snprintf(msg,1023, "%s: %s\n", (char*)GetTime(), str);

	FILE * logf = fopen("c:\\ParallelFeed.log", "a");
	fputs(msg, logf);
	fclose(logf);
}
#else
#define LogDbg
#endif

void __cdecl CParallelFeed::ThrProc(ParallelFeedData* pData)
{
	try
	{
		_CommandPtr spCommand;
		ERRCHKNULLEX(spCommand = pData->m_spCommand, spCommand, L"Invalid command object passed");

		IDataRecordSet2Ptr spDrsOut;
		ERRCHKNULLEX(spDrsOut = pData->m_spRecordSet, spDrsOut, L"Invalid result dataset passed");
		
		IEtxMapMessagePtr spMapMsg;
		ERRCHKNULLEX(spMapMsg = pData->m_spAdditionalData, spMapMsg, L"Invalid additional data passed");

		ERRCHKEX(spCommand->ActiveConnection->CursorLocation = adUseClient, L"Fail to set command cursor location");
		ERRCHKEX(spCommand->CommandTimeout = ADO_CMD_TIMEOUT, L"Fail to set command timeout");

		CLogger logger(NULL);
		logger.DumpToFile(_bstr_t(L"Thread started: ") + spCommand->CommandText);
		logger.DumpToFile("Executing fetch...");
		
		_RecordsetPtr spRsResults;
		ERRCHK(spRsResults.CreateInstance(CLSID_Recordset), L"Fail to create recordset object");
		ERRCHK(spRsResults->Open(spCommand.GetInterfacePtr(), vtMissing, adOpenForwardOnly, adLockReadOnly, adOptionUnspecified),
				L"Fail to open source recordset");

		// Retrieve key sequence
		IKeySequenceNamePtr spKeySequence;
		ERRCHKEX(spKeySequence = spMapMsg->Body[KEY_SEQUENCE], L"Fail to get key sequence value from additional data");
		ERRCHKEX(spDrsOut->KeySequence = spKeySequence, L"Fail to set results key sequence value");
		
		// Retrieve key field (one field is supported now)
		_bstr_t strKeyFldName = spMapMsg->Body[KEY_FIELDS];

		spDrsOut->IsFetchCompleted = VARIANT_FALSE;
		
		VARIANT_BOOL bIsReRead = spMapMsg->Body[RE_READ_CACHE];
		VARIANT_BOOL bIsLoadMissed = VARIANT_FALSE;
		if(bIsReRead)
			ERRCHK(spDrsOut->InitializeStructure(_variant_t(spRsResults.GetInterfacePtr())),
				L"Fail to initialize result dataset structure by source recordset while re-read cache");
		else
			ERRCHKEX(bIsLoadMissed = spMapMsg->Body[LOAD_MISSED_RECORDS], L"Fail to get 'LoadMissedRecords' value from additional data");
			
		char szMsg[MAX_PATH] = {0};
		_snprintf(szMsg, MAX_PATH - 1, "Executed fetch (%d records)", spRsResults->RecordCount);
		logger.DumpToFile(szMsg);

		for(long i = 0; !spRsResults->adoEOF; i ++)
		{
			// Make buffer for hash & CRC calculation
			DWORD dwHash = 0L, dwCRC = 0L;
			_variant_t vaKeyValue = spRsResults->Collect[strKeyFldName];
			//logger.DumpToFile(_BS("Record Name: ") + _BS(strKeyFldName)  + _BS(i));
			pData->m_pData->CalcHashAndCRC(vaKeyValue, dwHash, dwCRC);

			IDataRecordPtr spRecord;
			{
				CLockCS lock(pData->m_pData->m_Cs);
				HRESULT hRes = (bIsReRead == VARIANT_FALSE) ? pData->m_pData->m_ObjCache.Get(dwHash, dwCRC, &spRecord) : S_FALSE;
				ERRCHK(hRes, L"Fail to get record from cache");

				if(hRes == S_FALSE)
				{
					if(bIsReRead)
					{
						//logger.DumpToFile("Before RawInsert");
						ERRCHK(spDrsOut->RawInsert(_variant_t(spRsResults.GetInterfacePtr())),
								L"Fail to insert missing record into results dataset while re-read cache");
						ERRCHK(pData->m_pData->m_ObjCache.Put(dwHash, dwCRC, spDrsOut->Record), L"Fail to put record into cache while re-read");
						//logger.DumpToFile("After RawInsert");
					}
					else
					{
						if(bIsLoadMissed)
						{
							//logger.DumpToFile("bIsLoadMissed");
							_bstr_t strFullQueryTemplate = spMapMsg->Body[FULL_TEMPLATE];
							if(strFullQueryTemplate.length() > 0L)
							{
								WCHAR szFullQuery[MAX_QUERY_SIZE] = { 0 };
								_snwprintf(szFullQuery, MAX_QUERY_SIZE - 1, (LPCWSTR)strFullQueryTemplate, (LPCWSTR)_BS(vaKeyValue));
								logger.DumpToFile(_BS("Performing full query: ") + szFullQuery);
								
								// Create command for additional fetch (fill full data required)
								_CommandPtr spCmd;
								ERRCHK(spCmd.CreateInstance(CLSID_Command), L"Fail to create command object for full query");
								spCmd->ActiveConnection = spCommand->ActiveConnection;
								spCmd->CommandTimeout = ADO_CMD_TIMEOUT;
								spCmd->CommandText = szFullQuery;
								spCmd->CommandType = adCmdText;
								
								// Get full data
								_RecordsetPtr spRs;
								ERRCHK(spRs.CreateInstance(CLSID_Recordset), L"Fail to create recordset object for full query");
								spRs->CursorLocation = adUseClient;

								ERRCHK(spRs->Open(spCmd.GetInterfacePtr(), vtMissing, adOpenForwardOnly,
										adLockReadOnly, adOptionUnspecified), L"Fail to execute full query");

								//logger.DumpToFile("Performed full query");
								if(!spRs->adoEOF)
								{
									ERRCHK(spDrsOut->InitializeStructure(spRs.GetInterfacePtr()),
											L"Fail to initialize result dataset structure by missing record while refresh");

									ERRCHK(spDrsOut->RawInsert(_variant_t(spRs.GetInterfacePtr())),
											L"Fail to insert missing record into results dataset while refresh");
	
									ERRCHK(pData->m_pData->m_ObjCache.Put(dwHash, dwCRC, spDrsOut->Record), L"Fail to put record into cache while refresh");
								}
								else
								{
									logger.DumpToFile(_BS("No records were returned by full query for: ") + _BS(vaKeyValue));
								}
							}
							else
							{
								logger.DumpToFile(_BS("No full query template. Skip missing record: ") + _BS(vaKeyValue));
							}
						}
						else
						{
							//logger.DumpToFile(_BS("Skip missing record: ") + _BS(vaKeyValue));
						}
					}
				}
				else
				{
					//logger.DumpToFile("hRes = TRUE");
					if(!bIsReRead)
						ERRCHK(spDrsOut->InitializeStructure(spRecord.GetInterfacePtr()),
								L"Fail to initialize result dataset structure by source record while refresh");

					//logger.DumpToFile("Before hRes = TRUE spDrsOut->RawInsert");
					ERRCHK(spDrsOut->RawInsert(_variant_t(spRecord.GetInterfacePtr())),
							L"Fail to insert record into results dataset");
					//logger.DumpToFile("After hRes = TRUE spDrsOut->RawInsert");
				}
			}
			
			// If first portion of data is filled then set signal event
			if(spDrsOut->Count == pData->m_lPortionSize)
			{
				char szMsg2[256] = { 0 };
				_snprintf(szMsg2, 255, "First portion of %d(%d) records is complete", spDrsOut->Count, i);
				logger.DumpToFile(szMsg2);
				::SetEvent(pData->m_hReadyEvent);
			}
			
			ERRCHK(spRsResults->MoveNext(), L"Fail to move to next record in source recordset");
		}

#ifdef _DEBUG
		char szMsg2[256] = { 0 };
		_snprintf(szMsg2, 255, "End fetch: %ld records", i);
		logger.DumpToFile(szMsg2);
#endif
		spDrsOut->IsFetchCompleted = VARIANT_TRUE;
		::SetEvent(pData->m_hReadyEvent);
	}
	catch(const _com_error& e)
	{
		pData->m_hr = e.Error();
		pData->m_spErrorInfo = ERRSETEX(e, L"Fail to perform database operation");
		::SetEvent(pData->m_hErrorEvent);
	}
	catch(CATCH_ALL_TYPE)
	{
		pData->m_hr = E_UNEXPECTED;
		pData->m_spErrorInfo = ERRSETEX(E_UNEXPECTED, L"Fail to perform database operation: Unexpected error");
		::SetEvent(pData->m_hErrorEvent);
	}
}

inline void CParallelFeed::CRC32_Init(void)
{
	int i, j;
	
	unsigned int crc;
	
	for (i = 0; i < 256; i++)
	{
		crc = i << 24;
		for (j = 0; j < 8; j++)
		{
			if (crc & 0x80000000)
				crc = (crc << 1) ^ QUOTIENT;
			else
				crc = crc << 1;
		}
		m_crctab[i] = crc;
	}
}

inline unsigned int CParallelFeed::GetCRC(unsigned char *data, int len)
{
	unsigned int	result = 0;
	int i;
	
	int srclen = min(len, 4);
	
	char masks[] = {0x18,0x10,0x8,0};
	
	for (i = 0; i < srclen; i ++)
		result |= *data++ << (masks[i]);
	result = ~ result;
	len -=4;
	
	for (i=0; i<len; i++)
	{
		result = (result << 8 | *data++) ^ m_crctab[result >> 24];
	}
	
	return ~result;
}

void CParallelFeed::CalcHashAndCRC(VARIANT& vtValue, DWORD &dwHash, DWORD &dwCRC)
{
	VARIANT vtCopy = { 0 };
	::VariantInit(&vtCopy);

	if(vtValue.vt & VT_BYREF)
		::VariantCopyInd(&vtCopy, &vtValue);
	else
		::VariantCopy(&vtCopy, &vtValue);

	switch(vtCopy.vt)
	{
		case VT_I8:
			dwHash = Hash((unsigned char*)&vtCopy.llVal, sizeof(vtCopy.llVal));
			dwCRC = GetCRC((unsigned char*)&vtCopy.llVal, sizeof(vtCopy.llVal));
			break;
		case VT_I4:
			dwHash = Hash((unsigned char*)&vtCopy.lVal, sizeof(vtCopy.lVal));
			dwCRC = GetCRC((unsigned char*)&vtCopy.lVal, sizeof(vtCopy.lVal));
			break;
		case VT_UI1:
			dwHash = Hash((unsigned char*)&vtCopy.bVal, sizeof(vtCopy.bVal));
			dwCRC = GetCRC((unsigned char*)&vtCopy.bVal, sizeof(vtCopy.bVal));
			break;
		case VT_I2:
			dwHash = Hash((unsigned char*)&vtCopy.iVal, sizeof(vtCopy.iVal));
			dwCRC = GetCRC((unsigned char*)&vtCopy.iVal, sizeof(vtCopy.iVal));
			break;
		case VT_R4:
			dwHash = Hash((unsigned char*)&vtCopy.fltVal, sizeof(vtCopy.fltVal));
			dwCRC = GetCRC((unsigned char*)&vtCopy.fltVal, sizeof(vtCopy.fltVal));
			break;
		case VT_R8:
			dwHash = Hash((unsigned char*)&vtCopy.dblVal, sizeof(vtCopy.dblVal));
			dwCRC = GetCRC((unsigned char*)&vtCopy.dblVal, sizeof(vtCopy.dblVal));
			break;
		case VT_BOOL:
			dwHash = Hash((unsigned char*)&vtCopy.boolVal, sizeof(vtCopy.boolVal));
			dwCRC = GetCRC((unsigned char*)&vtCopy.boolVal, sizeof(vtCopy.boolVal));
			break;
		case VT_ERROR:
			dwHash = Hash((unsigned char*)&vtCopy.scode, sizeof(vtCopy.scode));
			dwCRC = GetCRC((unsigned char*)&vtCopy.scode, sizeof(vtCopy.scode));
			break;
		case VT_CY:
			dwHash = Hash((unsigned char*)&vtCopy.cyVal, sizeof(vtCopy.cyVal));
			dwCRC = GetCRC((unsigned char*)&vtCopy.cyVal, sizeof(vtCopy.cyVal));
			break;
		case VT_DATE:
			dwHash = Hash((unsigned char*)&vtCopy.date, sizeof(vtCopy.date));
			dwCRC = GetCRC((unsigned char*)&vtCopy.date, sizeof(vtCopy.date));
			break;
		case VT_BSTR:
			dwHash = Hash((unsigned char*)vtCopy.bstrVal, wcslen(vtCopy.bstrVal) * sizeof(wchar_t));
			dwCRC = GetCRC((unsigned char*)vtCopy.bstrVal, wcslen(vtCopy.bstrVal) * sizeof(wchar_t));
			break;
		case VT_I1:
			dwHash = Hash((unsigned char*)&vtCopy.cVal, sizeof(vtCopy.cVal));
			dwCRC = GetCRC((unsigned char*)&vtCopy.cVal, sizeof(vtCopy.cVal));
			break;
		case VT_UI2:
			dwHash = Hash((unsigned char*)&vtCopy.uiVal, sizeof(vtCopy.uiVal));
			dwCRC = GetCRC((unsigned char*)&vtCopy.uiVal, sizeof(vtCopy.uiVal));
			break;
		case VT_UI4:
			dwHash = Hash((unsigned char*)&vtCopy.ulVal, sizeof(vtCopy.ulVal));
			dwCRC = GetCRC((unsigned char*)&vtCopy.ulVal, sizeof(vtCopy.ulVal));
			break;
		case VT_UI8:
			dwHash = Hash((unsigned char*)&vtCopy.ullVal, sizeof(vtCopy.ullVal));
			dwCRC = GetCRC((unsigned char*)&vtCopy.ullVal, sizeof(vtCopy.ullVal));
			break;
		case VT_INT:
			dwHash = Hash((unsigned char*)&vtCopy.intVal, sizeof(vtCopy.intVal));
			dwCRC = GetCRC((unsigned char*)&vtCopy.intVal, sizeof(vtCopy.intVal));
			break;
		case VT_UINT:
			dwHash = Hash((unsigned char*)&vtCopy.uintVal, sizeof(vtCopy.uintVal));
			dwCRC = GetCRC((unsigned char*)&vtCopy.uintVal, sizeof(vtCopy.uintVal));
			break;
	}

	VariantClearSafe(&vtCopy);
}

STDMETHODIMP CParallelFeed::InitializeCache(long size, long density)
{
	try
	{
		CLockCS lock(m_Cs);
		ERRCHKEX(m_ObjCache.Clear(), L"Fail to clear parralel feed cache storage");
		ERRCHK(m_ObjCache.Initialize(size, density), L"Fail to initialize parralel feed cache storage");
		m_bIsCacheInitialized = true;
	}
	catch(const _com_error& e)
	{
		return ERRSET(e, L"Fail to initialize parralel feed cache");
	}
	catch(CATCH_ALL_TYPE)
	{
		return ERRSET(E_UNEXPECTED, L"Fail to initialize parralel feed cache: Unexpected error");
	}
	return S_OK;
}

STDMETHODIMP CParallelFeed::GetRecord(VARIANT/*[in]*/ keyfield, VARIANT additionalData, VARIANT *pRecord)
{
	try
	{
		*pRecord = _variant_t();

		if(!m_bIsCacheInitialized)
			ERRTRW(E_FAIL, L"Cache is not initialized");
		
		_bstr_t strKeyField;
		ERRCHKEX(strKeyField = _variant_t(keyfield),
			L"Invalid key field value passed");
		
		if(strKeyField.length() == 0)
			ERRTRW(E_INVALIDARG, L"Invalid key field value passed");

		if(pRecord == NULL)
			ERRTRW(E_INVALIDARG, L"Invalid parameter passed");

		// Make buffer for hash & CRC calculation
		DWORD dwHash = 0L, dwCRC = 0L;
		_variant_t vaKeyValue(additionalData);
		
		CalcHashAndCRC(vaKeyValue, dwHash, dwCRC);
		
		IDataRecordPtr spRecord;
		{
			CLockCS lock(m_Cs);
			HRESULT hRes = m_ObjCache.Get(dwHash, dwCRC, &spRecord);
			ERRCHK(hRes, L"Fail to get record from cache");
			
			if(hRes == S_OK)
				*pRecord = _variant_t(spRecord.GetInterfacePtr()).Detach();
		}
	}
	catch(const _com_error& e)
	{
		return ERRSET(e, L"Fail to process update");
	}
	catch(CATCH_ALL_TYPE)
	{
		return ERRSET(E_UNEXPECTED, L"Fail to process update: Unexpected error");
	}
	return S_OK;
}
