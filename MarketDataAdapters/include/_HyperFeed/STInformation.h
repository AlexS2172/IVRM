#if !defined(STINFORMATION_INCL)
#define STINFORMATION_INCL

#ifndef INCL_NOPACKING
	#pragma pack(1)						 
#endif
#include "pqtstats.h"
// The pqtstats file and structures defined in that header are kept around in 
// order to be compatable with legacy systems.  Newer programs can just write and 
// read from dba directly or use the CSTInformation class.
typedef struct _DBA_INFORMATION_KEY 
{
	CHAR	programName[32];
	CHAR	subName[32];
	CHAR	szServer[32];
} DBA_INFORMATION_KEY;


/******************************************************************************/
// These are the structures which defines the text portion of the FILE_TRANSFER
// control (FT_CONTROL in etix.h) message.  These messages are created to send commands to programs
// for example, it might be sent to the Line Mgr program in order to up and down a line.
typedef struct _ST_XMIT_MGR_COMMAND 
{
	int		number;
	char	symbol[20];
}ST_XMIT_MGR_COMMAND;

typedef union	_ST_STATS_COMMAND 
{
	ST_XMIT_MGR_COMMAND	xmit;
	char	text[80];
} ST_STATS_COMMAND;

typedef struct _ST_STATS_CONTROL 
{
	DBA_INFORMATION_KEY key;   // 64 bytes
	int	command;  // 4 bytes, this is the command as defined in pqtstats.h
	char name[20];
	ST_STATS_COMMAND data;
} ST_STATS_CONTROL;
// Note the text portion of FT_CONTROL is 168 bytes
/******************************************************************************/


typedef struct _STDCP_LINE_STAT 
{
	int		dcpNum;
	ULONG	errOverrun;
	ULONG	errParity;
	ULONG	errFraming;
	ULONG	errBreak;
	ULONG	errEscape;
	ULONG	errBcc;
	ULONG	errWrap;
	ULONG	errOther;
	ULONG	resv;
	ULONG	numPackets;
	ULONG	gaps;
} STDCP_LINE_STAT;


typedef struct _STDCPSTATS 
{
	int				dcpCount;
	int				lineCount;
	STDCP_LINE_STAT	line;
} STDCPSTATS;


typedef struct _STLINE 
{
	ULONG   status;
	ULONG   sequence;
	ULONG   gaps;
	ULONG   bytesReceived;
	ULONG   received;
	ULONG   OK;
	ULONG   sent;
	ULONG   rejected;
	ULONG   bad;
	BOOL	alarm;
	ULONG   ignored;
	ULONG	outOfSequence;
	int		state;
	ULONG	baudRate;
	ULONG	debug;
	ULONG	queueCount;
	UCHAR	reserved[4];
} STLINE;

typedef struct _STLINESTATS 
{
	char	version[10];
	STLINE  line[3];
} STLINESTATS;

typedef struct _STLMSTATS 
{
	char		lmName[60];
	int			lineCount;
	STLINESTATS	line;
} STLMSTATS;



#define MESSAGE_TYPE_MAX 40
typedef struct _STPACKETRATE 
{
	ULONG		pps;
	ULONG		bps;
} STPACKETRATE;

typedef struct _STXMITSTAT 
{
	STPACKETRATE	tx;
	STPACKETRATE	extLoop;
	STPACKETRATE	mmLoop;
	STPACKETRATE	ttLoop;
	STPACKETRATE	intlRate;
	STPACKETRATE	intlLoop;
	STPACKETRATE	spLoop;
	STPACKETRATE	bookLoop;
	ULONG		outCnt[MESSAGE_TYPE_MAX + 2];
	char		streamSym[20];
	char		extLoopSym[20];
	char		ttLoopSym[20];
	char		mmLoopSym[20];
	char		spLoopSym[20];
	char		bookLoopSym[20];
	ULONG		mainQueue;
	ULONG		waitQueue;
	ULONG		intlQueue;
	ULONG		pipeQueue;
	BOOL		alarm;
	BOOL		loopsEnabled;
	BOOL		transmitEnabled;
	UCHAR		reserved[64];
} STXMITSTAT;

typedef struct _STXMITSTATS 
{
	int			xmitCount;
	STXMITSTAT	xmit;  
} STXMITSTATS;


typedef struct _STALARMSTATS 
{
	BOOL	lmAlarm;
	BOOL	dbsAlarm;
	BOOL	xmitAlarm;
	BOOL	entitleAlarm;
	char	reserved[9];
} STALARMSTATS;


typedef struct _STBUFFERSTATS 
{
	UCHAR			bufferName[128];
	ULONG			pBegin; 				// Beginning of ring buffer
	ULONG			pEnd;					// End of ring buffer
	ULONG			pGet;					// Current get pointer
	ULONG			pPut;					// Current put pointer
	ULONG 			size; 					// Size of ring buffer
	ULONG 			used; 					// Bytes in ring buffer
	ULONG 			count;					// Packets in ring buffer
	ULONG 			queueUsed;				// Bytes in packet queue
	ULONG 			queueCount; 			// Packets in packet queue
	ULONG			wraps;					// Buffer Wraps
	ULONG			errors;
	ULONG			rejected;
	ULONG			processedRejected;
	ULONG			packetsSent;
	ULONG			currentSequence;
}STBUFFERSTATS;

typedef struct _ST_ETIX_CLIENT_STATS 
{
	DWORD		etiPID;										// Process PID
	ULONG		etiRunCntr; 								// Counter for average stat
	ULONG		etiStartMsec;								// Release start Msec
	ULONG		etiCurMsec; 								// Subtotal between ticks
	LONG 		etiLastMsec;								// Total for last interval
	ULONG		etiTotalMsec;								// Running total Msecs
	LONG		etiMinMsec; 								// Minimum Msec so far
	LONG		etiMaxMsec; 								// Maximum Msec so far
	ULONG		etiTotalPackets;							// Total packets processed
	ULONG		etiAttachType;								// Sync or Async Attach
} ST_ETIX_CLIENT_STATS;

#define STINFO_ETI_ATTACH_SYNC	0
#define STINFO_ETI_ATTACH_ASYNC	1

typedef struct _ST_ETIXSTATS 
{
	ULONG	pps;
	ULONG	packets;
	ULONG	syncClients;
	ULONG	asyncClients;
} ST_ETIXSTATS;

typedef struct _STDML 
{
	UCHAR	ip[40];
	ULONG	port;
	ULONG	ttl;
} STDML;

typedef struct _STDBASTATS 
{
	LONG	dsStatsInterval;  // wait interval for stats send ( milliseconds)
	ULONG	dsAlarmLimit;     // current alarm limit in use 
	ULONG	dsRxSequence;     //  recieve sequence number
	ULONG	dsTxSequence;     //  transmit sequence number
	CHAR	dsStreamItem[20];  //  item currently processed
	BOOL	isRunning;
} STDBASTATS;

typedef struct _STDBA_MESSAGESTATS 
{
	ULONG	ulPackets;
	ULONG	packetsIntl;
	ULONG 	ulMsecsTotal;
	ULONG 	ulMsecsMax;
	ULONG 	ulNonZeroMsecs;
} STDBA_MESSAGESTATS;

typedef struct _STTICKERSTATS 
{
	SHORT	systemYear;				/* current system year							*/
	SHORT	systemMonth;			/* current system month							*/
	SHORT	systemDay;				/* current system day							*/
	SHORT	systemHour;				/* current system hour							*/
	SHORT	systemMinute;		 	/* current system minute						*/
	SHORT	systemSecond;			/* current system second 						*/
	SHORT	systemRemainder;		/* current system seconds remainder				*/
	SHORT	startYear;				/* year system was started	 					*/
	SHORT	startMonth;				/* month system was started 					*/
	SHORT	startDay;				/* day system was started 						*/
	SHORT	startHour;				/* hour system was started	 					*/
	SHORT	startMinute;			/* minute system was started					*/
	SHORT	startSecond;			/* second system was started					*/
	SHORT	packetRate;				/* current packets per second					*/
	SHORT	packetMax;				/* maximum packet rate today					*/
	SHORT	packetCount;			/* packet count this second						*/
	LONG	packetTotal;			/* packets processed today						*/
	CHAR	refreshSymbol[32];		/* current refresh symbol						*/
	USHORT	refreshCap;				/* times refresh has caped today 				*/
	USHORT	sequenceGaps;			/* sequence gaps								*/
	USHORT	lineErrors[10];			/* communications errors of primary line		*/
	USHORT	line1Errors[10];    	/* communications errors of line a				*/
	USHORT	line2Errors[10];    	/* communications errors of line b				*/
	LONG	totalPackets;
	CHAR	szVersion[16];
	ULONG	ulBBOCount;				/* Total BBO's calculated						*/
	ULONG	ulIsBBO;				/* Number that resulted in a new BBO			*/
	ULONG	ulBBOAppendage;			/* Number that resulted in Appendages			*/
	ULONG   ulBarDBMaxkeys;			/* Max Bar Keys configured						*/
	ULONG	ulBarDBNumkeys;			/* Number of Bar Keys used						*/
	ULONG	ulBarQCount;			/* Number of Bars Queued						*/
	ULONG	ulTSQCount;				/* Number of TS's Queued						*/
	ULONG	ulToSQCount;			/* Number of TOS's Queued						*/
	ULONG	ulBarDbFullErrors;		/* Number of time Bar Db was full				*/
	ULONG	ulLowMemoryErrors;		/* Number of Low Memory Errors					*/
	ULONG 	ulDelaySize;			// Sizeof the Delay buffer (bytes)
	ULONG 	ulDelayMsgs;			// Messages in the Delay buffer (bytes)
	ULONG 	ulDelayBytes;			// Messages in the Delay buffer (bytes)
	ULONG 	ulDelayWraps;			// Number of Delay buffer wraps
	ULONG 	ulRealTimeSize;			// Sizeof the Real Time buffer (bytes)
	ULONG 	ulRealTimeMsgs;			// Messages in the Real Time buffer (bytes)
	ULONG 	ulRealTimeBytes;		// Messages in the Real Time buffer (bytes)
	ULONG 	ulRealTimeWraps;		// Number of Real Time buffer wraps
	ULONG 	ulDelay30Size;			// Sizeof the Delay 30 buffer (bytes)
	ULONG 	ulDelay30Msgs;			// Messages in the Delay 30 buffer (bytes)
	ULONG 	ulDelay30Bytes;			// Messages in the Delay 30 buffer (bytes)
	ULONG 	ulDelay30Wraps;			// Number of Delay 30 buffer wraps
	ULONG 	ulDelay15Size;			// Sizeof the Delay 15 buffer (bytes)
	ULONG 	ulDelay15Msgs;			// Messages in the Delay 15 buffer (bytes)
	ULONG 	ulDelay15Bytes;			// Messages in the Delay 15 buffer (bytes)
	ULONG 	ulDelay15Wraps;			// Number of Delay 15 buffer wraps
	ULONG	ulrecordLockErrors;		// Number of Record Lock Errors from DBAX_LOCK
} STTICKERSTATS;

typedef struct _STQSSTATS 
{
	CHAR	szVersion[16];
	LONG	startTime;
	ULONG	totalPackets;
	ULONG	etiQueueCount;
	ULONG	totalInterestRecs;
} STQSSTATS;


typedef struct _STDATABASE 
{
	ULONG	nAdds;
	ULONG	nSoftDeletes;
	ULONG	nHardDeletes;
	ULONG	nUpdates;
	ULONG	nRecords;
	ULONG	dbInfo;
	char	reserved[8];
} STDATABASE;

typedef struct _STMODE 
{
	CHAR	szGroupName[32];
	CHAR	szType[16];
	BOOL	isEnabled;
} STMODE;

#define DBA_INFORMATION_RECTYPE_DCP				0	
#define DBA_INFORMATION_RECTYPE_LM				1	
#define DBA_INFORMATION_RECTYPE_XMIT			2	
#define DBA_INFORMATION_RECTYPE_ALARM			3
#define DBA_INFORMATION_RECTYPE_BUFFER			4	
#define DBA_INFORMATION_RECTYPE_ETIX			5	
#define DBA_INFORMATION_RECTYPE_ETIX_CLIENT		6	
#define DBA_INFORMATION_RECTYPE_DML				7	
#define DBA_INFORMATION_RECTYPE_DBA				8	
#define DBA_INFORMATION_RECTYPE_DBAMSG			9	
#define DBA_INFORMATION_RECTYPE_TICKER			10	
#define DBA_INFORMATION_RECTYPE_QS				11	
#define DBA_INFORMATION_RECTYPE_IPINFO			12	
#define DBA_INFORMATION_RECTYPE_DATABASEINFO	13	
#define DBA_INFORMATION_RECTYPE_MODE			14
#define DBA_INFORMATION_MAX_RECTYPE				14
	
typedef struct _INFORMATION_DATA 
{
	int		type;
	union 
	{
		STDCPSTATS		dcpStats;
		STLMSTATS		lmStats;	
		STXMITSTATS		xmitStats;
		STALARMSTATS	alarmStats;
		STBUFFERSTATS	bufferStats;
		ST_ETIXSTATS	etixStats;
		ST_ETIX_CLIENT_STATS etixClient;	
		STDML			dml;
		STDBASTATS		dba;
		STDBA_MESSAGESTATS dbaMsg;
		STTICKERSTATS	ticker;
		STQSSTATS		qs;
		STDATABASE		dataBase;
		STMODE			mode;
	};
} INFORMATION_DATA;


typedef struct _INFORMATION_RECORD
{
	DBA_INFORMATION_KEY				key;
	INFORMATION_DATA				data;
} INFORMATION_RECORD, *lpINFORMATION_RECORD;


// Class Def.
#define ST_STATS_PROGRAM_NAME_DCP			"DCP"
#define ST_STATS_PROGRAM_NAME_LINE_MGR		"LineMgr"
#define ST_STATS_PROGRAM_NAME_XMIT			"Xmit"
#define ST_STATS_PROGRAM_NAME_ALARM			"Alarm"
#define ST_STATS_PROGRAM_NAME_BUFFER		"Buffer"
#define ST_STATS_PROGRAM_NAME_ETIX			"Etix"
#define ST_STATS_PROGRAM_NAME_DBA			"Dba"
#define ST_STATS_PROGRAM_NAME_TICKER		"Ticker"
#define ST_STATS_PROGRAM_NAME_QS			"QuoteServer"
#define ST_STATS_PROGRAM_NAME_IPINFO		"IPInformation"
#define ST_STATS_PROGRAM_NAME_DATABASEINFO	"DataBase"
#define ST_STATS_PROGRAM_NAME_MODE			"Mode"


#ifdef __cplusplus					//  added to build wt.dll

class CSTInformation  
{
public:
	DLLEXPORT CSTInformation();
	DLLEXPORT virtual ~CSTInformation();

	DLLEXPORT BOOL GetInformation(DBA_INFORMATION_KEY *pKey,INFORMATION_RECORD *pInfoRecord,
																					int command=1); //DBA_GET_EQUAL 
	DLLEXPORT BOOL SetInformation(DBA_INFORMATION_KEY *pKey,INFORMATION_RECORD *pInfoRecord);

	DLLEXPORT BOOL GetInformation(STATSBUF *packetBuffer);
	DLLEXPORT BOOL SetInformation(STATSBUF *packetBuffer);
	DLLEXPORT BOOL Connect(char *server=NULL, char *login=NULL, char *password=NULL);
	DLLEXPORT BOOL DisConnect();
	DLLEXPORT BOOL isConnected(){return m_bdbaAttached;}
	DLLEXPORT BOOL GetCommands(ST_STATS_CONTROL *pCtrlMsg,char *pProgramName=NULL,char *pName=NULL);
	DLLEXPORT BOOL AttachCommands();
	DLLEXPORT BOOL FreeCommands();
	DLLEXPORT BOOL SendCommand(ST_STATS_CONTROL *pCtrlMsg);
	DLLEXPORT BOOL SendCommand(COMMAND *pStatsCommand);
	DLLEXPORT BOOL StartDBA();
	DLLEXPORT void SetDMLMode(char *dmlMode);
	DLLEXPORT BOOL GetServerNames(char *pProgramName,char *serverName,int *nextIndex);

	char m_machineName[100];

private:
	BOOL GetInformationKey(DBA_INFORMATION_KEY *pKey,STATSBUF *packetBuffer);
	BOOL GetSubInformation(DBA_INFORMATION_KEY *pKey,INFORMATION_RECORD *pInfoRecord,
																STATSBUF *packetBuffer);
	BOOL SetSubInformation(DBA_INFORMATION_KEY *pKey,INFORMATION_RECORD *pInfoRecord,
																STATSBUF *packetBuffer);
	BOOL GetFunStats(DBA_INFORMATION_KEY *pKey,INFORMATION_RECORD *pInfoRecord,
																STATSBUF *packetBuffer);
	BOOL SetFunStats(DBA_INFORMATION_KEY *pKey,INFORMATION_RECORD *pInfoRecord,
																STATSBUF *packetBuffer);
	BOOL GetFunLineStats(DBA_INFORMATION_KEY *pKey,INFORMATION_RECORD *pInfoRecord,
																STATSBUF *packetBuffer);
	BOOL SetFunLineStats(DBA_INFORMATION_KEY *pKey,INFORMATION_RECORD *pInfoRecord,
																STATSBUF *packetBuffer);
	BOOL GetLTSStats(DBA_INFORMATION_KEY *pKey,INFORMATION_RECORD *pInfoRecord,
																STATSBUF *packetBuffer);
	BOOL SetLTSStats(DBA_INFORMATION_KEY *pKey,INFORMATION_RECORD *pInfoRecord,
																STATSBUF *packetBuffer);
	BOOL GetDCPStats(DBA_INFORMATION_KEY *pKey,INFORMATION_RECORD *pInfoRecord,
																STATSBUF *packetBuffer);
	BOOL GetDCPLineStats(DBA_INFORMATION_KEY *pKey,INFORMATION_RECORD *pInfoRecord,
																STATSBUF *packetBuffer);
	BOOL SetDCPLineStats(DBA_INFORMATION_KEY *pKey,INFORMATION_RECORD *pInfoRecord,
																STATSBUF *packetBuffer);
	BOOL DbaAttach(char *server, char *login, char *password);
	BOOL DbaDetach();
	
	friend void _cdecl EtiInput(void *myThis); 
	VOID EtiInputThread();

	UCHAR m_dbaPblk[256];
	UCHAR pBlk[256];
	UCHAR m_dmlMode[256];

	BOOL m_bdbaAttached;
	BOOL m_bTerminate;
	BOOL m_bFreeCommands;
	BOOL m_bDbaInitialized;
	
	int  m_lineRequestType;
	int	 m_lastTickCount;

	void *pCCommandQueue;
	void *m_pTransmit;
};

#endif						//  end of if cplusplus

#ifndef INCL_NOPACKING
	#pragma pack() 						 
#endif

#endif

