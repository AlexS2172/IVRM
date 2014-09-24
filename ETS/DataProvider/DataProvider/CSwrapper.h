#pragma once

#include <atlbase.h>

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
