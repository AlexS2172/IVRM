
#include <pcqdefs.h>
#include <mguide.h>        
#include <pcqt1111.h>
#include <udjndv.h>			 

#ifndef INCL_DBAX
#define INCL_DBAX

#ifdef __cplusplus									// C++ compiler is being used
	extern "C" {
#endif

#pragma pack(2)						 /* Word aligned. */

#define DBAX_FUNCTION_MASK					0x00ff

#define DBAX_LOCK 							0x8000
#define DBAX_GET_KEY 						0x4000
#define DBAX_INTEREST						0x2000
#define DBAX_ASYNC							0x1000
#define DBAX_INFINITE_WAIT					0xFFFFFFFF

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
#define DBA_STOP								14
#define DBA_GET_VERSION 					15
#define DBA_GET_DBHEADER					16

#define DBA_GET_DIRECT                 17
#define DBA_GET_NEXT_DIRECT            18
#define DBA_GET_PREV_DIRECT            19
#define DBA_GET_PREV 						20
#define DBA_GET_EXTENDED_ERROR			21
#define DBA_CLEAR_ALL_LOCKS				22
#define DBA_CREATE_DB						23
#define DBA_UNLOCK_RECORD					24
#define DBA_DELETE_ALL_KEYS            25
#define DBA_GET_LAST 						26
#define DBA_GET_USER_DIRECTORY			27
#define DBA_GET_DATA_DIRECTORY			28
#define DBA_GET_GREATER_OR_EQUAL			29
#define DBA_GET_LESS_OR_EQUAL				30

#define DBA_GET_FIRST_TS					31
#define DBA_GET_LAST_TS						32
#define DBA_GET_NEXT_TS						33
#define DBA_GET_PREV_TS						34
#define DBA_ADD_TS							35
#define DBA_DELETE_TS						36
#define DBA_INSERT_TS						37
#define DBA_LOGIN								38
#define DBA_LOGOUT							39
#define DBA_GET_LOCAL_LOGIN				40
#define DBA_ADD_INTEREST					41
#define DBA_DEC_INTEREST					42
#define DBA_DELETE_INTEREST				43
#define DBA_GET_EQUAL_TS					44
#define DBA_GET_APPROX_TS					45
#define DBA_GET_NEXT_STREAM				46
#define DBA_GET_PREV_STREAM				47
#define DBA_GET_NEXT_SUB_STREAM			48
#define DBA_GET_NEXT_TS_STREAM			49
#define DBA_GET_PREV_TS_STREAM			50
#define DBA_GET_NEXT_AND_SUB_STREAM    51
#define DBA_TERMINATE_STREAM_FUNCTION  52
#define DBA_WAIT_FOR_STREAM_TERMINATE       53
#define DBA_MAX_FUNCTION					DBA_WAIT_FOR_STREAM_TERMINATE

#define DBA_FLAT_FILE_LOCK 				119
#define DBA_FLAT_FILE_UNLOCK				120

/*
   Database Tables
*/

#define DBA_STANDARD 						1
#define DBA_USER_PRICE 						2
#define DBA_EXTENDED 						3
#define DBA_TOPTEN							4
#define DBA_MARKET_MAKER					5
#define DBA_NEWSFLAGS						6
#define DBA_PRICE_TS							7
#define DBA_SESSION							8
#define DBA_HISTORY							9

#define DBA_NEWS								49
#define DBA_OPTION							50
#define DBA_FUNDAMENTAL 					51
#define DBA_SECURITY_PROFILE				52
#define DBA_EXDIVSPLIT						53
#define DBA_OPTION_INTRADAY				54
#define DBA_1400								55
#define DBA_FUTURE							56
#define DBA_REFERENCE						57
#define DBA_MARKETGUIDE 					58

#define DBA_RESERVED59						59
#define DBA_RESERVED60						60
#define DBA_RESERVED61						61
#define DBA_RESERVED62						62
#define DBA_RESERVED63						63
#define DBA_RESERVED64						64
#define DBA_RESERVED65						65
#define DBA_RESERVED66						66
#define DBA_RESERVED67						67
#define DBA_RESERVED68						68

#define DBA_FIRSTCALLEARN					69

#define DBA_UNDERLYING_DATA				70
#define DBA_MASTER_OPTION 				71

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

#define DBA_REDIRECTOR						1000
#define DBA_PBLKTABLE						1001
#define DBA_PIDTABLE 						1002

/*
  DBAX Error Codes
*/

#define DBA_ERR_NO_ERROR               0
#define DBA_ERR_INV_FUNC_CODE          1
#define DBA_ERR_INV_DB_NUMBER          2
#define DBA_ERR_DB_NOT_LOADED          3
#define DBA_ERR_INIT			            3
#define DBA_ERR_KEY_NOT_FOUND          4
#define DBA_ERR_INV_KEY                5
#define DBA_ERR_EOF                    6
#define DBA_ERR_PCQCOM_RESERVED			7
#define DBA_ERR_UNDEFINED					7
#define DBA_ERR_TIMEOUT			         8
#define DBA_ERR_NO_MORE_CHG_REC        9
#define DBA_ERR_RESERVED_B            10
#define DBA_ERR_SAVE                  11
#define DBA_ERR_DB_LOCKED             12
#define DBA_ERR_KEY_ALREADY_EXISTS    13
#define DBA_ERR_DB_FULL               14
#define DBA_ERR_RESERVED_C            15
#define DBA_ERR_NO_ATTACH             16
#define DBA_ERR_RECORD_CHANGED        17
#define DBA_ERR_REC_LOCKED 			  18
#define DBA_ERR_MEMORY_ERROR			  19
#define DBA_ERR_BAD_ENVIRONMENT		  20
#define DBA_ERR_OUT_OF_HANDLES		  21
#define DBA_ERR_BAD_PBLK				  22
#define DBA_ERR_INV_KEY_LENGTH		  23
#define DBA_ERR_INV_DATA_LENGTH		  24
#define DBA_ERR_FILE_ERROR 			  25
#define DBA_ERR_BUSY			 			  26
#define DBA_ERR_PARTIAL		 			  27
#define DBA_ERR_PASSWORD	 			  28
#define DBA_ERR_POSITIONING			  29
#define DBA_ERR_DUPLICATE_LOGIN		  30
#define DBA_ERR_NO_LOGIN				  31
#define DBA_ERR_SERVER_MAX				  32
#define DBA_ERR_INVALID_LOGIN			  33
#define DBA_ERR_INTEREST				  34

#define DBA_MAX_ERROR					  DBA_ERR_INTEREST

/* Additional Dbai Error Codes  */

#define DBA_ERR_CREATE_REDIRECTOR			1000
#define DBA_ERR_CREATE_PBLKTABLE				1001
#define DBA_ERR_CREATE_SHAREMEM				1002
#define DBA_ERR_STARTUP_DAEMON				1003
#define DBA_ERR_STARTUP_DAEMON_ERROR      1004
#define DBA_ERR_CREATE_BTRV					1005
#define DBA_ERR_INVALID_PARAMETER			1006
#define DBA_ERR_ACCESS_DENIED					1007
#define DBA_ERR_DB_ALREADY_EXISTS			1008
#define DBA_ERR_DUPLICATE_DB_NUMBER       1009
#define DBA_ERR_REDIRECTOR_FULL				1010
#define DBA_ERR_ALLOCATION						1011
#define DBA_ERR_INVALID_DBIO_MODULE       1012
#define DBA_ERR_INVALID_DBIO_FUNC			1013
#define DBA_ERR_INVALID_DBA_TYPE				1014
#define DBA_ERR_PHYSICAL_LEN_ZERO			1015
#define DBA_ERR_INV_DB_NUM						1016
#define DBA_ERR_UNKNOWN_DB_TYPE				1017
#define DBA_ERR_DB_ALREADY_STARTED			1018
#define DBA_ERR_NETLINK							1019
#define DBA_ERR_SERVER_NOT_AVAILABLE      1020
#define DBA_ERR_FATAL_SERVER_ERROR			1021
#define DBA_ERR_OUT_OF_MEMORY					1022
#define DBA_ERR_SERVER_RESTART				1023

#define DBA_ERR_SERVER_BUSY					1024
#define DBA_ERR_SERVER_DISCONNECT			1025
#define DBA_ERR_NET_REQ_NOT_ACCEPTED      1026
#define DBA_ERR_SERVER_NOT_CONNECTED      1027
#define DBA_ERR_SERVER_BAD_PIPE				1028
#define DBA_ERR_SERVER_RESPONSE_TOO_BIG	1029

#define DBA_ERR_LOAD_ERROR						1030
#define DBA_ERR_PIDTABLE_FULL					1031
#define DBA_ERR_FILE_RENAME					1032

#define DBA_ERR_FILE_NOT_FOUND				1033
#define DBA_ERR_INSUFFICIENT_DISK_SPACE	1044
#define DBA_ERR_HIMEM_DRIVER					1045
#define DBA_ERR_HIMEM_IN_USE					1046
#define DBA_ERR_HASH_COLLISION				1047
#define DBA_ERR_SEMAPHORE						1048
#define DBA_ERR_PBLKTABLE_FULL				1049
#define DBA_ERR_BAD_VERSION					1050
#define DBA_ERR_BAD_CHECKSUM					1051
#define DBA_ERR_BAD_PACKETSIZE				1052
#define DBA_ERR_BAD_BYTEORDER					1053
#define DBA_ERR_BAD_PACKET						1054
#define DBA_ERR_PIDTABLE_ACCESS		      1055
#define DBA_ERR_PBLKTABLE_ACCESS		      1056
#define DBA_ERR_REDIRTABLE_ACCESS		      1057


#ifndef __INCL_DBA_NAMES
#define __INCL_DBA_NAMES

#define DBA_DLLNAME								"DBADLL"
#define DBA_DLL32NAME							"DBA32DLL"

#define DBANAME_REDIRECTOR 					"Redirector"
#define DBANAME_PBLKTABLE						"Pblk Table"
#define DBANAME_PIDTABLE						"PID Table"

//===========================================================================//
//                                                                                                                                                                                                //
//                                               DBS Database Record Definitions                                                          //
//                                                                                                                                                                                                //
//---------------------------------------------------------------------------//

#define DBADBIO_DLL                                     "dbio"
#define DBADBIO32_DLL                                   "dbio32"


#define DBANAME_USER										"User Price"
#define DBANAME_EXTENDED                        "Price"
#define DBANAME_TOPTEN                          "Top Ten"
#define DBANAME_MMAKER                          "Market Maker"
#define DBANAME_NEWSFLAGS                       "News Flags"
#define DBANAME_PRICE_TS                        "Time Series Price"
#define DBANAME_SESSION 								"Session"
#define DBANAME_HISTORY 								"History"
#define DBANAME_SECURITY_PROFILE 					"Security Profile"
#define DBANAME_PROFILE                         "Security Profile"
#define DBANAME_FUNDAMENTAL                     "Stock Fundamental"
#define DBANAME_OPTIONS                         "Option Fundamental"
#define DBANAME_MASTER_OPTION                   "Master Option" 
#define DBANAME_COUNTRY                         "Country"
#define DBANAME_EXCHANGE                        "Exchange"
#define DBANAME_PRIMARY_EXCH                    "Primary Exchange"
#define DBANAME_TRANSLATE                       "Translate Key"
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
#define DBANAME_REFERENCE								"Reference"
#define DBANAME_MARKETGUIDE							"Market Guide"
#define DBANAME_FIRSTCALLEARN							"First Call Earnings"

#define DBANAME_DV_CMINDEX								"Cap Markets Index"
#define DBANAME_DV_CMTEXT 								"Cap Markets Text"
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
#define DBANAME_IB_IBINDEX 							"IBES Index"
#define DBANAME_IB_IBTEXT								"IBES Text"

#define DBSNAME_SCHEDULER							   "DBS Schedules"

#define DBAFILE_USER	                           "pcqt1012"
#define DBAFILE_EXTENDED								"pcqt1062"
#define DBAFILE_TOPTEN		                     "pcqt1042"
#define DBAFILE_MMAKER                          "pcqt1052"
#define DBAFILE_NEWSFLAGS  		               "pcqt1072"
#define DBAFILE_PRICE_TS	  		               "pcqt1092"
#define DBAFILE_SESSION 								"pcqt1200"
#define DBAFILE_HISTORY 								"pcqt1210"
#define DBAFILE_PROFILE 								"pcqt1023"
#define DBAFILE_USFUNDAMENT                     "pcqt1021"
#define DBAFILE_INTLFUNDAMENT                   "pcqt1023"
#define DBAFILE_UNDERLYING                      "pcqt1024"
#define DBAFILE_EXCHANGE                        "pcqt1047"
#define DBAFILE_PRIMARY_EXCH                    "pcqt1048"
#define DBAFILE_COUNTRY                         "pcqt1049"
#define DBAFILE_1021DATA                        "pcqt1021"
//#define DBAFILE_NEWS                            "pcqt1081"
#define DBAFILE_NEWS                            "pcqt1082"
#define DBAFILE_OPTIONS                         "pcqt1100"
#define DBAFILE_MASTER_OPTION                   "pcqt1111"
#define DBAFILE_OPTION_INTRADAY                 "pcqt1150"
#define DBAFILE_FUTURES                         "pcqt1300"
#define DBAFILE_1400                            "pcqt1400"
#define DBAFILE_REFERENCE								"pcqt1600"
#define DBAFILE_MARKETGUIDE							"pcqt1700"
#define DBAFILE_FIRSTCALLEARN							"pcqt1800"

#define DBAFILE_DV_CMINDEX								"cmindex.dv"
#define DBAFILE_DV_CMTEXT 								"cmtext.dv"
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
#define DBAFILE_IB_IBINDEX 							"ibindex.nd"
#define DBAFILE_IB_IBTEXT								"ibtext.nd"

#define COMMENT_USER										"User Price Data Base"
#define COMMENT_EXTENDED                        "Open,High,Low,Last,Bid,Ask"
#define COMMENT_TOPTEN                          "Top ten Adv,Dec,Most Active"
#define COMMENT_MMAKER                          "Level 2 Quotations"
#define COMMENT_NEWSFLAGS                       "News Indicator Sources"
#define COMMENT_PRICE_TS                        "Time Series Price"
#define COMMENT_SESSION 								"Session"
#define COMMENT_HISTORY 								"Price History"
#define COMMENT_OPTIONS 								"Options Fixed Data"
#define COMMENT_MASTER_OPTION						"Master Option Database"
#define COMMENT_PROFILE                         "Security Fixed Data"
#define COMMENT_USFUNDAMENT                     "US Fundamental Data"
#define COMMENT_INTLFUNDAMENT                   "Intl Fundamental Data"
#define COMMENT_EXCHANGE                        "Exchange Processing Codes"
#define COMMENT_PRIMARY_EXCH                    "Primary exchange groups"
#define COMMENT_COUNTRY                         "Country codes"
#define COMMENT_UNDERLYING                      "Company Data"
#define COMMENT_1021DATA                        "Pcqt1021 Data"
#define COMMENT_NEWS                            "News"
#define COMMENT_1400                            "Cusip Data Base"
#define COMMENT_FUTURES                         "Futures Options Fixed Data"
#define COMMENT_OPTION_INTRADAY ""
#define COMMENT_REFERENCE								"Symbol Reference"
#define COMMENT_MARKETGUIDE							"Market Guide"
#define COMMENT_FIRSTCALLEARN							"First Call Earnings"

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
#define COMMENT_IB_IBINDEX 							"IBES Index"
#define COMMENT_IB_IBTEXT								"IBES Text"
#endif

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

#include <pcqt1400.h>
#pragma pack(2)
#include <pcqt1800.h>
#pragma pack(2)

typedef struct _DBA_MMKEY {
	USHORT	type;
	UCHAR 	symbol[12];
	UCHAR 	countryCode[2];
	UCHAR 	exchangeCode[2];
	UCHAR 	currencyCode[3];
	UCHAR 	mmid[4];
	UCHAR 	reserved;
	} DBA_MMKEY;

typedef	union  _DBA_MAXKEY {
	DBA_KEY	 dbaKey;
	DBA_MMKEY mmKey;
	} DBA_MAXKEY;

typedef struct _DBA_KEY52_0 {
	DBA_KEY	 item;							 /* unique key 							*/
	} DBA_KEY52_0;

typedef struct _DBA_KEY52_1 {
	DBA_KEY	 baseitem;						 /* allows duplicates					*/
	} DBA_KEY52_1;

typedef struct _DBA_KEY52_2 {
	BYTE		 name[36];
	} DBA_KEY52_2;

typedef struct _DBA_KEY52_3 {
	BYTE		 country[2];
	BYTE		 name[36];
	} DBA_KEY52_3;

typedef struct _DBA_VERSION {
	SHORT major;
	SHORT minor;
	CHAR release[2];
	CHAR serialNumber[16];
	CHAR softwareVersion[8];
	} DBA_VERSION;

typedef struct _DBA_USER {
	CHAR buffer[250];
	SHORT	serverData;
	SHORT	data0;
	SHORT	data1;
	} DBA_USER;

typedef struct _DBA_PRICE {	
	LONG price;
	ULONG size;
	CHAR exchangeCode[2];
	CHAR hour;
	CHAR minute;
	} DBA_PRICE;

typedef struct _PCQ_DATE {
   USHORT   year;
   BYTE     month;
   BYTE     day;
	} PCQ_DATE;

typedef struct _PCQ_TIME {
	BYTE		hour;
	BYTE		minute;
	BYTE		second;
	BYTE		hundredth;
	} PCQ_TIME;

typedef  struct  _TICK_BITS {
	BIT_FIELD  unused		:2;
	BIT_FIELD  midClose  :2;
	BIT_FIELD  mid       :2;
	BIT_FIELD  ask       :2;
	BIT_FIELD  bid       :2;
	BIT_FIELD  close     :2;
	BIT_FIELD  open      :2;
	BIT_FIELD  lastSale  :2;
	} TICK_BITS;

#define	NOTICK	0
#define  UPTICK   1
#define	DNTICK	2

#define TICK_LASTSALE	0xc000
#define TICK_OPEN 		0x3000
#define TICK_CLOSE		0x0c00
#define TICK_BID			0x0300
#define TICK_ASK			0x00c0
#define TICK_MID			0x0030
#define TICK_MIDCLOSE	0x000c

typedef  union   _PCQ_TICK {
	TICK_BITS   tickBits;
	USHORT      tick;
	} PCQ_TICK;

#define DB3_DELETED						0x0001
#define DB3_RESTRICTED					0x0010
#define DB3_OPEN_INDICATOR 			0x0020
#define DB3_NEWS_INDICATOR 			0x0040
#define DB3_FAST_MARKET 				0x0080
#define DB3_ROTATION_MARKET			0x0100
#define DB3_HALTED_MARKET				0x0180
#define DB3_SPREAD_TRADE				0x0200
#define DB3_STRADDLE_TRADE 			0x0400
#define DB3_NON_FIRM_QUOTE 			0x0600
#define DB3_AUTO_EXECUTE				0x0800
#define DB3_LOCKED_QUOTE				0x1000
#define DB3_CROSSED_QUOTE				0x2000
#define DB3_EXDIVIDEND					0x4000
#define DB3_SETTLEMENT					0x4000	/* flags settlement if future */
#define DB3_SPLIT 						0x8000

#if defined (TOWER_INCL)
typedef struct  _DBA_BITS_3 {
    BIT_FIELD  deleted			  :1;   /* soft delete						*/
    BIT_FIELD  correction		  :1;		/* bridge correction recvd today */
    BIT_FIELD  undefined0		  :3;
    BIT_FIELD  openIndicator	:1;		/* Issue has opened today		*/
    BIT_FIELD  newsIndicator	:1;		/* News indicator					*/
    BIT_FIELD  spCondFlag1  	:2;   /* Fast, Rotation, Halt.		*/
    BIT_FIELD  spCondFlag2  	:2;   /* Spread, Straddle, Non-firm.*/
    BIT_FIELD  autoExecute  	:1;   /* Auto execute					*/
    BIT_FIELD  spCondFlag3  	:2;   /* Locked, Crossed.           */
    BIT_FIELD  exDividend		  :1;  	/* ExDividend today.          */
    BIT_FIELD  split 		   	  :1;  	/* Split today.               */
} DBA_BITS_3;
#else
typedef struct  _DBA_BITS_3 {
	BIT_FIELD  deleted			:1;		// Soft delete.
	BIT_FIELD  undefined0		:3;
	BIT_FIELD  restricted		:1;		// Restricted.
	BIT_FIELD  openIndicator	:1;		// Issue has opened today.
	BIT_FIELD  newsIndicator	:1;		// News indicator.
	BIT_FIELD  spCondFlag1		:2;		//
	BIT_FIELD  spCondFlag2		:2;		//
	BIT_FIELD  autoExecute		:1;		// Auto execute
	BIT_FIELD  spCondFlag3		:2;		//
	BIT_FIELD  exDividend		:1;		// ExDividend today.
	BIT_FIELD  split				:1;		// Split today.
	} DBA_BITS_3;
#endif
typedef union {
	DBA_BITS_3	dbaBits3;
	USHORT		flags;
	} DBA_MASK_3;
	            
// defines for USHORT tick
#define DNTICK_LASTSALE 0x8000
#define UPTICK_LASTSALE 0x4000
#define DNTICK_OPEN 		0x2000
#define UPTICK_OPEN 		0x1000
#define DNTICK_CLOSE 	0x0800
#define UPTICK_CLOSE 	0x0400
#define DNTICK_BID		0x0200
#define UPTICK_BID		0x0100
#define DNTICK_ASK		0x0080
#define UPTICK_ASK		0x0040

typedef struct	_DBA_RECORD_3 {
	DBA_KEY		item;
	DBA_MASK_3	dbaMask3;
	USHORT		priceType;
	USHORT		securityCatagory;
	USHORT		tick;
	SHORT 		newsSource[3];
	CHAR			primaryExchangeCode[2];
	USHORT		quoteIndicator;
	USHORT		row;
	USHORT 		column;
	DBA_PRICE	lastSale;
	DBA_PRICE	open;
	DBA_PRICE	high;
	DBA_PRICE	low;
	DBA_PRICE	close;
	DBA_PRICE	bid;
	DBA_PRICE	ask;
	DBA_PRICE	mid;
	DBA_PRICE	midClose;
	ULONG 		openInterest;
	ULONG 		volume;
	ULONG			openRange;
	ULONG			closeRanges[2];
	USHORT		tradeIndicator;
	USHORT		session;
	} DBA_RECORD_3;

typedef struct	_DBA_RECORD_4{
	DBA_KEY item;
	DBA_KEY data[10];
	} DBA_RECORD_4; 


#define	BEST_SAEF_NO_DEAL 	0	/* unwilling to deal at best SAEF price  */
#define	BEST_SAEF_BID			1	/* willing to buy at best SAEF Bid		  */
#define	BEST_SAEF_ASK			2	/* willing to sell at best SAEF Ask		  */
#define	BEST_SAEF_BOTH 		3	/* willing to buy and sell at SAEF Best. */

typedef struct _DBA_BITS_5 {
	BIT_FIELD  bestIndicator	: 3;
	BIT_FIELD  deleted		 	: 1;
	BIT_FIELD  bidFiltered  	: 1;
	BIT_FIELD  askFiltered  	: 1;
//	BIT_FIELD  type            : 3;	// currently used only on Level 2 as a secondary quote indicator
//	BIT_FIELD  reserved 	 		: 7;
	BIT_FIELD  reserved 	 		: 5;
	BIT_FIELD  zeroBidAsk		: 1;
	BIT_FIELD  reserved1	 		: 4;
	} DBA_BITS_5;

typedef union {
	DBA_BITS_5	dbaBits5;
	USHORT		flags;
	} DBA_MASK_5;

typedef struct _DBA_RECORD_5	{					  /* Market Maker  */
	DBA_MMKEY	item;
	DBA_MASK_5	dbaMask5;
	PCQ_TICK			tick;
	USHORT		priceType;
	USHORT		securityCatagory;
	USHORT		quoteIndicator;
	DBA_PRICE	bid;
	DBA_PRICE	ask;
	BYTE			bidTimeHundredths;
	BYTE			bidTimeSeconds;
	BYTE			askTimeHundredths;
	BYTE			askTimeSeconds;
	USHORT		row;
	USHORT		column;
	} DBA_RECORD_5;

typedef struct _NEWSBITS {
	BIT_FIELD  prNewsWireInc		:1;		/* 			P			*/
	BIT_FIELD  securityTraders		:1;		/* (ST ) 	Y			*/
	BIT_FIELD  unitedPressInt 		:1;		/* (UPI) 	U			*/
	BIT_FIELD  prNewsWire			:1;		/* (PR ) 	R			*/
	BIT_FIELD  kyoddNewsInt			:1;		/* (KYO) 	K			*/
	BIT_FIELD  garvinGuybutler		:1;		/* (GGB) 	G			*/
	BIT_FIELD  associatedPress		:1;		/* (AP ) 	E			*/
	BIT_FIELD  afpDpa					:1;		/* (AFP) 	D			*/
	BIT_FIELD  comtexScientific	:1;		/* (CTX) 	X			*/
	BIT_FIELD  businessWire			:1;		/* (BW ) 	S			*/
	BIT_FIELD  capitalMarketNews 	:1;		/* (CMR) 	C			*/
	BIT_FIELD  dowJonesWallStreet	:1;		/* (DWS) 	W			*/
	BIT_FIELD  newsRetrieval		:1;		/* (DNR) 	N			*/
	BIT_FIELD  barrons				:1;		/* (BAR) 	B			*/
	BIT_FIELD  wallStreetJournal 	:1;		/* (WSJ) 	J			*/
	BIT_FIELD  dowJones 				:1;		/* (DJ)		T			*/
	BIT_FIELD  unused					:6;		/* future growth		*/
	BIT_FIELD  futureWorld 			:1;		/* (FWN) 	1			*/
	BIT_FIELD  federalFilings 		:1;		/* (FED) 	L			*/
	BIT_FIELD  firstCallUS 			:1;		/* (FC ) 	F			*/
	BIT_FIELD  profInvestor			:1;		/* (PIR) 	I			*/
	BIT_FIELD  reutersDirect		:1;		/* (RTR) 	A			*/
	BIT_FIELD  muniFacts				:1;		/* (MFX) 	M			*/
	BIT_FIELD  intlDataGroup		:1;		/* (not implemented) */
	BIT_FIELD  businessWireNews	:1;		/* (BW ) 	Z			*/
	BIT_FIELD  federalNews 			:1;		/* (FNS) 	V			*/
	BIT_FIELD  dpa						:1;		/* (DPA) 	H			*/
	} NEWSBITS;

typedef	struct  _NEWS_BITS {
	BIT_FIELD  dowJones 			 	: 1;  // (DJ) 	 T
	BIT_FIELD  wallStreetJournal  : 1;  // (WSJ)	 J
	BIT_FIELD  barrons				: 1;  // (BAR)	 B
	BIT_FIELD  newsRetrieval		: 1;  // (DNR)	 N
	BIT_FIELD  dowJonesWallStreet : 1;  // (DWS)	 W
	BIT_FIELD  capitalMarketNews  : 1;  // (CMR)	 C
	BIT_FIELD  businessWire			: 1;  // (BW )	 S
	BIT_FIELD  comtexScientific	: 1;  // (CTX)	 X
	BIT_FIELD  afpDpa				 	: 1;  // (AFP)	 D
	BIT_FIELD  associatedPress	 	: 1;  // (AP )	 E
	BIT_FIELD  garvinGuyButler	 	: 1;  // (GGB)	 G
	BIT_FIELD  kyoddNewsInt		 	: 1;  // (KYO)	 K
	BIT_FIELD  prNewsWire			: 1;  // (PR )	 R
	BIT_FIELD  unitedPressInt 	 	: 1;  // (UPI)	 U
	BIT_FIELD  securityTraders	 	: 1;  // (ST )	 Y
	BIT_FIELD  prNewsWireInc		: 1;  //			 P
	BIT_FIELD  dpa					 	: 1;  // (DPA)	 H
	BIT_FIELD  federalNews 		 	: 1;  // (FNS)	 V
	BIT_FIELD  businessWireNews	: 1;  // (BW )	 Z
	BIT_FIELD  intlDataGroup		: 1;  // (not implemented)
	BIT_FIELD  muniFacts			 	: 1;  // (MFX)	 M
	BIT_FIELD  reutersDirect		: 1;  // (RTR)	 A
	BIT_FIELD  profInvestor		 	: 1;  // (PIR)	 I
	BIT_FIELD  firstCallUS 		 	: 1;  // (FC )	 F
	BIT_FIELD  federalFilings 	 	: 1;  // (FED)	 L
	BIT_FIELD  endOfUS2 			 	: 7;

	BIT_FIELD  endOfUS3 			 	:16;
	BIT_FIELD  endOfUS4 			 	:16;

	BIT_FIELD  extel 				 	: 1;  // (EXT)	 E
	BIT_FIELD  extelWorldEcoNews	: 1;  // (GEX)	 W
	BIT_FIELD  extelAfp 			 	: 1;  // (AFX)	 A
	BIT_FIELD  vwdNews			 	: 1;  // (VWD)	 V
	BIT_FIELD  djiEcoNewsWire 	 	: 1;  // (DJE)	 I
	BIT_FIELD  djiFinNewsWire 	 	: 1;  // (DJF)	 F
	BIT_FIELD  djiEuroCorpRept	 	: 1;  // (DJC)	 C
	BIT_FIELD  firstCallEurope	 	: 1;  // (FCE)	 B
	BIT_FIELD  endofEurope1		 	: 8;
	BIT_FIELD  endofEurope2		 	:16;

	BIT_FIELD  nikkei				 	: 1;  // (NEN)	 N
	BIT_FIELD  austrFinRpt 		 	: 1;  // (AFR)	 A
	BIT_FIELD  austrAP				: 1;  // (AAP)	 P
	BIT_FIELD  kyodoNews			 	: 1;  // (KYO)	 K
	BIT_FIELD  firstCallPacific	: 1;  // (FCP)	 F
	BIT_FIELD  endofAsia1			:11;
	BIT_FIELD  endofAsia2			:16;
	} NEWS_BITS;

typedef union _NEWS_FLAGS {
	NEWS_BITS	bits;
	ULONG 		mask[4];
	} NEWS_FLAGS;

typedef union _NEWS_MASK {
	NEWSBITS 	bits;
	ULONG 		mask;
	} NEWS_MASK;

#define NB_US1 				0
#define NB_US2 				1
#define NB_EUROPE 			2
#define NB_ASIA				3

typedef struct _DBA_RECORD_6 {				 // News Flags
	DBA_KEY		 item;
	USHORT		 zeroCount;
	NEWS_FLAGS	 flags;
	} DBA_RECORD_6;

#define DBA_TS_TYPE_LAST	0
#define DBA_TS_TYPE_BID		1
#define DBA_TS_TYPE_ASK		2
#define DBA_TS_TYPE_MMBID	3
#define DBA_TS_TYPE_MMASK	4
#define DBA_TS_TYPE_DELETE	0x80
#define DBA_TS_TYPE_END		0xff

#define DBA_TS_MAX_ENTRY	8

typedef struct DBA_TS_ITEM {
	LONG 				price;
	ULONG 			size;
	UCHAR				type;
	UCHAR				indicator;
	UCHAR				priceType;
	UCHAR 			exchangeCode[4];
	UCHAR 			hour;
	UCHAR 			minute;
	UCHAR				second;
	} DBA_TS_ITEM;

typedef struct _DBA_TS_REC {
	DBA_TS_ITEM	tsData[DBA_TS_MAX_ENTRY];
	} DBA_TS_REC;

typedef struct _DBA_SESSION_KEY {
	USHORT			session; 				// Session number.
	USHORT			eventType;				// Type of event.
	} DBA_SESSION_KEY;

typedef struct _DBA_SESSION_BITS {
	BIT_FIELD		disabled : 1;			// Session disabled.
	BIT_FIELD		unused	: 15;
	} DBA_SESSION_BITS;

typedef union _DBA_SESSION_FLAGS {
	DBA_SESSION_BITS	bits;
	USHORT				flags;
	} DBA_SESSION_FLAGS;

typedef struct _DBA_SESSION_REC {
	DBA_SESSION_KEY	key;
	DBA_SESSION_FLAGS flags;
	PCQ_TIME 			timeGMT; 			// GMT time of event.
	PCQ_DATE 			dateLocal;			// Exchange local date of roll.
	PCQ_TIME 			timeReceipt;		// Local (PC) time of message receipt.
	UCHAR 				sessionBODs[7];	// # of session starts per day of week.
	UCHAR 				sessionEODs[7];	// # of session ends per day of week.
	UCHAR 				unused[64];
	} DBA_SESSION_REC;

#define DBAHIST_FIRSTDATE 711859L  // In DATEMATH format

typedef struct {
	DBA_KEY		item;
	union {									// OTHER INFORMATION USED IN REQUEST/RESPONSE
		CHAR				  unused[10];	// Insure we have atleast 10 bytes
		struct {
			USHORT		 startDate; 	// input: KEY's creation date
			CHAR			 flag;			// input: KEY's flag bits
			} addKey;						// request: DBA_ADD_KEY
		struct {
			USHORT		 date;			// input: date of get (0=none)
			SHORT 		 nRecs;			// output: Number of DBA_HISTORY_RECs found
			} getTS; 						// request: DBA_GET_FIRST_TS, etc.
		struct {
			LONG		 blockNo;			// input: Block to get
			} getDirect;					// request: DBA_GET_DIRECT
		} keys;
	} DBA_HISTORY_KEY;					// Must not exceed 64 bytes

#define DBAHIST_RECTYPE_PRICE 0		// Price record

typedef struct {							// Record Type == 0
	CHAR		type; 						// =0
	UCHAR 	priceType;					// PC Quote price type
	USHORT	date; 						// End of session date (0 is invalid)
												// hdr.startDate+date=record date in DATEMATH format
	LONG		open; 						// Session open price, by priceType
	LONG		high; 						// Session high price, by priceType
	LONG		low;							// Session low price, by priceType
	LONG		last; 						// Session last price, by priceType
	LONG		volume;						// Session volume in 100s shares
	CHAR		unused[8];					// Fill out to 32 bytes
	} DBA_HISTORY_RECS;

typedef struct {
	 LONG 	date; 						// Date of oldest record in volume (datemath)
	 LONG 	firstFreeBlk;				// First free block in free block chain
	 LONG 	dateOffset; 				// Add to record date to get datemath value
	 SHORT	sizeOfHIS_INFO;			// =sizeof(HIS_INFO)
	 SHORT	sizeOfHIS_BLK; 			// =sizeof(HIS_BLK)
	 SHORT	sizeOfHIS_HDR; 			// =sizeof(HIS_HDR)
	 SHORT	sizeOfHIS_REC; 			// =sizeof(HIS_REC)
    CHAR		unused[236];            // Fill out to 256 bytes
    } DBA_HISTORY_VOLINFO;

#define DBAHIST_FLAG_ISFIRST 0x01	// This is the first block for KEY

typedef struct {
	 DBA_KEY item; 						// PC Quote KEY
	 USHORT	startDate;					// Date KEY was created
	 USHORT	lastDate;					// Date in last record (or 0)
	 SHORT	nRecs;						// # of records used in this block
	 SHORT	nRecsMax;					// Max # of records allowed in this block
	 LONG 	nTotalRecs; 				// Total # of records for this KEY
	 LONG 	thisBlk; 					// This block number
	 LONG 	firstBlk;					// Block number of KEY block
	 LONG 	nextBlk; 					// Pointer to next block
	 LONG 	prevBlk; 					// Pointer to prev block
 	 CHAR 	flag;
	 CHAR	reserved;
	 USHORT	row;							// Entitlement row
	 USHORT	column;						// Entitlement column
	 CHAR 	unused[200];				// Fill out to 256 bytes
    } DBA_HISTORY_KEYINFO;

#define DBAHIST_NRECS	8				 // # of records in DBA_HISTORY_RECS

typedef union _DBA_HISTORY_REC {
	DBA_HISTORY_KEYINFO	keyInfo;
	DBA_HISTORY_RECS		recs[DBAHIST_NRECS];
	} DBA_HISTORY_REC;

typedef	struct _DBA_RECORD_50_1 {
	DBA_KEY item;
	USHORT classSequence;				      /* sequence number within the class */
	USHORT recordType;							/* record type   = 1 					*/
	USHORT claseType; 							/* class type i.e 1						*/
	CHAR name[32]; 								/* company name							*/
	LONG shortTermInterest; 					/* short term interest rate if any	*/
	USHORT subCount;								/* number of sub records following	*/
	} DBA_RECORD_50_1;

typedef struct _DBA_RECORD_50_2 {
	DBA_KEY	item;
	USHORT	classSequence; 					/* sequence number within the class */
	USHORT	recordType; 						/* record type = 2						*/
	CHAR		underlyingOption[5]; 			/* underlying option symbol			*/
	CHAR		type; 								/* type ?									*/
	CHAR		primaryExchangeCode[2]; 		/* primary exchange of the options	*/
	USHORT	cycles[8];							/* 8 option cycles						*/
	USHORT	marketMask; 						/* market mask 							*/
	USHORT	contractSize;						/* number of units in a contract 	*/
	} DBA_RECORD_50_2;

typedef struct _DBA_RECORD_50_3 {
	DBA_KEY item;
	USHORT classSequence;                  /* sequence number within the class */  
	USHORT recordType;							/* record type = 3						*/
	USHORT strikes[26];							/* 26 possible strike values			*/
	} DBA_RECORD_50_3;

typedef struct _DBA_RECORD_50_4 {
	DBA_KEY item;
	USHORT classSequence;						/* sequence number within the class */
	USHORT recordType;							/* record type = 4						*/
	CHAR underlyingStockSymbol[11];			/* underlying stock symbol 			*/
	CHAR primaryExchangeCode;					/* primary exchange of the stock 	*/
	USHORT sharesPerContract;					/* shares per contract					*/
	USHORT volatilities[8]; 					/* 8 volatilities 						*/
	USHORT frequency; 							/* dividend frequency					*/
	CHAR xDividendCentury;						/* century of the ex-dividend date	*/
	CHAR xDividendYear;							/* year of the ex-dividend date		*/
	CHAR xDividendMonth; 						/* month of the ex-dividend date 	*/
	CHAR xDividendDay;							/* day of the ex-dividend date		*/
	USHORT xDividend; 							/* ex-dividend amount 3 decimal		*/
	CHAR sDividendCentury;						/* century of any special dividend	*/
	CHAR sDividendYear;							/* year of any special dividend		*/
	CHAR sDividendMonth; 						/* month of any special dividend 	*/
	CHAR sDividendDay;							/* day of any special dividend		*/
	USHORT sDividend; 							/* amount of any special dividend	*/
	} DBA_RECORD_50_4;

typedef struct _DBA_RECORD_50_5 {
	DBA_KEY item;
	USHORT classSequence;                /* sequence number within the class */ 
	USHORT recordType;                   /* record type */                      
	CHAR monthCode;	                   /* month letter of the exception */
	CHAR strikeCode;	                   /* strike letter of the exception */
	USHORT strike;                       /* strike value for this exception */
	} DBA_RECORD_50_5;

typedef union _DBA_RECORD_50 {
	DBA_RECORD_50_1	definition; 		/* Option data base 50				  */
	DBA_RECORD_50_2	option;				/* Data base 50 sub-type 2 		  */
	DBA_RECORD_50_3	strike;				/* Data base 50 sub-type 3 		  */
	DBA_RECORD_50_4	stock;				/* Data base 50 sub-type 4 		  */
	DBA_RECORD_50_5	exception;			/* Data base 50 sub-type 5 		  */
	} DBA_RECORD_50;

#define DATE51_DAY		0x001f
#define DATE51_MONTH		0x01e0
#define DATE51_YEAR		0xfe00

typedef struct _DATE_51 {
	BIT_FIELD  day  	:5;
	BIT_FIELD  month	:4;
	BIT_FIELD  year 	:7;
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
	DBA_KEY	item;
	USHORT	spCode;								/* S&P code 			 */
	USHORT	high; 								/* 2 year High 		 */
	USHORT	low;									/* 2 year Low			 */
	DATE_51	date; 								/* x-dividend date	 */
	USHORT	oldDiv;						   
	USHORT	frequency;							/* freq. of divid.	 */
	SHORT		earnings;
	LONG		shares;								/* Shares outstanding */
	USHORT	volatility;
	USHORT	volume;
	CHAR		name[36];							/* Company Name		 */
	USHORT	priceType;							/* Bridge Format type */
	USHORT	padding;
	ULONG 	dividend;							/* dividend amount	 */
	ULONG		trailDividend;
	} DBA_RECORD_51;

#define MRK56_CBOT		1
#define MRK56_CME			2
#define MRK56_CEC			3
#define MRK56_KCBOT		4
#define MRK56_MIDAM		5
#define MRK56_MINN		6
#define MRK56_NYMEX		7

typedef 	struct _DATE_56 {
	CHAR	century;
	CHAR	year;
	CHAR	month;
	CHAR	day;
	} DATE_56;

typedef	struct _DBA_RECORD_56_0 {
	DBA_KEY 	item;
	USHORT 	classSequence;    	            /* sequence number within the class */
	USHORT 	recordType;								/* record type   = 0 					*/
	ULONG		cdRates[5];
	ULONG		brokerCall;
	ULONG		shortStock;
	DATE_56	tradeDate;
	DATE_56	calcDate;
	} DBA_RECORD_56_0;

typedef	struct _DBA_RECORD_56_1 {
	DBA_KEY 	item;
	USHORT 	classSequence; 	               /* sequence number within the class */ 
	USHORT 	recordType;								/* record type   = 1 					*/
   CHAR     group[6];                        /* Futures group i.e. grains,metals,currency,etc. */
   USHORT   type;										/* 0 = normal, 1 = interest rate year, 2 = interest rate 90 day */
	CHAR     contractName[32];						/* Description	*/
   USHORT   market;									/* Exchange Number	*/
   USHORT   priceType;								/* Price Type	*/
   DATE_56 	expDate;									/* Future Expirations Date	*/
   ULONG    contractSize;                    /* Contract Size		*/
   CHAR     contractDescription[12];			/* Contract Units	Descrription	*/
   ULONG    interest;								/* Special Risk free interest rate	*/
   ULONG    volatility;								/* Historic volatility	*/
   CHAR     priceUnits[12];						/* Price units	description */
   USHORT   contractSizeType;						/* Contract size type */
   USHORT   priceToDollars;                  /* Price to dollars conversion factor	*/
	UCHAR    exSymCall[2];							/* exchange symbol root for a call */
	UCHAR    exSymPut[2];							/* exchange symbol root for a put */
	} DBA_RECORD_56_1;

typedef	struct _DBA_RECORD_56_3 {
	DBA_KEY 	item;
	USHORT 	classSequence;                	/* sequence number within the class */ 
	USHORT 	recordType;						  		/* Record Type   = 3 					*/
	DBA_KEY	optionSymbol;
   DATE_56 	expDate;									/* Futures Option Expiration Date	*/
   ULONG    contractSize;                 	/* Contract Size	*/
   USHORT   contract;								/* Option Contract	*/
   USHORT   strikeConversion;						/* Strike Price Conversion Factor	*/
   USHORT   priceType;								/* Price Type	*/
   CHAR     contractDescription[12];			/* Contract Units Description			*/
	UCHAR    exSymCall[2];							/* exchange symbol root for a call */
	UCHAR    exSymPut[2];							/* exchange symbol root for a put */
   USHORT   priceToDollars;                  /* Price to dollars conversion factor	*/
	} DBA_RECORD_56_3;

typedef union _DBA_RECORD_56 {
	DBA_RECORD_56_0	header;						/* Data base 56 sub-type 0 		  */
	DBA_RECORD_56_1	futClass;					/* Data base 56 sub-type 1 		  */
	DBA_RECORD_56_3	option;						/* Data base 56 sub-type 3 		  */
	} DBA_RECORD_56;

#define VD1_PRIMEXCH_MASK	 0xf800				/* prim exch mask 0=none,1=A		*/
#define VD1_X_DISTRIBUTION  0x0020
#define VD1_X_DIVIDEND		 0x0010

#define VD2_VOLUME_MASK 	 0x0c00
#define VD2_VOLUME_UNIT 	 0x0000
#define VD2_VOLUME_TEN		 0x0400
#define VD2_VOLUME_HUNDRED  0x0800
#define VD2_VOLUME_THOUSAND 0x0c00

#define VD3_DIVFREQ_MASK	 0x0003
#define VD3_DIVFREQ_NONE	 0x0000
#define VD3_DIVFREQ_QUART	 0x0001
#define VD3_DIVFREQ_SEMI	 0x0002
#define VD3_DIVFREQ_ANNUAL  0x0003

#define VD3_CALLED			0x0008			  /* called bond						  */
#define VD3_CUMUL_REDEEMED 0x0010			  /* bonds=redeemed,pref=cumulative*/

/* Bridge Volume Types			 */

#define BVT_UNITS 				  0
#define BVT_TENS					  1
#define BVT_HUNDREDS 			  2
#define BVT_THOUSANDS			  3

typedef struct _SP_DATE {
	BYTE		century;
	BYTE		year;
	BYTE		month;
	BYTE		day;
	} SP_DATE;

typedef struct _VEHICLE_DATA {
	USHORT				 divFreq;				  /* freq of div 0=None,1=Q,2=S,3=A		*/
	USHORT				 vehicleFlag1; 		  /* VD1_* flags.								*/
	USHORT				 vehicleFlag3; 		  /* VD3_* flags.								*/
	USHORT				 priceType; 			  /* prtype for prices in this record	*/
	USHORT				 securityCategory;	  /* ISE Security Category 0 = ALPHA.	*/
	USHORT				 volatility;			  /* volatility.								*/
	USHORT				 vehicleFlag2;
	USHORT				 reservedSB;
	USHORT				 divFormat; 			  /* 1=1 decimal,2 = 2 decimals etc.	*/
	USHORT				 exdDivAmount; 		  /* amount of last dividend				*/
	USHORT				 spcDivAmount; 		  /* amount of special dividend			*/
	USHORT				 matDivAmount; 		  /* amount of dividends to Maturity	*/
	SP_DATE				 exdDivDate;			  /* date of last dividend					*/
	SP_DATE				 spcDivDate;			  /* date of special dividend				*/
	SP_DATE				 maturityDate; 		  /* date of dividends to Maturity		*/
	SP_DATE				 lastSaleDate; 		  /* date of last sale.						*/
	ULONG 				 reservedLA;
	LONG					 closePrice;			  /* yesterday's closing price. 			*/
	LONG					 historicalHigh;		  /* 52 week High.							*/
	LONG					 historicalLow;		  /* 52 week Low.								*/
	ULONG 				 yestVolume;			  /* yesterday's total volume				*/
	LONG					 openInterest; 		  /* yesterday's open Interest.			*/
	ULONG 				 sharesOutstanding;	  /* total shares outstanding.			*/
	ULONG 				 contractSize; 		  /* sh/ctract(op) contractsize(fut)	*/
	LONG					 strikePrice;			  /* option strike price.					*/
	ULONG 				 reservedLB;																
	} VEHICLE_DATA;	/* 80 bytes  */

#define UD1_TYPE_MASK		0x0007
#define UD1_TYPE_EQUITY 	0x0001
#define UD1_TYPE_FOREX		0x0002
#define UD1_TYPE_NARROW 	0x0003
#define UD1_TYPE_BROAD		0x0004
#define UD1_TYPE_TBOND		0x0005

typedef struct _UNDERLYING_DATA {
	CHAR					 name[36];				 /* Underlying Name.							 */
	CHAR					 desc[12];				 /* Description - (futures)				 */
	USHORT				 epsFormat; 			 /* 1=1 decimal,2=2 decimals,etc			 */
	SHORT 				 eps; 					 /* Earnings Per Share.						 */
	USHORT				 spCode; 				 /* S&P Industry Code						 */
	USHORT				 flag1;					 /* Flag word 1 UD1_* values.				 */
#if defined (TOWER_INCL)
  	CHAR					 column; 				 // Bridge Entitlement Word.
    CHAR					 row; 					 // Bridge Entitlement Word.
#else
	USHORT				 entitle;				 /* Bridge Entitlement Word.				 */
#endif
	} UNDERLYING_DATA; /* 58 bytes	*/

typedef struct _DBA_RECORD_70 {
	DBA_KEY          item;
	UNDERLYING_DATA  underlying;
	} DBA_RECORD_70;

#define	F52_NEW			  0x0001
#define	F52_CHANGED 	  0x0002
#define	F52_DELETED 	  0x0004
#define	F52_IGNORE		  0x0008
#define	F52_LOCKED		  0x0010
#define	F52_INDEPENDENT  0x8000
	
typedef struct _DBA_RECORD_52 {
	DBA_KEY				 item;				 /* Symbol			 -key 0		 22	*/
	USHORT				 flags;				 /* F52_* flags					  2	*/
	DBA_KEY				 baseItem;			 /* Parent Symbol  -key 1		 22	*/
	USHORT				 updDate;			 /* Date last updated			  2	*/
	VEHICLE_DATA		 vehicle;			 /*									 80	*/
	UNDERLYING_DATA	 underlying;		 /*									 58	*/
	} DBA_RECORD_52;								 /*									186	*/


#define	NEWS_TYPE_HEADLINE0		0			/* types used to write */
#define	NEWS_TYPE_HEADLINE1		1
#define	NEWS_TYPE_BODYLINE		2
#define	NEWS_TYPE_LASTLINE		3
#define	NEWS_TYPE_CONTINUELINE	4
#define	NEWS_TYPE_NEWSALERT		15

#define	NT_NEWS						0			/* types returned on read */
#define	NT_NEWSALERT				1

typedef struct _DBA_TEXT_49 {
	USHORT	length;                       /* length of data			*/
	UCHAR		data[252];							/* data						*/
	} DBA_TEXT_49;

typedef struct _DBA_ALERT_49 {
	UCHAR   title[4];
	DBA_KEY symbol;								/* type 0 packet: title */
	UCHAR   reserved[2];
	USHORT  serviceCategory;
	} DBA_ALERT_49;

typedef union _DBA_DETAIL_49 {
	DBA_TEXT_49		text;
	DBA_ALERT_49	alert;
	} DBA_DETAIL_49;
	
typedef struct _DBA_RECORD_49 {
	UCHAR		source;								/* source of news item	*/
	UCHAR		type;									/* type of news item 	*/
	DBA_DETAIL_49	detail;						/* data						*/
	} DBA_RECORD_49;

typedef struct _PAGE {
	DBA_RECORD_3  SEGPTR *dataPtrs[1];
   } PAGE;

typedef struct _PAGEMAP {
	PAGE SEGPTR *dataPagePtrs[1];
	} PAGEMAP;

#ifndef __INCL_DBHEADER

#if defined (__IBMC__) || defined (__IBMCPP__) || (_MSC_VER > 800)
typedef struct _DBHEADER {
   ULONG     numkeys;
   ULONG     maxkeys;
   USHORT    recordSize;
   USHORT    pointersPerPage;
	PBYTE 	 *pPointerTable;
	SHORT 	 reservedA;
   ULONG     savePosition;
	CHAR		 name[64];
	ULONG 	 dbMask;
	HMTX		 hmtxDb;
	BYTE		 *pDataTable;
	ULONG 	 hFile;
	USHORT	 reservedC;
	} DBHEADER;

#else 

typedef struct _DBHEADER {
	ULONG 	 numkeys;
	ULONG 	 maxkeys;
	USHORT	 recordSize;
	USHORT	 pointersPerPage;
	PAGEMAP	 SEGPTR *pagemapPtr;
	SHORT 	 dataSelector;
	ULONG 	 savePosition;
	CHAR		 name[64];
	ULONG 	 dbMask;
	DOSFSRSEM dbSemaphore;
	} DBHEADER;

#endif

#define __INCL_DBHEADER
#endif

#define ADFLAG_PRESENT	0x0001
#define ADFLAG_SERVER	0x0002
#define ADFLAG_LOGIN		0x0004

typedef struct _ATTACH_DATA {
	USHORT mask;
	CHAR	 serverName[32];
	UCHAR	 loginID[16];
	} ATTACH_DATA;

#ifndef __INCL_DBA_CREATE
#define __INCL_DBA_CREATE

/* PcqDbIO definitions  */

#ifndef __BORLANDC__

typedef BOOL (FAR *PFNPCQDBIO)(USHORT usFunction, PVOID pvOutputBuffer,
	PVOID pvInputBuffer, ULONG fulDbMask);

BOOL PcqDb3IO(USHORT usFunction, PVOID pvOutputBuffer, PVOID pvInputBuffer,
	ULONG fulDbMask);
BOOL PcqDb5IO(USHORT usFunction, PVOID pvOutputBuffer, PVOID pvInputBuffer,
	ULONG fulDbMask);

#endif

#define DBIO_CALC_RECSIZE                         	0
#define DBIO_PACK_RECORD                          	1
#define DBIO_UNPACK_RECORD                        	2

/*  Not implimented yet. */
//#define DBIO_CALC_KEYSIZE                       	3
//#define DBIO_PACK_KEY                           	4
//#define DBIO_UNPACK_KEY                         	5

/* DBA_TYPE_* values for dbaType                          */

#define DBA_TYPE_REDIRECTOR  							  	0xffff      // Redirector Table.
#define DBA_TYPE_PBLKTABLE   								0xfffe      // Pblk table.
#define DBA_TYPE_PIDTABLE         						0xfffd         // Pblk table.

#define DBA_TYPE_MEMORY                          	0       // Memory resident uncompressed.
#define DBA_TYPE_COMPRESSED              				1       // Memory resident compressed.
#define DBA_TYPE_BTRIEVE                         	2       // Brieve file.
#define DBA_TYPE_FLAT_FILE               				3       // Flat file.
#define DBA_TYPE_DATAFILE                        	4       // Keys-Memory/Data-Disk.
#define DBA_TYPE_NEWS                            	5       // News Database.
#define DBA_TYPE_HIMEMORY			 						6 	  	  // HiMem.
#define DBA_TYPE_BTRIEVE_OPTIONS  						7 	  	  // Simulate 1100 from Security Profile.
#define DBA_TYPE_HIMEMORY_2		 						8 	  	  // HiMem 2.
#define DBA_TYPE_TS				                     9       // Time Series data
#define DBA_TYPE_HISTORY								  10		  // Pricing history

/* DBAREC_* Bit Masks for DBARECORDMASK   */

#define DBAREC_VARIABLE_LEN             				0x0001 //(D) Allow Variable Length Records
#define DBAREC_TRUNCATE_BLANKS  							0x0002 //(D) Truncate Blanks in Var Len Recs
#define DBAREC_PREALLOCATE              				0x0004 //(1) Preallocate 'allocation' pages.
#define DBAREC_COMPRESS                 				0x0008 //(2) Compress the record.
#define DBAREC_KEY_ONLY                 				0x0010 //(D) key only database.
#define DBAREC_FREESPACE_10             				0x0040 //(D) 10% free space on Var Len pages.
#define DBAREC_FREESPACE_20             				0x0080 //(D) 20% free space on Var Len pages.
#define DBAREC_FREESPACE_30             				0x00C0 //(D) 30% free space on Var Len pages.

/* DBAKEY_* Bit Masks for DBAKEYMASK              */

#define DBAKEY_STRING										0x0000
#define DBAKEY_ALLOW_DUPLICATES 							0x0001 //(D) This key allows duplicates.
#define DBAKEY_MODIFIABLE               				0x0002 //(D) This key is modifiable.
#define DBAKEY_BINARY                           	0x0004 //(D) This key is a binary Key.
#define DBAKEY_NULL                             	0x0008 //(D) This key has a NULL value.
#define DBAKEY_SEGMENTED                        	0x0010 //(D) This key has another segment.
#define DBAKEY_ALTERNATE                        	0x0020 //(D) Use alternate collating sequence
#define DBAKEY_DESCENDING               				0x0040 //(D) Sort this key in descending order
#define DBAKEY_SUPPLEMENTAL             				0x0080 //        DO NOT USE.
#define DBAKEY_EXTENDED                 				0x0100 //(D) Use the extended key type.
#define DBAKEY_MANUAL                           	0x0200 //

/* DBAKEYEXT_* Values for extendedKeyType   */

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
#define DBAKEYEXT_UNSIGNED_BINARY        				14
#define DBAKEYEXT_AUTOINCREMENT          				15

/* Definitions for DBARECORDPARMS */

typedef struct _DBARECORDFLAGS {
	BIT_FIELD               variableLen :1;          //(D) Allow Variable Length Records
	BIT_FIELD               truncBlanks :1;          //(D) Truncate Blanks in Var Len Recs
   BIT_FIELD               preallocate :1;          //(1) Preallocate 'allocation' pages.
   BIT_FIELD               compress    :1;          //(2) Compress the record.
   BIT_FIELD               keyOnly     :1;          //(D) key only database.
   BIT_FIELD               reserved    :1;
   BIT_FIELD               freeSpace10 :1;          //(D) 10% free space on Var Len pages.
   BIT_FIELD               freeSpace20 :1;          //(D) 20% free space on Var Len pages.
   BIT_FIELD               unused      :8;
	} DBARECORDFLAGS;

typedef union   _DBARECORDMASK {
	DBARECORDFLAGS recFlags;
	USHORT                  recBits;
	} DBARECORDMASK;

typedef struct _DBARECORDPARMS {
	USHORT                  dbLogRecLen;             //Specify the logical record size
	USHORT                  dbPageSize;              //(D) specify the page size.
	USHORT                  dbNumberOfKeys;          //(3) number of keys for this DB.
	ULONG                   dbNumberOfRecs;          //Set To ZERO.
	DBARECORDMASK   		dbRecMask;               //DBAREC_* flags.
	USHORT                  reserved2;               //Set To ZERO.
	USHORT                  dbAllocation;            //(1) Number of pages to preallocate.
	} DBARECORDPARMS;


/* Definitions for DBAKEYPARMS */

typedef struct _DBAKEYFLAGS {
	BIT_FIELD               allowDuplicates :1;      //(D) This key allows duplicates.
	BIT_FIELD               keyModifiabl    :1;      //(D) This key is modifiable.
	BIT_FIELD               keyBinary       :1;      //(D) This key is a binary Key.
	BIT_FIELD               keyNull         :1;      //(D) This key has a NULL value.
	BIT_FIELD               keySegmented    :1;      //(D) This key has another segment.
	BIT_FIELD               sortAlternat    :1;      //(D) Use alternate collating sequence
	BIT_FIELD               sortDescending  :1;      //(D) Sort this key in descending order
	BIT_FIELD               supplemental    :1;      // MUST BE ZERO.
	BIT_FIELD               keyExtended     :1;      //(D) Use the extended key type.
	BIT_FIELD               keyManual       :1;      //(D) Manual Key (must use NULL value).
	BIT_FIELD               unused          :6;
	} DBAKEYFLAGS;

typedef union _DBAKEYMASK {
	DBAKEYFLAGS     keyFlags;
	USHORT          keyBits;
	} DBAKEYMASK;

typedef struct _DBAKEYPARMS {
	USHORT          keyPosition; 		               // Offset in rec of 1st byte of key.
	USHORT          keyLength;                      // Length of the key.
	DBAKEYMASK      keyMask;                        // DBAKEY_* flags.
	USHORT          reserved1a;                     // Set to ZERO.
	USHORT          reserved1b;                     // Set to ZERO.
	BYTE            extendedKeyType;                // DBAKEYEXT_* if DBAKEY_EXTENDED.
	BYTE            nullKeyValue;                   //
	USHORT          reserved3a;                     // Set to ZERO.
	USHORT          reserved3b;                     // Set to ZERO.
	} DBAKEYPARMS;                     
/* Definitions for DBAPARMS */

typedef struct _DBATABLEFLAGS {
	BIT_FIELD   saveData  :1;           // Indicates save to disk outstanding.
	BIT_FIELD   noSave    :1;           // DataBase cannot be save to disk;
	BIT_FIELD   noLoad    :1;           // DataBase cannot be loaded from disk.
	BIT_FIELD   noshare   :1;           // DataBase cannot be shared on network.
	BIT_FIELD   readOnly  :1;           // DataBase is Read Only.
	BIT_FIELD   daemon    :1;           // Indicates Daemon is attached.
	BIT_FIELD	hashTable  :1; 		   // Indicates optional Hash table used.
	BIT_FIELD	insertAsync:1;
	BIT_FIELD	password	 :1;				// DataBase is password protected.
	BIT_FIELD	reserved   :7;
	} DBATABLEFLAGS;

typedef union   _DBATABLEMASK {
	DBATABLEFLAGS   flags;
	ULONG           bits;
	} DBATABLEMASK;

typedef struct _HIMEM {
	USHORT			selCount;
	USHORT			selStart;
	USHORT			recsPerDataPage;
	USHORT			recSizeDataPage;
	USHORT			recsPerSeg;
	USHORT			recSizeSeg;
	USHORT			dataSelector;
	USHORT			shiftCount;
	} HIMEM;

//
// NOTE: a change to HASHTABLE requires a corresponding
// 		change to HASHASM.asm. DON'T FORGET!!!!
//

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

#define DBA_TS_VERSION	0

typedef struct _DBATSHEADER {
	USHORT			usLength;
	UCHAR				ucVersion;
	UCHAR				ucDays;
	ULONG				ulTime;
	} DBATSHEADER;

typedef struct _DBATABLE {
	CHAR            dbName[21];              // Database Table Name.
	CHAR            dbReserved;              
	USHORT          dbNum;                   // Database Table Number(optional);
	USHORT          dbType;                  // DBA_TYPE_* values.
	DBATABLEMASK    dbTableMask;
	USHORT          dbRemote;
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
	HASHTABLE		 hashTable;
	HIMEM 			 himem;
	ULONG 			 ulInsDelHash;
	ULONG 			 ulInsDelBinary;
	} DBATABLE;

#define DBATABLE_RECORD                 DBATABLE


typedef struct _DBAPID_RECORD {
	CHAR           szPID[4];
	USHORT         pid;
	CHAR           szProgName[20];
	ULONG          timeAttached;
	CHAR           reserved[4];
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
	} DBAPARMS;

/* Definitions for DBACREATEPARMS */

typedef struct _DBACREATEPARMS {
	DBAPARMS       dbaParms;
	DBARECORDPARMS recParms;
	DBAKEYPARMS    keyParms[1];
	} DBACREATEPARMS;

typedef struct _DBA_MAX_BUFR_SIZE {
	CHAR	data[256];
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
	DBA_KEY52_0 		spKey0;
	DBA_KEY52_1 		spKey1;
	DBA_KEY52_2 		spKey2;
	DBA_KEY52_3 		spKey3;
	DBA_SESSION_KEY	session;
	DBA_HISTORY_KEY	history;
  PCQT1111_KEY    moKey;                //  master option data base kyes union
	} DBA_KEYS;

typedef	union _DBA_RECORD {
	DBA_RECORD_3		standard;			/* Standard data base number one   */
	DBA_RECORD_3		userDB;				/* User Price data base number two */
	DBA_RECORD_3		extended;         /* Extended data base number three */
	DBA_RECORD_4		topten;				/* Topten data base number four	  */
	DBA_RECORD_5		mmaker;				/* Market Maker Quotes				  */
	DBA_RECORD_6		newsflags;			/* News Flags data base 			  */
	DBA_TS_REC			priceTS;				/* Price Time of Sales data base	  */
	DBA_SESSION_REC	session; 			/* Session database					  */
	DBA_HISTORY_REC	history; 			/* History database					  */
	DBA_RECORD_51		fundamental;		/* Fundamental data base number 51 */
	DBA_RECORD_50_1	definition;			/* Option fundamental data base 50 */
	DBA_RECORD_50_2	option;				/* Data base 50 sub-type 2 		  */
	DBA_RECORD_50_3	strike;				/* Data base 50 sub-type 3 		  */
	DBA_RECORD_50_4	stock;				/* Data base 50 sub-type 4 		  */
	DBA_RECORD_50_5	exception;			/* Data base 50 sub-type 5 		  */
	DBA_RECORD_52		securityProfile;	/* Security Profile Data base.	  */
	DBA_RECORD_49		newsDB;				/* 1081 news data base				  */
	DBA_RECORD_56		future;				/* Data base 56						  */
	DBA_RECORD_56_0	futHeader;			/* Data base 56 sub-type 0 		  */
	DBA_RECORD_56_1	futClass;			/* Data base 56 sub-type 1 		  */
	DBA_RECORD_56_3	futOption;			/* Data base 56 sub-type 3 		  */
	DBA_RECORD_57		reference;			/* Reference database				  */
	DBA_RECORD_58		marketGuide;		/* Market Guide database			  */
   DBA_RECORD_69     firstCallEarn;    /* First Call   database           */
	DBA_USER 			user; 				/* Used specific data area 		  */
	DBA_VERSION 		version; 			/* Get version data returned		  */
	DBHEADER 			dbheader;			/* Get Header data returned		  */
	PCQT1400				pcqt1400;			/* Data base 55                    */
	DBA_RECORD_55     cusipSnp;			/* 2 byte aligned data base 55     */
	ATTACH_DATA 		attach;				/* Attach record layout 			  */
	DBACREATEPARMS    create;				/* Create a data base				  */
	DV_INDEX_REC		dvIndex;			   /* Dow Vision Index Record         */
	PCQT1111 			masterOption;		/* Master Option database			  */
	CHAR					genericData[256]; /* generic data record (anonymous) */
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
 
#if ( _MSC_VER > 800 )
//#ifdef __NT__

#define __DBAX__

ULONG  PASCAL dbax(VOID *pblk,
		  		  			ULONG func,
				  			VOID *key,
				  			VOID *buffer,
				  			ULONG keynum );

BOOL PASCAL PcqDateToUShort(ULONG firstDate, PCQ_DATE *pPcqDate, USHORT *pDate);
BOOL PASCAL UShortToPcqDate(ULONG firstDate, USHORT pDate, PCQ_DATE *pPcqDate);

int PASCAL YMDToDayOfWeek (unsigned int year, unsigned int month, int day);
void PASCAL JulianToYMD(long jdn, int *yy, int *mm, int *dd);
long PASCAL YMDToJulian (short year, short month, short day);

BOOL StringToKey( CHAR *string, DBA_KEY *dbaKey );
BOOL KeyToString( DBA_KEY *dbaKey, CHAR *pszOutputStr,USHORT cchOutputLen );
BOOL _KeyToString( DBA_KEY *dbaKey, CHAR *pszOutputStr,USHORT cchOutputLen );
BOOL uKeyToString( DBA_KEY *dbaKey, CHAR *pszOutputStr,USHORT cchOutputLen );

	typedef LONG PASCAL FN_DBACALLBACK(VOID *pblk,
	  ULONG ulFunc, 
	  DBA_KEY *dbaKey, 
	  DBA_RECORD *dbaRec,
	  ULONG ulKey,
	  DWORD dwStatus,
	  VOID *pFilter);
	  
	typedef FN_DBACALLBACK *PFN_DBACALLBACK;

	typedef struct _DBA_STREAM_FILTER {
		PFN_DBACALLBACK lpfnDbaProc; // callBack function to get data
		INT	nCreateFlags;				// Thread Creation Flags
		PVOID pUser;						// Space for User data
		INT   nUserStatus;				// Space for user information;
		ULONG	ulPositionFunction;		// initial positioning function
		INT   nCount;						// -1 = all
		INT   nProcStatus;		     	// CallBack function status code
		HANDLE hThread;					// Handle of any created thread

		} DBA_STREAM_FILTER;
		
#endif

#ifndef __DBAX__

#define __DBAX__

USHORT _PCQENTRY dbax( VOID SEGPTR *pblk,
							  USHORT func,
							  VOID SEGPTR *key,
							  VOID SEGPTR *buffer,
							  USHORT keynum );

USHORT FAR16 CDECL StringToKey( CHAR SEGPTR *string, DBA_KEY SEGPTR *dbaKey );
USHORT FAR16 CDECL KeyToString( DBA_KEY SEGPTR *dbaKey, CHAR SEGPTR *pszOutputStr,
											USHORT cchOutputLen );
USHORT FAR16 CDECL _KeyToString( DBA_KEY SEGPTR *dbaKey, CHAR SEGPTR *pszOutputStr,
											USHORT cchOutputLen );
#endif	

VOID FAR16 CDECL    left_just( CHAR *string, USHORT num );

#pragma pack() 						 /* Word aligned. */


#ifdef __cplusplus									// C++ compiler is being used
		}
#endif

#endif
