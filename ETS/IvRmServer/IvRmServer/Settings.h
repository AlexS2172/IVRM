#pragma once

//---------------------------------------------------------------//
class CSettings
{
public:
	CSettings(void);
	~CSettings(void);

public:
	virtual void GetDBConnection(BSTR* bsConnect) = 0;
	virtual long GetGreeksCalculationMode() = 0;
	virtual long GetGreeksCalculationModel() = 0;
	virtual bool GetUseTimePrecision() = 0;
	virtual bool GetProcessTradeForAnalytics() = 0;
	virtual std::string GetAssetListFileName() = 0;
	virtual long GetLogLevel() = 0;
	virtual long GetLogLifetime() = 0;
	virtual long curve_interpolation_type() = 0;
	virtual long curve_extrapolation_type() = 0;
	virtual std::string GetLogDirectory() = 0;
};
//---------------------------------------------------------------//
class CXmlSettings: public CSettings
{
public:
	CXmlSettings(void);
	~CXmlSettings(void);

private:
	CXMLParamsHelper m_xmlParams;

public:
	void GetDBConnection(BSTR* bsConnect);
	long GetGreeksCalculationMode();
	long GetGreeksCalculationModel();
	bool GetUseTimePrecision();
	std::string GetAssetListFileName();
	long GetLogLevel();
	bool GetProcessTradeForAnalytics();
	long GetLogLifetime();
	long curve_interpolation_type();
	long curve_extrapolation_type();
	std::string GetLogDirectory();
};
typedef boost::shared_ptr<CSettings>	CSettingsPtr;
//---------------------------------------------------------------//