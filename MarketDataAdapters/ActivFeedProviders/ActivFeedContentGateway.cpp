#include "StdAfx.h"
#include ".\activfeedcontentgateway.h"
#include "ActivFeedAppication.h"
#include "ActivFeedBatchPriceInfo.h"
#include "CustomerInfo.h"
#include "ActivContentPlatform/ActivFeedCommon/FieldIds.h"
#include "ActivMiddleWare/StatusCodes.h"
#include "ActivContentPlatform/ActivFeedApiCommon/RealtimeTypes.h"
#include "ActivContentPlatform/ActivFeedCommon/Enumerations.h"
#include <vector>
#include <string>
#include <algorithm>
#include <deque>

#ifndef RULE_DOES_NOT_UPDATE_LAST_VALUE
//#define RULE_DOES_NOT_UPDATE_LAST_VALUE RULE_DO_NOT_UPDATE_CACHE
#endif

using namespace Activ;
using namespace ContentPlatform;
using namespace Feed;

std::string FutureExpiration(const std::string& sFutureName)
{
	std::string sExpName;

	std::string::size_type pos = sFutureName.find_last_of('/');
	if(pos!=std::string::npos)
	{
		std::string sFutTail = sFutureName.substr(pos+1);
		if(sFutTail.length()==3)
		{
			char chMonth = *(sFutTail.c_str()+2);
			std::string sYear = sFutTail.substr(0, 2);
			switch(chMonth)
			{
			case 'F':
			case 'f': 
				sExpName = _T("Jan");
				break;
			case 'G':
			case 'g':
	 		    sExpName = _T("Feb");
				break;
			case 'H':
			case 'h':
				sExpName = _T("Mar");
				break;
			case 'J':
			case 'j':
				sExpName = _T("Apr");
				break;
			case 'K':
			case 'k':
				sExpName = _T("May");
				break;
			case 'M':
			case 'm':
				sExpName = _T("Jun");
				break;
			case 'N':
			case 'n':
				sExpName = _T("Jul");
				break;
			case 'Q':
			case 'q':
				sExpName = _T("Aug");
				break;
			case 'U':
			case 'u':
				sExpName = _T("Sep");
				break;
			case 'V':
			case 'v':
				sExpName = _T("Oct");
				break;
			case 'X':
			case 'x':
				sExpName = _T("Nov");
				break;
			case 'Z':
			case 'z':
				sExpName = _T("Dec");
				break;
			}
		sExpName+=sYear;
		}
	}
	return sExpName;
}
void	Activ2Egar( std::string& Ee, InstrumentTypeEnum t  ){
	std::string strActiv( Ee );
	ActivFeedSettings::CEgarExchangeKey::ExchangeType	type;
	switch( t) {
		case	enSTK:
		case 	enFUT:
		case	enIDX:	
			type = ActivFeedSettings::CExchangeKey::enExchangeStock;
			break;
		case	enOPT:	
		case	enFOP:
			type = ActivFeedSettings::CEgarExchangeKey::enExchangeOption;
			break;
		default:	return;
	}
	ActivFeedSettings::CExchangeKey exhKey = ActivFeedSettings::CExchangeKey(strActiv, type );
	ActivFeedSettings::CActivExchangeKey ActivKey(exhKey);
	ActivFeedSettings::CEgarExchangeKey EgarKey = ActivFeedSettings::g_spUserSettings->Settings->EgarExchange[ActivKey];
	if(EgarKey.length())
		Ee = EgarKey;
}
void	EgarSTK2Activ( std::string& Ex, std::string& Ax, std::string& exchRegion)
{
	Ax = Ex;
	std::string strEgar( Ex );
	ActivFeedSettings::CExchangeKey exhKey = ActivFeedSettings::CExchangeKey(strEgar, ActivFeedSettings::CExchangeKey::enExchangeStock );
	ActivFeedSettings::CEgarExchangeKey EgarKey(exhKey);
	ActivFeedSettings::CActivExchangeKey ActivKey = ActivFeedSettings::g_spUserSettings->Settings->ActivExchange[EgarKey];

	if(ActivKey.length())
		Ax = ActivKey;

	exchRegion =  ActivKey.length() ? ActivFeedSettings::g_spUserSettings->Settings->ActivExchange[EgarKey].GetExchangeRegion() : "USA";
}

void	SEgarSTK2Activ( std::vector<std::string>& Ex, std::vector<std::string>& Ax, std::vector<std::string>& regions  )
{
	size_t t = Ex.size();
	for ( size_t i = 0; i < t; i++)
	{
		std::string s = "";
		s = Ex[i];
		Ax[i] = s;
		std::string strEgar( s );
		ActivFeedSettings::CExchangeKey exhKey = ActivFeedSettings::CExchangeKey(strEgar, ActivFeedSettings::CExchangeKey::enExchangeStock );
		ActivFeedSettings::CEgarExchangeKey EgarKey(exhKey);
		ActivFeedSettings::CActivExchangeKey ActivKey = ActivFeedSettings::g_spUserSettings->Settings->ActivExchange[EgarKey];
		if(ActivKey.length())
			Ax[i] = ActivKey;

		regions[i] =  ActivKey.length() ? ActivFeedSettings::g_spUserSettings->Settings->ActivExchange[EgarKey].GetExchangeRegion() : "USA";
	}
}
//void	SEgarOPT2Activ( std::vector<std::string>& Ex, std::vector<std::string>& Ax )
//{
//	size_t t = Ex.size();
//	for ( size_t i = 0; i < t; i++)
//	{
//		if( Ex[i] == " " || Ex[i] == "" )
//			Ax[i] = "_";
//		else
//			Ax[i] = Ex[i];
//		std::string strEgar( Ax[i] );
//		ActivFeedSettings::CExchangeKey exhKey = ActivFeedSettings::CExchangeKey(strEgar, ActivFeedSettings::CExchangeKey::enExchangeOption );
//		ActivFeedSettings::CEgarExchangeKey EgarKey(exhKey);
//		ActivFeedSettings::CActivExchangeKey ActivKey = ActivFeedSettings::g_spUserSettings->Settings->ActivExchange[EgarKey];
//		if(ActivKey.length())
//			Ax[i] = ActivKey;
//	}
//}

//void	EgarOPT2Activ( std::string& Es, std::string& Ex, std::string& As, std::string& Ax ) 
//{
//	As = Es;
//	basic_string <char>::iterator it = ( As.end ( ) - 2 );
//	As.insert ( it , '/' );
//	it+=2;
//	As.insert ( it , '/' );
//	Ax = Ex;
//	if( Ax == " " || Ax == "" )
//		Ax = "_";
//	if( Ax.length() )
//	{
//		std::string strEgar( Ax );
//		ActivFeedSettings::CExchangeKey exhKey = ActivFeedSettings::CExchangeKey(strEgar, ActivFeedSettings::CExchangeKey::enExchangeOption );
//		ActivFeedSettings::CEgarExchangeKey EgarKey(exhKey);
//		ActivFeedSettings::CActivExchangeKey ActivKey = ActivFeedSettings::g_spUserSettings->Settings->ActivExchange[EgarKey];
//
//		if(ActivKey.length())
//			Ax = ActivKey;
//	}
//}

void	EgarIDX2Activ( std::string& Es )
{
	Es = '=' + Es;
}


void
CActivFeedContentGateway::ConvertExchange(_QuoteRequestPtr request, std::string &exchange_code){
	if (request){
		if (enOPT == request->Type){

			exchange_code = request->Exchange;

			if (exchange_code == ""){ //NBBO mapping exchange code
				exchange_code = "_";
			}

			if (exchange_code.length()){
				std::string strEgar( exchange_code );
				
				ActivFeedSettings::CExchangeKey exhKey = 
					ActivFeedSettings::CExchangeKey(strEgar, ActivFeedSettings::CExchangeKey::enExchangeOption );

				ActivFeedSettings::CEgarExchangeKey EgarKey(exhKey);

				ActivFeedSettings::CActivExchangeKey ActivKey = 
					ActivFeedSettings::g_spUserSettings->Settings->ActivExchange[EgarKey];

				if(ActivKey.length())
					exchange_code = ActivKey;
			}
		}
	}
}

void
CActivFeedContentGateway::ConvertRequest(_QuoteRequestPtr request, std::string &key){
	if (request){
		if (enOPT == request->Type){
			static Activ::ContentPlatform::Feed::UsEquityOptionHelper optionHelper;

			std::string exchange;
			ConvertExchange(request, exchange);
			
			if(optionHelper.GetSymbolFromOsiSymbol(key,
												request->Symbol,
												exchange)
												==STATUS_CODE_FAILURE)
			{
				key = request->Symbol + ".O";
			}
		}
	}
};

void
CActivFeedContentGateway::CancelStock(VARIANT Params)
{
	CRequestsContainer::iterator i, iend = m_Container.end();
	for(i = m_Container.begin(); i != iend; ) {
		if(i->second->m_enType == CRequestBase::_enRequestStock ) {
			if( utils::HasValue( Params ) ) {
				_StockParams p( Params );
				CRequest<StockParams>::CRequestPtr spStockRequest = boost::shared_dynamic_cast<CRequest<StockParams> >( i->second );
				if( !wcscmp( spStockRequest->m_recParams->Stock, p->Stock ) ) {
					m_Container.erase( i );
					break;
				}
				++i;
			}
			else i = m_Container.erase( i );
		}
		else ++i;
	}
}


void
CActivFeedContentGateway::CancelOptions(VARIANT Params)
{
	CRequestsContainer::iterator i, iend = m_Container.end();
	for(i = m_Container.begin(); i != iend; ) {
		if( i->second->m_enType == CRequestBase::_enRequestOption ) {
			if( utils::HasValue( Params ) ) {
				_OptionParams p( Params );
				CRequest<OptionParams>::CRequestPtr spStockRequest = boost::shared_dynamic_cast<CRequest<OptionParams> >( i->second );
				if( !wcscmp( spStockRequest->m_recParams->UnderlyingSymbol,p->UnderlyingSymbol ) ) {
					m_Container.erase( i );
					break;
				}
				++i;
			}
			else i = m_Container.erase( i );
		}
		else ++i;
	}
}


void
CActivFeedContentGateway::CancelFuture(VARIANT Params)
{
	CRequestsContainer::iterator i, iend = m_Container.end();
	for( i = m_Container.begin(); i != iend; ) {
		if( i->second->m_enType == CRequestBase::_enRequestFuture ) {
			if( utils::HasValue( Params ) ) {
				_FutureParams p( Params );
				CRequest<FutureParams>::CRequestPtr spStockRequest = boost::shared_dynamic_cast<CRequest<FutureParams> >( i->second );
				if( !wcscmp( spStockRequest->m_recParams->FutureSymbol, p->FutureSymbol ) ) {
					m_Container.erase( i );
					break;
				}
				++i;
			}
			else i = m_Container.erase(i);
		}
		else ++i;
	}
}


void
CActivFeedContentGateway::CancelFutures(VARIANT Params)
{
	CRequestsContainer::iterator i, iend = m_Container.end();
	for(i = m_Container.begin(); i != iend; ) {
		if( i->second->m_enType == CRequestBase::_enRequestFutures ) {
			if( utils::HasValue( Params ) ) {
				_FutureRootParams p( Params );
				CRequest<FutureRootParams>::CRequestPtr spStockRequest = boost::shared_dynamic_cast<CRequest<FutureRootParams> >( i->second );
				if( !wcscmp( spStockRequest->m_recParams->FuturesRootSymbol, p->FuturesRootSymbol )  ) {
					m_Container.erase( i );
					break;
				}
				++i;
			}
			else i = m_Container.erase( i );
		}
		else ++i;
	}
}


void
CActivFeedContentGateway::CancelFuturesOptions(VARIANT Params)
{
	CRequestsContainer::iterator i, iend = m_Container.end();
	for(i = m_Container.begin(); i != iend; ) {
		if( i->second->m_enType == CRequestBase::_enRequestFuturesOption ) {
			if( utils::HasValue( Params ) ) {
				_FuturesOptionParams p( Params );
				CRequest<FuturesOptionParams>::CRequestPtr spStockRequest = boost::shared_dynamic_cast<CRequest<FuturesOptionParams> >( i->second );
				if( !wcscmp( spStockRequest->m_recParams->FutureSymbol, p->FutureSymbol ) ) {
					m_Container.erase( i );
					break;
				}
				++i;
			}
			else i = m_Container.erase( i );
		}
		else ++i;
	}
}

void	
CActivFeedContentGateway::SetMarketOpenTime( const unsigned int t ){
	unsigned h = t /100;
	unsigned m = t %100;

	if( t && t < 2400 ) {
		SYSTEMTIME st;
		::GetLocalTime( &st );
		st.wHour = h;
		st.wMinute = m;
		EgLib::CEasternTimeZone zn;
		zn.DestinationTimeToLocalTime( st, m_MarketOpenTime );
	}
	else
		EgLib::CEgLibTraceManager::TraceText(LogWarning, __FUNCTION__ , _T("Market open time has not been set") );
}

StatusCode	
CActivFeedContentGateway::RequestFOP( _QuoteRequestPtr& m_spRequest )
{
	RequestBlock Blk1, Blk2, Blk3;
	SymbolIdMatchRequestParameters Params;
	RequestIdPtr spRequestID(new RequestId);

	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));
	Blk1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	Blk1.m_fieldIdList.push_back(Feed::FID_BID );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_OPEN );
	Blk1.m_fieldIdList.push_back( Feed::FID_OPEN_INTEREST );
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	Blk1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	Blk1.m_fieldIdList.push_back( Feed::FID_SETTLEMENT );
	Blk1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );

	Params.m_requestBlockList.push_back( Blk1 );
	Blk2.m_relationshipId = Feed::RELATIONSHIP_ID_FUTURE_ROOT;
	Blk2.m_fieldIdList.push_back(FID_CONTRACT_SIZE);
	Blk2.m_fieldIdList.push_back(FID_MINIMUM_TICK); 	
	Blk2.m_fieldIdList.push_back(FID_UNIT_OF_MEASURE); 
	Blk2.m_fieldIdList.push_back(FID_UNIT_OF_MEASURE_CURRENCY);
	Params.m_requestBlockList.push_back( Blk2 );
	std::string sKey = m_spRequest->Symbol;

	Params.m_symbolId = SymbolId(Feed::TABLE_NO_US_FUTURE_OPTION, sKey) ;
	Params.m_matchType = MatchRequestParameters::MATCH_TYPE_COMPOSITE;

	StatusCode SC = GetMatch::PostRequest(*this, *spRequestID,	Params);
	if( SC == STATUS_CODE_SUCCESS ) 
		AddLastQuoteRequestToMap("", m_spRequest, spRequestID );
	return SC;
}

StatusCode	
CActivFeedContentGateway::RequestFUT( _QuoteRequestPtr& m_spRequest )
{
	RequestBlock Blk1, Blk2;
	SymbolIdMatchRequestParameters Params;
	RequestIdPtr spRequestID(new RequestId);

	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));
	Blk1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	Blk1.m_fieldIdList.push_back(Feed::FID_BID );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_OPEN );
	Blk1.m_fieldIdList.push_back( Feed::FID_OPEN_INTEREST );
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	Blk1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	Blk1.m_fieldIdList.push_back( Feed::FID_SETTLEMENT );
	Blk1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	Params.m_requestBlockList.push_back( Blk1 );

	Params.m_matchType = MatchRequestParameters::MATCH_TYPE_COMPOSITE;

	std::string sKey = m_spRequest->Symbol;
	Params.m_symbolId = SymbolId( Feed::TABLE_NO_US_FUTURE, sKey );

	StatusCode SC = GetMatch::PostRequest(*this, *spRequestID,	Params);
	if( SC == STATUS_CODE_SUCCESS ) 
		AddLastQuoteRequestToMap("", m_spRequest, spRequestID );
	return SC;
}
StatusCode	
CActivFeedContentGateway::RequestIDX( _QuoteRequestPtr& m_spRequest, std::string& As )
{
	RequestBlock Blk1, Blk2, Blk3;
	GetMultiplePatternMatch::RequestParameters Params;
	std::string sKey = As + '.' + '*';
	RequestIdPtr spRequestID(new RequestId);

	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));
	Blk1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	Blk1.m_fieldIdList.push_back( Feed::FID_OPEN );
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_BID); 
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );

	if( m_MarketOpenTime.wYear ) {
		SYSTEMTIME st;
		::GetLocalTime( &st );
		int t1 = st.wHour * 100 + st.wMinute;
		int t2 = m_MarketOpenTime.wHour * 100 + m_MarketOpenTime.wMinute;
		if ( t1 < t2 ) {
			Blk1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_ASK );
			Blk1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_BID );
		}
	}
	Blk1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	Blk1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_CURRENCY );
	Params.m_requestBlockList.push_back( Blk1 );
	Params.m_tableNumber = Feed::TABLE_NO_INDEX;
	Params.m_symbolPattern = sKey;
	
	StatusCode SC = GetMultiplePatternMatch::PostRequest( *this, *spRequestID,	Params);
	if( SC == STATUS_CODE_SUCCESS ) 
		AddLastQuoteRequestToMap("", m_spRequest, spRequestID );
	return SC;
}


StatusCode	CActivFeedContentGateway::RequestLastQuote( _QuoteRequestPtr& spRequest, CActivFeedContentGateway::CRequestType enRequestType )
{
	if(spRequest == NULL)
		return STATUS_CODE_MISSING_ARGS;

	if( spRequest->Type == enMStart || spRequest->Type == enMStop )
		return STATUS_CODE_SUCCESS;

	std::string As = ""; 
	std::string Ae = "";
	std::string exchRegion = "USA";

	switch( spRequest->Type )
	{
	case enFOP:
		{
			return RequestFOP( spRequest );
		}
	case enFUT:
		{
			return RequestFUT( spRequest );
		}
	case enOST:
		break;
	case enOFT:
		break;
	case enSTK:
		{
			EgarSTK2Activ( spRequest->Exchange, Ae, exchRegion );
			spRequest->Rate = ActivFeedSettings::g_spUserSettings->Settings->GetRegionRate(exchRegion);
			return RequestSTK( spRequest, Ae, exchRegion );

		}
	case enIDX:
		{
			As = spRequest->Symbol;
			EgarIDX2Activ( As );
			return RequestIDX( spRequest, As );
		}
	case enOPT:
		{
			//EgarOPT2Activ( spRequest->Symbol, spRequest->Exchange, As, Ae );
			return RequestOPT( spRequest, enRequestType);
		}
	case enMetal:
	case enFX:
		return RequestFX( spRequest );
	default:;
	}
	return STATUS_CODE_FAILURE;
}


StatusCode	
CActivFeedContentGateway::RequestOPT( _QuoteRequestPtr& spRequest, CActivFeedContentGateway::CRequestType enRequestType )
{
	StatusCode SC = STATUS_CODE_SUCCESS;

	RequestBlock Blk1, Blk2, Blk3;

	std::string sKey = "";//As + '.' + xch;
	ConvertRequest(spRequest, sKey);

	if(enRegularRequest == enRequestType || enStartMultipleRequest == enRequestType)
	{
		Blk1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
		Blk1.m_fieldIdList.push_back(Feed::FID_BID );							
		Blk1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
		Blk1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
		Blk1.m_fieldIdList.push_back( Feed::FID_ASK );
		Blk1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
		Blk1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
		Blk1.m_fieldIdList.push_back( Feed::FID_OPEN );
		Blk1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
		Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
		Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
		Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
		Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
		//Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
		Blk1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
		Blk1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
		Blk1.m_fieldIdList.push_back( Feed::FID_OPEN_INTEREST );
		Blk1.m_fieldIdList.push_back( Feed::FID_CLOSE );

		if( m_MarketOpenTime.wYear ) {
			SYSTEMTIME st;
			::GetLocalTime( &st );
			int t1 = st.wHour * 100 + st.wMinute;
			int t2 = m_MarketOpenTime.wHour * 100 + m_MarketOpenTime.wMinute;
			if ( t1 < t2 ) {
				Blk1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_ASK );		// need this only if market still not open
				Blk1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_BID );
			}
		}

	}

	switch(enRequestType)
	{
	case enRegularRequest:
		{
			RequestIdPtr spRequestID(new RequestId);
			long lRequestID = InterlockedIncrement(&m_nextRequestId);
			spRequestID->Set(&lRequestID, sizeof(lRequestID));

			GetEqual::RequestParameters Params;
			Params.m_requestBlockList.push_back( Blk1 );
			Params.m_symbolId = SymbolId( Feed::TABLE_NO_US_EQUITY_OPTION, sKey );
			SC = GetEqual::PostRequest(*this, *spRequestID,	Params );
			if( SC == STATUS_CODE_SUCCESS ) 
				AddLastQuoteRequestToMap("", spRequest, spRequestID );

		}break;
	case enStartMultipleRequest:
		m_nextMultipleOptionRequestId = InterlockedIncrement(&m_nextRequestId);
		m_MultipleOptionsRequest.m_symbolIdList.clear();
		m_MultipleOptionsRequest.m_requestBlockList.clear();
		m_MultipleOptionsRequest.m_requestBlockList.push_back( Blk1 );

		// no break here
	case enMultipleRequest:
		{
			RequestIdPtr spRequestID(new RequestId);
			long lRequestID = m_nextMultipleOptionRequestId;
			spRequestID->Set(&lRequestID, sizeof(lRequestID));

			m_MultipleOptionsRequest.m_symbolIdList.push_back( SymbolId( Feed::TABLE_NO_US_EQUITY_OPTION, sKey ) );

			AddLastQuoteRequestToMap(sKey, spRequest, spRequestID );
		}break;
	case enEndMultipleRequest:
		{

			RequestIdPtr spRequestID(new RequestId);
			long lRequestID = m_nextMultipleOptionRequestId;
			spRequestID->Set(&lRequestID, sizeof(lRequestID));
			m_MultipleOptionsRequest.m_symbolIdList.push_back( SymbolId( Feed::TABLE_NO_US_EQUITY_OPTION, sKey ) );
			AddLastQuoteRequestToMap(sKey, spRequest, spRequestID );
			SC = GetMultipleEqual::PostRequest(*this, *spRequestID,	m_MultipleOptionsRequest );
		}   	
	}

	return SC;
}
StatusCode  
CActivFeedContentGateway::RequestFX( _QuoteRequestPtr&  spRequest )
{
	RequestBlock Blk1, Blk2, Blk3;
	GetEqual::RequestParameters Params;
	std::string sKey = spRequest->Symbol + ".TF";

	RequestIdPtr spRequestID( new RequestId );

	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));
	Blk1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	Blk1.m_fieldIdList.push_back(Feed::FID_BID );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_OPEN );
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	Blk1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	Blk1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_CURRENCY );

	Params.m_requestBlockList.push_back( Blk1 );
	Params.m_symbolId = SymbolId( Feed::TABLE_NO_FOREX, sKey );

	StatusCode SC = GetEqual::PostRequest( *this, *spRequestID,	Params );
	if( SC == STATUS_CODE_SUCCESS ) 
		AddLastQuoteRequestToMap("", spRequest, spRequestID );
	return SC;
}


StatusCode	
CActivFeedContentGateway::RequestSTK( _QuoteRequestPtr& m_spRequest, std::string& xch, const std::string& region)
{
	RequestBlock Blk1, Blk2, Blk3;
	GetEqual::RequestParameters Params;
	std::string sKey = m_spRequest->Symbol + '.' + xch;

	Blk1.m_fieldIdList.push_back( Feed::FID_CLOSE );

	if( m_MarketOpenTime.wYear ) {
		SYSTEMTIME st;
		::GetLocalTime( &st );
		int t1 = st.wHour * 100 + st.wMinute;
		int t2 = m_MarketOpenTime.wHour * 100 + m_MarketOpenTime.wMinute;
		if ( t1 < t2 ) {
			Blk1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_ASK );		// need this only if market still not open
			Blk1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_BID );
		}
	}
	RequestIdPtr spRequestID( new RequestId );

	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));
	Blk1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	Blk1.m_fieldIdList.push_back(Feed::FID_BID );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_OPEN );
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	Blk1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	Blk1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	Blk1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_CURRENCY );
	if( m_MarketOpenTime.wYear ) {
		SYSTEMTIME st;
		::GetLocalTime( &st );
		int t1 = st.wHour * 100 + st.wMinute;
		int t2 = m_MarketOpenTime.wHour * 100 + m_MarketOpenTime.wMinute;
		if ( t1 < t2 ) {
			Blk1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_ASK );		// need this only if market still not open
			Blk1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_BID );
		}
	}
	Params.m_requestBlockList.push_back( Blk1 );
	Params.m_symbolId = SymbolId( region == "USA" ? Feed::TABLE_NO_US_LISTING : Feed::TABLE_NO_AMERICAS_LISTING, sKey );


	StatusCode SC = GetEqual::PostRequest( *this, *spRequestID,	Params );
	if( SC == STATUS_CODE_SUCCESS ) 
		AddLastQuoteRequestToMap("", m_spRequest, spRequestID );
	return SC;
}

bool	CActivFeedContentGateway::SubscribeFX( _RealtimeQuoteAtomPtr& r)
{
	RequestBlock Blk1, Blk2, Blk3;
	GetEqual::RequestParameters Params;
	std::string sKey = r->m_spRequest->Symbol + ".TF";
	RequestIdPtr spRequestID(new RequestId);

	// make new request ID
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	// fill up request block
	Blk1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	Blk1.m_fieldIdList.push_back(Feed::FID_BID );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_OPEN );
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	Blk1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	Blk1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	Blk1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );

	Params.m_requestBlockList.push_back( Blk1 );
	Blk2.m_relationshipId = Feed::RELATIONSHIP_ID_SECURITY;
	Blk2.m_fieldIdList.push_back(Feed::FID_CURRENCY );
	Params.m_requestBlockList.push_back( Blk2 );

	Params.m_symbolId = SymbolId(Feed::TABLE_NO_FOREX, sKey );

	Params.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	Params.m_permissionLevel = Feed::PERMISSION_LEVEL_DEFAULT;

	StatusCode statusCode = GetEqual::PostRequest(*this, *spRequestID, Params);	
	if ( STATUS_CODE_SUCCESS == statusCode )
	{
		AddSubscribeRequestToMap("", r, spRequestID);
		return true;
	}
	return false;

}

bool	CActivFeedContentGateway::SubscribeSTK( _RealtimeQuoteAtomPtr& r, std::string& Ae, std::string& region )
{
	RequestBlock Blk1, Blk2, Blk3;
	GetEqual::RequestParameters Params;
	std::string sKey = r->m_spRequest->Symbol + '.' + Ae;
	RequestIdPtr spRequestID(new RequestId);

	// make new request ID
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	// fill up request block
	Blk1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	Blk1.m_fieldIdList.push_back(Feed::FID_BID );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_OPEN );
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	Blk1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	Blk1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	Blk1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	if( m_MarketOpenTime.wYear ) {
		SYSTEMTIME st;
		::GetLocalTime( &st );
		int t1 = st.wHour * 100 + st.wMinute;
		int t2 = m_MarketOpenTime.wHour * 100 + m_MarketOpenTime.wMinute;
		if ( t1 < t2 ) {
			Blk1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_ASK );		// need this only if market still not open
			Blk1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_BID );
		}
	}
	Params.m_requestBlockList.push_back( Blk1 );


	Blk2.m_relationshipId = Feed::RELATIONSHIP_ID_SECURITY;
	Blk2.m_fieldIdList.push_back(Feed::FID_CURRENCY );
	Params.m_requestBlockList.push_back( Blk2 );

	Blk3.m_relationshipId = Feed::RELATIONSHIP_ID_OPTION_ROOT;
	Blk3.m_fieldIdList.push_back(Feed::FID_SHARES_PER_CONTRACT);
	Params.m_requestBlockList.push_back( Blk3 );

	Params.m_symbolId = SymbolId( region == "USA" ? Feed::TABLE_NO_US_LISTING : Feed::TABLE_NO_AMERICAS_LISTING, sKey );

	Params.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	Params.m_permissionLevel = Feed::PERMISSION_LEVEL_DEFAULT;

	StatusCode statusCode = GetEqual::PostRequest(*this, *spRequestID, Params);	
	if ( STATUS_CODE_SUCCESS == statusCode )
	{
		AddSubscribeRequestToMap("", r, spRequestID);
		return true;
	}
	return false;
}

bool	CActivFeedContentGateway::SubscribeIDX( _RealtimeQuoteAtomPtr& r, std::string& Idx )
{
	RequestBlock Blk1, Blk2;
	GetMultiplePatternMatch::RequestParameters Params;
	RequestIdPtr spRequestID(new RequestId);

	// make new request ID
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	// fill up request block
	Blk1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	Blk1.m_fieldIdList.push_back( Feed::FID_BID); 
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK); 
	Blk1.m_fieldIdList.push_back( Feed::FID_OPEN );
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	Blk1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	Blk1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	if( m_MarketOpenTime.wYear ) {
		SYSTEMTIME st;
		::GetLocalTime( &st );
		int t1 = st.wHour * 100 + st.wMinute;
		int t2 = m_MarketOpenTime.wHour * 100 + m_MarketOpenTime.wMinute;
		if ( t1 < t2 ) {
			Blk1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_ASK );
			Blk1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_BID );
		}
	}
	Params.m_requestBlockList.push_back( Blk1 );


	Blk2.m_relationshipId = Feed::RELATIONSHIP_ID_SECURITY;
	Blk2.m_fieldIdList.push_back(Feed::FID_CURRENCY );
	Params.m_requestBlockList.push_back( Blk2 );


	Params.m_tableNumber = Feed::TABLE_NO_INDEX;
	Params.m_symbolPattern = Idx;
	//Params.m_maxResponseBlocks = 1;

	Params.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	Params.m_permissionLevel = Feed::PERMISSION_LEVEL_DEFAULT;

	StatusCode statusCode = GetMultiplePatternMatch::PostRequest( *this, *spRequestID,	Params);
	if( statusCode == STATUS_CODE_SUCCESS )
	{
		AddSubscribeRequestToMap("", r, spRequestID);
		return true;
	}
	return false;
}

bool	CActivFeedContentGateway::SubscribeOPT( _RealtimeQuoteAtomPtr& r, CActivFeedContentGateway::CRequestType enRequestType)
{
	StatusCode SC = STATUS_CODE_SUCCESS;

	RequestBlock Blk1, Blk2, Blk3;
	std::string sKey = ""; //Opt + '.' + xch;
	ConvertRequest(r->m_spRequest, sKey);

	if(enRegularRequest == enRequestType || enStartMultipleRequest == enRequestType)
	{
		Blk1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
		Blk1.m_fieldIdList.push_back(Feed::FID_BID );
		Blk1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
		Blk1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
		Blk1.m_fieldIdList.push_back( Feed::FID_ASK );
		Blk1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
		Blk1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
		Blk1.m_fieldIdList.push_back( Feed::FID_OPEN );
		Blk1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
		Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
		Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
		Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
		Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
		Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
		Blk1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
		Blk1.m_fieldIdList.push_back( Feed::FID_CLOSE );
		Blk1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
		Blk1.m_fieldIdList.push_back( Feed::FID_OPEN_INTEREST);
	}

	switch(enRequestType)
	{
	case enRegularRequest:
		{
			RequestIdPtr spRequestID(new RequestId);
			long lRequestID = InterlockedIncrement(&m_nextRequestId);
			spRequestID->Set(&lRequestID, sizeof(lRequestID));

			// fill up request block
			GetEqual::RequestParameters Params;
			Params.m_requestBlockList.push_back( Blk1 );

			Params.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
			Params.m_permissionLevel = Feed::PERMISSION_LEVEL_DEFAULT;
			Params.m_symbolId = SymbolId(Feed::TABLE_NO_UNDEFINED, sKey );

			StatusCode statusCode = GetEqual::PostRequest(*this, *spRequestID, Params);	
			if ( STATUS_CODE_SUCCESS == statusCode )
			{
				AddSubscribeRequestToMap("", r, spRequestID);
				return true;
			}
			return false;
		}break;
	case enStartMultipleRequest:
		m_nextMultipleOptionRequestId = InterlockedIncrement(&m_nextRequestId);
		m_MultipleOptionsRequest.m_symbolIdList.clear();
		m_MultipleOptionsRequest.m_requestBlockList.clear();
		m_MultipleOptionsRequest.m_requestBlockList.push_back( Blk1 );
		m_MultipleOptionsRequest.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
		m_MultipleOptionsRequest.m_permissionLevel = Feed::PERMISSION_LEVEL_DEFAULT;

		// no break here
	case enMultipleRequest:
		{
			RequestIdPtr spRequestID(new RequestId);
			long lRequestID = m_nextMultipleOptionRequestId;
			spRequestID->Set(&lRequestID, sizeof(lRequestID));
			m_MultipleOptionsRequest.m_symbolIdList.push_back( SymbolId( Feed::TABLE_NO_US_EQUITY_OPTION, sKey ) );

			AddSubscribeRequestToMap(sKey, r, spRequestID);

		}break;
	case enEndMultipleRequest:
		{

			RequestIdPtr spRequestID(new RequestId);
			long lRequestID = m_nextMultipleOptionRequestId;
			spRequestID->Set(&lRequestID, sizeof(lRequestID));
			m_MultipleOptionsRequest.m_symbolIdList.push_back( SymbolId( Feed::TABLE_NO_US_EQUITY_OPTION, sKey ) );

			AddSubscribeRequestToMap(sKey, r, spRequestID);
			SC = GetMultipleEqual::PostRequest(*this, *spRequestID,	m_MultipleOptionsRequest );
			if ( STATUS_CODE_SUCCESS != SC )
			{
				CAutoLock l( m_csPSMap );
				m_PSMap.erase( spRequestID );
			} 
		}   	
	}
	return SC == STATUS_CODE_SUCCESS;
}

bool	CActivFeedContentGateway::SubscribeFUT( _RealtimeQuoteAtomPtr& r )
{
	RequestBlock Blk1, Blk2;
	SymbolIdMatchRequestParameters Params;
	RequestIdPtr spRequestID(new RequestId);

	// make new request ID
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	// fill up request block
	Blk1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	Blk1.m_fieldIdList.push_back(Feed::FID_BID );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_OPEN );
	Blk1.m_fieldIdList.push_back( Feed::FID_OPEN_INTEREST );
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	Blk1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	Blk1.m_fieldIdList.push_back( Feed::FID_SETTLEMENT );
	Blk1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	Params.m_requestBlockList.push_back( Blk1 );

	std::string sKey = r->m_spRequest->Symbol;

	Params.m_symbolId =  SymbolId( Feed::TABLE_NO_US_FUTURE, sKey );
	Params.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	Params.m_permissionLevel = Feed::PERMISSION_LEVEL_DEFAULT;
	Params.m_matchType = MatchRequestParameters::MATCH_TYPE_COMPOSITE;

	StatusCode statusCode = GetMatch::PostRequest(*this, *spRequestID,	Params);
	if( statusCode == STATUS_CODE_SUCCESS )
	{
		AddSubscribeRequestToMap("", r, spRequestID);
		return true;
	}
	return false;	
}
bool	CActivFeedContentGateway::SubscribeFOP( _RealtimeQuoteAtomPtr& r )
{
	RequestBlock Blk1, Blk2, Blk3;
	SymbolIdMatchRequestParameters Params;
	RequestIdPtr spRequestID(new RequestId);

	// make new request ID
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	// fill up request block
	Blk1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	Blk1.m_fieldIdList.push_back(Feed::FID_BID );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	Blk1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_OPEN );
	Blk1.m_fieldIdList.push_back( Feed::FID_OPEN_INTEREST );
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	Blk1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	Blk1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	Blk1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	Blk1.m_fieldIdList.push_back( Feed::FID_SETTLEMENT );
	Blk1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	Params.m_requestBlockList.push_back( Blk1 );

	std::string sKey = r->m_spRequest->Symbol;
	Params.m_symbolId = SymbolId( Feed::TABLE_NO_US_FUTURE_OPTION, sKey ) ;

	Params.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	Params.m_permissionLevel = Feed::PERMISSION_LEVEL_DEFAULT;
	Params.m_matchType = MatchRequestParameters::MATCH_TYPE_COMPOSITE;

	StatusCode statusCode = GetMatch::PostRequest(*this, *spRequestID,	Params);
	if( statusCode == STATUS_CODE_SUCCESS )
	{
		AddSubscribeRequestToMap("", r, spRequestID);
		return true;
	}
	return false;	
}

bool CActivFeedContentGateway::SubscribeQuote(_QuoteRequestPtr& spRequest, CActivFeedContentGateway::CRequestType enType)
{
	if( spRequest->Type == enMStart || spRequest->Type == enMStop )
		return true;

	std::vector<std::string> Ae;
	std::vector<std::string> Ee;
	std::vector<std::string> exchangeRegions;
	std::string As = "";
	
	std::string exchange;

	const char *pt = spRequest->Exchange.c_str();
	while( pt && *pt )
	{
		if( *pt == ',' )
		{
			if( exchange.empty() || exchange == " ")	
				Ee.push_back( std::string("") );			// BBO
			else
				Ee.push_back( exchange );
			exchange.clear();
		}
		else
			if( isalpha(*pt ) )
				exchange += ( char ) *pt; 
		pt++;
	}

	if( !exchange.empty() ) 
		Ee.push_back( exchange );
	else
		if( Ee.empty() )
			Ee.push_back( std::string("") );	

	switch( spRequest->Type )
	{
	case enFOP:
		{
			_RealtimeQuoteAtomPtr rtu ( new _RealtimeQuoteAtom );
			rtu->m_spRequest->Exchange = spRequest->Exchange;
			rtu->m_spRequest->Symbol = spRequest->Symbol;
			rtu->m_spRequest->Type = spRequest->Type;
			rtu->m_spRequest->TryCount = spRequest->TryCount;
			if( !SubscribeFOP( rtu ) )
				return false;
		}
		break;
	case enFUT:
		{
			_RealtimeQuoteAtomPtr rtu ( new _RealtimeQuoteAtom );
			rtu->m_spRequest->Exchange = spRequest->Exchange;
			rtu->m_spRequest->Symbol = spRequest->Symbol;
			rtu->m_spRequest->Type = spRequest->Type;
			rtu->m_spRequest->TryCount = spRequest->TryCount;
			if( !SubscribeFUT( rtu ) )
				return false;
		}
		break;
	case enOST:
		break;
	case enOFT:
		break;
	case enSTK:
		{
			size_t t = Ee.size();
			for( size_t i = 0; i < t; i++ ){
				Ae.push_back( std::string("") );
				exchangeRegions.push_back( std::string("") );
			}

			SEgarSTK2Activ( Ee, Ae, exchangeRegions );
			for ( size_t i = 0; i < t; i++ )
			{
				_RealtimeQuoteAtomPtr rtu ( new _RealtimeQuoteAtom );
				rtu->m_spRequest->Exchange = Ee[i];
				rtu->m_spRequest->Symbol = spRequest->Symbol;
				rtu->m_spRequest->Type = spRequest->Type;
				rtu->m_spRequest->TryCount = spRequest->TryCount;
				spRequest->Rate	=	ActivFeedSettings::g_spUserSettings->Settings->GetRegionRate(exchangeRegions[i]);	
				rtu->m_spRequest->Rate	=	spRequest->Rate;
				if( !SubscribeSTK( rtu, Ae[i], exchangeRegions[i] ) )
					return false;
			}
		}
		break;
	case enIDX:
		{
			As = '=' + spRequest->Symbol + ".*";
			_RealtimeQuoteAtomPtr rtu ( new _RealtimeQuoteAtom );
			rtu->m_spRequest->Exchange = spRequest->Exchange;
			rtu->m_spRequest->Symbol = spRequest->Symbol;
			rtu->m_spRequest->Type = spRequest->Type;
			rtu->m_spRequest->TryCount = spRequest->TryCount;
			if( !SubscribeIDX( rtu, As ) )
				return false;
		}
		break;
	case enOPT:
		{
			//As = spRequest->Symbol;
			size_t t = Ee.size();

			//for( size_t i = 0; i < t; i++ )
			//	Ae.push_back( std::string("") );

			//SEgarOPT2Activ( Ee, Ae );

			/*basic_string <char>::iterator it = ( As.end ( ) - 2 );
			As.insert ( it , '/' );
			it+=2;
			As.insert ( it , '/' );*/

			for ( size_t i = 0; i < t; i++ )
			{
				_RealtimeQuoteAtomPtr rtu( new _RealtimeQuoteAtom );
				rtu->m_spRequest->Exchange = Ee[i];
				rtu->m_spRequest->Symbol = spRequest->Symbol;
				rtu->m_spRequest->Type = spRequest->Type;
				rtu->m_spRequest->TryCount = spRequest->TryCount;
				if( !SubscribeOPT( rtu, enType ) )
					return false;
			}
		}
		break;
	case enMetal:
	case enFX:
		{
			_RealtimeQuoteAtomPtr rtu ( new _RealtimeQuoteAtom );
			rtu->m_spRequest->Exchange = spRequest->Exchange;
			rtu->m_spRequest->Symbol = spRequest->Symbol;
			rtu->m_spRequest->Type = spRequest->Type;
			rtu->m_spRequest->TryCount = spRequest->TryCount;
			if( !SubscribeFX( rtu ) )
				return false;

		}break;
	default:
		return false;
	}
	return true;
}



bool	CActivFeedContentGateway::Activate()
{
	StatusCode SC;

	if( m_bActivated ) return true;

	ServiceInstanceList serviceInstanceList;

	//statusCode = ServiceApi::FindServices(*this, "Service.ContentGateway", AGENT_SCOPE_GLOBAL, "", serviceInstanceList);
	SC = ServiceApi::FindServices(*this, "Service.WorkstationService", AGENT_SCOPE_GLOBAL, "", serviceInstanceList);

	if ( STATUS_CODE_SUCCESS != SC ) {
		EgLib::CEgLibTraceManager::Trace(LogError, __FUNCTION__ , _T("Failed to find service, %s"), StatusCodeToString( SC ).c_str());
		return false;
	}

	ContentGatewayClient::ConnectParameters conParams;
	const ServiceInstance &serviceInstance = serviceInstanceList.front();
	conParams.m_serviceId = serviceInstance.m_serviceId;
	conParams.m_url = serviceInstanceList.front().m_serviceAccessPointList.front().m_url;
	EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ , _T("Connecting to service, %s"), 
		serviceInstanceList.front().m_serviceAccessPointList.front().m_url.c_str());
	conParams.m_userId   = ActivFeedSettings::g_spUserSettings->Settings->Settings[_T("Login")];
	conParams.m_password = ActivFeedSettings::g_spUserSettings->Settings->Settings[_T("Password")];
	EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ , _T("Login:<%s> Password:<%s>"), 
		( conParams.m_userId.length() ? conParams.m_userId.c_str() : _T("Empty") ), 
		( conParams.m_password.length() ? conParams.m_password.c_str() : _T("Empty") ) );
	if( conParams.m_userId.empty() || conParams.m_password.empty() ){
		EgLib::CEgLibTraceManager::TraceText(LogError, __FUNCTION__ , _T("Illegal user name or password") );
		EgLib::CComErrorWrapper::ThrowError(E_FAIL, _T( "Bad user name or password" ) );
	}
	SC = Connect( conParams );
	EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("Connect() returned %s"), StatusCodeToString( SC ).c_str());
	m_bActivated = STATUS_CODE_SUCCESS == SC;
	if ( !m_bActivated ) {
		Disconnect();
		EgLib::CComErrorWrapper::ThrowError(E_FAIL, StatusCodeToString( SC ).c_str() );
	}
	return m_bActivated;
}

StatusCode CActivFeedContentGateway::RequestIndex(CRequest<StockParams>::CRequestPtr& spStockRequest, CResponse<StockResultsEx>::CResponsePtr spResponce)
{
	_bstr_t bsStock(L"");
	_bstr_t bsExchange(L"");
	if(spStockRequest->m_recParams->Stock)
		bsStock = spStockRequest->m_recParams->Stock;
	if(spStockRequest->m_recParams->Exchange)
		bsExchange = spStockRequest->m_recParams->Exchange;

	GetMultiplePatternMatch::RequestParameters getMultiplePatternMatchListRequestParameters;
	getMultiplePatternMatchListRequestParameters.m_tableNumber = TABLE_NO_INDEX;

	getMultiplePatternMatchListRequestParameters.m_symbolPattern = "=" + bsStock + ".*";;
	RequestBlock requestBlock;

	requestBlock.m_fieldIdList.push_back(FID_NAME);
	requestBlock.m_fieldIdList.push_back(FID_CLOSE);
	requestBlock.m_fieldIdList.push_back(Feed::FID_PREVIOUS_ASK);
	requestBlock.m_fieldIdList.push_back(Feed::FID_PREVIOUS_BID);


	requestBlock.m_fieldIdList.push_back(Feed::FID_BID);
	requestBlock.m_fieldIdList.push_back(Feed::FID_ASK);
	requestBlock.m_fieldIdList.push_back(Feed::FID_OPEN);
	requestBlock.m_fieldIdList.push_back(Feed::FID_CUMULATIVE_VOLUME);
	requestBlock.m_fieldIdList.push_back(Feed::FID_TRADE_HIGH);
	requestBlock.m_fieldIdList.push_back(Feed::FID_TRADE_LOW);
	requestBlock.m_fieldIdList.push_back(Feed::FID_TRADE_SIZE);
	requestBlock.m_fieldIdList.push_back(Feed::FID_CURRENCY);

	requestBlock.m_fieldIdList.push_back(Feed::FID_PRIMARY_EXCHANGE);
	//requestBlock.m_fieldIdList.push_back(Feed::FID_DIVIDEND /*FID_NEXT_DIVIDEND_1*/); /* DIVIDEND */
	requestBlock.m_fieldIdList.push_back(Feed::FID_NEXT_DIVIDEND_1); /* DIVIDEND */
	//requestBlock.m_fieldIdList.push_back(Feed::FID_EX_DATE /*FID_NEXT_DIVIDEND_1_DATE*/); /*EXDATE*/
	requestBlock.m_fieldIdList.push_back(Feed::FID_NEXT_DIVIDEND_1_DATE); /*EXDATE*/
	requestBlock.m_fieldIdList.push_back(Feed::FID_DIVIDEND_FREQUENCY_CODE);

	getMultiplePatternMatchListRequestParameters.m_requestBlockList.push_back(requestBlock);

	//getMultiplePatternMatchListRequestParameters.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	//getMultiplePatternMatchListRequestParameters.m_permissionLevel = /*(m_isDelayed ? PERMISSION_LEVEL_DELAYED : */PERMISSION_LEVEL_DEFAULT/*)*/;

	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetMultiplePatternMatch::PostRequest(*this, *spRequestID, getMultiplePatternMatchListRequestParameters));

	CAutoLock lock(m_csContainer);
	m_Container[spRequestID] = spStockRequest;

	return STATUS_CODE_SUCCESS;
}

void	CActivFeedContentGateway::OnRecordUpdate( Activ::HeapMessage &update )
{
	RecordUpdate recordUpdate;

	if( !IsValidResponse(update) )
		return;
	if ((STATUS_CODE_SUCCESS == RecordUpdateHelper::Deserialize(*this, update, recordUpdate)) &&
		(STATUS_CODE_SUCCESS == m_FieldListValidator.Initialize(recordUpdate.m_fieldData)) )
	{
		if( recordUpdate.m_fieldData.IsEmpty() )
			return;
		CAutoLock l1( m_csSMap );
		CSMap::iterator it = m_SMap.find( recordUpdate.m_requestKey.m_symbol );
		if( it == m_SMap.end() )
			return;
		_RealtimeQuoteAtomPtr r = it->second;
		FieldListValidator::ConstIterator fit, fitEnd;
		CAutoLock l( r->m_Lock );
		for ( fit = m_FieldListValidator.Begin(), fitEnd = m_FieldListValidator.End(); fit != fitEnd; fit++ ) 
		{
			const FieldListValidator::Field& fld = fit.GetField();
			if ( FIELD_STATUS_DEFINED == fld.m_fieldStatus ) 
			{
				if( m_bFilterOffHourUpdates && ( fld.m_rules & FieldListValidator::Field::RULE_DOES_NOT_UPDATE_LAST_VALUE ) )
					continue;
				ApplyUpdateToQUI( fld, r->m_spResponse.get(), r->m_spRequest.get());
				if( !r->m_bUpdate )
				{
					InterlockedIncrement(&m_UpdateCount);
					r->m_bUpdate = true;
				}
			}
		}
	}
}

void	CActivFeedContentGateway::ApplyUpdateToQUI(  const FieldListValidator::Field& fld, _QuoteInfo *p , _QuoteRequest *pRequest)
{
	double dRate	=	1.0;
	if (pRequest)
		dRate = pRequest->Rate;

	switch( fld.m_fieldId) 
	{
	case Feed::FID_OPEN_INTEREST:
		{
			Activ::UInt bidsize;
			IFieldType* pUpdateField = static_cast<Activ::UInt*>(&bidsize);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->OpenInterest = static_cast<long>( bidsize.Get());
		}
		break;
	case Feed::FID_CURRENCY:
		{
			Activ::TextArray pText;
			IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->Currency = pText.ToString();
		}
		break;
	case Feed::FID_BID:
		{
			Activ::TRational bid;
			IFieldType* pUpdateField = static_cast<Activ::TRational*>(&bid);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->BidPrice = bid.Get() * dRate;
			Activ::TRational::Tick t = bid.GetTick();
			switch( t )
			{
			case Activ::TRational::TICK_DOWN:
				p->PriceTick |= enBidDnTick;
				break;
			case Activ::TRational::TICK_UP:
				p->PriceTick |= enBidUpTick;
				break;
			default:;
			}
		}
		break;
	case Feed::FID_BID_SIZE:
		{
			Activ::UInt bidsize;
			IFieldType* pUpdateField = static_cast<Activ::UInt*>(&bidsize);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->BidSize = static_cast<long>( bidsize.Get());
		}
		break;
	case Feed::FID_BID_EXCHANGE:
		{
			Activ::TextString pText;
			IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->BidExchange = pText.ToString();
		}
		break;
	case Feed::FID_ASK:
		{
			Activ::TRational ask;
			IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->AskPrice = ask.Get() * dRate;
			Activ::TRational::Tick t = ask.GetTick();
			switch( t )
			{
			case Activ::TRational::TICK_DOWN:
				p->PriceTick |= enAskDnTick;
				break;
			case Activ::TRational::TICK_UP:
				p->PriceTick |= enAskUpTick;
				break;
			default:;
			}
		}
		break;
	case Feed::FID_ASK_SIZE:
		{
			Activ::UInt asksize;
			IFieldType* pUpdateField = static_cast<Activ::UInt*>(&asksize);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->AskSize = static_cast<long>(asksize.Get() );
		}
		break;
	case Feed::FID_ASK_EXCHANGE:
		{
			Activ::TextString pText;
			IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->AskExchange = pText.ToString();
		}
		break;
	case Feed::FID_TRADE_EXCHANGE:
		{
			Activ::TextString pText;
			IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->Exchange = pText.ToString();
		}
		break;
	case Feed::FID_OPEN:
		{
			Activ::Rational opn;
			IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->OpenPrice = opn.Get() * dRate;
		}
		break;
	case Feed::FID_LAST_UPDATE_DATE:
		{
			Activ::Date dt;
			EgLib::vt_date	pdate( p->UpdateDateTime );
			IFieldType* pUpdateField = static_cast<Activ::Date*>(&dt);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			int year = dt.GetYear();
			int month = dt.GetMonth();
			int day = dt.GetDay();
			EgLib::vt_date d( year, month, day, 0/*pdate.get_hour()*/, 0/*pdate.get_minute()*/, 0/*pdate.get_second()*/ );
			p->UpdateDateTime = d;
		}
		break;
	case Feed::FID_LAST_UPDATE_TIME:
		{
			Activ::Time dt;
			EgLib::vt_date	pdate( p->UpdateDateTime );
			IFieldType* pUpdateField = static_cast<Activ::Time*>(&dt);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			int hour = dt.GetHour();
			int minute = dt.GetMinute();
			int second = dt.GetSecond();
			EgLib::vt_date d( pdate.get_year(), pdate.get_month(), pdate.get_day(),hour, minute, second );
			p->UpdateDateTime = d;
		}
		break;
	case Feed::FID_CUMULATIVE_VOLUME:
		{
			Activ::UInt volume;
			IFieldType* pUpdateField = static_cast<Activ::UInt*>(&volume);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->Volume = static_cast<long>(volume.Get());
		}
		break;
	case Feed::FID_CLOSE:
		{
			Activ::Rational opn;
			IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->ClosePrice = opn.Get() * dRate;
		}
		break;
	case Feed::FID_CONTRACT_SIZE:
	case Feed::FID_SHARES_PER_CONTRACT:
		{
			Activ::UInt asksize;
			IFieldType* pUpdateField = static_cast<Activ::UInt*>(&asksize);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->LotSize = static_cast<long>(asksize.Get());
		}
		break;
	case Feed::FID_TRADE:
		{
			Activ::TRational ask;
			IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->LastPrice = ask.Get() * dRate;
			Activ::TRational::Tick t = ask.GetTick();
			switch( t )
			{
			case Activ::TRational::TICK_DOWN:
				p->PriceTick |= enLastSaleDnTick;
				break;
			case Activ::TRational::TICK_UP:
				p->PriceTick |= enLastSaleUpTick;
				break;
			default:;
			}
		}
		break;
	case Feed::FID_TRADE_HIGH:
		{
			Activ::Rational opn;
			IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->HighPrice = opn.Get() * dRate;
		}
		break;
	case Feed::FID_TRADE_LOW:
		{
			Activ::Rational opn;
			IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->LowPrice = opn.Get() * dRate;
		}
		break;
	case Feed::FID_NET_CHANGE:
		{
			Activ::TRational ask;
			IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
			(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
			p->NetChange = ask.Get() * dRate;
		}
		break;
	case Feed::FID_QUOTE_DATE:
	case Feed::FID_TRADE_TIME:
	case Feed::FID_TRADE_DATE:
	case Feed::FID_BID_TIME:
	case Feed::FID_ASK_TIME: 
	case Feed::FID_CUMULATIVE_PRICE:
	case Feed::FID_CUMULATIVE_VALUE:
	case Feed::FID_TRADE_COUNT:
	case Feed::FID_PERCENT_CHANGE:
		break;
	default:;
		p->Status = enQuoteStatus_Unreliable;
	}
}

bool	CActivFeedContentGateway::OnUnsubscribe( _QuoteRequestPtr& m_spRequest )
{
	bool rc = true;
	if (!m_spRequest) // unsubscribe from all subscriptions
	{
		CActivFeedAppication* pApp = dynamic_cast<CActivFeedAppication*>(&GetApplication());

		// delete all pending subscriptions
		CAutoLock l( m_csPSMap );
		m_PSMap.clear();

		// unsubscribe from all
		CAutoLock l1( m_csSMap );
		CSMap::iterator	it = m_SMap.begin(), itend = m_SMap.end();
		for(; it != itend; ){
			_RealtimeQuoteAtomPtr r= it->second;
			if ( Unsubscribe( r->m_Cookie) != STATUS_CODE_SUCCESS ) {
				std::string es("Unable to unsubscribe from " );
				es += r->m_spRequest->Symbol + "." + r->m_spRequest->Exchange;
				pApp->OnBIError( enInternalError, es.c_str(), enNoRequest, r->m_spRequest );
				rc = false;
				it++;
			}
			else{
				EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ ,\
					_T("Unsubsribed from [%s.%s[%d]]"), r->m_spRequest->Symbol.c_str(), r->m_spRequest->Exchange.c_str(), r->m_spRequest->Type );
				it = m_SMap.erase( it );
			}
		}
		m_UpdateCount = 0;
		return rc;
	}
	else // unsubscribe from particular subscription
	{
		CActivFeedAppication* pApp = dynamic_cast<CActivFeedAppication*>(&GetApplication());
		CAutoLock l( m_csSMap );

		for( CSMap::iterator it = m_SMap.begin(), itend = m_SMap.end(); it != itend; it++ ){
			_RealtimeQuoteAtomPtr r = it->second;
			if( m_spRequest->Symbol == r->m_spRequest->Symbol && m_spRequest->Exchange == r->m_spRequest->Exchange && m_spRequest->Type == r->m_spRequest->Type ) {
				if (r->m_bUpdate ) {
					CAutoLock l(r->m_Lock );
					r->m_bUpdate = false;
					InterlockedDecrement(&m_UpdateCount);
				}
				if ( Unsubscribe(r->m_Cookie) != STATUS_CODE_SUCCESS ) {
					std::string es("Unable to unsubscribe from " );
					es += r->m_spRequest->Symbol + "." + r->m_spRequest->Exchange;
					pApp->OnBIError( enInternalError, es.c_str(), enNoRequest, r->m_spRequest );
					rc = false;
				}
				else 
					it = m_SMap.erase( it );
				return rc;
			}
		}
		pApp->OnBIError( enInternalError, "Symbol was not found in subscriptions", enNoRequest, m_spRequest );
		return false;
	}
}

void	CActivFeedContentGateway::CopyUpdatedQuotes( QuoteUpdateFullInfo *pqufi, UINT size )
{
	CAutoLock l( m_csSMap );
	CSMap::const_iterator it = m_SMap.begin(), itend = m_SMap.end();
	UINT c = 0;

	EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("Begin sending RTU, Total %d subscriptions, updated - %d"), m_SMap.size(), m_UpdateCount  );
	for( ; it != itend && c < size; it++ )
	{
		_RealtimeQuoteAtomPtr r = it->second;
		{
			CAutoLock l( r->m_Lock );
			if ( r->m_bUpdate)	{
				//_QuoteUpdateFullInfo fullinfo;
				QuoteUpdateFullInfo*  fullinfo = &pqufi[c];
				//fullinfo.Init();
				if( r->m_spRequest->TryCount ){
					switch( r->m_spRequest->Type) {
					case enSTK:
						r->m_spRequest->Type = enIDX;
						break;
					case enIDX:
					default:
						r->m_spRequest->Type = enSTK;
					}
					r->m_spRequest->TryCount = 0;
				}
				fullinfo->Params.Type = r->m_spRequest->Type;
				fullinfo->Params.Symbol = SysAllocString(CA2W(r->m_spRequest->Symbol.c_str()));
				fullinfo->Params.Exchange = SysAllocString(CA2W(r->m_spRequest->Exchange.c_str()));

				fullinfo->Info.UpdateDateTime = r->m_spResponse->UpdateDateTime;
				fullinfo->Info.BidPrice= r->m_spResponse->BidPrice;		
				fullinfo->Info.AskPrice= r->m_spResponse->AskPrice;		
				fullinfo->Info.BidSize= r->m_spResponse->BidSize;		
				fullinfo->Info.AskSize= r->m_spResponse->AskSize;		
				fullinfo->Info.PriceTick= r->m_spResponse->PriceTick;	
				fullinfo->Info.LotSize= r->m_spResponse->LotSize;		
				fullinfo->Info.LastPrice= r->m_spResponse->LastPrice;	
				fullinfo->Info.Volume= r->m_spResponse->Volume;	
				fullinfo->Info.OpenPrice= r->m_spResponse->OpenPrice;	
				fullinfo->Info.ClosePrice= r->m_spResponse->ClosePrice;	
				fullinfo->Info.HighPrice= r->m_spResponse->HighPrice;	
				fullinfo->Info.LowPrice= r->m_spResponse->LowPrice;		
				fullinfo->Info.NetChange= r->m_spResponse->NetChange;	
				fullinfo->Info.Status = r->m_spResponse->Status;
				fullinfo->Info.OpenInterest = r->m_spResponse->OpenInterest;

				fullinfo->Info.Currency = SysAllocString(CA2W(r->m_spResponse->Currency.c_str()) );
				if( r->m_spResponse->AskExchange.length() )
					Activ2Egar( r->m_spResponse->AskExchange, r->m_spRequest->Type );
				if( r->m_spResponse->BidExchange.length() )
					Activ2Egar( r->m_spResponse->BidExchange, r->m_spRequest->Type );
				if( r->m_spResponse->Exchange.length() )
					Activ2Egar( r->m_spResponse->Exchange, r->m_spRequest->Type );

				fullinfo->Info.Exchange   = SysAllocString(CA2W( r->m_spResponse->Exchange.c_str())    );
				fullinfo->Info.BidExchange= SysAllocString(CA2W( r->m_spResponse->BidExchange.c_str()) );
				fullinfo->Info.AskExchange= SysAllocString(CA2W( r->m_spResponse->AskExchange.c_str()) );

				EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("RTU - %S.%S[%d]"), SafeBSTR( pqufi[c].Params.Symbol), SafeBSTR( pqufi[c].Params.Exchange), pqufi[c].Params.Type );
				r->m_bUpdate = false;
				InterlockedDecrement( &m_UpdateCount );
				if( m_UpdateCount < 0 )
					m_UpdateCount = 0;
				c++;
			}
		}
	}

	EgLib::CEgLibTraceManager::TraceText(LogWarning, __FUNCTION__ , _T("End sending RTU") );
}
void
CActivFeedContentGateway::OnGetEqualResponse(HeapMessage &response)
{
	if (!IsValidResponse( response ) ){
		int err = response.GetStatusCode();
		CActivFeedAppication* pApplication = dynamic_cast<CActivFeedAppication*>(&GetApplication());
		if(pApplication){
			if(STATUS_CODE_NOT_FOUND == err){
				RequestIdPtr pRequestID(new RequestId(response.GetRequestId()));
				CRequestBasePtr pRequest = GetRequest(pRequestID);
				if(pRequest)
				{
					switch(pRequest->m_enType)
					{
					case CRequestBase::_enRequestStock:
						{
							CRequest<StockParams>::CRequestPtr spOptRequest = boost::shared_dynamic_cast<CRequest<StockParams> >(pRequest);
							CComVariant vtRequest;
							spOptRequest->m_recParams.CopyTo(vtRequest);
							pApplication->OnSPError(enNoDataAvailableForSymbol, "No data received", enRequestStock, vtRequest);
						}
						break;
					case CRequestBase::_enRequestOption:
						{
							CRequest<OptionParams>::CRequestPtr spOptRequest = boost::shared_dynamic_cast<CRequest<OptionParams> >(pRequest);
							CComVariant vtRequest;
							spOptRequest->m_recParams.CopyTo(vtRequest);
							pApplication->OnSPError(enNoDataAvailableForSymbol, "No data received", enRequestOptions, vtRequest);
						}
						break;
					}
					DeleteRequest(pRequestID);
					return;
				}
			}
		}

	}

	RequestIdPtr pRequestID(new RequestId(response.GetRequestId()));
	CRequestBasePtr pRequest = GetRequest(pRequestID);

	if(pRequest)
	{
		bool IsLast = false;
		if(IsCompleteResponse(response))
			IsLast = true;

		switch(pRequest->m_enType)
		{
		case CRequestBase::_enRequestStock:
			{
				CRequest<StockParams>::CRequestPtr spStockRequest = boost::shared_dynamic_cast<CRequest<StockParams> >(pRequest);
				OnStock(spStockRequest, response);
				if(IsLast)
					DeleteRequest(pRequestID);
			}
			break;
		case CRequestBase::_enRequestOption:
			{
				SaveOptionRoots(response);
				if(IsLast)
					if( !m_optionRoots.empty() )
						RequestOptionsByRoot(pRequestID);
					else 
						RequestIndexOptions(pRequestID);
			}
			break;
		}
	}
	else
	{
		// try to find request in RequestLastQuote map
		_QuoteAtomPtr p;
		{
			CLUMap::iterator itr;
			CAutoLock lock(m_csLU);
			itr  =  m_LU.find( pRequestID );
			if( itr != m_LU.end() )
				p = itr->second.begin()->second;
		}
		if( p )
		{
			GetEqual::ResponseParameters Params;
			ResponseBlock::Status rbStatus	= ResponseBlock::STATUS_SUCCESS;
			StatusCode statusCode			= GetEqual::Deserialize(*this, response, Params);

			CActivFeedAppication* pApp = ApplyResponseToQUI(Params, p->m_spResponse.get(), p->m_spRequest->Type == enOPT ? true : false, p->m_spRequest.get(), &rbStatus);
			if( STATUS_CODE_SUCCESS == statusCode ) 
			{
				if( rbStatus != ResponseBlock::STATUS_SUCCESS /*&& rbStatus != ResponseBlock::STATUS_STALE*/ )	
				{
					pApp->OnBIError( enSymbolNotSupported, ResponseBlock::StatusToString( rbStatus ).c_str(), enRequestLastQuote, p->m_spRequest );
					pApp = NULL;
				}
				else
					if( pApp && IsCompleteResponse(response))	// notify client
						pApp->OnRequstLast( p->m_spRequest, p->m_spResponse );
			}
			CAutoLock lock(m_csLU);
			m_LU.erase( m_LU.find( pRequestID) );
			return;
		}

		// try to find request in pending subscriptions request
		bool DeleteRecord = false;
		CActivFeedAppication* pApp = NULL;
		_RealtimeQuoteAtomPtr r;

		CPSMap::iterator pitr;
		{
			CAutoLock lock( m_csPSMap );
			pitr = m_PSMap.find( pRequestID );
			if( pitr != m_PSMap.end() )
				r = pitr->second.begin()->second;
		}
		if( r ) 
		{
			GetEqual::ResponseParameters Params;
			StatusCode statusCode = GetEqual::Deserialize(*this, response, Params);

			if( STATUS_CODE_SUCCESS == statusCode )
			{
				CAutoLock l( r->m_Lock );
				pApp = ApplyResponseToQUI( Params, r->m_spResponse.get(), true, r->m_spRequest.get() );
				if( !r->m_bUpdate ) 
					r->m_bUpdate = true;
			}

			if( IsCompleteResponse(response) )
			{
				if( STATUS_CODE_SUCCESS == statusCode )
				{
					ResponseBlockList rbl = Params.m_responseBlockList;
					ResponseBlockList::const_iterator it, itend = rbl.end();
					for( it = rbl.begin(); it != itend; it++ )
					{
						const ResponseBlock& rb = *it;
						if( rb.IsValidResponse() && rb.IsResponseKeyDefined() )
						{
							CAutoLock lc( m_csSMap );
							CAutoLock lc1( m_csPSMap );
							CAutoLock lc2( r->m_Lock );
							r->m_Cookie = Params.m_subscriptionCookie;
							m_SMap[ rb.m_responseKey.m_symbol ]= r;
							m_PSMap.erase( m_PSMap.find( pRequestID ) );
							InterlockedIncrement(&m_UpdateCount);
							break;
						}
						else
						{
							ResponseBlockList::const_iterator itIsLast = it+1;
							if((r->m_spRequest->Type != enFX && r->m_spRequest->Type != enMetal) || itIsLast == itend)
							{
								int safetc = r->m_spRequest->TryCount;
								pApp->OnBIError( enSymbolNotSupported, ResponseBlock::StatusToString( rb.m_status ).c_str(), enSubscribeQuote, r->m_spRequest );
								if( r->m_spRequest->TryCount < safetc ){
									CAutoLock lc1( m_csPSMap );
									m_PSMap.erase( m_PSMap.find( pRequestID ) );
								}

								pApp = NULL;
								break;
							}
						}
					}
				}
				if( pApp )
					pApp->OnSubscribed( r->m_spRequest );
			}
		}
	}
}

void 
CActivFeedContentGateway::OnGetMatchResponse(HeapMessage &response)
{
	GetMatch::ResponseParameters Params;
	StatusCode statusCode = GetMatch::Deserialize(*this, response, Params);
	OnResponse(response,
		Params,
		RequestIdPtr(new RequestId(response.GetRequestId())),
		IsCompleteResponse(response),
		IsValidResponse(response)
		);
}

void
CActivFeedContentGateway::OnGetMultipleEqualResponse(HeapMessage &response)
{
	GetMultipleEqual::ResponseParameters Params;
	StatusCode statusCode = GetMultipleEqual::Deserialize(*this, response, Params);
	OnResponse(response, 
		Params,  
		RequestIdPtr(new RequestId(response.GetRequestId())),
		IsCompleteResponse(response),
		IsValidResponse(response)
		);
}

template<typename T>
void  CActivFeedContentGateway::OnResponse(HeapMessage &response,  T& Params,  RequestIdPtr pRequestID, bool bIsComplete, bool bIsValid)
{
	ResponseBlockList& rbl = Params.m_responseBlockList;
	ResponseBlockList::iterator it = rbl.begin(), itend = rbl.end();
	bool Magic = false;
	bool NotFound = true;

	CLUMap::iterator itr;
	CActivFeedAppication* pApp = dynamic_cast<CActivFeedAppication*>(&GetApplication());
	_QuoteAtomPtr p;
	CLMap* pMap = NULL; 
	{
		CAutoLock lock(m_csLU);
		itr =  m_LU.find( pRequestID );
		if( itr != m_LU.end() )
		{
			pMap = &itr->second;
			p = pMap->begin()->second;
		}
	}

	if(pMap && (pMap->size() > 1L || pMap->begin()->first.length())	)
	{
		//Multiple options request
		// process request last
		for (;it!=itend;it++) 
		{
			ResponseBlock& rb = *it;
			if( rb.IsValidResponse() ) 
			{
				CLMap::iterator itrRequest = pMap->find(rb.m_requestKey.m_symbol);
				if(itrRequest != pMap->end())
				{	
					_QuoteAtomPtr& spData = itrRequest->second;
					//StatusCode statusCode = GetMultipleEqual::Deserialize(*this, response, Params);
					if( rb.m_status != ResponseBlock::STATUS_SUCCESS /*&& rb.m_status != ResponseBlock::STATUS_STALE*/ )
					{
						pApp->OnBIError( enSymbolNotSupported, ResponseBlock::StatusToString( rb.m_status ).c_str(), enRequestLastQuote, spData->m_spRequest );
						pMap->erase(itrRequest);
						continue;
					}
					ResponseBlock::Status rbStatus	= ResponseBlock::STATUS_SUCCESS;
					ParseResponseBlock( rb, spData->m_spResponse.get(), spData->m_spRequest->Type == enOPT ? true : false, spData->m_spRequest.get());
					pApp->OnRequstLast( spData->m_spRequest, spData->m_spResponse );
					pMap->erase(itrRequest);
				}
				else
					ATLASSERT(FALSE);
			}
		}
		if(bIsComplete)
		{
			// Notify Client;
			CLMap::iterator itrReq = pMap->begin();
			while(itrReq != pMap->end())
			{
				_QuoteAtomPtr& unsent =	itrReq->second;
				pApp->OnRequstLast( unsent->m_spRequest, unsent->m_spResponse);
				++itrReq;
			}

			{
				CAutoLock lock(m_csLU);
				m_LU.erase( m_LU.find( pRequestID) );
			}

		}
		return;
	}
	if(!pMap) 
	{
		CSMap* pSubsMap = NULL; 

		_RealtimeQuoteAtomPtr r;
		CPSMap::iterator psitr;
		{
			CAutoLock l( m_csPSMap );
			psitr = m_PSMap.find( pRequestID );
			if( psitr == m_PSMap.end() )
				return;
			pSubsMap = &psitr->second;

			if(pSubsMap!=NULL && (pSubsMap->size() > 1L || pSubsMap->begin()->first.length()))
			{
				for (;it!=itend;it++) 
				{
					ResponseBlock& rb = *it;
					if( rb.IsValidResponse() ) 
					{
						CSMap::iterator itrRequest = pSubsMap->find(rb.m_requestKey.m_symbol);
						if(itrRequest != pSubsMap->end())
						{	
							_RealtimeQuoteAtomPtr spData = itrRequest->second;

							if ( rb.m_status == ResponseBlock::STATUS_SOURCE_NOT_FOUND ) 
							{
								pApp->OnBIError( enSymbolNotSupported, ResponseBlock::StatusToString( rb.m_status ).c_str(), enSubscribeQuote, spData->m_spRequest );
								pSubsMap->erase(itrRequest);
								continue;
							}

							ParseResponseBlock( rb, spData->m_spResponse.get(), spData->m_spRequest->Type == enOPT ? true : false, spData->m_spRequest.get());
							spData->m_bUpdate = true;
							spData->m_Cookie = Params.m_subscriptionCookie;
							m_SMap[ rb.m_responseKey.m_symbol ]= spData;
							pApp->OnSubscribed( spData->m_spRequest );
							++m_UpdateCount;
							pSubsMap->erase(itrRequest);
						}
						else
							ATLASSERT(FALSE);
					}
				}
				if(bIsComplete)
				{
					// Notify Client;
					CSMap::iterator itrReq = pSubsMap->begin();
					while(itrReq != pSubsMap->end())
					{
						m_SMap[ itrReq->first ]= itrReq->second;
						itrReq->second->m_Cookie = Params.m_subscriptionCookie;
						itrReq->second->m_bUpdate = true;
						pApp->OnSubscribed( itrReq->second->m_spRequest );
						++m_UpdateCount;
						++itrReq;
					}

					{
						//CAutoLock lock(m_csPSMap);
						m_PSMap.erase( pRequestID);
					}

				}
				return;
			}
		}
		/*else*/
		{
			r = pSubsMap->begin()->second;
			// The RequestId of this response was not found in 'request last updates' map ( m_LU )
			// and it's a response from subscribe attempt.
			// Because of multiple equal request there is possibility of subscription for several symbols,
			// so here, we'll unsubscribe from all except best 
			std::deque<ResponseBlock*> rbq;

			// Let's walk thru whole response block list
			for (;it!=itend;it++)
			{
				const ResponseBlock& rb = *it;

				// if this not a valid response - skip it
				if( rb.IsValidResponse() ) {

					// if symbol in response block was not found - skip it
					if ( rb.m_status == ResponseBlock::STATUS_SOURCE_NOT_FOUND ) 
						continue;

					// we've found a symbol for subscription
					NotFound = false;
				}
				else
					continue;

				// a subscribed symbol found - add it to deque
				ResponseBlock* ptr = &(*it);

				// if it '<anything>.CM*' symbol, add it to the front of deque
				if ( rb.m_responseKey.m_symbol.find(".CM*") != std::string::npos ) 
					rbq.push_front(  ptr );
				else
					rbq.push_back(  ptr );
			}
			// OK, now all subscribed symbols in deque, with best one is on the front

			// signal to client if no symbols were subscribed
			if ( NotFound ) {
				// err, clear map  & exit
				//std::string es("Unable to subscribe to " );
				//es += r->m_spRequest->Symbol;
				if ( bIsValid ) {
					const ResponseBlock& rb = *rbl.begin();
					pApp->OnBIError( enSymbolNotSupported, ResponseBlock::StatusToString( rb.m_status ).c_str(), enSubscribeQuote, r->m_spRequest );
				}else
					pApp->OnBIError( enSymbolNotSupported, StatusCodeToString( response.GetStatusCode() ).c_str(), enSubscribeQuote, r->m_spRequest );

				{
					CAutoLock lock(m_csPSMap);
					m_PSMap.erase( pRequestID);
				}
				return;
			}

			// we have a winner, get it 
			ResponseBlock *bestblock = rbq[0];	
			rbq.pop_front();	// pop best block for subscription from deque ( it's still in the ResponseBlockList 
			// and pointed by bestblock )

			// the whole deque now - unneeded symbols
			// so, let's try to unsubscribe from all unneeded symbols
			SymbolIdList	sl;
			std::deque<ResponseBlock*>::iterator qit = rbq.begin(), qitend = rbq.end();
			for (;qit!=qitend;qit++ ) {
				ResponseBlock* rblk = *qit;
				sl.push_back( rblk->m_responseKey );
			}

			// if there is something to unsubscribe - do it
			if( sl.size() ) {
				// unsubscribe from all unneeded by one call
				if( Unsubscribe( Params.m_subscriptionCookie, sl ) != STATUS_CODE_SUCCESS ) {
					// signal error to client
					std::string es("Unable to unsubscribe from some future symbol." );
					es += ". Updates will continue to come in.";
					pApp->OnBIError( enInternalError, es.c_str(), enNoRequest, r->m_spRequest );
				}
			}

			// symbol may be already subscribed 
			CSMap::iterator aitr = m_SMap.find( bestblock->m_responseKey.m_symbol );
			if( aitr != m_SMap.end() )
			{
				_RealtimeQuoteAtomPtr artu = aitr->second;
				std::string es("Symbol [" );
				es += bestblock->m_responseKey.m_symbol;
				es += "] already subscribed for [";
				es += artu->m_spRequest->Symbol + '.' + artu->m_spRequest->Exchange;
				es += "]. Subscription for [";
				es += r->m_spRequest->Symbol + '.' + r->m_spRequest->Exchange;
				es += "] abolished";
				pApp->OnBIError( enSymbolNotSupported, es.c_str(), enSubscribeQuote, r->m_spRequest );
				if( Unsubscribe(Params.m_subscriptionCookie) == STATUS_CODE_SUCCESS )
				{
					CAutoLock lock(m_csPSMap);
					m_PSMap.erase( pRequestID);
				}
			}
			else
			{
				// proceed with subscription routine - move pending subscription into subscribed map
				CAutoLock m_Lock( r->m_Lock );
				ParseResponseBlock(*bestblock, r->m_spResponse.get(), false, r->m_spRequest.get());
				if( !r->m_bUpdate )
				{
					InterlockedIncrement(&m_UpdateCount);
					r->m_bUpdate = true;
				}
				r->m_Cookie = Params.m_subscriptionCookie;
				{
					CAutoLock lc( m_csSMap );
					CAutoLock _lc( m_csPSMap );
					m_SMap[ bestblock->m_responseKey.m_symbol ]= r;
					{
						CAutoLock lock(m_csPSMap);
						m_PSMap.erase( pRequestID);
					}
				}
				pApp->OnSubscribed( r->m_spRequest );
			}
		}
		return;
	}


	// process request last
	for (;it!=itend;it++) {
		const ResponseBlock& rb = *it;
		if ( rb.m_status != ResponseBlock::STATUS_SUCCESS /*&& rb.m_status != ResponseBlock::STATUS_STALE*/ ) 
			continue;
		if( rb.IsValidResponse() && rb.IsResponseKeyDefined() && rb.m_responseKey.m_symbol.find(".CM*") != std::string::npos )
			Magic = true;

		if( STATUS_CODE_SUCCESS == m_FieldListValidator.Initialize( rb.m_fieldData ) )	{
			NotFound = false;
			FieldListValidator::ConstIterator fit = m_FieldListValidator.Begin(), fitEnd = m_FieldListValidator.End();
			for ( ; fit != fitEnd; fit++ ) {
				const FieldListValidator::Field& fld = fit.GetField();
				if ( FIELD_STATUS_DEFINED == fld.m_fieldStatus ) 
				{
					switch( fld.m_fieldId) 
					{
					case Feed::FID_OPEN_INTEREST:
						{
							Activ::UInt bidsize;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&bidsize);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->OpenInterest = static_cast<long>( bidsize.Get());
						}
						break;
					case Feed::FID_CURRENCY:
						{
							Activ::TextArray pText;
							IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->Currency = pText.ToString();
						}
						break;
					case Feed::FID_BID:
						{
							Activ::TRational bid;
							IFieldType* pUpdateField = static_cast<Activ::TRational*>(&bid);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->BidPrice = bid.Get();
							Activ::TRational::Tick t = bid.GetTick();
							switch( t )
							{
							case Activ::TRational::TICK_DOWN:
								p->m_spResponse->PriceTick |= enBidDnTick;
								break;
							case Activ::TRational::TICK_UP:
								p->m_spResponse->PriceTick |= enBidUpTick;
								break;
							default:;
							}
						}
						break;
					case Feed::FID_BID_SIZE:
						{
							Activ::UInt bidsize;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&bidsize);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->BidSize = static_cast<long>(bidsize.Get());
						}
						break;
					case Feed::FID_BID_EXCHANGE:
						{
							Activ::TextString pText;
							IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->BidExchange = pText.ToString();
						}
						break;
					case Feed::FID_ASK:
						{
							Activ::TRational ask;
							IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->AskPrice = ask.Get();
							Activ::TRational::Tick t = ask.GetTick();
							switch( t )
							{
							case Activ::TRational::TICK_DOWN:
								p->m_spResponse->PriceTick |= enAskDnTick;
								break;
							case Activ::TRational::TICK_UP:
								p->m_spResponse->PriceTick |= enAskUpTick;
								break;
							default:;
							}
						}
						break;
					case Feed::FID_ASK_SIZE:
						{
							Activ::UInt asksize;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&asksize);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->AskSize = static_cast<long>(asksize.Get());
						}
						break;
					case Feed::FID_ASK_EXCHANGE:
						{
							Activ::TextString pText;
							IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->AskExchange = pText.ToString();
						}
						break;
					case Feed::FID_TRADE_EXCHANGE:
						{
							Activ::TextString pText;
							IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->Exchange = pText.ToString();
						}
						break;
					case Feed::FID_OPEN:
						{
							Activ::Rational opn;
							IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->OpenPrice = opn.Get();
						}
						break;
					case Feed::FID_LAST_UPDATE_DATE:
						{
							Activ::Date dt;
							EgLib::vt_date	pdate( p->m_spResponse->UpdateDateTime );
							IFieldType* pUpdateField = static_cast<Activ::Date*>(&dt);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							int year = dt.GetYear();
							int month = dt.GetMonth();
							int day = dt.GetDay();
							EgLib::vt_date d( year, month, day, 0/*pdate.get_hour()*/, 0/*pdate.get_minute()*/, 0/*pdate.get_second()*/ );
							p->m_spResponse->UpdateDateTime = d;
						}
						break;
					case Feed::FID_LAST_UPDATE_TIME:
						{
							Activ::Time dt;
							EgLib::vt_date	pdate( p->m_spResponse->UpdateDateTime );
							IFieldType* pUpdateField = static_cast<Activ::Time*>(&dt);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							int hour = dt.GetHour();
							int minute = dt.GetMinute();
							int second = dt.GetSecond();
							EgLib::vt_date d( pdate.get_year(), pdate.get_month(), pdate.get_day(),hour, minute, second );
							p->m_spResponse->UpdateDateTime = d;
						}
						break;
					case Feed::FID_CUMULATIVE_VOLUME:
						{
							Activ::UInt volume;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&volume);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->Volume = static_cast<long>(volume.Get());
						}
						break;
					case Feed::FID_SETTLEMENT:
					case Feed::FID_CLOSE:
						{
							Activ::Rational opn;
							IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->ClosePrice = opn.Get();

							if( (  p->m_spResponse->LastPrice == BAD_DOUBLE_VALUE || ! p->m_spResponse->LastPrice) /*&& pRequest && pRequest->Type != enOPT/*&& !bRealTime*/ ) 
								p->m_spResponse->LastPrice =  p->m_spResponse->ClosePrice;
						}
						break;
					case Feed::FID_CONTRACT_SIZE:
					case Feed::FID_SHARES_PER_CONTRACT:
						{
							Activ::UInt asksize;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&asksize);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->LotSize = static_cast<long>(asksize.Get());
						}
						break;
					case Feed::FID_TRADE:
						{
							Activ::TRational ask;
							IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->LastPrice = ask.Get();
							Activ::TRational::Tick t = ask.GetTick();
							switch( t )
							{
							case Activ::TRational::TICK_DOWN:
								p->m_spResponse->PriceTick |= enLastSaleDnTick;
								break;
							case Activ::TRational::TICK_UP:
								p->m_spResponse->PriceTick |= enLastSaleUpTick;
								break;
							default:;
							}
						}
						break;
					case Feed::FID_TRADE_HIGH:
						{
							Activ::Rational opn;
							IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->HighPrice = opn.Get();
						}
						break;
					case Feed::FID_TRADE_LOW:
						{
							Activ::Rational opn;
							IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->LowPrice = opn.Get();
						}
						break;
					case Feed::FID_NET_CHANGE:
						{
							Activ::TRational ask;
							IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->m_spResponse->NetChange = ask.Get();
						}
						break;
					default:
						p->m_spResponse->Status = enQuoteStatus_Unreliable;
					}
				}
			}
		}

		if ( Magic ) 
			break;
	}

	if (NotFound) {
		if ( IsValidResponse( response ) ) {
			const ResponseBlock& rb = *rbl.begin();
			pApp->OnBIError( enSymbolNotSupported, ResponseBlock::StatusToString( rb.m_status ).c_str(), enRequestLastQuote, p->m_spRequest );
		}
		else
			pApp->OnBIError( enSymbolNotSupported, StatusCodeToString( response.GetStatusCode() ).c_str(), enRequestLastQuote, p->m_spRequest );
	}
	else
		pApp->OnRequstLast( p->m_spRequest, p->m_spResponse );	
		{
			CAutoLock lock(m_csLU);
			m_LU.erase( pRequestID );
		}
}


void 
CActivFeedContentGateway::OnGetMultiplePatternMatchListResponse(HeapMessage &response)
{
	if (!IsValidResponse(response)){
		int err = response.GetStatusCode();
		CActivFeedAppication* pApplication = dynamic_cast<CActivFeedAppication*>(&GetApplication());
		if(pApplication){
			if(STATUS_CODE_NOT_FOUND == err){
				RequestIdPtr pRequestID(new RequestId(response.GetRequestId()));
				CRequestBasePtr pRequest = GetRequest(pRequestID);
				if(pRequest)
				{
					switch(pRequest->m_enType)
					{
					case CRequestBase::_enRequestOption:
						{
							CRequest<OptionParams>::CRequestPtr spOptRequest = boost::shared_dynamic_cast<CRequest<OptionParams> >(pRequest);
							CComVariant vtRequest;
							spOptRequest->m_recParams.CopyTo(vtRequest);
							pApplication->OnSPError(enNoDataAvailableForSymbol, "No data received", enRequestOptions, vtRequest);
						}
						break;

					case CRequestBase::_enRequestFuturesOption:
						{
							CRequest<FuturesOptionParams>::CRequestPtr spOptRequest = boost::shared_dynamic_cast<CRequest<FuturesOptionParams> >(pRequest);
							CComVariant vtRequest;
							spOptRequest->m_recParams.CopyTo(vtRequest);
							pApplication->OnSPError(enNoDataAvailableForSymbol, "No data received", enRequestFuturesOptions, vtRequest);
						}
						break;
					}
				}

				DeleteRequest(pRequestID);

			}
			else{
				CComVariant vtRequest;
				vtRequest.Clear();
				pApplication->OnSPError(enNoDataAvailableForSymbol, "No data received", enRequestStock, vtRequest);
			}
		}
		return;
	}

	RequestIdPtr pRequestID(new RequestId(response.GetRequestId()));

	CRequestBasePtr pRequest = GetRequest(pRequestID);
	if(pRequest)
	{
		bool IsLast = false;
		if(IsCompleteResponse(response))
			IsLast = true;

		switch(pRequest->m_enType)
		{
		case CRequestBase::_enRequestOption:
			{
				CRequest<OptionParams>::CRequestPtr spOptRequest = boost::shared_dynamic_cast<CRequest<OptionParams> >(pRequest);
				OnOption(spOptRequest, response, IsLast);
			}
			break;
		case CRequestBase::_enRequestFuturesOption:
			{
				CRequest<FuturesOptionParams>::CRequestPtr spOptRequest = boost::shared_dynamic_cast<CRequest<FuturesOptionParams> >(pRequest);
				OnFuturesOption(spOptRequest, response, IsLast);
			}
			break;
		}

		if(IsLast)
			DeleteRequest(pRequestID);
	}
}

void CActivFeedContentGateway::OnGetMultiplePatternMatchResponse(HeapMessage &response)
{
	bool bHack = false;

	if (!IsValidResponse(response)){
		int err = response.GetStatusCode();
		CActivFeedAppication* pApplication = dynamic_cast<CActivFeedAppication*>(&GetApplication());
		if(pApplication){
			if(STATUS_CODE_NOT_FOUND == err){
				RequestIdPtr pRequestID(new RequestId(response.GetRequestId()));
				CRequestBasePtr pRequest = GetRequest(pRequestID);
				if(pRequest)
				{
					switch(pRequest->m_enType)
					{
					case CRequestBase::_enRequestStock:
						{
							CRequest<StockParams>::CRequestPtr spOptRequest = boost::shared_dynamic_cast<CRequest<StockParams> >(pRequest);
							CComVariant vtRequest;
							spOptRequest->m_recParams.CopyTo(vtRequest);
							pApplication->OnSPError(enNoDataAvailableForSymbol, "No data received", enRequestStock, vtRequest);
						}
						break;
					case CRequestBase::_enRequestFutures:
						{
							CRequest<FutureRootParams>::CRequestPtr spOptRequest = boost::shared_dynamic_cast<CRequest<FutureRootParams> >(pRequest);
							CComVariant vtRequest;
							spOptRequest->m_recParams.CopyTo(vtRequest);
							pApplication->OnSPError(enNoDataAvailableForSymbol, "No data received", enRequestFutures, vtRequest);
						}
						break;
					case CRequestBase::_enRequestFuture:
						{
							CRequest<FutureParams>::CRequestPtr spOptRequest = boost::shared_dynamic_cast<CRequest<FutureParams> >(pRequest);
							CComVariant vtRequest;
							spOptRequest->m_recParams.CopyTo(vtRequest);
							pApplication->OnSPError(enNoDataAvailableForSymbol, "No data received", enRequestFuture, vtRequest);
						}
						break;
					case CRequestBase::_enRequestFuturesOption:
						{
							CRequest<FuturesOptionParams>::CRequestPtr spOptRequest = boost::shared_dynamic_cast<CRequest<FuturesOptionParams> >(pRequest);
							CComVariant vtRequest;
							spOptRequest->m_recParams.CopyTo(vtRequest);
							pApplication->OnSPError(enNoDataAvailableForSymbol, "No data received", enRequestFuturesOptions, vtRequest);
						}
						break;
					case CRequestBase::_enRequestOption:
						{
							CRequest<OptionParams>::CRequestPtr spOptRequest = boost::shared_dynamic_cast<CRequest<OptionParams> >(pRequest);
							CComVariant vtRequest;
							spOptRequest->m_recParams.CopyTo(vtRequest);
							pApplication->OnSPError(enNoDataAvailableForSymbol, "No data received", enRequestOptions, vtRequest);
						}
						break;
					}

					DeleteRequest(pRequestID);
				}
				else
					bHack = true;
			}
			else{
				CComVariant vtRequest;
				vtRequest.Clear();
				pApplication->OnSPError(enNoDataAvailableForSymbol, "No data received", enNoRequest, vtRequest);
			}
		}
	}

	RequestIdPtr pRequestID(new RequestId(response.GetRequestId()));

	CRequestBasePtr pRequest = GetRequest(pRequestID);
	if(pRequest && !bHack )
	{
		bool IsLast = false;
		if(IsCompleteResponse(response))
			IsLast = true;

		switch(pRequest->m_enType)
		{
		case CRequestBase::_enRequestStock:
			{
				CRequest<StockParams>::CRequestPtr spOptRequest = boost::shared_dynamic_cast<CRequest<StockParams> >(pRequest);
				OnIndex(spOptRequest, response);
				if(IsLast)
					DeleteRequest(pRequestID);		
			}
			break;
		case CRequestBase::_enRequestFutures:
			{
				CRequest<FutureRootParams>::CRequestPtr spOptRequest = boost::shared_dynamic_cast<CRequest<FutureRootParams> >(pRequest);
				OnFutureByRoot(spOptRequest, response);
				if(IsLast)
				{
					CActivFeedAppication* pApplication = dynamic_cast<CActivFeedAppication*>(&GetApplication());
					if(pApplication)
					{

						// Here we going to answer to client. We'll send one additional response
						// due bug in EtsBO - it doesn't recognize last answer
						if ( !m_futMap.empty() ) {
							size_t i = 1, sz = m_futMap.size();

							CFutMap::iterator itr = m_futMap.begin(), itrend = m_futMap.end();
							CString monthYear;

							for (; itr != itrend; itr++, i++ ) {
								//spResult->m_respParams->Description = ::SysAllocString(itr->second.Description);//_bstr_t(" ").copy();
								CResponse<FutureResultsEx>::CResponsePtr spResult = itr->second;


								_bstr_t extDescription(spResult->m_respParams->Description);
								extDescription += L" "; 
								extDescription += _bstr_t( FutureExpiration(itr->first).c_str() );

								if(spResult->m_respParams->Description!=NULL)
									SysFreeString(spResult->m_respParams->Description);

								spResult->m_respParams->Description = extDescription.copy();

								if(i == sz)
								{
									m_futMap.clear();
									pApplication->OnFutureByRoot(spOptRequest, spResult, true );
									break;
								}  
								else
								{
									pApplication->OnFutureByRoot(spOptRequest, spResult, false );
								}
							}
						}
						else
						{
							CComVariant vtRequest;
							spOptRequest->m_recParams.CopyTo(vtRequest);
							pApplication->OnSPError(enNoDataAvailableForSymbol, "No data received", enRequestFutures, vtRequest);
						}  

					}
					else
						m_futMap.clear();
					DeleteRequest(pRequestID);		
				}
			}
			break;
		case CRequestBase::_enRequestFuture:
			{
				CRequest<FutureParams>::CRequestPtr spOptRequest = boost::shared_dynamic_cast<CRequest<FutureParams> >(pRequest);
				OnFuture(spOptRequest, response);
				if(IsLast)
					DeleteRequest(pRequestID);		
			}
			break;
		case CRequestBase::_enRequestFuturesOption:
			{
				SaveFutureOptionRoots(response);
				if(IsLast)
					RequestFutureOptionsByRoot(pRequestID);
			}
			break;
		case CRequestBase::_enRequestOption:
			{
				SaveIndexOptionRoots(response);
				//if(IsLast)
				RequestOptionsByRoot(pRequestID);
			}
			break;
		}
		/*if(IsLast)
		DeleteRequest(pRequestID);*/
	}
	else
	{
		bool bLU = false;
		{
			CLUMap::iterator itr, itrend;
			_QuoteAtomPtr p;
			{
				CAutoLock lock(m_csLU);
				itr = m_LU.find( pRequestID );
				itrend = m_LU.end();
				if( itr != itrend ) {
					bLU = true;
					p = itr->second.begin()->second;
				}
			}

			if( p )
			{
				CActivFeedAppication* pApp = dynamic_cast<CActivFeedAppication*>(&GetApplication());
				if( !IsValidResponse(response) ){
					pApp->OnBIError( enSymbolNotSupported, StatusCodeToString( response.GetStatusCode() ).c_str(), enRequestLastQuote, p->m_spRequest );
					{
						CAutoLock lock(m_csLU);
						m_LU.erase( pRequestID );
					}
					return;
				}
				/*				if( response.GetStatusCode() != STATUS_CODE_SUCCESS &&  response.GetStatusCode() != STATUS_CODE_PENDING ) {
				pApp->OnBIError( enSymbolNotSupported, StatusCodeToString( response.GetStatusCode() ).c_str(), enRequestLastQuote, p->m_spRequest );
				CAutoLock lock(m_csLU);
				m_LU.erase( m_LU.find( pRequestID) );
				return;
				}
				*/			
				GetMultiplePatternMatch::ResponseParameters Params;
				StatusCode statusCode = GetMultiplePatternMatch::Deserialize(*this, response, Params);
				bool bGotOne = false;
				if( STATUS_CODE_SUCCESS == statusCode ) {
					ResponseBlockList rbl = Params.m_responseBlockList;
					ResponseBlockList::const_iterator it = rbl.begin(), itend = rbl.end();
					if ( it != itend ) {
						for( it ; it != itend; it++ ) {
							if ( it->m_status == ResponseBlock::STATUS_SUCCESS /*|| it->m_status == ResponseBlock::STATUS_STALE*/ ) {
								std::string::size_type dotPos = (*it).m_responseKey.m_symbol.find('.');
								if(p->m_spRequest->Type != enIDX || (dotPos!= std::string::npos && dotPos == (*it).m_responseKey.m_symbol.find_last_of('.')))
								{
									ApplyMPResponseToQUI( static_cast<ResponseBlock>(*it), p->m_spResponse.get(), false, p->m_spRequest.get() );
									bGotOne = true;
									if ( (*it).m_responseKey.m_symbol.find(".CM") == std::string::npos ) 
										break;
								}
							}
						}
						if ( bGotOne ) 
							pApp->OnRequstLast( p->m_spRequest, p->m_spResponse );
						else{
							const ResponseBlock& rb = *rbl.begin();
							pApp->OnBIError( enSymbolNotSupported, ResponseBlock::StatusToString( rb.m_status ).c_str(), enRequestLastQuote, p->m_spRequest );
						}
					}
					else{
						EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , "Empty response for %s[%d]",  p->m_spRequest->Symbol.c_str(), p->m_spRequest->Type );
						pApp->OnBIError( enSymbolNotSupported, "Empty response", enRequestLastQuote, p->m_spRequest );
					}
				}
				else
					pApp->OnBIError( enSymbolNotSupported, StatusCodeToString( response.GetStatusCode() ).c_str(), enRequestLastQuote, p->m_spRequest );
				CAutoLock lock(m_csLU);
				m_LU.erase( pRequestID );
				return;
			}
		}
		{
			//bool Notify = false;
			//CActivFeedAppication* pApp = NULL;
			_RealtimeQuoteAtomPtr  r;
			if ( !bLU )
			{
				CPSMap::iterator itr;
				{
					CAutoLock lock( m_csPSMap );
					itr = m_PSMap.find( pRequestID );
					if( itr != m_PSMap.end() )
						r = itr->second.begin()->second;
				}
				if( r ) {
					CActivFeedAppication* pApp = dynamic_cast<CActivFeedAppication*>(&GetApplication());
					if( !IsValidResponse(response) ){
						int safetc = r->m_spRequest->TryCount;
						pApp->OnBIError( enSymbolNotSupported, StatusCodeToString( response.GetStatusCode() ).c_str(), enSubscribeQuote, r->m_spRequest );
						if( r->m_spRequest->TryCount < safetc ) {
							CAutoLock lck1( m_csPSMap );
							m_PSMap.erase( pRequestID );
						}
						return;
					}
					/*					{
					CAutoLock l( r->m_Lock );
					pApp = ApplyMPResponseToQUI( response, r->m_spResponse.get(), true );
					if( !r->m_bUpdate ) {
					r->m_bUpdate = true;
					}
					}
					if ( response.GetStatusCode() != STATUS_CODE_SUCCESS && response.GetStatusCode() != STATUS_CODE_PENDING ) {
					CAutoLock lck1( m_csPSMap );
					int safetc = r->m_spRequest->TryCount;
					pApp->OnBIError( enSymbolNotSupported, StatusCodeToString( response.GetStatusCode() ).c_str(), enSubscribeQuote, r->m_spRequest );
					if( r->m_spRequest->TryCount < safetc ) {
					CAutoLock lck1( m_csPSMap );
					m_PSMap.erase( m_PSMap.find( pRequestID ) );
					}
					return;
					}	
					*/
					GetMultiplePatternMatch::ResponseParameters Params;
					StatusCode statusCode = GetMultiplePatternMatch::Deserialize(*this, response, Params);
					//
					bool bGotOne = false;
					if( STATUS_CODE_SUCCESS == statusCode ) {
						ResponseBlockList rbl = Params.m_responseBlockList;
						ResponseBlockList::const_iterator it = rbl.begin(), itend = rbl.end();
						if ( it != itend ) {

							std::string fndRespSymb; 

							for( it ; it != itend; it++ ) {
								if ( it->m_status == ResponseBlock::STATUS_SUCCESS /*|| it->m_status == ResponseBlock::STATUS_STALE*/ ) {

									std::string::size_type dotPos = it->m_responseKey.m_symbol.find('.');
									if(r->m_spRequest->Type != enIDX || (dotPos!= std::string::npos && dotPos == it->m_responseKey.m_symbol.find_last_of('.')))
									{
										CAutoLock l( r->m_Lock ); 
										ApplyMPResponseToQUI( static_cast<ResponseBlock>(*it), r->m_spResponse.get(), false, r->m_spRequest.get() ); if( !r->m_bUpdate ) r->m_bUpdate = true; 
										bGotOne = true;
										fndRespSymb = it->m_responseKey.m_symbol;
										if ( it->m_responseKey.m_symbol.find(".CM") == std::string::npos ) 
											break;
									}
								}
							}
							if ( bGotOne ) {
								CAutoLock lc( m_csSMap );						
								CAutoLock lck1( m_csPSMap );
								CAutoLock lck2( r->m_Lock );
								r->m_Cookie = Params.m_subscriptionCookie;		
								m_SMap[ fndRespSymb ]= r;			
								m_PSMap.erase(  pRequestID  );							
								if( r->m_bUpdate )	InterlockedIncrement(&m_UpdateCount);
								pApp->OnSubscribed( r->m_spRequest );
							}
							else{
								int safetc = r->m_spRequest->TryCount;
								const ResponseBlock& rb = *rbl.begin();
								pApp->OnBIError( enSymbolNotSupported, ResponseBlock::StatusToString(rb.m_status).c_str(), enSubscribeQuote, r->m_spRequest );
								if( r->m_spRequest->TryCount < safetc) {
									CAutoLock lck1( m_csPSMap );
									m_PSMap.erase( pRequestID  );
								}
							}
						}
						else{
							EgLib::CEgLibTraceManager::Trace(LogError, __FUNCTION__ , "Empty response for %s[%d]",  r->m_spRequest->Symbol.c_str(), r->m_spRequest->Type );
							pApp->OnBIError( enSymbolNotSupported, "Empty response", enRequestLastQuote, r->m_spRequest );
						}
					}
					else{
						int safetc = r->m_spRequest->TryCount;
						pApp->OnBIError( enSymbolNotSupported, StatusCodeToString( statusCode ).c_str(), enSubscribeQuote, r->m_spRequest );
						if( r->m_spRequest->TryCount < safetc) {
							CAutoLock lck1( m_csPSMap );
							m_PSMap.erase( pRequestID  );
						}
					}
					//
					/*if( STATUS_CODE_SUCCESS == statusCode )
					{
					ResponseBlockList rbl = Params.m_responseBlockList;
					ResponseBlockList::const_iterator it, itend = rbl.end();
					for( it = rbl.begin(); it != itend; it++ )
					{
					const ResponseBlock& rb = *it;
					if( rb.IsValidResponse() && rb.IsResponseKeyDefined() )
					{
					CAutoLock lc( m_csSMap );						
					CAutoLock lck1( m_csPSMap );
					CAutoLock lck2( r->m_Lock );
					r->m_Cookie = Params.m_subscriptionCookie;		
					m_SMap[ rb.m_responseKey.m_symbol ]= r;			
					m_PSMap.erase( m_PSMap.find( pRequestID ) );							
					InterlockedIncrement(&m_UpdateCount);
					Notify = true;										
					break;											
					}
					else {
					InterlockedDecrement(&m_UpdateCount);
					int safetc = r->m_spRequest->TryCount;
					pApp->OnBIError( enSymbolNotSupported, ResponseBlock::StatusToString(rb.m_status).c_str(), enSubscribeQuote, r->m_spRequest );
					if( r->m_spRequest->TryCount < safetc) {
					CAutoLock lck1( m_csPSMap );
					m_PSMap.erase( m_PSMap.find( pRequestID ) );
					}
					break;
					}
					}
					}
					else {
					InterlockedDecrement(&m_UpdateCount);
					int safetc = r->m_spRequest->TryCount;
					pApp->OnBIError( enSymbolNotSupported, StatusCodeToString( statusCode ).c_str(), enSubscribeQuote, r->m_spRequest );
					if( r->m_spRequest->TryCount < safetc) {
					CAutoLock lck1( m_csPSMap );
					m_PSMap.erase( m_PSMap.find( pRequestID ) );
					}
					}*/
				}
			}
			//			if( pApp && Notify )
			//				pApp->OnSubscribed( r->m_spRequest );
		}
	}
}


StatusCode CActivFeedContentGateway::RequestStock(CRequest<StockParams>::CRequestPtr& spStockRequest)
{
	//The GetEqual helper class gives us the correct
	//request and response parameter types.
	RequestIdPtr spRequestID(new RequestId);

	GetEqual::RequestParameters requestParameters;
	//Set up the request block for our canonical request.
	RequestBlock requestBlock1;
	//	requestBlock1.m_flags |= RequestBlock::FLAG_ALL_FIELDS;

	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_COMPANY;
	requestBlock1.m_fieldIdList.push_back(Feed::FID_NAME);
	requestParameters.m_requestBlockList.push_back(requestBlock1);

	RequestBlock requestBlock2;
	requestBlock2.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock2.m_fieldIdList.push_back(Feed::FID_CLOSE);
	requestBlock2.m_fieldIdList.push_back(Feed::FID_PREVIOUS_ASK);
	requestBlock2.m_fieldIdList.push_back(Feed::FID_PREVIOUS_BID);


	requestBlock2.m_fieldIdList.push_back(Feed::FID_BID);
	requestBlock2.m_fieldIdList.push_back(Feed::FID_ASK);
	requestBlock2.m_fieldIdList.push_back(Feed::FID_OPEN);
	requestBlock2.m_fieldIdList.push_back(Feed::FID_CUMULATIVE_VOLUME);
	requestBlock2.m_fieldIdList.push_back(Feed::FID_TRADE_HIGH);
	requestBlock2.m_fieldIdList.push_back(Feed::FID_TRADE_LOW);
	requestBlock2.m_fieldIdList.push_back(Feed::FID_TRADE_SIZE);

	//requestBlock2.m_fieldIdList.push_back(Feed::FID_SHARES_PER_CONTRACT);
	requestParameters.m_requestBlockList.push_back(requestBlock2);

	RequestBlock requestBlock3;
	requestBlock3.m_relationshipId = Feed::RELATIONSHIP_ID_SECURITY;
	requestBlock3.m_flags |= RequestBlock::FLAG_ALL_FIELDS;
	requestBlock3.m_fieldIdList.push_back(Feed::FID_PRIMARY_EXCHANGE);
	//	requestBlock3.m_fieldIdList.push_back(Feed::FID_DIVIDEND ); 
	//	requestBlock3.m_fieldIdList.push_back(Feed::FID_EX_DATE ); 
	requestBlock3.m_fieldIdList.push_back(Feed::FID_DIVIDEND_FREQUENCY_CODE);
	requestBlock3.m_fieldIdList.push_back(Feed::FID_NEXT_DIVIDEND_1);
	requestBlock3.m_fieldIdList.push_back(Feed::FID_NEXT_DIVIDEND_1_DATE );
	requestBlock3.m_fieldIdList.push_back(Feed::FID_NEXT_DIVIDEND_1_STATUS );
	requestParameters.m_requestBlockList.push_back(requestBlock3);

	//Asynchronous requests need a request Id.

	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	//We do not know the table number, so set it to undefined.
	_bstr_t bsStock(L"");
	_bstr_t bsExchange(L"");
	if(spStockRequest->m_recParams->Stock)
		bsStock = spStockRequest->m_recParams->Stock;
	if(spStockRequest->m_recParams->Exchange)
		bsExchange = spStockRequest->m_recParams->Exchange;

	std::string sKey((LPCSTR)bsStock);
	sKey +=".";

	std::string sRegion = "USA";

	if(bsExchange.length())
	{
		std::string strEgar((LPCSTR)bsExchange);
		ActivFeedSettings::CExchangeKey exhKey = ActivFeedSettings::CExchangeKey(strEgar, ActivFeedSettings::CExchangeKey::enExchangeStock);

		ActivFeedSettings::CEgarExchangeKey EgarKey(exhKey);

		ActivFeedSettings::CActivExchangeKey ActivKey = ActivFeedSettings::g_spUserSettings->Settings->ActivExchange[EgarKey];
		if(ActivKey.length())
			sKey += ActivKey;
		sRegion = ActivKey.GetExchangeRegion();
	}

	//requestParameters.m_symbolId = SymbolId(Feed::TABLE_NO_UNDEFINED, sKey);
	requestParameters.m_symbolId = SymbolId(sRegion == "USA" ? Feed::TABLE_NO_US_LISTING : Feed::TABLE_NO_AMERICAS_LISTING, sKey);

	//Launch the request.
	StatusCode statusCode = GetEqual::PostRequest(*this, *spRequestID,	requestParameters);
	//If the request was launched successfully, the statusCode is
	//STATUS_CODE_SUCCESS.
	if(STATUS_CODE_SUCCESS == statusCode)
	{
		CAutoLock lock(m_csContainer);
		m_Container[spRequestID] = spStockRequest;
	}
	return statusCode;
}


StatusCode CActivFeedContentGateway::RequestFuture(CRequest<FutureParams>::CRequestPtr& spStockRequest)
{
	_bstr_t bsStock(L"");
	if(spStockRequest->m_recParams->FutureSymbol)
		bsStock = spStockRequest->m_recParams->FutureSymbol;

	GetMultiplePatternMatch::RequestParameters getMultiplePatternMatchListRequestParameters;
	getMultiplePatternMatchListRequestParameters.m_tableNumber = TABLE_NO_US_FUTURE;

	getMultiplePatternMatchListRequestParameters.m_symbolPattern = bsStock + ".*";
	RequestBlock requestBlock1;
	//	requestBlock1.m_flags |= RequestBlock::FLAG_ALL_FIELDS;

	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back(Feed::FID_SYMBOL);
	requestBlock1.m_fieldIdList.push_back(Feed::FID_LAST_TRADING_DATE );
	requestBlock1.m_fieldIdList.push_back(Feed::FID_CLOSE);
	requestBlock1.m_fieldIdList.push_back(Feed::FID_BID);
	requestBlock1.m_fieldIdList.push_back(Feed::FID_ASK);
	requestBlock1.m_fieldIdList.push_back(Feed::FID_TRADE);
	requestBlock1.m_fieldIdList.push_back(Feed::FID_EXPIRATION_DATE);

	getMultiplePatternMatchListRequestParameters.m_requestBlockList.push_back(requestBlock1);

	RequestBlock requestBlock2;
	requestBlock2.m_relationshipId = Feed::RELATIONSHIP_ID_FUTURE_ROOT;
	requestBlock2.m_fieldIdList.push_back(Feed::FID_CONTRACT_SIZE);
	requestBlock2.m_fieldIdList.push_back(Feed::FID_MINIMUM_TICK); 
	requestBlock2.m_fieldIdList.push_back(Feed::FID_UNIT_OF_MEASURE);
	requestBlock2.m_fieldIdList.push_back(Feed::FID_UNIT_OF_MEASURE_CURRENCY);
	requestBlock2.m_fieldIdList.push_back(Feed::FID_NAME);
	getMultiplePatternMatchListRequestParameters.m_requestBlockList.push_back(requestBlock2);

	//getMultiplePatternMatchListRequestParameters.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	//getMultiplePatternMatchListRequestParameters.m_permissionLevel = /*(m_isDelayed ? PERMISSION_LEVEL_DELAYED : */PERMISSION_LEVEL_DEFAULT/*)*/;

	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetMultiplePatternMatch::PostRequest(*this, *spRequestID, getMultiplePatternMatchListRequestParameters));

	CAutoLock lock(m_csContainer);
	m_Container[spRequestID] = spStockRequest;

	return STATUS_CODE_SUCCESS;

}

StatusCode CActivFeedContentGateway::RequestOptions(CRequest<OptionParams>::CRequestPtr& spOptionRequest)
{
	StatusCode statusCode = STATUS_CODE_SUCCESS;

	m_optionRoots.clear();
	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));
	//m_loadOptionRootID = *spRequestID.get();
	_bstr_t bsOpt(L"");
	_bstr_t bsExchange(L"");
	if(spOptionRequest->m_recParams->UnderlyingSymbol)
		bsOpt = spOptionRequest->m_recParams->UnderlyingSymbol;
	if(spOptionRequest->m_recParams->Exchange)
		bsExchange = spOptionRequest->m_recParams->Exchange;

	std::string sKey((LPCSTR)bsOpt);
	sKey +=".";

	if(bsExchange.length())
	{
		std::string strEgar((LPCSTR)bsExchange);
		ActivFeedSettings::CExchangeKey exhKey = ActivFeedSettings::CExchangeKey(strEgar, ActivFeedSettings::CExchangeKey::enExchangeOption);

		ActivFeedSettings::CEgarExchangeKey EgarKey(exhKey);

		ActivFeedSettings::CActivExchangeKey ActivKey = ActivFeedSettings::g_spUserSettings->Settings->ActivExchange[EgarKey];
		if(ActivKey.length())
			sKey += ActivKey;
	}

	GetEqual::RequestParameters getEqualRequestParameters;
	GetEqual::ResponseParameters getEqualResponseParameters;

	getEqualRequestParameters.m_symbolId = SymbolId(Feed::TABLE_NO_UNDEFINED, sKey);

	RequestBlock requestBlock;

	requestBlock.m_relationshipId = Feed::RELATIONSHIP_ID_OPTION_ROOT;
	requestBlock.m_fieldIdList.push_back(Feed::FID_SYMBOL);
	requestBlock.m_fieldIdList.push_back(Feed::FID_CONTRACT_MULTIPLIER/*FID_SHARES_PER_CONTRACT*/);
	requestBlock.m_fieldIdList.push_back(Feed::FID_CURRENCY);
	requestBlock.m_fieldIdList.push_back(Feed::FID_SETTLEMENT_TYPE);
	requestBlock.m_fieldIdList.push_back(Feed::FID_EXPIRATION_TYPE);
	 
	getEqualRequestParameters.m_requestBlockList.push_back(requestBlock);

	if (!sKey.empty())
	{
		statusCode = GetEqual::PostRequest(*this, *spRequestID,	getEqualRequestParameters);
	}
	else
	{
		CActivFeedAppication* pApplication = dynamic_cast<CActivFeedAppication*>(&GetApplication());
		if(pApplication){
			CComVariant vtRequest;
			spOptionRequest->m_recParams.CopyTo(vtRequest);
			pApplication->OnSPError(enNoDataAvailableForSymbol, "No underlying", enRequestOptions, vtRequest);
		}
	}

	if(STATUS_CODE_SUCCESS == statusCode)
	{
		CAutoLock lock(m_csContainer);
		m_Container[spRequestID] = spOptionRequest;
	}
	return statusCode;
}

StatusCode CActivFeedContentGateway::RequestFuturesOptions(CRequest<FuturesOptionParams>::CRequestPtr& spStockRequest)
{
	m_optionFutureRoots.clear();

	_bstr_t bsStock(L"");
	_bstr_t bsExchange(L"");
	if(spStockRequest->m_recParams->FutureSymbol)
		bsStock = spStockRequest->m_recParams->FutureSymbol;
	if(spStockRequest->m_recParams->Exchange)
		bsExchange = spStockRequest->m_recParams->Exchange;

	GetMultiplePatternMatch::RequestParameters getMultiplePatternMatchListRequestParameters;
	getMultiplePatternMatchListRequestParameters.m_tableNumber = TABLE_NO_US_FUTURE;

	getMultiplePatternMatchListRequestParameters.m_symbolPattern = bsStock + ".*";

	RequestBlock requestBlock1;
	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_FUTURE_ROOT;
	requestBlock1.m_fieldIdList.push_back(FID_CONTRACT_SIZE);
	requestBlock1.m_fieldIdList.push_back(FID_MINIMUM_TICK); 
	requestBlock1.m_fieldIdList.push_back(FID_UNIT_OF_MEASURE);
	requestBlock1.m_fieldIdList.push_back(FID_UNIT_OF_MEASURE_CURRENCY);
	requestBlock1.m_fieldIdList.push_back(FID_EXPIRATION_DATE);
	getMultiplePatternMatchListRequestParameters.m_requestBlockList.push_back(requestBlock1);

	RequestBlock requestBlock;
	requestBlock.m_relationshipId = Feed::RELATIONSHIP_ID_FUTURE_OPTION_EXPIRATION_ROOT;
	//requestBlock.m_fieldIdList.push_back(FID_NAME);
	getMultiplePatternMatchListRequestParameters.m_requestBlockList.push_back(requestBlock);

	//getMultiplePatternMatchListRequestParameters.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	//getMultiplePatternMatchListRequestParameters.m_permissionLevel = /*(m_isDelayed ? PERMISSION_LEVEL_DELAYED : */PERMISSION_LEVEL_DEFAULT/*)*/;

	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetMultiplePatternMatch::PostRequest(*this, *spRequestID, getMultiplePatternMatchListRequestParameters));

	CAutoLock lock(m_csContainer);
	m_Container[spRequestID] = spStockRequest;

	return STATUS_CODE_SUCCESS;
}

bool GetOptionRootPattern(const std::string &activRootSymbol, std::string &activOptionRootSymbol)
{
	std::string::size_type i = activRootSymbol.find_last_of('.');

	if (std::string::npos == i)
		return false;

	activOptionRootSymbol = activRootSymbol.substr(0, i);// + "/*";

	return true;
}

bool GetOptionRootFromOption(const std::string &activSymbol, std::string &activOptionSymbol)
{
	std::string::size_type i = activSymbol.find_first_of('/');

	if (std::string::npos == i)
		return false;

	activOptionSymbol = activSymbol.substr(0, i);

	return true;
}

bool GetOptionPattern(const std::string &activSymbol, std::string &activOptionSymbol)
{
	std::string::size_type i = activSymbol.find_last_of('.');

	if (std::string::npos == i)
		return false;

	activOptionSymbol = activSymbol.substr(0, i);

	while ((i = activOptionSymbol.find_first_of('/')) != activOptionSymbol.npos){
		activOptionSymbol = activOptionSymbol.substr(0, i) + activOptionSymbol.substr(i+1);
	}

	return true;
}

StatusCode CActivFeedContentGateway::SaveOptionRoots(HeapMessage &response)
{
	GetEqual::ResponseParameters responseParameters;
	StatusCode statusCode = GetEqual::Deserialize(*this, response,
		responseParameters);
	if (STATUS_CODE_SUCCESS != statusCode)
		return statusCode;
	ResponseBlockList &rbl =
		responseParameters.m_responseBlockList;
	ResponseBlockList::const_iterator it;

	for(it = rbl.begin(); it != rbl.end(); ++it)
	{
		const ResponseBlock &rb = *it;
		if (!rb.IsValidResponse())
			continue;

		FieldListValidator  validator;
		StatusCode sCode = validator.Initialize(rb.m_fieldData);
		if(STATUS_CODE_SUCCESS == sCode)
		{
			optionRootInfo ori;
			for (FieldListValidator::ConstIterator FieldListValidatorIterator = validator.Begin(), FieldListValidatorIteratorEnd = validator.End(); FieldListValidatorIteratorEnd != FieldListValidatorIterator; ++FieldListValidatorIterator)
			{
				const FieldListValidator::Field & field = FieldListValidatorIterator.GetField();
				if (0 != (field.m_rules & FieldListValidator::Field::RULE_DOES_NOT_UPDATE_LAST_VALUE))
					continue;

				switch(field.m_fieldId)
				{

				case Feed::FID_SYMBOL:	//106
					{
						Activ::TextString const * pText;
						if (STATUS_CODE_SUCCESS == field.GetActivFieldType(pText)){
							std::string symbol;
							symbol = pText->ToString();
							GetOptionRootPattern(symbol, ori.rootSymbol);
						}
					}break;
				case Feed::FID_CONTRACT_MULTIPLIER:	//0	???
					{
						Activ::Rational const *opn;
						if (STATUS_CODE_SUCCESS == field.GetActivFieldType(opn)){
							ori.lotsize = static_cast<long>( opn->Get() );
						}
					}break;
				case Feed::FID_CURRENCY: //105
					{
						Activ::TextArray pText;
						IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
						if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
							ori.currency = pText.ToString();
					}break;
				case Feed::FID_EXPIRATION_TYPE:
					{
						Activ::UInt const * uiLotSize;
						if (STATUS_CODE_SUCCESS == field.GetActivFieldType(uiLotSize))
							ori.expirationType = (long)uiLotSize->Get();
					}   break;
				case Feed::FID_SETTLEMENT_TYPE:
					{
						ori.SettlementType = static_cast<long>(enSTUndefined);
						Activ::TextArray pText;
						IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
						if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
						{
							const char ch= *pText.ToString().c_str();
							if(ch == 'a' || ch == 'A')
								ori.SettlementType = static_cast<long>(enSTAM);
							else if(ch == 'p' || ch == 'P')
								ori.SettlementType = static_cast<long>(enSTPM);
						}
					}   break;
				}

			}
			m_optionRoots[ori.rootSymbol] = ori;
		}
	}
	return statusCode;
}

StatusCode CActivFeedContentGateway::SaveIndexOptionRoots(HeapMessage &response)
{
	GetMultiplePatternMatch::ResponseParameters responseParameters;
	StatusCode statusCode = GetMultiplePatternMatch::Deserialize(*this, response,
		responseParameters);
	if (STATUS_CODE_SUCCESS != statusCode)
		return statusCode;
	ResponseBlockList &rbl =
		responseParameters.m_responseBlockList;
	ResponseBlockList::const_iterator it;

	for(it = rbl.begin(); it != rbl.end(); ++it)
	{
		const ResponseBlock &rb = *it;
		//We only have data if the status for this
		//block is good or stale so check here.
		if (!rb.IsValidResponse())
			continue;
		//Check that this block refers to our partial request
		FieldListValidator  validator;
		StatusCode sCode = validator.Initialize(rb.m_fieldData);
		if(STATUS_CODE_SUCCESS == sCode)
		{
			optionRootInfo ori;
			for (FieldListValidator::ConstIterator FieldListValidatorIterator = validator.Begin(), FieldListValidatorIteratorEnd = validator.End(); FieldListValidatorIteratorEnd != FieldListValidatorIterator; ++FieldListValidatorIterator)
			{
				const FieldListValidator::Field & field = FieldListValidatorIterator.GetField();
				if (0 != (field.m_rules & FieldListValidator::Field::RULE_DOES_NOT_UPDATE_LAST_VALUE))
					continue;

				switch(field.m_fieldId)
				{

				case Feed::FID_SYMBOL:	//106
					{
						Activ::TextString const * pText;
						if (STATUS_CODE_SUCCESS == field.GetActivFieldType(pText)){
							std::string symbol;
							symbol = pText->ToString();//::SysAllocString(CA2W(pText->ToString().c_str()));
							GetOptionRootPattern(symbol, ori.rootSymbol);
						}
					}break;
				case Feed::FID_SHARES_PER_CONTRACT:	//0	???
					{
						Activ::UInt const * uiLotSize;
						if (STATUS_CODE_SUCCESS == field.GetActivFieldType(uiLotSize))
							ori.lotsize = (long)uiLotSize->Get();
					}break;
				case Feed::FID_CURRENCY: //105
					{
						Activ::TextArray pText;
						IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
						if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
							ori.currency = pText.ToString();//::SysAllocString(CA2W(pText.ToString().c_str()));
					}break;

				case Feed::FID_EXPIRATION_TYPE:
					{
						Activ::UInt const * uiLotSize;
						if (STATUS_CODE_SUCCESS == field.GetActivFieldType(uiLotSize))
							ori.expirationType = (long)uiLotSize->Get();
					}   break;
				case Feed::FID_SETTLEMENT_TYPE:
					{
						ori.SettlementType = static_cast<long>(enSTUndefined);
						Activ::TextArray pText;
						IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
						if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
						{
							const char ch= *pText.ToString().c_str();
							if(ch == 'a' || ch == 'A')
								ori.SettlementType = static_cast<long>(enSTAM);
							else if (ch == 'p' || ch == 'P')
								ori.SettlementType = static_cast<long>(enSTPM);
						}
					}   break;

				}

			}
			m_optionRoots[ori.rootSymbol] = ori;
		}
	}
	return statusCode;
}

StatusCode CActivFeedContentGateway::SaveFutureOptionRoots(HeapMessage &response)
{
	GetEqual::ResponseParameters responseParameters;
	StatusCode statusCode = GetMultiplePatternMatch::Deserialize(*this, response,
		responseParameters);
	if (STATUS_CODE_SUCCESS != statusCode)
		return statusCode;
	ResponseBlockList &rbl =
		responseParameters.m_responseBlockList;
	ResponseBlockList::const_iterator it;

	long lLotSize = 0;
	bool bFirst = true;

	m_optionFutureRoots.clear();
	for(it = rbl.begin(); it != rbl.end(); ++it)
	{
		const ResponseBlock &rb = *it;
		//We only have data if the status for this
		//block is good or stale so check here.
		/*if (!rb.IsValidResponse())
		continue;*/
		string s = rb./*m_requestKey.*/m_responseKey.m_symbol;
		size_t i = s.find_last_of('.');
		if(i!=s.npos){
			s.resize(i);
			m_optionFutureRoots[s] = lLotSize;

			if(bFirst){
				FieldListValidator  validator;
				StatusCode sCode = validator.Initialize(rb.m_fieldData);

				if(STATUS_CODE_SUCCESS == sCode)
				{
					for (FieldListValidator::ConstIterator FieldListValidatorIterator = validator.Begin(), FieldListValidatorIteratorEnd = validator.End(); FieldListValidatorIteratorEnd != FieldListValidatorIterator; ++FieldListValidatorIterator)
					{
						const FieldListValidator::Field & field = FieldListValidatorIterator.GetField();
						if (0 != (field.m_rules & FieldListValidator::Field::RULE_DOES_NOT_UPDATE_LAST_VALUE))
							continue;

						switch(field.m_fieldId)
						{
						case Feed::FID_CONTRACT_SIZE:	//0	???
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									lLotSize = (long)dClose.Get();
									bFirst = false;
								}

							}break;
						}
					}
				}
			}
		}
	}
	return statusCode;
}

StatusCode CActivFeedContentGateway::RequestIndexOptions(RequestIdPtr pRequestID)
{
	CRequestBasePtr pRequest = GetRequest(pRequestID);
	if(!pRequest) return STATUS_CODE_FAILURE;
	CRequest<OptionParams>::CRequestPtr spOptionRequest = boost::shared_dynamic_cast<CRequest<OptionParams> >(pRequest);

	GetMultiplePatternMatch::RequestParameters getMultiplePatternMatchListRequestParameters;
	getMultiplePatternMatchListRequestParameters.m_tableNumber = TABLE_NO_INDEX;

	std::string symbolPattern = "=" + string((LPCSTR)(_bstr_t(spOptionRequest->m_recParams->UnderlyingSymbol)))+ ".*";// = *i;

	getMultiplePatternMatchListRequestParameters.m_symbolPattern = symbolPattern;

	RequestBlock requestBlock;

	requestBlock.m_relationshipId = Feed::RELATIONSHIP_ID_OPTION_ROOT;
	requestBlock.m_fieldIdList.push_back(Feed::FID_SYMBOL);
	requestBlock.m_fieldIdList.push_back(Feed::FID_SHARES_PER_CONTRACT);
	requestBlock.m_fieldIdList.push_back(Feed::FID_SETTLEMENT_TYPE);	
	requestBlock.m_fieldIdList.push_back(Feed::FID_EXPIRATION_TYPE);	
	requestBlock.m_fieldIdList.push_back(Feed::FID_CURRENCY);

	getMultiplePatternMatchListRequestParameters.m_requestBlockList.push_back(requestBlock);

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetMultiplePatternMatch::PostRequest(*this, /*RequestId::emptyRequestId*//**spRequestID*/*pRequestID, getMultiplePatternMatchListRequestParameters));
	return STATUS_CODE_SUCCESS;
}

StatusCode CActivFeedContentGateway::RequestOptionsByRoot(RequestIdPtr pRequestID)
{
	GetMultiplePatternMatchList::RequestParameters getMultiplePatternMatchListRequestParameters;
	getMultiplePatternMatchListRequestParameters.m_tableNumber = TABLE_NO_US_EQUITY_OPTION;

	COptionRootInfoMap::const_iterator i;
	for(i = m_optionRoots.begin(); i != m_optionRoots.end(); ++i)
	{
		std::string symbolPattern = i->first+"/*.O";// = *i;

		getMultiplePatternMatchListRequestParameters.m_symbolPatternList.push_back(symbolPattern);
	}
	RequestBlock requestBlock;

	requestBlock.m_fieldIdList.push_back(FID_SYMBOL);
	requestBlock.m_fieldIdList.push_back(FID_OPTION_TYPE);
	requestBlock.m_fieldIdList.push_back(FID_STRIKE_PRICE);
	requestBlock.m_fieldIdList.push_back(FID_EXPIRATION_DATE);
	requestBlock.m_fieldIdList.push_back(FID_CLOSE);

	requestBlock.m_fieldIdList.push_back(FID_BID);
	requestBlock.m_fieldIdList.push_back(FID_ASK);
	requestBlock.m_fieldIdList.push_back(FID_TRADE);
	requestBlock.m_fieldIdList.push_back(FID_CUMULATIVE_VOLUME);
	requestBlock.m_fieldIdList.push_back(FID_CLOSE_CUMULATIVE_VOLUME);
	requestBlock.m_fieldIdList.push_back(FID_OPEN_INTEREST);
	requestBlock.m_fieldIdList.push_back(FID_PREVIOUS_ASK);
	requestBlock.m_fieldIdList.push_back(FID_PREVIOUS_BID);

	getMultiplePatternMatchListRequestParameters.m_requestBlockList.push_back(requestBlock);

	//getMultiplePatternMatchListRequestParameters.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	//getMultiplePatternMatchListRequestParameters.m_permissionLevel = /*(m_isDelayed ? PERMISSION_LEVEL_DELAYED : */PERMISSION_LEVEL_DEFAULT/*)*/;

	if (!getMultiplePatternMatchListRequestParameters.m_symbolPatternList.empty())
	{
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetMultiplePatternMatchList::PostRequest(*this, /*RequestId::emptyRequestId*//**spRequestID*/*pRequestID, getMultiplePatternMatchListRequestParameters));
	}
	else
	{
		CActivFeedAppication* pApplication = dynamic_cast<CActivFeedAppication*>(&GetApplication());
		if(pApplication){
			CRequestBasePtr pRequest = GetRequest(pRequestID);
			if(pRequest)
			{
				CRequest<OptionParams>::CRequestPtr spOptionRequest = boost::shared_dynamic_cast<CRequest<OptionParams> >(pRequest);

				CComVariant vtRequest;
				spOptionRequest->m_recParams.CopyTo(vtRequest);
				pApplication->OnSPError(enNoDataAvailableForSymbol, "No option roots received", enRequestOptions, vtRequest);
			}
		}
	}
	return STATUS_CODE_SUCCESS;
}

StatusCode CActivFeedContentGateway::RequestFutureOptionsByRoot(RequestIdPtr pRequestID)
{
	GetMultiplePatternMatchList::RequestParameters getMultiplePatternMatchListRequestParameters;
	getMultiplePatternMatchListRequestParameters.m_tableNumber = TABLE_NO_US_FUTURE_OPTION;

	map<string, long>::const_iterator i;
	for(i = m_optionFutureRoots.begin(); i != m_optionFutureRoots.end(); ++i)
	{
		std::string symbolPattern = i->first+"/*.*";// = *i;

		getMultiplePatternMatchListRequestParameters.m_symbolPatternList.push_back(symbolPattern);
	}
	RequestBlock requestBlock;

	requestBlock.m_fieldIdList.push_back(FID_SYMBOL);
	requestBlock.m_fieldIdList.push_back(FID_OPTION_TYPE);
	requestBlock.m_fieldIdList.push_back(FID_STRIKE_PRICE);
	requestBlock.m_fieldIdList.push_back(FID_LAST_TRADING_DATE);
	requestBlock.m_fieldIdList.push_back(FID_EXPIRATION_DATE);
	requestBlock.m_fieldIdList.push_back(FID_SETTLEMENT);
	requestBlock.m_fieldIdList.push_back(FID_BID);
	requestBlock.m_fieldIdList.push_back(FID_ASK);
	requestBlock.m_fieldIdList.push_back(FID_PREVIOUS_BID);
	requestBlock.m_fieldIdList.push_back(FID_PREVIOUS_ASK);
	requestBlock.m_fieldIdList.push_back(FID_TRADE);

	getMultiplePatternMatchListRequestParameters.m_requestBlockList.push_back(requestBlock);

	//getMultiplePatternMatchListRequestParameters.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	//getMultiplePatternMatchListRequestParameters.m_permissionLevel = /*(m_isDelayed ? PERMISSION_LEVEL_DELAYED : */PERMISSION_LEVEL_DEFAULT/*)*/;

	if (!getMultiplePatternMatchListRequestParameters.m_symbolPatternList.empty())
	{
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetMultiplePatternMatchList::PostRequest(*this, /*RequestId::emptyRequestId*//**spRequestID*/*pRequestID, getMultiplePatternMatchListRequestParameters));
	}
	else
	{
		CActivFeedAppication* pApplication = dynamic_cast<CActivFeedAppication*>(&GetApplication());
		if(pApplication){
			CRequestBasePtr pRequest = GetRequest(pRequestID);
			if(pRequest)
			{
				CRequest<FuturesOptionParams>::CRequestPtr spOptionRequest = boost::shared_dynamic_cast<CRequest<FuturesOptionParams> >(pRequest);

				CComVariant vtRequest;
				spOptionRequest->m_recParams.CopyTo(vtRequest);
				pApplication->OnSPError(enNoDataAvailableForSymbol, "No future option roots received", enRequestFuturesOptions, vtRequest);
			}
		}
	}
	return STATUS_CODE_SUCCESS;
}

StatusCode CActivFeedContentGateway::RequestFutures(CRequest<FutureRootParams>::CRequestPtr& spStockRequest)
{
	_bstr_t	  bsFutureRoot(L"");
	m_futMap.clear();

	if(spStockRequest->m_recParams->FuturesRootSymbol)
		bsFutureRoot = spStockRequest->m_recParams->FuturesRootSymbol;

	GetMultiplePatternMatch::RequestParameters getMultiplePatternMatchListRequestParameters;
	getMultiplePatternMatchListRequestParameters.m_tableNumber = TABLE_NO_US_FUTURE;

	getMultiplePatternMatchListRequestParameters.m_symbolPattern = bsFutureRoot + "/*";;

	RequestBlock requestBlock1;
	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back(Feed::FID_SYMBOL);
	requestBlock1.m_fieldIdList.push_back(Feed::FID_LAST_TRADING_DATE);
	requestBlock1.m_fieldIdList.push_back(Feed::FID_SETTLEMENT);
	requestBlock1.m_fieldIdList.push_back(Feed::FID_CLOSE);
	requestBlock1.m_fieldIdList.push_back(Feed::FID_BID);
	requestBlock1.m_fieldIdList.push_back(Feed::FID_ASK);
	requestBlock1.m_fieldIdList.push_back(Feed::FID_TRADE);
	requestBlock1.m_fieldIdList.push_back(Feed::FID_EXPIRATION_DATE);

	getMultiplePatternMatchListRequestParameters.m_requestBlockList.push_back(requestBlock1);

	RequestBlock requestBlock2;
	requestBlock2.m_relationshipId = Feed::RELATIONSHIP_ID_FUTURE_ROOT;
	requestBlock2.m_fieldIdList.push_back(Feed::FID_CONTRACT_SIZE);
	requestBlock2.m_fieldIdList.push_back(Feed::FID_MINIMUM_TICK);     
	requestBlock2.m_fieldIdList.push_back(Feed::FID_UNIT_OF_MEASURE);
	requestBlock2.m_fieldIdList.push_back(Feed::FID_UNIT_OF_MEASURE_CURRENCY);
	requestBlock2.m_fieldIdList.push_back(Feed::FID_NAME);
	getMultiplePatternMatchListRequestParameters.m_requestBlockList.push_back(requestBlock2);

	//getMultiplePatternMatchListRequestParameters.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	//getMultiplePatternMatchListRequestParameters.m_permissionLevel = /*(m_isDelayed ? PERMISSION_LEVEL_DELAYED : */PERMISSION_LEVEL_DEFAULT/*)*/;

	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetMultiplePatternMatch::PostRequest(*this, *spRequestID, getMultiplePatternMatchListRequestParameters));

	CAutoLock lock(m_csContainer);
	m_Container[spRequestID] = spStockRequest;

	return STATUS_CODE_SUCCESS;
}

void	CActivFeedContentGateway::ParseResponseBlock(const ResponseBlock& rb, _QuoteInfo * pQuoteInfo, bool bRealTime, _QuoteRequest *pRequest)
{
	if( rb.IsValidResponse() )
	{
		if( STATUS_CODE_SUCCESS == m_FieldListValidator.Initialize( rb.m_fieldData ) )
		{
			double dRate	=	1.0;
			if (pRequest)
				dRate = pRequest->Rate;

			FieldListValidator::ConstIterator fit, fitEnd;
			for ( fit = m_FieldListValidator.Begin(), fitEnd = m_FieldListValidator.End(); fit != fitEnd; fit++ ) 
			{
				const FieldListValidator::Field& fld = fit.GetField();
				if ( FIELD_STATUS_DEFINED == fld.m_fieldStatus ) 
				{
					switch( fld.m_fieldId) 
					{
					case Feed::FID_OPEN_INTEREST:
						{
							Activ::UInt bidsize;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&bidsize);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->OpenInterest = static_cast<long>( bidsize.Get());
						}
						break;
					case Feed::FID_CURRENCY:
						{
							Activ::TextArray pText;
							IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->Currency = pText.ToString();
						}
						break;
					case Feed::FID_PREVIOUS_BID:
						{
							if( ( pQuoteInfo->BidPrice == BAD_DOUBLE_VALUE || !pQuoteInfo->BidPrice ) && (!bRealTime || pRequest->Type == enOPT) ) 
							{
								Activ::Rational bid;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&bid);
								(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
								pQuoteInfo->BidPrice = bid.Get() * dRate;
							}
						}
						break;
					case Feed::FID_BID:
						{
							Activ::TRational bid;
							IFieldType* pUpdateField = static_cast<Activ::TRational*>(&bid);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->BidPrice = bid.Get() * dRate;
							Activ::TRational::Tick t = bid.GetTick();
							switch( t )
							{
							case Activ::TRational::TICK_DOWN:
								pQuoteInfo->PriceTick |= enBidDnTick;
								break;
							case Activ::TRational::TICK_UP:
								pQuoteInfo->PriceTick |= enBidUpTick;
								break;
							default:;
							}
						}
						break;
					case Feed::FID_BID_SIZE:
						{
							Activ::UInt bidsize;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&bidsize);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->BidSize = static_cast<long>(bidsize.Get());
						}
						break;
					case Feed::FID_BID_EXCHANGE:
						{
							Activ::TextString pText;
							IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->BidExchange = pText.ToString();
						}
						break;
					case Feed::FID_PREVIOUS_ASK:
						{
							if( ( pQuoteInfo->AskPrice == BAD_DOUBLE_VALUE ||  !pQuoteInfo->AskPrice ) && ( !bRealTime || pRequest->Type == enOPT ) ) 
							{
								Activ::Rational ask;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&ask);
								(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
								pQuoteInfo->AskPrice = ask.Get() * dRate;
							}
						}
						break;
					case Feed::FID_ASK:
						{
							Activ::TRational ask;
							IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->AskPrice = ask.Get() * dRate;
							Activ::TRational::Tick t = ask.GetTick();
							switch( t )
							{
							case Activ::TRational::TICK_DOWN:
								pQuoteInfo->PriceTick |= enAskDnTick;
								break;
							case Activ::TRational::TICK_UP:
								pQuoteInfo->PriceTick |= enAskUpTick;
								break;
							default:;
							}
						}
						break;
					case Feed::FID_ASK_SIZE:
						{
							Activ::UInt asksize;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&asksize);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->AskSize = static_cast<long>(asksize.Get());
						}
						break;
					case Feed::FID_ASK_EXCHANGE:
						{
							Activ::TextString pText;
							IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->AskExchange = pText.ToString();
						}
						break;
					case Feed::FID_TRADE_EXCHANGE:
						{
							Activ::TextString pText;
							IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->Exchange = pText.ToString();
						}
						break;
					case Feed::FID_OPEN:
						{
							Activ::Rational opn;
							IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->OpenPrice = opn.Get() * dRate;
						}
						break;
					case Feed::FID_LAST_UPDATE_DATE:
						{
							Activ::Date dt;
							EgLib::vt_date	pdate( pQuoteInfo->UpdateDateTime );
							IFieldType* pUpdateField = static_cast<Activ::Date*>(&dt);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							int year = dt.GetYear();
							int month = dt.GetMonth();
							int day = dt.GetDay();
							EgLib::vt_date d( year, month, day, 0/*pdate.get_hour()*/, 0/*pdate.get_minute()*/, 0/*pdate.get_second()*/ );
							pQuoteInfo->UpdateDateTime = d;
						}
						break;
					case Feed::FID_LAST_UPDATE_TIME:
						{
							Activ::Time dt;
							EgLib::vt_date	pdate( pQuoteInfo->UpdateDateTime );
							IFieldType* pUpdateField = static_cast<Activ::Time*>(&dt);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							int hour = dt.GetHour();
							int minute = dt.GetMinute();
							int second = dt.GetSecond();
							EgLib::vt_date d( pdate.get_year(), pdate.get_month(), pdate.get_day(),hour, minute, second );
							pQuoteInfo->UpdateDateTime = d;
						}
						break;
					case Feed::FID_CUMULATIVE_VOLUME:
						{
							Activ::UInt volume;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&volume);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->Volume = static_cast<long>(volume.Get());
						}
						break;
					case Feed::FID_CLOSE:
						{
							Activ::Rational opn;
							IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->ClosePrice = opn.Get() * dRate;

							if( ( pQuoteInfo->LastPrice == BAD_DOUBLE_VALUE || !pQuoteInfo->LastPrice) /*&& pRequest && pRequest->Type != enOPT/*&& !bRealTime*/ ) 
							{
								pQuoteInfo->LastPrice = pQuoteInfo->ClosePrice;

							}
						}
						break;
					case Feed::FID_CONTRACT_SIZE:
					case Feed::FID_SHARES_PER_CONTRACT:
						{
							Activ::UInt asksize;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&asksize);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->LotSize = static_cast<long>(asksize.Get());
						}
						break;
					case Feed::FID_TRADE:
						{
							Activ::TRational ask;
							IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->LastPrice = ask.Get() * dRate;
							Activ::TRational::Tick t = ask.GetTick();
							switch( t )
							{
							case Activ::TRational::TICK_DOWN:
								pQuoteInfo->PriceTick |= enLastSaleDnTick;
								break;
							case Activ::TRational::TICK_UP:
								pQuoteInfo->PriceTick |= enLastSaleUpTick;
								break;
							default:;
							}
						}
						break;
					case Feed::FID_TRADE_HIGH:
						{
							Activ::Rational opn;
							IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->HighPrice = opn.Get() * dRate;
						}
						break;
					case Feed::FID_TRADE_LOW:
						{
							Activ::Rational opn;
							IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->LowPrice = opn.Get() * dRate;
						}
						break;
					case Feed::FID_NET_CHANGE:
						{
							Activ::TRational ask;
							IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->NetChange = ask.Get() * dRate;
						}
						break;
					default:;
					}
				}
			}
		}
	}
	else
		pQuoteInfo->Status = enQuoteStatus_Unreliable;
}

template<typename _Reaquest>
CActivFeedAppication*	CActivFeedContentGateway::ApplyResponseToQUI( _Reaquest &Params, _QuoteInfo * pResponse, bool bRealTime, _QuoteRequest *pRequest, ResponseBlock::Status* prbStatus )
{
	CActivFeedAppication* pApp = dynamic_cast<CActivFeedAppication*>(&GetApplication());
	if(prbStatus)
		*prbStatus =  ResponseBlock::STATUS_SUCCESS;
	if(!pApp)
	{
		pResponse->Status = enQuoteStatus_Unreliable;
		return NULL;
	}

	//	GetEqual::ResponseParameters Params;
	//	StatusCode statusCode = GetEqual::Deserialize(*this, response, Params);
	StatusCode statusCode = STATUS_CODE_SUCCESS; 

	if (STATUS_CODE_SUCCESS != statusCode)
	{
		pResponse->Status = enQuoteStatus_Unreliable;
		return pApp;
	}

	ResponseBlockList rbl = Params.m_responseBlockList;
	ResponseBlockList::const_iterator it;
	for( it = rbl.begin(); it != rbl.end(); it++ )
	{
		const ResponseBlock& rb = *it;
		if( rb.m_status != ResponseBlock::STATUS_SUCCESS /*&& rb.m_status != ResponseBlock::STATUS_STALE*/ )
		{
			if(prbStatus)
				*prbStatus = rb.m_status;
		}
		ParseResponseBlock(rb,  pResponse, bRealTime, pRequest);
	}
	return pApp;
}

void	CActivFeedContentGateway::ApplyMPResponseToQUI( ResponseBlock& rb, _QuoteInfo * p, bool bRealTime , _QuoteRequest *pRequest)
{
	if( rb.IsValidResponse() )
	{
		if( STATUS_CODE_SUCCESS == m_FieldListValidator.Initialize( rb.m_fieldData ) )
		{
			double	dRate = 1.0;
			if (pRequest)
				dRate	=	pRequest->Rate;

			FieldListValidator::ConstIterator fit, fitEnd;
			for ( fit = m_FieldListValidator.Begin(), fitEnd = m_FieldListValidator.End(); fit != fitEnd; fit++ ) 
			{
				const FieldListValidator::Field& fld = fit.GetField();
				if ( FIELD_STATUS_DEFINED == fld.m_fieldStatus ) 
				{
					switch( fld.m_fieldId) 
					{
					case Feed::FID_OPEN_INTEREST:
						{
							Activ::UInt bidsize;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&bidsize);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->OpenInterest = static_cast<long>( bidsize.Get());
						}
						break;
					case Feed::FID_CURRENCY:
						{
							Activ::TextArray pText;
							IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->Currency = pText.ToString();
						}
						break;
					case Feed::FID_PREVIOUS_BID:
						{
							if( ( p->LastPrice == BAD_DOUBLE_VALUE || !p->LastPrice) && !bRealTime  ) {
								Activ::Rational bid;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&bid);
								(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
								p->BidPrice = bid.Get() * dRate;
							}
						}
						break;
					case Feed::FID_BID:
						{
							Activ::TRational bid;
							IFieldType* pUpdateField = static_cast<Activ::TRational*>(&bid);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->BidPrice = bid.Get() * dRate;
							Activ::TRational::Tick t = bid.GetTick();
							switch( t )
							{
							case Activ::TRational::TICK_DOWN:
								p->PriceTick |= enBidDnTick;
								break;
							case Activ::TRational::TICK_UP:
								p->PriceTick |= enBidUpTick;
								break;
							default:;
							}
						}
						break;
					case Feed::FID_BID_SIZE:
						{
							Activ::UInt bidsize;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&bidsize);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->BidSize = static_cast<long>(bidsize.Get());
						}
						break;
					case Feed::FID_BID_EXCHANGE:
						{
							Activ::TextString pText;
							IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->BidExchange = pText.ToString();
						}
						break;
					case Feed::FID_PREVIOUS_ASK:
						{
							if( ( p->LastPrice == BAD_DOUBLE_VALUE || !p->LastPrice) && !bRealTime  ) {
								Activ::Rational ask;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&ask);
								(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
								p->AskPrice = ask.Get() * dRate;
							}
						}
						break;
					case Feed::FID_ASK:
						{
							Activ::TRational ask;
							IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->AskPrice = ask.Get() * dRate;
							Activ::TRational::Tick t = ask.GetTick();
							switch( t )
							{
							case Activ::TRational::TICK_DOWN:
								p->PriceTick |= enAskDnTick;
								break;
							case Activ::TRational::TICK_UP:
								p->PriceTick |= enAskUpTick;
								break;
							default:;
							}
						}
						break;
					case Feed::FID_ASK_SIZE:
						{
							Activ::UInt asksize;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&asksize);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->AskSize = static_cast<long>( asksize.Get() );
						}
						break;
					case Feed::FID_ASK_EXCHANGE:
						{
							Activ::TextString pText;
							IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->AskExchange = pText.ToString();
						}
						break;
					case Feed::FID_TRADE_EXCHANGE:
						{
							Activ::TextString pText;
							IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->Exchange = pText.ToString();
						}
						break;
					case Feed::FID_OPEN:
						{
							Activ::Rational opn;
							IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->OpenPrice = opn.Get() * dRate;
						}
						break;
					case Feed::FID_LAST_UPDATE_DATE:
						{
							Activ::Date dt;
							EgLib::vt_date	pdate( p->UpdateDateTime );
							IFieldType* pUpdateField = static_cast<Activ::Date*>(&dt);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							int year = dt.GetYear();
							int month = dt.GetMonth();
							int day = dt.GetDay();
							EgLib::vt_date d( year, month, day, 0/*pdate.get_hour()*/, 0/*pdate.get_minute()*/, 0/*pdate.get_second()*/ );
							p->UpdateDateTime = d;
						}
						break;
					case Feed::FID_LAST_UPDATE_TIME:
						{
							Activ::Time dt;
							EgLib::vt_date	pdate( p->UpdateDateTime );
							IFieldType* pUpdateField = static_cast<Activ::Time*>(&dt);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							int hour = dt.GetHour();
							int minute = dt.GetMinute();
							int second = dt.GetSecond();
							EgLib::vt_date d( pdate.get_year(), pdate.get_month(), pdate.get_day(),hour, minute, second );
							p->UpdateDateTime = d;
						}
						break;
					case Feed::FID_CUMULATIVE_VOLUME:
						{
							Activ::UInt volume;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&volume);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->Volume = static_cast<long>( volume.Get() );
						}
						break;
					case Feed::FID_CLOSE:
						{
							Activ::Rational opn;
							IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->ClosePrice = opn.Get() * dRate;
							if( ( p->LastPrice == BAD_DOUBLE_VALUE || !p->LastPrice) && !bRealTime ) {
								p->LastPrice = p->ClosePrice;
							}
						}
						break;
					case Feed::FID_CONTRACT_SIZE:
					case Feed::FID_SHARES_PER_CONTRACT:
						{
							Activ::UInt asksize;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&asksize);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->LotSize = static_cast<long>( asksize.Get() );
						}
						break;
					case Feed::FID_TRADE:
						{
							Activ::TRational ask;
							IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->LastPrice = ask.Get() * dRate;
							Activ::TRational::Tick t = ask.GetTick();
							switch( t )
							{
							case Activ::TRational::TICK_DOWN:
								p->PriceTick |= enLastSaleDnTick;
								break;
							case Activ::TRational::TICK_UP:
								p->PriceTick |= enLastSaleUpTick;
								break;
							default:;
							}
						}
						break;
					case Feed::FID_TRADE_HIGH:
						{
							Activ::Rational opn;
							IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->HighPrice = opn.Get() * dRate;
						}
						break;
					case Feed::FID_TRADE_LOW:
						{
							Activ::Rational opn;
							IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->LowPrice = opn.Get() * dRate;
						}
						break;
					case Feed::FID_NET_CHANGE:
						{
							Activ::TRational ask;
							IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							p->NetChange = ask.Get() * dRate;
						}
						break;
					default:;
					}
				}
			}

		}
	}
	else
		p->Status = enQuoteStatus_OK; //Unreliable;
}


void CActivFeedContentGateway::OnStock(CRequest<StockParams>::CRequestPtr& spRequest, HeapMessage &response)
{ 
	CActivFeedAppication* pApplication = dynamic_cast<CActivFeedAppication*>(&GetApplication());
	if(pApplication)
	{
		GetEqual::ResponseParameters responseParameters;
		//Use the helper class to 'crack' the response message into
		//typed responseParameters
		StatusCode statusCode = GetEqual::Deserialize(*this, response,
			responseParameters);

		if (STATUS_CODE_SUCCESS != statusCode)
			return;

		CResponse<StockResultsEx>::CResponsePtr spResult(new CResponse<StockResultsEx>());
		if(spResult!=NULL)
		{
			spResult->m_respParams->StockType = enIndex;
			ResponseBlockList &rbl =
				responseParameters.m_responseBlockList;
			ResponseBlockList::const_iterator it;
			bool bUseNextDiv = false;

			for(it = rbl.begin(); it != rbl.end(); ++it)
			{
				const ResponseBlock &rb = *it;
				//We only have data if the status for this
				//block is good or stale so check here.
				if (!rb.IsValidResponse())
					continue;
				//Check that this block refers to our partial request
				FieldListValidator  validator;
				StatusCode sCode = validator.Initialize(rb.m_fieldData);
				if(STATUS_CODE_SUCCESS == sCode)
				{
					spResult->m_respParams->LotSize   = 100;
					spResult->m_respParams->StockType = enStock;
 
					std::string Ae = "";
					std::string exchRegion = "USA";

					double	dRate = 1.0;
					if(spRequest->m_recParams->Exchange)
					{
						std::string Ee = _bstr_t(spRequest->m_recParams->Exchange);
						EgarSTK2Activ( Ee, Ae, exchRegion );
						dRate = ActivFeedSettings::g_spUserSettings->Settings->GetRegionRate(exchRegion);
					}	

					for (FieldListValidator::ConstIterator FieldListValidatorIterator = validator.Begin(), FieldListValidatorIteratorEnd = validator.End(); FieldListValidatorIteratorEnd != FieldListValidatorIterator; ++FieldListValidatorIterator)
					{
						const FieldListValidator::Field & field = FieldListValidatorIterator.GetField();
						if (0 != (field.m_rules & FieldListValidator::Field::RULE_DOES_NOT_UPDATE_LAST_VALUE))
							continue;

						switch(field.m_fieldId)
						{
						case Feed::FID_PRIMARY_EXCHANGE:  //105
							{
								Activ::TextString pText;
								IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									std::string s = pText.ToString();
									Activ2Egar( s, enSTK );
									spResult->m_respParams->Exchange = ::SysAllocString( CA2W( s.c_str() ) );
									//spResult->m_respParams->Exchange = ::SysAllocString(CA2W(pText.ToString().c_str()));
								}
							}break;
						case Feed::FID_PREVIOUS_ASK:
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->CloseAsk = dClose.Get() * dRate;

							}break;
						case Feed::FID_PREVIOUS_BID:
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->CloseBid = dClose.Get() * dRate;
							}
							break;
						case Feed::FID_CLOSE:  //0
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->ClosePrice = dClose.Get() * dRate;
								}
							}break;
						case Feed::FID_SHARES_PER_CONTRACT:	//0	???
							{
								Activ::UInt const * uiLotSize;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(uiLotSize))
									spResult->m_respParams->LotSize = (long)uiLotSize->Get();

							}break;
						case Feed::FID_NAME:	//106
							{
								Activ::TextString const * pText;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(pText))
									spResult->m_respParams->Description = ::SysAllocString(CA2W(pText->ToString().c_str()));
							}break;
						case Feed::FID_CURRENCY: //105
							{
								Activ::TextArray pText;
								IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->Currency = ::SysAllocString(CA2W(pText.ToString().c_str()));
								}
							}break;

						case Feed::FID_NEXT_DIVIDEND_1: //105
							{
								Activ::Rational const * pDividend;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(pDividend))
									spResult->m_respParams->DivAmount = pDividend->Get();
							}
							break;
						case Feed::FID_NEXT_DIVIDEND_1_DATE: //105
							{
								Activ::Date pDate;
								IFieldType* pUpdateField = static_cast<Activ::Date*>(&pDate);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									vt_date dtDiv((WORD)pDate.GetYear(), (WORD)pDate.GetMonth(), (WORD)pDate.GetDay());
									spResult->m_respParams->DivLastDate = dtDiv;
								}
							}
							break;
						case Feed::FID_NEXT_DIVIDEND_1_STATUS:
							{
								Activ::UInt const * uiStus;
								if ( STATUS_CODE_SUCCESS == field.GetActivFieldType( uiStus ) )
									if ( uiStus->Get() ) 
										bUseNextDiv = true;
							}
							break;
						case Feed::FID_DIVIDEND_FREQUENCY_CODE: //105
							{
								Activ::UInt uiFreq;
								IFieldType* pUpdateField = static_cast<Activ::UInt*>(&uiFreq);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									//spResult->m_respParams->DivFreq = (long)uiFreq.Get();
									switch( (long)uiFreq.Get() ) {
						case Feed::DIVIDEND_FREQUENCY_CODE_ANNUALLY/*	Annual*/ :
							spResult->m_respParams->DivFreq = 1;
							break;
						case Feed::DIVIDEND_FREQUENCY_CODE_MONTHLY	/*Monthly*/:
							spResult->m_respParams->DivFreq = 12;
							break;
						case Feed::DIVIDEND_FREQUENCY_CODE_QUARTERLY	/*Quarterly*/:
							spResult->m_respParams->DivFreq = 4;
							break;
						case Feed::DIVIDEND_FREQUENCY_CODE_SEMI_ANNUALLY  /*SemiAnnual*/:
							spResult->m_respParams->DivFreq = 2;
							break;
						case Feed::DIVIDEND_FREQUENCY_CODE_OTHER  /*Other*/:
						default:
							spResult->m_respParams->DivFreq = 0;
									}
								}
							}
							break;
						case Feed::FID_BID:
							{
								Activ :: TRational dClose;
								IFieldType* pUpdateField = static_cast<Activ::TRational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->Bid = dClose.Get() * dRate;
							}
							break;
						case Feed::FID_ASK:
							{
								Activ :: TRational dClose;
								IFieldType* pUpdateField = static_cast<Activ::TRational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->Ask = dClose.Get() * dRate;
							}break;
						case Feed::FID_OPEN:
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->OpenPrice = dClose.Get() * dRate;
							}break;
						case Feed::FID_CUMULATIVE_VOLUME:
							{
								Activ::UInt const * uiLotSize;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(uiLotSize)){
									spResult->m_respParams->CompositeVolume = spResult->m_respParams->Volume = (long)uiLotSize->Get();
									spResult->m_respParams->CurrentVolume = (long)uiLotSize->Get();
								}
							}break;
						case Feed::FID_TRADE_HIGH:
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->High = dClose.Get() * dRate;
							}break;
						case Feed::FID_TRADE_LOW:
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->Low = dClose.Get() * dRate;
							}break;
						case Feed::FID_TRADE_SIZE:
							{
								Activ::UInt const * uiLotSize;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(uiLotSize)){
									spResult->m_respParams->LastTradeVolume = (long)uiLotSize->Get();
								}
							}break;
						}
					}
				}
			}
			long lFreq = spResult->m_respParams->DivFreq ;

			/*vt_date_span dtSp ( vt_date::GetCurrentDate() - pNextDivDate ) ;
			long lDist = abs(dtSp.get_total_days()) ;
			switch (lFreq)
			{
			case 1:
			if ( lDist < 364 )
			bUseNextDiv = true ;
			break ;
			case 12:
			if ( lDist < 30 )
			bUseNextDiv = true ;
			break ;
			case 2:
			if ( lDist < 182 )
			bUseNextDiv = true ;
			break ;
			case 4:
			if ( lDist < 91 )
			bUseNextDiv = true ;
			break ;
			}*/

			if(spResult->m_respParams->StockType == enIndex)
				RequestIndex(spRequest, spResult);
			else{
				pApplication->OnStock(spRequest, spResult);
			}
		}
	}
}


void CActivFeedContentGateway::OnFuture(CRequest<FutureParams>::CRequestPtr& spRequest, HeapMessage &response)
{ 
	CActivFeedAppication* pApplication = dynamic_cast<CActivFeedAppication*>(&GetApplication());
	if(pApplication)
	{
		GetMultiplePatternMatch::ResponseParameters responseParameters;
		//Use the helper class to 'crack' the response message into
		//typed responseParameters
		StatusCode statusCode = GetMultiplePatternMatch::Deserialize(*this, response,
			responseParameters);

		if (STATUS_CODE_SUCCESS != statusCode)
			return;

		CResponse<FutureResultsEx>::CResponsePtr spResult(new CResponse<FutureResultsEx>());
		if(spResult!=NULL)
		{
			ResponseBlockList &rbl =
				responseParameters.m_responseBlockList;
			ResponseBlockList::const_iterator it;
			bool bDone = false;
			for(it = rbl.begin(); it != rbl.end(); ++it)
			{
				const ResponseBlock &rb = *it;
				//We only have data if the status for this
				//block is good or stale so check here.
				if (!rb.IsValidResponse())
					continue;
				//Check that this block refers to our partial request
				FieldListValidator  validator;
				StatusCode sCode = validator.Initialize(rb.m_fieldData);
				if(STATUS_CODE_SUCCESS == sCode)
				{
					spResult->m_respParams->OptionContractSize = 1;

					for (FieldListValidator::ConstIterator FieldListValidatorIterator = validator.Begin(), FieldListValidatorIteratorEnd = validator.End(); FieldListValidatorIteratorEnd != FieldListValidatorIterator; ++FieldListValidatorIterator)
					{
						const FieldListValidator::Field & field = FieldListValidatorIterator.GetField();
						if (0 != (field.m_rules & FieldListValidator::Field::RULE_DOES_NOT_UPDATE_LAST_VALUE))
							continue;

						switch(field.m_fieldId)
						{
						case Feed::FID_CLOSE:  //0
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->ClosePrice = dClose.Get();
								}
							}break;
						case Feed::FID_SYMBOL:	//106
							{
								Activ::TextString const * pText;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(pText)){
									std::string s = pText->ToString();
									s.resize(s.find_last_of('.'));//s.resize(s.length()-4);
									spResult->m_respParams->Symbol = (_bstr_t(s.c_str())).copy();//::SysAllocString(CA2W(pText->ToString().c_str()));
								}
							}break;
						case Feed::FID_LAST_TRADING_DATE: //300
							{
								Activ::Date pDate;
								IFieldType* pUpdateField = static_cast<Activ::Date*>(&pDate);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									vt_date dtDiv((WORD)pDate.GetYear(), (WORD)pDate.GetMonth(), (WORD)pDate.GetDay());
									//spResult->m_respParams->ExpirationDate = dtDiv + 1;

									spResult->m_respParams->LastTradingDate = dtDiv;
								}
							}
							break;
						case Feed::FID_EXPIRATION_DATE: //280
							{
								Activ::Date pDate;
								IFieldType* pUpdateField = static_cast<Activ::Date*>(&pDate);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									vt_date dtDiv((WORD)pDate.GetYear(), (WORD)pDate.GetMonth(), (WORD)pDate.GetDay());
									spResult->m_respParams->ExpirationDate = dtDiv;
								}
							}
							break;
						case Feed::FID_CONTRACT_SIZE:	//0	???
							{
								bDone = true;
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->ContractSize = (long)dClose.Get();
								}

							}break;
						case Feed::FID_MINIMUM_TICK:  
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->MinimumTick = dClose.Get();
								}
							}break;
						case Feed::FID_UNIT_OF_MEASURE:  
							{
								Activ::UInt unitofmeasure;
								IFieldType* pUpdateField = static_cast<Activ::UInt*>(&unitofmeasure);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->UnitOfMeasure = (UnitOfMeasureEnum)unitofmeasure.Get(); 
								}
							}break;
						case Feed::FID_UNIT_OF_MEASURE_CURRENCY:  
							{	
								Activ::TextArray pText;
								IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){
									std::string s = pText.ToString();
									spResult->m_respParams->UnitOfMeasureCurrency = (_bstr_t(s.c_str())).copy();
								}
							}break;
						case Feed::FID_NAME:
							{
								Activ::TextString const * pText;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(pText))
									spResult->m_respParams->Description = ::SysAllocString(CA2W(pText->ToString().c_str()));

								//Activ::TextArray pText;
								//IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
								//if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){
								//	std::string s = pText.ToString();
								//	spResult->m_respParams->Description = (_bstr_t(s.c_str())).copy();
								//}
							}
							break;
						case Feed::FID_BID:  
							{
								Activ :: TRational dBid;
								IFieldType* pUpdateField = static_cast<Activ::TRational*>(&dBid);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->Bid = dBid.Get();
								}
							}break;
						case Feed::FID_ASK:  
							{
								Activ :: TRational dAsk;
								IFieldType* pUpdateField = static_cast<Activ::TRational*>(&dAsk);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->Ask = dAsk.Get();
								}
							}break;
						case Feed::FID_TRADE:  
							{
								Activ :: TRational dTrade;
								IFieldType* pUpdateField = static_cast<Activ::TRational*>(&dTrade);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->Trade = dTrade.Get();
								}
							}break;
						}
					}
				}
			}
			if(bDone)
			{
				_bstr_t extDescription;
				extDescription = spResult->m_respParams->Description;
				extDescription += L" "; 
				if(spResult->m_respParams->Symbol)
					extDescription += _bstr_t( FutureExpiration((LPCSTR)_bstr_t(spResult->m_respParams->Symbol)).c_str() );

				spResult->m_respParams->Description = extDescription.copy();
				pApplication->OnFuture(spRequest, spResult);
			}
		}
	}
}

void CActivFeedContentGateway::OnFutureByRoot(CRequest<FutureRootParams>::CRequestPtr& spRequest, HeapMessage &response)
{
	GetMultiplePatternMatch::ResponseParameters responseParameters;
	StatusCode statusCode = GetMultiplePatternMatch::Deserialize(*this, response, responseParameters);

	if (STATUS_CODE_SUCCESS != statusCode)
		return;
	int i = 0;
	ResponseBlockList::const_iterator	it;
	ResponseBlockList&					rbl			= responseParameters.m_responseBlockList;

	for(it = rbl.begin(), i=0; it != rbl.end(); ++it, ++i)
	{
		const ResponseBlock &rb = *it;
		if (!rb.IsValidResponse()) 
		{
			EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("Invalid response - %s, Request key - %s"), ResponseBlock::StatusToString( rb.m_status ).c_str(), rb.m_requestKey.m_symbol.c_str() );
			continue;
		}


		string sSymbol = rb.m_requestKey.m_symbol;
		size_t pos = sSymbol.find_last_of('.');
		if( pos == sSymbol.npos)
			continue;

		sSymbol.resize(pos);

		//Check that this block refers to our partial request
		FieldListValidator  validator;
		StatusCode sCode = validator.Initialize(rb.m_fieldData);

		if(STATUS_CODE_SUCCESS == sCode)
		{
			//spResult->m_respParams->OptionContractSize = 1;
			CResponse<FutureResultsEx>::CResponsePtr spFut;
			if(m_futMap.find(sSymbol)!= m_futMap.end())
				spFut = m_futMap.find(sSymbol)->second; 
			else
			{
				spFut = CResponse<FutureResultsEx>::CResponsePtr(new CResponse<FutureResultsEx>());
				m_futMap.insert(m_futMap.end(), std::make_pair(sSymbol, spFut));
				spFut->m_respParams->Symbol				= SysAllocString(CA2W(sSymbol.c_str()));
				spFut->m_respParams->OptionContractSize = 1;
			}

			for (FieldListValidator::ConstIterator FieldListValidatorIterator = validator.Begin(), FieldListValidatorIteratorEnd = validator.End(); FieldListValidatorIteratorEnd != FieldListValidatorIterator; ++FieldListValidatorIterator)
			{
				const FieldListValidator::Field & field = FieldListValidatorIterator.GetField();
				if (0 != (field.m_rules & FieldListValidator::Field::RULE_DOES_NOT_UPDATE_LAST_VALUE))
					continue;

				switch(field.m_fieldId)
				{
				case Feed::FID_SYMBOL:	//106
					// 						{
					// 							Activ::TextString* pText;
					// 							if (futmap[sSymbol].Symbol==NULL && STATUS_CODE_SUCCESS == field.GetActivFieldType(pText)){
					// 								/*std::string s = pText->ToString();
					// 								s.resize(s.find_last_of('.'));*/
					// 								sLastSymbol = sSymbol;//s;
					// 								futmap[sSymbol].Symbol = _bstr_t(sSymbol.data()).copy();//(_bstr_t(s.c_str())).copy();
					// 								//spResult->m_respParams->Symbol = (_bstr_t(s.c_str())).copy();//::SysAllocString(CA2W(pText->ToString().c_str()));
					// 							}
					// 						}break;
					break;
				case Feed::FID_EXPIRATION_DATE: //105
					{
						Activ::Date pDate;
						IFieldType* pUpdateField = static_cast<Activ::Date*>(&pDate);
						if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
							vt_date dtExpDate((WORD)pDate.GetYear(), (WORD)pDate.GetMonth(), (WORD)pDate.GetDay());
							spFut->m_respParams->ExpirationDate = dtExpDate;
						}
					}
					break;
				case Feed::FID_LAST_TRADING_DATE: //300
					{
						Activ::Date pDate;
						IFieldType* pUpdateField = static_cast<Activ::Date*>(&pDate);
						if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
							vt_date dtLastTradingDate((WORD)pDate.GetYear(), (WORD)pDate.GetMonth(), (WORD)pDate.GetDay());
							spFut->m_respParams->LastTradingDate = dtLastTradingDate;
						}
					}
					break;
				case Feed::FID_SETTLEMENT:  //0
					{
						Activ :: Rational dClose;
						IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
						if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
							spFut->m_respParams->ClosePrice = dClose.Get();
						//spResult->m_respParams->ClosePrice = dClose.Get();
					}break;
				case Feed::FID_CONTRACT_SIZE:	//0	???
					{
						Activ :: Rational dClose;
						IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
						if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
							spFut->m_respParams->ContractSize = (long)dClose.Get();
						}

					}break;
				case Feed::FID_MINIMUM_TICK:  
					{
						Activ :: Rational dClose;
						IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
						if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
							spFut->m_respParams->MinimumTick = dClose.Get();
						}
					}break;
				case Feed::FID_UNIT_OF_MEASURE:  
					{
						Activ::UInt unitofmeasure;
						IFieldType* pUpdateField = static_cast<Activ::UInt*>(&unitofmeasure);
						if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
							spFut->m_respParams->UnitOfMeasure = (UnitOfMeasureEnum)unitofmeasure.Get(); 
						}
					}break;
				case Feed::FID_UNIT_OF_MEASURE_CURRENCY:  
					{	
						Activ::TextArray pText;
						IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
						if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){
							std::string s = pText.ToString();
							spFut->m_respParams->UnitOfMeasureCurrency = (_bstr_t(s.c_str())).copy();
						}
					}break; 
				case Feed::FID_NAME:
					{
						Activ::TextString const * pText;
						if (STATUS_CODE_SUCCESS == field.GetActivFieldType(pText))
							spFut->m_respParams->Description = ::SysAllocString(CA2W(pText->ToString().c_str()));
					}
					break;
				case Feed::FID_BID:  
					{
						Activ :: TRational dBid;
						IFieldType* pUpdateField = static_cast<Activ::TRational*>(&dBid);
						if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
							spFut->m_respParams->Bid = dBid.Get();
						}
					}break;
				case Feed::FID_ASK:  
					{
						Activ :: TRational dAsk;
						IFieldType* pUpdateField = static_cast<Activ::TRational*>(&dAsk);
						if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
							spFut->m_respParams->Ask = dAsk.Get();
						}
					}break;
				case Feed::FID_TRADE:  
					{
						Activ :: TRational dTrade;
						IFieldType* pUpdateField = static_cast<Activ::TRational*>(&dTrade);
						if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
							spFut->m_respParams->Trade = dTrade.Get();
						}
					}break;
				}
			}
			//if(bNeeded)
		}
	}
}

void CActivFeedContentGateway::OnFutureByRoot(CRequest<FutureRootParams>::CRequestPtr& spRequest, HeapMessage &response, bool bIsLast)
{
	CActivFeedAppication* pApplication = dynamic_cast<CActivFeedAppication*>(&GetApplication());

	if(pApplication) {

		GetMultiplePatternMatch::ResponseParameters responseParameters;
		StatusCode statusCode = GetMultiplePatternMatch::Deserialize(*this, response, responseParameters);

		if (STATUS_CODE_SUCCESS != statusCode)
			return;

		CResponse<FutureResultsEx>::CResponsePtr spResult(new CResponse<FutureResultsEx>());
		std::map<string, FutureResultsEx> futmap;

		if(spResult!=NULL)
		{
			std::string							sLastSymbol;
			ResponseBlockList::const_iterator	it;
			ResponseBlockList&					rbl				= responseParameters.m_responseBlockList;
			bool								bWasSize		= false;

			int i = 0;
			//std::map<std::string, long > BadFuturesMap;
			for(it = rbl.begin(), i=0; it != rbl.end(); ++it, ++i)
			{
				const ResponseBlock &rb = *it;
				if (!rb.IsValidResponse()) {
					EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , 
						"Invalid response - %s, Request key - %s", 
						ResponseBlock::StatusToString( rb.m_status ).c_str(), 
						rb.m_requestKey.m_symbol.c_str() );
					//BadFuturesMap[ rb.m_requestKey.m_symbol ] = 1;
					continue;
				}

				//if ( BadFuturesMap.find( rb.m_requestKey.m_symbol) != BadFuturesMap.end() ) 
				//	continue;

				string sSymbol = rb.m_requestKey.m_symbol;
				size_t pos = sSymbol.find_last_of('.');
				if( pos == sSymbol.npos)
					continue;

				sSymbol.resize(pos);

				//Check that this block refers to our partial request
				FieldListValidator  validator;
				StatusCode sCode = validator.Initialize(rb.m_fieldData);

				if(STATUS_CODE_SUCCESS == sCode)
				{
					//spResult->m_respParams->OptionContractSize = 1;
					futmap[sSymbol].OptionContractSize = 1;

					for (FieldListValidator::ConstIterator FieldListValidatorIterator = validator.Begin(), FieldListValidatorIteratorEnd = validator.End(); FieldListValidatorIteratorEnd != FieldListValidatorIterator; ++FieldListValidatorIterator)
					{
						const FieldListValidator::Field & field = FieldListValidatorIterator.GetField();
						if (0 != (field.m_rules & FieldListValidator::Field::RULE_DOES_NOT_UPDATE_LAST_VALUE))
							continue;

						switch(field.m_fieldId)
						{
						case Feed::FID_SYMBOL:	//106
							{
								Activ::TextString const * pText;
								if (futmap[sSymbol].Symbol==NULL && STATUS_CODE_SUCCESS == field.GetActivFieldType(pText)){
									/*std::string s = pText->ToString();
									s.resize(s.find_last_of('.'));*/
									sLastSymbol = sSymbol;//s;
									futmap[sSymbol].Symbol = _bstr_t(sSymbol.data()).copy();//(_bstr_t(s.c_str())).copy();
									//spResult->m_respParams->Symbol = (_bstr_t(s.c_str())).copy();//::SysAllocString(CA2W(pText->ToString().c_str()));
								}
							}break;
						case Feed::FID_LAST_TRADING_DATE: //300
							{
								Activ::Date pDate;
								IFieldType* pUpdateField = static_cast<Activ::Date*>(&pDate);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									vt_date dtLastTradingDate((WORD)pDate.GetYear(), (WORD)pDate.GetMonth(), (WORD)pDate.GetDay());
									spResult->m_respParams->LastTradingDate = dtLastTradingDate;
								}
							}
							break;
						case Feed::FID_EXPIRATION_DATE: //280
							{
								Activ::Date pDate;
								IFieldType* pUpdateField = static_cast<Activ::Date*>(&pDate);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									vt_date dtExpDate((WORD)pDate.GetYear(), (WORD)pDate.GetMonth(), (WORD)pDate.GetDay());
									spResult->m_respParams->ExpirationDate = dtExpDate;
								}
							}
							break;
						case Feed::FID_SETTLEMENT:  //0
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									futmap[sSymbol].ClosePrice = dClose.Get();
								//spResult->m_respParams->ClosePrice = dClose.Get();
							}break;
						case Feed::FID_CONTRACT_SIZE:	//0	???
							{
								bWasSize = true;
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									futmap[sSymbol].ContractSize = (long)dClose.Get();
									//spResult->m_respParams->ContractSize = (long)dClose.Get();
								}

							}break;
						case Feed::FID_MINIMUM_TICK:  
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->MinimumTick = dClose.Get();
								}
							}break;
						case Feed::FID_UNIT_OF_MEASURE:  
							{
								Activ::UInt unitofmeasure;
								IFieldType* pUpdateField = static_cast<Activ::UInt*>(&unitofmeasure);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->UnitOfMeasure = (UnitOfMeasureEnum)unitofmeasure.Get(); 
								}
							}break;
						case Feed::FID_UNIT_OF_MEASURE_CURRENCY:  
							{	
								Activ::TextArray pText;
								IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){
									std::string s = pText.ToString();
									spResult->m_respParams->UnitOfMeasureCurrency = (_bstr_t(s.c_str())).copy();
								}
							}break; 
						case Feed::FID_NAME:
							{
								Activ::TextString const * pText;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(pText))
									spResult->m_respParams->Description = ::SysAllocString(CA2W(pText->ToString().c_str()));
							}
							break;
						case Feed::FID_BID:  
							{
								Activ :: TRational dBid;
								IFieldType* pUpdateField = static_cast<Activ::TRational*>(&dBid);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->Bid = dBid.Get();
								}
							}break;
						case Feed::FID_ASK:  
							{
								Activ :: TRational dAsk;
								IFieldType* pUpdateField = static_cast<Activ::TRational*>(&dAsk);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->Ask = dAsk.Get();
								}
							}break;
						case Feed::FID_TRADE:  
							{
								Activ :: TRational dTrade;
								IFieldType* pUpdateField = static_cast<Activ::TRational*>(&dTrade);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->Trade = dTrade.Get();
								}
							}break;
						}
					}
					//if(bNeeded)
				}
			}
			// Here we going to answer to client. We'll send one additional response
			// due bug in EtsBO - it doesn't recognize last answer
			if ( futmap.size() ) {
				size_t i = 1, sz = futmap.size();
				std::map<std::string, FutureResultsEx>::iterator itr = futmap.begin(), itrend = futmap.end();
				CString monthYear;

				for (; itr != itrend; itr++, i++ ) {
					//spResult->m_respParams->Description = ::SysAllocString(itr->second.Description);//_bstr_t(" ").copy();
					_bstr_t extDescription, savedDescription;
					monthYear = _T("");
					extDescription = L"";
					savedDescription = spResult->m_respParams->Description;
					spResult->m_respParams->Symbol = itr->second.Symbol;
					spResult->m_respParams->ContractSize = itr->second.ContractSize;
					spResult->m_respParams->OptionContractSize = itr->second.OptionContractSize;
					spResult->m_respParams->ExpirationDate = itr->second.ExpirationDate;
					spResult->m_respParams->ClosePrice = itr->second.ClosePrice;

					vt_date	shortDate( spResult->m_respParams->ExpirationDate );
					monthYear = shortDate.Format("%b%y");
					extDescription = spResult->m_respParams->Description;
					extDescription += L" "; 
					monthYear.MakeUpper();
					extDescription += _bstr_t( monthYear );
					spResult->m_respParams->Description = extDescription.copy();

					pApplication->OnFutureByRoot(spRequest, spResult,  bIsLast&&i==sz );
					::SysFreeString(spResult->m_respParams->Description);
					spResult->m_respParams->Description = ::SysAllocString(savedDescription);
				}
				// 				if(bIsLast)
				// 				{
				// 					CResponse<FutureResultsEx>::CResponsePtr spLastResult(new CResponse<FutureResultsEx>());
				// 					spLastResult->m_respParams->Description = _bstr_t(" ").copy();
				// 					spLastResult->m_respParams->Symbol = _bstr_t(" ").copy();;
				// 					spLastResult->m_respParams->ContractSize = 0;
				// 					spLastResult->m_respParams->OptionContractSize = 0;
				// 					spLastResult->m_respParams->ClosePrice = 0;
				// 					pApplication->OnFutureByRoot(spRequest, spLastResult,  bIsLast );	// one more time
				// 				}
			}
			else{
				CComVariant vtRequest;
				spRequest->m_recParams.CopyTo(vtRequest);
				pApplication->OnSPError(enNoDataAvailableForSymbol, "No data received", enRequestFutures, vtRequest);
			}
		}
	}
}

void CActivFeedContentGateway::OnIndex(CRequest<StockParams>::CRequestPtr& spRequest, HeapMessage &response)
{ 
	CActivFeedAppication* pApplication = dynamic_cast<CActivFeedAppication*>(&GetApplication());
	if(pApplication)
	{
		GetMultiplePatternMatch::ResponseParameters responseParameters;
		//Use the helper class to 'crack' the response message into
		//typed responseParameters
		StatusCode statusCode = GetMultiplePatternMatch::Deserialize(*this, response,
			responseParameters);

		if (STATUS_CODE_SUCCESS != statusCode)
			return;

		CResponse<StockResultsEx>::CResponsePtr spResult(new CResponse<StockResultsEx>());
		if(spResult!=NULL)
		{
			ResponseBlockList &rbl =
				responseParameters.m_responseBlockList;
			ResponseBlockList::const_iterator it;
			bool bUpdated = false;
			for(it = rbl.begin(); it != rbl.end(); ++it)
			{
				spResult->m_respParams.Clear();
				//bUpdated = false;
				const ResponseBlock &rb = *it;
				//We only have data if the status for this
				//block is good or stale so check here.
				if (!rb.IsValidResponse())
					continue;
				//Check that this block refers to our partial request
				FieldListValidator  validator;
				StatusCode sCode = validator.Initialize(rb.m_fieldData);
				if(STATUS_CODE_SUCCESS == sCode)
				{
					//spResult->m_respParams->LotSize   = 100;
					spResult->m_respParams->StockType = enIndex;

					for (FieldListValidator::ConstIterator FieldListValidatorIterator = validator.Begin(), FieldListValidatorIteratorEnd = validator.End(); FieldListValidatorIteratorEnd != FieldListValidatorIterator; ++FieldListValidatorIterator)
					{
						const FieldListValidator::Field & field = FieldListValidatorIterator.GetField();
						if (0 != (field.m_rules & FieldListValidator::Field::RULE_DOES_NOT_UPDATE_LAST_VALUE))
							continue;

						switch(field.m_fieldId)
						{
						case Feed::FID_PRIMARY_EXCHANGE:  //105
							{
								Activ::TextString pText;
								IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									std::string s = pText.ToString();
									Activ2Egar( s, enSTK );
									//spResult->m_respParams->Exchange = ::SysAllocString(CA2W(pText.ToString().c_str()));
									spResult->m_respParams->Exchange = ::SysAllocString( CA2W( s.c_str() ) );
									bUpdated = true;

								}
							}break;
						case Feed::FID_PREVIOUS_ASK:
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->CloseAsk = dClose.Get();

							}break;
						case Feed::FID_PREVIOUS_BID:
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->CloseBid = dClose.Get();
							}
							break;
						case Feed::FID_CLOSE:  //0
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->ClosePrice = dClose.Get();
									bUpdated = true;
								}
							}break;
						case Feed::FID_SHARES_PER_CONTRACT:	//0	???
							{
								Activ::UInt const * uiLotSize;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(uiLotSize)){
									spResult->m_respParams->LotSize = (long)uiLotSize->Get();
									bUpdated = true;
								}

							}break;
						case Feed::FID_NAME:	//106
							{
								Activ::TextString const * pText;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(pText)){
									spResult->m_respParams->Description = ::SysAllocString(CA2W(pText->ToString().c_str()));
									bUpdated = true;
								}
							}break;
						case Feed::FID_CURRENCY: //105
							{
								Activ::TextArray pText;
								IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->Currency = ::SysAllocString(CA2W(pText.ToString().c_str()));
									bUpdated = true;
								}
							}break;
							//case Feed::FID_DIVIDEND:
						case Feed::FID_NEXT_DIVIDEND_1: //105
							{
								Activ::Rational const * pDividend;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(pDividend)){
									spResult->m_respParams->DivAmount = pDividend->Get();
									bUpdated = true;
								}
							}
							break;
							//case Feed::FID_EX_DATE :
						case Feed::FID_NEXT_DIVIDEND_1_DATE: //105
							{
								Activ::Date pDate;
								IFieldType* pUpdateField = static_cast<Activ::Date*>(&pDate);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									vt_date dtDiv((WORD)pDate.GetYear(), (WORD)pDate.GetMonth(), (WORD)pDate.GetDay());
									spResult->m_respParams->DivLastDate = dtDiv;
									bUpdated = true;
								}
							}
							break;
						case Feed::FID_DIVIDEND_FREQUENCY_CODE: //105
							{
								Activ::UInt uiFreq;
								IFieldType* pUpdateField = static_cast<Activ::UInt*>(&uiFreq);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									//spResult->m_respParams->DivFreq = (long)uiFreq.Get();
									switch( (long)uiFreq.Get() ) {
						case Feed::DIVIDEND_FREQUENCY_CODE_ANNUALLY/*	Annual*/ :
							spResult->m_respParams->DivFreq = 1;
							break;
						case Feed::DIVIDEND_FREQUENCY_CODE_MONTHLY	/*Monthly*/:
							spResult->m_respParams->DivFreq = 12;
							break;
						case Feed::DIVIDEND_FREQUENCY_CODE_QUARTERLY	/*Quarterly*/:
							spResult->m_respParams->DivFreq = 4;
							break;
						case Feed::DIVIDEND_FREQUENCY_CODE_SEMI_ANNUALLY  /*SemiAnnual*/:
							spResult->m_respParams->DivFreq = 2;
							break;
						case Feed::DIVIDEND_FREQUENCY_CODE_OTHER  /*Other*/:
						default:
							spResult->m_respParams->DivFreq = 0;
									}
									bUpdated = true;
								}
								/*
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
								spResult->m_respParams->DivFreq = (long)uiFreq.Get();

								}*/
							}
							break;
						case Feed::FID_BID:
							{
								Activ :: TRational dClose;
								IFieldType* pUpdateField = static_cast<Activ::TRational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->Bid = dClose.Get();
									bUpdated = true;
								}
							}
							break;
						case Feed::FID_ASK:
							{
								Activ :: TRational dClose;
								IFieldType* pUpdateField = static_cast<Activ::TRational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->Ask = dClose.Get();
									bUpdated = true;
								}
							}break;
						case Feed::FID_OPEN:
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->OpenPrice = dClose.Get();
									bUpdated = true;
								}
							}break;
						case Feed::FID_CUMULATIVE_VOLUME:
							{
								Activ::UInt const * uiLotSize;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(uiLotSize)){
									spResult->m_respParams->CompositeVolume = spResult->m_respParams->Volume = (long)uiLotSize->Get();
									spResult->m_respParams->CurrentVolume = (long)uiLotSize->Get();
									bUpdated = true;
								}
							}break;
						case Feed::FID_TRADE_HIGH:
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->High = dClose.Get();
									bUpdated = true;
								}
							}break;
						case Feed::FID_TRADE_LOW:
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									spResult->m_respParams->Low = dClose.Get();
									bUpdated = true;
								}
							}break;
						case Feed::FID_TRADE_SIZE:
							{
								Activ::UInt const * uiLotSize;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(uiLotSize)){
									spResult->m_respParams->LastTradeVolume = (long)uiLotSize->Get();
									bUpdated = true;
								}
							}break;
						}
					}
				}
			}
			if(bUpdated)
				pApplication->OnStock(spRequest, spResult);
			else{
				CComVariant vtRequest;
				spRequest->m_recParams.CopyTo(vtRequest);
				pApplication->OnSPError(enNoDataAvailableForSymbol, "No data received", enRequestStock, vtRequest);
			}
		}
	}
}

void CActivFeedContentGateway::OnOption(CRequest<OptionParams>::CRequestPtr& spRequest, HeapMessage &response, bool bIsLast)
{ 

	CActivFeedAppication* pApplication = dynamic_cast<CActivFeedAppication*>(&GetApplication());
	if(pApplication)
	{
		GetMultiplePatternMatchList::ResponseParameters responseParameters;
		//Use the helper class to 'crack' the response message into
		//typed responseParameters
		StatusCode statusCode = GetMultiplePatternMatchList::Deserialize(*this, response,
			responseParameters);

		if (STATUS_CODE_SUCCESS != statusCode)
			return;

		bool bIsCompleteResponse = IsCompleteResponse( response );

		CResponse<OptionResultsEx>::CResponsePtr spResult(new CResponse<OptionResultsEx>());
		if(spResult!=NULL)
		{
			ResponseBlockList &rbl =
				responseParameters.m_responseBlockList;
			ResponseBlockList::const_iterator it;
			int i = 0;
			for(it = rbl.begin(); it != rbl.end(); ++it)
			{
				i++;
				const ResponseBlock &rb = *it;
				//We only have data if the status for this
				//block is good or stale so check here.
				if (!rb.IsValidResponse())
					continue;

				//Check that this block refers to our partial request
				FieldListValidator  validator;
				StatusCode sCode = validator.Initialize(rb.m_fieldData);
				if(STATUS_CODE_SUCCESS == sCode)
				{
					spResult->m_respParams->ClosePrice = -1;
					spResult->m_respParams->CloseBid = BAD_DOUBLE_VALUE;
					spResult->m_respParams->CloseAsk = BAD_DOUBLE_VALUE;

					for (FieldListValidator::ConstIterator FieldListValidatorIterator = validator.Begin(), FieldListValidatorIteratorEnd = validator.End(); FieldListValidatorIteratorEnd != FieldListValidatorIterator; ++FieldListValidatorIterator)
					{

						const FieldListValidator::Field & field = FieldListValidatorIterator.GetField();
						if (0 != (field.m_rules & FieldListValidator::Field::RULE_DOES_NOT_UPDATE_LAST_VALUE))
							continue;
						if( field.m_fieldStatus != FIELD_STATUS_DEFINED )
							continue;
						switch(field.m_fieldId)
						{
						case Feed::FID_PREVIOUS_ASK:
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->CloseAsk = dClose.Get();

							}break;
						case Feed::FID_PREVIOUS_BID:
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->CloseBid = dClose.Get();
							}
							break;
						case Feed::FID_STRIKE_PRICE:  //0
							{
								Activ :: Rational m_strikePrice;
								IFieldType* pUpdateField = /*static_cast<Activ::Rational*>*/(&m_strikePrice);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->StrikePrice = m_strikePrice.Get();
							}break;
						case Feed::FID_CLOSE:  //0
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->ClosePrice = dClose.Get();
							}break;
						case Feed::FID_SYMBOL:	//106
							{
								Activ::TextString const * pText;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(pText)){
									spResult->m_respParams->Series = ::SysAllocString(CA2W(pText->ToString().c_str()));
								}
							}break;
						case Feed::FID_EXPIRATION_DATE:
							{
								Activ::Date pDate;
								IFieldType* pUpdateField = static_cast<Activ::Date*>(&pDate);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									vt_date dtDiv((WORD)pDate.GetYear(), (WORD)pDate.GetMonth(), (WORD)pDate.GetDay());
									spResult->m_respParams->ExpirationDate = dtDiv;
								}
							}
							break;
						case Feed::FID_OPTION_TYPE: //105
							{
								Activ::TextArray pText;
								IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									const char ch= *pText.ToString().c_str();
									if(ch == 'c' || ch == 'C')
										spResult->m_respParams->OptionType = enCall;
									else
										spResult->m_respParams->OptionType = enPut;
								}
							}
							break;
						case Feed::FID_BID:
							{
								Activ :: TRational dClose;
								IFieldType* pUpdateField = static_cast<Activ::TRational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->Bid = dClose.Get();
							}
							break;
						case Feed::FID_ASK:
							{
								Activ :: TRational dClose;
								IFieldType* pUpdateField = static_cast<Activ::TRational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->Ask = dClose.Get();
							}break;
						case Feed::FID_TRADE:
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->Last = dClose.Get();
							}break;
						case Feed::FID_CUMULATIVE_VOLUME:
							{
								Activ::UInt const * uiLotSize;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(uiLotSize))
									spResult->m_respParams->Volume = (long)uiLotSize->Get();
							}break;
						case Feed::FID_CLOSE_CUMULATIVE_VOLUME:
							{
								Activ::UInt const * uiLotSize;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(uiLotSize))
									spResult->m_respParams->YVolume = (long)uiLotSize->Get();
							}break;
						case Feed::FID_OPEN_INTEREST:
							{
								Activ::UInt const * uiLotSize;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(uiLotSize))
									spResult->m_respParams->OpenInterest = (long)uiLotSize->Get();
							}break;
						}
					}
					if(spResult->m_respParams->ClosePrice == -1)
						spResult->m_respParams->ClosePrice = BAD_DOUBLE_VALUE;
					else if( spResult->m_respParams->ClosePrice < 0 )
						spResult->m_respParams->ClosePrice *=(-1);

					std::string symbolPattern;
					std::string a = (_bstr_t)(spResult->m_respParams->Series);
					if (GetOptionRootFromOption(a, symbolPattern)){
						COptionRootInfoMap::const_iterator ori = m_optionRoots.find(symbolPattern);
						spResult->m_respParams->LotSize = ori->second.lotsize;
						spResult->m_respParams->Currency = (_bstr_t(ori->second.currency.c_str()).copy());

						spResult->m_respParams->SettlementType = static_cast<SettlementTypeEnum>(ori->second.SettlementType);
						spResult->m_respParams->ExpirationType = ori->second.expirationType;

						switch(ori->second.expirationType)
						{
						case 1:
						case 2:
						case 4:
						case 5:
							{
								/*if(spResult->m_respParams->ExpirationDate > 0 )
									spResult->m_respParams->ExpirationDate++;*/

							}break;
						case 7:
							{
								/*if(spResult->m_respParams->ExpirationDate > 0)
									spResult->m_respParams->ExpirationDate++;*/
							}break;

						case 0:
						default:
							{
								/*if(spResult->m_respParams->ExpirationDate > 0 )
									spResult->m_respParams->ExpirationDate--;*/
							}break;
						}
					}
					std::string s;
					symbolPattern = CW2A(spResult->m_respParams->Series);
					GetOptionPattern(symbolPattern, s);
					spResult->m_respParams->Series = ::SysAllocString(CA2W(s.data()));

					spResult->m_respParams->Exchange = ::SysAllocString(L" ");

					bool bLast = false;
					if (bIsCompleteResponse && i == rbl.size())
						bLast = true;

					pApplication->OnOption(spRequest, spResult, bLast);
					EgLib::CEgLibTraceManager::Trace(LogInfo, 
						__FUNCTION__ , _T("Passing option %s %s"), rb.m_responseKey.m_symbol.c_str(), bLast?"Last":"");

				}
			}	
		}
	}
}

void CActivFeedContentGateway::OnFuturesOption(CRequest<FuturesOptionParams>::CRequestPtr& spRequest, HeapMessage &response, bool bIsLast)
{ 
	CActivFeedAppication* pApplication = dynamic_cast<CActivFeedAppication*>(&GetApplication());
	if(pApplication)
	{
		GetMultiplePatternMatchList::ResponseParameters responseParameters;
		//Use the helper class to 'crack' the response message into
		//typed responseParameters
		StatusCode statusCode = GetMultiplePatternMatchList::Deserialize(*this, response,
			responseParameters);

		if (STATUS_CODE_SUCCESS != statusCode)
			return;

		std::vector< CResponse<FuturesOptionResultsEx>::CResponsePtr > vctr;
		std::map<std::string, long > BadFutureOptionsMap;
		//CResponse<FuturesOptionResultsEx>::CResponsePtr spResult(new CResponse<FuturesOptionResultsEx>());
		//if(spResult!=NULL)
		{
			ResponseBlockList &rbl = 	responseParameters.m_responseBlockList;
			ResponseBlockList::const_iterator it;
			int i = 0;
			string sLastSerries;
			for(it = rbl.begin(); it != rbl.end(); ++it)
			{
				i++;
				const ResponseBlock &rb = *it;
				//We only have data if the status for this
				//block is good or stale so check here.
				if (!rb.IsValidResponse()){
					EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , 
						"Invalid response - %s, Request key - %s", 
						ResponseBlock::StatusToString( rb.m_status ).c_str(), 
						rb.m_requestKey.m_symbol.c_str() );
					BadFutureOptionsMap[ rb.m_requestKey.m_symbol ] = 1;
					continue;
				}

				if ( BadFutureOptionsMap.find( rb.m_requestKey.m_symbol) != BadFutureOptionsMap.end() ) 
					continue;

				//Check that this block refers to our partial request
				CResponse< FuturesOptionResultsEx >::CResponsePtr spResult( new CResponse< FuturesOptionResultsEx>() );
				FieldListValidator  validator;
				StatusCode sCode = validator.Initialize(rb.m_fieldData);
				if(STATUS_CODE_SUCCESS == sCode)
				{
					for (FieldListValidator::ConstIterator FieldListValidatorIterator = validator.Begin(), FieldListValidatorIteratorEnd = validator.End(); FieldListValidatorIteratorEnd != FieldListValidatorIterator; ++FieldListValidatorIterator)
					{

						const FieldListValidator::Field & field = FieldListValidatorIterator.GetField();
						if (0 != (field.m_rules & FieldListValidator::Field::RULE_DOES_NOT_UPDATE_LAST_VALUE))
							continue;

						switch(field.m_fieldId)
						{
						case Feed::FID_STRIKE_PRICE:  //0
							{
								Activ :: Rational m_strikePrice;
								IFieldType* pUpdateField = /*static_cast<Activ::Rational*>*/(&m_strikePrice);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->StrikePrice = m_strikePrice.Get();
							}break;
						case Feed::FID_SETTLEMENT:  //0
							{
								Activ :: Rational dClose;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dClose);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->ClosePrice = dClose.Get();
							}break;
						case Feed::FID_SYMBOL:	//106
							{
								Activ::TextString const * pText;
								if (STATUS_CODE_SUCCESS == field.GetActivFieldType(pText)){
									spResult->m_respParams->Series = ::SysAllocString(CA2W(pText->ToString().c_str()));
								}
							}break;
						case Feed::FID_LAST_TRADING_DATE: //300
							{
								Activ::Date pDate;
								IFieldType* pUpdateField = static_cast<Activ::Date*>(&pDate);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									vt_date dtLastTradingDate((WORD)pDate.GetYear(), (WORD)pDate.GetMonth(), (WORD)pDate.GetDay());

									spResult->m_respParams->LastTradingDate = dtLastTradingDate;
								}
							}
							break;
						case Feed::FID_EXPIRATION_DATE: //280
							{
								Activ::Date pDate;
								IFieldType* pUpdateField = static_cast<Activ::Date*>(&pDate);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									vt_date dtExpDate((WORD)pDate.GetYear(), (WORD)pDate.GetMonth(), (WORD)pDate.GetDay());

									spResult->m_respParams->ExpirationDate = dtExpDate;
								}
							}
							break;
						case Feed::FID_OPTION_TYPE: //105
							{
								Activ::TextArray pText;
								IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)){ // set new field values
									_bstr_t t = ::SysAllocString(CA2W(pText.ToString().c_str()));
									if(t == ((_bstr_t)"c") || t == ((_bstr_t)"C"))
										spResult->m_respParams->OptionType = enCall;
									else
										spResult->m_respParams->OptionType = enPut;
								}
							}
							break;
						case Feed::FID_BID:
							{
								Activ :: Rational dBid;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dBid);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->Bid = dBid.Get();
							}break;

						case Feed::FID_ASK:
							{
								Activ :: Rational dAsk;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dAsk);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->Ask = dAsk.Get();
							}break;
						case Feed::FID_PREVIOUS_BID:
							{
								Activ :: Rational dBid;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dBid);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->CloseBid = dBid.Get();
							}break;

						case Feed::FID_PREVIOUS_ASK:
							{
								Activ :: Rational dAsk;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dAsk);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->CloseAsk = dAsk.Get();
							}break;
						case Feed::FID_TRADE:
							{
								Activ :: Rational dLast;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&dLast);
								if (field.m_pIFieldType != NULL && STATUS_CODE_SUCCESS == pUpdateField->Assign(field.m_pIFieldType)) // set new field values
									spResult->m_respParams->Last = dLast.Get();
							}break;
						}
					}
				}
				std::string symbolPattern;
				//std::string s;
				symbolPattern = (_bstr_t)(spResult->m_respParams->Series);
				//symbolPattern.resize(symbolPattern.find_last_of('.'));//GetOptionPattern(symbolPattern, s);
				std::string::size_type ii = symbolPattern.find_last_of('.');
				if (std::string::npos == ii)
					continue;
				symbolPattern = symbolPattern.substr(0, ii);
				spResult->m_respParams->Series = ::SysAllocString((_bstr_t)symbolPattern.data());

				//bool bLast = false;
				//if (bIsLast && i == rbl.size())
				//	bLast = true;
				if(sLastSerries != symbolPattern /*|| bLast*/){
					ii = symbolPattern.find_last_of('/');
					if (std::string::npos != ii){
						string s = symbolPattern.substr(0,ii);
						spResult->m_respParams->LotSize = m_optionFutureRoots[s];
					}
					//pApplication->OnFuturesOption(spRequest, spResult, bLast);
					sLastSerries = symbolPattern;
					vctr.push_back( spResult );
				}
			}	
			bool bNotified = false;
			size_t lLastJ  = vctr.size() -1;
			for ( size_t j = 0; j < vctr.size(); j++ ) 
			{
				pApplication->OnFuturesOption(spRequest, vctr[j],  j + 1 == vctr.size() && (bIsLast && lLastJ == j)? true : false );
				bNotified = true;
			}
			vctr.clear();
			if (!bNotified && bIsLast ) {
				//CRequest<FuturesOptionParams>::CRequestPtr spOptRequest = boost::shared_dynamic_cast<CRequest<FuturesOptionParams> >(pRequest);
				CComVariant vtRequest;
				spRequest->m_recParams.CopyTo(vtRequest);
				pApplication->OnSPError(enNoDataAvailableForSymbol, "No data received", enRequestFuturesOptions, vtRequest);
			}
		}
	}
}


StatusCode CActivFeedContentGateway::OnConnect(HeapMessage &connectMessage)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ContentGatewayClient::OnConnect(connectMessage));

	if ( m_bReconnected ) {
		m_bReconnected = false;
		CActivFeedAppication* pApplication = dynamic_cast<CActivFeedAppication*>(&GetApplication());
		if(pApplication){
			_QuoteRequestPtr p;
			pApplication->OnBIError( enProviderConnected, "", enNoRequest, p);
		}
	}

	return STATUS_CODE_SUCCESS;
}

StatusCode CActivFeedContentGateway::OnConnectFailed(HeapMessage &connectFailMessage)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ContentGatewayClient::OnConnectFailed(connectFailMessage));

	// we've failed to asynchronously connect to the gateway. However, we don't need to take any
	// action as the middleware will lazily attempt to connect again for us. If we wanted to stop connecting, we
	// can call Disconnect().
	//Disconnect();
	return STATUS_CODE_SUCCESS;
}

//--------------------------------------------------------------------------------------------------------------------------

StatusCode CActivFeedContentGateway::OnDisconnect(HeapMessage &disconnectMessage)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ContentGatewayClient::OnDisconnect(disconnectMessage));

	EgLib::CEgLibTraceManager::TraceText(LogSystem, __FUNCTION__ , _T("Enter"));

	// clear all maps
	{
		CAutoLock l(m_csPSMap);
		m_PSMap.clear();
	}
	{
		CAutoLock l(m_csLU);
		m_LU.clear();
	}
	{
		CAutoLock l( m_csSMap );
		m_SMap.clear();
	}

	::SetEvent(m_hOnDisconnect);

	EgLib::CEgLibTraceManager::TraceText(LogSystem, __FUNCTION__ , _T("Exit"));

	return STATUS_CODE_SUCCESS;
}

//--------------------------------------------------------------------------------------------------------------------------

StatusCode CActivFeedContentGateway::OnBreak(HeapMessage &breakMessage)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ContentGatewayClient::OnBreak(breakMessage));

	EgLib::CEgLibTraceManager::TraceText(LogSystem, __FUNCTION__ , _T("Enter"));

	// clear all maps
	{
		CAutoLock l(m_csPSMap);
		m_PSMap.clear();
	}
	{
		CAutoLock l(m_csLU);
		m_LU.clear();
	}
	{
		CAutoLock l( m_csSMap );
		m_SMap.clear();
	}
	
	if(!m_bDisconnection)
	{
		m_bReconnected = true;
		
		CActivFeedAppication* pApplication = dynamic_cast<CActivFeedAppication*>(&GetApplication());
		if(pApplication){
			_QuoteRequestPtr p;
			pApplication->OnBIError( enConnectionWasDisconnected, "", enNoRequest, p);
		}
	}
	else
	{
		::SetEvent(m_hOnDisconnect);
	}

	EgLib::CEgLibTraceManager::TraceText(LogSystem, __FUNCTION__ , _T("Exit"));

	return STATUS_CODE_SUCCESS;
}

