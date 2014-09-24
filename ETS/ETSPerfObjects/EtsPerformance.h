// EtsPerformance.h: interface for the CEtsPerformance class.
//


#pragma once

[ perf_object(namestring = "ETS Performance", helpstring = "ETS", detail = PERF_DETAIL_NOVICE) ]
class ETSPerfObject
{
public:
	ETSPerfObject()
		:CalcOptionGreeks(0)
		,CalcFutureOptionGreeks(0)
		,CalcGreeks(0)
		,CalcVolatility(0)
		,CalcOptionVolatility(0)
		,CalcFutureOptionVolatility(0)
		,Calculations(0)

	{}
	[ perf_counter(namestring = "Calculations", helpstring = "", countertype = PERF_SAMPLE_COUNTER, defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG Calculations;

	[ perf_counter(namestring = "CalcGreeks", helpstring = "", countertype = PERF_SAMPLE_COUNTER, defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG CalcGreeks;

	[ perf_counter(namestring = "CalcVolatility", helpstring = "", countertype = PERF_SAMPLE_COUNTER, defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG CalcVolatility;

	[ perf_counter(namestring = "CalcOptionGreeks", helpstring = "", countertype = PERF_SAMPLE_COUNTER, defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG CalcOptionGreeks;

	[ perf_counter(namestring = "CalcFutureOptionGreeks", helpstring = "", countertype = PERF_SAMPLE_COUNTER, defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG CalcFutureOptionGreeks;

	[ perf_counter(namestring = "CalcOptionVolatility", helpstring = "", countertype = PERF_SAMPLE_COUNTER, defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG CalcOptionVolatility;

	[ perf_counter(namestring = "CalcFutureOptionVolatility", helpstring = "", countertype = PERF_SAMPLE_COUNTER, defscale = 0, detail = PERF_DETAIL_NOVICE) ]
	ULONG CalcFutureOptionVolatility;
};

[ perfmon(name="Perf_EtsPerformance", register=true) ]
class CEtsPerformance
{
public:



};
