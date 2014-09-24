#include "StdAfx.h"
#include "settings.h"

CSettings::CSettings(void)
			:m_bsID(""),
			m_bsPrimaryIP(""),
			m_bsSecondaryIP(""),
			m_bsPassword(""),
			m_lPort(0)
{

}

CSettings::~CSettings(void)
{
}


bool CSettings:: operator!= (const ConnectionSettings*  pSettings) const 
{
	if(pSettings->Port != m_lPort)
		return true;

	if( _bstr_t( pSettings->ID)  != m_bsID)
		return true;

	if( _bstr_t( pSettings->Password ) != m_bsPassword)
		return true;

	if( _bstr_t( pSettings->PrimaryIP ) != m_bsPrimaryIP)
		return true;

	if( _bstr_t( pSettings->SecondaryIP ) != m_bsSecondaryIP)
		return true;

	return false;
}

bool CSettings::CreateFrom(const ConnectionSettings* pSettings, _bstr_t* pStrOut)
{
	ATLASSERT(pSettings);
	if(pSettings)
	{
		bool bError = false;
		if(pSettings->Port <= 0)
		{
			*pStrOut = L"Invalid port";
			bError = true;
		}

		if(_bstr_t(pSettings->ID).length()==0 )
		{
			if(pStrOut->length()>0)
				*pStrOut+=L", invalid Id";
			else
				*pStrOut+=L"Invalid Id";
			bError = true;
		}

		if( 
			(_bstr_t(pSettings->PrimaryIP).length()==0 )&&
			(_bstr_t(pSettings->SecondaryIP).length()==0 )
			)
		{
			if(pStrOut->length()>0)
				*pStrOut+=L", no valid IP provided";
			else
				*pStrOut+=L"No valid IP provided";
			bError = true;
		}

		if(bError)
		{
			*pStrOut+=".";
			return false;
		}

		m_bsID = pSettings->ID;
		m_bsPrimaryIP = pSettings->PrimaryIP;
		m_bsSecondaryIP= pSettings->SecondaryIP;
		m_bsPassword = pSettings->Password;
		m_lPort = pSettings->Port;
	}else
	{
		*pStrOut = L"Invalid pointer.";
		return false;
	}
	return true;
}