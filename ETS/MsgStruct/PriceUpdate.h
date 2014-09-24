// PriceUpdate.h : Declaration of the CPriceUpdate

#ifndef __PRICEUPDATE_H_
#define __PRICEUPDATE_H_

#pragma once
#include "resource.h"       // main symbols
#include "MsgPacking.h"
#include "MsgStruct.h"
#include "MsgPropImpl.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

struct __PriceUpdateData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_nContractType)
		FIELD_ENTRY(m_nUndID)
		FIELD_ENTRY(m_nContractID)
		FIELD_ENTRY(m_dPriceClose)
		FIELD_ENTRY(m_dTheoPriceClose)
	END_PACKING

	LongF			m_nContractType;
	LongF			m_nUndID;
	LongF			m_nContractID;
	DoubleF			m_dPriceClose;
	DoubleF			m_dTheoPriceClose;
};

// CPriceUpdate

class ATL_NO_VTABLE CPriceUpdate :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPriceUpdate, &CLSID_PriceUpdate>,
	public ISupportErrorInfo,
	public IDispatchImpl<IPriceUpdate, &IID_IPriceUpdate, &LIBID_MSGSTRUCTLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public INetPackingConstSizeDataImpl<CPriceUpdate, __PriceUpdateData>
{
public:
	CPriceUpdate()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PRICEUPDATE)


BEGIN_COM_MAP(CPriceUpdate)
	COM_INTERFACE_ENTRY(IPriceUpdate)
	COM_INTERFACE_ENTRY2(IDispatch, IPriceUpdate)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	COM_INTERFACE_ENTRY(INetPacking)
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

// IPriceUpdate
public:
	IMPLEMENT_LONG_PROP(ContractType, m_nContractType)
	IMPLEMENT_LONG_PROP(UndID, m_nUndID)
	IMPLEMENT_LONG_PROP(ContractID, m_nContractID)
	IMPLEMENT_DOUBLE_PROP(PriceClose, m_dPriceClose)
	IMPLEMENT_DOUBLE_PROP(TheoPriceClose, m_dTheoPriceClose)
};

OBJECT_ENTRY_AUTO(__uuidof(PriceUpdate), CPriceUpdate)

#endif //__PRICEUPDATE_H_