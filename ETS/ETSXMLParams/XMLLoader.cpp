// XMLLoader.cpp : Implementation of CXMLLoader

#include "stdafx.h"
#include "XMLLoader.h"

#include <iostream>

// CXMLLoader

STDMETHODIMP CXMLLoader::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IXMLLoader
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CXMLLoader::Load(BSTR bsFileName, IXMLParams** ppParams)
{
	try
	{
		__CHECK_POINTER(ppParams);

		m_spParams.Release();
		m_spXMLDocument.Release();
		m_CurrentNodePath.clear();
		_CHK(m_spParams.CoCreateInstance(CLSID_XMLParams), _T("Failed to load xml"));
		_CHK(m_spXMLDocument.CoCreateInstance(CLSID_DOMDocument), _T("Failed to load xml"));

		CComVariant svtFileName = bsFileName;
		VARIANT_BOOL bIsSuccessful = false;
		_CHK(m_spXMLDocument->load(svtFileName, &bIsSuccessful), _T("Failed to load xml"));
		if(!bIsSuccessful)
			EgLib::CComErrorWrapper::ThrowError(E_FAIL, _T("Failed to load xml"));

		CComPtr<IXMLDOMElement> spRoot;
		_CHK(m_spXMLDocument->get_documentElement(&spRoot), _T("Failed to load xml"));

		CComBSTR sbsRootName;
		_CHK(spRoot->get_nodeName(&sbsRootName), _T("Failed to load xml"));
		if(sbsRootName != CComBSTR(XML_ROOT_NODE_NAME))
			EgLib::CComErrorWrapper::ThrowError(E_UNEXPECTED, _T("Wrong root element"));

		_CHK(ParseXML(spRoot), _T("Failed to load xml"));
		*ppParams = m_spParams.Detach();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IXMLLoader, e.Error());
	}

	return S_OK;
}


STDMETHODIMP CXMLLoader::Save(BSTR bsFileName, IXMLParams* pParams)
{
	try
	{
		__CHECK_POINTER(pParams);

		CComPtr<IXMLParams> spOldParams;
		if(SUCCEEDED(Load(bsFileName, &spOldParams)))
		{
			m_spParams = spOldParams;
			_CHK(AttachParams(pParams), _T("Failed to save xml"));
		}
		else
			m_spParams = pParams;
		
		m_CurrentNodePath.clear();
		m_spXMLDocument.Release();
		_CHK(m_spXMLDocument.CoCreateInstance(CLSID_DOMDocument), _T("Failed to save xml"));

		CComPtr<IXMLDOMElement> spRoot;
		_CHK(CreateXML(&spRoot), _T("Failed to save xml"));

		m_strWhiteSpaces = L"\n";
		AddWhiteSpace(spRoot);

		_CHK(m_spXMLDocument->putref_documentElement(spRoot), _T("Failed to save xml"));
		CComVariant svtFileName = bsFileName;
		_CHK(m_spXMLDocument->save(svtFileName), _T("Failed to save xml"));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IXMLLoader, e.Error());
	}

	return S_OK;
}


HRESULT CXMLLoader::ParseXML(IXMLDOMNode* pXMLSettings)
{
	try
	{
		CComBSTR sbsNodeName;
		_CHK(pXMLSettings->get_nodeName(&sbsNodeName));
		m_CurrentNodePath.push_back((BSTR)sbsNodeName);

		CComPtr<IXMLDOMNodeList> spChildes;
		_CHK(pXMLSettings->get_childNodes(&spChildes));

		long nCount = 0;
		_CHK(spChildes->get_length(&nCount));

		for(long i = 0; i < nCount; ++i)
		{
			CComPtr<IXMLDOMNode> spCurrentElement;
			_CHK(spChildes->get_item(i, &spCurrentElement));

			bool bIsParameter = false;
			_CHK(GetParameter(spCurrentElement, &bIsParameter));
			if(!bIsParameter)
				_CHK(ParseXML(spCurrentElement));
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IXMLLoader, e.Error());
	}
	
	m_CurrentNodePath.pop_back();

	return S_OK;
}


HRESULT CXMLLoader::CreateXML(IXMLDOMElement** ppXMLRoot)
{
	try
	{
		CComPtr<IXMLDOMElement> spRoot;
		_CHK(m_spXMLDocument->createElement(XML_ROOT_NODE_NAME, &spRoot));

		long nCount = 0;
		_CHK(m_spParams->get_Count(&nCount));

		for(long i = 0; i < nCount; ++i)
		{
			CComVariant svtValue;
			CComBSTR sbsFullName;
			_CHK(m_spParams->get_Item(i, &sbsFullName, &svtValue, NULL));
			_CHK(GetNodePath(sbsFullName));

			CComPtr<IXMLDOMNode> spCurrentNode = spRoot;
			for(size_t i = 1; i < m_CurrentNodePath.size() - 1; ++i)
			{
				CComPtr<IXMLDOMNode> spNextNode;
				_CHK(OpenNode((BSTR)(m_CurrentNodePath[i].c_str()), spCurrentNode, &spNextNode));
				spCurrentNode = spNextNode.Detach();
			}

			CComPtr<IXMLDOMNode> spXMLParameter;
			_CHK(SetParameter(&spXMLParameter,
							  (BSTR)(m_CurrentNodePath[m_CurrentNodePath.size()-1].c_str()),
							  svtValue ));

			CComPtr<IXMLDOMNode> spNewXMLParameter;
			_CHK(spCurrentNode->appendChild(spXMLParameter, &spNewXMLParameter));
		}

		*ppXMLRoot = spRoot.Detach();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IXMLLoader, e.Error());
	}

	return S_OK;
}



HRESULT CXMLLoader::GetParameter(IXMLDOMNode* pXMLSettings, bool *pbIsParameter)
{
	try
	{
		*pbIsParameter = false;

		CComQIPtr<IXMLDOMElement> spXMLSettings = pXMLSettings;
		if(!spXMLSettings)
			return S_OK;

		CComBSTR sbsNodeName;
		_CHK(spXMLSettings->get_nodeName(&sbsNodeName));
		if(sbsNodeName != CComBSTR(XML_PARAMETER_NODE_NAME))
			return S_OK;

		CComVariant svtParamName;
		_CHK(spXMLSettings->getAttribute(XML_ATTRIBUTE_NAME_NAME, &svtParamName));

		CComVariant svtParamType;
		_CHK(spXMLSettings->getAttribute(XML_ATTRIBUTE_TYPE_NAME, &svtParamType));

		CComVariant svtParamValue;
		_CHK(spXMLSettings->getAttribute(XML_ATTRIBUTE_VALUE_NAME, &svtParamValue));

		CComBSTR sbsType;
		_CHK(svtParamType.CopyTo(&sbsType));

		CComBSTR sbsName;
		_CHK(svtParamName.CopyTo(&sbsName));

		CComBSTR sbsValue;
		_CHK(svtParamValue.CopyTo(&sbsValue));

		CComBSTR sbsFullPath;
		for(size_t i = 0; i < m_CurrentNodePath.size(); ++i)
		{
			sbsFullPath += m_CurrentNodePath[i].c_str();
			sbsFullPath += L"\\";
		}
		sbsFullPath += sbsName;

		if(sbsType == CComBSTR(XML_LONG_TYPE_NAME))
		{
			svtParamValue = _wtol((BSTR)sbsValue);
			//_CHK(m_spParams->SetLongValue(sbsFullPath, sbsName, _wtol((BSTR)sbsValue)));
			_CHK(m_spParams->Add(sbsFullPath, svtParamValue));
			*pbIsParameter = true;
		}
		
		if(sbsType == CComBSTR(XML_STRING_TYPE_NAME))
		{
			svtParamValue = sbsValue;
			//_CHK(m_spParams->SetStringValue(sbsFullPath, sbsName, sbsValue));
			_CHK(m_spParams->Add(sbsFullPath, svtParamValue));
			*pbIsParameter = true;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IXMLLoader, e.Error());
	}

	return (*pbIsParameter = true) ? S_OK : E_FAIL;
}

HRESULT CXMLLoader::SetParameter(IXMLDOMNode** ppXMLSettings, BSTR bsName, VARIANT vtValue)
{
	try
	{
		CComPtr<IXMLDOMElement> spXMLParameter;
		_CHK(m_spXMLDocument->createElement(XML_PARAMETER_NODE_NAME, &spXMLParameter));

		CComVariant svtAttributeValue = bsName;
		_CHK(spXMLParameter->setAttribute(XML_ATTRIBUTE_NAME_NAME, svtAttributeValue));

		switch(vtValue.vt)
		{
		case VT_I4:
			svtAttributeValue = XML_LONG_TYPE_NAME;
			break;

		case VT_BSTR:
			svtAttributeValue = XML_STRING_TYPE_NAME;
			break;

		default:
			return E_UNEXPECTED;
		}
		_CHK(spXMLParameter->setAttribute(XML_ATTRIBUTE_TYPE_NAME, svtAttributeValue));

		svtAttributeValue = vtValue;
		_CHK(spXMLParameter->setAttribute(XML_ATTRIBUTE_VALUE_NAME, svtAttributeValue));

		*ppXMLSettings = spXMLParameter.Detach();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IXMLLoader, e.Error());
	}

	return S_OK;
}


HRESULT CXMLLoader::GetNodePath(const BSTR bsFullName)
{
	std::wstring wstrFullName = bsFullName;
	m_CurrentNodePath.clear();

	std::wstring wstrNodeName;
	size_t end_pos = wstrFullName.npos;
	size_t start_pos = 0;
	while((end_pos = wstrFullName.find(L"\\", start_pos)) != wstrFullName.npos)
	{
		wstrNodeName = wstrFullName.substr(start_pos, end_pos - start_pos);
		m_CurrentNodePath.push_back(wstrNodeName);
		start_pos = end_pos + 1;
	}

	end_pos = wstrFullName.size();
	wstrNodeName = wstrFullName.substr(start_pos, end_pos - start_pos);
	m_CurrentNodePath.push_back(wstrNodeName);

	return S_OK;
}

HRESULT CXMLLoader::OpenNode(const BSTR bsNodeName, IXMLDOMNode* pCurrentNode, IXMLDOMNode** ppNewNode)
{
	try
	{
		CComPtr<IXMLDOMNodeList> spChildes;
		_CHK(pCurrentNode->get_childNodes(&spChildes));

		long nCount = 0;
		_CHK(spChildes->get_length(&nCount));

		for(long i = 0; i < nCount; ++i)
		{
			CComPtr<IXMLDOMNode> spCurrentChild;
			_CHK(spChildes->get_item(i, &spCurrentChild));

			CComBSTR sbsChildName;
			_CHK(spCurrentChild->get_nodeName(&sbsChildName));
			if(sbsChildName == CComBSTR(bsNodeName))
			{
				*ppNewNode = spCurrentChild.Detach();
			}
		}

		if(!(*ppNewNode))
		{
			CComPtr<IXMLDOMElement> spNewNode;
			_CHK(m_spXMLDocument->createElement(bsNodeName, &spNewNode));

			CComPtr<IXMLDOMNode> spNewChild;
			_CHK(pCurrentNode->appendChild(spNewNode, &spNewChild));

			*ppNewNode = spNewChild.Detach();
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IXMLLoader, e.Error());
	}

	return S_OK;
}

HRESULT CXMLLoader::AttachParams(IXMLParams* pParams)
{
	try
	{
		long nCount = 0;
		_CHK(pParams->get_Count(&nCount));

		for(long i = 0; i < nCount; ++i)
		{
			CComVariant svtValue;
			CComBSTR sbsFullName;
			VARIANT_BOOL bIsChanged;
			_CHK(pParams->get_Item(i, &sbsFullName, &svtValue, &bIsChanged));
			if(bIsChanged)
				_CHK(m_spParams->Add(sbsFullName, svtValue));
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IXMLLoader, e.Error());
	}

	return S_OK;
}

void CXMLLoader::AddWhiteSpace(IXMLDOMNode* pNode)
{
	try
	{
		m_strWhiteSpaces += L"\t";

		CComPtr<IXMLDOMNodeList> spChildes;
		_CHK(pNode->get_childNodes(&spChildes));

		long nCount = 0;
		_CHK(spChildes->get_length(&nCount));
		if(nCount == 0)
		{
			m_strWhiteSpaces.erase(m_strWhiteSpaces.end() - 1);
			return;
		}
		
		CComPtr<IXMLDOMNode> spCurrentElement;
		_CHK(spChildes->nextNode(&spCurrentElement));
		while(spCurrentElement)
		{
			DOMNodeType NodeType;
			_CHK(spCurrentElement->get_nodeType(&NodeType));
	
			if(NodeType == NODE_ELEMENT)
			{			
				CComBSTR bsText(m_strWhiteSpaces.c_str());
				CComPtr<IXMLDOMText> spXMLText;
				_CHK(m_spXMLDocument->createTextNode(bsText, &spXMLText));

				CComPtr<IXMLDOMNode> spXMLDummy;
				CComVariant svtValue = spCurrentElement;
				_CHK(pNode->insertBefore(spXMLText, svtValue, &spXMLDummy));
			}
			
			AddWhiteSpace(spCurrentElement);

			spCurrentElement.Release();
			_CHK(spChildes->nextNode(&spCurrentElement));
		}

		m_strWhiteSpaces.erase(m_strWhiteSpaces.end() - 1);

		CComBSTR bsText(m_strWhiteSpaces.c_str());
		CComPtr<IXMLDOMText> spXMLText;
		_CHK(m_spXMLDocument->createTextNode(bsText, &spXMLText));

		CComPtr<IXMLDOMNode> spXMLDummy;
		_CHK(pNode->appendChild(spXMLText, &spXMLDummy));
	}
	catch(const _com_error&)
	{}

	return;
}