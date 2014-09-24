// MmQvRowData.h : Declaration of the CMmQvRowData

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmQuotes.h"

#include "MmQvExpAtom.h"
#include "MmQvUndAtom.h"
#include "MmQvQuoteAtom.h"
#include "MmQvStrikeAtom.h"
#include "MmQvOptRootAtom.h"
#include "MmQvFutAtom.h"
#include "MmQvFutRootAtom.h"
#include "MmQvOptAtom.h"
#include "MmQvQuoteAtom.h"
#include "MmQvExpAtom.h"
#include "MmQvGrpAtom.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CMmQvRowData
_COM_SMARTPTR_TYPEDEF(IMmQvRowData, IID_IMmQvRowData);


class ATL_NO_VTABLE CMmQvRowData :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvRowData, &CLSID_MmQvRowData>,
	public ISupportErrorInfoImpl<&IID_IMmQvRowData>,
	public IDispatchImpl<IMmQvRowData, &IID_IMmQvRowData, &LIBID_EtsMmQuotesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMmQvRowData()
		:m_pUnd(NULL)
		,m_pUndQuote(NULL)
		,m_pExp(NULL)
		,m_pStrike(NULL)
		,m_pOptRoot(NULL)
		,m_pFut(NULL)
		,m_pFutRoot(NULL)
		,m_pGrp(NULL)
		,m_nPositionNumber(0)
		,m_pFutQuote(NULL)
	{
		m_pOpt[0]		= NULL;
		m_pOpt[1]		= NULL;
		m_pOptQuote[0]  = NULL;
		m_pOptQuote[1]	= NULL;
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVROWDATA)


BEGIN_COM_MAP(CMmQvRowData)
	COM_INTERFACE_ENTRY(IMmQvRowData)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		try
		{
			m_pGrp = NULL;

			m_pOpt[0] = NULL;
			m_pOpt[1] = NULL;
			m_pOptQuote[0] = NULL;
			m_pOptQuote[1] = NULL;

			m_spOpt[0] = NULL;
			m_spOpt[1] = NULL;
			m_spOptQuote[0] = NULL;
			m_spOptQuote[1] = NULL;

			m_pUnd = NULL;
			m_pUndQuote = NULL;
			m_pExp = NULL;
			m_pStrike = NULL;
			m_pOptRoot = NULL;
			m_pFut = NULL;
			m_pFutRoot = NULL;

			m_spUnd = NULL;
			m_spUndQuote = NULL;
			m_spExp = NULL;
			m_spStrike = NULL;
			m_spExch = NULL;
			m_spOptRoot = NULL;
			m_spFut = NULL;
			m_spFutRoot = NULL;
			m_pUnkMarshaler.Release();

		}
		catch(...) {}
		try
		{
			m_pGrp = NULL;

			m_pOpt[0] = NULL;
			m_pOpt[1] = NULL;
			m_pOptQuote[0] = NULL;
			m_pOptQuote[1] = NULL;

			m_spOpt[0] = NULL;
			m_spOpt[1] = NULL;
			m_spOptQuote[0] = NULL;
			m_spOptQuote[1] = NULL;

			m_pUnd = NULL;
			m_pUndQuote = NULL;
			m_pExp = NULL;
			m_pStrike = NULL;
			m_pOptRoot = NULL;
			m_pFut = NULL;
			m_pFutRoot = NULL;

			m_spUnd = NULL;
			m_spUndQuote = NULL;
			m_spExp = NULL;
			m_spStrike = NULL;
			m_spExch = NULL;
			m_spOptRoot = NULL;
			m_spFut = NULL;
			m_spFutRoot = NULL;
			m_pUnkMarshaler.Release();

		}
		catch(...) {}
	}
private:
	CComPtr<IUnknown> m_pUnkMarshaler;


public:
	IMmQvUndAtomPtr			m_spUnd;
	IMmQvQuoteAtomPtr		m_spUndQuote;
	IMmQvExpAtomPtr		    m_spExp;
	IMmQvStrikeAtomPtr		m_spStrike;
	IExchAtomPtr			m_spExch;
	IMmQvOptRootAtomPtr		m_spOptRoot;
	IMmQvFutAtomPtr			m_spFut;
	IMmQvFutRootAtomPtr		m_spFutRoot;

	IMmQvOptAtomPtr			m_spOpt[2];
	IMmQvQuoteAtomPtr		m_spOptQuote[2];

	IMmQvQuoteAtomPtr		m_spFutQuote;
private:
	long				m_nPositionNumber;
public:

	CComObject<CMmQvUndAtom>*	m_pUnd;
	CComObject<CMmQvQuoteAtom>*	m_pUndQuote;
	CComObject<CMmQvExpAtom>*	m_pExp;
	CComObject<CMmQvStrikeAtom>*	m_pStrike;
	CComObject<CMmQvOptRootAtom>*	m_pOptRoot;
	CComObject<CMmQvFutAtom>*	m_pFut;
	CComObject<CMmQvFutRootAtom>*	m_pFutRoot;
	CComObject<CMmQvGrpAtom>*	m_pGrp;


	CComObject<CMmQvOptAtom>*	m_pOpt[2];
	CComObject<CMmQvQuoteAtom>*	m_pOptQuote[2];

	CComObject<CMmQvQuoteAtom>*		m_pFutQuote;

private:
	bool GetField(MmQvOptColumnEnum enCol, _variant_t& vtRet, bool bForSorting, bool bShowIndexFutureOptions, CMmQvOptAtom* pOptAtom, CMmQvQuoteAtom* pQuote);


public:
	IMPLEMENT_OBJECT_PROPERTY(IExchAtom*,				 Exch,		m_spExch);

	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvUndAtom*,	 Und,		m_spUnd);
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvQuoteAtom*,   UndQuote,	m_spUndQuote);
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvExpAtom*,	 Exp,		m_spExp);
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvStrikeAtom*,	 Strike,	m_spStrike);
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvOptRootAtom*, OptRoot,	m_spOptRoot);
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvFutAtom*,	 Fut,		m_spFut);
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvFutRootAtom*, FutRoot,	m_spFutRoot);
	IMPLEMENT_SIMPLEREADONLY_PROPERTY(LONG,				 PositionNumber,		m_nPositionNumber);

	STDMETHOD(putref_Und)		(IMmQvUndAtom* pUnd);
	STDMETHOD(putref_UndQuote)	(IMmQvQuoteAtom* pUndQuote);
	STDMETHOD(putref_Exp)		(IMmQvExpAtom* pExp);
	STDMETHOD(putref_Strike)	(IMmQvStrikeAtom* pStrike);
	STDMETHOD(putref_OptRoot)	(IMmQvOptRootAtom* pOptRoot);
	STDMETHOD(putref_Fut)		(IMmQvFutAtom* pFut);
	STDMETHOD(putref_FutRoot)	(IMmQvFutRootAtom* pFutRoot);
	IMPLEMENT_OBJECT_PROPERTY(IMmQvQuoteAtom*,   FutQuote,	m_spFutQuote);

	STDMETHOD(putref_Opt)(EtsOptionTypeEnum enType, IMmQvOptAtom* pOptAtom); 
	STDMETHOD(get_Opt)(EtsOptionTypeEnum enType,  IMmQvOptAtom** ppOptAtom); 

	STDMETHOD(putref_OptQuote)(EtsOptionTypeEnum enType,  IMmQvQuoteAtom* pOptQuoteAtom); 
	STDMETHOD(get_OptQuote)(EtsOptionTypeEnum enType,  IMmQvQuoteAtom** ppOptQuoteAtom); 

	bool GetField(MmQvOptColumnEnum enCol, _variant_t& vt, bool bForSorting = false, bool bShowIndexFutureOptions = false);
	bool SetField(long lColumnId, _variant_t& vtData);

public:
	long GetRowPosition() const
	{
		return m_nPositionNumber;
	}
	void SetRowPosition(long lPosition)
	{
		m_nPositionNumber = lPosition;
	}

};

OBJECT_ENTRY_AUTO(__uuidof(MmQvRowData), CMmQvRowData)
