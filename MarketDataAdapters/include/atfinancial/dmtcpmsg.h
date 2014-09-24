/*
 *		DMTCPMsg.h		Date=000831
 *
 *		Data Manager TCP Message Structures
 *
 *		Written 12/19/97	C. Baker
 *
 *		Modified 07/09/98	C. Baker
 *			Don't need AtheDlg.h anymore
 *			Add AtDatmanManager Terminating Msg
 *			Add Test Connection Messages
 *
 *		Modified 07/13/98	C. Baker
 *			Added Multi-Index Deliver Update Message -> for price volume
 *
 *		Modified 07/22/98	C. Baker
 *			Corporate Action request and Deliver Refresh Message
 *
 *		Modified 07/28/98	C. Baker
 *			Added error and Done/Partial to status of msg
 *
 *		Modified 08/14/98	C. Baker
 *			Added period to request study message.
 *
 *		Modified 09/18/98	C. Baker
 *			Changed QSDTID to QSN.
 *
 *		Modified 09/20/98	C. Baker
 *			Add Tick trade type definitions.
 *
 *		Modified 11/03/98	C. Baker
 *			Change Max Splits to 5 from 3
 *
 *		Modified 12/21/98	C. Baker
 *			Add message to show ATDatMan
 *
 *		Modified 01/14/99	C. Baker
 *			Add Errors for Data Connection Loss
 *
 *		Modified 01/15/99	C. Baker
 *			Add Errors for Trading Halted
 *
 *		Modified 06/23/99	C. Baker
 *			Change bHasNews to dwPBFlags in ATDM_DLVR_PB_INFO
 *
 *		Modified 08/31/00	C. Baker
 *			Add ATServer registry path
 *
 *		Modified 10/19/00	S. Aharrah
 *			Add IDS_TCP_CONN_LOSS for detected local TCP connection loss.
 *
 *		Copyright (c) A-T Financial Information, Inc., 1997-2000.
 *		All rignts reserved.
 *
 */

/*=============================================================*/

/*** AT Data Manager Data Standards ***

  1) All array indexes start at 0.

  2) Most Recent Data has the lowest array index.

  For Example:

	double Data[3];

	Data[0] -> Most Recent Data 
	Data[1] -> Older Data
	Data[2] -> Oldest Data

  3) The Oldest Index in DLVR_DATA and OLD_DATA messages is one greater than the actual data array index

  4) All data is stored in Shared Memory and all fields are floating point values.

  5) The data is seperated into 2 arrays.  The first array contains the date and time.  The other array contains the data.

  6) There are many differnet types of data formats.  BAR, TICK and POINT_FIGURE data have specific structures.  STUDY data does not have any specific format.  Study data is always a continutous list of floating point values.

***  End AT Data Manager Data Standards ****/

/***  AT Data Manager Message Logic ***

  The TEST message can be used to test a connection.
  The Data Manager will respond with a DLVR_TEST message.

  To request data, the client must send a REQ_SYM message.
  All of the fields in message are required.

  If the client wants to receive updates while the market is open, the most recent date should be set to (DWORD)-1 .

  The Data Manager will always respond to a REQ_SYM message with a DLVR_DATA message.
  The DLVR_DATA message contains the names of the shared memory areas.
  The DLVR_DATA message contains a Data Manager ID, which should be used in any subsequent requests to the data manager for that data.

  The DLVR_DATA message may not have all of the data, so subsequent DLVR_OLD messages may follow.
  The status field will indicate if more data is coming.

  When a client is finished with the data, the client should send an UNREQ_DATA message with the corresponding Data Manager ID.
  If a client is done with all of its data sets, the UNREQ_ALL message will get rid of all of the clients data sets.

  If a client is closing, the CLOSE_SOCKET message helps speed up the closing process.
  The Data Manager will respond with a SOCKET_CLOSED message.

  To request Study data, the client needs to request the study with all of the REQ_STUDY fields filled in and the Data Manager IDs for the data to be studied.
  The Data Manager will always respond to a REQ_STUDY message with a DLVR_DATA message.
  The DLVR_DATA message may not have all of the data, so subsequent DLVR_OLD messages may follow.
  The status field will indicate if more data is coming.

  The REQ_MORE message will retrieve more data for that particular Data Manager ID.
  The Data Manager will always respond with at least one DLVR_OLD_DATA message.  There could be several DLVR_OLD messages, and the status field will indicate if more are coming.

  The CORP_ACTION message will open the Local Corporate Action dialog box.  This enables the user to enter ammended corporate action records.
  If the data has cnaged the Data Manager will send the client a DLVR_REFRESH message.  The client should unrequest the previous data, and request the data again.

  Whenever a shared memory space has become too small.  A new memory space will be allocated, and a DLVR_NEW_MEM message will be sent with the names of the new areas.
  The previous Shared Memory will not be deleted until all clients who have requested the data have responded with a USE_NEW_MEM message.

  When Data Manager receives an update from ATServer, the Data Manager will send a DLVR_PB_INFO message to every client who has request the data.

  The DLVR_INDEX_UPDATE is for those data sets that are not time series, such as the study Price Volume.
  The DLVR_INDEX_UPDATE message is a substitue for the DLVR_OLD and DLVR_NEW messages.

  The Data Manager sends a DLVR_HEARTBEAT message during its idle processing, and they are sent at undetermined intervals.  The Data Manager Heartbeat is not intended for timing of anything.

  When the Data Manager is closing for any reason, it will send a DLVR_TERMINATING message to all of its clients.

***  End Message Logic ***/

/*** Sequential Processing ****

  The WAIT_FOR_RESPONSE bit will be echoed back to the client in the initial response to a clients request for data.

  Error Messages will not contain this bit.  Only DLVR_DATA and DLVR_OLD.

 ***  End Sequential Processing ***/

/*** Error Handling ****

  The Data Manager will send a DLVR_ERROR message on any error.  The error codes are defined at the end of this file.

  Also, the LOWORD of status field in DLVR_DATA and DLVR_OLD and INDEX_UPDATE will contain the error code.

  The state of the data can be determined using the STATUS bit masks.

*** End Error Handling ***/

#ifndef __DMTCPMsg_DEFINES
#define __DMTCPMsg_DEFINES

#include <atfi.h>

#include <struct.h>
#include <Tsl.h>

// ********** Time Data Structure
/*  defined in struct.h
typedef struct _ATDM_TIME
{
	DWORD	min: 6,  // 6 bits = minute
			hour: 5,
			day: 5,
			month: 4,
			year: 7,
			unused: 5;
} ATDM_TIME;
*/			

// ********** End Time Data Structure

// ************ TCP Port Setup Constants

// Registry path for ATDataManager
#define	ATDM_ATServerReg "Software\\A-T Financial\\ATDatman"

// Registry path for ATServer
#define	ATDM_ATServerReg2 "Software\\A-T Financial\\ATServer"

// default port, if a port is not defined in the Registry
#define	ATDM_AttachPort      0x5252

// ************ End TCP Port Setup Constants

// Status Flags:
//		The client can use the ATDM_STATUS_WAIT_FOR_RESPONSE, and it will be returned in the subsequent Dlvr_data message
//		Status Flags are only relevant in DLVR_DATA, and  the OLD_DATA messages
//
//		The LOWORD is reserved for error codes

// Status Bit Masks for ATDataManager API Synchronous responses
#define ATDM_STATUS_WAIT_FOR_RESPONSE	0x80000000

// status bits for internal Data Manger requests, the data manager initiated this request
#define ATDM_STATUS_DM_REQ_MORE			0x40000000
#define ATDM_STATUS_DM_REQ_SYM			0x20000000

// status of the data
#define ATDM_STATUS_NONE				0x02000000
#define ATDM_STATUS_PARTIAL				0x04000000
#define ATDM_STATUS_COMPLETE			0x08000000

// ************ End TCP Message Flags and Bit Masks

// *** Data Manager Request Message Types -- dwMsgType Field
#define ATDM_MSGTYP_REQ_BEGIN		1 // Not a valid message, this is used for message validation
#define ATDM_MSGTYP_REQ_SYM			2
#define ATDM_MSGTYP_REQ_STUDY		3
#define ATDM_MSGTYP_UNREQ_DATA		4
#define ATDM_MSGTYP_UNREQ_ALL		5
#define ATDM_MSGTYP_SUSPECT_DP		6
#define ATDM_MSGTYP_REQ_MORE		7
#define ATDM_MSGTYP_USE_NEW_MEM		8
#define ATDM_MSGTYP_CLOSE_SOCKET	9
#define	ATDM_MSGTYP_TEST			10
#define	ATDM_MSGTYP_CORP_ACTION		11
#define	ATDM_MSGTYP_SHOW_DM			12
#define ATDM_MSGTYP_REQ_END			13  // Not a valid message, this is used for message validation
// *** End Data Manager message types

// *** Data Manager Deliver Message Types -- dwMsgType Field
#define ATDM_MSGTYP_DLVR_BEGIN			100 // Not a valid message, this is used for message validation
#define ATDM_MSGTYP_DLVR_DATA			101
#define ATDM_MSGTYP_DLVR_REC_DATA		102
#define ATDM_MSGTYP_DLVR_OLD_DATA		103
#define ATDM_MSGTYP_DLVR_NEW_MEM		104
#define ATDM_MSGTYP_DLVR_ERROR			105
#define ATDM_MSGTYP_DLVR_PB_INFO		106
#define ATDM_MSGTYP_DLVR_HEARTBEAT		107
#define ATDM_MSGTYP_DLVR_SOCKET_CLOSED	108
#define	ATDM_MSGTYP_DLVR_TEST			109
#define	ATDM_MSGTYP_DLVR_TERMINATING	110
#define	ATDM_MSGTYP_DLVR_INDEX_UPDATE	111
#define	ATDM_MSGTYP_DLVR_REFRESH		112
#define ATDM_MSGTYP_DLVR_END			113  // Not a valid message, this is used for message validation
// *** End Data Manager Request Message Types -- dwMsgType Field

	// Tick data record types
// vvvvvvvvvvvvvv Stolen from TICKDB.H vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

#define TICKTYPE_TRADE	0			// trade
#define TICKTYPE_BID	1			// bid
#define TICKTYPE_ASK	2			// ask

// ************ Begin TCP Messages

typedef struct _ATDM_HEADER  // basic information all messages have
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
} ATDM_HEADER;

typedef struct _ATDM_REQ_SYM_DATA  // Application Request for Symbol (Non-Study) Data
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	DWORD	dwQSN;
	PB		pb;
	int		dwTimePeriod;
	CHART_ENTRY_DATA	dwSymParams;
	int		iDispPoints;		// Displayable Data Points
	ATDM_TIME stOldestTime;		// Oldest Time
	ATDM_TIME stMostRecTime;	// Most Recent Time
	ATDM_TIME stFilterStart;	
	ATDM_TIME stFilterEnd;	
} ATDM_REQ_SYM_DATA;

typedef struct _ATDM_REQ_STUDY_DATA
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	int		iDispPoints;		// Displayable Data Points
	ATDM_TIME stOldestTime;		// Oldest Time
	ATDM_TIME stMostRecTime;	// Most Recent Time

	char	szDLLName[TS_LIB_SUB_KEY_NAME_LEN]; //selected study dll name
	DWORD	dwStudyPosition; //the ordinal position of the selected study in the DLL
	int		iUsesDMID[TS_MAX_LINE_NUM];  // Data Manager ID's used by this Study instance
	DWORD	dwDailyData;
	double	dBoxSize;
	char	sStudyParam[TS_MAX_TSP]; //This term holds the "study" specific information.
	DWORD	dwPeriod;  // primarily used for SMA when using daily data -> this tells the study what the real period is (Monthly or Weekly)
} ATDM_REQ_STUDY_DATA;

typedef struct _ATDM_REQ_MORE_DATA
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	int		iDMDataID;
	int		iAddPoints;		// Number of Additional Data Points Needed
	ATDM_TIME stOldestTime;		// Oldest Time
	ATDM_TIME stMostRecTime;	// Most Recent Time
} ATDM_REQ_MORE_DATA;

typedef struct _ATDM_UNREQ_DATA
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	int		iDMDataID;
} ATDM_UNREQ_DATA;

typedef struct _ATDM_UNREQ_ALL_DATA
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
} ATDM_UNREQ_ALL_DATA;

typedef struct _ATDM_SUSPECT_DP
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	int		iDMDataID;
	int		iDataPoint;
} ATDM_SUSPECT_DP;

typedef struct _ATDM_USING_NEW_MEM
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	int		iDMDataID;
} ATDM_USING_NEW_MEM;

typedef struct _ATDM_CLOSE_SOCKET
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
} ATDM_CLOSE_SOCKET;

typedef struct _ATDM_CORP_ACTION
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	int		iDMDataID;
} ATDM_CORP_ACTION;

typedef struct _ATDM_SHOW_DM
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	int		nCmdShow; 	// show state of window
} ATDM_SHOW_DM;

#define ATDM_MAX_SPLITS 5

typedef struct _ATDM_DLVR_DATA
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	int		iDMDataID;
	char	szTimeName[128];
	char	szDataName[128];
	int		iMostRecIndx;	// Most Recent Data Point Index
	int		iOldestIndx;	// Oldest Data Point Index
	int		iDataType;		// Tick, Bar, Study
	int		iNumFields;		// Number of fields in the data - Studies data does not have a consistent format or number of fields
	ATDM_TIME stSplitDate[ATDM_MAX_SPLITS];
	double dSplitFactor[ATDM_MAX_SPLITS];
} ATDM_DLVR_DATA;

typedef struct _ATDM_DLVR_NEW_MEM  // The current allocated memory is full, so switch to this new memory space
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	char	szTimeName[128];
	char	szDataName[128];
	int		iMostRecIndx;	// Most Recent Data Point Index
	int		iOldestIndx;	// Oldest Data Point Index
	int		iDataType;		// Tick, Bar, Study
	int		iNumFields;		// Number of fields in the data
	int		iPrevMostRecIndx; // New Memory Space's Current Index of the Previous Most Recent Index from the Old Memory Space
	int		iPrevOldestIndx;  // New Memory Space's Current Index of the Previous Oldest Index from the Old Memory Space
} ATDM_DLVR_NEW_MEM;

typedef struct _ATDM_DLVR_REC_DATA  // deliver more recent data
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	int		iMostRecIndx;
} ATDM_DLVR_REC_DATA;

typedef struct _ATDM_DLVR_OLD_DATA  // deliver older data
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	int		iOldestIndx;
} ATDM_DLVR_OLD_DATA;

typedef struct _ATDM_DLVR_INDEX_UPDATE  // deliver updated first and last index
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	int		iFirstIndx;
	int		iLastIndx;
} ATDM_DLVR_INDEX_UPDATE;

#define ATDM_ERROR_NO_DATA 1
#define ATDM_ERROR_PARTIAL_DATA 2
#define ATDM_ERROR_COMPLETE 3

typedef struct _ATDM_DLVR_ERROR  // deliver Error Message
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	int		iErrorId;
	int		iDataSetStatus;  // Partial, Complete, None
} ATDM_DLVR_ERROR;

typedef struct _ATDM_DLVR_PB_INFO  // deliver Price Block Information
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	double dLast;
	double dHigh;
	double dLow;
	double dOpen;
	double dVolume;
	double dOpenInt;
	double dNetChange;
	DWORD dwPBFlags;	// result of call to GetPriceBlockFlags() -> News, Alerts, Split, Dividend
} ATDM_DLVR_PB_INFO;

typedef struct _ATDM_DLVR_HEARTBEAT
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
} ATDM_DLVR_HEARTBEAT;

typedef struct _ATDM_DLVR_REFRESH
{
	DWORD	dwMsgType;
	DWORD	dwMsgBytes;
	DWORD	dwMsgStatus;
	DWORD	dwMsgID;
	int		iDMDataID;
} ATDM_DLVR_REFRESH;

// *** TCP Message Unions

typedef union _ATDM_REQ_MSG
{
	ATDM_HEADER				header;
	ATDM_REQ_SYM_DATA		req_sym;
	ATDM_REQ_STUDY_DATA		req_study;
	ATDM_UNREQ_DATA			unreq_data;
	ATDM_UNREQ_ALL_DATA		unreq_all;
	ATDM_REQ_MORE_DATA		req_more;
	ATDM_SUSPECT_DP			suspect_dp;
	ATDM_USING_NEW_MEM		use_new_mem;
	ATDM_CLOSE_SOCKET		close_socket;
	ATDM_CORP_ACTION		corp_action;
	ATDM_SHOW_DM			show_dm;
} ATDM_REQ_MSG;

typedef union _ATDM_DLVR_MSG
{
	ATDM_HEADER				header;
	ATDM_DLVR_DATA			dlvr_data;
	ATDM_DLVR_REC_DATA		rec_data;
	ATDM_DLVR_OLD_DATA		old_data;
	ATDM_DLVR_NEW_MEM		new_mem;
	ATDM_DLVR_ERROR			error_msg;
	ATDM_DLVR_PB_INFO		pb_info;
	ATDM_DLVR_HEARTBEAT		heartbeat;
	ATDM_DLVR_INDEX_UPDATE	index_update;
	ATDM_DLVR_REFRESH		refresh;
} ATDM_DLVR_MSG;

// AT Data Manager Error Codes

// Programing or Logic Error
#define IDS_UNKNOWN_ERROR               1

// Data Not Available
#define IDS_NO_MORE_DATA                100
#define IDS_NO_DATA_RETRIEVED           110

// Invalid Request Parameters
#define IDS_INVALID_QSN					200
#define IDS_NULL_PB                     210
#define IDS_INVALID_TIME_PERIOD         220
#define IDS_INVALID_STREAM_SOCKET       230
#define IDS_INVALID_DATA_SET            240
#define IDS_INVALID_CHT_ENT_TOS			250
#define IDS_INVALID_REQ_DATES			260

// Data Manager Limits
#define IDS_TOO_MANY_DATA_SETS          310
#define IDS_TOO_MANY_USERS              320
#define IDS_TOO_MANY_CON                330

// AT Server Errors
#define IDS_CREATE_TOS_TOO_MANY         400
#define IDS_CREATE_TPB_TOO_MANY         410
#define IDS_CONN_LOSS					420
#define IDS_CONN_HEARTBEAT				430
#define IDS_CONN_READVISE				440
#define IDS_TRADING_HALTED				450

// TCP Errors
#define IDS_TCP_READ_ERROR              500
#define IDS_TCP_CONN_LOSS               510

// Machine Errors
#define IDS_MEM_ALLOC_ERR               600

// Study Errors
#define IDS_CAN_NOT_LOAD_TSL			700  // Can't load Study DLL
#define IDS_INVALID_TSL					710  // Can't load at least one of Study functions (I/O Req, Info, Build ...)

#endif

// ************ End Data Manager TCP Messages
