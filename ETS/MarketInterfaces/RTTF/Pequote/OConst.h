#ifndef __OCONST_H__
#define __OCONST_H__

//xml
#define SETTINGS_XML_KEY				_T("ETS\\PequoteConnector")

// Registry
//#define SETTINGS_REGISTRY_KEY			_T("Software\\Egar\\ETS\\Pequote Connector")
//#define SETTINGS_DB_REGISTRY_KEY		_T("Software\\Egar\\ETS\\Database")
#define SETTINGS_CONNSTR_KEY			_T("")
#define SETTINGS_SERVER_ADDRESS         _T("Server Address")
#define SETTINGS_USER                   _T("User")
#define SETTINGS_PASSIVEFTP             _T("FTPMode")
#define SETTINGS_PASSWORD               _T("Password")
#define SETTINGS_TRADES_FILENAME        _T("Trades File")
#define SETTINGS_POSITIONS_FILENAME     _T("Positions File")
#define SETTINGS_READ_DATA_PORTION      _T("Read Data Portion")
#define SETTINGS_READ_TRADES_PERIOD     _T("Read Trades Period")
#define SETTINGS_POSITIONS_DOWNLOAD_TIME    _T("Positions Download Time")
#define SETTINGS_TRADES_LAST_LINE		_T("Last trade line")
#define SETTINGS_TRADES_LAST_DATE		_T("Last trade date")
#define SETTINGS_TRADES_LAST_TIME		_T("Last trade time")
#define SETTINGS_READ_ENCRYPTED		    _T("Encrypted")
#define SETTINGS_READ_PASSPHRASE	    _T("PassPhrase")

#define SETTINGS_PQ_CONNECT				_T("PeQuote Connect")

#define SETTINGS_WINDOW_PLACEMENT_KEY	SETTINGS_XML_KEY _T("\\wp")

// Times
#define	MAX_DBRECONNECTION_TRIES		10
#define	DBRECONNECTION_DELAY			10
// Files
#define TRADES_FILENAME					_T("Logs/PequoteTrades.log")
#define TRADES_FILENAME_BEGIN			_T("Logs/OTrades")
#define TRADES_FILENAME_END				_T(".log")
#define FAILES_FILENAME_BEGIN			_T("Logs/OFails")
#define FAILES_FILENAME_END				_T(".log")
#define STRUCTURE_FILENAME				_T("Logs/PequoteStructure.log")
// security_type
#define OPTION_FLAG                 'O'
#define STOCK_FLAG                  'S'
#define FUTURE_FLAG                 'F'
#define NON_EQUITY_FLAG             'X'
// put_call
#define NON_OPTION_FLAG             ' '
#define PUT_FLAG                    'P'
#define CALL_FLAG                   'C'
// ins_upd_del_flag
#define NEW_FLAG                    ' '
#define TRADE_FLAG                  'I'
#define ETRADE_FLAG                 'E'
#define UPD_FLAG                    'C'
#define DEL_FLAG                    'D'
// buy_sell
#define BUY_FLAG                    'L'
#define SELL_FLAG                   'S'
#include <boost/shared_ptr.hpp>
//////////////////////////////////////////////////////////////////////////////
inline bool IsValidInsUpdDelFlag(char flag)
{
    switch (flag)
    {
        case NEW_FLAG:
        case TRADE_FLAG:
        case ETRADE_FLAG:
        case UPD_FLAG:
        case DEL_FLAG:
            return true;
    }
    return false;
}

inline bool IsValidPutCall(char flag)
{
    switch (flag)
    {
        case NON_OPTION_FLAG:
        case PUT_FLAG:
        case CALL_FLAG:
            return true;
    }
    return false;
}

inline bool IsValidBuySell(char flag)
{
    switch (flag)
    {
        case BUY_FLAG:
        case SELL_FLAG:
            return true;
    }
    return false;
}

inline bool IsValidSecurityType(char flag)
{
    switch (flag)
    {
        case OPTION_FLAG:
        case STOCK_FLAG:
        case FUTURE_FLAG:
        case NON_EQUITY_FLAG:
            return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////
// struct CTrade

struct CTrade
{
	char    ins_upd_del_flag;
	char    put_call;
	char    buy_sell;
	char    origin;
	char    open_close;
	string  trade_id;
	string  user_id;
	string  trading_sym;
	string  base_sym;	
	string  parent_account;
	string  account;
	string  q_account;
	string  security_type;
	string  security_subtype;
	string  clearing_firm;
	string  cusip;
	string  basket_id;
	string  giveup_firm;	
	string  contra_firm;
	string  exec_broker;
	string  contra_broker;
	string  mmcs_sub_acct;
	double  exercise_multiplier;
	double  extended_premium;
	double  premium;
	double  strike;
	double  price;
	int     quantity;
	int     tag;
	DATE    expiration_date;
	DATE    execution_date;
	DATE    last_upd_date;
	DATE    contract_date;
	int     source_system;
	int     trade_id_num;
	int     trade_group_id;
	DATE    settlement_date;
	string  entry_firm;
	char    matched_flag;
	char    spread_ind;
	double  premium_multiplier;
	string  source_code;
	string  parent_acct_intrl_id;
	string  acct_intrl_id;
	string  q_acct_intrl_id;
	double  cap_interval;
	int     trade_exchange;
	char    settled_unsettled_ind;
	char    stock_origin;
	string  mm_card_number;
	int     basket_tag;
	double  basket_price;
	char    short_sale;
	string  commission_code;
	double  commission;
	char    same_day_exer;
	string  terminal_name;	
	string  status;	

    CTrade();
	inline string MakeTradingSymbol();
};
typedef boost::shared_ptr<CTrade> CTradePtr;

inline CTrade::CTrade() :
	ins_upd_del_flag(0),
	put_call(0),
	buy_sell(0),
	origin(0),
	open_close(0),
	exercise_multiplier(0),
	extended_premium(0),
	premium(0),
	strike(0),
	price(0),
	quantity(0),
	tag(0),
	expiration_date(0),
	execution_date(0),
	last_upd_date(0),
	contract_date(0),
	source_system(0),
	trade_id_num(0),
	trade_group_id(0),
	settlement_date(0),
	matched_flag(0),
	spread_ind(0),
	premium_multiplier(0),
	cap_interval(0),
	trade_exchange(0),
	settled_unsettled_ind(0),
	stock_origin(0),
	basket_tag(0),
	basket_price(0),
	short_sale(0),
	commission(0),
	same_day_exer(0)
{
}

inline string CTrade::MakeTradingSymbol()
{
	string sRet;
    const char  option_flag[] = {OPTION_FLAG, 0};

    if (security_type.compare(option_flag) == 0)
	{	
		LPCSTR acMonthName[] = {"JAN", "FEB", "MAR", "APR", 
								"MAY", "JUN", "JUL", "AUG", 
								"SEP", "OCT", "NOV", "DEC"};

		char    szSym[100] = {0};
        LPCSTR  lpszMonthName = NULL;

        SYSTEMTIME  system_time = {0};
        if (expiration_date 
			&& SUCCEEDED(VariantTimeToSystemTime(expiration_date, &system_time)) )
            lpszMonthName = acMonthName[system_time.wMonth - 1];

		_snprintf_s(szSym, sizeof(szSym), sizeof(szSym), "%s%s%d%c",
					trading_sym.c_str(),
                    lpszMonthName ? lpszMonthName : "XXX",
					(long)strike,		
					put_call);

		if(strlen(szSym))
		{
			sRet = szSym;
		}
	}
	else
	{
		sRet = trading_sym;
	}

	return sRet;
}

//////////////////////////////////////////////////////////////////////////////
// struct INPUTLINE
struct INPUTLINE
{
	char	trans_id[3];	// '346'
	char	record_id;		// ‘  ’ (blank) indicates open position, ‘E’ electronic trade, ‘I’ trade, ‘D’ delete, ‘C’ correction
	char	clearing_firm[4];	// clearing firm Right aligned, padded with leading
	char	account[10];	// Left aligned, padded with blanks
	char	put_call;	// ‘P’=put, ‘C’=call, '  '=non-option
	char	symbol[6];	//Base symbol, NOT option symbol
	char	expire_year[2];	//Year in which option of future expires. Zero fill if not option or future.
	char	expire_month[2];//Month in which option or future expires. Zero fill if not option or future 
	char	strike_dollar[5];	// Equity options use columns 32-34; other options use columns 31-34. Zero fill if not an option. If blank, go to 89-97. 
	char	strike_frac[2];	//Only equity options use this field; others zero fill in. The fraction is expressed in eighths, i.e. 5/8=05 
	char	long_short;	//‘L’=long/buy, ‘S’=short/sell 
	char	security_type;	//‘O’=option, ‘S’=stock, ‘F’=future, ‘X’=non-equity underlying, i.e. currency forwards. 
	char	trade_price[10];//If averaged or open trade, closing price or mark. 6 implied decimal points. 
	char	filler[10];	//(unused) 
	char	net_position[7];//Quantity, absolute value 
	char	CUSIP[9];	//(unused) 
	char	filler2[5]; //(unused) 
	char	record_type;// ‘ ‘ or ‘0’=opening position, ‘1’=trade 
	char	trade_symbol[6];// ALTSYM; the actual exchange symbol, differs from base symbol for some options. 
	char	expiration_day[2];	//Day of the month in which the option or future expires. Zero fill if not an option or future. 
	char	decimal_strike[9];	//4 assumed decimal places. PIC 9(5)V9(4) 
	char	SPC_Shares[5];	//per contract, right aligned. 
	char	execution_time[4];	//HHMM 
	char	execution_date[6];	//YYMMDD 
	char	tag_number[6];	//Unique ID or zero. 
	char	filler3[4];	//(unused) 
	char	option_type;// ‘1’=future option, ‘ ‘=equity or index option 
	char	filler4[6];	//(unused) 
	char	filler5[4];	//(unused) 
	char	contract_year[4];	//Contract year 
	char	contract_month[2];	//Numeric 
};

#endif // __OGCONST_H__