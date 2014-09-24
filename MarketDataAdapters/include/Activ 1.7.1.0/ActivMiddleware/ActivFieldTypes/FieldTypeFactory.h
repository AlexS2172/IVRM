/**
 *	@file	FieldTypeFactory.h
 *
 *	@brief	Header file for the field type factory class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_FIELD_TYPE_FACTORY_H)
#define ACTIV_FIELD_TYPE_FACTORY_H

#include "ActivMiddleware/ActivFieldTypes/External.h"
#include "ActivMiddleware/ActivFieldTypes/ActivFieldTypes.h"

namespace Activ
{

class MessageValidator;
class IFieldType;

/**
 *	@brief	Field type factory class.
 */
class FieldTypeFactory
{
public:
	/**
	 *	@brief	Create a field type object.
	 *
	 *	@param	fieldType the field type.
	 *
	 *	@return	a pointer to the created object (0 on failure).
	 */
	ACTIV_FIELD_TYPES_API IFieldType *Create(const FieldType fieldType) const;

	/**
	 *	@brief	Clone an existing field type object.
	 *
	 *	@param	pFieldType the object to clone.
	 *
	 *	@return	a pointer to the cloned object (0 on failure).
	 */
	ACTIV_FIELD_TYPES_API IFieldType *Clone(const IFieldType * const pIFieldType) const;

	/**
	 *	@brief	Convert an existing field type object.
	 *
	 *	@param	fieldType the field type.
	 *	@param	pFieldType the object to clone.
	 *
	 *	@return	a pointer to the converted object (0 on failure).
	 */
	ACTIV_FIELD_TYPES_API IFieldType *Convert(const FieldType fieldType, const IFieldType * const pIFieldType) const;

	/**
	 *	@brief	Get a reference to the singleton factory object.
	 *
	 *	@return	Reference to the singleton.
	 */
	static FieldTypeFactory &GetInstance();

private:
	/**
	 *	@brief	Default constructor.
	 */
	FieldTypeFactory();	

	ACTIV_FIELD_TYPES_API static FieldTypeFactory	m_factory;	///< Singleton field type factory.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline FieldTypeFactory &FieldTypeFactory::GetInstance()
{
	return m_factory;
}

} // namespace Activ

#endif // !defined (ACTIV_FIELD_TYPE_FACTORY_H)
