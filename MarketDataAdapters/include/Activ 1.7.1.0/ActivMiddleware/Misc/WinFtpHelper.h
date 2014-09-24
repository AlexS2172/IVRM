/**
 *	@file	WinFtpHelper.h
 *
 *	@brief	Header file for the win FTP helper class which uses wininet
 */

#if (!defined ACTIV_MISC_WIN_FTP_HELPER_H)
#define ACTIV_MISC_WIN_FTP_HELPER_H

#include "ActivMiddleware/ActivBase/StatusCodeException.h"
#include "ActivMiddleware/ActivBase/OsException.h"
#include "ActivMiddleware/ActivBase/File.h"
#include "ActivMiddleware/Misc/FtpParse.h"

#include <wininet.h>

#include "IFtpHelper.h"

#include <string>
#include <vector>
#include <time.h>

namespace Activ
{

class WinFtpHelper : public IFtpHelper
{
public:
	WinFtpHelper(const std::string server, const std::string &username = "", const std::string &password = "", uint32_t port = 21);
	virtual ~WinFtpHelper();

	virtual StatusCodeHelper Connect();
	virtual StatusCodeHelper Disconnect();
	virtual StatusCodeHelper GetFileList(FileInfoList &fileInfoList, const std::string &remoteDirectory = "");
	virtual StatusCodeHelper GetFile(const std::string &remoteFilename, const std::string &localFilename, const std::string &remoteDirectory = "");
	virtual StatusCodeHelper DeleteFile(const std::string &remoteFilename, const std::string &remoteDirectory = "");
	virtual StatusCodeHelper DeleteFile(const FileInfoList &fileInfoList, const std::string &remoteDirectory = "");
	virtual StatusCodeHelper GetAndDeleteFile(const std::string &remoteFilename, const std::string &localFilename, const std::string &remoteDirectory = "");

	virtual bool IsConnected() const;

private:

	std::string GetExtentedWinFtpErrorMessage();

	HINTERNET		m_internetHandle;		///< The internet handle.
	HINTERNET		m_ftpSessionHandle;		///< The ftp session handle.

};

// ---------------------------------------------------------------------------------------------------------------------------------

inline WinFtpHelper::WinFtpHelper(const std::string server, const std::string &username, const std::string &password, uint32_t port) :
	IFtpHelper(server, username, password, port),
	m_internetHandle(0),
	m_ftpSessionHandle(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline WinFtpHelper::~WinFtpHelper()
{
	Disconnect();
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool WinFtpHelper::IsConnected() const
{
	return (0 != m_ftpSessionHandle);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper WinFtpHelper::Connect()
{
	if (IsConnected())
		return STATUS_CODE_ALREADY_CONNECTED;

	m_internetHandle = ::InternetOpen(0, INTERNET_OPEN_TYPE_DIRECT, 0, 0, 0);

	if (0 == m_internetHandle)
		return StatusCodeHelper(STATUS_CODE_FAILURE, OsErrorToString(::GetLastError()));

	m_ftpSessionHandle = ::InternetConnect(m_internetHandle, m_server.c_str(), static_cast<INTERNET_PORT>(m_port), m_username.c_str(), m_password.c_str(), INTERNET_SERVICE_FTP, 0, 0);
	if (0 == m_ftpSessionHandle)
		StatusCodeHelper(STATUS_CODE_FAILURE, OsErrorToString(::GetLastError()) + ":" + GetExtentedWinFtpErrorMessage());

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper WinFtpHelper::Disconnect()
{
	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	if (0 != m_ftpSessionHandle)
		(void) ::InternetCloseHandle(m_ftpSessionHandle);

	if (0 != m_internetHandle)
		(void) ::InternetCloseHandle(m_internetHandle);

	m_ftpSessionHandle = m_internetHandle = 0;

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper WinFtpHelper::GetFileList(FileInfoList &fileInfoList, const std::string &remoteDirectory)
{
	if (!IsConnected())
		return STATUS_CODE_NOT_CONNECTED;

	WIN32_FIND_DATA findData;
	HINTERNET findHandle = ::FtpFindFirstFile(m_ftpSessionHandle, remoteDirectory.c_str(), &findData, INTERNET_FLAG_RELOAD, 0);

	if ((0 == findHandle) && (ERROR_NO_MORE_FILES == ::GetLastError()))
		return StatusCodeHelper();

	if ((0 == findHandle) && (ERROR_NO_MORE_FILES != ::GetLastError()))
		return StatusCodeHelper(STATUS_CODE_FAILURE, OsErrorToString(::GetLastError()));

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

			fileInfoList.push_back(fileInfo);
		}
	}
	while (::InternetFindNextFile(findHandle, &findData));

	(void) ::InternetCloseHandle(findHandle);

	if (ERROR_NO_MORE_FILES != ::GetLastError())
	{
		Disconnect();
		return StatusCodeHelper(STATUS_CODE_FAILURE, OsErrorToString(::GetLastError()) + ":" + GetExtentedWinFtpErrorMessage());
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper WinFtpHelper::GetFile(const std::string &remoteFilename, const std::string &localFilename, const std::string &remoteDirectory)
{
	const std::string requestPath = remoteDirectory.empty() ?  remoteFilename : remoteDirectory + "/" + remoteFilename;

	if (!FtpGetFile(m_ftpSessionHandle, requestPath.c_str() , localFilename.c_str(), FALSE, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_ASCII | INTERNET_FLAG_RELOAD, 0))
		return StatusCodeHelper(STATUS_CODE_FAILURE, OsErrorToString(::GetLastError()) + ":" + GetExtentedWinFtpErrorMessage());

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper WinFtpHelper::DeleteFile(const std::string &remoteFilename, const std::string &remoteDirectory)
{
	const std::string requestPath = remoteDirectory.empty() ?  remoteFilename : remoteDirectory + "/" + remoteFilename;

	if (!FtpDeleteFile(m_ftpSessionHandle, requestPath.c_str()))
		return StatusCodeHelper(STATUS_CODE_FAILURE, OsErrorToString(::GetLastError()) + ":" + GetExtentedWinFtpErrorMessage());

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper WinFtpHelper::GetAndDeleteFile(const std::string &remoteFilename, const std::string &localFilename, const std::string &remoteDirectory)
{
	StatusCodeHelper statusCodeHelper = GetFile(remoteFilename, localFilename, remoteDirectory);
	if (STATUS_CODE_FAILURE == statusCodeHelper)
		return statusCodeHelper;

	return DeleteFile(remoteFilename, remoteDirectory);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCodeHelper WinFtpHelper::DeleteFile(const FileInfoList &fileInfoList, const std::string &remoteDirectory)
{
	for (FileInfoList::const_iterator it = fileInfoList.begin(), itEnd = fileInfoList.end(); it != itEnd; ++it)
	{
		StatusCodeHelper statusCodeHelper = DeleteFile(it->m_name, remoteDirectory);
		if (STATUS_CODE_FAILURE == statusCodeHelper)
			return statusCodeHelper;
	}

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string WinFtpHelper::GetExtentedWinFtpErrorMessage()
{
	DWORD error = 0;
	DWORD size = 0;

	InternetGetLastResponseInfo(&error, 0, &size);

	if (0 == size)
		return "";

	std::vector<char> buff(size + 1);
	if (!InternetGetLastResponseInfo(&error, &buff[0], &size))
		return "";

	buff[size] = 0;

	return std::string(&buff[0]);
}

} // namespace Activ

#endif // (!defined ACTIV_MISC_WIN_FTP_HELPER_H)
