/**
 *	@file	DatabaseFactory.h
 *
 *	@brief	Header file for the database factory class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_DATABASE_FACTORY_H)
#define ACTIV_DATABASE_DATABASE_FACTORY_H

#include "ActivMiddleware/ActivDatabase/External.h"
#include "ActivMiddleware/ActivDatabase/IDatabase.h"

namespace Activ
{

namespace Database
{

/**
 *	@brief	Database factory class.
 */
class ACTIV_DATABASE_API DatabaseFactory
{
public:
	/**
	 *	@brief	Create a database class
	 *
	 *	@param	name the name of the database.
	 *
	 *
	 */
	IDatabase *Create(const std::string &name);

	/**
	 *	@brief	Get a reference to the singleton factory object.
	 */
	static DatabaseFactory &GetInstance();

private:
    /**
     *	@brief	Default constructor.
     */
	DatabaseFactory();	

	static DatabaseFactory	m_factory;	///< Singleton database factory.
};

} // namespace Database

} // namespace Activ

#endif // !defined (ACTIV_DATABASE_DATABASE_FACTORY_H)
