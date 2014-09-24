/**
 *	@file	UiIo.h
 *
 *	@brief	Header file for the ui io class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_FRAMEWORK_UI_IO_H)
#define ACTIV_FRAMEWORK_UI_IO_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivFieldTypes/BinaryArray.h"
#include "ActivMiddleware/ActivFieldTypes/BinaryString.h"
#include "ActivMiddleware/ActivFieldTypes/Blob.h"
#include "ActivMiddleware/ActivFieldTypes/CrcBlob.h"
#include "ActivMiddleware/ActivFieldTypes/Date.h"
#include "ActivMiddleware/ActivFieldTypes/DateTime.h"
#include "ActivMiddleware/ActivFieldTypes/Rational.h"
#include "ActivMiddleware/ActivFieldTypes/SInt.h"
#include "ActivMiddleware/ActivFieldTypes/TextArray.h"
#include "ActivMiddleware/ActivFieldTypes/TextString.h"
#include "ActivMiddleware/ActivFieldTypes/Time.h"
#include "ActivMiddleware/ActivFieldTypes/TRational.h"
#include "ActivMiddleware/ActivFieldTypes/UInt.h"

#include "boost/format.hpp"

namespace Activ
{

class UiUser;
class UiMenu;

/**
 *	@brief	Ui io class. Provides methods to get input from and post input to a user.
 */
class UiIo
{
public:
	/**
	 *	@brief	Put a string.
	 *
	 *	@param	str the string to write.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode PutString(const std::string &str) const;
	
	/**
	 *	@brief	Put a string from a boost format object.
	 *
	 *	@param	formatter boost formatter object to create the string from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode PutString(const boost::format &formatter) const;

	/**
	 *	@brief	Put a variable argument string.
	 *
	 *	@param	format printf style format string.
	 *	@param	... variable arguments to write.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode PutString(const char * const format, ...) const;

	/**
	 *	@brief	Put a variable argument string.
	 *
	 *	@param	format printf style format string.
	 *	@param	args variable arguments to write.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode PutStringV(const char * const format, std::va_list args) const;

	/**
	 *	@brief	Get a string.
	 *
	 *	@param	prompt the prompt to display. Pass empty string to display no prompt.
	 *	@param	str the string to read into, or the already initialized default.
	 *	@param	initializedWithDefault whether the string is initialized with a default.
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_UNCHANGED
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	ACTIV_FRAMEWORK_API StatusCode GetString(const std::string &prompt, std::string &str, const bool initializedWithDefault = false, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;

	/**
	 *	@brief	Get a string within bounds.
	 *
	 *	@param	prompt the prompt to display. Pass empty string to display no prompt.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	str the string to read into, or the already initialized default.
	 *	@param	initializedWithDefault whether the string is initialized with a default.
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_UNCHANGED
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	ACTIV_FRAMEWORK_API StatusCode GetString(const std::string &prompt, const byte_t lower, const byte_t upper, std::string &str, const bool initializedWithDefault = false, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;

	/**
	 *	@brief	Get a character.
	 *
	 *	@param	prompt the prompt to display. Pass empty string to display no prompt.
	 *	@param	ch the character to read into, or the already initialized default.
	 *	@param	initializedWithDefault whether the character is initialized with a default.
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_UNCHANGED
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	ACTIV_FRAMEWORK_API StatusCode GetChar(const std::string &prompt, char &ch, const bool initializedWithDefault = false, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;

	/**
	 *	@brief	Get a character and validate that it is present in the valid character string.
	 *
	 *	@param	prompt the prompt to display. Pass empty string to display no prompt.
	 *	@param	validChars a string containing the valid characters.
	 *	@param	ch the character to read into, or the already initialized default.
	 *	@param	initializedWithDefault whether the character is initialized with a default.
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_UNCHANGED
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	ACTIV_FRAMEWORK_API StatusCode GetChar(const std::string &prompt, const std::string &validChars, char &ch, const bool initializedWithDefault = false, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;
	
	/**
	 *	@brief	Get an unsigned integral of a given type.
	 *
	 *	@param	prompt the prompt to display. Pass empty string to display no prompt.
	 *	@param	value the value to read into, or the already initialized default.
	 *	@param	initializedWithDefault whether the value is initialized with a default.
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_UNCHANGED
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	template<typename T>
	ACTIV_TEMPLATE_API(ACTIV_FRAMEWORK_API) StatusCode GetUnsignedIntegral(const std::string &prompt, T &value, const bool initializedWithDefault = false, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;

	/**
	 *	@brief	Get an unsigned integral of a given type within a range.
	 *
	 *	@param	prompt the prompt to display. Pass empty string to display no prompt.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	value the value to read into, or the already initialized default.
	 *	@param	initializedWithDefault whether the value is initialized with a default.
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_UNCHANGED
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	template<typename T>
	ACTIV_TEMPLATE_API(ACTIV_FRAMEWORK_API) StatusCode GetUnsignedIntegral(const std::string &prompt, const uint64_t lower, const uint64_t upper, T &value, const bool initializedWithDefault = false, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;

	// explicit instantiations for unsigned types
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetUnsignedIntegral<uint8_t>(const std::string &, uint8_t &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetUnsignedIntegral<uint16_t>(const std::string &, uint16_t &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetUnsignedIntegral<uint32_t>(const std::string &, uint32_t &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetUnsignedIntegral<uint64_t>(const std::string &, uint64_t &, const bool, const TimeoutPeriod) const;)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetUnsignedIntegral<size_t>(const std::string &, size_t &, const bool, const TimeoutPeriod) const;)
#endif

	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetUnsignedIntegral<uint8_t>(const std::string &, const uint64_t, const uint64_t, uint8_t &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetUnsignedIntegral<uint16_t>(const std::string &, const uint64_t, const uint64_t, uint16_t &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetUnsignedIntegral<uint32_t>(const std::string &, const uint64_t, const uint64_t, uint32_t &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetUnsignedIntegral<uint64_t>(const std::string &, const uint64_t, const uint64_t, uint64_t &, const bool, const TimeoutPeriod) const;)
#if defined (ACTIV_EXPLICIT_INSTANTIATE_SIZE_T)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetUnsignedIntegral<size_t>(const std::string &, const uint64_t, const uint64_t, size_t &, const bool, const TimeoutPeriod) const;)
#endif

	/**
	 *	@brief	Get a signed integral of a given type.
	 *
	 *	@param	prompt the prompt to display. Pass empty string to display no prompt.
	 *	@param	value the value to read into, or the already initialized default.
	 *	@param	initializedWithDefault whether the value is initialized with a default.
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_UNCHANGED
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	template<typename T>
	ACTIV_TEMPLATE_API(ACTIV_FRAMEWORK_API) StatusCode GetSignedIntegral(const std::string &prompt, T &value, const bool initializedWithDefault = false, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;

	/**
	 *	@brief	Get a signed integral of a given type within a range.
	 *
	 *	@param	prompt the prompt to display. Pass empty string to display no prompt.
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	value the value to read into, or the already initialized default.
	 *	@param	initializedWithDefault whether the value is initialized with a default.
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_UNCHANGED
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	template<typename T>
	ACTIV_TEMPLATE_API(ACTIV_FRAMEWORK_API) StatusCode GetSignedIntegral(const std::string &prompt, const int64_t lower, const int64_t upper, T &value, const bool initializedWithDefault = false, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;

	// explicit instantiations for unsigned types
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetSignedIntegral<int8_t>(const std::string &, int8_t &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetSignedIntegral<int16_t>(const std::string &, int16_t &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetSignedIntegral<int32_t>(const std::string &, int32_t &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetSignedIntegral<int64_t>(const std::string &, int64_t &, const bool, const TimeoutPeriod) const;)

	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetSignedIntegral<int8_t>(const std::string &, const int64_t, const int64_t, int8_t &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetSignedIntegral<int16_t>(const std::string &, const int64_t, const int64_t, int16_t &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetSignedIntegral<int32_t>(const std::string &, const int64_t, const int64_t, int32_t &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetSignedIntegral<int64_t>(const std::string &, const int64_t, const int64_t, int64_t &, const bool, const TimeoutPeriod) const;)

	/**
	 *	@brief	Get a field type of a given type.
	 *
	 *	@param	prompt the prompt to display. Pass empty string to display no prompt.
	 *	@param	value the value to read into, or the already initialized default.
	 *	@param	initializedWithDefault whether the value is initialized with a default.
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_UNCHANGED
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	template<typename T>
	ACTIV_TEMPLATE_API(ACTIV_FRAMEWORK_API) StatusCode GetFieldType(const std::string &prompt, T &value, const bool initializedWithDefault = false, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;

	// explicit instantiations for field types
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldType<BinaryArray>(const std::string &, BinaryArray &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldType<BinaryString>(const std::string &, BinaryString &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldType<Blob>(const std::string &, Blob &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldType<CrcBlob>(const std::string &, CrcBlob &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldType<Date>(const std::string &, Date &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldType<DateTime>(const std::string &, DateTime &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldType<Rational>(const std::string &, Rational &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldType<SInt>(const std::string &, SInt &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldType<TextArray>(const std::string &, TextArray &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldType<TextString>(const std::string &, TextString &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldType<Time>(const std::string &, Time &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldType<TRational>(const std::string &, TRational &, const bool, const TimeoutPeriod) const;)
	ACTIV_DECLARE_TEMPLATE_INSTANTIATION(StatusCode GetFieldType<UInt>(const std::string &, UInt &, const bool, const TimeoutPeriod) const;)

	/**
	 *	@brief	Get a system address.
	 *
	 *	@param	prompt the prompt to display. Pass empty string to display no prompt.
	 *	@param	address the address to read into, or the already initialized default.
	 *	@param	initializedWithDefault whether the address is initialized with a default.
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_UNCHANGED
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	ACTIV_FRAMEWORK_API StatusCode GetAddress(const std::string &prompt, Address &address, const bool initializedWithDefault = false, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;

	/**
	 *	@brief	Get confirmation (Y/y or N/n) from user.
	 *
	 *	@param	prompt the prompt to display. Pass empty string to display default prompt.
	 *	@param	positiveDefault should the default be positive ('y') or negative ('n')?
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS user entered 'y', 'Y'
	 *	@retval	STATUS_CODE_FAILURE user entered 'n', 'N'
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	ACTIV_FRAMEWORK_API StatusCode GetConfirmation(const std::string &prompt, const bool positiveDefault = false, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;

	/**
	 *	@brief	Get confirmation. The user needs to enter a random confirmation response.
	 *
	 *	@param	prompt the prompt to display. Pass empty string to display default prompt.
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS user confirmed
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	ACTIV_FRAMEWORK_API StatusCode GetRandomConfirmation(const std::string &prompt, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;

	/**
	 *	@brief	Toggle a boolean.
	 *
	 *	@param	prompt the prompt to display. Pass empty string to display default prompt.
	 *	@param	option reference to a boolean to be toggled.
	 *	@param	positiveDefault should the default be positive ('y') or negative ('n')?
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS user changed the value of \a option
	 *	@retval	STATUS_CODE_FAILURE user did not change the value of \a option
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	ACTIV_FRAMEWORK_API StatusCode EnableDisable(const std::string &prompt, bool &option, const bool positiveDefault = false, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;

	/**
	 *	@brief	Get ip address.
	 *
	 *	@param	prompt the prompt to display. Pass empty string to display no prompt.
	 *	@param	ipAddress the address to read into, or the already initialized default.
	 *	@param	initializedWithDefault whether the address is initialized with a default.
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_UNCHANGED
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	ACTIV_FRAMEWORK_API StatusCode GetIpAddress(const std::string &prompt, IpAddress &ipAddress, const bool initializedWithDefault = false, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;

	/**
	 *	@brief	Get address of UiUser.
	 *
	 *	@return	UiUser address.
	 */
	ACTIV_FRAMEWORK_API const Address &GetUiUserAddress() const;

private:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	uiMenu current menu.
	 *	@param	uiUser current user of \a uiMenu.
	 */
	UiIo(UiMenu &uiMenu, UiUser &uiUser);

	/**
	 *	@brief	Get an unsigned integral within a range.
	 *
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	value the value to read into, or the already initialized default.
	 *	@param	initializedWithDefault whether the value is initialized with a default.
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_UNCHANGED
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	StatusCode GetUnsignedIntegralInternal(const uint64_t lower, const uint64_t upper, uint64_t &value, const bool initializedWithDefault, const TimeoutPeriod timeout) const;

	/**
	 *	@brief	Get a signed integral within a range.
	 *
	 *	@param	lower the lower bound value.
	 *	@param	upper the upper bound value.
	 *	@param	value the value to read into, or the already initialized default.
	 *	@param	initializedWithDefault whether the value is initialized with a default.
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_UNCHANGED
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	StatusCode GetSignedIntegralInternal(const int64_t lower, const int64_t upper, int64_t &value, const bool initializedWithDefault, const TimeoutPeriod timeout) const;

	UiMenu	&m_uiMenu;											///< current menu
	UiUser	&m_uiUser;											///< current user

	friend class UiMenu;										///< allow UiMenu to construct UiIo objects
	friend class UiMainComponent;								///< main component needs access to user
};

} // namespace Activ

#endif // !defined (ACTIV_FRAMEWORK_UI_IO_H)
