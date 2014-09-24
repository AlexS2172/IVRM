/**
 *	@file	MessageClasses.h
 *
 *	@brief	Message classes for content platform.
 *
 *	$Log: $
 *
 */

#if !defined (ACTIV_CONTENT_PLATFORM_MESSAGE_CLASSES_H)
#define ACTIV_CONTENT_PLATFORM_MESSAGE_CLASSES_H

#include "ActivMiddleware/ActivSystem/MessageTypes.h"

namespace Activ
{

/**
 *	@brief	List of available message classes used by ActivContentPlatform.
 */
#define ACTIV_CONTENT_PLATFORM_MESSAGE_CLASS_LIST(d)						\
	d(32,	MESSAGE_CLASS_CONTENT_SERVER,		"ContentServer")			\
	d(33,	MESSAGE_CLASS_CONTENT_GATEWAY,		"ContentGateway")			\
	d(34,	MESSAGE_CLASS_SYMBOL_DIRECTORY,		"SymbolDirectory")			\
	d(36,	MESSAGE_CLASS_HISTORY_MANAGER,		"History Manager")			\
	d(37,	MESSAGE_CLASS_INGEST,				"Ingest")

/**
 *	@brief	Declare list of constant message class values.
 */
ACTIV_CONTENT_PLATFORM_MESSAGE_CLASS_LIST(ACTIV_DECLARE_MESSAGE_CLASS)

} // namespace Activ

#endif // !defined (ACTIV_CONTENT_PLATFORM_MESSAGE_CLASSES_H)
