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
#include "boost/bind.hpp"

#include <map>

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
	typedef std::map<std::string, std::string> CompletionList;
	typedef std::vector<CompletionList::const_iterator> CompletionIteratorList;

	class StaticCompletionHelper;								///< forward reference
	class DynamicCompletionHelper;								///< forward reference

	/**
	 *	@brief	Base class to handle tab-completion in the ui.
	 *
	 *	You probably won't need to use this class directly - instead see StaticCompletionHelper and DynamicCompletionHelper.
	 *	Tab-completion is available via a UiIo::GetString() overload.
	 */
	class CompletionHelper
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	isCaseSensitive whether to do case-sensitive tab-completion.
		 */
		ACTIV_FRAMEWORK_API CompletionHelper(const bool isCaseSensitive);

		/**
		 *	@brief	Destructor.
		 */
		ACTIV_FRAMEWORK_API virtual ~CompletionHelper();

		/**
		 *	@brief	Add a string to the list of possible completions.
		 *
		 *	@param	completion the string to add.
		 */
		ACTIV_FRAMEWORK_API void AddCompletion(const std::string &completion);

	private:
		/**
		 *	@brief	Populate the list of possible completions.
		 *
		 *	@param	partialInput current partial input the user has entered, that can optionally be used
		 *			to intelligently populate the list of possible completion strings.
		 */
		virtual void PopulateCompletionList(const std::string &partialInput) = 0;

		const bool		m_isCaseSensitive;						///< whether to do case-sensitive completions
		CompletionList	m_completionList;						///< list of completions

		friend class UiIo;
		friend class StaticCompletionHelper;
		friend class DynamicCompletionHelper;
	};

	/**
	 *	@brief	Class to handle tab-completion with a fixed list of strings.
	 */
	class StaticCompletionHelper : public CompletionHelper
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	begin iterator to the first string in a range of all possible completions.
		 *	@param	end iterator to beyond the last string in a range of all possible completions.
		 *	@param	isCaseSensitive whether to do case-sensitive tab-completion.
		 */
		template<typename T>
		StaticCompletionHelper(const T &begin, const T &end, const bool isCaseSensitive);

	private:
		ACTIV_FRAMEWORK_API virtual void PopulateCompletionList(const std::string &partialInput);
	};

	/**
	 *	@brief	Class to handle tab-completion with a dynamic list of strings that may be different each time tab is hit.
	 */
	class DynamicCompletionHelper : public CompletionHelper
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	isCaseSensitive whether to do case-sensitive tab-completion.
		 */
		ACTIV_FRAMEWORK_API DynamicCompletionHelper(const bool isCaseSensitive);

	protected:
		/**
		 *	@brief	Callback to populate list of possible completions from partial user input.
		 *
		 *	Override this to set the possible completions (via AddCompletion()) when the user hits tab.
		 *
		 *	@param	partialInput the user's partial input at the moment tab was hit.
		 */
		virtual void OnPopulateCompletionList(const std::string &partialInput) = 0;

	private:
		ACTIV_FRAMEWORK_API virtual void PopulateCompletionList(const std::string &partialInput);
	};

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
	 *	@brief	Get a string with tab-completion.
	 *
	 *	@param	prompt the prompt to display. Pass empty string to display no prompt.
	 *	@param	completionHelper reference to a CompletionHelper object to handle tab-completion.
	 *	@param	str the string to read into, or the already initialized default.
	 *	@param	initializedWithDefault whether the string is initialized with a default.
	 *	@param	timeout how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	STATUS_CODE_UNCHANGED
	 *	@retval	STATUS_CODE_TIMEOUT
	 */
	ACTIV_FRAMEWORK_API StatusCode GetString(const std::string &prompt, CompletionHelper &completionHelper, std::string &str, const bool initializedWithDefault = false, const TimeoutPeriod timeout = ACTIV_WAIT_INFINITE) const;

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

	/**
	 *	@brief	Display available completion choices.
	 *
	 *	@param	completionIteratorList list of iterators to CompletionList entries to display.
	 */
	void DisplayCompletionChoices(const CompletionIteratorList &completionIteratorList) const;

	UiMenu	&m_uiMenu;											///< current menu
	UiUser	&m_uiUser;											///< current user

	friend class UiMenu;										///< allow UiMenu to construct UiIo objects
	friend class UiMainComponent;								///< main component needs access to user
};

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
UiIo::StaticCompletionHelper::StaticCompletionHelper(const T &begin, const T &end, const bool isCaseSensitive) :
	CompletionHelper(isCaseSensitive)
{
	// can't get boost::mem_fn to work :-(
	for (T iter = begin; iter != end; ++iter)
		AddCompletion(*iter);
}

} // namespace Activ

#endif // !defined (ACTIV_FRAMEWORK_UI_IO_H)
