#include "stdafx.h"
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#else	
#include <unistd.h>
#endif

#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <time.h>
#include "LastTrade.h"


LastTrade::LastTrade() : 
	m_restart(TRUE),
	m_pFileName(NULL)
{
}

LastTrade::~LastTrade()
{
}

bool LastTrade::init(const char* pFileName)
{
	struct stat filestatus;

	// Save the file name
	m_pFileName = pFileName;

	// See if file exists
	if (stat(pFileName, &filestatus))
	{
		// Since the file doesn't exist, this is not a restart.
		m_restart = FALSE;
	}

	if (m_restart)
	{
		// On a restart, restore the last trade date
		restoreGuts();
	
		// Make sure the date is current

		time_t now;
		struct tm* pDttm;

		time(&now);

#ifdef MULTI_THREADED // Solaris Flag
		struct tm dttm;
		pDttm = localtime_r(&now, &dttm);
#else
		pDttm = localtime(&now);
#endif

		if (m_last.year < pDttm->tm_year + 1900
			|| (m_last.year == pDttm->tm_year + 1900
				&& m_last.month < pDttm->tm_mon)
			|| (m_last.year == pDttm->tm_year + 1900
				&& m_last.month == pDttm->tm_mon
				&& m_last.day < pDttm->tm_mday))
		{
			// This is not a restart if the date is old
			// (i.e.  the file wasn't deleted before starting)
			m_restart = FALSE;

			fprintf(stdout, "Discarding old date from file\n");

			// If the date is old, re-initialize the last date
			memset(&m_last, 0, sizeof(m_last));
			m_last.day = pDttm->tm_mday;
			m_last.month = pDttm->tm_mon;
			m_last.year = pDttm->tm_year + 1900;
		}
	}
	else
	{
		// If not restarting, persist the default date
		saveGuts();
	}

	fprintf(stdout, "Last trade date: %02d-%02d-%04d %02d:%02d:%02d\n",
		m_last.month + 1, m_last.day, m_last.year,
		m_last.hour, m_last.min, m_last.sec);

	return TRUE;
}

const FOC_DATETIME& LastTrade::getDate(void)
{
	return m_last;
}

bool LastTrade::setDate(const FOC_DATETIME& dttm)
{
	FOC_DATETIME tradetime;

	// This code is for QA environment only!  This will force
	// the trade dates to have the current date.  This is because
	// QA in normally loaded with old data for testing...  we want
	// the time, but not the date that gets passed.
	// So, remove this for production!

	tradetime.hour = dttm.hour;
	tradetime.min = dttm.min;
	tradetime.sec = dttm.sec;

	if (tradetime > m_last)
	{
		// Copy the value
		m_last = tradetime;
	
		// Persist the value
		saveGuts();

		// Once the time is updated, this is a restart
		m_restart = TRUE;
	}

	return TRUE;	
}

void LastTrade::saveGuts(void)
{
	int fd;

#ifdef _WIN32
	fd = _open(m_pFileName, O_WRONLY | O_CREAT, _S_IREAD | _S_IWRITE);
#else
	fd = open(m_pFileName, O_WRONLY | O_CREAT, 0666);
#endif

	ofstream* ofs = new ofstream(fd);
	ofs->seekp(0);
	
	int month, day, year, hour, min, sec;

	month = m_last.month + 1;	
	day = m_last.day;
	year = m_last.year;
	hour = m_last.hour;
	min = m_last.min;
	sec = m_last.sec;
	
	*ofs << month << ' ';
	*ofs << day << ' ';
	*ofs << year << ' ';
	*ofs << hour << ' ';
	*ofs << min << ' ';
	*ofs << sec << ' ';
	*ofs << flush;

	ofs->close();
	delete(ofs);
	close(fd);
}

void LastTrade::restoreGuts(void)
{
	int fd;

#ifdef _WIN32
	fd = _open(m_pFileName, O_RDONLY);
#else
	fd = open(m_pFileName, O_RDONLY);
#endif
	
	ifstream* ifs = new ifstream(fd);
	ifs->seekg(0);

	int month, day, year, hour, min, sec;

	*ifs >> month;
	*ifs >> day;
	*ifs >> year;
	*ifs >> hour;
	*ifs >> min;
	*ifs >> sec;

	m_last.month = month - 1;	
	m_last.day = day;
	m_last.year = year;
	m_last.hour = hour;
	m_last.min = min;
	m_last.sec = sec;

	ifs->close();
	delete(ifs);
	close(fd);
}
