// IvBlob.cpp : Implementation of CIvBlob

#include "stdafx.h"
#include "IvBlob.h"


// CIvBlob

STDMETHODIMP CIvBlob::AddIvOption(INT vOption , DOUBLE vVolatility)
{
	std::string bs ( (LPCTSTR)m_sOptionVola.GetStr() ) ;
	char sBuff[50] ;
	memset ( sBuff , 0 , sizeof ( sBuff ) ) ;
	sprintf ( sBuff , "%i" , vOption ) ;
	bs += std::string ( sBuff ) ;
	bs += ":" ;
	memset ( sBuff , 0 , sizeof ( sBuff ) ) ;
	sprintf ( sBuff , "%f" , vVolatility ) ;
	bs += std::string ( sBuff ) ;
	bs+= "/" ;
	m_sOptionVola = bs ;
	return S_OK ;
}

STDMETHODIMP CIvBlob::AddIvStock(INT vStock, DOUBLE vPrice)
{
	
	std::string bs ( (LPCTSTR)m_sSymbolPrice.GetStr() ) ;
	char sBuff[50] ;
	memset ( sBuff , 0 , sizeof ( sBuff ) ) ;
	sprintf ( sBuff , "%i" , vStock ) ;
	bs += std::string ( sBuff ) ;
	bs += ":" ;
	memset ( sBuff , 0 , sizeof ( sBuff ) ) ;
	sprintf ( sBuff , "%f" , vPrice ) ;
	bs += std::string ( sBuff ) ;
	bs+= "/" ;
	m_sSymbolPrice  = bs ;
	return S_OK ;
}

/*STDMETHODIMP CIvBlob::GetData(BSTR*vDataStock , BSTR*vDataOption )
{
	
	if ( !vDataStock || !vDataOption )
		return E_POINTER ;
	::SysFreeString(*vDataStock );
	::SysFreeString(*vDataOption);

	*vDataStock		= ::SysAllocStringLen(A2W(m_sSymbolPrice.GetStr()), ::lstrlenA(m_sSymbolPrice.GetStr()));
	*vDataOption	= ::SysAllocStringLen(A2W(m_sOptionVola.GetStr()), ::lstrlenA(m_sOptionVola.GetStr()));;

	return S_OK ;
}
*/
STDMETHODIMP CIvBlob::Clear()
{
	m_sSymbolPrice.Clear() ;
	m_sOptionVola.Clear() ;
	return S_OK ;
}

STDMETHODIMP CIvBlob::Empty(VARIANT_BOOL*_Empty) 
{
	if ( !_Empty )
		return E_POINTER ;
	*_Empty = VARIANT_FALSE ;
	if ( m_sSymbolPrice.IsEmpty() && m_sOptionVola.IsEmpty() )
		*_Empty = VARIANT_TRUE ;
	return S_OK ;
}