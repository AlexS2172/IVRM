/**
 *  @file	BasicTypes.h
 *
 *	@brief	Basic types for ActivSystem.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SYSTEM_BASIC_TYPES_H)
#define ACTIV_SYSTEM_BASIC_TYPES_H

#include "ActivMiddleware/Activ.h"

#include <set>

namespace Activ
{

typedef uint16_t MessageType;									///< message type
typedef uintptr_t MessageId;									///< messageId has to be the size of a pointer as in local heaps it is one
typedef byte_t BucketId;										///< type of bucket ids
typedef uint32_t HeapId;										///< type of heap ids
typedef uint16_t PortId;										///< type of port ids
typedef uint16_t EndPointId;									///< type of endpoint ids
typedef uint16_t ProcessId;										///< type of process ids
typedef std::set<EndPointId> EndPointIdSet;

const MessageType INVALID_MESSAGE_TYPE = ~0;					///< invalid message type
const MessageId INVALID_MESSAGE_ID = ~0;						///< invalid message id
const HeapId INVALID_HEAP_ID = ~0;								///< invalid heap id
const PortId INVALID_PORT_ID = ~0;								///< invalid port id
const EndPointId INVALID_ENDPOINT_ID = ~0;						///< invalid endpoint id
const ProcessId INVALID_PROCESS_ID = ~0;						///< invalid process id

} // namespace Activ

#endif // !defined (ACTIV_SYSTEM_BASIC_TYPES_H)
