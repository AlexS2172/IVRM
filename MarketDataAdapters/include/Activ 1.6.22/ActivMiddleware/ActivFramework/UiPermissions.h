/**
 *	@file	UiPermissions.h
 *
 *	@brief	Header file for the ui permissions class..
 *
 *	$Log: $
 */

#if !defined (ACTIV_UI_PERMISSIONS_H)
#define ACTIV_UI_PERMISSIONS_H

#include "ActivMiddleware/ActivFramework/External.h"

namespace Activ
{
	
/**
 *	@brief	Ui permissions class.
 */
class UiPermissions
{
public:
	/**
	 *	@brief	Ui permissions list.
	 */
	#define UI_PERMISSIONS_LIST(d)								\
		d(READ,		READ_BIT)									\
		d(MODIFY,	(READ_BIT | MODIFY_BIT))					\
		d(SHUTDOWN,	(READ_BIT | SHUTDOWN_BIT))					\
		d(FULL,		((0x01 << NUMBER_OF_PERMISSIONS_BITS) - 1))

	#define DECLARE_UI_PERMISSIONS(name, bitmap) ACTIV_FRAMEWORK_API static const UiPermissions name;

	UI_PERMISSIONS_LIST(DECLARE_UI_PERMISSIONS)

	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_FRAMEWORK_API UiPermissions();

	/**
	 *	@brief	Convert the permissions value from a string.
	 *
	 *	@param	string the string to convert from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 */
	ACTIV_FRAMEWORK_API StatusCode FromString(const std::string &string);

	/**
	 *	@brief	Convert the permissions value to a string.
	 *
	 *	@return	The converted string representation.
	 */
	ACTIV_FRAMEWORK_API std::string ToString() const;

	/**
	 *	@brief	Serialize.
	 *
	 *	@param	messageBuilder message builder.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize.
	 *
	 *	@param	messageValidater message validater.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API StatusCode Deserialize(MessageValidater &messageValidater);

	/**
	 *	@brief	Check the permissions meets another permissions value.
	 *
	 *	@param	uiPermissions to compare with.
	 *
	 *	@return	true if this permissions meets the other permissions value.
	 */
	ACTIV_FRAMEWORK_API bool HasPermissions(const UiPermissions &uiPermissions) const;

	typedef int32_t Bitmap;									///< permissions bitmap type

private:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	bitmap bitmap of permissions.
	 */
	UiPermissions(const Bitmap bitmap);

	Bitmap m_bitmap;										///< the permissions bitmap
};

} // namespace Activ

#endif // !defined (ACTIV_UI_PERMISSIONS_H)
