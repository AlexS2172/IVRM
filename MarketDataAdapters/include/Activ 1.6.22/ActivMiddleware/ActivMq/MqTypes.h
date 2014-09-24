/**
 *  @file	MqTypes.h
 *
 *	@brief
 *
 *	$Log: $
 */

#if !defined (ACTIV_MQ_TYPES_H)
#define ACTIV_MQ_TYPES_H

// ActivMq includes
#include "ActivMiddleware/Activ.h"

namespace Activ
{

typedef uint16_t MqSectionIndex;
typedef uint32_t MqSectionOffset;
typedef uint32_t MqMsn;

#pragma pack(push, 1)

struct MsnIndexEntry
{
	MqSectionIndex		m_sectionIndex;
	MqSectionOffset		m_sectionOffset;
	uint32_t			m_messageLength;
};

struct MessageHeader // 23 bytes big so far
{
    byte_t		m_version;
    byte_t		m_status;
	MqMsn		m_msn;
	MqMsn		m_orderNumber;
	byte_t		m_bootNumber;	///< not currently used
	uint32_t	m_storeDate;	///< Date (in julian format) that record was added
	uint32_t	m_storeTime;	///< Time (in milliseconds since midnight) that record was added
	uint32_t	m_crc;			///< not currently used
	uint32_t	m_bodyLength;
};

enum MessageStatus
{
	MESSAGE_STATUS_PRESENT,
	MESSAGE_STATUS_MISSING,
	MESSAGE_STATUS_DELETED
};

#pragma pack(pop)

} // namespace Activ

#endif // !defined (ACTIV_MQ_TYPES_H)
