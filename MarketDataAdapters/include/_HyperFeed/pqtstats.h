#if !defined(STATS_INCL)
#define STATS_INCL

#if !defined(DllExport)
#define DllExport	__declspec( dllexport )
#endif

#pragma pack(1)					

#define SM_PORTNO			    1000
//#define SM_MAXBUFLEN		  9408
#define SM_MAX_TYPES      32

#define SM_PACKETHEADER "\b\a\r\t"

#define SM_CONNECT_TYPE_NONE	0
#define SM_CONNECT_TYPE_CLIENT	1
#define SM_CONNECT_TYPE_WTI		2
#define SM_CONNECT_TYPE_FUNNEL	3
#define SM_CONNECT_TYPE_DCP		4
#define SM_CONNECT_TYPE_DBS		5
#define SM_CONNECT_TYPE_LTS		6
#define SM_CONNECT_TYPE_LOG		7
#define SM_CONNECT_TYPE_WDB		8
#define SM_CONNECT_TYPE_CHRON	9
#define SM_CONNECT_TYPE_BUFFER	10
 
#define SM_CONNECT_TYPE_MAX		SM_CONNECT_TYPE_BUFFER

#define SM_TYPE_CONNECT			1
#define	SM_TYPE_DISCONNECT		2
#define SM_TYPE_REQUEST			3
#define SM_TYPE_GIVE_STATS		4
#define SM_TYPE_TAKE_STATS		5
#define SM_TYPE_SEND_COMMAND	6

#define SM_STATS_WTI			1
#define SM_STATS_WTICLIENT		2
#define SM_STATS_STATSMGR		3
#define SM_STATS_DCP			4
#define SM_STATS_DCPLINE		5
#define SM_STATS_FUN			6
#define SM_STATS_FUNLINE		7
#define SM_STATS_DBS			8
#define SM_STATS_LTS			9
#define SM_STATS_LOG			10
#define SM_STATS_WDB  			11
#define SM_STATS_CHRON			12
#define SM_STATS_ALARM			13
#define SM_STATS_BUFFER			14


#define FULINE_STATS_TYPE_COMPOSITE		0
#define FULINE_STATS_TYPE_LINE1			1
#define FULINE_STATS_TYPE_LINE2			2

/*
** This is the structure used for a SM_TYPE_CONNECT message.
*/
typedef struct _SMCONNECT 
{
	int		connectType;
	char	version[10];
	char	username[40];
	char	password[40];
} SMCONNECT;


/*
** This is the structure used for a SM_TYPE_REQUEST message.
*/
typedef struct _SMREQUEST 
{
	int		requestType;
} SMREQUEST;


/*
** These are the structures used for a SM_TYPE_GIVE_STATS message.
*/
typedef struct _WTISTATS 
{
	int		pps;
	int		packets;
	int		syncClients;
	int		asyncClients;
} WTISTATS;

typedef struct _WTICSTAT 
{
	int		type;
	char	process[64];
	UINT	lastMS;
	UINT	minMS;
	UINT	maxMS;
	UINT	avgMS;
	UINT	totalMS;
	ULONG	packets;
} WTICSTAT;

typedef struct _WTICLIENTSTATS 
{
	int			  clientCount;
	WTICSTAT	client[32];
} WTICLIENTSTATS;

typedef struct _STAT 
{
	int		num;
	int		type;
	char	name[100];
	int		xCount;
	int		xBytes;
	int		rCount;
	int		rBytes;
	char	version[10];
} STATCLIENT;

typedef struct _STATSSTATS 
{
	int			    clientCount;
	STATCLIENT	client[50];
} STATSSTATS;

/*
** These are the structures for SM_STATS_DCP messages.
*/
typedef struct _DCP 
{
	char	deviceName[20];
	int		lineCount;
} DCP;

typedef struct _DCPSTATS 
{
	int		dcpCount;
	DCP		dcp[10];
} DCPSTATS;

/*
** These are the structures for SM_STATS_DCPLINE messages.
*/
typedef struct _DCPLSTAT 
{
	int		dcpNum;
	char	deviceName[20];
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
} DCPLSTAT;


typedef struct _DCPLINESTATS 
{
	int			  lineCount;
	DCPLSTAT	line[80];
} DCPLINESTATS;

/*
** These are the structures for SM_STATS_FUN messages.
*/
typedef struct _FUN 
{
	char	funName[60];
	int		lineCount;
} FUN;

typedef struct _FUNSTAT 
{
	int		funCount;
	FUN		fun[10];
} FUNSTATS;

/*
** These are the structures for SM_STATS_FUNLINE messages.
*/
typedef struct _FUNLINE 
{
	char    funName[40];
	char    lineName[40];
	ULONG   status;
	ULONG   sequence;
	ULONG   gaps;
	ULONG   bytesReceived;
	ULONG   received;
	ULONG   ok;
	ULONG   sent;
	ULONG   rejected;
	ULONG   bad;
	char	version[10];
	BOOL	alarm;
	ULONG   ignored;
	ULONG	outOfSequence;
	ULONG	statsType;
	ULONG	sequenceA;
	ULONG	sequenceB;
	int		stateA;
	int		stateB;
	UCHAR	reserved[8];
} FUNLINE;

typedef struct _FUNLSTAT 
{
	int     lineCount;
	FUNLINE	line[64];
} FUNLINESTATS;

#define SM_MAXBUFLEN	max(sizeof(FUNLINESTATS),11000)

/*
**  Structures for DBS stats
*/

//      stats for each DBS process

typedef struct  _DBS_PROCESS_FLAG
{
	UINT    fLoaded             :1;             // process loaded
	UINT    fWait               :1;             // process in wait loop ( for another process to start)
	UINT    fRunning            :1;             //  process running
	UINT    fAlarm              :1;             //  process alarming
	UINT    fTerminate          :1;             //  process terminated
	UINT    afOptBboEnabled     :1;
	UINT    afPartialEnabled    :1;
	UINT    afTimeDateEnabled   :1;
	UINT    afCorxLogEnabled    :1;
	UINT    afRexLogEnabled     :1;
	UINT    afTimeLogEnabled    :1;
	UINT    afPartialLogEnabled :1;
	UINT    fSkipOptAdd         :1;
	UINT    fMSDisabled         :1;             //  alarming enabled this process
	UINT    fTSEnabled          :1;             //  alarming enabled this process
	UINT    fAlarmEnabled       :1;             //  alarming enabled this process
	UINT    afCalcBidTick       :1;             //  alarming enabled this process
	UINT    afOrderLogEnabled   :1;
	UINT    afOrderEnabled      :1;
	
	UINT    fill                :13;  
} DBS_PROCESS_FLAG;


typedef struct  _DBS_PROCESS_STATS
{
	CHAR                psID[16];         //  process ID ( for stats displaying
	DBS_PROCESS_FLAG    psFlag;           //  state flag
	USHORT              psAlarmCount;     //  current alarm count
	LONG                psReservedLong;   //
	CHAR                psReserved[8];    //
} DBS_PROCESS_STATS, * lpDBS_PROCESS_STATS;


//  stats and counts for each database processed by DBS

typedef struct 
{
	ULONG 	dvRecs;						//  current number of record
	ULONG	  dvMaxRecs;					//  maximum records allowed
	ULONG 	dvSavePosition;				//  record being saved ( during db save )
	ULONG   dvSkipQuotes;               //  number of quote messages dropped  
	ULONG   dvAdds;                     //  number of keys added 
	ULONG   dvReserved[3];              //
	
	SHORT   dvDataBaseNum;              //  database number
	BOOL    dvDoSave;                   //  start save flag
} DATABASE_VALUES, * lpDATABASE_VALUES;


//    Values for each Monitored database ITEM

typedef struct 
{
	ULONG	ivDatabaseNumber;             //  database number
	CHAR	ivItem[20];                   //  item key value
	LONG	ivValue;                      //  value to monitor ( usually last sale )
	LONG	ivNetChange;                  //  net change
	SHORT 	ivPriceType;                  //  price type of value and change
	SHORT 	ivDisplayPriceType;           //  display price type
} ITEM_VALUES, * lpITEM_VALUES;

//    Monitored stats and counts,etc for DBSCHRON ( reserved )

typedef struct  _CHRON_DATA
{
	CHAR        resreved[64];
} CHRON_DATA, * lpCHRON_DATA;

//    Monitored stats and counts,etc for database transfers ( reserved )

typedef struct  _DB_XFER_DATA
{
	CHAR        resreved[64];
} DB_XFER_DATA, * lpDB_XFER_DATA;


typedef struct  _DBS_STATS
{
/*
** dbs system parameters, data
	*/
	LONG		dsStatsInterval;  // wait interval for stats send ( milliseconds)
	
	BOOL		dsBusy;           // DBS in busy period
	BOOL		dsLogging;        // DBS logging enabled
	BOOL		dsXmit;           // DBS transmitting enabled
	BOOL		dsCalcBidTick;    //  
	BOOL		dsLogPTChanges;   // log pricetype changes
	
	ULONG		dsAlarmLimit;     // current alarm limit in use 
	ULONG		dsOpenIntFilter;  // current option open interest filter
	
	ULONG		dsRxSequence;     //  recieve sequence number
	ULONG		dsTxSequence;     //  transmit sequence number
	CHAR		dsStreamItem[20];     //  WTI item currently processed
	CHAR		dsMessage[24];    //  status messages
	BOOL    dsPriceTS;        //  time and sales enabled
	ULONG   dsPartialQueueLimit;
	ULONG   dsPartialQueueCount;
	
	CHAR    dsFill[120];
	
	/*
    reserved for Chron
	
	  CHRON_DATA           dsChronData; 
	  
		reserved for dbatabase xfer 
		DB_XFER_DATA        dsDBXfer;
	*/
	
	
	union
	{
		DBS_PROCESS_STATS dbsStatsTab[8];       //  process statics up to 8 process
		struct
		{
			DBS_PROCESS_STATS  dbsStats;
			DBS_PROCESS_STATS  statsStats;
			DBS_PROCESS_STATS  indicesStats;
			DBS_PROCESS_STATS  dbsmonStats;
			DBS_PROCESS_STATS  dbschronStats;
			DBS_PROCESS_STATS  reservedStats[3];
		} dbsPrStats;
	} dsStats;
	/*
    ** data base values
	*/
	union
	{
		DATABASE_VALUES    dbvTable[4];     // datbase values up to 4 databases
		struct
		{
			DATABASE_VALUES  dbvExtended;
			DATABASE_VALUES  dbvMarketMaker;
			DATABASE_VALUES  dvsReserved[2];
		} dbv;
	} dsDatabaseValues;
	ITEM_VALUES       ivsTable[8];        //  items being monitored ( up to 8 )  
	CHAR              filler[1024 - 928];
} DBS_STATS, * lpDBS_STATS;


//  Stats for DbsChron

typedef struct  _DBSCHRON_PROCESS_FLAG
{
	UINT    fLoaded       :1;             // process loaded
	UINT    fRunning      :1;             //  process running
	UINT    fTerminate    :1;             //  process terminated
	UINT    reserved      :11;
	UINT    fAlarmEnabled :1;             //  alarming enabled this process
	UINT    fEventPending :1;
} DBSCHRON_PROCESS_FLAG;



typedef struct  _DBSCHRON_STATS
{
	DBSCHRON_PROCESS_FLAG   flag;
	USHORT				          dsMinToNextStart;
	USHORT				          dsNextEventHour;
	USHORT				          dsNextEventMinute;
	USHORT                  dsNumConnects;
	
	CHAR                    reserved[1024 - 10];
} DBSCHRON_STATS, * lpDBSCHRON_STATS;






#define MESSAGE_TYPE_MAX 40
typedef struct _PACKETRATE 
{
	ULONG		pps;
	ULONG		currentRate;
	ULONG		bps;
} PACKETRATE;

typedef struct _LTSSTAT 
{
	char		ltsName[20];
	PACKETRATE	tx;
	PACKETRATE	extLoop;
	PACKETRATE	mmLoop;
	PACKETRATE	ttLoop;
	PACKETRATE	intlRate;
	PACKETRATE	intlLoop;
	ULONG		outCnt[MESSAGE_TYPE_MAX + 2];
	char		streamSym[20];
	char		extLoopSym[20];
	char		ttLoopSym[20];
	char		mmLoopSym[20];
	char		spLoopSym[20];
	ULONG		mainQueue;
	ULONG		waitQueue;
	ULONG		intlQueue;
	ULONG		pipeQueue;
	ULONG		nrsRealQueue;
	ULONG		nrsRefQueue;
	ULONG		ctxRealQueue;
	ULONG		ctxBatchQueue;
	ULONG		txUsed;
	BOOL		alarm;
	BOOL		loopsEnabled;
	BOOL		transmitEnabled;
	PACKETRATE	spLoop;
	PACKETRATE	bookLoop;
	char		bookLoopSym[20];
} LTSSTAT;

typedef struct _LTSSTATS 
{
	int			ltsCount;
	//	LTSSTAT		lts[5];
	LTSSTAT		lts[32];  // Added 10/13/2000 ALP for New Version of the Transmitter
} LTSSTATS;

/*
** Note that this is a copy of a logrec structure from logmgr.h
*/
typedef struct _lgrec 
{
	int		severity;						// LOG_SEVERITY_...
	int		program;						// SM_CONNECT_TYPE...
	char	machineName[20];
	char	programName[20];
	char	text[120];
	SYSTEMTIME	timeStamp;
} LGREC;

typedef struct _errorrec 
{
	unsigned int	seq;
	LGREC			body;
} ERRORREC;

typedef struct _LOGSTATS 
{
	int			logCount;
	ERRORREC	log[50];
} LOGSTATS;


typedef struct _ALARMSTATS 
{
	BOOL	funAlarm;
	BOOL	dbsAlarm;
	BOOL	xmitAlarm;
	char	reserved[13];
} ALARMSTATS;


typedef struct _BUFFERSTATS 
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
	ULONG			reserved[3];
}BUFFERSTATS;

typedef struct _STATS 
{
	int		type;
	union 
	{
		WTISTATS		wtiStats;
		WTICLIENTSTATS	wtiClient;
		STATSSTATS		statsMgr;
		DCPSTATS		dcpStats;
		DCPLINESTATS	dcpLine;
		FUNSTATS		funStats;
		FUNLINESTATS	funLine;	
		DBS_STATS		dbsStats;
		DBSCHRON_STATS	chronStats;
		LTSSTATS		ltsStats;
		LOGSTATS		logStats;
		ALARMSTATS		alarmStats;
		BUFFERSTATS		bufferStats;
	};
} STATS;

typedef struct _COMMANDBUF 
{
	int	commandType;
	
	union 
	{
		struct 
		{
			int lineMgrNum;
			int	lineNum;
			int command;
			char name[40]; 
			char lmName[40]; 
		} lineMgr;
		
		struct 
		{
			int xmitterNum;
			int number;
			int command;
			char	symbol[20];
			char	name[40]; // Added 10/13/2000 ALP for New Version of the Transmitter
			char	ltsName[40]; 
		} ltsMgr;
	};
} COMMAND;

/*
** This is the union that contains all the possible message types.
*/
typedef struct _STATSBUF 
{
	int		      type;
	union 
	{
		SMCONNECT	connect;
		SMREQUEST	request;
		STATS		stats;
		COMMAND		command;	
		char		  raw[SM_MAXBUFLEN];
	};
} STATSBUF;

#pragma pack()

#if defined(__cplusplus)
extern "C" 
{
#endif
	BOOL DllExport WINAPI StatsRecv(SOCKET s, STATSBUF *packetBuffer);
	BOOL DllExport WINAPI StatsSend(SOCKET s, STATSBUF *packetBuffer);
	
	ULONG DllExport WINAPI StatsLen(STATSBUF *packetBuffer);
	BOOL DllExport WINAPI StatsConnect(SOCKET *mySocket, char *hostName,
		int connectType, char *version);
	BOOL DllExport WINAPI StatsDisconnect(SOCKET *mySocket);
	
#if defined(__cplusplus)
}
#endif
#endif
