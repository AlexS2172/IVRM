// RecPtr.h: interface for the CRecPtr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECPTR_H__5DEB3E90_C889_48A0_8837_8078521DC107__INCLUDED_)
#define AFX_RECPTR_H__5DEB3E90_C889_48A0_8837_8078521DC107__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class T, const GUID* plibid = &CComModule::m_libid, WORD wMajor = 1, WORD wMinor = 0>
class CComRecPtr  
{
public:
	CComRecPtr() : m_pRec (NULL)
	{
	}

	CComRecPtr( const CComRecPtr<T, plibid, wMajor, wMinor> &  spRec) : m_pRec (NULL)
	{
		m_spRecInfo = spRec.m_spRecInfo;
		if (m_spRecInfo && spRec.m_pRec)
			m_spRecInfo->RecordCreateCopy (spRec.m_pRec, (PVOID *) &m_pRec);
	}

	CComRecPtr(const VARIANT & vt)  : m_pRec (NULL)
	{
		if ( (vt.vt & VT_RECORD) == VT_RECORD  )
		{
			m_spRecInfo = vt.pRecInfo;
			if (m_spRecInfo  && vt.pvRecord  )
				m_spRecInfo->RecordCreateCopy (vt.pvRecord, (PVOID *) &m_pRec);
		}
		else
		{
			ATLASSERT (FALSE);
		}
	}

	virtual ~CComRecPtr()
	{
		ReleaseRecord();
	}

	HRESULT Init(const VARIANT & vt)
	{
		HRESULT hr =E_FAIL;

		if ( (vt.vt & VT_RECORD) == VT_RECORD  )
		{
			m_spRecInfo = NULL;
			m_spRecInfo = vt.pRecInfo;
			if (m_spRecInfo  && vt.pvRecord  )
				hr = m_spRecInfo->RecordCreateCopy (vt.pvRecord, (PVOID *) &m_pRec);
		}
		return hr;
	}

	HRESULT Init ()
	{
		HRESULT hr = ::GetRecordInfoFromGuids(*plibid, wMajor, wMinor, 
			::GetUserDefaultLCID(), __uuidof(T), &m_spRecInfo); 

		if (SUCCEEDED(hr))
		{
			m_pRec = (T*)	m_spRecInfo->RecordCreate();
			if (!m_pRec)
				hr = E_OUTOFMEMORY;
		}

		return hr;
	}
	
	HRESULT Init(T* pVal)
	{
		HRESULT hr = ::GetRecordInfoFromGuids(*plibid, wMajor, wMinor, 
			::GetUserDefaultLCID(), __uuidof(T), &m_spRecInfo); 

		if (SUCCEEDED(hr) && m_spRecInfo != NULL)
		{
			hr = m_spRecInfo->RecordCreateCopy(pVal, (PVOID *) &m_pRec);
			if (!m_pRec)
				hr = E_OUTOFMEMORY;
		}

		return hr;
	}

	HRESULT Clear()
	{
		HRESULT hr = S_OK;
		if (m_pRec)
		{
			hr = m_spRecInfo->RecordClear(m_pRec);
			if ( SUCCEEDED(hr) )
				hr = m_spRecInfo->RecordInit(m_pRec);
		}	
		return hr;
	}

	HRESULT CopyTo (VARIANT & rvt)
	{
		HRESULT hr = S_OK;
		::VariantClear(&rvt);
		if (m_pRec)
		{
			rvt.vt = VT_RECORD;
			m_spRecInfo.CopyTo (&rvt.pRecInfo);
			hr = m_spRecInfo->RecordCreateCopy (m_pRec, (PVOID *) &rvt.pvRecord);
		}

		return hr;
	}

	HRESULT CopyTo (T* pVal)
	{
		HRESULT hr = S_OK;
		if (m_pRec != 0 && pVal != 0)
		{
			hr = m_spRecInfo->RecordClear(pVal);
			hr = m_spRecInfo->RecordCopy(m_pRec, pVal);
		}

		return hr;
	}

	T* operator->() const
	{ 
		ATLASSERT (m_pRec);
		return m_pRec; 
	}

	operator T*() const
	{
		return m_pRec;
	}

	operator T&() const
	{
		return *m_pRec;
	}
	IRecordInfo* GetRecordInfo()
	{
		return m_spRecInfo;
	}

protected:
	void ReleaseRecord()
	{
		if (m_pRec)
		{
			m_spRecInfo->RecordDestroy(m_pRec);
			m_pRec = NULL;
		}

		m_spRecInfo = NULL;
	}

	CComPtr <IRecordInfo> m_spRecInfo;
	T * m_pRec;
};

#endif // !defined(AFX_RECPTR_H__5DEB3E90_C889_48A0_8837_8078521DC107__INCLUDED_)
