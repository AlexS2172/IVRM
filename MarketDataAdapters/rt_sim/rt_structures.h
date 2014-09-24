#ifndef _RT_STRUCTURES_H
#define _RT_STRUCTURES_H

// {A685DAA6-09CE-4a45-8E5D-C5EF63DF7E0C}
static const GUID guid_Buffer = 
{ 0xa685daa6, 0x9ce, 0x4a45, { 0x8e, 0x5d, 0xc5, 0xef, 0x63, 0xdf, 0x7e, 0xc } };

struct CRespBuffer
{
	CRespBuffer(){ZeroMemory(this,sizeof(CRespBuffer));};

	bool Initialize(RT_FIELD fld, RT_FIELD_TYPE fldType, _variant_t& vtData)
	{
		m_rtField = fld;
		m_fldType = fldType;
		switch(m_fldType)
		{
		case RT_CHAR://                , // Null-terminated character string          
			strncpy((char*)m_buffer, (LPCSTR)_bstr_t(vtData),99);
			break;
		case RT_INT32://               , // 32-bit signed integer                     
			{
				long lData = (long) vtData;
				*((long*)m_buffer) = lData;
			}
				break;
		case RT_UINT32://              , // 32-bit unsigned integer                   
			{
				ULONG ulData = (long)vtData;
				*((ULONG*)m_buffer) = ulData;
			}
				break;
		case RT_REAL://                , // Double-precision floating point           
			{
				double fData = (double)vtData;
				*((double*)m_buffer) = fData;
			}
			break;
		case RT_TIME://                , // Time-of-day (hh:mm), in 24-hour time.     
			{
				CComVariant vt(vtData);
				vt.ChangeType(VT_DATE);
				EgLib::vt_date date(V_DATE(&vt));
				sprintf((LPSTR)m_buffer, "%02d:%02d", date.get_hour(), date.get_minute());

			}break;
		case RT_DATE://                  // Date (mm/dd/yy). Format will be adjusted  
			{
				CComVariant vt(vtData);
				vt.ChangeType(VT_DATE);
				_bstr_t bsDate = vt;			
				strncpy((char*)m_buffer, (LPCSTR)bsDate,99);
			}break;
		}
		return true;
	}
	RT_FIELD      m_rtField;
	RT_FIELD_TYPE m_fldType;
	BYTE          m_buffer[100];            
};

class CRespStructure
{
public:
	CRespStructure()
	{
		ZeroMemory(this, sizeof(CRespStructure));
		m_GUID = guid_Buffer;
	}


	~CRespStructure()
	{
		clear();
	}

	void clear()
	{
		m_lCount  = 0;
		if(m_pBuffer) delete[] m_pBuffer;
		if(m_pRawBuffer) delete m_pRawBuffer;
		m_pBuffer = NULL;
	}

	bool Initialize(int Num)
	{
		m_lCount = 0;
		if(m_pBuffer)
			delete[] m_pBuffer;
		m_pBuffer = new CRespBuffer[Num];
		if(m_pBuffer){
			m_lCount = Num;
			return true;
		}
		return false;
	}
	CRespStructure& operator=(const CRespStructure& resp)
	{
		clear();
		m_lCount = resp.m_lCount;
		m_pBuffer = new CRespBuffer[m_lCount];
		if(m_pBuffer)
			memcpy(m_pBuffer, resp.m_pBuffer, sizeof(CRespBuffer)*m_lCount);
		return *this;
	}

	void putData(long iPos, const CRespBuffer& pData)
	{
		if(iPos<m_lCount)
			memcpy(&m_pBuffer[iPos], &pData, sizeof(CRespBuffer));
	}
	CRespBuffer* getData(long iPos)
	{
		if(iPos>=m_lCount) return NULL;
		else return &m_pBuffer[iPos];
	}
	long getBufferSize()
	{
		return sizeof(long)+sizeof(GUID)+sizeof(CRespBuffer)*m_lCount;
	}
	long getFieldNum(){ return m_lCount;};
	RT_RAW_FIELD_BUF* getBuffer(){ 
		if(!m_pRawBuffer)
		{
			m_pRawBuffer = new RT_RAW_FIELD_BUF[getBufferSize()];
			RT_RAW_FIELD_BUF* pData = m_pRawBuffer;
			*((long*)pData) = m_lCount;
			pData += sizeof(long);
			memcpy(pData, &m_GUID, sizeof(GUID));
			pData +=sizeof(GUID);
			memcpy(pData, m_pBuffer, sizeof(CRespBuffer)*m_lCount);
		}
		return m_pRawBuffer;
	};

private:
	long		 m_lCount;
	CRespBuffer* m_pBuffer;

	RT_RAW_FIELD_BUF* m_pRawBuffer;
public:
	GUID		 m_GUID;

};


class CSymbol
{
public:
	EgLib::CLock m_cs;
	long                m_lID;
	long                m_lParentID;
	long				m_lMarketID;
	bool				m_bSubscribed;
	_bstr_t				m_szName;
	_bstr_t             m_szExchange;  
	_bstr_t				m_szDescription;

	enum enSymbolType { enNone=-1, enIndex=0, enStock=1, enOptCall=2, enOptPut=3} m_enType;

	double				m_Strike;
	double				m_dPrice;
	long                m_lVolume;
	long                m_lSumVolume;
	double              m_dOpenPrice;
	double              m_dLowPrice;
	double              m_dHighPrice;

	double				m_dBid;
	double				m_dAsk;
	double              m_dClose;
	double				m_lBidSize;
	double				m_lAskSize;
	double              m_dNetchange;
	int                 m_type;

	DATE				m_dtLastOperation;
	DATE                m_dtExpDate;

	CSymbol(){
		m_type = 0;
		m_enType = enNone;
		m_lMarketID = 0;
		m_lID = 0; 
		m_dPrice=0.; 
		m_szName =_T(""); 
		m_szDescription = _T("");
		m_szExchange = _T("");
		m_lVolume = 0;
		m_lSumVolume = 0;
		m_dOpenPrice = 0;
		m_dtLastOperation = 0;
		m_dBid = 0.;
		m_dClose = 0.;
		m_dAsk = 0.;
		m_lBidSize = 0;
		m_lAskSize = 0;
		m_dLowPrice = 0.;
		m_dHighPrice = 0.;
		m_bSubscribed = false;
		m_lParentID = 0;
		m_Strike = 0;
		m_dtExpDate = 0;
		m_dNetchange = 0.;
	}

	CSymbol(const CSymbol& symb){ Copy(symb);}
	CSymbol& operator=(const CSymbol& symb)	{ Copy(symb); return *this; }

private:
	void Copy(const CSymbol& symb)
	{
		m_lID = symb.m_lID;
		m_lMarketID = symb.m_lMarketID;
		m_szExchange = symb.m_szExchange;
		m_dPrice = symb.m_dPrice;
		m_szName = symb.m_szName;
		m_szDescription = symb.m_szDescription;
		m_enType = symb.m_enType;
		m_lVolume = symb.m_lVolume;
		m_lSumVolume = symb.m_lSumVolume;
		m_dOpenPrice = symb.m_dOpenPrice;
		m_dtLastOperation = symb.m_dtLastOperation;
		m_dBid = symb.m_dBid;
		m_dAsk = symb.m_dAsk;
		m_lBidSize = symb.m_lBidSize;
		m_lAskSize = symb.m_lAskSize;
		m_dLowPrice = symb.m_dLowPrice;
		m_dHighPrice = symb.m_dHighPrice;
		m_bSubscribed = symb.m_bSubscribed;
		m_lParentID = symb.m_lParentID;
		m_Strike = symb.m_Strike;
		m_dClose = symb.m_dClose;
		m_dtExpDate = symb.m_dtExpDate;
		m_type = symb.m_type;
		m_dNetchange = symb.m_dNetchange;
	}
};

typedef map<RT_REQ_ID, LPVOID> RTUSERDATA;

class CRequest
{
public:
	RT_HANDLE m_Handle;
	RT_SYMBOL m_Symbol;
	RT_REQ_TYPE m_Type;
	RT_GRP_REQ_TY m_groupType;
	_bstr_t m_bsSymbolName;
	
	RTUSERDATA m_UserData;
	RT_GET_FIELDS_CLBK    m_pNotifier;
	RT_GET_GROUP_CLBK     m_pGroupNotifier;

	CRequest():m_pFields(NULL){Clear();}
	
	CRequest(const CRequest& req):m_pFields(NULL)
	{
		Clear();
		m_Handle      = req.m_Handle;

		{
			for(RTUSERDATA::const_iterator iter = req.m_UserData.begin(); iter!=req.m_UserData.end();iter++)
			{
				m_UserData[iter->first]=iter->second;
			}
		}

		m_pNotifier   = req.m_pNotifier;
		m_pGroupNotifier = req.m_pGroupNotifier;
		m_Type		  = req.m_Type;	
		m_groupType   = req.m_groupType;
		m_bsSymbolName = req.m_bsSymbolName;
		
//		if(!m_Handle)
		memcpy(&m_Symbol, &req.m_Symbol,sizeof(RT_SYMBOL));

		CopyFields(req.m_FieldsCount, req.m_pFields);
	}

	~CRequest(){ Clear(); }
	void Clear()
	{
		m_Handle      = 0;

		m_UserData.clear();

		m_pNotifier   = NULL;
		m_Type = RT_REQ_TYPE_GET;
		m_groupType = RT_GRP_REQ_TY_THIS;
		m_bsSymbolName = _T("");

		ZeroMemory(&m_Symbol,sizeof(RT_SYMBOL));
		ClearFields();
	}
	
	void ClearFields()
	{
		m_FieldsCount = 0;
		if(m_pFields)
			delete[] m_pFields;
		m_pFields = NULL;
	};

	CRequest& operator=(const CRequest& req)
	{
		Clear();
		m_Handle      = req.m_Handle;


		for(RTUSERDATA::const_iterator iter = req.m_UserData.begin(); iter!=req.m_UserData.end();iter++)
		{
			m_UserData[iter->first]=iter->second;
		}

		m_pNotifier   = req.m_pNotifier;
		m_Type		  = req.m_Type;
		m_groupType	  = req.m_groupType;
		m_bsSymbolName = req.m_bsSymbolName;

		
		memcpy(&m_Symbol, &req.m_Symbol, sizeof(RT_SYMBOL));

		CopyFields(req.m_FieldsCount, req.m_pFields);
		return *this;
	}

	bool AddUserData(RT_REQ_ID ID, LPVOID pUserData)
	{
		bool bFound = false;
		for(RTUSERDATA::iterator iter = m_UserData.begin(); iter!=m_UserData.end(); iter++)
			if(iter->second == pUserData)
			{
				bFound = true;
				break;
			}

		if(!bFound )
			m_UserData[ID] = pUserData;

		return !bFound;
	}
	
	void CopyFields(int nFields, RT_FIELD* pData)
	{
		 ClearFields();
		 if(nFields && pData)
		 {
			 m_pFields = new RT_FIELD[nFields];
			 if(m_pFields)
			 {
				 memcpy(m_pFields, pData, nFields*sizeof(RT_FIELD));
				 m_FieldsCount = nFields;
			 }
		 }
	}

	int  GetFieldCount(){return m_FieldsCount;}
	RT_FIELD* GetFields(){ return m_pFields;}

	RT_FIELD GetField(int iPos){
		if(iPos<0 || iPos >= m_FieldsCount) 
			return RT_NUMBER_OF_FIELDS;
		else 
			return m_pFields[iPos];
	}

	bool Initialize(long dwID, RT_GRP_REQ_TY type, void* pUserData, RT_HANDLE handle, RT_GET_GROUP_CLBK callback)
	{
		Clear();
		if(dwID)
		{	
			AddUserData(reinterpret_cast<RT_REQ_ID>(dwID), pUserData);

			m_Handle = handle;
			m_pGroupNotifier = callback;
			m_groupType = type;
			return true;
		}
		return false;
	}
	bool Initialize(long dwID, RT_REQ_TYPE type, LPVOID pUserData, RT_SYMBOL* pSymbol, int field_count, RT_FIELD* pFields, RT_GET_FIELDS_CLBK pFunct)
	{
		Clear();
		if(dwID)
		{
			AddUserData(reinterpret_cast<RT_REQ_ID>(dwID), pUserData);

			m_pNotifier   = pFunct;
			m_Type = type;

			memcpy(&m_Symbol, pSymbol,sizeof(RT_SYMBOL));
			CopyFields(field_count, pFields);			
			return true;
		}
		return false;
	}
	bool Initialize(long dwID, RT_REQ_TYPE type, LPVOID pUserData, RT_HANDLE handle, int field_count, RT_FIELD* pFields, RT_GET_FIELDS_CLBK pFunct)
	{
		Clear();
		if(dwID)
		{
			AddUserData(reinterpret_cast<RT_REQ_ID>(dwID), pUserData);

			m_pNotifier   = pFunct;
			m_Type = type;
			m_Handle = handle;

			CopyFields(field_count, pFields);			
			return true;
		}
		return false;
	}

private:
	int		  m_FieldsCount;
	RT_FIELD* m_pFields;
};

class CSymbolResponce
{
public:
	CRespStructure m_Responce;
	CRequest       m_Request;
	RT_ERR         m_enErrorType;

	CSymbolResponce()
	{
		m_enErrorType = RT_ERR_OK;
	}

	CSymbolResponce(const CSymbolResponce& resp)
	{
		m_enErrorType = resp.m_enErrorType;
		m_Responce = resp.m_Responce;
		m_Request = resp.m_Request;


	}
	CSymbolResponce& operator=(const CSymbolResponce& resp)
	{
		m_enErrorType = resp.m_enErrorType;
		m_Request = resp.m_Request;
		m_Responce = resp.m_Responce;
		return *this;
	}

};


#endif //_RT_STRUCTURES_H