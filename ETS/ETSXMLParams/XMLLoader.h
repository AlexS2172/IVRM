// XMLLoader.h : Declaration of the CXMLLoader
#ifndef __XML_LOADER_H_
#define __XML_LOADER_H_

#pragma once
#include "resource.h"       // main symbols

#include "ETSXMLParams.h"
#include <vector>


#define XML_ATTRIBUTE_NAME_NAME		L"Name"
#define	XML_ATTRIBUTE_VALUE_NAME	L"Value"
#define	XML_ATTRIBUTE_TYPE_NAME		L"Type"

#define XML_PARAMETER_NODE_NAME		L"Parameter"
#define XML_ROOT_NODE_NAME			L"ETS"

#define XML_LONG_TYPE_NAME			L"long"
#define XML_STRING_TYPE_NAME		L"string"


typedef std::vector<std::wstring> NodePathType;
//typedef std::wstring XMLWhiteSpaceType;

// CXMLLoader

class ATL_NO_VTABLE CXMLLoader : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CXMLLoader, &CLSID_XMLLoader>,
	public ISupportErrorInfo,
	public IDispatchImpl<IXMLLoader, &IID_IXMLLoader, &LIBID_ETSXMLParamsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CXMLLoader()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_XMLLOADER)


BEGIN_COM_MAP(CXMLLoader)
	COM_INTERFACE_ENTRY(IXMLLoader)
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

	STDMETHOD(Load)(BSTR bsFileName, IXMLParams** ppParams);
	STDMETHOD(Save)(BSTR bsFileName, IXMLParams* pParams);

private:

	HRESULT ParseXML(IXMLDOMNode* pXMLSettings);
	HRESULT CreateXML(IXMLDOMElement** ppXMLRoot);

	HRESULT GetParameter(IXMLDOMNode* pXMLSettings, bool *pbIsParameter);
	HRESULT SetParameter(IXMLDOMNode** ppXMLSettings, BSTR bsName, VARIANT vtValue);

	HRESULT GetNodePath(const BSTR bsFullName);
	HRESULT OpenNode(const BSTR bsNodeName, IXMLDOMNode* pCurrentNode, IXMLDOMNode** ppNewNode);

	HRESULT AttachParams(IXMLParams* pParams);

	void AddWhiteSpace(IXMLDOMNode* pNode);

private:

	CComPtr<IXMLParams>			m_spParams;
	CComPtr<IXMLDOMDocument>	m_spXMLDocument;
	NodePathType				m_CurrentNodePath;
	std::wstring				m_strWhiteSpaces;
};

OBJECT_ENTRY_AUTO(__uuidof(XMLLoader), CXMLLoader)

#endif //__XML_LOADER_H_
