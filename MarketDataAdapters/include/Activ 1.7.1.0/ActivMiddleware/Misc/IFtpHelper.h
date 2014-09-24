/**
 *	@file	IFtpHelper.h
 *
 *	@brief	Header file for the FTP helper class which user cUrl and ftpparser.
 *
 *	When using this file you will need to link to the curl library (CurlPackage_x86_win32_vc71_mdid.lib etc.).
 *
 *	$Log: $
 */

#if (!defined ACTIV_MISC_IFTP_HELPER_H)
#define ACTIV_MISC_IFTP_HELPER_H

#include "ActivMiddleware/ActivBase/StatusCodeException.h"
#include "ActivMiddleware/ActivBase/OsException.h"
#include "ActivMiddleware/ActivBase/File.h"
#include "ActivMiddleware/Misc/FtpParse.h"

#include "curl/curl.h"

#include <string>
#include <vector>
#include <time.h>

namespace Activ
{

/**
 *	@brief	FileInfo class.
 *
 *	This FileInfo class wraps up file info from curllib
 */
class FileInfo
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	FileInfo();

	/**
	 *	@brief	compare the names of two FileInfo to deterimine which one is greater.
	 *
	 *	@param first the first FileInfo
	 *  @param second the second FileInfo
	 *
	 *  return true if greater
	 */
	static bool IsNameGreater(const FileInfo &first, const FileInfo &second);

	/**
	 *	@brief	compare the size of two FileInfo to deterimine which one is greater.
	 *
	 *	@param first the first FileInfo
	 *  @param second the second FileInfo
	 *
	 *  return true if greater
	 */
	static bool IsSizeGreater(const FileInfo &first, const FileInfo &second);

	/**
	 *	@brief	compare the time of two FileInfo to deterimine which one is greater.
	 *
	 *	@param first the first FileInfo
	 *  @param second the second FileInfo
	 *
	 *  return true if greater
	 */
	static bool IsTimeGreater(const FileInfo &first, const FileInfo &second);

	/**
	 *	@brief	compare the time of two FileInfo to deterimine which one is lesser.
	 *
	 *	@param first the first FileInfo
	 *  @param second the second FileInfo
	 *
	 *  return true if lesser
	 */
	static bool IsTimeLess(const FileInfo &first, const FileInfo &second);

	bool operator == (const FileInfo &fileInfo) const;

	std::string  		m_name;			///< The name of the file
	std::string  		m_directory;	///< The name of the directory
	int64_t      		m_size;			///< The size of the file
	File::FileTime	 	m_fileTime;		///< The modification date and time
};

/**
 *	@brief	StatusCodeHelper class.
 */
class StatusCodeHelper
{
public:
	/**
	 *	@brief	Constuctor.
	 *
	 *	@param	statusCode the status code.
	 *	@param	statusText any addtional comments.
	 */
	StatusCodeHelper(StatusCode statusCode = STATUS_CODE_SUCCESS, const std::string &statusText = "");

	/**
	 *	@brief	Copy constuctor.
	 *
	 *	@param	statusCodeHelper the other object to copy from.
	 */
	StatusCodeHelper(const StatusCodeHelper &statusCodeHelper);

	/**
	 *	@brief	equality operator.
	 *
	 *	@param	statusCode the status code.
	 *	@param	statusCodeHelper the status code helper.
	 */
	friend bool operator == (const StatusCode &statusCode, const StatusCodeHelper &statusCodeHelper);

	/**
	 *	@brief	equality operator.
	 *
	 *	@param	statusCode the status code.
	 *	@param	statusCodeHelper the status code helper.
	 */
	friend bool operator != (const StatusCode &statusCode, const StatusCodeHelper &statusCodeHelper);

	/**
	 *	@brief	Get the string version.
	 */
	const std::string &ToString() const;

	operator StatusCode() const
	{
		return m_statusCode;
	}

private:
	std::string		m_statusText;		///< Any addtional text
	StatusCode		m_statusCode;		///< The status code
};

/**
 *	@brief	List of FileInfo
 */
typedef std::vector<FileInfo> FileInfoList;

/**
 *	@brief	FtpHelper class
 */
class IFtpHelper
{
public:
	/**
	 *	@brief	Constuctor
	 *
	 *	@param	server the name of the ftp server.
	 *	@param	username the user name if required.
	 *	@param	password the password if required.
	 *	@param	port the ftp port to connect to.
	 */
	IFtpHelper(const std::string &server, const std::string &username = "", const std::string &password = "", uint32_t port = DEFAULT_FTP_PORT);

	/**
	 *	@brief	Destructor
	 */
	virtual ~IFtpHelper();

	/**
	 *	@brief	Connect to the server
	 */
	virtual StatusCodeHelper Connect() = 0;

	/**
	 *	@brief	Disconnects from the server
	 */
	virtual StatusCodeHelper Disconnect() = 0;

	/**
	 *	@brief	Get the names of files from the server
	 *
	 *	@param	fileList the names of the files found on the server.
	 *	@param	remoteDirectory the from which to get the file names from
	 */
	virtual StatusCodeHelper GetFileList(FileInfoList &fileList, const std::string &remoteDirectory = "") = 0;

	/**
	 *	@brief	Get a file
	 *
	 *	@param	remoteFilename the name of the file to get
	 *	@param	localFilename the save as name of the remoteFilename
	 *	@param	remoteDirectory where the remoteFilename lives
	 */
	virtual StatusCodeHelper GetFile(const std::string &remoteFilename, const std::string &localFilename, const std::string &remoteDirectory = "") = 0;

	/**
	 *	@brief	Delete a file
	 *
	 *	@param	remoteFilename the name of the file to delete
	 *	@param	remoteDirectory where the remoteFilename lives
	 */
	virtual StatusCodeHelper DeleteFile(const std::string &remoteFilename, const std::string &remoteDirectory = "") = 0;

	/**
	 *	@brief	Delete list of file
	 *
	 *	@param	fileInfoList file list
	 *	@param	remoteDirectory where the remoteFilename lives
	 */
	virtual StatusCodeHelper DeleteFile(const FileInfoList &fileInfoList, const std::string &remoteDirectory = "") = 0;

	/**
	 *	@brief	Get and delete a file
	 *
	 *	@param	remoteFilename the name of the file to get and delete
	 *	@param	localFilename the save as name of the remoteFilename
	 *	@param	remoteDirectory where the remoteFilename lives
	 */
	virtual StatusCodeHelper GetAndDeleteFile(const std::string &remoteFilename, const std::string &localFilename, const std::string &remoteDirectory = "") = 0;

	/**
	 *	@brief	Is connected
	 */
	virtual bool IsConnected() const = 0;

	void SetServer(const std::string &server);
	void SetUsername(const std::string &username);
	void SetPassword(const std::string &password);
	void SetPort(const uint32_t &port);

	static const uint32_t DEFAULT_FTP_PORT 				= 21;	///< The default ftp port
	static const uint32_t DEFAULT_FTP_TIMEOUT 			= 300;	///< The default ftp timeout
	static const uint32_t DEFAULT_FTP_LOW_SPEED_LIMIT 	= 5;	///< The default ftp low speed limit
	static const uint32_t DEFAULT_FTP_LOW_SPEED_TIME 	= 120;	///< The default ftp low speed time

protected:
	std::string 	m_server;					///< The name of the ftp server
	std::string 	m_username;					///< The username
	std::string 	m_password;					///< The password
	uint32_t		m_port;						///< The port number normally 21
};

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

inline FileInfo::FileInfo() :
	m_size(0),
	m_fileTime(File::FileTime(time(0)))
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FileInfo::IsNameGreater(const FileInfo &first, const FileInfo &second)
{
	return (first.m_name > second.m_name);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FileInfo::IsSizeGreater(const FileInfo &first, const FileInfo &second)
{
	return (first.m_size > second.m_size);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FileInfo::IsTimeGreater(const FileInfo &first, const FileInfo &second)
{
	return (first.m_fileTime > second.m_fileTime);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FileInfo::IsTimeLess(const FileInfo &first, const FileInfo &second)
{
	File::FileTime timeOne = first.m_fileTime;
	time_t *pFirstTime = reinterpret_cast<time_t*>(&timeOne);

	File::FileTime timeTwo = second.m_fileTime;
	time_t *pSecondTime = reinterpret_cast<time_t*>(&timeTwo);

	return (*pFirstTime < *pSecondTime);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FileInfo::operator == (const FileInfo &fileInfo) const
{
	return (m_name == fileInfo.m_name);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper::StatusCodeHelper(StatusCode statusCode, const std::string &statusText) :
	m_statusText(statusText),
	m_statusCode(statusCode)

{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper::StatusCodeHelper(const StatusCodeHelper &statusCodeHelper)
{
	m_statusText = statusCodeHelper.m_statusText;
	m_statusCode = statusCodeHelper.m_statusCode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const std::string &StatusCodeHelper::ToString() const
{
	return m_statusText;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool operator == (const StatusCode &statusCode, const StatusCodeHelper &statusCodeHelper)
{
	return (statusCodeHelper.m_statusCode == statusCode);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool operator != (const StatusCode &statusCode, const StatusCodeHelper &statusCodeHelper)
{
	return !(statusCodeHelper.m_statusCode == statusCode);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

inline IFtpHelper::IFtpHelper(const std::string &server, const std::string &username, const std::string &password, uint32_t port) :
	m_server(server),
	m_username(username),
	m_password(password),
	m_port(port)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline IFtpHelper::~IFtpHelper()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IFtpHelper::SetServer(const std::string &server)
{
	m_server = server;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IFtpHelper::SetUsername(const std::string &username)
{
	m_username = username;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IFtpHelper::SetPassword(const std::string &password)
{
	m_password = password;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IFtpHelper::SetPort(const uint32_t &port)
{
	m_port = port;
}

} // namespace Activ

#endif // (!defined ACTIV_MISC_IFTP_HELPER_H)
