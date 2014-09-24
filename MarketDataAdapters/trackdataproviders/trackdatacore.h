// TrackDataCore.h: interface for the TrackDataCore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACKDATACORE_H__ADCC285D_3B64_487E_BF7C_61C2CCE29090__INCLUDED_)
#define AFX_TRACKDATACORE_H__ADCC285D_3B64_487E_BF7C_61C2CCE29090__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RtCore.h"
#pragma warning( disable : 4290 )


class CTrackDataCore  
{
	typedef set<_bstr_t> SUBSSYMBOLS;
public:
	void CancelStocks();
	void CancelOptions();
	CTrackDataCore();
	virtual ~CTrackDataCore();

	void Connect(CBaseNotifier* pNotify);
	void Disconnect(CBaseNotifier* pNotify);


	RT_ERR RequestQuote(CBaseNotifier *pNotifier, 
						  RT_SYMBOL* pSymbol,
						  CComVariant& vtRequest,
						  bool bSubscribe);  // bSubscribe = true-Subscription, false-LastQuote
	RT_ERR RequestSymbol(CBaseNotifier *pNotifier, 
						  RT_SYMBOL* pSymbol,
						  CComVariant& vtRequest);

	RT_ERR RequestOptions(CBaseNotifier *pNotifier, 
							RT_SYMBOL* pUnderlyingSymbol,
							CComVariant& vtRequest);

	void UnsubscribeQuote(CBaseNotifier *pNotifier, RT_SYMBOL* pSymbol);
private:
	//-----------------------------------------------------------------------------------------------//
	void _Connect() throw(CTrackDataError);
	bool LoadProviderInfo();

	SUBSSYMBOLS					m_Subscribed;
	CComAutoCriticalSection     m_csSubscribed;


private:


	HANDLE		 m_hEventStop;


	_bstr_t CreateSymbolKey(const RT_SYMBOL* pSymbol)
	{
		if(!pSymbol) return L"_";
		_bstr_t bsName = _bstr_t(pSymbol->name);
		bsName+=_T("_");
		if(pSymbol->exchange)
			bsName+=pSymbol->exchange;

		CComBSTR bs = (BSTR)bsName;
		bs.ToUpper();
		bsName = (BSTR)bs;
			
		return bsName;
	}

	bool m_bConnected;

private:

	static CRTCore  m_Core;
};



#endif // !defined(AFX_TRACKDATACORE_H__ADCC285D_3B64_487E_BF7C_61C2CCE29090__INCLUDED_)
