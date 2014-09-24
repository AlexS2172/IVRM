#include "StdAfx.h"
#include ".\ivdata.h"

long				GetInt ( CString* _sData , char _Delimiter = '\t' ) ;
double				GetDouble ( CString* _sData ) ;
EgLib::vt_date		GetDate	( CString* _sData ) ;
char				GetCharValue	( CString* _sData ) ;

CIVOptionData::CIVOptionData(CString _sData)
{

		m_lOptionID			= GetInt ( &_sData ) ;
		m_lUnderlyingID		= GetInt ( &_sData ) ;
		m_dModelIV			= GetDouble( &_sData ) ;
		m_dIV				= GetDouble( &_sData ) ;
		m_dDelta			= GetDouble( &_sData ) ;
		m_dGamma			= GetDouble( &_sData ) ;
		m_dVega				= GetDouble( &_sData ) ;
		m_dThetta			= GetDouble( &_sData ) ;
		m_dRho				= GetDouble( &_sData ) ;
		m_dBidPrice			= GetDouble( &_sData ) ;
		m_dAskPrice			= GetDouble( &_sData ) ;
		m_lBidSize			= GetInt ( &_sData ) ;
		m_lAskSize			= GetInt ( &_sData ) ;

		m_dtBidTime			= GetDate ( &_sData ) ;
		m_dtAskTime			= GetDate ( &_sData ) ;

		m_cBidExchange		= GetCharValue( &_sData ) ;
		m_cAskExchange		= GetCharValue( &_sData ) ;
		m_lVolume			= GetInt ( &_sData ) ;

}

CIVOptionData::~CIVOptionData(void)
{
}

void CIVOptionData::Copy ( const CIVOptionData& _C ) 
{
	m_lOptionID		= _C.m_lOptionID ;
	m_lUnderlyingID	= _C.m_lUnderlyingID ;
	m_dModelIV		= _C.m_dModelIV ;
	m_dIV			= _C.m_dIV ;
	m_dDelta		= _C.m_dDelta ;
	m_dGamma		= _C.m_dGamma ;
	m_dVega			= _C.m_dVega ;
	m_dThetta		= _C.m_dThetta ;
	m_dRho			= _C.m_dRho ;
	m_dBidPrice		= _C.m_dBidPrice ;
	m_dAskPrice		= _C.m_dAskPrice ;
	m_lBidSize		= _C.m_lBidSize ;
	m_lAskSize		= _C.m_lAskSize ;
	m_dtBidTime		= _C.m_dtBidTime ;
	m_dtAskTime		= _C.m_dtAskTime ;
	m_cBidExchange	= _C.m_cBidExchange ;
	m_cAskExchange	= _C.m_cAskExchange ;
	m_lVolume		= _C.m_lVolume ;
}
long GetInt ( CString* _sData , char _Delimiter ) 
{
	long lResult = atoi ( *_sData ) ;
	long lPos = _sData->Find ( _Delimiter ) ;
	if ( lPos > 0 )
		*_sData = _sData->Mid ( lPos + 1 ) ;
	return lResult ;

}
double GetDouble ( CString* _sData ) 
{
	double dResult = atof ( *_sData ) ;
	long lPos = _sData->Find ( "\t" ) ;
	if ( lPos > 0 )
		*_sData = _sData->Mid ( lPos + 1 ) ;
	return dResult ;

}

EgLib::vt_date GetDate	( CString* _sData ) 
{

	long lBidTimeY = 0  , lBidTimeMon = 0, lBidTimeD = 0, lBidTimeH = 0, lBidTimeMin = 0, lBidTimeS = 0 ;
	
	lBidTimeY		= GetInt ( _sData , '-' ) ;
	lBidTimeMon		= GetInt ( _sData , '-' ) ;
	lBidTimeD		= GetInt ( _sData , ' ' ) ;

	lBidTimeH		= GetInt ( _sData , ':' ) ;
	lBidTimeMin		= GetInt ( _sData , ':' ) ;
	lBidTimeS		= GetInt ( _sData ) ;

	SYSTEMTIME tm ;
	GetLocalTime ( &tm ) ;
	tm.wHour		= (WORD)lBidTimeH ;
	tm.wMinute		= (WORD)lBidTimeMin ;
	tm.wSecond		= (WORD)lBidTimeS ;
	tm.wYear		= (WORD)lBidTimeY ;
	tm.wMonth		= (WORD)lBidTimeMon ;
	tm.wDay			= (WORD)lBidTimeD ;

	EgLib::vt_date date ( tm ) ;
	return date ;
}

char GetCharValue ( CString* _sData ) 
{

	char cChar = *_sData[0] ;
	*_sData = _sData->Mid(2) ;
	return cChar ;
}


void CIVStockData::Copy( const CIVStockData& _C) 
{

	m_lStockId		= _C.m_lStockId ;
	m_dBidPrice		= _C.m_dBidPrice ;
	m_dAskPrice		= _C.m_dAskPrice ;
	m_dLastPrice	= _C.m_dLastPrice ;
	m_lSize			= _C.m_lSize ;
	m_lAskSize		= _C.m_lAskSize ;
	m_lLastSize		= _C.m_lLastSize ;
	m_dtBidTime		= _C.m_dtBidTime ;
	m_dtAsktime		= _C.m_dtAsktime ;
	m_dtLastTime	= _C.m_dtLastTime ;
	m_cBidEx		= _C.m_cBidEx ;
	m_cAskEx		= _C.m_cAskEx ;
	m_cLastPrEx		= _C.m_cLastPrEx ;
	m_lVolume		= _C.m_lVolume ;
}

CIVStockData::CIVStockData ( CString str )
{

	m_lStockId		= GetInt ( &str ) ;
	m_dBidPrice		= GetDouble ( &str ) ;
	m_dAskPrice		= GetDouble ( &str ) ;
	m_dLastPrice	= GetDouble ( &str ) ;
	m_lSize			= GetInt  ( &str ) ;
	m_lAskSize		= GetInt  ( &str ) ;
	m_lLastSize		= GetInt  ( &str ) ;

	m_dtBidTime		= GetDate ( &str ) ;
	m_dtAsktime		= GetDate ( &str ) ;
	m_dtLastTime	= GetDate ( &str ) ;
	m_cBidEx		= GetCharValue ( &str ) ;
	m_cAskEx		= GetCharValue ( &str ) ;
	m_cLastPrEx		= GetCharValue ( &str ) ;
	m_lVolume		= GetInt  ( &str ) ;
}