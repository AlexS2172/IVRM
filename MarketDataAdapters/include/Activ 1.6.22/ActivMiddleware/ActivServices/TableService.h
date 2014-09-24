/**
 *  @file	TableService.h
 *
 *  @brief	Activ table service class
 *
 *  $Log: $
 */

#if !defined (ACTIV_TABLE_SERVICE_H)
#define ACTIV_TABLE_SERVICE_H

#include "ActivMiddleware/ActivServices/External.h"
#include "ActivMiddleware/ActivDatabase/ITable.h"
#include "ActivMiddleware/ActivDatabase/Record.h"
#include "ActivMiddleware/ActivDatabase/Key.h"

namespace Activ
{

/**
 *	@brief	Remotable table service.
 */
class TableService : public Service
{
public:
	/**
	 *	@brief	Settings for the table service.
	 */
	class Settings
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_SERVICES_API Settings();

		std::string			m_name;								///< name of the service
		ServiceInstance		m_serviceInstance;					///< id and access points for the table
		Database::ITable *	m_pTable;							///< table to expose
		IHeap *				m_pHeap;							///< default is system heap
		Address				m_requestLogAddress;				///< address of a log to log all requests to, or if not defined, don't log
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	Attaches this service to a managed endpoint at the next available slot.
	 *
	 *	@param	application Application object to attach to.
	 *	@param	managedEndPoint reference to a ManagedEndPoint object to which this port is to be attached.
	 *	@param	settings settings for the service.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_SERVICES_API TableService(Application &application, ManagedEndPoint &managedEndPoint, const Settings &settings);

protected:
	ACTIV_SERVICES_API virtual StatusCode OnRequest(HeapMessage &request);
	ACTIV_SERVICES_API virtual StatusCode ProcessOpenServiceRequest(HeapMessage &acceptMessage);

private:
	/**
	 *	@brief	Process request call add function that takes a const record and return results.
	 *
	 *	@param	request the request message.
	 */
	void ProcessRequestAdd(HeapMessage &request);

	/**
	 *	@brief	Process request call delete function that takes a const key and return results.
	 *
	 *	@param	request the request message.
	 */
	void ProcessRequestDelete(HeapMessage &request);

	/**
	 *	@brief	Process request call get table access function that takes a const key and return results.
	 *
	 *	@param	request the request message.
	 *	@param	pointer to get function to call.
	 */
	void ProcessRequestGet(HeapMessage &request, StatusCode (Database::ITable::*GetFunction)(const Database::Key &key, Database::Record &record) const);

	/**
	 *	@brief	Process request call get table access function and return results.
	 *
	 *	@param	request the request message.
	 *	@param	pointer to get function to call.
	 */
	void ProcessRequestGet(HeapMessage &request, StatusCode (Database::ITable::*GetFunction)(Database::Key &key, Database::Record &record) const);

	/**
	 *	@brief	Process request call get partial table access function and return results.
	 *
	 *	@param	request the request message.
	 *	@param	pointer to get function to call.
	 */
	void ProcessRequestGet(HeapMessage &request, StatusCode (Database::ITable::*GetFunction)(Database::Key &key, Database::Record &record, Database::FieldSpecification::Id lastKeyFieldId, size_t lastKeyCompareLength) const);

	/**
	 *	@brief	Process request call get table access function that takes a const key and return results.
	 *
	 *	@param	request the request message.
	 *	@param	pointer to get function to call.
	 */
	void ProcessRequestGet(HeapMessage &request, StatusCode (Database::ITable::*GetFunction)(size_t &value) const);

	/**
	 *	@brief	Process request call get multiple records equal to a list of specified keys and return results.
	 *
	 *	@param	request the request message.
	 *	@param	pointer to multiple get function to call.
	 */
	void ProcessRequestGetMultiple(HeapMessage &request, StatusCode (Database::ITable::*GetFunction)(const std::vector<Database::Key> &keyList, std::vector<StatusCode> &statusCodeList, std::vector<Database::Record> &recordList) const);

	/**
	 *	@brief	Process request call get multiple records matching a partially specified key and return results.
	 *
	 *	@param	pRequest actual request.
	 *	@param	pointer to multiple get function to call.
	 */
	void ProcessRequestGetMultiple(HeapMessage &request, StatusCode (Database::ITable::*GetFunction)(Database::Key &key, std::vector<Database::Record> &recordList, const size_t maxResponseRecords) const);

	/**
	 *	@brief	Process request call get multiple records matching a partially specified key and return results.
	 *
	 *	@param	pRequest actual request.
	 *	@param	pointer to multiple get function to call.
	 */
	void ProcessRequestGetMultiple(HeapMessage &request, StatusCode (Database::ITable::*GetFunction)(Database::Key &key, std::vector<Database::Record> &recordList, const Database::FieldSpecification::Id lastKeyFieldId, const size_t lastKeyCompareLength, const size_t maxResponseRecords) const);

	/**
	 *	@brief	Process request call step table access function and return results.
	 *
	 *	@param	request the request message.
	 *	@param	pointer to step function to call.
	 */
	void ProcessRequestStep(HeapMessage &request, StatusCode (Database::ITable::*StepFunction)(Database::DataId &recordId, Database::Record &record) const);

	/**
	 *	@brief	Process request call step multiple records matching a partially specified key and return results.
	 *
	 *	@param	pRequest actual request.
	 *	@param	pointer to multiple step function to call.
	 */
	void ProcessRequestStepMultiple(HeapMessage &request, StatusCode (Database::ITable::*StepFunction)(Database::DataId &recordId, std::vector<Database::Record> &recordList, const size_t maxResponseRecords) const);

	ACTIV_DECLARE_NESTED_CLASS(RequestLogHelper);

	const Settings	m_settings;									///< settings for the table service
};

} // namespace Activ

#endif // !defined(ACTIV_TABLE_SERVICE_H)
