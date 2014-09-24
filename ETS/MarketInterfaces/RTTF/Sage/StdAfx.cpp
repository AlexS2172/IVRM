// stdafx.cpp : source file that includes just the standard includes
//	Sage.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

long GetCommandLineParams(UserGroups& clParams)
{
	CString	strArgv;
	long	nParamsCount = 0;
	int		i = -1;

	clParams.clear();

	//parse command line params
	while (++i < __argc)
	{
		strArgv = CString(__argv [i]);
		if (strArgv.Find(_T("-GRP=")) >= 0)
		{
			strArgv.Replace(_T("-GRP="), _T(""));
			if (strArgv.Trim().GetLength() > 0)
				if (clParams.find(_bstr_t(strArgv)) == clParams.end())
					clParams.insert(UserGroups::value_type(_bstr_t(strArgv), nParamsCount++));
		}
	}

	// if nothing found in command line params 
	// then use default user group
	if (nParamsCount == 0)
	{	
		_bstr_t bsDefUserGroup;
		CXMLParamsHelper XMLParams;
		XMLParams.LoadXMLParams();
		XMLParams.GetUserGroup(bsDefUserGroup.GetAddress());
		clParams.insert(UserGroups::value_type(bsDefUserGroup, nParamsCount++));
	}
	return nParamsCount;
}


