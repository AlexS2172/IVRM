#pragma once
#include "FixBuffer.h"

#define STR_HEADER_VERSION "1.00"

[
	uuid(EFBAF35E-64D9-49bc-B1C8-B633554773C2),
	export

]
struct TNTData
{
	[helpstring("TradeID (7 characters)")]		BSTR			TradeID;
	[helpstring("isBuy")]						VARIANT_BOOL	isBuy;
	[helpstring("TradeDateTime")]				DATE			TradeDateTime;
	[helpstring("TradeQuantity")]				LONG			TradeQuantity;
	[helpstring("isCall")]						VARIANT_BOOL	isCall;
	[helpstring("OptionSymbol (6 characters)")]	BSTR			OptionSymbol;
	[helpstring("OptionExpDate")]				DATE			OptionExpDate;
	[helpstring("Strike")]						DOUBLE			Strike;
	[helpstring("PremiumPrice")]				DOUBLE			PremiumPrice;
	[helpstring("ExecutingFirm (4 characters)")]BSTR			ExecutingFirm;
	[helpstring("ExecutingMember (4 characters)")]	BSTR		ExecutingMember;
	[helpstring("CMTA (4 characters)")]			BSTR			CMTA;
	[helpstring("Quantifier (1 character)")]	BSTR			Quantifier;
	[helpstring("ContraFirm (4 characters)")]   BSTR			ContraFirm;
	[helpstring("ContraMember (4 characters)")] BSTR			ContraMember;	
	[helpstring("SpreadIndicator(1 character)")]BSTR			SpreadIndicator;
	[helpstring("OptionalData (13 characters)")]BSTR			OptionalData;
};


[
	export,
	uuid(41A53476-5F42-4cba-89DE-7A144DA23BED),
	v1_enum,
	helpstring("Error Code"),
]
enum MmhtErrorCodeEnum
{
	enMmhtErNoError = 0,					//0
//	enMmhtErNoError2 = 32,					//' '
	enMmhtErSequenceGap = 49,				//'1'
	enMmhtErDisabledOrNotRegistered = 50,	//'2'
	enMmhtErUnsupportedFMMC = 51,			//'3'
	enMmhtErDuplicateSequenceNum = 52,		//'4'
	enMmhtErUnknownError = 53,				//'5'
	enMmhtErNotLoggetIn = 54,				//'6'
	enMmhtErSequenceNumNotNumeric = 55,		//'7'
	enMmhtErTradeDateNotInvalid = 65,		//'A'
	enMmhtErTradeDateNotCurrentDate = 66,	//'B'
	enMmhtErTradeTimeInvalid = 67,			//'C'
	enMmhtErTradeTimeOutsideTradeHours = 68,//'D'
	enMmhtErTradeTimeExceedPOETSTime = 69,	//'E'
	enMmhtErFirmInvalid = 70,				//'F'
	enMmhtErExecMemberInvalid = 71,			//'G'
	enMmhtErContraFirmInvalid = 72,			//'H'
	enMmhtErContraMemberInvalid = 73,		//'I'
	enMmhtErBuySellInvalid = 74,			//'J'
	enMmhtErCallPutInvalid = 75,			//'K'
	enMmhtErSpreadIndicatorInvalid = 76,	//'L'
	enMmhtErTypeQuailfierInvalid = 77,		//'M'
	enMmhtErOptionSeriesInvalid = 78,		//'N'
	enMmhtErOptExpHasDDValue = 79,			//'O'
	enMmhtErPremiumPriceInvalid = 80,		//'P'
	enMmhtErTradeQuantityInvalid = 81,		//'Q'
	enMmhtErUnknownTransactionError = 82	//'R'

};




[
	export,
	uuid(3970A91E-E3E4-4c81-B215-68FFC8D7ACD6)
]
struct MessageDescriptor
{
	[helpstring("TradeID")]		BSTR TradeID;
	[helpstring("SequenceNo")]	LONG SequenceNo;
};

[
	export,
	uuid(D95849F5-4F5A-4e63-99A6-DBAA75F3BDB2)
]
struct ErrorDescription
{
	[helpstring("Error Code")]			MmhtErrorCodeEnum ErrCode;
	[helpstring("Error Description")]	BSTR Description;
};

[
	export,
	uuid(51C6CB93-68FE-4adf-A4AF-9F5F7A2AAD06)
]
struct ACKData
{
	[helpstring("SequenceNo")]							LONG SequenceNo;
	[helpstring("Unique POETS Transaction Identifyer")]	BSTR POEID;
};


namespace time_zone
{
	class CTimeZone
	{
	public:
		CTimeZone(){};

		static bool LocalTimeToPasificTime(SYSTEMTIME& LocalTime, SYSTEMTIME& PasificTime);
		static bool PasificTimeToLocalTime(SYSTEMTIME& PasificTime, SYSTEMTIME& LocalTime);

	public:
		static TIME_ZONE_INFORMATION m_TimeZoneInfo;
		static bool                  m_bTimeZoneInitialized;

		static bool InitializeTimeZone();
	protected:
		// ::TzSpecificLocalTimeToSystemTime is XP function.
		static BOOL ConvertTzSpecificLocalTimeToSystemTime(	const TIME_ZONE_INFORMATION &TimeZoneInformation,
															const SYSTEMTIME &LocalTime,
															LPSYSTEMTIME lpUniversalTime
															);

	};
};
namespace tnt_msg
{
	class CTNTRoot;

	class CMMHHMessage
	{
	public:
		virtual ~CMMHHMessage(){};

		virtual DWORD Parse(CFixBuffer& Buffer) =0;
		virtual CMMHHMessage* GetNewCopy() = 0;

		virtual void    SetSequenceNumber(long lSequence) = 0; 
		virtual long    GetSequenceNumber() = 0;
		virtual _bstr_t GetTradeID() =0;

		virtual bool    Execute(CTNTRoot* pRoot)  =0;

		//////////////////////////////////////////////////
		//Add your members access functions here
		//////////////////////////////////////////////////
	};


	typedef struct Header_data 
	{
		char m_Source;
		char m_MsgType;
		char m_Version[4];
		char m_Id[4];
		char m_SequenceNo[6];
		char m_Time[6];
		char m_Length[3];
		char m_Reserved[3];
	} Header_data ;


	typedef struct Fmmc_Alias_data
	{
		char m_Fmmc_Alias[10];
	}Fmmc_Alias_data;

	typedef struct Acknowlegement_body_type
	{
		char m_OriginalSequenceNo[6];
		char m_MmhtErrorCodeEnum1[2];
		char m_MmhtErrorCodeEnum2[2];
		char m_MmhtErrorCodeEnum3[2];
		char m_ExpectSequenceNo[6];
		char m_POEID[10];

	} Acknowlegement_body_type;

	typedef struct TNT_body_type
	{
		char m_TradeID[7];
		char m_BySell;//B or S
		char m_TradeDate[8];//YYYYMMDD
		char m_TradeTime[6];//HHMMSS
		char m_Quantity[7];
		char m_CallPut;
		char m_OptionSymbol[6];//C or P
		char m_OptionExpirationDate[8];//YYYYMMDD
		char m_StrikeDollar[4];
		char m_StrikeFraction[6];
		char m_PremiumPrice[11];
		char m_ExecutingFirm[4];
		char m_ExecutingMember[4];
		char m_CMTA[4];
		char m_TypeQualifier;
		char m_ContraFirm[4];
		char m_ContraMember[4];
		char m_SpreadIndicator;
		char m_OptionalData[13];
	} TNT_body_type;



	typedef struct TermConn_body_type
	{
		//	Empty
	} TermConn_body_type;


	class CHeaderData 
	{
	public:
		CHeaderData():m_lSequenceNumber(0){ Clean(); }
		CHeaderData(Header_data* pData):m_lSequenceNumber(0)
		{
			ATLASSERT(pData);
			m_pData = pData;
			Clean();
		}

		void Attach(Header_data* pData)
		{
			ATLASSERT(pData);
			m_pData = pData;
		};

		CHeaderData& operator=(const CHeaderData& cn)
		{
			m_lSequenceNumber = cn.m_lSequenceNumber;
			m_pData = NULL;
			return *this;
		}

		void Clean()
		{
			ATLASSERT(m_pData);
			m_pData->m_Source = 0;
			m_pData->m_MsgType = 0;
			m_lSequenceNumber = 0;
			//		memset(m_Version, 0, sizeof(m_Version));
			strcpy(m_pData->m_Version, STR_HEADER_VERSION);
			memset(m_pData->m_Id, ' ', sizeof(m_pData->m_Id));
			memset(m_pData->m_SequenceNo, '0', sizeof(m_pData->m_SequenceNo));
			memset(m_pData->m_Time, 0, sizeof(m_pData->m_Time));
			memset(m_pData->m_Length, '0', sizeof(m_pData->m_Length));
			memset(m_pData->m_Reserved, ' ', sizeof(m_pData->m_Reserved));

		}
		void SetCurrentTime();
		void SetBodySize(DWORD dwSize);
		void SetSequenceNumber(long lSeqNumber);

		long    GetSequenceNumber(){
			if(m_lSequenceNumber>0)
				return m_lSequenceNumber;
			else
			{
				CStringA str;
				char* szStr = str.GetBufferSetLength(sizeof(m_pData->m_SequenceNo)+1);
				memcpy(szStr,m_pData->m_SequenceNo,sizeof(m_pData->m_SequenceNo));
				szStr[sizeof(m_pData->m_SequenceNo)] = 0;
				str.ReleaseBuffer();
				return atol(str);				
			}
		}
		void SetSource(char chSource)  {m_pData->m_Source  =chSource; }
		void SetMsgType(char chMsgType){m_pData->m_MsgType =chMsgType;}
		char GetMsgType(){return m_pData->m_MsgType;}
		void SetID(LPCSTR szID);
		Header_data* operator->() const
		{ 
			ATLASSERT (m_pData);
			return m_pData; 
		}
	private:
		void CopyString(char* sBuffer, CStringA& strSource, int iSize);

		long	     m_lSequenceNumber;
		Header_data* m_pData;
	};

	template<class T, class Body_Type>
	class CMMHHLoadableMessage: public CMMHHMessage
	{
	protected:
		struct Message
		{
			Header_data m_header;
			Body_Type   m_body;
		}   m_message;
	public:
		CMMHHLoadableMessage(void):m_HeaderData(&m_message.m_header){CleanUp();};
		virtual ~CMMHHLoadableMessage(void){};

		CMMHHLoadableMessage<T, Body_Type>& operator=(const CMMHHLoadableMessage<T, Body_Type>& cn)
		{

			m_message = cn.m_message;
			m_HeaderData = cn.m_HeaderData;
			m_HeaderData.Attach(&m_message.m_header);
			return *this;
		}

		Body_Type&   GetBody(){return m_message.m_body;}
		CHeaderData& GetHeader(){return m_HeaderData;}
		LPVOID       GetBuffer(){return &m_message;}
		long         GetBufferLength(){return sizeof(m_message);}

		virtual _bstr_t GetTradeID()=0;

		virtual long GetSequenceNumber()
		{
			return GetHeader().GetSequenceNumber();
		}

		virtual void SetSequenceNumber(long lSequence)
		{
			GetHeader().SetSequenceNumber(lSequence);
		}

		virtual DWORD Parse(CFixBuffer& Buffer)
		{
#ifdef _DEBUG
			size_t iBufferSize = Buffer.GetLength();
			size_t iStructSize = sizeof(Header_data) + GetBodyLength();
#endif
			if(Buffer.GetLength() < sizeof(Header_data) + GetBodyLength())
				return ERROR_INSUFFICIENT_BUFFER;

			Message* pMessage = reinterpret_cast<Message*>(Buffer.GetData());

			if( (pMessage->m_header.m_MsgType != m_message.m_header.m_MsgType ) ||
				(pMessage->m_header.m_Source != m_message.m_header.m_Source) )
				return ERROR_BAD_FORMAT;


			CString strVersion1;
			char* chVersion = strVersion1.GetBufferSetLength(sizeof(m_message.m_header.m_Version)+1);
			memcpy(chVersion,pMessage->m_header.m_Version,sizeof(m_message.m_header.m_Version));
			chVersion[sizeof(m_message.m_header.m_Version)]=0;
			strVersion1.ReleaseBuffer();

			CString strVersion2;
			char* chVersion2 = strVersion2.GetBufferSetLength(sizeof(m_message.m_header.m_Version)+1);
			memcpy(chVersion2,m_message.m_header.m_Version,sizeof(m_message.m_header.m_Version));
			chVersion2[sizeof(m_message.m_header.m_Version)]=0;
			strVersion2.ReleaseBuffer();
			if(strVersion1 != strVersion2)
				return ERROR_INVALID_DATA;

			CString strLen;
			char* chLen = strLen.GetBufferSetLength(sizeof(m_message.m_header.m_Length)+1);
			memcpy(chLen,m_message.m_header.m_Length,sizeof(m_message.m_header.m_Length));
			chLen[sizeof(m_message.m_header.m_Length)]=0;
			strLen.ReleaseBuffer();
			long lSize = atol(strLen);
			if(lSize!=GetBodyLength())
				return ERROR_INVALID_DATA;


/*
			if(!memcmp(pMessage->m_header.m_Version,m_message.m_header.m_Version, sizeof(m_message.m_header.m_Version)))
				return ERROR_INVALID_DATA;

			if(!memcmp(pMessage->m_header.m_Length,m_message.m_header.m_Length, sizeof(m_message.m_header.m_Length))) 
				return ERROR_INVALID_DATA;
*/

			if( (CopyHeadDataFrom(&pMessage->m_header)) && (LoadBodyDataFrom((LPVOID)&pMessage->m_body)) )
			{
				Buffer.ShiftLeft(sizeof(Header_data) + GetBodyLength());
				return ERROR_SUCCESS;
			}

			CleanUp();
			return ERROR_INVALID_DATA;
		};
		CMMHHMessage& operator = (const CMMHHMessage& source)
		{
			template<class Body_Type>CMMHHLoadableMessage* pSrc = dynamic_cast<template<class Body_Type>CMMHHLoadableMessage*>&source;
			ATLASSERT(pSrc);
			if(pSrc)
			{
				m_header = pSrc->m_header;
				m_body = pSrc->m_body;
			}
			return *this;
		}
		CMMHHMessage* GetNewCopy()
		{
			CMMHHMessage* pRet = CreateEmpty();
			T* pT = dynamic_cast<T*>(pRet);
			T* pSrc = dynamic_cast<T*>(this);

			ATLASSERT(pT);
			ATLASSERT(pSrc);
			if(pT && pSrc)
				*pT = *pSrc;
			return pRet;
		}	
		DWORD GetBodyLength()
		{
			T* pT = static_cast<T*>(this);
			return pT->T_GetBodyLength();

		};

		_bstr_t GetString(LPCSTR szFiels, size_t lFieldSize)
		{
			_bstr_t bsRet = L"";
			CStringA strField;
			LPSTR pData = strField.GetBufferSetLength(int(lFieldSize+1));
			memcpy(pData,szFiels,lFieldSize);
			pData[lFieldSize]=0;
			strField.ReleaseBuffer();
			strField.Trim();
			bsRet = strField;
			return bsRet;
		}


		bool AssignString(char* strDestination, long lSource, UINT uiSizeMax, bool bIsRightJustified, char chFiller)
		{
			if(lSource<0)
				return false;

			CString str;
			str.Format(_T("%d"),lSource);

			return AssignString(strDestination, (BSTR)_bstr_t(str), 0, uiSizeMax, bIsRightJustified, chFiller);
		}

		bool AssignString(char* strDestination, BSTR bstrSource,UINT uiSizeMin, UINT uiSizeMax, bool bIsRightJustified, char chFiller)
		{

			if(!bstrSource)
			{
				if(uiSizeMin==0)
				{
					char sz[2];
					*sz = chFiller;
					sz[1] = 0;
					CComBSTR bFake = sz;
					return AssignString(strDestination, bFake,uiSizeMin, uiSizeMax, bIsRightJustified, chFiller);
				}				
				else
					return false;
			}

			CStringA str;
			char* pBuffer = str.GetBufferSetLength(uiSizeMax+1);
			ZeroMemory(pBuffer,uiSizeMax+1);
			ZeroMemory(strDestination,uiSizeMax);

			strncpy(pBuffer, CW2A(bstrSource),uiSizeMax);
			str.ReleaseBuffer();
			str.Trim();

			UINT lLenStr = str.GetLength();
			if(lLenStr>uiSizeMax || lLenStr<uiSizeMin)
				return false;

			if(lLenStr<uiSizeMax)
			{
				CStringA strResult;
				char* pchResult =  strResult.GetBufferSetLength(uiSizeMax+1);
				pchResult[uiSizeMax] = 0;
				memset(pchResult,chFiller,uiSizeMax);
				strResult.ReleaseBuffer();
				if(bIsRightJustified)
					memcpy(pchResult+uiSizeMax-lLenStr,(LPCSTR)str,lLenStr);
				else
					memcpy(pchResult,(LPCSTR)str,lLenStr);

				memcpy(strDestination,pchResult,uiSizeMax);
			}
			else
				memcpy(strDestination,(LPCSTR)str,uiSizeMax);
			return true;
		}

	protected:

		void CleanUp()
		{
			m_HeaderData.Clean();
			Initialize();
			GetHeader().SetBodySize(GetBodyLength());
			// check header size validity
			char   *stopstring;
			DWORD dwHeaderLength = strtol(m_message.m_header.m_Length, &stopstring, 10);
			DWORD dwBodyLength = GetBodyLength();
			ATLASSERT(dwHeaderLength == dwBodyLength);
		};

		bool CopyHeadDataFrom(const Header_data* pSrc)
		{
			memcpy(&m_message.m_header, pSrc, sizeof(m_message.m_header));	
			return true;
		};

		bool LoadBodyDataFrom(LPVOID pBuff)
		{
			memcpy(&m_message.m_body, pBuff, GetBodyLength());
			return true;
		}
		virtual CMMHHMessage*CreateEmpty() = 0;

		void Initialize() 
		{
			T* pT = static_cast<T*>(this);
			pT->Initialize();

		}

		DWORD T_GetBodyLength(){return sizeof(m_message.m_body);}

	private:
		CHeaderData m_HeaderData; 
	};

	class CAcknowlegementMessage :
		public CMMHHLoadableMessage<CAcknowlegementMessage, Acknowlegement_body_type>
	{
		friend class CMMHHLoadableMessage<CAcknowlegementMessage, Acknowlegement_body_type>;
	public:
		/*
		CAcknowlegementMessage(void);
		~CAcknowlegementMessage(void);
		*/
		HRESULT GetErrorDescriptions(LPSAFEARRAY* pSA);
		void GetError(long lPosition, MmhtErrorCodeEnum* penCode, _bstr_t* pbsDescription);
		MmhtErrorCodeEnum GetMmhtErrorCodeEnum(long lPosition);
		bool CheckError(long* plErrorCount);
		long GetMessagesCount();
		_bstr_t GetPOEID();

		virtual bool    Execute(CTNTRoot* pRoot);
		virtual _bstr_t GetTradeID(){return _bstr_t(L"");}
		long GetOriginalSequenceNumber();
		bool CheckSequenceViolation(long& lSentSeq, long& lExpectedSeq)
		{
			bool bRet = false;

			char chTmp = m_message.m_body.m_OriginalSequenceNo[sizeof(m_message.m_body.m_OriginalSequenceNo)];
			m_message.m_body.m_OriginalSequenceNo[sizeof(m_message.m_body.m_OriginalSequenceNo)] = 0;
			lSentSeq     = atol(m_message.m_body.m_OriginalSequenceNo);
			m_message.m_body.m_OriginalSequenceNo[sizeof(m_message.m_body.m_OriginalSequenceNo)] = chTmp;

			lExpectedSeq = lSentSeq;
			if(*m_message.m_body.m_MmhtErrorCodeEnum1=='4' || *m_message.m_body.m_MmhtErrorCodeEnum2=='4' || *m_message.m_body.m_MmhtErrorCodeEnum3=='4')
			{
				bRet = true;
				chTmp = m_message.m_body.m_ExpectSequenceNo[sizeof(m_message.m_body.m_ExpectSequenceNo)];
				m_message.m_body.m_ExpectSequenceNo[sizeof(m_message.m_body.m_ExpectSequenceNo)] = 0;
				lExpectedSeq = atol(m_message.m_body.m_ExpectSequenceNo);	
				m_message.m_body.m_ExpectSequenceNo[sizeof(m_message.m_body.m_ExpectSequenceNo)]= chTmp;
			}
			return bRet;
		}

	protected:
		void Initialize();
		virtual CMMHHMessage*CreateEmpty();

	};

	class CTNTMessage : public CMMHHLoadableMessage<CTNTMessage, TNT_body_type>
	{
		friend class CMMHHLoadableMessage<CTNTMessage, TNT_body_type>;
	public:
		
		CTNTMessage(bool bServerVersion = false):
			m_bServerVersion(bServerVersion)
			
		{
			CleanUp();
		}
		/*~CTNTMessage(void);
		*/

		_bstr_t GetErrorDescription(){ return m_bsDescription;}
		bool    SetMessage(const struct TNTData* pData/*, BSTR pszId*/);
		bool    GetMessage(CComRecPtr<TNTData>& dataRec);
//		bool	SetExecutingMember(BSTR bszMember);
		virtual bool    Execute(CTNTRoot* pRoot);
		virtual _bstr_t GetTradeID()
		{
			CStringA strTradeID;
			size_t lTraseIDLen = sizeof(GetBody().m_TradeID);

			LPSTR pData = strTradeID.GetBufferSetLength((int)(lTraseIDLen+1));

			memcpy(pData, GetBody().m_TradeID,lTraseIDLen);
			GetBody().m_TradeID[lTraseIDLen] = 0;

			strTradeID.ReleaseBuffer();
			return _bstr_t(strTradeID);
		}



	protected:
		void Initialize();
		virtual CMMHHMessage*CreateEmpty();
		_bstr_t m_bsDescription;
		bool m_bServerVersion;
	};

	class CTerminateConnectionMessage : public CMMHHLoadableMessage<CTerminateConnectionMessage, TermConn_body_type>
	{
		friend class CMMHHLoadableMessage<CTerminateConnectionMessage, TermConn_body_type>;
	public:
		/*
		CTerminateConnectionMessage (void);
		~CTerminateConnectionMessage (void);
		*/
		virtual bool    Execute(CTNTRoot* pRoot);
		virtual _bstr_t GetTradeID(){return _bstr_t(L"");}

	protected:
		void  Initialize();
		DWORD T_GetBodyLength(){return 0;}

		virtual CMMHHMessage*CreateEmpty();
	};


	class CLogOnData: public CMMHHLoadableMessage<CLogOnData, Fmmc_Alias_data>
	{
		friend class CMMHHLoadableMessage<CLogOnData, Fmmc_Alias_data>;
	public:

		CLogOnData(){
			Initialize();
		}
		void SetPassword(LPCSTR szPassword){
			ATLASSERT(szPassword);
			memset(m_message.m_body.m_Fmmc_Alias,' ',sizeof(m_message.m_body.m_Fmmc_Alias));
			if(szPassword)
			{
				size_t len = strlen(szPassword);
				size_t maxLen = sizeof(m_message.m_body.m_Fmmc_Alias);
				memcpy(m_message.m_body.m_Fmmc_Alias,szPassword,len>maxLen?maxLen:len);
			}
		}
		_bstr_t GetPassword()
		{
			CString str;
			char* szBuffer = str.GetBufferSetLength(sizeof(m_message.m_body.m_Fmmc_Alias)+1);
			memcpy(szBuffer,m_message.m_body.m_Fmmc_Alias,sizeof(m_message.m_body.m_Fmmc_Alias));
			szBuffer[sizeof(m_message.m_body.m_Fmmc_Alias)]=0;
			str.ReleaseBuffer();
			str.Trim();
			_bstr_t bsRet = (LPCTSTR)str;
			return bsRet;
		}

	public:
		virtual _bstr_t GetTradeID(){return _bstr_t(L"");}

		bool Execute(CTNTRoot* pRoot){
			ATLASSERT(FALSE);
			return false;
		};

	protected:
		virtual CMMHHMessage* CreateEmpty(){return new CLogOnData;};

		void Initialize(){
			ZeroMemory(&m_message.m_body, sizeof(m_message.m_body));

			m_message.m_header.m_MsgType = 'L';
			m_message.m_header.m_Source = 'N';
			GetHeader().SetCurrentTime();
		}
	};

	class CLogOffData: public CMMHHLoadableMessage<CLogOffData, Fmmc_Alias_data>
	{
		friend class CMMHHLoadableMessage<CLogOffData, Fmmc_Alias_data>;
	public:

		CLogOffData(){
			Initialize();
		}
		void SetPassword(LPCSTR szPassword){
			ATLASSERT(szPassword);
			memset(m_message.m_body.m_Fmmc_Alias,' ',sizeof(m_message.m_body.m_Fmmc_Alias));
			if(szPassword)
			{
				size_t len = strlen(szPassword);
				size_t maxLen = sizeof(m_message.m_body.m_Fmmc_Alias);
				memcpy(m_message.m_body.m_Fmmc_Alias,szPassword,len>maxLen?maxLen:len);
			}
		}
		_bstr_t GetPassword()
		{
			CString str;
			char* szBuffer = str.GetBufferSetLength(sizeof(m_message.m_body.m_Fmmc_Alias)+1);
			memcpy(szBuffer,m_message.m_body.m_Fmmc_Alias,sizeof(m_message.m_body.m_Fmmc_Alias));
			szBuffer[sizeof(m_message.m_body.m_Fmmc_Alias)]=0;
			str.ReleaseBuffer();
			str.Trim();
			_bstr_t bsRet = (LPCTSTR)str;
			return bsRet;
		}

	public:
		virtual _bstr_t GetTradeID(){return _bstr_t(L"");}

		bool Execute(CTNTRoot* pRoot){
			ATLASSERT(FALSE);
			return false;
		};

	protected:
		virtual CMMHHMessage* CreateEmpty(){return new CLogOffData;};
		void Initialize(){
			ZeroMemory(&m_message.m_body, sizeof(m_message.m_body));
			m_message.m_header.m_MsgType = 'G';
			m_message.m_header.m_Source  = 'N';
			GetHeader().SetCurrentTime();
		}
	};
/*
	class CLogOnData: public CLogin
	{
	public:
		CLogOnData():CLogin(){
		}

		bool Execute(CTNTRoot* pRoot){
			ATLASSERT(FALSE);
			return false;
		};

	protected:
		virtual CMMHHMessage* CreateEmpty(){return new CLogOnData;};
		virtual void Assign()
		{
			m_message.m_header.m_MsgType = 'L';
			m_message.m_header.m_Source = 'N';
		}
	};

	class CLogOffData: public CLogin
	{
	public:
		CLogOffData():CLogin()
		{
		}
		bool Execute(CTNTRoot* pRoot){
			ATLASSERT(FALSE);
			return false;
		};

	protected:
		void Assign(){
			m_message.m_header.m_MsgType = 'G';
			m_message.m_header.m_Source  = 'N';
		}
		virtual CMMHHMessage* CreateEmpty(){return new CLogOffData;};
	};
*/
	//*************************************************************************************
	class CTNTRoot
	{
	public:
		virtual bool OnAcknowlegement(CAcknowlegementMessage* pData) = 0;
		virtual bool OnTNTData(CTNTMessage* pData) = 0;
		virtual bool OnTerminateConnection()=0;
	};

};

