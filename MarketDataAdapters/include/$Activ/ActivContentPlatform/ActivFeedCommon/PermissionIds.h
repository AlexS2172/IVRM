/**
 *	@file	PermissionIds.h
 *
 *	@brief	Header file for the permission ids.
 *
 *	$Log: $
 */

#if !defined(ACTIV_FEED_PERMISSION_IDS_H)
#define ACTIV_FEED_PERMISSION_IDS_H

#include "ActivMiddleware/Activ.h"

#include "ActivContentPlatform/ActivFeedCommon/ActivFeedDefinitions.h"

namespace Activ
{

namespace Feed
{

const PermissionId PERMISSION_ID_NONE = 0;
const PermissionId PERMISSION_ID_UNKNOWN = static_cast<PermissionId>(~0);

const PermissionId PERMISSION_ID_NASDAQ_LEVEL1 = 1;
const PermissionId PERMISSION_ID_NASDAQ_LEVEL2 = 2;
const PermissionId PERMISSION_ID_NASDAQ_INDEX = 3;
const PermissionId PERMISSION_ID_NASDAQ_ADF = 4;

const PermissionId PERMISSION_ID_NYSE = 5;
const PermissionId PERMISSION_ID_NYSE_INDEX = 6;

const PermissionId PERMISSION_ID_AMEX = 7;
const PermissionId PERMISSION_ID_AMEX_INDEX = 8;

const PermissionId PERMISSION_ID_OPRA_COMPOSITE = 9;
const PermissionId PERMISSION_ID_OPRA_FCO = 10;
const PermissionId PERMISSION_ID_OPRA_INDEX = 11;
const PermissionId PERMISSION_ID_OPRA_FOREX = 12;

const PermissionId PERMISSION_ID_CBOT = 13;
const PermissionId PERMISSION_ID_CBOT_MINI = 14;
const PermissionId PERMISSION_ID_CBOT_DJ_INDEX = 15;

const PermissionId PERMISSION_ID_KCBOT = 16;

const PermissionId PERMISSION_ID_NYMEX = 17;
const PermissionId PERMISSION_ID_COMEX = 18;

const PermissionId PERMISSION_ID_NYBOT = 19;

const PermissionId PERMISSION_ID_CORPORATE_ACTION = 20;
const PermissionId PERMISSION_ID_SECURITY = 21;
const PermissionId PERMISSION_ID_COMPANY = 22;

const PermissionId PERMISSION_ID_TSX_LEVEL1 = 23;
const PermissionId PERMISSION_ID_TSX_LEVEL2 = 24;
const PermissionId PERMISSION_ID_TSX_INDEX = 25;
const PermissionId PERMISSION_ID_TSX_VENTURE_LEVEL1 = 26;
const PermissionId PERMISSION_ID_TSX_VENTURE_LEVEL2 = 27;
const PermissionId PERMISSION_ID_TSX_VENTURE_INDEX = 28;

const PermissionId PERMISSION_ID_NASDAQ_MFDS = 29;

const PermissionId PERMISSION_ID_DJN = 30;

const PermissionId PERMISSION_ID_COMTEX = 31;

const PermissionId PERMISSION_ID_CME = 32;
const PermissionId PERMISSION_ID_CME_E_MINI_LEVEL1 = 33;
const PermissionId PERMISSION_ID_CME_E_MINI_LEVEL2 = 34;
const PermissionId PERMISSION_ID_CME_GLOBEX_LEVEL1 = 35;
const PermissionId PERMISSION_ID_CME_GLOBEX_LEVEL2 = 36;
const PermissionId PERMISSION_ID_CME_SP_INDEX = 37;

const PermissionId PERMISSION_ID_INTERNAL = 38;

const PermissionId PERMISSION_ID_ARCA = 39;
const PermissionId PERMISSION_ID_BRUT = 40;
const PermissionId PERMISSION_ID_INET = 41;

const PermissionId PERMISSION_ID_OPRA_REGIONAL = 42;

const PermissionId PERMISSION_ID_MONTREAL = 43;

const PermissionId PERMISSION_ID_ONE_CHICAGO = 44;

const PermissionId PERMISSION_ID_TENFORE = 45;

const PermissionId PERMISSION_ID_CFE = 46;

const PermissionId PERMISSION_ID_COUNT = 47;

} // namespace Feed

} // namespace Activ

#endif // !defined(ACTIV_FEED_PERMISSION_IDS_H)
