/**
 *  @file	SerializableContainerHelperDetail.h
 *
 *  @brief	Classes to help serialize stl containers.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SERIALIZABLE_CONTAINER_HELPER_DETAIL_H)
#define ACTIV_SERIALIZABLE_CONTAINER_HELPER_DETAIL_H

#include "ActivMiddleware/ActivMessageHandler/MessageBuilder.h"
#include "ActivMiddleware/ActivMessageHandler/MessageValidater.h"

#include "boost/integer_traits.hpp"

#include <vector>
#include <list>
#include <set>

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
 *	for which specializations are provided that use the MessageValidater methods.
 *
 *	@param	messageBuilder.
 *	@param	object.
 *
 *	@retval	STATUS_CODE_SUCCESS
 *	@retval	...
 */
template <typename OBJECT_TYPE>
inline StatusCode DeserializeObject(MessageValidater &messageValidater, OBJECT_TYPE &object)
{
	return object.Deserialize(messageValidater);
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
#define ACTIV_DESERIALIZE_OBJECT(type, validater)													\
	inline StatusCode DeserializeObject(MessageValidater &messageValidater, type &value)			\
	{																								\
		return messageValidater.validater(sizeof(value), MessageValidater::ENDIAN_LITTLE, &value);	\
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
	return SerializeObject(messageBuilder, static_cast<uint8_t>(booleanValue));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode DeserializeObject(MessageValidater &messageValidater, bool &booleanValue)
{
	uint8_t value8;
	
	ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DeserializeObject(messageValidater, value8));

	booleanValue = (0 != value8);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

// Specializations for string type
inline StatusCode SerializeObject(MessageBuilder &messageBuilder, const std::string &str)
{
	return messageBuilder.Append(str);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode DeserializeObject(MessageValidater &messageValidater, std::string &str)
{
	return messageValidater.ValidateString(&str);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Helper class to (de)serialize objects.
 *
 *	Provides static member functions which wrap up the global SerializeObject() and DeserializeObject().
 *	This class can then be used as a parameter to the container serialize helper template classes.
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
	 *	@param	messageValidater.
	 *	@param	object object to deserialize into.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(MessageValidater &messageValidater, OBJECT_TYPE &object)
	{
		return DeserializeObject(messageValidater, object);
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Helper class to (de)serialize smart pointers to objects.
 *
 *	Smart pointer type must provide an element_type typedef for the underlying object type.
 *
 *	Provides static member functions which wrap up the global SerializeObject() and DeserializeObject().
 *	This class can then be used as a parameter to the container serialize helper template classes.
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
	 *	@param	messageValidater.
	 *	@param	object object to deserialize into.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(MessageValidater &messageValidater, POINTER_TYPE &pObject)
	{
		// new object into raw pointer
		ObjectType * const pRawObject = new (std::nothrow) ObjectType;

		if (0 == pRawObject)
			return STATUS_CODE_HEAP_EMPTY;

		// some smart pointers don't allow assignment from raw pointer. Assign from temporary
		pObject = POINTER_TYPE(pRawObject);

		return DeserializeObject(messageValidater, *pObject);
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Helper class to (de)serialize raw pointers to objects.
 *
 *	Provides static member functions which wrap up the global SerializeObject() and DeserializeObject().
 *	This class can then be used as a parameter to the container serialize helper template classes.
 */
template<typename POINTER_TYPE>
class SerializableRawPointerHelper
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
	static StatusCode Serialize(MessageBuilder &messageBuilder, const POINTER_TYPE &pObject)
	{
		return SerializeObject(messageBuilder, *pObject);
	}

	/**
	 *	@brief	Deserialize an object.
	 *
	 *	@param	messageValidater.
	 *	@param	object object to deserialize into.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(MessageValidater &messageValidater, POINTER_TYPE &pObject)
	{
		// new object into raw pointer
		// NB this can be removed if using a compiler that supports partial template specializations,
		// pObject = new (std::nothrow) boost::remove_pointer<POINTER_TYPE>::type could be used instead
		NewObject(pObject);

		if (0 == pObject)
			return STATUS_CODE_HEAP_EMPTY;

		return DeserializeObject(messageValidater, *pObject);
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	New an object.
 *
 *	@param	pObject pointer to receive object.
 */
template <typename T>
inline void NewObject(T* &pObject)
{
	pObject = new (std::nothrow) T;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Helper class to work around vc++7.0 not supporting partial template specialization.
 *
 *	With partial specialization, SerializableContainerHelper could be partially specialized for
 *	set<T>, vector<T>, etc. Here we rely on overloading instead, with an extra level of indirection.
 */
class SerializableContainerValueHelper
{
public:
	/**
	 *	@brief	Serialize a vector container.
	 *
	 *	@param	messageBuilder.
	 *	@param	vector the vector to serialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	template<typename OBJECT_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH, typename OBJECT_HELPER>
	static StatusCode Serialize(MessageBuilder &messageBuilder, const std::vector<OBJECT_TYPE> &vector)
	{
		// serialize vector container
		return SerializeContainer<std::vector<OBJECT_TYPE>, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, OBJECT_HELPER>(messageBuilder, vector);
	}

	/**
	 *	@brief	Serialize a list container.
	 *
	 *	@param	messageBuilder.
	 *	@param	list the list to serialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	template<typename OBJECT_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH, typename OBJECT_HELPER>
	static StatusCode Serialize(MessageBuilder &messageBuilder, const std::list<OBJECT_TYPE> &list)
	{
		// serialize vector container
		return SerializeContainer<std::list<OBJECT_TYPE>, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, OBJECT_HELPER>(messageBuilder, list);
	}

	/**
	 *	@brief	Serialize a set container.
	 *
	 *	@param	messageBuilder.
	 *	@param	set the set to serialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	template<typename OBJECT_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH, typename OBJECT_HELPER>
	static StatusCode Serialize(MessageBuilder &messageBuilder, const std::set<OBJECT_TYPE> &set)
	{
		// serialize set container
		return SerializeContainer<std::set<OBJECT_TYPE>, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, OBJECT_HELPER>(messageBuilder, set);
	}

	/**
	 *	@brief	Deserialize a vector container.
	 *
	 *	@param	messageValidater.
	 *	@param	vector the vector to deserialize into.
	 */
	template<typename OBJECT_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH, typename OBJECT_HELPER>
	static StatusCode Deserialize(MessageValidater &messageValidater, std::vector<OBJECT_TYPE> &vector)
	{
		typedef COUNT_TYPE Count;
		typedef std::vector<OBJECT_TYPE> Vector;
		typedef OBJECT_HELPER ObjectHelper;

		Count count;

		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(count), MIN_LENGTH, MAX_LENGTH, MessageValidater::ENDIAN_LITTLE, &count));

		const size_t originalSize = vector.size();
		const size_t newSize = originalSize + count;
		StatusCode statusCode;

		// expand the vector to the new size (default constructor called for new objects)
		vector.resize(newSize);

		for (size_t i = originalSize; i < newSize; ++i)
		{
			// deserialize directly into the object in the vector
			statusCode = ObjectHelper::Deserialize(messageValidater, vector.at(i));

			if (STATUS_CODE_SUCCESS != statusCode)
			{
				vector.resize(originalSize);
				return statusCode;
			}
		}

		return STATUS_CODE_SUCCESS;
	}

	/**
	 *	@brief	Deserialize a list container.
	 *
	 *	@param	messageValidater.
	 *	@param	list the list to deserialize into.
	 */
	template<typename OBJECT_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH, typename OBJECT_HELPER>
	static StatusCode Deserialize(MessageValidater &messageValidater, std::list<OBJECT_TYPE> &list)
	{
		typedef COUNT_TYPE Count;
		typedef std::list<OBJECT_TYPE> Vector;
		typedef OBJECT_HELPER ObjectHelper;

		Count count;

		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(count), MIN_LENGTH, MAX_LENGTH, MessageValidater::ENDIAN_LITTLE, &count));

		const size_t originalSize = list.size();
		StatusCode statusCode;

		for (size_t i = 0; i < count; ++i)
		{
			list.push_back(OBJECT_TYPE());

			// deserialize directly into the object in the list
			statusCode = ObjectHelper::Deserialize(messageValidater, list.back());

			if (STATUS_CODE_SUCCESS != statusCode)
			{
				list.resize(originalSize);
				return statusCode;
			}
		}

		return STATUS_CODE_SUCCESS;
	}

	/**
	 *	@brief	Deserialize a set container.
	 *
	 *	@param	messageValidater.
	 *	@param	set the set to deserialize into.
	 */
	template <typename OBJECT_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH, typename OBJECT_HELPER>
	static StatusCode Deserialize(MessageValidater &messageValidater, std::set<OBJECT_TYPE> &set)
	{
		typedef COUNT_TYPE Count;
		typedef std::set<OBJECT_TYPE> Set;
		typedef OBJECT_HELPER ObjectHelper;

		Count count;

		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidater.ValidateUnsignedBinaryIntegral(sizeof(count), MIN_LENGTH, MAX_LENGTH, MessageValidater::ENDIAN_LITTLE, &count));

		for (Count i = 0; i < count; ++i)
		{
			typename Set::value_type object;

			ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ObjectHelper::Deserialize(messageValidater, object));

			(void) set.insert(object);
		}

		return STATUS_CODE_SUCCESS;
	}

private:
	/**
	 *	@brief	Serialize any old container type.
	 *
	 *	@param	messageValidater.
	 *	@param	container the container to serialize.
	 */
	template<typename CONTAINER_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH, typename OBJECT_HELPER>
	static StatusCode SerializeContainer(MessageBuilder &messageBuilder, const CONTAINER_TYPE &container)
	{
		// this should be able to serialize all non-map type containers
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

	// TODO add a SerializePairContainer() for map container types
};

} // namespace Activ

#endif // !defined (ACTIV_SERIALIZABLE_CONTAINER_HELPER_DETAIL_H)
