/**
 *	@file	FunctionObjects.h
 *
 *	@brief	Header file for function objects.
 *
 *	$Log: $
 */

#if (!defined ACTIV_MISC_FUNCTION_OBJECTS_H)
#define ACTIV_MISC_FUNCTION_OBJECTS_H

#include <algorithm>
#include <functional>

namespace Activ
{

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Do-nothing function object.
 */
class NullObject
{
public:
	template<typename T>
	void operator()(const T *ptr) const
	{
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Delete object.
 *
 *	Can be used in conjunction with for_each() to destroy a sequence container of pointers, for instance.
 */
class DeleteObject
{
public:
	/**
	 *	@brief	Function operator.
	 *
	 *	@param	ptr a pointer to the object to delete.
	 */
	template<typename T>
	void operator()(const T *ptr) const
	{
		delete ptr;
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Delete object array.
 *
 *	Can be used in conjunction with for_each() to destroy a sequence container of pointers, for instance.
 */
class DeleteObjectArray
{
	public:
	/**
	 *	@brief	Function operator.
	 *
	 *	@param	ptr a pointer to the object array to delete.
	 */
	template<typename T>
	void operator()(const T *ptr) const
	{
		delete [] ptr;
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Delete associative object.
 *
 *	Can be used in conjunction with for_each() to destroy an associative container of pointers, for instance.
 */
class DeleteAssociativeObject
{
public:
	/**
	 *	@brief	Function operator.
	 *
	 *	@param	pair a reference to the object to delete.
	 */
	template<typename Pair>
	void operator()(const Pair &pair) const
	{
		delete pair.second;
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Less than pair comparison function object.
 */
template <typename Pair>
class LessThanPairComparisonObject : public std::binary_function<const Pair, const Pair, bool>
{
public:
	/**
	 *	@brief	Function operator.
	 *
	 *	@param	lhs a reference to the lhs object.
	 *	@param	rhs a reference to the rhs object.
	 *
	 *	@return	whether the lhs object is less than rhs object.
	 */
	bool operator()(const Pair &lhs, const Pair &rhs) const
	{
		return LessThan(lhs.first, rhs.first);
	}

	/**
	 *	@brief	Function operator.
	 *
	 *	@param	lhs a reference to the lhs object.
	 *	@param	rhs a reference to the rhs object.
	 *
	 *	@return	whether the lhs object is less than rhs object.
	 */
	bool operator()(const Pair &lhs, const typename Pair::first_type &rhs) const
	{
		return LessThan(lhs.first, rhs);
	}

	/**
	 *	@brief	Function operator.
	 *
	 *	@param	lhs a reference to the lhs object.
	 *	@param	rhs a reference to the rhs object.
	 *
	 *	@return	whether the lhs object is less than rhs object.
	 */
	bool operator()(const typename Pair::first_type &lhs, const Pair &rhs) const
	{
		return LessThan(lhs, rhs.first);
	}

private:
	/**
	 *	@brief	Less than.
	 *
	 *	@param	lhs a reference to the lhs object.
	 *	@param	rhs a reference to the rhs object.
	 *
	 *	@return	whether the lhs object is less than rhs object.
	 */
	bool LessThan(const typename Pair::first_type &lhs, const typename Pair::first_type &rhs) const
	{
		return lhs < rhs;
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Equal to pair comparison function object.
 */
template <typename Pair>
class EqualToPairComparisonObject : public std::binary_function<const Pair, const Pair, bool>
{
public:
	/**
	 *	@brief	Function operator.
	 *
	 *	@param	lhs a reference to the lhs object.
	 *	@param	rhs a reference to the rhs object.
	 *
	 *	@return	whether the lhs object is equal to the rhs object.
	 */
	bool operator()(const Pair &lhs, const Pair &rhs) const
	{
		return EqualTo(lhs.first, rhs.first);
	}

	/**
	 *	@brief	Function operator.
	 *
	 *	@param	lhs a reference to the lhs object.
	 *	@param	rhs a reference to the rhs object.
	 *
	 *	@return	whether the lhs object is equal to the rhs object.
	 */
	bool operator()(const Pair &lhs, const typename Pair::first_type &rhs) const
	{
		return EqualTo(lhs.first, rhs);
	}

	/**
	 *	@brief	Function operator.
	 *
	 *	@param	lhs a reference to the lhs object.
	 *	@param	rhs a reference to the rhs object.
	 *
	 *	@return	whether the lhs object is less than rhs object.
	 */
	bool operator()(const typename Pair::first_type &lhs, const Pair &rhs) const
	{
		return EqualTo(lhs, rhs.first);
	}

private:
	/**
	 *	@brief	Equal to.
	 *
	 *	@param	lhs a reference to the lhs object.
	 *	@param	rhs a reference to the rhs object.
	 *
	 *	@return	whether the lhs object is equal to rhs object.
	 */
	bool EqualTo(const typename Pair::first_type &lhs, const typename Pair::first_type &rhs) const
	{
		return (lhs == rhs);
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	External member function object adapter.
 */
template <typename Result, typename Type, typename Parameter>
class ExternalMemberFunctionObject : public std::unary_function<Parameter, Result>
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	type the type of the object containing the member function.
	 *	@param	pMemberFunction the member function to call.
	 */
	explicit ExternalMemberFunctionObject(Type *pType, Result (Type::*pMemberFunction)(Parameter))
	{
		m_pType = pType;
		m_pMemberFunction = pMemberFunction;
	}

	/**
	 *	@brief	Function operator.
	 *
	 *	@param	parameter the parameter of the member function.
	 *
	 *	@return	the result of the member function.
	 */
	Result operator()(Parameter parameter)
	{
		return (m_pType->*(m_pMemberFunction))(parameter);
	}

private:
	Type *m_pType;
	Result (Type::*m_pMemberFunction)(Parameter);
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	External member function object adapter.
 */
template <typename Result, typename Type, typename Parameter>
class ConstExternalMemberFunctionObject : public std::unary_function<Parameter, Result>
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	type the type of the object containing the member function.
	 *	@param	pMemberFunction the member function to call.
	 */
	explicit ConstExternalMemberFunctionObject(Type *pType, Result (Type::*pMemberFunction)(Parameter) const)
	{
		m_pType = pType;
		m_pMemberFunction = pMemberFunction;
	}

	/**
	 *	@brief	Function operator.
	 *
	 *	@param	parameter the parameter of the member function.
	 *
	 *	@return	the result of the member function.
	 */
	Result operator()(Parameter parameter) const
	{
		return (m_pType->*(m_pMemberFunction))(parameter);
	}

private:
	Type *m_pType;
	Result (Type::*m_pMemberFunction)(Parameter) const;
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Function template for passing a supplied parameter to an external member function.
 */
template <typename Result, typename Type, typename Parameter>
ExternalMemberFunctionObject<Result, Type, Parameter> ExternalMemberFunction(Type *pType, Result (Type::*pMemberFunction)(Parameter))
{
	return ExternalMemberFunctionObject<Result, Type, Parameter>(pType, pMemberFunction);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Function template for passing a supplied parameter to an external member function.
 */
template <typename Result, typename Type, typename Parameter>
ExternalMemberFunctionObject<Result, Type, Parameter> ExternalMemberFunction(Type &type, Result (Type::*pMemberFunction)(Parameter))
{
	return ExternalMemberFunctionObject<Result, Type, Parameter>(&type, pMemberFunction);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Function template for passing a supplied parameter to an external member function.
 */
template <typename Result, typename Type, typename Parameter>
ConstExternalMemberFunctionObject<Result, Type, Parameter> ConstExternalMemberFunction(Type *pType, Result (Type::*pMemberFunction)(Parameter) const)
{
	return ConstExternalMemberFunctionObject<Result, Type, Parameter>(pType, pMemberFunction);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Function template for passing a supplied parameter to an external member function.
 */
template <typename Result, typename Type, typename Parameter>
ConstExternalMemberFunctionObject<Result, Type, Parameter> ConstExternalMemberFunction(Type &type, Result (Type::*pMemberFunction)(Parameter) const)
{
	return ConstExternalMemberFunctionObject<Result, Type, Parameter>(&type, pMemberFunction);
}

} // namespace Activ

#endif // (!defined ACTIV_MISC_FUNCTION_OBJECTS_H)
