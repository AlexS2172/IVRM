/**
 *  @file	ActivIndexer/External.h
 *
 *	@brief	Import rules for ActivIndexer exports.
 *
 *  $Log: $
 */

#if !defined (ACTIV_INDEXER_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_INDEXER_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_INDEXER_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_INDEXER_API)

#include "ActivMiddleware/Activ.h"
