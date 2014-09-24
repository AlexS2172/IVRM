/**
 *	@file	SyncRequestId.h
 *
 *	@brief	Header file for synchronous request id class.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SYNC_REQUEST_ID_H)
#define ACTIV_SYNC_REQUEST_ID_H

#include "ActivMiddleware/ActivSystem/External.h"
#include "ActivMiddleware/ActivSystem/RequestId.h"

namespace Activ
{

class HeapMessage;
class SystemImpl;
class ISyncEndPoint;

/**
 *	@brief	Synchronous request id class.
 */
class SyncRequestId
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_SYSTEM_API SyncRequestId();

	/**
	 *	@brief	Copy constructor.
	 *
	 *	Ownership of an attached endpoint is transferred to the new object.
	 *
	 *	@param	syncRequestId the SyncRequestId to copy from.
	 */
	ACTIV_SYSTEM_API SyncRequestId(SyncRequestId &syncRequestId);

	/**
	 *	@brief	Destructor. An attached endpoint is returned to the system pool.
	 */
	ACTIV_SYSTEM_API ~SyncRequestId();

	/**
	 *	@brief	Explicitly return an attached sync endpoint to the system pool.
	 */
	ACTIV_SYSTEM_API void Return();

	/**
	 *	@brief	Wait for the response to a sync call.
	 *
	 *	@param	heapMessage reference to a HeapMessage object to receive the response.
	 *			If the call times out, heapMessage will be left unchanged.
	 *	@param	timeout how long to wait for the response, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS got the response in \a heapMessage.
	 *	@retval	STATUS_CODE_TIMEOUT no response received in timeout period, \a heapMessage is empty.
	 *	@retval	STATUS_CODE_INVALID_ENDPOINT.
	 */
	ACTIV_SYSTEM_API StatusCode GetResponse(HeapMessage &heapMessage, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE);

	/**
	 *	@brief	Get the underlying RequestId object.
	 */
	ACTIV_SYSTEM_API const RequestId &GetRequestId() const;

	/**
	 *	@brief	Assignment operator.
	 *
	 *	@param	rhs SyncRequestId object to copy from.
	 */
	ACTIV_SYSTEM_API SyncRequestId &operator=(SyncRequestId &rhs);

private:
	/**
	 *	@brief	Allocate an endpoint and send id from the system.
	 */
	void Set(SystemImpl * const pSystem, const RequestId::SendId sendId, ISyncEndPoint * const pEndPoint);

	SystemImpl *	m_pSystem;
	ISyncEndPoint *	m_pEndPoint;
	RequestId		m_requestId;

	friend class SystemImpl;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline const RequestId &SyncRequestId::GetRequestId() const
{
	return m_requestId;
}

} // namespace Activ

#endif // !defined (ACTIV_SYNC_REQUEST_ID_H)
