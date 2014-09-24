/**
 *  @file	MeteredSectionFactory.h
 *
 *	@brief	Header file for the metered section factory class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_METERED_SECTION_FACTORY_H)
#define ACTIV_METERED_SECTION_FACTORY_H

#include "ActivMiddleware/ActivBase/IMeteredSection.h"
#include "ActivMiddleware/ActivBase/Timeout.h"

#include <string>

namespace Activ
{

/**
 *	@brief	Metered section factory class.
 */
class MeteredSectionFactory
{
public:
	/**
     *	@brief	Create a metered section.
	 *
	 *	@param	name the metered section name.
	 *	@param	initialCount the initial count.
	 *	@param	maximumCount the maximum count.
	 *
	 *	@return	pointer to an object providing an IMeteredSection interface.
 	 *
	 *	@throw	ActivException
	 *	@throw	BaseException
	 *	@throw	OsException
     */
	ACTIV_BASE_API IMeteredSection *Create(const std::string &name, const size_t initialCount, const size_t maximumCount);

	/**
     *	@brief	Open an existing metered section.
	 *
	 *	@param	pName the metered section name.
	 *	@param	timeout how long to wait for the object to exist, in ms.
	 *
	 *	@return	pointer to an object providing an IMeteredSection interface.
 	 *
	 *	@throw	ActivException
	 *	@throw	BaseException
	 *	@throw	OsException
     */
	ACTIV_BASE_API IMeteredSection *Open(const std::string &name, Timeout &timeout = Timeout::GetInfinite());
	
	/**
	 *	@brief	Get the instance of the factory object.
	 */
	ACTIV_BASE_API static MeteredSectionFactory &GetInstance();
	
private:
    /**
     *	@brief	Default constructor.
     */
	MeteredSectionFactory();
	
	static MeteredSectionFactory  m_factory;					///< Singleton metered section factory.
};

} // namespace Activ

#endif // !defined (ACTIV_METERED_SECTION_FACTORY_H)
