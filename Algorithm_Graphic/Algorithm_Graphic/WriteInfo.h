#ifndef _WRITEINFO_H_
#define _WRITEINFO_H_

void MyWriteFile( int lineNum , TCHAR *dataNew ,const TCHAR* path );
void WriteFileInfo( int lineNum , FileInfo dataNew ,const TCHAR* path );
void GenerateFileName( TCHAR* str );

#endif