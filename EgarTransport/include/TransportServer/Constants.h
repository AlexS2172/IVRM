#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define CONNECTION_MANAGER_PORT_NUMBER		8000
#define PUB_SERVER_PORT_NUMBER				8002

#define SETTINGS_NETWORK_REGISTRY_KEY				_T("Software\\Egar\\Network\\Transport")
#define SETTINGS_NETWORK_CONNECTION_MANAGER_HOST	_T("ConnectionManagerHost")
#define SETTINGS_NETWORK_CONNECTION_MANAGER_PORT	_T("ConnectionManagerPort")
#define SETTINGS_NETWORK_DBGLOG_LEVEL	            _T("DebugLogLevel")
#define SETTINGS_NETWORK_PROVIDER	                _T("Provider")
#define SETTINGS_NETWORK_PROVIDER_EHAUL	            _T("ehaul")

#define MAX_HOST_NAME_LEN					100		/* Size of buffer which is used to resolve host name*/

#endif //__CONSTANTS_H__