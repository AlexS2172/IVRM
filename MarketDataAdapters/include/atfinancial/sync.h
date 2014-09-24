/*
 *		sync.h		Date=990810
 *
 *		mutex, event, and semaphore classes
 *
 *		Written 04/06/96	R. Holm
 *
 *		Modified 09/06/96	R. Holm
 *			check for definition of __MUTEX_CLASS.
 *
 *		Modified 05/12/97	R. Holm
 *			added FastMutex class.
 *
 *		Modified 06/03/97	R. Holm
 *			added RWLock class.
 *			added Event::Pulse() method.
 *
 *		Modified 08/26/97	B. Chastain
 *			added Event::Set and Event::Reset to match underlying APIs.
 *
 *		Modified 03/13/99	R. Holm
 *			added linux support.
 *
 *		Modified 05/27/99	R. Holm
 *			added UNIX98 support.
 *
 *		Modified 08/03/99	R. Holm
 *			added SOLARIS support.
 *
 *		Modified 08/10/99	R. Holm
 *			bool changed to BOOL.
 *
 *
 *		Copyright (c) A-T Financial Information, Inc., 1995-1999.
 *		All rights reserved.
 *
 */

/*===========================================================================*/

#ifndef __SYNC_CLASS
#define __SYNC_CLASS

/*===========================================================================*/

#ifdef WIN32

class FastMutex
{
private:
	CRITICAL_SECTION  m_criticalSection;

public:
	FastMutex()
	{
		InitializeCriticalSection( &m_criticalSection );
	}

	~FastMutex()
	{
		DeleteCriticalSection( &m_criticalSection );
	}

	void Request( void )
	{
		EnterCriticalSection( &m_criticalSection );
	}

	void Release( void )
	{
		LeaveCriticalSection( &m_criticalSection );
	}

//	BOOL Test( void )
//	{
//		return TryEnterCriticalSection( &m_criticalSection );
//	}
};

#ifndef __MUTEX_CLASS
#define __MUTEX_CLASS

class Mutex
{
private:
	HANDLE  m_hMutex;

public:
	Mutex( const char *name = NULL, const BOOL bState = FALSE )
	{
		m_hMutex = CreateMutex( NULL, bState, name );
	}

	~Mutex()
	{
		CloseHandle( m_hMutex );
	}

	UINT Request( UINT timeOut = INFINITE )
	{
		return WaitForSingleObject( m_hMutex, timeOut );
	}

	UINT Release( void )
	{
		return ReleaseMutex( m_hMutex );
	}
};

#endif

class Event
{
private:
	HANDLE  m_hEvent;

public:
	Event( const char *name = NULL, const BOOL bState = FALSE, const BOOL bManual = FALSE )
	{
		m_hEvent = CreateEvent( NULL, bManual, bState, name );
	}

	~Event()
	{
		CloseHandle( m_hEvent );
	}

	UINT Wait( UINT timeOut = INFINITE )
	{
		return WaitForSingleObject( m_hEvent, timeOut );
	}

	UINT Post()
	{
		return SetEvent( m_hEvent );
	}

	UINT Set()
	{
		return SetEvent( m_hEvent );
	}
	
	UINT Clear()
	{
		return ResetEvent( m_hEvent );
	}

	UINT Reset()
	{
		return SetEvent( m_hEvent );
	}
	
	UINT Pulse()
	{
		return PulseEvent( m_hEvent );
	}
};

class Semaphore
{
private:
	HANDLE  m_hSemaphore;

public:
	Semaphore( const char *name = NULL, const UINT initialCount = 0, const UINT maxCount = 100 )
	{
		m_hSemaphore = CreateSemaphore( NULL, initialCount, maxCount, name );
	}

	~Semaphore()
	{
		CloseHandle( m_hSemaphore );
	}

	UINT Request( UINT timeOut = INFINITE )
	{
		return WaitForSingleObject( m_hSemaphore, timeOut );
	}

	UINT Release( UINT count = 1, UINT *pPrevCount = NULL )
	{
		return ReleaseSemaphore( m_hSemaphore, count, (LPLONG)pPrevCount );
	}
};

class RWLock
{
private:
	DWORD  m_processID;
	Event  *m_pEvent;
	Mutex  *m_pMutex;
	UINT   m_readers, m_writers, m_writersWaiting;

protected:
	void Lock( void )
	{
		m_pMutex->Request();
	}

	void Unlock( void )
	{
		m_pMutex->Release();
	}

public:
	RWLock( const char *name = NULL )
	{
		char  objectName[64];

		m_readers = 0;
		m_writers = 0;
		m_writersWaiting = 0;
		m_processID = 0;

		strcpy( objectName, name );
		strcat( objectName, "-mutex" );
		m_pMutex = new Mutex( objectName );
		
		strcpy( objectName, name );
		strcat( objectName, "-event" );
		m_pEvent = new Event( objectName, FALSE, TRUE );
	}

	~RWLock()
	{
		delete m_pMutex;
		delete m_pEvent;
	}

	BOOL ReadLock( void )
	{
		Lock();

		while ( m_writers > 0 || m_writersWaiting > 0 ) {
			Unlock();

			m_pEvent->Wait();

			Lock();
		}

		m_readers++;

		Unlock();
		return TRUE;
	}

	BOOL ReadUnlock( void )
	{
		BOOL  retValue = TRUE;

		Lock();

		if ( m_readers > 0 ) {
			m_readers--;

			if ( m_readers == 0 )
				m_pEvent->Pulse();
		} else
			retValue = FALSE;

		Unlock();
		return retValue;
	}

	BOOL WriteLock( void )
	{
		Lock();

		m_writersWaiting++;
		while ( m_writers > 0 || m_readers > 0 ) {
			Unlock();

			m_pEvent->Wait();

			Lock();
		}

		m_writersWaiting--;

		m_processID = GetCurrentThreadId();
		m_writers++;

		Unlock();
		return TRUE;
	}

	BOOL WriteUnlock( void )
	{
		BOOL  retValue = TRUE;

		Lock();

		if ( m_writers > 0 ) {
			if ( m_processID == GetCurrentThreadId()) {
				m_writers = 0;
				m_pEvent->Pulse();
			} else
				retValue = FALSE;
		} else
			retValue = FALSE;

		Unlock();
		return retValue;
	}
};

#endif

/*===========================================================================*/

#ifdef __LINUX__

#include <pthread.h>

#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#ifdef _SEM_SEMUN_UNDEFINED

union semun {
	int                 val;		// value for SETVAL
	struct semid_ds     *buf;		// buffer for IPC_STAT & IPC_SET
	unsigned short int  *array;		// array for GETALL & SETALL
	struct seminfo      *__buf;		// buffer for IPC_INFO
};

#endif

#define INFINITE	(unsigned)-1

#ifdef _POSIX_THREADS

class FastMutex
{
private:
	pthread_mutex_t  m_mutex;

public:
	FastMutex()
	{
		pthread_mutexattr_t  attr;

		pthread_mutexattr_init( &attr );
#ifdef __USE_UNIX98
		pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
#elif __SOLARIS
#else
		pthread_mutexattr_setkind_np( &attr, PTHREAD_MUTEX_RECURSIVE_NP );
#endif
		pthread_mutex_init( &m_mutex, &attr );
		pthread_mutexattr_destroy( &attr );
	}

	~FastMutex()
	{
		pthread_mutex_destroy( &m_mutex );
	}

	void Request( void )
	{
		pthread_mutex_lock( &m_mutex );
	}

	void Release( void )
	{
		pthread_mutex_unlock( &m_mutex );
	}

	BOOL Test( void )
	{
		return pthread_mutex_trylock( &m_mutex ) == EBUSY;
	}
};

#endif

#define SEMFLG_DELETE		0x00000001

class Sem
{
private:
	char  m_semName[128];
	UINT  m_flags;

protected:
	int    m_sem;
	key_t  m_key;

	unsigned SemCall( int semId, int op )
	{
		struct sembuf  semOp;

		semOp.sem_num = 0;
		semOp.sem_op = op;
		semOp.sem_flg = SEM_UNDO;
		if ( semop( semId, &semOp, 1 ) == -1 )
			return ERROR_INVALID_DATA;
		else
			return ERROR_SUCCESS;
	}

public:
	Sem( char *name = NULL, char type = ' ' )
	{
		m_flags = 0;

		if ( name != NULL )
			strcpy( m_semName, name );
		else
			tmpnam( m_semName );

		int  hFile;

		// create the file if needed...
		if (( hFile = open( m_semName, O_CREAT | O_RDWR, 0666 )) == -1 )
			return;
		close( hFile );

		m_key = ftok( m_semName, type );
		m_sem = semget( m_key, 1, IPC_CREAT | 0666 );
	}

	virtual ~Sem()
	{
	}

	void SetDelete( void )
	{
		m_flags |= SEMFLG_DELETE;
	}

	void Delete( void )
	{
		union semun  semData;

		semctl( m_sem, 0, IPC_RMID, semData );
		if (( m_flags & SEMFLG_DELETE ) != 0 )
			unlink( m_semName );
	}
};

#ifndef __MUTEX_CLASS
#define __MUTEX_CLASS

class Mutex : public Sem
{
private:

public:
	Mutex( char *name = NULL, BOOL bState = FALSE ) : Sem( name, 'm' )
	{
		union semun  semBuf;

		semBuf.val = 1;
		semctl( m_sem, 0, SETVAL, semBuf );
		if ( bState )
			Request();
	}

	~Mutex()
	{
	}

	unsigned Request( unsigned timeOut = INFINITE )
	{
		return SemCall( m_sem, -1 );
	}

	unsigned Release( void )
	{
		return SemCall( m_sem, 1 );
	}
};

#endif

class Event : public Sem
{
private:

public:
	Event( char *name = NULL, BOOL bState = FALSE, BOOL bManual = FALSE ) : Sem( name, 'e' )
	{
		union semun  semBuf;

		semBuf.val = 1000;
		semctl( m_sem, 0, SETVAL, semBuf );
	}

	~Event()
	{
	}

	unsigned Wait( unsigned timeOut = INFINITE )
	{
		return SemCall( m_sem, -1 );
	}

	unsigned Post( void )
	{
		union semun  semBuf;

		semBuf.val = 1000;
		semctl( m_sem, 0, SETVAL, semBuf );
		return ERROR_SUCCESS;
	}

	unsigned Clear( void )
	{
		union semun  semBuf;

		semBuf.val = 0;
		semctl( m_sem, 0, SETVAL, semBuf );
		return ERROR_SUCCESS;
	}
};

#ifdef _POSIX_THREADS

class LocalEvent
{
private:
	BOOL             m_state;
	pthread_cond_t   m_event;
	pthread_mutex_t  m_mutex;

public:
	LocalEvent( char *name = NULL, BOOL bState = FALSE, BOOL bManual = FALSE )
	{
		pthread_mutexattr_t  attr;

		memset( &m_mutex, 0, sizeof( m_mutex ));
		pthread_mutexattr_init( &attr );
#ifdef __USE_UNIX98
		pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
#elif __SOLARIS
#else
		pthread_mutexattr_setkind_np( &attr, PTHREAD_MUTEX_RECURSIVE_NP );
#endif
		pthread_mutex_init( &m_mutex, &attr );
		pthread_mutexattr_destroy( &attr );
		memset( &m_event, 0, sizeof( m_mutex ));
		pthread_cond_init( &m_event, NULL );
		m_state = FALSE;

		if ( bState )
			Post();
		else
			Clear();
	}

	~LocalEvent()
	{
		pthread_cond_destroy( &m_event );
		pthread_mutex_destroy( &m_mutex );
	}

	unsigned Wait( unsigned timeWait = INFINITE )
	{
		int              status;
		struct timespec  timeout;
		struct timeval   now;
		struct timezone  zone;

		pthread_mutex_lock( &m_mutex );

		if ( !m_state ) {
			if ( timeWait != INFINITE ) {
				// get the current time
				::gettimeofday( &now, &zone );
				
				// set the time we will wait until
				timeout.tv_sec = now.tv_sec+( timeWait/1000 );
				timeout.tv_nsec = ( now.tv_usec+(( timeWait%1000 )*1000 ))*1000;
			
				// wait...
				status = pthread_cond_timedwait( &m_event, &m_mutex, &timeout );
			} else
				status = pthread_cond_wait( &m_event, &m_mutex );
		} else
			status = ERROR_SUCCESS;

		pthread_mutex_unlock( &m_mutex );
		return status;
	}

	unsigned Post( void )
	{
		pthread_mutex_lock( &m_mutex );

		m_state = TRUE;

		pthread_cond_broadcast( &m_event );
		pthread_mutex_unlock( &m_mutex );
		return ERROR_SUCCESS;
	}

	unsigned Clear( void )
	{
		pthread_mutex_lock( &m_mutex );

		m_state = FALSE;

		pthread_mutex_unlock( &m_mutex );
		return ERROR_SUCCESS;
	}
};

#endif

class Semaphore : public Sem
{
private:

public:
	Semaphore( char *name = NULL, unsigned initialCount = 0, 
			   unsigned maxCount = 100 ) : Sem( name, 's' )
	{
		union semun  semBuf;

		semBuf.val = maxCount;
		semctl( m_sem, 0, SETVAL, semBuf );
	}

	~Semaphore()
	{
	}

	unsigned Request( unsigned timeOut = INFINITE )
	{
		return SemCall( m_sem, -1 );
	}

	unsigned Release( unsigned count = 1, unsigned *pPrevCount = NULL )
	{
		if ( pPrevCount != NULL ) {
			union semun  semBuf;

			*pPrevCount = semctl( m_sem, 0, GETVAL, semBuf );
		}

		return SemCall( m_sem, count );
	}
};

#endif

/*===========================================================================*/

#endif

/*===========================================================================*/

