#pragma once

class Strategy
{
public:
	~Strategy(void);
	
	Strategy(std::string name = "NA", long id = 0);

	long& id();
	std::string& name();
	std::string& description();

private:
	long _id;
	std::string _name;
	std::string _description;
};
