#if !defined(AFX_ATFBASEFEED_H__C8612C6F_7382_4892_A338_92523A9051DE__INCLUDED_)
#define AFX_ATFBASEFEED_H__C8612C6F_7382_4892_A338_92523A9051DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class T>
inline void CATFBaseFeed<T>::AddRequest(const CComVariant& vtRequest, RequestType type,  GroupRequestType enOperationType)
{
	if (!m_bConnected)
	{
		CCommonException::ThrowError (E_FAIL,L"Provider is not connected to feed");
	}
		
	{
		_QuoteUpdateParams param(vtRequest);
		_bstr_t bsRet = param->Symbol;
		if (!bsRet.length())
			utils::ThrowErrorNoSetErrorInfo(E_INVALIDARG, L"Invalid symbol name(empty).");
	}

	T* pThis = static_cast<T*>(this);
	CRequest req;
	_bstr_t bsFullName = CreateFullName(vtRequest, type);
	{

		req.m_bsExtendedSymbol = bsFullName;
		req.m_enType = type;
		req.m_vtRequest = vtRequest;
		req.m_enGroupRequestType = enOperationType;

		{
			CCriticalSectionWrapper cs(m_csRequest);
			m_Request.insert(m_Request.end(), req);
		}
		Sleep(0);
		SetEvent(m_hRequest);
	}
}

template <class T>
inline void CATFBaseFeed<T>::PublicError(RequestsTypeEnum type, const CComVariant& request, _bstr_t description, ErrorNumberEnum Error)
{
	CEventsData data;
	data.m_type = _enRequestError;
	data.m_RequestType = type;
	data.m_vtRequest = request;

	if (CheckConnection())
	{
		data.m_bstrDescription = description;
		data.m_Error = Error;
	}
	else
	{
		data.m_bstrDescription = L"The AT connection is temporary broken";
		data.m_Error = enConnectionWasDisconnected;
	}

	m_EventsData.LockIn();
	m_EventsData.AddIn(data);
	if (m_EventsData.InSize() && !(m_EventsData.InSize() % 20))
		ATFPROVTRACE4(__FUNCTION__, _T("PublicError, description = %s, Events in buffer size = %d"), (LPCTSTR)data.m_bstrDescription, m_EventsData.InSize());
	SetEvent(m_hEvent);
	m_EventsData.UnlockIn();
}



template <class T>
inline void CATFBaseFeed<T>::PublicResponce(RequestType type, const CComVariant& request, const CComVariant& response, bool bValue)
{
	CEventsData data;
	data.m_type = type;
	data.m_vtRequest = request;
	data.m_vtResponse = response;
	data.m_bValue = bValue;
	data.m_bsKey = CreateFullName(request, type);

	m_EventsData.LockIn();
	m_EventsData.AddIn(data);
	if (m_EventsData.InSize() && !(m_EventsData.InSize() % 20))
		ATFPROVTRACE4(__FUNCTION__, _T("PublicResponce, key = %s, Events in buffer size = %d"), (LPCTSTR)data.m_bsKey, m_EventsData.InSize());
	SetEvent(m_hEvent);
	m_EventsData.UnlockIn();
}

template <class T>
inline void CATFBaseFeed<T>::PublicResponce(RequestType type, const CComVariant& request, const CComVariant& response)
{
	CEventsData data;
	data.m_type = type;
	data.m_vtRequest = request;
	data.m_vtResponse = response;
	data.m_bsKey = CreateFullName(request, type);

	m_EventsData.LockIn();
	m_EventsData.AddIn(data);
	if (m_EventsData.InSize() && !(m_EventsData.InSize() % 20))
		ATFPROVTRACE4(__FUNCTION__, _T("PublicResponce, key = %s, Events in buffer size = %d"), (LPCTSTR)data.m_bsKey, m_EventsData.InSize());
	SetEvent(m_hEvent);
	m_EventsData.UnlockIn();
}

template <class T>
inline void CATFBaseFeed<T>::PublicResponce(RequestType type, const CComVariant& request)
{
	CEventsData data;
	data.m_type = type;
	data.m_vtRequest = request;
	data.m_bsKey = CreateFullName(request, type);

	m_EventsData.LockIn();
	m_EventsData.AddIn(data);
	if (m_EventsData.InSize() && !(m_EventsData.InSize() % 20))
		ATFPROVTRACE4(__FUNCTION__, _T("PublicResponce, key = %s, Events in buffer size = %d"), (LPCTSTR)data.m_bsKey, m_EventsData.InSize());
	SetEvent(m_hEvent);
	m_EventsData.UnlockIn();
}

template <class T>
inline UINT CATFBaseFeed<T>::_Request(LPVOID pData)
{

	CATFBaseFeed<T>* pT = reinterpret_cast< CATFBaseFeed<T>* >(pData);
	if(pT)
	{
		HANDLE hEvents[] =
		{
			pT->m_hRequest,
			pT->m_hStop
		};

		ATFPROVTRACE(__FUNCTION__, _T("Begin request thread"));
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		
		while(1)
		{
			if(WAIT_OBJECT_0 != WaitWithEvents(2,hEvents))
				break;

			BOOL bIsSubRequest = FALSE;
			DWORD	dwTicks = 0;

			while(!pT->m_Request.empty())
			{
				try
				{
					CRequest req;
					try
					{
						CCriticalSectionWrapper s(pT->m_csSubsription);
						CCriticalSectionWrapper cs(pT->m_csRequest);
						if(pT->m_Request.empty())
							break;
						req = pT->m_Request.front();
						pT->m_Request.erase(pT->m_Request.begin());
						ATFPROVTRACE(__FUNCTION__, _T("Request vector size = %d"), pT->m_Request.size());
					}
					catch(...)
					{
						break;
					}
					
					BOOL bSubRequestNow = IsSubRequest(req.m_enType, req.m_enGroupRequestType);
					if (!bIsSubRequest && bSubRequestNow)
					{
						dwTicks = GetTickCount();
					}

					if (bIsSubRequest && !bSubRequestNow)
					{
						ATFPROVTRACE4(__FUNCTION__, _T("Subscribed quotes, time = %d"), GetTickCount() - dwTicks);
					}

					bIsSubRequest = bSubRequestNow;

					switch(req.m_enType)
					{
					case _enRequestRealtimeNotification:
						pT->Subscribe(req.m_vtRequest, true);
						break;
					case _enRequestRealtime:
						pT->Subscribe(req.m_vtRequest, false);
						break;
					case _enRequestGroup:
						{
							pT->GetGroup(req.m_vtRequest, req.m_enGroupRequestType);

						}break;
					case _enRequestGrpOptions:
						{
							pT->GetOptionsGroup(req.m_vtRequest, req.m_enGroupRequestType);

						} break;
					case _enRequestLast:
						{
							pT->GetPrice(req.m_vtRequest);
						} break;
					case _enRequestStock:
						{
							pT->GetStock(req.m_vtRequest);
						}break;
					case _enRequestOption:
						{
							pT->GetOptions(req.m_vtRequest); 
						}break;
					}
				}
				catch(...)
				{
				}
			}

			if (bIsSubRequest)
			{
				ATFPROVTRACE4(__FUNCTION__, _T("Subscribed quotes, time = %d"), GetTickCount() - dwTicks);
			}
		}

		CoUninitialize();
		ATFPROVTRACE(__FUNCTION__, _T("End request thread"));
	}
	return 0;
}


template <class T>
inline bool CATFBaseFeed<T>::GetOptionsGroup(const CComVariant& vtRequest, GroupRequestType enType)
{
	if(IsStopped())
		return false;
	m_lGroupRequest++;
	_StockParams param(vtRequest);
	USES_CONVERSION;
	T* pT = static_cast<T*>(this);

	PB		pb;
	DWORD  	rc,dwFType;
	ATFI_OPTION atfiOptionRecords[MAX_OPTROOTS];
	_bstr_t bsNameKey = CreateFullName(vtRequest, _enRequestGrpOptions);
	
	GROUPREQUEST::iterator it = m_GroupRequest.find(bsNameKey);

	_bstr_t bsName = param->Stock;
	bsName +=L".US";

	if(it!=m_GroupRequest.end())
		return false;

	rc = AtGetPriceBlock(m_pHAT, m_QSN, bsName, &pb, &dwFType, NULL, 0, 0);

	if (rc < PB_MINSIZE)
	{
		PublicError(enRequestOptions,vtRequest,L"Symbol is not supported",enNoDataAvailableForSymbol );
		return false;
	}
		// Get array of ATFI Option records
			
	rc = GetATFIOptionInfo(m_pHAT, m_QSN, &pb, atfiOptionRecords, MAX_OPTROOTS,0);

	if (rc == 0)
	{
		// Error
		PublicError(enRequestOptions,vtRequest,L"Symbol has no options",enNoDataAvailableForSymbol );
		return FALSE;
	}

	m_lGroupRequest++;
	{
		CCriticalSectionWrapper cs(m_GrpRequest.m_csOptionRequest);
		m_GrpRequest.m_bEnabled = true;
		m_GrpRequest.m_bsKey = bsNameKey;
		m_GrpRequest.m_vtRequest = vtRequest;
		m_GrpRequest.m_lGroupRequest = m_lGroupRequest;
	}

	m_GroupRequestID[m_lGroupRequest]= bsNameKey;

	CGroupRequest req;
	m_GroupRequest[bsNameKey].m_lID = m_lGroupRequest;

	for (DWORD i = 0; i < rc; ++i)
	{
		char	szRootSym[32];
		if(IsStopped())
			return false;

		::ZeroMemory(szRootSym, sizeof(szRootSym));
		memcpy(szRootSym, atfiOptionRecords[i].optionSymbol,4);
		rtrim(szRootSym);

		{

			CCriticalSectionWrapper cs(m_GrpRequest.m_csOptionRequest);
			m_GrpRequest.m_mapExchanges.clear();
			if(enGroupRequestAllExchanges != enType)
			{
				if(!param->Exchange || !*param->Exchange || *param->Exchange==' ')
				{
					std::set<char>& stData =  m_GrpRequest.m_mapExchanges[atfiOptionRecords[i].primaryExchange];
					if(!param->Exchange || !*param->Exchange)
						stData.insert(0);				
					else
						stData.insert(' ');				
				}
				else
				{
					string str;
					char buffer[100];
					ZeroMemory(buffer, sizeof(buffer));
					strncpy(buffer, W2A(param->Exchange), sizeof(buffer)-1);
					rtrim(buffer);
					str = buffer;
					
					size_t prevPos = 0;
					size_t  pos = str.find(',',prevPos);
					if (pos!=string::npos) 
					{
						do
						{
							if(prevPos==pos)
							{
								std::set<char>& stData =  m_GrpRequest.m_mapExchanges[atfiOptionRecords[i].primaryExchange];
								stData.insert(0);				
							}
							else
							{
								string strExch = str.substr(prevPos, pos-prevPos);
								char ch[32];
								ZeroMemory(ch,sizeof(ch));
								strncpy(ch,strExch.c_str(), sizeof(ch)-1);
								rtrim(ch);
								char cch[2];
								int len = strlen(ch);
								if(len)
									cch[0] = ch[len-1];
								else
									cch[0] = 0;
								cch[1] = 0;
								if(*cch!=' ' && *cch)
								{
									_bstr_t bsExch =GetATExchange(A2W(cch),_enRequestOption);
									if(bsExch.length())
									{
										std::set<char>& stData =  m_GrpRequest.m_mapExchanges[*W2A(bsExch)];
										stData.insert(*cch);				
									}
								}
								else
								{
									std::set<char>& stData =  m_GrpRequest.m_mapExchanges[atfiOptionRecords[i].primaryExchange];
									stData.insert(' ');				
								}
							}
							prevPos = pos+1;
							pos = str.find(',', prevPos);
						}
						while(pos!=string::npos);
						{
							string st = str.substr(prevPos, string::npos);
							if(!st.length())
							{
								std::set<char>& stData =  m_GrpRequest.m_mapExchanges[atfiOptionRecords[i].primaryExchange];
								stData.insert(0);				
							}
							else
							{
								char ch[32];
								ZeroMemory(ch,sizeof(ch));
								strncpy(ch,st.c_str(), sizeof(ch)-1);
								rtrim(ch);
								char cch[2];
								int len = strlen(ch);
								if(len)
									cch[0] = ch[len-1];
								else
									cch[0] = 0;

								cch[1] = 0;
								if(*cch!=' ' && *cch)
								{
									_bstr_t bsExch =GetATExchange(A2W(cch),_enRequestOption);
									if(bsExch.length())
									{
										std::set<char>& stData =  m_GrpRequest.m_mapExchanges[*W2A(bsExch)];
										stData.insert(*cch);				
									}
								}
								else
								{
									std::set<char>& stData =  m_GrpRequest.m_mapExchanges[atfiOptionRecords[i].primaryExchange];
									stData.insert(' ');				
								}

							}

						}
					}
					else
					{
						if(!str.length())
						{
							std::set<char>& stData =  m_GrpRequest.m_mapExchanges[atfiOptionRecords[i].primaryExchange];
							stData.insert(' ');				
						}
						else
						{
							char ch[32];
							ZeroMemory(ch,sizeof(ch));
							strncpy(ch,str.c_str(), sizeof(ch)-1);
							rtrim(ch);
							char cch[2];
							cch[0] = ch[strlen(ch)-1];
							cch[1] = 0;
							if(*cch!=' ')
							{
								_bstr_t bsExch =GetATExchange(A2W(cch),_enRequestOption);
								if(bsExch.length())
								{
									std::set<char>& stData =  m_GrpRequest.m_mapExchanges[*W2A(bsExch)];
									stData.insert(*cch);				
								}
							}
							else
							{
								std::set<char>& stData =  m_GrpRequest.m_mapExchanges[atfiOptionRecords[i].primaryExchange];
								stData.insert(' ');				
							}
						}
					}
				}
			}

			m_GrpRequest.m_iLotSize = atfiOptionRecords[i].contractMultiplier;
			m_GrpRequest.m_enType = enGroupRequestLastQuote; 
			m_GrpRequest.m_bATFSpec = true;

		}

		// Make the root symbol look right
		
		_strupr(szRootSym);

		// Use the proper datatype
		int iErrCount = 0;
		ENUM_DATATYPE enData;
		ZeroMemory(&enData, sizeof(ENUM_DATATYPE));
		enData.dwDatatype = DT_PQ_US_STKOPTS;
		enData.bInhibitAdvise = 1;

		while(!EnumPriceBlocks(m_pHAT, m_QSN, enData.dwDatatype, szRootSym,(GBLOCKENUMPBPROC) GroupSubscribeFunc, (LPARAM)this ))
		{
			iErrCount++;
			if(iErrCount <= 3 )
			{
				if(WAIT_TIMEOUT != WaitWithEvents(1, &m_hStop, 60000))
				{
					m_GrpRequest.m_bEnabled = false;
					break;
				}
			}
			else
			{
				PublicError(enRequestLastQuote,vtRequest,L"Error loading option prices", enNoDataAvailableForSymbol );
				m_GrpRequest.m_bEnabled = false;
				break;
			}
		}

		if(!m_GrpRequest.m_bEnabled)
			break;
	}

	{
		CCriticalSectionWrapper cs(m_GrpRequest.m_csOptionRequest);
		m_GrpRequest.m_bEnabled = false;
		m_GroupRequest.erase(bsNameKey);
	}

	pT->PublicGroupData(vtRequest, NULL, 0,TRUE);

	return true;
}




template <class T>
inline bool CATFBaseFeed<T>::GetGroup(const CComVariant& vtRequest, GroupRequestType enType)
{
	if(IsStopped())
		return false;

	RequestsTypeEnum type = enRequestOptions;
	if (enType == enGroupRequestLastQuote)
		type = enRequestLastQuote;
	else if (enType == enGroupRequestSubscribe || enType == enGroupRequestAllNotify)
		type = enSubscribeQuote;

	m_lGroupRequest++;
	_QuoteUpdateParams param(vtRequest);
	USES_CONVERSION;
	bool bError = false;

	PB		pb;
	DWORD  	rc,dwFType;
	ATFI_OPTION atfiOptionRecords[MAX_OPTROOTS];
	_bstr_t bsNameKey = CreateFullName(vtRequest, _enRequestGroup);
	
	GROUPREQUEST::iterator it = m_GroupRequest.find(bsNameKey);

	_bstr_t bsName = param->Symbol;
	
	bsName +=L".US";


	if(it!=m_GroupRequest.end())
		return false;

	ATFPROVTRACE4(__FUNCTION__, _T("Begin group request. Key = %s"), (LPCTSTR)bsNameKey);

	rc = AtGetPriceBlock(m_pHAT, m_QSN, bsName, &pb, &dwFType, NULL, 0, 0);

	if (rc < PB_MINSIZE)
	{
		PublicError(type,vtRequest,L"Symbol is not supported",enNoDataAvailableForSymbol );
		return false;
	}
		// Get array of ATFI Option records

	ATFPROVTRACE(__FUNCTION__, _T("Request option roots"));
			
	rc = GetATFIOptionInfo(m_pHAT, m_QSN, &pb, atfiOptionRecords, MAX_OPTROOTS,0);

	ATFPROVTRACE(__FUNCTION__, _T("Request option roots done. size = %d"), rc);

	if (rc == 0)
	{
		// Error
		PublicError(type,vtRequest,L"Symbol has no options",enNoDataAvailableForSymbol );
		return FALSE;
	}

	m_lGroupRequest++;
	{
		CCriticalSectionWrapper cs(m_GrpRequest.m_csOptionRequest);
		m_GrpRequest.m_bEnabled = true;
		m_GrpRequest.m_bsKey = bsNameKey;
		m_GrpRequest.m_vtRequest = vtRequest;
		m_GrpRequest.m_lGroupRequest = m_lGroupRequest;
	}

	m_GroupRequestID[m_lGroupRequest]= bsNameKey;

	CGroupRequest req;
	m_GroupRequest[bsNameKey].m_lID = m_lGroupRequest;

	for (DWORD i = 0; i < rc; ++i)
	{
		char	szRootSym[32];
		if(IsStopped())
			return false;

		::ZeroMemory(szRootSym, sizeof(szRootSym));
		memcpy(szRootSym, atfiOptionRecords[i].optionSymbol,4);
		rtrim(szRootSym);

		ATFPROVTRACE(__FUNCTION__, _T("Request exchange for %s option root"), szRootSym);

		{

			CCriticalSectionWrapper cs(m_GrpRequest.m_csOptionRequest);
			m_GrpRequest.m_mapExchanges.clear();

			if(!param->Exchange || !*param->Exchange || *param->Exchange==' ')
			{
				std::set<char>& stData =  m_GrpRequest.m_mapExchanges[atfiOptionRecords[i].primaryExchange];
				if(!param->Exchange || !*param->Exchange)
					stData.insert(0);				
				else
					stData.insert(' ');				
			}
			else
			{
				string str;
				char buffer[100];
				ZeroMemory(buffer, sizeof(buffer));
				strncpy(buffer, W2A(param->Exchange), sizeof(buffer)-1);
				rtrim(buffer);
				str = buffer;
				
				size_t prevPos = 0;
				size_t  pos = str.find(',',prevPos);

				if (pos!=string::npos) 
				{
					do
					{
						if(prevPos==pos)
						{
							std::set<char>& stData =  m_GrpRequest.m_mapExchanges[atfiOptionRecords[i].primaryExchange];
							stData.insert(0);	
						}
						else
						{
							string strExch = str.substr(prevPos, pos-prevPos);
							char ch[32];
							ZeroMemory(ch,sizeof(ch));
							strncpy(ch,strExch.c_str(), sizeof(ch)-1);
							rtrim(ch);
							char cch[2];
							int len = strlen(ch);
							if(len)
								cch[0] = ch[len-1];
							else
								cch[0] = 0;
							cch[1] = 0;
							if(*cch!=' ' && *cch)
							{
								_bstr_t bsExch =GetATExchange(A2W(cch),_enRequestOption);
								if(bsExch.length())
								{
									std::set<char>& stData =  m_GrpRequest.m_mapExchanges[*W2A(bsExch)];
									stData.insert(*cch);
								}
							}
							else
							{
								std::set<char>& stData =  m_GrpRequest.m_mapExchanges[atfiOptionRecords[i].primaryExchange];
								stData.insert(' ');				
							}
						}
						prevPos = pos+1;
						pos = str.find(',', prevPos);
					}
					while(pos!=string::npos);
					{
						string st = str.substr(prevPos, string::npos);
						if(!st.length())
						{
							std::set<char>& stData =  m_GrpRequest.m_mapExchanges[atfiOptionRecords[i].primaryExchange];
							stData.insert(0);				
						}
						else
						{
							char ch[32];
							ZeroMemory(ch,sizeof(ch));
							strncpy(ch,st.c_str(), sizeof(ch)-1);
							rtrim(ch);
							char cch[2];
							int len = strlen(ch);
							if(len)
								cch[0] = ch[len-1];
							else
								cch[0] = 0;

							cch[1] = 0;
							if(*cch!=' ' && *cch)
							{
								_bstr_t bsExch =GetATExchange(A2W(cch),_enRequestOption);
								if(bsExch.length())
								{
									std::set<char>& stData =  m_GrpRequest.m_mapExchanges[*W2A(bsExch)];
									stData.insert(*cch);				
								}
							}
							else
							{
								std::set<char>& stData =  m_GrpRequest.m_mapExchanges[atfiOptionRecords[i].primaryExchange];
								stData.insert(' ');				
							}

						}

					}
				}
				else
				{
					if(!str.length())
					{
						std::set<char>& stData =  m_GrpRequest.m_mapExchanges[atfiOptionRecords[i].primaryExchange];
						stData.insert(' ');				
					}
					else
					{
						char ch[32];
						ZeroMemory(ch,sizeof(ch));
						strncpy(ch,str.c_str(), sizeof(ch)-1);
						rtrim(ch);
						char cch[2];
						cch[0] = ch[strlen(ch)-1];
						cch[1] = 0;
						if(*cch!=' ')
						{
							_bstr_t bsExch = GetATExchange(A2W(cch),_enRequestOption);
							if(bsExch.length())
							{
								std::set<char>& stData =  m_GrpRequest.m_mapExchanges[*W2A(bsExch)];
								stData.insert(*cch);				
							}
						}
						else
						{
							std::set<char>& stData =  m_GrpRequest.m_mapExchanges[atfiOptionRecords[i].primaryExchange];
							stData.insert(' ');				
						}
					}
				}
			}

			m_GrpRequest.m_iLotSize = atfiOptionRecords[i].contractMultiplier;
			m_GrpRequest.m_enType = enType; 
			m_GrpRequest.m_bATFSpec = false;

		}

		if(!m_GrpRequest.m_mapExchanges.size())
		{
			PublicError(type, vtRequest,L"Error loading option prices", enNoDataAvailableForSymbol );
			bError = true;
			break;
		}
		else
		{
			// Make the root symbol look right
			
			_strupr(szRootSym);

			// Use the proper datatype
			int iErrCount = 0;
			ENUM_DATATYPE enData;
			ZeroMemory(&enData, sizeof(ENUM_DATATYPE));
			enData.dwDatatype = DT_PQ_US_STKOPTS;
			enData.bInhibitAdvise = 1;

			ATFPROVTRACE(__FUNCTION__, _T("Request prices for %s option root"), szRootSym);
			while(!EnumPriceBlocks(m_pHAT, m_QSN, enData.dwDatatype, szRootSym,(GBLOCKENUMPBPROC) GroupSubscribeFunc, (LPARAM)this ))
			{
				iErrCount++;
				if(iErrCount <= 3 )
				{
					if(WAIT_TIMEOUT != WaitWithEvents(1, &m_hStop, 60000))
					{
						m_GrpRequest.m_bEnabled = false;
						break;
					}
				}
				else
				{
					PublicError(type,vtRequest,L"Error loading option prices", enNoDataAvailableForSymbol );
					m_GrpRequest.m_bEnabled = false;
					break;
				}
			}

			if(!m_GrpRequest.m_bEnabled)
				break;
		}
	}

	{
		CCriticalSectionWrapper cs(m_GrpRequest.m_csOptionRequest);
		m_GrpRequest.m_bEnabled = false;
	}

	T* pThis = static_cast<T*>(this);
	if ((enType != enGroupRequestSubscribe && enType!=enGroupRequestAllNotify )|| bError)
	{
		if(enType!=enGroupRequestAll || bError)
			m_GroupRequest.erase(bsNameKey);

		if(!bError)
			pThis->PublicGroupData(vtRequest, NULL, 0,TRUE);
	}
	else if (enType == enGroupRequestSubscribe && !bError)
	{
		CCriticalSectionWrapper cs(m_csRequest);
		GROUPREQUEST::const_iterator grit = m_GroupRequest.find(bsNameKey);

		if (grit != m_GroupRequest.end())
		{
			const CGroupRequest& grreq = grit->second;
			const CGroupRequest::GROUP& gr = grreq.m_keys;

			CGroupRequest::GROUP::const_iterator it = gr.begin();
			CCriticalSectionWrapper cs(m_csSubsription);

			for (;it != gr.end(); it++)
			{
				const _bstr_t& bsKey = *it;
				SUBSCRIPTION::const_iterator sit = m_Subscription.find(bsKey);

				if (sit != m_Subscription.end())
				{
					const CSubscription& sub = sit->second;
					const PB& pb = sub.m_key;
					if (pb.atheader.updType == 'R')
					{
						ATFPROVTRACE(__FUNCTION__, _T("LastQuote %s: bid = %.2f, ask = %.2f, last = %.2f"), 
							(LPCTSTR)bsKey, pb.atstkopt.bid /100., pb.atstkopt.ask / 100., pb.atstkopt.last / 100.);
						pThis->PublicRealtime(bsKey, (PB*)&pb);
					}
				}
			}
		}
	}
	else if (enType == enGroupRequestAllNotify && !bError)
	{
		ATFPROVTRACE(__FUNCTION__, _T("PublicSubscriptionNotification, key = %s"), (LPCTSTR)bsNameKey);
		pThis->PublicSubscriptionNotification(vtRequest);

		CCriticalSectionWrapper cs(m_csRequest);
		GROUPREQUEST::const_iterator grit = m_GroupRequest.find(bsNameKey);

		if (grit != m_GroupRequest.end())
		{
			const CGroupRequest& grreq = grit->second;
			const CGroupRequest::GROUP& gr = grreq.m_keys;

			CGroupRequest::GROUP::const_iterator it = gr.begin();
			CCriticalSectionWrapper cs(m_csSubsription);

			for (;it != gr.end(); it++)
			{
				const _bstr_t& bsKey = *it;
				SUBSCRIPTION::const_iterator sit = m_Subscription.find(bsKey);

				if (sit != m_Subscription.end())
				{
					const CSubscription& sub = sit->second;
					const PB& pb = sub.m_key;
					if (pb.atheader.updType == 'R')
					{
						ATFPROVTRACE(__FUNCTION__, _T("LastQuote %s: bid = %.2f, ask = %.2f, last = %.2f"), 
							(LPCTSTR)bsKey, pb.atstkopt.bid /100., pb.atstkopt.ask / 100., pb.atstkopt.last / 100.);
						pThis->PublicRealtime(bsKey, (PB*)&pb);
					}
				}
			}
		}
	}

	return true;
}

template <class T>
inline bool CATFBaseFeed<T>::GetOptions(const CComVariant& vtRequest)
{
	bool bRet = true;
	
	if(IsStopped())
		return false;

	try
	{
		T* pThis = static_cast<T*>(this);
		_OptionParams param(vtRequest);

        PB		pb = {0};
		DWORD  	rc = 0;
        DWORD   dwFType = 0;
		bool    bRet = true;
        ATFI_OPTION atfiOptionRecords[MAX_OPTROOTS] = {0};

		_bstr_t bsNameKey = CreateFullName(vtRequest, _enRequestOption);

        USES_CONVERSION;
		rc = AtGetPriceBlock(m_pHAT, m_QSN, W2A(param->UnderlyingSymbol), &pb, &dwFType, NULL, 0, 0);

		if (rc < PB_MINSIZE)
		{
			PublicError(enRequestOptions,vtRequest,L"Symbol is not supported",enNoDataAvailableForSymbol );
			return false;
		}
			// Get array of ATFI Option records
				
		rc = GetATFIOptionInfo(m_pHAT, m_QSN, &pb, atfiOptionRecords, MAX_OPTROOTS,0);

		if (rc == 0)
		{
			PublicError(enRequestOptions,vtRequest,L"Symbol has no options",enNoDataAvailableForSymbol );
			return FALSE;;
		}

		{
			CCriticalSectionWrapper cs(m_OptRequest.m_csOptionRequest);
			m_OptRequest.m_bEnabled = true;
			m_OptRequest.m_bsKey = bsNameKey;
			m_OptRequest.m_vtRequest = vtRequest;
		}

		for (DWORD i = 0; i < rc; ++i)
		{
			char	szRootSym[32];
			if(IsStopped())
				return false;

			{
				CCriticalSectionWrapper cs(m_OptRequest.m_csOptionRequest);
				m_OptRequest.m_mapExchanges.clear();

				if (param->Exchange && *param->Exchange && *param->Exchange != ' ')    
                    m_OptRequest.m_mapExchanges[*W2A(GetATExchange(param->Exchange, _enRequestOption))];
                else
                    m_OptRequest.m_mapExchanges[atfiOptionRecords[i].primaryExchange];

				m_OptRequest.m_iLotSize = atfiOptionRecords[i].sharesPerContract;
			}

			memcpy(szRootSym, atfiOptionRecords[i].optionSymbol,4);
			szRootSym[4] = 0;
			rtrim(szRootSym);
			// Make the root symbol look right
			
			_strupr(szRootSym);

			// Use the proper datatype
			int iErrCount = 0;

			ENUM_DATATYPE enData;
			ZeroMemory(&enData, sizeof(ENUM_DATATYPE));
			enData.dwDatatype = DT_PQ_US_STKOPTS;
			enData.bInhibitAdvise = 1;

			while(!EnumPriceBlocks(m_pHAT, m_QSN, enData.dwDatatype, szRootSym,(GBLOCKENUMPBPROC) OptionFunc, (LPARAM)this))
			{
				iErrCount++;
				if(iErrCount <= 3 )
				{
					if(WAIT_TIMEOUT != WaitWithEvents(1, &m_hStop, 60000))
					{
						m_OptRequest.m_bEnabled = false;
						break;
					}
				}
				else
				{
					PublicError(enRequestOptions,vtRequest,L"Error loading options",enNoDataAvailableForSymbol );
					m_OptRequest.m_bEnabled = false;
					break;
				}
			}

			if(!m_OptRequest.m_bEnabled)
				break;
		}

		pThis->PublicOptionData(vtRequest, NULL, 0, TRUE);
	}
	catch(...)
	{
			PublicError(enRequestOptions,vtRequest,L"Error loading options",enNoDataAvailableForSymbol );
			bRet =  false;
	}

	{
		CCriticalSectionWrapper cs(m_OptRequest.m_csOptionRequest);
		m_OptRequest.m_bEnabled = false;
	}

	return bRet;
}

template <class T>
inline bool CATFBaseFeed<T>::GetStock(const CComVariant vtRequest)
{
	if(IsStopped())
		return false;

	USES_CONVERSION;
	DWORD   	  rc;
	DWORD         dwFType;
	GPBE_FLAGS	  gpbFlags;
	FUNDDATA      fData;
	PB            pb;

	try
	{
		_StockParams  param(vtRequest);

		gpbFlags.flags = 0;
		gpbFlags.bFundCopy = TRUE;

		_bstr_t bsKey = CreateFullName(vtRequest, _enRequestStock);
		rc = AtGetPriceBlock(m_pHAT, m_QSN, bsKey,
							 &pb,				// The price block will go here
							 &dwFType,			// The 'type' of fundamental data here
							 (LPSTR) &fData,	// The fundamental data goes here
							 PFI_FIXEDPAGE,		// This is what we want
							 gpbFlags.flags);


		if (LOBYTE(rc) < PB_MINSIZE || ( dwFType != FDT_ATFI_EQUITY_FP && dwFType != FDT_ATFI_INDEX )|| (!GetPriceBlockFlags(&pb,GBC_ANYSTOCK) && !GetPriceBlockFlags(&pb,GBC_INDEX)))
		{
			PublicError(enRequestStock,vtRequest,L"No such symbol found", enSymbolNotSupported);
			return false;
		}
		if(!IsStopped())
		{
			T* pThis = static_cast<T*>(this);
					pThis->PublicStockData(vtRequest, &pb, (ATFI_FUNDAMENTAL*)&fData);
		}
	}
	catch(...)
	{
		PublicError(enRequestStock,vtRequest,L"Error loading Symbol",enProviderInternalError);
		return false;
	}
	return true;
}



#endif // AFX_ATFBASEFEED_H__C8612C6F_7382_4892_A338_92523A9051DE__INCLUDED_
