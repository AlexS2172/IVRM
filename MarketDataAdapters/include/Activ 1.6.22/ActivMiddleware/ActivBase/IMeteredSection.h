/**
 *	@file	IMeteredSection.h
 *
 *	@brief	Header file for the metered section interface class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IMETERED_SECTION_H)
#define ACTIV_IMETERED_SECTION_H

#include "ActivMiddleware/ActivBase/External.h"

namespace Activ
{

/**
 *	@brief	Abstract base metered section class.
 *  
 */
class IMeteredSection
{
public:
    /**
     *  Destructor.
     */
	virtual ~IMeteredSection() = 0;

    /**
     *	@brief	Enter the metered section.
	 *
	 *	@param	timeout how long to wait for the metered section, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_TIMEOUT
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
     */
	virtual StatusCode Enter(const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) = 0;

    /**
     *	@brief	Leave the metered section.
	 *
	 *	@param	releaseCount the count to release.
	 *	@param	pPreviousCount the previous count.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
     */
	virtual StatusCode Leave(const size_t releaseCount = 1, size_t *pPreviousCount = 0) = 0;

    /**
     *	@brief	Wait until the metered section is empty.
	 *
	 *	@param	timeout how long to wait for the metered section, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval	STATUS_CODE_TIMEOUT
     */
	virtual StatusCode WaitOnEmpty(const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) = 0;

	/**
     *	@brief	Clear the metered section.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
     */
	virtual StatusCode Clear() = 0;

	/**
     *	@brief	Get the metered section count.
	 *
	 *	@return	count the current count.
     */
	virtual size_t GetCount() const = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IMeteredSection::~IMeteredSection()
{
}

} // namespace Activ

#endif // !defined (ACTIV_IMETERED_SECTION_H)
