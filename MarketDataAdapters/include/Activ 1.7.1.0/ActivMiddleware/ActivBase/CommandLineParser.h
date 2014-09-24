/**
 *  @file	CommandLineParser.h
 *
 *  @brief	Header file for a command line parser class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_COMMAND_LINE_PARSER_H)
#define ACTIV_BASE_COMMAND_LINE_PARSER_H

#include "ActivMiddleware/ActivBase/Detail/CommandLineParserDetail.h"

namespace Activ
{

/**
 *	@brief	Command line parser class.
 *
 *	This is a wrapper over the more complete boost::program_options library.
 *
 *	A "help" switch (short form '?') is implictly added.
 */
class CommandLineParser
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_BASE_API CommandLineParser();

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_API virtual ~CommandLineParser();

	/**
	 *	@brief	Process a command line.
	 *
	 *	@param	argc as passed to main(),
	 *	@param	argc as passed to main().
	 *
	 *	@retval	true command line parsed.
	 *	@retval	false help switch was passed. PrintUsage() has been called.
	 *
	 *	@throw	Exception.
	 *	@throw	boost::program_options::error.
	 */
	ACTIV_BASE_API bool Process(const int argc, char *argv[]);

	/**
	 *	@brief	Print usage display.
	 *
	 *	@param	argv0 argv[0] from main().
	 *	@param	outStream iostream to print usage to.
	 */
	ACTIV_BASE_API virtual void PrintUsage(const std::string &argv0, std::ostream &outStream);

	/**
	 *	@brief	Get object to help with appending descriptions.
	 *
	 *	@return	DescriptionAppender helper object reference.
	 */
	ACTIV_BASE_API CommandLineParserDetail::DescriptionAppender &AddDescriptions();

	/**
	 *	@brief	Helper function to construct a mandatory parameter description with default store action.
	 *
	 *	@param	name name of the parameter.
	 *	@param	description descriptive text of the parameter.
	 *	@param	value location to store the parameter's value.
	 */
	template<typename T>
	static CommandLineParserDetail::MandatoryParameterDescription<T> Parameter(const std::string &name, const std::string &description, T &value)
	{
		return CommandLineParserDetail::MandatoryParameterDescription<T>(name, description, CommandLineParserDetail::ValueStore<T>(value));
	}

	/**
	 *	@brief	Helper function to construct a mandatory parameter description with arbitrary action.
	 *
	 *	@param	name name of the parameter.
	 *	@param	description descriptive text of the parameter.
	 *	@param	notifier function to invoke with the parameter's value.
	 */
	template<typename T>
	static CommandLineParserDetail::MandatoryParameterDescription<T> Parameter(const std::string &name, const std::string &description, boost::function<void (const T &)> notifier)
	{
		return CommandLineParserDetail::MandatoryParameterDescription<T>(name, description, notifier);
	}
	
	/**
	 *	@brief	Helper function to construct an optional parameter description with default value and store action.
	 *
	 *	@param	name name of the parameter.
	 *	@param	description descriptive text of the parameter.
	 *	@param	defaultValue the default value to assign for this parameter if none is provided.
	 *	@param	value location to store the parameter's value.
	 */
	template<typename T>
	static CommandLineParserDetail::OptionalParameterDescription<T> Parameter(const std::string &name, const std::string &description, const T &defaultValue, T &value)
	{
		return CommandLineParserDetail::OptionalParameterDescription<T>(name, description, defaultValue, CommandLineParserDetail::ValueStore<T>(value));
	}

	/**
	 *	@brief	Helper function to construct an optional parameter description with a default value and arbitrary action.
	 *
	 *	@param	name name of the parameter.
	 *	@param	description descriptive text of the parameter.
	 *	@param	defaultValue the default value to assign for this parameter if none is provided.
	 *	@param	notifier function to invoke with the parameter's value.
	 */
	template<typename T>
	static CommandLineParserDetail::OptionalParameterDescription<T> Parameter(const std::string &name, const std::string &description, const T &defaultValue, boost::function<void (const T &)> notifier)
	{
		return CommandLineParserDetail::OptionalParameterDescription<T>(name, description, defaultValue, notifier);
	}

	/**
	 *	@brief	Helper function to construct a mandatory switch description with default store action.
	 *
	 *	@param	name name of the switch.
	 *	@param	description descriptive text of the switch.
	 *	@param	value location to store the switch's value.
	 */
	template<typename T>
	static CommandLineParserDetail::MandatorySwitchDescription<T> Switch(const std::string &name, const std::string &description, T &value)
	{
		return CommandLineParserDetail::MandatorySwitchDescription<T>(name, description, CommandLineParserDetail::ValueStore<T>(value));
	}

	/**
	 *	@brief	Helper function to construct a mandatory switch description with arbitrary action.
	 *
	 *	@param	name name of the switch.
	 *	@param	description descriptive text of the switch.
	 *	@param	defaultValue the default value to assign for this switch if none is provided.
	 *	@param	notifier function to invoke with the switch's value.
	 */
	template<typename T>
	static CommandLineParserDetail::MandatorySwitchDescription<T> Switch(const std::string &name, const std::string &description, boost::function<void (const T &)> notifier)
	{
		return CommandLineParserDetail::MandatorySwitchDescription<T>(name, description, notifier);
	}

	/**
	 *	@brief	Helper function to construct an optional switch description with default value and default store action.
	 *
	 *	@param	name name of the switch.
	 *	@param	description descriptive text of the switch.
	 *	@param	defaultValue the default value to assign for this switch if none is provided.
	 *	@param	value location to store the switch's value.
	 */
	template<typename T>
	static CommandLineParserDetail::OptionalSwitchDescription<T> Switch(const std::string &name, const std::string &description, const T &defaultValue, T &value)
	{
		return CommandLineParserDetail::OptionalSwitchDescription<T>(name, description, defaultValue, CommandLineParserDetail::ValueStore<T>(value));
	}

	/**
	 *	@brief	Helper function to construct an optional switch description with default value and arbitrary action.
	 *
	 *	@param	name name of the switch.
	 *	@param	description descriptive text of the switch.
	 *	@param	defaultValue the default value to assign for this switch if none is provided.
	 *	@param	notifier function to invoke with the switch's value.
	 */
	template<typename T>
	static CommandLineParserDetail::OptionalSwitchDescription<T> Switch(const std::string &name, const std::string &description, const T &defaultValue, boost::function<void (const T &)> notifier)
	{
		return CommandLineParserDetail::OptionalSwitchDescription<T>(name, description, defaultValue, notifier);
	}

	/**
	 *	@brief	Helper function to construct a boolean switch description and default store action.
	 *
	 *	@param	name name of the switch.
	 *	@param	description descriptive text of the switch.
	 *	@param	notifier function to invoke with the switch's value.
	 */
	ACTIV_BASE_API static CommandLineParserDetail::BooleanSwitchDescription BooleanSwitch(const std::string &name, const std::string &description, bool &value);

	/**
	 *	@brief	Helper function to construct a boolean switch description and arbitrary action.
	 *
	 *	@param	name name of the switch.
	 *	@param	description descriptive text of the switch.
	 *	@param	notifier function to invoke with the switch's value.
	 */
	ACTIV_BASE_API static CommandLineParserDetail::BooleanSwitchDescription BooleanSwitch(const std::string &name, const std::string &description, boost::function<void (const bool &)> notifier);

private:
	CommandLineParserDetail::CommandLineParserImpl	m_commandLineParserImpl;
	CommandLineParserDetail::DescriptionAppender	m_descriptionAppender;
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_COMMAND_LINE_PARSER_H)
