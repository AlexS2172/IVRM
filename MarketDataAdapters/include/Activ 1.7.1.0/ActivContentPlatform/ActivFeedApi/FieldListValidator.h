/**
 *	@file	FieldListValidator.h
 *
 *	@brief	Header file for the field list validator class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_FEED_API_FIELD_LIST_VALIDATOR_H)
#define ACTIV_FEED_API_FIELD_LIST_VALIDATOR_H

#include "ActivContentPlatform/ActivFeedApi/External.h"
#include "ActivContentPlatform/ActivFeedCommon/FieldIds.h"

#include "boost/dynamic_bitset.hpp"

#include <vector>

namespace Activ
{

class MessageValidator;
class IFieldType;

namespace ContentPlatform
{

namespace FeedApi
{

/**
 *	@brief	Field list validator class.
 *
 *	Note this class is not thread safe. Do not make concurrent calls to its methods from multiple threads.
 */
class FieldListValidator
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
		ACTIV_FEED_API Field(const Feed::FieldId fieldId);

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
		 *	@throw	StatusCodeException, where m_errorCode is:
		 *			STATUS_CODE_UNDEFINED field is undefined
		 *			STATUS_CODE_NOT_FOUND field not present in record
		 *			STATUS_CODE_INVALID_FIELD_TYPE template type T does not match the type of the field
		 *			STATUS_CODE_ACCESS_DENIED not permissioned to see this field
	 	 *			STATUS_CODE_FAILURE some other failure
		 */
		template<typename T>
		const T *GetActivFieldType() const;

		typedef uint8_t Rules;										///< field rules type

		static const Rules RULE_NONE = 0x00;
		static const Rules RULE_DOES_NOT_UPDATE_LAST_VALUE = 0x01;	///< this is not the latest value for this field (e.g. a late trade)

		const Feed::FieldId	m_fieldId;								///< id of this field
		FieldStatus			m_fieldStatus;							///< status of this field
		Rules				m_rules;								///< rules for this field
		IFieldType *		m_pIFieldType;							///< field data, if FIELD_STATUS_DEFINED
	};

	/**
	 *	@brief	Const iterator class.
	 */
	class ConstIterator
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
		 *	@param	pFieldListValidator the underlying field list validator.
		 *	@param	iterator the iterator.
		 */
		ConstIterator(const FieldListValidator *pFieldListValidator, const Feed::FieldIdList::const_iterator &iterator);

		const FieldListValidator *			m_pFieldListValidator;	///< the underlying field list validator
		Feed::FieldIdList::const_iterator	m_iterator;				///< the iterator

		friend class FieldListValidator;
	};

	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_FEED_API FieldListValidator();

	/**
	 *	@brief	Copy constructor.
	 *
	 *	@param	rhs the value to assign to this object.
	 */
	ACTIV_FEED_API FieldListValidator(const FieldListValidator &rhs);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	fieldData reference to a FieldData object containing serialized field data.
	 *
	 *	@throw	StatusCodeException.
	 */
	ACTIV_FEED_API FieldListValidator(const FieldData &fieldData);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_FEED_API ~FieldListValidator();

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
	ACTIV_FEED_API StatusCode Initialize(const FieldData &fieldData);

	/**
	 *	@brief	Apply fields to this.
	 *
	 *	This call differs from an Initialize() call in that existing fields are not cleared.
	 *	Note fields in \a fieldData that have the rule RULE_DOES_NOT_UPDATE_LAST_VALUE will not be applied to this.
	 *
	 *	@param	fieldData the fields to apply.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	...
	 */
	ACTIV_FEED_API StatusCode ApplyUpdate(const FieldData &fieldData);

	/**
	 *	@brief	Apply the all fields in another FieldListValidator to this.
	 *
	 *	This call differs from an Initialize() call in that existing fields are not cleared.
	 *	Note fields in \a fieldListValidator that have the rule RULE_DOES_NOT_UPDATE_LAST_VALUE will not be applied to this.
	 *
	 *	@param	fieldListValidator the fields to apply.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_FORMAT
	 *	@retval	STATUS_CODE_HEAP_EMPTY
	 *	@retval	...
	 */
	ACTIV_FEED_API StatusCode ApplyUpdate(const FieldListValidator &fieldListValidator);

	/**
	 *	@brief	Reset.
	 *
	 *	Resets list of fields present in message.
	 */
	ACTIV_FEED_API void Reset();

	/**
	 *	@brief	Clear.
	 *
	 *	Clears all cached fields.
	 */
	ACTIV_FEED_API void Clear();
	
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
	 *	@retval STATUS_CODE_NOT_INITIALIZED FieldListValidator not initialized
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
	 *	@throw	StatusCodeException, where m_errorCode is:
	 *			STATUS_CODE_UNDEFINED field is undefined
	 *			STATUS_CODE_NOT_FOUND field not present in record
	 *			STATUS_CODE_INVALID_FIELD_TYPE template type T does not match the type of the field
	 *			STATUS_CODE_ACCESS_DENIED not permissioned to see this field
	 *			STATUS_CODE_NOT_INITIALIZED FieldListValidator not initialized
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
	 *	@brief	Is a field present?
	 *
	 *	@return	whether the specified field is present in the field list validator.
	 */
	bool IsFieldPresent(const Feed::FieldId fieldId) const;

	/**
	 *	@brief	Are the fields in this FieldListValidator a subset of the fields in rhs?
	 *
	 *	@return	True if *all* the fields present in this, are present in rhs
	 */
	bool IsSubset(const FieldListValidator &rhs) const;

	/**
	 *	@brief	Are any of the fields in this FieldListValidator present in rhs?
	 *
	 *	@return	True if *any* of the fields present in this, are present in rhs
	 */
	bool IsPartialSubset(const FieldListValidator &rhs) const;

	/**
	 *	@brief	Get an iterator to the first field in the message.
	 *
	 *	@return	an iterator object.
	 *
	 *	@throw	StatusCodeException
	 */
	ACTIV_FEED_API ConstIterator Begin() const;

	/**
	 *	@brief	Get an iterator that points to beyond the last field in the message.
	 *
	 *	@return	an iterator object.
	 *
	 *	@throw	StatusCodeException
	 */
	ACTIV_FEED_API ConstIterator End() const;

	/**
	 *	@brief	Assignment operator.
	 *
	 *	@param	rhs the value to assign to this object.
	 *
	 *	@return	a reference to this object.
	 */
	ACTIV_FEED_API FieldListValidator &operator=(const FieldListValidator &rhs);

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

	typedef boost::dynamic_bitset<> FieldIdPresentBitmap;			///< the present field id bitmap type definition
	typedef std::vector<CachedField *> CachedFieldList;				///< the cached field list type definition

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

	/**
	 *	@brief	Make m_fieldIdPresentBitmap the same size, extending the smaller one
	 */
	void MakeBitmapsSameSize(const FieldListValidator &rhs) const;

	bool							m_isInitialized;				///< indicates whether the object is initialized
	Feed::FieldIdList				m_fieldIdList;					///< sorted vector of field ids in message
	mutable FieldIdPresentBitmap	m_fieldIdPresentBitmap;			///< bitmap of field ids present in message.
	CachedFieldList					m_cachedFieldList;				///< the cached field list
	MessageValidator				m_messageValidator;				///< to validate serialized data
};

// ---------------------------------------------------------------------------------------------------------------------------------
	
inline size_t FieldListValidator::GetNumberOfFields() const
{
	return m_fieldIdList.size();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FieldListValidator::GetField(const Feed::FieldId fieldId, const Field* &pField) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	if (!IsFieldPresent(fieldId))
		return STATUS_CODE_NOT_FOUND;

	pField = &m_cachedFieldList[fieldId]->m_field;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
inline StatusCode FieldListValidator::GetActivFieldType(const Feed::FieldId fieldId, const T* &pFieldType) const
{
	if (!IsInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	if (!IsFieldPresent(fieldId))
		return STATUS_CODE_NOT_FOUND;

	return m_cachedFieldList[fieldId]->m_field.GetActivFieldType(pFieldType);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
const T *FieldListValidator::GetActivFieldType(const Feed::FieldId fieldId) const
{
	const T *pFieldType;

	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetActivFieldType(fieldId, pFieldType));

	return pFieldType;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldListValidator::IsInitialized() const
{
	return m_isInitialized;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldListValidator::IsFieldPresent(const Feed::FieldId fieldId) const
{
	return ((m_fieldIdPresentBitmap.size() > fieldId) && m_fieldIdPresentBitmap.test(fieldId));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldListValidator::IsSubset(const FieldListValidator &rhs) const
{
	// need to make bitsets same size before we can call is_subset_of()
	MakeBitmapsSameSize(rhs);

	return m_fieldIdPresentBitmap.is_subset_of(rhs.m_fieldIdPresentBitmap);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldListValidator::IsPartialSubset(const FieldListValidator &rhs) const
{
	// need to make bitsets same size before we can do bitwise AND
	MakeBitmapsSameSize(rhs);

	return (m_fieldIdPresentBitmap & rhs.m_fieldIdPresentBitmap).any();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void FieldListValidator::MakeBitmapsSameSize(const FieldListValidator &rhs) const
{
	FieldIdPresentBitmap::size_type ourSize = m_fieldIdPresentBitmap.size();
	FieldIdPresentBitmap::size_type theirSize = rhs.m_fieldIdPresentBitmap.size();

	if (ourSize < theirSize)
		m_fieldIdPresentBitmap.resize(theirSize, false);
	else if (theirSize < ourSize)
		rhs.m_fieldIdPresentBitmap.resize(ourSize, false);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T>
StatusCode FieldListValidator::Field::GetActivFieldType(const T* &pFieldType) const
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
const T *FieldListValidator::Field::GetActivFieldType() const
{
	const T *pFieldType;

	ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, GetActivFieldType(pFieldType));

	return pFieldType;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldListValidator::ConstIterator::ConstIterator() :
	m_pFieldListValidator(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldListValidator::ConstIterator &FieldListValidator::ConstIterator::operator++()
{
	++m_iterator;

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const FieldListValidator::ConstIterator FieldListValidator::ConstIterator::operator++(int)
{
	ConstIterator oldValue = *this;

	++(*this);

	return oldValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldListValidator::ConstIterator::operator==(const FieldListValidator::ConstIterator &rhs) const
{
	return ((m_pFieldListValidator == rhs.m_pFieldListValidator) && (m_iterator == rhs.m_iterator));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FieldListValidator::ConstIterator::operator!=(const FieldListValidator::ConstIterator &rhs) const
{
	return !((*this) == rhs);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline Feed::FieldId FieldListValidator::ConstIterator::GetFieldId() const
{
	return *m_iterator;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const FieldListValidator::Field &FieldListValidator::ConstIterator::GetField() const
{
	return m_pFieldListValidator->m_cachedFieldList[*m_iterator]->m_field;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldListValidator::ConstIterator::ConstIterator(const FieldListValidator *pFieldListValidator, const Feed::FieldIdList::const_iterator &iterator) :
	m_pFieldListValidator(pFieldListValidator),
	m_iterator(iterator)
{
}

} // namespace FeedApi

} // namespace ContentPlatform

} // namespace Activ

#endif // (!defined ACTIV_FEED_API_FIELD_LIST_VALIDATOR_H)
