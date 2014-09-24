#include "stdafx.h"

#include "FixGap.h"

void	CFixGap::FillBySeqNum(const int iSeqNum)
{
	CAutoLock Lock(this);

	set<int>::iterator it = m_setSeqNums.find(iSeqNum);
	if(it != m_setSeqNums.end())
		m_setSeqNums.erase(it);
};

void	CFixGap::FillByReset(const int iSeqNum)
{
	CAutoLock Lock(this);

	for(set<int>::iterator it = m_setSeqNums.begin(); it != m_setSeqNums.end(); )
	{
		if(*it <= iSeqNum)
		{
			set<int>::iterator itDel = it;
			it++;
			m_setSeqNums.erase(itDel);
		}
		else
		{
			break;
		}
	}
};

void	CFixGap::AddSeqNumToBeFilled(const int iSeqNum)
{
	CAutoLock Lock(this);

	m_setSeqNums.insert(iSeqNum);
};

bool	CFixGap::IsFilled()
{
	CAutoLock Lock(this);

	return m_setSeqNums.empty();
};

void	CFixGap::Clear()
{
	CAutoLock Lock(this);

	m_setSeqNums.clear();
	m_dequeMessages.clear();
};

void	CFixGap::Push(const CFixMessage& Msg)
{
	CAutoLock Lock(this);

	m_dequeMessages.push_back(Msg);	
};

bool	CFixGap::Pop(CFixMessage& Msg)
{
	CAutoLock Lock(this);
	if(m_dequeMessages.empty())
		return false;

	Msg = m_dequeMessages.front();	
	m_dequeMessages.pop_front();

	return true;
};