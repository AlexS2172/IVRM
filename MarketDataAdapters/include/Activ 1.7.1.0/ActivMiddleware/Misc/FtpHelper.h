/**
 *	@file	FtpHelper.h
 *
 *	@brief	Header file for the FTP helper class which user cUrl and ftpparser.
 *
 *	When using this file you will need to link to the curl library (CurlPackage_x86_win32_vc71_mdid.lib etc.).
 *
 *	$Log: $
 */

#if (!defined ACTIV_MISC_FTP_HELPER_H)
#define ACTIV_MISC_FTP_HELPER_H

#include "ActivMiddleware/ActivBase/StatusCodeException.h"
#include "ActivMiddleware/ActivBase/OsException.h"
#include "ActivMiddleware/ActivBase/File.h"
#include "ActivMiddleware/Misc/FtpParse.h"

#include "curl/curl.h"

#include <string>
#include <vector>
#include <time.h>

#include "IFtpHelper.h"
#include "CurlFtpHelper.h"

#if (defined ACTIV_PLATFORM_WINDOWS)
#include "WinFtpHelper.h"
#endif

namespace Activ
{

/**
 *	@brief	FtpHelper class
 */
class FtpHelper
{
public:

	/**
	 *  @brief	Ftp type
	 */
	 enum FtpType
	 {
	 	CURL_FTP,
	 	WIN_FTP,
	 };

	/**
	 *	@brief	Constuctor
	 *
	 *	@param	server the name of the ftp server.
	 *	@param	username the user name if required.
	 *	@param	password the password if required.
	 *	@param	port the ftp port to connect to.
	 */
	 FtpHelper(const std::string &server, const std::string &username = "", const std::string &password = "", uint32_t port = IFtpHelper::DEFAULT_FTP_PORT, FtpType ftpType = CURL_FTP);

	/**
	 *	@brief	Destructor
	 */
	 ~FtpHelper();

	/**
	 *	@brief	Connect to the server
	 */
	StatusCodeHelper Connect();

	/**
	 *	@brief	Disconnects from the server
	 */
	StatusCodeHelper Disconnect();

	/**
	 *	@brief	Get the names of files from the server
	 *
	 *	@param	fileList the names of the files found on the server.
	 *	@param	remoteDirectory the from which to get the file names from
	 */
	StatusCodeHelper GetFileList(FileInfoList &fileList, const std::string &remoteDirectory = "");

	/**
	 *	@brief	Get a file
	 *
	 *	@param	remoteFilename the name of the file to get
	 *	@param	localFilename the save as name of the remoteFilename
	 *	@param	remoteDirectory where the remoteFilename lives
	 */
	StatusCodeHelper GetFile(const std::string &remoteFilename, const std::string &localFilename, const std::string &remoteDirectory = "");

	/**
	 *	@brief	Delete a file
	 *
	 *	@param	remoteFilename the name of the file to delete
	 *	@param	remoteDirectory where the remoteFilename lives
	 */
	StatusCodeHelper DeleteFile(const std::string &remoteFilename, const std::string &remoteDirectory = "");


	StatusCodeHelper DeleteFile(const FileInfoList &fileInfoList, const std::string &remoteDirectory = "");

	/**
	 *	@brief	Get and delete a file
	 *
	 *	@param	remoteFilename the name of the file to get and delete
	 *	@param	localFilename the save as name of the remoteFilename
	 *	@param	remoteDirectory where the remoteFilename lives
	 */
	StatusCodeHelper GetAndDeleteFile(const std::string &remoteFilename, const std::string &localFilename, const std::string &remoteDirectory = "");

	/**
	 *	@brief	Is connected
	 */
	bool IsConnected() const;

protected:

	IFtpHelper *m_pFtpHelper;	///< ftp helper

};

// ---------------------------------------------------------------------------------------------------------------------------------

inline FtpHelper::FtpHelper(const std::string &server, const std::string &username, const std::string &password, uint32_t port, FtpType ftpType) :
	m_pFtpHelper(0)
{
	switch (ftpType)
	{

#if (defined ACTIV_PLATFORM_WINDOWS)
		case WIN_FTP:
			m_pFtpHelper = new WinFtpHelper(server, username, password, port);
			break;
#endif

		case CURL_FTP:
			m_pFtpHelper = new CurlFtpHelper(server, username, password, port);
			break;

		default:
			ACTIV_THROW(StatusCodeException, STATUS_CODE_INVALID_PARAMETER);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline FtpHelper::~FtpHelper()
{
	delete m_pFtpHelper;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool FtpHelper::IsConnected() const
{
	return m_pFtpHelper->IsConnected();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper FtpHelper::Connect()
{
	return m_pFtpHelper->Connect();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper FtpHelper::Disconnect()
{
	return m_pFtpHelper->Disconnect();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper FtpHelper::GetFileList(FileInfoList &fileinfoList, const std::string &remoteDirectory)
{
	return m_pFtpHelper->GetFileList(fileinfoList, remoteDirectory);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper FtpHelper::GetFile(const std::string &remoteFilename, const std::string &localFilename, const std::string &remoteDirectory)
{
	return m_pFtpHelper->GetFile(remoteFilename, localFilename, remoteDirectory);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper FtpHelper::DeleteFile(const std::string &remoteFilename, const std::string &remoteDirectory)
{
	return m_pFtpHelper->DeleteFile(remoteFilename, remoteDirectory);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper FtpHelper::DeleteFile(const FileInfoList &fileInfoList, const std::string &remoteDirectory)
{
	return m_pFtpHelper->DeleteFile(fileInfoList, remoteDirectory);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper FtpHelper::GetAndDeleteFile(const std::string &remoteFilename, const std::string &localFilename, const std::string &remoteDirectory)
{
	return m_pFtpHelper->GetAndDeleteFile(remoteFilename, localFilename, remoteDirectory);
}

} // namespace Activ

#endif // (!defined ACTIV_MISC_FTP_HELPER_H)
