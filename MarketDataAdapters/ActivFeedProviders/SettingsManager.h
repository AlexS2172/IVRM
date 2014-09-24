#pragma once
namespace ActivFeedSettings
{
//  [5/25/2005]--------------------------------------------------------------
	class CDictionaryKey:   public std::string
	{
	public:
		CDictionaryKey(){};
		CDictionaryKey(const std::string str): std::string(str){};

	};
	class CDictionaryValue:	public std::string
	{
	public:
		CDictionaryValue(){};
		CDictionaryValue(const std::string str): std::string(str){};
	};
// ----------------------------------------------------------------------------------- [5/25/2005]
	class CDictionary
	{
	public:
		typedef std::map<CDictionaryKey, CDictionaryValue> DictionaryHolder;

	public:
		const CDictionaryValue GetValue(const CDictionaryKey& sKey)
		{
			CDictionaryValue sValue;

			DictionaryHolder::iterator itr = m_Dictionary.find(utils::upcase(sKey));
			if(itr != m_Dictionary.end())
				sValue = itr->second;
			else
				sValue = CDictionaryValue(_T(""));
			return sValue;

		}
		void SetValue(const CDictionaryKey& sKey, CDictionaryValue sValue)
		{
			m_Dictionary[utils::upcase(sKey)] = sValue;
		}

		__declspec (property(get=GetValue, put=SetValue)) const CDictionaryValue Dictionary[];
	private:
		DictionaryHolder m_Dictionary;

	};
	//  [5/25/2005]-----------------------------------------------------------
	class CSettingsKey: public CDictionaryKey
	{
	public:
		CSettingsKey(const CSettingsKey& key)
			:CDictionaryKey(static_cast<std::string>(key)){};
		CSettingsKey(const std::string& key)
			:CDictionaryKey(key){};
		CSettingsKey(LPCSTR szKey)
			:CDictionaryKey(std::string(szKey)){};

		CSettingsKey& operator=(const CSettingsKey& key) 
		{
			*this = key;
			return *this;
		}


	};

	class CExchangeKey: public CSettingsKey
	{
	public:
		typedef enum
		{
			enExchangeStock,
			enExchangeOption
		} ExchangeType;

		CExchangeKey(const std::string sKey, ExchangeType enType, const std::string&	region = "USA" )
			:CSettingsKey(CSettingsKey(sKey)), region_(region),
			m_enType(enType){}

		CExchangeKey(const CExchangeKey& key)
			: CSettingsKey(key), 
			m_enType(key.Type),
			region_(key.region_)
		{}
	public:

		__declspec (property(get = GetExchangeType)) ExchangeType Type;
		ExchangeType GetExchangeType() const {return m_enType;}
		const std::string& GetExchangeRegion() const { return region_; }
		void SetExchangeRegion(const std::string& region) { region_=region;}

		const std::string GetKey() const
		{
			std::string srKey;
			switch(Type)
			{
			case enExchangeOption:
				srKey+=_T("O_");
				break;
			case enExchangeStock:
				srKey+=_T("S_");
				break;
			}
			srKey += *static_cast<const string*>(this);
			return srKey;
		}

	private:
		ExchangeType m_enType;
		std::string region_;
	};

	class CActivExchangeKey: public CExchangeKey
	{
	public:
		CActivExchangeKey(const CExchangeKey& exchangeData)
			:CExchangeKey(exchangeData){};
		CActivExchangeKey& operator=(const std::string& str)
		{
			*static_cast<std::string*>(this) = str;
			return *this;
		}
	};
	class CEgarExchangeKey: public CExchangeKey
	{
	public:
		CEgarExchangeKey(const CExchangeKey& exchangeData)
			:CExchangeKey(exchangeData){};

		CEgarExchangeKey& operator=(const std::string& str)
		{
			*static_cast<std::string*>(this) = str;
			return *this;
		}
	};

	//  [5/25/2005]-----------------------------------------------------------
	class CSettingsManager
	{
	public:

		CSettingsManager(){}

		virtual ~CSettingsManager(void){}

	public:
		__declspec (property(get=GetSettings, put=PutSettings)) const CDictionaryValue Settings[];
		__declspec (property(get=GetActivExchange))				const CActivExchangeKey ActivExchange[];
		__declspec (property(get=GetEgarExchange))				const CEgarExchangeKey EgarExchange[];

	private:
		bool                InitializeProperties (MSXML2::IXMLDOMNodeListPtr spList);
		void                InitializeExchanges  (MSXML2::IXMLDOMNodeListPtr spList, CExchangeKey::ExchangeType enType );

	public:

		bool                Initialize(const std::string& sSessionName);
		void				PutSettings(const CSettingsKey& Key, CDictionaryValue& sValue);
		CDictionaryValue	GetSettings(const CSettingsKey& Key);
        CActivExchangeKey	GetActivExchange(const CEgarExchangeKey& sKey);
		CEgarExchangeKey	GetEgarExchange(const CActivExchangeKey& sKey);
		void				SetRegionRate(const std::string& sKey, double& dRate);
		double				GetRegionRate(const std::string& sKey);


	private:
		CDictionary  m_Settings;
		CDictionary  m_Egar2ActivExchanges;
		CDictionary  m_Activ2EgarExchanges;

		typedef std::map<std::string, std::string> EchangeRegionMap;
		typedef	std::map<std::string, double>	RegionRateMap;

		EchangeRegionMap		exchRegions_;
		RegionRateMap			m_Region2Rate;

	};

	typedef boost::shared_ptr<CSettingsManager>  CSettingsManagerPtr;
// inline functions definitions ------------------------------------------------------[5/25/2005]

	inline 
	void	CSettingsManager::PutSettings(const CSettingsKey& Key, CDictionaryValue& sValue)
	{
		m_Settings.Dictionary[Key] = sValue;
	}

	inline 
	CDictionaryValue CSettingsManager::GetSettings(const CSettingsKey& Key)
	{  
		return m_Settings.Dictionary[Key];
	}

	inline
		void CSettingsManager::SetRegionRate(const std::string& sKey, double& dRate)
	{
		RegionRateMap::iterator rItr = m_Region2Rate.find(sKey);
		if (rItr != m_Region2Rate.end() && dRate > 0.0)
			m_Region2Rate[sKey] = 1.0 / dRate;
	}

	inline
		double CSettingsManager::GetRegionRate(const std::string& sKey)
	{
		RegionRateMap::iterator rItr = m_Region2Rate.find(sKey);
		if (rItr != m_Region2Rate.end())
			return rItr->second;

		return 1.0;
	}

	inline 
		CActivExchangeKey CSettingsManager::GetActivExchange(const CEgarExchangeKey& sKey)
	{
		CActivExchangeKey retKey(sKey);
		CDictionaryValue Value = m_Egar2ActivExchanges.Dictionary[sKey.GetKey()];
		if(Value.length()){
			retKey = Value;
			EchangeRegionMap::iterator itr = exchRegions_.find(sKey.GetKey() );
			if (itr != exchRegions_.end() ) {
				retKey.SetExchangeRegion(itr->second);	
			}
		}
		return retKey;

	}
	inline 
	CEgarExchangeKey CSettingsManager::GetEgarExchange(const CActivExchangeKey& sKey)
	{
		CEgarExchangeKey retKey(sKey);
		CDictionaryValue Value = m_Activ2EgarExchanges.Dictionary[sKey.GetKey()];
		if(Value.length())
			retKey = Value;

		return retKey;
	}

	inline
	bool   CSettingsManager::Initialize(const std::string& sSessionName)
	{
		CEgRegKey regProvideerKey;
		if(ERROR_SUCCESS == regProvideerKey.Open(HKEY_LOCAL_MACHINE, PROVIDER_SETTINGS_ROOT, KEY_READ))
		{
			CoInitializeEx(NULL, COINIT_MULTITHREADED);

			try
			{
				_bstr_t bsFilePath;
				regProvideerKey.QueryValue(bsFilePath, _T("Configuration File"));
				MSXML2::IXMLDOMDocument2Ptr spDocument(__uuidof(MSXML2::DOMDocument));
				spDocument->async =VARIANT_FALSE;
				spDocument->resolveExternals = VARIANT_FALSE;
				spDocument->setProperty(_bstr_t(L"SelectionLanguage"), _variant_t(_bstr_t(L"XPath")));
				spDocument->load(bsFilePath);
				
				//EgLib::CEgLibTraceManager::Trace(EgLib::LogInfoExt,"CSettingsManager::Initialize", "Reading Default settings");
				InitializeProperties(spDocument->selectNodes(L"./Settings/Sessions/Session [@name=\"Default\"]/*"));

				_bstr_t bsSession  = L"./Settings/Sessions/Session [@name=\"";
						bsSession += sSessionName.c_str();
						bsSession += L"\"]/*";
	
				//EgLib::CEgLibTraceManager::Trace(EgLib::LogInfoExt,"CSettingsManager::Initialize", "Reading %s session settings", sSessionName.c_str());
				InitializeProperties(spDocument->selectNodes(bsSession));

				//EgLib::CEgLibTraceManager::Trace(EgLib::LogInfoExt,"CSettingsManager::Initialize", "Reading Stock exchanges");
				InitializeExchanges(spDocument->selectNodes(L"./Settings/Exchanges/Exchange[@type=\"Stock\"]"),  CExchangeKey::enExchangeStock);
				//EgLib::CEgLibTraceManager::Trace(EgLib::LogInfoExt,"CSettingsManager::Initialize", "Reading Option exchanges");
				InitializeExchanges(spDocument->selectNodes(L"./Settings/Exchanges/Exchange[@type=\"Option\"]"), CExchangeKey::enExchangeOption);
			}
			catch (_com_error e)
			{
				//EgLib::CEgLibTraceManager::Trace(EgLib::LogError,"CSettingsManager::Initialize","Com Exception:%d %s", e.Error(), (LPCSTR)e.Description());
			}
			CoUninitialize();
		}
		return true;
	}

	inline
	bool CSettingsManager::InitializeProperties(MSXML2::IXMLDOMNodeListPtr spList)
	{
		if(spList!=NULL && spList->length)
		{
			for(int i = 0; i<spList->length; ++i)
			{
				MSXML2::IXMLDOMNodePtr spNode = spList->item[i];
				MSXML2::IXMLDOMNamedNodeMapPtr spAttributes = spNode->attributes;
				MSXML2::IXMLDOMNodePtr spName  =   spAttributes->getNamedItem(L"name");
				MSXML2::IXMLDOMNodePtr spValue =   spAttributes->getNamedItem(L"value");
				if(spName!=NULL && spValue!=NULL)
				{
					//EgLib::CEgLibTraceManager::Trace(EgLib::LogInfoExt,"CSettingsManager::InitializeProperties","[%s] = %s",(LPCSTR)spName->text,(LPCSTR)spValue->text);
					m_Settings.SetValue(CDictionaryKey((LPCSTR)spName->text), CDictionaryValue((LPCSTR)spValue->text));
				}
			}
		}
		//else
		//	EgLib::CEgLibTraceManager::Trace(EgLib::LogInfoExt,"CSettingsManager::InitializeProperties","No settings definition found");

		return true;
	}

	inline
		void CSettingsManager::InitializeExchanges  (MSXML2::IXMLDOMNodeListPtr spList, CExchangeKey::ExchangeType enType )
	{
		if(spList!=NULL && spList->length)
		{
			try
			{
				for(int i = 0; i<spList->length; ++i)
				{
					MSXML2::IXMLDOMNodePtr spNode = spList->item[i];
					MSXML2::IXMLDOMNamedNodeMapPtr spAttributes = spNode->attributes;
					MSXML2::IXMLDOMNodePtr spActiv  =   spAttributes->getNamedItem(L"activ");
					MSXML2::IXMLDOMNodePtr spEgar =   spAttributes->getNamedItem(L"egar");
					MSXML2::IXMLDOMNodePtr spRegion =   spAttributes->getNamedItem(L"region");
					if(spActiv!=NULL && spEgar!=NULL)
					{
						//EgLib::CEgLibTraceManager::Trace(EgLib::LogInfoExt,"CSettingsManager::InitializeExchanges","Activ [%s] = Egar [%s]",(LPCSTR)spActiv->text,(LPCSTR)spEgar->text);
						CExchangeKey keyEgar ((LPCSTR)spEgar->text, enType, spRegion == NULL ? "USA" : (LPCSTR)spRegion->text);
						CExchangeKey keyActiv((LPCSTR)spActiv->text, enType, spRegion == NULL ? "USA" : (LPCSTR)spRegion->text);
						CDictionaryValue valueEgar((LPCSTR)spEgar->text);
						CDictionaryValue valueActiv((LPCSTR)spActiv->text);
						m_Egar2ActivExchanges.SetValue(keyEgar.GetKey(), valueActiv);
						m_Activ2EgarExchanges.SetValue(keyActiv.GetKey(), valueEgar);

						exchRegions_[keyEgar.GetKey()] = ( spRegion == NULL ? "USA" : (LPCSTR)spRegion->text );
						m_Region2Rate[spRegion == NULL ? "USA" : (LPCSTR)spRegion->text ] = 1.0;
					}
				}
			}
			catch (_com_error e)
			{
				//EgLib::CEgLibTraceManager::Trace(EgLib::LogError,"CSettingsManager::InitializeExchanges", "Com Exception:%d %s", e.Error(), (LPCSTR)e.Description());
			}
		}
		//else
		//	EgLib::CEgLibTraceManager::Trace(EgLib::LogInfoExt,"CSettingsManager::InitializeExchanges","No stock exchange definition found");
	}
};
