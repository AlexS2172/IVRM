// VMESurface.cpp : Implementation of CVMESurface
#include "stdafx.h"
#include "MsgStruct.h"
#include "VMESurface.h"

/////////////////////////////////////////////////////////////////////////////
// IVMESurface implementation
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CVMESurface::put_Symbol(BSTR NewVal)								
{																	
	ObjectLock lock(this);											

	_bstr_t tmpString(NewVal);
	std::string asciiStr = tmpString;
	memset( m_HeaderData.__Symbol, 0, sizeof(m_HeaderData.__Symbol));
	memcpy(m_HeaderData.__Symbol, asciiStr.c_str(), asciiStr.length() );

	return S_OK;													
}																	

STDMETHODIMP CVMESurface::get_Symbol(BSTR* pRetVal)								
{																	
	if (!pRetVal)													
		return E_POINTER;											

	if (*pRetVal)													
		::SysFreeString(*pRetVal);									

	ObjectLock lock(this);											
	_bstr_t* bstr = new _bstr_t( std::string(m_HeaderData.__Symbol).c_str() );

	*pRetVal = bstr->GetBSTR(); //::SysAllocStringByteLen(m_HeaderData.__Symbol, strlen(m_HeaderData.__Symbol));
	/*USES_CONVERSION;												

	*pRetVal = ::SysAllocStringLen(A2W(m_HeaderData.__sSymbol),			
		::lstrlenA(m_HeaderData.__sSymbol));		*/

	return S_OK;													
}

STDMETHODIMP CVMESurface::get_SurfaceID(long *pVal)
{
	if( !pVal ) return E_POINTER;

	ObjectLock lock(this);

	*pVal = m_HeaderData.__nSurfaceID;

	return S_OK;
}

STDMETHODIMP CVMESurface::put_SurfaceID(long newVal)
{
	ObjectLock lock(this);

	m_HeaderData.__nSurfaceID = newVal;

	return S_OK;
}

STDMETHODIMP CVMESurface::get_OptType(long *pVal)
{
	if( !pVal ) return E_POINTER;

	ObjectLock lock(this);

	*pVal = m_HeaderData.__nOptType;

	return S_OK;
}

STDMETHODIMP CVMESurface::put_OptType(long newVal)
{
	ObjectLock lock(this);

	m_HeaderData.__nOptType = newVal;

	return S_OK;
}

STDMETHODIMP CVMESurface::get_UnderlinePrice(double *pVal)
{
	if( !pVal ) return E_POINTER;

	ObjectLock lock(this);

	*pVal = m_HeaderData.__fUnderlinePrice;

	return S_OK;
}

STDMETHODIMP CVMESurface::put_UnderlinePrice(double newVal)
{
	ObjectLock lock(this);

	m_HeaderData.__fUnderlinePrice = newVal;

	return S_OK;
}

STDMETHODIMP CVMESurface::get_SmileAccelerator(double *pVal)
{
	if( !pVal ) return E_POINTER;

	ObjectLock lock(this);

	*pVal = m_HeaderData.__fSmileAccelerator;

	return S_OK;
}

STDMETHODIMP CVMESurface::put_SmileAccelerator(double newVal)
{
	ObjectLock lock(this);

	m_HeaderData.__fSmileAccelerator = newVal;

	return S_OK;
}

STDMETHODIMP CVMESurface::get_InterpolationFactor(double *pVal)
{
	if( !pVal ) return E_POINTER;

	ObjectLock lock(this);

	*pVal = m_HeaderData.__fInterpolationFactor;

	return S_OK;
}

STDMETHODIMP CVMESurface::put_InterpolationFactor(double newVal)
{
	ObjectLock lock(this);

	m_HeaderData.__fInterpolationFactor = newVal;

	return S_OK;
}

STDMETHODIMP CVMESurface::get_DiscreteAcceleration(VARIANT_BOOL *pVal)
{
	if( !pVal ) return E_POINTER;

	ObjectLock lock(this);

	*pVal = m_HeaderData.__bDiscreteAcceleration;

	return S_OK;
}

STDMETHODIMP CVMESurface::put_DiscreteAcceleration(VARIANT_BOOL newVal)
{
	ObjectLock lock(this);

	m_HeaderData.__bDiscreteAcceleration = newVal;

	return S_OK;
}

STDMETHODIMP CVMESurface::get_PriceOverride(VARIANT_BOOL *pVal)
{
	if( !pVal ) return E_POINTER;

	ObjectLock lock(this);

	*pVal = m_HeaderData.__bPriceOverride;

	return S_OK;
}

STDMETHODIMP CVMESurface::put_PriceOverride(VARIANT_BOOL newVal)
{
	ObjectLock lock(this);

	m_HeaderData.__bPriceOverride = newVal;

	return S_OK;
}

STDMETHODIMP CVMESurface::get_Points(IUnknown **pVal)
{
	if (!pVal) return E_POINTER;

	ObjectLock lock(this);

	if (m_spPoints == NULL)
	{
		*pVal = NULL;
		return S_OK;
	}

	try
	{
		long nCount = m_spPoints->RecordCount;
		
		if (nCount > 0)
		{
			_RecordsetPtr spPoints = m_spPoints->Clone(adLockReadOnly);
			*pVal = spPoints.Detach();
		}
		else
			*pVal = NULL;
	}
	catch(const _com_error& e)
	{
		return e.Error();
	}

	return S_OK;
}

STDMETHODIMP CVMESurface::put_Points(IUnknown *newVal)
{
	ObjectLock lock(this);

	m_spPoints = newVal;

	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// INetPacking implementation
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CVMESurface::Pack(BSTR *pRetVal)
{
	if (pRetVal == NULL)
		return E_POINTER;
		
	ObjectLock lock(this);

	if (*pRetVal)
	{
		::SysFreeString(*pRetVal);	// We do it to prevent leaks of memory
		*pRetVal = NULL;
	}

	long nPointsCount = 0;

	if (m_spPoints != NULL)
		m_spPoints->get_RecordCount(&nPointsCount);

	ULONG nDataSize	= sizeof(VolaHeaderData) + nPointsCount * sizeof(VolaPointData);
	ULONG nFixSize	= nDataSize + nDataSize % 2;

	void* pData = alloca(nFixSize);
	
	ATLASSERT(pData);
	
	if (pData == NULL)	// More tests is the best
		return E_FAIL;

	VolaHeaderData*	pHeaderData = (VolaHeaderData*)pData;

	memcpy(pHeaderData, &m_HeaderData, sizeof(VolaHeaderData));

	pHeaderData->__nPointsCount = nPointsCount;
	
	try
	{
		if (m_spPoints != NULL && nPointsCount)
		{
			VolaPointData* pPointData = (VolaPointData*)(pHeaderData + 1);

			m_spPoints->MoveFirst();
			
			while (!m_spPoints->Eof)
			{
				pPointData->__nPointID = m_spPoints->Fields->Item[L"iCustomStrikeSkewPointID"]->Value.lVal;
				pPointData->__fStrike = m_spPoints->Fields->Item[L"fStrike"]->Value;
				pPointData->__fVola = m_spPoints->Fields->Item[L"fVolatility"]->Value;
				pPointData->__ExpDate = m_spPoints->Fields->Item[L"dtExpDate"]->Value.date;
				pPointData->__IsBasePoint = m_spPoints->Fields->Item[L"iIsBasePoint"]->Value.lVal != 0;
				pPointData->__bStatus = m_spPoints->Fields->Item[L"iStatus"]->Value.bVal;

				m_spPoints->MoveNext();

				pPointData++;
			}
		}
	}
	catch(const _com_error& e)
	{
		return e.Error();
	}

	*pRetVal = ::SysAllocStringLen((BSTR)pData, nFixSize >> 1);

	return S_OK;
}


STDMETHODIMP CVMESurface::Unpack(BSTR Val)
{
	ObjectLock lock(this);

	ULONG nHeaderSize = sizeof(VolaHeaderData);

	ULONG nFixSize = SysStringLen(Val) * 2;

	if (nFixSize < nHeaderSize)
		return E_FAIL;

	VolaHeaderData* pHeaderData = (VolaHeaderData*)Val;

	ULONG nDataSize = nHeaderSize + pHeaderData->__nPointsCount * sizeof(VolaPointData);

	if (nFixSize < nDataSize)
		return E_FAIL;

	memcpy(&m_HeaderData, pHeaderData, nHeaderSize);

	m_spPoints = NULL;

	if (m_HeaderData.__nPointsCount == 0)
		return S_OK;
	
	try
	{
		VolaPointData* pPointData = (VolaPointData*)(pHeaderData + 1);

		m_spPoints.CreateInstance(CLSID_Recordset);

		// Specify RS fields and properties
		m_spPoints->CursorLocation = adUseClient;
		m_spPoints->LockType	   = adLockOptimistic;
		m_spPoints->Fields->Append( L"iCustomStrikeSkewPointID", adInteger, 4, adFldUnspecified );
		m_spPoints->Fields->Append( L"fStrike",			         adDouble,  4, adFldUnspecified );
		m_spPoints->Fields->Append( L"fVolatility",			     adDouble,  4, adFldUnspecified );
		m_spPoints->Fields->Append( L"dtExpDate",				 adDate,    4, adFldUnspecified );
		m_spPoints->Fields->Append( L"iIsBasePoint",			 adInteger, 4, adFldUnspecified );
		m_spPoints->Fields->Append( L"iStatus",				     adInteger, 4, adFldUnspecified );

		// Open disconnected RS
		m_spPoints->Open(L"data source=none", vtMissing, adOpenUnspecified, adLockUnspecified, adCmdUnspecified);

		for (ULONG i = 0; i < m_HeaderData.__nPointsCount; i++)
		{
			m_spPoints->AddNew();
			m_spPoints->Fields->GetItem(L"iCustomStrikeSkewPointID")->PutValue(pPointData->__nPointID);
			m_spPoints->Fields->GetItem(L"fStrike")->PutValue(pPointData->__fStrike);
			m_spPoints->Fields->GetItem(L"fVolatility")->PutValue(pPointData->__fVola);
			m_spPoints->Fields->GetItem(L"dtExpDate")->PutValue(pPointData->__ExpDate);
			m_spPoints->Fields->GetItem(L"iIsBasePoint")->PutValue(pPointData->__IsBasePoint);
			m_spPoints->Fields->GetItem(L"iStatus")->PutValue((long)pPointData->__bStatus);
			m_spPoints->Update();

			pPointData++;
		}
	}
	catch(const _com_error& e)
	{
		return e.Error();
	}

	return S_OK;
}

