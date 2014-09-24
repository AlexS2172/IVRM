#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "clearingbase.h"
#include <string>

#define PORTFOLIO_NAME_DEFAULT		"DefaultPortfolio"

/*
	Rule format:
		FieldName1=Mask1;FieldName2=Mask2;...
	Mask format:
		1. Examples for strings
			Mask = "MSFT"
			Mask = "MS*"
			Mask = "*SFT"
			Mask = "M*F*"
		2. Examples for integer and double values
			Mask = "1 10"	- from 1 to 10 inclusive
			Mask = "-1"		- from -1 to infinity 
			Mask = "-4.5 8.98"	- for double
		3. Time are represented as time_t (long) values
*/

typedef enum
{
	MEMBER_TYPE_INT,
	MEMBER_TYPE_DOUBLE,
	MEMBER_TYPE_CHAR,
	MEMBER_TYPE_STRING,
	MEMBER_TYPE_TIME
} rttf_trade_member_type_t;

struct rttf_trade_member_t
{
	int							m_iOffset;		// offset of member in foc_rttf_trade structure
	rttf_trade_member_type_t	m_Type;			// type of field
	int							m_iLength;		// length of field (for strings only)
};

typedef map<string, rttf_trade_member_t>	trade_members_t;
typedef trade_members_t::iterator			trade_members_it_t;

typedef union
{
	int			m_IntValue;
	double		m_DoubleValue;
	char		m_CharValue;
	time_t		m_TimeValue;
}	limit_value_t;

typedef struct
{
	bool			m_bEnable;
	limit_value_t	m_Value;
}	limit_t;

typedef struct 
{
	limit_t			m_HiLimit;
	limit_t			m_LowLimit;
} num_limits_t;

typedef struct
{
	string			m_StringMask;
	num_limits_t	m_NumMask;
} limits_t;

struct	rttf_trade_filter_t
{
	rttf_trade_member_type_t	m_Type;
	vector<limits_t>			m_Filter;
};

typedef map<string, rttf_trade_filter_t>	rule_items_t;
typedef rule_items_t::iterator				rule_items_it_t;

typedef map<tstring, rule_items_t>			rules_t;
typedef rules_t::iterator					rules_it_t;

typedef vector<tstring>						rule_by_prios_t;
typedef rule_by_prios_t::iterator			rule_by_prios_it_t;

class CPortfolio
{
private:

	trade_members_t		m_TradeMembers;
	rules_t				m_Rules;
	rule_by_prios_t		m_RuleByPrios;

private:

	void FillMembersInfo();

	HRESULT	FillRules();

	BOOL _METH_	ParseMask(	const _bstr_t & bstrPortfolioName, 
							const _bstr_t & bstrItemName,
							const _bstr_t & bstrMask,
							const _bstr_t & bstrTypeName);


public:

	DWORD Init();

	DWORD Classify(const foc_rttf_trade & Trade, string & RortfolioName);

	DWORD Exit();
};

BOOL MatchWithPattern( PCHAR Pattern, PCHAR Name );

struct foc_rttf_trade_ex : virtual public foc_rttf_trade
{
	tstring				m_Portfolio;
	mutable std::string	m_sType;
	
	string MakeOptionSymbol();
};

struct FocRttfTradeMsgEx : public foc_rttf_trade_ex, public FocRttfTradeMsg
{
	FocRttfTradeMsgEx
	(
		const char* orig_id,
		u_long msg_seq_num = 1,
		u_char msg_type = FOC_RTTF_TRADE_MSG 
	):FocRttfTradeMsg(orig_id, msg_seq_num, msg_type){};

	FocRttfTradeMsgEx(const FocCsHeader& h):FocRttfTradeMsg(h){};
};

struct FocRttfPositionMsgEx : public foc_rttf_trade_ex, public FocRttfPositionMsg
{
	FocRttfPositionMsgEx
	(
		const char* orig_id,
		u_long msg_seq_num = 1,
		u_char msg_type = FOC_RTTF_TRADE_MSG 
	):FocRttfPositionMsg(orig_id, msg_seq_num, msg_type){};

	FocRttfPositionMsgEx(const FocCsHeader& h):FocRttfPositionMsg(h){};
};

#endif