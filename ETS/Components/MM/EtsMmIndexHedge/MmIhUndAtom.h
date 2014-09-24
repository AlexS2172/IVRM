// MmIhUndAtom.h : Declaration of the CMmIhUndAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmIndexHedge.h"

_COM_SMARTPTR_TYPEDEF(IMmIhUndAtom, IID_IMmIhUndAtom);

struct __MmIhUndAtom
{
	LONG						m_nID;
	CComBSTR					m_bstrSymbol;
	IEtsPriceProfileAtomPtr		m_spUndPriceProfile;
	LONG						m_nTotalQtyInShares;
	CComBSTR					m_bstrPrimaryExch;

	DOUBLE						m_dWeight;
	DOUBLE						m_dPriceBid;
	DOUBLE						m_dPriceAsk;
	DOUBLE						m_dPriceLast;

	LONG						m_nIndexShares1;
	LONG						m_nIndexShares2;
	LONG						m_nIndexTxn;
	LONG						m_nCurShares;
	LONG						m_nTransact;
	CComBSTR					m_bstrPrimaryExchImportID;

	__MmIhUndAtom()
		: m_nID(0L), m_nTotalQtyInShares(BAD_LONG_VALUE), m_dWeight(BAD_DOUBLE_VALUE),
		m_dPriceBid(BAD_DOUBLE_VALUE), m_dPriceAsk(BAD_DOUBLE_VALUE), m_dPriceLast(BAD_DOUBLE_VALUE),
		m_nIndexShares1(BAD_LONG_VALUE), m_nIndexShares2(BAD_LONG_VALUE), m_nIndexTxn(BAD_LONG_VALUE),
		m_nCurShares(BAD_LONG_VALUE), m_nTransact(BAD_LONG_VALUE)
	{
	}
};

// CMmIhUndAtom

class ATL_NO_VTABLE CMmIhUndAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmIhUndAtom, &CLSID_MmIhUndAtom>,
	public IDispatchImpl<IMmIhUndAtom, &IID_IMmIhUndAtom, &LIBID_EtsMmIndexHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmIhUndAtom
{
public:
	CMmIhUndAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMIHUNDATOM)


BEGIN_COM_MAP(CMmIhUndAtom)
	COM_INTERFACE_ENTRY(IMmIhUndAtom)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spUndPriceProfile = NULL;
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, UndPriceProfile, m_spUndPriceProfile)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, TotalQtyInShares, m_nTotalQtyInShares)
	IMPLEMENT_BSTR_PROPERTY(PrimaryExch, m_bstrPrimaryExch)
	IMPLEMENT_BSTR_PROPERTY(PrimaryExchImportID, m_bstrPrimaryExchImportID)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Weight, m_dWeight)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceBid, m_dPriceBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceAsk, m_dPriceAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceLast, m_dPriceLast)

	IMPLEMENT_SIMPLE_PROPERTY(LONG, IndexShares1, m_nIndexShares1)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, IndexShares2, m_nIndexShares2)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, IndexTxn, m_nIndexTxn)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, CurShares, m_nCurShares)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Transact, m_nTransact)
};

OBJECT_ENTRY_AUTO(__uuidof(MmIhUndAtom), CMmIhUndAtom)
