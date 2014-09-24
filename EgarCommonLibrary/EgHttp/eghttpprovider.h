#ifndef __EGHTTP_PROVIDER_H__
#define __EGHTTP_PROVIDER_H__

#include "EgHttpCommon.h"

/////////////////////////////////////////////////////////////////////////////
// This class Provide core access to HTPP data
class EGHTTP_API CEgHttpProvider
{
public:
	CEgHttpProvider::CEgHttpProvider():
#ifdef _DEBUG
		m_dwThreadID(0),
#endif
		m_hFile(NULL), m_hSession(NULL), m_dwBufferSize(0){}

	virtual ~CEgHttpProvider()
	{
		CloseURL();
		CloseSession();
	}
	
	HINTERNET GetSessionHandle() { return m_hSession; }
	HINTERNET GetRequestHandle() { return m_hFile; }

	// Initialize http session.
	void InitSession(const CEgHttpConnectionParam& crcpParams);

	// Open url relative to server address.
	void OpenURL(CEgString strURL);

	// Read a portion of data. Returns false if no more data available. Control buffer size inside.
	bool ReadData(CEgString& rstrData);
	
	// Read all data. Use sequental calls of ReadData inside.
	virtual void ReadAllData(CEgString& rstrData);

	void CloseURL();

	// Close http sesstion and url
	void CloseSession();

protected:
	CEgHttpConnectionParam m_cpParams;
	HINTERNET m_hSession;
	HINTERNET m_hFile;

private:
	DWORD m_dwBufferSize;
	ATL::CHeapPtr<BYTE, ATL::CCRTAllocator> m_spbyBuffer;

#ifdef _DEBUG
protected:
	DWORD m_dwThreadID;
#endif
};

#endif // __EGHTTP_PROVIDER_H__
