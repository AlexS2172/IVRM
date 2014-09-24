#ifndef __ATL_EXTENTION_H_
#define __ATL_EXTENTION_H_

/*****************************************************************************
	There is some ATL extetions
		Free threaded marshaler macros
		Collection (on vector & on map) templates
		Collection utility templates
		ISupportErrorInfo ext. macros
*****************************************************************************/
#pragma warning(disable : 4530)

#include <map>
#include <vector>

#include <atlcom.h>

/*****************************************
 Free threaded marshler extention

 include this marco in your Com class
 declaration, call CreateUnkMarshaler 
 in FinalConstruct and ReleaseUnkMarshaler 
 in FinalRelease.
******************************************/
#define		DECLARE_UNKNOWN_MARSHALER_FUNC					\
															\
	CComPtr<IUnknown> m_pUnkMarshaler;						\
															\
	HRESULT CreateUnkMarshaler()							\
	{														\
		m_pUnkMarshaler = NULL;								\
															\
		return CoCreateFreeThreadedMarshaler(				\
			GetControllingUnknown(), &m_pUnkMarshaler.p);	\
	}														\
	void ReleaseUnkMarshaler()								\
	{														\
		m_pUnkMarshaler.Release();							\
	}														\


/*************************************
	_CopyToVariant<T>

	This template needed for
	IEnum_VARIANT realization in 
	collection
	
	 example:
		_CopyToVariant<IExample*>
		_CopyToVariant<BSTR>
**************************************/
template <typename T>
class _CopyToVariant 
{
public:
	static HRESULT copy(VARIANT* p1, T* p2) 
	{
		if (!p2) return E_POINTER;
		CComVariant	var(*p2);
		var.Detach(p1);
		return S_OK;
	}
	static void init(VARIANT* p)
	{
		VariantInit(p);
	}
	static void destroy(VARIANT* p)
	{
		VariantClear(p);
	}
};

/*************************************
	IEnumOnSTLMapImpl<...>

	This template needed for
	IEnum_XXX realization on 
	STL map
	
	 example:
		_CopyToVariant<IExample*>
		_CopyToVariant<BSTR>
**************************************/
template <class Base, const IID* piid, class T, class Copy, class CollType>
class ATL_NO_VTABLE IEnumOnSTLMapImpl : public Base
{
public:
	HRESULT Init(IUnknown *pUnkForRelease, CollType& collection)
	{
		m_spUnk = pUnkForRelease;
		m_pcollection = &collection;
		m_iter = m_pcollection->begin();
		return S_OK;
	}
	STDMETHOD(Next)(ULONG celt, T* rgelt, ULONG* pceltFetched);
	STDMETHOD(Skip)(ULONG celt);
	STDMETHOD(Reset)(void)
	{
		if (m_pcollection == NULL)
			return E_FAIL;
		m_iter = m_pcollection->begin();
		return S_OK;
	}
	STDMETHOD(Clone)(Base** ppEnum);

	CComPtr<IUnknown> m_spUnk;
	CollType* m_pcollection;
	typename CollType::iterator m_iter;
};

template <class Base, const IID* piid, class T, class Copy, class CollType>
STDMETHODIMP IEnumOnSTLMapImpl<Base, piid, T, Copy, CollType>::Skip(ULONG celt)
{
	HRESULT hr = S_OK;
	while (celt--)
	{
		if (m_iter != m_pcollection->end())
			m_iter++;
		else
		{
			hr = S_FALSE;
			break;
		}
	}
	return hr;
}

template <class Base, const IID* piid, class T, class Copy, class CollType>
STDMETHODIMP IEnumOnSTLMapImpl<Base, piid, T, Copy, CollType>::Next(ULONG celt, T* rgelt,
	ULONG* pceltFetched)
{
	if (rgelt == NULL || (celt != 1 && pceltFetched == NULL))
		return E_POINTER;
	if (m_pcollection == NULL)
		return E_FAIL;

	ULONG nActual = 0;
	HRESULT hr = S_OK;
	T* pelt = rgelt;
	while (SUCCEEDED(hr) && m_iter != m_pcollection->end() && nActual < celt)
	{
		hr = Copy::copy(pelt, &(*m_iter).second);
		if (FAILED(hr))
		{
			while (rgelt < pelt)
				Copy::destroy(rgelt++);
			nActual = 0;
		}
		else
		{
			pelt++;
			m_iter++;
			nActual++;
		}
	}
	if (pceltFetched)
		*pceltFetched = nActual;
	if (SUCCEEDED(hr) && (nActual < celt))
		hr = S_FALSE;
	return hr;
}

template <class Base, const IID* piid, class T, class Copy, class CollType>
STDMETHODIMP IEnumOnSTLMapImpl<Base, piid, T, Copy, CollType>::Clone(Base** ppEnum)
{
	typedef CComObject<CComEnumOnSTLMap<Base, piid, T, Copy, CollType> > _class;
	HRESULT hr = E_POINTER;
	if (ppEnum != NULL)
	{
		*ppEnum = NULL;
		_class* p;
		hr = _class::CreateInstance(&p);
		if (SUCCEEDED(hr))
		{
			hr = p->Init(m_spUnk, *m_pcollection);
			if (SUCCEEDED(hr))
			{
				p->m_iter = m_iter;
				hr = p->_InternalQueryInterface(*piid, (void**)ppEnum);
			}
			if (FAILED(hr))
				delete p;
		}
	}
	return hr;
}

template <class Base, const IID* piid, class T, class Copy, class CollType, class ThreadModel = CComObjectThreadModel>
class ATL_NO_VTABLE CComEnumOnSTLMap :
	public IEnumOnSTLMapImpl<Base, piid, T, Copy, CollType>,
	public CComObjectRootEx< ThreadModel >
{
public:
	typedef CComEnumOnSTLMap<Base, piid, T, Copy, CollType, ThreadModel > _CComEnum;
	typedef IEnumOnSTLMapImpl<Base, piid, T, Copy, CollType > _CComEnumBase;
	BEGIN_COM_MAP(_CComEnum)
		COM_INTERFACE_ENTRY_IID(*piid, _CComEnumBase)
	END_COM_MAP()
};

/*
	Often using enumerators templates
*/
template <class T, class CollType>
class CComEnumVariantOnSTL :
	public CComEnumOnSTL<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _CopyToVariant<T>, CollType> 
{
};

template <class V>
class CComEnumVariantOnSTLVector : 
	public CComEnumOnSTL<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, 
			_CopyToVariant<V>, std::vector<V> > 
{
};

template <class K, class V>
class CComEnumVariantOnSTLMap : 
	public CComEnumOnSTLMap<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, 
			_CopyToVariant<V>, std::map<K, V> > 
{
};

template <class K, class V>
class CComEnumVariantOnSTLMultiMap : 
	public CComEnumOnSTLMap<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, 
			_CopyToVariant<V>, std::multimap<K, V> > 
{
};

template <class T, typename ItemType, class Copy, class EnumType>
class ICollectionOnSTLVectorImpl : 
public ICollectionOnSTLImpl<T, std::vector <ItemType>, ItemType, Copy, EnumType>
{
public:
	typedef		std::vector <ItemType>			CollType;
	typedef		typename CollType::iterator		IterType;

	STDMETHOD(Add)(ItemType Value) 
	{
		HRESULT		hr;
		ItemType	item;
		Copy::init(&item);
		hr = Copy::copy(&item, &Value);
		if (FAILED(hr)) return hr;
		m_coll.push_back(item);
		return S_OK;
	}
	STDMETHOD(Clear)()
	{
		IterType	it = m_coll.begin();
		for (;it != m_coll.end(); it++) 
			Copy::destroy(&*it);
		m_coll.clear();
		return S_OK;
	}
	STDMETHOD(Remove)(LONG Index)
	{
		if (Index >= m_coll.size())
			return E_FAIL;
		IterType	it		= m_coll.begin() + Index;
		Copy::destroy(&*it); 
		m_coll.erase(it);
		return S_OK;
	}
};

template <class T, class KeyType, class ItemType, class Copy, class EnumType, class KeyClass >
class ICollectionOnSTLMapImpl : 
	public T
{
public:
	typedef		std::map<KeyClass, ItemType>		CollType;
	typedef		std::pair<KeyClass, ItemType>		PairType;


	typedef		typename CollType::iterator			IterType;

 	STDMETHOD(get_Count)(LONG* pCount)
	{
		if (pCount == NULL)
			return E_POINTER;
		*pCount = (LONG)m_coll.size();
		return S_OK;
	}
	STDMETHOD(get_Item)(KeyType Key, ItemType* pValue)
	{
		if (!pValue)
			return E_POINTER;
		KeyClass	_key	= Key;
		IterType	it		= m_coll.find(_key);
		if (it != m_coll.end())
		{
			Copy::init(pValue);
			return Copy::copy(pValue, &(*it).second);
		}
		else
		{
			*pValue = NULL;
			return S_OK;
		}
	}
	STDMETHOD(Add)(KeyType Key, ItemType Value)
	{
		KeyClass	_key	= Key;
		IterType	it		= m_coll.find(_key);
		ItemType	_item;
		HRESULT		hr;
		if (it != m_coll.end())
			return E_FAIL;
		Copy::init(&_item);
		if (FAILED(hr = Copy::copy(&_item, &Value)))
			return hr;
		m_coll[_key] = _item;
		return S_OK;
	}
	STDMETHOD(Remove)(KeyType Key)
	{
		KeyClass	_key	= Key;
		IterType	it		= m_coll.find(_key);
		if (it == m_coll.end())
			return E_FAIL;
		Copy::destroy(&(*it).second);
		m_coll.erase(_key);
		return S_OK;
	}
	STDMETHOD(Clear)()
	{
		IterType it = m_coll.begin();
		for (;it != m_coll.end(); it++) 
			Copy::destroy(&(*it).second);
		m_coll.clear();
		return S_OK;
	}
	STDMETHOD(get__NewEnum)(IUnknown** ppUnk)
	{
		if (ppUnk == NULL)
			return E_POINTER;
		*ppUnk = NULL;
		HRESULT hr = S_OK;
		CComObject<EnumType>* p;
		hr = CComObject<EnumType>::CreateInstance(&p);
		if (SUCCEEDED(hr))
		{
			hr = p->Init(this, m_coll);
			if (hr == S_OK)
				hr = p->QueryInterface(IID_IUnknown, (void**)ppUnk);
		}
		if (hr != S_OK)
			delete p;
		return hr;
	}

	CollType m_coll;
};

// Sharky: this class may need a redesign - you are welcome to improve it.
//			The idea is to have one access key and another sort key (enum key) in the same collection.
//			This collection allow to have duplicate sortkeys.
//			If you want unique sortkeys then override Add function
template <class T, typename ItemType, class Copy, class EnumType,
			typename KeyType, typename KeyClass, typename EnumKeyType, typename EnumKeyClass>
class ICollectionOnSTLMapExImpl : 
	public T
{
public:
	typedef		std::multimap<EnumKeyClass, ItemType>	EnumCollType;
	typedef		std::pair<EnumKeyClass, ItemType>		EnumPairType;
	typedef		typename EnumCollType::iterator			EnumIterType;

	typedef		std::map<KeyClass, EnumIterType>		CollType;
	typedef		std::pair<KeyClass, EnumIterType>		PairType;
	typedef		typename CollType::iterator				IterType;

 	STDMETHOD(get_Count)(LONG* pCount)
	{
		if(pCount == NULL)
			return E_POINTER;
		*pCount = (LONG)m_coll.size();
		return S_OK;
	}

	STDMETHOD(get_Item)(KeyType Key, ItemType* pValue)
	{
		if(!pValue)
			return E_POINTER;
		typename KeyClass	_key	= Key;
		IterType	it		= m_collRef.find(_key);

		if(it != m_collRef.end())
		{
			Copy::init(pValue);
			return Copy::copy(pValue, &(it->second->second));
		}
		else
		{
			*pValue = NULL;
			return S_OK;
		}
	}

	STDMETHOD(Add)(KeyType Key, EnumKeyType EnumKey, ItemType Value)
	{
		KeyClass	_key	= Key;
		IterType	it		= m_collRef.find(_key);

		if(it != m_collRef.end())
			return E_FAIL;

		EnumKeyClass	_enumKey	= EnumKey;
		ItemType	_item;
		HRESULT		hr;

		Copy::init(&_item);
		if(FAILED(hr = Copy::copy(&_item, &Value)))
			return hr;

		m_collRef[_key] = m_coll.insert(EnumPairType(_enumKey, _item));
		return S_OK;
	}

	STDMETHOD(Remove)(KeyType Key)
	{
		KeyClass	_key	= Key;
		IterType	it		= m_collRef.find(_key);
		
		if(it == m_collRef.end())
			return E_FAIL;

		if(it->second == m_coll.end())
			return E_FAIL;

		Copy::destroy(&(it->second->second));
		m_coll.erase(it->second);
		m_collRef.erase(_key);
		return S_OK;
	}
	STDMETHOD(Clear)()
	{
		EnumIterType it = m_coll.begin();
		for (;it != m_coll.end(); it++) 
			Copy::destroy(&(it->second));
		m_coll.clear();
		m_collRef.clear();
		return S_OK;
	}
	STDMETHOD(get__NewEnum)(IUnknown** ppUnk)
	{
		if (ppUnk == NULL)
			return E_POINTER;
		*ppUnk = NULL;
		HRESULT hr = S_OK;
		CComObject<EnumType>* p;
		hr = CComObject<EnumType>::CreateInstance(&p);
		if (SUCCEEDED(hr))
		{
			hr = p->Init(this, m_coll);
			if (hr == S_OK)
				hr = p->QueryInterface(IID_IUnknown, (void**)ppUnk);
		}
		if (hr != S_OK)
			delete p;
		return hr;
	}

	STDMETHOD(get_BySortKey)(EnumKeyType Key, ItemType* pValue)
	{		   
		HRESULT hr = S_OK;
		if(!pValue)
			return E_POINTER;

		EnumKeyClass _key = Key;
		EnumIterType it	= m_coll.find(_key);
		if(it != m_coll.end())
		{
			Copy::init(pValue);
			return Copy::copy(pValue, &it->second);
		}
		else
		{
			*pValue = NULL;
			return S_OK;
		}

		return hr;
	}

	EnumCollType	m_coll;
	CollType		m_collRef;
};

template< class T > class AutoDeleteArray
{
public:
	T* p;

	AutoDeleteArray () : p (NULL)
	{
	}

	~AutoDeleteArray ()
	{
		if (p)
			delete [] p;
	}
};

/*
this is a first version of collection with additional unique sortkey

template <class T, typename ItemType, class Copy, class EnumType,
			typename KeyType, class KeyClass, typename EnumKeyType, class EnumKeyClass>
class ICollectionOnSTLMapExImpl : 
	public T
{
public:
	typedef		std::map<EnumKeyClass, ItemType>		EnumCollType;
	typedef		std::pair<EnumKeyClass, ItemType>		EnumPairType;
	typedef		EnumCollType::iterator					EnumIterType;

	typedef struct _ValueReference
	{
		EnumKeyType		_EnumKey;
		ItemType		_EnumItem;
	} ValueReference;

	typedef		std::map<KeyClass, ValueReference>		CollType;
	typedef		std::pair<KeyClass, ValueReference>		PairType;
	typedef		CollType::iterator						IterType;

 	STDMETHOD(get_Count)(LONG* pCount)
	{
		if(pCount == NULL)
			return E_POINTER;
		*pCount = (LONG)m_coll.size();
		return S_OK;
	}

	STDMETHOD(get_Item)(KeyType Key, ItemType* pValue)
	{
		if(!pValue)
			return E_POINTER;
		KeyClass	_key	= Key;
		IterType	it		= m_collRef.find(_key);

		if(it != m_collRef.end())
		{
			Copy::init(pValue);
			return Copy::copy(pValue, &(it->second._EnumItem));
		}
		else
		{
			*pValue = NULL;
			return S_OK;
		}
	}

	STDMETHOD(Add)(KeyType Key, EnumKeyType EnumKey, ItemType Value)
	{
		KeyClass	_key	= Key;
		IterType	it		= m_collRef.find(_key);

		if(it != m_collRef.end())
			return E_FAIL;

		EnumKeyClass	_enumKey	= EnumKey;
		EnumIterType	enumIt		= m_coll.find(_enumKey);

		if(enumIt != m_coll.end())
			return E_FAIL;

		ItemType	_item;
		HRESULT		hr;

		Copy::init(&_item);
		if(FAILED(hr = Copy::copy(&_item, &Value)))
			return hr;

		m_coll[_enumKey] = _item;

		ValueReference ref;
		ref._EnumKey = EnumKey;
		ref._EnumItem = Value;

		m_collRef[_key] = ref;

		return S_OK;
	}

	STDMETHOD(Remove)(KeyType Key)
	{
		KeyClass	_key	= Key;
		IterType	it		= m_collRef.find(_key);
		
		if(it == m_collRef.end())
			return E_FAIL;

		EnumKeyClass	_enumKey	= it->second._EnumKey;
		EnumIterType	enumIt		= m_coll.find(_enumKey);

		if(enumIt != m_coll.end())
			return E_FAIL;

		Copy::destroy(&(enumIt->second));
		m_coll.erase(_enumKey);
		m_collRef.erase(_key);
		return S_OK;
	}
	STDMETHOD(Clear)()
	{
		EnumIterType it = m_coll.begin();
		for (;it != m_coll.end(); it++) 
			Copy::destroy(&(it->second));
		m_coll.clear();
		m_collRef.clear();
		return S_OK;
	}
	STDMETHOD(get__NewEnum)(IUnknown** ppUnk)
	{
		if (ppUnk == NULL)
			return E_POINTER;
		*ppUnk = NULL;
		HRESULT hr = S_OK;
		CComObject<EnumType>* p;
		hr = CComObject<EnumType>::CreateInstance(&p);
		if (SUCCEEDED(hr))
		{
			hr = p->Init(this, m_coll);
			if (hr == S_OK)
				hr = p->QueryInterface(IID_IUnknown, (void**)ppUnk);
		}
		if (hr != S_OK)
			delete p;
		return hr;
	}
	EnumCollType	m_coll;
	CollType		m_collRef;
};
*/

/******************************
	This marco help to use more
	than one interface for 
	ISupportErrorInfo
*******************************/
#define BEGIN_ERROR_INFO_MAP									\
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)			\
	{															\
		static const IID* arr[] =								\
		{														

#define ERROR_INFO_INTERFACE_ENTRY(x)							\
			&_ATL_IIDOF(x),
			
#define END_ERROR_INFO_MAP										\
			NULL												\
		};														\
		for (int i=0; i < sizeof(arr) / sizeof(arr[0]) -1; i++)	\
		{														\
		if (ATL::InlineIsEqualGUID(*arr[i],riid))			\
				return S_OK;									\
		}														\
		return S_FALSE;											\
	}															

#endif	__ATL_EXTENTION_H_
