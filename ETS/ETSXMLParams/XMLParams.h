// XMLParams.h : Declaration of the CXMLParams
#ifndef __XML_PARAMS_H_
#define __XML_PARAMS_H_

#pragma once
#include "resource.h"       // main symbols

#include "ETSXMLParams.h"
#include <map>

typedef struct
{
	bool		IsChanged;
	CComVariant	Value;
} XMLParameterType;


typedef std::map<CAdapt<CComBSTR>, XMLParameterType>	ParametersMap;


// CXMLParams
class ATL_NO_VTABLE CXMLParams : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CXMLParams, &CLSID_XMLParams>,
	public ISupportErrorInfo,
	public IDispatchImpl<IXMLParams, &IID_IXMLParams, &LIBID_ETSXMLParamsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CXMLParams()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_XMLPARAMS)


BEGIN_COM_MAP(CXMLParams)
	COM_INTERFACE_ENTRY(IXMLParams)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
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

	STDMETHOD(SetLongValue)(BSTR bsPath, BSTR bsName, long nValue);
	STDMETHOD(SetStringValue)(BSTR bsPath, BSTR bsName, BSTR bsValue);
	STDMETHOD(GetLongValue)(BSTR bsPath, BSTR bsName, long nDefValue, long* pnValue);
	STDMETHOD(GetStringValue)(BSTR bsPath, BSTR bsName, BSTR bsDefValue, BSTR* pbsValue);

	STDMETHOD(get_Count)(long* pnCount);
	STDMETHOD(get_Item)(long idx, BSTR* pbsFullName, VARIANT* pvtParameter, VARIANT_BOOL* pIsChanged);
	STDMETHOD(Add)(BSTR bsFullName, VARIANT vtParameter);

private:

	inline void GetFullName(const BSTR bsPath, const BSTR bsName, BSTR* pbsFullName) const;

private:
	
	ParametersMap	m_Params;
};

OBJECT_ENTRY_AUTO(__uuidof(XMLParams), CXMLParams)

#endif //__XML_PARAMS_H_
