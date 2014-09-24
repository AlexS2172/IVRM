#pragma once

class Trader
{
public:
	Trader(void);
	~Trader(void);
	
	Trader(std::string name, long id = 0);

	long& id();
	std::string& name();
	std::string& description();
	
private:
	long _id;
	std::string _name;
	std::string _description;
};
