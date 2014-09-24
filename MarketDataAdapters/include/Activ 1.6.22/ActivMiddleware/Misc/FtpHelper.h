/**
 *	@file	FtpHelper.h
 *
 *	@brief	Header file for the FTP helper class which user cUrl and ftpparser.
 *
 *	When using this file you will need to link to the curl library (curllib_x86_win32_vc70_mdid.lib etc.).
 *
 *	$Log: $
 */

#if !defined (ACTIV_FTP_HELPER_H)
#define ACTIV_FTP_HELPER_H

#include "ActivMiddleware/ActivBase/ActivException.h"
#include "ActivMiddleware/ActivBase/File.h"
#include "ActivMiddleware/ActivBase/OsException.h"
#include "ActivMiddleware/Misc/FtpParse.h"

#include "curl/curl.h"

#include "boost/regex.hpp"

#if defined (ACTIV_PLATFORM_WINDOWS)
#include <wininet.h>
#endif

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

	std::string  		m_name;			///< The name of the file
	std::string  		m_directory;	///< The name of the directory
	int64_t      		m_size;			///< The size of the file
	File::FileTime	 	m_fileTime;		///< The modification date and time

};

// ---------------------------------------------------------------------------------------------------------------------------------

typedef std::vector<FileInfo> 		FileInfoList;

// ---------------------------------------------------------------------------------------------------------------------------------


// These need to be here as Curl is written in C and doesn't like calling functions in C++ classes.
size_t DummyCallbackFunction(void *ptr, size_t size, size_t nmemb, void *stream);
size_t GetFileListCallbackFunction(void *ptr, size_t size, size_t nmemb, void *stream);
size_t GetFileCallbackFunction(void *ptr, size_t size, size_t nmemb, void *stream);

class CallbackStruct
{
public:
};

// ---------------------------------------------------------------------------------------------------------------------------------

class GetFileListCallbackStruct : public CallbackStruct
{
public:
	std::string			m_data;
	std::string *		pError;
};

// ---------------------------------------------------------------------------------------------------------------------------------

class GetFileCallbackStruct : public CallbackStruct
{
public:
	std::string *		pError;
	File		*		m_localFile;
};

// ---------------------------------------------------------------------------------------------------------------------------------


/**
 *	@brief	FtpHelper class.
 *
 *	This FtpHelper class wraps up ftp commands which use cUrl and ftpparse (Only downloading at the moment).
 */
class FtpHelper
{
public:
	/**
	 *	@brief	Default constructor.
	 */
	FtpHelper();

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	server server name.
	 *	@param	username username.
	 *	@param	password password.
	 *	@param	useWinFtp use win ftp or not.
	 */
	FtpHelper(const std::string &server, const std::string &username, const std::string &password, bool useWinFtp = false);

	/**
	 *	@brief	Destructor.
	 */
    ~FtpHelper();

	/**
	 *	@brief	Initialize - must be called only once by the application before using Connect().
	 */
    static void Initialize();

	/**
	 *	@brief	CleanUp - must be called only once by the application when shutting down.
	 */
    static void CleanUp();

	/**
	 *	@brief	Connects to ftp site.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
    StatusCode Connect();

	/**
	 *	@brief	Disconnects from ftp site.
 	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
    StatusCode Disconnect();

	/**
	 *	@brief	If the connectio to the ftp site up.
 	 *
	 *	@return	true if connected.
	 */
    bool IsConnected() const;

	/**
	 *	@brief	Gets a list of all files on ftp site in current folder.
	 *
	 *	@param	fileList The list where the filenames will be put.
	 *	@param	includeDirectories Include directories in the list.
	 *	@param	directory from which the requested info will come from
	 *
 	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	template<typename Container>
	StatusCode GetFileList(std::insert_iterator<Container> it, const bool includeDirectories, const std::string &directory = "");

	/**
	 *	@brief	Download a file from the ftp site.
	 *
	 *	@param	fileInfo The filename to get from the ftp site.
	 *	@param	localFilename The local filename to copy the file to.
	 *	@param	directory from which the requested info will come from
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode GetFile(const FileInfo &fileInfo, const std::string localFilename, const std::string &directory);

	/**
	 *	@brief	Deletes a file from the ftp site.
	 *
	 *	@param	fileInfo The filename to delete from the ftp site.
	 *	@param	directory from which the requested info will come from
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode DeleteFile(const FileInfo &fileInfo, const std::string &directory);

	/**
	 *	@brief	Get and deletes a file from the ftp site.
	 *
	 *	@param	fileInfo The filename to delete from the ftp site.
	 *	@param	directory from which the requested info will come from
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode GetAndDeleteFile(const FileInfo &fileInfo, const std::string &localFilename, const std::string &directory);

	/**
	 *	@brief	Gets the readable error string.
	 *
	 *	@retval	Error string
	 */
	std::string GetErrorString();

private:
	/**
	 *	@brief	Url class.
	 *
	 *	To parse url info copied from ActivBase
 	 */
 	class Url
 	{
	public:
		/**
		 *	@brief	Constructor.
		 */
		Url();

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	url the url
		 *
		 *	@throw	ActivException
	 	 */
		Url(const std::string &url);

		std::string m_url;					///< The url
		std::string m_protocol;				///< The protocol
		std::string m_location;				///< The location
	    std::string m_path;					///< The path
	    std::string m_host;					///< The ipaddress
	    uint16_t 	m_port;					///< The port

	};

	/**
	 *	@brief	Connects to ftp site.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode ConnectWinFtp();

	/**
	 *	@brief	Disconnects from ftp site.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode DisconnectWinFtp();

	/**
	 *	@brief	If the connectio to the ftp site up.
	 *
	 *	@return	true if connected.
	 */
	bool IsConnectedWinFtp() const;

	/**
	 *	@brief	Gets a list of all files on ftp site in current folder.
	 *
	 *	@param	fileList The list where the filenames will be put.
	 *	@param	includeDirectories Include directories in the list.
	 *	@param	directory from which the requested info will come from
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	template<typename Container>
	StatusCode GetFileListWinFtp(std::insert_iterator<Container> it, const bool includeDirectories, const std::string &directory);

	/**
	 *	@brief	Download a file from the ftp site.
	 *
	 *	@param	fileInfo The filename to get from the ftp site.
	 *	@param	localFilename The local filename to copy the file to.
	 *	@param	directory from which the requested info will come from
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode GetFileWinFtp(const FileInfo &fileInfo, const std::string &localFilename, const std::string &directory);

	/**
	 *	@brief	Deletes a file from the ftp site.
	 *
	 *	@param	fileInfo The filename to delete from the ftp site.
	 *	@param	directory from which the requested info will come from
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode DeleteFileWinFtp(const FileInfo &fileInfo, const std::string &directory);

	/**
	 *	@brief	Get and Deletes a file from the ftp site.
	 *
	 *	@param	fileInfo The filename to delete from the ftp site.
	 *	@param	directory from which the requested info will come from
	 *	@param	localFilename The local filename to copy the file to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode GetAndDeleteFileWinFtp(const FileInfo &fileInfo, const std::string &localFilename, const std::string &directory);

	/**
	 *	@brief	cleans up the url
	 *
	 *	@param	url The url
	 */
	void TidyFtpUrl(std::string &url, size_t from = 0);

	/**
	 *	@brief	typedef of a call back function
	 */
	typedef size_t (CallBackFunction)(void *ptr, size_t size, size_t nmemb, void *stream);

	/**
	 *	@brief	Excute a ftp operation
	 *
	 *	@param	pCurl The curl handler
	 *	@param	url  The url
	 *	@param	list  Any addtional curl commands, e.g. delete
	 *	@param	callBackFunction  The call back function
	 *	@param	data  Data for the call back function
	 *	@param	getText  to use text mode or not, mainly for getting a list of files
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode CurlExcute(CURL *pCurl, const std::string &url, struct curl_slist *list = 0, CallBackFunction *callBackFunction = DummyCallbackFunction, CallbackStruct *data = 0, bool getText = false);

#if defined (ACTIV_PLATFORM_WINDOWS)
	/**
	 *	@brief	Addtional windows error msg
	 */
	std::string GetExtentedWinFtpErrorMessage();
#endif

	const std::string m_prot;				///< The "ftp://";

	Url				m_url;					///< The Url of the ftp site.
	std::string		m_username;				///< The username
	std::string		m_password;				///< The password
	std::string		m_usernamePassword;		///< The usernamePassword string
	std::string		m_error;				///< A readable string of the last error.

	bool			m_useWinFtp;			///< Use win ftp or not

#if defined (ACTIV_PLATFORM_WINDOWS)
	HINTERNET		m_internetHandle;		///< The internet handle.
	HINTERNET		m_ftpSessionHandle;		///< The ftp session handle.
#endif

	CURL *			m_curl;					///< The curl handle.
};

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

inline FtpHelper::FtpHelper() :
	m_curl(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FtpHelper::FtpHelper(const std::string &server, const std::string &username, const std::string &password, bool useWinFtp) :
	m_prot("ftp://"),
	m_url(m_prot + server),
	m_username(username),
	m_password(password),
	m_usernamePassword(m_username + ":" + m_password),
	m_useWinFtp(useWinFtp),
#if defined (ACTIV_PLATFORM_WINDOWS)
	m_internetHandle(0),
	m_ftpSessionHandle(0),
#endif
	m_curl(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FtpHelper::~FtpHelper()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void FtpHelper::Initialize()
{
	curl_global_init(CURL_GLOBAL_ALL);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void FtpHelper::CleanUp()
{
	curl_global_cleanup();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FtpHelper::Connect()
{
	if (m_useWinFtp)
		return ConnectWinFtp();

	StatusCode statusCode = STATUS_CODE_FAILURE;

	if (IsConnected())
		return STATUS_CODE_ALREADY_CONNECTED;

	std::string connectUrl = m_url.m_url + "/";
	TidyFtpUrl(connectUrl, m_prot.size());

	m_error.clear();

	try
	{
		m_curl = curl_easy_init();

		if (0 == m_curl)
		{
			m_error = "Failed to Initialize Curl";
			return statusCode;
		}

		statusCode = CurlExcute(m_curl, connectUrl);
	}
	catch (Exception &exception)
	{
		m_error += std::string("CURL error - ") + curl_easy_strerror(static_cast<CURLcode>(exception.m_errorCode));

		Disconnect();
	}

	return statusCode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FtpHelper::Disconnect()
{
	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	if (m_useWinFtp)
		return DisconnectWinFtp();

	if (0 != m_curl)
		curl_easy_cleanup(m_curl);

	m_curl = 0;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FtpHelper::IsConnected() const
{
	if (m_useWinFtp)
		return IsConnectedWinFtp();

	return (0 != m_curl);
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename Container> inline StatusCode FtpHelper::GetFileList(std::insert_iterator<Container> it, const bool includeDirectories, const std::string &directory)
{
	if (m_useWinFtp)
		return GetFileListWinFtp(it, includeDirectories, directory);

	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	StatusCode statusCode = STATUS_CODE_FAILURE;
	GetFileListCallbackStruct getFileListCallbackStruct;

	m_error.clear();

	getFileListCallbackStruct.pError = &m_error;

	std::string tempUrl = m_url.m_url + "/" + directory + "/";
	TidyFtpUrl(tempUrl, m_prot.size());

	statusCode = CurlExcute(m_curl, tempUrl, 0, GetFileListCallbackFunction, &getFileListCallbackStruct, true);
	if (STATUS_CODE_SUCCESS == statusCode)
	{
		size_t start = 0, end = 0;
		struct ftpparse parseInfo;

		std::string &data = getFileListCallbackStruct.m_data;
		while((end = data.find_first_of("\r\n", start)) != data.npos)
		{
			if (ftpparse(&parseInfo, const_cast<char*>(data.c_str()) + start, static_cast<int>(end-start)))
			{
				FileInfo fileInfo;

				if (!(includeDirectories || !parseInfo.flagtrycwd))
				{
					start = end+2;
					continue;
				}

				fileInfo.m_name = std::string(parseInfo.name, parseInfo.namelen);
				fileInfo.m_size = static_cast<int64_t>(parseInfo.size);

				if (FTPPARSE_MTIME_UNKNOWN != parseInfo.mtimetype)
				{
					fileInfo.m_fileTime = File::FileTime(parseInfo.mtime);
				}

				fileInfo.m_directory = directory;
				*it = fileInfo;
			}
			else
			{
				m_error = "Failed to parse ftp LIST information - unknown format. ";
				statusCode = STATUS_CODE_FAILURE;
				break;
			}

			start = end+2;
		}
	}

	return statusCode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FtpHelper::GetFile(const FileInfo &fileInfo, const std::string localFilename, const std::string &directory)
{
	if (m_useWinFtp)
		return GetFileWinFtp(fileInfo, localFilename, directory);

	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	StatusCode statusCode = STATUS_CODE_FAILURE;
	const std::string &filename = fileInfo.m_name;
	std::string requestUrl(m_url.m_url + "/" + directory + "/" + filename);
	TidyFtpUrl(requestUrl, m_prot.size());
	GetFileCallbackStruct getFileCallbackStruct;

	m_error.clear();
	File localFile;

	getFileCallbackStruct.pError = &m_error;
	getFileCallbackStruct.m_localFile = &localFile;

	if (STATUS_CODE_SUCCESS != localFile.Create(localFilename, File::ACCESS_FLAG_WRITE, File::SHARE_FLAG_READ))
	{
		m_error += "Failed to create temp file ";
		return STATUS_CODE_FAILURE;
	}

	statusCode = CurlExcute(m_curl, requestUrl, 0, GetFileCallbackFunction, &getFileCallbackStruct);
	localFile.Close();

	return statusCode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FtpHelper::DeleteFile(const FileInfo &fileInfo, const std::string &directory)
{
	if (m_useWinFtp)
		return DeleteFileWinFtp(fileInfo, directory);

	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	m_error.clear();
	StatusCode statusCode = STATUS_CODE_FAILURE;
	const std::string &filename = fileInfo.m_name;

	std::string deleteUrl = m_url.m_url + "/" + directory + "/";
	TidyFtpUrl(deleteUrl, m_prot.size());

	const std::string deleteCommand = "DELE ";
	std::string ftpCommand;
	if (directory == "/")
		ftpCommand = deleteCommand + filename;
	else
		ftpCommand = deleteCommand + "/" + directory + "/" + filename;

	TidyFtpUrl(ftpCommand, deleteCommand.size());
	struct curl_slist *list = 0;
	list = curl_slist_append(list, ftpCommand.c_str());

	//Now try to delete the file
	statusCode = CurlExcute(m_curl, deleteUrl, list);

	curl_slist_free_all(list);

	return statusCode;
}

inline StatusCode FtpHelper::GetAndDeleteFile(const FileInfo &fileInfo, const std::string &directory, const std::string &localFilename)
{
	if (m_useWinFtp)
		return GetAndDeleteFileWinFtp(fileInfo, localFilename, directory);

	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	m_error.clear();
	StatusCode statusCode = STATUS_CODE_FAILURE;
	const std::string &filename = fileInfo.m_name;
	std::string requestUrl(m_url.m_url + "/" + directory + "/" + filename);
	TidyFtpUrl(requestUrl, m_prot.size());
	GetFileCallbackStruct getFileCallbackStruct;

	m_error.clear();
	File localFile;

	std::string deleteUrl = m_url.m_url + "/" + directory + "/";
	TidyFtpUrl(deleteUrl, m_prot.size());

	const std::string deleteCommand = "DELE ";
	std::string ftpCommand;
	if (directory == "/")
		ftpCommand = deleteCommand + filename;
	else
		ftpCommand = deleteCommand + "/" + directory + "/" + filename;

	TidyFtpUrl(ftpCommand, deleteCommand.size());
	struct curl_slist *list = NULL;
	list = curl_slist_append(list, ftpCommand.c_str());

	getFileCallbackStruct.pError = &m_error;
	getFileCallbackStruct.m_localFile = &localFile;

	if (STATUS_CODE_SUCCESS != localFile.Create(localFilename, File::ACCESS_FLAG_WRITE, File::SHARE_FLAG_READ))
	{
		m_error += "Failed to create temp file ";
		return STATUS_CODE_FAILURE;
	}

	try
	{
		curl_easy_reset(m_curl);

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_URL, requestUrl.c_str()));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_USERPWD, m_usernamePassword.c_str()));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, GetFileCallbackFunction));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &getFileCallbackStruct));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_POSTQUOTE, list));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_perform(m_curl));

		statusCode = STATUS_CODE_SUCCESS;

	}
	catch (Exception &exception)
	{
		m_error += std::string("CURL error - ") + curl_easy_strerror(static_cast<CURLcode>(exception.m_errorCode));

		if (CURLE_COULDNT_CONNECT == exception.m_errorCode)
		{
			Disconnect();
			statusCode = STATUS_CODE_NOT_CONNECTED;
		}
	}

	curl_slist_free_all(list);
	return statusCode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FtpHelper::CurlExcute(CURL *pCurl, const std::string &url, struct curl_slist *list, CallBackFunction *callBackFunction, CallbackStruct *data, bool getText)
{
	StatusCode statusCode = STATUS_CODE_FAILURE;

	try
	{
		curl_easy_reset(pCurl);

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str()));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(pCurl, CURLOPT_USERPWD, m_usernamePassword.c_str()));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(pCurl, CURLOPT_FTP_USE_EPSV, 0));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 1));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, callBackFunction));

		if (getText)
			ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(pCurl, CURLOPT_TRANSFERTEXT, 1));

		if (0 != list)
			ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(pCurl, CURLOPT_QUOTE, list));

		if (0 != data)
			ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, data));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_perform(pCurl));

		statusCode = STATUS_CODE_SUCCESS;
	}
	catch (Exception &exception)
	{
		m_error += std::string("CURL error - ") + curl_easy_strerror(static_cast<CURLcode>(exception.m_errorCode));

		if (CURLE_COULDNT_CONNECT == exception.m_errorCode)
		{
			Disconnect();
			statusCode = STATUS_CODE_NOT_CONNECTED;
		}
	}

	return statusCode;

}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FtpHelper::ConnectWinFtp()
{
	StatusCode statusCode = STATUS_CODE_FAILURE;

#if defined (ACTIV_PLATFORM_WINDOWS)

	if (IsConnectedWinFtp())
		return STATUS_CODE_ALREADY_CONNECTED;

	m_error.clear();

	try
	{
		if (!m_url.m_url.empty())
		{
			m_internetHandle = ::InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

			if (NULL == m_internetHandle)
				ACTIV_THROW(OsException, ::GetLastError());

			Url url(m_url);

			m_ftpSessionHandle = ::InternetConnect(m_internetHandle, url.m_host.c_str(), url.m_port, m_username.c_str(), m_password.c_str(), INTERNET_SERVICE_FTP, 0, 0);

			if (NULL == m_ftpSessionHandle)
				ACTIV_THROW(OsException, ::GetLastError());

			statusCode = STATUS_CODE_SUCCESS;

		}
	}
	catch (OsException &osException)
	{
		m_error += "Failed to Connect to server " + m_url.m_url + ", WinFtp error - " + OsErrorToString(osException.m_errorCode) + ":" +  GetExtentedWinFtpErrorMessage();
		DisconnectWinFtp();
	}
	catch (ActivException &activException)
	{
		m_error += "Failed to connect server " + m_url.m_url + ", " + activException.ToString();
	}

#endif

	return statusCode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FtpHelper::DisconnectWinFtp()
{
	StatusCode statusCode = STATUS_CODE_FAILURE;

#if defined (ACTIV_PLATFORM_WINDOWS)

	if (!IsConnectedWinFtp())
		return STATUS_CODE_NOT_CONNECTED;

	if (0 != m_ftpSessionHandle)
		(void) ::InternetCloseHandle(m_ftpSessionHandle);

	if (0 != m_internetHandle)
		(void) ::InternetCloseHandle(m_internetHandle);

	m_ftpSessionHandle = m_internetHandle = 0;

	statusCode = STATUS_CODE_SUCCESS;

#endif

	return statusCode;

}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FtpHelper::IsConnectedWinFtp() const
{

#if defined (ACTIV_PLATFORM_WINDOWS)

	return (0 != m_ftpSessionHandle);

#endif

	return false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename Container> inline StatusCode FtpHelper::GetFileListWinFtp(std::insert_iterator<Container> it, const bool includeDirectories, const std::string &directory)
{
	StatusCode statusCode = STATUS_CODE_SUCCESS;

#if defined (ACTIV_PLATFORM_WINDOWS)

	if (!IsConnectedWinFtp())
		return STATUS_CODE_NOT_CONNECTED;

	WIN32_FIND_DATA findData;
	m_error.clear();

	std::string requestPath;
	if (directory == "/")
		requestPath = "";
	else
	{
		requestPath = directory;
		TidyFtpUrl(requestPath);
	}

	HINTERNET findHandle = ::FtpFindFirstFile(m_ftpSessionHandle, requestPath.c_str(), &findData, INTERNET_FLAG_RELOAD, 0);

	try
	{
		if (0 == findHandle)
			ACTIV_THROW(OsException, ::GetLastError());

		do
		{
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				FileInfo fileInfo;

				const std::string localFileName(findData.cFileName);
				fileInfo.m_name = std::string(findData.cFileName);

				fileInfo.m_size = static_cast<int64_t>((findData.nFileSizeHigh * (static_cast<int64_t>(MAXDWORD)+1)) + findData.nFileSizeLow);

				SYSTEMTIME stUtc;

				// Convert the last-write time to system time.
				FileTimeToSystemTime(&findData.ftLastWriteTime, &stUtc);

				fileInfo.m_fileTime = File::FileTime(stUtc.wYear , stUtc.wMonth, stUtc.wDay, stUtc.wHour, stUtc.wMinute, stUtc.wSecond);

				*it = fileInfo;
			}
		}
		while (::InternetFindNextFile(findHandle, &findData));

		ACTIV_THROW(OsException, ::GetLastError());
	}
	catch (OsException &osException)
	{
		if (0 != findHandle)
			(void) ::InternetCloseHandle(findHandle);

		if (ERROR_NO_MORE_FILES != osException.m_errorCode)
		{
			m_error += "Failed to Get File List from server " + m_url.m_url + ", WinFtp error - " + OsErrorToString(osException.m_errorCode) + ":" +  GetExtentedWinFtpErrorMessage();

			statusCode = STATUS_CODE_FAILURE;

			DisconnectWinFtp();
		}
	}

#endif

	return statusCode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FtpHelper::GetFileWinFtp(const FileInfo &fileInfo, const std::string &localFilename, const std::string &directory)
{
	StatusCode statusCode = STATUS_CODE_FAILURE;

#if defined (ACTIV_PLATFORM_WINDOWS)

	if (!IsConnectedWinFtp())
		return STATUS_CODE_NOT_CONNECTED;

	m_error.clear();
	const std::string &filename = fileInfo.m_name;

	try
	{
		std::string requestPath;
		if (directory == "/")
			requestPath = filename;
		else
			requestPath = directory + "/" + filename;

		TidyFtpUrl(requestPath);

		if (!FtpGetFile(m_ftpSessionHandle, requestPath.c_str() , localFilename.c_str(), FALSE, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_ASCII | INTERNET_FLAG_RELOAD, 0))
			ACTIV_THROW(OsException, ::GetLastError());

		statusCode = STATUS_CODE_SUCCESS;
	}
	catch (OsException &osException)
	{
		m_error += "Failed to Get File " + filename + " from server " + m_url.m_url + ", WinFtp error - " +  OsErrorToString(osException.m_errorCode) + ":" +  GetExtentedWinFtpErrorMessage();
		DisconnectWinFtp();
	}

#endif

	return statusCode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FtpHelper::DeleteFileWinFtp(const FileInfo &fileInfo, const std::string &directory)
{
	StatusCode statusCode = STATUS_CODE_FAILURE;

#if defined (ACTIV_PLATFORM_WINDOWS)

	if (!IsConnectedWinFtp())
		return STATUS_CODE_NOT_CONNECTED;

	m_error.clear();
	const std::string &filename = fileInfo.m_name;

	try
	{
		std::string requestPath;
		if (directory == "/")
			requestPath = filename;
		else
			requestPath = directory + "/" + filename;

		TidyFtpUrl(requestPath);

		if (!FtpDeleteFile(m_ftpSessionHandle, requestPath.c_str()))
			ACTIV_THROW(OsException, ::GetLastError());

		statusCode = STATUS_CODE_SUCCESS;
	}
	catch (OsException &osException)
	{
		m_error += "Failed to Delete File " + filename + " from server " + m_url.m_url + ", WinFtp error - " + OsErrorToString(osException.m_errorCode) + ":" +  GetExtentedWinFtpErrorMessage();

		DisconnectWinFtp();
	}

#endif

	return statusCode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FtpHelper::GetAndDeleteFileWinFtp(const FileInfo &fileInfo, const std::string &localFilename, const std::string &directory)
{
	StatusCode statusCode = GetFileWinFtp(fileInfo, localFilename, directory);
	if (STATUS_CODE_FAILURE == statusCode)
		return statusCode;

	return DeleteFileWinFtp(fileInfo, directory);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string FtpHelper::GetErrorString()
{
	std::string tempError = m_error;
	m_error = "";
	return tempError;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FtpHelper::Url::Url() : m_port(21)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FtpHelper::Url::Url(const std::string &url) : m_url(url), m_port(21)
{
	static const char protocolSeparator[] = "://";
	static const size_t  protocolSeparatorLength = std::strlen(protocolSeparator);
	static const char locationDelimiter = '/';
	static const char parameterDelimiters[] = ",?;&";

	size_t position, endPosition;
	std::string fixedString, parameterString;

	// split up optional parameters and fixed
	endPosition = url.find_first_of(parameterDelimiters);
	if (std::string::npos != endPosition)
	{
		fixedString.assign(url, 0, endPosition);
		parameterString.assign(url, endPosition + 1, std::string::npos);
	}
	else
		fixedString = url;

	// skip leading white space
	position = fixedString.find_first_not_of(" \t", 0);

	// first find the protocol separator
	endPosition = fixedString.find(protocolSeparator, position);
	if (std::string::npos == endPosition)
		ACTIV_THROW(ActivException, STATUS_CODE_INVALID_URL);

	// extract the protocol and advance position
	m_protocol.assign(fixedString, position, endPosition - position);
	if (0 == m_protocol.length())
		ACTIV_THROW(ActivException, STATUS_CODE_INVALID_URL);

	position = endPosition + protocolSeparatorLength;

	// find end of the host location
	endPosition = fixedString.find(locationDelimiter, position);

	if (std::string::npos == endPosition)
	{
		m_location.assign(fixedString, position, std::string::npos);
	}
	else
	{
		// extract the location and advance position
		m_location.assign(fixedString, position, endPosition - position);
		position = endPosition;

		// extract the path
		m_path.assign(fixedString, position, std::string::npos);
	}

	// parse the address to get hostname + port
	std::string::size_type pos = m_location.find_last_of(":");
	if (std::string::npos != pos)
	{
		m_host.assign(m_location, 0, pos);
		std::string portName(m_location, pos + 1);

		if (!StringToType(portName, m_port))
			ACTIV_THROW(ActivException, STATUS_CODE_INVALID_URL);
	}
	else
	{
		m_host = m_location;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t DummyCallbackFunction(void *ptr, size_t size, size_t nmemb, void *stream)
{
	return size * nmemb;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t GetFileListCallbackFunction(void *ptr, size_t size, size_t nmemb, void *stream)
{
	using namespace Activ;

	size_t realsize = size * nmemb;
	GetFileListCallbackStruct *pGetFileListCallbackStruct = static_cast<GetFileListCallbackStruct *>(stream);

	pGetFileListCallbackStruct->m_data.append(static_cast<char *>(ptr), realsize);
	return realsize;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t GetFileCallbackFunction(void *ptr, size_t size, size_t nmemb, void *stream)
{
	using namespace Activ;

	size_t realsize = size * nmemb;
	GetFileCallbackStruct *pGetFileCallbackStruct = (GetFileCallbackStruct *)(stream);
	File *localFile = pGetFileCallbackStruct->m_localFile;

	try
	{
		int64_t size = 0;

		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, localFile->GetSize(size));

		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, localFile->SetPosition(size));

		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, localFile->Write(ptr, realsize));

	}
	catch (ActivException &activException)
	{
		std::string filename;
		localFile->GetFileName(filename);

		(*pGetFileCallbackStruct->pError) = "Failed to save downloaded file " + filename + " - " + activException.ToString() + ". ";
	}

	return realsize;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void FtpHelper::TidyFtpUrl(std::string &url, size_t from)
{
	static const std::string singleSlash = "/";
	static const std::string doubleSlash = "//";

	for (int i = 0 ; i < 3; ++i)
	{
		std::string::size_type idx = url.find(doubleSlash, from);
		while (idx != std::string::npos)
		{
			url.replace(idx, doubleSlash.size(), singleSlash,0, singleSlash.size());
			idx = url.find(doubleSlash,  from + idx - 1);
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

#if defined (ACTIV_PLATFORM_WINDOWS)

inline std::string FtpHelper::GetExtentedWinFtpErrorMessage()
{
	DWORD error = 0;
	DWORD size = 0;

	InternetGetLastResponseInfo(&error, 0, &size);

	if (0 == size)
		return "";

	std::vector<char> buff(size+1);
	if (!InternetGetLastResponseInfo(&error, &buff[0], &size))
		return "";

	buff[size] = 0;

	return std::string(&buff[0]);
}

#endif

} // namespace Activ

#endif // !defined (ACTIV_FTP_HELPER_H)
