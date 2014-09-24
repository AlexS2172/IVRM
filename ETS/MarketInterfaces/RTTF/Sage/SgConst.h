#ifndef _SGCONST_H
#define _SGCONST_H


#define SETTINGS_XML_KEY				_T("ETS\\SageConnector")
#define SETTINGS_XML_SETTINGS_KEY		SETTINGS_XML_KEY _T("\\Settings")

//#define SETTINGS_REGISTRY_KEY			_T("Software\\Egar\\ETS\\Sage Connector")
//#define SETTINGS_DB_REGISTRY_KEY		_T("Software\\Egar\\ETS\\Database")
#define SETTINGS_CONNSTR_KEY			_T("")



#define	MAX_DBRECONNECTION_TRIES		10
#define	DBRECONNECTION_DELAY			10

//#define TRADES_FILENAME					_T("Logs/SageTrades.log")

#define TRADES_FILENAME_BEGIN			_T("Logs/STrades")
#define TRADES_FILENAME_END				_T(".log")

#define FAILES_FILENAME_BEGIN			_T("Logs/SFailes")
#define FAILES_FILENAME_END				_T(".log")

#define STRUCTURE_FILENAME_BEGIN  	    _T("Logs/SStructure")
#define STRUCTURE_FILENAME_END			_T(".log")

#endif
