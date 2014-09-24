#ifndef _FIXCONST_H
#define _FIXCONST_H

typedef enum
{
	// tags necessary for FIX message header
	enFtBeginString = 8,
	enFtBodyLength = 9,
	enFtMsgType = 35,
	enFtSenderCompID = 49,
	enFtTargetCompID = 56,
	enFtMsgSeqNum = 34,
	enFtSendingTime = 52,

	// tags necessary for FIX logon message 
	enFtEncryptMethod = 98,
	enFtHeartBtInt = 108,	enFtResetSeqNumFlag = 141,

	// tags necessary for FIX test request message 
	enFtTestReqID = 112,

	// tags necessary for FIX resend request message 
	enFtBeginSeqNo = 7,
	enFtEndSeqNo = 16,

	// tags necessary for FIX reject request message 
	enFtRefSeqNum = 45,
	enFtText = 58,		// not required but recommended

	// tags necessary for FIX sequence reset (gap fill) message 
	enFtGapFillFlag = 123,
	enFtNewSeqNo = 36,

	// tags necessary for FIX message trailer
	enFtCheckSum = 10,

	// possible send & duplicate
	enFtPossDupFlag = 43,
	enFtPossResend = 97,

	// unknown tag
	enFtUnknown  = -1,

	// tags for execution reports
	enFtOrderID = 37, //Y
	enFtSecondaryOrderID = 198, //N
	enFtClOrdID = 11, //N
	enFtOrigClOrdID = 41, //N
	enFtClientID = 109, //N
	enFtExecBroker = 76, //N
	enFtListID = 66, //N
	enFtExecID = 17, //Y
	enFtExecTransType = 20, //Y
	enFtExecRefID = 19, //N
	enFtExecType = 150, //Y
	enFtOrdStatus = 39, //Y
	enFtOrdRejReason = 103, //N
	enFtAccount = 1, //N
	enFtSettlmntTyp = 63, //N
	enFtFutSettDate = 64, //N
	enFtSymbol = 55, //Y
	enFtSymbolSfx = 65, //N
	enFtSecurityID = 48, //N
	enFtIDSource = 22, //N
	enFtSecurityType = 167, //N
	enFtMaturityMonthYear = 200, //N
	enFtMaturityDay = 205, //N
	enFtPutOrCall = 201, //N
	enFtStrikePrice = 202, //N
	enFtOptAttribute = 206, //N
	enFtSecurityExchange = 207, //N
	enFtIssuer = 106, //N
	enFtSecurityDesc = 107, //N
	enFtSide = 54,	//Y
	enFtOrderQty = 38, //Y
	enFtOrdType = 40, //N
	enFtPrice = 44, //N
	enFtStopPx = 99, //N
	enFtPegDifference = 211, //N
	enFtCurrency = 15, //N
	enFtTimeInForce = 59, //N
	enFtExpireTime = 126, //N
	enFtExecInst = 18, //N
	enFtRule80A = 47, //N
	enFtLastShares = 32, //Y
	enFtLastPx = 31, //Y
	enFtLastSpotRate = 194, //N
	enFtLastForwardPoints = 195, //N
	enFtLastMkt = 30, //N
	enFtLastCapacity = 29, //N
	enFtLeavesQty = 151, //Y
	enFtCumQty = 14, //Y
	enFtAvgPx = 6, //Y
	enFtTradeDate = 75, //N
	enFtTransactTime = 60, //N
	enFtReportToExch = 113, //N
	enFtCommission = 12, //N
	enFtCommType = 13, //N
	enFtSettlCurrAmt = 119,//N
	enFtSettlCurrency = 120, //N
	enFtSettlCurrFxRate	= 155, //N
	enFtSettlCurrFxRateCalc = 156, //N
	enFtUnderlying = 311,	//?
	enFtOpenClose = 77,
	enFtTag375 = 375
}	FixTagEnum;

typedef enum
{
	enFemNone = 0,
	enFemDES = 2,
	enFemPKCS_DES = 3,
	enFemPGP_DES_MD5 = 4,
	enFemPEM_DES_MD5 = 5
} FixEncryptMethodEnum;

typedef enum
{
	enFmtLogon			= 'A',
	enFmtTestRequest	= '1',
	enFmtHeartBeat		= '0',
	enFmtResendRequest  = '2',
	enFmtReject			= '3',
	enFmtSequenceReset  = '4',
	enFmtLogout			= '5',
	enFmtExecutionReport = '8',

	enFmtUnknown		= '?'
} FixMsgTypeEnum;

#define	SOH								((unsigned char)01)
#define HEART_BIT_INTERVAL				((unsigned int)30)
#define SHUTDOWN_TIMEOUT				((unsigned int)20)
#define MAX_LOGON_SAME_SESSION_TRIES	10

#endif