/**
 *	@file	Url.h
 *
 *	@brief	Url parser & builder class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_URL_H)
#define ACTIV_BASE_URL_H

#include "ActivMiddleware/ActivBase/External.h"

#include <string>
#include <list>

namespace Activ
{

/**
 *	@brief	Url parser & builder class.
 *
 *	A Url is of the form protocol://location/path?parameter?parameter
 *	parameter is of the form key=value
 *	Other accepted parameter delimiters in addition to ? are , ; &
 *	Another accepted key/value delimiter is :
 */
class Url
{
public:
	typedef std::pair<std::string, std::string> Parameter;		///< parameter type (key, value)
	typedef std::list<Parameter> ParameterList;					///< parameter list type

	/**
	 *	@brief	Parse a url into the class members.
	 *
	 *	@param	url the url to parse.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 */
	ACTIV_BASE_API StatusCode Parse(const std::string &url);

	/**
	 *	@brief	Build a url from the component parts.
	 */
	ACTIV_BASE_API std::string Build() const;

	std::string		m_protocol;									///< protocol portion of the url
	std::string		m_location;									///< location portion of the url
	std::string		m_path;										///< path portion of the url
	ParameterList	m_parameterList;							///< parameter list portion of the url

private:
	/**
	 *	@brief	Parse an optional parameter.
	 *
	 *	@param	parameterString a string containing the parameter.
	 *	@param	parameterList the list to append a Parameter object to containing the parsed key and value.
	 */
	static void ParseParameter(const std::string &parameterString, ParameterList &parameterList);
};

} // namespace Activ

#endif // (!defined ACTIV_BASE_URL_H)
