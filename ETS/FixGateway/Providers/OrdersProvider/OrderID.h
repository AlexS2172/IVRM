#pragma once
#include <ATLComTime.h>

class COrderID
{
	CString	m_sOrderID;

public:

	COrderID()
	{
		m_sOrderID = _T("");
	}

	COrderID(LPCTSTR sOrderID)
	{
		m_sOrderID = sOrderID ? sOrderID : _T("");
		m_sOrderID.MakeUpper();
	}

	COrderID& operator = (LPCTSTR sOrderID)
	{
		m_sOrderID = sOrderID ? sOrderID : _T("");
		m_sOrderID.MakeUpper();
		return *this;
	}

	operator LPCTSTR () const
	{
		return m_sOrderID.GetString();
	}

	COrderID& operator ++()
	{
		CString sBranchCode;
		long	nSeqNum;
		CString sDateTime;
		CString	sDateTimeNow;
		CString sNewOrderID;

		try
		{
			time_t tNow;
			time(&tNow);
			struct tm* tmNow = gmtime(&tNow);

			COleDateTime dt;
			dt.SetDate(tmNow->tm_year + 1900, tmNow->tm_mon + 1, tmNow->tm_mday);
			sDateTimeNow = dt.Format(_T("%y%m%d"));

			if (m_sOrderID.GetLength() != 16 || m_sOrderID.GetLength() == 0)
			{
				sNewOrderID = L"AAAAA00001";
				sNewOrderID += sDateTimeNow;
			}
			else
			{
				sBranchCode = m_sOrderID.Left(5);
				nSeqNum = _tcstol(m_sOrderID.Mid(5,5), NULL, 10);
				sDateTime = m_sOrderID.Mid(10, 6);

				if(!sDateTime.CompareNoCase(sDateTimeNow))
				{
					if(++nSeqNum == 100000)
					{
						// if seqnumb is too much then change BranchCode
						nSeqNum = 1;

						LPTSTR pszBuf = sBranchCode.GetBuffer(5);

						for(int nChar = 0; nChar < sBranchCode.GetLength(); nChar++)
						{
							if(_istdigit(pszBuf[nChar]))
							{
								if(pszBuf[nChar] != _T('9'))
								{
									pszBuf[nChar]++;
									break;
								}
							}
							else if(_istalpha(pszBuf[nChar]))
							{
								if(pszBuf[nChar] != _T('Z'))
								{
									pszBuf[nChar]++;
									break;
								}
							}
						}
						sBranchCode.ReleaseBuffer();
					}

					sNewOrderID.Format(_T("%05s%05d%06s"), (LPCTSTR)sBranchCode, nSeqNum, (LPCTSTR)sDateTime);
				}
				else
				{
					sNewOrderID.Format(_T("%05s00001%06s"), (LPCTSTR)sBranchCode, (LPCTSTR)sDateTimeNow);
				}
			}

			m_sOrderID = sNewOrderID;
		}
		catch(...) {}
		return *this;
	}

	COrderID operator ++(int)
	{
		COrderID Temp = *this;
		++(*this);
		return Temp;
	}
};
