// MmVaOptPairAtom.h : Declaration of the CMmVaOptPairAtom
#ifndef __MMVAOPTPAIRATOM_H__
#define __MMVAOPTPAIRATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmVolaAnalysis.h"
#include "MmVaOptAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmVaOptPairAtom, IID_IMmVaOptPairAtom);

struct __MmVaOptPairAtom
{
	LONG				m_nRootID;
	LONG				m_nLotSize;
	IMmVaOptAtomPtr		m_spOpt[2];
	DOUBLE				m_dStraddleBid;
	DOUBLE				m_dStraddleAsk;
	DOUBLE				m_dStraddleIVBid;
	DOUBLE				m_dStraddleIVAsk;
	DOUBLE				m_dTotalVegaBid;
	DOUBLE				m_dTotalVegaAsk;
	DOUBLE				m_dNetDeltaBid;
	DOUBLE				m_dNetDeltaAsk;
	LONG				m_nContractsBid;
	LONG				m_nContractsAsk;
	LONG				m_nDeltaInSharesBid;
	LONG				m_nDeltaInSharesAsk;

	__MmVaOptPairAtom()
		: m_nRootID(0L), m_nLotSize(BAD_LONG_VALUE), m_dStraddleBid(BAD_DOUBLE_VALUE), m_dStraddleAsk(BAD_DOUBLE_VALUE),
		m_dStraddleIVBid(BAD_DOUBLE_VALUE), m_dStraddleIVAsk(BAD_DOUBLE_VALUE), 
		m_dTotalVegaBid(BAD_DOUBLE_VALUE), m_dTotalVegaAsk(BAD_DOUBLE_VALUE), m_dNetDeltaBid(BAD_DOUBLE_VALUE),
		m_dNetDeltaAsk(BAD_DOUBLE_VALUE), m_nContractsBid(BAD_LONG_VALUE), m_nContractsAsk(BAD_LONG_VALUE),
		m_nDeltaInSharesBid(BAD_LONG_VALUE), m_nDeltaInSharesAsk(BAD_LONG_VALUE)
	{
	}

	void ClearValues()
	{
		m_dStraddleBid = BAD_DOUBLE_VALUE;
		m_dStraddleAsk = BAD_DOUBLE_VALUE;
		m_dStraddleIVBid = BAD_DOUBLE_VALUE;
		m_dStraddleIVAsk = BAD_DOUBLE_VALUE;
		m_dTotalVegaBid = BAD_DOUBLE_VALUE;
		m_dTotalVegaAsk = BAD_DOUBLE_VALUE;
		m_dNetDeltaBid = BAD_DOUBLE_VALUE;
		m_dNetDeltaAsk = BAD_DOUBLE_VALUE;
		m_nContractsBid = BAD_LONG_VALUE;
		m_nContractsAsk = BAD_LONG_VALUE;
		m_nDeltaInSharesBid = BAD_LONG_VALUE;
		m_nDeltaInSharesAsk = BAD_LONG_VALUE;
	}
};

// CMmVaOptPairAtom
class ATL_NO_VTABLE CMmVaOptPairAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmVaOptPairAtom, &CLSID_MmVaOptPairAtom>,
	public ISupportErrorInfoImpl<&IID_IMmVaOptPairAtom>,
	public IDispatchImpl<IMmVaOptPairAtom, &IID_IMmVaOptPairAtom, &LIBID_EtsMmVolaAnalysisLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmVaOptPairAtom
{
public:
	CMmVaOptPairAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMVAOPTPAIRATOM)


BEGIN_COM_MAP(CMmVaOptPairAtom)
	COM_INTERFACE_ENTRY(IMmVaOptPairAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			__CHECK_HRESULT(m_spOpt[0].CreateInstance(CLSID_MmVaOptAtom), _T("Fail to create option."));
			__CHECK_HRESULT(m_spOpt[1].CreateInstance(CLSID_MmVaOptAtom), _T("Fail to create option."));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmVaOptPairAtom, e.Error());
		}
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spOpt[0] = NULL;
		m_spOpt[1] = NULL;
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, RootID, m_nRootID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, LotSize, m_nLotSize)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, StraddleBid, m_dStraddleBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, StraddleAsk, m_dStraddleAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, StraddleIVBid, m_dStraddleIVBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, StraddleIVAsk, m_dStraddleIVAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TotalVegaBid, m_dTotalVegaBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TotalVegaAsk, m_dTotalVegaAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetDeltaBid, m_dNetDeltaBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetDeltaAsk, m_dNetDeltaAsk)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ContractsBid, m_nContractsBid)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ContractsAsk, m_nContractsAsk)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, DeltaInSharesBid, m_nDeltaInSharesBid)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, DeltaInSharesAsk, m_nDeltaInSharesAsk)

	STDMETHOD(get_Opt)(EtsOptionTypeEnum enOptType, IMmVaOptAtom** pVal);
	STDMETHODIMP ClearValues()
	{
		__MmVaOptPairAtom::ClearValues();
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(MmVaOptPairAtom), CMmVaOptPairAtom)

#endif //__MMVAOPTPAIRATOM_H__