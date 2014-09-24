#ifndef __PPPRICEREQUEST_H__
#define __PPPRICEREQUEST_H__

#include "EodGeneral.h"

namespace EODPP
{

/////////////////////////////////////////////////////////////////////////////
//
class CPpPriceRequestKey
{	
public:
	CPpPriceRequestKey() {}

	CPpPriceRequestKey(const PP::QuoteUpdateParams& aKey)
	{
		m_sSymbol = (LPCTSTR)_bstr_t(aKey.Symbol);
		m_enType = aKey.Type;
		m_sExchange = (LPCTSTR)_bstr_t(aKey.Exchange);
	}

	virtual ~CPpPriceRequestKey(void)
	{
	}

	void CopyTo(PP::QuoteUpdateParams& aKey)
	{
		aKey.Symbol = _bstr_t(m_sSymbol).Detach();
		aKey.Type = m_enType;
		aKey.Exchange = _bstr_t(m_sExchange).Detach();
	}

	bool operator < (const CPpPriceRequestKey& aKey) const
	{
		return m_enType < aKey.m_enType
			|| m_enType == aKey.m_enType && m_sSymbol < aKey.m_sSymbol
			|| m_enType == aKey.m_enType && m_sSymbol == aKey.m_sSymbol && m_sExchange < aKey.m_sExchange;
	}

	bool operator == (const CPpPriceRequestKey& aKey) const
	{
		return m_enType == aKey.m_enType && m_sSymbol == aKey.m_sSymbol && m_sExchange == aKey.m_sExchange;
	}	

public:
	CString m_sSymbol;
	PP::InstrumentTypeEnum m_enType;
	CString m_sExchange;
};

// collections
typedef std::set<CPpPriceRequestKey> CPpPriceRequestKeySet;
typedef boost::shared_ptr<CPpPriceRequestKeySet> CPpPriceRequestKeySetPtr;

};

#endif //__PPPRICEREQUEST_H__