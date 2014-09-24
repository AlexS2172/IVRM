#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define ROUTER_PORT_NUMBER					8000

#define SERVER_CONNECTED					0
#define SERVER_DISCONNECTED					1
#define CONNECTING_FAILED					2
#define PROCESS_STOPED						4
#define LOGON_COMPLETED						5

#define SETTINGS_NETWORK_REGISTRY_KEY		_T("Software\\EGAR\\Common\\Transport")
#define SETTINGS_LOGS_REGISTRY_KEY			_T("Software\\EGAR\\Common\\Logs\\Transport")

#define SETTINGS_NETWORK_ROUTER_HOST		_T("RouterHost")
#define SETTINGS_NETWORK_ROUTER_PORT		_T("RouterPort")
#define SETTINGS_LOCAL_ROUTER_ADDR			_T("LocalRouterAddress")
#define SETTINGS_LOCAL_ROUTER_PORT			_T("LocalRouterPort")
#define SETTINGS_INSTALL_TIME				_T("Key")


#define MAX_HOST_NAME_LEN					100		/* Size of buffer which is used to resolve host name*/

/* timeout for losing connection*/
#define	SETTINGS_CONNECTION_LOSE_TIMEOUT   _T("ConnectionLoseTimeout")
#define	CONNECTION_LOSE_TIMEOUT				60*3  // 3 min
/* timeout for receiving connect*/
#define SETTINGS_CONNECT_TIMEOUT			_T("ConnectTimeout")
#define CONNECT_TIMEOUT						60	  // 60 sec
/* reconnect time*/
#define SETTINGS_RECONNECT_TIME				_T("ReconnectTime")
#define RECONNECT_TIME						1  // 1 sec

/* time period of resend test request message*/
#define SETTINGS_TESTREQUEST_TIMEOUT		_T("TestRequestTimeout")
#define TESTREQUEST_TIMEOUT					10 // 10 sec

#define SETTINGS_CHECK_HOSTS_CONN_PERIOD	_T("CheckHostConnPeriod")
#define CHECK_HOSTS_CONN_PERIOD				60   //  60 sec(1 min)
#define SETTINGS_HOST_CONN_EXP_TIMEOUT		_T("HostConnectExpTimeout")
#define HOST_CONN_EXP_TIMEOUT				10*60 //  10 min

#define SETTINGS_STATISTIC_TIMER			_T("StatisticTimer")
#define STATISTIC_TIMER						3 //  3 sec

#define SETTINGS_ISTRADESLOGED				_T("TraceTrades")

#define SETTINGS_LOGS_DIRECTORY				_T("Path")
#define SETTINGS_LOGS_HISTORY				_T("LogsHistory")
#define SETTINGS_LOGS_MIN_LEVEL				_T("LogsMinLevel")

#define SEC_IN_DAY							60*60*24

#define MAXULONG							0xffffffff

#define TRANSPORT_SERVICE					_T("Egar Transport")
#define TRANSPORT_SERVICE_DESC				_T("Provides service of multicasting messages delivery via TCP/IP")
#define TRANSPORT_SERVICE_DEPEND			_T("RPCSS\0")
#define ROUTER_SERVICE						_T("Egar Router")
#define ROUTER_SERVICE_DESC					_T("Provides routing of messages and delivered they to the subscribers hosts")
#define ROUTER_SERVICE_DEPEND				_T("Egar Transport\0")

#define SUBJECT_LEN					64
#define ALLOCATION_GRANULARITY		1024

#define TRANSPORT_INIT_MUTEX				_T("EGAR Transport Initialization Mutex")



#endif //__CONSTANTS_H__