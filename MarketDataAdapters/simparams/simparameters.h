// SimParameters.h : Declaration of the CSimParameters

#ifndef __SIMPARAMETERS_H_
#define __SIMPARAMETERS_H_

#include "resource.h"       // main symbols
#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibReg.h>

/////////////////////////////////////////////////////////////////////////////
// CSimParameters
class ATL_NO_VTABLE CSimParameters : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSimParameters, &CLSID_SimParameters>,
	public ISupportErrorInfoImpl<&IID_ISimParameters>,
	public IDispatchImpl<ISimParameters, &IID_ISimParameters, &LIBID_SIMPARAMSLib>
{
public:
	CSimParameters()
	{
		m_lVolumeMax = 10;
		m_lVolumeMin = 100;

		m_lPriceDelta = 30; //Cent
		m_lBidDelta = 20; //Cent
		m_lAskDelta = 20; //Cent
		
		m_lBidSizeMin = 20;
		m_lBidSizeMax = 200;
		m_lAskSizeMin = 20;
		m_lAskSizeMax = 200;

		m_lIntervalMin = 0;
		m_lIntervalMax = 10;

		m_lNonRealtimeIntervalMin = 0;
		m_lNonRealtimeIntervalMax = 0;

		m_lTradeIntervalMin = 0;
		m_lTradeIntervalMax = 1;

		m_bEnableOptions = VARIANT_TRUE;
		m_bEnableStocks = VARIANT_TRUE;

		m_lVolatilityIntervalMin =0;
		m_lVolatilityIntervalMax = 100;
		m_dVolatilityDelta = 1.;

		m_lStockIntervalMin = 0;
		m_lStockIntervalMax = 10;
		m_bsConnectionString  = L"";
		m_bsDatabaseName = L"";

		m_dtPlayback = 0.;
		m_lPlaybackSpeed = 0L;

		m_lPriceProvider = -1L;

		EgLib::CEgRegKey key;
		if (ERROR_SUCCESS != key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_READ))
		{
			key.Create(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"));
		}
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SIMPARAMETERS)
DECLARE_CLASSFACTORY_SINGLETON(CSimParameters)

DECLARE_PROTECT_FINAL_CONSTRUCT()

HRESULT FinalConstruct()
{
	EgLib::CEgRegKey key;
	if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_READ))
	{
		key.QueryValue(m_lVolumeMin, _T("VolumeMin"));
		key.QueryValue(m_lVolumeMax, _T("VolumeMax"));

		key.QueryValue(m_lPriceDelta, _T("PriceDelta"));
		key.QueryValue(m_lBidDelta, _T("BidDelta"));
		key.QueryValue(m_lAskDelta, _T("AskDelta"));
		

		key.QueryValue(m_lBidSizeMin, _T("MinBidSize"));
		key.QueryValue(m_lBidSizeMax, _T("MaxBidSize"));
		key.QueryValue(m_lAskSizeMin, _T("MinAskSize"));
		
		key.QueryValue(m_lIntervalMin, _T("MinInterval"));
		key.QueryValue(m_lIntervalMax, _T("MaxInterval"));
		key.QueryValue(m_lStockIntervalMin, _T("MinStockInterval"));
		key.QueryValue(m_lStockIntervalMax, _T("MaxStockInterval"));

		key.QueryValue(m_lNonRealtimeIntervalMax, _T("MaxGetInterval"));
		key.QueryValue(m_lNonRealtimeIntervalMin, _T("MinGetInterval"));

		key.QueryValue(m_lVolatilityIntervalMin, _T("MinVolaInterval"));
		key.QueryValue(m_lVolatilityIntervalMax, _T("MaxVolaInterval"));

		key.QueryValue(m_lTradeIntervalMin, _T("MinTradeInterval"));
		key.QueryValue(m_lTradeIntervalMax, _T("MaxTradeInterval"));

		DWORD dwVolaDelta = 0;
		key.QueryValue(dwVolaDelta, _T("VolaTility"));
		if(dwVolaDelta)
			m_dVolatilityDelta = 1. * dwVolaDelta /1000.;

		key.QueryValue(m_bsConnectionString, _T("DBConnection"));
		key.QueryValue(m_bsDatabaseName, _T("DatabaseName"));

		DWORD dwDate;
		key.QueryValue(dwDate, _T("PlayBackDate"));
		m_dtPlayback =dwDate;

		key.QueryValue(m_lPlaybackSpeed, _T("PlayBackSpeed"));

		key.QueryValue(m_lPriceProvider, _T("PriceProvider"));
	}
	return S_OK;
}

void FinalRelease()
{
	//TODO: I don`t know why this code has been commented
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lVolumeMin, _T("VolumeMin"));
		key.SetValue(m_lVolumeMax, _T("VolumeMax"));

		key.SetValue(m_lPriceDelta, _T("PriceDelta"));
		key.SetValue(m_lBidDelta, _T("BidDelta"));
		key.SetValue(m_lAskDelta, _T("AskDelta"));
		
		key.SetValue(m_lBidSizeMin, _T("MinBidSize"));
		key.SetValue(m_lBidSizeMax, _T("MaxBidSize"));
		key.SetValue(m_lAskSizeMin, _T("MinAskSize"));
		key.SetValue(m_lAskSizeMax, _T("MaxAskSize"));
		
		key.SetValue(m_lIntervalMin, _T("MinInterval"));
		key.SetValue(m_lIntervalMax, _T("MaxInterval"));

		key.SetValue(m_lStockIntervalMin, _T("MinStockInterval"));
		key.SetValue(m_lStockIntervalMax, _T("MaxStockInterval"));

		key.SetValue(m_bsConnectionString, _T("DBConnection"));
		key.SetValue(m_bsDatabaseName, _T("DatabaseName"));

		key.SetValue(m_lNonRealtimeIntervalMax, _T("MaxGetInterval"));
		key.SetValue(m_lNonRealtimeIntervalMin, _T("MinGetInterval"));

		key.SetValue(m_lVolatilityIntervalMin, _T("MinVolaInterval"));
		key.SetValue(m_lVolatilityIntervalMax, _T("MaxVolaInterval"));

		key.SetValue(m_lTradeIntervalMin, _T("MinTradeInterval"));
		key.SetValue(m_lTradeIntervalMax, _T("MaxTradeInterval"));

		DWORD dwDate = (DWORD)m_dtPlayback;
		key.SetValue(dwDate, _T("PlayBackDate"));
		key.SetValue(m_lPlaybackSpeed, _T("PlayBackSpeed"));	

		DWORD dwVolaDelta = (DWORD)(m_dVolatilityDelta*1000);
		key.SetValue(dwVolaDelta, _T("MinGetInterval"));

		key.SetValue(m_lPriceProvider, _T("PriceProvider"));
	}
}
BEGIN_COM_MAP(CSimParameters)
	COM_INTERFACE_ENTRY(ISimParameters)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()


// ISimParameters
public:
	STDMETHOD(get_EnableStocks)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_EnableStocks)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_EnableOptions)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_EnableOptions)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_PlayBackSpeed)(/*[out, retval]*/ LONG *pVal);
	STDMETHOD(put_PlayBackSpeed)(/*[in]*/ LONG newVal);
	STDMETHOD(get_PlayBackDate)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_PlayBackDate)(/*[in]*/ DATE newVal);
	STDMETHOD(get_TradeUpdateIntervalMax)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_TradeUpdateIntervalMax)(/*[in]*/ long newVal);
	STDMETHOD(get_TradeUpdateIntervalMin)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_TradeUpdateIntervalMin)(/*[in]*/ long newVal);
	STDMETHOD(get_VolatilityDelta)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_VolatilityDelta)(/*[in]*/ double newVal);
	STDMETHOD(get_VolatilityUpdateIntervalMin)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_VolatilityUpdateIntervalMin)(/*[in]*/ long newVal);
	STDMETHOD(get_VolatilityUpdateIntervalMax)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_VolatilityUpdateIntervalMax)(/*[in]*/ long newVal);
	STDMETHOD(get_NonRealtimeIntervalMax)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_NonRealtimeIntervalMax)(/*[in]*/ long newVal);
	STDMETHOD(get_NonRealtimeIntervalMin)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_NonRealtimeIntervalMin)(/*[in]*/ long newVal);
	STDMETHOD(get_StockIntervalMin)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_StockIntervalMin)(/*[in]*/ long newVal);
	STDMETHOD(get_StockIntervalMax)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_StockIntervalMax)(/*[in]*/ long newVal);
	STDMETHOD(get_ConnectionString)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ConnectionString)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_IntervalMax)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_IntervalMax)(/*[in]*/ long newVal);
	STDMETHOD(get_IntervalMin)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_IntervalMin)(/*[in]*/ long newVal);
	STDMETHOD(get_AskSizeMax)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_AskSizeMax)(/*[in]*/ long newVal);
	STDMETHOD(get_AskSizeMin)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_AskSizeMin)(/*[in]*/ long newVal);
	STDMETHOD(get_BidSizeMax)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_BidSizeMax)(/*[in]*/ long newVal);
	STDMETHOD(get_BidSizeMin)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_BidSizeMin)(/*[in]*/ long newVal);
	STDMETHOD(get_AskPercent)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_AskPercent)(/*[in]*/ double newVal);
	STDMETHOD(get_BidPercent)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_BidPercent)(/*[in]*/ double newVal);
	STDMETHOD(get_PricePercent)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_PricePercent)(/*[in]*/ double newVal);
	STDMETHOD(get_VolumeMax)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_VolumeMax)(/*[in]*/ long newVal);
	STDMETHOD(get_VolumeMin)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_VolumeMin)(/*[in]*/ long newVal);
	STDMETHOD(get_DatabaseName)(/*[out, retval]*/ BSTR* pVal);
	STDMETHOD(put_DatabaseName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_PriceProvider)(/*[out, retval]*/ long* pVal);
	STDMETHOD(put_PriceProvider)(/*[in]*/ long newVal);

private:
	DWORD m_lVolumeMax;
	DWORD m_lVolumeMin;

	DWORD m_lPriceDelta;
	DWORD m_lBidDelta;
	DWORD m_lAskDelta;

	DWORD m_lBidSizeMin;
	DWORD m_lBidSizeMax;
	DWORD m_lAskSizeMin;
	DWORD m_lAskSizeMax;

	DWORD m_lIntervalMin;
	DWORD m_lIntervalMax;

	DWORD m_lNonRealtimeIntervalMin;
	DWORD m_lNonRealtimeIntervalMax;

	DWORD m_lStockIntervalMin;
	DWORD m_lStockIntervalMax;

	DWORD m_lTradeIntervalMin;
	DWORD m_lTradeIntervalMax;

	_bstr_t m_bsConnectionString;
	_bstr_t m_bsDatabaseName;

	DWORD m_lVolatilityIntervalMin;
	DWORD m_lVolatilityIntervalMax;
	double m_dVolatilityDelta;
	DATE   m_dtPlayback;
	DWORD   m_lPlaybackSpeed;

	VARIANT_BOOL  m_bEnableOptions;
	VARIANT_BOOL  m_bEnableStocks;

	DWORD m_lPriceProvider;
};

#endif //__SIMPARAMETERS_H_
