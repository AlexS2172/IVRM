#ifndef __LASTTRADE_H__
#define __LASTTRADE_H__

#include "CsTypes.h"

class LastTrade
{
public:
	LastTrade();
	virtual ~LastTrade();

	// Must call after creating object
	bool init(const char* pFileName);

	const FOC_DATETIME& getDate(void);
	bool setDate(const FOC_DATETIME& dttm);
	bool isRestart(void) { return m_restart; }

protected:
	void saveGuts();
	void restoreGuts();

private:
	const char* m_pFileName;
	bool m_restart;
	FOC_DATETIME m_last;
};

#endif
