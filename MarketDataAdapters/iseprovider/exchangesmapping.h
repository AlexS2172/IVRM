#ifndef EXCHANGESMAPPING_H
#define EXCHANGESMAPPING_H

class CExchangesMapping
{
	map<string, string>	m_mapExchanges;

public:

	CExchangesMapping()
	{
		m_mapExchanges["AMEX"] = "A";
		m_mapExchanges["ISE"]  = "K";
		m_mapExchanges["NYSE"] = "N";
		m_mapExchanges["PHLX"] = "X";
		m_mapExchanges["PCX"]  = "P";
		m_mapExchanges["NASD"] = "S";
		m_mapExchanges["CBOE"] = "H";
		m_mapExchanges["CSX"]  = "M";
		m_mapExchanges["NSSC"] = "I";
		m_mapExchanges["BOST"] = "B";
	}

	const string& GetExchangeCode(const string& sExchange)
	{
		map<string, string>::iterator it = m_mapExchanges.find(sExchange);
		if(it == m_mapExchanges.end())
			return sExchange;
		else
			return it->second;	
	}
};

#endif