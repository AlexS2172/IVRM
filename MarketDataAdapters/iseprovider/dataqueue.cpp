#include "stdafx.h"

#pragma hdrstop

#include "DataQueue.h"
#include "trace.h"

#pragma once

template<class _Key, class _Type>
void  CDataQueueNotBuffered<_Key, _Type>::Push(const _Key & Key, const _Type & Value)
{
	CReentry  Entry(&m_Lock);
/*
	if(m_Data.find(Key) != m_Data.end())
		Trace(_T("Override"));
*/
	m_Data[Key] = Value;

	SetEvent(m_hNotification);
}

template<class _Key, class _Type>
BOOL  CDataQueueNotBuffered<_Key, _Type>::Get(const _Key & Key, _Type & Value)
{
	CReentry  Entry(&m_Lock);

	data_queue_it_t It = m_Data[Key];
	if(It == m_Data.end())
		return FALSE;

	Value = It->second;

	return TRUE;
}

template<class _Key, class _Type>
BOOL  CDataQueueNotBuffered<_Key, _Type>::Pop(_Key & Key, _Type & Value)  
{
	CReentry  Entry(&m_Lock);

	if(m_It == m_Data.end())
		m_It = m_Data.begin();

	if(m_It == m_Data.end())
		return FALSE;

	Key = m_It->first;
	Value = m_It->second;

	data_queue_it_t DelIt = m_It;
	m_It++;

	m_Data.erase(DelIt);

	return TRUE;
}


template<class _Key, class _Type>
void  CDataQueueNotBuffered<_Key, _Type>::Clear()
{
	CReentry  Entry(&m_Lock);
	m_Data.clear();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class _Type>
void  CDataQueueBuffered<_Type>::Push(const _Type & Value)
{
	CReentry  Entry(&m_Lock);
	m_Data.push(Value);
	SetEvent(m_hNotification);
}

template<class _Type>
BOOL  CDataQueueBuffered<_Type>::Pop(_Type & Value)  
{
	CReentry  Entry(&m_Lock);

	if(m_Data.empty())
		return FALSE;

	Value = m_Data.front();
	m_Data.pop();

	return TRUE;
}

template<class _Type>
void  CDataQueueBuffered<_Type>::Clear()
{
	CReentry  Entry(&m_Lock);
	m_Data.clear();
}