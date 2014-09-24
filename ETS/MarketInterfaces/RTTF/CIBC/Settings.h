#ifndef __SETTINGS_H__
#define __SETTINGS_H__

class CSettings
{
public:

	static bool SetDefaults();
    
    static bool IsLoaded() { return m_bIsLoaded; }

	static bool LoadSettings() throw (...);
	static bool SaveSettings() throw (...);

    static _bstr_t m_bsExchange_DB_Path;
    static _bstr_t m_bsETS_DB_Path;
	static int	   m_nTradesCount;
	static int	   m_nWaitingTime;
	static int	   m_nPublishDelayTime;

private:
    static bool m_bIsLoaded;
};


template <class T>
LONG GetRegistrySetting(LPCTSTR lpszValue, T& Value) throw(...)
{
   	CEgRegKey key;
	LONG lResult = key.Open(HKEY_LOCAL_MACHINE, SETTINGS_REGISTRY_KEY, KEY_READ);
    if (lResult == ERROR_SUCCESS)
    {
       	lResult = key.QueryValue(Value, lpszValue);
        key.Close();
    }
 
    return lResult;
}

//--------------------------------------------------------------------------------------------------------------//
template <class T>
void SetRegistrySetting(LPCTSTR lpszValue, const T& Value) throw(...)
{
   	CEgRegKey key;
	LONG lResult = key.Open(HKEY_LOCAL_MACHINE, SETTINGS_REGISTRY_KEY, KEY_WRITE);
	if (lResult != ERROR_SUCCESS )
        lResult = key.Create(HKEY_LOCAL_MACHINE, SETTINGS_REGISTRY_KEY);

    HRESULT hr = HRESULT_FROM_WIN32(lResult);
	if ( SUCCEEDED(hr))
	{
        try
	    {
		    lResult = key.SetValue(Value, lpszValue);
			if (lResult)
            {
                _bstr_t bsMsg = L"Failed to write <";
                bsMsg += lpszValue;
                bsMsg += L"> setting value to the registry.";
                utils::ThrowErrorNoSetErrorInfo(HRESULT_FROM_WIN32(lResult), bsMsg);
            }
    	}
	    catch (_com_error&)
		{
            USES_CONVERSION;
//		    TraceError(e.Error(), W2T(e.Description()) );
            throw;
		}
	
		key.Close();
	}
}
#endif //__SETTINGS_H__