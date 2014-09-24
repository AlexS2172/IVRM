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

#include "boost/dynamic_bitset.hpp"

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
		 *	@brief	Constructor.
		 *
		 *	@param	fieldId.
		 */
		ACTIV_CONTENT_GATEWAY_API Field(const Feed::FieldId fieldId);

		/**
		 *	@brief	Get Activ field type.
		 *
		 *	@param	pFieldType reference to a const pointer to a concrete Activ field type object.
		 *
		 *	@retval	STATUS_CODE_SUCCESS field is defined and pFieldType is set
		 *	@retval	STATUS_CODE_UNDEFINED field is undefined
		 *	@retval	STATUS_CODE_NOT_FOUND field not present in record
		 *	@retval	STATUS_CODE_INVALID_FIELD_TYPE template type T does not match the type of the field
		 *	@retval	STATUS_CODE_ACCESS_DENIED not permissioned to see this field
	 	 *	@retval	STATUS_CODE_FAILURE some other failure
		 */
		template<typename T>
		StatusCode GetActivFieldType(const T* &pFieldType) const;

		/**
		 *	@brief	Get Activ field type.
		 *
		 *	@return	Const pointer to a concrete Activ field type object.
		 *
		 *	@throw	ActivException, where m_errorCode is:
		 *			STATUS_CODE_UNDEFINED field is undefined
		 *			STATUS_CODE_NOT_FOUND field not present in record
		 *			STATUS_CODE_INVALID_FIELD_TYPE template type T does not match the type of the field
		 *			STATUS_CODE_ACCESS_DENIED not permissioned to see this field
	 	 *			STATUS_CODE_FAILURE some other failure
		 */
		template<typename T>
		const T *GetActivFieldType() const;

		typedef uint8_t Rules;									///< field rules type

		static const Rules RULE_NONE = 0x00;
		static const Rules RULE_DO_NOT_UPDATE_LAST_VALUE = 0x01;///< do not update a cached "last" value for this field (e.g. for a late trade)

		const Feed::FieldId	m_fieldId;							///< id of this field
		FieldStatus			m_fieldStatus;						///< status of this field
		Rules				m_rules;							///< rules for this field
		IFieldType *		m_pIFieldType;						///< field data, if FIELD_STATUS_DEFINED
	};

	/**
	 *	@brief	Const iterator class.
	 */
	ACTIV_DECLARE_NESTED_CLASS(ConstIterator)
	{
	public:
		/**
		 *	@brief	Default constructor.
		 */
		ConstIterator();

		/**
		 *	@brief	Prefix increment operator.
		 *
		 *	@return	a reference to the updated object.
		 */
		ConstIterator &operator++();

		/**
		 *	@brief	Postfix increment operator.
		 *
		 *	@return	a copy of the original object.
		 */
		const ConstIterator operator++(int);

		/**
		 *	@brief	Equality operator.
		 *
		 *	@param	rhs the object to test against.
		 *
		 *	@return	whether the two objects are equal.
		 */
		bool operator==(const ConstIterator &rhs) const;

		/**
		 *	@brief	Inequality operator.
		 *
		 *	@param	rhs the object to test against.
		 *
		 *	@return	whether the two objects are not equal.
		 */
		bool operator!=(const ConstIterator &rhs) const;

		/**
		 *	@brief	Get the field id.
		 *
		 *	@return	the field id.
		 */
		Feed::FieldId GetFieldId() const;

		/**
		 *	@brief	Get the field.
		 *
		 *	@return	a reference to the field.
		 */
		const Field &GetField() const;

	private:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	pFieldListValidater the underlying field list validater.
		 *	@param	iterator the iterator.
		 */
		ConstIterator(const FieldListValidater *pFieldListValidater, const Feed::FieldIdList::const_iterator &iterator);

		const FieldListValidater *			m_pFieldListValidater;	///< the underlying field list validater
		Feed::FieldIdList::const_iterator	m_iterator;				///< the iterator

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
	 *	Note fields in \a fieldData that have the rule RULE_DO_NOT_UPDATE_LAST_VALUE will not be applied to this.
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
	 *	Note fields in \a fieldListValidater that have the rule RULE_DO_NOT_UPDATE_LAST_VALUE will not be applied to this.
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
	 *	@brief	Reset.
	 *
	 *	Resets list of fields present in message.
	 */
	ACTIV_CONTENT_GATEWAY_API void Reset();

	/**
	 *	@brief	Clear.
	 *
	 *	Clears all cached fields.
	 */
	ACTIV_CONTENT_GATEWAY_API void Clear();
	
	/**
	 *	@brief	Get the number of fields present.
	 *
	 *	@return	Number of fields in the object.
	 */
	size_t GetNumberOfFields() const;

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
	StatusCode GetField(const Feed::FieldId fieldId, const Field* &pField) const;

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
	StatusCode GetActivFieldType(const Feed::FieldId fieldId, const T* &pFieldType) const;

	/**
	 *	@brief	Get Activ field type.
	 *
	 *	@param	fieldId the field id.
	 *
	 *	@return	Const pointer to a concrete Activ field type object.
	 *
	 *	@throw	ActivException, where m_errorCode is:
	 *			STATUS_CODE_UNDEFINED field is undefined
	 *			STATUS_CODE_NOT_FOUND field not present in record
	 *			STATUS_CODE_INVALID_FIELD_TYPE template type T does not match the type of the field
	 *			STATUS_CODE_ACCESS_DENIED not permissioned to see this field
	 *			STATUS_CODE_NOT_INITIALIZED FieldListValidater not initialized
	 *			STATUS_CODE_FAILURE some other failure
	 */
	template<typename T>
	const T *GetActivFieldType(const Feed::FieldId fieldId) const;

	/**
	 *	@brief	Is the object initialized.
	 *
	 *	@return	whether the object is initialized.
	 */
	bool IsInitialized() const;

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
	class CachedField
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	fieldId.
		 */
		CachedField(const Feed::FieldId fieldId);

		/**
		 *	@brief	Destructor.
		 */
		~CachedField();

		Field		m_field;
		IFieldType *m_pIFieldType;
	};

	typedef boost::dynamic_bitset<> FieldIdPresentBitmap;		///< the present field id bitmap type definition
	typedef std::vector<CachedField *> CachedFieldList;			///< the cached field list type definition

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
	 *	@brief	Get cached field.
	 *
	 *	Extends cached field list as necessary.
	 *
	 *	@param	fieldId.
	 *
	 *	@return	Reference to cached field for @a fieldId.
	 */
	CachedField &GetCachedField(const Feed::FieldId fieldId);

	/**
	 *	@brief	Update a field from serialized data.
	 *
	 *	@param	isFieldRulesPresent.
	 *	@param	isFieldDefined.
	 *	@param	shouldSkipFields.
	 *	@param	cachedField the field to update.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode UpdateField(const bool isFieldRulesPreset, const bool isFieldDefined, const bool shouldSkipFields, CachedField &cachedField);

	/**
	 *	@brief	Update a field from an existing field.
	 *
	 *	@param	sourceCachedField the field to initialize from.
	 *	@param	targetCachedField the field to initialize.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode UpdateField(const CachedField &sourceCachedField, CachedField &targetCachedField);

	bool					m_isInitialized;					///< indicates whether the object is initialized
	Feed::FieldIdList		m_fieldIdList;						///< sorted vector of field ids in message
	FieldIdPresentBitmap	m_fieldIdPresentBitmap;				///< bitmap of field ids present in message.
	CachedFieldList			m_cachedFieldList;					///< the cached field list
	MessageValidater		m_messageValidater;					///< to validate serialized data
};

// ---------------------------------------------------------------------------------------------------------------------------------
	
inline size_t FieldListValidater::GetNumberOfFields() const
{
	return m_fieldIdList.size();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListValidater::GetField(const Feed::FieldId fieldId, const Field* &pField) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	if ((fieldId >= m_fieldIdPresentBitmap.size()) || !m_fieldIdPresentBitmap.test(fieldId))
		return STATUS_CODE_NOT_FOUND;

	pField = &m_cachedFieldList[fieldId]->m_field;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
inline StatusCode FieldListValidater::GetActivFieldType(const Feed::FieldId fieldId, const T* &pFieldType) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	if ((fieldId >= m_fieldIdPresentBitmap.size()) || !m_fieldIdPresentBitmap.test(fieldId))
		return STATUS_CODE_NOT_FOUND;

	return m_cachedFieldList[fieldId]->m_field.GetActivFieldType(pFieldType);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
const T *FieldListValidater::GetActivFieldType(const Feed::FieldId fieldId) const
{
	const T *pFieldType;

	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetActivFieldType(fieldId, pFieldType));

	return pFieldType;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldListValidater::IsInitialized() const
{
	return m_isInitialized;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
StatusCode FieldListValidater::Field::GetActivFieldType(const T* &pFieldType) const
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

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
const T *FieldListValidater::Field::GetActivFieldType() const
{
	const T *pFieldType;

	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetActivFieldType(pFieldType));

	return pFieldType;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldListValidater::ConstIterator::ConstIterator() :
	m_pFieldListValidater(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldListValidater::ConstIterator &FieldListValidater::ConstIterator::operator++()
{
	++m_iterator;

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const FieldListValidater::ConstIterator FieldListValidater::ConstIterator::operator++(int)
{
	ConstIterator oldValue = *this;

	++(*this);

	return oldValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldListValidater::ConstIterator::operator==(const FieldListValidater::ConstIterator &rhs) const
{
	return ((m_pFieldListValidater == rhs.m_pFieldListValidater) && (m_iterator == rhs.m_iterator));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldListValidater::ConstIterator::operator!=(const FieldListValidater::ConstIterator &rhs) const
{
	return !((*this) == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Feed::FieldId FieldListValidater::ConstIterator::GetFieldId() const
{
	return *m_iterator;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const FieldListValidater::Field &FieldListValidater::ConstIterator::GetField() const
{
	return m_pFieldListValidater->m_cachedFieldList[*m_iterator]->m_field;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldListValidater::ConstIterator::ConstIterator(const FieldListValidater *pFieldListValidater, const Feed::FieldIdList::const_iterator &iterator) :
	m_pFieldListValidater(pFieldListValidater),
	m_iterator(iterator)
{
}

} // namespace ContentPlatform

} // namespace Activ

#endif // !defined(ACTIV_CONTENT_GATEWAY_API_FIELD_LIST_VALIDATER_H)
