#include <pcqdefs.h>
#include <mguide.h>        
#include <pcqt1111.h>
#include <udjndv.h>	
#include <dbacorx.h>
#include <masterfundamental.h>		 
#include <stinformation.h>		 
#include <crossreference.h>
#include <statistics.h>

#ifndef INCL_DBAX
     #define INCL_DBAX

#ifdef __cplusplus				// C++ compiler is being used
	#ifndef __unix //unix sdk requires c++
		extern "C" {
	#endif
#endif

#ifndef INCL_NOPACKING
	#pragma pack(2)						 /* Word aligned. */
#endif

#define DBAX_FUNCTION_MASK					0x00ff
#define DBAX_DBN_ALIAS						0x8000
#define DBAX_DBN_MASK						0x00ff


#define DBAX_LOCK 							0x8000
#define DBAX_REMOTE 						0x4000
#define DBAX_INTEREST						0x2000
#define DBAX_MIRROR				   		 	0x1000
#define DBAX_INFINITE_WAIT					0xFFFFFFFF

#define DBA_WORKDIR_LEN						256

#define DBA_ATTACH							0
#define DBA_GET_EQUAL						1
#define DBA_GET_APPROX						2
#define DBA_GET_FIRST						3
#define DBA_GET_NEXT 						4
#define DBA_CHECK_POINT 					5
#define DBA_GET_CHANGED 					6
#define DBA_DETACH							7
#define DBA_ADD_KEY							8
#define DBA_DELETE_KEY						9
#define DBA_ATTACH_RESTORE					10
#define DBA_GET_HIGHER						11
#define DBA_GET_NEXT_SUB					12
#define DBA_WRITE_RECORD					13
#define DBA_STOP							14
#define DBA_GET_VERSION 					15
#define DBA_GET_DBHEADER					16
#define DBA_GET_DIRECT						17
#define DBA_GET_NEXT_DIRECT					18
#define DBA_GET_PREV_DIRECT					19
#define DBA_GET_PREV 						20
#define DBA_GET_EXTENDED_ERROR				21
#define DBA_CLEAR_ALL_LOCKS					22
#define DBA_CREATE_DB						23
#define DBA_UNLOCK_RECORD					24
#define DBA_DELETE_ALL_KEYS					25
#define DBA_DELETE_FILE						25	// delete file for type DBA_FILE
#define DBA_GET_LAST 						26
#define DBA_GET_USER_DIRECTORY				27
#define DBA_GET_DATA_DIRECTORY				28
#define DBA_GET_GREATER_OR_EQUAL			29
#define DBA_GET_LESS_OR_EQUAL				30
#define DBA_GET_FIRST_TS					31
#define DBA_GET_LAST_TS						32
#define DBA_GET_NEXT_TS						33
#define DBA_GET_PREV_TS						34
#define DBA_ADD_TS							35
#define DBA_DELETE_TS						36
#define DBA_INSERT_TS						37
#define DBA_LOGIN							38
#define DBA_LOGOUT							39
#define DBA_GET_LOCAL_LOGIN					40
#define DBA_ADD_INTEREST					41
#define DBA_DEC_INTEREST					42
#define DBA_DELETE_INTEREST					43
#define DBA_GET_EQUAL_TS					44
#define DBA_GET_APPROX_TS					45
#define DBA_GET_NEXT_STREAM					46
#define DBA_GET_PREV_STREAM					47
#define DBA_GET_NEXT_SUB_STREAM				48
#define DBA_GET_NEXT_TS_STREAM				49
#define DBA_GET_PREV_TS_STREAM				50
#define DBA_GET_NEXT_AND_SUB_STREAM			51
#define DBA_TERMINATE_STREAM_FUNCTION		52
#define DBA_WAIT_FOR_STREAM_TERMINATE		53
#define DBA_GET_LOCAL_ETI_CONN				54
#define DBA_STREAM_ACK						55
#define DBA_WRITE_TS						56
#define DBA_LOCK_DB							57
#define DBA_UNLOCK_DB						58
#define DBA_GET_LOWER						59
#define DBA_RESET_RAW_MODE					60
#define DBA_SET_RAW_MODE					61
#define DBA_GET_RAW_MODE					62			
#define DBA_GET_BARS						63
#define DBA_GET_OPTIONS						64
#define DBA_GET_NAME						65
#define DBA_MULTI_GET_EQUAL					66
#define DBA_MULTI_DEC_INTEREST				67
#define DBA_MULTI_DELETE_INTEREST			68
#define DBA_GET_DEPTH						69
#define DBA_GET_SERVER_NAME					70
#define DBA_CLEAR_LOCK						71      // Clear record lock
#define DBA_GET_PBLK_MASK					72      // Get the PBLK mask field
#define DBA_GET_FIRST_TS_BLK  				73
#define DBA_GET_LAST_TS_BLK					74
#define DBA_GET_NEXT_TS_BLK					75
#define DBA_GET_PREV_TS_BLK					76
#define DBA_CANCEL_TS						77      //  cancels trade and rolls forward new values
#define DBA_CORRECT_TS						78      //  modoifies trad and rolls forward new values
#define DBA_GET_REMOTE_STATUS				79      //  Get Remote Status flag and return it as return code
#define DBA_GET_FILE_INFORMATION			80	// return file information for type DBA_FILE
#define DBA_MAX_FUNCTION					DBA_GET_FILE_INFORMATION

#define DBA_START							100
#define DBA_CHECK_POINT_DB					101
#define DBA_SET_DATA_DIRECTORY				102
#define DBA_GET_AUTOSAVE					103
#define DBA_SET_AUTOSAVE					104
#define DBA_GET_PROCESS_COUNT				105
#define DBA_GET_REDIRECTOR_TABLE			106
#define DBA_USE_DB							107
#define DBA_LOAD_DB							108
#define DBA_GET_DB_NUM						109
#define DBA_GET_POSITION					110
#define DBA_GET_LOGICAL_RECORD_LEN			111
#define DBA_SET_DATAFILE_DIRECTORY			112
#define DBA_GET_DATAFILE_DIRECTORY			113
#define DBA_GET_KEY_LEN						114
#define DBA_GET_DB_TYPE						115
#define DBA_RECOVER_BTRIEVE_FILE			116
#define DBA_SET_DBA_TIMEOUT					117
#define DBA_GET_DBA_TIMEOUT					118
#define DBA_FLAT_FILE_LOCK					119
#define DBA_FLAT_FILE_UNLOCK				120
#define DBA_SET_BTRPARMS					121
#define DBA_GET_BTRPARMS					122
#define DBA_SET_HIMEMORY_PAGES				123 // Obsolete OS/2 function
#define DBA_GET_HIMEMORY_PAGES				124	// Obsolete OS/2 function
#define DBA_SET_HIMEMORY_START				125	// Obsolete OS/2 function
#define DBA_GET_HIMEMORY_START				126	// Obsolete OS/2 function		
#define DBA_GET_HIMEMORY_NEXT_AVAIL			127	// Obsolete OS/2 function
#define DBA_GET_EQUAL_HASH					128	// Obsolete OS/2 function
#define DBA_GET_EQUAL_NO_HASH				129	// Obsolete OS/2 function
#define DBA_UPDATE_HASH_TABLE				130	// Obsolete OS/2 function	
#define DBA_INSERT_ASYNC_ENABLE				131	// Obsolete OS/2 function
#define DBA_INSERT_ASYNC_DISABLE			132	// Obsolete OS/2 function	
#define DBA_INSERT_ASYNC_PAUSE				133	// Obsolete OS/2 function
#define DBA_INSERT_ASYNC_RESUME				134	// Obsolete OS/2 function	
#define DBA_INSERT_PRIORITY_CLASS			135	// Obsolete OS/2 function
#define DBA_INSERT_PRIORITY_DELTA			136	// Obsolete OS/2 function
#define DBA_INSERT_ACTION_WAIT				137	// Obsolete OS/2 function
#define DBA_INSERT_ACTION_COUNT				138	// Obsolete OS/2 function
#define DBA_SET_DEBUG						139	
#define DBA_GET_DEBUG						140	
#define DBA_SHARE							141
#define DBA_GET_HIMEMORY_SEL_PTR			142	// Obsolete OS/2 function
#define DBA_UPD_REC_COUNT					143	// ( for btrieve files )
#define DBA_GET_DB_NUMBER					144 // return data base number base one as a status code
#define DBA_GET_SAVED_KEY					145	// return pblk saved key

#define DBA_KEY_0	0
#define DBA_KEY_1	1
#define DBA_KEY_2	2
#define DBA_KEY_3	3
#define DBA_KEY_4	4
#define DBA_KEY_5	5
/*
   Database Tables
*/

#define DBA_STANDARD 						1
#define DBA_USER_PRICE 						2
#define DBA_EXTENDED 						3
#define DBA_TOPTEN							4
#define DBA_MARKET_MAKER					5
#define DBA_NEWSFLAGS						6
#define DBA_PRICE_TS						7
#define DBA_SESSION							8
#define DBA_HISTORY							9
#define DBA_BAR								10
#define DBA_TOS								11
#define DBA_SPLITS							12
#define DBA_FILTER							13
#define DBA_BREAKOUT						14
#define DBA_BREAKIN							15
#define DBA_GAPUP							16
#define DBA_GAPDOWN							17
#define DBA_LONG							18		
#define DBA_SHORT							19
#define DBA_EXPLODEUP						20
#define DBA_EXPLODEDOWN						21
#define DBA_ALERT_HIGHS						22
#define DBA_ALERT_LOWS						23
#define DBA_NEW_HIGHS						24
#define DBA_NEW_LOWS						25
#define DBA_PRICE_CORX						26
#define DBA_LOG								27
#define DBA_INFORMATION						28
#define DBA_RESERVED29						29
#define DBA_CROSS_REFERENCE                 30
#define DBA_RESERVED31						31
#define DBA_CONFIGURATION					32
#define DBA_RESERVED33						33
#define DBA_RESERVED34						34
#define DBA_RESERVED35						35
#define DBA_EXTENDED_DELETED				36
#define DBA_MMAKER_DELETED					37
#define DBA_BRIDGE_FUTURES					38
#define DBA_PRICEROLL						39
#define DBA_ERRORLOG						40
#define DBA_INDEX_CONSTITUENTS				41
#define DBA_INDICES							42
#define DBA_STATISTICS						43
#define DBA_EXCHTOPTEN						44
#define DBA_TRANSLATE_KEY					45
#define DBA_COUNTRY							46
#define DBA_EXCHANGE						47
#define DBA_PRIMARY_EXCH					48

#define DBA_NEWS							49
#define DBA_OPTION							50
#define DBA_FUNDAMENTAL 					51
#define DBA_SECURITY_PROFILE				52
#define DBA_EXDIVSPLIT						53
#define DBA_OPTION_INTRADAY					54
#define DBA_1400							55
#define DBA_FUTURE							56
#define DBA_REFERENCE						57
#define DBA_MARKETGUIDE 					58

#define DBA_RESERVED59						59
#define DBA_RESERVED60						60
#define DBA_RESERVED61						61
#define DBA_RESERVED62						62
#define DBA_RESERVED62						62
#define DBA_RESERVED63						63
#define DBA_RESERVED64						64
#define DBA_RESERVED65						65
#define DBA_RESERVED66						66
#define DBA_RESERVED67						67
#define DBA_MASTER_FUTURE					68
#define DBA_FIRSTCALLEARN					69
#define DBA_RESERVED70						70
#define DBA_MASTER_OPTION 					71
#define DBA_SECURITY_PROFILE2				72
#define DBA_ORDER							73
#define DBA_PRICE_VOLUME					74
#define DBA_DEPTH							75
#define DBA_MASTER_FUNDAMENTAL				76

#define DBA_RESERVED77						77
#define DBA_RESERVED78						78

#define DBA_RESERVED79						79
#define DBA_RESERVED80						80
#define DBA_RESERVED81						81
#define DBA_RESERVED82						82
#define DBA_RESERVED83						83
#define DBA_RESERVED84						84
#define DBA_RESERVED85						85
#define DBA_RESERVED86						86
#define DBA_RESERVED87						87
#define DBA_RESERVED88						88
#define DBA_RESERVED89						89
#define DBA_RESERVED90						90
#define DBA_RESERVED91						91
#define DBA_RESERVED92						92
#define DBA_RESERVED93						93
#define DBA_RESERVED94						94
#define DBA_RESERVED95						95
#define DBA_RESERVED96						96
#define DBA_RESERVED97						97
#define DBA_RESERVED98						98
#define DBA_RESERVED99						99

#define DBA_DV_CMINDEX						100
#define DBA_DV_CMTEXT						101
#define DBA_DV_BTINDEX						102
#define DBA_DV_BTTEXT						103
#define DBA_DV_PIINDEX						104
#define DBA_DV_PITEXT						105
#define DBA_DV_PRINDEX						106
#define DBA_DV_PRTEXT						107
#define DBA_DV_DIINDEX						108
#define DBA_DV_DITEXT						109
#define DBA_DV_FFINDEX						110
#define DBA_DV_FFTEXT						111
#define DBA_DV_PDINDEX						112
#define DBA_DV_PDTEXT						113
#define DBA_DV_WJINDEX						114
#define DBA_DV_WJTEXT						115
#define DBA_DV_FXINDEX						116
#define DBA_DV_FXTEXT						117
#define DBA_DV_SYINDEX						118
#define DBA_DV_SYTEXT						119
#define DBA_CX_CXINDEX						120
#define DBA_CX_CXTEXT						121
#define DBA_CX_FUINDEX						122
#define DBA_CX_FUTEXT						123
#define DBA_TR_NLINDEX						124		// Timed Release News Letters
#define DBA_TR_NLTEXT						125		// Timed Release News Letters
#define DBA_EX_EXINDEX						126		// Extel News
#define DBA_EX_EXTEXT						127		// Extel News
#define DBA_DV_OLINDEX						128      // Dow Jones Online
#define DBA_DV_OLTEXT						129		// Dow Jones Online
#define DBA_UP_UPINDEX						130      // UPI News
#define DBA_UP_UPTEXT						131		// UPI News
#define DBA_KR_KRINDEX						132      // Knight Ridder News
#define DBA_KR_KRTEXT						133		// Knight Ridder News
#define DBA_IB_IBINDEX						134		// IBES News
#define DBA_IB_IBTEXT						135		// IBES News
#define DBA_CN_FNINDEX						136		// CNNfn News
#define DBA_CN_FNTEXT						137     // CNNfn News
#define DBA_ZK_ZAINDEX						138		// Zacks Alerts
#define DBA_ZK_ZATEXT						139		// Zacks Alerts
#define DBA_MARKET_QUOTE					140
#define DBA_RESERVED201						201

#define DBA_REDIRECTOR						1000
#define DBA_PBLKTABLE						1001
#define DBA_PIDTABLE 						1002

/*
  DBAX Error Codes
*/

#define DBA_ERR_NO_ERROR				0
#define DBA_ERR_INV_FUNC_CODE			1
#define DBA_ERR_INV_DB_NUMBER			2
#define DBA_ERR_DB_NOT_LOADED			3
#define DBA_ERR_INIT					3
#define DBA_ERR_KEY_NOT_FOUND			4
#define DBA_ERR_INV_KEY					5
#define DBA_ERR_EOF						6
#define DBA_ERR_PCQCOM_RESERVED			7
#define DBA_ERR_UNDEFINED				7
#define DBA_ERR_TIMEOUT			        8
#define DBA_ERR_NO_MORE_CHG_REC			9
#define DBA_ERR_RESERVED_B				10
#define DBA_ERR_SAVE					11
#define DBA_ERR_DB_LOCKED				12
#define DBA_ERR_KEY_ALREADY_EXISTS		13
#define DBA_ERR_DB_FULL					14
#define DBA_ERR_RESERVED_C				15
#define DBA_ERR_NO_ATTACH				16
#define DBA_ERR_RECORD_CHANGED			17
#define DBA_ERR_REC_LOCKED 				18
#define DBA_ERR_MEMORY_ERROR			19
#define DBA_ERR_BAD_ENVIRONMENT			20
#define DBA_ERR_OUT_OF_HANDLES			21
#define DBA_ERR_BAD_PBLK				22
#define DBA_ERR_INV_KEY_LENGTH			23
#define DBA_ERR_INV_DATA_LENGTH			24
#define DBA_ERR_FILE_ERROR 				25
#define DBA_ERR_BUSY			 		26
#define DBA_ERR_PARTIAL		 			27
#define DBA_ERR_PASSWORD	 			28
#define DBA_ERR_POSITIONING				29
#define DBA_ERR_DUPLICATE_LOGIN			30
#define DBA_ERR_NO_LOGIN				31
#define DBA_ERR_SERVER_MAX				32
#define DBA_ERR_INVALID_LOGIN			33
#define DBA_ERR_INTEREST				34
#define DBA_ERR_INVALID_PASSWORD		35
#define DBA_ERR_SET_PASSWORD			36
#define DBA_ERR_INVALID_SERVER			37
#define DBA_ERR_NOT_LOCKED				38
#define DBA_ERR_ZERO_POSITION			39
#define DBA_ERR_FILE_IO					40
#define DBA_ERR_TS_FULL					41
#define DBA_ERR_TS_NOT_FOUND			42
#define DBA_ERR_TS_BAD_CHAIN			43
#define DBA_ERR_DELETE_FILE				44
#define DBA_ERR_WRITE_TIMEOUT			45
#define DBA_ERR_READ_TIMEOUT			46
#define DBA_ERR_CONNECT_TIMEOUT			47
#define DBA_ERR_TRANSACT_TIMEOUT		48
#define DBA_ERR_BUFFER_SIZE				49
#define DBA_ERR_CLOSED					50

#define DBA_ERR_DB_BLOCKED				51    
#define DBA_ERR_DB_IN_USE				52    
#define DBA_ERR_DB_NOT_AVAILABLE		53    
#define DBA_ERR_DB_NOT_LOCKED			54
#define DBA_ERR_NOT_CURRENT_RECORD		55
#define DBA_ERR_INV_KEY_NUM				56
#define DBA_ERR_INV_KEY_VALUE			57
	
#define DBA_MAX_ERROR					DBA_ERR_INV_KEY_VALUE

/* Additional Dbai Error Codes  */

#define DBA_ERR_CREATE_REDIRECTOR		1000
#define DBA_ERR_CREATE_PBLKTABLE		1001
#define DBA_ERR_CREATE_SHAREMEM			1002
#define DBA_ERR_STARTUP_DAEMON			1003
#define DBA_ERR_STARTUP_DAEMON_ERROR    1004
#define DBA_ERR_CREATE_BTRV				1005
#define DBA_ERR_INVALID_PARAMETER		1006
#define DBA_ERR_ACCESS_DENIED			1007
#define DBA_ERR_DB_ALREADY_EXISTS		1008
#define DBA_ERR_DUPLICATE_DB_NUMBER     1009
#define DBA_ERR_REDIRECTOR_FULL			1010
#define DBA_ERR_ALLOCATION				1011
#define DBA_ERR_INVALID_DBIO_MODULE     1012
#define DBA_ERR_INVALID_DBIO_FUNC		1013
#define DBA_ERR_INVALID_DBA_TYPE		1014
#define DBA_ERR_PHYSICAL_LEN_ZERO		1015
#define DBA_ERR_INV_DB_NUM				1016
#define DBA_ERR_UNKNOWN_DB_TYPE			1017
#define DBA_ERR_DB_ALREADY_STARTED		1018
#define DBA_ERR_NETLINK					1019
#define DBA_ERR_SERVER_NOT_AVAILABLE    1020
#define DBA_ERR_FATAL_SERVER_ERROR		1021
#define DBA_ERR_OUT_OF_MEMORY			1022
#define DBA_ERR_SERVER_RESTART			1023

#define DBA_ERR_SERVER_BUSY				1024
#define DBA_ERR_SERVER_DISCONNECT		1025
#define DBA_ERR_NET_REQ_NOT_ACCEPTED    1026
#define DBA_ERR_SERVER_NOT_CONNECTED    1027
#define DBA_ERR_SERVER_BAD_PIPE			1028
#define DBA_ERR_SERVER_RESPONSE_TOO_BIG	1029

#define DBA_ERR_LOAD_ERROR				1030
#define DBA_ERR_PIDTABLE_FULL			1031
#define DBA_ERR_FILE_RENAME				1032

#define DBA_ERR_FILE_NOT_FOUND			1033
#define DBA_ERR_INSUFFICIENT_DISK_SPACE	1044
#define DBA_ERR_HIMEM_DRIVER			1045
#define DBA_ERR_HIMEM_IN_USE			1046
#define DBA_ERR_HASH_COLLISION			1047
#define DBA_ERR_SEMAPHORE				1048
#define DBA_ERR_PBLKTABLE_FULL			1049
#define DBA_ERR_BAD_VERSION				1050
#define DBA_ERR_BAD_CHECKSUM			1051
#define DBA_ERR_BAD_PACKETSIZE			1052
#define DBA_ERR_BAD_BYTEORDER			1053
#define DBA_ERR_BAD_PACKET				1054
#define DBA_ERR_PIDTABLE_ACCESS			1055
#define DBA_ERR_PBLKTABLE_ACCESS		1056
#define DBA_ERR_REDIRTABLE_ACCESS		1057
#define DBA_ERR_TERMINATE_STREAM		1058
#define DBA_ERR_LOCKOUT					1059
#define DBA_ERR_SERVER_UNAVAILABLE      1060
#define DBA_ERR_DELETE_FAILED           1061
#define DBA_ERR_INCORRECT_VERSION       1062
#define DBA_ERR_REDIRECTOR_LOCKED       1063
#define DBA_ERR_FATAL_SOCKET_ERROR      1064

#ifndef __INCL_DBA_NAMES
#define __INCL_DBA_NAMES

#define DBAPIPENAME					"\\PIPE\\PCQDBA\\DBAX.PIP"

#define DBA_DLLNAME					"DBADLL"
#define DBA_DLL32NAME				"DBA32DLL"

#define DBANAME_REDIRECTOR 			"Redirector"
#define DBANAME_PBLKTABLE			"Pblk Table"
#define DBANAME_PIDTABLE			"PID Table"

//===========================================================================//
//                                                                                                                                                                                                //
//                                               DBS Database Record Definitions                                                          //
//                                                                                                                                                                                                //
//---------------------------------------------------------------------------//

#define DBADBIO_DLL								"dbio"
#define DBADBIO32_DLL                           "dbio32"


#define DBANAME_USER							"User Price"
#define DBANAME_EXTENDED                        "Price"
#define DBANAME_TOPTEN                          "Top Ten"
#define DBANAME_MMAKER                          "Market Maker"
#define DBANAME_NEWSFLAGS                       "News Flags"
#define DBANAME_PRICE_TS                        "Time Series Price"
#define DBANAME_PRICE_CORX                      "Price Corrections"
#define DBANAME_LOG								"Log"
#define DBANAME_HISTORY_PRICE_TS				"History Time Series"
#define DBANAME_SESSION							"Session"
#define DBANAME_HISTORY							"History"
#define DBANAME_BAR								"Bar Data"		
#define DBANAME_TOS								"Price Time Series"
#define DBANAME_SPLITS							"Splits"
#define DBANAME_FILTER							"Filter"
#define DBANAME_BREAKOUT						"Break Out"
#define DBANAME_BREAKIN							"Break In"
#define DBANAME_GAPUP							"Gap Up"
#define DBANAME_GAPDOWN							"Gap Down"
#define DBANAME_LONG							"Long"
#define DBANAME_SHORT							"Short"
#define DBANAME_EXPLODEUP						"Explode Up"
#define DBANAME_EXPLODEDOWN						"Explode Down"
#define DBANAME_ALERT_HIGHS						"Alert Highs"
#define DBANAME_ALERT_LOWS						"Alert Lows"
#define DBANAME_NEW_HIGHS						"New Highs"
#define DBANAME_NEW_LOWS						"New Lows"
#define DBANAME_ORDER							"Order"
#define DBANAME_DEPTH							"Depth"
#define DBANAME_INDEX_CONSTITUENTS				"Index Constituents"
#define DBANAME_INDICES							"Indices"
#define DBANAME_STATISTICS						"Statistics"
#define DBANAME_SECURITY_PROFILE 				"Security Profile"
#define DBANAME_SECURITY_PROFILE2               "Security Profile 2"
#define DBANAME_FUNDAMENTAL                     "Stock Fundamental"
#define DBANAME_OPTIONS                         "Option Fundamental"
#define DBANAME_MASTER_OPTION                   "Master Option"
#define DBANAME_MASTER_FUTURE					"Master Future"
#define DBANAME_PRICE_VOLUME					"Price Volume" 
#define DBANAME_COUNTRY                         "Country"
#define DBANAME_EXCHANGE                        "Exchange"
#define DBANAME_PRIMARY_EXCH                    "Primary Exchange"
#define DBANAME_TRANSLATE_KEY                   "Translate Key"
#define DBANAME_PRICEROLL                       "Price Roll"
#define DBANAME_BRIDGE_FUTURES                  "Bridge Future Keys"
#define DBANAME_EXTENDED_DELETED                "Del Keys-Price"
#define DBANAME_MMAKER_DELETED                  "Del Keys-Mmaker"
#define DBANAME_FUNDAMENTAL9                    "Fundamental9"
#define DBANAME_FUNDAMENTAL99                   "Fundamental9"
#define DBANAME_UNDERLYING                      "Underlying Data"
#define DBANAME_1021DATA                        "Pcqt1021"
#define DBANAME_USFUNDAMENT                     "US Fundamental"
#define DBANAME_INTLFUNDAMENT                   "Intl Fundamental"
#define DBANAME_NEWS                            "News"
#define DBANAME_1400                            "Cusip"
#define DBANAME_FUTURES                         "Futures Options"
#define DBANAME_OPTION_INTRADAY                 "Options Intraday"
#define DBANAME_REFERENCE						"Reference"
#define DBANAME_MARKETGUIDE						"Market Guide"
#define DBANAME_FIRSTCALLEARN					"First Call Earnings"
#define DBANAME_MASTER_FUNDAMENTAL				"Master Fundamental"
#define DBANAME_INFORMATION						"Information"
#define DBANAME_CROSS_REFERENCE					"Cross Reference"
#define DBANAME_CONFIGURATION					"Configuration Data"

#define DBANAME_DV_CMINDEX						"Cap Markets Index"
#define DBANAME_DV_CMTEXT 						"Cap Markets Text"
#define DBANAME_DV_BTINDEX                      "Broad Tape Index" 
#define DBANAME_DV_BTTEXT                       "Broad Tape Text"
#define DBANAME_DV_PIINDEX                      "Prof Investor Index" 
#define DBANAME_DV_PITEXT                       "Prof Investor Text"
#define DBANAME_DV_PRINDEX                      "Press Release Index" 
#define DBANAME_DV_PRTEXT                       "Press Release Text"
#define DBANAME_DV_DIINDEX                      "Dow Intl Wire Index"
#define DBANAME_DV_DITEXT                       "Dow Intl Wire Text"
#define DBANAME_DV_FFINDEX                      "Fed Filing Index"
#define DBANAME_DV_FFTEXT                       "Fed Filing Text"
#define DBANAME_DV_PDINDEX                      "Delayed Prof Index"
#define DBANAME_DV_PDTEXT                       "Delayed Prof Text"
#define DBANAME_DV_WJINDEX                      "Wall Street Index"
#define DBANAME_DV_WJTEXT                       "Wall Street Text"
#define DBANAME_DV_FXINDEX                      "Fed 10 K Index"
#define DBANAME_DV_FXTEXT                       "Fed 10 K Text"
#define DBANAME_DV_SYINDEX                      "Dow Symbol Index"
#define DBANAME_DV_SYTEXT                       "Dow Symbol Text"
#define DBANAME_CX_CXINDEX                      "Comtex Index"
#define DBANAME_CX_CXTEXT                       "Comtex Text"
#define DBANAME_CX_FUINDEX                      "Future World Index"
#define DBANAME_CX_FUTEXT                       "Future World Text"
#define DBANAME_TR_NLINDEX                      "News Letter Index"
#define DBANAME_TR_NLTEXT                       "News Letter Text"
#define DBANAME_EX_EXINDEX                      "Extel Index"
#define DBANAME_EX_EXTEXT                       "Extel Text"
#define DBANAME_DV_OLINDEX                      "Dow OnLine Index"
#define DBANAME_DV_OLTEXT                       "Dow OnLine Text"
#define DBANAME_UP_UPINDEX                      "UPI News Index"
#define DBANAME_UP_UPTEXT                       "UPI News Text"
#define DBANAME_KR_KRINDEX                      "KR News Index"
#define DBANAME_KR_KRTEXT                       "KR News Text"
#define DBANAME_IB_IBINDEX 						"IBES Index"
#define DBANAME_IB_IBTEXT						"IBES Text"
#define DBANAME_CN_FNINDEX						"CNNfn Index"
#define DBANAME_CN_FNTEXT						"CNNfn Text"
#define DBANAME_ZK_ZAINDEX						"Zacks Alert Index"
#define DBANAME_ZK_ZATEXT						"Zacks Alert Text"
#define DBANAME_MARKET_QUOTE					"Market Quote"

#define DBSNAME_SCHEDULER						"DBS Schedules"

#define DBAFILE_USER	                        "pcqt1012"
#define DBAFILE_EXTENDED						"pcqt1062"
#define DBAFILE_TOPTEN		                    "pcqt1042"
#define DBAFILE_MMAKER                          "pcqt1052"
#define DBAFILE_NEWSFLAGS  						"pcqt1072"
#define DBAFILE_PRICE_TS	  		            "pcqt1092"
#define DBAFILE_PRICE_CORX	  		            "pcqt1093"
#define DBAFILE_LOG								"HyperFeed.log"
#define DBAFILE_SESSION 						"pcqt1200"
#define DBAFILE_HISTORY 						"pcqt1210"
#define DBAFILE_BAR								"pcqt1220"
#define DBAFILE_TOS								"pcqt1230"
#define DBAFILE_SPLITS							"splits.dat"
#define DBAFILE_FILTER							"pcqt2213"
#define DBAFILE_BREAKOUT						"pcqt2214"
#define DBAFILE_BREAKIN							"pcqt2215"
#define DBAFILE_GAPUP							"pcqt2216"
#define DBAFILE_GAPDOWN							"pcqt2217"
#define DBAFILE_LONG							"pcqt2218"
#define DBAFILE_SHORT							"pcqt2219"
#define DBAFILE_EXPLODEUP						"pcqt2220"
#define DBAFILE_EXPLODEDOWN						"pcqt2221"
#define DBAFILE_ALERT_HIGHS						"Pcqt2222"
#define DBAFILE_ALERT_LOWS						"Pcqt2223"
#define DBAFILE_NEW_HIGHS						"Pcqt2224"
#define DBAFILE_NEW_LOWS						"Pcqt2225"

#define DBAFILE_SECURITY_PROFILE				"pcqt1023"
#define DBAFILE_USFUNDAMENT                     "pcqt1021"
#define DBAFILE_INTLFUNDAMENT                   "pcqt1023"
#define DBAFILE_UNDERLYING                      "pcqt1024"
#define DBAFILE_SECURITY_PROFILE2				"pcqt1025"
#define DBAFILE_INDEX_CONSTITUENTS				"pcqt4041"
#define DBAFILE_INDICES							"pcqt4042"
#define DBAFILE_STATISTICS						"pcqt4043"
#define DBAFILE_COUNTRY                         "pcqt4046"
#define DBAFILE_EXCHANGE                        "pcqt4047"
#define DBAFILE_PRIMARY_EXCH                    "pcqt4048"
#define DBAFILE_TRANSLATE_KEY					"pcqt4045"
#define DBAFILE_ORDER							"Pcqt1073"
#define DBAFILE_1021DATA                        "pcqt1021"
//#define DBAFILE_NEWS							"pcqt1081"
#define DBAFILE_NEWS                            "pcqt1082"
#define DBAFILE_OPTIONS                         "pcqt1100"
#define DBAFILE_MASTER_OPTION                   "pcqt1111"
#define DBAFILE_MASTER_FUTURE					"pcqt3068"
#define DBAFILE_OPTION_INTRADAY                 "pcqt1150"
#define DBAFILE_FUTURES                         "pcqt1300"
#define DBAFILE_1400                            "pcqt1400"
#define DBAFILE_REFERENCE						"pcqt1600"
#define DBAFILE_MARKETGUIDE						"pcqt1700"
#define DBAFILE_FIRSTCALLEARN					"pcqt1800"
#define DBAFILE_MASTER_FUNDAMENTAL				"pcqt1076"
#define DBAFILE_INFORMATION						"pcqt1077"
#define DBAFILE_CROSS_REFERENCE					"pcqt2230"
#define DBAFILE_CONFIGURATION					"pcquote.ini"
#define DBAFILE_SOFTWARE_UPDATE					"STUpdate"

#define DBAFILE_DV_CMINDEX						"cmindex.dv"
#define DBAFILE_DV_CMTEXT 						"cmtext.dv"
#define DBAFILE_DV_BTINDEX                      "btindex.dv"
#define DBAFILE_DV_BTTEXT                       "bttext.dv" 
#define DBAFILE_DV_PIINDEX                      "piindex.dv"
#define DBAFILE_DV_PITEXT                       "pitext.dv" 
#define DBAFILE_DV_PRINDEX                      "prindex.dv"
#define DBAFILE_DV_PRTEXT                       "prtext.dv" 
#define DBAFILE_DV_DIINDEX                      "diindex.dv"
#define DBAFILE_DV_DITEXT                       "ditext.dv" 
#define DBAFILE_DV_FFINDEX                      "ffindex.dv"
#define DBAFILE_DV_FFTEXT                       "fftext.dv" 
#define DBAFILE_DV_PDINDEX                      "pdindex.dv"
#define DBAFILE_DV_PDTEXT                       "pdtext.dv" 
#define DBAFILE_DV_WJINDEX                      "wjindex.dv"
#define DBAFILE_DV_WJTEXT                       "wjtext.dv" 
#define DBAFILE_DV_FXINDEX                      "fxindex.dv" 
#define DBAFILE_DV_FXTEXT                       "fxtext.dv"  
#define DBAFILE_DV_SYINDEX                      "syindex.dv" 
#define DBAFILE_DV_SYTEXT                       "sytext.dv"  
#define DBAFILE_CX_CXINDEX                      "cxindex.nd" 
#define DBAFILE_CX_CXTEXT                       "cxtext.nd"  
#define DBAFILE_CX_FUINDEX                      "fuindex.nd" 
#define DBAFILE_CX_FUTEXT                       "futext.nd"  
#define DBAFILE_TR_NLINDEX                      "nlindex.nd" 
#define DBAFILE_TR_NLTEXT                       "nltext.nd"  
#define DBAFILE_EX_EXINDEX                      "exindex.nd" 
#define DBAFILE_EX_EXTEXT                       "extext.nd"  
#define DBAFILE_DV_OLINDEX                      "onindex.dv" 
#define DBAFILE_DV_OLTEXT                       "ontext.dv"  
#define DBAFILE_UP_UPINDEX                      "upindex.nd" 
#define DBAFILE_UP_UPTEXT                       "uptext.nd"  
#define DBAFILE_KR_KRINDEX                      "krindex.nd" 
#define DBAFILE_KR_KRTEXT                       "krtext.nd"  
#define DBAFILE_IB_IBINDEX 						"ibindex.nd"
#define DBAFILE_IB_IBTEXT						"ibtext.nd"
#define DBAFILE_CN_FNINDEX						"cfindex.nd"
#define DBAFILE_CN_FNTEXT						"cftext.nd"
#define DBAFILE_ZK_ZAINDEX						"zaindex.nd"
#define DBAFILE_ZK_ZATEXT						"zatext.nd"
#define DBAFILE_MARKET_QUOTE					"MarketQuote.dat"

#define COMMENT_USER							"User Price Data Base"
#define COMMENT_EXTENDED                        "Open,High,Low,Last,Bid,Ask"
#define COMMENT_TOPTEN                          "Top ten Adv,Dec,Most Active"
#define COMMENT_MMAKER                          "Level 2 Quotations"
#define COMMENT_NEWSFLAGS                       "News Indicator Sources"
#define COMMENT_PRICE_TS                        "Time Series Price"
#define COMMENT_PRICE_CORX                      "Price Corrections"
#define COMMENT_LOG								"Logger Data Base"
#define COMMENT_HISTORY_PRICE_TS	            "History Time Series Price"
#define COMMENT_SESSION 						"Session"

#define COMMENT_HISTORY 						"Price History"
#define COMMENT_BAR								"Bar History"
#define COMMENT_TOS								"Price Time Series"
#define COMMENT_SPLITS							"Corporate Actions"
#define COMMENT_FILTER							"Filter Table"
#define COMMENT_BREAKOUT						"Break Out Filter Table"
#define COMMENT_BREAKIN							"Break In Filter Table"
#define COMMENT_GAPUP							"Gap Up Filter Table"
#define COMMENT_GAPDOWN							"Gap Down Filter Table"
#define COMMENT_LONG							"Long Filter Table"
#define COMMENT_SHORT							"Short Filter Table"
#define COMMENT_EXPLODEUP						"Explode Up Filter Table"
#define COMMENT_EXPLODEDOWN						"Explode Down Filter Table"
#define COMMENT_ALERT_HIGHS						"Daily High Alerts"
#define COMMENT_ALERT_LOWS						"Daily Low Alerts"
#define COMMENT_NEW_HIGHS						"New 52 Week Highs"
#define COMMENT_NEW_LOWS						"New 52 Week Lows"
#define COMMENT_INDEX_CONSTITUENTS				"Constituents of Indices"
#define COMMENT_INDICES							"Exchange Indices Database"
#define COMMENT_STATISTICS						"Exchange Stats Database"
#define COMMENT_OPTIONS 						"Options Fixed Data"
#define COMMENT_MASTER_OPTION					"Master Option Database"
#define COMMENT_MASTER_FUTURE					"Master Future Database"
#define COMMENT_SECURITY_PROFILE                 "Security Fixed Data"
#define COMMENT_PRICE_VOLUME					"Price Volume"
#define COMMENT_USFUNDAMENT                     "US Fundamental Data"
#define COMMENT_INTLFUNDAMENT                   "Intl Fundamental Data"
#define COMMENT_EXCHANGE                        "Exchange Processing Codes"
#define COMMENT_PRIMARY_EXCH                    "Primary exchange groups"
#define COMMENT_TRANSLATE_KEY					"Symbol Translation"
#define COMMENT_COUNTRY                         "Country codes"
#define COMMENT_ORDER							"Open Book Orders"
#define COMMENT_DEPTH							"Depth Of Book"
#define COMMENT_UNDERLYING                      "Company Data"
#define COMMENT_1021DATA                        "Pcqt1021 Data"
#define COMMENT_NEWS                            "News"
#define COMMENT_1400                            "Cusip Data Base"
#define COMMENT_FUTURES                         "Futures Options Fixed Data"
#define COMMENT_OPTION_INTRADAY ""
#define COMMENT_REFERENCE						"Symbol Reference"
#define COMMENT_MARKETGUIDE						"Market Guide"
#define COMMENT_FIRSTCALLEARN					"First Call Earnings"
#define COMMENT_MASTER_FUNDAMENTAL				"Master Fundamental"
#define COMMENT_INFORMATION						"Process Information"
#define COMMENT_CROSS_REFERENCE					"Cross Reference"
#define COMMENT_CONFIGURATION					"Configuration Data Base"

#define COMMENT_DV_CMINDEX                      "Capital Markets Index"       
#define COMMENT_DV_CMTEXT                       "Capital Markets Text"        
#define COMMENT_DV_BTINDEX                      "Broad Tape Index"            
#define COMMENT_DV_BTTEXT                       "Broad Tape Text"             
#define COMMENT_DV_PIINDEX                      "Professional Investor Index" 
#define COMMENT_DV_PITEXT                       "Professional Investor Text"  
#define COMMENT_DV_PRINDEX                      "Press Release Index"         
#define COMMENT_DV_PRTEXT                       "Press Release Text"          
#define COMMENT_DV_DIINDEX                      "Dow Intl News Index"        
#define COMMENT_DV_DITEXT                       "Dow Intl News Text"         
#define COMMENT_DV_FFINDEX                      "Federal Filing Index"        
#define COMMENT_DV_FFTEXT                       "Federal Filing Text"         
#define COMMENT_DV_PDINDEX                      "Delayed Prof Index"         
#define COMMENT_DV_PDTEXT                       "Delayed Prof Text"          
#define COMMENT_DV_WJINDEX                      "Wall Street Journal Index"   
#define COMMENT_DV_WJTEXT                       "Wall Street Journal Text"    
#define COMMENT_DV_FXINDEX                      "Dow 10 K Filings Index"        
#define COMMENT_DV_FXTEXT                       "Dow 10 K Filings Text"         
#define COMMENT_DV_SYINDEX                      "Dow Symbols Index"         
#define COMMENT_DV_SYTEXT                       "Dow Symbols Text"          
#define COMMENT_CX_CXINDEX                      "Comtex News Index"        
#define COMMENT_CX_CXTEXT                       "Comtex News Text"         
#define COMMENT_CX_FUINDEX                      "Future World News Index"         
#define COMMENT_CX_FUTEXT                       "Future World News Text"          
#define COMMENT_TR_NLINDEX                      "News Letter Index"         
#define COMMENT_TR_NLTEXT                       "News Letter Text"          
#define COMMENT_EX_EXINDEX                      "Extel Index"         
#define COMMENT_EX_EXTEXT                       "Extel Text"          
#define COMMENT_DV_OLINDEX                      "Dow OnLine Index"         
#define COMMENT_DV_OLTEXT                       "Dow OnLine Text"          
#define COMMENT_UP_UPINDEX                      "UPI News Index"
#define COMMENT_UP_UPTEXT                       "UPI News Text"
#define COMMENT_KR_KRINDEX                      "KR News Index"
#define COMMENT_KR_KRTEXT                       "KR News Text"
#define COMMENT_IB_IBINDEX 						"IBES Index"
#define COMMENT_IB_IBTEXT						"IBES Text"
#define COMMENT_CN_FNINDEX						"CNNfn Index"
#define COMMENT_CN_FNTEXT						"CNNfn Text"
#define COMMENT_ZK_ZAINDEX						"Zacks Alert Index"
#define COMMENT_ZK_ZATEXT						"Zacks Alert Text"
#define COMMENT_MARKET_QUOTE					"Market Quote"

#endif

//added for unix (for alignment and size limits) -ksr
  typedef LONG DBAX_PBLK[64];	
  typedef LONG ETIX_PBLK[64];	
/*
#ifndef DBA_KEY
typedef struct _DBA_KEY {
	USHORT	type;
	UCHAR 	symbol[12];
	UCHAR 	countryCode[2];
	UCHAR 	exchangeCode[2];
	UCHAR 	currencyCode[3];
	UCHAR 	reserved;
} DBA_KEY;

typedef struct _DBA_ITEM {
	USHORT	type;
	UCHAR 	symbol[12];
	UCHAR 	countryCode[2];
	UCHAR 	exchangeCode[2];
} DBA_ITEM;
#endif
*/
#include <pcqt1400.h>
#ifndef INCL_NOPACKING
	#pragma pack(2)
#endif

#include <pcqt1800.h>
#ifndef INCL_NOPACKING
	#pragma pack(2)
#endif
/*
typedef struct _DBA_MMKEY {
	USHORT		type;
	UCHAR 		symbol[12];
	UCHAR 		countryCode[2];
	UCHAR 		exchangeCode[2];
	UCHAR 		currencyCode[3];
	UCHAR 		mmid[4];
	UCHAR 		reserved;
} DBA_MMKEY;

typedef	union  _DBA_MAXKEY {
	DBA_KEY		dbaKey;
	DBA_MMKEY	mmKey;
} DBA_MAXKEY;
*/
typedef struct _DBA_KEY52_0 {
	DBA_KEY		item;							 /* unique key 							*/
} DBA_KEY52_0;

typedef struct _DBA_KEY52_1 {
	DBA_KEY		baseitem;						 /* allows duplicates					*/
} DBA_KEY52_1;

typedef struct _DBA_KEY52_2 {
	BYTE		name[36];
} DBA_KEY52_2;

typedef struct _DBA_KEY52_3 {
	BYTE		country[2];
	BYTE		name[36];
} DBA_KEY52_3;

typedef struct _DBA_KEY52_4 {
	BYTE		idNumber[12];
} DBA_KEY52_4;

typedef struct _DBA_VERSION {
	SHORT		major;
	SHORT		minor;
	CHAR		release[2];
	CHAR		serialNumber[16];
	CHAR		softwareVersion[8];
} DBA_VERSION;

typedef struct _DBA_USER {
	CHAR		buffer[250];
	SHORT		serverData;
	SHORT		data0;
	SHORT		data1;
} DBA_USER;

#ifndef __DBA_PRICE_DEFINED
#define __DBA_PRICE_DEFINED
typedef struct _DBA_PRICE {	
	LONG		price;
	ULONG		size;
	CHAR		exchangeCode[2];
	CHAR		hour;
	CHAR		minute;
} DBA_PRICE;
#endif


typedef struct _DBA_EXT_PRICE {	
	LONG		price;
	ULONG		size;
	CHAR		exchangeCode[2];
	ULONG		time;
	USHORT		priceType;
} DBA_EXT_PRICE;


typedef struct _PCQ_DATE {
   USHORT		year;
   BYTE			month;
   BYTE			day;
} PCQ_DATE;

typedef struct _PCQ_TIME {
	BYTE		hour;
	BYTE		minute;
	BYTE		second;
	BYTE		hundredth;
} PCQ_TIME;

typedef  struct  _TICK_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	unused		:2;
	BIT_FIELD	midClose	:2;
	BIT_FIELD	mid			:2;
	BIT_FIELD	ask			:2;
	BIT_FIELD	bid			:2;
	BIT_FIELD	close		:2;
	BIT_FIELD	open		:2;
	BIT_FIELD	lastSale	:2;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	lastSale	:2;
	BIT_FIELD	open		:2;
	BIT_FIELD	close		:2;
	BIT_FIELD	bid			:2;
	BIT_FIELD	ask			:2;
	BIT_FIELD	mid			:2;
	BIT_FIELD	midClose	:2;
	BIT_FIELD	unused		:2;
#endif
} TICK_BITS;

#define	NOTICK	0
#define UPTICK  1
#define	DNTICK	2

#define TICK_LASTSALE	0xc000
#define TICK_OPEN 		0x3000
#define TICK_CLOSE		0x0c00
#define TICK_BID		0x0300
#define TICK_ASK		0x00c0
#define TICK_MID		0x0030
#define TICK_MIDCLOSE	0x000c

#ifdef __sun			/* sun actually has a tick type already */
  typedef  union   _TICK_PCQ {
    TICK_BITS   tickBits;
    USHORT      tick;
} TICK_PCQ;
#else
  typedef  union   _TICK {
    TICK_BITS   tickBits;
    USHORT      tick;
} TICK;
#endif

#define DB3_DELETED				0x0001
#define DB3_SPREAD_TRADE		0x0002
#define DB3_BANKRUPTCY			0x0004
#define DB3_NON_FIRM_QUOTE 		0x0008
#define DB3_RESTRICTED			0x0010
#define DB3_OPEN_INDICATOR 		0x0020
#define DB3_NEWS_INDICATOR 		0x0040
#define DB3_FAST_MARKET 		0x0080
#define DB3_ROTATION_MARKET		0x0100
#define DB3_HALTED_MARKET		0x0200
#define DB3_STRADDLE_TRADE 		0x0400
#define DB3_AUTO_EXECUTE		0x0800
#define DB3_LOCKED_QUOTE		0x1000
#define DB3_CROSSED_QUOTE		0x2000
#define DB3_EXDIVIDEND			0x4000
//#define DB3_SETTLEMENT			0x4000	/* flags settlement if future */
#define DB3_SPLIT 				0x8000

#if defined (TOWER_INCL)
typedef struct  _DBA_BITS_3 {
    BIT_FIELD  deleted			:1;		/* soft delete					*/
    BIT_FIELD  correction		:1;		/* bridge correction recvd today*/
    BIT_FIELD  undefined0		:3;
    BIT_FIELD  openIndicator	:1;		/* Issue has opened today		*/
    BIT_FIELD  newsIndicator	:1;		/* News indicator				*/
    BIT_FIELD  spCondFlag1  	:2;		/* Fast, Rotation, Halt.		*/
    BIT_FIELD  spCondFlag2  	:2;		/* Spread, Straddle, Non-firm.	*/
    BIT_FIELD  autoExecute  	:1;		/* Auto execute					*/
    BIT_FIELD  spCondFlag3  	:2;		/* Locked, Crossed.				*/
    BIT_FIELD  exDividend		:1;  	/* ExDividend today.			*/
    BIT_FIELD  split 		   	:1;  	/* Split today.					*/
} DBA_BITS_3;
#else
typedef struct  _DBA_BITS_3 {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	deleted			:1;		// Soft delete.
	BIT_FIELD	spread			:1;
	BIT_FIELD	bankruptcy		:1;
	BIT_FIELD	nonFirm			:1;
//	BIT_FIELD	oddLotOpen		:1;		// Moved to Extended Bits
	BIT_FIELD	restricted		:1;		// Restricted.
	BIT_FIELD	openIndicator	:1;		// Issue has opened today.
	BIT_FIELD	newsIndicator	:1;		// News indicator.
	BIT_FIELD	fastMarket		:1;
	BIT_FIELD	rotation		:1;
	BIT_FIELD   haltedMarket	:1;
	BIT_FIELD	straddle		:1;
	BIT_FIELD	autoExecute		:1;		// Auto execute
	BIT_FIELD	lockedMarket	:1;
	BIT_FIELD	crossedMarket	:1;
	BIT_FIELD	exDividend		:1;		// ExDividend today
	BIT_FIELD	split			:1;		// Split today.
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	split			:1;		
	BIT_FIELD	exDividend		:1;
	BIT_FIELD	crossedMarket	:1;
	BIT_FIELD	lockedMarket	:1;
	BIT_FIELD	autoExecute		:1;
	BIT_FIELD	straddle		:1;
	BIT_FIELD   haltedMarket	:1;
	BIT_FIELD	rotation		:1;
	BIT_FIELD	fastMarket		:1;
	BIT_FIELD	newsIndicator	:1;		
	BIT_FIELD	openIndicator	:1;		
	BIT_FIELD	restricted		:1;	
//	BIT_FIELD	oddLotOpen		:1;		// Moved to Extended Bits
	BIT_FIELD	nonFirm			:1;
	BIT_FIELD	bankruptcy		:1;
	BIT_FIELD	spread			:1;
	BIT_FIELD	deleted			:1;		
#endif
} DBA_BITS_3;
#endif

typedef union {
	DBA_BITS_3	dbaBits3;
	USHORT		flags;
} DBA_MASK_3;	            
// defines for USHORT tick
#define DNTICK_LASTSALE		0x8000
#define UPTICK_LASTSALE		0x4000
#define DNTICK_OPEN 		0x2000
#define UPTICK_OPEN 		0x1000
#define DNTICK_CLOSE 		0x0800
#define UPTICK_CLOSE 		0x0400
#define DNTICK_BID			0x0200
#define UPTICK_BID			0x0100
#define DNTICK_ASK			0x0080
#define UPTICK_ASK			0x0040
// Define flag bits in Extended database as mask values.

#define EXT_COMP_REC_ONLY			  0x00000001L
#define EXT_COMP_CREATION			  0x00000002L
#define EXT_PRIM_EXCH_LOCKED		  0x00000004L
//#define EXT_DELETED					  0x00000008L
//#define EXT_OPEN_INDICATOR 			  0x00000010L
#define EXT_UP_TICK					  0x00000020L
#define EXT_DN_TICK					  0x00000040L
//#define EXT_NEWS_INDICATOR 			  0x00000080L
//#define EXT_BANKRUPTCY				  0x00000100L
//#define EXT_FAST_MARKET 			  0x00000200L
//#define EXT_ROTATION_MARKET			  0x00000400L
//#define EXT_HALTED_MARKET			  0x00000800L
//#define EXT_SPREAD_TRADE			  0x00001000L
//#define EXT_STRADDLE_TRADE 			  0x00002000L
//#define EXT_NON_FIRM_QUOTE 			  0x00004000L
//#define EXT_LOCKED_QUOTE			  0x00008000L
//#define EXT_CROSSED_QUOTE			  0x00010000L
//#define EXT_AUTO_EXECUTE			  0x00020000L
//#define EXT_EXDIVIDEND				  0x00040000L
//#define EXT_SPLIT 					  0x00080000L
#define EXT_DSP_PRICE_LOCKED		  0x00100000L
#define EXT_OPEN_PENDING			  0x00200000L
//#define EXT_RESTRICTED				  0x00400000L
#define EXT_SKIP_BIDASK_RESET 		  0x00800000L
#define EXT_SETTLEMENT				  0x01000000L
#define EXT_QUOTED					  0x02000000L
#define EXT_ODDLOTOPEN				  0x04000000L
#define EXT_BID_DOWN_TICK			  0x08000000L
#define EXT_OPEN_INTEREST_FILTER	  0x10000000L
#define EXT_NEW_ADD                   0x20000000L
#define EXT_CORRECTED                 0x40000000L

typedef struct  DBA_EXTENDED_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD		 compRecOnly			:1;	// Composite record only.
	BIT_FIELD		 compCreation			:1;	// Created from a composite exchange msg.
	BIT_FIELD		 primExchLocked			:1;	// Primary exchange code is fixed.
	BIT_FIELD		 undefineddeleted		:1;	// Issue will be deleted.
	BIT_FIELD		 undefinedopenIndicator	:1;	// Issue should be displayed as open.
	BIT_FIELD		 upTick					:1;	// Last trade > previous trade.
	BIT_FIELD		 dnTick					:1;	// Last trade < previous trade.
	BIT_FIELD		 undefinednewsIndicator	:1;	// Symbol has a news story.
	BIT_FIELD		 undefinedbankruptcy	:1;
	BIT_FIELD		 undefinedfastMarket 	:1;	// Market condition flags.
	BIT_FIELD		 undefinedrotationMarket:1;
	BIT_FIELD		 undefinedhaltedMarket	:1;
	BIT_FIELD		 undefinedspreadTrade	:1;	// Trade condition flags.
	BIT_FIELD		 undefinedstraddleTrade :1;
	BIT_FIELD		 undefinednonFirmQuote	:1;
	BIT_FIELD		 undefinedlockedQuote	:1;	// Quote condition flags.
	BIT_FIELD		 undefinedcrossedQuote	:1;
	BIT_FIELD		 undefinedautoExecute	:1;
	BIT_FIELD		 undefinedexDividend	:1;	// ExDividend today.
	BIT_FIELD		 undefinedsplit			:1;	// Split today.
	BIT_FIELD		 dspPriceLocked			:1;	// Display price type is locked.
	BIT_FIELD		 openPending			:1;	// Traded and closed.
	BIT_FIELD		 undefinedrestricted	:1;
	BIT_FIELD		 skipBidAskReset		:1;	// Skip bid/ask reset at BOD.
	BIT_FIELD		 settlement				:1;
	BIT_FIELD		 quoted					:1;	// Symbol quoted today.
	BIT_FIELD		 oddlotOpen				:1;	// Opened by oddlot and no boardlot.
	BIT_FIELD		 bidDnTick				:1;	// Bid down tick.
	BIT_FIELD		 drop1			  		:1;	// Drop symbol on 19.2 (openInterest filter).
	BIT_FIELD		 newAdd				  	:1;
	BIT_FIELD		 corrected				:1;
	BIT_FIELD		 splitApplied			:1;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD		 splitApplied			:1;
	BIT_FIELD		 corrected				:1;
	BIT_FIELD		 newAdd				  	:1;
	BIT_FIELD		 drop1			  		:1;	// Drop symbol on 19.2 (openInterest filter).
	BIT_FIELD		 bidDnTick				:1;	// Bid down tick.
	BIT_FIELD		 oddlotOpen 			:1;	// Opened by oddlot and no boardlot.
	BIT_FIELD		 quoted					:1;	// Symbol quoted today.
	BIT_FIELD		 settlement			 	:1;
	BIT_FIELD		 skipBidAskReset		:1;	// Skip bid/ask reset at BOD.
	BIT_FIELD		 undefinedrestricted 	:1;
	BIT_FIELD		 openPending			:1;	// Traded and closed.
	BIT_FIELD		 dspPriceLocked			:1;	// Display price type is locked.
	BIT_FIELD		 undefinedsplit			:1;	// Split today.
	BIT_FIELD		 undefinedexDividend 	:1;	// ExDividend today.
	BIT_FIELD		 undefinedautoExecute	:1;
	BIT_FIELD		 undefinedcrossedQuote	:1;
	BIT_FIELD		 undefinedlockedQuote	:1;	// Quote condition flags.
	BIT_FIELD		 nonFirmQuote			:1;
	BIT_FIELD		 straddleTrade 			:1;
	BIT_FIELD		 spreadTrade			:1;	// Trade condition flags.
	BIT_FIELD		 undefinedhaltedMarket	:1;
	BIT_FIELD		 rotationMarket			:1;
	BIT_FIELD		 undefinedfastMarket	:1;	// Market condition flags.
	BIT_FIELD		 undefinedbankruptcy 	:1;
	BIT_FIELD		 newsIndicator 			:1;	// Symbol has a news story.
	BIT_FIELD		 dnTick					:1;	// Last trade < previous trade.
	BIT_FIELD		 upTick					:1;	// Last trade > previous trade.
	BIT_FIELD		 undefinedopenIndicator :1;	// Issue should be displayed as open.
	BIT_FIELD		 undefineddeleted 		:1;	// Issue will be deleted.
	BIT_FIELD		 primExchLocked			:1;	// Primary exchange code is fixed.
	BIT_FIELD		 compCreation			:1;	// Created from a composite exchange msg.
	BIT_FIELD		 compRecOnly			:1;	// Composite record only.

#endif

#define	noStatistics	undefinedrestricted		//  re use old flag for Stats processing	

} DBA_EXTENDED_BITS;  // 4 bytes

typedef union _DBA_EXTENDED_FLAGS {
	DBA_EXTENDED_BITS   bits; // 4 bytes
	ULONG           mask;
} DBA_EXTENDED_FLAGS;       // 4 bytes

typedef struct	_DBA_RECORD_3 {
	DBA_KEY				item;
	DBA_MASK_3			dbaMask3;
	USHORT				priceType;
	USHORT				securityCatagory;
	USHORT				tick;
	SHORT 				newsSource[3];
	CHAR				primaryExchangeCode[2];
	USHORT				quoteIndicator;
	USHORT				row;
	USHORT 				column;
	DBA_PRICE			lastSale;
	DBA_PRICE			open;
	DBA_PRICE			high;
	DBA_PRICE			low;
	DBA_PRICE			close;
	DBA_PRICE			bid;
	DBA_PRICE			ask;
	DBA_PRICE			mid;
	DBA_PRICE			midClose;
	ULONG 				openInterest;
	ULONG 				volume;
	ULONG				openRange;
	ULONG				closeRanges[2];
	USHORT				tradeIndicator;
	USHORT				session;
	LONG				prevBidPrice;				// Previous closing bid price.
	LONG				prevAskPrice;				// Previous closing ask price.
	ULONG				bidVolume;					// Total volume of trades at the bid price.
	ULONG				askVolume;					// Total volume of trades at the ask price.
	LONGLONG			moneyFlow;					// Total value of ask side sales minus bid side sales.
	UCHAR				prevBidAskMonth;			// Previous Bid Ask Month
	UCHAR				prevBidAskDay;				// Previous Bid Ask Day
	CHAR				reserved[48];
	USHORT				serverData;
	VOID				*pServerList;

} DBA_RECORD_3;

// tickTrend values.  Used to calculate $TICK.

#define TICKTREND_BOD    0      // Beginning Of Day
#define TICKTREND_UP    +1
#define TICKTREND_DOWN  -1

typedef struct	_DBA_RECORD_EXTENDED{

	DBA_RECORD_3		dbaRec;

	//	Following values only available via dba over a network

	DBA_EXTENDED_FLAGS	flags;
	DBA_PRICE			settlement;					// 12 bytes
	DBA_PRICE			yestSettle;					// 12 bytes
	ULONG				yestVolume;					//  yesterdays volume
	ULONG				lateVolume;					//  late trading volume 
	ULONG				lateTicks;					//  late trading volume
	ULONG				lastTradeSrcSeq;
	ULONG				lastQuoteSrcSeq;
	UCHAR				lastQuoteExch[2];			//  exchange of last quote 	
	SHORT				tickTrend;

} DBA_RECORD_EXTENDED;

typedef struct	_DBA_RECORD_4{
	DBA_KEY		item;
	DBA_KEY		data[10];
} DBA_RECORD_4; 

/* packet Type MT_ORDER */
typedef struct _HTI_ENTRY_ID_ECN {
	CHAR id[16];			// right justified Ecn sequence number
} HTI_ENTRY_ID_ECN;

typedef struct _HTI_ENTRY_ID_ADAP {
	CHAR ticker[7];
	CHAR rank[9];			// right justified.
} HTI_ENTRY_ID_ADAP;

typedef struct _HTI_ENTRY_ID_DEPTH {
	CHAR ticker[12];
	CHAR rank[4];			// right justified.
} HTI_ENTRY_ID_DEPTH;

typedef struct _HTI_ENTRY_ID_PRIME {
	CHAR ticker[7];
	CHAR price[9]; 		// decimal no decimal point
} HTI_ENTRY_ID_PRIME;

typedef struct _HTI_ENTRY_ID_OPEN_BOOK {
	CHAR	price[16];		// right justified no decimal point			
} HTI_ENTRY_ID_OPEN_BOOK;

typedef union _HTI_ENTRY_ID {
	HTI_ENTRY_ID_ECN ecn;		// Use for Product code EC
	HTI_ENTRY_ID_ADAP adap;		// Use for Product code AD
	HTI_ENTRY_ID_PRIME prime;	// Use for Product code PR	
	HTI_ENTRY_ID_OPEN_BOOK openBook; // Use for Product code NY
	HTI_ENTRY_ID_DEPTH depth; // Use for Product code DB
	CHAR entryID[16];	// 16 bytes	 // Default use	
} HTI_ENTRY_ID;

#define	DBA_ORDER_TYPE_BUY							0x01
#define DBA_ORDER_TYPE_NEGOTIABLE					0x02
#define DBA_ORDER_TIME_IN_FORCE_DAY					0x04
#define DBA_ORDER_TIME_IN_FORCE_GTC					0x08
#define DBA_ORDER_TIME_IN_FORCE_OPENING				0x10
#define DBA_ORDER_TIME_IN_FORCE_IMEDIATE_OR_CANCEL	0x20
#define DBA_ORDER_TIME_IN_FORCE_FILL_OR_KILL		0x40
#define DBA_ORDER_TIME_IN_FORCE_GOOD_TILL_CROSSING	0x80
#define DBA_ORDER_TIME_IN_FORCE_TILL_DATE			0x100
#define DBA_ORDER_OPEN_INDICATOR					0x200
#define DBA_ORDER_DELETED							0x8000

typedef struct _DBA_ORDER_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD		buySell			: 1;  //0 Sell 1=Buy
	BIT_FIELD		negotiable		: 1;
	BIT_FIELD		TIF_Day			: 1;
	BIT_FIELD		TIF_Gtc			: 1;
	BIT_FIELD		TIF_Opening		: 1;
	BIT_FIELD		TIF_Immediate	: 1;
	BIT_FIELD		TIF_Fill		: 1;
	BIT_FIELD		TIF_Crossing	: 1;
	BIT_FIELD		TIF_Date		: 1;
	BIT_FIELD		openIndicator	: 1;	// Open
	BIT_FIELD		unused			: 5;
	BIT_FIELD		deleted			: 1;  // deleted record
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD		deleted			: 1;  // deleted record
	BIT_FIELD		unused			: 5;
	BIT_FIELD		openIndicator	: 1;	// Open
	BIT_FIELD		TIF_Date		: 1;
	BIT_FIELD		TIF_Crossing	: 1;
	BIT_FIELD		TIF_Fill		: 1;
	BIT_FIELD		TIF_Immediate	: 1;
	BIT_FIELD		TIF_Opening		: 1;
	BIT_FIELD		TIF_Gtc			: 1;
	BIT_FIELD		TIF_Day			: 1;
	BIT_FIELD		negotiable		: 1;
	BIT_FIELD		buySell			: 1;  //0 Sell 1=Buy
#endif
} DBA_ORDER_BITS;

typedef union _DBA_ORDER_FLAGS {
	DBA_ORDER_BITS	bits;
	USHORT          mask;
} DBA_ORDER_FLAGS;           // 2 bytes

#define DBA_ORDER_KEY_ENTRY		0			// Primary Key number by Entry Number
#define DBA_ORDER_KEY_KEYECN	1			// By Key & ECN
//#define DBA_ORDER_KEY_ECN		2			// By ECN

//#define ORDER_ECN_ID_SIZE		8
//#define ORDER_ENTRY_ID_SIZE	16

#define ORDER_ECN_PRODUCT_SIZE	2
#define ORDER_ECN_SOURCE_SIZE	6

typedef struct _DBA_ORDER_KEY_0 {
	CHAR				productCode[ORDER_ECN_PRODUCT_SIZE];		// 2 bytes 	
	CHAR				sourceID[ORDER_ECN_SOURCE_SIZE];			// 6 bytes 	
	HTI_ENTRY_ID		entryID;									// 16 bytes
} DBA_ORDER_KEY_0;

typedef struct _DBA_ORDER_KEY_1 {
	DBA_KEY				dbaKey;
	CHAR				productCode[ORDER_ECN_PRODUCT_SIZE];		// 2 bytes 	
	CHAR				sourceID[ORDER_ECN_SOURCE_SIZE];			// 6 bytes 	
} DBA_ORDER_KEY_1;

typedef struct	_DBA_RECORD_73 {
	DBA_KEY				item;
	CHAR				productCode[2];		// 2 bytes 	
	CHAR				sourceID[6];		// 6 bytes 	
	HTI_ENTRY_ID		entryID;			// 16 bytes
	DBA_ORDER_FLAGS		flags;
	USHORT				priceType;
	USHORT				securityCatagory;
	USHORT				row;
	USHORT				column;
	USHORT				session;
	ULONG				orderTime;        //  4 bytes
	LONG				price;            //  4 bytes
	LONG				size;             //  4 bytes
	LONG				numOrders;        //  4 bytes
	ULONG				expireTime;       //  4 bytes
	USHORT				timeInForce;	  //  2 bytes
	UCHAR				indicator;		  //  1 byte
	UCHAR				reserved;
} DBA_RECORD_73;

/*
typedef struct	_DBA_RECORD_73 {
	DBA_KEY			item;
	CHAR			ecnID[ORDER_ECN_ID_SIZE];		// 8 bytes ECN Identifier
	CHAR			entryID[ORDER_ENTRY_ID_SIZE];	// 16 bytes Order reference
	DBA_ORDER_FLAGS	flags;
	USHORT			priceType;
	USHORT			securityCatagory;
	USHORT			row;
	USHORT			column;
	USHORT			session;
	ULONG			orderTime;        //  4 bytes
	LONG			price;            //  4 bytes
	LONG			size;             //  4 bytes
	LONG			numOrders;        //  4 bytes
	ULONG			expireTime;       //  4 bytes
	USHORT			timeInForce;	  //  2 bytes
} DBA_RECORD_73;
*/

#define	DBA_MARKET_QUOTE_CANCEL           0x0001
#define	DBA_MARKET_QUOTE_OPEN_INDICATOR   0x0002
#define	DBA_MARKET_QUOTE_RESTRICTED       0x0004
#define	DBA_MARKET_QUOTE_BID_TICK_PRESENT 0x0008
#define	DBA_MARKET_QUOTE_BID_DOWN_TICK    0x0010
#define	DBA_MARKET_QUOTE_NO_TICK		  0x0020
#define	DBA_MARKET_QUOTE_DELETED		  0x0040

typedef struct _DBA_MARKET_QUOTE_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	cancel       		: 1;
	BIT_FIELD	openIndicator		: 1;
	BIT_FIELD	restricted			: 1;
	BIT_FIELD	bidTickPresent		: 1;
	BIT_FIELD	bidDownTick			: 1;
	BIT_FIELD	noTick				: 1;
	BIT_FIELD	deleted				: 1;
	BIT_FIELD	undefined			: 9;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	undefined			: 9;
	BIT_FIELD	deleted				: 1;
	BIT_FIELD	noTick				: 1;
	BIT_FIELD	bidDownTick			: 1;
	BIT_FIELD	bidTickPresent		: 1;
	BIT_FIELD	restricted			: 1;
	BIT_FIELD	openIndicator		: 1;
	BIT_FIELD	cancel       		: 1;
	
#endif
} DBA_MARKET_QUOTE_BITS;

typedef union _DBA_MARKET_QUOTE {
	DBA_MARKET_QUOTE_BITS	bits;
	USHORT					mask;
} DBA_MARKET_QUOTE_FLAGS;	// 2 bytes


typedef struct _DBA_RECORD_MARKET_QUOTE {
	DBA_KEY					item;				//22 Bytes
	DBA_MARKET_QUOTE_FLAGS	flags;				// 2 Bytes
	USHORT					securityCategory;	// 2 Bytes
	USHORT					row;				// 2 Bytes
	USHORT 					column;				// 2 Bytes
	DBA_EXT_PRICE			bid;				// 16 bytes
	DBA_EXT_PRICE			ask;				// 16 bytes
	UCHAR					bidQuoteIndicator;	// 1 Byte
	UCHAR					bidXpressIndicator;	// 1 Byte
	UCHAR					askQuoteIndicator;	// 1 Byte
	UCHAR					askXpressIndicator;	// 1 Byte
} DBA_RECORD_MARKET_QUOTE;				//66 bytes


#define	BEST_SAEF_NO_DEAL 	0	/* unwilling to deal at best SAEF price		*/
#define	BEST_SAEF_BID		1	/* willing to buy at best SAEF Bid			*/
#define	BEST_SAEF_ASK		2	/* willing to sell at best SAEF Ask			*/
#define	BEST_SAEF_BOTH 		3	/* willing to buy and sell at SAEF Best.	*/

typedef struct _DBA_BITS_5 {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD  bestIndicator	: 3;
	BIT_FIELD  deleted		 	: 1;
	BIT_FIELD  bidFiltered  	: 1;
	BIT_FIELD  askFiltered  	: 1;
	BIT_FIELD  bidUpTick		: 1;
	BIT_FIELD  bidDnTick		: 1;
	BIT_FIELD  askUpTick		: 1;
	BIT_FIELD  askDnTick		: 1;
	BIT_FIELD  reserved 	 	: 1;
	BIT_FIELD  zeroBidAsk		: 1;
	BIT_FIELD  reserved1	 	: 4;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD  reserved1	 	: 4;
	BIT_FIELD  zeroBidAsk		: 1;
	BIT_FIELD  reserved 	 	: 1;
	BIT_FIELD  askDnTick		: 1;
	BIT_FIELD  askUpTick		: 1;
	BIT_FIELD  bidDnTick		: 1;
	BIT_FIELD  bidUpTick		: 1;
	BIT_FIELD  askFiltered  	: 1;
	BIT_FIELD  bidFiltered  	: 1;
	BIT_FIELD  deleted		 	: 1;
	BIT_FIELD  bestIndicator	: 3;
#endif
} DBA_BITS_5;

typedef union {
	DBA_BITS_5	dbaBits5;
	USHORT		flags;
} DBA_MASK_5;

typedef struct _DBA_RECORD_5	{					  /* Market Maker  */
	DBA_MMKEY	item;
	DBA_MASK_5	dbaMask5;
#ifdef __sun
	TICK_PCQ	tick;
#else
	TICK		tick;
#endif
	USHORT		priceType;
	USHORT		securityCatagory;
	USHORT		quoteIndicator;
	DBA_PRICE	bid;
	DBA_PRICE	ask;
	BYTE		bidTimeHundredths;
	BYTE		bidTimeSeconds;
	BYTE		askTimeHundredths;
	BYTE		askTimeSeconds;
	USHORT		row;
	USHORT		column;
	USHORT      session;
	ULONG 		lastQuoteSrcSeq;
} DBA_RECORD_5;

typedef struct _NEWSBITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD  prNewsWireInc		:1;		/* 			P			*/
	BIT_FIELD  securityTraders		:1;		/* (ST ) 	Y			*/
	BIT_FIELD  unitedPressInt 		:1;		/* (UPI) 	U			*/
	BIT_FIELD  prNewsWire			:1;		/* (PR ) 	R			*/
	BIT_FIELD  kyoddNewsInt			:1;		/* (KYO) 	K			*/
	BIT_FIELD  garvinGuybutler		:1;		/* (GGB) 	G			*/
	BIT_FIELD  associatedPress		:1;		/* (AP ) 	E			*/
	BIT_FIELD  afpDpa				:1;		/* (AFP) 	D			*/
	BIT_FIELD  comtexScientific		:1;		/* (CTX) 	X			*/
	BIT_FIELD  businessWire			:1;		/* (BW ) 	S			*/
	BIT_FIELD  capitalMarketNews 	:1;		/* (CMR) 	C			*/
	BIT_FIELD  dowJonesWallStreet	:1;		/* (DWS) 	W			*/
	BIT_FIELD  newsRetrieval		:1;		/* (DNR) 	N			*/
	BIT_FIELD  barrons				:1;		/* (BAR) 	B			*/
	BIT_FIELD  wallStreetJournal 	:1;		/* (WSJ) 	J			*/
	BIT_FIELD  dowJones 			:1;		/* (DJ)		T			*/
	BIT_FIELD  unused				:6;		/* future growth		*/
	BIT_FIELD  futureWorld 			:1;		/* (FWN) 	1			*/
	BIT_FIELD  federalFilings 		:1;		/* (FED) 	L			*/
	BIT_FIELD  firstCallUS 			:1;		/* (FC ) 	F			*/
	BIT_FIELD  profInvestor			:1;		/* (PIR) 	I			*/
	BIT_FIELD  reutersDirect		:1;		/* (RTR) 	A			*/
	BIT_FIELD  muniFacts			:1;		/* (MFX) 	M			*/
	BIT_FIELD  intlDataGroup		:1;		/* (not implemented)	*/
	BIT_FIELD  businessWireNews		:1;		/* (BW ) 	Z			*/
	BIT_FIELD  federalNews 			:1;		/* (FNS) 	V			*/
	BIT_FIELD  dpa					:1;		/* (DPA) 	H			*/
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD  dpa					:1;		
	BIT_FIELD  federalNews 			:1;		
	BIT_FIELD  businessWireNews		:1;		
	BIT_FIELD  intlDataGroup		:1;		
	BIT_FIELD  muniFacts			:1;		
	BIT_FIELD  reutersDirect		:1;		
	BIT_FIELD  profInvestor			:1;		
	BIT_FIELD  firstCallUS 			:1;		
	BIT_FIELD  federalFilings 		:1;		
	BIT_FIELD  futureWorld 			:1;		
	BIT_FIELD  unused				:6;		
	BIT_FIELD  dowJones 			:1;		
	BIT_FIELD  wallStreetJournal 	:1;		
	BIT_FIELD  barrons				:1;		
	BIT_FIELD  newsRetrieval		:1;		
	BIT_FIELD  dowJonesWallStreet	:1;		
	BIT_FIELD  capitalMarketNews 	:1;		
	BIT_FIELD  businessWire			:1;		
	BIT_FIELD  comtexScientific		:1;		
	BIT_FIELD  afpDpa				:1;		
	BIT_FIELD  associatedPress		:1;		
	BIT_FIELD  garvinGuybutler		:1;		
	BIT_FIELD  kyoddNewsInt			:1;		
	BIT_FIELD  prNewsWire			:1;		
	BIT_FIELD  unitedPressInt 		:1;		
	BIT_FIELD  securityTraders		:1;		
	BIT_FIELD  prNewsWireInc		:1;		
#endif
} NEWSBITS;

typedef	struct  _NEWS_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD  dowJones 			: 1;  // (DJ) 	 T
	BIT_FIELD  wallStreetJournal	: 1;  // (WSJ)	 J
	BIT_FIELD  barrons				: 1;  // (BAR)	 B
	BIT_FIELD  newsRetrieval		: 1;  // (DNR)	 N
	BIT_FIELD  dowJonesWallStreet	: 1;  // (DWS)	 W
	BIT_FIELD  capitalMarketNews	: 1;  // (CMR)	 C
	BIT_FIELD  businessWire			: 1;  // (BW )	 S
	BIT_FIELD  comtexScientific		: 1;  // (CTX)	 X
	BIT_FIELD  afpDpa				: 1;  // (AFP)	 D
	BIT_FIELD  associatedPress	 	: 1;  // (AP )	 E
	BIT_FIELD  garvinGuyButler	 	: 1;  // (GGB)	 G
	BIT_FIELD  kyoddNewsInt		 	: 1;  // (KYO)	 K
	BIT_FIELD  prNewsWire			: 1;  // (PR )	 R
	BIT_FIELD  unitedPressInt 	 	: 1;  // (UPI)	 U
	BIT_FIELD  securityTraders	 	: 1;  // (ST )	 Y
	BIT_FIELD  prNewsWireInc		: 1;  //		 P
	BIT_FIELD  dpa					: 1;  // (DPA)	 H
	BIT_FIELD  federalNews 		 	: 1;  // (FNS)	 V
	BIT_FIELD  businessWireNews		: 1;  // (BW )	 Z
	BIT_FIELD  intlDataGroup		: 1;  // (not implemented)
	BIT_FIELD  muniFacts			: 1;  // (MFX)	 M
	BIT_FIELD  reutersDirect		: 1;  // (RTR)	 A
	BIT_FIELD  profInvestor		 	: 1;  // (PIR)	 I
	BIT_FIELD  firstCallUS 		 	: 1;  // (FC )	 F
	BIT_FIELD  federalFilings 	 	: 1;  // (FED)	 L
	BIT_FIELD  endOfUS2 			: 7;
	BIT_FIELD  endOfUS3 			:16;
	BIT_FIELD  endOfUS4 			:16;
	BIT_FIELD  extel 				: 1;  // (EXT)	 E
	BIT_FIELD  extelWorldEcoNews	: 1;  // (GEX)	 W
	BIT_FIELD  extelAfp 			: 1;  // (AFX)	 A
	BIT_FIELD  vwdNews			 	: 1;  // (VWD)	 V
	BIT_FIELD  djiEcoNewsWire 	 	: 1;  // (DJE)	 I
	BIT_FIELD  djiFinNewsWire 	 	: 1;  // (DJF)	 F
	BIT_FIELD  djiEuroCorpRept	 	: 1;  // (DJC)	 C
	BIT_FIELD  firstCallEurope	 	: 1;  // (FCE)	 B
	BIT_FIELD  endofEurope1		 	: 8;
	BIT_FIELD  endofEurope2		 	:16;
	BIT_FIELD  nikkei				: 1;  // (NEN)	 N
	BIT_FIELD  austrFinRpt 		 	: 1;  // (AFR)	 A
	BIT_FIELD  austrAP				: 1;  // (AAP)	 P
	BIT_FIELD  kyodoNews			: 1;  // (KYO)	 K
	BIT_FIELD  firstCallPacific		: 1;  // (FCP)	 F
	BIT_FIELD  endofAsia1			:11;
	BIT_FIELD  endofAsia2			:16;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD  endofAsia2			:16;
	BIT_FIELD  endofAsia1			:11;
	BIT_FIELD  firstCallPacific		: 1;  
	BIT_FIELD  kyodoNews			: 1;  
	BIT_FIELD  austrAP				: 1;  
	BIT_FIELD  austrFinRpt 		 	: 1;  
	BIT_FIELD  nikkei				: 1;  
	BIT_FIELD  endofEurope2		 	:16;
	BIT_FIELD  endofEurope1		 	: 8;
	BIT_FIELD  firstCallEurope	 	: 1;  
	BIT_FIELD  djiEuroCorpRept	 	: 1;  
	BIT_FIELD  djiFinNewsWire 	 	: 1;  
	BIT_FIELD  djiEcoNewsWire 	 	: 1;  
	BIT_FIELD  vwdNews			 	: 1;  
	BIT_FIELD  extelAfp 			: 1;  
	BIT_FIELD  extelWorldEcoNews	: 1;  
	BIT_FIELD  extel 				: 1;  
	BIT_FIELD  endOfUS4 			:16;
	BIT_FIELD  endOfUS3 			:16;
	BIT_FIELD  endOfUS2 			: 7;
	BIT_FIELD  federalFilings 	 	: 1;  
	BIT_FIELD  firstCallUS 		 	: 1;  
	BIT_FIELD  profInvestor		 	: 1;  
	BIT_FIELD  reutersDirect		: 1;  
	BIT_FIELD  muniFacts			: 1;  
	BIT_FIELD  intlDataGroup		: 1;  
	BIT_FIELD  businessWireNews		: 1;  
	BIT_FIELD  federalNews 		 	: 1;  
	BIT_FIELD  dpa					: 1;  
	BIT_FIELD  prNewsWireInc		: 1;  
	BIT_FIELD  securityTraders	 	: 1;  
	BIT_FIELD  unitedPressInt 	 	: 1;  
	BIT_FIELD  prNewsWire			: 1;  
	BIT_FIELD  kyoddNewsInt		 	: 1;  
	BIT_FIELD  garvinGuyButler	 	: 1;  
	BIT_FIELD  associatedPress	 	: 1;  
	BIT_FIELD  afpDpa				: 1;  
	BIT_FIELD  comtexScientific		: 1;  
	BIT_FIELD  businessWire			: 1;  
	BIT_FIELD  capitalMarketNews	: 1;  
	BIT_FIELD  dowJonesWallStreet	: 1;  
	BIT_FIELD  newsRetrieval		: 1;  
	BIT_FIELD  barrons				: 1;  
	BIT_FIELD  wallStreetJournal	: 1;  
	BIT_FIELD  dowJones 			: 1;  
#endif
} NEWS_BITS;

typedef union _NEWS_FLAGS {
	NEWS_BITS	bits;
	ULONG 		mask[4];
} NEWS_FLAGS;

typedef union _NEWS_MASK {
	NEWSBITS 	bits;
	ULONG 		mask;
} NEWS_MASK;

#define NB_US1 		0
#define NB_US2 		1
#define NB_EUROPE 	2
#define NB_ASIA		3

typedef struct _DBA_RECORD_6 {				 // News Flags
	DBA_KEY		 item;
	USHORT		 zeroCount;
	NEWS_FLAGS	 flags;
} DBA_RECORD_6;

//	szCoode 
//	1 = Dividend
//  2 = Split
//  3 = Symbol Change

typedef struct _DBA_RECORD_12 {
	CHAR		szCode[2];
	CHAR		szSymbol[12];
	CHAR		szValue1[6];
	CHAR		szValue2[6];
	CHAR		szPadding[38];
} DBA_RECORD_12;

#define DBA_TS_TYPE_LAST	0
#define DBA_TS_TYPE_BID		1
#define DBA_TS_TYPE_ASK		2
#define DBA_TS_TYPE_MMBID	3
#define DBA_TS_TYPE_MMASK	4
#define DBA_TS_TYPE_LAST_ID	5
#define DBA_TS_TYPE_DELETE	0x80
#define DBA_TS_TYPE_END		0xff

#define DBA_TS_MAX_ENTRY	8

typedef struct DBA_TS_ITEM {
	LONG 	price;
	ULONG 	size;
	UCHAR	type;
	UCHAR	indicator;
	UCHAR	priceType;
	UCHAR 	exchangeCode[4];
	UCHAR 	hour;
	UCHAR 	minute;
	UCHAR	second;
} DBA_TS_ITEM;

typedef struct _DBA_TS_REC {
	DBA_TS_ITEM		tsData[DBA_TS_MAX_ENTRY];
	USHORT			row;
	USHORT			column;
} DBA_TS_REC;

typedef struct _DBA_SESSION_KEY {
	USHORT			session; 				// Session number.
	USHORT			eventType;				// Type of event.
} DBA_SESSION_KEY;

typedef struct _DBA_SESSION_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD		disabled	: 1;		// Session disabled.
	BIT_FIELD		unused		: 15;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD		unused		: 15;
	BIT_FIELD		disabled	: 1;	
#endif
} DBA_SESSION_BITS;

typedef union _DBA_SESSION_FLAGS {
	DBA_SESSION_BITS	bits;
	USHORT				flags;
} DBA_SESSION_FLAGS;

typedef struct _DBA_SESSION_REC {	
	DBA_SESSION_KEY		key;
	DBA_SESSION_FLAGS	flags;
	PCQ_TIME 			timeGMT; 		// GMT time of event.
	PCQ_DATE 			dateLocal;		// Exchange local date of roll.
	PCQ_TIME 			timeReceipt;	// Local (PC) time of message receipt.
	UCHAR 				sessionBODs[7];	// # of session starts per day of week.
	UCHAR 				sessionEODs[7];	// # of session ends per day of week.
	UCHAR 				unused[64];
} DBA_SESSION_REC;

typedef struct {
	DBA_KEY		item;
	union {								// OTHER INFORMATION USED IN REQUEST/RESPONSE
		CHAR		unused[10];			// Insure we have atleast 10 bytes
		struct {
			ULONG		startTime; 		// input: KEY's creation time
			CHAR		flag;			// input: KEY's flag bits
		} addKey;						// request: DBA_ADD_KEY
		struct {
			ULONG		time;			// input: time of get (0=none)
			SHORT 		nRecs;			// output: Number of DBA_HISTORY_RECs found
		} getTS; 						// request: DBA_GET_FIRST_TS, etc.
		struct {
			LONG		blockNo;		// input: Block to get
		} getDirect;					// request: DBA_GET_DIRECT
	} keys;
} DBA_HISTORY_KEY;	// Must not exceed 64 bytes

#define DBAHIST_RECTYPE_PRICE		0	// Price record

#define DBAHIST_RECTYPE_TRADE		0	// Trade record
#define DBAHIST_RECTYPE_QUOTE		1	// Quote record
#define DBAHIST_RECTYPE_MMQUOTE		3	// Level II Quote record
#define DBAHIST_RECTYPE_TRADE_ID	5	// Buyer / Seller ID in Exchange Field

#define DBAHIST_RECTYPE_SPLIT		16	// Split Record
#define DBAHIST_RECTYPE_SYMBOL		17	// Symbol Change
#define DBAHIST_RECTYPE_DIVIDEND	18	// Dividend 

#define DBAHIST_RECTYPE_DELETED		0x7f	// Deleted 

typedef struct _DBA_TOS_SEQ {
	UCHAR  low[2];
	UCHAR  high;
} DBA_TOS_SEQ;

typedef struct {						// Record Type == DBAHIST_RECTYPE_SPLIT
	CHAR		type; 					// = DBAHIST_RECTYPE_SPLIT
	UCHAR 		priceType;				// HyperFeed price type
	ULONG		time; 					// End of session time
	LONG		numerator;				// Numerator of Split ratio.
	LONG		denominator; 			// Demoninator of Split ratio
	ULONG		timeOfLastSplit;		// time last split was recorded
	ULONG		timeOfLastDividend;		// time last dividend was recorded
	CHAR		padding[10];			// Fill out to 32 bytes
} DBA_HISTORY_SPLIT;

typedef struct {						// Record Type == DBAHIST_RECTYPE_SYMBOL
	CHAR		type; 					// = DBAHIST_RECTYPE_SYMBOL
	UCHAR 		priceType;				// HyperFeed price type
	ULONG		time; 					// End of session time
	DBA_KEY		newKey;
	CHAR		padding[4];				// Fill out to 32 bytes
} DBA_HISTORY_SYMBOL;

typedef struct {						// Record Type == DBAHIST_RECTYPE_DIVIDEND
	CHAR		type; 					// = DBAHIST_RECTYPE_SPLIT
	UCHAR 		priceType;				// HyperFeed price type
	ULONG		time; 					// End of session time
	ULONG		reserved;
	LONG		dividend;				// Divicend amount.
	ULONG		timeOfLastSplit;		// time last split was recorded
	ULONG		timeOfLastDividend;		// time last dividend was recorded
	CHAR		padding[10];			// Fill out to 32 bytes
} DBA_HISTORY_DIVIDEND;

typedef struct {						// Record Type == DBAHIST_RECTYPE_PRICE
	CHAR		type; 					// = DBAHIST_RECTYPE_PRICE
	UCHAR 		priceType;				// HyperFeed price type
	ULONG		time; 					// End of session time
	LONG		open; 					// Session open price, by priceType
	LONG		high; 					// Session high price, by priceType
	LONG		low;					// Session low price, by priceType
	LONG		last; 					// Session last price, by priceType
	LONG		volume;					// Session volume in 100s shares
	CHAR		unused[6];				// Fill out to 32 bytes
} DBA_HISTORY_RECS;

typedef struct _DBA_BAR_RECS {			// Record Type == DBAHIST_RECTYPE_PRICE
	CHAR		type; 					// = DBAHIST_RECTYPE_PRICE
	UCHAR		priceType;				// HyperFeed price type
	ULONG		time; 					// Bar begin time
	LONG		start; 					// Bar Start price, by priceType
	LONG		high; 					// Bar High price price, by priceType
	LONG		low;					// Bar Low price, by priceType
	LONG		last; 					// Bar Close price, by priceType
	LONG		volume;					// Session volume in 100s shares
	CHAR		unused[6];				// Fill out to 32 bytes
} DBA_BAR_RECS;

typedef struct _DBA_TOS_TRADE_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD			intraDay 		:1;
	BIT_FIELD			regionalOpen	:1;
	BIT_FIELD			regionalLow 	:1;
	BIT_FIELD			regionalHigh	:1;
	BIT_FIELD			filtered 		:1;
	BIT_FIELD		 	compositeOpen	:1;
	BIT_FIELD			compositeLow	:1;
	BIT_FIELD			compositeHigh	:1;
	BIT_FIELD 			updateVolume	:1;
	BIT_FIELD			updateOhl		:1;
	BIT_FIELD			updateLast		:1;
	BIT_FIELD			cancel			:1;
	BIT_FIELD			unused			:4;
#elif defined( PQTENDIAN_BIG ) 
	BIT_FIELD			unused			:4;
	BIT_FIELD			cancel			:1;
	BIT_FIELD			updateLast		:1;
	BIT_FIELD			updateOhl		:1;
	BIT_FIELD 			updateVolume	:1;
	BIT_FIELD			compositeHigh	:1;
	BIT_FIELD			compositeLow	:1;
	BIT_FIELD		 	compositeOpen	:1;
	BIT_FIELD			filtered 		:1;
	BIT_FIELD			regionalHigh	:1;
	BIT_FIELD			regionalLow 	:1;
	BIT_FIELD			regionalOpen	:1;
	BIT_FIELD			intraDay 		:1;
#endif
} DBA_TOS_TRADE_BITS;

typedef union _DBA_TOS_TRADE_FLAGS {
	DBA_TOS_TRADE_BITS	bits;
	USHORT				mask;
} DBA_TOS_TRADE_FLAGS;

typedef struct _DBA_TOS_TRADE {
	CHAR				type;					// 0 = trade
	UCHAR				priceType;				// HyperFeed price type
	ULONG				time;					// time
	LONG				price;					// last price
	LONG				size;					// last size
	UCHAR				exchangeCode[2];		// exchange code
	DBA_TOS_TRADE_FLAGS	flags;					// trade flags
	USHORT				buyerID;				// buyer ID
	USHORT				sellerID;				// seller ID
	UCHAR				reserved[6];
	UCHAR				indicator;				// sale or quote indicator;
	UCHAR				sequence[3];
} DBA_TOS_TRADE;

typedef struct _DBA_TOS_QUOTE_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD			zeroBidAsk		:1;
	BIT_FIELD			bboNoBbo 		:1;	
	BIT_FIELD			bboNoChange 	:1;	
	BIT_FIELD			bboIsBbo 		:1;	
	BIT_FIELD			unused			:4;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD			unused			:4;
	BIT_FIELD			bboIsBbo 		:1;
	BIT_FIELD			bboNoChange 	:1;
	BIT_FIELD			bboNoBbo 		:1;
	BIT_FIELD			zeroBidAsk		:1;
#endif
} DBA_TOS_QUOTE_BITS;

typedef union _DBA_TOS_QUOTE_FLAGS {
	DBA_TOS_QUOTE_BITS	bits;
	UCHAR				mask;
} DBA_TOS_QUOTE_FLAGS;

typedef struct _DBA_TOS_QUOTE {
	CHAR					type;					// 1 = quote
	UCHAR					priceType;				// HyperFeed price type
	ULONG					time;					// time
	LONG					bidPrice;				// bid price
	LONG					bidSize;				// bid size
	LONG					askPrice;				// ask price
	LONG					askSize;				// ask size
	UCHAR					bidExchangeCode[2];		// bid exchange code
	UCHAR					askExchangeCode[2];		// ask exchange
	DBA_TOS_QUOTE_FLAGS		flags;
	UCHAR					indicator;				// sale or quote indicator;
	UCHAR					sequence[3];
} DBA_TOS_QUOTE;

typedef struct _DBA_TOS_MMQUOTE {
	CHAR					type;					// 2 = level 2 quote
	UCHAR					priceType;				// HyperFeed price type
	ULONG					time;					// time
	LONG					bidPrice;				// bid price
	LONG					bidSize;				// bid size
	LONG					askPrice;				// ask price
	LONG					askSize;				// ask size
	UCHAR					mmid[4];				// market maker id
	UCHAR					unused[2];				// reserved for mmquote flags.
	UCHAR					indicator;				// sale or quote indicator.
	UCHAR					sequence[3];
} DBA_TOS_MMQUOTE;

typedef struct _DBA_HISTORY_UNDEFINED {
	CHAR		type;					// record type
	UCHAR		priceType;				// HyperFeed price type
	ULONG		time;					// time
	UCHAR		undefined[26];
} DBA_HISTORY_UNDEFINED;

typedef union _DBA_HISTORY_RECORDS {
	DBA_HISTORY_RECS		recs;
	DBA_BAR_RECS			bars;
	DBA_TOS_TRADE			trade;
	DBA_TOS_QUOTE			quote;
	DBA_TOS_MMQUOTE			mmquote;
	DBA_HISTORY_SPLIT		splits;
	DBA_HISTORY_SYMBOL		symbol;
	DBA_HISTORY_DIVIDEND	dividend;
	DBA_HISTORY_UNDEFINED	undefined;
} DBA_HISTORY_RECORDS;

typedef struct {
	ULONG 		time; 					// Time of oldest record in volume (unix time)
	LONG 		firstFreeBlk;			// First free block in free block chain
	LONG 		timeOffset; 			// Add to record time to get datemath value
	SHORT		sizeOfHIS_INFO;			// =sizeof(HIS_INFO)
	SHORT		sizeOfHIS_BLK; 			// =sizeof(HIS_BLK)
	SHORT		sizeOfHIS_HDR; 			// =sizeof(HIS_HDR)
	SHORT		sizeOfHIS_REC; 			// =sizeof(HIS_REC)
    CHAR		unused[236];            // Fill out to 256 bytes
} DBA_HISTORY_VOLINFO;

#define DBAHIST_FLAG_ISFIRST 0x01	// This is the first block for KEY

// Caution: Many of these fields are for diagnostic purposes only and are not supported.
typedef struct {
	 DBA_KEY	item; 					// HyperFeed KEY
	 ULONG		startTime;				// Time KEY was created
	 ULONG		lastTime;				// Time in last record (or 0)
	 SHORT		nRecs;					// # of records used in this block
	 SHORT		nRecsMax;				// Max # of records allowed in this block
	 LONG 		nTotalRecs; 			// Total # of records for this KEY
	 LONG 		thisBlk; 				// This block number
	 LONG 		firstBlk;				// Block number of KEY block
	 LONG 		nextBlk; 				// Pointer to next block
	 LONG 		prevBlk; 				// Pointer to prev block
 	 CHAR 		flag;
	 CHAR		reserved;
	 USHORT		row;					// Entitlement row
	 USHORT		column;					// Entitlement column
	 CHAR		unused[196];			// Fill out to 256 bytes
} DBA_HISTORY_KEYINFO;

#define DBAHIST_NRECS	8				 // # of records in DBA_HISTORY_RECS
#define DBABAR_NRECS	8				 // # of records in DBA_BAR_RECS
#define DBATOS_NRECS	8				 // # of records in DBA_TOS_RECS

typedef union _DBA_HISTORY_REC {
	DBA_HISTORY_KEYINFO		keyInfo;
	DBA_HISTORY_RECS		recs[DBAHIST_NRECS];
	DBA_BAR_RECS			bars[DBABAR_NRECS];
	DBA_TOS_TRADE			trade[DBATOS_NRECS];
	DBA_TOS_QUOTE			quote[DBATOS_NRECS];
	DBA_TOS_MMQUOTE			mmquote[DBATOS_NRECS];
	DBA_HISTORY_SPLIT		splits[DBAHIST_NRECS];
	DBA_HISTORY_SYMBOL		symbol[DBAHIST_NRECS];
	DBA_HISTORY_DIVIDEND	dividend[DBAHIST_NRECS];
	DBA_HISTORY_UNDEFINED	undefined[DBAHIST_NRECS];
} DBA_HISTORY_REC;


//--------------------------------------//
//		  Extended Deleted Key Record	//
//--------------------------------------//

typedef struct _EXTENDED_DELETED_RECORD_DBA {
	DBA_KEY		item;
	USHORT      flags;
	USHORT      dateDeleted;
	USHORT		row;
	USHORT		column;
} EXTENDED_DELETED_RECORD_DBA;

#define DBA_RECORD_36   EXTENDED_DELETED_RECORD_DBA

//--------------------------------------//
//  Market Maker Deleted Key Record		//
//--------------------------------------//

typedef struct _MMAKER_DELETED_RECORD_DBA {
	DBA_MMKEY   item;
	USHORT      flags;
	USHORT      dateDeleted;
	USHORT		row;
	USHORT		column;
} MMAKER_DELETED_RECORD_DBA;

#define DBA_RECORD_37   MMAKER_DELETED_RECORD_DBA

//--------------------------//
//	  Price Roll Record		//
//--------------------------//

#define	PR_SUNDAY			0x01		/* Day of the Week Scheduling Flags */
#define	PR_MONDAY			0x02
#define	PR_TUESDAY			0x04
#define	PR_WEDNESDAY		0x08
#define	PR_THURSDAY			0x10
#define	PR_FRIDAY			0x20
#define	PR_SATURDAY			0x40

#define	PR_BOD				0			/* index value for Beginning of Day */
#define	PR_EOD				1			/* index value for End of Day		*/
#define	PR_MOD				2			/* index value for Middle of Day	*/

#define	PR_ENABLED			0x0001
#define	PR_BOD_IMMEDIATE	0x0002
#define	PR_EOD_IMMEDIATE	0x0004
#define	PR_MOD_IMMEDIATE	0x0008

typedef struct _DBSTIME {
	ULONG		time;			/* time in seconds	*/
	ULONG		msecs;			/* milliseconds		*/
	UCHAR		hour;			/* hours			*/
	UCHAR		minutes;		/* minutes			*/
	UCHAR		seconds;		/* seconds			*/
	UCHAR		hundredths;		/* hundredths		*/
	ULONG		reserved;
	UCHAR		day;			/* day				*/
	UCHAR		month;			/* month			*/
	USHORT		year;			/* year				*/
	UCHAR		weekday;		/* day of week		*/
} DBSTIME;

typedef struct _PRICEROLL_RECORD_DBA {
	DBA_KEY		item;
	USHORT		flags;
	USHORT		state;
	UCHAR		schedHourLocal[2];
	UCHAR		schedMinuteLocal[2];
	UCHAR		schedDow[2];
	DBSTIME		lastStartGMT[2];
	DBSTIME		lastFinishGMT[2];
	DBSTIME		lastStartLocal[2];
	DBSTIME		lastFinishLocal[2];
} PRICEROLL_RECORD_DBA;

#define DBA_RECORD_39   PRICEROLL_RECORD_DBA

#define DBA_INDEX_PRICE_WEIGHTED  1
#define DBA_INDEX_MARKET_WEIGHTED 2
#define DBA_INDEX_SPREAD          3

//---------------------------//
// 	 Indices Record.		  //
//---------------------------//

typedef struct _DBA_INDICES_BITS {
	BIT_FIELD		openChanged 	:1;
	BIT_FIELD		highChanged 	:1;
	BIT_FIELD		lowChanged		:1;
	BIT_FIELD		lastChanged 	:1;
	BIT_FIELD		tickChanged 	:1;
	BIT_FIELD		closeChanged	:1;
	BIT_FIELD		manualChange	:1;
	BIT_FIELD		reserved 		:9;
	BIT_FIELD		dowAverage		:1;		//  dow jones average 
	BIT_FIELD		dowComposite	:1;		//  dow jones composite
	BIT_FIELD		schwab			:1;     //  schwab index
	BIT_FIELD		reserved2 		:9;
	BIT_FIELD		inActive		:1;		//   new index which has not had constituents initialized
	BIT_FIELD		recalcPending	:1;     //   modified and waiting for recalculation
	BIT_FIELD		recalculated	:1;     //   has been updated by index recalculation
	BIT_FIELD		deleted			:1;     //   deleted
}	DBA_INDICES_BITS;

typedef union	_DBA_INDICES_FLAGS {
	DBA_INDICES_BITS	bits;
	ULONG				mask;
} DBA_INDICES_FLAGS;

typedef	union	_INDICES_DATE
{
	struct	
	{
		BYTE	century;
		BYTE	year;
		BYTE	month;
		BYTE	day;
	}	date;
	ULONG	lDate;
}	INDICES_DATE;


typedef struct _DBA_INDICES_RECORD { 		// Indices
	DBA_KEY				item;
	USHORT				priceType;
	USHORT				divisorPriceType;		// price type of the divisor
	USHORT				session;
	USHORT				indexType;
	LONGLONG			openSum; 				// Sum of index opens
	LONGLONG			highSum; 				// Sum of index highs
	LONGLONG			lowSum;					// Sum of index lows
	LONGLONG			closeSum; 				// Sum of index closes
	LONGLONG			lastSum; 				// Sum of index last sales
	ULONG				divisor; 				// Index divisor
	LONG				tick; 					// Index tick
	LONG				openValue;				// Index intra day open value
	LONG				highValue;				// Index intra day high value
	LONG				lowValue;				// Index intra day low value
	LONG				lastValue;				// Index intra day last value
	LONG				closeValue; 			// Index intra day close value
	DBA_INDICES_FLAGS	flags;
	INT					indexCount; 			// # of instruments in index
	INT					openCount;				// # of instruments opened
	INT					closeCount;				// # of instruments closed
	INT					reserved[4];
	INDICES_DATE		updateDate;
	CHAR				szIndexName[64];		// Name of the Index
} DBA_INDICES_RECORD;

#define DBA_RECORD_42	DBA_INDICES_RECORD

//------------------------------//
// 	 DBS Statistics record.   //
//------------------------------//

typedef struct _DBA_STATISTICS_BITS {
	BIT_FIELD		volumeChanged		:1;
	BIT_FIELD		advancesChanged		:1;
	BIT_FIELD		upVolumeChanged		:1;
	BIT_FIELD		declinesChanged		:1;
	BIT_FIELD		downVolumeChanged	:1;
	BIT_FIELD		issuesChanged		:1;
	BIT_FIELD		tickChanged 		:1;
	BIT_FIELD		trinChanged 		:1;
	BIT_FIELD		activesChanged 		:1;
	BIT_FIELD		gainersChanged 		:1;
	BIT_FIELD		losersChanged		:1;
	BIT_FIELD		netGainersChanged   :1;
	BIT_FIELD		netLosersChanged	:1;
	BIT_FIELD		regActivesChanged   :1;
	BIT_FIELD		regionalsValid 	    :1;	// Regionals stats configured.
	BIT_FIELD		lateActivesChanged  :1;
	BIT_FIELD		lateTicksChanged	:1;
	BIT_FIELD		lateValid			:1;
	BIT_FIELD		reserved2			:14;
} DBA_STATISTICS_BITS;

typedef union _DBA_STATISTICS_FLAGS {
	DBA_STATISTICS_BITS	bits;
	ULONG 				mask;
} DBA_STATISTICS_FLAGS;

typedef struct _DBA_TOP15_ITEM {
	DBA_KEY				item;
	LONG					value;
} DBA_TOP15_ITEM;

typedef struct _DBA_TOP15NET_ITEM {
	DBA_KEY				item;
	LONG				value;
	SHORT				priceType;
	USHORT				displayPriceType;
} DBA_TOP15NET_ITEM;


typedef struct _DBA_STATISTICS_RECORD { 			// Statistics
	DBA_KEY				item;
	LONGLONG			volume;						// exchange volume
	ULONG				advances;					// exchange advances
	LONGLONG			upVolume;					// exchange up volume
	ULONG				declines;					// exchange declines
	LONGLONG			downVolume; 				// exchange down volume
	ULONG				issues;						// issues traded
	LONG				tick; 						// exchange tick
	LONG				trin; 						// exchange trin
	DBA_TOP15_ITEM		actives[15];				// 15 most active instruments
	DBA_TOP15_ITEM		gainers[15];				// 15 biggest gainers
	DBA_TOP15_ITEM		losers[15]; 				// 15 biggest losers
	USHORT				activeIndex;				// index of lowest active
	USHORT				gainersIndex;				// index of lowest gainer
	USHORT				losersIndex;				// index of highest loser
	USHORT				reserved1;
	DBA_STATISTICS_FLAGS	flags;						// bit field flags
	USHORT				netGainersIndex;			// index of lowest gainer
	USHORT				netLosersIndex;			// index of highest loser
	DBA_TOP15NET_ITEM	netGainers[15];			// 15 biggest net change gainers
	DBA_TOP15NET_ITEM	netLosers[15]; 			// 15 biggest net change losers
	DBA_TOP15_ITEM		regActives[15];			// 15 most active regional instruments
	USHORT				regActiveIndex;			  // index of lowest active
	DBA_TOP15_ITEM		lateActives[15];		// 15 most active late traded instruments
	DBA_TOP15_ITEM		lateTicks[15];		// 15 most active late traded instruments
	USHORT				lateActiveIndex;			// index of lowest late traded active
	USHORT				lateTicksIndex;			// index of lowest late traded active
	USHORT				session;
	USHORT				fill;
	USHORT				row;
	USHORT				column;
	UCHAR 				reserved2[32];
} DBA_STATISTICS_RECORD;

#define DBA_RECORD_43	DBA_STATISTICS_RECORD

//typedef struct _WT_ITEM_DBA {
//	USHORT	type;
//	UCHAR 	symbol[14];
//	UCHAR 	countryCode[2];
//	UCHAR 	exchangeCode[2];
//} WT_ITEM_DBA;					// 20 bytes

//----------------------------//
// Symbol translation record. //
//----------------------------//
typedef struct _TRANSLATE_BITS_DBA {
	BIT_FIELD		fDeleted 		:1; // Processing for this country enabled.
	BIT_FIELD		fNoAutoDelete	:1;
	BIT_FIELD		fLocked			:1;
	BIT_FIELD		fChanged		:1;
	BIT_FIELD		reserved 		:1;
} TRANSLATE_BITS_DBA;

typedef union	_TRANSLATE_FLAGS_DBA {
	TRANSLATE_BITS_DBA	bits;
	USHORT 				mask;
} TRANSLATE_FLAGS_DBA;

typedef union	_TRANSLATE_DATE
{
	struct
	{
		BYTE			century;			// century
		BYTE			year;				// year
		BYTE			month;				// month
		BYTE			day;				// day
	}	date;
	ULONG	ldate;
}	TRANSLATE_DATE;


typedef struct _TRANSLATE_KEY_DBA {				 // Symbol Translation Key
	USHORT      source;
//	WT_ITEM_DBA	item;
	DBA_KEY		item;
} TRANSLATE_KEY_DBA;

typedef struct _TRANSLATE_KEY_DBA_RECORD {		 // Symbol Translation
	TRANSLATE_KEY_DBA  key;
//	WT_ITEM_DBA		     outItem;
	DBA_KEY				outItem;
	USHORT				priceType;
	USHORT				displayPriceType;
	USHORT				row;
	USHORT				column;
	CHAR				description[40];
	TRANSLATE_DATE		updDate; 
	SHORT				strikeScale; 
	SHORT				session; 
	TRANSLATE_FLAGS_DBA	flags;
} TRANSLATE_KEY_DBA_RECORD;

#define DBA_RECORD_45   TRANSLATE_KEY_DBA_RECORD

//---------------------------//
// 	  Country Record		  //
//---------------------------//

typedef struct _COUNTRY_KEY_DBA {
	UCHAR 		countryCode[2];
} COUNTRY_KEY_DBA;

typedef struct _COUNTRY_BITS_DBA {
	BIT_FIELD		fEnabled 		:1; // Processing for this country enabled.
	BIT_FIELD		fCompExchange	:1; // Country has only 1 exchange.
	BIT_FIELD		fDeleted		:1; // Country has only 1 exchange.
	BIT_FIELD		reserved 		:13;
	BIT_FIELD		fReserved		:16;
} COUNTRY_BITS_DBA;

typedef union	_COUNTRY_FLAGS_DBA {
	COUNTRY_BITS_DBA	bits;
	ULONG 				mask;
} COUNTRY_FLAGS_DBA;

typedef union	_COUNTRY_DATE
{
	struct
	{
		BYTE	century;
		BYTE	year;
		BYTE	month;
		BYTE	day;
	}			date;
	ULONG		lDate;
}	COUNTRY_DATE;


typedef struct _COUNTRY_RECORD_DBA {
	COUNTRY_KEY_DBA 	key;
	COUNTRY_FLAGS_DBA	flags;
	UCHAR 				countryName[30];
	UCHAR 				primaryCurrencyName[30];
	UCHAR 				primaryCurrencyCode[3];
	UCHAR 				bridgeCountryCode;
	USHORT				bridgeCurrencyCode;
	USHORT				continentCode;
	USHORT				hoursFromGMT;
	USHORT				minutesFromGMT;
	COUNTRY_DATE		deleteDate;
	ULONG				reserved[9];
} COUNTRY_RECORD_DBA;
/*
typedef struct _COUNTRY_RECORD_DBA {
	DBA_KEY		item;
	USHORT		fEnabled;
	CHAR		countryName[30];
	CHAR		primaryCurrencyName[30];
	CHAR		primaryCurrencyCode[3];
	CHAR		bridgeCountryCode;
	USHORT		bridgeCurrencyCode;
	USHORT		continentCode;
	USHORT		hoursFromGMT;
	USHORT		minutesFromGMT;
	ULONG		reserved[10];
} COUNTRY_RECORD_DBA;
*/
#define DBA_RECORD_46   COUNTRY_RECORD_DBA

//-----------------------------//
//  Primary Exchange Record	//
//-----------------------------//

typedef struct _PRIMARY_EXCHANGE_KEY_DBA {
	USHORT		type;
	UCHAR 		countryCode[2];
} PRIMARY_EXCHANGE_KEY_DBA;
typedef union	_PRIMARY_EXCHANGE_FLAGS_DBA {
	struct
	{
		BIT_FIELD		fDeleted;
	}					bits;
	ULONG 				mask;
} PRIMARY_EXCHANGE_FLAGS_DBA;

typedef union	_PRIMARY_EXCHANGE_DATE
{
	struct
	{
		BYTE	century;
		BYTE	year;
		BYTE	month;
		BYTE	day;
	}			date;
	ULONG		lDate;
}	PRIMARY_EXCHANGE_DATE;


typedef struct _PRIMARY_EXCHANGE_RECORD_DBA {
	PRIMARY_EXCHANGE_KEY_DBA	key;
	UCHAR 						primaryMarkets[120][2];
	PRIMARY_EXCHANGE_FLAGS_DBA	flags;
	PRIMARY_EXCHANGE_DATE		deleteDate;
	UCHAR						reserved[4];
} PRIMARY_EXCHANGE_RECORD_DBA;

#define DBA_RECORD_48 PRIMARY_EXCHANGE_RECORD_DBA

#if defined (TOWER_INCL)
#define DBS_RECORD_48 DBA_RECORD_48
#endif

//------------------------//
// 	 Exchange Record	  //
//------------------------//

typedef struct _EXCHANGE_KEY_DBA {
	USHORT	type;
	UCHAR 	countryCode[2];
	UCHAR 	exchangeCode[2];
} EXCHANGE_KEY_DBA;

typedef struct _EXCHANGE_BITS_DBA{
	BIT_FIELD		fEnabled 				:1;
	BIT_FIELD		fVolumeCumulative		:1;
	BIT_FIELD		fPrimExchIsBbo 			:1;
	BIT_FIELD		fCalcMarketStats		:1;
	BIT_FIELD		fCalcTopTen 			:1;
	BIT_FIELD		fFixTrades				:1;
	BIT_FIELD		fFilterTrades			:1;
	BIT_FIELD		fFilterQuotes			:1;
	BIT_FIELD       fzeroBidAsk				:1;
	BIT_FIELD		fnoOneSideQuotes		:1;
	BIT_FIELD		fFilterHighLow 			:1;
	BIT_FIELD		fCheckTradeTimes		:1;
	BIT_FIELD		fCalcRegTopTen 			:1;
	BIT_FIELD		fCalcLateTopTen 		:1;
	BIT_FIELD		fDeleted				:1;
	BIT_FIELD		fReserved				:1;
	BIT_FIELD		fReserved2				:16;
} EXCHANGE_BITS_DBA;

typedef union	_EXCHANGE_FLAGS_DBA {
	EXCHANGE_BITS_DBA	bits;
	ULONG 				mask;
} EXCHANGE_FLAGS_DBA;

typedef union	_EXCHANGE_DATE
{
	struct
	{
		BYTE	century;
		BYTE	year;
		BYTE	month;
		BYTE	day;
	}			date;
	ULONG		lDate;
}	EXCHANGE_DATE;



typedef struct _EXCHANGE_CODE_DBA {
	UCHAR 	exchangeCode[2];
} EXCHANGE_CODE_DBA;

typedef struct _EXCHANGE_RECORD_DBA {
	EXCHANGE_KEY_DBA	key;
	EXCHANGE_FLAGS_DBA	flags;
	CHAR				hoursFromGMT;
	CHAR				minutesFromGMT;
	UCHAR 				exchangeName[30];
	USHORT 				session;
	UCHAR 				quoteProcess;
	UCHAR 				tradeProcess;
	USHORT				tradeSource;
	USHORT				quoteSource;
	USHORT				summSource;
	USHORT				tradeDataSource;
	USHORT				quoteDataSource;
	USHORT				summDataSource;
	union
	{
		struct					//		old unused
		{
			USHORT				tradeRestrictions;
			USHORT				quoteRestrictions;
			USHORT				summRestrictions;
		};
		struct					//      new
		{
			EXCHANGE_DATE	deleteDate;
			USHORT			reserved;
		};
	};
	
} EXCHANGE_RECORD_DBA;

/*
typedef struct _EXCHANGE_RECORD_DBA {
	DBA_KEY      item;
	BIT_FIELD	fEnabled			:1;
	BIT_FIELD	fVolumeCumulative	:1;
	BIT_FIELD	fPrimExchIsBbo		:1;
	BIT_FIELD	fCalcMarketStats	:1;
	BIT_FIELD	fCalcTopTen			:1;
	BIT_FIELD	fFixTrades			:1;
	BIT_FIELD	fFilterTrades		:1;
	BIT_FIELD	fFilterQuotes		:1;
	BIT_FIELD	fzeroBidAsk			:1;
	BIT_FIELD	fnoOneSideQuotes	:1;
	BIT_FIELD	fReserved			:6;
	CHAR        hoursFromGMT;
	CHAR        minutesFromGMT;
	CHAR        exchangeName[30];
	UCHAR		quoteProcess;
	UCHAR		tradeProcess;
	SHORT       forceColumn;
	USHORT      delayTime;
	USHORT      summSource;
	USHORT      tradeDataSource;
	USHORT      quoteDataSource;
	USHORT      summDataSource;
	USHORT      tradeRestrictions;
	USHORT      quoteRestrictions;
	USHORT      summRestrictions;
} EXCHANGE_RECORD_DBA;
*/
#define DBA_RECORD_47   EXCHANGE_RECORD_DBA

typedef	struct _DBA_RECORD_50_1 {
	DBA_KEY		item;
	USHORT		classSequence;				    /* sequence number within the class */
	USHORT		recordType;						/* record type   = 1 				*/
	USHORT		claseType; 						/* class type i.e 1					*/
	CHAR		name[32]; 							/* company name						*/
	LONG		shortTermInterest; 				/* short term interest rate if any	*/
	USHORT		subCount;						/* number of sub records following	*/
} DBA_RECORD_50_1;

typedef struct _DBA_RECORD_50_2 {
	DBA_KEY		item;
	USHORT		classSequence; 				/* sequence number within the class */
	USHORT		recordType; 				/* record type = 2					*/
	CHAR		underlyingOption[5]; 		/* underlying option symbol			*/
	CHAR		type; 						/* type ?							*/
	CHAR		primaryExchangeCode[2]; 	/* primary exchange of the options	*/
	USHORT		cycles[8];					/* 8 option cycles					*/
	USHORT		marketMask; 				/* market mask 						*/
	USHORT		contractSize;				/* number of units in a contract 	*/
} DBA_RECORD_50_2;

typedef struct _DBA_RECORD_50_3 {
	DBA_KEY		item;
	USHORT		classSequence;				/* sequence number within the class */  
	USHORT		recordType;					/* record type = 3					*/
	USHORT		strikes[26];				/* 26 possible strike values		*/
} DBA_RECORD_50_3;

typedef struct _DBA_RECORD_50_4 {
	DBA_KEY		item;
	USHORT		classSequence;				/* sequence number within the class */
	USHORT		recordType;					/* record type = 4					*/
	CHAR		underlyingStockSymbol[11];	/* underlying stock symbol 			*/
	CHAR		primaryExchangeCode;		/* primary exchange of the stock 	*/
	USHORT		sharesPerContract;			/* shares per contract				*/
	USHORT		volatilities[8]; 			/* 8 volatilities 					*/
	USHORT		frequency; 					/* dividend frequency				*/
	CHAR		xDividendCentury;			/* century of the ex-dividend date	*/
	CHAR		xDividendYear;				/* year of the ex-dividend date		*/
	CHAR		xDividendMonth; 			/* month of the ex-dividend date 	*/
	CHAR		xDividendDay;				/* day of the ex-dividend date		*/
	USHORT		xDividend; 					/* ex-dividend amount 3 decimal		*/
	CHAR		sDividendCentury;			/* century of any special dividend	*/
	CHAR		sDividendYear;				/* year of any special dividend		*/
	CHAR		sDividendMonth; 			/* month of any special dividend 	*/
	CHAR		sDividendDay;				/* day of any special dividend		*/
	USHORT		sDividend; 					/* amount of any special dividend	*/
} DBA_RECORD_50_4;

typedef struct _DBA_RECORD_50_5 {
	DBA_KEY		item;
	USHORT		classSequence;				/* sequence number within the class */ 
	USHORT		recordType;					/* record type						*/                      
	CHAR		monthCode;	                /* month letter of the exception	*/
	CHAR		strikeCode;	                /* strike letter of the exception	*/
	USHORT	strike;							/* strike value for this exception	*/
} DBA_RECORD_50_5;

typedef union _DBA_RECORD_50 {
	DBA_RECORD_50_1	definition; 			/* Option data base 50				*/
	DBA_RECORD_50_2	option;					/* Data base 50 sub-type 2 			*/
	DBA_RECORD_50_3	strike;					/* Data base 50 sub-type 3 			*/
	DBA_RECORD_50_4	stock;					/* Data base 50 sub-type 4 			*/
	DBA_RECORD_50_5	exception;				/* Data base 50 sub-type 5 			*/
} DBA_RECORD_50;

#define DATE51_DAY		0x001f
#define DATE51_MONTH	0x01e0
#define DATE51_YEAR		0xfe00

typedef struct _DATE_51 {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	day  	:5;
	BIT_FIELD	month	:4;
	BIT_FIELD	year 	:7;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	year 	:7;
	BIT_FIELD	month	:4;
	BIT_FIELD	day  	:5;
#endif
} DATE_51;

#define BPT_32 			0			// Bridge format price types.
#define BPT_64 			2
#define BPT_256			3
#define BPT_DECIMAL_0	8
#define BPT_DECIMAL_1	9
#define BPT_DECIMAL_2	10
#define BPT_DECIMAL_3	11
#define BPT_DECIMAL_4	12
#define BPT_DECIMAL_5	13
#define BPT_DECIMAL_6	14
#define BPT_DECIMAL_7	15
#define BPT_DECIMAL_8	16
#define BPT_DECIMAL_9	17

typedef struct _DBA_RECORD_51 {
	DBA_KEY		item;
	USHORT		spCode;								/* S&P code 			*/
	USHORT		high; 								/* 2 year High 			*/
	USHORT		low;								/* 2 year Low			*/
	DATE_51		date; 								/* x-dividend date		*/
	USHORT		oldDiv;						   
	USHORT		frequency;							/* freq. of divid.		*/
	SHORT		earnings;
	LONG		shares;								/* Shares outstanding	*/
	USHORT		volatility;
	USHORT		volume;
	CHAR		name[36];							/* Company Name			*/
	USHORT		priceType;							/* Bridge Format type	*/
	USHORT		padding;
	ULONG 		dividend;							/* dividend amount		*/
	ULONG		trailDividend;
} DBA_RECORD_51;

#define MRK56_CBOT		1
#define MRK56_CME		2
#define MRK56_CEC		3
#define MRK56_KCBOT		4
#define MRK56_MIDAM		5
#define MRK56_MINN		6
#define MRK56_NYMEX		7
#define MRK56_COMEX		8
#define MRK56_WGE		9
#define MRK56_AITK		10
#define MRK56_EMINI		11
#define	MRK56_SPCASHIND	12
#define MRK56_ALL		99

typedef 	struct _DATE_56 {
	BYTE	century;
	BYTE	year;
	BYTE	month;
	BYTE	day;
} DATE_56;

typedef	struct _DBA_RECORD_56_0 {
	DBA_KEY 	item;
	USHORT 		classSequence;    			/* sequence number within the class */
	USHORT 		recordType;					/* record type   = 0 				*/
	ULONG		cdRates[5];
	ULONG		brokerCall;
	ULONG		shortStock;
	DATE_56		tradeDate;
	DATE_56		calcDate;
} DBA_RECORD_56_0;

typedef	struct _DBA_RECORD_56_1 {
	DBA_KEY 	item;
	USHORT 		classSequence; 				/* sequence number within the class */ 
	USHORT 		recordType;					/* record type   = 1 					*/
	CHAR		group[6];                   /* Futures group i.e. grains,metals,currency,etc. */
	USHORT		type;						/* 0 = normal, 1 = interest rate year, 2 = interest rate 90 day */
	CHAR		contractName[32];			/* Description	*/
	USHORT		market;						/* Exchange Number	*/
	USHORT		priceType;					/* Price Type	*/
	DATE_56		expDate;					/* Future Expirations Date	*/
	ULONG		contractSize;               /* Contract Size		*/
	CHAR		contractDescription[12];	/* Contract Units	Descrription	*/
	ULONG		interest;					/* Special Risk free interest rate	*/
	ULONG		volatility;					/* Historic volatility	*/
	CHAR		priceUnits[12];				/* Price units	description */
	USHORT		contractSizeType;			/* Contract size type */
	USHORT		priceToDollars;             /* Price to dollars conversion factor	*/
	UCHAR		exSymCall[2];				/* exchange symbol root for a call */
	UCHAR		exSymPut[2];				/* exchange symbol root for a put */
} DBA_RECORD_56_1;

typedef	struct _DBA_RECORD_56_3 {
	DBA_KEY		item;
	USHORT 		classSequence;              /* sequence number within the class */ 
	USHORT 		recordType;					/* Record Type   = 3 					*/
	DBA_KEY		optionSymbol;
	DATE_56		expDate;					/* Futures Option Expiration Date	*/
	ULONG		contractSize;               /* Contract Size	*/
	USHORT		contract;					/* Option Contract	*/
	USHORT		strikeConversion;			/* Strike Price Conversion Factor	*/
	USHORT		priceType;					/* Price Type	*/
	CHAR		contractDescription[12];	/* Contract Units Description			*/
	UCHAR		exSymCall[2];				/* exchange symbol root for a call */
	UCHAR		exSymPut[2];				/* exchange symbol root for a put */
	USHORT		priceToDollars;             /* Price to dollars conversion factor	*/
} DBA_RECORD_56_3;

typedef union _DBA_RECORD_56 {
	DBA_RECORD_56_0	header;					/* Data base 56 sub-type 0 		  */
	DBA_RECORD_56_1	futClass;				/* Data base 56 sub-type 1 		  */
	DBA_RECORD_56_3	option;					/* Data base 56 sub-type 3 		  */
} DBA_RECORD_56;


typedef struct	_DBA_RECORD_FILTER {		// Filter Table Record (295 bytes)
	DBA_KEY			item;
	USHORT			priceType;		// HyperFeed price type
	DBA_BAR_RECS	price;			// Current price (OHLC)
	DBA_BAR_RECS	price1;			// yesterday price (OHLC)
	DBA_BAR_RECS	price2;			// two days ago price (OHLC)
	DBA_BAR_RECS	price3;			// three days ago price (OHLC)
	DBA_BAR_RECS	price4;			// four days ago price (OHLC)
	DBA_PRICE		bid;
	DBA_PRICE		bid2;			// last bid (bid and time)
	DBA_PRICE		bid3;			// two bids ago (bid and time)
	DBA_PRICE		ask;
	DBA_PRICE		ask2;			// last ask (ask and time)
	DBA_PRICE		ask3;			// two asks ago (ask and time)
	LONG			volumeAvg20; 	// Average volume for last 20 days
	LONG			volumeHigh60;	// Highest volume in last 60 days
	LONG			range9;			// Largest range in last 9 days
	LONG			tickVolume;		// Number of trades today
	LONG			ma10;			// 10 day moving avg (close)
	LONG			ma20;			// 20 day moving avg (close)
	LONG			ma40;			// 40 day moving avg (close)
	LONG			ma50;           // 50 day moving avg (close)
	LONG			ma50p5;			// 50 day moving avg as of the close 5 days ago (close)
	LONG			ma200;			// 200 day moving avg (close)
	LONG			rsi8;			// relative strength index as of yesterday's close
	LONG			wk52High;		// 52 wk high
	LONG			wk52Low;		// 52 wk low
	LONG			mo6High;		// 6 month high
	LONG			mo6Low;			// 6 month low
	LONG			mo2High2P;		// 2 month high as of close 2 days ago
	LONG			mo2Low2P;		// 2 month low as of close 2 days ago
	LONG			mo2High3P;		// 2 month high as of close 3 days ago
	LONG			mo2Low3P;		// 2 month low as of close 3 days ago
	LONG			day10High2P;	// 10 day high as of close 2 days ago
	LONG			day10Low2P;		// 10 day low as of close 2 days ago
	LONG			day20High;      // 20 day high as of close yesterday
	LONG			day20Low;		// 20 day low as of close yesterday
	DATE_56			day20HighDate;	// date of 20 day high
	DATE_56			day20LowDate;	// date of 20 day low
} DBA_RECORD_FILTER;

typedef struct {
	DBA_KEY	item;
	USHORT	priceType;
	LONG	askPrice;
	LONG	day4High;
} DBA_RECORD_BREAKOUT;		// BreakOut filter

typedef struct {
	DBA_KEY	item;
	USHORT	priceType;
	LONG	bidPrice;
	LONG	day4Low;
} DBA_RECORD_BREAKIN;		// BreakIn filter

typedef struct {
	DBA_KEY	item;
	USHORT	priceType;
	LONG	yestHigh;
	LONG	todayOpen;
} DBA_RECORD_GAPUP;			// GapUp filter

typedef struct {
	DBA_KEY	item;
	USHORT	priceType;
	LONG	yestLow;
	LONG	todayOpen;
} DBA_RECORD_GAPDOWN;		// GapDown filter

typedef struct {
	DBA_KEY	item;
	USHORT	priceType;
	LONG	rsi8;
} DBA_RECORD_LONG;			// Long filter

typedef struct {
	DBA_KEY	item;
	USHORT	priceType;
	LONG	rsi8;
} DBA_RECORD_SHORT;			// Short filter

typedef struct {
	DBA_KEY	item;
	USHORT	priceType;
	LONG	volumeRatio;
} DBA_RECORD_EXPLODEUP;		// ExplodeUp

typedef struct {
	DBA_KEY	item;
	USHORT	priceType;
	LONG	volumeRatio;
} DBA_RECORD_EXPLODEDOWN;	// ExplodeDown

typedef struct {
	DBA_KEY item;
	USHORT	priceType;
	LONG    new52WkHigh;
} DBA_RECORD_NEW_HIGHS;		// NewHighs

typedef struct {
	DBA_KEY item;
	USHORT	priceType;
	LONG    new52WkLow;
} DBA_RECORD_NEW_LOWS;		// NewLows

typedef struct {
	DBA_KEY item;
	USHORT	priceType;
	DBA_PRICE newHigh;
} DBA_RECORD_ALERT_HIGHS;	// Highs

typedef struct {
	DBA_KEY item;
	USHORT	priceType;
	DBA_PRICE newLow;
} DBA_RECORD_ALERT_LOWS;	// Lows



typedef struct _DBA_KEY_41 {
	DBA_KEY			dbaKey;			// Constituent Symbol
	DBA_KEY			index;
}	DBA_KEY_41;

typedef	union	_CONSTITUENTS_DATE
{
	struct	
	{
		BYTE	century;
		BYTE	year;
		BYTE	month;
		BYTE	day;
	}	date;
	ULONG	lDate;
}	CONSTITUENTS_DATE;

typedef	union	_CONSTITUENTS_FLAGS
{
	struct
	{
		BIT_FIELD	deleted				:1;
		BIT_FIELD	open				:1;
		BIT_FIELD	dowJones			:1;
		BIT_FIELD	dowJonesComp		:1;
		BIT_FIELD	spreadSpot			:1;
		BIT_FIELD	spreadFrontMonth	:1;
		BIT_FIELD	reserved			:10;
		BIT_FIELD	reserved2			:16;
	}		bits;
	ULONG	mask;
}	CONSTITUENTS_FLAGS;


//	Index constituents record

typedef struct _DBA_RECORD_41 {
	DBA_KEY_41			item;
	LONG				weight;
	USHORT				indexMask;
	USHORT				priceType;
	ULONG				open;
	ULONG				high;
	ULONG				low;
	ULONG				last;
	SHORT				tickTrend;
	USHORT				resv;
	CONSTITUENTS_FLAGS	flags;
	CONSTITUENTS_DATE	updateDate;
} DBA_RECORD_41;


#define	INDEX_CONSTITUENTS_KEY		DBA_KEY_41
#define	INDEX_CONSTITUENTS_RECORD	DBA_RECORD_41


#define SP_KEY_SYMBOL		0				// Symbol
#define SP_KEY_BASE			1				// Base Symbol
#define SP_KEY_NAME			2				// Company Name

#define VD1_PRIMEXCH_MASK	0xf800			/* prim exch mask 0=none,1=A		*/
#define VD1_X_DISTRIBUTION	0x0020
#define VD1_X_DIVIDEND		0x0010

#define VD2_VOLUME_MASK 	0x0c00
#define VD2_VOLUME_UNIT 	0x0000
#define VD2_VOLUME_TEN		0x0400
#define VD2_VOLUME_HUNDRED  0x0800
#define VD2_VOLUME_THOUSAND 0x0c00

#define VD3_DIVFREQ_MASK	0x000f
#define VD3_DIVFREQ_NONE	0x0000
#define VD3_DIVFREQ_QUART	0x0001
#define VD3_DIVFREQ_SEMI	0x0002
#define VD3_DIVFREQ_ANNUAL  0x0003
#define VD3_DIVFREQ_MONTHLY 0x0004

#define VD3_CALLED			0x0008			/* called bond	(not used )	  */
#define VD3_CUMUL_REDEEMED	0x0010			/* bonds=redeemed,pref=cumulative */

/* Bridge Volume Types	*/

#define BVT_UNITS 		0
#define BVT_TENS		1
#define BVT_HUNDREDS 	2
#define BVT_THOUSANDS	3

typedef struct _SP_DATE {
	BYTE		century;
	BYTE		year;
	BYTE		month;
	BYTE		day;
} SP_DATE;

typedef struct _SP_UPDATE_CTRL_BITS {

#if defined( PQTENDIAN_LITTLE )

	USHORT		extUpdate		:1;
	USHORT		mgUpdate		:1;
	USHORT		mg2Update		:1;
	USHORT		telekUpdate		:1;
	USHORT		partUpdate		:1;
	USHORT		partReserved	:3;
	USHORT		idsiDivUpdate	:1;
	USHORT		idsiHiLoUpdate	:1;
	USHORT		sharesAdjusted	:1;
	USHORT		unused			:4;
	USHORT		isNotEquity		:1;
#elif defined( PQTENDIAN_BIG )
	USHORT		isNotEquity		:1;
	USHORT		unused			:4;
	USHORT		sharesAdjusted	:1;
	USHORT		idsiHiLoUpdate	:1;
	USHORT		idsiDivUpdate	:1;
	USHORT		partReserved	:3;
	USHORT		partUpdate		:1;
	USHORT		telekUpdate		:1;
	USHORT		mg2Update		:1;
	USHORT		mgUpdate		:1;
	USHORT		extUpdate		:1;
#endif

} SP_UPDATE_CTRL_BITS;

typedef union _SP_DBA_UPDATE_CTRL {
	SP_UPDATE_CTRL_BITS bits;
	USHORT				mask;
}	SP_DBA_UPDATE_CTRL;

typedef struct _VEHICLE_DATA {
	USHORT		divFreq;				/* Number of dividends per year		*/
	USHORT		vehicleFlag1; 			/* VD1_* flags.						*/
	USHORT		vehicleFlag3; 			/* VD3_* flags.						*/
	USHORT		priceType; 				/* prtype for prices in this record	*/
	USHORT		securityCategory;		/* ISE Security Category 0 = ALPHA.	*/
	USHORT		beta;					/* Beta (0.000) (changed from volatility 11/30/99 */
	USHORT		vehicleFlag2;			
	SP_DBA_UPDATE_CTRL updateCtrl;
	USHORT		divFormat; 				/* 1=1 decimal,2 = 2 decimals etc.	*/
	USHORT		exdDivAmount; 			/* amount of last dividend			*/
	USHORT		spcDivAmount; 			/* amount of special dividend		*/
	USHORT		matDivAmount; 			/* amount of dividends to Maturity	*/
	SP_DATE		exdDivDate;				/* date of last dividend			*/
	SP_DATE		spcDivDate;				/* date of special dividend			*/
	SP_DATE		maturityDate; 			/* date of dividends to Maturity	*/
	SP_DATE		lastSaleDate; 			/* date of last sale.				*/
	ULONG 		reservedLA;				
	LONG		closePrice;				/* yesterday's closing price. 		*/
	LONG		historicalHigh;			/* 52 week High.					*/
	LONG		historicalLow;			/* 52 week Low.						*/
	ULONG 		yestVolume;				/* yesterday's total volume			*/
	LONG		openInterest; 			/* yesterday's open Interest.		*/
	ULONG 		sharesOutstanding;		/* total shares outstanding.		*/
	ULONG 		contractSize; 			/* sh/ctract(op) contractsize(fut)	*/
	LONG		strikePrice;			/* option strike price.				*/
	ULONG 		reservedLB;																
} VEHICLE_DATA;	/* 80 bytes  */

#define UD1_TYPE_MASK		0x0007
#define UD1_TYPE_EQUITY 	0x0001
#define UD1_TYPE_FOREX		0x0002
#define UD1_TYPE_NARROW 	0x0003
#define UD1_TYPE_BROAD		0x0004
#define UD1_TYPE_TBOND		0x0005

typedef struct _UNDERLYING_DATA {
	CHAR		name[36];				/* Underlying Name.					*/
	CHAR		desc[12];				/* Description - (futures)			*/
	USHORT		epsFormat; 				/* 1=1 decimal,2=2 decimals,etc		*/
	SHORT 		eps; 					/* Earnings Per Share.				*/
	USHORT		spCode; 				/* S&P Industry Code				*/
	USHORT		flag1;					/* Flag word 1 UD1_* values.		*/
#if defined (TOWER_INCL)
  	CHAR		column; 				// Bridge Entitlement Word.
    CHAR		row; 					// Bridge Entitlement Word.
#else
	USHORT		entitle;				/* Bridge Entitlement Word.			*/
#endif
	CHAR		idNumber[12];			//  cusip, sedol, isin, etc; 
} UNDERLYING_DATA; /* 58 bytes	*/

typedef struct _DBA_RECORD_70 {
	DBA_KEY			item;
	UNDERLYING_DATA	underlying;
} DBA_RECORD_70;

#define	F52_NEW				0x0001
#define	F52_CHANGED 		0x0002
#define	F52_DELETED 		0x0004

#define	F52_IGNORE			0x0008
#define	F52_IGNORE_NAME		0x0010		// previously LOCKED
#define	F52_DIV				0x0020
#define	F52_IGNORE_DIV		0x0020
#define	F52_VLT				0x0040
#define	F52_IGNORE_VLT		0x0040
#define	F52_HIST			0x0080
#define	F52_IGNORE_HIST		0x0080
#define	F52_SHARES			0x0100
#define	F52_IGNORE_SHARES	0x0100
#define	F52_EARN			0x0200
#define	F52_IGNORE_EARN		0x0200
#define	F52_ID				0x0400
#define	F52_IGNORE_ID		0x0400
#define	F52_Z				0x0800
#define	F52_IGNORE_Z		0x0800
#define	F52_IGNORE_PREV		0x0800

#define	F52_NEED_FUND		0x1000
#define	F52_NEED_HIST		0x2000
#define	F52_INCOMPLETE		0x4000

#define F52_SPLIT_APPLIED	0x8000


typedef struct _DBA_RECORD_52 {
	DBA_KEY			item;				/* Symbol			-key 0		 22	*/
	USHORT			flags;				/* F52_* flags					  2	*/
	DBA_KEY			baseItem;			/* Parent Symbol	-key 1		 22	*/
	USHORT			updDate;			/* Date last updated			  2	*/
	VEHICLE_DATA	vehicle;			/*								 80	*/
	UNDERLYING_DATA	underlying;			/*								 58	*/
} DBA_RECORD_52;						/*								186	*/

// Security Profile2 database

typedef struct _DBA72_DATE {
	BYTE	century;
	BYTE	year;
	BYTE	month;
	BYTE	day;
} DBA72_DATE;

#define DBA72_RECTYPE_BASIC		0

typedef struct _DBA72_KEY {
	DBA_KEY		item;							
	USHORT		recordType;
} DBA72_KEY;

typedef struct _DBA72_REC {
	USHORT		highLowPriceType;		// Price type for high/low prices
	UCHAR		estimateEPSFormat;		// # of decimal places in estimatedEPS
	UCHAR		splitFactorFormat;		// # of decimal places in splitFactor
	LONG		highPrice;				// 52 week high price
	LONG		lowPrice;				// 52 week low price
	DBA72_DATE	highPriceDate;			// Date 52 week high price was set
	DBA72_DATE	lowPriceDate;			// Date 52 week low price was set
	LONG		estimatedEPS;			// Consensus estimated earnings for next non-completed quarter.
	LONG		splitFactor;			// Size of the last split
	DBA72_DATE	splitDate;				// Date of the last split
	USHORT		sicCodes[10];			// Standard Industrial Classification codes
	UCHAR		annualDividendFormat;	// # of decimal places in annualDividend
	UCHAR		couponRateFormat;		// # of decimal places in couponRate
	LONG		annualDividend;			// Indicated Annual Dividend rate
	DBA72_DATE	dividendExDate;			// Last quarterly dividend ex-dividend date
	DBA72_DATE	dividendPayDate;		// Last quarterly dividend payment date
	DBA72_DATE	dividendRecordDate;		// Last quarterly dividend record date
	LONG		couponRate;				// Bond coupon rate.
	USHORT		securityCategory;		// Type of security
	UCHAR		filler[26];
} DBA72_BASIC_REC;

typedef union _DBA_RECORD_72_RECS {
	DBA72_BASIC_REC		general;
} DBA72_RECS;

typedef struct _DBA_RECORD_72 {
	DBA72_KEY			key;
	DBA72_RECS			recs;
} DBA_RECORD_72;

#define	NEWS_TYPE_HEADLINE0		0		/* types used to write */
#define	NEWS_TYPE_HEADLINE1		1
#define	NEWS_TYPE_BODYLINE		2
#define	NEWS_TYPE_LASTLINE		3
#define	NEWS_TYPE_CONTINUELINE	4
#define	NEWS_TYPE_NEWSALERT		15

#define	NT_NEWS					0		/* types returned on read	*/
#define	NT_NEWSALERT			1

typedef struct _DBA_TEXT_49 {
	USHORT			length;				/* length of data			*/
	UCHAR			data[252];			/* data						*/
} DBA_TEXT_49;

typedef struct _DBA_ALERT_49 {
	UCHAR			title[4];
	DBA_KEY			symbol;				/* type 0 packet: title		*/
	UCHAR			reserved[2];
	USHORT			serviceCategory;
} DBA_ALERT_49;

typedef union _DBA_DETAIL_49 {
	DBA_TEXT_49		text;
	DBA_ALERT_49	alert;
} DBA_DETAIL_49;
	
typedef struct _DBA_RECORD_49 {
	UCHAR			source;				/* source of news item		*/
	UCHAR			type;				/* type of news item 		*/
	DBA_DETAIL_49	detail;				/* data						*/
} DBA_RECORD_49;

#define PVTYPE_STATS			0 
#define PVTYPE_VOLUMEATPRICE	1 
#define PVTYPE_MONEYFLOWATPRICE	2

typedef struct _PRICE_VOLUME_KEY {
	DBA_KEY			item;
	USHORT			recType;
	ULONG			startTime;
	ULONG			endTime;
} PRICE_VOLUME_KEY;

typedef struct _PRICE_VOLUME_STATS {
	USHORT			priceType;
	UCHAR			unused[2];
	ULONG			totalVolume;
	ULONG			averageWeightedPrice;
} PRICE_VOLUME_STATS;

typedef struct _PRICE_VOLUME_VOLUME {
	LONG			price;
	ULONG			volume;
} PRICE_VOLUME_VOLUME;

#define PV_MAX_VOLUMES 27

typedef struct _PRICE_VOLUME_VOLUMES {
	USHORT					priceType;
	USHORT					numPrices;
	PRICE_VOLUME_VOLUME		volumes[PV_MAX_VOLUMES];
} PRICE_VOLUME_VOLUMES;

typedef struct _PRICE_VOLUME_MONEYFLOW {
	LONG			price;
	LONGLONG		moneyFlow;
} PRICE_VOLUME_MONEYFLOW;

#define PV_MAX_MONEYFLOWS 18

typedef struct _PRICE_VOLUME_MONEYFLOWS {
	USHORT					priceType;
	USHORT					numPrices;
	PRICE_VOLUME_MONEYFLOW	moneyFlows[PV_MAX_MONEYFLOWS];
} PRICE_VOLUME_MONEYFLOWS;

typedef union _PRICE_VOLUME_RECS {
	PRICE_VOLUME_STATS		stats;
	PRICE_VOLUME_VOLUMES	volumes;
	PRICE_VOLUME_MONEYFLOWS	moneyFlows;
} PRICE_VOLUME_RECS;

typedef struct _PRICE_VOLUME_REC {
	PRICE_VOLUME_KEY	key;
	PRICE_VOLUME_RECS	recs;
} PRICE_VOLUME_REC;

typedef struct _PAGE {
	DBA_RECORD_3 SEGPTR	*dataPtrs[1];
} PAGE;

typedef struct _PAGEMAP {
	PAGE SEGPTR	*dataPagePtrs[1];
} PAGEMAP;

#ifndef __DBA_PBLK_MASK_DEFINED
#define __DBA_PBLK_MASK_DEFINED
	#define DBA_PBLK_MASK_REMOTE			0x0001
	#define DBA_PBLK_MASK_ORGREMOTE			0x0002
	#define DBA_PBLK_MASK_STREAMCONTINUE	0x0004
	#define DBA_PBLK_MASK_STREAMNACKED		0x0008
	#define DBA_PBLK_MASK_RAWMODE			0x0010

	typedef struct _DBA_PBLK_BITS {
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

#ifndef __INCL_DBHEADER

#if defined (__IBMC__) || defined (__IBMCPP__) || (_MSC_VER > 800) || defined(__unix)
typedef struct _DBHEADER {
	ULONG		numkeys;
	ULONG		maxkeys;
	USHORT		recordSize;
	USHORT		pointersPerPage;
	PBYTE 		*pPointerTable;
	UCHAR		numIndices;
	UCHAR 		reservedA;
	ULONG		savePosition;
	CHAR		name[64];
	ULONG 		dbMask;
	HMTX		hmtxDb;
	BYTE		*pDataTable;
	ULONG 		hFile;
	USHORT		reservedC;
} DBHEADER;

#else 

typedef struct _DBHEADER {
	ULONG 		numkeys;
	ULONG 		maxkeys;
	USHORT		recordSize;
	USHORT		pointersPerPage;
	PAGEMAP SEGPTR *pagemapPtr;
	SHORT 		dataSelector;
	ULONG 		savePosition;
	CHAR		name[64];
	ULONG 		dbMask;
	DOSFSRSEM	dbSemaphore;
} DBHEADER;

#endif

#define __INCL_DBHEADER
#endif

#define ADFLAG_PASSWORD_CRYPT	0x0001
#define ADFLAG_SERVER			0x0002
#define ADFLAG_LOGIN			0x0004
#define ADFLAG_PASSWORD			0x0008
#define ADFLAG_SET_PASS			0x0010
#define ADFLAG_PRESENT			0x0080
#define ADFLAG_HOSTIP			0x0100
#define ADFLAG_FILEOPEN			0x0200

#define DBA_FILE_CREATE_NEW          1
#define DBA_FILE_CREATE_ALWAYS       2
#define DBA_FILE_OPEN_EXISTING       3
#define DBA_FILE_OPEN_ALWAYS         4
#define DBA_FILE_TRUNCATE_EXISTING   5

typedef struct _DBA_FILE_OPEN {
	CHAR szFileName[128];
	INT	createFlags;
} DBA_FILE_OPEN;

typedef struct _ATTACH_DATA {
	USHORT		mask;
	CHAR		serverName[32];
	UCHAR		loginID[16];
	UCHAR		szPassword[16];
	UCHAR		dbKeyLen[10];
	LONG		sd;								// returned socket descriptor
	CHAR		szHostIP[2][16];				// user IP
	UCHAR		qsVersion[4];					// returned version of quote server of from 99.99.99.99 i.e. 5.81.79.1
	CHAR		szPadding[140];					// pad out to 256 bytes
	DBA_FILE_OPEN fileOpen;					    // used for TYPE_FILE

} ATTACH_DATA;

#ifndef __INCL_DBA_CREATE
#define __INCL_DBA_CREATE

/* PcqDbIO definitions  */

#ifndef __BORLANDC__

typedef BOOL (FAR *PFNPCQDBIO)(USHORT usFunction, PVOID pvOutputBuffer,
	PVOID pvInputBuffer, ULONG fulDbMask);

BOOL PcqDb3IO(USHORT usFunction, PVOID pvOutputBuffer, PVOID pvInputBuffer,	ULONG fulDbMask);
BOOL PcqDb5IO(USHORT usFunction, PVOID pvOutputBuffer, PVOID pvInputBuffer,	ULONG fulDbMask);

#endif

#define DBIO_CALC_RECSIZE                         	0
#define DBIO_PACK_RECORD                          	1
#define DBIO_UNPACK_RECORD                        	2

/*  Not implimented yet. */
//#define DBIO_CALC_KEYSIZE                       	3
//#define DBIO_PACK_KEY                           	4
//#define DBIO_UNPACK_KEY                         	5

/* DBA_TYPE_* values for dbaType  */

#define DBA_TYPE_REDIRECTOR  						0xffff  // Redirector Table.
#define DBA_TYPE_PBLKTABLE   						0xfffe  // Pblk table.
#define DBA_TYPE_PIDTABLE         					0xfffd  // Pblk table.

#define DBA_TYPE_MEMORY                          	0       // Memory resident uncompressed.
#define DBA_TYPE_COMPRESSED              			1       // Memory resident compressed.
#define DBA_TYPE_BTRIEVE                         	2       // Brieve file.
#define DBA_TYPE_FLAT_FILE               			3       // Flat file.
#define DBA_TYPE_DATAFILE                        	4       // Keys-Memory/Data-Disk.
#define DBA_TYPE_NEWS                            	5       // News Database.
#define DBA_TYPE_AGGREGATE			 				6 	  	// Aggregate
#define DBA_TYPE_BTRIEVE_OPTIONS  					7 	  	// Simulate 1100 from Security Profile.
#define DBA_TYPE_HSMEMORY							8		// High speed memory resident
#define DBA_TYPE_TS				                    9       // Time & Sales data
#define DBA_TYPE_HISTORY							10		// Pricing history
#define DBA_TYPE_CORX								11		// Correction Time Series
#define DBA_TYPE_LOG								12		// Log File Type
#define DBA_TYPE_TEXT								13		// Ascii Text
#define DBA_TYPE_FILE								14		// Generic File
#define DBA_TYPE_REGISTRY							15		// Generic Registry

/* DBAREC_* Bit Masks for DBARECORDMASK   */

#define DBAREC_VARIABLE_LEN             			0x0001 //(D) Allow Variable Length Records
#define DBAREC_TRUNCATE_BLANKS  					0x0002 //(D) Truncate Blanks in Var Len Recs
#define DBAREC_PREALLOCATE              			0x0004 //(1) Preallocate 'allocation' pages.
#define DBAREC_COMPRESS                 			0x0008 //(2) Compress the record.
#define DBAREC_KEY_ONLY                 			0x0010 //(D) key only database.
#define DBAREC_FREESPACE_10             			0x0040 //(D) 10% free space on Var Len pages.
#define DBAREC_FREESPACE_20             			0x0080 //(D) 20% free space on Var Len pages.
#define DBAREC_FREESPACE_30             			0x00C0 //(D) 30% free space on Var Len pages.

/* DBAKEY_* Bit Masks for DBAKEYMASK */

#define DBAKEY_STRING								0x0000
#define DBAKEY_ALLOW_DUPLICATES 					0x0001 //(D) This key allows duplicates.
#define DBAKEY_MODIFIABLE               			0x0002 //(D) This key is modifiable.
#define DBAKEY_BINARY                           	0x0004 //(D) This key is a binary Key.
#define DBAKEY_NULL                             	0x0008 //(D) This key has a NULL value.
#define DBAKEY_SEGMENTED                        	0x0010 //(D) This key has another segment.
#define DBAKEY_ALTERNATE                        	0x0020 //(D) Use alternate collating sequence
#define DBAKEY_DESCENDING               			0x0040 //(D) Sort this key in descending order
#define DBAKEY_SUPPLEMENTAL             			0x0080 //        DO NOT USE.
#define DBAKEY_EXTENDED                 			0x0100 //(D) Use the extended key type.
#define DBAKEY_MANUAL                           	0x0200 //

/* DBAKEYEXT_* Values for extendedKeyType */

#define DBAKEYEXT_STRING                          	0
#define DBAKEYEXT_INTEGER                         	1
#define DBAKEYEXT_FLOAT                           	2
#define DBAKEYEXT_DATE                            	3
#define DBAKEYEXT_TIME                            	4
#define DBAKEYEXT_DECIMAL                         	5
#define DBAKEYEXT_MONEY                           	6
#define DBAKEYEXT_LOGICAL                         	7
#define DBAKEYEXT_NUMERIC                         	8
#define DBAKEYEXT_BFLOAT                          	9
#define DBAKEYEXT_LSTRING                        	10
#define DBAKEYEXT_ZSTRING                        	11
#define DBAKEYEXT_UNSIGNED_BINARY        			14
#define DBAKEYEXT_AUTOINCREMENT          			15

/* Definitions for DBARECORDPARMS */

typedef struct _DBARECORDFLAGS {
	BIT_FIELD		variableLen :1;			//(D) Allow Variable Length Records
	BIT_FIELD       truncBlanks :1;         //(D) Truncate Blanks in Var Len Recs
	BIT_FIELD       preallocate :1;         //(1) Preallocate 'allocation' pages.
	BIT_FIELD       compress    :1;         //(2) Compress the record.
	BIT_FIELD       keyOnly     :1;         //(D) key only database.
	BIT_FIELD       reserved    :1;
	BIT_FIELD       freeSpace10 :1;         //(D) 10% free space on Var Len pages.
	BIT_FIELD       freeSpace20 :1;         //(D) 20% free space on Var Len pages.
	BIT_FIELD       unused      :8;
} DBARECORDFLAGS;

typedef union   _DBARECORDMASK {
	DBARECORDFLAGS	recFlags;
	USHORT          recBits;
} DBARECORDMASK;

typedef struct _DBARECORDPARMS {
	USHORT          dbLogRecLen;            //Specify the logical record size
	USHORT          dbPageSize;             //(D) specify the page size.
	USHORT          dbNumberOfKeys;         //(3) number of keys for this DB.
	ULONG           dbNumberOfRecs;         //Set To ZERO.
	DBARECORDMASK   dbRecMask;              //DBAREC_* flags.
	USHORT          reserved2;              //Set To ZERO.
	USHORT          dbAllocation;           //(1) Number of pages to preallocate.
} DBARECORDPARMS;

/* Definitions for DBAKEYPARMS */

typedef struct _DBAKEYFLAGS {
	BIT_FIELD       allowDuplicates :1;     //(D) This key allows duplicates.
	BIT_FIELD       keyModifiabl    :1;     //(D) This key is modifiable.
	BIT_FIELD       keyBinary       :1;     //(D) This key is a binary Key.
	BIT_FIELD       keyNull         :1;     //(D) This key has a NULL value.
	BIT_FIELD       keySegmented    :1;		//(D) This key has another segment.
	BIT_FIELD       sortAlternat    :1;     //(D) Use alternate collating sequence
	BIT_FIELD       sortDescending  :1;     //(D) Sort this key in descending order
	BIT_FIELD       supplemental    :1;     // MUST BE ZERO.
	BIT_FIELD       keyExtended     :1;     //(D) Use the extended key type.
	BIT_FIELD       keyManual       :1;     //(D) Manual Key (must use NULL value).
	BIT_FIELD       unused          :6;
} DBAKEYFLAGS;

typedef union _DBAKEYMASK {
	DBAKEYFLAGS     keyFlags;
	USHORT          keyBits;
} DBAKEYMASK;

typedef struct _DBAKEYPARMS {
	USHORT          keyPosition; 			// Offset in rec of 1st byte of key.
	USHORT          keyLength;              // Length of the key.
	DBAKEYMASK      keyMask;                // DBAKEY_* flags.
	USHORT          reserved1a;             // Set to ZERO.
	USHORT          reserved1b;             // Set to ZERO.
	BYTE            extendedKeyType;        // DBAKEYEXT_* if DBAKEY_EXTENDED.
	BYTE            nullKeyValue;           //
	USHORT          reserved3a;             // Set to ZERO.
	USHORT          reserved3b;             // Set to ZERO.
} DBAKEYPARMS;
                     
/* Definitions for DBAPARMS */

typedef struct _DBATABLEFLAGS {
	BIT_FIELD		saveData	:1;			// Indicates save to disk outstanding.
	BIT_FIELD		noSave		:1;         // DataBase cannot be save to disk;
	BIT_FIELD		noLoad		:1;         // DataBase cannot be loaded from disk.
	BIT_FIELD		noshare		:1;         // DataBase cannot be shared on network.
	BIT_FIELD		readOnly	:1;         // DataBase is Read Only.
	BIT_FIELD		daemon		:1;         // Indicates Daemon is attached.
	BIT_FIELD		hashTable	:1; 		// Indicates optional Hash table used.
	BIT_FIELD		insertAsync	:1;
	BIT_FIELD		password	:1;			// DataBase is password protected.
	BIT_FIELD		maintenance	:1;			// maintenance in progress
	BIT_FIELD		fRemote		:1;			// Remote DataBase
	BIT_FIELD		fSaving		:1;			// save in progress
	BIT_FIELD		fLocked		:1;			// Data Base Locked
	BIT_FIELD		fDuplicates	:1;			// Allows duplicates
	BIT_FIELD		reserved	:2;
} DBATABLEFLAGS;

typedef union   _DBATABLEMASK {
	DBATABLEFLAGS   flags;
	ULONG           bits;
} DBATABLEMASK;

// NOTE: a change to HASHTABLE requires a corresponding
// 		change to HASHASM.asm. DON'T FORGET!!!!

typedef struct _HASHTABLE {
	ULONG 			ulEntries;
	ULONG 			ulNumkeys;
	ULONG 			ulPollWait;
	ULONG 			semInsertPending;
	ULONG 			semInsertPause;
	ULONG 			ulActionWait;
	USHORT			usActionCount;
	USHORT			prtyClass;
	USHORT			prtyDelta;
	USHORT			selCount;
	USHORT			selStart;
	USHORT			usKeyLen;
	USHORT			usEntrySize;
	USHORT			usEntryPerPage;
	USHORT			usCompares;
} HASHTABLE;

#define DBA_TS_VERSION	3

typedef struct _DBATSHEADER {
	USHORT			usLength;
	UCHAR			ucVersion;
	UCHAR			ucDays;
	ULONG			freeBlk;
} DBATSHEADER;

#define DBA_CORX_VERSION	0

typedef struct _DBACORXHEADER {
	USHORT				usLength;
	UCHAR				ucVersion;
	UCHAR				ucDays;
	ULONG				ulTime;
} DBACORXHEADER;

typedef struct _DBA_LOGREC {
	USHORT	version;
	USHORT	severityBase;
	USHORT	severityLevel;
	USHORT	sequence;
	LONG	time;
	LONG	mask;
	LONG	hostIP[2];
	LONG	reserved[8];
	CHAR	progName[ 24 ];
	CHAR	logText[256];
} DBA_LOGREC;

typedef struct _DBA_TEXTREC {
	CHAR	text[256];
} DBA_TEXTREC;
#define MAX_REMOTE_SERVERS	10

typedef struct _DBATABLE {
	CHAR            dbName[20];              // Database Table Name.
	CHAR            dbReserved[2];              
	USHORT          dbNum;                   // Database Table Number(optional);
	USHORT          dbType;                  // DBA_TYPE_* values.
	DBATABLEMASK    dbTableMask;
	USHORT          dbReserved2;
	CHAR            dbFileName[20];          // File Name without path.(or '\\serv')
	CHAR            dbioModule[20];          // DLL name with dbio function.
	CHAR            dbioProc[20];            // dbio procedure name.
	USHORT          dbKeyLen[10];            // Key Length for 10 possible keys.
	USHORT          dbLogRecLen;             // Logical Record Length.
	USHORT          dbPhysRecLen;            // Physical Record Length.
	ULONG           dbMaxRecs;               // Max records this table can grow to.
	USHORT          dbAttachCount;
	CHAR            dbComment[30];
	CHAR            szDataFile[128];
	USHORT          bufLen;                  // buffer length for btrieve STAT.
	DBHEADER        dbHeader;                // Database header structure.
	HASHTABLE		hashTable;
	ULONG			archiveCount;
	ULONG			freeBlk;				//  auxillary storage (used by TS  & Corx) data bases	
	ULONG			dbMaxPtr;				//  used by corx type DB
	ULONG			reserved;
	ULONG 			ulInsDelHash;
	ULONG 			ulInsDelBinary;
	CHAR			szMirrorServer[32];		 // backup server name if any
	CHAR			szRemoteServer[MAX_REMOTE_SERVERS][32];	 // servers for remote databases
	ULONG			remoteServerCount;
} DBATABLE;

#define DBATABLE_RECORD		DBATABLE


typedef struct _DBAPID_RECORD {
	CHAR			szPID[4];
	USHORT			pid;
	CHAR			szProgName[20];
	ULONG			timeAttached;
	CHAR			reserved[4];
} DBAPID_RECORD;  

typedef struct _DBAPARMS {
	CHAR            dbName[20];
	CHAR            dbComment[30];
	USHORT          dbType;
	USHORT          dbNum;
	DBATABLEMASK    dbTableMask;
	ULONG           dbMask;
	ULONG           maxRecs;
	USHORT          dbPhysRecLen;
	CHAR            dbFilePath[64];
	CHAR            dbFileName[20];
	CHAR            dbioModule[20];
	CHAR            dbioProc[20];

#if ( _MSC_VER > 800 ) || defined(__unix)
	ULONG			archiveCount;
	CHAR			szMirrorServer[32];		 // backup server name if any
#endif
	CHAR			szRemoteServer[MAX_REMOTE_SERVERS][32];	
	ULONG			remoteServerCount;
} DBAPARMS;

/* Definitions for DBACREATEPARMS */

typedef struct _DBACREATEPARMS {
	DBAPARMS		dbaParms;
	DBARECORDPARMS	recParms;
	DBAKEYPARMS		keyParms[32];			// room for 32 key segments
} DBACREATEPARMS;

#if !defined( LOGIN_ID_SIZE )
#define LOGIN_ID_SIZE			16
#endif

#if !defined( E_ROWS )
#define E_ROWS					10
#define E_COLS					128
#endif

typedef struct _DBA_LOCAL_LOGIN {
	UCHAR	szLoginID[LOGIN_ID_SIZE];		// Login ID of user
	USHORT  numRows;
	UCHAR   entitleArray[E_ROWS][E_COLS];	// allow for US row
} DBA_LOCAL_LOGIN;

typedef struct _DBA_SYSTEMTIME { 
    WORD wYear; 
    WORD wMonth; 
    WORD wDayOfWeek; 
    WORD wDay; 
    WORD wHour; 
    WORD wMinute; 
    WORD wSecond; 
    WORD wMilliseconds; 
} DBA_SYSTEMTIME; 
 
#define DBA_FILE_ATTRIBUTE_READONLY             0x00000001  
#define DBA_FILE_ATTRIBUTE_HIDDEN               0x00000002  
#define DBA_FILE_ATTRIBUTE_SYSTEM               0x00000004  
#define DBA_FILE_ATTRIBUTE_DIRECTORY            0x00000010  
#define DBA_FILE_ATTRIBUTE_ARCHIVE              0x00000020  
#define DBA_FILE_ATTRIBUTE_ENCRYPTED            0x00000040  
#define DBA_FILE_ATTRIBUTE_NORMAL               0x00000080  
#define DBA_FILE_ATTRIBUTE_TEMPORARY            0x00000100  
#define DBA_FILE_ATTRIBUTE_SPARSE_FILE          0x00000200  
#define DBA_FILE_ATTRIBUTE_REPARSE_POINT        0x00000400  
#define DBA_FILE_ATTRIBUTE_COMPRESSED           0x00000800  
#define DBA_FILE_ATTRIBUTE_OFFLINE              0x00001000  
#define DBA_FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000  

typedef struct _DBA_FILE_INFORMATION {
    DWORD			dwFileAttributes; 
    DWORD			dwVolumeSerialNumber; 
    DWORD			nFileSizeHigh; 
    DWORD			nFileSizeLow; 
    DWORD			nNumberOfLinks; 
    DWORD			nFileIndexHigh; 
    DWORD			nFileIndexLow; 
    DBA_SYSTEMTIME	ftCreationTime; 
    DBA_SYSTEMTIME	ftLastAccessTime; 
    DBA_SYSTEMTIME	ftLastWriteTime; 
} DBA_FILE_INFORMATION; 

typedef struct _DBA_MAX_BUFR_SIZE {
	CHAR		data[256];
} DBA_MAX_BUFR_SIZE;

#endif


// DBA_KEYS all DBAX key types
typedef	union  _DBA_KEYS {
	DBA_KEY				dbaKey;
	DBA_MMKEY			mmKey;
	DV_INDEX_KEY0		dvIndexKey0;
	DV_INDEX_KEY1		dvIndexKey1;
	DV_TEXT_KEY0		dvTextKey0;
	DV_TEXT_KEY1		dvTextKey1;
	DV_TEXT_KEY2		dvTextKey2;
	DV_TEXT_KEY3		dvTextKey3;
	DV_TEXT_KEY4		dvTextKey4;
	DV_TEXT_KEY5		dvTextKey5;
	DBA_KEY52_0 		spKey0;
	DBA_KEY52_1 		spKey1;
	DBA_KEY52_2 		spKey2;
	DBA_KEY52_3 		spKey3;
	DBA_KEY52_4			spKey4;
	DBA_SESSION_KEY		session;
	DBA_HISTORY_KEY		history;
	PCQT1111_KEY		moKey;              // Master option data base keyfs union
	MGUIDE_KEY			mgKey;				// Market guide data base key
	PRICE_VOLUME_KEY	pvKey;				// Price Volume database key
	TRANSLATE_KEY_DBA	translateKey;
	DBA_INFORMATION_KEY	informationKey;		// Information Key Contains Program Stats/Information
	CROSS_REFERENCE_KEY_0	crossReferenceKey0;
	CROSS_REFERENCE_KEY_1	crossReferenceKey1;
	CHAR				maxKey[128];
} DBA_KEYS;

typedef	union _DBA_RECORD {
	DBA_RECORD_3				standard;			/* Standard data base number one	*/
	DBA_RECORD_3				userDB;				/* User Price data base number two	*/
	DBA_RECORD_3				extended;			/* Extended data base number three	*/
	DBA_RECORD_EXTENDED			extendedPrice;
	DBA_RECORD_4				topten;				/* Topten data base number four		*/
	DBA_RECORD_5				mmaker;				/* Market Maker Quotes				*/
	DBA_RECORD_6				newsflags;			/* News Flags data base 			*/
	DBA_TS_REC					priceTS;			/* Price Time of Sales data base	*/
//	DBA_CORX_REC				priceCorx;			/* Price Corrections Data base      TOO BIG */
	DBA_SESSION_REC				session; 			/* Session database					*/
	DBA_HISTORY_REC				history; 			/* History database					*/
//	DBA_RECORD_FILTER			filter;
	DBA_RECORD_36				extdeleted;
	DBA_RECORD_37				mmdeleted;
	DBA_RECORD_39				priceroll;
	DBA_RECORD_41				constituents;		/* index constituents table			*/
	DBA_RECORD_42				indices;				/* indices data base                */
	DBA_RECORD_45				translateKey;		/* symbol translation Database */
	DBA_RECORD_46				country;
	DBA_RECORD_48				primExch;
	DBA_RECORD_47				exchange;
	DBA_RECORD_51				fundamental;		/* Fundamental data base number 51	*/
	DBA_RECORD_50_1				definition;			/* Option fundamental data base 50	*/
	DBA_RECORD_50_2				option;				/* Data base 50 sub-type 2 			*/
	DBA_RECORD_50_3				strike;				/* Data base 50 sub-type 3 			*/
	DBA_RECORD_50_4				stock;				/* Data base 50 sub-type 4 			*/
	DBA_RECORD_50_5				exception;			/* Data base 50 sub-type 5 			*/
	DBA_RECORD_52				securityProfile;	/* Security Profile Data base.		*/
	DBA_RECORD_49				newsDB;				/* 1081 news data base				*/
	DBA_RECORD_56				future;				/* Data base 56						*/
	DBA_RECORD_56_0				futHeader;			/* Data base 56 sub-type 0 			*/
	DBA_RECORD_56_1				futClass;			/* Data base 56 sub-type 1 			*/
	DBA_RECORD_56_3				futOption;			/* Data base 56 sub-type 3 			*/
	DBA_RECORD_57				reference;			/* Reference database				*/
	DBA_RECORD_58				marketGuide;		/* Market Guide database			*/
    DBA_RECORD_69				firstCallEarn;		/* First Call   database			*/
	DBA_RECORD_72				securityProfile2;	/* Security Profile2 Data base.		*/	
	DBA_RECORD_73				dbaOrder;			/* Order Book Data Base             */
	DBA_USER 					user; 				/* Used specific data area 			*/
	DBA_VERSION 				version; 			/* Get version data returned		*/
	DBHEADER 					dbheader;			/* Get Header data returned			*/
	PCQT1400					pcqt1400;			/* Data base 55						*/
	DBA_RECORD_55				cusipSnp;			/* 2 byte aligned data base 55		*/
	ATTACH_DATA 				attach;				/* Attach record layout 			*/
	DBACREATEPARMS				create;				/* Create a data base				*/
	DV_INDEX_REC				dvIndex;			/* Dow Vision Index Record			*/
	PCQT1111 					masterOption;		/* Master Option database			*/
	DBA_RECORD_12				dbaSplitsDat;		/* splits.dat						*/
	PRICE_VOLUME_REC			priceVolume;		/* Price Volume Statistics			*/
	MASTER_FUNDAMENTAL_RECORD	masterFundamental;  /* Master Fundamental Data Base     */
	DBA_LOGREC					dbaLogRec;			/* Log Record Structure             */
	DBA_TEXTREC					dbaTextRec;			/* Generic Text Data				*/
	INFORMATION_RECORD			informationRec;		/* Program stats and information	*/
	CROSS_REFERENCE_RECORD		crossReference;		/* Cross Reference                  */
	DBA_FILE_INFORMATION		dbaFileInformation;	/* File Information Structure       */
	DBA_RECORD_MARKET_QUOTE		marketQuote;		/* Market Quote						*/	
	CHAR						genericData[256];	/* generic data record (anonymous)	*/
} DBA_RECORD;

/* DBAX Function Prototype  */
#if defined (__IBMC__) || defined (__IBMCPP__) 

#define __DBAX__

USHORT  _Pascal dbax(VOID *pblk,
		  		  			USHORT func,
				  			VOID *key,
				  			VOID *buffer,
				  			USHORT keynum );

BOOL StringToKey( CHAR *string, DBA_KEY *dbaKey );
BOOL KeyToString( DBA_KEY *dbaKey, CHAR *pszOutputStr,USHORT cchOutputLen );
BOOL _KeyToString( DBA_KEY *dbaKey, CHAR *pszOutputStr,USHORT cchOutputLen );
BOOL uKeyToString( DBA_KEY *dbaKey, CHAR *pszOutputStr,USHORT cchOutputLen );
#endif
 
#if ( _MSC_VER > 800 ) || defined(__unix) || defined(_WIN32_WCE)
//#ifdef __NT__

#define __DBAX__

ULONG  PASCAL dbax(VOID *pblk,
		  		  			ULONG func,
				  			VOID *key,
				  			VOID *buffer,
				  			ULONG keynum );

ULONG  PASCAL dba(VOID *pblk,
		  		  			ULONG func,
				  			VOID *key,
							INT *pKeyLen,
				  			VOID *buffer,
							INT *pRecLen,
				  			ULONG keynum );

BOOL PASCAL PcqDateToUShort(ULONG firstDate, PCQ_DATE *pPcqDate, USHORT *pDate);
BOOL PASCAL UShortToPcqDate(ULONG firstDate, USHORT pDate, PCQ_DATE *pPcqDate);

int PASCAL YMDToDayOfWeek (unsigned int year, unsigned int month, int day);
void PASCAL JulianToYMD(long jdn, int *yy, int *mm, int *dd);
long PASCAL YMDToJulian (short year, short month, short day);

BOOL StringToKeyW( TCHAR *string, DBA_KEY *dbaKey );
BOOL KeyToStringW( DBA_KEY *dbaKey, TCHAR *pszOutputStr,USHORT cchOutputLen );
BOOL uKeyToStringW( DBA_KEY *dbaKey, TCHAR *pszOutputStr,USHORT cchOutputLen );

BOOL StringToKeyA( CHAR *string, DBA_KEY *dbaKey );
BOOL KeyToStringA( DBA_KEY *dbaKey, CHAR *pszOutputStr,USHORT cchOutputLen );
BOOL uKeyToStringA( DBA_KEY *dbaKey, CHAR *pszOutputStr,USHORT cchOutputLen );

#ifndef PCQW32_EXPORTS
#ifdef UNICODE
#define StringToKey  StringToKeyW
#define KeyToString  KeyToStringW
#define _KeyToString  _KeyToStringW
#define uKeyToString  uKeyToStringW
#else
#define StringToKey  StringToKeyA
#define KeyToString  KeyToStringA
#define _KeyToString  _KeyToStringA
#define uKeyToString  uKeyToStringA
#endif // !UNICODE
#endif // !PCQW32

typedef LONG PASCAL FN_DBACALLBACK(VOID *pblk,
	  ULONG			ulFunc, 
	  DBA_KEY		*dbaKey, 
	  DBA_RECORD	*dbaRec,
	  ULONG			ulKey,
	  DWORD			dwStatus,
	  VOID			*pFilter);
	  
typedef FN_DBACALLBACK *PFN_DBACALLBACK;

typedef LONG PASCAL FN_DBCALLBACK(VOID *pblk,
	  ULONG			ulFunc, 
	  VOID			*pKey,
	  INT			keyLen,
	  VOID			*pRecord,
	  INT			recLen,
	  ULONG			ulKey,
	  DWORD			dwStatus,
	  VOID			*pFilter);
	  
typedef FN_DBCALLBACK *PFN_DBCALLBACK;

typedef LONG PASCAL FN_DBBARCALLBACK(VOID *pblk,
	  ULONG				ulFunc, 
	  DBA_HISTORY_KEY	*dbaKey,
	  INT				keyLen,
	  DBA_HISTORY_REC	*dbaRec,
	  INT				recLen,
	  ULONG				ulKey,
	  DWORD				dwStatus,
	  VOID				*pFilter);
	  
typedef FN_DBBARCALLBACK *PFN_DBBARCALLBACK;

typedef struct _DBA_STREAM_FILTER {
	PFN_DBACALLBACK lpfnDbaProc;		// callBack function to get data
	INT				nCreateFlags;					// Thread Creation Flags
	PVOID			pUser;						// Space for User data
	INT				nUserStatus;					// Space for user information;
	ULONG			ulPositionFunction;			// initial positioning function
	INT				nCount;						// -1 = all
	INT				nProcStatus;		     		// CallBack function status code
	HANDLE			hThread;						// Handle of any created thread
} DBA_STREAM_FILTER;

typedef struct _DB_STREAM_FILTER {
	PFN_DBCALLBACK	lpfnDbProc;			// callBack function to get data
	INT				nCreateFlags;		// Thread Creation Flags
	PVOID			pUser;				// Space for User data
	INT				nUserStatus;		// Space for user information;
	ULONG			ulPositionFunction;	// initial positioning function
	INT				nCount;				// -1 = all
	INT				nProcStatus;		// CallBack function status code
	HANDLE			hThread;			// Handle of any created thread
	ULONG			nRecLen;			// Record length to be used in CallBack Routine
} DB_STREAM_FILTER;	
	
#define		DBA_BFM_MONTHLY		0x00000001
#define		DBA_BFM_QUARTERLY	0x00000002

typedef struct BAR_FILTER_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	monthly		:1;			// monthly
	BIT_FIELD	quarterly	:1;			// quarterly
	BIT_FIELD	reserved	:14;		
	BIT_FIELD	reserved2	:16;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	reserved2	:16;
	BIT_FIELD	reserved	:14;		
	BIT_FIELD	quarterly	:1;			// quarterly
	BIT_FIELD	monthly		:1;			// monthly
#endif
} BAR_FILTER_BITS;

typedef union	BAR_FILTER_FLAGS {
	BAR_FILTER_BITS		bits;
	ULONG				mask;
} BAR_FILTER_FLAGS;

typedef struct _BAR_FILTER {
	PFN_DBBARCALLBACK	lpfnDbBarProc;		// callBack function to get data
	INT					nCreateFlags;		// Thread Creation Flags
	HANDLE				hThread;			// Handle of any created thread
	PVOID				pUser;				// Space for User data
	INT					nUserInfo;			// Space for user information;
	ULONG				nRecLen;			// Record length to be used in CallBack Routine
	INT					nRecCount;			// count of returned records
	INT					nProcStatus;		// CallBack function status code
	ULONG				start;				// start time
	ULONG				stop;				// stop time
	INT					number;				// number of bars to combine
	INT					interval;			// interval in seconds encapsulated within each bar, i.e. 30 minutes, 7 days, etc..
	INT					count;				// max number of bars to return, -1 = all
	BAR_FILTER_FLAGS	flags;
} BAR_FILTER;

typedef struct _DBA_OPTIONS_FILTER_RECORD {
	INT					type;			// 0 = CALL, 1 = PUT
	INT					series;			// number of a series from the MasterOptions database
	INT					month;			// absolute month (Jan=0,Feb=1)
	INT					relativemonth;  // relative month (this month=0,next month=1)
	INT					daystillexp;
	INT					strikePriceType;
	ULONG				strikePrice;
	ULONG				reserved[2];
	DBA_RECORD_EXTENDED	priceRec;
} DBA_OPTIONS_FILTER_RECORD;

#define		DBA_OFM_CALLS				0x00000001
#define		DBA_OFM_PUTS				0x00000002
#define		DBA_OFM_NEAR_MONEY			0x00000004
#define		DBA_OFM_REGIONALS			0x00000008
#define		DBA_OFM_COMPOSITES			0x00000010
#define		DBA_OFM_NEAR_IN				0x00000020
#define		DBA_OFM_SUPRESS_ZERO_OI		0x00000040
#define		DBA_OFM_SUPRESS_ZERO_VOLUME	0x00000080

typedef struct OPTIONS_FILTER_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	calls				:1;			// calls
	BIT_FIELD	puts				:1;			// puts
	BIT_FIELD	inTheMoney			:1;			// in the money only
	BIT_FIELD	regionals			:1;			// regionals
	BIT_FIELD	composites			:1;			// composites
	BIT_FIELD	nearIn				:1;			// near in month
	BIT_FIELD	supressZeroOI		:1;			// supress zero open interest
	BIT_FIELD	supressZeroVolume	:1;			// supress zero volume
	BIT_FIELD	reserved			:8;
	BIT_FIELD	reserved2			:16;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	reserved2			:16;
	BIT_FIELD	reserved			:8;		
	BIT_FIELD	supressZeroVolume	:1;			// supress zero volume
	BIT_FIELD	supressZeroOI		:1;			// supress zero open interest
	BIT_FIELD	nearIn				:1;			// near in month
	BIT_FIELD	composites			:1;			// composites
	BIT_FIELD	regionals			:1;			// regionals
	BIT_FIELD	inTheMoney			:1;			// in the money only
	BIT_FIELD	puts				:1;			// puts
	BIT_FIELD	calls				:1;			// calls

#endif
} OPTIONS_FILTER_BITS;

typedef union	OPTIONS_FILTER_FLAGS {
	OPTIONS_FILTER_BITS	bits;
	ULONG				mask;
} OPTIONS_FILTER_FLAGS;

typedef LONG PASCAL FN_DBOPTIONSCALLBACK( VOID *pblk,
	  ULONG						ulFunc, 
	  DBA_KEY					*dbaKey,
	  INT						keyLen,
	  DBA_OPTIONS_FILTER_RECORD *dbaRec,
	  INT						recLen,
	  ULONG						ulKey,
	  DWORD						dwStatus,
	  VOID						*pFilter);
	  
typedef FN_DBOPTIONSCALLBACK *PFN_DBOPTIONSCALLBACK;

typedef struct _OPTIONS_FILTER {
	PFN_DBOPTIONSCALLBACK	lpfnDbOptionsProc;	// callBack function to get data
	INT						nCreateFlags;		// Thread Creation Flags
	HANDLE					hThread;			// Handle of any created thread
	PVOID					pUser;				// Space for User data
	INT						nUserInfo;			// Space for user information;
	ULONG					nRecLen;			// Record length to be used in CallBack Routine
	INT						nRecCount;			// count of returned records
	INT						nProcStatus;		// CallBack function status code
	INT						months;				// number of months to return
	INT						count;				// max number of bars to return, -1 = all
	OPTIONS_FILTER_FLAGS	flags;
} OPTIONS_FILTER;

typedef struct _DBA_NAME_FILTER_RECORD {
	DBA_RECORD	dbaRec;
} DBA_NAME_FILTER_RECORD;


#define		DBA_NFM_ALL					0x00000001
#define		DBA_NFM_EQUITY				0x00000002
#define		DBA_NFM_OPTION				0x00000004
#define		DBA_NFM_FUTURE				0x00000008
#define		DBA_NFM_FUTURE_OPTION		0x00000010
#define		DBA_NFM_INDEX				0x00000020
#define		DBA_NFM_FOREX				0x00000040
#define		DBA_NFM_BOND				0x00000080
#define		DBA_NFM_MUNI_BOND			0x00000100
#define		DBA_NFM_DEPOSIT_RATE		0x00000200
#define		DBA_NFM_DIRECTION			0x00008000

typedef struct NAME_FILTER_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	all				:1;			// include all names for all instruments types
	BIT_FIELD	equity			:1;			// include equity names
	BIT_FIELD	option			:1;			// include equity options names
	BIT_FIELD	future			:1;			// include futures names
	BIT_FIELD	futureOption	:1;			// include futures options names
	BIT_FIELD	index			:1;			// include index names
	BIT_FIELD	forex			:1;			// include foreign exchange names
	BIT_FIELD	bond			:1;			// include bond names
	BIT_FIELD	muniBond		:1;			// include muni bond names
	BIT_FIELD	depositRate		:1;			// include deposite rate names
	BIT_FIELD	reserved		:5;	
	BIT_FIELD	direction		:1;			// If True, look backward versus forward
	BIT_FIELD	reserved2		:16;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	reserved2		:16;
	BIT_FIELD	direction		:1;			// If True, look backward versus forward
	BIT_FIELD	reserved		:5;
	BIT_FIELD	depositRate		:1;			// include deposite rate names
	BIT_FIELD	muniBond		:1;			// include muni bond names
	BIT_FIELD	bond			:1;			// include bond names
	BIT_FIELD	forex			:1;			// include foreign exchange names
	BIT_FIELD	index			:1;			// include index names
	BIT_FIELD	futureOption	:1;			// include futures options names
	BIT_FIELD	future			:1;			// include futures names
	BIT_FIELD	option			:1;			// include equity options names
	BIT_FIELD	equity			:1;			// include equity names
	BIT_FIELD	all				:1;			// include all names for all instruments types
#endif
} NAME_FILTER_BITS;

typedef union	NAME_FILTER_FLAGS {
	NAME_FILTER_BITS	bits;
	ULONG				mask;
} NAME_FILTER_FLAGS;

typedef LONG PASCAL FN_DBNAMECALLBACK(VOID *pblk,
	  ULONG ulFunc, 
	  CHAR	*nameKey,
	  INT keyLen,
	  DBA_NAME_FILTER_RECORD *dbaRec,
	  INT recLen,
	  ULONG ulKey,
	  DWORD dwStatus,
	  VOID *pFilter);
	  
typedef FN_DBNAMECALLBACK *PFN_DBNAMECALLBACK;

typedef struct _NAME_FILTER {
	PFN_DBNAMECALLBACK		lpfnDbNameProc;			// callBack function to get data
	INT						nCreateFlags;		// Thread Creation Flags
	HANDLE					hThread;			// Handle of any created thread
	PVOID					pUser;				// Space for User data
	INT						nUserInfo;			// Space for user information;
	ULONG					nRecLen;			// Record length to be used in CallBack Routine
	INT						nRecCount;			// count of returned records
	INT						nProcStatus;		// CallBack function status code
	INT						count;				// max number of names to return, -1 = all
	NAME_FILTER_FLAGS		flags;
} NAME_FILTER;

typedef LONG PASCAL FN_DBMULTIGECALLBACK(VOID *pblk,
	  ULONG ulFunc, 
	  VOID *pDbaKey,
	  INT keyLen,
	  DBA_RECORD *dbaRec,
	  INT recLen,
	  ULONG ulKey,
	  DWORD dwStatus,
	  VOID *pFilter);
	  
typedef FN_DBMULTIGECALLBACK *PFN_DBMULTIGECALLBACK;

typedef struct _MULTIGE_FILTER {
	PFN_DBMULTIGECALLBACK	lpfnDbMultiGEProc;			// callBack function to get data
	INT						nCreateFlags;		// Thread Creation Flags
	HANDLE					hThread;			// Handle of any created thread
	PVOID					pUser;				// Space for User data
	INT						nUserInfo;			// Space for user information;
	ULONG					nRecLen;			// Record length to be used in CallBack Routine
	INT						nRecCount;			// count of returned records
	INT						nProcStatus;		// CallBack function status code
	INT						count;				// number of keys to return 
	ULONG					flags;
} MULTIGE_FILTER;

typedef struct _DBA_DEPTH_PRICE {	
	LONG		price;
	ULONG		size;
	LONG		time;
	CHAR		exchangeCode[2];
} DBA_DEPTH_PRICE;

#define DEPTH_ENTRY_ID_SIZE		32

/*		Old 5.81.79 version defines

#define		DBA_DFM_AGGREGATE	0x00000001
#define		DBA_DFM_TOP			0x00000002
#define		DBA_DFM_MARKET		0x00000004		 // return Market Maker data if applicable
#define		DBA_DFM_BOOK		0x00000008		 // return Book data if applicable
#define		DBA_DFM_FRACTIONS	0x00000010       // force price type to fractions (if possible)
#define		DBA_DFM_DECIMALS	0x00000020       // force price type to decimals
#define		DBA_DFM_REPLACE_TOP	0x00000040       // replace market maker data top of book
#define     DBA_DFM_REGIONALS	0x00000080		 // return regionals if applicable

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// db structure for depth queries

#define DEPTH_ENTRY_ID_SIZE		32

typedef struct DEPTH_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	market			:1;			// market maker or price data
	BIT_FIELD	book			:1;			// book data
	BIT_FIELD	listed			:1;			// listed instrument
	BIT_FIELD	bidOpenIndicator:1;			// valid bid
	BIT_FIELD	askOpenIndicator:1;			// valid ask
	BIT_FIELD	regional		:1;			// regional record need to dec interest if add interest enabled
	BIT_FIELD	reserved		:10;
	BIT_FIELD	reserved2		:16;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	reserved2		:16;
	BIT_FIELD	reserved		:10;
	BIT_FIELD	regional		:1			// regional record need to dec interest if add interest enabled
	BIT_FIELD	askOpenIndicator:1;			// valid ask
	BIT_FIELD	bidOpenIndicator:1;			// valid bid
	BIT_FIELD	listed			:1;			// listed instrument
	BIT_FIELD	book			:1;			// book data
	BIT_FIELD	market			:1;			// market maker or price data
#endif
} DEPTH_BITS;
*/

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// db structure for depth queries

#define		DBA_DFM_AGGREGATE			0x00000001
#define		DBA_DFM_TOP					0x00000002
#define		DBA_DFM_MARKET				0x00000004		 // return Market Maker data if applicable
#define		DBA_DFM_BOOK				0x00000008		 // return Book data if applicable
#define		DBA_DFM_LISTED				0x00000010		 // return listed montage
#define		DBA_DFM_BID_OI				0x00000020       // valid Bid
#define		DBA_DFM_ASK_OI				0x00000040       // valid Ask
#define     DBA_DFM_REGIONALS			0x00000080		 // return regionals if applicable
#define     DBA_DFM_ADAP    			0x00000100		 // return ADAP if applicable
#define     DBA_DFM_PRIME   			0x00000200		 // return Prime if applicable
#define     DBA_DFM_NYSE_OPEN			0x00000400		 // return NYSE OpenBook if applicable
#define     DBA_DFM_OMDF				0x00000800		 // return OTC Market Data Feed
#define     DBA_DFM_INTER_MARKET		0x00001000		 // return IM Quotes
#define     DBA_DFM_DEPTH				0x00002000		 // return depth of Book
#define     DBA_DFM_LIQUIDITY_QUOTE		0x00004000		 // return NYSE Liquidity Quote
#define     DBA_DFM_NASDAQ_INSIDE		0x00008000		 // return NASDAQ Inside Quote
#define     DBA_DFM_ALL_MARKET_QUOTE	0x00010000		 // return All Market Quotes Including NYSE Liquidity and NASDAQ Inside Quote

typedef struct DEPTH_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	aggregate		:1;			// aggregate price
	BIT_FIELD	top				:1;			// top of book
	BIT_FIELD	market			:1;			// market maker or price data
	BIT_FIELD	book			:1;			// book data
	BIT_FIELD	listed			:1;			// listed instrument
	BIT_FIELD	bidOpenIndicator:1;			// valid bid
	BIT_FIELD	askOpenIndicator:1;			// valid ask
	BIT_FIELD	regionals		:1;			// regional record need to dec interest if add interest enabled
	BIT_FIELD	adap			:1;			// ADAP (super montage)
	BIT_FIELD	prime			:1;			// Prime (super montage)
	BIT_FIELD	nyse			:1;			// NYSE OpenBook
	BIT_FIELD	omdf			:1;			// OTC Market Data Feed
	BIT_FIELD	interMarket		:1;			// IM Quotes
	BIT_FIELD	depth			:1;			// Depth
	BIT_FIELD	liquidity		:1;			// NYSE Liquidity
	BIT_FIELD	inside			:1;			// NASDAQ Inside
	BIT_FIELD	marketQuote		:1;			// All Market Quotes
	BIT_FIELD	reserved2		:15;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	reserved2		:15;
	BIT_FIELD	marketQuote		:1;			// All Market Quotes
	BIT_FIELD	liquidity		:1;			// NYSE Liquidity
	BIT_FIELD	inside			:1;			// NASDAQ Inside
	BIT_FIELD	depth			:1;			// Depth
	BIT_FIELD	interMarket		:1;			// IM Quotes
	BIT_FIELD	omdf			:1;			// OTC Market Data Feed
	BIT_FIELD	nyse			:1;			// NYSE OpenBook
	BIT_FIELD	prime			:1;			// Prime (super montage)
	BIT_FIELD	adap			:1;			// ADAP (super montage)
	BIT_FIELD	regionals		:1;			// regional record need to dec interest if add interest enabled
	BIT_FIELD	askOpenIndicator:1;			// valid ask	
	BIT_FIELD	bidOpenIndicator:1;			// valid bid
	BIT_FIELD	listed			:1;			// listed instrument
	BIT_FIELD	book			:1;			// book data
	BIT_FIELD	market			:1;			// market maker or price data
	BIT_FIELD	top				:1;			// top of book
	BIT_FIELD	aggregate		:1;			// aggregate price
#endif
} DEPTH_BITS;

typedef union	DEPTH_FLAGS {
	DEPTH_BITS	bits;
	ULONG		mask;
} DEPTH_FLAGS;

typedef struct	_DBA_DEPTH_RECORD {
	DBA_KEY			item;
	DEPTH_FLAGS		flags;
	USHORT			row;
	USHORT			column;
	CHAR			entryID[DEPTH_ENTRY_ID_SIZE];	// 32 bytes
	USHORT			priceType;
	USHORT			quoteIndicator;
	DBA_DEPTH_PRICE	bid;
	DBA_DEPTH_PRICE	ask;
	USHORT			serverData;						// Instance count for regionals
} DBA_DEPTH_RECORD;

typedef union  _DBA_DEPTH_DATA {
	DBA_DEPTH_RECORD	depth;
	DBA_RECORD_EXTENDED	price;
} DBA_DEPTH_DATA;

// valid types 
// DBA_EXTENDED for price records
// DBA_DEPTH

typedef struct _DBA_DEPTH_FILTER_RECORD {
	INT				type;			// # corresponding to type of record returned
	DBA_DEPTH_DATA	data;
} DBA_DEPTH_FILTER_RECORD;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

typedef struct DEPTH_FILTER_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	aggregate	:1;			// aggregate price levels
	BIT_FIELD	top			:1;			// top of book
	BIT_FIELD	market		:1;			// market maker or price data
	BIT_FIELD	book		:1;			// book data
	BIT_FIELD	fractions	:1;			// force price to fractions
	BIT_FIELD	decimal		:1;			// force price to decimals
	BIT_FIELD	replace		:1;			// replace market maker top with calculated top
	BIT_FIELD	regionals	:1;			// regional records
	BIT_FIELD	reserved	:8;		
	BIT_FIELD	reserved2	:16;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	reserved2	:16;
	BIT_FIELD	reserved	:8;
	BIT_FIELD	regionals	:1;			// regional records
	BIT_FIELD	replace		:1;			// replace market maker top with calculated top
	BIT_FIELD	decimal		:1;			// force price to decimals
	BIT_FIELD	fractions	:1;			// force price to fractions
	BIT_FIELD	book		:1;			// book data
	BIT_FIELD	market		:1;			// market maker or price data
	BIT_FIELD	top			:1;			// top of book
	BIT_FIELD	aggregate	:1;			// aggregate price levels
#endif
} DEPTH_FILTER_BITS;

typedef union	DEPTH_FILTER_FLAGS {
	DEPTH_FILTER_BITS	bits;
	ULONG				mask;
} DEPTH_FILTER_FLAGS;

typedef LONG PASCAL FN_DBDEPTHCALLBACK( VOID *pblk,
	  ULONG						ulFunc, 
	  DBA_KEY					*dbaKey,
	  INT						keyLen,
	  DBA_DEPTH_FILTER_RECORD	*dbaRec,
	  INT						recLen,
	  ULONG						ulKey,
	  DWORD						dwStatus,
	  VOID						*pFilter);
	  
typedef FN_DBDEPTHCALLBACK *PFN_DBDEPTHCALLBACK;

typedef struct _DEPTH_FILTER {
	PFN_DBDEPTHCALLBACK	lpfnDbDepthProc;		// callBack function to get data
	INT						nCreateFlags;		// Thread Creation Flags
	HANDLE					hThread;			// Handle of any created thread
	PVOID					pUser;				// Space for User data
	INT						nUserInfo;			// Space for user information;
	ULONG					nRecLen;			// Record length to be used in CallBack Routine
	INT						nRecCount;			// count of returned records
	INT						nProcStatus;		// CallBack function status code
	INT						depth;				// number of prices to return
	INT						count;				// max number to return, -1 = all
	DEPTH_FILTER_FLAGS	flags;
} DEPTH_FILTER;

#endif

#ifndef __DBAX__

#define __DBAX__

USHORT _PCQENTRY dbax( VOID SEGPTR *pblk,
							  USHORT func,
							  VOID SEGPTR *key,
							  VOID SEGPTR *buffer,
							  USHORT keynum );

USHORT CDECL StringToKey( TCHAR *string, DBA_KEY SEGPTR *dbaKey );
USHORT CDECL KeyToString( DBA_KEY SEGPTR *dbaKey, TCHAR *pszOutputStr,	USHORT cchOutputLen );
USHORT CDECL _KeyToString( DBA_KEY SEGPTR *dbaKey, TCHAR *pszOutputStr,	USHORT cchOutputLen );
#endif	

VOID CDECL    left_just( TCHAR *string, USHORT num );

#ifndef INCL_NOPACKING
	#pragma pack() 						 /* Word aligned. */
#endif

#ifdef __cplusplus	// C++ compiler is being used
	#ifndef __unix
		   }
	#endif
#endif

#endif
