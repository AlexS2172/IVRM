#ifndef __EODGENERAL_H__
#define __EODGENERAL_H__

#include <map>
#include <CString>
#include <vector>
#include <stdexcept>
using namespace std;

#pragma warning (push)
#pragma warning( disable : 4290 )

#include <ATLComTime.h>
using namespace ATL;

#include <Record.h>

//const LPCTSTR c_strParentRegKey = _T("SOFTWARE\\EGAR\\ETS\\EtsEodServer");
//const LPCTSTR c_strSettingsRegKey = _T("SOFTWARE\\EGAR\\ETS\\EtsEodServer\\Settings");
//const LPCTSTR c_strTaskRegKey = _T("SOFTWARE\\EGAR\\ETS\\EtsEodServer\\Task");

const LPCTSTR c_strParentXMLKey = _T("ETS\\EtsEodServer");
const LPCTSTR c_strSettingsXMLKey = _T("ETS\\EtsEodServer\\Settings");
const LPCTSTR c_strTaskXMLKey = _T("ETS\\EtsEodServer\\Task");

////////////////////////////////////////////
// external type libraries
#import <msado25.tlb> rename("EOF", "AdoEof") rename("BOF", "AdoBof") rename_namespace("ADO")
#import "..\..\tlb\MsgStruct.tlb" named_guids no_namespace //rename_namespace("MSGSTR")
#import "..\..\tlb\ETSManager.tlb" named_guids rename_namespace("MGR")
#import "..\..\tlb\VADBLayout.tlb" named_guids no_namespace //rename_namespace("VSDB")
#import "..\..\tlb\VolatilitySources.tlb" named_guids no_namespace //rename_namespace("VS")
#import "..\..\..\MarketDataAdapters\tlb\PriceProviders.tlb" named_guids rename_namespace ("PP")

#import "../../tlb/ETSXMLParams.tlb" no_namespace named_guids
#include "XMLParamsHelper.h"

template<typename retValue, typename Collection, typename _key>
retValue GetCollectionItem(boost::shared_ptr<Collection>& collectionValue, _key KeyValue)
{
	retValue retVal;
	Collection::iterator itr = collectionValue->find(KeyValue);
	if(itr != collectionValue->end())
		retVal = itr->second;
	return retVal;
}

template<typename Collection, typename _key>
bool IsExists(boost::shared_ptr<Collection>& collectionValue, _key KeyValue)
{
	Collection::iterator itr = collectionValue->find(KeyValue);
	return itr != collectionValue->end();
}


////////////////////////////////////////////
// external lib
#include <OptionCalc/OptionCalc.h>

////////////////////////////////////////////
// IDL generated
#include "EtsEodServer.h"

////////////////////////////////////////////
// General objects
#include "EodErrors.h"
#include "EsDefs.h"
#include "EsTrace.h"

////////////////////////////////////////////
// Settings manager specific
#include "SmSettingsMgr.h"

////////////////////////////////////////////
// Volatility provider specific
#include "VpVolaProvider.h"
#include "VpUndVolaSource.h"

////////////////////////////////////////////
// Market structure specific
namespace EODMS
{
	typedef vector<REGULAR_DIVIDENDS>	RegularDividendVec;
	typedef vector<RATE>				MsRateVec;
};



#include "MsRate.h"
#include "MsBasketIndex.h"
#include "MsContract.h"
#include "MsIndex.h"
#include "MsStock.h"
#include "MsUndExp.h"
#include "MsUnd.h"
#include "MsUndWeight.h"
#include "MsSynthRoot.h"
#include "MsOptRoot.h"
#include "MsOption.h"
#include "MsExpDate.h"
#include "MsFuturesRoot.h"
#include "MsFuture.h"
#include "MsFutureOption.h"



#include "EodMrkStructureMng.h"
////////////////////////////////////////////
// Price provider specific
#include "PpPriceRequest.h"
#include "PpRequestData.h"
#include "PpPriceProvider.h"
#include "PpStructureProvider.h"

////////////////////////////////////////////
// IVolatility specific
#include <EgHttp.h>
#include "IvDataLoader.h"
////////////////////////////////////////////

////////////////////////////////////////////
#include "EodStepProcessorBase.h"
#include "EodStepManager.h"
//#include "EodLoadIndexFutures.h"

#include "EodServer.h"

#pragma warning (pop)

#endif //__EODGENERAL_H__
