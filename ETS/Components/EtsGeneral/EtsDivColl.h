// EtsDivColl.h : Declaration of the CEtsDivColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"
#include "EtsDivAtom.h"
#include "EtsHolidayAtom.h"
#include "CommonSPtr.h"

//_COM_SMARTPTR_TYPEDEF(IEtsDivColl, IID_IEtsDivColl);

// CEtsDivColl

class ATL_NO_VTABLE CEtsDivColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsDivColl, &CLSID_EtsDivColl>,
	public ISupportErrorInfoImpl<&CLSID_EtsDivColl>,
	public IDispatchImpl<IEtsDivColl, &IID_IEtsDivColl, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CEtsDivColl()
		:m_nTodayCache(0)
		,m_lExpryCache(0)
		,m_lQuantityCache(0)
		,m_bDivCacheInit(false)
		,m_spHolidays(NULL)
		,m_nAssetID(0)

	{
		m_pUnkMarshaler = NULL;
	}

	BEGIN_PROP_MAP(CEtsDivColl)
	END_PROP_MAP()

DECLARE_REGISTRY_RESOURCEID(IDR_ETSDIVCOLL)


BEGIN_COM_MAP(CEtsDivColl)
	COM_INTERFACE_ENTRY(IEtsDivColl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		ClearCache();
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		m_DivColl.clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	STDMETHOD(GetDividends)(DATE nToday, DATE nExpiry, LONG nCount, SAFEARRAY ** psaDivAmounts,  SAFEARRAY ** psaDivDates, LONG* pnCount);
	STDMETHOD(GetNearest)(DATE nToday, DATE nExpiry, DOUBLE* pdDivAmount,  DOUBLE* pdDivDate);
	STDMETHOD(GetDividendCount)( DATE nToday, DATE nExpiry, LONG* pnCount);
private:
	typedef		std::map<LONG, DOUBLE>		EnumCollType;
	typedef		EnumCollType::iterator		EnumIterType;
	typedef		std::pair<DOUBLE,DOUBLE>	EnumItemType;
	typedef     std::map<LONG, LONG>		EnumQuantityCacheType;

	EnumCollType							m_DivColl;

	DATE                                    m_nTodayCache;
	DATE                                    m_lExpryCache;
	long                                    m_lQuantityCache;
	bool                                    m_bDivCacheInit;

	CComSafeArray<double>					m_spDatesCache;
	CComSafeArray<double>					m_spAmountCache;

	void ClearCache()
	{
		m_nTodayCache    = 0;
		m_lExpryCache    = 0;
		m_lQuantityCache = 0;
		m_bDivCacheInit  = false;
	}
public:

	IEtsHolidayAtomPtr						m_spHolidays;
	LONG									m_nAssetID;

	STDMETHOD(Add)(DATE Date, DOUBLE Amount);
	STDMETHOD(AddNonUnique)(DATE Date, DOUBLE Amount);
	STDMETHOD(Remove)(DATE Date);
	STDMETHOD(Clear)(void);
	STDMETHOD(CopyToWithWeight)( DOUBLE dWeight, IEtsDivColl* pDest , IEtsDivColl** ppVal);
	STDMETHOD(Clone)(IEtsDivColl** ppVal);
	//ETM MODE//
	IMPLEMENT_OBJECT_PROPERTY(IEtsHolidayAtom*, Holidays, m_spHolidays);
	STDMETHOD(GetDividendCount2)(DATE dtNow, DATE dtExpiryOV, DATE	dtCloseTime, LONG* pnCount);
	STDMETHOD(GetDividends2)(DATE dtNow, DATE dtExpiryOV, DATE dtCloseTime, LONG nCount, SAFEARRAY ** psaDivAmounts,  SAFEARRAY ** psaDivDates, LONG* pnCount);
	STDMETHOD(GetNearest2)( DATE dtNow, DATE dtExpiryOV, DATE dtCloseTime,  DOUBLE* pdDivAmount,  DOUBLE* pdDivDate);
	IMPLEMENT_SIMPLE_PROPERTY(LONG, AssetID, m_nAssetID)
	STDMETHOD(Reload)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsDivColl), CEtsDivColl)
