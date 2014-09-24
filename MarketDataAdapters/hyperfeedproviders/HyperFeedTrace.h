#ifndef __HYPERFEEDTRACE_H__
#define __HYPERFEEDTRACE_H__

#pragma once
//#include <EgStdLib/trace.h>

class CHyperFeedTrace : public EgLib::CEgLibTraceManager
{
public:
	CHyperFeedTrace(void);
	~CHyperFeedTrace(void);

private:
	//virtual bool WINAPI Init();

	//EgLib::CLock	m_ObjectLock;
};

#endif //__HYPERFEEDTRACE_H__

