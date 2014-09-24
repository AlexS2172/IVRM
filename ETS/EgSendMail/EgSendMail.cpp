// EgSendMail.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#import "C:\Program Files\Common Files\System\ado\msado25.tlb" no_namespace, named_guids, rename("EOF", "Eof")
#import <cdosys.dll> no_namespace
#include <windows.h>
#include <iostream>
using namespace std;
#include <comdef.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <conio.h>
#include <fstream>
#include <io.h>



#define COM_ISSUE_HR(x) { if( FAILED(hr = x) ) _com_issue_error(hr); }

struct ParamsDescription 
{
	_bstr_t bsSMTPServer;
	_bstr_t bsServerName;
	_bstr_t bsDescription;
	_bstr_t bsMailTo;
	_bstr_t bsUserName;
};

int SendEmail(ParamsDescription& pParams);
void CleanUp(IMessage** pMsg, IConfiguration** pConfig);
void GetExecutionParams(int argc, _TCHAR* argv[], ParamsDescription& Params);
void GetParamsFromFile(ParamsDescription& Params);
void GetEnviromentParams(ParamsDescription& Params);
void InitDefaultSettings(ParamsDescription& Params);	


int _tmain(int argc, _TCHAR* argv[])
{
	ParamsDescription  pParams;

	InitDefaultSettings( pParams );
	// get params from command line
	GetExecutionParams( argc, argv, pParams );
	// get configuration params from configuration file
	GetParamsFromFile( pParams );
	// send mail main function used CODNTS
	SendEmail( pParams );
	Sleep(1000);
	return 0;
}

void InitDefaultSettings(ParamsDescription& Params)
{
	Params.bsMailTo			=	_bstr_t("achuchev@egartech.com");
	Params.bsDescription	=	_bstr_t("default message");
	Params.bsSMTPServer		=	_bstr_t("192.168.8.3");
}

void GetEnviromentParams(ParamsDescription& Params)
{
	wchar_t wcpValue[256] = {0};
	wchar_t wcpName[256] = {0};
	DWORD lNameLen = 256;

	lNameLen = 256;
	if (!GetComputerNameW(wcpName, &lNameLen))
		Params.bsServerName = _bstr_t(_T("SERVER"));
	else
		Params.bsServerName = _bstr_t(wcpName);

	lNameLen = 256;
	if (!GetUserNameW(wcpName, &lNameLen))
		Params.bsUserName = _bstr_t(_T("USER"));
	else
		Params.bsUserName = _bstr_t(wcpName);
}

void GetParamsFromFile(ParamsDescription& Params)
{
#define CFG_FILE_NAME "egsmconfig.ini"
	char szBuf[64] = {0};
	std::fstream f_cfg;

	GetEnviromentParams(Params);

	if (_access(CFG_FILE_NAME, 04) != -1)
	{
		f_cfg.open(CFG_FILE_NAME);
		while (!f_cfg.eof())
		{
			f_cfg>>szBuf;
			CString strParam(szBuf);
			if (strParam.Find(_T("-IP=")) >= 0)
			{
				strParam.Replace(_T("-IP="), _T(""));
				strParam.Replace(_T("\""), _T(""));
				if (strParam.TrimLeft().TrimRight().GetLength() > 0)
				{
					Params.bsSMTPServer = _bstr_t(strParam);
					continue;
				}
			}
			else if (strParam.Find(_T("-Mail")) >= 0)
			{
				strParam.Replace(_T("-Mail="), _T(""));
				strParam.Replace(_T("\""), _T(""));
				if (strParam.TrimLeft().TrimRight().GetLength() > 0)
				{
					Params.bsMailTo = _bstr_t(strParam);
					continue;
				}
			}
		}
		f_cfg.close();
	}
};

void GetExecutionParams(int argc, _TCHAR* argv[], ParamsDescription& Params)
{
	CString	strArgv;
	int i = -1;

	wchar_t wcpValue[256] = {0};
	wchar_t wcpName[256] = {0};

	GetEnviromentParams(Params);

	while (++i < argc && argc > 1)
	{
		strArgv = CString(argv [i]);
		if (strArgv.Find(_T("-IP=")) >= 0)
		{
			strArgv.Replace(L"-IP=", L"");
			if (strArgv.TrimLeft().TrimRight().GetLength() > 0)
				Params.bsSMTPServer = _bstr_t(strArgv.GetBuffer());
			continue;
		}
		else if (strArgv.Find(_T("-DS=")) >= 0)
		{
			strArgv.Replace(L"-DS=", L"");
			if (strArgv.TrimLeft().TrimRight().GetLength() > 0)
				Params.bsDescription = _bstr_t(strArgv.GetBuffer());
			continue;
		}
		else if (strArgv.Find(_T("-Mail=")) >= 0)
		{
			strArgv.Replace(L"-Mail=", L"");
			if (strArgv.TrimLeft().TrimRight().GetLength() > 0)
				Params.bsMailTo = _bstr_t(strArgv.GetBuffer());
			continue;
		}
	}
}

int SendEmail(ParamsDescription& pParams)
{
	IMessage* pMsg = NULL;
	IConfiguration* pConfig = NULL;
	Fields* pFlds = NULL;
	Field* pFld = NULL;

	HRESULT hr;

	try
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED);

		if ((hr = CoCreateInstance(__uuidof(Configuration), NULL,
			CLSCTX_INPROC_SERVER, __uuidof(IConfiguration), (void**)&pConfig)) !=
			S_OK)
		{
			CleanUp(NULL, &pConfig);
			throw hr;
		}

		if ((hr = CoCreateInstance(__uuidof(Message), NULL,
			CLSCTX_INPROC_SERVER, __uuidof(IMessage), (void**)&pMsg)) != S_OK)
		{
			CleanUp(&pMsg, &pConfig);
			throw hr;
		}

		pConfig->get_Fields(&pFlds);

		pFlds->get_Item(_variant_t(cdoSendUsingMethod),&pFld);
		pFld->put_Value(_variant_t((int)cdoSendUsingPort));
		pFld->Release();
		pFld = NULL;

		pFlds->get_Item(_variant_t(cdoSMTPServer),&pFld);
		pFld->put_Value(_variant_t(pParams.bsSMTPServer));
		pFld->Release();
		pFld = NULL;

		pFlds->get_Item(_variant_t(cdoSMTPServerPort),&pFld);
		pFld->put_Value(_variant_t(25));
		pFld->Release();
		pFld = NULL;

		pFlds->get_Item(_variant_t(cdoSMTPAuthenticate),&pFld);
		pFld->put_Value(_variant_t(cdoAnonymous));
		pFld->Release();
		pFld = NULL;

		pFlds->Update();
		pFlds->Release();
		pFlds = NULL;

		COM_ISSUE_HR(pMsg->putref_Configuration(pConfig));
		pConfig->Release();
		pConfig = NULL;

		COM_ISSUE_HR (pMsg->put_To(pParams.bsMailTo));
		COM_ISSUE_HR (pMsg->put_From(_bstr_t("RiskSystem@egartech.com")));
		COM_ISSUE_HR (pMsg->put_Subject(_bstr_t("System worning message: ") + pParams.bsServerName));
		COM_ISSUE_HR (pMsg->put_TextBody(_bstr_t("Username: ") + pParams.bsUserName + _bstr_t("\nDescription: ") + pParams.bsDescription));

		COM_ISSUE_HR(pMsg->Send());
		pMsg->Release();
		pMsg = NULL;
		printf("Email set successfully\n");
	}
	catch (_com_error& e)
	{
		hr = e.Error();
		printf("Exception in SendEmail. Code = 0x%08lx, Error message: %S\n", hr, e.ErrorMessage());
		if (e.ErrorInfo())
		{
			printf("COM error: %s\n", hr, e.Description().copy());
		}
		CleanUp(&pMsg, &pConfig);
	}
	catch (...)
	{
		printf("Excepection in SendEmail.\n");
		CleanUp(&pMsg, &pConfig);
	}

	CoUninitialize();

	return 0;
}

void CleanUp(IMessage** pMsg, IConfiguration** pConfig)
{
	printf("Starting cleanup.\n");
	try
	{
		if (pConfig != NULL && *pConfig != NULL)
		{
			(*pConfig)->Release();
			*pConfig = NULL;
		}
		if (pMsg != NULL && *pMsg != NULL)
		{
			(*pMsg)->Release();
			*pMsg = NULL;
		}
	}
	catch (...)
	{
		printf("Excepection in CleanUp.\n");
	}
}
