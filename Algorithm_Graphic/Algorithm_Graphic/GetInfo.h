#ifndef _GETINFO_H_
#define _GETINFO_H_

extern int lineNum;
extern LONG offset;
extern LONG WriteOffset;

int GetLineNum(const TCHAR* path);
FileInfo GetData( const TCHAR* path);

extern int end;
extern int fileend;

#endif