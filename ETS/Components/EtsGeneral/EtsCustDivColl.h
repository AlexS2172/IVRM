// EtsCustDivColl.h : Declaration of the CEtsCustDivColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"

_COM_SMARTPTR_TYPEDEF(IEtsCustDivColl, IID_IEtsCustDivColl);

typedef IDispatchImpl<IEtsCustDivColl, &IID_IEtsCustDivColl, &LIBID_EtsGeneralLib>						IEtsCustDivCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IEtsCustDivCollDispImpl, DATE, IEtsCustDivAtom, DATE >		IEtsCustDivCollImpl;


// CEtsCustDivColl

class ATL_NO_VTABLE CEtsCustDivColl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEtsCustDivColl, &CLSID_EtsCustDivColl>,
	public ISupportErrorInfo,
	public IEtsCustDivCollImpl
{
public:
	CEtsCustDivColl():
	  m_lStockID (0),
	  m_enDivType (enMarketDiv),
	  m_nDivFreqMarket (0),
	  m_dtDivDateMarket (0),
	  m_dDivAmtMarket (0),
	  m_nDivFreqCust (0),
	  m_dtDivDateCust (0),
	  m_dDivAmtCust (0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSCUSTDIVCOLL)


BEGIN_COM_MAP(CEtsCustDivColl)
	COM_INTERFACE_ENTRY(IEtsCustDivColl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		IEtsCustDivCollImpl::Clear();
		
	}

public:
	STDMETHOD(Add) (DATE Key, IEtsCustDivAtom *Value, IEtsCustDivAtom **pRetVal);

	STDMETHOD(Clear)();

	// Add or Edit Dividend Amount for Specified Date
	STDMETHOD (SetDividend) (DATE DivDate, double dDivAmount);
	STDMETHOD (GetDividend) (double dDivAmount, double* pdRetVal);
	STDMETHOD (DeleteDividen) (double DivDate);

	STDMETHOD (GetNextDevidentsCount) (/*[in]*/LONG nToday, /*[in]*/LONG nExpiryDay, /*[out, retval]*/LONG* pnDivCount);
	//С эти методом надо обращатья осторожно и не вызывать по моршалингу, т.к. предпологается, что память выделяется вне 
	STDMETHOD (GetNextDevidentsAsArray) (/*[in]*/LONG nToday, /*[in]*/LONG nExpiryDay, /*[in]*/LONG nCount, /*[in,out]*/DATE* pdDivDte, /*[in,out]*/DOUBLE* pdDivAmt,/*[out,retval]*/LONG* pnCount);

	STDMETHOD (IsValidDivs) (/*[in]*/ VARIANT_BOOL CheckCustom, /*[out, retval]*/ VARIANT_BOOL* pVal);

	STDMETHOD (CorrectDivs) ();


public:
	IMPLEMENT_SIMPLE_PROPERTY (LONG, StockID, m_lStockID);

	IMPLEMENT_SIMPLE_PROPERTY (EtsDividentsTypeEnum, DivType, m_enDivType)

	IMPLEMENT_SIMPLE_PROPERTY (LONG, DivFreqMarket, m_nDivFreqMarket)
	IMPLEMENT_SIMPLE_PROPERTY (DATE, DivDateMarket, m_dtDivDateMarket)
	IMPLEMENT_SIMPLE_PROPERTY (DOUBLE, DivAmtMarket, m_dDivAmtMarket)

	IMPLEMENT_SIMPLE_PROPERTY (LONG, DivFreqCust, m_nDivFreqCust)
	IMPLEMENT_SIMPLE_PROPERTY (DATE, DivDateCust, m_dtDivDateCust)
	IMPLEMENT_SIMPLE_PROPERTY (DOUBLE, DivAmtCust, m_dDivAmtCust)

	IMPLEMENT_SIMPLE_PROPERTY (LONG, DivFreq, _DivFreqRef ())
	IMPLEMENT_SIMPLE_PROPERTY (DATE, DivDate, _DivDateRef ())
	IMPLEMENT_SIMPLE_PROPERTY (DOUBLE, DivAmt, _DivAmtRef ())


protected:
	CComPtr<IEtsCustDivAtom> CEtsCustDivColl::_AddNew (DATE Key);
	LONG _GetNextDevidentsCountFromFrequency (LONG nToday, LONG nExpiryDay);
	LONG _GetNextDevidentsFromFrequency (LONG nToday, LONG nExpiryDay, LONG nCount, DATE* pdDivDte, DOUBLE* pdDivAmt);
	LONG _GetNextDevidentsFromTabl (LONG nToday, LONG nExpiryDay, LONG nCount = 0, DATE* pdDivDte = NULL, DOUBLE* pdDivAmt = NULL);

	//---------
	LONG& _DivFreqRef ()
	{
		static LONG lNoting;
		lNoting = 0;

		switch(m_enDivType) {
		case enMarketDiv:
			return m_nDivFreqMarket;
			break;
		case enCustomDFDiv:
			return m_nDivFreqCust;
			break;
		default:
			return lNoting;
		}
	}

	//-------------
	DATE& _DivDateRef ()
	{
		static DOUBLE dNoting;
		dNoting = 0;

		switch(m_enDivType) {
		case enMarketDiv:
			return m_dtDivDateMarket;
			break;
		case enCustomDFDiv:
			return m_dtDivDateCust;
				break;
		default:
			return dNoting;
		}
	}

	//-------------
	DOUBLE& _DivAmtRef ()
	{
		static DOUBLE dNoting;
		dNoting = 0;

		switch(m_enDivType) {
		case enMarketDiv:
			return m_dDivAmtMarket;
			break;
		case enCustomDFDiv:
			return m_dDivAmtCust;
			break;
		default:
			return dNoting;
		}
	}

	bool _IsValidDivs (bool bCheckCustom);
	

private:
	LONG					m_lStockID;
	EtsDividentsTypeEnum	m_enDivType;
	LONG					m_nDivFreqMarket;
	DATE					m_dtDivDateMarket;
	DOUBLE					m_dDivAmtMarket;
	LONG					m_nDivFreqCust;
	DATE					m_dtDivDateCust;
	DOUBLE					m_dDivAmtCust;
};

OBJECT_ENTRY_AUTO(__uuidof(EtsCustDivColl), CEtsCustDivColl)
