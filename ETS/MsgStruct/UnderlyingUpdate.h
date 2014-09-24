// UnderlyingUpdate.h : Declaration of the CUnderlyingUpdate

#ifndef __UNDERLYINGUPDATE_H_
#define __UNDERLYINGUPDATE_H_

#pragma once
#include "resource.h"       // main symbols
#include "MsgPacking.h"
#include "MsgStruct.h"
#include "MsgPropImpl.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


struct __UnderlyingUpdateData
{
	BEGIN_PACKING
		FIELD_ENTRY(m_nUndID)
		FIELD_ENTRY(m_dCoeff)
		FIELD_ENTRY(m_bUseHead)

		FIELD_ENTRY(m_dDivAmt)
		FIELD_ENTRY(m_dDivAmtCust)
		FIELD_ENTRY(m_dtDivDate)
		FIELD_ENTRY(m_dtDivDateCust)
		FIELD_ENTRY(m_dDivFreq)
		FIELD_ENTRY(m_dDivFreqCust)
		FIELD_ENTRY(m_nDivType)
		FIELD_ENTRY(m_dYield)

		FIELD_ENTRY(m_dSkew)
		FIELD_ENTRY(m_dKurt)
		FIELD_ENTRY(m_bIsActive)
		FIELD_ENTRY(m_bIsHTB)
		FIELD_ENTRY(m_bAllowLending)
		FIELD_ENTRY(m_dSOQ)
		FIELD_ENTRY(m_bIsHedgeSymbol)
		FIELD_ENTRY(m_nStkProfileID)
		FIELD_ENTRY(m_nOptProfileID)
		FIELD_ENTRY(m_nPrimaryExchange)
		FIELD_ENTRY(m_nRegion)

		FIELD_ENTRY(m_enUpdStatus)
	END_PACKING

	LongF			m_nUndID;
	DoubleF			m_dCoeff;
	BoolF			m_bUseHead;

	DoubleF			m_dDivAmt;
	DoubleF			m_dDivAmtCust;
	DateF			m_dtDivDate;
	DateF			m_dtDivDateCust;
	DoubleF			m_dDivFreq;
	DoubleF			m_dDivFreqCust;
	LongF			m_nDivType;
	DoubleF			m_dYield;

	DoubleF			m_dSkew;
	DoubleF			m_dKurt;
	BoolF			m_bIsActive;
	BoolF			m_bIsHTB;
	BoolF			m_bAllowLending;
	DoubleF			m_dSOQ;
	BoolF			m_bIsHedgeSymbol;
	LongF			m_nStkProfileID;
	LongF			m_nOptProfileID;
	LongF			m_nPrimaryExchange;
	LongF			m_nRegion;
	

	TStaticField<UNDERLYING_UPDATE_STATUS>		m_enUpdStatus;
};


// CUnderlyingUpdate

class ATL_NO_VTABLE CUnderlyingUpdate :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CUnderlyingUpdate, &CLSID_UnderlyingUpdate>,
	public ISupportErrorInfo,
	public IDispatchImpl<IUnderlyingUpdate, &IID_IUnderlyingUpdate, &LIBID_MSGSTRUCTLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public INetPackingConstSizeDataImpl<CUnderlyingUpdate, __UnderlyingUpdateData>
{
public:
	CUnderlyingUpdate()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_UNDERLYINGUPDATE)


BEGIN_COM_MAP(CUnderlyingUpdate)
	COM_INTERFACE_ENTRY(IUnderlyingUpdate)
	COM_INTERFACE_ENTRY2(IDispatch, IUnderlyingUpdate)
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

public:

	IMPLEMENT_LONG_PROP		(UndID,					m_nUndID)
	IMPLEMENT_DOUBLE_PROP	(Coeff,					m_dCoeff)
	IMPLEMENT_BOOL_PROP		(UseHead,				m_bUseHead)

	IMPLEMENT_DOUBLE_PROP	(DivAmt,				m_dDivAmt)
	IMPLEMENT_DOUBLE_PROP	(DivAmtCust,			m_dDivAmtCust)
	IMPLEMENT_DATE_PROP		(DivDate,				m_dtDivDate)
	IMPLEMENT_DATE_PROP		(DivDateCust,			m_dtDivDateCust)
	IMPLEMENT_DOUBLE_PROP	(DivFreq,				m_dDivFreq)
	IMPLEMENT_DOUBLE_PROP	(DivFreqCust,			m_dDivFreqCust)
	IMPLEMENT_DOUBLE_PROP	(Yield,					m_dYield)
	IMPLEMENT_LONG_PROP		(DivType,				m_nDivType)

	IMPLEMENT_DOUBLE_PROP	(Skew,					m_dSkew)
	IMPLEMENT_DOUBLE_PROP	(Kurt,					m_dKurt)
	IMPLEMENT_BOOL_PROP		(IsActive,				m_bIsActive)
	IMPLEMENT_BOOL_PROP		(IsHTB,					m_bIsHTB)
	IMPLEMENT_BOOL_PROP		(AllowLending,			m_bAllowLending)
	IMPLEMENT_BOOL_PROP		(IsHedgeSymbol,			m_bIsHedgeSymbol)
	IMPLEMENT_DOUBLE_PROP	(SOQ,					m_dSOQ)
	IMPLEMENT_LONG_PROP		(StkProfile,			m_nStkProfileID)
	IMPLEMENT_LONG_PROP		(OptProfile,			m_nOptProfileID)
	IMPLEMENT_LONG_PROP		(PrimaryExchange,		m_nPrimaryExchange)
	IMPLEMENT_LONG_PROP		(Region,				m_nRegion)

	
	IMPLEMENT_SIMPLE_PROP(UNDERLYING_UPDATE_STATUS, UpdStatus, m_enUpdStatus)

};

OBJECT_ENTRY_AUTO(__uuidof(UnderlyingUpdate), CUnderlyingUpdate)

#endif