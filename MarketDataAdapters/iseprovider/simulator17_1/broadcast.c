/*
Copyright © 1999 International Securities Exchange LLC ("ISE").
All Rights Reserved.

USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
THE LICENSE GRANTED TO THE RECIPIENT UNDER THE "DEVELOPERS AGREEMENT".
REDISTRIBUTION OF THIS SOFTWARE IS PROHIBITED.

THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT A WARRANTY OF ANY KIND.
ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES,
INCLUDING ANY EXPRESS OR IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT OF INTELLECTUAL PROPERTY
RIGHTS, ARE HEREBY EXCLUDED.  ISE AND ITS LICENSORS SHALL NOT BE LIABLE
FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR
DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES.  IN NO EVENT WILL ISE OR
ITS LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR
DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES,
HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING
OUT OF THE USE OF OR INABILITY TO USE THE SOFTWARE, EVEN IF ISE HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.


Title : broadcast.c

Function :

    This file provides broadcast handling to the ISESim.exe program.
    
Author:  Neil Cossor

Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
990908	v0.01		    update for change in log_bcast.. don't require click_px param
990914			    update for synchronised update for BI204 (underlying update)
			    add some processing for bi114
990923			    update for synchronised update for BI210 (instrument class update)
991005	v0.02		    fix bug in handle_bcast_BI2.. used (random) text_buffer as format string
991008			    make bcast_io_thread call om_disconnect (if logged on), if bcast
				thread exiting because of bad status
991013			    add call to point_series_at_underlying with bi202 broadcast
991103	v0.03		    add debug reporting of broadcast details, as next step on 'smarter'
				broadcast subscription
			    remove call to clear_subscriptions_for_all_event_types in
				subscribe_for_events..
991104			    mod BN1 reporting to report date/time on screen
991111			    add control for overall broadcasts on/off
991115	v0.04		    replace BI101 with BI8 (new OM docs) -> handle_bcast_BI8, not handle_bcast_BI101
991122	v0.05		    continue enhancements for selective subscriptions
991130			    mod init_bcast_io_thread to not create the thread unless the sync startup is OK
			    increase delay between bcast polls (with no data)
			    add mutex locks in subscribe_to_one_bcast_event, subscribe_to_all_bcast_events
			    to avoid clashes between command / script driven omniapi calls adding / cancelling
			    subscriptions, and the normal broadcast loop.
991204			    add subscr_list_bin, and update subscribe_to_one_bcast_event to call it for %bin
991205			    fix bug in find_subscription.. if no match, was returning last pointer val, not NULL
991206			    improve error reporting in subscribe_to_one_bcast_event if no default bin
			    enhance reporting of BO115 locked orders
000105	v0.06		    update for latest OM .h files (15.7 OM release)
			    enhance bo105, new nasdaq fields, and struct name change
			    enhance bo106, extra broker-percentage field for facilitation orders
			    add new bd111, deal in market bcast
			    bd103 is just name change for struct
000110			    fix handle_bcast_bo103, displayed incorrect best bid/ask exchange names
			    change handle_bcast_BI2 to display country names
			    pull some of the argc/argv list handling for underlying out, for reuse
000113			    add #include for mm.h
			    upgrade stuff in infobj with new info in 15.7
000122	v0.07		    mod to be source-portable between NT and Solaris.. thanks Joshua P.
			    fix couple of string reporting bugs for hidden fields
000123			    fix bug in subscribe_to_one_bcast_event, which did not pack commodity code
			    into underlying infotype, with specific underlying selected
000204	v0.08		    move include of process.h into stdincl, only if WinNT
				mod init_bcast_io_thread to call ISE_THREAD_CREATE
000327	v0.09		    mod process_one_bcast to ignore unknown broadcasts, and to report 'unprintable'
				central modules and server type chars in hex.
000328			    update handle_bcast_BO107 to zot (delete) quotes from in-memory info
			    update handle_bcast_BO110, handle_bcast_BO109 to update in-memory price / qty
				from step-up, tick-worse
000404			    update handle_bcast_BO107 to zot orders from in-memory info.
				This required adding a dependency on order.h
000407			    add some debug code (currently commented out) to check value of handle
				on actual bcast. A better check would be to search the list of
				actual bcasts we have, and ensure that every bcast has a handle
				value which matches a subscription we issued (keeping in mind
				that when we are cancelling a subscription, we might get an extra
				or two at the cancel boundary).
000413	v0.10		    update handle_bcast_BI132 for bcasts about unknown underlying
000422	v1.00		    update to match OM release 15.7-3 (move of some fields from da203 to da210).
				update handle_bcast_BD101, handle_bcast_BD111, handle_bcast_BI103,
				handle_bcast_BO101, handle_bcast_BO102, handle_bcast_BO103,
				handle_bcast_BO106, handle_bcast_BO110, handle_bcast_BO115
000426	v1.01		    fix handle_bcast_BO106.. used logical && not bitwise & in checking expose flag
000502	v1.02		    enhance reporting in handle_bcast_BD102 and handle_bcast_BO104
000503			    hack read_bcast_stream to cater for mixture of forced broadcasts
				with subscribed (by list) broadcasts
000505			    update handle_bcast_BI2 to store the new status, and turn
				off autoquoting if change is -> closed
			    fix bug in display_subscriptions.. did not lock Q of authorized bcasts
				which could crash if session terminated during display
000509			    enhance handle_bcast_BO110 to report qty, reason, tick level
000515			    mod handle_bcast_BO101 - put bid info before ask (normal industry standard)
000521			    same deal with handle_bcast_BO102 (bid before ask)
000530			    hack bd111 to produce logfile of ISE trades vs away BBOs
			    and bd102 to produce logfile of away trades vs ISE BBO
000601			    mod open_tradefile to include prod/test and business date into trade filename
			    mod handling of tradefile records (bd111 and bd102) to indicate if
				ISE better / match / worse
000606			    mod bd111 and bd102 handling to be easier to import into excel, database etc
000619			    add checks in bcast_io_thread, for time around loop, and time for bcast read
001219	v1.03		    added module variable for delay between bcast poll,
			    and routine to set it (bcast_set_poll_delay)
000721	v1.03		    mod input params to subscribe_to_one_bcast_event, to take the prebuilt
			    list of commodity codes
0012xx	v2.00		    start updates for OM release 16.3 (from 15.7-3)
			    remove BI2 message (superceded by BI41)
			    add BI41 message
			    several fields disappered in BO108
			    fix refs to order_category (8->16 bit)
			    various qty changes (16->32 bit)
010305	v2.01		    put additional "while logged on" check in read_bcast_stream, attempting
			    to narrow down a window of lots of bcasts, while logging off.
010307			    enhance BI103 to handle bcast item which is by underyling, not series
			    this (apparently) could occur in 15-7, as well as 16.3
010605			    update for new BO130 member OB bcast, and BO131 (public)and BO132
				(member) combo bcasts
010618			    update handle_bcast_BO110 (now called Quote update not Tick-Worse)
				and report tick reason
011017	v2.02		    update bcast_io_thread to be 'smarter' about warnings on long time
				for this poll, calculating time (by system) between polls etc
			    Added static vars to hold the warning limits, and new routines
				which allow the values to be set by the user,
				from defaults of 1000 milliseconds.

To do:
------

- add some heartbeat monitoring code onto the bi9... this could
    be handled by having BI9 note that latest heartbeat time,
    another thread which checks periodically for last heartbeat vs
    the current time..

- add routines which cancel existing subscriptions.

- add specific processing for lots of the rest of the broadcasts...
    things like a bo107 (order/quote deleted) which interact with
    the local orderbook

Notes on this module
--------------------

    Broadcasts from the OM system are queued for API users, based upon
    subscriptions requested by that API user, and a list of 'mandatory'
    broadcasts for that particular user (set up in the CDB).

    Broadcasts may be subscribed to an an ad-hoc basis...
    finer selections include (combinations of)
    - particular commodity
    - particular expiration date
    - particular central module, server, and transaction type

    Each subscription has qualifying selectors of commodity and expiry date (yy and mm NOT dd).
    Obviously, some bcasts don't have an expiry date, like underlying trades,
    and some bcasts don't have an underlying, like market open/closed

    For the present time, I will implement selective bcasts based on underlying (where relevant)
    but NOT do selective bcasts using expiry date.

*/
#include "stdincl.h"

#include "broadcast.h"
#include "combos.h"
#include "connect.h"
#include "error.h"
#include "instrument.h"
#include "logger.h"
#include "misc.h"
#include "mm.h"
#include "order.h"
#include "series.h"
#include "trades.h"
#include "underlying.h"
#include "utility.h"
#include "autoqute.h"


// ------------------- module private types, vars --------------------------

#define CANCEL_ALL_SUBSCRIPTIONS	    0
#define GENERAL_BROADCAST_EVENT_TYPE	    1
#define EVENT_LIST_BUFFER_LEN		    2048
// the information type values for broadcasts
#define INFO_TYPE_GENERAL		    1
#define INFO_TYPE_DERIVATIVE		    2
#define	INFO_TYPE_UNDERLYING		    3
#define	INFO_TYPE_DIRECTED		    4
#define	INFO_TYPE_DISSEMINATION_GROUP	    5
// the strings we report these information types with
#define INFO_TYPE_GENERAL_STRING	    "General"
#define INFO_TYPE_DERIVATIVE_STRING	    "Derivative Specific"
#define INFO_TYPE_UNDERLYING_STRING	    "Underlying"
#define INFO_TYPE_DIRECTED_STRING	    "Directed"
#define INFO_TYPE_DISSEMINATION_GROUP_STRING "Dissemination Group"

/*
    information object used for subscriptions - this is in local-endian order.
    Related to infobj_t, in omniapi.h - updated with 15.7
*/
typedef struct
{
    uint16 infsrc_u;		// information source - ?? is this an exchange code (1 byte),
				// or the exchange mnemonic.. but that should be text
    uint16 inftyp_u;		// information type - there are only 4 values, so 1 byte would be enuff

    // the broadcast - contains central module, server type, and trans #.. like 'B' 'I' 9
    broadcast_type_t		broadcast_type_x;

    union			// attribute based on info type
    {
	struct			// general inftyp
	{
	    uint32 no_use_u;	// currently always set to one
	} general_x;		// underlying: inftyp == 1

	struct			// derivative inftyp
	{
	    uint16 commodity_u;	// OMex commodity code
	    uint8 exp_year_u;	// expiration year [binary 96 for 1996]
	    uint8 exp_month_u;	// expiration month [binary 1 for Jan]
	} derivative_x;		// underlying: inftyp == 2

	struct
	{
	    uint32 commodity_u;	// OMex commodity code
	} underlying_x;		// underlying: inftyp == 3

	struct
	{
	    uint16 no_use_n;		// currently always set to one
	    uint16 member_info_n;	// member information, 0: all member broadcast
					//                     1: only user broadcast
	} dedicated_x;		// dedicated: inftyp == 4

 	struct
	  {
	    uint32 dissemination_u;	// OMex dissemination group # - values configured in central system
	  } dissemination_x;		// dissemination: inftyp == 5

    } attrib_x;			// info type dependent attributes
} ise_infobj_xt;

/* info about one subscription(event).. this has multibyte numbers in local format
    This is a combination of the authorised transactions (collected automagically at ISEsim logon),
    and the user (manual or scripted) selection of interest in particular broadcasts.
*/
typedef struct {
    ise_infobj_xt	ise_infobj_x;		// information object
						// note that this contains the broadcast type e.g. 'b' 'd' 101
    uint8		force_u;		// force flag, 1 == already subscr for by gwy, not optional
    /* if the broadcast is optional (i.e. force_u is 0), we may have some subscriptions..
	this is a list of underlying. I store this as an arry of underlying (commodity codes), and the count of active members.
	First, we have the count of entries in the commodity_code list
	If this is 0, then we have NOT subcribed to this broadcast.
	If this is 1, and the commodity_code[0] is 0, we are subscribed for all underlying.
    */
    uint16		underlying_count_u;
    // and the entries. If an entry with a commodity code of 0 is present, then we are subscribed to all
    uint16		commodity_code_u [MAX_UNDERLYING_PER_LIST];
    // corresponding 1:1 with the commodity codes, are the subscription handles.. needed to cancel
    uint32		handle_u [MAX_UNDERLYING_PER_LIST];
    // if it is a dedicated subscription, this indicates if we subscribed by user or by member
    unsigned		user_member_u;
} one_subscription_xt;

// some 'selectors' used in bcast on|off commands
    static char *bcast_selectors_s [] =
    {
	"*",	    // all underlying
	"%BIN",	    // my default bin (auto set for MM, can be manually set for EAM)
	"%FOLIO"    // a portfolio previously specified
    };
    // enumerated list, corresponding 1:1 (and in same order) as string list
    enum bcast_selectors_et { bcast_under_all_e, bcast_under_bin_e, bcast_under_folio_e };




// NB vars declared at this level MUST BE EXPICITLY DECLARED static, and initialisation values are
//  set before any of the fn's may be called

/* overall flag, which if set (1) kills all broadcasts.
    This means that upon logon, we don't subscribe to any broadcasts
    and if we had any subscriptions open, we cancel them.
    Note, however, some broadcasts are NOT optional, and will
    arrive anyway (e.g. BN1).
    Also note, that we MUST poll for broadcasts (in session) anyway,
    we must satisfy the OMnet heartbeat requirements.
    This requires that we do "something" within the heartbeat period
    (which is of the order of seconds), so the gateway knows our
    application hasn't fallen asleep / fallen over.
    If we have a genuine transaction to do, then all is sweet,
    however if there are no queries / transactions to do, we must still
    do something. A 'fetch broadcasts' is adequate, and useful anyway,
    so this is our "I'm awake" transaction. With this (and compulsory
    broadcasts) in mind, it is clear that even if the application
    specified that it does NOT want any broadcasts, that we should
    poll frequently.
*/
static unsigned kill_broadcasts_u = FALSE;

    /* the list of bcasts - a hybrid of the authorised, and the ones actually subscribed to.
	Cleared at each logoff, added to by calls
	to subscribe_to_one_bcast_event, and subscribe_to_all_bcast_events.
	For each entry in this list, we have a current subscription.
	Each entry in the list is for one particular broadcast type, e.g. BD101.
	That particular entry itself contains a list, which determines what particular
	underlying we have subscribed for.
	Common combinations are:-
	    0 = wildcard (all). This could be used for things not related to underlying
		(like market status), or where we subscribe to all (e.g. underlying status)
	    one underlying
	    a list of specific underlying which are all of the bin
	    a list of specific underlying, which are a subset of the bin
	    a list of specific underlying, which have nothing to do with bins (e.g. if we are an EAM)
    */ 
static node_header_xt authorised_bcasts_q;

// file for logging trades to
static FILE *tradefile_px = NULL;

// our nominated bcast poll delay, in milliseconds..the preferred interval between polls
static uint32 bcast_poll_delay_u = 50;
// what is the acceptable time taken (milliseconds) for a single poll, before we place a warning in the log
static uint32 bcast_poll_limit_u = 1000;
// what is the acceptable limit (add be local system) into our loop (time-slicing etc)
// before we place a warning in the log.
static uint32 system_poll_limit_u = 1000;

// ------------------- internal routines -----------------------------------

char * bcast_type_to_string ( broadcast_type_t bcast_type_x )
/*
Function : bcast_type_to_string
Author	 : Neil Cossor
Modified : 991126
Description:

    We have a broadcast type, which we need to spit out in readable form.

    As we know there is a single char for each of central module and server,
    and an uint32.. we know the max size of the string resulting.

    Note that we assume the 2-byte transaction number is in local-endian form.

    ??? How do we nicely do this string formatting..?
	we need to allocate some memory for the string (when do we return it)
	or use a static buffer, which we return the address of (possible contention clashes)
	or be given an address to write into (more cumbersome for caller)

    Use solution #2 for now.. local static buffer
*/
{
    // allow for max size, and a terminating NUL
    static char bcast_type_s [ 8 ];

    sprintf ( (char *)&bcast_type_s, "%c%c%u",
		bcast_type_x.central_module_c,
		bcast_type_x.server_type_c,
		bcast_type_x.transaction_number_n
	    );
    return (char *)&bcast_type_s;
}   // bcast_type_to_string

// ---------------------------------------------------------------------------

void rx_infobj ( one_subscription_xt *ise_subscr_data_px,
		    infobj_t *om_infobj_px,
		    uint32 force_u )
/*
Function : rx_infobj
Author	 : Neil Cossor
Modified : 20000111
Description:

    routine which transfers all the fields from an OM format authorised event
    into the ISEsim internal format, including the necessary multibyte field swaps.

    We also store the 'force' flag, which arrived with the infoobj.

    We never subscribe to a forced bcast (??).. if the user asks for it, we just
    skip sending the request...

Input params:

    pointer to the target (ISEsim format) data struct
    pointer to the source (OM format) data struct
    the OMnet force-flag, in central-site byte order
*/
{
    // temp
    uint32  ise_force_u;

    PUTLONG ( ise_force_u, force_u );

    // just grab the low byte..do consistency check at end
    ise_subscr_data_px->force_u = ise_force_u;

    PUTSHORT ( ise_subscr_data_px->ise_infobj_x.infsrc_u, om_infobj_px->infsrc_n );
    PUTSHORT ( ise_subscr_data_px->ise_infobj_x.inftyp_u, om_infobj_px->inftyp_n );
    ise_subscr_data_px->ise_infobj_x.broadcast_type_x.central_module_c = om_infobj_px->brdcst_x.central_module_c;
    ise_subscr_data_px->ise_infobj_x.broadcast_type_x.server_type_c = om_infobj_px->brdcst_x.server_type_c;
    PUTSHORT ( ise_subscr_data_px->ise_infobj_x.broadcast_type_x.transaction_number_n,
		om_infobj_px->brdcst_x.transaction_number_n );
    // there are 4 bytes of attribute. The format of this varies with the info-type.
    switch ( ise_subscr_data_px->ise_infobj_x.inftyp_u )
    {
	case INFO_TYPE_GENERAL :
	    // where we know the format (4 bytes) but not the relevance,
	    // and all cases where we don't know the format .. assume 4 byte # for now
	    PUTLONG ( ise_subscr_data_px->ise_infobj_x.attrib_x.general_x.no_use_u,
			om_infobj_px->attrib_x.general_x.no_use_u );
	    break;

	case INFO_TYPE_DERIVATIVE :
	    PUTSHORT ( ise_subscr_data_px->ise_infobj_x.attrib_x.derivative_x.commodity_u,
			om_infobj_px->attrib_x.derivative_x.commodity_n );
	    ise_subscr_data_px->ise_infobj_x.attrib_x.derivative_x.exp_year_u =
			om_infobj_px->attrib_x.derivative_x.exp_year_c;
	    ise_subscr_data_px->ise_infobj_x.attrib_x.derivative_x.exp_month_u =
			om_infobj_px->attrib_x.derivative_x.exp_month_c;
	    break;

	case INFO_TYPE_UNDERLYING :
	    PUTLONG ( ise_subscr_data_px->ise_infobj_x.attrib_x.underlying_x.commodity_u,
			om_infobj_px->attrib_x.underlying_x.commodity_u );
	    break;

	case INFO_TYPE_DIRECTED :
	    PUTSHORT ( ise_subscr_data_px->ise_infobj_x.attrib_x.dedicated_x.member_info_n,
			om_infobj_px->attrib_x.dedicated_x.member_info_n );
	    PUTSHORT ( ise_subscr_data_px->ise_infobj_x.attrib_x.dedicated_x.no_use_n,
			om_infobj_px->attrib_x.dedicated_x.no_use_n );
	    break;

	case INFO_TYPE_DISSEMINATION_GROUP :
	    PUTLONG ( ise_subscr_data_px->ise_infobj_x.attrib_x.dissemination_x.dissemination_u,
			om_infobj_px->attrib_x.dissemination_x.dissemination_u );
	    break;

	default :
	    printf ("rx_infobj with Unknown infotype %u, in %s\n",
			ise_subscr_data_px->ise_infobj_x.inftyp_u,
			bcast_type_to_string ( ise_subscr_data_px->ise_infobj_x.broadcast_type_x )
		    );
	    break;
    }   // switch 



    // consistency checks - report anything suspect
    if ( ise_force_u > 1 )
	// suspect 'force' flag for this bcast - report it
	printf ("Odd force flag value %u, with broadcast \n",
		    ise_force_u );
}   // rx_infobj

// ---------------------------------------------------------------------------

void tx_infobj ( infobj_t *om_infobj_px,
		    one_subscription_xt *ise_subscr_data_px )
/*
Function : tx_infobj
Author	 : Neil Cossor
Modified : 20000123
Description:

    routine which transfers the fields from an ISE internal format about one subscription
    into the OM format infobj_t, including the necessary multibyte field swaps.

Input params:

    pointer to the target (OM format) data struct
    pointer to the source (ISEsim format) data struct
*/
{
    PUTSHORT ( om_infobj_px->infsrc_n, ise_subscr_data_px->ise_infobj_x.infsrc_u );
    PUTSHORT ( om_infobj_px->inftyp_n, ise_subscr_data_px->ise_infobj_x.inftyp_u );
    om_infobj_px->brdcst_x.central_module_c = ise_subscr_data_px->ise_infobj_x.broadcast_type_x.central_module_c;
    om_infobj_px->brdcst_x.server_type_c = ise_subscr_data_px->ise_infobj_x.broadcast_type_x.server_type_c;
    PUTSHORT ( om_infobj_px->brdcst_x.transaction_number_n,
		ise_subscr_data_px->ise_infobj_x.broadcast_type_x.transaction_number_n );
    /* there are 4 bytes of attribute. The format of this varies with the info-type.
	We do not, at present, have the correct breakdown for all forms of the attribute..
	only General and Derivative.
	For the ones we don't know (and hence don't know the 'endian' order of the components,
	just copy all 4 bytes.
    */
    switch ( ise_subscr_data_px->ise_infobj_x.inftyp_u )
    {
	case INFO_TYPE_GENERAL :
	    // we know the format (4 bytes) but not the relevance,
	    // and all cases where we don't know the format .. assume 4 byte # for now
	    PUTLONG ( om_infobj_px->attrib_x.general_x.no_use_u,
			ise_subscr_data_px->ise_infobj_x.attrib_x.general_x.no_use_u
		    );
	    break;

	case INFO_TYPE_DERIVATIVE :
	    // NB if all underlying are authorised, and we are subscribing to one,
	    // we override the commodity (0) we put in here, in the calling code
	    PUTSHORT ( om_infobj_px->attrib_x.derivative_x.commodity_n,
			ise_subscr_data_px->ise_infobj_x.attrib_x.derivative_x.commodity_u
			 );
	    om_infobj_px->attrib_x.derivative_x.exp_year_c =
			ise_subscr_data_px->ise_infobj_x.attrib_x.derivative_x.exp_year_u;
	    om_infobj_px->attrib_x.derivative_x.exp_month_c =
			ise_subscr_data_px->ise_infobj_x.attrib_x.derivative_x.exp_month_u;
	    break;

	case INFO_TYPE_UNDERLYING :
	    // NB if all underlying are authorised, and we are subscribing to one,
	    // we override the commodity (0) we put in here, in the calling code
	    PUTLONG ( om_infobj_px->attrib_x.underlying_x.commodity_u,
			ise_subscr_data_px->ise_infobj_x.attrib_x.underlying_x.commodity_u );
	    break;

	case INFO_TYPE_DIRECTED :
	    PUTSHORT ( om_infobj_px->attrib_x.dedicated_x.member_info_n,
			ise_subscr_data_px->ise_infobj_x.attrib_x.dedicated_x.member_info_n );
	    PUTSHORT ( om_infobj_px->attrib_x.dedicated_x.no_use_n,
			ise_subscr_data_px->ise_infobj_x.attrib_x.dedicated_x.no_use_n );
	    break;

	case INFO_TYPE_DISSEMINATION_GROUP :
	    PUTLONG ( om_infobj_px->attrib_x.dissemination_x.dissemination_u ,
			ise_subscr_data_px->ise_infobj_x.attrib_x.dissemination_x.dissemination_u,
			);
	    break;

	default :
	    printf ("tx_infobj with Unknown infotype %u, in %s\n",
			ise_subscr_data_px->ise_infobj_x.inftyp_u,
			bcast_type_to_string ( ise_subscr_data_px->ise_infobj_x.broadcast_type_x )
		    );
	    break;
    }   // switch 

}   // tx_infobj

// ---------------------------------------------------------------------------

one_subscription_xt * find_subscription ( broadcast_type_t bcast_type_x,
					    uint16 commodity_code_u )
/*
Function : find_subscription
Author	 : Neil Cossor
Modified : 20000113
Description:

    We need to find the (ISEsim) data about one subscription.
    This is a search for a matching broadcast type.
    This might be done if we are about to subscribe to something, or cancel a subscription.

    If we find a match, we return the pointer to the data structure.
    If we don't, we return a NULL

    Note that there are some bcast infobj which have specific commodity codes, e.g. bo102.
    If it is one of those, then we should find the correct infobj to update..

    To this end, if a subscription is found which might be commodity based (underlying
    and derivative), then we look for a match..note, however that because an infobj MIGHT
    have a commodity code, it doesn't mean that it will. So.. if type is deriv/under,
    and the commodity_code of the infobj is non-zero, look for exact match, unless the caller
    specified a search value of 0. If the infobj commodity code of the infobj is 0 or the
    caller supplied a 0, then we match any supplied commodity. The infobj having a non-zero
    and us being called with a 0 could be if the caller is merely identifying if we are
    authorised for a broadcast type, and what the info type for that bcast is, preparatory
    to building up an underlying list.

    For example, we may have
    bo102, type under, with commodity 34
    bo102, type under, with commodity 402
    bo102, type under, with commodity 710
    bi132, type under, with commodity 0

    If we were looking for a bo102, we would look for exact commodity match
    if we were looking for a bi132, we only find a 0 commodity code, so ignore the supplied commodity code

    I am, in doing this, assuming that we won't get mixed bcast types.. e.g. with the same
    set above that the following will NOT be supplied by OM.. or things might blow up
    bo102, type under, with commodity 0
    bi132, type under, with commodity <n> (n being non-zero).
*/
{
    // pointer to the actual data we know (in ISEsim format) about one subscription (event)
    one_subscription_xt	    *ise_subscr_data_px = NULL;
    // pointer to q'able node, which contains pointer to the data about one subscription
    gen_buf_node_xt	    *ise_subscr_node_px;

    // whiz down the known subscription list, looking for a match.
    // NB, this list will be small (say 12 for an EAM, maybe 25 for a PMM)
    ise_subscr_node_px = (gen_buf_node_xt *)authorised_bcasts_q.chain.next;
    while ( ise_subscr_node_px )
    {
	// break out if we find a match.. the forward block of the last block will be NULL
	ise_subscr_data_px = (one_subscription_xt *)ise_subscr_node_px->msg_p;

	// correct broadcast type ?
	if (( ise_subscr_data_px->ise_infobj_x.broadcast_type_x.central_module_c == bcast_type_x.central_module_c )
	&& ( ise_subscr_data_px->ise_infobj_x.broadcast_type_x.server_type_c == bcast_type_x.server_type_c )
	&& ( ise_subscr_data_px->ise_infobj_x.broadcast_type_x.transaction_number_n == bcast_type_x.transaction_number_n ))
	{
	    // a match for bcast type - how about matching commodity codes...did the caller specify one ?
	    if ( commodity_code_u == 0 )
		// no, caller did NOT specify one
		return ise_subscr_data_px;

	    switch ( ise_subscr_data_px->ise_infobj_x.inftyp_u )
	    {
	    case INFO_TYPE_DIRECTED:
	    case INFO_TYPE_GENERAL:
		// neither of these infotypes allow a qualification by underlying / commodity code
		return ise_subscr_data_px;
		break;

	    case INFO_TYPE_DERIVATIVE:
		// if there is no specific value in the infobj then match
		if ( ise_subscr_data_px->ise_infobj_x.attrib_x.derivative_x.commodity_u == 0 )
		{
		    // return pointer to this entry to caller
		    return ise_subscr_data_px;;
		}
		else
		{
		    // this infobj has specific commodity - and we had one passed to us
		    if ( ise_subscr_data_px->ise_infobj_x.attrib_x.derivative_x.commodity_u == commodity_code_u )
		    {
			// exact commodity code match
			// return pointer to this entry to caller
			return ise_subscr_data_px;
		    }
		    // else case drops thru to try next authorised bcast

		}   // test if authorised has specified bcast

		break;
	    case INFO_TYPE_UNDERLYING:
		// if there is no specific value in the infobj then match
		if ( ise_subscr_data_px->ise_infobj_x.attrib_x.underlying_x.commodity_u == 0 )
		{
		    // return pointer to this entry to caller
		    return ise_subscr_data_px;;
		}
		else
		{
		    // this infobj has specific commodity - and we had one passed to us
		    if ( ise_subscr_data_px->ise_infobj_x.attrib_x.underlying_x.commodity_u == commodity_code_u )
		    {
			// exact commodity code match
			// return pointer to this entry to caller
			return ise_subscr_data_px;
		    }
		    // else case drops thru to try next authorised bcast

		}   // test if authorised has specified bcast

		break;

	    // we need a case for INFO_TYPE_DESSEMINATION_GROUP %%%%

	    default:
		printf ("find_subscription with Unknown infotype %u, in %s\n",
			    ise_subscr_data_px->ise_infobj_x.inftyp_u,
			    bcast_type_to_string ( ise_subscr_data_px->ise_infobj_x.broadcast_type_x )
			);
	    }	// switch on infobj type
	}   // test for matching broadcast type

	// no match, advance to next (which may be NULL)
	ise_subscr_node_px = (gen_buf_node_xt *)ise_subscr_node_px->chain.next;
    }

    // if we got here we failed to find a match
    return NULL;

}   // find_subscription

// ---------------------------------------------------------------------------

static int32 query_bcast_events ( click_details_xt * click_px )
/*
Function : query_bcast_events
Author	 : Neil Cossor
Modified : 991127
Description:

    For the broadcast system facility, interrogate the
    gateway for all broadcasts for which we have 
    permission to subscribe.

    We then save that list.

    Note that this is the complete set of broadcasts 
    for which we can subscribe - it is up to us to 
    decide whether we want to subscribe to them

    Important note:- 991127, earlier versions of the code which queried the
    authorised broadcast list looked for the OMNIAPI_ALLEVTS status value
    in a loop. There is no mechanism for this query to return the information
    in more than one query / response...
    The comparison in the old code was an = NOT an ==, meaning the code
    did terminate (correctly) after one query, although for the wrong reasons.

    This function call is ALWAYS done at logon, and control
    is not returned until after we have the authorised list
    -> we won't have contention issues with command-driven omniapi calls.
*/
{
    // pointer to a list of items, each one describing one bcast which we are authorised for
    show_subscr_list_t	*auth_bcast_px 	    =	NULL;
    // block of memory, into which the OMNIAPI query puts the raw info about a bunch of authorised bcast subscriptions
    int8		auth_bcast_buffer_ps[EVENT_LIST_BUFFER_LEN];
    // pointer to info (in OM format) about one authorised subscription (event)
    auth_infobj_t	*auth_bcast_info_px		    =	NULL;
    // status returned to us by API calls
    int32		status_i			    =	SUCCESSFUL;
    // number of authorised items returned to us in one query
    uint16		auth_items_u;

    // loop var, for extracting useful info from the array of authorised bcasts in a response
    uint32		auth_bcast_index_u		    =	0;
    // how big our buffer is, for receiving lists of authorised broadcasts
    uint32		auth_bcast_bufflen_u		    =	0;
    // temp, for reporting additional info about some bcast types
    uint16		commodity_u			    =	0;
    // pointer to the string we will use to describe one particular authorised event (general, derivative, underlying, directed )
    int8		*info_type_ps		    =	NULL;
    // pointer to q'able node, which will contain info about one authorised subscription (event)
    gen_buf_node_xt	*ise_subscr_node_px;
    // pointer to the actual data we store (in ISEsim format) about one authorised subscription (event)
    one_subscription_xt
			*ise_subscr_data_px;

    // point to start of buffer, into which the omniapi_read_event_ext has retrieved a block
    // of info about authorised bcasts
    auth_bcast_px = (show_subscr_list_t *) &auth_bcast_buffer_ps[0];

    // blank the target buffer for reading about the authorised broadcasts
    memset ( &auth_bcast_buffer_ps[0],
		0,
		sizeof (auth_bcast_buffer_ps));
    // set up var with the size of the rx buffer - NB must set this for each call, as on each call
    // the var is updated with the # of bytes rx'ed
    auth_bcast_bufflen_u =  sizeof (auth_bcast_buffer_ps);

    // go get a bunch of authorised subscriptions (events)
    status_i = omniapi_read_event_ext ( OMNI_EVTTYP_SHOW_SUBSCR,
					   &auth_bcast_buffer_ps[0],	// address of our buffer
					   &auth_bcast_bufflen_u,	// size of our buffer -> bytes returned
					   NULL,			// reserved, not used by OM
					   0 );				// event mask, not used for this query type
    if ( status_i < SUCCESSFUL )
	// -ve, hard error.. bail out
	return (status_i);

    if ( status_i == OMNIAPI_OVERFLOW )
	// for reasons unknown, OM considers an overflow a 'normal' +ve status
	// do nothing - just bail out - errors should ripple out
	return (status_i);

    // normal, (non-overflow) status, retrieving a chunk of the authorised subscriptions (events)

    /* There are two basic types of subscription to messages:
	1) Forced subscription (we have no choice, we will get them)
	2) Optional subscription (if we have permission, it is up
	to us to decide whether we want to subscribe to them)
	In this routine we discard any forced subscriptions as
	the gateway will take care of those, and only attempt
	to subscribe for those broadcasts in which we're interested
	In this case we will subscribe to everything, though
	there is nothing to prevent the application developer
	imposing their own in-house logic as to which broadcasts 
	for which we should subscribe.
     */

    // save the info about the available broadcasts
    // how many did we get in this response (in local endian order)
    PUTSHORT ( auth_items_u, auth_bcast_px->objects_u );
    for (auth_bcast_index_u = 0;
	 auth_bcast_index_u < auth_items_u; 
	 auth_bcast_index_u++ )
    {
	// we wish to save info about one authorised subcription (event)

	// point into info about one authorised bcast in this query's response
	auth_infobj_t *auth_bcast_info_px = &auth_bcast_px->assobj_x[auth_bcast_index_u];
	// allocate a q'able node, into which we store the info.. in local format
	ise_subscr_node_px = new_gen_buf_node ( sizeof ( one_subscription_xt ));
	// and point to the part of the allocated node, to the part we actually put the data into
	// grab typecast copy of pointer into dest storage for instrument class details
	(char *)ise_subscr_data_px = ise_subscr_node_px->msg_p;

	// extract info about one authorised subscription (event).. save fields in local endian order
	rx_infobj ( ise_subscr_data_px,
		    &auth_bcast_info_px->infobj_x,
		    auth_bcast_info_px->force_u );

	// add this node into the list of known, authorised subscriptions (events)
	add_to_q_tail ( &authorised_bcasts_q,
			&ise_subscr_node_px->chain );
    }	// for loop, extracting items for individual authorised broadcasts

    printf("\n");

    return (status_i);
}   // query_bcast_events

// -------------------------------------------------------------------------

static int32 subscribe_to_one_bcast_event ( click_details_xt * click_px,
					    broadcast_type_t bcast_type_x,
					    ise_underlying_list_xt *ise_subscr_list_px,
					    unsigned dedicated_user_u )
/*
Function : subscribe_to_one_bcast_event
Author	 : Neil Cossor
Modified : 20010621
Description:

    For the broadcast system, we wish to add a particular subscription
    to the list. This means, of course, that the subscription we are
    requesting should be an optional one (not much point subscribing to
    a mandatory broadcast).

    There are still some issues, where, for example we can subscribe for one
    transaction type at the commodity level, or all commodities (wildcard 0).
    Say, we have a subscription with a particular commodity, and then want
    to have a subscription (for the same transaction type) for all commodities..
    do we cancel the previous (single commodity) subscription, and then add
    the new one..

    A precondition is that we have already found out the list of authorised optional
    broadcasts (at logon, with query_bcast_events).

    We are only adding a subscription for a single bcast type, it is just a matter
    of which combination of underlying / expiry dates we are subscribing to.

Input Params:

    - pointer to structure containing everything we know about central system / market
    - pointer to broadcast type string, e.g. "BD101"
    - prepacked list of subscription info relating to underlying
    - flag (if value TRUE) for dedicated, indicates that we only want User bcasts, not member bcasts
*/
{
    // return status from API calls - returned to caller
    int32		status_i			    =	SUCCESSFUL;
    // local var, for looping thru arrays
    uint16		item_u;
    /* similar stuff as we store for each auth / subscribed bcast.
	We generate the count and commodity-code list from the argc / argv given.
	If the subscription request goes OK, then we add our list to the attributes we
	    already had for the authorised / subscribed bcast..
    */

    // the size, in bytes, of the data that we have added to our list of optionals, we are about to subscribe to
    int32		set_event_list_size_u	    =	0;
    // pointer to the list of optionals.. we malloc this as necessary, and release before return
    set_event_list_t	*set_event_list_px;
    // pointer to one item in the actual optional in the set_event list
    subscr_item_t	*set_event_item_px;

    // pointer to info about one subcription (event) record, containing things ISEsim already knows about
    one_subscription_xt *ise_subscr_data_px;




    // pack the request
    // work out the size of the request..
    set_event_list_size_u = sizeof (set_event_list_px->buflen_i)
				    + ( sizeof ( set_event_list_px->subitm_x[0] )
					* ise_subscr_list_px->underlying_count_u );

    // allocate the room to store the request
    set_event_list_px = ( set_event_list_t * ) malloc ( set_event_list_size_u );

    // blank the allocated memory - don't want any leftover's in memory
    // this blanks the status and handle fields.
    memset ( set_event_list_px, 0, set_event_list_size_u );

    /* save the size of the request.. NB this is multibyte, so endian
	Note that unlike some of the other subscription records, the header
	contains the size (in bytes) of the command, and NOT the number of entries...
    */ 
    PUTLONG (set_event_list_px->buflen_i, set_event_list_size_u);  

    // pack in all of the components - there will be only one component for non-derivative infotype
    for ( item_u = 0;
	    item_u < ise_subscr_list_px->underlying_count_u; 
	    item_u++ )
    {
	// point into one item of the subscription list
	set_event_item_px = &set_event_list_px->subitm_x [item_u];


	/* find the appropriate authorised bcast.. 
	    NB if the info type of the authorised subscription (event) is derivative
	    of underlying, then there may be multiple authorised infobj entries..
	    and we need to use the specific infobj
	*/
	ise_subscr_data_px = find_subscription ( bcast_type_x, ise_subscr_list_px->commodity_code_u [item_u] );
	// did we find a match ? it is possible (say bo102) for us to have a specific list
	// of authorised underling, which the caller / script has gone outside
	if ( ise_subscr_data_px == NULL )
	{
	    printf ("warning, specific underlying %s is NOT authorised for bcast type %s\n",
			commodity_code_to_string ( click_px,
						    ise_subscr_list_px->commodity_code_u [item_u] ),
			bcast_type_to_string ( bcast_type_x )
		    );
	    // fudge - attempt to use the infobj from whatever the first matching bcast_type we find
	    ise_subscr_data_px = find_subscription ( bcast_type_x, 0 );
	}   // test if problem finding exact authorised bcast

	// put in the standard infobj from the known list list
	tx_infobj ( &set_event_item_px->infobj_x, 
		    ise_subscr_data_px );
	// if this is a 'derivative' or 'underlying' infotype, then we manipulate the commodity code
	switch ( ise_subscr_data_px->ise_infobj_x.inftyp_u )
	{
	case INFO_TYPE_DERIVATIVE:
	    // tack in the particular commodity code.. which may be zero
	    PUTSHORT ( set_event_item_px->infobj_x.attrib_x.derivative_x.commodity_n,
			ise_subscr_list_px->commodity_code_u [ item_u ] );
	    // do nothing with expiry month and year, we are NOT implementing granularity this fine
	    break;
	case INFO_TYPE_UNDERLYING:
	    // tack in the particular commodity code.. which may be zero
	    PUTLONG ( set_event_item_px->infobj_x.attrib_x.underlying_x.commodity_u,
			((uint32)ise_subscr_list_px->commodity_code_u [item_u ]) );
	    break;
	case INFO_TYPE_DIRECTED:
	    // tack in the user/member flag - member is default (zeroes)
	    if ( dedicated_user_u == user_member_user_e )
		// OK, must set flag to get user only
		PUTSHORT ( set_event_item_px->infobj_x.attrib_x.dedicated_x.member_info_n, 1 );
	    break;
	default:
	    break;
	}   // switch on infotype, whether we needed to tack in a commodity code
    }	// loop, putting individual components into set_event request


    // grab exclusive access to OMNIAPI - i.e. block out any other threads
    ISE_MUTEX_LOCK ( click_px->omni_io_active_mutex );

    // actually send in the subcription request
    status_i = omniapi_set_event ( GENERAL_BROADCAST_EVENT_TYPE,
				    (char*)set_event_list_px );

    // release the synchronisation lock so other threads can do OMNIAPI IOs
    ISE_MUTEX_UNLOCK ( click_px->omni_io_active_mutex );

    // what happens here if we get an error status for the overall transaction
    // Particularly, can some of the list work, and others fail ?
    // for now, assume overall status failure means none of the components worked
    if ( status_i < SUCCESSFUL )
    {
	// release memory used for event list
	free ( set_event_list_px );

	// and return to caller
	return (status_i);
    }

    // need to save the subscription handles (for later cancels), and note any error statii
    for ( item_u = 0;
	    item_u < ise_subscr_list_px->underlying_count_u; 
	    item_u++ )
    {
	// point into one item of the subscription list
	set_event_item_px = &set_event_list_px->subitm_x [item_u];

	// OK status ? If so, save the handle for that component
	if ( set_event_item_px->status_i < SUCCESSFUL )
	    printf ("Error %d with subscription\n",
			set_event_item_px->status_i );
	else
	{
	    // success - add to list of active subscriptions - try for specific authorised subscr
	    ise_subscr_data_px = find_subscription ( bcast_type_x, ise_subscr_list_px->commodity_code_u [item_u] );
	    // did we find a match ? it is possible (say bo102) for us to have a specific list
	    // of authorised underling, which the caller / script has gone outside
	    if ( ise_subscr_data_px == NULL )
	    {
		// don't have to tell user again - we did above

		// fudge - attempt to use the infobj from whatever the first matching bcast_type we find
		ise_subscr_data_px = find_subscription ( bcast_type_x, 0 );
	    }   // test if problem finding exact authorised bcast

	    // note the commodity code - could be 0 (i.e. wildcard *)
	    ise_subscr_data_px ->commodity_code_u[ise_subscr_data_px->underlying_count_u] =
		ise_subscr_list_px->commodity_code_u [ item_u ];
	    // note the subscription handle
	    PUTLONG ( ise_subscr_data_px ->handle_u[ise_subscr_data_px->underlying_count_u],
			set_event_item_px->handle_u );
	    ise_subscr_data_px->underlying_count_u++;
	    // if it is dedicated, note whether we did user/member subscribe
	    if ( ise_subscr_data_px->ise_infobj_x.inftyp_u == INFO_TYPE_DIRECTED )
		// OK, note whether we subscribed user or member
		ise_subscr_data_px->user_member_u = dedicated_user_u;

	}
    }	// loop, putting individual components into set_event request

    // release memory allocated for set_event
    free ( set_event_list_px );

    return (status_i);
}   // subscribe_to_one_bcast_event

//----------------------------------------------------------------------------

int32 parse_underlying_bcast_list ( click_details_xt * click_px,
				    broadcast_type_t bcast_type_x,
				    ise_underlying_list_xt *ise_subscr_list_px,
				    int argc,
				    char *argv[])
/*
Function : parse_underlying_bcast_list
Author	 : Neil Cossor
Modified : 20000721
Description:

    We had a command to add/remove a subscription..

Input Params:

    pointer to structure containing everything we know about central system / market
    pointer to broadcast type string, e.g. "BD101"
    pointer to list (which we update) with the commodity codes that we parsed
    count of the # of string arguments, presented in, which must be 2+
    pointer to array of strings, containing the rest of the arguments
	the first of these should be (string version) of a broadcast type e.g. "BD101"
	from the second on should be a list of underlying. "*" and "%bin" are allowed

Output Params:

    returns INVALID_ARGUMENTS status if bad broadcast type specified or underlying params
    return other error statii from lower routines
*/
{

    // which 'case' did the caller specify for each entry in the argv list.. *, %bin, <underlying>
    int			under_selected_i;
    // local var, for looping thru arrays
    uint16		item_u;
    // status var from called routines, which we return to caller
    int32		    status_i;

    // pointer to data about one underlying
    ise_underlying_xt * ise_under_data_px;
    // pointer to info about one subcription (event) record, containing things ISEsim already knows about
    one_subscription_xt *ise_subscr_data_px;

    /* see if we know about the nominated broadcast type
	If not (NULL pointer), we are NOT authorised for it.
	NB if the info type of the authorised subscription (event) is derivative
	of underlying, then there may be multiple authorised infobj entries..
	and we should update the correct authorised entry
    */
    ise_subscr_data_px = find_subscription ( bcast_type_x, 0 );
    // did we find a match ?
    if ( ise_subscr_data_px == NULL )
    {
	// fail - we are not authorised for this bcast.. of course, perhaps that bcast doesn't exist
	printf ( "Unable to subscribe to bcast %s, not authorised or nonexistent\n",
		    bcast_type_to_string ( bcast_type_x )
		);
	// bail out, returning bad status
	return INVALID_ARGUMENTS;
    }	// test if we are authorised for this broadcast type
    
    // is this broadcast optional - no point subscribing if it is forced.. doing so may actually be an error
    if ( ise_subscr_data_px->force_u != 0 )
    {
	// warn user
	printf ("Warning - subscription to bcast %s ignored - already forced (not optional)\n",
		    bcast_type_to_string ( bcast_type_x )
		);		
	// bail out, returning bad status
	return INVALID_ARGUMENTS;
    }	// test if bcast was already forced (not optional)

    // start out with an 'empty' list of specific underlying to subscribe to :- 0 count, all entries 0
    memset ( ise_subscr_list_px, 0, sizeof (*ise_subscr_list_px));

    /* we only get called for derivative and underlying infotypes.
	ISEsim at this point, only supports derivative by underlying (not by expiration year or month)
    */

    // derivative or underlying type - we can have a list of underlying
    /* we have a known (authorised), optional broadcast..
	Now we need to check if we are already subscribed for the particular underlying listed..
	in the process, we generate a commodity-code list to pack into the subscription
    */
    if ( argc < 1 )
    {
	printf ("Insufficient args to add_one_subscription (%s), with infotyp 'derivative'\n",
		bcast_type_to_string ( bcast_type_x )
		);
	return INVALID_ARGUMENTS;
    }	// test if arg count is valid

    for ( item_u = 0;
	    item_u < argc; 
	    item_u++ )
    {
	under_selected_i = select_string_option ( 3, bcast_selectors_s, argv[item_u] );
	switch ( under_selected_i )
	{
	case bcast_under_all_e:
	    // if any item is '*', then illegal with any other underlying in this list

	    // we have wildcard * - is there only one entry in the underlying list ?
	    if ( argc != 1 )
	    {
		printf ("Error - wildcard '*' in bcast subscription, with specific underlying also specified\n" );
		return INVALID_ARGUMENTS;
	    }	// oops - all underlying wildcard, along with specific underlying

	    // put into list for subscription..
	    ise_subscr_list_px->underlying_count_u = 1;

	    // as the whole list was zotted with memset, we already have commodity code 0 in the [0] slot

	    break;

	case bcast_under_bin_e:
	    /* %bin special.. must substitute in all of the underlying for this bin
		Of course, we must be a CMM / PMM for this to make sense.

		If we are NOT an MM, then we should have set a default bin to work in,
		using set,bin,<number>
	    */
	    status_i = subscr_list_bin ( click_px,
					    ise_subscr_list_px,
					    click_px->our_market_x.default_bin_u );
	    // did that work ?
	    if (status_i != SUCCESSFUL)
	    {
		printf ("Error subscribing to %s in bin %u\n",
			    bcast_type_to_string ( bcast_type_x ),
			    click_px->our_market_x.default_bin_u );
		return status_i;
	    }

	    break;

	case bcast_under_folio_e:
	    // %folio
	    not_yet_implemented ("folio selectors in BCast command");
	    return INVALID_ARGUMENTS;

	default:
	    // must be a real underlying (or %u)
	    // search for the underlying.. if unknown, then skip it from list with a warning to user
	    ise_under_data_px = find_under_by_name_s ( click_px,
							argv [item_u ] );
	    // did we find a match
	    if ( ise_under_data_px )
	    {
		// known underlying.. add the commodity code to the list
		ise_subscr_list_px->commodity_code_u [ ise_subscr_list_px->underlying_count_u ]
		    = ise_under_data_px->commodity_code_u;
		ise_subscr_list_px->underlying_count_u++;
	    }
	    else
	    {
		// unknown underlying - report it.. and keep going, skipping this underlying
		printf ("Subscription to %s requested for unknown underlying %s\n",
			    bcast_type_to_string ( bcast_type_x ),
			    argv [ item_u ] );
	    }   // test if we found specific underlying (by name)
	}	// switch, on the type of a particular argv[]
    }	// for loop, scanning the underlying list


    // probably should do consistency check that we don't have any repeat commodity codes

    // have we at least one commodity-code entry (which could be 0 == all commodity codes)
    if ( ise_subscr_list_px->underlying_count_u == 0 )
    {
	printf ( "Subscription request with no commodity selection remaining\n" );
	// bail out of this routine
	return INVALID_ARGUMENTS;
    }	// test if we actually got at least one entry after all that

    return SUCCESSFUL;
}   // parse_underlying_bcast_list


// -------------------------------------------------------------------------

int32 add_one_subscription ( click_details_xt * click_px,
				char * bcast_type_s,
				int argc,
				char *argv[])
/*
Function : add_one_subscription
Author	 : Neil Cossor
Modified : 20010624
Description:

    We had a command to add a subscription..

Input Params:

    pointer to structure containing everything we know about central system / market
    pointer to broadcast type string, e.g. "BD101"
    count of the # of string arguments, presented in, which must be 2+
    pointer to array of strings, containing the rest of the arguments
	the first of these should be (string version) of a broadcast type e.g. "BD101"
	from the second on should be a list of underlying. "*" and "%bin" are allowed

Output Params:

    returns INVALID_ARGUMENTS status if bad broadcast type specified or underlying params
    return other error statii from lower routines
*/
{
    // status var from called routines, which we return to caller
    int32		    status_i = SUCCESSFUL;
    // local (struct) version of the broadcast type (supplied to us as string)
    broadcast_type_t	    bcast_type_x;
    // temp - list of underlying we parse from arglist, to pass into routine to add subscriptions
    ise_underlying_list_xt  ise_subscr_list_x;
    // pointer to info about one subcription (event) record, containing things ISEsim already knows about
    one_subscription_xt *ise_subscr_data_px;
    // flag, indicates is we only want User bcasts, not member bcasts, default Member
    unsigned		dedicated_user_u = user_member_member_e;




    if (!click_px->logged_on_i )
    {
	prompt_logged_on ("BCAST,<bcast ID>, ON");
	return NOT_LOGGED_ON;
    }

    
    /* try to find the broadcast corresponding to argv[0]
	The input form for a broadcast form MUST be 2 x alphabetic, followed by a number.
	NB we know that leading / trailing spaces have been stripped.
    */
    if ( strlen ( bcast_type_s) < 3 )
    {
	printf ("Insufficient chars (%u) to add_one_subscription in broadcast type\n",
		    strlen ( bcast_type_s )
		);
	return INVALID_ARGUMENTS;
    }	// test if enuff chars in argv[0] to contain valid broadcast type

    // grab first two chars
    bcast_type_x.central_module_c = *bcast_type_s++;
    bcast_type_x.server_type_c = *bcast_type_s++;
    // read the transaction # from the string.. returns a 0 if error..
    // which means we won't find a matching bcast type, as none of them have a 0 
    bcast_type_x.transaction_number_n = atoi ( bcast_type_s );

    /* see if we know about the nominated broadcast type
	If not (NULL pointer), we are NOT authorised for it.
	NB if the info type of the authorised subscription (event) is derivative
	of underlying, then there may be multiple authorised infobj entries..
	and we should update the correct authorised entry
    */
    ise_subscr_data_px = find_subscription ( bcast_type_x, 0 );
    // did we find a match ?
    if ( ise_subscr_data_px == NULL )
    {
	// fail - we are not authorised for this bcast.. of course, perhaps that bcast doesn't exist
	printf ( "Unable to subscribe to bcast %s, not authorised or nonexistent\n",
		    bcast_type_to_string ( bcast_type_x )
		);
	// bail out, returning bad status
	return INVALID_ARGUMENTS;
    }	// test if we are authorised for this broadcast type
    
    // is this broadcast optional - no point subscribing if it is forced.. doing so may actually be an error
    if ( ise_subscr_data_px->force_u != 0 )
    {
	// warn user
	printf ("Warning - subscription to bcast %s ignored - already forced (not optional)\n",
		    bcast_type_to_string ( bcast_type_x )
		);		
	// bail out, returning bad status
	return INVALID_ARGUMENTS;
    }	// test if bcast was already forced (not optional)

    // OK, what sort of arguments can we have ?
    switch ( ise_subscr_data_px->ise_infobj_x.inftyp_u )
    {
    case INFO_TYPE_DERIVATIVE:
    case INFO_TYPE_UNDERLYING:
	// both of these have an underyling list as the the arguments

	// parse the underyling list, w.r.t. the allowed subscriptions
	status_i = parse_underlying_bcast_list ( click_px,
						    bcast_type_x,
						    &ise_subscr_list_x,
						    argc,
						    &argv[0] );
	break;
    case INFO_TYPE_GENERAL:
	//  no arguments for this.. it is on or off.. we package this
	// as a one-entry underlying list, which has 0 as the underlying
	ise_subscr_list_x.underlying_count_u = 1;
	break;
    case INFO_TYPE_DIRECTED:
	// optional argument is user vs member

	// did they specify user / member ? 
	if (( argc >= 1) && ( argv[0] != NULL))
	{
	    dedicated_user_u = cmd_user_member ( argv[0] );
	}
	else
	{
	    // command did not specify user vs member.. use our default setting
	    dedicated_user_u = click_px->user_member_default_u;
	}

	// mark this subscription as 1 entry, commodity code 0 (all)
	// although we don't actually specify a commodity_code for this infotype's,
	// simpler to track and report this way
	ise_subscr_list_x.underlying_count_u = 1;

	break;
    default:
	printf ("Unknown infotype %u for bcast %s\n",
		ise_subscr_data_px->ise_infobj_x.inftyp_u,
		    bcast_type_to_string ( bcast_type_x )
		);
	return INVALID_ARGUMENTS;
    }


    if ( status_i >= SUCCESSFUL )
	// go attempt to actually add subscription
	status_i = subscribe_to_one_bcast_event ( click_px,
						    bcast_type_x,
						    &ise_subscr_list_x,
						    dedicated_user_u );
    return status_i;
}   // add_one_subscription

// -------------------------------------------------------------------------

static int32 subscribe_to_all_bcast_events ( click_details_xt * click_px )
/*
Function : subscribe_to_all_bcast_events
Author	 : Neil Cossor
Modified : 20000122
Description:

    For the broadcast system, we wish to subscribe to all optional events.

    A precondition is that we have already found out the list of authorised optional
    broadcasts (at logon, with query_bcast_events).

    As part of this routine, we update the info we store about subscriptions..

    What does this mean, however, to any other subscriptions we have made..

    There is a particular flavour of set_event which allows subscribing to
    all optionals in one go. This is done by NOT specifying a eventlist buffer.
    Note, however, that in doing this, we do NOT have a subscription handle..
    this fouls up the code for reading the broadcasts. If you make the subscription
    in this all-flavour (without handles), the format of the buffer returned
    when actually reading the broadcasts is different. Also, it makes it impossible
    to do cancellation of the broadcasts in anything smaller than 'everything'..
    you can't cancel one particular broadcast type (e.g. BD101).

Input Params:

    none
*/
{
    // return status from API calls - returned to caller
    int32		status_i			    =	SUCCESSFUL;
    // local var, for looping thru arrays
    uint16		item_u;

    // the size, in bytes, of the data that we have added to our list of optionals, we are about to subscribe to
    int32		set_event_list_size_u	    =	0;
    // pointer to the list of optionals.. we malloc this as necessary, and release before return
    set_event_list_t	*set_event_list_px;
    // pointer to one item in the actual optional in the set_event list
    subscr_item_t	*set_event_item_px;

    // pointer to info about one subcription (event) record, containing things ISEsim already knows about
    one_subscription_xt *ise_subscr_data_px;
    // pointer to q'able node, each containing info about one subscription event.
    // Use this to chain thru all authorised subscriptions
    gen_buf_node_xt	*ise_subscr_node_px;
    // how many optional broadcasts types we can subscribe to
    uint16		opt_subscr_count_u = 0;


    // first count how many optional subscriptions there are..
    // i.e. how many of our authorised are not FORCED
    ise_subscr_node_px = (gen_buf_node_xt *)authorised_bcasts_q.chain.next;
    while ( ise_subscr_node_px )
    {
	// point to the data component of this q'ed entry
	ise_subscr_data_px = (one_subscription_xt *)ise_subscr_node_px->msg_p;

	// nothing to do if it is a forced subscription
	if ( !ise_subscr_data_px->force_u )
	{
	    // optional subscription.. one more entry required in the set_event list
	    opt_subscr_count_u++;
	}   // test if this is a forced subscription

	// no match, advance to next (which may be NULL)
	ise_subscr_node_px = (gen_buf_node_xt *)ise_subscr_node_px->chain.next;
    }	// loop, while we still have any nodes in the authorised subscriptions to update



    // pack the request
    // work out the size of the request..
    set_event_list_size_u = sizeof (set_event_list_px->buflen_i)
				    + ( sizeof ( set_event_list_px->subitm_x[0] )
					* opt_subscr_count_u );

    // allocate the room to store the request
    set_event_list_px = ( set_event_list_t * ) malloc ( set_event_list_size_u );
    // blank the allocated memory - don't want any leftover's in memory
    // this blanks the status and handle fields
    memset ( set_event_list_px, 0, set_event_list_size_u );
    /* save the size of the request.. NB this is multibyte, so endian
	Note that unlike some of the other subscription records, the header
	contains the size (in bytes) of the command, and NOT the number of entries...
    */ 
    PUTLONG (set_event_list_px->buflen_i, set_event_list_size_u);  

    // pack in all of the individual components
    item_u = 0;
    ise_subscr_node_px = (gen_buf_node_xt *)authorised_bcasts_q.chain.next;
    while ( ise_subscr_node_px )
    {
	// point to the data component of this q'ed entry
	ise_subscr_data_px = (one_subscription_xt *)ise_subscr_node_px->msg_p;

	// nothing to do if it is a forced subscription
	if ( !ise_subscr_data_px->force_u )
	{
	    // optional subscription.. one more entry required in the set_event list

	    // point into one item of the subscription list
	    set_event_item_px = &set_event_list_px->subitm_x [item_u];

	    // put in the standard infobj from the known list list
	    tx_infobj ( &set_event_item_px->infobj_x, 
			ise_subscr_data_px );
	    item_u++;
	}   // test if this is a forced subscription

	// no match, advance to next (which may be NULL)
	ise_subscr_node_px = (gen_buf_node_xt *)ise_subscr_node_px->chain.next;
    }	// loop, while we still have any nodes in the authorised subscriptions to add to set_event list
    // at the end, the number of items we added has to == the number of optional items we counted earlier
    if ( item_u != opt_subscr_count_u )
	printf ("Fatal error, packed more set_event items (%u) than known optional events (%u)\n",
		    item_u,
		    opt_subscr_count_u );


    // grab exclusive access to OMNIAPI - i.e. block out any other threads
    ISE_MUTEX_LOCK ( click_px->omni_io_active_mutex );

    // actually send in the subcription request
    status_i = omniapi_set_event ( GENERAL_BROADCAST_EVENT_TYPE,
				    (char*)set_event_list_px );

    // release the synchronisation lock so other threads can do OMNIAPI IOs
    ISE_MUTEX_UNLOCK ( click_px->omni_io_active_mutex );

    
    // what happens here if we get an error status for the overall transaction
    // Particularly, can some of the list work, and others fail ?
    // for now, assume overall status failure means none of the components worked
    if ( status_i < SUCCESSFUL )
    {
	// release memory used for event list
	free ( set_event_list_px );

	// and return to caller
	return (status_i);
    }




    /* go and update all authorised optional subscriptions to have one
	entry, with 'all commodities' listed for all, with handle values from the buffer.
	Note that we could go thru the buffer, search for the corresponding bcast_type in
	the authorised list, and update that.. but we know the order of packing from above.
	Provided we update using the same logic as the packing above, then we know
	that they are in order
    */
    // pack in all of the individual components
    item_u = 0;
    ise_subscr_node_px = (gen_buf_node_xt *)authorised_bcasts_q.chain.next;
    while ( ise_subscr_node_px )
    {
	// point to the data component of this q'ed entry
	ise_subscr_data_px = (one_subscription_xt *)ise_subscr_node_px->msg_p;

	// nothing to do if it is a forced subscription
	if ( !ise_subscr_data_px->force_u )
	{
	    // optional subscription.. one more entry required in the set_event list

	    // point into one item of the subscription list
	    set_event_item_px = &set_event_list_px->subitm_x [item_u];

	    /* we could (?should) do a debug consistency check, that the bcast_type in the
		infobj_t we are up to in the buffer is the same bcast_type as the
		item we are up to in the authorised broadcast list
	    */

	    // first check that we did NOT have a specific subscription enabled before.. warn user if so
	    if ( ise_subscr_data_px->commodity_code_u [0] != 0 )
	    {
		// warning - we previously had optional subscription for specific underlying
		printf ("Warning, subscribe ALL overriding specific for bcast %s\n",
			    bcast_type_to_string ( ise_subscr_data_px->ise_infobj_x.broadcast_type_x )
			    );
	    }	// test if we already had a subscription list for specific underlying

	    // note one subscription for this, commodity code 0, handle 0
	    ise_subscr_data_px->underlying_count_u = 1;
	    ise_subscr_data_px->commodity_code_u [0] = 0;
	    // note the subscription handle
	    PUTLONG ( ise_subscr_data_px ->handle_u[0],
			set_event_item_px->handle_u );

	    item_u++;
	}   // test if this is a forced subscription

	// no match, advance to next (which may be NULL)
	ise_subscr_node_px = (gen_buf_node_xt *)ise_subscr_node_px->chain.next;
    }	// loop, while we still have any nodes in the authorised subscriptions to add to set_event list


    // release memory allocated for set_event
    free ( set_event_list_px );

    return (status_i);
}   // subscribe_to_all_bcast_events

// -------------------------------------------------------------------------

int32 display_subscriptions ( click_details_xt * click_px )
/*
Function : display_subscriptions
Author	 : Neil Cossor
Modified : 20010621
Description:

    For the broadcast system, we wish to display what subscriptions we have

    A precondition is that we have already found out the list of authorised optional
    broadcasts (at logon, with query_bcast_events).

Input Params:

    pointer to all the stuff we know about the central system.. we need this so that
    we can translate the commodity codes to underlying symbols.
*/
{
    // return status from API calls - returned to caller
    int32		status_i			    =	SUCCESSFUL;

    // pointer to info about one subcription (event) record, containing things ISEsim already knows about
    one_subscription_xt *ise_subscr_data_px;
    // pointer to q'able node, each containing info about one subscription event.
    // Use this to chain thru all authorised subscriptions
    gen_buf_node_xt	*ise_subscr_node_px;

    // local loop var, for whizzing thru the list of underlying for a subscription
    uint16		item_u;


    printf ("The authorised broadcasts are: \n");
    // work our way thru the list of subscriptions.. start at the top.. it may, of course be empty

    lock_q ( &authorised_bcasts_q );

    ise_subscr_node_px = (gen_buf_node_xt *)authorised_bcasts_q.chain.next;
    while ( ise_subscr_node_px )
    {
	// point to the data component of this q'ed entry
	ise_subscr_data_px = (one_subscription_xt *)ise_subscr_node_px->msg_p;

	// print the standard header
	printf ("0-%-1d-%-1d-",
		 ise_subscr_data_px->ise_infobj_x.infsrc_u,
		 ise_subscr_data_px->ise_infobj_x.inftyp_u
		 );
	// if a central module is specified, then all three params of the bcast type should be filled
	if ( ise_subscr_data_px->ise_infobj_x.broadcast_type_x.central_module_c == NUL )
	{
	    printf ( "0, " );
	}
	else
	{
	    // debug - 991103
	    printf ( "%s, ",
			bcast_type_to_string ( ise_subscr_data_px->ise_infobj_x.broadcast_type_x )
		    );
	}   // test if we have a specific broadcast type
    

	/* Decide what information type the broadcast is.
	    Report whatever we know about the information type.
	    NB the information type determines how we interpret the attribute field
	 */

	/* Note that at this point we could, if we wished, display the underlying/derivative
	    information for derivative specific broadcasts - we have the commodity number 
	    and other information supplied - it is up to us to interrogate the series/stock
	    information we have previously loaded to display it appropriately.
	 */
	switch ( ise_subscr_data_px->ise_infobj_x.inftyp_u )
	{
	case INFO_TYPE_DERIVATIVE :
	    printf ( "%s-%u[%s]",
		     &INFO_TYPE_DERIVATIVE_STRING,
		     ise_subscr_data_px->ise_infobj_x.attrib_x.derivative_x.commodity_u,
		     commodity_code_to_string ( click_px,
						ise_subscr_data_px->ise_infobj_x.attrib_x.derivative_x.commodity_u )
		    );
	    break;
	case INFO_TYPE_GENERAL:
	    printf ( "%s-%u",
		     &INFO_TYPE_GENERAL_STRING,
		     ise_subscr_data_px->ise_infobj_x.attrib_x.general_x.no_use_u
		     );
	    break;

	case INFO_TYPE_UNDERLYING:
	    printf ( "%s-%d[%s]",
		     &INFO_TYPE_UNDERLYING_STRING,
		     ise_subscr_data_px->ise_infobj_x.attrib_x.underlying_x.commodity_u,
		     commodity_code_to_string ( click_px,
						(uint16)ise_subscr_data_px->ise_infobj_x.attrib_x.underlying_x.commodity_u )
		     );
	    break;

	case INFO_TYPE_DIRECTED:
	    // directed can not be authorised at a lower level (i.e. always up to user to decide user vs member)
	    printf ( "%s",
			&INFO_TYPE_DIRECTED_STRING
		     );
	    break;

	case INFO_TYPE_DISSEMINATION_GROUP:
	    // 
	    printf ( "%s [%u]",
			&INFO_TYPE_DISSEMINATION_GROUP_STRING,
			ise_subscr_data_px->ise_infobj_x.attrib_x.dissemination_x.dissemination_u
		     );
	    break;

	default:
	    printf ( "Unknown Info type (%u), attrib-%d",
			 ise_subscr_data_px->ise_infobj_x.inftyp_u,
			 ise_subscr_data_px->ise_infobj_x.attrib_x.general_x.no_use_u
		     );
	    break;
	};  // switch on info type


	// nothing more to report if it is a forced subscription
	if ( ise_subscr_data_px->force_u )
	{
	    // forced subscription, report it as such
	    printf ( ", forced\n" );
	}
	else
	{
	    // optional subscription.. need to work thru the list of commodities

	    if (ise_subscr_data_px->underlying_count_u == 0)
		printf (",none\n");
	    else
	    {

		// if it is a directed subscription, display User vs Member, not list of underyling
		if ( ise_subscr_data_px->ise_infobj_x.inftyp_u == INFO_TYPE_DIRECTED )
		{
		    // if we have we subscribed, then display choice and subscription handle
		    
		    printf ( ", %s(%u)\n",
			    (ise_subscr_data_px->user_member_u == user_member_user_e) ? "User" : "Member",
			    ise_subscr_data_px->handle_u[0]
			    );

		}
		else
		{
		    // ??? need some code to handle long lists (line wraps)
		    for (item_u = 0;
			 item_u < ise_subscr_data_px->underlying_count_u;
			 item_u++ )
		    {
			// report the underlying.. NB commodity_code_to_string can handle code 0, and unknown underlying
			printf (",%s(%u)",
		    		commodity_code_to_string ( click_px,
							    ise_subscr_data_px->commodity_code_u [item_u] ),
				ise_subscr_data_px->handle_u [item_u]
				);
		    }	// for loop, whizzing thru loop of commodity codes for this broadcast type

		    printf ("\n");
		}
	    }	// test if we have a subscription active for this one
	}   // test if this is a forced subscription

	// no match, advance to next (which may be NULL)
	ise_subscr_node_px = (gen_buf_node_xt *)ise_subscr_node_px->chain.next;
    }	// loop, while we still have any nodes in the authorised subscriptions to update

    unlock_q ( &authorised_bcasts_q );

    return (status_i);
}   // display_subscriptions

// -------------------------------------------------------------------------

/*
Function : clear_subscriptions_for_event_type
Author	 : Neil Cossor
Modified : 9905xx
Description:

    For supplied event type, clear any subscriptions that may exist
*/
static int32 clear_subscriptions_for_event_type ( click_details_xt * click_px,
						    int32 EventType_i )
{
    int32   status_i	=   SUCCESSFUL;

    // Cancel all subscriptions for this event type
    status_i = omniapi_clear_event (EventType_i, 
				    CANCEL_ALL_SUBSCRIPTIONS);
    
    return ( status_i );
}   // clear_subscriptions_for_event_type

// -------------------------------------------------------------------------

/*
Function : clear_subscriptions_for_all_event_types
Author	 : Neil Cossor
Modified : 9905xx
Description:

    For each known to the system event type, clear
    any subscriptions that may exist
*/
static int32 clear_subscriptions_for_all_event_types ( click_details_xt * click_px )
{

    int32		status_i	 =   SUCCESSFUL;
    int32		EventType_i	=   0;
    int8		*Buffer_ps    =   NULL;
    int8		Buffer_s[TEXT_BUFFER_LEN];
    uint16		TxnNumber_n	=   0;
    uint32		BufferLength_u	=   TEXT_BUFFER_LEN;
    int8		SubscriptionCompleted_b     =	FALSE;
//	quad_word dummyQW;
    Buffer_ps = &Buffer_s[0];

    /* Ask the gateway for a list of known event types */
    if ( (status_i = omniapi_read_event_ext (OMNI_EVTTYP_SHOW, 
						Buffer_s, 
						&BufferLength_u, 
						NULL, 
						0))
						< SUCCESSFUL)
    {
	// report the error
	
	report_click_error ( click_px,
				status_i,
				0);		// no Transaction ID
	return status_i;
    }
    else
    {
	// print out the string buffer, length limited by the # of bytes supplied
	printf ("Cancelling subscription for known Event Types %.*s\n",
		BufferLength_u,
		&Buffer_s[0] );
    
	do 
	{
	    /* For each event type which we are allowed to subscribe to,
	     * cancel the subscription
	     */
	    EventType_i = strtol ( Buffer_ps, &Buffer_ps, 10 );

	    if ( EventType_i == 0)
	    {
		SubscriptionCompleted_b = TRUE;
	    }
	    else
	    {
		if ( *Buffer_ps != NUL )
		{
		    Buffer_ps++;  // skip comma delimiter
		}

		status_i = clear_subscriptions_for_event_type (click_px,
							    EventType_i);
	    }

	} while ( (SubscriptionCompleted_b == FALSE) &&
		  (status_i == SUCCESSFUL)
		);
    }

    return (status_i);
}   // clear_subscriptions_for_all_event_types

// ---------------------------------------------------------------------------

void disable_optional_bcasts ( click_details_xt * click_px )
/*
Function : disable_optional_bcasts
Author	 : Neil Cossor
Modified : 991112
Description:

    Change our overall condition of paying attention to (optional)
    broadcasts, to ignore all
*/
{
    kill_broadcasts_u = TRUE;

    // if we are online (and we had optional subscriptions), we must cancel our subscriptions
    // ????
}   // disable_optional_bcasts

// ---------------------------------------------------------------------------

void enable_optional_bcasts ( click_details_xt * click_px )
/*
Function : enable_optional_bcasts
Author	 : Neil Cossor
Modified : 991112
Description:

    Change our overall condition of paying attention to (optional)
    broadcasts, to not ignore. Note, however, that this does NOT
    actually request (subscribe) to any of the actual broadcast
    type that are available.
*/
{
    kill_broadcasts_u = FALSE;

    // if we are online (and we had optional subscriptions), we must enable all subscriptions
    // ????
}   // enable_optional_bcasts

// ---------------------------------------------------------------------------

int32 subscribe_for_events ( click_details_xt * click_px )
/*
Function : subscribe_for_events
Author	 : Neil Cossor
Modified : 991127
Description:

    Requests a list of assigned event types (for our user)
    and then subscribes to all of them
*/
{
    int32		status_i	=   SUCCESSFUL;
    int32		EventType_i	=   0;
    uint32		BufferLength_u	=   0;
    int8	       *BufferPtr_ps	=   NULL;
    int8		Buffer_s[EVENT_LIST_BUFFER_LEN];


    BufferLength_u = sizeof (Buffer_s);
    memset (&Buffer_s[0], 
	    (char)NUL, 
	    BufferLength_u);

    /* The gateway has returned all of the events to which we can subscribe,
     * so let's subscribe explicitly to them. Note that we don't HAVE to do this
     * as if we read all events (OMNI_EVTTYP_ALL) in OMNIAPI_READ_EVENT_EXT, 
     * without specifically subscribing to something, it will automatically
     * give us what we need. The point here is that you can take the events that
     * you are allowed to read, and decide then whether you want them all, or a subset
     */
    
    /* Ask the gateway for a list of known event types */
    status_i = omniapi_read_event_ext ( OMNI_EVTTYP_SHOW, 
					 Buffer_s, 
					 &BufferLength_u, 
					 NULL, 
					 0);
    if ( status_i < SUCCESSFUL)
	return status_i;

    printf ("Subscribing for Event Types %.*s\n",
	    BufferLength_u,
	    &Buffer_s[0] );
    BufferPtr_ps = &Buffer_s[0];

    // event list arrives as a string, containing a comma-separated list of numbers
    do
    {
	// read the next number in the string buffer
	EventType_i = strtol ( BufferPtr_ps, &BufferPtr_ps, 10 );

	if ( EventType_i == 0)
	{
	    // all done, bail out of loop
	    break;
	}

	// out of buffer ?
	if ( *BufferPtr_ps != NUL )
	{
	    BufferPtr_ps++;  // skip comma delimiter
	}

	// only events with type 1 have the complex optional subscription...
	if ( EventType_i == GENERAL_BROADCAST_EVENT_TYPE)
	{
	    // go and find out what optional broadcasts there are
	    status_i = query_bcast_events ( click_px );
	    if ( status_i >= SUCCESSFUL )
	    {
		// have a list of optional subscriptions - have we been told to kill optional bcasts ?
		if ( !kill_broadcasts_u )
		{
		    // we have NOT been told to kill optional, so subscribe all
		    status_i = subscribe_to_all_bcast_events ( click_px );
		}   // test if we were asked to kill optional broadcasts
	    }	// test success of querying all the authorised subscriptions
	}
	else
	{
	    // yes, we will take this overall event type (and there is no fine granularity)
	    status_i = omniapi_set_event ( EventType_i, 0 );
	}   // test if this is the complex event type 1, with fine-granularity optional subscription

    } while ( status_i >= SUCCESSFUL );

    return (status_i);
}   // subscribe_for_events

// -------------------------------------------------------------------------

static int32 handle_bcast_BD5( click_details_xt * click_px,
			directed_trade_t *bcast_px,
			uint32 bcast_len_u )
/*
Function : handle_bcast_BD5
Author	 : Neil Cossor
Modified : 9905xx
Description:

    got a Dedicated trade info msg - process it
*/
{
    int32	  status_i  = SUCCESSFUL;
    // pointer to the trade component of the bcast (which is basically all of it)
    cl_trade_api_t *trade_px;


    // log our response
    log_bcast ( LOG_K_B_DEDICATED_TRADE,
		bcast_px,
		bcast_len_u );


    // debug - NB got_a_cl_trade_api will print more stuff out onto stdout, so no \n
    printf ( "Broadcast BD5: Dedicated Trade " );

    // stash / report the info about that trade
    trade_px = &bcast_px->cl_trade_api;
    got_a_cl_trade_api ( click_px,
			    trade_px );

    return (status_i);
}   // handle_bcast_BD5

// -------------------------------------------------------------------------

static int32 handle_bcast_BD101( click_details_xt * click_px,
				    trade_info_bd101_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BD101
Author	 : Neil Cossor
Modified : 20000422
Description:

    got a Trade Info msg - process it
*/
{
    int32	  status_i  = SUCCESSFUL;
    // the string form of a premium, with appropriate # of decimal places
    series_premium_st	    premium_s;
    ise_series_xt	    ise_series_x;	// temp, the series in ISE form
    ise_series_info_pt	    found_series_px;	// pointer to info about a series
    // (correct endian) fields from the trade
    uint32		    quantity_u,
			    premium_i;

    // log our response
    log_bcast ( LOG_K_B_TRADE_INFO,
		bcast_px,
		bcast_len_u );


    // debug - no \n, as we are about to spit out more info
    printf ( "BD101Bcast : Trade information, " );

    // get the series, in "normal" form
    get_click_series ( &ise_series_x,
			&bcast_px->series );
    // find the record with our data about the series
    found_series_px = find_series_by_ise_series ( click_px,
						    &ise_series_x );
    // format the premium appropriately - assume that 
    PUTLONG ( premium_i, bcast_px->last_price_i );
    format_whole_decimal_s ( (char *)&premium_s,
				premium_i,
				found_series_px->instrument_class_px->dec_in_premium_u,
				MAX_SERIES_PREMIUM );	// allowed width
    // grab the quantity, correct endian - note the LAST volume is the qty in the trade
    PUTLONG ( quantity_u, bcast_px->volume_u );

    printf("%s%9u@%10s\n",
	    &found_series_px->ise_name_s,
	    quantity_u,
	    &premium_s
	    );

    return (status_i);
}   // handle_bcast_BD101

// -------------------------------------------------------------------------

static int32 handle_bcast_BD102( click_details_xt * click_px,
				    opra_trade_info_bd102_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BD102
Author	 : Neil Cossor
Modified : 20000601
Description:

    got a OPRA Trade info msg - process it
*/
{
    int32	  status_i  = SUCCESSFUL;
    ise_series_xt	    ise_series_x;	// temp, the series in ISE form
    ise_series_info_pt	    found_series_px;	// pointer to info about a series
    // the numeric form of the premium
    int32		    premium_i;
    // the string form of a premium, with appropriate # of decimal places
    series_premium_st	    premium_s,
    // and the ISE premiums
    			    ask_premium_s,
			    bid_premium_s;
    // which exchange an item's BBO is from
    uint8		    country_u;
    // quantity of the trade
    uint32		    quantity_u;
    // string indicator is ISE market is better, match, worse
    char		    compare_s[20];

    // log our response
    log_bcast ( LOG_K_B_OPRA_TRADE,
		bcast_px,
		bcast_len_u );

    // get the series, in "normal" form
    get_click_series ( &ise_series_x,
			&bcast_px->series );
    // grab the trade premium
    PUTLONG ( premium_i, bcast_px->last_price_i );
    // and which market they are at
    country_u = bcast_px->country_c;
    // and the quantity
    PUTLONG ( quantity_u, bcast_px->volume_u );

    // debug
    // find the record with our data about the series
    found_series_px = find_series_by_ise_series ( click_px,
						    &ise_series_x );
    // we SHOULD know about this series
    if ( found_series_px )
    {
	// reformat the away premium for display, with appropriate implied decimals
	format_whole_decimal_s ( (char *)&premium_s,
				    premium_i,
				    found_series_px->instrument_class_px->dec_in_premium_u,
				    MAX_SERIES_PREMIUM );	// allowed width

	printf ("Broadcast BD102: OPRA Trade info, %u@%s in %s at %s\n",
		    quantity_u,
		    &premium_s,
		    &found_series_px->ise_name_s,
		    exchange_to_string ( click_px,
					    country_u )
		    );
	// report in tradefile
	if ( tradefile_px != NULL )
	{
	    fprintf_time ( tradefile_px );

	    // reformat the ISE ask premium for display, with appropriate implied decimals
	    format_whole_decimal_s ( (char *)&ask_premium_s,
					found_series_px->best_ask_premium_i,
					found_series_px->instrument_class_px->dec_in_premium_u,
					MAX_SERIES_PREMIUM );	// allowed width
	    // reformat the ISE bid premium for display, with appropriate implied decimals
	    format_whole_decimal_s ( (char *)&bid_premium_s,
					found_series_px->best_bid_premium_i,
					found_series_px->instrument_class_px->dec_in_premium_u,
					MAX_SERIES_PREMIUM );	// allowed width

	    // is ISEbid < awaytrade < ISEask.. allow for pre-open and post-clost
	    if ( found_series_px->series_status_u != SERIES_STATUS_REGULAR )
	    {
		strcpy ( compare_s, "not regular");
	    }
	    else if (( premium_i > found_series_px->best_bid_premium_i )
		&& ( premium_i < found_series_px->best_ask_premium_i ))
	    {
		// away trade is better.. so ISE is worse
		strcpy ( compare_s, "ISE worse");
	    }
	    else if (( premium_i == found_series_px->best_bid_premium_i )
		|| ( premium_i == found_series_px->best_ask_premium_i ))
	    {
		// away trade only matches the ISE BBO
		strcpy ( compare_s, "ISE match");
	    }
	    else
	    {
		// away trade worse than ISE BBO.. yay for use, loser is customer
		strcpy ( compare_s, "ISE better");
	    }

	    fprintf ( tradefile_px, "%s, trade, %s, %u, %s, %s, bid, %s, ask, %s\n",
			exchange_to_string ( click_px,
						country_u ),
			found_series_px->ise_name_s,
			quantity_u,
			&premium_s,
			&compare_s,
			&bid_premium_s,
			&ask_premium_s
		    );
	    fflush ( tradefile_px );
	}
    }
    else
    {
	printf ("Broadcast BD102: OPRA Trade info, unknown series\n" );
    }

    return (status_i);
}   // handle_bcast_BD102

// -------------------------------------------------------------------------

static int32 handle_bcast_BD103( click_details_xt * click_px,
				    underlying_trade_info_bd103_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BD103
Author	 : Neil Cossor
Modified : 20000105
Description:

    got an Underlying Trade info msg - process it
*/
{
    int32	  status_i  = SUCCESSFUL;

    // the commodity code of underlying in the bcast
    uint16		found_commodity_code_u;
    // the underlying record for the underlying
    ise_underlying_xt	*found_under_data_px;

    // log our response
    log_bcast ( LOG_K_B_UNDERLYING_INFO,
		bcast_px,
		bcast_len_u );

    /* update the common with the data in the bcast
	NB potential locking problem.. as we are just updating numbers,
	shouldn't be a problem
    */
    PUTSHORT ( found_commodity_code_u,
		bcast_px->series.commodity_n );
    // find the underlying record
    found_under_data_px = find_under_data_by_commodity ( click_px,
						    found_commodity_code_u );

	if(found_under_data_px == NULL)
		return 0;
    // and update with the data we got
    // update the high
    PUTLONG ( found_under_data_px->high_price_i,
		bcast_px->high_price_i );
    // same deal for low
    PUTLONG ( found_under_data_px->low_price_i,
		bcast_px->low_price_i );
    // same deal for last
    PUTLONG ( found_under_data_px->last_price_i,
		bcast_px->last_price_i );
    // the volume
    PUTLONG ( found_under_data_px->volume_u,
		bcast_px->volume_u );
    // and the turnover
    PUTLONG ( found_under_data_px->turnover_u,
		bcast_px->turnover_u );

    // note the time we got this update
    found_under_data_px->time_trade_update_u = time(NULL);

    // debug
    printf ( "Broadcast BD103: Underlying Trade info, " );

    display_one_under_bbo ( found_under_data_px );

    return (status_i);
}   // handle_bcast_BD103

// -------------------------------------------------------------------------

static int32 handle_bcast_BD111( click_details_xt * click_px,
				    deal_user_bd111_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BD111
Author	 : Neil Cossor
Modified : 20001229
Description:

    got an Deal in Market msg - process it
*/
{
    int32		    status_i  = SUCCESSFUL;
    // deal price, in local endian order
    uint32		    price_i;
    // quantity, in local endian order
    uint16		    qty_u;
    ise_series_xt	    ise_series_x;	// temp, the series in ISE form
    ise_series_info_pt	    found_series_px;	// pointer to info about a series
    // the string form of a deal price, with appropriate # of decimal places
    series_premium_st	    deal_price_s,
    // and the away premiums
    			    ask_premium_s,
			    bid_premium_s;

   	quad_conv_t * convOrderNo;

    // loop var for displaying deal components i.e. trades
    uint8		    deal_item_u;
    
    // string indicator is ISE trade is better, match, worse
    char		    compare_s[20];

    // log our response
    log_bcast ( LOG_K_B_DEAL_MARKET,
		bcast_px,
		bcast_len_u );

    // get the series, in "normal" form
    get_click_series ( &ise_series_x,
			&bcast_px->series );
    // find the record with our data about the series
    found_series_px = find_series_by_ise_series ( click_px,
						    &ise_series_x );
    // grab deal qty in local-endian
    PUTLONG ( qty_u, bcast_px->deal_quantity_u );
    // get the deal price in local endian
    PUTLONG (  price_i, bcast_px->deal_price_i );
    // reformat the price for display, with appropriate implied decimals
    format_whole_decimal_s ( (char *)&deal_price_s,
				price_i,
				found_series_px->instrument_class_px->dec_in_premium_u,
				MAX_SERIES_PREMIUM );	// allowed width

   // debug - display overall deal info
    printf ( "Broadcast BD111: Deal in Market, %s, %u@%s, source %u, %u items\n",
		found_series_px->ise_name_s,
		qty_u,
		&deal_price_s,
		bcast_px->deal_source_c,
		bcast_px->items_c );

    // report in tradefile
    if ( tradefile_px != NULL )
    {
	fprintf_time ( tradefile_px );

	// reformat the away premiums for display, with appropriate implied decimals
	format_whole_decimal_s ( (char *)&ask_premium_s,
				    found_series_px->opra_best_ask_premium_i,
				    found_series_px->instrument_class_px->dec_in_premium_u,
				    MAX_SERIES_PREMIUM );	// allowed width
	format_whole_decimal_s ( (char *)&bid_premium_s,
				    found_series_px->opra_best_bid_premium_i,
				    found_series_px->instrument_class_px->dec_in_premium_u,
				    MAX_SERIES_PREMIUM );	// allowed width
	// do we have away prices yet
	if ( found_series_px->opra_best_ask_premium_i == 0 )
	{
	    strcpy ( compare_s, "not regular");
	}
	// is awaybid < ISEtrade < awayask
	else if (( price_i > found_series_px->opra_best_bid_premium_i )
	    && ( price_i < found_series_px->opra_best_ask_premium_i ))
	{
	    // ISE trade is better
	    strcpy ( compare_s, "ISE better");
	}
	else if (( price_i == found_series_px->opra_best_bid_premium_i )
	    || ( price_i == found_series_px->opra_best_ask_premium_i ))
	{
	    // ISE trade only matches the away BBO
	    strcpy ( compare_s, "ISE match");
	}
	else
	{
	    // ISE trade worse than BBO - ignore-away customer, firm/farm, or local view of BBO lagging IMP_SRV
	    strcpy ( compare_s, "ISE worse");
	}

	fprintf ( tradefile_px, "ISE, trade, %s, %u, %s, %s, bid, %s, ask, %s, %s, %s, status %s, status %s, source %u\n",
		    found_series_px->ise_name_s,
		    qty_u,
		    &deal_price_s,
		    &compare_s,
		    &bid_premium_s,
		    &ask_premium_s,
		    exchange_to_string ( click_px,
					    found_series_px->opra_best_bid_exchange_u),
		    exchange_to_string ( click_px,
					    found_series_px->opra_best_ask_exchange_u),
		    away_market_status_to_string(found_series_px->opra_best_bid_exchange_status_u),
		    away_market_status_to_string(found_series_px->opra_best_ask_exchange_status_u),
		    bcast_px->deal_source_c
		);
	fflush ( tradefile_px );
    };

    // and now display the components
    for ( deal_item_u = 0;
	    deal_item_u < bcast_px->items_c;
	    deal_item_u++ )
    {
	// grab qty for one trade
	PUTLONG ( qty_u, bcast_px->item [deal_item_u].quantity_u );
	// point structure pointer at the 64bit order ID
	convOrderNo = (quad_conv_t *) &(bcast_px->item [deal_item_u].order_number_u);
	printf ( "\t%s %u, orderID %08lX:%0l8X\n",
		    bid_ask_to_string ( bcast_px->item [deal_item_u].bid_or_ask_c ),
		    qty_u,
		    convOrderNo->l2,
			convOrderNo->l1
		 );
    }	// for loop, displaying deal components

    return (status_i);
}   // handle_bcast_BD111

// -------------------------------------------------------------------------

static int32 handle_bcast_BI8 (click_details_xt *click_px,
				    general_message_t *bcast_px, 
				    uint32 bcast_len_u)
/*
Function : handle_bcast_BI8
Author	 : Neil Cossor 
Modified : 2001229
Description:

    Displays the information contained within the General Message broadcast
*/
{
    free_text_80_st	Buffer_s;
    uint16		item_index_u,	// local loop var
			item_limit_u;	// limit on loop - may be endian-swapped from response
    int32		status_i    =	SUCCESSFUL;


    // log our response
    log_bcast ( LOG_K_B_GENERAL_TEXT,
		bcast_px,
		bcast_len_u );

    // no Bcast sequence number in OM 16.3+

    // debug
    printf ("Broadcast BI8: market message (%d bytes), text:\n",
		bcast_len_u );

    // get item count in correct endian order
    PUTSHORT (item_limit_u, bcast_px->items_n );

    for ( item_index_u = 0; item_index_u < item_limit_u; item_index_u++ )
    {
	// extract the string - allowing for no NUL terminator etc
	rx_click_string ( &Buffer_s,
			    &bcast_px->item[item_index_u].text_line_s,
			    sizeof (bcast_px->item[item_index_u].text_line_s) );
	printf ("%s\n",
		(char *)&Buffer_s );
    }	// for loop, extracting the lines of text supplied by central system

    return (status_i);
}   // handle_bcast_BI8

// -------------------------------------------------------------------------

static int32 handle_bcast_BI9(click_details_xt * click_px,
				 info_heartbeat_t *bcast_px,
				 uint32 bcast_len_u )
/*
Function : handle_bcast_BI9
Author	 : Neil Cossor
Modified : 9905xx
Description:

    got a heartbeat msg - process it

    We really need to note the time of this bcast, and separately be
    reporting when we don't get one of these for a period of time,
    and when they start arriving again

    Note that there may be several heartbeat sources.
    An example is if there are multiple MP servers.

    We really need to track the health of the heartbeat flow from
    all sources.
*/
{
    int32	  status_i  = SUCCESSFUL;

/* debug - manually disable heartbeat logging
    Later on will have intelligent mechanism to determine which
    things to log, which to not
    log_bcast ( LOG_K_B_INFO_HEARTBEAT,
		bcast_px,
		bcast_len_u );
    printf_time();
    printf ("BI9 heartbeat, interval %u (%u of %u)\n",
	    bcast_px->heartbeat_interval_c,
	    bcast_px->instance_c,
	    bcast_px->tot_instances_c );
*/

    return (status_i);
}   // handle_bcast_BI9

// -------------------------------------------------------------------------

static int32 handle_bcast_BI41(click_details_xt * click_px,
				 instrument_status_info_t *bcast_px,
				 uint32 bcast_len_u )
/*
Function : handle_bcast_BI41
Author	 : Neil Cossor
Modified : 20010304
Description:

    got a instrument status broadcast msg - process it

    The broadcast can contain several items, each one affecting one change of session state.

    Each state change can affect
    - the market
    - one instrument type
    - one instrument class
*/
{
    int32		    status_i  = SUCCESSFUL;
    uint16		    item_index_u,	// local loop var
			    item_limit_u;	// limit on loop - may be endian-swapped from response
    // get the trading state #
    uint16		    trade_state_number_u;
    // how long (in minutes, in local endian order) till the event
    uint16		    event_delay_u;
    // local buffer, to put in the 'message string'
    free_text_80_st	    warning_msg_s;
    // local copy of the 'series' we got the info for, in local byte-order etc
    ise_series_xt	ise_series_x;

    // log the message
    log_bcast ( LOG_K_B_INSTRUMENT_STATUS,
		bcast_px,
		bcast_len_u );

    // get the count of items in the bcast
    PUTSHORT ( item_limit_u, bcast_px->items_n );

    // loop around, extracting update components
    for (item_index_u =0;
	    item_index_u < item_limit_u;
	    item_index_u++)
    {
	// grab the trading state number in local-endian order
	PUTSHORT ( trade_state_number_u, bcast_px->item [item_index_u].state_number_n );
	// grab all the stuff out of the series.. will contain some wildcards
	get_click_series ( &ise_series_x,
			    &bcast_px->item [item_index_u].series );
	// if the warning period (in minutes) is zero, then actual change, otherwise warning
	PUTSHORT ( event_delay_u, 
		    bcast_px->item [item_index_u].minutes_to_state_change_n );
	if ( event_delay_u == 0 )
	{
	    // actual state change

	    // update saved data
	    update_trading_state ( click_px,
				    &ise_series_x,
				    trade_state_number_u );

	}
	else
	{
	    // warning of impending state change
	    rx_click_string ( warning_msg_s,
				bcast_px->item [item_index_u].warning_msg_s,
				sizeof ( bcast_px->item [item_index_u].warning_msg_s ));
	    printf ("%s event in %u minutes, [%s]\n",
			series_wildcard_to_string ( click_px,
						    &ise_series_x ),
			event_delay_u,
			&warning_msg_s );
	}   // test if warning message, or actual state change
    }	// for loop

    return (status_i);
}   // handle_bcast_BI41

// -------------------------------------------------------------------------

static int32 handle_bcast_BI103(click_details_xt *click_px,
				series_status_change_bi103_t *bcast_px,
				uint32 bcast_len_u )
/*
Function : handle_bcast_BI103
Author	 : Neil Cossor
Modified : 20010307
Description:

    got a Series Status Change msg - process it
*/
{
    int32		    status_i  = SUCCESSFUL;
    ise_series_xt	    ise_series_x;	// temp, the series in ISE form
    ise_series_info_pt	    found_series_px;	// pointer to info about a series
    uint16		    item_index_u,	// local loop var
			    item_limit_u;	// limit on loop - may be endian-swapped from response
    series_premium_st	    open_premium_s;	// the opening price
    ise_underlying_xt	    *under_data_px;	// pointer to info about one underlying
    gen_buf_node_xt	    *series_node_px;	// pointer to chained item, which contains pointer to info about one series

    // log our response
    log_bcast ( LOG_K_B_SERIES_STATUS_CHG,
		bcast_px,
		bcast_len_u );

    // ignore the 'series' in the header.. just pay attention to the components
    // how many components are there in the msg - endian
    PUTSHORT ( item_limit_u, bcast_px->items_n );

    // loop around, extracting update components
    for (item_index_u =0;
	    item_index_u < item_limit_u;
	    item_index_u++)
    {
	// get the series, in "normal" form
	get_click_series ( &ise_series_x,
			    &bcast_px->item[item_index_u].series );
	// NOTE - BI103 may be specifying an underlying, and NOT a specific series,
	// which means that the change affects all series under that underlying.
	if ( ise_series_x.strike_price_i == 0 )
	{
	    // we are talking about a status change of all series in one underlying

	    // in this case, opening price, volume should not be affected

	    // see if we can find the info about this underlying (commodity code)
	    under_data_px = find_under_data_by_commodity ( click_px,
							    ise_series_x.underlying_code_u );

	    if ( under_data_px )
	    {
		// found the underlying.. need to update the status of all series for this underlying

		// point to node for one series
		series_node_px = (gen_buf_node_xt *)under_data_px->under_series_hdr_x.chain.next;
		while ( series_node_px )
		{
		    // point to actual data
		    found_series_px = (ise_series_info_xt *)series_node_px->msg_p;

		    // set the status
		    found_series_px->series_status_u =
			bcast_px->item[item_index_u].series_status_c;
		    // if it is fast market, also need to save the FM level.. just save it anyway, irrelevant unless FM
		    PUTSHORT ( found_series_px->fast_market_level_u,
				bcast_px->item[item_index_u].fast_market_level_n );

		    // advance along the chain of series for the underlying
		    series_node_px = (gen_buf_node_xt *)series_node_px->chain.next;
		}   // loop around series within one underlying
	    }
	    else
	    {
		// no success
		printf ("ERROR :- BI103 Series Status Change specified commodity %u, unknown\n",
			    ise_series_x.underlying_code_u );
	    }	// test if we found the specific underlying
	}
	else
	{
	    // we are talking about a specific series

	    // find the record with our data about the series
	    found_series_px = find_series_by_ise_series ( click_px,
							    &ise_series_x );
	    // we SHOULD know about this series
	    if ( found_series_px )
	    {
		// found the ISEsim data - update it
		PUTLONG ( found_series_px->opening_price_i,
			    bcast_px->item[item_index_u].opening_price_i );
		PUTLONG ( found_series_px->volume_u,
			    bcast_px->item[item_index_u].volume_u );
		found_series_px->series_status_u =
		    bcast_px->item[item_index_u].series_status_c;
		PUTSHORT ( found_series_px->fast_market_level_u,
			    bcast_px->item[item_index_u].fast_market_level_n );

		// reformat the open premium for display, with appropriate implied decimals
		format_whole_decimal_s ( (char *)&open_premium_s,
					    found_series_px->opening_price_i,
					    found_series_px->instrument_class_px->dec_in_premium_u,
					    MAX_SERIES_PREMIUM );	// allowed width
		if ( found_series_px->series_status_u == SERIES_STATUS_FAST_MARKET )
		    // in Fast Market
		    printf ( "Bcast BI103 : Series Status Change, series %s, price %s, vol %u, status %s, level %d -> interval %d ms\n",
				&found_series_px->ise_name_s,
				&open_premium_s,
				found_series_px->volume_u,
				series_status_to_string ( found_series_px->series_status_u),
				found_series_px->fast_market_level_u,
				found_series_px->parent_under_px->fast_level_x[found_series_px->fast_market_level_u].match_interval_i
			    );
		else
		    // some other state
		    printf ( "Bcast BI103 : Series Status Change, series %s, price %s, vol %u, status %s\n",
				&found_series_px->ise_name_s,
				&open_premium_s,
				found_series_px->volume_u,
				series_status_to_string ( found_series_px->series_status_u)
			    );
	    }
	    else
	    {
		// unknown series

		printf ( "Error - Broadcast BI103 : Series Status Change, for unknown series\n" );
	    }	// test if we found the info about a specific series
	}   // test if series qualified a strike i.e. for one underlying or one series
    }	// for loop, updating the series status of multiple series

    return (status_i);
}   // handle_bcast_BI103

// -------------------------------------------------------------------------

static int32 handle_bcast_BI114 ( click_details_xt *click_px,
				    away_market_flag_update_us_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BI114
Author	 : Neil Cossor
Modified : 9905xx
Description:

    got an Away Market Check msg - process it
*/
{
    int32		status_i  = SUCCESSFUL;
    uint16		item_count_u,
			item_index_u,
			commodity_code_u;
    ise_underlying_xt	*found_under_data_px;

    // log our response
    log_bcast ( LOG_K_B_AWAY_MARKET_CHECK,
		bcast_px,
		bcast_len_u );

    // how many items in the bcast
    PUTSHORT ( item_count_u, bcast_px->items_n );

    // debug
    printf ("Bcast BI114: Away Market Check\n" );
    for ( item_index_u = 0;
	    item_index_u < item_count_u;
	    item_index_u++ )
    {
	// extract underlying code
	PUTSHORT ( commodity_code_u, bcast_px->item [ item_index_u ].commodity_n );
	// find the underlying record
	found_under_data_px = find_under_data_by_commodity ( click_px,
								commodity_code_u );
	if ( !found_under_data_px )
	{
	    printf ("Unknown underlying. code %hu\n",
			commodity_code_u );
	}
	else
	{
	    printf ( "%s exchange, underlying %s, ignore-away %u\n",
			exchange_to_string ( click_px,
						bcast_px->item [item_index_u].country_c ),
			&found_under_data_px->under_id_s,
			bcast_px->item[item_index_u].ignore_away_market_c );
	}   // test if we knew about that underlying
    }	// for loop, processing components of msg

    return (status_i);
}   // handle_bcast_BI114

// -------------------------------------------------------------------------

static int32 handle_bcast_BI132 ( click_details_xt *click_px,
				    underlying_trading_status_bi132_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BI132
Author	 : Neil Cossor
Modified : 20000413
Description:

    got an Underlying Trading Status msg - process it
*/
{
    int32	  status_i  = SUCCESSFUL;

    // the commodity code of underlying in the bcast
    uint16		found_commodity_code_u;
    // the underlying record for the underlying
    ise_underlying_xt	*found_under_data_px;


    // log our response
    log_bcast ( LOG_K_B_UNDER_STATUS_CHG,
		bcast_px,
		bcast_len_u );

    /* update the common with the data in the bcast
	NB potential locking problem.. as we are just updating numbers,
	shouldn't be a problem
    */
    PUTSHORT ( found_commodity_code_u,
		bcast_px->commodity_n );
    // find the underlying record
    found_under_data_px = find_under_data_by_commodity ( click_px,
						    found_commodity_code_u );

    if ( found_under_data_px == NULL)
    {
	printf ("Got status change for unknown commodity %u\n",
		    found_commodity_code_u );
	return status_i;
    }

    // and update the status
    found_under_data_px->trading_status_u = bcast_px->underlying_trading_status_c;

    // debug
    if ( found_under_data_px )
    {
	printf ("BI132 Bcast : Underlying Trading Status for %s, now %s\n",
		&found_under_data_px->under_id_s,
		underlying_status_to_string ( found_under_data_px->trading_status_u) );
    }
    else
    {
	printf ("Warning - received unknown commodity code %hu in bi132\n",
		found_commodity_code_u );
    }

    return (status_i);
}   // handle_bcast_BI132

// -------------------------------------------------------------------------

static int32 handle_bcast_BI202 ( click_details_xt *click_px,
				    series_update_bi202_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BI202
Author	 : Neil Cossor
Modified : 9905xx
Description:

    got a Series Update msg - process it
*/
{
    int32		status_i  = SUCCESSFUL;
    // the type of change msg - add, delete, or modify
    uint16		change_type_u;
    // temp, the local series name
    series_st		full_series_name_s;
    // temp, the symbol name
    symbol_name_st	symbol_name_s;
    // copy of the series we are updating
    ise_series_xt	series_x;
    // pointer to the main ISEsim data for the series we are playing with
    ise_series_info_xt	*ise_series_info_px;
    // temp, ptr to Q'ed block, containing ptr to data about one series, default NULL
    gen_buf_node_xt	* series_px = NULL;

    // log our response
    log_bcast ( LOG_K_B_SERIES_UPDATE,
		bcast_px,
		bcast_len_u );

    // grab the change type - 2 byte, so endian
    PUTSHORT ( change_type_u, bcast_px->chg_type_n );
    get_click_series ( &series_x, &bcast_px->da202.series );
    // grab the short and full name of the underlying
    rx_click_string ( &full_series_name_s,
			&bcast_px->da202.ins_id_s,
			sizeof (bcast_px->da202.ins_id_s ));
    rx_click_string ( &symbol_name_s,
			&bcast_px->da202.symbol_s,
			sizeof (bcast_px->da202.symbol_s));
    // common output part of display debug info
    printf ( "Bcast BI202: Series Update, %s series, symbol %s, [%s]\n",
		change_type_to_string (change_type_u),
		&symbol_name_s,
		&full_series_name_s );

    // grab the global - we are about to change it
    lock_global ( click_px );

    switch ( change_type_u )
    {
	case CHANGE_TYPE_ADD:
	    // adding a new series
	    // hopefully done after we add the instrument classes and underlying that this uses

	    // allocate space to store info about one underlying - control part first
	    series_px = new_gen_buf_node (sizeof(ise_series_info_xt));
	    // point to the actual data area
	    (char *)ise_series_info_px = series_px->msg_p;

	    // fill in the contents of our ISEsim internal structure with info from msg
	    update_one_series ( click_px,
				ise_series_info_px,
				&bcast_px->da202 );

	    // and add to our chain of known series.
	    add_to_q_tail ( &click_px->our_market_x.known_series_q,
			    &series_px->chain );

	    break;
	case CHANGE_TYPE_DELETE:
	    // deleting an existing series
	    // hopefully the series is deleted before the instrument classes and underlying

	    series_px = find_series_block_by_ise_series ( click_px,
							    &series_x );
	    if ( !series_px )
	    {
		printf ("Warning, delete refers to unknown series\n" );
		break;
	    }

	    // remove the node from the known series q
	    remove_buff_from_q ( &click_px->our_market_x.known_series_q,
				    &series_px->chain );

	    break;
	case CHANGE_TYPE_MODIFY:
	    // changing attributes of an existing series
	    // should NOT impact the underlying and instrument classes

	    /* question here is.. what aspects of an existing series can change.
		The answer determines how we find the existing underlying data to update
	    */
	    ise_series_info_px = find_series_by_ise_series ( click_px,
								&series_x );
	    if ( !ise_series_info_px )
	    {
		printf ("Warning, update refers to unknown series\n" );
		break;
	    }

	    // fill in the contents of our ISEsim internal structure with info from msg
	    update_one_series ( click_px,
				ise_series_info_px,
				&bcast_px->da202 );

	    break;
	default:
	    printf ("Error - unknown change type in BI202, %hu\n",
			change_type_u );
	    break;
    }	// switch on change type

    // update the series sorted tables.. after discarding the old ones
    sort_series_tables ( &click_px->our_market_x, TRUE );

    // backlinks from all series to 'describing' instrument class already done in updating the series

    // update links from all series to parent underlyings,
    // and attach list of series to each underlying
    // 3 param TRUE says discard old pointer data
    point_series_at_underlying ( click_px,
				    &click_px->our_market_x,
				    TRUE );

    // release the global - we are done with changes
    unlock_global ( click_px );

    return (status_i);
}   // handle_bcast_BI202

// -------------------------------------------------------------------------

static int32 handle_bcast_BI204 ( click_details_xt *click_px,
				    underlying_update_bi204_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BI204
Author	 : Neil Cossor
Modified : 9905xx
Description:

    got an Underlying Update msg - process it
*/
{
    int32	status_i  = SUCCESSFUL;
    // the type of change msg - add, delete, or modify
    uint16	change_type_u,
    // the commodity code in the msg
		commodity_code_u;

    // the local exchange code for the underlying - i.e. short name
    under_id_st under_id_s;
    // == name_s, full name of the underlying
    name_st	under_fullname_s;
    // temp, ptr to Q'ed block, containing ptr to data about one underlying, default NULL
    gen_buf_node_xt		* underlying_px = NULL;
    // temp, ptr to an ISEsim block of data about one underlying
    ise_underlying_xt		* ise_underlying_px;


    // log our response
    log_bcast ( LOG_K_B_UNDER_UPDATE,
		bcast_px,
		bcast_len_u );

    // grab the change type - 2 byte, so endian
    PUTSHORT ( change_type_u, bcast_px->chg_type_n );
    PUTSHORT ( commodity_code_u, bcast_px->da204.commodity_n );
    // grab the short and full name of the underlying
    rx_click_string ( &under_id_s,
			&bcast_px->da204.com_id_s,
			sizeof (bcast_px->da204.com_id_s ));
    rx_click_string ( &under_fullname_s,
			&bcast_px->da204.name_s,
			sizeof (bcast_px->da204.name_s));
    // common output part of display debug info
    printf ( "Broadcast BI204: Underlying Update, %s underlying, symbol %s, [%s]\n",
		change_type_to_string (change_type_u),
		&under_id_s,
		&under_fullname_s );

    // grab the global - we are about to change it
    lock_global ( click_px );

    switch ( change_type_u )
    {
	case CHANGE_TYPE_ADD:
	    // adding a new underlying
	    // hopefully done before we add the instrument classes and series that use this

	    // allocate space to store info about one underlying - control part first
	    underlying_px = new_gen_buf_node (sizeof(ise_underlying_xt));
	    // allocate enuff space for the the actual data
	    (char *)ise_underlying_px = underlying_px->msg_p;

	    // fill in the contents of our ISEsim internal structure with info from msg
	    update_one_underlying ( click_px,
				    ise_underlying_px,
				    &bcast_px->da204 );

	    // and add to our chain of known underlyings.
	    add_to_q_tail ( &click_px->our_market_x.known_underlying_q,
			    &underlying_px->chain );

	    break;
	case CHANGE_TYPE_DELETE:
	    // deleting an existing underlying
	    // hopefully the series and instrument classes that use this are already gone

	    underlying_px = find_under_block_by_commodity ( click_px,
							    commodity_code_u );
	    if ( !underlying_px )
	    {
		printf ("Warning, delete refers to unknown commodity %hu\n",
			    commodity_code_u );
		break;
	    }

	    // remove the node from the known underlying q
	    remove_buff_from_q ( &click_px->our_market_x.known_underlying_q,
				    &underlying_px->chain );

	    break;

	case CHANGE_TYPE_MODIFY:
	    // changing attributes of an existing underlying
	    // should NOT impact the series and instrument classes

	    /* question here is.. what aspects of an existing underlying can change.
		Can the short name or commodity code of an underlying change... ?
		The answer determines how we find the existing underlying data to update

	    */
	    ise_underlying_px = find_under_data_by_commodity ( click_px,
								commodity_code_u );
	    if ( !ise_underlying_px )
	    {
		printf ("Warning, update refers to unknown commodity %hu\n",
			    commodity_code_u );
		break;
	    }

	    // and update the old data with the new
	    update_one_underlying ( click_px,
				    ise_underlying_px,
				    &bcast_px->da204 );

	    break;
	default:
	    printf ("Error - unknown change type in BI204, %hu\n",
			change_type_u );
	    break;
    }	// switch on change type

    // update the underlying sorted tables.. after discarding the old ones
    sort_underlying_tables ( &click_px->our_market_x, TRUE );

    // release the global - we are done with changes
    unlock_global ( click_px );

    return (status_i);
}   // handle_bcast_BI204

// -------------------------------------------------------------------------

static int32 handle_bcast_BI210 ( click_details_xt *click_px,
				    instrument_class_update_bi210_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BI210
Author	 : Neil Cossor
Modified : 9905xx
Description:

    got an Instrument Class Update msg - process it
*/
{
    int32	  status_i  = SUCCESSFUL;
    // the type of change msg - add, delete, or modify
    uint16		change_type_u,
    // the commodity code of the instrument class
			commodity_code_u;
    // the instrument group #
    uint8		instr_group_u;
    // temp, ptr to an ISEsim block of data about one underlying
    ise_underlying_xt	* ise_underlying_px;
    // copy of the series we are updating.. actually only contains instr class : country, market, commodity, instr group
    ise_series_xt	series_x;
    // copy of the instrument class we are updating
    instrument_class_xt	* ise_instr_class_info_px;
    // temp, ptr to Q'ed block, containing ptr to data about one instr class, default NULL
    gen_buf_node_xt	* instr_node_px = NULL;

    // log our response
    log_bcast ( LOG_K_B_INSTR_CLASS_UPDATE,
		bcast_px,
		bcast_len_u );

    // grab the change type - 2 byte, so endian
    PUTSHORT ( change_type_u, bcast_px->chg_type_n );
    // grab the commodity code of the instr class - 2 byte, endian
    PUTSHORT ( commodity_code_u, bcast_px->da210.series.commodity_n );
    // grab the instrument group of the instr class - 1 byte, not endian
    instr_group_u = bcast_px->da210.series.instrument_group_c;
    // extract all the fields that define the instrument class
    get_click_series ( &series_x, &bcast_px->da210.series );
    // find the relevant underlying
    ise_underlying_px = find_under_data_by_commodity ( click_px,
							commodity_code_u );
    if ( !ise_underlying_px )
    {
	printf ("Warning, update refers to unknown commodity %hu\n",
		    commodity_code_u );
	return INVALID_ARGUMENTS;
    }

    // common output part of display debug info
    printf ( "Bcast BI210: Instrument Class Update, %s underlying, instr group %s, [%s]\n",
		change_type_to_string (change_type_u),
		&ise_underlying_px->under_id_s,
		instr_group_name (click_px,
				    instr_group_u)
	    );

    // grab the global - we are about to change it
    lock_global ( click_px );

    switch ( change_type_u )
    {
	case CHANGE_TYPE_ADD:
	    // adding a new series
	    // hopefully done after we add the instrument classes and underlying that this uses

	    // allocate space to store info about one instrument class record
	    instr_node_px = new_gen_buf_node (sizeof (instrument_class_xt));
	    // grab typecast copy of pointer into dest storage for instrument class details
	    (char *)ise_instr_class_info_px = instr_node_px->msg_p;

	    // fill in the contents of our ISEsim internal structure with info from msg
	    update_one_instrument_class ( ise_instr_class_info_px,
					    &bcast_px->da210 );

	    // and add to our chain of known series.
	    add_to_q_tail ( &click_px->our_market_x.known_classes_q,
			    &instr_node_px->chain );

	    break;
	case CHANGE_TYPE_DELETE:
	    // deleting an existing series
	    // hopefully the series is deleted before the instrument classes and underlying

	    instr_node_px = find_instrument_class_block_by_series ( &click_px->our_market_x,
								    &series_x );
	    if ( !instr_node_px )
	    {
		printf ("Warning, delete refers to unknown underlying / instr group\n" );
		break;
	    }

	    // remove the node from the known series q
	    remove_buff_from_q ( &click_px->our_market_x.known_classes_q,
				    &instr_node_px->chain );

	    break;
	case CHANGE_TYPE_MODIFY:
	    // changing attributes of an existing series
	    // should NOT impact the underlying and instrument classes

	    /* question here is.. what aspects of an existing series can change.
		The answer determines how we find the existing underlying data to update
	    */
	    ise_instr_class_info_px = find_instrument_class_by_series ( &click_px->our_market_x,
									&series_x );
	    if ( !ise_instr_class_info_px )
	    {
		printf ("Warning, update refers to unknown commodity/instr group\n" );
		break;
	    }

	    // fill in the contents of our ISEsim internal structure with info from msg
	    update_one_instrument_class ( ise_instr_class_info_px,
					    &bcast_px->da210 );

	    break;
	default:
	    printf ("Error - unknown change type in BI210, %hu\n",
			change_type_u );
	    break;
    }	// switch on change type

    // update the instrument class sorted tables.. after discarding the old ones
    sort_instrument_classes ( &click_px->our_market_x, TRUE );

    // release the global - we are done with changes
    unlock_global ( click_px );

    return (status_i);
}   // handle_bcast_BI210

// -------------------------------------------------------------------------

static int32 handle_bcast_BN1 ( click_details_xt *click_px,
				    omni_broadcast *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BN1
Author	 : Neil Cossor
Modified : 991104
Description:

    got an Network broadcast msg - process it
    In all cases, something has gone wrong
*/
{
    // copy of the string buffer from the bcast, but with NUL termination
#define MAX_REPORT_STRING 128
//    char	    temp_string_s [MAX_REPORT_STRING+1];
    // size of the # of bytes in the bcast, in local-endian order
    uint16	    string_len_u;
    // network status, in local endian order
    uint32	    status_i  = SUCCESSFUL;
    // pointer to network report string
    char	    * net_report_ps;
    // how we report the particular network statii
    static char	    *net_status_s [] =
    {
	"Data lost (at least 1 bcast missed)",
	"Link lost with bcast server",
	"Bcast receiver offline to sender",
	"Unknown status"
    };



    // log our response
    log_bcast ( LOG_K_B_NETWORK_STATUS,
		bcast_px,
		bcast_len_u );

    // grab data from the msg, in local endian order etc
    PUTSHORT ( string_len_u, bcast_px->network_status_x.msglen_n );
    PUTLONG ( status_i, bcast_px->network_status_x.status_u );
    // limit how much string we report to amount of text buffer space we grabbed
    if ( string_len_u > MAX_REPORT_STRING )
	string_len_u = MAX_REPORT_STRING;

	//COMMENTING THIS OUT - DOESNT EXIST IN 17.1  -E. Salem
    // and grab the string from the msg, adding a terminating NUL if necessary
    //rx_click_string ( &temp_string_s,
	//		&bcast_px->network_status_x.message_s,
	//		string_len_u );

    // NOTE that all BN1 message mean that we have a problem.
    // For now, return this bad status back, which should mean that we attempt
    // to do a logoff / clean processing etc
    switch ( status_i )
    {
    case OMNI_DU_DATALOST:
	net_report_ps = net_status_s[0];
	break;
    case OMNI_DU_LINKLOST:
	net_report_ps = net_status_s[1];
	break;
    case OMNI_DU_SENDERLOST:
	net_report_ps = net_status_s[2];
	break;
    default:
	net_report_ps = net_status_s[3];
	break;
    }	// on supplied status

    // debug
    printf_time ();
    printf ( "Broadcast BN1: Network status alert, status %u [%#10x], %s\n",
		status_i,
		status_i,
		(net_report_ps));
		//&temp_string_s ); no longer in 17.1

    // 20000612 - try returning a SUCCESSFUL, so broadcast thread does NOT terminate session
    // if BN1 is from back-end failure, then not our fault / problem
    // if BN1 is forced disconnect, then next broadcast read / tx / query should kill session anyway
    return SUCCESSFUL;

    return (status_i);
}   // handle_bcast_BN1

// -------------------------------------------------------------------------

static int32 handle_bcast_BO101 ( click_details_xt *click_px,
				    best_bid_offer_bo101_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BO101
Author	 : Neil Cossor
Modified : 20000515
Description:
    got an Best Bid Offer msg - process it
*/
{
    int32	  status_i  = SUCCESSFUL;

    ise_series_xt	    ise_series_x;	// temp, the series in ISE form
    ise_series_info_pt	    found_series_px;	// pointer to info about a series
    uint16		    item_index_u,	// local loop var
			    item_limit_u;	// limit on loop - may be endian-swapped from response
    // the string form of a premiums, with appropriate # of decimal places
    series_premium_st	    ask_premium_s,
			    bid_premium_s;


    // log our response
    log_bcast ( LOG_K_B_BEST_BID_OFFER,
		bcast_px,
		bcast_len_u );

    // ignore the 'series' in the header.. just pay attention to the components
    // how many components are there in the msg - endian
    PUTSHORT ( item_limit_u, bcast_px->items_n );

    // loop around, extracting update components
    for (item_index_u =0;
	    item_index_u < item_limit_u;
	    item_index_u++)
    {
	// get the series, in "normal" form
	get_click_series ( &ise_series_x,
			    &bcast_px->item[item_index_u].series );
	// find the record with our data about the series
	found_series_px = find_series_by_ise_series ( click_px,
							&ise_series_x );
	// we SHOULD know about this series
	if ( found_series_px )
	{
	    // found the ISEsim data - update it

	    // note when we got this info
	    found_series_px->time_bbo_update_u = time(NULL);

	    PUTLONG ( found_series_px->best_ask_premium_i,
			bcast_px->item[item_index_u].best_ask_premium_i );
	    PUTLONG ( found_series_px->best_bid_premium_i,
			bcast_px->item[item_index_u].best_bid_premium_i );
	    PUTLONG ( found_series_px->best_ask_size_u,
			bcast_px->item[item_index_u].best_ask_size_u );
	    PUTLONG ( found_series_px->best_bid_size_u,
			bcast_px->item[item_index_u].best_bid_size_u );

	    // reformat the premium for display, with appropriate implied decimals
	    format_whole_decimal_s ( (char *)&ask_premium_s,
					found_series_px->best_ask_premium_i,
					found_series_px->instrument_class_px->dec_in_premium_u,
					MAX_SERIES_PREMIUM );	// allowed width
	    // reformat the premium for display, with appropriate implied decimals
	    format_whole_decimal_s ( (char *)&bid_premium_s,
					found_series_px->best_bid_premium_i,
					found_series_px->instrument_class_px->dec_in_premium_u,
					MAX_SERIES_PREMIUM );	// allowed width

	    printf ( "BO101 BBO bcast, series %s, bid %u @%s, ask %u @%s\n",
			&found_series_px->ise_name_s,
			found_series_px->best_bid_size_u,
			&bid_premium_s,
			found_series_px->best_ask_size_u,
			&ask_premium_s );
	}
	else
	{
	    // unknown series

	    printf ( "Error - Broadcast BO101 : BBO, for unknown series\n" );
	}
    }	// for loop, updating the series status of multiple series

    return (status_i);
}   // handle_bcast_BO101

// -------------------------------------------------------------------------

static int32 handle_bcast_BO102 ( click_details_xt *click_px,
				    best_bid_offer_extended_bo102_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BO102
Author	 : Neil Cossor
Modified : 20000505
Description:

    got an Extended Best Bid Offer msg - process it
*/
{
    int32	  status_i  = SUCCESSFUL;
    ise_series_xt	    ise_series_x;	// temp, the series in ISE form
    ise_series_info_pt	    found_series_px;	// pointer to info about a series
    uint16		    item_index_u,	// local loop var
			    item_limit_u;	// limit on loop - may be endian-swapped from response
    // the string form of a premiums, with appropriate # of decimal places
    series_premium_st	    ask_premium_s,
			    bid_premium_s;


    // log our response
    log_bcast ( LOG_K_B_EXT_BEST_BID_OFFER,
		bcast_px,
		bcast_len_u );

    // ignore the 'series' in the header.. just pay attention to the components
    // how many components are there in the msg - endian
    PUTSHORT ( item_limit_u, bcast_px->items_n );

    // loop around, extracting update components
    for (item_index_u =0;
	    item_index_u < item_limit_u;
	    item_index_u++)
    {
	// get the series, in "normal" form
	get_click_series ( &ise_series_x,
			    &bcast_px->item[item_index_u].series );
	// find the record with our data about the series
	found_series_px = find_series_by_ise_series ( click_px,
							&ise_series_x );
	// we SHOULD know about this series
	if ( found_series_px )
	{
	    // found the ISEsim data - update it

	    // note when we got this info
	    found_series_px->time_bbo_update_u = time(NULL);

	    PUTLONG ( found_series_px->best_ask_premium_i,
			bcast_px->item[item_index_u].best_ask_premium_i );
	    PUTLONG ( found_series_px->best_bid_premium_i,
			bcast_px->item[item_index_u].best_bid_premium_i );
	    PUTLONG ( found_series_px->best_ask_size_u,
			bcast_px->item[item_index_u].best_ask_size_u );
	    PUTLONG ( found_series_px->best_bid_size_u,
			bcast_px->item[item_index_u].best_bid_size_u );
	    // and the extra fields that are in the BO102
	    PUTLONG ( found_series_px->bid_market_order_size_u,
			bcast_px->item[item_index_u].bid_market_order_size_u );
	    PUTLONG ( found_series_px->ask_market_order_size_u,
			bcast_px->item[item_index_u].ask_market_order_size_u );
	    PUTSHORT ( found_series_px->mm_number_u,
			bcast_px->item[item_index_u].mm_number_n );

	    // reformat the premium for display, with appropriate implied decimals
	    format_whole_decimal_s ( (char *)&ask_premium_s,
					found_series_px->best_ask_premium_i,
					found_series_px->instrument_class_px->dec_in_premium_u,
					MAX_SERIES_PREMIUM );	// allowed width
	    // reformat the premium for display, with appropriate implied decimals
	    format_whole_decimal_s ( (char *)&bid_premium_s,
					found_series_px->best_bid_premium_i,
					found_series_px->instrument_class_px->dec_in_premium_u,
					MAX_SERIES_PREMIUM );	// allowed width

	    printf ( "BO102 BBO bcast, series %s, bid %u @%s, ask %u @%s\n",
			&found_series_px->ise_name_s,
			found_series_px->best_bid_size_u,
			&bid_premium_s,
			found_series_px->best_ask_size_u,
			&ask_premium_s );
	    printf ( "\tMarket bid %u, market ask %u, %hu market makers\n",
			found_series_px->bid_market_order_size_u,
			found_series_px->ask_market_order_size_u,
			found_series_px->mm_number_u );
	}
	else
	{
	    // unknown series

	    printf ( "Error - Broadcast BO102 : extended BBO, for unknown series\n" );
	}
    }	// for loop, updating the series status of multiple series

    return (status_i);
}   // handle_bcast_BO102

// -------------------------------------------------------------------------

static int32 handle_bcast_BO103 ( click_details_xt *click_px,
				    opra_best_bid_offer_bo103_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BO103
Author	 : Neil Cossor
Modified : 20010304
Description:

    got an OPRA Best Bid Offer msg - process it
*/
{
    int32		    status_i  = SUCCESSFUL;
    uint16		    item_count_u,   // how many BBO items in the msg
			    item_index_u;   // index var of which item we are examining
    ise_series_xt	    ise_series_x;	// temp, the series in ISE form
    ise_series_info_pt	    found_series_px;	// pointer to info about a series
    // the string form of a premiums, with appropriate # of decimal places
    series_premium_st	    ask_premium_s,
			    bid_premium_s;
    // pointer to one opra item in the broadcast
    opra_best_bid_offer_bo103_item_t	*txn_reply_px;

    // log our response
    log_bcast ( LOG_K_B_OPRA_BEST_BID_OFFER,
		bcast_px,
		bcast_len_u );

    // debug - spit out some output onto the screen

    // how many opra BBO components in the msg
    PUTSHORT ( item_count_u, bcast_px->items_n );
    printf ( "Broadcast BO103: OPRA Best Bid Offer\n" );
    for ( item_index_u = 0;
	    item_index_u < item_count_u;
	    item_index_u++ )
    {
	txn_reply_px = &bcast_px->item [ item_index_u ];

	// get the series, in "normal" form
	get_click_series ( &ise_series_x,
			    &txn_reply_px->series );
	// find the record with our data about the series
	found_series_px = find_series_by_ise_series ( click_px,
							&ise_series_x );
	// we SHOULD know about this series
	if ( found_series_px )
	{
	    // found the ISEsim data - update it.. ask and bid
	    PUTLONG ( found_series_px->opra_best_ask_premium_i,
			txn_reply_px->best_ask_premium_i );
	    PUTLONG ( found_series_px->opra_best_bid_premium_i,
			txn_reply_px->best_bid_premium_i );
	    //PUTLONG ( found_series_px->opra_best_ask_size_u, no longer in 17.1
		//	txn_reply_px->best_ask_size_u );
	    //PUTLONG ( found_series_px->opra_best_bid_size_u,
		//	txn_reply_px->best_bid_size_u );

	    // and which market they are at
	    found_series_px->opra_best_ask_exchange_u = txn_reply_px->country_ask_c;
	    found_series_px->opra_best_bid_exchange_u = txn_reply_px->country_bid_c;

	    // and the status of those markets
	    found_series_px->opra_best_ask_exchange_status_u = txn_reply_px->away_market_status_ask_c;
	    found_series_px->opra_best_bid_exchange_status_u = txn_reply_px->away_market_status_bid_c;

	    // reformat the premium for display, with appropriate implied decimals
	    format_whole_decimal_s ( (char *)&ask_premium_s,
					found_series_px->opra_best_ask_premium_i,
					found_series_px->instrument_class_px->dec_in_premium_u,
					MAX_SERIES_PREMIUM );	// allowed width
	    // reformat the premium for display, with appropriate implied decimals
	    format_whole_decimal_s ( (char *)&bid_premium_s,
					found_series_px->opra_best_bid_premium_i,
					found_series_px->instrument_class_px->dec_in_premium_u,
					MAX_SERIES_PREMIUM );	// allowed width
	    printf ("Series %s, OPRA bid %u@%s, at %s, status %s, OPRA ask %u@%s, at %s, status %s\n",
			&found_series_px->ise_name_s,
			found_series_px->opra_best_bid_size_u,
			&bid_premium_s,
			exchange_to_string ( click_px,
						found_series_px->opra_best_bid_exchange_u),
			away_market_status_to_string(found_series_px->opra_best_bid_exchange_status_u),
			found_series_px->opra_best_ask_size_u,
			&ask_premium_s,
			exchange_to_string ( click_px,
						found_series_px->opra_best_ask_exchange_u),
			away_market_status_to_string(found_series_px->opra_best_ask_exchange_status_u)
		    );
	}
    }	// for loop spitting out items from the msg

    printf ("\n");

    return (status_i);
}   // handle_bcast_BO103

// -------------------------------------------------------------------------

static int32 handle_bcast_BO104 ( click_details_xt *click_px,
				    opra_all_best_bid_offer_bo104_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BO104
Author	 : Neil Cossor
Modified : 20000502
Description:

    got an Each Exchange OPRA Best Bid Offer msg - process it
*/
{
    int32	  status_i  = SUCCESSFUL;
    uint16		    item_count_u,   // how many BBO items in the msg
			    item_index_u;   // index var of which item we are examining
    ise_series_xt	    ise_series_x;	// temp, the series in ISE form
    ise_series_info_pt	    found_series_px;	// pointer to info about a series
    // local endian copy of BBO quantities
//    uint32		    ask_size_u,
//			    bid_size_u;
    // the numeric form of the premiums, in local byte order
    int32		    ask_premium_i,
			    bid_premium_i;
    // the string form of a premiums, with appropriate # of decimal places
    series_premium_st	    ask_premium_s,
			    bid_premium_s;
    // pointer to one opra item in the broadcast
    opra_all_best_bid_offer_bo104_item_t	*txn_reply_px;
    // which exchange an item's BBO is from
    uint8		    country_u;
    // and the status of that exchange
    uint8		    exchange_status_u;



    // log our response
    log_bcast ( LOG_K_B_EACH_BEST_BID_OFFER,
		bcast_px,
		bcast_len_u );

    // debug
    printf ( "Broadcast BO104: Each Exchange OPRA Best Bid Offer\n" );

    // debug - spit out some output onto the screen

    // how many opra BBO components in the msg
    PUTSHORT ( item_count_u, bcast_px->items_n );
    for ( item_index_u = 0;
	    item_index_u < item_count_u;
	    item_index_u++ )
    {
	txn_reply_px = &bcast_px->item [ item_index_u ];

	// get the series, in "normal" form
	get_click_series ( &ise_series_x,
			    &txn_reply_px->series );
	// find the record with our data about the series
	found_series_px = find_series_by_ise_series ( click_px,
							&ise_series_x );
	// we SHOULD know about this series
	if ( found_series_px )
	{
	    // found the ISEsim data - update it.. ask and bid
	    PUTLONG ( ask_premium_i,
			txn_reply_px->best_ask_premium_i );
	    PUTLONG ( bid_premium_i,
			txn_reply_px->best_bid_premium_i );
//	    PUTLONG ( ask_size_u,
//			txn_reply_px->best_ask_size_u ); not in 17.1
//	    PUTLONG ( bid_size_u,
//			txn_reply_px->best_bid_size_u );

	    // and which market they are at
	    country_u = txn_reply_px->country_c;
	    // the status of that exchange
	    exchange_status_u = txn_reply_px->away_market_status_c;

	    // reformat the premium for display, with appropriate implied decimals
	    format_whole_decimal_s ( (char *)&ask_premium_s,
					ask_premium_i,
					found_series_px->instrument_class_px->dec_in_premium_u,
					MAX_SERIES_PREMIUM );	// allowed width
	    // reformat the premium for display, with appropriate implied decimals
	    format_whole_decimal_s ( (char *)&bid_premium_s,
					bid_premium_i,
					found_series_px->instrument_class_px->dec_in_premium_u,
					MAX_SERIES_PREMIUM );	// allowed width
	    printf ("Series %s, OPRA bid @%s, ask @%s, at %s, status %s\n",
			&found_series_px->ise_name_s,
			&bid_premium_s,
			&ask_premium_s,
			exchange_to_string ( click_px,
						country_u ),
			away_market_status_to_string(exchange_status_u)
		    );
	}
    }	// for loop spitting out items from the msg

    printf ("\n");

    return (status_i);
}   // handle_bcast_BO104

// -------------------------------------------------------------------------

static int32 handle_bcast_BO105 ( click_details_xt *click_px,
				    underlying_best_bid_offer_bo105_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BO105
Author	 : Neil Cossor
Modified : 20010304
Description:

    got an Underlying Best Bid Offer msg - process it
*/
{
    int32		status_i  = SUCCESSFUL;
    // the commodity code of underlying in the bcast
    uint16		found_commodity_code_u;
    // the underlying record for the underlying
    ise_underlying_xt	*found_under_data_px;
    // the new bid and ask price
    // need these to figure out if we need to generate autoquote record
    int32		new_best_ask_i,
			new_best_bid_i;
    // pointer to a q'able block, which we can insert into the Q for the autoquoting
    gen_buf_node_xt	*quote_node_px;
    // pointer to the data component of the block we generate for the autoquoting thread
    autoquote_price_change_xt	*quote_data_px;

    // log our response
    log_bcast ( LOG_K_B_CQS_BEST_BID_OFFER,
		bcast_px,
		bcast_len_u );

    /* update the common with the data in the bcast
	NB potential locking problem.. as we are just updating numbers,
	shouldn't be a problem
    */
    PUTSHORT ( found_commodity_code_u,
		bcast_px->series.commodity_n );
    // find the underlying record
    found_under_data_px = find_under_data_by_commodity ( click_px,
							    found_commodity_code_u );

    if ( found_under_data_px == NULL )
    {
	printf ("Warning, got BO105 for unknown commodity code %u\n",
		    found_commodity_code_u );
	return INVALID_ARGUMENTS;
    }

    // grab the bid & ask out of the msg
    PUTLONG ( new_best_ask_i, bcast_px->best_ask_premium_i );
    PUTLONG ( new_best_bid_i, bcast_px->best_bid_premium_i );

    // are we autoquoting for this underlying ?
    if ( found_under_data_px->autoquoting_b )
    {
	// yes, we are autoquoting.. figure out if BBO changed -> need to requote
	if (( new_best_ask_i != found_under_data_px->best_ask_premium_i)
	    || ( new_best_bid_i != found_under_data_px->best_bid_premium_i ))
	{
	    // one of the BBOs changed - we need to recalc
	    quote_node_px = new_gen_buf_node ( sizeof ( autoquote_price_change_xt ));
	    // grab typecast copy of pointer into dest storage for price update details
	    (char *)quote_data_px = quote_node_px->msg_p;
	    // note the underlying block this update is for
	    quote_data_px->ise_underlying_px = found_under_data_px;
	    // save the current prices
	    quote_data_px->new_under_ask_premium_i = new_best_ask_i;
	    quote_data_px->new_under_bid_premium_i = new_best_bid_i;

	    // insert into the Q for the autoquote thread
	    add_to_q_tail ( &click_px->authorised_autoquote_q, 
			    &quote_node_px->chain );
	}
    }	// are we autoquoting for this underlying

    // and update the BBO - ask and bid.. regardless of whether we are autoquoting  or not
    found_under_data_px->best_ask_premium_i = new_best_ask_i;
    found_under_data_px->best_bid_premium_i = new_best_bid_i;

    // note the NASDAQ fields, which will be 0 for non-NASDAQ i.e. CQS feed
    found_under_data_px->nasdaq_bid_tick_u = bcast_px->nasdaq_short_sale_bid_tick_c;
    // don't understand why the UPC indicator comes with each bid.. should be attribute of underlying
    found_under_data_px->nasdaq_upc_u = bcast_px->nasdaq_upc_indicator_c;
    // the originating exchange.. this is NOT the same as country-codes
    found_under_data_px->originator_id_u = bcast_px->originator_id_c;


    // note the time we got this update
    found_under_data_px->time_bbo_update_u = time(NULL);

    // debug - don't put \n newline in start of line.. more data follows
    printf ("BO105 Underlying BBO bcast, ");
    display_one_under_bbo ( found_under_data_px );

    // are we autoquoting for this underlying ?
    if ( found_under_data_px->autoquoting_b )
    {
	// yes, we are autoquoting.. figure out if BBO changed -> need to requote
    }	// are we autoquoting for this underlying

    return (status_i);
}   // handle_bcast_BO105

// -------------------------------------------------------------------------

static int32 handle_bcast_BO106 ( click_details_xt *click_px,
				    special_order_bo106_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BO106
Author	 : Neil Cossor
Modified : 20001229
Description:

    We got an Block/Facilitation Order msg - process it.
    This means that we stash the interesting info about it...
    note that this order can be deleted (with appropriate bcasts),
    from a manual cancel by the owner, or by the timer expiring
*/
{
    int32		    status_i  = SUCCESSFUL;
    ise_series_xt	    ise_series_x;   // temp, the series in ISE form
    ise_series_info_pt	    found_series_px;	// pointer to info about a series
    // pointer to uint64 as a structure, so we can dump out each half of order ID
    uint64_xt		    *uint64_pxu;
    // string for displaying bid/ask characteristics - big enuff to put "Hidden" into
    char		    *bid_ask_ps;
    // string for displaying quantity (5 digits), but big enuff to put "Hidden" into
    char		    quantity_s[7];
    // ditto, for validity time (3 plus NUL), allow for "Hidden"
    char		    *validity_time_ps;
    // expiry time, as a string.. unfortunately there is no info in the DFI on this field
    char		    expiry_time_s[9];
    // the string form of a premium, with appropriate # of decimal places
    series_premium_st	    premium_s;
    // temp longword, for correct endian value of 32bit values (premium)
    uint32		    temp_32_u;
    // temp word, for correct endian value of 16bit values (quantity, validity time)
    uint16		    temp_16_u;


    // log our response
    log_bcast ( LOG_K_B_BLOCK_FACIL_ORDER,
		bcast_px,
		bcast_len_u );

    // get the series, in "normal" form
    get_click_series ( &ise_series_x,
			&bcast_px->series );
    // find the record with our data about the series
    found_series_px = find_series_by_ise_series ( click_px,
						    &ise_series_x );
    // we SHOULD know about this series
    if ( found_series_px )
    {
	// save the order ID, so user can respond without having to enter orderID
	PUTQUAD ( found_series_px->block_facil_orderID_u,
		    bcast_px->order_number_u );

	// point structure pointer at the 64bit order ID
	(uint64 *)uint64_pxu = (uint64 *)&(bcast_px->order_number_u);

	// optional fields exposed for block orders
	if (( bcast_px->expose_flag_c & 1 ) != 0 )
	{
	    // premium is exposed
	    PUTLONG ( temp_32_u, bcast_px->premium_i );
	    format_whole_decimal_s ( (char *)&premium_s,
					temp_32_u,
					found_series_px->instrument_class_px->dec_in_premium_u,
					MAX_SERIES_PREMIUM );	// allowed width
	}
	else
	{
	    strcpy ( premium_s, "Hidden");
	}

	if (( bcast_px->expose_flag_c & 2 ) != 0 )
	{
	    // qty is exposed
	    PUTLONG ( temp_32_u, bcast_px->quantity_u );
	    sprintf ( quantity_s, "%hu",
			temp_32_u );
	}
	else
	{
	    strcpy ( quantity_s, "Hidden");
	}

	if (( bcast_px->expose_flag_c & 4 ) != 0 )
	{
	    // bid/ask is exposed
	    bid_ask_ps = bid_ask_to_string ( bcast_px->bid_or_ask_c );
	}
	else
	{
	    bid_ask_ps = "Hidden";
	}

	if (( bcast_px->expose_flag_c & 8 ) != 0 )
	{
	    // validity time is exposed
	    PUTSHORT ( temp_16_u, bcast_px->time_validity_n );
	    validity_time_ps = validity_time_to_string ( temp_16_u );
	}
	else
	{
	    // point to (in code area) static string
	    validity_time_ps = "Hidden";
	}

	PUTSHORT ( temp_16_u, bcast_px->expiration_time_n );
	sprintf ( expiry_time_s, "%d",
		     temp_16_u );
	// report the interesting things about the bcast
	printf ( "BO106 %s order bcast, series %s, orderID %08lx:%08lx, Bid/Ask=%s\n"
		 " \tPremium=%s, Qty=%s, Validity Time=%s, Expiry Time %s",
		order_category_to_string ( bcast_px->order_category_n ),
		&found_series_px->ise_name_s,
		uint64_pxu->l1,
		uint64_pxu->l0,
		bid_ask_ps,
		&premium_s,
		&quantity_s,
		validity_time_ps,
		&expiry_time_s
		);

	// optional field exposed for facilitation orders
	if (( bcast_px->expose_flag_c & 16 ) != 0 )
	{
	    // broker percentage is exposed
	    printf (" \tbroker percentage %3u\n",
			bcast_px->broker_percentage_c );
	}
	else
	{
	    // broker percentage is hidden
	    printf (" \tbroker percentage hidden\n" );
	}

	// need to add the order to the known order info
    }
    else
    {
	printf ( "Error - received BO106 bcast for unknown series\n");
    }

    return (status_i);
}   // handle_bcast_BO106

// -------------------------------------------------------------------------

static int32 handle_bcast_BO107 ( click_details_xt *click_px,
				    order_quote_deleted_bo107_t *bcast_px ,
				    uint32 bcast_len_u)
/*
Function : handle_bcast_BO107
Author	 : Neil Cossor
Modified : 20010304
Description:

    got an Order/Quote Deleted msg - process it
*/
{
    int32		    status_i  = SUCCESSFUL;
    ise_series_xt	    ise_series_x;	// temp, the series in ISE form
    ise_series_info_pt	    series_data_px;	// pointer to info about a series
    quad_word		    delete_orderID_u;	// orderID being deleted
    uint16		    order_category_u;	// order category.. BID / ASK
    uint16		    rem_quantity_u;	// quantity being deleted


    // get the series, in "normal" form
    get_click_series ( &ise_series_x,
			&bcast_px->series );
    // find the record with our data about the series
    series_data_px = find_series_by_ise_series ( click_px,
						    &ise_series_x );
    // log our response
    log_bcast ( LOG_K_B_ORDER_QUOTE_DELETED,
		bcast_px,
		bcast_len_u );

    // debug
    PUTSHORT ( rem_quantity_u, bcast_px->rem_quantity_n );
    printf ( "Broadcast BO107: %s deleted, series %s, %s %u contracts, reason %s\n",
		order_category_to_string ( bcast_px->order_category_n ),
		&series_data_px->ise_name_s,
		bid_ask_to_string ( bcast_px->bid_or_ask_c ),
		rem_quantity_u,
		reason_delete_to_string ( bcast_px->reason_order_delete_c )
	    );

    PUTQUAD ( delete_orderID_u,
		bcast_px->order_number_u );

    // an order or a quote ?
    PUTSHORT ( order_category_u,
		bcast_px->order_category_n );
    if ( order_category_u == ORDER_CAT_QUOTE )
    {
	// zot (i.e. zero) info about our quote in this series & bid/ask
	if ( bcast_px->bid_or_ask_c == BID )
	{
	    // zot the BID
		series_data_px->bid_quoteID_u.quad_word[0] = '\0';
	    series_data_px->quote_bid_premium_i = 0;
	    series_data_px->quote_bid_qty_u = 0;
	}
	else
	{
	    // zot the ASK
	    series_data_px->ask_quoteID_u.quad_word[0] = '\0';
	    series_data_px->quote_ask_premium_i = 0;
	    series_data_px->quote_ask_qty_u = 0;
	}
    }
    else
    {
	// all other categories relate to an order.. see if we can find the order to delete it
	delete_order_by_orderID ( click_px,
				    delete_orderID_u );
    }

    return (status_i);
}   // handle_bcast_BO107

// -------------------------------------------------------------------------

static int32 handle_bcast_BO108 ( click_details_xt *click_px,
				    quote_rejection_reason_bo108_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BO108
Author	 : Neil Cossor
Modified : 20001230
Description:

    got an Quote Rejection msg - process it

    NB release 16.3 makes major change. Most fields from the old format,
    such as 'Series', bid/ask, quantity etc have all been removed.

    To report these correctly would require that we keep (to the side) any rejected
    MO106 quotes, so that when the rejects come back, we can look them up,
    and do the reporting from that.

    This then has issues of when (exactly) do we get to release the 'saved' quotes,
    as we may receive multiple rejects for the one MO106.

    I leave this as an exercise for the reader i.e. someone who has to
    do the production version of this code..
*/
{
    int32	    status_i  = SUCCESSFUL;
    // actual (local-endian) quote (order) ID for the accepted block quote
    quad_word	    quoteID_u;
    // pointer to uint64 as a structure, so we can dump out each half of order ID
   quad_conv_t * convOrderNo;


    // log our response
    log_bcast ( LOG_K_B_QUOTE_REJECT_REASON,
		bcast_px,
		bcast_len_u );

    PUTQUAD ( quoteID_u,
		bcast_px->order_number_u );

    // debug
	convOrderNo = (quad_conv_t *) &(quoteID_u.quad_word);
    printf ( "Broadcast BO108: Quote Rejection, quoteID [%08lX:%08lX], item # %u, reject code [%d,0x%08x]\n",
		convOrderNo->l2,
			convOrderNo->l1,
		bcast_px->item_number_c,
		bcast_px->reason_quote_rejected_u,
		bcast_px->reason_quote_rejected_u
	    );

    return (status_i);
}   // handle_bcast_BO108

// -------------------------------------------------------------------------

static int32 handle_bcast_BO109 ( click_details_xt *click_px,
				    quote_stepped_up_bo109_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BO109
Author	 : Neil Cossor
Modified : 20001230
Description:

    got an Quote Step-up msg - process it
*/
{
    int32	  status_i  = SUCCESSFUL;
    ise_series_xt	    ise_series_x;   // temp, the series in ISE form
    ise_series_info_pt	    series_data_px;	// pointer to info about a series


    // get the series, in "normal" form
    get_click_series ( &ise_series_x,
			&bcast_px->series );
    // find the record with our data about the series
    series_data_px = find_series_by_ise_series ( click_px,
						    &ise_series_x );
    // log our response
    log_bcast ( LOG_K_B_QUOTE_STEP_UP,
		bcast_px,
		bcast_len_u );

    // debug
    printf ( "Broadcast BO109: Quote Step-up, %s\n",
		&series_data_px->ise_name_s );

    // update the existing quote.. we may have chewed down from above min-BBO qty, and stepped to that
    if ( bcast_px->bid_or_ask_c == BID )
    {
	// update the BID
	PUTLONG ( series_data_px->quote_bid_premium_i,
		    bcast_px->premium_i );
	PUTLONG ( series_data_px->quote_bid_qty_u,
		    bcast_px->quantity_u );
    }
    else
    {
	// update the ASK
	PUTLONG ( series_data_px->quote_ask_premium_i,
		    bcast_px->premium_i );
	PUTLONG ( series_data_px->quote_ask_qty_u,
		    bcast_px->quantity_u );
    }	// updating BID or ASK

    return (status_i);
}   // handle_bcast_BO109

// -------------------------------------------------------------------------

static int32 handle_bcast_BO110 ( click_details_xt *click_px,
				    quote_tick_worse_bo110_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BO110
Author	 : Neil Cossor
Modified : 20010618
Description:

    got an Quote Update msg (formerly known as Tick-Worse) - process it
*/
{
    int32	  status_i  = SUCCESSFUL;
    ise_series_xt	    ise_series_x;	// temp, the series in ISE form
    ise_series_info_pt	    series_data_px;	// pointer to info about a series
    // the string form of a premium, with appropriate # of decimal places
    series_premium_st	    premium_s;
    // temp longword, for correct endian value of 32bit values
    uint32		    temp_32_u;
    // temp word, for correct endian of tick level
    uint16		    tick_worse_u;


    // get the series, in "normal" form
    get_click_series ( &ise_series_x,
			&bcast_px->series );
    // find the record with our data about the series
    series_data_px = find_series_by_ise_series ( click_px,
						    &ise_series_x );
    // log our response
    log_bcast ( LOG_K_B_QUOTE_TICK_WORSE,
		bcast_px,
		bcast_len_u );
    // format premium into local endian, and then convert to string with appropr dec places
    PUTLONG ( temp_32_u, bcast_px->premium_i );
    format_whole_decimal_s ( (char *)&premium_s,
				temp_32_u,
				series_data_px->instrument_class_px->dec_in_premium_u,
				MAX_SERIES_PREMIUM );	// allowed width
    // update the existing quote.. 
    if ( bcast_px->bid_or_ask_c == BID )
    {
	// update the BID
	PUTLONG ( series_data_px->quote_bid_premium_i,
		    bcast_px->premium_i );
	PUTLONG ( series_data_px->quote_bid_qty_u,
		    bcast_px->quantity_u );
	temp_32_u = series_data_px->quote_bid_qty_u;
    }
    else
    {
	// update the ASK
	PUTLONG ( series_data_px->quote_ask_premium_i,
		    bcast_px->premium_i );
	PUTLONG ( series_data_px->quote_ask_qty_u,
		    bcast_px->quantity_u );
	temp_32_u = series_data_px->quote_ask_qty_u;
    }	// updating BID or ASK

    // get tick worse level in local-endian order
    PUTSHORT ( tick_worse_u, bcast_px->tick_worse_level_n );

    // debug
    printf ( "Broadcast BO110: Quote Update, %s series %s, %u@%s, reason %s, tick level %u\n",
		bid_ask_to_string ( bcast_px->bid_or_ask_c ),
		series_data_px->ise_name_s,
		temp_32_u,
		premium_s,
		reason_tick_to_string (bcast_px->reason_tick_worse_c),
		tick_worse_u );

    return (status_i);
}   // handle_bcast_BO110

// -------------------------------------------------------------------------

static int32 handle_bcast_BO112 ( click_details_xt *click_px,
				    special_order_ended_bo112_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BO112
Author	 : Neil Cossor
Modified : 9905xx
Description:

    got an Block/Facilitation Order Ended msg - process it
*/
{
    int32	  status_i  = SUCCESSFUL;
    ise_series_xt	    ise_series_x;   // temp, the series in ISE form
    ise_series_info_pt	    found_series_px;	// pointer to info about a series


    // get the series, in "normal" form
    get_click_series ( &ise_series_x,
			&bcast_px->series );
    // find the record with our data about the series
    found_series_px = find_series_by_ise_series ( click_px,
						    &ise_series_x );
    // log our response
    log_bcast ( LOG_K_B_BLOCK_FACIL_ENDED,
		bcast_px,
		bcast_len_u );

    // debug
    printf ( "Broadcast BO112: Special Order Ended, series %s\n",
		&found_series_px->ise_name_s );

    return (status_i);
}   // handle_bcast_BO112

// -------------------------------------------------------------------------

static int32 handle_bcast_BO115 ( click_details_xt *click_px,
				    locked_order_bo115_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BO115
Author	 : Neil Cossor
Modified : 20011024
Description:

    got an PMM Locked Order msg - process it
*/
{
    int32	  status_i  = SUCCESSFUL;
    ise_series_xt	    ise_series_x;   // temp, the series in ISE form
    ise_series_info_pt	    found_series_px;	// pointer to info about a series
    // the string form of a premium, with appropriate # of decimal places
    series_premium_st	    premium_s;
    // pointer to uint64 as a structure, so we can dump out each half of order ID
    uint64_xt		    *uint64_pxu;
    // temp longword, for correct endian value of 32bit values
    uint32		    temp_32_u;
    // temp word, for correct endian value of quantity
    uint32		    qty_u;
    // the trading code for a locked order
    ise_user_code_xt	    user_code_x;

    // get the series, in "normal" form
    get_click_series ( &ise_series_x,
			&bcast_px->series );
    // find the record with our data about the series
    found_series_px = find_series_by_ise_series ( click_px,
						    &ise_series_x );
    // log our response
    log_bcast ( LOG_K_B_PMM_LOCKED_ORDER,
		bcast_px,
		bcast_len_u );

    // point structure pointer at the 64bit order ID
    (uint64 *)uint64_pxu = (uint64 *)&(bcast_px->order_number_u);

    // format premium into local endian, and then convert to string with appropr dec places
    PUTLONG ( temp_32_u, bcast_px->premium_i );
    format_whole_decimal_s ( (char *)&premium_s,
				temp_32_u,
				found_series_px->instrument_class_px->dec_in_premium_u,
				MAX_SERIES_PREMIUM );	// allowed width
    // grab quantity, in local endian order
    PUTLONG ( qty_u, bcast_px->quantity_u );
    // grab the trading code info == user_code info
    get_ise_user_code_xt ( &user_code_x,
			    (user_code_t *)&bcast_px->trading_code );
    // NB trading code sometimes contains something useful in 16.5.. so report it too

    // debug.. 
    printf ( "BO115: PMM Locked Order, %s %s, orderID %08lx:%08lx, %u@%s, action %u, locktype %s, (%5s),(%5s)\n",
		bid_ask_to_string ( bcast_px->bid_or_ask_c ),
		&found_series_px->ise_name_s,
		uint64_pxu->l1,
		uint64_pxu->l0,
		qty_u,
		&premium_s,
		bcast_px->action_locked_order_c,
		lock_type_to_string ( bcast_px->lock_type_c ),
		&user_code_x.customer_id_s,
		&user_code_x.user_id_s
	    );

    return (status_i);
}   // handle_bcast_BO115

// -------------------------------------------------------------------------

static int32 handle_bcast_BO116 ( click_details_xt *click_px,
				    derived_order_bo116_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BO116
Author	 : Neil Cossor
Modified : 9905xx
Description:

    got an PMM Derived Order msg - process it
*/
{
    int32	  status_i  = SUCCESSFUL;
    ise_series_xt	    ise_series_x;   // temp, the series in ISE form
    ise_series_info_pt	    found_series_px;	// pointer to info about a series


    // get the series, in "normal" form
    get_click_series ( &ise_series_x,
			&bcast_px->series );
    // find the record with our data about the series
    found_series_px = find_series_by_ise_series ( click_px,
						    &ise_series_x );
    // log our response
    log_bcast ( LOG_K_B_PMM_DERIVED_ORDER,
		bcast_px,
		bcast_len_u );

    // debug
    printf ( "Broadcast BO116: PMM Derived Order%s\n",
		&found_series_px->ise_name_s );

    return (status_i);
}   // handle_bcast_BO116

// -------------------------------------------------------------------------

static int32 handle_bcast_BO130 ( click_details_xt *click_px,
				    ob_changes_bo130_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BO130
Author	 : Neil Cossor
Modified : 20010605
Description:

    got an Orderbook broadcast msg - process it
    One of these describes a change to one order for our user/member (depending on our subscription)
    This could be an add/amend/delete
*/
{
    int32	  status_i  = SUCCESSFUL;
    ise_series_xt	    ise_series_x;	// temp, the series in ISE form
    ise_series_info_pt	    found_series_px;	// pointer to info about a series
    uint16		    order_category_u;
    xref_st		    free_text_s;
    user_id_st		    broker_oper_id_s;   // the broker ID who owns this order
    cmta_member_st	    cmta_member_s;	// CMTA member ID string
    sub_account_id_st	    sub_ac_s;		// and sub account ID string


    // log our bcast
    log_bcast ( LOG_K_B_OB_CHANGES,
		bcast_px,
		bcast_len_u );

    // get the series, in "normal" form
    get_click_series ( &ise_series_x,
			&bcast_px->series );
    // find the record with our data about the series
    found_series_px = find_series_by_ise_series ( click_px,
						    &ise_series_x );
    // extract order category
    PUTSHORT ( order_category_u,
		bcast_px->order_category_n );
    // debug
    printf ( "Bcast BO130: OB change %s %s %s in %s\n",
		change_reason_to_string (bcast_px->change_reason_c),
		bid_ask_to_string ( bcast_px->bid_or_ask_c ),
		order_category_to_string ( order_category_u ),
		&found_series_px->ise_name_s
		);

    switch ( bcast_px->change_reason_c )
    {
	case CHANGE_REASON_ADD:
	case CHANGE_REASON_MODIFY:
	    // extract the string fields in the message - necessary for updates
	    rx_click_string ( &free_text_s,
				(char *)bcast_px->free_text_s,
				sizeof (bcast_px->free_text_s) );
	    rx_click_string ( &broker_oper_id_s,
				bcast_px->broker_id_s,
				sizeof (bcast_px->broker_id_s));
	    // grab the two parts of the client ID, which are actually the CMTA a/c and sub a/c IDs
	    rx_click_string ( &cmta_member_s,
				&bcast_px->ex_client_s [0],
				MAX_CMTA_MEMBER );
	    rx_click_string ( &sub_ac_s,
				&bcast_px->ex_client_s [MAX_CMTA_MEMBER],	// start offset
				MAX_SUB_ACCOUNT_ID );


	    external_update_order ( click_px,
				    &ise_series_x,
				    bcast_px->order_number_u,
				    bcast_px->premium_i,
				    bcast_px->quantity_u,
				    bcast_px->bid_or_ask_c,
				    free_text_s,
				    bcast_px->time_validity_n,
				    bcast_px->order_type_c,
				    bcast_px->client_category_c,
				    bcast_px->ignore_away_market_c,
				    broker_oper_id_s,
				    0,				// bo130 does NOT give us any lock info
				    bcast_px->order_category_n,
				    0,				// expose flag - none, not block order
				    bcast_px->open_close_req_c,	// a/c behaviour during OCC settlement
				    cmta_member_s,		// CMTA account
				    sub_ac_s,			// and sub account ID
				    0,				// can't get broker % in an MA101
				    bcast_px->trading_code	// who sent in the original order
				    );

	    break;

	case CHANGE_REASON_MATCH:
	case CHANGE_REASON_STOP:
	case CHANGE_REASON_INACTIVATE:
	case CHANGE_REASON_DELETE:
		// deleting an order.. of course, we may not have known about this order if we hadn't
		// queried or heard an earlier relevant bo130 about it
		delete_order_by_orderID ( click_px,
					    bcast_px->order_number_u );
	    break;
	default:
	    printf ("  Error - Unknown change reason %u\n",
			bcast_px->change_reason_c );
    }

    return (status_i);
}   // handle_bcast_BO130

// -------------------------------------------------------------------------

static int32 handle_bcast_BO131 ( click_details_xt *click_px,
				    combo_ob_changes_bo131_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BO131
Author	 : Neil Cossor
Modified : 20011017
Description:

    got a public combo OB change msg - process it
    One of these describes a change to one order for the public combo orderbook
    This could be an add/amend/delete/match/stop/inactivate
*/
{
    int32		    status_i  = SUCCESSFUL;
    // the commodity code of underlying in the bcast
    uint16		    found_commodity_code_u;
    // the underlying record for the underlying
    ise_underlying_xt	    *under_data_px;
    // the order category - generate from the interest_order_c field
    uint16		    order_category_u = 0;
    combo_message_st	    message_text_s;
    // quantity in broadcast
    uint32		    combo_qty_u;


    PUTSHORT ( found_commodity_code_u,
		bcast_px->commodity_n );
    // find the underlying record
    under_data_px = find_under_data_by_commodity ( click_px,
						    found_commodity_code_u );
    // log our response
    log_bcast ( LOG_K_B_COMBO_OB_CHANGES,
		bcast_px,
		bcast_len_u );

    switch ( bcast_px->interest_order_c )
    {
    case COMBO_INTEXEC_LIMIT:
	order_category_u = ORDER_CAT_COMBO_LIMIT;
	break;
    case COMBO_INTEXEC_INTEREST:
	order_category_u = ORDER_CAT_COMBO_INTEREST;
	break;
    default:
	printf (" Error, unknown interest_only_c value %u\n",
		    bcast_px->interest_order_c );
    }	// switch on values interest_only field

    // grab the combo qty
    PUTLONG (combo_qty_u, bcast_px->quantity_u );

    // debug
    printf ( "Broadcast BO131: %s public %s in %s\n",
		change_reason_to_string (bcast_px->change_reason_c),
		order_category_to_string ( order_category_u ),
		&under_data_px->under_id_s );

    switch ( bcast_px->change_reason_c )
    {
	case CHANGE_REASON_ADD:
	case CHANGE_REASON_MODIFY:
	    // extract the string fields in the message - necessary for updates
	    rx_click_string ( &message_text_s,
				(char *)bcast_px->message_s,
				sizeof (bcast_px->message_s) );

	    external_update_combo ( click_px,
				    under_data_px,
				    bcast_px->order_number_u,	// orderID is public - needed for HIT
				    bcast_px->premium_i,
				    bcast_px->quantity_u,
				    NULL,			// can't see freetext in public data
				    bcast_px->time_validity_n,
				    order_category_u,
				    0,				// can't see client category in public data
				    NULL,			// can't see brokerID in public data
				    NULL,			//   "    "  CMTA account in public data
				    NULL,			//   "    "  sub account ID in public data
				    message_text_s,
				    NULL,			// can't see who sent original order in public data
				    bcast_px->items_c,		// how many legs
					bcast_px->ul_price_high_i,
					bcast_px->ul_price_low_i,
					bcast_px->ul_price_method_c,
				    (OM_one_combo_leg_xt *)&bcast_px->item[0]		// pointer to first leg
				    );

	    break;

	case CHANGE_REASON_MATCH:
	    // the quantity in the bcast indicates the remaining qty.. if it is 0,
	    // then the combo completely traded out
	    if ( combo_qty_u == 0 )
	    {
		// combo completely traded out.. delete it
		delete_combo_by_orderID ( click_px,
					    bcast_px->order_number_u,
					    FALSE );
	    }
	    else
	    {
		// combo partially traded.. update the qty
		update_combo_qty_by_orderID ( click_px,
						bcast_px->order_number_u,
						FALSE,
						combo_qty_u );
	    }
	    break;

	case CHANGE_REASON_STOP:
	case CHANGE_REASON_INACTIVATE:
	case CHANGE_REASON_DELETE:
		// deleting an order.. of course, we may not have known about this order if we hadn't
		// queried or heard an earlier relevant bo130 about it
		delete_combo_by_orderID ( click_px,
					    bcast_px->order_number_u,
					    FALSE );
	    break;
	default:
	    printf ("  Error - Unknown change reason %u\n",
			bcast_px->change_reason_c );
    }

    return (status_i);
}   // handle_bcast_BO131

// -------------------------------------------------------------------------

static int32 handle_bcast_BO132 ( click_details_xt *click_px,
				    combo_ob_member_bo132_t *bcast_px,
				    uint32 bcast_len_u )
/*
Function : handle_bcast_BO132
Author	 : Neil Cossor
Modified : 20010605
Description:

    got an private combo Orderbook msg - process it
    One of these describes a change to one combo order for our user/member (depending on our subscription)
    This could be an add/amend/delete/match/stop/inactivate

    Note that we may have already heard about the combo in the BO131 bcast,
    in which case we will be updating some of the fields with details
    not available in the public bcast. This includes
    - freetext
    - client category
    - brokerID
    - CMTA
    - sub-account
    - which user (in our member) sent in the combo
    - open/close attributes of the legs
*/
{
    int32		    status_i  = SUCCESSFUL;
    // the commodity code of underlying in the bcast
    uint16		    found_commodity_code_u;
    // the underlying record for the underlying
    ise_underlying_xt	    *under_data_px;
    // the order category - generate from the interest_order_c field
    uint16		    order_category_u = 0;
    // C-style extracts (NUl terminated) of strings from the message
    combo_message_st	    message_text_s;
    xref_st		    free_text_s;
    user_id_st		    broker_oper_id_s;   // the broker ID who owns this order
    cmta_member_st	    cmta_member_s;	// CMTA member ID string
    sub_account_id_st	    sub_ac_s;		// and sub account ID string
    // quantity in broadcast
    uint32		    combo_qty_u;


    PUTSHORT ( found_commodity_code_u,
		bcast_px->commodity_n );
    // find the underlying record
    under_data_px = find_under_data_by_commodity ( click_px,
						    found_commodity_code_u );
    // log our response
    log_bcast ( LOG_K_B_COMBO_OB_CHANGE_OWN,
		bcast_px,
		bcast_len_u );

    switch ( bcast_px->interest_order_c )
    {
    case COMBO_INTEXEC_LIMIT:
	order_category_u = ORDER_CAT_COMBO_LIMIT;
	break;
    case COMBO_INTEXEC_INTEREST:
	order_category_u = ORDER_CAT_COMBO_INTEREST;
	break;
    default:
	printf (" Error, unknown interest_only_c value %u\n",
		    bcast_px->interest_order_c );
    }	// switch on values interest_only field

    // grab the combo qty
    PUTLONG (combo_qty_u, bcast_px->quantity_u );

    // debug
    printf ( "Broadcast BO132: %s member Combo %s in %s\n",
		change_reason_to_string (bcast_px->change_reason_c),
		order_category_to_string ( order_category_u ),
		&under_data_px->under_id_s );

    switch ( bcast_px->change_reason_c )
    {
	case CHANGE_REASON_ADD:
	case CHANGE_REASON_MODIFY:
	    // extract the string fields in the message - necessary for updates
	    rx_click_string ( &message_text_s,
				(char *)bcast_px->message_s,
				sizeof (bcast_px->message_s) );
	    rx_click_string ( &free_text_s,
				(char *)bcast_px->free_text_s,
				sizeof (bcast_px->free_text_s) );
	    rx_click_string ( &broker_oper_id_s,
				bcast_px->broker_id_s,
				sizeof (bcast_px->broker_id_s));
	    // grab the two parts of the client ID, which are actually the CMTA a/c and sub a/c IDs
	    rx_click_string ( &cmta_member_s,
				&bcast_px->ex_client_s [0],
				MAX_CMTA_MEMBER );
	    rx_click_string ( &sub_ac_s,
				&bcast_px->ex_client_s [MAX_CMTA_MEMBER],	// start offset
				MAX_SUB_ACCOUNT_ID );


	    external_update_combo ( click_px,
				    under_data_px,
				    bcast_px->order_number_u,	// orderID is public - needed for HIT
				    bcast_px->premium_i,
				    bcast_px->quantity_u,
				    free_text_s,		// Xref private to member
				    bcast_px->time_validity_n,
				    order_category_u,
				    bcast_px->client_category_c, // client category
				    broker_oper_id_s,		// just text (thru to clearing member).. "B/D" used to be special
				    cmta_member_s,
				    sub_ac_s,
				    message_text_s,
				    &bcast_px->trading_code,	// who sent original combo
				    bcast_px->items_c,		// how many legs
					bcast_px->ul_price_high_i,
					bcast_px->ul_price_low_i,
					bcast_px->ul_price_method_c,
				    (OM_one_combo_leg_xt *)&bcast_px->item[0]		// pointer to first leg
				    );

	    break;

	case CHANGE_REASON_MATCH:
	    // the quantity in the bcast indicates the remaining qty.. if it is 0,
	    // then the combo completely traded out
	    if ( combo_qty_u == 0 )
	    {
		// combo completely traded out.. delete it
		delete_combo_by_orderID ( click_px,
					    bcast_px->order_number_u,
					    FALSE );
	    }
	    else
	    {
		// combo partially traded.. update the qty
		update_combo_qty_by_orderID ( click_px,
						bcast_px->order_number_u,
						FALSE,
						combo_qty_u );
	    }
	    break;

	case CHANGE_REASON_STOP:
	case CHANGE_REASON_INACTIVATE:
	case CHANGE_REASON_DELETE:
		// deleting an order.. of course, we may not have known about this order if we hadn't
		// queried or heard an earlier relevant bo130 about it
		delete_combo_by_orderID ( click_px,
					    bcast_px->order_number_u,
					    FALSE );
	    break;
	default:
	    printf ("  Error - Unknown change reason %u\n",
			bcast_px->change_reason_c );
    }

    return (status_i);
}   // handle_bcast_BO132

// -------------------------------------------------------------------------

static int32 handle_BD_series_bcasts (click_details_xt *click_px,
				       struct broadcast_type *bcast_px,
					uint16 bcast_length_u )
/*
Function : handle_BD_series_bcasts
Author	 : Neil Cossor
Modified : 20000105
Description:

    These broadcasts come from the Deal system.
    These could be trades on our system, or on underlying markets.
*/
{
    int32   status_i = SUCCESSFUL;

    switch (bcast_px->transaction_number_n)
    {	
	case 5:   // BD5 - Dedicated trade info
		handle_bcast_BD5( click_px,
				    ( directed_trade_t * ) bcast_px,
				    bcast_length_u );
		break;

	case 101: // BD101 - Trade Information
		handle_bcast_BD101( click_px,
				    ( trade_info_bd101_t * ) bcast_px,
				    bcast_length_u );
		break;

	case 102:  // BD102 - OPRA Trade info
		handle_bcast_BD102( click_px,
				    ( opra_trade_info_bd102_t * ) bcast_px,
				    bcast_length_u );
		break;

	case 103:  // BD103 - Underlying Trade info
		handle_bcast_BD103( click_px,
				    ( underlying_trade_info_bd103_t * ) bcast_px,
				    bcast_length_u );
		break;

	case 111:   // BD111 - Deal
		handle_bcast_BD111 ( click_px,
				    ( deal_user_bd111_t *) bcast_px,
				    bcast_length_u );
		break;
	default:
		printf ("Unknown BD broadcast type %d\n",
			 bcast_px->transaction_number_n );
		// status_i = UNHANDLED_CASE;
		break;
    }

    return (status_i);
}   // handle_BD_series_bcasts

// -------------------------------------------------------------------------

static int32 handle_BI_series_bcasts (click_details_xt *click_px,
					struct broadcast_type *bcast_px,
					uint16 bcast_length_u)
/*
Function : handle_BI_series_bcasts
Author	 : Neil Cossor
Modified : 9905xx
Description: 

    These broadcasts come from the Information system.
    These could be status changes (market, underlying, series), text info,
    or changes in the CDB (changes in underlying, instr class or series data)
*/
{
    int32   status_i = SUCCESSFUL;

    switch (bcast_px->transaction_number_n)
    {
	case 8:// BI8 - General Message i.e. text
		handle_bcast_BI8 ( click_px,
				    (general_message_t *)bcast_px,
				    bcast_length_u );
		break;

	case 9:  // BI9 - price information heartbeat
		handle_bcast_BI9 ( click_px,
				(info_heartbeat_t *)bcast_px,
				bcast_length_u);
		break;

	case 41: // BI41 Instrument Status 
		handle_bcast_BI41 ( click_px,
				    (instrument_status_info_t *)bcast_px,
				    bcast_length_u );
		break;

	case 103:// BI103 - Series Status Change
		handle_bcast_BI103 ( click_px,
				    (series_status_change_bi103_t *)bcast_px,
				    bcast_length_u );
		break;

	case 114:// BI114 - Away Market Check
		handle_bcast_BI114 ( click_px,
				    (away_market_flag_update_us_t *)bcast_px,
				    bcast_length_u );
		break;

	case 132: // BI132 - Underlying Trading Status
		handle_bcast_BI132( click_px,
				    (underlying_trading_status_bi132_t *)bcast_px,
				    bcast_length_u );
		break;

	case 202:// BI202 - Series Update - new or altered series info
		handle_bcast_BI202 ( click_px,
				    (series_update_bi202_t *)bcast_px,
				    bcast_length_u );
		break;

	case 204:// BI204 - Underlying Update - new or altered underlying info
		handle_bcast_BI204 ( click_px,
				    (underlying_update_bi204_t *)bcast_px,
				    bcast_length_u );
		break;

	case 210:// BI210 - Instrument Class Update - new or altered instrument class
		handle_bcast_BI210 ( click_px,
				    (instrument_class_update_bi210_t *)bcast_px,
				    bcast_length_u );
		break;

	default:
		printf ("Unknown BI broadcast type %d\n",
			 bcast_px->transaction_number_n );
		//status_i = UNHANDLED_CASE;
		break;
    }
    return (status_i);
}   // handle_BI_series_bcasts

// -------------------------------------------------------------------------

static int32 handle_BN_series_bcasts (click_details_xt *click_px,
				       struct broadcast_type *bcast_px,
					uint16 bcast_length_u )
/*
Function : handle_BN_series_bcasts
Author	 : Neil Cossor
Modified : 9905xx
Description:

    These broadcasts come from the Network system.
    Note that these are built into the OMnet system.
    They are documented in the OM API manual, and NOT in the normal System Programmers Manual.
    They are not subscribed.. ALL API users will receive them.
*/
{
    int32	status_i = SUCCESSFUL;

    switch (bcast_px->transaction_number_n)
    {	
	case 1:   // BN1 - Network  broadcast
		status_i = handle_bcast_BN1( click_px,
					    ( omni_broadcast * ) bcast_px,
					    bcast_length_u );
		break;

	default:
		printf ("Unknown BN broadcast type %d\n",
			 bcast_px->transaction_number_n );
		status_i = UNHANDLED_CASE;
		break;
    }

    return (status_i);
}   // handle_BN_series_bcasts

// -------------------------------------------------------------------------

static int32 handle_BO_series_bcasts (click_details_xt *click_px,
					struct broadcast_type *bcast_px,
					uint16 bcast_length_u)
/*
Function : handle_BO_series_bcasts
Author	 : Neil Cossor
Modified : 20000105
Description: 
    These broadcasts come from the Orderbook system, which includes
    external data which affects the behavior of the orderbook system,
    such as CQS BBOs.

    This includes things like options BBO (ours, and OPRA),
    underlying BBO, Block and Facilitation orders, Quote alterations,
    and Locked and Derived orders (PMM only).
*/
{
    int32   status_i = SUCCESSFUL;

    switch (bcast_px->transaction_number_n)
    {
	case 101: // BO101 - Best Bid Offer
		handle_bcast_BO101 ( click_px,
				    (best_bid_offer_bo101_t *)bcast_px,
				    bcast_length_u);
		break;

	case 102:  // BO102 - Extended Best Bid Offer
		handle_bcast_BO102 ( click_px,
				    (best_bid_offer_extended_bo102_t *)bcast_px,
				    bcast_length_u);
		break;

	case 103:  // BO103 - OPRA Best Bid Offer
		handle_bcast_BO103 ( click_px,
				    (opra_best_bid_offer_bo103_t *)bcast_px,
				    bcast_length_u);
		break;

	case 104:  // BO104 - Each Exchange OPRA Best Bid Offer
		handle_bcast_BO104 ( click_px,
				    (opra_all_best_bid_offer_bo104_t *)bcast_px,
				    bcast_length_u);
		break;

	case 105:  // BO105 - Underlying Best Bid Offer
		handle_bcast_BO105 ( click_px,
				    (underlying_best_bid_offer_bo105_t *)bcast_px,
				    bcast_length_u);
		break;

	case 106:  // BO106 - Block/Facilitation Order
		handle_bcast_BO106 ( click_px,
				    (special_order_bo106_t *)bcast_px,
				    bcast_length_u);
		break;

	case 107:  // BO107 - Order/Quote Deleted
		handle_bcast_BO107 ( click_px,
				    (order_quote_deleted_bo107_t *)bcast_px,
				    bcast_length_u);
		break;

	case 108:  // BO108 - Quote Rejection
		handle_bcast_BO108 ( click_px,
				    (quote_rejection_reason_bo108_t *)bcast_px,
				    bcast_length_u);
		break;

	case 109:  // BO109 - Quote Step-up
		handle_bcast_BO109 ( click_px,
				    (quote_stepped_up_bo109_t *)bcast_px,
				    bcast_length_u);
		break;

	case 110:  // BO110 - Quote Tick-Worse
		handle_bcast_BO110 ( click_px,
				(quote_tick_worse_bo110_t *)bcast_px,
				bcast_length_u);
		break;

	case 112:  // BO112 - Block/Facilitation Order Ended
		handle_bcast_BO112 ( click_px,
				    (special_order_ended_bo112_t *)bcast_px,
				    bcast_length_u);
		break;

	case 115:  // BO115 - PMM Locked Order
		handle_bcast_BO115 ( click_px,
				    (locked_order_bo115_t *)bcast_px,
				    bcast_length_u);
		break;

	case 116:  // BO116 - PMM Derived Order
		handle_bcast_BO116 ( click_px,
				    (derived_order_bo116_t *)bcast_px,
				    bcast_length_u);
		break;


	case 130:  // BO130 - orderbook changes, user to maintain local copy of user's/member's OB
		handle_bcast_BO130 ( click_px,
				    (ob_changes_bo130_t *)bcast_px,
				    bcast_length_u);
		break;

	case 131:  // BO131 - public combo OB broadcast
		handle_bcast_BO131 ( click_px,
				    (combo_ob_changes_bo131_t *)bcast_px,
				    bcast_length_u);
		break;

	case 132:  // BO132 - combo OB broadcast for own user's/member's.. like bo130 for combos
		handle_bcast_BO132 ( click_px,
				    (combo_ob_member_bo132_t *)bcast_px,
				    bcast_length_u);
		break;

	default:
		printf ("BO%d broadcast type unknown\n",
			 bcast_px->transaction_number_n);
		// status_i = UNHANDLED_CASE;
		break;
    }

    return (status_i);
}   // handle_BO_series_bcasts

// -------------------------------------------------------------------------

static int32 handle_B_series_bcasts (click_details_xt *click_px,
				       struct broadcast_type *bcast_px,
				       uint16 bcast_length_u)
/*
Function : handle_B_series_bcasts
Author	 : Neil Cossor
Modified : 990906
Description: 
*/
{
    int32   status_i = SUCCESSFUL;

    switch (bcast_px->server_type_c)
    {
	case 'D':   status_i = handle_BD_series_bcasts (click_px,
							    bcast_px,
							    bcast_length_u);
		    break;

	case 'I':   status_i = handle_BI_series_bcasts (click_px,
							    bcast_px,
							    bcast_length_u);
		    break;

	case 'N':   status_i = handle_BN_series_bcasts (click_px,
							    bcast_px,
							    bcast_length_u );
		    break;

	case 'O':   status_i = handle_BO_series_bcasts (click_px,
							    bcast_px,
							    bcast_length_u);
		    break;

	default:
		    printf ("Unknown B broadcast server type %d\n",
			    bcast_px->server_type_c );
		    status_i = UNHANDLED_CASE;
		    break;
    }
    return (status_i);
}   // handle_B_series_bcasts

// -------------------------------------------------------------------------

static int32 process_one_bcast (click_details_xt * click_px,
				 struct broadcast_type *bcast_px,
				 uint16 bcast_length_u,
				 uint32 subscr_handle_u )
/*
Function : process_one_bcast
Author	 : Neil Cossor
Modified : 20000407
Description:

    This function interprets one known broadcasts.

    CAUTION
    The broadcast buffer reference by 'bcast_px' is assumed to be
    longword aligned.
*/
{

    int32 status_i  =	SUCCESSFUL;

/*
    if ((subscr_handle_u <= 0) || (subscr_handle_u >= UNDEFINED_INT32))
    {
	printf ("Warning, got %c%c%d with %4.4xX handle\n",
		 bcast_px->central_module_c,
		 bcast_px->server_type_c,
		 bcast_px->transaction_number_n,
		 subscr_handle_u );
    }
*/
    switch(bcast_px->central_module_c )
    {
	/* start the BD, BI, BO, BN  */
	case 'B':   status_i = handle_B_series_bcasts (click_px,
							bcast_px, 
							bcast_length_u);
		    break;

	default:    printf ("Broadcast %c<%2.2xX>%c<%2.2xX>%d: unknown broadcast (%d bytes)\n",
			     bcast_px->central_module_c,
			     bcast_px->central_module_c,
			     bcast_px->server_type_c,
			     bcast_px->server_type_c,
			     bcast_px->transaction_number_n,
			     bcast_length_u );
/*  20000327, OM says to ignore things you don't understand.. ?
		    status_i = UNHANDLED_CASE;
*/
		    break;
    }

    return (status_i);
}   // process_one_bcast

// -----------------------------------------------------------------

static int32 read_bcast_stream (click_details_xt * click_px)
/*
Function : read_bcast_stream
Author	 : Neil Cossor
Modified : 20010305
Description:

    Reads one bunch of broadcasts that are currently buffered by the gateway.
    A precondition of this routine being called is that they have
    specifically subscribed to all events in which they are interested
*/
{
    // pointer to the broadcast msg under processing... works its way
    // along through a block of 0..n broadcasts
    int8		    *Buffer_pc		= NULL;
    // pointer to start of bcast rx buffer - use the standard rx area
    // in the click detail block
    int8		    *bcast_rx_buff_px	  = NULL;
    // the current event under processing
    subscribed_event_t	    *subscribed_event_px;

    char		    text_buffer_s[TEXT_BUFFER_LEN];
    char		    *TextBuffer_ps	= NULL;
    uint32		    bcast_buff_len_u	= 0;
    int32		    status_i		= SUCCESSFUL;
    // count of how many broadcasts we did in this call of the fn
    int 		    broadcasts_done_i	= 0;
    // how much space is remaining in the buffer at the end of processing one event
    uint32		    rem_space_u;
    // how big a header for each event is...
    uint32		    event_hdr_size_u =
				    sizeof (subscribed_event_px->evelen_n) +
				    sizeof (subscribed_event_px->handle_u);

    TextBuffer_ps = (char *)&text_buffer_s;
    text_buffer_s[0] = NUL;

    // point at start of rx buffer area
    bcast_rx_buff_px = (char *) &click_px->bcast_reply_x;

    /* This call may return only one event, or many events
     *
     * o To return one event, the least significant is cleared in
     *	 the optmsk argument and subscriptions were specified in SET_EVENT
     *
     * o To return 1..many events, the optmsk argument is set to READEV_OPTMSK_MANY
     *
     * In this example we demonstrate a buffer which MAY contain many broadcasts
     *
     * Depending on whether we have subscribed to events, the format of the 
     * returned structure is as follows:
     * 
     * If we have not specifically subscribed to any event using omniapi_set_event
     * -------------------------------------------------------
     * | L1 |Event_1  | L2  | Event_2  |...| LN |Event_N  | 0 |
     * -------------------------------------------------------
     * Where L1 is the length of the event. The stream is 0 terminated
     *
     * If we have specified events to which we subscribe using omniapi_set_event
     * ---------------------------------------------------------------------------
     * | SH1 | L1 | Event_1  | SH2 | L2  | Event_2  |...| SHN | LN |Event_N  | 0 |
     * ---------------------------------------------------------------------------
     *
     * We demonstrate the last case
     * NB there may be 1 to 3 NUL chars at the end of the buffer
     */
    bcast_buff_len_u = sizeof (click_px->bcast_reply_x);

    // grab exclusive access to OMNIAPI - i.e. block out any other threads
    ISE_MUTEX_LOCK ( click_px->omni_io_active_mutex );

    // if we have just logged off (race condition), then DONT do the IO
    if ( click_px->logged_on_i)
    {
	status_i = omniapi_read_event_ext (OMNI_EVTTYP_ALL,	    // we want to examine all events for which we have subscribed
					    bcast_rx_buff_px,	    // addr of our buffer we allocated
					    &bcast_buff_len_u,	    // number of bytes placed into buffer
					    NULL,		    // reserved by OM
					    READEV_OPTMSK_MANY );   // the buffer may contain 1..n events
    }
    else
    {
	// we are logged off - pretend we just did an IO and got no bcasts
	status_i = OMNIAPI_NOT_FOUND;
    }
    // release the synchronisation lock so other threads can do OMNIAPI IOs
    ISE_MUTEX_UNLOCK ( click_px->omni_io_active_mutex );

    if ( status_i < SUCCESSFUL)
    {
	// for some reason the NOT_FOUND status, which is common, is in the error range (-ve)		 
	if ( status_i != OMNIAPI_NOT_FOUND)
	{
	    // real error
	    // report the error
	    report_click_error ( click_px,
				    status_i,
				    0);		    // no secondary status
				        // no Transaction ID
	    // let error code drop us out of the broadcast thread
	}
	else
	{
	    // we got a -ve status BUT this is just a "no data is available",
	    // which should be a good status - force it to be good, so we don't drop out of loop
	    status_i = SUCCESSFUL;
	}
    }
    else
    {
	/* We retrieved some events successfully - we have a broadcast buffer
	    which consists of a set of subscribed_event_t structures each
	    followed by a byte stream, whose length is descibed in the
	    subscribed_event_t structure
	/* Check that we haven't inadvertently missed some */

	if ( status_i == OMNIAPI_OVERFLOW )
	{
	    /* The gateway buffer has overflowed - we have missed some 
		broadcasts. Tell them about it, but at least try
		to download the other broadcasts being held in the gateway
	    */
	    printf ("\nWARNING At least one broadcast has been lost\n");
	}
	else
	{
	    // do nothing
	}

	Buffer_pc = bcast_rx_buff_px;

	// Iterate until we have handled all events in the buffer
	// and while no errors in processing the bcasts in this buffer
	// NB we know that there is at least one subcription event.
	// AND we are still logged on.. try to narrow the window of 
	// processing lots of bcast data, while logging off
	do
	{
	    // point to one subscribed event within the broadcast block
	    subscribed_event_px = (subscribed_event_t *)Buffer_pc;

	    /*
	    20000503 .. if there are forced subscriptions, these appear to be delivered
	    in the format WITHOUT the subscription handle i.e. as if we had NOT used
	    subscription lists. This is a problem, as we are there trying to interpret
	    the actual header e.g. 'B' 'I' as the length.. and it all goes pear shaped
	    from there.

	    I am confirming with OM that this behaviour is to be expected, if mixing
	    forced broadcasts with optional subscriptions (with subscription lists used)
	    and if so, will have it updated in the ISE operations procedures to never do.

	    In the meantime, try coping with it.
	    
	    If a broadcast is delivered which was requested by a subscription list,
	    is has a subscription handle.. which is always -1 (a previous problem reported
	    to OM). For now, if the subscription handle part does NOT return a -1,
	    then assume that there is no subscription handle.. 
	    */
	    if (subscribed_event_px->handle_u != UNDEFINED_UINT32)
	    {
		// broadcast did NOT have a subscription handle.. on the assumption noted above
		// fudge.. point subscribed_event_px four bytes less.. horrible hack
		(unsigned)subscribed_event_px -= 4;

		// one more broadcast
		broadcasts_done_i++;

		// Now examine the contents of the actual broadcast
		status_i = process_one_bcast (
				click_px,
				(broadcast_type_t*)((int8*)Buffer_pc + 
				    event_hdr_size_u ),
				subscribed_event_px->evelen_n,
				UNDEFINED_UINT32	// supply handle as we would expect from OMnet
				);

		/* Move the current buffer pointer past the message we just read
		 * to align it with the start of the next subscribed_event_t structure
		 */
		Buffer_pc += event_hdr_size_u + subscribed_event_px->evelen_n;

		/* if there is insufficient room for another message (header etc)
		    then bail out.. DO NOT try to figure out when to finish based
		    on whether the 'next' buffer has a certain size.. that pointer
		    may already be past the end of the buffer, and pick up
		    any rubbish, or get an access violation.
		    The tail of the buffer is padded with zeroes.. but only to the
		    nearest 4 byte boundary.
		*/
		rem_space_u = (uint32) ( bcast_rx_buff_px + bcast_buff_len_u - Buffer_pc );
		if ( rem_space_u <= event_hdr_size_u )
		{
		    // end of buffer - bail out
		    break;
		}
	    }
	    /* Now we can examine the event that caused the message to be raised 
		The actual message starts immediately after the subscription handle
		(which describles the subscription which generated the update message)
		and the event length field
	    */
	    else if (subscribed_event_px->evelen_n > 0)
	    {
		// one more broadcast
		broadcasts_done_i++;

		// Now examine the contents of the actual broadcast
		status_i = process_one_bcast (
				click_px,
				(broadcast_type_t*)((int8*)Buffer_pc + 
				    event_hdr_size_u ),
				subscribed_event_px->evelen_n,
				subscribed_event_px->handle_u
				);

		/* Move the current buffer pointer past the message we just read
		 * to align it with the start of the next subscribed_event_t structure
		 */
		Buffer_pc += event_hdr_size_u + subscribed_event_px->evelen_n;

		/* if there is insufficient room for another message (header etc)
		    then bail out.. DO NOT try to figure out when to finish based
		    on whether the 'next' buffer has a certain size.. that pointer
		    may already be past the end of the buffer, and pick up
		    any rubbish, or get an access violation.
		    The tail of the buffer is padded with zeroes.. but only to the
		    nearest 4 byte boundary.
		*/
		rem_space_u = (uint32) ( bcast_rx_buff_px + bcast_buff_len_u - Buffer_pc );
		if ( rem_space_u <= event_hdr_size_u )
		{
		    // end of buffer - bail out
		    break;
		}
	    }
	    else
	    {
		/* as the evelen_n field is unsigned, if it is not greater than 0
		    then it is 0.

		    As we know we do NOT enter the do loop unless there is at least
		    one event, and we check for end-of-buffer after each event,
		    if we hit a 0 event length, we have a problem
		*/
		printf ("Possible Error in the API - Event length 0, tot size %u, buff addr %u, up to %u\n",
			    bcast_buff_len_u,
			    bcast_rx_buff_px,
			    Buffer_pc );

		status_i = API_ERROR;

		// bail out of do loop, all done
		break;
	    }

	} while (( click_px->logged_on_i == TRUE ) && ( status_i == SUCCESSFUL ));
    }	// check for good status from OMNIAPI call

    // if error (-ve status) return that, otherwise return
    // how many bcasts we got
    if (status_i < SUCCESSFUL)
	return (status_i);	    // return the error
    else
	return broadcasts_done_i;   // worst case is 0, which is SUCCESSFUL
}   // read_bcast_stream

// ---------------------------------------------------------------------------

static void bcast_io_thread ( click_details_xt *click_px )
/*
Function:   bcast_io_thread
Author:     Neil Cossor
Modified:   20011017
Description:

    The main thread of the broadcast and IO handler.
    This does all the read work.
*/
{
    // status / count of bcasts done in one pass through our main loop
    int status_i;

    // some stats info - timestamp before we do the call, and timestamp after (shows net/gw pauses)
    Timeb_t		starttime_x;
    Timeb_t		finishtime_x;
    // and those times in milliseconds
    unsigned __int64		starttime_f = 0; 
    unsigned __int64		finishtime_f = 0;
    // the last time we called the routine
    unsigned __int64		lasttime_f = 0;
    // time diff between calls, in milliseconds.. uint32 is big enough for 4
    // million seconds.. or 49 days
    uint32		elapsedtime_u = 0;
    // log string, for us to log this interesting event
    char		error_msg_s [120];

    // set initial time
    lasttime_f = 0;

    // repeat until logged off
    do	// while logged on
    {
	// note the start time before our call
	ftime( &starttime_x );
	starttime_f = (starttime_x.time*1000) + starttime_x.millitm;

	// is this a long time since our last call ? (allowing for first call)
	// we are looking for local node hold-ups.. process time-slicing etc
	// but allow for how long we Sleep for, and how long our last poll took
	if ( lasttime_f != 0 )
	{
	    elapsedtime_u = (uint32)(starttime_f - lasttime_f - elapsedtime_u);
	    if ( elapsedtime_u > system_poll_limit_u )
	    {
		sprintf ( error_msg_s, 
			    "delay between omniapi_read_event_ext() calls of %u milliseconds\n",
			    elapsedtime_u );
		log_text ( (char *)&error_msg_s );
	    }	// check if too long around the bcast loop.. local CPU holdup ?
	}   // check if first time thru the loop

	// save the time of this call, to check next time
	lasttime_f = starttime_f;

	// read and process as many broadcasts we get in one read
	status_i = read_bcast_stream ( click_px );

	// note time after the call
	ftime( &finishtime_x );
	finishtime_f = (finishtime_x.time*1000) + finishtime_x.millitm;
	// how long was our wait, in milliseconds, for this poll
	elapsedtime_u = (uint32)(finishtime_f - starttime_f);

	// did the call take a long time ? i.e. are we held up elsewhere on node ?
	if ( elapsedtime_u > bcast_poll_limit_u)
	{
	    sprintf ( error_msg_s, 
			"slow omniapi_read_event_ext() of %u milliseconds\n",
			elapsedtime_u );
	    log_text ( (char *)&error_msg_s );
	}   // check if too long for single poll for bcast - network or gateway problem ?

	// if we found nothing to do (no broadcasts), then have a little
	// sleep - give other threads / processes a go - fairness contention
	if ( status_i <= 0 )		    // error, or no broadcasts found on this pass
	{
	    // lets snooze for a few NT clock ticks
	    Sleep ( bcast_poll_delay_u );   // milliseconds (changed from 10, 991130)
	}
    } while (( click_px->logged_on_i == TRUE ) && ( status_i >= SUCCESSFUL));

    // release the module-private structures
    free_gen_buf_node_q ( &authorised_bcasts_q, TRUE );

    // report why the thread is closing
    if ( click_px->logged_on_i == FALSE )
    {
	printf ("Broadcast thread closing - logged off\n");
	return;
    }   // check, whether we are closing the loop because we are logged off



    // if we get here, we must have had a bad status
    printf ("Broadcast thread closing, bad status %i\n",
		status_i );
    // if we were logged on, then we need to tidy up, so user can try to log on again
    if ( click_px->logged_on_i == TRUE )
    {
	om_disconnect ( click_px );
    }	// check if logged on, when we got the bad bcast status

}   // bcast_io_thread

// ---------------------------------------------------------------------------

int32 init_bcast_io_thread ( click_details_xt *click_px )
/*
Function:   init_bcast_io_thread
Author:     Neil Cossor
Modified:   991130
Description:

    This fn, called once we have logged on (and subscribed to the appropriate
    list of broadcasts), starts a thread which handles all subsequent
    broadcast reading / processing with the OMAPI, until we are logged off.

    It does this, by creating a thread, which spins around, doing reads (and
    processing of broadcasts). When it is actually about to do an IO, it
    first grabs a synchronised lock, and releases it upon completing the IO.

    The main thread of the app does the same deal around its OMNIAPI IOs,
    so we cant end up with simultaneous calls to the OMNIAPI functions..
    which is NOT allowed.

    The bcast_io thread terminates (after releasing any resources) once
    it observes that we are logged off (check each time around the loop).

    All of this is done so that we can continue to whip out all of the
    stream of broadcasts from the gateway, while the main thread is
    doing things which might be delayed, like waiting for keyboard input,
    or doing a scripted pause.

    Note also, that if the processing of a broadcast manipulates the common
    (for example if we received a broadcast which defined a new series),
    then the update of that needs to be synchronised with the access
    to the common being done by the main thread.
*/
{
    // our return status, with an initial default value
    int     status_i = SUCCESSFUL;

    // initialise the module-private structures
    init_q ( &authorised_bcasts_q );

    // go and set up the subscription..this includes getting the list of authorised bcasts
    // and MAY include subscribing to some broadcasts (e.g. BI9 heartbeat)
    status_i = subscribe_for_events (click_px);
    // if that initial setup failed, then there is no point starting the thread
    if ( status_i < SUCCESSFUL )
	return status_i;


    // start up the thread which looks after this click channel
    status_i = ISE_THREAD_CREATE  ( bcast_io_thread,	    // main fn of created thread
				    (click_px));	    // 32 bit value passed to thread
							    // - pointer to our struct

    // a +ve status means that it worked, and the status is the handle for the thread
    // BUT we want to return the success status.. NOT some arbitrary handle value
    if (status_i > 0)
    {
	status_i = SUCCESSFUL;

	// we wish to wait here until the bcast_io_thread has actually successully
	// started up, found about authorised bcasts etc.
    }

    return status_i;
}   // init_bcast_io_thread

// ---------------------------------------------------------------------------

void open_tradefile ( click_details_xt *click_px )
/*
Function:   open_tradefile
Author:	    Neil Cossor
Modified:   20000601
Description:

    Open the trade logfile, for BD111 and BD102 hacks above.. NB they flush after
    each trade write (trades are uncommon c.f. BBOs, OPRA prices etc).

    We don't actually have an explicit file close.. but provided the flushes
    were timely, the implicit close in the application exit should do the trick
    anyway.
*/
{
    char    tradefilename_s[60] = "";

    if (tradefile_px != NULL )
	printf ("Trades already being logged\n");
    else
    {
	// attempt to open the trade log file.. include the business date, and PROD vs TEST in filename
	if ( click_px->facility_no_u == OMNI_FACTYP_ISE_XUSEP0 )
	    strcat ( tradefilename_s, "PROD_TRADES_" );
	else
	    strcat ( tradefilename_s, "TEST_TRADES_" );

	strcat ( tradefilename_s, click_px->current_business_date_s );
	strcat ( tradefilename_s, ".LOG" );

	tradefile_px = fopen ( tradefilename_s, "a+" );
	if ( tradefile_px == NULL )
	{
	    printf ("Error opening %s for trade logging\n", &tradefilename_s );
	}
	else
	{
	    printf ("Starting trade logging into %s\n", &tradefilename_s );
	}
    }	// test if tradefile already open
}   // open_tradefile

// ---------------------------------------------------------------------------

void bcast_set_poll_delay ( uint32 new_poll_delay_u )
/*
Function:   bcast_set_poll_delay
Author:	    Neil Cossor
Modified:   20001219
Description:

    We have been given a new interval to set for the delay between polls
    for bcasts (in milliseconds).
*/
{
    bcast_poll_delay_u = new_poll_delay_u;
}   // bcast_set_poll_delay

// ---------------------------------------------------------------------------

void bcast_set_poll_limit ( uint32 new_poll_limit_u )
/*
Function:   bcast_set_poll_limit
Author:	    Neil Cossor
Modified:   20011017
Description:

    We have been given a new interval to set for the limit of the time taken
    for one bcast poll (in milliseconds), before adding warning into log.
*/
{
    bcast_poll_limit_u = new_poll_limit_u;
}   // bcast_set_poll_limit

// ---------------------------------------------------------------------------

void bcast_set_system_poll_limit ( uint32 new_system_limit_u )
/*
Function:   bcast_set_system_poll_limit
Author:	    Neil Cossor
Modified:   20011017
Description:

    We have been given a new interval to set for the limit of the time taken
    in one interpoll loop by the system (on top of actual IO time, and voluntary Sleep),
    before adding warning into log.
*/
{
    system_poll_limit_u = new_system_limit_u;
}   // bcast_set_system_poll_limit

