#include "stdafx.h"
#include "RttfMsgs.h"

FocRttfTradeRequestMsg::FocRttfTradeRequestMsg
(
	const char* orig_id,
	u_long msg_seq_num,
	u_char msg_type
)
{
	header.init(orig_id, msg_type, msg_seq_num);
	request_type = 0;
	memset(&last_upd_date, 0, sizeof(last_upd_date));
}

FocRttfTradeRequestMsg::FocRttfTradeRequestMsg(const FocCsHeader& h) :
	FocCsMsg(h)
{
}

bool FocRttfTradeRequestMsg::xdr_convert(XDR* xdrs, xdr_op x_op)
{
	if (x_op == XDR_ENCODE)
	{
		if (!header.xdr_convert(xdrs, x_op))
		{
			fprintf(stderr, "Bad XDR conversion on CS header\n");
			return FALSE;
		}
	}
	else xdrs->x_op = x_op;
	
	if (!xdr_int(xdrs, &request_type))
	{
		fprintf(stderr, "Bad XDR conversion on 'request_type'\n");
		return FALSE;
	}

	if (!xdr_foc_datetime(xdrs, &last_upd_date))
	{
		fprintf(stderr, "Bad XDR conversion on 'last_upd_date'\n");
		return FALSE;
	}

	return TRUE;
}	

FocRttfLoginResponseMsg::FocRttfLoginResponseMsg
(
	const char* orig_id,
	u_long msg_seq_num,
	u_char msg_type
)
{
	header.init(orig_id, msg_type, msg_seq_num);
	status = 0;
}

FocRttfLoginResponseMsg::FocRttfLoginResponseMsg(const FocCsHeader& h) :
	FocCsMsg(h)
{
}

bool FocRttfLoginResponseMsg::xdr_convert(XDR* xdrs, xdr_op x_op)
{
	if (x_op == XDR_ENCODE)
	{
		if (!header.xdr_convert(xdrs, x_op))
		{
			fprintf(stderr, "Bad XDR conversion on CS header\n");
			return FALSE;
		}
	}
	else xdrs->x_op = x_op;

	if (!xdr_int(xdrs, &status))
	{
		fprintf(stderr, "Bad XDR conversion on 'status'\n");
		return FALSE;
	}

	return TRUE;
}	

FocRttfTradeMsg::FocRttfTradeMsg
(
	const char* orig_id,
	u_long msg_seq_num,
	u_char msg_type
)
{
	header.init(orig_id, msg_type, msg_seq_num);
}

FocRttfTradeMsg::FocRttfTradeMsg(const FocCsHeader& h) :
	FocCsMsg(h)
{
}

bool FocRttfTradeMsg::xdr_convert(XDR* xdrs, xdr_op x_op)
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

	if (!xdr_char(xdrs, &ins_upd_del_flag))
	{
		fprintf(stderr, "Bad XDR conversion on 'ins_upd_del_flag'\n");
		return FALSE;
	}
  
	if (!xdr_char(xdrs, &put_call))
	{
		fprintf(stderr, "Bad XDR conversion on 'put_call'\n");
		return FALSE;
	}

	if (!xdr_char(xdrs, &buy_sell))
	{
		fprintf(stderr, "Bad XDR conversion on 'buy_sell'\n");
		return FALSE;
	}

	if (!xdr_char(xdrs, &origin))
	{
		fprintf(stderr, "Bad XDR conversion on 'origin'\n");
		return FALSE;
	}

	if (!xdr_char(xdrs, &open_close))
	{
		fprintf(stderr, "Bad XDR conversion on 'open_close'\n");
		return FALSE;
	}
	
	str = (char *) trade_id;
	if (!xdr_string(xdrs, &str, sizeof(trade_id)))
	{
		fprintf(stderr, "Bad XDR conversion on 'trade_id'\n");
		return FALSE;
	}

	str = (char *) user_id;
	if (!xdr_string(xdrs, &str, sizeof(user_id)))
	{
		fprintf(stderr, "Bad XDR conversion on 'user_id'\n");
		return FALSE;
	}

	str = (char *) trading_sym;
	if (!xdr_string(xdrs, &str, sizeof(trading_sym)))
	{
		fprintf(stderr, "Bad XDR conversion on 'trading_sym'\n");
		return FALSE;
	}

	str = (char *) base_sym;
	if (!xdr_string(xdrs, &str, sizeof(base_sym)))
	{
		fprintf(stderr, "Bad XDR conversion on 'base_sym'\n");
		return FALSE;
	}

	str = (char *) parent_account;
	if (!xdr_string(xdrs, &str, sizeof(parent_account)))
	{
		fprintf(stderr, "Bad XDR conversion on 'parent_account'\n");
		return FALSE;
	}

	str = (char *) account;
	if (!xdr_string(xdrs, &str, sizeof(account)))
	{
		fprintf(stderr, "Bad XDR conversion on 'account'\n");
		return FALSE;
	}

	str = (char *) q_account;
	if (!xdr_string(xdrs, &str, sizeof(q_account)))
	{
		fprintf(stderr, "Bad XDR conversion on 'q_account'\n");
		return FALSE;
	}

	str = (char *) security_type;
	if (!xdr_string(xdrs, &str, sizeof(security_type)))
	{
		fprintf(stderr, "Bad XDR conversion on 'security_type'\n");
		return FALSE;
	}

	str = (char *) security_subtype;
	if (!xdr_string(xdrs, &str, sizeof(security_subtype)))
	{
		fprintf(stderr, "Bad XDR conversion on 'security_subtype'\n");
		return FALSE;
	}

	str = (char *) clearing_firm;
	if (!xdr_string(xdrs, &str, sizeof(clearing_firm)))
	{
		fprintf(stderr, "Bad XDR conversion on 'clearing_firm'\n");
		return FALSE;
	}

	str = (char *) cusip;
	if (!xdr_string(xdrs, &str, sizeof(cusip)))
	{
		fprintf(stderr, "Bad XDR conversion on 'cusip'\n");
		return FALSE;
	}

	str = (char *) basket_id;
	if (!xdr_string(xdrs, &str, sizeof(basket_id)))
	{
		fprintf(stderr, "Bad XDR conversion on 'basket_id'\n");
		return FALSE;
	}

	str = (char *) giveup_firm;
	if (!xdr_string(xdrs, &str, sizeof(giveup_firm)))
	{
		fprintf(stderr, "Bad XDR conversion on 'giveup_firm'\n");
		return FALSE;
	}

	str = (char *) contra_firm;
	if (!xdr_string(xdrs, &str, sizeof(contra_firm)))
	{
		fprintf(stderr, "Bad XDR conversion on 'contra_firm'\n");
		return FALSE;
	}

	str = (char *) exec_broker;
	if (!xdr_string(xdrs, &str, sizeof(exec_broker)))
	{
		fprintf(stderr, "Bad XDR conversion on 'exec_broker'\n");
		return FALSE;
	}

	str = (char *) contra_broker;
	if (!xdr_string(xdrs, &str, sizeof(contra_broker)))
	{
		fprintf(stderr, "Bad XDR conversion on 'contra_broker'\n");
		return FALSE;
	}

	str = (char *) mmcs_sub_acct;
	if (!xdr_string(xdrs, &str, sizeof(mmcs_sub_acct)))
	{
		fprintf(stderr, "Bad XDR conversion on 'mmcs_sub_acct'\n");
		return FALSE;
	}

	if (!xdr_double(xdrs, &exercise_multiplier))
	{
		fprintf(stderr, "Bad XDR conversion on 'exercise_multiplier'\n");
		return FALSE;
	}

	if (!xdr_double(xdrs, &extended_premium))
	{
		fprintf(stderr, "Bad XDR conversion on 'extended_premium'\n");
		return FALSE;
	}

	if (!xdr_double(xdrs, &premium))
	{
		fprintf(stderr, "Bad XDR conversion on 'premium'\n");
		return FALSE;
	}
  
	if (!xdr_double(xdrs, &strike))
	{
		fprintf(stderr, "Bad XDR conversion on 'strike'\n");
		return FALSE;
	}

	if (!xdr_double(xdrs, &price))
	{
		fprintf(stderr, "Bad XDR conversion on 'price'\n");
		return FALSE;
	}

	if (!xdr_int(xdrs, &quantity))
	{
		fprintf(stderr, "Bad XDR conversion on 'quantity'\n");
		return FALSE;
	}

	if (!xdr_int(xdrs, &tag))
	{
		fprintf(stderr, "Bad XDR conversion on 'tag'\n");
		return FALSE;
	}

	if (!xdr_foc_datetime(xdrs, &expiration_date))
	{
		fprintf(stderr, "Bad XDR conversion on 'expiration_date'\n");
		return FALSE;
	}

	if (!xdr_foc_datetime(xdrs, &execution_date))
	{
		fprintf(stderr, "Bad XDR conversion on 'execution_date'\n");
		return FALSE;
	}

	if (!xdr_foc_datetime(xdrs, &last_upd_date))
	{
		fprintf(stderr, "Bad XDR conversion on 'last_upd_date'\n");
		return FALSE;
	}

	if (!xdr_foc_datetime(xdrs, &contract_date))
	{
		fprintf(stderr, "Bad XDR conversion on 'contract_date'\n");
		return FALSE;
	}

	if (!xdr_int(xdrs, &source_system))
	{
		fprintf(stderr, "Bad XDR conversion on 'source_system'\n");
		return FALSE;
	}

	if (!xdr_int(xdrs, &trade_id_num))
	{
		fprintf(stderr, "Bad XDR conversion on 'trade_id_num'\n");
		return FALSE;
	}

	if (!xdr_int(xdrs, &trade_group_id))
	{
		fprintf(stderr, "Bad XDR conversion on 'trade_group_id'\n");
		return FALSE;
	}

	if (!xdr_foc_datetime(xdrs, &settlement_date))
	{
		fprintf(stderr, "Bad XDR conversion on 'settlement_date'\n");
		return FALSE;
	}

	str = (char *) entry_firm;
	if (!xdr_string(xdrs, &str, sizeof(entry_firm)))
	{
		fprintf(stderr, "Bad XDR conversion on 'entry_firm'\n");
		return FALSE;
	}

	if (!xdr_char(xdrs, &matched_flag))
	{
		fprintf(stderr, "Bad XDR conversion on 'matched_flag'\n");
		return FALSE;
	}

	if (!xdr_char(xdrs, &spread_ind))
	{
		fprintf(stderr, "Bad XDR conversion on 'spread_ind'\n");
		return FALSE;
	}

	if (!xdr_double(xdrs, &premium_multiplier))
	{
		fprintf(stderr, "Bad XDR conversion on 'premium_multiplier'\n");
		return FALSE;
	}

	str = (char *) source_code;
	if (!xdr_string(xdrs, &str, sizeof(source_code)))
	{
		fprintf(stderr, "Bad XDR conversion on 'source_code'\n");
		return FALSE;
	}

	str = (char *) parent_acct_intrl_id;
	if (!xdr_string(xdrs, &str, sizeof(parent_acct_intrl_id)))
	{
		fprintf(stderr, "Bad XDR conversion on 'parent_acct_intrl_id'\n");
		return FALSE;
	}

	str = (char *) acct_intrl_id;
	if (!xdr_string(xdrs, &str, sizeof(acct_intrl_id)))
	{
		fprintf(stderr, "Bad XDR conversion on 'acct_intrl_id'\n");
		return FALSE;
	}

	str = (char *) q_acct_intrl_id;
	if (!xdr_string(xdrs, &str, sizeof(q_acct_intrl_id)))
	{
		fprintf(stderr, "Bad XDR conversion on 'q_acct_intrl_id'\n");
		return FALSE;
	}

	if (!xdr_double(xdrs, &cap_interval))
	{
		fprintf(stderr, "Bad XDR conversion on 'cap_interval'\n");
		return FALSE;
	}

	if (!xdr_int(xdrs, &trade_exchange))
	{
		fprintf(stderr, "Bad XDR conversion on 'trade_exchange'\n");
		return FALSE;
	}

	if (!xdr_char(xdrs, &settled_unsettled_ind))
	{
		fprintf(stderr, "Bad XDR conversion on 'settled_unsettled_ind'\n");
		return FALSE;
	}

	if (!xdr_char(xdrs, &stock_origin))
	{
		fprintf(stderr, "Bad XDR conversion on 'stock_origin'\n");
		return FALSE;
	}

	str = (char *) mm_card_number;
	if (!xdr_string(xdrs, &str, sizeof(mm_card_number)))
	{
		fprintf(stderr, "Bad XDR conversion on 'mm_card_number'\n");
		return FALSE;
	}

	if (!xdr_int(xdrs, &basket_tag))
	{
		fprintf(stderr, "Bad XDR conversion on 'basket_tag'\n");
		return FALSE;
	}

	if (!xdr_double(xdrs, &basket_price))
	{
		fprintf(stderr, "Bad XDR conversion on 'basket_price'\n");
		return FALSE;
	}

	if (!xdr_char(xdrs, &short_sale))
	{
		fprintf(stderr, "Bad XDR conversion on 'short_sale'\n");
		return FALSE;
	}

	str = (char *) commission_code;
	if (!xdr_string(xdrs, &str, sizeof(commission_code)))
	{
		fprintf(stderr, "Bad XDR conversion on 'commission_code'\n");
		return FALSE;
	}

	if (!xdr_double(xdrs, &commission))
	{
		fprintf(stderr, "Bad XDR conversion on 'commission'\n");
		return FALSE;
	}

	if (!xdr_char(xdrs, &same_day_exer))
	{
		fprintf(stderr, "Bad XDR conversion on 'same_day_exer'\n");
		return FALSE;
	}

	str = (char *) terminal_name;
	if (!xdr_string(xdrs, &str, sizeof(terminal_name)))
	{
		fprintf(stderr, "Bad XDR conversion on 'terminal_name'\n");
		return FALSE;
	}

	return TRUE;
}	

FocRttfPositionMsg::FocRttfPositionMsg
(
	const char* orig_id,
	u_long msg_seq_num,
	u_char msg_type
)
: FocRttfTradeMsg(orig_id, msg_seq_num, msg_type)
{
}

FocRttfPositionMsg::FocRttfPositionMsg(const FocCsHeader& h)
: FocRttfTradeMsg(h)
{
}

bool FocRttfPositionMsg::xdr_convert(XDR* xdrs, xdr_op x_op)
{
	return (FocRttfTradeMsg::xdr_convert(xdrs, x_op));
}
