/**
 *	@file	Intel.h
 *
 *	@brief	Intel C++ Compiler configuration file.
 *
 *	$Log: $
 */

#if (!defined ACTIV_CONFIG_INTEL_H)
#define ACTIV_CONFIG_INTEL_H

#define ACTIV_COMPILER_NAME "Intel C++ Compiler version " ACTIV_STRINGIZE(__INTEL_COMPILER)

#define ACTIV_EXPORT_DLL	__declspec (dllexport)
#define ACTIV_IMPORT_DLL	__declspec (dllimport)
#define ACTIV_EXPORT_LIB
#define ACTIV_IMPORT_LIB

/**
 *	@brief	Define rules for exporting template member functions.
 *
 *	The Intel & Visual C++ compilers have different rules for __declspec exporting template
 *	instantiations from dlls. Use ACTIV_TEMPLATE_API as a workaround, e.g.
 *
 *	class Example
 *	{
 *		template <typename T>
 *		ACTIV_TEMPLATE_API(MY_LIB_API) void Fn(T &);
 *	};
 *
 *	@param	a __declspec macro, e.g. ACTIV_BASE_API, ACTIV_MESSAGE_HANDLER_API, MY_LIB_API.
 */
#define ACTIV_TEMPLATE_API(a)

#define snprintf	_snprintf
#define vsnprintf	_vsnprintf

// "C4355: 'this' : used in base member initializer list"
#pragma warning(disable:4355)

#endif // (!defined ACTIV_CONFIG_INTEL_H)
