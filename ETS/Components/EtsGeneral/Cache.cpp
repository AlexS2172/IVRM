#include "StdAfx.h"
#include "Cache.h"

//---------------------------------------------------------------------------//
CCache::CCache(void)
{
	Clear();
}
//---------------------------------------------------------------------------//
CCache::~CCache(void)
{
	Clear();
}
//---------------------------------------------------------------------------//
void	CCache::Clear()
{
	m_pMain = NULL;
	m_bstrDBConnectionString = L"";
};
//--------------------------------------------------------------------------------------------------------
bool CCache::SetMain(IEtsMainPtr spMain)
{
	ATLASSERT(spMain != NULL);

	if (static_cast<bool>(spMain))
	{
		if (m_pMain == NULL)	
			m_pMain = spMain.GetInterfacePtr();
		return true;
	}
	return false;
};
//--------------------------------------------------------------------------------------------------------
IEtsMainPtr		CCache::GetMain()
{
	ATLASSERT(m_pMain != NULL);
	IEtsMainPtr spRet = m_pMain;
	return spRet;
}
//--------------------------------------------------------------------------------------------------------
CDBConnection& CCache::GetDBConnection()
{
	return m_DbConnection;
}
//--------------------------------------------------------------------------------------------------------
bool	CCache::SetConnectionString(BSTR bsDBConnectionString)
{
	m_bstrDBConnectionString = CComBSTR(bsDBConnectionString);
	return	true;
};
//--------------------------------------------------------------------------------------------------------
bool	CCache::InitializeDB()
{
	try
	{
		if(!m_DbConnection.IsOpened())
			m_DbConnection.Open(_bstr_t((BSTR)m_bstrDBConnectionString), 10, 120, 300, 300);
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
		throw;
	}
	catch(...)
	{
		TRACE_UNKNOWN_ERROR();
		throw;		
	}
	return true;
};
//--------------------------------------------------------------------------------------------------------
