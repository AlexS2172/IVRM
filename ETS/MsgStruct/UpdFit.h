// UpdFit.h : Declaration of the CUpdFit

#pragma once
#include "resource.h"       // main symbols

#include "MsgStruct.h"

struct __IVFitTimeUpd
{
	BEGIN_PACKING
		FIELD_ENTRY(m_dMivUpd)
		FIELD_ENTRY(m_dLastFitTime)
		FIELD_ENTRY(m_dNextFitTime)
	END_PACKING

	DateF							m_dMivUpd ;
	DateF							m_dLastFitTime ;
	DateF							m_dNextFitTime ;
};


// CUpdFit

class ATL_NO_VTABLE CUpdFit : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CUpdFit, &CLSID_UpdFit>,
	public IDispatchImpl<IUpdFit, &IID_IUpdFit, &LIBID_MSGSTRUCTLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public INetPackingConstSizeDataImpl<CUpdFit,__IVFitTimeUpd>
{
public:
	CUpdFit()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_UPDFIT)


BEGIN_COM_MAP(CUpdFit)
	COM_INTERFACE_ENTRY(IUpdFit)
	COM_INTERFACE_ENTRY2(IDispatch, IUpdFit)
	COM_INTERFACE_ENTRY(INetPacking)
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
	}

public:

	//IMPLEMENT_DATE_PROP(MIVUpdTime,m_dMivUpd) ;
	STDMETHODIMP put_MIVUpdTime(DATE NewVal)
	{
		ObjectLock lock(this);
		m_dMivUpd = NewVal;
		return S_OK;
	}
	STDMETHODIMP get_MIVUpdTime(DATE* pRetVal)
	{
		if (!pRetVal)
			return E_POINTER;
		ObjectLock lock(this);
		*pRetVal = m_dMivUpd;
		return S_OK;
	}													
	STDMETHODIMP put_LastFitTime(DATE NewVal)
	{
		ObjectLock lock(this);
		m_dLastFitTime = NewVal;
		return S_OK;
	}
	STDMETHODIMP get_LastFitTime(DATE* pRetVal)
	{
		if (!pRetVal)
			return E_POINTER;
		ObjectLock lock(this);
		*pRetVal = m_dLastFitTime;
		return S_OK;
	}

	STDMETHODIMP put_NextFitTime(DATE NewVal)
	{
		ObjectLock lock(this);
		m_dNextFitTime = NewVal;
		return S_OK;
	}
	STDMETHODIMP get_NextFitTime(DATE* pRetVal)
	{
		if (!pRetVal)
			return E_POINTER;
		ObjectLock lock(this);
		*pRetVal = m_dNextFitTime;
		return S_OK;
	}

//	IMPLEMENT_DATE_PROP(LastfitTime,m_dLastFitTime) ;
//	IMPLEMENT_DATE_PROP(NextFitTime,m_dNextFitTime) ;

};

OBJECT_ENTRY_AUTO(__uuidof(UpdFit), CUpdFit)
