#ifndef TRACE_H
#define TRACE_H

#include "clearingbase.h"

void Trace(const TCHAR * lpszMessage, ...);
DWORD TraceError(const DWORD dwErr, const TCHAR* const szDescription);
void InitMainLog();

#endif