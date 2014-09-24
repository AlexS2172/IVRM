// MmHsCalc.h : Declaration of the CMmHsCalc

#pragma once
#include "resource.h"       // main symbols
#include "EtsMmHedge.h"
#include "MmHsUndColl.h"
#include "MmHsCorrelationColl.h"

// CMatrix

template<class _Value>
class CCommonMatrix
{
	typedef std::map<CMmHsKey, _Value> TMap;

public:

	_Value Get(CMmHsKey& Key) const
	{
		TMap::const_iterator _iter = m_Map.find(Key);
		if (_iter != m_Map.end() )
			return _iter->second;

		return NULL;
	}

	void Set(CMmHsKey& Key, _Value Value)
	{
		m_Map[Key] = Value;
	}

	void Add(CMmHsKey& Key, _Value Value)
	{
		TMap::iterator _iter = m_Map.find(Key);
		if (_iter == m_Map.end() )
			m_Map[Key] = Value;
	}

	void Clear()
	{
		m_Map.clear();
	}

	LONG Size()
	{
		return (LONG)m_Map.size();
	}

private:
	TMap	m_Map;
};

// CMmHsCalc

class ATL_NO_VTABLE CMmHsCalc : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmHsCalc, &CLSID_MmHsCalc>,
	public ISupportErrorInfoImpl<&IID_IMmHsCalc>,
	public IDispatchImpl<IMmHsCalc, &IID_IMmHsCalc, &LIBID_EtsMmHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
	typedef std::map<LONG, DOUBLE>	TMapOfDoubleValues;
	typedef TMapOfDoubleValues	TMapStocksYs;
	typedef TMapOfDoubleValues	TMapInstrumentsYs;
	typedef TMapOfDoubleValues	TMapInstrumentsYs;
	typedef TMapOfDoubleValues	TMapInstrumentsKs;	
	typedef TMapOfDoubleValues	TMapInstrumentsBs;	
	typedef TMapOfDoubleValues	TMapInstrumentsDeltas;
	typedef TMapOfDoubleValues	TMapInstrumentsQs;
	typedef TMapOfDoubleValues	TMapInstrumentsVegas;	
	typedef TMapOfDoubleValues	TMapOfResults;

public:
	CMmHsCalc()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMHSCALC)


BEGIN_COM_MAP(CMmHsCalc)
	COM_INTERFACE_ENTRY(IMmHsCalc)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			__CHECK_HRESULT(m_spStocks.CreateInstance(CLSID_MmHsUndColl), 
				_T("Failed to create stocks collection"));
			__CHECK_HRESULT(m_spInstruments.CreateInstance(CLSID_MmHsUndColl), 
				_T("Failed to create instruments collection"));
			__CHECK_HRESULT(m_spCorrelations.CreateInstance(CLSID_MmHsCorrelationColl), 
				_T("Failed to create correlations collection"));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmHsCalc, e.Error());
		}

		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spStocks = NULL;
		m_spInstruments = NULL;
		m_spCorrelations = NULL;
	}

public:

	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmHsUndColl*, Stocks, m_spStocks)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmHsUndColl*, Instruments, m_spInstruments)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmHsCorrelationColl*, Correlations, m_spCorrelations)
	
	STDMETHOD(CalcYs)( EtsHedgeTypeEnum enHedgeType, 
						DOUBLE dUndPriceTolerance, 
						EtsPriceRoundingRuleEnum enPriceRoundingRule);
	STDMETHOD(CreateEqs)();
	STDMETHOD(DecomposeMatrix)();
	STDMETHOD(SolveEqs)(EtsHedgeTypeEnum enHedgeType);
	STDMETHOD(CalcPositions)(DOUBLE dUndPriceTolerance, 
						EtsPriceRoundingRuleEnum enPriceRoundingRule);
	STDMETHOD(CalcGain)(DOUBLE* dblGain);

private:

	IMmHsUndCollPtr				m_spStocks;
	IMmHsUndCollPtr				m_spInstruments;
	IMmHsCorrelationCollPtr		m_spCorrelations;

	CMmHsCalc::TMapStocksYs			m_StocksYs;
	CMmHsCalc::TMapInstrumentsYs	m_InstrumentsYs;	
	CCommonMatrix<DOUBLE>			m_InstrumentsAs;
	CMmHsCalc::TMapInstrumentsBs	m_InstrumentsBs;	
	CMmHsCalc::TMapOfResults		m_Results;
	
	void	_CalcPortfolioVariations( DOUBLE* pdVar, DOUBLE* pdVarHedged) throw(const _com_error&);	
	void	_CalcAs() throw(const _com_error&);
	void	_CalcBs() throw(const _com_error&);
	
	HRESULT	_GetStockNameById(LONG nId, CComBSTR& bstrName);
	HRESULT	_GetInstrumentNameById(LONG nId, CComBSTR& bstrName);
};

OBJECT_ENTRY_AUTO(__uuidof(MmHsCalc), CMmHsCalc)
