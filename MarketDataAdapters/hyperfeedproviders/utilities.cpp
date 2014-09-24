// Utilities.cpp : Implementation of Common Utilities
#include "stdafx.h"
#include "Utilities.h"
#include <edbx.h>

/////////////////////////////////////////////////////////////////////////////
// Common Utilities
long CUtilities::Dbl2Price(short nPriceType, double dblPrice)
{
	switch(nPriceType)
	{
	case PT_LCD_256:
	case PT_LCD_64:
	case PT_FUTURE_256:
	case PT_FUTURE_64:
	case PT_FUTURE_32:
	case PT_FUTURE_8:
		dblPrice *= 256.0;
		break;
	case PT_DECIMAL_0:
	case PT_LEADING:
		break;
	case PT_DECIMAL_1:
		dblPrice *= 10.0;
		break;
	case PT_DECIMAL_2:
		dblPrice *= 100.0;
		break;
	case PT_DECIMAL_3:
		dblPrice *= 1000.0;
		break;
	case PT_DECIMAL_4:
		dblPrice *= 10000.0;
		break;
	case PT_DECIMAL_5:
		dblPrice *= 100000.0;
		break;
	case PT_DECIMAL_6:
		dblPrice *= 1000000.0;
		break;
	case PT_DECIMAL_7:
		dblPrice *= 10000000.0;
		break;
	case PT_DECIMAL_8:
		dblPrice *= 100000000.0;
		break;
	case PT_DECIMAL_9:
		dblPrice *= 1000000000.0;
		break;
	case PT_UNDEFINED:
	default:
		break;
	}
	return (long)dblPrice;

}

double CUtilities::Price2Dbl(short nPriceType, double dblPrice)
{
	switch(nPriceType)
	{
	case PT_LCD_256:
	case PT_LCD_64:
	case PT_FUTURE_256:
	case PT_FUTURE_64:
	case PT_FUTURE_32:
	case PT_FUTURE_8:
		dblPrice /= 256.0;
		break;
	case PT_DECIMAL_0:
	case PT_LEADING:
		break;
	case PT_DECIMAL_1:
		dblPrice /= 10.0;
		break;
	case PT_DECIMAL_2:
		dblPrice /= 100.0;
		break;
	case PT_DECIMAL_3:
		dblPrice /= 1000.0;
		break;
	case PT_DECIMAL_4:
		dblPrice /= 10000.0;
		break;
	case PT_DECIMAL_5:
		dblPrice /= 100000.0;
		break;
	case PT_DECIMAL_6:
		dblPrice /= 1000000.0;
		break;
	case PT_DECIMAL_7:
		dblPrice /= 10000000.0;
		break;
	case PT_DECIMAL_8:
		dblPrice /= 100000000.0;
		break;
	case PT_DECIMAL_9:
		dblPrice /= 1000000000.0;
		break;
	case PT_UNDEFINED:
	default:
		break;
	}
	return dblPrice;
}

void CUtilities::CreateSz(char* sz, const char* pch, int count, bool bRightJustified /*= false*/)
{
	if (bRightJustified)
	{
		int i = 0;
		if (count > 0)
		{
			while (pch[i] == ' ' || pch[i] == '\0')
			{
				if (++i == count)
				{
					*sz = '\0';
					return;
				}
			}
		}
		else
		{
			*sz = '\0';
			return;
		}
		strncpy(sz, pch + i, count - i);
		sz[count - i] = '\0';
	}
	else
	{
		TrimRight(sz, pch, count);
		/*
		int i = 0, n = 0;
		if(count > 0)
		{
			while(i < count)
			{
				if(pch[i] != ' ' && pch[i] != '\0')
					sz[n++] = pch[i];

				i++;
			}
		}
		sz[n] = '\0';*/
	}
}

void CUtilities::TrimRight(char* sz, const char* pch, int count)
{
	if(count > 0)
	{
		while(pch[count - 1] == ' ' || pch[count - 1] == '\0')
		{
			if(--count == 0)
				break;
		}
	}
	
	sz[count] = '\0';
	
	while(--count >= 0)
	{
		sz[count] = pch[count];
	}
}

void CUtilities::CreateSpaced(char* sspaced, const char* sz, int count, bool bRightJustified /*= false*/)
{
	if (bRightJustified)
	{
		int nSpaces = count;
		if (sz)
		{
			int len = strlen(sz);
			nSpaces = (count - len > 0 ? count - len : 0);
			memcpy(sspaced + nSpaces, sz, count - nSpaces);
		}
		memset(sspaced, ' ', nSpaces);
	}
	else
	{
		if (sz)
		{
			while (*sz != '\0' && count > 0)
			{
				*sspaced++ = *sz++;
				--count;
			}
		}
		
		while (count > 0)
		{
			*sspaced++ = ' ';
			--count;
		}
	}
}

void CUtilities::CreateSpaced(char* sspaced, const OLECHAR* wsz, int count, bool bRightJustified /*= false*/)
{
	//char* sz = 0;
	if (wsz)
	{
		//int len = wcslen(wsz) + 1;
		//char* sz = new char[len];
		//wcstombs(sz, wsz, len);
		char sz[MAX_LENGTH];
		wcstombs(sz, wsz, MAX_LENGTH);
		CreateSpaced(sspaced, sz, count, bRightJustified);
	}
	else
	{
		CreateSpaced(sspaced, (char*)0, count, bRightJustified);
	}
}

void CUtilities::CreateSpacedUpper(char* sspaced, const char* sz, int count)
{
	if (sz)
	{
		while (*sz != '\0' && count > 0)
		{
			*sspaced++ = toupper(*sz++);
			--count;
		}
	}
	
	while (count > 0)
	{
		*sspaced++ = ' ';
		--count;
	}
}

void CUtilities::CreateSpacedUpper(char* sspaced, const OLECHAR* wsz, int count)
{
	if (wsz)
	{
		char sz[MAX_LENGTH];
		wcstombs(sz, wsz, MAX_LENGTH);
		CreateSpacedUpper(sspaced, sz, count);
	}
	else
	{
		CreateSpacedUpper(sspaced, (char*)0, count);
	}
}

bool CUtilities::StringToKey(const OLECHAR* wsz, DBA_KEY *dbaKey)
{
	BOOL bres;
	if (wsz)
	{
		char sz[MAX_LENGTH];
		wcstombs(sz, wsz, MAX_LENGTH);
		bres = ::StringToKey(sz, dbaKey);
	}
	else
	{
		bres = ::StringToKey(0, dbaKey);
	}
	return (bres == TRUE);
}

bool CUtilities::KeyToString(DBA_KEY* dbaKey, char* string, short stringLen)
{
	short string_idx = 0;

	// Item type with a prefix are identified here
	switch(dbaKey->type)
	{
	case IT_EQUITY:
	case IT_BOND:
		break;

	case IT_FOREIGN_EXCHG:
		*string = '~';
		++string_idx;
		break;



	case IT_FUTURE:
	case IT_FUTURE_OPTION:
		*string = '/';
		++string_idx;
	break;

	case IT_EQUITY_OPTION:
		*string = '.';
		++string_idx;
		break;

	case IT_INDEX:
		*string = '$';
		++string_idx;
		break;

	default:	// Invalid type
		*string = 0;
		return false;
	}
	
	// Fill in the symbol	
	short len = MAX_TICKER - 1;
	while (len >= 0)
	{
		if(dbaKey->symbol[len] != ' ')
			break;
		--len;
	}

	++len;
	memcpy(string + string_idx, dbaKey->symbol, len);
	string_idx += len;

	// Check for currency or country other than US
	if(dbaKey->currencyCode[0] != dbaKey->countryCode[0] &&
		dbaKey->currencyCode[1] != dbaKey->countryCode[1])
	{
		if (dbaKey->currencyCode[0] != ' ' &&
			dbaKey->currencyCode[1] != ' ' &&
			dbaKey->currencyCode[2] != ' ' &&
			dbaKey->currencyCode[0] != '\0' &&
			dbaKey->currencyCode[1] != '\0' &&
			dbaKey->currencyCode[2] != '\0')
		{
			*(string + string_idx++) = '/';
			*(string + string_idx++) = dbaKey->currencyCode[0];
			*(string + string_idx++) = dbaKey->currencyCode[1];
			*(string + string_idx++) = dbaKey->currencyCode[2];
			if (dbaKey->countryCode[0] != ' ' &&
				dbaKey->countryCode[1] != ' ' &&
				dbaKey->countryCode[0] != '\0' &&
				dbaKey->countryCode[1] != '\0')
			{
				*(string + string_idx++) = '.';
				*(string + string_idx++) = dbaKey->countryCode[0];
				*(string + string_idx++) = dbaKey->countryCode[1];
			}
		}
	}
	else
	{
		if (dbaKey->countryCode[0] != 'U' &&
			dbaKey->countryCode[1] != 'S' &&
			dbaKey->countryCode[0] != ' ' &&
			dbaKey->countryCode[1] != ' ' &&
			dbaKey->countryCode[0] != '\0' &&
			dbaKey->countryCode[1] != '\0')
		{
			*(string + string_idx++) = '.';
			*(string + string_idx++) = dbaKey->countryCode[0];
			*(string + string_idx++) = dbaKey->countryCode[1];
		}
	}

	// Check for exchange code
	if(dbaKey->exchangeCode[0] != ' ' &&

		dbaKey->exchangeCode[0] != '\0')
	{
		if ((dbaKey->type == IT_FUTURE )&&(dbaKey->exchangeCode[0] == 'G'))
		{
			*(string + string_idx++) = 'G';
			if (dbaKey->exchangeCode[1] != ' ')
				*(string + string_idx++) = dbaKey->exchangeCode[1];
		
		}
		else
		{
			*(string + string_idx++) = '&';
			*(string + string_idx++) = dbaKey->exchangeCode[0];
			if (dbaKey->exchangeCode[1] != ' ')
				*(string + string_idx++) = dbaKey->exchangeCode[1];
		}
	}
	*(string + string_idx++) = '\0';



	return true;
}

/******************************************************************************
/		Convertion:
/	'A  BC'	->	'ABC'
/	'AB CD'	->	'ABCD'
******************************************************************************/
void CUtilities::RemoveSpaceFromName(char* szName)
{
	int len = strlen(szName);
	
	for(int i = len - 1; i >=0; --i)
	{
		if(*(szName + i) == ' ')
		{
			for(int j = i; j < len; ++j)
			{
				*(szName + j) =  *(szName + j + 1);
			}
			len--;
		}
	}
}

/******************************************************************************
/		Convertion:
/	'$DJX'	->	'DJX'
******************************************************************************/
void CUtilities::RemoveBucksFromName(char* szName)
{
	if(*szName == '$')
		for(int i = 0; i < MAX_TICKER - 1; i++)
		{
			*(szName + i) = *(szName + i + 1);
		}
}

/******************************************************************************
/		Convertion:
/	'DJX'	->	'$DJX'
******************************************************************************/
void CUtilities::AddBucksInName(char* szName)
{
	int len = strlen(szName);
	for(int i = len; i > 0; i--)
	{
		*(szName + i) = *(szName + i - 1);
	}
	*szName = '$';
	*(szName + len + 1) = 0;
}
