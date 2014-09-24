/**
 *	@file	Intel.h
 *
 *	@brief	Intel C++ Compiler configuration file.
 *
 *	$Log: $
 */

#if !defined (ACTIV_CONFIG_INTEL_H)
#define ACTIV_CONFIG_INTEL_H

#define ACTIV_COMPILER_NAME "Intel C++ Compiler version " ACTIV_STRINGIZE(__INTEL_COMPILER)

#define ACTIV_EXPORT_DLL	__declspec (dllexport)
#define ACTIV_IMPORT_DLL	__declspec (dllimport)
#define ACTIV_EXPORT_LIB
#define ACTIV_IMPORT_LIB

/**
 *	@brief	Define rules for explicit template instantiations.
 *
 *	With Visual C++ 7.0, explicit instantiations of member function templates has to be done
 *	inline in the class definition. With 7.1 (and GCC, SunPro), the explicit instantiation
 *	needs to be outside the class definition (and is not allowed inline in the case of GCC and VC).
 *
 *	Use ACTIV_DECLARE_TEMPLATE_INSTANTIATION to declare an explicit instantiation in the class
 *	definition as a workaround, e.g.
 *
 *	class Example
 *	{
 *		template <typename T>
 *		ACTIV_TEMPLATE_API(MY_LIB_API) void Fn(T &);
 *
 *		ACTIV_DECLARE_TEMPLATE_INSTANTIATION(void Fn<bool>(bool &);)
 *	};
 *
 *	And outside the class definition (in a .cpp, presumably):
 *
 *		template MY_LIB_API void Exmaple::Fn<bool>(bool &);
 */
#define ACTIV_DECLARE_TEMPLATE_INSTANTIATION(t)

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

#endif // !defined (ACTIV_CONFIG_INTEL_H)
