// MmRvPrice.h : Declaration of the CMmRvPrice

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmRisks.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

_COM_SMARTPTR_TYPEDEF(IMmRvPrice, IID_IMmRvPrice);

#define IMPLEMENT_SIMPLE_WITHCHECK_PROPERTY(Type, Name, Member)	\
	\
	STDMETHODIMP put_##Name(Type NewVal)							\
{																	\
	ObjectLock lock(this);											\
	if(Member != NewVal)											\
	m_bDirty = VARIANT_TRUE;										\
	Member = NewVal;												\
	return S_OK;													\
}																	\
	\
	STDMETHODIMP get_##Name(Type* pVal)								\
{																	\
	if(!pVal)														\
{																\
	return E_POINTER;												\
}																\
	\
	ObjectLock lock(this);											\
	*pVal = Member;													\
	return S_OK;													\
}													


// CMmRvPrice
class __CMmRvPrice
{
public:
	DOUBLE							m_dPriceBid;
	DOUBLE							m_dPriceAsk;
	DOUBLE							m_dPriceLast;
	DOUBLE							m_dPriceClose;
	DOUBLE							m_dPriceTheoClose;
	DOUBLE							m_dNetChange;
	DOUBLE							m_dActivePrice;
	VARIANT_BOOL					m_bDirty;
	VARIANT_BOOL					m_bManualActive;


	__CMmRvPrice()
	{
		Clear();
	}

	void Clear()
	{

		m_dPriceBid = BAD_DOUBLE_VALUE;
		m_dPriceAsk = BAD_DOUBLE_VALUE;
		m_dPriceLast = BAD_DOUBLE_VALUE;
		m_dPriceClose = BAD_DOUBLE_VALUE;
		m_dNetChange = BAD_DOUBLE_VALUE;
		m_dActivePrice = BAD_DOUBLE_VALUE;
		m_dPriceTheoClose = BAD_DOUBLE_VALUE;

		m_bDirty = VARIANT_TRUE;
		m_bManualActive = VARIANT_FALSE;
	}
};

class ATL_NO_VTABLE CMmRvPrice :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmRvPrice, &CLSID_MmRvPrice>,
	public ISupportErrorInfoImpl<&IID_IMmRvPrice>,
	public IDispatchImpl<IMmRvPrice, &IID_IMmRvPrice, &LIBID_EtsMmRisksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __CMmRvPrice
{
public:
	CMmRvPrice()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRVPRICE)


BEGIN_COM_MAP(CMmRvPrice)
	COM_INTERFACE_ENTRY(IMmRvPrice)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

		IMPLEMENT_SIMPLE_WITHCHECK_PROPERTY(DOUBLE, Bid,	 m_dPriceBid)
		IMPLEMENT_SIMPLE_WITHCHECK_PROPERTY(DOUBLE, Ask,	 m_dPriceAsk)
		IMPLEMENT_SIMPLE_WITHCHECK_PROPERTY(DOUBLE, Last,	 m_dPriceLast)
		IMPLEMENT_SIMPLE_WITHCHECK_PROPERTY(DOUBLE, Close,	 m_dPriceClose)
		IMPLEMENT_SIMPLE_WITHCHECK_PROPERTY(DOUBLE, Active,    m_dActivePrice)
		IMPLEMENT_SIMPLE_WITHCHECK_PROPERTY(DOUBLE, TheoClose,	 m_dPriceTheoClose)

		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		NetChange, m_dNetChange)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsDirty, m_bDirty)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsUseManualActive, m_bManualActive)

};

OBJECT_ENTRY_AUTO(__uuidof(MmRvPrice), CMmRvPrice)
