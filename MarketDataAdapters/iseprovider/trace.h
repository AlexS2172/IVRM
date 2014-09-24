#ifndef TRACE_H
#define TRACE_H

void EgKillOldTrace();
void EgTrace(const TCHAR * lpszMessage, ...);
void EgTrace2(const TCHAR * lpszMessage);

#if defined(_EGDEBUG)
#define EgTraceD EgTrace
#define EgTraceD2 EgTrace2
#else
#define EgTraceD (void)0
#define EgTraceD2 (void)0
#endif

#endif