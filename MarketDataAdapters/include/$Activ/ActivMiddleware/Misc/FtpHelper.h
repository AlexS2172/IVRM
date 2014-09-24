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
#include "ActivMiddleware/Misc/FtpParse.h"

#include <string>
#include <vector>
#include <time.h>
#include <curl/curl.h>

namespace Activ
{

typedef std::vector<std::string> FtpFilenameList;

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
	 */
	FtpHelper(std::string server, std::string username, std::string password);

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
	 *
 	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode GetFileList(FtpFilenameList *pFileList, const bool includeDirectories);

	/**
	 *	@brief	Gets a list of all files on ftp site in current folder.
	 *
	 *	@param	fileList The list where the filenames will be put.
	 *	@param	includeDirectories Include directories in the list.
	 *
 	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	template<typename Contanier>
	StatusCode GetFileList(std::insert_iterator<Contanier> it, const bool includeDirectories);

	/**
	 *	@brief	Download a file from the ftp site.
	 *
	 *	@param	filename The filename to get from the ftp site.
	 *	@param	localFilename The local filename to copy the file to.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode GetFile(const std::string filename, const std::string localFilename);
	
	/**
	 *	@brief	Deletes a file from the ftp site.
	 *
	 *	@param	filename The filename to delete from the ftp site.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode DeleteFile(const std::string filename);

	/**
	 *	@brief	Gets the readable error string.
	 *
	 *	@retval	Error string
	 */
	std::string GetErrorString();

private:
	std::string		m_url;					///< The Url of the ftp site.
	std::string		m_usernamePassword;		///< The 'username':'password' used to connect to the ftp site.
	std::string		m_error;				///< A readable string of the last error.

	CURL *			m_curl;					///< The curl handle.
};

// ---------------------------------------------------------------------------------------------------------------------------------

struct GetFileListCallbackStruct
{
	//FtpFilenameList *	pFilenameList;
	//bool				includeDirectories;
	std::string			m_data;
	std::string *		pError;
};

struct GetFileCallbackStruct
{
	//std::string			filename;
	std::string *		pError;
	File		*		m_localFile;
};

// These need to be here as Curl is written in C and doesn't like calling functions in C++ classes.
size_t DummyCallbackFunction(void *ptr, size_t size, size_t nmemb, void *stream);
size_t GetFileListCallbackFunction(void *ptr, size_t size, size_t nmemb, void *stream);
size_t GetFileCallbackFunction(void *ptr, size_t size, size_t nmemb, void *stream);

// ---------------------------------------------------------------------------------------------------------------------------------

inline FtpHelper::FtpHelper() :
	m_curl(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FtpHelper::FtpHelper(std::string server, std::string username, std::string password) :
	m_curl(0),
	m_url("ftp://" + server),
	m_usernamePassword(username + ":" + password)
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
	StatusCode statusCode = STATUS_CODE_FAILURE;

	if (IsConnected())
		return STATUS_CODE_ALREADY_CONNECTED;

	m_error.clear();

	try
	{
		m_curl = curl_easy_init();

		if (0 == m_curl)
		{
			m_error = "Failed to Initialize Curl";
			return statusCode;
		}

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_URL, m_url.c_str()));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_USERPWD, m_usernamePassword.c_str()));
		
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1));
		
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, DummyCallbackFunction));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_perform(m_curl));

		curl_easy_reset(m_curl);

		statusCode = STATUS_CODE_SUCCESS;
	}
	catch (Exception &exception)
	{
		m_error += "Failed to Connect to server " + m_url + ", CURL error - " + curl_easy_strerror(static_cast<CURLcode>(exception.m_errorCode));
	
		Disconnect();
	}

	return statusCode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FtpHelper::Disconnect()
{
	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	if (0 != m_curl)
		curl_easy_cleanup(m_curl);

	m_curl = 0;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FtpHelper::IsConnected() const
{
	return (0 != m_curl);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FtpHelper::GetFileList(FtpFilenameList *pFileList, const bool includeDirectories)
{
	return GetFileList(std::insert_iterator<FtpFilenameList>(*pFileList, pFileList->begin()), includeDirectories);
}

template<typename Container> inline StatusCode FtpHelper::GetFileList(std::insert_iterator<Container> it, const bool includeDirectories)
{
	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	StatusCode statusCode = STATUS_CODE_FAILURE;
	GetFileListCallbackStruct getFileListCallbackStruct;

	m_error.clear();

	getFileListCallbackStruct.pError = &m_error;

	try
	{		
		curl_easy_reset(m_curl);
		
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_URL, m_url.c_str()));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_USERPWD, m_usernamePassword.c_str()));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_TRANSFERTEXT, 1));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, GetFileListCallbackFunction));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &getFileListCallbackStruct));
		
		struct SessionHandle *ddata = (struct SessionHandle *)m_curl;
		//ddata->set.set_url;

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_perform(m_curl));

		size_t start = 0, end = 0;
		struct ftpparse fileInfo;

		std::string &data = getFileListCallbackStruct.m_data;
		while((end = data.find_first_of("\r\n", start)) != data.npos)
		{
			if (ftpparse(&fileInfo, const_cast<char*>(data.c_str()) + start, static_cast<int>(end-start)))
			{
				if (includeDirectories || !fileInfo.flagtrycwd)
					*it = std::string(fileInfo.name, fileInfo.namelen);
			}
			else
			{
				m_error = "Failed to parse ftp LIST information - unknown format. ";
				break;
			}

			start = end+2;
		}

		statusCode = STATUS_CODE_SUCCESS;
	}
	catch (Exception &exception)
	{
		m_error += "Failed to Get File List from server " + m_url + ", CURL error - " + curl_easy_strerror(static_cast<CURLcode>(exception.m_errorCode));
	
		if (CURLE_COULDNT_CONNECT == exception.m_errorCode)
			Disconnect();
	}

	

	return statusCode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FtpHelper::GetFile(const std::string filename, const std::string localFilename)
{
	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	StatusCode statusCode = STATUS_CODE_FAILURE;
	std::string requestUrl(m_url + "/" + filename);
	GetFileCallbackStruct getFileCallbackStruct;

	m_error.clear();
	File localFile;
	
	getFileCallbackStruct.pError = &m_error;
	getFileCallbackStruct.m_localFile = &localFile;

	try
	{
		curl_easy_reset(m_curl);
		
		ACTIV_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, localFile.Create(localFilename, File::ACCESS_FLAG_WRITE, File::SHARE_FLAG_READ));
		
		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_URL, requestUrl.c_str()));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_USERPWD, m_usernamePassword.c_str()));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, GetFileCallbackFunction));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &getFileCallbackStruct));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_perform(m_curl));

		statusCode = STATUS_CODE_SUCCESS;
	}
	catch (Exception &exception)
	{
		m_error += "Failed to Get File " + filename + " from server " + m_url + ", CURL error - " + curl_easy_strerror(static_cast<CURLcode>(exception.m_errorCode));
	
		if (CURLE_COULDNT_CONNECT == exception.m_errorCode)
			Disconnect();
	}
	
	localFile.Close(); //Its ok even if the file was not created!
	if (statusCode != STATUS_CODE_SUCCESS)
		File::Remove(localFilename);


	

	return statusCode;
}


// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode FtpHelper::DeleteFile(const std::string filename)
{
	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	StatusCode statusCode = STATUS_CODE_FAILURE;
	struct curl_slist *list = NULL;
	std::string ftpCommand;

	ftpCommand = "DELE " + filename;
	list = curl_slist_append(list, ftpCommand.c_str());

	try
	{
		curl_easy_reset(m_curl);

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_URL, m_url.c_str()));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_USERPWD, m_usernamePassword.c_str()));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_QUOTE, list));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, DummyCallbackFunction));

		ACTIV_THROW_RESULT_IF(CURLE_OK, !=, curl_easy_perform(m_curl));

		statusCode = STATUS_CODE_SUCCESS;
	}
	catch (Exception &exception)
	{
		m_error += "Failed to Delete File " + filename + " from server " + m_url + ", CURL error - " + curl_easy_strerror(static_cast<CURLcode>(exception.m_errorCode));
	
		if (CURLE_COULDNT_CONNECT == exception.m_errorCode)
			Disconnect();
	}

	curl_slist_free_all(list);
	

	return statusCode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string FtpHelper::GetErrorString()
{
	return m_error;
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

} // namespace Activ

#endif // !defined (ACTIV_FTP_HELPER_H)
