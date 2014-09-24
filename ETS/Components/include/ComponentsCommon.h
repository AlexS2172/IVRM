#ifndef __COMPONENTSCOMMON_H__
#define __COMPONENTSCOMMON_H__
#pragma once

#include <float.h>
#include <math.h>
#include <atlSafe.h>
/////////////////////////////////////////////////////////////////////////////
//
#define ETS_DEF_VOLA		0.
#define BAD_DOUBLE_VALUE	-1E+308
#define BAD_LONG_VALUE		-2147483647

/////////////////////////////////////////////////////////////////////////////
//
#define IMPLEMENT_SIMPLE_PROPERTY(Type, Name, Member)				\
																	\
STDMETHODIMP put_##Name(Type NewVal)								\
{																	\
	ObjectLock lock(this);											\
	Member = NewVal;												\
	return S_OK;													\
}																	\
																	\
STDMETHODIMP get_##Name(Type* pVal)									\
{																	\
	if(!pVal)														\
	{																\
		return E_POINTER;											\
	}																\
																	\
	ObjectLock lock(this);											\
	*pVal = Member;													\
	return S_OK;													\
}													

/////////////////////////////////////////////////////////////////////////////
//
#define IMPLEMENT_SIMPLEREADONLY_PROPERTY(Type, Name, Member)		\
																	\
STDMETHODIMP get_##Name(Type* pVal)									\
{																	\
	if(!pVal)														\
	{																\
		return E_POINTER;											\
	}																\
																	\
	ObjectLock lock(this);											\
	*pVal = Member;													\
	return S_OK;													\
}													

/////////////////////////////////////////////////////////////////////////////
//
#define IMPLEMENT_BSTR_PROPERTY(Name, Member)						\
																	\
STDMETHODIMP put_##Name(BSTR NewVal)								\
{																	\
	ObjectLock lock(this);											\
	Member = NewVal;												\
	return S_OK;													\
}																	\
																	\
STDMETHODIMP get_##Name(BSTR* pVal)									\
{																	\
	if(!pVal)														\
	{																\
		return E_POINTER;											\
	}																\
																	\
	if(*pVal)														\
	{																\
		::SysFreeString(*pVal);										\
	}																\
																	\
	ObjectLock lock(this);											\
	*pVal = Member.Copy();											\
	return S_OK;													\
}

/////////////////////////////////////////////////////////////////////////////
//
#define IMPLEMENT_VARIANT_PROPERTY(Name, Member)					\
	\
	STDMETHODIMP put_##Name(VARIANT NewVal)							\
{																	\
	ObjectLock lock(this);											\
	Member = NewVal;												\
	return S_OK;													\
}																	\
	\
	STDMETHODIMP get_##Name(VARIANT* pVal)							\
{																	\
	if(!pVal)														\
	{																\
	return E_POINTER;												\
	}																\
	\
	ObjectLock lock(this);											\
	VariantCopy(pVal, Member.GetAddress());							\
	return S_OK;													\
}
/////////////////////////////////////////////////////////////////////////////
//
#define IMPLEMENT_BSTRT_PROPERTY(Name, Member)						\
	\
	STDMETHODIMP put_##Name(BSTR NewVal)								\
{																	\
	ObjectLock lock(this);											\
	Member = NewVal;												\
	return S_OK;													\
}																	\
	\
	STDMETHODIMP get_##Name(BSTR* pVal)									\
{																	\
	if(!pVal)														\
	{																\
	return E_POINTER;											\
	}																\
	\
	if(*pVal)														\
	{																\
	::SysFreeString(*pVal);										\
	}																\
	\
	ObjectLock lock(this);											\
	*pVal = Member.copy();											\
	return S_OK;													\
}

/////////////////////////////////////////////////////////////////////////////
//
#define IMPLEMENT_BSTRREADONLY_PROPERTY(Name, Member)				\
																	\
STDMETHODIMP get_##Name(BSTR* pVal)									\
{																	\
	if(!pVal)														\
	{																\
		return E_POINTER;											\
	}																\
																	\
	if(*pVal)														\
	{																\
		::SysFreeString(*pVal);										\
	}																\
																	\
	ObjectLock lock(this);											\
	*pVal = Member.Copy();											\
	return S_OK;													\
}

/////////////////////////////////////////////////////////////////////////////
//
#define IMPLEMENT_BSTRTREADONLY_PROPERTY(Name, Member)				\
	\
	STDMETHODIMP get_##Name(BSTR* pVal)									\
{																	\
	if(!pVal)														\
	{																\
	return E_POINTER;											\
	}																\
	\
	if(*pVal)														\
	{																\
	::SysFreeString(*pVal);										\
	}																\
	\
	ObjectLock lock(this);											\
	*pVal = Member.copy();											\
	return S_OK;													\
}

/////////////////////////////////////////////////////////////////////////////
//
#define IMPLEMENT_OBJECT_PROPERTY(Type, Name, Member)				\
																	\
STDMETHODIMP putref_##Name(Type NewVal)								\
{																	\
	ObjectLock lock(this);											\
	Member = NewVal;												\
	return S_OK;													\
}																	\
																	\
STDMETHODIMP get_##Name(Type* pVal)									\
{																	\
	if(!pVal)														\
	{																\
		return E_POINTER;											\
	}																\
																	\
	if(*pVal)														\
	{																\
		(*pVal)->Release();											\
		*pVal = NULL;												\
	}																\
																	\
	ObjectLock lock(this);											\
	*pVal = (Type)Member;											\
	if(*pVal)														\
	{																\
		(*pVal)->AddRef();											\
	}																\
	return S_OK;													\
}													

/////////////////////////////////////////////////////////////////////////////
//
#define IMPLEMENT_OBJECTREADONLY_PROPERTY(Type, Name, Member)		\
																	\
STDMETHODIMP get_##Name(Type* pVal)									\
{																	\
	if(!pVal)														\
	{																\
		return E_POINTER;											\
	}																\
																	\
	if(*pVal)														\
	{																\
		(*pVal)->Release();											\
		*pVal = NULL;												\
	}																\
																	\
	ObjectLock lock(this);											\
	*pVal = (Type)Member;											\
	if(*pVal)														\
	{																\
		(*pVal)->AddRef();											\
	}																\
	return S_OK;													\
}													

/////////////////////////////////////////////////////////////////////////////
//
static __inline BOOL IsBadValue(LONG nValue)
{
	return (BAD_LONG_VALUE == nValue);
}

/////////////////////////////////////////////////////////////////////////////
//
static __inline BOOL IsBadValue(DOUBLE dValue)
{
	return !(_finite(dValue) && dValue > BAD_DOUBLE_VALUE);
}

/////////////////////////////////////////////////////////////////////////////
//
static __inline DOUBLE PriceMidEx(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast, VARIANT_BOOL bUseLast)
{
	if(dPriceBid > 0. && dPriceAsk > 0.)
	{
		return (dPriceBid + dPriceAsk) / 2.;
	}

	if(VARIANT_FALSE != bUseLast && dPriceLast > 0.)
	{
		return dPriceLast;
	}

	if(dPriceBid > 0. || dPriceAsk > 0.)
	{
		if(dPriceBid > 0. && dPriceAsk <= 0.)
		{
			if(bUseLast)
			{
				return dPriceBid;
			}

			return dPriceBid / 2.;
		}

		if(bUseLast)
		{
			return dPriceAsk;
		}
		return dPriceAsk / 2.;
	}

	return BAD_DOUBLE_VALUE;
}

/////////////////////////////////////////////////////////////////////////////
// check for dVal == 0.
static __inline bool DoubleEQZero(double dVal)
{
	return fabs(dVal) < DBL_EPSILON;		
}

/////////////////////////////////////////////////////////////////////////////
// check for dVal != 0.
static __inline bool DoubleNEQZero(double dVal)
{
	return fabs(dVal) > DBL_EPSILON;		
}

/////////////////////////////////////////////////////////////////////////////
// check for dVal > 0.
static __inline bool DoubleGTZero(double dVal)
{
	return dVal > DBL_EPSILON;		
}

/////////////////////////////////////////////////////////////////////////////
// check for dVal >= 0.
static __inline bool DoubleGEQZero(double dVal)
{
	return dVal > -DBL_EPSILON;		
}

/////////////////////////////////////////////////////////////////////////////
// check for dVal < 0.
static __inline bool DoubleLSZero(double dVal)
{
	return dVal < -DBL_EPSILON;		
}

/////////////////////////////////////////////////////////////////////////////
// check for dVal <= 0.
static __inline bool DoubleLEQZero(double dVal)
{
	return dVal < DBL_EPSILON;		
}

// Check result and throw exception if error
__inline void _CHK(HRESULT hr, LPCTSTR szDesc = NULL)
{
	if(FAILED(hr))
		EgLib::CComErrorWrapper::ThrowError(hr, szDesc ? szDesc : _T(""));
}

//---------------------------------------------------------------------------------------------------------
template< typename T>
class CSafeArrayWrapper
{
public:
	CSafeArrayWrapper()
		:m_pData(NULL)
		,m_pAccessData(NULL)
		,m_bIsLocked(false)
	{
	}
	~CSafeArrayWrapper()
	{
		Destroy();
	}

	void Attach(LPSAFEARRAY pData)
	{
		if(m_pData != pData)
		{
			Destroy();
			m_pData = pData;
		}
	}
	LPSAFEARRAY Detach()
	{
		if(m_pData)
		{
			LPSAFEARRAY pRetData = m_pData;

			if(m_pAccessData)
			{
				::SafeArrayUnaccessData(m_pData);
				m_pAccessData = NULL;
			}
			if(m_bIsLocked)
			{
				::SafeArrayUnlock(m_pData);
				m_bIsLocked = false;
			}
			m_pData = NULL;
			return pRetData;

		}
		return NULL;
	}
	void Lock()
	{
		if(!m_bIsLocked && m_pData)
		{
			::SafeArrayLock(m_pData);
			m_bIsLocked = true;
		}
	}
	T* GetPlainData()
	{
		if(m_pAccessData)
			return reinterpret_cast<T*>(m_pAccessData);
		if(m_pData)
		{
			if(!m_bIsLocked)
				Lock();
			::SafeArrayAccessData(m_pData, &m_pAccessData);
			return reinterpret_cast<T*>(m_pAccessData);
		}
		return NULL;
	}

private:
	void Destroy()
	{
		if(m_pData)
		{
			if(m_pAccessData)
			{
				::SafeArrayUnaccessData(m_pData);
				m_pAccessData = NULL;
			}
			if(m_bIsLocked)
			{
				::SafeArrayUnlock(m_pData);
				m_bIsLocked = false;
			}
			::SafeArrayDestroy(m_pData);
			m_pData = NULL;
		}
	}

	LPSAFEARRAY  m_pData;
	LPVOID       m_pAccessData;
	bool         m_bIsLocked;
};


#endif //__COMPONENTSCOMMON_H__
