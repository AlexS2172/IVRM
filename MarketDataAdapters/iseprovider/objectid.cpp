#include "StdAfx.h"
#include "ISEProvider.h"
#include "ObjectID.h"

DWORD CObjectID::m_dwUniqueID = 0;
CLock CObjectID::m_LockUniqueID;