// CalculationSettings.cpp : Implementation of CCalculationSettings

#include "stdafx.h"
#include "CalculationSettings.h"

//--------------------------------------------------------------------------------//
STDMETHODIMP CCalculationSettingsAtom::get_GreeksCalculationMode(LONG* pRetVal)
{
	if (pRetVal == NULL)
		return E_POINTER;

	try
	{
		*pRetVal = static_cast<LONG>(CCalculationSettingsSingleton::Instance()->GetGreeksCalculationMode());
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	};
	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP CCalculationSettingsAtom::put_GreeksCalculationMode(LONG newVal)
{
	try
	{
		CCalculationSettingsSingleton::Instance()->SetGreeksCalculationMode(static_cast<IvRmCore::GreeksCalculationModeEnum>(newVal));
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	};
	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP CCalculationSettingsAtom::get_GreeksCalculationModel(LONG* pRetVal)
{
	if (pRetVal == NULL)
		return E_POINTER;
	try
	{
		*pRetVal = static_cast<LONG>(CCalculationSettingsSingleton::Instance()->GetGreeksCalculationModel());
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	};

	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP CCalculationSettingsAtom::put_GreeksCalculationModel(LONG newVal)
{
	try
	{
		CCalculationSettingsSingleton::Instance()->SetGreeksCalculationModel(newVal);
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	}
	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP CCalculationSettingsAtom::get_UseTimePrecision(LONG* pRetVal)
{
	if (pRetVal == NULL)
		return E_POINTER;
	try
	{
		*pRetVal = static_cast<LONG>(CCalculationSettingsSingleton::Instance()->GetUseTimePrecision());
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	};

	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP CCalculationSettingsAtom::put_UseTimePrecision(LONG newVal)
{
	try
	{
		CCalculationSettingsSingleton::Instance()->SetUseTimePrecision(newVal > 0 ? true : false);
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	}
	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP CCalculationSettingsAtom::get_UseTheoVolaIfNoBid(LONG* pRetVal)
{
	if (pRetVal == NULL)
		return E_POINTER;
	try
	{
		*pRetVal = static_cast<LONG>(CCalculationSettingsSingleton::Instance()->GetUseTheoVolaIfNoBid());
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	};

	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP CCalculationSettingsAtom::put_UseTheoVolaIfNoBid(LONG newVal)
{
	try
	{
		CCalculationSettingsSingleton::Instance()->SetUseTheoVolaIfNoBid(newVal > 0 ? true : false);
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	}
	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP CCalculationSettingsAtom::get_UseTheoVolaIfBadMarketVola(LONG* pRetVal)
{
	if (pRetVal == NULL)
		return E_POINTER;
	try
	{
		*pRetVal = static_cast<LONG>(CCalculationSettingsSingleton::Instance()->GetUseTheoVolaIfBadMarketVola());
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	};

	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP CCalculationSettingsAtom::put_UseTheoVolaIfBadMarketVola(LONG newVal)
{
	try
	{
		CCalculationSettingsSingleton::Instance()->SetUseTheoVolaIfBadMarketVola(newVal > 0 ? true : false);
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	}
	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP CCalculationSettingsAtom::get_PriceTolerance(DOUBLE* pRetVal)
{
	if (pRetVal == NULL)
		return E_POINTER;
	try
	{
		*pRetVal = static_cast<DOUBLE>(CCalculationSettingsSingleton::Instance()->GetPriceTolerance());
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	};

	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP CCalculationSettingsAtom::put_PriceTolerance(DOUBLE newVal)
{
	try
	{
		CCalculationSettingsSingleton::Instance()->SetPriceTolerance(newVal);
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	}
	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP CCalculationSettingsAtom::get_PriceRoundingRule(LONG* pRetVal)
{
	if (pRetVal == NULL)
		return E_POINTER;
	try
	{
		*pRetVal = static_cast<LONG>(CCalculationSettingsSingleton::Instance()->GetPriceRoundingRule());
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	};

	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP CCalculationSettingsAtom::put_PriceRoundingRule(LONG newVal)
{
	try
	{
		CCalculationSettingsSingleton::Instance()->SetPriceRoundingRule(newVal);
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	}
	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP
CCalculationSettingsAtom::put_Normal(DOUBLE Value){

	try{
		CCalculationSettingsSingleton::Instance()->setNormal(double(Value));
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	};
	return S_OK;

};
//--------------------------------------------------------------------------------//
STDMETHODIMP 
CCalculationSettingsAtom::get_Normal(DOUBLE* RetVal){
	
	if (RetVal == NULL)
		return E_POINTER;

	try{
		*RetVal = CCalculationSettingsSingleton::Instance()->getNormal();
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	};

	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP 
CCalculationSettingsAtom::get_CurveExtrapolationType(LONG* pRetVal){
	if (pRetVal == NULL)
		return E_POINTER;

	try{
		*pRetVal = CCalculationSettingsSingleton::Instance()->extrapolation_type();
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	};
	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP 
CCalculationSettingsAtom::put_CurveExtrapolationType(LONG newVal){
	try {
		CCalculationSettingsSingleton::Instance()->set_extrapolation_type(newVal);
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	};
	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP 
CCalculationSettingsAtom::get_CurveInterpolationType(LONG* pRetVal){
	if (pRetVal == NULL)
		return E_POINTER;

	try{
		*pRetVal = CCalculationSettingsSingleton::Instance()->interpolation_type();
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	};
	return S_OK;
};
//--------------------------------------------------------------------------------//
STDMETHODIMP 
CCalculationSettingsAtom::put_CurveInterpolationType(LONG newVal){
	try {
		CCalculationSettingsSingleton::Instance()->set_interpolation_type(newVal);
	}
	catch (_com_error& err){
		TRACE_COM_ERROR(err);
		return E_FAIL;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		return E_FAIL;
	};
	return S_OK;
};
//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//
// CCalculationSettings

