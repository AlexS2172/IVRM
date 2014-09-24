#ifndef __EGIVPROVIDER_H__
#define __EGIVPROVIDER_H__

#include "EgHttp.h"

class EGHTTP_API CEgIvProvider : public CEgHttpProvider
{
public:
//	CEgIvProvider(){};

	// Returns SID asked from <server address>/auth.asp?name=<sAccount>
	// and then encoded using GetMDString and sPassword
	CEgString Login();
	
	// analyzing the answer from server (detecting logic errors)
	static void CheckErrorMessage(CEgString strData);

	// Additional check of data for IV errors
	virtual void ReadAllData(CEgString& rstrData);

	// Initialize http session for IVolatility.com server.
	void InitIvSession(const CEgHttpConnectionParam& cpParams, const CEgAccount& crIvLogin);

protected:
	CEgAccount m_accIvLogin;
};

#endif

