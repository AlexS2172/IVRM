// DateTimeFormat.h : Declaration of the CDateTimeFormat

#pragma once
#include "resource.h"       // main symbols

#include "EgFormat.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CDateTimeFormat

class ATL_NO_VTABLE CDateTimeFormat :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDateTimeFormat, &CLSID_DateTimeFormat>,
	public IDispatchImpl<IDateTimeFormat, &IID_IDateTimeFormat, &LIBID_EgFormatLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CDateTimeFormat()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DATETIMEFORMAT)


BEGIN_COM_MAP(CDateTimeFormat)
	COM_INTERFACE_ENTRY(IDateTimeFormat)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

public:
	STDMETHOD(egStrftime)(BSTR sFormat, BSTR* sRet);
};

OBJECT_ENTRY_AUTO(__uuidof(DateTimeFormat), CDateTimeFormat)
