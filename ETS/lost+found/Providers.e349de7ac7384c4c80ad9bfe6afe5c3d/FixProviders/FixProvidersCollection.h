// FixProvidersCollection.h: interface for the CFixProvidersCollection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROVIDERSCOLLECTION_H__528DDF68_9A6B_4690_B2DF_58C1C5685669__INCLUDED_)
#define AFX_PROVIDERSCOLLECTION_H__528DDF68_9A6B_4690_B2DF_58C1C5685669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FixProviders.h"
#include <comdef.h>
#include <atlbase.h>

class CFixProvidersCollection
{

public:
	CFixProvidersCollection();
	virtual ~CFixProvidersCollection();

	bool Initialize();
	bool GetProgID(long lFixProviderID, _bstr_t& bsProgID);

private:
	CComAutoCriticalSection m_csLock;
	bool                    m_bInitialized;
	CComPtr<IFixProviders>  m_spFixProviders;
};

#endif // !defined(AFX_PROVIDERSCOLLECTION_H__528DDF68_9A6B_4690_B2DF_58C1C5685669__INCLUDED_)
