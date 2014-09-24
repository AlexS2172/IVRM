#ifndef __PPREQUESTDATA_H__
#define __PPREQUESTDATA_H__

#include "PpPriceRequest.h"

namespace EODPP
{

class CPpRequestData 
{
public:
	CPpRequestData(void)
		: m_spReqs(new CPpPriceRequestKeySet),
		m_spUndReqs(new CPpPriceRequestKeySet),
		m_spUndGroupReqs(new CPpPriceRequestKeySet)
	{
	}

	virtual ~CPpRequestData(void)
	{
//		m_spUndReqs = NULL;
//		m_spUndGroupReqs = NULL;
//		m_spReqs = NULL;
	}

	CPpPriceRequestKeySetPtr Reqs()
	{
		return m_spReqs;
	}

	CPpPriceRequestKeySetPtr UndReqs()
	{
		return m_spUndReqs;
	}

	CPpPriceRequestKeySetPtr UndGroupReqs()
	{
		return m_spUndGroupReqs;
	}

protected:
	CPpPriceRequestKeySetPtr	m_spReqs; // collection of all requests
	CPpPriceRequestKeySetPtr	m_spUndReqs; // collection of all underlying requests references to m_spReqs
	CPpPriceRequestKeySetPtr	m_spUndGroupReqs; // collection of all underlying group requests
};

// object pointer
typedef boost::shared_ptr<CPpRequestData>		CPpRequestDataPtr;

};

#endif //__PPREQUESTDATA_H__
