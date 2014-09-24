#ifndef __RECORD_H__
#define __RECORD_H__

#include <guiddef.h>

template<class _T, const GUID* _pLIBID = &CComModule::m_libid, ULONG _uVerMajor = 1L, ULONG _uVerMinor = 0L>
class TRecord : public _T
{
public:
	static HRESULT GetRecord(IRecordInfo** ppri)
	{
		return ::GetRecordInfoFromGuids(*_pLIBID, _uVerMajor, _uVerMinor, ::GetUserDefaultLCID(), __uuidof(_T), ppri); 
	}

public:
	TRecord(IRecordInfo* pri = NULL)
	{
		::VariantInit(&m_vt);
		::memset((_T*)this, 0, sizeof(_T));

		if(pri)
		{
			pri->AddRef();
		}
		else
		{
			if(FAILED(GetRecord(&pri)))
			{
				m_vt.vt = VT_EMPTY;
				m_vt.pvRecord = NULL;
				m_vt.pRecInfo = NULL;
				return;
			}
		}

		m_vt.vt = VT_RECORD | VT_BYREF;
		m_vt.pvRecord = (void*)((_T*)this);
		m_vt.pRecInfo = pri;
	}

	virtual ~TRecord()
	{
		if(m_vt.pRecInfo)
		{
			m_vt.pRecInfo->Release();
		}
	}

	const TRecord& operator=(const VARIANT& vt)
	{
		Clear();
		Copy(vt);
	}

	void Zero()
	{
		memset(&m_vt, 0, sizeof(VARIANT));
		memset((_T *)this, 0, sizeof(_T));
	}
	
	void Copy(const VARIANT& vt)
	{
		::VariantInit(&m_vt);
		if(VT_RECORD & vt.vt)
		{
			m_vt.vt = VT_RECORD | VT_BYREF;
			vt.pRecInfo->AddRef();
			m_vt.pRecInfo = vt.pRecInfo;
			
			m_vt.pvRecord = (void*)((_T*)this);
			m_vt.pRecInfo->RecordCopy(vt.pvRecord, m_vt.pvRecord);
		}
	}

	void CopyTo(VARIANT* vt)
	{
		::VariantInit(vt);
		if(VT_RECORD & m_vt.vt)
		{
			vt->vt = VT_RECORD | VT_BYREF;
			m_vt.pRecInfo->AddRef();
			
			vt->pRecInfo = m_vt.pRecInfo;
			vt->pvRecord = vt->pRecInfo->RecordCreate();
			vt->pRecInfo->RecordCopy(m_vt.pvRecord, vt->pvRecord);
		}
	}

	void Clear()
	{
		if(m_vt.pRecInfo)
		{
			m_vt.pRecInfo->RecordClear(m_vt.pvRecord);
			m_vt.pRecInfo->Release();
		}
		Zero();
	}

	bool Valid(const VARIANT& vt)
	{
		bool bValid = false;
		HRESULT hr = E_INVALIDARG;
		if(vt.vt & VT_RECORD)
		{
			IRecordInfo * pri = vt.pRecInfo;
			if(pri && m_vt.pRecInfo)
			{
				GUID vtguid, myGuid;
				pri->GetGuid(&vtguid);

				m_vt.pRecInfo->GetGuid(&myGuid);

				if (0 == memcmp(&myGuid, &vtguid, sizeof(GUID)))
				{
					bValid = (NULL != vt.pvRecord);
				}
			}
		}
		return bValid;
	}

	bool Attach(const VARIANT& vt)
	{
		if(Valid(vt))
		{
			memcpy((_T *)this, vt.pvRecord, sizeof(_T));
			return true;
		}
		return false;
	}

	void Detach()
	{
		memset((_T *)this, 0, sizeof(_T));
	}

	operator const VARIANT() { return m_vt; }
	operator const VARIANT& () { return m_vt; }
	operator const VARIANT* () { return &m_vt; }
	operator const IRecordInfo* () { return m_vt.pRecInfo; }

	operator VARIANT() { return m_vt; }
	operator VARIANT& () { return m_vt; }
	operator VARIANT* () { return &m_vt; }
	operator IRecordInfo* () { return m_vt.pRecInfo; }

protected:
	VARIANT m_vt;
};

#endif	//__RECORD_H__