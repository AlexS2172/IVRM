#ifndef __CSTYPES_H__
#define __CSTYPES_H__

#include <time.h>

#ifdef _WIN32
#include "XDR.h"
#else
#include <rpc/rpc.h>
#endif

// CS Message Field Lengths
#define	FOC_ID_LEN						6
#define  FOC_USER_ID_LEN				10
#define	FOC_PASSWORD_LEN				10
#define	FOC_SECURITY_ID_LEN			10
#define	FOC_LAST_ACT_DATE_LEN		5
#define	FOC_TE_ID_LEN					11
#define	FOC_FORCE_ID_LEN				11

// CS System Service IDs
#define	FOC_SRVC_LOGIN_RQST			1
#define	FOC_SRVC_LOGIN_RE_RQST		2
#define	FOC_SRVC_LOGOUT_RQST			3
#define	FOC_SRVC_LOGIN_RESPONSE		4
#define	FOC_SRVC_INFO					5
#define	FOC_SRVC_USERS_LIST			6
#define	FOC_SRVC_LOGIN_RQST_NODL	7
#define	FOC_SRVC_CSID					8

// CS Application Service IDs
#define	FOC_SRVC_RTTF					10

// CS Message Classes 
#define	FOC_CLASS_APPLICATION		1
#define	FOC_CLASS_SYSTEM				2

// CS System Message Types
#define	FOC_MSG_LOGIN					1
#define	FOC_MSG_LOGIN_RESPONSE		2
#define	FOC_MSG_INFO					3
#define	FOC_MSG_LOGOUT					0

// CS Info Message Codes
#define	FOC_ERR_NONE					0
#define	FOC_ERR_INTERNAL				1
#define	FOC_ERR_SERVER_NOT_RESPONDING	2
#define	FOC_ERR_RELOGON_ATTEMPTED	3	
#define	FOC_ERR_INVALID_USERID		4
#define	FOC_ERR_USER_CONN_BROKEN	5
#define	FOC_ERR_NO_SUBSCRIPTION		6
#define	FOC_ERR_INVALID_PASSWD		7
#define	FOC_ERR_INVALID_SECURE_ID	8
#define	FOC_SERVER_UP					9
#define	FOC_ERR_UNKNOWN_SERVICE_ID	10
#define	FOC_ERR_UNKNOWN_MSG_CLASS	11
#define	FOC_ERR_LOGIN_REQUIRED		12

// FOC_DATETIME structure

struct FOC_DATETIME 
{
	FOC_DATETIME();
	FOC_DATETIME(time_t t);
	FOC_DATETIME(const char * s);

	operator time_t() const;
	
	bool operator>(const FOC_DATETIME& rhs) const;
	bool operator>=(const FOC_DATETIME& rhs) const;
	bool operator<(const FOC_DATETIME& rhs) const;
	bool operator<=(const FOC_DATETIME& rhs) const;
	bool operator==(const FOC_DATETIME& rhs) const;

	operator SYSTEMTIME() const;

	u_char sec;		// [0, 59]
	u_char min;		// [0, 59] 
	u_char hour;		// [0, 23]
	u_char day;		// [1, 31]
	u_char month;		// [0, 11]
	u_short year;		// 4 digit year
};

// FOC_DATETIME XDR prototype
bool_t xdr_foc_datetime(XDR* xdrs, FOC_DATETIME* dttm);

// CS header structure
typedef struct
{
	u_char orig_id[FOC_ID_LEN];
	u_char recip_id[FOC_ID_LEN];
	u_char msg_priority;
	u_char msg_class;
	u_char num_items;
	u_char msg_type;
	u_short service_id;
	u_short msg_time;
	u_long msg_seq_num;
} foc_cs_header;

// CS login fields
typedef struct
{
	u_char user_id[FOC_USER_ID_LEN];
	u_char password[FOC_PASSWORD_LEN];
	u_char last_act_date[FOC_LAST_ACT_DATE_LEN];
	u_char security_id[FOC_SECURITY_ID_LEN];
} foc_login;

// CS logout fields
typedef struct
{
	u_char foc_user[FOC_USER_ID_LEN];
} foc_logout;

// CS login response fields
typedef struct
{
	u_int login_status;
	u_int reason;
	u_int servers_up;
	u_int subscription;
	u_char te_id[FOC_TE_ID_LEN];
	u_char force_id[FOC_FORCE_ID_LEN];
} foc_login_response;

// CS info message fields
typedef struct
{
	u_int what;
	u_int value;
	u_char value_str[FOC_USER_ID_LEN];	
} foc_info;

class FocCsHeader : public foc_cs_header
{
public:
	FocCsHeader();
	FocCsHeader(const foc_cs_header& h) : foc_cs_header(h) {}
	
	void init
	(
		const char* p_orig_id,
		u_char p_msg_type,
		u_long p_msg_seq_num = 1,
		u_char p_msg_class = FOC_CLASS_APPLICATION,		
		u_short p_service_id = FOC_SRVC_RTTF,
		const char* p_recip_id = "_RT_4",
		u_char p_num_items = 1,
		u_char p_msg_priority = 5
	);
	
	bool xdr_convert(XDR* xdrs, xdr_op x_op);
};

class FocCsMsg 
{
public:
	FocCsMsg() {};
	FocCsMsg(const FocCsHeader& h) { header = h; }
	virtual ~FocCsMsg() {};

	virtual bool xdr_convert(XDR* xdrs, xdr_op x_op) = 0;

	FocCsHeader header;
};

#endif
