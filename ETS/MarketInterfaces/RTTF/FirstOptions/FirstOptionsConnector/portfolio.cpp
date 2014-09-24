#include "stdafx.h"

#include "portfolio.h"
#include "Publisher.h"
#include "trace.h"

#define __INIT_CHAR(member_name)	TradeMember.m_Type = MEMBER_TYPE_CHAR;\
	TradeMember.m_iOffset = offsetof(foc_rttf_trade, member_name);\
	TradeMember.m_iLength = -1;\
	m_TradeMembers[#member_name] = TradeMember;

#define __INIT_STRING(member_name, len)	TradeMember.m_Type = MEMBER_TYPE_STRING;\
	TradeMember.m_iOffset = offsetof(foc_rttf_trade, member_name);\
	TradeMember.m_iLength = len;\
	m_TradeMembers[#member_name] = TradeMember;

#define __INIT_DOUBLE(member_name)	TradeMember.m_Type = MEMBER_TYPE_DOUBLE;\
	TradeMember.m_iOffset = offsetof(foc_rttf_trade, member_name);\
	TradeMember.m_iLength = -1;\
	m_TradeMembers[#member_name] = TradeMember;

#define __INIT_INT(member_name)	TradeMember.m_Type = MEMBER_TYPE_INT;\
	TradeMember.m_iOffset = offsetof(foc_rttf_trade, member_name);\
	TradeMember.m_iLength = -1;\
	m_TradeMembers[#member_name] = TradeMember;

#define __INIT_TIME(member_name)	TradeMember.m_Type = MEMBER_TYPE_TIME;\
	TradeMember.m_iOffset = offsetof(foc_rttf_trade, member_name);\
	TradeMember.m_iLength = -1;\
	m_TradeMembers[#member_name] = TradeMember;

void CPortfolio::FillMembersInfo()
{
	rttf_trade_member_t TradeMember;

	__INIT_CHAR(ins_upd_del_flag);
	__INIT_CHAR(put_call);
	__INIT_CHAR(buy_sell);
	__INIT_CHAR(origin);
	__INIT_CHAR(open_close);
	__INIT_STRING(trade_id, TRADE_ID_LEN);
	__INIT_STRING(user_id, USER_ID_LEN);
	__INIT_STRING(trading_sym, SYMBOL_LEN);
	__INIT_STRING(base_sym, SYMBOL_LEN);
	__INIT_STRING(parent_account, ACCOUNTS_LEN);
	__INIT_STRING(account, ACCOUNTS_LEN);
	__INIT_STRING(q_account, ACCOUNTS_LEN);
	__INIT_STRING(security_type, SECURITY_LEN);
	__INIT_STRING(security_subtype, SECURITY_LEN);		
	__INIT_STRING(clearing_firm, FIRM_LEN);
	__INIT_STRING(cusip, FIRM_LEN);
	__INIT_STRING(basket_id, BASKET_ID_LEN);
	__INIT_STRING(giveup_firm, FIRM_LEN);
	__INIT_STRING(contra_firm, FIRM_LEN);
	__INIT_STRING(exec_broker, BROKER_LEN);
	__INIT_STRING(contra_broker, BROKER_LEN);
	__INIT_STRING(mmcs_sub_acct, MMCS_SUB_ACCT_LEN);
	__INIT_DOUBLE(exercise_multiplier);
	__INIT_DOUBLE(extended_premium);
	__INIT_DOUBLE(premium);
	__INIT_DOUBLE(strike);
	__INIT_DOUBLE(price);
	__INIT_INT(quantity);
	__INIT_INT(tag);
	__INIT_TIME(expiration_date);
	__INIT_TIME(execution_date);
	__INIT_TIME(last_upd_date);
	__INIT_TIME(contract_date);
	__INIT_INT(source_system);
	__INIT_INT(trade_id_num);
	__INIT_INT(trade_group_id);
	__INIT_TIME(settlement_date);
	__INIT_STRING(entry_firm, FIRM_LEN);
	__INIT_CHAR(matched_flag);
	__INIT_CHAR(spread_ind);
	__INIT_DOUBLE(premium_multiplier);
	__INIT_STRING(source_code, SOURCE_CODE_LEN);
	__INIT_STRING(parent_acct_intrl_id, INTRL_ID_LEN);
	__INIT_STRING(acct_intrl_id, INTRL_ID_LEN);
	__INIT_STRING(q_acct_intrl_id, INTRL_ID_LEN);
	__INIT_DOUBLE(cap_interval);
	__INIT_INT(trade_exchange);
	__INIT_CHAR(settled_unsettled_ind);
	__INIT_CHAR(stock_origin);
	__INIT_STRING(mm_card_number, MM_CARD_NUM_LEN);
	__INIT_INT(basket_tag);
	__INIT_DOUBLE(basket_price);
	__INIT_CHAR(short_sale);
	__INIT_STRING(commission_code, COMMISSION_CODE_LEN);
	__INIT_DOUBLE(commission);
	__INIT_CHAR(same_day_exer);
	__INIT_STRING(terminal_name, TERMINAL_NAME_LEN);
}

BOOL _METH_	CPortfolio::ParseMask(	const _bstr_t & bstrPortfolioName, 
									const _bstr_t & bstrItemName,
									const _bstr_t & bstrMask,
									const _bstr_t & bstrTypeName)
{
	int nLimits = 0;

	rule_items_t	&			RuleItems = m_Rules[(TCHAR*)bstrPortfolioName];
	rttf_trade_filter_t &		Filter = RuleItems[(TCHAR*)bstrItemName];

	limits_t	Limits;

	if( bstrTypeName == _bstr_t( _T("string")))
	{
		Filter.m_Type = MEMBER_TYPE_STRING;
		Limits.m_StringMask = bstrMask;
		nLimits = 1;
	}
	else if(bstrTypeName == _bstr_t( _T("double")))
	{
		Filter.m_Type = MEMBER_TYPE_DOUBLE;
		nLimits = _stscanf((TCHAR*)bstrMask, "%lf%lf", 
			&Limits.m_NumMask.m_LowLimit.m_Value.m_DoubleValue,
			&Limits.m_NumMask.m_HiLimit.m_Value.m_DoubleValue);

	}
	else if(bstrTypeName == _bstr_t(_T("int")))
	{
		Filter.m_Type = MEMBER_TYPE_INT;
		nLimits = _stscanf((TCHAR*)bstrMask, "%d%d", 
			&Limits.m_NumMask.m_LowLimit.m_Value.m_IntValue,
			&Limits.m_NumMask.m_HiLimit.m_Value.m_IntValue);
	}
	else if(bstrTypeName == _bstr_t(_T("char")))
	{
		Filter.m_Type = MEMBER_TYPE_CHAR;
		char cSpace;
		nLimits = _stscanf((TCHAR*)bstrMask, "%c%c%c", 
			&Limits.m_NumMask.m_LowLimit.m_Value.m_CharValue,
			&cSpace,
			&Limits.m_NumMask.m_HiLimit.m_Value.m_CharValue);
	}
	else if(bstrTypeName == _bstr_t(_T("time")))
	{
		Filter.m_Type = MEMBER_TYPE_TIME;
		nLimits = _stscanf((TCHAR*)bstrMask, "%d%d", 
			&Limits.m_NumMask.m_LowLimit.m_Value.m_TimeValue,
			&Limits.m_NumMask.m_HiLimit.m_Value.m_TimeValue);
	}

	Limits.m_NumMask.m_LowLimit.m_bEnable = true;

	if(nLimits == 0)
	{
		return FALSE;
	}
	else if(nLimits == 1)
	{
		Limits.m_NumMask.m_HiLimit.m_bEnable = false;
	}
	else
	{
		Limits.m_NumMask.m_HiLimit.m_bEnable = true;
	}

	Filter.m_Filter.push_back(Limits);

	return TRUE;
}

HRESULT CPortfolio::FillRules()
{
/*
	_bstr_t bstrConnectionString;

	HRESULT hr = GetConnectionString(bstrConnectionString);
	if(FAILED(hr))
		return hr;

	CDBConnection Connection;

	try
	{
		Connection.Open(bstrConnectionString);

		CClientRecordset rs;		
		CStoredProc<> spGetAllRules(Connection, L"usp_BookRule_GetAll");
		
		rs.Open(spGetAllRules);
		for (;!rs.IsEOF(); ++rs )
		{
			_bstr_t bstrPortfolioName	= rs[L"PortfolioName"];
			_bstr_t bstrItemName		= rs[L"ItemName"];
			_bstr_t bstrMask			= rs[L"Mask"];
			_bstr_t bstrTypeName		= rs[L"TypeName"];

			if(m_Rules.find( (TCHAR*)bstrPortfolioName) == m_Rules.end())
			{
				m_RuleByPrios.push_back( (TCHAR*)bstrPortfolioName);
			}
			
			if( ParseMask(bstrPortfolioName, 
				  		  bstrItemName,
						  bstrMask,
						  bstrTypeName) == FALSE)
				Trace( _T("Invalid rule %s for item %s"), 
						(TCHAR*)bstrPortfolioName, (TCHAR*)bstrItemName);
		}

		rs.Close();
	} 
	catch(const _com_error & e)
	{
		Trace(_T("Failed to retrieve rules from DB : %s"), (TCHAR*)Connection.GetErrorMessage());
		return e.Error();
	}
*/

	return S_OK;
}

DWORD CPortfolio::Init()
{
	FillMembersInfo();

	return FillRules();
};

DWORD CPortfolio::Exit()
{
	m_TradeMembers.clear();
	m_Rules.clear();
	return 0;
}

DWORD CPortfolio::Classify(const foc_rttf_trade & Trade, string & RortfolioName)
{
	RortfolioName = PORTFOLIO_NAME_DEFAULT;

	for(rule_by_prios_it_t PrioIt = m_RuleByPrios.begin(); PrioIt != m_RuleByPrios.end(); PrioIt++)
	{
		rules_it_t RulesIt = m_Rules.find(*PrioIt);
		if(RulesIt == m_Rules.end())
			continue;

		rule_items_t & RuleItems = RulesIt->second;
		const string & RuleName = RulesIt->first;

		bool  bRuleWorks = false;
		
		for(rule_items_it_t ItemsIt = RuleItems.begin(); ItemsIt != RuleItems.end(); ItemsIt++)
		{
			const rttf_trade_filter_t & Member = ItemsIt->second;
			const string & MemberName = ItemsIt->first;

			const vector<limits_t> & Filter = Member.m_Filter;

			bRuleWorks = false;

			// find member info 
			trade_members_it_t MemberIt = m_TradeMembers.find(MemberName);
			if(MemberIt == m_TradeMembers.end())
				break;

			const rttf_trade_member_t & MemberInfo = MemberIt->second;

			// type cast
			if(MemberInfo.m_Type != Member.m_Type)
				break;

			switch(MemberInfo.m_Type)
			{
				case MEMBER_TYPE_INT:
				{
					int iValue = *(int*)((char*)&Trade + MemberInfo.m_iOffset);
					for(vector<limits_t>::const_iterator it = Filter.begin(); it != Filter.end(); it++)
					{
						bRuleWorks = it->m_NumMask.m_HiLimit.m_bEnable ?
							(iValue >= it->m_NumMask.m_LowLimit.m_Value.m_IntValue && 
							iValue <= it->m_NumMask.m_HiLimit.m_Value.m_IntValue) :
							(iValue >= it->m_NumMask.m_LowLimit.m_Value.m_IntValue);
						if(bRuleWorks)
							break;
					}
					break;
				}

				case MEMBER_TYPE_DOUBLE:
				{
					double fValue = *(double*)((char*)&Trade + MemberInfo.m_iOffset);
					for(vector<limits_t>::const_iterator it = Filter.begin(); it != Filter.end(); it++)
					{
						bRuleWorks = it->m_NumMask.m_HiLimit.m_bEnable ?
							(fValue >= it->m_NumMask.m_LowLimit.m_Value.m_DoubleValue && 
							fValue <= it->m_NumMask.m_HiLimit.m_Value.m_DoubleValue) :
							(fValue >= it->m_NumMask.m_LowLimit.m_Value.m_DoubleValue);
						if(bRuleWorks)
							break;
					}
					break;
				}

				case MEMBER_TYPE_CHAR:
				{
					char cValue = *((char*)&Trade + MemberInfo.m_iOffset);
					for(vector<limits_t>::const_iterator it = Filter.begin(); it != Filter.end(); it++)
					{
						bRuleWorks = it->m_NumMask.m_HiLimit.m_bEnable ?
							(cValue >= it->m_NumMask.m_LowLimit.m_Value.m_CharValue && 
							cValue <= it->m_NumMask.m_HiLimit.m_Value.m_CharValue) :
							(cValue >= it->m_NumMask.m_LowLimit.m_Value.m_CharValue);
						if(bRuleWorks)
							break;
					}
					break;
				}


				case MEMBER_TYPE_STRING:
				{
					for(vector<limits_t>::const_iterator it = Filter.begin(); it != Filter.end(); it++)
					{
						string strValue((char*)&Trade + MemberInfo.m_iOffset, MemberInfo.m_iLength);
						char * szValue = strupr((char*)strValue.c_str());
						char * szMask  = strupr((char*)it->m_StringMask.c_str());
						bRuleWorks = MatchWithPattern(szMask, szValue) == TRUE;
						if(bRuleWorks)
							break;
					}

					break;
				}

				case MEMBER_TYPE_TIME:
				{
					time_t tmValue = *(double*)((char*)&Trade + MemberInfo.m_iOffset);
					for(vector<limits_t>::const_iterator it = Filter.begin(); it != Filter.end(); it++)
					{
						bRuleWorks = it->m_NumMask.m_HiLimit.m_bEnable ?
							((ULONG)tmValue >= (ULONG)it->m_NumMask.m_LowLimit.m_Value.m_TimeValue && 
							(ULONG)tmValue <= (ULONG)it->m_NumMask.m_HiLimit.m_Value.m_TimeValue) :
							((ULONG)tmValue >= (ULONG)it->m_NumMask.m_LowLimit.m_Value.m_TimeValue);
						if(bRuleWorks)
							break;
					}

					break;
				}
			}

			if(!bRuleWorks)
				break;
		}

		if(bRuleWorks)
		{
			RortfolioName = RuleName;
			break;
		}
	}

	return 0;
};

/****************************************************************************
*
*	FUNCTION: MatchOkay
*
*	PURPOSE: Only thing left after compare is more mask. This routine makes
*	sure that its a valid wild card ending so that its really a match.
*
****************************************************************************/
BOOL MatchOkay( PCHAR Pattern )
{
    // If pattern isn't empty, it must be a wildcard
    if( *Pattern && *Pattern != '*' ) {
 
       return FALSE;
    }

    // Matched
    return TRUE;
}

/****************************************************************************
*
*	FUNCTION: MatchWithPattern
*
*	PURPOSE: Performs nifty wildcard comparison.
*
****************************************************************************/
BOOL MatchWithPattern( PCHAR Pattern, PCHAR Name )
{
	char matchchar;

    // End of pattern?
    if( !*Pattern ) {
        return FALSE;
    }

    // If we hit a wild card, do recursion
    if( *Pattern == '*' ) {

        Pattern++;
        while( *Name && *Pattern ) {

			matchchar = *Name;
			if( matchchar >= 'a' && 
				matchchar <= 'z' ) {

				matchchar -= 'a' - 'A';
			}

            // See if this substring matches
		    if( *Pattern == matchchar ) {

  		        if( MatchWithPattern( Pattern, Name )) {

                    return TRUE;
                }
            }

            // Try the next substring
            Name++;
        }

        // See if match condition was met
        return MatchOkay( Pattern );
    } 

    // Do straight compare until we hit a wild card
    while( *Name && *Pattern != '*' ) {

		matchchar = *Name;
		if( matchchar >= 'a' && 
			matchchar <= 'z' ) {

			matchchar -= 'a' - 'A';
		}

        if( *Pattern == matchchar ) {
            Pattern++;
            Name++;

        } else {

            return FALSE;
		}
    }

    // If not done, recurse
    if( *Name ) {

        return MatchWithPattern( Pattern, Name );
    }

    // Make sure its a match
    return MatchOkay( Pattern );
}

string foc_rttf_trade_ex::MakeOptionSymbol()
{
	string sRet;

	if(security_type[0] == '0')	// i.e., option
	{
		LPCSTR acMonthName[12] = {	"JAN", "FEB", "MAR", "APR", 
								"MAY", "JUN", "JUL", "AUG", 
								"SEP", "OCT", "NOV", "DEC" };

		char szSym[21] = {0};
		char szTemp[2] = { put_call, 0 };	// preserve original side

		_snprintf(	szSym, sizeof(szSym) - 1, "%s%s%d%s",
					trading_sym,
					acMonthName[expiration_date.month >= 0 && expiration_date.month < 12 ? expiration_date.month : 0],
					static_cast<long>(strike),
					strupr(szTemp));
		
		if(0 != strlen(szSym))
			sRet = szSym;
	}
	else
	{
		sRet = base_sym;
	}

	return sRet;
}