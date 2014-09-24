/**
 *	@file	File.h
 *
 *	@brief	Header file for file class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_BASE_FILE_H)
#define ACTIV_BASE_FILE_H

#include "ActivMiddleware/ActivBase/External.h"

#include <string>

namespace Activ
{

/**
 *	@brief	basic file class
 */
class File
{
public:
	typedef uint32_t AccessFlags;									///< The access flags type definition
	static const AccessFlags ACCESS_FLAG_READ = 0x00000001;			///< Open/create file with read access
	static const AccessFlags ACCESS_FLAG_WRITE = 0x00000002;		///< Open/create file with write access

	typedef uint32_t ShareFlags;									///< The share modes type definition
	static const ShareFlags SHARE_FLAG_READ = 0x00000001;			///< Open/create file with read sharing access
	static const ShareFlags SHARE_FLAG_WRITE = 0x00000002;			///< Open/create file with write sharing access

	/**
	 *	@brief	Seek method.
	 */
	enum SeekMethod
	{
		SEEK_METHOD_BEGIN,
		SEEK_METHOD_END,
		SEEK_METHOD_CURRENT,
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
	 *	@retval	STATUS_CODE_FAILURE
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
	 *	@retval	STATUS_CODE_FAILURE
     */
	ACTIV_BASE_API StatusCode Open(const std::string &fileName, const AccessFlags accessFlags, const ShareFlags shareFlags);

	/**
     *  @brief	Close the file.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	STATUS_CODE_FAILURE
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
	 *	@retval	STATUS_CODE_FAILURE
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
	 *	@retval	STATUS_CODE_FAILURE
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
     */
	ACTIV_BASE_API StatusCode Write(const void *pBuffer, const size_t nBytesToWrite);

	/**
     *  @brief	Get the current position within the file.
	 *
	 *	@param	position the position within the file.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	STATUS_CODE_FAILURE
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
	 *	@retval	STATUS_CODE_FAILURE
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
     */
	ACTIV_BASE_API StatusCode GetSize(int64_t &size) const;

	/**
     *  @brief	Set the size of the file.
	 *
	 *	@param	size the size of the file.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_OPEN
	 *	@retval	STATUS_CODE_FAILURE
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
     */
	ACTIV_BASE_API static StatusCode Copy(const std::string &sourceFileName, const std::string &targetFileName, bool replaceExisting = false);

	/**
     *  @brief	Remove a file.
	 *
	 *	@param	fileName the file to remove.
	 *
     *  @retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_FAILURE
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
	 *	@retval	STATUS_CODE_FAILURE
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
     */
	ACTIV_BASE_API static StatusCode GetSize(const std::string &fileName, int64_t &size);

	/**
     *  @brief	Is the file found.
	 *
	 *	@param	fileName the file to find.
	 *
	 *	@return	whether the file exists.
     */
	ACTIV_BASE_API static bool IsFound(const std::string &fileName);

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
	 *	@brief	Create a fully qualified filename from directory and basic file name.
	 *
	 *	@param	directory the directory.
	 *	@param	basicFileName the basic file name.
	 *
	 *	@return	Full pathname.
	 */
	ACTIV_BASE_API static std::string MakePath(const std::string &directory, const std::string &basicFileName);

	/**
	 *	@brief	Get the directory and basic filename from a fully qualified filename.
	 *
	 *	@param	fullyQualifiedFileName the fully qualified file name.
	 *	@param	directory the directory.
	 *	@param	basicFileName the basic file name.
	 *
	 *	@return	Full pathname.
	 */
	ACTIV_BASE_API static void SplitPath(const std::string &fullyQualifiedFileName, std::string &directory, std::string &basicFileName);

	/**
	 *	@brief	Search the current path for a file.
	 *
	 *	NB the current directory is always included in the search.
	 *
	 *	@param	basicFileName the file (without any directory component) to search for.
	 *	@param	fullyQualifiedFileName reference to a string to receive the path + filename of the found file.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	STATUS_CODE_NOT_FOUND
	 *	@retval	STATUS_CODE_FAILURE
	 */
	ACTIV_BASE_API static StatusCode SearchPath(const std::string &basicFileName, std::string &fullyQualifiedFileName);

	/**
	 *	@brief	Is the provided path name just a file name?
	 *
	 *	@param	pathName path to check.
	 *
	 *	@return	true if \a pathName is just a file name.
	 */
	ACTIV_BASE_API static bool IsBasicFileName(const std::string &pathName);

private:
	std::string	m_fileName;
	
#if defined (ACTIV_PLATFORM_WINDOWS)
	HANDLE		m_fileHandle;
#elif defined (ACTIV_PLATFORM_POSIX)
	static const size_t COPY_BUFFER_SIZE = 4096;					///< Size of copy buffer used for copying files

	int			m_fileHandle;
#else
#error
#endif
};

} // namespace Activ

#endif // !defined (ACTIV_BASE_FILE_H)
