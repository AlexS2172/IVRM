#ifndef ISE_MESSAGE_H
#define ISE_MESSAGE_H

#pragma pack(push)
#pragma pack(1)

/**************************************************************************************
	COMMON MESSAGES
**************************************************************************************/

struct CISEMessage
{
	uint32		m_uiLen;
};

struct CISELogonMessage : public CISEMessage, public omni_login_message
{
	CISELogonMessage()
	{
		memset(this, 0, sizeof(CISELogonMessage));
		m_uiLen = sizeof(omni_login_message);
	}
};

struct CISELogoffMessage : public CISEMessage, public transaction_type_t
{
	CISELogoffMessage()
	{
		memset(this, 0, sizeof(CISELogoffMessage));
		m_uiLen = sizeof(transaction_type_t);
	}
};


/**************************************************************************************
	QUERIES
**************************************************************************************/

template	<class _Query, char _CM, char _ST, uint16 _TN> 
class TISEQuery : public CISEMessage, public _Query
{
public:
	TISEQuery<_Query, _CM, _ST, _TN>()
	{
		memset(this, 0, sizeof(TISEQuery));
		m_uiLen = sizeof(_Query);
		transaction_type.central_module_c = _CM;
		transaction_type.server_type_c = _ST;
		transaction_type.transaction_number_n = _TN;
	}
};

typedef TISEQuery<query_user_type_info_t,	'D', 'Q',  30>			CISEQueryUserInfo;
typedef TISEQuery<query_instrument_group_t, 'D', 'Q', 208>			CISEQueryInstrumentGroup;
typedef TISEQuery<query_instrument_t,		'D', 'Q', 203>			CISEQueryInstrument;
typedef TISEQuery<query_market,				'D', 'Q', 207>			CISEQueryMarket;
typedef TISEQuery<query_exchange_dq24_t,	'D', 'Q',  24>			CISEQueryExchange;
typedef TISEQuery<query_trading_state_dq245_t,'D', 'Q',245>			CISEQueryTradingState;
typedef TISEQuery<query_underlying,			'D', 'Q', 204>			CISEQueryUnderlying;
typedef TISEQuery<query_instrument_class,	'D', 'Q', 210>			CISEQueryInstrumentClass;
typedef TISEQuery<query_series,				'D', 'Q', 202>			CISEQuerySeries;
typedef TISEQuery<query_broker,				'D', 'Q', 206>			CISEQueryBrokers;
typedef TISEQuery<query_bin_dq211,			'D', 'Q', 211>			CISEQueryMmBin;
typedef TISEQuery<query_underlying_status_us, 'I', 'Q', 132>		CISEQueryUnderlyingTradingStatus;
typedef TISEQuery<query_instrument_status, 'U', 'Q', 15>			CISEQueryInstrumentTradingStatus;
typedef TISEQuery<query_bi8_broadcasts_sent_t, 'U', 'Q', 11>		CISEQueryGeneralMessage;
typedef TISEQuery<query_quote_mq104,		'M', 'Q', 104>			CISEQueryQuotes;

typedef TISEQuery<query_prices_iq101,		'I', 'Q', 101>			CISEQuerySeriesPrices;
typedef TISEQuery<query_underlying_prices_iq121, 'I', 'Q', 121>		CISEQueryUnderlyingPrices;
typedef TISEQuery<application_status_t,		'U', 'I', 1>			CISEReadyToTrade;
typedef TISEQuery<set_pmm_parameters_ui102,	'U', 'I', 102>			CISESetPMMParameters;
typedef TISEQuery<set_mm_parameters_ui101,	'U', 'I', 101>			CISESetCMMParameters;
typedef TISEQuery<quote_trans_mo106_t,		'M', 'O', 106>			CISESubmitQuote;
typedef TISEQuery<delete_trans_mo104_mo105_t,'M', 'O', 105>			CISEDeleteQuote;
typedef TISEQuery<order_trans_mo101,         'M', 'O', 101>			CISEEnterOrder;


/**************************************************************************************
	ANSWERS
**************************************************************************************/

union UISEAnswerData
{
	// the static market data
	answer_series_da202_t				m_Series;				// ISE flavour, dq202
	answer_instrument_da203_t			m_Instrument;			// ISE, dq203
	answer_underlying_da204_t			m_Underlying;			// ISE, dq204
	answer_broker_da206_t				m_Broker;				// ISE, dq206
	answer_market_da207_t				m_Market;				// ISE, dq207
	answer_instrument_group_da208_t     m_Instrument_group;	    // ISE, dq208
	answer_instrument_class_da210_t     m_Instrument_class;	    // ISE, dq210
	answer_bin_da211_t					m_Bin;					// ISE, dq211
	answer_dependencies_da212_t			m_Synthetic;		    // ISE, dq212
	answer_exchange_da24_t				m_Exchange;				// ISE, dq24
	answer_trading_state_da245_t	    m_Trading_states;	    // 16.3+, dq245
	answer_user_type_info_t				m_UserType;				// 16.5-1+, dq30

	// the market snapshots / queries
	answer_missing_trade_t				m_MissingTrade;			// cq1/ca1
	answer_trade_t						m_PastTrade;			// cq2/ca2
	answer_order_ma101_t				m_Order;				// ISE, mq101
	answer_inactive_ma102_t 			m_Inactive;				// ISE, mq102
	answer_ob_ma103_t					m_Ob;					// ISE, mq103, PMM only
	answer_quote_ma104_t				m_Quote;				// ISE, mq104
	answer_locked_order_ma106_t			m_LockedOrder;			// ISE, mq106, PMM only
	answer_rotation_ma114_t 			m_Rotation;				// ISE, mq114, PMM only
	answer_combo_order_ma131_t			m_ComboActive;			// 16.5+, mq131, member's active combos
	answer_inactive_combo_ma132_t	    m_ComboInactive;	    // 16.5+, mq132, member's inactive combos
	answer_combo_ob_ma133_t				m_Combobook;		    // 16.5+, mq133, public combobook
	answer_prices_ia101_t				m_Prices;				// ISE, iq101
	answer_opra_prices_ia111_t			m_OpraPrices;			// ISE, iq111
	answer_each_opra_prices_ia112_t     m_EachOpraPrices;	    // ISE, iq112
	answer_underlying_prices_ia121_t    m_UnderlyingPrices;     // ISE, iq121
	answer_underlying_status_us_t	    m_UnderlyingStatus;     // ISE, iq132
	answer_bi8_broadcasts_sent_t	    m_PastGeneralText;	    // uq11
	answer_business_date_t				m_BusinessDate;			// uq12
	answer_instrument_status_t			m_InstrumentStatus;	    // ISE, uq15
	answer_mm_parameters_ua101_t	    m_MmParameters;			// ISE, uq101
	answer_pmm_parameters_ua102_t	    m_PmmParameters;	    // ISE, uq102, PMM only
	answer_mm_exposure_limit_ua103_t    m_MmExposureLimit;	    // ISE, uq103, CMM/PMM, 16.5-1+

	// directives / commands / statii
	transaction_type_t					m_TranType;
};

#define MAX_RESPONSE_SIZE ((int32) 64000)	/* response buffer size */

struct CISEAnswer : public CISEMessage
{
	UISEAnswerData	m_Data;

	CISEAnswer()
	{
		memset(this, 0, sizeof(CISEAnswer));
		m_uiLen = MAX_RESPONSE_SIZE;
	}
};



#pragma pack(pop)

#endif