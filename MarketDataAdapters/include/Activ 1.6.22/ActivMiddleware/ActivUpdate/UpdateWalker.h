/**
 *	@file	UpdateWalker.h
 *
 *	@brief	Header file for the update walker class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_UPDATE_WALKER_H)
#define ACTIV_UPDATE_WALKER_H

#include "ActivMiddleware/ActivUpdate/External.h"

// ActivUpdate includes
#include "ActivMiddleware/ActivUpdate/UpdateDescription.h"

// ActivBase includes
#include "ActivMiddleware/ActivBase/DirectoryWalker.h"

namespace Activ
{

/**
 *	@brief	Update walker class.
 */
class UpdateWalker : public DirectoryWalker
{
public:
	/**
	 *	@brief	On update callback.
	 *
	 *	@param	updateDescriptionFileName the ini file name.
	 *	@param	updateDescription the update description.
	 *
	 *	@return	true to continue walk, false to stop.
	 */
	virtual bool OnUpdate(const std::string &updateDescriptionFileName, const UpdateDescription &updateDescription) = 0;

private:
	// DirectoryWalker interface
	virtual bool OnFile(const std::string &path, const std::string &name);
};

} // namespace Activ

#endif // !defined (ACTIV_UPDATE_WALKER_H)
