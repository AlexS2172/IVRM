// Message.cpp 
#include "stdafx.h"
#include "Message.h"

MsgTypeEnum GetMessageType(const void* pBuffer)
{
	MsgTypeEnum	tpEnum;
	
	::CopyMemory(&tpEnum,(char*)pBuffer + sizeof(u_long), sizeof(MsgTypeEnum));

	if(tpEnum > enCheckSubject || tpEnum < enLogon)
		tpEnum = enUnknown;

	return tpEnum;
};
