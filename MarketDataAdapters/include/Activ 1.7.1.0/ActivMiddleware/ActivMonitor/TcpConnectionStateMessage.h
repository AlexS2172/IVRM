/**
 *  @file	ActivMonitor/TcpConnectionStateMessage.h
 *
 *	@brief	Header file for monitor tcp connection state message class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_MONITOR_TCP_CONNECTION_STATE_MESSAGE_H)
#define ACTIV_MONITOR_TCP_CONNECTION_STATE_MESSAGE_H

#include "ActivMonitor.h"

#include "ActivMiddleware/ActivRouter/AddressInfo.h"

namespace Activ
{

namespace Monitor
{

/**
 *	@brief	Tcp connection state message.
 */
class TcpConnectionStateMessage : public Message
{
public:
	static const MessageType MESSAGE_TYPE = MONITOR_MESSAGE_TCP_CONNECTION_STATE;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder.
	 *	@param	systemName.
	 *	@param	clientAddressInfo local address, remote address, protocol.
	 *	@param	isConnection is the connection up?
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const AddressInfo &addressInfo, const bool isConnected);

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deserialize(MessageValidator &messageValidator);

	AddressInfo	m_addressInfo;
	bool		m_isConnected;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode TcpConnectionStateMessage::Serialize(MessageBuilder &messageBuilder, const std::string &systemName, const AddressInfo &addressInfo, const bool isConnected)
{
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Serialize(messageBuilder, systemName));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, addressInfo.Serialize(messageBuilder));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(static_cast<uint8_t>(isConnected), MessageHandler::ENDIAN_LITTLE));

	// for backwards compat
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(static_cast<uint8_t>(0), MessageHandler::ENDIAN_LITTLE));

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode TcpConnectionStateMessage::Deserialize(MessageValidator &messageValidator)
{
	uint8_t temp8;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, Message::Deserialize(messageValidator));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_addressInfo.Deserialize(messageValidator));
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(temp8), MessageHandler::ENDIAN_LITTLE, &temp8));
	m_isConnected = (temp8 != 0);
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(temp8), MessageHandler::ENDIAN_LITTLE, &temp8));

	return STATUS_CODE_SUCCESS;
}

} // namespace Monitor

} // namespace Activ

#endif // !defined (ACTIV_MONITOR_TCP_CONNECTION_STATE_MESSAGE_H)
