
#include "stdafx.h"
#include "IniReader.h"
#include <string.h>

/*INI file read helper*/
IniReader::IniReader(const std::string& file_name): file_name_(file_name){
};
//----------------------------------------------------------//
long 
IniReader::read_section(Section& sc){

	key_container section_keys;

	std::string& section_id = sc.id_;
	if (read_keys_list(section_id, section_keys) > 0){		

		key_container::iterator it = section_keys.begin();
		key_container::iterator it_end = section_keys.end();

		std::string value;
		for ( ; it != it_end; it++){
			std::string& key = (*it);			
			long size = read_key(section_id, key, value);
			if (size > 0){
				sc.add(key, value);
			}
		}
	}
	return -1;		
};
//----------------------------------------------------------//
long
IniReader::read_key(const std::string& section_id, const std::string& key, std::string& value){
	std::vector<char> buffer;
	buffer.resize(2048);

	long size = GetPrivateProfileString(section_id.c_str(), 
										key.c_str(), NULL, &buffer[0], 2048, 
										file_name_.c_str());	
	value = std::string(&buffer[0]);

	return size;
};
//----------------------------------------------------------//
long 
IniReader::read_keys_list(const std::string& section_id, key_container& keys){
	std::vector<char> buffer;
	buffer.resize(2048);
	long size = GetPrivateProfileString(section_id.c_str(), 
										NULL, NULL, &buffer[0], 2048, 
										file_name_.c_str());
	return split(buffer, size, keys);
};
//----------------------------------------------------------//
long 
IniReader::read_sections_list(key_container& keys){
	std::vector<char> buffer;
	buffer.resize(2048);	

	long size = GetPrivateProfileString(NULL, NULL, NULL, 
										&buffer[0], 2048, 
										file_name_.c_str());
	return split(buffer, size, keys);
};
//----------------------------------------------------------//
long 
IniReader::split(std::vector<char>& buffer, long size, key_container& keys){

	std::string key;
	key.clear();

	for(long i = 0; i < size; i++) {	
		if (buffer[i]  != '\0'){
			key.append(1, buffer[i]);
		}
		else {
			if (!key.empty()) {
				keys.push_back(key);
				key.clear();
			}
		}
	}

	return static_cast<long>(keys.size());
};
//---------------------------------------------------------//
IniReader::Section::Section(const std::string& id):id_(id){
	parameters_.reserve(32);
};
//---------------------------------------------------------//
bool
IniReader::Section::get_value(const std::string &key, 
							  long &value,
							  long default_value){

	std::string string_value;
	if (get_value(key, string_value, "0")){
		value = atoi(string_value.c_str());
		return true;
	};
	value = default_value;
	return false;
};
//---------------------------------------------------------//
bool
IniReader::Section::get_value(const std::string& key, 
							  std::string& value,
							  const std::string& default_value) {

	key_container::iterator it = parameters_.begin();
	key_container::iterator it_end = parameters_.end();

	for ( ; it != it_end; it++) {
		key_value& item = (*it);
		if (item.first == key) {
			value = item.second;
			return true;
		};
	};
	value = default_value;
	return false;
};
//---------------------------------------------------------//
void
IniReader::Section::add(const std::string& key, 
						const std::string& value) {
	parameters_.push_back(key_value(key, value));
};
//---------------------------------------------------------//
void
IniReader::Section::remove(const std::string& key) {
	
	key_container::iterator it = parameters_.begin();
	key_container::iterator it_end = parameters_.end();
	
	for ( ; it != it_end; it++) {
		IniReader::Section::key_value& pmtr = *it;
		if (key == pmtr.first) {
			parameters_.erase(it);
			break;
		}
	};
};
//---------------------------------------------------------//
void
CorbaSettings::add(const std::string& key,
				   const std::string& value) {
	parameters_.push_back(key_value("-" + key, value));
};
//---------------------------------------------------------//
void
CorbaSettings::get_orb_args(char**& args, int& count){

	int		argc		= __argc;
	TCHAR**	argv		= __targv;
	
	split_parameter("-ORBInitRef", ";");
	
	count = argc + static_cast<int>(2 * parameters_.size());
	args = new char*[count];
	memcpy(args, argv, sizeof(char*)*argc);

	int paramIndex = argc;

	IniReader::Section::key_container::iterator it = parameters_.begin();
	IniReader::Section::key_container::iterator it_end = parameters_.end();

	for( ;it != it_end; it++) {
		IniReader::Section::key_value& pmtr = *it;
		args[paramIndex++] = const_cast<char*>(pmtr.first.c_str());
		args[paramIndex++] = const_cast<char*>(pmtr.second.c_str());
	};
};
//---------------------------------------------------------//
void
CorbaSettings::split_parameter(	const std::string& key, 
								const std::string& delim) {
	
	IniReader::Section::key_container::iterator it = parameters_.begin();
	IniReader::Section::key_container::iterator it_end = parameters_.end();
	
	std::string parameter_value = "";
	for ( ;it != it_end; it++) {
		IniReader::Section::key_value& pmtr = *it;
		
		if (pmtr.first == key) {
			parameter_value = pmtr.second;
			Section::remove(key);
			break;
		};
	};

	if (!parameter_value.empty()) {
		char* next_token = 0;
		char* first_token = strtok_s(const_cast<char*>(parameter_value.c_str()), delim.c_str(), &next_token);
		
		while (first_token) {
			std::string new_value = first_token;
			Section::add(key, new_value);	
			first_token = strtok_s(0, delim.c_str(), &next_token);
		};
	};
};
//---------------------------------------------------------//
CorbaSettings::CorbaSettings(const std::string &id)
:IniReader::Section(id) {
};
//---------------------------------------------------------//