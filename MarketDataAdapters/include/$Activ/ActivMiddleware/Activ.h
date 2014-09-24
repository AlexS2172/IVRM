/**
 *	@file	Activ.h
 *
 *	@brief	Activ header file.
 *
 *	$Log: $
 */

#if !defined (ACTIV_H)
#define ACTIV_H

/**
 *	@brief	Activ middleware version.
 */
#define ACTIV_MIDDLEWARE_VERSION							"1.6.10.0"

/**
 *	@brief	Calculate the number of elements in an array.
 */
#define ACTIV_ARRAY_SIZE(a)									(sizeof((a)) / sizeof((a)[0]))

/**
 *	@brief	Retrieve the first element of a macro pair.
 */
#define ACTIV_DECLARE_ELEMENT_1_WITH_COMMA(a, b)			a,

/**
 *	@brief	Retrieve the second element of a macro pair.
 */
#define ACTIV_DECLARE_ELEMENT_2_WITH_COMMA(a, b)			b,

/**
 *	@brief	Retrieve the macro parameter.
 */
#define ACTIV_DECLARE_VALUE_WITH_COMMA(a)					a,

/**
 *	@brief	Retrieve the macro parameter and prepend with 4 underscores.
 */
#define ACTIV_DECLARE_HIDDEN_VALUE_WITH_COMMA(a)			____##a,

/**
 *	@brief	Retrieve the first element of a macro pair and prepend with 4 underscores.
 */
#define ACTIV_DECLARE_HIDDEN_ELEMENT_1(a, b)				____##a,

/**
 *	@brief	Retrieve the second element of a macro pair and prepend with 4 underscores.
 */
#define ACTIV_DECLARE_HIDDEN_ELEMENT_2(a, b)				____##b,

/**
 *	@brief	Declare a constanst value from a hidden enumeration value declared with ACTIV_DECLARE_HIDDEN_ELEMENT_1.
 */
#define ACTIV_DECLARE_CONSTANT_ELEMENT_1(type, a, b)		const type a = ____##a;
#define ACTIV_DECLARE_STATIC_CONSTANT_ELEMENT_1(type, a, b)	static const type a = ____##a;

/**
 *	@brief	Converts parameter a to a string after macro replacement on a has been performed.
 */
#define ACTIV_DO_STRINGIZE(a)								#a
#define ACTIV_STRINGIZE(a)									ACTIV_DO_STRINGIZE(a)
#define ACTIV_STRINGIZE_WITH_COMMA(a)						ACTIV_DO_STRINGIZE(a),

/**
 *	@brief	Retrieves the offset of a member from the beginning of its parent structure.
 */
#define ACTIV_FIELD_OFFSET(type, field)						(reinterpret_cast<size_t>(&(reinterpret_cast<type *>(0)->*(&type::field))))

/**
 *	@brief	Retrieves the size of a member in its parent structure.
 */
#define ACTIV_FIELD_SIZE(type, field)						(sizeof(reinterpret_cast<type *>(0)->*(&type::field)))

/**
 *	@brief	Debug macros.
 */
#if defined _DEBUG
#define ACTIV_DEBUG(a)	a
#else
#define ACTIV_DEBUG(a)
#endif

/**
 *	@brief	Release macros.
 */
#if !defined _DEBUG
#define ACTIV_RELEASE(a)	a
#else
#define ACTIV_RELEASE(a)
#endif

/**
 *	@brief	Enhanced debug macros.
 */
#if defined _DEBUG && defined ACTIV_ENABLE_ENHANCED_DEBUG
#define ACTIV_ENHANCED_DEBUG(a)	a
#else
#define ACTIV_ENHANCED_DEBUG(a)
#endif

#include "Config/Config.h"
#include "BasicTypes.h"
#include "FormatSpecifiers.h"
#include "StatusCodes.h"

#endif // !defined (ACTIV_H)
