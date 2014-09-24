#ifndef __CSMSGS_H__
#define __CSMSGS_H__

#include "CsTypes.h"

class FocCsLoginMsg : public FocCsMsg, public foc_login
{
public:
	FocCsLoginMsg
	(
		const char* orig_id,
		u_long msg_seq_num = 1,
		u_char msg_type = FOC_MSG_LOGIN, 
		u_char msg_class = FOC_CLASS_SYSTEM,		
		u_short service_id = FOC_SRVC_LOGIN_RQST,
		const char* recip_id = ""
	);
	FocCsLoginMsg(const FocCsHeader& h);
	bool xdr_convert(XDR* xdrs, xdr_op x_op);
};

class FocCsLogoutMsg : public FocCsMsg
{
public:
	FocCsLogoutMsg(const char * _user_id);

	bool xdr_convert(XDR* xdrs, xdr_op x_op);
};


class FocCsLoginResponseMsg : public FocCsMsg, public foc_login_response
{
public:
	FocCsLoginResponseMsg
	(
		const char* orig_id,
		u_long msg_seq_num = 1,
		u_char msg_type = FOC_MSG_LOGIN_RESPONSE, 
		u_char msg_class = FOC_CLASS_SYSTEM,		
		u_short service_id = FOC_SRVC_LOGIN_RESPONSE,
		const char* recip_id = ""
	);
	FocCsLoginResponseMsg(const FocCsHeader& h);
	bool xdr_convert(XDR* xdrs, xdr_op x_op);
};

class FocCsInfoMsg : public FocCsMsg, public foc_info
{
public:
	FocCsInfoMsg
	(
		const char* orig_id,
		u_long msg_seq_num = 1,
		u_char msg_type = FOC_MSG_INFO, 
		u_char msg_class = FOC_CLASS_SYSTEM,		
		u_short service_id = FOC_SRVC_INFO,
		const char* recip_id = ""
	);
	FocCsInfoMsg(const FocCsHeader& h);
	bool xdr_convert(XDR* xdrs, xdr_op x_op);

};

#endif
