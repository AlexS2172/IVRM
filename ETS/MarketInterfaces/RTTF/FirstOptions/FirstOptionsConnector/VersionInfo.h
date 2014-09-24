// VersionInfo.h: interface for the CVersionInfo class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
using std::string;

class CVersionInfo  
{
public:
	CVersionInfo(const string& FileToQuery = "");
	virtual ~CVersionInfo();

	string GetVersionAsString();

protected:
	VS_FIXEDFILEINFO m_VersionInfo;
};

//#endif // !defined(AFX_VERSIONINFO_H__8CD17DD8_3C69_40A8_88FE_0648E1EFAD72__INCLUDED_)
