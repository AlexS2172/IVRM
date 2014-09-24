// DataProtector.h: interface for the CDataProtector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAPROTECTOR_H__400AA815_1CA9_406F_B057_4C3E4E96DAC5__INCLUDED_)
#define AFX_DATAPROTECTOR_H__400AA815_1CA9_406F_B057_4C3E4E96DAC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObjectLock;

class CDataProtector  
{
	mutable CRITICAL_SECTION m_cs;
	mutable bool			 m_bEnterCriticalSection;

public:
	CDataProtector( bool bEnterCriticalSection = false ): m_bEnterCriticalSection( bEnterCriticalSection )
		{
#if (_WIN32_WINNT >= 0x0403)
		::InitializeCriticalSectionAndSpinCount( &m_cs, 4000 );
#else
		::InitializeCriticalSection( &m_cs );
#endif

		if( m_bEnterCriticalSection )
			::EnterCriticalSection( &m_cs );
		}

	virtual ~CDataProtector()
		{
		if( m_bEnterCriticalSection )
			::LeaveCriticalSection( &m_cs );

		::DeleteCriticalSection( &m_cs );
		}

	friend CObjectLock;
};


class CObjectLock
{
	const CDataProtector* m_protector;

public:
	CObjectLock( const CDataProtector* protector ): m_protector( protector )
		{
		::EnterCriticalSection( &m_protector->m_cs );
		}

	virtual ~CObjectLock()
		{
		::LeaveCriticalSection( &m_protector->m_cs );
		}
};

#endif // !defined(AFX_DATAPROTECTOR_H__400AA815_1CA9_406F_B057_4C3E4E96DAC5__INCLUDED_)
