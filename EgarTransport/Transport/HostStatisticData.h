// HostStatisticData.h : Declaration of the CHostStatisticData

#ifndef __HOSTSTATISTICDATA_H_
#define __HOSTSTATISTICDATA_H_

#include "resource.h"       // main symbols
_COM_SMARTPTR_TYPEDEF(IHostStatisticData, __uuidof(IHostStatisticData));

/////////////////////////////////////////////////////////////////////////////
// CHostStatisticData
class ATL_NO_VTABLE CHostStatisticData : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CHostStatisticData, &CLSID_HostStatisticData>,
	public IDispatchImpl<IHostStatisticData, &IID_IHostStatisticData, &LIBID_TRANSPORTLib>
{
public:
	CHostStatisticData(): m_ulReceivedMessage(0),m_ulSentMessage(0),
		m_ulConfirmedMessage(0),m_ulUnsentMessage(0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HOSTSTATISTICDATA)
DECLARE_NOT_AGGREGATABLE(CHostStatisticData)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHostStatisticData)
	COM_INTERFACE_ENTRY(IHostStatisticData)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IHostStatisticData
public:
	STDMETHOD(get_LogonTime)(/*[out, retval]*/ DATE *pVal);
	STDMETHOD(put_LogonTime)(/*[in]*/ DATE newVal);
	STDMETHOD(get_UnsentMessageNum)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_UnsentMessageNum)(/*[in]*/ long newVal);
	STDMETHOD(get_SentMessageNum)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_SentMessageNum)(/*[in]*/ long newVal);
	STDMETHOD(get_ReceivedMessageNum)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_ReceivedMessageNum)(/*[in]*/ long newVal);
	STDMETHOD(get_ConfirmedMessageNum)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_ConfirmedMessageNum)(/*[in]*/ long newVal);
	STDMETHOD(get_HostName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_HostName)(/*[in]*/ BSTR newVal);

public:
	unsigned long	m_ulReceivedMessage;
	unsigned long	m_ulSentMessage;
	unsigned long	m_ulConfirmedMessage;
	unsigned long	m_ulUnsentMessage;
	DATE			m_dtLogonTime;
	_bstr_t         m_bsHostData;
};

#endif //__HOSTSTATISTICDATA_H_
