// CIvDataUpdate.h : Declaration of the CCIvDataUpdate
// Created by Artem Shilaev 2005 - 05 - 13

// IvDataUpdateStock.h : Declaration of the CIvDataUpdateStock

#pragma once
#include "resource.h"       // main symbols

#include "MsgStruct.h"

struct __IVMessageDataStock
{
	BEGIN_PACKING
		FIELD_ENTRY(m_lSymbolID)
	END_PACKING

	LongF							m_lSymbolID;
};

// CIvDataUpdateStock

class ATL_NO_VTABLE CIvDataUpdateStock : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CIvDataUpdateStock, &CLSID_IvDataUpdateStock>,
	public IDispatchImpl<IIvDataUpdateStock, &IID_IIvDataUpdateStock, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CIvDataUpdateStock, __IVMessageDataStock>
{
public:
	CIvDataUpdateStock()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_IVDATAUPDATESTOCK)


BEGIN_COM_MAP(CIvDataUpdateStock)
	COM_INTERFACE_ENTRY(IIvDataUpdateStock)
	COM_INTERFACE_ENTRY2(IDispatch, IIvDataUpdateStock)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

//	IMPLEMENT_LONG_PROP(SymbolID,m_lSymbolID) ;
	STDMETHODIMP put_SymbolID(long NewVal)
	{
		ObjectLock lock(this);
		m_lSymbolID = NewVal;
		return S_OK;
	}
	
	STDMETHODIMP get_SymbolID(long* pRetVal)
	{
		if (!pRetVal)		
			return E_POINTER;	
		ObjectLock lock(this);
		*pRetVal = m_lSymbolID;
		return S_OK;
	}													
};

OBJECT_ENTRY_AUTO(__uuidof(IvDataUpdateStock), CIvDataUpdateStock)
