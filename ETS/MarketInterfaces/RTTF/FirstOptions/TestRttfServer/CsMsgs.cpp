#include "stdafx.h"
#include "CsMsgs.h"

FocCsLoginMsg::FocCsLoginMsg
(
	const char* orig_id,
	u_long msg_seq_num,
	u_char msg_type,
	u_char msg_class,
	u_short service_id,
	const char* recip_id
)
{
	header.init(orig_id, msg_type, msg_seq_num, msg_class, service_id, recip_id);
	memset(user_id, 0, sizeof(user_id));
	memset(password, 0, sizeof(password));
	memset(last_act_date, 0, sizeof(last_act_date));
	memset(security_id, 0, sizeof(security_id));
}

FocCsLoginMsg::FocCsLoginMsg(const FocCsHeader& h) :
	FocCsMsg(h) 
{
}

bool FocCsLoginMsg::xdr_convert(XDR* xdrs, xdr_op x_op)
{
	char* str;

	if (x_op == XDR_ENCODE)
	{
		if (!header.xdr_convert(xdrs, x_op))
		{
			fprintf(stderr, "Bad XDR conversion on CS header\n");
			return FALSE;
		}
	}
	else xdrs->x_op = x_op;
	
	str = (char *) user_id;
	if (!xdr_string(xdrs, &str, sizeof(user_id)))
	{
		fprintf(stderr, "Bad XDR conversion on 'user_id'\n");
		return FALSE;
	}
	
	str = (char *) password;
	if (!xdr_string(xdrs, &str, sizeof(password)))
	{
		fprintf(stderr, "Bad XDR conversion on 'password'\n");
		return FALSE;
	}
	
	str = (char *) last_act_date;
	if (!xdr_string(xdrs, &str, sizeof(last_act_date)))
	{
		fprintf(stderr, "Bad XDR conversion on 'last_act_date'\n");
		return FALSE;
	}
	
	str = (char *) security_id;
	if (!xdr_string(xdrs, &str, sizeof(security_id)))
	{
		fprintf(stderr, "Bad XDR conversion on 'security_id'\n");
		return FALSE;
	}

	return TRUE;
}

FocCsLoginResponseMsg::FocCsLoginResponseMsg
(
	const char* orig_id,
	u_long msg_seq_num,
	u_char msg_type,
	u_char msg_class,
	u_short service_id,
	const char* recip_id
)
{
	header.init(orig_id, msg_type, msg_seq_num, msg_class, service_id, recip_id);
	login_status = 0;
	reason = 0;
	servers_up = 0;
	subscription = 0; 
	memset(te_id, 0, sizeof(te_id));
	memset(force_id, 0, sizeof(force_id));
}

FocCsLoginResponseMsg::FocCsLoginResponseMsg(const FocCsHeader& h) :
	FocCsMsg(h)
{
}

bool FocCsLoginResponseMsg::xdr_convert(XDR* xdrs, xdr_op x_op)
{
	char* str;

	if (x_op == XDR_ENCODE)
	{
		if (!header.xdr_convert(xdrs, x_op))
		{
			fprintf(stderr, "Bad XDR conversion on CS header\n");
			return FALSE;
		}
	}
	else xdrs->x_op = x_op;
	
	if (!xdr_u_int(xdrs, &login_status))
	{
		fprintf(stderr, "Bad XDR conversion on 'login_status'\n");
		return FALSE;
	}
	
	if (!xdr_u_int(xdrs, &reason))
	{
		fprintf(stderr, "Bad XDR conversion on 'reason'\n");
		return FALSE;
	}
	
	if (!xdr_u_int(xdrs, &servers_up))
	{
		fprintf(stderr, "Bad XDR conversion on 'servers_up'\n");
		return FALSE;
	}
	
	if (!xdr_u_int(xdrs, &subscription))
	{
		fprintf(stderr, "Bad XDR conversion on 'subscription'\n");
		return FALSE;
	}
	
	str = (char *) te_id;
	if (!xdr_string(xdrs, &str, sizeof(te_id)))
	{
		fprintf(stderr, "Bad XDR conversion on 'te_id'\n");
		return FALSE;
	}
	
	str = (char *) force_id;
	if (!xdr_string(xdrs, &str, sizeof(force_id)))
	{
		fprintf(stderr, "Bad XDR conversion on 'force_id'\n");
		return FALSE;
	}

	return TRUE;
}

FocCsInfoMsg::FocCsInfoMsg
(
	const char* orig_id,
	u_long msg_seq_num,
	u_char msg_type,
	u_char msg_class,
	u_short service_id,
	const char* recip_id
)
{
	header.init(orig_id, msg_type, msg_seq_num, msg_class, service_id, recip_id);
	what = 0;
	value = 0; 
	memset(value_str, 0, sizeof(value_str));
}

FocCsInfoMsg::FocCsInfoMsg(const FocCsHeader& h) :
	FocCsMsg(h)
{
}

bool FocCsInfoMsg::xdr_convert(XDR* xdrs, xdr_op x_op)
{
	char* str;

	if (x_op == XDR_ENCODE)
	{
		if (!header.xdr_convert(xdrs, x_op))
		{
			fprintf(stderr, "Bad XDR conversion on CS header\n");
			return FALSE;
		}
	}
	else xdrs->x_op = x_op;
	
	if (!xdr_u_int(xdrs, &what))
	{
		fprintf(stderr, "Bad XDR conversion on 'what'\n");
		return FALSE;
	}
	
	if (!xdr_u_int(xdrs, &value))
	{
		fprintf(stderr, "Bad XDR conversion on 'value'\n");
		return FALSE;
	}
	
	str = (char *) value_str;
	if (!xdr_string(xdrs, &str, sizeof(value_str)))
	{
		fprintf(stderr, "Bad XDR conversion on 'value_str'\n");
		return FALSE;
	}

	return TRUE;
}	
