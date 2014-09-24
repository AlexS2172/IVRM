#pragma once


typedef  struct VSData
{
	CLSID	m_clsid;
	_bstr_t m_Title;
	_bstr_t m_FileName;
	_bstr_t m_Version;
	_bstr_t m_ProgID;

	VSData& operator=(const VSData& src)
	{
		m_clsid = src.m_clsid ;
		m_Title = src.m_Title.copy();
		m_FileName = src.m_FileName.copy();
		m_Version = src.m_Version.copy();
		m_ProgID = src.m_ProgID.copy();
		return *this;
	};

}VSData;

class CVSDataStorage:protected std::map<long, VSData>
{
public:
	CVSDataStorage();
	~CVSDataStorage(){clear();};
	long GetSize(){ CAutoLock cs(m_csDataAccess);return (long)size();};
	bool GetItembyID(long ID, VSData * pData);
	bool GetItembyIndex(long index, long *ID, VSData * pData);
//	HRESULT GetItembyIndex(long index, CLSID* pClsid);
//	HRESULT GetCurrentSource(CLSID* pClsid);
//	HRESULT GetCurrentSource(long* pID);
//	HRESULT SetCurrentSource(long ID);
	void Reload();
	bool IsInitialized(){ CAutoLock cs(m_csDataAccess);return m_bInitialized;};
private:
	void LoadDefaultItem();
	bool FillFileNameAndVersionW(VSData* pData);
	bool FillFileNameAndVersionA(VSData* pData);
//	long m_lCurrentID;
	bool m_bInitialized;
	CLock m_csDataAccess;
} ;
// CVolatilitySources
