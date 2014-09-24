#ifndef __HYPERFEEDTRACE_H__
#define __HYPERFEEDTRACE_H__

#pragma once

class CHyperFeedTrace :
	public EgStd::CEgTraceManager
{
public:
	CHyperFeedTrace(void);
	~CHyperFeedTrace(void);

private:
	virtual bool WINAPI Init();

	EgStd::CLock	m_ObjectLock;
};

#endif //__HYPERFEEDTRACE_H__

