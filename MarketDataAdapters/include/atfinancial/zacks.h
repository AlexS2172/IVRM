// Zacks.h
// Zacks Header File 
// By Wade and Jane, 4/97

#if !defined(_ZACKS_H)
#define	_ZACKS_H

#ifndef RC_INVOKED
#pragma pack(1)
#endif /* RC_INVOKED */

#ifdef	__cplusplus
extern	"C" {
#endif

#define ZACK_SEE_TIER_ONE       0
#define ZACK_SEE_TIER_TWO       1
																               
#define ZACK_SRCH_FORWARD		0
#define ZACK_SRCH_BACKWARD		1

// maximum zacks summaries
#define MAX_ZACK_SUMS			8
#define MAX_ZACK_RECORD		   14

//key type
#define ZKT_TICKERS				0
#define ZKT_COMPANY				1
#define ZKT_INDCODE             2

#define	PAGETYPE_DA				0
#define PAGETYPE_QE				1
#define PAGETYPE_QA				2
#define PAGETYPE_AE				3
#define PAGETYPE_AP				4

#define CMD_CLOSE				255		//Client closes app

// maximum value stored in the value field (31 bits)
#define MAX_ZK_VALUE_DEC	 2147583648  // 2**31

// zacks server errors returned in status field of response
#define ZACKERR_SUCCESS				0		// successful call
#define ZACKERR_DB_OPEN				1		// zackers diskDB open error
#define ZACKERR_INVALID_KEY			2		// invalid key number specified
#define ZACKERR_INVALID_FUNC		3		// invalid function specified
#define ZACKERR_INVALID_MSG_TYPE	4		// invalid message type ('z' '?')
#define ZACKERR_KEY_NOT_FOUND		5		// key not found in database
#define ZACKERR_SERVER_BUSY			6		// server could not process request at this time
#define ZACKERR_SERVER_SHUTDOWN		7		// server could not process request, shutting donw
#define ZACKERR_NOT_LOGGED_IN		8		// server could not process request, workstation not logged in
#define ZACKERR_SUBCOUNT			9		// invalid sub count specified

// ===========================================================================
// value field with implied decimals
typedef	struct tagZK_VALUE_DEC
{
	DWORD	value:	31,		// Lower  31 bits = value with implied 2 decimal digits
			sign:	1;		// Upper 1 bit = sign ( 0 == positive or zero, 1 == negative
} ZK_VALUE_DEC, *LPZK_VALUE_DEC;

/*typedef struct tagZK_VALUE_WORD
{
	WORD	value:	15,
			sign:	1;
}ZK_VALUE_WORD, *LPZK_VALUE_WORD;			  */

// ===========================================================================
// compressed date field -- Day, Month, Year
typedef	struct _ZK_COMP_DATE
{
	WORD	day:  5,		// Lower  5 bits = day   (1 - 31)
			month:4,		// Middle 4 bits = month (1 - 12)
			year: 7;		// Upper  7 bits = year	 (0 - 99) NOTE: Offset from 1970
} ZK_COMP_DATE, *LPZK_COMP_DATE;

typedef struct tagZACK_DB_RECORD																								     
{
	char			sTickerSymbol[10];
	char			sCompany[15];
	BYTE			byFYE_MONTH;
	WORD			wIndustry;
	BYTE			bySector;
	ZK_VALUE_DEC	vdEST_EPS_F1;					
	ZK_VALUE_DEC	vdEST_EPS_F2;					
	ZK_VALUE_DEC	vdEST_EPS_Q1;						
	ZK_VALUE_DEC	vdEST_EPS_Q2;
	ZK_VALUE_DEC	vdCH_4WK_F1;
	ZK_VALUE_DEC	vdCH_4WK_F2;
	ZK_VALUE_DEC	vdCH_4WK_Q1;
	ZK_VALUE_DEC	vdHI_EST_F1;					
	ZK_VALUE_DEC	vdHI_EPS_F2;
    ZK_VALUE_DEC	vdLOW_EST_F1;
	ZK_VALUE_DEC	vdLOW_EPS_F2;
	ZK_VALUE_DEC	vdPE_12M;
	ZK_VALUE_DEC	vdPE_F1;
	ZK_VALUE_DEC	vdPE_F2;
	ZK_VALUE_DEC	vdREL_PE_12M;
	ZK_VALUE_DEC	vdREL_PE_F1;
	ZK_VALUE_DEC	vdREL_PE_F2;
	ZK_VALUE_DEC	vdEST_EPS_GR;
    ZK_VALUE_DEC	vdAVG_RATING;
	WORD			wAN_RATING;
	ZK_VALUE_DEC	vdLAST_QSUR1;
	ZK_VALUE_DEC	vdLAST_QSUR2;
	ZK_VALUE_DEC	vdACT_EPS_Y1;
	ZK_VALUE_DEC	vdACT_EPS_Y2;
	ZK_VALUE_DEC	vdSDV_EPS_F1;
	ZK_VALUE_DEC	vdSDV_EPS_F2;
	ZK_VALUE_DEC	vdSDV_EPS_Q1;
	ZK_VALUE_DEC	vdSDV_EPS_Q2;
	ZK_COMP_DATE	cdEXP_REP_DT;
	BYTE			byAN_EPS_F1;
	ZK_VALUE_DEC    vdCH_4WK_Q2;
	ZK_VALUE_DEC    vdHI_EST_Q1;
	ZK_VALUE_DEC    vdLOW_EST_Q1;
	ZK_VALUE_DEC    vdHI_EPS_Q2;
	ZK_VALUE_DEC    vdLOW_EPS_Q2;
}ZACK_DB_RECORD, *LPZACK_DB_RECORD;

typedef struct tagZACK_TIERONE_RECORD																								     
{
	char			sTickerSymbol[10];
	char			sCompany[15];
	BYTE			byFYE_MONTH;

	ZK_VALUE_DEC	vdEST_EPS_F1;					
	ZK_VALUE_DEC	vdEST_EPS_F2;					
	ZK_VALUE_DEC	vdEST_EPS_Q1;						
	ZK_VALUE_DEC	vdEST_EPS_Q2;
	ZK_COMP_DATE	cdEXP_REP_DT;
	BYTE			byAN_EPS_F1;
	ZK_VALUE_DEC	vdEST_EPS_GR;
}ZACK_TIERONE_RECORD, *LPZACK_TIERONE_RECORD;

typedef struct tagZACK_QUATEST_RECORD  //Quarterly Estimates EPS, 36 bytes
{
	char			sTickerSymbol[10];
	ZK_VALUE_DEC	vdEST_EPS_Q1;
	ZK_VALUE_DEC	vdCH_4WK_Q1;
	ZK_VALUE_DEC	vdEST_EPS_Q2;
	ZK_VALUE_DEC    vdCH_4WK_Q2;
	ZK_VALUE_DEC	vdAVG_RATING;
	WORD			wAN_RATING;
	ZK_VALUE_DEC	vdEST_EPS_GR;
}ZACK_QUATEST_RECORD, *LPZACK_QUATEST_RECORD;

typedef struct tagZACK_QUATACT_RECORD  //Quarterly Actual EPS, 30 bytes
{
	char			sTickerSymbol[10];
	ZK_VALUE_DEC	vdACT_EPS_Y1;
	ZK_VALUE_DEC	vdLAST_QSUR1;
	ZK_VALUE_DEC	vdACT_EPS_Y2;
	ZK_VALUE_DEC	vdLAST_QSUR2;
	ZK_VALUE_DEC	vdEST_EPS_GR;
}ZACK_QUATACT_RECORD, *LPZACK_QUATACT_RECORD;

typedef struct tagZACK_ANULEST_RECORD  //Annual Estimates, 36 bytes
{
	char			sTickerSymbol[10];
	ZK_VALUE_DEC	vdEST_EPS_F1;					
	ZK_VALUE_DEC	vdCH_4WK_F1;
	ZK_VALUE_DEC	vdEST_EPS_F2;
	ZK_VALUE_DEC	vdCH_4WK_F2;
	ZK_VALUE_DEC	vdAVG_RATING;
	WORD			wAN_RATING;
 	ZK_VALUE_DEC	vdEST_EPS_GR;
}ZACK_ANULEST_RECORD, *LPZACK_ANULEST_RECORD;

typedef struct tagZACK_ANULPE_RECORD	//Annual P/E, 34 bytes
{
	char			sTickerSymbol[10];
	ZK_VALUE_DEC	vdPE_F1;
	ZK_VALUE_DEC	vdREL_PE_F1;
	ZK_VALUE_DEC	vdPE_F2;
	ZK_VALUE_DEC	vdREL_PE_F2;
	ZK_VALUE_DEC	vdPE_12M;
	ZK_VALUE_DEC	vdREL_PE_12M;
}ZACK_ANULPE_RECORD, *LPZACK_ANULPE_RECORD;

typedef union tagZACK_RECORD
{
	ZACK_DB_RECORD		ZackTier2Record;
	ZACK_TIERONE_RECORD	ZackTier1Record;
	ZACK_QUATEST_RECORD	ZackQuatEstRecord[MAX_ZACK_RECORD];
	ZACK_QUATACT_RECORD	ZackQuatActRecord[MAX_ZACK_RECORD];
	ZACK_ANULEST_RECORD	ZackAnulestRecord[MAX_ZACK_RECORD];
	ZACK_ANULPE_RECORD	ZackAnulpeRecord[MAX_ZACK_RECORD];
}ZACK_RECORD, *LPZACK_RECORD;

typedef union tagZACK_SUMMARY_RECORD
{
	char	sTickerSymbol[MAX_ZACK_SUMS][10];
	char	sCompany[MAX_ZACK_SUMS][15];
}ZACK_SUMMARY_RECORD, *LPZACK_SUMMARY_RECORD;


typedef union tagZACK_KEYS
{
	char	sTickerSymbol[10];
	char	sCompany[15];
	UCHAR	key[30];		// general purpose member to access this in a generic way	
} ZACK_KEYS, *LPZACK_KEYS;	// NOTE: key must be at least 4 bytes larger than biggest key


#ifdef __cplusplus
}
#endif

#ifndef RC_INVOKED
#pragma pack()
#endif /* RC_INVOKED */

#endif

