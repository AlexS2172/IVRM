// PriceProviderPerfCounter.h: interface for the PriceProviderPerfCounter class.
//


#pragma once

[ perf_object(
		namestring="Price Providers",
		helpstring="A PerfMon Object for PriceProviderCounter",
		detail=PERF_DETAIL_NOVICE) ]
class PriceProviderPerfCounterObject
{
public:
	void Clear()
	{
		m_RealtimeClients  = 0;
		m_Clients          = 0;
		m_IncomingCounter  = 0;
		m_IncomingDataFlow = 0;
		m_ProcessedCounter = 0;
		m_lQuoteUpdatesQueueLength = 0;
		m_lUpdatesWorkload = 0;
		m_lUpdatesWorkloadBase = 0;
		m_lGroupRequest  = 0;
		m_lMultipleRequest = 0;
		m_lMultipleUnsubscription = 0;
		m_lClientUpdates = 0;
		Subscriptions = 0;

	}

public:

	[ perf_counter(namestring = "Clients",    helpstring = "", countertype = PERF_COUNTER_RAWCOUNT, defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG m_Clients;

	[ perf_counter(namestring = "RT Clients", helpstring = "", countertype = PERF_COUNTER_RAWCOUNT, defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG m_RealtimeClients;

	[ perf_counter(namestring = "Real-Time DataFlow", helpstring = "", countertype = PERF_SAMPLE_COUNTER , defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG m_IncomingDataFlow;

	[ perf_counter(namestring = "Incoming Updates", helpstring = "", countertype = PERF_SAMPLE_COUNTER , defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG m_IncomingCounter;

	[ perf_counter(namestring = "Processed Updates", helpstring = "", countertype = PERF_SAMPLE_COUNTER , defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG m_ProcessedCounter;

	[ perf_counter(namestring = "Updates Queue Snapshot", helpstring = "", countertype = PERF_COUNTER_RAWCOUNT, defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG m_lQuoteUpdatesQueueLength;

	// Start Group
	[ perf_counter(namestring = "Updates Workload", helpstring = "", countertype = PERF_SAMPLE_FRACTION , defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG m_lUpdatesWorkload;
	[ perf_counter(namestring = "Updates Workload", helpstring = "", countertype = PERF_SAMPLE_BASE , defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG m_lUpdatesWorkloadBase;
	// End Group

	[ perf_counter(namestring = "Group Request", helpstring = "", countertype = PERF_SAMPLE_COUNTER , defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG m_lGroupRequest;

	[ perf_counter(namestring = "Multiple Request", helpstring = "", countertype = PERF_SAMPLE_COUNTER , defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG m_lMultipleRequest;

	[ perf_counter(namestring = "Multiple Un-Subscription", helpstring = "", countertype = PERF_SAMPLE_COUNTER , defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG m_lMultipleUnsubscription;

	[ perf_counter(namestring = "Client Updates", helpstring = "", countertype = PERF_SAMPLE_COUNTER , defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG m_lClientUpdates;

	[ perf_counter(namestring = "Subscriptions",    helpstring = "", countertype = PERF_COUNTER_RAWCOUNT, defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG Subscriptions;

};


[ perfmon(name="PriceProvider", register=true) ]
class PriceProviderPerfCounter
{
public:



};
