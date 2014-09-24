// EtsPriceProfileAtom.h : Declaration of the CEtsPriceProfileAtom
#ifndef __ETSPRICEPROFILEATOM_H__
#define __ETSPRICEPROFILEATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"

//_COM_SMARTPTR_TYPEDEF(IEtsPriceProfileAtom, IID_IEtsPriceProfileAtom);

struct __EtsPriceProfileAtom
{
	LONG							m_nID;
	CComBSTR						m_bstrName;
	CComBSTR						m_bstrDescription;
	LONG							m_nDefaultID;
	VARIANT_BOOL					m_bIsOptionProfile;
	VARIANT_BOOL					m_bAlwaysUseLast;
	VARIANT_BOOL					m_bUseMidMarketForPnL;
	VARIANT_BOOL					m_bUseZeroBidRule;
	EtsUndBadPriceRuleEnum			m_enBadUndPriceRule;
	EtsOptBadSinglePriceRuleEnum	m_enBadOptSinglePriceRule;
	EtsOptBadBothPricesRuleEnum		m_enBadOptBothPriceRule;

	__EtsPriceProfileAtom()
		: m_nID(0L), m_nDefaultID(0L), m_bIsOptionProfile(VARIANT_FALSE), m_bAlwaysUseLast(VARIANT_FALSE),
		m_bUseMidMarketForPnL(VARIANT_FALSE), m_enBadUndPriceRule(enUbprDoNotCheckTolerance),
		m_enBadOptSinglePriceRule(enObsprReplaceWithLast), m_enBadOptBothPriceRule(enObbprUseLast), m_bUseZeroBidRule(VARIANT_FALSE)
	{
	}
};

// CEtsPriceProfileAtom

class ATL_NO_VTABLE CEtsPriceProfileAtom : 
	public CComObjectRootEx<CComMultiThreadModel/*CComMultiThreadModel*/>,
	public CComCoClass<CEtsPriceProfileAtom, &CLSID_EtsPriceProfileAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsPriceProfileAtom>,
	public IDispatchImpl<IEtsPriceProfileAtom, &IID_IEtsPriceProfileAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __EtsPriceProfileAtom
{
public:
	CEtsPriceProfileAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSPRICEPROFILEATOM)


BEGIN_COM_MAP(CEtsPriceProfileAtom)
	COM_INTERFACE_ENTRY(IEtsPriceProfileAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID);
	IMPLEMENT_BSTR_PROPERTY(Name, m_bstrName);
	IMPLEMENT_BSTR_PROPERTY(Description, m_bstrDescription);
	IMPLEMENT_SIMPLE_PROPERTY(LONG, DefaultID, m_nDefaultID);
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsOptionProfile, m_bIsOptionProfile);
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, AlwaysUseLast, m_bAlwaysUseLast);
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, UseMidMarketForPnL, m_bUseMidMarketForPnL);
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, UseZeroBidRule, m_bUseZeroBidRule);
	IMPLEMENT_SIMPLE_PROPERTY(EtsUndBadPriceRuleEnum, BadUndPriceRule, m_enBadUndPriceRule);
	IMPLEMENT_SIMPLE_PROPERTY(EtsOptBadSinglePriceRuleEnum, BadOptSinglePriceRule, m_enBadOptSinglePriceRule);
	IMPLEMENT_SIMPLE_PROPERTY(EtsOptBadBothPricesRuleEnum, BadOptBothPriceRule, m_enBadOptBothPriceRule);

	STDMETHOD(GetUndPriceMid)(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
							DOUBLE dTolerance, EtsPriceRoundingRuleEnum enPriceRound,
							EtsReplacePriceStatusEnum* penPriceStatus, VARIANT_BOOL CheckProfilManual, DOUBLE* pdRetVal);

	STDMETHOD(GetUndPriceBidForPnL)(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
							DOUBLE dTolerance, EtsPriceRoundingRuleEnum enPriceRound,
							EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal);

	STDMETHOD(GetUndPriceAskForPnL)(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
							DOUBLE dTolerance, EtsPriceRoundingRuleEnum enPriceRound,
							EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal);

	STDMETHOD(GetUndPriceBid)(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
							DOUBLE dTolerance, EtsPriceRoundingRuleEnum enPriceRound,
							EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal);

	STDMETHOD(GetUndPriceAsk)(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
							DOUBLE dTolerance, EtsPriceRoundingRuleEnum enPriceRound,
							EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal);

	STDMETHOD(GetOptPriceMid)(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
							 EtsPriceRoundingRuleEnum enPriceRound, VARIANT_BOOL bUseTheoVola,
							 DOUBLE dPriceTheo, EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal);

	STDMETHOD(GetOptPriceBid)(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
							 EtsPriceRoundingRuleEnum enPriceRound, VARIANT_BOOL bUseTheoVola,
							 DOUBLE dPriceTheo, EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal);

	STDMETHOD(GetOptPriceAsk)(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
							 EtsPriceRoundingRuleEnum enPriceRound, VARIANT_BOOL bUseTheoVola,
							 DOUBLE dPriceTheo, EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal);

	STDMETHOD(GetOptPriceBidForPnL)(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
							 EtsPriceRoundingRuleEnum enPriceRound, VARIANT_BOOL bUseTheoVola,
							 DOUBLE dPriceTheo, EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal);

	STDMETHOD(GetOptPriceAskForPnL)(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
							 EtsPriceRoundingRuleEnum enPriceRound, VARIANT_BOOL bUseTheoVola,
							 DOUBLE dPriceTheo, EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal);

private:
	inline bool CheckTolerance(double dPriceBid, double dPriceAsk, double dLast, double dTolerance);
	inline double RoundPrice(double dPrice, double dRound, EtsPriceRoundingRuleEnum enPriceRound);
	inline double ReplacePrice(double dPriceTheo, double dPriceLast, double dPriceOpp, VARIANT_BOOL bUseTheoVola);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsPriceProfileAtom), CEtsPriceProfileAtom)
#define ROUND_MULT	100000.

/////////////////////////////////////////////////////////////////////////////
//
inline bool CEtsPriceProfileAtom::CheckTolerance(double dPriceBid, double dPriceAsk, double dLast, double dTolerance)
{
	const double dTol = dLast * dTolerance;
	return (dLast > 0) && 
		(fabs(dPriceBid - dLast) <= dTol) && (fabs(dPriceAsk - dLast) <= dTol);
}

inline double CEtsPriceProfileAtom::RoundPrice(double dPrice, double dRound, EtsPriceRoundingRuleEnum enPriceRound)
{
	switch (enPriceRound)
	{
	case enPrrRoundUp:
		dPrice =  ceil(round(dPrice * ROUND_MULT) / round(dRound * ROUND_MULT) ) * dRound;
		break;
	case enPrrRoundDown:
		dPrice =  floor(round(dPrice * ROUND_MULT) / round(dRound * ROUND_MULT) ) * dRound;
		break;
	}
	return dPrice;
}

inline double CEtsPriceProfileAtom::ReplacePrice(double dPriceTheo, double dPriceLast, double dPriceOpp, VARIANT_BOOL bUseTheoVola)
{
	double dRetVal = 0;

	switch (m_enBadOptSinglePriceRule)
	{
	case enObsprReplaceWithTheo:
		if (bUseTheoVola != VARIANT_FALSE)
		{
			dRetVal = dPriceTheo;
			break;
		}
	case enObsprReplaceWithLast:
		dRetVal = dPriceLast;
		break;
	case enObsprReplaceWithOpposite:
		dRetVal = dPriceOpp;
		break;
	case enObsprReplaceWithZero:
		dRetVal = 0;
		break;
	}
	return dRetVal;
}

#endif //__ETSPRICEPROFILEATOM_H__
