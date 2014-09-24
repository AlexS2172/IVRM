// MmTradeChannel.h : Declaration of the CMmTradeChannel

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"
#include "MmTradeInfoColl.h"
#include "MmTradeByUndColl.h"
#include "MmTradeMatchColl.h"

#include "_IMmTradeChannelEvents_CP.h"


// CMmTradeChannel

class ATL_NO_VTABLE CMmTradeChannel : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmTradeChannel, &CLSID_MmTradeChannel>,
	public ISupportErrorInfoImpl<&IID_IMmTradeChannel>,
	public IConnectionPointContainerImpl<CMmTradeChannel>,
	public CProxy_IMmTradeChannelEvents<CMmTradeChannel>, 
	public IDispatchImpl<IMmTradeChannel, &IID_IMmTradeChannel, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMmTradeChannel()
		:m_pTrd(NULL)
		,m_pTrdByUndMan(NULL)
		,m_pTrdByUnd(NULL)
		,m_pTrdByTraderAndContract(NULL)
		,m_pTrdByFut(NULL)
		,m_pTrdForMatch(NULL)
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMTRADECHANNEL)

BEGIN_COM_MAP(CMmTradeChannel)
	COM_INTERFACE_ENTRY(IMmTradeChannel)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CMmTradeChannel)
	CONNECTION_POINT_ENTRY(__uuidof(_IMmTradeChannelEvents))
END_CONNECTION_POINT_MAP()
// ISupportsErrorInfo

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		try
		{
			_CHK(CComObject<CMmTradeInfoColl>::CreateInstance(&m_pTrd), _T("Fail to create Trade Info Collection."));
			m_spTrd.Attach(m_pTrd, TRUE);

			_CHK(CComObject<CMmTradeByUndColl>::CreateInstance(&m_pTrdByUndMan), _T("Fail to create Manual Trades Collection."));
			m_spTrdByUndMan.Attach(m_pTrdByUndMan, TRUE);

			_CHK(CComObject<CMmTradeByUndColl>::CreateInstance(&m_pTrdByUnd), _T("Fail to create Trades Collection."));
			m_spTrdByUnd.Attach(m_pTrdByUnd, TRUE);

			_CHK(CComObject<CMmTradeByUndColl>::CreateInstance(&m_pTrdByTraderAndContract), _T("Fail to create Trades By Trader and Contract Collection."));
			m_spTrdByTraderAndContract.Attach(m_pTrdByTraderAndContract, TRUE);

			_CHK(CComObject<CMmTradeByUndColl>::CreateInstance(&m_pTrdByFut), _T("Fail to create Futures Trades Collection."));
			m_spTrdByFut.Attach(m_pTrdByFut, TRUE);

			_CHK(CComObject<CMmTradeMatchColl>::CreateInstance(&m_pTrdForMatch), _T("Fail to create Futures Trades For Matching Collection."));
			m_spTrdForMatch.Attach(m_pTrdForMatch, TRUE);


		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), __uuidof(IMmTradeChannel), e.Error());
		}
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

private:
	IMmTradeByUndCollPtr m_spTrdByUndMan;
	IMmTradeByUndCollPtr m_spTrdByUnd;
	IMmTradeByUndCollPtr m_spTrdByTraderAndContract;
	IMmTradeByUndCollPtr m_spTrdByFut;
	IMmTradeMatchCollPtr m_spTrdForMatch;
	IMmTradeInfoCollPtr  m_spTrd;

private:
	void	AddTradeToTradesByUnd(CComObject<CMmTradeInfoAtom>* pTrd);
	void	AddTradeToTradesByTraderAndContract(CComObject<CMmTradeInfoAtom>* pTrd);
	void	AddManualTradeToTradesByUndMan(CComObject<CMmTradeInfoAtom>* pTrd);
	void	AddTradeToTradesByFut(CComObject<CMmTradeInfoAtom>* pTrd);
	HRESULT AdjustTraderContractAndStrategy(CComObject<CMmTradeInfoAtom>* pTrade, bool bFireEvents = false);

public:
	void FilterData(/*long* filterData,*/IEtsFilterDataPtr spFilterData,  CTradeInfoViewData&  vecShow)
	{
		if(m_pTrd)
			//m_pTrd->FilterData(filterData, m_spMain, vecShow);
			m_pTrd->FilterData(spFilterData, m_spMain, vecShow);
	}

private:

	CComObject<CMmTradeInfoColl>*   m_pTrd;
	CComObject<CMmTradeByUndColl>*  m_pTrdByUndMan;
	CComObject<CMmTradeByUndColl>*  m_pTrdByUnd;
	CComObject<CMmTradeByUndColl>*  m_pTrdByTraderAndContract;
	CComObject<CMmTradeByUndColl>*  m_pTrdByFut;

	CComObject<CMmTradeMatchColl>*	m_pTrdForMatch;

	_bstr_t				 m_bstrDatabase;
	EgLib::CDBConnection m_Connection;

	IEtsMainPtr          m_spMain;
public:

	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmTradeInfoColl*,  Trd,						m_spTrd);
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmTradeByUndColl*, TrdByUndMan,				m_spTrdByUndMan)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmTradeByUndColl*, TrdByUnd,					m_spTrdByUnd)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmTradeByUndColl*, TrdByTraderAndContract,	m_spTrdByTraderAndContract)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmTradeByUndColl*, TrdByFut,					m_spTrdByFut)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmTradeMatchColl*, TrdForMatch,				m_spTrdForMatch)
	IMPLEMENT_BSTRT_PROPERTY(DatabaseString, m_bstrDatabase)


	//STDMETHOD(OpenTradeView)(IMmTradeView** pTradeView);


	STDMETHOD(LoadTrades)(LONG lTraderId,
						  IEtsMain* pMain,
						  VARIANT_BOOL bShowProgress);

	STDMETHOD(LoadStrategy)(LONG lStrategyID, 
							IEtsStrategyAtom** pStrategyAtom, 
							VARIANT_BOOL* bNew);

	STDMETHOD(UpdateManualActivePrices)();

	STDMETHOD(UpdatePriceInfo)(LONG lTraderId, IEtsMain* pMain);
	STDMETHOD(GetData)(IEtsFilterData* ipFilter, IMmTradeInfoColl** ipResult);

};

OBJECT_ENTRY_AUTO(__uuidof(MmTradeChannel), CMmTradeChannel)
