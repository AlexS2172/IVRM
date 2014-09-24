/*
 *		shrmem.h		Date=990804
 *
 *		portable (eventually, when the OS/2 and UNIX? versions are written) 
 *		shared memory object.
 *
 *		Written 04/27/96	R. Holm
 *
 *		Modified 05/12/96	R. Holm
 *			added unique parameter to the constructor.
 *
 *		Modified 09/11/96	R. Holm
 *			SharedMemory::m_pMutex was not deleted in the destructor.
 *
 *		Modified 10/08/96	R. Holm
 *			added SharedMemory::m_bExisting and increased SharedMemory::m_name to 128.
 *
 *		Modified 10/10/96	R. Holm
 *			m_bExisting was not being checked before clear the mapped block.
 *
 *		Modified 03/14/99	R. Holm
 *			added linux support.
 *
 *		Modified 08/04/99	R. Holm
 *			added solaris support.
 *
 *
 *		Copyright (c) A-T Financial Information, Inc., 1995-1999.
 *		All rights reserved.
 *
 */

/*=========================================================================================*/

#ifndef __SHAREDMEMORY_CLASS
#define __SHAREDMEMORY_CLASS

/*=========================================================================================*/

#include <sync.h>

/*=========================================================================================*/

#ifdef WIN32

class SharedMemory
{
private:
	BOOL    m_bExisting;
	char    m_name[128];
	HANDLE  m_hMem;
	LPVOID  m_pMem;
	Mutex   *m_pMutex;

protected:

public:
	SharedMemory( const char *memName, UINT access, UINT size, BOOL unique = FALSE )
	{
		char  semName[64];
		UINT  memAccess;

		strcpy( m_name, memName );
		strcpy( semName, memName );
		strcat( semName, "-Sem" );
		m_pMutex = new Mutex( semName, TRUE );

		if (( m_hMem = CreateFileMapping((HANDLE)0xffffffff, NULL, access | SEC_COMMIT, 
										 0, size, memName )) == INVALID_HANDLE_VALUE ) {
			m_pMem = NULL;
			Unlock();
			return;
		}

		if ( GetLastError() == ERROR_ALREADY_EXISTS && unique ) {
			m_pMem = NULL;
			Unlock();
			return;
		} else if ( GetLastError() == ERROR_ALREADY_EXISTS )
			m_bExisting = TRUE;
		else
			m_bExisting = FALSE;

		if ( access == PAGE_READWRITE )
			memAccess = FILE_MAP_WRITE;
		else
			memAccess = FILE_MAP_READ;

		if (( m_pMem = (LPVOID)MapViewOfFile( m_hMem, memAccess, 0, 0, 0 )) != NULL ) {
			if ( !m_bExisting )
				memset( m_pMem, 0, size );
		}

		Unlock();
	}

	SharedMemory( const char *memName, UINT access )
	{
		char  semName[64];
		UINT  memAccess;

		strcpy( m_name, memName );
		strcpy( semName, memName );
		strcat( semName, "-Sem" );
		m_pMutex = new Mutex( semName );

		if ( access == PAGE_READWRITE )
			memAccess = FILE_MAP_WRITE;
		else
			memAccess = FILE_MAP_READ;

		m_bExisting = TRUE;

		if (( m_hMem = OpenFileMapping( memAccess, FALSE, memName )) == NULL ) {
			m_pMem = NULL;
			return;
		}

		m_pMem = (LPVOID)MapViewOfFile( m_hMem, memAccess, 0, 0, 0 );
	}

	~SharedMemory()
	{
		delete m_pMutex;
		if ( m_pMem != NULL )
			UnmapViewOfFile( m_pMem );
		CloseHandle( m_hMem );
	}

	LPVOID GetPointer( void )
	{
		LPVOID  pMem;

		Lock();

		pMem = m_pMem;

		Unlock();
		return pMem;
	}

	void Lock( void )
	{
		m_pMutex->Request();
	}

	void Unlock( void )
	{
		m_pMutex->Release();
	}

	BOOL OpenExisting( void )
	{
		return m_bExisting;
	}
};

#endif

/*=========================================================================================*/

#ifdef __LINUX__

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#pragma pack(1)

#define SHMEM_SIGNATURE			0x306d6873		// "shm0"

#define SHMEMFLG_DELETE			0x00000001

typedef struct _SHMEM_HEADER {
	ULONG  sig;
	ULONG  length;
	BYTE   resv[8];
} SHMEM_HEADER, *LPSHMEM_HEADER, *PSHMEM_HEADER;

#pragma pack()

class SharedMemory
{
private:
	BOOL            m_bExisting;
	char            m_name[128];
	int             m_hMem;
	LPSHMEM_HEADER  m_pHeader;
	Mutex           *m_pMutex;
	UINT            m_flags, m_resv[2];
	void            *m_pMem;

protected:

public:
	SharedMemory( const char *memName, UINT access, UINT size, BOOL bUnique = FALSE )
	{
		char  semName[128];
		int   hFile;
		UINT  flags = 0666 | IPC_CREAT;

		strcpy( m_name, memName );
		m_pMutex = NULL;
		m_bExisting = FALSE;
		m_flags = 0;
		m_resv[0] = 0;
		m_resv[1] = 0;

		// create the file if needed...
		if (( hFile = open( memName, O_CREAT | O_RDWR, 0666 )) == -1 ) {
			m_pMem = NULL;
			return;
		}
		close( hFile );

		// get the id...
		if (( m_hMem = shmget( ftok( memName, 'M' ), size+sizeof( SHMEM_HEADER ), flags )) < 0 ) {
			m_pMem = NULL;
			return;
		}

		// and attach...
		m_pMem = shmat( m_hMem, NULL, 0 );
		if ( m_pMem == NULL || m_pMem == (void*)-1 ) {
			m_pMem = NULL;
			return;
		}

		// point to the shared memory header
		m_pHeader = (LPSHMEM_HEADER)m_pMem;

		// did the caller want to create this?
		if ( bUnique && m_pHeader->sig == SHMEM_SIGNATURE ) {
			m_pMem = NULL;
			return;
		}

		// it's ok to get the semaphore now!
		strcpy( semName, memName );
		m_pMutex = new Mutex( semName, TRUE );

		if ( m_pHeader->sig == SHMEM_SIGNATURE ) {
			m_bExisting = TRUE;
		} else {
			m_bExisting = FALSE;

			// initialize the memory header
			memset( m_pHeader, 0, sizeof( *m_pHeader ));
			m_pHeader->sig = SHMEM_SIGNATURE;
			m_pHeader->length = size+sizeof( SHMEM_HEADER );
		}

		Unlock();
	}

	SharedMemory( const char *memName, UINT access )
	{
		char  semName[128];
		int   hFile;
		UINT  flags = 0666, length;

		m_bExisting = FALSE;
		strcpy( m_name, memName );
		m_pMutex = NULL;

		// create the file if needed...
		if (( hFile = open( memName, O_CREAT | O_RDWR, 0666 )) == -1 ) {
			m_pMem = NULL;
			return;
		}
		close( hFile );

		// attach for the first time
		if (( m_hMem = shmget( ftok( memName, 'M' ), sizeof( SHMEM_HEADER ), flags )) < 0 ) {
			m_pMem = NULL;
			return;
		}

		m_pMem = shmat( m_hMem, NULL, 0 );
		if ( m_pMem == NULL || m_pMem == (void*)-1 ) {
			m_pMem = NULL;
			return;
		}

		// point to the shared memory header
		m_pHeader = (LPSHMEM_HEADER)m_pMem;

		if ( m_pHeader->sig != SHMEM_SIGNATURE ) {
			m_pMem = NULL;
			return;
		} else {
			// the segment already existed!
			m_bExisting = TRUE;
			length = m_pHeader->length;

			// detach from the first instance
			shmdt((char*)m_pMem );

			// go get it again!
			if (( m_hMem = shmget( ftok( memName, 'M' ), length, flags )) < 0 ) {
				m_pMem = NULL;
				return;
			}

			// map it!
			m_pMem = shmat( m_hMem, NULL, 0 );
			if ( m_pMem == NULL || m_pMem == (void*)-1 ) {
				m_pMem = NULL;
				return;
			}

			// point to the shared memory header
			m_pHeader = (LPSHMEM_HEADER)m_pMem;

			// ok to create the semaphore now!
			strcpy( semName, memName );
			m_pMutex = new Mutex( semName, TRUE );
		}

		Unlock();
	}

	~SharedMemory()
	{
		if ( m_hMem != -1 ) {
			struct shmid_ds  memInfo;

			if ( m_pMem != NULL )
				shmdt((char*)m_pMem );

			shmctl( m_hMem, IPC_STAT, &memInfo );

			// anyone still looking at this memory?
			if ( memInfo.shm_nattch == 0 ) {
				shmctl( m_hMem, IPC_RMID, NULL );
				m_pMutex->Delete();
				if (( m_flags & SHMEMFLG_DELETE ) != 0 )
					unlink( m_name );
			}

			delete m_pMutex;
		}
	}

	void SetDelete( void )
	{
		m_flags |= SHMEMFLG_DELETE;
		m_pMutex->SetDelete();
	}

	void *GetPointer( void )
	{
		void *pMem;

		Lock();

		if ( m_pMem != NULL )
			pMem = ((char*)m_pMem)+sizeof( SHMEM_HEADER );
		else
			pMem = NULL;

		Unlock();
		return pMem;
	}

	void Lock( void )
	{
		if ( m_pMutex != NULL )
			m_pMutex->Request();
	}

	void Unlock( void )
	{
		if ( m_pMutex != NULL )
			m_pMutex->Release();
	}

	BOOL OpenExisting( void )
	{
		return m_bExisting;
	}

	UINT GetAttachCount( void )
	{
		UINT  count;

		Lock();

		if ( m_hMem == -1 )
			count = 0;
		else {
			struct shmid_ds  memInfo;

			shmctl( m_hMem, IPC_STAT, &memInfo );
			count = memInfo.shm_nattch;
		}

		Unlock();
		return count;
	}
};

#endif

/*=========================================================================================*/

#endif

/*=========================================================================================*/

