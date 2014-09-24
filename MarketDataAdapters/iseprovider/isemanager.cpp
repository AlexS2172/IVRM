#include "StdAfx.h"

#include "ISEManager.h"
/*-------------------------------------------------------------------------------------------*/

HRESULT	CISEManagerBase::RealConnect()
{
	HRESULT hr = S_OK;

	IseTrace(enInfo, "Trying to connect...");

	for(int iGw = 0; iGw < m_vecGateways.size(); iGw++)
	{
		hr = S_OK;

		try
		{
			Logon(m_vecGateways[iGw].m_sAddr.c_str(), m_vecGateways[iGw].m_sPort.c_str(), 
				m_mapBinToAccounts[1], "ETS");
		}
		catch(CISEException& e)
		{
			hr = e.GetHRESULT();

			IseTrace(enError, "Failed to connect.");
		}

		if(SUCCEEDED(hr))
		{
			IseTrace(enInfo, "Connected.");
			break;
		}

		//To manage an unsuccessful logon:
		//Wait 30 seconds before attempting to log on again.
		if(iGw != m_vecGateways.size() - 1)
		{
			if(IsLogonEnabled())
			{
				IseTrace(enInfo, "Waiting 30 seconds before trying next gateway.");
			}

			if(LogonSleep(30000) == false)
			{
				IseTrace(enInfo, "Operation cancelled.");
				break;
			}
		}
	}

	return hr;
}


HRESULT	CISEManagerBase::Connect()
{
	CAutoLock	ConnectLock(&m_ConnectLock);

	if(m_vecGateways.size() == 0)
	{
		CISESettingManager	SettingsManager;

		DWORD dwErr = SettingsManager.GetGateways(m_vecGateways);
		if (dwErr != ERROR_SUCCESS)
		{
			return ComError(E_FAIL, L"Fail to read 'Gateways' settings from the registry.");
		}
		
		dwErr = SettingsManager.GetAccounts(m_mapBinToAccounts);
		if (dwErr != ERROR_SUCCESS)
		{
			return ComError(E_FAIL, L"Fail to read 'Accounts' settings from the registry.");	
		}

		dwErr = SettingsManager.GetWorkTime(&m_liLogon, &m_liLogoff);
		if (dwErr != ERROR_SUCCESS)
		{
			return ComError(E_FAIL, L"Fail to read 'Work Time' settings from the registry.");	
		}
		
		if(dwErr)
		{
			return ComError(E_FAIL, L"Fail to read settings from the registry.");
		}

		if(m_mapBinToAccounts.size() == 0)
			return ComError(E_FAIL, L"No valid bins found.");

		if(m_vecGateways.size() == 0)
			return ComError(E_FAIL, L"No valid gateways found.");

		const vector<CAccount>& vecAccounts = m_mapBinToAccounts[1];
		if(vecAccounts.size() == 0)
			return ComError(E_FAIL, L"No valid accounts found for bin.");
	}

	if(m_dwRefs++ == 0)
	{
		EnableLogon();

		QueueWorkTimeEvent();

		QueueConnectEvent(WAIT_BEFORE_CONNECT_TIME);
	}

	EgTrace("CISEManagerBase::Connect");

	return S_OK;
}

HRESULT	CISEManagerBase::Disconnect()
{
	CAutoLock	ConnectLock(&m_ConnectLock);


	EgTrace("CISEManagerBase::Disconnect");
	
	if(m_dwRefs == 0)
		return S_OK;

	if(--m_dwRefs != 0)
		return S_OK;

	EnableLogon(false);

	try
	{
		Logoff();
	}
	catch(CISEException&)
	{
	}

	m_bIseConnected = false;

	IseTrace(enInfo, "Disconnected.");

	return S_OK;
}

/*-------------------------------------------------------------------------------------------*/

void CISEManager::OnQuoteBBO(const CSymbolParams& InstrumentParams, 
							 const _EventTypeEnum RequestType,
							 const CQuoteBBO&	  BBO)
{
	CPriceProviderManagerObject::OnQuoteBBO(InstrumentParams, RequestType, BBO);
}

void CISEManager::OnQuoteRejection(const CQuoteParams& Params, 
					 			   const string& sReason)
{
	CMMManagerObject::OnQuoteRejection(Params, sReason);
}

void CISEManager::OnQuoteStepUp(const CQuoteParams& Params)
{
	CMMManagerObject::OnQuoteStepUp(Params);
};
	
void CISEManager::OnQuoteTickWorse(const CQuoteParams& Params,
								  const unsigned short uiTickWorseLevel,
								  const string& sReason)
{
	CMMManagerObject::OnQuoteTickWorse(Params, uiTickWorseLevel, sReason);
};

void CISEManager::OnMessage(const ISEInfoTypeEnum Type, const string& sText)
{
	CISETracerManagerObject::OnMessage(Type, sText);
}

void CISEManager::OnHeartbeat(const long lInstance)
{
	CISETracerManagerObject::OnHeartbeat(lInstance);
}

void CISEManager::OnDeal(const CISEDeal& Deal)
{
	CMMManagerObject::OnDeal(Deal);
}

void CISEManager::OnTrade(const CISETrade& Trade)
{
	CMMManagerObject::OnTrade(Trade);
}

void CISEManager::OnConnected(const DWORD dwSession)
{
	CStructureProviderManagerObject::OnConnected(dwSession);
	CPriceProviderManagerObject::OnConnected(dwSession);
	CMMManagerObject::OnConnected(dwSession);
	CMMManagerPriceObject::OnConnected(dwSession);
}

void CISEManager::OnDisconnected(const DWORD dwSessionID)
{
	CAutoLock		Entry(&m_DisconnectLock);

	if(dwSessionID > m_dwPrevSessionID)
	{
		m_dwPrevSessionID = dwSessionID;
		QueueReconnectEvent(0);
	}
}

CISEManager::CISEManager()
{
	m_dwPrevSessionID = 0;
	CISEManagerBase::Start();
}

CISEManager::~CISEManager()
{
	EgTrace("~CISEManager");
	CISEManagerBase::Stop();
	CStructureProviderManagerObject::Stop();
	CPriceProviderManagerObject::Stop();
	CMMManagerObject::Stop();
	CMMManagerPriceObject::Stop();
	CISETracerManagerObject::Stop();
	EgTrace("~CISEManager - all stopped");
}

/*-------------------------------------------------------------------------------------------*/

HRESULT	CMMManagerObject::SetMMParameters(IMMParameters* pParameters, bool bPMM)
{
	HRESULT hr;

	_bstr_t bsUnd;
	InstrumentGroupEnum		IG;
	ExpirationGroupEnum		EG;
	StrikePriceGroupEnum	SPG;
	unsigned short pusTickWorseVolume[8];
	unsigned short pusDerivedOrderMaxSize[8];
	unsigned short pusMatchAwayMarketMaxSize[8];
	long lStepUpAfterRegenBuffer;
	long lIseMMTradeLimitAbsolute;
 	long lFirmTradeLimitAbsolute;
	long lFarMMTradeLimitAbsolute;
	long lIseMmTradeLimitFraction;
	long lFirmTradeLimitFraction;
	long lFarMmTradeLimitFraction;

	BSTR bsTemp;
	hr = pParameters->get_UnderlyingSymbol(&bsTemp);
	if(FAILED(hr))
		return hr;
	bsUnd = bsTemp;
	SysFreeString(bsTemp);
	
	hr = pParameters->get_InstrumentGroup(&IG);
	if(FAILED(hr))
		return hr;
	hr = pParameters->get_ExpirationGroup(&EG);
	if(FAILED(hr))
		return hr;
	hr = pParameters->get_StrikePriceGroup(&SPG);
	if(FAILED(hr))
		return hr;

	for(long lTick = 0; lTick < 8; lTick++)
	{
		long lParam;

		hr = pParameters->get_TickWorseVolume(lTick + 1, &lParam);
		if(FAILED(hr))
			return hr;
		pusTickWorseVolume[lTick] = lParam;

		hr = pParameters->get_DerivedOrderMaxSize(lTick + 1, &lParam);
		if(FAILED(hr))
			return hr;
		pusDerivedOrderMaxSize[lTick] = lParam;

		hr = pParameters->get_MatchAwayMarketMaxSize(lTick + 1, &lParam);
		if(FAILED(hr))
			return hr;
		pusMatchAwayMarketMaxSize[lTick] = lParam;
	}

	hr = pParameters->get_StepUpAfterRegenBuffer(&lStepUpAfterRegenBuffer);
	if(FAILED(hr))
		return hr;
	hr = pParameters->get_IseMMTradeLimitAbsolute(&lIseMMTradeLimitAbsolute);
	if(FAILED(hr))
		return hr;
 	hr = pParameters->get_FirmTradeLimitAbsolute(&lFirmTradeLimitAbsolute);
	if(FAILED(hr))
		return hr;
	hr = pParameters->get_FarMMTradeLimitAbsolute(&lFarMMTradeLimitAbsolute);
	if(FAILED(hr))
		return hr;
	hr = pParameters->get_IseMmTradeLimitFraction(&lIseMmTradeLimitFraction);
	if(FAILED(hr))
		return hr;
	hr = pParameters->get_FirmTradeLimitFraction(&lFirmTradeLimitFraction);
	if(FAILED(hr))
		return hr;
	hr = pParameters->get_FarMmTradeLimitFraction(&lFarMmTradeLimitFraction);
	if(FAILED(hr))
		return hr;

	try
	{
		CISEMultiSession::SetMMParameters(bPMM,
									(char*)bsUnd, IG, EG, SPG, 
									pusTickWorseVolume,
									lStepUpAfterRegenBuffer,
									lIseMMTradeLimitAbsolute,
									lFirmTradeLimitAbsolute,
									lFarMMTradeLimitAbsolute,
									lIseMmTradeLimitFraction,
									lFirmTradeLimitFraction,
									lFarMmTradeLimitFraction,
									pusDerivedOrderMaxSize,
									pusMatchAwayMarketMaxSize);
	}
	catch(CISEException& e)
	{
		return e.GetHRESULT();
	}
	return S_OK;
}

HRESULT	CMMManagerObject::StartTrading()
{
	try
	{
		CISEMultiSession::ReadyToTrade();
	}
	catch(CISEException& e)
	{
		return e.GetHRESULT();
	}
	return S_OK;
}

HRESULT CMMManagerObject::EnterOrder( const string& sSeries, 
									const unsigned int uiQuantity, 
									const double dPremium,
									const BidAskEnum BidOrAsk,
									string& sOrderID)
{
	try
	{
		CISEMultiSession::EnterOrder(sSeries, uiQuantity, dPremium, BidOrAsk, sOrderID);
	}
	catch(CISEException& e)
	{
		return e.GetHRESULT();
	}

	return S_OK;
}