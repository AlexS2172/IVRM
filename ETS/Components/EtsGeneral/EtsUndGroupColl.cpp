// EtsUndGroupColl.cpp : Implementation of CEtsUndGroupColl

#include "stdafx.h"
#include "EtsUndGroupColl.h"
#include <fstream>

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsUndGroupColl::Add(LONG Key, BSTR SortKey, IEtsUndGroupAtom* Value, IEtsUndGroupAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Underlying group with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CEtsUndGroupAtom>* pNewVal;

			_CHK(CComObject<CEtsUndGroupAtom>::CreateInstance(&pNewVal), _T("Fail to add underlying group."));
			pNewVal->AddRef();
			if(FAILED(IEtsUndGroupCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add underlying group."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IEtsUndGroupCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add underlying group."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsUndGroupColl, e.Error());
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------------------------//
STDMETHODIMP CEtsUndGroupColl::LoadProperty(BSTR FileName)
{
	try
	{
		std::vector<CString> Values;
		std::ifstream GroupRatioFile;
		GroupRatioFile.open( FileName );

		char* pToken = NULL;
		char* pNextToken = NULL;
		char line[512];
		memset(line, 0, static_cast<size_t>(sizeof(char) * 512));
		char seps[] = ";";

		/* clear group ratio in case of reload*/
		CEtsUndGroupColl::CollType::iterator itr = m_collRef.begin();
		CEtsUndGroupColl::CollType::iterator itrEnd = m_collRef.end();
		for(; itr != itrEnd; ++itr){
			CEtsUndGroupAtom* ptrGroup = dynamic_cast<CComObject<CEtsUndGroupAtom>*>(itr->second->second);
			if ( ptrGroup ){
				ptrGroup->clear();
			};
		}

		/*load data from file*/
		while (!GroupRatioFile.eof())
		{
			GroupRatioFile.getline(line, 256, '\n');

			pToken = strtok_s( line, seps, &pNextToken ); 
		
			//separate line values
			while( pToken != NULL )
			{
				CString strValue(pToken);

				strValue.TrimLeft().TrimRight();

				Values.push_back(strValue);

				pToken = strtok_s( NULL, seps, &pNextToken );
			};

			//parse values
			if (Values.size() > 1)
			{
				if (!Values[0].IsEmpty())
				{
					CEtsUndGroupColl::CollType::iterator itr = m_collRef.begin();
					CEtsUndGroupColl::CollType::iterator itrEnd = m_collRef.end();
					for(; itr != itrEnd; ++itr)
					{
						CEtsUndGroupAtom* ptrGroup = dynamic_cast<CComObject<CEtsUndGroupAtom>*>(itr->second->second);
						if ( ptrGroup )
						{
							if (ptrGroup->m_bstrName == CComBSTR(Values[0]))
							{
								
								long lBegin = static_cast<long>(enGrSpot) + 1;
								long lEnd = static_cast<GroupRatioEnum>(enGrEnumSize);
								for (long i = lBegin; i <= lEnd; i++)
								{							
									if (Values[i].GetLength() > 0) // VolaRatio
									{
										double dRatio = atof(Values[i]);
										ptrGroup->put_Ratio(static_cast<GroupRatioEnum>(i-1), dRatio);
									}
								}
								break;
							}
						}
					};
				}
			};
			//clear for new line parse
			Values.clear();
		}
	}
	catch (_com_error& err)
	{
		return err.Error();
	}
	catch (...)
	{
		return E_UNEXPECTED;
	}
	return S_OK;
};
//-------------------------------------------------------------------------------------------------------------------//
