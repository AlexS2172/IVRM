// FlexOption.h : Declaration of the CFlexOption

#pragma once
#include "resource.h"       // main symbols

#include "MsgStruct.h"
#include "MsgPropImpl.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

struct __FlexOption
{
public:

	LongF		m_nUndID;
	LongF		m_nRootID;
	StringF		m_sRootName;
	LongF       m_nLotSize;
	BoolF		m_bIsFit;
	BoolF		m_bIsSynth;

	BEGIN_PACKING
		FIELD_ENTRY(m_nUndID)
		FIELD_ENTRY(m_nRootID)
		FIELD_ENTRY(m_sRootName)
		FIELD_ENTRY(m_nLotSize)
		FIELD_ENTRY(m_bIsFit)
		FIELD_ENTRY(m_bIsSynth)
	END_PACKING

		__FlexOption()
		: m_nUndID(0L)
		, m_nRootID(0L)
		, m_sRootName("")
		, m_nLotSize(BAD_LONG_VALUE)
		, m_bIsFit(VARIANT_TRUE)
		, m_bIsSynth(VARIANT_FALSE){}
};

// CFlexOption

class ATL_NO_VTABLE CFlexOption :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CFlexOption, &CLSID_FlexOption>,
	public ISupportErrorInfo,
	//public IDispatchImpl<IFlexOption, &IID_IFlexOption, &LIBID_MSGSTRUCTLib>,
	public IPersistImpl<CFlexOption>,
	public INetPackingConstSizeDataImpl<CFlexOption, __FlexOption>,
	public IDispatchImpl<IFlexOption, &IID_IFlexOption, &LIBID_MSGSTRUCTLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
	//public IDispatchImpl<IUIDGenerator, &IID_IUIDGenerator, &LIBID_MSGSTRUCTLib>
{
public:
	CFlexOption()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FLEXOPTION)


BEGIN_COM_MAP(CFlexOption)
	COM_INTERFACE_ENTRY(IFlexOption)
	COM_INTERFACE_ENTRY2(IDispatch, IFlexOption)
	//COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(INetPacking)
	//COM_INTERFACE_ENTRY(IUIDGenerator)
	COM_INTERFACE_ENTRY(IPersist)
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
	IMPLEMENT_VARCHAR_PROP(RootName, m_sRootName)
	IMPLEMENT_LONG_PROP(UndID, m_nUndID)
	IMPLEMENT_LONG_PROP(RootID, m_nRootID)
	IMPLEMENT_LONG_PROP(LotSize, m_nLotSize)
	IMPLEMENT_BOOL_PROP(IsSynth,m_bIsSynth)
	IMPLEMENT_BOOL_PROP(IsFit,m_bIsFit)
};

OBJECT_ENTRY_AUTO(__uuidof(FlexOption), CFlexOption)
