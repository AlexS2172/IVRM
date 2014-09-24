#include "stdafx.h"

#include "data.h"

#define __GET				if(f.eof()) break; f >> s; 
#define __GET_CHAR(member)	__GET; trade.##member = *s.c_str();
#define __GET_STRING(member, len)	__GET; trade.##member[len-1] = 0; strncpy(trade.##member,s.c_str(), len-1);
#define __GET_DOUBLE(member)	__GET; trade.##member = atof(s.c_str());
#define __GET_INT(member)	__GET; trade.##member = atoi(s.c_str());
#define __GET_NOW_TIME(member)	__GET; trade.##member = FOC_DATETIME(s.c_str());

trades_t	g_Trades;

BOOL _FUNC_ LoadTrades()
{
	foc_rttf_trade trade;

	fstream f(TRADES_FILE, ios_base::in);

	if(!f)
		return FALSE;

	char buff[4096];
	string s;
	f.getline(buff, 4096);

	while(true)
	{
		__GET_CHAR(ins_upd_del_flag);
		__GET_CHAR(put_call);
		__GET_CHAR(buy_sell);
		__GET_CHAR(origin);
		__GET_CHAR(open_close);
		__GET_STRING(trade_id, TRADE_ID_LEN);
		__GET_STRING(user_id, USER_ID_LEN);
		__GET_STRING(trading_sym, SYMBOL_LEN);
		__GET_STRING(base_sym, SYMBOL_LEN);
		__GET_STRING(parent_account, ACCOUNTS_LEN);
		__GET_STRING(account, ACCOUNTS_LEN);
		__GET_STRING(q_account, ACCOUNTS_LEN);
		__GET_STRING(security_type, SECURITY_LEN);
		__GET_STRING(security_subtype, SECURITY_LEN);		
		__GET_STRING(clearing_firm, FIRM_LEN);
		__GET_STRING(cusip, FIRM_LEN);
		__GET_STRING(basket_id, BASKET_ID_LEN);
		__GET_STRING(giveup_firm, FIRM_LEN);
		__GET_STRING(contra_firm, FIRM_LEN);
		__GET_STRING(exec_broker, BROKER_LEN);
		__GET_STRING(contra_broker, BROKER_LEN);
		__GET_STRING(mmcs_sub_acct, MMCS_SUB_ACCT_LEN);
		__GET_DOUBLE(exercise_multiplier);
		__GET_DOUBLE(extended_premium);
		__GET_DOUBLE(premium);
		__GET_DOUBLE(strike);
		__GET_DOUBLE(price);
		__GET_INT(quantity);
		__GET_INT(tag);
		__GET_NOW_TIME(expiration_date);
		__GET_NOW_TIME(execution_date);
		__GET_NOW_TIME(last_upd_date);
		__GET_NOW_TIME(contract_date);
		__GET_INT(source_system);
		__GET_INT(trade_id_num);
		__GET_INT(trade_group_id);
		__GET_NOW_TIME(settlement_date);
		__GET_STRING(entry_firm, FIRM_LEN);
		__GET_CHAR(matched_flag);
		__GET_CHAR(spread_ind);
		__GET_DOUBLE(premium_multiplier);
		__GET_STRING(source_code, SOURCE_CODE_LEN);
		__GET_STRING(parent_acct_intrl_id, INTRL_ID_LEN);
		__GET_STRING(acct_intrl_id, INTRL_ID_LEN);
		__GET_STRING(q_acct_intrl_id, INTRL_ID_LEN);
		__GET_DOUBLE(cap_interval);
		__GET_INT(trade_exchange);
		__GET_CHAR(settled_unsettled_ind);
		__GET_CHAR(stock_origin);
		__GET_STRING(mm_card_number, MM_CARD_NUM_LEN);
		__GET_INT(basket_tag);
		__GET_DOUBLE(basket_price);
		__GET_CHAR(short_sale);
		__GET_STRING(commission_code, COMMISSION_CODE_LEN);
		__GET_DOUBLE(commission);
		__GET_CHAR(same_day_exer);
		__GET_STRING(terminal_name, TERMINAL_NAME_LEN);

		g_Trades.push_back(trade);
	};

	_tprintf(_T("Trades count read : %d\n"), g_Trades.size());

	return TRUE;
}