#include "StdAfx.h"
#include "resource.h"
#include "FixReportRec.h"

/////////////////////////////////////////////////////////////////////////////
//
CFixReportRec::CFixReportRec(void)
	: m_dtTime(0.),
	m_nType(enFgLogStart)
{
}

/////////////////////////////////////////////////////////////////////////////
//
CFixReportRec::~CFixReportRec(void)
{
}

/////////////////////////////////////////////////////////////////////////////
//
long CFixReportRec::GetTypeIconResID() const
{
	switch(m_nType)
	{
	case enFgLogStart:
		return IDI_TYPE_START;
	case enFgLogInfo:
		return IDI_TYPE_INFO;
	case enFgLogWarning:
		return IDI_TYPE_WARNING;
	case enFgLogError:
		return IDI_TYPE_ERROR;
	case enFgLogStop:
		return IDI_TYPE_END;
	}
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
//
COLORREF CFixReportRec::GetTypeBackColor(long nType)
{
	switch(nType)
	{
	case enFgLogStart:
		return RGB(235, 235, 255);
	case enFgLogInfo:
		return RGB(235, 255, 255);
	case enFgLogWarning:
		return RGB(255, 255, 235);
	case enFgLogError:
		return RGB(255, 235, 255);
	case enFgLogStop:
		return RGB(235, 255, 235);
	}
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
//
COLORREF CFixReportRec::GetTypeForeColor(long nType)
{
	switch(nType)
	{
	case enFgLogStart:
		return RGB(0, 0, 128);
	case enFgLogInfo:
		return RGB(0, 128, 128);
	case enFgLogWarning:
		return RGB(128, 128, 0);
	case enFgLogError:
		return RGB(128, 0, 128);
	case enFgLogStop:
		return RGB(0, 128, 0);
	}
	return 0L;
}