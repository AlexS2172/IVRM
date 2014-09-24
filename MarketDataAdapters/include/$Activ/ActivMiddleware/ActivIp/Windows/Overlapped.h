/**
 *	@file	Windows/Overlapped.h
 *
 *	@brief	Header file for overlapped class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_IP_WINDOWS_OVERLAPPED_H)
#define ACTIV_IP_WINDOWS_OVERLAPPED_H

#include "ActivMiddleware/ActivIp/External.h"

namespace Activ
{

class IpSocket;

/**
 *	@brief	Overlapped class.
 */
class ACTIV_IP_API Overlapped : public OVERLAPPED
{
public:
	/**
	 *	@brief	Default constructor.
	 *
	 *	@param	ipSocket the IpSocket object containing this Overlapped object.
	 */
	Overlapped(IpSocket &ipSocket);

	IpSocket &	m_ipSocket;									///< reference back to the owning object
	bool		m_posted;										///< overlapped operation posted?
	DWORD		m_numberOfBytes;								///< number of bytes xferred in the overlapped operation
	DWORD		m_errorCode;									///< result of the operation, or NO_ERROR
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline Overlapped::Overlapped(IpSocket &ipSocket) :
	m_ipSocket(ipSocket),
	m_posted(false)
{
	OVERLAPPED *pOverlapped = static_cast<OVERLAPPED *>(this);
	
	::memset(pOverlapped, 0, sizeof(OVERLAPPED));
}

} // namespace Activ

#endif // !defined (ACTIV_IP_WINDOWS_OVERLAPPED_H)
