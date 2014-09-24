#if !defined(AFX_COMMON_H_)
#define AFX_COMMON_H_
#include "HyperFeedProviders.h"
#include <EgLib/EgLibMisc.h>

#define DEF_PATH _T("SOFTWARE\\eGAR\\{279F8CEA-29DE-4b2a-B868-75BE88C7C41E}")
#define TIMEOUT_LOGON			60 //1 minute wating timeout

#ifdef _DEBUG
	#define TIMEOUT_TRACK		1*1000 
#else
	#define TIMEOUT_TRACK		2*1000 //2 seconds waiting while send next symbols
#endif

//----------------------------------------------------------------------------------------//
typedef EgLib::CComRecPtr<QuoteUpdateInfo, &LIBID_PRICEPROVIDERSLib >	_QuoteUpdateInfo;
typedef EgLib::CComRecPtr<QuoteUpdateParams, &LIBID_PRICEPROVIDERSLib >	_QuoteUpdateParams;
typedef EgLib::CComRecPtr<QuoteUpdateResult, &LIBID_PRICEPROVIDERSLib>	_QuoteUpdateResult;
typedef EgLib::CComRecPtr<StockResults, &LIBID_PRICEPROVIDERSLib>		_StockResults;
typedef EgLib::CComRecPtr<StockResultsEx, &LIBID_PRICEPROVIDERSLib>		_StockResultsEx;
typedef EgLib::CComRecPtr<StockInfo, &LIBID_PRICEPROVIDERSLib>			_StockInfo;
typedef EgLib::CComRecPtr<OptionResults, &LIBID_PRICEPROVIDERSLib>		_OptionResults;
typedef EgLib::CComRecPtr<OptionResultsEx, &LIBID_PRICEPROVIDERSLib>		_OptionResultsEx;
typedef EgLib::CComRecPtr<OptionInfo, &LIBID_PRICEPROVIDERSLib>		_OptionInfo;
typedef EgLib::CComRecPtr<StockParams, &LIBID_PRICEPROVIDERSLib>		_StockParams;
typedef EgLib::CComRecPtr<OptionParams, &LIBID_PRICEPROVIDERSLib>		_OptionParams;
typedef EgLib::CComRecPtr<FutureParams, &LIBID_PRICEPROVIDERSLib>		_FutureParams;
typedef EgLib::CComRecPtr<FutureRootParams, &LIBID_PRICEPROVIDERSLib>		_FutureRootParams;
typedef EgLib::CComRecPtr<FuturesOptionParams, &LIBID_PRICEPROVIDERSLib>	_FuturesOptionParams;
typedef EgLib::CComRecPtr<FutureResults, &LIBID_PRICEPROVIDERSLib>	_FutureResults;
typedef EgLib::CComRecPtr<FutureResultsEx, &LIBID_PRICEPROVIDERSLib>	_FutureResultsEx;
typedef EgLib::CComRecPtr<FuturesOptionResults, &LIBID_PRICEPROVIDERSLib>	_FuturesOptionResults;
typedef EgLib::CComRecPtr<FuturesOptionResultsEx, &LIBID_PRICEPROVIDERSLib>	_FuturesOptionResultsEx;
typedef EgLib::CComRecPtr<OptionForIvResult, &LIBID_HYPERFEEDPROVIDERSLib>	_OptionForIvResult;
typedef EgLib::CComRecPtr<StockForIvResult, &LIBID_HYPERFEEDPROVIDERSLib>	_StockForIvResult;
typedef EgLib::CComRecPtr<QuoteUpdateFullInfo, &LIBID_PRICEPROVIDERSLib >	_QuoteUpdateFullInfo;


template <typename TStrategy, class Base, bool bClearOutgoing = true>
class CStrategy:
	protected EgLib::CStrategyT<CStrategy, TStrategy>
{
public:
	CStrategy(Base* pBase): m_pBase(pBase){}

	virtual void OnData(TStrategy& Data)
	{
		if(m_pBase && !IsRemoveAll())
			m_pBase->OnData(Data);
	}

	void  Start()     
	{ 
		EgLib::CStrategyT<CStrategy, TStrategy>::Start();
	}
	void  Stop(DWORD dwTimeout = INFINITE )      
	{
		EgLib::CStrategyT<CStrategy, TStrategy>::Stop(true, dwTimeout);
	}
	void  RemoveAll() 
	{ 
		if(m_pBase)
			m_pBase->callOnRemoveAll();

		EgLib::CStrategyT<CStrategy, TStrategy>::RemoveAll(bClearOutgoing);
	}
	void  PushData(const TStrategy& Data)
	{
		EgLib::CStrategyT<CStrategy, TStrategy>::PushData(Data);
	}

	HRESULT OnStrategyInitialize()
	{
		HRESULT hr = E_FAIL;
		if(m_pBase)
			hr = m_pBase->OnStrategyInitialize();

		return hr;
	}

	void  OnStrategyUninitialize()
	{ 
		if(m_pBase)
			m_pBase->OnStrategyUninitialize();

	}

private:
	Base* m_pBase;
};

#endif