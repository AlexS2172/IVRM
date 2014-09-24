#ifndef __MSG_PACKING_H__
#define __MSG_PACKING_H__

/*****************************************************************************************************
*****************************************************************************************************/

#include <windows.h>
#include <comutil.h>

#include <vector>
#include <string>
#include <map>

using namespace std;

_COM_SMARTPTR_TYPEDEF(IDynamicMessage, __uuidof(IDynamicMessage));

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

	const void* GetData(unsigned& nSize) const 
	{
		nSize = sizeof(T);
		return &m_T; 
	}

	unsigned SetData(const void* pData, unsigned nSize)
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
typedef TStaticField<VARIANT_BOOL>		BoolF;
typedef TStaticField<__int64>			LongLongF;

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

	const void* GetData(unsigned& nSize) const 
	{
		nSize = m_Data.size() + 1;
		return m_Data.c_str(); 
	}

	unsigned SetData(const void* pData, unsigned nSize)
	{
		unsigned nLen = 1L;
		
		if(pData)
			nLen = strlen((const char*)pData) + 1L;

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

class WStringF
{
public:
	WStringF(const wchar_t* szStr = L"") { m_Data = szStr; }
	WStringF(const WStringF& r) { m_Data = r.m_Data; }
	
	const wchar_t* GetStr() { return m_Data.c_str(); }

	const WStringF& operator =(const wchar_t* szStr) 
	{ 
		m_Data = szStr; 
		return *this; 
	}
	const WStringF& operator =(const WStringF& r) 
	{ 
		m_Data = r.m_Data; 
		return *this; 
	}

    bool operator < (const WStringF& Compare) const
    {
        return m_Data.compare(Compare.m_Data) < 0;
    }

	const void* GetData(unsigned& nSize) const 
	{
		nSize = (m_Data.size() + 1) * sizeof(wchar_t);
		return m_Data.c_str(); 
	}

	unsigned SetData(const void* pData, unsigned nSize)
	{
		unsigned nLen = (wcslen((const wchar_t*)pData) + 1) * sizeof(wchar_t);

		if (nLen > nSize)
			return 0;

		m_Data = (const wchar_t*)pData;
		
		return nLen;
	}

private:
	wstring	m_Data;
};

/*****************************************************************************************************
*****************************************************************************************************/

template <class K, class V> class MapF;
class VariantF;

class BlobF
{
	typedef vector<byte>	_Data;

public:
	BlobF() : m_nOffset(0) {}
    BlobF(const byte* pData, size_t nSize) : m_Data(pData, pData + nSize), m_nOffset(0) { }
	BlobF(const BlobF& r) : m_Data(r.m_Data), m_nOffset(0) {}
    BlobF(const BSTR bs) : m_Data((byte*)bs, (byte*)bs + ::SysStringByteLen(bs)), m_nOffset(0) { }
	
	const BlobF& operator =(const BlobF& r) 
	{ 
		m_Data = r.m_Data;
		return *this; 
	}

    operator BSTR() const
    {
        return ::SysAllocStringByteLen((LPCSTR)&m_Data.front(), m_Data.size());
    }

	const void* GetData(unsigned& nSize) const 
	{
		nSize = m_Data.size();
		return &m_Data.front(); 
	}

	unsigned SetData(const void* pData, unsigned nSize)
	{
        m_Data.clear();
		m_Data.insert(m_Data.end(), (byte*)pData, (byte*)pData + nSize);
        m_nOffset = 0;
		
		return m_Data.size();
	}

    template <class T> 
    BlobF& operator << (T& rT)
	{ 
		unsigned nSize;
		const void* pData = rT.GetData(nSize);
        if (nSize)
        {
            AddData((const void*)&nSize, sizeof(nSize));
            AddData(pData, nSize);
        }

		return *this;
	}

    template <class T> 
    BlobF& operator >> (T& rT) throw(...)
	{ 
		if (Eof())
			throw ERROR_HANDLE_EOF;

		unsigned nSize = *(unsigned*)(&m_Data.front() + m_nOffset);	
        
        if (m_nOffset + nSize + sizeof(nSize) > m_Data.size() )
			throw ERROR_HANDLE_EOF;

		rT.SetData(&m_Data.front() + m_nOffset + sizeof(nSize), nSize);

        m_nOffset += nSize + sizeof(nSize);

		return *this;
	}

	bool Eof() const
	{ 
		return m_nOffset == m_Data.size(); 
	}

protected:
	unsigned AddData(const void* pData, unsigned nSize)
	{
		m_Data.insert(m_Data.end(), (byte*)pData, (byte*)pData + nSize);
		
		return m_Data.size();
	}

	_Data		m_Data;
	unsigned	m_nOffset;

    friend class VariantF;
};

/*****************************************************************************************************
*****************************************************************************************************/

class VariantF : public BlobF
{
    typedef variant_t  _Data;

public:
    VariantF() { }
    VariantF(VARIANT* var) { m_Data.Attach(*var); }
    VariantF(const _Data var) { m_Data = var; }
	VariantF(const VariantF& r) { m_Data = r.m_Data; }
	
	const VariantF& operator =(const _Data var) 
	{ 
		m_Data = var; 
		return *this; 
	}
	const VariantF& operator =(const VariantF& var) 
	{ 
		m_Data = var.m_Data;
		return *this;
	}

    operator VARIANT() const
    {
        _Data d = m_Data;
        return d.Detach();
    }

	const void* GetData(unsigned& nSize) throw()
	{
        const void* pData = 0;
        nSize = 0;
        Convert(m_Data, pData, nSize, true);
		return pData; 
	}

	unsigned SetData(const void* pData, unsigned nSize) throw()
	{
        Convert(m_Data, pData, nSize, false);
		return nSize;
	}

private:
    template <class T> void GetSetStaticValue(T& Value, bool fGet) throw()
    {
        TStaticField<T>     _Value;
        if (fGet)
        {
            _Value = Value;
            *this << _Value;
        }
        else
        {
            *this >> _Value;
            Value = _Value;
        }
    }

    void GetSetWStringValue(BSTR& Value, bool fGet) throw()
    {
        WStringF     _Value;
        if (fGet)
        {
            BlobF blobValue;
            blobValue.SetData(Value, ::SysStringByteLen(Value));
            *this << blobValue;
        }
        else
        {
            BlobF blobValue;
            *this >> blobValue;
            UINT nLen = 0;
            const void* pData = blobValue.GetData(nLen);
            Value = ::SysAllocStringByteLen((const char*)pData, nLen);
        }
    }

    template <class T> void GetSetReferenceValue(T*& Value, bool fGet) throw(...)
    {
        CLSID           clsid = CLSID_NULL;
        CComBSTR        bsValue;
        HRESULT         hr = S_OK;;

        IDynamicMessagePtr spDynamicMessage;
        INetPackingPtr  ptr;
        IPersistPtr     spPersist;

        if (fGet)
        {
            spPersist = Value;
            if (spPersist == 0)
                throw E_NOINTERFACE;

            if (FAILED( hr = spPersist->GetClassID(&clsid) ))
                throw hr;

            if (FAILED( hr = spDynamicMessage.CreateInstance(__uuidof(DynamicMessage)) ))
                throw hr;

            LPOLESTR strClsid = 0;
            StringFromCLSID(clsid, &strClsid);
            if (FAILED( hr = spDynamicMessage->put_Value(L"ObjectID", _variant_t(strClsid)) ))
                throw hr;
            
            ::CoTaskMemFree(strClsid);

            ptr = Value;
            if (ptr == 0)
                throw E_NOINTERFACE;
            if (FAILED(hr = ptr->Pack(&bsValue)))
                throw hr;

            _bstr_t bsVal((BSTR)bsValue, true); // be careful to call SysAllocStringByteLen
//            _bstr_t bsVal(SysAllocStringByteLen("d\0\0\0s\0", 6), false); // be careful to call SysAllocStringByteLen
            if (FAILED( hr = spDynamicMessage->put_Value(L"Object", _variant_t(bsVal)) ))
                throw hr;
                        
            ptr = spDynamicMessage;
            bsValue.Empty();
            if (ptr == 0)
                throw E_NOINTERFACE;
            if (FAILED(hr = ptr->Pack(&bsValue)))
                throw hr;

            *this << BlobF(bsValue);
        }
        else
        {
            BlobF blobValue;
            *this >> blobValue;
            bsValue.Attach(blobValue);

            if (FAILED( hr = spDynamicMessage.CreateInstance(__uuidof(DynamicMessage)) ))
                throw hr;

            ptr = spDynamicMessage;
            if (ptr == 0)
                throw E_NOINTERFACE;
            if (FAILED(hr = ptr->Unpack(bsValue) ))
                throw hr;

            _variant_t      vTmp;
            if (FAILED( hr = spDynamicMessage->get_Value(L"ObjectID", &vTmp) ))
                throw hr;

            if ( FAILED(hr = CLSIDFromString((_bstr_t)vTmp, &clsid) ))
                throw hr;

            vTmp.Clear();
            if (FAILED( hr = spDynamicMessage->get_Value(L"Object", &vTmp) ))
                throw hr;

            if (FAILED( hr = ptr.CreateInstance(clsid) ))
                throw hr;

            _bstr_t bsVal(V_BSTR(&vTmp), true); // be careful to call SysAllocStringByteLen
            if (FAILED(hr = ptr->Unpack(bsVal) ))
                throw hr;

            if (FAILED( hr = ptr.QueryInterface(__uuidof(Value), &Value) ))
                throw hr;
        }
    }

#define GETSET_BEGIN(direction, v, data, size) \
    bool    _get = direction;               \
    unsigned    _nSize = 0;                 \
    const void* _pData = 0;                 \
    VARIANT     _var = v;                   \
    if (_get)                               \
    {                                       \
        data = 0;                           \
        size = 0;                           \
    }                                       \
    BlobF::SetData(data, size);             \

#define GETSET_END(v, data, size)           \
    if (_get)                               \
    {                                       \
        data = BlobF::GetData(size);        \
    }                                       \
    else                                    \
    {                                       \
        v.Attach(_var);                     \
        size = m_nOffset;                   \
    }                                       \

#define GETSET_STATIC_VALUE(Type)           \
    GetSetStaticValue(V_##Type(&_var), _get);\

#define GETSET_WSTRING_VALUE()              \
    GetSetWStringValue(V_BSTR(&_var), _get);\

#define GETSET_REFERENCE_VALUE(Type)        \
    GetSetReferenceValue(V_##Type(&_var), _get);\

#define VALUE(Type)                         \
    V_##Type(&_var)                          \

#define STATIC_CASE(Type)                   \
    case VT_##Type:                         \
        GETSET_STATIC_VALUE(Type)           \
        break;                              \

#define REFERENCE_CASE(Type)                \
    case VT_##Type:                         \
        GETSET_REFERENCE_VALUE(Type)        \
        break;                              \

    void Convert(_Data& var, const void*& pData, unsigned& nSize, bool fGet) throw(...)
    {
        GETSET_BEGIN(fGet, var, pData, nSize)
        GETSET_STATIC_VALUE(VT)

        switch (VALUE(VT))
        {
        case VT_EMPTY:
        case VT_NULL:
            break;
        STATIC_CASE(I2)
        STATIC_CASE(I4)
        STATIC_CASE(R4)
        STATIC_CASE(R8)
        STATIC_CASE(CY)
        STATIC_CASE(DATE)
        case VT_BSTR:
            GETSET_WSTRING_VALUE()
            break;
        REFERENCE_CASE(DISPATCH) 
#undef ERROR // defined in WinGDI.h
        STATIC_CASE(ERROR)
        STATIC_CASE(BOOL)
//        case VT_VARIANT:
        REFERENCE_CASE(UNKNOWN)
        STATIC_CASE(DECIMAL)
        STATIC_CASE(I1)
        STATIC_CASE(UI1)
        STATIC_CASE(UI2)
        STATIC_CASE(UI4)
        STATIC_CASE(I8)
        STATIC_CASE(UI8)
        STATIC_CASE(INT)
        STATIC_CASE(UINT)
//        case VT_VOID:
//        case VT_HRESULT:
//        case VT_PTR:
//        case VT_SAFEARRAY:
//        case VT_CARRAY:
//        case VT_USERDEFINED:
//        case VT_LPSTR:
//        case VT_LPWSTR:
//        case VT_RECORD:
//        case VT_FILETIME:
//        case VT_BLOB:
//        case VT_STREAM:
//        case VT_STORAGE:
//        case VT_STREAMED_OBJECT:
//        case VT_STORED_OBJECT:
//        case VT_BLOB_OBJECT:
//        case VT_CF:
//        case VT_CLSID:
//        case VT_VERSIONED_STREAM:
//        case VT_BSTR_BLOB:
//        case VT_VECTOR:
//        case VT_ARRAY:
//        case VT_BYREF:
//        case VT_RESERVED:
//        case VT_ILLEGAL:
//        case VT_ILLEGALMASKED:
//        case VT_TYPEMASK:

        default:
            throw DISP_E_BADVARTYPE;
        }

        GETSET_END(var, pData, nSize)
    }

private:

	_Data	m_Data;
};

/*****************************************************************************************************
*****************************************************************************************************/

template <class K, class V>
class MapF : public BlobF
{
	typedef map<K, V>	_Data;
    typedef typename _Data::iterator		_iterator;
    typedef typename _Data::const_iterator  _const_iterator;

public:
    MapF() {}
	MapF(const MapF& r) : m_Data(r.m_Data) {}
	
	const MapF& operator =(const MapF& r) 
	{ 
		m_Data = r.m_Data;
		return *this;
	}

    V& operator [] (const K& Key)
    {
        return m_Data[Key];
    }

    bool find (const K& Key)
    {
        return m_Data.find(Key) != m_Data.end();
    }

	const void* GetData(unsigned& nSize)
	{
        _iterator   it = m_Data.begin();

        const void* _pData = 0;
        unsigned    _nData = 0;

        BlobF::SetData(0, 0);

        while (it != m_Data.end())
        {
            *this << it->first << it->second;
            it++;
        }

		return BlobF::GetData(nSize); 
	}

	unsigned SetData(const void* pData, unsigned nSize)
	{
        m_Data.clear();

        BlobF::SetData(pData, nSize);

        while (!BlobF::Eof())
        {
            // check for last null
            if (BlobF::m_nOffset == (BlobF::m_Data.size() - 1) &&
                BlobF::m_Data[BlobF::m_nOffset] == 0)
            {
                m_nOffset = BlobF::m_Data.size();
                continue;
            }
            
            K first; V second;
            *this >> first >> second;
            m_Data[first] = second;
        }

		return nSize;
	}

private:
	_Data		m_Data;
};

/*****************************************************************************************************
*****************************************************************************************************/

class CPacket
{
	typedef vector<byte>			_Buffer;
	typedef _Buffer::iterator		_Cursor;

public:
	CPacket() : m_nOffset(0) {}

	CPacket(const void* pData, unsigned nSize) : m_nOffset(0)
	{
		if (pData == NULL)
			throw ERROR_INVALID_DATA;

		if (nSize > 0)
			_Insert(pData, nSize);
	};

	template <class T>
	CPacket& operator << (T& rT) 
	{ 
		unsigned nSize;
		const void* pData = rT.GetData(nSize);

		if (nSize > 0)
			_Insert(pData, nSize);

		return *this;
	}

	template <class T>
	CPacket& operator >> (T& rT) 
	{ 
		if (Eof())
			throw ERROR_INVALID_ACCESS;

		unsigned nSize;
		
		nSize = rT.SetData(&m_Buffer.front() + m_nOffset, 
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

	void* GetBuffer(unsigned& nSize)
	{
		nSize = m_Buffer.size();
		return &m_Buffer.front();
	}

private:
	void _Insert(const void* pData, unsigned nSize)
	{
		m_Buffer.insert(m_Buffer.end(),
						(byte*)pData, (byte*)pData + nSize);
	}

	_Buffer		m_Buffer;
	unsigned	m_nOffset;
};

#define BEGIN_PACKING				bool Packing(CPacket& rPacket, bool bUnpack = false) { try {
#define FIELD_ENTRY(Field)			bUnpack ? rPacket >> Field : rPacket << Field;
#define END_PACKING					} catch(...) { return false; } return true; } 

struct TestMsg
{
	TestMsg() {}

	BEGIN_PACKING
	FIELD_ENTRY(m_Int)
	FIELD_ENTRY(m_Double)
	FIELD_ENTRY(m_String)
	FIELD_ENTRY(m_Float)
	FIELD_ENTRY(m_Char)
	END_PACKING

	IntF		m_Int;
	DoubleF		m_Double;
	StringF		m_String;
	FloatF		m_Float;
	CharF		m_Char;
};

#endif //__MSG_PACKING_H__