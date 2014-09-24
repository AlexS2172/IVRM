// MmTradeUpdateColl.h : Declaration of the CMmTradeUpdateColl
#ifndef __MM_TRADE_UPDATE_COLL_H__
#define __MM_TRADE_UPDATE_COLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"

_COM_SMARTPTR_TYPEDEF(IMmTradeUpdateColl, IID_IMmTradeUpdateColl);

typedef IDispatchImpl<IMmTradeUpdateColl, &IID_IMmTradeUpdateColl, &LIBID_EtsGeneralLib>			IMmTradeUpdateCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmTradeUpdateCollDispImpl, LONG, ITradeUpdate>	IMmTradeUpdateCollImpl;

// CMmTradeUpdateColl
class ATL_NO_VTABLE CMmTradeUpdateColl :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmTradeUpdateColl, &CLSID_MmTradeUpdateColl>,
	public ISupportErrorInfoImpl<&IID_IMmTradeUpdateColl>,
	public IMmTradeUpdateCollImpl
{
public:
	CMmTradeUpdateColl()
	{
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MMTRADEUPDATECOLL)


	BEGIN_COM_MAP(CMmTradeUpdateColl)
		COM_INTERFACE_ENTRY(IMmTradeUpdateColl)
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
		IMmTradeUpdateCollImpl::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(Add)(LONG Key, ITradeUpdate* Value, ITradeUpdate** pRetVal);
	STDMETHOD(Clone)(IMmTradeUpdateColl** ppSrcColl);

private:

	HRESULT CopyTrade(ITradeUpdate* pSrc, ITradeUpdate* pDst);

};

OBJECT_ENTRY_AUTO(__uuidof(MmTradeUpdateColl), CMmTradeUpdateColl)

#endif //__MM_TRADE_UPDATE_COLL_H__