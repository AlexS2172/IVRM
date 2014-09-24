// NumericEditEx.cpp : implementation file
//

#include "stdafx.h"
#include "NumericEditEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNumericEditEx

IMPLEMENT_DYNAMIC(CNumericEditEx, CEditEx)

BEGIN_MESSAGE_MAP(CNumericEditEx, CEditEx)
	//{{AFX_MSG_MAP(CNumericEditEx)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(EG_SM_VALUECHANGED, OnEgSmValueChanged)
	ON_REGISTERED_MESSAGE(EG_SM_MOUSEENTER, OnEgSmMouseEnter)
	ON_REGISTERED_MESSAGE(EG_SM_MOUSEEXIT, OnEgSmMouseExit)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////
// Constructor
CNumericEditEx::CNumericEditEx()
{
	m_pSpin = NULL;
	m_bMouseOverSpin = FALSE;

	m_bNegativeValue=FALSE;

	m_clrTextNegative=m_clrText;

	m_bForceToUsePeriod=TRUE;

	// default values
	//
	m_nDecimalDigitCount=-1;
	m_chGroupSeparator=_T(',');
	m_nGroupLength=3;	
	m_chDecimalSeparator=_T('.');	
	m_bShowLeadingZero=TRUE;	
	m_nFractionalDigitCount=2;
	m_sFormatNegative=_T("-1.1");
	m_sFormatPositive=_T("1.1");

	m_nDecimalDigitCountDefault=-1;
	m_chGroupSeparatorDefault=_T(',');
	m_nGroupLengthDefault=3;	
	m_chDecimalSeparatorDefault=_T('.');	
	m_bShowLeadingZeroDefault=TRUE;	
	m_nFractionalDigitCountDefault=2;
	m_sFormatNegativeDefault=_T("-1.1");
	m_sFormatPositiveDefault=_T("1.1");
}

BOOL CNumericEditEx::InitControl()
{
	if(!CEditEx::InitControl())
		return FALSE;
	
	return TRUE;
}

void CNumericEditEx::DeleteContents()
{
	CEditEx::DeleteContents();
	m_dValue=0.0;
}

int CNumericEditEx::InsertAt(int nSelectionStart, TCHAR chNewChar)
{
	return TranslateSymbol(nSelectionStart,chNewChar,FALSE);
}

int CNumericEditEx::SetAt(int nSelectionStart, TCHAR chNewChar)
{
	return TranslateSymbol(nSelectionStart,chNewChar,TRUE);
}

int CNumericEditEx::DeleteRange(int& nSelectionStart, int& nSelectionEnd)
{
	ASSERT(nSelectionStart>=0);
	ASSERT(nSelectionStart<=nSelectionEnd);

	int nDeletedCount=0;
	int nDecimalDelimiterPos=FindSymbol(GetDecimalSeparator());
	if(nDecimalDelimiterPos==-1 || nDecimalDelimiterPos<nSelectionStart ||
		nDecimalDelimiterPos>nSelectionEnd)
	{
		nDeletedCount=CEditEx::DeleteRange(nSelectionStart,nSelectionEnd);
		if(nDeletedCount>0)
			ValidateInput();
	}
	else
	{
		if(nDecimalDelimiterPos<nSelectionEnd)
		{
			int nPosition=nDecimalDelimiterPos+1;
			nDeletedCount=CEditEx::DeleteRange(nPosition,nSelectionEnd);
		}
		if(nDecimalDelimiterPos>nSelectionStart)
		{
			nDeletedCount+=CEditEx::DeleteRange(nSelectionStart,
				nDecimalDelimiterPos);
		}
		if(nDeletedCount>0)
			ValidateInput();
	}

	return nDeletedCount;
}

CString CNumericEditEx::GetInputData()const
{
	CString sInputData=CEditEx::GetInputData();
	if(GetGroupLength()>0)
	{
		sInputData.Remove(GetGroupSeparator());
	}
	return sInputData;
}

BOOL CNumericEditEx::IsInputData(int nPosition) const
{
	if(!CEditEx::IsInputData(nPosition))
		return FALSE;
	if(m_arrInputData[RPtoLP(nPosition)]==GetDecimalSeparator() ||
		m_arrInputData[RPtoLP(nPosition)]==GetGroupSeparator())
		return FALSE;
	return TRUE;
}

BOOL CNumericEditEx::GetLocaleSettings()
{
	int nLength;
	BOOL bSetNewValue;

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SDECIMAL,NULL,0);
	if(nLength)
	{
		CString sExisting(m_chDecimalSeparator);
		CString sDefault(m_chDecimalSeparatorDefault);
		bSetNewValue=(sExisting.CompareNoCase(sDefault)==0);
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SDECIMAL,
			&m_chDecimalSeparatorDefault,nLength);
		if(bSetNewValue)
		{
			m_chDecimalSeparator=m_chDecimalSeparatorDefault;
		}
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_STHOUSAND,NULL,0);
	if(nLength)
	{
		CString sExisting(m_chGroupSeparator);
		CString sDefault(m_chGroupSeparatorDefault);
		bSetNewValue=(sExisting.CompareNoCase(sDefault)==0);
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_STHOUSAND,
			&m_chGroupSeparatorDefault,nLength);
		if(bSetNewValue)
		{
			m_chGroupSeparator=m_chGroupSeparatorDefault;
		}
	}

	TCHAR* buffer;
	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_ILZERO,NULL,0);
	if(nLength)
	{
		bSetNewValue=(m_bShowLeadingZero==m_bShowLeadingZeroDefault);
		buffer=new TCHAR[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_ILZERO,(LPTSTR)buffer,nLength);
		m_bShowLeadingZeroDefault=(BOOL)_ttoi(buffer);
		delete[] buffer;
		if(bSetNewValue)
		{
			m_bShowLeadingZero=m_bShowLeadingZeroDefault;
		}
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SGROUPING,NULL,0);
	if(nLength)
	{
		bSetNewValue=(m_nGroupLength==m_nGroupLengthDefault);
		buffer=new TCHAR[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SGROUPING,(LPTSTR)buffer,nLength);
		CString sGrouping=buffer;
		int nPosition=sGrouping.Find(_T(";0"));
		if(nPosition!=-1)
		{
			ASSERT(nPosition>0);
			sGrouping=sGrouping.Left(nPosition);
			m_nGroupLengthDefault=_ttoi(sGrouping);
		}
		delete[] buffer;
		if(bSetNewValue)
		{
			m_nGroupLength=m_nGroupLengthDefault;
		}
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_IDIGITS,NULL,0);
	if(nLength)
	{
		bSetNewValue=(m_nFractionalDigitCount==m_nFractionalDigitCountDefault);
		buffer=new TCHAR[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_IDIGITS,(LPTSTR)buffer,nLength);
		m_nFractionalDigitCountDefault=_ttoi(buffer);
		delete[] buffer;
		if(bSetNewValue)
		{
			m_nFractionalDigitCount=m_nFractionalDigitCountDefault;
		}
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_INEGNUMBER,NULL,0);
	if(nLength)
	{
		bSetNewValue=(m_sFormatNegative.CompareNoCase(m_sFormatNegativeDefault)==0);
		buffer=new TCHAR[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_INEGNUMBER,(LPTSTR)buffer,nLength);
		int nNegativeNumberFormatType=_ttoi(buffer);
		switch(nNegativeNumberFormatType)
		{
		case 0:
			m_sFormatNegativeDefault=_T("(1.1)");
			break;
		case 1:
			m_sFormatNegativeDefault=_T("-1.1");
			break;
		case 2:
			m_sFormatNegativeDefault=_T("- 1.1");
			break;
		case 3:
			m_sFormatNegativeDefault=_T("1.1-");
			break;
		case 4:
			m_sFormatNegativeDefault=_T("1.1 -");
			break;
		}
		delete[] buffer;
		if(bSetNewValue)
		{
			m_sFormatNegative=m_sFormatNegativeDefault;
		}
	}

	return TRUE;
}

int CNumericEditEx::FindSymbol(TCHAR chSymbol) const
{
	int nPosition=-1;
	for(int nIndex=0; nIndex<m_arrInputData.GetSize(); nIndex++)
	{
		if(m_arrInputData[nIndex]==chSymbol)
		{
			nPosition=LPtoRP(nIndex);
			break;
		}
	}

	return nPosition;
}

int CNumericEditEx::ValidateInput()
{
	int nSymbolsBefore=m_arrInputData.GetSize();

	BOOL bSetToNegative=FALSE;
	BOOL bDecimalSeparatorFound=FALSE;
	for(int nIndex=m_arrInputData.GetSize()-1; nIndex>=0; nIndex--)
	{
		if(!(m_arrInputData[nIndex]>=_T('0') && m_arrInputData[nIndex]<=_T('9')))
		{
			if(nIndex==0 && m_arrInputData[nIndex]==_T('-'))
				bSetToNegative=TRUE;

			if(m_arrInputData[nIndex]==m_chDecimalSeparator)
			{
				if(!bDecimalSeparatorFound)
					bDecimalSeparatorFound=TRUE;
				else
					m_arrInputData.RemoveAt(nIndex);
			}
			else
				m_arrInputData.RemoveAt(nIndex);
		}
	}

	int nDecimalDelimiterPos=FindSymbol(GetDecimalSeparator());
	int nDecimalDigitsInserted=GetDecimalDigitsInserted();
	int nFractionalDigitsInserted=GetFractionalDigitsInserted();

	for(;m_arrInputData.GetSize()>0;)
	{
		if(m_arrInputData[0]==_T('0') && nDecimalDigitsInserted>1)
		{
			m_arrInputData.RemoveAt(0);
			nDecimalDigitsInserted--;
			if(nDecimalDelimiterPos!=-1)
				nDecimalDelimiterPos--;
		}
		else
			break;
	}

	BOOL bUpdate=FALSE;

	int nFractionalOvertypedCount=
		nFractionalDigitsInserted-GetFractionalDigitCount();
	if(nFractionalOvertypedCount>0)
	{
		for(int nIndex=0; nIndex<nFractionalOvertypedCount; nIndex++)
			m_arrInputData.RemoveAt(m_arrInputData.GetSize()-1);
		bUpdate=TRUE;
	}

	if(m_arrInputData.GetSize()>0 && GetDecimalDigitCount()!=-1)
	{
		if(m_arrInputData[0]!=_T('0'))
		{
			int nDecimalOvertypedCount=nDecimalDigitsInserted-GetDecimalDigitCount();
			if(nDecimalOvertypedCount>0)
			{
				for(int nIndex=0; nIndex<nDecimalOvertypedCount; nIndex++)
				{
					m_arrInputData.RemoveAt(0);
					nDecimalDigitsInserted--;
					if(nDecimalDelimiterPos!=-1)
						nDecimalDelimiterPos--;
				}
				bUpdate=TRUE;
			}
		}
		else
		{
			ASSERT(nDecimalDigitsInserted==1);
		}
	}

	if(GetFractionalDigitCount()!=0)
	{
		if(nDecimalDelimiterPos==-1)
			m_arrInputData.InsertAt(m_arrInputData.GetSize(),GetDecimalSeparator());

		if(GetFractionalDigitCount()>nFractionalDigitsInserted)
		{
			for(int nIndex=nFractionalDigitsInserted; 
				nIndex<GetFractionalDigitCount(); 
				nIndex++)
			{
				m_arrInputData.InsertAt(m_arrInputData.GetSize(),_T('0'));
			}
		}

		if(nDecimalDigitsInserted==0 && GetShowLeadingZero())
			m_arrInputData.InsertAt(0,_T('0'));

		if(nDecimalDigitsInserted==1 && !GetShowLeadingZero() 
			&& m_arrInputData[0]==_T('0'))
		{
			m_arrInputData.RemoveAt(0);
			nDecimalDigitsInserted--;
			if(nDecimalDelimiterPos!=-1)
				nDecimalDelimiterPos--;
		}
	}
	else
	{
		if(nDecimalDelimiterPos!=-1)
		{
			m_arrInputData.RemoveAt(RPtoLP(nDecimalDelimiterPos));
			nDecimalDelimiterPos=-1;
		}

		if(nDecimalDigitsInserted==0 && GetShowLeadingZero())
			m_arrInputData.InsertAt(0,_T('0'));
	}

	if(GetGroupLength()>0)
	{
		int nDecimalDelimiterPos=RPtoLP(FindSymbol(m_chDecimalSeparator));
		nDecimalDelimiterPos=(nDecimalDelimiterPos==-1) ?
			m_arrInputData.GetSize() : nDecimalDelimiterPos;

		int nDigitsInRow=0;
		for(int nIndex=nDecimalDelimiterPos-1; nIndex>=0; nIndex--)
		{
			if(nDigitsInRow==GetGroupLength())
			{
				if(m_arrInputData[nIndex]!=GetGroupSeparator())
				{
					m_arrInputData.InsertAt(nIndex+1,GetGroupSeparator());
					bUpdate=TRUE;
				}
				nDigitsInRow=1;
			}
			else
			{
				if(m_arrInputData[nIndex]!=GetGroupSeparator())
					nDigitsInRow++;
			}
		}
	}


	if(bSetToNegative)
		ChangeSign(bSetToNegative);
	else
		ChangeSign(IsNegativeValue());

	if(bUpdate)
		Update(-1);

	CString sInputData=GetInputData();
	if(GetGroupLength()>0)
	{
		sInputData.Remove(GetGroupSeparator());
//		sInputData.Replace(GetDecimalSeparator(),_T('.'));
	}
	TCHAR* pchStop;
	m_dValue=_tcstod(sInputData,&pchStop);
	if(IsNegativeValue())
		m_dValue*=-1;

	int nSymbolsAfter=m_arrInputData.GetSize();

	if(::IsWindow(m_pSpin->GetSafeHwnd()))
		m_pSpin->SetValue(m_dValue);

	return nSymbolsAfter-nSymbolsBefore;
}

int CNumericEditEx::ChangeSign(BOOL bNegative)
{
	m_bNegativeValue=bNegative;

	CString sMask=GetMask();
	int nPosInputData=sMask.Find(EXEDITABLETEXT_SYMBOL);
	CString sLeftLiterals=(nPosInputData>0 ? sMask.Left(nPosInputData) : _T(""));
	CString sRightLiterals=(nPosInputData==-1 ? 
		_T("") : sMask.Mid(nPosInputData+1));

	int nLeftLiteralsBefore=m_arrLeftLiterals.GetSize();

	m_arrLeftLiterals.RemoveAll();
	m_arrRightLiterals.RemoveAll();

	for(int nIndex=0; nIndex<sLeftLiterals.GetLength(); nIndex++)
	{
		m_arrLeftLiterals.InsertAt(m_arrLeftLiterals.GetSize(),
			sLeftLiterals[nIndex]);
	}

	if(m_bNegativeValue)
	{
		int nPosInNegative=m_sFormatNegative.Find(EXNUMERICVALUE_PLACEHOLDER);
		CString sLeftNegativeLiterals=(nPosInNegative>0 ? 
			m_sFormatNegative.Left(nPosInNegative) : _T(""));
		CString sRightNegativeLiterals=(nPosInNegative==-1 ? 
			_T("") : m_sFormatNegative.Mid(nPosInNegative+
			EXNUMERICVALUE_PLACEHOLDER_LENGTH));

		for(int nIndex=0; nIndex<sLeftNegativeLiterals.GetLength(); nIndex++)
		{
			m_arrLeftLiterals.InsertAt(m_arrLeftLiterals.GetSize(),
				sLeftNegativeLiterals[nIndex]);
		}
		for(int nIndex=0; nIndex<sRightNegativeLiterals.GetLength(); nIndex++)
		{
			m_arrRightLiterals.InsertAt(m_arrRightLiterals.GetSize(),
				sRightNegativeLiterals[nIndex]);
		}
	}
	else
	{
		int nPosInPositive=m_sFormatPositive.Find(EXNUMERICVALUE_PLACEHOLDER);
		CString sLeftPositiveLiterals=(nPosInPositive>0 ? 
			m_sFormatPositive.Left(nPosInPositive) : _T(""));
		CString sRightPositiveLiterals=(nPosInPositive==-1 ? 
			_T("") : m_sFormatPositive.Mid(nPosInPositive+
			EXNUMERICVALUE_PLACEHOLDER_LENGTH));

		for(int nIndex=0; nIndex<sLeftPositiveLiterals.GetLength(); nIndex++)
		{
			m_arrLeftLiterals.InsertAt(m_arrLeftLiterals.GetSize(),
				sLeftPositiveLiterals[nIndex]);
		}
		for(int nIndex=0; nIndex<sRightPositiveLiterals.GetLength(); nIndex++)
		{
			m_arrRightLiterals.InsertAt(m_arrRightLiterals.GetSize(),
				sRightPositiveLiterals[nIndex]);
		}
	}

	for(int nIndex=0; nIndex<sRightLiterals.GetLength(); nIndex++)
	{
		m_arrRightLiterals.InsertAt(m_arrRightLiterals.GetSize(),
			sRightLiterals[nIndex]);
	}

	UpdateMask();

	// SHK
	//Update(-1);

	if((m_bNegativeValue && m_dValue>=0) || (!m_bNegativeValue && m_dValue<0))
		m_dValue*=-1;

	if(::IsWindow(m_pSpin->GetSafeHwnd()))
		m_pSpin->SetValue(m_dValue);

	return m_arrLeftLiterals.GetSize()-nLeftLiteralsBefore;
}

BOOL CNumericEditEx::SetDecimalDigitCount(int nDecimalDigitCount)
{
	if(nDecimalDigitCount<-1)
	{
		TRACE(_T("CNumericEditEx::SetDecimalDigitCount: invalid decimal digit count has been specified\n"));
		return FALSE;
	}

	if(m_nDecimalDigitCount==nDecimalDigitCount)
		return TRUE;

	CString sInputData=GetInputData();
	m_nDecimalDigitCount=nDecimalDigitCount;
	EmptyData(TRUE);
	VERIFY(SetInputData(sInputData));

	return TRUE;
}

BOOL CNumericEditEx::SetDecimalSeparator(TCHAR chDecimalSeparator)
{
	if((chDecimalSeparator>=_T('0') && chDecimalSeparator<=_T('9')) ||
		chDecimalSeparator==_T('-') || chDecimalSeparator==GetGroupSeparator() ||
		chDecimalSeparator==NULL)
	{
		TRACE(_T("CNumericEditEx::SetDecimalSeparator: invalid decimal separator has been specified\n"));
		return FALSE;
	}

	int nDecimalDelimiterPos=FindSymbol(GetDecimalSeparator());
	m_chDecimalSeparator=chDecimalSeparator;
	if(nDecimalDelimiterPos!=-1)
	{
		m_arrInputData.SetAt(RPtoLP(nDecimalDelimiterPos),chDecimalSeparator);
		Update(-1);
	}

	return TRUE;
}

BOOL CNumericEditEx::SetFractionalDigitCount(int nFractionalDigitCount)
{
	if(nFractionalDigitCount<0)
	{
		TRACE(_T("CNumericEditEx::SetFractionalDigitCount: invalid fractional digit count has been specified\n"));
		return FALSE;
	}

	if(m_nFractionalDigitCount==nFractionalDigitCount)
		return TRUE;

	CString sInputData=GetInputData();
	m_nFractionalDigitCount=nFractionalDigitCount;
	EmptyData(TRUE);
	VERIFY(SetInputData(sInputData));

	if(::IsWindow(m_pSpin->GetSafeHwnd()))
		m_pSpin->SetPrecision(m_nFractionalDigitCount + 1);

	return TRUE;
}

BOOL CNumericEditEx::SetGroupLength(int nGroupLength)
{
	if(nGroupLength<0)
	{
		TRACE(_T("CNumericEditEx::SetGroupLength: invalid group length has been specified\n"));
		return FALSE;
	}

	if(m_nGroupLength==nGroupLength)
		return TRUE;

	CString sInputData=GetInputData();
	m_nGroupLength=nGroupLength;
	EmptyData(TRUE);
	VERIFY(SetInputData(sInputData));

	return TRUE;
}

BOOL CNumericEditEx::SetGroupSeparator(TCHAR chGroupSeparator)
{
	if((chGroupSeparator>=_T('0') && chGroupSeparator<=_T('9')) ||
		chGroupSeparator==_T('-') || chGroupSeparator==GetDecimalSeparator() ||
		chGroupSeparator==NULL)
	{
		TRACE(_T("CNumericEditEx::SetGroupSeparator: invalid group separator has been specified\n"));
		return FALSE;
	}

	if(m_chGroupSeparator==chGroupSeparator)
		return TRUE;

	CString sInputData=GetInputData();
	m_chGroupSeparator=chGroupSeparator;
	EmptyData(TRUE);
	VERIFY(SetInputData(sInputData));

	return TRUE;
}

BOOL CNumericEditEx::SetNegativeFormat(LPCTSTR lpszFormatNegative)
{
	ASSERT(lpszFormatNegative!=NULL);

	CString sNegativeFormat=lpszFormatNegative;
	if(sNegativeFormat.Find(EXNUMERICVALUE_PLACEHOLDER)==-1)
	{
		TRACE(_T("CNumericEditEx::SetNegativeFormat: invalid format for displaying negative value has been specified\n"));
		return FALSE;
	}

	m_sFormatNegative=sNegativeFormat;
	ChangeSign(IsNegativeValue());

	return TRUE;
}

BOOL CNumericEditEx::SetPositiveFormat(LPCTSTR lpszFormatPositive)
{
	ASSERT(lpszFormatPositive!=NULL);

	CString sPositiveFormat=lpszFormatPositive;
	if(sPositiveFormat.Find(EXNUMERICVALUE_PLACEHOLDER)==-1)
	{
		TRACE(_T("CNumericEditEx::SetPositiveFormat: invalid format for displaying positive value has been specified\n"));
		return FALSE;
	}

	m_sFormatPositive=sPositiveFormat;
	ChangeSign(IsNegativeValue());

	return TRUE;
}

BOOL CNumericEditEx::SetValue(double dValue)
{
	m_dValue=dValue;
	m_bNegativeValue=(m_dValue<0 ? TRUE : FALSE);
	BOOL bSetNegative=m_bNegativeValue;


	int decimal, sign;
	char buffer[128];
	_fcvt_s(buffer, sizeof(buffer), m_dValue,GetFractionalDigitCount(),&decimal,&sign);
	int nSymbolCount=strlen(buffer);
	TCHAR* chInputData=new TCHAR[nSymbolCount+1];
	_tcsncpy_s(chInputData, nSymbolCount+1, CA2T(buffer),nSymbolCount);
	chInputData[nSymbolCount]=_T('\0');

	CString sInputData=chInputData;
	CString sDecimalSeparator(GetDecimalSeparator());
	if(!sInputData.IsEmpty())
	{
		for(int nIndex=decimal; nIndex<1; nIndex++)
		{
			sInputData=_T("0")+sInputData;
			decimal++;
			nSymbolCount++;
		}
		sInputData=sInputData.Left(decimal)+
			sDecimalSeparator+sInputData.Mid(decimal);
		if(sign!=0)
			sInputData=_T("-")+sInputData;
	}

	m_arrInputData.RemoveAll();
	//EmptyData(TRUE);
	VERIFY(SetInputData(sInputData));

	delete[] chInputData;

	ChangeSign(bSetNegative);

	if(::IsWindow(m_pSpin->GetSafeHwnd()))
		m_pSpin->SetValue(m_dValue);

   return TRUE;
}

void CNumericEditEx::SetShowLeadingZero(BOOL bShowLeadingZero)
{
	if(m_bShowLeadingZero==bShowLeadingZero)
		return;

	CString sInputData=GetInputData();
	m_bShowLeadingZero=bShowLeadingZero;
	EmptyData(TRUE);
	VERIFY(SetInputData(sInputData));

	return;
}

void CNumericEditEx::SetNegativeTextColor(COLORREF clrTextNegative, 
										  BOOL bRedraw/*=TRUE*/)
{
	if(m_clrTextNegative!=clrTextNegative)
	{
		m_clrTextNegative=clrTextNegative;
		if(bRedraw && IsNegativeValue())
			_RedrawWindow();
	}
}

int CNumericEditEx::TranslateSymbol(int nSelectionStart, 
									TCHAR chNewChar, BOOL bReplaceExisting)
{
	if((chNewChar>=_T('0') && chNewChar<=_T('9')) || 
		chNewChar==m_chDecimalSeparator || chNewChar==_T('-') ||
		(m_bForceToUsePeriod && chNewChar==_T('.')))
	{
		if(chNewChar==_T('-'))
		{
			nSelectionStart+=ChangeSign(!IsNegativeValue())-1;
			return nSelectionStart;
		}

		if(chNewChar==m_chDecimalSeparator || 
			(m_bForceToUsePeriod && chNewChar==_T('.')))
		{
			if(GetFractionalDigitCount()==0)
				return -1;
			int nPos=FindSymbol(m_chDecimalSeparator);
			if(nPos==-1)
			{
				nSelectionStart=m_arrLeftLiterals.GetSize()+m_arrInputData.GetSize();
				return CEditEx::InsertAt(nSelectionStart,chNewChar);
			}
			else
				return GetNextInputLocation(nPos);
		}

		if(m_arrInputData.GetSize()>0 && chNewChar==_T('0') && 
			RPtoLP(nSelectionStart)==0 && IsInputData(nSelectionStart))
		{
			if(bReplaceExisting)
			{
				m_arrInputData.RemoveAt(0);
				ValidateInput();
				if(m_arrInputData[0]==_T('0') || 
					m_arrInputData[0]==GetDecimalSeparator())
					return nSelectionStart;
				else
					return -1;
			}
			else
				return -1;
		}

		if(m_arrInputData.GetSize()>0 && RPtoLP(nSelectionStart)==0)
		{
			if(bReplaceExisting && m_arrInputData[0]==GetDecimalSeparator())
				return CEditEx::InsertAt(nSelectionStart,chNewChar);
			else if(!bReplaceExisting && m_arrInputData[0]==_T('0'))
				return CEditEx::SetAt(nSelectionStart,chNewChar);
		}

		int nInsertedAtPos=-1;
		if(bReplaceExisting)
		{
			nInsertedAtPos=CEditEx::SetAt(nSelectionStart,chNewChar);
		}
		else
		{
			int nDecimalDelimiterPos=FindSymbol(GetDecimalSeparator());
			if(nDecimalDelimiterPos==-1 || nDecimalDelimiterPos>=nSelectionStart)
			{
				if(nDecimalDelimiterPos==-1 && GetDecimalDigitsInserted()==1 &&
					m_arrInputData[0]==_T('0'))
					nInsertedAtPos=CEditEx::SetAt(nSelectionStart-1,chNewChar);
				else if(GetDecimalDigitsInserted()<GetDecimalDigitCount() ||
					GetDecimalDigitCount()==-1)
					nInsertedAtPos=CEditEx::InsertAt(nSelectionStart,chNewChar);
				else
					nInsertedAtPos=CEditEx::SetAt(nSelectionStart,chNewChar);
			}
			else
			{
				if(GetFractionalDigitsInserted()<GetFractionalDigitCount())
					nInsertedAtPos=CEditEx::InsertAt(nSelectionStart,chNewChar);
				else
					nInsertedAtPos=CEditEx::SetAt(nSelectionStart,chNewChar);
			}
		}

		return nInsertedAtPos;
	}

	return -1;
}

int CNumericEditEx::GetDecimalDigitsInserted() const
{
	int nCount=0;
	int nDecimalDelimiterPos=FindSymbol(GetDecimalSeparator());
	nDecimalDelimiterPos=(nDecimalDelimiterPos==-1 ? 
		m_arrInputData.GetSize() : RPtoLP(nDecimalDelimiterPos));
	for(int nIndex=0; nIndex<nDecimalDelimiterPos; nIndex++)
	{
		if(m_arrInputData[nIndex]>=_T('0') && m_arrInputData[nIndex]<=_T('9'))
			nCount++;
	}

	return nCount;
}

int CNumericEditEx::GetFractionalDigitsInserted() const
{
	int nCount=0;
	int nDecimalDelimiterPos=FindSymbol(GetDecimalSeparator());
	nDecimalDelimiterPos=(nDecimalDelimiterPos==-1 ? 
		m_arrInputData.GetSize() : RPtoLP(nDecimalDelimiterPos));
	for(int nIndex=nDecimalDelimiterPos+1; nIndex<m_arrInputData.GetSize(); nIndex++)
	{
		if(m_arrInputData[nIndex]>=_T('0') && m_arrInputData[nIndex]<=_T('9'))
			nCount++;
	}

	return nCount;
}

HBRUSH CNumericEditEx::CtlColor(CDC* pDC, UINT nCtlColor)
{
	HBRUSH hBrush=CEditEx::CtlColor(pDC,nCtlColor);
	if(IsNegativeValue())
		pDC->SetTextColor(GetNegativeTextColor());
	return hBrush;
}

BOOL CNumericEditEx::_RedrawWindow()
{
	if(CEditEx::_RedrawWindow())
	{
		if(::IsWindow(m_pSpin->GetSafeHwnd()))
			m_pSpin->RedrawWindow();
		return TRUE;
	}
	return FALSE;
}

void CNumericEditEx::SetSpinned(BOOL bSpinned /*= TRUE*/,
								double dMin /*= -100.*/,
								double dMax /*= 100.*/,
								double dStep /*= 1.*/)
{
	if(bSpinned)
	{
		if(!::IsWindow(m_pSpin->GetSafeHwnd()))
		{
			if(m_pSpin)
				delete m_pSpin;

			m_pSpin = new CEGSpin();
			m_pSpin->Create(GetParent(), this, 11);
			m_pSpin->SetFlat(m_bIsFlat);
		}

		m_pSpin->SetValue(m_dValue);
		m_pSpin->SetPrecision(m_nFractionalDigitCount + 1);
		m_pSpin->SetRange(dMin, dMax, FALSE);
		m_pSpin->SetStep(dStep);

		//CEGSpin::EGSpinAccel esAccel[2] = { { 2, 0.25 }, { 5, 0.5} };
		//m_pSpin->SetAccel(2, esAccel);
	}
	else if(!bSpinned && ::IsWindow(m_pSpin->GetSafeHwnd()))
	{
		m_pSpin->DestroyWindow();
		delete m_pSpin;
		m_pSpin = NULL;
	}
}

void CNumericEditEx::OnDestroy() 
{
	if(::IsWindow(m_pSpin->GetSafeHwnd()))
	{
		m_pSpin->DestroyWindow();
		delete m_pSpin;
		m_pSpin = NULL;
	}

	CEditEx::OnDestroy();
}

LRESULT CNumericEditEx::OnEgSmValueChanged(WPARAM wParam, LPARAM lParam)
{
	if(::IsWindow(m_pSpin->GetSafeHwnd()))
	{
		EG_SM_VALUECHANGED_DATA* smData = (EG_SM_VALUECHANGED_DATA*)lParam;
		if(smData->hwndFrom == m_pSpin->GetSafeHwnd())
		{
			SetValue(smData->dValue);
			return 1L;
		}
	}
	return 0L;
}

LRESULT CNumericEditEx::OnEgSmMouseEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_bIsFlat && ::GetFocus() != m_hWnd)
	{
		m_bMouseOverSpin = TRUE;
		OnTrackStart();
		_RedrawWindow();
	}
	return (LRESULT)0L;
}

LRESULT CNumericEditEx::OnEgSmMouseExit(WPARAM wParam, LPARAM lParam)
{
	if(m_bIsFlat && m_bMouseOverSpin && ::GetFocus() != m_hWnd)
	{
		m_bMouseOverSpin = FALSE;
		_RedrawWindow();
		if(!PointInRect())
		{
			OnTrackEnd();
		}
		else
		{
			SetTimer(1,10,NULL);
			OnTimer(1);
			OnTrackStart();
		}
	}
	return (LRESULT)0L;
}

void CNumericEditEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(::IsWindow(m_pSpin->GetSafeHwnd()) && (GetStyle() & ES_READONLY) != ES_READONLY)
	{
		if(nChar == VK_UP || nChar == VK_DOWN)
		{
			SHORT nKeyState = 0;
			if(nChar == VK_UP)
				nKeyState = ::GetKeyState(VK_DOWN);
			else
				nKeyState = ::GetKeyState(VK_UP);

			if(nKeyState >= 0)
				::SendMessage(m_pSpin->GetSafeHwnd(), WM_KEYDOWN, (WPARAM)nChar, MAKELONG(nRepCnt, nFlags)); 
			return;
		}
	}

	CEditEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CNumericEditEx::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(::IsWindow(m_pSpin->GetSafeHwnd()) && (GetStyle() & ES_READONLY) != ES_READONLY)
	{
		if(nChar == VK_UP || nChar == VK_DOWN)
		{
			::SendMessage(m_pSpin->GetSafeHwnd(), WM_KEYUP, (WPARAM)nChar, MAKELONG(nRepCnt, nFlags)); 
			return;
		}
	}

	CEditEx::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CNumericEditEx::OnKillFocus(CWnd* pNewWnd) 
{
	CEditEx::OnKillFocus(pNewWnd);
	if(::IsWindow(m_pSpin->GetSafeHwnd()))
	{
		if(!PointInRect())
		{
			m_pSpin->OnBuddyStateChanged(CEGSpin::enBuddyStateNormal, TRUE);
			::SendMessage(m_pSpin->GetSafeHwnd(), WM_KEYUP, (WPARAM)VK_DOWN, MAKELONG(1, 0));
		}
		else
		{
			m_pSpin->OnBuddyStateChanged(CEGSpin::enBuddyStateTracked, TRUE);
		}
	}
}

void CNumericEditEx::OnSetFocus(CWnd* pOldWnd) 
{
	CEditEx::OnSetFocus(pOldWnd);
	
	if(::IsWindow(m_pSpin->GetSafeHwnd()))
	{
		m_pSpin->OnBuddyStateChanged(CEGSpin::enBuddyStateFocused, TRUE);
	}
}

void CNumericEditEx::OnTrackStart()
{
	if(::IsWindow(m_pSpin->GetSafeHwnd()))
	{
		m_pSpin->OnBuddyStateChanged(CEGSpin::enBuddyStateTracked, TRUE);
	}
}

void CNumericEditEx::OnTrackEnd()
{
	if(::IsWindow(m_pSpin->GetSafeHwnd()))
	{
		m_pSpin->OnBuddyStateChanged(CEGSpin::enBuddyStateNormal, TRUE);
	}
}
