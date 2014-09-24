#ifndef __EGHTTP_COMMON_H__
#define	__EGHTTP_COMMON_H__

/////////////////////////////////////////////////////////////////////////////
// disable warnings on extern before template instantiation
#pragma warning (disable : 4231)

#include <AtlStr.h>
using namespace ATL;

#ifdef EGHTTP_EXPORTS
	#define EGHTTP_API __declspec(dllexport)
	#define EGHTTP_EXT
#else
	#define EGHTTP_API __declspec(dllimport)
	#define EGHTTP_EXT extern 
#endif

/////////////////////////////////////////////////////////////////////////////
// This piece of code need to correct exporting CEgString. Don't remove this lines.
template<class T> 
class CEgStringHelper : public CStringT<T, StrTraitATL<T, ChTraitsCRT<T> > >{};
EGHTTP_EXT template class EGHTTP_API CEgStringHelper<TCHAR>;

/////////////////////////////////////////////////////////////////////////////
// instantiate class ATL::CStringT<...>
EGHTTP_EXT template class EGHTTP_API CStringT<TCHAR, StrTraitATL<TCHAR, ChTraitsCRT<TCHAR> > >;
typedef CStringT<TCHAR, StrTraitATL<TCHAR, ChTraitsCRT<TCHAR> > > CEgString;
EGHTTP_EXT template class EGHTTP_API CHeapPtr<BYTE, ATL::CCRTAllocator>;

/////////////////////////////////////////////////////////////////////////////
//
enum enumEgHttpProxySettings
{
	enEgPsDisabled = 0,	// proxy isn't used
	enEgPsCustom   = 1,	// proxy is used with custom login/password
	enEgPsDefault  = 2	// get information about proxy from InternetExplorer settings
};

/////////////////////////////////////////////////////////////////////////////
//
enum enumEgHttpProxyAuth
{
	enEgPaDisabled = 0,
	enEgPaEnabled  = 1
};

/////////////////////////////////////////////////////////////////////////////
//
struct EGHTTP_API CEgAccount
{
	CEgAccount(){}
	CEgAccount(CEgString strLogin, CEgString strPassword):
		m_strLogin(strLogin), m_strPassword(strPassword){}

	CEgString m_strLogin;
	CEgString m_strPassword;
};

/////////////////////////////////////////////////////////////////////////////
//
struct EGHTTP_API CEgHttpConnectionParam
{
	CEgHttpConnectionParam() : m_lServerPort(0), m_lProxyPort(0), 
		m_enProxySettings(enEgPsDefault), m_enProxyAuth(enEgPaDisabled){}

	CEgString m_strServerAddress;
	long m_lServerPort;
	
	enumEgHttpProxySettings m_enProxySettings;
	enumEgHttpProxyAuth m_enProxyAuth;

	CEgString m_strProxyAddress;
	long m_lProxyPort;
	CEgAccount m_ProxyAcount;
};

/////////////////////////////////////////////////////////////////////////////
// This function performs case-insensitive search that makes unnecessary case conversions
LPCTSTR _tcsistr(LPCTSTR ptszStr1, LPCTSTR ptszStr2);

/////////////////////////////////////////////////////////////////////////////
// restore warnings on extern before template instantiation
#pragma warning (default : 4231)

#endif // __EGHTTP_COMMON_H__

