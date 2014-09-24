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
	END_SINK_MAP()

	static _ATL_FUNC_INFO		m_fiOnTradeUpdate;

public:
	void			SubscribeTradeUpdate();
	void			UnSubscribeTradeUpdate();
	void			SetDBConnectionString(std::string dbconnection);
	
private:
	STDMETHOD(raw_OnTradeUpdate)(struct ITradeUpdate * Data);

	ISubManagerPtr	m_spSubManager;
	ISubManagerPtr	GetSubManager();
};
typedef boost::shared_ptr<CMessenger>	CMessengerPtr;
//-------------------------------------------------------------------------------------------------------------//