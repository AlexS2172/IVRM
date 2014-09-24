// Extended.h : Declaration of the CExtended

#ifndef __EXTENDED_H__
#define __EXTENDED_H__

#include "Database.h"
/////////////////////////////////////////////////////////////////////////////
// CExtended

class CExtended : 
	public CDatabase<DBA_EXTENDED>
{
public:
	virtual ~CExtended(){}
};


#endif //__EXTENDED_H__
