#include "StdAfx.h"
#include "resource.h"
#include "EodReportRec.h"

/////////////////////////////////////////////////////////////////////////////
//
CEodReportRec::CEodReportRec(void)
	: m_dtTime(0.),
	m_nType(0L)
{
}

/////////////////////////////////////////////////////////////////////////////
//
CEodReportRec::~CEodReportRec(void)
{
}

/////////////////////////////////////////////////////////////////////////////
//
bool CEodReportRec::ReadFromString(const CString& strRecord, long& nStep)
{
	int iStart = 0;
	CString strValue;
	if(strRecord.GetLength())
	{
		try{
			// date and time
			CString strDate;
			strValue = strRecord.Tokenize(_T("\t"), iStart).Trim();
			if(strValue != _T(""))
				m_dtTime.ParseDateTime(strValue, (DWORD)LOCALE_NOUSEROVERRIDE, LANG_SYSTEM_DEFAULT);

			// step id
			strValue = strRecord.Tokenize(_T("\t"), iStart).Trim();
			if(strValue != _T(""))
				nStep = _ttol(strValue);

			// message type id
			strValue = strRecord.Tokenize(_T("\t"), iStart).Trim();
			if(strValue != _T(""))
				m_nType = _ttol(strValue);

			// message
			if(iStart >= 0)
				m_strMessage = strRecord.Mid(iStart).Trim();
		}
		catch(...){
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//
long CEodReportRec::GetTypeIconResID() const
{
	switch(m_nType)
	{
	case enRpMtStart:
		return IDI_TYPE_START;
	case enRpMtMessage:
		return IDI_TYPE_INFO;
	case enRpMtWarning:
		return IDI_TYPE_WARNING;
	case enRpMtError:
		return IDI_TYPE_ERROR;
	case enRpMtFinishSuccess:
		return IDI_TYPE_END;
	case enRpMtFinishWithError:
		return IDI_TYPE_END_WITH_ERRORS;
	}
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
//
COLORREF CEodReportRec::GetTypeBackColor(long nType)
{
	switch(nType)
	{
	case enRpMtStart:
		return RGB(235, 235, 255);
	case enRpMtMessage:
		return RGB(235, 255, 255);
	case enRpMtWarning:
		return RGB(255, 255, 235);
	case enRpMtError:
		return RGB(255, 235, 255);
	case enRpMtFinishSuccess:
		return RGB(235, 255, 235);
	case enRpMtFinishWithError:
		return RGB(255, 235, 235);
	}
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
//
COLORREF CEodReportRec::GetTypeForeColor(long nType)
{
	switch(nType)
	{
	case enRpMtStart:
		return RGB(0, 0, 128);
	case enRpMtMessage:
		return RGB(0, 128, 128);
	case enRpMtWarning:
		return RGB(128, 128, 0);
	case enRpMtError:
		return RGB(128, 0, 128);
	case enRpMtFinishSuccess:
		return RGB(0, 128, 0);
	case enRpMtFinishWithError:
		return RGB(128, 0, 0);
	}
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
//
long CEodReportStep::_StepDescriptionID(long nStep)
{
	switch(nStep)
	{
	case enSmEmpty:
		return IDS_STEP_OVERALL;
	case enSmLoadAvgVolumes:
		return IDS_STEP_AVG_VOLUME;
	case enSmLoadIVBands:
		return IDS_STEP_IV_BAND;
	case enSmLoadSkewKurtosis:
		return IDS_STEP_SKEW_KURT;
	case enSmLoadOptClosePrice:
		return IDS_STEP_OPT_CLOSE_PRICE;
	case enSmLoadUndClosePrice:
		return IDS_STEP_UND_CLOSE_PRICE;
	case enSmLoadMktStructure:
		return IDS_STEP_MKT_STRUCTURE;
	case enSmCalculateVega:
		return IDS_STEP_CALC_WTD_VEGA;
	}
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
//
CString CEodReportStep::StepDescription(long nStep)
{
	CString strRet;
	long nID = _StepDescriptionID(nStep);

	if(nID)
		strRet.LoadString(nID);
	else
		strRet = _T("<Unknown>");

	return strRet;
}
