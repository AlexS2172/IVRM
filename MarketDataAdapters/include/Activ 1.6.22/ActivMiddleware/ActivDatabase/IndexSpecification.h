/**
 *  @file	IndexSpecification.h
 *
 *	@brief	Header file for the index specification.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_INDEX_SPECIFICATION_H)
#define ACTIV_DATABASE_INDEX_SPECIFICATION_H

#include "ActivMiddleware/ActivDatabase/External.h"

#include "ActivMiddleware/ActivDatabase/FieldSpecification.h"

#include <string>
#include <vector>
#include <iostream>

namespace Activ
{

class MessageBuilder;
class MessageValidater;

namespace Database
{

/**
 *	@brief	Declaration of an index specification.
 */
#define ACTIV_DATABASE_INDEX_FIELD(id, properties)																Activ::Database::IndexSpecification::Field(id, Activ::Database::IndexSpecification::Field::properties)														///< Initializer for an index specification field.
#define ACTIV_DATABASE_INDEX_ENUM(number, name, type, properties, fieldList)									number,																																						///< Enumeration of index numbers.
#define ACTIV_DATABASE_INDEX_ENUM_WITH_CACHE(number, name, type, properties, fieldList, fieldIdList)			number,																																						///< Enumeration of index numbers.
#define ACTIV_DATABASE_INDEX_SPECIFICATION(number, name, type, properties, fieldList)							Activ::Database::IndexSpecification(name, Activ::Database::IndexSpecification::type, Activ::Database::IndexSpecification::properties, std::vector<Activ::Database::IndexSpecification::Field>(fieldList, fieldList + ACTIV_ARRAY_SIZE(fieldList))),																															///< Initializer for index specification structures.
#define ACTIV_DATABASE_INDEX_SPECIFICATION_WITH_CACHE(number, name, type, properties, fieldList, fieldIdList)	Activ::Database::IndexSpecification(name, Activ::Database::IndexSpecification::type, Activ::Database::IndexSpecification::properties, std::vector<Activ::Database::IndexSpecification::Field>(fieldList, fieldList + ACTIV_ARRAY_SIZE(fieldList)), std::vector<Activ::Database::FieldSpecification::Id>(fieldIdList, fieldIdList + ACTIV_ARRAY_SIZE(fieldIdList))),			///< Initializer for index specification structures.

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Index Specification structure.
 */
class IndexSpecification
{
public:
	typedef uint16_t Number;		///< Unique number for an index.

	/**
	 *	@brief	Type of the index.
	 */
	enum Type
	{
		TYPE_NONE,
		TYPE_HASH,
		TYPE_RED_BLACK_TREE
	};

	/**
	 *	@brief	Properties of the index.
	 */
	enum
	{
		PROPERTY_NONE = 0x00000001,
		PROPERTY_UNIQUE = 0x00000002,
		PROPERTY_DUPLICATE = 0x00000004,
		PROPERTY_PRIMARY_UNIQUE = 0x00000008
	};
	typedef uint32_t Properties;		///< Definition of the index properties.

	/**
	 *	@brief	Index field specification structure.
	 */
	class ACTIV_DATABASE_API Field
	{
	public:
		/**
		 *	@brief	Index Field Properties.
		 */
		enum
		{
			PROPERTY_NONE = 0x00000000,
			PROPERTY_ASCENDING = PROPERTY_NONE,
			PROPERTY_DESCENDING = 0x80000000
		};
		typedef uint32_t Properties;	///< Field properties type definition.
		
		/**
		 *	@brief	Constructor.
		 */
		Field();

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	fieldId the identifier of the key field.
		 *	@param	properties properties of the key field.
		 */
		explicit Field(const FieldSpecification::Id fieldId, const Properties properties = PROPERTY_NONE);

		/**
		 *	@brief	Equality operator.
		 *
		 *	@param	rhs the object to compare with.
		 *
		 *	@return	the result of the test.
		 */
		bool operator==(const Field &rhs) const;

		/**
		 *	@brief	Inequality operator.
		 *
		 *	@param	rhs the object to compare with.
		 *
		 *	@return	the result of the test.
		 */
		bool operator!=(const Field &rhs) const;

		/**
		 *	@brief	Serialize the index field specification into a message.
		 *
		 *	@param	messageBuilder the message builder used for constructing the message.
		 *
		 *	@retval STATUS_CODE_SUCCESS
		 *	@retval STATUS_CODE_INVALID_PARAMETER
		 *	@retval	STATUS_CODE_NOT_INITIALIZED
		 *	@retval STATUS_CODE_FAILURE
		 */
		StatusCode Serialize(MessageBuilder &messageBuilder) const;

		/**
		 *	@brief	Deserialize an index field specification from a message.
		 *
		 *	@param	messageValidater the message validater from which the field specification is extracted.
		 *
		 *	@retval	STATUS_CODE_SUCCESS
		 *	@retval	STATUS_CODE_INVALID_PARAMETER
		 *	@retval STATUS_CODE_HEAP_EMPTY
		 *	@retval	STATUS_CODE_NOT_INITIALIZED
		 */
		StatusCode Deserialize(MessageValidater &messageValidater);

		/**
		 *	@brief	Is ascending.
		 *
		 *	@param	properties the field properties to check.
		 *
		 *	@return	true if the field should be ordered in ascending value.
		 */
		static bool IsAscending(const Properties properties);

		/**
		 *	@brief	Is descending.
		 *
		 *	@param	properties the field properties to check.
		 *
		 *	@return	true if the field should be ordered in descending value.
		 */
		static bool IsDescending(const Properties properties);

		FieldSpecification::Id	m_fieldId;					///< Field identifier for matching field within the record field list.
		Properties				m_properties;				///< Properties.
	};

	/**
	 *	@brief	Constructor.
	 */
	ACTIV_DATABASE_API IndexSpecification();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	name name of index.
	 *	@param	type index type.
	 *	@param	properties index properties.
	 *	@param	indexFieldSpecificationList list of index field specifications.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API IndexSpecification(const std::string &name, const Type type, const Properties properties, const std::vector<IndexSpecification::Field> &indexFieldSpecificationList);

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	name name of index.
	 *	@param	type index type.
	 *	@param	properties index properties.
	 *	@param	indexFieldSpecificationList list of index field specifications.
	 *	@param	cachedFieldIdList list of ids of cached fields.
	 *
	 *	@throw	DatabaseException
	 */
	IndexSpecification(const std::string &name, const Type type, const Properties properties, const std::vector<IndexSpecification::Field> &indexFieldSpecificationList, const std::vector<uint16_t> &cachedFieldIdList);

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_DATABASE_API bool operator==(const IndexSpecification &rhs) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 */
	ACTIV_DATABASE_API bool operator!=(const IndexSpecification &rhs) const;

	/**
	 *	@brief	Less than operator.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	the result of the test.
	 *
	 *	@throw	ActivException
	 */
	bool operator<(const IndexSpecification &rhs) const;

	/**
	 *	@brief	Validate the index specification.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API void Validate() const;

	/**
	 *	@brief	Serialize the index specification into a message.
	 *
	 *	@param	messageBuilder the message builder used for constructing the message.
	 *
	 *	@retval STATUS_CODE_SUCCESS
	 *	@retval STATUS_CODE_INVALID_PARAMETER
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 *	@retval STATUS_CODE_FAILURE
	 */
	ACTIV_DATABASE_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize an index specification from a message.
	 *
	 *	@param	messageValidater the message validater from which the specification is extracted.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_INVALID_PARAMETER
	 *	@retval STATUS_CODE_HEAP_EMPTY
	 *	@retval	STATUS_CODE_NOT_INITIALIZED
	 */
	ACTIV_DATABASE_API StatusCode Deserialize(MessageValidater &messageValidater);

	/**
	 *	@brief	Is this the primary unique index.
	 *
	 *	@return	true if this is the primary unqiue index, false otherwise.
	 */
	ACTIV_DATABASE_API bool IsPrimaryUnique() const;

	/**
	 *	@brief	Is this a unique index.
	 *
	 *	@return	true if added keys have to be unique, false otherwise.
	 */
	ACTIV_DATABASE_API bool IsUnique() const;

	/**
	 *	@brief	Is this a duplicate index.
	 *
	 *	@return	true if added keys have can be duplicated in the index, false otherwise.
	 */
	ACTIV_DATABASE_API bool IsDuplicate() const;

	/**
	 *	@brief	Is this a hash index.
	 *
	 *	@return	true if this is a hash index, false otherwise.
	 */
	ACTIV_DATABASE_API bool IsHash() const;

	/**
	 *	@brief	Is this a tree index.
	 *
	 *	@return	true if this is a tree index, false otherwise.
	 */
	ACTIV_DATABASE_API bool IsTree() const;

	/**
	 *	@brief	Is a match.
	 *
	 *	@param	rhs the object to compare with.
	 *
	 *	@return	true if this index matches the criteria specified in the supplied index specification.
	 */
	ACTIV_DATABASE_API bool IsMatch(const IndexSpecification &rhs) const;

	/**
	 *	@brief	Convert the specification to a C-style string.
	 *
	 *	@return	the specification as a string.
	 */
	ACTIV_DATABASE_API std::string ToString() const;

	static const Number		UNDEFINED_INDEX_NUMBER = ~0;	///< Reserved value used to identify undefined index identifier.

	Number									m_number;						///< Number of the index.
	std::string								m_name;							///< Name of the index.
	Type									m_type;							///< Type of the index.
	Properties								m_properties;					///< Properties of the index.
	std::vector<Field>						m_indexFieldSpecificationList;	///< List of fields in the index.
	std::vector<FieldSpecification::Id>		m_cachedFieldIdList;			///< List of ids of cached fields.

private:
	typedef std::vector<Field>	FieldList;									///< Definition of a list of index fields.

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	number the number of the index.
	 *	@param	name name of index.
	 *	@param	type index type.
	 *	@param	properties index properties.
	 *	@param	indexFieldSpecificationList list of index field specifications.
	 *	@param	cachedFieldIdList list of ids of cached fields.
	 *
	 *	@throw	DatabaseException
	 */
	ACTIV_DATABASE_API IndexSpecification(const IndexSpecification::Number number, const std::string &name, const Type type, const Properties properties, const std::vector<IndexSpecification::Field> &indexFieldSpecificationList, const std::vector<FieldSpecification::Id> &cachedFieldIdList);

	friend class Index;
};

// ---------------------------------------------------------------------------------------------------------------------------------

}	// namespace Database

}	// namespace Activ

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Global operator<< to dump an index specification to an ostream.
 *
 *	@param	os the ostream to write the index specification to.
 *	@param	rhs the index specification to write.
 *
 *	@return	the ostream \a os.
 */
ACTIV_DATABASE_API std::ostream &operator<<(std::ostream &os, const Activ::Database::IndexSpecification &rhs);

// ---------------------------------------------------------------------------------------------------------------------------------

#endif	// !defined(ACTIV_DATABASE_INDEX_SPECIFICATION_H)
