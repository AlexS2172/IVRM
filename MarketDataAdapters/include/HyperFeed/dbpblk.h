#ifndef _PCQ_INCL_DBPBLK									// only once
	#define _PCQ_INCL_DBPBLK

#if(_MSC_VER > 800)
	#include <winsock.h>
#endif
#include <textp.h>
#include <edbx.h>

#ifdef __cplusplus	// C++ compiler is being used
	#ifndef __unix //unix sdk requires c++
		extern "C" {
	#endif
#endif


#ifndef INCL_NOPACKING
	#pragma pack(2)						 /* Word aligned. */
#endif

#define DBAPBLKATTACHED			    0x3df5	 // Arbitrary Non-Zero Value.
#define DBAPBLKDETACHED			    0xd35f	 // Arbitrary Non-Zero Value.
#define LOG_BUFFER_SIZE				8192

#define DBAX_DBAX					0x80000000

typedef struct _TEXTLINE {
		BIT_FIELD	length:8;			/* length of entire txtline_s */
		BIT_FIELD	source:6;			/* same as TEXT_SPEC.source */
		BIT_FIELD	nonblank:1; 		/* true if other than 7, 10, or 13 */
		BIT_FIELD	last:1;				/* true if last record of item */
		UCHAR		type; 				/* line type preserved from funnel */
		} TEXTLINE;

typedef struct _DBAX_NORMAL {
	USHORT		dbNumber;			//0
	USHORT		dbStatus;			//2
	USHORT		handle;				//4
	USHORT		dbPosition[2]; 	//6
	USHORT		lastKey[12];		//10
	USHORT		recordLength;		//34
	} DBAX_NORMAL;

typedef struct _DBA_NEWS_LINE_HEADER {
	BIT_FIELD nonBlank		:1;	  // True if other than 7, 10, or 13
	BIT_FIELD last				:1;	  // True if last record of item
	BIT_FIELD alertItem		:1;
	BIT_FIELD splitLine		:1;
	BIT_FIELD continueLine	:1;
	BIT_FIELD reserved0		:11;
	USHORT	  length;					  // Length of entire textline
	USHORT	  source;					  // Same as TEXT_SPEC source
	UCHAR		  type;						  // Type of text line.
	UCHAR		  reserved1;
	} DBA_NEWS_LINE_HEADER;

#define DBA_NEWS_LINE_HEADER_SIZE ( sizeof( DBA_NEWS_LINE_HEADER ) )

typedef struct _DBA_NEWS_SPEC {
	BIT_FIELD alertItem	:1;			 // News Alert
	BIT_FIELD reserved0	:15;
	USHORT	 blankCount;				 // Count of leading blank lines this item
	USHORT	 source;						 // Identify source of item
	USHORT	 reserved1;
	ULONG		 start;						 // First byte of item
	} DBA_NEWS_SPEC;

#define DBA_NEWS_SPEC_SIZE ( sizeof( DBA_NEWS_SPEC ) )

typedef struct _DBA_NEWS_LINE {
	DBA_NEWS_LINE_HEADER  header;
	UCHAR	data[256];
	} DBA_NEWS_LINE;

typedef struct _DBA_NEWS_STRUC {
	DBA_NEWS_SPEC textSpec;
	DBA_NEWS_LINE_HEADER textLineHeader;
	} DBA_NEWS_STRUC;
#if(_MSC_VER > 800)

	typedef union _DBA_NEWS_TS {
		DBA_NEWS_STRUC newsStruc;
		OVERLAPPED 		overLapped;
	} DBA_NEWS_TS;
	
   typedef struct _DBA_TS_FORWARD {
		HANDLE hFile;
	   	OVERLAPPED overLapped;
		HANDLE hPoolFile;				// handle to free TS pool file
   	} DBA_TS_FORWARD;

	typedef struct _DBA_CORX_FORWARD  {
		HANDLE			hFile; 			// 32 bits
		OVERLAPPED		overLapped;
		USHORT			chkSum;
		HANDLE 			hAuxFile; 			// 32 bits
		ULONG			savCorxPtr;
		SHORT			savCorxEntry;
		DBA_CORXF_REC   savCorxRec;
		HANDLE			hAuxFileMap;  //  auxillary file mapping
		PULONG			pAuxMem;       //  pointer to auxillary memory
		ULONG			savTsPtr;
		INT				savTsEntry;
		DBA_CORXF_REC	savTsRec;
	} DBA_CORX_FORWARD;

    typedef struct _DBA_MEM_BACKUP {
   		CHAR	pblk[256];
   	} DBA_MEM_BACKUP;  

	typedef struct _DBA_PRICE_VOLUMES {
		LONG	price;
		LONG	volume;
	} DBA_PRICE_VOLUMES;
	
	typedef struct _DBA_PRICE_MONEYFLOWS {
		LONG		price;
		LONGLONG	moneyFlow;
	} DBA_PRICE_MONEYFLOWS;
	
	typedef struct _DBA_AGGREGATE {
		LONGLONG				weightedPriceTotal;
		ULONG					totalVolume;
		USHORT					volumesMax;
		USHORT					volumesCount;
		DBA_PRICE_VOLUMES		*pPriceVolumes;		
		USHORT					moneyFlowsMax;
		USHORT					moneyFlowsCount;
		DBA_PRICE_MONEYFLOWS	*pPriceMoneyFlows;
		USHORT					priceType;
		USHORT					recSequence;
		USHORT					numPblks;
		CHAR					pblks[1][256];	// Expandable.
	} DBA_AGGREGATE;

	typedef struct _DBA_FILE_CONTEXT {
		LONGLONG	fileSeekPosition;
		INT			direction;
		INT			nBufrPos;
		DWORD		nByteCount;
		INT			nBufrSize;
		ATTACH_DATA	attachData;
		CHAR		szFileName[128];
		CHAR		dataBufr[1];
	} DBA_FILE_CONTEXT;

	typedef struct _DBA_REGISTRY_CONTEXT {
		LONGLONG	fileSeekPosition;
		HKEY		hKey;
		INT			direction;
		INT			nBufrPos;
		DWORD		nByteCount;
		INT			nBufrSize;
		ATTACH_DATA	attachData;
		CHAR		szFileName[128];
		CHAR		dataBufr[1];
	} DBA_REGISTRY_CONTEXT;

	typedef union _DBA_SPECDB {
   		DBA_TS_FORWARD			*pTSForward;
   		CHAR					*pBtrvPblk;
		VOID					*hisConnectContext;		// Connection context for history db
		DBA_MEM_BACKUP			*pBackupPblk;
		DBA_AGGREGATE			*pAggregate;
		DBA_CORX_FORWARD		*pCorxForward;
		DBA_FILE_CONTEXT		*pFileContext;
		DBA_REGISTRY_CONTEXT	*pRegistryContext;
		VOID					*pClientCache;
	} DBA_SPECDB;
   
#endif

#define DBAPIPEPACK_VERSION	1		// Change if WDBPIPEPACK changes.

typedef struct  _DBAPIPEPACK {
	USHORT	version;						// For compatability test.
	USHORT	retry;
	SHORT	status;
	USHORT	function;
	USHORT	keynum;
	UCHAR	pblk[256];
	UCHAR	reserved[16];				// Filler for new stuff.
	USHORT	keyLen;
	CHAR	key[100];
	USHORT	buffLen;
	UCHAR	buff[1];						// Key and buffer.
	} DBAPIPEPACK;

#define SIZEOF_CLIENTPACKET			8192
#define SIZEOF_DBAPBLK				256
#define REMOTE_MASK					0x2000

#define DBA_PERM_READ				0x0001
#define DBA_PERM_WRITE				0x0002
#define DBA_PERM_DELETE				0x0004
#define DBA_PERM_ERASE				0x0008

typedef struct _DBA_PERM_FLAGS {
	BIT_FIELD pRead	:1;
	BIT_FIELD pWrite:1;
	BIT_FIELD pDelete:1;
	BIT_FIELD pErase:1;
	BIT_FIELD reserved:12;
	} DBA_PERM_FLAGS;
	
typedef union _DBA_PERMS {
	DBA_PERM_FLAGS	flags;
	USHORT			bits;
	} DBA_PERMS;

typedef struct  _DBACLIENTPACK {
	USHORT		  len;
	CHAR			  szPipeName[256];
	#ifndef __PCQWIN
		#if(_MSC_VER > 800)
			HANDLE		  pipeHandle;
		#else
			HFILE			  pipeHandle;
		#endif
	#else
		ULONG			  pipeHandle;
	#endif
	DBAPIPEPACK   dbaPack;
} DBACLIENTPACK;

#ifndef __DBA_PBLK_MASK_DEFINED
#define __DBA_PBLK_MASK_DEFINED
	#define DBA_PBLK_MASK_REMOTE			0x0001
	#define DBA_PBLK_MASK_ORGREMOTE			0x0002
	#define DBA_PBLK_MASK_STREAMCONTINUE	0x0004
	#define DBA_PBLK_MASK_STREAMNACKED		0x0008
	#define DBA_PBLK_MASK_RAWMODE			0x0010

	typedef struc _DBA_PBLK_BITS {
		BIT_FIELD				remote	 :1;
		BIT_FIELD				orgRemote :1;
		BIT_FIELD				streamContinue:1;
		BIT_FIELD				streamNacked:1;
		BIT_FIELD				fRawMode:1;
		BIT_FIELD				reservedB:11;
	} DBA_PBLK_BITS;

	typedef union _DBA_PBLK_FLAGS {
		DBA_PBLK_BITS   bits; // 2 bytes
		USHORT          mask;
	} DBA_PBLK_FLAGS;       // 2 bytes
#endif

typedef struct _DBAX_DBAI {
	USHORT					dbNum;
	#if !defined(__PCQWIN)
		#if(_MSC_VER > 800)
			DWORD						pid;
		#else
			#if defined (__IBMC__) || defined (__IBMCPP__) || defined (__BORLANDC__) 
				PID	pid;
			#else
				PID						pid;			// 16 bit
				USHORT					reservedP;
			#endif
		#endif
	#else
		ULONG						pid;
	#endif
	
	USHORT					handle;				// dbadll handle
	USHORT					reservedH;
	USHORT					attached;
	DBA_PBLK_FLAGS			flags;

	#if (_MSC_VER > 800) || defined(__unix)
		ULONG		(*dba)(void	*pblk,ULONG func,CHAR *key, ULONG *pKeyLen, CHAR *buffer, ULONG *pRecLen, ULONG keynum);
	#else
		SHORT						(*dbax)(struct _DBAX_DBAI
			*pblk,USHORT func,CHAR *key,CHAR *buffer,USHORT keynum);
	#endif
	#ifndef __PCQWIN
		#if(_MSC_VER > 800)
			HANDLE			fhDataFile;
		#else
			HFILE			fhDataFile;
		#endif
	#else
		ULONG				fhDataFile;
	#endif
	USHORT					lastKeyNum;
	SHORT					savLock;
	LONG					savPos;
	VOID					*pClass;				// Generic pointer for classes allocated on attach.
	CHAR					reservedSave[52];		// not used
	CHAR					nackCount;
	CHAR					ackCount;
	USHORT					maxKeyLen;
	CHAR					*pSaveKey;
	CHAR					*savPtr;
	USHORT					keylen;
	SHORT					lastFunc;
	USHORT					dbType;
	DBACLIENTPACK			*pClientPack;
	struct _DBATABLE		*ptblDb;
	BYTE					dbKeyLen[10];
	USHORT					dbLogRecLen;
	USHORT					extendedStatus;
	#ifndef __PCQWIN
		SHORT				(*dbio)(USHORT,PVOID,PVOID,ULONG, ULONG);
	#else
		SHORT				(FAR PASCAL *dbio)(USHORT,PVOID,PVOID,ULONG, ULONG);
	#endif
	CHAR					*pRecPhys;
	ULONG					serverStartTime;
	#if (_MSC_VER > 800)
		DBA_SPECDB			specDB;
	    DBA_NEWS_TS			dbaNewsTS;
		ULONG				*pPointerTable;
		DBHEADER			*pDbHeader;          
		HMTX				hmtxDb;
	#else
		PCHAR				pBtrvPblk;
		DBA_NEWS_LINE_HEADER textLineHeader;
		DBA_NEWS_SPEC		textSpec;
		USHORT				hashCompares;
		ULONG				ulSavInsDelHash;
		ULONG				ulSavInsDelBinary;
		USHORT				savStatus;
	#endif
	ULONG					savNSOffset;			// next sub offset saving
	ULONG					savPhysLen;				// phys record len this record
	DBA_PERMS				permissions;			// data base permissions
	#if !defined(__PCQWIN)
		HINSTANCE	  		hDbioMod;				// IO Module handle
	#else
		ULONG		  		hDbioMod;				// IO Module handle
	#endif
	#if (_MSC_VER > 800)
		HANDLE				hFile;
	#else
		ULONG				hFile;					// Handle to memory maped file
	#endif

	#if (_MSC_VER > 800)
		HANDLE				hStreamThread;				// for streaming thread
	#else
		ULONG				hStreamThread;
	#endif

		CHAR				szProcessName[32];
		ULONG				lastRequestDuration;	// In milliseconds
	} DBAX_DBAI;

typedef struct _DBAX_NEWS {
	USHORT		dbNumber;
	USHORT 		dbStatus;
	USHORT		handle;
	USHORT 		dbPosition;
	TEXT_SPEC	textSpec;
	TEXTLINE		textLine;
	} DBAX_NEWS;

typedef struct _DBAX_BTRIEVE {
	USHORT		dbNumber;
	USHORT 		dbStatus;
	USHORT 		handle;
	USHORT		lastKeyNo;
	UCHAR			lastKey[24];
	SHORT			lastRecType;
	CHAR			btrPblk[128];
	} DBAX_BTRIEVE;

typedef union _DBA_SERVER {
	struct {
		CHAR	serverName[8];
		#if defined (__IBMC__) || defined (__IBMCPP__) || defined (__BORLANDC__) || defined(__unix)
			} sn;
		#else
			};
		#endif
	struct {
		USHORT		txSocket;
		USHORT		rxSocket;
		#if defined (__IBMC__) || defined (__IBMCPP__) || defined (__BORLANDC__) || defined(__unix) 
			} ss;
		#else
			};
		#endif
	} DBA_SERVER;
		
typedef struct _REQUESTER_TCP {
	ATTACH_DATA 	attachData;
	#if (_MSC_VER > 800)
		SOCKET				sd;				// Socket.
////		CSocket				*pCSocket;
	#else
		INT					sd;
	#endif
	VOID    *pDbaSession;
    
	} REQUESTER_TCP;

typedef union _REQUESTER_DATA {
	REQUESTER_TCP	tcp;
	} REQUESTER_DATA;

typedef struct _DBAX_MAXPBLK {
	UCHAR 			reserved[240];
	REQUESTER_DATA *pReqData;			// Used and maintained by requesters.
	DBA_SERVER		server;
	USHORT			checkSum;
	USHORT			dbaHandle;
	} DBAX_MAXPBLK;

typedef union _DBAPBLK {
	DBAX_NORMAL		normal;
	DBAX_NEWS		news;
	DBAX_BTRIEVE	btrieve;
	DBAX_MAXPBLK	max;
	DBAX_DBAI		dbai;
	} DBAPBLK;

#ifndef INCL_NOPACKING
	#pragma pack() 						 /* Word aligned. */
#endif

#ifdef __cplusplus			// C++ compiler is being used
	#ifndef __unix //unix sdk requires c++
		}
	#endif
#endif
#endif
