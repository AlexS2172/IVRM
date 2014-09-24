// MmRvSynthGreeksAtom.h : Declaration of the CMmRvSynthGreeksAtom
#ifndef __MMRVSYNTHGREEKSATOM_H__
#define __MMRVSYNTHGREEKSATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmRisks.h"
#include "MmRvPrice.h"
_COM_SMARTPTR_TYPEDEF(IMmRvSynthGreeksAtom, IID_IMmRvSynthGreeksAtom);

/////////////////////////////////////////////////////////////////////////////
//
struct __MmRvSynthGreeksAtom
{
	// core info
	LONG						m_nContractID;
	EtsContractTypeEnum			m_enContractType;
	_bstr_t						m_bstrSymbol;
	_bstr_t						m_bstrSynthUndSymbol;

	LONG						m_nSynthUndID;
	VARIANT_BOOL				m_bIsTotal;

	VARIANT_BOOL				m_bVisible;
	DOUBLE						m_dDeliveryUnits;

	DOUBLE						m_dDeltaInShares;
	DOUBLE						m_dGammaInShares;
	DOUBLE						m_dGammaInSharesPerc;
	DOUBLE						m_dDeltaInMoney;
	DOUBLE						m_dBetaWtdDeltaInMoney;

	DOUBLE						m_dNetExposure;
	VARIANT_BOOL				m_bBadNetExposure;

	VARIANT_BOOL				m_bBadDelta;
	VARIANT_BOOL				m_bBadGamma;
	VARIANT_BOOL				m_bBadGammaPerc;
	VARIANT_BOOL				m_bBadDeltaInMoney;
	VARIANT_BOOL				m_bBadBetaWtdDeltaInMoney;

	DOUBLE						m_dNetDelta;
	DOUBLE						m_dNetGamma;
	DOUBLE						m_dRhoInShares;
	VARIANT_BOOL				m_bBadNetDelta;
	VARIANT_BOOL				m_bBadNetGamma;
	VARIANT_BOOL				m_bBadRhoInShares;

	IMmRvPricePtr				m_spSuPrice;
	CComObject<CMmRvPrice>*     m_pSuPrice;

	DOUBLE						m_dSuPrice;

	DOUBLE						m_dThetaInShares;
	VARIANT_BOOL				m_bBadThetaInShares;

	__MmRvSynthGreeksAtom() :
	m_pSuPrice(NULL),
		m_nContractID(0L),
		m_enContractType(enCtIndex),

		m_nSynthUndID(0L),
		m_bIsTotal(VARIANT_FALSE),

		m_bVisible(VARIANT_FALSE),
		m_dDeliveryUnits(BAD_DOUBLE_VALUE),

		m_dDeltaInShares(BAD_DOUBLE_VALUE),
		m_dGammaInShares(BAD_DOUBLE_VALUE),
		m_dGammaInSharesPerc(BAD_DOUBLE_VALUE),
		m_dDeltaInMoney(BAD_DOUBLE_VALUE),
		m_dBetaWtdDeltaInMoney(BAD_DOUBLE_VALUE),
		
		m_dSuPrice(BAD_DOUBLE_VALUE),
		m_dRhoInShares(BAD_DOUBLE_VALUE),
		m_dNetExposure(BAD_DOUBLE_VALUE),

		m_bBadDelta(VARIANT_FALSE),
		m_bBadGamma(VARIANT_FALSE),
		m_bBadGammaPerc(VARIANT_FALSE),
		m_bBadDeltaInMoney(VARIANT_FALSE),
		m_bBadBetaWtdDeltaInMoney(VARIANT_FALSE),
		m_dNetGamma(BAD_DOUBLE_VALUE),
		m_bBadNetGamma(VARIANT_FALSE),
		m_dNetDelta(BAD_DOUBLE_VALUE),
		m_bBadNetDelta(VARIANT_FALSE),
		m_bBadRhoInShares(VARIANT_FALSE),
		m_bBadNetExposure(VARIANT_FALSE),
		m_bBadThetaInShares(VARIANT_FALSE),
		m_dThetaInShares(BAD_DOUBLE_VALUE)
	{
	}

	void ClearValuesInShares()
	{
		m_dDeltaInShares = BAD_DOUBLE_VALUE;
		m_dGammaInShares = BAD_DOUBLE_VALUE;
		m_dGammaInSharesPerc = BAD_DOUBLE_VALUE;
		m_dDeltaInMoney = BAD_DOUBLE_VALUE;
		m_dBetaWtdDeltaInMoney = BAD_DOUBLE_VALUE;
		m_dNetGamma = BAD_DOUBLE_VALUE;
		m_dNetDelta = BAD_DOUBLE_VALUE;
		m_dRhoInShares = BAD_DOUBLE_VALUE;
		m_dNetExposure = BAD_DOUBLE_VALUE;
		m_dThetaInShares = BAD_DOUBLE_VALUE;

		m_bBadDelta					= VARIANT_FALSE;
		m_bBadGamma					= VARIANT_FALSE;
		m_bBadGammaPerc				= VARIANT_FALSE;
		m_bBadDeltaInMoney			= VARIANT_FALSE;
		m_bBadBetaWtdDeltaInMoney	= VARIANT_FALSE;
		m_bBadNetGamma				= VARIANT_FALSE;
		m_bBadNetDelta				= VARIANT_FALSE;
		m_bBadRhoInShares			= VARIANT_FALSE;
		m_bBadNetExposure			= VARIANT_FALSE;
		m_bBadThetaInShares			= VARIANT_FALSE;
	}
};

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmRvSynthGreeksAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmRvSynthGreeksAtom, &CLSID_MmRvSynthGreeksAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRvSynthGreeksAtom>,
	public IDispatchImpl<IMmRvSynthGreeksAtom, &IID_IMmRvSynthGreeksAtom, &LIBID_EtsMmRisksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRvSynthGreeksAtom
{
public:
	CMmRvSynthGreeksAtom()
	{
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MMRVSYNTHGREEKSATOM)

	DECLARE_NOT_AGGREGATABLE(CMmRvSynthGreeksAtom)

	BEGIN_COM_MAP(CMmRvSynthGreeksAtom)
		COM_INTERFACE_ENTRY(IMmRvSynthGreeksAtom)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		HRESULT hr = CComObject<CMmRvPrice>::CreateInstance(&m_pSuPrice);
		if(FAILED(hr))
			return hr;

		m_spSuPrice.Attach(m_pSuPrice, TRUE);
		return CoCreateFreeThreadedMarshaler(GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease() 
	{
		m_pUnkMarshaler.Release();
	}

private:

	CComPtr<IUnknown>		m_pUnkMarshaler;

public:

	// core info
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ContractID, m_nContractID)
		IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum, ContractType, m_enContractType)
		IMPLEMENT_BSTRT_PROPERTY(Symbol,		 m_bstrSymbol)
		IMPLEMENT_BSTRT_PROPERTY(SynthUndSymbol, m_bstrSynthUndSymbol)

		IMPLEMENT_SIMPLE_PROPERTY(LONG, SynthUndID, m_nSynthUndID)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsTotal, m_bIsTotal)

		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, Visible, m_bVisible)

		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeliveryUnits, m_dDeliveryUnits)

		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaInShares, m_dDeltaInShares)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaInShares, m_dGammaInShares)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaInSharesPerc, m_dGammaInSharesPerc)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaInMoney, m_dDeltaInMoney)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, BetaWtdDeltaInMoney, m_dBetaWtdDeltaInMoney)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetGamma, m_dNetGamma)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetDelta, m_dNetDelta)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, RhoInShares, m_dRhoInShares)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetExposure, m_dNetExposure)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ThetaInShares, m_dThetaInShares)

		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDelta, m_bBadDelta)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGamma, m_bBadGamma)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaPerc, m_bBadGammaPerc)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaInMoney, m_bBadDeltaInMoney)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadBetaWtdDeltaInMoney, m_bBadBetaWtdDeltaInMoney)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetGamma, m_bBadNetGamma)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetDelta, m_bBadNetDelta)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadRhoInShares, m_bBadRhoInShares)
		IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetExposure, m_bBadNetExposure)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadThetaInShares, m_bBadThetaInShares)

		IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvPrice*,	SuPrice, m_spSuPrice);


	STDMETHODIMP ClearValuesInShares()
	{
		__MmRvSynthGreeksAtom::ClearValuesInShares();
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvSynthGreeksAtom), CMmRvSynthGreeksAtom)

#endif //__MMRVSYNTHGREEKSATOM_H__
//// MmRvSynthGreeksAtom.h : Declaration of the CMmRvSynthGreeksAtom
//#ifndef __MMRVSYNTHGREEKSATOM_H__
//#define __MMRVSYNTHGREEKSATOM_H__
//#pragma once
//
//#include "resource.h"       // main symbols
//#include "EtsMmRisks.h"
//
//_COM_SMARTPTR_TYPEDEF(IMmRvSynthGreeksAtom, IID_IMmRvSynthGreeksAtom);
//
///////////////////////////////////////////////////////////////////////////////
////
//struct __MmRvSynthGreeksAtom
//{
//	// core info
//	LONG						m_nContractID;
//	EtsContractTypeEnum			m_enContractType;
//	_bstr_t						m_bstrSymbol;
//	_bstr_t						m_bstrSynthUndSymbol;
//
//	LONG						m_nSynthUndID;
//	VARIANT_BOOL				m_bIsTotal;
//
//	VARIANT_BOOL				m_bVisible;
//	DOUBLE						m_dDeliveryUnits;
//		
//	DOUBLE						m_dDeltaInShares;
//	DOUBLE						m_dGammaInShares;
//	DOUBLE						m_dGammaInSharesPerc;
//	DOUBLE						m_dDeltaInMoney;
//	DOUBLE						m_dBetaWtdDeltaInMoney;
//		
//	VARIANT_BOOL				m_bBadDelta;
//	VARIANT_BOOL				m_bBadGamma;
//	VARIANT_BOOL				m_bBadGammaPerc;
//	VARIANT_BOOL				m_bBadDeltaInMoney;
//	VARIANT_BOOL				m_bBadBetaWtdDeltaInMoney;
//
//	DOUBLE							m_dNetGamma;
//	VARIANT_BOOL					m_bBadNetGamma;
//
//	
//	__MmRvSynthGreeksAtom() :
//		m_nContractID(0L),
//		m_enContractType(enCtIndex),
//		
//		m_nSynthUndID(0L),
//		m_bIsTotal(VARIANT_FALSE),
//
//		m_bVisible(VARIANT_FALSE),
//		m_dDeliveryUnits(BAD_DOUBLE_VALUE),
//			
//		m_dDeltaInShares(BAD_DOUBLE_VALUE),
//		m_dGammaInShares(BAD_DOUBLE_VALUE),
//		m_dGammaInSharesPerc(BAD_DOUBLE_VALUE),
//		m_dDeltaInMoney(BAD_DOUBLE_VALUE),
//		m_dBetaWtdDeltaInMoney(BAD_DOUBLE_VALUE),
//			
//		m_bBadDelta(VARIANT_FALSE),
//		m_bBadGamma(VARIANT_FALSE),
//		m_bBadGammaPerc(VARIANT_FALSE),
//		m_bBadDeltaInMoney(VARIANT_FALSE),
//		m_bBadBetaWtdDeltaInMoney(VARIANT_FALSE),
//		m_dNetGamma(BAD_DOUBLE_VALUE),
//		m_bBadNetGamma(VARIANT_FALSE)
//
//	{
//	}
//
//	void ClearValuesInShares()
//	{
//		m_dDeltaInShares = BAD_DOUBLE_VALUE;
//		m_dGammaInShares = BAD_DOUBLE_VALUE;
//		m_dGammaInSharesPerc = BAD_DOUBLE_VALUE;
//		m_dDeltaInMoney = BAD_DOUBLE_VALUE;
//		m_dBetaWtdDeltaInMoney = BAD_DOUBLE_VALUE;
//			
//		m_bBadDelta = VARIANT_FALSE;
//		m_bBadGamma = VARIANT_FALSE;
//		m_bBadGammaPerc = VARIANT_FALSE;
//		m_bBadDeltaInMoney = VARIANT_FALSE;
//		m_bBadBetaWtdDeltaInMoney = VARIANT_FALSE;
//	}
//};
//
///////////////////////////////////////////////////////////////////////////////
////
//class ATL_NO_VTABLE CMmRvSynthGreeksAtom : 
//	public CComObjectRootEx<CComSingleThreadModel>,
//	public CComCoClass<CMmRvSynthGreeksAtom, &CLSID_MmRvSynthGreeksAtom>,
//	public ISupportErrorInfoImpl<&IID_IMmRvSynthGreeksAtom>,
//	public IDispatchImpl<IMmRvSynthGreeksAtom, &IID_IMmRvSynthGreeksAtom, &LIBID_EtsMmRisksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
//	public __MmRvSynthGreeksAtom
//{
//public:
//	CMmRvSynthGreeksAtom()
//	{
//	}
//
//DECLARE_REGISTRY_RESOURCEID(IDR_MMRVSYNTHGREEKSATOM)
//
//DECLARE_NOT_AGGREGATABLE(CMmRvSynthGreeksAtom)
//
//BEGIN_COM_MAP(CMmRvSynthGreeksAtom)
//	COM_INTERFACE_ENTRY(IMmRvSynthGreeksAtom)
//	COM_INTERFACE_ENTRY(IDispatch)
//	COM_INTERFACE_ENTRY(ISupportErrorInfo)
//END_COM_MAP()
//
//	DECLARE_PROTECT_FINAL_CONSTRUCT()
//
//	HRESULT FinalConstruct()
//	{
//		return S_OK;
//	}
//	
//	void FinalRelease() 
//	{
//	}
//
//public:
//	
//	// core info
//	IMPLEMENT_SIMPLE_PROPERTY(LONG, ContractID, m_nContractID)
//	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum, ContractType, m_enContractType)
//	IMPLEMENT_BSTRT_PROPERTY(Symbol,		 m_bstrSymbol)
//	IMPLEMENT_BSTRT_PROPERTY(SynthUndSymbol, m_bstrSynthUndSymbol)
//
//	IMPLEMENT_SIMPLE_PROPERTY(LONG, SynthUndID, m_nSynthUndID)
//	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsTotal, m_bIsTotal)
//
//	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, Visible, m_bVisible)
//
//	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeliveryUnits, m_dDeliveryUnits)
//		
//	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaInShares, m_dDeltaInShares)
//	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaInShares, m_dGammaInShares)
//	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaInSharesPerc, m_dGammaInSharesPerc)
//	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaInMoney, m_dDeltaInMoney)
//	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, BetaWtdDeltaInMoney, m_dBetaWtdDeltaInMoney)
//		
//	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDelta, m_bBadDelta)
//	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGamma, m_bBadGamma)
//	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaPerc, m_bBadGammaPerc)
//	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaInMoney, m_bBadDeltaInMoney)
//	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadBetaWtdDeltaInMoney, m_bBadBetaWtdDeltaInMoney)
//
//	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetGamma, m_dNetGamma)
//	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetGamma, m_bBadNetGamma)
//
//
//	STDMETHODIMP ClearValuesInShares()
//	{
//		__MmRvSynthGreeksAtom::ClearValuesInShares();
//		return S_OK;
//	}
//};
//
//OBJECT_ENTRY_AUTO(__uuidof(MmRvSynthGreeksAtom), CMmRvSynthGreeksAtom)
//
//#endif //__MMRVSYNTHGREEKSATOM_H__