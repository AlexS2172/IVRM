// edaTools.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "wtypes.h"
#include "edaTools.h"

#define POLYNOMIAL 0x04C11DB7L
static unsigned long g_ulCRCTable[256];
void _GenerateCRCTable();

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			_GenerateCRCTable();

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

void _GenerateCRCTable()
{
	register int i, j;
	register unsigned long ulCrcAccum;
	for(i = 0;  i < 256; i++)
	{
		ulCrcAccum = ((unsigned long) i << 24);
		for(j = 0; j < 8; j++)
		{
			if(ulCrcAccum & 0x80000000L)
				ulCrcAccum = (ulCrcAccum << 1) ^ POLYNOMIAL;
			else
				ulCrcAccum = (ulCrcAccum << 1 );
		}
		g_ulCRCTable[i] = ulCrcAccum;
	}
}

unsigned long EDATOOLS_API CalcCRC32(const char* lpszData, int nDataLen)
{
	unsigned long ulCrcAccum = 0L;
	register int i, j;
	for(j = 0;  j < nDataLen; j++)
	{
		i = ((int)( ulCrcAccum >> 24) ^ *lpszData++) & 0xff;
		ulCrcAccum = (ulCrcAccum << 8) ^ g_ulCRCTable[i];
	}
	return ulCrcAccum;
}

