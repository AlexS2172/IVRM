#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

namespace OPM
{

//=============================================================================
// Error codes 
//=============================================================================
const long __BaseErrorCode				= 0x0000;
const long __InvalidParametr			= __BaseErrorCode + 1;
const long __OutOfAllowSteps			= __BaseErrorCode + 2;
const long __InvalidDividendData		= __BaseErrorCode + 3;
const long __TwoDividendsAtSameTime		= __BaseErrorCode + 4;
const long __TooBigDividends			= __BaseErrorCode + 5;

//=============================================================================
// Function codes
//=============================================================================
const long __BaseFuncCode				= 0x0000;
const long __Laplace					= __BaseFuncCode + 1;
const long __BlackAndScholes			= __BaseFuncCode + 2;
const long __CoxRoss					= __BaseFuncCode + 3;
const long __CoxRossDiscDividends		= __BaseFuncCode + 4;


class CException
{
public:
	CException(long lFuncCode, long lErrorCode)
	{
		m_lFuncCode		= lFuncCode;
		m_lErrorCode	= lErrorCode;
	}

	CException(const CException& rE) : 
										m_lFuncCode(rE.m_lFuncCode), 
										m_lErrorCode(rE.m_lErrorCode)
	{
	}

	long GetFunction() 
	{ 
		return m_lFuncCode; 
	}
	
	long GetError() 
	{ 
		return m_lErrorCode; 
	}

private:
	long	m_lFuncCode;
	long	m_lErrorCode;
};

};

#endif	__EXCEPTION_H__