#ifndef __INETPACKINGIMPL_H__
#define __INETPACKINGIMPL_H__

#include "MsgStruct.h"
#include "MsgPacking.h"

_COM_SMARTPTR_TYPEDEF(INetPacking, __uuidof(INetPacking));

template<class T, typename _Data>
class INetPackingConstSizeDataImpl : public _Data,
	public IDispatchImpl<INetPacking, &IID_INetPacking, &LIBID_MSGSTRUCTLib>
{
public:
	STDMETHOD(Pack)(BSTR *pRetVal)
	{
		if (pRetVal == NULL)
			return E_POINTER;

		if (*pRetVal)
		{
			::SysFreeString(*pRetVal);	// We do it to prevent leaks of memory
			*pRetVal = NULL;
		}

		T::ObjectLock lock((T*)this);

		CPacket	Packet;

		if (!this->Packing(Packet))
			return E_FAIL;

		UINT nSize;
		void* pData = Packet.GetBuffer(nSize);

		ATLASSERT(pData != NULL);

		ULONG nFixSize = nSize + nSize % 2;

//		ULONG nSize = sizeof(_Data);
//		ULONG nFixSize = nSize + nSize % 2;
		
		void* pTmp = alloca(nFixSize);

        ::memset(pTmp, 0, nFixSize);
		::memcpy(pTmp, pData, nSize);

		*pRetVal = ::SysAllocStringLen((BSTR)pTmp, nFixSize >> 1);

		return S_OK;
	}

	STDMETHOD(Unpack)(BSTR Val)
	{
		T::ObjectLock lock((T*)this);

		ULONG nFixSize = ::SysStringByteLen(Val);

		CPacket	Packet(Val, nFixSize);

		if (!this->Packing(Packet, true))
			return E_FAIL;

		/*ULONG nSize = sizeof(_Data);
		ULONG nFixSize `= ::SysStringByteLen(Val);

		if (nFixSize < nSize)
			return E_FAIL;

		::memcpy((_Data*)this, Val, nSize);*/

		return S_OK;
	}
};

#endif //__INETPACKINGIMPL_H__