//==============================================================================
//	File name: 		pcqutils.h
//
//  Description:	A collection of utility routines used by many of the sample
//					programs.  Each program will include this header file in 
//					its own project.
//
//	HyperFeed
//	Databases Used:	N/A
//
//  HyperFeed APIs	
//	Used:			N/A
//
//  History:		Created June 2002 by Todd Peterson
//						- added comments
//						- made variable names more descriptive
//==============================================================================

#if !defined(PCQUTIL_H__INCLUDED_)
#define PCQUTIL_H__INCLUDED_

#include <string.h> 
#include <ctype.h> 
#include <math.h>

//---------------------------------------------
// include files - HyperFeed related
//---------------------------------------------
#include <etix.h>		// PC Quote ETI data structures 
#include "pcqutils.h"



/**************************************************************************
	s t r p t r  =  c l _ s w i t c h ( a r g c , a r g v , c h a r ) ;
**************************************************************************/
char* clSwitch(int argc, char** argv, char pa_sw)
{
    static char cl_sw[4] =
        {
            '/', 0, ':', 0
        };
    int i;
    char* pos,* strstr();

    cl_sw[1] = toupper(pa_sw); /* Convert to UpperCase */
    i = 1;

    while (argc > 1)
    {
        pos = strstr(argv[i], cl_sw);

        if (pos != NULL)
            return(pos + 3);

        cl_sw[1] = tolower(pa_sw); /* Try Lowercase */
        pos = strstr(argv[i], cl_sw);
        if (pos != NULL)
            return(pos + 3);
        i++;
        argc--;
    }
    return(NULL);
}

float PriceToFloat(USHORT nPriceType, LONG lPrice)
{
	double dDenominator = 1.0;
	float fReturn = 0.0;

	switch(nPriceType)
	{
	case PT_LCD_256:
	case PT_FUTURE_256:		// should display as 256ths i.e. "24 13/256"
	case PT_FUTURE_64:		// should display as 64ths
	case PT_FUTURE_32:		// should display as 32nds
	case PT_FUTURE_8:		// should display as 8ths
		dDenominator = 256;
		break;
	case PT_LCD_64:
		dDenominator = 64;
		break;
	case PT_DECIMAL_0:
	case PT_LEADING:			// should display with leading zeros, ie "00147"
		dDenominator = 1;
		break;
	case PT_DECIMAL_1: 
	case PT_DECIMAL_2: 
	case PT_DECIMAL_3: 
	case PT_DECIMAL_4: 
	case PT_DECIMAL_5: 
	case PT_DECIMAL_6: 
	case PT_DECIMAL_7: 
	case PT_DECIMAL_8: 
	case PT_DECIMAL_9: 
		dDenominator = pow(10, (nPriceType - 10));
		break;

	default:
		break;
	}

	fReturn = (float)(lPrice/(float)dDenominator);

	return(fReturn);
}

#endif // !defined(PCQUTIL_H__INCLUDED_)
