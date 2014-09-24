#include "stdafx.h"
#include "Settings.h"

_bstr_t	CSettings::m_bsExchange_DB_Path;
_bstr_t	CSettings::m_bsETS_DB_Path;
int	    CSettings::m_nTradesCount;
int	    CSettings::m_nWaitingTime;
int	    CSettings::m_nPublishDelayTime;
bool    CSettings::m_bIsLoaded = false;

bool CSettings::SetDefaults()
{
	m_nTradesCount = MIN_TRADES_COUNT;
	m_nWaitingTime = DEFAULT_WAITING_TIME;
	m_nPublishDelayTime = MIN_PUBLISH_DELAY;

    m_bIsLoaded = false;
    return true;
}

bool CSettings::LoadSettings()
{
    ISettingsPtr spGlobalSettings;
	BSTR    bsETS_DB_Path;
	CString bsExchange_DB_Path;
    HRESULT hr;

    bool bIsAllExisted = true;

    // to get ETS DB
    hr = spGlobalSettings.CreateInstance(__uuidof(Settings));
    if (FAILED(hr))
    {
        utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Global Settings object.");
    }
    
    hr = spGlobalSettings->get_DatabaseConnection(&bsETS_DB_Path);
    if(FAILED(hr))
    {
        utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to get Connection String.");
    }
    
    CSettings::m_bsETS_DB_Path = bsETS_DB_Path;
    SysFreeString(bsETS_DB_Path);
    
    LONG lResult;


	CXMLParamsHelper XMLParams;
	XMLParams.LoadXMLParams();

	_bstr_t sbsValue;

	// to get Exchange DB
	XMLParams.GetMainXMLString(SETTINGS_XML_KEY, SETTINGS_DB_SOURCE, &CSettings::m_bsExchange_DB_Path);
    //lResult = GetRegistrySetting(SETTINGS_DB_SOURCE, bsExchange_DB_Path);
    if (CSettings::m_bsExchange_DB_Path.length() == 0)
        bIsAllExisted = false;

    // to get Trades Count
	XMLParams.GetMainXMLLong(SETTINGS_XML_KEY, SETTINGS_TRADES_COUNT, (long*)&CSettings::m_nTradesCount);
    //lResult = GetRegistrySetting(SETTINGS_TRADES_COUNT, CSettings::m_nTradesCount);
    //if (lResult != ERROR_SUCCESS)
        //bIsAllExisted = false;
	if (CSettings::m_nTradesCount < MIN_TRADES_COUNT || CSettings::m_nTradesCount > MAX_TRADES_COUNT)
		CSettings::m_nTradesCount = MIN_TRADES_COUNT;

    // to get Sleep Time
	XMLParams.GetMainXMLLong(SETTINGS_XML_KEY, SETTINGS_SLEEP_TIME, (long*)&CSettings::m_nWaitingTime);
    //lResult = GetRegistrySetting(SETTINGS_SLEEP_TIME, CSettings::m_nWaitingTime);
    //if (lResult != ERROR_SUCCESS)
        //bIsAllExisted = false;
	if (CSettings::m_nWaitingTime < MIN_WAITING_TIME || CSettings::m_nWaitingTime > MAX_WAITING_TIME)
		CSettings::m_nWaitingTime = DEFAULT_WAITING_TIME;

    // to get Publish Delay
	XMLParams.GetMainXMLLong(SETTINGS_XML_KEY, SETTINGS_PUBLISH_DELAY, (long*)&CSettings::m_nPublishDelayTime);
    //lResult = GetRegistrySetting(SETTINGS_PUBLISH_DELAY, CSettings::m_nPublishDelayTime);
    //if (lResult != ERROR_SUCCESS)
        //bIsAllExisted = false;
	if (CSettings::m_nPublishDelayTime < MIN_PUBLISH_DELAY || CSettings::m_nPublishDelayTime > MAX_PUBLISH_DELAY)
		CSettings::m_nPublishDelayTime = MIN_PUBLISH_DELAY;

	   
    if (bIsAllExisted)
        m_bIsLoaded = true;

    return bIsAllExisted;
}

bool CSettings::SaveSettings()
{
	CXMLParamsHelper XMLParams;
	XMLParams.LoadXMLParams();

	XMLParams.SetMainXMLString(SETTINGS_XML_KEY, SETTINGS_DB_SOURCE, CSettings::m_bsExchange_DB_Path);
	XMLParams.SetMainXMLLong(SETTINGS_XML_KEY, SETTINGS_TRADES_COUNT, CSettings::m_nTradesCount);
	XMLParams.SetMainXMLLong(SETTINGS_XML_KEY, SETTINGS_SLEEP_TIME, CSettings::m_nWaitingTime);
	XMLParams.SetMainXMLLong(SETTINGS_XML_KEY, SETTINGS_PUBLISH_DELAY, CSettings::m_nPublishDelayTime);

	XMLParams.SaveMainXMLParams();

	//SetRegistrySetting(SETTINGS_DB_SOURCE, (const char*)CSettings::m_bsExchange_DB_Path);
	//SetRegistrySetting(SETTINGS_TRADES_COUNT, CSettings::m_nTradesCount);
	//SetRegistrySetting(SETTINGS_SLEEP_TIME, CSettings::m_nWaitingTime);
	//SetRegistrySetting(SETTINGS_PUBLISH_DELAY, CSettings::m_nPublishDelayTime);

    ISettingsPtr spGlobalSettings;
    HRESULT hr = spGlobalSettings.CreateInstance(__uuidof(Settings));
    if (FAILED(hr))
        utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create Settings object.");
    
    spGlobalSettings->DatabaseConnection = (LPCTSTR)CSettings::m_bsETS_DB_Path;

    return true;
}
