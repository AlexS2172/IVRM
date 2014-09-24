// Created 03 - 05 - 2005
// Created by Artem Shilaev
// Egar T


#pragma once
#include "resource.h"       // main symbols
#include "MsgPropImpl.h"
//#include "MsgStruct.h"


struct __IVSettingsData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_sUseMarketIV)
		FIELD_ENTRY(m_sAutoFitIV )
		FIELD_ENTRY(m_sFitOnly)
	END_PACKING

	StringF							m_sUseMarketIV ;
	StringF							m_sAutoFitIV ;
	StringF							m_sFitOnly ;
};

// CUpdateUndIVSettings

class ATL_NO_VTABLE CUpdateUndIVSettings : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CUpdateUndIVSettings, &CLSID_UpdateUndIVSettings>,
	//public ISupportErrorInfo,
	public IDispatchImpl<IUpdateUndIVSettings, &IID_IUpdateUndIVSettings, &LIBID_MSGSTRUCTLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public INetPackingConstSizeDataImpl<CUpdateUndIVSettings , __IVSettingsData>
{
public:
	CUpdateUndIVSettings()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_UPDATEUNDIVSETTINGS)

DECLARE_NOT_AGGREGATABLE(CUpdateUndIVSettings)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CUpdateUndIVSettings)
	COM_INTERFACE_ENTRY(IUpdateUndIVSettings)
//	COM_INTERFACE_ENTRY2(IDispatch, CUpdateUndIVSettings)
	//COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:
	IMPLEMENT_VARCHAR_PROP(UseMarketIV , m_sUseMarketIV )
	IMPLEMENT_VARCHAR_PROP( AutoFitIV ,m_sAutoFitIV )
	IMPLEMENT_VARCHAR_PROP( FitOnly ,m_sFitOnly )
};

OBJECT_ENTRY_AUTO(__uuidof(UpdateUndIVSettings), CUpdateUndIVSettings)
