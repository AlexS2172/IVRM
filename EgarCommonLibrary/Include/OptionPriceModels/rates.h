#ifndef __OPM_RATES_H__
#define __OPM_RATES_H__

namespace OPM
{

const double cdDaysPerYear360 = 360.;
const double cdDaysPerYear365 = 365.;

double RateDiscToCont(double dDiscRate, unsigned int nDays);

}

#endif //__OPM_RATES_H__