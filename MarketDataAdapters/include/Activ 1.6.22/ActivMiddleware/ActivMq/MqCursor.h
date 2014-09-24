/**
 *  @file	MqCursor.h
 *
 *	@brief
 *
 *	$Log: $
 */

#if !defined (ACTIV_MQ_CURSOR_H)
#define ACTIV_MQ_CURSOR_H

// ActivMq includes
#include "ActivMiddleware/ActivMq/External.h"
#include "ActivMiddleware/ActivMq/MqTypes.h"

namespace Activ
{

class LocalMq;
class MqRecord;

class MqCursor
{
public:
	ACTIV_MQ_API MqCursor(const LocalMq &localMq, MqMsn firstMsn);

	ACTIV_MQ_API ~MqCursor();

	ACTIV_MQ_API StatusCode GetNextMessage(MqRecord &mqRecord);

	void Reset();

	const LocalMq &GetLocalMq() const;

private:
	const LocalMq &		m_localMq;
	MqMsn				m_firstMsn;			///< First message to retreive
	MqMsn				m_nextMsn;			///< Next message to retrieve
};

// --------------------------------------------------------------------------------------------------------------------

inline void MqCursor::Reset()
{
	m_nextMsn = m_firstMsn;
}

// --------------------------------------------------------------------------------------------------------------------

inline const LocalMq &MqCursor::GetLocalMq() const
{
	return m_localMq;
}

} // namespace Activ

#endif // !defined (ACTIV_MQ_CURSOR_H)
