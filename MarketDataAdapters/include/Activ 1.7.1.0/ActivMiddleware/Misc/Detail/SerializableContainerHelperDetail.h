/**
 *  @file	SerializableContainerHelperDetail.h
 *
 *  @brief	Classes to help serialize STL containers.
 *
 *  $Log: $
 */

#if (!defined ACTIV_MISC_SERIALIZABLE_CONTAINER_HELPER_DETAIL_H)
#define ACTIV_MISC_SERIALIZABLE_CONTAINER_HELPER_DETAIL_H

#include "ActivMiddleware/ActivBase/MessageBuilder.h"
#include "ActivMiddleware/ActivBase/MessageValidator.h"

#include "boost/integer_traits.hpp"
#include "boost/type_traits/remove_pointer.hpp"
#include "boost/static_assert.hpp"

#include <vector>
#include <list>
#include <set>
#include <map>

namespace Activ
{

/**
 *	@brief	Serialize object helper function.
 *
 *	This calls Serialize() on the provided object, unless it is a basic signed or unsigned type,
 *	for which specializations are provided that use the MessageBuilder methods.
 *
 *	@param	messageBuilder.
 *	@param	object.
 *
 *	@retval	STATUS_CODE_SUCCESS
 *	@retval	...
 */
template <typename OBJECT_TYPE>
inline StatusCode SerializeObject(MessageBuilder &messageBuilder, const OBJECT_TYPE &object)
{
	return object.Serialize(messageBuilder);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Deserialize object helper function.
 *
 *	This calls Deserialize() on the provided object, unless it is a basic signed or unsigned type,
 *	for which specializations are provided that use the MessageValidator methods.
 *
 *	@param	messageBuilder.
 *	@param	object.
 *
 *	@retval	STATUS_CODE_SUCCESS
 *	@retval	...
 */
template <typename OBJECT_TYPE>
inline StatusCode DeserializeObject(MessageValidator &messageValidator, OBJECT_TYPE &object)
{
	return object.Deserialize(messageValidator);
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Macro to declare SerializeObject() for a basic type.
 */
#define ACTIV_SERIALIZE_OBJECT(type)																\
	inline StatusCode SerializeObject(MessageBuilder &messageBuilder, const type &value)			\
	{																								\
		return messageBuilder.Append(value, MessageBuilder::ENDIAN_LITTLE);							\
	}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Macro to declare DeserializeObject() for a basic type.
 */
#define ACTIV_DESERIALIZE_OBJECT(type, validator)													\
	inline StatusCode DeserializeObject(MessageValidator &messageValidator, type &value)			\
	{																								\
		return messageValidator.validator(sizeof(value), MessageValidator::ENDIAN_LITTLE, &value);	\
	}

// ---------------------------------------------------------------------------------------------------------------------------------

// Specializations for basic unsigned types
ACTIV_SERIALIZE_OBJECT(uint8_t)
ACTIV_DESERIALIZE_OBJECT(uint8_t, ValidateUnsignedBinaryIntegral)
ACTIV_SERIALIZE_OBJECT(uint16_t)
ACTIV_DESERIALIZE_OBJECT(uint16_t, ValidateUnsignedBinaryIntegral)
ACTIV_SERIALIZE_OBJECT(uint32_t)
ACTIV_DESERIALIZE_OBJECT(uint32_t, ValidateUnsignedBinaryIntegral)
ACTIV_SERIALIZE_OBJECT(uint64_t)
ACTIV_DESERIALIZE_OBJECT(uint64_t, ValidateUnsignedBinaryIntegral)

// ---------------------------------------------------------------------------------------------------------------------------------

// Specializations for basic signed types
ACTIV_SERIALIZE_OBJECT(int8_t)
ACTIV_DESERIALIZE_OBJECT(int8_t, ValidateSignedBinaryIntegral)
ACTIV_SERIALIZE_OBJECT(int16_t)
ACTIV_DESERIALIZE_OBJECT(int16_t, ValidateSignedBinaryIntegral)
ACTIV_SERIALIZE_OBJECT(int32_t)
ACTIV_DESERIALIZE_OBJECT(int32_t, ValidateSignedBinaryIntegral)
ACTIV_SERIALIZE_OBJECT(int64_t)
ACTIV_DESERIALIZE_OBJECT(int64_t, ValidateSignedBinaryIntegral)

// ---------------------------------------------------------------------------------------------------------------------------------

// Specializations for boolean type
inline StatusCode SerializeObject(MessageBuilder &messageBuilder, const bool &booleanValue)
{
	return messageBuilder.AppendBoolean(booleanValue);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode DeserializeObject(MessageValidator &messageValidator, bool &booleanValue)
{
	return messageValidator.ValidateBoolean(&booleanValue);
}

// ---------------------------------------------------------------------------------------------------------------------------------

// Specializations for string type
inline StatusCode SerializeObject(MessageBuilder &messageBuilder, const std::string &str)
{
	return messageBuilder.Append(str);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode DeserializeObject(MessageValidator &messageValidator, std::string &str)
{
	return messageValidator.ValidateString(&str);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Helper class to (de)serialize objects.
 *
 *	Provides static member functions which wrap up the global SerializeObject() and DeserializeObject().
 *	This class can then be used as a parameter to SerializableContainerHelper.
 */
template<typename OBJECT_TYPE>
class SerializableObjectHelper
{
public:
	/**
	 *	@brief	Serialize an object.
	 *
	 *	@param	messageBuilder.
	 *	@param	object object to serialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const OBJECT_TYPE &object)
	{
		return SerializeObject(messageBuilder, object);
	}

	/**
	 *	@brief	Deserialize an object.
	 *
	 *	@param	messageValidator.
	 *	@param	object object to deserialize into.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(MessageValidator &messageValidator, OBJECT_TYPE &object)
	{
		return DeserializeObject(messageValidator, object);
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Helper class to (de)serialize smart pointers to objects.
 *
 *	Smart pointer type must provide an element_type typedef for the underlying object type.
 *
 *	Provides static member functions which wrap up the global SerializeObject() and DeserializeObject().
 *	This class can then be used as a parameter to SerializableContainerHelper.
 */
template<typename POINTER_TYPE>
class SerializableSmartPointerHelper
{
public:
	typedef typename POINTER_TYPE::element_type ObjectType;				///< Underlying object type

	/**
	 *	@brief	Serialize an object.
	 *
	 *	@param	messageBuilder.
	 *	@param	object object to serialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const POINTER_TYPE &pObject)
	{
		return SerializeObject(messageBuilder, *pObject);
	}

	/**
	 *	@brief	Deserialize an object.
	 *
	 *	@param	messageValidator.
	 *	@param	object object to deserialize into.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(MessageValidator &messageValidator, POINTER_TYPE &pObject)
	{
		// new object into raw pointer
		ObjectType * const pRawObject = new (std::nothrow) ObjectType;

		if (0 == pRawObject)
			return STATUS_CODE_HEAP_EMPTY;

		// some smart pointers don't allow assignment from raw pointer. Assign from temporary
		pObject = POINTER_TYPE(pRawObject);

		return DeserializeObject(messageValidator, *pObject);
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Helper class to (de)serialize raw pointers to objects.
 *
 *	Provides static member functions which wrap up the global SerializeObject() and DeserializeObject().
 *	This class can then be used as a parameter to SerializableContainerHelper.
 */
template<typename POINTER_TYPE>
class SerializableRawPointerHelper
{
public:
	typedef typename boost::remove_pointer<POINTER_TYPE>::type ObjectType;

	/**
	 *	@brief	Serialize an object.
	 *
	 *	@param	messageBuilder.
	 *	@param	object object to serialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const POINTER_TYPE &pObject)
	{
		return SerializeObject(messageBuilder, *pObject);
	}

	/**
	 *	@brief	Deserialize an object.
	 *
	 *	@param	messageValidator.
	 *	@param	object object to deserialize into.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(MessageValidator &messageValidator, POINTER_TYPE &pObject)
	{
		// new object into raw pointer
		pObject = new (std::nothrow) ObjectType;

		if (0 == pObject)
			return STATUS_CODE_HEAP_EMPTY;

		return DeserializeObject(messageValidator, *pObject);
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Serialize a non-associative container type.
 *
 *	@param	messageValidator.
 *	@param	container the container to serialize.
 */
template<typename CONTAINER_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH, typename OBJECT_HELPER>
StatusCode SerializeContainer(MessageBuilder &messageBuilder, const CONTAINER_TYPE &container)
{
	typedef COUNT_TYPE Count;
	typedef CONTAINER_TYPE Container;
	typedef OBJECT_HELPER ObjectHelper;

	const size_t size = container.size();

	if ((size < MIN_LENGTH) || (size > MAX_LENGTH))
		return STATUS_CODE_INVALID_LENGTH;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(static_cast<Count>(size), MessageBuilder::ENDIAN_LITTLE));

	for (typename Container::const_iterator iter = container.begin(), iterEnd = container.end(); iter != iterEnd; ++iter)
	{
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ObjectHelper::Serialize(messageBuilder, *iter));
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Serialize an associative container type.
 *
 *	The first type of the pair is assumed to @a not be a pointer.
 *
 *	@param	messageValidator.
 *	@param	container the container to serialize.
 */
template<typename CONTAINER_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH, typename OBJECT_HELPER>
StatusCode SerializeAssociativeContainer(MessageBuilder &messageBuilder, const CONTAINER_TYPE &container)
{
	typedef COUNT_TYPE Count;
	typedef CONTAINER_TYPE Container;
	typedef OBJECT_HELPER ObjectHelper;

	const size_t size = container.size();

	if ((size < MIN_LENGTH) || (size > MAX_LENGTH))
		return STATUS_CODE_INVALID_LENGTH;

	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageBuilder.Append(static_cast<Count>(size), MessageBuilder::ENDIAN_LITTLE));

	for (typename Container::const_iterator iter = container.begin(), iterEnd = container.end(); iter != iterEnd; ++iter)
	{
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, SerializeObject(messageBuilder, iter->first));
		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ObjectHelper::Serialize(messageBuilder, iter->second));
	}

	return STATUS_CODE_SUCCESS;
}

} // namespace Activ

#endif // (!defined ACTIV_MISC_SERIALIZABLE_CONTAINER_HELPER_DETAIL_H)
