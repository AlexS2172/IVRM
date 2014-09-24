#ifndef __UTILS_H__
#define __UTILS_H__

// Returns true if st1 less than st2
inline bool operator<( const SYSTEMTIME& st1, const SYSTEMTIME& st2 )
{
	// Year
	if( st1.wYear < st2.wYear )
		return true;
	else if( st1.wYear > st2.wYear )
		return false;

	// Month
	if( st1.wMonth < st2.wMonth )
		return true;
	else if( st1.wMonth > st2.wMonth )
		return false;

	// Day
	if( st1.wDay < st2.wDay )
		return true;
	else if( st1.wDay > st2.wDay )
		return false;

	// Hour
	if( st1.wHour < st2.wHour )
		return true;
	else if( st1.wHour > st2.wHour )
		return false;

	// Minute
	if( st1.wMinute < st2.wMinute )
		return true;
	else if( st1.wMinute > st2.wMinute )
		return false;

	// Second
	if( st1.wSecond < st2.wSecond )
		return true;
	else if( st1.wSecond > st2.wSecond )
		return false;

	// Millisecond
	if( st1.wMilliseconds < st2.wMilliseconds )
		return true;
	else
		return false;
}

#endif //__UTILS_H__