#if !defined(AFX_HYPERFEEPERFORMANCE_H__AFA29E2D_4861_4458_B760_3D493F1EFCA5__INCLUDED_)
#define AFX_HYPERFEEPERFORMANCE_H__AFA29E2D_4861_4458_B760_3D493F1EFCA5__INCLUDED_
#include "Process.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma once
#include <atlperf.h>
[ perf_object(
			  namestring="HyperFeed Price Provider",
			  helpstring="A PerfMon Object for HF PriceProviderCounter",
			  detail=PERF_DETAIL_NOVICE) ]
class PriceProviderPerfCounterObject
{
public:
	[ perf_counter(namestring = "SubscribedClients", helpstring = "", countertype = PERF_COUNTER_RAWCOUNT, defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG m_RealtimeClients;
};
// TODO:  First add Performance Monitor objects to your project.
// You can then add counters to those objects.  Use the context
// menu of PriceProviderPerfCounter in ClassView to add a PerfMon Object
// to your project.  Then use the context menu of the newly
// created PerfMon Object to add a PerfMon Counter.
[ perfmon(name="PriceProvider", register=true) ]
class PriceProviderPerfCounter
{
public:
};

#endif //AFX_HYPERFEEPERFORMANCE_H__AFA29E2D_4861_4458_B760_3D493F1EFCA5__INCLUDED_