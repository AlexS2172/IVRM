// EtsIndexDivColl.h : Declaration of the CEtsIndexDivColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"

#include "EtsIndexDivAtom.h"


// CEtsIndexDivColl

_COM_SMARTPTR_TYPEDEF(IEtsIndexDivColl, IID_IEtsIndexDivColl);

class CEtsIndexDivColl;
typedef IDispatchImpl<IEtsIndexDivColl, &IID_IEtsIndexDivColl, &LIBID_EtsGeneralLib>		IEtsIndexDivCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IEtsIndexDivCollDispImpl, IEtsIndexDivAtom, LONG, LONG, LONG, LONG  >	IEtsIndexDivCollImpl;

const double OneMinute = 1./(24.*59.);

class ATL_NO_VTABLE CEtsIndexDivColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsIndexDivColl, &CLSID_EtsIndexDivColl>,
	public ISupportErrorInfoImpl<&CLSID_EtsIndexDivColl>,
	public IEtsIndexDivCollImpl
{
	typedef std::map<long, double> CWeightsCollection;
public:
	CEtsIndexDivColl()
		:m_nTodayCache(0L)
		,m_nExpiryCache(0L)
	{
		m_pUnkMarshaler = NULL;
	}

	BEGIN_PROP_MAP(CEtsIndexDivColl)
	END_PROP_MAP()



DECLARE_REGISTRY_RESOURCEID(IDR_ETSINDEXDIVCOLL)


BEGIN_COM_MAP(CEtsIndexDivColl)
	COM_INTERFACE_ENTRY(IEtsIndexDivColl)
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
		IEtsIndexDivCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

private:
	IEtsDivCollPtr m_spCustomDivsCache;
	DATE m_nTodayCache;
	DATE m_nExpiryCache;

	IEtsDivCollPtr GetCachedDividends(DATE nToday, DATE nExpiry);
	IEtsDivCollPtr GetCachedDividends2(DATE dtNow, DATE dtExpiryOV, DATE tmCloseTime);


private:
	CWeightsCollection m_collWeights;
public:
	STDMETHOD(Add)(LONG Key, LONG SortKey, IEtsIndexDivAtom* Value, IEtsIndexDivAtom** pRetVal);
	STDMETHOD(CopyToWithWeight)( DOUBLE dWeight, IEtsIndexDivColl* pDest , IEtsIndexDivColl** ppVal);
	STDMETHOD(GetDividends)( DATE nToday,  DATE nExpiry,  LONG nCount, SAFEARRAY ** psaDivAmounts, SAFEARRAY ** psaDivDates,  LONG* pnCount);
	STDMETHOD(GetNearest)( DATE nToday,  DATE nExpiry,  DOUBLE* pdDivAmount,  DOUBLE* pdDivDate);
	STDMETHOD(GetDividendCount)( DATE nToday,  DATE nExpiry,  LONG* pnCount);
	STDMETHOD(SetWeight)(LONG nKey, DOUBLE dWeight);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	STDMETHOD(GetDividends2)(DATE dtNow,  DATE dtExpiryOV, DATE tmCloseTime, LONG nCount, SAFEARRAY ** psaDivAmounts, SAFEARRAY ** psaDivDates,  LONG* pnCount);
	STDMETHOD(GetNearest2)(DATE dtNow,  DATE dtExpiryOV, DATE tmCloseTime,  DOUBLE* pdDivAmount,  DOUBLE* pdDivDate);
	STDMETHOD(GetDividendCount2)(DATE dtNow, DATE dtExpiryOV, DATE tmCloseTime,  LONG* pnCount);
	STDMETHOD(Reload)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsIndexDivColl), CEtsIndexDivColl)
