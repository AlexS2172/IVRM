/**
 *  @file	 DirectoryRemover.h
 *
 *  @brief   Directory remover class.
 *
 *  $Log: $
 */

#if (!defined ACTIV_BASE_DIRECTORY_REMOVER_H)
#define ACTIV_BASE_DIRECTORY_REMOVER_H

#include "ActivMiddleware/ActivBase/DirectoryWalker.h"

namespace Activ
{

/**
 *	@brief	Directory remover class.
 */
class DirectoryRemover : private DirectoryWalker
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_BASE_API DirectoryRemover();

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_API virtual ~DirectoryRemover();

	/**
	 *	@brief	Remove files from a directory tree.
	 *
	 *	@param	path the path to the directory to remove.
	 *	@param	shouldRecurseDirectory indicates whether the directory should be recursed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	ACTIV_BASE_API StatusCode Remove(const std::string &path, const bool shouldRecurseDirectory);

protected:
	ACTIV_BASE_API virtual bool OnDirectory(const std::string &path, const std::string &name);
	ACTIV_BASE_API virtual bool OnFile(const std::string &path, const std::string &name);

private:
	bool m_shouldRecurseDirectory; 		///< indicates whether the directory should be recursed.
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_DIRECTORY_REMOVER_H)
