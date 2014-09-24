// MMParameters.cpp : Implementation of CMMParameters
#include "stdafx.h"
#include "ISEProvider.h"
#include "MMParameters.h"

/////////////////////////////////////////////////////////////////////////////
// CMMParameters
/*
STDMETHODIMP CMMParameters::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IMMParameters
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
*/

STDMETHODIMP CMMParameters::get_UnderlyingSymbol(BSTR* pVal)
{
	if (!pVal) 
		return E_POINTER;
	
	ObjectLock lock(this);

	*pVal = SysAllocString(m_bsUnd);

	return S_OK;
}

STDMETHODIMP CMMParameters::put_UnderlyingSymbol(BSTR newVal)
{
	ObjectLock lock(this);

	m_bsUnd = newVal;

	return S_OK;
}

STDMETHODIMP CMMParameters::get_InstrumentGroup(InstrumentGroupEnum* pVal)
{
	if (!pVal) 
		return E_POINTER;
	
	ObjectLock lock(this);

	*pVal = (InstrumentGroupEnum)series.instrument_group_c;

	return S_OK;
}

STDMETHODIMP CMMParameters::put_InstrumentGroup(InstrumentGroupEnum newVal)
{
	ObjectLock lock(this);

	series.instrument_group_c = (unsigned char)newVal;

	return S_OK;
}

STDMETHODIMP CMMParameters::get_ExpirationGroup(ExpirationGroupEnum* pVal)
{
	if (!pVal) 
		return E_POINTER;
	
	ObjectLock lock(this);

	*pVal = (ExpirationGroupEnum)expiration_group_c;

	return S_OK;
}

STDMETHODIMP CMMParameters::put_ExpirationGroup(ExpirationGroupEnum newVal)
{
	ObjectLock lock(this);
	
	expiration_group_c = (unsigned char)newVal;

	return S_OK;
}

STDMETHODIMP CMMParameters::get_StrikePriceGroup(StrikePriceGroupEnum* pVal)
{
	if (!pVal) 
		return E_POINTER;
	
	ObjectLock lock(this);

	*pVal = (StrikePriceGroupEnum)strike_price_group_c;

	return S_OK;
}

STDMETHODIMP CMMParameters::put_StrikePriceGroup(StrikePriceGroupEnum newVal)
{
	ObjectLock lock(this);
	
	strike_price_group_c = (unsigned char)newVal;

	return S_OK;
}

STDMETHODIMP CMMParameters::get_TickWorseVolume(long Tick, long* pVal)
{
	if (!pVal) 
		return E_POINTER;

	if(Tick < 1 || Tick > 8)
		return Error("Invalid Tick (valid values = 1..8)");
	
	ObjectLock lock(this);

	*pVal = mm_parameters.tick_worse_volume_an[Tick - 1];

	return S_OK;
}

STDMETHODIMP CMMParameters::put_TickWorseVolume(long Tick, long newVal)
{
	if(Tick < 1 || Tick > 8)
		return Error("Invalid Tick (valid values = 1..8)");

	ObjectLock lock(this);
	
	mm_parameters.tick_worse_volume_an[Tick - 1] = newVal;

	return S_OK;
}

STDMETHODIMP CMMParameters::get_StepUpAfterRegenBuffer(long* pVal)
{
	if (!pVal) 
		return E_POINTER;
	
	ObjectLock lock(this);

	*pVal = mm_parameters.step_up_after_regen_buffer_n;

	return S_OK;
}

STDMETHODIMP CMMParameters::put_StepUpAfterRegenBuffer(long newVal)
{
	ObjectLock lock(this);
	
	mm_parameters.step_up_after_regen_buffer_n = newVal;

	return S_OK;
}

STDMETHODIMP CMMParameters::get_IseMMTradeLimitAbsolute(long* pVal)
{
	if (!pVal) 
		return E_POINTER;
	
	ObjectLock lock(this);

	*pVal = mm_parameters.isemm_trade_limit_absolute_n;

	return S_OK;
}

STDMETHODIMP CMMParameters::put_IseMMTradeLimitAbsolute(long newVal)
{
	ObjectLock lock(this);
	
	mm_parameters.isemm_trade_limit_absolute_n = newVal;

	return S_OK;
}

STDMETHODIMP CMMParameters::get_FirmTradeLimitAbsolute(long* pVal)
{
	if (!pVal) 
		return E_POINTER;
	
	ObjectLock lock(this);

	*pVal = mm_parameters.firm_trade_limit_absolute_n;

	return S_OK;
}

STDMETHODIMP CMMParameters::put_FirmTradeLimitAbsolute(long newVal)
{
	ObjectLock lock(this);

	mm_parameters.firm_trade_limit_absolute_n = newVal;

	return S_OK;
}

STDMETHODIMP CMMParameters::get_FarMmTradeLimitFraction(long* pVal)
{
	if (!pVal) 
		return E_POINTER;
	
	ObjectLock lock(this);

	*pVal = mm_parameters.firm_trade_limit_fraction_c;

	return S_OK;
}

STDMETHODIMP CMMParameters::put_FarMmTradeLimitFraction(long newVal)
{
	ObjectLock lock(this);
	
	mm_parameters.firm_trade_limit_fraction_c = newVal;

	return S_OK;
}

STDMETHODIMP CMMParameters::get_IseMmTradeLimitFraction(long* pVal)
{
	if (!pVal) 
		return E_POINTER;
	
	ObjectLock lock(this);

	*pVal = mm_parameters.isemm_trade_limit_fraction_c;

	return S_OK;
}

STDMETHODIMP CMMParameters::put_IseMmTradeLimitFraction(long newVal)
{
	ObjectLock lock(this);
	
	mm_parameters.isemm_trade_limit_fraction_c = newVal;

	return S_OK;
}

STDMETHODIMP CMMParameters::get_FirmTradeLimitFraction(long* pVal)
{
	if (!pVal) 
		return E_POINTER;
	
	ObjectLock lock(this);

	*pVal = mm_parameters.firm_trade_limit_fraction_c;

	return S_OK;
}

STDMETHODIMP CMMParameters::put_FirmTradeLimitFraction(long newVal)
{
	ObjectLock lock(this);
	
	mm_parameters.firm_trade_limit_fraction_c = newVal;

	return S_OK;
}

STDMETHODIMP CMMParameters::get_FarMMTradeLimitAbsolute(long* pVal)
{
	if (!pVal) 
		return E_POINTER;
	
	ObjectLock lock(this);

	*pVal = mm_parameters.farmm_trade_limit_absolute_n;

	return S_OK;
}

STDMETHODIMP CMMParameters::put_FarMMTradeLimitAbsolute(long newVal)
{
	ObjectLock lock(this);
	
	mm_parameters.farmm_trade_limit_absolute_n = newVal;

	return S_OK;
}

STDMETHODIMP CMMParameters::get_DerivedOrderMaxSize(long Tick, long* pVal)
{
	if (!pVal) 
		return E_POINTER;

	if(Tick < 1 || Tick > 8)
		return Error("Invalid Tick (valid values = 1..8)");
	
	ObjectLock lock(this);

	*pVal = pmm_parameters.derived_order_max_size_an[Tick - 1];

	return S_OK;
}

STDMETHODIMP CMMParameters::put_DerivedOrderMaxSize(long Tick, long newVal)
{
	if(Tick < 1 || Tick > 8)
		return Error("Invalid Tick (valid values = 1..8)");

	ObjectLock lock(this);
	
	pmm_parameters.derived_order_max_size_an[Tick - 1] = newVal;

	return S_OK;
}

STDMETHODIMP CMMParameters::get_MatchAwayMarketMaxSize(long Tick, long* pVal)
{
	if (!pVal) 
		return E_POINTER;

	if(Tick < 1 || Tick > 8)
		return Error("Invalid Tick (valid values = 1..8)");
	
	ObjectLock lock(this);

	*pVal = pmm_parameters.match_away_market_max_size_an[Tick - 1];

	return S_OK;
}

STDMETHODIMP CMMParameters::put_MatchAwayMarketMaxSize(long Tick, long newVal)
{
	if(Tick < 1 || Tick > 8)
		return Error("Invalid Tick (valid values = 1..8)");

	ObjectLock lock(this);
	
	pmm_parameters.match_away_market_max_size_an[Tick - 1] = newVal;

	return S_OK;
}
