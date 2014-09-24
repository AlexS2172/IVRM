/*
 *		udjndv.h		Date=940907
 *
 *		DJ Dow Vision User Application header file.
 *
 */

/*=====================================================================*/

// key types (see DV spec)

#ifndef __UDJNDV_H
#define __UDJNDV_H

#define MAX_DV_TEXT_SIZE 14336

#define KT_COMPANY				'C'			// company
#define KT_INDUSTRY				'I'			// industry
#define KT_GOVERNMENT			'G'			// government
#define KT_PAGE					'P'			// page
#define KT_SUBJECT				'S'			// subject
#define KT_MARKET					'M'			// market
#define KT_PRODUCT				'R'			// product
#define KT_GEOGRAPHIC			'E'			// geographic
#define KT_STATS					'T'			// stats
#define KT_CUSIP					'U'			// CUSIP number

#define SIZE_PRODUCT				2						// size of the product field
#define SIZE_DATE					8						// size of the date field
#define SIZE_SEQNUM				4						// size of the sequence number field
#define SIZE_KEY0					SIZE_PRODUCT+SIZE_DATE+SIZE_SEQNUM

/*=====================================================================*/

// these are the structures that describe the keys in the INDEX.DV file

typedef struct _DV_INDEX_KEY0 {
	UCHAR   product[2];							// DV product
	UCHAR   date[8];								// date of story
	UCHAR   sequence[4];							// sequence number of story
} DV_INDEX_KEY0;

typedef struct _DV_INDEX_KEY1 {
	UCHAR         type;							// key type (see above)
	UCHAR         key[13];						// actual key
	DV_INDEX_KEY0 storyKey;						// story key
} DV_INDEX_KEY1;

typedef struct _DV_INDEX_REC {
	DV_INDEX_KEY0 storyKey;						// story key
	UCHAR         type;							// key type (see above)
	UCHAR         key[13];						// actual key
} DV_INDEX_REC;

//	storyKey is the primary key
// type+key is segmented with storyKey.
  
// these are the structures that describe the keys in the TEXT.DV file

typedef struct _DV_TEXT_KEY0 {
	UCHAR   product[2];							// DV product
	UCHAR   date[8];								// date of story
	UCHAR   sequence[4];							// sequence number of story
} DV_TEXT_KEY0;

typedef struct _DV_TEXT_KEY1 {
	UCHAR tempPerm;								// temporary/permanent indicator
} DV_TEXT_KEY1;

typedef struct _DV_TEXT_KEY2 {
	UCHAR         retention;					// retention indicator
	DV_TEXT_KEY0  storyKey;						// story key
} DV_TEXT_KEY2;

typedef struct _DV_TEXT_KEY3 {
	USHORT        hot;							// hot news indicator
	DV_TEXT_KEY0 storyKey;						// story key
} DV_TEXT_KEY3;

typedef struct _DV_TEXT_KEY4 {
	UCHAR         newsSource[4];				// news source
	DV_TEXT_KEY0 storyKey;						// story key
} DV_TEXT_KEY4;

typedef struct _DV_TEXT_REC {
	DV_TEXT_KEY0 storyKey;						// story key
	UCHAR         newsSource[4];				// news source
	UCHAR         origSource[4];
	UCHAR         tempPerm;						// temporary/permanent indicator
	UCHAR         retention;					// retention indicator
	UCHAR         displayDate[6];				// display date
	UCHAR         displayTime[4];
	UCHAR         accession[10];
	USHORT        headlineLength;
	USHORT  		  hot;
	UCHAR         reserved[4];
	UCHAR         text[MAX_DV_TEXT_SIZE];	// text (headline and story)
} DV_TEXT_REC;

// storyKey is the primary key
// tempPerm is a key
// retention, hot, and newsSource are segmented with storyKey.

/*=====================================================================*/

#endif
