#pragma	once
#ifndef __EODERRORS_H__
#define __EODERRORS_H__

#include <CString>
#include <stdexcept>
using namespace std;

namespace EODCO
{

class CEodStopException : public exception
{
public:
	CEodStopException() : exception(_T("Immediately Stop All Operations")){}
};

};

#endif// __EODERRORS_H__
