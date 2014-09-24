#ifndef __VPUNDVOLASOURCE_H__
#define __VPUNDVOLASOURCE_H__

#include "EodGeneral.h"
#include "VpVolaProvider.h"

namespace EODVP
{

/////////////////////////////////////////////////////////////////////////////
//
class CVpUndVolaSource :  public EODCO::CEgTraceable
{
private:
	friend class CVpVolaProvider;

public:
	CVpUndVolaSource(void);
	~CVpUndVolaSource(void);

	double Vola(double dUndPrice, DATE dtExpiry, double dStrike) const throw (exception);

protected:
	void Init(IVSSymbolVolatilityPtr spSymbolVola) throw (exception);

private:
	IVSSymbolVolatilityPtr		m_spSymbolVola;
};

// object pointer
typedef boost::shared_ptr<CVpUndVolaSource>		CVpUndVolaSourcePtr;

};

#endif //__VPUNDVOLASOURCE_H__