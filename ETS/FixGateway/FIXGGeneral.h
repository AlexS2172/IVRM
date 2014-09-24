#ifndef __FIXGGENERAL_H__
#define __FIXGGENERAL_H__

#include <map>
#include <CString>
#include <vector>
#include <stdexcept>
using namespace std;

#pragma warning (push)
#pragma warning( disable : 4290 )

#include <ATLComTime.h>
using namespace ATL;

#include <comdef.h>
#include <initguid.h>
#include <stdio.h>
#include <conio.h>

////////////////////////////////////////////
#import <fixproviders.tlb> rename_namespace("FX") named_guids
#import <mmw.tlb> no_namespace named_guids
#import <msgstruct.tlb> no_namespace named_guids no_function_mapping
#import <ETSManager.tlb> named_guids rename_namespace("MGR")

//STL includes
#include <vector>
#include <map>
#include <queue>
using namespace std;

//EGAR includes
#include <EgLib/EgLibDbg.h>
#include <EgLib/EgLibSync.h>
#include <EgLib/EgLibThread.h>
#include <EgLib/EgLibMisc.h>
#include <EgLib/EgLibMath.h>
#include <EgLib/EgLibDb.h>
#include <EgLib/EgLibDynamicGITArray.h>

using namespace EgLib;
#include <ComErrorWrapper.h>

#include "ThreadPool.h"

////////////////////////////////////////////
// Settings manager specific
#include "SmSettingsMgr.h"


#include "FIXGateway.h"

#include "Storage.h"
#include "FixGServer.h"

#pragma warning (pop)

#endif //__FIXGGENERAL_H__
