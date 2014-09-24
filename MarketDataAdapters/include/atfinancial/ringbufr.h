/*
 *		ringbufr.h			Date=990823
 *
 *		Ring Buffer functions.
 *
 *		Written 11/04/96	R. Holm
 *
 *		Modified 11/05/96	R. Holm
 *			added RingBuffer::GetSize(), RingBuffer::GetUsage(), and 
 *				RingBuffer::RingBuffer( char *, UINT ).
 *
 *		Modified 04/02/98	R. Holm
 *			added get/set timeout.
 *
 *		Modified 03/14/99	R. Holm
 *			added linux support.
 *
 *		Modified 08/23/99	R. Holm
 *			FastMutexes for UNIX.
 *
 *
 *		Copyright (c) A-T Financial Information, Inc., 1994-1999.
 *		All rignts reserved.
 *
 */

/*=====================================================================*/

#ifndef __RINGBUFFER_CLASS
#define __RINGBUFFER_CLASS

/*=====================================================================*/

#ifdef __OS2__
#define ERROR_SUCCESS		NO_ERROR

#ifdef _DLL
#define DLLENTRY			_Export
#else
#define DLLENTRY			_Import
#endif
#endif

#ifdef WIN32
#ifdef _DLL
#define DLLENTRY			__declspec( dllexport )
#else
#define DLLENTRY			__declspec( dllimport )
#endif
#endif

#ifdef __LINUX__
#define DLLENTRY
#endif

/*=====================================================================*/

#include <sync.h>
#include <shrmem.h>

/*=====================================================================*/

typedef struct _RING_ERRORS {
	ULONG  overrun;
	ULONG  parity;
	ULONG  framing;
	ULONG  cbreak;
	ULONG  escape;
	ULONG  bcc;
	ULONG  wrap;
	ULONG  other;
} RING_ERRORS, *LPRING_ERRORS;

typedef struct _RING_BUFFER {
	PUCHAR  pBegin;
	PUCHAR  pEnd;
	PUCHAR  pHead;
	PUCHAR  pTail;
	ULONG   count[2];
	ULONG   timeout;
} RING_BUFFER, *LPRING_BUFFER;

/*=====================================================================*/

class RingBuffer
{
private:
	char           m_name[64];
#ifdef WIN32
	Event          m_hEvent;
	Mutex          *m_pMutex;
#endif
#ifdef __LINUX__
	LocalEvent     m_hEvent;
	FastMutex      *m_pMutex;
#endif
	LPRING_BUFFER  m_pHeader;
	LPRING_ERRORS  m_pErrors;
	PUCHAR         m_pBuffer;
	SharedMemory   *m_pMem;
	UINT           m_error, m_bufferSize;

protected:
	UINT Lock( UINT timeOut = INFINITE )
	{
#ifndef __LINUX__
		return m_pMutex->Request( timeOut );
#else
		m_pMutex->Request();
		return ERROR_SUCCESS;
#endif
	}

	void Unlock()
	{
		m_pMutex->Release();
	}

public:
	DLLENTRY RingBuffer( const UINT bufferSize );
	DLLENTRY RingBuffer( const char *name, const UINT bufferSize = 0 );
	DLLENTRY ~RingBuffer();

	UINT GetError( void )
	{
		return m_error;
	}

	UINT DLLENTRY Write( void *pData, UINT length );
	UINT DLLENTRY Read( void *pData, UINT size, UINT *length );

	UINT Peek( void )
	{
		UINT  num;

		Lock();

		num = m_pHeader->count[0]-m_pHeader->count[1];

		Unlock();
		return num;
	}

	UINT GetSize( void )
	{
		return m_bufferSize;
	}

	UINT GetUsage( void )
	{
		UINT  freeSpace, usage;

		Lock();

		if ( m_pHeader->count[0] == m_pHeader->count[1] )
			freeSpace = m_bufferSize;
		else if ( m_pHeader->pHead > m_pHeader->pTail )
			freeSpace = ( m_pHeader->pEnd-m_pHeader->pHead )+( m_pHeader->pTail-m_pHeader->pBegin );
		else if ( m_pHeader->pHead < m_pHeader->pTail )
			freeSpace = ( m_pHeader->pTail-m_pHeader->pHead );
		else
			freeSpace = 0;

		usage = m_bufferSize-freeSpace;

		Unlock();
		return usage;
	}

	UINT DLLENTRY Flush( void );

	void SetTimeout( ULONG timeout )
	{
		if ( timeout == 0 )
			timeout = 1;
		m_pHeader->timeout = timeout;
	}

	ULONG GetTimeout( void )
	{
		return m_pHeader->timeout;
	}
};

/*=====================================================================*/

#endif

/*=====================================================================*/
