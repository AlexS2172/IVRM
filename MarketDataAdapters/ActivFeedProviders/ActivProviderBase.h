#pragma once
using namespace Activ;
//----------------------------------------------------------------------------------------//




typedef boost::shared_ptr<Activ::RequestId>				RequestIdPtr;

struct RequestIdPtr_less
	: public binary_function<RequestIdPtr, RequestIdPtr, bool>
{	// functor for operator<
	bool operator()(const RequestIdPtr& _Left, const RequestIdPtr& _Right) const
	{	// apply operator< to operands
		return (*_Left < *_Right);
	}
};


//////////////////////////////////////////////////////////////////////////////
class CRequestBase
{
public:
	enum RequestType
	{
		_enRequestNone,
		_enRequestStock,
		_enRequestOption,
		_enRequestFuture,
		_enRequestFutures,
		_enRequestFuturesOption,
		_enRequestLast,
		_enRequestRealtime,
		_enRequestGroup
	};
public:
	CRequestBase() : m_enType(_enRequestNone){}
	virtual ~CRequestBase(){}

	RequestType m_enType;
};



template<typename T>
class CRequest : public CRequestBase
{
public:
	typedef EgLib::CComRecPtr<T,	&LIBID_PRICEPROVIDERSLib>	CParams;
	typedef boost::shared_ptr<CParams >							CParamsPtr;
	typedef boost::shared_ptr<CRequest<T> >						CRequestPtr;

	CRequest(RequestType enType, T* Params) { m_enType = enType; m_recParams.Init(Params);}
	CParams  m_recParams;
};


template<typename T>
class CResponse : public CRequestBase
{
public:
	typedef EgLib::CComRecPtr<T,	&LIBID_PRICEPROVIDERSLib>	CParams;
	typedef boost::shared_ptr<CParams >							CParamsPtr;
	typedef boost::shared_ptr<CResponse<T> >					CResponsePtr;

	CResponse() {m_respParams.Init();}
	CParams  m_respParams;
};

typedef boost::shared_ptr<CRequestBase> CRequestBasePtr;

//----------------------------------------------------------------------------------------//
struct _QUI;
struct _QUP;

class CActivProviderBase
{
public:

	CActivProviderBase(void)
		:m_lUnnotifiedSubscriptions(0L)
	{
	}

	virtual ~CActivProviderBase(void)
	{
	}

public:
	virtual HRESULT		OnFuture( const CComVariant &varParams,const CComVariant &varResults ){return E_FAIL;}
	virtual HRESULT		OnFutureByRoot(const CComVariant &varParams,const CComVariant &varResults, const CComVariant &varIsLastFutures ){return E_FAIL;}
	virtual HRESULT		OnFuturesOption(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastOption){return E_FAIL;}
	virtual HRESULT		OnError(ErrorNumberEnum e, _bstr_t b, RequestsTypeEnum en, const CComVariant &v) { return E_FAIL; }
	virtual HRESULT		OnLastQuote( const CComVariant &varParams, const CComVariant &varResults )	{ return E_FAIL; }
	virtual HRESULT		OnOption(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastOption){return E_FAIL;}
	virtual void		OnRequstLast( QuoteUpdateParams& qup, QuoteUpdateInfo& qui )	{}	
	virtual HRESULT		OnStock(const CComVariant &varParams,const CComVariant &varResults){return E_FAIL;}
	virtual HRESULT		OnSubscribed( const CComVariant &varParams )	{ return E_FAIL; }
	virtual void		OnUnsubscribe( _QuoteUpdateParams* qup )	{}

public:
	volatile long       m_lUnnotifiedSubscriptions;
};
