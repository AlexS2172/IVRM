#include "StdAfx.h"
#include "msmqreceiver.h"

#include <boost/shared_array.hpp>
#include <vector>

#pragma comment ( lib , "Mqrt.lib" )

static const BYTE C_Byte_End_Of_Line = 13 ;
#include "IvData.h"

CMSMQReceiver::CMSMQReceiver(BIV_Data_Update_Service* _pService)
	:m_hQuOpt(INVALID_HANDLE_VALUE)
	,m_hQuSt(INVALID_HANDLE_VALUE)
	,m_pService(_pService)
{
}

CMSMQReceiver::~CMSMQReceiver(void)
{

	Close() ;
}

void CMSMQReceiver::PurgeQue(  )
{
	try
	{

		HRESULT hres ;

		hres = MQOpenQueue
		( 
			m_sMSQMStrOpt,
			MQ_RECEIVE_ACCESS  ,
			MQ_DENY_RECEIVE_SHARE,
			&m_hQuOpt
		) ;

		if ( hres != S_OK )
		{
			if ( hres == MQ_ERROR_QUEUE_NOT_FOUND)
				g_Logs.TraceToFile ( LogFaults, "Option queue - QUEUE NOT FOUND" ) ;
			if ( hres == MQ_ERROR_INVALID_PARAMETER )
				g_Logs.TraceToFile ( LogFaults, "Option queue - One of the IN parameters is not valid." ) ;
			if ( hres == MQ_ERROR_ILLEGAL_FORMATNAME )
				g_Logs.TraceToFile ( LogFaults, "Option queue - The lpwcsFormatName parameter specified an illegal format name. " ) ;

			g_Logs.TraceToFile ( LogFaults, "Can't open Option queue %d" , hres ) ;

		}

		hres = MQOpenQueue
		( 
			m_sMSQMStrStock,
			MQ_RECEIVE_ACCESS,
			MQ_DENY_RECEIVE_SHARE,
			&m_hQuSt
		) ;

		if ( hres != S_OK )
		{
			if ( hres == MQ_ERROR_QUEUE_NOT_FOUND)
				g_Logs.TraceToFile ( LogFaults, "Stock queue - QUEUE NOT FOUND" ) ;
			if ( hres == MQ_ERROR_INVALID_PARAMETER )
				g_Logs.TraceToFile ( LogFaults, "Stock queue - One of the IN parameters is not valid." ) ;
			if ( hres == MQ_ERROR_ILLEGAL_FORMATNAME )
				g_Logs.TraceToFile ( LogFaults, "Stock queue - The lpwcsFormatName parameter specified an illegal format name. " ) ;

			g_Logs.TraceToFile ( LogFaults, "Can't open Stock queue %d" , hres ) ;

		}
		g_Logs.TraceToFile ( LogInfo , "Purging Options queue ..." );
		if ( MQ_OK != MQPurgeQueue ( m_hQuOpt ) )
			g_Logs.TraceToFile ( LogInfo , "Can't Delete Data from Option Queue" ) ;
		else
			g_Logs.TraceToFile ( LogInfo , "Data from Option Queue was deleted completely" ) ;


		g_Logs.TraceToFile ( LogInfo , "Purging Stocks queue ..." );
		g_Logs.TraceToFile ( LogDebug , "Queue were opened for purge" ) ;
		if ( MQ_OK != MQPurgeQueue ( m_hQuSt ) )
			g_Logs.TraceToFile ( LogInfo , "Can't Delete Data from Stock Queue" ) ;
		else
			g_Logs.TraceToFile ( LogInfo , "Data from Stock  Queue was deleted completely" ) ;

		Close () ;
	}
	catch ( ... )
	{
	}
}

void CMSMQReceiver::Close() 
{
	if ( m_hQuOpt != INVALID_HANDLE_VALUE && m_hQuOpt )
		::MQCloseQueue ( m_hQuOpt ) ;
	m_hQuOpt = INVALID_HANDLE_VALUE ;
	if ( m_hQuSt != INVALID_HANDLE_VALUE && m_hQuSt )
		::MQCloseQueue ( m_hQuSt ) ;
	m_hQuSt = INVALID_HANDLE_VALUE ;
}

bool CMSMQReceiver::Open(bool bStock)
{

	try
	{

		HRESULT hres ;
		if ( !bStock )
		{
		
			hres = MQOpenQueue
			( 
				m_sMSQMStrOpt,
				MQ_RECEIVE_ACCESS  ,
				MQ_DENY_RECEIVE_SHARE,
				&m_hQuOpt
			) ;
			
			if ( hres != S_OK )
			{
				if ( hres == MQ_ERROR_QUEUE_NOT_FOUND)
					g_Logs.TraceToFile ( LogFaults, "Option queue - QUEUE NOT FOUND" ) ;
				if ( hres == MQ_ERROR_INVALID_PARAMETER )
					g_Logs.TraceToFile ( LogFaults, "Option queue - One of the IN parameters is not valid." ) ;
				if ( hres == MQ_ERROR_ILLEGAL_FORMATNAME )
					g_Logs.TraceToFile ( LogFaults, "Option queue - The lpwcsFormatName parameter specified an illegal format name. " ) ;

				g_Logs.TraceToFile ( LogFaults, "Can't open Option queue %d" , hres ) ;

				return false ;
			}
		}
		else
		{
		
			hres = MQOpenQueue
			( 
				m_sMSQMStrStock,
				MQ_RECEIVE_ACCESS,
				MQ_DENY_RECEIVE_SHARE,
				&m_hQuSt
			) ;


			if ( hres != S_OK )
			{
				if ( hres == MQ_ERROR_QUEUE_NOT_FOUND)
					g_Logs.TraceToFile ( LogFaults, "Stock queue - QUEUE NOT FOUND" ) ;
				if ( hres == MQ_ERROR_INVALID_PARAMETER )
					g_Logs.TraceToFile ( LogFaults, "Stock queue - One of the IN parameters is not valid." ) ;
				if ( hres == MQ_ERROR_ILLEGAL_FORMATNAME )
					g_Logs.TraceToFile ( LogFaults, "Stock queue - The lpwcsFormatName parameter specified an illegal format name. " ) ;

				g_Logs.TraceToFile ( LogFaults, "Can't open Stock queue %d" , hres ) ;

				return false ;
			}
		}

		g_Logs.TraceToFile ( LogDebug , "Queue were opened" ) ;
	}
	catch ( ... )
	{
		return false ;
	}
	return true ;
}

void CMSMQReceiver::GetMessages () 
{
	
	CIVOptionData IVData ( "16627	32751	1.308743	1.308743	1.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0.000000	0	0	2005-04-07 00:00:00	2005-04-07 00:00:00	*	*	0" ) ;
	m_pService->SaveIVOptionData ( IVData) ;
	m_pService->SendUpdate() ;
	m_pService->SaveIVOptionData ( IVData) ;
	CIVStockData IvStock ( "9483	1.800000	2.060000	1.910000	1	1	0	2005-05-06 09:30:00	2005-05-06 09:30:00	2005-05-05 16:02:00	P	P	P	47666" ) ;
	m_pService->SaveIVStockData( IvStock ) ;
	m_pService->SaveIVStockData( IvStock ) ;
	
	m_pService->SendUpdate() ;

	if ( Open( false ) )
		GetOptionMessages () ;
	Close() ;
	if ( Open( true ) )
		GetStockMessages () ;
	Close() ;
	PurgeQue ( ) ;
	m_pService->SendUpdate() ;
}

void CMSMQReceiver::GetStockMessages () 
{

	if ( m_hQuSt != INVALID_HANDLE_VALUE && m_hQuSt )
	{
		try
		{

			DWORD dwSize_ = 0 ;
			if ( GetMessageSize ( &dwSize_ , m_hQuSt ) )
			{

				g_Logs.TraceToFile ( LogInfo , "In MSMQ for stock are %i bytes of data" , dwSize_ ) ;

				boost::shared_array<BYTE> pBody ( new BYTE[ dwSize_ ] ) ;
				DWORD dwSizeQ = dwSize_ ;
				if ( PeekMessage ( &dwSize_ , pBody.get() , m_hQuSt ) )
				{

					char bLine[2048] ;
					
					for ( DWORD dwPos = 0 ; dwPos <=dwSizeQ ; dwPos++ )
					{
						memset ( bLine , 0 , sizeof ( bLine ) ) ;
						DWORD dwEndOfLine = dwPos ;
						while ( pBody[dwEndOfLine] != C_Byte_End_Of_Line && dwEndOfLine < dwSizeQ )
							dwEndOfLine++ ;

						memcpy ( bLine , &pBody[dwPos] , min ( (long)sizeof(bLine) , dwEndOfLine - dwPos ) ) ;
						
						CString str ( bLine ) ;
						if ( str.GetLength() < 20 ) 
							break ;
						CIVStockData IVData ( str ) ;

						m_pService->SaveIVStockData( IVData ) ;

						dwPos = dwEndOfLine ;
					}
				}
			}
			g_Logs.TraceToFile ( LogInfo , "Data from Stocks MSMQ was loaded" ) ;
		}
		catch ( ... )
		{
			ATLASSERT ( !"catch ..." ) ;
			g_Logs.TraceToFile ( LogFaults , "Can't get Data from Queue - some unknown error" ) ;
		}
	}
}

void CMSMQReceiver::GetOptionMessages () 
{
	if ( m_hQuOpt != INVALID_HANDLE_VALUE && m_hQuOpt )
	{
		try
		{

			DWORD dwSize_ = 0 ;
			if ( GetMessageSize ( &dwSize_ , m_hQuOpt ) )
			{

				g_Logs.TraceToFile ( LogInfo  , "In MSMQ are %i bytes of data" , dwSize_ ) ;
				
				boost::shared_array<BYTE> pBody ( new BYTE[ dwSize_ ] ) ;
				long lSize = (long)dwSize_ ;
				if ( PeekMessage ( &dwSize_ , pBody.get() , m_hQuOpt ) )
				{
					char bLine[2048] ;
					memset ( bLine , 0 , (long)sizeof ( bLine ) ) ;
					for ( long dwPos = 0 ; dwPos <= lSize ; dwPos++ )
					{
						long dwEndOfLine = (long)dwPos ;
						while ( pBody[dwEndOfLine] != C_Byte_End_Of_Line && dwEndOfLine <= lSize )
							dwEndOfLine++ ;

						memcpy ( bLine , &pBody[dwPos] , min ( (long)sizeof(bLine) , dwEndOfLine - dwPos ) ) ;

						CString str ( bLine ) ;
						CIVOptionData IVData ( str ) ;

						m_pService->SaveIVOptionData ( IVData) ;

						memset ( bLine , 0 , sizeof(bLine) ) ;
						dwPos = dwEndOfLine ;
					}
				}
			}
			g_Logs.TraceToFile ( LogInfo , "Data from MSMQ Options was loaded" ) ;
		}
		catch ( ... )
		{
			ATLASSERT ( !"catch ..." ) ;
			g_Logs.TraceToFile ( LogFaults, "Can't get data from option queue" ) ;
		}
	}

}

void APIENTRY GetMessage
(
	HRESULT hrStatus,
	QUEUEHANDLE hSource,
	DWORD dwTimeout,
	DWORD dwAction,
	MQMSGPROPS* pMessageProps,
	LPOVERLAPPED lpOverlapped,
	HANDLE hCursor
)
{
}

bool CMSMQReceiver::PeekMessage ( DWORD* _pdwSize , PBYTE _pBody , HANDLE _hndl ) 
{

	DWORD   cProps = 0;
	WCHAR	szLabel[512];
	

	HRESULT hr = S_OK;  

	m_amPropId[cProps]				= PROPID_M_BODY; 
	m_aPropVar[cProps].vt			= VT_UI1 | VT_VECTOR;         
	m_aPropVar[cProps].caub.cElems	= *_pdwSize;         
	m_aPropVar[cProps].caub.pElems	= _pBody;         
	cProps++; 

	m_amPropId[cProps]				= PROPID_M_BODY_SIZE; 
	m_aPropVar[cProps].vt			= VT_UI4; 
	cProps++; 

	//	USES_CONVERSION;
	m_amPropId[cProps]				= PROPID_M_LABEL;         
	m_aPropVar[cProps].vt			= VT_LPWSTR;         
	m_aPropVar[cProps].pwszVal		= szLabel;         
	cProps++; 

	m_amPropId[cProps]				= PROPID_M_LABEL_LEN;         
	m_aPropVar[cProps].vt			= VT_UI4;         
	m_aPropVar[cProps].ulVal		= MQ_MAX_MSG_LABEL_LEN;         
	cProps++; 

	m_msgprops.cProp				= cProps;         
	m_msgprops.aPropID				= m_amPropId;         
	m_msgprops.aPropVar				= m_aPropVar;         
	m_msgprops.aStatus				= 0; 

	hr = MQReceiveMessage
	(	
		_hndl, 
		INFINITE , 
		MQ_ACTION_RECEIVE,//MQ_ACTION_PEEK_CURRENT ,//MQ_ACTION_RECEIVE,
		&m_msgprops,
		NULL,
		NULL,
		NULL,
		NULL
	);

	if ( MQ_ERROR_ACCESS_DENIED == hr )
		g_Logs.TraceToFile ( LogSystem , "Access Denied 2 Queue" ) ;

	*_pdwSize = m_aPropVar[1].ulVal;
	if (SUCCEEDED(hr))
	{
		return true ;
	}
	return false ;
}

bool CMSMQReceiver::GetMessageSize ( DWORD* dwSize , HANDLE _hndl)
{

	HRESULT hr = S_OK;
	DWORD   cProps = 0;
	*dwSize = 0;

	m_amPropId[cProps]				= PROPID_M_BODY_SIZE; 
	m_aPropVar[cProps].vt				= VT_UI4; 
	cProps++; 

	m_msgprops.cProp				= cProps;         
	m_msgprops.aPropID				= m_amPropId;         
	m_msgprops.aPropVar				= m_aPropVar;         
	m_msgprops.aStatus				= 0; 

	hr = MQReceiveMessage
	(	
		_hndl,
		30000, 
		MQ_ACTION_PEEK_CURRENT,
		&m_msgprops,
		NULL,
		NULL,
		NULL,
		NULL
	);


	if (SUCCEEDED(hr))
	{
		//*dwSize = min ( (DWORD)10000000 ,  (DWORD)m_aPropVar[0].ulVal ) ;
		*dwSize = m_aPropVar[0].ulVal ;
		if ( *dwSize  == -1)
			return false ;
		return true ;
	}
	return false;
}
