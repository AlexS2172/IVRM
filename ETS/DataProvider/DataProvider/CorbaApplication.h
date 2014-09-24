#pragma once

class CCorbaApplication
{
public:

	void Run(const ORBParametersList& orbParameters, const std::string& objectName);
	bool IsStarted();
	bool IsStopping();
	void Stop();

	void SetLogLevel(const int logLevel);
	long GetLogLevel() const;

protected:
	CCorbaApplication(CSettings::pointer settings);
	virtual ~CCorbaApplication(void);

	virtual void PreORBInit(const std::string& objectName);
	virtual void PostORBInit(const std::string& objectName);

	CORBA::ORB_ptr		GetORB();
	CSettings::pointer	m_Settings;

private:
	class		CApplicationImpl;
	static		CApplicationImpl*		m_pImpl;
	static		size_t					m_refCount;	

	static BOOL ConsoleCtrlHandler(DWORD ctrlType); 
	
};
