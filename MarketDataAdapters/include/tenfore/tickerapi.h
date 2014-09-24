/*  TickerApi.H
 *
 *  Tenfore Ticker --> 3rd parties
 *
 *  Conversion of Tenfore Ticker messages (from the feed server)
 *  into a general format for 3rd parties
 *
 *  (C) 1997-03 EuroAmericanGroup Inc. all rights reserved
 *  Author: M.H.Gruener
 *
 *  Version history:
 *  1.00  14.10.99  mg  port from the .OBJ version of the API
 *  1.01  19.04.00  mg  improved rpc initialization
 *  1.02  20.04.00	gh  new functions for ClientApi
 *  1.03  11.05.00	mg  new function to switch back to realtime feed TApiRealtimeFeedServer()
 *  1.04  29.06.00	mg  bug fix when disabling news with TApiSetNewsFlag()
 *  1.05  12.08.00  mg  function interface to access packets and fields
 *  1.06  28.09.00  mg  new data type Julian Date (and conversion function TApiConvJulianDate())
 *  1.07  08.12.00  mg  new (internal) function to load raw feed file TApiLoadFeedFile()
 *                      copy data referenced only by pointers (strings, binary, double) also
 *                      into the user supplied buffer (for other programming languages)
 *  1.08  01.02.01  mg  increase internal field id table
 *  1.09  12.04.01  mg  add TApiSendCommand() interface
 *  1.10  05.03.02  mg  eag2tf.cpp: allow field ids > 255 in EnableFieldInTic()
 *  1.11  22.05.02  mg  new fields and exchange ids in tapidef.h
 *  1.12  04.11.02  mg  better rpc server (FeedServer/QuoteSpeed client) detection in TApiInitialize()/CApiInitialize()
 *  1.13  15.04.03  mg  increase news markets to 200
 *  1.14  20.06.03  mg  bug fix when sending Admin/Error messages (error text was not copied
 *                      into the the user supplied buffer
 *  1.15  03.07.03  mg  new functions TApiPtrToOff(), TApiOffToPtr(), TApiDataToString()
 */
#ifndef __TICKERAPI_H
#define __TICKERAPI_H

#ifndef __WINDOWS_
#include <windows.h>
#endif


#ifndef __TAPIDEF_H
#include "tapidef.h"
#endif

//
// function pointers to dynamically link the API
//
//
// TickerApi function pointer declarations
//
typedef int      (WINAPI* pTApiInitialize)(int);
typedef void     (WINAPI* pTApiShutDown)(void);
typedef int      (WINAPI* pTApiReadRecord)(BYTE*);
typedef int      (WINAPI* pTApiConvertRecord)(BYTE*, BYTE*, int);
typedef int      (WINAPI* pTApiReadAndDropRecord)(void);
typedef char*    (WINAPI* pTApiGetEag2TfVersion)(void);
typedef char*    (WINAPI* pTApiGetFeedServerVersion)(void);
typedef char*    (WINAPI* pTApiGetRpcVersion)(void);
typedef char*    (WINAPI* pTApiGetVersion)(void);
typedef void     (WINAPI* pTApiSaveDataToFile)(int, char*);
typedef int      (WINAPI* pTApiEmulateFeedServer)(char*);
typedef int      (WINAPI* pTApiRealtimeFeedServer)(void);
typedef void     (WINAPI* pTApiSetReadDelay)(long);
typedef double   (WINAPI* pTApiFracToDouble)(TfValue*);
typedef long     (WINAPI* pTApiFracToDenom)(TfValue*);
typedef long     (WINAPI* pTApiFracToNumerator)(TfValue*);
typedef void     (WINAPI* pTApiSetPrintf)(int(*)(char*, ...));
typedef void     (WINAPI* pTApiSetMarketFlag)(int, int);
typedef void     (WINAPI* pTApiSetNewsFlag)(int, int);
typedef void     (WINAPI* pTApiSetMessageFlag)(int, int);
typedef void     (WINAPI* pTApiSetSecurityFlag)(int, int);
typedef void     (WINAPI* pTApiSetFieldFlag)(int, int);
typedef long     (WINAPI* pTApiFieldToLong)(TfField*);
typedef float    (WINAPI* pTApiFieldToFloat)(TfField*);
typedef double   (WINAPI* pTApiFieldToDouble)(TfField*);
typedef int      (WINAPI* pTApiFieldToString)(TfField*, char *);
typedef int      (WINAPI* pTApiFieldToBinary)(TfField*, char *, int);
typedef int      (WINAPI* pTApiConvJulianDate)(DWORD , int *, int *, int *);
typedef TfField *(WINAPI* pTApiGetNextField)(TfPacket*,TfField*);
typedef int      (WINAPI* pTApiGetMessageType)(TfPacket *);
typedef int      (WINAPI* pTApiGetSecurityType)(TfPacket *);
typedef int      (WINAPI* pTApiGetExchangeId)(TfPacket *);
typedef int      (WINAPI* pTApiGetMarketCenter)(TfPacket *);
typedef int      (WINAPI* pTApiGetFieldCount)(TfPacket *);
typedef int      (WINAPI* pTApiGetSymbol)(TfPacket *, char *);
typedef int      (WINAPI* pTApiGetFieldId)(TfField *);
typedef int      (WINAPI* pTApiGetValueType)(TfField *);
typedef int      (WINAPI* pTApiGetValueBuffer)(TfField *, int *, int *, void *);
typedef void     (WINAPI* pTApiLoadFeedFile)(char *);
typedef int      (WINAPI* pTApiSendCommand)(char *);
typedef int      (WINAPI* pTApiPtrToOff) (char *);
typedef int      (WINAPI* pTApiOffToPtr) (char *);
typedef int      (WINAPI* pTApiDataToString) (char *, char *, int);

//
// ClientApi function pointer declarations
//
typedef int (WINAPI* pCApiInitialize)(int);
typedef void (WINAPI* pCApiShutDown)(void);
typedef void (WINAPI* pCApiSymbolRequest)(TfCApiSymbolRequest *pRequest);
typedef void (WINAPI* pCApiDbaseRequest)(TfCApiDbaseRequest *pRequest);

/*  TApiInitialize
 *
 *  Initialize and enable the communication with the feed server
 *
 *  nMaxBufSize - the max size (in bytes) of the conversion buffer
 *                supplied by the main application.
 *                A default of 1024 bytes is assumed if the parameter
 *                is set to 0.
 *
 *  return value - 1 on success
 *                 0 on any failure
 */
__declspec(dllexport) int WINAPI TApiInitialize (int nMaxBufSize);

/*  TApiShutDown
 *
 *  Disable the communication with the feed server.
 *  It does NOT shut down the program on the feed server, any subsequent
 *  call to TApiInitialize() will enable the communication again.
 */
__declspec(dllexport) void WINAPI TApiShutDown (void);

/*  TApiReadRecord
 *
 *  Read a record from the feed server into the application supplied
 *  buffer. The function is non-blocking, i.e. it returns if there
 *  is no data available.
 *
 *  That means the function must be called periodically to empty the
 *  feed server buffers. The average amount of data during the market
 *  (without OPRA) is 2500 .. 3000 bytes per seconds.
 *
 *  pBuffer - pointer to the conversion buffer supplying enough space
 *            for the converted data. The size of this buffer must
 *            be either the nMaxBufSize bytes specified in the
 *            TApiInitialize (nMaxBufSize) function or the default
 *            1024 bytes.
 *
 *  return value -   0  if no data available
 *                 > 0  size of the received record
 */
__declspec(dllexport) int WINAPI TApiReadRecord (BYTE *pBuffer);

/*  TApiConvertRecord
 *
 *  Convert a record received somehow from the feed server into the dynamic
 *  TfPacket structure.
 *
 *  pDest   - pointer to the conversion buffer supplying enough space
 *            for the converted data. The size of this buffer must
 *            be either the nMaxBufSize bytes specified in the
 *            TicInitialize (nMaxBufSize) function or the default
 *            1024 bytes.
 *  pSource - pointer to the raw feed server data
 *  nSize   - size in bytes of the raw data
 *
 *  return value -   0  if no data available
 *                 > 0  size of the received record
 */
__declspec(dllexport) int WINAPI TApiConvertRecord (BYTE *pDest, BYTE *pSource, int nSize);

/*  TApiReadAndDropRecord
 *
 *  Read a record from the feed server and discard the data if available
 *  The function is non-blocking, i.e. it returns if there
 *  is no data available.
 *
 *  return value -   0  if no data available
 *                 > 0  size of the received record
 */
__declspec(dllexport) int WINAPI TApiReadAndDropRecord (void);

/*  TApiGetVersion
 *
 *  Returns a pointer to the version string of the TApi library.
 */
__declspec(dllexport) char *WINAPI TApiGetVersion (void);

/*  TApiGetEag2TfVersion
 *
 *  Returns a pointer to the version string of the library.
 */
__declspec(dllexport) char *WINAPI TApiGetEag2TfVersion (void);

/*  TApiGetFeedServerVersion
 *
 *  Returns a pointer to the version string of the program.
 */
__declspec(dllexport) char *WINAPI TApiGetFeedServerVersion (void);

/*  TApiGetRpcVersion
 *
 *  Returns a pointer to the version string of the PC2T (RPC communication) library.
 */
__declspec(dllexport) char *WINAPI TApiGetRpcVersion (void);

/*  TApiSaveDataToFile
 *
 *  Use this function to disable or enable saving raw feed server data
 *  into a binary file.
 *
 *  This function can be called in any moment before or during
 *  communication with the feed server .
 *
 *  saveData - 0 disable saving
 *             1 enable saving
 *
 *  name     - file name, pass NULL to use the default name FEEDOUT.DAT
 */
__declspec(dllexport) void WINAPI TApiSaveDataToFile (int saveData, char *name);

/*  TApiEmulateFeedServer
 *
 *  Call this function BEFORE calling TApiInitialize() to
 *  emulate the feed server with data from a file.
 *
 *  name - file name of the data file,
 *         pass NULL to use the default name FEEDIN.DAT
 *
 *  return value - 0  if file does not exist
 *                 1  on success
 */
__declspec(dllexport) int WINAPI TApiEmulateFeedServer (char *name);

/*  TApiRealtimeFeedServer
 *
 *  Call this function to change the feed source back to
 *  the real time feed server.
 *
 *  return value - 0  if file does not exist
 *                 1  on success
 */
__declspec(dllexport) int WINAPI TApiRealtimeFeedServer (void);

/*  TApiSetReadDelay
 *
 *  set the delay in msecs between reading raw feed data blocks
 *  in emulation mode
 */
__declspec(dllexport) void WINAPI TApiSetReadDelay (long delay);

/*  TApiFracToDouble
 *
 *  Converts the EAGs fractional format into a double
 */
__declspec(dllexport) double WINAPI TApiFracToDouble (TfValue *v);

/*  TApiFracToDenom
 *
 *  Returns the denominator of a value in EAGs fractional format
 */
__declspec(dllexport) long WINAPI TApiFracToDenom (TfValue *v);

/*  TApiFracToNumerator
 *
 *  Returns the numerator of a value in EAGs fractional format
 */
__declspec(dllexport) long WINAPI TApiFracToNumerator (TfValue *v);

/*  TApiSetPrintf
 *
 *  Function to generate and redirect the debug output of the conversion
 *  library to a user defined function. It expects a printf() - compatibel
 *  function.
 *
 *  The following Example redirects the debug output to the standard printf
 *  function:
 *
 *      TApiSetPrintf ((int (*)(char *, ...))printf);
 *
 *  To disable the output call the function with a NULL pointer as an argument.
 */
__declspec(dllexport) void WINAPI TApiSetPrintf (int(*fp)(char *fmt, ...));

/*  TApiSetMarketFlag
 *
 *  Function to enable or disable a specific market center
 *
 *  market - market center (see TF_EXCHG_ID_... definitions in tapidef.h)
 *  flag   - 0  disable the market
 *           1  enable the market
 */
__declspec(dllexport) void WINAPI TApiSetMarketFlag (int market, int flag);

/*  TApiSetNewsFlag
 *
 *  Function to enable or disable a specific news source
 *
 *  source - news cource (see TF_EXCHG_ID_NEWS_... definitions in tapidef.h)
 *  flag   - 0  disable the news source
 *           1  enable the news source
 */
__declspec(dllexport) void WINAPI TApiSetNewsFlag (int source, int flag);

/*  TApiSetMessageFlag
 *
 *  Function to enable or disable a specific message type
 *
 *  msg   - message type (see TF_MSG_TYPE_... definitions in tapidef.h)
 *  flag  - 0  disable the message
 *          1  enable the message
 */
__declspec(dllexport) void WINAPI TApiSetMessageFlag (int msg, int flag);

/*  TApiSetSecurityFlag
 *
 *  Function to enable or disable a specific security type
 *
 *  sec   - security type (see TF_SEC_TYPE_... definitions in tapidef.h)
 *  flag  - 0  disable the security type
 *          1  enable the security type
 */
__declspec(dllexport) void WINAPI TApiSetSecurityFlag (int sec, int flag);

/*  TApiSetFieldFlag
 *
 *  Function to enable or disable a specific field type
 *
 *  field - field type (see TF_FIELD_... definitions in tapidef.h)
 *  flag  - 0  disable the field type
 *          1  enable the field type
 */
__declspec(dllexport) void WINAPI TApiSetFieldFlag (int field, int flag);

/*  TApiFieldToLong
 *
 *  Converts the value of any valid TF_VAL_TYPE_... into a
 *  long value.
 *
 *  NOTE: a return value of 0 might indicate that the conversion
 *        is not supplied.
 *
 *  f - pointer to the field structure containing the value
 */
__declspec(dllexport) long WINAPI TApiFieldToLong (TfField *f);

/*  TApiFieldToFloat
 *
 *  Converts the value of any valid TF_VAL_TYPE_... into a
 *  float value.
 *
 *  NOTE: a return value of 0.0 might indicate that the conversion
 *        is not supplied.
 *
 *  f - pointer to the field structure containing the value
 */
__declspec(dllexport) float WINAPI TApiFieldToFloat (TfField *f);

/*  TApiFieldToDouble
 *
 *  Converts the value of any valid TF_VAL_TYPE_... into a
 *  double value.
 *
 *  NOTE: a return value of 0.0 might indicate that the conversion
 *        is not supplied.
 *
 *  f - pointer to the field structure containing the value
 */
__declspec(dllexport) double WINAPI TApiFieldToDouble (TfField *f);

/*  TApiFieldToString
 *
 *  Copies the zero terminated string from a field with value
 *  type TF_VAL_TYPE_LPSTR into a application supplied buffer.
 *
 *  Parameters:
 *    f       - pointer to the field structure containing the string
 *    pBuffer - application supplied string buffer
 *
 *  Return value:
 *    len - string length  (> 0) of the coppied string
 *    0   - otherwise, no string copied
 *
 *  Note: Does not work with fields containing a value type other that
 *        TF_VAL_TYPE_LPSTR
 */
__declspec(dllexport) int WINAPI TApiFieldToString (TfField *f, char *pBuffer);

/*  TApiFieldToBinary
 *
 *  Copies the binary data from a field with value
 *  type TF_VAL_TYPE_BINARY into a application supplied buffer.
 *
 *  Parameters:
 *    f        - pointer to the field structure containing the binary data
 *    pBuffer  - application supplied buffer
 *    nMaxSize - maximum buffer size
 *
 *  Return value:
 *    len - data length in bytes (> 0) of the coppied data
 *    0   - otherwise, no data copied
 *
 *  Note: Does not work with fields containing a value type other that
 *        TF_VAL_TYPE_BINARY
 */
__declspec(dllexport) int WINAPI TApiFieldToBinary (TfField *f, char *pBuffer, int nMaxSize);

/*  TApiConvJulianDate
 *
 *  Converts a Julian date value (value type == TF_VAL_TYPE_JULIAN) into
 *  day, month, year (with 4 digits)
 *
 *  Parameters:
 *    dwJulDate - Julian Date value
 *    nDay      - pointer to day value
 *    nMonth    - pointer to month value
 *    nYear     - pointer to year value
 *
 *  Return value:
 *    1   - conversion applied, nDay, nMonth, nYear contain valid figures
 *    0   - conversion not possible, nDay, nMonth, nYear contain no figures
 */
__declspec(dllexport) int WINAPI TApiConvJulianDate (DWORD dwJulDate, int *nDay, int *nMonth, int *nYear);

/*  TApiGetNextField
 *
 *  Return the pointer to the next field structure in a TApi message
 *  read with TApiReadRecord().
 *
 *  This function should preferably be used to sequentially read all
 *  fields from a TApi message (instead of accessing the structures
 *  directly through pointers)
 *
 *  Parameters:
 *    pPacket   - pointer to the TApi message (read with TApiReadRecord())
 *    pCurField - pointer to the current field structure
 *                set it to NULL to retrieve the 1st field in the message
 *
 *  Return value:
 *    f    - pointer to next field if exists
 *    NULL - otherwise, no further field exists
 *
 *  Example:
 *
 *  char Buffer[1024];
 *  TfPacket *p;
 *  TfField  *f = NULL;
 *
 *  if (TApiReadRecord (Buffer)) {
 *      p = (TfPacket *)Buffer;
 *      f = NULL;   // start with the 1st field
 *      while ((f = TApiGetNextField (p, f)) != NULL) {
 *          // process the field
 *      }
 *  }
 *
 */
__declspec(dllexport) TfField *WINAPI TApiGetNextField (TfPacket *pPacket, TfField *pCurrField);

/*  TApiGetMessageType
 *
 *  Returns the message type of the TApi message
 *
 *  Parameters:
 *    pPacket   - pointer to the TApi message (read with TApiReadRecord())
 *
 *  Return value:
 *    msg - message type (TF_MSG_TYPE_... definitions in tapidef.h)
 *    0   - error (inv message type, pPacket undefined,...)
 */
__declspec(dllexport) int WINAPI TApiGetMessageType (TfPacket *pPacket);

/*  TApiGetSecurityType
 *
 *  Returns the security type of the TApi message
 *
 *  Parameters:
 *    pPacket   - pointer to the TApi message (read with TApiReadRecord())
 *
 *  Return value:
 *    sec - security type (TF_SEC_TYPE_... definitions in tapidef.h)
 *    0   - error (inv security type, pPacket undefined,...)
 */
__declspec(dllexport) int WINAPI TApiGetSecurityType (TfPacket *pPacket);

/*  TApiGetExchangeId
 *
 *  Returns the exchange id of the TApi message
 *
 *  Parameters:
 *    pPacket   - pointer to the TApi message (read with TApiReadRecord())
 *
 *  Return value:
 *    id  - exchange id (TF_EXCHG_ID_... definitions in tapidef.h)
 *    0   - error (inv exchange id, pPacket undefined,...)
 */
__declspec(dllexport) int WINAPI TApiGetExchangeId (TfPacket *pPacket);

/*  TApiGetMarketCenter
 *
 *  Returns the original market center of the TApi message
 *
 *  This field has only a value if the original market center is different from
 *  the exchange id (see TApiGetExchangeId ()). This might be the case for
 *  US options (e.g. exchange id == TF_EXCHG_ID_CBOE, market center == TF_EXCHG_ID_AMEX) or
 *  equities from german regional exchanges (e.g. exchange id == TF_EXCHG_ID_FFM,
 *  market center == TF_EXCHG_ID_MUNICH, ...)
 *
 *  Parameters:
 *    pPacket   - pointer to the TApi message (read with TApiReadRecord())
 *
 *  Return value:
 *    id  - market center (TF_EXCHG_ID_... definitions in tapidef.h)
 *    0   - no extra market center
 */
__declspec(dllexport) int WINAPI TApiGetMarketCenter (TfPacket *pPacket);

/*  TApiGetFieldCount
 *
 *  Returns the number of fields contained in the TApi message. Could be used to
 *  determine how many fields are contained in a TApi message.
 *
 *  Preferably the function TApiGetNextField() should be used to read the different
 *  fields sequentially (see above)
 *
 *  Parameters:
 *    pPacket   - pointer to the TApi message (read with TApiReadRecord())
 *
 *  Return value:
 *    nFields - # of attached fields
 *    0       - no fields in the message
 */
__declspec(dllexport) int WINAPI TApiGetFieldCount (TfPacket *pPacket);

/*  TApiGetSymbol
 *
 *  Returns the symbol to be updated with the following fields
 *  The combination of symbol, exchange id and security type defines a unique instrument
 *  in the feed.
 *
 *  NOTE: The symbol is NOT attached as a field to the message but always included in
 *        the message header although the symbol has also s field id (TF_FIELD_SYMBOL).
 *
 *  Parameters:
 *    pPacket   - pointer to the TApi message (read with TApiReadRecord())
 *
 *  Return value:
 *    len     - length in characters of the symbol
 *    0       - no symbol name in the header or parameter error
 */
__declspec(dllexport) int WINAPI TApiGetSymbol (TfPacket *pPacket, char *pSymbol);

/*  TApiGetFieldId
 *
 *  Returns the field id of a TApi field
 *
 *  Parameters:
 *    pField - pointer to the TApi field structure
 *
 *  Return value:
 *    id  - field id (see TF_FIELD_... in tapidef.h)
 *    -1  - inv. field id or parameter error
 */
__declspec(dllexport) int WINAPI TApiGetFieldId (TfField *pField);

/*  TApiGetValueType
 *
 *  Returns the value type of the value in a TApi field
 *
 *  Parameters:
 *    pField - pointer to the TApi field structure
 *
 *  Return value:
 *    type - value type (see TF_VAL_TYPE_... in tapidef.h)
 *    0    - inv. value type or parameter error
 */
__declspec(dllexport) int WINAPI TApiGetValueType (TfField *pField);

/*  TApiGetValueBuffer
 *
 *  COPIES the data part of a TApi field into the application supplied buffer
 *  depending on the value type. The function also return the field id and
 *  value type of the field.
 *
 *  An application using this function has to check the field id and value type
 *  in order to correctly interpret the returned data.
 *
 *  Examples:
 *    a) if *pValType == TF_VAL_TYPE_CHAR
 *       pValue points to a 1 byte signed character value,
 *    b) if *pValType == TF_VAL_TYPE_LONG
 *       pValue points to a 4 byte signed long value,
 *    c) if *pValType == TF_VAL_TYPE_FLOAT
 *       pValue points to a 4 byte float value,
 *    d) if *pValType == TF_VAL_TYPE_DOUBLE
 *       pValue points to a 8 byte double value,
 *    e) if *pValType == TF_VAL_TYPE_LPSTR
 *       pValue points to a zero terminated string
 *    f) if *pValType == TF_VAL_TYPE_FRAC
 *       pValue points to a 4 byte frac value
 *       use TfFracTo...() functions to interpret the value
 *       (see definition of the frac representation in tapidef.h)
 *
 *  Parameters:
 *    pField   - (in) pointer to the TApi field structure
 *    pFieldId - (out) contains the field id on return
 *    pValType - (out) contains the value type on return
 *    pValue   - (out) contains the data itself
 *
 *  Return value:
 *    size - # of valid bytes copied to the application supplied buffer pValue
 *    0    - no data returned
 */
__declspec(dllexport) int WINAPI TApiGetValueBuffer (TfField *pField, int *pFieldId, int *pValType, void *pValue);


/*------------------------------------------------------------------------
 *  Function prototypes for ClientApi interface
 *-----------------------------------------------------------------------*/

/*  CApiInitialize
 *
 *  Initialize and enable the communication with the QuoteSpeed workstation.
 *  Should be used instead of TApiInitialize() to receive data from a QuoteSpeed
 *  workstation
 *
 *  nMaxBufSize - the max size (in bytes) of the conversion buffer
 *                supplied by the main application.
 *                A default of 1024 bytes is assumed if the parameter
 *                is set to 0.
 *
 *  return value - 1 on success
 *                 0 on any failure
 */
__declspec(dllexport) int WINAPI CApiInitialize (int nMaxBufSize);

/*  CApiShutDown
 *
 *  Disable the communication with the QuoteSpeed workstation.
 */
__declspec(dllexport) void WINAPI CApiShutDown (void);

/*  CApiSymbolRequest
 *
 *	Requests realtime and fundamental information and activates or
 *  deactivates realtime update of symbol. The structure
 *  TfCApiSymbolRequest defined in tapidef.h has to be populated befor the call.
 */
__declspec(dllexport) void WINAPI CApiSymbolRequest (TfCApiSymbolRequest *pRequest);

/*  CApiDbaseRequest
 *
 *  Requests database information from the server database. The database
 *	is organized by countries and not by markets. The database is not
 *	able to distinguish between stocks, indices, funds and warrants.
 *
 */
__declspec(dllexport) void WINAPI CApiDbaseRequest (TfCApiDbaseRequest *pRequest);

/*  TApiLoadFeedFile
 *
 *  Inform the FeedServer to load the raw binary feed from the
 *  specified file
 *
 *  szFeedFile - name of the file containing raw feed data
 *
 *  NOTE: FOR INTERNAL USE ONLY
 */
__declspec(dllexport) void WINAPI TApiLoadFeedFile (char *szFeedFile);

/*  TApiSendCommand
 *
 *  Send a command to the TApi/Capi server.
 *
 */
__declspec(dllexport) int WINAPI TApiSendCommand (char *szCommand);

/*  TApiPtrToOff
 *
 *  convert all pointers in the tapi message to buffer offsets
 *  to allow to copy the complete buffer.
 *  the conversion is done in place.
 *
 *  szBuffer - data buffer holding the complete Tapi message
 */
__declspec(dllexport) int WINAPI TApiPtrToOff (char *szBuffer);

/*  TApiOffToPtr
 *
 *  convert all offsets in the tapi message back to pointers
 *  the conversion is done in place.
 *
 *  szBuffer - data buffer holding the complete Tapi message
 */
__declspec(dllexport) int WINAPI TApiOffToPtr (char *szBuffer);

/*  TApiDataToString
 *
 *  convert the comlete message into a text string
 *
 *  szData   - original Tapi data buffer
 *  szDest   - destination buffer for the string
 *  nMaxSize - max size of the destination buffer (to protect against overwriting)
 */
__declspec(dllexport) int WINAPI TApiDataToString (char *szData, char *szDest, int nMaxSize);

#endif


