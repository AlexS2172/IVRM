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


Title:	utility.h
Author: Neil Cossor
Description:

    Header file for utility.c, which provides all sorts of utility
    fns to ISEsim.c

Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
000105		Neil Cossor update for latest OM .h files (15.7 OM release)
			    remove our own swapint64 routine (OM now supply one)
000110			    add header for subscr_list_bin, formerly private to broadcast.c
000114			    add new routine, flush_gen_buf_node_q
000122			    import several to/from double routines from autoqute.c
000213			    add defn (if not WIN32) for _strupr (moved from command.c)
			    add (updated) swapint64 for Solaris users, until new .o files arrive
000403			    add bin_under_series_s (Under|Series|Bin|* selector)
000505			    add market_status_to_string
000515			    mod bid_ask_s to include BOTH as selector (change cmd_bid_ask too)
000530			    added fprintf_time(), for use in logging trades (ISE and away)
0012xx	v2.00		    start updates for OM release 16.3 (from 15.7-3)
010319			    rename uint_from_double_with_round to fract_from_double_with_round,
			    add new routine to decimal_from_double_with_round
010620			    added cmd_user_member fn

*/
#ifndef UTILITY_H
#define UTILITY_H

// ---------------------------------------------------------------------------

// the allowed choices for various 'named' selectors
static char *bid_ask_s [] =
{
    "BOTH",	    // can be used for query, wildcard delete
    "BID",	    // NB NOT in alphabetical order..
    "ASK",
    "%O",	    // use bid/ask from last order that we snapshot-saved
    "Unknown"	    // for invalid selector
};

static char *validity_time_s [] =
{
    "FOK",
    "GTC",
    "IOC",
    "ROD",
    "FASDAY",
    "FASGTC",
    "FAS",
    "AONDAY",
    "AONGTC",
    "AON",
    "Unknown"
};

static char *ul_price_methods_s[] = {
	"NO BOUNDARIES",
	"BID",
	"ASK",
	"LAST PAID",
	"ADJUSTED LAST PAID"
};

static char *order_selects_s[] = {
	"GTC",
	"DAY",
	"MINUTES",
	"IMMEDIATE",
	"PART ORDER",
	"WHOLE ORDER",
	"LOCKED ORDERS",
	"ACTIVE ORDERS"
};
  

static char *actions_s[] = {
	"CHANGE TO",
	"REDUCE BY",
	"CHANGE TO / CHECK EQUAL",
	"CHANGE TO / CHECK GREATER",
	"CANCEL AND REPLACE"
};
// and the enumeration type which corresponds to the strings
enum validity_time_et { valid_fok_e, valid_gtc_e, valid_ioc_e, valid_rod_e,
			valid_fas_day_e, valid_fas_gtc_e, valid_fas_time_e,
			valid_aon_day_e, valid_aon_gtc_e, valid_aon_time_e };

// same deal for order types
static char *order_type_s [] =
{
    "LIMIT ",
    "MARKET",
    "Unknwn"
};
enum order_type_et { order_type_limit_e, order_type_market_e };

// same deal for strike price groups
static char *strike_price_group_s [] =
{
    "DIN ",
    "IN  ",
    "AT  ",
    "OUT ",
    "DOUT",
    "*",	    // wildcard input
    "Unkn"
};
enum strike_price_group_et { strike_price_deepin_e,
				strike_price_in_e,
				strike_price_at_e,
				strike_price_out_e,
				strike_price_deepout_e,
				strike_price_all_e };

// similar deal for underlying | series | * selection for many commands
static char *under_series_s [] =
{
    "UNDERLYING",
    "SERIES",
    "*"
};
enum under_series_et { under_series_under_e,
			under_series_series_e,
			under_series_all_e };

// similar deal for underlying | series | bin selection for many commands
static char *bin_under_series_s [] =
{
    "UNDERLYING",
    "SERIES",
    "BIN",
    "*"
};
enum bin_under_series_et { bin_under_series_under_e,
			    bin_under_series_series_e,
			    bin_under_series_bin_e,
			    bin_under_series_all_e };

// same deal for order types
static char *order_category_s [] =
{
    "Order",
    "Quote",
    "Block",
    "BlockResponse",
    "Facilitation",
    "FacilResponse",
    "CABOrder",
    "DerivedOrder",
    "ImproveQuote",
    "ComboLimit",
    "ComboInterest",
    "UnknownCatgry"
};
// and the enumeration type which corresponds to the strings
enum order_category_et { order_cat_order_e,
			    order_cat_quote_e,
			    order_cat_block_e,
			    order_cat_block_response_e,
			    order_cat_facilitation_e,
			    order_cat_facilitation_response_e,
			    order_cat_cab_e,
			    order_cat_derived_e,
			    order_cat_improve_quote_e,
			    order_cat_combo_limit_e,
			    order_cat_combo_interest_e
};

// declaration of string choices for user vs member subcommands
// NB, by putting Member 1st, the default setting in global (value 0) is member, which is preference.
static char *user_member_s [] =
{
    "MEMBER",
    "USER"
};
// enumerated list, corresponding 1:1 (and in same order) as string list
enum user_member_et { user_member_member_e,
			user_member_user_e
			};

/* the divide #, for each 10^n level, n=0..9
    10^10 is bigger than you can fit into a strike/permium price i.e. a 4byte number
    We use this constant array of dividers, as the only C fn which appears
    to be able to raise one number to a power is a double-precision
    floating point operation, which are horribly inefficient, when we know
    we are only dealing with powers of 10, over a limited range.

    Used by read_decimal_string, and format_whole_decimal_s fns
*/
static int dividers_i[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };






// ------------------------- and the routines --------------------------------

#ifndef WIN32
    void _strupr (char *p);
    /*
    Function	_strupr
    Author	Joshua P
    Modified	20000104
    Description:

	In WinNT, _strupr is provided, which converts a whole string
	to upper case.

	On Solaris, no _strupr routine, provide one
    */
#endif

// ---------------------------------------------------------------------------

int select_string_option ( int choices_u,
			    char *choices_s[],
			    char *input_s );
/*
Function:   select_string_option
Author:     Neil Cossor
Modified:   9908xx
Description:

    We have an input string, which we (hope) selects one of those
    from a (compile-time) known list of string choices.

    The input string (which is assumed to be the same case as the strings
    in the search array), is supposed to uniquely match against one
    of the strings in the supplied string array.

    A match may be a substring.

    It is possible for multiple matches.. which is an error.
    For example if there are choices LOGGER, and LOAD, then and input
    string of LO would be ambiguous.

    To check for ambiguity, we compare the input string
    against every string in the choices_s array, unless of course,
    we have already found that it is an ambiguous choice.

    We assume that the list of possible choices is fairly small,
    or this would be rather inefficient.

    If the caller maintains a enumerated list (starting from 0)
    which corresponds 1:1 with the choices list, then the value
    of our return integer can be used directly in a comparison
    (or switch) against the enumerated choices.

Input Params:

    The number of string choices we have
    The array of possible strings (with no duplicates)
    The string which we are trying to identify (uniquely match)

Output Params:

    If a (unique) match is found, then return the index # of the matching string
    which will, of course, have a value 0..(choices_u-1)

    If no match is found, then return the value choices_u

    If multiple matches are found (ambiguous string), then return the value (choices_u+1)
*/

// ------------------------------------------------------------------------------

void display_help_screen ( void );
/*
Function : display_help_screen
Author	 : Neil Cossor
Modified : 9905xx
Description:

    Displays a list of implemented commands onto standard output
*/

// ------------------------------------------------------------------------------

int32 get_user_information ( click_details_xt * click_px);
/*
Function : get_user_information
Author	 : Neil Cossor 
Modified : 9905xx
Description:

    Downloads the user code (from which you can
    determine the country in which you are working)
    and the first business facility code

Implicit params:

    Several fields in the common CLICK structure are updated
    presuming, of course, that we have success getting the data from
    the OM system
*/

// ------------------------------------------------------------------------------

char *read_kbd_input ( char *Buffer_ps );
/*
Function : read_kbd_input
Author	 : Neil Cossor 
Modified : 9905xx
Description:

    Accepts a line from the terminal. If CTRL-Z is entered,
    NULL is returned.

    NB converts data to uppercase.
    NB does NOT have any check on the size of the input vs size of the
    target buffer
*/

// ------------------------------------------------------------------------------

/*
Function : save_user_param_file
Author	 : Neil Cossor 
Modified : 9905xx
Description:
 
    Saves the user details to a User param file for later use.
    This includes username, gateway name and port, and password.

    NOTE that as the file is plaintext, this leaves the password insecure...

Input Params:

    Pointer to the name of the filename we should be saving in.
    If not supplied, then default (USER_DATA_FILENAME) is used.

Implicit Params:

    The extern variables (from global.h) which contain the
    actual data we wish to save. Inelegant, but functional.
*/
void save_user_param_file ( click_details_xt *click_px,
			    char * user_filename_ps );

// ------------------------------------------------------------------------------------

void pack_OM_header ( generic_txn_xt	*tx_msg_px,	    // pointer to a generic transaction message
			uint32		tx_length_u,	    // length of message (not including OMnet heading)
							    // e.g. sizeof (omni_login_message)
			uint8		central_module_u,   // transaction type central module field	e.g. B
			uint8		server_type_u,	    // transaction type server type field	e.g. 0
			uint16		tranaction_no_u );  // transaction type transaction number field    e.g. 101
/*
    Creates and fills in a framework OM transaction header.
    This header includes the transaction length and transaction type.  
*/

// ----------------------------------------------------------------------------

int32 who_am_i ( click_details_xt *click_px );
/*
Function : who_am_i
Author	 : Neil Cossor 
Modified : 9905xx
Description:

    Displays all ID info for logged-on user.
*/

// ------------------------------------------------------------------------------

/*
Function : load_previous_user
Author	 : Neil Cossor 
Modified : 9905xx
Description:

    If a user file exists, loads the details of the previous user.
    This includes the username, name of Gateway, Gateway port #.
    If also include the password... which is NOT secure,
    as it is just a text file.

Input Parameters:

    pointer to all info about our connection with CLICK, including username etc
    (optional) name of user filename.
    Defaults to USER_DATA_FILENAME if not supplied
*/
int32 load_previous_user( click_details_xt * click_px,
			    char * user_filename_ps );

// ------------------------------------------------------------------------------

/*
    routine to transfer an rx'ed click string into a C-style string.

    The input is a fixed-size character array. The input string may or may not be
    NUL terminated (CLICK docs say not, but evidence suggests that they sometimes are).
    We should strip off trailing blanks.

    The target buffer is assumed to be (at least) limit_size+1 long, to allow
    us to put a terminating NUL, even if the source string contains all-significant characters.
*/
void rx_click_string ( void * target_ps,
		void * source_ps,
		int limit_size );

// ------------------------------------------------------------------------------

/*
    routine to transfer a C-style string into CLICK format for tx.

    The input is a fixed-size character array. The input string is always NUL terminated.
    The output string MUST be SPACE padded.
*/
void tx_click_string ( void * target_ps,
		void * source_ps,
		int limit_size );
		
// ------------------------------------------------------------------------------

// routine to put a blank string in CLICK format for tx.
void tx_null_click_string ( void * target_ps,
			    int limit_size );

// ------------------------------------------------------------------------------

void put_ise_user_code_xt ( user_code_t * click_user_code_px,
			    ise_user_code_xt * ise_user_code_px );
/*
    Write (to a CLICK msg) the fields of a user_code_t.
    NB user_code_t == trading_code_t
*/

//-----------------------------------------------------------------------------------

void get_ise_user_code_xt ( ise_user_code_xt * ise_user_code_px,
			    user_code_t * click_user_code_px );
/*
    Read (from a CLICK msg) the fields of a user_code_t.
    NB user_code_t == trading_code_t
*/


//-----------------------------------------------------------

uint16_t cmd_order_select (char * order_select_s);
/*
Function:   cmd_order_select
Author:     Eric Salem
Modified:   Apr 9,2002
Description:

    Translates a string to a uint16_t for order_select_n

Input params:

    The string which supplied on the command line.

Output params:

  0 == GTC
  1 == DAY
  2 == MINUTES
  3 == IMMEDIATE
  4 == PART ORDER
  5 == WHOLE ORDER
  6 == LOCKED ORDERS
  7 == ACTIVE ORDERS
    
*/
char cmd_ul_price_method(char * method_s);

// ------------------------------------------------------------------------------

int do_click_query_rx ( click_details_xt * click_px,
			generic_txn_xt * tx_request_px,
			int tx_request_size_u,
			uint32 rx_fn_code_u);
/*
    We wish to make a query request of CLICK.
    The request to send is packed into the buffer pointed to by tx_request,
	with the size supplied.

    If unsuccessful, then report the error, and log the our_click->log_out_head error,
    with a MSG_K_STATUS message type.

    If successul, we optionally log the response buffer.
    If rx_fn_code_u parameter is non-zero, then we log the response.
    This can be done where we know the type (and format) of the response.
    If there are alternative responses, then rx_fn_code_u should be supplied as a 0.
    We will not log the response, and leave the logging (with the appropriate format
    qualifiers) to the caller.
    
    The facility code to send MUST have been filled in the tx buffer by the caller.

    We return -1 if an error was returned, 0 if succesful.
*/

// ------------------------------------------------------------------------------

int do_click_tx_rx ( click_details_xt * our_click,
		    generic_txn_xt * tx_request_px,
		    int tx_request_size_u );
/*
    We wish to do a tx/rx with CLICK. These are typically used where there is no
    response, other than the status i.e. no rxed as part of the operation.

    The behaviour, initial parameters etc are exactly the same as do_click_tx_rx,
    excepting the specific internal handling that is different between omniapi_query
    and omniapi_tx.

    There are less params in an omniapi_tx,
    and the "send" buffer is passed by the address of a tx vector, which has the first
    entry pointing to the caller's buffer, while query passes the address of the buffer.

    As per do_click_query_rx for input parameters, return statii.

    NB the facility_u, in the buffer, may be one of the "specials", like OMNI_FACTYP_LOGIN
*/

// ------------------------------------------------------------------------------

gen_buf_node_xt * new_gen_buf_node ( int size_of_msg_buffer );
/*
    fn which allocates a gen_buf_node_xt, in preparation for the caller
    to add more stuff to it, set additional flags, etc
    It zeroes the msg length field of the structure
    It zots the "special purpose" flags to clear / false / zero.

    Input parameters are
    the size of msg buffer to allocate in the gen_buf_node_xt
*/

// ------------------------------------------------------------------------------

void add_to_q_tail(struct node_header *header,
		    struct q_chain_t *new_node);
/*
    Adds msg to a buffer Q.
    Takes a gen_buf_node_xt as input, and inserts it into the
    nominated Q, at the tail
*/

// ------------------------------------------------------------------------------

void remove_buff_from_q (struct node_header *header,
			    struct q_chain_t *tmp_node);
/*
    fn to remove a nominated node from the nominated Q.
    It may be at the head, tail, combinations, or somewhere in the middle

    The real work of this fn is to make sure that the pointers in the
    chain are intact AFTER this entry is removed
*/

// ------------------------------------------------------------------------------

struct q_chain_t * remove_front_from_q (struct node_header *header );
/*
    fn to remove the front node from the nominated Q.
    Returns a NULL if the Q is empty.

    The real work of this fn is to make sure that the pointers in the
    chain are intact AFTER this entry is removed.

    It returns the pointer to the Q chain structure which is the first field in
    whatever structure it is in i.e. it returns pointer to structure which (was) in a Q
*/

// ------------------------------------------------------------------------------

void init_q ( struct node_header *header );
/*
    initialise a Q header.
    This includes creation of the OS-specific synchronisation locks for the Q header
*/

// ------------------------------------------------------------------------------

int empty_q (struct node_header *header );
// returns true (non-zero) if Q is empty, false otherwise

// ------------------------------------------------------------------------------

void flush_gen_buf_node_q (node_header_xt *header_px,
			    int release_child );
/*
    release all buffers on a Q, 
    DO NOT delete the actual Q object itself

    This is a specific routine for releasing entries in a Q of gen_buf_node_xt's,
    as these have a substructure which was also allocated, and hence must be deallocated.

    Note that there are some cases, where the block of data pointed to by msg_p
    was NOT allocated in new_gen_buf_node.. and we should NOT release the stuff
    pointed to by it.. that free() is the responsibility of whoever did allocate it.

    To kill the child, then supply a TRUE for release_child, otherwise supply FALSE
*/
// ------------------------------------------------------------------------------

void free_gen_buf_node_q (node_header_xt *header_px,
			  int release_child );
/*
    release all buffers on a Q, and the (OS-specific) synchronisation control
    object for that Q i.e. we are destroying the Q

    This is a specific routine for releasing entries in a Q of gen_buf_node_xt's,
    as these have a substructure which was also allocated, and hence must be deallocated.

    Note that there are some cases, where the block of data pointed to by msg_p
    was NOT allocated in new_gen_buf_node.. and we should NOT release the stuff
    pointed to by it.. that free() is the responsibility of whoever did allocate it.

    To kill the child, then supply a TRUE for release_child, otherwise supply FALSE
*/

// ------------------------------------------------------------------------------

void printf_time ();
/*
    fn which grabs the current date/time, and spits out the result in readable form to stdout
    It does NOT add any newlines etc, just a space after the date/time.
*/

// ------------------------------------------------------------------------------

void unpack_click_bit_date ( uint16 date,
		    uint16 * yyyy_u,
		    uint8 * mm_u,
		    uint8 * dd_u );
/*
    used to extract the (bit-packed) date format used by CLICK in series,
    and put the results into the three supplied
    variables.

    The bit-packed date format is a 16-bit number, with
	7 bits giving the offset year, with the offset being 1989. So a value of 1 == 1990
	4 bits gving the mongth, where 1 = January
	5 bits giving the day of month.
*/

// ------------------------------------------------------------------------------

void pack_click_bit_date ( uint16 *date,
		    uint16 yyyy_u,
		    uint8 mm_u,
		    uint8 dd_u );
/*
    used to pack the (bit-packed) bit-field packed date used by CLICK in series.
    Use as inputs the three supplied values.
    
    The bit-packed date format is as per unpack_click_bit_date (above)
*/

// ------------------------------------------------------------------------------

void free_gen_buf_node (gen_buf_node_xt *tmp_node_px );
/*
    release one gen_buf_node_xt.

    If this gen_buf_node_xt has a substructure allocated (pointed to by msg_p)
    then release that too.

    If this buffer is still currently on a Q,
    then things will go DRASTICALLY WRONG
*/

// ------------------------------------------------------------------------------

void format_whole_decimal_s ( char *formatted_string_ps,
				int32 input_u,
				uint32 decimals_u,
				uint32 max_width_u );
/*
Function: format_whole_decimal_s
Author:   Neil Cossor
Modified: 20010607
Description:

    From the given unsigned 32 bit number (e.g. a strike price, and underlying
    price, a bid/ask price), figure out a "nice" output string,
    and return that to the caller in the string provided.

    Each of these input data types has an attribute which determines the
    implied # of decimal places.
    
    Use that to produce a string with the <whole>.<decimals>
    correctly formatted.

Input Params:

    pointer to the string buffer to update
    the input number to format
    the number of implied decimals.. 0..9
    the total width of the output string (plus terminating NUL)

Output Params:

    We modify the supplied string pointer to 
*/

// ------------------------------------------------------------------------------

int read_decimal_string_int ( char *input_string_ps,
				uint32 decimals_u );
/*
Function: read_decimal_string
Author:   Neil Cossor
Modified: 9906xx
Description:

    From the given numeric string, and the given implied decimal places, figure out
    how this number should be represented in the target (integer) longword.

    If the input string has no ".", then we just shift left (decimal) by the
    appropriate # of positions.

    If the input string has a ".", then there must be at least one numeric digit
    after the decimal point.

    If there are more digits on the RHS of the decimal point than are allowed
    (in the decimal places), then this is an error.

    If there are less digits on the RHS, then we pad the RHS with the requisite # of
    zeroes.

    If the total # of digits in the LHS and RHS combined are greater than
    what can fit into an int i.e. > 9, then an error.

    For example,

    for an input of "23", digits 4 -> 230000
    for an input of "65.1", digits 4 -> 651000
    for an input of "40.12345", digits 4 -> error, truncated to 40.1234

Input Params:

    pointer to the string buffer to read from
    the number of implied decimals.. 0..9

Output Params:

    the (packed) resulting #...

    Note that as we are using sscanf, you could have "40. 35", with digits 4,
    and this would be interpreted as 40.3500.. rather than as an error
*/




// ----------------------------------------------------------------------------------
uint8 cmd_action(char * action_s); 
	/*
Function:   cmd_action
Author:     Eric Salem

Description:

    The user has a command which (should have) selected a action type for mo103.
    From the supplied string, return an appropriate value.
Input params:

    The string which supplied on the command line.
    
Output params:

    1 == "Change To",
	2 == "Reduce By",
	3 == "Change To / Check Equal",
	4 == "Change To / Check Greater",
	5 == "Cancel and Replace",
*/


// ----------------------------------------------------------------------------------



uint8 cmd_bid_ask ( click_details_xt * click_px,
		    char *bid_ask_s );
/*
Function:   cmd_bid_ask
Author:     Neil Cossor
Modified:   9907xx
Description:

    The user has a command which (should have) selected a BID or ASK.
    From the supplied string, return an appropriate value, as used in
    the bid_ask_c field in many messages.

Input params:

    The string which supplied on the command line

Output params:

    The value to use. As per DFI
    1 == BID
    2 == ASK
    0 if illegal selection supplied
*/

// ------------------------------------------------------------------------------------

uint16	cmd_validity_time ( char *validity_time_input_s,
			    uint16 default_validity_time_u );
/*
Function:   cmd_validity_time
Author:     Neil Cossor
Modified:   20010526
Description:

    The command line has a validity time string selector.
    Convert this to a validity time numeric value, as per DFI.

Input Params:

    the command line parameter string.
	NB since 16.5 we now have "timed" flavours for AON and FAS.
	For now, I recognize FASday, FASGTC, and if I just get a match
	on FAS, I assume timed FAS. I need to figure a "nice" commandline
	way of allowing a number to specified, without changing everything else.

	It should be one of
	    FOK Fill or Kill
	    GTC Good Till Cancel
	    IOC Immediate or Cancel
	    ROD Rest of Day.
	    FASday
	    FASGTC
	    FAS timed.. trickier
	    AONday
	    AONGTC
	    AON timed.. trickier

	    Note that GTC and ROD are really flavours of FaS - Fill and Store.

    the default "unit" to use, if no selector string is supplied.
	Only the "timed" flavours of validity time require a value for the
	"value" byte, so skip that.. assume we will be defaulting to
	one of the flavours which can be uniquely specified without a time.

Output Params:

    The encoded value, is a string was supplied, and mapped to a valid choice.

    If no input string is supplied,
    then it generates a validity time based on the "unit" value supplied in
    the default_validity_time_u parameter.

    If a string was supplied, but string does not pick a valid choice,
    then a 0 is returned, which will map to the soon-to-be-deprecated version of IOC
*/

// ------------------------------------------------------------------------------------

uint8	cmd_client_category ( char *client_category_s,
				uint8 default_client_category_u );
/*
Function:   cmd_client_category
Author:     Neil Cossor
Modified:   991102
Description:

    The command line has a client category string
    Convert this to a client category numeric value, as per DFI.

Input Params:

    the command line parameter string.
	It should be one of
	CLIENT
	FIRM
	FARMM

    the default value to use, if no selector string is supplied

Output Params:

    the encoded value,
    or 0 if the user did NOT supply an allowed choice.

    An error msg is displayed on stdout in this case
*/

// ------------------------------------------------------------------------------------

uint8 cmd_order_type ( char * order_type_s,
			uint8 default_order_type_u );
/*
Function:   cmd_order_type
Author:     Neil Cossor
Modified:   991102
Description:

    The command line has an order type string
    Convert this to a order type numeric value, as per DFI.

Input Params:

    the command line parameter string.
	It should be one of
	LIMIT
	MARKET

    the default value to use, if no selector string is supplied

Output Params:

    the encoded value,
    or MAX_UINT8 if the user did NOT supply an allowed choice.

    An error msg is displayed on stdout in this case
*/

// ------------------------------------------------------------------------------------

void cmd_orderID ( click_details_xt *click_px,
			char * order_ID_s, quad_word *delete_orderID_u  );
/*
Function:   cmd_orderID
Author:     Neil Cossor
Modified:   9907xx
Description:

    The command line has an order ID string.
    Convert this to a order ID numeric value, as a quadword

Input Params:

    the command line parameter string.
    It should be
    xxxxxxxx:xxxxxxxx

Output Params:

    the encoded value,
    or binary 0 if not translatable

    An error msg is displayed on stdout in this case
*/

// ------------------------------------------------------------------------------------

char * validity_time_to_string ( uint16 validity_time_u );
/*
Function:   validity_time_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a numeric validity time value
    Convert this to a validity time string i.e. readable form

Input Params:

    the validity time (as per DFI)
	It should be one of
	FOK Fill or Kill
	GTC Good Till Cancel
	IOC Immediate or Cancel
	ROD Rest of Day.

Output Params:

    pointer to (static) string with the validity time in string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ------------------------------------------------------------------------------------

char * client_category_to_string ( uint8 client_category_u );
/*
Function:   client_category_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a numeric client category value
    Convert this to a string i.e. readable form

Input Params:

    the client category (as per DFI)
    It should be one of
	Client
	Firm (== Professional)
	FARMM (Far Away Market Market)

Output Params:

    pointer to (static) string with the client categoryin string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ------------------------------------------------------------------------------------

char * bid_ask_to_string ( uint8 bid_ask_u );
/*
Function:   bid_ask_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a numeric bid/ask value
    Convert this to a string i.e. readable form

Input Params:

    
Output Params:

    pointer to (static) string with the validity time in string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ------------------------------------------------------------------------------------

char * order_type_to_string ( uint8 order_type_u );
/*
Function:   order_type_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a numeric order_type value
    Convert this to a string i.e. readable form

Input Params:

    
Output Params:

    pointer to (static) string with the order type in string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ------------------------------------------------------------------------------------

char * strike_price_group_to_string ( uint8 strike_price_group_u );
/*
Function:   strike_price_group_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a numeric strike price group value
    Convert this to a string i.e. readable form

Input Params:

    the validity time (as per DFI)
    It should be one of
	Deep-in
	In
	At
	Out
	Deep-out

Output Params:

    pointer to (static) string with the validity time in string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ---------------------------------------------------------------------------

gen_buf_node_xt * find_bin ( country_market_xt *country_market_px,
				uint8 search_bin_u );
/*
Function:   find_bin
Author:     Neil Cossor
Modified:   9907xx
Description:

    We wish to find the (internal) record about a nominated bin, within
    a nominated country/market.

    Return a pointer to the found record,
    unless we can't find it, in which case return NULL

    As we expect a very small number of bins (i.e. 10),
    just do a linear search, which will check against half of the bins, on average.
*/

// ---------------------------------------------------------------------------

void display_exchanges ( click_details_xt * click_px );
/*
Function:   display_exchanges
Author:     Neil Cossor
Modified:   9907xx
Description:

    Show the info we have about known exchanges.
    This includes equities exchanges,
    as well as derivaties exchanges
*/

// ------------------------------------------------------------------------------------

uint8	cmd_strike_price_group ( char *client_category_s,
				    uint8 default_strike_price_group_u );
/*
Function:   cmd_strike_price_group
Author:     Neil Cossor
Modified:   991102
Description:

    The command line has a Strike Price Group string
    Convert this to a numeric value, as per DFI.

Input Params:

    the command line parameter string.
	It should be one of
	DIn     Deep In
	In	    In
	At	    At
	Out     Out
	DOut    Deep Out
	*	    all of the above - this generally means that you must generate multiple records

    the default value to use, if no selector string is supplied

Output Params:

    the encoded value,
    or UNDEFINED_UINT8 if the user did NOT supply an allowed choice.

    An error msg is displayed on stdout in this case
*/

//-----------------------------------------------------------------------------------

int read_unsigned_limited ( char *input_string_ps,
			    unsigned max_value_u );
/*
Function : read_unsigned_limited
Author	 : Neil Cossor
Modified : 9905xx
Description:

    We have a numeric field to be read from a command line argument.
    The command line argument is a string - but it MAY be NULL (return a 0)
    The target number is an ordinal number (i.e. no fractional part).

    If we get any silly input, then return a 0, as well as spitting out a warning.

    Note that we are also given a max value. If the user supplies a string which
    gives a larger numeric value, return the number, but spit out a warning.

    This will allow QA-style testing, where we know we have a limited
    range that are officially supported, but allow us to test out the boundary
    behaviour (and beyond).
*/

// ------------------------------------------------------------------------------------

char * order_category_to_string ( uint16 order_category_u );
/*
Function:   order_category_to_string
Author:     Neil Cossor
Modified:   20001230
Description:

    We have a numeric order category value
    Convert this to a order category string i.e. readable form
    NB changed from 8-bit to 16-bit in release 16.3

Input Params:

    the order category (as per DFI)
    It should be one of
	Order		plain order
	Quote
	BlockOrder
	BlockResponse
	FacilOrder
	FacilResponse
	CABOrder
	DerivedOrder
	ImprovementQuote (we should NOT see these in ISE system)


Output Params:

    pointer to (static) string with the order category in string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ------------------------------------------------------------------------------------

char * reason_delete_to_string ( uint8 reason_delete_u );
/*
Function:   reason_delete_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a numeric reason_delete value
    Convert this to a string i.e. readable form

Input Params:

    
Output Params:

    pointer to (static) string with the deletion reason in string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ------------------------------------------------------------------------------------

char * lock_type_to_string ( uint8 lock_type_u );
/*
Function:   lock_type_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a numeric lock_type value
    Convert this to a string i.e. readable form

Input Params:

    
Output Params:

    pointer to (static) string with the lock type in string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ---------------------------------------------------------------------------

void prompt_logged_on ( char *command_ps );
/*
Function:   prompt_logged_on
Author:     Neil Cossor
Modified:   9908xx
Description:

    A command was executed which requires being logged on.
    Point out this problem to the caller.

    This is called by most of the commands.

Input Param:

    String, which is the command that was being attempted
*/

// ------------------------------------------------------------------------------------

char * open_close_to_string ( uint8 open_close_u );
/*
Function:   open_close_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a numeric open_close value
    Convert this to a string i.e. readable form

Input Params:

    
Output Params:

    pointer to (static) string with the open/close setting (for OCC settlement) in string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ------------------------------------------------------------------------------------

char * order_status_to_string ( int32 order_status_u );
/*
Function:   order_status_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a numeric order_status value
    Convert this to a string i.e. readable form

Input Params:

    
Output Params:

    pointer to (static) string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ------------------------------------------------------------------------------------

char * away_market_status_to_string ( uint8 away_market_status_u );
/*
Function:   away_market_status_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a numeric away_market_status value
    Convert this to a string i.e. readable form

Input Params:

    
Output Params:

    pointer to (static) string with the corresponding value in string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ------------------------------------------------------------------------------

void lock_q (node_header_xt *header_px );
/*
    locks access to a Q temporarily.

    Whichever thread calls this will lock until it gains exclusive access to the Q.
    A call to unlock_q should be called when the thread is done with this Q
*/

// ------------------------------------------------------------------------------

void unlock_q (node_header_xt *header_px );
/*
    unlocks the exclusive lock on a Q.

    A call to lock_q should have been called before this.
*/

// ------------------------------------------------------------------------------------

char * change_type_to_string ( uint16 change_type_u );
/*
Function:   change_type_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a numeric change_type value
    Convert this to a string i.e. readable form.
    These are in bi202, bi204, bi204 update messages

Input Params:

    
Output Params:

    pointer to (static) string with the change type in string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ------------------------------------------------------------------------------------

char * change_reason_to_string ( uint16 change_type_u );
/*
Function:   change_reason_to_string
Author:     Neil Cossor
Modified:   20010531
Description:

    We have a numeric change_reason value
    Convert this to a string i.e. readable form.
    These are in bo130, bo131, bo132 update messages

Input Params:

    
Output Params:

    pointer to (static) string with the change type in string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ---------------------------------------------------------------------------

void lock_global ( click_details_xt *click_px );
/*
Function:   lock_global
Author:	    Neil Cossor
Modified:   990914
Description:

    We wish to lock the global. We are either doing an update, or we are doing
    read access that requires a consistent common.
*/

// ---------------------------------------------------------------------------

void unlock_global ( click_details_xt *click_px );
/*
Function:   unlock_global
Author:	    Neil Cossor
Modified:   990914
Description:

    We wish to unlock the global. We are have finished doing our update or read.
    The global must have previously been locked with lock_global.
*/

// ------------------------------------------------------------------------------------

char * underlying_type_to_string ( uint8 underlying_type_u );
/*
Function:   underlying_type_to_string
Author:     Neil Cossor
Modified:   990929
Description:

    We have a numeric underlying type value
    Convert this to a underlying type string i.e. readable form

Input Params:

    the underlying type (as per DFI)
    It should be one of
	Stock		ordinary equity
	Currency
	Interest Rate
	Energy
	Soft and Agrics
	Metal
	Stock Index
	..
	Metal Index


Output Params:

    pointer to (static) string with the underlying type in string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ------------------------------------------------------------------------------------

char * trade_type_to_string ( uint8 trade_type_u );
/*
Function:   trade_type_to_string
Author:     Neil Cossor
Modified:   991006
Description:

    We have a numeric trade_type value
    Convert this to a string i.e. readable form.
    These are in BD5, CA1 and CA2 messages

Input Params:

    the 1-byte value in the OM msg

Output Params:

    pointer to (static) string with the trade type in string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ------------------------------------------------------------------------------------

char * trade_state_to_string ( uint8 trade_state_u );
/*
Function:   trade_state_to_string
Author:     Neil Cossor
Modified:   991006
Description:

    We have a numeric trade_state value
    Convert this to a string i.e. readable form.
    These are in BD5, CA1 and CA2 messages

Input Params:

    the 1-byte value in the OM msg

Output Params:

    pointer to (static) string with the trade_state in string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// --------------------------------------------------------------------------

void not_yet_implemented ( char *extra_text_s);
/*
Function : not_yet_implemented
Author	 : Neil Cossor
Modified : 9907xx
Description:

    the operator / script called a command which we have defined, but not
    yet written
*/

// ---------------------------------------------------------------------------

int32 subscr_list_bin ( click_details_xt * click_px,
			ise_underlying_list_xt * ise_subscr_list_px,
			uint8 bin_u );
/*
Function : subscr_list_bin
Author	 : Neil Cossor
Modified : 20000110
Description:

    We are partway through handling a command, involving
    turning on or off broadcasts. As part of that, we are converting
    an argc/argv[] string input list of underlying names into a counted
    array of commodity codes, which we can then pack into the appropriate
    omniapi_<> function.
    
    For the particular command, a "%bin" mnemonic has been supplied as
    part of the qualifying argv[] list.

    We need to add in the appropriate commodity codes for the specified
    bin. Note, the count of underlying / array may already be partially
    populated.. i.e. %bin may NOT have been the first qualifier in the
    argv[] list.

    For this to work, of course, we must be signed in.. so we have
    the underlying->commodity information...

Input Params:

    pointer to everything we know about click (the central system / market)
    pointer to the structure we are populating
    the bin #.. if we are an MM, then this is probably the MM's assigned bin,
	but they may choose to listen to other bcasts...

Output params:

    an error (-ve) status if the bin # is invalid,
	or we would overflow the target structure
    SUCCESSFUL status, and an updated commodity list struct if all OK
*/

// ---------------------------------------------------------------------------

char * market_status_to_string ( uint8 trading_status_u );
/*
Function:   market_status_to_string
Author:     Neil Cossor
Modified:   20000505
Description:

    We have a market status number, which we wish to output.
    Convert from the number into a readable string

Input Params:

    The market trading status #

Output Params:

    Points to the resultant string
*/
// ---------------------------------------------------------------------------

uint32 fract_from_double_with_round (  double  input_d,
					uint32  decimals_u,
					int8    round_i);
/*
Function:   fract_from_double_with_round 
Author:     Scott Collins
Modified:   000122
Description:

    Convert a double to a uint with an implied decimal and round to the nearest fraction

Input Params:

    Value as double
    Number of implied decimals
    Round direction (0 = down, 1 = up)

Output Params:

    Returns to the resultant uint32
*/
// ---------------------------------------------------------------------------

uint32 decimal_from_double_with_round ( double  input_d,
					instrument_class_xt * ise_instr_class_px,
					int8    round_i);
/*
Function:   decimal_from_double_with_round 
Author:	    Neil Cossor
Modified:   20010319
Description:

    Convert a double to a uint with an implied decimal and round to the nearest decimal tick boundary
    This could probably be generalised to cover fractional / decimal..

Input Params:

    Value as double
    Pointer to instr class data, which has tick info data
    Round direction (0 = down, 1 = up)

Output Params:

    Returns to the resultant uint32
*/
// ---------------------------------------------------------------------------

double double_from_whole_decimal (  uint32  input_u,
                                    uint32  decimals_u );
/*
Function:   double_from_whole_decimal
Author:     Scott Collins
Modified:   000110
Description:

    Convert a uint with an implied decimal to a double

Input Params:

    Value as uint
    Number of implied decimals

Output Params:

    Returns the resultant double
*/

// ---------------------------------------------------------------------------

double double_days_from_now_to_date_xt ( date_xt *date_px );
/*
Function:   double_days_from_now_to_date_xt 
Author:     Scott Collins
Modified:   000122
Description:

    Compute the days to the date specified in a date_xt

Input Params:

    date

Output Params:

    Returns a double storing the number of elapsed days
*/

#ifndef WIN32
    /* the OM Solaris libraries (v15.7) have not yet been delivered. Accordingly,
	the swapint64 is provided as a temporary solution by ISE
	for Solaris must still be used, until the new link libraries
	are available.
    */
int64 swapint64 ( uint64 source_u );
/*
    swaps the eight bytes of source_q, and returns them to the caller.

    NB it doesn't really matter whether the source data is signed or not.
*/
#endif


// ------------------------------------------------------------------------------

void fprintf_time (FILE *output_px);
/*
Function:   fprintf_time ()
Author:	    Neil Cossor
Modified    20000530
Description:

    fn which grabs the current date/time, and spits out the result in readable form to 
    the nominated file. Just like printf_time(), which goes to stdout.

    It does NOT add any newlines etc, just a space after the date/time.
    This way, we can get date/time at the start of lines of text logged.
*/

// ---------------------------------------------------------------------------

char * series_wildcard_to_string ( click_details_xt *click_px,
				    ise_series_xt * ise_series_px );
/*
Function:   series_wildcard_to_string
Author:	    Neil Cossor
Modified:   20010228
Description:

    We have a 7-field series structure, which is used for some selector.
    We are trying to report (display) something about this, but the series
    can be a wildcard, which could specify many different combinations
    of fields.

    This can include things like
    - a market :- a specific country and market
    - an instrument type :- a specific country, market and instrument group
    - an instrument class :- a specific country, market, instr group and commodity
    - a series :- all fields qualified
*/

// ------------------------------------------------------------------------------------

char * reason_tick_to_string ( uint8 tick_type_u );
/*
Function:   reason_tick_to_string
Author:     Neil Cossor
Modified:   20010618
Description:

    We have a numeric reason, tick-worse value
    Convert this to a string i.e. readable form.
    These are in bo110 quote update messages

Input Params:
    
Output Params:

    pointer to (static) string with the change type in string form
    If it is an unknown type, then the string will be the code in numeric form
*/

// ------------------------------------------------------------------------------------

uint8 cmd_user_member( char *user_member_input_s );
/*
Function:   cmd_user_member
Author:     Neil Cossor
Modified:   20010620
Description:

    The user has a command which (should have) selected a USER or MEMBER.
    This can be used in a dedicated broadcast subscription, or
    in qualifying the flavour of some queries (such as query order, or query combo)
    From the supplied string, return an appropriate value

Input params:

    The string which supplied on the command line.

Output params:

    The predefined value
    user_member_member_e == default
    user_member_user_e
*/



#endif
