#pragma once

class CDatabaseImpl
{
public:
	typedef CDatabaseImpl*	pointer;

	CDatabaseImpl(): m_ConnectionString(""){
	};

	long getContractType(long contractId){
		try	{
			if (Initialize()) {
				CStoredProc<CClientRecordset> rs(m_DbConnection, L"usp_ContractType_Get");

				/*Set procedure params*/
				rs << contractId;

				/*execute query*/
				rs.Open();
				if(rs.GetRecordCount())
				{
					rs.MoveFirst();
					while(!rs.IsEOF())
					{
						long ContractType = (long)rs[L"iContractType"];
						rs.Close();
						return ContractType;
					}
				};
			}
		}
		catch (_com_error& err)	{
			LOG4CPLUS_ERROR(ProviderLog, _T("ComError: getContractType(long) ")<<(char*)(err.Description()));
			ATLASSERT(FALSE);
		}
		catch (...)	{
			LOG4CPLUS_ERROR(ProviderLog, _T("Unknown error occured while getContractType(long)"));
			ATLASSERT(FALSE);
		};
		return -1;
	};

	long getContractType(std::string symbol, bool reload = false){
		try{
			if (m_contracts.empty() || reload){
				if (Initialize()){

					m_contracts.clear();
					CStoredProc<CClientRecordset> rs(m_DbConnection, L"usp_Contract_Get");

					rs.Open();
					if (rs.GetRecordCount()){
						rs.MoveFirst();
						while (!rs.IsEOF()){
							std::string	bsSymbol = (char*)(_bstr_t(rs[L"vcSymbolName"]));
							m_contracts[bsSymbol] = static_cast<long>(rs[L"iContractTypeID"]);
							rs.MoveNext();
						}
					}

					if ( m_contracts.find(symbol) != m_contracts.end() ){
						return m_contracts[symbol];
					}
				}
			}
			else{
				if ( m_contracts.find(symbol) != m_contracts.end() ){
					return m_contracts[symbol];
				}
				return -1;
			}
		}
		catch (_com_error& err)	{
			LOG4CPLUS_ERROR(ProviderLog, _T("ComError: getContractType(std::string) ")<<(char*)(err.Description()));
			ATLASSERT(FALSE);
		}
		catch (...)	{
			LOG4CPLUS_ERROR(ProviderLog, _T("Unknown error occured while getContractType(std::string)"));
			ATLASSERT(FALSE);
		};
		return -1;
	};

	void setConnectionString(std::string ConnectionString){
		m_ConnectionString = ConnectionString;
	};

	bool Initialize(){
		try	{
			if(!m_DbConnection.IsOpened())
				m_DbConnection.Open(_bstr_t(m_ConnectionString.c_str()), 10, 120, 300, 300);
		}
		catch (_com_error& err)	{
			LOG4CPLUS_ERROR(ProviderLog, _T("ComError: CDatabaseImpl::GetContractType() ")<<(char*)(err.Description()));
			ATLASSERT(FALSE);
			return false;
		}
		catch (...)	{
			LOG4CPLUS_ERROR(ProviderLog, _T("Unknown error occured while CDatabaseImpl::GetContractType()"));
			ATLASSERT(FALSE);
			return false;
		};
		return true;
	};

private:
	std::string						m_ConnectionString;
	CDBConnection					m_DbConnection;
	std::map<std::string, long>		m_contracts;
};

class CDataBase
{
public:
	static CDatabaseImpl::pointer	getInstance(){
		if (!m_dbImpl){
			m_dbImpl = new CDatabaseImpl();
			LOG4CPLUS_INFO(ProviderLog, _T("DB instance created."));
		}
		return m_dbImpl;
	};
private:
	static CDatabaseImpl::pointer	m_dbImpl;
};
