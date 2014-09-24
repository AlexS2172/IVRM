#pragma once
//-------------------------------------------------------------------------------------------------------------//
class CMessenger: public IDispEventSimpleImpl<0x1, CMessenger, &DIID__IOnMessages>
{
public:
	CMessenger(void);
	~CMessenger(void);

public:
	BEGIN_SINK_MAP(CMessenger)
		SINK_ENTRY_INFO(0x1, DIID__IOnMessages, DISPID_OnTradeUpdate, raw_OnTradeUpdate, &m_fiOnTradeUpdate)
		SINK_ENTRY_INFO(0x1, DIID__IOnMessages, DISPID_OnSettingsUpdate, raw_OnSettingsUpdate, &m_fiOnTradeUpdate)
		SINK_ENTRY_INFO(0x1, DIID__IOnMessages, DISPID_OnUnderlyingUpdate, raw_OnUnderlyingUpdate, &m_fiOnTradeUpdate)
	END_SINK_MAP()

	static _ATL_FUNC_INFO		m_fiOnTradeUpdate;
	static _ATL_FUNC_INFO		m_fiOnSettingsUpdate;
	static _ATL_FUNC_INFO		m_fiOnUnderlyingUpdate;

public:
	void			SubscribeTradeUpdate();
	void			UnSubscribeTradeUpdate();

	void			SubscribeSettingsUpdate();
	void			UnSubscribeSettingsUpdate();

	void			SubscribeUnderlyingUpdate();
	void			UnSubscribeUnderlyingUpdate();
public:
	virtual	void	OnTradeUpdate(CTradePtr spTrade) {};
	virtual void	OnSettingsUpdate(CCalculationSettingsPtr spSettings) {};
	virtual void	OnUnderlyingUpdate(struct IUnderlyingUpdate* Data) {};
	
private:
	STDMETHOD(raw_OnTradeUpdate)(struct ITradeUpdate * Data);
	STDMETHOD(raw_OnSettingsUpdate)(struct ISettingsUpdate * Data);
	STDMETHOD(raw_OnUnderlyingUpdate)(struct IUnderlyingUpdate * Data);

	ISubManagerPtr	m_spSubManager;
	ISubManagerPtr	GetSubManager();
};
typedef boost::shared_ptr<CMessenger>	CMessengerPtr;
//-------------------------------------------------------------------------------------------------------------//