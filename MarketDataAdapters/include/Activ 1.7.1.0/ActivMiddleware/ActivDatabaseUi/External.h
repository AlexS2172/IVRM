/**
 *  @file	ActivDatabaseUi/External.h
 *
 *	@brief	Import rules for ActivDatabaseUi exports.
 *
 *  $Log: $
 */

#if !defined (ACTIV_DATABASE_UI_API)
#	if !defined (ACTIV_STATIC)
#		define ACTIV_DATABASE_UI_API ACTIV_IMPORT_DLL
#	else
#		define ACTIV_DATABASE_UI_API ACTIV_IMPORT_LIB
#	endif // !defined (ACTIV_STATIC)
#endif // !defined (ACTIV_DATABASE_UI_API)

#include "ActivMiddleware/Activ.h"
