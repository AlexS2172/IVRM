/**
 *	@file	Subjects.h
 *
 *	@brief	Header file for framework subject classes.
 *
 *  $Log: $
 */

#if !defined (ACTIV_FRAMEWORK_SUBJECTS_H)
#define ACTIV_FRAMEWORK_SUBJECTS_H

#include "ActivMiddleware/ActivSystem/Subject.h"

namespace Activ
{

/**
 *	@brief	Endpoint subject.
 */
class EndPointSubject : public Subject
{
public:
	/**
	 *	@brief	Constructor.
	 */
	EndPointSubject();
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline EndPointSubject::EndPointSubject() :
	Subject("/system/endpoint")
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Shutdown subject.
 */
class ShutdownSubject : public Subject
{
public:
	/**
	 *	@brief	Constructor.
	 */
	ShutdownSubject();
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline ShutdownSubject::ShutdownSubject() :
	Subject("/system/shutdown")
{
}

} // namespace Activ

#endif // !defined (ACTIV_FRAMEWORK_SUBJECTS_H)
