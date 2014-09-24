/* 
**** A-T Debug Window
*/

#if !defined(INCL_DBWIN)
#define INCL_DBWIN

void OutputDebugMessage(char * lpszFormat, ...);
void OutputDebugHexDump(LPBYTE inBuffer,UINT inLen);

#endif
