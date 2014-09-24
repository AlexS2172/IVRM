#ifndef FIXGAP_H
#define FIXGAP_H

#include "FixMessage.h"

class CFixGap : private CLock
{
private:

	set<int>			m_setSeqNums;
	deque<CFixMessage>	m_dequeMessages;

public:

	void	FillBySeqNum(const int iSeqNum);
	
	void	FillByReset(const int iSeqNum);

	void	AddSeqNumToBeFilled(const int iSeqNum);

	bool	IsFilled();

	void	Clear();

	void	Push(const CFixMessage& Msg);

	bool	Pop(CFixMessage& Msg);
};


#endif //FIXGAP_H