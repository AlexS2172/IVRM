#include "StdAfx.h"
#include ".\activfeedappication.h"
#include <PriceProviders_i.c>
#include "ActivMiddleware\ActivBase\OsException.h"

StatusCode CActivFeedAppication::OnMessage(HeapMessage &message)
{
	return AgentApplication::OnMessage(message);
}

void CActivFeedAppication::OnShutdown()
{
	AgentApplication::OnShutdown();
}

void CActivFeedAppication::OnData(CAsyncNotificationPtr& Data)
{
	  if(Data != NULL)
	  {
		  switch(Data->GetType())
		  {
		  case CAsyncNotification::enRequestLast:
			  {

				  if( m_pBase )
				  {
					  if( Data->GetRequest()->TryCount )
					  {
						  switch( Data->GetRequest()->Type)
						  {
						  case enSTK:
							  Data->GetRequest()->Type = enIDX;
							  break;
						  case enIDX:
						  default:
							  Data->GetRequest()->Type = enSTK;
						  }
					  }

					  CComVariant vp;
					  CComVariant vi;
					  _QuoteUpdateInfo ri;
					  _QuoteUpdateParams rp;
					  ri.Init();
					  rp.Init();

					  if( Data->GetResponse()->AskExchange.length() )
						  Activ2Egar( Data->GetResponse()->AskExchange, Data->GetRequest()->Type );
					  if( Data->GetResponse()->BidExchange.length() )
						  Activ2Egar( Data->GetResponse()->BidExchange, Data->GetRequest()->Type );
					  if( Data->GetResponse()->Exchange.length() )
						  Activ2Egar( Data->GetResponse()->Exchange,    Data->GetRequest()->Type );

					  InitQUI( ri );
					  Data->GetResponse()->AssignToQuoteUpdateInfo( ri );
					  Data->GetRequest()->AssignToQuoteUpdateParams( rp );

					  rp.CopyTo( vp );
					  ri.CopyTo( vi );
					  m_pBase->OnLastQuote( vp, vi );
					  EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ ,\
						  _T("RLQ Response QUI[%s.%s[%d]]"), Data->GetRequest()->Symbol.c_str(), Data->GetRequest()->Exchange.c_str(), Data->GetRequest()->Type );
				  }

			  }break;
		  case CAsyncNotification::enSubscribe:
			  {
				  if( m_pBase  )
				  {
					  if( Data->GetRequest()->TryCount )
					  {
						  switch( Data->GetRequest()->Type) 
						  {
						  case enSTK:
							  Data->GetRequest()->Type = enIDX;
							  break;
						  case enIDX:
						  default:
							  Data->GetRequest()->Type = enSTK;
						  }
					  }
					  CComVariant vp;
					  _QuoteUpdateParams rp;
					  rp.Init();
					  Data->GetRequest()->AssignToQuoteUpdateParams( rp );
					  rp.CopyTo( vp );
					  EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ ,\
						  _T("SQU Sending Response- OK: QUI[%s.%s[%d]]"), Data->GetRequest()->Symbol.c_str(), Data->GetRequest()->Exchange.c_str(), Data->GetRequest()->Type );
					  if(m_pBase->m_lUnnotifiedSubscriptions > 0)
					  {
						  InterlockedDecrement(&m_pBase->m_lUnnotifiedSubscriptions);
						  m_pBase->OnSubscribed( vp );
					  }
				  }
			  }break;
		  }
	  }
}




