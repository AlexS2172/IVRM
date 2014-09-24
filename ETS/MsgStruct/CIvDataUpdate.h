// CIvDataUpdate.h : Declaration of the CCIvDataUpdate
// Created by Artem Shilaev 2005 - 04 - 11

#pragma once
#include "resource.h"       // main symbols

#include "MsgPropImpl.h"

struct __IVMessageData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_dOptionID)
		FIELD_ENTRY(m_dIV)
		FIELD_ENTRY(m_dMIV)
		FIELD_ENTRY(m_sSymbolName)
		FIELD_ENTRY(m_bCall)
		FIELD_ENTRY(m_pHavPosition)
		FIELD_ENTRY(m_dTheoVol)
	END_PACKING

	LongF							m_dOptionID;
	CharF							m_bCall ;
	CharF							m_pHavPosition ;
	DoubleF							m_dIV;
	DoubleF							m_dMIV;
	StringF							m_sSymbolName ;
	DoubleF							m_dTheoVol ;
};

// CCIvDataUpdate

class ATL_NO_VTABLE CCIvDataUpdate : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CCIvDataUpdate, &CLSID_CIvDataUpdate>,
	public IDispatchImpl<ICIvDataUpdate, &IID_ICIvDataUpdate, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CCIvDataUpdate, __IVMessageData>
{
public:
	CCIvDataUpdate()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CIVDATAUPDATE)
DECLARE_NOT_AGGREGATABLE(CCIvDataUpdate)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCIvDataUpdate)
	COM_INTERFACE_ENTRY(ICIvDataUpdate)
	COM_INTERFACE_ENTRY2(IDispatch, ICIvDataUpdate)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()


	HRESULT FinalConstruct()
	{
		m_dTheoVol  = -1.0 ;
		m_dMIV = 0 ;
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:
	STDMETHODIMP put_OptionIDName(long NewVal)
	{
		ObjectLock lock(this);
		m_dOptionID = NewVal;
		return S_OK;
	}
	STDMETHODIMP get_OptionIDName(long* pRetVal)
	{
		if (!pRetVal)
			return E_POINTER;
		ObjectLock lock(this);
		*pRetVal = m_dOptionID ;
		return S_OK;
	}													
	IMPLEMENT_DOUBLE_PROP(TheoVol,m_dTheoVol)
	IMPLEMENT_DOUBLE_PROP(dIV, m_dIV)
	IMPLEMENT_DOUBLE_PROP(dMIV,m_dMIV)
	IMPLEMENT_LONG_PROP(OptionID,m_dOptionID)
	IMPLEMENT_VARCHAR_PROP(SymbolName,m_sSymbolName)
	STDMETHODIMP put_Call(VARIANT_BOOL NewVal)
	{
		ObjectLock lock(this);
		m_bCall = ( NewVal == VARIANT_TRUE ? 1 : 0 ) ;
		return S_OK;								
	}
	
	STDMETHODIMP get_Call(VARIANT_BOOL* pRetVal)
	{
		if (!pRetVal)
			return E_POINTER;
		ObjectLock lock(this);
		*pRetVal = ( m_bCall == 1 ? VARIANT_TRUE : VARIANT_FALSE ) ;   
		return S_OK;
	}													

	STDMETHODIMP put_Position(VARIANT_BOOL NewVal)
	{
		ObjectLock lock(this);
		m_pHavPosition = ( NewVal == VARIANT_TRUE ? 1 : 0 ) ;
		return S_OK;								
	}

	STDMETHODIMP get_Position(VARIANT_BOOL* pRetVal)
	{
		if (!pRetVal)
			return E_POINTER;
		ObjectLock lock(this);
		*pRetVal = ( m_pHavPosition == 1 ? VARIANT_TRUE : VARIANT_FALSE ) ;   
		return S_OK;
	}													
};

OBJECT_ENTRY_AUTO(__uuidof(CIvDataUpdate), CCIvDataUpdate)
