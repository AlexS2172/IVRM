#ifndef _IVMCTRLCP_H_
#define _IVMCTRLCP_H_


template <class T>
class CProxy_IIVControlEvents : public IConnectionPointImpl<T, &DIID__IIVControlEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_CommandEvent(IVMCommandTypeEnum type, VARIANT data)
	{
		_variant_t varResult;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		_variant_t* pvars = new _variant_t[2];
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[1] = (long)type;
				pvars[0] = data;
				
				DISPPARAMS disp = { pvars, NULL, 2, 0 };
				pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;
	
	}
};
#endif