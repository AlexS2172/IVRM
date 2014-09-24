/**
 *	@file	File.h
 *
 *	@brief	Header file for file class.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_FILE_H)
#define ACTIV_BASE_FILE_H

#include "ActivMiddleware/ActivBase/External.h"

#include <string>

namespace Activ
{

/**
 *	@brief	File class.
 */
class File
{
public:
	typedef uint32_t AccessFlags;								///< The access flags type definition
	static const AccessFlags ACCESS_FLAG_READ = 0x00000001;		///< Open/create file with read access
	static const AccessFlags ACCESS_FLAG_WRITE = 0x00000002;	///< Open/create file with write access

	typedef uint32_t ShareFlags;								///< The share modes type definition
	static const ShareFlags SHARE_FLAG_READ = 0x00000001;		///< Open/create file with read sharing access
	static const ShareFlags SHARE_FLAG_WRITE = 0x00000002;		///< Open/create file with write sharing access

	/**
	 *	@brief	Seek method.
	 */
	enum SeekMethod
	{
		SEEK_METHOD_BEGIN,
		SEEK_METHOD_END,
		SEEK_METHOD_CURRENT
	};

	/**
	 *	@brief	FileTime class.
	 */
	class FileTime
	{
	public:
		/**
		 *  @brief	Constructor.
		 *
		 *	@param	fileTime the time.
		 */
		ACTIV_BASE_API FileTime(time_t fileTime = 0);

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	year the year.
		 *	@param	month the month.
		 *	@param	day the day.
		 *	@param	hour the hour.
		 *	@param	minute the minute.
		 *	@param	second the second.
		 *
		 *	@throw	StatusCodeException
		 */
		ACTIV_BASE_API FileTime(const int32_t &year, const int32_t &month, const int32_t &day, const uint32_t &hour, const uint32_t &minute, const uint32_t &second);

		/**
		 *	@brief	compare FileTime
		 *
		 *	@param	fileTime the file time.
		 *
		 *	@retval	true
		 *	@retval	false
		 */
		bool operator>(const FileTime &fileTime) const;

	private:
		time_t m_fileTime;										///< The datetime
	};

    /**
     *  @brief	Default constructor.
     */
    ACTIV_BASE_API File();

    /**
     *  @brief	Destructor.
     */
	ACTIV_BASE_API ~File();

	/**
     *  @brief	Create the file.
	 *
	 *	@param	fileName the file name.
	 *	@param	accessFlags the file access flags.
	 *	@param	shareFlags the file sharing flags.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_ALREADY_OPEN
	 *	@retval	...
     */
	ACTIV_BASE_API StatusCode Create(const std::string &fileName, const AccessFlags accessFlags, const ShareFlags shareFlags);

	/**
     *  @brief	Open the file.
	 *
	 *	@param	fileName the file name.
	 *	@param	accessFlags the file access flags.
	 *	@param	shareFlags the file sharing flags.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_ALREADY_OPEN
	 *	@retval	...
     */
	ACTIV_BASE_API StatusCode Open(const std::string &fileName, const AccessFlags accessFlags, const ShareFlags shareFlags);

	/**
     *  @brief	Close the file.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	...
     */
	ACTIV_BASE_API StatusCode Close();

	/**
     *  @brief	Read from a file.
	 *
	 *	@param	pBuffer the buffer to read into.
	 *	@param	nBytesToRead the number of bytes to read.
	 *	@param	nBytesRead the number of bytes read.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	...
     */
	ACTIV_BASE_API StatusCode Read(void *pBuffer, const size_t nBytesToRead, size_t &nBytesRead);

	/**
     *  @brief	Read from a file.
	 *
	 *	@param	pBuffer the buffer to read into.
	 *	@param	nBytesToRead the number of bytes to read.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	...
     */
	ACTIV_BASE_API StatusCode Read(void *pBuffer, const size_t nBytesToRead);

	/**
     *  @brief	Write to a file.
	 *
	 *	@param	pBuffer the buffer to write from.
	 *	@param	nBytesToWrite the number of bytes to write.
	 *	@param	nBytesWritten the number of bytes written.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	...
     */
	ACTIV_BASE_API StatusCode Write(const void *pBuffer, const size_t nBytesToWrite, size_t &nBytesWritten);

	/**
     *  @brief	Write to a file.
	 *
	 *	@param	pBuffer the buffer to write from.
	 *	@param	nBytesToWrite the number of bytes to write.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	...
     */
	ACTIV_BASE_API StatusCode Write(const void *pBuffer, const size_t nBytesToWrite);

	/**
     *  @brief	Get the current position within the file.
	 *
	 *	@param	position the position within the file.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	...
     */
	ACTIV_BASE_API StatusCode GetPosition(int64_t &position) const;

	/**
     *  @brief	Set the current position within the file.
	 *
	 *	@param	position the position within the file.
	 *	@param	seekMethod the seek method.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	...
     */
	ACTIV_BASE_API StatusCode SetPosition(int64_t position, const SeekMethod seekMethod = SEEK_METHOD_BEGIN);

	/**
     *  @brief	Get the size of the file.
	 *
	 *	@param	size the size of the file.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	...
     */
	ACTIV_BASE_API StatusCode GetSize(int64_t &size) const;

	/**
	 *  @brief	Get the modified date time of the file.
	 *
	 *	@param	dateTime the date time of the file.
	 *
	 *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	...
	 */
	ACTIV_BASE_API StatusCode GetModifiedDateTime(FileTime &dateTime) const;

	/**
     *  @brief	Set the size of the file.
	 *
	 *	@param	size the size of the file.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	...
     */
	ACTIV_BASE_API StatusCode SetSize(int64_t size);

	/**
     *	@brief	Get the file name.
	 *
	 *	@param	fileName the file name.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
     */
	ACTIV_BASE_API StatusCode GetFileName(std::string &fileName) const;

	/**
	 *	@brief	Get the underlying file handle
	 *
	 *	@return	the file handle
	 */
	HANDLE GetHandle() const;

	/**
     *  @brief	Is the file open.
	 *
	 *	@return	whether the file is open.
     */
	ACTIV_BASE_API bool IsOpen() const;

	/**
     *  @brief	Copy a file.
	 *
	 *	@param	sourceFileName the name of the existing file.
	 *	@param	targetFileName the name of the new file.
	 *	@param	replaceExisting whether an existing file should be overwritten.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	...
     */
	ACTIV_BASE_API static StatusCode Copy(const std::string &sourceFileName, const std::string &targetFileName, bool replaceExisting = false);

	/**
     *  @brief	Remove a file.
	 *
	 *	@param	fileName the file to remove.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	...
     */
	ACTIV_BASE_API static StatusCode Remove(const std::string &fileName);

	/**
     *  @brief	Rename a file.
	 *
	 *	@param	sourceFileName the name of the existing file.
	 *	@param	targetFileName the name of the new file.
	 *	@param	replaceExisting whether an existing file should be overwritten.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	...
     */
	ACTIV_BASE_API static StatusCode Rename(const std::string &sourceFileName, const std::string &targetFileName, bool replaceExisting = false);

	/**
     *  @brief	Get the size of the file.
	 *
	 *	@param	fileName the file name.
	 *	@param	size the size of the file.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	...
     */
	ACTIV_BASE_API static StatusCode GetSize(const std::string &fileName, int64_t &size);

	/**
	 *  @brief	Get the Modified date time of the file.
	 *
	 *	@param	fileName the file name.
	 *	@param	dateTime the date time of the file.
	 *
	 *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	...
	 */
	ACTIV_BASE_API static StatusCode GetModifiedDateTime(const std::string &fileName, FileTime &dateTime);

	/**
     *  @brief	Is the file found?
	 *
	 *	@param	fileName the file to find.
	 *
	 *	@return	whether the file exists.
     */
	ACTIV_BASE_API static bool IsFound(const std::string &fileName);

	/**
	 *	@brief	Is a path a directory?
	 *
	 *	@param	pathName path to check.
	 *
	 *	@return	boolean.
	 */
	ACTIV_BASE_API static bool IsDirectory(const std::string &pathName);

	/**
     *  @brief	Get the end of line characters.
	 *
	 *	@return	the end of line characters.
     */
	ACTIV_BASE_API static const std::string &GetEndOfLine();

	/**
     *  @brief	Get directory separator character.
	 *
	 *	@return	Directory separator character.
     */
	ACTIV_BASE_API static char GetDirectorySeparator();

	/**
	 *	@brief	Get temporary directory path.
	 *
	 *	Note the returned path may or may not contain a trailing directory separator,
	 *	so use File::MakePath() to construct the path of a file in the temporary directory.
	 *
	 *	@return	Temporary directory path.
	 */
	ACTIV_BASE_API static std::string GetTemporaryPath();

	/**
	 *	@brief	Create a fully qualified filename from directory and file name.
	 *
	 *	@param	directory the directory.
	 *	@param	fileName the file name.
	 *
	 *	@return	Full pathname.
	 */
	ACTIV_BASE_API static std::string MakePath(const std::string &directory, const std::string &fileName);

	/**
	 *	@brief	Get the directory and filename from a fully qualified filename.
	 *
	 *	@param	fullyQualifiedFileName the fully qualified file name.
	 *	@param	directory to receive the directory.
	 *	@param	fileName to receive the file name.
	 *
	 *	@return	Full pathname.
	 */
	ACTIV_BASE_API static void SplitPath(const std::string &fullyQualifiedFileName, std::string &directory, std::string &fileName);

	/**
	 *	@brief	Search the current path for a file.
	 *
	 *	NB the current directory is always included in the search.
	 *
	 *	@param	fileName the file (without any directory component) to search for.
	 *	@param	fullyQualifiedFileName reference to a string to receive the path + filename of the found file.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_FAILURE
	 *	@retval	...
	 */
	ACTIV_BASE_API static StatusCode SearchPath(const std::string &fileName, std::string &fullyQualifiedFileName);

	/**
	 *	@brief	Is the provided path name just a file name?
	 *
	 *	@param	pathName path to check.
	 *
	 *	@return	true if \a pathName is just a file name.
	 */
	ACTIV_BASE_API static bool IsFileNameOnly(const std::string &pathName);

private:
	std::string	m_fileName;
	HANDLE		m_fileHandle;
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline HANDLE File::GetHandle() const
{
	return m_fileHandle;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

inline bool File::FileTime::operator>(const FileTime &fileTime) const
{
	return (m_fileTime > fileTime.m_fileTime);
}

} // namespace Activ

#endif // (!defined ACTIV_BASE_FILE_H)
