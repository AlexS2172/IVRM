/**
 *	@file	CurlFtpHelper.h
 *
 *	@brief	Header file for the curl FTP helper class which user cUrl and ftpparser.
 *
 *	When using this file you will need to link to the curl library (CurlPackage_x86_win32_vc71_mdid.lib etc.).
 *
 *	$Log: $
 */

#if (!defined ACTIV_MISC_CURL_FTP_HELPER_H)
#define ACTIV_MISC_CURL_FTP_HELPER_H

#include "ActivMiddleware/ActivBase/StatusCodeException.h"
#include "ActivMiddleware/ActivBase/OsException.h"
#include "ActivMiddleware/ActivBase/File.h"
#include "ActivMiddleware/Misc/FtpParse.h"

#include "curl/curl.h"

#include "IFtpHelper.h"

#include <string>
#include <vector>
#include <time.h>

namespace Activ
{

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	CurlFtpHelper class.
 */
class CurlFtpHelper : public IFtpHelper
{
public:

	/**
     *	@brief	CurlInitializeHelper class.
     */
	class CurlInitializeHelper
	{
	public:
		/**
         *	@brief	Constuctor
         */
		CurlInitializeHelper();

		/**
         *	@brief	Destructor
         */
		~CurlInitializeHelper();
	};

	/**
	 *	@brief	Constuctor
	 *
	 *	@param	server the name of the ftp server.
	 *	@param	username the user name if required.
	 *	@param	password the password if required.
	 *	@param	port the ftp port to connect to.
	 *  @param  useEpsv something to do with 32 bit or 64 bit ip address etc
	 *  @param  curlLogFilename for detail curl ftp log
	 */
	CurlFtpHelper(const std::string &server, const std::string &username = "", const std::string &password = "", uint32_t port = IFtpHelper::DEFAULT_FTP_PORT, bool useEpsv = false, const std::string &curlLogFilename = "", uint32_t timeout = IFtpHelper::DEFAULT_FTP_TIMEOUT, uint32_t lowSpeedLimit = IFtpHelper::DEFAULT_FTP_LOW_SPEED_LIMIT, uint32_t lowSpeedTime = IFtpHelper::DEFAULT_FTP_LOW_SPEED_TIME);

	virtual ~CurlFtpHelper();

	virtual StatusCodeHelper Connect();
	virtual StatusCodeHelper Disconnect();
	virtual StatusCodeHelper GetFileList(FileInfoList &fileInfoList, const std::string &remoteDirectory = "");
	virtual StatusCodeHelper GetFile(const std::string &remoteFilename, const std::string &localFilename, const std::string &remoteDirectory = "");
	virtual StatusCodeHelper DeleteFile(const std::string &remoteFilename, const std::string &remoteDirectory = "");
	virtual StatusCodeHelper DeleteFile(const FileInfoList &fileInfoList, const std::string &remoteDirectory = "");
	virtual StatusCodeHelper GetAndDeleteFile(const std::string &remoteFilename, const std::string &localFilename, const std::string &remoteDirectory = "");

	virtual bool IsConnected() const;

	/**
	 *	@brief	Set functions
     */
	void SetTimeout(uint32_t timeout);
	void SetLowSpeedLimit(uint32_t lowSpeedLimit);
	void SetLowSpeedTime(uint32_t lowSpeedTime);
	void SetUseEpsv(bool useEpsv);
	void SetCurlLogFilename(const std::string &logFilename);

	/**
	 *	@brief	Get functions
     */
	uint32_t GetTimeout() const;
	uint32_t GetLowSpeedLimit() const;
	uint32_t GetLowSpeedTime() const;
	bool GetUseEpsv() const;

	/**
     *	@brief	Initialize the curl lib, once per app
     */
	static void Initialize();

	/**
     *	@brief	CleanUp the curl lib, once per app
     */
	static void CleanUp();

private:

	class CallbackData
	{
	public:
		virtual ~CallbackData();
		virtual size_t AppendData(void *ptr, size_t size);
	};

	class FileListCallbackData : public CallbackData
	{
	public:
		virtual size_t AppendData(void *ptr, size_t size);
		StatusCodeHelper GetFileList(FileInfoList &fileInfoList, bool includeDirectories);

	private:
		std::string m_data;
	};

	class GetFileCallbackData : CallbackData
	{
	public:
		GetFileCallbackData(const std::string &filename);
		virtual size_t AppendData(void *ptr, size_t size);

	private:
		File m_file;
	};

	static size_t CallbackFunction(void *ptr, size_t size, size_t nmemb, void *stream);

	std::string GetUrl() const;

	CURL 				*m_pCurl;					///< The curl "easy" handle.
	bool 				m_useEpsv;					///< To use EPSV or not
	FILE 				*m_pCurlLogFileHandle;		///< Curl's internal log
	uint32_t 			m_timeout;					///< Maximum time in seconds that you allow the libcurl transfer operation to take
	uint32_t 			m_lowSpeedLimit;			///< The transfer speed in bytes per second that the transfer should be below during CURLOPT_LOW_SPEED_TIME seconds for the library to consider it too slow and abort.
	uint32_t 			m_lowSpeedTime;				///< It contains the time in seconds that the transfer should be below the CURLOPT_LOW_SPEED_LIMIT for the library to consider it too slow and abort.
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline CurlFtpHelper::CurlInitializeHelper::CurlInitializeHelper()
{
	CurlFtpHelper::Initialize();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline CurlFtpHelper::CurlInitializeHelper::~CurlInitializeHelper()
{
	CurlFtpHelper::CleanUp();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void CurlFtpHelper::Initialize()
{
	curl_global_init(CURL_GLOBAL_ALL);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void CurlFtpHelper::CleanUp()
{
	curl_global_cleanup();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline CurlFtpHelper::CurlFtpHelper(const std::string &server, const std::string &username, const std::string &password, uint32_t port, bool useEpsv, const std::string &curlLogFilename, uint32_t timeout, uint32_t lowSpeedLimit, uint32_t lowSpeedTime) :
	IFtpHelper(server, username, password, port),
	m_pCurl(0),
	m_useEpsv(useEpsv),
	m_pCurlLogFileHandle(0),
	m_timeout(timeout),
	m_lowSpeedLimit(lowSpeedLimit),
	m_lowSpeedTime(lowSpeedTime)
{
	char *pEscapedUsername = curl_escape(username.c_str(), static_cast<int>(username.size())); 
	char *pEscapedPassword = curl_escape(password.c_str(), static_cast<int>(password.size()));
	
	m_username = std::string(pEscapedUsername);
	m_password = std::string(pEscapedPassword);

	curl_free(pEscapedUsername);
	curl_free(pEscapedPassword);

	if ("" != curlLogFilename)
		m_pCurlLogFileHandle = fopen(curlLogFilename.c_str(), "a+");
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline CurlFtpHelper::~CurlFtpHelper()
{
	Disconnect();
	if (!m_pCurlLogFileHandle)
		fclose(m_pCurlLogFileHandle);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool CurlFtpHelper::IsConnected() const
{
	return (0 != m_pCurl);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string CurlFtpHelper::GetUrl() const
{
	return "ftp://" + m_username + ":" + m_password + "@" + m_server + ":" + TypeToString(m_port) + "/";
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper CurlFtpHelper::Connect()
{
	if (IsConnected())
		return STATUS_CODE_ALREADY_CONNECTED;

	m_pCurl = curl_easy_init();

	if (0 == m_pCurl)
		return StatusCodeHelper(STATUS_CODE_FAILURE, "failed to to initialize curl");
	try
	{
		const std::string url = GetUrl();

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_URL, url.c_str()));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, 1));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_NOBODY, 1));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, CallbackFunction));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, m_timeout));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_LOW_SPEED_LIMIT, m_lowSpeedLimit));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_LOW_SPEED_TIME, m_lowSpeedTime));

		if (!m_useEpsv)
			ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_FTP_USE_EPSV, 0));

		if (NULL != m_pCurlLogFileHandle)
		{
			ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_STDERR , m_pCurlLogFileHandle));
			ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE , 1));
		}

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_perform(m_pCurl));
	}
	catch (Exception &exception)
	{
		Disconnect();
		return StatusCodeHelper(STATUS_CODE_FAILURE, curl_easy_strerror(static_cast<CURLcode>(exception.m_errorCode)));
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper CurlFtpHelper::Disconnect()
{
	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	if (0 != m_pCurl)
		curl_easy_cleanup(m_pCurl);

	m_pCurl = 0;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper CurlFtpHelper::GetFileList(FileInfoList &fileinfoList, const std::string &remoteDirectory)
{
	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	FileListCallbackData fileListCallBackData;

	try
	{
		const std::string url = (remoteDirectory.empty() ? GetUrl() : GetUrl() + remoteDirectory + "/");

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_URL, url.c_str()));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_NOBODY, 0));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &fileListCallBackData));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_perform(m_pCurl));
	}
	catch (Exception &exception)
	{
		if (CURLE_COULDNT_CONNECT == exception.m_errorCode)
			Disconnect();

		return StatusCodeHelper(STATUS_CODE_FAILURE, curl_easy_strerror(static_cast<CURLcode>(exception.m_errorCode)));
	}

	return fileListCallBackData.GetFileList(fileinfoList, false);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper CurlFtpHelper::GetFile(const std::string &remoteFilename, const std::string &localFilename, const std::string &remoteDirectory)
{
	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	GetFileCallbackData getFileCallbackData(localFilename);

	try
	{
		const std::string url = (remoteDirectory.empty() ? GetUrl() + remoteFilename : GetUrl() + remoteDirectory + "/" + remoteFilename);

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_URL, url.c_str()));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_NOBODY, 0));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &getFileCallbackData));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_perform(m_pCurl));
	}
	catch (Exception &exception)
	{
		if (CURLE_COULDNT_CONNECT == exception.m_errorCode)
			Disconnect();

		return StatusCodeHelper(STATUS_CODE_FAILURE, curl_easy_strerror(static_cast<CURLcode>(exception.m_errorCode)));
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper CurlFtpHelper::DeleteFile(const std::string &remoteFilename, const std::string &remoteDirectory)
{
	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	struct curl_slist *list = 0;

	try
	{
		std::string ftpCommand =  "DELE " + remoteFilename;
		list = curl_slist_append(list, ftpCommand.c_str());

		const std::string url = (remoteDirectory.empty() ? GetUrl() : GetUrl() + remoteDirectory + "/");
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_URL, url.c_str()));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_NOBODY, 1));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_QUOTE, list));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_perform(m_pCurl));
	}
	catch (Exception &exception)
	{
		if (CURLE_COULDNT_CONNECT == exception.m_errorCode)
			Disconnect();

		curl_easy_setopt(m_pCurl, CURLOPT_QUOTE, 0);
		curl_slist_free_all(list);
		return StatusCodeHelper(STATUS_CODE_FAILURE, curl_easy_strerror(static_cast<CURLcode>(exception.m_errorCode)));
	}

	curl_easy_setopt(m_pCurl, CURLOPT_QUOTE, 0);
	curl_slist_free_all(list);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper CurlFtpHelper::DeleteFile(const FileInfoList &fileInfoList, const std::string &remoteDirectory)
{
	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	struct curl_slist *list = 0;

	try
	{
		for (FileInfoList::const_iterator it = fileInfoList.begin(), itEnd = fileInfoList.end(); it != itEnd; ++it)
		{
			std::string ftpCommand = "DELE " + it->m_name;
			list = curl_slist_append(list, ftpCommand.c_str());
		}

		const std::string url = (remoteDirectory.empty() ? GetUrl() : GetUrl() + remoteDirectory + "/");
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_URL, url.c_str()));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_NOBODY, 1));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_QUOTE, list));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_perform(m_pCurl));
	}
	catch (Exception &exception)
	{
		if (CURLE_COULDNT_CONNECT == exception.m_errorCode)
			Disconnect();

		curl_easy_setopt(m_pCurl, CURLOPT_QUOTE, 0);
		curl_slist_free_all(list);
		return StatusCodeHelper(STATUS_CODE_FAILURE, curl_easy_strerror(static_cast<CURLcode>(exception.m_errorCode)));
	}

	curl_easy_setopt(m_pCurl, CURLOPT_QUOTE, 0);
	curl_slist_free_all(list);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper CurlFtpHelper::GetAndDeleteFile(const std::string &remoteFilename, const std::string &localFilename, const std::string &remoteDirectory)
{
	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	struct curl_slist *list = 0;
	GetFileCallbackData getFileCallbackData(localFilename);

	try
	{
		std::string ftpCommand = "DELE " + remoteFilename;
		list = curl_slist_append(list, ftpCommand.c_str());

		const std::string url = (remoteDirectory.empty() ? GetUrl() + remoteFilename : GetUrl() + remoteDirectory + "/" + remoteFilename);

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_URL, url.c_str()));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_NOBODY, 0));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &getFileCallbackData));
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_pCurl, CURLOPT_POSTQUOTE, list));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_perform(m_pCurl));
	}
	catch (Exception &exception)
	{
		if (CURLE_COULDNT_CONNECT == exception.m_errorCode)
			Disconnect();

		curl_easy_setopt(m_pCurl, CURLOPT_POSTQUOTE, 0);
		curl_slist_free_all(list);

		return StatusCodeHelper(STATUS_CODE_FAILURE, curl_easy_strerror(static_cast<CURLcode>(exception.m_errorCode)));
	}

	curl_easy_setopt(m_pCurl, CURLOPT_POSTQUOTE, 0);
	curl_slist_free_all(list);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t CurlFtpHelper::CallbackFunction(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t realsize = size * nmemb;
	CallbackData *pCallBackData = static_cast<CallbackData *>(stream);

	return pCallBackData->AppendData(ptr, realsize);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline CurlFtpHelper::CallbackData::~CallbackData()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t CurlFtpHelper::CallbackData::AppendData(void *ptr, size_t size)
{
	return size;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper CurlFtpHelper::FileListCallbackData::GetFileList(FileInfoList &fileInfoList, bool includeDirectories)
{
	size_t start = 0, end = 0;
	struct ftpparse parseInfo;

	std::string &data = m_data;
	while((end = data.find_first_of("\r\n", start)) != data.npos)
	{
		if (ftpparse(&parseInfo, const_cast<char*>(data.c_str()) + start, static_cast<int>(end - start)))
		{
			FileInfo fileInfo;

			if (!(includeDirectories || !parseInfo.flagtrycwd))
			{
				start = end + 2;
				continue;
			}

			fileInfo.m_name = std::string(parseInfo.name, parseInfo.namelen);
			fileInfo.m_size = static_cast<int64_t>(parseInfo.size);

			if (FTPPARSE_MTIME_UNKNOWN != parseInfo.mtimetype)
			{
				fileInfo.m_fileTime = File::FileTime(parseInfo.mtime);
			}

			fileInfoList.push_back(fileInfo);
		}
		else
			return StatusCodeHelper(STATUS_CODE_FAILURE, "unable to parse file list");

		start = end + 2;
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t CurlFtpHelper::FileListCallbackData::AppendData(void *ptr, size_t size)
{
	m_data.append(static_cast<char *>(ptr), size);

	return size;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline CurlFtpHelper::GetFileCallbackData::GetFileCallbackData(const std::string &filename)
{
	StatusCode statusCode = m_file.Create(filename, File::ACCESS_FLAG_WRITE, File::SHARE_FLAG_READ);

	if (statusCode != STATUS_CODE_SUCCESS)
		ACTIV_THROW(OsException, statusCode);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline size_t CurlFtpHelper::GetFileCallbackData::AppendData(void *ptr, size_t size)
{
	try
	{
		int64_t fileSize = 0;

		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_file.GetSize(fileSize));
		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_file.SetPosition(fileSize));
		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_file.Write(ptr, size));
	}
	catch (StatusCodeException &statusCodeException)
	{
		LogMessage(LOG_TYPE_ERROR, "Failed to write data to file " + statusCodeException.ToString());
		return 0;
	}

	return size;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void CurlFtpHelper::SetTimeout(uint32_t timeout)
{
	m_timeout = timeout;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void CurlFtpHelper::SetLowSpeedLimit(uint32_t lowSpeedLimit)
{
	m_lowSpeedLimit = lowSpeedLimit;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void CurlFtpHelper::SetLowSpeedTime(uint32_t lowSpeedTime)
{
	m_lowSpeedTime = lowSpeedTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void CurlFtpHelper::SetUseEpsv(bool useEpsv)
{
	m_useEpsv = useEpsv;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void CurlFtpHelper::SetCurlLogFilename(const std::string &logFilename)
{
	if ("" != logFilename)
	{
		if (!m_pCurlLogFileHandle)
			fclose(m_pCurlLogFileHandle);

		m_pCurlLogFileHandle = fopen(logFilename.c_str(), "a+");
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t CurlFtpHelper::GetTimeout() const
{
	return m_timeout;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t CurlFtpHelper::GetLowSpeedLimit() const
{
	return m_lowSpeedLimit;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline uint32_t CurlFtpHelper::GetLowSpeedTime() const
{
	return m_lowSpeedTime;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool CurlFtpHelper::GetUseEpsv() const
{
	return m_useEpsv;
}

} // namespace Activ

#endif // (!defined ACTIV_MISC_CURL_FTP_HELPER_H)
