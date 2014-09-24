// Database.h : Declaration of the CDatabase

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <edbx.h>			// PC Quote DBA data structures
#include "resource.h"		// main symbols
#include "Utilities.h"

#define DBA_TERMINATED -1

/////////////////////////////////////////////////////////////////////////////
// CDatabase
template <long _Dba>
class CDatabase : virtual public CUtilities
{
public:
	CDatabase() : 
		m_bAttached(false),
		m_nKeyNum(0)
	{
		ZeroMemory(&m_dbaKey, sizeof (DBA_KEYS));
		ZeroMemory(&m_dbaRec, sizeof (DBA_RECORD));
		ZeroMemory(&m_blk, sizeof (DBAPBLK));
	}
	~CDatabase()
	{
		Detach();
	}

protected:
	DBA_RECORD	m_dbaRec;			/* stores last extended record */
	bool		m_bAttached;
	short		m_nKeyNum;
	DBAPBLK		m_blk;
	DBA_KEYS	m_dbaKey;			/* used for DBAX access */

public:
	long Attach()
	{
		if (m_bAttached)
		{
			return DBA_ERR_NO_ERROR;
		}
		long status = dba(&m_blk, DBA_ATTACH, NULL, 0, NULL, 0, _Dba);
		m_bAttached = status == DBA_ERR_NO_ERROR;
		return status;
	}
	long Detach()
	{
		if (!m_bAttached)
		{
			return DBA_ERR_NO_ERROR;
		}
		long status = dba(&m_blk, DBA_DETACH,  NULL, 0, NULL, 0, _Dba);
		m_bAttached = status != DBA_ERR_NO_ERROR;
		return status;
	}
protected:
	long GetEqual()
	{
		return dbax(&m_blk, DBA_GET_EQUAL, &m_dbaKey, &m_dbaRec, m_nKeyNum);
	}
	long GetApprox()
	{
		return dbax(&m_blk, DBA_GET_APPROX, &m_dbaKey, &m_dbaRec, m_nKeyNum);
	}
	long  GetFirst()
	{
		return dbax(&m_blk, DBA_GET_FIRST, &m_dbaKey, &m_dbaRec, m_nKeyNum);
	}
	long  GetLast()
	{
		return dbax(&m_blk, DBA_GET_LAST, &m_dbaKey, &m_dbaRec, m_nKeyNum);
	}
	long  GetNext()
	{
		return dbax(&m_blk, DBA_GET_NEXT, &m_dbaKey, &m_dbaRec, m_nKeyNum);
	}
	long GetPrev()
	{
		return dbax(&m_blk, DBA_GET_PREV, &m_dbaKey, &m_dbaRec, m_nKeyNum);
	}
	long  GetNextSub()
	{
		return dbax(&m_blk, DBA_GET_NEXT_SUB, &m_dbaKey, &m_dbaRec, m_nKeyNum);
	}
	long  GetHigher()
	{
		return dbax(&m_blk, DBA_GET_HIGHER, &m_dbaKey, &m_dbaRec, m_nKeyNum);
	}	
}; 

#endif //__DATABASE_H__
