#include "StdAfx.h"
#include "Provider.h"

//-------------------------------------------------------------------//
CGateway::CGateway(CGatewayNotify *client):
m_client(client)
{
	m_spData = CDataCollPtr( new CDataColl );
};
//-------------------------------------------------------------------//
CGateway::~CGateway(void)
{
};
//-------------------------------------------------------------------//
bool CGateway::FindResponse(CRequestKeyPtr key,
							ResponseTypeEnum type)
{
	CAutoLock lock(m_dataLock);

	if (m_spData)
	{
		CDataColl::iterator it = m_spData->find(key);

		CResponseCollPtr spResponseColl;

		if (m_spData->end() == it){
			return false;
		}
		spResponseColl = it->second;

		CResponsePtr spResponse;
		CResponseColl::iterator itRespone = spResponseColl->find(type);
		if (spResponseColl->end() == itRespone){
			return false;
		}
		return true;
	}
	return false;
};
//-------------------------------------------------------------------//
bool CGateway::GetResponse(CRequestKeyPtr key,
							ResponseTypeEnum type,
							CResponsePtr &response)
{
	CAutoLock lock(m_dataLock);

	bool __isExist = true;

	if (m_spData)
	{
		CDataColl::iterator it = m_spData->find(key);

		CResponseCollPtr spResponseColl;

		if (m_spData->end() == it){
			spResponseColl = CResponseCollPtr(new CResponseColl);
			(*m_spData)[key] = spResponseColl;
			__isExist = false;
		}
		else{
			spResponseColl = it->second;
		};

		CResponsePtr spResponse;
		CResponseColl::iterator itRespone = spResponseColl->find(type);
		if (spResponseColl->end() == itRespone){
			if (enQuotesResponse == type)
				spResponse = CResponsePtr(new CQuoteResponse(type, key));
			else if (enRisksResponse == type)
				spResponse = CResponsePtr(new CRisksResponse(type, key));

			if (spResponse)
			{
				(*spResponseColl)[type] = spResponse;
				__isExist = false;
			}
		}
		else{
			spResponse = itRespone->second;
		}

		if (spResponse)
			response = spResponse;
	}
	return __isExist;
};
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
