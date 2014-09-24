/**
 *	@file	VisualC.h
 *
 *	@brief	Microsoft Visual C++ configuration file.
 *
 *	$Log: $
 */

#if (!defined ACTIV_CONFIG_VISUAL_C_H)
#define ACTIV_CONFIG_VISUAL_C_H

#if (_MSC_VER < 1310)
#error VC++ version not supported
#endif

#define ACTIV_COMPILER_NAME "Microsoft Visual C++ version " ACTIV_STRINGIZE(_MSC_VER)

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
#define ACTIV_TEMPLATE_API(a)	a

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
#define ACTIV_TEMPLATE_FN template

#define snprintf	_snprintf
#define vsnprintf	_vsnprintf

// turn off annoying "C4355: 'this' : used in base member initializer list" warning
#pragma warning(disable:4355)

// turn off annoying "C4163: not available as an intrinsic function" warning
#pragma warning(disable:4163)

// turn off annoying "C4996: : 'blah' was declared deprecated" warning
#pragma warning(disable:4996)

#if (_MSC_VER >= 1400)
#if (!defined _SCL_SECURE_NO_DEPRECATE)
#define _SCL_SECURE_NO_DEPRECATE
#endif

#if (defined _SECURE_SCL)
#undef _SECURE_SCL
#endif
#define _SECURE_SCL	0

#if (!defined _CRT_SECURE_NO_DEPRECATE)
#define _CRT_SECURE_NO_DEPRECATE
#endif
#endif

#endif // (!defined ACTIV_CONFIG_VISUAL_C_H)
