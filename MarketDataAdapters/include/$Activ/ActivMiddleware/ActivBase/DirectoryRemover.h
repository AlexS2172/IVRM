/**
 *  @file	 DirectoryRemover.h
 *
 *  @brief   Directory remover class.
 *
 *  $Log: $
 */

#if !defined(ACTIV_DIRECTORY_REMOVER_H)
#define ACTIV_DIRECTORY_REMOVER_H

#include "ActivMiddleware/ActivBase/DirectoryWalker.h"

namespace Activ
{

/**
 *	@brief	Directory remover class.
 */
class ACTIV_BASE_API DirectoryRemover : private DirectoryWalker
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	DirectoryRemover();

	/**
	 *	@brief	Destructor.
	 */
	virtual ~DirectoryRemover();

	/**
	 *	@brief	Remove files from a directory tree.
	 *
	 *	@param	path the path to the directory to remove.
	 *	@param	recurseDirectory indicates whether the directory should be recursed.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_NOT_FOUND
	 */
	StatusCode Remove(const std::string &path, const bool isRecurseDirectory);

private:
	/**
	 *	@brief	Callback function for when directories are located in the directory walk.
	 *
	 *	@param	path the path to the directory.
	 *	@param	name the name of the directory.
	 *
	 *	@return	true to continue, false to stop (interrupt) the walk.
	 */
	virtual bool OnDirectory(const std::string &path, const std::string &name);

	/**
	 *	@brief	Callback function for when files are located in a directory walk.
	 *
	 *	@param	path the path to the file.
	 *	@param	name the name of the file.
	 *
	 *	@return	true to continue, false to stop (interrupt) the walk.
	 */
	virtual bool OnFile(const std::string &path, const std::string &name);

	bool m_isRecurseDirectory; ///< indicates whether the directory should be recursed.
};

} // namespace Activ

#endif // !defined(ACTIV_DIRECTORY_REMOVER_H)
