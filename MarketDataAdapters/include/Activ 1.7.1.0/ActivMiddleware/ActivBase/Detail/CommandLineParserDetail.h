/**
 *  @file	CommandLineParserDetail.h
 *
 *  @brief  Implementation details for CommandLineParser.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_COMMAND_LINE_PARSER_DETAIL_H)
#define ACTIV_BASE_COMMAND_LINE_PARSER_DETAIL_H

#include "ActivMiddleware/ActivBase/External.h"
#include "ActivMiddleware/ActivBase/StatusCodeException.h"

#include "boost/program_options.hpp"

#include <iostream>

namespace Activ
{

namespace CommandLineParserDetail
{

/**
 *	@brief	Default action to take for a command line value.
 */
template<typename T>
class ValueStore
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	value where to store the value.
	 */
	ValueStore(T &value) :
		m_value(value)
	{
	}

	/**
	 *	@brief	Function operator.
	 *
	 *	@param	value value to store in the location provided to the constructor.
	 */
	void operator()(const T &value)
	{
		m_value = value;
	}

private:
	T &m_value;													///< location to store value from command line
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Description of a command line parameter/switch.
 */
template<typename T>
class Description
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	name name of the parameter/switch.
	 *	@param	description descriptive text of the parameter/switch.
	 *	@param	notifier action to take when parameter/switch is seen on the command line.
	 */
	Description(const std::string &name, const std::string &description, boost::function<void (const T &)> notifier) :
		m_name(name),
		m_description(description),
		m_notifier(notifier)
	{
	}

	const std::string						m_name;				///< name of the parameter/switch
	const std::string						m_description;		///< descriptive text of the parameter/switch
	const boost::function<void (const T &)>	m_notifier;			///< action to take when parameter/switch is seen on the command line
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Description of a command line parameter/switch with default value.
 */
template<typename T>
class DefaultDescription : public Description<T>
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	name name of the parameter/switch.
	 *	@param	description descriptive text of the parameter/switch.
	 *	@param	defaultValue the default value to use if not provided on the command line.
	 *	@param	notifier action to take when parameter/switch is seen on the command line.
	 */
	DefaultDescription(const std::string &name, const std::string &description, const T &defaultValue, boost::function<void (const T &)> notifier) :
		Description<T>(name, description, notifier),
		m_defaultValue(defaultValue)
	{
	}

	const T	m_defaultValue;										///< the default value to use if not provided on the command line
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Description of a mandatory command line parameter.
 */
template<typename T>
class MandatoryParameterDescription : public Description<T>
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	name name of the parameter.
	 *	@param	description descriptive text of the parameter.
	 *	@param	notifier action to take when parameter is seen on the command line.
	 */
	MandatoryParameterDescription(const std::string &name, const std::string &description, boost::function<void (const T &)> notifier) :
		Description<T>(name, description, notifier)
	{
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Description of an optional command line parameter.
 */
template<typename T>
class OptionalParameterDescription : public DefaultDescription<T>
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	name name of the parameter.
	 *	@param	description descriptive text of the parameter.
	 *	@param	defaultValue the default value to use if not provided on the command line.
	 *	@param	notifier action to take when parameter is seen on the command line.
	 */
	OptionalParameterDescription(const std::string &name, const std::string &description, const T &defaultValue, boost::function<void (const T &)> notifier) :
		DefaultDescription<T>(name, description, defaultValue, notifier)
	{
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Description of a mandatory command line switch.
 */
template<typename T>
class MandatorySwitchDescription : public Description<T>
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	name name of the switch.
	 *	@param	description descriptive text of the switch.
	 *	@param	defaultValue the default value to use if not provided on the command line.
	 *	@param	notifier action to take when switch is seen on the command line.
	 */
	MandatorySwitchDescription(const std::string &name, const std::string &description, boost::function<void (const T &)> notifier) :
		Description<T>(name, description, notifier)
	{
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Description of an optional command line switch.
 */
template<typename T>
class OptionalSwitchDescription : public DefaultDescription<T>
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	name name of the switch.
	 *	@param	description descriptive text of the switch.
	 *	@param	defaultValue the default value to use if not provided on the command line.
	 *	@param	notifier action to take when switch is seen on the command line.
	 */
	OptionalSwitchDescription(const std::string &name, const std::string &description, const T &defaultValue, boost::function<void (const T &)> notifier) :
		DefaultDescription<T>(name, description, defaultValue, notifier)
	{
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Description of a boolean command line switch.
 */
class BooleanSwitchDescription : public Description<bool>
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	name name of the switch.
	 *	@param	description descriptive text of the switch.
	 *	@param	notifier action to take when switch is seen on the command line.
	 */
	BooleanSwitchDescription(const std::string &name, const std::string &description, boost::function<void (const bool &)> notifier) :
		Description<bool>(name, description, notifier)
	{
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

class DescriptionAppender;

/**
 *	@brief	Implementation details of CommandLineParser.
 */
class CommandLineParserImpl
{
public:
	/**
	 *	@brief	Constructor.
	 */
	CommandLineParserImpl();

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
	bool Process(const int argc, char *argv[]);

	/**
	 *	@brief	Print usage display.
	 *
	 *	@param	argv0 argv[0] from main().
	 *	@param	outStream iostream to print usage to.
	 */
	void PrintUsage(const std::string &argv0, std::ostream &outStream);

private:
	/**
	 *	@brief	Get the program name.
	 *
	 *	@brief	argv0 argv[0] as provided to main().
	 *
	 *	@return	The program name.
	 */
	static std::string GetProgramName(const std::string &argv0);

	boost::program_options::options_description				m_mandatoryParameterDescriptions;
	boost::program_options::options_description				m_optionalParameterDescriptions;
	boost::program_options::positional_options_description	m_positionalDescriptions;

	boost::program_options::options_description				m_mandatorySwitchDescriptions;
	boost::program_options::options_description				m_optionalSwitchDescriptions;

	friend class DescriptionAppender;
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Helper class to build description.
 */
class DescriptionAppender
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	commandLineParserImpl CommandLineParserImpl to append descriptions to.
	 */
	DescriptionAppender(CommandLineParserImpl &commandLineParserImpl);

	/**
	 *	@brief	Append a mandatory parameter description.
	 *
	 *	@param	parameterDescription ParameterDescription to append.
	 *
	 *	@return	*this.
	 */
	template<typename T>
	DescriptionAppender &operator()(const MandatoryParameterDescription<T> &parameterDescription)
	{
		if (parameterDescription.m_name.empty())
			ACTIV_THROW(StatusCodeException, STATUS_CODE_INVALID_FORMAT);

		m_commandLineParserImpl.m_mandatoryParameterDescriptions.add_options()
			(
				parameterDescription.m_name.c_str(),
				boost::program_options::value<T>()->notifier(parameterDescription.m_notifier),
				parameterDescription.m_description.c_str()
			);

		m_commandLineParserImpl.m_positionalDescriptions.add(parameterDescription.m_name.c_str(), 1);

		return *this;
	}

	/**
	 *	@brief	Append a mandatory parameter list description.
	 *
	 *	@param	parameterDescription ParameterDescription to append.
	 *
	 *	@return	*this.
	 */
	template<typename T>
	DescriptionAppender &operator()(const MandatoryParameterDescription<std::vector<T> > &parameterDescription)
	{
		if (parameterDescription.m_name.empty())
			ACTIV_THROW(StatusCodeException, STATUS_CODE_INVALID_FORMAT);

		m_commandLineParserImpl.m_mandatoryParameterDescriptions.add_options()
			(
				parameterDescription.m_name.c_str(),
				boost::program_options::value<std::vector<T> >()->composing()->notifier(parameterDescription.m_notifier),
				parameterDescription.m_description.c_str()
			);

		m_commandLineParserImpl.m_positionalDescriptions.add(parameterDescription.m_name.c_str(), -1);

		return *this;
	}

	/**
	 *	@brief	Append an optional parameter description.
	 *
	 *	@param	parameterDescription OptionalParameterDescription to append.
	 *
	 *	@return	*this.
	 */
	template<typename T>
	DescriptionAppender &operator()(const OptionalParameterDescription<T> &parameterDescription)
	{
		if (parameterDescription.m_name.empty())
			ACTIV_THROW(StatusCodeException, STATUS_CODE_INVALID_FORMAT);

		// disallow short options
		if (std::string::npos != parameterDescription.m_name.find_first_of(','))
			ACTIV_THROW(StatusCodeException, STATUS_CODE_INVALID_FORMAT);

		m_commandLineParserImpl.m_optionalParameterDescriptions.add_options()
			(
				parameterDescription.m_name.c_str(),
				boost::program_options::value<T>()->default_value(parameterDescription.m_defaultValue)->notifier(parameterDescription.m_notifier),
				parameterDescription.m_description.c_str()
			);

		m_commandLineParserImpl.m_positionalDescriptions.add(parameterDescription.m_name.c_str(), 1);

		return *this;
	}

	/**
	 *	@brief	Append an optional parameter list description.
	 *
	 *	@param	parameterDescription OptionalParameterDescription to append.
	 *
	 *	@return	*this.
	 */
	template<typename T>
	DescriptionAppender &operator()(const OptionalParameterDescription<std::vector<T> > &parameterDescription)
	{
		if (parameterDescription.m_name.empty())
			ACTIV_THROW(StatusCodeException, STATUS_CODE_INVALID_FORMAT);

		// disallow short options
		if (std::string::npos != parameterDescription.m_name.find_first_of(','))
			ACTIV_THROW(StatusCodeException, STATUS_CODE_INVALID_FORMAT);

		m_commandLineParserImpl.m_optionalParameterDescriptions.add_options()
			(
				parameterDescription.m_name.c_str(),
				boost::program_options::value<std::vector<T> >()->default_value(parameterDescription.m_defaultValue, "")->composing()->notifier(parameterDescription.m_notifier),
				parameterDescription.m_description.c_str()
			);

		m_commandLineParserImpl.m_positionalDescriptions.add(parameterDescription.m_name.c_str(), -1);

		return *this;
	}

	/**
	 *	@brief	Append a mandatory switch description.
	 *
	 *	@param	switchDescription MandatorySwitchDescription to append.
	 *
	 *	@return	*this.
	 */
	template<typename T>
	DescriptionAppender &operator()(const MandatorySwitchDescription<T> &switchDescription)
	{
		m_commandLineParserImpl.m_mandatorySwitchDescriptions.add_options()
			(
				switchDescription.m_name.c_str(),
				boost::program_options::value<T>()->notifier(switchDescription.m_notifier),
				switchDescription.m_description.c_str()
			);

		return *this;
	}

	/**
	 *	@brief	Append a mandatory switch list description.
	 *
	 *	@param	switchDescription MandatorySwitchDescription to append.
	 *
	 *	@return	*this.
	 */
	template<typename T>
	DescriptionAppender &operator()(const MandatorySwitchDescription<std::vector<T> > &switchDescription)
	{
		m_commandLineParserImpl.m_mandatorySwitchDescriptions.add_options()
			(
				switchDescription.m_name.c_str(),
				boost::program_options::value<std::vector<T> >()->composing()->notifier(switchDescription.m_notifier),
				switchDescription.m_description.c_str()
			);

		return *this;
	}

	/**
	 *	@brief	Append an optional switch description with default value.
	 *
	 *	@param	optionDescription OptionDescription to append.
	 *
	 *	@return	*this.
	 */
	template<typename T>
	DescriptionAppender &operator()(const OptionalSwitchDescription<T> &switchDescription)
	{
		m_commandLineParserImpl.m_optionalSwitchDescriptions.add_options()
			(
				switchDescription.m_name.c_str(),
				boost::program_options::value<T>()->default_value(switchDescription.m_defaultValue)->notifier(switchDescription.m_notifier),
				switchDescription.m_description.c_str()
			);

		return *this;
	}

	/**
	 *	@brief	Append an optional switch list description with default value.
	 *
	 *	@param	optionDescription OptionDescription to append.
	 *
	 *	@return	*this.
	 */
	template<typename T>
	DescriptionAppender &operator()(const OptionalSwitchDescription<std::vector<T> > &switchDescription)
	{
		m_commandLineParserImpl.m_optionalSwitchDescriptions.add_options()
			(
				switchDescription.m_name.c_str(),
				boost::program_options::value<std::vector<T> >()->default_value(switchDescription.m_defaultValue, "")->composing()->notifier(switchDescription.m_notifier),
				switchDescription.m_description.c_str()
			);

		return *this;
	}

	/**
	 *	@brief	Append a boolean switch description.
	 *
	 *	@param	booleanSwitchDescription BooleanSwitchDescription to append.
	 *
	 *	@return	*this.
	 */
	DescriptionAppender &operator()(const BooleanSwitchDescription &booleanSwitchDescription)
	{
		m_commandLineParserImpl.m_optionalSwitchDescriptions.add_options()
			(
				booleanSwitchDescription.m_name.c_str(),
				boost::program_options::bool_switch()->notifier(booleanSwitchDescription.m_notifier),
				booleanSwitchDescription.m_description.c_str()
			);

		return *this;
	}

private:
	CommandLineParserImpl &	m_commandLineParserImpl;
};

} // namespace CommandLineParserDetail

} // namespace Activ

#endif // (!defined ACTIV_BASE_COMMAND_LINE_PARSER_DETAIL_H)
