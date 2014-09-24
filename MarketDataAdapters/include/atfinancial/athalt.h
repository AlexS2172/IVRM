/*
 *		athalt.h
 *
 *		Halt/Resume structure and messages
 *
 *		Copyright (c) A-T Financial Information, Inc., 1996.
 *		All rights reserved.
 *
 */

#if !defined(_ATHALT_H)
#define	_ATHALT_H

#ifndef RC_INVOKED
#pragma pack(1)
#endif /* RC_INVOKED */

#ifdef	__cplusplus
extern	"C" {
#endif

typedef	struct
		{
		char	sym[8];
		char	cce[3];
		WORD	time;
		BYTE	code;
		BYTE	extra[2];
		}
		HALTDATA, *LP_HALTDATA;

typedef struct
		{
		BYTE	Q,three;		// Message Type 'Q3'
		DWORD	dwRecNum;		// Record number. Note: 0xffff5441 is a special "test" value
		}
		QSM_RQ_HALT;

typedef	struct
		{
		BYTE	Q,three;		// Message Type 'Q3'
		BYTE	nRecs;			// Number of records in this message
		BYTE	nStatus;		// 0 = End, no more, kaput, finito
		BYTE	byData[530];	// Compressed data
		}
		QSM_RS_HALT;

// Note: A QS will respond to  Q3 message with the
//		 dwRecNum = 0xffff5441 with nRec = 'O' and the nStatus = 'K'
//
//       also the byData is encoded this way: {Symbol,MSB CCE,Time,Halt Code}
//		 The Symbol is terminated by the first byte that has the MSB set, this will
//       be the first byte of the Country code, the next two bytes will also have
//		 their upper bits set (second Country code and Exchange) then the next
//		 WORD will be the time, the final BYTE will be the Halt code.
//       The minimum record will be 7 bytes, the maximum will be 14 bytes.


#define	HC_OPENDELAYED					0xa8
#define	HC_TRADINGHALT					0xa9
#define	HC_ORDERINFLUX_ZP				0xaa
#define	HC_ORDERIMBALANCE_ZP			0xab
#define	HC_NEWSPENDING_ZP				0xac
#define	HC_NEWSDISSEMINIATION_ZP		0xad
#define	HC_RELATEDSECURITY_ZP			0xae
#define	HC_INVIEWOFCOMMON_ZP			0xaf

#define	HC_NORESUMERESTOFDAY_ZP			0xb0
#define	HC_TRADINGRESUMED_ZP			0xb2
#define	HC_TRADINGRESUMED_NZP			0xb3

#define	HC_ORDERINFLUX_NZP				0xdc
#define	HC_ORDERIMBALANCE_NZP			0xdd
#define	HC_NEWSPENDING_NZP				0xde
#define	HC_NEWSDISSEMINIATION_NZP		0xdf

#define	HC_RELATEDSECURITY_NZP			0xe0
#define	HC_INVIEWOFCOMMON_NZP			0xe1
#define	HC_NORESUMERESTOFDAY_NZP		0xe2

#define	HC_ORD_RELATEDSECURITYNEWSDISS	0xe6
#define	HC_ORD_RELATEDSECURITYNEWSPEND	0xe7
#define	HC_ORD_EQUIPMENTCHANGEOVER		0xe8

#ifdef	__cplusplus
}
#endif

#ifndef RC_INVOKED
#pragma pack()
#endif /* RC_INVOKED */

#endif

