/**
 *  @file	RegistryHelper.h
 *
 *	@brief	Header file for the registry helper class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_REGISTRY_HELPER_H)
#define ACTIV_REGISTRY_HELPER_H

#include "ActivMiddleware/ActivBase/External.h"

namespace Activ
{

/**
 *	@brief	Registry helper class.
 *
 *	RegistryHelper wraps up methods for getting, setting and deleting registry values,
 *	deleting registry keys and enumerating keys and values.
 */
class ACTIV_BASE_API RegistryHelper
{
public:
	/**
	 *	@brief	Enumeration of basic types supported by RegistryHelper
	 */
	enum Type
	{
		TYPE_UNSUPPORTED,				///< Unsupported type.
		TYPE_UNSIGNED32,				///< Unsigned 32 bit type.
		TYPE_STRING,					///< 0 terminated string type.
		TYPE_BINARY,					///< Variable length binary type.
	};

	/**
	 *	@brief	Destructor.
	 */
	virtual ~RegistryHelper();

	/**
	 *	@brief	Get an UNSIGNED32 value.
	 *
	 *	@param	keyName the full name of the key holding the value to retrieve.
	 *	@param	valueName the name of the value within key \a keyName to retrieve.
	 *	@param	value on success, the retrieved value is stored in \a value.
	 *
	 *	@throw	OsException
	 *	@throw	BaseException
	 */
	static void GetValue(const char *keyName, const char *valueName, uint32_t &value);

	/**
	 *	@brief	Get a STRING value.
	 *
	 *	@param	keyName the full name of the key holding the value to retrieve.
	 *	@param	valueName the name of the value within key \a keyName to retrieve.
	 *	@param	value on success, the retrieved string is stored in \a value.
	 *	@param	maxValueLen the size of the buffer pointer to by \a value, including space for terminating \0 character.
	 *
	 *	@throw	OsException
	 *	@throw	BaseException
	 */
	static void GetValue(const char *keyName, const char *valueName, const char *value, uint_fast32_t maxValueLen);

	/**
	 *	@brief	Get a BINARY value.
	 *
	 *	@param	keyName the full name of the key holding the value to retrieve.
	 *	@param	valueName the name of the value within key \a keyName to retrieve.
	 *	@param	value on success, the retrieved buffer is stored in \a value.
	 *	@param	maxValueLen the size of the buffer pointer to by \a value.
	 *			On success, maxValueLen will contain the length of the retrieved buffer.
	 *
	 *	@throw	OsException
	 *	@throw	BaseException
	 */
	static void GetValue(const char *keyName, const char *valueName, const void *value, uint_fast32_t &maxValueLen);

	/**
	 *	@brief	Set an UNSIGNED32 value.
	 *
	 *	@param	keyName the full name of the key in which to set the value.
	 *	@param	valueName the name of the value within key \a keyName to set.
	 *	@param	value the UNSIGNED32 value to set.
	 *
	 *	@throw	OsException
	 */
	static void SetValue(const char *keyName, const char *valueName, uint32_t value);

	/**
	 *	@brief	Set a STRING value.
	 *
	 *	@param	keyName the full name of the key in which to set the value.
	 *	@param	valueName the name of the value within key \a keyName to set.
	 *	@param	value the string to set the value to.
	 *
	 *	@throw	OsException
	 */
	static void SetValue(const char *keyName, const char *valueName, const char *value);

	/**
	 *	@brief	Set a BINARY value.
	 *
	 *	@param	keyName the full name of the key in which to set the value.
	 *	@param	valueName the name of the value within key \a keyName to set.
	 *	@param	value pointer to the buffer to store.
	 *	@param	valueLen the length of the buffer pointer to by \a value.
	 *
	 *	@throw	OsException
	 */
	static void SetValue(const char *keyName, const char *valueName, const void *value, uint_fast32_t valueLen);
	
	/**
	 *	@brief	Delete a key and all sub keys.
	 *
	 *	@param	keyName the full name of the key to delete.
	 *
	 *	@throw	OsException
	 */
	static void DeleteKey(const char *keyName);

	/**
	 *	@brief	Delete a value from a key.
	 *
	 *	@param	keyName the full name of the key containing the value to delete.
	 *	@param	valueName the name of the value to delete.
	 *
	 *	@throw	OsException
	 */
	static void DeleteValue(const char *keyName, const char *valueName);

	/**
	 *	@brief	Enumerate all sub keys of a given key.
	 *
	 *	The OnKey() callback will be invoked for all sub keys of the given key.
	 *
	 *	@param	keyName the full name of the root key to enumerate from.
	 *
	 *	@return	the number of keys enumerated.
	 */
	uint_fast32_t EnumerateKeys(const char *keyName);

	/**
	 *	@brief	Enumerate all values of a given key.
	 *
	 *	The OnValue() callback will be invoked for all values within the given key.
	 *
	 *	@param	keyName the full name of the key to enumerate values from.
	 *
	 *	@return	the number of values enumerated.
	 */
	uint_fast32_t EnumerateValues(const char *keyName);

private:
	/**
	*	@brief	Registry key class.
	*/
	class RegistryKey
	{
	public:
		/**
		*	@brief	Default constructor.
		*/
		RegistryKey();

		/**
		*	@brief	Destructor.
		*/
		~RegistryKey();

		/**
		*	@brief	Address of operator.
		*/
		HKEY *operator &();

		/**
		*	@brief	Cast operator.
		*/
		operator HKEY() const;

	private:
		HKEY m_key;		///< Handle to the key.
	};

	/**
	 *	@brief	The callback invoked by EnumerateKeys().
	 *
	 *	@param	fullKeyName the full key name, including all parent keys.
	 *	@param	keyName the key name only.
	 *	@param	index the index of the key within it's parent key, 0 based.
	 *
	 *	@return	false to stop enumerating more keys, true to continue.
	 */
	virtual bool OnKey(const char *fullKeyName, const char *keyName, uint_fast32_t index);

	/**
	 *	@brief	The callback invoked by EnumerateValues().
	 *
	 *	@param	fullKeyName the full key name, including all parent keys.
	 *	@param	valueName the name of the enumerated value within \a fullKeyName.
	 *	@param	index the index of the value within it's key, 0 based.
	 *	@param	type the type of the value.
	 *
	 *	@return	false to stop enumerating more values, true to continue.
	 */
	virtual bool OnValue(const char *fullKeyName, const char *valueName, uint_fast32_t index, Type type);

	/**
	 *	@brief	Get a value.
	 *
	 *	@param	keyName the key name.
	 *	@param	valueName the name of the enumerated value within \a KeyName.
	 *	@param	type the type of the value.
	 *	@param	value the actual value.
	 *	@param	maxValueLen the length of the \a value.
	 */
	static void GetValue(const char *keyName, const char *valueName, uint32_t type, const void *value, uint_fast32_t &maxValueLen);

	/**
	 *	@brief	Set a value.
	 *
	 *	@param	keyName the key name.
	 *	@param	valueName the name of the enumerated value within \a KeyName.
	 *	@param	type the type of the value.
	 *	@param	value the actual value.
	 *	@param	maxValueLen the length of the \a value.
	 */
	static void SetValue(const char *keyName, const char *valueName, uint32_t type, const void *value, uint_fast32_t maxValueLen);

	/**
	 *	@brief	Map the OS value type to internal representation.
	 *
	 *	@param	type the OS type.
	 *
	 *	@retval	TYPE_UNSUPPORTED
	 *	@retval	TYPE_BINARY
	 *	@retval	TYPE_STRING
	 *	@retval	TYPE_UNSIGNED32
	 */
	static Type MapWinType(uint32_t type);
};

} // namespace Activ

#endif // !defined (ACTIV_REGISTRY_HELPER_H)
