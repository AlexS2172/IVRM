/**
 *	@file	Subject.h
 *
 *	@brief	Subject type header file.
 *
 *	$Log: $
 */

#if !defined (ACTIV_SUBJECT_H)
#define ACTIV_SUBJECT_H

#include "ActivMiddleware/ActivSystem/External.h"
#include "ActivMiddleware/ActivSystem/MessageTypes.h"
#include "ActivMiddleware/Misc/MemoryHelper.h"

namespace Activ
{

class MessageBuilder;
class MessageValidator;

/**
 *	@brief	Subject class.
 */
class Subject
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	ACTIV_SYSTEM_API Subject();

	/**
	 *	@brief	Constructor for binary subject.
	 *
	 *	@param	pSubjectData subject data.
	 *	@param	subjectDataLength subject data length.
	 *
	 *	@throw	StatusCodeException
	 */
	ACTIV_SYSTEM_API Subject(const void *pSubjectData, const size_t subjectDataLength);

	/**
	 *	@brief	Constructor for c-string subject.
	 *
	 *	@param	pSubject textual subject.
	 *
	 *	@throw	StatusCodeException
	 */
	ACTIV_SYSTEM_API Subject(const char *pSubject);

	/**
	 *	@brief	Constructor for stl string subject.
	 *
	 *	@param	subject subject string.
	 *
	 *	@throw	StatusCodeException
	 */
	ACTIV_SYSTEM_API Subject(const std::string &subject);

	/**
	 *	@brief	Clear the subject down.
	 */
	void Clear();

	/**
	 *	@brief	Serialize a subject.
	 *
	 *	@param	messageBuilder.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_SYSTEM_API StatusCode Serialize(MessageBuilder &messageBuilder) const;

	/**
	 *	@brief	Deserialize a subject.
	 *
	 *	@param	messageValidator.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_SYSTEM_API StatusCode Deserialize(MessageValidator &messageValidator);

	/**
	 *	@brief	Get the length of the subject.
	 *
	 *	@param	Length of subject.
	 */
	size_t GetLength() const;

	/**
	 *	@brief	Get the length of the subject when serialized.
	 *
	 *	@return	The serialized length of the subject.
	 */
	size_t GetSerializedLength() const;

	/**
	 *	@brief	Set subject.
	 *
	 *	@param	pSubject textual subject.
	 *
	 *	@throw	StatusCodeException
	 */
	ACTIV_SYSTEM_API void Set(const char *pSubject);

	/**
	 *	@brief	Set subject.
	 *
	 *	@param	pSubjectData subject data.
	 *	@param	subjectDataLength subject data length.
	 *
	 *	@throw	StatusCodeException
	 */
	ACTIV_SYSTEM_API void Set(const void *pSubjectData, const size_t subjectDataLength);

	/**
	 *	@brief	Set the values.
	 *
	 *	@param	messageClass subject namespace.
	 *	@param	subject subject string.
	 *
	 *	@throw	StatusCodeException
	 */
	ACTIV_SYSTEM_API void Set(const std::string &subject);

	/**
	 *	@brief	Set subject from another Subject object.
	 *
	 *	@param	subject Subject to assign from.
	 */
	void Set(const Subject &subject);

	/**
	 *	@brief	Get pointer to raw subject data.
	 *
	 *	@return	Pointer to raw subject data.
	 */
	const void *Get() const;

	/**
	 *	@brief	Convert the subject to a string.
	 *
	 *	@return	A string containing the subject. Note if the subject is not printable, nor will the returned string be!
	 */
	ACTIV_SYSTEM_API std::string ToString() const;

	/**
	 *	@brief	Equality operator.
	 *
	 *	@param	subject subject to compare.
	 *
	 *	@return	boolean.
	 */
	ACTIV_SYSTEM_API bool operator==(const Subject &subject) const;

	/**
	 *	@brief	Inequality operator.
	 *
	 *	@param	subject subject to compare.
	 *
	 *	@return	boolean.
	 */
	ACTIV_SYSTEM_API bool operator!=(const Subject &subject) const;

	/**
	 *	@brief	Less than operator.
	 *
	 *	@param	subject subject to compare.
	 *
	 *	@return	boolean.
	 */
	ACTIV_SYSTEM_API bool operator<(const Subject &subject) const;

	/**
	 *	@brief	Greater than operator.
	 *
	 *	@param	subject subject to compare.
	 *
	 *	@return	boolean.
	 */
	ACTIV_SYSTEM_API bool operator>(const Subject &subject) const;

	/**
	 *	@brief	Less than or equal operator.
	 *
	 *	@param	subject subject to compare.
	 *
	 *	@return	boolean.
	 */
	ACTIV_SYSTEM_API bool operator<=(const Subject &subject) const;

	/**
	 *	@brief	Greater than or equal operator.
	 *
	 *	@param	subject subject to compare.
	 *
	 *	@return	boolean.
	 */
	ACTIV_SYSTEM_API bool operator>=(const Subject &subject) const;

private:
	/**
	 *	@brief	Initialize method.
	 *
	 *	@param	pSubjectData subject data.
	 *	@param	subjectDataLength subject data length.
	 *
	 *	@throw	StatusCodeException
	 */
	void Initialize(const void *pSubjectData, const size_t subjectDataLength);

	/**
	 *	@brief	Compare 2 subjects.
	 *
	 *	@param	subject Subject to compare.
	 *
	 *	@retval	0 if the subjects are identical.
	 *	@retval	+ve if this is lexicographically greater than \a subject.
	 *	@retval	-ve if this is lexicographically less than \a subject.
	 */
	int Compare(const Subject &subject) const;

	typedef byte_t SubjectLength;								///< type of the length
	static const SubjectLength	MAX_LENGTH = 48;				///< maximum subject length

#pragma pack(1)

	byte_t						m_subject[MAX_LENGTH];			///< subject
	SubjectLength				m_length;						///< length of the subject

#pragma pack()
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Subject::Clear()
{
	m_length = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Subject::GetLength() const
{
	return m_length;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t Subject::GetSerializedLength() const
{
	return sizeof(m_length) + m_length;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void Subject::Set(const Subject &subject)
{
	m_length = subject.m_length;
	MemoryCopy(m_subject, subject.m_subject, m_length);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const void *Subject::Get() const
{
	return m_subject;
}

} // namespace Activ

#endif // !defined (ACTIV_SUBJECT_H)
