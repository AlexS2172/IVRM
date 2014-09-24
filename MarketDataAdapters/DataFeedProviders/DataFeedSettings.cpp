#include "stdafx.h"
#include "DataFeedSettings.h"

std::string CDataFeedSettings::m_userName;
std::string CDataFeedSettings::m_password;
std::string CDataFeedSettings::m_remoteObj;
std::string CDataFeedSettings::m_provider;
std::string CDataFeedSettings::m_endpoint;
std::string CDataFeedSettings::m_ORBTraceLevel = "35";
CORBA::Short CDataFeedSettings::m_compression;
CORBA::ULong CDataFeedSettings::m_serverTimeout;
int CDataFeedSettings::m_attempts = 10;
long CDataFeedSettings::log_lifetime = 2;
long CDataFeedSettings::log_level = 2;
long CDataFeedSettings::pull_timeout = 1000;
std::string CDataFeedSettings::log_directory = "";

bool CDataFeedSettings::UseXML = false;

std::string CDataFeedSettings::GetProcessPath()
{
	TCHAR szBuf[_MAX_PATH] = {0};
	if(GetModuleFileName(NULL, szBuf, sizeof(szBuf)))
	{
		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];
		TCHAR szName[_MAX_FNAME];
		_tsplitpath( szBuf, szDrive, szDir, szName, NULL);
		std::string sPath = szDrive;
		sPath += szDir;
		return sPath;		
	}
	throw std::runtime_error("Can't get process path");
}

/*static*/
std::string
CDataFeedSettings::GetUserName() {
	TCHAR buffer[MAX_PATH];
	DWORD buffer_size = MAX_PATH;
	::GetUserName(buffer, &buffer_size);
	return std::string(buffer);
};

std::string CDataFeedSettings::GetProcessFilename()
{
	TCHAR szBuf[_MAX_PATH] = {0};
	if(GetModuleFileName(NULL, szBuf, sizeof(szBuf)))
	{
		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];
		TCHAR szName[_MAX_FNAME];
		_tsplitpath( szBuf, szDrive, szDir, szName, NULL);
		return szName;
	}
	throw std::runtime_error("Can't get process name");
}

void CDataFeedSettings::Initialize()
{
		CEgRegKey regProvideerKey;
		if(ERROR_SUCCESS == regProvideerKey.Open(HKEY_LOCAL_MACHINE, PROVIDER_SETTINGS_ROOT, KEY_READ))
		{
			CoInitializeEx(NULL, COINIT_MULTITHREADED);
			_bstr_t bsFilePath;
			int useXML = 0;
			regProvideerKey.QueryValue(bsFilePath, _T("Configuration File"));
			if (regProvideerKey.QueryValue( useXML, _T("UseXML")) == 0)
				UseXML = useXML == 1;
			if (UseXML)
			{
				MSXML2::IXMLDOMDocument2Ptr spDocument(__uuidof(MSXML2::DOMDocument));
				spDocument->async =VARIANT_FALSE;
				spDocument->resolveExternals = VARIANT_FALSE;
				spDocument->setProperty(_bstr_t(L"SelectionLanguage"), _variant_t(_bstr_t(L"XPath")));
				spDocument->load(bsFilePath);

				std::string strUserName = GetUserName();
				
				_bstr_t bsSession  = L"./Settings/Sessions/Session [@name=\"";
				bsSession += strUserName.c_str();
				bsSession += L"\"]/*";

				MSXML2::IXMLDOMNodeListPtr spList = spDocument->selectNodes(bsSession);
				if(spList!=NULL && spList->length)
				{
					for(int i = 0; i<spList->length; ++i)
					{
						MSXML2::IXMLDOMNodePtr spNode = spList->item[i];
						MSXML2::IXMLDOMNamedNodeMapPtr spAttributes = spNode->attributes;
						MSXML2::IXMLDOMNodePtr spName  =   spAttributes->getNamedItem(L"name");
						MSXML2::IXMLDOMNodePtr spValue =   spAttributes->getNamedItem(L"value");
						if(spName!=NULL && spValue!=NULL)
						{
							if (spName->text == _bstr_t(PROVIDER_NODE))
								CDataFeedSettings::m_provider = spValue->text;
							if (spName->text == _bstr_t(LOG_LIFETIME))
								CDataFeedSettings::log_lifetime = atol(spValue->text);
							if (spName->text == _bstr_t(LOG_LEVEL))
								CDataFeedSettings::log_level = atol(spValue->text);
							if (spName->text == _bstr_t(LOG_DIRECTORY))
								CDataFeedSettings::log_directory = spValue->text;
							if (spName->text == _bstr_t(PULL_TIMEOUT))
								CDataFeedSettings::pull_timeout = atol(spValue->text);
							if (spName->text == _bstr_t(REMOTEOBJECT_NODE))
								CDataFeedSettings::m_remoteObj = spValue->text;
							if (spName->text == _bstr_t(USERNAME_NODE))
								CDataFeedSettings::m_userName = spValue->text;
							if (spName->text == _bstr_t(PASSWORD_NODE))
								CDataFeedSettings::m_password = spValue->text;
							if (spName->text == _bstr_t(COMPRESSION_NODE))
								CDataFeedSettings::m_compression = (spValue->text == _bstr_t("0"))?DDS::COMPRESS_NONE:DDS::COMPRESS_ZIP;
							if (spName->text == _bstr_t(SERVERTIMEOUT_NODE))
								CDataFeedSettings::m_serverTimeout = atol(spValue->text);
							if (spName->text == _bstr_t(SERVERENDPOINT_NODE))
								CDataFeedSettings::m_endpoint = spValue->text;
							if (spName->text == _bstr_t(ORBTRACELEVEL_NODE))
								CDataFeedSettings::m_ORBTraceLevel = spValue->text;
							if (spName->text == _bstr_t(ATTEMPTS_NODE))
								CDataFeedSettings::m_attempts = atol(spValue->text);
						}
					}
				}
			}
			//else
			//{
			//	try
			//	{

			//		std::ostringstream oss;
			//		std::string val = GetProcessPath() + GetProcessFilename() + _T(".ini");
			//		EgLib::CEgLibTraceManager::Trace(EgLib::LogInfo,__FUNCTION__,  val.c_str());
			//		VS::CIniFileParser Ini(val);
			//		Ini.ReadRequiredParameter(_T("MAIN"),_T(PROVIDER_NODE),CDataFeedSettings::m_provider);
			//		Ini.ReadRequiredParameter(_T("MAIN"),_T(REMOTEOBJECT_NODE),CDataFeedSettings::m_remoteObj);
			//		Ini.ReadRequiredParameter(_T("MAIN"),_T(USERNAME_NODE),CDataFeedSettings::m_userName);
			//		Ini.ReadRequiredParameter(_T("MAIN"),_T(PASSWORD_NODE),CDataFeedSettings::m_password);
			//		Ini.ReadRequiredParameter(_T("MAIN"),_T(COMPRESSION_NODE),val);
			//		CDataFeedSettings::m_compression = (val == _T("0"))?DDS::COMPRESS_NONE:DDS::COMPRESS_ZIP;
			//		Ini.ReadRequiredParameter(_T("MAIN"),_T(SERVERTIMEOUT_NODE),val);
			//		CDataFeedSettings::m_serverTimeout = atol(val.c_str());
			//		Ini.ReadRequiredParameter(_T("ORB"),_T(SERVERENDPOINT_NODE),CDataFeedSettings::m_endpoint);
			//		if (Ini.ReadOptionalParameter(_T("ORB"),_T(ORBTRACELEVEL_NODE),val))
			//			CDataFeedSettings::m_ORBTraceLevel = val;
			//		if (Ini.ReadOptionalParameter(_T("MAIN"),_T(ATTEMPTS_NODE),val))
			//			CDataFeedSettings::m_attempts = atol(val.c_str());
			//		oss << "Provider: " << CDataFeedSettings::m_provider << "; ";
			//		oss << "RemoteObject: " << CDataFeedSettings::m_remoteObj << "; ";
			//		oss << "UserName: " << CDataFeedSettings::m_userName << "; ";
			//		oss << "Password: " << CDataFeedSettings::m_password << "; ";
			//		oss << "Compression: " << CDataFeedSettings::m_compression << "; ";
			//		oss << "ServerTimeout: " << CDataFeedSettings::m_serverTimeout << "; ";
			//		oss << "ORBTraceLeve: " << CDataFeedSettings::m_ORBTraceLevel << "; ";
			//		oss << "Endpoint: " << CDataFeedSettings::m_endpoint << "; ";
			//		oss << "Attempts: " << val << "; ";

			//		EgLib::CEgLibTraceManager::Trace(EgLib::LogInfo,__FUNCTION__,  oss.str().c_str());
			//	}
			//	catch (...)
			//	{
			//		TRACE_UNKNOWN_ERROR();
			//		EgLib::CEgLibTraceManager::Trace(EgLib::LogError,__FUNCTION__,  "Error while reading parameters from ini-file!");
			//	}
			//}
		}
}