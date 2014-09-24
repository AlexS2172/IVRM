#ifndef __RTTFMSGS_H__
#define __RTTFMSGS_H__

#include "CsMsgs.h"

// Rttf Field Lengths
#define	TRADE_ID_LEN						10
#define	USER_ID_LEN							11
#define	SYMBOL_LEN							7
#define	ACCOUNTS_LEN						11	
#define	SECURITY_LEN						3
#define	FIRM_LEN								5	
#define	CUSIP_LEN							10
#define	BASKET_ID_LEN						6
#define	BROKER_LEN							4
#define	MMCS_SUB_ACCT_LEN					3
#define	SOURCE_CODE_LEN					3
#define	INTRL_ID_LEN						14
#define	MM_CARD_NUM_LEN					8
#define	COMMISSION_CODE_LEN				3
#define	TERMINAL_NAME_LEN					11

// RTTF Application Message Types
#define	FOC_RTTF_TRADE_MSG				1
#define	FOC_RTTF_TRADEREQ_MSG			2
#define	FOC_RTTF_LOGINRESP_MSG			3

// RTTF login response status codes
#define	RTTF_NO_ERROR						1
#define	RTTF_INTERNAL_ERROR				2
#define	RTTF_NO_SUBSCRIPTION				3

// RTTF request types
#define	RTTF_OPENING_POS_ONLY			1
#define	RTTF_OPENING_POS_AND_TRADES	2
#define	RTTF_TRADES_ONLY					3

// Rttf trade fields
typedef struct
{
	char ins_upd_del_flag;
	char put_call;
	char buy_sell;
	char origin;
	char open_close;
	char trade_id[TRADE_ID_LEN];
	char user_id[USER_ID_LEN];
	char trading_sym[SYMBOL_LEN];
	char base_sym[SYMBOL_LEN];
	char parent_account[ACCOUNTS_LEN];
	char account[ACCOUNTS_LEN];
	char q_account[ACCOUNTS_LEN];
	char security_type[SECURITY_LEN];
	char security_subtype[SECURITY_LEN];
	char clearing_firm[FIRM_LEN];
	char cusip[CUSIP_LEN];
	char basket_id[BASKET_ID_LEN];
	char giveup_firm[FIRM_LEN];	
	char contra_firm[FIRM_LEN];
	char exec_broker[BROKER_LEN];
	char contra_broker[BROKER_LEN];
	char mmcs_sub_acct[MMCS_SUB_ACCT_LEN];
	double exercise_multiplier;
	double extended_premium;
	double premium;
	double strike;
	double price;
	int quantity;
	int tag;
	FOC_DATETIME expiration_date;
	FOC_DATETIME execution_date;
	FOC_DATETIME last_upd_date;
	FOC_DATETIME contract_date;
	int source_system;
	int trade_id_num;
	int trade_group_id;
	FOC_DATETIME settlement_date;
	char entry_firm[FIRM_LEN];
	char matched_flag;
	char spread_ind;
	double premium_multiplier;
	char source_code[SOURCE_CODE_LEN];
	char parent_acct_intrl_id[INTRL_ID_LEN];
	char acct_intrl_id[INTRL_ID_LEN];
	char q_acct_intrl_id[INTRL_ID_LEN];
	double cap_interval;
	int trade_exchange;
	char settled_unsettled_ind;
	char stock_origin;
	char mm_card_number[MM_CARD_NUM_LEN];
	int basket_tag;
	double basket_price;
	char short_sale;
	char commission_code[COMMISSION_CODE_LEN];
	double commission;
	char same_day_exer;
	char terminal_name[TERMINAL_NAME_LEN];	
} foc_rttf_trade;

// RTTF trade request fields
typedef struct
{
	int request_type;
	FOC_DATETIME last_upd_date;
} foc_rttf_trade_request;

// RTTF login response fields
typedef struct
{
	int status;
} foc_rttf_login_response;

class FocRttfTradeMsg : public FocCsMsg, public virtual foc_rttf_trade
{
public:
	FocRttfTradeMsg
	(
		const char* orig_id,
		u_long msg_seq_num = 1,
		u_char msg_type = FOC_RTTF_TRADE_MSG 
	);
	FocRttfTradeMsg(const FocCsHeader& h);
	bool xdr_convert(XDR* xdrs, xdr_op x_op);
};

class FocRttfPositionMsg : public FocRttfTradeMsg
{
public:
	FocRttfPositionMsg
	(
		const char* orig_id,
		u_long msg_seq_num = 1,
		u_char msg_type = FOC_RTTF_TRADE_MSG 
	);
	FocRttfPositionMsg(const FocCsHeader& h);
	bool xdr_convert(XDR* xdrs, xdr_op x_op);
};

class FocRttfTradeRequestMsg : public FocCsMsg, public foc_rttf_trade_request
{
public:
	FocRttfTradeRequestMsg
	(
		const char* orig_id,
		u_long msg_seq_num = 1,
		u_char msg_type = FOC_RTTF_TRADEREQ_MSG 
	);
	FocRttfTradeRequestMsg(const FocCsHeader& h);
	bool xdr_convert(XDR* xdrs, xdr_op x_op);
};

class FocRttfLoginResponseMsg : public FocCsMsg, public foc_rttf_login_response
{
public:
	FocRttfLoginResponseMsg
	(
		const char* orig_id,
		u_long msg_seq_num = 1,
		u_char msg_type = FOC_RTTF_LOGINRESP_MSG 
	);
	FocRttfLoginResponseMsg(const FocCsHeader& h);
	bool xdr_convert(XDR* xdrs, xdr_op x_op);
};

#endif
