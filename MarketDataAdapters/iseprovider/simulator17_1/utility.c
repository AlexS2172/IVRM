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
-FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR
DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES.  IN NO EVENT WILL ISE OR
ITS LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR
DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES,
HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING
OUT OF THE USE OF OR INABILITY TO USE THE SOFTWARE, EVEN IF ISE HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.


Title : utility.c
Author: Neil Cossor
Function :

    This file provides general utilities to the ISESim.exe program.
    
Revision history :-

yymmdd	vers'n	author	    notes
------	------	------	    -----
9905xx	v0.00	Neil Cossor initial version
990910	v0.01		    fix NT/Solaris access to Q locks, add routines allowing locks
			    by external routines
990913			    added change_type_to_string
990914	v0.02		    added lock/unlock_global routines
			    updated help info for AUTOCBD and QUIT cmds
			    alter free_gen_buf_node_q to make free() of child data optional
990915			    mod for extra param to free_gen_buf_node_q
990917			    moved sim_pause into command.c, preparatory to additional timer control
990920			    update help info for SET,INTERVAL,<msecs> new command
			    and WAIT,BOUNDARY and WAIT,UNTIL new commands
990929			    added underlying_type_to_string fn
			    update display_help_screen
991005	v0.03		    update display_help_screen for logger,header commands
991006			    added trade_type_to_string, trade_state_to_string
991102	v0.04		    enhance cmd_<> routines to provide 'default' value (where sensible)
991116	v0.05		    update display_help_screen (add logger,auto and resize for 25-line screen)
991130			    brought in not_yet_implemented () from command.c
991202			    fix get_user_information to save facility using PUTLONG (Solaris problem)
991204			    fix bug in not_yet_implemented.. referenced string param incorrectly
000104			    update for latest OM .h files (15.7 OM release)
			    remove our own swapint64 routine (OM now supply one)
000105			    fix endian bug in pack/unpack_click_bit_date
000110			    import subscr_list_bin from broadcast.c
000114			    add new routine, flush_gen_buf_node_q
000117			    mod who_am_i to report the username, as well as the user-code details
			    update display_help_screen for latest changes
000122			    mod to be source-portable between NT and Solaris.. thanks Joshua P.
			    mod get_user_information to check for valid OM system (one of ISE's).
			    import several to/from double routines from autoqute.c
			    mod add_to_q_tail to maintain a max Q-length field in Q header
000201	v0.06		    fix bug in trade_state_to_string (incorrect index for unknown state)
000204			    fix couple of sizeof errors in get_user_information
			    move include of process.h into stdincl, only if WinNT
			    mod read_kbd_input to always call _strupr
			    mod save_user_param_file, load_previous_user address refs to default filename
000213			    add defn (if not WIN32) for _strupr (moved from command.c)
			    correct display_help_screen, to include LOCKED commands
000309			    fix %% display bugs in display_help_screen
			    fix bugs in read_decimal_string_int (reading 0.0625 as 0.625)
000328	v0.07		    mod cmd_validity_time to return 0 if invalid string selector
000330			    mod display_help_screen. Add "quote,delete". Remove Get,A/I,U,<undername>.
			    Remove quote,query,U,<undername>
000331			    fix bug in rx_click_string (did not check correctly for NUL)
000425	v0.08		    mod display_help_screen to include new LIST commands
000505	v0.09		    add market_status_to_string () to turn market status into readable text
000515			    update bid_ask_to_string and cmd_bid_ask to reflect BOTH in string choices
000530			    added fprintf_time(), for use in logging trades (ISE and away)
000601			    mod underlying_type_to_string and away_market_status_to_string, to just say
			    "unknown" for undefined values
000605			    mod fprintf_time() to put "," between date, time.. easier for import into excel, DB etc
000606			    fix bug in flush_gen_buf_node_q.. did not zot header ptrs, or length correctly
0012xx	v2.00		    start updates for OM release 16.3 (from 15.7-3)
			    16-bit order category
0101xx			    continuing
0102xx			    new routine to convert series, with potential wildcards, into a descriptive string
010319			    rename uint_from_double_with_round to fract_from_double_with_round,
			    add new routine to decimal_from_double_with_round
010411			    updated for changed 16.5 client_categories, and added some lock types (which
				have been around for a while).
			    corrected display_help_screen (STATE, SYNTHETIC, VOLUMES commands)
010526	V2.01		    updated for new QUOTE,PANIC command
			    update cmd_validity_time and validity_time_to_string for new 16.5 flavours
				NB temp hacks around the "timed" FAS/AON flavours.. more thinking
010531			    added change_reason_to_string
010601			    removed trailing NUL from output of save_user_param_file.. made file hard to edit
010604			    update order_category_to_string for 2 new combo order categories
010607			    correct format_whole_decimal_s for -ve premiums (needed for combos)
			    allow read_decimal_string_int to take -ve input
010620			    added cmd_user_member fn
010706			    mod who_am_i to include data retrieved from DQ30 (message authorised etc)
010711			    update display_help_screen for speed,set + speed,get
			    + combo commands + partial 

*/
#include "stdincl.h"

#include "error.h"
#include "instrument.h"
#include "misc.h"
#include "series.h"
#include "underlying.h"
#include "utility.h"
#include "logger.h"
#include "math.h"



// ---------------- module private statics, globals etc ---------------------


// string choices for client categories
static char *client_category_s [] =
{
    "MM",
    "CUSTOMER",
    "FIRM",
    "FARMM",
    "DISCRETIONARY",
    "BROKERDEALER",
// LINKAGE, at some point post 16.5
    "Unknown"
};
enum client_cat_et { client_cat_mm_e, 
			client_cat_cust_e, 
			client_cat_firm_e, 
			client_cat_farmm_e,
			client_cat_disc_e,
			client_cat_bd_e 
		    };

    
    
// ------------------ fns we are supplying to the outside world -------------

#ifndef WIN32
    void _strupr (char *p)
    /*
    Function	_strupr
    Author		Joshua P
    Modified	20000104
    Description:

	In WinNT, _strupr is provided, which converts a whole string
	to upper case.

	On Solaris, no _strupr routine, provide one
    */
    {
	while (*p)
	{
	    *p = toupper (*p);
	    ++p;
	}
    }	// _strupr
#endif


//-----------------------------------------------------------------------------------

void display_help_screen ( void )
/*
Function : display_help_screen
Author	 : Neil Cossor
Modified : 200104xx
Description:

    Displays a list of implemented commands onto standard output
*/
{
/*
#ifdef _WINDOWS_
    WinHelp (GetFocus(), "ISEsimhelp.HLP", HELP_KEY, (DWORD)(LPSTR)"Contents");
#else
*/
    int8  ReplyBuffer_s[5];

    printf (
	    "\nThe following commands and symbols have been implemented:                      \n"
	      "AMend       Amend an order                                                     \n"
	      "            AMEND,Bid|Ask,<seriesname>,<orderID>,[<new qty>][,<new xref>       \n"
	      "               [,<new valid.time>[,<new ign.away>[,<new bkr ID>                \n"
	      "               [,<new open/close>]]]]]                                         \n\n"
	      "ASk         Enter an Ask order. Supply all of the details in one line          \n"
	      "            ASK,<seriesname>,<qty>,<premium>,<Xref>[,<validity>                \n"
	      "                 [,<client category>[,<Type>[,<ignore_away>                    \n"
	      "                 [,<broker id>[,<open/close>[,<CMTA ID>[,<sub a/c>]]]]]]]      \n"
	      "               eg ASK,ABS9SEP35.0C,45,560,TEST1,GTC,FIRM,LIMIT,1,TE31,3        \n\n"
	      "AUTOCdb     Turn automatic CDB load (default) ON or OFF.      ISE QA only.     \n\n"
	      "AUTOQuote   handling of autoquoting (for ISE in-house testing only)            \n"
	      "              except for show, disable, enable..                               \n"
	      "	             the rest are setting params which are by underlying, and take a  \n"
	      "                <underlying list> == *|%%bin|%%folio|<under>[,...]               \n"
              "            AUTOQuote,DISABLE    kill all autoquoting (flag.. params unchanged)\n"
	      "            AUTOQuote,ENABLE     enable autoquoting (doesn't check params)     \n"
	      "            AUTOQuote,INTEREST,<int%%>,<underlying list>		              \n"
	      "              sets interest rates for one or more underlying                   \n"
	      "              int% can contain 2 decimal places                                \n"
	      );
 
    printf ("Press ENTER to continue");
    read_kbd_input(&ReplyBuffer_s[0] );

    printf (
	      "            AUTOQuote,OFF,<underlying list>  turns autoquoting off (by under)  \n"
	      "            AUTOQuote,ON,<underlying list>   turns autoquoting on (by under)   \n"
	      "            AUTOQuote,OPEN_QUANTITY,<open_qty>,<underlying list>               \n"
	      "              sets the quantity. used in opening rotations                     \n"
	      "            AUTOQuote,QUANTITY,<quantity>,<underlying list>                    \n"
	      "              sets the quantity, used in normal trading                        \n"
	      "            AUTOQuote,SHOW               shows autoquoting settings for the bin\n"
	      "            AUTOQuote,STEPUP,<underlying list>  set the step-up buffer quantity\n"
	      "            AUTOQuote,VOLATILITY,<vol%%>,<underlying list>                      \n"
	      "              sets volatility for one or more underlying (2 decimal places)    \n\n"
	      "BCast       Handling of dynamic broadcast subscription                         \n"
	      "            BCast,ALL,ON|OFF    en(dis)ables all bcasts, or kills all bcasts.. \n"
	      "                      if offline, prevents / enables subscribe-all at logon    \n"
	      "                      if online, then cancels / enables immediately            \n"
	      "            BCast,MM,ON|OFF    (MM only) toggles bcasts to be MM's bin only    \n"
	      "            BCast,POLL,<delay> set delay between bcast poll, in milliseconds   \n"
	      "            BCast,SHOW     displays current bcast subscriptions                \n"
	      "            BCast,<bcastID>,ON|OFF,<*|%%bin|<under>[,...]|User|Member           \n"
	      "                      enables / disables individual bcasts                     \n"
	      "                      NB * and anything else illegal, can't repeat %%bin,       \n"
	      "                          only MM can use %%bin                                 \n\n"
	      "                      by User|Member only allowed for Directed broadcasts      \n"
	      "BID         You can enter a bid order the same ways as an Ask order.           \n\n"
	      "BIN         Show which underlying, and how many series in each, in a bin       \n"
	      "            BIN,<bin #>                                                        \n\n"
	      );
 
    printf ("Press ENTER to continue");
    read_kbd_input(&ReplyBuffer_s[0] );

    printf (
	      "BLock       BLock order commands - send an order, or send response             \n"
	      "            BLock,Bid|Ask,<seriesname>,<qty>,<premium>,<Xref>[,<validity>      \n"
	      "              [,<client category>[,<expose flag>[,<bkr id>[,<open/close>       \n"
	      "              [,<CMTA ID>[,<sub a/c>]]]]]]]]                                   \n"
	      "            BLock,RESPONSE,Bid|Ask,<seriesname>,<qty>,<premium>,<Xref>         \n"
	      "                 [,<OrderID>[,<bkr id>[,<open/close>[,<CMTA ID>[,<sub a/c>]]]] \n\n"
	      "CAb         Enter a CAB order                                                  \n"
	      "            CAB,BID/ASK,<seriesname>,<qty>,<Xref>[,<validity>                  \n"
	      "                 [,<client category>[,<bkr id>[,<open/close>                   \n"
	      "                 [,<CMTA ID>[,<sub a/c>]]]]]                                   \n\n"
	      "CLear       This command clears any script out of memory leaving a clean       \n"
	      "            slate for the next sim file to be loaded.                          \n\n"
	      );
 
    printf ("Press ENTER to continue");
    read_kbd_input(&ReplyBuffer_s[0] );

    printf (
	      "COmbo       various combo commands, including                                  \n"
	      "         COMBO,AMEND,<details for overal amend>    amend combo in book         \n"
	      "         COMBO,ASK,<details for one leg>   add ask combo leg (refer combo,send)\n"
	      "         COMBO,BID,<details for one leg>   add bid combo leg                   \n"
	      "         COMBO,BOOK         show combobook..includes public and member combos  \n"
	      "         COMBO,CLEAR			  clear out legs built up for combo   \n"
	      "         COMBO,DELETE,<local #>|<underyling>|*	  delete combos               \n"
	      "         COMBO,HIT,<details for overall hit>       hit combo in book           \n"
	      "         COMBO,LOAD			  load local combobook from system    \n"
	      "		COMBO,SAVE			  take snapshot of last combo.        \n"
	      "                                           Can reference data from this with %%O\n"
	      "                                           Used in COMBO AMEND|DELETE|HIT      \n"
	      "         COMBO,SEND,<details for overall combo>    send combo incl. legs       \n"
	      "         COMBO,VIEW			  show in-memory combo legs           \n\n"
	      "Delete      Delete orders :-                                                   \n"
	      "             Delete,*                ALL local orders for ALL underlying       \n"
	      "             Delete,E[xact],<series>,<orderID>,<bid/ask>  delete one order     \n"
	      "             Delete,L[ocal],#        ONE order, using local order number.      \n"
	      "             Delete,R[ef],Brian      Local order with a reference              \n"
	      "             Delete,S[eries],<name>  ALL local orders for one series           \n"
	      "             Delete,U[nder],<name>   ALL local orders for selected underlying  \n"
	      "             Delete,%%O               delete last saved order (see SAVE command)\n\n"
	      );
 
    printf ("Press ENTER to continue");
    read_kbd_input(&ReplyBuffer_s[0] );

    printf (
	      "EXChanges   Displays information about exchanges (underlying and derivatives)  \n\n"
	      "EXIt        Logs Out of the system and exit the program.                       \n\n"
	      "Facilitation Facilitation order commands - send an order, or send response     \n"
	      "            Facilitation,Bid|Ask,<seriesname>,<qty>,<premium>,<Xref>           \n"
	      "                [,<client category>[,<bkr id>[,<open/close>                    \n"
	      "                [,<CMTA ID>[,<sub a/c>[,<broker %%>[,<expose flag>]]]]]]        \n"
	      "            Facilitation,Response,Bid|Ask,<seriesname>,<qty>,<premium>,<Xref>  \n"
	      "                 [,<OrderID>[,<bkr id>[,<open/close>[,<CMTA ID>[,<sub a/c>]]]] \n\n"
	      "Get         Gets orders in the central orderbook into the local orderbook.     \n"
	      "             Get,<Active|Inactive>,*                   orders in all series    \n"
	      "             Get,<Active|Inactive>,S,<series name>     orders in one series    \n\n"
	      "Help        Displays this help information                                     \n\n"
	      "Instruments Displays information about instrument groups                       \n\n"
	      "LIst        LIst,<UNDERLYING|SERIES>                                           \n"
	      "              produces list files of all underlying / series                   \n\n"
	      "LOad        Load a script file into memory. May specify the filename as param  \n\n"
	      "LOCked      LOCked,<UNDERLYING|SERIES>,<undername>|<seriesname>                \n"
	      "              queries and displays locked orders in one underlying / series    \n\n"
	      );

    printf ("Press ENTER to continue");
    read_kbd_input(&ReplyBuffer_s[0] );

    printf (
	      "LOGGer      Controls logging. Choices are                                      \n"
	      "                LOGGer,Auto,ON|OFf              en(dis)able auto logging       \n"
	      "                LOGGer,ON|OFf                   start/stop logging             \n"
	      "                LOGGer,Bcasts,ON|OFf            do/don't include broadcasts    \n"
	      "                LOGGer,File,<filename>          switch log output to <filename>\n"
	      "                LOGGer,Header,ON,OFf            do/don't include headers       \n"
	      "                LOGGer,OrderIDs,ON|OFf          do/don't include order/transIDs\n"
	      "                LOGGer,Timestamps,ON,OFf        do/don't include timestamps    \n\n"
	      "LOGOFf      Logoff (leaves orders in central orderbook)                        \n\n"
	      "LOGON       Log on to the system. Choices are :-                               \n"
	      "            LOGON        uses User Param file last used, or supplied at startup\n"
	      "            LOGON,<user filename>               uses nominated user param file \n"
	      "            LOGON,<username>,<password>,<gatewayname>,<port>,<save filename>   \n\n"
	      "MM          MM,Get,<underlying>[,<instrument grp #>]          query MM params  \n\n"
	      "            MM,Set,<underlying>,<instr grp>,<strike grp>,<exp grp>,<stepup>,   \n"
	      "              <ISEMM abs>,<ISEMM %%>,<firm abs>,<firm %%>,<FARMM abs>,<FARMM %%>,\n"
	      "              <TickWrs1>,..<TickWrs8>                                          \n"
	      "            NB: <instr group>,<strike group>,<exp group> support '*' wildcards \n\n"
	      );

    printf ("Press ENTER to continue");
    read_kbd_input(&ReplyBuffer_s[0] );

    printf (
	      "New         (debug) calls whatever the newest command implemented in ISESim.   \n\n"
	      "OPEn        OPEn,Query,Series|Underlying,<series name>|<underlying name>,      \n"
	      "              <ignore away market>                                             \n"
	      "            OPEn,Request,S[eries]|U[nderlying]|*,<series name>|<under name>,   \n"
	      "              <ignore away market>[,<series status>,<fast mkt lvl (if fast)>]  \n\n"
	      "OPRA        query OPRA prices. Can get each exchange, or amalgamated best      \n"
	      "            OPRa,BEST,S[eries]|U[nderlying],<series name>|<underlying name>    \n"
	      "                [,<instr group>]                                               \n"
	      "            OPRA,EACH,S[eries]|U[nderlying],<series name>|<underlying name>    \n"
	      "                [,<instr group>]                                               \n\n"
	      "ORDERS      Display the local users orders, from GET and newly entered orders. \n\n"
	      "ORDERBook   Query/display complete orderbook.    PMM only                      \n"
	      "            ORDERBook,Series,<series name>                                     \n"
	      "            ORDERBook,Underlying,<underlying name>                             \n\n"
	      "PAuse       Allows you to pause for a number of seconds. eg pause 1.5          \n\n"
	      );

    printf ("Press ENTER to continue");
    read_kbd_input(&ReplyBuffer_s[0] );

    printf (
	      "PMm         PMM,Get,<underlying>[,<instrument group #>    query PMM params     \n\n"
	      "            PMM,Set,<underlying>,<instr grp>,<strike grp>,<exp grp>,<stepup>,  \n"
	      "              <ISEMM abs>,<ISEMM %%>,<firm abs>,<firm %%>,<FARMM abs>,<FARMM %%>,\n"
	      "              <TickWrs1>,..<TickWrs8>,<Derive1>,..<Derive8>,                    \n"
	      "              <MatchAway1>,..<MatchAway8>                                       \n"
	      "            NB: <instr group>,<strike group>,<exp group> support '*' wildcards \n\n"
	      "PRIces      Send Underlying Equity Price Query                                 \n\n"
	      "PROmpt      PROmpt[,<prompt string>]                         Script only       \n"
	      "            Displays a manual prompt, and pauses a script until RETURN is hit  \n\n"
	      "QUIt        immedately does a program abort.                 ISE QA only.      \n\n"
	      "QUOte       Quote commands - includes add component, clear, send and view set  \n"
	      "            Quote,Bid|Ask,<seriesname>,<qty>,<premium>,<open qty>,<step up>    \n"
	      "            Quote,Clear         clear the local (in-memory) block quote        \n"
	      "            Quote,Delete,Bid|Ask,<series> delete one quote component           \n"
	      "            Quote,Panic         remove all quotes for all users, whole bin     \n"
	      "            Quote,Query,<*|S>[,<series name>]  query MP server(s)              \n"
	      "            Quote,Send          end local block quote to MP & clear from memory\n"
	      "            Quote,View          show the local (in-memory) block quote         \n\n"
	      );

    printf ("Press ENTER to continue");
    read_kbd_input(&ReplyBuffer_s[0] );

    printf (
	      "RElease     Release a locked order :-                                          \n"
	      "            RElease,Bid|Ask,<seriesname>,<orderID>,<lock type>                 \n\n"
	      "RUn         Run the script already loaded into memory. If you add a number     \n"
	      "            to the command eg 'Run 5', then the script will be run 5 times.    \n\n"
	      "SAve        SAve the last order (any type), for reference with %%O              \n"
	      "              Note: this saved order data is kept across logoff/logon          \n\n"
	      "SERies      Display either all of the active series, or the active series for  \n" 
	      "            a selected underlying.                                             \n\n"
	      "SET         Sets a default environment value, until overriden by another set   \n"
	      "            Currently supports                                                 \n"
	      "                SET,Interval,<value>       set delay (millisecs) between cmds  \n"
	      "                SET,Member                 set bcast/qeury to be by member     \n"
	      "                SET,Series,<value>         set default Series, say abs9sep35.0c\n"
	      "                SET,UNderlying,<value>     set default Underlying, say INTC    \n"
	      "                SET,USer                   set bcast/query to be by user       \n\n"
	      "SPeed       Sets or gets CMM/PMM speedbump parameters                          \n"
	      "                SPEED,GET,<underlying>|*                                       \n"
	      "                SPEED,SET,<underlying|%%BIN,<# contracts>,<widen %%>           \n"
	      "STate       Displays the Session States, and their attributes                  \n\n"
	      "SYnthetic   Displays know info about synthetic underyling (ratios, cash etc)   \n\n"
	      );

    printf ("Press ENTER to continue");
    read_kbd_input(&ReplyBuffer_s[0] );

    printf (
	      "Trade       Collects (and displays) information about Trades/Deals             \n"
	      "            Trade,Today[,<instr group>[,<start seq #>[,<end seq #>]]]          \n"
	      "            Trade,PARtrectify,<series>,<trade #>,                              \n"
	      "             <qty 1>,<CMTA # 1>,<sub a/c1>,<a/c type 1>,<open/close 1>,<Xref 1>,\n"
	      "             [<qty 2>,<CMTA # 2>...]                                           \n"
	      "            Trade,PASt,<start yyyymmdd>,<end yyyymmdd>                         \n"
	      "                [,<start hhmmss>[,<end hhmmss>[,<instr group>]]]               \n"
	      "            Trade,Rectify,<series name>,<trade #>,<CMTA #>,<sub a/c ID>,       \n"
	      "                <a/c type>,<open/close>,<Xref>                                 \n"
	      "            Trade,REPort,Bid|Ask,<seriesname>,<orderID>,<quantity>,            \n"
	      "                <premium>,<order category>                                     \n\n"
	      "Underlying  Underlying[,<underlying string>[,BRIEF|DETAIL]]                    \n"
	      "            Display underling in the system. If an <underlying string> is      \n"
	      "            supplied, only names starting with that are shown.                 \n"
	      "            In alphabetic stock-code order                                     \n\n"
	      "VErsion     Displays the current version of ISEsim                             \n\n"
	      "VIew        Allows you to display the script that is in memory.                \n\n"
	      "VOlumes	   Queries and display ISE BBO/statii on series                       \n"
	      "                VOLUMES,<underlying name>\n                                    \n"
	      "                VOLUMES,*[,<orderbook #>[,<bin #>]]                            \n\n"
	      "WAit        WAit,Boundary,<minute boundary>   wait until N minute past the hour\n"
	      "            WAit,Until,<hh[:mm[:ss]]>[,ROLL]  wait until absolute time         \n\n"
	      );

    printf ("Press ENTER to continue");
    read_kbd_input(&ReplyBuffer_s[0] );

    printf (
	      "WHoami      Shows who you are signed on as.                                    \n\n"
	      "~ or !      Comments: ~ displays the comment on screen and in any log file,    \n"
	      "                        ! doesn't display the comment anywhere.                \n"
	      "%%U          Substitutes for the default Underlying        (see SET command)    \n"
	      "%%S          Substitutes for the default Series            (see SET command)    \n"
	      "%%O          Substitutes for the last 'Saved' Order        (see SAVE command)   \n\n"
	      "_____________________________________________________________________________  \n\n"
	      ); 

/*
#endif
*/
}   // display_help_screen

//-----------------------------------------------------------------------------------

int select_string_option ( int choices_u,
			    char *choices_s[],
			    char *input_s )
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
{
    // the value we will return to the caller
    int choice_i;
    // loop var, to flip thru the choice list
    int loop_i;

    // assume we will NOT match
    choice_i = choices_u;

    if (input_s == NULL)
	return choice_i;

    for ( loop_i = 0;
	    loop_i < choices_u;
	    loop_i++ )
    {
	// see if our input string is the start of this possible choice
	if ( strstr ( choices_s [loop_i ], input_s ) == choices_s [loop_i] )
	{
	    // match - have we previously found a match
	    if ( choice_i == choices_u )
	    {
		// never matched before, note this match
		choice_i = loop_i;
	    }
	    else
	    {
		// previous match, ambiguous.. note and bail out
		choice_i = choices_u + 1;
		break;
	    }	// test if we have previously matched
	}   // test for match against one possible match
    }	// for loop, comparing strings

    // return value to caller
    return choice_i;
}   // select_string_option

// ---------------------------------------------------------------------------

int read_decimal_string_int ( char *input_string_ps,
				uint32 decimals_u )
/*
Function: read_decimal_string_int
Author:   Neil Cossor
Modified: 20000309
Description:

    From the given numeric string, and the given implied decimal places, figure out
    how this number should be represented in the target (integer) longword.

    As we know the target is 4 bytes, then it can not have more than
    10 significant digits (max unsigned is ~4 billion). As a double-precision
    floating point number has a 15-16 significant digits, this can "safely"
    store the interim value..

    If the input string has a ".", then there must be at least one numeric digit
    after the decimal point.

    If there are more digits on the RHS of the decimal point than are allowed
    (in the decimal places), then this is an error.

    If the total # of digits in the LHS and RHS combined are greater than
    what can fit into an int i.e. > 9, then an error.

    If there is a + or - char, it must be the first char.

    For example,

    for an input of "23", digits 4 -> 230000
    for an input of "65.1", digits 4 -> 651000
    for an input of "40.12345", digits 4 -> error, truncated to 40.1234

Input Params:

    pointer to the string buffer to read from
    the number of implied decimals.. 0..9

Output Params:

    the (packed) resulting #...
*/
{
    int     whole_i;	    // the result #, after multiplying up
    // temp, output strings, used in figuring how many digits we have
//  char    float_s[20];
    // temp interim double-precision numbers
    double  start_f,
	    end_f;

    // make sure we initialise the target numbers
    whole_i = 0;

    // convert to floating point
    start_f = atof ( input_string_ps );

    // multiply by the appropriate factor
    if ( start_f < 0.0 )
    {
	// we know number is < 0.. so ceiling
	whole_i = (int) ceil ( start_f * (double) dividers_i [ decimals_u ]);
    }
    else
	// we know number is >= 0.. so floor
	whole_i = (int) floor ( start_f * (double) dividers_i [ decimals_u ]);

    // how many digits in result ?
//  sprintf ( (char *)&float_s, "%d", whole_i );

    // does our result match what we were given.. or has truncation / rounding occurred ?
    end_f = (double) whole_i / (double) dividers_i [ decimals_u ];

    if (start_f != end_f)
    {
	printf ("Warning - read_decimal_string_int, src %g != dest %g. Too many digits or rounding\n",
		    start_f,
		    end_f );
    }

    return whole_i;
}   // read_decimal_string_int

//-----------------------------------------------------------------------------------

void format_whole_decimal_s ( char *formatted_string_ps,
				int32 input_u,
				uint32 decimals_u,
				uint32 max_width_u )
/*
Function: format_whole_decimal_s
Author:   Neil Cossor
Modified: 20010607
Description:

    From the given signed 32 bit number (e.g. a strike price, an underlying
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
{
    /* the divide #, for each 10^n level, n=0..9
	10^10 is bigger than you can fit into a strike price
	We use this constant array of dividers, as the only C fn which appears
	to be able to raise one number to a power is a double-precision
	floating point operation, which are horribly inefficient, when we know
	we are only dealing with powers of 10, over a limited range.
    */
    int     whole_i,	    // the (left-hand) whole part, after dividing down
	    decimals_i;     // the (right-hand) whole part, after dividing down

    // divide down, with TRUNCATE fn, NOT rounding
    whole_i = input_u / dividers_i [ decimals_u ];
    decimals_i = abs (input_u) % dividers_i [ decimals_u ];
    sprintf ( formatted_string_ps,
		"%*d.%0*d",		    // rh justified whole, point, zero-filled decimals
		(max_width_u - decimals_u - 1),     // max width, minus space for '.' and rhs
		whole_i,		    // actual value for whole
		decimals_u, // width after dec point
		decimals_i		    // figures after dec point
	    );
}   // format_whole_decimal_s

//-----------------------------------------------------------------------------------

int read_unsigned_limited ( char *input_string_ps,
			    unsigned max_value_u )
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

    If the input string is a '*', then return a 0
*/
{
    uint32 result_u;
    double result_f;

    if ( input_string_ps == NULL )
	return 0;

    if ( input_string_ps[0] == '*' )
	return 0;

    result_f = atof ( input_string_ps );
    result_u = atoi ( input_string_ps );

    if ( result_f != (double) result_u )
    {
	printf ("Input number %s contains a fractional part - 0 used\n",
		    input_string_ps );
	return 0;
    }

    // no fractional part - are we greater than the allowed max value
    if ( result_u > max_value_u )
	printf ("Value (%d) greater than allowed max (%d)\n",
		    result_u,
		    max_value_u );

    return result_u;
}   // read_unsigned_limited

//-----------------------------------------------------------------------------------

void get_ise_user_code_xt ( ise_user_code_xt * ise_user_code_px,
			    user_code_t * click_user_code_px )
/*
    Read (from a CLICK msg) the fields of a user_code_t.
    NB user_code_t == trading_code_t
*/
{
    // grab the fields. Three string fields
    rx_click_string ( &ise_user_code_px->country_id_s,
	    &click_user_code_px->country_id_s,
	    sizeof (click_user_code_px->country_id_s));
    rx_click_string ( &ise_user_code_px->customer_id_s,
	    &click_user_code_px->ex_customer_s,
	    sizeof (click_user_code_px->ex_customer_s));
    rx_click_string ( &ise_user_code_px->user_id_s,
	    &click_user_code_px->user_id_s,
	    sizeof (click_user_code_px->user_id_s));
}   // get_ise_user_code_xt

//-----------------------------------------------------------------------------------

void put_ise_user_code_xt ( user_code_t * click_user_code_px,
			    ise_user_code_xt * ise_user_code_px )
/*
    Write (to a CLICK msg) the fields of a user_code_t.
    NB user_code_t == trading_code_t
*/
{
    // grab the fields. Three string fields
    tx_click_string ( &click_user_code_px->country_id_s,
	    &ise_user_code_px->country_id_s,
	    sizeof (click_user_code_px->country_id_s));
    tx_click_string ( &click_user_code_px->ex_customer_s,
	    &ise_user_code_px->customer_id_s,
	    sizeof (click_user_code_px->ex_customer_s));
    tx_click_string ( &click_user_code_px->user_id_s,
	    &ise_user_code_px->user_id_s,
	    sizeof (click_user_code_px->user_id_s));
}   // put_ise_user_code_xt

// ----------------------------------------------------------------------------

int32 get_user_information ( click_details_xt * click_px)
/*
Function : get_user_information
Author	 : Neil Cossor 
Modified : 20000204
Description:

    Downloads the user code (from which you can
    determine the country in which you are working)
    and the first business facility code

Implicit params:

    Several fields in the common CLICK structure are updated
    presuming, of course, that we have success getting the data from
    the OM system
*/
{
    int32 APIStatus_i;
    int32 TxnStatus_i;
    int32 FatalError_i = FALSE;
    uint32 BufferLength_u;
	
    // where we put the user code - before we reformat into normal C-strings
    user_code_t tmp_user_code_x;
    uint32  tmp_facility_u;	// copy of the facility code in central-site endian

    // grab exclusive access to OMNIAPI - i.e. block out any other threads
    ISE_MUTEX_LOCK ( click_px->omni_io_active_mutex );

    BufferLength_u = sizeof ( click_px->user_code_x );
    APIStatus_i = omniapi_get_info (&TxnStatus_i,
				      OMNI_INFTYP_USERCODE,
				      &BufferLength_u,
				      &tmp_user_code_x );

    // if we have just logged on, then will get "not ready" status
    // our caller should sleep for a bit, and call this routine again
    if ( APIStatus_i == OMNIAPI_NOT_READY )
    {
	// do nothing - just return this status to the caller
    }
    else if ( APIStatus_i < SUCCESSFUL )
    {
	// report the error

	report_click_error ( click_px,
				APIStatus_i,
				TxnStatus_i);
    }
    // normal good status for getting used code
    else if ( BufferLength_u != sizeof ( tmp_user_code_x ) )
    {
	printf ( "\n?strange error code length: %lu\n", BufferLength_u );
    }
    else
    {
	// got user info OK - extract the components into more normal form
	get_ise_user_code_xt ( &click_px->user_code_x,
				&tmp_user_code_x );

	printf ( "\tYou are user '%s', trading for '%s' in '%s'\n\n",
		    &click_px->user_code_x.user_id_s,
		    &click_px->user_code_x.customer_id_s,
		    &click_px->user_code_x.country_id_s);

	BufferLength_u = sizeof ( tmp_facility_u );
	if ( (APIStatus_i = omniapi_get_info (&TxnStatus_i,
					      OMNI_INFTYP_FACTYP_E0,
					      &BufferLength_u,
					      &tmp_facility_u ))
					      < SUCCESSFUL)
	{
	    // error getting facility nunber - EP0 or ET0
	    // report the error
	    report_click_error ( click_px,
				    APIStatus_i,
				    TxnStatus_i);
	}
	// got data OK - is the response a good size ?
	// response size should exactly match size of supplied buffer
	else if ( BufferLength_u != sizeof ( tmp_facility_u ) )
	{
		printf ( "\nRxed strange facility number size: %lu\n", 
			    BufferLength_u );
	}
	else
	{
	    // All OK - now have user_code info and EP0/ET0 base facility #

	    // rearrange facility code into local-endian order
	    PUTLONG ( click_px->facility_no_u, tmp_facility_u );
	    // and report it
	    printf ( " The first business Facility Number is %u\n\n",
			click_px->facility_no_u );

	    /* test if this is one of the two acceptable values.. i.e. one of the ISE's OM systems,
		rather than (say) OM Sweden, or OM Austria, or OM Australia, which we aren't programmed for.
	    */
	    switch ( click_px->facility_no_u )
	    {
	    case OMNI_FACTYP_ISE_XUSEP0:
		printf ("Connected to the OM/ISE Production system\n");
		break;
	    case OMNI_FACTYP_ISE_XUSET0:
		printf ("Connected to an OM/ISE Test system\n");
		break;
	    default:
		return COULD_NOT_LOG_ON_USER;
	    }	// switch on which OM system we are connected to
	}   // test for status return for getting facility code
    }	// test for status return from getting user code

    // release the synchronisation lock so other threads can do OMNIAPI IOs
    ISE_MUTEX_UNLOCK ( click_px->omni_io_active_mutex );

    return APIStatus_i;
}   // get_user_information

// ----------------------------------------------------------------------------

char *read_kbd_input ( char *Buffer_ps )
/*
Function : read_kbd_input
Author	 : Neil Cossor 
Modified : 20000204
Description:

    Accepts a line from the terminal. If CTRL-Z is entered,
    NULL is returned.

    NB converts data to uppercase.
    NB does NOT have any check on the size of the input vs size of the
    target buffer
*/
{
    /*	code:	 */
    Buffer_ps = gets ( Buffer_ps );

    {
	if (Buffer_ps == NULL)
	{
	    /* do nothing */
	}
	else
	{
	    // convert string to upper case
	    _strupr (Buffer_ps);
	}
    }

    return (Buffer_ps);

}   // read_kbd_input

// ----------------------------------------------------------------------------

/*
Function : save_user_param_file
Author	 : Neil Cossor 
Modified : 20010604
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
			    char * user_filename_ps )
{
    FILE    *UserFile_px;	    // local file handle
    char    *local_filename_ps;     // pointer to default or supplied filename


    // did the caller supply a potentially reasonable filename ?
    if ((!user_filename_ps)
	|| (strlen (user_filename_ps) == 0))
    {
	// not supplied with a filename, use default
	local_filename_ps = USER_DATA_FILENAME;
    }
    else
	// user supplied log_filename_ps, use it
	local_filename_ps = user_filename_ps;

    
    if ( (UserFile_px = fopen (local_filename_ps, 
			       FILE_WRITE_ACCESS))
			       != NULL)
    {
	printf ("Saving User params to file %s\n",
		    user_filename_ps );

	// write to the user file
	fprintf (UserFile_px, "%s/%s/%s/%s", 
		 &click_px->user_name_s[0],
		 &click_px->gateway_name_s[0],
		 &click_px->gw_socket_name_s[0],
		 &click_px->password_s[0]
		 );
  
	fclose ( UserFile_px );
    }
    else
    {
	printf ("Could not open user data file %s for save operation\n",
		    user_filename_ps );
    }
}   // save_user_param_file

// ------------------------------------------------------------------------------------

void pack_OM_header ( generic_txn_xt	*tx_msg_px,	    // pointer to a generic transaction message
			uint32		tx_length_u,	    // length of message (not including OMnet heading)
							    // e.g. sizeof (omni_login_message)
			uint8		central_module_u,   // transaction type central module field	e.g. B
			uint8		server_type_u,	    // transaction type server type field	e.g. 0
			uint16		tranaction_no_u )   // transaction type transaction number field    e.g. 101
/*
    Creates and fills in a framework OM transaction header.
    This header includes the transaction length and transaction type.  
*/
{
    /* set length as supplied - this appears to be used locally by the OM API, so
	local-endian is correct.. I think.. (NAC). This may, however, just
	be because the native-endian order of an Intel system it the same
	as the OM back-end.. so this may require a PUTSHORT.. can't readily tell
	without different hardware platform
    */
    tx_msg_px->header_x.length_u = tx_length_u;
    // set the central module, e.g. B
    tx_msg_px->tx_x.plain_trans_x.central_module_c = central_module_u;
    // set the central server type e.g. O for Order
    tx_msg_px->tx_x.plain_trans_x.server_type_c = server_type_u;
    // this field, being passed up to OM back end, must be in correct-endian format
    PUTSHORT( tx_msg_px->tx_x.plain_trans_x.transaction_number_n, tranaction_no_u );
}   // pack_OM_header

// ----------------------------------------------------------------------------

int32 who_am_i ( click_details_xt *click_px )
/*
Function : who_am_i
Author	 : Neil Cossor 
Modified : 20010706
Description:

    Displays all ID info for logged-on user.
    With 16.5, this includes the info of which transaction/queries/bcasts we
    are authorised for.
*/
{
    unsigned int temp_u;

    if (click_px->logged_on_i == TRUE)
    {
	printf ( "You are logged onto %s using port %s, with username %s\nYou are user %s, trading for %s in %s.\n",
		    click_px->gateway_name_s,
		    click_px->gw_socket_name_s,
		    click_px->user_name_s,
		    click_px->user_code_x.user_id_s,
		    click_px->user_code_x.customer_id_s,
		    click_px->user_code_x.country_id_s );
	printf ("User type %s, %s, trader=%s, %u messages authorised",
		&click_px->user_type_name_s,
		(click_px->internal_user_u == TRUE) ? "Internal" : "External",
		(click_px->non_trader_u) ? "FALSE" : "TRUE",
		click_px->trans_query_bcast_u
		);
	for ( temp_u=0; temp_u < click_px->trans_query_bcast_u; temp_u++ )
	{
	    // where do we spit out a end-of-line
	    if (( temp_u % 5 ) == 0)
		printf ("\n");
	    printf ("  %c%c%-3u %-5s",
			click_px->ise_trans_query_bcast_x [temp_u].transaction_type.central_module_c,
			click_px->ise_trans_query_bcast_x [temp_u].transaction_type.server_type_c,
			click_px->ise_trans_query_bcast_x [temp_u].transaction_type.transaction_number_n,
			(click_px->ise_trans_query_bcast_x [temp_u].is_bcast_u) ? "Bcast" : "     "
		    );
	}
	printf ("\n");
    }
    else
    {
	prompt_logged_on ( "whoami");
    }

    return SUCCESSFUL;
}   // who_am_i

// ----------------------------------------------------------------------------

int32 load_previous_user( click_details_xt * click_px,
			    char * user_filename_ps )
/*
Function : load_previous_user
Author	 : Neil Cossor 
Modified : 20000204
Description:

    If a user file exists, loads the details of the previous user.
    This includes the username, name of Gateway, Gateway port #.
    If also include the password... which is NOT secure,
    as it is just a text file.

Input Parameters:

    (optional) name of user filename.
    Defaults to USER_DATA_FILENAME if not supplied
*/

{
    int8    InputBuffer_s[USER_INPUT_BUFFER_LEN];
    int32   Status_i		= SUCCESSFUL;
    int8    *Token_ps		= NULL;
    int8    *InputBuffer_ps	= NULL;
    FILE    *UserFile_px	= NULL;
    char    *local_filename_ps;     // pointer to default or supplied filename


    // did the caller supply a potentially reasonable filename ?
    if ((!user_filename_ps)
	|| (strlen (user_filename_ps) == 0))
    {
	// not supplied with a filename, use default
	local_filename_ps = USER_DATA_FILENAME;
    }
    else
	// user supplied log_filename_ps, use it
	local_filename_ps = user_filename_ps;

    if ( (UserFile_px = fopen (local_filename_ps, 
			       FILE_READ_ACCESS))
			       != NULL)
    {
	InputBuffer_ps = fgets( &InputBuffer_s[0], sizeof( InputBuffer_s ) - 1, UserFile_px );
	if ( InputBuffer_ps != NULL )
	{
	    InputBuffer_ps = &InputBuffer_s[0];
	    if ( (Token_ps = strtok ( InputBuffer_ps, "/" )) != NULL)
	    {
		// read Username
		strcpy ( &click_px->user_name_s[0], Token_ps );
		if ( (Token_ps = strtok ( NULL, "/" )) != NULL)
		{
		    // read Gateway nodename
		    strcpy ( click_px->gateway_name_s, Token_ps);

		    if ( (Token_ps = strtok ( NULL, "/")) != NULL)
		    {
			// read gateway socket number (as string)
			strcpy ( click_px->gw_socket_name_s, Token_ps);

			if ( (Token_ps = strtok ( NULL, "/")) != NULL)
			{
			    // read user password
			    strcpy ( click_px->password_s, Token_ps);
			}
			else
			{
			    Status_i = INVALID_USER_FILE;
			}
		    }
		    else
		    {
			Status_i = INVALID_USER_FILE;
		    }
		}
		else
		{
		    Status_i = INVALID_USER_FILE;
		}
	    }
	    else
	    {
		Status_i = INVALID_USER_FILE;
	    }
	    
	    fclose ( UserFile_px );
	} 

	return(0);
    }
    else
    {
	Status_i = COULD_NOT_OPEN_USER_FILE;
    }

    return (Status_i);
}   // load_previous_user

// ------------------------------------------------------------------------------------

/*
Function:   rx_click_string
Author :    Neil Cossor
Modified :  20000331
Description:

    routine to transfer an rx'ed click string into a C-style string.

    The input is a fixed-size character array. The input string may or may not be
    NUL terminated (CLICK docs say not, but evidence suggests that they sometimes are).
    We should strip off trailing blanks.

    The target buffer is assumed to be (at least) limit_size+1 long, to allow
    us to put a terminating NUL, even if the source string contains all-significant characters.
*/
void rx_click_string ( void * target_ps,
			void * source_ps,
			int limit_size )
{
    // temp, used to figure out if we can chop off trailing blanks
    char * blank_ps;

    // sensible limit size ?
    if (limit_size < 1)
    {
	printf ("Rx_click_string called with invalid limit size %d. Abort\n",
		limit_size );
	exit (0);
    }

    // point to last allowed char of source string
    blank_ps = (char *)((int)source_ps + limit_size - 1);
    // if limit_size is 1, this will skip the loop, and we will limit the transfer t
    do
    {
	// is this particular character a blank, or a NUL
	if ((*blank_ps != ' ') && (*blank_ps != NUL))
	{
	    // yes, so break from loop
	    break;
	}
	// and decrement the number of non-blank bytes to copy
	// NB limit size is a copy of original number in a local var
	limit_size--;
	// point to previous byte, before we see whether to stop looking
	(int)blank_ps--;
    }
    while ( (unsigned) blank_ps >= (unsigned) source_ps );
    // stopped, with blank_search_ps pointing to non-blank char, which may be just b4 the source string

    // copy at most limit_size chars. Pad with NULs if source has less than limit_size
    // i.e. it aborts early if it finds a NUL.
    // notable though, it does NOT add a NUL at the end, if the size is == limit_size
    strncpy ( (char *)target_ps,
	(char *)source_ps,
	limit_size );
    // add a terminating NUL, in case strncpy didn't - i.e. if limit_size chars were in the source
    *(char *)((int)target_ps + limit_size) = NUL;
}   // rx_click_string

// ------------------------------------------------------------------------------------

/*
Function:   tx_click_string
Author :    Neil Cossor
Modified :  9905xx
Description:

    routine to transfer a C-style string into CLICK format for tx.

    The input is a fixed-size character array. The input string is always NUL terminated.
    The output string MUST be SPACE padded.
*/
void tx_click_string ( void * target_ps,
		void * source_ps,
		int limit_size )
{
    // temp, used to figure out if we can chop off trailing blanks
    char * blank_ps;

    // sensible limit size ?
    if (limit_size < 1)
    {
	printf ("Invalid limit size %d in call to Tx_click_string. Abort\n",
		limit_size );
	exit (0);
    }

    // is the input string correctly terminated.
    if ( (int)strlen ( source_ps ) > limit_size )
    {
	printf ("Tx_click_string called with limit size %d, input string size %d. Abort\n",
		limit_size,
		strlen ( source_ps ));
	exit (0);
    }

    // copy at most limit_size chars. Pad with NULs if source has less than limit_size
    // i.e. it aborts early if it finds a NUL.
    strncpy ( (char *)target_ps,
	(char *)source_ps,
	limit_size );

    // point to NUL at end of output string (from strncpy above)
    blank_ps = (char *)((int)target_ps + strlen (source_ps));
    // now, if strlen ( source_ps ) < limit_size, then we need to stick in some blanks
    memset ( (blank_ps),		// address to start filling at
	' ',			// fill character
	(limit_size - strlen (source_ps))); // how many bytes to fill

}   // tx_click_string

// ------------------------------------------------------------------------------------

/*
Function:   tx_null_click_string
Author :    Neil Cossor
Modified :  9905xx
Description:

    routine to put a blank string in CLICK format for tx.
*/
void tx_null_click_string ( void * target_ps,
			    int limit_size )
{
    // sensible limit size ?
    if (limit_size < 1)
    {
	printf ("Invalid limit size %d in call to tx_null_click_string. Abort\n",
		limit_size );
	exit (0);
    }

    // stick in some blanks
    memset ( (target_ps),		// address to start filling at
	' ',			// fill character
	limit_size );		    // how many bytes to fill

}   // tx_null_click_string

// -----------------------------------------------------------------------------------

int do_click_query_rx ( click_details_xt * click_px,
			generic_txn_xt * tx_request_px,
			int tx_request_size_u,
			uint32 rx_fn_code_u 
			)
/*
Function:   do_click_query_rx
Author:	    Neil Cossor
Modified:   991202
Description:

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
{

    // set size of rx buffer. As this buffer is a union of all the types we expect
    // for any call we have programmed, this will be big enough.
    // Note that we do this on every call, as it will be altered by each call.
    //click_px->rx_length_i = sizeof ( click_px->log_reply_x );
	click_px->rx_length_i = MAX_RESPONSE_SIZE;

    // fill in standard CLICK OUT logger header
    // length of the msg we are logging - our query, plus the fixed header stuff
    click_px->log_out_head_x.msg_len_u = tx_request_size_u + CLICK_Q_EXT_HDR_SIZE;
    click_px->log_out_head_x.msg_status_i = 0;		// no request ID - not triggered by client request
    // note that we are doing a query
    click_px->log_out_head_x.msg_code_u = CLICK_QUERY;

    // log our request/command
    log_msg ( &click_px->log_out_head_x,
		(char *)tx_request_px,
		tx_request_size_u + CLICK_Q_INT_HDR_SIZE );

    // fill in standard CLICK IN logger header
    // set default transaction ID value so we have a "nicer" log, when no trans ID update is done by call
    click_px->log_reply_x.TxnIdent_u = 0;
	 // and the same for a default Order ID
    click_px->log_reply_x.OrderIdent_u = blank;

    // NB, the facility code as a param to the API should be in local-endian order, which it already is

    // grab exclusive access to OMNIAPI - i.e. block out any other threads
    ISE_MUTEX_LOCK ( click_px->omni_io_active_mutex );

    // make a query for one buffer full of data
    click_px->log_in_head_x.msg_status_i =
	omniapi_query (
		&click_px->log_reply_x.TxnStatus_i,		    // secondary return status 
		tx_request_px->facility_u,			    // from the caller
		(omni_message *)&tx_request_px->header_x,	    // our request buffer
		TRUE,						    // flag, immediate response expected
		(int8 *) &click_px->log_reply_x.trans_reply_x,	    // our response buffer
		&click_px->rx_length_i, 			    // size of our response buffer
		(unsigned *)&click_px->log_reply_x.TxnIdent_u,	    // Transaction ID
		(unsigned *)&click_px->log_reply_x.OrderIdent_u     // Order ID - irrelevant on this query
		);

    // release the synchronisation lock so other threads can do OMNIAPI IOs
    ISE_MUTEX_UNLOCK ( click_px->omni_io_active_mutex );

    // note the length of the msg we are logging - our response, plus the fixed header stuff
    click_px->log_in_head_x.msg_len_u = click_px->rx_length_i + CLICK_R_EXT_HDR_SIZE;
    // if we had a bad status, then we are just logging a header/statii
    if ( click_px->log_in_head_x.msg_status_i < 0 )
    {
	// error - note we are logging a status msg
	click_px->log_reply_x.fn_code = LOG_K_STATUS;

	// report the error
	report_click_error ( click_px,
				click_px->log_in_head_x.msg_status_i,
				click_px->log_reply_x.TxnStatus_i);

	// log our bad statii
	log_msg ( &click_px->log_in_head_x, 
		    (char *)&click_px->log_reply_x,
		    click_px->rx_length_i + CLICK_R_INT_HDR_SIZE );

	// bail out, with bad return status
	return -1;
    }

    // OK, we got what we expected. Did the caller know exactly what format of response
    // we were expecting i.e. can we correctly log the response in this generic routine ?
    if ( rx_fn_code_u != 0)
    {
	// OK, there was a simple, known expected response, which we can log.
	// put the fn code of that response into the log header, so logger can do the right thing
	click_px->log_reply_x.fn_code = rx_fn_code_u;

	// log our response
	log_msg ( &click_px->log_in_head_x,
	    (char *)&click_px->log_reply_x, 
	    click_px->rx_length_i + CLICK_R_INT_HDR_SIZE );
    }
    /* the else case here is where the caller must examine the details of the
	response (e.g. some of the data) before it knows how to correctly log
	the response.. at which point the caller should call log_msg
    */

    // success - return good status to caller
    return click_px->log_in_head_x.msg_status_i;
}   // do_click_query_rx

// -----------------------------------------------------------------------------------

int do_click_tx_rx ( click_details_xt * click_px,
		    generic_txn_xt * tx_request_px,
		    int tx_request_size_u )
/*
Function:   do_click_tx_rx
Author:	    Neil Cossor
Modified:   991202
Description:

    We wish to do a tx/rx with CLICK. These are typically used where there is no
    response, other than the status i.e. no data rxed as part of the operation.

    The behaviour, initial parameters etc are exactly the same as do_click_tx_rx,
    excepting the specific internal handling that is different between omniapi_query
    and omniapi_tx.

    There are less params in an omniapi_tx,
    and the "send" buffer is passed by the address of a tx vector, which has the first
    entry pointing to the caller's buffer, while query passes the address of the buffer.

    As per do_click_query_rx for input parameters, return statii.

    NB the facility_u, in the buffer, may be one of the "specials", like OMNI_FACTYP_LOGIN
*/
{
    // fill in standard CLICK OUT logger header
    // length of the msg we are logging - our query, plus the fixed header stuff

	click_px->log_out_head_x.msg_len_u = tx_request_size_u + CLICK_Q_EXT_HDR_SIZE;
    click_px->log_out_head_x.msg_status_i = 0;		// no request ID - not triggered by client request
    // note that we are doing a tx/rx
    click_px->log_out_head_x.msg_code_u = CLICK_TX;

    // log our request/command
    log_msg ( &click_px->log_out_head_x,
		(char *)tx_request_px,
		tx_request_size_u + CLICK_Q_INT_HDR_SIZE );

    // fill in standard CLICK IN logger header
    // set default transaction ID value so we have a "nicer" log,
    // when no trans ID update is done by call
    click_px->log_reply_x.TxnIdent_u = 0;
    // and the same for a default Order ID
    click_px->log_reply_x.OrderIdent_u = blank;


    // set the transaction vector to point to the message we wish to send
    click_px->OM_tx_vector_px[0] = &tx_request_px->header_x;

    // NB, the facility code as a param to the API should be in local-endian order, which it already is

    // grab exclusive access to OMNIAPI - i.e. block out any other threads
    ISE_MUTEX_LOCK ( click_px->omni_io_active_mutex );
	
    click_px->log_in_head_x.msg_status_i =
	omniapi_tx (
	    &click_px->log_reply_x.TxnStatus_i, 		// secondary return status 
	    tx_request_px->facility_u,				// from the caller - EP/ET<n>
	    (omni_message **)&click_px->OM_tx_vector_px[0],	// vector of Tx messages (points to our 1)
	    (unsigned *)&click_px->log_reply_x.TxnIdent_u,	// Transaction ID
	    (unsigned *)&click_px->log_reply_x.OrderIdent_u	// Order ID - irrelevant on this query
	    );

    // release the synchronisation lock so other threads can do OMNIAPI IOs
    ISE_MUTEX_UNLOCK ( click_px->omni_io_active_mutex );

    // note the length of the msg we are logging - our response, plus the fixed header stuff
    click_px->log_in_head_x.msg_len_u = CLICK_R_EXT_HDR_SIZE;
    // we are ALWAYS just logging a status msg
    click_px->log_reply_x.fn_code = LOG_K_STATUS;

    // log our good / bad statii
    log_msg ( &click_px->log_in_head_x, (char *)&click_px->log_reply_x, CLICK_R_INT_HDR_SIZE );

    // if we had a bad status, then report it and return bad status
    if ( click_px->log_in_head_x.msg_status_i < 0 )
    {
	// report the error
	report_click_error ( click_px,
				click_px->log_in_head_x.msg_status_i,
				click_px->log_reply_x.TxnStatus_i);

	// bail out, with bad return status
	return click_px->log_in_head_x.msg_status_i;
    }
    else
	// success - return secondary status to caller - which are may be informative
	return click_px->log_reply_x.TxnStatus_i;
}   // do_click_tx_rx

// ------------------------------------------------------------------------------

gen_buf_node_xt * new_gen_buf_node ( int size_of_msg_buffer )
/*
    fn which allocates a gen_buf_node_xt, in preparation for the caller
    to add more stuff to it, set additional flags, etc
    It zeroes the msg length field of the structure
    It saves the msg_code supplied into the structure
    It zots the "special purpose" flags to clear / false / zero.

    Input parameters are
    the msg_code, which is stored in the header field
    the size of msg buffer to allocate in the gen_buf_node_xt
*/
{
    gen_buf_node_xt *msg_node;

    // allocate the msg node
    msg_node = (gen_buf_node_xt *) malloc (sizeof (gen_buf_node_xt));
    // zero the memory that was allocated, before allocating the substruct
    memset ( msg_node, 0, sizeof (gen_buf_node_xt));
    // allocate the nominated amount of space for the contents of the actual msg
    msg_node->msg_p = (char *) malloc (size_of_msg_buffer);
    // zero the memory that was allocated for the substruct
    memset ( msg_node->msg_p, 0, size_of_msg_buffer);

    if (msg_node->msg_p == NULL)
    {
//	printf_time ();
	printf ("urk - no mem allocated by malloc for new_gen_buf_node\n" );
    }

    // clear other flags - so we don't get leftover values

    // zot the length. The caller can put something in (such as the size_of_msg_buffer),
    // or the length of the string contents, if it feels so inclined
    msg_node->msg_len_i = 0;

    return msg_node;
}   // new_gen_buf_node

// ------------------------------------------------------------------------------

void lock_q (node_header_xt *header_px )
/*
    locks access to a Q temporarily.

    Whichever thread calls this will lock until it gains exclusive access to the Q.
    A call to unlock_q should be called when the thread is done with this Q
*/
{
    // grab synchronisation lock on the structure, so inter-thread clashes don't
    // screw up the Q

    ISE_MUTEX_LOCK ( header_px->hMutex );
}   // lock_q

// ------------------------------------------------------------------------------

void unlock_q (node_header_xt *header_px )
/*
    unlocks the exclusive lock on a Q.

    A call to lock_q should have been called before this.
*/
{
    // release the synchronisation lock, so other threads can get at the structure
    ISE_MUTEX_UNLOCK ( header_px->hMutex );
}   // unlock_q

// ------------------------------------------------------------------------------

void add_to_q_tail(node_header_xt *header_px,
		    struct q_chain_t *new_node)
/*
    Adds msg to a buffer Q.
    Takes a gen_buf_node_xt as input, and inserts it into the
    nominated Q, at the tail
*/
{
    // grab synchronisation lock on the structure, so inter-thread clashes don't
    // screw up the Q
    lock_q ( header_px );

    // NB we maintain the Q in FIFO order. We insert at tail, remove from head
    // set pointers - we are the last entry, so forward pointer of the new entry is NULL
    new_node->next = NULL;
    // set the back pointer of the new entry at whatever the last was - which may be NULL
    new_node->prev = header_px->chain.prev;
    if (header_px->chain.prev == NULL)
    {
	// Q was formerly empty - make head point to our new (only) entry
	header_px->chain.next = new_node;
    }
    else
    {
	// Q has at least one entry, update the next pointer of the formerly last slot
	header_px->chain.prev->next = new_node;
    }
    
    // move tail pointer to point to our new entry
    header_px->chain.prev = new_node;

    // adjust the length of the Q
    header_px->length_i++;

    if (header_px->length_i > header_px->max_length_i)
	header_px->max_length_i = header_px->length_i;

    // release the synchronisation lock, so other threads can get at the structure
    unlock_q ( header_px );
}   // add_to_q_tail

// ------------------------------------------------------------------------------

void remove_buff_from_q (node_header_xt *header_px,
			struct q_chain_t *tmp_node_px)
/*
    fn to remove a nominated node from the nominated Q.
    It may be at the head, tail, combinations, or somewhere in the middle

    The real work of this fn is to make sure that the pointers in the
    chain are intact AFTER this entry is removed
*/
{
    // grab synchronisation lock on the structure, so inter-thread clashes don't
    // screw up the Q
    lock_q ( header_px );

    // could cross-check that the node is actually in the list


    // is this the front of the Q ?
    if (header_px->chain.next == tmp_node_px)
    {
	// we are removing from the front of the Q
	// this MAY also be the tail
	if (header_px->chain.prev == tmp_node_px)
	{
	    // removing the only one in the Q
	    header_px->chain.prev = NULL;
	    header_px->chain.next = NULL;
	}
	else
	{
	    // removing header_px, which is NOT the only one in the Q
	    header_px->chain.next = tmp_node_px->next;
	    // adjust backwards pointer of new head to be NULL
	    header_px->chain.next->prev = NULL;
	}
    }
    else
    {
	// NOT removing from front
	// are we removing from tail ? (which we know is NOT front)
	if ( header_px->chain.prev == tmp_node_px)
	{
	    // removing from (non-front) tail
	    header_px->chain.prev = tmp_node_px->prev;
	    // adjust forwards pointer of new tail to be NULL
	    header_px->chain.prev->next = NULL;
	}
	else
	{
	    // not at head or tail...
	    // removing from middle of Q - make the surrounding entries point at each other
	    tmp_node_px->next->prev = tmp_node_px->prev;
	    tmp_node_px->prev->next = tmp_node_px->next;
	}   // is this node at the tail of the Q (which is NOT the head)
    }	// if this node at the head of the Q

    // adjust the length of the Q
    header_px->length_i--;

    // release the synchronisation lock, so other threads can get at the structure
    unlock_q ( header_px );
}   // remove_buff_from_q

// ------------------------------------------------------------------------------

struct q_chain_t * remove_front_from_q (node_header_xt *header_px )
/*
    fn to remove the front node from the nominated Q.
    Returns a NULL if the Q is empty.

    The real work of this fn is to make sure that the pointers in the
    chain are intact AFTER this entry is removed.

    It returns the pointer to the Q chain structure which is the first field in
    whatever structure it is in i.e. it returns pointer to structure which (was) in a Q
*/
{
    // the address of the node we return
    struct q_chain_t *tmp_node_px;

    // grab synchronisation lock on the structure, so inter-thread clashes don't
    // screw up the Q
    lock_q ( header_px );

    // we are removing from the front of the Q
    // this MAY also be the tail - or may be NULL
    tmp_node_px = header_px->chain.next;
    // was the Q empty ?
    if (tmp_node_px)
    {
	// not empty Q - is there only one node in the Q
	if (header_px->chain.prev == tmp_node_px)
	{
	    // removing the only one in the Q
	    header_px->chain.prev = NULL;
	    header_px->chain.next = NULL;
	}
	else
	{
	    // removing header_px, which is NOT the only one in the Q
	    header_px->chain.next = tmp_node_px->next;
	    // adjust backwards pointer of new head to be NULL
	    header_px->chain.next->prev = NULL;
	}

	// adjust the length of the Q
	header_px->length_i--;
    }

    // release the synchronisation lock, so other threads can get at the structure
    unlock_q ( header_px );

    return tmp_node_px;
}   // remove_buff_from_q

// ------------------------------------------------------------------------------

void init_q ( node_header_xt *header_px )
/*
    initialise a Q header_px.
    This includes creation of the OS-specific synchronisation locks for the Q header_px
*/
{
    header_px->chain.prev = NULL;
    header_px->chain.next = NULL;

    // create the synchronisation lock for this Q header_px - required by the insert / remove routines
    // to guarantee the process threads don't interfere with each other
    ISE_MUTEX_INIT ( header_px->hMutex );

    // no members - a new Q
    header_px->length_i = 0;
}   // init_q

// ------------------------------------------------------------------------------

void flush_gen_buf_node_q (node_header_xt *header_px,
			    int release_child )
/*
Function:   flush_gen_buf_node_q
Author:	    Neil Cossor
Modified:   20000606
Description:

    release all buffers on a Q, 
    DO NOT delete the actual Q object itself

    This is a specific routine for releasing entries in a Q of gen_buf_node_xt's,
    as these have a substructure which was also allocated, and hence must be deallocated.

    Note that there are some cases, where the block of data pointed to by msg_p
    was NOT allocated in new_gen_buf_node.. and we should NOT release the stuff
    pointed to by it.. that free() is the responsibility of whoever did allocate it.

    To kill the child, then supply a TRUE for release_child, otherwise supply FALSE
*/
{
    // temp - address of the node we remove (if there are any)
    gen_buf_node_xt *tmp_node_px;

    // grab the synchronisation lock for this Q header_px

    lock_q ( header_px );

    // free up all entries on the Q
    while (header_px->chain.next)
    {
	// there is still an entry on the Q
	// point to that first entry on the Q
	tmp_node_px = (gen_buf_node_xt *)header_px->chain.next;
	// advance header_px to following entry (which may be NULL), for next go in the loop
	header_px->chain.next = tmp_node_px->chain.next;
	// release the memory allocate to this Q entry
	// first the character array that was allocated
	if ( release_child == TRUE )
	    free ( tmp_node_px->msg_p );
	// and then the actual Q entry node
	free ( tmp_node_px );
    }

    // and update the Q headers to be an empty Q
    header_px->chain.prev = NULL;
    header_px->chain.next = NULL;
    // and the count of entries
    header_px->length_i = 0;

    // release the synchronisation lock, so other threads can get at the structure
    unlock_q ( header_px );
}   // flush_gen_buf_node_q

// ------------------------------------------------------------------------------

void free_gen_buf_node_q (node_header_xt *header_px,
			  int release_child )
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
{
    // temp - address of the node we remove (if there are any)
    gen_buf_node_xt *tmp_node_px;

    // grab the synchronisation lock for this Q header_px

    lock_q ( header_px );

    // free up all entries on the Q
    while (header_px->chain.next)
    {
	// there is still an entry on the Q
	// point to that first entry on the Q
	tmp_node_px = (gen_buf_node_xt *)header_px->chain.next;
	// advance header_px to following entry (which may be NULL), for next go in the loop
	header_px->chain.next = tmp_node_px->chain.next;
	// release the memory allocate to this Q entry
	// first the character array that was allocated
	if ( release_child == TRUE )
	    free ( tmp_node_px->msg_p );
	// and then the actual Q entry node
	free ( tmp_node_px );
    }

    // release the synchronisation lock
    ISE_MUTEX_DESTROY ( header_px->hMutex );

    // and update the Q headers to be an empty Q
    header_px->chain.prev = NULL;
    header_px->chain.next = NULL;
    // and the count of entries
    header_px->length_i = 0;

}   // free_gen_buf_node_q

// ------------------------------------------------------------------------------

void free_gen_buf_node (gen_buf_node_xt *tmp_node_px )
/*
    release one gen_buf_node_xt.

    If this gen_buf_node_xt has a substructure allocated (pointed to by msg_p)
    then release that too.

    If this buffer is still currently on a Q,
    then things will go DRASTICALLY WRONG
*/
{
    // release the memory allocate to this Q entry
    // first the character array that was allocated, if there is one
    if ( tmp_node_px->msg_p != NULL )
	free ( tmp_node_px->msg_p );

    // and then the actual Q entry node
    free ( tmp_node_px );
}   // free_gen_buf_node

// ------------------------------------------------------------------------------

int empty_q (node_header_xt *header_px )
// returns true (non-zero) if Q is empty, false otherwise
{
    int ret_value;

    // grab synchronisation lock on the structure, so inter-thread clashes don't
    // screw up the Q
    lock_q ( header_px );

    if (header_px->chain.next)
	ret_value = FALSE;
    else
	ret_value = TRUE;

    // release the synchronisation lock, so other threads can get at the structure
    unlock_q ( header_px );

    return ret_value;
}   // empty_q

// ------------------------------------------------------------------------------

void printf_time ()
/*
    fn which grabs the current date/time, and spits out the result in readable form to stdout
    It does NOT add any newlines etc, just a space after the date/time.
    This way, we can get date/time at the start of lines of text logged.
*/
{
    // var in which we store the current time
    time_t curr_time;
    // and in structure form - NB pointer, struct allocated in call to localtime
    struct tm *curr_time_t;
    // and in formatted string form
    char curr_time_s [50];
    size_t size_of_curr_time = sizeof (curr_time_s);

    // grab the current time
    time ( &curr_time);

    // convert from calendar time to local time
    // returns a pointer to a static data area - do NOT have to release *curr_time_t
    curr_time_t = localtime ( &curr_time );

    // format the current date/time
    // desired format is yyyy-mm-dd:hh:mm:ss, with 24 hour clock, trailing blank
    strftime ( (char *)&curr_time_s,	// (char *) to avoid compiler warnings char * != char *[50]
	size_of_curr_time, "%Y-%m-%d:%H:%M:%S ", curr_time_t );

    // and spit out that formatted string into stdout
    printf ("%s", curr_time_s );
}   // printf_time

// ------------------------------------------------------------------------------

void fprintf_time (FILE *output_px)
/*
Function:   fprintf_time ()
Author:	    Neil Cossor
Modified    20000605
Description:

    fn which grabs the current date/time, and spits out the result in readable form to 
    the nominated file. Just like printf_time(), which goes to stdout.

    It does NOT add any newlines etc, just a space after the date/time.
    This way, we can get date/time at the start of lines of text logged.
*/
{
    // var in which we store the current time
    time_t curr_time;
    // and in structure form - NB pointer, struct allocated in call to localtime
    struct tm *curr_time_t;
    // and in formatted string form
    char curr_time_s [50];
    size_t size_of_curr_time = sizeof (curr_time_s);

    // grab the current time
    time ( &curr_time);

    // convert from calendar time to local time
    // returns a pointer to a static data area - do NOT have to release *curr_time_t
    curr_time_t = localtime ( &curr_time );

    // format the current date/time
    // desired format is yyyy-mm-dd:hh:mm:ss, with 24 hour clock, trailing blank
    strftime ( (char *)&curr_time_s,	// (char *) to avoid compiler warnings char * != char *[50]
	size_of_curr_time, "%Y-%m-%d,%H:%M:%S, ", curr_time_t );

    // and spit out that formatted string into stdout
    fprintf (output_px, "%s", curr_time_s );
}   // printf_time

// ------------------------------------------------------------------------------------

void unpack_click_bit_date ( uint16 date,
		    uint16 * yyyy_u,
		    uint8 * mm_u,
		    uint8 * dd_u )
/*
Function:   unpack_click_bit_date
Author:     Neil Cossor
Modified:   20000105
Description:

    Used to extract the (bit-packed) date format used by CLICK in series,
    and put the results into the three supplied
    variables.
    
    The bit-packed date format is a 16-bit number, with
	7 bits giving the offset year, with the offset being 1989. So a value of 1 == 1990
	4 bits gving the mongth, where 1 = January
	5 bits giving the day of month.

    We could handle this as a typecast onto a structure which contains bit-sized fields,
    but the packing of these within a structure is not standardised. Neither is it
    standard whether a multi-bit field may cross a byte boundary (as one does here)
    in different compilers.
*/
{
    // we use a local copy of the date, as it is apparently endian
    uint16 exp_date_u;

    // do byte-swap if necessary
    PUTSHORT ( exp_date_u, date );

    // if no date was supplied (i.e. all bits off) then just return all zeroes
    if (exp_date_u == 0)
    {
	*dd_u = 0;
	*mm_u = 0;
	*yyyy_u = 0;
    }
    else
    {
	// extract the day of month - low five bits
	*dd_u = exp_date_u & 0x001F;
	// and the month - next 4 bits, shifted down 5 bits
	*mm_u = ( exp_date_u >> 5 ) &  0x000F;
	// and the year - shifted down 11 bites, remaining 7 bits, plus year offset
	*yyyy_u = (( exp_date_u >> 9 ) & 0x007F ) + CLICK_BIT_YEAR_OFFSET;
    }
}   // unpack_click_bit_date

// ------------------------------------------------------------------------------------

void pack_click_bit_date ( uint16 *date,
		    uint16 yyyy_u,
		    uint8 mm_u,
		    uint8 dd_u )
/*
Function:   pack_click_bit_date
Author:     Neil Cossor
Modified:   20000105
Description:

    used to pack the (bit-packed) bit-field packed date used by CLICK in series.
    Use as inputs the three supplied values.
    
    The bit-packed date format is as per unpack_click_bit_date (above)

    NOTE that if all three input params are 0, then pack a zero
    DO NOT just include the 'orrible year offset
*/
{
    // we use a local copy of the date, as it is apparently endian
    uint16 exp_date_u;

    // OK, here we go
    if (( yyyy_u == 0 ) && ( mm_u == 0 ) && ( dd_u == 0 ))
	exp_date_u = 0;
    else
	exp_date_u = (( yyyy_u - CLICK_BIT_YEAR_OFFSET ) << 9 )
		| ( (uint16)mm_u << 5 ) 	// cast b4 shift, in case it shifts out lhs of uint8
		| ( (uint16)dd_u );		// "

    // do byte-swap if necessary
    PUTSHORT ( *date, exp_date_u );
}   // pack_click_bit_date



//----------------------------------------------------------------------------

uint8 cmd_action(char * action_s) {
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
	4 == "Change To / ChEck Greater",
	5 == "Cancel and Replace",

  250 == not found
*/

   int i;
   for (i = 0; i < 6; i++) {
	   if (!(strcmp(actions_s[i], action_s))) {
		 return i + 1;
	   }
   }
   printf ("Action must be Change To|Reduce By|Change To / Check Equal|Change To / Check Greater|Cancel and Replace\n");
   return 250;
}



uint16_t cmd_order_select (char * order_select_s) {
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
	int i;
	for (i = 0; i < 8; i++) {
	   if (!(strcmp(order_selects_s[i], order_select_s))) {
		 return i;
	   }
	}
	printf ("Order Selection must be GTC|DAY|MINUTES|IMMEDIATE|PART ORDER|WHOLE ORDER|LOCKED ORDERS|ACTIVE ORDERS\n");
	return 250;
}
// ------------------------------------------------------------------------------------

uint8 cmd_bid_ask( click_details_xt * click_px,
		    char *bid_ask_input_s )
/*
Function:   cmd_bid_ask
Author:     Neil Cossor
Modified:   9907xx
Description:

    The user has a command which (should have) selected a BID or ASK.
    From the supplied string, return an appropriate value, as used in
    the bid_ask_c field in many messages.

Input params:

    The string which supplied on the command line.
    This can be %O, which says to use the bid/ask from the last snapshot-saved order
	(which may, of course, be 0)

Output params:

    The value to use. As per DFI
    0 == BOTH (for query selectors, wildcard deletes)
    1 == BID
    2 == ASK
    3 if illegal selection supplied
    NB %O, which is value from last order will be 1 or 2 (after we saved an order)
	or 0 (if no saved order)
*/
{
    uint8   bid_ask_u;
    // customer selection of bid/ask
    bid_ask_u = select_string_option ( 4, &bid_ask_s[0], bid_ask_input_s );
    if ( bid_ask_u == 3 )
    {
	// %O specified - use whatever value is in the saved last order
	bid_ask_u = click_px->saved_order_x.bid_ask_u;
    }
    return bid_ask_u;
}   // cmd_bid_ask

// ------------------------------------------------------------------------------------

uint8 cmd_user_member( char *user_member_input_s )
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
{
    uint8   user_member_u;
    // customer selection of user/member
    user_member_u = select_string_option ( 2, &user_member_s[0], user_member_input_s );
    if ( user_member_u == 2 )
    {
	// oops - not recongnisable choice - warn, but default to member
	user_member_u = user_member_member_e;
	printf ("warning - did not recognise %s, should be User|Member (defaulted to Member)\n",
		    user_member_input_s );
    }
    return user_member_u;
}   // cmd_user_member

// ------------------------------------------------------------------------------------

uint16	cmd_validity_time ( char *validity_time_input_s,
			    uint16 default_validity_time_u )
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
{
    uint8   units_u,
	    value_u = 0;

    uint16  validity_time_u;
    // which string they specified
    int     choice_i;

    if ((validity_time_input_s == NULL) || ( strlen ( validity_time_input_s ) == 0 ))
    {
	return default_validity_time_u;
    }
    else
    {

	choice_i = select_string_option ( 10, &validity_time_s[0], validity_time_input_s );

	// figure out which validity time they selected
	switch ( choice_i )
	{
	case valid_fok_e :	    // Fill or Kill
	    units_u = VALIDITY_UNIT_FOK;
	    break;
	case valid_gtc_e :	    // Good till cancel - default
	    units_u = VALIDITY_UNIT_OLD_GTC;
	    break;
	case valid_ioc_e :	    // Immediate or cancel
	    units_u = VALIDITY_UNIT_IOC;
	    break;
	case valid_rod_e:	    // Rest of Day
	    units_u = VALIDITY_UNIT_OLD_DAY;
	    break;
	case valid_fas_day_e:    // FillAndStore, Day
	    units_u = VALIDITY_UNIT_FAS_DAY;
	    break;
	case valid_fas_gtc_e:    // FillAndStore, GTC
	    units_u = VALIDITY_UNIT_FAS_GTC;
	    break;
	case valid_fas_time_e:  // FillAndStore, timed
	    units_u = VALIDITY_UNIT_FAS_TIMED;
	    // %%%% default to 1 hour for the moment
	    value_u = 60;
	    break;
	case valid_aon_day_e:    // AllOrNone, Day
	    units_u = VALIDITY_UNIT_AON_DAY;
	    break;
	case valid_aon_gtc_e:    // AllOrNone, GTC
	    units_u = VALIDITY_UNIT_AON_GTC;
	    break;
	case valid_aon_time_e:    // AllOrNone, timed
	    units_u = VALIDITY_UNIT_AON_TIMED;
	    // %%%% default to 1 hour for the moment
	    value_u = 60;
	    break;
	default :
	    printf ("Illegal validity time qualifier - FOK|GTC|IOC|ROD|FASday|FASGTC|FAS|AONday|AONGTC|AON\n" );
	}	// switch on validity time
    }	// else on whether we had a selection string, or are using a defaulted value

    validity_time_u = ( units_u << 8 ) | value_u;

    return validity_time_u;
}   // cmd_validity_time

// ------------------------------------------------------------------------------------

uint8	cmd_client_category ( char *client_category_input_s,
				uint8 default_client_category_u )
/*
Function:   cmd_client_category
Author:     Neil Cossor
Modified:   20010411
Description:

    The command line has a client category string
    Convert this to a client category numeric value, as per DFI.

Input Params:

    the command line parameter string.
	It should be one of
	MM
	CLIENT
	FIRM
	FARMM
	DISCRETIONARY
	BROKERDEALER

    the default value to use, if no selector string is supplied

Output Params:

    the encoded value,
    or 0 if the user did NOT supply an allowed choice.

    An error msg is displayed on stdout in this case
*/
{
    uint8   choice_i;
    uint8   client_category_u;

    if ( client_category_input_s == NULL )
	return default_client_category_u;

    // customer specified a client category.. C[ustomer] | FI[rm\ | FA[rmm]
    choice_i = select_string_option ( 6, client_category_s, client_category_input_s );
    switch ( choice_i )
    {
    case client_cat_mm_e:
	// this one is erratically documented by OM. It is listed as undefined in some places,
	// is explicitly stated in a couple of places in 16.3 and 15.7 for MM orders
	client_category_u = CUST_CATEGORY_MM;
	break;
    case client_cat_cust_e :	// customer
	client_category_u = CUST_CATEGORY_CUSTOMER;
	break;
    case client_cat_firm_e :	// FIRM
	client_category_u = CUST_CATEGORY_FIRM;
	break;
    case client_cat_farmm_e :	// FARMM
	client_category_u = CUST_CATEGORY_FARMM;
	break;
    case client_cat_disc_e :
	client_category_u = CUST_CATEGORY_DISCRETIONARY;
	break;
    case client_cat_bd_e:
	// used to be entered with "B/D" in BrokerID. Trades under Firm ratios/rules etc,
	// but clears with OCC as "C"ustomer. Used by EAMs for orders NOT from retail consumers.
	client_category_u = CUST_CATEGORY_BROKERDEALER;
	break;
    default :
	printf ("Illegal Client category qualifier - C[ustomer] | FI[rm] | FA[rmm] | D[iscretionary] | B[rokerdealer]\n");
	client_category_u = 0;	// actually, 0 appears to be legal in some cases
    }	// switch on order type

    return client_category_u;
}   // cmd_client_category

// ------------------------------------------------------------------------------------

uint8 cmd_order_type ( char * order_type_input_s,
			uint8 default_order_type_u )
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
{
    uint8   order_type_u;
    int     choice_i;

    if ( order_type_input_s == NULL )
	return default_order_type_u;

    // customer specified a order type
    choice_i = select_string_option ( 2, &order_type_s[0], order_type_input_s );
    switch ( choice_i )
    {
    case order_type_limit_e :	// Limit order
	order_type_u = ORDER_TYPE_LIMIT;
	break;
    case order_type_market_e :	// Market order
	order_type_u = ORDER_TYPE_MARKET;
	break;
    default :
	printf ("Illegal Order type qualifier - Limit | Market\n");
	order_type_u = UNDEFINED_UINT8;
    }	// switch on order type

    return order_type_u;
}   // cmd_order_type

// ------------------------------------------------------------------------------------

void cmd_orderID ( click_details_xt *click_px,
			char * order_ID_s, quad_word *delete_orderID_u  )
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
    or shortcuts
    %O, use the info from the last order that was snapshot-saved
    %T, use the info from the last trade that was snapshot-saved

Output Params:

    the encoded value,
    or binary 0 if not translatable

    An error msg is displayed on stdout in this case
*/
{
    // the value we will return to the caller
    quad_word	order_ID_u;
    //uint64_xt	*order_ID_pxu = (uint64_xt *)&order_ID_u;
   	quad_conv_t * convOrderNo;

    memset ( &order_ID_u, 0, sizeof ( order_ID_u ));

    if (strcmp ( order_ID_s, "%O" ) == 0 )
    {
	// use orderID from last snapshot
	memcpy ( &order_ID_u,
		    &click_px->saved_order_x.orderID_u,
		    sizeof ( order_ID_u ));
		    
    }
    else if (strcmp ( order_ID_s, "%T" ) == 0 )
    {
	printf ("Last Trade options not yet implemented\n");
    }
    else
    {
	// not one of these.. it MUST contain two hex numbers
	convOrderNo = (quad_conv_t *) &(order_ID_u.quad_word);

	sscanf ( order_ID_s, "%08lX:%08lX",
				&(convOrderNo->l2),
				&(convOrderNo->l1));
    }

    // customer specified a order ID
	memcpy (delete_orderID_u, &order_ID_u, sizeof(order_ID_u));
}   // cmd_orderID


// ------------------------------------------------------------------------------------
/*
Function:   cmd_ul_price_method
Author:     Eric Salem
Modified:	4/9/2002
Description:

    Changes a string to a number representing the Underlying Price Method

Input Params:
	
	  NO BOUNDARIES|BID|ASK|LAST PAID|ADJUSTED LAST PAID
    

Output Params:

    The proper uint8 representing the ul_price_method or 250 if not found
*/
char cmd_ul_price_method(char * method_s) {
   int i;
   for (i = 0; i < 6; i++) {
	   if (!(strcmp(ul_price_methods_s[i], method_s))) {
		 return i + 1;
	   }
   }
   printf ("Underlying Price Method must be No Boundaries|Bid|Ask|Last Paid|Adjusted Last Paid\n");
   return -1;
}


// ------------------------------------------------------------------------------------

char * validity_time_to_string ( uint16 validity_time_u )
/*
Function:   validity_time_to_string
Author:     Neil Cossor
Modified:   20010526
Description:

    We have a numeric validity time value
    Convert this to a validity time string i.e. readable form

Input Params:

    the validity time (as per DFI)
    From release 16.5+, validity time can contain timed periods (in minutes),
    and is packed quite differently. The old IOC and FOK values are supported,
    but will be deprecated, probably in release 17.0

Output Params:

    pointer to (static) string with the validity time in string form
    If it is an unknown type, then the string will be the code in numeric form
*/
{
    uint8   units_u,
	    value_u;

    units_u = validity_time_u & 0xff;
    value_u = validity_time_u >> 8;

    switch ( value_u )
    {
	case VALIDITY_UNIT_OLD_MATCH :
	    // one of the deprecated IOC/FOK values
	    {
		switch (units_u)
		{
		case VALIDITY_VALUE_OLD_FOK:
			return validity_time_s[0];
		    break;
		case VALIDITY_VALUE_OLD_IOC:
			return validity_time_s[2];
		    break;
		default:
		    return validity_time_s[10];
		}   // switch on units, with old 0 for unit
	    }
	    break;
	case VALIDITY_UNIT_OLD_DAY:
		return validity_time_s[3];
	    break;
	case VALIDITY_UNIT_OLD_GTC:
		return validity_time_s[1];
	    break;
	case VALIDITY_UNIT_IOC:
		return validity_time_s[2];
	    break;
	case VALIDITY_UNIT_FOK:
		return validity_time_s[0];
	    break;
	case VALIDITY_UNIT_FAS_DAY:
		return validity_time_s[4];
	    break;
	case VALIDITY_UNIT_FAS_GTC:
		return validity_time_s[5];
	    break;
	case VALIDITY_UNIT_FAS_TIMED:
		return validity_time_s[6];
	    break;
	case VALIDITY_UNIT_AON_DAY:
		return validity_time_s[7];
	    break;
	case VALIDITY_UNIT_AON_GTC:
		return validity_time_s[8];
	    break;
	case VALIDITY_UNIT_AON_TIMED:
		return validity_time_s[9];
	    break;
    default:
	return validity_time_s[10];
    }	// switch on numeric value
}   // validity_time_to_string

// ------------------------------------------------------------------------------------

char * order_category_to_string ( uint16 order_category_u )
/*
Function:   order_category_to_string
Author:     Neil Cossor
Modified:   20010604
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
{
    switch ( order_category_u )
    {
    case ORDER_CAT_ORDER:
	return order_category_s[0];
	break;
    case ORDER_CAT_QUOTE:
	return order_category_s[1];
	break;
    case ORDER_CAT_BLOCK:
	return order_category_s[2];
	break;
    case ORDER_CAT_BLOCK_RESPONSE:
	return order_category_s[3];
	break;
    case ORDER_CAT_FACILITATION:
	return order_category_s[4];
	break;
    case ORDER_CAT_FACILITATION_RESPONSE:
	return order_category_s[5];
	break;
    case ORDER_CAT_CAB:
	return order_category_s[6];
	break;
    case ORDER_CAT_DERIVED:
	return order_category_s[7];
	break;
    case ORDER_CAT_IMPROVE_QUOTE:
	return order_category_s[8];
	break;
    case ORDER_CAT_COMBO_LIMIT:
	return order_category_s[9];
	break;
    case ORDER_CAT_COMBO_INTEREST:
	return order_category_s[10];
	break;
    default:
	return order_category_s[11];
    }	// switch on numeric value
}   // order_category_to_string

// ------------------------------------------------------------------------------------

char * client_category_to_string ( uint8 client_category_u )
/*
Function:   client_category_to_string
Author:     Neil Cossor
Modified:   20010411
Description:

    We have a numeric client category value
    Convert this to a string i.e. readable form

Input Params:

    the client category (as per DFI)

Output Params:

    pointer to (static) string with the client categoryin string form
    If it is an unknown type, then the string will be the code in numeric form
*/
{
    uint16  cat_u = client_category_u;

    switch ( cat_u )
    {
    case CUST_CATEGORY_MM:
	return client_category_s[ cat_u];
	break;
    case CUST_CATEGORY_CUSTOMER:
	return client_category_s[1];
	break;
    case CUST_CATEGORY_FIRM:
	return client_category_s[2];
	break;
    case CUST_CATEGORY_FARMM:
	return client_category_s[3];
	break;
    case CUST_CATEGORY_DISCRETIONARY:
	return client_category_s[4];
	break;
    case CUST_CATEGORY_BROKERDEALER:
	return client_category_s[5];
	break;
    default:
	return client_category_s[6];
    }	// switch on client category
}   // client_category_to_string

// ------------------------------------------------------------------------------------

char * bid_ask_to_string ( uint8 bid_ask_u )
/*
Function:   bid_ask_to_string
Author:     Neil Cossor
Modified:   20000515
Description:

    We have a numeric bid/ask value
    Convert this to a string i.e. readable form

Input Params:

    
Output Params:

    pointer to (static) string with the bid / ask in string form
    If it is an unknown type, then the string will be the code in numeric form
*/
{
    switch ( bid_ask_u )
    {
    case ASK:
    case BID:
	return bid_ask_s[bid_ask_u];
	break;
    default:
	return bid_ask_s[4];
    }	// switch on numeric value
}   // bid_ask_to_string

// ------------------------------------------------------------------------------------

char * order_type_to_string ( uint8 order_type_u )
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
{
    switch ( order_type_u )
    {
    case ORDER_TYPE_LIMIT:
	return order_type_s[0];
	break;
    case ORDER_TYPE_MARKET:
	return order_type_s[1];
	break;
    default:
	return order_type_s[2];
    }	// switch on numeric value
}   // order_type_to_string

// ------------------------------------------------------------------------------------

char * strike_price_group_to_string ( uint8 strike_price_group_u )
/*
Function:   strike_price_group_to_string
Author:     Neil Cossor
Modified:   9907xx
Description:

    We have a numeric strike price group value
    Convert this to a string i.e. readable form

Input Params:

    the strike price group (as per DFI)
    It should be one of
	Deep-in
	In
	At
	Out
	Deep-out

Output Params:

    pointer to (static) string with the strike price group in string form
    If it is an unknown type, then the string will be the code in numeric form
*/
{
    switch ( strike_price_group_u )
    {
    case STRIKE_PRICE_GROUP_DEEPIN:
	return strike_price_group_s[0];
	break;
    case STRIKE_PRICE_GROUP_IN:
	return strike_price_group_s[1];
	break;
    case STRIKE_PRICE_GROUP_AT:
	return strike_price_group_s[2];
	break;
    case STRIKE_PRICE_GROUP_OUT:
	return strike_price_group_s[3];
	break;
    case STRIKE_PRICE_GROUP_DEEPOUT:
	return strike_price_group_s[4];
	break;
    default:
	return strike_price_group_s[6]; // 5 is for wildcard input - not relevant to output
    }	// switch on numeric value
}   // strike_price_group_to_string

// ------------------------------------------------------------------------------------

uint8	cmd_strike_price_group ( char *strike_price_group_input_s,
				    uint8 default_strike_price_group_u )
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
{
    uint8   strike_price_group_u;
    int     choice_i;

    if ( strike_price_group_input_s == NULL )
	return default_strike_price_group_u;

    // customer specified a strike price group.. A[t] | DI[n] | DO[ut] | I[n] | O[ut] | *
    choice_i = select_string_option ( 6, &strike_price_group_s[0], strike_price_group_input_s );
    switch ( choice_i )
    {
    case strike_price_at_e :	    // At
	strike_price_group_u = STRIKE_PRICE_GROUP_AT;
	break;
    case strike_price_deepin_e :    // DIn
	strike_price_group_u = STRIKE_PRICE_GROUP_DEEPIN;
	break;
    case strike_price_deepout_e :   // DOut
	strike_price_group_u = STRIKE_PRICE_GROUP_DEEPOUT;
	break;
    case strike_price_in_e :	    // In
	strike_price_group_u = STRIKE_PRICE_GROUP_IN;
	break;
    case strike_price_out_e :	    // Out
	strike_price_group_u = STRIKE_PRICE_GROUP_OUT;
	break;
    case strike_price_all_e:	// wildcard, all
	strike_price_group_u = 0;
	break;
    default :
	printf ("Illegal Strike Price Group qualifier - A[t] | DI[n] | DO[ut] | I[n] | O[ut] | * \n");
	strike_price_group_u = UNDEFINED_UINT8;
    }	// switch on order type

    return strike_price_group_u;
}   // cmd_strike_price_group

// ------------------------------------------------------------------------------------

char * lock_type_to_string ( uint8 lock_type_u )
/*
Function:   lock_type_to_string
Author:     Neil Cossor
Modified:   20010411
Description:

    We have a numeric lock_type value
    Convert this to a string i.e. readable form

Input Params:

    
Output Params:

    pointer to (static) string with the lock type in string form
    If it is an unknown type, then the string will be the code in numeric form
*/
{
    static char *lock_type_s [] =
    {
	"CAB        ",
	"AwayBetter ",
	"Price??    ",
	"FARMM/Ticks",
	"Inversion  ",
	"Unknown    "
    };

    switch ( lock_type_u )
    {
    case LOCK_TYPE_CAB:
	return lock_type_s[0];
	break;
    case LOCK_TYPE_AWAY_BETTER:
	return lock_type_s[1];
	break;
    case LOCK_TYPE_GUARANTEE:
	return lock_type_s[2];
	break;
    case LOCK_TYPE_FARMM_TICKS:
	return lock_type_s[3];
	break;
    case LOCK_TYPE_INVERSION:
	return lock_type_s[4];
	break;
    default:
	return lock_type_s[5];
    }	// switch on numeric value
}   // lock_type_to_string

// ------------------------------------------------------------------------------------

char * reason_delete_to_string ( uint8 reason_delete_u )
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
{
    static char *reason_delete_s [] =
    {
	"Matching rotation round ended",
	"Too small volume",
	"Auto deleted by Marketplace",
	"Manual deleted by Exchange staff",
	"Released by PMM",
	"Quote crossed or locked",
	"Block/Facilitation ended",
	"Quote ticked-down to zero",
	"?? another Block/Facil ended",
	"Unknown delete reason"
    };

    switch ( reason_delete_u )
    {
    case REASON_DELETE_MATCH_ROTATION_ENDED:
	return reason_delete_s[0];
	break;
    case REASON_DELETE_TOO_SMALL_VOLUME:
	return reason_delete_s[1];
	break;
    case REASON_DELETE_AUTO_MARKETPLACE:
	return reason_delete_s[2];
	break;
    case REASON_DELETE_MANUAL_STAFF:
	return reason_delete_s[3];
	break;
    case REASON_DELETE_PMM_RELEASED:
	return reason_delete_s[4];
	break;
    case REASON_DELETE_QUOTE_CROSS_LOCKED:
	return reason_delete_s[5];
	break;
    case REASON_DELETE_BLOCK_FACIL_ENDED:
	return reason_delete_s[6];
	break;
    case REASON_DELETE_QUOTE_TICKED_ZERO:
	return reason_delete_s[7];
	break;
    case REASON_DELETE_BLOCK_FACIL_ENDED_2:
	return reason_delete_s[8];
	break;
    default:
	return reason_delete_s[9];
    }	// switch on numeric value
}   // reason_delete_to_string

// ------------------------------------------------------------------------------------

char * order_status_to_string ( int32 order_status_u )
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
{
    static char *order_status_s [] =
    {
	"no part in orderbook, no part closed",
	"no part in orderbook, all matched",
	"part matched, no part left in orderbook",
	"whole order/quote entered",
	"part matched, part left in orderbook",
	"whole order entered, but series not in regular mode",
	"Far away better, wait (with PMM)",
	"FARMM order, wait (with PMM)",
	"whole market order converted to limit",
	"market order partially matched, remainder converted to limit order",
	"market order converted to block",
	"Unknown order status reason"
    };

    switch ( order_status_u )
    {
    case ORDER_STATUS_NO_PART:
	return order_status_s[0];
	break;
    case ORDER_STATUS_MATCH:
	return order_status_s[1];
	break;
    case ORDER_STATUS_MATCH_NO_PART:
	return order_status_s[2];
	break;
    case ORDER_STATUS_ENTERED:
	return order_status_s[3];
	break;
    case ORDER_STATUS_MATCH_PART:
	return order_status_s[4];
	break;
    case ORDER_STATUS_ENTERED_NOT_REGULAR:
	return order_status_s[5];
	break;
    case ORDER_STATUS_FAR_AWAY_BETTER:
	return order_status_s[6];
	break;
    case ORDER_STATUS_FARMM:
	return order_status_s[7];
	break;
    case ORDER_STATUS_TO_LIMIT:
	return order_status_s[8];
	break;
    case ORDER_STATUS_MATCH_TO_LIMIT:
	return order_status_s[9];
	break;
    case ORDER_STATUS_TO_BLOCK:
	return order_status_s[10];
	break;
    default:
	return order_status_s[11];
    }	// switch on numeric value
}   // order_status_to_string

// ---------------------------------------------------------------------------

void prompt_logged_on ( char *command_ps )
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
{
    printf ( "The %s command  is only available if you are logged on\n",
	     command_ps );
}   // prompt_logged_on

// ------------------------------------------------------------------------------------

char * open_close_to_string ( uint8 open_close_u )
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
{
    static char *open_close_s [] =
    {
	"Default o/c",
	"Open",
	"Close (net)",
	"Close (mand)",
	"Unknown open/close"
    };

    switch ( open_close_u )
    {
    case OPEN_CLOSE_REQUEST_DEFAULT:
	return open_close_s[0];
	break;
    case OPEN_CLOSE_REQUEST_OPEN:
	return open_close_s[1];
	break;
    case OPEN_CLOSE_REQUEST_CLOSE_NET:
	return open_close_s[2];
	break;
    case OPEN_CLOSE_REQUEST_CLOSE_MAND:
	return open_close_s[3];
	break;
    default:
	return open_close_s[4];
    }	// switch on numeric value
}   // open_close_to_string

// ------------------------------------------------------------------------------------

char * away_market_status_to_string ( uint8 away_market_status_u )
/*
Function:   away_market_status_to_string
Author:     Neil Cossor
Modified:   000601
Description:

    We have a numeric away_market_status value
    Convert this to a string i.e. readable form

Input Params:

    
Output Params:

    pointer to (static) string with the corresponding value in string form
    If it is an unknown type, then the string will be the code in numeric form
*/
{
    static char *away_market_status_s [] =
    {
	"Regular",
	"Fast MKt",
	"Rotation",
	"Halted",
	"Auto Exec",
	"Inactive",
	"Unknown"
    };

    switch ( away_market_status_u )
    {
    case AWAY_MKT_STS_REGULAR:
	return away_market_status_s[0];
	break;
    case AWAY_MKT_STS_FAST_MARKET:
	return away_market_status_s[1];
	break;
    case AWAY_MKT_STS_ROTATION:
	return away_market_status_s[2];
	break;
    case AWAY_MKT_STS_HALTED:
	return away_market_status_s[3];
	break;
    case AWAY_MKT_STS_AUTO_EXEC:
	return away_market_status_s[4];
	break;
    case AWAY_MKT_STS_INACTIVE:
	return away_market_status_s[5];
	break;
    default:
	return away_market_status_s[6];
    }	// switch on numeric value
}   // away_market_status_to_string

// ------------------------------------------------------------------------------------

char * underlying_type_to_string ( uint8 underlying_type_u )
/*
Function:   underlying_type_to_string
Author:     Neil Cossor
Modified:   000601
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
{
    static char *underlying_type_s [] =
    {
	"Equity",
	"Currency",
	"Int. Rate",
	"Energy",
	"Soft&Agric",
	"Metal",
	"Equity Index",
	"Currency Index",
	"Int. Rate Index",
	"Energy Index",
	"Soft&Agric Index",
	"Metal Index",
	"Unknown"
    };

    switch ( underlying_type_u )
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
	return underlying_type_s [ underlying_type_u - 1];
	break;
    default:
	return underlying_type_s[12];
    }	// switch on numeric value
}   // underlying_type_to_string

// ------------------------------------------------------------------------------------

char * change_type_to_string ( uint16 change_type_u )
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
{
    static char *change_type_s [] =
    {
	"Add",
	"Delete",
	"Modify",
	"Unk change"
    };

    switch ( change_type_u )
    {
    case CHANGE_TYPE_ADD:
	return change_type_s[0];
	break;
    case CHANGE_TYPE_DELETE:
	return change_type_s[1];
	break;
    case CHANGE_TYPE_MODIFY:
	return change_type_s[2];
	break;
    default:
	return change_type_s[3];
    }	// switch on numeric value
}   // change_type_to_string

// ------------------------------------------------------------------------------------

char * change_reason_to_string ( uint16 change_type_u )
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
{
    static char *change_reason_s [] =
    {
	"Delete",
	"Stop",
	"Match",
	"Inactivate",
	"Modify",
	"Add",
	"Unk change"
    };

    switch ( change_type_u )
    {
    case CHANGE_REASON_DELETE:
    case CHANGE_REASON_STOP:
    case CHANGE_REASON_MATCH:
    case CHANGE_REASON_INACTIVATE:
    case CHANGE_REASON_MODIFY:
    case CHANGE_REASON_ADD:
	return change_reason_s[change_type_u - 1];
    default:
	return change_reason_s[3];
    }	// switch on numeric value
}   // change_reason_to_string

// ------------------------------------------------------------------------------------

char * trade_type_to_string ( uint8 trade_type_u )
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

    pointer to (static) string with the trade_type in string form
    If it is an unknown type, then the string will be the code in numeric form
*/
{
    // as we only receive these (never generate them), just put the constants in this
    // decode routine
    static char *trade_type_s [] =
    {
	"Standard",	// 1
	"Transitory",	// 2
	"Overtaking",	// 3
	"Reversing",	// 4
	"Transfer",	// 5
	"Exercise",	// 6
	"Assign",	// 7
	"Closing",	// 8
	"Issue",	// 9
	"New contract",	// 10
	"Unk trade type "
    };

    switch ( trade_type_u )
    {
    case 1: case 2: case 3: case 4: case 5:
    case 6: case 7: case 8: case 9: case 10:
	return trade_type_s[trade_type_u-1];
	break;
    default:
	return trade_type_s[10];
    }	// switch on numeric value
}   // trade_type_to_string

// ------------------------------------------------------------------------------------

char * trade_state_to_string ( uint8 trade_state_u )
/*
Function:   trade_state_to_string
Author:     Neil Cossor
Modified:   20000201
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
{
    // as we only receive these (never generate them), just put the constants in this
    // decode routine
    static char *trade_state_s [] =
    {
	"Active",	// 1
	"Rectified",	// 2
	"Unk trade state "
    };

    switch ( trade_state_u )
    {
    case 1: case 2:
	return trade_state_s[trade_state_u-1];
	break;
    default:
	return trade_state_s[2];
    }	// switch on numeric value
}   // trade_state_to_string

// ------------------------------------------------------------------------------------

char * reason_tick_to_string ( uint8 tick_type_u )
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
{
#define TICK_REASON_TRADEALL	    1	    // traded whole qty at this price, normal tick (just after trade)
#define TICK_REASON_NOFIRMFARMM	    2	    // insufficient qty for firm/farm trade at this price
#define TICK_REASON_LESSBBO	    3	    // insufficient qty remaining to be at BBO (partial trade of qty)
#define TICK_REASON_SPEEDBUMP	    4	    // MM speedbump kicked in.. too many contracts in time period, BBO widened

    static char *tick_reason_s [] =
    {
	"All traded",
	"no Firm/FARMM qty",
	"qty < min for BBO",
	"Speedbump",
	"Unk tick reason"
    };

    switch ( tick_type_u )
    {
    case TICK_REASON_TRADEALL:
    case TICK_REASON_NOFIRMFARMM:
    case TICK_REASON_LESSBBO:
    case TICK_REASON_SPEEDBUMP:
	return tick_reason_s[tick_type_u - 1];
    default:
	return tick_reason_s[4];
    }	// switch on numeric value
}   // reason_tick_to_string

// ---------------------------------------------------------------------------

void lock_global ( click_details_xt *click_px )
/*
Function:   lock_global
Author:	    Neil Cossor
Modified:   990914
Description:

    We wish to lock the global. We are either doing an update, or we are doing
    read access that requires a consistent common.
*/
{
    ISE_MUTEX_LOCK ( click_px->global_lock_mutex );
}   // lock_global

// ---------------------------------------------------------------------------

void unlock_global ( click_details_xt *click_px )
/*
Function:   unlock_global
Author:	    Neil Cossor
Modified:   990914
Description:

    We wish to unlock the global. We are have finished doing our update or read.
    The global must have previously been locked with lock_global.
*/
{
    ISE_MUTEX_UNLOCK ( click_px->global_lock_mutex );
}   // unlock_global

// --------------------------------------------------------------------------

void not_yet_implemented ( char *extra_text_s)
/*
Function : not_yet_implemented
Author	 : Neil Cossor
Modified : 991204
Description:

    the operator / script called a command which we have defined, but not
    yet written
*/
{
    printf_time();
    if (extra_text_s != NULL)
	printf ("%s not yet implemented\n",
		(extra_text_s)
		);
    else
	printf ("Not yet implemented\n");
}   // not_yet_implemented

// ---------------------------------------------------------------------------

int32 subscr_list_bin ( click_details_xt * click_px,
			ise_underlying_list_xt * ise_subscr_list_px,
			uint8 bin_u )
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
{
    // pointer to one (chained) node, in the known_bins_q
    gen_buf_node_xt	    *bin_node_px = NULL;
    // pointer to the data component of the node in the known_bins_q
    ise_bin_xt		    *bin_data_px = NULL;
    // Q'able node, which contains a msg_p pointer to an underlying, which is linked into the bin
    gen_buf_node_xt	    *bin_under_node_px = NULL;
    // and pointer to actual underlying data
    ise_underlying_xt	    *under_data_px;

    // lets see if we know about this bin ?
    bin_node_px = find_bin ( &click_px->our_market_x,
				bin_u );

    // if we now aren't pointing to a bin block, then we must create one
    if ( !bin_node_px )
    {
	printf ("Unknown bin %d\n",
		    bin_u );
	return INVALID_ARGUMENTS;
    }

    // we found the bin - pack in the interesting stuff
    bin_data_px = (ise_bin_xt *)bin_node_px->msg_p;

    // now, display the underlying in the bin - point to the first
    bin_under_node_px = (gen_buf_node_xt *)bin_data_px->bin_under_hdr_x.chain.next;
    while ( bin_under_node_px )
    {
	// is there room for another underlying / commodity in the list ?
	if ( ise_subscr_list_px->underlying_count_u >= MAX_UNDERLYING_PER_LIST )
	{
	    printf ("%bin expansion overflows max # of entries (%u) per list in one command\n",
			(uint32) MAX_UNDERLYING_PER_LIST
		    );
	    return INVALID_ARGUMENTS;
	}

	// point to the underlying
	under_data_px = (ise_underlying_xt *)bin_under_node_px->msg_p;

	// OK.. add this underlying / commodity into the list.
	ise_subscr_list_px->commodity_code_u [ise_subscr_list_px->underlying_count_u]
	    = under_data_px->commodity_code_u;

	// try for the next underlying in the bin
	bin_under_node_px = (gen_buf_node_xt *)bin_under_node_px->chain.next;
	// increase the count of commodities in the list
	ise_subscr_list_px->underlying_count_u++;
    }	// while we have a underlying node in the bin's chain

    return SUCCESSFUL;
}   // subscr_list_bin

// ---------------------------------------------------------------------------

char * market_status_to_string ( uint8 trading_status_u )
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
{
    static char *trading_status_s [] =
    {
	"undefined (0)",
	"Open",
	"Closed",
	"Locked",
	"Pre-Open",
	"Opening Allocation",
	"Automatic Delay",
	"Manual Delay",
	"Unknown"	// space reserved to put string form of number value, for unknown case
    };

    switch (trading_status_u)
    {
    case 1: case 2: case 3: case 4: case 5:
    case 6: case 7:	    case 0:		// include zero as a special
	    return trading_status_s[trading_status_u];
	    break;
    default:
	    return trading_status_s[8];
    }
}   // market_status_to_string

// ---------------------------------------------------------------------------

uint32 fract_from_double_with_round (  double  input_d,
					uint32  decimals_u,
					int8    round_i)
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
{
    int8            divisor_i;
    uint32          whole_i;
    double          multiplier_d,
                    fraction_d,
                    rounded_d;

    // if the value to be converted is 3 or more, use 8ths, otherwise use 16ths
    divisor_i =     input_d < 3 ? 16 : 8;
    multiplier_d =  1 / (double)divisor_i;

    // split the fraction (decimal) from the whole number
    fraction_d =    input_d-(int32)input_d;
    whole_i =       (int)input_d;
    
    // round the number to the nearest fraction
    rounded_d =     (double)whole_i + 
                    ((int32)(fraction_d * divisor_i) + round_i) * multiplier_d;

    // convert the number to a uint with implied decimal
    return (uint32)((double)dividers_i [ decimals_u ] * rounded_d);
} // fract_from_double_with_round

// ---------------------------------------------------------------------------

uint32 decimal_from_double_with_round ( double  input_d,
					instrument_class_xt * ise_instr_class_px,
					int8    round_i)
/*
Function:   decimal_from_double_with_round 
Author:	    Neil Cossor
Modified:   20010412
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
{
    int8            divisor_i;
    uint32          whole_i;
    double          multiplier_d,
                    fraction_d,
                    rounded_d;
    unsigned	    tick_item_u;    // used to look up our tick boundaries, to work our rounding

    // need to look up the nearest tick-slot, in the instrument class

    /* however, as the double-precision input_d might actually fit between the upper limit of one,
	and the lower limit of the next, need to limit precision of input # first.
	For example, if input_d == 2.9999x (x>0), then on a 0 -> 2.9999 and 3.0000 -> 99999 etc,
	we don't find a match, and end up using a tick_size_x[] which doesn't contain data

	So.. multiply up by 10^(# decimals), truncate (discard trailing digits),
	and divide back again by 10^(# decimals). Really this should round (not truncate),
	but unless the # digits is really small, then it should be insignificant c.f. the tick
	size.
    */
    input_d = (double)((int)(input_d * dividers_i [ ise_instr_class_px->dec_in_premium_u ] ))
		/  dividers_i [ ise_instr_class_px->dec_in_premium_u ];
    // this could actually be a little trickier, in that depending on our round up or down
    for ( tick_item_u = 0;
	    tick_item_u < ise_instr_class_px->tick_count_u;
	    tick_item_u++ )
    {
	// if this the correct price slot.. our price is >= lower, and <= upper
	// NB each boundary is always (much) wider than one tick
	if (( input_d >= (double)ise_instr_class_px->tick_size_x [tick_item_u].lower_limit_i / dividers_i [ ise_instr_class_px->dec_in_premium_u ] )
	    && ( input_d <= (double)ise_instr_class_px->tick_size_x [tick_item_u].upper_limit_i / dividers_i [ ise_instr_class_px->dec_in_premium_u ]))
	{
	    // we have our match
	    break;
	}
    }	// loop, looking for correct tick slot


    // I presume that our tick will be an integer divisor of 100.. penny, nickel, dim, quarter
    divisor_i =     10000 / ise_instr_class_px->tick_size_x [tick_item_u].step_size_i;
    multiplier_d =  1 / (double)divisor_i;

    // split the fraction (decimal) from the whole number NB could do weird things if -ve premium
    fraction_d =    input_d-(int32)input_d;
    whole_i =       (int)input_d;
    
    // round the number to the nearest fraction
    rounded_d =     (double)whole_i + 
                    ((int32)(fraction_d * divisor_i) + round_i) * multiplier_d;

    // convert the number to a uint with implied decimal
    return (uint32)((double)dividers_i [ ise_instr_class_px->dec_in_premium_u ] * rounded_d);
} // decimal_from_double_with_round

// ---------------------------------------------------------------------------

double double_from_whole_decimal (  uint32  input_u,
                                    uint32  decimals_u )
/*
Function:   double_from_whole_decimal
Author:     Scott Collins
Modified:   000122
Description:

    Convert a uint with an implied decimal to a double

Input Params:

    Value as uint
    Number of implied decimals

Output Params:

    Returns the resultant double
*/
{
    return ((double) input_u / dividers_i [ decimals_u ]);
} // double_from_whole_decimal

// ---------------------------------------------------------------------------

double double_days_from_now_to_date_xt ( date_xt *date_px )
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
{
    time_t  from_t,
            to_t;

    struct tm to_x;

    // get the current date/time
    from_t = time(NULL);

    // copy the current date/time to the 'to' time
    memcpy (&to_x, localtime(&from_t), sizeof(to_x));

    // overlay the day, month and year of the 'to' with the values passed in
    // NB C time starts from 1900, counts months from 0 = Jan, but days from 1-31
    to_x.tm_mday =  date_px->day_u;
    to_x.tm_mon =   date_px->month_u - 1;
    to_x.tm_year =  date_px->year_u - 1900;

    // convert the 'to' time to a time_t for time calculation
    to_t = mktime(&to_x);

    // compute the difference in times in days (divide by seconds in a day)
    return ((double)(difftime (to_t, from_t) / SECONDS_IN_A_DAY ));
} // double_days_from_now_to_date_xt

// ---------------------------------------------------------------------------

char * series_wildcard_to_string ( click_details_xt *click_px,
				    ise_series_xt * ise_series_px )
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

    Can we have
    - an underlying :- a specific commodity code .. probably no country / market
    - an expiration :- country, market, instr group, commodity, expiry.. but no strike price
*/
{
    // static area into which we can write some interim output, and return the
    // address of to the caller. Potential problem if multiple threads call the code
    // but this is unlikely. A hack.. I know.. but useful
    static char out_buffer_s [60];


    // have to work our way through the possibilities

    // ignore the modifier.. shouldn't be specified unless a specific series anyway

    // if no date, we can't be specifying a series
    if (( ise_series_px->expiry_date_x.day_u == 0 )
	&& ( ise_series_px->expiry_date_x.month_u == 0 )
	&& ( ise_series_px->expiry_date_x.year_u == 0 ))
    {
	// OK.. no date..see if instr class
	if ( ise_series_px->underlying_code_u == 0 )
	{
	    // no commodity.. see if we have an instrument group
	    if ( ise_series_px->instrument_group_u == 0 )
	    {
		// we hopefully have a country and market..
		sprintf ( out_buffer_s, "market [%s]",
			    market_name ( click_px,
					    ise_series_px->country_u,
					    ise_series_px->market_u )
			);

		return (char *)&out_buffer_s;
	    }
	    else
	    {
		// we should have an instrument type
		sprintf ( out_buffer_s, "instr type %s",
			    instr_group_name ( click_px,
						ise_series_px->instrument_group_u )
			);

		return (char *)&out_buffer_s;
	    }	// test for instr group
	}
	else
	{
	    // have a commodity code.. should be a instr class
	    instrument_class_xt *ise_instr_class_px;

	    ise_instr_class_px = find_instrument_class_by_series ( &click_px->our_market_x,
								    ise_series_px );

	    sprintf ( out_buffer_s, "instr class %s %s",
			commodity_code_to_string ( click_px,
						    ise_instr_class_px->series_x.underlying_code_u ),
			instr_group_name ( click_px,
					    ise_instr_class_px->series_x.instrument_group_u )
		    );

	    return (char *)&out_buffer_s;
	}   // test for if we have commodity code
    }
    else
    {
	// have a date - should be a series. What about an expiry (no strike) ?
	ise_series_info_xt *ise_series_info_px;

	ise_series_info_px = find_series_by_ise_series ( click_px,
							    ise_series_px );

	return (char *)&ise_series_info_px->ise_name_s;
    }	// test if we have expiry date
}

// ---------------------------------------------------------------------------

#ifndef WIN32
    /* the OM Solaris libraries (v15.7) have not yet been delivered. Accordingly,
	the swapint64 is provided as a temporary solution by ISE
	for Solaris must still be used, until the new link libraries
	are available.
    */
int64 swapint64 ( uint64 source_u )
/*
    swaps the eight bytes of source_q, and returns them to the caller.

    NB it doesn't really matter whether the source data is signed or not.
*/
{
    // tmp storage for return value
    uint64   tmp_destination_u;
    // tmp pointers to tmp 8-byte storage
    void *  destination_pq;
    void *  source_pq;
    int	    loop_i;

    // point to first byte of source data
    source_pq = &source_u;
    // point to last byte of (tmp) destination data
    destination_pq = (void *)((unsigned)(&tmp_destination_u) + 7);

    for (loop_i = 0;
	    loop_i < 8;
	    loop_i++ )
    {
	memcpy ( (char *)destination_pq, (char *)source_pq, 1 );
	(unsigned)destination_pq -= 1;
	(unsigned)source_pq += 1;
    }

    // return the byteswapped data
    return tmp_destination_u;
}   // swapint64




#endif