/*
    combos.c

    This program attemps to 'test' the OM combo-pricing mechanism,
    as supplied to the ISE as in 'text' in April 2001

    We expect the caller to specify a script file.
    This script file has basic commands which set up our 'environment',
    with things like the bid and ask prices in our 'orderbook',
    and whether there are customer orders at those BBOs.
    These are then 'stored' in our orderbook, to be used when we play
    with the actual combos.

    It also allows us to specify the series, buy/sell, and ratios
    of the legs of a combo (always in one underlying), and the overall
    premium and quantity of a combo, and the other parameters of
    the combo, such as the step-up-to-bbo, and step-up-odd-contract
    attributes of the combo.

    The purpose of this is
    - to cross-check OM's logic, that the pre-check logic and
	the calculate-price algorithms actually correctly intersect
    - to check if OM's logic of calculating the prices and quantities
	for the legs / ratios etc actually always comes up with the correct
	price (or nearest). We have some doubts about the price when
	there are ratios AND quantities in the combo, and when there
	are ratios and an odd-quantity
    - to be able to easily put in some pre-conditions, and find out what we
	should expect (presuming we don't find errors in the previous cases),
	which should be helpful for generating the ATPs for testing combos.

    We need to check for
    - standard 2 leg 1:1 contracts (e.g. price-spread and time-spread)
    - standard 3 and 4 leg 1:1 contracts
    - combinations with all legs < price-tick boundary, all > boundary, some each way
    - combinations with non-standard 'simple' ratios, like 2:1
    - combinations with non-standard 'odd' ratios, like 7:3
    - combinations with even and odd numbers (including 1) of contracts, including
	different flavours of step-up-to-bbo and step-up-odd-contract
    - combinations with narrow and wide market spreads
    - combinations with customer orders at the BBO in none, some, all legs
    - combinations with combo premium in low-tick (e.g. 5c, 15c), but some/all legs
	at high tick (e.g. 10c)
    - combinations with customer/MM combo being hit by customer/MM

    We might also need to check for some of
    - ? combinations with the away market better than ISE BBO. We should NOT
	be providing?
    - ? combinations with different tick tables (all penny, penny-nickel, dime-quarter) ?
    - ? combinations with hit-price not equal to existing combo-price, is we ask OM to do this.
	NB OM API messages / algorithm does NOT currently allow for this. I believe Greg
	thinks it should (hitting combo allowed to take a 'worse' price to get an
	execution) ?
    - ? combinations with different # deliverable equities per contract between legs e.g. some 100, some 200 ?
	If we are not allowing this, we should have IMP rules which rejects (standing) combos in such series.
	Or should we only allow combo trading where # deliverables is 100 ?

Calculating price for legs of a combo

Note :- get_ask and get_bid are used in the pre-check routine (part of step 3), 
and the calculation of price/volume (step 5). This is incorrect. As correctly noted 
in step 2, there are cases where we must NOT use the BBO from the book.
Having found the adjusted prices in step 2, they are then incorrectly ignored
in steps 3 and 5, and we go back to working off the actual orderbook BBO,
which ignores the rule about at customer only allowed in any leg, if at least
one leg executes at a price better than the BBO.

There are also some unstated assumptions by OM
- the tick-table for the Calls and Puts are the same (obvious, but not actually an
OM requirement for anything else)
- the tick-table is Nickel/Dime. There may be Penny trading at some time, 
with Penny across all premiums, but this does not seem to create any problems with the algorithm.
- The validation for the existing combo (COB) has already enforced the requirement
for all legs to be in series in the same underlying
- We have validated (somewhere) the premium for the COB and HIT combos against the tick-table.
- The existing combo (COB) has enforced the requirement that the contract size in
all of the instrument classes are the same


Issues about rounding

The problem arises when the closest valid tick is sought for the multiplied price.
The regular functions can not be used, since it would look in the appropriate range for the multiplied price. 
What is interesting here is the tick size in the range where the price for each regular contract will end up, 
since the information is used to find the closest valid tick that is equal or less than the multiplied price. 
Therefore the multiplied price is divided by the multiplier, the tick size in that range is found, and the 
multiplied price is truncated to the closest value that is a valid tick according to the tick size obtained above.

*/

#include <stdio.h>

// -------------------- global constants, types ------------------------------

#ifndef TRUE
#define TRUE (1 == 1)
#endif
#ifndef FALSE
#define FALSE (0 == 1)
#endif

// actually, we only deal with 4 legs, but C indexes arrays from 0,
// but OM's 'demo' code starts from 1
#define MAX_LEGS_IN_COMBO 5

// constant 'bad' status returned as a no-go, can't hit status
#define NOK	-1
// set global yes/no to FARMM combos
#define cdc_FarMM_allowed FALSE

// client categories
#define Cust	1
#define MM	2
#define FarMM	4

// validity times.. as we are implementing timers etc, we can just define unique #s for values
#define AoN	1
#define FoK	2
#define FaS	3

// buy/sell indicator values
#define BUY	1
#define SELL	2

typedef	unsigned char		uint8;
typedef	char			int8;
typedef	unsigned short		uint16;
typedef	short			int16;
typedef long			int32;
typedef unsigned long		uint32;

typedef struct {
    // a non-string date. Required for the extensive date-based operations in CLICK
    // 2 bytes for year, and 1 each for month & day - leaves us a neat total size
    uint16  year_u;	    // absolute, i.e. since 0AD, not some weird 19xx offset
    uint8   month_u;
    uint8   day_u;
} date_xt;

/* info about one series. As we only care about the orderbook at the top,
    and the values relevant to a combo in those series, we can combine
*/
typedef struct {    // ise_series_xt
    // strike price - don't know why CLICK has this as signed.
    int32   strike_price_i;
    // commodity. This is the CLICK number of the underlying e.g. 3 = IBM
    uint16  underlying_code_u;
    // expiry date, exploded from the bit fields of CLICK, and in true terms
    date_xt expiry_date_x;
    // country code e.g. 20 == USA/ISE. NB these aren't really country codes - see country_market_xt
    uint8   country_u;
    // market code e.g. 1 == Derivatives Market, within this country code
    uint8   market_u;
    // instrument group. This is a CLICK allocated #, e.g. 7 == American Put option
    uint8   instrument_group_u;
    // modifier count. This is the # of times the series has been altered e.g. splits / restructures.
    // starts out as 0 for all series
    uint8   modifier_count_u;
    // there is an invisible 2-byte filler added by C - make it explicit, so we know about it
    uint16  filler;
    // current BBO in 'orderbook'
    double  bid_price_d,
	    ask_price_d;
    // whether we have a customer order in each side in the orderbook
    // value TRUE means there are customer orders
    unsigned bid_customer,
		ask_customer;
    // buy/sell action
    unsigned action;
} ise_series_xt;



typedef struct combo {
    // premium desired.. must be on a tick, may be -ve
    double net_pri;
    // how many units .. this is times any ratio numbers in the legs
    unsigned quantity;
    // client category.. Cust, MM etc
    unsigned cli_cat;
    // are we prepared to step up an on odd contract.. FALSE value means no step
    unsigned step_up_flag;
    // are we prepared to step to the BBO.. FALSE value means no step
    unsigned step_up_to_BBO;
    // validity time. we will discard any 'time' portion of AoN/FaS values
    unsigned short t_valid;
    // how many legs.. max of 4
    unsigned number_of_legs;
    // the legs
    ise_series_xt series [MAX_LEGS_IN_COMBO];
    // multiplier ratio.. OM structures it this way, so we do the same to minimise recoding
    unsigned multipl [MAX_LEGS_IN_COMBO];
    // the buy-sell characteristics of the legs..
    unsigned action [MAX_LEGS_IN_COMBO];
} combo_t;

// -------------------- global vars -----------------------------------------

// we will store a bunch of 'orderbook' info about a few series
// vales set from external script file
ise_series_xt ise_series_x [4];

// we will build up a existing Combo On Book (aka COB) from a script
// having done this, when our script says to HIT it, we try to do so
combo_t COB;
// and the hit.. which implicitly picks up many things, like the premium
// and ratios and (reverse) buy/sell leg attributes from the COB
combo_t HIT;

// where we store the bid and ask from the orderbook. This may be modified if
// we have bid==0
double m_ask [MAX_LEGS_IN_COMBO],
	m_bid [MAX_LEGS_IN_COMBO];

// where we store the high and low prices we will execute for each leg
double l_pri [MAX_LEGS_IN_COMBO],
	h_pri [MAX_LEGS_IN_COMBO];

// where we store the quantities we will execute (per combo unit) at each leg
// at the prices in l_pri and h_pri
unsigned lp_vol [MAX_LEGS_IN_COMBO],
	hp_vol [MAX_LEGS_IN_COMBO];

// flag value from precheck, which is used in one part of the actual price-calculation
int odd_result = 0;

// our 'sorted' indices, so when we sort the legs, we actually sort the indices
// and not shuffle all of the data for the legs
unsigned ind [MAX_LEGS_IN_COMBO];

// ---------------------------------------------------------------------------

double double_abs ( double premium_d )
/*
return absolute value of given number
*/
{
    if (premium_d < 0)
	return -premium_d;
    else
	return premium_d;
}   // double_abs

// ---------------------------------------------------------------------------

int sgn ( double number_d )
/*
return the sign of the given number.. -1, +1.. guess we return 0 as +ve
*/
{
    if ( number_d < 0 )
	return -1;
    else
	return +1;
}   // sgn

// ---------------------------------------------------------------------------

int round ( double number_d )
/*
return the nearest rounded integer (NOT truncated)
*/
{
}   // round

// ---------------------------------------------------------------------------

double get_tick_size ( double price_at_tick_d )
/*
Function:   get_tick_size
Author:	    Neil Cossor
Modified:   20010416
Description:

    Pretends to do the lookup in the (in-memory copy of the) CDB tick-table
    for the tick size (floating point, in dollars) at the nominated price.

    For the moment, I will assume we are ND at $US 3 boundary.
    Need to make this more flexible to be able to set other tables which might
    be traded, by underlying e.g. set Penny (likely), or Nickel/Quarter (unlikely)
*/
{
    // total hack, but accurate for current market
    if (price_at_tick_d < (double)3.0)
	return (double) 0.05;
    else
	return (double) 0.10;
}   // get_tick_size

// ---------------------------------------------------------------------------

double get_ask ( ise_series_xt series_x )
/*
Function:   get_ask
Author:	    Neil Cossor
Modified:   20010416
Description:

    Go look up the ask price at the nominated series from the orderbook.
    In this 'simulated' market, this means returning the ask-price
    as set up in our script file.
*/
{
    return series_x.ask_price_d;
}   // get_ask

// ---------------------------------------------------------------------------

double get_bid ( ise_series_xt series_x )
/*
Function:   get_bid
Author:	    Neil Cossor
Modified:   20010416
Description:

    Go look up the ask price at the nominated series from the orderbook.
    In this 'simulated' market, this means returning the ask-price
    as set up in our script file.
*/
{
    return series_x.bid_price_d;
}   // get_ask

// ---------------------------------------------------------------------------

unsigned exist_cust_orders (ise_series_xt series_x, unsigned buy_sell_u, double price_d)
/*
Function:   exist_cust_orders
Author:	    Neil Cossor
Modified:   20010417
Description:

    We wish to emulate the real system, to look up the orderbook on one side,
    at a particular price, whether there are customer orders.

    In our 'fake' orderbook, we only know price/customer flag at the actual BBO
    so we can ignore the price parameter
*/
{
    if ( buy_sell_u == BUY )
	return series_x.bid_customer;
    else
	return series_x.ask_customer;
}   // exist_cust_orders

// ---------------------------------------------------------------------------

long precheck_quantity ( combo_t COB,
			    combo_t HIT )
/*
Function: Pre-check quantity that can be executed
This function is used for two purposes. One is to give a quick answer to how large volume that 
can be traded between the two parties. The response should also answer the question of which party 
should loose the odd tick when applicable, i.e. indicating how to price the odd contract.

In parameters:
- Combo order on the book, called COB.
- Hitting order (or execute transaction), called HIT
- BBO[i]

Result out:
? Status:
-3 = Order on the book benefits on odd contract (without sacrifice for the hitter).
-2 = Order on the book benefits on odd contract (with sacrifice for the hitter).
-1 = Nothing can be executed.
0 = Order executable in full without sacrifice at unique price.
1 = All but one contract executable.
2 = Hitting order benefits an odd contract (with sacrifice for the book). // can’t occur
3 = Hitting order benefits an odd contract (without sacrifice for the book).
Note that the value 2 if only for future use (possibly stage 2).
*/
{
    double leg_tick_size = 0;
    double max_tick_size = 0;
    unsigned i;
    for ( i=0; i< COB.number_of_legs; i++ )
    {
	// NC why at ask-1tick ? Can’t we trade exactly at the adjusted ask ?
	leg_tick_size = get_tick_size( get_ask (COB.series[i]) /*-1tick*/ );

	if (leg_tick_size == get_tick_size(0))
	    // get_tick_size(0) is assumed to be the tick size for combos
	    // Any leg has the same tick size as combos within its 
	    // whole spread (i.e. within BBO)
	    return 0;
	else if (leg_tick_size > max_tick_size)
	    // save new 'largest' tick we found
	    max_tick_size = leg_tick_size;
    }	// for loop


    // If we reach this part, max_tick_size should be 2*get_tick_size(0)
    // not true if Penny-Nickel trading is ever introduced
    // or a Dime-Quarter boundary.. 2.5:1 boundary
    if (COB.net_pri / max_tick_size == 0)
	// If the net price is a valid tick using the largest tick size
	return 0;
    if (HIT.quantity % 2 == 0)
	// Even number of combo contracts are to be executed in every leg
	return 0;
    // We have a situation where it will be necessary to split one leg
    if ((COB.cli_cat==Cust) && (HIT.cli_cat==MM))
	return -3;
    if ((COB.cli_cat=MM )&& (HIT.cli_cat==Cust))
	return 3;
    if (HIT.step_up_flag==TRUE)
	return -2;
    // no symmetric +2.. COB.step_up_flag ignored (or rejected at entry?)

    // We have a situation where at most HIT.quantity-1 can be executed
    if ((COB.t_valid==AoN) || (HIT.t_valid==AoN) || (HIT.t_valid==FoK))
    {
	printf ("Reject: precheck says we can trade all but one of HIT, but AoN/FoK combination bars this\n");
	return -1;
    }
    else
    {
	printf ("precheck says we can trade all but one of HIT\n");
	return 1;
    }
}   // precheck_quantity

// ---------------------------------------------------------------------------

int basic_validity_check ( combo_t COB, combo_t HIT )
/*
First validity check in a HIT, called from main function
Result out:
- Status
- Trades taking place

1. General validation
This section may not be complete.
*/
{
    unsigned i;

    if ((COB.t_valid==AoN) && (HIT.quantity < COB.quantity))
    {
	printf ("All or None COB, with HIT quantity too small\n");
	return NOK;
    }
    if ((HIT.t_valid==FoK) && (HIT.quantity > COB.quantity))
    {
	printf ("Hit FOK with Hit quantity too big\n");
	return NOK;
    }
    if ((HIT.cli_cat==FarMM) && (!cdc_FarMM_allowed))
    {
	printf ("FarMMs not allowed\n");
	return NOK;
    }
//    if (COB.order_type==InterestOrder)
//	return NOK;
    if (HIT.t_valid==AoN || HIT.t_valid==FaS)
    {
	printf ("Hit All or None, Fill and Store are illegal\n");
	return NOK;
    }
/*
    presume for now, that our 'hit' script can specify a quantity, validity time, but
    as there is no need to qualify the open/close characteristics, there is no need to
    respecify the legs of the hit for anything.. so implicitly have the correct series

    if (COB.number_of_legs != HIT.number_of_legs)
    {
	printf ("Different # of legs\n");
	return NOK;
    }

    for (i=1; i<COB.number_of_legs; i++) // assuming legs have been sorted by series
    {
	if (COB.series[i] != HIT.series[i])
	    return NOK;
    }
*/


    /*
    Note:- this also assumes that the validation on a COB order checked that the series in all
    legs were in the same underlying, and that the multipliers were non-zero.

    Question : should ISE rules allow an executable COB order to have 1 leg ?
    2. Calculate modified BBO
    In some cases, one or both sides of the BBO are not an acceptable price to execute a
    particular leg. There are also some other special cases where the BBO to use in the 
    following computations is not the value to use.
    */
    for (i=1; i<COB.number_of_legs; i++ )
    {
	m_ask[i] = get_ask(COB.series[i]);
	m_bid[i] = get_bid(COB.series[i]);
	if (m_ask[i] == 0)
	{
	    printf ("No ask BBO in one leg\n");
	    return NOK;
	}
	// we have an ask, do we have a bid price of 0.. cant trade at 0
	if (m_bid[i] == 0)
	    // adjust bid to lowest tick we can trade at
	    m_bid[i] = get_tick_size (0.0);
	// this next bit looks wrong..!!!!! this is NOT the rule, which OM is now working on
	if (exist_cust_orders (COB.series[i], BUY, m_bid[i]))
	    // there is a customer order on the bid, raise the minimum bid for this series
	    m_bid[i] += get_tick_size ( m_bid[i] );
	if (exist_cust_orders (COB.series[i], SELL, m_ask[i]))
	    // there is a customer order on the bid, raise the minimum bid for this series
	    m_ask[i] -= get_tick_size ( m_ask[i] );
	if (m_bid[i] > m_ask[i])
	{
	    printf ("After calculating modified BBO, no executable spread in one leg\n");
	    return NOK;
	}
    }	// for loop, figuring out the modified BBO in the legs

    /*
    3. Pre-check odd contract cases
    In some cases an odd contract can not be executed at a valid tick. There are some rules that take of this in most situations, 
    although it can happen that the execution is rejected as a whole.
    */
    // what is the third parameter to precheck_quantity.. 
    // there doesn't appear to be anything there that needs more params ????
    odd_result = precheck_quantity (COB, HIT /*, BBO*/ );
    if (odd_result == -1)
    {
	printf ("after precheck, not executable\n");
	return NOK;
    }
    // Otherwise, keep this value for later use
}   // basic_validity_check

// ---------------------------------------------------------------------------

void swap_ind ( unsigned i, unsigned j )
/*
    We know we need to swap the indices for the legs in the series
*/
{
    unsigned tmp;

    tmp = ind[i];
    ind[i] = ind[j];
    ind[j] = tmp;
}   // swap_ind

// ---------------------------------------------------------------------------

void compare_swap ( unsigned i, unsigned j )
{
    // if the lower leg alread has a 0 spread, then leave it where it is
    if (m_ask[ind[i]] - m_bid[ind[i]] < 1e-6)
    {
	// no spread in lower item. If the second item has a spread, we can quit
	if (m_ask[ind[j]] - m_bid[ind[j]] >= 1e-6)
	{
	    // no swap, but we know we don't need secondary, tertiary comparisons
	    return;
	}
    }
    else
    {
	// OK, we have some spread in the lower item [i].. do we have 0 spread in [j]
	if (m_ask[ind[j]] - m_bid[ind[j]] <1e-6)
	{
	    // no spread in upper item, need to swap
	    swap_ind ( i, j );
	    return;
	}
	// else case is some spread in both legs.. maybe secondary and tertiary rules will cause swap
    }	// test if lower leg has any spread

    // OK, is we are here, we had both legs equivalent.. both have no spread, or both have some spread

    // secondary sort rule.. larger ticks first
    if ( get_tick_size ( m_ask[ind[i]] ) > get_tick_size ( m_ask [ind[j]] ))
    {
	// items already in correct order for tick size, bail out
	return;
    }
    else if ( get_tick_size ( m_ask[ind[i]] ) < get_tick_size ( m_ask [ind[j]] ))
    {
	// not in correct tick-size order, swap
	swap_ind ( i, j );
	return;
    }	// check if tick in lower > tick in upper item

    // OK, we still have equivalent series.. tertiary sort - calls before puts
    // NB Calls have instrument group value 6, and Puts have value 7, so numeric will do
    if (COB.series[ind[i]].instrument_group_u < COB.series[ind[j]].instrument_group_u )
    {
	// items already in correct order for tick size, bail out
	return;
    }
    else if (COB.series[ind[i]].instrument_group_u > COB.series[ind[j]].instrument_group_u )
    {
	// not in correct call v put order, swap
	swap_ind ( i, j );
	return;
    }	// check if can sort by call vs put

    // OK, sort by expiration date

    // how about sort by expiration
}

void sort_leg_indices ()
/*
Function:   sort_leg_indices
Author:	    Neil Cossor
Modified:   20010417
Description:

/*
4. Sort the legs temporarily
The legs should be temporarily sorted in a way that is required for the price calculation algorithm to function.

Sorting criteria:
- Primary: Legs with zero spread (m_ask - m_bid == 0) should be sorted first
- Secondary: Legs with large tick size should be sorted first
- Tertiary : 
    - Calls before puts
    - Early expiry before later expiration
    - Low strike before high strike

The result should be an index structure. E.g. ind[i] will give the index to the leg
that was temporarily sorted as number i.

Note, as we can have a max of 4 legs, the venerable old bubblesort is as good as any.
*/
{
    unsigned i,j;

    // first make the ind[] values point to the COB components in existing order
    for (i=1;i<COB.number_of_legs;i++)
	ind[i] = i;

    // now, start by sorting those legs with no spread to the stop
    for (i=1;i<COB.number_of_legs;i++)
    {
	for (j=i+1;j<COB.number_of_legs;j++)
	{
	    // sort this pair ind[i] and ind[j]
	    compare_swap ( i,j );

	}   // inner sort loop
    }	// outer sort loop
}   // sort_leg_indices

// ---------------------------------------------------------------------------

/*
*/
long calculate_prices ( double net_pri )
/*
5. Calculate prices/volumes

Function: calculate prices
Parameters in:
- net_pri

Result out:
- l_pri[1..4]	(lower alternative for the price of each leg)
- lp_vol[1..4]	(volume for each leg that should execute at lower price)
- h_pri[1..4]	(higher alternative for the price of each leg)
- hp_vol[1..4]	(volume for each leg that should execute at higher price)
Note that these are already sorted according to the index, i.e. l_pri[1..4] <->  leg[ind[1..4]]. 
Also note that hp_vol[i] can be zero for one or more legs.

Call the function that calculates the price and volume for each leg, see above. 
The result is stored in the structures listed below. Note that the sorting of the legs 
in these structures so that the index unsorting is not needed. Also note that some further
validation is done when calling this function
l_pri[1..4]		<-> leg[ind[1..4]]
lp_vol[1..4]		etc
h_pri[1..4]
hp_vol[1..4]

*/
{
    // where we store the bid and ask for orderbook for each leg
    double c_ask [MAX_LEGS_IN_COMBO],
	    c_bid [MAX_LEGS_IN_COMBO];
    // where we store the bid and ask for total combo, including all legs from this one on
    double cs_ask [MAX_LEGS_IN_COMBO],
	    cs_bid [MAX_LEGS_IN_COMBO];
    // temps, where we store our moving target premiums (for leg, and for combo)
    double tmp_net,
	    tmp_pri;
    // tick-size at the price where we are targetting
    double tick_size;

    unsigned i;
    // Perform some preporatory calculations
    cs_ask[COB.number_of_legs + 1] = 0;
    cs_bid[COB.number_of_legs + 1] = 0;
    // need to initialise all of cs_ask[] and cs_bid, not just [#legs+1]
    for ( i = COB.number_of_legs; i>0; i-- )
    {
	if (COB.action[ind[i]] == BUY)
	{
		c_ask[i] = get_ask(COB.series[ind[i]]);	// m_ask[]
		c_bid[i] = get_bid(COB.series[ind[i]]);	// m_bid[]
	}
	else
	{
		c_ask[i] = -get_bid(COB.series[ind[i]]);// m_bid[]
		c_bid[i] = -get_ask(COB.series[ind[i]]);// m_ask[]
	}
	c_ask [i] *=  COB.multipl[ind[i]];
	c_bid [i] *=  COB.multipl[ind[i]];
	cs_ask[i] = c_ask[i] + cs_ask[i+1];
	cs_bid[i] = c_bid[i] + cs_bid[i+1];
    }

    // cs_bid[1] and cs_ask[1] now contains the BBO for the combo

    // Validation of the entire combo is performed here (which is 
    // a little bit out of place, but we do it here anyway)
    tmp_net = COB.net_pri;
    if (tmp_net < cs_bid[1])
	    return NOK;
    if (tmp_net > cs_ask[1])
	if (HIT.step_up_to_BBO)
		tmp_net = cs_ask[1];
	else
	    return NOK;

    for (i = 1; i <= COB.number_of_legs; i++)
    {
	/* these (x-y)>1e-6 tests seem to be testing for equality
	    This should only be necessary if we are working in floating point
	    All OM prices are represented as a integer number with a implicit
	    decimal point, so this is a little strange..

	    Would this no-choice case be more obvious by rearranging 
	    the code slightly
	*/
	if (double_abs(cs_ask[i]-cs_bid[i]) > 1e-6)
	{
	    double percent = (tmp_net-cs_bid[i]) / (cs_ask[i]-cs_bid[i]);
	    tmp_pri = double_abs (percent*(c_ask[i]-c_bid[i]) + c_bid[i]);
	}
	else
	{
//	    percent = 0;
	    tmp_pri = c_bid[i];
	}

//	tmp_pri = double_abs (percent*(c_ask[i]-c_bid[i]) + c_bid[i])

	// For more details of the rationale, see “Issues about rounding”
	tick_size = get_tick_size (tmp_pri/COB.multipl[ind[i]]);
	tmp_pri = tick_size * round (tmp_pri/tick_size);
	tmp_pri /= COB.multipl[ind[i]];
//	l_pri[i] = get_next_lower_tick (tmp_pri);
	// Or use the same value if tmp_pri is a valid tick
	h_pri[i] = l_pri[i] + get_tick_size (l_pri[i]);
	hp_vol[i] = (tmp_pri-l_pri[i]) * COB.multipl[ind[i]] / tick_size;
	lp_vol[i] = COB.multipl[ind[i]] - hp_vol[i];
	if (COB.action[ind[i]] == BUY)
		tmp_net -= tmp_pri * COB.multipl[ind[i]];
	else
		tmp_net += tmp_pri * COB.multipl[ind[i]];
	// missing symmetry.. don’t care about l_pri[] unless vol > 0
	if ((l_pri[i] < get_bid(COB.series[ind[i]])) && (double_abs (lp_vol[i])>0))
		printf ("Selected price below BBO\n");
	if (h_pri[i]>get_ask(COB.series[ind[i]]) && double_abs(hp_vol[i])>0)
		printf ("Selected price above BBO\n");
	if (double_abs(lp_vol[i]*l_pri[i]+hp_vol[i]*h_pri[i] - tmp_pri) > 1e-6)
		printf ("Selected prices wrong\n");
    }	// for loop, calculating price on each leg

    // how did we do ?
    if (double_abs(tmp_net) > 1e-6)
	    printf ("Error in the end\n");
}   // calculate_prices

// ---------------------------------------------------------------------------

char get_opra_char (combo_t COB)
/*
6. Determine type of combination to report
When reporting to OPRA, they distinguish between two types of combinations, L and M.
Here this value is determined and stored to be used for each leg that is later reported.
*/
{
    unsigned i;
    char c_type = 104;	    // assume all one instrument type.. this equals 'L' char to OPRA
    for (i=1; i<COB.number_of_legs; i++ )
    {
	if (COB.series[i].instrument_group_u != COB.series[1].instrument_group_u)
	c_type = 105;	// found one in different instrument type.. this equals 'M' char to OPRA
    }

    return c_type;
}   // get_opra_char

// ---------------------------------------------------------------------------

void execute_trade ( ise_series_xt series_x, 
			double exec_price, 
			int exec_volume, 
			char opra_type )
/*
Function:   execute_trade
Author:	    Neil Cossor
Modified:   20010417
Description:

    We have just executed a leg of the combo

    Report what we just did
*/
{
/*
kashliuk
    printf ("Trade %d contracts at %f.2, series %s, OPRA char %c\n",
		exec_volume,
		exec_price,
		series_to_string ( series_x ),
		opra_type
	    );
*/
}   // execute_trade

// ---------------------------------------------------------------------------

long execute_trades ()
/*
7. Execute trades
With the calculated price and volume together with the case for odd contracts,
execute all the trades with appropriate price and volume.
*/
{
    unsigned i;
    // execution volume - integer
    int e_vol;
    // execution price
    double e_pri;
    // correction number for the low and hi prices for execution in each leg
    double lp_corr, hp_corr;

    // Number of combo contracts to execute
    unsigned ce_qty = HIT.quantity;
    if (odd_result == 1)
	    // ce_qty will now become an even number, which will ensure
	    // that the combo can executed despite 
	    ce_qty--;
    if (ce_qty == 0)
	    return NOK;

    for (i=1; i<COB.number_of_legs; i++ )
    {
	lp_corr = 0;
	hp_corr = 0;
	if ((double_abs(odd_result)>=2) && (i==COB.number_of_legs))	// NB odd_result == +2 can’t occur
	    // An odd contract must be dealt with (for the last leg)
	    lp_corr = sgn(odd_result) * 0,5;
	    if (COB.action[ind[i]] == BUY)
		    lp_corr = -lp_corr;
	    hp_corr = -lp_corr;

	// Execute current leg at the lower price
	e_pri = l_pri[i];
	e_vol = round (lp_vol[i] * ce_qty + lp_corr);
	// The value should be an integer value
	// Round is done just in case to avoid 1.99999999 -> 1
/*
Kashliuk
	if (e_vol != 0)
	    execute_trade (COB.series[ind[i]], e_pri, e_vol, c_type);
*/
	    // lp_vol[i] could be zero if HIT.quantity==1 and that contract
	    // was rounded to the higher price

	// Execute current leg at the higher price (if applicable)
	e_pri = h_pri[i];
	e_vol = round (hp_vol[i] * ce_qty + hp_corr);
	// Round is done just in case, see above

/*
Kashliuk

	if (e_vol != 0)
	    execute_trade (COB.series[ind[i]], e_pri, e_vol, c_type);
*/
    }

    /* ???? NC, what the hell does this mean.. whether there was attached text or not,
	Why wouldn't we report an executed combo to CIBOIS
    if (!is_empty_field(COB.message))
	printf ("report trade to CIBOIS\n");
    */

    if (COB.quantity == ce_qty)
    {
	// remove combo from OB - traded completely
	printf ( "traded whole standing combo, removed from combo book\n");
    }
    else
    {
	// partially traded standing quantity
	COB.quantity -= ce_qty;
	printf ("partially traded combo, quantity updated\n" );
    }

    if (HIT.quantity == ce_qty)
	printf ("whole orders was closed\n");
    else
	printf ("part closed, remainder cancelled\n");
}   // execute_trades
// ---------------------------------------------------------------------------
/*
int main ( int argc,
	    char *argv[ ] )
{
}   // main of combos.c
*/