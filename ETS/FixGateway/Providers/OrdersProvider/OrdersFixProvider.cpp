// OrdersFixProvider.cpp : Implementation of COrdersFixProvider

#include "stdafx.h"
#include "OrdersFixProvider.h"


// COrdersFixProvider

static const _variant_t	vtTrue(true);

static const _bstr_t    SETTINGS_MAIN_XML_KEY(L"ETS\\Asp\\Groups\\");
static const _bstr_t    SETTINGS_USER_XML_KEY(L"ETS\\OrdersGateway\\");

static const _bstr_t    SETTINGS_XML_CLIENT(L"Client");
static const _bstr_t    SETTINGS_XML_EXECBROKER(L"ExecBroker");
static const _bstr_t    SETTINGS_XML_CONFIGFILE(L"OrdersCfg");

LPCSTR g_cMonthName[12] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };

#define SETTINGS_REGISTRY_KEY			_T("SOFTWARE\\EGAR\\{63E8FDC7-FC0A-4339-93FB-6AA9BF4FBE01}")
#define SETTINGS_LAST_ORDERID_KEY		_T("LastOrderID")
#define PROVIDER_GUID					L"{63E8FDC7-FC0A-4339-93FB-6AA9BF4FBE01}"

void COrdersFixProvider::FinalRelease()
{
	Disconnect();
}

void COrdersFixProvider::_ReadSettings()
{
	CRegKey	aKeyCur, aKeyLoc;
	bool bIsCU = false, bIsLM = false;

	LONG lResult = aKeyCur.Open(HKEY_CURRENT_USER, SETTINGS_REGISTRY_KEY, KEY_READ);		
	if(lResult == ERROR_SUCCESS)
		bIsCU = true;

	lResult = aKeyLoc.Open (HKEY_LOCAL_MACHINE, SETTINGS_REGISTRY_KEY, KEY_READ);
	if(lResult == ERROR_SUCCESS)
		bIsLM = true;

	TCHAR szOrderID[256] = {0};
	DWORD dwSize = sizeof(szOrderID) / sizeof(*szOrderID);

	if(bIsCU)
	{
		lResult = aKeyCur.QueryStringValue(SETTINGS_LAST_ORDERID_KEY, szOrderID, &dwSize);

		if (lResult == ERROR_SUCCESS)
		{
			m_LastOrderID = szOrderID;
			return;
		}
	}

	dwSize = sizeof(szOrderID) / sizeof(*szOrderID);
	if(bIsLM)
	{
		lResult = aKeyLoc.QueryStringValue(SETTINGS_LAST_ORDERID_KEY, szOrderID, &dwSize);

		if (lResult == ERROR_SUCCESS)
		{
			m_LastOrderID = szOrderID;
			return;
		}
	}
}

void COrdersFixProvider::_SaveSettings()
{
	CRegKey key;

	try
	{
		LONG lResult = key.Open(HKEY_CURRENT_USER, SETTINGS_REGISTRY_KEY, KEY_WRITE);
		if (lResult == ERROR_FILE_NOT_FOUND )
			lResult = key.Create(HKEY_CURRENT_USER, SETTINGS_REGISTRY_KEY);

		if (lResult != ERROR_SUCCESS )
			throw lResult;

		lResult = key.SetStringValue (SETTINGS_LAST_ORDERID_KEY, m_LastOrderID);
		if (lResult != ERROR_SUCCESS )
			throw lResult;
	}
	catch(LONG lResult)
	{
		CComErrorWrapper::ThrowError(HRESULT_FROM_WIN32(lResult), _T("Failed to write settings into the registry"));
	}
}


template<class OrderMessage>
void COrdersFixProvider::_FillOrder(FoOrder* Order, OrderMessage& Message)
{
	Order->ClOrdID = _bstr_t((LPCTSTR)m_LastOrderID).copy();
	if(!Order->ClOrdID || SysStringLen(Order->ClOrdID) == 0)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Order ID should be set."));

	Message.set(FIX::ClOrdID((LPCSTR)CW2A(Order->ClOrdID)));
	if(Order->ExecBroker && SysStringLen(Order->ExecBroker))
		Message.set(FIX::ExecBroker((LPCSTR)CW2T(Order->ExecBroker)));

	if(Order->ExDestination && SysStringLen(Order->ExDestination))
		Message.set(FIX::ExDestination((LPCSTR)CW2T(Order->ExDestination)));

	Message.set(FIX::HandlInst((char)Order->HandlInst));

	//symbol
	if(!Order->Symbol || SysStringLen(Order->Symbol) == 0)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Symbol should be set."));

	Message.set(FIX::Symbol((LPCSTR)CW2A(Order->Symbol)));

#if FIX_VERSION >= 42
	Message.set(FIX::TransactTime());
#endif

	// security type
	switch (Order->SecurityType)
	{
	case enFoStOPT:
		Message.set(FIX::SecurityType(_T("OPT")));
		break;
	case enFoStSTK:
		Message.set(FIX::SecurityType(_T("CS")));
		break;
	default:
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Security Type is not supported."));
	}


	if (Order->SecurityType == enFoStOPT)
	{
		//expdate
		if (EgNumeric::LessOrEq<DOUBLE>((DOUBLE)Order->ExpirationDate, 0.))
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Expiration Date should be set."));

		COleDateTime	dtExpDate(Order->ExpirationDate);
		TCHAR			szExpDate[12] = {0};

		_stprintf(szExpDate, _T("%04d%02d"), dtExpDate.GetYear(), dtExpDate.GetMonth());

		Message.set(FIX::MaturityMonthYear(szExpDate));

		//put-call
		Message.set(FIX::PutOrCall(Order->IsCall?1:0));

		//strike
		if (EgNumeric::LessOrEq<DOUBLE>(Order->StrikePrice, 0.))
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Strike Price should be set."));

		Message.set(FIX::StrikePrice(Order->StrikePrice));

		//open-close
		Message.set(FIX::OpenClose(m_bIsOpeningPosition?'O':'C'));
	}

	switch (Order->Side)
	{
	case enFoOsdBuy:
		Message.set(FIX::Side(FIX::Side_BUY));
		break;
	case enFoOsdSell:
		Message.set(FIX::Side(FIX::Side_SELL));
		break;
	default:
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Order Side Type is not supported."));
	}

	Message.set(FIX::OrdType((char)Order->OrdType));
	if(Order->OrdType == enFoOrdMarket || Order->OrdType == enFoOrdLimit)
	{
		if(Order->TimeInForce != enFoTifDay && Order->TimeInForce != enFoTifGTC)
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("TimeInForce is invalid for this kind of order."));
	}
	Message.set(FIX::TimeInForce((char)Order->TimeInForce));

	if(Order->OrderQty <= 0L) 
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Quantity should be set."));

	Message.set(FIX::OrderQty(Order->OrderQty));

	// price - for order types other then 1(market) and 5(market on close)
	if(Order->OrdType != enFoOrdMarket && Order->OrdType != enFoOrdMarketOnClose)
	{
		if(EgNumeric::LessOrEq<DOUBLE>(Order->Price, 0.))
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Price should be set for not-market orders."));
		Message.set(FIX::Price(Order->Price));
	}

	if(Order->OrdType == enFoOrdStop)
	{
		if(EgNumeric::LessOrEq<DOUBLE>(Order->StopPrice, 0.))
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Stop Price should be set for stop orders."));
		Message.set(FIX::StopPx(Order->StopPrice));
	}

	if(Order->Account && SysStringLen(Order->Account) != 0)
		Message.set(FIX::Account((LPCSTR)CW2A(Order->Account)));

	if(Order->MinQty > 0L)
		Message.set(FIX::MinQty(Order->MinQty));

	// Client Category - FirstTraders required field.
	if( m_chClientCategory != '0' )
		Message.set(FIX::Rule80A((char) m_chClientCategory));

	//if( m_bUseClearingAcc )
	//	Message.set(FIX::ClearingAccount("123"));

	//if( m_chTimeInForce == 'Z' )
	//	Message.set(FIX::ExecBroker("234"));
	//else
	//{
	//	if( m_chTimeInForce != '0' )
	//		Message.set(FIX::TimeInForce((char) m_chTimeInForce));
	//}


	// VIANET //
	if ( m_bsClientID.length() > 0 )
		Message.set(FIX::ClientID((LPCTSTR) m_bsClientID));
	if ( m_bsExecBroker.length() > 0 )
		Message.set(FIX::ExecBroker((LPCTSTR) m_bsExecBroker));
	
}

template<class OrderMessage>
void COrdersFixProvider::_FillOrderCancelReplaceRequest(FoOrderCancelReplaceRequest* Request, OrderMessage& Message)
{
	Request->ClOrdID = _bstr_t((LPCTSTR)m_LastOrderID).copy();
	if(!Request->ClOrdID || SysStringLen(Request->ClOrdID) == 0)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Order ID should be set."));

	Message.set(FIX::ClOrdID((LPCSTR)CW2A(Request->ClOrdID)));

	if(!Request->OrigClOrdID || SysStringLen(Request->OrigClOrdID) == 0)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Original Order ID should be set."));
	Message.set(FIX::OrigClOrdID((LPCSTR)CW2A(Request->OrigClOrdID)));

	if(Request->ExecBroker && SysStringLen(Request->ExecBroker))
		Message.set(FIX::ExecBroker((LPCSTR)CW2A(Request->ExecBroker)));

	if(Request->ExDestination && SysStringLen(Request->ExDestination))
		Message.set(FIX::ExDestination((LPCSTR)CW2A(Request->ExDestination)));

	Message.set(FIX::HandlInst((char)Request->HandlInst));

#if FIX_VERSION >= 42
	Message.set(FIX::TransactTime());
#endif

	//symbol
	if(!Request->Symbol || SysStringLen(Request->Symbol) == 0)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Symbol should be set."));
	Message.set(FIX::Symbol((LPCSTR)CW2A(Request->Symbol)));

	// security type
	switch (Request->SecurityType)
	{
	case enFoStOPT:
		Message.set(FIX::SecurityType("OPT"));
		break;
	case enFoStSTK:
		Message.set(FIX::SecurityType("CS"));
		break;
	default:
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Security Type is not supported."));
	}


	if (Request->SecurityType == enFoStOPT)
	{
		//expdate
		if (EgNumeric::LessOrEq<DOUBLE>(Request->ExpirationDate, 0.))
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Expiration Date should be set."));

		COleDateTime	dtExpDate(Request->ExpirationDate);
		TCHAR			szExpDate[12] = {0};
		_stprintf(szExpDate, _T("%04d%02d"), dtExpDate.GetYear(), dtExpDate.GetMonth());

		Message.set(FIX::MaturityMonthYear(szExpDate));

		//put-call
		Message.set(FIX::PutOrCall(Request->IsCall ? 1 : 0));

		//strike
		if (EgNumeric::LessOrEq<DOUBLE>(Request->StrikePrice, 0.))
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Strike Price should be set."));
		Message.set(FIX::StrikePrice(Request->StrikePrice));

		//open-close
		Message.set(FIX::OpenClose(m_bIsOpeningPosition?'O':'C'));
	}

	switch (Request->Side)
	{
	case enFoOsdBuy:
		Message.set(FIX::Side(FIX::Side_BUY));
		break;
	case enFoOsdSell:
		Message.set(FIX::Side(FIX::Side_SELL));
		break;
	default:
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Order Side Type is not supported."));
	}


	Message.set(FIX::OrdType((char)Request->OrdType));

	if(Request->OrdType == enFoOrdMarket || Request->OrdType == enFoOrdLimit)
	{
		if(Request->TimeInForce != enFoTifDay && Request->TimeInForce != enFoTifGTC)
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("TimeInForce is invalid for this kind of order."));
	}
	Message.set(FIX::TimeInForce((char)Request->TimeInForce));

	if(Request->OrderQty <= 0L)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Quantity should be set."));

	Message.set(FIX::OrderQty(Request->OrderQty));

	// price - for order types other then 1(market) and 5(market on close)
	if(Request->OrdType != enFoOrdMarket && Request->OrdType != enFoOrdMarketOnClose)
	{
		if(EgNumeric::LessOrEq<DOUBLE>(Request->Price, 0.))
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Price should be set for not-market orders."));

		Message.set(FIX::Price(Request->Price));
	}

	if(Request->OrdType == enFoOrdStop)
	{
		if(EgNumeric::LessOrEq<DOUBLE>(Request->StopPrice, 0.))
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Stop Price should be set for stop orders."));

		Message.set(FIX::StopPx(Request->StopPrice));
	}
	if(Request->Account && SysStringLen(Request->Account) != 0)
		Message.set(FIX::Account((LPCSTR)CW2A(Request->Account)));

	// Client Category - FirstTraders required field.
	if( m_chClientCategory != '0' )
		Message.set(FIX::Rule80A((char) m_chClientCategory));

	//if( m_chTimeInForce != '0' )
	//	Message.set(FIX::TimeInForce((char) m_chTimeInForce));

	//if( m_chExecInst == 'G' )
	//	Message.set(FIX::ExecInst((char) m_chExecInst));

// VIANET //
	if ( m_bsClientID.length() > 0 )
		Message.set(FIX::ClientID((LPCTSTR) m_bsClientID ));
	if ( m_bsExecBroker.length() > 0 )
		Message.set(FIX::ExecBroker((LPCTSTR) m_bsExecBroker));
}

template<class OrderMessage>
void COrdersFixProvider::_FillOrderCancelRequest(FoOrderCancelRequest* Request, OrderMessage& Message)
{
	Request->ClOrdID = _bstr_t((LPCTSTR)m_LastOrderID).copy();
	if(!Request->ClOrdID || SysStringLen(Request->ClOrdID) == 0)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Order ID should be set."));

	Message.set(FIX::ClOrdID((LPCSTR)CW2A(Request->ClOrdID)));
	
#if FIX_VERSION >= 42
	Message.set(FIX::TransactTime());
#endif

	if(!Request->OrigClOrdID || SysStringLen(Request->OrigClOrdID) == 0)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Original Order ID should be set."));
	Message.set(FIX::OrigClOrdID((LPCSTR)CW2A(Request->OrigClOrdID)));

	if(Request->ExecBroker && SysStringLen(Request->ExecBroker))
		Message.set(FIX::ExecBroker((LPCSTR)CW2A(Request->ExecBroker)));

	Message.set(FIX::OrderQty(Request->OrderQty));

	//symbol
	if(!Request->Symbol || SysStringLen(Request->Symbol) == 0)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Symbol should be set."));
	Message.set(FIX::Symbol((LPCSTR)CW2A(Request->Symbol)));

	// security type
	switch (Request->SecurityType)
	{
	case enFoStOPT:
		Message.set(FIX::SecurityType("OPT"));
		break;
	case enFoStSTK:
		Message.set(FIX::SecurityType("CS"));
		break;
	default:
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Security Type is not supported."));
	}


	if (Request->SecurityType == enFoStOPT)
	{
		//expdate
		if (EgNumeric::LessOrEq<DOUBLE>(Request->ExpirationDate, 0.))
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Expiration Date should be set."));
		COleDateTime	dtExpDate(Request->ExpirationDate);
		TCHAR			szExpDate[12] = {0};

		_stprintf(szExpDate, _T("%04d%02d"), dtExpDate.GetYear(), dtExpDate.GetMonth());

		Message.set(FIX::MaturityMonthYear(szExpDate));

		//put-call
		Message.set(FIX::PutOrCall(Request->IsCall ? 1 : 0));

		//strike
		if (EgNumeric::LessOrEq<DOUBLE>(Request->StrikePrice, 0.))
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Strike Price should be set."));
		Message.set(FIX::StrikePrice(Request->StrikePrice));
	}

	switch (Request->Side)
	{
	case enFoOsdBuy:
		Message.set(FIX::Side(FIX::Side_BUY));
		break;
	case enFoOsdSell:
		Message.set(FIX::Side(FIX::Side_SELL));
		break;
	default:
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Order Side Type is not supported."));
	}

// VIANET //
	if ( m_bsClientID.length() > 0 )
		Message.set(FIX::ClientID((LPCTSTR) m_bsClientID ));
	if ( m_bsExecBroker.length() > 0 )
		Message.set(FIX::ExecBroker((LPCTSTR) m_bsExecBroker));
}

template<class OrderMessage>
void COrdersFixProvider::_FillOrderStatusRequest(FoOrderStatusRequest* Request, OrderMessage& Message)
{
	if(!Request->ClOrdID || SysStringLen(Request->ClOrdID) == 0)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Order ID should be set."));
	Message.set(FIX::ClOrdID((LPCSTR)CW2A(Request->ClOrdID)));

	if(Request->ExecBroker && SysStringLen(Request->ExecBroker))
		Message.set(FIX::ExecBroker((LPCSTR)CW2A(Request->ExecBroker)));

	//symbol
	if(!Request->Symbol || SysStringLen(Request->Symbol) == 0)
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Symbol should be set."));

	Message.set(FIX::Symbol((LPCSTR)CW2A(Request->Symbol)));
	
	// security type
	switch (Request->SecurityType)
	{
	case enFoStOPT:
		Message.set(FIX::SecurityType("OPT"));
		break;
	case enFoStSTK:
		Message.set(FIX::SecurityType("CS"));
		break;
	default:
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Security Type is not supported."));
	}


	if (Request->SecurityType == enFoStOPT)
	{
		//expdate
		if (EgNumeric::LessOrEq<DOUBLE>(Request->ExpirationDate, 0.))
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Expiration Date should be set."));

		COleDateTime	dtExpDate(Request->ExpirationDate);
		TCHAR			szExpDate[12] = {0};		

		_stprintf(szExpDate, _T("%04d%02d"), dtExpDate.GetYear(), dtExpDate.GetMonth());

		Message.set(FIX::MaturityMonthYear(szExpDate));

		//put-call
		Message.set(FIX::PutOrCall(Request->IsCall ?1:0));

		//strike
		if (EgNumeric::LessOrEq<DOUBLE>(Request->StrikePrice, 0.))
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Strike Price should be set."));

		Message.set(FIX::StrikePrice(Request->StrikePrice));
	}

	switch (Request->Side)
	{
	case enFoOsdBuy:
		Message.set(FIX::Side(FIX::Side_BUY));
		break;
	case enFoOsdSell:
		Message.set(FIX::Side(FIX::Side_SELL));
		break;
	default:
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Order Side Type is not supported."));
	}
}

void COrdersFixProvider::_ExtractExecutionReport(const FIX42::ExecutionReport& Message, FoExecutionReport* Report)
{
	_variant_t	vtSet;
	_bstr_t		bsValue;

	try
	{
		// EXTRACT REQUIRED FIELDS
		FIX::ClOrdID orderID;
		Message.get(orderID);
		Report->ClOrdID = SysAllocString(CA2W(orderID.getValue().c_str()));

		FIX::ExecID execID;
		Message.get(execID);
		Report->ExecID = SysAllocString(CA2W(execID.getValue().c_str()));

		FIX::ExecTransType execTransType;
		Message.get(execTransType);
		switch(execTransType.getValue())
		{
		case '0':
			Report->ExecTransType = enFoTrnNew;
			break;
		case '1':
			Report->ExecTransType = enFoTrnCancel;
			break;
		case '2':
			Report->ExecTransType = enFoTrnCorrect;
			break;
		case '3':
			Report->ExecTransType = enFoTrnStatus;
			break;
		default:
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid Execution Transaction Type."));
		}

		FIX::ExecType execType;
		Message.get(execType);
		Report->ExecType = (FoOrderStatusEnum)execType.getValue();

		FIX::OrdStatus ordStatus;
		Message.get(ordStatus);
		Report->OrdStatus = (FoOrderStatusEnum)ordStatus.getValue();

		FIX::Symbol symbol;
		Message.get(symbol);
		Report->Symbol = SysAllocString(CA2W(symbol.getValue().c_str()));

		try // TODO: Add futures
		{
			FIX::SecurityType securityType;
			Message.get(securityType);
			const _bstr_t bsType(securityType.getValue().c_str());

			if (bsType == _bstr_t(L"OPT"))
				Report->SecurityType = enFoStOPT;
			else if (bsType == _bstr_t(L"CS"))
				Report->SecurityType = enFoStSTK;
			else
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid Security type."));
		}
		catch(...) {}

		if (Report->SecurityType == enFoStOPT)
		{
			FIX::MaturityMonthYear maturityMonthYear;
			Message.get(maturityMonthYear);

			int	nYear  = 0;
			int	nMonth = 0;
			int	nDay   = 1;

			int iRes = _stscanf(maturityMonthYear.getValue().c_str(), _T("%04d%02d"), &nYear, &nMonth);
			if (iRes != 2)
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid Maturity Month-Year."));

			try
			{
				FIX::MaturityDay maturityDay;
				Message.get(maturityDay);

				iRes = _stscanf(maturityDay.getValue().c_str(), _T("%02d"), &nDay);
				if (iRes != 1)
					nDay = 15;
			}
			catch(FIX::FieldNotFound& /*fieldError*/) {}

			COleDateTime dtExpDate;
			iRes = dtExpDate.SetDate(nYear, nMonth, nDay);

			if (iRes)
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Can't set Expiration Day."));

			Report->ExpirationDate = dtExpDate;

			FIX::StrikePrice strikePrice;
			Message.get(strikePrice);
			Report->StrikePrice = strikePrice;

			bsValue = L"1";
			FIX::PutOrCall putOrCall;
			Message.get(putOrCall);

			if(putOrCall.getValue() == FIX::PutOrCall_CALL)
				Report->IsCall = VARIANT_TRUE;
			else if(putOrCall.getValue() == FIX::PutOrCall_PUT)
				Report->IsCall = VARIANT_FALSE;
			else
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid Put/Call."));

			Report->Root = ::SysAllocString(Report->Symbol);
			char szSym[21] = { 0 };
			_snprintf(szSym, sizeof(szSym) - 1, "%S%s%d%s",
				Report->Symbol,
				g_cMonthName[nMonth >= 0 && nMonth < 12 ? nMonth : 0], (long)Report->StrikePrice, (Report->IsCall ? "C" : "P"));
			if(strlen(szSym))
			{
				SysFreeString(Report->Symbol);
				Report->Symbol = SysAllocString(CA2W(szSym));
			}

			if ( Message.isSetField( 9001 ) ) {
				FIX::DoubleField fld( 9001 );
				bool bExist = true;
				try{ Message.getField( fld ); }
				catch(...) { bExist = false; }
				if(bExist)
					Report->StockRefPrice = (fld);
				else
					Report->StockRefPrice = 0.;
			}

			try
			{
				FIX::UnderlyingSymbol undSymbol;
				if(Message.isSetField(undSymbol))
					Report->UndSymbol = SysAllocString(CA2W(undSymbol.getValue().c_str()));
			}
			catch (...)
			{
				Report->UndSymbol = SysAllocString(L"");
			}
		}
		else if(Report->SecurityType == enFoStSTK)
		{
			Report->UndSymbol = Report->Symbol;
		}

		FIX::Side side;
		Message.get(side);
		Report->Side = (FoOrderSideEnum)side.getValue();

		try
		{
			FIX::LastShares lastShares;
			Message.get(lastShares);
			Report->LastShares = (long)lastShares;
		}
		catch(FIX::FieldNotFound& /*fieldError*/) {}

		try
		{
			FIX::LeavesQty leavesQty;
			Message.get(leavesQty);
			Report->LeavesQuantity = (long)leavesQty;
		}
		catch(FIX::FieldNotFound& /*fieldError*/) {}

		try
		{
			FIX::CumQty cumQty;
			Message.get(cumQty);
			Report->CumQuantity = (long)cumQty;
		}
		catch(FIX::FieldNotFound& /*fieldError*/) {}

		try
		{
			FIX::LastPx lastPx;
			Message.get(lastPx);
			Report->LastPrice = lastPx;
		}
		catch(FIX::FieldNotFound& /*fieldError*/){}

		try
		{
			FIX::ExecBroker execBroker;
			Message.get(execBroker);
			Report->ExecBroker = SysAllocString(CA2W(execBroker.getValue().c_str()));
		}
		catch(FIX::FieldNotFound& /*fieldError*/){}

		try
		{
			FIX::Account account;
			Message.get(account);
			Report->Account = SysAllocString(CA2W(account.getValue().c_str()));
		}
		catch(FIX::FieldNotFound& /*fieldError*/){}

		try
		{
			FIX::OrderQty orderQty;
			Message.get(orderQty);
			Report->OrderQty = (long)orderQty;
		}
		catch(FIX::FieldNotFound& /*fieldError*/){}

		try
		{
			FIX::OrdType ordType;
			Message.get(ordType);
			Report->OrderType = (FoOrderTypeEnum)ordType.getValue();
		}
		catch(FIX::FieldNotFound& /*fieldError*/){}

		try
		{
			FIX::Price price;
			Message.get(price);
			Report->Price = price;
		}
		catch(FIX::FieldNotFound& /*fieldError*/){}

		try
		{
			FIX::StopPx stopPx;
			Message.get(stopPx);
			Report->StopPrice = stopPx;
		}
		catch(FIX::FieldNotFound& /*fieldError*/){}

		try
		{
			FIX::TimeInForce timeInForce;
			Message.get(timeInForce);
			Report->TimeInForce = (FoTimeInForceEnum)timeInForce.getValue();
		}
		catch(FIX::FieldNotFound& /*fieldError*/){}

		//		try
		//		{
		//			FIX::CxlQty cxlQty;
		//			Message.get(cxlQty);
		//			Report->CanceledQuantity = cxlQty;
		//		}
		//		catch(FIX::FieldNotFound& /*fieldError*/){}

		try
		{
			FIX::OrigClOrdID origClOrdID ;
			Message.get(origClOrdID);
			Report->OrigClOrdID = SysAllocString(CA2W(origClOrdID.getValue().c_str()));
		}
		catch(FIX::FieldNotFound& /*fieldError*/){}

		try
		{
			FIX::ExecRefID execRefID;
			Message.get(execRefID);
			Report->ExecRefID = SysAllocString(CA2W(execRefID.getValue().c_str()));
		}
		catch(FIX::FieldNotFound& fldError)
		{
			if(Report->ExecTransType == enFoTrnCancel || Report->ExecTransType == enFoTrnCorrect)
				throw(fldError);
		}

		try
		{
			FIX::Text text;
			Message.get(text);
			Report->Text = SysAllocString(CA2W(text.getValue().c_str()));
		}
		catch(FIX::FieldNotFound& /*fieldError*/){}

		try
		{
			FIX::TransactTime transactTime;
			Message.get(transactTime);

			int nYear    = transactTime.getValue().getYear();
			int nMonth   = transactTime.getValue().getMonth();
			int	nDay     = transactTime.getValue().getDate();
			int	nHour    = transactTime.getValue().getHour();
			int nMinutes = transactTime.getValue().getMinute();
			int nSeconds = transactTime.getValue().getSecond();;
			COleDateTime dtVal(nYear, nMonth, nDay, nHour, nMinutes, nSeconds);
			Report->TransactTime = dtVal;
		}
		catch(FIX::FieldNotFound& /*fieldError*/)
		{
			time_t tNow = time(NULL);
			struct tm* tmNow = gmtime(&tNow);
			COleDateTime dtVal(tmNow->tm_year + 1900, tmNow->tm_mon + 1, tmNow->tm_mday, tmNow->tm_hour, tmNow->tm_min, tmNow->tm_sec);
			Report->TransactTime = dtVal;
		}

		try
		{
			FIX::AvgPx avgPx;
			Message.get(avgPx);
			Report->AvgPrice = avgPx;
		}
		catch(...){}
	}
	catch (FIX::FieldNotFound& fieldError)
	{
		CComErrorWrapper::ThrowError(E_INVALIDARG, fieldError.what());		
	}
}

void COrdersFixProvider::_ExtractOrderCancelReject(const FIX42::OrderCancelReject& Message, FoOrderCancelReject* Reject)
{
	FIX::OrigClOrdID origClOrdID;
	Message.get(origClOrdID);
	Reject->OrigClOrdID = ::SysAllocString(CA2W(origClOrdID.getValue().c_str()));

	FIX::ClOrdID clOrdID;
	Message.get(clOrdID);
	Reject->ClOrdID = ::SysAllocString(CA2W(clOrdID.getValue().c_str()));

	FIX::OrdStatus ordStatus;
	Message.get(ordStatus);
	Reject->OrdStatus = (FoOrderStatusEnum)ordStatus.getValue();

	try
	{
		FIX::CxlRejReason cxlRejReason;
		Message.get(cxlRejReason);
		if (cxlRejReason == 1)
			Reject->CxlRejReason = enFoCrrUnknownOrder;
		else if(cxlRejReason == 0)
			Reject->CxlRejReason = enFoCrrTooLate;
	}
	catch(FIX::FieldNotFound& /*fieldError*/) {}

	try
	{
		FIX::ExecBroker execBroker;
		Message.get(execBroker);
		Reject->ExecBroker = ::SysAllocString(CA2W(execBroker.getValue().c_str()));
	}
	catch(FIX::FieldNotFound& /*fieldError*/) {}

	try
	{
		FIX::Text text;
		Message.get(text);
		Reject->Text = ::SysAllocString(CA2W(text.getValue().c_str()));
	}
	catch(FIX::FieldNotFound& /*fieldError*/) {}

}

STDMETHODIMP COrdersFixProvider::raw_SubmitNewOrder(FoOrder* Order, BSTR* OrderID)
{
	if(!Order || !OrderID)
		return E_POINTER;

	try
	{
		ATLTRACE("SubmitNewOrder called");
		m_LastOrderID++;
		_SaveSettings();

		FIX42::NewOrderSingle message;
		_FillOrder(Order, message);

		if(m_bsSenderSubID.length())
		{
			FIX::SenderSubID senderSubID;
			message.getHeader().setField(senderSubID.getField(), (LPCTSTR) m_bsSenderSubID );
		}

		FIX::Session::sendToTarget(message, m_cussrentSession);

		*OrderID = _bstr_t((LPCTSTR)m_LastOrderID).copy();
	}
	catch(std::exception& ex)
	{
		return Error(ex.what(), __uuidof(IOrderProvider), E_FAIL);	
	}
	catch(const _com_error& e)
	{
		return utils::ComError2ErrInfo(e, this, L"O'Connor FIX Provider SubmitNewOrder failed.", __uuidof(IOrderProvider));	
	}

	return S_OK;
}


STDMETHODIMP COrdersFixProvider::raw_AlterOrder(BSTR OrderID, FoOrderCancelReplaceRequest* Request, BSTR* AlteredOrderID)
{
	if(!Request || !OrderID || !AlteredOrderID)
		return E_POINTER;

	try
	{
		m_LastOrderID++;
		_SaveSettings();

		FIX42::OrderCancelReplaceRequest message;
		_FillOrderCancelReplaceRequest(Request, message);
		
		if(m_bsSenderSubID.length())
		{
			FIX::SenderSubID senderSubID;
			message.getHeader().setField(senderSubID.getField(), (LPCTSTR) m_bsSenderSubID );
		}

		*AlteredOrderID = _bstr_t((LPCTSTR)m_LastOrderID).copy();
		FIX::Session::sendToTarget(message, m_cussrentSession);
	}
	catch(std::exception& ex)
	{
		return Error(ex.what(), __uuidof(IOrderProvider), E_FAIL);	
	}
	catch(const _com_error& e)
	{
		return utils::ComError2ErrInfo(e, this, L"O'Connor FIX Provider AlterOrder failed.", __uuidof(IOrderProvider));	
	}

	return S_OK;
}

STDMETHODIMP COrdersFixProvider::raw_CancelOrder(BSTR OrderID, FoOrderCancelRequest* Request, BSTR* CanceledOrderID)
{
	if(!Request || !OrderID || !CanceledOrderID)
		return E_POINTER;

	try
	{
		m_LastOrderID++;

		_SaveSettings();

		FIX42::OrderCancelRequest message;
		_FillOrderCancelRequest(Request, message);

		if(m_bsSenderSubID.length())
		{
			FIX::SenderSubID senderSubID;
			message.getHeader().setField(senderSubID.getField(), (LPCTSTR) m_bsSenderSubID );
		}

		*CanceledOrderID = _bstr_t((LPCTSTR)m_LastOrderID).copy();
		FIX::Session::sendToTarget(message, m_cussrentSession);
	}
	catch(std::exception& ex)
	{
		return Error(ex.what(), __uuidof(IOrderProvider), E_FAIL);	
	}
	catch(const _com_error& e)
	{
		return utils::ComError2ErrInfo(e, this, L"O'Connor FIX Provider CancelOrder failed.", __uuidof(IOrderProvider));	
	}

	return S_OK;
}

STDMETHODIMP COrdersFixProvider::raw_RequestOrderStatus(BSTR OrderID, FoOrderStatusRequest* Request)
{
	if(!Request || !OrderID)
		return E_POINTER;

	try
	{

		FIX42::OrderStatusRequest message;
		_FillOrderStatusRequest(Request, message);

		if(m_bsSenderSubID.length())
		{
			FIX::SenderSubID senderSubID;
			message.getHeader().setField(senderSubID.getField(), (LPCTSTR) m_bsSenderSubID );
		}

		FIX::Session::sendToTarget(message, m_cussrentSession);
	}
	catch(std::exception& ex)
	{
		return Error(ex.what(), __uuidof(IOrderProvider), E_FAIL);	
	}
	catch(const _com_error& e)
	{
		return utils::ComError2ErrInfo(e, this, L"O'Connor FIX Provider RequestOrderStatus failed.", __uuidof(IOrderProvider));
	}

	return S_OK;
}



void COrdersFixProvider::onMessage(const FIX42::ExecutionReport& message, const FIX::SessionID& sessionID)
{
	try
	{
		FoExecutionReport Report;
		ZeroMemory(&Report, sizeof(FoExecutionReport));

		_ExtractExecutionReport(message, &Report);

		Fire_OnExecutionReport(&Report);
	}
	catch(const _com_error& e)
	{
		CComErrorWrapper::ThrowError(utils::ComError2ErrInfo(e, this,  L"O'Connor FIX Provider OnExecutionReport failed.", __uuidof(IOrderProvider)));
	}
}

void COrdersFixProvider::onMessage(const FIX42::OrderCancelReject& message, const FIX::SessionID& sessionID)
{
	_variant_t	vtSet;
	_bstr_t		bsValue;

	try
	{
		FoOrderCancelReject  Reject;
		ZeroMemory(&Reject,sizeof(FoOrderCancelReject));


		_ExtractOrderCancelReject(message, &Reject);

		_bstr_t bsReason;
		if (Reject.CxlRejReason = enFoCrrUnknownOrder)
			bsReason = L"Unknown order";
		else if(Reject.CxlRejReason == enFoCrrTooLate)
			bsReason = L"Too late to cancel";

		Fire_OnCancelReject(bsReason, &Reject);
	}
	catch(const _com_error& e)
	{
		CComErrorWrapper::ThrowError(utils::ComError2ErrInfo(e, this,  L"O'Connor FIX Provider OnCancelReject failed.", __uuidof(IOrderProvider)));
	}
};

void COrdersFixProvider::_GetSessionParams(const FIX::SessionID& sessionID)
{
	try
	{
		FIX::Dictionary dicSettings = m_pSettings->get(sessionID);

		if(dicSettings.has("SenderSubID"))
			m_bsSenderSubID = ::SysAllocString(CA2W(dicSettings.getString("SenderSubID").c_str()));
		else
			m_bsSenderSubID = ::SysAllocString(L"");

		if(dicSettings.has("ClientID"))
			m_bsClientID = ::SysAllocString(CA2W(dicSettings.getString("ClientID").c_str()));
		else
			m_bsClientID = ::SysAllocString(L"");

		if(dicSettings.has("Rule80A"))
		{
			std::string sClientCategory = dicSettings.getString("Rule80A");
			m_chClientCategory = sClientCategory[0];
		}
		else
			m_chClientCategory = '0'; // Default Value - ZERO '0': Not used in both FIX Spec and FirstTrades

		if(dicSettings.has("OpenClose"))
		{
			m_bIsOpeningPosition = dicSettings.getBool("OpenClose");
		}
		else
			m_bIsOpeningPosition = false;

		//if(dicSettings.has("TimeInForce"))
		//{
		//	std::string sTimeInForce = dicSettings.getString("TimeInForce");
		//	m_chTimeInForce = sTimeInForce[0];
		//}
		//else
		//	m_chTimeInForce = '0';

		//if(dicSettings.has("ClearingAccount"))
		//{
		//	m_bUseClearingAcc = dicSettings.getBool("ClearingAccount");
		//}
		//else
		//	m_bUseClearingAcc = false;
	}
	catch(const _com_error&) {}
}


STDMETHODIMP COrdersFixProvider::raw_Connect()
{

	try
	{
		DWORD dwResult = S_OK;
		_ReadSettings();

		CXMLParamsHelper XMLParams;
		XMLParams.LoadXMLParams();

		_bstr_t sbsUserGroup;
		_bstr_t sbsValue;
  const _bstr_t bsEmptyString(L"");
		_bstr_t sbsFullXMLKey(SETTINGS_MAIN_XML_KEY);

		XMLParams.GetUserGroup(sbsUserGroup.GetAddress());
		sbsFullXMLKey+=sbsUserGroup;

		XMLParams.GetMainXMLString(sbsFullXMLKey, SETTINGS_XML_CONFIGFILE, &sbsValue);
		//XMLParams.GetXMLString(SETTINGS_USER_XML_KEY, SETTINGS_XML_CLIENT, &m_bsClientID, bsEmptyString);
		XMLParams.GetXMLString(SETTINGS_USER_XML_KEY, SETTINGS_XML_EXECBROKER, &m_bsExecBroker, bsEmptyString);

		CString ConfigurationFile = (LPTSTR)sbsValue;

		if(ConfigurationFile.GetLength())
		{

			m_pSettings     = boost::shared_ptr<FIX::SessionSettings>(new FIX::SessionSettings(ConfigurationFile.GetString()));
			m_pStoreFactory = boost::shared_ptr<FIX::FileStoreFactory>(new FIX::FileStoreFactory(*m_pSettings));
			m_pLogFactory   = boost::shared_ptr<FIX::FileLogFactory>(new FIX::FileLogFactory(*m_pSettings));

			boost::shared_ptr<FIX::SocketInitiator> pInitiator = boost::shared_ptr<FIX::SocketInitiator>(new FIX::SocketInitiator(*this, *m_pStoreFactory, *m_pSettings, *m_pLogFactory));
			if( pInitiator != NULL )
			{
				m_pInitiator = pInitiator;
				m_pInitiator->start();
				dwResult = ERROR_SUCCESS;
			}
			else
				dwResult = GetLastError();
		}
		else
			dwResult = E_FAIL;
	}
	catch(FIX::ConfigError& re)
	{
		std::string sMsg("FIX Configuration Error:");
		sMsg += re.what();
		return Error(sMsg.c_str(), __uuidof(IOrderProvider), E_FAIL);

	}
	catch(FIX::RuntimeError& re)
	{
		std::string sMsg("FIX Runtime Error:");
		sMsg += re.what();
		return Error(sMsg.c_str(), __uuidof(IOrderProvider), E_FAIL);
	}

	catch(_com_error& e)
	{
		return utils::ComError2ErrInfo(e, this, L"O'Connor FIX Provider Connect failed.", __uuidof(IOrderProvider));
	}

	return S_OK;
}

STDMETHODIMP COrdersFixProvider::raw_Disconnect()
{
	try
	{
		if(m_pInitiator)
		{
			m_pInitiator->stop();
			m_pInitiator = boost :: shared_ptr<FIX::SocketInitiator>();

			m_pStoreFactory = boost::shared_ptr<FIX::FileStoreFactory>();
			m_pLogFactory   = boost::shared_ptr<FIX::FileLogFactory>();     
			m_pSettings     = boost::shared_ptr<FIX::SessionSettings>();

		}

	}
	catch(FIX::ConfigError& re)
	{
		std::string sMsg("FIX Configuration Error:");
		sMsg += re.what();
		return Error(sMsg.c_str(), __uuidof(IOrderProvider), E_FAIL);

	}
	catch(FIX::RuntimeError& re)
	{
		std::string sMsg("FIX Runtime Error:");
		sMsg += re.what();
		return Error(sMsg.c_str(), __uuidof(IOrderProvider), E_FAIL);
	}

	catch(_com_error& e)
	{
		return utils::ComError2ErrInfo(e, this, L"O'Connor FIX Provider Disconnect failed.", __uuidof(IOrderProvider));	
	}
	return S_OK;
}

