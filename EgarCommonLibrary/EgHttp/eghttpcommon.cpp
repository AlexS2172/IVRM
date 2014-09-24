#include "stdafx.h"
#include "EgHttpCommon.h"

/////////////////////////////////////////////////////////////////////////////
// This function performs case-insensitive search that makes unnecessary case conversions
LPCTSTR _tcsistr(LPCTSTR ptszStr1, LPCTSTR ptszStr2)
{
    LPCTSTR cp(ptszStr1);
    LPCTSTR s1;
	LPCTSTR s2;

    if (!*ptszStr2)
	{
        return ptszStr1;
	}

    while (*cp)
    {
            s1 = cp;
            s2 = ptszStr2;

            while (*s1 && *s2)
			{
                TCHAR szCmp [3] = { *s1, *s2, _T('\0') };

#if _MSC_VER >= 1400
				_tcslwr_s (szCmp, sizeof(szCmp));
#else
				_tcslwr (szCmp);
#endif

				if ( szCmp[0]-szCmp[1] ) 
					break;

				s1++, s2++;
			}	
            if (!*s2)
			{
				return(cp);
			}

            cp++;
    }

	return(NULL);
}

