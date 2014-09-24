#include "StdAfx.h"
#include "ActivFeedContentGateway.h"
#include "ActivFeedConnection.h"

//------------------------------------------------------------------------------------------------------//
UINT CActivFeedContentGateway::Run()
{
	while (true)
	{
		WaitForSingleObject(m_hOnBreakEvent, INFINITE);

		if (m_bUserDisconnect){
			break;
		}
		else{
			m_bConnected = false;
			while(true)
			{
				if (WAIT_TIMEOUT == WaitForSingleObject(m_hOnReconnectionEvent, 5000))
				{
					LOG4CPLUS_INFO(ProviderLog, _T("Reconnection started."));
					if (m_client) 
						m_client->Connect();
				}
				else
					break;
			}
			Resubscribe();
		}
	}
	return 0;
};
//------------------------------------------------------------------------------------------------------//
StatusCode CActivFeedContentGateway::OnBreak(HeapMessage &breakMessage)
{
	LOG4CPLUS_ERROR(ProviderLog, _T("Connection has broken."));

	SetEvent(m_hOnBreakEvent);

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ContentGatewayClient::OnBreak(breakMessage));

	return STATUS_CODE_SUCCESS;
}
//------------------------------------------------------------------------------------------------------//
StatusCode CActivFeedContentGateway::OnDisconnect(HeapMessage &disconnectMessage)
{
	LOG4CPLUS_INFO(ProviderLog, _T("Provider disconnected from gateway."));

	m_bConnected = false;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ContentGatewayClient::OnDisconnect(disconnectMessage));
	
	return STATUS_CODE_SUCCESS;
}
//------------------------------------------------------------------------------------------------------//
StatusCode CActivFeedContentGateway::OnConnectFailed(HeapMessage &connectFailMessage)
{
	StatusCode statusCode = connectFailMessage.GetStatusCode();
	
	LOG4CPLUS_ERROR(ProviderLog, "Connection failed. StatusCode=" << statusCode << " (" << StatusCodeToString(statusCode) << ")");

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ContentGatewayClient::OnConnectFailed(connectFailMessage));

	return STATUS_CODE_SUCCESS;
}
//------------------------------------------------------------------------------------------------------//
StatusCode CActivFeedContentGateway::OnConnect(HeapMessage &connectMessage)
{
	LOG4CPLUS_INFO(ProviderLog, _T("Connected."));

	SetEvent(m_hOnReconnectionEvent);

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ContentGatewayClient::OnConnect(connectMessage));
	
	return STATUS_CODE_SUCCESS;	
}
//------------------------------------------------------------------------------------------------------//
StatusCode CActivFeedContentGateway::ParseResponseBlock(const ResponseBlock& rb, QuoteInfo* pQuoteInfo, bool bRealTime, QuoteRequest* pRequest)
{
	if (rb.IsValidResponse())
	{
		FieldListValidator	fieldListValidator;

		if( STATUS_CODE_SUCCESS == fieldListValidator.Initialize( rb.m_fieldData ) )
		{
			FieldListValidator::ConstIterator fit, fitEnd;

			for ( fit = fieldListValidator.Begin(), fitEnd = fieldListValidator.End(); fit != fitEnd; fit++ ) 
			{
				const FieldListValidator::Field& fld = fit.GetField();
				double dBid = 0;
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
					case Feed::FID_UNDERLYING_SYMBOL:
						{
							const Activ::TextString* pText;
							if (STATUS_CODE_SUCCESS == fld.GetActivFieldType(pText))
							{
								pQuoteInfo->BaseContract = pText->ToString();
								pQuoteInfo->BaseContract = pQuoteInfo->BaseContract.substr(0, pQuoteInfo->BaseContract.find('.'));
							};
						}
						break;
					case Feed::FID_PREVIOUS_BID:
						{
							if( ( pQuoteInfo->BidPrice == BAD_DOUBLE_VALUE || !pQuoteInfo->BidPrice ) && (!bRealTime || pRequest->Type == enOPT) ) 
							{
								Activ::Rational bid;
								IFieldType* pUpdateField = static_cast<Activ::Rational*>(&bid);
								(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
								pQuoteInfo->BidPrice = bid.Get();
							}
						}
						break;
					case Feed::FID_BID:
						{
							Activ::TRational bid;
							IFieldType* pUpdateField = static_cast<Activ::TRational*>(&bid);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->BidPrice = bid.Get();
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
								pQuoteInfo->AskPrice = ask.Get();
							}
						}
						break;
					case Feed::FID_ASK:
						{
							Activ::TRational ask;
							IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->AskPrice = ask.Get();
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
							pQuoteInfo->OpenPrice = opn.Get();
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
					case Feed::FID_BID_TIME:
						{
							Activ::Time dt;
							EgLib::vt_date	pdate( EgLib::vt_date::GetCurrentDate(true) );
							IFieldType* pUpdateField = static_cast<Activ::Time*>(&dt);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							int hour = dt.GetHour();
							int minute = dt.GetMinute();
							int second = dt.GetSecond();
							EgLib::vt_date d( pdate.get_year(), pdate.get_month(), pdate.get_day(), hour, minute, second );
							pQuoteInfo->BidDateTime = d;
						}
						break;
					case Feed::FID_ASK_TIME:
						{
							Activ::Time dt;
							EgLib::vt_date	pdate( EgLib::vt_date::GetCurrentDate(true) );
							IFieldType* pUpdateField = static_cast<Activ::Time*>(&dt);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							int hour = dt.GetHour();
							int minute = dt.GetMinute();
							int second = dt.GetSecond();
							EgLib::vt_date d( pdate.get_year(), pdate.get_month(), pdate.get_day(), hour, minute, second );
							pQuoteInfo->AskDateTime = d;
						}
						break;
					case Feed::FID_TRADE_DATE:
						{
							Activ::Date dt;
							EgLib::vt_date	pdate( pQuoteInfo->LastDateTime );
							IFieldType* pUpdateField = static_cast<Activ::Date*>(&dt);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							int year = dt.GetYear();
							int month = dt.GetMonth();
							int day = dt.GetDay();
							EgLib::vt_date d( year, month, day, pdate.get_hour(), pdate.get_minute(), pdate.get_second() );
							pQuoteInfo->LastDateTime = d;
						}
						break;
					case Feed::FID_TRADE_TIME:
						{
							Activ::Time dt;
							EgLib::vt_date	pdate( pQuoteInfo->LastDateTime );
							IFieldType* pUpdateField = static_cast<Activ::Time*>(&dt);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							int hour = dt.GetHour();
							int minute = dt.GetMinute();
							int second = dt.GetSecond();
							EgLib::vt_date d( pdate.get_year(), pdate.get_month(), pdate.get_day(), hour, minute, second );
							pQuoteInfo->LastDateTime = d;
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
					case Feed::FID_CLOSE_CUMULATIVE_VOLUME:
						{
							Activ::UInt volume;
							IFieldType* pUpdateField = static_cast<Activ::UInt*>(&volume);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							if (pQuoteInfo->Volume == BAD_LONG_VALUE)
								pQuoteInfo->Volume = static_cast<long>(volume.Get());
						}
						break;
					case Feed::FID_CLOSE:
						{
							Activ::Rational opn;
							IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->ClosePrice = opn.Get();

							if( ( pQuoteInfo->LastPrice == BAD_DOUBLE_VALUE || !pQuoteInfo->LastPrice) ) 
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
							pQuoteInfo->LastPrice = ask.Get();
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
							pQuoteInfo->HighPrice = opn.Get();
						}
						break;
					case Feed::FID_TRADE_LOW:
						{
							Activ::Rational opn;
							IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->LowPrice = opn.Get();
						}
						break;
					case Feed::FID_NET_CHANGE:
						{
							Activ::TRational ask;
							IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
							(void) pUpdateField->Assign(fld.m_pIFieldType); // set new field values
							pQuoteInfo->NetChange = ask.Get();
						}
						break;
					default:;
					}
				}
			}
		}
	}
	else
	{
		return	STATUS_CODE_FAILURE;
	}
	return STATUS_CODE_SUCCESS;
};
//------------------------------------------------------------------------------------------------------//
void CActivFeedContentGateway::UpdateField(const FieldListValidator::Field& filed, QuoteInfo* pQuoteInfo, QuoteRequest* pRequest)
{

	switch( filed.m_fieldId ) 
	{
	case Feed::FID_OPEN_INTEREST:
		{
			Activ::UInt bidsize;
			IFieldType* pUpdateField = static_cast<Activ::UInt*>(&bidsize);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->OpenInterest = static_cast<long>( bidsize.Get());
		}
		break;
	case Feed::FID_CURRENCY:
		{
			Activ::TextArray pText;
			IFieldType* pUpdateField = static_cast<Activ::TextArray*>(&pText);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->Currency = pText.ToString();
		}
		break;
	case Feed::FID_BID:
		{
			Activ::TRational bid;
			IFieldType* pUpdateField = static_cast<Activ::TRational*>(&bid);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->BidPrice = bid.Get();
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
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->BidSize = static_cast<long>( bidsize.Get());
		}
		break;
	case Feed::FID_BID_EXCHANGE:
		{
			Activ::TextString pText;
			IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->BidExchange = pText.ToString();
		}
		break;
	case Feed::FID_ASK:
		{
			Activ::TRational ask;
			IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->AskPrice = ask.Get();
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
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->AskSize = static_cast<long>(asksize.Get() );
		}
		break;
	case Feed::FID_ASK_EXCHANGE:
		{
			Activ::TextString pText;
			IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->AskExchange = pText.ToString();
		}
		break;
	case Feed::FID_TRADE_EXCHANGE:
		{
			Activ::TextString pText;
			IFieldType* pUpdateField = static_cast<Activ::TextString*>(&pText);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->Exchange = pText.ToString();
		}
		break;
	case Feed::FID_OPEN:
		{
			Activ::Rational opn;
			IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->OpenPrice = opn.Get();
		}
		break;
	case Feed::FID_LAST_UPDATE_DATE:
		{
			Activ::Date dt;
			EgLib::vt_date	pdate( pQuoteInfo->UpdateDateTime );
			IFieldType* pUpdateField = static_cast<Activ::Date*>(&dt);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
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
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			int hour = dt.GetHour();
			int minute = dt.GetMinute();
			int second = dt.GetSecond();
			EgLib::vt_date d( pdate.get_year(), pdate.get_month(), pdate.get_day(),hour, minute, second );
			pQuoteInfo->UpdateDateTime = d;
		}
		break;
	case Feed::FID_BID_TIME:
		{
			Activ::Time dt;
			EgLib::vt_date	pdate( EgLib::vt_date::GetCurrentDate(true) );
			IFieldType* pUpdateField = static_cast<Activ::Time*>(&dt);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			int hour = dt.GetHour();
			int minute = dt.GetMinute();
			int second = dt.GetSecond();
			EgLib::vt_date d( pdate.get_year(), pdate.get_month(), pdate.get_day(),hour, minute, second );
			pQuoteInfo->BidDateTime = d;
		}
		break;
	case Feed::FID_ASK_TIME:
		{
			Activ::Time dt;
			EgLib::vt_date	pdate( EgLib::vt_date::GetCurrentDate(true) );
			IFieldType* pUpdateField = static_cast<Activ::Time*>(&dt);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			int hour = dt.GetHour();
			int minute = dt.GetMinute();
			int second = dt.GetSecond();
			EgLib::vt_date d( pdate.get_year(), pdate.get_month(), pdate.get_day(),hour, minute, second );
			pQuoteInfo->AskDateTime = d;
		}
		break;
	case Feed::FID_TRADE_DATE:
		{
			Activ::Date dt;
			EgLib::vt_date	pdate( pQuoteInfo->LastDateTime );
			IFieldType* pUpdateField = static_cast<Activ::Date*>(&dt);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			int year = dt.GetYear();
			int month = dt.GetMonth();
			int day = dt.GetDay();
			EgLib::vt_date d( year, month, day, pdate.get_hour(), pdate.get_minute(), pdate.get_second());
			pQuoteInfo->LastDateTime = d;
		}
		break;
	case Feed::FID_TRADE_TIME:
		{
			Activ::Time dt;
			EgLib::vt_date	pdate( pQuoteInfo->LastDateTime );
			IFieldType* pUpdateField = static_cast<Activ::Time*>(&dt);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			int hour = dt.GetHour();
			int minute = dt.GetMinute();
			int second = dt.GetSecond();
			EgLib::vt_date d( pdate.get_year(), pdate.get_month(), pdate.get_day(), hour, minute, second );
			pQuoteInfo->LastDateTime = d;
		}
		break;
	case Feed::FID_CUMULATIVE_VOLUME:
		{
			Activ::UInt volume;
			IFieldType* pUpdateField = static_cast<Activ::UInt*>(&volume);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->Volume = static_cast<long>(volume.Get());
		}
		break;
	case Feed::FID_CLOSE_CUMULATIVE_VOLUME:
		{
			Activ::UInt volume;
			IFieldType* pUpdateField = static_cast<Activ::UInt*>(&volume);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			if (pQuoteInfo->Volume == BAD_LONG_VALUE)
				pQuoteInfo->Volume = static_cast<long>(volume.Get());
		}
		break;		
	case Feed::FID_CLOSE:
		{
			Activ::Rational opn;
			IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->ClosePrice = opn.Get();
		}
		break;
	case Feed::FID_CONTRACT_SIZE:
	case Feed::FID_SHARES_PER_CONTRACT:
		{
			Activ::UInt asksize;
			IFieldType* pUpdateField = static_cast<Activ::UInt*>(&asksize);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->LotSize = static_cast<long>(asksize.Get());
		}
		break;
	case Feed::FID_TRADE:
		{
			Activ::TRational ask;
			IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->LastPrice = ask.Get();
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
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->HighPrice = opn.Get();
		}
		break;
	case Feed::FID_TRADE_LOW:
		{
			Activ::Rational opn;
			IFieldType* pUpdateField = static_cast<Activ::Rational*>(&opn);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->LowPrice = opn.Get();
		}
		break;
	case Feed::FID_NET_CHANGE:
		{
			Activ::TRational ask;
			IFieldType* pUpdateField = static_cast<Activ::TRational*>(&ask);
			(void) pUpdateField->Assign(filed.m_pIFieldType); // set new field values
			pQuoteInfo->NetChange = ask.Get();
		}
		break;
	case Feed::FID_QUOTE_DATE:
	case Feed::FID_CUMULATIVE_PRICE:
	case Feed::FID_CUMULATIVE_VALUE:
	case Feed::FID_TRADE_COUNT:
	case Feed::FID_PERCENT_CHANGE:
		break;
	default:;
		pQuoteInfo->Status = enQuoteStatus_Unreliable;
	}
}
//------------------------------------------------------------------------------------------------------//
void CActivFeedContentGateway::OnRecordUpdate(Activ::HeapMessage &update)
{
	CAutoLock lock1(m_RequestLock);
	CAutoLock lock2(m_QuoteLock);

	RecordUpdate recordUpdate;

	if (!IsValidResponse(update))
		return;

	RequestIdPtr spRequestID(new RequestId(update.GetRequestId()));
	
	QuoteAtomPtr spQuote;
	QuoteRequestPtr spRequest;

	FieldListValidator fieldListValidator;
	if ((STATUS_CODE_SUCCESS == RecordUpdateHelper::Deserialize(*this, update, recordUpdate)) &&
		(STATUS_CODE_SUCCESS == fieldListValidator.Initialize(recordUpdate.m_fieldData)) )
	{
		if (GetRequest(recordUpdate.m_requestKey.m_symbol, spRequestID, spRequest) || GetRequest("", spRequestID, spRequest))
		{

			GetQuote(spRequest, spQuote);
			CAutoLock lock(spQuote->m_lock);

			if( recordUpdate.m_fieldData.IsEmpty() )
				return;

			FieldListValidator::ConstIterator fit, fitEnd;
			fit = fieldListValidator.Begin();
			fitEnd = fieldListValidator.End();

			for (; fit != fitEnd; fit++ ) 
			{
				const FieldListValidator::Field& fld = fit.GetField();
				if ( FIELD_STATUS_DEFINED == fld.m_fieldStatus ) 
				{
					if( fld.m_rules & FieldListValidator::Field::RULE_DOES_NOT_UPDATE_LAST_VALUE )	continue;

					UpdateField( fld, spQuote->m_spResponse.get(), spQuote->m_spRequest.get());
					if( !spQuote->m_bUpdate )
					{
						InterlockedIncrement(&m_UpdateCount);
						spQuote->m_bUpdate = true;
					}
				}
			}
		}
	}
}
//------------------------------------------------------------------------------------------------------//
void CActivFeedContentGateway::OnGetEqualResponse(HeapMessage &response)
{
	CAutoLock lock1(m_RequestLock);
	CAutoLock lock2(m_QuoteLock);

	QuoteAtomPtr	spQuote;
	QuoteRequestPtr spRequest;
	RequestIdPtr	spRequestID(new RequestId(response.GetRequestId()));

	GetEqual::ResponseParameters responseParameters;
	StatusCode statusCode = GetEqual::Deserialize(*this, response, responseParameters);

	if( true )
	{
		if( STATUS_CODE_SUCCESS == statusCode )
		{
			ResponseBlockList rbl = responseParameters.m_responseBlockList;
			ResponseBlockList::const_iterator it, itend = rbl.end();
			for( it = rbl.begin(); it != itend; it++ )
			{
				const ResponseBlock& rb = *it;
				if( rb.IsValidResponse() )
				{
					if (!GetRequest(rb.m_requestKey.m_symbol, spRequestID, spRequest)) continue;

					GetQuote(spRequest, spQuote);
					CAutoLock lock(spQuote->m_lock);

					spQuote->m_Cookie = responseParameters.m_subscriptionCookie;

					m_subs[spRequest]++;
					if (m_subs[spRequest] == 1){

						LOG4CPLUS_INFO(ProviderLog, 
							_T("Subscribed: ")<<spRequest->Symbol << 
							_T("\tType:")<<_T(spRequest->Type)<<
							_T("\tCount: ")<<static_cast<long>(m_subs.size())<<
							_T("\tFrom: ")<<static_cast<long>(m_QuoteMap.size()));
					};

					ParseResponseBlock( rb, spQuote->m_spResponse.get(), true, spQuote->m_spRequest.get());
					if( !spQuote->m_bUpdate ) 
						spQuote->m_bUpdate = true;

					InterlockedIncrement(&m_UpdateCount);
				}
			}
		}
	};

};
//------------------------------------------------------------------------------------------------------//
void CActivFeedContentGateway::OnGetMultiplePatternMatchResponse(HeapMessage &response)
{
	CAutoLock lock1(m_RequestLock);
	CAutoLock lock2(m_QuoteLock);

	if (IsValidResponse(response))
	{

		QuoteAtomPtr	spQuote;
		QuoteRequestPtr spRequest;

		RequestIdPtr spRequestID(new RequestId(response.GetRequestId()));

		GetMultiplePatternMatch::ResponseParameters responseParameters;
		StatusCode statusCode = GetMultiplePatternMatch::Deserialize(*this, response, responseParameters);

		if( STATUS_CODE_SUCCESS == statusCode ) 
		{
			ResponseBlockList rbl = responseParameters.m_responseBlockList;
			ResponseBlockList::const_iterator it = rbl.begin(), itend = rbl.end();

			for( it ; it != itend; it++ ) 
			{
				const ResponseBlock& rb = *it;
				if( rb.m_status == ResponseBlock::STATUS_SUCCESS )
				{
					if (!GetRequest("", spRequestID, spRequest)) continue;

					std::string::size_type dotPos = rb.m_responseKey.m_symbol.find('.');

					if (dotPos!= std::string::npos && dotPos == rb.m_responseKey.m_symbol.find_last_of('.'))
					{
						GetQuote(spRequest, spQuote);
						CAutoLock lock(spQuote->m_lock);

						spQuote->m_Cookie = responseParameters.m_subscriptionCookie;

						m_subs[spRequest]++;
						if (m_subs[spRequest] == 1){
							
							LOG4CPLUS_INFO(ProviderLog, 
								_T("Subscribed: ")<<spRequest->Symbol << 
								_T("\tType:")<<_T(spRequest->Type)<<
								_T("\tCount: ")<<static_cast<long>(m_subs.size())<<
								_T("\tFrom: ")<<static_cast<long>(m_QuoteMap.size()));
						};

						ParseResponseBlock( rb, spQuote->m_spResponse.get(), true, spQuote->m_spRequest.get());
						if( !spQuote->m_bUpdate ) 
							spQuote->m_bUpdate = true;

						InterlockedIncrement(&m_UpdateCount);
					}
				}
			}
		}
	}
};
//------------------------------------------------------------------------------------------------------//
void CActivFeedContentGateway::OnGetMultiplePatternMatchListResponse(HeapMessage &response)
{
	//Not impl
};
//------------------------------------------------------------------------------------------------------//
void CActivFeedContentGateway::OnGetMultipleEqualResponse(HeapMessage &response)
{
	CAutoLock lock1(m_RequestLock);
	CAutoLock lock2(m_QuoteLock);

	QuoteAtomPtr	spQuote;
	QuoteRequestPtr spRequest;
	RequestIdPtr	spRequestID(new RequestId(response.GetRequestId()));

	GetMultipleEqual::ResponseParameters responseParameters;
	StatusCode statusCode = GetMultipleEqual::Deserialize(*this, response, responseParameters);
	OnResponse(	response, 
				responseParameters,  
				RequestIdPtr(new RequestId(response.GetRequestId())),
				IsCompleteResponse(response),
				IsValidResponse(response)
				);
};
//------------------------------------------------------------------------------------------------------//
void CActivFeedContentGateway::OnGetMatchResponse(HeapMessage &response)
{
	RequestIdPtr	spRequestID(new RequestId(response.GetRequestId()));
	if (m_functions.find(spRequestID) != m_functions.end()){
		FUNC parseFunction = m_functions[spRequestID];
		((*this).*(parseFunction))(response);
	}
};
//------------------------------------------------------------------------------------------------------//
template<typename T>
void CActivFeedContentGateway::OnResponse(HeapMessage &response,  T& Params,  RequestIdPtr spRequestID, bool bIsComplete, bool bIsValid)
{

	ResponseBlockList& rbl = Params.m_responseBlockList;
	ResponseBlockList::iterator it = rbl.begin(), itend = rbl.end();

	QuoteAtomPtr spQuote;
	QuoteRequestPtr spRequest; 


	for (;it!=itend;it++) 
	{
		ResponseBlock& rb = *it;
		if( rb.IsValidResponse() && rb.IsResponseKeyDefined()) 
		{		
			if (!GetRequest(rb.m_requestKey.m_symbol, spRequestID, spRequest)) continue;
			GetQuote(spRequest, spQuote);
			CAutoLock lock(spQuote->m_lock);

			ParseResponseBlock( rb, spQuote->m_spResponse.get(),  true, spQuote->m_spRequest.get());
			spQuote->m_bUpdate = true;
			spQuote->m_Cookie = Params.m_subscriptionCookie;
			
			m_subs[spRequest]++;
			if (m_subs[spRequest] == 1){
				
				LOG4CPLUS_INFO(ProviderLog, 
					_T("Subscribed: ")<<spRequest->Symbol << 
					_T("\tType:")<<_T(spRequest->Type)<<
					_T("\tCount: ")<<static_cast<long>(m_subs.size())<<
					_T("\tFrom: ")<<static_cast<long>(m_QuoteMap.size()));
			};

			InterlockedIncrement(&m_UpdateCount);
		}
	}
};
//------------------------------------------------------------------------------------------------------//
template<typename T>
void CActivFeedContentGateway::ParseResponseParameters(T& responseParameters, RequestIdPtr spRequestID, bool Single)
{
	QuoteAtomPtr spQuote;
	QuoteRequestPtr spRequest;

	for (	ResponseBlockList::const_iterator responseBlockListIterator = responseParameters.m_responseBlockList.begin(),
		responseBlockListIteratorEnd = responseParameters.m_responseBlockList.end();
		responseBlockListIteratorEnd != responseBlockListIterator; ++responseBlockListIterator)
	{

		const ResponseBlock &responseBlock = *responseBlockListIterator;
		if (responseBlock.m_status != ResponseBlock::STATUS_SOURCE_NOT_FOUND)
		{
			if (GetRequest(responseBlock.m_requestKey.m_symbol, spRequestID, spRequest))
			{
				GetQuote(spRequest, spQuote);
				CAutoLock lock(spQuote->m_lock);

				ParseResponseBlock(responseBlock, spQuote->m_spResponse.get(), false, spRequest.get());
			}
		}
	};
	RemoveRequest(spRequestID);
};
//------------------------------------------------------------------------------------------------------//
bool CActivFeedContentGateway::Activate()
{
	ServiceInstanceList						serviceInstanceList;
	ContentGatewayClient::ConnectParameters conParams;
	StatusCode	SC;

	SC = ServiceApi::FindServices(*this, "Service.WorkstationService", AGENT_SCOPE_GLOBAL, "", serviceInstanceList);

	if ( STATUS_CODE_SUCCESS != SC ) return false;

	const ServiceInstance &serviceInstance	= serviceInstanceList.front();
	conParams.m_serviceId					= serviceInstance.m_serviceId;
	conParams.m_url							= serviceInstanceList.front().m_serviceAccessPointList.front().m_url;
	conParams.m_userId						= m_Settings->m_UserID;
	conParams.m_password					= m_Settings->m_Password;
	
	SC = Connect( conParams );

	m_bConnected = ( STATUS_CODE_SUCCESS == SC );
	
	return m_bConnected;
};
//------------------------------------------------------------------------------------------------------//
bool CActivFeedContentGateway::Deactivate()
{
	StatusCode SC = Disconnect();

	m_bUserDisconnect = true;
	m_bConnected = false;
	SetEvent(m_hOnBreakEvent);

	return	STATUS_CODE_SUCCESS == SC;
}
//------------------------------------------------------------------------------------------------------//
StatusCode			
CActivFeedContentGateway::RequestIndex(QuoteRequestPtr spRequest)
{
	// make new request ID
	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	//prepare request block's
	RequestBlock requestBlock1;
	GetMultiplePatternMatch::RequestParameters requestParameters;
	std::string sKey;
	CRequestConverter::EgarIndex2Activ(spRequest, sKey);
	
	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_TIME);
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_TIME);
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_DATE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_BID );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CURRENCY );

	requestParameters.m_requestBlockList.push_back( requestBlock1 );
	requestParameters.m_tableNumber = Feed::TABLE_NO_INDEX;
	requestParameters.m_symbolPattern = sKey;
	
	AddRequest(sKey, spRequest, spRequestID);

	GetMultiplePatternMatch::ResponseParameters responseParameters;
	StatusCode SC =	GetMultiplePatternMatch::SendRequest(*this, requestParameters, responseParameters);

	if( SC == STATUS_CODE_SUCCESS ) 
	{
		ParseResponseParameters(responseParameters, spRequestID, true);
	};

	return SC;
}
//------------------------------------------------------------------------------------------------------//
StatusCode
CActivFeedContentGateway::RequestStock(QuoteRequestPtr spRequest)
{
	// make new request ID
	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	//prepare request block's
	std::string region = "USA";
	RequestBlock requestBlock1;
	GetEqual::RequestParameters requestParametrs;
	std::string sKey;
	CRequestConverter::EgarStock2Activ(spRequest, sKey);

	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_DATE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CURRENCY );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_BID );
	
	requestParametrs.m_requestBlockList.push_back( requestBlock1 );
	requestParametrs.m_symbolId = SymbolId( region == "USA" ? Feed::TABLE_NO_US_LISTING : Feed::TABLE_NO_AMERICAS_LISTING, sKey );

	AddRequest(sKey, spRequest, spRequestID);

	GetEqual::ResponseParameters responseParameters;
	StatusCode SC =	GetEqual::SendRequest(*this, requestParametrs, responseParameters);

	if( SC == STATUS_CODE_SUCCESS ) 
	{
		ParseResponseParameters(responseParameters, spRequestID, true);
	};

	return SC;
};
//------------------------------------------------------------------------------------------------------//
StatusCode			
CActivFeedContentGateway::RequestOption(QuoteRequestPtr spRequest)
{
	// make new request ID
	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	//prepare request block's
	RequestBlock requestBlock1, requestBlock2;
	std::string sKey;
	CRequestConverter::EgarOption2Activ(spRequest, sKey);
	GetEqual::RequestParameters requestParameters;

	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID );							
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_DATE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN_INTEREST );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_BID );
	requestParameters.m_requestBlockList.push_back( requestBlock1 );

	requestBlock2.m_relationshipId = Feed::RELATIONSHIP_ID_OPTION_ROOT;
	requestBlock2.m_fieldIdList.push_back( Feed::FID_UNDERLYING_SYMBOL );
	requestParameters.m_requestBlockList.push_back( requestBlock2 );

	requestParameters.m_symbolId = SymbolId( Feed::TABLE_NO_US_EQUITY_OPTION, sKey );

	AddRequest(sKey, spRequest, spRequestID);

	GetEqual::ResponseParameters responseParameters;
	StatusCode SC = GetEqual::SendRequest(*this, requestParameters, responseParameters);

	if( SC == STATUS_CODE_SUCCESS ) 
	{
		ParseResponseParameters(responseParameters, spRequestID, true);
	};

	return SC;
};
//------------------------------------------------------------------------------------------------------//
StatusCode			
CActivFeedContentGateway::RequestFuture(QuoteRequestPtr spRequest)
{
	// make new request ID
	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	//prepare request block's
	RequestBlock requestBlock1, requestBlock2;
	SymbolIdMatchRequestParameters requestParameters;

	std::string sKey = spRequest->Symbol;

	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back(Feed::FID_BID );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN_INTEREST );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_DATE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_SETTLEMENT );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	requestParameters.m_requestBlockList.push_back( requestBlock1 );

	requestBlock2.m_relationshipId = Feed::RELATIONSHIP_ID_OPTION_ROOT;
	requestBlock2.m_fieldIdList.push_back( Feed::FID_UNDERLYING_SYMBOL );
	requestParameters.m_requestBlockList.push_back( requestBlock2 );

	requestParameters.m_matchType = MatchRequestParameters::MATCH_TYPE_COMPOSITE;
	requestParameters.m_symbolId = SymbolId( Feed::TABLE_NO_US_FUTURE, sKey );

	AddRequest(sKey, spRequest, spRequestID);

	GetMatch::ResponseParameters responseParameters;
	StatusCode SC = GetMatch::SendRequest(*this, requestParameters, responseParameters);

	if( SC == STATUS_CODE_SUCCESS ) 
	{
		ParseResponseParameters(responseParameters, spRequestID, true);
	};

	return SC;
}
//------------------------------------------------------------------------------------------------------//
StatusCode			
CActivFeedContentGateway::RequestFutureOption(QuoteRequestPtr spRequest)
{
	// make new request ID
	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	//prepare request block's
	RequestBlock requestBlock1, requestBlock2;
	SymbolIdMatchRequestParameters requestParameters;

	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN_INTEREST );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_DATE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_SETTLEMENT );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );

	requestParameters.m_requestBlockList.push_back( requestBlock1 );
	requestBlock2.m_relationshipId = Feed::RELATIONSHIP_ID_FUTURE_ROOT;
	requestBlock2.m_fieldIdList.push_back(FID_CONTRACT_SIZE);
	requestBlock2.m_fieldIdList.push_back(FID_MINIMUM_TICK); 	
	requestBlock2.m_fieldIdList.push_back(FID_UNIT_OF_MEASURE); 
	requestBlock2.m_fieldIdList.push_back(FID_UNIT_OF_MEASURE_CURRENCY);
	requestParameters.m_requestBlockList.push_back( requestBlock2 );

	std::string sKey = spRequest->Symbol;

	requestParameters.m_symbolId = SymbolId(Feed::TABLE_NO_US_FUTURE_OPTION, sKey) ;
	requestParameters.m_matchType = MatchRequestParameters::MATCH_TYPE_COMPOSITE;

	AddRequest(sKey, spRequest, spRequestID);

	GetMatch::ResponseParameters responseParameters;
	StatusCode SC = GetMatch::SendRequest(*this, requestParameters, responseParameters);

	if( SC == STATUS_CODE_SUCCESS ) 
	{
		ParseResponseParameters(responseParameters, spRequestID, true);
	};

	return SC;
};
//------------------------------------------------------------------------------------------------------//
StatusCode			
CActivFeedContentGateway::RequestCurrency(QuoteRequestPtr spRequest)
{
	// make new request ID
	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	//prepare request block's
	RequestBlock requestBlock1;
	GetEqual::RequestParameters requestParameters;
	std::string sKey = spRequest->Symbol + ".TF";

	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_DATE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CURRENCY );

	requestParameters.m_requestBlockList.push_back( requestBlock1 );
	requestParameters.m_symbolId = SymbolId( Feed::TABLE_NO_FOREX, sKey );

	AddRequest(sKey, spRequest, spRequestID);

	GetEqual::ResponseParameters responseParameters;
	StatusCode SC =	GetEqual::SendRequest(*this, requestParameters, responseParameters);

	if( SC == STATUS_CODE_SUCCESS ) 
	{
		ParseResponseParameters(responseParameters, spRequestID, true);
	};

	return SC;
}
//------------------------------------------------------------------------------------------------------//
StatusCode
CActivFeedContentGateway::Request(QuoteRequestPtr& spRequest, QuoteAtomPtr& spQuote)
{
	CAutoLock lock1(m_RequestLock);
	CAutoLock lock2(m_QuoteLock);

	GetQuote(spRequest, spQuote);
	
	LOG4CPLUS_INFO(ProviderLog, _T("Request: ")<<spRequest->Symbol << 
								_T("\tType:")<<_T(spRequest->Type));


	if (spRequest->Type == enSTK)
	{
		return RequestStock(spRequest);
	}
	else if (spRequest->Type == enIDX)
	{
		return RequestIndex(spRequest);
	}
	else if (spRequest->Type == enOPT)
	{
		return RequestOption(spRequest);
	}
	else if (spRequest->Type == enFUT)
	{
		return RequestFuture(spRequest);
	}
	else if (spRequest->Type == enFOP)
	{
		return RequestFutureOption(spRequest);
	}
	else if (spRequest->Type == enFX)
	{
		return RequestCurrency(spRequest);
	}
	
	return STATUS_CODE_FAILURE;
}
//------------------------------------------------------------------------------------------------------//
void				
CActivFeedContentGateway::Filter(CRequestVector& request, InstrumentTypeEnum type, CRequestVector& result, CQuoteVector& response)
{
	CRequestVector::iterator it = request.begin();
	CRequestVector::iterator itEnd = request.end();

	for (; it != itEnd; it++)
	{
		if ((*it)->Type == type)
		{
			QuoteAtomPtr spQuote;
			
			if (!GetQuote(*it, spQuote)) //check if already requested 
			{ 
				result.push_back(*it);

				LOG4CPLUS_DEBUG(ProviderLog, "Process multiple request: " << (*it)->Symbol << "\tType:" << (*it)->Type);
			}
			
			response.push_back(spQuote);
		}
	}
}
//------------------------------------------------------------------------------------------------------//
StatusCode		
CActivFeedContentGateway::RequestMultiple(CRequestVector& request, CQuoteVector& response)
{
	CAutoLock lock1(m_RequestLock);
	CAutoLock lock2(m_QuoteLock);

	CRequestVector toRequest;
	
	StatusCode sc = STATUS_CODE_SUCCESS;

	Filter(request, enOPT, toRequest, response);
	if (!toRequest.empty()){
		sc = RequestOptionMultiple(toRequest);
		toRequest.clear();
	};

	Filter(request, enIDX, toRequest, response);
	if (!toRequest.empty()){
		CRequestVector::iterator it = toRequest.begin();
		CRequestVector::iterator itEnd = toRequest.end();
		for (; it != itEnd; it++){
			sc = RequestIndex(*it);
		}
		toRequest.clear();
	}

	Filter(request, enSTK, toRequest, response);
	if (!toRequest.empty()){
		CRequestVector::iterator it = toRequest.begin();
		CRequestVector::iterator itEnd = toRequest.end();
		for (; it != itEnd; it++){
			sc = RequestStock(*it);
		}
		toRequest.clear();
	}

	Filter(request, enFUT, toRequest, response);
	if (!toRequest.empty()){
		CRequestVector::iterator it = toRequest.begin();
		CRequestVector::iterator itEnd = toRequest.end();
		for (; it != itEnd; it++){
			sc = RequestFuture(*it);
		}
		toRequest.clear();
	}

	Filter(request, enFOP, toRequest, response);
	if (!toRequest.empty()){
		CRequestVector::iterator it = toRequest.begin();
		CRequestVector::iterator itEnd = toRequest.end();
		for (; it != itEnd; it++){
			sc = RequestFutureOption(*it);
		}
		toRequest.clear();
	}
	return sc;
}
//------------------------------------------------------------------------------------------------------//
StatusCode		
CActivFeedContentGateway::SubscribeMultiple(CRequestVector& request)
{	
	CAutoLock lock1(m_RequestLock);
	CAutoLock lock2(m_QuoteLock);

	CRequestVector toRequest;
	CQuoteVector quotes;
	StatusCode sc = STATUS_CODE_SUCCESS;

	Filter(request, enOPT, toRequest, quotes);
	if (!toRequest.empty()){
		sc = SubscribeOptionMultiple(toRequest);
		toRequest.clear();
	};

	Filter(request, enSTK, toRequest, quotes);
	if (!toRequest.empty()){
		sc = SubscribeEquityMultiple(toRequest);
		toRequest.clear();
	};

	Filter(request, enIDX, toRequest, quotes);
	if (!toRequest.empty()){
		CRequestVector::iterator it = toRequest.begin();
		CRequestVector::iterator itEnd = toRequest.end();

		for (; it != itEnd; it++){
			sc = SubscribeIndex(*it);
		}
		toRequest.clear();
	};

	Filter(request, enFUT, toRequest, quotes);
	if (!toRequest.empty()){
		CRequestVector::iterator it = toRequest.begin();
		CRequestVector::iterator itEnd = toRequest.end();

		for (; it != itEnd; it++){
			sc = SubscribeFuture(*it);
		}
		toRequest.clear();
	};

	Filter(request, enFOP, toRequest, quotes);
	if (!toRequest.empty()){
		CRequestVector::iterator it = toRequest.begin();
		CRequestVector::iterator itEnd = toRequest.end();

		for (; it != itEnd; it++){
			sc = SubscribeFutureOption(*it);
		}
		toRequest.clear();
	};

	return sc;
}
//------------------------------------------------------------------------------------------------------//
StatusCode		
CActivFeedContentGateway::SubscribeOptionMultiple(CRequestVector& request)
{
	// make new request ID
	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	//prepare request block's
	RequestBlock requestBlock1, requestBlock2;
	GetMultipleEqual::RequestParameters requestParameters;

	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID );							
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_DATE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN_INTEREST );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_BID );
	requestParameters.m_requestBlockList.push_back( requestBlock1 );

	requestBlock2.m_relationshipId = Feed::RELATIONSHIP_ID_OPTION_ROOT;
	requestBlock2.m_fieldIdList.push_back( Feed::FID_UNDERLYING_SYMBOL );
	requestParameters.m_requestBlockList.push_back( requestBlock2 );

	requestParameters.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	requestParameters.m_permissionLevel = Feed::PERMISSION_LEVEL_DEFAULT;

	CRequestVector::iterator itr = request.begin();
	CRequestVector::iterator itEnd = request.end();

	for(; itr != itEnd; itr++)
	{
		std::string sKey;
		CRequestConverter::EgarOption2Activ(*itr, sKey);
		requestParameters.m_symbolIdList.push_back( SymbolId( Feed::TABLE_NO_US_EQUITY_OPTION, sKey ) );
		AddRequest(sKey, *itr, spRequestID);
	}

	GetMultipleEqual::ResponseParameters responseParameters;
	StatusCode SC = GetMultipleEqual::PostRequest(*this, *spRequestID,	requestParameters );

	return SC;
};
//------------------------------------------------------------------------------------------------------//
StatusCode			
CActivFeedContentGateway::SubscribeEquityMultiple(CRequestVector& request)
{
	std::string	sRegion = "USA";
	// make new request ID
	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	//prepare request block's
	RequestBlock requestBlock1, requestBlock2, requestBlock3;
	GetMultipleEqual::RequestParameters requestParameters;

	// fill up request block
	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_DATE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_ASK );	
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_BID );
	requestParameters.m_requestBlockList.push_back( requestBlock1 );

	requestBlock2.m_relationshipId = Feed::RELATIONSHIP_ID_SECURITY;
	requestBlock2.m_fieldIdList.push_back(Feed::FID_CURRENCY );
	requestParameters.m_requestBlockList.push_back( requestBlock2 );

	requestBlock3.m_relationshipId = Feed::RELATIONSHIP_ID_OPTION_ROOT;
	requestBlock3.m_fieldIdList.push_back(Feed::FID_SHARES_PER_CONTRACT);
	requestParameters.m_requestBlockList.push_back( requestBlock3 );

	requestParameters.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	requestParameters.m_permissionLevel = Feed::PERMISSION_LEVEL_DEFAULT;

	CRequestVector::iterator itr = request.begin();
	CRequestVector::iterator itEnd = request.end();

	for(; itr != itEnd; itr++)
	{
		std::string sKey;
		CRequestConverter::EgarStock2Activ(*itr, sKey);
		requestParameters.m_symbolIdList.push_back( SymbolId( sRegion == "USA" ? Feed::TABLE_NO_US_LISTING : Feed::TABLE_NO_AMERICAS_LISTING, sKey ) );
		AddRequest(sKey, *itr, spRequestID);
	}

	GetMultipleEqual::ResponseParameters responseParameters;
	StatusCode SC = GetMultipleEqual::PostRequest(*this, *spRequestID,	requestParameters );

	return SC;
};
//------------------------------------------------------------------------------------------------------//
StatusCode
CActivFeedContentGateway::Subscribe(QuoteRequestPtr& spRequest)
{
	CAutoLock lock1(m_RequestLock);
	CAutoLock lock2(m_QuoteLock);

	QuoteAtomPtr spQuote;

	if (!IsSubscribed(spRequest))
	{

		LOG4CPLUS_DEBUG(ProviderLog, "Subscribe: " << spRequest->Symbol << ", Type:" << spRequest->Type);
			
		GetQuote(spRequest, spQuote);

		if (spRequest->Type == enSTK)
		{
			return SubscribeStock(spRequest);
		}
		else if (spRequest->Type == enIDX)
		{
			return SubscribeIndex(spRequest);
		}
		else if (spRequest->Type == enOPT)
		{
			return SubscribeOption(spRequest);
		}
		else if (spRequest->Type == enFUT)
		{
			return SubscribeFuture(spRequest);
		}
		else if (spRequest->Type == enFOP)
		{
			return SubscribeFutureOption(spRequest);
		}
		else if (spRequest->Type == enFX)
		{
			return SubscribeCurrency(spRequest);
		}
	}
	return STATUS_CODE_SUCCESS;
}
//------------------------------------------------------------------------------------------------------//
StatusCode
CActivFeedContentGateway::SubscribeStock(QuoteRequestPtr spRequest)
{
	RequestBlock requestBlock1, requestBlock2, requestBlock3;
	GetEqual::RequestParameters requestParameters;
	std::string	sRegion = "USA";
	std::string sKey;
	CRequestConverter::EgarStock2Activ(spRequest, sKey);

	// make new request ID
	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	// fill up request block
	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_DATE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_ASK );	
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_BID );
	requestParameters.m_requestBlockList.push_back( requestBlock1 );

	requestBlock2.m_relationshipId = Feed::RELATIONSHIP_ID_SECURITY;
	requestBlock2.m_fieldIdList.push_back(Feed::FID_CURRENCY );
	requestParameters.m_requestBlockList.push_back( requestBlock2 );

	requestBlock3.m_relationshipId = Feed::RELATIONSHIP_ID_OPTION_ROOT;
	requestBlock3.m_fieldIdList.push_back(Feed::FID_SHARES_PER_CONTRACT);
	requestParameters.m_requestBlockList.push_back( requestBlock3 );

	requestParameters.m_symbolId = SymbolId( sRegion == "USA" ? Feed::TABLE_NO_US_LISTING : Feed::TABLE_NO_AMERICAS_LISTING, sKey );
	requestParameters.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	requestParameters.m_permissionLevel = Feed::PERMISSION_LEVEL_DEFAULT;

	StatusCode statusCode = GetEqual::PostRequest(*this, *spRequestID, requestParameters);
	
	if (statusCode == STATUS_CODE_SUCCESS){
		AddRequest(sKey, spRequest, spRequestID);
	};

	return statusCode;	
}
//------------------------------------------------------------------------------------------------------//
StatusCode			
CActivFeedContentGateway::SubscribeIndex(QuoteRequestPtr spRequest)
{
	RequestBlock requestBlock1, requestBlock2;
	GetMultiplePatternMatch::RequestParameters requestParameters;
	RequestIdPtr spRequestID(new RequestId);

	std::string sKey;
	CRequestConverter::EgarIndex2Activ(spRequest, sKey);

	// make new request ID
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	// fill up request block
	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_TIME);
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_TIME);
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_DATE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_BID );
	requestParameters.m_requestBlockList.push_back( requestBlock1 );

	requestBlock2.m_relationshipId = Feed::RELATIONSHIP_ID_SECURITY;
	requestBlock2.m_fieldIdList.push_back(Feed::FID_CURRENCY );
	requestParameters.m_requestBlockList.push_back( requestBlock2 );

	requestParameters.m_tableNumber = Feed::TABLE_NO_INDEX;
	requestParameters.m_symbolPattern = sKey;

	requestParameters.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	requestParameters.m_permissionLevel = Feed::PERMISSION_LEVEL_DEFAULT;

	StatusCode statusCode = GetMultiplePatternMatch::PostRequest( *this, *spRequestID,	requestParameters);

	if (statusCode == STATUS_CODE_SUCCESS){
		AddRequest("", spRequest, spRequestID);
	};

	return statusCode;	
};
//------------------------------------------------------------------------------------------------------//
StatusCode			
CActivFeedContentGateway::RequestOptionMultiple(CRequestVector& request)
{
	// make new request ID
	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));
	
	//prepare request block's
	RequestBlock requestBlock1, requestBlock2;
	GetMultipleEqual::RequestParameters requestParameters;

	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID );							
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_DATE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN_INTEREST );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_BID );
	requestParameters.m_requestBlockList.push_back( requestBlock1 );

	requestBlock2.m_relationshipId = Feed::RELATIONSHIP_ID_OPTION_ROOT;
	requestBlock2.m_fieldIdList.push_back( Feed::FID_UNDERLYING_SYMBOL );
	requestParameters.m_requestBlockList.push_back( requestBlock2 );

	CRequestVector::iterator itr = request.begin();
	CRequestVector::iterator itEnd = request.end();

	for(; itr != itEnd; itr++)
	{
		std::string sKey;
		CRequestConverter::EgarOption2Activ(*itr, sKey);
		requestParameters.m_symbolIdList.push_back( SymbolId( Feed::TABLE_NO_US_EQUITY_OPTION, sKey ) );
		AddRequest(sKey, *itr, spRequestID);
	}

	GetMultipleEqual::ResponseParameters responseParameters;
	StatusCode SC = GetMultipleEqual::SendRequest(*this, requestParameters, responseParameters);
	
	ParseResponseParameters(responseParameters, spRequestID, false);
	
	return SC;
}
//------------------------------------------------------------------------------------------------------//
StatusCode			
CActivFeedContentGateway::SubscribeOption(QuoteRequestPtr spRequest)
{
	StatusCode SC = STATUS_CODE_SUCCESS;

	RequestBlock requestBlock1, requestBlock2;
	GetEqual::RequestParameters requestParameters;
	std::string sKey;
	CRequestConverter::EgarOption2Activ(spRequest, sKey);

	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));
	
	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_DATE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN_INTEREST);
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_ASK );	
	requestBlock1.m_fieldIdList.push_back( Feed::FID_PREVIOUS_BID );

	// fill up request block
	requestParameters.m_requestBlockList.push_back( requestBlock1 );

	requestBlock2.m_relationshipId = Feed::RELATIONSHIP_ID_OPTION_ROOT;
	requestBlock2.m_fieldIdList.push_back( Feed::FID_UNDERLYING_SYMBOL );
	requestParameters.m_requestBlockList.push_back( requestBlock2 );

	requestParameters.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	requestParameters.m_permissionLevel = Feed::PERMISSION_LEVEL_DEFAULT;
	requestParameters.m_symbolId = SymbolId(Feed::TABLE_NO_UNDEFINED, sKey );

	StatusCode statusCode = GetEqual::PostRequest(*this, *spRequestID, requestParameters);

	if (statusCode == STATUS_CODE_SUCCESS){
		AddRequest(sKey, spRequest, spRequestID);
	};

	return statusCode;
};
//------------------------------------------------------------------------------------------------------//
StatusCode			
CActivFeedContentGateway::SubscribeFuture(QuoteRequestPtr spRequest)
{
	RequestBlock requestBlock1;
	SymbolIdMatchRequestParameters requestParameters;
	
	//make new requestid
	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	std::string sKey = spRequest->Symbol;

	// fill up request block
	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN_INTEREST );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_DATE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_SETTLEMENT );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	requestParameters.m_requestBlockList.push_back( requestBlock1 );

	requestParameters.m_symbolId =  SymbolId( Feed::TABLE_NO_US_FUTURE, sKey );
	requestParameters.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	requestParameters.m_permissionLevel = Feed::PERMISSION_LEVEL_DEFAULT;
	requestParameters.m_matchType = MatchRequestParameters::MATCH_TYPE_COMPOSITE;

	StatusCode statusCode = GetMatch::PostRequest(*this, *spRequestID,	requestParameters);
	
	m_functions[spRequestID] = &CActivFeedContentGateway::ProcessSingleFutureSubscribeResponse;

	if (statusCode == STATUS_CODE_SUCCESS){
		AddRequest("", spRequest, spRequestID);
	};
	return statusCode;	
};
//------------------------------------------------------------------------------------------------------//
StatusCode			
CActivFeedContentGateway::SubscribeFutureOption(QuoteRequestPtr spRequest)
{
	RequestBlock requestBlock1;
	SymbolIdMatchRequestParameters requestParameters;

	std::string sKey = spRequest->Symbol;

	// make new request ID
	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	// fill up request block
	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN_INTEREST );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_DATE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_SETTLEMENT );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	requestParameters.m_requestBlockList.push_back( requestBlock1 );

	
	requestParameters.m_symbolId = SymbolId( Feed::TABLE_NO_US_FUTURE_OPTION, sKey ) ;
	requestParameters.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	requestParameters.m_permissionLevel = Feed::PERMISSION_LEVEL_DEFAULT;
	requestParameters.m_matchType = MatchRequestParameters::MATCH_TYPE_COMPOSITE;

	StatusCode statusCode = GetMatch::PostRequest(*this, *spRequestID,	requestParameters);
	
	m_functions[spRequestID] = &CActivFeedContentGateway::ProcessSingleFutureSubscribeResponse;

	if (statusCode == STATUS_CODE_SUCCESS){
		AddRequest("", spRequest, spRequestID);
	};

	return statusCode;	
};
//------------------------------------------------------------------------------------------------------//
StatusCode
CActivFeedContentGateway::SubscribeCurrency(QuoteRequestPtr spRequest)
{
	RequestBlock requestBlock1, requestBlock2;
	GetEqual::RequestParameters requestParameters;
	std::string sKey = spRequest->Symbol + ".TF";
	
	// make new request ID
	RequestIdPtr spRequestID(new RequestId);
	long lRequestID = InterlockedIncrement(&m_nextRequestId);
	spRequestID->Set(&lRequestID, sizeof(lRequestID));

	// fill up request block
	requestBlock1.m_relationshipId = Feed::RELATIONSHIP_ID_NONE;
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_BID_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_SIZE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_ASK_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_OPEN );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_HIGH ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_LOW ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_EXCHANGE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_DATE ); 
	requestBlock1.m_fieldIdList.push_back( Feed::FID_TRADE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_DATE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_LAST_UPDATE_TIME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE_CUMULATIVE_VOLUME );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_CLOSE );
	requestBlock1.m_fieldIdList.push_back( Feed::FID_NET_CHANGE );
	requestParameters.m_requestBlockList.push_back( requestBlock1 );

	requestBlock2.m_relationshipId = Feed::RELATIONSHIP_ID_SECURITY;
	requestBlock2.m_fieldIdList.push_back(Feed::FID_CURRENCY );
	requestParameters.m_requestBlockList.push_back( requestBlock2 );

	requestParameters.m_symbolId = SymbolId(Feed::TABLE_NO_FOREX, sKey );
	requestParameters.m_subscribeParameters.m_type = SubscribeParameters::TYPE_FULL;
	requestParameters.m_permissionLevel = Feed::PERMISSION_LEVEL_DEFAULT;

	StatusCode statusCode = GetEqual::PostRequest(*this, *spRequestID, requestParameters);	
	
	if (statusCode == STATUS_CODE_SUCCESS){
		AddRequest(sKey, spRequest, spRequestID);
	};
	return statusCode;
}
//------------------------------------------------------------------------------------------------------//
bool	
CActivFeedContentGateway::GetQuote(QuoteRequestPtr& spRequest, QuoteAtomPtr& spQuote)
{
	CRequestQuoteMap::iterator it = m_QuoteMap.find( spRequest );

	if (it == m_QuoteMap.end())
	{
		QuoteAtomPtr quote( new QuoteAtom );
		quote->m_spResponse = QuoteInfoPtr( new QuoteInfo );
		quote->m_spRequest = spRequest;
		quote->m_Cookie = 0;
		m_QuoteMap[ spRequest ] = quote;
		spQuote = quote;

		return false;
	}
	else{
		spQuote = it->second;
	}

	return true;
}
//------------------------------------------------------------------------------------------------------//
void CActivFeedContentGateway::RemoveQuote(QuoteRequestPtr& spRequest)
{
	CRequestQuoteMap::iterator it = m_QuoteMap.find( spRequest );

	if (m_QuoteMap.end() != it)
		m_QuoteMap.erase(it);
}
//------------------------------------------------------------------------------------------------------//
void CActivFeedContentGateway::AddRequest(std::string Key, QuoteRequestPtr& spRequest, RequestIdPtr& spRequestID)
{
	//CAutoLock lock(m_RequestLock);
	m_RequestMap[ spRequestID ][Key] = m_QuoteMap[ spRequest ];
};
//------------------------------------------------------------------------------------------------------//
void CActivFeedContentGateway::RemoveRequest(RequestIdPtr &spRequestID)
{
	//CAutoLock lock(m_RequestLock);
	m_RequestMap.erase(m_RequestMap.find(spRequestID));
};
//------------------------------------------------------------------------------------------------------//
void CActivFeedContentGateway::RemoveRequestKey(RequestIdPtr& spRequestID, std::string Key)
{
	if (m_RequestMap.find(spRequestID) != m_RequestMap.end()){
		if (m_RequestMap[spRequestID].find(Key) != m_RequestMap[spRequestID].end()){			
			m_RequestMap[spRequestID].erase(m_RequestMap[spRequestID].find(Key));
		}
	}
}
//------------------------------------------------------------------------------------------------------//
bool CActivFeedContentGateway::GetRequest(std::string Key, RequestIdPtr& spRequestID, QuoteRequestPtr& spRequest)
{
	//CAutoLock lock(m_RequestLock);
	if (m_RequestMap[spRequestID].find(Key) != m_RequestMap[spRequestID].end()){
		spRequest = m_RequestMap[ spRequestID ][Key]->m_spRequest;
		return true;
	}
	return false;
};
//------------------------------------------------------------------------------------------------------//
bool CActivFeedContentGateway::IsSubscribed(QuoteRequestPtr& spRequest)
{
	return (m_QuoteMap.find( spRequest ) != m_QuoteMap.end());
};
//------------------------------------------------------------------------------------------------------//
bool CActivFeedContentGateway::GetUpdates(CQuoteVector& quotes)
{
	bool	bQuotesUpdated = false;

	try 
	{
		CAutoLock lock1(m_RequestLock);
		CAutoLock lock2(m_QuoteLock);

		CRequestQuoteMap::iterator	itBegin = m_QuoteMap.begin();
		CRequestQuoteMap::iterator	itEnd = m_QuoteMap.end();
		CRequestQuoteMap::iterator	itr = itBegin;
		
		m_nUpdate++;

		for (;itr != itEnd; itr++)
		{
			CAutoLock quote_lock(itr->second->m_lock);

			if(itr->second->m_bUpdate)
			{
				quotes.push_back(itr->second);
				itr->second->m_bUpdate = false;
				bQuotesUpdated = true;

				/*itr->second->m_spResponse->BidPrice +=  (double(m_nUpdate % 2) - 0.5) * 0.001;
				itr->second->m_spResponse->AskPrice +=  (double(m_nUpdate % 2) - 0.5) * 0.001;
				itr->second->m_spResponse->LastPrice +=  (double(m_nUpdate % 2) - 0.5) * 0.001;*/

				//itr->second->m_spResponse->BidSize = (100 + m_nUpdate );
				//itr->second->m_spResponse->AskSize = (200 + m_nUpdate );
				//itr->second->m_spResponse->Volume = (300 + m_nUpdate );

				LOG4CPLUS_DEBUG(ProviderLog, _T("Symbol: ") << itr->second->m_spRequest->Symbol <<
												_T("\tType: ") << itr->second->m_spRequest->Type<<
												_T("\tBid:") <<	itr->second->m_spResponse->BidPrice<<
												_T("\tAsk:") << itr->second->m_spResponse->AskPrice<<
												_T("\tLast:") << itr->second->m_spResponse->LastPrice<<
												_T("\tBidSize:")<< itr->second->m_spResponse->BidSize<<
												_T("\tAskSize:")<< itr->second->m_spResponse->AskSize<<
												_T("\tVolume:")<< itr->second->m_spResponse->Volume);
			}			
		}
	}
	catch (...)
	{
		LOG4CPLUS_ERROR(ProviderLog, _T("Unknwn exception while GettingUpdates()"));
		return false;
	}
	
	LOG4CPLUS_INFO(ProviderLog, "GetUpdates requested " << static_cast<long>(quotes.size()) << " records");
	
	return bQuotesUpdated;
};
//------------------------------------------------------------------------------------------------------//
void CActivFeedContentGateway::Resubscribe()
{
	CRequestQuoteMap::iterator it = m_QuoteMap.begin();
	CRequestQuoteMap::iterator itEnd = m_QuoteMap.end();

	CRequestVector request;
	for ( ; it != itEnd; it++)
	{
		request.push_back(it->first);
	}

	{
		CRequestMap::iterator it = m_RequestMap.begin();
		CRequestMap::iterator itEnd = m_RequestMap.end();
		for ( ; it != itEnd; it++) it->second.clear();
	}
	m_RequestMap.clear();
	m_QuoteMap.clear();

	SubscribeMultiple(request);
	//CRequestVector::iterator itr = request.begin();
	//CRequestVector::iterator itrEnd = request.end();

	/*for (; itr != itrEnd; itr++)
	{
		Subscribe(*itr);
	}*/
}
//------------------------------------------------------------------------------------------------------//
bool CActivFeedContentGateway::ProcessSingleFutureSubscribeResponse(HeapMessage &response)
{
	CAutoLock lock1(m_RequestLock);
	CAutoLock lock2(m_QuoteLock);

	QuoteAtomPtr	spQuote;
	QuoteRequestPtr spRequest;
	RequestIdPtr	spRequestID(new RequestId(response.GetRequestId()));

	GetMatch::ResponseParameters responseParameters;
	StatusCode statusCode = GetMatch::Deserialize(*this, response, responseParameters);

	if( STATUS_CODE_SUCCESS != statusCode )	return false;
	
	ResponseBlockList rbl = responseParameters.m_responseBlockList;
	ResponseBlockList::const_iterator it, itend = rbl.end();

	std::deque<ResponseBlock*>	best_response;
	
	for( it = rbl.begin(); it != itend; it++ )
	{
		const ResponseBlock& rb = *it;
		if( rb.IsValidResponse() )
		{
			ResponseBlock* ptr = const_cast<ResponseBlock*>(&rb);
			
			if (rb.m_responseKey.m_symbol.find(".CM*") != std::string::npos) 
				best_response.push_front(ptr);
			else
				best_response.push_back(ptr);					
		}
	}

	if (!best_response.empty()){

		ResponseBlock *bestblock = best_response[0];
		best_response.pop_front();	

		SymbolIdList	sl;
		std::deque<ResponseBlock*>::iterator qit = best_response.begin(), qitend = best_response.end();
		for ( ; qit != qitend; qit++){
			ResponseBlock* rblk = *qit;
			sl.push_back( rblk->m_responseKey );
		};

		if(sl.size()) {
			Unsubscribe(responseParameters.m_subscriptionCookie, sl);
		};

		if (GetRequest("", spRequestID, spRequest)){
			GetQuote(spRequest, spQuote);
			CAutoLock lock(spQuote->m_lock);
			ParseResponseBlock( *bestblock, spQuote->m_spResponse.get(), true, spQuote->m_spRequest.get());
		};

		if (IsCompleteResponse(response)) {
			if (GetRequest("", spRequestID, spRequest)) {

				GetQuote(spRequest, spQuote);
				CAutoLock lock(spQuote->m_lock);

				spQuote->m_Cookie = responseParameters.m_subscriptionCookie;
				spQuote->m_bUpdate = true;
			}
		};

	}	
	return true;
};
//------------------------------------------------------------------------------------------------------//
