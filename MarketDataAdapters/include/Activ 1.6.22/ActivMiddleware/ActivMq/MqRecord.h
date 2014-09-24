/**
 *  @file	MqRecord.h
 *
 *	@brief
 *
 *	$Log: $
 */

#if !defined (ACTIV_MQ_RECORD_H)
#define ACTIV_MQ_RECORD_H

#include "ActivMiddleware/ActivMq/External.h"
#include "ActivMiddleware/ActivMq/MqTypes.h"

#include "ActivMiddleware/ActivBase/ScatterBufferHelper.h"

#include <vector>

namespace Activ
{

class MqRecord
{
public:
	ACTIV_MQ_API MqRecord(size_t initialBufferSize = 1024);
	ACTIV_MQ_API ~MqRecord();

	ACTIV_MQ_API StatusCode AddData(size_t offset, ScatterBufferHelper &scatterBufferHelper);

	const MessageHeader &GetHeader() const;
	const void *GetData() const;
	size_t GetLength() const;

	bool operator < (const MqRecord &rhs) const;

private:
	typedef std::vector<byte_t> ByteVector;

	MessageHeader	m_header;
	ByteVector		m_buffer;
};

// --------------------------------------------------------------------------------------------------------------------

inline const MessageHeader &MqRecord::GetHeader() const
{
	return m_header;
}

// --------------------------------------------------------------------------------------------------------------------

inline const void *MqRecord::GetData() const
{
	return &(m_buffer[0]);
}

// --------------------------------------------------------------------------------------------------------------------

inline size_t MqRecord::GetLength() const
{
	return m_header.m_bodyLength;
}

// --------------------------------------------------------------------------------------------------------------------

inline bool MqRecord::operator < (const MqRecord &rhs) const
{
	// Order by store date, store time then order number
	if (m_header.m_storeDate == rhs.m_header.m_storeDate)
	{
		if (m_header.m_storeTime == rhs.m_header.m_storeTime)
			return (m_header.m_orderNumber < rhs.m_header.m_orderNumber);

		return (m_header.m_storeTime < rhs.m_header.m_storeTime);
	}

	return (m_header.m_storeDate < rhs.m_header.m_storeDate);
}

}; // namespace Activ

#endif // !defined (ACTIV_MQ_RECORD_H)
