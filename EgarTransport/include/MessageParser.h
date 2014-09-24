#ifndef __MESSAGE_PARSER_H__
#define __MESSAGE_PARSER_H__

/*****************************************************************************************************
*****************************************************************************************************/

#include <windows.h>
#include <comutil.h>

#include <vector>
#include <string>
#include <map>

#include "Constants.h"

using namespace std;


/*****************************************************************************************************
*****************************************************************************************************/

template<typename T>
class TStaticField 
{
public:
	TStaticField() { ::ZeroMemory(&m_T, sizeof(T)); }
	TStaticField(const T& rT) { m_T = rT; }
	TStaticField(const TStaticField<T>& r) { m_T = r.m_T; }
	
	operator T() { return m_T; }

	const TStaticField& operator =(const T& rT) 
	{ 
		m_T = rT; 
		return *this; 
	}
	const TStaticField& operator =(const TStaticField<T>& r) 
	{ 
		m_T = r.m_T; 
		return *this; 
	}

	const void* GetData(unsigned long& nSize) const 
	{
		nSize = sizeof(T);
		return &m_T; 
	}

	unsigned long SetData(const void* pData, unsigned long nSize)
	{
		if (nSize < sizeof(T))
			return 0;
		
		m_T = *(T*)(pData);
		return sizeof(T);
	}

private:
	T	m_T;
};

/*****************************************************************************************************
*****************************************************************************************************/

typedef TStaticField<char>				CharF;
typedef TStaticField<int>				IntF;
typedef TStaticField<long>				LongF;
typedef TStaticField<unsigned char>		UCharF;
typedef TStaticField<unsigned short>	UShortF;
typedef TStaticField<unsigned int>		UIntF;
typedef TStaticField<unsigned long>		ULongF;
typedef TStaticField<double>			DoubleF;
typedef TStaticField<float>				FloatF;
typedef TStaticField<DATE>				DateF;
typedef TStaticField<CURRENCY>			CurrencyF;

/*****************************************************************************************************
*****************************************************************************************************/

class StringF
{
public:
	StringF(const char* szStr = "") { m_Data = (szStr ? szStr : ""); }
	StringF(const StringF& r) { m_Data = r.m_Data; }
	
	const char* GetStr() { return m_Data.c_str(); }

	const StringF& operator =(const char* szStr) 
	{ 
		m_Data = (szStr ? szStr : ""); 
		return *this; 
	}
	const StringF& operator =(const StringF& r) 
	{ 
		m_Data = r.m_Data; 
		return *this; 
	}

	const void* GetData(unsigned long& nSize) const 
	{
		nSize = static_cast<unsigned long>(m_Data.size()) + 1;
		return m_Data.c_str(); 
	}

	unsigned SetData(const void* pData, unsigned long nSize)
	{
		unsigned nLen = 1L;
		
		if(pData)
			nLen = static_cast<unsigned long>(strlen((const char*)pData)) + 1L;

		if(nLen > nSize)
			return 0;

		m_Data = (pData ? (const char*)pData : "");
		
		return nLen;
	}

private:
	string	m_Data;
};

/*****************************************************************************************************
*****************************************************************************************************/

class TStringF
{
public:
	TStringF(const TCHAR* szStr = _T("")) { m_Data = szStr; }
	TStringF(const TStringF& r) { m_Data = r.m_Data; }
	
	const TCHAR* GetStr() { return m_Data.c_str(); }

	const TStringF& operator =(const TCHAR* szStr) 
	{ 
		m_Data = szStr; 
		return *this; 
	}
	const TStringF& operator =(const TStringF& r) 
	{ 
		m_Data = r.m_Data; 
		return *this; 
	}

    bool operator < (const TStringF& Compare) const
    {
        return m_Data.compare(Compare.m_Data) < 0;
    }

	const void* GetData(unsigned long& nSize) const 
	{
		nSize = static_cast<unsigned long>((m_Data.size() + 1) * sizeof(TCHAR));
		return m_Data.c_str(); 
	}

	unsigned long SetData(const void* pData, unsigned long nSize)
	{
		unsigned long nLen = static_cast<unsigned long>((_tcslen((const TCHAR*)pData) + 1) * sizeof(TCHAR));

		if (nLen > nSize)
			return 0;

		m_Data = (const TCHAR*)pData;
		
		return nLen;
	}

private:
	tstring	m_Data;
};

/*****************************************************************************************************
*****************************************************************************************************/

class GuidF
{
public:
	GuidF() { ::ZeroMemory(&m_guid, sizeof(GUID)); }
	GuidF(const GUID& guid) { ::CopyMemory(&m_guid,&guid,sizeof(GUID)); }
	GuidF(const GuidF& r) { ::CopyMemory(&m_guid,&r.m_guid,sizeof(GUID)); }
	
	const GuidF& operator =(const GUID& guid) 
	{ 
		::CopyMemory(&m_guid,&guid,sizeof(GUID));
		return *this; 
	}
	const GuidF& operator =(const GuidF& r) 
	{ 
		::CopyMemory(&m_guid,&r.m_guid,sizeof(GUID));
		return *this; 
	}

	const GUID& GetGUID() 
	{ 
		return m_guid;
	}

    bool operator < (const GuidF& Compare) const
    {
        return memcmp(&m_guid,&Compare.m_guid,sizeof(GUID)) < 0;
    }

    bool operator != (const GUID& Compare) const
    {
        return memcmp(&m_guid,&Compare,sizeof(GUID)) != 0;
    }

    bool operator !() const
    {
		GUID guid;
		memset(&guid,0,sizeof(GUID));
        return memcmp(&m_guid,&guid,sizeof(GUID)) != 0;
    }

	const void* GetData(unsigned long& nSize) const 
	{
		nSize = sizeof(GUID);
		return (void*)&m_guid; 
	}

	unsigned long SetData(const void* pData, unsigned long nSize)
	{
		if (nSize < sizeof(GUID))
			return 0;

		::CopyMemory(&m_guid,pData,sizeof(GUID));
		
		return sizeof(GUID);
	}

private:
	GUID	m_guid;
};

/*****************************************************************************************************
	first 4 bytes - size of Data in Blob !!!
*****************************************************************************************************/

class BlobF
{
	typedef vector<byte>	_Data;

public:
	BlobF()
	{
		unsigned long nSize = 0;	
		m_Data.clear();
		m_Data.insert(m_Data.end(),(byte*)&nSize,(byte*)&nSize + sizeof(nSize));
	}

    BlobF(const byte* pData, unsigned long nSize) 
	{
		m_Data.clear();
		m_Data.insert(m_Data.end(),(byte*)&nSize,(byte*)&nSize + sizeof(nSize));
		m_Data.insert(m_Data.end(),pData,pData + nSize);
	}

	BlobF(const BlobF& r) : m_Data(r.m_Data) {}
    
	BlobF(const BSTR bs)
	{
		unsigned long nSize = ::SysStringByteLen(bs);	
		m_Data.clear();
		m_Data.insert(m_Data.end(),(byte*)&nSize,(byte*)&nSize + sizeof(nSize));
		m_Data.insert(m_Data.end(),(byte*)bs, (byte*)bs + nSize );
	}
	
	const BlobF& operator =(const BlobF& r) 
	{ 
		m_Data = r.m_Data;
		return *this;
	}

	const BlobF& operator = (const BSTR bs)
	{
		unsigned long nSize = ::SysStringByteLen(bs);	
		m_Data.clear();
		m_Data.insert(m_Data.end(),(byte*)&nSize,(byte*)&nSize + sizeof(nSize));
		m_Data.insert(m_Data.end(),(byte*)bs, (byte*)bs + nSize );
		return *this;
	}

    operator BSTR() const
    {
		
        return ::SysAllocStringByteLen((LPCSTR)&(*m_Data.begin()) + sizeof(unsigned long) , m_Data.size() - sizeof(unsigned long));
    }

	const void* GetData(unsigned long& nSize) const 
	{
		nSize = m_Data.size();
		return &(*m_Data.begin()); 
	}

	unsigned long SetData(const void* pData, unsigned long nSize)
	{
		unsigned long nRealSize = *(unsigned long*)(pData);

        m_Data.clear();
		m_Data.insert(m_Data.end(), (byte*)pData, (byte*)pData + nRealSize + sizeof(unsigned long));
		
		return m_Data.size();
	}

private:

	_Data		m_Data;
};

/*****************************************************************************************************
*****************************************************************************************************/

class CEgPacket
{
	typedef vector<byte>			_Buffer;
	typedef _Buffer::iterator		_Cursor;

public:
	CEgPacket() : m_nOffset(0) {}

	CEgPacket(const void* pData, unsigned long nSize) : m_nOffset(0)
	{
		if (pData == NULL)
			throw ERROR_INVALID_DATA;

		if (nSize > 0)
			_Insert(pData, nSize);
	};

	template <class T>
	CEgPacket& operator << (T& rT) 
	{ 
		unsigned long nSize;
		const void* pData = rT.GetData(nSize);

		if (nSize > 0)
			_Insert(pData, nSize);

		return *this;
	}

	template <class T>
	CEgPacket& operator >> (T& rT) 
	{ 
		if (Eof())
			throw ERROR_INVALID_ACCESS;

		unsigned long nSize;
		
		nSize = rT.SetData(&(*m_Buffer.begin()) + m_nOffset, 
								m_Buffer.size() - m_nOffset);

		if (nSize == 0)
			throw ERROR_INVALID_DATA;

		m_nOffset += nSize;

		return *this;
	}

	bool Eof() 
	{ 
		return m_nOffset == m_Buffer.size(); 
	}

	void* GetBuffer(unsigned long& nSize)
	{
		nSize = m_Buffer.size();
		return &(*m_Buffer.begin());
	}

private:
	void _Insert(const void* pData, unsigned long nSize)
	{
		m_Buffer.insert(m_Buffer.end(),
						(byte*)pData, (byte*)pData + nSize);
	}

	_Buffer		m_Buffer;
	unsigned long	m_nOffset;
};

#define BEGIN_PACKING				virtual bool Packing(CEgPacket& rPacket, bool bUnpack = false)	\
									{																\
										try															\
										{															\
											if(!bUnpack)											\
											{														\
												CAutoLock lock(*this);								\
												m_ulSize = GetMsgSize();							\
											}															


#define FIELD_ENTRY(Field)			bUnpack ? rPacket >> Field : rPacket << Field;
#define END_PACKING					} catch(...) { return false; } return true; } 

/*****************************************************************************************************
*****************************************************************************************************/

class CEgSocketBuffer
{
	typedef vector<byte>			_Buffer;

public:
	CEgSocketBuffer() {}

	CEgSocketBuffer(const void* pData, unsigned long nSize)
	{
		if (pData == NULL)
			throw ERROR_INVALID_DATA;

		if (nSize > 0)
			Insert(pData, nSize);
	};

	inline void * GetData() const {return (void*)&(*m_Buffer.begin());}
	inline DWORD  GetLength() const {return m_Buffer.size();}
	
	void ShiftLeft(const unsigned long nDelta)
	{
		if (m_Buffer.size() >= nDelta)
		{
			m_Buffer.erase(m_Buffer.begin(),m_Buffer.begin() + nDelta);
		}
	}

	void Insert(const void* pData, unsigned long nSize)
	{
		if(nSize > m_Buffer.capacity() - m_Buffer.size())
		{
			m_Buffer.reserve(((nSize % ALLOCATION_GRANULARITY) + 1) * ALLOCATION_GRANULARITY);
		}
		m_Buffer.insert(m_Buffer.end(),
						(byte*)pData, (byte*)pData + nSize);
	}
	void Clear()
	{
		m_Buffer.clear();
	}

private:

	_Buffer		m_Buffer;
};


#endif //__MESSAGE_PARESER_H__