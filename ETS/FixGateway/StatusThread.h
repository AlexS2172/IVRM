#pragma once
#include "FIXGGeneral.h"

class CStatusThread : public FXGCO::CThread
{
public:
	CStatusThread(void) {};

	UINT  Thread(void* pParam);
};
