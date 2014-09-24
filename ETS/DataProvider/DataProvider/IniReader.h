#pragma once

class IniReader {
public:

	class Section {
	public:

		Section(const std::string& id);

		typedef std::pair<std::string, std::string> key_value;
		typedef std::vector<key_value> key_container;

		bool 
		get_value(const std::string& key, std::string& value, const std::string& default_value);

		bool
		get_value(const std::string& key, long& value, long default_value);

		virtual 
		void add(const std::string& key, const std::string& value);

		void remove(const std::string& key);
		
		key_container parameters_;
		std::string id_;
	};

	typedef std::vector<std::string> key_container;

	IniReader(const std::string& file_name);

	long 
	read_section(Section& sc);

	long
	read_key(const std::string& section_id, const std::string& key, std::string& value);

	long 
	read_keys_list(const std::string& section_id, key_container& keys);

	long 
	read_sections_list(key_container& keys);

private:
	long 
	split(std::vector<char>& buffer, long size, key_container& keys);

	key_container sections;
	std::string	file_name_;
};

class CorbaSettings: public IniReader::Section {
public:

	CorbaSettings(const std::string& id);

	/*virtual*/
	void add(const std::string& key, const std::string& value);

	void get_orb_args(char**& args, int& count);
	
private:
	void split_parameter(const std::string& key, const std::string& delim);
};
