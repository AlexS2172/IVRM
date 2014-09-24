/*
 *    textp.h     Date=930526
 *
 *    stuff for entities processed by the textp handler.
 */

/*======================================================================*/
#include <pcqdefs.h>
/*======================================================================*/

#ifndef _INCL_TEXTP
#define _INCL_TEXTP

#ifndef INCL_NOPACKING
	#pragma pack(2)						 /* Word aligned. */
#endif

/*======================================================================*/

/*
 * item_file:  the item file contains all data received on the item lines( DJ,
 *             AP ). it is structured as follows.
 *
 *				/----------------------------\
 *				|	item area and text area   |
 *				|			 pointers			  |
 *				|----------------------------|
 *				|	item specs...1 for each   |
 *				|	 specified in pcqt.cfg	  |
 *				|----------------------------|
 *				|	text area.....up to max   |
 *				|	 specified in pcqt.cfg	  |
 *				\----------------------------/
 *
 *				use the structures below to access the different portions of
 *				the item file.
 */ 

/*======================================================================*/

/* Added the (LONG) to eliminate a C6 compiler warning. */

#define	FOFFSET( struct_id, field_id ) ( (SHORT)(LONG)&( ( struct_id *)0l )->field_id )

#define	TEXT_LINE_SIZE ( FOFFSET( TEXT_LINE, buf[0] ) )

/* the text file identifier.... */

#define TEXT_FILE "\\PCQT1071"

/*======================================================================*/

/*
 * TEXT_HEADER:	this structure defines the header of the item file.
 */

typedef struct _TEXT_HEADER {
   USHORT  item_max;			      /* total items to be tracked */
	USHORT  item_start; 		      /* oldest item on list */
	USHORT  item_end;			      /* newest item on list */
	USHORT  item_next;		      /* place to add next item */
	ULONG   textarea_max;	      /* max size of text portion of item file */
	ULONG   textarea_start;	      /* oldest byte of text area */
	ULONG   textarea_end;	      /* next byte( to be written ) of text area */
	UCHAR   open; 				      /* true if file is being maintained by background */
	UCHAR   reserved[11];	      /* go to 32 byte boundary */
} TEXT_HEADER;

#define TEXT_HEADER_SIZE   ( sizeof( TEXT_HEADER ))

/*======================================================================*/

/*
 * TEXT_SPEC:	this structure is used to identify and locate textp items.
 */
typedef struct _TEXT_SPEC {
	BIT_FIELD  source		:7;		/* identify source of item */
	BIT_FIELD  alert_item	:1;
	BIT_FIELD  blank_cnt	:5;		/* count of leading blank lines this item */
	ULONG      start;				/* first byte of item */
} TEXT_SPEC;

#define	TEXT_SPEC_SIZE ( sizeof( TEXT_SPEC ))

/*======================================================================*/

/*
 * TEXT_LINE:	this structure is used to access the text area of the item file.
 *					each text area line is preceded by this structure.
 */

typedef struct _TEXT_LINE {
	BIT_FIELD	length		:8;			/* length of entire txtline_s */
	BIT_FIELD	source		:6;			/* same as TEXT_SPEC.source */
	BIT_FIELD	nonblank		:1; 		/* true if other than 7, 10, or 13 */
	BIT_FIELD	last			:1;			/* true if last record of item */
	UCHAR		type; 				   	/* line type preserved from funnel */
	UCHAR		buf[256];
} TEXT_LINE;

/*======================================================================*/

/*
 * MSG_HDR:	this struct defines the header of a message sent to the textp.
 */
 
typedef struct _MSG_HDR {
	BIT_FIELD  line_type	:4;	      /* will receive value of current line type */
	BIT_FIELD  source		:4;	      /* will always contain wow jones source id */
	BIT_FIELD  reserved 	:8;	      /* used as length byte							 */
} MSG_HDR;

#define	MSG_HDR_SIZE	( sizeof( MSG_HDR ) )

/*======================================================================*/

typedef struct _TEXTSTRUC {
	USHORT		 maxItems;
	USHORT		 textFlags;			   /* news mask 0x80 eti 0x40 database */
	UCHAR 		 news_cat[32];		   /* catagories of news desired 		*/
	ULONG 		 headline_flags;	   /* source of headline flags desired */
	SHORT		 text_h;
	TEXT_HEADER	 text_header;
} TEXTSTRUC;

/*======================================================================*/

#define	HEADLINE0			0
#define	HEADLINE1			1
#define	BODYLINE 			2
#define	LASTLINE 			3
#define CONTINUELINE		4
#define NEWSALERT			15

/* output header struct for apnews */

typedef struct _AP_OUT_HEADER {
   UCHAR	control[2];
   UCHAR	story_number[2];
   UCHAR	num_category[2];
   UCHAR	service_level;
   UCHAR	priority;
   UCHAR	category;
   UCHAR	headline[24];
} AP_OUT_HEADER;

/*======================================================================*/

/*
 *	  service levels are: a & b	  nationwide news transmission
 *								 c 		  nationwide selected standing features
 *								 d & e	  state and regional wires
 *								 f 		  nationwide financial news
 *								 g & h	  state and regional wires
 *								 i 		  reserved
 *								 j - q	  state and regional wires
 *								 r & s	  nationwide sports
 *								 t 		  state and regional wires
 *								 u - z	  reserved for wire services
 *								 V 		  viewtext - this is us -
 *
 *	  prioritys are:		 f 		  flash highest priority(seldom used)
 *								 b 		  bulletin prime news
 *								 u 		  urgent just under prime news
 *								 r 		  regular routine news copy
 *								 d 		  deferred lowest priority
 *								 a 		  weekend advance
 *								 s 		  weekend advance for weekend copy
 *								 h 		  ap transmission to be treated as "d"
 *								 y 		  ap transmission ids certain reruns
 *
 *	  categorys are:		 a & b	  domestic non-washington general news
 *
 * 							 c 		  standing general features
 *								 e 		  entertainment and culture
 *								 f 		  financial for financial pages
 *								 i 		  international news
 *								 k 		  commentary editorial
 *								 l 		  lifestyle
 *								 n 		  state and regional(ap)
 *								 q 		  individual sports scores
 *								 r 		  racing results
 *								 s 		  sports including packages of sports scores
 *								 t 		  travel
 * 							 u 		  state and regional(upi)
 *								 v 		  advisories that affect more than one cat.
 *								 w 		  washington dateline
 *								 y - z	  reserved by wire services
 */

/*======================================================================*/

#ifndef INCL_NOPACKING
	#pragma pack()
#endif

#endif

/*======================================================================*/
