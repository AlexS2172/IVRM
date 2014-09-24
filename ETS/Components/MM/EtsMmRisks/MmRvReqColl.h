// MmRvReqColl.h : Declaration of the CMmRvReqColl
// MmRvReqColl.h : Declaration of the CMmRvReqColl
#ifndef __MMRVREQCOLL_H__
#define __MMRVREQCOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmRisks.h"
#include "MmRvReqAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmRvReqColl, IID_IMmRvReqColl);

typedef IDispatchImpl<IMmRvReqColl, &IID_IMmRvReqColl, &LIBID_EtsMmRisksLib>					    IMmRvReqCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRvReqCollDispImpl, BSTR, IMmRvReqAtom, _bstr_t >	IMmRvReqCollImpl;

// CMmRvReqColl

class ATL_NO_VTABLE CMmRvReqColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmRvReqColl, &CLSID_MmRvReqColl>,
	public ISupportErrorInfoImpl<&IID_IMmRvReqColl>,
	public IMmRvReqCollImpl
{
public:
	CMmRvReqColl()
	{
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MMRVREQCOLL)


	BEGIN_COM_MAP(CMmRvReqColl)
		COM_INTERFACE_ENTRY(IMmRvReqColl)
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
		IMmRvReqCollImpl::Clear();

		m_pUnkMarshaler.Release();
	}

private:

	CComPtr<IUnknown>		m_pUnkMarshaler;

public:
	IMmRvReqAtomPtr GetRequest(const _bstr_t& bsKey)
	{
		IMmRvReqAtomPtr spResult;
		IterType iterFind = m_coll.find(bsKey);
		if(iterFind != m_coll.end())
			spResult = iterFind->second;
		return spResult;
	}

	IMmRvReqAtomPtr AddNew(_bstr_t bsKey, CComObject<CMmRvReqAtom>** pObject = NULL);
public:

	STDMETHOD(Add)(BSTR Key, IMmRvReqAtom* Value, IMmRvReqAtom** pRetVal);
	STDMETHOD(StartRealtime)(IBatchPriceProvider* spProvider);
	STDMETHOD(RequestLastQuotes)(IBatchPriceProvider* spProvider);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRvReqColl), CMmRvReqColl)

#endif //__MMRVREQCOLL_H__

//// MmRvReqColl.h : Declaration of the CMmRvReqColl
//// MmRvReqColl.h : Declaration of the CMmRvReqColl
//#ifndef __MMRVREQCOLL_H__
//#define __MMRVREQCOLL_H__
//#pragma once
//
//#include "resource.h"       // main symbols
//#include "EtsMmRisks.h"
//#include "MmRvReqAtom.h"
//
//_COM_SMARTPTR_TYPEDEF(IMmRvReqColl, IID_IMmRvReqColl);
//
//typedef IDispatchImpl<IMmRvReqColl, &IID_IMmRvReqColl, &LIBID_EtsMmRisksLib>					IMmRvReqCollDispImpl;
//typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmRvReqCollDispImpl, BSTR, IMmRvReqAtom, CAdapt<CComBSTR> >	IMmRvReqCollImpl;
//
//// CMmRvReqColl
//
//class ATL_NO_VTABLE CMmRvReqColl : 
//	public CComObjectRootEx<CComSingleThreadModel>,
//	public CComCoClass<CMmRvReqColl, &CLSID_MmRvReqColl>,
//	public ISupportErrorInfoImpl<&IID_IMmRvReqColl>,
//	public IMmRvReqCollImpl
//{
//public:
//	CMmRvReqColl()
//	{
//	}
//
//DECLARE_REGISTRY_RESOURCEID(IDR_MMRVREQCOLL)
//
//
//BEGIN_COM_MAP(CMmRvReqColl)
//	COM_INTERFACE_ENTRY(IMmRvReqColl)
//	COM_INTERFACE_ENTRY(IDispatch)
//	COM_INTERFACE_ENTRY(ISupportErrorInfo)
//END_COM_MAP()
//
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
//		IMmRvReqCollImpl::Clear();
//	}
//
//public:
//
//	STDMETHOD(Add)(BSTR Key, IMmRvReqAtom* Value, IMmRvReqAtom** pRetVal);
//};
//
//OBJECT_ENTRY_AUTO(__uuidof(MmRvReqColl), CMmRvReqColl)
//
//#endif //__MMRVREQCOLL_H__
