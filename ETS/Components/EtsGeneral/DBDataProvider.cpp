#include "StdAfx.h"
#include "DBDataProvider.h"
#include "DataProvider.h"
#include "Position.h"
#include "Trader.h"
#include "Strategy.h"
#include "AbstractContract.h"
#include "Analytics.h"
#include "AccountItem.h"

DBDataProvider::DBDataProvider(std::string connection_string){
	this->connection_string = connection_string;
};

/*virtual*/
DBDataProvider::~DBDataProvider(void){
};

CDBConnection&
DBDataProvider::get_connection(){
	return db_connection;
};

/*virtual*/
bool
DBDataProvider::open(std::string connection_string /* = std::string("")*/){
	try {
		if (!db_connection.IsOpened()){
			if (this->connection_string.length()){
				BSTR bstr = string_to_bstr(this->connection_string);
				db_connection.Open(_bstr_t(bstr), 10, 120, 300, 300);
			}
			else{
				throw DataProvider::Exception("Invalid connection string.");
			};
		};
		return true;
	}
	catch (DataProvider::Exception exception){
		throw exception;
	}
	catch(...){
		throw DataProvider::Exception("Unknown exception occured while trying open connection.");
	};
	return false;
};

/*virtual*/
bool
DBDataProvider::close(){
	try {
		db_connection.Close();
	}
	catch (...){
		throw DataProvider::Exception("Unknown exception occured while close connection.");
	};
	return true;
};


/*virtual*/
Table*
DBDataProvider::parse(DataProvider::DataRecordset* recordset){
	DBDataRecordset* db_recordset = dynamic_cast<DBDataRecordset*>(recordset);
	CStoredProc<CClientRecordset>* rs = db_recordset->db_recordset;

	Table* table = 0;
	if(rs->GetRecordCount()){
		table = new Table;
		rs->MoveFirst();
		while(!rs->IsEOF()){

			_RecordsetPtr ado_recordset = 0;
			rs->GetAdoRecordset(&ado_recordset);

			long field_count = ado_recordset->GetFields()->GetCount();
			if (field_count > 0){
				Row* row = new Row;
				for (long i = 0; i < field_count; i++){
					FieldPtr field = ado_recordset->GetFields()->Item[i];
					_bstr_t filed_name = field->Name;
					_variant_t field_value = field->Value;
					std::string column_name = bstr_to_string(filed_name.GetBSTR());
					row->set_column(Value(column_name, field_value));
				}
				table->insert(row);
			}
			rs->MoveNext();
		}
	};
	return table;
};

/*virtual*/
Table*
DBDataProvider::execute_command(DataProvider::Command* command){
	DBCommand* cmd = dynamic_cast<DBCommand*>(command);
	if (cmd){
		DataProvider::DataRecordset* recordset = cmd->execute(this);
		if (recordset){
			Table* table = parse(recordset);
			delete recordset;
			return table;
		};
	};
	return 0;
};


DBDataRecordset::DBDataRecordset(){
};

DBDataRecordset::~DBDataRecordset(){
	delete db_recordset;
};

/*virtual*/
DataProvider::DataRecordset*
DBSelectTrades::execute(DataProvider *data_provider){
	DBDataProvider* provider = dynamic_cast<DBDataProvider*>(data_provider);
	if (provider){
		DBDataRecordset* recordset = new DBDataRecordset();
		recordset->db_recordset = new
			CStoredProc<CClientRecordset>(provider->get_connection(), L"usp_trades_get");
		recordset->db_recordset->Open();
		return recordset;
	};
	return 0;
};

/*virtual*/
DataProvider::DataRecordset*
DBSelectTraders::execute(DataProvider *data_provider){
	DBDataProvider* provider = dynamic_cast<DBDataProvider*>(data_provider);
	if (provider){
		DBDataRecordset* recordset = new DBDataRecordset();
		recordset->db_recordset = new
			CStoredProc<CClientRecordset>(provider->get_connection(), L"usp_traders_get");
		recordset->db_recordset->Open();
		return recordset;
	};
	return 0;
};

/*virtual*/
DataProvider::DataRecordset*
DBSelectStrategys::execute(DataProvider *data_provider){
	DBDataProvider* provider = dynamic_cast<DBDataProvider*>(data_provider);
	if (provider){
		DBDataRecordset* recordset = new DBDataRecordset();
		recordset->db_recordset = new
			CStoredProc<CClientRecordset>(provider->get_connection(), L"usp_strategys_get");
		recordset->db_recordset->Open();
		return recordset;
	};
	return 0;
};

/*virtual*/
DataProvider::DataRecordset*
DBSelectPositions::execute(DataProvider *data_provider){
	DBDataProvider* provider = dynamic_cast<DBDataProvider*>(data_provider);
	if (provider){
		DBDataRecordset* recordset = new DBDataRecordset();
		recordset->db_recordset = new
			CStoredProc<CClientRecordset>(provider->get_connection(), L"usp_positions_get");
		recordset->db_recordset->Open();
		return recordset;
	};
	return 0;
};

void
DBImportPositionAnalytics::add_position(Position *position){
	positions.push_back(position);
};

/*virtual*/
DataProvider::DataRecordset*
DBImportPositionAnalytics::execute(DataProvider *data_provider){
	DBDataProvider* provider = dynamic_cast<DBDataProvider*>(data_provider);
	if (provider){
		provider->open("");
		std::vector<Position*>::iterator it = positions.begin();
		std::vector<Position*>::iterator it_end = positions.end();
		for ( ; it != it_end; it++){

			std::string xml_string;
			std::ostringstream strm;
						
			Analytics::variables_iterator ita = Analytics::get_variables().begin();
			Analytics::variables_iterator ita_end = Analytics::get_variables().end();
			Analytics* anx = (*it)->get_analytics();
			
			Trader* trader = (*it)->get_account_item()->get_trader();
			Strategy* strategy = (*it)->get_account_item()->get_strategy();
			CAbstractContract* contract = (*it)->get_account_item()->get_contract();
			CAsset* contract_asset = dynamic_cast<CAsset*>(contract->GetAsset().GetInterfacePtr());

			strm << "<analytics ";
			for ( ; ita != ita_end; ita++){
				if (!bad_value(anx->values[ita->second])){
					strm << ita->first <<"=\"" << anx->values[ita->second] << "\" ";
				};
			};
			strm << " asset_id=\"" << contract_asset->m_nID << "\" ";
			strm << " />";
			xml_string = std::string(strm.str());
			
			DBDataRecordset recordset;
			recordset.db_recordset = new
				CStoredProc<CClientRecordset>(provider->get_connection(), L"usp_analytics_import");
			if (trader == 0){
				*(recordset.db_recordset) << CDBNull();
			}
			else{
				*(recordset.db_recordset) << trader->id();
			};

			if (strategy == 0){
				*(recordset.db_recordset) << CDBNull();
			}
			else{
				*(recordset.db_recordset) << strategy->id();				
			};

			*(recordset.db_recordset) << contract->m_nID;
			*(recordset.db_recordset) << _bstr_t(string_to_bstr(xml_string), false);
			recordset.db_recordset->Open();
		};
		provider->close();
		return 0;
	};
	return 0;
};

