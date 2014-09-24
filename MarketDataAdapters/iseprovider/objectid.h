#ifndef OBJECT_ID_H
#define OBJECT_ID_H

class CObjectID
{
	static DWORD m_dwUniqueID;
	static CLock m_LockUniqueID;

	DWORD m_dwID;

public:

	CObjectID()
	{
		CAutoLock Lock(&m_LockUniqueID);
		m_dwID = ++m_dwUniqueID;
	};

	DWORD GetID() const {return m_dwID;}
};

// class CISEStructureBase
class CISEStructureBase : 
    public IUnknown,
    public CObjectID
{
public:
    
	virtual HRESULT OnStock(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastStock ) = 0;
	virtual HRESULT OnOption(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastOption) = 0;
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, const CComVariant &varRequest) = 0;
    virtual HRESULT OnEvent(const EventNumberEnum EventNumber, _bstr_t Description) = 0;
};

// class CISEPriceBase
class CISEPriceBase : 
    public IUnknown,
    public CObjectID
{
public:
    
	virtual HRESULT OnLastQuote(const CComVariant &varParams, const CComVariant &varResults) = 0;
	virtual HRESULT OnQuoteUpdate(const CComVariant &varParams, const CComVariant &varResults) = 0;
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, const CComVariant &varRequest) = 0;
	virtual HRESULT	OnEvent(const EventNumberEnum EventNumber, _bstr_t Description) = 0;
};
    
#endif OBJECT_ID_H