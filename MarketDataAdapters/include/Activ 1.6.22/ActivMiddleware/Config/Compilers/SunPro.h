/**
 *	@file	SunPro.h
 *
 *	@brief	SunPro C++ configuration file.
 *
 *	$Log: $
 */

#if !defined (ACTIV_CONFIG_SUNPRO_H)
#define ACTIV_CONFIG_SUNPRO_H

#if (__SUNPRO_CC < 0x560)
#error SunPro version not supported
#endif

#define ACTIV_COMPILER_NAME	"SunPro C++ version " ACTIV_STRINGIZE(__SUNPRO_CC)

#define ACTIV_EXPORT_DLL
#define ACTIV_IMPORT_DLL
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
 *	The C++ standard requires using object.template TemplateFn<Type>(params ...); however gcc (as of 3.4)
 *	will not accept that and we need to use object.TemplateFn<Type>(params ...);
 *	ACTIV_TEMPLATE_FN abstracts this difference away.
 *
 *	So use object.ACTIV_TEMPLATE_FN TemplateFn<Type>(params ...);
 */
#define ACTIV_TEMPLATE_FN template

/**
 *	@brief	Declare a nested class.
 *
 *	Sun's compiler doesn't use the same rules as GCC & Visual C++ - a nested class won't automatically
 *	have unrestricted access to the parent class. Use this macro as a work around, e.g.
 *
 *	class Parent
 *	{
 *		ACTIV_DECLARE_NESTED_CLASS(Child) : public Base { };
 *	};
 *
 *	@param	c name of the class to declare.
 */
#define ACTIV_DECLARE_NESTED_CLASS(c)	\
	class c;							\
	friend class c;						\
	class c

#define _XPG4_2
#define __EXTENSIONS__
#define _POSIX_PTHREAD_SEMANTICS

#define __FUNCTION__	__FILE__ ":" ACTIV_STRINGIZE(__LINE__)	///< SunPro doesn't provide a __FUNCTION__ predefined macro, so fake one

#define stricmp			strcasecmp
#define strnicmp		strncasecmp

#endif // !defined (ACTIV_CONFIG_SUNPRO_H)
