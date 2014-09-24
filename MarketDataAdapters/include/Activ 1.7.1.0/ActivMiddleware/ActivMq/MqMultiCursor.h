/**
 *  @file	MqMultiCursor.h
 *
 *	@brief
 *
 *	$Log: $
 */

#if !defined (ACTIV_MQ_MULTI_CURSOR_H)
#define ACTIV_MQ_MULTI_CURSOR_H

#include "ActivMiddleware/ActivMq/External.h"
#include "ActivMiddleware/ActivMq/LocalMq.h"
#include "ActivMiddleware/ActivMq/MqCursor.h"

#include "ActivMiddleware/Misc/IntrusivePtrHelper.h"

#include <vector>
#include <set>
#include <functional>

namespace Activ
{

class MqService;
class MqRecord;

class MqMultiCursor
{
public:
	typedef std::vector<Activ::LocalMq::Settings> MqSettingsList;

	/**
	 *	@brief	Constructor
	 */
	ACTIV_MQ_API MqMultiCursor(MqService &mqService, const MqSettingsList &mqSettingsList);

	/**
	 *	@brief	Destructor
	 */
	ACTIV_MQ_API ~MqMultiCursor();

	/**
	 *	@brief	Get the next message from the cursor with the lowest order number
	 */
	ACTIV_MQ_API StatusCode GetNextMessage(MqRecord &mqRecord, const LocalMq *&localMq);

private:
	/**
	 *	@brief	Class to hold a Cursor and a copy of the message it is currently pointing at
	 */
	class CursorContainer
	{
	public:
		ACTIV_INTRUSIVE_PTR_HELPER_ADD_REFERENCE_COUNTER;

		/**
		 *	@brief	Constructor
		 */
		ACTIV_MQ_API CursorContainer(const LocalMq &localMq, MqMsn firstMsn);

		/**
		 *	@brief	Destructor
		 */
		ACTIV_MQ_API ~CursorContainer();

		/**
		 *	@brief	Get the next message from Cursor and store it
		 */
		StatusCode GetNextMessage(void);

		/**
		 *	@brief	Get the current message
		 */
		const MqRecord &GetCurrentMessage(void) const;

		/**
		 *	@brief	Get the Cursor
		 */
		const MqCursor &GetCursor(void) const;

		bool operator < (const CursorContainer &rhs) const;

	private:
		MqCursor							m_mqCursor;
		MqRecord							m_currentMessage;
	};

	/**
	 *	@brief	Class to perform less comparison on dereferenced pointers
	 */
	struct DereferenceLess
	{
		template <typename PtrType>
		bool operator()(const PtrType &lhs, const PtrType &rhs) const
		{
			return (*lhs < *rhs);
		}
	};

	ACTIV_DECLARE_INTRUSIVE_PTR_TYPEDEF(CursorContainer) CursorContainerPtr;	///< Instrusive ptr to CursorContainer type
	typedef std::set<CursorContainerPtr, DereferenceLess> CursorPtrSet;
	typedef std::vector<LocalMq *> LocalMqList;

	MqService &			m_mqService;
	LocalMqList			m_localMqList;
	CursorPtrSet		m_cursorSet;

}; // class MqMultiCursor

// --------------------------------------------------------------------------------------------------------------------

inline StatusCode MqMultiCursor::CursorContainer::GetNextMessage(void)
{
	return (m_mqCursor.GetNextMessage(m_currentMessage));
}

// ------------------------------------------------------------------------------------------------------------------------------

inline const MqRecord &MqMultiCursor::CursorContainer::GetCurrentMessage(void) const
{
	return m_currentMessage;
}

// ------------------------------------------------------------------------------------------------------------------------------

inline const MqCursor &MqMultiCursor::CursorContainer::GetCursor(void) const
{
	return m_mqCursor;
}

// ------------------------------------------------------------------------------------------------------------------------------

inline bool MqMultiCursor::CursorContainer::operator < (const CursorContainer &rhs) const
{
	return m_currentMessage < rhs.m_currentMessage;
}

} // namespace Activ

#endif // !defined (ACTIV_MQ_MULTI_CURSOR_H)
