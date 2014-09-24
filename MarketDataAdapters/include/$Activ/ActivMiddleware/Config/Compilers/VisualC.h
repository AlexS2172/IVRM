/**
 *	@file	VisualC.h
 *
 *	@brief	Microsoft Visual C++ configuration file.
 *
 *	$Log: $
 */

#if !defined (ACTIV_CONFIG_VISUAL_C_H)
#define ACTIV_CONFIG_VISUAL_C_H

#if (_MSC_VER < 1300)
#error VC++ version not supported
#endif

#define ACTIV_COMPILER_NAME "Microsoft Visual C++ version " ACTIV_STRINGIZE(_MSC_VER)

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
#if (_MSC_VER >= 1310)
#define ACTIV_DECLARE_TEMPLATE_INSTANTIATION(t)
#else
#define ACTIV_DECLARE_TEMPLATE_INSTANTIATION(t)	template t
#endif

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
#define ACTIV_DECLARE_NESTED_CLASS(c)	class c

#define snprintf	_snprintf
#define vsnprintf	_vsnprintf

// turn off annoying "C4355: 'this' : used in base member initializer list" warning
#pragma warning(disable:4355)

// turn off annoying "C4163: not available as an intrinsic function" warning
#pragma warning(disable:4163)

#endif // !defined (ACTIV_CONFIG_VISUAL_C_H)
