// EtsIndexDivAtom.h : Declaration of the CEtsIndexDivAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"
#include "EtsDivAtom.h"
#include "EtsDivColl.h"
#include "EtsHolidayAtom.h"

struct _IndexDivAtomBaseTypes
{
	LONG				m_nDivFreq;
	DATE				m_dtDivDate;
	DOUBLE				m_dDivAmt;
	LONG				m_nDivFreqCust;
	DATE				m_dtDivDateCust;
	DOUBLE				m_dDivAmtCust;
	EtsDivTypeEnum		m_enDivType;
	LONG				m_nAssetID;
	
	_IndexDivAtomBaseTypes():m_nDivFreq(0),
							 m_dtDivDate(0.0),
							 m_dDivAmt(0.0),
							 m_nDivFreqCust(0),
							 m_dtDivDateCust(0.0),
							 m_dDivAmtCust(0.0),
							 m_enDivType(enDivMarket),
							 m_nAssetID(0)
	{
	}
};

struct _IndexDivAtom: public _IndexDivAtomBaseTypes
{
	IEtsDivCollPtr		m_spCustomDivs;
	IEtsHolidayAtomPtr	m_spHolidays;
};

// CEtsIndexDivAtom

//_COM_SMARTPTR_TYPEDEF(IEtsIndexDivAtom, IID_IEtsIndexDivAtom);

class ATL_NO_VTABLE CEtsIndexDivAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsIndexDivAtom, &CLSID_EtsIndexDivAtom>,
	public ISupportErrorInfoImpl<&CLSID_EtsIndexDivAtom>,
	public IDispatchImpl<IEtsIndexDivAtom, &IID_IEtsIndexDivAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public _IndexDivAtom

{
public:
	CEtsIndexDivAtom()
		:m_CacheToday(0L)
		,m_CacheExpiry(0L)
		,m_lCacheDivCount(0L)
	{
		m_pUnkMarshaler = NULL;
	}

	BEGIN_PROP_MAP(CEtsIndexDivAtom)
	END_PROP_MAP()

	

DECLARE_REGISTRY_RESOURCEID(IDR_ETSINDEXDIVATOM)


BEGIN_COM_MAP(CEtsIndexDivAtom)
	COM_INTERFACE_ENTRY(IEtsIndexDivAtom)
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
		m_spCustomDivs = NULL;
		m_spHolidays = NULL;
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

private:
	DATE  m_CacheToday;
	DATE  m_CacheExpiry;
	long  m_lCacheDivCount;

public:
	IMPLEMENT_SIMPLE_PROPERTY(LONG, DivFreq, m_nDivFreq)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, DivDate,m_dtDivDate)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DivAmt, m_dDivAmt)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, DivFreqCust, m_nDivFreqCust)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, DivDateCust, m_dtDivDateCust)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DivAmtCust, m_dDivAmtCust)
	IMPLEMENT_SIMPLE_PROPERTY(EtsDivTypeEnum, DivType, m_enDivType)
	IMPLEMENT_OBJECT_PROPERTY(IEtsDivColl*, CustomDivs, m_spCustomDivs)
	STDMETHOD(IsValidDivs)(EtsDivTypeEnum enDivType, VARIANT_BOOL* pVal);
	//STDMETHOD(CopyToWithWeight)( DOUBLE dWeight, IEtsIndexDivAtom* pDest , IEtsIndexDivAtom** ppVal);
	STDMETHOD(GetDividends)( DATE nToday,  DATE nExpiry,  LONG nCount, SAFEARRAY ** psaDivAmounts,  SAFEARRAY ** psaDivDates,  LONG* pnCount);
	STDMETHOD(GetNearest)( DATE nToday,  DATE nExpiry,  DOUBLE* pdDivAmount,  DOUBLE* pdDivDate);
	STDMETHOD(GetDividendCount)( DATE nToday,  DATE nExpiry,  LONG* pnCount);
	STDMETHOD(Clear)(void);
	STDMETHOD(Clone)(IEtsIndexDivAtom** pDestination);
	STDMETHOD(ResetInternalCache)(void);
	IMPLEMENT_OBJECT_PROPERTY(IEtsHolidayAtom*, Holidays, m_spHolidays);
	STDMETHOD(GetDividends2)(DATE dtNow, DATE dtExpiryOV, DATE tmCloseTime, LONG nCount, SAFEARRAY ** psaDivAmounts, SAFEARRAY ** psaDivDates, LONG* pnCount);
	STDMETHOD(GetDividendCount2)(DATE dtNow, DATE dtExpiryOV, DATE tmCloseTime, LONG* pnCount);
	STDMETHOD(GetNearest2)(DATE dtNow, DATE dtExpiryOV, DATE dtCloseTime, DOUBLE* pdDivAmount,  DOUBLE* pdDivDate);
	IMPLEMENT_SIMPLE_PROPERTY(LONG, AssetID, m_nAssetID)
	STDMETHOD(Reload)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsIndexDivAtom), CEtsIndexDivAtom)
