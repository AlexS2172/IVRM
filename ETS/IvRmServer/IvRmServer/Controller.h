#pragma once

class CController: public CMessenger
{
public:
	CController(void);
	virtual ~CController(void);

private:
	IEtsMain*				m_pMain;
	CSettingsPtr			m_spSettings;

public:
	bool					SetMain(IEtsMainPtr spMain);
	IEtsMainPtr				GetMain();

	void					SetSettingsImpl(CSettingsPtr spSettings);
	CSettingsPtr			GetSettingsImpl();
	void					LoadAssetList();

	void					LoadSettings();

public:
	void OnSettingsUpdate(CCalculationSettingsPtr spSettings);
	void OnUnderlyingUpdate(struct IUnderlyingUpdate* Data);
};

typedef boost::shared_ptr<CController>	CControllerPtr;
