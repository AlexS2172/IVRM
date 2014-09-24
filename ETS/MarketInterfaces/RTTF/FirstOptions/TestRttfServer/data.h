#ifndef RTTFDATA_H
#define RTTFDATA_H

#include "rttfserver.h"

#define TRADES_FILE		"trades.dat"

typedef vector<foc_rttf_trade>	trades_t;

extern trades_t	g_Trades;

BOOL _FUNC_ LoadTrades();

#endif