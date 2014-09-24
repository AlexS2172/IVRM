// VolaMgmtInstance.h: interface for the CVolaMgmtInstance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VOLAMGMTINSTANCE_H__55B4CA3D_4FAB_4566_88B6_10D315EE5D7C__INCLUDED_)
#define AFX_VOLAMGMTINSTANCE_H__55B4CA3D_4FAB_4566_88B6_10D315EE5D7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// {D885704B-2CD2-4260-86FC-54297135E830}
DEFINE_GUID(CLSID_VolaMgmtInstance, 
0xd885704b, 0x2cd2, 0x4260, 0x86, 0xfc, 0x54, 0x29, 0x71, 0x35, 0xe8, 0x30);


class ATL_NO_VTABLE CVolaMgmtInstance:  
	public CComObjectRootEx<CComSingleThreadModel>,
	public IExternalConnection
{
public:
	CVolaMgmtInstance()
	{
	}
	~CVolaMgmtInstance()
	{
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()
DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct ()
	{
		HRESULT hr = m_spUnknownAgg.CoCreateInstance (VME::CLSID_VolatilityManagement,  GetControllingUnknown());
		return hr;
	}

	void FinalRelease ()
	{
		if (m_spUnknownAgg)
		{
			m_spUnknownAgg = NULL;
		}
	}

BEGIN_COM_MAP(CVolaMgmtInstance)
	COM_INTERFACE_ENTRY (IExternalConnection)
	COM_INTERFACE_ENTRY_AGGREGATE (__uuidof (VME::IVolatilityManagement), m_spUnknownAgg.p)
	COM_INTERFACE_ENTRY_AGGREGATE (IID_ISupportErrorInfo, m_spUnknownAgg.p)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IConnectionPointContainer, m_spUnknownAgg.p)
END_COM_MAP()

	CComPtr <IUnknown> m_spUnknownAgg;
	long m_nConnections;
};

////////////////////////////////


#endif // !defined(AFX_VOLAMGMTINSTANCE_H__55B4CA3D_4FAB_4566_88B6_10D315EE5D7C__INCLUDED_)
