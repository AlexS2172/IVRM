#pragma once

class CCorbaErrorHelper
{
public:
	CCorbaErrorHelper(void);
	~CCorbaErrorHelper(void);

public:
	static std::string ToString(const CORBA::SystemException& err);
	static std::string ToString(const CORBA::Exception& err);
	static std::string ToString(const omniORB::fatalException& err);
};
