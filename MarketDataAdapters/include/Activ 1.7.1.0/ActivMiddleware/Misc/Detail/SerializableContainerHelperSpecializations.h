/**
 *  @file	SerializableContainerHelperSpecializations.h
 *
 *  @brief	Classes to help serialize STL containers.
 *
 *  $Log: $
 */

#if (!defined ACTIV_MISC_SERIALIZABLE_CONTAINER_HELPER_SPECIALIZATIONS_H)
#define ACTIV_MISC_SERIALIZABLE_CONTAINER_HELPER_SPECIALIZATIONS_H

namespace Activ
{

/**
 *	@brief	SerializableContainerHelper specialization for std::vector containers.
 */
template <typename OBJECT_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH, typename OBJECT_HELPER>
class SerializableContainerHelper<std::vector<OBJECT_TYPE>, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, OBJECT_HELPER>
{
public:
	typedef std::vector<OBJECT_TYPE> Vector;
	typedef COUNT_TYPE Count;
	typedef OBJECT_HELPER ObjectHelper;

	/**
	 *	@brief	Serialize a vector.
	 *
	 *	@param	messageBuilder.
	 *	@param	vector container to serialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const Vector &vector)
	{
		return SerializeContainer<Vector, Count, MIN_LENGTH, MAX_LENGTH, ObjectHelper>(messageBuilder, vector);
	}

	/**
	 *	@brief	Deserialize a vector.
	 *
	 *	@param	messageValidator.
	 *	@param	vector container to deserialize into.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(MessageValidator &messageValidator, Vector &vector)
	{
		Count count;

		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(count), MIN_LENGTH, MAX_LENGTH, MessageValidator::ENDIAN_LITTLE, &count));

		const size_t originalSize = vector.size();
		const size_t newSize = originalSize + count;
		StatusCode statusCode;

		// expand the vector to the new size (default constructor called for new objects)
		vector.resize(newSize);

		for (size_t i = originalSize; i < newSize; ++i)
		{
			// deserialize directly into the object in the vector
			statusCode = ObjectHelper::Deserialize(messageValidator, vector.at(i));

			if (STATUS_CODE_SUCCESS != statusCode)
			{
				vector.resize(originalSize);
				return statusCode;
			}
		}

		return STATUS_CODE_SUCCESS;
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Specialization for list containers.
 */
template <typename OBJECT_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH, typename OBJECT_HELPER>
class SerializableContainerHelper<std::list<OBJECT_TYPE>, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, OBJECT_HELPER>
{
public:
	typedef std::list<OBJECT_TYPE> List;
	typedef COUNT_TYPE Count;
	typedef OBJECT_HELPER ObjectHelper;

	/**
	 *	@brief	Serialize a list.
	 *
	 *	@param	messageBuilder.
	 *	@param	list container to serialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const List &list)
	{
		return SerializeContainer<List, Count, MIN_LENGTH, MAX_LENGTH, ObjectHelper>(messageBuilder, list);
	}

	/**
	 *	@brief	Deserialize a list.
	 *
	 *	@param	messageValidator.
	 *	@param	list container to deserialize into.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(MessageValidator &messageValidator, List &list)
	{
		Count count;

		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(count), MIN_LENGTH, MAX_LENGTH, MessageValidator::ENDIAN_LITTLE, &count));

		const size_t originalSize = list.size();
		StatusCode statusCode;

		for (size_t i = 0; i < count; ++i)
		{
			list.push_back(OBJECT_TYPE());

			// deserialize directly into the object in the list
			statusCode = ObjectHelper::Deserialize(messageValidator, list.back());

			if (STATUS_CODE_SUCCESS != statusCode)
			{
				list.resize(originalSize);
				return statusCode;
			}
		}

		return STATUS_CODE_SUCCESS;
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Specialization for set containers.
 */
template <typename OBJECT_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH, typename OBJECT_HELPER>
class SerializableContainerHelper<std::set<OBJECT_TYPE>, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, OBJECT_HELPER>
{
public:
	typedef std::set<OBJECT_TYPE> Set;
	typedef COUNT_TYPE Count;
	typedef OBJECT_HELPER ObjectHelper;

	/**
	 *	@brief	Serialize a set.
	 *
	 *	@param	messageBuilder.
	 *	@param	set container to serialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const Set &set)
	{
		return SerializeContainer<Set, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, OBJECT_HELPER>(messageBuilder, set);
	}

	/**
	 *	@brief	Deserialize a set.
	 *
	 *	@param	messageValidator.
	 *	@param	set container to deserialize into.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(MessageValidator &messageValidator, Set &set)
	{
		Count count;

		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(count), MIN_LENGTH, MAX_LENGTH, MessageValidator::ENDIAN_LITTLE, &count));

		for (Count i = 0; i < count; ++i)
		{
			typename Set::value_type object;

			ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ObjectHelper::Deserialize(messageValidator, object));

			(void) set.insert(object);
		}

		return STATUS_CODE_SUCCESS;
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Specialization for map containers.
 */
template <typename KEY_TYPE, typename MAPPED_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH, typename OBJECT_HELPER>
class SerializableContainerHelper<std::map<KEY_TYPE, MAPPED_TYPE>, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, OBJECT_HELPER>
{
public:
	typedef std::map<KEY_TYPE, MAPPED_TYPE> Map;
	typedef COUNT_TYPE Count;
	typedef OBJECT_HELPER ObjectHelper;

	/**
	 *	@brief	Serialize a map.
	 *
	 *	@param	messageBuilder.
	 *	@param	map container to serialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const Map &map)
	{
		return SerializeAssociativeContainer<Map, Count, MIN_LENGTH, MAX_LENGTH, ObjectHelper>(messageBuilder, map);
	}

	/**
	 *	@brief	Deserialize a map.
	 *
	 *	@param	messageValidator.
	 *	@param	map container to deserialize into.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(MessageValidator &messageValidator, Map &map)
	{
		Count count;

		ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, messageValidator.ValidateUnsignedBinaryIntegral(sizeof(count), MIN_LENGTH, MAX_LENGTH, MessageValidator::ENDIAN_LITTLE, &count));

		for (Count i = 0; i < count; ++i)
		{
			typename Map::value_type value;

			// cheating slightly to avoid deserializing into separate key and value and resultant additional construction of MapType::value_type
			// (first of value_type is const. using a pair<KEY_TYPE, MAPPED_TYPE> will incur copy construction if passed to insert)
			ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DeserializeObject(messageValidator, const_cast<KEY_TYPE &>(value.first)));
			ACTIV_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, ObjectHelper::Deserialize(messageValidator, value.second));

			(void) map.insert(value);
		}

		return STATUS_CODE_SUCCESS;
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Partial specialization of SerializableObjectContainerHelper to serialize maps of objects.
 */
template <typename KEY_TYPE, typename MAPPED_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH>
class SerializableObjectContainerHelper<std::map<KEY_TYPE, MAPPED_TYPE>, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH> : public SerializableContainerHelper<std::map<KEY_TYPE, MAPPED_TYPE>, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, SerializableObjectHelper<MAPPED_TYPE> >
{
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Partial specialization of SerializableRawPointerContainerHelper to serialize maps of raw pointers to objects.
 *
 *	Note that currently the key type must not be a pointer, the mapped type should be a raw pointer.
 */
template <typename KEY_TYPE, typename MAPPED_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH>
class SerializableRawPointerContainerHelper<std::map<KEY_TYPE, MAPPED_TYPE>, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH> : public SerializableContainerHelper<std::map<KEY_TYPE, MAPPED_TYPE>, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, SerializableRawPointerHelper<MAPPED_TYPE> >
{
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Partial specialization of helper class to serialize maps of smart pointers to objects.
 *
 *	Note that currently the key type must not be a pointer, the mapped type should be a smart pointer.
 */
template <typename KEY_TYPE, typename MAPPED_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH>
class SerializableSmartPointerContainerHelper<std::map<KEY_TYPE, MAPPED_TYPE>, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH> : public SerializableContainerHelper<std::map<KEY_TYPE, MAPPED_TYPE>, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, SerializableSmartPointerHelper<MAPPED_TYPE> >
{
};

} // namespace Activ

#endif // (!defined ACTIV_MISC_SERIALIZABLE_CONTAINER_HELPER_SPECIALIZATIONS_H)
