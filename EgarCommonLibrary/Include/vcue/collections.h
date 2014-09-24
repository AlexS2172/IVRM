#ifndef __COLLECTIONS_H_
#define __COLLECTIONS_H_

#include "AtlExt.h"

// NOTE: please don't forget to call Clear method in FinalRelease of your collection

template <class T, class Interface>
class ICollectionOnSTLVectorOfInterfacePtrImpl : 
	public ICollectionOnSTLVectorImpl<T, Interface*, _CopyInterface<Interface>, CComEnumVariantOnSTLVector<Interface*> >
{
};

template <class T>
class ICollectionOnSTLVectorOfVariantImpl : 
	public ICollectionOnSTLVectorImpl<T, VARIANT, _Copy<VARIANT>, CComEnumVariantOnSTLVector<VARIANT> >
{
};

template <class T, typename KeyType, class Interface, class KeyClass = KeyType >
class ICollectionOnSTLMapOfInterfacePtrImpl : 
	public ICollectionOnSTLMapImpl<T, KeyType, Interface*, _CopyInterface<Interface>, 
		CComEnumVariantOnSTLMap<KeyClass, Interface*>, KeyClass >
{
};

template <class T, typename KeyType, class KeyClass = KeyType >
class ICollectionOnSTLMapOfVariantImpl : 
	public ICollectionOnSTLMapImpl<T, KeyType, VARIANT, _Copy<VARIANT>, 
		CComEnumVariantOnSTLMap<KeyClass, VARIANT>, KeyClass >
{
};

template <class T, class Interface, typename KeyType, class KeyClass, typename EnumKeyType, class EnumKeyClass>
class ICollectionOnSTLMapExOfInterfacePtrImpl : 
	public ICollectionOnSTLMapExImpl<T, Interface*, _CopyInterface<Interface>, CComEnumVariantOnSTLMultiMap<EnumKeyClass, Interface*>,
									KeyType, KeyClass, EnumKeyType, EnumKeyClass >
{
};

#endif __COLLECTIONS_H_
