#ifndef ISECONST_H
#define ISECONST_H

#define GENERAL_BROADCAST_EVENT_TYPE		1
#define TIME_DELAY_BROADCASTS				50	//msec
#define READEVENT_OPTMASK_MANY				1
#define RECONNECTION_TIME					60  // sec
#define WAIT_BEFORE_CONNECT_TIME			1

#define ISE_EXCHANGE_CODE					_T("K")
#define ISE_EXCHANGE_CODE2					_T("k")

// in BI202 series update, BI204 underlying update, BI210 instrument class update
#define CHANGE_TYPE_ADD			    1
#define CHANGE_TYPE_DELETE		    2
#define CHANGE_TYPE_MODIFY		    3

#define FACILITY_EP0		0
#define FACILITY_EP1		1
#define FACILITY_EP7		7

// ext_or_int_c
#define USERTYPE_INTERNAL	2
#define USERTYPE_EXTERNAL	1

//is_trader_c
#define USERTYPE_TRADER		1
#define USERTYPE_NONTRADER	2

// trans or bdx
#define	TRANTYPE_TRANSACTION	1
#define	TRANTYPE_BROADCAST		2

//instrument group
#define IG_EUROPEAN_CALL_OPTION	1
#define IG_EUROPEAN_PUT_OPTION	2
#define IG_FORWARD				3
#define IG_FUTURE				4
#define IG_AVISTA				5
#define IG_AMERICAN_CALL_OPTION	6
#define IG_AMERICAN_PUT_OPTION	7
#define IG_RESERVED				0

//market code
#define MC_RESERVED				0
#define MC_INDEX				1
#define MC_STOCK				2
#define MC_BOND					3
#define MC_CURRENCY				4
#define MC_LDC_INDEX			5
#define MC_TM_INDEX				6
#define MC_TM_STOCK				7
#define MC_EUROTOP_100_INDEX	8
#define MC_TM_BOND				9
#define MC_TM_FINNISH_BOND		10
#define MC_TM_FX				11
#define MC_TM_POWER_DERIVATES	12
#define MC_POWER				13
#define MC_BOND_INDEX			14

//type of matching
#define MATCH_NORMAL			1
#define MATCH_NO				3

//bool for ISE
#define ISEBOOL_YES				1
#define ISEBOOL_NO				2

// underlying type
#define UT_STOCK				1
#define UT_CURRENCY				2
#define UT_INTEREST_RATE		3
#define UT_ENERGY				4
#define UT_SOFT_AND_AGRICS		5
#define UT_METAL				6
#define UT_STOCK_INDEX			7
#define UT_CURRENCY_INDEX		8
#define UT_INTEREST_RATE_INDEX	9
#define UT_ENERGY_INDEX			10
#define UT_SOFT_AND_AGRICS_INDEX 11
#define UT_METAL_INDEX			12

// underlying price unit
#define UPU_PRICE				1
#define UPU_YIELD				2
#define UPU_POINTS				3
#define UPU_IMM_INDEX			5
#define UPU_BASIS_POINTS		6
#define UPU_INVERTED_YIELD		7

//Identifies what is ranked as being:
#define RANK_NORMAL				1
#define RANK_INVERSED			2

//Market maker type
#define MM_PRIMARY				1
#define MM_COMPETITIVE			2

// series ts
#define STS_UNKNOWN				100

// Underlying trading status
#define UTS_UNKNOWN				100
#define UTS_CLOSED				0
#define UTS_OPEN				1
#define UTS_SUSPENDED			2
#define UTS_RESUMED				3
#define UTS_FASTMARKET			4

// Bid or ask to submit quote for
#define BA_BID					1
#define BA_ASK					2

#define ORDER_TYPE_LIMIT		1
#define VALIDITY_UNIT_IOC		0x10

// user category
#define CUST_CATEGORY_MM		    0	    // erratically documented by OM - appears to be for MM orders
#define CUST_CATEGORY_CUSTOMER		1	    // customer
#define CUST_CATEGORY_FIRM		    2	    // Firm/Proprietary
#define CUST_CATEGORY_FARMM		    4	    // FARMM - was 3 before 16.3
#define CUST_CATEGORY_DISCRETIONARY	5	    // customer discretionary - was 4 before 16.3
#define CUST_CATEGORY_BROKERDEALER	6	    // Broker/Dealer - formerly handled as B/D in BrokerID field

// order open close flag
#define OPEN_CLOSE_REQUEST_DEFAULT	    0
#define OPEN_CLOSE_REQUEST_OPEN 	    1
#define OPEN_CLOSE_REQUEST_CLOSE_NET	2
#define OPEN_CLOSE_REQUEST_CLOSE_MAND	3
#define OPEN_CLOSE_MAX					3

// CMTA
#define MAX_CMTA_MEMBER	    5	// max length of CMTA member ID string
#define MAX_SUB_ACCOUNT_ID  5

// Trade state
#define TS_ACTIVE			1
#define TS_RECTIFIED		2

// bought or sold
#define BS_BOUGHT			1
#define BS_SOLD				2

#endif