#ifndef __MSG_PROPS_IMPL_H__
#define __MSG_PROPS_IMPL_H__

#define IMPLEMENT_VARCHAR_PROP( Name, Member )						\
																	\
STDMETHODIMP put_##Name(BSTR NewVal)								\
{																	\
	ObjectLock lock(this);											\
																	\
	USES_CONVERSION;												\
																	\
	long lLen = min(sizeof(Member) - 1, ::SysStringLen(NewVal));	\
	/*::strncpy(Member, W2A(NewVal), lLen);							\
	Member[lLen] = 0;*/												\
	Member = W2A(NewVal);											\
																	\
	return S_OK;													\
}																	\
																	\
STDMETHODIMP get_##Name(BSTR* pRetVal)								\
{																	\
	if (!pRetVal)													\
		return E_POINTER;											\
																	\
	if (*pRetVal)													\
		::SysFreeString(*pRetVal);									\
																	\
	ObjectLock lock(this);											\
																	\
	USES_CONVERSION;												\
																	\
	*pRetVal = ::SysAllocStringLen(A2W(Member.GetStr()),			\
								 ::lstrlenA(Member.GetStr()));		\
																	\
	return S_OK;													\
}

#define IMPLEMENT_LONG_PROP( Name, Member )			\
													\
STDMETHODIMP put_##Name(long NewVal)				\
{													\
	ObjectLock lock(this);							\
													\
	Member = NewVal;								\
													\
	return S_OK;									\
}													\
													\
STDMETHODIMP get_##Name(long* pRetVal)				\
{													\
	if (!pRetVal)									\
		return E_POINTER;							\
													\
	ObjectLock lock(this);							\
													\
	*pRetVal = Member;								\
													\
	return S_OK;									\
}													

#define IMPLEMENT_DATE_PROP( Name, Member )			\
													\
STDMETHODIMP put_##Name(DATE NewVal)				\
{													\
	ObjectLock lock(this);							\
													\
	Member = NewVal;								\
													\
	return S_OK;									\
}													\
													\
STDMETHODIMP get_##Name(DATE* pRetVal)				\
{													\
	if (!pRetVal)									\
		return E_POINTER;							\
													\
	ObjectLock lock(this);							\
													\
	*pRetVal = Member;								\
													\
	return S_OK;									\
}													

#define IMPLEMENT_DOUBLE_PROP( Name, Member )		\
													\
STDMETHODIMP put_##Name(double NewVal)				\
{													\
	ObjectLock lock(this);							\
													\
	Member = NewVal;								\
													\
	return S_OK;									\
}													\
													\
STDMETHODIMP get_##Name(double* pRetVal)			\
{													\
	if (!pRetVal)									\
		return E_POINTER;							\
													\
	ObjectLock lock(this);							\
													\
	*pRetVal = Member;								\
													\
	return S_OK;									\
}													

#define IMPLEMENT_CURRENCY_PROP( Name, Member )		\
													\
STDMETHODIMP put_##Name(CURRENCY NewVal)			\
{													\
	ObjectLock lock(this);							\
													\
	Member = NewVal;								\
													\
	return S_OK;									\
}													\
													\
STDMETHODIMP get_##Name(CURRENCY* pRetVal)			\
{													\
	if (!pRetVal)									\
		return E_POINTER;							\
													\
	ObjectLock lock(this);							\
													\
	*pRetVal = Member;								\
													\
	return S_OK;									\
}													

#define IMPLEMENT_MAP_PROP( Name, Type, ArgType )	\
													\
STDMETHODIMP put_##Name(BSTR PropName, ArgType NewVal)	\
{													\
	ObjectLock lock(this);							\
													\
    try                                             \
    {                                               \
        Type d = NewVal;                            \
    	m_##Name[PropName] = d;                     \
    }                                               \
    catch (_com_error& e)                           \
    {                                               \
        return e.Error();                           \
    }                                               \
													\
	return S_OK;									\
}													\
													\
STDMETHODIMP get_##Name(BSTR PropName, ArgType* pRetVal)\
{													\
	if (!pRetVal)									\
		return E_POINTER;							\
													\
	ObjectLock lock(this);							\
													\
    if (!pRetVal || !m_##Name.find(PropName))       \
        return E_INVALIDARG;                        \
                                                    \
    try                                             \
    {                                               \
        Type d = pRetVal;                           \
        d = m_##Name[PropName];                     \
        *pRetVal = d;                               \
    }                                               \
    catch (_com_error& e)                           \
    {                                               \
        return e.Error();                           \
    }                                               \
													\
	return S_OK;									\
}													

#define IMPLEMENT_SIMPLE_PROP(Type, Name, Member)	\
													\
STDMETHODIMP put_##Name(Type NewVal)	    		\
{													\
	ObjectLock lock(this);							\
													\
	Member = NewVal;                				\
													\
	return S_OK;									\
}													\
													\
STDMETHODIMP get_##Name(Type* pRetVal)				\
{													\
	if (!pRetVal)									\
		return E_POINTER;							\
													\
	ObjectLock lock(this);							\
													\
	*pRetVal = Member;                              \
													\
	return S_OK;									\
}													

#define IMPLEMENT_VARIANT_PROP(Name, Member)	    \
													\
STDMETHODIMP put_##Name(VARIANT NewVal)	    		\
{													\
	ObjectLock lock(this);							\
													\
	Member = NewVal;                				\
													\
	return S_OK;									\
}													\
													\
STDMETHODIMP get_##Name(VARIANT* pRetVal)			\
{													\
	if (!pRetVal)									\
		return E_POINTER;							\
													\
	ObjectLock lock(this);							\
    VariantClear(pRetVal);                          \
                                                    \
    return ::VariantCopy(pRetVal, &(VARIANT)Member);\
}													

#define IMPLEMENT_OTHERTYPE_PROP(Type, Name, SimpleMemberType, Member)	\
													\
STDMETHODIMP put_##Name(Type NewVal)			\
{													\
	ObjectLock lock(this);							\
													\
	Member = (SimpleMemberType)NewVal;				\
													\
	return S_OK;									\
}													\
													\
STDMETHODIMP get_##Name(Type* pRetVal)				\
{													\
	if (!pRetVal)									\
		return E_POINTER;							\
													\
	ObjectLock lock(this);							\
													\
	*pRetVal = (Type)(SimpleMemberType)Member;      \
													\
	return S_OK;									\
}													

#define IMPLEMENT_BOOL_PROP( Name, Member )			\
													\
STDMETHODIMP put_##Name(VARIANT_BOOL NewVal)		\
{													\
	ObjectLock lock(this);							\
													\
	Member = NewVal;								\
													\
	return S_OK;									\
}													\
													\
STDMETHODIMP get_##Name(VARIANT_BOOL* pRetVal)		\
{													\
	if (!pRetVal)									\
		return E_POINTER;							\
													\
	ObjectLock lock(this);							\
													\
	*pRetVal = Member;								\
													\
	return S_OK;									\
}													

#define IMPLEMENT_LONGLONG_PROP( Name, Member )		\
													\
STDMETHODIMP put_##Name(__int64 NewVal)				\
{													\
	ObjectLock lock(this);							\
													\
	Member = NewVal;								\
													\
	return S_OK;									\
}													\
													\
STDMETHODIMP get_##Name(__int64* pRetVal)			\
{													\
	if (!pRetVal)									\
		return E_POINTER;							\
													\
	ObjectLock lock(this);							\
													\
	*pRetVal = Member;								\
													\
	return S_OK;									\
}													

#endif //__MSG_PROPS_IMPL_H__
