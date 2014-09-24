/**
 *  @file	 DirectoryWalker.h
 *
 *  @brief   Directory walker class.
 *
 *  $Log: $
 */

#if !defined(ACTIV_DIRECTORY_WALKER_H)
#define ACTIV_DIRECTORY_WALKER_H

#include "ActivMiddleware/ActivBase/External.h"

#include <string>

namespace Activ
{

/**
 *	@brief	Directory walker class.
 */
class DirectoryWalker
{
public:
	/**
	 *	@brief	Constructor.
	 */
	ACTIV_BASE_API DirectoryWalker();

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_API virtual ~DirectoryWalker();

	/**
	 *	@brief	Walk a directory tree.
	 *
	 *	@param	path the path to the directory to walk.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_INTERRUPTED
	 */
	ACTIV_BASE_API StatusCode Walk(const std::string &path);

protected:
	/**
	 *	@brief	Callback function for when directories are located in the directory walk.
	 *
	 *	@param	path the path to the directory.
	 *	@param	name the name of the directory.
	 *
	 *	@return	true to continue, false to stop (interrupt) the walk.
	 */
	ACTIV_BASE_API virtual bool OnDirectory(const std::string &path, const std::string &name);

	/**
	 *	@brief	Callback function for when files are located in a directory walk.
	 *
	 *	@param	path the path to the file.
	 *	@param	name the name of the file.
	 *
	 *	@return	true to continue, false to stop (interrupt) the walk.
	 */
	ACTIV_BASE_API virtual bool OnFile(const std::string &path, const std::string &name);
};

} // namespace Activ

#endif
