/**
 *	@file	SunPro.h
 *
 *	@brief	SunPro C++ configuration file.
 *
 *	$Log: $
 */

#if (!defined ACTIV_CONFIG_SUNPRO_H)
#define ACTIV_CONFIG_SUNPRO_H

#if (__SUNPRO_CC < 0x580)
#error SunPro version not supported
#endif

#define ACTIV_COMPILER_NAME	"SunPro C++ version " ACTIV_STRINGIZE(__SUNPRO_CC)

#define ACTIV_EXPORT_DLL
#define ACTIV_IMPORT_DLL
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

/**
 *	@brief	Declare a type in a template parameter list.
 *
 *	When using the curiously recurring template pattern, e.g.
 *
 *		template<...> class Derived : public Base<Derived, ...>
 *
 *	under GCC requires
 *
 *		template<...> class Derived : public Base<class Derived, ...>
 *
 *	but Visual C++ won't accept that (it will accept typename). Use this macro as a workaround, e.g.
 *
 *		template<...> class Derived : public Base<ACTIV_TEMPLATE_SELF Derived, ...>
 */
#define ACTIV_TEMPLATE_SELF	typename

/**
 *	@brief	Additional specifier required when accessing template member function.
 *
 *	The C++ standard requires using object.template TemplateFn<Type>(params ...); however GCC (as of 3.4)
 *	will not accept that and we need to use object.TemplateFn<Type>(params ...);
 *	ACTIV_TEMPLATE_FN abstracts this difference away.
 *
 *	So use object.ACTIV_TEMPLATE_FN TemplateFn<Type>(params ...);
 */
#define ACTIV_TEMPLATE_FN

#define _XPG4_2
#define __EXTENSIONS__
#define _POSIX_PTHREAD_SEMANTICS

#define __FUNCTION__	__FILE__ ":" ACTIV_STRINGIZE(__LINE__)	///< SunPro doesn't provide a __FUNCTION__ predefined macro, so fake one

#define stricmp			strcasecmp
#define strnicmp		strncasecmp

#endif // (!defined ACTIV_CONFIG_SUNPRO_H)
