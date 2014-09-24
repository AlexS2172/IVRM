/**
 *  @file	SerializableContainerHelper.h
 *
 *  @brief	Classes to help serialize stl containers.
 *
 *  $Log: $
 */

#if !defined (ACTIV_SERIALIZABLE_CONTAINER_HELPER_H)
#define ACTIV_SERIALIZABLE_CONTAINER_HELPER_H

// Hide the implementation a little as there's a lot of workarounds for lack of template specialization in vc++ 7.0.
#include "ActivMiddleware/Misc/Detail/SerializableContainerHelperDetail.h"

namespace Activ
{

/**
 *	@brief	Helper class to serialize stl containers.
 *
 *	The OBJECT_HELPER template class must provide the following methods:
 *		static StatusCode Serialize(MessageBuilder &messageBuilder, const typename CONTAINER_TYPE::value_type &object)
 *		static StatusCode Deserialize(MessageValidater &messageValidater, typename CONTAINER_TYPE::value_type &object)
 *
 *	Derived classes are provided for containers of object, raw & smart pointers where the underlying object to be stored
 *	is either a bultin type or provides Serialize() and Deserialize() methods. This class should only be used if
 *	specialized behaviour is required.
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
		// call the overloaded Serialize() in SerializableContainerValueHelper as we can't partially specialize by container type
		// (though in the case of Serialize() it might not be needed as the serialize code is the same for all container types
		// supported so far)
		return SerializableContainerValueHelper::Serialize<typename CONTAINER_TYPE::value_type, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, OBJECT_HELPER>(messageBuilder, container);
	}

	/**
	 *	@brief	Deserialize a container.
	 *
	 *	@param	messageValidater.
	 *	@param	container container to deserialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	static StatusCode Deserialize(MessageValidater &messageValidater, Container &container)
	{
		// call the overloaded Deserialize() in SerializableContainerValueHelper as we can't partially specialize by container type
		return SerializableContainerValueHelper::Deserialize<typename CONTAINER_TYPE::value_type, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, OBJECT_HELPER>(messageValidater, container);
	}
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Helper class to serialize containers of objects.
 *
 *	The type of object being (de)serialized must provide the following methods:
 *		StatusCode Serialize(MessageBuilder &messageBuilder) const
 *		StatusCode Deserialize(MessageValidater &messageValidater)
 *
 *	unless the type is a builtin signed, unsigned or boolean type for which specializtions are provided.
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
 *	@brief	Helper class to serialize containers of smart pointers to objects.
 *
 *	The pointee type that is being (de)serialized must provide the following methods:
 *		StatusCode Serialize(MessageBuilder &messageBuilder) const
 *		StatusCode Deserialize(MessageValidater &messageValidater)
 *
 *	unless the type is a builtin signed, unsigned or boolean type for which specializtions are provided.
 *
 *	@param	CONTAINER_TYPE type of container.
 *	@param	COUNT_TYPE type to use for container length in serialized form. Must be unsigned basic type.
 *	@param	MIN_LENGTH min allowed container length, default is 0.
 *	@param	MAX_LENGTH max allowed container length, default is max value of COUNT_TYPE.
 */
template <typename CONTAINER_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH = 0, COUNT_TYPE MAX_LENGTH = boost::integer_traits<COUNT_TYPE>::const_max>
class SerializableRawPointerContainerHelper : public SerializableContainerHelper<CONTAINER_TYPE, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, SerializableSmartPointerHelper<typename CONTAINER_TYPE::value_type> >
{
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Helper class to serialize containers of raw pointers to objects.
 *
 *	The pointee type that is being (de)serialized must provide the following methods:
 *		StatusCode Serialize(MessageBuilder &messageBuilder) const
 *		StatusCode Deserialize(MessageValidater &messageValidater)
 *
 *	unless the type is a builtin signed, unsigned or boolean type for which specializtions are provided.
 *
 *	@param	CONTAINER_TYPE type of container.
 *	@param	COUNT_TYPE type to use for container length in serialized form. Must be unsigned basic type.
 *	@param	MIN_LENGTH min allowed container length, default is 0.
 *	@param	MAX_LENGTH max allowed container length, default is max value of COUNT_TYPE.
 */
template <typename CONTAINER_TYPE, typename COUNT_TYPE, COUNT_TYPE MIN_LENGTH = 0, COUNT_TYPE MAX_LENGTH = boost::integer_traits<COUNT_TYPE>::const_max>
class SerializableSmartPointerContainerHelper : public SerializableContainerHelper<CONTAINER_TYPE, COUNT_TYPE, MIN_LENGTH, MAX_LENGTH, SerializableRawPointerHelper<typename CONTAINER_TYPE::value_type> >
{
};

} // namespace Activ

#endif // !defined (ACTIV_SERIALIZABLE_CONTAINER_HELPER_H)
