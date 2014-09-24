// Utilities.h : Declaration of Common Utilities

#ifndef __UTILITIES_H__
#define __UTILITIES_H__


/////////////////////////////////////////////////////////////////////////////
// Common Utilities

class CUtilities
{
public:
	static void CreateSz(char* sz, const char* pch, int count, bool bRightJustified = false);
	static bool StringToKey(const OLECHAR* wsz, DBA_KEY *dbaKey);
	static bool KeyToString(DBA_KEY* dbaKey, char* string, short stringLen);
	static void TrimRight(char* sz, const char* pch, int count);
	
protected:
	static double Price2Dbl(short nPriceType, double dblPrice);
	static long Dbl2Price(short nPriceType, double dblPrice);

	static void CreateSpaced(char* sspaced, const char* sz, int count, bool bRightJustified = false);
	static void CreateSpaced(char* sspaced, const OLECHAR* wsz, int count, bool bRightJustified = false);
	static void CreateSpacedUpper(char* sspaced, const char* sz, int count);
	static void CreateSpacedUpper(char* sspaced, const OLECHAR* wsz, int count);
	static void RemoveSpaceFromName(char* szName);
	static void RemoveBucksFromName(char* szName);
	static void AddBucksInName(char* szName);

};

#endif // __UTILITIES_H__
