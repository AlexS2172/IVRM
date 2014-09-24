// DataProcessor.cpp : implementation file
//

#include "stdafx.h"
#include "IVAnalyzer.h"
#include "DataProcessor.h"
#include <OptionCalc\OptionCalc.h>


//#include "..\\include\\NetMsgStruct.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifdef _VOLA_MANAGER

#include <initguid.h>
#include "VolaMgmtInstance.h"

#endif //_VOLA_MANAGER

CDataProcessor g_DataProcessor;

// internal messages

#define WM_GET_DATA			WM_USER+2700
#define WM_STOP_HANDLING	WM_USER+2701
#define WM_RELOAD			WM_USER+2702

const double VOLA_MAX = 2.5;
const double VOLA_MIN = 0.;
// Internal exception used when processing is interrupted
class CStopException
{
public:
	CStopException() {}
};

/////////////////////////////////////////////////////////////////////////////
// CDataProcessor

const double CDataProcessor::m_dPriceDelta = 0.1;


_ATL_FUNC_INFO CDataProcessor::m_OnLastQuoteInfo = 
	{CC_STDCALL, VT_EMPTY, 2, {VT_RECORD, VT_RECORD} };
 
_ATL_FUNC_INFO CDataProcessor::m_OnQuoteUpdateInfo = 
	{CC_STDCALL, VT_EMPTY, 0, {VT_EMPTY} };

_ATL_FUNC_INFO CDataProcessor::m_OnErrorInfo = 
	{CC_STDCALL, VT_EMPTY, 4, {VT_I4, VT_BSTR, VT_I4, VT_VARIANT} };

_ATL_FUNC_INFO CDataProcessor::m_OnSubscribedInfo = 
{CC_STDCALL, VT_EMPTY, 1, {VT_RECORD} };

_ATL_FUNC_INFO CDataProcessor::m_OnUnsubscribedInfo = 
{CC_STDCALL, VT_EMPTY, 0, {VT_EMPTY} };

_ATL_FUNC_INFO CDataProcessor::m_OnSOAPErrorInfo = 
	{CC_STDCALL, VT_EMPTY, 2, {VT_I4, VT_BSTR}};

_ATL_FUNC_INFO CDataProcessor::m_OnSOAPResultInfo = 
	{CC_STDCALL, VT_EMPTY, 3, {VT_BSTR, VT_VARIANT, VT_VARIANT}};


_ATL_FUNC_INFO CDataProcessor::m_OnIVAuthErrorInfo = 
	{CC_STDCALL, VT_EMPTY, 2, {VT_I4, VT_BSTR}};

_ATL_FUNC_INFO CDataProcessor::m_OnIVAuthResultInfo = 
	{CC_STDCALL, VT_EMPTY, 1, {VT_BSTR}};

_ATL_FUNC_INFO CDataProcessor::m_OnControllingEventInfo = 
	{CC_STDCALL, VT_EMPTY, 2, {VT_I4, VT_VARIANT} };

STDMETHODIMP CDataProcessor::OnControllingEvent(IVM::IVMCommandTypeEnum en, VARIANT vt)
{
	try
	{
		switch(en)
		{
		case IVM::enIVMSymbol:
			{
				if(V_VT(&vt)==VT_BSTR)
				{
					USES_CONVERSION;
					_bstr_t bsContract;
					CString strContract = W2T(V_BSTR(&vt));
					ContractChangedNotification(strContract);
				}
			}break;
		}
	}
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	return S_OK;
}

#ifdef _VOLA_MANAGER
_ATL_FUNC_INFO CDataProcessor::m_OnDefaultSurfaceChangedInfo = 
	{CC_STDCALL, VT_EMPTY, 3, {VT_I4, VT_I4, VT_BOOL}};

_ATL_FUNC_INFO CDataProcessor::m_OnDatabaseConnectionChangedInfo = 
	{CC_STDCALL, VT_EMPTY, 1, {VT_BSTR}};

_ATL_FUNC_INFO CDataProcessor::m_OnPriceProviderChangedInfo = 
	{CC_STDCALL, VT_EMPTY, 1, {VT_I4}};

_ATL_FUNC_INFO CDataProcessor::m_OnIVSettingsChangedInfo = 
	{CC_STDCALL, VT_EMPTY, 1, {VT_I4}};

_ATL_FUNC_INFO CDataProcessor::m_OnTradeUpdateInfo = 
	{CC_STDCALL, VT_EMPTY, 1, {VT_DISPATCH}};

STDMETHODIMP CDataProcessor::OnDatabaseConnectionChanged(/*[in]*/BSTR bsConnectionString)
{
	AfxGetMainWnd()->PostMessage( WM_SETTINGS, 1L, 0L );
	return S_OK;
}
STDMETHODIMP CDataProcessor::OnPriceProviderChanged(/*[in]*/ long lProviderType)
{
	AfxGetMainWnd()->PostMessage( WM_SETTINGS, 2L, 0L );
	return S_OK;
}
STDMETHODIMP CDataProcessor::OnIVSettingsChanged(/*[in]*/ Stg::IVSettingsType enType)
{
	AfxGetMainWnd()->PostMessage( WM_SETTINGS, 3L, (LPARAM)enType );
	return S_OK;
}

STDMETHODIMP CDataProcessor::OnTradeUpdate(/*[in]*/ ITradeUpdate* Params)
{
	try
	{
		long lSeqNum;
		_com_util::CheckError(Params->get_SeqNum(&lSeqNum));
		
		m_csTradeData.Lock();
		if(lSeqNum > m_lTradeSeqNum + 1)
		{			
			//Reload UnderlyingPos !!!
			ReLoadContractPos();
			m_csTradeData.Unlock();
			UpdateVola();

			return S_OK;
		}
		else if(lSeqNum == m_lTradeSeqNum + 1)
		{
			m_lTradeSeqNum++;
		}
		else
		{
			m_csTradeData.Unlock();
			return S_OK;
		}
		m_csTradeData.Unlock();

		TRADE_STATUS enTradeStatus;
		_com_util::CheckError(Params->get_TradeStatus(&enTradeStatus));
		
		if(enTradeStatus == enTsSimulated || enTradeStatus == enTsManual)
			return S_OK;
				
		SYMBOL_TYPE symType;
		_com_util::CheckError(Params->get_SymbolType(&symType));
		if(symType != enStStock && symType != enStIndex)
			return S_OK;

		long lConID;
		_com_util::CheckError(Params->get_UndSymbolID(&lConID));

		m_csContractData.Lock();		
		if(m_contractData.nID != lConID)
		{
			m_csContractData.Unlock();
			return S_OK;
		}
		m_csContractData.Unlock();

		TRADE_ACTION action;
		_com_util::CheckError(Params->get_Action(&action));

		double dPrice;
		_com_util::CheckError(Params->get_Price(&dPrice));

		long lQuant;
		_com_util::CheckError(Params->get_Quantity(&lQuant));

		long lLotSize;
		_com_util::CheckError(Params->get_LotSize(&lLotSize));

		long lIsBuy;
		_com_util::CheckError(Params->get_IsBuy(&lIsBuy));

		double OldPos;
		m_csContractData.Lock();
		
		OldPos = m_contractData.m_dPosition;

		if(action == enTeNew)
		{
			if(m_contractData.m_dPosition == (-DBL_MAX))
				m_contractData.m_dPosition = 0;

			m_contractData.m_dPosition += dPrice * lQuant * lLotSize * ( lIsBuy ? 1 : -1);
		}
		else if(action == enTeDelete && m_contractData.m_dPosition > (-DBL_MAX))
		{
			m_contractData.m_dPosition -= dPrice * lQuant * lLotSize * ( lIsBuy ? 1 : -1);
		}
		
		m_csContractData.Unlock();
		
		UpdateVola();
	}
	catch( const _com_error& e )
	{
		//ErrorNotification( e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage(), true, true ); 
		SetStatusText(e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage());
		return e.Error();
	}
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	return S_OK;
}

void CDataProcessor::UpdateVola( )
{
	try
	{
		if( BeginProcessing() )
		{
			try
			{
				if( RecalcOptions() )
				{
					UpdateVolaCache();
					VolaChangedNotification();
				}
			}
			catch (CStopException &) {}

			EndProcessing();
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;

}

#endif //_VOLA_MANAGER


IMPLEMENT_DYNCREATE(CDataProcessor, CWinThread)

BEGIN_MESSAGE_MAP(CDataProcessor, CWinThread)
	//{{AFX_MSG_MAP(CDataProcessor)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP

	ON_THREAD_MESSAGE (WM_GET_DATA, OnGetData)
	ON_THREAD_MESSAGE (WM_STOP_HANDLING, OnStopHandling)
	ON_THREAD_MESSAGE (WM_RELOAD, OnReload)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataProcessor message handlers


CDataProcessor::CDataProcessor(): 
	m_bReload(false), m_bConnected(false), m_evtStopData(FALSE, TRUE),
	m_evtWaiting(TRUE, TRUE), m_evtReady(FALSE, TRUE), m_evtError(FALSE, TRUE),
	m_bUpdateCache(true), m_bInitial(true), m_ErrorCount(0),
	m_bGotPrices(false), m_bGotRanges(false),
	m_nSleepDuration(0), m_nSleepFrequency(1), m_nUpdatesCount(0), m_PriceProviderType(-1), m_lTradeSeqNum(-1)
{
	m_bAutoDelete = false;
}


CDataProcessor::~CDataProcessor()
{
}


// Calculates midmarket value
double CDataProcessor::GetMidMarket (double dBid, double dAsk, double dLast )
{
	double dMidMarket =0;
	if(fNEQZero(dAsk) && fNEQZero(dBid))
		dMidMarket = (dBid + dAsk) / 2.0;
	else
	{
		if(fEQZero(dAsk) || fEQZero(dBid))
		{
			if(fNEQZero(dLast))
				dMidMarket = dLast;
			else
			{
				if(fEQZero(dAsk))
					dMidMarket = dBid;
				else
					dMidMarket = dAsk;
			}
		}			
	}

	return dMidMarket;
}

// Checks that processing is stopped
bool CDataProcessor::CheckStop (bool bThrow)
{
	if (m_evtStopData.Lock(0) )
	{
		if (bThrow)
			throw CStopException();
		else
			return true;
	}

	return false;
}

STDMETHODIMP CDataProcessor::OnLastQuote (PP::QuoteUpdateParams *Params, PP::QuoteUpdateInfo *Results)
{
	if (! m_bConnected)
		return S_OK;

	m_ErrorCount = 0;

	try
	{

		UpdatePrice (Params, Results);	
		switch (Params->Type )
		{
		case PP::enSTK:
		case PP::enIDX:
			if ( BeginProcessing ()  )
			{
				--m_lGotUndCounter;
				if(0 == m_lGotUndCounter)
				{

					m_lGotOptCounter = m_optsData.size();
					m_lTotalOptCounter = m_lGotOptCounter;
//					m_spPriceProvider->SubscribeQuote (Params);		///		disable RT

	#ifdef _MULTIPROVIDERS
					if(m_spProviderData->GetIsGroupRequestSupported())					
					{
						if(_bstr_t(Params->Symbol).length())
						{
							if(fEQZero(Results->AskPrice) && fEQZero(Results->BidPrice) && fEQZero(Results->LastPrice) && fEQZero(Results->UpdateDateTime))
							{
								m_lGotOptCounter =0;
								m_bGotPrices = true;
								FinishProcessing();
							}
							else
							{
								Params->Type = PP::enSTK == Params->Type ? PP::enGrSTK : PP::enGrIDX;
								CComQIPtr<PP::IGroupPrice> spPrice = m_spPriceProvider;
								spPrice->RequestGroup(Params, /*PP::enGroupRequestAll*/PP::enGroupRequestLastQuote);
							}
						}
						else
						{
							m_lGotOptCounter =0;
							m_bGotPrices = true;
							FinishProcessing();
						}
					}
					else
	#endif
					{

						PP::QuoteUpdateParams qpp;
						qpp.Type = PP::enMStart;
						qpp.Symbol = SysAllocString(L"");
						qpp.Exchange = SysAllocString(L"");
						m_spPriceProvider->RequestLastQuote(&qpp);

						for (options_map::iterator it = m_optsData.begin(); it !=  m_optsData.end(); it++  )
						{
							if (CheckStop(false))
								break;


							_bstr_t bstrSymbol = it->first.c_str();
							
							PP::QuoteUpdateParams qp;
							qp.Type = PP::enOPT;
							qp.Symbol = bstrSymbol;
							qp.Exchange = NULL;

							m_spPriceProvider->RequestLastQuote(&qp);
						}
						qpp.Type = PP::enMStop;
						qpp.Exchange = NULL;    
						m_spPriceProvider->RequestLastQuote(&qpp);
					    SysFreeString(qpp.Symbol);
						SysFreeString(qpp.Exchange);
					}
				}
				
			EndProcessing();
			}
			break;			
		case PP::enGrSTK:
		case PP::enGrIDX:
			if ( BeginProcessing ()  )
			{
				GotLastQuoteResponse();
				EndProcessing();
			}
			break;

		default: // option
			{
			_bstr_t bsSymbol(Params->Symbol);
#ifndef _MULTIPROVIDERS
			GotQuoteResponse (bsSymbol, true);
#else
			bool bValue =  (m_spProviderData->GetIsGroupRequestSupported()==VARIANT_FALSE);
			GotQuoteResponse (bsSymbol, bValue);
#endif
			}
		}
	}
	catch (_com_error & /*e*/)
	{
		//ErrorNotification (e); 
		SetStatusText("Error getting data...");
		OnStopHandling (0,0);
	}
	catch (CStopException & ) 
	{}
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	return S_OK;
}

STDMETHODIMP CDataProcessor::OnSubscribed(PP::QuoteUpdateParams *Params)
{
	return S_OK;
}
STDMETHODIMP CDataProcessor::OnUnsubscribed()
{
	return S_OK;
}


STDMETHODIMP CDataProcessor::OnQuoteUpdate ()
{
//	if (! m_bConnected)
//		return S_OK;
//
//	try
//	{
//		m_ErrorCount = 0;
//		double dPrice = m_contractData.m_dPrice;
//		_bstr_t Symbol = Params->Symbol;
//
//		UpdatePrice (Params, Results);
//
//		if ( m_opts.m_enCurveMode == enOTM &&  
//			 (Symbol == _bstr_t(m_strSymbol)) &&
//			 (Params->Type  == PP::enSTK || Params->Type  == PP::enIDX) &&
//			 ( fabs(m_contractData.m_dPrice - dPrice)  >  dPrice * m_dPriceDelta) )
//			{
//				OnStopHandling (1,0); // cancel subscription	
//				InitialRequests (true, false);
//			}
//	}
//	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
//
	return S_OK;
}


STDMETHODIMP CDataProcessor::OnError (PP::ErrorNumberEnum ErrorNumber, BSTR Description, PP::RequestsTypeEnum ReqType, VARIANT Request)
{
	if (! m_bConnected)
		return S_OK;

	try
	{
		bool bFatal = false;
		bool bSignalError = false;
		bool bCantGetLastQuote = false;
		
		switch (ErrorNumber)
		{
		case PP::enCouldNotConnectToProvider:
		case PP::enNotConnected:
		case PP::enConnectionWasDisconnected:
			{
				ATLTRACE ( _T("Provider was disconnected.\n"));
				//SetStatusText( CString ("Feed error..."));	// Quiet mode
			}	break;
		case PP::enProviderInternalError:
		case PP::enInternalError:
		{
		ASSERT (FALSE);
		//	bFatal = true;	// Don't allow VM to reconnect to HF itself
		bSignalError = true;
		}
		
		}

		if (! bFatal)
		{

			switch (ReqType)
			{
			case  PP::enRequestLastQuote:
				{
					PP::QuoteUpdateParams * Param = (PP::QuoteUpdateParams *) Request.pvRecord;
					
					if (Param->Type != PP::enOPT)
						bCantGetLastQuote = true;
					else
					{
						_bstr_t bsSymbol(Param->Symbol);
						GotQuoteResponse (bsSymbol, false);
					}
				}
				break;
			case PP::enSubscribeQuote:
				break;
			case PP::enNoRequest:
			{
				break;
			}
			}
		}

		if ( bSignalError && (!m_bGotPrices) )
		{
			CString strErr;
			_bstr_t bstrDeskr (Description);
			strErr.Format ("Error connecting to feed: %s",  (LPCTSTR)bstrDeskr);
			SetStatusText(strErr);
		}

		if ( bCantGetLastQuote)
			SetStatusText( CString ("Got last quotes with errors..."));

		
		
		if ( bFatal)
			PostThreadMessage (WM_STOP_HANDLING, 0, 1);


	}
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	return S_OK;
}


// Called when all responses on last prices requests have received
void CDataProcessor::GotQuoteResponse (_bstr_t& szSymbol, bool bSubscribe)
{
	ASSERT( szSymbol.length() );
	LPCOLESTR pszSymbol = szSymbol;
	options_map::iterator it = m_optsData.find( pszSymbol );
	if( it == m_optsData.end() )
		return;

	try
	{
		if( BeginProcessing() )
		{
			if( bSubscribe )	
				SubscribeOption( it );

			CString str;
			if( m_lTotalOptCounter ) 
			{
				long delta = m_lTotalOptCounter-m_lGotOptCounter;
				double dpersent = double(100.* delta)/m_lTotalOptCounter;
				if(dpersent>100.)
					dpersent = 100;

				if(dpersent >=0 )
					str.Format(_T("Getting the latest data: %.2f%%"), dpersent);
			}
			else 
			{
				str.Format(_T("Getting the latest data: %d"),m_lGotOptCounter);
			}

			SetStatusText( str );
			if ( --m_lGotOptCounter == 0 )
			{
				m_bGotPrices = true;
				FinishProcessing();
			}

			EndProcessing ();
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;

}

void CDataProcessor::GotLastQuoteResponse ()
{
	try
	{
		m_lGotOptCounter = 0;
		m_bGotPrices = true;
		FinishProcessing();
	}
	_CATCH_UNHANDLED_EXCEPTION;

}

HRESULT CDataProcessor::CreatePriceProvider(long type)
{
	HRESULT hr =S_OK;
#ifndef _MULTIPROVIDERS
	try
	{
		hr = m_spPriceProvider.CreateInstance (__uuidof (MT::MyTrackPriceInfo) );
	}
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

#else
	try
	{
		if(m_spProviders==NULL)
		{
			hr = m_spProviders.CreateInstance(__uuidof(PP::Providers));
			if(FAILED(hr))
				return hr;
		}
	}
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;


	try
	{
		m_spProviders->Initialize();
		long lCount = m_spProviders->GetCount();
		m_spProviderData = NULL;
		for(long l = 1; l<=lCount; l++)
		{
			hr = m_spProviders->get_Item(l, &m_spProviderData);
			if(FAILED(hr))
				break;
			if(type == m_spProviderData->GetProviderID())
			{
				break;
			}
			else
				m_spProviderData = NULL;

		}

		if(m_spProviderData!=NULL)
		{
			if(m_spPriceProvider==NULL)
			{
				hr = m_spPriceProvider.CreateInstance (__uuidof (PP::BatchPriceInfo) );
				
				if(SUCCEEDED(hr))
					_IPriceInfoEventsImpl::DispEventAdvise (m_spPriceProvider);
			}

			if(SUCCEEDED(hr))
			{
				PP::IProviderPtr spControl = m_spPriceProvider;
				spControl->PutType(type);
			}
		}	
	}
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;


#endif

	if(SUCCEEDED(hr))
	{
		m_PriceProviderType = type;
	}
	else
	{
		SetStatusText( _T("Error creating price provider instance"));	
		m_PriceProviderType = -1;
	}

return hr;
}

BOOL CDataProcessor::InitInstance()
{
	::CoInitialize(NULL);

	DWORD dwID = ::GetCurrentThreadId ();
	ATLTRACE ( _T("Data thread:  %x\n"), dwID);
	
	HRESULT hr = S_OK;

	m_ivc.Init ();

	CIVAnalyzerOptions opts;
	theApp.GetOptions( opts );

#ifdef _VOLA_MANAGER

	try
	{
		_com_util::CheckError( m_spVolaManagement.CreateInstance( VME::CLSID_VolatilityManagement ) );
		
		
		_com_util::CheckError( m_spDBManagement.CreateInstance(__uuidof(Stg::Settings)));
		_com_util::CheckError( m_spIVManagement.CreateInstance(__uuidof(Stg::IVSettings)));
		_com_util::CheckError( m_spTradeSubscriber.CreateInstance(__uuidof(MMW::MsgManager)));


		_com_util::CheckError( _IVolatilityManagementEventsImpl::DispEventAdvise( m_spVolaManagement ) );
		_com_util::CheckError( _IDBSettingsEventsImpl::DispEventAdvise( m_spDBManagement ) );
		_com_util::CheckError( _IIVSettingsEventsImpl::DispEventAdvise( m_spIVManagement ) );
		_com_util::CheckError( _IMMWOnMessagesEventImpl::DispEventAdvise( m_spTradeSubscriber ) );
		_com_util::CheckError( ::CoMarshalInterThreadInterfaceInStream (VME::IID_IVolatilityManagement, m_spVolaManagement,
			&m_spVolaMgmtStream ) );

		_com_util::CheckError(m_spTradeSubscriber->SubTradeUpdate());
	}
	catch( const _com_error& e )
	{
		//ErrorNotification( e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage(), true, true ); 
		SetStatusText(e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage());
		return FALSE;
	}
	_CATCH_UNHANDLED_EXCEPTION;

#endif //_VOLA_MANAGER

	if(FAILED(CreatePriceProvider(opts.m_enProviderType)))
		return FALSE;

	m_nSleepDuration  = opts.m_nSleepDuration;
	m_nSleepFrequency = opts.m_nSleepFrequency;

	if(SUCCEEDED(m_spControl.CreateInstance(__uuidof(IVM::IVControl))))
	{
		_IIVControlImpl::DispEventAdvise (m_spControl);
	}
	m_hStartMutex =  CreateMutex(NULL,FALSE,_T("{09F19211-F6A4-481f-B7DF-5F02FBAB56F9}"));



	SetReady();
	

	m_evtReady.SetEvent();

	return TRUE;
}

//  Called to start data processing. nID is identifier from database table
bool CDataProcessor::GetData( LPCTSTR szSymbol, int nID, months_container& mnths, bool bInitial )
{
	if (!m_evtReady.Lock(0))
		return false;
	
	try
	{
		if(!bInitial)
			CancelData();

		m_time_skewData.clear();
		m_contractData.Clear();
		m_PriceData.clear();
		m_volaBandsData.clear();
		m_optsData.clear();
		
		m_evtError.ResetEvent();	

		m_csSymbol.Lock();
		m_strSymbol = szSymbol;
		m_csSymbol.Unlock();
		
		m_contractData.nID = nID;

		m_csSynthUndList.Lock();
		m_SynthUndList.clear();
		m_csSynthUndList.Unlock();

		theApp.GetOptions(m_opts);
	}
	_CATCH_UNHANDLED_EXCEPTION;


	try
	{
#ifdef _VOLA_MANAGER
		if( !InitializeVolaManager( m_opts.m_strConnection ) )
			return false;
#endif //_VOLA_MANAGER

		LoadStructure( nID );

		if (m_optsData.size() == 0)
		{
			if(nID>0)
				SetStatusText( _T("No options for the selected contract"));
			return false;
		}
		
		LoadContractParams( nID );

		if(m_contractData.m_bIsIndex)
			LoadBasketIndexDividends(nID,m_contractData.m_Divs);

		LoadRates();

		for( options_map::iterator it = m_optsData.begin(); it != m_optsData.end(); it++ )
		{
			COleDateTime dtExp = it->second.m_dtExpiryOV;
			//COleDateTime dtMonth ( dtExp.GetYear(), dtExp.GetMonth(), 1, 0, 0, 0);

			mnths.addMonth( dtExp );
		}
	}
	catch(_com_error & e)
	{
		//ErrorNotification (e);
		SetStatusText(e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage());
		return false;
	}
	_CATCH_UNHANDLED_EXCEPTION;


	if( m_optsData.size() > 0 )
	{
		PostThreadMessage( WM_GET_DATA, 0, (LPARAM) nID );

		return true;
	}
	else
		return false;
}
// Cancels data processing
void CDataProcessor::CancelData()
{
	if (!m_evtReady.Lock(0))
		return;

	try
	{
		m_evtStopData.SetEvent();
		m_evtWaiting.Lock();
		
		m_evtHandlingStopped.ResetEvent();
		
		PostThreadMessage (WM_STOP_HANDLING, 0, 0);
		
		m_evtHandlingStopped.Lock();
		
		Sleep(500); // Waiting event handlers to complete

		SetReady();
	}
	_CATCH_UNHANDLED_EXCEPTION;

}



int CDataProcessor::ExitInstance()
{	
	try
	{
		m_ivc.Term();

	#ifdef _VOLA_MANAGER
		if( m_spVolaManagement != NULL )
		{
			_IVolatilityManagementEventsImpl::DispEventUnadvise( m_spVolaManagement );
			m_spVolaManagement->UnregisterPublisher();

			m_spVolaMgmtStream = NULL;
			m_spVolaManagement = NULL;
		}

		if(m_spDBManagement!=NULL)
		{
			_IDBSettingsEventsImpl::DispEventUnadvise( m_spDBManagement );
			m_spDBManagement = NULL;
		}
		if(m_spIVManagement!=NULL)
		{
			 _IIVSettingsEventsImpl::DispEventUnadvise( m_spIVManagement );
			m_spIVManagement = NULL;
		}

		if(m_spTradeSubscriber!=NULL)
		{
			m_spTradeSubscriber->UnsubTradeUpdate();
			_IMMWOnMessagesEventImpl::DispEventUnadvise( m_spTradeSubscriber );
			m_spTradeSubscriber = NULL;
		}

	#endif //_VOLA_MANAGER

		m_spProviderData = NULL;
		if (m_spPriceProvider != NULL)
		{
			if (m_bConnected)
			{
				_IPriceInfoEventsImpl::DispEventUnadvise (m_spPriceProvider);
				m_spPriceProvider->raw_Disconnect();	
				m_bConnected = false;
			}
			m_spPriceProvider = NULL;
		}

		CloseHandle(m_hStartMutex);
		if(m_spControl!=NULL)
		{
			_IIVControlImpl::DispEventUnadvise(m_spControl);
		}


		m_dbCache.Disconnect();

		CoUninitialize();
	}
	_CATCH_UNHANDLED_EXCEPTION;

	return CWinThread::ExitInstance();
}


int CDataProcessor::Run() 
{
	return CWinThread::Run();
}

void CDataProcessor::OnGetData (WPARAM wParam, LPARAM lParam)
{
	try
	{
		m_evtStopData.ResetEvent();
		
		m_lTotalOptCounter = m_lGotOptCounter = m_optsData.size();
		m_lGotUndCounter = m_SynthUndList.size() + 1;
		m_bUpdateCache = true;
		
		m_bGotPrices = false;
		m_bGotRanges = false;

		InitialRequests ();
	}
	_CATCH_UNHANDLED_EXCEPTION;

	return;
}

// Loads market structure
void CDataProcessor::LoadStructure (int nID)
{
	try
	{
		m_optsData.clear();

		CDBLayout db;
		COptionRootData* cpRoot = NULL;
		
		db.Connect();
		CClientRecordset rs;

		db.GetOptions(nID, m_opts.m_enCurveMode,  rs);
		for( ;!rs.IsEOF(); ++rs )
		{
			COptionData opt;
			opt.m_dtExpiration = (long) rs[L"dtExpDate"];
			opt.m_dStrike = rs[L"fStrike"];
			opt.m_bIsCall = rs[L"bIsCall"].GetValue();
			opt.m_lOptionRootID =  rs[L"iOptionRootID"];
			opt.m_bIsFitEnabled =   (long(rs[L"tiIsFitEnabled"]))!=0;
			opt.m_dtExpiryOV = (double) rs[L"dtExpiryOV"];
			opt.m_dtTradingClose = (double) rs[L"dtTradingClose"] - 2;

	#ifdef _IVAN_DB_ACCESS
			opt.m_lOptionID = rs[L"iOptionID"];
			opt.m_dVola = rs[L"fIV"];
			opt.m_dPrice = rs[L"fPriceMid"];
			opt.m_dtPriceDate = rs[L"dtDate"];
	#endif 

			_bstr_t bsName =  rs[L"vcSymbolName"]; 
			_tcsncpy_s(opt.m_strName, sizeof(opt.m_strName),(LPCTSTR)bsName, sizeof(opt.m_strName));

			m_optsData [(LPOLESTR)bsName ] = opt;

			if(m_contractData.m_mapOptionRoots.find(opt.m_lOptionRootID) == m_contractData.m_mapOptionRoots.end())
			{
				bool bIsSynthetic = (bool)rs[L"tiIsSynthetic"].GetValue();

				if(!bIsSynthetic)
				{
					COptionRootData* pRoot = new COptionRootData();
					pRoot->m_bIsSynthetic = false;
					pRoot->m_lOptionRootID = opt.m_lOptionRootID;
					pRoot->m_lLotSize = rs[L"iLotSize"];
					pRoot->m_bIsFitEnabled =  opt.m_bIsFitEnabled ;

					m_contractData.m_mapOptionRoots.insert(std::make_pair(pRoot->m_lOptionRootID,pRoot));
				}
				else
				{
					//Skip Syntetic
					continue;
					CSyntheticOptionRootData* pSynthRoot = new CSyntheticOptionRootData();
					pSynthRoot->m_bIsSynthetic = true;
					pSynthRoot->m_lOptionRootID = opt.m_lOptionRootID;
					pSynthRoot->m_lLotSize = rs[L"iLotSize"];

					LoadSyntheticRoot(pSynthRoot->m_lOptionRootID,pSynthRoot);

					m_contractData.m_mapOptionRoots.insert(std::make_pair(pSynthRoot->m_lOptionRootID,(COptionRootData*)pSynthRoot));
				}
			}
			cpRoot = m_contractData.m_mapOptionRoots[opt.m_lOptionRootID];
			
			cpRoot->m_Options.insert(std::make_pair((LPOLESTR)bsName,&(m_optsData[(LPOLESTR)bsName])));

		}
		rs.Close();

		// Loading Cache 
		
		m_contractData.m_dPrice = db.GetContractMarketPrice( nID );
		sUndPrice PriceData;
		PriceData.m_fLast = m_contractData.m_dPrice;
		PriceData.m_iID = nID;
		m_PriceData[nID] = PriceData;

		if(!m_SynthUndList.empty())
		{
			for (synth_underl_map::iterator it = m_SynthUndList.begin(); it != m_SynthUndList.end();it++)
			{
				it->second.m_dPrice  = db.GetContractMarketPrice( it->second.nID );
				PriceData.m_fLast = it->second.m_dPrice;
				PriceData.m_iID = it->second.nID;
				m_PriceData[PriceData.m_iID] = PriceData;

	#ifdef _VOLA_MANAGER
				GetVolaManager()->UnderlinePrice[_bstr_t(it->first.c_str())] = it->second.m_dPrice;
	#endif	

			}
		}

	#ifdef _VOLA_MANAGER
		GetVolaManager()->UnderlinePrice[_bstr_t(GetContractSymbol())] = m_contractData.m_dPrice;
	#endif	

	#ifndef _IVAN_DB_ACCESS
		db.GetOptionsDataCache (nID, rs, m_opts.m_enCurveMode);

		for (; !rs.IsEOF(); ++rs)
		{
			options_map::iterator it = m_optsData.find ( (BSTR) (_bstr_t) rs[L"vcOptionTicker"] );
			if ( it != m_optsData.end() )
			{
				COptionData & opt = it->second;
				opt.m_dVola = rs[L"fIV"];
				opt.m_dPrice = rs[L"fPriceMid"];
				opt.m_dtPriceDate = rs[L"dtDate"];
			}
		}
		rs.Close();
	#endif	
		CalcTimeSkew();

		db.VolaBandsDataGet(nID,rs);
		for (; !rs.IsEOF(); ++rs)
		{
			double dExpPer = rs[L"iExpPeriod"];
			
			CVolaBandsData data;

			data.m_dIVBandHigh = rs[L"fIVBandHi"];
			data.m_dIVBandLow = rs[L"fIVBandLow"];
			data.m_dStrikeMoneyness = rs[L"fStrikeMoneyness"];
			
			m_volaBandsData.insert(std::make_pair ((long) dExpPer, data));
		}

		db.Disconnect();
	}
	_CATCH_UNHANDLED_EXCEPTION;

}

#ifdef _VOLA_MANAGER
bool CDataProcessor::InitializeVolaManager( LPCTSTR szConnection )
{
	bool bRes = true;
	try
	{
		IVADBLayoutPtr spDS = GetVolaManager()->DataSource;
		if( spDS.GetInterfacePtr() != NULL && spDS->ConnectionString == _bstr_t( szConnection ) )
		{
		}
		else
		{
			spDS = NULL;
			_com_util::CheckError( spDS.CreateInstance( __uuidof(DBLayout) ) );
			spDS->ConnectionString = szConnection;

			IVAManagement *pManagement;
			_com_util::CheckError( spDS->QueryInterface( __uuidof(IVAManagement), (void**)&pManagement ) );

			GetVolaManager()->DataSource = pManagement;
		}
	}
	catch( const _com_error& e )
	{
		CString message;
		message.Format( _T("Error providing volatility manager with data source. (%X) "), e.Error() );
		message += e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage();
		//ErrorNotification( message, true, true );
		SetStatusText( _T("Error providing volatility manager with data source..."));

		bRes =  false;
	}
	_CATCH_UNHANDLED_EXCEPTION;


	return bRes;
}
#endif //_VOLA_MANAGER


// Error notification sent to the main window
void CDataProcessor::ErrorNotification (LPCTSTR szError, bool bFatal, bool bTerminate  )
{
	try
	{

		if( bFatal )
			SetErrorState();

		m_csError.Lock();
		m_strError = szError;
		m_csError.Unlock();

		AfxGetMainWnd()->PostMessage( WM_ERROR, MAKELONG( bFatal ? 1: 0, bTerminate ? 1 : 0 ), NULL );
	}
	_CATCH_UNHANDLED_EXCEPTION;
}

void CDataProcessor::ErrorNotification (_com_error & e, bool bFatal)
{
	try
	{
		CString strError = e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage();
		ErrorNotification (strError, bFatal);
	}
	_CATCH_UNHANDLED_EXCEPTION;
}


void CDataProcessor::SetReady()
{
	SetStatusText( CString( (LPCSTR)IDS_READY ) );
}

CString CDataProcessor::GetStatusText()
{
	CString strResult;
	
	m_csStatus.Lock();
	strResult = m_strStatus;
	m_csStatus.Unlock();

	return strResult;
}

CString CDataProcessor::GetErrorText()
{
	CString strResult;
	
	m_csError.Lock();
	strResult = m_strError;
	m_csError.Unlock();

	return strResult;
}

// Subscription to the last prices and realtime processing
void CDataProcessor::InitialRequests(bool bPrices, bool bRanges)
{
	if (BeginProcessing())
	{
		try
		{	
			if (bPrices)
			{
				m_bGotPrices = false;
				
				if ( (m_oldPriceOpts != m_opts.m_PriceProvOpts && m_bConnected) || m_bReload) 
				{
					m_spPriceProvider->raw_Disconnect();
					m_bConnected = false;
					if(m_bReload)
					{
#ifdef _MULTIPROVIDERS
						if(m_spProviderData->GetIsGroupRequestSupported())
						{
							CComQIPtr<PP::IGroupPrice> spPrice = m_spPriceProvider;
							if(spPrice!=NULL)
							{
								spPrice->raw_CancelGroup();
							}
						}
#endif
						CreatePriceProvider(m_opts.m_enProviderType);					
						m_bReload = false;
					}
				}	
				
				if (! m_bConnected)
				{
					SetStatusText (_T("Connecting to the price provider"));
					m_spPriceProvider->Connect();
					m_bConnected = true;

					m_oldPriceOpts = m_opts.m_PriceProvOpts;
				}

				AfxGetMainWnd()->PostMessage (WM_ONLINE);
				
				SetRequestsStatusText (bPrices, bRanges);

				PP::QuoteUpdateParams qp;

				_bstr_t bstrSymbol = GetContractSymbol();
				qp.Symbol = bstrSymbol;
				qp.Exchange = NULL;

				qp.Type = PP::enMStart;
				m_spPriceProvider->RequestLastQuote(&qp);

				if ( m_contractData.m_bIsIndex )
					qp.Type = PP::enIDX;
				else
					qp.Type = PP::enSTK;


				m_spPriceProvider->RequestLastQuote(&qp);

				for (synth_underl_map::iterator it = m_SynthUndList.begin(); it != m_SynthUndList.end();it++)
				{
					bstrSymbol = it->first.c_str();
					qp.Symbol = bstrSymbol;
					qp.Exchange = NULL;

					if ( it->second.m_bIsIndex )
						qp.Type = PP::enIDX;
					else
						qp.Type = PP::enSTK;

					m_spPriceProvider->RequestLastQuote (&qp);
				}
				qp.Type = PP::enMStop;
				m_spPriceProvider->RequestLastQuote(&qp);

			}

			FinishProcessing();

		}
		catch (_com_error & e)
		{
			//ErrorNotification (e);		
			SetStatusText(e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage());
			OnStopHandling (0,0);
		}
		catch (CStopException &  )
		{
			OnStopHandling (0,0);
		}
		_CATCH_UNHANDLED_EXCEPTION;

		EndProcessing();
	}
}

// recalculates volatility and time skew data
bool CDataProcessor::RecalcOptions(CString strSymbol)
{
	bool bChanged = false;

	try
	{

		m_csSymbol.Lock();
		if(strSymbol.IsEmpty()) strSymbol = m_strSymbol;
		m_csSymbol.Unlock();

		CalcTimeSkew();

		if(strSymbol == m_strSymbol)
		{
		
			for (options_map::iterator it = m_optsData.begin(); it != m_optsData.end(); it++ )
			{
				CheckStop();

				CalcIV( it->second );

				if( it->second.m_bIsChanged )
				{
					it->second.m_dtPriceDate = COleDateTime::GetCurrentTime();
					bChanged = true;
				}
			}

		}
		else
		{
			USES_CONVERSION;
			CContractData& contr = m_SynthUndList[T2W((LPCTSTR)strSymbol)];
			
			for(option_roots_map::iterator it1 = contr.m_mapRefSynthOptRoots.begin(); it1 != contr.m_mapRefSynthOptRoots.end(); it1++)
			{
				COptionRootData* pRoot = it1->second;

				for (root_options_map::iterator it2 = pRoot->m_Options.begin(); it2 != pRoot->m_Options.end(); it2++ )
				{
					CheckStop();

					CalcIV( *(it2->second) );

					if( it2->second->m_bIsChanged )
					{
						it2->second->m_dtPriceDate = COleDateTime::GetCurrentTime();
						bChanged = true;
					}
				}
			}
			
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;

	return bChanged;
}


// Calculates implied volatility
bool CDataProcessor::CalcIV( COptionData & opt )
{
	LPSAFEARRAY psaDates = NULL;
	LPSAFEARRAY psaAmounts  = NULL ;
	DOUBLE * pdDates = NULL;	
	DOUBLE * pdAmounts = NULL;
	LPVOID lpDateData = NULL;
	LPVOID lpAmountData = NULL;
	double*   pDivAmts = NULL;
	double*   pDivYears = NULL;

	bool   bRetVal = true;
	try
	{
	
		double dVola   = 0;


		if( !fEQZero( opt.m_dPrice ))
		{
			COptionRootData* pRoot = NULL;

			if(m_contractData.m_mapOptionRoots.find(opt.m_lOptionRootID) != m_contractData.m_mapOptionRoots.end())
				pRoot = m_contractData.m_mapOptionRoots[opt.m_lOptionRootID];
			
			DATE	dtNow		= GetNewYorkTime();
			DATE	dtExpiryOV	= opt.m_dtExpiryOV;
			DATE	dtCloseTime = opt.m_dtTradingClose;
			DOUBLE	dYTE		= opt.GetYTE();

			double dRate = CalcRates(dYTE);

			if(pRoot && !pRoot->m_bIsSynthetic && !fEQZero( m_contractData.m_dPrice ) )
			{
				
				long nDivCount = 0;
				long      nRetCount = 0;

				if(m_contractData.m_bIsIndex && m_contractData.m_Divs!=NULL)
				{
					m_contractData.m_Divs->GetDividendCount2(dtNow, dtExpiryOV, dtCloseTime, &nDivCount);
					//m_contractData.m_Divs->GetDividendCount((long)dtToday, nDTE+(long)dtToday, &nDivCount);				

					if (nDivCount > 0)
					{
						//m_contractData.m_Divs->GetDividends((long)dtToday, nDTE+(long)dtToday, nDivCount, &psaAmounts, &psaDates, &nRetCount);
						m_contractData.m_Divs->GetDividends2(dtNow, dtExpiryOV, dtCloseTime, nDivCount, &psaAmounts, &psaDates, &nRetCount);
						::SafeArrayLock(psaDates);
						::SafeArrayLock(psaAmounts);
						::SafeArrayAccessData(psaDates, &lpDateData);
						::SafeArrayAccessData(psaAmounts, &lpAmountData);
						pDivYears =	reinterpret_cast<DOUBLE*>(lpDateData);
						pDivAmts  = reinterpret_cast<DOUBLE*>(lpAmountData);
						
						m_contractData.m_dYield = 0.0f;
					}

				}
				else
				{
					if (m_contractData.m_spDividend != NULL)
					{
						nDivCount = 0;
						//m_contractData.m_spDividend->GetDividendCount((long)dtToday, nDTE+(long)dtToday, &nDivCount);
						m_contractData.m_spDividend->GetDividendCount2(dtNow, dtExpiryOV, dtCloseTime, &nDivCount);
						if (nDivCount< 0)
							nDivCount = 0;

						if (nDivCount> 0)
						{
							//m_contractData.m_spDividend->GetDividends((long)dtToday, nDTE+(long)dtToday, nDivCount, &psaAmounts, &psaDates, &nRetCount);
							m_contractData.m_spDividend->GetDividends2(dtNow, dtExpiryOV, dtCloseTime, nDivCount, &psaAmounts, &psaDates, &nRetCount);
							::SafeArrayLock(psaDates);
							::SafeArrayLock(psaAmounts);
							::SafeArrayAccessData(psaDates, &lpDateData);
							::SafeArrayAccessData(psaAmounts, &lpAmountData);
							pDivYears =	reinterpret_cast<DOUBLE*>(lpDateData);
							pDivAmts  = reinterpret_cast<DOUBLE*>(lpAmountData);
						}
					}
				}

				
				long lPrModel = MM_EGAR_BINOMIAL;
				
				switch(m_opts.m_enCalculationMode)
				{
				case enBinominal:
					lPrModel = MM_EGAR_BINOMIAL;
					break;
				case enOptBinominal:
					lPrModel = MM_EGAR_OPTIMIZED;
					break;
				case enVSKLog:
					lPrModel = MM_EGAR_VSKLOG;
					break;
				default:
					lPrModel = MM_EGAR_BINOMIAL;
					break;
				}

				dVola = CalcVolatilityMM2 (dRate, m_contractData.m_dYield, BAD_DOUBLE_VALUE, m_contractData.m_dPrice, opt.m_dPrice, opt.m_dStrike, dYTE, 
					opt.m_bIsCall, m_contractData.m_bIsAmerican, nDivCount, pDivAmts, pDivYears, 100, 
					m_contractData.m_dSkew, m_contractData.m_dKurt, lPrModel );

				if( dVola < VOLA_MIN || dVola > VOLA_MAX ) 
				{
					dVola = 0;
					bRetVal = false;
				}
			}
			else if(pRoot) 
			{

				double dPrice = 0.0f;

				CSyntheticOptionRootData* pSynthRoot = (CSyntheticOptionRootData*)pRoot;
				long nDivCount = 0;

				dPrice = pSynthRoot->m_dCashAmount;

				for (synthunderl_param_map::iterator it = pSynthRoot->m_mapSynthRootComponents.begin(); it != pSynthRoot->m_mapSynthRootComponents.end();it++)
				{
					sUndPrice PriceData;

					if(m_PriceData.find((it->second).m_pUnderlying->nID) == m_PriceData.end())
						continue;

					PriceData = m_PriceData[(it->second).m_pUnderlying->nID];

					dPrice += it->second.m_dWeight * GetMidMarket(PriceData.m_fBid,PriceData.m_fAsk,PriceData.m_fLast);
				}

				if(dPrice <= 0.0f)
					return false;


				if(pSynthRoot->m_bIsBasket && pSynthRoot->m_Divs != NULL)
				{
					//pSynthRoot->m_Divs->GetDividendCount((long)dtToday, nDTE+(long)dtToday, &nDivCount);				
					pSynthRoot->m_Divs->GetDividendCount2(dtNow, dtExpiryOV, dtCloseTime, &nDivCount);				
					if (nDivCount > 0)
					{
						//pSynthRoot->m_Divs->GetDividends((long)dtToday, nDTE+(long)dtToday, nDivCount, &psaAmounts, &psaDates, &nDivCount);
						pSynthRoot->m_Divs->GetDividends2(dtNow, dtExpiryOV, dtCloseTime, nDivCount, &psaAmounts, &psaDates, &nDivCount);
						::SafeArrayLock(psaDates);
						::SafeArrayLock(psaAmounts);
						::SafeArrayAccessData(psaDates, &lpDateData);
						::SafeArrayAccessData(psaAmounts, &lpAmountData);
						pDivYears =	reinterpret_cast<DOUBLE*>(lpDateData);
						pDivAmts  = reinterpret_cast<DOUBLE*>(lpAmountData);

						m_contractData.m_dYield = 0.0f;
					}

				}

				long lPrModel = MM_EGAR_BINOMIAL;
				
				switch(m_opts.m_enCalculationMode)
				{
				case enBinominal:
					lPrModel = MM_EGAR_BINOMIAL;
					break;
				case enOptBinominal:
					lPrModel = MM_EGAR_OPTIMIZED;
					break;
				case enVSKLog:
					lPrModel = MM_EGAR_VSKLOG;
					break;
				default:
					lPrModel = MM_EGAR_BINOMIAL;
					break;
				}			

				dVola = CalcVolatilityMM2 (dRate, pSynthRoot->m_dYield, BAD_DOUBLE_VALUE, dPrice, opt.m_dPrice, opt.m_dStrike, dYTE, 
					opt.m_bIsCall, m_contractData.m_bIsAmerican, nDivCount, pDivAmts, pDivYears, 100, 
					pSynthRoot->m_dSkew,pSynthRoot->m_dKurt, lPrModel );

				if( dVola < VOLA_MIN || dVola > VOLA_MAX ) 
				{
					dVola = 0;
					bRetVal = false;
				}

			}

		}
		
		opt.m_bIsChanged = false;
				
		m_csVola.Lock();
		double dDelta = fabs(opt.m_dVola - dVola);

		if( dDelta > 0 /*0.01*/ )
		{
			opt.m_bIsChanged = true;
			opt.m_dVola = dVola;
		}
		m_csVola.Unlock();
	}
	_CATCH_UNHANDLED_EXCEPTION;

	if (psaAmounts !=NULL)
	{
		::SafeArrayUnaccessData(psaAmounts);
		::SafeArrayUnlock(psaAmounts);
		::SafeArrayDestroy(psaAmounts);
	}

	if (psaDates !=NULL)
	{
		::SafeArrayUnaccessData(psaDates);
		::SafeArrayUnlock(psaDates);
		::SafeArrayDestroy(psaDates);
	}

	return bRetVal;	
}


void CDataProcessor::CalcOptPrice( COptionData& opt )
{
	LPSAFEARRAY psaDates = NULL;
	LPSAFEARRAY psaAmounts = NULL;
	DOUBLE * pdDates = NULL;	
	DOUBLE * pdAmounts = NULL;
	LPVOID lpDateData = NULL;
	LPVOID lpAmountData = NULL;

	try
	{
	
		CLockReload lock;

		LONG nDivCount = 0L, nRetCount = 0L;
		DOUBLE* pdDivDte = NULL;
		DOUBLE* pdDivAmt = NULL;

		// Get DTE
		//DATE dtToday = COleDateTime::GetCurrentTime();
		//int nDTE	 = opt.GetDTE();
		DATE	dtNow		=	GetNewYorkTime();
		DATE	dtCloseTime =	opt.m_dtTradingClose;
		DATE	dtExpiryOV	=	opt.m_dtExpiryOV;
		DOUBLE	dYTE		=	opt.GetYTE();

		// Get rates
		double dRate = CalcRates(dYTE);

		// Get dividends
		DIVIDEND * pDiv = NULL;

        if (m_contractData.m_spDividend != NULL)			
		{
			//m_contractData.m_spDividend->GetDividendCount((long)dtToday, ((long)dtToday)+nDTE, &nDivCount);
			m_contractData.m_spDividend->GetDividendCount2(dtNow, dtExpiryOV, dtCloseTime, &nDivCount);
			if(nDivCount > 0L) 
			{
				//spDiv->GetDividends(nToday, nExpiry, nDivCount, pdDivAmt, pdDivDte, &nRetCount);
				//m_contractData.m_spDividend->GetDividends((long)dtToday, ((long)dtToday)+nDTE, nDivCount, &psaAmounts, &psaDates, &nRetCount);
				m_contractData.m_spDividend->GetDividends2(dtNow, dtExpiryOV, dtCloseTime, nDivCount, &psaAmounts, &psaDates, &nRetCount);
				::SafeArrayLock(psaAmounts);
				::SafeArrayLock(psaDates);

				::SafeArrayAccessData(psaDates, &lpDateData);
				::SafeArrayAccessData(psaAmounts, &lpAmountData);
				pdDivDte	=	 reinterpret_cast<DOUBLE *>(lpDateData);
				pdDivAmt	= reinterpret_cast<DOUBLE *>(lpAmountData);
			}

		}


		// Calc price
		GREEKS g;
		g.nMask = GT_THEOPRICE;
		long lPrModel = MM_EGAR_BINOMIAL;

		switch(m_opts.m_enCalculationMode)
		{
		case enBinominal:
			lPrModel = MM_EGAR_BINOMIAL;
			break;
		case enOptBinominal:
			lPrModel = MM_EGAR_OPTIMIZED;
			break;
		case enVSKLog:
			lPrModel = MM_EGAR_VSKLOG;
			break;
		default:
			lPrModel = MM_EGAR_BINOMIAL;
			break;
		}

		CalcGreeksMM2( dRate, 
					   m_contractData.m_dYield, BAD_DOUBLE_VALUE,
					   m_contractData.m_dPrice, 
					   opt.m_dStrike,
					   opt.m_dVola, 
					   dYTE,
					   opt.m_bIsCall, TRUE, nDivCount, pdDivAmt, pdDivDte, 100, m_contractData.m_dSkew, m_contractData.m_dKurt, lPrModel, &g );
		opt.m_dPrice = g.dTheoPrice;

		if (pDiv)
			delete pDiv;
	}
	_CATCH_UNHANDLED_EXCEPTION;

	if (psaAmounts !=NULL)
	{
		::SafeArrayUnaccessData(psaAmounts);
		::SafeArrayUnlock(psaAmounts);
		::SafeArrayDestroy(psaAmounts);
	}

	if (psaDates !=NULL)
	{
		::SafeArrayUnaccessData(psaDates);
		::SafeArrayUnlock(psaDates);
		::SafeArrayDestroy(psaDates);
	}


}


void CDataProcessor::CalcOptIVola( COptionData& opt )
{

	try
	{
		CLockReload lock;
		LPSAFEARRAY psaDates = NULL;
		LPSAFEARRAY psaAmounts = NULL;
		DOUBLE * pdDates = NULL;	
		DOUBLE * pdAmounts = NULL;
		LPVOID lpDateData = NULL;
		LPVOID lpAmountData = NULL;
		LONG nDivCount = 0L, nRetCount = 0L;
		DOUBLE* pdDivDte = NULL;
		DOUBLE* pdDivAmt = NULL;

		// Get DTE
		//DATE dtToday = COleDateTime::GetCurrentTime();
		//int nDTE	 = opt.GetDTE();
		DATE	dtNow	=	GetNewYorkTime();
		DATE	dtExpiryOV	=	opt.m_dtExpiryOV;
		DATE	dtCloseTime =	opt.m_dtTradingClose;
		DOUBLE	dYTE = opt.GetYTE();

		// Get rates
		double dShortRate = 0;
		if( m_ratesAsk.size() > 0 )
			dShortRate = InterpolateRates( m_ratesAsk.size(),  &m_ratesAsk[0], dYTE );
		double dLongRate = 0;
		if( m_ratesBid.size() > 0 )
			dLongRate = InterpolateRates( m_ratesBid.size(),  &m_ratesBid[0], dYTE );
		double dRate = (dShortRate + dLongRate)/2;

		// Get dividends
		if (m_contractData.m_spDividend != NULL)
		{
			//m_contractData.m_spDividend->GetDividendCount((long)dtToday, nDTE+(long)dtToday, &nDivCount);
			m_contractData.m_spDividend->GetDividendCount2(dtNow, dtExpiryOV, dtCloseTime, &nDivCount);
			if(nDivCount > 0L) 
			{
				//spDiv->GetDividends(nToday, nExpiry, nDivCount, pdDivAmt, pdDivDte, &nRetCount);
				//m_contractData.m_spDividend->GetDividends((long)dtToday, nDTE+(long)dtToday, nDivCount, &psaAmounts, &psaDates, &nRetCount);
				m_contractData.m_spDividend->GetDividends2(dtNow, dtExpiryOV, dtCloseTime, nDivCount, &psaAmounts, &psaDates, &nRetCount);
				::SafeArrayLock(psaAmounts);
				::SafeArrayLock(psaDates);

				::SafeArrayAccessData(psaDates, &lpDateData);
				::SafeArrayAccessData(psaAmounts, &lpAmountData);
				pdDivDte	 =	 reinterpret_cast<DOUBLE *>(lpDateData);
				pdDivAmt	= reinterpret_cast<DOUBLE *>(lpAmountData);

			}
		}



		// Calc ivola
		LONG nFlag = VF_OK;

		double dVola = CalcVolatilityMM3 (dRate, m_contractData.m_dYield, BAD_DOUBLE_VALUE, m_contractData.m_dPrice, opt.m_dPrice, opt.m_dStrike, dYTE, 
			opt.m_bIsCall , TRUE, nDivCount, pdDivAmt,pdDivDte, 100, m_contractData.m_dSkew, m_contractData.m_dKurt, 0, &nFlag); 

		opt.m_dVola = dVola > 0 ? dVola : 0;

		if (psaAmounts !=NULL)
		{
			::SafeArrayUnaccessData(psaAmounts);
			::SafeArrayUnlock(psaAmounts);
			::SafeArrayDestroy(psaAmounts);
		}

		if (psaDates !=NULL)
		{
			::SafeArrayUnaccessData(psaDates);
			::SafeArrayUnlock(psaDates);
			::SafeArrayDestroy(psaDates);
		}

	}
	_CATCH_UNHANDLED_EXCEPTION;

}


// Notifies main window that vola has changed
void CDataProcessor::VolaChangedNotification (const COptionData & opt)
{
	try
	{
		bool bTSChanged = false;

		COleDateTime dtExp = opt.m_dtExpiryOV;

		time_skew_map ::iterator it = m_time_skewData.find (CExpDate((DATE)dtExp));
		if( it != m_time_skewData.end() )
			bTSChanged = it->second.m_pOption == &opt;
				
		theApp.GetVolaNotificationClient()->VolaChangedNotification( bTSChanged, (long)dtExp );
	}
	_CATCH_UNHANDLED_EXCEPTION;

}

void CDataProcessor::UnderlyingChangedNotification(int iID, double fBidPrice, double fAskPrice, double fLastPrice)
{	
	try
	{
		if(m_PriceData.find(iID) == m_PriceData.end() )
		{
			sUndPrice PriceData;
			PriceData.m_iID = iID ;
			PriceData.m_fBid = fBidPrice;
			PriceData.m_fAsk = fAskPrice;
			PriceData.m_fLast = fLastPrice;
			m_PriceData[iID] = PriceData;
		}
		else
		{
			m_PriceData[iID].m_fBid = fBidPrice;
			m_PriceData[iID].m_fAsk = fAskPrice;
			m_PriceData[iID].m_fLast = fLastPrice;
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;

}


void CDataProcessor::VolaChangedNotification()
{
	try
	{
		CVolaNotificationClient *pClient = (CVolaNotificationClient*)AfxGetMainWnd();
		theApp.GetVolaNotificationClient()->VolaChangedNotification();
	}
	_CATCH_UNHANDLED_EXCEPTION;

}


void CDataProcessor::ContractChangedNotification(CString strContract)
{
	try
	{
		long iSize = strContract.GetLength()+1;

		char* pBuffer = new char[iSize];
		ZeroMemory(pBuffer, iSize);
		strncpy_s(pBuffer,iSize, (LPCSTR)strContract, strContract.GetLength());
		AfxGetMainWnd()->PostMessage (WM_STOCK_CHANGED,(WPARAM)pBuffer);
	}
	_CATCH_UNHANDLED_EXCEPTION;

}

// Processes WM_STOP_HANDLING message sent when CancelData() is calling
void CDataProcessor::OnStopHandling (WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_OK;
	try
	{
		if (wParam == 0 || wParam == 1)
		{
			if (m_spPriceProvider)
			{
	#ifdef _MULTIPROVIDERS
				if(m_spProviderData->GetIsGroupRequestSupported())
				{
					CComQIPtr<PP::IGroupPrice> spPrice = m_spPriceProvider;
					if(spPrice!=NULL)
					{
						spPrice->raw_CancelGroup();
					}
				}
				else
					m_spPriceProvider->raw_CancelLastQuote();
#else
				m_spPriceProvider->raw_CancelLastQuote();
#endif
//				m_spPriceProvider->raw_UnSubscribeQuote();	// disable RT

				if (lParam)
				{
					m_spPriceProvider->raw_Disconnect();
					CreatePriceProvider(m_opts.m_enProviderType);
					m_bConnected = false;
				}
			}
		}

		if (wParam == 0 || wParam == 2)
		{
			m_ivc.StopRequests();
		}

		if (wParam == 0)
			m_evtHandlingStopped.SetEvent();
	}
	_CATCH_UNHANDLED_EXCEPTION;

}



// Requests historical ranges from IVolatility

CString CDataProcessor::GetErrorMessage(HRESULT hr, BSTR bstrDesc)
{
	CString strResult = bstrDesc;
	strResult.TrimRight();
	
	if (strResult.IsEmpty()  )
	{
		_com_error err = hr;
		strResult = err.ErrorMessage();
	}
	CString strFmt;
#ifdef _DEBUG
	strFmt.Format (_T("%s error code %X"), (LPCTSTR) strResult, hr);
#else
	strFmt = strResult;
#endif

	return strFmt;
}

// Loads dividents information
void CDataProcessor::LoadContractParams(int nID)
{
	try
	{
		CDBLayout db;
		
		db.Connect();
		CClientRecordset rs;

		db.GetSymbolParams(nID, rs);

		m_contractData.m_dYield = rs[L"fYield"];
		m_contractData.m_dSkew = rs[L"fSkew"];
		m_contractData.m_dKurt = rs[L"fKurt"];


		IEtsIndexDivAtomPtr spDividends(__uuidof(EtsIndexDivAtom));

		spDividends->DivType = (EtsDivTypeEnum)long(rs["isDivCustom"]);
		spDividends->DivFreq = rs["iDivFrequency"]; 
		spDividends->DivDate = rs["dtDivDate"];
		spDividends->DivAmt =  rs["fAmount"];
		spDividends->DivFreqCust = rs["iDivFrequencyCustom"];
		spDividends->DivDateCust = rs["dtDivDateCustom"];
		spDividends->DivAmtCust = rs["fAmountCustom"];

		CClientRecordset rsDividends;
		db.GetDividends(nID, rsDividends);

		IEtsDivCollPtr spCustomColl(__uuidof(EtsDivColl));
		while(!rsDividends.IsEOF())
		{
			vt_date dDate   = rsDividends[L"DivYtes"];
			double 	dAmount = rsDividends[L"DivAmnt"];
			spCustomColl->AddNonUnique(dDate, dAmount);
			++rsDividends;
		}
		spDividends->CustomDivs = spCustomColl;
		m_StockDividends[nID] =	 spDividends;
		m_contractData.m_spDividend = spDividends;


		m_contractData.m_bIsAmerican = true;
		m_contractData.m_bIsIndex = (int) rs[L"iContractTypeID"]  == 1;
		

	#ifdef _VOLA_MANAGER
		try
		{
			CClientRecordset rsUndPos, rsSeqNum;
			db.GetUnderlyingPosGet(nID,rsUndPos);
			//Load Underlying Position
			m_contractData.m_dPosition = rsUndPos[L"fPosition"].IsNull() ? 
				-DBL_MAX : rsUndPos[L"fPosition"];
			
			db.GetLastTradeSeqNum(rsSeqNum);
			m_lTradeSeqNum = rsSeqNum[L"iSeqNum"];

			m_contractData.m_bIsHTB = ((int)rs[L"bIsHTB"]==0)?false:true;
			m_contractData.m_bIsAmerican = ((int)rs[L"tiCalcOptionType"]==0)?false:true;
		}
		_CATCH_UNHANDLED_EXCEPTION;
	#endif
		
		db.Disconnect();
	}
	_CATCH_UNHANDLED_EXCEPTION;

}

#ifdef _VOLA_MANAGER
// Loads dividents information
void CDataProcessor::ReLoadContractPos()
{
	try
	{
	
		CDBLayout db;
		
		db.Connect();
		try
		{
			CClientRecordset rsUndPos, rsSeqNum;
			db.GetUnderlyingPosGet(m_contractData.nID,rsUndPos);
			//Load Underlying Position
			m_contractData.m_dPosition = rsUndPos[L"fPosition"].IsNull() ? 
				-DBL_MAX : rsUndPos[L"fPosition"];
			
			db.GetLastTradeSeqNum(rsSeqNum);
			m_lTradeSeqNum = rsSeqNum[L"iSeqNum"];
		}
		_CATCH_UNHANDLED_EXCEPTION;

		
		db.Disconnect();
	}
	_CATCH_UNHANDLED_EXCEPTION;

}
#endif

void CDataProcessor::LoadRates()
{
	try
	{
		m_ratesBid.clear();
		m_ratesAsk.clear();

		CDBLayout db;
		
		db.Connect();
		CClientRecordset rsCurve,rsPoint;
		

		db.GetIRCurve(rsCurve);
		int nCnt = rsCurve.GetRecordCount();

		if(nCnt)
		{
			rsCurve.MoveFirst();
			m_IRCurveData.m_lID = rsCurve[L"iCurveID"];
			m_IRCurveData.m_dbPosThreshold = rsCurve[L"mPositionThreshold"];
			m_IRCurveData.m_lCurveType = rsCurve[L"iCurveTypeID"];
			m_IRCurveData.m_RuleID = (CIRCurveData::IrRuleType)(int)rsCurve[L"iRuleTypeID"];
		}	

		// Now this strored procedure gets data for all IRPoints
		// If more than one IRCurve will be available
		// Stored procedure must get on more parameter - CurveID !!!
		db.GetRates(rsPoint);
		
		nCnt = rsPoint.GetRecordCount();
		m_ratesBid.reserve (nCnt);
		m_ratesAsk.reserve (nCnt);

		for (;!rsPoint.IsEOF(); ++rsPoint )
		{
			RATE rtBid, rtAsk;
	#ifdef _VOLA_MANAGER
			if(m_contractData.m_bIsHTB)
				rtAsk.dRate = rsPoint[L"fHTBRate"];
			else
	#endif
				rtAsk.dRate = rsPoint[L"fRateAsk"];

			rtBid.dRate = rsPoint[L"fRateBid"];

			rtBid.nDTE = rsPoint[L"iDaysFromToday"];
			rtAsk.nDTE = rtBid.nDTE;
			
			m_ratesBid.push_back (rtBid);
			m_ratesAsk.push_back (rtAsk);
		}

		db.Disconnect();
	}
	_CATCH_UNHANDLED_EXCEPTION;

}

void CDataProcessor::UpdateMidMarket()
{
	if (!m_bUpdateCache)
		return;

	try
	{
		
#ifdef _IVAN_DB_ACCESS
		CDBLayout  & db  = m_dbCache;
#else
		CDBLayout  db;
#endif 
		
		if (!db.IsConnected())
			db.Connect( m_opts.m_strConnection );


		try
		{
			db.BeginTran();
			db.UpdateContractMarketData (m_contractData.nID, m_contractData.m_dPrice);	

			for (synth_underl_map::iterator it = m_SynthUndList.begin(); it != m_SynthUndList.end(); it++)
			{
				db.UpdateContractMarketData (it->second.nID, it->second.m_dPrice);	

			}
			db.Commit();	
		}
		catch (_com_error & )
		{
			db.Rollback();
			throw;
		}

	}
	catch (_com_error & e)
	{
		m_bUpdateCache = false;
		//ErrorNotification (e, false);
		SetStatusText(e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage());
	}
	_CATCH_UNHANDLED_EXCEPTION;


}
void CDataProcessor::UpdateVolaCache()
{
	if (!m_bUpdateCache)
		return;

	try
	{
		
#ifdef _IVAN_DB_ACCESS
		CDBLayout  & db  = m_dbCache;
#else
		CDBLayout  db;
#endif 
		
		if (!db.IsConnected())
			db.Connect( m_opts.m_strConnection );


		db.BeginTran();
		try
		{
			db.UpdateContractMarketData (m_contractData.nID, m_contractData.m_dPrice);
			
			for (synth_underl_map::iterator it = m_SynthUndList.begin(); it != m_SynthUndList.end(); it++)
			{
				db.UpdateContractMarketData (it->second.nID, it->second.m_dPrice);	
			}

			
			for (options_map::iterator itOpt = m_optsData.begin(); itOpt != m_optsData.end(); itOpt++ )
			{
				COptionData & opt =  itOpt->second;
				if(opt.m_bIsChanged)
				{
					db.UpdateOptionDataCache (itOpt->first.c_str(), opt);
				}
			}
			db.Commit();	
		}
		catch (_com_error & )
		{
			db.Rollback();
			throw;
		}

	}
	catch (_com_error & e)
	{
		m_bUpdateCache = false;
		//ErrorNotification (e, false);
		SetStatusText(e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage());
	}
	_CATCH_UNHANDLED_EXCEPTION;

}

void CDataProcessor::UpdateVolaCache(const std::wstring & strOption, const COptionData & opt)
{
	if (!m_bUpdateCache)
		return;

	try
	{

#ifdef _IVAN_DB_ACCESS
		CDBLayout  & db  = m_dbCache;
#else
		CDBLayout  db;
#endif 
		

		if(opt.m_bIsChanged)
		{
			if (! db.IsConnected())
				db.Connect(m_opts.m_strConnection);

			db.UpdateOptionDataCache (strOption.c_str(), opt);
		}
	}
	catch (_com_error & e)
	{
		m_bUpdateCache = false;
		//ErrorNotification (e, false);
		SetStatusText(e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage());
	}
	_CATCH_UNHANDLED_EXCEPTION;


}

void CDataProcessor::CalcTimeSkew()
{
	try
	{
	
		m_csTimeSkew.Lock();
		double dPrice = m_contractData.m_dPrice; 
		
		m_time_skewData.clear();

		for( options_map::iterator it = m_optsData.begin(); it != m_optsData.end(); it++ )
		{
			COptionData & opt = it->second;
			CExpDate dtExp(opt.m_dtExpiryOV);
			
			
			bool bValid = m_opts.m_enCurveMode == enCall || 
						  m_opts.m_enCurveMode == enPut	 || 
						  opt.m_bIsCall;
			
			if( bValid )
			{
				time_skew_map::iterator itCalc = m_time_skewData.find( dtExp );
				if( itCalc != m_time_skewData.end() )
				{
					CTimeSkewData& tsdata = itCalc->second;

					if( fabs(dPrice - opt.m_dStrike) < fabs(dPrice - tsdata.m_pOption->m_dStrike) )
						m_time_skewData[dtExp] = &opt; 
				}
				else
					m_time_skewData[dtExp] = &opt;
			}
		}

		m_csTimeSkew.Unlock();
	}
	_CATCH_UNHANDLED_EXCEPTION;

}


void CDataProcessor::UpdatePrice( PP::QuoteUpdateParams *Params, PP::QuoteUpdateInfo *Results )
{
	if( m_nSleepFrequency && !(m_nUpdatesCount % m_nSleepFrequency) )
		::Sleep( m_nSleepDuration );
	m_nUpdatesCount++;

	if( BeginProcessing() )
	{
		try
		{
			double dThreshold = theApp.GetThreshold();

			switch (Params->Type)
			{
			case PP::enSTK:
			case PP::enIDX:
				{
					_bstr_t bsSymbol = Params->Symbol;

					if(bsSymbol == _bstr_t(GetContractSymbol()))
					{

						double dPrice = m_contractData.m_dPrice;
						double dTemp;

						dTemp = Results->BidPrice;
						if(dTemp > BAD_DOUBLE_VALUE) m_contractData.m_dPriceBid = dTemp;

						dTemp = Results->AskPrice;
						if(dTemp > BAD_DOUBLE_VALUE) m_contractData.m_dPriceAsk = dTemp;
						
						dTemp = Results->LastPrice;
						if(dTemp > BAD_DOUBLE_VALUE) m_contractData.m_dPriceLast = dTemp;

						double dNewPrice = GetMidMarket( m_contractData.m_dPriceBid, m_contractData.m_dPriceAsk, m_contractData.m_dPriceLast );
						if( fEQZero( dNewPrice ) )
							throw CStopException();
						
						double dDelta = fabs (dNewPrice - dPrice);
						TRACE (_T("Delta: %f \n"), dDelta);
						
						m_csContractData.Lock();
						m_contractData.m_dPrice = dNewPrice;
						UnderlyingChangedNotification(m_contractData.nID, m_contractData.m_dPriceBid, m_contractData.m_dPriceAsk, m_contractData.m_dPriceLast );
						m_csContractData.Unlock();

	#ifdef _VOLA_MANAGER
						try
						{
							GetVolaManager()->UnderlinePrice[_bstr_t(GetContractSymbol())] = dNewPrice;
							AfxGetMainWnd()->PostMessage( WM_CUSTOM_VOLA_PRICE_CHANGED );
						}
						catch( const _com_error& )
						{
						}
						_CATCH_UNHANDLED_EXCEPTION;

	#endif	
						if (dDelta > dThreshold || (fEQ (dDelta, dThreshold) && dThreshold >0.0001) )
						{
							if( RecalcOptions() )
							{
								UpdateVolaCache();
								VolaChangedNotification();
							}
							else
								UpdateMidMarket();
						}
					}
					else if(m_SynthUndList.find(Params->Symbol) != m_SynthUndList.end())
					{
						CContractData & contr = m_SynthUndList[Params->Symbol];
						double dPrice = contr.m_dPrice;
						double dTemp;

						dTemp = Results->BidPrice;
						if(dTemp > BAD_DOUBLE_VALUE) contr.m_dPriceBid = dTemp;

						dTemp = Results->AskPrice;
						if(dTemp > BAD_DOUBLE_VALUE) contr.m_dPriceAsk = dTemp;
						
						dTemp = Results->LastPrice;
						if(dTemp > BAD_DOUBLE_VALUE) contr.m_dPriceLast = dTemp;

						double dNewPrice = GetMidMarket( contr.m_dPriceBid, contr.m_dPriceAsk, contr.m_dPriceLast );
						if( fEQZero( dNewPrice ) )
							throw CStopException();
						
						double dDelta = fabs (dNewPrice - dPrice);
						TRACE (_T("Delta: %f \n"), dDelta);
						
						m_csSynthUndList.Lock();
						contr.m_dPrice = dNewPrice;
						UnderlyingChangedNotification(contr.nID, contr.m_dPriceBid, contr.m_dPriceAsk, contr.m_dPriceLast );
						m_csSynthUndList.Unlock();

	#ifdef _VOLA_MANAGER
						try
						{
							GetVolaManager()->UnderlinePrice[_bstr_t(Params->Symbol)] = dNewPrice;
							AfxGetMainWnd()->PostMessage( WM_CUSTOM_VOLA_PRICE_CHANGED );
						}
						catch( const _com_error& )
						{
						}
						_CATCH_UNHANDLED_EXCEPTION;

	#endif	
						if (dDelta > dThreshold || (fEQ (dDelta, dThreshold) && dThreshold >0.0001) )
						{
							if( RecalcOptions(Params->Symbol) )
							{
								UpdateVolaCache();
								VolaChangedNotification();
							}
							else
								UpdateMidMarket();
						}
					}
				}
				break;
			case PP::enOPT:
				{		
					options_map::iterator it =  m_optsData.find (Params->Symbol);
					if (it != m_optsData.end() )
					{
						COptionData & opt = it->second;

						double dTemp;

						dTemp = Results->BidPrice;
						if(dTemp > BAD_DOUBLE_VALUE) opt.m_dPriceBid = dTemp;

						dTemp = Results->AskPrice;
						if(dTemp > BAD_DOUBLE_VALUE) opt.m_dPriceAsk = dTemp;
						
						dTemp = Results->LastPrice;
						if(dTemp > BAD_DOUBLE_VALUE) opt.m_dPriceLast = dTemp;

						double dNewPrice = GetMidMarket( opt.m_dPriceBid, opt.m_dPriceAsk, opt.m_dPriceLast );
						if( fEQZero( dNewPrice ) )
							throw CStopException();

						double dDelta = fabs (dNewPrice - opt.m_dPrice);
						TRACE (_T("Opt Delta: %f \n"), dDelta);

						if( !fEQZero(dNewPrice) && (dDelta > dThreshold || fEQ(dDelta, dThreshold)))
						{
							// Store current price
							double dOldPrice = opt.m_dPrice;
							opt.m_dPrice = dNewPrice;

							// Calc IV
							CalcIV( opt );

							if( opt.m_bIsChanged )
							{
								// IV has been changed
								opt.m_dtPriceDate = COleDateTime::GetCurrentTime();
								UpdateVolaCache (Params->Symbol, opt);
								VolaChangedNotification(opt);
							}
							else
							{
								// Restore price
								opt.m_dPrice = dOldPrice;
							}
						}
					}
				}
				break;
			};
		}
		catch (CStopException &) {}
		_CATCH_UNHANDLED_EXCEPTION;


		EndProcessing();
	}
}

void CDataProcessor::OnReload (WPARAM wParam, LPARAM lParam)
{
	
	if (BeginProcessing())
	{
		CLockReload lock;

		theApp.GetOptions(m_opts);
		
		try
		{
			switch (wParam)
			{
			case 1: // Rates
				if (! IsErrorState() )
				{
				
					CString strOld = GetStatusText();
					SetStatusText (_T("Recalculating implied volatility"));

					LoadRates();
					
					RecalcOptions();
					UpdateVolaCache();
					
					SetStatusText (strOld);
					VolaChangedNotification();
				}
				break;
			case 2: //Prices connection
				{
					{
						OnStopHandling (1,1);
						InitialRequests (true, false);
					}
				}
				break;
			case 3: // Ranges
				if (! IsErrorState())
				{
					OnStopHandling (2,0); // cancel ranges	
					InitialRequests (false, true);
				}
				break;
			}
		}
		catch (_com_error & e)
		{
			//ErrorNotification (e);
			SetStatusText(e.ErrorInfo() ? (LPCTSTR) e.Description() : e.ErrorMessage());
		}
		catch (CStopException &)
		{
		}
		_CATCH_UNHANDLED_EXCEPTION;

		EndProcessing();
	}
}


void CDataProcessor::ReloadRates()
{
	PostThreadMessage (WM_RELOAD, 1, 0);
}

void CDataProcessor::ReloadPricesConnection(bool bReloadProvider)
{
	m_bReload = bReloadProvider;
	PostThreadMessage (WM_RELOAD, 2, 0);
}
void CDataProcessor::ReloadRanges()
{
	PostThreadMessage (WM_RELOAD, 3, 0);
}


#ifdef _VOLA_MANAGER
STDMETHODIMP CDataProcessor::OnDefaultSurfaceChanged(/*[in]*/long SurfaceID, /*[in]*/EOptType OptType, /*in*/VARIANT_BOOL NewDefault )
{
	
	try
	{
		if( NewDefault == VARIANT_TRUE )
		{
			AfxGetMainWnd()->PostMessage( WM_CUSTOM_VOLA_DEFAULTSURFACE_CHANGED, (WPARAM)SurfaceID, (LPARAM)OptType );
		}
		else
		{
			CCustomVolaNotification *pNotification = new CCustomVolaNotification( CCustomVolaNotification::enSkew, 0 );
			AfxGetMainWnd()->PostMessage( WM_CUSTOM_VOLA_SAVED, (WPARAM)pNotification, NULL );
		}

	}
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	return S_OK;
}
#endif //_VOLA_MANAGER


void CDataProcessor::SubscribeOption( options_map::iterator it )
{
	return;		//	disable RT
	///
	
	try
	{
	
		ASSERT (it != m_optsData.end() );

		bool bSubscribe = true;
		COptionData & opt = it->second;

		if (m_opts.m_enCurveMode == enOTM)
		{
			double dDelta = m_contractData.m_dPrice * m_dPriceDelta;
			
			if ((opt.m_bIsCall && opt.m_dStrike < (m_contractData.m_dPrice /*- dDelta*/)) ||
				(!opt.m_bIsCall && opt.m_dStrike > (m_contractData.m_dPrice /*+ dDelta*/)) )
				bSubscribe = false;
		}

		
		if (bSubscribe)
		{
			_bstr_t bstrSymbol = it->first.c_str();
			
			PP::QuoteUpdateParams  qp;
			qp.Symbol = bstrSymbol;
			qp.Exchange = NULL;
			qp.Type = PP::enOPT;

			m_spPriceProvider->SubscribeQuote (&qp);
		}
		
	}
	_CATCH_UNHANDLED_EXCEPTION;
}

void CDataProcessor::SetRequestsStatusText(bool bPrices, bool bRanges)
{
	SetStatusText ( _T("Getting the latest data"));
}

void CDataProcessor::FinishProcessing()
{
	if( m_bGotPrices )
			SetReady();
}


bool CDataProcessor::GetRangeInfo( double& dStartStrike, double& dEndStrike )
{
	bool bRes;

	try
	{
	
		double dSpot = GetContractPrice();

		dStartStrike = m_opts.m_fChartRangeFrom * dSpot/100.;
		dEndStrike   = m_opts.m_fChartRangeTo   * dSpot/100.;
		
		
		bRes = ((m_opts.m_enChartMode == enChart_All_Range || m_opts.m_enChartMode == enChart_5Penny_Range) && (!fEQZero(dStartStrike) || !fEQZero(dEndStrike)) && dStartStrike < dEndStrike);

	}
	_CATCH_UNHANDLED_EXCEPTION;
	return bRes;
}


long CDataProcessor::GetOptionsDataByEpiration( const COleDateTime& dtMonth, COleDateTime& dtExpiration, strikes_map& strikesData, bool bUseRange )
{
	long nCntVola = 0;
	try
	{
	
		CDataProcessor::CLockVola lv;

		dtExpiration.SetStatus( COleDateTime::null );

	

		double dSpot = GetContractPrice();


		double dStartStrike = 0.;
		double dEndStrike   = 0.;

		bool bRange = GetRangeInfo( dStartStrike, dEndStrike );

		for( options_map::const_iterator itOpts = m_optsData.begin(); itOpts != m_optsData.end(); itOpts++ )
		{
			const COptionData & od = itOpts->second;
			COleDateTime dtExp = od.m_dtExpiration;

			if( dtMonth.GetYear()  == dtExp.GetYear() && 
				dtMonth.GetMonth() == dtExp.GetMonth() )
			{
				bool bValid = true;
				if( m_opts.m_enCurveMode == enOTM )
				{
					if( od.m_bIsCall )
						bValid = od.m_dStrike > dSpot;
					else
						bValid = od.m_dStrike < dSpot;
				}
				
				if( bValid )
				{
					if( dtExpiration.GetStatus() != COleDateTime::valid )
						dtExpiration = dtExp;
						
					if( fNEQZero(od.m_dVola) )
					{
						if(bUseRange && ( bRange && ( dStartStrike > od.m_dStrike || dEndStrike < od.m_dStrike )) )
							continue;
						strikesData.insert( std::make_pair( od.m_dStrike, &od ) );
						nCntVola ++;
					}
				}
			}
		}

		if( nCntVola > 0 && m_opts.m_enCurveMode == enOTM )
		{
			bool bChangeATMOpt = false;

			strikes_map::const_iterator itATMOpt = strikesData.upper_bound( dSpot );
			if( itATMOpt != strikesData.begin() && itATMOpt != strikesData.end() )
			{
				strikes_map::const_iterator itCallOpt = itATMOpt;
				strikes_map::const_iterator itPutOpt  = itATMOpt;
				--itPutOpt;

				double dCallStrike = itCallOpt->first;
				double dPutStrike  = itPutOpt->first;
				if( dSpot - dPutStrike <= dCallStrike - dSpot )
					bChangeATMOpt = true;
			}
			else
			{
				if (itATMOpt != strikesData.begin())
				{
					--itATMOpt;
					bChangeATMOpt = true;
				}
			}

			if( bChangeATMOpt )
			{
				for(options_map::const_iterator itOpts = m_optsData.begin(); itOpts != m_optsData.end(); itOpts++ )
				{
					const COptionData & od = itOpts->second;
					COleDateTime dtExp = od.m_dtExpiration;

					if( dtMonth.GetYear()  == dtExp.GetYear()  && 
						dtMonth.GetMonth() == dtExp.GetMonth() &&
						od.m_dStrike == itATMOpt->first        &&
						od.m_bIsCall == true
					  )
					{
						//strikesData.erase( od.m_dStrike );
						strikesData.insert( std::make_pair( od.m_dStrike, &od ) );
					}
				}
			}
		}

		if( nCntVola > 0 &&  
			( m_opts.m_enChartMode == enChart_5Penny || 
			  m_opts.m_enChartMode == enChart_5Penny_Range )
		  )
		{
			try
			{			
				int nCount = 0;
				strikes_map::iterator it = strikesData.begin();			

				while(it != strikesData.end())
				{
					double dStrike = it->first;
					const COptionData * pOpt = it->second;

					if(fabs(pOpt->m_dPrice - 0.05)< 0.00001)
					{
//						nCount ++;
//						if(nCount == 3)
//						{
//							strikes_map::iterator itTmp = it;
//							itTmp--;
//
//							double dStrike = itTmp->first;
//							strikesData.erase(itTmp);
//
//							itTmp = it;
//							itTmp--;
//							dStrike = itTmp->first;
//							strikesData.erase(itTmp);
//						}
//						if(nCount>=3)
//						{
//							strikes_map::iterator itTmp = it;
//							itTmp++;
//
//							strikesData.erase(it);
//							it = itTmp;
//							continue;
//						}
						it = strikesData.erase(it);

					}
					else
					{
						nCount  = 0;
						it++;
					}
				}
			}
			_CATCH_UNHANDLED_EXCEPTION;
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;
	return nCntVola;
}

double CDataProcessor::CalcRates(DOUBLE dYTE)
{

	double dShortRate = 0;
	double dLongRate = 0;
	bool bUseMidRates;

	try
	{
		double dPosThreshold;
	
		if( m_IRCurveData.m_RuleID == CIRCurveData::enRateBasedOnPosition 
			&& m_contractData.m_dPosition > (-DBL_MAX))
		{
			dPosThreshold = fabs(m_IRCurveData.m_dbPosThreshold);
			bUseMidRates = ( fEQZero(dPosThreshold) || 
				(fNEQZero(dPosThreshold) && fabs(m_contractData.m_dPosition) <= dPosThreshold));
		}
		else
		{
			bUseMidRates = true;
		}
		

		if (m_ratesAsk.size() > 0)
			dShortRate = InterpolateRates (m_ratesAsk.size(),  &m_ratesAsk[0], dYTE);

		if (m_ratesBid.size() > 0)
			dLongRate = InterpolateRates ( m_ratesBid.size(),  &m_ratesBid[0], dYTE );

	}
	_CATCH_UNHANDLED_EXCEPTION;

		if(bUseMidRates)
		{		
			return (dShortRate + dLongRate) / 2;
		}
		else
		{
			return m_contractData.m_dPosition < 0 ? dShortRate : dLongRate;
		}
}

void CDataProcessor::LoadSyntheticRoot( int nRootID, CSyntheticOptionRootData* pSynthRoot )
{
	try
	{
	
		CDBLayout db;
		
		db.Connect();
		CClientRecordset rs;

		db.GetOptionRoot	(nRootID, rs);

		bool IsSynthetic = (bool)rs[L"tiIsSynthetic"];

		if(IsSynthetic)
		{
			pSynthRoot->m_bIsBasket = (bool)rs[L"tiSyntheticIsBasket"];
			pSynthRoot->m_dCashAmount = rs[L"fCash"];
			pSynthRoot->m_dSkew = rs[L"fSyntheticSkew"];
			pSynthRoot->m_dKurt	= rs[L"fSyntheticKurt"];
			pSynthRoot->m_dYield = rs[L"fSyntheticYield"];
					
			rs.Close();

			db.GetSyntheticRootParams(nRootID,rs);
			
			m_csSynthUndList.Lock();

			for( ;!rs.IsEOF(); ++rs )
			{

				_bstr_t bsName =  rs[L"vcUndSymbol"];
				long nID = rs[L"iUnderlyingID"];
				CSyntheticOptionRootParam SynthRootParam;
				CContractData* pContr = NULL;
				
				SynthRootParam.m_dWeight = rs[L"fWeight"];

				if(nID == m_contractData.nID)
				{
					pContr = &m_contractData; 
				}
				else 
				{
					if( m_SynthUndList.find((LPOLESTR)bsName) ==  m_SynthUndList.end())
					{
						CContractData contData;
						contData.nID = rs[L"iUnderlyingID"];
						contData.m_bIsIndex = (int) rs[L"iUndTypeID"]  != 2;
						m_SynthUndList[(LPOLESTR)bsName] = contData;
					}
					
					pContr = &(m_SynthUndList[(LPOLESTR)bsName]);
					pContr->m_mapRefSynthOptRoots.insert(std::make_pair(nRootID,pSynthRoot));
				}


				SynthRootParam.m_pUnderlying = pContr;

				pSynthRoot->m_mapSynthRootComponents[(LPOLESTR)bsName] = SynthRootParam;
			}

			if(pSynthRoot->m_bIsBasket)
				LoadSynthRootDividends(*pSynthRoot);
			
			m_csSynthUndList.Unlock();

			rs.Close();
		}
			
		db.Disconnect();
	}
	_CATCH_UNHANDLED_EXCEPTION;

}

void CDataProcessor::LoadBasketIndexDividends( int nID, IEtsIndexDivCollPtr& spDivColl, double dCoeff)
{
	
	try
	{
		CDBLayout db;
		
		db.Connect();
		CClientRecordset rs;

		db.GetIndexDefinition (nID,rs);
		spDivColl.CreateInstance(__uuidof(EtsIndexDivColl));


		long lCount = 0;
		for( ;!rs.IsEOF(); ++rs )
		{
			long StockID = rs[L"iStockID"];
			double dWeight = rs[L"fWeight"];

		
			if(dWeight <= 0.0f || m_StockDividends.find(StockID) == m_StockDividends.end())
				continue;

			IEtsIndexDivAtomPtr spDiv = m_StockDividends[StockID];
			spDivColl->Add(lCount,lCount, spDiv);
			spDivColl->SetWeight(lCount,dWeight * dCoeff);
			lCount++;
		}
			
		db.Disconnect();
	}
	_CATCH_UNHANDLED_EXCEPTION;
}

void CDataProcessor::LoadSynthRootDividends( CSyntheticOptionRootData& synthRoot)
{

	if(!synthRoot.m_bIsBasket)
		return;

	try
	{

		synthRoot.m_Divs = NULL;

		for(synthunderl_param_map::iterator it =  synthRoot.m_mapSynthRootComponents.begin(); it != synthRoot.m_mapSynthRootComponents.end(); it++)
		{	
			CContractData* pContrData = it->second.m_pUnderlying;
			if(pContrData->m_bIsIndex)
			{
				LoadBasketIndexDividends(pContrData->nID,synthRoot.m_Divs, it->second.m_dWeight);			
			}
			else
			{
				if(m_StockDividends.find(pContrData->nID) == m_StockDividends.end() || it->second.m_dWeight<=0.f)
					continue;

				IEtsIndexDivAtomPtr spDiv = m_StockDividends[pContrData->nID];
				long lIDX = synthRoot.m_Divs->Count;
				synthRoot.m_Divs->Add(lIDX, lIDX, spDiv);
				synthRoot.m_Divs->SetWeight(lIDX, it->second.m_dWeight);
			}
		}
	}
	_CATCH_UNHANDLED_EXCEPTION;
}
