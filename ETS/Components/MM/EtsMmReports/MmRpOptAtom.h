// MmRpOptAtom.h : Declaration of the CMmRpOptAtom
#ifndef __MM_RP_OPT_ATOM__
#define __MM_RP_OPT_ATOM__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"
#include "MmRpPosAtom.h"
#include "MmRpFutAtom.h"
#include "RpPrice.h"

_COM_SMARTPTR_TYPEDEF(IMmRpOptAtom, IID_IMmRpOptAtom);

struct __MmRpOptAtom
{
	LONG					m_nID;
	CComBSTR				m_bstrSymbol;
	
	DOUBLE					m_dPriceTheoClose;
	DATE					m_dtExpiry;
	DOUBLE					m_dStrike;
	EtsOptionTypeEnum		m_enOptType;
	LONG					m_nRootID;
	LONG					m_nLotSize;
	DOUBLE					m_dVegaWeight;
	IMmRpPosAtomPtr			m_spPos;
	CComBSTR				m_bstrAccount;
	CComBSTR				m_bstrGroup;
	CComBSTR				m_bstrUnderlyingName ;
	EtsContractTypeEnum					m_enContractType;
	IMmRpFutAtomPtr						m_spFut;
	IMMRpPricePtr						m_spPrice ;

	DATE					m_dtExpiryOV;
	DATE					m_dtTradingClose;


	__MmRpOptAtom() :
		m_nID(0L), m_dPriceTheoClose(0.),
		m_dtExpiry(0.), m_dStrike(0.), m_enOptType(enOtCall), 
		m_nRootID(0L), m_nLotSize(0L), m_dVegaWeight(0.),
		m_dtExpiryOV(0), m_dtTradingClose(0)
	{
	}
};

// CMmRpOptAtom

class ATL_NO_VTABLE CMmRpOptAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpOptAtom, &CLSID_MmRpOptAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRpOptAtom>,
	public IDispatchImpl<IMmRpOptAtom, &IID_IMmRpOptAtom, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRpOptAtom
{
public:
	CMmRpOptAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPOPTATOM)

DECLARE_NOT_AGGREGATABLE(CMmRpOptAtom)

BEGIN_COM_MAP(CMmRpOptAtom)
	COM_INTERFACE_ENTRY(IMmRpOptAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			m_spFut = NULL;
			__CHECK_HRESULT(m_spPos.CreateInstance(CLSID_MmRpPosAtom), _T("Failed to create position."));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpOptAtom, e.Error());
		}

		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spPos = NULL;
		m_spFut = NULL;
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG,					ID,				m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol,		m_bstrSymbol)
	/*IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				PriceBid,		m_dPriceBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				PriceAsk,		m_dPriceAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				PriceLast,		m_dPriceLast)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				PriceClose,		m_dPriceClose)*/
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				PriceTheoClose,	m_dPriceTheoClose)
	IMPLEMENT_SIMPLE_PROPERTY(DATE,					Expiry,			m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(DATE,					ExpiryOV,		m_dtExpiryOV)
	IMPLEMENT_SIMPLE_PROPERTY(DATE,					TradingClose,	m_dtTradingClose)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Strike,			m_dStrike)
	IMPLEMENT_SIMPLE_PROPERTY(EtsOptionTypeEnum,	OptType,		m_enOptType)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					RootID,			m_nRootID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					LotSize,		m_nLotSize)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				VegaWeight,		m_dVegaWeight)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRpPosAtom*,	Pos,	m_spPos)
	IMPLEMENT_OBJECT_PROPERTY(IMMRpPrice*,	Price, m_spPrice )

	IMPLEMENT_BSTR_PROPERTY(Account,		m_bstrAccount)
	IMPLEMENT_BSTR_PROPERTY(Group,			m_bstrGroup)
	IMPLEMENT_BSTR_PROPERTY(UnderlyingName, m_bstrUnderlyingName)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum, ContractType, m_enContractType)
	IMPLEMENT_OBJECT_PROPERTY(IMmRpFutAtom*,		Fut,			m_spFut)
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpOptAtom), CMmRpOptAtom)

#endif //__MM_RP_OPT_ATOM__