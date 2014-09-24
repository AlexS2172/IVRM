#ifndef __HH_COMMON_PROPS_H__
#define __HH_COMMON_PROPS_H__

#include "MsgPropImpl.h"

#define IMPLEMENT_SEQUENCE_NUMBER_PROP				\
	IMPLEMENT_LONG_PROP(SequenceNumber, m_nSequenceNumber)

#define IMPLEMENT_TOTAL_RECORDS_PROP				\
	IMPLEMENT_LONG_PROP(TotalRecords, m_nTotalRecords)

#define IMPLEMENT_UPDATE_DATE_PROP					\
  	IMPLEMENT_DATE_PROP(UpdateDate, m_dtUpdateDate)

#define IMPLEMENT_STATUS_PROP						\
	IMPLEMENT_SIMPLE_PROP(RECORD_STATUS, Status, m_enStatus)

#define IMPLEMENT_ERROR_CODE_PROP					\
	IMPLEMENT_LONG_PROP(ErrorCode, m_nErrorCode)

#define IMPLEMENT_RECORD_TYPE_PROP					\
	IMPLEMENT_SIMPLE_PROP(RECORD_TYPE, RecordType, m_enRecordType)

#define IMPLEMENT_ERROR_DESCRIPTION_PROP			\
	IMPLEMENT_VARCHAR_PROP(ErrorDescription, m_strErrorDescription)

#define IMPLEMENT_HH_COMMON_PROP					\
	IMPLEMENT_SEQUENCE_NUMBER_PROP					\
	IMPLEMENT_TOTAL_RECORDS_PROP					\
	IMPLEMENT_UPDATE_DATE_PROP						\
	IMPLEMENT_STATUS_PROP							\
	IMPLEMENT_ERROR_CODE_PROP						\
	IMPLEMENT_RECORD_TYPE_PROP						\
	IMPLEMENT_ERROR_DESCRIPTION_PROP				\


#endif //__HH_COMMON_PROPS_H__