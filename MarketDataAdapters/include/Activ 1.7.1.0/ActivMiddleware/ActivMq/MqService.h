/**
 *  @file	MqService.h
 *
 *	@brief
 *
 *	$Log: $
 */

#if !defined (ACTIV_MQ_SERVICE_H)
#define ACTIV_MQ_SERVICE_H

// ActivMq includes
#include "ActivMiddleware/ActivMq/External.h"
#include "ActivMiddleware/ActivMq/MqTypes.h"

// ActivFieldTypes includes
#include "ActivMiddleware/ActivFieldTypes/Date.h"

// System includes
#include <string>
#include <list>
#include <set>

namespace Activ
{

class LocalMq;
class IMutex;

class MqService
{
public:
	typedef std::pair<std::string, Activ::Date> StringDatePair;
	typedef std::set<StringDatePair> StringDateSet;

	ACTIV_MQ_API MqService(const std::string &rootDirectory);

	ACTIV_MQ_API ~MqService();

	ACTIV_MQ_API StatusCode OpenMq(const std::string &queueName, const Date &queueDate);

	ACTIV_MQ_API StatusCode CloseMq(const std::string &queueName, const Date &queueDate);

	ACTIV_MQ_API const std::string &GetRootDirectory();

	ACTIV_MQ_API StatusCode GetAvailableMqs(StringDateSet &messageQueues) const;

	static const std::string &GetIndexFileExtension(void);

	static const std::string &GetDataFileExtension(void);

private:
	typedef std::list<LocalMq *> LocalMqList;
	typedef LocalMqList::iterator LocalMqListIter;

	IMutex * const		m_pMutex;			///< Mutex
	const std::string	m_rootDirectory;	///< Directory to store data in
	LocalMqList			m_mqList;			///< List of MQs

	ACTIV_MQ_API static const std::string m_indexFileExtension;	///< File extension of index files
	ACTIV_MQ_API static const std::string m_dataFileExtension;	///< File extension of data files
};

// --------------------------------------------------------------------------------------------------------------------

inline const std::string &MqService::GetIndexFileExtension(void)
{
	return m_indexFileExtension;
}

// --------------------------------------------------------------------------------------------------------------------

inline const std::string &MqService::GetDataFileExtension(void)
{
	return m_dataFileExtension;
}

} // namespace Activ

#endif // !defined (ACTIV_MQ_SERVICE_H)
