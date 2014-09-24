#include "stdafx.h"

#include "ISESession.h"
#include "OmniApiUtils.h"

/*--------------------------------[ User information ]--------------------------------------------*/

void CISESession::QueryUserType()
{
	CISEQueryUserInfo	qDQ30;
	CISEAnswer			aDQ30;
	uint64_t			uiTransactionID;
	uint64_t			uiOrderID;

	qDQ30.segment_number_n = 0;

	try
	{
		uint16 nItems;

		do
		{
			qDQ30.segment_number_n++;

			SendQuery(qDQ30, m_uiBaseFacility + FACILITY_EP0, aDQ30, 
				uiTransactionID, uiOrderID);

			EgAssert(aDQ30.m_uiLen <= sizeof(answer_user_type_info_t));
			
			CAutoLock UserLock(&m_User);

			CS2S(aDQ30.m_Data.m_UserType.ust_id_s, m_User.m_sType);	// Type
			m_User.m_bIsInternal = (aDQ30.m_Data.m_UserType.ext_or_int_c == USERTYPE_INTERNAL);
			m_User.m_bIsTrader = (aDQ30.m_Data.m_UserType.is_trader_c == USERTYPE_TRADER);

			nItems = aDQ30.m_Data.m_UserType.items_n;

			for(uint16	nIndex = 0; nIndex < nItems; nIndex++)
			{
				const answer_user_type_info_item_t&	Item = aDQ30.m_Data.m_UserType.item[nIndex];

				CMessageID		TranID;
				TranID.m_Type = Item.transaction_type;
				TranID.m_bIsBroadcast = (Item.trans_or_bdx_c == TRANTYPE_BROADCAST);
				m_User.m_AllowedMessages.push_back(TranID);
			}

		}while(aDQ30.m_Data.m_UserType.segment_number_n != 0 && nItems != 0);
	}
	catch(CISEException& e)
	{
		e.AddDescription("Failed to query user type.");
		throw;
	}
}

void CISESession::QueryUserInfo(const char* szUser, const char* szPassword)
{
	user_code_t	UserCode;
	uint32		uiLen = sizeof(user_code_t);

	int32  iTranStatus = 0;
	int32  iStatus; 

	do
	{
		{
			CAutoLock	OmniApiLock(&m_OmniApiLock);

			iStatus = omniapi_get_info_ex(m_hSession, &iTranStatus, OMNI_INFTYP_USERCODE,
								&uiLen, &UserCode);
		}

		if(iStatus == OMNIAPI_NOT_READY)
		{
			if(LogonSleep(200) == false)
				throw CISEException(0, 0, this, "Operation cancelled.");
		}
	}while(iStatus == OMNIAPI_NOT_READY);

	if(CSTATUS_FAILED(iStatus))
		throw CISEException(iStatus, iTranStatus, this, "Failed to collect user information");

	EgAssert(uiLen == sizeof(user_code_t)); 

	{
		CAutoLock UserLock(&m_User);

		CS2S(UserCode.country_id_s, m_User.m_sCountryID);
		CS2S(UserCode.ex_customer_s, m_User.m_sCustomerID);
		CS2S(UserCode.user_id_s, m_User.m_sID);
		m_User.m_sPassword = szPassword;
		m_User.m_sName = szUser;
	}

	uiLen = sizeof(m_uiBaseFacility);
	iTranStatus = 0;

	{
		CAutoLock	OmniApiLock(&m_OmniApiLock);

		iStatus = omniapi_get_info_ex(m_hSession, &iTranStatus, OMNI_INFTYP_FACTYP_E0,
							&uiLen, &m_uiBaseFacility);
	}

	if(CSTATUS_FAILED(iStatus))
		throw CISEException(iStatus, iTranStatus, this, "Failed to get CLICK facility information");

	EgAssert(uiLen == sizeof(m_uiBaseFacility)); 

	// query user type
	QueryUserType();
}
