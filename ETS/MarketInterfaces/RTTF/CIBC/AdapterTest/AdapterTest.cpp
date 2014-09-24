  // AdapterTest.cpp : Defines the entry point for the console application.
//


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "stdafx.h"
#include "AdapterTest.h"
#include "Trade.h"

static CDBConnection g_Connection;
static _bstr_t g_bsDBPath;

void PutTestTradesInfo(unsigned int nTradeCount);
void GenerateTrades(TradesArray &arTrades, unsigned int nTradeCount);
void ClearTrades(TradesArray &arTrades);
void PutTradesIntoDB(TradesArray &arTrades);
void PrintTrade(CTrade *pTrade);
long  PutTrade(CTrade *pTrade);
template <class T>
LONG GetRegistrySetting(LPCTSTR lpszValue, T& Value);
bool ReadDBPath();

int main(int argc, char* argv[])
{
	int nRetCode = 0;
    unsigned int nTradeCount = 0;

    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    if (ReadDBPath())
    {
        bool bIsExit = false;
        while (!bIsExit)
        {
            char cY = 'y';

            cout << "Input trades count : "; 
            cin  >> nTradeCount; 
            cout << endl; 
        
            PutTestTradesInfo(nTradeCount);

            cout << endl; 
            cout << "Input more trades (y/n)? : "; 
            cin  >> cY; 
            cout << endl; 

            if ('n' == cY)
                bIsExit = true;
        }
        //getche(); 
    }
    else
    {
        cout << "Please write Exchange DB Path in the Registry" << endl; 
    }


    CoUninitialize();
    return nRetCode;
}

bool ReadDBPath()
{
    LONG lResult;
	CString bsExchange_DB_Path;
    
    // to get Exchange DB
    lResult = GetRegistrySetting(SETTINGS_DB_SOURCE, bsExchange_DB_Path);
    if (lResult == ERROR_SUCCESS)
    {
        if (!bsExchange_DB_Path.IsEmpty())
        {
            g_bsDBPath = bsExchange_DB_Path;
            return true;
        }
    }

    return false;
}

void PutTestTradesInfo(unsigned int nTradeCount)
{
    TradesArray arTrades;

    GenerateTrades(arTrades, nTradeCount);
    PutTradesIntoDB(arTrades);
    ClearTrades(arTrades);
}

void GenerateTrades(TradesArray &arTrades, unsigned int nTradeCount)
{
    int  i = 0;
    static long nBaseTradeID = 1000000;

    ClearTrades(arTrades);

    srand(time(0));
    
    for(i = 0; i < nTradeCount; ++i)
    {
        CTrade *pTrade = new CTrade;

        pTrade->vcAcronym = g_AcctIDs[ rand() % g_AcctIDsCount];

        pTrade->nTradeID = ++nBaseTradeID;

        pTrade->siStatus = 0;
        
        pTrade->vcSymbol = g_Symbols[rand() % g_SymbolsCount ]; //[15]

        pTrade->fUnits = (rand() % 1000) * ( sin(rand()) > 0 ? 1 : -1 );

        pTrade->vcAction = (pTrade->fUnits < 0)?(_T("S")):(_T("B")); //[15]

        pTrade->fPrice = floor( 100.0 * fabs( sin( rand() % 100 ) ) );

        pTrade->dtTradeDate = CTime::GetCurrentTime();
        
        pTrade->vcHedgeGroup = g_HedgeGroups[rand() % g_HedgeGroupsCount ]; //[15]
        
        arTrades.push_back(pTrade);
    }
}


void ClearTrades(TradesArray &arTrades)
{
    TradesArrayIter iter;

    if (!arTrades.empty())
    {
        for(iter = arTrades.begin(); iter != arTrades.end();  ++iter)
        {
            CTrade *pTrade = *iter;
            if (pTrade) 
            {
                delete pTrade;
                pTrade = NULL;
            }
        }

        arTrades.clear();
    }
}

void PutTradesIntoDB(TradesArray &arTrades)
{
    TradesArrayIter iter;

	try
	{
        g_Connection.Open(g_bsDBPath);
		cout << "Connection to databases is established." << endl;
	}
	catch(_com_error& e)
	{
		g_Connection.Close();

		cout << "Failed to connect to database" << endl;

        if (g_Connection.GetErrorMessage().length() != 0)
		    cout << (char*) g_Connection.GetErrorMessage() << endl;
        
        if (e.Description().length() != 0)
		    cout << (char*) e.Description() << endl;
        return;
	}

    cout << endl;
    
    long nTime = 0;

    for(iter = arTrades.begin(); iter != arTrades.end();  ++iter)
    {
        CTrade *pTrade = *iter;
        if (pTrade) 
        {
            nTime += PutTrade(pTrade);
            PrintTrade(pTrade);
        }
    }

    cout << endl;
    cout << "Inserting time of " << arTrades.size() << " trades - ";
    
    div_t result = div(nTime, 1000);  

    CString sTime;
    sTime.Format("%d,%03d", result.quot, result.rem);
    cout << (LPCSTR) sTime << " (sec)" << endl;
    
	g_Connection.Close();
}

void PrintTrade(CTrade *pTrade)
{
     CString sPrice;
     sPrice.Format("%.2f", pTrade->fPrice);

     cout << pTrade->vcAcronym << "\t";
     cout << pTrade->nTradeID << "\t";
     cout << pTrade->siStatus << "\t";
     cout << pTrade->vcSymbol << "\t";
     cout << pTrade->fUnits << "\t";
     cout << pTrade->vcAction << "\t";
     cout << (LPCSTR) sPrice<< "\t";
     cout << (LPCSTR) pTrade->dtTradeDate.Format("%Y/%m/%d %H:%M:%S") << "\t";
     cout << pTrade->vcHedgeGroup;
     cout << endl;
}

long PutTrade(CTrade *pTrade)
{
    DWORD dwStart = GetTickCount();

    CClientRecordset rs;
    CStoredProc<> sp(g_Connection, L"sp_Trade_Insert");
    
    sp << pTrade->vcAcronym.c_str();
    sp << pTrade->nTradeID;
    sp << pTrade->siStatus;
    sp << pTrade->vcSymbol.c_str();
    sp << pTrade->vcAction.c_str();
    sp << pTrade->fUnits;
    sp << pTrade->fPrice;
    sp << (LPCSTR) pTrade->dtTradeDate.Format("%Y/%m/%d %H:%M:%S"); 
    sp << pTrade->vcHedgeGroup.c_str();

    rs.Open (sp);
    return GetTickCount() - dwStart;
}