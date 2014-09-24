// Options.h : Declaration of the COptions

#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include "Extended.h"
#include "HyperFeedBase.h"

/////////////////////////////////////////////////////////////////////////////
// COptions

class COptions : 
	virtual public CExtended
{
public:
	COptions();
	virtual ~COptions();

protected:
	void GetOptions(BSTR bsUnderlying);
	void CancelRequest();

	virtual void PublicGetOptionError(ErrorNumberEnum nError, BSTR bsDescription){};
	virtual void PublicLastOption(){};
	virtual void PublicOption(DBA_OPTIONS_FILTER_RECORD* pData) = 0;

	void CopyOptionFilterToInfo(const DBA_OPTIONS_FILTER_RECORD& rec, _OptionInfo& Info);
	void CopyOptionFilterToResult(const DBA_OPTIONS_FILTER_RECORD& rec, _OptionResults& Result);
	void CopyOptionFilterToResult(const DBA_OPTIONS_FILTER_RECORD& rec, _OptionResultsEx& Result);
	void CopyOptionFilterToIvResult(const DBA_OPTIONS_FILTER_RECORD& rec, _OptionForIvResult& Result);

private:
	void _SetDbKey(BSTR bsUnderlying);
	long _GetOptions();
	DATE _CreateExpiryDate(INT nRelativeMonth);

	static long CALLBACK _OptionsProcStatic(VOID *pblk, ULONG ulFunction, DBA_KEY *pDbaKey, int nKeyLen, DBA_OPTIONS_FILTER_RECORD *pDbaRec, int nRecLen, ULONG ulKey, DWORD dwStatus, void *p);
	long _OptionsProc(ULONG ulFunction, DBA_KEY *pDbaKey, DBA_OPTIONS_FILTER_RECORD *pDbaRec, DWORD dwStatus);
private:

	OPTIONS_FILTER			m_optionsFilter;	
	bool					m_bIsGotOptions;
	bool					m_bIsGotError;
	volatile bool			m_bTerminate;

public:
	void OnRemoveAll()
	{
		m_bTerminate             = true;
	}


};

#endif //__OPTIONS_H__
