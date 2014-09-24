#ifndef ISE_USER_H
#define ISE_USER_H

struct	CMessageID
{
	transaction_type_t	m_Type;
	bool				m_bIsBroadcast;
};

typedef vector<CMessageID>		vecMessageIDs;

struct CISEUser : public CLock
{
	string			m_sName;
	string			m_sPassword;

	string			m_sCountryID;
	string			m_sCustomerID;
	string			m_sID;

	string			m_sType;
	bool			m_bIsInternal;
	bool			m_bIsTrader;

	vecMessageIDs	m_AllowedMessages;
};

#endif