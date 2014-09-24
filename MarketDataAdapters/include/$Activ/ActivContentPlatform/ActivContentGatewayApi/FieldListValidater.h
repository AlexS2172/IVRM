/**
 *	@file	FieldListValidater.h
 *
 *	@brief	Header file for the field list validater class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_CONTENT_GATEWAY_API_FIELD_LIST_VALIDATER_H)
#define ACTIV_CONTENT_GATEWAY_API_FIELD_LIST_VALIDATER_H

#include "ActivContentPlatform/ActivContentGatewayApi/External.h"
#include "ActivContentPlatform/ActivFeedCommon/FieldIds.h"

#include <map>
#include <vector>

namespace Activ
{

class MessageValidater;
class IFieldType;

namespace ContentPlatform
{

/**
 *	@brief	Field list validater class.
 *
 *	Note this class is not thread safe. Do not make concurrent calls to its methods from multiple threads.
 */
class FieldListValidater
{
public:
	/**
	 *	@brief	Field class.
	 */
	class Field
	{
	public:
		/**
		 *	@brief	Get activ field type.
		 *
		 *	@param	pFieldType reference to a pointer to a concrete Activ field type object.
		 *
		 *	@retval	STATUS_CODE_SUCCESS field is defined and pFieldType is set
		 *	@retval	STATUS_CODE_UNDEFINED field is undefined
		 *	@retval	STATUS_CODE_NOT_FOUND field not present in record
		 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE template type T does not match the type of the field
		 *	@retval	STATUS_CODE_ACCESS_DENIED not permissioned to see this field
	 	 *	@retval	STATUS_CODE_FAILURE
		 */
		template<typename T>
		StatusCode GetActivFieldType(T* &pFieldType) const;

		typedef uint8_t Rules;									///< field rules type

		static const Rules RULE_NONE = 0x00;
		static const Rules RULE_DO_NOT_UPDATE_CACHE = 0x01;		///< do not update a cached value for this field

		Feed::FieldId	m_fieldId;								///< id of this field
		FieldStatus		m_fieldStatus;							///< status of this field
		Rules			m_rules;								///< rules for this field
		IFieldType *	m_pIFieldType;							///< field data, if FIELD_STATUS_DEFINED
	};

private:
	typedef std::map<Feed::FieldId, Field> FieldMap;			///< the field map type definition

public:
	/**
	 *	@brief	Const iterator class.
	 */
	ACTIV_DECLARE_NESTED_CLASS(ConstIterator)
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ACTIV_CONTENT_GATEWAY_API ConstIterator();

		/**
		 *	@brief	Prefix increment operator.
		 *
		 *	@return	a reference to the updated object.
		 */
		ACTIV_CONTENT_GATEWAY_API ConstIterator &operator++();

		/**
		 *	@brief	Postfix increment operator.
		 *
		 *	@return	a copy of the original object.
		 */
		ACTIV_CONTENT_GATEWAY_API const ConstIterator operator++(int);

		/**
		 *	@brief	Equality operator.
		 *
		 *	@param	rhs the object to test against.
		 *
		 *	@return	whether the two objects are equal.
		 */
		ACTIV_CONTENT_GATEWAY_API bool operator==(const ConstIterator &rhs) const;

		/**
		 *	@brief	Inequality operator.
		 *
		 *	@param	rhs the object to test against.
		 *
		 *	@return	whether the two objects are not equal.
		 */
		ACTIV_CONTENT_GATEWAY_API bool operator!=(const ConstIterator &rhs) const;

		/**
		 *	@brief	Get the field id.
		 *
		 *	@return	the field id.
		 */
		ACTIV_CONTENT_GATEWAY_API Feed::FieldId GetFieldId() const;

		/**
		 *	@brief	Get the field.
		 *
		 *	@return	a reference to the field.
		 */
		ACTIV_CONTENT_GATEWAY_API const Field &GetField() const;

	private:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	pFieldListValidater the underlying field list validater.
		 *	@param	iterator the iterator.
		 */
		ConstIterator(const FieldListValidater *pFieldListValidater, const FieldMap::const_iterator &iterator);

		const FieldListValidater *	m_pFieldListValidater;		///< the underlying field list validater
		FieldMap::const_iterator	m_iterator;					///< the iterator

		friend class FieldListValidater;
	};

	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_CONTENT_GATEWAY_API FieldListValidater();

	/**
	 *	@brief	Copy constructor.
	 *
	 *	@param	rhs the value to assign to this object.
	 */
	ACTIV_CONTENT_GATEWAY_API FieldListValidater(const FieldListValidater &rhs);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	fieldData reference to a FieldData object containing serialized field data.
	 *
	 *	@throw	ActivException.
	 */
	ACTIV_CONTENT_GATEWAY_API FieldListValidater(const FieldData &fieldData);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_CONTENT_GATEWAY_API ~FieldListValidater();

	/**
	 *	@brief	Initialize.
	 *
	 *	This call clears any fields currently in this object. New fields are then added from the
	 *	\a fieldData object.
	 *
	 *	@param	fieldData reference to a FieldData object containing serialized field data.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_API StatusCode Initialize(const FieldData &fieldData);

	/**
	 *	@brief	Apply fields to this.
	 *
	 *	This call differs from an Initialize() call in that existing fields are not cleared.
	 *	Note fields in \a fieldData that have the rule RULE_DO_NOT_UPDATE_CACHE will not be applied to this.
	 *
	 *	@param	fieldData the fields to apply.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_API StatusCode ApplyUpdate(const FieldData &fieldData);

	/**
	 *	@brief	Apply the all fields in another FieldListValidater to this.
	 *
	 *	This call differs from an Initialize() call in that existing fields are not cleared.
	 *	Note fields in \a fieldListValidater that have the rule RULE_DO_NOT_UPDATE_CACHE will not be applied to this.
	 *
	 *	@param	fieldListValidater the fields to apply.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	...
	 */
	ACTIV_CONTENT_GATEWAY_API StatusCode ApplyUpdate(const FieldListValidater &fieldListValidater);

	/**
	 *	@brief	Reset. Clears field map.
	 */
	ACTIV_CONTENT_GATEWAY_API void Reset();

	/**
	 *	@brief	Clear. Clears field map and IFieldType cache.
	 */
	ACTIV_CONTENT_GATEWAY_API void Clear();

	/**
	 *	@brief	Get a field.
	 *
	 *	@param	fieldId the field id.
	 *	@param	pField a pointer to the field.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_NOT_FOUND
	 *	@retval STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_CONTENT_GATEWAY_API StatusCode GetField(const Feed::FieldId fieldId, const Field* &pField) const;

	/**
	 *	@brief	Get activ field type.
	 *
	 *	@param	fieldId the field id.
	 *	@param	pFieldType reference to a pointer to a concrete Activ field type object.
	 *
	 *	@retval	STATUS_CODE_SUCCESS field is defined and pFieldType is set
	 *	@retval	STATUS_CODE_UNDEFINED field is undefined
	 *	@retval	STATUS_CODE_NOT_FOUND field not present
	 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE template type T does not match the type of the field
	 *	@retval	STATUS_CODE_ACCESS_DENIED not permissioned to see this field
	 *	@retval STATUS_CODE_NOT_INITIALIZED FieldListValidater not initialized
	 *	@retval	STATUS_CODE_FAILURE
	 */
	template<typename T>
	StatusCode GetActivFieldType(const Feed::FieldId fieldId, T* &pFieldType) const;

	/**
	 *	@brief	Is the object initialized.
	 *
	 *	@return	whether the object is initialized.
	 */
	ACTIV_CONTENT_GATEWAY_API bool IsInitialized() const;

	/**
	 *	@brief	Get an iterator to the first field in the message.
	 *
	 *	@return	an iterator object.
	 *
	 *	@throw	ActivException
	 */
	ACTIV_CONTENT_GATEWAY_API ConstIterator Begin() const;

	/**
	 *	@brief	Get an iterator that points to beyond the last field in the message.
	 *
	 *	@return	an iterator object.
	 *
	 *	@throw	ActivException
	 */
	ACTIV_CONTENT_GATEWAY_API ConstIterator End() const;

	/**
	 *	@brief	Assignment operator.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@return	a reference to this object.
	 */
	ACTIV_CONTENT_GATEWAY_API FieldListValidater &operator=(const FieldListValidater &rhs);

private:
	typedef std::vector<IFieldType *> CachedFieldList;			///< the cached field list type definition

	/**
	 *	@brief	Get a field.
	 *
	 *	This is to prevent VC++ automatically casting away the constness.
	 *
	 *	@param	fieldId the field id.
	 *	@param	pIFieldType a pointer to the field.
	 *
	 *	@retval STATUS_CODE_NOT_IMPLEMENTED
	 */
	StatusCode GetField(const Feed::FieldId fieldId, Field* &pField) const;

	/**
	 *	@brief	Initialize a field from serialized data.
	 *
	 *	@param	isFieldRulesPresent.
	 *	@param	isFieldDefined.
	 *	@param	field the field to initialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode InitializeField(const bool isFieldRulesPreset, const bool isFieldDefined, Field &field);

	/**
	 *	@brief	Update a field from serialized data.
	 *
	 *	Field is not updated if do not update rule is set.
	 *
	 *	@param	isFieldRulesPresent.
	 *	@param	isFieldDefined.
	 *	@param	field the field to update.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode UpdateField(const bool isFieldRulesPreset, const bool isFieldDefined, Field &field);

	/**
	 *	@brief	Initialize a field from an existing field.
	 *
	 *	@param	sourceField the field to initialize from.
	 *	@param	targetField the field to initialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode UpdateField(const Field &sourceField, Field &targetField);

	bool					m_isInitialized;					///< indicates whether the object is initialized
	FieldMap				m_fieldMap;							///< the field map
	FieldMap::value_type	m_fieldMapValue;					///< to save ctor cost on insert into \a m_fieldMap
	CachedFieldList			m_cachedFieldList;					///< the cached field list
	MessageValidater		m_messageValidater;					///< to validate serialized data
};

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
StatusCode FieldListValidater::GetActivFieldType(const Feed::FieldId fieldId, T* &pFieldType) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	const FieldMap::const_iterator iterator = m_fieldMap.find(fieldId);

	if (m_fieldMap.end() == iterator)
		return STATUS_CODE_NOT_FOUND;

	return iterator->second.GetActivFieldType(pFieldType);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
StatusCode FieldListValidater::Field::GetActivFieldType(T* &pFieldType) const
{
	switch (m_fieldStatus)
	{
		case FIELD_STATUS_DEFINED:
			if (T::FIELD_TYPE != m_pIFieldType->GetType())
				return STATUS_CODE_INVALID_FIELD_TYPE;

			pFieldType = static_cast<T *>(m_pIFieldType);
			return STATUS_CODE_SUCCESS;

		case FIELD_STATUS_UNDEFINED:
			return STATUS_CODE_UNDEFINED_FIELD;

		case FIELD_STATUS_NOT_FOUND:
			return STATUS_CODE_NOT_FOUND;

		case FIELD_STATUS_NOT_PERMISSIONED:
			return STATUS_CODE_ACCESS_DENIED;

		default:
			return STATUS_CODE_FAILURE;
	}
}

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined(ACTIV_CONTENT_GATEWAY_API_FIELD_LIST_VALIDATER_H)
