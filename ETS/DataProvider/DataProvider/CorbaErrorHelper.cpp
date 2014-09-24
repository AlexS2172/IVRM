#include "StdAfx.h"
#include "CorbaErrorHelper.h"

//-----------------------------------------------------------------------------//
CCorbaErrorHelper::CCorbaErrorHelper(void)
{
}
//-----------------------------------------------------------------------------//
CCorbaErrorHelper::~CCorbaErrorHelper(void)
{
}
//-----------------------------------------------------------------------------//
std::string CCorbaErrorHelper::ToString(const CORBA::SystemException& err)
{
	std::string errDescription;
	TCHAR buf[50];
	memset(buf, 0, sizeof(TCHAR) * 50);
	
	CORBA::Any tmp;
	tmp <<= err;
	CORBA::TypeCode_var tc = tmp.type();

	errDescription = std::string(tc->name()) + _T(" Description: ") + err.NP_minorString();
	errDescription += _T(" Minor code: ");
	errDescription += _ltoa_s(err.minor(), buf, 50, 10);

	return errDescription;
}
//-----------------------------------------------------------------------------//
std::string CCorbaErrorHelper::ToString(const CORBA::Exception& err)
{
	CORBA::Any tmp;
	tmp <<= err;
	CORBA::TypeCode_var tc = tmp.type();
	return std::string(tc->name());
}
//-----------------------------------------------------------------------------//
std::string CCorbaErrorHelper::ToString(const omniORB::fatalException& err)
{
	return std::string(err.errmsg());
}
//-----------------------------------------------------------------------------//
