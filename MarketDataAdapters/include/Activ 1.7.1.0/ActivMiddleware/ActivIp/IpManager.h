/**
 *	@file	ActivIp/IpManager.h
 *
 *	@brief	Header file for base ip manager class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_MANAGER_H)
#define ACTIV_IP_MANAGER_H

#include "External.h"

#if defined (ACTIV_PLATFORM_WINDOWS)
#include "Windows/IpManager.h"
#elif defined (ACTIV_PLATFORM_POSIX)
#include "Posix/IpManager.h"
#else
#error No IpManager.h for this platform yet.
#endif

namespace Activ
{

inline const IpManager::Settings &IpManager::GetSettings() const
{
	return m_settings;
}

} // namespace Activ

#endif // !defined (ACTIV_IP_MANAGER_H)
