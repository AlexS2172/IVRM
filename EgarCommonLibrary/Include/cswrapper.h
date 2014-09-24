#ifndef __CSWRAPPER_H__
#define __CSWRAPPER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <atlbase.h>

// critical section wrapper class
class CCriticalSectionWrapper
{
public:
	// constructor
	CCriticalSectionWrapper(CRITICAL_SECTION& cs)
	{
		m_pCS = &cs;
		::EnterCriticalSection(m_pCS);
	}

	// constructor
	CCriticalSectionWrapper(CComAutoCriticalSection &cs)
	{
		m_pCS = &cs.m_sec;
		::EnterCriticalSection(m_pCS);
	}

	// destructor
	~CCriticalSectionWrapper()
	{
		::LeaveCriticalSection(m_pCS);
	}

private:
	// wrapped critical section pointer
	CRITICAL_SECTION*		m_pCS;	
};

#endif	//__CSWRAPPER_H__
