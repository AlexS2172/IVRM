#include "StdAfx.h"
#include ".\iv_data_update_service.h"

#include "IVData.h"

LPCTSTR CIV_Data_Update_Service::mProfileSectionDatabase	= "Database" ;
LPCTSTR CIV_Data_Update_Service::mProfileSectionServive		= "Service" ;


CIV_Data_Update_Service::CIV_Data_Update_Service()
: m_pMSMQRec ( new CMSMQReceiver(this) )
{
	HRESULT hr = ::CoCreateInstance
	(
		CLSID_MsgManager,
		NULL,
		CLSCTX_INPROC_SERVER  ,
		IID_IPubManager,
		(void**)&m_pEtsaMessenger
	) ;

	if ( FAILED (hr) )
	{
		m_pEtsaMessenger = NULL ;
		return ;
	}
	hr = m_pIvBlob.CreateInstance ( CLSID_IvBlob ) ;
	if ( FAILED (hr) )
	{
		g_Logs.TraceToFile ( LogFaults , "Fail to initialize Iv Blog Interface" ) ;
		return ;
	}
	m_pIvBlob->Clear() ;
	
	m_hStop.Attach(CreateEvent(NULL, TRUE, FALSE, NULL)) ;
}

CIV_Data_Update_Service::~CIV_Data_Update_Service(void)
{
	m_DbInterface.Disconnect() ;
}

bool CIV_Data_Update_Service::WaitTillOpen() 
{

	bool bResult = false ;
	EgLib::vt_date CurrentDateTime = EgLib::vt_date::GetCurrentDate();

	EgLib::vt_date_span span = m_dStart - CurrentDateTime ;
	DWORD dwInterwal = span.get_total_seconds()*1000;
	
	if ( CurrentDateTime < m_dStart )
	{
		g_Logs.TraceToFile ( LogInfo , "Will Wait %i seconds" , dwInterwal ) ;
		Sleep ( dwInterwal ) ;
		bResult = true ;
	}

	return bResult ;
}

void CIV_Data_Update_Service::LoadingFunction()
{
	
	try
	{
	
		g_Logs.TraceToFile ( LogInfo  , "Start of main function" ) ;
		Load() ;

		for ( ;; )
		{
		
			WaitTillOpen() ;
			LoadDataFromQueue () ;
		}
	}
	catch ( ...)
	{
		g_Logs.TraceToFile ( LogFaults , "Unknown exseption in main load function" ) ;
		ATLASSERT ( !"catch ..." ) ;
	}
}

void CIV_Data_Update_Service::LoadDataFromQueue ()
{

	do
	{
	
		m_pMSMQRec->GetMessages() ;
		if ( IsEndOfDay() )
			break ;
		else
		{
			HRESULT hr = WaitForSingleObject ( m_hStop , 6000 ) ;
			if ( hr == WAIT_OBJECT_0 )
			{
				g_Logs.TraceToFile ( LogDebug , "Stop of reading data from IV" ) ;
				break ;
			}
		}
	}while ( 1 ) ;

	m_pMSMQRec->Close() ;
}

void CIV_Data_Update_Service::Load(void)
{
	ReadProfile() ;
	try
	{
	
		m_DbInterface.Connect() ;
	}
	catch (  _com_error )
	{
		//ATLASSERT ( !"Can't open Data Base" ) ;
	}
}
void CIV_Data_Update_Service::ReadProfile() 
{
	
	char cBuff[128]  ;
	GetCurrentDirectory ( 128, cBuff );
	char stBuffer[128] ;
	CString sPath ( cBuff ) ;
	sPath += "\\" ;
	sPath += C_ProfileName ;
	memset ( stBuffer , 0 , sizeof(stBuffer) ) ;
	
	SYSTEMTIME st ;
	GetLocalTime ( &st ) ;

	DWORD dwRes = GetPrivateProfileString ( mProfileSectionServive  , "StartTime" , "6:10" , stBuffer ,sizeof(stBuffer) , sPath  ) ;
	if ( dwRes > 0 )
	{
		CString sStart ( stBuffer ) ;
		WORD lH = atoi ( sStart ) ;
		long lPos = sStart.Find ( ":" ) ;
		WORD lM = 0 ;
		if ( lPos > 0 )
			lM = atoi ( sStart.Mid ( lPos + 1 ) ) ;

		st.wHour	= lH ;
		st.wMinute	= lM ;
		m_dStart = EgLib::vt_date ( st ) ;
	}
	BOOL bRes = WritePrivateProfileString ( mProfileSectionServive  , "StartTime" ,  stBuffer , sPath  ) ;

	memset ( stBuffer , 0 , sizeof(stBuffer) ) ;
	dwRes = GetPrivateProfileString ( mProfileSectionServive , "StopTime"  , "23:00" , stBuffer ,sizeof(stBuffer) , sPath  ) ;
	if ( dwRes > 0 )
	{
	
		CString sStop ( stBuffer ) ;

		WORD lH = atoi ( sStop ) ;
		long lPos = sStop.Find ( ":" ) ;
		WORD lM = 0 ;
		if ( lPos > 0 )
			lM = atoi ( sStop.Mid ( lPos + 1 ) ) ;

		st.wHour	= lH ;
		st.wMinute	= lM ;
		st.wSecond	= 0 ;
		m_dStop		= EgLib::vt_date ( st ) ;
	}
	WritePrivateProfileString ( mProfileSectionServive , "StopTime" , stBuffer , sPath  ) ;
	memset ( stBuffer , 0 , sizeof(stBuffer) ) ;

	dwRes = GetPrivateProfileString ( mProfileSectionDatabase , "Connection"  , "" , stBuffer ,sizeof(stBuffer) , sPath  ) ;
	if ( dwRes > 0 )
	{
		m_DbInterface.SetConnectionString (  stBuffer ) ;
	}
	WritePrivateProfileString ( mProfileSectionDatabase , "Connection" , stBuffer , sPath  ) ;

	memset ( stBuffer , 0 , sizeof(stBuffer) ) ;
	dwRes = GetPrivateProfileString ( "IV" , "IVOptionsQueue", "Queue$" , stBuffer ,sizeof(stBuffer) , sPath  ) ;
	_bstr_t bsQueOpt ;
	if ( dwRes > 0 )
	{
		g_Logs.TraceToFile ( LogDebug , "Option Queue name %s", stBuffer ) ;
		bsQueOpt = _bstr_t ( stBuffer ) ;
	}
	WritePrivateProfileString ( "IV" , "IVOptionsQueue" , stBuffer , sPath  ) ;

	memset ( stBuffer , 0 , sizeof(stBuffer) ) ;
	dwRes = GetPrivateProfileString ( "IV" , "IVStocksQueue", "Queue$" , stBuffer ,sizeof(stBuffer) , sPath  ) ;
	_bstr_t bsQueStock ;
	if ( dwRes > 0 )
	{
		g_Logs.TraceToFile ( LogDebug , "Stock Queue name %s", stBuffer ) ;
		bsQueStock = _bstr_t ( stBuffer ) ;
	}
	WritePrivateProfileString ( "IV" , "IVStocksQueue" , stBuffer , sPath  ) ;

	m_pMSMQRec->SetMSQMStr ( bsQueOpt , bsQueStock ) ;
}

void CIV_Data_Update_Service::SendUpdate() 
{
	try
	{
		if ( m_pIvBlob->Empty() == VARIANT_TRUE )
			return ;

		g_Logs.TraceToFile ( LogDebug , "Sending Update" ) ;
		long lRes = m_pEtsaMessenger->PubIVBlobUpdate ( m_pIvBlob ) ;
		m_pIvBlob->Clear () ;
	}
	catch ( ... )
	{
		g_Logs.TraceToFile ( LogFaults, "Unknown exception in sending mess to transport" ) ;
	}
}
bool CIV_Data_Update_Service::SaveIVOptionData(const CIVOptionData& _IVData)
{
	bool bRes = m_DbInterface.SaveIVOptionData ( _IVData ) ;
	m_pIvBlob->AddIvOption ( _IVData.GetUnderlyingID() , _IVData.GetIV() ) ;
	return bRes ;
}
bool CIV_Data_Update_Service::SaveIVStockData(const CIVStockData& _IVData)
{
	bool bRes = m_DbInterface.SaveIVStockData ( _IVData ) ;
	m_pIvBlob->AddIvStock ( _IVData.GetStockID() , _IVData.GetLastPrice() ) ;
	return bRes ;
}

/*bool CIV_Data_Update_Service::SaveIVData ( const CIVOptionData& _IVData ) 
{
	bool bRes = m_DbInterface.SaveIVData ( _IVData ) ;
	m_pIvBlob->AddIvOption ( _IVData.GetOptionID() , _IVData.GetIV() ) ;
	return bRes ;
}
*/
bool CIV_Data_Update_Service::IsEndOfDay ()
{

	EgLib::vt_date CurrentDateTime = EgLib::vt_date::GetCurrentDate();
	if ( CurrentDateTime  >= m_dStop )
		return true ;
	return false ;
}