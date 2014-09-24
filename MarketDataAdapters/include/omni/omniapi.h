#if !defined(__OMNIAPI_H)
#define __OMNIAPI_H
/*
    OMNIAPI.H - Definitions for the OMnet Application Programming Interface,
		V5.0
*/

/*
    Platform-specific definitions
*/

/*
    On Windows NT, VMS, VMS/TCP, HP-UX, AIX, Sun Solaris and Sun SunOS,
    simply compile
*/

/*  
	osf on ALPHA is 64-bit architecture.
 Passing zero to a pointer argument when no function
 prototype is visible results in the use of 4-yte zero insted
 of 6-byte zero. =L results in 8-byte zero. 
*/
#if defined( __osf__ )
#define W_NULL (void*)0L
#define NULL 0L
#else
#define W_NULL (void*)0
#endif

#define om_malloc malloc
#define om_free free      
#define om_strlen strlen
#define om_memcpy memcpy
#define om_sprintf sprintf

#ifndef TRUE
#define TRUE (1 == 1)
#endif
#ifndef FALSE
#define FALSE (0 == 1)
#endif

#define omniapi_read_event(et,rb,rl,em)	\
	omniapi_read_event_ext(et,rb,rl,em,(int32)0)

/*
    Important!

    The following two literals must not be greater than 64000 to
    be supported by the OMnet backbone.
*/
#define MAX_REQUEST_SIZE  ((int32) 64000)	/* request buffer size */
#define MAX_RESPONSE_SIZE ((int32) 64000)	/* response buffer size */

#define USE_GLOBAL_SOCKET	 ((int16  *)-1)
#define UNINITIALIZED_HANDLE	 ((int16)-1)

#define SRC_DEFAULT ((uint8) 16)		/* the default source for
						   PUT_MGMT calls */
#define READEV_OPTMSK_MANY 1			/* option for many events
						   from a _READ_EVENT call */
#define READEV_OPTMSK_SINGLE 0                  /* Option for one event */

#ifdef SWAP
#define PUTQUAD(y,x) swapint64_t(&y, &x)
#define PUTQUAD_OLD(y,x) y = swapint64(x)
#define PUTLONG(y,x) y = swapint32 (x)
#define PUTSHORT(y,x) y = swapint16 (x)
#define PUTBYTE(y,x) y = (x)
#else
#define PUTQUAD(y,x) y = (x)
#define PUTQUAD_OLD(y,x) y = (x)
#define PUTLONG(y,x) y = (x)
#define PUTSHORT(y,x) y = (x)
#define PUTBYTE(y,x) y = (x)
#endif

/*
    OMNIAPI type definitions
*/
typedef struct
  {					/* OMnet transaction message */
    uint32		length_u;	/* Number of bytes after USRCOD_X */
  } omni_message;

typedef struct
  {					/* OMnet logging message */
    uint32	length_u;		/* Number of bytes after RESRVD */
    uint8	resrvd_s[116];		/* Reserved by OM for OMnet use */
  } omni_log_message;

typedef struct
  {			            /* OMnet login message */
    uint32		type_u;		/* routing id (always 0 at login) */
    omni_username	user_s;		/* user idenfication */
    omni_password	pass_s;		/* password */
    omni_password	new_pass_s;	/* the new password */
    int8		node_s[129];/* node name or number */
    int8		sock_s[21];	/* socket name or number */
    int8		idnt_s[18];	/* program identification */
  } omni_login_message;

/*
    information object used for subscriptions
*/
typedef struct
  {
    uint16 infsrc_n;		/* information source */
    uint16 inftyp_n;		/* information type */
    struct
      {
	char central_module_c;
	char server_type_c;
	unsigned short transaction_number_n;
      } brdcst_x;		/* broadcast type, like 'B' 'I' 9 */
    union			/* attribute based on info type */
      {
	struct
	  {
	    uint32 no_use_u;	/* currently always set to one */
	  } general_x;		/* general: inftyp == 1 */
	struct
	  {
	    uint16 commodity_n;	/* OMex commodity code */
	    uint8 exp_year_c;	/* expiration year [binary 96 for 1996] */
	    uint8 exp_month_c;	/* expiration month [binary 1 for Jan] */
	  } derivative_x;	/* derivative: inftyp == 2 */
 	struct
	  {
	    uint32 commodity_u;	/* OMex commodity code */
	  } underlying_x;		/* underlying: inftyp == 3 */
	struct
	  {
	    uint16 no_use_n;		/* currently always set to one */
	    uint16 member_info_n;	/* member information, 0: all member broadcast */
					/*                     1: only user broadcast */
	  } dedicated_x;		/* dedicated: inftyp == 4 */
 	struct
	  {
	    uint32 dissemination_u;	/* OMex dissemination group */
	  } dissemination_x;		/* dissemination: inftyp == 5 */
      } attrib_x;		/* info type dependent attributes */
  } infobj_t;

typedef struct			/* authorised info object return from omniapi */
  {				/* read event ext with the SHOW_SUBSCR flag */
    infobj_t infobj_x;		/* information object */
    uint32 force_u;		/* force flag: 1: already subscr for by gwy */
				/*	       0: not subscribed for by gwy */
  } auth_infobj_t;

typedef struct 			/* returned when new subscription is added to the CDB through the API */
{						/* broadcast type is BN2 */ 
    int8 central_module_c;
    int8 server_type_c;
    uint16 broadcast_number_n;
	auth_infobj_t auth_infobj_x;
} new_subscr_t;

typedef struct			/* information object description for */
  {				/* omniapi set event */
    infobj_t infobj_x;		/* information object */
    uint32 handle_u;		/* subscription handle */
    int32 status_i;		/* subscription status */
  } subscr_item_t;

typedef struct			/* event received from omniapi read event ext */
  {				/* if subscriptions are used */
    uint32 handle_u;		/* subscription handle causing the event */
    uint16 evelen_n;		/* length of the event */
    /* event forthcoming: subscribed_event_t *foo; found at ( foo + 1 ) */
  } subscribed_event_t;

/*
    returned structure when asking for known subscriptions
    (omniapi_read_event_ext with the OMNI_EVTTYP_SHOW_SUBSCR option)
*/
typedef struct
  {
    uint32 objects_u;		/* number of information objects forthcoming */
    auth_infobj_t assobj_x[1];	/* first assigned info object in the list */
  } show_subscr_list_t;

typedef struct
  {
    int32 buflen_i;		/* len of the set event buffer (buflen incl) */
    subscr_item_t subitm_x[1];	/* first subscription item */
  } set_event_list_t;

/*
    Callable Routines:

    Facility types 0-15 are reserved.

    Source values in the range 0-31 are reserved.
*/

#ifdef __cplusplus
extern "C" 
{
#endif

int32 omniapi_tx 
	(
	 int32  *,			/* transaction status, by ref */
	 uint32,			/* facility type, by value */
	 omni_message  **,	        /* tx buffers, pointer vector by ref */
	 uint32	 *,		        /* transaction ID, by ref */
	 uint32	 *);		        /* order ID, by ref */

int32 omniapi_query (
	int32  *,			/* transaction status, by ref */
	uint32,				/* facility type, by value */
	omni_message  *,		/* send buffer, by ref */
	int8,				/* return buffer flag, by value */
	int8 *,		                /* receive buffer, by ref */
	uint32  *,			/* receive buf length, by ref */
	uint32  *,			/* transaction ID, by ref */
	uint32  *);		        /* order ID, by ref */

int32 omniapi_set_event (
	uint32,				/* event type, by value */
	int8  *);			/* reserved by OM */

int32 omniapi_clear_event (
	uint32,				/* event type, by value */
	int8  *);			/* reserved by OM */

int32 omniapi_read_event_ext (
	uint32,				/* event type, by value */
	int8 *,		                /* receive buffer, by reference */
	uint32  *,			/* receive length, by reference */
	int32  *,			/* event mask, by reference */
	int32);				/* option mask, by value */

int32 omniapi_put_mgmt (
	uint8,				/* message entity, by value */
	uint8,				/* message severity, by value */
	uint16,				/* entity type, by value */
	uint8,				/* message source, by value */
	omni_log_message  *);	        /* log message, by reference */

void omniapi_cvt_string (
	int8,				/* TRUE=TO, FALSE=FROM ISO format */
	uint8  *);			/* string to be converted */

int32 omniapi_get_message (
	int32,				/* message code, by value */
	int8  *,			/* message string, by reference */
	uint32  *,			/* messge length, by reference */
	int32);				/* simple message string, bool by val */


int32 omniapi_get_info (
	int32  *,			/* reason, by reference */
	uint32,				/* information type, by value */
	uint32  *,			/* buffer length, by reference */
	void  *);			/* information buffer, by reference */

#ifdef SWAP
void omniapi_cvt_int (
	void  *,			/* integer to be converted to/from
		  		 	   little-endian, by reference */
	int16);				/* size (in bytes) of integer */

void swapint64_t (void *outVal, void *inVal);
uint64 swapint64 (uint64 intval);
uint32 swapint32 (uint32 intval);
uint16 swapint16 (uint16 intval);
#endif

#define omniapi_session_handle  void*   /* to ease the readability in code */

int32 omniapi_tx_ex (
        omniapi_session_handle,         /* Handle to session */
        int32 *,			/* transaction status, by ref */
        uint32,                         /* facility type, by value */
        omni_message **,	        /* tx buffers, pointer vector by ref */
        uint32	*,		        /* transaction ID, by ref */
        uint32	*);		        /* order ID, by ref */

int32 omniapi_query_ex (
        omniapi_session_handle,         /* Handle to session */
        int32 *,			/* transaction status, by ref */
	uint32,				/* facility type, by value */
	omni_message *,	        	/* send buffer, by ref */
	int8,				/* return buffer flag, by value */
	int8 *,		                /* receive buffer, by ref */
	uint32 *,			/* receive buf length, by ref */
	uint32 *,			/* transaction ID, by ref */
	uint32 *);	        	/* order ID, by ref */

int32 omniapi_set_event_ex (
        omniapi_session_handle,         /* Handle to session */
        uint32,				/* event type, by value */
	int8 *);			/* reserved by OM */

int32 omniapi_clear_event_ex (
        omniapi_session_handle,         /* Handle to session */
        uint32,				/* event type, by value */
	int8 *);			/* reserved by OM */

int32 omniapi_read_event_ext_ex (
        omniapi_session_handle,         /* Handle to session */
        uint32,				/* event type, by value */
	int8 *,   		        /* receive buffer, by reference */
	uint32 *,			/* receive length, by reference */
	int32 *,			/* event mask, by reference */
	int32);				/* option mask, by value */

int32 omniapi_put_mgmt_ex (
        omniapi_session_handle,         /* Handle to session */
        uint8,				/* message entity, by value */
	uint8,				/* message severity, by value */
	uint16,				/* entity type, by value */
	uint8,				/* message source, by value */
	omni_log_message *);	        /* log message, by reference */

int32 omniapi_get_message_ex (
        omniapi_session_handle,         /* Handle to session */
        int32,				/* message code, by value */
	int8 *,		        	/* message string, by reference */
	uint32 *,			/* messge length, by reference */
	int32);				/* simple message string, bool by val */

int32 omniapi_get_info_ex (
        omniapi_session_handle,         /* Handle to session */
        int32 *,			/* reason, by reference */
	uint32,				/* information type, by value */
	uint32 *,			/* buffer length, by reference */
	void *);			/* information buffer, by reference */

/* new functions */
omniapi_session_handle omniapi_create_session();
void    omniapi_close_session( 
        omniapi_session_handle          /* Handle to session */
        );

#ifdef __cplusplus
}
#endif

/*
    Error Codes
*/

/*
    module OAPI
*/
#define OMNIAPI_ALLEVTS		 4	/* All events collected */
#define OMNIAPI_OVERFLOW 	 3	/* Event message buffer overflow */
#define OMNIAPI_ALR_CLR		 2	/* Event type already cleared */
#define OMNIAPI_ALR_SET		 1	/* Event type already set */
#define OMNIAPI_NORMAL		 0	/* Normal completion */
#define OMNIAPI_FAILURE		-1	/* Failure completion */
#define OMNIAPI_NOT_READY	-2	/* OAPI not ready */
#define OMNIAPI_FACID_NOT_VALID	-3	/* Facility Id not in range */
#define OMNIAPI_INVALID_TABLE	-4	/* Invalid table type */
#define OMNIAPI_NOT_LOGGED_IN	-5	/* OAPI not logged in */
#define OMNIAPI_LOCKED		-6	/* OAPI locked */
#define OMNIAPI_ALREADY_LOCKED	-7	/* OAPI already locked */
#define OMNIAPI_NET_INIT_FAIL	-8	/* OAPI network initialization failed */
#define OMNIAPI_NOT_INITIALIZED	-9	/* OAPI not initialized */
#define OMNIAPI_NO_INFO_RCVD -10	/* No network information received */
#define OMNIAPI_NORTR		-11	/* RTR connection failed */
#define OMNIAPI_TX_ABORTED	-12	/* Transaction aborted */
#define OMNIAPI_TRUNCATED	-13	/* Data truncated */
#define OMNIAPI_CNV_NO_RANGE	-14	/* Table conversion range error */
#define OMNIAPI_CNV_NOT_SORTED	-15	/* Table conversion sort error */
#define OMNIAPI_CNV_OFFS_ERROR	-16	/* Table conversion offset error */
#define OMNIAPI_NO_SUCH_ID	-17	/* Invalid transaction type */
#define OMNIAPI_VER_FIELD_ERROR	-18	/* Transaction verification error */
#define OMNIAPI_VER_INT_ERROR	-19	/* Message verific. internal error */
#define OMNIAPI_VER_TABLE_ERROR	-20	/* Table verification error */
#define OMNIAPI_TX_TIMEOUT	-21	/* Transaction timeout */
#define OMNIAPI_TX_DECLFAIL	-22	/* Transaction server/requester */
					/* declaration failure */
#define OMNIAPI_TX_FAILURE	-23	/* Transaction failure */
#define OMNIAPI_DYNMEM		-24	/* Error obtaining dynamic memory */
#define OMNIAPI_INVARG		-25	/* Invalid argument */
#define OMNIAPI_NOT_FOUND	-26	/* Requested data not found */
#define OMNIAPI_ITV_ERROR	-27	/* Information table verification error */
#define OMNIAPI_NO_USR_OR_PASSW	-28	/* Username and/or Password missing */
#define OMNIAPI_NO_NET_PATH	-29	/* Net Path data missing */
#define OMNIAPI_INVEVT		-30	/* Invalid event type */
#define OMNIAPI_NOT_LOCKED	-31	/* OAPI not locked */
#define OMNIAPI_INVTXTYPE	-32	/* Invalid transaction type */
#define OMNIAPI_NOFACILITY	-33	/* OMnet facility not found */
#define OMNIAPI_HIGHRTRCHN	-34	/* Expected RTR channel # exceeded */
#define OMNIAPI_FATAL		-35	/* Fatal OAPI error */
#define OMNIAPI_NOORDERID	-36	/* No order id */
#define OMNIAPI_APINORSP        -37     /* Api not responding, GBLSEC reinitialized */
#define OMNIAPI_APIDISCON       -38     /* Api disconnected, GBLSEC reinitialized   */
#define OMNIAPI_NOTAUTH		-39     /* Subscription not authorized, or failed   */
#define OMNIAPI_PASSW_EXPIRED	-40     /* The password has expired */
#define OMNIAPI_INVALID_PASSW   -41     /* The password is invalid (e.g. to short or not unique in relation to previous passwords */

/*
    module OMNIAPI
*/
#define OMNIAPI_RECONNECT 2000		/* successful reconnection */
#define OMNIAPI_SUCCESS 0		/* normal OMNIAPI completion */
#define OMNIAPI_PROBLEM -2000		/* error in OMNIAPI internal call */
#define OMNIAPI_INTFAILURE -2001	/* internal OMNIAPI error */
#define OMNIAPI_BADNARGS -2003		/* bad number of arguments */
#define OMNIAPI_BADARGVAL -2004		/* bad argument value */
#define OMNIAPI_NONETWORK -2005		/* no network is present for IPC */
#define OMNIAPI_OSBADCONFIG -2006	/* O/S incorrectly configured */
#define OMNIAPI_OAPI -2007		/* problem in the internal OM API */
#define OMNIAPI_NOTCONNECTED -2008	/* invalid operation before LOGIN */
#define OMNIAPI_NOGWYSRV -2009		/* no gateway service defined */
#define OMNIAPI_BADHOSTNAME -2010	/* host name could not be translated */
#define OMNIAPI_ERRSOCKET -2011		/* socket could not be allocated */
#define OMNIAPI_ERRCONNECT -2012	/* system error occured on 'connect' */
#define OMNIAPI_ERRBIND -2013		/* system error occured on 'bind' */
#define OMNIAPI_NOSESSION -2014		/* session is aborted */
#define OMNIAPI_ERRSEND -2015		/* error on send() */
#define OMNIAPI_ERRMEM -2016		/* error on malloc() */
#define OMNIAPI_APIOLD -2017		/* OMNIAPI too old for the gateway */
#define OMNIAPI_RECONREJ -2018		/* reconnection rejected */
#define OMNIAPI_SESINUSE -2019		/* gateway API session already in use */

#endif
