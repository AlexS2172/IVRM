// ManualPriceUpdate.h : Declaration of the CManualPriceUpdate

#pragma once
#include "resource.h"       // main symbols
#include "MsgPacking.h"
#include "MsgPropImpl.h"
#include "MsgStruct.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

struct __ManualPriceUpdateData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_nContractType)
		FIELD_ENTRY(m_nUndID)
		FIELD_ENTRY(m_nContractID)
		FIELD_ENTRY(m_nStatus)
		FIELD_ENTRY(m_dActivePrice)
	END_PACKING

	LongF			m_nContractType;
	LongF			m_nUndID;
	LongF			m_nContractID;
	TStaticField<MANUAL_PRICE_UPDATE_STATUS>		m_nStatus;
	DoubleF			m_dActivePrice;
	
};


// CManualPriceUpdate

class ATL_NO_VTABLE CManualPriceUpdate :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CManualPriceUpdate, &CLSID_ManualPriceUpdate>,
	public ISupportErrorInfo,
	public IDispatchImpl<IManualPriceUpdate, &IID_IManualPriceUpdate, &LIBID_MSGSTRUCTLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public INetPackingConstSizeDataImpl<CManualPriceUpdate, __ManualPriceUpdateData>
{
public:
	CManualPriceUpdate()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MANUALPRICEUPDATE)


BEGIN_COM_MAP(CManualPriceUpdate)
	COM_INTERFACE_ENTRY(IManualPriceUpdate)
	COM_INTERFACE_ENTRY2(IDispatch, IManualPriceUpdate)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

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

	IMPLEMENT_LONG_PROP(ContractType, m_nContractType)
	IMPLEMENT_LONG_PROP(UndID, m_nUndID)
	IMPLEMENT_LONG_PROP(ContractID, m_nContractID)
	IMPLEMENT_DOUBLE_PROP(ActivePrice, m_dActivePrice)
	
	IMPLEMENT_SIMPLE_PROP(MANUAL_PRICE_UPDATE_STATUS, Status, m_nStatus)
};

OBJECT_ENTRY_AUTO(__uuidof(ManualPriceUpdate), CManualPriceUpdate)
