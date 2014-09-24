// TransportTest2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class CQuotePublish : public CThreadClass
{
	void Do()
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		int counter = 1;
		HRESULT hr;
		DWORD dwRes;
		
		try
		{
			ISubManagerPtr pSubMng;
			IPubManagerPtr pPubMng;

 			hr = pSubMng.CreateInstance(CLSID_MsgManager);
			
			if(pSubMng == NULL)
				throw _com_error(E_FAIL);
			pSubMng->QueryInterface(IID_IPubManager,(void**)&pPubMng);
			
			if(pPubMng == NULL)
				throw _com_error(E_FAIL);

			ISymbolObjectPtr pSymbol;
			pSymbol.CreateInstance(CLSID_SymbolObject);

			if(pSymbol == NULL)
				throw _com_error(E_FAIL);

			IQuotePtr pQuote;
			pQuote.CreateInstance(CLSID_Quote);

			if(pQuote == NULL)
				throw _com_error(E_FAIL);

			pSymbol->Set(enStStock,L"MSFT",L"NASD",L"USD");

			Sleep(10000);

			while(true)
			{
				dwRes = WaitForSingleObject(m_hStopEvent, 0);
				
				if(dwRes == WAIT_OBJECT_0)
					throw _com_error(S_OK); // stop
				
				pQuote->put_AskPrice(51 + counter);
				pQuote->put_AskSize(1000);
				pQuote->put_BidPrice(49 + counter);
				pQuote->put_BidSize(2000);
				pQuote->put_LastPrice(50 + counter);

				long lResult;
  				hr = pPubMng->raw_PubQuote(pSymbol,pQuote,&lResult);
					
				counter++;
			}
		}
		catch(_com_error cer)
		{
			Trace(_T("Error occured (%x)"),cer.Error());
		}

		CoUninitialize();
	}

};

class CTradePublish : public CThreadClass
{
	void Do()
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		int counter = 1;
		HRESULT hr;
		DWORD dwRes;
		
		try
		{
			ISubManagerPtr pSubMng;
			IPubManagerPtr pPubMng;

			hr = pSubMng.CreateInstance(CLSID_MsgManager);

			if(pSubMng == NULL)
				throw _com_error(E_FAIL);

			pSubMng->QueryInterface(IID_IPubManager,(void**)&pPubMng);
			
			if(pPubMng == NULL)
				throw _com_error(E_FAIL);
			
			ITradeUpdatePtr pTrade;
			pTrade.CreateInstance(CLSID_TradeUpdate);

			if(pTrade == NULL)
				throw _com_error(E_FAIL);

			Sleep(10000);

			while(true)
			{
				dwRes = WaitForSingleObject(m_hStopEvent, 0);
				
				if(dwRes == WAIT_OBJECT_0)
					throw _com_error(S_OK); // stop
				
				pTrade->put_SeqNum(counter);
				pTrade->put_TradeID(1);
				pTrade->put_Quantity(1000);
				pTrade->put_Price(25.6);
				pTrade->put_IsBuy(true);
				pTrade->put_TraderID(2);
				pTrade->put_BookID(3);
				pTrade->put_StrategyID(4);
				pTrade->put_BrokerID(5);
				pTrade->put_BrokerCommission(0.1);
				pTrade->put_ClearingBrokerID(6);
				pTrade->put_ClearingBrokerCommission(0.05);
				pTrade->put_SpotReference(73.9);
				pTrade->put_TradedIV(0.67);

				long lResult;
  				hr = pPubMng->raw_PubTradeUpdate(pTrade,&lResult);
				
				counter++;
			}
		}
		catch(_com_error cer)
		{
			Trace(_T("Error occured (%x)"),cer.Error());
		}

		CoUninitialize();
	}

};

class CVolatilityPublish : public CThreadClass
{
	void Do()
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		int counter = 1;
		HRESULT hr;
		DWORD dwRes;
		
		try
		{
			ISubManagerPtr pSubMng;
			IPubManagerPtr pPubMng;
 			
			hr = pSubMng.CreateInstance(CLSID_MsgManager);

			if(pSubMng == NULL)
				throw _com_error(E_FAIL);

			pSubMng->QueryInterface(IID_IPubManager,(void**)&pPubMng);
			
			if(pPubMng == NULL)
				throw _com_error(E_FAIL);

			ISymbolObjectPtr pSymbol;
			pSymbol.CreateInstance(CLSID_SymbolObject);

			if(pSymbol == NULL)
				throw _com_error(E_FAIL);

			IVMESurfacePtr pVola;
			pVola.CreateInstance(CLSID_VMESurface);

			if(pVola == NULL)
				throw _com_error(E_FAIL);

			pSymbol->Set(enStStock,L"MSFT",L"NASD",L"USD");

			Sleep(10000);

			while(true)
			{
				dwRes = WaitForSingleObject(m_hStopEvent, 0);
				
				if(dwRes == WAIT_OBJECT_0)
					throw _com_error(S_OK); // stop
				
				pVola->put_DiscreteAcceleration(VARIANT_TRUE);
				pVola->put_InterpolationFactor(1);
				pVola->put_OptType(3);
				pVola->put_PriceOverride(0);
				pVola->put_SmileAccelerator(1);
				pVola->put_SurfaceID(counter);
				pVola->put_UnderlinePrice(50);

				long lResult;
  				hr = pPubMng->raw_PubVola(pSymbol,pVola,&lResult);
				
				counter++;
			}
		}
		catch(_com_error cer)
		{
			Trace(_T("Error occured (%x)"),cer.Error());
		}

		CoUninitialize();
	}
};

int main(int argc, char* argv[])
{
	CheckLogsFolder();

	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CTradePublish TradePub;
	CQuotePublish	QuotePub;
	CVolatilityPublish VolaPub;

try
{
	/*ISubManagerPtr pSubMng;

 	pSubMng.CreateInstance(CLSID_MsgManager);
			
	if(pSubMng == NULL)
		throw _com_error(E_FAIL);
			
	ISymbolObjectPtr pSymbol;
	pSymbol.CreateInstance(CLSID_SymbolObject);

	if(pSymbol == NULL)
		throw _com_error(E_FAIL);

	pSymbol->Set(enStStock,L"MSFT",L"NASD",L"USD");

	pSubMng->raw_SubVola(pSymbol);
	pSubMng->raw_SubQuote(pSymbol);
	pSubMng->raw_SubTradeUpdate();

	*/QuotePub.Initialize();
	QuotePub.Start();
	
	TradePub.Initialize();
	TradePub.Start();

	VolaPub.Initialize();
	VolaPub.Start();

    HANDLE hStdin; 
    DWORD cNumRead, fdwMode, fdwSaveOldMode; 
    INPUT_RECORD irInBuf[2]; 
 
    // Get the standard input handle. 
 
    hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    if (hStdin == INVALID_HANDLE_VALUE)
		throw -1;
 
    // Save the current input mode, to be restored on exit. 
 
    if (! GetConsoleMode(hStdin, &fdwSaveOldMode) ) 
		throw -1;
 
    // Enable the window input events. 
 
    fdwMode = ENABLE_WINDOW_INPUT; 
    if (! SetConsoleMode(hStdin, fdwMode) ) 
		throw -1;

    // Loop to read and handle the input events. 
 
    while (1) 
    { 
 
        // Wait for the events. 
 
        if (! ReadConsoleInput( 
                hStdin,      // input buffer handle 
                irInBuf,     // buffer to read into 
                2,         // size of read buffer 
                &cNumRead) ) // number of records read 
				throw -1;
 
        // Dispatch the events to the appropriate handler. 
 
        if(irInBuf[0].EventType == KEY_EVENT )
        { 
			if(irInBuf[0].Event.KeyEvent.dwControlKeyState & LEFT_CTRL_PRESSED)
			{
               if(irInBuf[0].Event.KeyEvent.wVirtualKeyCode == 67)
					break; 
			}
        }
    } 
}
catch(...)
{
}

	VolaPub.Stop();
	VolaPub.Uninitialize();
	
	TradePub.Stop();
	TradePub.Uninitialize();

	QuotePub.Stop();
	QuotePub.Uninitialize();
	
	CoUninitialize();
	return 0;

}
