#ifndef __VPVOLAPROVIDER_H__
#define __VPVOLAPROVIDER_H__

#include "EodGeneral.h"
#include "VpUndVolaSource.h"

namespace EODVP
{

/////////////////////////////////////////////////////////////////////////////
//
class CVpVolaProvider :  public EODCO::CEgTraceable
{
public:
	CVpVolaProvider(void);
	~CVpVolaProvider(void);

	void Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr) throw (exception);
	void Disconnect() throw (exception);

	CVpUndVolaSourcePtr UndVolaSource(const CString& sUndSymbol, SYMBOL_TYPE enType);

private:
	IVolatilitySourcePtr		m_spVolaSource;
};

// object pointer
typedef boost::shared_ptr<CVpVolaProvider>		CVpVolaProviderPtr;

};

#endif //#define __VPVOLAPROVIDER_H__