#include "StdAfx.h"
#include "Settings.h"

//------------------------------------------------------------------//
CSettings::CSettings(void)
{
}
//------------------------------------------------------------------//
CSettings::~CSettings(void)
{
}
//------------------------------------------------------------------//
CXmlSettings::CXmlSettings(void):m_xmlParams()
{
	m_xmlParams.LoadXMLParams();
}
//------------------------------------------------------------------//
CXmlSettings::~CXmlSettings(void)
{

}
//------------------------------------------------------------------//
void CXmlSettings::GetDBConnection(BSTR *bsConnect)
{
	try	
	{	
		m_xmlParams.GetDbConnect(bsConnect);
	}
	catch (...) 
	{
		ATLASSERT(false);
	}
}
//------------------------------------------------------------------//
/*@return the value of interpolation type
@default value is (1) - FLAT_EXTRAPOLATION*/

/*virtual*/
long CXmlSettings::curve_extrapolation_type()
{
	try
	{
		const	_bstr_t bstrSector = _bstr_t(TEXT("ETS\\MarketMaker\\Settings"));
		const	_bstr_t bstrKey = _bstr_t(TEXT("CurveExtrapolationType"));
		long	lReturn = 0;
		
		m_xmlParams.GetXMLLong(bstrSector, bstrKey, &lReturn, 1);

		return lReturn;	
	}
	catch(...)
	{
		ATLASSERT(false);
	}
	return 1L;
}
//------------------------------------------------------------------//
/*@return log directory*/
/*virtual*/
std::string	CXmlSettings::GetLogDirectory() 
{
	try 
	{
		const	_bstr_t bstrSector = _bstr_t(TEXT("ETS\\Asp\\GeneralSettings"));
		const	_bstr_t bstrKey = _bstr_t(TEXT("LogDirectory"));
		_bstr_t bsXmlValue("");

		m_xmlParams.GetMainXMLString(bstrSector, bstrKey, &bsXmlValue);
		
		if (bsXmlValue.length() > 0) {

			TCHAR user_name[128] = {0};
			DWORD user_name_length = 128;

			::GetUserName(user_name, &user_name_length);

			bsXmlValue = bsXmlValue + _bstr_t("\\") + _bstr_t(user_name);
			
			return (char*)bsXmlValue;
		};
		
		return std::string("");
	}
	catch (...) 
	{
		ATLASSERT(false);
	}
	
	return std::string("");
}
//------------------------------------------------------------------//
/*@return the value of extrapolation type
@default value is (1) - FLAT_FORWARD_INTERPOLATION*/

/*virtual*/
long
CXmlSettings::curve_interpolation_type(){
	try{
		const	_bstr_t bstrSector = _bstr_t(TEXT("ETS\\MarketMaker\\Settings"));
		const	_bstr_t bstrKey = _bstr_t(TEXT("CurveInterpolationType"));
		long	lReturn = 0;

		m_xmlParams.GetXMLLong(bstrSector, bstrKey, &lReturn, 1);

		return lReturn;	
	}
	catch(...){
		ATLASSERT(false);
	};
	return 1L;
};
//------------------------------------------------------------------//
long CXmlSettings::GetGreeksCalculationMode()
{
	try	
	{	
		const	_bstr_t bstrSector = _bstr_t(TEXT("ETS\\MarketMaker\\Settings"));
		const	_bstr_t bstrKey = _bstr_t(TEXT("UseTheoVolatility"));
		long	lReturn = 0;

		m_xmlParams.GetXMLLong(bstrSector, bstrKey, &lReturn, 0);

		return lReturn;
	}
	catch (...) 
	{
		ATLASSERT(false);
	}
	return 0;
};
//------------------------------------------------------------------//
long CXmlSettings::GetGreeksCalculationModel()
{
	try	
	{	
		const	_bstr_t bstrSector = _bstr_t(TEXT("ETS\\MarketMaker\\Settings"));
		const	_bstr_t bstrKey = _bstr_t(TEXT("CalcModel"));
		long	lReturn = 0;

		m_xmlParams.GetXMLLong(bstrSector, bstrKey, &lReturn, 2);

		return lReturn;
	}
	catch (...) 
	{
		ATLASSERT(false);
	}
	return 0;
};
//------------------------------------------------------------------//
bool CXmlSettings::GetUseTimePrecision()
{
	try	
	{	
		const	_bstr_t bstrSector = _bstr_t(TEXT("ETS\\MarketMaker\\Settings"));
		const	_bstr_t bstrKey = _bstr_t(TEXT("UseTTE"));
		long	lReturn = 0;

		m_xmlParams.GetXMLLong(bstrSector, bstrKey, &lReturn, 1);

		return (lReturn > 0 ? true : false);
	}
	catch (...) 
	{
		ATLASSERT(false);
	}
	return true;
};
//------------------------------------------------------------------//
std::string CXmlSettings::GetAssetListFileName()
{
	try	
	{	
		const	_bstr_t bstrSector = _bstr_t(TEXT("ETS\\IvRmServer"));
		const	_bstr_t bstrKey = _bstr_t(TEXT("AssetListFile"));
		_bstr_t bsXmlValue;

		m_xmlParams.GetXMLString(bstrSector, bstrKey, &bsXmlValue);

		std::string retValue = (char*)bsXmlValue;

		return retValue;
		
	}
	catch (...) 
	{
		ATLASSERT(false);
	}
	return std::string("");
};
//------------------------------------------------------------------//
/*virtual*/
/*@return log level for current group from main xml*/
long CXmlSettings::GetLogLevel()
{
	try
	{
		const	_bstr_t bstrSector = _bstr_t(TEXT("ETS\\Asp\\DebugLevel"));
		
		_bstr_t bstrKey;
		m_xmlParams.GetUserGroup(&bstrKey.GetBSTR());

		long	lReturn = 0;

		//default log level is LogInfo
		m_xmlParams.GetMainXMLLong(bstrSector, bstrKey, &lReturn, 5); 

		return lReturn;

	}
	catch (...)
	{
		ATLASSERT(FALSE);
	};
	return 5L;
};
//------------------------------------------------------------------//
/*virtual*/
/*@return log lifetime in days for current group from main xml*/
/*default value is 2 days*/
long CXmlSettings::GetLogLifetime()
{
	try
	{
		const	_bstr_t bstrSector = _bstr_t(TEXT("ETS\\Asp\\LogLifetime"));

		_bstr_t bstrKey;
		m_xmlParams.GetUserGroup(&bstrKey.GetBSTR());

		long	lReturn = 0;

		//default log level is LogInfo
		m_xmlParams.GetMainXMLLong(bstrSector, bstrKey, &lReturn, 2); 

		return lReturn;

	}
	catch (...)
	{
		ATLASSERT(FALSE);
	};
	return 2L;
};
//------------------------------------------------------------------//
/*virtual*/
/*@return true if need to process trade analytics*/
/*@info read setting from user.xml */
bool CXmlSettings::GetProcessTradeForAnalytics()
{
	try{
		const	_bstr_t bstrSector = _bstr_t(TEXT("ETS\\IvRmServer"));
		const	_bstr_t bstrKey = _bstr_t(TEXT("ProcessTradeForAnalytics"));

		long	lReturn = 0;

		//default value is false (0)
		m_xmlParams.GetXMLLong(bstrSector, bstrKey, &lReturn, 0); 

		return (lReturn != 0);

	}
	catch (_com_error& err){
		std::ostringstream out_stream;
		out_stream<<"Exception occured while reading setting [ProcessTradeForAnalytics]";
		TRACE_COM_ERROR(LOG4CPLUS_ERROR, VS::Log, err, std::string(out_stream.str()));
		return false;
	}
	catch (...){
		std::ostringstream out_stream;
		out_stream<<"Unknown C++ exception occured while reading setting [ProcessTradeForAnalytics]";
		TRACE_UNKNOWN_ERROR(LOG4CPLUS_ERROR, VS::Log, std::string(out_stream.str()));
		return false;
	}
	return false;
};
//------------------------------------------------------------------//