#ifndef __IVDATALOADER_INL__
#define __IVDATALOADER_INL__

namespace EODIV
{

template <class _RequestData, class _ResultData>
void CIvDataLoader<_RequestData,_ResultData>::_Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	EODSM::CSmSettingsMgrPtr spSettingsMgr(pSettingsMgr);
	if(!pSettingsMgr)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Invalid settings manager."));
		throw exception(_T("Invalid settings manager."));
	}

	if (GetSessionHandle() != NULL)
		_Disconnect();

	CEgAccount aAccount;
	CEgHttpConnectionParam aParams;
	// initalize connection parameters
	aParams.m_strServerAddress = pSettingsMgr->IVolalilityServerName();
	aParams.m_lServerPort = pSettingsMgr->IVolatilityServerPort();
	aParams.m_enProxyAuth = static_cast<enumEgHttpProxyAuth>(pSettingsMgr->ProxyAuthType());
	aParams.m_strProxyAddress = pSettingsMgr->ProxyName();
	aParams.m_lProxyPort = pSettingsMgr->ProxyPort();
	aParams.m_enProxySettings = static_cast<enumEgHttpProxySettings>(pSettingsMgr->UseProxy());
	aParams.m_ProxyAcount.m_strLogin = pSettingsMgr->ProxyLogin();
	aParams.m_ProxyAcount.m_strPassword = pSettingsMgr->ProxyPassword();

	aAccount.m_strLogin = pSettingsMgr->IVolatilityLogin();
	aAccount.m_strPassword = pSettingsMgr->IVolatilityPassword();

	m_nReconnectAttempt = pSettingsMgr->IvReconnectAttempt();
	m_nWaitInterval = pSettingsMgr->IvWaitInterval();

	InitIvSession(aParams, aAccount);

	// getting SID for query to IV
	{
		CAutoLock lock(m_Lock);
		m_strSID = Login();
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

template <class _RequestData, class _ResultData>
void CIvDataLoader<_RequestData,_ResultData>::_Disconnect()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	{
		CAutoLock lock(m_Lock);
		m_strSID.Empty();
	}
	CloseSession();
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

template < class _RequestData, class _ResultData >
void CIvDataLoader< _RequestData, _ResultData >::RequestIvData(_RequestData* pRequest)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	
	try
	{
		CString strURL;
		long nTry = 0;
		strURL.Empty();
		
		while(true)
		{
			try
			{
				if(nTry)
				{
					DWORD dwRes = ::WaitForSingleObject(_StopEventHandle(), 1000 * m_nWaitInterval);
					if(dwRes == WAIT_OBJECT_0)
						throw EODCO::CEodStopException();
				}

				_Connect(pRequest->SettingsMgr());

				if(strURL.IsEmpty())
				{
					try
					{
						strURL = pRequest->GetURL();
						Trace(EODCO::enTrLogDebug,__FUNCTION__,strURL);
					}
					catch (length_error)
					{
						//all ok - we already send request on all underlings
						break;
					}
				}

				CString strTmp;
				strTmp = strURL + _GetSidStr();
				
				Trace(EODCO::enTrLogDebug,__FUNCTION__, _T("OpenUrl: %s"), (LPCSTR)strTmp );
				OpenURL(strURL + _GetSidStr());

				CEgString strData;
				_ReadAllData(strData);

				_CheckResultFormat(strData);
				
				//Trace(EODCO::enTrLogDebug,__FUNCTION__, (LPCSTR)strData);

				long nPos = 0L;
				while(_ResultData spResult = _ResultData::element_type::Parse(strData, nPos))
				{
					OnIvData(spResult);
				}

				_Disconnect();
				nTry = 0;
				strURL.Empty();
			}
			catch(CEgHttpError& e)
			{
				Trace(EODCO::enTrLogError,__FUNCTION__,_T("Http exception : %d, %s for: %s "),e.Error(), (LPCSTR)e.Description(), LPCSTR((CString)(strURL + _GetSidStr())));
				_Disconnect();

				if(++nTry > m_nReconnectAttempt)
				{
					throw exception(e.Description());
				}
				else
				{
					Report(_StepMask(),enRpMtWarning, _T("Error connect to IVolatility : %s. Try to recconnect"), e.Description());
				}
			}
		}
	}
	catch(EODCO::CEodStopException)
	{
		_SetCancel();
		_Disconnect();
	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception : %s, %s"), (LPCSTR)typeid(e).name(), (LPCSTR)e.what());
		_Disconnect();
		throw exception(e);
	}
	if(_IsCancel())
		throw EODCO::CEodStopException();

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

template <class _RequestData, class _ResultData>
void CIvDataLoader<_RequestData,_ResultData>::_CheckResultFormat(CEgString& strData)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	if( strData.Find(_T("BEGIN IV DATA")) == -1 || strData.Find(_T("END IV DATA")) == -1)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Unknown response from IVolatility"));
		throw logic_error(_T("Unknown response from IVolatility"));
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
// Read all data. Use sequental calls of ReadData inside.
template <class _RequestData, class _ResultData>
void CIvDataLoader<_RequestData,_ResultData>::_ReadAllData(CEgString& rstrData)
{
	rstrData.Empty();

	CEgString strPiece;
	bool bDataPresent = false;
	do 
	{
		if(_IsCancel())
			throw EODCO::CEodStopException();

		bDataPresent = ReadData(strPiece);
		rstrData += strPiece;
	}
	while (bDataPresent);
	CheckErrorMessage(rstrData);
}

template < class _RequestData, class _ResultData >
void CIvDataLoader< _RequestData, _ResultData >::CancelRequest()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	_SetCancel();
	_Disconnect();
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

template < class _RequestData, class _ResultData >
CString CIvDataLoader< _RequestData, _ResultData >::_GetSidStr()
{
	CString sSid = _T("&SID=");
	{
		CAutoLock lock(m_Lock);
		sSid += m_strSID;
	}
	return sSid;
}

template < class _RequestData, class _ResultData >
bool CIvDataLoader< _RequestData, _ResultData >::_IsCancel()
{
	CAutoLock lock(m_Lock);
	return m_bIsCancel;
}

template < class _RequestData, class _ResultData >
void CIvDataLoader< _RequestData, _ResultData >::_SetCancel()
{
	CAutoLock lock(m_Lock);
	m_bIsCancel = true;
}

};

#endif// __IVDATALOADER_INL__