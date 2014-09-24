#include "StdAfx.h"
#include "dbinterfacebo.h"
#include ".\dbinterfacebo.h"
//**************************************************************************************************************//
// class CDbInterfaceBo
//**************************************************************************************************************//
#include "IVData.h"


CDbInterfaceBo* CDbInterfaceBo::m_pDbInterface(NULL);
_bstr_t CDbInterfaceBo::m_bsConnectionString("");
DWORD CDbInterfaceBo::m_nReconnectionAttempts(5);
DWORD CDbInterfaceBo::m_nReconnectionDelay(5000);

CDbInterfaceBo::CDbInterfaceBo(void)
{

}

CDbInterfaceBo::~CDbInterfaceBo(void)
{
}

void CDbInterfaceBo::GetInstance(CDbInterfaceBo** ppDbInterface)
{
	if ( m_pDbInterface)
		m_pDbInterface = new CDbInterfaceBo();
	*ppDbInterface = m_pDbInterface;
}

//--------------------------------------------------------------------------------------------------------------//
void CDbInterfaceBo::SetConnectionString(const std::string& rsConnectionString)
{
	m_bsConnectionString = rsConnectionString.c_str();
}
//--------------------------------------------------------------------------------------------------------------//
void CDbInterfaceBo::SetReconnectionAttempts(DWORD nReconnectionAttempts)
{
	m_nReconnectionAttempts = nReconnectionAttempts;
}

//--------------------------------------------------------------------------------------------------------------//
void CDbInterfaceBo::SetReconnectionDelay(DWORD nReconnectionDelay)
{
	m_nReconnectionDelay = nReconnectionDelay;
}

//--------------------------------------------------------------------------------------------------------------//
EgLib::CDBConnection& CDbInterfaceBo::GetConnection()
{
	return m_Connection;
}

//--------------------------------------------------------------------------------------------------------------//
void CDbInterfaceBo::Connect() 
{
	try
	{

		m_Connection.Open(m_bsConnectionString, m_nReconnectionDelay, m_nReconnectionAttempts, 30, 120);
	}
	catch ( _com_error _e)
	{
		g_Logs.TraceToFile ( LogFaults , "Can't connect to DB %s" , (LPCTSTR)_e.Description() ) ;
		throw  _e ;
	}
}

//--------------------------------------------------------------------------------------------------------------//
void CDbInterfaceBo::Disconnect()
{
	m_Connection.Close();
}

DWORD CDbInterfaceBo::BeginOfDay(void)
{

	DWORD dwResult = S_OK ;

	_bstr_t sUsp(_T("usp_DatabaseCleanUp"));
	try
	{
		EgLib::CStoredProc<> spCleanUp(m_Connection, sUsp);
		spCleanUp.Execute () ;
	}
	catch (const _com_error& re)
	{
		//OMLIB_TRACE_EX(re.Error(), ERROR_OM_DB_STOREDPROC, (LPCTSTR)sUsp, re.Description().length()?(LPCTSTR)re.Description():re.ErrorMessage());
		//AfxMessageBox(_T("Error checking DB Version."), MB_ICONERROR);
		dwResult = re.Error();
	}
	return dwResult;
}

bool CDbInterfaceBo::SaveIVStockData(const CIVStockData& _IVData)
{
	DWORD dwResult = S_OK ;

	_bstr_t sUsp(_T("usp_InputStockData"));
	try
	{

		EgLib::CStoredProc<> spSave (m_Connection, sUsp);
		spSave << _IVData.m_lStockId 
			<< _IVData.m_dBidPrice
			<< _IVData.m_dAskPrice
			<< 0
			<< _IVData.m_lAskSize
			<< _IVData.m_dtBidTime
			<< _IVData.m_dtAsktime
			<< _IVData.m_cBidEx
			<< _IVData.m_cAskEx
			<< _IVData.m_lVolume ;

		spSave.Execute () ;
	}
	catch (const _com_error& re)
	{
		//OMLIB_TRACE_EX(re.Error(), ERROR_OM_DB_STOREDPROC, (LPCTSTR)sUsp, re.Description().length()?(LPCTSTR)re.Description():re.ErrorMessage());
		//AfxMessageBox(_T("Error checking DB Version."), MB_ICONERROR);
		dwResult = re.Error();
		g_Logs.TraceToFile ( LogFaults , "DB Error: %s" , re.Description().length()?(LPCTSTR)re.Description():(LPCTSTR)re.ErrorMessage() );
	}
	return dwResult == S_OK;
}

bool CDbInterfaceBo::SaveIVOptionData ( const CIVOptionData& _IVData )
{
	DWORD dwResult = S_OK ;

	_bstr_t sUsp(_T("usp_InputOptionData"));
	try
	{

		EgLib::CStoredProc<> spSave (m_Connection, sUsp);
		spSave << _IVData.m_lOptionID 
			<< _IVData.m_lUnderlyingID
			<< _IVData.m_dModelIV 
			<< _IVData.m_dIV 
			<< _IVData.m_dDelta 
			<< _IVData.m_dGamma 
			<< _IVData.m_dVega 
			<< _IVData.m_dThetta 
			<< _IVData.m_dRho
			<< _IVData.m_dBidPrice
			<< _IVData.m_dAskPrice 
			<< _IVData.m_lBidSize
			<< _IVData.m_lAskSize
			<< _IVData.m_dtBidTime
			<< _IVData.m_dtAskTime
			<< _IVData.m_cBidExchange
			<< _IVData.m_cAskExchange
			<< _IVData.m_lVolume  ;

		spSave.Execute () ;
	}
	catch (const _com_error& re)
	{
		//OMLIB_TRACE_EX(re.Error(), ERROR_OM_DB_STOREDPROC, (LPCTSTR)sUsp, re.Description().length()?(LPCTSTR)re.Description():re.ErrorMessage());
		//AfxMessageBox(_T("Error checking DB Version."), MB_ICONERROR);
		dwResult = re.Error();
		g_Logs.TraceToFile ( LogFaults , "DB Error: %s" , re.Description().length()?(LPCTSTR)re.Description():(LPCTSTR)re.ErrorMessage() );
	}
	return dwResult == S_OK;

}
