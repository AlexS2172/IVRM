// stdafx.cpp : source file that includes just the standard includes
// IvRmServer.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
//------------------------------------------------------------//
long 
get_configuration_file_name(std::string& file_name){

	file_name.clear();
	std::vector<char> buffer;
	buffer.resize(1024);

	long buffer_size = GetModuleFileName(NULL, &buffer[0], 1024);

	if (buffer_size > 0){

		file_name.resize(1024);

		TCHAR szDrive[8] = {0};
		TCHAR szDir[512] = {0};
		TCHAR szName[128] = {0};

		_tsplitpath(&buffer[0], szDrive, szDir, szName, NULL);

		file_name = std::string(szDrive) + 
			std::string(szDir) + 
			std::string(szName) + 
			std::string(".ini");
	};

	return buffer_size;
};
//------------------------------------------------------------//
std::string 
ToString(const CORBA::SystemException& e){
	CORBA::Any tmp;
	tmp <<= e;
	CORBA::TypeCode_var tc = tmp.type();
	return std::string(tc->name()) + " Description: " + e.NP_minorString();
};
//------------------------------------------------------------//
std::string 
ToString(const CORBA::Exception & e){
	CORBA::Any tmp;
	tmp <<= e;
	CORBA::TypeCode_var tc = tmp.type();
	return std::string(tc->name());
};
//------------------------------------------------------------//
std::string 
ToString(const omniORB::fatalException & e){
	return std::string(e.errmsg());
};
//------------------------------------------------------------//
// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
