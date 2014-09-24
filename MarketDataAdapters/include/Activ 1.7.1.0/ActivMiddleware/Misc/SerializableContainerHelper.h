/**
 *  @file	SerializableContainerHelper.h
 *
 *  @brief	Classes to help serialize STL containers.
 *
 *  $Log: $
 */

#if (!defined ACTIV_MISC_SERIALIZABLE_CONTAINER_HELPER_H)
#define ACTIV_MISC_SERIALIZABLE_CONTAINER_HELPER_H

#include "ActivMiddleware/Misc/Detail/SerializableContainerHelperDetail.h"

namespace Activ
{

/**
 *	@brief	Helper class to serialize STL containers.
 *
 *	The OBJECT_HELPER template class must provide the following methods:
 *		static StatusCode Serialize(MessageBuilder &messageBuilder, const typename CONTAINER_TYPE::value_type &object)
 *		static StatusCode Deserialize(MessageValidator &messageValidator, typename CONTAINER_TYPE::value_type &object)
 *
 *	Derived classes are provided for containers of object, raw & smart pointers where the underlying object to be stored
 *	is either a bultin type or provides Serialize() and Deserialize() methods.
 *
 *	@param	CONTAINER_TYPE type of container.
 *	@param	COUNT_TYPE type to use for container length in serialized form.
 *	@param	MIN_LENGTH min allowed container length.
 *	@param	MAX_LENGTH max allowed container length.
 *	@param	OBJECT_HELPER class to (de)serialize objects stored in the container.
 */
template <typename CONTAINER_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH, COUNT_TYPE MAX_LENGTH, typename OBJECT_HELPER>
class SerializableContainerHelper
{
public:
	typedef CONTAINER_TYPE Container;

	/**
	 *	@brief	Serialize a container.
	 *
	 *	@param	messageBuilder.
	 *	@param	container container to serialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Serialize(MessageBuilder &messageBuilder, const Container &container)
	{
		// currently specializations for std::vector, std::list, std::set and std::map are provided
		BOOST_STATIC_ASSERT(sizeof(COUNT_TYPE) == 0);

		return STATUS_CODE_NOT_SUPPORTED;
	}

	/**
	 *	@brief	Deserialize a container.
	 *
	 *	@param	messageValidator.
	 *	@param	container container to deserialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(MessageValidator &messageValidator, Container &container)
	{
		// currently specializations for std::vector, std::list, std::set and std::map are provided
		BOOST_STATIC_ASSERT(sizeof(COUNT_TYPE) == 0);

		return STATUS_CODE_NOT_SUPPORTED;
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Helper class to serialize containers of objects.
 *
 *	The type of object being (de)serialized must provide the following methods:
 *		StatusCode Serialize(MessageBuilder &messageBuilder) const
 *		StatusCode Deserialize(MessageValidator &messageValidator)
 *
 *	unless the type is a builtin signed, unsigned or boolean type for which specializations are provided.
 *
 *	The class may then be used as follows, where MyClass is a class providing Serialize()
 *	and Deserialize() methods as described above.
 *
 *	typedef std::vector<MyClass> MyClassList;
 *	typedef SerializableObjectContainerHelper<MyClassList, uint8_t, 0, 10> MyClassListSerializer;
 *
 *	MyClassList myClassList;
 *
 *	if (STATUS_CODE_SUCCESS == MyClassListSerializer::Serialize(messageBuilder, myClassList)) ...
 *	if (STATUS_CODE_SUCCESS == MyClassListSerializer::Deserialize(messageValidator, myClassList)) ...
 *
 *	In this case, MyClassListSerializer will use one byte to serialize the length of the container, and the container
 *	can contain a maximum of 10 items otherwise (de)serialization will fail with STATUS_CODE_INVALID_LENGTH.
 *
 *	@param	CONTAINER_TYPE type of container.
 *	@param	COUNT_TYPE type to use for container length in serialized form. Must be unsigned basic type.
 *	@param	MIN_LENGTH min allowed container length, default is 0.
 *	@param	MAX_LENGTH max allowed container length, default is max value of COUNT_TYPE.
 */
template <typename CONTAINER_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH = 0, COUNT_TYPE MAX_LENGTH = boost::integer_traits<COUNT_TYPE>::const_max>
class SerializableObjectContainerHelper : public SerializableContainerHelper<CONTAINER_TYPE, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, SerializableObjectHelper<typename CONTAINER_TYPE::value_type> >
{
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Helper class to serialize containers of raw pointers to objects.
 *
 *	The pointee type that is being (de)serialized must provide the following methods:
 *		StatusCode Serialize(MessageBuilder &messageBuilder) const
 *		StatusCode Deserialize(MessageValidator &messageValidator)
 *
 *	unless the type is a builtin signed, unsigned or boolean type for which specializations are provided.
 *
 *	The class may then be used as follows, where MyClass is a class providing Serialize()
 *	and Deserialize() methods as described above.
 *
 *	typedef std::set<MyClass *> MyClassList;
 *	typedef SerializableRawPointerContainerHelper<MyClassList, uint16_t> MyClassListSerializer;
 *
 *	MyClassList myClassList;
 *
 *	if (STATUS_CODE_SUCCESS == MyClassListSerializer::Serialize(messageBuilder, myClassList)) ...
 *	if (STATUS_CODE_SUCCESS == MyClassListSerializer::Deserialize(messageValidator, myClassList)) ...
 *
 *	In this case, MyClassListSerializer will use two bytes to serialize the length of the container, and the container
 *	can INT16_MAX number of elements.
 *
 *	@param	CONTAINER_TYPE type of container.
 *	@param	COUNT_TYPE type to use for container length in serialized form. Must be unsigned basic type.
 *	@param	MIN_LENGTH min allowed container length, default is 0.
 *	@param	MAX_LENGTH max allowed container length, default is max value of COUNT_TYPE.
 */
template <typename CONTAINER_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH = 0, COUNT_TYPE MAX_LENGTH = boost::integer_traits<COUNT_TYPE>::const_max>
class SerializableRawPointerContainerHelper : public SerializableContainerHelper<CONTAINER_TYPE, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, SerializableRawPointerHelper<typename CONTAINER_TYPE::value_type> >
{
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Helper class to serialize containers of smart pointers to objects.
 *
 *	The pointee type that is being (de)serialized must provide the following methods:
 *		StatusCode Serialize(MessageBuilder &messageBuilder) const
 *		StatusCode Deserialize(MessageValidator &messageValidator)
 *
 *	unless the type is a builtin signed, unsigned or boolean type for which specializations are provided.
 *
 *	The class may then be used as follows, where MyClass is a class providing Serialize()
 *	and Deserialize() methods as described above.
 *
 *	typedef std::list<boost::shared_ptr<MyClass> > MyClassList;
 *	typedef SerializableRawPointerContainerHelper<MyClassList, uint16_t> MyClassListSerializer;
 *
 *	MyClassList myClassList;
 *
 *	if (STATUS_CODE_SUCCESS == MyClassListSerializer::Serialize(messageBuilder, myClassList)) ...
 *	if (STATUS_CODE_SUCCESS == MyClassListSerializer::Deserialize(messageValidator, myClassList)) ...
 *
 *	In this case, MyClassListSerializer will use two bytes to serialize the length of the container, and the container
 *	can have INT16_MAX number of elements.
 *
 *	@param	CONTAINER_TYPE type of container.
 *	@param	COUNT_TYPE type to use for container length in serialized form. Must be unsigned basic type.
 *	@param	MIN_LENGTH min allowed container length, default is 0.
 *	@param	MAX_LENGTH max allowed container length, default is max value of COUNT_TYPE.
 */
template <typename CONTAINER_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH = 0, COUNT_TYPE MAX_LENGTH = boost::integer_traits<COUNT_TYPE>::const_max>
class SerializableSmartPointerContainerHelper : public SerializableContainerHelper<CONTAINER_TYPE, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, SerializableSmartPointerHelper<typename CONTAINER_TYPE::value_type> >
{
};

} // namespace Activ

#include "ActivMiddleware/Misc/Detail/SerializableContainerHelperSpecializations.h"

#endif // (!defined ACTIV_MISC_SERIALIZABLE_CONTAINER_HELPER_H)
