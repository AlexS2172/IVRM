/**
 *	@file	RemoteTableFactory.h
 *
 *	@brief	Header file for the remote table factory class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_REMOTE_TABLE_FACTORY_H)
#define ACTIV_DATABASE_REMOTE_TABLE_FACTORY_H

#include "ActivMiddleware/ActivDatabase/External.h"
#include "ActivMiddleware/ActivDatabase/Definitions.h"

namespace Activ
{

class MessageValidater;

namespace Database
{

class ITable;
	
/**
 *	@brief	Remote table factory class.
 */
class ACTIV_DATABASE_API RemoteTableFactory
{
public:
	/**
	 *	@brief	Create a remote table.
	 *
	 *	@param	messageValidater the message validater containing the remote table specification.
	 *
	 *	@return	a pointer to an interface to a table.
	 *
	 *	@throw	DatabaseException
	 */
	ITable *Create(MessageValidater &messageValidater);

	/**
	 *	@brief	Get a reference to the singleton factory object.
	 */
	static RemoteTableFactory &GetInstance();

private:
    /**
     *	@brief	Default constructor.
     */
	RemoteTableFactory();	

	static RemoteTableFactory	m_factory;	///< Singleton remote table factory.
};

} // namespace Database

} // namespace Activ

#endif // !defined (ACTIV_DATABASE_REMOTE_TABLE_FACTORY_H)
