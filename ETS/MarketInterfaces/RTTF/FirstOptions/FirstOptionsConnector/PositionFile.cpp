//**************************************************************************************************************//
// includes
//**************************************************************************************************************//
#include "stdafx.h"
#include "PositionFile.h"
#include "Publisher.h"
#include "trace.h"

//--------------------------------------------------------------------------------------------------------------//
DWORD CPositionFile::GetFileName(string& sFileName)
{
	char szPath[MAX_PATH] = {0};
	if (0 == ::GetModuleFileName(NULL, szPath, MAX_PATH))
	{
		return ::GetLastError();
	}
	
	for (int i = strlen(szPath); i != 0 && szPath[i] != '\\'; i--);
	szPath[i] = 0;

	SYSTEMTIME st = {0};
	::GetLocalTime(&st);

	char szFileName[MAX_PATH] = {0};
	sprintf(szFileName, "%s\\logs\\foc%4d%.2d%.2d.dat", szPath, st.wYear, st.wMonth, st.wDay);

	sFileName = szFileName;
	return ERROR_SUCCESS;
}

//--------------------------------------------------------------------------------------------------------------//
DWORD CPositionFile::WriteFile(FocRttfPositionMsgEx* pPos)
{
	///!!!
	///:TODO: 
	/// * Numeric fields values validation
	/// * File header
	/// * Date format (days/mounth begins form zero or 1?
	
	string sFileName;
	DWORD dwError = ERROR_SUCCESS; 

	dwError = GetFileName(sFileName);
	if (ERROR_SUCCESS != dwError) return dwError;
	
	// Real length of szPositionLine must be 143
	// 140 - maximum length of all fields
	// 2 - new line (0x0D 0x0A)
	// 1 - zero
	char szPositionLine[200];	
																	  
	char cPutCall = ' ';
	bool bIsOption = false;
	unsigned long uStrike = 0;
	unsigned long uStrikeFrac = 0;
	float fStrikeFrac = 0.0;
	
	bIsOption = strlen(pPos->security_type) >= 2 && pPos->security_type[0] == '0';
		
	if (bIsOption)
	{
		// call or put
		if (CALL_FLAG == pPos->put_call)
		{
			cPutCall = 'C';
		}
		else
		{
			cPutCall = 'P';
		}
	
		// strike integer value
		uStrike = static_cast<unsigned long>(pPos->strike);
		
		// strike frac value
		fStrikeFrac = fStrikeFrac = pPos->strike - uStrike;
		uStrikeFrac = static_cast<unsigned long>(fStrikeFrac * 8);
	}

	// long/short
	char cLongShort = ' ';
	if (BUY_FLAG == pPos->buy_sell)
	{
		cLongShort = 'L';
	}
	else
	{
		cLongShort = 'S';
	}
	
	// security type
	char cSecurityType = ' ';
	switch (pPos->security_type[0])
	{
		case '0': // option
			cSecurityType = 'O';
			break;

		case '1': // stock
			cSecurityType = 'S';
			break;

		case '5': // future
			cSecurityType = 'F';
			break;

		default:
			cSecurityType = 'X';
	}

	// Numeric values range validation
	{
		string sErrorMsg("");
		long lFieldValue = 0;

		if (uStrike > 9999)
		{
			sErrorMsg = "strike value (size > 4)";
			lFieldValue = uStrike;
		}

		if (8 <= uStrikeFrac || (float)uStrikeFrac / 8 != fStrikeFrac) 
		{
			sErrorMsg = "strike frac value";
			lFieldValue = uStrikeFrac;
		}


		if (sErrorMsg.length() > 0)
		{
			Trace("Failed to save position to file. Symbol %s. Invalid %s", pPos->trading_sym, sErrorMsg.c_str());
			return ERROR_INVALID_DATA;
		}
	}
	
	char szFormat[] = 
		// General fields
		"346"		// 3 transaction ID Literal '346'
		" "			// 1 record ID '  ' (blank) indicates open position, 'E' electronic 
		"% 4.4s"	// 4 clearing firm Right aligned, padded with leading
		"%- 10.10s"	// 10 account Left aligned, padded with blanks
		"%c"		// 1 put/call 'P'=put, 'C'=call, ' '=non-option
		"%- 6.6s"	// 6 symbol Base symbol, NOT option symbol
		"%.2d"		// 2 expire year Year in which option of future expires. Zero fill if not option or future.
		"%.2d"		// 2 expire month Month in which option or future expires. Zero fill if not option or future.
		"%.5d"		// 5 strike dollar Equity options use columns 32-34; other options use columns 31-34. ... 
		"%.2d"		// 2 strike frac Only equity options use this field; others zero fill in. ... i.e. 5/8=05 ...
		"%c"		// 1 long/short 'L'=long/buy, 'S'=short/sell 
		"%c"		// 1 security type 'O'=option, 'S'=stock, 'F'=future, 'X'= on-equity underlying
		"          "// 10 trade price If averaged or open trade, closing price or mark. 6 implied decimal points.
		"          "// 10 filler (unused)
		"%7d"		// 7 net position Quantity, absolute value
		"         " // 9 CUSIP (unused) 
		"     "		// 5 filler (unused)
		"0"			// 1 record type ' ' or '0'=opening position, '1'=trade
		// Optional fields
		"%- 6.6s"	// 6 trade symbol ALTSYM; the actual exchange symbol, differs from base symbol for some options. 
		"%2d"		// 2 expiration day Day of the month in which the option or future expires. Zero fill if not an option or future. 
		"%9d"		// 9 decimal strike 4 assumed decimal places. PIC 9(5)V9(4) 
		"%5d"		// 5 SPC Shares per contract, right aligned. 
		"%2d%2d"	// 4 Execution time HHMM 
		"%2d%2d%2d"	// 6 Execution date YYMMDD 
		"     0"	// 6 tag number Unique ID or zero. 
		"    "		// 4 filler (unused) 
		"x"			// 1 option type '1'=future option, ' '=equity or index option 
		"          "// 10 filler (unused) 
		"   0"		// 4 contract year Contract year 
		" 0"		// 2 contract month Numeric
		"\n"; // New line
	
	sprintf(szPositionLine, szFormat,
		// General fields
		pPos->clearing_firm,
		pPos->account,
		cPutCall,
		pPos->base_sym,
		pPos->expiration_date.year % 100,
		pPos->expiration_date.month,
		uStrike,
		uStrikeFrac,
		cLongShort,
		pPos->security_type[0],
		pPos->quantity,
		// Optional fields
		pPos->trading_sym,
		pPos->expiration_date.day,
		static_cast<unsigned long>(pPos->strike * 10000),
		100, ///!!!
		pPos->execution_date.hour, pPos->execution_date.min,
		pPos->execution_date.year % 100, pPos->execution_date.month, pPos->execution_date.day);

	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = ::CreateFile(
		sFileName.c_str(), 
		GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return ::GetLastError();
	}

	
	if (INVALID_SET_FILE_POINTER == ::SetFilePointer(hFile, 0, NULL, FILE_END))
	{
		dwError = ::GetLastError();
		::CloseHandle(hFile);
		return dwError;
	}
	
	DWORD dwLength = 0;
	dwLength = lstrlen(szPositionLine);

	DWORD dwWriten = 0;
	if (FALSE == ::WriteFile(hFile, szPositionLine, dwLength, &dwWriten, NULL) || dwWriten != dwLength)
	{
		dwError = ::GetLastError();
		::CloseHandle(hFile);
		return dwError;
	}

	::CloseHandle(hFile);

	return dwError;
}

//--------------------------------------------------------------------------------------------------------------//
DWORD CPositionFile::DeleteFile()
{
	string sFileName;
	DWORD dwError = ERROR_SUCCESS;

	dwError = GetFileName(sFileName);
	if (ERROR_SUCCESS == dwError)
	{
		if (FALSE == ::DeleteFile(sFileName.c_str()))
		{
			dwError = ::GetLastError();
		}
	}

	return dwError;
}