// ProvidersCollection.h: interface for the CProvidersCollection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROVIDERSCOLLECTION_H__528DDF68_9A6B_4690_B2DF_58C1C5685669__INCLUDED_)
#define AFX_PROVIDERSCOLLECTION_H__528DDF68_9A6B_4690_B2DF_58C1C5685669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Providers.h"

class CProvidersCollection  
{
	
public:
	bool GetProgID(long lProviderID, ProviderRole role, _bstr_t& bsProgID);
	CProvidersCollection();
	virtual ~CProvidersCollection();

	bool     Initialize();

	
private:
	CLock				 m_csLock;
	bool                 m_bInialized;
	CComPtr<IProviders>  m_spProviders;

};

#endif // !defined(AFX_PROVIDERSCOLLECTION_H__528DDF68_9A6B_4690_B2DF_58C1C5685669__INCLUDED_)
