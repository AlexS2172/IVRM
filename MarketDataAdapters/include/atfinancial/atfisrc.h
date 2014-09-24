/*
 *		atfisrc.h		Date=990721
 *
 *		A-T Financial Feed defines and structures.
 *
 *		Written 06/23/98	R. Holm
 *
 *		Modified 08/31/98	R. Holm
 *			added ATFISRC_* defines.
 *
 *		Modified 09/01/98	R. Holm
 *			added ATFISRC_ATFI.
 *
 *		Modified 09/09/98	R. Holm
 *			added ATFISRC_NYSE_R and ATFISRC_AMEX_R (regional exchanges).
 *
 *		Modified 12/10/98	T. Tian
 *			added DATASRC_TSE.
 *
 *		Modified 12/15/98	R. Holm
 *			added ATFISRC_WSE - Winnipeg Stock Exchange.
 *
 *		Modified 01/13/99	R. Holm
 *			added PERM_FINEX, PERM_NYBOT, and PERM_CSC.
 *
 *		Modified 02/10/99	R. Holm
 * 			added DATASRC_MAINSTREAM and DATASRC_DOWVISION.
 *
 *		Modified 02/12/99	R. Holm
 *			added ATFISRC_CANTOR.
 *
 *		Modified 2/26/99	LM Loh
 *			added DATASRC_NEWS
 *
 *		Modified 06/14/99	R. Holm
 *			added ATFISRC_FIXED_INCOME and ATFISRC_FOREX.
 *
 *		Modified 07/21/99	T. Tian and R. Holm
 *			added ATFISRC_NY_QUOTES.
 *
 *		Modified 08/18/99	T. Tian
 *			added ATFISRC_ABS.
 *
 *		Modified 8/20/99 LM Loh
 *			Added DATASRC_REUTERS
 *			Added DATASRC_COMTEX
 *			Added DATASRC_UNK	
 *
 *
 *		Copyright (c) A-T Financial Information, Inc., 1998-1999.
 *		All rights reserved.
 *
 */

/*=======================================================================*/

#ifndef __ATFI_SOURCES
#define __ATFI_SOURCES

/*=======================================================================*/

// data sources

#define DATASRC_ATFI					0

#define DATASRC_LEVEL1					11
#define DATASRC_LEVEL2					12
#define DATASRC_LEVEL2_A				12
#define DATASRC_LEVEL2_B				13
#define DATASRC_LEVEL2_C				14
#define DATASRC_NTDS					15
#define DATASRC_NTDS_A					15
#define DATASRC_NTDS_B					16
#define DATASRC_CTS						17			
#define DATASRC_CQS						17			
#define DATASRC_OPRA					18			
#define DATASRC_FCO						18			
#define DATASRC_FUTOPT					19			
#define DATASRC_ABS						20			
#define DATASRC_TSE						30			

#define DATASRC_TELESPHERE				100
#define DATASRC_COMSTOCK				101
#define	DATASRC_MAINSTREAM				102
#define DATASRC_DOWVISION				103
#define	DATASRC_REUTERS					104
#define	DATASRC_COMTEX					105
#define	DATASRC_UNK						106

#define	DATASRC_NEWS					150

/*=======================================================================*/

#define	ATFISRC_NYSE				0
#define	ATFISRC_NASDAQ				1
#define	ATFISRC_OPRA				2
#define	ATFISRC_AMEX				3
#define ATFISRC_NASDAQ_LEVEL2		4
								
#define	ATFISRC_CBOT				8
#define	ATFISRC_CME					9
#define	ATFISRC_CEC					10
#define	ATFISRC_COMEX				10
#define	ATFISRC_NYMEX				11
#define	ATFISRC_KCBOT				12
#define	ATFISRC_WGE					13
#define	ATFISRC_MACE				14
#define	ATFISRC_MGE					15
#define ATFISRC_ITC					16
#define ATFISRC_FINEX				17
#define ATFISRC_NYBOT				18
#define ATFISRC_CSC					19

#define ATFISRC_NYSE_R				17
#define ATFISRC_AMEX_R				18

#define ATFISRC_MUTUAL_FUND			20
#define ATFISRC_MONEY_MARKET		21
#define	ATFISRC_SMALLCAP			22
#define ATFISRC_OTCBB				23
#define ATFISRC_BOSTON				24
#define ATFISRC_CINCINNATI			25
#define ATFISRC_MIDWEST				26
#define ATFISRC_PACIFIC				27
#define ATFISRC_THIRD				28
#define ATFISRC_PHLX				29
#define ATFISRC_CBOE				30
#define ATFISRC_FCO					31
								
#define	ATFISRC_SST					32
#define	ATFISRC_GOVPX				33
#define	ATFISRC_FIRSTCALL			34

#define ATFISRC_CANTOR				35
#define ATFISRC_FIXED_INCOME		36

#define ATFISRC_NY_QUOTES			37
								
#define	ATFISRC_CPX_FRX				48
#define	ATFISRC_CPX_CHEMBK_FRX		49
#define	ATFISRC_CPX_MIDLBK_FRX		50
#define	ATFISRC_CPX_NATWBK_FRX		51
#define	ATFISRC_CPX_CITIBK_FRX		52
#define ATFISRC_CPX_BUTLER_FRX		53
#define	ATFISRC_FOREX				54
								
#define	ATFISRC_ENGLAND				64
#define	ATFISRC_LSE					64
#define	ATFISRC_LSSI				65
#define	ATFISRC_GERMANY				66
#define	ATFISRC_FRANKFURTW			66
#define	ATFISRC_IBIS				67
#define	ATFISRC_FRANCE				68
#define	ATFISRC_PSE					68
#define	ATFISRC_SWISS				69
								
#define	ATFISRC_LUXEMBOURG			72
#define	ATFISRC_VIENNA				73
#define	ATFISRC_LISBON				74
#define	ATFISRC_SPANISH				75
								
#define	ATFISRC_NETHERLANDS			77
#define	ATFISRC_AMSTERDAM			77
#define	ATFISRC_BRUSSELS			78
#define	ATFISRC_COPENHAGEN			79
								
#define	ATFISRC_STOCKHOLM			80
#define	ATFISRC_OSLO				81
#define	ATFISRC_HELSINKI			82
#define	ATFISRC_ITALY				83
								
#define	ATFISRC_JOHANNESBURG		85
#define	ATFISRC_TURKEY				86
#define	ATFISRC_JORDAN				87
								
#define	ATFISRC_GREECE				88
#define	ATFISRC_POLAND				89
#define	ATFISRC_IRELAND				90
#define	ATFISRC_ISRAEL				91
#define	ATFISRC_CZECH				92
#define	ATFISRC_HUNGARY				93
#define	ATFISRC_RUSSIA				94
#define	ATFISRC_SLOVAKIA			95
								
#define	ATFISRC_DTB					96
#define	ATFISRC_LTO					97
#define	ATFISRC_MONEP				98
#define	ATFISRC_SOFFEX				99
#define	ATFISRC_EOE					100
#define	ATFISRC_VIENNAOPT			101
								
#define	ATFISRC_LIFFE				104
#define	ATFISRC_IPE					105
#define	ATFISRC_LME					106
#define	ATFISRC_MATIF				107
#define	ATFISRC_FOX					108
#define	ATFISRC_PARISSOFT			109
#define	ATFISRC_DEF			    	110
#define	ATFISRC_NLF			    	111
#define	ATFISRC_CHF			    	112
#define	ATFISRC_ITF			    	113

#define ATFISRC_MOROCCO				120
#define ATFISRC_KENYA				121

#define ATFISRC_CANADA				128								
#define	ATFISRC_TSE			        128
#define	ATFISRC_MSE			        129
#define	ATFISRC_ASE			        130
#define	ATFISRC_VSE			        131
#define	ATFISRC_COATS				132
#define	ATFISRC_WSE					133
								
#define	ATFISRC_MEXICO				136
#define	ATFISRC_BAS					137
#define	ATFISRC_BOGOTA				138
#define	ATFISRC_SPS					139
#define	ATFISRC_MVS					140
#define	ATFISRC_CARACAS				141
#define	ATFISRC_SANTIAGO			142
								
#define	ATFISRC_BOLIVIA				144
#define	ATFISRC_PERU				145
#define	ATFISRC_PARAGUAY			146
								
#define	ATFISRC_TSO					152
#define	ATFISRC_MSO					153
#define	ATFISRC_VSO					154
								
#define	ATFISRC_CONT_TK				168
#define	ATFISRC_CONT_SP				169
#define	ATFISRC_CONT_KR				170
#define	ATFISRC_CONT_RSF			171
#define	ATFISRC_CONT_NDQ			172
#define	ATFISRC_CONT_RS1			173
#define	ATFISRC_CONT_RS2			174
#define	ATFISRC_CONT_PCQ			175
								
#define ATFISRC_AUSOPT           	184
#define ATFISRC_JAPOPT           	185
								
#define	ATFISRC_AUSTRALIA			192
#define	ATFISRC_NEWZEALAND			193
#define	ATFISRC_JAKARKTA			194
#define	ATFISRC_HONGKONG			195
#define	ATFISRC_SINGAPORE			196
#define	ATFISRC_MANILA				197
#define	ATFISRC_KL					198
#define	ATFISRC_SEOUL				199
								
#define	ATFISRC_TAIPEI				200
#define	ATFISRC_BANGKOK				201
#define	ATFISRC_JAPAN				202
#define	ATFISRC_TOKYO				202
#define	ATFISRC_HIROSHIMA			203
#define	ATFISRC_NAGOYA				204
#define	ATFISRC_OSAKA				205
#define	ATFISRC_FUKUOKA				206
#define	ATFISRC_SAPPORO				207
								
#define	ATFISRC_KYOTO				208
#define	ATFISRC_NIIGATA				209
#define	ATFISRC_JASDAQ				210
#define	ATFISRC_INDIA				211
#define	ATFISRC_PAKISTAN			212
#define ATFISRC_SRILANKA         	213
#define ATFISRC_CHINA				214
#define ATFISRC_MAURITIUS			215
								
#define	ATFISRC_SYDNEY				224
#define	ATFISRC_SIMEX				225
#define	ATFISRC_TIFFE				226
#define	ATFISRC_HKF					227
#define	ATFISRC_HKM					228
								
#define	ATFISRC_SSTNEWS				232
#define	ATFISRC_CARVIEW				233
#define	ATFISRC_SCOPE				234
#define	ATFISRC_SPMVIEW				235
#define	ATFISRC_EARNVIEW			236
#define	ATFISRC_MMS					237
#define	ATFISRC_MAX					237

#define ATFISRC_ALL					510
#define ATFISRC_ATFI				510
#define ATFISRC_NONE				511

/*=======================================================================*/

#endif

/*=======================================================================*/

