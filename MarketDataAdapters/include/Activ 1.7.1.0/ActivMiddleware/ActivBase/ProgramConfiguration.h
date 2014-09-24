/**
 *  @file	ProgramConfiguration.h
 *
 *  @brief  A header file for a program configuration class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_PROGRAM_CONFIGURATION_H)
#define ACTIV_BASE_PROGRAM_CONFIGURATION_H

#include "ActivMiddleware/ActivBase/External.h"

#include <iostream>

namespace Activ
{

/**
 *	@brief	Declare a ProgramConfiguration::ArgumentSpecification entry from an argument list entry.
 */
#define ACTIV_DECLARE_PROGRAM_CONFIGURATION_ARGUMENT_SPECIFICATION_ENTRY(index, type, name, alias, description, defaultValue)	\
	{	type, name, alias, description, defaultValue	},

/**
 *	@brief	Declare an argument index from an argument list entry.
 */
#define ACTIV_DECLARE_PROGRAM_CONFIGURATION_ARGUMENT_INDEX(index, type, name, alias, description, defaultValue)	index,

/**
 *	@brief	Declare a ProgramConfiguration::ArgumentSpecification array and index enumeration from
 *			an argument list.
 *
 *	@param	className the name of the class which is to contain the array.
 *	@param	memberName the member name of the array.
 *	@param	argumentList the argument list.
 */
#define ACTIV_PROGRAM_CONFIGURATION_ARGUMENT_SPECIFICATION(className, memberName, argumentList)									\
	Activ::ProgramConfiguration::ArgumentSpecification className::memberName[] =												\
		{ argumentList(ACTIV_DECLARE_PROGRAM_CONFIGURATION_ARGUMENT_SPECIFICATION_ENTRY) };										\
	enum																														\
	{																															\
		argumentList(ACTIV_DECLARE_PROGRAM_CONFIGURATION_ARGUMENT_INDEX)														\
	}

/**
 *  @brief	Program Configuration class.
 */
class ProgramConfiguration
{
public:
	/**
	 *	@brief	Argument types.
	 */
	enum ArgumentType
	{
		PARAMETER,
		OPTION
	};

	/**
	 *	@brief	Parse data formats.
	 */
	enum ParseDataFormat
	{
		FORMAT_NOT_SUPPORTED,
		FORMAT_STRING,
		FORMAT_BINARY,
		FORMAT_DWORD,
		FORMAT_MULTI_STRING,
		FORMAT_EXPANDABLE_STRING
	};

	/**
	 *  @brief	Information about allowed command line arguments.
	 */
	struct ArgumentSpecification
	{
		ArgumentType	type;
		const char		*pName;
		const char		*pAliasName;
		const char		*pDescription;
		const char		*pDefaultValue;
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	numArgSpecs the number of argument specifications.
	 *	@param	pArgSpecs pointer to an array of ArgumentSpecification of size \a numArgSpecs.
	 */
	ACTIV_BASE_API ProgramConfiguration(const size_t numArgSpecs, const ArgumentSpecification *pArgSpecs);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_API virtual ~ProgramConfiguration();

	/**
	 *	@brief	Parse the program and argument information.
	 *
	 *	@param	numArgs argument count.
	 *	@param	pArgList argument list.
	 *
	 *	@throw	StatusCodeException
	 *	@throw	OsException
	 */
	ACTIV_BASE_API void Parse(const size_t numArgs, char **pArgList);

    /**
	 *	@brief	Print usage.
	 *
	 *  @param	pArgZero pointer to argment with program name.
	 *	@param	outStream output stream to dump to. Default is standard out.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
     */
	ACTIV_BASE_API virtual StatusCode PrintUsage(const char *pArgZero, std::ostream &outStream = std::cout);
	
	/**
     *  @brief	Get the executable file name.
	 *
	 *  @param	pArgZero pointer to argment with program name
	 *	@param	pFileName return buffer for execution file name
	 *	@param	maxLength length of return buffer for execution file name
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
     */
	ACTIV_BASE_API virtual StatusCode GetExecutionFileName(const char *pArgZero, char *pFileName, const size_t maxLength);

	/**
     *  @brief	Process the command line.
	 *
	 *	@param	numArgs argument count.
	 *	@param	pArgList argument list.
	 *
	 *	@return	whether execution should be continued.
     */
	ACTIV_BASE_API virtual bool Process(const size_t numArgs, char **pArgList);

protected:
    /**
	 *	@brief	Callback for a parsed item.
	 *
	 *  @param	parseIndex index into parse specification table.
	 *	@param	pParseData return buffer for parse data.
	 *  @param	dataLength data length.
	 *  @param	dataFormat enumeration of ParseDataFormat.
	 *
	 *	@retval	true to continue.
     */
	ACTIV_BASE_API virtual bool OnProcessParse(const size_t parseIndex, const void *pParseData, const size_t dataLength,
											   const ParseDataFormat dataFormat);

private:
	/**
	 *	@brief	Validate arguments.
	 *
	 *	@param	numArgs argument count.
	 *	@param	pArgList argument list.
	 *
	 *	@throw	Exception.
	 */	
	void ValidateArguments(const size_t numArgs, char **pArgList) const;

	/**
	 *	@brief	Invoke OnProcessParse() callback for arguments.
	 *
	 *	@param	numArgs argument count.
	 *	@param	pArgList argument list.
	 *
	 *	@throw	Exception.
	 */	
	void DistributeArguments(const size_t numArgs, char **pArgList);

	/**
	 *	@brief	Get argument pointer.
	 *
	 *	@param	argSpecIndex index of the argument specifier.
	 *	@param	pArg pointer to the current argument being processed.
	 *	@param	pParseData pointer to a buffer to receive the argument.
	 *	@param	dataLength reference to a variable to receive the argument length.
	 */
	void GetArgumentPointer(const size_t argSpecIndex, const char *pArg, const void * &pParseData, size_t &dataLength) const;

	/**
	 *	@brief	Find an argument specifaction by name.
	 *
	 *	@param	pName the name of the argument.
	 *	@param	specIndex reference to a variable to receive the index of the argument specifier matching \a pName.
	 *
	 *	@return	true if the argument is found, else false.
	 */
	bool FindArgumentSpecificationByName(const char *pName, size_t &specIndex) const;

	/**
	 *	@brief	Find parameter specification.
	 *
	 *	@param	parameterNumber
	 *	@param	argSpecIndex reference to a variable to receive the index of the argument specifier.
	 *
	 *	@return	true if the parameter is found, else false.
	 */
	bool FindParameterSpecification(const size_t parameterNumber, size_t &argSpecIndex) const;

	/**
	 *	@brief	Validate parameter number.
	 *
	 *	@param	argSpecIndex the index of the argument specifier.
	 *	@param	expectedParameterNumber the expected parameter number.
	 *
	 *	@return	true if the parameter number matches, else false.
	 */
	bool IsValidParameterNumber(const size_t argSpecIndex, const size_t expectedParameterNumber) const;

	/**
	 *	@brief	Get short specification name.
	 *
	 *	@param	argSpecIndex the index of the argument specification.
	 *	@param	parameterNumber the parameter number.
	 *	@param	pName pointer to a buffer to receive the name.
	 *	@param	maxLength the size of the buffer pointer to by \a pName.
	 */
	void GetShortSpecificationName(const size_t argSpecIndex, const size_t parameterNumber,
								   char *pName, const size_t maxLength) const;

	/**
	 *	@brief	Get argument name.
	 *
	 *	@param	argSpecIndex the index of the argument specification.
	 *	@param	parameterNumber the parameter number.
	 *	@param	pName pointer to a buffer to receive the name.
	 *	@param	maxLength the size of the buffer pointer to by \a pName.
	 *
	 *	@param	return pName.
	 */
	const char *GetArgumentName(const size_t argSpecIndex, const size_t parameterNumber,
								char *pName, const size_t nameLength) const;

	/**
	 *	@brief	Get alias name of argument.
	 *
	 *	@param	argSpecIndex the index of the argument specification.
	 *	@param	pName pointer to a buffer to receive the name.
	 *	@param	maxLength the size of the buffer pointer to by \a pName.
	 *
	 *	@param	return pName.
	 */
	const char *GetArgumentAliasName(const size_t argSpecIndex, char *pName, const size_t maxLength) const;

	/**
	 *	@brief	Extract argument name.
	 *
	 *	@param	pArg pointer to the current argument being processed.
	 *	@param	pName pointer to a buffer to receive the name.
	 *	@param	maxLength the size of the buffer pointed to by \a pName.
	 *
	 *	@retval	true if the argument is valid.
	 *	@retval	false if the argument is invalid.
	 */
	static bool ExtractArgumentName(const char *pArg, char *pName, const size_t maxLength);

	/**
	 *	@brief	Get the program name.
	 *
	 *	@brief	pExecutionFileName the execution string.
	 *	@brief	pProgramName pointer to a buffer to receive the name.
	 *	@param	nameLength the size of the buffer pointer to by \a pName.
	 */
	static void GetProgramName(const char *pExecutionFileName, char *pProgramName, const size_t nameLength);

	/**
	 *	@brief	Is the argument a help request?
	 *
	 *	@param	pArg the argument being processed.
	 *
	 *	@return	true if help should be displayed, else false.
	 */
	static bool IsHelpRequest(const char *pArg);

	/**
	 *	@brief	Is the argument optional?
	 *
	 *	@param	pArg the argument being processed.
	 *
	 *	@return	true if optional, else false.
	 */
	static bool IsOptional(const char *pArg);

	const size_t						m_numArgSpecs;		///< The number of argument specifiers
	const ArgumentSpecification * const	m_pArgSpecs;		///< The argument specifier array
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_PROGRAM_CONFIGURATION_H)
