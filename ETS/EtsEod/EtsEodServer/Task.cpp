// Task.cpp : Implementation of CTask

#include "stdafx.h"
#include "Task.h"


// CTask


STDMETHODIMP CTask::get_StartTime(DATE* pVal)
{
	*pVal = m_dtStartTime;
	return S_OK;
}

STDMETHODIMP CTask::put_StartTime(DATE newVal)
{
	m_dtStartTime = newVal;
	return S_OK;
}
