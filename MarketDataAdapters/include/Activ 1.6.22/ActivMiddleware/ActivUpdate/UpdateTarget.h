/**
 *	@file	UpdateTarget.h
 *
 *	@brief	Header file for the update target class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_UPDATE_TARGET_H)
#define ACTIV_UPDATE_TARGET_H

#include "ActivMiddleware/ActivUpdate/External.h"

namespace Activ
{

/**
 *	@brief	Update target class.
 */
class UpdateTarget
{
public:
	/**
	 *	@brief	case-insensitive less than string comparison object.
	 */
	class CaseInsensitiveLessThanStringComparisonObject
	{
	public:
		/**
		 *	@brief	Function operator.
		 *
		 *	@param	lhs a reference to the lhs object.
		 *	@param	rhs a reference to the rhs object.
		 *
		 *	@return	whether the lhs object is less than rhs object.
		 */
		bool operator()(const std::string &lhs, const std::string &rhs) const;
	};

	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_UPDATE_API UpdateTarget();

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_UPDATE_API ~UpdateTarget();

	/**
	 *	@brief	Read settings from an ini file section.
	 *
	 *	@param	sectionIterator iterator to an ini file section.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_UPDATE_API StatusCode Read(const IniFileParser::ConstSectionIterator &sectionIterator);

	/**
	 *	@brief	Is match?
	 *
	 *	@param	systemName the system name.
	 *	@param	platformName the platform name.
	 *	@param	hostName the host name.
	 *
	 *	@return	true if match.
	 */
	ACTIV_UPDATE_API bool IsMatch(const std::string &systemName, const std::string &platformName, const std::string &hostName) const;

	/**
	 *	@brief	Is match?
	 *
	 *	@param	updateTargetFilter the update target filter to match against.
	 *
	 *	@return	true if match.
	 */
	ACTIV_UPDATE_API bool IsMatch(const UpdateTarget &updateTargetFilter) const;

	typedef std::set<std::string, CaseInsensitiveLessThanStringComparisonObject>	CaseInsensitiveStringSet;	///< Case insensitive string set type.

	CaseInsensitiveStringSet	m_systemNameSet;				///< The system name set to target.
	CaseInsensitiveStringSet	m_platformNameSet;				///< The platform name set to target.
	CaseInsensitiveStringSet	m_hostNameSet;					///< The hostname set to target.

private:
	/**
	 *	@brief	Have common item?
	 *
	 *	@param	lhs.
	 *	@param	rhs.
	 *
	 *	@return	true if sets have common item.
	 */
	static bool HaveCommonItem(const CaseInsensitiveStringSet &lhs, const CaseInsensitiveStringSet &rhs);
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool UpdateTarget::CaseInsensitiveLessThanStringComparisonObject::operator()(const std::string &lhs, const std::string &rhs) const
{
	return (::stricmp(lhs.c_str(), rhs.c_str()) < 0);
}

} // namespace Activ

#endif // !defined (ACTIV_UPDATE_TARGET_H)
