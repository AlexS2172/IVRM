//**************************************************************************************************************//
// abstract:	interface & implementation of COmException
// content:		CEgException
//**************************************************************************************************************//
#pragma once

//**************************************************************************************************************//
// includes
//**************************************************************************************************************//
#include <string>
#include <EgLib/EgLibTrace.h>

namespace EgLib
{
class CEgExceptionBase
{
protected:
// data members
	std::string		m_sDescription;
	DWORD			m_dwErrCode;
	enLogLevel		m_enLogLevel;
public:

//  constructors/destructor
	CEgExceptionBase(enLogLevel enLevel, DWORD dwErrCode, const std::string& rsDescription)
		:	m_sDescription(rsDescription)
		,	m_dwErrCode(dwErrCode)
		,	m_enLogLevel(LogDebug)
	{}

	CEgExceptionBase(const std::string& rsDescription)
		:	m_sDescription(rsDescription)
		,	m_dwErrCode(ERROR_SUCCESS)
		,   m_enLogLevel(LogError)
	{}

	CEgExceptionBase(const CEgExceptionBase& rhs)
	{
		Copy(rhs);
	}

	CEgExceptionBase& operator= (const CEgExceptionBase& rhs)
	{
		Copy(rhs);
		return *this;
	}

protected:
	void Copy(const CEgExceptionBase& rhs)
	{
		m_sDescription	= rhs.m_sDescription;
		m_dwErrCode		= rhs.m_dwErrCode;
		m_enLogLevel	= rhs.m_enLogLevel;
	}
public:


//  operations
	const std::string GetErrorDescription() const
	{
		return m_sDescription;
	}
	DWORD GetErrorCode() const
	{
		return m_dwErrCode;
	}
	enLogLevel  GetLogLevel() const
	{
		return m_enLogLevel;
	}
};

} // namespace OmLib