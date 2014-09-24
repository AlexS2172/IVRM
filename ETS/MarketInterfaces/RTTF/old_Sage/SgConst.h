#ifndef _SGCONST_H
#define _SGCONST_H
/*
#define USE_PRODUCTION_SAGE_SERVER

#define SAGE_DEVELOPMENT_ADDR			_T("sage-dev.ny.egartech.com")
#define SAGE_PRODUCTION_ADDR			_T("sage-prd.ny.egartech.com")
#define SAGE_SERVER_PORT				2711


#ifdef USE_PRODUCTION_SAGE_SERVER
	#define SAGE_SERVER_ADDR	SAGE_PRODUCTION_ADDR
#else
	#define SAGE_SERVER_ADDR	SAGE_DEVELOPMENT_ADDR
#endif

#define SAGE_SERVER_PORT		2711
#define RECONNECT_TIME			5	// seconds
#define RECONNECT_SESSION_TIME	(60 * 60 * 2) //2 hours


#ifdef USE_PRODUCTION_SAGE_SERVER
	#define TARGET_COMP_ID			_T("SAGE_FIX_DROPCOPY")
	#define SENDER_COMP_ID			_T("MAIN_LINE")
#else 
	#define TARGET_COMP_ID			_T("SAGE_FIX_SERVER")
	#define SENDER_COMP_ID			_T("MAIN_LINE")
#endif //USE_PRODUCTION_SAGE_SERVER
*/

#define SETTINGS_REGISTRY_KEY			_T("Software\\Egar\\ETS\\Sage Connector")
#define SETTINGS_DB_REGISTRY_KEY		_T("Software\\Egar\\ETS\\Database")
#define SETTINGS_IN_SEQNUM_KEY			_T("InSeqNum")
#define SETTINGS_OUT_SEQNUM_KEY			_T("OutSeqNum")
#define SETTINGS_CONNSTR_KEY			_T("")

#define SETTINGS_RECONNECT_TIME_KEY		_T("ReconnectTime")
#define SETTINGS_TARGET_COMP_ID_KEY		_T("TargetCompID")
#define SETTINGS_SENDER_COMP_ID_KEY		_T("SenderCompID")
#define SETTINGS_SERVER_ADDR_KEY		_T("ServerAddr")
#define SETTINGS_SERVER_PORT_KEY		_T("ServerPort")


#define	MAX_DBRECONNECTION_TRIES		10
#define	DBRECONNECTION_DELAY			10

#define TRADES_FILENAME					_T("Logs/SageTrades.log")

#define TRADES_FILENAME_BEGIN			_T("Logs/STrades")
#define TRADES_FILENAME_END				_T(".log")

#define FAILES_FILENAME_BEGIN			_T("Logs/SFailes")
#define FAILES_FILENAME_END				_T(".log")

#define STRUCTURE_FILENAME				_T("Logs/SageStructure.log")

#endif