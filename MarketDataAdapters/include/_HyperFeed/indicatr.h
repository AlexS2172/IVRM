/* $Header:   G:/LOG/INCLUDE/indicatr.h_v   1.11   Jul 09 1997 08:18:32   unknown  $ */
/*
 *      indicatr.h      Date = 931103
 *
 *    trade and quote indicators.
 *
 * $Log::   G:/LOG/INCLUDE/indicatr.h_v                                      $
 * 
 *    Rev 1.11   Jul 09 1997 08:18:32   unknown
 * added back NON_COMPLIENCE for existing systems
 * 
 *    Rev 1.10   Jul 09 1997 08:16:42   unknown
 * changed spelling of NON_COMPLIENCE to NON_COMPLIANCE
 * 
 *    Rev 1.9   10 Oct 1996 15:54:38   JCT
 * add QI define
 * 
 *    Rev 1.8   08 May 1996 14:06:50   RPW
 * Added Quote Indicators for the New CQS feed. Added 45 thru 50.
 * 
 *    Rev 1.7   15 May 1995 17:27:36   BPM
 * Added TI_CH_FWD_TRADE(142) for Bridge.  Updated TI_MAX also.
 * 
 *    Rev 1.6   10 Jan 1995 14:34:14   BPM
 * Added 4 new trade conditions(138 - 141) for Bridge
 * 
 *    Rev 1.5   20 Dec 1994 12:36:46   BPM
 * Added two new TI codes for Bridge.
 * Z
 * 
 *    Rev 1.4   18 Aug 1994 11:27:50   BAC
 * No changes
 * 
 *    Rev 1.3   18 Aug 1994 11:26:02   BAC
 * No changes
 * 
 *    Rev 1.2   16 Aug 1994 15:24:26   BAC
 * Eliminated QI_TEMPORARY since it not used.
 * 
 *    Rev 1.1   19 Jul 1994 12:11:18   BPM
 * Added 73 - 76 to support new CIT conditions
 *
 *
 *    11/11/93 - jpy - modified to make consistent with condtion.h
 *                     for 32kb feed production.  Condtion.h now
 *                     obsolete.
 *
 *
 */

/*=====================================================================*/

#ifndef INCL_INDICATORS
#define INCL_INDICATORS

/* Quote Indicators */

#define QI_REGULAR						0
#define QI_OFFER_DEPTH					1
#define QI_BID_DEPTH					2
#define QI_BID_OFFER_DEPTH				3
#define QI_OPENING						4
#define QI_CLOSING						5
#define QI_NEWS_DISS					6
#define QI_FAST_MARKET					7
#define QI_ORDER_INFLUX					8
#define QI_MM_CLOSED					9
#define QI_NON_FIRM						10
#define QI_NEWS_PENDING					11
#define QI_DUE_TO_RELATED				12
#define QI_VIEW_OF_COMMON				13
#define QI_ORDER_IMBALANCE				14
#define QI_NO_OPEN_RESUME				15
#define QI_LOCKED						16
#define QI_CROSSED						17
#define QI_SYNDICATE_BID				18
#define QI_PRE_SYND_BID					19
#define QI_PENALTY_BID					20
#define QI_CABINET_BID					21
#define QI_CANCEL_BID_ASK				22
#define QI_CANCEL_BID					23
#define QI_CANCEL_ASK					24
#define QI_BID_ONLY						25
#define QI_ASK_ONLY						26
#define QI_ROTATION						27
#define QI_HALT							28
#define QI_AUTO_EXECUTE					29
#define QI_INACTIVE						30

#define QI_INFO_PENDING					31
										
#define QI_IND_IMBALANCE				32
#define QI_UNABLE_TO_QUOTE				33
#define QI_END_UNABLE_TO_QUOTE			34
#define QI_RELEASE						35
#define QI_SUSPEND						36
#define QI_OUT_OF_SEQ					37
#define QI_RESTRICTED					38
#define QI_IND_PRICE_LIMITS				39
#define QI_KASSAKURS					40    // 09/28/92
#define QI_ISSUE						41    // 09/28/92
#define QI_REDEMPTION					42    // 09/28/92
#define QI_TRADING_RANGE_CANCEL			43
										
#define QI_PREOPENING_BID				44
#define QI_TRADING_RANGE				45
#define QI_DUE_TO_RELATED_NEWS_DISS		46
#define QI_DUE_TO_RELATED_NEWS_PENDING	47
#define QI_ADD_INFO						48
#define QI_ADD_INFO_DUE_TO_RELATED		49
#define QI_EQUIPMENT_CHANGEOVER			50
										
#define QI_DELETED						51
#define QI_NOT_CURRENT					52   // halt
#define QI_QUOTATION_CANCELED			53	// halt
#define QI_NON_COMPLIANCE				54	// halt
#define QI_NON_COMPLIENCE				54	// mispelled for existing code
#define QI_NO_ACTIVE_MM					55
#define QI_SEC_SUSPENSION				56	// halt
#define QI_NEWS_DISS_RESUMPTION_TIME	57
#define QI_OTHER						58	// halt

// QI_*_ETH are added to support Extended Trading Hours on OPRA, BJB 05/03/2001
#define QI_REGULAR_ETH					59
#define QI_FAST_MARKET_ETH				60
#define QI_ROTATION_ETH					61
#define QI_HALT_ETH						62
#define QI_AUTO_EXECUTE_ETH				63
#define QI_INACTIVE_ETH					64

#define QI_MAX							64    // Last legal QI_*


/* Trade Indicators */

#define TI_REGULAR_SALE					0
#define TI_ACQUISITION					1
#define TI_DISTRIBUTION					2
#define TI_SPLIT						3
#define TI_RESERVED_SALE				4
#define TI_CASH_SALE					5
#define TI_NEXT_DAY_SALE				6
#define TI_SELLER_SALE					7
#define TI_RULE_155						8
#define TI_SOLD_LAST_SALE				9
#define TI_OPENED_SALE					10	// Late and out of sequence report of the open.
#define TI_OUT_OF_SEQ					11
#define TI_CANCEL_VOL					12
#define TI_CANCEL_LAST					13
#define TI_CANCEL_OPEN					14
#define TI_CANCEL_ONLY					15
#define TI_CANCEL_STOPPED				16
#define TI_REOPEN						17
#define TI_ADJUSTED						18
#define TI_SPREAD						19
#define TI_STRADDLE						20
#define TI_STOPPED						21
#define TI_BUNCHED						22
#define TI_BUNCHED_SOLD					23
#define TI_VOLUME_ONLY					24
#define TI_FUTURE_TRADE					25
#define TI_PREMIUM						26
#define TI_MID_PRICE					27
#define TI_SETTLEMENT					28
#define TI_FORM_T						29
#define TI_LATE_OPEN					30
#define TI_AUTO_EXECUTE					31
#define TI_INTRADAY_DETAIL				32
#define TI_OPENING_DETAIL				33
#define TI_RULE_127						34
										
#define TI_OVERNIGHT_TRADE				35
#define TI_DELAYED_TRADE				36
#define TI_AGENCY_CROSS					37
#define TI_AGENCY_CROSS_FORWARD			38
#define TI_FAST_MARKET					39
#define TI_OPEN_INTEREST				40
										
#define TI_OPEN_PRICE					41
#define TI_HIGH_PRICE					42
#define TI_LOW_PRICE					43
										
#define TI_SPOT_PRICE					44
										
#define TI_VALUATION_PRICE				45
#define TI_LAST_SALE_TELEKURS			46
#define TI_X_DIVIDEND					47
#define TI_OPTION_INIT					48
										
#define TI_CLOSE_PRICE					49
										
#define TI_SUMMARY_PRICE				50
#define TI_YESTERDAY_PRICE				51

#define TI_COMBO						52 // CMBO spread trade, OPRA, BJB, 11/28/2001
										
#define TI_AVERAGE_PRICE				53
#define TI_BURST_BASKET					54
#define TI_BASKET_INDEX_CLOSE			55
#define TI_ODDLOT 						56 	// non-board lot trade in Canada
										
#define TI_PAID_BID_TK					60
#define TI_PAID_ASK_TK					61
#define TI_DRAWING_PRICE_TK				62
#define TI_TRADING_HALT_TK				63
#define TI_DRAWING_PRICE_FORWARD_TK		64
										
#define TI_DELETE_PREV_INDEX			65
#define TI_FORCED_PRICE					66
#define TI_BARGAIN_CONDITION			67
#define TI_OVERSEAS_CROSS				68
#define TI_CROSSED						69
										
#define TI_FRENCH_COMM_REFERENCE		70
#define TI_BLOCKED						71
#define TI_JAKARTA_CROSSED				72

/*
** The following 4 trade conditions(73 through 76) were added
** 7/19/94 by BPM to support changes to the CIT feed.
*/

#define TI_BR_ODD_LOT					73 	// Should set the bits: 	 V
#define TI_COMPENSATION 				74 	// Should set the bits: L O V
#define TI_REPORTED_OUTSIDE				75 	// Should set the bits: 	 V
#define TI_EUROPE_ODD_LOT				76 	// Should set the bits: 	 V

#define	TI_PRIOR_REFERENCE_PRICE		77	 /* NTDS "P"(Prior Reference Price) sale condition */ // 4/19/99, MFT

#define	TI_AUTOMATIC_EXECUTION			78	 /* NYSE Direct+ Automatic Execution System sale condition */ // 7/7/00, MFT

#define TI_PREZZO_UFFICIALE				80
#define TI_BUYER_PRICE_JOHANNESBURG		81
#define TI_SELLER_PRICE_JOHANNESBURG	82
#define TI_BASE_PRICE_JAPAN				84
#define TI_ACCEPT_COPENHAGEN			85
#define TI_MATCH_COPENHAGEN				86
#define TI_PREVIOUS_DAY					87
#define TI_CANCEL_VOLUME				88
#define TI_INDICATIVE_TRADE				89
#define TI_SPECIAL_TRADE_ASX			90
#define TI_CUM_DIV_TRADE_ASX			91
#define TI_EX_DIV_TRADE_ASX				92
#define TI_LATE_TRADE_ASX				93

/*
** The following were added by BPM on 12/7/94 to support enhancements to the
** CIT feed.
*/
#define TI_LSE_BLK_TRD_PREV_DAY			94    /* Bridge code "BD" */
#define TI_LSE_BLK_TRD_SAME_DAY			95    /* Bridge code "BL" */

#define TI_CUM_VOLUME					97
                                                                                                                //
#define TI_GMT_OFFSET					100

/* 
** The following trade indicators were defined by BPM on 12/8/93 to expand
** support of international trade indicators coming from Bridge.
*/

#define TI_BROKER_TO_BROKER				105
#define TI_DE_PART_PD_BID_QUOTE			106
#define TI_DE_PART_PD_ASK_QUOTE			107
#define TI_UK_NON_PROTECTED				108
#define TI_DE_OFFICIAL_PRICE			109
#define TI_NE_PAID_BID					110
#define TI_NE_PAID_ASK					111
#define TI_UK_PROTECT_PORT_TRADE		112
#define TI_DE_REPESENT_PRICE			113
#define TI_DE_RATIONED_BID				114
#define TI_DE_RATIONED_ASK				115
#define TI_DE_CANCELED_BID				116
#define TI_DE_CANCELED_ASK				117
#define TI_IT_PREZZO_DI_LISTINO			118
#define TI_DE_EST_PRICE					119
#define TI_DE_EST_BID_PRICE				120
#define TI_DE_EST_ASK_PRICE				121
#define TI_DE_DRAWING_ASK				122
#define TI_DE_KASSA_PAID_BID			123
#define TI_DE_KASSA_PAID_ASK			124
#define TI_DE_KASSA_REP_PRICE			125
#define TI_DE_KASSA_DRAWING				126
#define TI_DE_DRAWING_BID_PRICE			127
#define TI_DE_ASKING_QUOTE				128
#define TI_UK_INDICATIVE_PRICE			129
#define TI_DE_INDICATIVE_PRICE			130
#define TI_DE_BID_QUOTATION				131
#define TI_DE_KASSA_ASK_QUOTE			132
#define TI_DE_KASSA_BID_QUOTE			133
#define TI_DE_KASSA_CANCELED			134
#define TI_UK_MEAN_QUOTE				135
#define TI_JA_PRICE_RANGE_LIMIT			136
#define TI_UK_VOLATILITY_TRADE			137
#define TI_IT_REFERENCE_PRICE			138     /* Should set the bits: V   (PR) */
#define TI_IT_AUCTION_PRICE				139     /*                          (AP) */
#define TI_IT_LAST_VALID_TRADE			140     /*                          (pv) */
#define TI_IT_SUBSCRIPTION_PRICE		141     /*                          (ps) */
#define TI_CH_FWD_TRADE					142     /*                      V   (FW) */

// TI_*_ETH are added to support Extended Trading Hours on OPRA, BJB 05/03/2001
#define TI_REGULAR_SALE_ETH				143
#define TI_SOLD_LAST_SALE_ETH			144
#define TI_OPENED_SALE_ETH				145
#define TI_OUT_OF_SEQUENCE_ETH			146
#define TI_CANCEL_VOL_ETH				147
#define TI_CANCEL_LAST_ETH				148
#define TI_CANCEL_OPEN_ETH				149
#define TI_CANCEL_ONLY_ETH				150
#define TI_CANCEL_STOPPED_ETH			151
#define TI_REOPEN_ETH					152
#define TI_ADJUSTED_ETH					153
#define TI_SPREAD_ETH					154
#define TI_STRADDLE_ETH					155
#define TI_STOPPED_ETH					156
#define TI_LATE_OPEN_ETH				157
#define TI_AUTO_EXECUTE_ETH				158
#define TI_COMBO_ETH					159


#define TI_MAX							159 // Last legal TI_*


/* Order Indicators */
#define ORDER_INDICATOR_HALTED					1
#define ORDER_INDICATOR_QUOTE_ONLY				2
#define ORDER_INDICATOR_SYNDICATE				3
#define ORDER_INDICATOR_PRE_SYNDICATE			4
#define ORDER_INDICATOR_PENALITY				5
#define ORDER_INDICATOR_PASSIVE					6

#define ORDER_INDICATOR_MAX						6

#endif

/*=====================================================================*/
