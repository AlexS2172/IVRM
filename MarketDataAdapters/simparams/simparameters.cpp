// SimParameters.cpp : Implementation of CSimParameters
#include "stdafx.h"
#include "SimParams.h"
#include "SimParameters.h"

/////////////////////////////////////////////////////////////////////////////
// CSimParameters

STDMETHODIMP CSimParameters::get_VolumeMin(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lVolumeMin;
	return S_OK; 
}

STDMETHODIMP CSimParameters::put_VolumeMin(long newVal)
{
	m_lVolumeMin = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lVolumeMin, _T("VolumeMin"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_VolumeMax(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lVolumeMax;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_VolumeMax(long newVal)
{
	m_lVolumeMax = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lVolumeMax, _T("VolumeMax"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_PricePercent(double *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  (1. * double(m_lPriceDelta))/100000;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_PricePercent(double newVal)
{
	m_lPriceDelta = long(newVal*100000);
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lPriceDelta, _T("PriceDelta"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_BidPercent(double *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  (1. * double(m_lBidDelta))/100000;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_BidPercent(double newVal)
{
	m_lBidDelta = long(newVal*100000);
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lBidDelta, _T("BidDelta"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_AskPercent(double *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal = (1.* double(m_lAskDelta)) /100000;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_AskPercent(double newVal)
{
	m_lAskDelta = long(newVal*100000);
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lAskDelta, _T("AskDelta"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_BidSizeMin(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lBidSizeMin;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_BidSizeMin(long newVal)
{
	m_lBidSizeMin = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lBidSizeMin, _T("MinBidSize"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_BidSizeMax(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lBidSizeMax;

	return S_OK;
}

STDMETHODIMP CSimParameters::put_BidSizeMax(long newVal)
{
	m_lBidSizeMax = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lBidSizeMax, _T("MaxBidSize"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_AskSizeMin(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lAskSizeMin;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_AskSizeMin(long newVal)
{
	m_lAskSizeMin = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lAskSizeMin, _T("MinAskSize"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_AskSizeMax(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lAskSizeMax;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_AskSizeMax(long newVal)
{
	m_lAskSizeMax = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lAskSizeMax, _T("MaxAskSize"));
	}

	return S_OK;
}

STDMETHODIMP CSimParameters::get_IntervalMin(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lIntervalMin;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_IntervalMin(long newVal)
{
	m_lIntervalMin = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lIntervalMin, _T("MinInterval"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_IntervalMax(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lIntervalMax;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_IntervalMax(long newVal)
{
	m_lIntervalMax = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lIntervalMax, _T("MaxInterval"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_ConnectionString(BSTR *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_bsConnectionString.copy();

	return S_OK;
}

STDMETHODIMP CSimParameters::put_ConnectionString(BSTR newVal)
{
	m_bsConnectionString = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_bsConnectionString, _T("DBConnection"));
	}
	return S_OK;
}


STDMETHODIMP CSimParameters::get_StockIntervalMax(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lStockIntervalMax;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_StockIntervalMax(long newVal)
{
	m_lStockIntervalMax = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lStockIntervalMax, _T("MaxStockInterval"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_StockIntervalMin(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lStockIntervalMin;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_StockIntervalMin(long newVal)
{
	m_lStockIntervalMin = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lStockIntervalMin, _T("MinStockInterval"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_NonRealtimeIntervalMin(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lNonRealtimeIntervalMin;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_NonRealtimeIntervalMin(long newVal)
{
	m_lNonRealtimeIntervalMin = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lNonRealtimeIntervalMin, _T("MinGetInterval"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_NonRealtimeIntervalMax(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lNonRealtimeIntervalMax;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_NonRealtimeIntervalMax(long newVal)
{
	m_lNonRealtimeIntervalMax = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lNonRealtimeIntervalMax, _T("MaxGetInterval"));
	}
	return S_OK;
}


STDMETHODIMP CSimParameters::get_VolatilityUpdateIntervalMax(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lVolatilityIntervalMax;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_VolatilityUpdateIntervalMax(long newVal)
{
	m_lVolatilityIntervalMax = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lVolatilityIntervalMax, _T("MaxVolaInterval"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_VolatilityUpdateIntervalMin(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lVolatilityIntervalMin;

	return S_OK;
}

STDMETHODIMP CSimParameters::put_VolatilityUpdateIntervalMin(long newVal)
{
	m_lVolatilityIntervalMin = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lVolatilityIntervalMin, _T("MinVolaInterval"));
	}

	return S_OK;
}

STDMETHODIMP CSimParameters::get_VolatilityDelta(double *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_dVolatilityDelta;

	return S_OK;
}

STDMETHODIMP CSimParameters::put_VolatilityDelta(double newVal)
{
	m_dVolatilityDelta = newVal;

	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		DWORD dwVolaDelta = (DWORD)(m_dVolatilityDelta*1000);
		key.SetValue(dwVolaDelta, _T("Volatility"));
	}
	return S_OK;
}


STDMETHODIMP CSimParameters::get_TradeUpdateIntervalMin(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lTradeIntervalMin;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_TradeUpdateIntervalMin(long newVal)
{
	m_lTradeIntervalMin = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lTradeIntervalMin, _T("MinTradeInterval"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_TradeUpdateIntervalMax(long *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_lTradeIntervalMax;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_TradeUpdateIntervalMax(long newVal)
{
	m_lTradeIntervalMax = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lTradeIntervalMax, _T("MaxTradeInterval"));
	}
	return S_OK;
}


STDMETHODIMP CSimParameters::get_PlayBackDate(DATE *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  long(m_dtPlayback);
	return S_OK; 
}

STDMETHODIMP CSimParameters::put_PlayBackDate(DATE newVal)
{
	m_dtPlayback = long(newVal);
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		DWORD dwDate = (DWORD)m_dtPlayback;
		key.SetValue(dwDate, _T("PlayBackDate"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_PlayBackSpeed(LONG *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  long(m_lPlaybackSpeed);
	return S_OK; 
}

STDMETHODIMP CSimParameters::put_PlayBackSpeed(LONG newVal)
{
	m_lPlaybackSpeed = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_lPlaybackSpeed, _T("PlayBackSpeed"));	
	}
	return S_OK;
}


STDMETHODIMP CSimParameters::get_EnableOptions(VARIANT_BOOL *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal = m_bEnableOptions;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_EnableOptions(VARIANT_BOOL newVal)
{
	m_bEnableOptions = newVal;
	return S_OK;
}

STDMETHODIMP CSimParameters::get_EnableStocks(VARIANT_BOOL *pVal)
{
	if(!pVal) return E_POINTER;
	*pVal = m_bEnableStocks ;

	return S_OK;
}

STDMETHODIMP CSimParameters::put_EnableStocks(VARIANT_BOOL newVal)
{
	m_bEnableStocks = newVal;
	return S_OK;
}

STDMETHODIMP CSimParameters::get_DatabaseName(BSTR* pVal)
{
	if(!pVal) return E_POINTER;
	*pVal =  m_bsDatabaseName.copy();

	return S_OK;
}

STDMETHODIMP CSimParameters::put_DatabaseName(BSTR newVal)
{
	m_bsDatabaseName = newVal;
	EgLib::CEgRegKey key;
	if(ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, _T("Software\\Egar\\TrackData\\Simulator"),KEY_WRITE))
	{
		key.SetValue(m_bsDatabaseName, _T("DatabaseName"));
	}
	return S_OK;
}

STDMETHODIMP CSimParameters::get_PriceProvider(long* pVal)
{
	if(!pVal) return E_POINTER;
	*pVal = m_lPriceProvider;
	return S_OK;
}

STDMETHODIMP CSimParameters::put_PriceProvider(long newVal)
{
	m_lPriceProvider = newVal;
	return S_OK;
}