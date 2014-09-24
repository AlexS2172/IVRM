/*
**** QSDSKAPI.H - Include file for ATFI QS file access API.
****
*/

#ifndef __QSDSKAPI_H__
#define __QSDSKAPI_H__

#define	BTR_OPEN					0
#define	BTR_CLOSE					1
#define	BTR_INSERT					2
#define	BTR_UPDATE					3
#define	BTR_DELETE					4
#define	BTR_GET_EQUAL				5
#define	BTR_GET_NEXT				6
#define	BTR_GET_PREV				7
#define	BTR_GET_GREATER				8
#define	BTR_GET_GREATER_EQ			9
#define	BTR_GET_LESSTHAN			10
#define	BTR_GET_LESSTHAN_EQ			11
#define	BTR_GET_FIRST				12
#define	BTR_GET_LAST				13
#define	BTR_CREATE					14
#define	BTR_STAT					15
#define	BTR_GET_POSITION			22
#define	BTR_GET_DIRECT				23

#ifdef	__cplusplus
extern	"C" {
#endif

//	QS Disk API functions - flat files

DWORD WINAPI	QSOpen(HAT *hAt,UINT qsn,LPSTR szFilename);
DWORD WINAPI	QSRead(HAT *hAt,UINT qsn,UINT fh,DWORD dwOffset,
						 	  LPSTR lpBuffer,DWORD wLength);
DWORD WINAPI	QSSeek(HAT *hAt,UINT qsn,UINT fh,DWORD dwOffset,DWORD origin);
DWORD WINAPI	QSClose(HAT *hAt,UINT qsn,UINT fh);
DWORD WINAPI	QSDelete(HAT *hAt,UINT qsn,LPSTR szFilename);
DWORD WINAPI	QSWrite(HAT *hAt,UINT QSN,UINT fh,LPSTR lpBuffer,DWORD wLength);
DWORD WINAPI	QSOpenBT(HAT *hAt,UINT qsn,LPSTR szFilename);
DWORD WINAPI	QSCloseBT(HAT *hAt,UINT qsn,DWORD tree);
DWORD WINAPI	QSNextKeyBT(HAT *hAt,UINT qsn,DWORD tree);
DWORD WINAPI	QSKeyValueBT(HAT *hAt,UINT qsn,DWORD tree,LPSTR szKeyvalue);
DWORD WINAPI	QSLocateBT(HAT *hAt,UINT qsn,DWORD tree,LPSTR szKeyValue);
DWORD WINAPI	QSWildCompareBT(HAT *hAt,UINT qsn,DWORD tree);
DWORD WINAPI	QSPrevKeyBT(HAT *hAt,UINT QSN,DWORD tree);
DWORD WINAPI	QSFirstKeyBT(HAT *hAt,UINT qsn,DWORD tree);
DWORD WINAPI	QSLastKeyBT(HAT *hAt,UINT qsn,DWORD tree);
DWORD WINAPI	QSKeySizeBT(HAT *hAt,UINT qsn,DWORD tree);
DWORD WINAPI	QSCurrentKeyBT(HAT *hAt,UINT qsn,DWORD tree);
BOOL  WINAPI	QSFileExist(HAT *hAt,UINT qsn,LPSTR lpszFN);

/*====== QS Btrieve File access functions,structures and values =======*/

UINT WINAPI		QSBtvOpen(HAT *hAt,UINT qsn,DWORD dwFileID,LPDWORD lpdwStatus);
UINT WINAPI		QSBtvClose(HAT *hAt,UINT qsn,UINT hBtvFile);
UINT WINAPI		QSBtvInsert(HAT *hAt,UINT qsn,UINT hBtvFile,LPSTR lpData,DWORD dwDataLen,DWORD dwKeyNum);
UINT WINAPI		QSBtvUpdate(HAT *hAt,UINT qsn,UINT hBtvFile,LPSTR lpData,DWORD dwDataLen,DWORD dwKeyNum);
UINT WINAPI		QSBtvDelete(HAT *hAt,UINT qsn,UINT hBtvFile);
UINT WINAPI		QSBtvGetEqual(HAT *hAt,UINT qsn,UINT hBtvFile,LPSTR  lpKey,DWORD wKeyNum,LPDWORD lpdwDataLen,LPSTR lpData);
UINT WINAPI		QSBtvGetNext(HAT *hAt,UINT qsn,UINT hBtvFile,LPSTR lpKey,DWORD dwKeyNum,LPDWORD lpdwDataLen,LPSTR lpData);
UINT WINAPI		QSBtvGetPrev(HAT *hAt,UINT qsn,UINT hBtvFile,LPSTR lpKey,DWORD dwKeyNum,LPDWORD lpdwDataLen,LPSTR lpData);
UINT WINAPI		QSBtvGetGreater(HAT *hAt,UINT qsn,UINT hBtvFile,LPSTR lpKey,DWORD dwKeyNum,LPDWORD lpdwDataLen,LPSTR lpData);
UINT WINAPI		QSBtvGetGreaterOrEqual(HAT *hAt,UINT qsn,UINT hBtvFile,LPSTR lpKey,DWORD dwKeyNum,LPDWORD lpdwDataLen,LPSTR lpData);
UINT WINAPI		QSBtvGetLessThan(HAT *hAt,UINT qsn,UINT hBtvFile,LPSTR lpKey,DWORD dwKeyNum,LPDWORD lpdwDataLen,LPSTR lpData);
UINT WINAPI		QSBtvGetLessThanOrEqual(HAT *hAt,UINT qsn,UINT hBtvFile,LPSTR lpKey,DWORD dwKeyNum,LPDWORD lpdwDataLen,LPSTR lpData);
UINT WINAPI		QSBtvGetFirst(HAT *hAt,UINT qsn,UINT hBtvFile,LPSTR lpKey,DWORD dwKeyNum,LPDWORD lpdwDataLen,LPSTR lpData);
UINT WINAPI		QSBtvGetLast(HAT *hAt,UINT qsn,UINT hBtvFile,LPSTR lpKey,DWORD dwKeyNum,LPDWORD lpdwDataLen,LPSTR lpData);
UINT WINAPI		QSBtvCreate(HAT *hAt,UINT qsn,UINT hFileID,LPSTR lpFileSpec,DWORD dwFileSpecLen,BOOL bWarnExist);
UINT WINAPI		QSBtvStat(HAT *hAt,UINT qsn,UINT hBtvFile,LPSTR lpStat,LPDWORD lpdwStatLen,LPSTR lpFilename);
UINT WINAPI		QSBtvGetPosition(HAT *hAt,UINT qsn,UINT hBtvFile,LPDWORD lpdwPosition);
UINT WINAPI		QSBtvGetDirect(HAT *hAt,UINT qsn,UINT hBtvFile,DWORD dwPosition,DWORD dwOffset,DWORD dwKeyNum,LPDWORD lpdwDataLen,LPSTR lpKey,LPSTR lpData);

/*====== QS DBM File access functions,structures and values =======*/

UINT WINAPI		QSDBMInfo(HAT *hAt,UINT qsn,UINT hDBMFile,LPSTR lpInfo,LPDWORD lpdwInfoLen);
UINT WINAPI		QSDBMOpen(HAT *hAt,UINT qsn,DWORD dwBFN,LPDWORD lpdwStatus);
UINT WINAPI		QSDBMClose(HAT *hAt,UINT qsn,UINT hDBMFile);
UINT WINAPI		QSDBMAdd(HAT *hAt,UINT qsn,UINT hDBMFile,DWORD dwKeyNum,
							LPSTR lpKey,DWORD dwKeyLen,LPSTR lpData,DWORD dwDataLen);
UINT WINAPI		QSDBMUpdate(HAT *hAt,UINT qsn,UINT hDBMFile,DWORD dwKeyNum,
							LPSTR lpKey,DWORD dwKeyLen,LPSTR lpData,DWORD dwDataLen);
UINT WINAPI		QSDBMDelete(HAT *hAt,UINT qsn,UINT hDBMFile,DWORD dwKeyNum,
							LPSTR lpKey,DWORD dwKeyLen);
UINT WINAPI		QSDBMGetEqual(HAT *hAt,UINT qsn,UINT hDBMFile,DWORD dwKeyNum,
							LPSTR lpKey,LPDWORD dwKeyLen,LPSTR lpData,LPDWORD dwDataLen);
UINT WINAPI		QSDBMGetNext(HAT *hAt,UINT qsn,UINT hDBMFile,DWORD dwKeyNum,
							LPSTR lpKey,LPDWORD dwKeyLen,LPSTR lpData,LPDWORD dwDataLen);
UINT WINAPI		QSDBMGetPrev(HAT *hAt,UINT qsn,UINT hDBMFile,DWORD dwKeyNum,
							LPSTR lpKey,LPDWORD dwKeyLen,LPSTR lpData,LPDWORD dwDataLen);
UINT WINAPI		QSDBMGetGreater(HAT *hAt,UINT qsn,UINT hDBMFile,DWORD dwKeyNum,
							LPSTR lpKey,LPDWORD dwKeyLen,LPSTR lpData,LPDWORD dwDataLen);
UINT WINAPI		QSDBMGetGreaterOrEqual(HAT *hAt,UINT qsn,UINT hDBMFile,DWORD dwKeyNum,
							LPSTR lpKey,LPDWORD dwKeyLen,LPSTR lpData,LPDWORD dwDataLen);
UINT WINAPI		QSDBMGetLessThan(HAT *hAt,UINT qsn,UINT hDBMFile,DWORD dwKeyNum,
							LPSTR lpKey,LPDWORD dwKeyLen,LPSTR lpData,LPDWORD dwDataLen);
UINT WINAPI		QSDBMGetLessThanOrEqual(HAT *hAt,UINT qsn,UINT hDBMFile,DWORD dwKeyNum,
							LPSTR lpKey,LPDWORD dwKeyLen,LPSTR lpData,LPDWORD dwDataLen);
UINT WINAPI		QSDBMGetFirst(HAT *hAt,UINT qsn,UINT hDBMFile,DWORD dwKeyNum,
							LPSTR lpKey,LPDWORD dwKeyLen,LPSTR lpData,LPDWORD dwDataLen);
UINT WINAPI		QSDBMGetLast(HAT *hAt,UINT qsn,UINT hDBMFile,DWORD dwKeyNum,
							LPSTR lpKey,LPDWORD dwKeyLen,LPSTR lpData,LPDWORD dwDataLen);

#ifdef __cplusplus
}
#endif

#endif
