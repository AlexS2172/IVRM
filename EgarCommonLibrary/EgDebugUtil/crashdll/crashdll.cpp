#include "stdafx.h"

extern "C" void __stdcall DoCrash ( void )
{
	int* a = NULL;

	*a = 10;
}
