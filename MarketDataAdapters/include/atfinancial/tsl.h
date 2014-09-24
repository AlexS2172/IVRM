/*
 *		tsl.h		Date=990727
 *
 *		Technical Study Library Structures.
 *
 *		Written 02/03/98	S. Xia
 *
 *		Modified 07/10/98	C. Baker
 *			Added dwMinMemoryReq to TSIOREQ
 *
 *		Modified 07/13/98	C. Baker
 *			Added constant TS_PRICE_VOLUME_TIME_CONVERT
 *
 *      Modified 07/17/98   Xia
 *          Added constant TS_REGCAPSTR and prototype REGCAPSTR
 *
 *		Modified 07/20/98   Xia
 *          Added constant TS_STDDLGSTR and prototype STDDLGSTR
 *
 *		Modified 12/18/98   Xia
 *          Added dwStudyScale in IOReq
 *
 *		Modified 06/08/99   C. Baker
 *          Exported writeRegistry()
 *
 *		Modified 07/20/99	C. Baker
 *			Change enum POSITION to TSL_POSITION due to conflict with MFC
 *
 *		Modified 07/27/99	C. Baker
 *			Add Historical Volatility Constant
 *
 *
 *		Copyright (c) A-T Financial Information, Inc., 1998-1999.
 *		All rignts reserved.
 *
 */

/*=============================================================*/
/*
**** Special Types,structures and values used by ALL Technical Studies Libaries
****
**** tsl.h is used by the TSL's, Athena and the Data manager
*/
#ifndef _TSL_H_
#define _TSL_H_

#include "struct.h"

#define TS_MAX_TSP				512
#define TS_MAX_INPUT			16
#define TS_LIB_SUB_KEY_NAME_LEN	32
#define TS_STUDY_SHORT_NAME_LEN 8
#define TS_STUDY_LONG_NAME_LEN  128
#define TS_INPUTDATA_LEN		64
#define TS_MAX_LINE_NUM			32
#define TS_MAX_SYMBOL_LEN		200
#define TS_PARAM_NEXT			32
#define TS_MAX_REG				50
#define TS_REG_PATH_NAME_LEN	64
#define TS_STUDY_PARAM_DEF_LEN	32
#define TS_PARAM_NEXT_SHORT	    8
#define TS_INPUTDATA_LEN_SHORT  16
#define TS_PRICE_VOLUME_TIME_CONVERT 10000
#define TS_STDDLGSTR_LEN        200
#define TS_HISTORICAL_VOL_FLAG  "999999999"

#ifdef __cplusplus

enum STUDY_GROUP { G_MA, G_ENV, G_OSC, G_VOL_OPI, G_STO_RSI, G_MONEY_FLOW, G_PORTFOLIA,
				   G_PRICE_VOL, G_POINT_FIG, G_OPTION }; //classify all studies into different groups

typedef	struct	_TSLINFO		// Provides info about this whole DLL
		{
		char	szLibSubKeyName[TS_LIB_SUB_KEY_NAME_LEN];
		WORD	wVersionNum;		// 100*major + minor
		char	sVersionLetter;
		DWORD	dwOrdinalNext; //relative position of studies
		WORD	wOrdinalBase;  //begining of study definitions
		WORD	wStudies; // number of studies
		}
		TSLINFO, *LPTSLINFO;

typedef	struct	_TSINFO			// Information about a specific study
		{
		char	szShortName[TS_STUDY_SHORT_NAME_LEN];// "Menu" name of study
		char	szLongName[TS_STUDY_LONG_NAME_LEN];	// Slightly more verbose
		char	szDescription[TS_MAX_TSP];// Full description
		char	sSymbol;              //'-' for spread, '/' for division, '' otherwise
		char	szParamDefinition[TS_STUDY_PARAM_DEF_LEN]; //s=string, d=decimal... example sma: ifb
		char	szSymbolParamDef[TS_STUDY_PARAM_DEF_LEN]; //example sma: sifb
		enum ATC_BSTYLE		dwDefaultBarStyle; //display default bar style for each study;
		enum STUDY_GROUP	dwGroup; //all studies are classified as their meaning. one study belongs one and only one group
		DWORD	dwNoParameter; //if there is no parameter, like volume, set to TRUE. Otherwise, FALSE
		}
		TSINFO, *LPTSINFO;

// constants used by dwPreviousOutputData
#define TS_PREV_OUTPUT_LAST 1   // last output needed
#define TS_PREV_OUTPUT_NONE 0   // no output needed
#define TS_PREV_OUTPUT_ALL  99  // all output needed

typedef	struct	_TSIOREQ		// I/O Requirements of a specific study
		{
		//input info
	    DWORD	dwNeedTimeData;  // 1 = yes, 0 = no
		DWORD   dwRequiredInputDataType; //Bar | Tick | Study
	   	DWORD	dwMultInputDataFlag; // 0 = only one input buffer needed, 1 = more than 1 input buffer needed
				//output info
		DWORD	dwOutputTimeData; // 1 = yes, 0 = no
		DWORD	dwPreviousOutputData; // see constants TS_PREV_OUTPUT_*
		DWORD	dwOutputDataType; //study, price_volume, point_figure
		DWORD   dwNumFields; //number of fields that will be in the output data record except time
		enum TSL_POSITION dwOutputPlace; //=P_MERGE if merge with input, =P_ATTACH if attach to, 
							   //= P_ATTACH_EX if exclusive attach
		enum ATC_STUDY_SCALES dwStudyScale; //display scales for some study in the restricted interval;
		DWORD   dwDataManIndexes; //=TRUE if data manager sets up indexes; =FALSE if tsl sets up indexes
		DWORD	dwExtraMemory; //the data Type of memory is double. This field specifies the number of doubles to allocate
		DWORD	dwMinMemoryReq; //the minimum output memory required, (the number of records which should initially be allocated) only used when tsl sets up indexes -> dwDataManIndexes == FALSE
		}
		TSIOREQ, *LPTSIOREQ;

typedef struct _TSPARAM
{
	enum PERIOD iPeriod; //tick, one min., 5 min., day ...
	DWORD dwInputDataType; //Historical, tick, study...?
	DWORD dwNumberOfFields; //Number of fields in the input record
	char szInputDataNames[TS_MAX_LINE_NUM][TS_INPUTDATA_LEN]; //list of names of buffer
	DWORD dwInputDataIndex; //For spread, basket ..., what is the index of first input chosen?
	//output
	DWORD dwOutputDataNewOrOld[TS_MAX_LINE_NUM]; //new = 1, old = 0
	char szOutputDataNames[TS_MAX_LINE_NUM][TS_INPUTDATA_LEN]; //for spread and baskets ... who need extra symbol as input 
	DWORD dwDailyData; //for moving average only. Use daily data = 1, otherwise = 0.
	double dBoxSize; //for point & figure only
	char sStudyParam[TS_MAX_TSP]; //This term holds the "study" specific information. The actual format of 
	                  //the data in the pData only known to the library.
} TSPARAM, *LPTSPARAM; //used in every study parameter dialog box

//used for dwBuildStatus
#define TS_BUILD_STATUS_INITIAL 100  // First Time Build function is called
#define TS_BUILD_STATUS_NEW		101  // Build function called with more recent data - data is put on top
#define TS_BUILD_STATUS_OLD		102  // Build function called with older data - data is put on bottom

typedef	struct	_TSPIF //study input parameter, used by DLL and data manager
		{
		//these are the input information
		ATDM_TIME *lpTimeInputs[TS_MAX_INPUT]; //List of input time data points
		LPVOID	  lpInputs[TS_MAX_INPUT];	// Pointers to input data
	    DWORD	  dwInputMostRecIndex[TS_MAX_INPUT]; // list of input most recent position
		DWORD     dwInputOldestIndex[TS_MAX_INPUT];  // list of input oldest position
		DWORD	  dwInputDataType[TS_MAX_INPUT]; //Historical, tick, study...?
		DWORD	  dwNumberOfFields[TS_MAX_INPUT]; //Number of fields in the input record
		DWORD	  dwBuildStatus;
		DWORD	  dwPeriod; //Time period, TP_WEEKLY, .... used when using daily data moving average overlay on weekly or monthly chart
		char	  sStudyParam[TS_MAX_TSP]; //This term holds the "study" specific information. 
									//The actual format of the data in the pData only known to the library.
		
		//The following are the input, output information
		DWORD	  dwOutputMostRecIndex; //output most recent position
		DWORD	  dwOutputOldestIndex; //output oldest position
		DWORD     dwTotalAllocated; //total number 
		ATDM_TIME *lpTimeOutput; //List of output time data points
		LPVOID	  lpOutput;	// Pointers to output buffer
		double	 *lpdExtraMemory; //extra memory used for tslbasic to store some data for updating
		DWORD	  dwNumAddedRecords; //number of new input records without counting the update existing
		//output info
		DWORD	  retCode;
		}
		TSPIF, *LPTSPIF;

// Ordinal offset (from ordinal base)

#define	TS_INFO			0
#define	TS_IOREQ		1
#define	TS_DIALOG		2
#define TS_EXTRAINPUTS  3
#define	TS_BUILD		4
#define TS_DEFAULT		5
#define TS_DISPLAYSTR   6
#define TS_CONVERTSTR	7
#define TS_PARAM		8
#define TS_REGCAPSTR    9
#define TS_STDDLGSTR    10


// Function prototypes

typedef DWORD (WINAPI *TSLINFOPROC)(TSLINFO *);//library information
typedef DWORD (WINAPI *TSINFOPROC)(TSINFO *);//study information
typedef DWORD (WINAPI *TSIO) (TSIOREQ *);//input and output requirement
typedef DWORD (WINAPI *TSDLGPROC) (HWND, LPTSPARAM);//dialog box
typedef DWORD (WINAPI *BUILD)(TSPIF *);//calculate studies
typedef DWORD (WINAPI *EXTRAINPUTS)(char *, int *);//extra inputs only needed when datamanage sets up output range
typedef DWORD (WINAPI *DEFAULT) (char *, char *, char *);//read default special string from registry
typedef DWORD (WINAPI *DISPLAYSTR) (...);//combing short name and parameter for display purpose
typedef DWORD (WINAPI *CONVERTSTR) (char *, LPTSPARAM); //from special string to TSPARAM
typedef DWORD (WINAPI *PARAM)(...); //used for input string parsing, it takes input parameter strings and output TSPARAM
typedef DWORD (WINAPI *REGCAPSTR)(char *, char *); //used for reginal caption display, second parameter = sStudyParam (in TS_PARAM) 
typedef DWORD (WINAPI *STDDLGSTR)(char *, char *); //used in study dialog box to display the study parameters when the user clicks on a study, 
													//second parameter = sStudyParam

//  This function is used to put parameter string to registry when the user pushes default button
//  The registry key is: "Software\\A-T Financial\\Atlas\\Study\\keyName";
BOOL writeRegistry(char *pData, char *keyName);

#endif

#endif


