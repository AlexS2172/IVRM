//		atdmapi.h
//		TCP IPC HATDM

#ifndef _ATDMAPI_H
#define	_ATDMAPI_H

#ifndef __DMTCPMsg_DEFINES
#include "dmtcpmsg.h"
#endif

#ifndef RC_INVOKED
#pragma pack(1)
#endif /* RC_INVOKED */

#ifdef	__cplusplus
extern	"C" {
#endif

typedef struct _HATDM
		{
		SOCKET			  socket;
		int				  iStatus;
		HANDLE			  hRecvReady;
		HANDLE			  hReadThread;
		HANDLE			  hUpdateThread;
		ATDM_DLVR_MSG	  rspMessage;
#ifdef __cplusplus
		RingBuffer		  *updateRingBuffer;
#else
		void              *updateRingBuffer;
#endif
		volatile BOOL	  bRingBufferValid;
		DWORD			  (WINAPI *callbackProc)(void *buffer,DWORD length,DWORD param);
		DWORD			  dwParam;
		DWORD			  dwMsgID;
		CRITICAL_SECTION  lock;
		}
		HATDM;

//	AT Client API functions - dwMsgID should always be no-zero.

DWORD WINAPI  AtDmAttach(HATDM **hAtDm,ATMESSAGEPROC,DWORD param);
DWORD WINAPI  AtDmDetach(HATDM **hAtDm);

DWORD WINAPI  AtDmAsyncRequest(HATDM *hAtDm,VOID *msgIn,DWORD lenIn);

DWORD WINAPI  AtDmRequest(HATDM *hAtDm,VOID *msgIn, DWORD lenIn,
							 		   VOID *msgOut,DWORD *lenOut);

DWORD WINAPI  AtDmUnrequestData(HATDM *hAt,DWORD dwMsgID,int iDMDataID);
DWORD WINAPI  AtDmUnrequestAll(HATDM *hAt);

DWORD WINAPI  AtDmUsingNewMem(HATDM *hAt,DWORD dwMsgID,int iDMDataID);

#ifdef __cplusplus
}
#endif

#ifndef RC_INVOKED
#pragma pack()
#endif /* RC_INVOKED */

#endif
