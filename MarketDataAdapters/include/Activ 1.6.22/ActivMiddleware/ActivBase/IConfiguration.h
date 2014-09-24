/**
 *  @file	 IConfiguration.h
 *
 *  @brief   Header file for abstract configuration interface
 *
 *	$Log: $
 */

#if !defined (ACTIV_ICONFIGURATION_H)
#define ACTIV_ICONFIGURATION_H

#include "ActivMiddleware/ActivBase/External.h"

namespace Activ
{

/**
 *  @brief   Abstract configure class.
 */
class IConfiguration
{
public:
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
	 *	@brief	destructor.
     */
	virtual ~IConfiguration() = 0;

private:
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
	virtual bool OnProcessParse(const size_t parseIndex, const void *pParseData, const size_t dataLength,
								const ParseDataFormat dataFormat) = 0;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline IConfiguration::~IConfiguration()
{
}

}; // namespace Activ

#endif // !defined (ACTIV_ICONFIGURATION_H)
