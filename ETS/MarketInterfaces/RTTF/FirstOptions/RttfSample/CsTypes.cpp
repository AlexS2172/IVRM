#include "stdafx.h"
#ifndef _WIN32
#include <strings.h>
#endif
#include "CsTypes.h"

FOC_DATETIME::FOC_DATETIME()
{
	time_t now;
#ifndef _WIN32
	struct tm dttm;
#endif
	struct tm* pDttm;

	time(&now);

#ifdef MULTI_THREADED
	pDttm = localtime_r(&now, &dttm);
#else
	pDttm = localtime(&now);
#endif

	this->month = pDttm->tm_mon;
	this->day = pDttm->tm_mday;
	this->year = pDttm->tm_year + 1900;
	this->hour = 0;
	this->min = 0;
	this->sec = 0;		
}

FOC_DATETIME::FOC_DATETIME(time_t t)
{
	struct tm* pDttm;

#ifdef MULTI_THREADED // Solaris Flag
	struct tm dttm;
	pDttm = localtime_r(&t, &dttm);
#else
	pDttm = localtime(&t);
#endif

	this->month = pDttm->tm_mon;
	this->day = pDttm->tm_mday;
	this->year = pDttm->tm_year + 1900;
	this->hour = pDttm->tm_hour; 
	this->min = pDttm->tm_min;
	this->sec = pDttm->tm_sec;		
}

FOC_DATETIME::FOC_DATETIME(const char* s)
{
	char time_buff[256];
	memset(time_buff, 0, 256);
	strcpy(time_buff, s);

	tm tim;

	strftime(s, 256, "%d/%m/2%Y.%H:%M:%S", &tim);

	month = tim.tm_mon;
	day = tim.tm_mday;
	year = tim.tm_year;
	hour = tim.tm_hour;
	min = tim.tm_min;
	sec = tim.tm_sec;
}

FOC_DATETIME::operator time_t() const
{
	struct tm dttm;

	memset(&dttm, 0, sizeof(dttm));
	dttm.tm_mon = this->month;
	dttm.tm_mday = this->day;
	dttm.tm_year = this->year - 1900;
	dttm.tm_hour = this->hour;
	dttm.tm_min = this->min;
	dttm.tm_sec = this->sec;
	
	return (mktime(&dttm));
}

bool FOC_DATETIME::operator<(const FOC_DATETIME& rhs) const
{
	return ((time_t) *this < (time_t) rhs);
}

bool FOC_DATETIME::operator<=(const FOC_DATETIME& rhs) const
{
	return ((time_t) *this <= (time_t) rhs);
}

bool FOC_DATETIME::operator>(const FOC_DATETIME& rhs) const
{
	return ((time_t) *this > (time_t) rhs);
}

bool FOC_DATETIME::operator>=(const FOC_DATETIME& rhs) const
{
	return ((time_t) *this > (time_t) rhs);
}

bool FOC_DATETIME::operator==(const FOC_DATETIME& rhs) const
{
	return ((time_t) *this == (time_t) rhs);
}

bool_t xdr_foc_datetime(XDR* xdrs, FOC_DATETIME* dttm)
{
	if (!xdr_u_char(xdrs, &dttm->sec))
	{
		fprintf(stderr, "Bad XDR conversion on 'FOC_DATETIME(sec)'\n");
		return FALSE;
	}
	
	if (!xdr_u_char(xdrs, &dttm->min))
	{
		fprintf(stderr, "Bad XDR conversion on 'FOC_DATETIME(min)'\n");
		return FALSE;
	}
	
	if (!xdr_u_char(xdrs, &dttm->hour))
	{
		fprintf(stderr, "Bad XDR conversion on 'FOC_DATETIME(hour)'\n");
		return FALSE;
	}
	
	if (!xdr_u_char(xdrs, &dttm->day))
	{
		fprintf(stderr, "Bad XDR conversion on 'FOC_DATETIME(day)'\n");
		return FALSE;
	}
	
	if (!xdr_u_char(xdrs, &dttm->month))
	{
		fprintf(stderr, "Bad XDR conversion on 'FOC_DATETIME(month)'\n");
		return FALSE;
	}
	
	if (!xdr_u_short(xdrs, &dttm->year))
	{
		fprintf(stderr, "Bad XDR conversion on 'FOC_DATETIME(year)'\n");
		return FALSE;
	}

	return TRUE;
}

FocCsHeader::FocCsHeader()
{
	memset(orig_id, 0, sizeof(orig_id));
	memset(recip_id, 0, sizeof(recip_id));
	msg_priority = 0;
	msg_class = 0;
	num_items = 0;
	msg_type = 0;
	service_id = 0;
	msg_time = 0;
	msg_seq_num = 0;
}

void FocCsHeader::init
(
	const char* p_orig_id,
	u_char p_msg_type,
	u_long p_msg_seq_num,
	u_char p_msg_class,
	u_short p_service_id,
	const char* p_recip_id,
	u_char p_num_items,
	u_char p_msg_priority
)
{
	time_t now;
	struct tm* pDttm;

	// Get the current time
	time(&now);

#ifdef MULTI_THREADED // Solaris Flag
	struct tm dttm;
	pDttm = localtime_r(&now, &dttm);
#else
	pDttm = localtime(&now);	
#endif

	// Fill the header
	strcpy((char *) this->orig_id, p_orig_id);
	strcpy((char *) this->recip_id, p_recip_id);
	this->msg_priority = p_msg_priority;
	this->msg_class = p_msg_class;
	this->num_items = p_num_items;
	this->msg_type = p_msg_type;
	this->service_id = p_service_id;
	this->msg_time = (pDttm->tm_hour * 3600) + (pDttm->tm_min * 60) + pDttm->tm_sec;
	this->msg_seq_num = p_msg_seq_num;
}
		
bool FocCsHeader::xdr_convert(XDR* xdrs, xdr_op x_op)
{
	char* str;

	xdrs->x_op = x_op;

	str = (char *) orig_id;
	if (!xdr_string(xdrs, &str, sizeof(orig_id)))
	{
		fprintf(stderr, "Bad XDR conversion on 'orig_id'\n");
		return FALSE;
	}

	str = (char *) recip_id;
	if (!xdr_string(xdrs, &str, sizeof(recip_id)))
	{
		fprintf(stderr, "Bad XDR conversion on 'recip_id'\n");
		return FALSE;
	}

	if (!xdr_u_char(xdrs, &msg_priority))
	{
		fprintf(stderr, "Bad XDR conversion on 'msg_priority'\n");
		return FALSE;
	}
	
	if (!xdr_u_char(xdrs, &msg_class))
	{
		fprintf(stderr, "Bad XDR conversion on 'msg_class'\n");
		return FALSE;
	}
	
	if (!xdr_u_char(xdrs, &num_items))
	{
		fprintf(stderr, "Bad XDR conversion on 'num_items'\n");
		return FALSE;
	}
	
	if (!xdr_u_char(xdrs, &msg_type))
	{
		fprintf(stderr, "Bad XDR conversion on 'msg_type'\n");
		return FALSE;
	}
	
	if (!xdr_u_short(xdrs, &service_id))
	{
		fprintf(stderr, "Bad XDR conversion on 'service_id'\n");
		return FALSE;
	}
	
	if (!xdr_u_short(xdrs, &msg_time))
	{
		fprintf(stderr, "Bad XDR conversion on 'msg_time'\n");
		return FALSE;
	}
	
	if (!xdr_u_long(xdrs, &msg_seq_num))
	{
		fprintf(stderr, "Bad XDR conversion on 'msg_seq_num'\n");
		return FALSE;
	}

	return TRUE;
}
