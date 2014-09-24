// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "windows.h"
#include "EgDebugUtil.h"


void test2()
{
	int* a = NULL;

	*a = 10;
}


void test()
{
	test2();
}

unsigned WINAPI Thread(LPVOID)
{
	test();
	return 0;
};

int main(int argc, char* argv[])
{
	SetMiniDumpDefaultCrashHandler();

	WaitForSingleObject((HANDLE)_beginthreadex(NULL, 0, Thread, NULL, 0, NULL), INFINITE);
	return 0;
}
