#include "StdAfx.h"
#include "IVDataManagerImpl.h"
#include "Packer.h"
#include "TableInitializer.h"
//--------------------------------------------------------------------------------------//
CIVDataManagerImpl::CIVDataManagerImpl(std::string objectID, CSettings::pointer	settings): 
m_Settings(settings)
{
	m_objectID = objectID;
	InitTables();
}
//--------------------------------------------------------------------------------------//
CIVDataManagerImpl::~CIVDataManagerImpl(void)
{
	CProviderSingleton::GetProvider()->Disconnect();
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::Connect(const char* username, const char* password)
{
	return CProviderSingleton::GetProvider()->Connect() == true ? CORBA::Short(DDS::S_ERR_NOERROR) : CORBA::Short(DDS::S_ERR_FAILED);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::SecureConnect(const char* username, const char* password)
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::Disconnect()
{
	return CProviderSingleton::GetProvider()->Disconnect();
}
//--------------------------------------------------------------------------------------//
SE::SEDBQuery_ptr		
CIVDataManagerImpl::CreateQuery()
{
	SE::SEDBQuery_ptr seDBQuery = SE::SEDBQuery::_nil();

	return seDBQuery;
}
//--------------------------------------------------------------------------------------//
SE::SEViewsManager_ptr	
CIVDataManagerImpl::CreateViewsManager()
{
	SE::SEViewsManager_ptr seViewsManager = SE::SEViewsManager::_nil();

	return seViewsManager;
}
//--------------------------------------------------------------------------------------//
CORBA::Boolean		
CIVDataManagerImpl::IsConnected()
{
	return CORBA::Boolean(true);
}
//--------------------------------------------------------------------------------------//
char*		
CIVDataManagerImpl::GetSessionID()
{
	std::string sessionID = std::string("NA");

	return const_cast<char*>(sessionID.c_str());
}
//--------------------------------------------------------------------------------------//
CORBA::Any*				
CIVDataManagerImpl::try_pull()
{
	std::auto_ptr<CORBA::Any> anyData;
	anyData = std::auto_ptr<CORBA::Any>(new CORBA::Any());
	return anyData.release();
}
//--------------------------------------------------------------------------------------//
CORBA::Any*				
CIVDataManagerImpl::pull(CORBA::Long wait_ms)
{

	_set_se_translator( SeTranslator );

	std::auto_ptr<CORBA::Any> anyData;
	try 
	{
		::Sleep(wait_ms);
		
		anyData = std::auto_ptr<CORBA::Any>(new CORBA::Any());
		
		CQuoteVector updates;
		if (CProviderSingleton::GetProvider()->GetUpdates(updates))
		{
			CORBA::Any* result = PackResponse(updates);
			updates.clear();
			return result;		
		}
	}
	catch(CORBA::SystemException&) {
		LOG4CPLUS_ERROR(ProviderLog, _T("CORBA::SystemException"));
	}catch(CORBA::Exception&){
		LOG4CPLUS_ERROR(ProviderLog, _T("CORBA::Exception"));
	}catch(omniORB::fatalException&) {
		LOG4CPLUS_ERROR(ProviderLog, _T("omniORB::fatalException"));
	}catch(...){
		LOG4CPLUS_ERROR(ProviderLog, _T("Unknwn exception"));
	}
	return anyData.release();
}
//--------------------------------------------------------------------------------------//
CORBA::Any*				
CIVDataManagerImpl::strict_pull()
{
	std::auto_ptr<CORBA::Any> anyData;
	anyData = std::auto_ptr<CORBA::Any>(new CORBA::Any());
	return anyData.release();
}
//--------------------------------------------------------------------------------------//
void	
CIVDataManagerImpl::obj_pull(DDS::IVObserver_ptr obj)
{

}
//--------------------------------------------------------------------------------------//
void					
CIVDataManagerImpl::observer_pull()
{

}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::SetCompressionMode(CORBA::Short compression_type)
{
	return DDS::S_ERR_NOERROR;
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::GetCompressionMode()
{
	return CORBA::Short(0);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::SetProtocolVersion(CORBA::Short protocolID)
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::GetProtocolVersion()
{
	return CORBA::Short(0);
}
//--------------------------------------------------------------------------------------//
CORBA::Any*
CIVDataManagerImpl::PackResponse(CQuoteVector& data)
{
	CAutoLock	lock(m_Lock);

	CORBA::Any* anyData = new CORBA::Any();
	
	CQuoteVector::iterator	itQBegin = data.begin();
	CQuoteVector::iterator	itQEnd = data.end();
	CQuoteVector::iterator	itrQ = itQBegin;

	QuoteAtomPtr		spQuote;
	VS::FieldDataList 	fieldDataList;
	long				tableID;

	//----------------------//
	for ( ; itrQ != itQEnd; itrQ++)
	{
		spQuote = *itrQ;
		try 
		{
			/*if (spQuote->m_spRequest->Type == enSTK || 
				spQuote->m_spRequest->Type == enIDX ||
				spQuote->m_spRequest->Type == enOPT ||)*/
			//{
				CPacker::PackQuote(spQuote,	fieldDataList);
				tableID = GetTableID(spQuote->m_spRequest->Type);
				m_Tables[tableID]->push_back(new VS::CTableRow(static_cast<long>(m_Tables[tableID]->size()), fieldDataList));
				fieldDataList.clear();
			//}
		}
		catch (std::exception e)
		{
			LOG4CPLUS_ERROR(ProviderLog, _T("std::exception occured while Pack Quote.") << e.what() << 
										_T(" TableID: ") << tableID);
			return anyData;
		}
		catch (CSeException* e)
		{
			CString strErrDescription;
			e->GetErrorMessage(strErrDescription);
			LOG4CPLUS_FATAL(ProviderLog , _T("SEH exception occured while Pack Quote.") << strErrDescription << 
										_T(" TableID: ") << tableID);
			e->Delete();
			return anyData;
		}
		catch (...)
		{
			LOG4CPLUS_ERROR(ProviderLog , _T("Unknown C++ exception occured while Pack Quote.") << 
										_T(" TableID: ") << tableID);
			return anyData;
		}
	}
	
	//----------------------//
	TableMap::iterator itrT = m_Tables.begin();
	TableMap::iterator itTEnd = m_Tables.end();

	VS::CMessageGroup messageGroup(VS::enCompressionNone);

	for ( ; itrT != itTEnd; itrT++)
	{
		try
		{
			if (!itrT->second->empty())
			{	
				VS::CMessageTrails messageTrails(itrT->first,
												VS::CMessageTrails::enRealtimeMessage,
												*m_FiledIDList[itrT->first], 
												*itrT->second, 
												*m_IDList[itrT->first]);

				LOG4CPLUS_INFO(ProviderLog, _T(" Table: ") << itrT->first << 
											_T(" FieldIDList size: ") << (*m_FiledIDList[itrT->first]).size() <<
											_T(" RowsCount: ") << (*itrT->second).size() <<
											_T(" IDList size: ") << (*m_IDList[itrT->first]).size());

				messageTrails.SetProtocolVersion(1);

				VS::CMessage::pointer message = VS::CMessageFactory::Instance().CreateMessage(messageTrails);
				if (message)
					messageGroup.AddMessage(message);

			}
		}
		catch (std::exception e)
		{
			LOG4CPLUS_ERROR(ProviderLog, _T("std::exception occured while create message trails: ") << e.what());
			return anyData;
		}
		catch (CSeException* e)
		{
			CString strErrDescription;
			e->GetErrorMessage(strErrDescription);
			LOG4CPLUS_FATAL(ProviderLog , _T("SEH exception occured while create message trails: ") << strErrDescription);
			e->Delete();
			return anyData;
		}
		catch (...)
		{
			LOG4CPLUS_ERROR(ProviderLog , _T("Unknown C++ exception occured while create message trails."));
			return anyData;
		}
	}
	//----------------------//
	CORBA::ULong msgSize = 0;
	BYTE* msg = NULL;
	try
	{
		msg = messageGroup.AssembleMessage(msgSize);
	}
	catch (std::exception e)
	{
		LOG4CPLUS_ERROR(ProviderLog, _T("std::exception occured while assemble message. ") << e.what());
		return anyData;
	}
	catch(CSeException* e)
	{
		CString strErrDescription;
		e->GetErrorMessage(strErrDescription);
		LOG4CPLUS_FATAL(ProviderLog , _T("SEH exception occured while assemble message. ") << strErrDescription);
		e->Delete();
		return anyData;
	}
	catch (...)
	{
		LOG4CPLUS_ERROR(ProviderLog , _T("Unknown C++ exception occured while assemble message."));
		return anyData;
	}

	//----------------------//
	DDS::IVData msgSequence(msgSize, msgSize, msg, 1);
	*anyData <<= msgSequence;

	try 
	{
		itrT = m_Tables.begin();
		itTEnd = m_Tables.end();
		for ( ; itrT != itTEnd; itrT++)
		{
			if (!itrT->second->empty())
			{	
				TableRowList::iterator itr = itrT->second->begin();
				TableRowList::iterator itBegin = itrT->second->end();

				for (; itr != itBegin; itr++){
					VS::CTableRow::pointer row = *itr;
					delete row;
				}
				itrT->second->clear();
			}
		}
	}
	catch (std::exception e)
	{
		LOG4CPLUS_ERROR(ProviderLog, _T("std::exception occured while clearing result rows. ") << e.what());
		return anyData;
	}
	catch(CSeException* e)
	{
		CString strErrDescription;
		e->GetErrorMessage(strErrDescription);
		LOG4CPLUS_FATAL(ProviderLog , _T("SEH exception occured while clearing result rows. ") << strErrDescription);
		e->Delete();
	}
	catch (...)
	{
		LOG4CPLUS_ERROR(ProviderLog , _T("Unknown C++ exception occured while clearing result rows."));
	}
	return anyData;
};
//--------------------------------------------------------------------------------------//
InstrumentTypeEnum	CIVDataManagerImpl::GetInstrumentType(CORBA::Short table, std::string& symbol)
{
	if (table == DDS::T_EQUITY_QUOTE)
	{
		long lIvRmContractType = CDataBase::getInstance()->getContractType(symbol);
		if (lIvRmContractType == 1)
			return enIDX;
		else if (lIvRmContractType == 2)
			return enSTK;
	}
	else if (table == DDS::T_OPTION_QUOTE){
		return enOPT;
	}
	else if (table == DDS::T_FUTURE_QUOTE){
		return enFUT;
	}
	else if (table == DDS::T_DIRECT_FUTURE_OPTION_QUOTE){
		return enFOP;
	}
	return enNone;
};
//--------------------------------------------------------------------------------------//
void
CIVDataManagerImpl::InitTables()
{
	//EQUITY_QUOTE
	long __Table = GetTableID(enSTK);
	m_Tables[__Table] = TableRowListPtr(new TableRowList);
	m_FiledIDList[__Table] = FieldIDListPtr(new FieldIDList);
	m_IDList[__Table] = IDListPtr(new IDList);
	CTableInitializer::GetEquityQuoteTable(*m_FiledIDList[__Table], *m_IDList[__Table]);

	//OPTION_QUOTE
	__Table = GetTableID(enOPT);
	m_Tables[__Table] = TableRowListPtr(new TableRowList);
	m_FiledIDList[__Table] = FieldIDListPtr(new FieldIDList);
	m_IDList[__Table] = IDListPtr(new IDList);
	CTableInitializer::GetOptionQuoteTable(*m_FiledIDList[__Table], *m_IDList[__Table]);

	//FUTURE_QUOTE
	__Table = GetTableID(enFUT);
	m_Tables[__Table] = TableRowListPtr(new TableRowList);
	m_FiledIDList[__Table] = FieldIDListPtr(new FieldIDList);
	m_IDList[__Table] = IDListPtr(new IDList);
	CTableInitializer::GetFutureQuoteTable(*m_FiledIDList[__Table], *m_IDList[__Table]);

	//FUTURE_OPTION_QUOTE
	__Table = GetTableID(enFOP);
	m_Tables[__Table] = TableRowListPtr(new TableRowList);
	m_FiledIDList[__Table] = FieldIDListPtr(new FieldIDList);
	m_IDList[__Table] = IDListPtr(new IDList);
	CTableInitializer::GetFutureOptionQuoteTable(*m_FiledIDList[__Table], *m_IDList[__Table]);
};
//--------------------------------------------------------------------------------------//
long							
CIVDataManagerImpl::GetTableID(InstrumentTypeEnum enType)
{
	if (enType == enSTK || enType == enIDX)
		return static_cast<long>(DDS::T_EQUITY_QUOTE);  //EQUITY_QUOTE
	else if (enType == enOPT)
		return static_cast<long>(DDS::T_OPTION_QUOTE);  //OPTION_QUOTE
	else if (enType == enFUT)
		return static_cast<long>(DDS::T_FUTURE_QUOTE);  //FUTURE_QUOTE
	else if (enType == enFOP)
		return static_cast<long>(DDS::T_DIRECT_FUTURE_OPTION_QUOTE);  //FUTURE_OPTION_QUOTE
	
	throw std::runtime_error("Invalid contract type: " + boost::lexical_cast<std::string>(enType));
}
//--------------------------------------------------------------------------------------//
SymbolListPtr			
CIVDataManagerImpl::ExtractSymbolList(const CORBA::Any& symbolGroup)
{
	SymbolListPtr symbolList(new SymbolList());
	DDS::SymbolsGroup* extractedSymbolGroup = NULL;
	if (symbolGroup >>= extractedSymbolGroup)
	{
		symbolList->reserve(extractedSymbolGroup->length());
		for (CORBA::ULong i = 0; i < extractedSymbolGroup->length(); i++)
		{
			std::string strSymbol = (*extractedSymbolGroup)[i];
			_strupr(const_cast<char*>(strSymbol.c_str()));
			symbolList->push_back(strSymbol);
		}
	}
	std::sort(symbolList->begin(), symbolList->end());

	return symbolList;
}
//--------------------------------------------------------------------------------------//
CORBA::Any*				
CIVDataManagerImpl::GetMarketData(const char* Symbol, CORBA::Short symbol_type, CORBA::Short Type, const CORBA::Any& fields)
{
	CORBA::Any* anyData;
	anyData = new CORBA::Any();

	if (GetInstrumentType(Type, std::string(Symbol)) != enNone)
	{
		CQuoteVector response;
		QuoteRequestPtr spRequest = QuoteRequestPtr(new QuoteRequest);
		QuoteAtomPtr spQuote;

		spRequest->Symbol = std::string(Symbol);
		spRequest->Type = GetInstrumentType(Type, std::string(Symbol));

		CProviderSingleton::GetProvider()->Request(spRequest, spQuote);
		response.push_back(spQuote);

		return PackResponse(response);
	}
	return anyData;
}
//--------------------------------------------------------------------------------------//
CORBA::Any*				
CIVDataManagerImpl::GetMarketDataGroup(const CORBA::Any& group, CORBA::Short symbol_type, CORBA::Short type, const CORBA::Any& fields)
{
	std::auto_ptr<CORBA::Any> anyData;
	anyData = std::auto_ptr<CORBA::Any>(new CORBA::Any());
	
	SymbolListPtr symbolList = ExtractSymbolList(group);

	if (!symbolList->empty())
	{	
		SubscribeGroup(group, symbol_type, type);

		CQuoteVector response;
		CRequestVector request;
		request.reserve(symbolList->size());
		response.reserve(symbolList->size());

		SymbolList::iterator itr = symbolList->begin();
		SymbolList::iterator itEnd = symbolList->end();
		for (; itr != itEnd; itr++)
		{	
			QuoteRequestPtr spRequest = QuoteRequestPtr(new QuoteRequest);
			spRequest->Symbol = std::string(*itr);
			spRequest->Type = GetInstrumentType(type, std::string(*itr));
			request.push_back(spRequest);
		}
		
		CProviderSingleton::GetProvider()->RequestMultiple(request, response);

		return PackResponse(response);
	}
	
	return anyData.release();
}
//--------------------------------------------------------------------------------------//
CORBA::Any*	
CIVDataManagerImpl::GetMarketDataAll(CORBA::Short Type, const CORBA::Any& fields)
{
	std::auto_ptr<CORBA::Any> anyData;
	anyData = std::auto_ptr<CORBA::Any>(new CORBA::Any());
	return anyData.release();
}
//--------------------------------------------------------------------------------------//
CORBA::Short		
CIVDataManagerImpl::ReloadMarketStructure()
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Boolean			
CIVDataManagerImpl::IsMarketActive()
{
	return CORBA::Boolean(true);
}
//--------------------------------------------------------------------------------------//
CORBA::Boolean			
CIVDataManagerImpl::IsMarketStructureChanged()
{
	return CORBA::Boolean(false);
}
//--------------------------------------------------------------------------------------//
CORBA::Boolean			
CIVDataManagerImpl::GetMarketStructureChangedStatus()
{
	return CORBA::Boolean(false);
}
//--------------------------------------------------------------------------------------//
void	
CIVDataManagerImpl::ResetMarketStructureChangedStatus()
{

}
//--------------------------------------------------------------------------------------//
CORBA::Short	
CIVDataManagerImpl::Subscribe(const char* Symbol, CORBA::Short sumbol_type, CORBA::Short type)
{
	if (GetInstrumentType(type, std::string(Symbol)) != enNone)
	{
		QuoteRequestPtr spRequest = QuoteRequestPtr(new QuoteRequest);

		QuoteAtomPtr spQuote;
		spRequest->Symbol = std::string(Symbol);
		spRequest->Type = GetInstrumentType(type, std::string(Symbol));
		
		if (CProviderSingleton::GetProvider()->Subscribe(spRequest))
			return CORBA::Short(DDS::S_ERR_NOERROR);
	}
	return CORBA::Short(DDS::S_ERR_NOT_FOUND);
}
//--------------------------------------------------------------------------------------//
CORBA::Short CIVDataManagerImpl::SubscribeGroup(const CORBA::Any& group, CORBA::Short symbol_type, CORBA::Short type)
{
	LOG4CPLUS_INFO(ProviderLog, "Incoming request: Subscribe group");

	SymbolListPtr symbolList = ExtractSymbolList(group);

	if (!symbolList->empty())
	{	
		LOG4CPLUS_INFO(ProviderLog, "Subscribe group request contain " << symbolList->size() << " symbols");

		CQuoteVector response;
		CRequestVector request;
		request.reserve(symbolList->size());
		response.reserve(symbolList->size());

		SymbolList::iterator itr = symbolList->begin();
		SymbolList::iterator itEnd = symbolList->end();
		for (; itr != itEnd; itr++)
		{	
			QuoteRequestPtr spRequest = QuoteRequestPtr(new QuoteRequest);
			spRequest->Symbol = std::string(*itr);
			spRequest->Type = GetInstrumentType(type, std::string(*itr));
			request.push_back(spRequest);
		}

		if (CProviderSingleton::GetProvider()->SubscribeMultiple(request))
			return CORBA::Short(DDS::S_ERR_NOERROR);
	}
	else
	{
		LOG4CPLUS_WARN(ProviderLog, "Subscribe group request contain no symbols");
	}
		
	return CORBA::Short(DDS::S_ERR_NOT_FOUND);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::SubscribeAll(CORBA::Short type)
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::UnSubscribe(const char* Symbol, CORBA::Short symbol_type, CORBA::Short type)
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::UnSubscribeGroup(const CORBA::Any& group, CORBA::Short symbol_type, CORBA::Short type)
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::UnSubscribeAll(CORBA::Short type)
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::RegisterFields(CORBA::Short Type, const CORBA::Any& fields)
{
	 return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::RegisterAllFields(CORBA::Short Type)
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::UnRegisterFields(CORBA::Short Type, const CORBA::Any& fields)
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Short		
CIVDataManagerImpl::UnRegisterAllFields(CORBA::Short Type)
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::RegisterCallBack(DDS::IVDataClient_ptr client)
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Boolean			
CIVDataManagerImpl::IsCallBackRegistered()
{
	return CORBA::Boolean(false);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::UnRegisterCallBack()
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::ValidateUser(const char* username, const char* password, CORBA::Any_OUT_arg info)
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::ValidateType(const char* username, CORBA::Short Type)
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::ValidateSubscription(const char* username, const char* symbol, CORBA::Short symbol_type)
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Any*				
CIVDataManagerImpl::GetRegisteredFields(CORBA::Short Type)
{
	std::auto_ptr<CORBA::Any> anyData;
	anyData = std::auto_ptr<CORBA::Any>(new CORBA::Any());
	return anyData.release();
}
//--------------------------------------------------------------------------------------//
CORBA::Any*				
CIVDataManagerImpl::GetValidFields(const char* username, CORBA::Short Type)
{
	std::auto_ptr<CORBA::Any> anyData;
	anyData = std::auto_ptr<CORBA::Any>(new CORBA::Any());
	return anyData.release();
}
//--------------------------------------------------------------------------------------//
CORBA::Any*
CIVDataManagerImpl::GetValidStocks(const char* username)
{
	std::auto_ptr<CORBA::Any> anyData;
	anyData = std::auto_ptr<CORBA::Any>(new CORBA::Any());
	return anyData.release();
}
//--------------------------------------------------------------------------------------//
CORBA::Any*				
CIVDataManagerImpl::ValidateStocksGroup(const char* username, const CORBA::Any& group, CORBA::Short type)
{
	std::auto_ptr<CORBA::Any> anyData;
	anyData = std::auto_ptr<CORBA::Any>(new CORBA::Any());
	return anyData.release();
}
//--------------------------------------------------------------------------------------//
void
CIVDataManagerImpl::UpdateStorage(CORBA::Short type, const CORBA::Any& fields, const CORBA::Any& data)
{

}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::InsertRows(const CORBA::Any& anyMsg)
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//
CORBA::Short			
CIVDataManagerImpl::DeleteRowsByKey(CORBA::Short dataType, CORBA::Short indexID, const char* key)
{
	return CORBA::Short(DDS::S_ERR_NOERROR);
}
//--------------------------------------------------------------------------------------//