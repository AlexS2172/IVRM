/**
 *  @file	TableResponses.h
 *
 *	@brief	Header file for the asynchronous table responses class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_TABLE_RESPONSES_H)
#define ACTIV_TABLE_RESPONSES_H

#include "ActivMiddleware/ActivServices/External.h"

namespace Activ
{

/**
 *	@brief	Asynchronous table response class.
 */
class TableResponses
{
public:
	/**
	 *	@brief	Destructor.
	 */
	virtual ~TableResponses();

	/**
	 *	@brief	Invoke the correct On*Response() by response.GetType().
	 *
	 *	@param	response the response message.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_UNRECOGNIZED.
	 */
	ACTIV_SERVICES_API virtual StatusCode InvokeCallback(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get equal response.
	 *
	 *	Call DeserializeGetResponse() to retrieve the key and record.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetEqualResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get multiple equal response.
	 *
	 *	Call DeserializeGetMultipleEqualResponse() to retrieve the records and status.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetMultipleEqualResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get first response.
	 *
	 *	Call DeserializeGetResponse() to retrieve the key and record.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetFirstResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get last response.
	 *
	 *	Call DeserializeGetResponse() to retrieve the key and record.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetLastResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get next response.
	 *
	 *	Call DeserializeGetResponse() to retrieve the key and record.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetNextResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get previous response.
	 *
	 *	Call DeserializeGetResponse() to retrieve the key and record.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetPreviousResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get first partial response.
	 *
	 *	Call DeserializeGetResponse() to retrieve the key and record.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetFirstPartialResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get last partial response.
	 *
	 *	Call DeserializeGetResponse() to retrieve the key and record.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetLastPartialResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get next partial response.
	 *
	 *	Call DeserializeGetResponse() to retrieve the key and record.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetNextPartialResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get previous partial response.
	 *
	 *	Call DeserializeGetResponse() to retrieve the key and record.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetPreviousPartialResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get multiple first response.
	 *
	 *	Call DeserializeGetMultipleResponse() to retrieve the key and records.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetMultipleFirstResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get multiple last response.
	 *
	 *	Call DeserializeGetMultipleResponse() to retrieve the key and records.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetMultipleLastResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get multiple next response.
	 *
	 *	Call DeserializeGetMultipleResponse() to retrieve the key and records.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetMultipleNextResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get multiple previous response.
	 *
	 *	Call DeserializeGetMultipleResponse() to retrieve the key and records.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetMultiplePreviousResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get multiple first partial response.
	 *
	 *	Call DeserializeGetMultipleResponse() to retrieve the key and records.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetMultipleFirstPartialResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get multiple last partial response.
	 *
	 *	Call DeserializeGetMultipleResponse() to retrieve the key and records.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetMultipleLastPartialResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get multiple next partial response.
	 *
	 *	Call DeserializeGetMultipleResponse() to retrieve the key and records.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetMultipleNextPartialResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous get multiple previous partial response.
	 *
	 *	Call DeserializeGetMultipleResponse() to retrieve the key and records.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnGetMultiplePreviousPartialResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous step first response.
	 *
	 *	Call DeserializeStepResponse() to retrieve the record.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnStepFirstResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous step next response.
	 *
	 *	Call DeserializeStepResponse() to retrieve the record.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnStepNextResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous step multiple first response.
	 *
	 *	Call DeserializeStepMultipleResponse() to retrieve the records.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnStepMultipleFirstResponse(HeapMessage &response);

	/**
	 *	@brief	Process asynchronous step multiple next response.
	 *
	 *	Call DeserializeStepMultipleResponse() to retrieve the records.
	 *
	 *	@param	response the response message.
	 */
	ACTIV_SERVICES_API virtual void OnStepMultipleNextResponse(HeapMessage &response);
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline TableResponses::~TableResponses()
{
}

} // namespace Activ

#endif // !defined (ACTIV_TABLE_RESPONSES_H)
