#ifndef _FIXTAG_H
#define _FIXTAG_H

#include "FixConst.h"

#define ALLOCATION_GRANULARITY		1024

///*-------------------------------------------------------------------------------
//	CFixBuffer class
//--------------------------------------------------------------------------------*/
//
class CFixBuffer : public CLock
{
	void * m_pData;
	DWORD  m_dwLen;
	DWORD  m_dwSize;

protected:

	void   PrepareBuffer(DWORD dwDelta)
	{
		if (m_dwLen + dwDelta <= m_dwSize)
			return;

		if (dwDelta < ALLOCATION_GRANULARITY)
			dwDelta = ALLOCATION_GRANULARITY;
		
        m_dwSize += dwDelta;
        if (!m_pData)
        {
            m_pData = malloc(m_dwSize);
            _ASSERTE(m_pData);
        }
        else
        {
            void *pNewData = malloc(m_dwSize);
            _ASSERTE(pNewData);
            memcpy(pNewData, m_pData, m_dwLen);
            free(m_pData);
            m_pData = pNewData;
        }
	}

public:

	void Clear()
	{
		if (m_pData)
		{
			free(m_pData);
			m_pData = NULL;
		}

		m_dwLen = 0;
		m_dwSize = 0;
	}

	void Put(const void * pData, const DWORD dwLen)
	{
		PrepareBuffer(dwLen);
		memcpy((char*)m_pData + m_dwLen, pData, dwLen);
		m_dwLen += dwLen;
	}

	bool IsEmpty() const {return m_pData == NULL;}

	void ShiftLeft(const DWORD dwDelta)
	{
		if (m_dwLen > dwDelta)
		{
			m_dwLen -= dwDelta;

			if (m_dwLen)
				memcpy(m_pData, ((char*)m_pData) + dwDelta, m_dwLen);
		}
        else
        {        
            Clear();
        }
	}

public:

	CFixBuffer() : m_pData(NULL), m_dwLen(0), m_dwSize(0){}
	
	virtual ~CFixBuffer(){Clear();}

	CFixBuffer(const CFixBuffer& Origin) : m_pData(NULL), m_dwLen(0), m_dwSize(0)
	{
		Put(Origin.GetData(), Origin.GetLength());
	}

	CFixBuffer& operator=(const CFixBuffer& Origin) 
	{
		Clear();
		Put(Origin.GetData(), Origin.GetLength());
		return *this;
	}

	inline void * GetData() const {return m_pData;}
	inline DWORD  GetLength() const {return m_dwLen;}
};

/*-------------------------------------------------------------------------------
	CFixTime & Date & Binary classes
--------------------------------------------------------------------------------*/

struct CFixTimeValue : public tm {};
struct CFixDateValue : public tm {};
struct CFixMonthYearValue : public tm {};
struct CFixDayOfMonthValue : public tm {};
struct CFixBinaryValue
{
	void * m_pData;
	DWORD  m_dwLen;
};

/*-------------------------------------------------------------------------------
	CFixTagValue class
--------------------------------------------------------------------------------*/

class CFixTagValue : public CFixBuffer
{
public:

	CFixTagValue(){};

	// Char
	CFixTagValue(const char & Value)	
	{									
		char s[100];					
		sprintf(s, "%c", Value);	
		Put(s, strlen(s));				
	}									
	CFixTagValue& operator=(const char & Value)	
	{									
		char s[100];					
		sprintf(s, "%c", Value);	
		Clear();						
		Put(s, strlen(s));				
		return *this;					
	}

	BOOL  GetChar(char & cResult) const
	{
		if (GetLength() != 1)
			return FALSE;

		cResult = *(char*)GetData();
		return TRUE;
	}

	// Integer
	CFixTagValue(const int & Value)	
	{									
		char s[100];					
		sprintf(s, "%d", Value);	
		Put(s, strlen(s));				
	}									
	CFixTagValue& operator=(const int & Value)	
	{									
		char s[100];					
		sprintf(s, "%d", Value);	
		Clear();						
		Put(s, strlen(s));				
		return *this;					
	}
	BOOL  GetInt(int & iResult) const
	{
		if (GetLength() < 1)
			return FALSE;
		
		string Temp((char*)GetData(), GetLength());
		return (sscanf(Temp.c_str(), "%d", &iResult) == 1);
	}
	CFixTagValue(const long & Value)	
	{									
		char s[100];					
		sprintf(s, "%d", Value);	
		Put(s, strlen(s));				
	}									
	CFixTagValue& operator=(const long & Value)	
	{									
		char s[100];					
		sprintf(s, "%d", Value);	
		Clear();						
		Put(s, strlen(s));				
		return *this;					
	}
	BOOL  GetLong(long & lResult) const
	{
		if (GetLength() < 1)
			return FALSE;
		
		string Temp((char*)GetData(), GetLength());
		return (sscanf(Temp.c_str(), "%d", &lResult) == 1);
	}

	// Unsigned integer
	CFixTagValue(const unsigned int & Value)	
	{									
		char s[100];					
		sprintf(s, "%u", Value);	
		Put(s, strlen(s));				
	}									
	CFixTagValue& operator=(const unsigned int & Value)	
	{									
		char s[100];					
		sprintf(s, "%u", Value);	
		Clear();						
		Put(s, strlen(s));				
		return *this;					
	}
	BOOL  GetUnsignedInt(unsigned int & uiResult) const
	{
		if (GetLength() < 1)
			return FALSE;
		
		string Temp((char*)GetData(), GetLength());
		return (sscanf(Temp.c_str(), "%u", &uiResult) == 1);
	}
	CFixTagValue(const unsigned long & Value)	
	{									
		char s[100];					
		sprintf(s, "%u", Value);	
		Put(s, strlen(s));				
	}									
	CFixTagValue& operator=(const unsigned long & Value)	
	{									
		char s[100];					
		sprintf(s, "%u", Value);	
		Clear();						
		Put(s, strlen(s));				
		return *this;					
	}
	BOOL  GetUnsignedLong(unsigned long & ulResult) const
	{
		if (GetLength() < 1)
			return FALSE;
		
		string Temp((char*)GetData(), GetLength());
		return (sscanf(Temp.c_str(), "%u", &ulResult) == 1);
	}

	// float & double
	CFixTagValue(const float & Value)	
	{									
		char s[100];					
		sprintf(s, "%.15f", Value);	
		Put(s, strlen(s));				
	}									
	CFixTagValue& operator=(const float & Value)	
	{									
		char s[100];					
		sprintf(s, "%.15f", Value);	
		Clear();						
		Put(s, strlen(s));				
		return *this;					
	}
	BOOL  GetFloat(float & fResult) const
	{
		if (GetLength() < 1)
			return FALSE;

		
		string Temp((char*)GetData(), GetLength());
		return (sscanf(Temp.c_str(), "%f", &fResult) == 1);
	}

	// string
	CFixTagValue(const char* Value)	
	{									
		Put(Value, strlen(Value));				
	}									
	CFixTagValue& operator=(const char* Value)	
	{						
		Clear();			
		Put(Value, strlen(Value));				
		return *this;					
	}
	BOOL  GetString(string & sResult) const
	{
		if (GetLength() < 1)
			return FALSE;
		
		sResult.assign((char*)GetData(), GetLength());
		return TRUE;
	}

	// binary CFixBinaryValue
	CFixTagValue(const CFixBinaryValue& Value)	
	{									
		Put(Value.m_pData, Value.m_dwLen);				
	}									
	CFixTagValue& operator=(const CFixBinaryValue& Value)	
	{						
		Clear();			
		Put(Value.m_pData, Value.m_dwLen);				
		return *this;					
	}
	BOOL  GetBinary(CFixBinaryValue & Result) const
	{
		Result.m_dwLen = GetLength();
		Result.m_pData = GetData();

		return TRUE;
	}

	// Date & Time
	CFixTagValue(const CFixTimeValue& Value)	
	{									
		char s[100];					
		sprintf(s, "%04d%02d%02d-%02d:%02d:%02d", 
			Value.tm_year + 1900, Value.tm_mon + 1, Value.tm_mday,
			Value.tm_hour, Value.tm_min, Value.tm_sec);	
		Put(s, strlen(s));				
	}									
	CFixTagValue& operator=(const CFixTimeValue& Value)	
	{									
		char s[100];					
		sprintf(s, "%04d%02d%02d-%02d:%02d:%02d", 
			Value.tm_year + 1900, Value.tm_mon + 1, Value.tm_mday,
			Value.tm_hour, Value.tm_min, Value.tm_sec);	
		Clear();
		Put(s, strlen(s));				
		return *this;					
	}
	BOOL  GetTime(CFixTimeValue & Result) const
	{
		if (GetLength() != 17)
			return FALSE;

		if (((char*)GetData())[8]  != '-' || 
		   ((char*)GetData())[11] != ':' ||
		   ((char*)GetData())[14] != ':' )
		   return FALSE;
		
		string Temp;

		Temp.assign((char*)GetData(), 4);
		if (sscanf(Temp.c_str(), "%u", &Result.tm_year) != 1) return FALSE;
		Temp.assign(((char*)GetData()) + 4, 2);
		if (sscanf(Temp.c_str(), "%u", &Result.tm_mon) != 1) return FALSE;
		Temp.assign(((char*)GetData()) + 6, 2);
		if (sscanf(Temp.c_str(), "%u", &Result.tm_mday) != 1) return FALSE;
		Temp.assign(((char*)GetData()) + 9, 2);
		if (sscanf(Temp.c_str(), "%u", &Result.tm_hour) != 1) return FALSE;
		Temp.assign(((char*)GetData()) + 12, 2);
		if (sscanf(Temp.c_str(), "%u", &Result.tm_min) != 1) return FALSE;
		Temp.assign(((char*)GetData()) + 15, 2);
		if (sscanf(Temp.c_str(), "%u", &Result.tm_sec) != 1) return FALSE;
		
		Result.tm_year -= 1900;
		Result.tm_mon -= 1;

		return TRUE;
	}


	CFixTagValue(const CFixDateValue& Value)	
	{									
		char s[100];					
		sprintf(s, "%04d%02d%02d", 
			Value.tm_year + 1900, Value.tm_mon + 1, Value.tm_mday);	
		Put(s, strlen(s));				
	}									
	CFixTagValue& operator=(const CFixDateValue& Value)	
	{									
		char s[100];					
		sprintf(s, "%04d%02d%02d", 
			Value.tm_year + 1900, Value.tm_mon + 1, Value.tm_mday);	
		Clear();
		Put(s, strlen(s));				
		return *this;					
	}
	BOOL  GetDate(CFixDateValue & Result) const
	{
		if (GetLength() != 8)
			return FALSE;
		
		string Temp;

		Temp.assign((char*)GetData(), 4);
		if (sscanf(Temp.c_str(), "%u", &Result.tm_year) != 1) return FALSE;
		Temp.assign(((char*)GetData()) + 4, 2);
		if (sscanf(Temp.c_str(), "%u", &Result.tm_mon) != 1) return FALSE;
		Temp.assign(((char*)GetData()) + 6, 2);
		if (sscanf(Temp.c_str(), "%u", &Result.tm_mday) != 1) return FALSE;
		
		Result.tm_year -= 1900;
		Result.tm_mon -= 1;

		return TRUE;
	}


	CFixTagValue(const CFixMonthYearValue& Value)	
	{									
		char s[100];					
		sprintf(s, "%04d%02d", Value.tm_year + 1900, Value.tm_mon + 1);	
		Put(s, strlen(s));				
	}									
	CFixTagValue& operator=(const CFixMonthYearValue& Value)	
	{									
		char s[100];					
		sprintf(s, "%04d%02d", Value.tm_year + 1900, Value.tm_mon + 1);	
		Clear();
		Put(s, strlen(s));				
		return *this;					
	}
	BOOL  GetMonthYear(CFixMonthYearValue & Result) const
	{
		if (GetLength() != 6)
			return FALSE;

		string Temp;

		Temp.assign((char*)GetData(), 4);
		if (sscanf(Temp.c_str(), "%u", &Result.tm_year) != 1) return FALSE;
		Temp.assign(((char*)GetData()) + 4, 2);
		if (sscanf(Temp.c_str(), "%u", &Result.tm_mon) != 1) return FALSE;
		
		Result.tm_year -= 1900;
		Result.tm_mon -= 1;

		return TRUE;
	}


	CFixTagValue(const CFixDayOfMonthValue& Value)	
	{									
		char s[100];					
		sprintf(s, "%02d", Value.tm_mday);	
		Put(s, strlen(s));				
	}									
	CFixTagValue& operator=(const CFixDayOfMonthValue& Value)	
	{									
		char s[100];					
		sprintf(s, "%02d", Value.tm_mday);	
		Clear();
		Put(s, strlen(s));				
		return *this;					
	}
	BOOL  GetDayOfMonth(CFixDayOfMonthValue& Result) const
	{
		if (GetLength() != 2)
			return FALSE;

		string Temp;

		Temp.assign((char*)GetData(), 2);
		if (sscanf(Temp.c_str(), "%u", &Result.tm_mday) != 1) return FALSE;

		return TRUE;
	}


	void Assign(const char * const pData, const DWORD dwDataLen = -1)
	{
		Clear();

		DWORD dwLen = (dwDataLen == -1) ? (strchr(pData, SOH) - pData) - 1 : dwDataLen;
		if (dwLen > 0)
		{
			Put(pData, dwLen);
		}
	}
};

/*-------------------------------------------------------------------------------
	CFixTag class
--------------------------------------------------------------------------------*/

class CFixTag : public CFixBuffer
{
	FixTagEnum	   m_Key;
	CFixTagValue   m_Value;

public:

	const FixTagEnum&	GetKey() const {return m_Key;}
	const CFixTagValue&	GetValue() const {return m_Value;}
	

	CFixTag(const FixTagEnum Key, const CFixTagValue & Value, const bool bSOH = true)
	{
		Assign(Key, Value, bSOH);
	}

	CFixTag(const char * const pData, const DWORD dwDataLen = -1)
	{
		Assign(pData, dwDataLen);
	}

	void Assign(const char * const pData, const DWORD dwDataLen = -1)
	{
		DWORD dwLen = (dwDataLen == -1) ? (strchr(pData, SOH) - pData) : dwDataLen;
		if (dwLen > 2 && pData[1] == '=')
		{
			m_Key = (FixTagEnum)pData[0];
			m_Value.Assign(pData, dwLen - 2);
		}
		else
			m_Key = enFtUnknown;
	}

	void Assign(const FixTagEnum Key, const CFixTagValue & Value, const bool bSOH = true)
	{
		char s[100];
		sprintf(s, "%d=", (long)Key);
		Put(s, strlen(s));
		Put(Value.GetData(), Value.GetLength());
		if (bSOH)
		{
			static unsigned char chSOH = SOH;
			Put(&chSOH, 1);
		}

		m_Key = Key;
		m_Value = Value;
	}

};

#endif