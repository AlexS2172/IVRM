/**
 *  @file	SmtpHelper.h
 *
 *	@brief	Smtp helper class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_MISC_SMTP_HELPER_H)
#define ACTIV_MISC_SMTP_HELPER_H

#include "ActivMiddleware/ActivBase/ActivBase.h"
#include "ActivMiddleware/ActivBase/StatusCodeException.h"
#include "ActivMiddleware/ActivBase/Thread.h"

#include "ActivMiddleware/ActivIp/IpManager.h"

#include "boost/tokenizer.hpp"

#include <string>

namespace Activ
{

/**
 *	@brief	Smtp helper class.
 */
class SmtpHelper
{
public:
	/**
	 *	@brief	Email class.
	 */
	class Email
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	fromAddress the address the message is from.
		 *	@param	toAddressList the 'to' address list.
		 *	@param	ccAddressList the 'cc' address list.
		 *	@param	bccAddressList the 'bcc'address list.
		 *	@param	subject the message subject.
		 *	@param	text the message text.
		 */
		Email(const std::string &fromAddress, const std::string &toAddressList, const std::string &ccAddressList, 
			  const std::string &bccAddressList, const std::string &subject, const std::string &text);

		const std::string	m_fromAddress;						///< the address the message is from.
		const std::string	m_toAddressList;					///< the 'to' address list.
		const std::string	m_ccAddressList;					///< the 'cc' address list.
		const std::string	m_bccAddressList;					///< the 'bcc' address list.
		const std::string	m_subject;							///< the message subject.
		const std::string	m_text;								///< the message text.
	};

	/**
	 *	@brief	Default constructor.
	 *
	 *	@throw	StatusCodeException
	 */
	SmtpHelper();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	server the server.
	 *	@param	hostname the hostname.
	 *
	 *	@throw	StatusCodeException
	 */
	SmtpHelper(const std::string &server, const std::string &hostname = GetHostname());

	/**
	 *	@brief	Destructor.
	 */
	~SmtpHelper();

	/**
	 *	@brief	Connect to the server.
	 *
	 *	@param	server the server.
	 *	@param	hostname the hostname.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Connect(const std::string &server, const std::string &hostname = GetHostname());

	/**
	 *	@brief	Disconnect from the server.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Disconnect();

	/**
	 *	@brief	Post a message.
	 *
	 *	@param	fromAddress the address the message is from.
	 *	@param	toAddressList the 'to' address list.
	 *	@param	ccAddressList the 'cc' address list.
	 *	@param	bccAddressList the 'bcc'address list.
	 *	@param	subject the message subject.
	 *	@param	text the message text.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode PostMessage(const std::string &fromAddress, const std::string &toAddressList, const std::string &ccAddressList, const std::string &bccAddressList, const std::string &subject, const std::string &text);

	/**
	 *	@brief	Is connected to the server.
	 *
	 *	@return	whether the object is connected to the server.
	 */
	bool IsConnected() const;

	/**
	 *	@brief	Get the hostname.
	 *
	 *	@return	the hostname.
	 */
	static std::string GetHostname();

	/**
	 *	@brief	Post an email asynchronously.
	 *
	 *	Starts a new thread to send an email. An exception is thrown only on failure of starting a new thread, not if the attempt to
	 *	send the email fails.
	 *
	 *	@param	email the email to send.
	 *	@param	server the mail server to use.
	 *	@param	hostname the hostname.
	 *
	 *	@throw	Exception
	 */
	static void PostEmail(const Email &email, const std::string &server, const std::string &hostname = GetHostname());

private:
	/**
	 *	@brief	Send email thread.
	 *
	 *	This thread deletes itself when it finishes.
	 */
	class PostEmailThread : public Thread
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	email the email to send.
		 *	@param	server the mail server to use.
		 *	@param	hostname the hostname.
		 */
		PostEmailThread(const Email &email, const std::string &server, const std::string &hostname = GetHostname());

	private:
		// Thread interface
		virtual StatusCode Process(void *pData);

		Email		m_email;									///< the email to send.
		std::string	m_server;									///< the mail server to use.
		std::string	m_hostname;									///< the hostname.
	};

	static const uint16_t PORT_NUMBER = 25; // The port number.
	static const uint32_t READ_TIMEOUT = 5; // The read timeout.

	/**
	 *	@brief	Confirm reply code.
	 *
	 *	@param	replyCode the reply code.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 */
	StatusCode ConfirmReplyCode(const std::string &replyCode);

	/**
	 *	@brief	Connect the socket.
	 *
	 *	@param	server the server.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 */
	StatusCode ConnectSocket(const std::string &server);

	/**
	 *	@brief	Disconnect the socket.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 */
	StatusCode DisconnectSocket();

	/**
	 *	@brief	Read data from the socket.
	 *
	 *	@param	pData a pointer to the data buffer.
	 *	@param	size the size of the data buffer.
	 *	@param	length the number of characters read.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	StatusCode ReadSocket(void * const pData, const size_t size, size_t &length);

	/**
	 *	@brief	Write data to the socket.
	 *
	 *	@param	pData a pointer to the data buffer.
	 *	@param	length the number of characters to write.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 */
	StatusCode WriteSocket(const void * const pData, const size_t length);

	SOCKET	m_socket;		///< The socket
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline SmtpHelper::SmtpHelper() :
	m_socket(INVALID_SOCKET)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline SmtpHelper::SmtpHelper(const std::string &server, const std::string &hostname) :
	m_socket(INVALID_SOCKET)
{
	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, Connect(server, hostname));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline SmtpHelper::~SmtpHelper()
{
	if (IsConnected())
		(void) Disconnect();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode SmtpHelper::Connect(const std::string &server, const std::string &hostname)
{
	if (IsConnected())
		return STATUS_CODE_ALREADY_CONNECTED;

	try
	{
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ConnectSocket(server));

		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, ConfirmReplyCode("220"));

		const std::string heloCommand("HELO " + hostname + "\r\n");
		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, WriteSocket(heloCommand.c_str(), heloCommand.length()));

		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, ConfirmReplyCode("250"));
	}
	catch (StatusCodeException &statusCodeException)
	{
		(void) DisconnectSocket();

		return static_cast<const StatusCode>(statusCodeException.m_errorCode);
	}
	catch (...)
	{
		(void) DisconnectSocket();

		return STATUS_CODE_FAILURE;
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode SmtpHelper::Disconnect()
{
	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	try
	{
		const std::string quitCommand("QUIT\r\n");
		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, WriteSocket(quitCommand.c_str(), quitCommand.length()));

		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, ConfirmReplyCode("221"));

		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DisconnectSocket());
	}
	catch (StatusCodeException &statusCodeException)
	{
		(void) DisconnectSocket();

		return static_cast<const StatusCode>(statusCodeException.m_errorCode);
	}
	catch (...)
	{
		(void) DisconnectSocket();

		return STATUS_CODE_FAILURE;
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode SmtpHelper::PostMessage(const std::string &fromAddress, const std::string &toAddressList, const std::string &ccAddressList, const std::string &bccAddressList, const std::string &subject, const std::string &text)
{
	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	try
	{
		const std::string mailFromCommand("MAIL FROM: " + fromAddress + "\r\n");
		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, WriteSocket(mailFromCommand.c_str(), mailFromCommand.length()));

		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, ConfirmReplyCode("250"));

		typedef boost::char_separator<char> CharSeparator;
		const CharSeparator semiColonSeparator(";");

		boost::tokenizer<CharSeparator> tokenizer(toAddressList, semiColonSeparator);

		for (boost::tokenizer< boost::char_separator<char> >::const_iterator iterator = tokenizer.begin(), iteratorEnd = tokenizer.end(); iteratorEnd != iterator; ++iterator)
		{
			const std::string rcptToCommand("RCPT TO: " + *iterator + "\r\n");
			ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, WriteSocket(rcptToCommand.c_str(), rcptToCommand.length()));

			ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, ConfirmReplyCode("250"));
		}

		tokenizer.assign(ccAddressList, semiColonSeparator);

		for (boost::tokenizer< boost::char_separator<char> >::const_iterator iterator = tokenizer.begin(), iteratorEnd = tokenizer.end(); iteratorEnd != iterator; ++iterator)
		{
			const std::string rcptToCommand("RCPT TO: " + *iterator + "\r\n");
			ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, WriteSocket(rcptToCommand.c_str(), rcptToCommand.length()));

			ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, ConfirmReplyCode("250"));
		}

		tokenizer.assign(bccAddressList, semiColonSeparator);

		for (boost::tokenizer< boost::char_separator<char> >::const_iterator iterator = tokenizer.begin(), iteratorEnd = tokenizer.end(); iteratorEnd != iterator; ++iterator)
		{
			const std::string rcptToCommand("RCPT TO: " + *iterator + "\r\n");
			ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, WriteSocket(rcptToCommand.c_str(), rcptToCommand.length()));

			ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, ConfirmReplyCode("250"));
		}

		const std::string dataCommand("DATA\r\n");
		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, WriteSocket(dataCommand.c_str(), dataCommand.length()));

		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, ConfirmReplyCode("354"));

		std::string dataHeader;

		dataHeader += std::string("From: " + fromAddress + "\r\n");
		dataHeader += std::string("To: " + toAddressList + "\r\n");
		dataHeader += std::string("Cc: " + ccAddressList + "\r\n");
		dataHeader += std::string("Subject: " + subject + "\r\n");
		dataHeader += std::string("\r\n");

		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, WriteSocket(dataHeader.c_str(), dataHeader.length()));
		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, WriteSocket(text.c_str(), text.length()));

		const std::string endOfDataCommand("\r\n.\r\n");
		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, WriteSocket(endOfDataCommand.c_str(), endOfDataCommand.length()));

		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, ConfirmReplyCode("250"));
	}
	catch (StatusCodeException &statusCodeException)
	{
		(void) DisconnectSocket();

		return static_cast<const StatusCode>(statusCodeException.m_errorCode);
	}
	catch (...)
	{
		(void) DisconnectSocket();

		return STATUS_CODE_FAILURE;
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool SmtpHelper::IsConnected() const
{
	return (INVALID_SOCKET != m_socket);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string SmtpHelper::GetHostname()
{
	return IpManager::GetHostName();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode SmtpHelper::ConfirmReplyCode(const std::string &replyCode)
{
	char buffer[1024];
	size_t length;
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ReadSocket(buffer, ACTIV_ARRAY_SIZE(buffer), length));

	return (((length >= replyCode.length()) && (0 == ::memcmp(buffer, replyCode.c_str(), replyCode.length()))) ? STATUS_CODE_SUCCESS : STATUS_CODE_FAILURE);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode SmtpHelper::ConnectSocket(const std::string &server)
{

	try
	{
		IpAddress remoteAddress(SOCK_STREAM, IPPROTO_TCP);
		IpManager::GetIpAddress(server, PORT_NUMBER, remoteAddress);

		m_socket = ::socket(remoteAddress.m_address.Address.sa_family, remoteAddress.m_type, remoteAddress.m_protocol);

		if (INVALID_SOCKET == m_socket)
			ACTIV_THROW(Activ::StatusCodeException, STATUS_CODE_FAILURE);

		if (SOCKET_ERROR == ::connect(m_socket, reinterpret_cast<const sockaddr *>(&remoteAddress.m_address), remoteAddress.m_addressLength))
			ACTIV_THROW(Activ::StatusCodeException, STATUS_CODE_FAILURE);
	}
	catch (StatusCodeException &statusCodeException)
	{
		(void) DisconnectSocket();

		return static_cast<const StatusCode>(statusCodeException.m_errorCode);
	}
	catch (...)
	{
		(void) DisconnectSocket();

		return STATUS_CODE_FAILURE;
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode SmtpHelper::DisconnectSocket()
{
	if (INVALID_SOCKET != m_socket)
		(void) closesocket(m_socket);

	m_socket = INVALID_SOCKET;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode SmtpHelper::ReadSocket(void * const pData, const size_t size, size_t &length)
{
	fd_set readFds;
	FD_ZERO(&readFds);
	FD_SET(m_socket, &readFds);

	struct timeval timeout;
	timeout.tv_sec = READ_TIMEOUT;
	timeout.tv_usec = 0;

	int result = ::select(1, &readFds, 0, 0, &timeout);

	if (SOCKET_ERROR == result)
		return STATUS_CODE_FAILURE;
	if (0 == result)
		return STATUS_CODE_TIMEOUT;

	result = ::recv(m_socket, reinterpret_cast<char * const>(pData), static_cast<const int>(size), 0);

	if (SOCKET_ERROR == result)
		return STATUS_CODE_FAILURE;

	length = static_cast<const size_t>(result);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode SmtpHelper::WriteSocket(const void * const pData, const size_t length)
{
	const int toWrite = static_cast<const int>(length);

	for (int written = 0, result; written < toWrite; written += result)
	{
		result = ::send(m_socket, reinterpret_cast<const char * const>(pData) + written, toWrite - written, 0);

		if (SOCKET_ERROR == result)
			return STATUS_CODE_FAILURE;
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void SmtpHelper::PostEmail(const Email &email, const std::string &server, const std::string &hostname)
{
	std::auto_ptr<PostEmailThread> pPostEmailThread(new (std::nothrow) PostEmailThread(email, server, hostname));

	if (0 == pPostEmailThread.get())
		ACTIV_THROW(StatusCodeException, STATUS_CODE_HEAP_EMPTY);

	pPostEmailThread->Start(); // this might throw an exception

	// if we get to here, thread started ok and will delete itself
	pPostEmailThread.release();
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

inline SmtpHelper::Email::Email(const std::string &fromAddress, const std::string &toAddressList, const std::string &ccAddressList, 
								const std::string &bccAddressList, const std::string &subject, const std::string &text) :
	m_fromAddress(fromAddress),
	m_toAddressList(toAddressList),
	m_ccAddressList(ccAddressList),
	m_bccAddressList(bccAddressList),
	m_subject(subject),
	m_text(text)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

inline SmtpHelper::PostEmailThread::PostEmailThread(const Email &email, const std::string &server, const std::string &hostname) :
	m_email(email),
	m_server(server),
	m_hostname(hostname)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode SmtpHelper::PostEmailThread::Process(void *pData)
{
	SmtpHelper smtpHelper;

	StatusCode statusCode = smtpHelper.Connect(m_server, m_hostname);

	if (STATUS_CODE_SUCCESS != statusCode)
	{
		LogMessage(LOG_TYPE_INFO, "Error sending email, failed to connect to " + m_server);
		return statusCode;
	}

	statusCode = smtpHelper.PostMessage(m_email.m_fromAddress, m_email.m_toAddressList, m_email.m_ccAddressList, 
		m_email.m_bccAddressList, m_email.m_subject, m_email.m_text);

	if (STATUS_CODE_SUCCESS != statusCode)
	{
		LogMessage(LOG_TYPE_INFO, "Error posting email message");
		return statusCode;
	}
	
	delete this;

	return STATUS_CODE_SUCCESS;
}

} // namespace Activ

#endif // (!defined ACTIV_MISC_SMTP_HELPER_H)
