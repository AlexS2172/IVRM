#pragma once
#include "..\..\..\..\ivservice\ivserver\volatilityserver_3_0_0_8\idl\ivdataservice410.h"

#include "VolatilityServerCore\VSDefinitions.h"
#include "ActivFeedConnection.h"
#include "TableInitializer.h"
#include <VolatilityServerTypes/TableRow.h>
#include <MsgAssembler/MessageGroup.h>
#include <MsgAssembler/MessageTrails.h>
#include <MsgAssembler/MessageFactory.h>

using namespace VS;

typedef	boost::shared_ptr<VS::TableRowList>	TableRowListPtr;
typedef std::map<long, TableRowListPtr>		TableMap;
typedef std::map<long, VS::FieldIDListPtr>	FieldIDListMap;
typedef boost::shared_ptr<VS::IDList>		IDListPtr;
typedef std::map<long, IDListPtr>			IDListMap;

class CIVDataManagerImpl :
	public	POA_DDS::IVDataManager
{
public:
	CIVDataManagerImpl(std::string objectID, CSettings::pointer settings);
	virtual ~CIVDataManagerImpl(void);

public:
	//IVDataManager
	CORBA::Short			Connect(const char* username, const char* password);
	CORBA::Short			SecureConnect(const char* username, const char* password);
	CORBA::Short			Disconnect();
	SE::SEDBQuery_ptr		CreateQuery();
	SE::SEViewsManager_ptr	CreateViewsManager();	

	CORBA::Boolean			IsConnected();
	char*					GetSessionID();

	CORBA::Any*				try_pull();
	CORBA::Any*				pull(CORBA::Long wait_ms);
	CORBA::Any*				strict_pull();

	void					obj_pull(DDS::IVObserver_ptr obj);
	void					observer_pull();

	CORBA::Short			SetCompressionMode(CORBA::Short compression_type);
	CORBA::Short			GetCompressionMode();

	CORBA::Short			SetProtocolVersion(CORBA::Short protocolID);
	CORBA::Short			GetProtocolVersion();

	CORBA::Any*				GetMarketData(const char* Symbol, CORBA::Short symbol_type, CORBA::Short Type, const CORBA::Any& fields);
	CORBA::Any*				GetMarketDataGroup(const CORBA::Any& group, CORBA::Short symbol_type, CORBA::Short type, const CORBA::Any& fields);
	CORBA::Any*				GetMarketDataAll(CORBA::Short Type, const CORBA::Any& fields);

	CORBA::Short			ReloadMarketStructure();
	CORBA::Boolean			IsMarketActive();
	CORBA::Boolean			IsMarketStructureChanged();
	CORBA::Boolean			GetMarketStructureChangedStatus();
	void					ResetMarketStructureChangedStatus();

	CORBA::Short			Subscribe(const char* Symbol, CORBA::Short sumbol_type, CORBA::Short type);
	CORBA::Short			SubscribeGroup(const CORBA::Any& group, CORBA::Short symbol_type, CORBA::Short type);
	CORBA::Short			SubscribeAll(CORBA::Short type);
	CORBA::Short			UnSubscribe(const char* Symbol, CORBA::Short symbol_type, CORBA::Short type);
	CORBA::Short			UnSubscribeGroup(const CORBA::Any& group, CORBA::Short symbol_type, CORBA::Short type);
	CORBA::Short			UnSubscribeAll(CORBA::Short type);
	CORBA::Short			RegisterFields(CORBA::Short Type, const CORBA::Any& fields);
	CORBA::Short			RegisterAllFields(CORBA::Short Type);
	CORBA::Short			UnRegisterFields(CORBA::Short Type, const CORBA::Any& fields);
	CORBA::Short			UnRegisterAllFields(CORBA::Short Type);
	CORBA::Short			RegisterCallBack(DDS::IVDataClient_ptr client);
	CORBA::Boolean			IsCallBackRegistered();
	CORBA::Short			UnRegisterCallBack();

	CORBA::Short			ValidateUser(const char* username, const char* password, CORBA::Any_OUT_arg info);
	CORBA::Short			ValidateType(const char* username, CORBA::Short Type);
	CORBA::Short			ValidateSubscription(const char* username, const char* symbol, CORBA::Short symbol_type);
	CORBA::Any*				GetRegisteredFields(CORBA::Short Type);
	CORBA::Any*				GetValidFields(const char* username, CORBA::Short Type);
	CORBA::Any*				GetValidStocks(const char* username);
	CORBA::Any*				ValidateStocksGroup(const char* username, const CORBA::Any& group, CORBA::Short type);

	void					UpdateStorage(CORBA::Short type, const CORBA::Any& fields, const CORBA::Any& data);
	CORBA::Short			InsertRows(const CORBA::Any& anyMsg);
	CORBA::Short			DeleteRowsByKey(CORBA::Short dataType, CORBA::Short indexID, const char* key);

public:
	std::string				m_objectID;
	CSettings::pointer		m_Settings;

	TableMap				m_Tables;
	FieldIDListMap			m_FiledIDList;
	IDListMap				m_IDList;

	//CQuoteVector			m_RTQuotes;

	CORBA::Any*				PackResponse(CQuoteVector& data);
	void					InitTables();
	long					GetTableID(InstrumentTypeEnum enType);

	SymbolListPtr			ExtractSymbolList(const CORBA::Any& symbolGroup);
	InstrumentTypeEnum		GetInstrumentType(CORBA::Short table, std::string& symbol);
	CLock					m_Lock;
};
