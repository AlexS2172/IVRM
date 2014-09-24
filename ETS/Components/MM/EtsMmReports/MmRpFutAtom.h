// MmRpFutAtom.h : Declaration of the CMmRpFutAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmReports.h"
#include "MmRpPosAtom.h"
#include "RpPrice.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

_COM_SMARTPTR_TYPEDEF(IMmRpFutAtom, IID_IMmRpFutAtom);

struct __MmRpFutAtom
{
	// core info
	LONG									m_nID;
	DATE									m_dtMaturity;
	_bstr_t								m_bstrSymbol;
	_bstr_t								m_bstrFutRootSymbol;
	_bstr_t								m_bstrContractName;
	VARIANT_BOOL						m_bIsAmerican;
	LONG									m_nExpCalendarID;
	IEtsPriceProfileAtomPtr			m_spUndPriceProfile;
	IEtsPriceProfileAtomPtr			m_spOptPriceProfile;
	LONG									m_nFutRootID;
	LONG									m_nUndID;
	VARIANT_BOOL						m_bCalcGreeks;
	DOUBLE								m_dNetChange;
	LONG									m_nOptLotSize;
	LONG									m_nFutLotSize;
	EtsReplacePriceStatusEnum		m_enReplacePriceStatus;
	DOUBLE								m_dBasis;
	DOUBLE								m_dActiveFuturePrice;
	IMmRpFutAtomPtr					m_spActiveFuture;
	IMmRpPosAtomPtr					m_spPos;
	IMMRpPricePtr						m_spPrice ;
	double								m_dRatio;

	__MmRpFutAtom() :
		m_nID(0L),
		m_dtMaturity(0.),
		m_bIsAmerican(VARIANT_FALSE),
		m_nExpCalendarID(0L),
		m_nFutRootID(0L),
		m_nUndID(0L),
		m_bCalcGreeks(VARIANT_FALSE),
		m_dNetChange(BAD_DOUBLE_VALUE),
		m_nOptLotSize(0L),
		m_nFutLotSize(0L),
		m_enReplacePriceStatus(enRpsNone),
		m_dBasis(BAD_DOUBLE_VALUE),
		m_dActiveFuturePrice(BAD_DOUBLE_VALUE),
		m_dRatio(1.)
	{
	}
};

// CMmRpFutAtom

class ATL_NO_VTABLE CMmRpFutAtom :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmRpFutAtom, &CLSID_MmRpFutAtom>,
	public ISupportErrorInfo,
	public IDispatchImpl<IMmRpFutAtom, &IID_IMmRpFutAtom, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRpFutAtom
{
public:
	CMmRpFutAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPFUTATOM)


BEGIN_COM_MAP(CMmRpFutAtom)
	COM_INTERFACE_ENTRY(IMmRpFutAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		try
		{	
			__CHECK_HRESULT(m_spPos.CreateInstance(CLSID_MmRpPosAtom), 
				_T("Failed to create position."));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpFutAtom, e.Error());
		}


		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		m_spUndPriceProfile = NULL;
		m_spOptPriceProfile = NULL;
		m_spActiveFuture = NULL;
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	IMPLEMENT_SIMPLE_PROPERTY(LONG,								ID,							m_nID)
	IMPLEMENT_BSTRT_PROPERTY(										Symbol,						m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(DATE,								Maturity,					m_dtMaturity)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL,					IsAmerican,					m_bIsAmerican)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,								ExpCalendarID,				m_nExpCalendarID)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*,		UndPriceProfile,			m_spUndPriceProfile)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*,		OptPriceProfile,			m_spOptPriceProfile)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,								FutRootID,					m_nFutRootID)
	IMPLEMENT_BSTRT_PROPERTY(										FutRootSymbol,				m_bstrFutRootSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,								UndID,						m_nUndID)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL,					CalcGreeks,					m_bCalcGreeks)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,							NetChange,					m_dNetChange)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,								OptLotSize,					m_nOptLotSize)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,								FutLotSize,					m_nFutLotSize)
	IMPLEMENT_BSTRT_PROPERTY(										ContractName,				m_bstrContractName)
	IMPLEMENT_SIMPLE_PROPERTY(EtsReplacePriceStatusEnum,	ReplacePriceStatus,		m_enReplacePriceStatus)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, 							FutureBasis,				m_dBasis)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, 							ActiveFuturePrice,		m_dActiveFuturePrice)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRpPosAtom*,		Pos,							m_spPos)
	IMPLEMENT_OBJECT_PROPERTY(IMMRpPrice*,						Price,						m_spPrice )
	IMPLEMENT_OBJECT_PROPERTY(IMmRpFutAtom*,					ActiveFuture,				m_spActiveFuture)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, 							Ratio,						m_dRatio)

	STDMETHOD(GetFuturePrice)(	DOUBLE dTolerance,
										EtsPriceRoundingRuleEnum enPriceRound,
										EtsReplacePriceStatusEnum* penPriceStatus,
										VARIANT_BOOL *bFutureUsed,
										DOUBLE *pPrice);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRpFutAtom), CMmRpFutAtom)
