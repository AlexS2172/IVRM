// IVTrace.h: interface for the CIVTrace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IVTRACE_H__57F42E66_2DBD_47F0_979A_372DAD071A72__INCLUDED_)
#define AFX_IVTRACE_H__57F42E66_2DBD_47F0_979A_372DAD071A72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIVTrace:
	public EgStd::CEgTraceManager
{

public:
	CIVTrace();
	virtual ~CIVTrace();

private:

	virtual bool WINAPI Init();
	EgStd::CLock	m_ObjectLock;

};

#endif // !defined(AFX_IVTRACE_H__57F42E66_2DBD_47F0_979A_372DAD071A72__INCLUDED_)
