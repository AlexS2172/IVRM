// ActiveFuturesChange.h : Declaration of the CActiveFuturesChange

#pragma once
#include "resource.h"       // main symbols
#include "MsgPropImpl.h"

#include "MsgStruct.h"
#include "MsgPropImpl.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


struct __ActiveFuturesChange
{
public:

	LongF		m_nUndID;
	LongF		m_nFuturesID;
	BoolF       m_bIsActiveFutures;  
	DoubleF     m_dBasis;
	DoubleF     m_dRatio;

	BEGIN_PACKING
		FIELD_ENTRY(m_nUndID)
		FIELD_ENTRY(m_nFuturesID)
		FIELD_ENTRY(m_bIsActiveFutures)
		FIELD_ENTRY(m_dBasis)
		FIELD_ENTRY(m_dRatio)
		
	END_PACKING

	__ActiveFuturesChange()
		: m_nUndID(0L)
		, m_nFuturesID(0L)
		, m_bIsActiveFutures(VARIANT_FALSE)
		, m_dBasis(BAD_DOUBLE_VALUE)
		, m_dRatio(BAD_DOUBLE_VALUE){}
};


// CActiveFuturesChange

class ATL_NO_VTABLE CActiveFuturesChange :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CActiveFuturesChange, &CLSID_ActiveFuturesChange>,
	public ISupportErrorInfo,
	public IPersistImpl<CActiveFuturesChange>,
	public IDispatchImpl<IActiveFuturesChange, &IID_IActiveFuturesChange, &LIBID_MSGSTRUCTLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public INetPackingConstSizeDataImpl<CActiveFuturesChange, __ActiveFuturesChange>
{
public:
	CActiveFuturesChange()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ACTIVEFUTURESCHANGE)

BEGIN_COM_MAP(CActiveFuturesChange)
	COM_INTERFACE_ENTRY(IActiveFuturesChange)
	COM_INTERFACE_ENTRY2(IDispatch, IActiveFuturesChange)
	COM_INTERFACE_ENTRY(INetPacking)
	COM_INTERFACE_ENTRY(IPersist)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


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
	IMPLEMENT_LONG_PROP(UndID,		 m_nUndID)
	IMPLEMENT_LONG_PROP(FutID, m_nFuturesID)

	IMPLEMENT_BOOL_PROP(IsActive, m_bIsActiveFutures)
	IMPLEMENT_DOUBLE_PROP(Basis, m_dBasis)
	IMPLEMENT_DOUBLE_PROP(Ratio, m_dRatio)

};

OBJECT_ENTRY_AUTO(__uuidof(ActiveFuturesChange), CActiveFuturesChange)
